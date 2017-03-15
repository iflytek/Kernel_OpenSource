#include <asm/arch/common.h>
#include <asm/arch/sprd_reg.h>
#include <adi_hal_internal.h>

static int power_on_voltage_init(void)
{
	regulator_set_voltage("vddsim2",2800);
	return 0;
}

static int regulator_mode_init(void)
{
	/*
	 * fix dcdc_wpa ripple over 70mv during a conversation
	 */
	sci_adi_raw_write(ANA_REG_GLB_DCDC_WPA_REG3,0x898);
}

int regulator_init(void)
{
	power_on_voltage_init();
	regulator_mode_init();
	return 0;
}
