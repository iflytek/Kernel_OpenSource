#include <asm/arch/common.h>
#include <asm/arch/sprd_reg.h>
#include <adi_hal_internal.h>

static int power_on_voltage_init(void)
{
	regulator_set_voltage("vddgen",1875);
	regulator_set_voltage("avdd18",1775);
	regulator_set_voltage("vddrf18a",1775);
	regulator_set_voltage("vddrf18b",1775);

	/*only scaling voltage,didn't power on*/
	regulator_set_voltage("vddcamio",1775);
	return 0;
}

int board_regulator_init(void)
{
	power_on_voltage_init();
	return 0;
}
