#include <common.h>
#include <asm/e820.h>
#include <asm/post.h>

unsigned install_e820_map(unsigned max_entries, struct e820entry *entries)
{
	u8  e820_entries = 0;

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
