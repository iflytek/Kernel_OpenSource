/*
 * Copyright (C) 2017 Spreadtrum Communications Inc.
 *
 * This file is dual-licensed: you can use it either under the terms
 * of the GPL or the X11 license, at your option. Note that this dual
 * licensing only applies to this file, and not this project as a
 * whole.
 *
 */


#ifndef _PMU_APB_REG_H
#define _PMU_APB_REG_H

#define CTL_BASE_PMU_APB 0x402B0000


#define REG_PMU_APB_PD_CA7_TOP_CFG                         ( CTL_BASE_PMU_APB + 0x0000 )
#define REG_PMU_APB_PD_CA7_C0_CFG                          ( CTL_BASE_PMU_APB + 0x0004 )
#define REG_PMU_APB_PD_CA7_C1_CFG                          ( CTL_BASE_PMU_APB + 0x0008 )
#define REG_PMU_APB_PD_CA7_C2_CFG                          ( CTL_BASE_PMU_APB + 0x000C )
#define REG_PMU_APB_PD_CA7_C3_CFG                          ( CTL_BASE_PMU_APB + 0x0010 )
#define REG_PMU_APB_PD_AP_SYS_CFG                          ( CTL_BASE_PMU_APB + 0x0018 )
#define REG_PMU_APB_PD_MM_TOP_CFG                          ( CTL_BASE_PMU_APB + 0x001C )
#define REG_PMU_APB_PD_GPU_TOP_CFG                         ( CTL_BASE_PMU_APB + 0x0020 )
#define REG_PMU_APB_PD_WTLCP_TGDSP_CFG                     ( CTL_BASE_PMU_APB + 0x0038 )
#define REG_PMU_APB_PD_WTLCP_HU3GE_A_CFG                   ( CTL_BASE_PMU_APB + 0x003C )
#define REG_PMU_APB_PD_CP_SYS_CFG                          ( CTL_BASE_PMU_APB + 0x0048 )
#define REG_PMU_APB_CP_FRC_STOP_REQ_FOR_WTL                ( CTL_BASE_PMU_APB + 0x004C )
#define REG_PMU_APB_PD_WCN_TOP_CFG                         ( CTL_BASE_PMU_APB + 0x0050 )
#define REG_PMU_APB_PD_WCN_WIFI_CFG                        ( CTL_BASE_PMU_APB + 0x0054 )
#define REG_PMU_APB_PD_WCN_GNSS_CFG                        ( CTL_BASE_PMU_APB + 0x0058 )
#define REG_PMU_APB_PD_PUB_SYS_CFG                         ( CTL_BASE_PMU_APB + 0x005C )
#define REG_PMU_APB_XTL_WAIT_CNT                           ( CTL_BASE_PMU_APB + 0x0070 )
#define REG_PMU_APB_XTLBUF_WAIT_CNT                        ( CTL_BASE_PMU_APB + 0x0074 )
#define REG_PMU_APB_PLL_WAIT_CNT0                          ( CTL_BASE_PMU_APB + 0x0078 )
#define REG_PMU_APB_PLL_WAIT_CNT1                          ( CTL_BASE_PMU_APB + 0x007C )
#define REG_PMU_APB_XTL0_REL_CFG                           ( CTL_BASE_PMU_APB + 0x0080 )
#define REG_PMU_APB_CPLL_REL_CFG                           ( CTL_BASE_PMU_APB + 0x0088 )
#define REG_PMU_APB_XTLBUF0_REL_CFG                        ( CTL_BASE_PMU_APB + 0x008C )
#define REG_PMU_APB_GPLL_REL_CFG                           ( CTL_BASE_PMU_APB + 0x0090 )
#define REG_PMU_APB_MPLL_REL_CFG                           ( CTL_BASE_PMU_APB + 0x0094 )
#define REG_PMU_APB_DPLL_REL_CFG                           ( CTL_BASE_PMU_APB + 0x0098 )
#define REG_PMU_APB_TWPLL_REL_CFG                          ( CTL_BASE_PMU_APB + 0x00A0 )
#define REG_PMU_APB_PLL_FRC_OFF                            ( CTL_BASE_PMU_APB + 0x00A4 )
#define REG_PMU_APB_PLL_FRC_ON                             ( CTL_BASE_PMU_APB + 0x00A8 )
#define REG_PMU_APB_PUB_LSLP_CFG                           ( CTL_BASE_PMU_APB + 0x00AC )
#define REG_PMU_APB_SOFT_RST                               ( CTL_BASE_PMU_APB + 0x00B0 )
#define REG_PMU_APB_CP_SLP_STATUS_DBG0                     ( CTL_BASE_PMU_APB + 0x00B4 )
#define REG_PMU_APB_PWR_STATUS0_DBG                        ( CTL_BASE_PMU_APB + 0x00BC )
#define REG_PMU_APB_PWR_STATUS1_DBG                        ( CTL_BASE_PMU_APB + 0x00C0 )
#define REG_PMU_APB_PWR_STATUS2_DBG                        ( CTL_BASE_PMU_APB + 0x00C4 )
#define REG_PMU_APB_SLEEP_CTRL                             ( CTL_BASE_PMU_APB + 0x00CC )
#define REG_PMU_APB_DDR_SLEEP_CTRL                         ( CTL_BASE_PMU_APB + 0x00D0 )
#define REG_PMU_APB_SLEEP_STATUS                           ( CTL_BASE_PMU_APB + 0x00D4 )
#define REG_PMU_APB_CA7_TOP_CFG                            ( CTL_BASE_PMU_APB + 0x00E4 )
#define REG_PMU_APB_CA7_C0_CFG                             ( CTL_BASE_PMU_APB + 0x00E8 )
#define REG_PMU_APB_CA7_C1_CFG                             ( CTL_BASE_PMU_APB + 0x00EC )
#define REG_PMU_APB_CA7_C2_CFG                             ( CTL_BASE_PMU_APB + 0x00F0 )
#define REG_PMU_APB_CA7_C3_CFG                             ( CTL_BASE_PMU_APB + 0x00F4 )
#define REG_PMU_APB_DDR_CHN_SLEEP_CTRL0                    ( CTL_BASE_PMU_APB + 0x00F8 )
#define REG_PMU_APB_DDR_OP_MODE_CFG                        ( CTL_BASE_PMU_APB + 0x012C )
#define REG_PMU_APB_DDR_PHY_RET_CFG                        ( CTL_BASE_PMU_APB + 0x0130 )
#define REG_PMU_APB_BISR_DONE_STATUS                       ( CTL_BASE_PMU_APB + 0x0138 )
#define REG_PMU_APB_BISR_BUSY_STATUS                       ( CTL_BASE_PMU_APB + 0x013C )
#define REG_PMU_APB_BISR_BYP_CFG                           ( CTL_BASE_PMU_APB + 0x0140 )
#define REG_PMU_APB_BISR_EN_CFG                            ( CTL_BASE_PMU_APB + 0x0144 )
#define REG_PMU_APB_CGM_AUTO_GATE_SEL_CFG0                 ( CTL_BASE_PMU_APB + 0x0148 )
#define REG_PMU_APB_CGM_AUTO_GATE_SEL_CFG1                 ( CTL_BASE_PMU_APB + 0x014C )
#define REG_PMU_APB_CGM_AUTO_GATE_SEL_CFG2                 ( CTL_BASE_PMU_APB + 0x0150 )
#define REG_PMU_APB_CGM_AUTO_GATE_SEL_CFG3                 ( CTL_BASE_PMU_APB + 0x0154 )
#define REG_PMU_APB_CGM_FORCE_EN_CFG0                      ( CTL_BASE_PMU_APB + 0x0158 )
#define REG_PMU_APB_CGM_FORCE_EN_CFG1                      ( CTL_BASE_PMU_APB + 0x015C )
#define REG_PMU_APB_CGM_FORCE_EN_CFG2                      ( CTL_BASE_PMU_APB + 0x0160 )
#define REG_PMU_APB_CGM_FORCE_EN_CFG3                      ( CTL_BASE_PMU_APB + 0x0164 )
#define REG_PMU_APB_SLEEP_XTLON_CTRL                       ( CTL_BASE_PMU_APB + 0x0168 )
#define REG_PMU_APB_MEM_SLP_CFG                            ( CTL_BASE_PMU_APB + 0x016C )
#define REG_PMU_APB_MEM_SD_CFG                             ( CTL_BASE_PMU_APB + 0x0170 )
#define REG_PMU_APB_CA7_CORE_PU_LOCK                       ( CTL_BASE_PMU_APB + 0x0174 )
#define REG_PMU_APB_PWR_CNT_WAIT_CFG0                      ( CTL_BASE_PMU_APB + 0x017C )
#define REG_PMU_APB_PWR_CNT_WAIT_CFG1                      ( CTL_BASE_PMU_APB + 0x0180 )
#define REG_PMU_APB_MEM_AUTO_SLP_CFG                       ( CTL_BASE_PMU_APB + 0x0190 )
#define REG_PMU_APB_MEM_AUTO_SD_CFG                        ( CTL_BASE_PMU_APB + 0x0194 )
#define REG_PMU_APB_WAKEUP_LOCK_EN                         ( CTL_BASE_PMU_APB + 0x01A0 )
#define REG_PMU_APB_WTLCP_TGDSP_CORE_INT_DISABLE           ( CTL_BASE_PMU_APB + 0x01B0 )
#define REG_PMU_APB_CP_CORE_INT_DISABLE                    ( CTL_BASE_PMU_APB + 0x01B8 )
#define REG_PMU_APB_CA7_C0_CORE_INT_DISABLE                ( CTL_BASE_PMU_APB + 0x01BC )
#define REG_PMU_APB_CA7_C1_CORE_INT_DISABLE                ( CTL_BASE_PMU_APB + 0x01C0 )
#define REG_PMU_APB_CA7_C2_CORE_INT_DISABLE                ( CTL_BASE_PMU_APB + 0x01C4 )
#define REG_PMU_APB_CA7_C3_CORE_INT_DISABLE                ( CTL_BASE_PMU_APB + 0x01C8 )
#define REG_PMU_APB_AON_CM4_INT_DISABLE                    ( CTL_BASE_PMU_APB + 0x01CC )
#define REG_PMU_APB_WTLCP_TGDSP_DSLP_ENA                   ( CTL_BASE_PMU_APB + 0x0200 )
#define REG_PMU_APB_AP_DSLP_ENA                            ( CTL_BASE_PMU_APB + 0x0208 )
#define REG_PMU_APB_CP_DSLP_ENA                            ( CTL_BASE_PMU_APB + 0x020C )
#define REG_PMU_APB_CA7_TOP_DSLP_ENA                       ( CTL_BASE_PMU_APB + 0x0214 )
#define REG_PMU_APB_CM4_DSLP_ENA                           ( CTL_BASE_PMU_APB + 0x0218 )
#define REG_PMU_APB_PUBCP_INT_DISABLE                      ( CTL_BASE_PMU_APB + 0x0220 )
#define REG_PMU_APB_WTLCP_INT_DISABLE                      ( CTL_BASE_PMU_APB + 0x0224 )
#define REG_PMU_APB_LIGHT_SLEEP_ENABLE                     ( CTL_BASE_PMU_APB + 0x0230 )
#define REG_PMU_APB_CM4_LIGHT_SLEEP                        ( CTL_BASE_PMU_APB + 0x0234 )
#define REG_PMU_APB_SYS_DOZE_DSLP_ENA                      ( CTL_BASE_PMU_APB + 0x0240 )
#define REG_PMU_APB_PUB_ACC_RDY                            ( CTL_BASE_PMU_APB + 0x0250 )
#define REG_PMU_APB_PUB_CLK_RDY                            ( CTL_BASE_PMU_APB + 0x0254 )
#define REG_PMU_APB_EIC_SEL                                ( CTL_BASE_PMU_APB + 0x0258 )
#define REG_PMU_APB_AXI_LP_CTRL_DISABLE                    ( CTL_BASE_PMU_APB + 0x0260 )
#define REG_PMU_APB_PMU_DEBUG                              ( CTL_BASE_PMU_APB + 0x0270 )
#define REG_PMU_APB_SLEEP_CNT_CLR                          ( CTL_BASE_PMU_APB + 0x0274 )
#define REG_PMU_APB_PAD_OUT_ADIE_CTRL0                     ( CTL_BASE_PMU_APB + 0x0290 )
#define REG_PMU_APB_BISR_FORCE_SEL                         ( CTL_BASE_PMU_APB + 0x0300 )
#define REG_PMU_APB_AON_MEM_CTRL                           ( CTL_BASE_PMU_APB + 0x0330 )
#define REG_PMU_APB_PWR_DOMAIN_INT_CLR                     ( CTL_BASE_PMU_APB + 0x0334 )
#define REG_PMU_APB_WCN_SYS_CFG_STATUS                     ( CTL_BASE_PMU_APB + 0x0338 )
#define REG_PMU_APB_RF_CFG                                 ( CTL_BASE_PMU_APB + 0x033C )
#define REG_PMU_APB_PUB_PLL_CFG                            ( CTL_BASE_PMU_APB + 0x0340 )
#define REG_PMU_APB_BBPLL_REL_CFG                          ( CTL_BASE_PMU_APB + 0x0344 )
#define REG_PMU_APB_CP_CG_CFG                              ( CTL_BASE_PMU_APB + 0x0348 )
#define REG_PMU_APB_WCN_WIFI_DSLP_ENA                      ( CTL_BASE_PMU_APB + 0x034C )
#define REG_PMU_APB_WCN_GNSS_DSLP_ENA                      ( CTL_BASE_PMU_APB + 0x0350 )
#define REG_PMU_APB_EFUSE_SELECT_BUF_CFG                   ( CTL_BASE_PMU_APB + 0x0354 )
#define REG_PMU_APB_EFUSE_SELECT_BUF_CLR                   ( CTL_BASE_PMU_APB + 0x0358 )
#define REG_PMU_APB_EFUSE_SELECT_BUF_STS                   ( CTL_BASE_PMU_APB + 0x035C )
#define REG_PMU_APB_PMU_CLK_CFG                            ( CTL_BASE_PMU_APB + 0x0360 )
#define REG_PMU_APB_PWR_ST_DLY0                            ( CTL_BASE_PMU_APB + 0x0364 )
#define REG_PMU_APB_PWR_ST_DLY1                            ( CTL_BASE_PMU_APB + 0x0368 )
#define REG_PMU_APB_PMU_CLK_DIV_CFG0                       ( CTL_BASE_PMU_APB + 0x036C )
#define REG_PMU_APB_PMU_CLK_DIV_CFG1                       ( CTL_BASE_PMU_APB + 0x0370 )
#define REG_PMU_APB_PUB_DSLP_CFG0                          ( CTL_BASE_PMU_APB + 0x0374 )
#define REG_PMU_APB_PUB_DSLP_CFG1                          ( CTL_BASE_PMU_APB + 0x0378 )
#define REG_PMU_APB_PUB_DSLP_EN                            ( CTL_BASE_PMU_APB + 0x037C )
#define REG_PMU_APB_PLL_CNT_DONE                           ( CTL_BASE_PMU_APB + 0x0380 )
#define REG_PMU_APB_CGM_XTL_EN                             ( CTL_BASE_PMU_APB + 0x0384 )
#define REG_PMU_APB_CGM_XTL_EN_SEL                         ( CTL_BASE_PMU_APB + 0x0388 )
#define REG_PMU_APB_CA7_RST_CFG                            ( CTL_BASE_PMU_APB + 0x038C )
#define REG_PMU_APB_PD_SYS_ACK                             ( CTL_BASE_PMU_APB + 0x0390 )
#define REG_PMU_APB_PD_CA7_C0_SHUTDOWN_MARK_STATUS         ( CTL_BASE_PMU_APB + 0x3000 )
#define REG_PMU_APB_PD_CA7_C1_SHUTDOWN_MARK_STATUS         ( CTL_BASE_PMU_APB + 0x3004 )
#define REG_PMU_APB_PD_CA7_C2_SHUTDOWN_MARK_STATUS         ( CTL_BASE_PMU_APB + 0x3008 )
#define REG_PMU_APB_PD_CA7_C3_SHUTDOWN_MARK_STATUS         ( CTL_BASE_PMU_APB + 0x300C )
#define REG_PMU_APB_PD_CA7_TOP_SHUTDOWN_MARK_STATUS        ( CTL_BASE_PMU_APB + 0x3010 )
#define REG_PMU_APB_PD_AP_SYS_SHUTDOWN_MARK_STATUS         ( CTL_BASE_PMU_APB + 0x3014 )
#define REG_PMU_APB_PD_GPU_TOP_SHUTDOWN_MARK_STATUS        ( CTL_BASE_PMU_APB + 0x3018 )
#define REG_PMU_APB_PD_MM_TOP_SHUTDOWN_MARK_STATUS         ( CTL_BASE_PMU_APB + 0x301C )
#define REG_PMU_APB_PD_WTLCP_TGDSP_SHUTDOWN_MARK_STATUS    ( CTL_BASE_PMU_APB + 0x3034 )
#define REG_PMU_APB_PD_WTLCP_HU3GE_A_SHUTDOWN_MARK_STATUS  ( CTL_BASE_PMU_APB + 0x3038 )
#define REG_PMU_APB_PD_CP_SYS_SHUTDOWN_MARK_STATUS         ( CTL_BASE_PMU_APB + 0x3044 )
#define REG_PMU_APB_PD_PUB_SYS_SHUTDOWN_MARK_STATUS        ( CTL_BASE_PMU_APB + 0x3054 )
#define REG_PMU_APB_PD_WCN_TOP_SHUTDOWN_MARK_STATUS        ( CTL_BASE_PMU_APB + 0x3058 )
#define REG_PMU_APB_PD_WCN_WIFI_SHUTDOWN_MARK_STATUS       ( CTL_BASE_PMU_APB + 0x305C )
#define REG_PMU_APB_PD_WCN_GNSS_SHUTDOWN_MARK_STATUS       ( CTL_BASE_PMU_APB + 0x3060 )
#define REG_PMU_APB_PMU_DUMMY1                             ( CTL_BASE_PMU_APB + 0x3064 )
#define REG_PMU_APB_PMU_DUMMY2                             ( CTL_BASE_PMU_APB + 0x3068 )
#define REG_PMU_APB_PMU_DUMMY3                             ( CTL_BASE_PMU_APB + 0x306C )
#define REG_PMU_APB_PMU_DUMMY4                             ( CTL_BASE_PMU_APB + 0x3070 )
#define REG_PMU_APB_PMU_DUMMY5                             ( CTL_BASE_PMU_APB + 0x3074 )
#define REG_PMU_APB_PMU_DUMMY6                             ( CTL_BASE_PMU_APB + 0x3078 )

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_CA7_TOP_CFG
// Register Offset : 0x0000
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_CA7_TOP_DBG_SHUTDOWN_EN                             BIT(28)
#define BIT_PMU_APB_PD_CA7_TOP_PD_SEL                                      BIT(27)
#define BIT_PMU_APB_PD_CA7_TOP_FORCE_SHUTDOWN                              BIT(25)
#define BIT_PMU_APB_PD_CA7_TOP_AUTO_SHUTDOWN_EN                            BIT(24)
#define BIT_PMU_APB_PD_CA7_TOP_PWR_ON_DLY(x)                               (((x) & 0xFF) << 16)
#define BIT_PMU_APB_PD_CA7_TOP_PWR_ON_SEQ_DLY(x)                           (((x) & 0xFF) << 8)
#define BIT_PMU_APB_PD_CA7_TOP_ISO_ON_DLY(x)                               (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_CA7_C0_CFG
// Register Offset : 0x0004
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_CA7_C0_WFI_SHUTDOWN_EN                              BIT(29)
#define BIT_PMU_APB_PD_CA7_C0_DBG_SHUTDOWN_EN                              BIT(28)
#define BIT_PMU_APB_PD_CA7_C0_PD_SEL                                       BIT(27)
#define BIT_PMU_APB_PD_CA7_C0_FORCE_SHUTDOWN                               BIT(25)
#define BIT_PMU_APB_PD_CA7_C0_AUTO_SHUTDOWN_EN                             BIT(24)
#define BIT_PMU_APB_PD_CA7_C0_PWR_ON_DLY(x)                                (((x) & 0xFF) << 16)
#define BIT_PMU_APB_PD_CA7_C0_PWR_ON_SEQ_DLY(x)                            (((x) & 0xFF) << 8)
#define BIT_PMU_APB_PD_CA7_C0_ISO_ON_DLY(x)                                (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_CA7_C1_CFG
// Register Offset : 0x0008
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_CA7_C1_WFI_SHUTDOWN_EN                              BIT(29)
#define BIT_PMU_APB_PD_CA7_C1_DBG_SHUTDOWN_EN                              BIT(28)
#define BIT_PMU_APB_PD_CA7_C1_PD_SEL                                       BIT(27)
#define BIT_PMU_APB_PD_CA7_C1_FORCE_SHUTDOWN                               BIT(25)
#define BIT_PMU_APB_PD_CA7_C1_AUTO_SHUTDOWN_EN                             BIT(24)
#define BIT_PMU_APB_PD_CA7_C1_PWR_ON_DLY(x)                                (((x) & 0xFF) << 16)
#define BIT_PMU_APB_PD_CA7_C1_PWR_ON_SEQ_DLY(x)                            (((x) & 0xFF) << 8)
#define BIT_PMU_APB_PD_CA7_C1_ISO_ON_DLY(x)                                (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_CA7_C2_CFG
// Register Offset : 0x000C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_CA7_C2_WFI_SHUTDOWN_EN                              BIT(29)
#define BIT_PMU_APB_PD_CA7_C2_DBG_SHUTDOWN_EN                              BIT(28)
#define BIT_PMU_APB_PD_CA7_C2_PD_SEL                                       BIT(27)
#define BIT_PMU_APB_PD_CA7_C2_FORCE_SHUTDOWN                               BIT(25)
#define BIT_PMU_APB_PD_CA7_C2_AUTO_SHUTDOWN_EN                             BIT(24)
#define BIT_PMU_APB_PD_CA7_C2_PWR_ON_DLY(x)                                (((x) & 0xFF) << 16)
#define BIT_PMU_APB_PD_CA7_C2_PWR_ON_SEQ_DLY(x)                            (((x) & 0xFF) << 8)
#define BIT_PMU_APB_PD_CA7_C2_ISO_ON_DLY(x)                                (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_CA7_C3_CFG
// Register Offset : 0x0010
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_CA7_C3_WFI_SHUTDOWN_EN                              BIT(29)
#define BIT_PMU_APB_PD_CA7_C3_DBG_SHUTDOWN_EN                              BIT(28)
#define BIT_PMU_APB_PD_CA7_C3_PD_SEL                                       BIT(27)
#define BIT_PMU_APB_PD_CA7_C3_FORCE_SHUTDOWN                               BIT(25)
#define BIT_PMU_APB_PD_CA7_C3_AUTO_SHUTDOWN_EN                             BIT(24)
#define BIT_PMU_APB_PD_CA7_C3_PWR_ON_DLY(x)                                (((x) & 0xFF) << 16)
#define BIT_PMU_APB_PD_CA7_C3_PWR_ON_SEQ_DLY(x)                            (((x) & 0xFF) << 8)
#define BIT_PMU_APB_PD_CA7_C3_ISO_ON_DLY(x)                                (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_AP_SYS_CFG
// Register Offset : 0x0018
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_AP_SYS_FORCE_SHUTDOWN                               BIT(25)
#define BIT_PMU_APB_PD_AP_SYS_AUTO_SHUTDOWN_EN                             BIT(24)
#define BIT_PMU_APB_PD_AP_SYS_PWR_ON_DLY(x)                                (((x) & 0xFF) << 16)
#define BIT_PMU_APB_PD_AP_SYS_PWR_ON_SEQ_DLY(x)                            (((x) & 0xFF) << 8)
#define BIT_PMU_APB_PD_AP_SYS_ISO_ON_DLY(x)                                (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_MM_TOP_CFG
// Register Offset : 0x001C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_MM_TOP_FORCE_SHUTDOWN                               BIT(25)
#define BIT_PMU_APB_PD_MM_TOP_AUTO_SHUTDOWN_EN                             BIT(24)
#define BIT_PMU_APB_PD_MM_TOP_PWR_ON_DLY(x)                                (((x) & 0xFF) << 16)
#define BIT_PMU_APB_PD_MM_TOP_PWR_ON_SEQ_DLY(x)                            (((x) & 0xFF) << 8)
#define BIT_PMU_APB_PD_MM_TOP_ISO_ON_DLY(x)                                (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_GPU_TOP_CFG
// Register Offset : 0x0020
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_GPU_TOP_FORCE_SHUTDOWN                              BIT(25)
#define BIT_PMU_APB_PD_GPU_TOP_AUTO_SHUTDOWN_EN                            BIT(24)
#define BIT_PMU_APB_PD_GPU_TOP_PWR_ON_DLY(x)                               (((x) & 0xFF) << 16)
#define BIT_PMU_APB_PD_GPU_TOP_PWR_ON_SEQ_DLY(x)                           (((x) & 0xFF) << 8)
#define BIT_PMU_APB_PD_GPU_TOP_ISO_ON_DLY(x)                               (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_WTLCP_TGDSP_CFG
// Register Offset : 0x0038
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_WTLCP_TGDSP_PD_SEL                                  BIT(27)
#define BIT_PMU_APB_PD_WTLCP_TGDSP_FORCE_SHUTDOWN                          BIT(25)
#define BIT_PMU_APB_PD_WTLCP_TGDSP_AUTO_SHUTDOWN_EN                        BIT(24)
#define BIT_PMU_APB_PD_WTLCP_TGDSP_PWR_ON_DLY(x)                           (((x) & 0xFF) << 16)
#define BIT_PMU_APB_PD_WTLCP_TGDSP_PWR_ON_SEQ_DLY(x)                       (((x) & 0xFF) << 8)
#define BIT_PMU_APB_PD_WTLCP_TGDSP_ISO_ON_DLY(x)                           (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_WTLCP_HU3GE_A_CFG
// Register Offset : 0x003C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_WTLCP_HU3GE_A_FORCE_SHUTDOWN                        BIT(25)
#define BIT_PMU_APB_PD_WTLCP_HU3GE_A_AUTO_SHUTDOWN_EN                      BIT(24)
#define BIT_PMU_APB_PD_WTLCP_HU3GE_A_PWR_ON_DLY(x)                         (((x) & 0xFF) << 16)
#define BIT_PMU_APB_PD_WTLCP_HU3GE_A_PWR_ON_SEQ_DLY(x)                     (((x) & 0xFF) << 8)
#define BIT_PMU_APB_PD_WTLCP_HU3GE_A_ISO_ON_DLY(x)                         (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_CP_SYS_CFG
// Register Offset : 0x0048
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_PUBCP_SYS_AUTO_SHUTDOWN_EN                          BIT(27)
#define BIT_PMU_APB_PD_CP_SYS_DBG_SHUTDOWN_EN                              BIT(26)
#define BIT_PMU_APB_PD_CP_SYS_FORCE_SHUTDOWN                               BIT(25)
#define BIT_PMU_APB_PD_WTLCP_SYS_AUTO_SHUTDOWN_EN                          BIT(24)
#define BIT_PMU_APB_PD_CP_SYS_PWR_ON_DLY(x)                                (((x) & 0xFF) << 16)
#define BIT_PMU_APB_PD_CP_SYS_PWR_ON_SEQ_DLY(x)                            (((x) & 0xFF) << 8)
#define BIT_PMU_APB_PD_CP_SYS_ISO_ON_DLY(x)                                (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CP_FRC_STOP_REQ_FOR_WTL
// Register Offset : 0x004C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CP_FRC_STOP_REQ_FOR_WTL                                BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_WCN_TOP_CFG
// Register Offset : 0x0050
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_WCN_TOP_FORCE_SHUTDOWN                              BIT(25)
#define BIT_PMU_APB_PD_WCN_TOP_AUTO_SHUTDOWN_EN                            BIT(24)
#define BIT_PMU_APB_PD_WCN_TOP_PWR_ON_DLY(x)                               (((x) & 0xFF) << 16)
#define BIT_PMU_APB_PD_WCN_TOP_PWR_ON_SEQ_DLY(x)                           (((x) & 0xFF) << 8)
#define BIT_PMU_APB_PD_WCN_TOP_ISO_ON_DLY(x)                               (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_WCN_WIFI_CFG
// Register Offset : 0x0054
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_WCN_WIFI_AUTO_SEL                                   BIT(26)
#define BIT_PMU_APB_PD_WCN_WIFI_FORCE_SHUTDOWN                             BIT(25)
#define BIT_PMU_APB_PD_WCN_WIFI_AUTO_SHUTDOWN_EN                           BIT(24)
#define BIT_PMU_APB_PD_WCN_WIFI_PWR_ON_DLY(x)                              (((x) & 0xFF) << 16)
#define BIT_PMU_APB_PD_WCN_WIFI_PWR_ON_SEQ_DLY(x)                          (((x) & 0xFF) << 8)
#define BIT_PMU_APB_PD_WCN_WIFI_ISO_ON_DLY(x)                              (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_WCN_GNSS_CFG
// Register Offset : 0x0058
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_WCN_GNSS_AUTO_SEL                                   BIT(26)
#define BIT_PMU_APB_PD_WCN_GNSS_FORCE_SHUTDOWN                             BIT(25)
#define BIT_PMU_APB_PD_WCN_GNSS_AUTO_SHUTDOWN_EN                           BIT(24)
#define BIT_PMU_APB_PD_WCN_GNSS_PWR_ON_DLY(x)                              (((x) & 0xFF) << 16)
#define BIT_PMU_APB_PD_WCN_GNSS_PWR_ON_SEQ_DLY(x)                          (((x) & 0xFF) << 8)
#define BIT_PMU_APB_PD_WCN_GNSS_ISO_ON_DLY(x)                              (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_PUB_SYS_CFG
// Register Offset : 0x005C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_PUB_SYS_PRE_EN                                      BIT(26)
#define BIT_PMU_APB_PD_PUB_SYS_FORCE_SHUTDOWN                              BIT(25)
#define BIT_PMU_APB_PD_PUB_SYS_AUTO_SHUTDOWN_EN                            BIT(24)
#define BIT_PMU_APB_PD_PUB_SYS_PWR_ON_DLY(x)                               (((x) & 0xFF) << 16)
#define BIT_PMU_APB_PD_PUB_SYS_PWR_ON_SEQ_DLY(x)                           (((x) & 0xFF) << 8)
#define BIT_PMU_APB_PD_PUB_SYS_ISO_ON_DLY(x)                               (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_XTL_WAIT_CNT
// Register Offset : 0x0070
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_XTL0_WAIT_CNT(x)                                       (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_XTLBUF_WAIT_CNT
// Register Offset : 0x0074
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_XTLBUF0_WAIT_CNT(x)                                    (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PLL_WAIT_CNT0
// Register Offset : 0x0078
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CPLL_WAIT_CNT(x)                                       (((x) & 0xFF) << 24)
#define BIT_PMU_APB_TWPLL_WAIT_CNT(x)                                      (((x) & 0xFF) << 16)
#define BIT_PMU_APB_DPLL_WAIT_CNT(x)                                       (((x) & 0xFF) << 8)
#define BIT_PMU_APB_MPLL_WAIT_CNT(x)                                       (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PLL_WAIT_CNT1
// Register Offset : 0x007C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_GPLL_WAIT_CNT(x)                                       (((x) & 0xFF) << 16)
#define BIT_PMU_APB_BBPLL2_WAIT_CNT(x)                                     (((x) & 0xFF) << 8)
#define BIT_PMU_APB_BBPLL1_WAIT_CNT(x)                                     (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_XTL0_REL_CFG
// Register Offset : 0x0080
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_XTL0_RF_DIG_RAM_SEL                                    BIT(8)
#define BIT_PMU_APB_XTL0_CM4_RAM_SEL                                       BIT(7)
#define BIT_PMU_APB_XTL0_AON_RAM_SEL                                       BIT(6)
#define BIT_PMU_APB_XTL0_CM4_SEL                                           BIT(5)
#define BIT_PMU_APB_XTL0_PUBCP_SEL                                         BIT(4)
#define BIT_PMU_APB_XTL0_PUB_SEL                                           BIT(3)
#define BIT_PMU_APB_XTL0_WCN_SEL                                           BIT(2)
#define BIT_PMU_APB_XTL0_WTLCP_SEL                                         BIT(1)
#define BIT_PMU_APB_XTL0_AP_SEL                                            BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CPLL_REL_CFG
// Register Offset : 0x0088
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CPLL_CM4_SEL                                           BIT(5)
#define BIT_PMU_APB_CPLL_PUB_SEL                                           BIT(3)
#define BIT_PMU_APB_CPLL_WCN_SEL                                           BIT(2)
#define BIT_PMU_APB_CPLL_CP_SEL                                            BIT(1)
#define BIT_PMU_APB_CPLL_AP_SEL                                            BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_XTLBUF0_REL_CFG
// Register Offset : 0x008C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_XTLBUF0_RF_DIG_RAM_SEL                                 BIT(8)
#define BIT_PMU_APB_XTLBUF0_CM4_RAM_SEL                                    BIT(7)
#define BIT_PMU_APB_XTLBUF0_AON_RAM_SEL                                    BIT(6)
#define BIT_PMU_APB_XTLBUF0_CM4_SEL                                        BIT(5)
#define BIT_PMU_APB_XTLBUF0_PUBCP_SEL                                      BIT(4)
#define BIT_PMU_APB_XTLBUF0_PUB_SEL                                        BIT(3)
#define BIT_PMU_APB_XTLBUF0_WCN_SEL                                        BIT(2)
#define BIT_PMU_APB_XTLBUF0_WTLCP_SEL                                      BIT(1)
#define BIT_PMU_APB_XTLBUF0_AP_SEL                                         BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_GPLL_REL_CFG
// Register Offset : 0x0090
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_GPLL_AP_SEL                                            BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_MPLL_REL_CFG
// Register Offset : 0x0094
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_MPLL_CM4_SEL                                           BIT(5)
#define BIT_PMU_APB_MPLL_PUB_SEL                                           BIT(3)
#define BIT_PMU_APB_MPLL_AP_SEL                                            BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_DPLL_REL_CFG
// Register Offset : 0x0098
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_DPLL_CM4_SEL                                           BIT(5)
#define BIT_PMU_APB_DPLL_PUB_SEL                                           BIT(3)
#define BIT_PMU_APB_DPLL_CP_SEL                                            BIT(1)
#define BIT_PMU_APB_DPLL_AP_SEL                                            BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_TWPLL_REL_CFG
// Register Offset : 0x00A0
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_TWPLL_CM4_SEL                                          BIT(5)
#define BIT_PMU_APB_TWPLL_PUBCP_SEL                                        BIT(4)
#define BIT_PMU_APB_TWPLL_PUB_SEL                                          BIT(3)
#define BIT_PMU_APB_TWPLL_WCN_SEL                                          BIT(2)
#define BIT_PMU_APB_TWPLL_WTLCP_SEL                                        BIT(1)
#define BIT_PMU_APB_TWPLL_AP_SEL                                           BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PLL_FRC_OFF
// Register Offset : 0x00A4
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_GPLL_FRC_OFF                                           BIT(7)
#define BIT_PMU_APB_XTLBUF0_FRC_OFF                                        BIT(6)
#define BIT_PMU_APB_BBPLL2_FRC_OFF                                         BIT(5)
#define BIT_PMU_APB_BBPLL1_FRC_OFF                                         BIT(4)
#define BIT_PMU_APB_CPLL_FRC_OFF                                           BIT(3)
#define BIT_PMU_APB_DPLL_FRC_OFF                                           BIT(2)
#define BIT_PMU_APB_TWPLL_FRC_OFF                                          BIT(1)
#define BIT_PMU_APB_MPLL_FRC_OFF                                           BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PLL_FRC_ON
// Register Offset : 0x00A8
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_GPLL_FRC_ON                                            BIT(7)
#define BIT_PMU_APB_XTLBUF0_FRC_ON                                         BIT(6)
#define BIT_PMU_APB_BBPLL2_FRC_ON                                          BIT(5)
#define BIT_PMU_APB_BBPLL1_FRC_ON                                          BIT(4)
#define BIT_PMU_APB_CPLL_FRC_ON                                            BIT(3)
#define BIT_PMU_APB_DPLL_FRC_ON                                            BIT(2)
#define BIT_PMU_APB_TWPLL_FRC_ON                                           BIT(1)
#define BIT_PMU_APB_MPLL_FRC_ON                                            BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PUB_LSLP_CFG
// Register Offset : 0x00AC
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PLL_AUTO_PD_EN                                         BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_SOFT_RST
// Register Offset : 0x00B0
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CM4_SOFT_RST                                           BIT(8)
#define BIT_PMU_APB_WCN_SOFT_RST                                           BIT(7)
#define BIT_PMU_APB_PUB_SOFT_RST                                           BIT(6)
#define BIT_PMU_APB_AP_SOFT_RST                                            BIT(5)
#define BIT_PMU_APB_GPU_SOFT_RST                                           BIT(4)
#define BIT_PMU_APB_MM_SOFT_RST                                            BIT(3)
#define BIT_PMU_APB_WTLCP_DSP_SYS_SRST                                     BIT(2)
#define BIT_PMU_APB_CP_SOFT_RST                                            BIT(1)
#define BIT_PMU_APB_WTLCP_SOFT_RST                                         BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CP_SLP_STATUS_DBG0
// Register Offset : 0x00B4
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CP_DEEP_SLP_DBG(x)                                     (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PWR_STATUS0_DBG
// Register Offset : 0x00BC
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_MM_TOP_STATE(x)                                     (((x) & 0xF) << 28)
#define BIT_PMU_APB_PD_GPU_TOP_STATE(x)                                    (((x) & 0xF) << 24)
#define BIT_PMU_APB_PD_AP_SYS_STATE(x)                                     (((x) & 0xF) << 20)
#define BIT_PMU_APB_PD_CA7_C3_STATE(x)                                     (((x) & 0xF) << 16)
#define BIT_PMU_APB_PD_CA7_C2_STATE(x)                                     (((x) & 0xF) << 12)
#define BIT_PMU_APB_PD_CA7_C1_STATE(x)                                     (((x) & 0xF) << 8)
#define BIT_PMU_APB_PD_CA7_C0_STATE(x)                                     (((x) & 0xF) << 4)
#define BIT_PMU_APB_PD_CA7_TOP_STATE(x)                                    (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PWR_STATUS1_DBG
// Register Offset : 0x00C0
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_WCN_GNSS_STATE(x)                                   (((x) & 0xF) << 24)
#define BIT_PMU_APB_PD_WCN_WIFI_STATE(x)                                   (((x) & 0xF) << 20)
#define BIT_PMU_APB_PD_WCN_TOP_STATE(x)                                    (((x) & 0xF) << 16)
#define BIT_PMU_APB_PD_PUB_SYS_STATE(x)                                    (((x) & 0xF) << 12)
#define BIT_PMU_APB_PD_WTLCP_TGDSP_STATE(x)                                (((x) & 0xF) << 8)
#define BIT_PMU_APB_PD_WTLCP_HU3GE_A_STATE(x)                              (((x) & 0xF) << 4)
#define BIT_PMU_APB_PD_CP_SYS_STATE(x)                                     (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PWR_STATUS2_DBG
// Register Offset : 0x00C4
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_WCN_GNSS_STATE_4                                    BIT(14)
#define BIT_PMU_APB_PD_WCN_WIFI_STATE_4                                    BIT(13)
#define BIT_PMU_APB_PD_WCN_TOP_STATE_4                                     BIT(12)
#define BIT_PMU_APB_PD_PUB_SYS_STATE_4                                     BIT(11)
#define BIT_PMU_APB_PD_WTLCP_TGDSP_STATE_4                                 BIT(10)
#define BIT_PMU_APB_PD_WTLCP_HU3GE_A_STATE_4                               BIT(9)
#define BIT_PMU_APB_PD_CP_SYS_STATE_4                                      BIT(8)
#define BIT_PMU_APB_PD_MM_TOP_STATE_4                                      BIT(7)
#define BIT_PMU_APB_PD_GPU_TOP_STATE_4                                     BIT(6)
#define BIT_PMU_APB_PD_AP_SYS_STATE_4                                      BIT(5)
#define BIT_PMU_APB_PD_CA7_C3_STATE_4                                      BIT(4)
#define BIT_PMU_APB_PD_CA7_C2_STATE_4                                      BIT(3)
#define BIT_PMU_APB_PD_CA7_C1_STATE_4                                      BIT(2)
#define BIT_PMU_APB_PD_CA7_C0_STATE_4                                      BIT(1)
#define BIT_PMU_APB_PD_CA7_TOP_STATE_4                                     BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_SLEEP_CTRL
// Register Offset : 0x00CC
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CM4_FORCE_DEEP_SLEEP                                   BIT(21)
#define BIT_PMU_APB_CP_FORCE_DEEP_SLEEP                                    BIT(18)
#define BIT_PMU_APB_AP_FORCE_DEEP_SLEEP                                    BIT(16)
#define BIT_PMU_APB_PD_WCN_GNSS_DEEP_SLEEP                                 BIT(8)
#define BIT_PMU_APB_PD_WCN_WIFI_DEEP_SLEEP                                 BIT(7)
#define BIT_PMU_APB_CP_DEEP_SLEEP                                          BIT(2)
#define BIT_PMU_APB_AP_DEEP_SLEEP                                          BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_DDR_SLEEP_CTRL
// Register Offset : 0x00D0
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_BUSY_TRANSFER_HWDATA_SEL                               BIT(16)
#define BIT_PMU_APB_DDR_PUBL_APB_SOFT_RST                                  BIT(12)
#define BIT_PMU_APB_DDR_UMCTL_APB_SOFT_RST                                 BIT(11)
#define BIT_PMU_APB_DDR_PUBL_SOFT_RST                                      BIT(10)
#define BIT_PMU_APB_DDR_PHY_SOFT_RST                                       BIT(8)
#define BIT_PMU_APB_DDR_PHY_AUTO_GATE_EN                                   BIT(6)
#define BIT_PMU_APB_DDR_PUBL_AUTO_GATE_EN                                  BIT(5)
#define BIT_PMU_APB_DDR_UMCTL_AUTO_GATE_EN                                 BIT(4)
#define BIT_PMU_APB_DDR_PHY_EB                                             BIT(2)
#define BIT_PMU_APB_DDR_UMCTL_EB                                           BIT(1)
#define BIT_PMU_APB_DDR_PUBL_EB                                            BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_SLEEP_STATUS
// Register Offset : 0x00D4
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CM4_SLP_STATUS(x)                                      (((x) & 0xF) << 20)
#define BIT_PMU_APB_WCN_SLP_STATUS(x)                                      (((x) & 0xF) << 12)
#define BIT_PMU_APB_CP_SLP_STATUS(x)                                       (((x) & 0xF) << 8)
#define BIT_PMU_APB_AP_SLP_STATUS(x)                                       (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CA7_TOP_CFG
// Register Offset : 0x00E4
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CA7_L2RSTDISABLE                                       BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CA7_C0_CFG
// Register Offset : 0x00E8
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CA7_VINITHI_C0                                         BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CA7_C1_CFG
// Register Offset : 0x00EC
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CA7_VINITHI_C1                                         BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CA7_C2_CFG
// Register Offset : 0x00F0
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CA7_VINITHI_C2                                         BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CA7_C3_CFG
// Register Offset : 0x00F4
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CA7_VINITHI_C3                                         BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_DDR_CHN_SLEEP_CTRL0
// Register Offset : 0x00F8
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_DDR_CTRL_AXI_LP_EN                                     BIT(31)
#define BIT_PMU_APB_DDR_CTRL_CGM_SEL                                       BIT(30)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_DDR_OP_MODE_CFG
// Register Offset : 0x012C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_DDR_OPERATE_MODE_BUSY                                  BIT(28)
#define BIT_PMU_APB_DDR_PUBL_RET_EN                                        BIT(27)
#define BIT_PMU_APB_DDR_PHY_ISO_RST_EN                                     BIT(26)
#define BIT_PMU_APB_DDR_UMCTL_RET_EN                                       BIT(25)
#define BIT_PMU_APB_DDR_PHY_AUTO_RET_EN                                    BIT(24)
#define BIT_PMU_APB_DDR_OPERATE_MODE_CNT_LMT(x)                            (((x) & 0xFF) << 16)
#define BIT_PMU_APB_DDR_OPERATE_MODE(x)                                    (((x) & 0x7) << 8)
#define BIT_PMU_APB_DDR_OPERATE_MODE_IDLE(x)                               (((x) & 0x7))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_DDR_PHY_RET_CFG
// Register Offset : 0x0130
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_DDR_UMCTL_SOFT_RST                                     BIT(16)
#define BIT_PMU_APB_DDR_PHY_CKE_RET_EN                                     BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_BISR_DONE_STATUS
// Register Offset : 0x0138
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_AON_MEM_BISR_DONE                                   BIT(18)
#define BIT_PMU_APB_PD_CP_SYS_BISR_DONE                                    BIT(17)
#define BIT_PMU_APB_PD_WTLCP_HU3GE_A_BISR_DONE                             BIT(14)
#define BIT_PMU_APB_PD_WTLCP_TGDSP_BISR_DONE                               BIT(13)
#define BIT_PMU_APB_PD_PUB_TOP_BISR_DONE                                   BIT(11)
#define BIT_PMU_APB_PD_WCN_TOP_BISR_DONE                                   BIT(10)
#define BIT_PMU_APB_PD_WCN_WIFI_BISR_DONE                                  BIT(9)
#define BIT_PMU_APB_PD_WCN_GNSS_BISR_DONE                                  BIT(8)
#define BIT_PMU_APB_PD_MM_TOP_BISR_DONE                                    BIT(7)
#define BIT_PMU_APB_PD_GPU_TOP_BISR_DONE                                   BIT(6)
#define BIT_PMU_APB_PD_AP_SYS_BISR_DONE                                    BIT(5)
#define BIT_PMU_APB_PD_CA7_TOP_BISR_DONE                                   BIT(4)
#define BIT_PMU_APB_PD_CA7_C3_BISR_DONE                                    BIT(3)
#define BIT_PMU_APB_PD_CA7_C2_BISR_DONE                                    BIT(2)
#define BIT_PMU_APB_PD_CA7_C1_BISR_DONE                                    BIT(1)
#define BIT_PMU_APB_PD_CA7_C0_BISR_DONE                                    BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_BISR_BUSY_STATUS
// Register Offset : 0x013C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_AON_MEM_BISR_BUSY                                   BIT(18)
#define BIT_PMU_APB_PD_CP_SYS_BISR_BUSY                                    BIT(17)
#define BIT_PMU_APB_PD_WTLCP_HU3GE_A_BISR_BUSY                             BIT(14)
#define BIT_PMU_APB_PD_WTLCP_TGDSP_BISR_BUSY                               BIT(13)
#define BIT_PMU_APB_PD_PUB_TOP_BISR_BUSY                                   BIT(11)
#define BIT_PMU_APB_PD_WCN_TOP_BISR_BUSY                                   BIT(10)
#define BIT_PMU_APB_PD_WCN_WIFI_BISR_BUSY                                  BIT(9)
#define BIT_PMU_APB_PD_WCN_GNSS_BISR_BUSY                                  BIT(8)
#define BIT_PMU_APB_PD_MM_TOP_BISR_BUSY                                    BIT(7)
#define BIT_PMU_APB_PD_GPU_TOP_BISR_BUSY                                   BIT(6)
#define BIT_PMU_APB_PD_AP_SYS_BISR_BUSY                                    BIT(5)
#define BIT_PMU_APB_PD_CA7_TOP_BISR_BUSY                                   BIT(4)
#define BIT_PMU_APB_PD_CA7_C3_BISR_BUSY                                    BIT(3)
#define BIT_PMU_APB_PD_CA7_C2_BISR_BUSY                                    BIT(2)
#define BIT_PMU_APB_PD_CA7_C1_BISR_BUSY                                    BIT(1)
#define BIT_PMU_APB_PD_CA7_C0_BISR_BUSY                                    BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_BISR_BYP_CFG
// Register Offset : 0x0140
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_AON_MEM_BISR_FORCE_BYP                              BIT(18)
#define BIT_PMU_APB_PD_CP_SYS_BISR_FORCE_BYP                               BIT(17)
#define BIT_PMU_APB_PD_WTLCP_HU3GE_A_BISR_FORCE_BYP                        BIT(14)
#define BIT_PMU_APB_PD_WTLCP_TGDSP_BISR_FORCE_BYP                          BIT(13)
#define BIT_PMU_APB_PD_PUB_SYS_BISR_FORCE_BYP                              BIT(11)
#define BIT_PMU_APB_PD_WCN_TOP_BISR_FORCE_BYP                              BIT(10)
#define BIT_PMU_APB_PD_WCN_WIFI_BISR_FORCE_BYP                             BIT(9)
#define BIT_PMU_APB_PD_WCN_GNSS_BISR_FORCE_BYP                             BIT(8)
#define BIT_PMU_APB_PD_MM_TOP_BISR_FORCE_BYP                               BIT(7)
#define BIT_PMU_APB_PD_GPU_TOP_BISR_FORCE_BYP                              BIT(6)
#define BIT_PMU_APB_PD_AP_SYS_BISR_FORCE_BYP                               BIT(5)
#define BIT_PMU_APB_PD_CA7_TOP_BISR_FORCE_BYP                              BIT(4)
#define BIT_PMU_APB_PD_CA7_C3_BISR_FORCE_BYP                               BIT(3)
#define BIT_PMU_APB_PD_CA7_C2_BISR_FORCE_BYP                               BIT(2)
#define BIT_PMU_APB_PD_CA7_C1_BISR_FORCE_BYP                               BIT(1)
#define BIT_PMU_APB_PD_CA7_C0_BISR_FORCE_BYP                               BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_BISR_EN_CFG
// Register Offset : 0x0144
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_AON_MEM_BISR_FORCE_EN                               BIT(18)
#define BIT_PMU_APB_PD_CP_SYS_BISR_FORCE_EN                                BIT(17)
#define BIT_PMU_APB_PD_WTLCP_HU3GE_A_BISR_FORCE_EN                         BIT(14)
#define BIT_PMU_APB_PD_WTLCP_TGDSP_BISR_FORCE_EN                           BIT(13)
#define BIT_PMU_APB_PD_PUB_SYS_BISR_FORCE_EN                               BIT(11)
#define BIT_PMU_APB_PD_WCN_TOP_BISR_FORCE_EN                               BIT(10)
#define BIT_PMU_APB_PD_WCN_WIFI_BISR_FORCE_EN                              BIT(9)
#define BIT_PMU_APB_PD_WCN_GNSS_BISR_FORCE_EN                              BIT(8)
#define BIT_PMU_APB_PD_MM_TOP_BISR_FORCE_EN                                BIT(7)
#define BIT_PMU_APB_PD_GPU_TOP_BISR_FORCE_EN                               BIT(6)
#define BIT_PMU_APB_PD_AP_SYS_BISR_FORCE_EN                                BIT(5)
#define BIT_PMU_APB_PD_CA7_TOP_BISR_FORCE_EN                               BIT(4)
#define BIT_PMU_APB_PD_CA7_C3_BISR_FORCE_EN                                BIT(3)
#define BIT_PMU_APB_PD_CA7_C2_BISR_FORCE_EN                                BIT(2)
#define BIT_PMU_APB_PD_CA7_C1_BISR_FORCE_EN                                BIT(1)
#define BIT_PMU_APB_PD_CA7_C0_BISR_FORCE_EN                                BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CGM_AUTO_GATE_SEL_CFG0
// Register Offset : 0x0148
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CGM_AUTO_GATE_SEL_CFG0(x)                              (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CGM_AUTO_GATE_SEL_CFG1
// Register Offset : 0x014C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CGM_AUTO_GATE_SEL_CFG1(x)                              (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CGM_AUTO_GATE_SEL_CFG2
// Register Offset : 0x0150
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CGM_AUTO_GATE_SEL_CFG2(x)                              (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CGM_AUTO_GATE_SEL_CFG3
// Register Offset : 0x0154
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CGM_AUTO_GATE_SEL_CFG3(x)                              (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CGM_FORCE_EN_CFG0
// Register Offset : 0x0158
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CGM_FORCE_EN_CFG0(x)                                   (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CGM_FORCE_EN_CFG1
// Register Offset : 0x015C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CGM_FORCE_EN_CFG1(x)                                   (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CGM_FORCE_EN_CFG2
// Register Offset : 0x0160
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CGM_FORCE_EN_CFG2(x)                                   (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CGM_FORCE_EN_CFG3
// Register Offset : 0x0164
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CGM_FORCE_EN_CFG3(x)                                   (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_SLEEP_XTLON_CTRL
// Register Offset : 0x0168
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CM4_SLEEP_XTL_ON                                       BIT(5)
#define BIT_PMU_APB_PUBCP_SLEEP_XTL_ON                                     BIT(3)
#define BIT_PMU_APB_WCN_SLEEP_XTL_ON                                       BIT(2)
#define BIT_PMU_APB_WTLCP_SLEEP_XTL_ON                                     BIT(1)
#define BIT_PMU_APB_AP_SLEEP_XTL_ON                                        BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_MEM_SLP_CFG
// Register Offset : 0x016C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_MEM_SLP_CFG(x)                                         (((x) & 0xFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_MEM_SD_CFG
// Register Offset : 0x0170
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_MEM_SD_CFG(x)                                          (((x) & 0xFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CA7_CORE_PU_LOCK
// Register Offset : 0x0174
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CA7_C3_GIC_WAKEUP_EN                                   BIT(11)
#define BIT_PMU_APB_CA7_C2_GIC_WAKEUP_EN                                   BIT(10)
#define BIT_PMU_APB_CA7_C1_GIC_WAKEUP_EN                                   BIT(9)
#define BIT_PMU_APB_CA7_C0_GIC_WAKEUP_EN                                   BIT(8)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PWR_CNT_WAIT_CFG0
// Register Offset : 0x017C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CP_PWR_WAIT_CNT(x)                                     (((x) & 0xFF) << 16)
#define BIT_PMU_APB_WCN_PWR_WAIT_CNT(x)                                    (((x) & 0xFF) << 8)
#define BIT_PMU_APB_AP_PWR_WAIT_CNT(x)                                     (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PWR_CNT_WAIT_CFG1
// Register Offset : 0x0180
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CM4_PWR_WAIT_CNT(x)                                    (((x) & 0xFF) << 8)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_MEM_AUTO_SLP_CFG
// Register Offset : 0x0190
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_MEM_AUTO_SLP_EN(x)                                     (((x) & 0xFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_MEM_AUTO_SD_CFG
// Register Offset : 0x0194
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_MEM_AUTO_SD_EN(x)                                      (((x) & 0xFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_WAKEUP_LOCK_EN
// Register Offset : 0x01A0
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PUBCP_SYS_WAKEUP_LOCK_EN                               BIT(25)
#define BIT_PMU_APB_WTLCP_SYS_WAKEUP_LOCK_EN                               BIT(24)
#define BIT_PMU_APB_AP_SYS_WAKEUP_LOCK_EN                                  BIT(22)
#define BIT_PMU_APB_PD_PUB_SYS_WAKEUP_LOCK_EN                              BIT(21)
#define BIT_PMU_APB_PD_CP_SYS_WAKEUP_LOCK_EN                               BIT(17)
#define BIT_PMU_APB_PD_WTLCP_HU3GE_A_WAKEUP_LOCK_EN                        BIT(14)
#define BIT_PMU_APB_PD_WTLCP_TGDSP_WAKEUP_LOCK_EN                          BIT(13)
#define BIT_PMU_APB_PD_WCN_TOP_WAKEUP_LOCK_EN                              BIT(10)
#define BIT_PMU_APB_PD_WCN_WIFI_WAKEUP_LOCK_EN                             BIT(9)
#define BIT_PMU_APB_PD_WCN_GNSS_WAKEUP_LOCK_EN                             BIT(8)
#define BIT_PMU_APB_PD_MM_TOP_WAKEUP_LOCK_EN                               BIT(7)
#define BIT_PMU_APB_PD_GPU_TOP_WAKEUP_LOCK_EN                              BIT(6)
#define BIT_PMU_APB_PD_AP_SYS_WAKEUP_LOCK_EN                               BIT(5)
#define BIT_PMU_APB_PD_CA7_TOP_WAKEUP_LOCK_EN                              BIT(4)
#define BIT_PMU_APB_PD_CA7_C3_WAKEUP_LOCK_EN                               BIT(3)
#define BIT_PMU_APB_PD_CA7_C2_WAKEUP_LOCK_EN                               BIT(2)
#define BIT_PMU_APB_PD_CA7_C1_WAKEUP_LOCK_EN                               BIT(1)
#define BIT_PMU_APB_PD_CA7_C0_WAKEUP_LOCK_EN                               BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_WTLCP_TGDSP_CORE_INT_DISABLE
// Register Offset : 0x01B0
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_WTLCP_TGDSP_CORE_INT_DISABLE_FRC                       BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CP_CORE_INT_DISABLE
// Register Offset : 0x01B8
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CP_SYS_INT_DISABLE_FRC                                 BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CA7_C0_CORE_INT_DISABLE
// Register Offset : 0x01BC
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CA7_C0_CORE_INT_DISABLE                                BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CA7_C1_CORE_INT_DISABLE
// Register Offset : 0x01C0
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CA7_C1_CORE_INT_DISABLE                                BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CA7_C2_CORE_INT_DISABLE
// Register Offset : 0x01C4
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CA7_C2_CORE_INT_DISABLE                                BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CA7_C3_CORE_INT_DISABLE
// Register Offset : 0x01C8
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CA7_C3_CORE_INT_DISABLE                                BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_AON_CM4_INT_DISABLE
// Register Offset : 0x01CC
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_AON_CM4_INT_DISABLE                                    BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_WTLCP_TGDSP_DSLP_ENA
// Register Offset : 0x0200
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_WTLCP_TGDSP_DSLP_ENA                                   BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_AP_DSLP_ENA
// Register Offset : 0x0208
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_AP_DSLP_ENA                                            BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CP_DSLP_ENA
// Register Offset : 0x020C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PUBCP_DSLP_ENA                                         BIT(1)
#define BIT_PMU_APB_WTLCP_DSLP_ENA                                         BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CA7_TOP_DSLP_ENA
// Register Offset : 0x0214
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CA7_TOP_DSLP_ENA                                       BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CM4_DSLP_ENA
// Register Offset : 0x0218
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CM4_DSLP_ENA                                           BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PUBCP_INT_DISABLE
// Register Offset : 0x0220
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PUBCP_INT_DISABLE                                      BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_WTLCP_INT_DISABLE
// Register Offset : 0x0224
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_WTLCP_INT_DISABLE                                      BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_LIGHT_SLEEP_ENABLE
// Register Offset : 0x0230
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_DMA_CHN3_LSLP_ENA                                      BIT(18)
#define BIT_PMU_APB_DMA_CHN1_LSLP_ENA                                      BIT(17)
#define BIT_PMU_APB_DMA_CHNALL_LSLP_ENA                                    BIT(16)
#define BIT_PMU_APB_AON_DMA_LSLP_ENA                                       BIT(3)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CM4_LIGHT_SLEEP
// Register Offset : 0x0234
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CM4_LIGHT_SLEEP                                        BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_SYS_DOZE_DSLP_ENA
// Register Offset : 0x0240
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_WTLCP_DOZE_SLEEP_ENA                                   BIT(2)
#define BIT_PMU_APB_PUBCP_DOZE_SLEEP_ENA                                   BIT(1)
#define BIT_PMU_APB_AP_DOZE_SLEEP_ENA                                      BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PUB_ACC_RDY
// Register Offset : 0x0250
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PUB_ACC_RDY                                            BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PUB_CLK_RDY
// Register Offset : 0x0254
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PUB_CLK_RDY                                            BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_EIC_SEL
// Register Offset : 0x0258
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_EIC_WCN_SEL                                            BIT(2)
#define BIT_PMU_APB_EIC_WCN_DEEP_SLEEP_SEL                                 BIT(1)
#define BIT_PMU_APB_EIC_DEEP_SLEEP_SEL                                     BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_AXI_LP_CTRL_DISABLE
// Register Offset : 0x0260
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_AXI_LP_CTRL_DISABLE                                    BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PMU_DEBUG
// Register Offset : 0x0270
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PMU_DEBUG(x)                                           (((x) & 0xFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_SLEEP_CNT_CLR
// Register Offset : 0x0274
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_WTLCP_DOZE_SLEEP_CNT_CLR                               BIT(11)
#define BIT_PMU_APB_PUBCP_DOZE_SLEEP_CNT_CLR                               BIT(10)
#define BIT_PMU_APB_AP_DOZE_SLEEP_CNT_CLR                                  BIT(9)
#define BIT_PMU_APB_CM4_DEEP_SLEEP_CNT_CLR                                 BIT(8)
#define BIT_PMU_APB_PUB_DEEP_SLEEP_CNT_CLR                                 BIT(7)
#define BIT_PMU_APB_CP_DEEP_SLEEP_CNT_CLR                                  BIT(6)
#define BIT_PMU_APB_AP_DEEP_SLEEP_CNT_CLR                                  BIT(4)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PAD_OUT_ADIE_CTRL0
// Register Offset : 0x0290
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PAD_OUT_XTL_EN_POL_SEL                                 BIT(30)
#define BIT_PMU_APB_DCXO_LC_DEEP_SLEEP_POL_SEL                             BIT(29)
#define BIT_PMU_APB_PAD_OUT_CHIP_SLEEP_POL_SEL                             BIT(28)
#define BIT_PMU_APB_DCXO_LC_DEEP_SLEEP_PLL_PD_MASK                         BIT(26)
#define BIT_PMU_APB_DCXO_LC_DEEP_SLEEP_CM4_RAM_XTL_PD_MASK                 BIT(25)
#define BIT_PMU_APB_DCXO_LC_DEEP_SLEEP_AON_RAM_XTL_PD_MASK                 BIT(24)
#define BIT_PMU_APB_DCXO_LC_DEEP_SLEEP_XTLBUF0_PD_MASK                     BIT(23)
#define BIT_PMU_APB_DCXO_LC_DEEP_SLEEP_XTL0_PD_MASK                        BIT(22)
#define BIT_PMU_APB_DCXO_LC_DEEP_SLEEP_EXT_XTL_PD_MASK                     BIT(21)
#define BIT_PMU_APB_DCXO_LC_DEEP_SLEEP_CM4_DEEP_SLEEP_MASK                 BIT(20)
#define BIT_PMU_APB_DCXO_LC_DEEP_SLEEP_CA7_TOP_PD_MASK                     BIT(19)
#define BIT_PMU_APB_DCXO_LC_DEEP_SLEEP_WCN_DEEP_SLEEP_MASK                 BIT(18)
#define BIT_PMU_APB_DCXO_LC_DEEP_SLEEP_CP_DEEP_SLEEP_MASK                  BIT(17)
#define BIT_PMU_APB_DCXO_LC_DEEP_SLEEP_AP_DEEP_SLEEP_MASK                  BIT(16)
#define BIT_PMU_APB_PAD_OUT_CHIP_SLEEP_PUB_PD_MASK                         BIT(15)
#define BIT_PMU_APB_PAD_OUT_CHIP_SLEEP_PLL_PD_MASK                         BIT(14)
#define BIT_PMU_APB_PAD_OUT_CHIP_SLEEP_CM4_RAM_XTL_PD_MASK                 BIT(13)
#define BIT_PMU_APB_PAD_OUT_CHIP_SLEEP_AON_RAM_XTL_PD_MASK                 BIT(12)
#define BIT_PMU_APB_PAD_OUT_CHIP_SLEEP_XTLBUF0_PD_MASK                     BIT(11)
#define BIT_PMU_APB_PAD_OUT_CHIP_SLEEP_XTL0_PD_MASK                        BIT(10)
#define BIT_PMU_APB_PAD_OUT_CHIP_SLEEP_EXT_XTL_PD_MASK                     BIT(9)
#define BIT_PMU_APB_PAD_OUT_CHIP_SLEEP_CM4_DEEP_SLEEP_MASK                 BIT(8)
#define BIT_PMU_APB_PAD_OUT_CHIP_SLEEP_CA7_TOP_PD_MASK                     BIT(7)
#define BIT_PMU_APB_PAD_OUT_CHIP_SLEEP_WCN_DEEP_SLEEP_MASK                 BIT(6)
#define BIT_PMU_APB_PAD_OUT_CHIP_SLEEP_PUBCP_DEEP_SLEEP_MASK               BIT(5)
#define BIT_PMU_APB_PAD_OUT_CHIP_SLEEP_AP_DEEP_SLEEP_MASK                  BIT(4)
#define BIT_PMU_APB_PAD_OUT_CHIP_SLEEP_WTLCP_DEEP_SLEEP_MASK               BIT(3)
#define BIT_PMU_APB_EXT_XTL1_COMB_EN                                       BIT(1)
#define BIT_PMU_APB_EXT_XTL0_COMB_EN                                       BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_BISR_FORCE_SEL
// Register Offset : 0x0300
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_AON_MEM_BISR_FORCE_SEL                              BIT(18)
#define BIT_PMU_APB_PD_CP_SYS_BISR_FORCE_SEL                               BIT(17)
#define BIT_PMU_APB_PD_WTLCP_HU3GE_A_BISR_FORCE_SEL                        BIT(14)
#define BIT_PMU_APB_PD_WTLCP_TGDSP_BISR_FORCE_SEL                          BIT(13)
#define BIT_PMU_APB_PD_PUB_SYS_BISR_FORCE_SEL                              BIT(11)
#define BIT_PMU_APB_PD_WCN_TOP_BISR_FORCE_SEL                              BIT(10)
#define BIT_PMU_APB_PD_WCN_WIFI_BISR_FORCE_SEL                             BIT(9)
#define BIT_PMU_APB_PD_WCN_GNSS_BISR_FORCE_SEL                             BIT(8)
#define BIT_PMU_APB_PD_MM_TOP_BISR_FORCE_SEL                               BIT(7)
#define BIT_PMU_APB_PD_GPU_TOP_BISR_FORCE_SEL                              BIT(6)
#define BIT_PMU_APB_PD_AP_SYS_BISR_FORCE_SEL                               BIT(5)
#define BIT_PMU_APB_PD_CA7_TOP_BISR_FORCE_SEL                              BIT(4)
#define BIT_PMU_APB_PD_CA7_C3_BISR_FORCE_SEL                               BIT(3)
#define BIT_PMU_APB_PD_CA7_C2_BISR_FORCE_SEL                               BIT(2)
#define BIT_PMU_APB_PD_CA7_C1_BISR_FORCE_SEL                               BIT(1)
#define BIT_PMU_APB_PD_CA7_C0_BISR_FORCE_SEL                               BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_AON_MEM_CTRL
// Register Offset : 0x0330
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_WCN_MEM_SEL                                            BIT(3)
#define BIT_PMU_APB_RF_DIG_MEM_ALL_SEL                                     BIT(2)
#define BIT_PMU_APB_CM4_MEM_ALL_SEL                                        BIT(1)
#define BIT_PMU_APB_AON_MEM_CM4_SEL                                        BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PWR_DOMAIN_INT_CLR
// Register Offset : 0x0334
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_INT_REQ_PWR_DOWN_CLR(x)                                (((x) & 0x3FF) << 16)
#define BIT_PMU_APB_INT_REQ_PWR_UP_CLR(x)                                  (((x) & 0x3FF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_WCN_SYS_CFG_STATUS
// Register Offset : 0x0338
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_WCN_GNSS_WRAP_DEEP_STOP                                BIT(21)
#define BIT_PMU_APB_WCN_WIFI_WRAP_DEEP_STOP                                BIT(20)
#define BIT_PMU_APB_WCN_TOP_DDR_WAKE_UP_N                                  BIT(19)
#define BIT_PMU_APB_WCN_EXTCK_CLKOUT_EN                                    BIT(18)
#define BIT_PMU_APB_WCN_TOP_XTL_STOP                                       BIT(17)
#define BIT_PMU_APB_WCN_TOP_DEEP_STOP                                      BIT(16)
#define BIT_PMU_APB_WCN_XTL_SOFT_CNT_DONE                                  BIT(13)
#define BIT_PMU_APB_WCN_PLL_SOFT_CNT_DONE                                  BIT(12)
#define BIT_PMU_APB_WCN_XTL_PLL_FORCE_DOWN                                 BIT(11)
#define BIT_PMU_APB_WCN_XTL_CNT_SEL                                        BIT(8)
#define BIT_PMU_APB_WCN_PLL_CNT_SEL                                        BIT(7)
#define BIT_PMU_APB_WCN_XTL_AUTO_EN                                        BIT(6)
#define BIT_PMU_APB_WCN_PLL_AUTO_EN                                        BIT(5)
#define BIT_PMU_APB_WCN_XTL_AUTO_SEL                                       BIT(4)
#define BIT_PMU_APB_WCN_DEEP_STOP_AUTO_EN                                  BIT(3)
#define BIT_PMU_APB_WCN_PLL_PD_EN                                          BIT(2)
#define BIT_PMU_APB_WCN_GNSS_SYS_EN                                        BIT(1)
#define BIT_PMU_APB_WCN_BTWF_SYS_EN                                        BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_RF_CFG
// Register Offset : 0x033C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_EXT_XTLBUF_MASK                                        BIT(1)
#define BIT_PMU_APB_RF_XTLBUF_MASK                                         BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PUB_PLL_CFG
// Register Offset : 0x0340
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PUB_PLL_AUTO_PD_EN4                                    BIT(6)
#define BIT_PMU_APB_PUB_PLL_AUTO_PD_EN3                                    BIT(5)
#define BIT_PMU_APB_PUB_PLL_AUTO_PD_EN2                                    BIT(4)
#define BIT_PMU_APB_PUB_PLL_AUTO_PD_EN1                                    BIT(3)
#define BIT_PMU_APB_PUB_PLL_AUTO_PD_EN0                                    BIT(2)
#define BIT_PMU_APB_PUB_PLL_FORCE_DOWN                                     BIT(1)
#define BIT_PMU_APB_PUB_PLL_AUTO_PD_EN                                     BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_BBPLL_REL_CFG
// Register Offset : 0x0344
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_BBPLL2_PUBCP_SEL                                       BIT(7)
#define BIT_PMU_APB_BBPLL1_PUBCP_SEL                                       BIT(6)
#define BIT_PMU_APB_BBPLL2_CM4_SEL                                         BIT(5)
#define BIT_PMU_APB_BBPLL1_CM4_SEL                                         BIT(4)
#define BIT_PMU_APB_BBPLL2_AP_SEL                                          BIT(3)
#define BIT_PMU_APB_BBPLL1_AP_SEL                                          BIT(2)
#define BIT_PMU_APB_BBPLL2_WTLCP_SEL                                       BIT(1)
#define BIT_PMU_APB_BBPLL1_WTLCP_SEL                                       BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CP_CG_CFG
// Register Offset : 0x0348
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_WTLCP_CG_FORCE_OFF                                     BIT(6)
#define BIT_PMU_APB_WTLCP_CG_WITH_SLP_ENA                                  BIT(5)
#define BIT_PMU_APB_WTLCP_CG_AUTO_SEL                                      BIT(4)
#define BIT_PMU_APB_PUBCP_CG_FORCE_OFF                                     BIT(2)
#define BIT_PMU_APB_PUBCP_CG_WITH_SLP_ENA                                  BIT(1)
#define BIT_PMU_APB_PUBCP_CG_AUTO_SEL                                      BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_WCN_WIFI_DSLP_ENA
// Register Offset : 0x034C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_WCN_WIFI_DSLP_ENA                                      BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_WCN_GNSS_DSLP_ENA
// Register Offset : 0x0350
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_WCN_GNSS_DSLP_ENA                                      BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_EFUSE_SELECT_BUF_CFG
// Register Offset : 0x0354
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_EFUSE_SELECT_BUF_EN                                    BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_EFUSE_SELECT_BUF_CLR
// Register Offset : 0x0358
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_SELECT_BUF_WCN_WIFI_CLR                                BIT(11)
#define BIT_PMU_APB_SELECT_BUF_WCN_GNSS_CLR                                BIT(10)
#define BIT_PMU_APB_SELECT_BUF_WCN_TOP_CLR                                 BIT(9)
#define BIT_PMU_APB_SELECT_BUF_PUB_TOP_CLR                                 BIT(8)
#define BIT_PMU_APB_SELECT_BUF_WTLCP_HU3GE_A_CLR                           BIT(7)
#define BIT_PMU_APB_SELECT_BUF_WTLCP_TGDSP_CLR                             BIT(6)
#define BIT_PMU_APB_SELECT_BUF_GPU_TOP_CLR                                 BIT(5)
#define BIT_PMU_APB_SELECT_BUF_MM_TOP_CLR                                  BIT(4)
#define BIT_PMU_APB_SELECT_BUF_CP_SYS_CLR                                  BIT(3)
#define BIT_PMU_APB_SELECT_BUF_CA7_TOP_CLR                                 BIT(2)
#define BIT_PMU_APB_SELECT_BUF_AP_SYS_CLR                                  BIT(1)
#define BIT_PMU_APB_SELECT_BUF_AON_CLR                                     BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_EFUSE_SELECT_BUF_STS
// Register Offset : 0x035C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_SELECT_BUF_WCN_WIFI                                    BIT(11)
#define BIT_PMU_APB_SELECT_BUF_WCN_GNSS                                    BIT(10)
#define BIT_PMU_APB_SELECT_BUF_WCN_TOP                                     BIT(9)
#define BIT_PMU_APB_SELECT_BUF_PUB_TOP                                     BIT(8)
#define BIT_PMU_APB_SELECT_BUF_WTLCP_HU3GE_A                               BIT(7)
#define BIT_PMU_APB_SELECT_BUF_WTLCP_TGDSP                                 BIT(6)
#define BIT_PMU_APB_SELECT_BUF_GPU_TOP                                     BIT(5)
#define BIT_PMU_APB_SELECT_BUF_MM_TOP                                      BIT(4)
#define BIT_PMU_APB_SELECT_BUF_CP_SYS                                      BIT(3)
#define BIT_PMU_APB_SELECT_BUF_CA7_TOP                                     BIT(2)
#define BIT_PMU_APB_SELECT_BUF_AP_SYS                                      BIT(1)
#define BIT_PMU_APB_SELECT_BUF_AON                                         BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PMU_CLK_CFG
// Register Offset : 0x0360
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_AON_APB_PROTECT_TWPLL_CNT_EN                           BIT(3)
#define BIT_PMU_APB_AON_APB_PROTECT_CPLL_DISABLE                           BIT(2)
#define BIT_PMU_APB_AON_APB_PROTECT_EN                                     BIT(1)
#define BIT_PMU_APB_CGM_PMU_SEL_REG                                        BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PWR_ST_DLY0
// Register Offset : 0x0364
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CGM_OFF_DLY(x)                                         (((x) & 0xFF) << 24)
#define BIT_PMU_APB_CGM_ON_DLY(x)                                          (((x) & 0xFF) << 16)
#define BIT_PMU_APB_ISO_OFF_DLY(x)                                         (((x) & 0xFF) << 8)
#define BIT_PMU_APB_RST_ASSERT_DLY(x)                                      (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PWR_ST_DLY1
// Register Offset : 0x0368
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_WAKEUP_LOCK_DLY(x)                                     (((x) & 0xFF) << 24)
#define BIT_PMU_APB_RESTORE_DLY(x)                                         (((x) & 0xFF) << 16)
#define BIT_PMU_APB_SHUTDOWN_DLY(x)                                        (((x) & 0xFF) << 8)
#define BIT_PMU_APB_BISR_RST_DLY(x)                                        (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PMU_CLK_DIV_CFG0
// Register Offset : 0x036C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PWR_ST_CLK_DIV_CFG(x)                                  (((x) & 0x3FF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PMU_CLK_DIV_CFG1
// Register Offset : 0x0370
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_SLP_CTRL_CLK_DIV_CFG(x)                                (((x) & 0x3FF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PUB_DSLP_CFG0
// Register Offset : 0x0374
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_AP_PUB_DSLP_EN(x)                                      (((x) & 0xFF) << 24)
#define BIT_PMU_APB_PUBCP_PUB_DSLP_EN(x)                                   (((x) & 0xFF) << 16)
#define BIT_PMU_APB_WTLCP_PUB_DSLP_EN(x)                                   (((x) & 0xFF) << 8)
#define BIT_PMU_APB_WCN_PUB_DSLP_EN0(x)                                    (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PUB_DSLP_CFG1
// Register Offset : 0x0378
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_WCN_PUB_DSLP_EN1(x)                                    (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PUB_DSLP_EN
// Register Offset : 0x037C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PUB_DSLP_EN                                            BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PLL_CNT_DONE
// Register Offset : 0x0380
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_BBPLL1_CNT_DONE                                        BIT(5)
#define BIT_PMU_APB_BBPLL2_CNT_DONE                                        BIT(4)
#define BIT_PMU_APB_CPLL_CNT_DONE                                          BIT(3)
#define BIT_PMU_APB_TWPLL_CNT_DONE                                         BIT(2)
#define BIT_PMU_APB_DPLL_CNT_DONE                                          BIT(1)
#define BIT_PMU_APB_MPLL_CNT_DONE                                          BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CGM_XTL_EN
// Register Offset : 0x0384
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_BBPLL2_PD_REG                                          BIT(5)
#define BIT_PMU_APB_BBPLL1_PD_REG                                          BIT(4)
#define BIT_PMU_APB_RF_XTL_PD_REG                                          BIT(3)
#define BIT_PMU_APB_CGM_26M_WCN_EN                                         BIT(2)
#define BIT_PMU_APB_CGM_26M_CP_EN                                          BIT(1)
#define BIT_PMU_APB_CGM_26M_AP_EN                                          BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CGM_XTL_EN_SEL
// Register Offset : 0x0388
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_BBPLL2_PD_SEL                                          BIT(5)
#define BIT_PMU_APB_BBPLL1_PD_SEL                                          BIT(4)
#define BIT_PMU_APB_RF_XTL_SEL                                             BIT(3)
#define BIT_PMU_APB_CGM_26M_WCN_SEL                                        BIT(2)
#define BIT_PMU_APB_CGM_26M_CP_SEL                                         BIT(1)
#define BIT_PMU_APB_CGM_26M_AP_SEL                                         BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_CA7_RST_CFG
// Register Offset : 0x038C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_CA7_VINITHI_RST_DISABLE                                BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_SYS_ACK
// Register Offset : 0x0390
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_PUB_SYS_ACK_M                                       BIT(29)
#define BIT_PMU_APB_PD_PUB_SYS_ACK_D                                       BIT(28)
#define BIT_PMU_APB_PD_WCN_GNSS_ACK_M                                      BIT(27)
#define BIT_PMU_APB_PD_WCN_GNSS_ACK_D                                      BIT(26)
#define BIT_PMU_APB_PD_WCN_WIFI_ACK_M                                      BIT(25)
#define BIT_PMU_APB_PD_WCN_WIFI_ACK_D                                      BIT(24)
#define BIT_PMU_APB_PD_WCN_TOP_ACK_M                                       BIT(23)
#define BIT_PMU_APB_PD_WCN_TOP_ACK_D                                       BIT(22)
#define BIT_PMU_APB_PD_WTLCP_TGDSP_ACK_M                                   BIT(21)
#define BIT_PMU_APB_PD_WTLCP_TGDSP_ACK_D                                   BIT(20)
#define BIT_PMU_APB_PD_WTLCP_HU3GE_A_ACK_M                                 BIT(19)
#define BIT_PMU_APB_PD_WTLCP_HU3GE_A_ACK_D                                 BIT(18)
#define BIT_PMU_APB_PD_MM_TOP_ACK_M                                        BIT(17)
#define BIT_PMU_APB_PD_MM_TOP_ACK_D                                        BIT(16)
#define BIT_PMU_APB_PD_GPU_TOP_ACK_M                                       BIT(15)
#define BIT_PMU_APB_PD_GPU_TOP_ACK_D                                       BIT(14)
#define BIT_PMU_APB_PD_CP_SYS_ACK_M                                        BIT(13)
#define BIT_PMU_APB_PD_CP_SYS_ACK_D                                        BIT(12)
#define BIT_PMU_APB_PD_AP_SYS_ACK_M                                        BIT(11)
#define BIT_PMU_APB_PD_AP_SYS_ACK_D                                        BIT(10)
#define BIT_PMU_APB_PD_CA7_C3_ACK_M                                        BIT(9)
#define BIT_PMU_APB_PD_CA7_C3_ACK_D                                        BIT(8)
#define BIT_PMU_APB_PD_CA7_C2_ACK_M                                        BIT(7)
#define BIT_PMU_APB_PD_CA7_C2_ACK_D                                        BIT(6)
#define BIT_PMU_APB_PD_CA7_C1_ACK_M                                        BIT(5)
#define BIT_PMU_APB_PD_CA7_C1_ACK_D                                        BIT(4)
#define BIT_PMU_APB_PD_CA7_C0_ACK_M                                        BIT(3)
#define BIT_PMU_APB_PD_CA7_C0_ACK_D                                        BIT(2)
#define BIT_PMU_APB_PD_CA7_TOP_ACK_M                                       BIT(1)
#define BIT_PMU_APB_PD_CA7_TOP_ACK_D                                       BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_CA7_C0_SHUTDOWN_MARK_STATUS
// Register Offset : 0x3000
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_CA7_C0_SHUTDOWN_MARK(x)                             (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_CA7_C1_SHUTDOWN_MARK_STATUS
// Register Offset : 0x3004
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_CA7_C1_SHUTDOWN_MARK(x)                             (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_CA7_C2_SHUTDOWN_MARK_STATUS
// Register Offset : 0x3008
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_CA7_C2_SHUTDOWN_MARK(x)                             (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_CA7_C3_SHUTDOWN_MARK_STATUS
// Register Offset : 0x300C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_CA7_C3_SHUTDOWN_MARK(x)                             (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_CA7_TOP_SHUTDOWN_MARK_STATUS
// Register Offset : 0x3010
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_CA7_TOP_SHUTDOWN_MARK(x)                            (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_AP_SYS_SHUTDOWN_MARK_STATUS
// Register Offset : 0x3014
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_AP_SYS_SHUTDOWN_MARK(x)                             (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_GPU_TOP_SHUTDOWN_MARK_STATUS
// Register Offset : 0x3018
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_GPU_TOP_SHUTDOWN_MARK(x)                            (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_MM_TOP_SHUTDOWN_MARK_STATUS
// Register Offset : 0x301C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_MM_TOP_SHUTDOWN_MARK(x)                             (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_WTLCP_TGDSP_SHUTDOWN_MARK_STATUS
// Register Offset : 0x3034
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_WTLCP_TGDSP_SHUTDOWN_MARK(x)                        (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_WTLCP_HU3GE_A_SHUTDOWN_MARK_STATUS
// Register Offset : 0x3038
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_WTLCP_HU3GE_A_SHUTDOWN_MARK(x)                      (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_CP_SYS_SHUTDOWN_MARK_STATUS
// Register Offset : 0x3044
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_CP_SYS_SHUTDOWN_MARK(x)                             (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_PUB_SYS_SHUTDOWN_MARK_STATUS
// Register Offset : 0x3054
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_PUB_SYS_SHUTDOWN_MARK(x)                            (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_WCN_TOP_SHUTDOWN_MARK_STATUS
// Register Offset : 0x3058
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_WCN_TOP_SHUTDOWN_MARK(x)                            (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_WCN_WIFI_SHUTDOWN_MARK_STATUS
// Register Offset : 0x305C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_WCN_WIFI_SHUTDOWN_MARK(x)                           (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PD_WCN_GNSS_SHUTDOWN_MARK_STATUS
// Register Offset : 0x3060
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PD_WCN_GNSS_SHUTDOWN_MARK(x)                           (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PMU_DUMMY1
// Register Offset : 0x3064
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PMU_DUMMY1(x)                                          (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PMU_DUMMY2
// Register Offset : 0x3068
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PMU_DUMMY2(x)                                          (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PMU_DUMMY3
// Register Offset : 0x306C
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PMU_DUMMY3(x)                                          (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PMU_DUMMY4
// Register Offset : 0x3070
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PMU_DUMMY4(x)                                          (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PMU_DUMMY5
// Register Offset : 0x3074
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PMU_DUMMY5(x)                                          (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_PMU_APB_PMU_DUMMY6
// Register Offset : 0x3078
// Description     :
---------------------------------------------------------------------------*/

#define BIT_PMU_APB_PMU_DUMMY6(x)                                          (((x) & 0xFFFFFFFF))


#endif // _PMU_APB_REG_H
