#include <common.h>
#include <stdio_dev.h>
#include <libfdt.h>
#include <fdtdec.h>

static const char MEM_REGION_STR[] = {"memory-region"};
static const char MEM_REGION_AUTO_STR[] = {"sprd,memory-region-auto"};
#ifdef CONFIG_ARM64
typedef uint64_t mem_addr_t;
typedef uint64_t mem_size_t;
#define fdt_to_cpu(reg) fdt64_to_cpu(reg)
#define cpu_to_fdt(reg) cpu_to_fdt64(reg)
#else
typedef uint32_t mem_addr_t;
typedef uint32_t mem_size_t;
#define fdt_to_cpu(reg) fdt32_to_cpu(reg)
#define cpu_to_fdt(reg) cpu_to_fdt32(reg)
#endif

uint32_t g_rsvmem_p;

static int fdt_get_addr_size(const void *fdt, int node, const char *propname,
			mem_addr_t *addrp, mem_size_t *sizep)
{
	const mem_addr_t *addr;
	const mem_size_t *size;

	addr = fdt_getprop(fdt, node, propname, NULL);
	if (addr) {
		if (addrp)
			*addrp = fdt_to_cpu(*addr);
		if (sizep) {
			size = (mem_size_t *)((char *)addr + sizeof(mem_addr_t));
			*sizep = fdt_to_cpu(*size);
			debugf("size: 0x%lx\n", *sizep);
		}
		return 0;
	}
	return -1;
}

static int fdt_set_addr_size(const void *fdt, int node, const char *propname,
			mem_addr_t *addrp, mem_size_t *sizep)
{
	int err;
	mem_addr_t addr;
	mem_size_t size;
	if (addrp) {
		addr = cpu_to_fdt(*addrp);
		err =  fdt_setprop(fdt, node, propname, &addr, sizeof(addr));
		if (err < 0) {
			errorf("cannot set node's reg property(addr)!\n");
			return err;
		}
	}
	if (sizep) {
		size = cpu_to_fdt(*sizep);
		err = fdt_appendprop(fdt, node, propname, &size, sizeof(size));
		if (err < 0)
			errorf("cannot set node's reg property(size)!\n");
	}
	return err;
}

static int fdt_node_offset_by_name(const void *fdt,  const char *nodename)
{
	int offset;
	char *name;
	for (offset = fdt_next_node(fdt, -1, NULL); offset >= 0;
		offset = fdt_next_node(fdt, offset, NULL)) {
		name = fdt_get_name(fdt, offset, NULL);
		if(0 == strcmp(nodename, name))
			return offset;
	}
	return offset;
}

int fdt_fixup_memory_addr(void *fdt, int offset,
				mem_addr_t *addr, mem_size_t *size)
{
	const uint32_t *phandle;
	mem_addr_t prev_addr;
	int nodeoffset;

	phandle = fdt_getprop(fdt, offset, MEM_REGION_STR, NULL);
	if (!phandle) {
		errorf("device tree doesn't have %s prop.\n", MEM_REGION_STR);
		return -1;
	} else {
		nodeoffset = fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(*phandle));
		if (nodeoffset < 0) {
			errorf("get nodeoffset error\n");
			return nodeoffset;
		}

		if(fdt_get_addr_size(fdt, nodeoffset, "reg", &prev_addr, size) < 0) {
			errorf("cannot find reg addr and size.\n" );
			return -1;
		}

		if(fdt_set_addr_size(fdt, nodeoffset, "reg", addr, size) < 0) {
			errorf("set reg addr and size fail.\n" );
			return -1;
		}
	}
	return 0;
}

int fdt_fixup_memory_region(void *fdt)
{
	int offset;
	mem_addr_t addr;
	mem_size_t size;
	char nodename[32];
	int str_len;

	memset(nodename, 0, 32);
	for (offset = fdt_next_node(fdt, -1, NULL);
	     offset >= 0;
	     offset = fdt_next_node(fdt, offset, NULL)) {
		if (fdt_get_property(fdt, offset, MEM_REGION_AUTO_STR, NULL)) {
			addr = g_rsvmem_p;
			sprintf(nodename, "%s", fdt_get_name(fdt, offset, NULL));
			str_len = strlen(nodename);
			nodename[str_len] = '\0';
			debugf("prop name: %s \n", nodename);
			if (fdt_fixup_memory_addr(fdt, offset, &addr, &size) < 0 ) {
				return -1;
			} else {
				g_rsvmem_p += size;
				offset = fdt_node_offset_by_name(fdt, nodename);
				if(offset < 0)
				 	return -1;
			}
		}
	}
	return 0;
}

