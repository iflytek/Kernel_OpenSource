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


static int file_read(char *mpart, char *filenm, void *buf, int len)
{
#ifdef CONFIG_FS_EXT4
	disk_partition_t info;
	block_dev_desc_t *dev_desc;

	loff_t len_read = 0;
	loff_t file_len = 0;
	int ret=0;

	dev_desc = get_dev("mmc", 0);
	if (NULL == dev_desc) {
		printf("ext4_read_content get dev error\n");
		return -1;
	}

	if (get_partition_info_by_name(dev_desc, mpart, &info) == -1) {
		printf("## Valid EFI partition not found ##\n");
		return -1;
	}

	/* set the device as block device */
	ext4fs_set_blk_dev(dev_desc, &info);

	/* mount the filesystem */
	if (!ext4fs_mount(info.size)) {
		printf("Bad ext4 partition:%s\n",  mpart);
		ext4fs_close();
		return -1;
	}

	/* start read */
	if (ext4_read_file(filenm, buf, 0,len, &len_read) == -1) {
		printf("** Error ext4_read_file() **\n");
		ext4fs_close();
		return -1;
	}

	ext4fs_open(filenm, &file_len);

	ext4fs_close();

	ret = (int)file_len;
	return ret;

#endif
}

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
	ret = load_sd_ramdisk( (void *)RAMDISK_ADR, size);
	if(ret <= 0){
		ret = file_read(PART_SYSTEM, SPRDISK_RAMDISK_PATH, (void *)RAMDISK_ADR, size);
		if(ret < 0){
			errorf("read file %s wrong!\n", SPRDISK_RAMDISK_PATH);
		}
	}
	return ret;
}

int sprdisk_mode_detect(void)
{
	if(reboot_mode_check() == CMD_SPRDISK_MODE) {
		return 1;
	}

	return 0;
}
