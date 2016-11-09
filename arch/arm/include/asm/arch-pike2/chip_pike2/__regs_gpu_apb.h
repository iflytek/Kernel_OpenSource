/*
 * Copyright (C) 2015 Spreadtrum Communications Inc.
 *
 * This file is dual-licensed: you can use it either under the terms
 * of the GPL or the X11 license, at your option. Note that this dual
 * licensing only applies to this file, and not this project as a
 * whole.
 *
 */


#ifndef _GPU_APB_REG_H
#define _GPU_APB_REG_H

#define CTL_BASE_GPU_APB SPRD_GPUAPB_PHYS


#define REG_GPU_APB_APB_RST                       ( CTL_BASE_GPU_APB + 0x0000 )
#define REG_GPU_APB_APB_CLK_CTRL                  ( CTL_BASE_GPU_APB + 0x0004 )
#define REG_GPU_APB_APB_M400_INT_STS              ( CTL_BASE_GPU_APB + 0x0008 )
#define REG_GPU_APB_APB_GPU_PRB_SEL               ( CTL_BASE_GPU_APB + 0x000C )
#define REG_GPU_APB_GPU_EMC_BRG_S0_LPC_CTRL       ( CTL_BASE_GPU_APB + 0x0010 )
#define REG_GPU_APB_GPU_EMC_BRG_M0_LPC_CTRL       ( CTL_BASE_GPU_APB + 0x0014 )
#define REG_GPU_APB_GPU_EMC_BRG_GPV_LPC_CTRL      ( CTL_BASE_GPU_APB + 0x0018 )
#define REG_GPU_APB_GPU_APB_BRG_LPC_CTRL          ( CTL_BASE_GPU_APB + 0x001C )
#define REG_GPU_APB_GPU_CLOCK_SLOWDOEN_CTRL       ( CTL_BASE_GPU_APB + 0x0020 )

/*---------------------------------------------------------------------------
// Register Name   : REG_GPU_APB_APB_RST
// Register Offset : 0x0000
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_GPU_APB_GPU_SOFT_RST                                 BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_GPU_APB_APB_CLK_CTRL
// Register Offset : 0x0004
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_GPU_APB_CLK_GPU_DIV(x)                               (((x) & 0x3) << 4)
#define BIT_GPU_APB_CLK_GPU_SEL(x)                               (((x) & 0x7))

/*---------------------------------------------------------------------------
// Register Name   : REG_GPU_APB_APB_M400_INT_STS
// Register Offset : 0x0008
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_GPU_APB_M400_IRQPP0                                  BIT(4)
#define BIT_GPU_APB_M400_IRQPPMMU0                               BIT(3)
#define BIT_GPU_APB_M400_IRQGP                                   BIT(2)
#define BIT_GPU_APB_M400_IRQGPMMU                                BIT(1)
#define BIT_GPU_APB_M400_IRQPMU                                  BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_GPU_APB_APB_GPU_PRB_SEL
// Register Offset : 0x000C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_GPU_APB_GPU_PRB_SEL(x)                               (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_GPU_APB_GPU_EMC_BRG_S0_LPC_CTRL
// Register Offset : 0x0010
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_GPU_APB_GPU_EMC_BRG_S0_LP_STAT_M                     BIT(22)
#define BIT_GPU_APB_GPU_EMC_BRG_S0_MMTX_STOP_CH                  BIT(21)
#define BIT_GPU_APB_GPU_EMC_BRG_S0_FRC_LSLP_MST                  BIT(20)
#define BIT_GPU_APB_GPU_EMC_BRG_S0_MTX_LP_DISABLE                BIT(19)
#define BIT_GPU_APB_GPU_EMC_BRG_S0_LP_AUTO_CTRL_EN               BIT(18)
#define BIT_GPU_APB_GPU_EMC_BRG_S0_LP_FORCE_M                    BIT(17)
#define BIT_GPU_APB_GPU_EMC_BRG_S0_LP_EB_M                       BIT(16)
#define BIT_GPU_APB_GPU_EMC_BRG_S0_LP_NUM(x)                     (((x) & 0xFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_GPU_APB_GPU_EMC_BRG_M0_LPC_CTRL
// Register Offset : 0x0014
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_GPU_APB_GPU_EMC_BRG_M0_LP_STAT_M                     BIT(22)
#define BIT_GPU_APB_GPU_EMC_BRG_M0_LP_FORCE_M                    BIT(17)
#define BIT_GPU_APB_GPU_EMC_BRG_M0_LP_EB_M                       BIT(16)
#define BIT_GPU_APB_GPU_EMC_BRG_M0_LP_NUM(x)                     (((x) & 0xFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_GPU_APB_GPU_EMC_BRG_GPV_LPC_CTRL
// Register Offset : 0x0018
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_GPU_APB_GPU_EMC_BRG_GPV_LP_STAT_M                    BIT(22)
#define BIT_GPU_APB_GPU_EMC_BRG_GPV_LP_FORCE_M                   BIT(17)
#define BIT_GPU_APB_GPU_EMC_BRG_GPV_LP_EB_M                      BIT(16)
#define BIT_GPU_APB_GPU_EMC_BRG_GPV_LP_NUM(x)                    (((x) & 0xFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_GPU_APB_GPU_APB_BRG_LPC_CTRL
// Register Offset : 0x001C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_GPU_APB_GPU_APB_BRG_S0_LP_STAT_M                     BIT(22)
#define BIT_GPU_APB_GPU_APB_BRG_S0_LP_FORCE_M                    BIT(17)
#define BIT_GPU_APB_GPU_APB_BRG_S0_LP_EB_M                       BIT(16)
#define BIT_GPU_APB_GPU_APB_BRG_S0_LP_NUM(x)                     (((x) & 0xFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_GPU_APB_GPU_CLOCK_SLOWDOEN_CTRL
// Register Offset : 0x0020
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_GPU_APB_GPU_CORE_IDLE_SLOWDOWN_EN                    BIT(8)
#define BIT_GPU_APB_GPU_CORE_IDLE_SLOWDOWN_REAR(x)               (((x) & 0xF) << 4)
#define BIT_GPU_APB_GPU_CORE_IDLE_SLOWDOWN_RATE(x)               (((x) & 0xF))


#endif // _GPU_APB_REG_H
