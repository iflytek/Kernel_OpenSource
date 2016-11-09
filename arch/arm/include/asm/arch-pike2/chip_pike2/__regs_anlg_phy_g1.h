/*
 * Copyright (C) 2015 Spreadtrum Communications Inc.
 *
 * This file is dual-licensed: you can use it either under the terms
 * of the GPL or the X11 license, at your option. Note that this dual
 * licensing only applies to this file, and not this project as a
 * whole.
 *
 */


#ifndef _ANLG_PHY_G1_REG_H
#define _ANLG_PHY_G1_REG_H

#define CTL_BASE_ANLG_PHY_G1 SPRD_ANLG_PHY_G1


#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_ANA_BB_PWR_CTRL      ( CTL_BASE_ANLG_PHY_G1 + 0x0000 )
#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_ANA_BB_RST_CTRL      ( CTL_BASE_ANLG_PHY_G1 + 0x0004 )
#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_ANALOG_PLL_RES       ( CTL_BASE_ANLG_PHY_G1 + 0x0008 )
#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_CTRL0           ( CTL_BASE_ANLG_PHY_G1 + 0x000C )
#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_CTRL1           ( CTL_BASE_ANLG_PHY_G1 + 0x0010 )
#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_CTRL2           ( CTL_BASE_ANLG_PHY_G1 + 0x0014 )
#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_BIST_CTRL       ( CTL_BASE_ANLG_PHY_G1 + 0x0018 )
#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_CTRL0           ( CTL_BASE_ANLG_PHY_G1 + 0x001C )
#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_CTRL1           ( CTL_BASE_ANLG_PHY_G1 + 0x0020 )
#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_CTRL2           ( CTL_BASE_ANLG_PHY_G1 + 0x0024 )
#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_BIST_CTRL       ( CTL_BASE_ANLG_PHY_G1 + 0x0028 )
#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_CTRL0          ( CTL_BASE_ANLG_PHY_G1 + 0x002C )
#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_CTRL1          ( CTL_BASE_ANLG_PHY_G1 + 0x0030 )
#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_CTRL2          ( CTL_BASE_ANLG_PHY_G1 + 0x0034 )
#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_BIST_CTRL      ( CTL_BASE_ANLG_PHY_G1 + 0x0038 )
#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_CTRL0          ( CTL_BASE_ANLG_PHY_G1 + 0x003C )
#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_CTRL1          ( CTL_BASE_ANLG_PHY_G1 + 0x0040 )
#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_CTRL2          ( CTL_BASE_ANLG_PHY_G1 + 0x0044 )
#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_BIST_CTRL      ( CTL_BASE_ANLG_PHY_G1 + 0x0048 )
#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_THM_CTRL_0           ( CTL_BASE_ANLG_PHY_G1 + 0x004C )
#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_THM_CTRL_1           ( CTL_BASE_ANLG_PHY_G1 + 0x0050 )
#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_ANA_BB_RSVD          ( CTL_BASE_ANLG_PHY_G1 + 0x0054 )
#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_TEST_CLK_CTRL        ( CTL_BASE_ANLG_PHY_G1 + 0x0058 )
#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_REG_SEL_CFG_0        ( CTL_BASE_ANLG_PHY_G1 + 0x005C )
#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_REG_SEL_CFG_1        ( CTL_BASE_ANLG_PHY_G1 + 0x0060 )
#define REG_ANLG_PHY_G1_ANALOG_BB_TOP_REG_SEL_CFG_2        ( CTL_BASE_ANLG_PHY_G1 + 0x0064 )

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_ANA_BB_PWR_CTRL
// Register Offset : 0x0000
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_PD                                       BIT(4)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_PD                                       BIT(3)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_PD                                      BIT(2)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_PD                                      BIT(1)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_THM_PD                                        BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_ANA_BB_RST_CTRL
// Register Offset : 0x0004
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_RST                                      BIT(4)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_RST                                      BIT(3)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_RST                                     BIT(2)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_RST                                     BIT(1)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_THM_RSTN                                      BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_ANALOG_PLL_RES
// Register Offset : 0x0008
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_ANALOG_PLL_RESERVED(x)                        (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_CTRL0
// Register Offset : 0x000C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_REF_SEL                                  BIT(22)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_N(x)                                     (((x) & 0x7FF) << 11)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_IBIAS(x)                                 (((x) & 0x3) << 9)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_SDM_EN                                   BIT(8)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_MOD_EN                                   BIT(7)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_DIV_S                                    BIT(6)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_RESERVED(x)                              (((x) & 0xF) << 2)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_CLKOUT_EN                                BIT(1)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_LOCK_DONE                                BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_CTRL1
// Register Offset : 0x0010
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_NINT(x)                                  (((x) & 0x7F) << 23)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_KINT(x)                                  (((x) & 0x7FFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_CTRL2
// Register Offset : 0x0014
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_CCS_CTRL(x)                              (((x) & 0xFF) << 3)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_IL_DIV2                                  BIT(2)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_OL_DIV2                                  BIT(1)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_TEST_EN                                  BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_BIST_CTRL
// Register Offset : 0x0018
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_BIST_EN                                  BIT(16)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_DPLL_BIST_CNT(x)                              (((x) & 0xFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_CTRL0
// Register Offset : 0x001C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_REF_SEL                                  BIT(22)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_N(x)                                     (((x) & 0x7FF) << 11)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_IBIAS(x)                                 (((x) & 0x3) << 9)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_SDM_EN                                   BIT(8)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_MOD_EN                                   BIT(7)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_DIV_S                                    BIT(6)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_RESERVED(x)                              (((x) & 0xF) << 2)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_CLKOUT_EN                                BIT(1)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_LOCK_DONE                                BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_CTRL1
// Register Offset : 0x0020
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_NINT(x)                                  (((x) & 0x7F) << 23)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_KINT(x)                                  (((x) & 0x7FFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_CTRL2
// Register Offset : 0x0024
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_CCS_CTRL(x)                              (((x) & 0xFF) << 3)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_IL_DIV2                                  BIT(2)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_OL_DIV2                                  BIT(1)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_TEST_EN                                  BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_BIST_CTRL
// Register Offset : 0x0028
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_BIST_EN                                  BIT(16)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_MPLL_BIST_CNT(x)                              (((x) & 0xFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_CTRL0
// Register Offset : 0x002C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_REF_SEL                                 BIT(27)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_N(x)                                    (((x) & 0x7FF) << 16)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_IBIAS(x)                                (((x) & 0x3) << 14)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_SDM_EN                                  BIT(13)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_MOD_EN                                  BIT(12)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_DIV_S                                   BIT(11)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_RESERVED(x)                             (((x) & 0xF) << 7)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_CLKOUT_EN                               BIT(6)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_DIV1_EN                                 BIT(5)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_DIV2_EN                                 BIT(4)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_DIV3_EN                                 BIT(3)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_DIV5_EN                                 BIT(2)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_DIV7_EN                                 BIT(1)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_LOCK_DONE                               BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_CTRL1
// Register Offset : 0x0030
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_NINT(x)                                 (((x) & 0x7F) << 23)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_KINT(x)                                 (((x) & 0x7FFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_CTRL2
// Register Offset : 0x0034
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_CCS_CTRL(x)                             (((x) & 0xFF) << 3)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_IL_DIV2                                 BIT(2)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_OL_DIV2                                 BIT(1)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_TEST_EN                                 BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_BIST_CTRL
// Register Offset : 0x0038
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_BIST_EN                                 BIT(16)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TWPLL_BIST_CNT(x)                             (((x) & 0xFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_CTRL0
// Register Offset : 0x003C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_REF_SEL                                 BIT(27)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_N(x)                                    (((x) & 0x7FF) << 16)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_IBIAS(x)                                (((x) & 0x3) << 14)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_SDM_EN                                  BIT(13)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_MOD_EN                                  BIT(12)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_DIV_S                                   BIT(11)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_RESERVED(x)                             (((x) & 0xF) << 7)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_CLKOUT_EN                               BIT(6)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_DIV1_EN                                 BIT(5)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_DIV2_EN                                 BIT(4)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_DIV3_EN                                 BIT(3)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_DIV5_EN                                 BIT(2)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_DIV7_EN                                 BIT(1)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_LOCK_DONE                               BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_CTRL1
// Register Offset : 0x0040
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_NINT(x)                                 (((x) & 0x7F) << 23)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_KINT(x)                                 (((x) & 0x7FFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_CTRL2
// Register Offset : 0x0044
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_CCS_CTRL(x)                             (((x) & 0xFF) << 3)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_IL_DIV2                                 BIT(2)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_OL_DIV2                                 BIT(1)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_TEST_EN                                 BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_BIST_CTRL
// Register Offset : 0x0048
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_BIST_EN                                 BIT(16)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_CPPLL_BIST_CNT(x)                             (((x) & 0xFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_THM_CTRL_0
// Register Offset : 0x004C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_THM_RUN                                       BIT(20)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_THM_RESERVED(x)                               (((x) & 0xFF) << 12)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_THM_BG_RBIAS_MODE                             BIT(11)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_THM_TEST_SEL(x)                               (((x) & 0x3) << 9)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_THM_BP_MODE                                   BIT(8)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_THM_BP_DATA(x)                                (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_THM_CTRL_1
// Register Offset : 0x0050
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_THM_DATA(x)                                   (((x) & 0xFF) << 1)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_THM_VALID                                     BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_ANA_BB_RSVD
// Register Offset : 0x0054
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_ANA_BB_RESERVED(x)                            (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_TEST_CLK_CTRL
// Register Offset : 0x0058
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TEST_PLL_THM_MUX(x)                           (((x) & 0x1F) << 2)
#define BIT_ANLG_PHY_G1_ANALOG_BB_TOP_TEST_CLK_DIV(x)                               (((x) & 0x3))

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_REG_SEL_CFG_0
// Register Offset : 0x005C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_DPLL_PD                               BIT(31)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_MPLL_PD                               BIT(30)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_TWPLL_PD                              BIT(29)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_CPPLL_PD                              BIT(28)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_THM_PD                                BIT(27)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_DPLL_RST                              BIT(26)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_MPLL_RST                              BIT(25)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_TWPLL_RST                             BIT(24)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_CPPLL_RST                             BIT(23)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_THM_RSTN                              BIT(22)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_ANALOG_PLL_RESERVED                   BIT(21)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_DPLL_REF_SEL                          BIT(20)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_DPLL_N                                BIT(19)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_DPLL_IBIAS                            BIT(18)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_DPLL_SDM_EN                           BIT(17)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_DPLL_MOD_EN                           BIT(16)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_DPLL_DIV_S                            BIT(15)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_DPLL_RESERVED                         BIT(14)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_DPLL_CLKOUT_EN                        BIT(13)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_DPLL_NINT                             BIT(12)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_DPLL_KINT                             BIT(11)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_DPLL_CCS_CTRL                         BIT(10)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_DPLL_IL_DIV2                          BIT(9)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_DPLL_OL_DIV2                          BIT(8)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_DPLL_TEST_EN                          BIT(7)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_DPLL_BIST_EN                          BIT(6)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_MPLL_REF_SEL                          BIT(5)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_MPLL_N                                BIT(4)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_MPLL_IBIAS                            BIT(3)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_MPLL_SDM_EN                           BIT(2)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_MPLL_MOD_EN                           BIT(1)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_MPLL_DIV_S                            BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_REG_SEL_CFG_1
// Register Offset : 0x0060
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_MPLL_RESERVED                         BIT(31)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_MPLL_CLKOUT_EN                        BIT(30)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_MPLL_NINT                             BIT(29)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_MPLL_KINT                             BIT(28)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_MPLL_CCS_CTRL                         BIT(27)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_MPLL_IL_DIV2                          BIT(26)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_MPLL_OL_DIV2                          BIT(25)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_MPLL_TEST_EN                          BIT(24)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_MPLL_BIST_EN                          BIT(23)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_TWPLL_REF_SEL                         BIT(22)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_TWPLL_N                               BIT(21)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_TWPLL_IBIAS                           BIT(20)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_TWPLL_SDM_EN                          BIT(19)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_TWPLL_MOD_EN                          BIT(18)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_TWPLL_DIV_S                           BIT(17)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_TWPLL_RESERVED                        BIT(16)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_TWPLL_CLKOUT_EN                       BIT(15)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_TWPLL_DIV1_EN                         BIT(14)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_TWPLL_DIV2_EN                         BIT(13)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_TWPLL_DIV3_EN                         BIT(12)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_TWPLL_DIV5_EN                         BIT(11)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_TWPLL_DIV7_EN                         BIT(10)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_TWPLL_NINT                            BIT(9)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_TWPLL_KINT                            BIT(8)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_TWPLL_CCS_CTRL                        BIT(7)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_TWPLL_IL_DIV2                         BIT(6)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_TWPLL_OL_DIV2                         BIT(5)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_TWPLL_TEST_EN                         BIT(4)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_TWPLL_BIST_EN                         BIT(3)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_CPPLL_REF_SEL                         BIT(2)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_CPPLL_N                               BIT(1)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_CPPLL_IBIAS                           BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_ANLG_PHY_G1_ANALOG_BB_TOP_REG_SEL_CFG_2
// Register Offset : 0x0064
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_CPPLL_SDM_EN                          BIT(24)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_CPPLL_MOD_EN                          BIT(23)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_CPPLL_DIV_S                           BIT(22)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_CPPLL_RESERVED                        BIT(21)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_CPPLL_CLKOUT_EN                       BIT(20)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_CPPLL_DIV1_EN                         BIT(19)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_CPPLL_DIV2_EN                         BIT(18)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_CPPLL_DIV3_EN                         BIT(17)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_CPPLL_DIV5_EN                         BIT(16)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_CPPLL_DIV7_EN                         BIT(15)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_CPPLL_NINT                            BIT(14)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_CPPLL_KINT                            BIT(13)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_CPPLL_CCS_CTRL                        BIT(12)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_CPPLL_IL_DIV2                         BIT(11)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_CPPLL_OL_DIV2                         BIT(10)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_CPPLL_TEST_EN                         BIT(9)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_CPPLL_BIST_EN                         BIT(8)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_THM_RUN                               BIT(7)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_THM_RESERVED                          BIT(6)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_THM_BG_RBIAS_MODE                     BIT(5)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_THM_TEST_SEL                          BIT(4)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_THM_BP_MODE                           BIT(3)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_THM_BP_DATA                           BIT(2)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_ANA_BB_RESERVED                       BIT(1)
#define BIT_ANLG_PHY_G1_DBG_SEL_ANALOG_BB_TOP_TEST_PLL_THM_MUX                      BIT(0)


#endif // _ANLG_PHY_G1_REG_H
