#include <secureboot/sprdsec_header.h>
#include <secureboot/sprd_rsa.h>
#include <secureboot/sec_string.h>
#include <secureboot/sprdsha.h>
#include <secure_verify.h>
#ifndef CONFIG_SC9833
#include <asm/arch/chip_drv_common_io.h>
#endif
#include <asm/arch/sci_types.h>
#include <asm/arch/sprd_reg.h>
#include <secureboot/sec_common.h>
#include <otp_helper.h>
#include "sprd_common_rw.h"

#define EFUSE_HASH_STARTID 2
#ifdef TOS_TRUSTY
#define  PAGE_ALIGN 4096
#define  ARG_START_ADRESS 0x94000000
imgToVerifyInfo *img_verify_info_tos = ARG_START_ADRESS;
#endif
 imgToVerifyInfo img_verify_info = {0};
uint8_t pubkhash[32];

static uchar *const s_force_secure_check[] = {
    "splloader",
	"sml",
	"trustos",
	"uboot",
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
	sprd_dl_verify(partition_name, header, code);
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
	//sprd_fb__secure_verify(partition_name, header, code);
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
			ret = secure_efuse_program_sprd();
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
	else if ((SEC_TRUE == strcmp("sml",part_name))||(SEC_TRUE == strcmp("trustos",part_name))){
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
	else if ((SEC_TRUE == strcmp("sml",part_name))||(SEC_TRUE == strcmp("trustos",part_name))) {
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
		fb_secure_verify("uboot",buf,0);
	}

	return VERIFY_OK;
}

void secboot_init(void)
{
#if defined (CONFIG_SANSA_SECBOOT)
	sansa_calc_hbk();
#endif
}

void secboot_terminal(void)
{
}

void secboot_secure_process_flow(char *partition_name, uint64_t size, uint64_t offset, char *ram_addr)
{
	secure_load_partition(partition_name,size,offset,ram_addr);
	#ifdef TOS_TRUSTY
	secboot_param_set(VERIFY_ADDR,img_verify_info_tos);
	img_verify_info_tos->pubkeyhash = (uint8_t*)ARG_START_ADRESS+PAGE_ALIGN;
	secboot_get_pubkhash(UBOOT_START,img_verify_info_tos->pubkeyhash);
	//dumpHex("PARAM",ARG_START_ADRESS,sizeof(imgToVerifyInfo));
	printf("p1:%x, p2:%x,P3:%x,p4:%x,p5:%x\n",img_verify_info_tos->img_addr,img_verify_info_tos->img_len,img_verify_info_tos->pubkeyhash,img_verify_info_tos->hash_len,img_verify_info_tos->flag);
	uboot_verify_img((imgToVerifyInfo*)img_verify_info_tos,sizeof(imgToVerifyInfo));
	#else
	secboot_param_set(VERIFY_ADDR,&img_verify_info);
	secboot_get_pubkhash(UBOOT_START,img_verify_info.pubkeyhash);
	uboot_verify_img(&img_verify_info,sizeof(imgToVerifyInfo));
	#endif
}

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

