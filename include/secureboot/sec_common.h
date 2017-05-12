

#ifndef __SEC_COMMON_H
#define __SEC_COMMON_H

#include<secureboot/sprdsec_header.h>
#include<crypto/sprdrsa.h>
#include<secure_efuse.h>
#include<asm/arch/chip_releted_def.h>

typedef struct{
#ifdef CONFIG_SC9833
	uint32_t img_addr; //the base address of image to verify
	uint32_t img_len; //length of image
	uint32_t pubkeyhash;//pubkey hash for verifying image
	uint32_t hash_len; //length of pubkeyhash
#else
	uint64_t img_addr; //the base address of image to verify
	uint64_t img_len; //length of image
	uint64_t pubkeyhash;//pubkey hash for verifying image
	uint64_t hash_len; //length of pubkeyhash
#endif
	uint32_t flag;//sprd or sansa plan
}imgToVerifyInfo;

#define SPRD_FLAG 1
#define SANSA_FLAG 2

extern imgToVerifyInfo img_verify_info;
#ifdef TOS_TRUSTY
extern imgToVerifyInfo *img_verify_info_tos;
#define  PAGE_ALIGN 4096
#define  ARG_START_ADRESS 0x98100000//0x94000000+64M
#endif

/*this two marco is diffent among products, pls check it!*/
#define SIMG_BUF     0x2E00000
#define CERT_SIZE    0x1000
#define SYS_HEADER_SIZE 0x200
#define RESERVE_SIZE 0x3000000
#if defined (CONFIG_SOC_IWHALE2)||defined (CONFIG_SPRD_SOC_SP9853I)
#define IRAM_BEGIN   0xe6000000
#else
#define IRAM_BEGIN   (SPL_BASE_ADDR - SYS_HEADER_SIZE)
#endif
#define UBOOT_START  (CONFIG_SYS_TEXT_BASE - SYS_HEADER_SIZE)
#define VERIFY_ADDR  (CONFIG_SYS_TEXT_BASE - RESERVE_SIZE)


#define SEC_TRUE    0
#define SEC_FALSE   1

/***108K***/
#define SPL_IMAGE_SIZE (108*1024)
#define SECTION_SIZE   (512)
/**
SPL_EMMC_SECTOR_NUM=SPL_IMAGE_SIZE / SECTOR_SIZE
**/
#define SPL_EMMC_SECTOR_NUM  (SPL_IMAGE_SIZE/SECTION_SIZE)
#define SAVE_SPL_IMAGE  VERIFY_ADDR

#define MAX_PACKED_IMAGE_SIZE  0x1400000  // 20MB
typedef struct{
    uint32_t  mMagicNum;        // "BTHD"=="0x42544844"=="boothead"
    uint32_t  mVersion;         // 1
    uint8_t   mPayloadHash[32]; // sha256 hash value
    uint64_t  mImgAddr;         // image loaded address
    uint32_t  mImgSize;         // image size
    uint32_t  is_packed;        // packed image flag 0:false 1:true
    uint32_t  mFirmwareSize;    // runtime firmware size
    uint32_t  mFirmwareOff;     // runtime firmware offset
    uint8_t   reserved[448];    // 448 + 16*4 = 512
}sys_img_header;

typedef enum VERIFY_RESULT {
	VERIFY_NO_NEED = 1,
	VERIFY_OK,
	VERIFY_FAIL,
	VERIFY_STOP_WRITE,
	VERIFY_SPECIAL
} VERIFY_RESULT;

VERIFY_RESULT dl_secure_process_flow(ulong * strip, unsigned char * part_name, 
				uint32_t rcv_size, uint32_t total_size, unsigned char * buf);

VERIFY_RESULT fb_secure_process_flow(ulong * strip, unsigned char * part_name,
			    uint32_t rcv_size, uint32_t total_size, unsigned char * buf);

void secboot_param_set(uint64_t load_buf,imgToVerifyInfo *imginfo);

int secure_efuse_program();

void secboot_init(void);

void secboot_terminal(void);

void secboot_secure_process_flow(char *partition_name, uint64_t size, uint64_t offset, char *ram_addr);

void secboot_update_swVersion(void);

#endif
