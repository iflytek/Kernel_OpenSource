/*
 * Copyright (C) 2013 Spreadtrum Communications Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 */

#include <boot_mode.h>
#include <common.h>
#include <sprd_common_rw.h>
#include <asm/arch/check_reboot.h>


#define PART_SYSTEM						"userdata"
#define SPRDISK_RAMDISK_PATH	"/sprdisk/ramdisk.img"

int boot_sprdisk(void)
{
	int	ret;
	int size;
#ifndef CONFIG_SUPPORT_LTE
#ifdef WDSP_ADR
		size = WDSP_ADR - RAMDISK_ADR;
#else
		size = TDDSP_ADR - RAMDISK_ADR;
#endif
#endif
	ret = do_fs_file_read(PART_SYSTEM, SPRDISK_RAMDISK_PATH, (void *)RAMDISK_ADR, size);
	if(ret < 0)
		errorf("read file %s wrong!\n", SPRDISK_RAMDISK_PATH);

	return ret;
}

int sprdisk_mode_detect(void)
{
	if(reboot_mode_check() == CMD_SPRDISK_MODE) {
		return 1;
	}

	return 0;
}
