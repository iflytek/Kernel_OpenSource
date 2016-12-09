/*
 * Copyright (C) 2015, Intel Corporation
 *
 * SPDX-License _ Identifier: 	GPL-2.0+
 */

#include <common.h>
#include <asm/ibmpc.h>
#include <asm/pnp_def.h>
#include <netdev.h>
#include <smsc_lpc47m.h>
#include <malloc.h>
#include <boot_mode.h>
#include <adi_hal_internal.h>
#include <sprd_adc.h>
#include <chipram_env.h>

#define SERIAL_DEV PNP_DEV(0x2e, 4)

DECLARE_GLOBAL_DATA_PTR;
phys_size_t real_ram_size = 0x80000000;


int board_init(void)
{
	printf("Board_init:\n");
	sprd_timer_init();
#ifndef CONFIG_FPGA
	/*get env transfer from chipram*/
	setup_chipram_env();
	gd->bd->bi_boot_params = PHYS_SDRAM_1 + 0x100;
	ADI_init();
	misc_init();
	LDO_Init();
	pmic_adc_Init();
	pin_init();
	sprd_eic_init();
	sprd_gpio_init();
	sprd_pmu_lowpower_init();
	TDPllRefConfig(1);
#endif
	return 0;
}

int misc_init_r(void)
{
/*reserver for future use*/
	return 0;
}

int board_late_init(void)
{
#ifndef CONFIG_FPGA
	boot_pwr_check();
#ifdef CONFIG_NAND_BOOT
	//extern int nand_ubi_dev_init(void);
	nand_ubi_dev_init();
	debugf("nand ubi init OK!\n");
#endif
	sprdbat_init();
	debugf("CHG init OK!\n");
	if (sprdbat_get_vbatauxadc_caltype() != 0) {
		/*fix me*/
		DCDC_Cal_ArmCore();
	}
	board_keypad_init();
#endif
	return 0;
}

phys_size_t get_real_ram_size(void)
{
        return real_ram_size;
}

CBOOT_FUNC s_boot_func_array[CHECK_BOOTMODE_FUN_NUM] = {
#ifndef CONFIG_FPGA
	get_mode_from_bat_low,
	write_sysdump_before_boot_extend,
	/* 1 get mode from file*/
	get_mode_from_file_extend,
	/* 2 get mode from watch dog */
	get_mode_from_watchdog,
	/* 3 get mode from alarm register */
	get_mode_from_alarm_register,
	/* 0 get mode from calibration detect */
	get_mode_from_pctool,
	/*4 get mode from charger */
	get_mode_from_charger,
	/*5 get mode from keypad*/
	get_mode_from_keypad,
	0
#else
	0
#endif
};

void board_boot_mode_regist(CBOOT_MODE_ENTRY *array)
{
	MODE_REGIST(CMD_NORMAL_MODE, normal_mode);
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
	MODE_REGIST(CMD_FACTORYTEST_MODE, factorytest_mode);
	MODE_REGIST(CMD_CALIBRATION_MODE, calibration_mode);
	MODE_REGIST(CMD_EXT_RSTN_REBOOT_MODE, normal_mode);
	MODE_REGIST(CMD_IQ_REBOOT_MODE, iq_mode);
	MODE_REGIST(CMD_ALARM_MODE, alarm_mode);
	MODE_REGIST(CMD_SPRDISK_MODE, sprdisk_mode);
	MODE_REGIST(CMD_AUTOTEST_MODE, autotest_mode);
	return;
}
