/*
 * Copyright (c) 2009, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <common.h>
#include <malloc.h>
#include <android_bootimg.h>
#include <boot_mode.h>
#include <dl_common.h>
#include "sparse_format.h"
#include <linux/usb/usb_uboot.h>
#include <sprd_common_rw.h>

#include "dl_operate.h"
#include <asm/arch/chip_releted_def.h>

#include <secureboot/sec_common.h>


extern SPECIAL_PARTITION_CFG const s_special_partition_cfg[];
extern uchar *_get_backup_partition_name(uchar * partition_name);

#ifdef CONFIG_SECURE_BOOT
#include "secure_verify.h"
#endif

#ifdef CONFIG_NAND_BOOT
#include <nand.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>

typedef struct {
	char *vol;
	char *bakvol;
} FB_NV_VOL_INFO;

static FB_NV_VOL_INFO s_nv_vol_info[] = {
	{"fixnv1", "fixnv2"},
	{"wfixnv1", "wfixnv2"},
	{"tdfixnv1", "tdfixnv2"},
	{NULL, NULL}
};

#endif

struct dl_image_inf {
	uint8_t *base_address;
	uint64_t max_size;
	uint64_t data_size;
};

#define ROUND_TO_PAGE(x,y) (((x) + (y)) & (~(y)))

struct fastboot_cmd {
	struct fastboot_cmd *next;
	const char *prefix;
	unsigned prefix_len;
	void (*handle) (const char *arg, void *data, uint64_t sz);
};

struct fastboot_var {
	struct fastboot_var *next;
	const char *name;
	const char *value;
};

static struct fastboot_cmd *cmdlist;
struct dl_image_inf ImageInfo;

static unsigned char buffer[4096] __attribute__((aligned(64)));

typedef enum {
	STATE_OFFLINE = 0,
	STATE_COMMAND,
	STATE_COMPLETE,
	STATE_ERROR
} FB_USB_STATE;

static unsigned fastboot_state = STATE_OFFLINE;

extern int usb_fastboot_init(void);
extern int fb_usb_write(void *buf, unsigned len);
extern int fb_usb_read(void *_buf, unsigned len);

extern void power_down_devices(unsigned pd_cmd);
extern void reboot_devices(unsigned reboot_mode);
extern void splFillCheckData(uchar * splBuf);

#ifdef CONFIG_X86
unsigned char *g_FbBuf = (unsigned char *)0x35000000;
#else
unsigned char *g_FbBuf = (unsigned char *)0x82000000;
#endif

void fastboot_register(const char *prefix, void (*handle) (const char *arg, void *data, uint64_t sz))
{
	struct fastboot_cmd *cmd;
	cmd = malloc(sizeof(*cmd));
	if (cmd) {
		cmd->prefix = prefix;
		cmd->prefix_len = strlen(prefix);
		cmd->handle = handle;
		cmd->next = cmdlist;
		cmdlist = cmd;
	}
}

static struct fastboot_var *varlist;

void fastboot_publish(const char *name, const char *value)
{
	struct fastboot_var *var;
	var = malloc(sizeof(*var));
	if (var) {
		var->name = name;
		var->value = value;
		var->next = varlist;
		varlist = var;
	}
}
static char response[64] __attribute__((aligned(64)));
void fastboot_ack(const char *code, const char *reason)
{

	if (fastboot_state != STATE_COMMAND)
		return;
	if (reason == 0)
		reason = "";

	//snprintf(response, 64, "%s%s", code, reason);
	if (strlen(code) + strlen(reason) >= 64) {
		debugf("too long string\r\n");
	}
	sprintf(response, "%s%s", code, reason);
	fastboot_state = STATE_COMPLETE;

	fb_usb_write(response, strlen(response));

}

void fastboot_fail(const char *reason)
{
	fastboot_ack("FAIL", reason);
}

void fastboot_okay(const char *info)
{
	fastboot_ack("OKAY", info);
}

static void fb_cmd_getvar(const char *arg, void *data, uint64_t sz)
{
	struct fastboot_var *var;

	for (var = varlist; var; var = var->next) {
		if (!strcmp(var->name, arg)) {
			fastboot_okay(var->value);
			return;
		}
	}
	fastboot_okay("");
}

static void fb_cmd_download(const char *arg, void *data, uint64_t sz)
{
	char response[64];
	uint64_t len = simple_strtoul(arg, NULL, 16);
	int total_rcv;

	debugf("Start fastboot download, image len=0x%llx \n", len);
	debugf("buffer base %p, buffer size 0x%llx \n", ImageInfo.base_address, ImageInfo.max_size);
	if (len > ImageInfo.max_size) {
		debugf("Image size over the max buffer size,can not accept \n");
		fastboot_fail("data too large");
		return;
	}

	sprintf(response, "DATA%08x", len);
	if (fb_usb_write(response, strlen(response)) < 0) {
		fastboot_state = STATE_ERROR;
		return;
	}

	total_rcv = fb_usb_read(ImageInfo.base_address, len);
	if (total_rcv != len) {
		fastboot_state = STATE_ERROR;
		return;
	}
	ImageInfo.data_size = len;
	fastboot_okay("");
}

int _fb_write_nv_img(char * partition_name, uint64_t size)
{
	char *backup_partition_name = NULL;
	uint8_t header_buf[NV_HEAD_LEN];
	nv_header_t *nv_header_p = NULL;
	void *buf = ImageInfo.base_address;
	int32_t i;

	memset(header_buf, 0x00, NV_HEAD_LEN);
	nv_header_p = header_buf;
	nv_header_p->magic = NV_HEAD_MAGIC;
	nv_header_p->len = size;
	nv_header_p->checksum = (uint32_t) fdl_calc_checksum(buf, size);
	nv_header_p->version = NV_VERSION;

	debugf("Start to write NV partition(%s)\n", partition_name);
	if (0 != common_raw_write(partition_name, NV_HEAD_LEN, (uint64_t)0, (uint64_t)0, header_buf))
		return 0;

	if (0 != common_raw_write(partition_name, size, (uint64_t)0, NV_HEAD_LEN, buf))
		return 0;

	/*write the backup partition */
	backup_partition_name = _get_backup_partition_name(partition_name);
	if (NULL == backup_partition_name) {
		errorf(" get backup partition name fail\n");
		return 0;
	}
	debugf("Start to write backup partition(%s)\n", backup_partition_name);
	if (0 != common_raw_write(backup_partition_name, NV_HEAD_LEN, (uint64_t)0, (uint64_t)0, header_buf))
		return 0;
	if (0 != common_raw_write(backup_partition_name, size, (uint64_t)0, NV_HEAD_LEN, buf))
		return 0;

	return 1;
}


int _fb_download_image(char *partition_name,ulong offset)
{
	int retval;
	int i;
	uint64_t write_size = 0;
	uint64_t img_format = IMG_RAW;
	void *buf = ImageInfo.base_address;
	PARTITION_PURPOSE partition_purpose = PARTITION_PURPOSE_NORMAL;
	sparse_header_t *sparse_header;

	if (0 == strcmp("splloader", partition_name)) {
		splFillCheckData(buf);
		write_size = max(ImageInfo.data_size, SPL_CHECKSUM_LEN);
	} else {
		write_size =ImageInfo.data_size;
	}

	/*get the special partition info */
	for (i = 0; NULL != s_special_partition_cfg[i].partition; i++) {
		if (strcmp(s_special_partition_cfg[i].partition, partition_name) == 0) {
			partition_purpose = s_special_partition_cfg[i].purpose;
			img_format = s_special_partition_cfg[i].imgattr;
			break;
		}
	}

	sparse_header = (sparse_header_t *)(ImageInfo.base_address+offset);
	if ((sparse_header->magic == SPARSE_HEADER_MAGIC) && (sparse_header->major_version == SPARSE_HEADER_MAJOR_VER)) {
		img_format = IMG_WITH_SPARSE;
		debugf("img_format =FB_IMG_WITH_SPARSE\n");
	}

	if (IMG_WITH_SPARSE == img_format) {
		debugf("Handle the saving of image with sparse,name=%s,buf start at 0x%p,size=0x%llx\n",
		       partition_name, buf, write_size);
		retval = write_sparse_img(partition_name, buf, (ulong)write_size);
		if (-1 == retval) {
			errorf("Write sparse img fail\n");
			return 0;
		}
		debugf("Write packed img success,return value=%d\n", retval);
	} else if (PARTITION_PURPOSE_NV == partition_purpose) {
		if (!_fb_write_nv_img(partition_name, FIXNV_SIZE)) {
			errorf("Write nv img fail\n");
			return 0;
		}
	} else {
		if (0 != common_raw_write(partition_name, write_size, (uint64_t)0, (uint64_t)0, buf)) {
			errorf("write %s size 0x%llx fail\n", partition_name, write_size);
			return 0;
		}
	}
	debugf("cmd_flash: success!\n");
	return 1;
}

OPERATE_STATUS _fb_img_write_with_backup(char *partition_name)
{
	uint64_t write_size = ImageInfo.data_size;
	char *buf = ImageInfo.base_address;
	if (0 == strcmp("splloader",  partition_name)) {
		write_size = max(write_size, SPL_CHECKSUM_LEN);
		if (0 != common_raw_write(partition_name, write_size, (uint64_t)0, (uint64_t)0, buf))
			return OPERATE_WRITE_ERROR;
		strcat(partition_name, "_bak");
		if (0 != common_raw_write(partition_name, write_size, (uint64_t)0, (uint64_t)0, buf))
			return OPERATE_WRITE_ERROR;
	} else if (0 == strcmp("uboot",  partition_name)
		|| 0 == strcmp("trustos",partition_name)
		|| 0 == strcmp("sml",partition_name)) {
		if (0 != common_raw_write(partition_name, write_size, (uint64_t)0, (uint64_t)0, buf))
			return OPERATE_WRITE_ERROR;
		strcat(partition_name, "_bak");
		if (0 != common_raw_write(partition_name, write_size, (uint64_t)0, (uint64_t)0, buf))
			return OPERATE_WRITE_ERROR;
	}
	  else {
		return OPERATE_IGNORE;
	}
	debugf("cmd_flash: success!\n");
	return OPERATE_SUCCESS;
}

void fb_cmd_flash(const char *arg, void *data, uint64_t sz)
{
	int i;
	OPERATE_STATUS status = 0;
	char partition_name[PARTNAME_SZ];
	char *partition_name_p = NULL;
	ulong strip_header = 0;
	ulong sec_offset=0x200;
	static sys_img_header *bakup_header;

	debugf("data = %p,ImageInfo[0].base_address = %p\n", data, ImageInfo.base_address);

	for (i = 0; i < PARTNAME_SZ; i++) {
		partition_name[i] = arg[i];
		if (0 == arg[i])
			break;
	}
	partition_name_p = partition_name;
#if defined (CONFIG_SECURE_BOOT) ||defined (CONFIG_SECBOOT)
	VERIFY_RESULT verify_res;
	verify_res = fb_secure_process_flow(&strip_header, partition_name,NULL, NULL, data);
	switch (verify_res) {
	case VERIFY_FAIL:
		if (0 == strcmp("system", partition_name))
			common_raw_erase(partition_name, 0, 0);
		return OPERATE_SYSTEM_ERROR;
	/*for future extension*/
	case VERIFY_STOP_WRITE:
	case VERIFY_NO_NEED:
	case VERIFY_OK:
	default:
		break;
	}
#endif
	//for spl uboot bakup
	bakup_header = malloc(sizeof(sys_img_header));
	if (NULL == bakup_header) {
		errorf("No space to store bakup_header!\n");
		return ;
	}

	bakup_header = (sys_img_header *)(ImageInfo.base_address);
	debugf("bakup_header->mMagicNum=0x%x\n", bakup_header->mMagicNum);
	if(bakup_header->mMagicNum == IMG_BAK_HEADER) {
		status=_fb_img_write_with_backup(partition_name_p);
		if(OPERATE_WRITE_ERROR==status) {
			errorf("image write with backup error!\n");
			return ;
		}
		else if(OPERATE_IGNORE==status) {
			if(!_fb_download_image(partition_name_p,sec_offset)) {
				errorf("image write error!\n");
				return ;
			}
		}
	}
	else if(!_fb_download_image(partition_name_p,0)) {
		errorf("image write error!\n");
		return ;
	}
	fastboot_okay("");
}

void fb_cmd_erase(const char *arg, void *data, uint64_t size)
{
	int i;
	char partition_name[PARTNAME_SZ];
	char *partition_name_p = NULL;
	uchar *backup_partition_name = NULL;

	for (i = 0; i < PARTNAME_SZ; i++) {
		partition_name[i] = arg[i];
		if (0 == arg[i])
			break;
	}
	partition_name_p = partition_name;
	if ((0 == strcmp(partition_name, "erase_all")) && (0xffffffff == size)) {
		debugf("Erase all!\n");
		if (0 != common_raw_erase(partition_name_p, 0, 0))
			return ;
	} else {
		debugf("erase partition %s!\n", partition_name_p);

		if (0 != common_raw_erase(partition_name_p, size, 0)) {
			return ;
		}

		/* erase backup partition */
		backup_partition_name = _get_backup_partition_name(partition_name_p);

		if (NULL != backup_partition_name) {
			if (0 != common_raw_erase(backup_partition_name, size, 0))
				return ;
		}
	}

	debugf("Cmd Erase OK\n");
	fastboot_okay("");
	return;
}

/*for nand this can not work,TODO*/
#ifdef CONFIG_NAND_BOOT
void fb_cmd_flash(const char *arg, void *data, uint64_t sz)
{
	int ret = -1;
	int i;
	uint64_t *code_addr;
	debugf("arg:%x date: 0x%x, sz 0x%x\n", arg, data, sz);

	if (!strcmp(arg, "boot") || !strcmp(arg, "recovery")) {
#ifdef CONFIG_SECURE_BOOT
		code_addr = (uint64_t *) get_code_addr(NULL, data);
		if (memcmp((void *)code_addr, BOOT_MAGIC, BOOT_MAGIC_SIZE)) {
			fastboot_fail("image is not a boot image");
			return;
		}
		secure_verify("uboot", data, 0);
#else
		if (memcmp((void *)data, BOOT_MAGIC, BOOT_MAGIC_SIZE)) {
			fastboot_fail("image is not a boot image");
			return;
		}
#endif
	}
#ifdef CONFIG_SECURE_BOOT
	if (strcmp(arg, "spl") == 0) {
		secure_verify("splloader0", data, 0);
	} else if (strcmp(arg, "2ndbl") == 0) {
		secure_verify("splloader", data, 0);
	}
#endif

	/**
	 *	FIX ME!
	 *	assume first image buffer is big enough for nv
	 */
	for (i = 0; s_nv_vol_info[i].vol != NULL; i++) {
		if (!strcmp(arg, s_nv_vol_info[i].vol)) {
			nv_header_t *header = NULL;
			uint8_t tmp[NV_HEAD_LEN];

			memset(tmp, 0x00, NV_HEAD_LEN);
			header = tmp;
			header->magic = NV_HEAD_MAGIC;
			header->len = FIXNV_SIZE;
			header->checksum = (uint32_t) calc_checksum((unsigned char *)data, FIXNV_SIZE);
			header->version = NV_VERSION;
			/*write org nv */
			ret = do_raw_data_write(arg, FIXNV_SIZE + NV_HEAD_LEN, NV_HEAD_LEN, 0, tmp);
			if (ret)
				goto end;
			ret = do_raw_data_write(arg, 0, FIXNV_SIZE, NV_HEAD_LEN, data);
			if (ret)
				goto end;
			/*write bak nv */
			ret = do_raw_data_write(s_nv_vol_info[i].bakvol, FIXNV_SIZE + NV_HEAD_LEN, NV_HEAD_LEN, 0, tmp);
			if (ret)
				goto end;
			ret = do_raw_data_write(s_nv_vol_info[i].bakvol, 0, FIXNV_SIZE, NV_HEAD_LEN, data);
			goto end;
		}
	}

	if (ImageInfo[1].data_size) {
		uint32_t total_sz = ImageInfo[0].data_size + ImageInfo[1].data_size;
		ret = do_raw_data_write(arg, total_sz, ImageInfo[0].data_size, 0, ImageInfo[0].base_address);
		if (ret)
			goto end;
		ret = do_raw_data_write(arg, 0, ImageInfo[1].data_size, ImageInfo[0].data_size, ImageInfo[1].base_address);
	} else {
		ret = do_raw_data_write(arg, ImageInfo[0].data_size, ImageInfo[0].data_size, 0, ImageInfo[0].base_address);
	}

end:
	if (!ret)
		fastboot_okay("");
	else
		fastboot_fail("flash error");
	return;
}

void fb_cmd_erase(const char *arg, void *data, uint64_t sz)
{
	struct mtd_info *nand;
	struct mtd_device *dev;
	struct part_info *part;
	nand_erase_options_t opts;
	u8 pnum;
	int ret;
	char buf[1024];

	debugf("\n");

	ret = find_dev_and_part(arg, &dev, &pnum, &part);
	if (!ret) {
		nand = &nand_info[dev->id->num];
		memset(&opts, 0, sizeof(opts));
		opts.offset = (loff_t) part->offset;
		opts.length = (loff_t) part->size;
		opts.jffs2 = 0;
		opts.quiet = 1;
		ret = nand_erase_opts(nand, &opts);
		if (ret)
			goto end;
	}

	/*just erase 1k now */
	memset(buf, 0x0, 1024);
	ret = do_raw_data_write(arg, 1024, 1024, 0, buf);

end:
	if (ret)
		fastboot_fail("nand erase error");
	else
		fastboot_okay("");
	return;
}
#endif

void boot_linux(unsigned kaddr, unsigned taddr)
{
	void (*theKernel) (void *dtb_addr, int zero, int arch, int reserved) = (void*)kaddr;

	invalidate_dcache_all();

	theKernel(DT_ADR, 0, 0, 0);
}

extern unsigned char raw_header[8192];

void fb_cmd_boot(const char *arg, void *data, uint64_t sz)
{
	boot_img_hdr *hdr = (boot_img_hdr *)raw_header;
	unsigned kernel_actual;
	unsigned ramdisk_actual;

	if (memcmp(hdr->magic, BOOT_MAGIC, BOOT_MAGIC_SIZE)) {
		debugf("boot image headr: %s\n", hdr->magic);
		fastboot_fail("bad boot image header");
		return;
	}
	kernel_actual = ROUND_TO_PAGE(hdr->kernel_size, (KERNL_PAGE_SIZE - 1));
	if (kernel_actual <= 0) {
		fastboot_fail("kernel image should not be zero");
		return;
	}
	ramdisk_actual = ROUND_TO_PAGE(hdr->ramdisk_size, (KERNL_PAGE_SIZE - 1));
	if (0 == ramdisk_actual) {
		fastboot_fail("ramdisk size error");
		return;
	}

	memcpy((void *)hdr->kernel_addr, (void *)data + KERNL_PAGE_SIZE, kernel_actual);
	memcpy((void *)hdr->ramdisk_addr, (void *)data + KERNL_PAGE_SIZE + kernel_actual, ramdisk_actual);

	debugf("kernel @0x%08x (0x%08x bytes)\n", hdr->kernel_addr, kernel_actual);
	debugf("ramdisk @0x%08x (0x%08x bytes)\n", hdr->ramdisk_addr, ramdisk_actual);

	fastboot_okay("");
	usb_driver_exit();
	boot_linux(hdr->kernel_addr, hdr->tags_addr);
}

void fb_cmd_continue(const char *arg, void *data, uint64_t sz)
{
	fastboot_okay("");
	usb_driver_exit();
	normal_mode();
}

void fb_cmd_reboot(const char *arg, void *data, uint64_t sz)
{
	fastboot_okay("");
	reboot_devices(CMD_NORMAL_MODE);
}

void fb_cmd_reboot_bootloader(const char *arg, void *data, uint64_t sz)
{
	fastboot_okay("");
	usb_driver_exit();
	reboot_devices(CMD_FASTBOOT_MODE);
}

void fb_cmd_powerdown(const char *arg, void *data, uint64_t sz)
{
	fastboot_okay("");
	power_down_devices(0);

}

static void fastboot_command_loop(void)
{
	struct fastboot_cmd *cmd;
	int r;
	debugf("fastboot: processing commands\n");

again:
	while (fastboot_state != STATE_ERROR) {
		memset(buffer, 0, 64);
		r = fb_usb_read(buffer, 64);
		if (r < 0)
			break;
		buffer[r] = 0;
		debugf("fastboot: %s, r:%d\n", buffer, r);

		for (cmd = cmdlist; cmd; cmd = cmd->next) {
			if (memcmp(buffer, cmd->prefix, cmd->prefix_len))
				continue;
			debugf("Receive cmd from host :%s \n", cmd->prefix);
			fastboot_state = STATE_COMMAND;
			cmd->handle((const char *)buffer + cmd->prefix_len, ImageInfo.base_address, ImageInfo.data_size);
			if (fastboot_state == STATE_COMMAND)
				fastboot_fail("unknown reason");
			goto again;
		}

		fastboot_fail("unknown command");

	}
	fastboot_state = STATE_OFFLINE;
	debugf("fastboot: oops!\n");
}

static int fastboot_handler(void *arg)
{
	for (;;) {
		fastboot_command_loop();
	}
	return 0;
}

int do_fastboot(void)
{
	int ret = 0;
	debugf("start fastboot\n");

	ret = usb_fastboot_init();
	if (ret < 0)
		return ret;

	ImageInfo.base_address = g_FbBuf;
	/*except the u-boot occupied area , all RAM can be used as fastboot buffer */
	ImageInfo.max_size = (uint64_t) (CONFIG_SYS_TEXT_BASE - (uint64_t)g_FbBuf);
	ImageInfo.data_size = 0;

	fastboot_register("getvar:", fb_cmd_getvar);
	/*when you input cmd"flash" in host, we will rcv cmd"download" first,then the "flash",
	   so even if we can't see cmd"download" in host fastboot cmd list,it is also used */
	fastboot_register("download:", fb_cmd_download);
	fastboot_publish("version", "1.0");

	fastboot_register("flash:", fb_cmd_flash);
	fastboot_register("erase:", fb_cmd_erase);
	fastboot_register("boot", fb_cmd_continue);
	fastboot_register("reboot", fb_cmd_reboot);
	fastboot_register("powerdown", fb_cmd_powerdown);
	fastboot_register("continue", fb_cmd_continue);
	fastboot_register("reboot-bootloader", fb_cmd_reboot_bootloader);

	fastboot_handler(0);

	return 0;

}
