#include <asm/arch/sci_types.h>
#include <adi_hal_internal.h>
#include <asm/arch/common.h>
#include <asm/arch/sprd_reg.h>

static int power_on_voltage_init(void)
{
	return 0;
}

int regulator_init(void)
{
	power_on_voltage_init();
	return 0;
}
