#ifndef _SPRD_COMMON_RW_H_
#define _SPRD_COMMON_RW_H_

#define SPRD_DISK_GUID_STR "11111111-2222-3333-4444-000000000000"

int common_raw_read(char *part_name, uint64_t size, uint64_t offset, char *buf);
int common_raw_write(char *part_name, uint64_t size, uint64_t updsize, uint64_t offset, char *buf);
int common_raw_erase(char *part_name, uint64_t size, uint64_t offset);
int common_repartition(disk_partition_t *partitions, int parts_count);
int32_t common_get_lba_size(void);

#endif /* _LOADER_COMMON_H_ */
