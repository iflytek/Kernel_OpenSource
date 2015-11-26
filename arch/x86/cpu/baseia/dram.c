/*
 * Copyright (C) 2015 Intel, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 *
 */

#include <common.h>
#include <asm/e820.h>
#include <asm/post.h>
#include <fdtdec.h>

#define E820_MAP_NAME "intel,e820_map"
#define E820_ITEMS "e820_map"

/* element array: addr(64), size(64), type(32)*/
#define E820_ITEM_SIZE (8 + 8 + 4)

static unsigned install_e820_map_fdt(unsigned max_entries,
			struct e820entry *entries)
{
	int node, count, i;
	const void *array;

	node = fdt_node_offset_by_compatible(gd->fdt_blob, -1, E820_MAP_NAME);
	if (node < 0) {
		debug("%s, find %s fail: %d!\n", __func__, E820_MAP_NAME, node);
		return 0;
	}

	/* parse map list */
	array = fdt_getprop(gd->fdt_blob, node, E820_ITEMS, &count);
	count /= E820_ITEM_SIZE;
	count = min(count, (int)max_entries);

	for (i = 0; i < count; i++) {
		entries->addr = fdt64_to_cpu(*((uint64_t *)array));
		array += sizeof(uint64_t);
		entries->size = fdt64_to_cpu(*((uint64_t *)array));
		array += sizeof(uint64_t);
		entries->type = fdt32_to_cpu(*((uint32_t *)array));
		array += sizeof(uint32_t);

		entries++;
	}

	printf("%s, found %d e820 entries\n", __func__, count);
	return count;
}

unsigned install_e820_map(unsigned max_entries, struct e820entry *entries)
{
	u8 e820_entries = 0;

	e820_entries = install_e820_map_fdt(max_entries, entries);
	if (e820_entries)
		return e820_entries;

	entries->addr = 0x0;
	entries->size = PHYS_SDRAM_1_SIZE;
	entries->type = E820_RAM;
	e820_entries++;
	entries++;

	entries->addr = 0xfe000000;
	entries->size = 0x1000000;
	entries->type = E820_RESERVED;
	e820_entries++;
	entries++;

	return e820_entries > max_entries ? max_entries : e820_entries;
}

int dram_init(void)
{
	gd->ram_size = PHYS_SDRAM_1_SIZE;
	post_code(POST_DRAM);

	return 0;
}

void dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = 0;
	gd->bd->bi_dram[0].size = gd->ram_size;
}

ulong board_get_usable_ram_top(ulong total_size)
{
	return gd->ram_size;
}
