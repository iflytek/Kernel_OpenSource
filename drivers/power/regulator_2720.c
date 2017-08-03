#include <common.h>
#include <asm/io.h>
#include "adc_drvapi.h"
#include "adi_hal_internal.h"
#include <asm/arch/sprd_reg.h>

int regulator_disable_all(void)
{
	return 0;
}

int regulator_enable_all(void)
{
	return 0;
}

int regulator_disable(const char con_id[])
{
	return 0;
}

int regulator_enable(const char con_id[])
{
	return 0;
}

int regulator_set_voltage(const char con_id[], int to_vol)
{
	return 0;
}

int regulator_default_get(const char con_id[])
{
	return 0;
}

void regulator_default_set(const char con_id[], int vol)
{
	return 0;
}

int regulator_default_set_all(void)
{
	return 0;
}

int regulator_pmic_init(void)
{
	//TODO Mingwei
	return 0;
}
