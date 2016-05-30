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

    entries->addr = 0xfe000000;
	entries->size = 0x1000000;
	entries->type = E820_RESERVED;
	e820_entries++;
	entries++;

#ifdef CONFIG_IWHALE2_DDR4G
#ifdef CONFIG_IWHALE2_DDR4G_INTERLEAVE
/*interleave solution for soc 0.9.17*/
	/*reserved mem for kernel*/
        entries->addr = 0x0;
        entries->size = 0x6000000;
        entries->type = E820_RAM;
        e820_entries++;
        entries++;

	/*bottom 1GB of pub0 for non-interleave area*/
	/*for memdisk,cache.img ,userdata.img*/
    /*    entries->addr = 0x28000000;
        entries->size = 0x18000000;
        entries->type = E820_RAM;
        e820_entries++;
        entries++;*/
	/*bottom 1GB of pub1 for non-interleave area*/
        /*for memdisk,system.img */
	    /*entries->addr = 0x140000000;
        entries->size = 0x3d000000;
        entries->type = E820_RAM;
        e820_entries++;*/

	/*reserve the top 1GB non-interleave area on each pub, 0x4000 0000-0x7fff ffff and 0x1 8000 0000-0x1 c000 0000, make sure no one use it*/
        entries->addr = 0x40000000;
        entries->size = 0x40000000;
        entries->type = E820_RESERVED;
        e820_entries++;
        entries++;

        entries->addr = 0x180000000;
        entries->size = 0x40000000;
        entries->type = E820_RESERVED;
        e820_entries++;
        entries++;

	/*top 2GB of interleave area memory,0x2 C000 0000-0x2 ffff ffff and 0x3 0000 0000-0x3 3fff ffff, for soc 0.9.17 interleave pattern.*/
        entries->addr = 0x2C0000000;
        entries->size = 0x40000000;
        entries->type = E820_RAM;
        e820_entries++;
        entries++;

        entries->addr = 0x300000000;
        entries->size = 0x40000000;
        entries->type = E820_RAM;
        e820_entries++;
        entries++;

#else /*CONFIG_IWHALE2_DDR4G_INTERLEAVE*/
    /*add e820 entry for memory 0x0~0x3fffffff ,pub0*/
	entries->addr = 0x0;
	entries->size = PHYS_SDRAM_1_SIZE;
	entries->type = E820_RAM;
	e820_entries++;
	entries++;

    /*add e820 entry for memory  0x4000 0000~0x7fff ffff, pub0*/
    entries->addr = 0x40000000;
    entries->size = 0x40000000;
    entries->type = E820_RAM;
    e820_entries++;
    entries++;

    /*add e820 entry for memory  0x1 4000 0000~0x1 7fff ffff, pub1*/
    entries->addr = 0x140000000;
    entries->size = 0x40000000;
    entries->type = E820_RAM;
    e820_entries++;
    entries++;

    /*add e820 entry for memory  0x1 8000 0000~0x1 bfff ffff, pub1*/
    entries->addr = 0x180000000;
    entries->size = 0x40000000;
    entries->type = E820_RAM;
    e820_entries++;
    entries++;
#endif /*CONFIG_IWHALE2_DDR4G_INTERLEAVE*/

#else /*CONFIG_IWHALE2_DDR4G*/

    /*add e820 entry for memory 0x0~0x7fffffff ,pub0*/
	entries->addr = 0x0;
	entries->size = PHYS_SDRAM_1_SIZE;
	entries->type = E820_RAM;
	e820_entries++;
#endif /*CONFIG_IWHALE2_DDR4G*/
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
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;

#ifdef CONFIG_IWHALE2_DDR4G
    #ifdef CONFIG_IWHALE2_DDR4G_INTERLEAVE 
    gd->bd->bi_dram[1].start = 0x140000000; /*non interleave area for memdisk*/
    gd->bd->bi_dram[1].size  = PHYS_SDRAM_1_SIZE;

    gd->bd->bi_dram[2].start = 0x2C0000000;
    gd->bd->bi_dram[2].size  = PHYS_SDRAM_1_SIZE;

    gd->bd->bi_dram[3].start = 0x300000000;
    gd->bd->bi_dram[3].size  = PHYS_SDRAM_1_SIZE;
    #else
    gd->bd->bi_dram[1].start = 0x40000000;
    gd->bd->bi_dram[1].size  = PHYS_SDRAM_1_SIZE;

    gd->bd->bi_dram[2].start = 0x140000000;
    gd->bd->bi_dram[2].size  = PHYS_SDRAM_1_SIZE;

    gd->bd->bi_dram[3].start = 0x180000000;
    gd->bd->bi_dram[3].size  = PHYS_SDRAM_1_SIZE;
    #endif
#endif

}

ulong board_get_usable_ram_top(ulong total_size)
{
	return gd->ram_size;
}
