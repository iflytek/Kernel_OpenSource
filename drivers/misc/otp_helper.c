/*
 * Copyright (C) 2014 Spreadtrum Communications Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

/* IMPORTANT:
 *
 * TODO:
 */

#include <common.h>
#include <otp_helper.h>

#define BLK_UID_DETA_START			( 0 )
#define BLK_UID_DETA_END			( 1)
#define UID_BLOCK0 0
#define UID_BLOCK1  1
#define BIN_BLOCK13 13
#define BIN_BLOCK15 15

void get_efuse_uid(void)
{
	unsigned int block0, block1;
	block0 = sprd_ap_efuse_read(BLK_UID_DETA_START);
	block1 = sprd_ap_efuse_read(BLK_UID_DETA_END);
	printf("efuse uid is %08x%08x\n", __func__, block0, block1);
}

static void efuse_uid(void)
{
	u32 block0,block1;
    u32 x,y,wafer_id;
    u32 LOTID_0,LOTID_1,LOTID_2,LOTID_3,LOTID_4,LOTID_5;
	char bufallid[20];
	block0= sprd_ap_efuse_read(UID_BLOCK0);
	block1= sprd_ap_efuse_read(UID_BLOCK1);
    y=block1&0x7F;
    x=(block1>>7)&0x7F;
    wafer_id=(block1>>14)&0x1F;
    LOTID_0=(block1>>19)&0x3F;
    LOTID_1=(block1>>25)&0x3F;
    LOTID_2=block0&0x3F;
    LOTID_3=(block0>>6)&0x3F;
    LOTID_4=(block0>>12)&0x3F;
    LOTID_5=(block0>>18)&0x3F;
	printf("the uid = %c%c%c%c%c%c_%d_%d_%d\n",LOTID_5+48,LOTID_4+48,LOTID_3+48,LOTID_2+48,LOTID_1+48,LOTID_0+48,wafer_id,x,y);
}

static void efuse_binning(void)
{
	u32 block13,block15;
	block13= sprd_ap_efuse_read(BIN_BLOCK13);
	block15= sprd_ap_efuse_read(BIN_BLOCK15);
	printf("BIF = 0x%x, 0x%x",BIN_BLOCK13,BIN_BLOCK15);
}

void sprd_uid_bin_info(void)
{
	efuse_uid();
	efuse_binning();
}


