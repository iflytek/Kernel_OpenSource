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
#include <chipram_env.h>

#include <asm/arch/sprd_reg.h>

#define E820_MAP_NAME "intel,e820_map"
#define E820_ITEMS "e820_map"

/* element array: addr(64), size(64), type(32)*/
#define E820_ITEM_SIZE (8 + 8 + 4)

#define U32_TO_U64(val1,val2)	(u64)((((u64)(val1))<<32)|((u64)(val2)))

ddr_info_t local_ddr_range;

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

	printf("%s, !!!!!ERROR!!!!! found %d e820 entries in DTS file! \n", __func__, e820_entries);
	return e820_entries > max_entries ? max_entries : e820_entries;
}

void setup_chipram_ddr_range(void)
{
	ddr_info_t * env = CHIPRAM_DDR_RANGE_LOCATION;

	local_ddr_range = * env;

}

ddr_info_t* get_ddr_range(void)
{
	return &local_ddr_range;
}

unsigned install_e820_map_auto_detect(unsigned max_entries, struct e820entry *entries)
{
	extern ddr_info_t* get_ddr_range(void);
	u8 e820_entries_detect = 0;
	u8 e820_entries_dts = 0;
	int i=0;
	ddr_info_t* ddr_info = get_ddr_range();
	e820_entries_detect=ddr_info->section_number;
	/*the first part of ddr,may be reserved for vmm in vmm board*/
	e820_entries_dts = install_e820_map_fdt(max_entries, entries);
	entries++;
	entries->addr = U32_TO_U64(ddr_info->sec_info[0].start_address_high,ddr_info->sec_info[0].start_address_low)+VMM_RESERVE_SIZE;
	entries->size  = U32_TO_U64(ddr_info->sec_info[0].end_address_high,ddr_info->sec_info[0].end_address_low)-entries->addr+0x1;
	entries->type  = E820_RAM;
	entries++;

	for (i = 1; i < e820_entries_detect; i++) {
		entries->addr = U32_TO_U64(ddr_info->sec_info[i].start_address_high,ddr_info->sec_info[i].start_address_low);
		entries->size  = U32_TO_U64(ddr_info->sec_info[i].end_address_high,ddr_info->sec_info[i].end_address_low)-entries->addr+0x1;
		entries->type  = E820_RAM;
		entries++;
	}
	return e820_entries_dts+e820_entries_detect;
}

static void uncore_register_set_bits(u8 portID, u8 offset, u32 mask, u32 val)
{
	u32 orig, tmp;
	u32 index = (portID << 16) |(offset << 8) | 0xF0;

	outl(0x800000D0, 0xCF8);
	outl(0x06000000 | index, 0xCFC);
	outl(0x800000D4, 0xCF8);
	orig = inl(0xCFC);

	tmp = orig & ~mask;
	tmp |= val & mask;

	outl(0x800000D4,0xCF8);
	outl(tmp,0xCFC);
	outl(0x800000D0,0xCF8);
	outl(0x07000000 | index,0xCFC);
}

static void dram_configure_bridge_perf(void)
{
#define BIT(x) (0x1 << x)

	/*
	 * Sync FIFO to have same CLK
	 * 0 - Not same
	 * 1 - same
	 * Bit[4], Bit[2]
	 */
	uncore_register_set_bits(0x1B, 0x0, BIT(4) | BIT(2), BIT(4) | BIT(2));

	/*
	 * Request MAX_NUM of pending reads
	 * Bit[7:0]
	 * Update cache paramaters
	 * Bit[19:16]
	 */
	uncore_register_set_bits(0x1B, 0x1, BIT(16), BIT(16));

	/*
	 * Disable Non-posted writes
	 * Bit[0]
	 * Clear last bit
	 */
	uncore_register_set_bits(0x14, 0x10, BIT(0), 0);

	/* ackgate bit 0-22 */
	uncore_register_set_bits(0x00, 0x18, 0x007FFFFF, 0x007FFFFF);

	/* aisochctl */
	uncore_register_set_bits(0x00, 0x20, 0x1FF, 0x1);

	/* avcctl bit 31 8 */
	uncore_register_set_bits(0x00, 0x21, BIT(31) | BIT(8), BIT(31) | BIT(8));

	/* aarbctl0 */
	uncore_register_set_bits(0x00, 0xC0, 0xF, 0x4);

	/* bwflush */
	uncore_register_set_bits(0x03, 0x07, 0x0000FFFF, 0x0);

	/* bbankmask 25 */
	uncore_register_set_bits(0x03, 0x08, 0x01FFFFFF, 0x0);

	/* browmask 25 */
	uncore_register_set_bits(0x03, 0x09, 0x01FFFFFF, 0x003FFFFC);

	/* brankmask 10 */
	uncore_register_set_bits(0x03, 0x0A, 0x000002FF, 0x80);

	/* balimit0 */
	uncore_register_set_bits(0x03, 0x0B, 0x3F3F3F3F, 0x1F1F1F1F);
	/* balimit1 */
	uncore_register_set_bits(0x03, 0x0C, 0x3F3F3F3F, 0x1F1F1F1F);
	/* balimit2 */
	uncore_register_set_bits(0x03, 0x0D, 0x3F3F3F3F, 0x1F1F1F1F);
	/* balimit3 */
	uncore_register_set_bits(0x03, 0x0E, 0x3F3F3F3F, 0x1F1F1F1F);

	/* bmisc ??? */
	uncore_register_set_bits(0x03, 0x28, 0x7<<6, 0x7<<6);

	/* bctrl 0,1,2,3 11,12 */
	uncore_register_set_bits(0x03, 0x3D, 0xF|BIT(11)|BIT(12), 0xF|BIT(11)|BIT(12));

	/* cunit_ssa_regional_trunkgate_ctl */
	uncore_register_set_bits(0x08, 0x43, 0xFF|BIT(16)|BIT(18), 0x08|BIT(16)|BIT(18));

	/* t_ctl */
	uncore_register_set_bits(0x02, 0x03, BIT(10)|BIT(16), BIT(10));

	/* t_misc_ctl */
	uncore_register_set_bits(0x02, 0x04, 0xF<<1, 0x6<<1);

	/* t_clkgate_ctl 0-28 */
	uncore_register_set_bits(0x02, 0x05, 0x1FFFFFFF, 0x1FFFFFFF);

#ifdef CONFIG_SOC_IWHALE2
	/* memaxi AW cobuf enable */
	(*((volatile unsigned int *)(REG_PUB_APB_PUB0_AXI_AW_COBUF_EN))) |= 0x2;
#endif
}

int dram_init(void)
{
	gd->ram_size = PHYS_SDRAM_1_SIZE;
	post_code(POST_DRAM);

	/* Configure mem bridges for better performance
	 * Since these are memory related bridges it makes sense
	 * to enabled them once the DRAM is up
	 * TODO: need to further investigate if this is
	 * 	 the best place to enable
	 * TODO: Do we need to reenable after suspend?
	 */
	dram_configure_bridge_perf();

	return 0;
}

void dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = 0; 
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;

#ifdef CONFIG_SP9861_DDR4G
    #ifdef CONFIG_SP9861_DDR4G_SINGLE	/*single channel ddr 4g memory map*/
    	gd->bd->bi_dram[1].start = 0x40000000;
    	gd->bd->bi_dram[1].size  = PHYS_SDRAM_1_SIZE;

    	gd->bd->bi_dram[2].start = 0x80000000;
    	gd->bd->bi_dram[2].size  = PHYS_SDRAM_1_SIZE;

    	gd->bd->bi_dram[3].start = 0x100000000;
    	gd->bd->bi_dram[3].size  = PHYS_SDRAM_1_SIZE;
    #elif CONFIG_SP9861_DDR4G_INTERLEAVE	/*interleave memory map*/
    	gd->bd->bi_dram[1].start = 0x140000000; /*non interleave area for memdisk*/
    	gd->bd->bi_dram[1].size  = PHYS_SDRAM_1_SIZE;

    	gd->bd->bi_dram[2].start = 0x2C0000000;
    	gd->bd->bi_dram[2].size  = PHYS_SDRAM_1_SIZE;

    	gd->bd->bi_dram[3].start = 0x300000000;
    	gd->bd->bi_dram[3].size  = PHYS_SDRAM_1_SIZE;
    #else	/*noninterleave memory map*/
        gd->bd->bi_dram[1].start = 0x40000000;
        gd->bd->bi_dram[1].size  = PHYS_SDRAM_1_SIZE;

        gd->bd->bi_dram[2].start = 0x140000000;
        gd->bd->bi_dram[2].size  = PHYS_SDRAM_1_SIZE;

        gd->bd->bi_dram[3].start = 0x180000000;
        gd->bd->bi_dram[3].size  = PHYS_SDRAM_1_SIZE;
    #endif
#endif

#ifdef CONFIG_SP9853I_DDR4G
    gd->bd->bi_dram[1].start = 0x40000000;
    gd->bd->bi_dram[1].size  = PHYS_SDRAM_1_SIZE;

    gd->bd->bi_dram[2].start = 0x80000000;
    gd->bd->bi_dram[2].size  = PHYS_SDRAM_1_SIZE;

    gd->bd->bi_dram[3].start = 0x100000000;
    gd->bd->bi_dram[3].size  = PHYS_SDRAM_1_SIZE;
#endif
}

ulong board_get_usable_ram_top(ulong total_size)
{
	return gd->ram_size;
}
