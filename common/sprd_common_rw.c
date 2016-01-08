#include <common.h>
#include <malloc.h>
#include <part.h>
#include <sprd_common_rw.h>
#ifdef CONFIG_CMD_UBI
#include <ubi_uboot.h>
#endif
#ifdef  CONFIG_EMMC_BOOT
#include <sprd_mmc.h>
#endif

static disk_partition_t local_part_info;

int block_dev_read(block_dev_desc_t *dev_desc, uint64_t size, uint64_t offset, char *buf)
{
	lbaint_t start = 0;
	lbaint_t blkcnt = 0;
	char * temp = NULL;
	uint64_t header_sz = 0;
	uint64_t offb = 0;

	if ((0 != size % dev_desc->blksz) || (0 != offset % dev_desc->blksz)) {
		temp = malloc(dev_desc->blksz);
		if (NULL == temp)
			goto fail;
	}

	start = offset / dev_desc->blksz;
	offb = offset % dev_desc->blksz;
	if (0 != offb) {
		header_sz = dev_desc->blksz - offb;
		if (1 != dev_desc->block_read(dev_desc->dev, start, 1, temp)) {
			errorf("read start block 0x%x fail\n", start);
			goto fail;
		}
		if (size <= header_sz) {
			memcpy(buf, temp + offb, size);
			size = 0;
		} else {
			memcpy(buf, temp + offb, header_sz);
			start++;
			buf += header_sz;
			size -= header_sz;
		}
	}

	blkcnt = size / dev_desc->blksz;
	if (blkcnt > 0) {
		if (blkcnt != dev_desc->block_read(dev_desc->dev, start, blkcnt, buf)) {
			errorf("read block 0x%x size 0x%x fail\n", start, blkcnt);
			goto fail;
		}
		start += blkcnt;
		buf += (blkcnt * dev_desc->blksz);
		size = size % dev_desc->blksz;
	}

	if (size > 0) {
		if (1 != dev_desc->block_read(dev_desc->dev, start, 1, temp)) {
			errorf("read end block 0x%x fail\n", start);
			goto fail;
		}
		memcpy(buf, temp, size);
	}

	return 0;

fail:
	if (NULL != temp)
		free(temp);
	return -1;
}


int block_dev_write(block_dev_desc_t *dev_desc, uint64_t size, uint64_t offset, char *buf)
{
	lbaint_t start = 0;
	lbaint_t blkcnt = 0;
	char * temp = NULL;
	uint64_t header_sz = 0;
	uint64_t offb = 0;

	if ((0 != size % dev_desc->blksz) || (0 != offset % dev_desc->blksz)) {
		temp = malloc(dev_desc->blksz);
		if (NULL == temp)
			goto fail;
	}

	start = offset / dev_desc->blksz;
	offb = offset % dev_desc->blksz;
	if (0 != offb) {
		header_sz = dev_desc->blksz - offb;
		if (1 != dev_desc->block_read(dev_desc->dev, start, 1, temp)) {
			errorf("read start block 0x%x for write fail\n", start);
			goto fail;
		}
		if (size <= header_sz) {
			memcpy(temp + offb, buf, size);
			size = 0;
		} else {
			memcpy(temp + offb, buf, header_sz);
			buf += header_sz;
			size -= header_sz;
		}
		if (1 != dev_desc->block_write(dev_desc->dev, start, 1, temp)) {
			errorf("write start block 0x%x fail\n", start);
			goto fail;
		}
		start++;
	}

	blkcnt = size / dev_desc->blksz;
	if (blkcnt > 0) {
		if (blkcnt != dev_desc->block_write(dev_desc->dev, start, blkcnt, buf)) {
			errorf("write block 0x%x size 0x%x fail\n", start, blkcnt);
			goto fail;
		}
		start += blkcnt;
		buf += (blkcnt * dev_desc->blksz);
		size = size % dev_desc->blksz;
	}

	if (size > 0) {
		if (1 != dev_desc->block_read(dev_desc->dev, start, 1, temp)) {
			errorf("read end block 0x%x for write fail\n", start);
			goto fail;
		}
		memcpy(temp, buf, size);
		if (1 != dev_desc->block_write(dev_desc->dev, start, 1, temp)) {
			errorf("write end block 0x%x fail\n", start);
			goto fail;
		}
	}

	return 0;

fail:
	if (NULL != temp)
		free(temp);
	return -1;
}

int block_dev_erase(block_dev_desc_t *dev_desc, uint64_t size, uint64_t offset)
{
	lbaint_t start = 0;
	lbaint_t blkcnt = 0;
	char * temp = NULL;
	uint64_t header_sz = 0;
	uint64_t offb = 0;

	if ((0 != size % dev_desc->blksz) || (0 != offset % dev_desc->blksz)) {
		temp = malloc(dev_desc->blksz);
		if (NULL == temp)
			goto fail;
	}

	start = offset / dev_desc->blksz;
	offb = offset % dev_desc->blksz;
	if (0 != offb) {
		header_sz = dev_desc->blksz - offb;
		if (1 != dev_desc->block_read(dev_desc->dev, start, 1, temp)) {
			errorf("read start block 0x%x for erase fail\n", start);
			goto fail;
		}
		if (size <= header_sz) {
			memset(temp + offb, 0, size);
			size = 0;
		} else {
			memset(temp + offb, 0, header_sz);
			size -= header_sz;
		}
		if (1 != dev_desc->block_write(dev_desc->dev, start, 1, temp)) {
			errorf("erase start block 0x%x fail\n", start);
			goto fail;
		}
		start++;
	}

	blkcnt = size / dev_desc->blksz;
	if (blkcnt > 0) {
		if (blkcnt != dev_desc->block_erase(dev_desc->dev, start, blkcnt)) {
			errorf("erase block 0x%x size 0x%x fail\n", start, blkcnt);
			goto fail;
		}
		start += blkcnt;
		size = size % dev_desc->blksz;
	}

	if (size > 0) {
		if (1 != dev_desc->block_read(dev_desc->dev, start, 1, temp)) {
			errorf("read end block 0x%x for erase fail\n", start);
			goto fail;
		}
		memset(temp, 0, size);
		if (1 != dev_desc->block_write(dev_desc->dev, start, 1, temp)) {
			errorf("erase end block 0x%x fail\n", start);
			goto fail;
		}
	}

	return 0;

fail:
	if (NULL != temp)
		free(temp);
	return -1;
}


#ifdef CONFIG_UFS_BOOT

struct lu_info_tbl factory_lu_tbl[] = {
	{0x00, MAIN_BOOT, 		NORMAL_MEMORY, PROVISIONING_ENABLE_WITH_TPRZ, 0x0C, 1024},
	{0x01, ALTERNATE_BOOT, NORMAL_MEMORY, PROVISIONING_ENABLE_WITH_TPRZ, 0x0C, 1024},
	{0x02, NONE_BOOT, 		NORMAL_MEMORY, PROVISIONING_ENABLE_WITH_TPRZ, 0x0C, 0},
	{0	  , 0			   , 		0				  , 0								     , 0	, 0}
};



int get_lun_by_bootable(enum lu_bootable target)
{
	int id = 0;
	for (id = 0; id < UFS_MAX_LU_NUM; id++) {
		if (0 == factory_lu_tbl[id].log2blksz)
			break;
		if (factory_lu_tbl[id].bootable == target)
			return factory_lu_tbl[id].lu_index;
	}
	return -1;
}

int get_user_lun(disk_partition_t *part_info, char *part_name)
{
	int id = 0;
	int ret = 0;
	block_dev_desc_t *dev_desc;
	
	for (id = 0; id < UFS_MAX_LU_NUM; id++) {
		if (0 == factory_lu_tbl[id].log2blksz)
			break;
		if (factory_lu_tbl[id].bootable == NONE_BOOT) {
			dev_desc = get_dev("ufs", factory_lu_tbl[id].lu_index);
			if (NULL == dev_desc)
				continue;
			ret = get_partition_info_by_name(dev_desc, part_name, part_info);
			if (0 == ret)
				return factory_lu_tbl[id].lu_index;
		}
	}

	return -1;

}


int common_raw_read(char *part_name, uint64_t size, uint64_t offset, char *buf)
{
	int dev_id = 0;
	uint64_t part_start = 0;
	disk_partition_t part_info;
	block_dev_desc_t *dev_desc;
	uint64_t total_part_size = 0;

	if (0 == strcmp(part_name, "splloader")) {
		dev_id = get_lun_by_bootable(MAIN_BOOT);
	} else if (0 == strcmp(part_name, "splloader_bak")) {
		dev_id = get_lun_by_bootable(ALTERNATE_BOOT);
	} else if (0 == strcmp(part_name, "uboot")) {
		dev_id = get_user_lun(&part_info, part_name);
		if (-1 == dev_id) {
			dev_id = get_lun_by_bootable(ALTERNATE_BOOT);
		} else {
			part_start = (uint64_t)part_info.blksz * (uint64_t)part_info.start;
			total_part_size = (uint64_t)part_info.blksz * (uint64_t)part_info.size;
			if (size + offset > total_part_size)
				return -1;
		}
	} else {
		dev_id = get_user_lun(&part_info, part_name);
		part_start = (uint64_t)part_info.blksz * (uint64_t)part_info.start;
		total_part_size = (uint64_t)part_info.blksz * (uint64_t)part_info.size;
		if (size + offset > total_part_size)
			return -1;
	}

	if (-1 == dev_id)
		return -1;

	dev_desc = get_dev("ufs", dev_id);
	if (NULL == dev_desc)
		return -1;

	offset += part_start;

	return block_dev_read(dev_desc, size, offset, buf);
	
}


int common_raw_write(char *part_name, uint64_t size, uint64_t updsize, uint64_t offset, char *buf)
{
	int dev_id = 0;
	uint64_t part_start = 0;
	disk_partition_t part_info;
	block_dev_desc_t *dev_desc;
	uint64_t total_part_size = 0;

	if (0 == strcmp(part_name, "splloader")) {
		dev_id = get_lun_by_bootable(MAIN_BOOT);
	} else if(0 == strcmp(part_name, "splloader_bak")) {
		dev_id = get_lun_by_bootable(ALTERNATE_BOOT);
	} else if(0 == strcmp(part_name, "uboot")) {
		dev_id = get_user_lun(&part_info, part_name);
		if (-1 == dev_id) {
			dev_id = get_lun_by_bootable(ALTERNATE_BOOT);
		} else {
			part_start = (uint64_t)part_info.blksz * (uint64_t)part_info.start;
			total_part_size = (uint64_t)part_info.blksz * (uint64_t)part_info.size;
			if (size + offset > total_part_size)
				return -1;
		}
	} else {
		dev_id = get_user_lun(&part_info, part_name);
		part_start = (uint64_t)part_info.blksz * (uint64_t)part_info.start;
		total_part_size = (uint64_t)part_info.blksz * (uint64_t)part_info.size;
		if (size + offset > total_part_size)
			return -1;
	}

	if (-1 == dev_id)
		return -1;

	dev_desc = get_dev("ufs", dev_id);
	if (NULL == dev_desc)
		return -1;

	offset += part_start;

	return block_dev_write(dev_desc, size, offset, buf);
	
}

/*TODO*/
int common_raw_erase(char *part_name, uint64_t size, uint64_t offset)
{
	int dev_id = 0;
	lbaint_t part_start = 0;
	disk_partition_t part_info;
	block_dev_desc_t *dev_desc;
	uint64_t total_part_size = 0;
	/*TODO, if size=0, erase whole partition, if part_name=erase_all, erase whole flash*/
	if (0 == strcmp(part_name, "splloader")) {
		dev_id = get_lun_by_bootable(MAIN_BOOT);
	} else if (0 == strcmp(part_name, "uboot")) {
		dev_id = get_lun_by_bootable(ALTERNATE_BOOT);
	} else {
		dev_id = get_user_lun(&part_info, part_name);
		part_start = part_info.start;
		total_part_size = (uint64_t)part_info.blksz * (uint64_t)part_info.size;
		if (size + offset > total_part_size)
			return -1;
	}

	if (-1 == dev_id)
		return -1;

	dev_desc = get_dev("ufs", dev_id);
	if (NULL == dev_desc)
		return -1;

	offset += (uint64_t)part_start * dev_desc->blksz;

	/*TODO*/
	return block_dev_erase(dev_desc, size, offset);

}


int common_repartition(disk_partition_t *partitions, int parts_count)
{
	int dev_id = 0;
	block_dev_desc_t *dev_desc;
	int counter = 0;
	int ret = 0;

	/*future extention, now we only have one none boot lu*/
	dev_id = get_lun_by_bootable(NONE_BOOT);
	if (-1 == dev_id)
		return -1;

	dev_desc = get_dev("ufs", dev_id);
	if (NULL == dev_desc)
		return -1;

	while (3 < counter) {
		ret = gpt_restore(dev_desc, SPRD_DISK_GUID_STR, partitions, parts_count);
		if (0 == ret)
			break;
		counter++;
	}

	if (3 == counter) {
		return -1;
	} else {
		init_part(dev_desc);
		return 0;
	}
}

int32_t common_get_lba_size(void)
{
	int dev_id = 0;
	block_dev_desc_t *dev_desc;

	dev_id = get_lun_by_bootable(NONE_BOOT);
	if (-1 == dev_id)
		return -1;

	dev_desc = get_dev("ufs", dev_id);
	if (NULL == dev_desc)
		return -1;

	return (int32_t)dev_desc->blksz;
}

#endif

/*TODO*/
#ifdef CONFIG_EMMC_BOOT
int common_raw_read(char *part_name, uint64_t size, uint64_t offset, char *buf)
{
	uint64_t part_start = 0;
	disk_partition_t part_info;
	block_dev_desc_t *dev_desc;
	uint64_t total_part_size = 0;
	int ret = 0;

	if (0 == strcmp(part_name, "splloader")) {
		dev_desc = get_dev_hwpart("mmc", 0, PARTITION_BOOT1);
	} else if (0 == strcmp(part_name, "splloader_bak")) {
		dev_desc = get_dev_hwpart("mmc", 0, PARTITION_BOOT2);
	} else if (0 == strcmp(part_name, "uboot")) {
		dev_desc = get_dev_hwpart("mmc", 0, PARTITION_USER);
		ret = get_partition_info_by_name(dev_desc, part_name, &part_info);
		if (-1 == ret) {
			ret = 0;
			dev_desc = get_dev_hwpart("mmc", 0, PARTITION_BOOT2);
		} else {
			part_start = (uint64_t)part_info.start * (uint64_t)part_info.blksz;
			total_part_size = (uint64_t)part_info.blksz * (uint64_t)part_info.size;
			if (size + offset > total_part_size)
				ret = -1;
		}
	} else {
		dev_desc = get_dev_hwpart("mmc", 0, PARTITION_USER);
		ret = get_partition_info_by_name(dev_desc, part_name, &part_info);
		part_start = (uint64_t)part_info.start * (uint64_t)part_info.blksz;
		total_part_size = (uint64_t)part_info.blksz * (uint64_t)part_info.size;
		if (size + offset > total_part_size) {
			errorf("read size(0x%llx + 0x%llx) overflow the total partition size(0x%llx)\n", size, offset, total_part_size);
			ret = -1;
		}
	}

	if (-1 == ret || NULL == dev_desc)
		return -1;

	offset += part_start;

	return block_dev_read(dev_desc, size, offset, buf);
	
}


int common_raw_write(char *part_name, uint64_t size, uint64_t updsize, uint64_t offset, char *buf)
{
	uint64_t part_start = 0;
	disk_partition_t part_info;
	block_dev_desc_t *dev_desc;
	uint64_t total_part_size = 0;
	int ret = 0;

	if (0 == strcmp(part_name, "splloader")) {
		dev_desc = get_dev_hwpart("mmc", 0, PARTITION_BOOT1);
	} else if (0 == strcmp(part_name, "splloader_bak")) {
		dev_desc = get_dev_hwpart("mmc", 0, PARTITION_BOOT2);
	} else if (0 == strcmp(part_name, "uboot")) {
		dev_desc = get_dev_hwpart("mmc", 0, PARTITION_USER);
		ret = get_partition_info_by_name(dev_desc, part_name, &part_info);
		if (-1 == ret) {
			debugf("no uboot partition in user hardwarepart, write to boot2\n");
			dev_desc = get_dev_hwpart("mmc", 0, PARTITION_BOOT2);
			ret = 0;
		} else {
			part_start = (uint64_t)part_info.start * (uint64_t)part_info.blksz;
			total_part_size = (uint64_t)part_info.blksz * (uint64_t)part_info.size;
			if (size + offset > total_part_size)
				ret = -1;
		}
	} else {
		dev_desc = get_dev_hwpart("mmc", 0, PARTITION_USER);
		ret = get_partition_info_by_name(dev_desc, part_name, &part_info);
		part_start = (uint64_t)part_info.start * (uint64_t)part_info.blksz;
		total_part_size = (uint64_t)part_info.blksz * (uint64_t)part_info.size;
		if (size + offset > total_part_size) {
			errorf("write size(0x%llx) + offset(0x%llx) overflow the total partition size(0x%llx)\n", size, offset, total_part_size);
			ret = -1;
		}
	}

	if (-1 == ret || NULL == dev_desc)
		return -1;

	offset += part_start;

	return block_dev_write(dev_desc, size, offset, buf);
	
}

int common_raw_erase(char *part_name, uint64_t size, uint64_t offset)
{
	uint64_t part_start = 0;
	disk_partition_t part_info;
	block_dev_desc_t *dev_desc;
	uint64_t total_part_size = 0;
	int ret = 0;

	if (0 == strcmp(part_name, "erase_all")) {
		debugf("erase all the flash\n");
		dev_desc = get_dev_hwpart("mmc", 0, PARTITION_BOOT1);
		total_part_size = Emmc_GetCapacity(PARTITION_BOOT1);
		if (NULL == dev_desc || 0 == total_part_size)
			return -1;
		if (0 != block_dev_erase(dev_desc, total_part_size, 0))
			return -1;

		dev_desc = get_dev_hwpart("mmc", 0, PARTITION_BOOT2);
		total_part_size = Emmc_GetCapacity(PARTITION_BOOT2);
		if (NULL == dev_desc || 0 == total_part_size)
			return -1;
		if (0 != block_dev_erase(dev_desc, total_part_size, 0))
			return -1;

		dev_desc = get_dev_hwpart("mmc", 0, PARTITION_USER);
		total_part_size = Emmc_GetCapacity(PARTITION_USER);
		if (NULL == dev_desc || 0 == total_part_size)
			return -1;
		if (0 != block_dev_erase(dev_desc, total_part_size, 0))
			return -1;
		return 0;
	}

	/*TODO, if size=0, erase whole partition, if part_name=erase_all, erase whole flash*/
	if (0 == strcmp(part_name, "splloader")) {
		dev_desc = get_dev_hwpart("mmc", 0, PARTITION_BOOT1);
		total_part_size = Emmc_GetCapacity(PARTITION_BOOT1);
	} else if (0 == strcmp(part_name, "splloader_bak")) {
		dev_desc = get_dev_hwpart("mmc", 0, PARTITION_BOOT2);
		total_part_size = Emmc_GetCapacity(PARTITION_BOOT2);
	} else if (0 == strcmp(part_name, "uboot")) {
		dev_desc = get_dev_hwpart("mmc", 0, PARTITION_USER);
		ret = get_partition_info_by_name(dev_desc, part_name, &part_info);
		if (-1 == ret) {
			ret = 0;
			dev_desc = get_dev_hwpart("mmc", 0, PARTITION_BOOT2);
			total_part_size = Emmc_GetCapacity(PARTITION_BOOT2);
		} else {
			part_start = (uint64_t)part_info.start * (uint64_t)part_info.blksz;
			total_part_size = (uint64_t)part_info.blksz * (uint64_t)part_info.size;
		}
	} else {
		dev_desc = get_dev_hwpart("mmc", 0, PARTITION_USER);
		ret = get_partition_info_by_name(dev_desc, part_name, &part_info);
		part_start = (uint64_t)part_info.start * (uint64_t)part_info.blksz;
		total_part_size = (uint64_t)part_info.blksz * (uint64_t)part_info.size;
	}

	if (-1 == ret || NULL == dev_desc || 0 == total_part_size)
		return -1;

	if (0 == size) {
		size = total_part_size;
		offset = part_start;
	} else if (size + offset > total_part_size) {
		return -1;
	} else {
		offset += part_start;
	}

	return block_dev_erase(dev_desc, size, offset);

}

int common_write_Backstage(char *part_name, uint32_t size, uint32_t offset, char *buf)
{
	uint32_t start = 0;
	uint32_t blk_num = 0;
	uint64_t total_part_size = 0;
	block_dev_desc_t *dev_desc = NULL;
	int ret = 0;
	if (0 != strcmp(part_name, local_part_info.name)) {
		dev_desc = get_dev_hwpart("mmc", 0, PARTITION_USER);
		ret = get_partition_info_by_name(dev_desc, part_name, &local_part_info);
	}

	/*invoker must guarantee that the size and offset aligned to the block size*/
	if (0 != size % local_part_info.blksz || 0 != offset % local_part_info.blksz) {
		errorf("input size or offset in not aligned, size(0x%x), offset(0x%x)\n", size, offset);
		ret = -1;
	}

	blk_num = size / local_part_info.blksz;
	start = local_part_info.start + (offset / local_part_info.blksz);
	if (blk_num + start > local_part_info.size + local_part_info.start) {
		errorf("backstage write size overflow the total partition size\n");
		ret = -1;
	}

	if (-1 == ret)
		return -1;

	/*only user partition can use backstage write*/
	if (blk_num != emmc_write_backstage(PARTITION_USER, start, blk_num, buf))
		return -1;

	return 0;
}


int common_query_backstage(char *part_name, uint64_t size, char *buf)
{
	uint32_t blk_num = 0;
	if (0 != strcmp(part_name, local_part_info.name))
		return -1;
	blk_num = size / local_part_info.blksz;
	if (blk_num != emmc_query_backstage(PARTITION_USER, blk_num, buf))
		return -1;

	return 0;
}


int common_repartition(disk_partition_t *partitions, int parts_count)
{
	block_dev_desc_t *dev_desc;
	int counter = 0;
	int ret = 0;

	memset(&local_part_info, 0 , sizeof(disk_partition_t));
	dev_desc = get_dev_hwpart("mmc", 0, PARTITION_USER);
	if (NULL == dev_desc) {
		errorf("get mmc device hardware part(%d) fail\n", PARTITION_USER);
		return -1;
	}

	while (counter < 3) {
		ret = gpt_restore(dev_desc, SPRD_DISK_GUID_STR, partitions, parts_count);
		if (0 == ret)
			break;
		counter++;
	}

	if (3 == counter) {
		return -1;
	} else {
		init_part(dev_desc);
		return 0;
	}
}

int32_t common_get_lba_size(void)
{
	block_dev_desc_t *dev_desc;

	dev_desc = get_dev_hwpart("mmc", 0, PARTITION_USER);
	if (NULL == dev_desc)
		return -1;

	return (int32_t)dev_desc->blksz;
}

#endif

