#include <linux/types.h>
#include <secureboot/sprdsec_header.h>
#include <crypto/sprdrsa.h>
#include <secureboot/sec_string.h>
#include <crypto/sprdsha.h>
#include <secure_verify.h>
#include <asm/arch/sprd_reg.h>
#include <secureboot/sec_common.h>
#include <otp_helper.h>
#include <avb/uboot_avb_ops.h>
#include "sprd_common_rw.h"

#define EFUSE_HASH_STARTID 2
#ifdef TOS_TRUSTY
imgToVerifyInfo *img_verify_info_tos = ARG_START_ADRESS;
VbootVerifyInfo *vboot_verify_info = ARG_START_ADRESS;
#endif
 imgToVerifyInfo img_verify_info = {0};
uint8_t pubkhash[32];

static uchar *const s_force_secure_check[] = {
    "splloader",
	"sml",
	"trustos",
#ifdef CONFIG_X86
	"mobilevisor",
	"mvconfig",
	"secvm",
#endif
	"uboot",
#ifdef CONFIG_VBOOT_V2
	"vbmeta",
#endif
	"boot",
	"recovery",
	"wl_modem",
	"wl_ldsp",
	"wmodem",
	"wl_gdsp",
	"wl_warm",
	"pm_sys",
	"tl_ldsp",
	"tl_tgdsp",
	"tl_modem",
	"l_modem",
	"l_ldsp",
	"l_gdsp",
	"l_warm",
	"l_tgdsp",
	"l_agdsp",
	"wdsp",
	#ifdef CONFIG_SUPPORT_WIFI
	"wcnmodem",
	#endif
	#ifdef CONFIG_SYSTEM_VERIFY
	"system",
	#endif
	NULL
};

static int dl_secure_verify(void *partition_name,void *header,void *code)
{
#ifdef CONFIG_SPRD_SECBOOT
#ifdef CONFIG_VBOOT_V2
	sprd_dl_vboot_verify(partition_name, header, code);
#else
	sprd_dl_verify(partition_name, header, code);
#endif
#elif defined CONFIG_SANSA_SECBOOT
	sansa_enable_efuse_EB();
	sansa_dl_secure_verify(partition_name,header,code);
#else
	secure_verify(partition_name,header,code);
#endif
}

static void fb_secure_verify(void *partition_name,void *header,void *code)
{
#ifdef CONFIG_SPRD_SECBOOT
	sprd_fb_secure_verify(partition_name, header, code);
#elif defined CONFIG_SANSA_SECBOOT
	sansa_fb_secure_verify(partition_name,header,code);
#else
	secure_verify(partition_name,header,code);
#endif
}

static int _check_secure_part(uchar * partition_name)
{
	int i = 0;
	do {
		if (SEC_TRUE == strcmp(s_force_secure_check[i], partition_name))
			return SEC_TRUE;
		i++;
	}
	while (s_force_secure_check[i] != 0);

	return SEC_FALSE;
}

static int secure_load_partition(char *partition_name, uint64_t size, uint64_t offset, char *ram_addr)
{
	sys_img_header img_header;
	uint32_t img_size = 0;

	if ((0 == strcmp(partition_name, "boot"))||(0 == strcmp(partition_name, "recovery")))
	{
#if (!defined CONFIG_VBOOT_V2)
		if (0 != common_raw_read(partition_name, SYS_HEADER_SIZE, 0, (char*)&img_header))
		{
			errorf("read %s partition header fail\n", partition_name);
			return;
		}

	img_size = img_header.mImgSize;

		if (0 != common_raw_read(partition_name, img_size+CERT_SIZE, 0, ram_addr))
		{
			errorf("read %s partition fail\n", partition_name);
			return;
		}
#else
		debugf("in %s, l:%d.\n, %s, 0x%x, 0x%x", __func__, __LINE__, partition_name, size, offset);
		if (0 != common_raw_read(partition_name,size, offset, (u8*)ram_addr))
		{
			errorf("read %s partition fail\n", partition_name);
			return;
		}
#endif
	}
	else
	{
		if (0 != common_raw_read(partition_name,size, offset, (u8*)ram_addr))
		{
			errorf("read %s partition fail\n", partition_name);
			return;
		}
	}
}

static void secboot_get_pubkhash(uint64_t img_buf,uint64_t imginfo)
{
#if defined (CONFIG_SPRD_SECBOOT)
	sprd_get_hash_key((uint8_t *)img_buf,(uint8_t *)imginfo);//be carefull! the first param in chipram is not same as uboot64
#endif
#if defined (CONFIG_SANSA_SECBOOT)
	sansa_get_hash_key((uint8_t *)img_buf,(uint8_t *)imginfo);
#endif
}

void  secboot_param_set(uint64_t load_buf,imgToVerifyInfo *imginfo)
{
	imginfo->img_addr = load_buf;
	imginfo->img_len = SIMG_BUF;
	imginfo->hash_len = 32;
	imginfo->pubkeyhash = pubkhash;
#ifdef CONFIG_SPRD_SECBOOT
	imginfo->flag = SPRD_FLAG;
#endif
#ifdef CONFIG_SANSA_SECBOOT
    sansa_enable_efuse_EB();
	imginfo->flag = SANSA_FLAG;
#endif
}

int secure_efuse_program(void)
{
	int ret = 0;

#if defined(CONFIG_SANSA_SECBOOT)
			printf("###### secure_efuse_program ######\n");
			ret = secure_efuse_program_sansa();
#endif

#if defined(CONFIG_SPRD_SECBOOT)
			//ret = secure_efuse_program_sprd(); /*wait for efuse driver ready*/
#endif

#if defined(CONFIG_SECURE_BOOT)
			ret = secure_efuse_program_native();
#endif
}

VERIFY_RESULT dl_secure_process_flow(ulong * strip, unsigned char * part_name,
			uint32_t rcv_size, uint32_t total_size, unsigned char * buf)
{
	static uint32_t rcv_already = 0;
	int ret = 0;

	if (SEC_FALSE == _check_secure_part(part_name))	{
	    errorf("no secure image to need verify \n");
		return VERIFY_NO_NEED;
	}
#ifdef CONFIG_SECURE_BOOT
	if (1 != secure_header_parser(buf)) {
		errorf("check secure image failed!! \n");
		return VERIFY_FAIL;
	}
#endif

	if (SEC_TRUE == strcmp("uboot", part_name)) {
		dl_secure_verify("splloader", buf, 0);
	} else if (SEC_TRUE == strcmp("splloader", part_name)) {
		dl_secure_verify("splloader0", buf, 0);
	}
#ifdef CONFIG_SECBOOT
	else if ((SEC_TRUE == strcmp("sml",part_name))||(SEC_TRUE == strcmp("trustos",part_name))
				||(SEC_TRUE == strcmp("mobilevisor",part_name))
				||(SEC_TRUE == strcmp("mvconfig",part_name))
				||(SEC_TRUE == strcmp("secvm",part_name))){
			dl_secure_verify("fdl1",buf,0);
	}
#endif
#ifdef CONFIG_SECURE_BOOT
	else if (0 == strcmp("system", part_name)) {
		if (0 == rcv_already) {
			Sec_SHA1_Start();
			*strip = VLR_CODE_OFF;
		}
		secure_verify_system_start("fdl2", buf, rcv_size);
		rcv_already += rcv_size;
		if (total_size == rcv_already) {
			debugf("Check system image \n");
			Sec_SHA1_Finish();
			ret = secure_verify_system_end("fdl2", buf, 0);
			debugf("secure_verify_system  ret = %d \n", ret);
			if (0 == ret)
				return VERIFY_FAIL;
			else
				return VERIFY_OK;
		}
	}
#endif
#ifdef CONFIG_VBOOT_V2
	else if (0 == strcmp("vbmeta", part_name)) {
		/*vbmeta image*/
		//dl_vboot_verify_vbmeta();
	}
#endif
	else {
		dl_secure_verify("fdl2",buf,0);
	}

	return VERIFY_OK;
}

VERIFY_RESULT fb_secure_process_flow(ulong * strip, unsigned char * part_name,
			uint32_t rcv_size, uint32_t total_size, unsigned char * buf)
{
	static uint32_t rcv_already = 0;
	int ret = 0;

	if (SEC_FALSE == _check_secure_part(part_name)){
		errorf("no secure image to need verify \n");
		return VERIFY_NO_NEED;
		}
#ifdef CONFIG_SECURE_BOOT
	if (1 != secure_header_parser(buf)) {
		errorf("check secure image failed!! \n");
		return VERIFY_FAIL;
	}
#endif

	if (SEC_TRUE == strcmp("uboot", part_name)) {
		fb_secure_verify("splloader", buf, 0);
	} else if (SEC_TRUE == strcmp("splloader", part_name)) {
		fb_secure_verify("splloader0", buf, 0);
	}
#ifdef CONFIG_SECBOOT
	else if ((SEC_TRUE == strcmp("sml",part_name))||(SEC_TRUE == strcmp("trustos",part_name))
				||(SEC_TRUE == strcmp("mobilevisor",part_name))
				||(SEC_TRUE == strcmp("mvconfig",part_name))
				||(SEC_TRUE == strcmp("secvm",part_name))) {
			fb_secure_verify("splloader",buf,0);
	}
#endif
#ifdef CONFIG_SECURE_BOOT
	else if (0 == strcmp("system", part_name)) {
		if (0 == rcv_already) {
			Sec_SHA1_Start();
			*strip = VLR_CODE_OFF;
		}
		secure_verify_system_start("fdl2", buf, rcv_size);
		rcv_already += rcv_size;
		if (total_size == rcv_already) {
			debugf("Check system image \n");
			Sec_SHA1_Finish();
			ret = secure_verify_system_end("fdl2", buf, 0);
			debugf("secure_verify_system  ret = %d \n", ret);
			if (0 == ret)
				return VERIFY_FAIL;
			else
				return VERIFY_OK;
		}
	}
#endif
	else {
#ifdef CONFIG_VBOOT_V2
		/*vbmeta image*/
		if(0 == strcmp("vbmeta", part_name)) {
			//fb_vboot_verify_vbmeta();
		} else if((0 == strcmp("boot", part_name)) || (0 == strcmp("recovery", part_name)) || (0 == strcmp("modem", part_name))) { 		/*boot image / recovery image / modem*/
			//fb_vboot_verify_bootimg();
		} else {
			fb_secure_verify("uboot",buf,0);
		}
#else
		fb_secure_verify("uboot",buf,0);
#endif
	}

	return VERIFY_OK;
}

void secboot_init(char *partition_name)
{
#if defined (CONFIG_SANSA_SECBOOT)
	sansa_calc_hbk();
#endif
#ifdef CONFIG_VBOOT_V2
	/*malloc one memory to vbmeta image buffer use. ok? 2KB*/
	debugf("VERIFY_ADDR:0x%x, KERNEL_ADR:0x%x. \n", VERIFY_ADDR, KERNEL_ADR);
	vboot_secure_process_prepare();
	vboot_secure_process_init(partition_name, VERIFY_ADDR, VBMETA_IMG_ADDR);
#endif
}

void secboot_terminal(void)
{
	#if defined (CONFIG_SPRD_SECBOOT)
	sprd_calc_hbk();
	#endif
}

void secboot_secure_process_flow(char *partition_name, uint64_t size, uint64_t offset, char *ram_addr)
{
	uint32_t ret = 1;
#ifdef CONFIG_VBOOT_V2
	vboot_secure_process_flow(partition_name);
#else
	secure_load_partition(partition_name,size,offset,ram_addr);
#if defined (TOS_TRUSTY)
	secboot_param_set(VERIFY_ADDR,img_verify_info_tos);
	img_verify_info_tos->pubkeyhash = (uint8_t*)ARG_START_ADRESS+PAGE_ALIGN;
	secboot_get_pubkhash(UBOOT_START,img_verify_info_tos->pubkeyhash);

	printf("p1:%x, p2:%x,P3:%x,p4:%x,p5:%x\n",img_verify_info_tos->img_addr,img_verify_info_tos->img_len,img_verify_info_tos->pubkeyhash,img_verify_info_tos->hash_len,img_verify_info_tos->flag);
	ret = uboot_verify_img((imgToVerifyInfo*)img_verify_info_tos,sizeof(imgToVerifyInfo));
	while(ret);
#else
	secboot_param_set(VERIFY_ADDR,&img_verify_info);
	secboot_get_pubkhash(UBOOT_START,img_verify_info.pubkeyhash);
	uboot_verify_img(&img_verify_info,sizeof(imgToVerifyInfo));
#endif
#endif
}

#ifdef CONFIG_VBOOT_V2
void vboot_secure_process_init(char *partition_name, char *img_ram_addr, char * vbmeta_ram_addr)
{
	uint64_t partition_size = 0;
	uint8_t * avb_vboot_key = NULL;
	uint32_t avb_vboot_key_len = 0;

	secure_get_partition_size(partition_name, &partition_size);

	debug("partintion:%s, size:0x%x\n", partition_name, partition_size);
	vboot_verify_info->img_len = partition_size;

	secure_get_partition_size("vbmeta", &partition_size);
	debug("partintion:%s, size:0x%x.\n", "vbmeta", partition_size);

	vboot_verify_info->vbmeta_img_len = partition_size;

	debug("in %s, l:%d, partition_size:0x%x, VERIFY_ADDR:0x%x \n", __func__, __LINE__, partition_size, VERIFY_ADDR);
	
	vboot_verify_info->img_addr = img_ram_addr;

	vboot_verify_info->vbmeta_img_addr = vbmeta_ram_addr;

	debugf("wo kao... size:0x%x, ram_addr:0x%x.   vbmeta_ram_addr:0x%x.   \n", partition_size, img_ram_addr, vbmeta_ram_addr);

	vboot_verify_info->vbmeta_pubkey_addr = (char *)g_sprd_vboot_key;
	vboot_verify_info->vbmeta_pubkey_len = g_sprd_vboot_key_len;

	debugf("avb_vboot_key:0x%x.   avb_vboot_key_len:%d.   \n", avb_vboot_key, avb_vboot_key_len);
}

void vboot_secure_process_flow(char *partition_name)
{
	debugf("img_addr:0x%x.  len:0x%x. \n", vboot_verify_info->img_addr, vboot_verify_info->img_len);

	/*partition load*/
	secure_load_partition(partition_name,vboot_verify_info->img_len, (uint64_t)0, vboot_verify_info->img_addr);

	secure_load_partition("vbmeta",vboot_verify_info->vbmeta_img_len, (uint64_t)0, vboot_verify_info->vbmeta_img_addr);

#if defined (TOS_TRUSTY)

#ifdef CONFIG_VBOOT_DUMP
	//dumpHex("boot image",  (uchar *)vboot_verify_info->img_addr, 512);

	//dumpHex("vbmeta image",  (uchar *)vboot_verify_info->vbmeta_img_addr, 512);

	//dumpHex("vbmeta pubkey",  (uchar *)vboot_verify_info->vbmeta_pubkey_addr, 512);
#endif
	uboot_vboot_verify_img((VbootVerifyInfo*)vboot_verify_info,sizeof(VbootVerifyInfo));

	debugf("exit %s, leave vboot secure check.\n",  __func__);
#else
	secboot_param_set(VERIFY_ADDR,&img_verify_info);
	secboot_get_pubkhash(UBOOT_START,img_verify_info.pubkeyhash);
	uboot_verify_img(&img_verify_info,sizeof(imgToVerifyInfo));
#endif
}

void vboot_secure_process_prepare(void)
{
	unsigned char sprd_vboot_key[SPRD_RSA4096PUBKLEN];
	sprd_get_vboot_key(UBOOT_START, sprd_vboot_key);

#ifdef CONFIG_VBOOT_DUMP
	dumpHex("vboot pubk", sprd_vboot_key, SPRD_RSA4096PUBKLEN);
#endif
	avb_ops_set_vboot_key(sprd_vboot_key, sizeof(sprd_vboot_key));
}
#endif

void secboot_update_swVersion(void)
{
	uint32_t flag;
#ifdef CONFIG_SPRD_SECBOOT
	flag = SPRD_FLAG;
#endif
#ifdef CONFIG_SANSA_SECBOOT
	flag = SANSA_FLAG;
#endif

#ifdef CONFIG_SANSA_SECBOOT
	ap_sansa_efuse_prog_power_on();
#endif

	uboot_update_swVersion(&flag,sizeof(flag));

#ifdef CONFIG_SANSA_SECBOOT
	ap_sansa_efuse_power_off();
#endif

}

