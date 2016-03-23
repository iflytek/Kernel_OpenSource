#ifndef __COUPLING_INFO_H__
#define __COUPLING_INFO_H__

#define HEADER_INFO_MAGIC	0x5043454D
#define HEADER_INFO_VERSION	0x302E3156

#define MAX_HEAD_LENGTH	0x100
#define MAX_REGION_NUM	0x8
#define MAX_NAME_LEN	0x14
#define MAX_VALUE_LEN	0x100
#define MAX_RSVMEM_NUM	0x4
#define MAX_RES_NUM		0x4
#define MAX_CODE_NUM	0x32

#define EXPAND_FDT_SIZE		0x400

#define PHY_BASE_ADDR		0x80000000

#ifdef CONFIG_SUPPORT_LTE
#define PARTITION_PREFIX	"l_"
#endif
#define NV_PARTITION_KEY	"nv"
#define	DECOUPLING_INFO_PARTITION	"modem"

enum {
	be_ignore = 0,
	re_load, //require load to ddr
	re_clear //require clear
};

enum {
	str_t,
	num_t,
};

enum {
	init_prop,
	bad_prop,
	filled_prop,
};

struct partition_name_info {
	char partition[MAX_NAME_LEN];
	char partition_bk[MAX_NAME_LEN];
};

struct keyinfo {
	char name[MAX_NAME_LEN];
	char value[MAX_VALUE_LEN];
};

struct decoup_conf {
	struct keyinfo key;
	int (*do_parse_add)(struct keyinfo *key, struct cp_mem_coupling_info *info, struct fdt_header *fdt);
};

struct property {
	char name[MAX_NAME_LEN];
	char value[MAX_VALUE_LEN];
	uint32_t len;
	uint32_t offset;
	uint32_t flag;
};

struct mem_resource {
	char name[MAX_NAME_LEN];
	uint64_t base;
	uint32_t size;
};

struct region_info {
	struct mem_resource res;
	uint32_t flag; //load flag
};

struct boot_code {
	uint32_t count;
	uint32_t code[MAX_CODE_NUM];
};

struct rsvmem_info_conf {
	struct region_info rsvmem[MAX_RSVMEM_NUM];
};

struct boot_info_conf {
	struct boot_code bcode;
	struct region_info regions[MAX_REGION_NUM];
	struct mem_resource res[MAX_RES_NUM];
	uint32_t ex_info;
};

struct comm_info_conf {
	struct mem_resource res[MAX_RES_NUM];
	uint32_t ex_info;
};

struct cp_mem_coupling_info {
	uint32_t magic;
	uint32_t version;
	uint32_t length;
	uint32_t ex_info;
	struct rsvmem_info_conf rsvmem_info;
	struct boot_info_conf boot_info;
	struct comm_info_conf comm_info;
};

int fdt_fixup_cp_coupling_info(void *blob);
#endif
