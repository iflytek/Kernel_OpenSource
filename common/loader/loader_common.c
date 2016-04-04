#include "loader_common.h"
#include <mmc.h>
#include <fat.h>
#if defined(CONFIG_OF_LIBFDT)
#include <libfdt.h>
#include <fdt_support.h>
#endif
#include "sprd_common_rw.h"

#ifdef CONFIG_MINI_TRUSTZONE
#include "trustzone_def.h"
#endif

#include "sprd_cpcmdline.h"

unsigned char raw_header[8192];

unsigned int g_charger_mode = 0;
char serial_number_to_transfer[SP09_MAX_SN_LEN];

extern int charger_connected(void);
extern void modem_entry(void);
extern void fixup_pmic_items(void);

unsigned short calc_checksum(unsigned char *dat, unsigned long len)
{
	unsigned short num = 0;
	unsigned long chkSum = 0;
	while (len > 1) {
		num = (unsigned short)(*dat);
		dat++;
		num |= (((unsigned short)(*dat)) << 8);
		dat++;
		chkSum += (unsigned long)num;
		len -= 2;
	}
	if (len) {
		chkSum += *dat;
	}
	chkSum = (chkSum >> 16) + (chkSum & 0xffff);
	chkSum += (chkSum >> 16);
	return (~chkSum);
}

unsigned char _chkNVEcc(uint8_t * buf, uint32_t size, uint32_t checksum)
{
	uint16_t crc;

	crc = calc_checksum(buf, size);
	debugf("_chkNVEcc calcout 0x%lx, org 0x%llx\n", crc, checksum);
	return (crc == (uint16_t) checksum);
}

char *get_product_sn(void)
{
	SP09_PHASE_CHECK_T phase_check;

	memset(serial_number_to_transfer, 0x0, SP09_MAX_SN_LEN);

	strcpy(serial_number_to_transfer, "0123456789ABCDEF");
	if (0 != common_raw_read(PRODUCTINFO_FILE_PATITION, sizeof(phase_check), 0, (char *)&phase_check)) {
		errorf("read miscdata error.\n");
		return serial_number_to_transfer;
	}

	if ((phase_check.Magic == SP09_SPPH_MAGIC_NUMBER) && strlen(phase_check.SN1)) {
		memcpy(serial_number_to_transfer, phase_check.SN1, SP09_MAX_SN_LEN);
	}
	return serial_number_to_transfer;
}

void fdt_fixup_all(u8 *fdt_blob)
{
	uint32_t fdt_size;
	boot_img_hdr *hdr = raw_header;
	int err;

	if (fdt_check_header(fdt_blob) != 0) {
		printf("image is not a fdt\n");
	}
	fdt_size = fdt_totalsize(fdt_blob);

	err = fdt_open_into(fdt_blob, fdt_blob, fdt_size + FDT_ADD_SIZE);
	if (err != 0) {
		printf("libfdt fdt_open_into(): %s\n", fdt_strerror(err));
	}
#ifndef CONFIG_MEMORY_DEFRAGMENT
	/* for memory defragment,ramdisk_adr is allocated dynamically,
	so put fdt_initrd_norsvmem after caculate the ramdisk_adr */
	fdt_initrd_norsvmem(fdt_blob, RAMDISK_ADR, RAMDISK_ADR + hdr->ramdisk_size, 1);
#endif

#ifdef CONFIG_SPLASH_SCREEN
	fdt_fixup_lcdid(fdt_blob);
	fdt_fixup_lcdbase(fdt_blob);
	fdt_fixup_lcdsize(fdt_blob);
#endif
	fdt_fixup_adc_calibration_data(fdt_blob);
	fdt_fixup_dram_training(fdt_blob);
	fdt_fixup_ddr_size(fdt_blob);
	fdt_fixup_sysdump_magic(fdt_blob);
#ifdef CONFIG_SECURE_BOOT
	fdt_fixup_secureboot_param(fdt_blob);
#endif
#ifdef CONFIG_NAND_BOOT
	fdt_fixup_mtd(fdt_blob);
#endif

	/*max let cp_cmdline_fixup befor fdt_fixup_cp_boot*/
	cp_cmdline_fixup();

	fdt_fixup_cp_boot(fdt_blob);

	fdt_fixup_serialno(fdt_blob);

	fdt_fixup_chosen_bootargs_board_private(fdt_blob);

#ifdef CONFIG_SANSA_SECBOOT
	//fdt_fixup_socid(fdt_blob);
#endif

	/* for non iq mode, remove reserved iq memory from dtb */
	fdt_fixup_iq_reserved_mem(fdt_blob);

#ifdef CONFIG_MEM_LAYOUT_DECOUPLING
extern int fdt_fixup_cp_coupling_info(void *blob);
	fdt_fixup_cp_coupling_info(fdt_blob);
#endif

	return;
}


static int start_linux_x86()
{
	struct boot_params *base_ptr;
	struct boot_img_hdr *boot_hdr;
	ulong load_address;
	char *bzImage_addr = KERNEL_ADR;
	ulong bzImage_size = 0;
	ulong initrd_addr = RAMDISK_ADR;
	ulong initrd_size = 0;
	ulong dtb_addr = 0;

	disable_interrupts();

	/* Setup board for maximum PC/AT Compatibility */
	setup_pcat_compatibility();
	/* Lets look for */
	base_ptr = load_zimage(bzImage_addr, bzImage_size, &load_address);

	if (!base_ptr) {
		puts("## Kernel loading failed ...\n");
		return -1;
	}
	if (setup_zimage(base_ptr, (char *)base_ptr + COMMAND_LINE_OFFSET,
			0, initrd_addr, initrd_size, dtb_addr)) {
		puts("Setting up boot parameters failed ...\n");
		return -1;
	}

	/* we assume that the kernel is in place */
	return boot_linux_kernel((ulong)base_ptr, load_address, false);
}
#ifdef CONFIG_ARM
static int start_linux(uchar *dt_addr)
{
#ifdef CONFIG_OF_LIBFDT
	void (*theKernel) (int zero, int arch, u32 params);
	theKernel = (void (*)(void *, int, int, int))KERNEL_ADR;

	/*start modem CP */
	modem_entry();

	/*disable all caches */
	cleanup_before_linux();
#ifdef CONFIG_MINI_TRUSTZONE
	trustzone_entry(TRUSTZONE_ADR + 0x200);
#endif
	/* jump to kernel with register set */
	theKernel(0, machine_arch_type, (u32)dt_addr);
#else
	void (*theKernel) (int zero, int arch, u32 params);
	u32 machine_type = 0;

	machine_type = machine_arch_type;	/* get machine type */
	theKernel = (void (*)(int, int, u32))KERNEL_ADR;	/* set the kernel address */

	/*start modem CP */
	cp_cmdline_fixup();
	modem_entry();

	/* jump to kernel with register set */
	theKernel(0, machine_type, VLX_TAG_ADDR);
#endif /*CONFIG_OF_LIBFDT */
	while (1) ;
	return 0;
}
#endif
static int start_linux_armv8(uchar *dt_addr)
{
	void (*theKernel) (void *dtb_addr, int zero, int arch, int reserved);
	theKernel = (void (*)(void *, int, int, int))KERNEL_ADR;

	/*start modem CP */
	modem_entry();

	/*before switch to el2,flush all cache */
	/*FIXME: cleanup_before_linux() will cause panic here, we need to find the solution*/
	flush_dcache_range(CONFIG_SYS_SDRAM_BASE, CONFIG_SYS_SDRAM_END);
#ifdef CONFIG_DUAL_DDR
		flush_dcache_range(CONFIG_SYS_SDRAM_BASE2, CONFIG_SYS_SDRAM_END2);
#endif

#ifdef CONFIG_MINI_TRUSTZONE
	trustzone_entry(TRUSTZONE_ADR + 0x200);
#endif

	/*kernel must run in el2, so here switch to el2 */
	armv8_switch_to_el2();
	theKernel(dt_addr, 0, 0, 0);

	/*never enter here*/
	while (1) ;
	return 0;
}


void vlx_entry(uchar *dt_addr)
{
#ifdef DFS_ON_ARM7
	cp_dfs_param_for_arm7();
#endif
	/*shutdown usb ldo, can't shutdown it in the ldo_sleep.c because download mode must use usb */
	fixup_pmic_items();

#ifdef CONFIG_ARM64
	smp_kick_all_cpus();
	start_linux_armv8(dt_addr);
#endif
#ifdef CONFIG_ARM
	start_linux(dt_addr);
#endif
#ifdef CONFIG_X86
	start_linux_x86();
#endif
}
