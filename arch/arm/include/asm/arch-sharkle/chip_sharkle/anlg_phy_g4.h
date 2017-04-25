/*
 * Copyright (C) 2015 Spreadtrum Communications Inc.
 *
 * This file is dual-licensed: you can use it either under the terms
 * of the GPL or the X11 license, at your option. Note that this dual
 * licensing only applies to this file, and not this project as a
 * whole.
 *
 */


#ifndef _ANLG_PHY_G4_H
#define _ANLG_PHY_G4_H

#define CTL_BASE_ANLG_PHY_G4 0x403F0000


#define REG_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_CTRL0          ( CTL_BASE_ANLG_PHY_G4 + 0x0000 )
#define REG_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_CTRL1          ( CTL_BASE_ANLG_PHY_G4 + 0x0004 )
#define REG_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_CTRL2          ( CTL_BASE_ANLG_PHY_G4 + 0x0008 )
#define REG_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_CTRL3          ( CTL_BASE_ANLG_PHY_G4 + 0x000C )
#define REG_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_BIST_CTRL      ( CTL_BASE_ANLG_PHY_G4 + 0x0010 )
#define REG_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_THM_CTRL       ( CTL_BASE_ANLG_PHY_G4 + 0x0014 )
#define REG_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_THM1_CTRL_0         ( CTL_BASE_ANLG_PHY_G4 + 0x0018 )
#define REG_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_THM1_CTRL_1         ( CTL_BASE_ANLG_PHY_G4 + 0x001C )
#define REG_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_ANA_MPLL_DUMY       ( CTL_BASE_ANLG_PHY_G4 + 0x0020 )

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_CTRL0
// Register Offset : 0x0000
// Description     :
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_N(x)                       (((x) & 0x7FF) << 8)
#define BIT_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_IBIAS(x)                   (((x) & 0x3) << 6)
#define BIT_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_LPF(x)                     (((x) & 0x7) << 3)
#define BIT_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_SDM_EN                     BIT(2)
#define BIT_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_MOD_EN                     BIT(1)
#define BIT_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_DIV_S                      BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_CTRL1
// Register Offset : 0x0004
// Description     :
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_NINT(x)                    (((x) & 0x7F) << 23)
#define BIT_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_KINT(x)                    (((x) & 0x7FFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_CTRL2
// Register Offset : 0x0008
// Description     :
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_RESERVED(x)                (((x) & 0xFF) << 1)
#define BIT_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_POSTDIV                    BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_CTRL3
// Register Offset : 0x000C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_CCS_CTRL(x)                (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_BIST_CTRL
// Register Offset : 0x0010
// Description     :
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_BIST_EN                    BIT(24)
#define BIT_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_BIST_CTRL(x)               (((x) & 0xFF) << 16)
#define BIT_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_BIST_CNT(x)                (((x) & 0xFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_MPLL_THM_CTRL
// Register Offset : 0x0014
// Description     :
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_TEST_CLK_EN                     BIT(13)
#define BIT_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_TEST_SEL                        BIT(12)
#define BIT_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_TEST_THM_SEL                    BIT(11)
#define BIT_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_ANALOG_PLL_RESERVED(x)          (((x) & 0x7FF))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_THM1_CTRL_0
// Register Offset : 0x0018
// Description     :
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_THM_BG_RBIAS_MODE               BIT(2)
#define BIT_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_THM_TEST_SEL(x)                 (((x) & 0x3))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_THM1_CTRL_1
// Register Offset : 0x001C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_THM_BP_MODE                     BIT(24)
#define BIT_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_THM_BP_DATA(x)                  (((x) & 0xFF) << 16)
#define BIT_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_THM_RESERVED(x)                 (((x) & 0xFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_ANA_MPLL_DUMY
// Register Offset : 0x0020
// Description     :
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_ANALOG_MPLL_DUMY_IN(x)          (((x) & 0xFFFF) << 16)
#define BIT_ANLG_PHY_G4_ANALOG_MPLL_THM_TOP_ANALOG_MPLL_DUMY_OUT(x)         (((x) & 0xFFFF))


#endif
