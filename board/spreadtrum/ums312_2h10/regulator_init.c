#include <asm/arch/common.h>
#include <asm/arch/sprd_reg.h>
#include <adi_hal_internal.h>
#define WCN_26M_EN 0x3

static int power_on_voltage_init(void)
{
	regulator_set_voltage("vddsim2",2800);
/*resolve asic workaround issue that the power supply fluctuate serious.*/
	regulator_set_voltage("vddrf1v25",1245);
	return 0;
}

int regulator_init(void)
{
	/*
	 */
	ANA_REG_OR(ANA_REG_GLB_TSX_CTRL0, BITS_DCXO_26M_REF_OUT_EN(WCN_26M_EN));
	power_on_voltage_init();
	return 0;
}
