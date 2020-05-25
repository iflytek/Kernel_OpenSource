/*
 * (C) Copyright 2014
 * David Feng <fenghua@phytium.com.cn>
 * Sharma Bhupesh <bhupesh.sharma@freescale.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <asm/io.h>
#include <asm/arch/sprd_reg.h>
#include <common.h>
#include <malloc.h>
#include <boot_mode.h>
#include <adi_hal_internal.h>
#include <chipram_env.h>
#include <sprd_adc.h>
#include <sprd_led.h>
#include <sprd_battery.h>
#include "loader_common.h"

#define BOOT_PART "boot"
DECLARE_GLOBAL_DATA_PTR;
phys_size_t real_ram_size = 0x40000000;

phys_size_t get_real_ram_size(void)
{
        return real_ram_size;
}

void enable_global_clocks(void)
{
	__raw_writel(BIT_AON_APB_AI_EB | BIT_AON_APB_GPU_EB | BIT_AON_APB_MM_EB | BIT_AON_APB_IPA_EB, REG_AON_APB_APB_EB0 + 0x1000);
	while (__raw_readl(REG_PMU_APB_PWR_STATUS2_DBG) & BIT_PMU_APB_PD_MM_TOP_STATE(~0));
	sci_glb_set(REG_MM_AHB_AHB_EB, BIT_MM_AHB_CKG_EB);
}

static void thm_overheate_en(void)
{
	sci_glb_set(REG_AON_APB_OVERHEAT_CTRL, BIT_AON_APB_THM0_OVERHEAT_ALARM_ADIE_EN |
					BIT_AON_APB_THM1_OVERHEAT_ALARM_ADIE_EN |
					BIT_AON_APB_THM2_OVERHEAT_ALARM_ADIE_EN |
					BIT_AON_APB_THM3_OVERHEAT_ALARM_ADIE_EN);
}

int usb_hub_init(void)
{
	/* ud710_6h10 need to work as host mode within CMD_NORMAL_MODE,
	 * or else usb work with device mode in any other boot modes.
	 */
	printf("usb dpdm switch to host within CMD_NORMAL_MODE\n");

	sprd_gpio_request(NULL, USBHUB_DPDM_EN);
	sprd_gpio_direction_output(NULL, USBHUB_DPDM_EN, 1);
	mdelay(5);

	return 0;
}

static void usb_hub_vdd1v2_en(void)
{
	sprd_gpio_request(NULL, USBHUB_VDD1V2_EN);
	sprd_gpio_direction_output(NULL, USBHUB_VDD1V2_EN, 1);
	mdelay(30);

}

static void usb_pci_vdd1v05_en(void)
{
	sprd_gpio_request(NULL, USBPCI_VDD1V05_EN);
	sprd_gpio_direction_output(NULL, USBPCI_VDD1V05_EN, 1);
}

static void set_peclk_100M(void)
{
	u32 reg = 0;

	reg = __raw_readl(REG_ANLG_PHY_G6_ANALOG_PCIEPLL_V_PCIEPLL_V_CTRL0);
	reg |= BIT_ANLG_PHY_G6_ANALOG_PCIEPLL_V_RG_PCIEPLLV_DIVN(0x1F);
	__raw_writel(reg, REG_ANLG_PHY_G6_ANALOG_PCIEPLL_V_PCIEPLL_V_CTRL0);

	/* TODO: Hardware recommended delay 100 ms */
	mdelay(100);
}

static void set_perst(void)
{
	u32 reg = 0;

	reg = __raw_readl(REG_PMU_APB_PCIE_PERST_CTRL_CFG);
	reg &= ~(BIT_PMU_APB_REG_PCIE3_PERST_N_ASSERT);
	__raw_writel(reg, REG_PMU_APB_PCIE_PERST_CTRL_CFG);
}

extern void setup_chipram_env(void);
int board_init(void)
{
	setup_chipram_env();
#ifndef CONFIG_FPGA
	gd->bd->bi_boot_params = PHYS_SDRAM_1 + 0x100;
	ADI_init();
	/*FPGA forbiden*/
	misc_init();
	pmic_adc_Init();
	/*FPGA forbiden*/
	pin_init();
	regulator_init();
	sprd_eic_init();
	sprd_gpio_init();
	sprd_led_init();
	usb_hub_vdd1v2_en();
	/*FPGA forbiden*/
	sprd_pmu_lowpower_init();

	/* Renesas PCIe->USB Chip Sequence */
	usb_pci_vdd1v05_en();
	set_peclk_100M();
	set_perst();

	TDPllRefConfig(1);
	enable_global_clocks();
	thm_overheate_en();
#endif
	return 0;
}
int dram_init(void)
{
#ifdef CONFIG_DDR_AUTO_DETECT
	ulong sdram_base = CONFIG_SYS_SDRAM_BASE;
	ulong sdram_size = 0;
	chipram_env_t * env = CHIPRAM_ENV_LOCATION;
	if (CHIPRAM_ENV_MAGIC != env->magic) {
		printf("Chipram magic wrong , ddr data may be broken\n");
		return 0;
	}

	real_ram_size = 0;

	if (env->cs_number == 1) {
		real_ram_size += env->cs0_size;
		debugf("dram cs0 size %x\n",env->cs0_size);
	} else if(env->cs_number == 2){
		real_ram_size += env->cs0_size;
		real_ram_size += env->cs1_size;
		debugf("dram cs0 size %x\ndram cs1 size %x\n",env->cs0_size, env->cs1_size);
	}

	//real_ram_size = get_ram_size((volatile void *)sdram_base, real_ram_size);
#else
	real_ram_size = REAL_SDRAM_SIZE;
#endif

	gd->ram_size = PHYS_SDRAM_1_SIZE;

	return 0;
}

#ifdef CONFIG_DUAL_DDR
void dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_1_SIZE;
	gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
	gd->bd->bi_dram[1].size = PHYS_SDRAM_2_SIZE;
}
#endif


int misc_init_r(void)
{
	/*reserver for future use*/
	return 0;
}


int board_late_init(void)
{
	boot_mode_t boot_role;
	extern chipram_env_t* get_chipram_env(void);
	chipram_env_t* cr_env = get_chipram_env();
	boot_role = cr_env->mode;

	boot_pwr_check();
	sprdfgu_init();
#if !defined(CONFIG_FPGA)
#ifdef CONFIG_NAND_BOOT
	//extern int nand_ubi_dev_init(void);
	nand_ubi_dev_init();
	debugf("nand ubi init OK!\n");
#endif
	debugf("CHG init OK!\n");
#endif
	board_keypad_init();
	return 0;
}

static boot_mode_enum_type  get_mode_from_plug_in_charger_to_normal_boot(void)
{
	if (charger_connected()) {
		debugf("get mode from charger\n");
		return CMD_NORMAL_MODE;
	} else {
		return CMD_UNDEFINED_MODE;
	}
}

CBOOT_FUNC s_boot_func_array[CHECK_BOOTMODE_FUN_NUM] = {
	get_mode_from_bat_low,
	write_sysdump_before_boot_extend,
	/* 1 get mode from file*/
	get_mode_from_file_extend,
	/* 2 get mode from watch dog*/
	get_mode_from_watchdog,
	/*3 get mode from alarm register*/
	get_mode_from_alarm_register,
	/*0 get mode from calibration detect*/
	get_mode_from_pctool,
	/*4 get mode from charger*/
	get_mode_from_plug_in_charger_to_normal_boot,
	/*5 get mode from keypad*/
	get_mode_from_keypad,
	/*6 get mode from gpio*/
	get_mode_from_gpio_extend,

	/*shutdown device*/
	//get_mode_from_shutdown,
	0
};


static void ud710_6h10_normal_mode(void)
{
	usb_hub_init();
	sprd_led_pattern_set(Green,255);
	vlx_nand_boot(BOOT_PART, BACKLIGHT_ON, LCD_ON);
	return;
}

void ud710_6h10_factorytest_mode(void)
{
	debugf("factorytest enter\n");
	setenv("bootmode", "factorytest");
	usb_hub_init();
	vlx_nand_boot(RECOVERY_PART, BACKLIGHT_ON, LCD_ON);
	return;
}

void board_boot_mode_regist(CBOOT_MODE_ENTRY *array)
{
	MODE_REGIST(CMD_NORMAL_MODE, ud710_6h10_normal_mode);
	MODE_REGIST(CMD_RECOVERY_MODE, recovery_mode);
	MODE_REGIST(CMD_FASTBOOT_MODE, fastboot_mode);
	MODE_REGIST(CMD_WATCHDOG_REBOOT, watchdog_mode);
	MODE_REGIST(CMD_AP_WATCHDOG_REBOOT, ap_watchdog_mode);
	MODE_REGIST(CMD_UNKNOW_REBOOT_MODE, unknow_reboot_mode);
	MODE_REGIST(CMD_PANIC_REBOOT, panic_reboot_mode);
	MODE_REGIST(CMD_AUTODLOADER_REBOOT, autodloader_mode);
	MODE_REGIST(CMD_SPECIAL_MODE, special_mode);
	MODE_REGIST(CMD_CHARGE_MODE, charge_mode);
	MODE_REGIST(CMD_ENGTEST_MODE,engtest_mode);
	MODE_REGIST(CMD_FACTORYTEST_MODE, ud710_6h10_factorytest_mode);
	MODE_REGIST(CMD_CALIBRATION_MODE, calibration_mode);
	MODE_REGIST(CMD_EXT_RSTN_REBOOT_MODE, ud710_6h10_normal_mode);
	MODE_REGIST(CMD_IQ_REBOOT_MODE, iq_mode);
	MODE_REGIST(CMD_ALARM_MODE, alarm_mode);
	MODE_REGIST(CMD_SPRDISK_MODE, sprdisk_mode);
	MODE_REGIST(CMD_AUTOTEST_MODE, autotest_mode);
	return ;
}

