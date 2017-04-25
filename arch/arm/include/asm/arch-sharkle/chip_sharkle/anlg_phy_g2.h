/*
 * Copyright (C) 2015 Spreadtrum Communications Inc.
 *
 * This file is dual-licensed: you can use it either under the terms
 * of the GPL or the X11 license, at your option. Note that this dual
 * licensing only applies to this file, and not this project as a
 * whole.
 *
 */


#ifndef _ANLG_PHY_G2_H
#define _ANLG_PHY_G2_H

#define CTL_BASE_ANLG_PHY_G2 0x403D0000


#define REG_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_DPLL_CTRL0          ( CTL_BASE_ANLG_PHY_G2 + 0x0000 )
#define REG_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_DPLL_CTRL1          ( CTL_BASE_ANLG_PHY_G2 + 0x0004 )
#define REG_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_DPLL_CTRL2          ( CTL_BASE_ANLG_PHY_G2 + 0x0008 )
#define REG_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_DPLL_BIST_CTRL      ( CTL_BASE_ANLG_PHY_G2 + 0x000C )
#define REG_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_DPLL_THM_CTRL       ( CTL_BASE_ANLG_PHY_G2 + 0x0010 )
#define REG_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_THM0_CTRL_0         ( CTL_BASE_ANLG_PHY_G2 + 0x0014 )
#define REG_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_THM0_CTRL_1         ( CTL_BASE_ANLG_PHY_G2 + 0x0018 )
#define REG_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_ANA_DPLL_DUMY       ( CTL_BASE_ANLG_PHY_G2 + 0x001C )

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_DPLL_CTRL0
// Register Offset : 0x0000
// Description     :
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_DPLL_N(x)                       (((x) & 0x7FF) << 17)
#define BIT_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_DPLL_IBIAS(x)                   (((x) & 0x3) << 15)
#define BIT_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_DPLL_LPF(x)                     (((x) & 0x7) << 12)
#define BIT_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_DPLL_SDM_EN                     BIT(11)
#define BIT_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_DPLL_MOD_EN                     BIT(10)
#define BIT_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_DPLL_DIV_S                      BIT(9)
#define BIT_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_DPLL_RESERVED(x)                (((x) & 0xFF) << 1)
#define BIT_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_DPLL_POSTDIV                    BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_DPLL_CTRL1
// Register Offset : 0x0004
// Description     :
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_DPLL_NINT(x)                    (((x) & 0x7F) << 23)
#define BIT_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_DPLL_KINT(x)                    (((x) & 0x7FFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_DPLL_CTRL2
// Register Offset : 0x0008
// Description     :
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_DPLL_CCS_CTRL(x)                (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_DPLL_BIST_CTRL
// Register Offset : 0x000C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_DPLL_BIST_EN                    BIT(24)
#define BIT_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_DPLL_BIST_CTRL(x)               (((x) & 0xFF) << 16)
#define BIT_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_DPLL_BIST_CNT(x)                (((x) & 0xFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_DPLL_THM_CTRL
// Register Offset : 0x0010
// Description     :
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_TEST_CLK_EN                     BIT(13)
#define BIT_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_TEST_SEL                        BIT(12)
#define BIT_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_TEST_THM_SEL                    BIT(11)
#define BIT_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_ANALOG_PLL_RESERVED(x)          (((x) & 0x7FF))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_THM0_CTRL_0
// Register Offset : 0x0014
// Description     :
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_THM_BG_RBIAS_MODE               BIT(2)
#define BIT_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_THM_TEST_SEL(x)                 (((x) & 0x3))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_THM0_CTRL_1
// Register Offset : 0x0018
// Description     :
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_THM_BP_MODE                     BIT(24)
#define BIT_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_THM_BP_DATA(x)                  (((x) & 0xFF) << 16)
#define BIT_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_THM_RESERVED(x)                 (((x) & 0xFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_ANA_DPLL_DUMY
// Register Offset : 0x001C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_ANALOG_DPLL_DUMY_IN(x)          (((x) & 0xFFFF) << 16)
#define BIT_ANLG_PHY_G2_ANALOG_DPLL_THM_TOP_ANALOG_DPLL_DUMY_OUT(x)         (((x) & 0xFFFF))


#endif
