/* -*- C -*- ****************************************************************
 *
 *  System        : 
 *  Module        : 
 *  Object Name   : $RCSfile$
 *  Revision      : $Revision$
 *  Date          : $Date$
 *  Author        : $Author$
 *  Created By    : zhenxiong.lai
 *  Created       : Thu Dec 6 08:32:13 2018
 *  Last Modified : <181206.0833>
 *
 *  Description	
 *
 *  Notes
 *
 *  History
 *	
 ****************************************************************************
 *
 *  Copyright (c) 2018 sprd.
 * 
 *  All Rights Reserved.
 * 
 * This  document  may  not, in  whole  or in  part, be  copied,  photocopied,
 * reproduced,  translated,  or  reduced to any  electronic  medium or machine
 * readable form without prior written consent from sprd.
 *
 ****************************************************************************/
#include <common.h>
#include <malloc.h>
#include <part.h>
#include <sprd_common_rw.h>
#include <ext4fs.h>
#include <mmc.h>
#include <chipram_env.h>

typedef struct ddr_partition {
	char part_name[32];
	uint64_t partition_address;
} ddr_partition_t;

static const ddr_partition_t ddr_partition_info[] = {
#ifdef CONFIG_DDR_BOOT_BOOTIMAGE_ADR
	{"boot", CONFIG_DDR_BOOT_BOOTIMAGE_ADR},
#endif
#ifdef CONFIG_DDR_BOOT_DTBIMAGE_ADR
	{"dtb", CONFIG_DDR_BOOT_DTBIMAGE_ADR},
#endif
#ifdef CONFIG_DDR_BOOT_DTBOIMAGE_ADR
	{"dtbo", CONFIG_DDR_BOOT_DTBOIMAGE_ADR},
#endif
#ifdef CONFIG_PSCP_IMG_ADDR
	{"l_modem", CONFIG_PSCP_IMG_ADDR},
#endif
};

int get_img_partition_id(char *part_name, unsigned int *id)
{
	return -1;
}

#ifdef CONFIG_PARTITION_UUIDS
int get_img_partition_uuid(char *part_name, char* uuid)
{
	return -1;
}
#endif

int common_raw_read(char *part_name, uint64_t size, uint64_t offset, char *buf)
{
	uint64_t part_start = 0;
	uint64_t i = 0;

	if ((!part_name) || (!size) || (!buf))
		return -1;

	for (i = 0; i < ARRAY_SIZE(ddr_partition_info); i++) {
		if (!strcmp(part_name, ddr_partition_info[i].part_name)) {
			part_start = ddr_partition_info[i].partition_address;
			goto copy_flag;
		}
	}

	return -2;

copy_flag:
	offset += part_start;
	memcpy(buf, offset, size);

	return 0;
}

int common_raw_write(char *part_name, uint64_t size, uint64_t updsize, uint64_t offset, char *buf)
{
	return -1;
}

int common_raw_erase(char *part_name, uint64_t size, uint64_t offset)
{
	return -1;
}

int common_write_backstage(char *part_name, uint32_t size, uint64_t offset, char *buf)
{
	return -1;
}


int common_query_backstage(char *part_name, uint32_t size, char *buf)
{
	return -1;
}


int common_repartition(disk_partition_t *partitions, int parts_count)
{
	return -1;
}

int32_t common_get_lba_size(void)
{
	return -1;
}

int do_fs_file_read(char *mpart, char *filenm, void *buf, int len)
{
	return -1;
}

int do_fs_file_write(char *mpart, char *filenm, void *buf, int len)
{
	return -1;
}

int do_raw_data_write(char *part, u32 updsz, u32 size, u32 off, char *buf)
{
	return -1;
}


