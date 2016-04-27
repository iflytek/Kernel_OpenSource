#include <common.h>
#include <command.h>
#include <linux/types.h>
#include <part.h>

#include <sprd_common_rw.h>
#include <linux/sizes.h>
#include <malloc.h>

int test_ufs_repart(int argc, char * argv[])
{
	disk_partition_t *partition_info = NULL;
	int part_num = 0;
	int32_t blksz = 0;
	uint32_t lbas_per_m = 0;
	lbaint_t partition_start_lba = 0;
	int i = 0;
	uint32_t partition_size_m = 0;
	int arg_id = 0;
	int ret = 0;
	
	if (0 != argc % 2) {
		errorf("illegal arguement number\n");
		return CMD_RET_USAGE;
	}

	part_num = argc / 2;

	partition_info = malloc(sizeof(disk_partition_t) * part_num);
	if (NULL == partition_info) {
		errorf("No space to store partition_info!\n");
		return CMD_RET_FAILURE;
	}

	blksz = common_get_lba_size();
	if (-1 == blksz) {
		errorf("can not get lba size!\n");
		free(partition_info);
		return CMD_RET_FAILURE;
	}
	lbas_per_m = SZ_1M / blksz;

	partition_start_lba = SZ_1M / blksz;

	for (i = 0; i < part_num; i++) {
		arg_id = i * 2;
		partition_info[i].blksz = (ulong)blksz;
		strcpy(partition_info[i].type, "U-boot");	/*unuseful */
		
		partition_size_m = (uint32_t) simple_strtoul(argv[arg_id + 1], NULL, 10);
		partition_info[i].size = (lbaint_t) partition_size_m * lbas_per_m;
		strcpy(partition_info[i].name, argv[arg_id]);
		partition_info[i].start = partition_start_lba;
		partition_start_lba += partition_info[i].size;

		debugf("partition name:%s,partition_size:0x%lx,partiton_start:0x%lx,\n", partition_info[i].name, partition_info[i].size,
		       partition_info[i].start);
	}

	ret = common_repartition(partition_info, part_num);
	debugf("Common repartition for UFS result=%d\n", ret);
	free(partition_info);

	if (0 == ret)
		return 0;
	else
		return CMD_RET_FAILURE;
}

/*TODO*/
int test_ufs_read(int argc, char * argv[])
{
	return 0;
}

int test_ufs_write(int argc, char * argv[])
{
	return 0;
}

int test_ufs_erase(int argc, char * argv[])
{
	return 0;
}

int do_ufs(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{


	if (0 == strcmp(argv[1], "repart")) {
		debugf("enter repart command\n");
		argc -= 2;
		return test_ufs_repart(argc, &argv[2]);
	}

	if (0 == strcmp(argv[1], "read")) {
		argc -= 2;
		return test_ufs_read(argc, &argv[2]);
	}

	if (0 == strcmp(argv[1], "write")) {
		argc -= 2;
		return test_ufs_write(argc, &argv[2]);
	}

	if (0 == strcmp(argv[1], "erase")) {
		argc -= 2;
		return test_ufs_erase(argc, &argv[2]);
	}

	return CMD_RET_USAGE;
}


U_BOOT_CMD(
	ufs, CONFIG_SYS_MAXARGS, 1, do_ufs,
	"ufs test commands\n",
	"ufs repart [part] [size(in mega)]..."
		" - Repartition UFS device, support no more than 7 partitions\n"
	"ufs read"
		" - 1\n"
	"ufs write"
		" - 2\n"
	"ufs erase"
		" - 3\n"
);

