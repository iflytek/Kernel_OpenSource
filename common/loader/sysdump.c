/*
 * Copyright (C) 2013 Spreadtrum Communications Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

//#include "normal_mode.h"
#include <rtc.h>
//#include <asm/sizes.h>
#include <boot_mode.h>
#include <common.h>

#include <linux/input.h>
#include "sysdump.h"
#include <linux/mtd/mtd.h>
#include <linux/sizes.h>
#include <asm/arch/check_reboot.h>
#include <mmc.h>
#include <fat.h>
#include <exfat.h>
#include "loader_common.h"

#include <libfdt.h>
#include <fdt.h>
#include <linux/ctype.h>
#include <fdtdec.h>

#ifdef CONFIG_X86
#include <asm/e820.h>
#include <chipram_env.h>
#endif

#ifdef CONFIG_X86
#define LLX			"0x%08llx"
#define SYSDUMP_LONG		unsigned long long
#define U32_T_U64(val1,val2)	(u64)((((u64)(val1))<<32)|((u64)(val2)))
#define RESERVED_MEMORY_COUNT	25
struct mem_reserved{
        unsigned long long addr;
        unsigned long long size;
};
static struct mem_reserved mem_res_arry[RESERVED_MEMORY_COUNT];
#else
#define LLX			"0x%08lx"
#define SYSDUMP_LONG		unsigned long
#endif

enum {
	FS_FAT32,
	FS_EXFAT,
	FS_INVALID
};

extern  int sprd_host_init(int sdio_type);
extern struct rtc_time get_time_by_sec(void);

#ifdef CONFIG_ETB_DUMP
extern unsigned char etb_dump_mem[SZ_32K];
extern void sprd_etb_hw_dis(void);
extern void sprd_etb_dump (void);
#endif

#ifdef CONFIG_SPLASH_SCREEN
	extern void set_backlight(uint32_t value);
	extern void lcd_printf(const char *fmt, ...);
	extern void lcd_enable(void);
	#define sysdump_lcd_printf(fmt, args...)   lcd_printf(fmt, ##args)
#else
	#define sysdump_lcd_printf(fmt, args...)
	#define lcd_enable(void)
#endif
extern uint64_t _get_kernel_ramdisk_dt_offset( boot_img_hdr * hdr, uchar *partition);
extern int load_fixup_dt_img(uchar *partition, uchar **dt_start_addr);

void display_writing_sysdump(void)
{
	debugf("%s\n", __FUNCTION__);
#ifdef CONFIG_SPLASH_SCREEN
	vibrator_hw_init();
	set_vibrator(1);
	sysdump_lcd_printf("   -------------------------------  \n"
		   "   Sysdumpping now, keep power on.  \n"
		   "   -------------------------------  \n");
	lcd_enable();
	set_backlight(255);
	set_vibrator(0);
#endif
}
void display_special_mode(void)
{
	debugf("%s\n", __FUNCTION__);
#ifdef CONFIG_SPLASH_SCREEN
	vibrator_hw_init();
	set_vibrator(1);
	sysdump_lcd_printf("   -------------------------------  \n"
		   "   Restart now, keep power on.  \n"
		   "   -------------------------------  \n");
	lcd_enable();
	set_backlight(255);
	set_vibrator(0);
#endif
}
/*display without character*/
void display_sysdump()
{
	debugf("%s\n", __FUNCTION__);
#ifdef CONFIG_SPLASH_SCREEN
	vibrator_hw_init();
	set_vibrator(1);
	set_backlight(255);
	set_vibrator(0);
#endif
}

static void wait_for_keypress()
{
	int key_code;

	do {
		udelay(50 * 1000);
		key_code = board_key_scan();
		//printf("key_code: %d, (vd:%d,vu:%d,p:%d)\n", key_code, KEY_VOLUMEDOWN, KEY_VOLUMEUP, KEY_POWER);
		if (key_code == KEY_VOLUMEDOWN || key_code == KEY_VOLUMEUP || key_code == KEY_HOME)
			break;
	} while (1);
	debugf("Pressed key: %d\n", key_code);
	sysdump_lcd_printf("Pressed key: %d\n", key_code);
	lcd_enable();
}



int init_mmc_fat(int *fs_type)
{
	struct mmc *mmc;
	block_dev_desc_t *dev_desc = NULL;
	int ret;
	*fs_type = FS_INVALID;
	debugf("init_mmc_fat\n");
	sprd_host_init(1);
	mmc = find_mmc_device(1);// 1 is  sd, 0 is emmc
	if(mmc) {
		ret = mmc_init(mmc);
		if(ret < 0){
			debugf("mmc init failed %d\n", ret);
			return -1;
		}
	} else {
		debugf("no mmc card found\n");
		return -1;
	}

	dev_desc = &mmc->block_dev;
	if(dev_desc==NULL){
		debugf("no mmc block device found\n");
		return -1;
	}

	if (dev_desc ->part_type != PART_TYPE_DOS){
		debugf(" no part_type_dos!\n");
		return -1;
	}
	ret = fat_register_device(dev_desc, 1);
	if (ret == 0) {
		ret = file_fat_detectfs();
		if(ret) {
			debugf("detect fs fat failed\n");
			return -1;
		}
		*fs_type = FS_FAT32;
		return 0;
	}
	debugf("Fat register fail. Try exFat. \n");

	ret = exfat_register_device(dev_desc, 1);
	if(ret < 0) {
		debugf("exFat register fail %d.\n", ret);
		return -1;
	}

	ret = file_exfat_detectfs();
	if(ret) {
		debugf("Detect fs exfat failed\n");
		return -1;
	}
	*fs_type = FS_EXFAT;
	return 0;
}

void write_mem_to_mmc(char *path, char *filename, int fs_type,
	void *memaddr, unsigned long memsize)
{
	int ret;

	if (path) {
		do {} while (0); /* TODO: jianjun.he */
	}

	debugf("writing 0x%lx bytes to sd file %s\n",
		memsize, filename);
	sysdump_lcd_printf("writing 0x%lx bytes to sd file %s\n",
		memsize, filename);
	lcd_enable();

	unsigned long *actwrite = NULL;
	if (fs_type==FS_FAT32)
		ret = file_fat_write(filename, memaddr, 0, memsize, actwrite);
	else if (fs_type==FS_EXFAT)
		ret = file_exfat_write(filename, memaddr, memsize);
	else
		ret = -1;

	if (ret <= 0) {
		debugf("sd file write error %d\n", ret);
	}

	return;
}

static size_t get_elfhdr_size(int nphdr)
{
	size_t elfhdr_len;

	elfhdr_len = sizeof(struct elfhdr) +
		(nphdr + 1) * sizeof(struct elf_phdr);
#if SETUP_NOTE
	elfhdr_len += ((sizeof(struct elf_note)) +
		roundup(sizeof(CORE_STR), 4)) * 3 +
		roundup(sizeof(struct elf_prstatus), 4) +
		roundup(sizeof(struct elf_prpsinfo), 4) +
		roundup(sizeof(struct task_struct), 4);
#endif
	elfhdr_len = PAGE_ALIGN(elfhdr_len); //why?

	return elfhdr_len;
}

#if SETUP_NOTE
static int notesize(struct memelfnote *en)
{
	int sz;

	sz = sizeof(struct elf_note);
	sz += roundup((strlen(en->name) + 1), 4);
	sz += roundup(en->datasz, 4);

	return sz;
}

static char *storenote(struct memelfnote *men, char *bufp)
{
	struct elf_note en;

#define DUMP_WRITE(addr,nr) do { memcpy(bufp,addr,nr); bufp += nr; } while(0)

	en.n_namesz = strlen(men->name) + 1;
	en.n_descsz = men->datasz;
	en.n_type = men->type;

	DUMP_WRITE(&en, sizeof(en));
	DUMP_WRITE(men->name, en.n_namesz);

	/* XXX - cast from long long to long to avoid need for libgcc.a */
	bufp = (char*) roundup((unsigned long)bufp,4);
	DUMP_WRITE(men->data, men->datasz);
	bufp = (char*) roundup((unsigned long)bufp,4);

#undef DUMP_WRITE

	return bufp;
}

#endif

static void sysdump_fill_core_hdr(struct pt_regs *regs,
						struct sysdump_mem *sysmem, int mem_num,
						char *bufp, int nphdr, int dataoff)
{
	struct elf_phdr *nhdr, *phdr;
	struct elfhdr *elf;
	struct memelfnote notes[3];
	off_t offset = 0;
	Elf_Off f_offset = dataoff;
	int i;

	/* setup ELF header */
	elf = (struct elfhdr *) bufp;
	bufp += sizeof(struct elfhdr); //printk("sizeof(struct elfhdr): %d\n");
	offset += sizeof(struct elfhdr); //printk("sizeof(struct elfhdr): %d\n");
	memcpy(elf->e_ident, ELFMAG, SELFMAG); //printk("ELFMAG: %s, SELFMAG:%d\n", ELFMAG, SELFMAG);
	elf->e_ident[EI_CLASS]	= ELF_CLASS;//printk("EI_CLASS:%d, ELF_CLASS: %d", EI_CLASS, ELF_CLASS);
	elf->e_ident[EI_DATA]	= ELF_DATA;//printk("EI_DATA:%");
	elf->e_ident[EI_VERSION]= EV_CURRENT;
	elf->e_ident[EI_OSABI] = ELF_OSABI;
	memset(elf->e_ident+EI_PAD, 0, EI_NIDENT-EI_PAD);
	elf->e_type	= ET_CORE;
	elf->e_machine	= ELF_ARCH;
	elf->e_version	= EV_CURRENT;
	elf->e_entry	= 0;
	elf->e_phoff	= sizeof(struct elfhdr);
	elf->e_shoff	= 0;
	elf->e_flags	= ELF_CORE_EFLAGS;
	elf->e_ehsize	= sizeof(struct elfhdr);
	elf->e_phentsize= sizeof(struct elf_phdr);
	elf->e_phnum	= nphdr;
	elf->e_shentsize= 0;
	elf->e_shnum	= 0;
	elf->e_shstrndx	= 0;

	/* setup ELF PT_NOTE program header */
	nhdr = (struct elf_phdr *) bufp;
	bufp += sizeof(struct elf_phdr);
	offset += sizeof(struct elf_phdr);
	nhdr->p_type	= PT_NOTE;
	nhdr->p_offset	= 0;
	nhdr->p_vaddr	= 0;
	nhdr->p_paddr	= 0;
	nhdr->p_filesz	= 0;
	nhdr->p_memsz	= 0;
	nhdr->p_flags	= 0;
	nhdr->p_align	= 0;

	/* setup ELF PT_LOAD program header for every area */
	for (i = 0; i < mem_num; i++) {
		phdr = (struct elf_phdr *) bufp;
		bufp += sizeof(struct elf_phdr);
		offset += sizeof(struct elf_phdr);

		phdr->p_type	= PT_LOAD;
		phdr->p_flags	= PF_R|PF_W|PF_X;
		phdr->p_offset	= f_offset;
		phdr->p_vaddr	= sysmem[i].vaddr;
		phdr->p_paddr	= sysmem[i].paddr;
		phdr->p_filesz	= phdr->p_memsz	= sysmem[i].size;
		phdr->p_align	= 0;//PAGE_SIZE;
		f_offset += sysmem[i].size;
	}
#if SETUP_NOTE
	/*
	 * Set up the notes in similar form to SVR4 core dumps made
	 * with info from their /proc.
	 */
	nhdr->p_offset	= offset;

	/* set up the process status */
	notes[0].name = CORE_STR;
	notes[0].type = NT_PRSTATUS;
	notes[0].datasz = sizeof(struct elf_prstatus);
	notes[0].data = &prstatus;

	memset(&prstatus, 0, sizeof(struct elf_prstatus));

	nhdr->p_filesz	= notesize(&notes[0]);
	bufp = storenote(&notes[0], bufp);

	/* set up the process info */
	notes[1].name	= CORE_STR;
	notes[1].type	= NT_PRPSINFO;
	notes[1].datasz	= sizeof(struct elf_prpsinfo);
	notes[1].data	= &prpsinfo;

	memset(&prpsinfo, 0, sizeof(struct elf_prpsinfo));
	//fill_psinfo(&prpsinfo, current, current->mm);

	strcpy(prpsinfo.pr_fname, "vmlinux");
	//strncpy(prpsinfo.pr_psargs, saved_command_line, ELF_PRARGSZ);

	nhdr->p_filesz	+= notesize(&notes[1]);
	bufp = storenote(&notes[1], bufp);

	/* set up the task structure */
	notes[2].name	= CORE_STR;
	notes[2].type	= NT_TASKSTRUCT;
	notes[2].datasz	= sizeof(struct task_struct);
	notes[2].data	= current;

	printk("%s: data size is %d, data addr is %p",__func__,notes[2].datasz,notes[2].data);

	nhdr->p_filesz	+= notesize(&notes[2]);
	bufp = storenote(&notes[2], bufp);
#endif
	return;
} /* end elf_kcore_store_hdr() */

char *rstmode[] = {
	"undefind mode",			//CMD_UNDEFINED_MODE=0,
	"power down",				//CMD_POWER_DOWN_DEVICE,
	"normal",				//CMD_NORMAL_MODE,
	"recovery",				//CMD_RECOVERY_MODE,
	"fastboot",				//CMD_FASTBOOT_MODE,
	"alarm",				//CMD_ALARM_MODE,
	"charge",				//CMD_CHARGE_MODE,
	"engtest",				//CMD_ENGTEST_MODE,
	"watchdog timeout",			//CMD_WATCHDOG_REBOOT ,
	"ap watchdog timeout",			//CMD_AP_WATCHDOG_REBOOT ,
	"framework crash",			//CMD_SPECIAL_MODE,
	"manual dump",				//CMD_UNKNOW_REBOOT_MODE,
	"kernel crash",				//CMD_PANIC_REBOOT,
	"vmm panic",				//CMD_VMM_PANIC_MODE
	"tos panic",				//CMD_TOS_PANIC_MODE
	"ext rstn reboot",                      //CMD_EXT_RSTN_REBOOT_MODE,
	"calibration",				//CMD_CALIBRATION_MODE,
	"factory test",				//CMD_FACTORYTEST_MODE,
	"autodloader",				//CMD_AUTODLOADER_REBOOT,
	"autotest",				//CMD_AUTOTEST_MODE,
	"iq reboot",				//CMD_IQ_REBOOT_MODE,
	"sleep",				//CMD_SLEEP_MODE,
	"sprd disk",				//CMD_SPRDISK_MODE,
};

#define GET_RST_MODE(x) rstmode[(x) < 23 ? (x) : 0]

#ifdef CONFIG_NAND_BOOT
/*Copy the data saved in nand flash to ram*/
int read_nand_to_ram( struct mtd_info *mtd, loff_t paddr, unsigned int size, unsigned char *buf)
{
        int ret = 0;
        unsigned int retlen = 0;
        loff_t read_addr = 0;
        unsigned char *read_buf = NULL;
        unsigned int readsize = 0;

        debugf("%s, read 0x%.8x:0x%.8x buf: 0x%.8x\n", __func__, (unsigned int)paddr, size, buf);
        for(read_addr = paddr, read_buf = buf; read_addr < (paddr + size); read_addr += readsize, read_buf += readsize) {
                readsize = (paddr + size - read_addr) > mtd->erasesize ? mtd->erasesize : (paddr + size - read_addr);
                if(mtd->block_isbad(mtd, read_addr) == 1) {//if met bad block, we just fill it with 0x5a
                        memset(read_buf, 0x5a, readsize);
                        continue;
                }

                ret = mtd->read(mtd, read_addr, readsize, &retlen, read_buf);
                if(ret != 0 && retlen != readsize) {
                        printf("%s, read addr: 0x%.8x len: 0x%.8x 's value err, ret: %d, retlen: 0x%.8x\n",\
			 __func__, (unsigned int)read_addr, readsize, ret, retlen);
			sysdump_lcd_printf("\nRead nand flash 0x%.8x error, you can dump it use download tools again!\n", read_addr);
			lcd_enable();
			break;
                }
        }
	return ret;
}

/*dump the data saved in nand flash to sdcard when needed*/
void mtd_dump(int fs_type)
{
	int ret = 0;
	unsigned int write_len = 0, write_addr = 0;
	char *buf = NULL;
	unsigned int part_len = 0x8000000;//The size of each ubipac-part file
	int loop = 0;
	char fname[72];
	struct mtd_info *mtd = NULL;

	buf = CONFIG_SYS_SDRAM_BASE;//After dump memory to sdcard, we suppose the whole memory except u-boot used are avaliable.
	mtd = get_mtd_device_nm(UBIPAC_PART);
	if(mtd == NULL) {
		printf("Can't get the mtd part: %s\n", UBIPAC_PART);
		return;
	}

	debugf("Begin to dump 0x%.8x ubipac to sdcard!\n", mtd->size);
	for(write_addr = 0; write_addr < mtd->size; write_addr += write_len, loop++)
	{
		write_len = (mtd->size - write_addr) > part_len ? part_len : (mtd->size - write_addr);
		debugf("begin to read 0x%.8x value to ubipac%d\n", write_len, loop);
		memset(buf, 0, write_len);
		ret = read_nand_to_ram(mtd, (loff_t)(write_addr), write_len, buf);
		if(ret != 0) {
			printf("%s, read ubipac%d error, the ret is %d\n", __func__, loop, ret);
			break;
		}
		debugf("read ubipac%d end\n", loop);

		memset(fname, 0, 72);
		sprintf(fname, "ubipac%d", loop);
		write_mem_to_mmc(NULL, fname, fs_type, buf, write_len);
		debugf("write ubipac%d end\n", loop);
	}
	put_mtd_device(mtd);
}
#endif

unsigned char *get_dt_addr(void)
{
	unsigned char *dt_addr = DT_ADR;
	boot_img_hdr *hdr = (void *)raw_header;
	if (1 == _get_kernel_ramdisk_dt_offset(hdr, "boot"))
		load_fixup_dt_img("boot", &dt_addr);
	return dt_addr;
}

static int check_dts_sysdump_node(unsigned char *fdt_blob)
{
	int nodeoffset = fdt_path_offset(fdt_blob, "/sprd-sysdump");
	debugf("sysdump nodeoffset = %d\n", nodeoffset);

	return nodeoffset;
}

static int check_dts_reserved_mem_node(unsigned char *fdt_blob)
{
	int nodeoffset = fdt_path_offset(fdt_blob, "/reserved-memory");

	debugf("reserved-memory nodeoffset = %d\n", nodeoffset);

	return nodeoffset;
}

static int fill_dump_mem(unsigned char *fdt_blob, struct sysdump_mem *sprd_dump_mem, int nodeoffset)
{
	int sprd_dump_mem_num, lenp, size, offset, phandle, lookup;
	unsigned long *ptr = NULL, *ptr_end = NULL;
	int i = 0;

	ptr = fdt_getprop(fdt_blob, nodeoffset, "memory-region", &lenp);
	__be32 *list = fdt_getprop(fdt_blob, nodeoffset, "memory-region", &size);
	if (list == NULL) {
		debugf("no memory-region property.\n");
		return 0;
	}
	__be32 *list_end = list + size / sizeof(*list);

	while (list < list_end) {
		debugf("while 1.\n");
		phandle = be32_to_cpup(list++);
		lookup = fdt_node_offset_by_phandle(fdt_blob, phandle);

		ptr = fdt_getprop(fdt_blob, lookup, "reg", &lenp);
		if (ptr == NULL)
			debugf("no memory-region property.\n");
		else {
			ptr_end = ptr + lenp / (sizeof(SYSDUMP_LONG));
			while (ptr < ptr_end) {
				debugf("while 2.\n");
				sprd_dump_mem[i].paddr = fdt_addr_to_cpu (*ptr ++);
#ifdef CONFIG_X86
				sprd_dump_mem[i].paddr = (sprd_dump_mem[i].paddr<<32) | fdt_addr_to_cpu(*ptr ++);
#endif
				sprd_dump_mem[i].vaddr = __va(sprd_dump_mem[i].paddr);
				sprd_dump_mem[i].size  = fdt_size_to_cpu (*ptr ++);
#ifdef CONFIG_X86
				sprd_dump_mem[i].size  = (sprd_dump_mem[i].size<<32) | fdt_addr_to_cpu(*ptr ++);
#endif
				sprd_dump_mem[i].soff  = 0xffffffff;
				sprd_dump_mem[i].type  = SYSDUMP_RAM;
				debugf("sprd_dump_mem[%d].paddr is %lx\n", i, sprd_dump_mem[i].paddr);
				debugf("sprd_dump_mem[%d].size  is %lx\n", i, sprd_dump_mem[i].size);
				i ++;
			}
		}
	}
	sprd_dump_mem_num = i;
	return sprd_dump_mem_num;
}

static int get_dts_version(unsigned char *fdt_blob, int nodeoffset)
{
	__be32 *list = NULL;
	int lenp, version;

	list = fdt_getprop(fdt_blob, nodeoffset, "version", &lenp);
	if (list == NULL) {
		debugf("no version property in sysdump.\n");
		version = 0;
	} else {
		version = fdt_size_to_cpu(*list);
	}
	debugf("dts sysdump version is 0x%x.\n", version);
	return version;
}

static unsigned long get_sprd_dump_size_auto(struct sysdump_mem *sysmem, int mem_num)
{
	int i;
	SYSDUMP_LONG size = 0;

	for(i=0; i<mem_num; i++) {
		size += sysmem[i].size;
	}
	debugf("max size is %llx.\n", size);
	return size;
}

static unsigned long get_sprd_dump_size(unsigned char *fdt_blob, int nodeoffset)
{
	unsigned long *ptr = NULL, *ptr_end = NULL;
	__be32 *list = NULL, *list_end = NULL;
	SYSDUMP_LONG size = 0, temp;
	int lenp, phandle, lookup;

	list = fdt_getprop(fdt_blob, nodeoffset, "memory-region", &lenp);
	if (list == NULL)
		debugf("no memory-region property.\n");
	else {
		list_end = list + lenp / sizeof(*list);
		while (list < list_end) {
			phandle = be32_to_cpup(list++);
			lookup = fdt_node_offset_by_phandle(fdt_blob, phandle);

			ptr = fdt_getprop(fdt_blob, lookup, "reg", &lenp);
			if (ptr == NULL) {
				debugf("no memory-region reg property.\n");
				break;
			}
			else {
				ptr_end = ptr + lenp / (sizeof(SYSDUMP_LONG));
				while (ptr < ptr_end) {
					ptr ++;
#ifdef CONFIG_X86
					ptr ++;
#endif
					temp = fdt_size_to_cpu(*ptr++);
#ifdef CONFIG_X86
					temp = (temp<<32)|fdt_size_to_cpu(*ptr++);
#endif
					size += temp;
					debugf("size is %lx\n",size);
				}
			}
		}
	}
	list = fdt_getprop(fdt_blob, nodeoffset, "iomem", &lenp);
	if (list == NULL)
		debugf("no iomem property.\n");
	else {
		list_end = list + lenp / sizeof(*list);
		while (list < list_end) {
			phandle = be32_to_cpup(list++);
			lookup = fdt_node_offset_by_phandle(fdt_blob, phandle);

			ptr = fdt_getprop(fdt_blob, lookup, "reg", &lenp);
			if (ptr == NULL) {
				debugf("no iomem reg property.\n");
				break;
			}
			else {
				ptr_end = ptr + lenp / (sizeof(unsigned long));
				while (ptr < ptr_end) {
					ptr ++;
					size += fdt_size_to_cpu(*ptr);
					ptr ++;
					debugf("size is %lx\n",size);
				}
			}
		}
	}
	return size;
}

/* i: memory region form dts */
static void write_to_mmc(char *path, char *filename, int fs_type,
	void *memaddr, struct sysdump_mem *mem, int i)
{
#ifdef CONFIG_RAMDUMP_NO_SPLIT
	sprintf(filename,
		SYSDUMP_FOLDER_NAME"/1/"SYSDUMP_CORE_NAME_FMT"_"LLX"-"LLX"_dump.lst",
		i + 1, mem[i].paddr, mem[i].paddr+mem[i].size -1);
	write_mem_to_mmc(path, filename, fs_type, memaddr, mem[i].size);
#else
	int j;
	if (mem[i].size <= SZ_8M) {
		sprintf(filename, SYSDUMP_FOLDER_NAME"/1/"SYSDUMP_CORE_NAME_FMT"_"LLX,
			i + 1,mem[i].paddr);
		write_mem_to_mmc(path, filename, fs_type, memaddr, mem[i].size);
	} else {
		for (j = 0; j < mem[i].size / SZ_8M; j++) {
			sprintf(filename, SYSDUMP_FOLDER_NAME"/1/"SYSDUMP_CORE_NAME_FMT"_%03d_"LLX,
				i + 1, j, mem[i].paddr+j*SZ_8M);
			write_mem_to_mmc(path, filename, fs_type, memaddr + j*SZ_8M, SZ_8M);
		}
		if (mem[i].size % SZ_8M) {
			sprintf(filename, SYSDUMP_FOLDER_NAME"/1/"SYSDUMP_CORE_NAME_FMT"_%03d_"LLX,
				i + 1, j, mem[i].paddr+j*SZ_8M);
			write_mem_to_mmc(path, filename, fs_type, memaddr + j*SZ_8M, (mem[i].size % SZ_8M));
		}
	}
#endif
}

#ifdef CONFIG_X86
/*
 * i: memory region form dts
 * k: number k block in i
 */
static void write_to_mmc_x86(char *path, char *filename, int fs_type,
				void *memaddr, struct sysdump_mem *mem,
				int i, int k, unsigned long size)
{
	unsigned long long vaddr = mem[i].paddr+k*SYSDUMP_256M;

#ifdef CONFIG_RAMDUMP_NO_SPLIT
	sprintf(filename, SYSDUMP_FOLDER_NAME"/1/"SYSDUMP_CORE_NAME_FMT"_%03d_"LLX"-"LLX"_dump.lst",
		i + 1, k, vaddr, vaddr+size -1);
	write_mem_to_mmc(path, filename, fs_type, memaddr, size);
#else
	int j;
	if (size <= SZ_8M) {
		sprintf(filename, SYSDUMP_FOLDER_NAME"/1/"SYSDUMP_CORE_NAME_FMT"_"LLX, i + 1, vaddr);
		write_mem_to_mmc(path, filename, fs_type, memaddr, size);
	} else {
		for (j = 0; j < size/SZ_8M; j++) {
			sprintf(filename, SYSDUMP_FOLDER_NAME"/1/"SYSDUMP_CORE_NAME_FMT"_%03d_"LLX,
				i + 1, j, vaddr+j*SZ_8M);
			write_mem_to_mmc(path, filename, fs_type, memaddr + j*SZ_8M, SZ_8M);
		}
		if (size%SZ_8M) {
			sprintf(filename, SYSDUMP_FOLDER_NAME"/1/"SYSDUMP_CORE_NAME_FMT"_%03d_"LLX,
				i + 1, j, vaddr+j*SZ_8M);
			write_mem_to_mmc(path, filename, fs_type, memaddr + j*SZ_8M, (size % SZ_8M));
		}
	}
#endif
}

static u32 e820_map_auto_detect(struct e820entry *entries)
{
	extern ddr_info_t* get_ddr_range(void);
	u32 e820_entries_detect = 0;
	int i=0;
	ddr_info_t* ddr_info = get_ddr_range();
	e820_entries_detect = ddr_info->section_number;

	entries->addr = U32_T_U64(ddr_info->sec_info[0].start_address_high,ddr_info->sec_info[0].start_address_low)+VMM_RESERVE_SIZE;
	entries->size  = U32_T_U64(ddr_info->sec_info[0].end_address_high,ddr_info->sec_info[0].end_address_low)-entries->addr;
	entries->type  = E820_RAM;
	debugf("e820 addr = 0x%08llx, size = 0x%lx.\n", entries->addr, entries->size);
	entries++;

	for (i = 1; i < e820_entries_detect; i++) {
		entries->addr = U32_T_U64(ddr_info->sec_info[i].start_address_high,ddr_info->sec_info[i].start_address_low);
		entries->size  = U32_T_U64(ddr_info->sec_info[i].end_address_high,ddr_info->sec_info[i].end_address_low)-entries->addr;
		entries->type  = E820_RAM;
		debugf("e820 addr = 0x%08llx, size = 0x%lx.\n", entries->addr, entries->size);
		entries++;
	}
	debugf("e820 num is %d.\n", e820_entries_detect);
	return e820_entries_detect;
}

static void reorder_reserved_memory_by_addr(struct mem_reserved *memArray, int size)
{
	int i, j, min;
	struct mem_reserved temp;

	for (i=0; i<size; i++) {
		min = i;
		for (j=i+1; j<size; j++) {
			if (memArray[min].addr > memArray[j].addr) {
				min = j;
			}
		}
		if (min != i) {
			temp.addr = memArray[min].addr;
			temp.size = memArray[min].size;
			memArray[min].addr = memArray[i].addr;
			memArray[min].size = memArray[i].size;
			memArray[i].addr = temp.addr;
			memArray[i].size = temp.size;
		}
	}
	return;
}

static int fill_dump_mem_auto_x86(unsigned char *fdt_blob, struct sysdump_mem *sprd_dump_mem,
					int nodeoffset, int ofs_remem, int e820_num, struct e820entry *entries, int reset_mode)
{
	int sprd_dump_mem_num, lenp, size, phandle, lookup, num;
	unsigned long *ptr = NULL, *ptr_end = NULL;
	int i = 0, reserve_num = 0, j = 0, e820_used = 0, k;
	int offset;
	unsigned long long maddr, msize, base_addr, old_addr=0, old_size=0;

	/* step1: vmm region */
	if (VMM_RESERVE_SIZE) {
		sprd_dump_mem[0].paddr = 0;
		sprd_dump_mem[0].vaddr = 0;
		sprd_dump_mem[0].size  = VMM_RESERVE_SIZE;
		sprd_dump_mem[0].soff  = 0xffffffff;
		sprd_dump_mem[0].type  = SYSDUMP_RAM;
		i++;
	}

	/* step2: memory region from kernel dts */
	if (reset_mode == CMD_TOS_PANIC_MODE) {
		ptr = fdt_getprop(fdt_blob, nodeoffset, "memory-region-se", &lenp);
		__be32 *list = fdt_getprop(fdt_blob, nodeoffset, "memory-region-se", &size);
		if (list == NULL) {
			debugf("no memory-region-se property.\n");
			goto memory_done;
		}
		__be32 *list_end = list + size / sizeof(*list);

		while (list < list_end) {
			debugf("while 1.\n");
			phandle = be32_to_cpup(list++);
			lookup = fdt_node_offset_by_phandle(fdt_blob, phandle);
			ptr = fdt_getprop(fdt_blob, lookup, "reg", &lenp);
			if (ptr == NULL) {
				debugf("no reg property in memory-region-se.\n");
				goto memory_done;
			}
			else {
				ptr_end = ptr + lenp / (sizeof(unsigned long long));
				while (ptr < ptr_end) {
					debugf("while 2.\n");
					sprd_dump_mem[i].paddr = fdt_addr_to_cpu (*ptr ++);
					sprd_dump_mem[i].paddr = (sprd_dump_mem[i].paddr<<32) | fdt_addr_to_cpu(*ptr ++);
					sprd_dump_mem[i].vaddr = __va(sprd_dump_mem[i].paddr);
					sprd_dump_mem[i].size  = fdt_size_to_cpu (*ptr ++);
					sprd_dump_mem[i].size  = (sprd_dump_mem[i].size<<32) | fdt_addr_to_cpu(*ptr ++);
					sprd_dump_mem[i].soff  = 0xffffffff;
					sprd_dump_mem[i].type  = SYSDUMP_RAM;
					i ++;
				}
			}
		}
	}

	/* step3: auto calculate linux memory region */
	if ((reset_mode != CMD_TOS_PANIC_MODE) && (reset_mode != CMD_VMM_PANIC_MODE)) {
		/* save reserved-memory region to array */
		num = 0;
		for (offset = fdt_first_subnode(fdt_blob, ofs_remem); offset >= 0; offset = fdt_next_subnode(fdt_blob, offset)) {
			fdt_get_addr_size(fdt_blob, offset, "reg", &maddr, &msize);
			mem_res_arry[num].addr = maddr;
			mem_res_arry[num].size = msize;
		//	debugf("reserved-memory addr is %llx\n", mem_res_arry[num].addr);
		//	debugf("reserved-memory size is %llx\n", mem_res_arry[num].size);
			num++;
		}
		debugf("reserved-memory num is %d\n.", num);

		/* reorder reserved-memory by addr */
		reorder_reserved_memory_by_addr(mem_res_arry, num);

		/* calculate linux memory region */
		base_addr = VMM_RESERVE_SIZE;
		for (k=0; k<num; k++) {
			maddr = mem_res_arry[k].addr;
			msize = mem_res_arry[k].size;
			debugf("base_addr is 0x%llx.\n", base_addr);
			debugf("fdt addr is %llx\n", maddr);
			debugf("fdt size is %llx\n", msize);

			if ((maddr + msize) > (entries->addr + entries->size +1)) {  //next entry
				base_addr = entries->addr + entries->size + 1;
				entries++;  e820_used++;
				if (base_addr > old_addr+old_size) {
					debugf("d1\n");
					sprd_dump_mem[i].paddr = old_addr + old_size;
					sprd_dump_mem[i].vaddr = __va(sprd_dump_mem[i].paddr);
					sprd_dump_mem[i].size  = base_addr - sprd_dump_mem[i].paddr;
					sprd_dump_mem[i].soff  = 0xffffffff;
					sprd_dump_mem[i].type  = SYSDUMP_RAM;
					i++;
				}
				if (maddr > entries->addr) {
					debugf("d2\n");
					sprd_dump_mem[i].paddr = entries->addr;
					sprd_dump_mem[i].vaddr = __va(sprd_dump_mem[i].paddr);
					sprd_dump_mem[i].size  = maddr - entries->addr;
					sprd_dump_mem[i].soff  = 0xffffffff;
					sprd_dump_mem[i].type  = SYSDUMP_RAM;
					i++;
				}
			} else {   //this entry
				if (maddr == base_addr) {
					base_addr += msize;
				} else if (maddr > base_addr) {
					debugf("d3\n");
					sprd_dump_mem[i].paddr = base_addr;
					sprd_dump_mem[i].vaddr = __va(sprd_dump_mem[i].paddr);
					sprd_dump_mem[i].size  = maddr - base_addr;
					sprd_dump_mem[i].soff  = 0xffffffff;
					sprd_dump_mem[i].type  = SYSDUMP_RAM;
					base_addr = maddr + msize;
					i++;
				} else {
					debugf("error: maddr < base_addr.\n");
				}
			}

			old_addr = maddr;
			old_size = msize;
		}

		if (e820_used <= e820_num-1) {
			for (j=e820_used; j<=e820_num-1; j++) {
				if (j == e820_used) {
					if ((maddr+msize) < (entries->addr + entries->size + 1)) {
						debugf("d4\n");
						sprd_dump_mem[i].paddr = maddr+msize;
						sprd_dump_mem[i].vaddr = __va(sprd_dump_mem[i].paddr);
						sprd_dump_mem[i].size  = entries->addr + entries->size + 1 - sprd_dump_mem[i].paddr;
						sprd_dump_mem[i].soff  = 0xffffffff;
						sprd_dump_mem[i].type  = SYSDUMP_RAM;
						i++;
					}
					entries++;
				} else {
					debugf("d5\n");
					sprd_dump_mem[i].paddr = entries->addr;
					sprd_dump_mem[i].vaddr = __va(sprd_dump_mem[i].paddr);
					sprd_dump_mem[i].size  = entries->size + 1;
					sprd_dump_mem[i].soff  = 0xffffffff;
					sprd_dump_mem[i].type  = SYSDUMP_RAM;
					i++;
					entries++;
				}
			}
		}

		/* calculate specific reserved-memory*/
		ptr = fdt_getprop(fdt_blob, nodeoffset, "memory-region-re", &lenp);
		__be32 *list = fdt_getprop(fdt_blob, nodeoffset, "memory-region-re", &size);
		if (list == NULL) {
			debugf("no memory-region-re property.\n");
			goto memory_done;
		}

		__be32 *list_end = list + size / sizeof(*list);

		while (list < list_end) {
			debugf("memory-region-re 1.\n");
			phandle = be32_to_cpup(list++);
			lookup = fdt_node_offset_by_phandle(fdt_blob, phandle);
			ptr = fdt_getprop(fdt_blob, lookup, "reg", &lenp);
			if (ptr == NULL) {
				debugf("no reg property in memory-region-re.\n");
				goto memory_done;
			}
			else {
				ptr_end = ptr + lenp / (sizeof(unsigned long long));
				while (ptr < ptr_end) {
					debugf("memory-region-re 2.\n");
					sprd_dump_mem[i].paddr = fdt_addr_to_cpu (*ptr ++);
					sprd_dump_mem[i].paddr = (sprd_dump_mem[i].paddr<<32) | fdt_addr_to_cpu(*ptr ++);
					sprd_dump_mem[i].vaddr = __va(sprd_dump_mem[i].paddr);
					sprd_dump_mem[i].size  = fdt_size_to_cpu (*ptr ++);
					sprd_dump_mem[i].size  = (sprd_dump_mem[i].size<<32) | fdt_addr_to_cpu(*ptr ++);
					sprd_dump_mem[i].soff  = 0xffffffff;
					sprd_dump_mem[i].type  = SYSDUMP_RAM;
					i ++;
				}
			}
		}
	}

memory_done:
	/* step4: return */
	sprd_dump_mem_num = i;
	debugf("auto sprd_dump_mem_num is %d\n", sprd_dump_mem_num);
	for (i=0; i<sprd_dump_mem_num; i++) {
		debugf("auto sprd_dump_mem[%d].paddr is %llx\n", i, sprd_dump_mem[i].paddr);
		debugf("auto sprd_dump_mem[%d].size  is %llx\n", i, sprd_dump_mem[i].size);
	}
	return sprd_dump_mem_num;
}
#endif

void write_sysdump_before_boot(int rst_mode)
{
	int i, j, k, sprd_dump_mem_num, ret, fs_type;
	int auto_test_flag = 0;
	char fnbuf[72] = {0}, fnbuf_rename[72] = {0}, *path, *waddr;
	struct rtc_time tm;
	struct sysdump_mem *mem;
	struct sysdump_mem sprd_dump_mem[MAX_NUM_DUMP_MEM];
	struct sysdump_info *infop = NULL;
	unsigned long sprd_sysdump_magic = 0;
	int nodeoffset = 0, ofs_remem = 0;
	dir_entry *dentptr = NULL;
	unsigned char *dt_addr = NULL;
	int dts_version=0;	 /* 0:old version, memory region transfer by DTS;  0xA1: auto calculate linux memory region */

#ifdef CONFIG_X86
	unsigned long mem_virt;
	int e820_num;
	struct e820entry eny[10];
#endif
	sprd_sysdump_magic = SPRD_SYSDUMP_MAGIC;

	infop = (struct sysdump_info *)sprd_sysdump_magic;
	debugf("rst_mode:0x%x, Check if need to write sysdump info of 0x%08lx to file...\n", rst_mode,
	sprd_sysdump_magic);
	if ((rst_mode == CMD_WATCHDOG_REBOOT) || (rst_mode == CMD_AP_WATCHDOG_REBOOT) || (rst_mode == CMD_UNKNOW_REBOOT_MODE) || \
		(rst_mode == CMD_EXT_RSTN_REBOOT_MODE) || ((rst_mode == CMD_PANIC_REBOOT) && !memcmp(infop->magic, SYSDUMP_MAGIC, sizeof(infop->magic))) \
		|| (rst_mode == CMD_SPECIAL_MODE) || (rst_mode == CMD_VMM_PANIC_MODE) || (rst_mode == CMD_TOS_PANIC_MODE)) {

		debugf("Now enter in sysdump!!!\n");

		dt_addr = get_dt_addr();
		nodeoffset = check_dts_sysdump_node(dt_addr);
		if (nodeoffset < 0) {
			debugf("ERROR: device tree must have /sprd-sysdump node %s.\n", fdt_strerror(nodeoffset));
			goto FINISH;
		}
		dts_version = get_dts_version(dt_addr, nodeoffset);
		memset(infop->magic, 0, sizeof(infop->magic));

#ifdef CONFIG_SPLASH_SCREEN
		extern int drv_lcd_init(void);
		drv_lcd_init();
		lcd_splash(LOGO_PART);
		lcd_enable();
#endif

		/* display on screen */
		display_writing_sysdump();
		#define CONSOLE_COLOR_RED (0x1f<<11)
		#define CONSOLE_COLOR_GREEN 0x07e0
		sysdump_lcd_printf("\nReset mode: %s\n\n",GET_RST_MODE(rst_mode));
		lcd_enable();

		if (init_mmc_fat(&fs_type)) {
			debugf("ERROR: init_mmc_fat,fs_type=%d.\n", fs_type);
			sysdump_lcd_printf("\ninit_mmc_fat failed, Please check SD Card!!!.\nPress volume up key (Exp power key) to continue...");
                        lcd_enable();
                        wait_for_keypress();
			goto FINISH;
		}

		if (dts_version == 0xA1) {
			debugf("Dts new version, auto memory region.\n");
			ofs_remem = check_dts_reserved_mem_node(dt_addr);
			if (ofs_remem < 0) {
				debugf("ERROR: device tree must have /reserved-memory node %s.\n", fdt_strerror(ofs_remem));
				goto FINISH;
			}
#ifdef CONFIG_X86
			e820_num = e820_map_auto_detect(eny);
			infop->dump_mem_paddr = (unsigned long)sprd_dump_mem;

			sprd_dump_mem_num = fill_dump_mem_auto_x86(dt_addr, sprd_dump_mem, nodeoffset, ofs_remem, e820_num, eny, rst_mode);
			infop->mem_num = sprd_dump_mem_num;
			infop->elfhdr_size = get_elfhdr_size(infop->mem_num);

			if (rst_mode == CMD_VMM_PANIC_MODE) {
				strcpy(infop->time, "vmm_panic");
			} else if (rst_mode == CMD_TOS_PANIC_MODE) {
				strcpy(infop->time, "tos_panic");
			}

			memset((char *)infop + sizeof(*infop), 0, infop->elfhdr_size);
			sysdump_fill_core_hdr(NULL, sprd_dump_mem,	sprd_dump_mem_num,
					(char *)infop + sizeof(*infop), infop->mem_num + 1, infop->elfhdr_size);
#else
#endif
		} else if ((rst_mode == CMD_WATCHDOG_REBOOT) || (rst_mode == CMD_AP_WATCHDOG_REBOOT) || \
			(rst_mode == CMD_UNKNOW_REBOOT_MODE) || (rst_mode == CMD_EXT_RSTN_REBOOT_MODE) || \
			(rst_mode == CMD_SPECIAL_MODE) || (rst_mode == CMD_VMM_PANIC_MODE) || (rst_mode == CMD_TOS_PANIC_MODE)) {

			infop->dump_path[0] = '\0';

			sprd_dump_mem_num = fill_dump_mem(dt_addr, sprd_dump_mem, nodeoffset);
			infop->mem_num = sprd_dump_mem_num;
			infop->dump_mem_paddr = (unsigned long)sprd_dump_mem;
			if (rst_mode == CMD_VMM_PANIC_MODE) {
				strcpy(infop->time, "vmm_panic");
			} else if (rst_mode == CMD_TOS_PANIC_MODE) {
				strcpy(infop->time, "tos_panic");
			} else {
				strcpy(infop->time, "hw_watchdog");
			}
			infop->elfhdr_size = get_elfhdr_size(infop->mem_num);
			infop->crash_key = 0;

			sysdump_fill_core_hdr(NULL,
					sprd_dump_mem,
					sprd_dump_mem_num,
					(char *)infop + sizeof(*infop),
					infop->mem_num + 1,
					infop->elfhdr_size);
		}

		if (strlen(infop->dump_path))
			path = infop->dump_path;
		else
			path = NULL;

	if (fs_type == FS_FAT32) {
		int mod, key_code;
		unsigned long max_size = 0;

		debugf("FileSystem is FAT32!!\n");
		if (dts_version == 0xA1) {
			max_size = get_sprd_dump_size_auto(sprd_dump_mem, sprd_dump_mem_num);
		} else {
			max_size = get_sprd_dump_size(dt_addr, nodeoffset);
		}
		debugf("FAT32 max space size is %lx\n",max_size);

		mod = fat_checksys(max_size);
		if (mod &(FSFATAL | FSNOSPACE)) {
			int nospace_flag = 1;
			if (mod & FSFATAL) {
				sysdump_lcd_printf("\ndetect sd is demaged");
				lcd_enable();
			}
			else if (mod & FSNOSPACE) {
				for (i = 1; i <= SYSDUMP_FOLDER_NUM; i++) {
					sprintf(fnbuf, SYSDUMP_FOLDER_NAME"/%d", i);
					fnbuf[strlen(fnbuf)] = '\0';
					//debugf("fnbuf is %s\n",fnbuf);
					dentptr = check_folder_flag(fnbuf);
					if(dentptr == NULL)
						break;
				}
				if (i != 1) {
					for(j = i; j != 1;) {
						sprintf(fnbuf, SYSDUMP_FOLDER_NAME"/%d", --j);
						fnbuf[strlen(fnbuf)] = '\0';
						ret = delete_folder(fnbuf);
						if(ret == -1) {
							debugf("Error: delete files or folder\n");
							goto FINISH;
						}
						mod = fat_checksys(max_size);
						if (!(mod & FSNOSPACE)) {
							nospace_flag = 0;
							break;
						}
					}
				}
				if (nospace_flag) {
					sysdump_lcd_printf("\ndetect sd have not enough space");
					lcd_enable();
				}
			}
			if (nospace_flag) {
				sysdump_lcd_printf("\npress volumedown to format sd\notherwise press volumeup to skip sysdump\n");
				lcd_enable();

				do {
					udelay(50 * 1000);
					key_code = board_key_scan();
					if (key_code == KEY_VOLUMEDOWN) {
						fat_format();
						break;
					} else if(key_code == KEY_VOLUMEUP)
						goto FINISH;
				} while (1);
			}
		}

		if(do_new_folder(SYSDUMP_FOLDER_NAME)) {
			debugf("Error: creat %s folder failed\n", SYSDUMP_FOLDER_NAME);
			goto FINISH;
		}

		if(check_folder_flag(SYSDUMP_AUTO_TEST)) {
			auto_test_flag = 1;
			debugf("there existed sysdump_auto_test.txt, so jump wait_for_keypress.\n");
		}

		for (i = 1; i <= SYSDUMP_FOLDER_NUM; i++) {
			sprintf(fnbuf, SYSDUMP_FOLDER_NAME"/%d", i);
			fnbuf[strlen(fnbuf)] = '\0';
			//debugf("fnbuf is %s\n",fnbuf);
			dentptr = check_folder_flag(fnbuf);
			if(dentptr == NULL)
				break;
		}

		if (i > SYSDUMP_FOLDER_NUM) {
			debugf("there existed %d history log.\n", SYSDUMP_FOLDER_NUM);
			i --;
			sprintf(fnbuf, SYSDUMP_FOLDER_NAME"/%d", i);
			fnbuf[strlen(fnbuf)] = '\0';
			ret = delete_folder(fnbuf);
			if(ret == -1) {
				debugf("Error: delete files or folder.\n");
				goto FINISH;
			}
		}

		for(j = i - 1; j > 0 && i != 1; j = j - 2) {
				sprintf(fnbuf, SYSDUMP_FOLDER_NAME"/%d", j++);
				fnbuf[strlen(fnbuf)] = '\0';
				sprintf(fnbuf_rename, SYSDUMP_FOLDER_NAME"/%d", j);
				fnbuf_rename[strlen(fnbuf_rename)] ='\0';
				debugf("rename folder from %s to %s.\n", fnbuf, fnbuf_rename);
				ret = rename_folder(fnbuf, fnbuf_rename);
				if (ret == -1) {
					debugf("Error: rename folder !\n");
					break;
				}
			}

		sprintf(fnbuf, SYSDUMP_FOLDER_NAME"/%d", 1);

		if(do_new_folder(fnbuf)) {
			debugf("Error: creat %s folder failed\n", fnbuf);
			goto FINISH;
		}
	}
	else if (fs_type == FS_EXFAT) {
		int mod, key_code;
		unsigned long max_size;

		debugf("FileSystem is ExFAT!!\n");
		if (dts_version == 0xA1) {
			max_size = get_sprd_dump_size_auto(sprd_dump_mem, sprd_dump_mem_num);
		} else {
			max_size = get_sprd_dump_size(dt_addr, nodeoffset);
		}
		debugf("ExFAT max space size is %lx\n",max_size);

		mod = exfat_checksys(max_size);
		if (mod & FSSMSIZE) {
			debugf("SD card volume size is smaller then dumped size. Skip sysdump.\n");
			goto FINISH;
		}

		if (mod &(FSFATAL | FSNOSPACE)) {
			if (mod & FSFATAL)
				sysdump_lcd_printf("\ndetect sd is demaged");
			else if (mod & FSNOSPACE)
				sysdump_lcd_printf("\ndetect sd have not enough space");

			sysdump_lcd_printf("\npress volumedown to format sd\notherwise press volumeup to skip sysdump\n");
			lcd_enable();
			do {
				udelay(50 * 1000);
				key_code = board_key_scan();
				if (key_code == KEY_VOLUMEDOWN) {
					int ret;
					sysdump_lcd_printf("\nvolumedown key is pressed.\nSD Card format starts........\n");
					lcd_enable();
					ret = exfat_format();
					if (ret != 0) {
						sysdump_lcd_printf("\nSD Card format failed. Skip sysdump.\n");
						lcd_enable();
						goto FINISH;
					}
					debugf("SD Card formatting done.\n");
					break;
				} else if(key_code == KEY_VOLUMEUP) {
					sysdump_lcd_printf("\nvolumedup key is pressed and don't format SD Card. sysdump will be skipped.\n");
					lcd_enable();
					goto FINISH;
				}
			} while (1);
		}
	}
	else {
		sysdump_lcd_printf("Invalid file system.  Sysdump will be skipped.  \n");
		lcd_enable();
		goto FINISH;
	}

		tm = get_time_by_sec();

		debugf("time is %04d.%02d.%02d_%02d:%02d:%02d\n", tm.tm_year, tm.tm_mon, \
			tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

		sprintf(fnbuf,SYSDUMP_FOLDER_NAME"/1/%04d_%02d_%02d_%02d_%02d_%02d.txt", \
			tm.tm_year, tm.tm_mon, tm.tm_mday, \
			tm.tm_hour, tm.tm_min, tm.tm_sec);

		char info[1000] = {0};
		sprintf(info,"-%s%s\n-%s%s\n-%s%d\n-%s%d\n-%s"LLX"\n-%s%d\n",
			"time is ",
			infop->time,
			"dump_path is ",
			infop->dump_path,
			"elfhdr_size is ",
			infop->elfhdr_size,
			"mem_num is ",
			infop->mem_num,
			"dump_mem_paddr is ",
			infop->dump_mem_paddr,
			"crash_key is ",
			infop->crash_key);
		write_mem_to_mmc(path, fnbuf, fs_type, (char *)(info), strlen(info));

		sprintf(fnbuf, SYSDUMP_FOLDER_NAME"/1/"SYSDUMP_CORE_NAME_FMT, 0);
		write_mem_to_mmc(path, fnbuf, fs_type,
			(char *)infop + sizeof(*infop), infop->elfhdr_size);

#if 1 /* TODO: jianjun.he */
		mem = (struct sysdump_mem *)infop->dump_mem_paddr;
		for (i = 0; i < infop->mem_num; i++) {
			if (0xffffffff != mem[i].soff)
				waddr = (char *)infop + sizeof(*infop) +
						infop->elfhdr_size + mem[i].soff;
			else {
				debugf("mem[%d].paddr is %llx\n", i, mem[i].paddr);
				debugf("mem[%d].size  is %llx\n", i, mem[i].size);
#ifdef CONFIG_X86
				if (mem[i].paddr > SYSDUMP_4GB) {  //if mem[i].paddr > SYSDUMP_4GB, mem[i].size should <= 512M
					cpu_paging_disable();
					mem_virt = cpu_page_remap(SYSDUMP_MAPPING_ADDR, mem[i].size, mem[i].paddr);

					if (!SPLIT_MEM_REGION) {
						cpu_paging_enable();
						memmove((void *)SYSDUMP_COPY_PADDR, (void *)mem_virt, mem[i].size);
						cpu_paging_disable();
						waddr = SYSDUMP_COPY_PADDR;
					} else {
						for (k=0; k<mem[i].size/SYSDUMP_256M; k++) {
							cpu_paging_enable();
							memmove((void *)SYSDUMP_COPY_PADDR, (void *)mem_virt+SYSDUMP_256M*k, SYSDUMP_256M);
							cpu_paging_disable();
							write_to_mmc_x86(path, fnbuf, fs_type, SYSDUMP_COPY_PADDR+k*SYSDUMP_256M, mem, i, k, SYSDUMP_256M);
						}
						if (mem[i].size%SYSDUMP_256M) {
							cpu_paging_enable();
							memmove((void *)SYSDUMP_COPY_PADDR, (void *)mem_virt+SYSDUMP_256M*k, mem[i].size%SYSDUMP_256M);
							cpu_paging_disable();
							write_to_mmc_x86(path, fnbuf, fs_type, SYSDUMP_COPY_PADDR+k*SYSDUMP_256M, mem, i, k, mem[i].size%SYSDUMP_256M);
						}
						goto FINISH_write_mmc;
					}
			} else
#endif
			{
				waddr = mem[i].paddr;
			}
		}
		write_to_mmc(path, fnbuf, fs_type, waddr, mem, i);
		/*
		#ifdef CONFIG_RAMDUMP_NO_SPLIT
				sprintf(fnbuf, SYSDUMP_FOLDER_NAME"/1/"SYSDUMP_CORE_NAME_FMT"_0x%08lx-0x%08lx_dump.lst", i + 1, mem[i].paddr, mem[i].paddr + mem[i].size -1);
				write_mem_to_mmc(path, fnbuf, fs_type, waddr, mem[i].size);
		#else
			if (mem[i].size <= SZ_8M) {
				sprintf(fnbuf, SYSDUMP_FOLDER_NAME"/1/"SYSDUMP_CORE_NAME_FMT, i + 1);
				write_mem_to_mmc(path, fnbuf, fs_type, waddr, mem[i].size);
			} else {
				for (j = 0; j < mem[i].size / SZ_8M; j++) {
					sprintf(fnbuf, SYSDUMP_FOLDER_NAME"/1/"SYSDUMP_CORE_NAME_FMT"_%03d",
						i + 1, j);
					write_mem_to_mmc(path, fnbuf, fs_type, waddr + j * SZ_8M, SZ_8M);
				}

				if (mem[i].size % SZ_8M) {
					sprintf(fnbuf, SYSDUMP_FOLDER_NAME"/1/"SYSDUMP_CORE_NAME_FMT"_%03d",
						i + 1, j);
					write_mem_to_mmc(path, fnbuf, fs_type, waddr + j * SZ_8M,
									(mem[i].size % SZ_8M));
				}
			}
		#endif
		*/
		}
#else
		for (i = 0; i < infop->mem_num; i++) {
			sprintf(fnbuf, SYSDUMP_CORE_NAME_FMT, /*infop->time,*/ i + 1);
			write_mem_to_mmc(path, fnbuf, fs_type, mem[i].paddr, mem[i].size);
		}
#endif

FINISH_write_mmc:
#ifdef CONFIG_ETB_DUMP
		debugf ("\n Start to dump ETB trace data.\n");
		sprd_etb_hw_dis();
		sprd_etb_dump();
		write_mem_to_mmc(0, "etbdata_uboot.bin", fs_type, &etb_dump_mem[0], SZ_32K);
		debugf ("\n Dump ETB data finished.\n");
#endif

#ifdef CONFIG_NAND_BOOT
		sysdump_lcd_printf("\nBegin to dump nand flash:\n");
		lcd_enable();
		mtd_dump(fs_type);
#endif
		debugf("\nwriting done.\nPress any key to continue...");
		sysdump_lcd_printf("\nWriting done.\nPress any key (Exp power key) to continue...");
		lcd_enable();
		if(!auto_test_flag)
			wait_for_keypress();
FINISH:
		debugf ("sysdump  finish or happen abnormally!!!.\n");
#if (defined CONFIG_X86) && (defined CONFIG_MOBILEVISOR)
		reset_to_normal(CMD_NORMAL_MODE);
#endif
	} else
		debugf("no need.\n");

	return;
}
