/******************************************************************************
 ** File Name:      pmu.c                                             *
 ** Author:         Chris                                              *
 ** DATE:           04/11/2014                                                *
 ** Copyright:      2014 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic information on chip.          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/11/2014     Chris               Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include <asm/io.h>
#include "adi_hal_internal.h"
//#include "asm/arch/wdg_drvapi.h"
#include "asm/arch/sprd_reg.h"
#include "asm/arch-whale2/common.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Macro defines.
 **---------------------------------------------------------------------------*/
#define DDR_PHY_IND_ADDR  0x30010184
#define DDR_PHY_IND_ADDR1 0x30040000
#define DDR_PHY_IND_ADDR2 0x402e3048
#define DDR_PHY_IND_PHY_ALIVE 0x00000001


/**---------------------------------------------------------------------------*
 **                         Struct defines.
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Struct defines.
 **---------------------------------------------------------------------------*/
struct dcdc_core_ds_step_info{
	u32 ctl_reg;
	u32 ctl_sht;
	u32 cal_reg;
	u32 cal_sht;
};


#define CONFIG_PMU_CLK_4M
#define CONFIG_PMU_CLK_RC_4M
#ifndef CONFIG_FPGA
void pmu_commom_config(void)
{
	//TODO
}

static void setup_autopd_mode(void)
{
	//TODO
}

static void dcdc_core_ds_config(uint32_t core_cal_para, uint32_t core_ctrl_para)
{
	/*Get the chip id for eco chip*/
	if(0x1 == CHIP_REG_GET(REG_AON_APB_AON_VER_ID))
		ANA_REG_SET(ANA_REG_GLB_DCDC_CORE_SLP_CTRL1,
			BITS_DCDC_CORE_CAL_DS_SW(core_cal_para)|
			BITS_DCDC_CORE_CTRL_DS_SW(core_ctrl_para)
		);
	else {
		ANA_REG_AND(ANA_REG_GLB_SLP_DCDC_PD_CTRL,
			~BIT_SLP_DCDCCORE_DROP_EN
		);

		ANA_REG_AND(ANA_REG_GLB_SLP_DCDC_LP_CTRL,
			~BIT_SLP_DCDCCORE_LP_EN
		);
	}
}

void CSP_Init(uint32_t gen_para)
{
	uint32_t reg_val;
	//setup_autopd_mode();
	pmu_commom_config();
	dcdc_core_ds_config(0,3);
}

#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif


