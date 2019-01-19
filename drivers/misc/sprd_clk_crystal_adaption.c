#include <common.h>
#include <sprd_adc.h>
#include "otp_helper.h"
#include "adi_hal_internal.h"
#include <asm/arch/sprd_reg.h>

#define M26_DCXO_32KLESS 0x8018
#define M26_TCXO_32K 0x8
#define M26_TSX_32KLESS 0x8010

typedef enum k_less {
	K32 = 0,
	K_LESS,
}k_less_t;

typedef enum crystal_type {
	VCTCXO = 0,
	DCXO,
	TSX,
	TCXO,
	M26_TCXO,
	M52_TXCO,
	M26_TSX,
	M52_TSX,
	M26_DCXO,
	M52_DXCO,
}crystal_type_t;

struct clock_table {
	k_less_t k32_less_data;
	crystal_type_t crystal_data;
};

static const struct clock_table clock_table[] = {
	{K32, M26_TCXO},
	{K_LESS, M26_TSX},
	{K_LESS, M26_DCXO},
};

static int get_clockid(void)
{
	unsigned int reg_val = 0;

	reg_val = ANA_REG_GET(ANA_REG_GLB_CLK32KLESS_CTRL0);

	return reg_val;
}

/* Return crystal type */
int sprd_get_crystal(void)
{
	int id,data1,data2,data3;
	int value = get_clockid();

	data1 = value & M26_TCXO_32K;
	data2 = value & M26_TSX_32KLESS;
	data3 = value & M26_DCXO_32KLESS;

	if(data1 != M26_TCXO_32K && data2 != M26_TSX_32KLESS && data3 != M26_DCXO_32KLESS){
		errorf("option value is error\n");
		return -1;
	}

	if(data1 == M26_TCXO_32K)
		id = 0;
	else if (data2 == M26_TSX_32KLESS)
		id = 1;
	else if(data3 == M26_DCXO_32KLESS)
		id = 2;

	return clock_table[id].crystal_data;
}

/* Return 32k less or not */
int sprd_get_32k(void)
{
/* sw auto-adaption */
	int id,data1,data2,data3;
	int value = get_clockid();

	data1 = value & M26_TCXO_32K;
	data2 = value & M26_TSX_32KLESS;
	data3 = value & M26_DCXO_32KLESS;

	if(data1 != M26_TCXO_32K && data2 != M26_TSX_32KLESS && data3 != M26_DCXO_32KLESS){
		errorf("option value is error\n");
		return -1;
	}

	if(data1 == M26_TCXO_32K)
		id = 0;
	else if (data2 == M26_TSX_32KLESS)
		id = 1;
	else if(data3 == M26_DCXO_32KLESS)
		id = 2;

	return clock_table[id].k32_less_data;
}

/* Return board id */
int sprd_get_boardid(void)
{
	return get_clockid();
}

