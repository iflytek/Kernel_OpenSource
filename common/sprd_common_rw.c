#include <common.h>
#include <malloc.h>
#ifdef CONFIG_CMD_UBI
#include <ubi_uboot.h>
#endif
#ifdef  CONFIG_EMMC_BOOT
#include <mmc.h>
#endif


int do_fs_file_read(char *mpart, char *filenm, void *buf, int len)
{
#ifdef CONFIG_FS_EXT4
	disk_partition_t info;
	block_dev_desc_t *dev_desc;

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
	if (ext4_read_file(filenm, buf, 0,len) == -1) {
		printf("** Error ext4_read_file() **\n");
		ext4fs_close();
		return -1;
	}

	ext4fs_close();
	return 0;
#elif defined(CONFIG_FS_UBIFS)
	int ret=-1;
	static int is_ubifs_init = 0;
	if(is_ubifs_init == 0){
		ubifs_init();
		is_ubifs_init = 1;
	}

	ret = ubifs_mount(mpart);
	if(ret){
		printf("do_fs_file_read:mount %s failed!\n",mpart);
		return ret;
	}
	ret = ubifs_load(filenm, buf, len);
	if(ret)
		printf("do_fs_file_read:file %s not found!\n", filenm);
	return ret;
#endif
}

int do_fs_file_write(char *mpart, char *filenm, void *buf, int len)
{
	/*do not write in uboot now*/
	return -1;
}



int get_partition_info_by_name (block_dev_desc_t *dev_desc, uchar * partition_name,
						disk_partition_t *info)
{
	switch(dev_desc->part_type){

	case PART_TYPE_EFI:
		if (get_partition_info_efi_by_name(dev_desc, partition_name, info)== 0) {
			//PRINTF ("## Valid EFI partition found ##\n");
			return (0);
		}
		break;
	default:
		break;
	}
	return -1;
}

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
		if (1 != dev_desc->block_read(dev_desc->dev, start, 1, temp))
			goto fail;
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
		if (blkcnt != dev_desc->block_read(dev_desc->dev, start, blkcnt, buf))
			goto fail;
		start += blkcnt;
		buf += (blkcnt * dev_desc->blksz);
		size = size % dev_desc->blksz;
	}

	if (size > 0) {
		if (1 != dev_desc->block_read(dev_desc->dev, start, 1, temp))
			goto fail;
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
		if (1 != dev_desc->block_read(dev_desc->dev, start, 1, temp))
			goto fail;
		if (size <= header_sz) {
			memcpy(temp + offb, buf, size);
			size = 0;
		} else {
			memcpy(temp + offb, buf, header_sz);
			start++;
			buf += header_sz;
			size -= header_sz;
		}
		if (1 != dev_desc->block_write(dev_desc->dev, start, 1, temp))
			goto fail;
	}

	blkcnt = size / dev_desc->blksz;
	if (blkcnt > 0) {
		if (blkcnt != dev_desc->block_write(dev_desc->dev, start, blkcnt, buf))
			goto fail;
		start += blkcnt;
		buf += (blkcnt * dev_desc->blksz);
		size = size % dev_desc->blksz;
	}

	if (size > 0) {
		if (1 != dev_desc->block_read(dev_desc->dev, start, 1, temp))
			goto fail;
		memcpy(temp, buf, size);
		if (1 != dev_desc->block_write(dev_desc->dev, start, 1, temp))
			goto fail;
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
	lbaint_t part_start = 0;
	disk_partition_t part_info;
	block_dev_desc_t *dev_desc;
	uint64_t total_part_size = 0;

	if (0 == strcmp(part_name, "splloader")) {
		dev_id = get_lun_by_bootable(MAIN_BOOT);
	} else if (0 == strcmp(part_name, "uboot")) {
		dev_id = get_lun_by_bootable(ALTERNATE_BOOT);
	} else {
		dev_id = get_user_lun(&part_info, part_name);
		part_start = part_info.start;
		total_part_size = (uint64_t)part_info.blksz * (uint64_t)part_info.size;
		if (size + offset < total_part_size)
			return -1;
	}

	if (-1 == dev_id)
		return -1;

	dev_desc = get_dev("ufs", dev_id);
	if (NULL == dev_desc)
		return -1;

	offset += (uint64_t)part_start * dev_desc->blksz;
	
	return block_dev_read(dev_desc, size, offset, buf);
	
}


int common_raw_write(char *part_name, uint64_t size, uint64_t updsize, uint64_t offset, char *buf)
{
	int dev_id = 0;
	lbaint_t part_start = 0;
	disk_partition_t part_info;
	block_dev_desc_t *dev_desc;
	uint64_t total_part_size = 0;

	if (0 == strcmp(part_name, "splloader")) {
		dev_id = get_lun_by_bootable(MAIN_BOOT);
	} else if(0 == strcmp(part_name, "splloader_bak")) {
		dev_id = get_lun_by_bootable(ALTERNATE_BOOT);
	} else if(0 == strcmp(part_name, "uboot")) {
		dev_id = get_lun_by_bootable(ALTERNATE_BOOT);
	} else if(0 == strcmp(part_name, "uboot_main")) {
		dev_id = get_user_lun(&part_info, "uboot");
		part_start = part_info.start;
		total_part_size = (uint64_t)part_info.blksz * (uint64_t)part_info.size;
		if (size + offset < total_part_size)
			return -1;
	} else {
		dev_id = get_user_lun(&part_info, part_name);
		part_start = part_info.start;
		total_part_size = (uint64_t)part_info.blksz * (uint64_t)part_info.size;
		if (size + offset < total_part_size)
			return -1;
	}

	if (-1 == dev_id)
		return -1;
	
	dev_desc = get_dev("ufs", dev_id);
	if (NULL == dev_desc)
		return -1;

	offset += (uint64_t)part_start * dev_desc->blksz;
	
	return block_dev_write(dev_desc, size, offset, buf);
	
}

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
		if (size + offset < total_part_size)
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

	dev_desc = get_dev("ufs", dev_id);
	if (NULL == dev_desc)
		return -1;

	/*future extention, now we only have one none boot lu*/
	dev_id = get_lun_by_bootable(NONE_BOOT);
	if (-1 == dev_id)
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

#endif

/*TODO*/
#ifdef CONFIG_EMMC_BOOT

#endif




