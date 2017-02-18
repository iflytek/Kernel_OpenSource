#include <common.h>
#include <stdio_dev.h>
#include <linux/ctype.h>
#include <linux/types.h>
#include <asm/global_data.h>
#include <libfdt.h>
#include <fdt_support.h>
#include <exports.h>
#include "loader_common.h"
#include "sprd_cpcmdline.h"
#include "sysdump.h"
#include "sprd_common_rw.h"
#include <adi_hal_internal.h>
#include <asm/arch/sprd_reg.h>
#include <uboot_sec_drv.h>
#include <fdtdec.h>

extern void *lcd_get_base_addr(void *lcd_base);
extern uint32_t load_lcd_id_to_kernel();
extern uint32_t load_lcd_width_to_kernel();
extern uint32_t load_lcd_hight_to_kernel();
extern int get_dram_cs_number(void);
extern int get_dram_cs0_size(void);
extern char *get_product_sn(void);

/*for verified boot*/
enVerifiedState g_verifiedbootstate = v_state_green;
extern unsigned int g_fb_lockflag;

#ifdef CONFIG_SECURE_BOOT
#define CMDLINE_BUF_SIZE	(1024)
unsigned spl_data_buf[0x2000] __attribute__ ((align(4))) = {
0};

unsigned harsh_data_buf[8] __attribute__ ((align(4))) = {
0};

void *spl_data = spl_data_buf;
void *harsh_data = harsh_data_buf;
#endif

typedef enum {
	TOS_MEMORY = 0,
	MULTIMEDIA,
	AUDIO_MEMORY,
	SIPC_MEMORY,
	CP_MODEM,
	FRAMEBUFFER,
	OVERLAYBUFFER
} mem_type;

typedef struct {
	mem_type type;
	uint32_t start_high;
	uint32_t start_low;
	uint32_t size;
} mem_info;

int fdt_fixup_verified_boot(void *fdt)
{
	int ret = 0;
	char buf[64];
	int str_len;
	memset(buf, 0, 64);

	switch(g_verifiedbootstate)
	{
		case v_state_green:
			sprintf(buf,"androidboot.verifiedbootstate=green");
			break;
		case v_state_yellow:
			sprintf(buf,"androidboot.verifiedbootstate=yellow");
			break;
		case v_state_orange:
			sprintf(buf,"androidboot.verifiedbootstate=orange");
			break;
		case v_state_red:
			sprintf(buf,"androidboot.verifiedbootstate=red");
			break;
		default:
			sprintf(buf,"androidboot.verifiedbootstate=green");
			break;
	}

	str_len = strlen(buf);
	buf[str_len] = '\0';
	ret = fdt_chosen_bootargs_append(fdt, buf, 1);

	return ret;
}

int fdt_fixup_flash_lock_state(void *fdt)
{
	int ret = 0;
	char buf[64];
	int str_len;
	memset(buf, 0, 64);

	switch(g_fb_lockflag)
	{
		case 1:
			sprintf(buf,"androidboot.flash.locked=0");
			break;
		case 0:
			sprintf(buf,"androidboot.flash.locked=1");
			break;
		default:
			sprintf(buf,"androidboot.flash.locked=1");
			break;
	}

	str_len = strlen(buf);
	buf[str_len] = '\0';
	ret = fdt_chosen_bootargs_append(fdt, buf, 1);

	return ret;
}

int fdt_initrd_norsvmem(void *fdt, ulong initrd_start, ulong initrd_end, int force)
{
	int nodeoffset;
	int err;
	unsigned long tmp=0;
	const char *path;
	uint64_t addr, size;

	/* Find the "chosen" node.  */
	nodeoffset = fdt_path_offset(fdt, "/chosen");

	/* If there is no "chosen" node in the blob return */
	if (nodeoffset < 0) {
		errorf("fdt_initrd: %s\n", fdt_strerror(nodeoffset));
		return nodeoffset;
	}

	/* just return if initrd_start/end aren't valid */
	if ((initrd_start == 0) || (initrd_end == 0))
		return 0;

	path = fdt_getprop(fdt, nodeoffset, "linux,initrd-start", NULL);
	if ((path == NULL) || force) {
#ifdef CONFIG_ARM64
		tmp = __cpu_to_be64(initrd_start);
#else
		tmp = __cpu_to_be32(initrd_start);
#endif
		err = fdt_setprop(fdt, nodeoffset, "linux,initrd-start", &tmp, sizeof(tmp));
		if (err < 0) {
			errorf("could not set linux,initrd-start %s.\n", fdt_strerror(err));
			return err;
		}
#ifdef CONFIG_ARM64
		tmp = __cpu_to_be64(initrd_end);
#else
		tmp = __cpu_to_be32(initrd_end);
#endif
		err = fdt_setprop(fdt, nodeoffset, "linux,initrd-end", &tmp, sizeof(tmp));
		if (err < 0) {
			errorf("could not set linux,initrd-end %s.\n", fdt_strerror(err));

			return err;
		}
	}

	return 0;
}

int fdt_chosen_bootargs_append(void *fdt, char *append_args, int force)
{
	int nodeoffset;
	int err;
	const char *path;
	char *strargs;

	if (!append_args)
		return -1;

	err = fdt_check_header(fdt);
	if (err < 0) {
		errorf("fdt_chosen_bootargs_append: %s\n", fdt_strerror(err));
		return err;
	}

	/*
	 * Find the "chosen" node.
	 */
	nodeoffset = fdt_path_offset(fdt, "/chosen");

	/*
	 * If there is no "chosen" node in the blob, leave.
	 */
	if (nodeoffset < 0) {
		errorf("fdt_chosen_bootargs_append: cann't find chosen");
		return -1;
	}

	/*
	 * If the property exists, update it only if the "force" parameter
	 * is true.
	 */
	path = fdt_getprop(fdt, nodeoffset, "bootargs", NULL);
	if ((path == NULL) || force) {
		strargs = malloc(1024);
		if (!strargs)
			return -1;
		memset(strargs, 0, 1024);
		if(path != NULL)
		{
		    sprintf(strargs, "%s %s", path, append_args);
		}
		else
		{
		    sprintf(strargs, "%s", append_args);
		}
		err = fdt_setprop(fdt, nodeoffset, "bootargs", strargs, strlen(strargs) + 1);
		if (err < 0)
			errorf("could not set bootargs %s.\n", fdt_strerror(err));
		free(strargs);
	}

	return err;
}

int uboot_set_secure_range_paraneters(mem_type type,fdt_addr_t start,fdt_addr_t size){
	uint32_t ret;
	uint32_t smcid;
	uint32_t funcid;
	mem_info mi;

	smcid  = TEESMC_SIPCALL_WITH_ARG;
	funcid = FUNCTYPE_SET_SECURE_RANGE_PARAM;
	struct smc_param param;
	mi.type = type;
	mi.start_high = (start >> 32);
	mi.start_low = (start & 0xffffffff);
	mi.size = size;
	param.a0 = smcid;
	param.a1 = funcid;
	param.a2 = &mi;
	param.a3 = sizeof(mem_info);
#ifdef CONFIG_ARM64
	tee_smc_call(&param);
#endif
	ret = param.a0;
	return ret;
}

int fdt_reserved_mem_multimedia_parse(void*fdt)
{
	int parentoffset,nodeoffset;
	int offset;
	uchar * dt_addr;
	const fdt32_t *reg;
	int str_len;
	char nodename[256];
	fdt_addr_t  s_addr;
	fdt_addr_t  s_size ;
	char *pStr=NULL;
	mem_info mi;

	parentoffset = fdt_path_offset(fdt, "/reserved-memory");
	if (parentoffset < 0) {
		errorf("fdt_reserved_mem_multimedia_parse: cann't find mm_reserved \n");
		return -1;
	}
	for (offset = fdt_first_subnode(fdt, parentoffset);
	      offset >= 0; offset = fdt_next_subnode(fdt, offset)) {
		sprintf(nodename, "%s", fdt_get_name(fdt, offset, NULL));
		str_len = strlen(nodename);
		nodename[str_len] = '\0';
		pStr = strstr(nodename,"multimediabuffer");
		if(pStr){
			if(fdtdec_decode_region(fdt, offset, "reg", &s_addr, &s_size) < 0)
			{
				debugf("cannot find reg prop, go to next node.\n" );
				goto next;
			}
			uboot_set_secure_range_paraneters(MULTIMEDIA,s_addr,s_size);
		}
		pStr = strstr(nodename,"tos-mem");
		if(pStr){
			if(fdtdec_decode_region(fdt, offset, "reg", &s_addr, &s_size) < 0)
			{
				debugf("cannot find reg prop, go to next node.\n" );
				goto next;
			}
			uboot_set_secure_range_paraneters(TOS_MEMORY,s_addr,s_size);
		}
		pStr = strstr(nodename,"audio-mem");
		if(pStr){
			if(fdtdec_decode_region(fdt, offset, "reg", &s_addr, &s_size) < 0)
			{
				debugf("cannot find reg prop, go to next node.\n" );
				goto next;
			}
			uboot_set_secure_range_paraneters(AUDIO_MEMORY,s_addr,s_size);
		}
		pStr = strstr(nodename,"cp-modem");
		if(pStr){
			if(fdtdec_decode_region(fdt, offset, "reg", &s_addr, &s_size) < 0)
			{
				debugf("cannot find reg prop, go to next node.\n" );
				goto next;
			}
			uboot_set_secure_range_paraneters(CP_MODEM,s_addr,s_size);
		}
		pStr = strstr(nodename,"framebuffer");
		if(pStr){
			if(fdtdec_decode_region(fdt, offset, "reg", &s_addr, &s_size) < 0)
			{
				debugf("cannot find reg prop, go to next node.\n" );
				goto next;
			}
			uboot_set_secure_range_paraneters(FRAMEBUFFER,s_addr,s_size);
		}
		pStr = strstr(nodename,"overlaybuffer");
		if(pStr){
			if(fdtdec_decode_region(fdt, offset, "reg", &s_addr, &s_size) < 0)
			{
				debugf("cannot find reg prop, go to next node.\n" );
				goto next;
			}
			uboot_set_secure_range_paraneters(OVERLAYBUFFER,s_addr,s_size);
		}
next:;
	}
}

int fdt_chosen_bootargs_replace(void *fdt, char *old_args, char *new_args)
{
	int nodeoffset;
	int err, i;
	char *str, *src, *dst;
	const char *path;
	char *strargs;

	if (!old_args || !new_args)
		return -1;
	debugf("fdt_chosen_bootargs_replace start!");

	err = fdt_check_header(fdt);
	if (err < 0) {
		errorf("fdt_chosen_bootargs_replace: %s\n", fdt_strerror(err));
		return err;
	}

	/*
	 * Find the "chosen" node.
	 */
	nodeoffset = fdt_path_offset(fdt, "/chosen");

	/*
	 * If there is no "chosen" node in the blob, leave.
	 */
	if (nodeoffset < 0) {
		errorf("fdt_chosen_bootargs_replace: cann't find chosen");
		return -1;
	}

	/*
	 * If the property exists, update it only if the "force" parameter
	 * is true.
	 */
	path = fdt_getprop(fdt, nodeoffset, "bootargs", NULL);
	if (path != NULL ) {

        debugf("fdt_chosen_bootargs_replace: old bootargs %s!", path);
        str = strstr(path, old_args);
        if(!str) {
            errorf("fdt_chosen_bootargs_replace: cann't find str %s!", old_args);
            return -1;
        }
		strargs = malloc(1024);
		if (!strargs)
			return -1;
        src = path;
        dst = strargs;
        i = 0;
        /* copy the front str */
        while(src != str && i < 1023){
            *dst++ = *src++;
            i++;
        }

        /* copy the new str */
        src = new_args;
        while(*src && i < 1023){
            *dst++ = *src++;
            i++;
        }

        /* copy the back str */
        src = str + strlen(old_args);
        while(*src && i < 1023){
            *dst++ = *src++;
            i++;
        }
        *dst = 0;

        debugf("fdt_chosen_bootargs_replace: new bootargs %s!", strargs);
		err = fdt_setprop(fdt, nodeoffset, "bootargs", strargs, strlen(strargs) + 1);
		if (err < 0)
			errorf("could not set bootargs %s.\n", fdt_strerror(err));
		free(strargs);
	}

	return err;
}

void fdt_fixup_pmic_wa(void *fdt)
{
	int cpunode, vddnode;
	u32 cpureg;
	int vddarm0_node, vddarm1_node;
	u32 vddarm0_phandle, vddarm1_phandle;
	char buf[16];
	int str_len;
	int ret;

	if (ANA_REG_GET(ANA_REG_GLB_CHIP_ID_LOW) >= 0xC003) {
		debugf("2731 later than CE version, skip pmic wa\n");
		return;
	}

	vddarm0_node = fdt_node_offset_by_prop_value(fdt, -1, "regulator-name", "vddarm0", 8);
	if (vddarm0_node == -FDT_ERR_NOTFOUND) {
		errorf("vddarm0 not found\n");
		return;
	} else
		vddarm0_phandle = fdt_get_phandle(fdt, vddarm0_node);

	vddarm1_node = fdt_node_offset_by_prop_value(fdt, -1, "regulator-name", "vddarm1", 8);
	if (vddarm1_node == -FDT_ERR_NOTFOUND)
		return;
	else
		vddarm1_phandle = fdt_get_phandle(fdt, vddarm1_node);

	debugf("vddarm0 phandle 0x%x, vddarm1 phandle 0x%x\n", vddarm0_phandle, vddarm1_phandle);

	cpunode = fdt_node_offset_by_prop_value(fdt, 0, "device_type", "cpu", 4);
	if (cpunode < 0) {
		errorf("cannot find bullhill cpu\n");
		/* Exit if there is no CPU devices but this should never happen */
		return;
	} else {
		while (cpunode != -FDT_ERR_NOTFOUND) {
			u32 *reg = (u32 *)fdt_getprop(fdt, cpunode, "reg", 0);
			if (reg) {
				cpureg = *reg >> 24;
				if (cpureg == 0) {
					debugf("add vddarm0 to cpu0\n");
					vddnode = fdt_appendprop_u32(fdt, cpunode, "cpu0-supply", vddarm0_phandle);
					if (vddnode < 0) {
						char s[64];
						fdt_get_path(fdt, cpunode, s, sizeof(s));
						debugf("could not add vddarm0 node to %s, %s\n", s,
								fdt_strerror(vddnode));
					}
					vddnode = fdt_appendprop(fdt, cpunode, "dual-phase-supply", NULL, 0);
					if (vddnode < 0) {
						char s[64];
						fdt_get_path(fdt, cpunode, s, sizeof(s));
						debugf("could not add dual phase supply node to %s, %s\n", s,
								fdt_strerror(vddnode));
					}
				} else if (cpureg == 4) {
					debugf("add vddarm1 to cpu2\n");
					vddnode = fdt_appendprop_u32(fdt, cpunode, "cpu1-supply", vddarm1_phandle);
					if (vddnode < 0) {
						char s[64];
						fdt_get_path(fdt, cpunode, s, sizeof(s));
						debugf("could not add vddarm1 node to %s, %s\n", s,
								fdt_strerror(vddnode));
					}
					vddnode = fdt_appendprop(fdt, cpunode, "dual-phase-supply", NULL, 0);
					if (vddnode < 0) {
						char s[64];
						fdt_get_path(fdt, cpunode, s, sizeof(s));
						debugf("could not add dual phase supply node to %s, %s\n", s,
								fdt_strerror(vddnode));
					}
				}
			}
			cpunode = fdt_node_offset_by_prop_value(fdt, cpunode, "device_type", "cpu", 4);
		}
	}

	/* append bootargs if all above dts edits sucess */
	memset(buf, 0, 16);
	sprintf(buf, "pmic2731_wa");
	str_len = strlen(buf);
	buf[str_len] = '\0';
	fdt_chosen_bootargs_append(fdt, buf, 1);

	return;
}

int fdt_fixup_iq_reserved_mem(void *fdt)
{
	int offset;
	int ret = 0;
	char *mode;

	mode = getenv("bootmode");
	if (!mode || strcmp("iq", mode)) {
		offset = fdt_node_offset_by_prop_value(fdt, 0, "compatible", "sprd,iq-mem", strlen("sprd,iq-mem") + 1);
		if (offset < 0) {
			return 0;
		}
		ret = fdt_del_node(fdt, offset);
		if (ret < 0)
			errorf("del iq reserved mem failed\n");
	}

	return ret;
}

int fdt_fixup_cp_boot(void *fdt)
{
	int ret = 0;
#if defined( CONFIG_KERNEL_BOOT_CP )
	char buf[64];
	int str_len;
	memset(buf, 0, 64);
	sprintf(buf,"modem=shutdown");
	str_len = strlen(buf);
	buf[str_len] = '\0';
	ret = fdt_chosen_bootargs_append(fdt, buf, 1);
#endif

	ret += fdt_chosen_bootargs_append(fdt, cp_getcmdline(), 1);

	/* if is uart calibraton, remove ttys1 console */
	if(is_calibration_by_uart())
	{
		//__raw_writel(0x285480, CTL_PIN_BASE + REG_PIN_CTRL2);
		//ret = fdt_chosen_bootargs_replace(fdt,"console=ttyS1", "console=ttyS3");
		ret = fdt_chosen_bootargs_replace(fdt,"console=ttyS1", "console=null");
	}

	return 0;
}


#ifdef CONFIG_NAND_BOOT
int fdt_fixup_mtd(void *fdt)
{
	char buf[128];
	int str_len;
	int ret;

	memset(buf, 0, 128);

	sprintf(buf, MTDPARTS_DEFAULT);
	str_len = strlen(buf);
	buf[str_len] = '\0';

	ret = fdt_chosen_bootargs_append(fdt, buf, 1);
	return ret;
}
#endif

int fdt_fixup_lcdid(void *fdt)
{
	char buf[16];

	uint32_t lcd_id = 0;
	int str_len;
	int ret;

	lcd_id = load_lcd_id_to_kernel();
	memset(buf, 0, 16);

	sprintf(buf, "lcd_id=ID");
	str_len = strlen(buf);
	sprintf(&buf[str_len], "%x", lcd_id);
	str_len = strlen(buf);
	buf[str_len] = '\0';

	ret = fdt_chosen_bootargs_append(fdt, buf, 1);
	return ret;
}

int fdt_fixup_lcdbase(void *fdt)
{
	char buf[32];
	int str_len;
	int ret;
	void *lcd_base;

	memset(buf, 0, 32);

	//add lcd frame buffer base, length should be lcd w*h*2(RGB565)
	lcd_base = lcd_get_base_addr(NULL);
	debugf("lcd_base=%p\n" ,lcd_base);
	sprintf(buf, "lcd_base=");
	str_len = strlen(buf);
	sprintf(&buf[str_len], "%lx", lcd_base);
	str_len = strlen(buf);
	buf[str_len] = '\0';

	ret = fdt_chosen_bootargs_append(fdt, buf, 1);
	return ret;
}

int fdt_fixup_lcdsize(void *fdt)
{
	char buf[32];
	int str_len;
	int ret;

	memset(buf,0,32);
	sprintf(buf, "lcd_size=");
	str_len = strlen(buf);
	sprintf(&buf[str_len], "%dx%d", load_lcd_width_to_kernel(), load_lcd_hight_to_kernel());
	str_len = strlen(buf);

	buf[str_len] = '\0';

	ret = fdt_chosen_bootargs_append(fdt, buf, 1);
	return ret;
}
int fdt_fixup_serialno(void *fdt)
{
	char buf[255];
	int str_len;
	int ret;
	memset(buf, 0, 255);

	sprintf(buf, " androidboot.serialno=%s", get_product_sn());
	str_len = strlen(buf);
	buf[str_len] = '\0';
	ret = fdt_chosen_bootargs_append(fdt, buf, 1);
	return ret;
}

int fdt_fixup_memleakon(void *fdt)
{
	char buf[32];
	int str_len;
	int ret;
	memset(buf, 0, 32);


	sprintf(buf, "kmemleak=on");
	str_len = strlen(buf);
	buf[str_len] = '\0';
	ret = fdt_chosen_bootargs_append(fdt, buf, 1);
	return ret;
}

int fdt_fixup_sysdump_magic(void *fdt)
{
	char buf[64];
	int str_len;
	int ret;
	void* magic = (void *)SPRD_SYSDUMP_MAGIC;
	memset(buf, 0, 64);

	sprintf(buf, " sysdump_magic=%lx", magic);
	str_len = strlen(buf);
	buf[str_len] = '\0';
	//debugf("sysdump flag in fdt:%s\n", buf);
	ret = fdt_chosen_bootargs_append(fdt, buf, 1);
	return ret;
}


int fdt_fixup_adc_calibration_data(void *fdt)
{
	//extern unsigned int *adc_data_to_transfer;
	unsigned int *adc_data = malloc(64);
	int ret = read_adc_calibration_data(adc_data, 48);

	if (ret) {
		char buf[64];
		int str_len;
		int ret;

		memset(buf, 0, 64);
		if (((adc_data[2] & 0xffff) < 4500) && ((adc_data[2] & 0xffff) > 3000) && ((adc_data[3] & 0xffff) < 4500) && ((adc_data[3] & 0xffff) > 3000)) {
			str_len = strlen(buf);
			sprintf(&buf[str_len], " adc_cal=%d,%d", adc_data[2], adc_data[3]);
		}
		/*just after fgu adc calibration,and no aux adc calibration,need save fgu adc parameters */
		if ((0x00000002 == adc_data[10]) && (0x00000002 & adc_data[11])) {
			str_len = strlen(buf);
			sprintf(&buf[str_len], " fgu_cal=%d,%d,%d", adc_data[4], adc_data[5], adc_data[6]);
		}

		str_len = strlen(buf);
		buf[str_len] = '\0';
		ret = fdt_chosen_bootargs_append(fdt, buf, 1);
	} else {
		debugf("read_adc_calibration_data failed\n");
	}
	free(adc_data);
	return ret;
}

int fdt_fixup_dram_training(void *fdt)
{
	char buf[64];
	int str_len;
	int ret;
	uint64_t addr;
	uint64_t size;
	size = 0x1000;
	memset(buf, 0, 64);

	sprintf(buf, " mem_cs=%d, mem_cs0_sz=%08x", get_dram_cs_number(), get_dram_cs0_size());
	str_len = strlen(buf);
	buf[str_len] = '\0';
	ret = fdt_chosen_bootargs_append(fdt, buf, 1);
	debugf(" mem_cs=%d, mem_cs0_sz=%08x", get_dram_cs_number(), get_dram_cs0_size());

	/* gerenally, reserved memory should be configured in dts file. Add fixup here to avoid the wide
	 * range of change for various boards.
	 * reserved for ddr training data used
	 */
	if (ret < 0)
		errorf("failed to append ddr training data args bootargs\n");
	else {
		addr = PHYS_SDRAM_1;
		ret = fdt_add_mem_rsv(fdt, addr, size);
		if(ret < 0) {
			errorf("failed to reserved cs0 ddr training data space\n");
		}
		else {
			if(2 == get_dram_cs_number()) {
				addr = PHYS_SDRAM_1 + get_dram_cs0_size();
				ret = fdt_add_mem_rsv(fdt, addr, size);
				if(ret < 0) {
					errorf("failed to reserved cs1 ddr training data space\n");
				}
			}
		}
	}

	return ret;
}

/**
 * You can re-define function void fdt_fixup_chosen_bootargs_board(char *buf, const char *boot_mode, int calibration_mode)
 * in your u-boot/board/spreadtrum/xxx/xxx.c to override this default function
 */
void __attribute__ ((weak)) fdt_fixup_chosen_bootargs_board(char *buf, int calibration_mode)
{
}

int fdt_fixup_chosen_bootargs_board_private(void *fdt)
{
	int ret = 0;
	char buf[512];
	memset(buf, 0, sizeof buf);
	fdt_fixup_chosen_bootargs_board(buf, poweron_by_calibration());
	if (buf[0])
		ret = fdt_chosen_bootargs_append(fdt, buf, 1);
	return ret;
}

#ifdef CONFIG_SECURE_BOOT
static void convertBinToHex(char *bin_ptr, int length, char *hex_ptr)
{
    int i;
    unsigned char tmp;

    if (bin_ptr == NULL || hex_ptr == NULL) {
        return;
    }
    for (i=0; i<length; i++) {
        tmp = (unsigned char)((bin_ptr[i] & 0xf0)>>4);
        if (tmp <= 9) {
            *hex_ptr = (unsigned char)(tmp + '0');
        } else {
            *hex_ptr = (unsigned char)(tmp + 'A' - 10);
        }
        hex_ptr++;
        tmp = (unsigned char)(bin_ptr[i] & 0x0f);
        if (tmp <= 9) {
            *hex_ptr = (unsigned char)(tmp + '0');
        } else {
            *hex_ptr = (unsigned char)(tmp + 'A' - 10);
        }
        hex_ptr++;
    }
}

void fdt_fixup_secureboot_param(void *fdt_blob)
{

	uint8_t *buf = NULL;
	unsigned int str_len = 0;
	buf = malloc(CMDLINE_BUF_SIZE);
	memset(buf, 0, CMDLINE_BUF_SIZE);
	uint8_t *pubk = NULL;
	pubk = malloc(512);
	int i = 0;
#ifdef PRIMPUKPATH
	str_len = strlen(buf);
	sprintf(&buf[str_len], " primpukpath=%s", PRIMPUKPATH);	//"/dev/block/mmcblk0boot0");
	str_len = strlen(buf);
	buf[str_len] = '\0';
#endif
#ifdef PRIMPUKSTART
	str_len = strlen(buf);
	sprintf(&buf[str_len], " primpukstart=%d", PRIMPUKSTART);	//512);
	str_len = strlen(buf);
	buf[str_len] = '\0';
#endif
#ifdef PRIMPUKLEN
	str_len = strlen(buf);
	sprintf(&buf[str_len], " primpuklen=%d", PRIMPUKLEN);	//260);
	str_len = strlen(buf);
	buf[str_len] = '\0';
#endif
	/*hash write by pc tool,but the hash value calculated by u-boot */
	/*if rom secure enable,do not need cal spl hash and pass to kernel */
	if (!secureboot_enabled()) {
		int ret = get_spl_hash(harsh_data);
		if (ret) {
			str_len = strlen(buf);
			sprintf(&buf[str_len], " securesha1=%08x%08x%08x%08x%08x", *(uint32_t *) harsh_data, *(uint32_t *) (harsh_data + 4),
				*(uint32_t *) (harsh_data + 8), *(uint32_t *) (harsh_data + 12), *(uint32_t *) (harsh_data + 16));
			str_len = strlen(buf);
			buf[str_len] = '\0';
		}
	}
/*TODO*/
#ifdef  CONFIG_EMMC_BOOT
	if (0 != common_raw_read("uboot" , SEC_HEADER_MAX_SIZE,
			PUBKEY_VLR_BLOCK_INDEX*EMMC_SECTOR_SIZE, (uint8_t *) pubk)) {
				errorf("PARTITION_BOOT2 read error \n");
				return 0;
			}
#endif

	str_len = strlen(buf);
	sprintf(&buf[str_len]," pubk=");
	str_len = strlen(buf);
	buf[str_len] = '\0';

	str_len = strlen(buf);

	convertBinToHex(pubk, 260, &buf[str_len]);

	fdt_chosen_bootargs_append(fdt_blob, buf, 1);
}

#endif

/**
 * Fix me:
 * fix_memory_size() interface which can be fixup memory size, but because of our
 * device tree is not suitable for the context, for simple, we add a new function. In arm64,
 * we should discard it. It should be ok now.
 */
int fdt_fixup_ddr_size(void *fdt)
{
#ifdef CONFIG_DDR_AUTO_DETECT
	int nodeoffset;
	int err;
	const char *path;
	phys_addr_t ddr_base = CONFIG_SYS_SDRAM_BASE;
	phys_size_t ddr_size = get_real_ram_size();

	nodeoffset = fdt_path_offset(fdt, "/memory");
	debugf("/memory nodeoffset = %d, ddr_base: 0x%x, ddr_size: 0x%x\n",
		nodeoffset, ddr_base, ddr_size);
	if (nodeoffset < 0) {
	    errorf("ERROR: device tree must have /memory node %s.\n", fdt_strerror(nodeoffset));
	    return nodeoffset;
	}

	fdt_delprop(fdt, nodeoffset, "reg");

#ifdef CONFIG_ARM64
	err = fdt_setprop_u64(fdt, nodeoffset, "reg", ddr_base);
	if (err < 0) {
	    errorf("ERROR: cannot set /memory node's reg property(addr)!\n");
	    return err;
	}

	err = fdt_appendprop_u64(fdt, nodeoffset, "reg", ddr_size);
	if (err < 0) {
	    errorf("ERROR: cannot set /memory node's reg property(size)!\n");
	}
#else
	err = fdt_setprop_u32(fdt, nodeoffset, "reg", ddr_base);
	if (err < 0) {
	    errorf("ERROR: cannot set /memory node's reg property(addr)!\n");
	    return err;
	}

	err = fdt_appendprop_u32(fdt, nodeoffset, "reg", ddr_size);
	if (err < 0) {
	    errorf("ERROR: cannot set /memory node's reg property(size)!\n");
	}
#endif

	return err;
#else
	return 0;
#endif
}

#ifdef CONFIG_SANSA_SECBOOT
int fdt_fixup_socid(void *fdt)
{
	char buf[128];
	char *tmp = NULL;
	uint8_t soc_id[32] = {0};
	int str_len;
	int ret;
	int i;

	ret = sansa_compute_socid(soc_id);
	if (ret != 0) {
		errorf("ERROR: compute socid fail!\n");
		return ret;
	}
	memset(buf, 0, sizeof(buf));

	str_len = sprintf(buf, "soc_id=");
	tmp = buf + str_len;
	for (i=0; i<32; i++,tmp+=2) {
		str_len += sprintf(tmp,"%02X",soc_id[i]);
	}
	buf[str_len] = '\0';

	ret = fdt_chosen_bootargs_append(fdt, buf, 1);
	return ret;
}
#endif
