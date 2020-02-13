/*
 * SPRD verify orca modem process in AP side.
 *
 * Copyright (C) 2020 Spreadtrum Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 of
 * the License as published by the Free Software Foundation.
 *
 * This program is used to verify orca modem in AP side for
 * Spreadtrum SoCs.
 */
#include "sprd_verify.h"
#include "sprd_secureboot.h"
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/mm.h>
#include <linux/string.h>

u8 mode_flag;
static u8 hash_key[HASH_BYTE_LEN];

static int __init scanner_hash(char *str)
{
	u8 i;

	if (str) {
		for (i = 0; i < HASH_BYTE_LEN; i++) {
			hash_key[i] = (u8)(*str);
			str++;
		}
	}
	return 0;
}
__setup("hash=", scanner_hash);

int sprd_check_secure_part(u8 *partition_name)
{
	int i = 0;

	do {
		if (strcmp(secure_modem[i], partition_name) == 0)
			return i;
		i++;
	} while (secure_modem[i] != 0);
	return -ERANGE;
}

int sprd_image_verify(struct kbc_load_table_s  *p_table)
{
	u8 i;
	void *vaddr;
	u32 data_len;
	int ret = 0;

	if (!p_table->image_cnt)
		return -ENOENT;

	for (i = 0; i < MAX_MODEM_NUM; i++) {
		if (!p_table->image[i].img_name)
			continue;
		if (strcmp(p_table->image[i].img_name, "modem") == 0)
			mode_flag = MODEM_BLOCK;
		else if (strcmp(p_table->image[i].img_name, "v3phy") == 0)
			mode_flag = V3PHY_BLOCK;
		else if (strcmp(p_table->image[i].img_name, "nrphy") == 0)
			mode_flag = NRPHY_BLOCK;
		else if (strcmp(p_table->image[i].img_name, "nrdsp1") == 0)
			mode_flag = NRDSP1_BLOCK;

		/* skip "modem" */
		if ((strcmp(p_table->image[i].img_name, "v3phy") == 0) ||
		     (strcmp(p_table->image[i].img_name, "nrphy") == 0) ||
		     (strcmp(p_table->image[i].img_name, "nrdsp1") == 0)) {
			vaddr = memremap(p_table->image[i].img_addr,
				p_table->image[i].img_len, MEMREMAP_WB);
			if (!vaddr)
				return -ENOMEM;
			data_len = sprd_get_img_size(vaddr);
			ret = sprd_verify_img_kbc(hash_key,
				(u8 *)vaddr + SECBOOT_HEAD_SIZE, data_len);
			memunmap(vaddr);
			if (ret)
				return ret;
		}
	}
	pr_info("secboot verify success\n");
	return ret;
}

