#include <common.h>
#include <libfdt_env.h>
#include <libfdt.h>
#include "cp_mem_decoupling.h"
#include "loader_common.h"
#include <secureboot/sec_common.h>

//#define CP_MEM_DECOUP_DEBUG

extern int _boot_load_required_image(boot_image_required_t img_info);
extern void of_bus_default_count_cells(void *blob, int parentoffset,
					int *addrc, int *sizec);

static boot_image_required_t s_boot_image_cp_table[MAX_REGION_NUM] = {0};
static struct partition_name_info g_partition_info[MAX_REGION_NUM] = {0};

static struct cp_mem_coupling_info g_info;

static void print_fdt_rsvmem(struct fdt_header *fdt)
{
#ifdef CP_MEM_DECOUP_DEBUG
	char fdt_cmd[MAX_VALUE_LEN] = {0};

	sprintf(fdt_cmd, "fdt addr %p", fdt);
	run_command(fdt_cmd, 0);
	run_command("fdt rsvmem print", 0);
#endif
	return;
}

static void print_fdt_node(struct fdt_header *fdt, int noffs)
{
#ifdef CP_MEM_DECOUP_DEBUG
	int index = 0;
	char fdt_cmd[MAX_VALUE_LEN] = {0};

	sprintf(fdt_cmd, "fdt addr %p", fdt);
	run_command(fdt_cmd, 0);

	memset(fdt_cmd, 0, sizeof(fdt_cmd));
	strcpy(fdt_cmd, "fdt print ");
	index = strlen(fdt_cmd);
	if(fdt_get_path(fdt, noffs, fdt_cmd + index, sizeof(fdt_cmd) - index) == 0) {
		run_command(fdt_cmd, 0);
	}
#endif
	return;
}

static int prop_init(char *name, uint32_t noffs, struct property *prop)
{
	int num;

	if(NULL == name || NULL == prop) {
		errorf("%s: param error\n", __func__);
		return -1;
	}

	memset(prop, 0, sizeof(struct property));
	num = min(sizeof(prop->name) - 1, strlen(name));
	strncpy(prop->name, name, num);
	prop->offset = noffs;

	return 0;
}

static int fill_prop_value(int type, void *value, int len, struct property *prop)
{
	int i, n, lenp;
	uint32_t val;
	char *s = NULL;
	char *p = NULL;

	if(NULL == value || NULL == prop || 0 == len) {
		errorf("%s: param error\n", __func__);
		return -1;
	}

	if(len > sizeof(prop->value)) {
		errorf("%s: <%s> fill value too big, len = %d\n", __func__, prop->name, len);
		return 0;
	}

	lenp = prop->len;
	p = prop->value + lenp;

	if(type == str_t) { //property value is string type
		strcpy(p, value);
		lenp += strlen(value) + 1;

	} else {
		n = len / sizeof(uint32_t);
		if(!n)
			n = 1;

		memset(p, 0, n * sizeof(uint32_t));

		p += n * sizeof(uint32_t);
		lenp += n * sizeof(uint32_t);

		for(s = (char *)value + len ; n > 0; n--) {
			s -= sizeof(uint32_t);
			p -= sizeof(uint32_t);
			memcpy(&val, s, sizeof(uint32_t));
			val = __cpu_to_be32(val);
			memcpy(p, &val, sizeof(uint32_t));
		}
	}

	prop->len = lenp;
	prop->flag = filled_prop;

	return 0;
}

static int fill_cell_prop_value(void *value, uint32_t size, int cell, struct property *prop)
{
	int i, n, delta;
	char temp[64];
	char *p = NULL;

	if(!value || !prop) {
		errorf("%s: param error\n", __func__);
		return -1;
	}

	p = (char *)value;
	n = cell * sizeof(uint32_t) < size ? cell * sizeof(uint32_t) : size;
	delta = cell * sizeof(uint32_t) - sizeof(uint32_t);
	memset(temp, 0, sizeof(temp));
	for(i = cell; i > 0; i--) {
		memcpy(temp + delta, p, sizeof(uint32_t));
		delta -= sizeof(uint32_t);
		p += sizeof(uint32_t);
	}

	return fill_prop_value(num_t, temp, cell * sizeof(uint32_t), prop);
}

static int fill_reg_prop_value(uint64_t base, uint64_t size, int ac, int sc, struct property *prop)
{
	if(ac > 2 || sc > 2 || !prop) {
		errorf("%s: param error\n", __func__);
		return -1;
	}

	if(size == 0) {
		errorf("%s: size = 0, invalid\n", __func__);
		return 0;
	}

	if(fill_cell_prop_value(&base, sizeof(base), ac, prop)) {
		errorf("%s: fill base to %s prop failed.\n", __func__, prop->name);
		return -1;
	}

	if(fill_cell_prop_value(&size, sizeof(size), sc, prop)) {
		errorf("%s: fill size to %s prop failed.\n", __func__, prop->name);
		return -1;
	}

	return 0;
}

static int fill_ranges_prop_value(uint64_t offs, uint64_t base,
					uint64_t size, int c_ac, int ac, int c_sc, struct property *prop)
{
	if(ac > 2 || c_ac > 2 || c_sc > 2 || !prop) {
		errorf("%s: param error\n", __func__);
		return -1;
	}

	if(fill_cell_prop_value(&offs, sizeof(offs), c_ac, prop)) {
		errorf("%s: fill base to %s prop failed.\n", __func__, prop->name);
		return -1;
	}

	if(fill_cell_prop_value(&base, sizeof(base), ac, prop)) {
		errorf("%s: fill base to %s prop failed.\n", __func__, prop->name);
		return -1;
	}

	if(fill_cell_prop_value(&size, sizeof(size), c_sc, prop)) {
		errorf("%s: fill size to %s prop failed.\n", __func__, prop->name);
		return -1;
	}

	return 0;
}

static int do_add_prop_to_fdt(struct fdt_header *fdt, int mode, struct property *prop)
{
	int ret;

	if(NULL == prop || NULL == fdt) {
		errorf("%s: param error\n", __func__);
		return -1;
	}

	if(filled_prop != prop->flag) {
		errorf("%s: %s prop is bad or no value, do not add to fdt\n", __func__, prop->name);
		return -1;
	}

	if(mode != 0) {
		ret = fdt_appendprop(fdt, prop->offset, prop->name, prop->value, prop->len);
	} else {
		ret = fdt_setprop(fdt, prop->offset, prop->name, prop->value, prop->len);
	}

	if (ret < 0) {
		errorf("%s: add prop %s to %s failed, reason: %s\n",
			__func__, prop->name, fdt_get_name(fdt, prop->offset, 0), fdt_strerror(ret));
		return -1;
	}

	return 0;
}

static int do_parse_add_sipc_info(struct keyinfo *key,
	struct cp_mem_coupling_info *info, struct fdt_header *fdt)
{
	int i;
	int p_offs, offs;
	uint64_t base;
	uint32_t deta, size;
	uint32_t na = 2, ns = 1;
	uint32_t c_na = 2, c_ns = 1;
	struct property reg, c_reg, ranges;
	struct comm_info_conf *comm_info = NULL;

	if(NULL == key || NULL == info || NULL == fdt) {
		errorf("param error\n");
		goto err;
	}

	comm_info = &info->comm_info;
	offs = fdt_node_offset_by_prop_value(fdt, 0, key->name, key->value, strlen(key->value) + 1);
	if(offs < 0) {
		errorf("the sipc node do not support decoupling\n");
		return 1;
	}

	p_offs = fdt_parent_offset(fdt, offs);
	if(p_offs < 0) {
		na = 2;
		ns = 1;
	}

	debugf("start update sipc node info\n");
	print_fdt_node(fdt, offs);

	of_bus_default_count_cells(fdt, p_offs, &na, &ns);
	debugf("a_cells = %d, s_cells = %d\n", na, ns);

	of_bus_default_count_cells(fdt, offs, &c_na, &c_ns);
	debugf("ca_cells: %d, cs_cells: %d\n", c_na, c_ns);

	prop_init("reg", 0, &c_reg);
	prop_init("reg", offs, &reg);
	prop_init("ranges", offs, &ranges);

	for(i = 0; comm_info->res[i].size > 0; i++) {
		base = comm_info->res[i].base;
		size = comm_info->res[i].size;
		deta = comm_info->res[i].base - PHY_BASE_ADDR;
		debugf("%s: i = %d, base = 0x%x, size = 0x%x\n", i, base, size);
		if (i == 0) {
			fill_reg_prop_value(base, size, na, ns, &reg);
		}

		fill_reg_prop_value(deta, size, c_na, c_ns, &c_reg);

		fill_ranges_prop_value(deta, base, size, c_na, na, c_ns, &ranges);
	}

	if(do_add_prop_to_fdt(fdt, 0, &reg) < 0) {
		errorf("update reg property to sipc node failed\n");
		goto err;
	}

	if(do_add_prop_to_fdt(fdt, 1, &ranges) < 0) {
		errorf("update ranges property to sipc node failed\n");
		goto err;
	}

	c_reg.offset = fdt_first_subnode(fdt, offs);
	if(c_reg.offset < 0) {
		errorf("the sipc child node not found\n");
		goto err;
	}

	if(do_add_prop_to_fdt(fdt, 0, &c_reg) < 0) {
		errorf("update reg property to sipc child node failed\n");
		goto err;
	}

	debugf("update sipc node info done\n");
	print_fdt_node(fdt, offs);

	return 0;

err:
	print_fdt_node(fdt, offs);
	return -1;
}

static int do_parse_add_cproc_info(struct keyinfo *key,
	struct cp_mem_coupling_info *info, struct fdt_header *fdt)
{
	int i;
	int p_offs, offs, c_offs;
	uint64_t base;
	uint32_t size;
	uint32_t deta, cp_base;
	uint32_t na, ns;
	uint32_t c_na, c_ns;
	char node_name[2 * MAX_NAME_LEN] = {0};
	struct property prop;
	struct property ranges;
	struct region_info *region = NULL;
	struct boot_info_conf *binfo = NULL;

	debugf("start update cproc node info\n");

	if(NULL == key || NULL == info || NULL == fdt) {
		errorf("%s: param error\n", __func__);
		goto err0;
	}

	binfo = &info->boot_info;
	offs = fdt_node_offset_by_prop_value(fdt, 0, key->name, key->value, strlen(key->value) + 1);
	if(offs < 0) {
		errorf("%s: the cproc node do not support decoupling\n", __func__);
		return 1;
	}

	p_offs = fdt_parent_offset(fdt, offs);
	if(p_offs < 0) {
		na = 2;
		ns = 1;
	}

	print_fdt_node(fdt, offs);

	of_bus_default_count_cells(fdt, p_offs, &na, &ns);
	debugf("%s: a_cells: %d, s_cells: %d\n", __func__, na, ns);

	prop_init("reg", offs, &prop);
	for(i = 0; binfo->res[i].size > 0; i++) {
		base = binfo->res[i].base;
		size = binfo->res[i].size;

		if (i == 0)
			cp_base = base;

		fill_reg_prop_value(base, size, na, ns, &prop);
	}

	if(do_add_prop_to_fdt(fdt, 1, &prop) < 0) {
		errorf("%s: add reg property to cproc node failed\n", __func__);
		goto err0;
	}

	prop_init("sprd,iram-dsize", offs, &prop);
	fill_prop_value(num_t, &binfo->bcode.count, sizeof(uint32_t), &prop);
	if(do_add_prop_to_fdt(fdt, 0, &prop) < 0) {
		errorf("%s: add iram_dsize property to cproc node failed\n", __func__);
	}

	prop_init("sprd,iram-data", offs, &prop);
	fill_prop_value(num_t, binfo->bcode.code, binfo->bcode.count * sizeof(uint32_t), &prop);
	if(do_add_prop_to_fdt(fdt, 0, &prop) < 0) {
		errorf("%s: add iram_data property to cproc node failed\n", __func__);
	}

	for(i = 0, region = binfo->regions; i < MAX_REGION_NUM; i++, region++) {
		if(strlen(region->res.name) > 0) {
			deta = region->res.base - cp_base;
			c_offs = fdt_node_offset_by_prop_value(fdt, offs, "cproc,name",
					region->res.name, strlen(region->res.name) + 1);
			if(c_offs == -FDT_ERR_NOTFOUND) {
				sprintf(node_name, "%s@%x", region->res.name, deta);
				c_offs = fdt_add_subnode(fdt, offs, node_name);
				if(c_offs < 0) {
					errorf("%s: create %s node failed\n", __func__, node_name);
					continue;
				}
			}

			of_bus_default_count_cells(fdt, offs, &c_na, &c_ns);

			prop_init("reg", c_offs, &prop);
			fill_reg_prop_value(deta, region->res.size, c_na, c_ns, &prop);
			if(do_add_prop_to_fdt(fdt, 0, &prop) < 0) {
				errorf("%s: add reg property failed, in %s node\n", __func__, node_name);
				goto err1;
			}

			prop_init("cproc,name", c_offs, &prop);
			fill_prop_value(str_t, region->res.name, sizeof(region->res.name), &prop);
			if(do_add_prop_to_fdt(fdt, 0, &prop) < 0) {
				errorf("%s: add %s property failed, in %s node\n", __func__, prop.name, node_name);
				goto err1;
			}

			prop_init("ranges", offs, &ranges);
			fill_ranges_prop_value(deta, region->res.base, region->res.size, c_na, na, c_ns, &ranges);
			if(do_add_prop_to_fdt(fdt, 1, &ranges) < 0) {
				errorf("%s: add ranges property to cproc node failed\n", __func__);
				goto err1;
			}

			continue;
err1:
			fdt_del_node(fdt, c_offs);
			continue;

		} else {
			break;
		}
	}

	debugf("update cproc node info done\n");
	print_fdt_node(fdt, offs);

	return 0;

err0:
	print_fdt_node(fdt, offs);
	return -1;
}

static int do_parse_add_rsvmem_info(struct keyinfo *key,
	struct cp_mem_coupling_info *info, struct fdt_header *fdt)
{
	int i, j, err;
	int len;
	int node, offs;
	int na, ns;
	int na_t;
	fdt32_t cells[4];
	uint64_t base, r;
	char name[64];
	char *p;
	struct region_info *rsvmem;

	if(NULL == info || NULL == fdt)	{
		errorf("%s: param error\n", __func__);
		return -1;
	}

	debugf("start add cp rsvmem to fdt\n");

	node = fdt_path_offset(fdt, "/reserved-memory");
	if (node < 0) {
		errorf("/reserved-memory node not exist\n");
		return -1;
	}
	print_fdt_node(fdt, node);

	for(rsvmem = info->rsvmem_info.rsvmem, i = 0; i < MAX_RSVMEM_NUM; i++, rsvmem++) {
		if(rsvmem->res.size == 0) {
			break;
		}

		debugf("rsvbase = 0x%p, rsvsize = 0x%p\n", rsvmem->res.base, rsvmem->res.size);
		base = rsvmem->res.base;
		offs = node;
		of_bus_default_count_cells(fdt, node, &na, &ns);
		for (offs = fdt_first_subnode(fdt, node); offs > 0; offs = fdt_next_subnode(fdt, offs)) {
			p = fdt_get_name(fdt, offs, &len);
			debugf("node name: %s\n", p);
			if (p && strstr(p, rsvmem->res.name)) {
				p = fdt_getprop(fdt, offs, "reg", &len);
				if (p) {
					r = 0;
					na_t = na;
					while (na_t--)
						r = (r << 32) | fdt32_to_cpu(*((fdt32_t *)p++));
					if (r) {
						debugf("%s info already exist\n", rsvmem->res.name);
						break;
					}

					sprintf(name, "%s@%x", rsvmem->res.name, base);
					err = fdt_set_name(fdt, offs, name);
					if (err < 0) {
						errorf("%s: set node name error(%s)\n", __func__, fdt_strerror(err));
						break;
					}

					j = 0;
					if (na == 2)
						cells[j++] = cpu_to_fdt32(base >> 32);
					cells[j++] = cpu_to_fdt32(base);
					if (ns == 2)
						cells[j++] = 0;
					cells[j++] = cpu_to_fdt32(rsvmem->res.size);
					err = fdt_setprop(fdt, offs, "reg", cells, sizeof(cells[0]) * j);
					if (err < 0) {
						errorf("%s: set reg prop error(%s)\n", __func__, fdt_strerror(err));
					}

					debugf("%s: update %s reg info done\n", __func__, name);
					break;
				}
			}
		}
	}

	debugf("add cp rsvmem to fdt done\n");
	print_fdt_node(fdt, node);

	return 0;
}

static int add_fdt_total_size(uint size, struct fdt_header *fdt)
{
	if(size <= 0 || NULL == fdt)	{
		errorf("%s: param error\n", __func__);
		return -1;
	}

	if(fdt_totalsize(fdt) <= fdt_off_dt_strings(fdt) + fdt_size_dt_strings(fdt) + size) {
		fdt_set_totalsize(fdt, fdt_totalsize(fdt) + size);
	}

	return 0;
}

static int fixup_image_partition_name(const char *rname, struct partition_name_info *pinfo) {
	int num, index = 0;
	char *p = pinfo->partition;

	if(NULL == rname || NULL == pinfo) {
		errorf("%s: param error\n", __func__);
		return -1;
	}

	strcpy(pinfo->partition, PARTITION_PREFIX);
	num = min(sizeof(pinfo->partition) - strlen(pinfo->partition) - 1, strlen(rname));
	strncat(pinfo->partition, rname, num);
	index = strlen(pinfo->partition) - strlen(NV_PARTITION_KEY);
	if(index >= 0) {
		p = pinfo->partition + index;
	}

	if(!strcmp(p, NV_PARTITION_KEY)) {
		if(!strcmp(pinfo->partition + strlen(PARTITION_PREFIX), "runnv"))
			strcpy(pinfo->partition + strlen(PARTITION_PREFIX), "runtimenv");

		strcpy(pinfo->partition_bk, pinfo->partition);
		num = min(sizeof(pinfo->partition) - strlen(pinfo->partition) - 1, strlen("1"));
		strncat(pinfo->partition, "1", num);
		num = min(sizeof(pinfo->partition_bk) - strlen(pinfo->partition_bk) - 1, strlen("2"));
		strncat(pinfo->partition_bk, "2", num);
	}

	return 0;
}

int read_cp_required_image(boot_image_required_t img_info)
{
	uint32_t secure_boot_offset = 0;
	uint32_t normal_boot_offset = 0;

	debugf("load %s to addr 0x%08x\n", img_info.partition, img_info.mem_addr);

	if (NULL != img_info.bak_partition) {
		debugf("load %s with backup img %s\n", img_info.partition, img_info.bak_partition);
		_boot_read_partition_with_backup(img_info);
	} else {
#if defined (CONFIG_SECURE_BOOT) || defined (CONFIG_SECBOOT)
		secure_boot_offset = SYS_HEADER_SIZE;
		if (0 != common_raw_read(img_info.partition, img_info.size, secure_boot_offset, (char *)img_info.mem_addr))
			errorf("read %s partition fail\n", img_info.partition);
#else
		normal_boot_offset = get_boot_offset(img_info, 0);
		if (0 != common_raw_read(img_info.partition, img_info.size, normal_boot_offset, (char *)img_info.mem_addr))
			errorf("read %s partition fail\n", img_info.partition);
#endif
	}

	return 1;
}

int get_coupling_info(const char *partition, struct cp_mem_coupling_info *info)
{
	boot_image_required_t img_info;

	if(NULL == partition || NULL == info){
		errorf("%s: param error\n", __func__);
		return -1;
	}

	memset(&img_info, 0, sizeof(boot_image_required_t));
	img_info.partition = partition;
	img_info.mem_addr = (unsigned long)info;
	img_info.size = sizeof(struct cp_mem_coupling_info);
	read_cp_required_image(img_info);

	return 0;
}

static struct decoup_conf g_decopu_conf[] = {
#ifndef CONFIG_DECOUPLING_NO_SURPPORT_RSVMEM
	{{"rsvmem", NULL}, do_parse_add_rsvmem_info},
#endif
	{{"sprd,decoup", "sipc-use-decoup"}, do_parse_add_sipc_info},
	{{"sprd,decoup", "cproc-use-decoup"}, do_parse_add_cproc_info},
	NULL
};

int fdt_fixup_cp_coupling_info(void *blob)
{
	int i, ret;
	unsigned long x = 0;
	char cmd_buf[64] = {0};
	struct fdt_header *fdt = NULL;
	struct partition_name_info mp;

	if(NULL == blob)	{
		errorf("%s: param error\n", __func__);
		return -1;
	}

	fdt = (struct fdt_header *)blob;

	fixup_image_partition_name(DECOUPLING_INFO_PARTITION, &mp);

	if(g_info.magic != HEADER_INFO_MAGIC && get_coupling_info(mp.partition, &g_info) < 0) {
		errorf(" read cp mem coupling info failed from %s\n", mp.partition);
		return -1;
	}

	debugf("%s: magic = 0x%x, version = 0x%x\n", __func__, g_info.magic, g_info.version);
	if(HEADER_INFO_MAGIC != g_info.magic) {
		debugf("this version not support cp mem decoupling\n");
		return 1;
	}

	add_fdt_total_size(EXPAND_FDT_SIZE, fdt);

	for(i = 0; i < sizeof(g_decopu_conf)/sizeof(struct decoup_conf); i++) {
		if(g_decopu_conf[i].do_parse_add) {
			ret = (g_decopu_conf[i].do_parse_add)(&g_decopu_conf[i].key, &g_info, fdt);
			if(ret < 0) {
				debugf("do parse add %s failed.\n", g_decopu_conf[i].key.name);
				return -1;
			}
		}
	}

	return 0;
}

boot_image_required_t * get_cp_load_table(void)
{
	int i, j;
	struct region_info *region = NULL;
	struct partition_name_info mp;

	fixup_image_partition_name(DECOUPLING_INFO_PARTITION, &mp);

	if(g_info.magic != HEADER_INFO_MAGIC && get_coupling_info(mp.partition, &g_info) < 0) {
		errorf("%s: read cp mem coupling info failed from %s\n", __func__, mp.partition);
		return NULL;
	}

	debugf("%s: magic: 0x%x, version: 0x%x\n", __func__, g_info.magic, g_info.version);
	if(HEADER_INFO_MAGIC != g_info.magic) {
		errorf("%s: this version not support cp mem decoupling\n", __func__);
		return NULL;
	}

	for(i = 0, j = 0; i < MAX_REGION_NUM; i++) {
		region = &g_info.boot_info.regions[i];
		if(strlen(region->res.name) > 0) {
			if(region->flag == re_load) {
				fixup_image_partition_name(region->res.name, &g_partition_info[j]);
				if(strlen(g_partition_info[j].partition) > 0) {
					s_boot_image_cp_table[j].partition = g_partition_info[j].partition;
				}
				if(strlen(g_partition_info[j].partition_bk) > 0) {
					s_boot_image_cp_table[j].bak_partition = g_partition_info[j].partition_bk;
				}

				s_boot_image_cp_table[j].size = region->res.size;
				s_boot_image_cp_table[j].mem_addr = region->res.base;

				if(s_boot_image_cp_table[j].bak_partition)
					debugf("partition: %s, bak_partition: %s, size: 0x%llx, mem_addr: 0x%x\n",
						s_boot_image_cp_table[j].partition, s_boot_image_cp_table[j].bak_partition,
						s_boot_image_cp_table[j].size, s_boot_image_cp_table[j].mem_addr);
				else
					debugf("partition: %s, bak_partition: NULL, size: 0x%llx, mem_addr: 0x%x\n",
						s_boot_image_cp_table[j].partition,
						s_boot_image_cp_table[j].size, s_boot_image_cp_table[j].mem_addr);
				j++;

			}
		}
	}

	return s_boot_image_cp_table;
}

int get_cp_loader(u32 *count, u32 *loader)
{
	struct partition_name_info mp;

	if (NULL == count || NULL == loader)
		return -1;

	fixup_image_partition_name(DECOUPLING_INFO_PARTITION, &mp);

	if(g_info.magic != HEADER_INFO_MAGIC && get_coupling_info(mp.partition, &g_info) < 0) {
		errorf("%s: read cp mem coupling info failed from %s\n", __func__, mp.partition);
		return -1;
	}

	if(g_info.magic != HEADER_INFO_MAGIC)
		return -1;

	*count = g_info.boot_info.bcode.count;
	memcpy(loader, g_info.boot_info.bcode.code, *count * sizeof(uint32_t));

	return 0;
}

extern char * strstr(const char * s1,const char * s2);
int get_ring_info(u64 *addr, u64 *size)
{
	int i;
	struct partition_name_info mp;

	if (NULL == addr || NULL == size)
		return -1;

	fixup_image_partition_name(DECOUPLING_INFO_PARTITION, &mp);

	if(g_info.magic != HEADER_INFO_MAGIC && get_coupling_info(mp.partition, &g_info) < 0) {
		errorf("%s: read cp mem coupling info failed from %s\n", __func__, mp.partition);
		return -1;
	}

	if(g_info.magic != HEADER_INFO_MAGIC)
		return -1;

	for (i=0; i<MAX_RES_NUM; i++) {
		if (strstr("ring", g_info.comm_info.res[i].name)) {
			*addr = g_info.comm_info.res[i].base;
			*size = g_info.comm_info.res[i].size;
			return 0;
		}
	}

	return -1;
}

