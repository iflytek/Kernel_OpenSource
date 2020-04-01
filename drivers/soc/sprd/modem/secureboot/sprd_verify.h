#ifndef __SPRD_VERIFY_H
#define __SPRD_VERIFY_H
#include "sprdsec_header.h"
#include <linux/raid/pq.h>
#include <uapi/asm-generic/errno-base.h>

#define MODEM_BLOCK 27
#define V3PHY_BLOCK 28
#define NRPHY_BLOCK 29
#define NRDSP1_BLOCK 30
#define OLD_IMG 2

struct sys_img_header {
	u32 magic_num;
	u32 version;
	u8 payload_hash[32];
	u64 img_addr;
	u32 img_size;
	u32 is_packed;
	u32 firmware_size;
	u32 firmware_off;
	u8 reserved[448];
};

void cal_sha256(u8 *input, u32 bytes_num, u8 *output);
int sprd_verify_cert(u8 *hash_key_precert, u8 *hash_data, u8 *certptr);
u32 sprd_get_img_size(u8 *buf);
u32 sprd_get_modem_size(u8 *buf);
int sprd_verify_modem(u8 *hash_key_precert, u8 *imgbuf, u32 imgsize);
int sprd_verify_img_kbc(u8 *hash_key_precert, u8 *imgbuf, u32 imgsize);
#endif
