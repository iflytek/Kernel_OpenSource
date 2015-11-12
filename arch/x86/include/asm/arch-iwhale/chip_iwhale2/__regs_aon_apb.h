/*
 * Copyright (C) 2015 Spreadtrum Communications Inc.
 *
 * This file is dual-licensed: you can use it either under the terms
 * of the GPL or the X11 license, at your option. Note that this dual
 * licensing only applies to this file, and not this project as a
 * whole.
 *
 */


#ifndef _AON_APB_REG_H
#define _AON_APB_REG_H

#define CTL_BASE_AON_APB    REGS_AON_APB_BASE


#define REG_AON_APB_APB_EB0                        ( CTL_BASE_AON_APB + 0x0000 )
#define REG_AON_APB_APB_EB1                        ( CTL_BASE_AON_APB + 0x0004 )
#define REG_AON_APB_APB_RST0                       ( CTL_BASE_AON_APB + 0x0008 )
#define REG_AON_APB_APB_RST1                       ( CTL_BASE_AON_APB + 0x000C )
#define REG_AON_APB_APB_RTC_EB                     ( CTL_BASE_AON_APB + 0x0010 )
#define REG_AON_APB_PWR_CTRL                       ( CTL_BASE_AON_APB + 0x0024 )
#define REG_AON_APB_TS_CFG                         ( CTL_BASE_AON_APB + 0x0028 )
#define REG_AON_APB_BOOT_MODE                      ( CTL_BASE_AON_APB + 0x002C )
#define REG_AON_APB_BB_BG_CTRL                     ( CTL_BASE_AON_APB + 0x0030 )
#define REG_AON_APB_CP_ARM_JTAG_CTRL               ( CTL_BASE_AON_APB + 0x0034 )
#define REG_AON_APB_DCXO_LC_REG0                   ( CTL_BASE_AON_APB + 0x003C )
#define REG_AON_APB_DCXO_LC_REG1                   ( CTL_BASE_AON_APB + 0x0040 )
#define REG_AON_APB_AGCP_BOOT_PROT                 ( CTL_BASE_AON_APB + 0x0078 )
#define REG_AON_APB_AON_REG_PROT                   ( CTL_BASE_AON_APB + 0x007C )
#define REG_AON_APB_DAP_DJTAG_SEL                  ( CTL_BASE_AON_APB + 0x0084 )
#define REG_AON_APB_USER_RSV_FLAG1                 ( CTL_BASE_AON_APB + 0x0088 )
#define REG_AON_APB_AON_CGM_CFG                    ( CTL_BASE_AON_APB + 0x0098 )
#define REG_AON_APB_VSP_FIFO_CH0_1                 ( CTL_BASE_AON_APB + 0x00A0 )
#define REG_AON_APB_VSP_FIFO_CH0_2                 ( CTL_BASE_AON_APB + 0x00A4 )
#define REG_AON_APB_VSP_FIFO_CH1_1                 ( CTL_BASE_AON_APB + 0x00AC )
#define REG_AON_APB_VSP_FIFO_CH1_2                 ( CTL_BASE_AON_APB + 0x00B0 )
#define REG_AON_APB_CAM_FIFO_CH0_1                 ( CTL_BASE_AON_APB + 0x00B8 )
#define REG_AON_APB_CAM_FIFO_CH0_2                 ( CTL_BASE_AON_APB + 0x00BC )
#define REG_AON_APB_CAM_FIFO_CH1_1                 ( CTL_BASE_AON_APB + 0x00C0 )
#define REG_AON_APB_CAM_FIFO_CH1_2                 ( CTL_BASE_AON_APB + 0x00C4 )
#define REG_AON_APB_GPU_FIFO_CH0_1                 ( CTL_BASE_AON_APB + 0x00CC )
#define REG_AON_APB_GPU_FIFO_CH0_2                 ( CTL_BASE_AON_APB + 0x00D0 )
#define REG_AON_APB_GPU_FIFO_CH1_1                 ( CTL_BASE_AON_APB + 0x00D4 )
#define REG_AON_APB_GPU_FIFO_CH1_2                 ( CTL_BASE_AON_APB + 0x00D8 )
#define REG_AON_APB_AP_FIFO_CH_1                   ( CTL_BASE_AON_APB + 0x00E0 )
#define REG_AON_APB_AP_FIFO_CH_2                   ( CTL_BASE_AON_APB + 0x00E4 )
#define REG_AON_APB_PUBCP_FIFO_CH_1                ( CTL_BASE_AON_APB + 0x00E8 )
#define REG_AON_APB_PUBCP_FIFO_CH_2                ( CTL_BASE_AON_APB + 0x00EC )
#define REG_AON_APB_PUBCP_FIFO_CH_3                ( CTL_BASE_AON_APB + 0x00F0 )
#define REG_AON_APB_AON_CHIP_ID0                   ( CTL_BASE_AON_APB + 0x00F8 )
#define REG_AON_APB_AON_CHIP_ID1                   ( CTL_BASE_AON_APB + 0x00FC )
#define REG_AON_APB_CCIR_RCVR_CFG                  ( CTL_BASE_AON_APB + 0x0100 )
#define REG_AON_APB_PLL_BG_CFG                     ( CTL_BASE_AON_APB + 0x0108 )
#define REG_AON_APB_BIA_R_FIFO_CH_1                ( CTL_BASE_AON_APB + 0x0110 )
#define REG_AON_APB_BIA_R_FIFO_CH_2                ( CTL_BASE_AON_APB + 0x0114 )
#define REG_AON_APB_BIA_W_FIFO_CH_1                ( CTL_BASE_AON_APB + 0x0118 )
#define REG_AON_APB_BIA_W_FIFO_CH_2                ( CTL_BASE_AON_APB + 0x011C )
#define REG_AON_APB_SP_CFG_BUS                     ( CTL_BASE_AON_APB + 0x0124 )
#define REG_AON_APB_RTC4M_0_CFG                    ( CTL_BASE_AON_APB + 0x0128 )
#define REG_AON_APB_APB_RST2                       ( CTL_BASE_AON_APB + 0x0130 )
#define REG_AON_APB_RC100M_CFG                     ( CTL_BASE_AON_APB + 0x0134 )
#define REG_AON_APB_CGM_REG1                       ( CTL_BASE_AON_APB + 0x0138 )
#define REG_AON_APB_CGM_CLK_TOP_REG1               ( CTL_BASE_AON_APB + 0x013C )
#define REG_AON_APB_AGCP_DSP_CTRL0                 ( CTL_BASE_AON_APB + 0x0140 )
#define REG_AON_APB_AGCP_DSP_CTRL1                 ( CTL_BASE_AON_APB + 0x0144 )
#define REG_AON_APB_AGCP_SOFT_RESET                ( CTL_BASE_AON_APB + 0x0148 )
#define REG_AON_APB_AGCP_CTRL                      ( CTL_BASE_AON_APB + 0x014C )
#define REG_AON_APB_WTLCP_LDSP_CTRL0               ( CTL_BASE_AON_APB + 0x0150 )
#define REG_AON_APB_WTLCP_LDSP_CTRL1               ( CTL_BASE_AON_APB + 0x0154 )
#define REG_AON_APB_WTLCP_TDSP_CTRL0               ( CTL_BASE_AON_APB + 0x0158 )
#define REG_AON_APB_WTLCP_TDSP_CTRL1               ( CTL_BASE_AON_APB + 0x015C )
#define REG_AON_APB_WTLCP_SOFT_RESET               ( CTL_BASE_AON_APB + 0x0160 )
#define REG_AON_APB_WTLCP_CTRL                     ( CTL_BASE_AON_APB + 0x0164 )
#define REG_AON_APB_WTL_WCDMA_EB                   ( CTL_BASE_AON_APB + 0x0168 )
#define REG_AON_APB_PCP_AON_EB                     ( CTL_BASE_AON_APB + 0x0170 )
#define REG_AON_APB_PCP_SOFT_RST                   ( CTL_BASE_AON_APB + 0x0174 )
#define REG_AON_APB_PUBCP_CTRL                     ( CTL_BASE_AON_APB + 0x0178 )
#define REG_AON_APB_USB3_CTRL                      ( CTL_BASE_AON_APB + 0x0180 )
#define REG_AON_APB_DEBUG_U2PMU                    ( CTL_BASE_AON_APB + 0x0184 )
#define REG_AON_APB_DEBUG_U3PMU                    ( CTL_BASE_AON_APB + 0x0188 )
#define REG_AON_APB_FIREWALL_CLK_EN                ( CTL_BASE_AON_APB + 0x0190 )
#define REG_AON_APB_GPU_CTRL                       ( CTL_BASE_AON_APB + 0x0194 )
#define REG_AON_APB_SUB_SYS_DEBUG_BUS_SEL_0        ( CTL_BASE_AON_APB + 0x01C0 )
#define REG_AON_APB_SUB_SYS_DEBUG_BUS_SEL_1        ( CTL_BASE_AON_APB + 0x01C4 )
#define REG_AON_APB_GLB_DEBUG_BUS_SEL              ( CTL_BASE_AON_APB + 0x01C8 )
#define REG_AON_APB_SOFT_RST_AON_ADD1              ( CTL_BASE_AON_APB + 0x01CC )
#define REG_AON_APB_EB_AON_ADD1                    ( CTL_BASE_AON_APB + 0x01D0 )
#define REG_AON_APB_DBG_DJTAG_CTRL                 ( CTL_BASE_AON_APB + 0x01D4 )
#define REG_AON_APB_PUB_FC_CTRL                    ( CTL_BASE_AON_APB + 0x01E0 )
#define REG_AON_APB_EMC_CKG_SEL                    ( CTL_BASE_AON_APB + 0x01E4 )
#define REG_AON_APB_ANA_PHY_CTRL0                  ( CTL_BASE_AON_APB + 0x01E8 )
#define REG_AON_APB_GPU_DDR_CTRL                   ( CTL_BASE_AON_APB + 0x01EC )
#define REG_AON_APB_GPU_BOUDING_PIN                ( CTL_BASE_AON_APB + 0x01F0 )
#define REG_AON_APB_SP_EIC_IN_SEL                  ( CTL_BASE_AON_APB + 0x01F4 )
#define REG_AON_APB_BIA_WDG_WARM_RST_EN            ( CTL_BASE_AON_APB + 0x01F8 )
#define REG_AON_APB_AON_UFS_CTRL                   ( CTL_BASE_AON_APB + 0x0200 )
#define REG_AON_APB_VSP_DDR_CTRL                   ( CTL_BASE_AON_APB + 0x0204 )
#define REG_AON_APB_CAM_DDR_CTRL                   ( CTL_BASE_AON_APB + 0x0208 )
#define REG_AON_APB_PRE_DBG_BUS_5_0_SEL            ( CTL_BASE_AON_APB + 0x0214 )
#define REG_AON_APB_PRE_DBG_BUS_11_6_SEL           ( CTL_BASE_AON_APB + 0x0218 )
#define REG_AON_APB_PRE_DBG_BUS_17_12_SEL          ( CTL_BASE_AON_APB + 0x021C )
#define REG_AON_APB_PRE_DBG_BUS_23_18_SEL          ( CTL_BASE_AON_APB + 0x0220 )
#define REG_AON_APB_PRE_DBG_BUS_29_24_SEL          ( CTL_BASE_AON_APB + 0x0224 )
#define REG_AON_APB_PRE_DBG_BUS_31_30_SEL          ( CTL_BASE_AON_APB + 0x0228 )
#define REG_AON_APB_SUB_SYS_DBG_SIG0               ( CTL_BASE_AON_APB + 0x029C )
#define REG_AON_APB_SUB_SYS_DBG_SIG1               ( CTL_BASE_AON_APB + 0x02A0 )
#define REG_AON_APB_SUB_SYS_DBG_SIG2               ( CTL_BASE_AON_APB + 0x02A4 )
#define REG_AON_APB_SUB_SYS_DBG_SIG3               ( CTL_BASE_AON_APB + 0x02A8 )
#define REG_AON_APB_SUB_SYS_DBG_SIG4               ( CTL_BASE_AON_APB + 0x02AC )
#define REG_AON_APB_SUB_SYS_DBG_SIG5               ( CTL_BASE_AON_APB + 0x02B0 )
#define REG_AON_APB_SUB_SYS_DBG_SIG6               ( CTL_BASE_AON_APB + 0x02B4 )
#define REG_AON_APB_SUB_SYS_DBG_SIG7               ( CTL_BASE_AON_APB + 0x02B8 )
#define REG_AON_APB_DEBUG_SUBSYS_SEL               ( CTL_BASE_AON_APB + 0x02BC )
#define REG_AON_APB_MDAR_SYS_HSDL_CFG              ( CTL_BASE_AON_APB + 0x02C0 )
#define REG_AON_APB_WTLCP2AON_R_FIFO_CH_1          ( CTL_BASE_AON_APB + 0x02C4 )
#define REG_AON_APB_WTLCP2AON_R_FIFO_CH0_2         ( CTL_BASE_AON_APB + 0x02C8 )
#define REG_AON_APB_WTLCP2AON_W_FIFO_CH_1          ( CTL_BASE_AON_APB + 0x02CC )
#define REG_AON_APB_WTLCP2AON_W_FIFO_CH0_2         ( CTL_BASE_AON_APB + 0x02D0 )
#define REG_AON_APB_AGCP2AON_R_FIFO_CH_1           ( CTL_BASE_AON_APB + 0x02D4 )
#define REG_AON_APB_AGCP2AON_R_FIFO_CH0_2          ( CTL_BASE_AON_APB + 0x02D8 )
#define REG_AON_APB_AGCP2AON_W_FIFO_CH_1           ( CTL_BASE_AON_APB + 0x02DC )
#define REG_AON_APB_AGCP2AON_W_FIFO_CH0_2          ( CTL_BASE_AON_APB + 0x02E0 )
#define REG_AON_APB_PUBCP2AON_R_FIFO_CH_1          ( CTL_BASE_AON_APB + 0x02E4 )
#define REG_AON_APB_PUBCP2AON_R_FIFO_CH0_2         ( CTL_BASE_AON_APB + 0x02E8 )
#define REG_AON_APB_PUBCP2AON_W_FIFO_CH_1          ( CTL_BASE_AON_APB + 0x02EC )
#define REG_AON_APB_PUBCP2AON_W_FIFO_CH0_2         ( CTL_BASE_AON_APB + 0x02F0 )
#define REG_AON_APB_AON2PUBCP_R_FIFO_CH_1          ( CTL_BASE_AON_APB + 0x02F4 )
#define REG_AON_APB_AON2PUBCP_R_FIFO_CH0_2         ( CTL_BASE_AON_APB + 0x02F8 )
#define REG_AON_APB_AON2PUBCP_W_FIFO_CH_1          ( CTL_BASE_AON_APB + 0x02FC )
#define REG_AON_APB_AON2PUBCP_W_FIFO_CH0_2         ( CTL_BASE_AON_APB + 0x0300 )
#define REG_AON_APB_AON2AGCP_R_FIFO_CH_1           ( CTL_BASE_AON_APB + 0x0304 )
#define REG_AON_APB_AON2AGCP_R_FIFO_CH0_2          ( CTL_BASE_AON_APB + 0x0308 )
#define REG_AON_APB_AON2AGCP_W_FIFO_CH_1           ( CTL_BASE_AON_APB + 0x030C )
#define REG_AON_APB_AON2AGCP_W_FIFO_CH0_2          ( CTL_BASE_AON_APB + 0x0310 )
#define REG_AON_APB_AON2PUB0_R_FIFO_CH_1           ( CTL_BASE_AON_APB + 0x0314 )
#define REG_AON_APB_AON2PUB0_R_FIFO_CH0_2          ( CTL_BASE_AON_APB + 0x031C )
#define REG_AON_APB_AON2PUB0_W_FIFO_CH_1           ( CTL_BASE_AON_APB + 0x0320 )
#define REG_AON_APB_AON2PUB0_W_FIFO_CH0_2          ( CTL_BASE_AON_APB + 0x0324 )
#define REG_AON_APB_AON2PUB1_R_FIFO_CH_1           ( CTL_BASE_AON_APB + 0x0328 )
#define REG_AON_APB_AON2PUB1_R_FIFO_CH0_2          ( CTL_BASE_AON_APB + 0x032C )
#define REG_AON_APB_AON2PUB1_W_FIFO_CH_1           ( CTL_BASE_AON_APB + 0x0330 )
#define REG_AON_APB_AON2PUB1_W_FIFO_CH0_2          ( CTL_BASE_AON_APB + 0x0334 )
#define REG_AON_APB_BIA_NOC_CTRL                   ( CTL_BASE_AON_APB + 0x0338 )
#define REG_AON_APB_AP_NOC_CTRL                    ( CTL_BASE_AON_APB + 0x033C )
#define REG_AON_APB_DMC_CLK_PLL_SEL                ( CTL_BASE_AON_APB + 0x0340 )
#define REG_AON_APB_USB30_SLEEP                    ( CTL_BASE_AON_APB + 0x0344 )
#define REG_AON_APB_ACCDDR_LATENCY_REDUCE_SEL      ( CTL_BASE_AON_APB + 0x0348 )
#define REG_AON_APB_BIA_DVFS_CHANNEL_SEL           ( CTL_BASE_AON_APB + 0x034C )
#define REG_AON_APB_IO_DLY_CTRL                    ( CTL_BASE_AON_APB + 0x3014 )
#define REG_AON_APB_WDG_RST_FLAG                   ( CTL_BASE_AON_APB + 0x3024 )
#define REG_AON_APB_PMU_RST_MONITOR                ( CTL_BASE_AON_APB + 0x302C )
#define REG_AON_APB_THM_RST_MONITOR                ( CTL_BASE_AON_APB + 0x3030 )
#define REG_AON_APB_AP_RST_MONITOR                 ( CTL_BASE_AON_APB + 0x3034 )
#define REG_AON_APB_BOND_OPT0                      ( CTL_BASE_AON_APB + 0x303C )
#define REG_AON_APB_BOND_OPT1                      ( CTL_BASE_AON_APB + 0x3040 )
#define REG_AON_APB_RES_REG0                       ( CTL_BASE_AON_APB + 0x3044 )
#define REG_AON_APB_RES_REG1                       ( CTL_BASE_AON_APB + 0x3048 )
#define REG_AON_APB_AON_QOS_CFG                    ( CTL_BASE_AON_APB + 0x304C )
#define REG_AON_APB_AON_MTX_PROT_CFG               ( CTL_BASE_AON_APB + 0x3058 )
#define REG_AON_APB_PLL_LOCK_OUT_SEL               ( CTL_BASE_AON_APB + 0x3064 )
#define REG_AON_APB_RTC4M_RC_VAL                   ( CTL_BASE_AON_APB + 0x3068 )
#define REG_AON_APB_DDR_STAT_FLAG                  ( CTL_BASE_AON_APB + 0x306C )
#define REG_AON_APB_AON_QOS_CFG1                   ( CTL_BASE_AON_APB + 0x3070 )
#define REG_AON_APB_USER_COMM_FLAG                 ( CTL_BASE_AON_APB + 0x3074 )
#define REG_AON_APB_AON_MAIN_MTX_QOS1              ( CTL_BASE_AON_APB + 0x3078 )
#define REG_AON_APB_AON_MAIN_MTX_QOS2              ( CTL_BASE_AON_APB + 0x307C )
#define REG_AON_APB_RC100M_RC_VAL                  ( CTL_BASE_AON_APB + 0x3080 )
#define REG_AON_APB_AON_APB_RSV                    ( CTL_BASE_AON_APB + 0x30F0 )
#define REG_AON_APB_GLB_CLK_AUTO_GATE_EN           ( CTL_BASE_AON_APB + 0x30F4 )
#define REG_AON_APB_AON_OSC_FUNC_CNT               ( CTL_BASE_AON_APB + 0x3100 )
#define REG_AON_APB_AON_OSC_FUNC_CTRL              ( CTL_BASE_AON_APB + 0x3104 )
#define REG_AON_APB_DEEP_SLEEP_MUX_SEL             ( CTL_BASE_AON_APB + 0x3108 )
#define REG_AON_APB_NIU_IDLE_DET_DISABLE           ( CTL_BASE_AON_APB + 0x310C )
#define REG_AON_APB_FUNC_TEST_BOOT_ADDR            ( CTL_BASE_AON_APB + 0x3110 )
#define REG_AON_APB_CGM_RESCUE                     ( CTL_BASE_AON_APB + 0x3114 )
#define REG_AON_APB_AON_PLAT_ID0                   ( CTL_BASE_AON_APB + 0x3118 )
#define REG_AON_APB_AON_PLAT_ID1                   ( CTL_BASE_AON_APB + 0x311C )
#define REG_AON_APB_AON_VER_ID                     ( CTL_BASE_AON_APB + 0x3120 )
#define REG_AON_APB_AON_MFT_ID                     ( CTL_BASE_AON_APB + 0x3124 )
#define REG_AON_APB_AON_MPL_ID                     ( CTL_BASE_AON_APB + 0x3128 )
#define REG_AON_APB_AON_SDIO                       ( CTL_BASE_AON_APB + 0x312C )
#define REG_AON_APB_PWM_3D_MUX                     ( CTL_BASE_AON_APB + 0x3134 )
#define REG_AON_APB_VSP_NOC_CTRL                   ( CTL_BASE_AON_APB + 0x315C )
#define REG_AON_APB_CAM_NOC_CTRL                   ( CTL_BASE_AON_APB + 0x3160 )
#define REG_AON_APB_DISP_NOC_CTRL                  ( CTL_BASE_AON_APB + 0x3164 )
#define REG_AON_APB_GSP_NOC_CTRL                   ( CTL_BASE_AON_APB + 0x3168 )
#define REG_AON_APB_WTL_CP_NOC_CTRL                ( CTL_BASE_AON_APB + 0x316C )
#define REG_AON_APB_LACC_NOC_CTRL                  ( CTL_BASE_AON_APB + 0x3170 )
#define REG_AON_APB_AGCP_NOC_CTRL                  ( CTL_BASE_AON_APB + 0x3174 )
#define REG_AON_APB_PCP_NOC_CTRL                   ( CTL_BASE_AON_APB + 0x3178 )
#define REG_AON_APB_PCP_TIMESTAMP_CTRL             ( CTL_BASE_AON_APB + 0x3180 )
#define REG_AON_APB_SP_WAKEUP_MASK_EN1             ( CTL_BASE_AON_APB + 0x3184 )
#define REG_AON_APB_SP_WAKEUP_MASK_EN2             ( CTL_BASE_AON_APB + 0x3188 )
#define REG_AON_APB_THERMAL_OVERHEAT_EN            ( CTL_BASE_AON_APB + 0x318C )
#define REG_AON_APB_BIA_REG_DAT0                   ( CTL_BASE_AON_APB + 0x3278 )
#define REG_AON_APB_BIA_REG_DAT1                   ( CTL_BASE_AON_APB + 0x327C )

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_APB_EB0
// Register Offset : 0x0000
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_I2C_EB                                       BIT(31)
#define BIT_AON_APB_CA53_DAP_EB                                  BIT(30)
#define BIT_AON_APB_BIA_DVFS_EB                                  BIT(29)
#define BIT_AON_APB_DBG_SERDES_EB                                BIT(28)
#define BIT_AON_APB_AON_GPU_EB                                   BIT(27)
#define BIT_AON_APB_CKG_EB                                       BIT(26)
#define BIT_AON_APB_PIN_EB                                       BIT(25)
#define BIT_AON_APB_MDAR_APB_EN                                  BIT(24)
#define BIT_AON_APB_MSPI_EB                                      BIT(23)
#define BIT_AON_APB_SPLK_EB                                      BIT(22)
#define BIT_AON_APB_AP_HS_SPI_EB                                 BIT(21)
#define BIT_AON_APB_AP_INTC3_EB                                  BIT(20)
#define BIT_AON_APB_AP_INTC2_EB                                  BIT(19)
#define BIT_AON_APB_AP_INTC1_EB                                  BIT(18)
#define BIT_AON_APB_AP_INTC0_EB                                  BIT(17)
#define BIT_AON_APB_ADI_EB                                       BIT(16)
#define BIT_AON_APB_AON_APB_EB                                   BIT(15)
#define BIT_AON_APB_EIC_EB                                       BIT(14)
#define BIT_AON_APB_EFUSE_EB                                     BIT(13)
#define BIT_AON_APB_AP_TMR0_EB                                   BIT(12)
#define BIT_AON_APB_AON_TMR_EB                                   BIT(11)
#define BIT_AON_APB_AP_SYST_EB                                   BIT(10)
#define BIT_AON_APB_AON_SYST_EB                                  BIT(9)
#define BIT_AON_APB_KPD_EB                                       BIT(8)
#define BIT_AON_APB_PWM3_EB                                      BIT(7)
#define BIT_AON_APB_PWM2_EB                                      BIT(6)
#define BIT_AON_APB_PWM1_EB                                      BIT(5)
#define BIT_AON_APB_PWM0_EB                                      BIT(4)
#define BIT_AON_APB_GPIO_EB                                      BIT(3)
#define BIT_AON_APB_PUB_APB_EB                                   BIT(2)
#define BIT_AON_APB_AVS_BIA_CPU1_EB                              BIT(1)
#define BIT_AON_APB_AVS_BIA_CPU0_EB                              BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_APB_EB1
// Register Offset : 0x0004
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_DBG_AXI_IF_EB                                BIT(31)
#define BIT_AON_APB_AON_DISP_EB                                  BIT(30)
#define BIT_AON_APB_AON_CAM_EB                                   BIT(29)
#define BIT_AON_APB_AON_VSP_EB                                   BIT(28)
#define BIT_AON_APB_ORP_JTAG_EB                                  BIT(27)
#define BIT_AON_APB_ANALOG_APB_EB                                BIT(26)
#define BIT_AON_APB_DEF_EB                                       BIT(25)
#define BIT_AON_APB_LVDS_PLL_DIV_EN                              BIT(24)
#define BIT_AON_APB_THM1_EB                                      BIT(23)
#define BIT_AON_APB_IO_APB_EB                                    BIT(22)
#define BIT_AON_APB_MBOX_EB                                      BIT(21)
#define BIT_AON_APB_DJTAG_EB                                     BIT(20)
#define BIT_AON_APB_RC100M_CAL_EB                                BIT(19)
#define BIT_AON_APB_CP_APB_EB                                    BIT(18)
#define BIT_AON_APB_MDAR_EB                                      BIT(17)
#define BIT_AON_APB_BIA_THM1_EB                                  BIT(16)
#define BIT_AON_APB_BIA_THM0_EB                                  BIT(15)
#define BIT_AON_APB_OSC_AON_TOP_EB                               BIT(14)
#define BIT_AON_APB_GSP_EMC_EB                                   BIT(13)
#define BIT_AON_APB_ZIP_EMC_EB                                   BIT(12)
#define BIT_AON_APB_DISP_EMC_EB                                  BIT(11)
#define BIT_AON_APB_AP_TMR2_EB                                   BIT(10)
#define BIT_AON_APB_AP_TMR1_EB                                   BIT(9)
#define BIT_AON_APB_AP_WDG_EB                                    BIT(8)
#define BIT_AON_APB_AVS_GPU1_EB                                  BIT(7)
#define BIT_AON_APB_AVS_GPU0_EB                                  BIT(6)
#define BIT_AON_APB_PROBE_EB                                     BIT(5)
#define BIT_AON_APB_AUX2_EB                                      BIT(4)
#define BIT_AON_APB_AUX1_EB                                      BIT(3)
#define BIT_AON_APB_AUX0_EB                                      BIT(2)
#define BIT_AON_APB_THM_EB                                       BIT(1)
#define BIT_AON_APB_PMU_EB                                       BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_APB_RST0
// Register Offset : 0x0008
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AP_HS_SPI_SOFT_RST                           BIT(31)
#define BIT_AON_APB_I2C_SOFT_RST                                 BIT(30)
#define BIT_AON_APB_BIA_DVFS_SOFT_RST                            BIT(29)
#define BIT_AON_APB_DBG_SERDES_SOFT_RST                          BIT(28)
#define BIT_AON_APB_DAP_MTX_SOFT_RST                             BIT(27)
#define BIT_AON_APB_MSPI1_SOFT_RST                               BIT(26)
#define BIT_AON_APB_MSPI0_SOFT_RST                               BIT(25)
#define BIT_AON_APB_SPLK_SOFT_RST                                BIT(24)
#define BIT_AON_APB_CKG_SOFT_RST                                 BIT(23)
#define BIT_AON_APB_PIN_SOFT_RST                                 BIT(22)
#define BIT_AON_APB_AP_INTC3_SOFT_RST                            BIT(21)
#define BIT_AON_APB_AP_INTC2_SOFT_RST                            BIT(20)
#define BIT_AON_APB_AP_INTC1_SOFT_RST                            BIT(19)
#define BIT_AON_APB_AP_INTC0_SOFT_RST                            BIT(18)
#define BIT_AON_APB_ADI_SOFT_RST                                 BIT(17)
#define BIT_AON_APB_EIC_SOFT_RST                                 BIT(15)
#define BIT_AON_APB_EFUSE_SOFT_RST                               BIT(14)
#define BIT_AON_APB_AP_TMR0_SOFT_RST                             BIT(12)
#define BIT_AON_APB_AON_TMR_SOFT_RST                             BIT(11)
#define BIT_AON_APB_AP_SYST_SOFT_RST                             BIT(10)
#define BIT_AON_APB_AON_SYST_SOFT_RST                            BIT(9)
#define BIT_AON_APB_KPD_SOFT_RST                                 BIT(8)
#define BIT_AON_APB_PWM3_SOFT_RST                                BIT(7)
#define BIT_AON_APB_PWM2_SOFT_RST                                BIT(6)
#define BIT_AON_APB_PWM1_SOFT_RST                                BIT(5)
#define BIT_AON_APB_PWM0_SOFT_RST                                BIT(4)
#define BIT_AON_APB_GPIO_SOFT_RST                                BIT(3)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_APB_RST1
// Register Offset : 0x000C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AVS_BIA_CPU1_SOFT_RST                        BIT(31)
#define BIT_AON_APB_AVS_BIA_CPU0_SOFT_RST                        BIT(30)
#define BIT_AON_APB_DEF_SLV_INT_SOFT_CLR                         BIT(29)
#define BIT_AON_APB_DEF_SOFT_RST                                 BIT(28)
#define BIT_AON_APB_MBOX_SOFT_RST                                BIT(24)
#define BIT_AON_APB_RC100M_CAL_SOFT_RST                          BIT(23)
#define BIT_AON_APB_ADC3_CAL_SOFT_RST                            BIT(16)
#define BIT_AON_APB_ADC2_CAL_SOFT_RST                            BIT(15)
#define BIT_AON_APB_ADC1_CAL_SOFT_RST                            BIT(14)
#define BIT_AON_APB_MDAR_SOFT_RST                                BIT(13)
#define BIT_AON_APB_BB_CAL_SOFT_RST                              BIT(11)
#define BIT_AON_APB_DCXO_LC_SOFT_RST                             BIT(10)
#define BIT_AON_APB_AP_TMR2_SOFT_RST                             BIT(9)
#define BIT_AON_APB_AP_TMR1_SOFT_RST                             BIT(8)
#define BIT_AON_APB_AP_WDG_SOFT_RST                              BIT(7)
#define BIT_AON_APB_BIA_THM1_SOFT_RST                            BIT(6)
#define BIT_AON_APB_BIA_THM0_SOFT_RST                            BIT(5)
#define BIT_AON_APB_THM1_SOFT_RST                                BIT(4)
#define BIT_AON_APB_GPU_THMA_SOFT_RST                            BIT(3)
#define BIT_AON_APB_ARM_THMA_SOFT_RST                            BIT(2)
#define BIT_AON_APB_THM_SOFT_RST                                 BIT(1)
#define BIT_AON_APB_PMU_SOFT_RST                                 BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_APB_RTC_EB
// Register Offset : 0x0010
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_RTCCNT_RTCDV10_EB                            BIT(27)
#define BIT_AON_APB_GPU_TS_EB                                    BIT(24)
#define BIT_AON_APB_AVS_GPU1_RTC_EB                              BIT(23)
#define BIT_AON_APB_AVS_GPU0_RTC_EB                              BIT(22)
#define BIT_AON_APB_AVS_BIA_CPU0_RTC_EB                          BIT(21)
#define BIT_AON_APB_AVS_BIA_CPU1_RTC_EB                          BIT(20)
#define BIT_AON_APB_BB_CAL_RTC_EB                                BIT(18)
#define BIT_AON_APB_DCXO_LC_RTC_EB                               BIT(17)
#define BIT_AON_APB_AP_TMR2_RTC_EB                               BIT(16)
#define BIT_AON_APB_AP_TMR1_RTC_EB                               BIT(15)
#define BIT_AON_APB_GPU_THMA_RTC_AUTO_EN                         BIT(14)
#define BIT_AON_APB_ARM_THMA_RTC_AUTO_EN                         BIT(13)
#define BIT_AON_APB_GPU_THMA_RTC_EB                              BIT(12)
#define BIT_AON_APB_ARM_THMA_RTC_EB                              BIT(11)
#define BIT_AON_APB_THM_RTC_EB                                   BIT(10)
#define BIT_AON_APB_AP_WDG_RTC_EB                                BIT(9)
#define BIT_AON_APB_EIC_RTCDV5_EB                                BIT(7)
#define BIT_AON_APB_EIC_RTC_EB                                   BIT(6)
#define BIT_AON_APB_AP_TMR0_RTC_EB                               BIT(5)
#define BIT_AON_APB_AON_TMR_RTC_EB                               BIT(4)
#define BIT_AON_APB_AP_SYST_RTC_EB                               BIT(3)
#define BIT_AON_APB_AON_SYST_RTC_EB                              BIT(2)
#define BIT_AON_APB_KPD_RTC_EB                                   BIT(1)
#define BIT_AON_APB_ARCH_RTC_EB                                  BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_PWR_CTRL
// Register Offset : 0x0024
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_EFUSE_BIST_PWR_ON                            BIT(3)
#define BIT_AON_APB_EFUSE_PWON_RD_END_FLAG                       BIT(2)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_TS_CFG
// Register Offset : 0x0028
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_DBG_TRACE_CTRL_EN                            BIT(16)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_BOOT_MODE
// Register Offset : 0x002C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PTEST_FUNC_ATSPEED_SEL                       BIT(8)
#define BIT_AON_APB_PTEST_FUNC_MODE                              BIT(7)
#define BIT_AON_APB_FUNCTST_DMA_EB                               BIT(6)
#define BIT_AON_APB_PTEST_BIST_MODE                              BIT(5)
#define BIT_AON_APB_USB_DLOAD_EN                                 BIT(4)
#define BIT_AON_APB_ARM_BOOT_MD3                                 BIT(3)
#define BIT_AON_APB_ARM_BOOT_MD2                                 BIT(2)
#define BIT_AON_APB_ARM_BOOT_MD1                                 BIT(1)
#define BIT_AON_APB_ARM_BOOT_MD0                                 BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_BB_BG_CTRL
// Register Offset : 0x0030
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_BB_BG_AUTO_PD_EN                             BIT(3)
#define BIT_AON_APB_BB_BG_SLP_PD_EN                              BIT(2)
#define BIT_AON_APB_BB_BG_FORCE_ON                               BIT(1)
#define BIT_AON_APB_BB_BG_FORCE_PD                               BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_CP_ARM_JTAG_CTRL
// Register Offset : 0x0034
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_CP_ARM_JTAG_PIN_SEL(x)                       (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_DCXO_LC_REG0
// Register Offset : 0x003C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_DCXO_LC_FLAG                                 BIT(8)
#define BIT_AON_APB_DCXO_LC_FLAG_CLR                             BIT(1)
#define BIT_AON_APB_DCXO_LC_CNT_CLR                              BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_DCXO_LC_REG1
// Register Offset : 0x0040
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_DCXO_LC_CNT(x)                               (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AGCP_BOOT_PROT
// Register Offset : 0x0078
// Description     : protect register for AGCP
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AGCP_BOOTCTRL_PROT                           BIT(31)
#define BIT_AON_APB_AGCP_REG_PROT_VAL(x)                         (((x) & 0xFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON_REG_PROT
// Register Offset : 0x007C
// Description     : protect register for LDSP
---------------------------------------------------------------------------*/

#define BIT_AON_APB_LDSP_CTRL_PROT                               BIT(31)
#define BIT_AON_APB_REG_PROT_VAL(x)                              (((x) & 0xFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_DAP_DJTAG_SEL
// Register Offset : 0x0084
// Description     : DAP_DJTAG_SEL
---------------------------------------------------------------------------*/

#define BIT_AON_APB_DAP_DBGPWRUP_SOFT_EN                         BIT(2)
#define BIT_AON_APB_DAP_SYSPWRUP_SOFT_EN                         BIT(1)
#define BIT_AON_APB_DAP_DJTAG_EN                                 BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_USER_RSV_FLAG1
// Register Offset : 0x0088
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_USER_RSV_FLAG1_B1                            BIT(1)
#define BIT_AON_APB_USER_RSV_FLAG1_B0                            BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON_CGM_CFG
// Register Offset : 0x0098
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_EMC_DDR1_CKG_SEL_CURRENT(x)                  (((x) & 0x7F) << 24)
#define BIT_AON_APB_CLK_DMC1_SEL_HW_EN                           BIT(23)
#define BIT_AON_APB_CLK_DMC1_1X_DIV                              BIT(22)
#define BIT_AON_APB_CLK_DMC1_2X_DIV(x)                           (((x) & 0xF) << 18)
#define BIT_AON_APB_CLK_DMC1_SEL(x)                              (((x) & 0x3) << 16)
#define BIT_AON_APB_EMC_DDR0_CKG_SEL_CURRENT(x)                  (((x) & 0x7F) << 8)
#define BIT_AON_APB_CLK_DMC0_SEL_HW_EN                           BIT(7)
#define BIT_AON_APB_CLK_DMC0_1X_DIV                              BIT(6)
#define BIT_AON_APB_CLK_DMC0_2X_DIV(x)                           (((x) & 0xF) << 2)
#define BIT_AON_APB_CLK_DMC0_SEL(x)                              (((x) & 0x3))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_VSP_FIFO_CH0_1
// Register Offset : 0x00A0
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_VSP_FIFO_AF_LVL_AW_CH0(x)                    (((x) & 0xF) << 28)
#define BIT_AON_APB_VSP_FIFO_AE_LVL_AW_CH0(x)                    (((x) & 0xF) << 24)
#define BIT_AON_APB_VSP_FIFO_AF_LVL_W_CH0(x)                     (((x) & 0x1F) << 19)
#define BIT_AON_APB_VSP_FIFO_AE_LVL_W_CH0(x)                     (((x) & 0x1F) << 14)
#define BIT_AON_APB_VSP_FIFO_AF_LVL_B_CH0(x)                     (((x) & 0x1F) << 9)
#define BIT_AON_APB_VSP_FIFO_AE_LVL_B_CH0(x)                     (((x) & 0x1F) << 4)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_VSP_FIFO_CH0_2
// Register Offset : 0x00A4
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_VSP_FIFO_AF_LVL_R_CH0(x)                     (((x) & 0x1F) << 27)
#define BIT_AON_APB_VSP_FIFO_AE_LVL_R_CH0(x)                     (((x) & 0x1F) << 22)
#define BIT_AON_APB_BADDTION_S_VSP_PUB0(x)                       (((x) & 0xF) << 12)
#define BIT_AON_APB_RADDTION_S_VSP_PUB0(x)                       (((x) & 0xF) << 8)
#define BIT_AON_APB_VSP_FIFO_AF_LVL_AR_CH0(x)                    (((x) & 0xF) << 4)
#define BIT_AON_APB_VSP_FIFO_AE_LVL_AR_CH0(x)                    (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_VSP_FIFO_CH1_1
// Register Offset : 0x00AC
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_VSP_FIFO_AF_LVL_AW_CH1(x)                    (((x) & 0xF) << 28)
#define BIT_AON_APB_VSP_FIFO_AE_LVL_AW_CH1(x)                    (((x) & 0xF) << 24)
#define BIT_AON_APB_VSP_FIFO_AF_LVL_W_CH1(x)                     (((x) & 0x1F) << 19)
#define BIT_AON_APB_VSP_FIFO_AE_LVL_W_CH1(x)                     (((x) & 0x1F) << 14)
#define BIT_AON_APB_VSP_FIFO_AF_LVL_B_CH1(x)                     (((x) & 0x1F) << 9)
#define BIT_AON_APB_VSP_FIFO_AE_LVL_B_CH1(x)                     (((x) & 0x1F) << 4)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_VSP_FIFO_CH1_2
// Register Offset : 0x00B0
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_VSP_FIFO_AF_LVL_R_CH1(x)                     (((x) & 0x1F) << 27)
#define BIT_AON_APB_VSP_FIFO_AE_LVL_R_CH1(x)                     (((x) & 0x1F) << 22)
#define BIT_AON_APB_BADDTION_S_VSP_PUB1(x)                       (((x) & 0xF) << 12)
#define BIT_AON_APB_RADDTION_S_VSP_PUB1(x)                       (((x) & 0xF) << 8)
#define BIT_AON_APB_VSP_FIFO_AF_LVL_AR_CH1(x)                    (((x) & 0xF) << 4)
#define BIT_AON_APB_VSP_FIFO_AE_LVL_AR_CH1(x)                    (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_CAM_FIFO_CH0_1
// Register Offset : 0x00B8
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_CAM_FIFO_AF_LVL_AW_CH0(x)                    (((x) & 0xF) << 28)
#define BIT_AON_APB_CAM_FIFO_AE_LVL_AW_CH0(x)                    (((x) & 0xF) << 24)
#define BIT_AON_APB_CAM_FIFO_AF_LVL_W_CH0(x)                     (((x) & 0x1F) << 19)
#define BIT_AON_APB_CAM_FIFO_AE_LVL_W_CH0(x)                     (((x) & 0x1F) << 14)
#define BIT_AON_APB_CAM_FIFO_AF_LVL_B_CH0(x)                     (((x) & 0x1F) << 9)
#define BIT_AON_APB_CAM_FIFO_AE_LVL_B_CH0(x)                     (((x) & 0x1F) << 4)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_CAM_FIFO_CH0_2
// Register Offset : 0x00BC
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_CAM_FIFO_AF_LVL_R_CH0(x)                     (((x) & 0x1F) << 27)
#define BIT_AON_APB_CAM_FIFO_AE_LVL_R_CH0(x)                     (((x) & 0x1F) << 22)
#define BIT_AON_APB_BADDTION_S_CAM_PUB0(x)                       (((x) & 0xF) << 12)
#define BIT_AON_APB_RADDTION_S_CAM_PUB0(x)                       (((x) & 0xF) << 8)
#define BIT_AON_APB_CAM_FIFO_AF_LVL_AR_CH0(x)                    (((x) & 0xF) << 4)
#define BIT_AON_APB_CAM_FIFO_AE_LVL_AR_CH0(x)                    (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_CAM_FIFO_CH1_1
// Register Offset : 0x00C0
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_CAM_FIFO_AF_LVL_AW_CH1(x)                    (((x) & 0xF) << 28)
#define BIT_AON_APB_CAM_FIFO_AE_LVL_AW_CH1(x)                    (((x) & 0xF) << 24)
#define BIT_AON_APB_CAM_FIFO_AF_LVL_W_CH1(x)                     (((x) & 0x1F) << 19)
#define BIT_AON_APB_CAM_FIFO_AE_LVL_W_CH1(x)                     (((x) & 0x1F) << 14)
#define BIT_AON_APB_CAM_FIFO_AF_LVL_B_CH1(x)                     (((x) & 0x1F) << 9)
#define BIT_AON_APB_CAM_FIFO_AE_LVL_B_CH1(x)                     (((x) & 0x1F) << 4)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_CAM_FIFO_CH1_2
// Register Offset : 0x00C4
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_CAM_FIFO_AF_LVL_R_CH1(x)                     (((x) & 0x1F) << 27)
#define BIT_AON_APB_CAM_FIFO_AE_LVL_R_CH1(x)                     (((x) & 0x1F) << 22)
#define BIT_AON_APB_BADDTION_S_CAM_PUB1(x)                       (((x) & 0xF) << 12)
#define BIT_AON_APB_RADDTION_S_CAM_PUB1(x)                       (((x) & 0xF) << 8)
#define BIT_AON_APB_CAM_FIFO_AF_LVL_AR_CH1(x)                    (((x) & 0xF) << 4)
#define BIT_AON_APB_CAM_FIFO_AE_LVL_AR_CH1(x)                    (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_GPU_FIFO_CH0_1
// Register Offset : 0x00CC
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_GPU_FIFO_AE_LVL_AW_CH0(x)                    (((x) & 0xF) << 28)
#define BIT_AON_APB_GPU_FIFO_AF_LVL_W_CH0(x)                     (((x) & 0x1F) << 23)
#define BIT_AON_APB_GPU_FIFO_AE_LVL_W_CH0(x)                     (((x) & 0x1F) << 18)
#define BIT_AON_APB_GPU_FIFO_AF_LVL_B_CH0(x)                     (((x) & 0x1F) << 13)
#define BIT_AON_APB_GPU_FIFO_AE_LVL_B_CH0(x)                     (((x) & 0x1F) << 8)
#define BIT_AON_APB_GPU_FIFO_AF_LVL_AR_CH0(x)                    (((x) & 0xF) << 4)
#define BIT_AON_APB_GPU_FIFO_AE_LVL_AR_CH0(x)                    (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_GPU_FIFO_CH0_2
// Register Offset : 0x00D0
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_BADDTION_S_GPU_PUB0(x)                       (((x) & 0xF) << 20)
#define BIT_AON_APB_RADDTION_S_GPU_PUB0(x)                       (((x) & 0xF) << 16)
#define BIT_AON_APB_GPU_FIFO_AF_LVL_AW_CH0(x)                    (((x) & 0xF) << 12)
#define BIT_AON_APB_GPU_FIFO_AF_LVL_R_CH0(x)                     (((x) & 0x1F) << 5)
#define BIT_AON_APB_GPU_FIFO_AE_LVL_R_CH0(x)                     (((x) & 0x1F))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_GPU_FIFO_CH1_1
// Register Offset : 0x00D4
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_GPU_FIFO_AE_LVL_AW_CH1(x)                    (((x) & 0xF) << 28)
#define BIT_AON_APB_GPU_FIFO_AF_LVL_W_CH1(x)                     (((x) & 0x1F) << 23)
#define BIT_AON_APB_GPU_FIFO_AE_LVL_W_CH1(x)                     (((x) & 0x1F) << 18)
#define BIT_AON_APB_GPU_FIFO_AF_LVL_B_CH1(x)                     (((x) & 0x1F) << 13)
#define BIT_AON_APB_GPU_FIFO_AE_LVL_B_CH1(x)                     (((x) & 0x1F) << 8)
#define BIT_AON_APB_GPU_FIFO_AF_LVL_AR_CH1(x)                    (((x) & 0xF) << 4)
#define BIT_AON_APB_GPU_FIFO_AE_LVL_AR_CH1(x)                    (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_GPU_FIFO_CH1_2
// Register Offset : 0x00D8
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_BADDTION_S_GPU_PUB1(x)                       (((x) & 0xF) << 20)
#define BIT_AON_APB_RADDTION_S_GPU_PUB1(x)                       (((x) & 0xF) << 16)
#define BIT_AON_APB_GPU_FIFO_AF_LVL_AW_CH1(x)                    (((x) & 0xF) << 12)
#define BIT_AON_APB_GPU_FIFO_AF_LVL_R_CH1(x)                     (((x) & 0x1F) << 5)
#define BIT_AON_APB_GPU_FIFO_AE_LVL_R_CH1(x)                     (((x) & 0x1F))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AP_FIFO_CH_1
// Register Offset : 0x00E0
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AP_FIFO_AF_LVL_AW_CH(x)                      (((x) & 0xF) << 28)
#define BIT_AON_APB_AP_FIFO_AE_LVL_AW_CH(x)                      (((x) & 0xF) << 24)
#define BIT_AON_APB_AP_FIFO_AF_LVL_W_CH(x)                       (((x) & 0xF) << 20)
#define BIT_AON_APB_AP_FIFO_AE_LVL_W_CH(x)                       (((x) & 0xF) << 16)
#define BIT_AON_APB_AP_FIFO_AF_LVL_B_CH(x)                       (((x) & 0xF) << 12)
#define BIT_AON_APB_AP_FIFO_AE_LVL_B_CH(x)                       (((x) & 0xF) << 8)
#define BIT_AON_APB_AP_FIFO_AF_LVL_AR_CH(x)                      (((x) & 0xF) << 4)
#define BIT_AON_APB_AP_FIFO_AE_LVL_AR_CH(x)                      (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AP_FIFO_CH_2
// Register Offset : 0x00E4
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AP_FIFO_AF_LVL_R_CH(x)                       (((x) & 0xF) << 28)
#define BIT_AON_APB_AP_FIFO_AE_LVL_R_CH(x)                       (((x) & 0xF) << 24)
#define BIT_AON_APB_BADDTION_S_AP_PUB1(x)                        (((x) & 0xF) << 20)
#define BIT_AON_APB_RADDTION_S_AP_PUB1(x)                        (((x) & 0xF) << 16)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_PUBCP_FIFO_CH_1
// Register Offset : 0x00E8
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PUBCP_FIFO_AF_LVL_AW_CH(x)                   (((x) & 0xF) << 28)
#define BIT_AON_APB_PUBCP_FIFO_AE_LVL_AW_CH(x)                   (((x) & 0xF) << 24)
#define BIT_AON_APB_PUBCP_FIFO_AF_LVL_W_CH(x)                    (((x) & 0x1F) << 19)
#define BIT_AON_APB_PUBCP_FIFO_AE_LVL_W_CH(x)                    (((x) & 0x1F) << 14)
#define BIT_AON_APB_PUBCP_FIFO_AF_LVL_B_CH(x)                    (((x) & 0x1F) << 9)
#define BIT_AON_APB_PUBCP_FIFO_AE_LVL_B_CH(x)                    (((x) & 0x1F) << 4)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_PUBCP_FIFO_CH_2
// Register Offset : 0x00EC
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PUBCP_FIFO_AF_LVL_R_CH(x)                    (((x) & 0x1F) << 27)
#define BIT_AON_APB_PUBCP_FIFO_AE_LVL_R_CH(x)                    (((x) & 0x1F) << 22)
#define BIT_AON_APB_BADDTION_S_PUBCP_PUB0(x)                     (((x) & 0xFF) << 12)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_PUBCP_FIFO_CH_3
// Register Offset : 0x00F0
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_RADDTION_S_PUBCP_PUB0(x)                     (((x) & 0xFF) << 8)
#define BIT_AON_APB_PUBCP_FIFO_AF_LVL_AR_CH(x)                   (((x) & 0xF) << 4)
#define BIT_AON_APB_PUBCP_FIFO_AE_LVL_AR_CH(x)                   (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON_CHIP_ID0
// Register Offset : 0x00F8
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AON_CHIP_ID0(x)                              (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON_CHIP_ID1
// Register Offset : 0x00FC
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AON_CHIP_ID1(x)                              (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_CCIR_RCVR_CFG
// Register Offset : 0x0100
// Description     : APB clock control
---------------------------------------------------------------------------*/

#define BIT_AON_APB_ANALOG_PLL_RSV(x)                            (((x) & 0xFF) << 16)
#define BIT_AON_APB_ANALOG_TESTMUX(x)                            (((x) & 0xFF) << 8)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_PLL_BG_CFG
// Register Offset : 0x0108
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PLL_BG_RSV(x)                                (((x) & 0x3F))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_BIA_R_FIFO_CH_1
// Register Offset : 0x0110
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_BIA_R_FIFO_AF_LVL_AW_CH(x)                   (((x) & 0xF) << 28)
#define BIT_AON_APB_BIA_R_FIFO_AE_LVL_AW_CH(x)                   (((x) & 0xF) << 24)
#define BIT_AON_APB_BIA_R_FIFO_AF_LVL_W_CH(x)                    (((x) & 0xF) << 20)
#define BIT_AON_APB_BIA_R_FIFO_AE_LVL_W_CH(x)                    (((x) & 0xF) << 16)
#define BIT_AON_APB_BIA_R_FIFO_AF_LVL_B_CH(x)                    (((x) & 0xF) << 12)
#define BIT_AON_APB_BIA_R_FIFO_AE_LVL_B_CH(x)                    (((x) & 0xF) << 8)
#define BIT_AON_APB_BIA_R_FIFO_AF_LVL_AR_CH(x)                   (((x) & 0xF) << 4)
#define BIT_AON_APB_BIA_R_FIFO_AE_LVL_AR_CH(x)                   (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_BIA_R_FIFO_CH_2
// Register Offset : 0x0114
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_BIA_R_FIFO_AF_LVL_R_CH(x)                    (((x) & 0xF) << 28)
#define BIT_AON_APB_BIA_R_FIFO_AE_LVL_R_CH(x)                    (((x) & 0xF) << 24)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_BIA_W_FIFO_CH_1
// Register Offset : 0x0118
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_BIA_W_FIFO_AF_LVL_AW_CH(x)                   (((x) & 0xF) << 28)
#define BIT_AON_APB_BIA_W_FIFO_AE_LVL_AW_CH(x)                   (((x) & 0xF) << 24)
#define BIT_AON_APB_BIA_W_FIFO_AF_LVL_W_CH(x)                    (((x) & 0xF) << 20)
#define BIT_AON_APB_BIA_W_FIFO_AE_LVL_W_CH(x)                    (((x) & 0xF) << 16)
#define BIT_AON_APB_BIA_W_FIFO_AF_LVL_B_CH(x)                    (((x) & 0xF) << 12)
#define BIT_AON_APB_BIA_W_FIFO_AE_LVL_B_CH(x)                    (((x) & 0xF) << 8)
#define BIT_AON_APB_BIA_W_FIFO_AF_LVL_AR_CH(x)                   (((x) & 0xF) << 4)
#define BIT_AON_APB_BIA_W_FIFO_AE_LVL_AR_CH(x)                   (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_BIA_W_FIFO_CH_2
// Register Offset : 0x011C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_BIA_W_FIFO_AF_LVL_R_CH(x)                    (((x) & 0xF) << 28)
#define BIT_AON_APB_BIA_W_FIFO_AE_LVL_R_CH(x)                    (((x) & 0xF) << 24)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_SP_CFG_BUS
// Register Offset : 0x0124
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_ARM7_AHB_CLK_SOFT_EN                         BIT(7)
#define BIT_AON_APB_CM4_SLEEPING_STAT                            BIT(6)
#define BIT_AON_APB_CM4_LOCKUP_STAT                              BIT(5)
#define BIT_AON_APB_CM4_SOFT_MPUDIS                              BIT(4)
#define BIT_AON_APB_MMTX_SLEEP_CM4_PUB_WR                        BIT(3)
#define BIT_AON_APB_MMTX_SLEEP_CM4_PUB_RD                        BIT(2)
#define BIT_AON_APB_INT_REQ_CM4_SOFT                             BIT(1)
#define BIT_AON_APB_SP_CFG_BUS_SLEEP                             BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_RTC4M_0_CFG
// Register Offset : 0x0128
// Description     : 
---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_APB_RST2
// Register Offset : 0x0130
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_DJTAG_PUB1_SOFT_RST                          BIT(18)
#define BIT_AON_APB_DJTAG_PUB0_SOFT_RST                          BIT(17)
#define BIT_AON_APB_DJTAG_AON_SOFT_RST                           BIT(16)
#define BIT_AON_APB_DJTAG_AGCP_SOFT_RST                          BIT(10)
#define BIT_AON_APB_DJTAG_WTLCP_SOFT_RST                         BIT(9)
#define BIT_AON_APB_DJTAG_PUBCP_SOFT_RST                         BIT(8)
#define BIT_AON_APB_DJTAG_DISP_SOFT_RST                          BIT(5)
#define BIT_AON_APB_DJTAG_CAM_SOFT_RST                           BIT(4)
#define BIT_AON_APB_DJTAG_VSP_SOFT_RST                           BIT(3)
#define BIT_AON_APB_DJTAG_GPU_SOFT_RST                           BIT(2)
#define BIT_AON_APB_DJTAG_AP_SOFT_RST                            BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_RC100M_CFG
// Register Offset : 0x0134
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_RC100M_DIV(x)                                (((x) & 0x3F) << 7)
#define BIT_AON_APB_RC100M_CAL_DONE                              BIT(6)
#define BIT_AON_APB_RC100M_CAL_START                             BIT(5)
#define BIT_AON_APB_RC100M_ANA_SOFT_RST                          BIT(4)
#define BIT_AON_APB_RC100M_FORCE_EN                              BIT(1)
#define BIT_AON_APB_RC100M_AUTO_GATE_EN                          BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_CGM_REG1
// Register Offset : 0x0138
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_LTE_PCCSCC_RFTI_CLK_SW_CFG(x)                (((x) & 0xFF) << 24)
#define BIT_AON_APB_LPLL1_CLKOUT_SW                              BIT(23)
#define BIT_AON_APB_LPLL0_CLKOUT_SW                              BIT(22)
#define BIT_AON_APB_CGM_SP_TMR_EN                                BIT(19)
#define BIT_AON_APB_CGM_DET_32K_EN                               BIT(18)
#define BIT_AON_APB_CGM_DEBOUNCE_EN                              BIT(17)
#define BIT_AON_APB_CGM_RC100M_FDK_EN                            BIT(16)
#define BIT_AON_APB_CGM_RC100M_REF_EN                            BIT(15)
#define BIT_AON_APB_CGM_USB2_REF_EN                              BIT(14)
#define BIT_AON_APB_CGM_USB3_SUSPEND_EN                          BIT(13)
#define BIT_AON_APB_CGM_USB3_REF_EN                              BIT(12)
#define BIT_AON_APB_CGM_DPHY1_REF_EN                             BIT(11)
#define BIT_AON_APB_CGM_DPHY0_REF_EN                             BIT(10)
#define BIT_AON_APB_CGM_DJTAG_TCK_EN                             BIT(9)
#define BIT_AON_APB_CGM_TS_EN                                    BIT(7)
#define BIT_AON_APB_CGM_MDAR_CHK_EN                              BIT(6)
#define BIT_AON_APB_CGM_LVDSRF_CALI_EN                           BIT(5)
#define BIT_AON_APB_CGM_RFTI2_LTH_EN                             BIT(4)
#define BIT_AON_APB_CGM_RFTI2_XO_EN                              BIT(3)
#define BIT_AON_APB_CGM_RFTI1_LTH_EN                             BIT(2)
#define BIT_AON_APB_CGM_RFTI1_XO_EN                              BIT(1)
#define BIT_AON_APB_CGM_RFTI_SBI_EN                              BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_CGM_CLK_TOP_REG1
// Register Offset : 0x013C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_CGM_WCDMA_ICI_EN                             BIT(27)
#define BIT_AON_APB_SOFT_DDR0_CKG_1X_EN                          BIT(26)
#define BIT_AON_APB_SOFT_DDR1_CKG_1X_EN                          BIT(25)
#define BIT_AON_APB_SOFT_DDR1_DATA_RET                           BIT(24)
#define BIT_AON_APB_SOFT_DDR0_DATA_RET                           BIT(23)
#define BIT_AON_APB_LIGHT_SLEEP_DDR1_DATA_RET_EN                 BIT(22)
#define BIT_AON_APB_LIGHT_SLEEP_DDR0_DATA_RET_EN                 BIT(21)
#define BIT_AON_APB_EMC1_CKG_SEL_LOAD                            BIT(20)
#define BIT_AON_APB_EMC0_CKG_SEL_LOAD                            BIT(19)
#define BIT_AON_APB_DMC_DDR1_1X_EN                               BIT(18)
#define BIT_AON_APB_DMC_DDR1_2X_EN                               BIT(17)
#define BIT_AON_APB_DMC_DDR0_1X_EN                               BIT(16)
#define BIT_AON_APB_DMC_DDR0_2X_EN                               BIT(15)
#define BIT_AON_APB_CGM_W_SYS_EN                                 BIT(14)
#define BIT_AON_APB_CGM_W_ZIF_EN                                 BIT(13)
#define BIT_AON_APB_CGM_RFTI_RX_WD_EN                            BIT(12)
#define BIT_AON_APB_CGM_RFTI_TX_WD_EN                            BIT(11)
#define BIT_AON_APB_CGM_WCDMA_EN                                 BIT(10)
#define BIT_AON_APB_CGM_EMMC_2X_EN                               BIT(9)
#define BIT_AON_APB_CGM_EMMC_1X_EN                               BIT(8)
#define BIT_AON_APB_CGM_SDIO2_1X_EN                              BIT(7)
#define BIT_AON_APB_CGM_SDIO2_2X_EN                              BIT(6)
#define BIT_AON_APB_CGM_SDIO1_1X_EN                              BIT(5)
#define BIT_AON_APB_CGM_SDIO1_2X_EN                              BIT(4)
#define BIT_AON_APB_CGM_SDIO0_1X_EN                              BIT(3)
#define BIT_AON_APB_CGM_SDIO0_2X_EN                              BIT(2)
#define BIT_AON_APB_CGM_AP_AXI_EN                                BIT(1)
#define BIT_AON_APB_CGM_CSSYS_EN                                 BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AGCP_DSP_CTRL0
// Register Offset : 0x0140
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AGCP_DSP_BOOT_VECTOR(x)                      (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AGCP_DSP_CTRL1
// Register Offset : 0x0144
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AGCP_STCK_DSP                                BIT(13)
#define BIT_AON_APB_AGCP_STMS_DSP                                BIT(12)
#define BIT_AON_APB_AGCP_STDO_DSP                                BIT(11)
#define BIT_AON_APB_AGCP_STDI_DSP                                BIT(10)
#define BIT_AON_APB_AGCP_STRTCK_DSP                              BIT(9)
#define BIT_AON_APB_AGCP_SW_JTAG_ENA_DSP                         BIT(8)
#define BIT_AON_APB_AGCP_DSP_EXTERNAL_WAIT                       BIT(1)
#define BIT_AON_APB_AGCP_DSP_BOOT                                BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AGCP_SOFT_RESET
// Register Offset : 0x0148
// Description     : 
---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AGCP_CTRL
// Register Offset : 0x014C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AGCP_FRC_CLK_DSP_EN                          BIT(1)
#define BIT_AON_APB_AGCP_TOP_ACCESS_EN                           BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_WTLCP_LDSP_CTRL0
// Register Offset : 0x0150
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_WTLCP_LDSP_BOOT_VECTOR(x)                    (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_WTLCP_LDSP_CTRL1
// Register Offset : 0x0154
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_WTLCP_STCK_LDSP                              BIT(13)
#define BIT_AON_APB_WTLCP_STMS_LDSP                              BIT(12)
#define BIT_AON_APB_WTLCP_STDO_LDSP                              BIT(11)
#define BIT_AON_APB_WTLCP_STDI_LDSP                              BIT(10)
#define BIT_AON_APB_WTLCP_STRTCK_LDSP                            BIT(9)
#define BIT_AON_APB_WTLCP_SW_JTAG_ENA_LDSP                       BIT(8)
#define BIT_AON_APB_WTLCP_LDSP_EXTERNAL_WAIT                     BIT(1)
#define BIT_AON_APB_WTLCP_LDSP_BOOT                              BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_WTLCP_TDSP_CTRL0
// Register Offset : 0x0158
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_WTLCP_TDSP_BOOT_VECTOR(x)                    (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_WTLCP_TDSP_CTRL1
// Register Offset : 0x015C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_WTLCP_STCK_TDSP                              BIT(13)
#define BIT_AON_APB_WTLCP_STMS_TDSP                              BIT(12)
#define BIT_AON_APB_WTLCP_STDO_TDSP                              BIT(11)
#define BIT_AON_APB_WTLCP_STDI_TDSP                              BIT(10)
#define BIT_AON_APB_WTLCP_STRTCK_TDSP                            BIT(9)
#define BIT_AON_APB_WTLCP_SW_JTAG_ENA_TDSP                       BIT(8)
#define BIT_AON_APB_WTLCP_TDSP_EXTERNAL_WAIT                     BIT(1)
#define BIT_AON_APB_WTLCP_TDSP_BOOT                              BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_WTLCP_SOFT_RESET
// Register Offset : 0x0160
// Description     : 
---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_WTLCP_CTRL
// Register Offset : 0x0164
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_WTLCP_AON_FRC_WSYS_LT_STOP                   BIT(4)
#define BIT_AON_APB_WTLCP_AON_FRC_WSYS_STOP                      BIT(3)
#define BIT_AON_APB_WTLCP_DSP_DEEP_SLEEP_EN                      BIT(2)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_WTL_WCDMA_EB
// Register Offset : 0x0168
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_WTLCP_WCMDA_EB                               BIT(16)
#define BIT_AON_APB_WCDMA_AUTO_GATE_EN                           BIT(8)
#define BIT_AON_APB_WTLCP_WTLSYS_RFTI_TX_EB                      BIT(1)
#define BIT_AON_APB_WTLCP_WTLSYS_RFTI_RX_EB                      BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_PCP_AON_EB
// Register Offset : 0x0170
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PUBCP_SYST_RTC_EB                            BIT(11)
#define BIT_AON_APB_PUBCP_TMR_EB                                 BIT(10)
#define BIT_AON_APB_PUBCP_TMR_RTC_EB                             BIT(9)
#define BIT_AON_APB_PUBCP_SYST_EB                                BIT(8)
#define BIT_AON_APB_PUBCP_WDG_EB                                 BIT(7)
#define BIT_AON_APB_PUBCP_WDG_RTC_EB                             BIT(6)
#define BIT_AON_APB_PUBCP_ARCH_RTC_EB                            BIT(5)
#define BIT_AON_APB_PUBCP_EIC_EB                                 BIT(4)
#define BIT_AON_APB_PUBCP_EIC_RTCDV5_EB                          BIT(3)
#define BIT_AON_APB_PUBCP_EIC_RTC_EB                             BIT(2)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_PCP_SOFT_RST
// Register Offset : 0x0174
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_CP_SDIO2_SOFT_RST                            BIT(11)
#define BIT_AON_APB_PUBCP_CR5_DBG_SOFT_RST                       BIT(9)
#define BIT_AON_APB_PUBCP_CR5_ETM_SOFT_RST                       BIT(8)
#define BIT_AON_APB_PUBCP_CR5_MP_SOFT_RST                        BIT(7)
#define BIT_AON_APB_PUBCP_CR5_CS_DBG_SOFT_RST                    BIT(6)
#define BIT_AON_APB_PUBCP_TMR_SOFT_RST                           BIT(5)
#define BIT_AON_APB_PUBCP_SYST_SOFT_RST                          BIT(4)
#define BIT_AON_APB_PUBCP_WDG_SOFT_RST                           BIT(3)
#define BIT_AON_APB_PUBCP_EIC_SOFT_RST                           BIT(2)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_PUBCP_CTRL
// Register Offset : 0x0178
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PUBCP_CR5_STANDBYWFI_N                       BIT(12)
#define BIT_AON_APB_PUBCP_CR5_STANDBYWFE_N                       BIT(11)
#define BIT_AON_APB_PUBCP_CR5_CLKSTOPPED0_N                      BIT(10)
#define BIT_AON_APB_PUBCP_CR5_L2IDLE                             BIT(9)
#define BIT_AON_APB_PUBCP_CR5_VALIRQ0_N                          BIT(8)
#define BIT_AON_APB_PUBCP_CR5_VALFIQ0_N                          BIT(7)
#define BIT_AON_APB_PUBCP_CR5_STOP                               BIT(6)
#define BIT_AON_APB_PUBCP_CR5_CSYSACK_ATB                        BIT(5)
#define BIT_AON_APB_PUBCP_CR5_CACTIVE_ATB                        BIT(4)
#define BIT_AON_APB_PUBCP_CR5_CSSYNC_REQ                         BIT(3)
#define BIT_AON_APB_PUBCP_CR5_CSYSREQ_ATB                        BIT(2)
#define BIT_AON_APB_PUBCP_CR5_NODBGCLK                           BIT(1)
#define BIT_AON_APB_PUBCP_CR5_CFGEE                              BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_USB3_CTRL
// Register Offset : 0x0180
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_USB3_SUSPEND_EB                              BIT(31)
#define BIT_AON_APB_USB3_SOFT_RST                                BIT(30)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_DEBUG_U2PMU
// Register Offset : 0x0184
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_DEBUG_U2PMU(x)                               (((x) & 0x7FFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_DEBUG_U3PMU
// Register Offset : 0x0188
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_DEBUG_U3PMU(x)                               (((x) & 0x7FFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_FIREWALL_CLK_EN
// Register Offset : 0x0190
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AGCP_FIREWALL_CLK_EN                         BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_GPU_CTRL
// Register Offset : 0x0194
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_GPU_AVS_SOFT_RST                             BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_SUB_SYS_DEBUG_BUS_SEL_0
// Register Offset : 0x01C0
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_WTLCP_DEBUG_BUS_SELECT(x)                    (((x) & 0xF) << 28)
#define BIT_AON_APB_PUBCP_DEBUG_BUS_SELECT(x)                    (((x) & 0xF) << 24)
#define BIT_AON_APB_DISP_DEBUG_BUS_SELECT(x)                     (((x) & 0xF) << 20)
#define BIT_AON_APB_CAM_DEBUG_BUS_SELECT(x)                      (((x) & 0xF) << 16)
#define BIT_AON_APB_VSP_DEBUG_BUS_SELECT(x)                      (((x) & 0xF) << 12)
#define BIT_AON_APB_GPU_DEBUG_BUS_SELECT(x)                      (((x) & 0xF) << 8)
#define BIT_AON_APB_AP_DEBUG_BUS_SELECT(x)                       (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_SUB_SYS_DEBUG_BUS_SEL_1
// Register Offset : 0x01C4
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_MDAR_LVDSRF_DBG_SEL(x)                       (((x) & 0xF) << 20)
#define BIT_AON_APB_MDAR_DEBUG_BUS_SELECT(x)                     (((x) & 0xF) << 16)
#define BIT_AON_APB_PUB1_DEBUG_BUS_SELECT(x)                     (((x) & 0xF) << 12)
#define BIT_AON_APB_PUB0_DEBUG_BUS_SELECT(x)                     (((x) & 0xF) << 8)
#define BIT_AON_APB_AON_DEBUG_BUS_SELECT(x)                      (((x) & 0xF) << 4)
#define BIT_AON_APB_AGCP_DEBUG_BUS_SELECT(x)                     (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_GLB_DEBUG_BUS_SEL
// Register Offset : 0x01C8
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_DEBUG_SUBSYS_SELECT(x)                       (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_SOFT_RST_AON_ADD1
// Register Offset : 0x01CC
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_BSMTMR_SOFT_RST                              BIT(7)
#define BIT_AON_APB_RFTI2_LTH_SOFT_RST                           BIT(6)
#define BIT_AON_APB_RFTI1_LTH_SOFT_RST                           BIT(5)
#define BIT_AON_APB_CSSYS_SOFT_RST                               BIT(4)
#define BIT_AON_APB_ANLG_SOFT_RST                                BIT(3)
#define BIT_AON_APB_RFTI_SBI_SOFT_RST                            BIT(1)
#define BIT_AON_APB_LVDSRF_CALI_SOFT_RST                         BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_EB_AON_ADD1
// Register Offset : 0x01D0
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AON_APB_IDLE_EN                              BIT(2)
#define BIT_AON_APB_BSM_TMR_EB                                   BIT(1)
#define BIT_AON_APB_ANLG_EB                                      BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_DBG_DJTAG_CTRL
// Register Offset : 0x01D4
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_DBGSYS_CSSYS_STM_NSGUAREN                    BIT(16)
#define BIT_AON_APB_DJTAG_SRC_MUX_SEL                            BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_PUB_FC_CTRL
// Register Offset : 0x01E0
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PUB1_DMC_2X_CGM_SEL                          BIT(25)
#define BIT_AON_APB_PUB1_DMC_1X_CGM_SEL                          BIT(24)
#define BIT_AON_APB_PUB1_DFS_SWITCH_WAIT_TIME(x)                 (((x) & 0xFF) << 16)
#define BIT_AON_APB_PUB0_DMC_2X_CGM_SEL                          BIT(9)
#define BIT_AON_APB_PUB0_DMC_1X_CGM_SEL                          BIT(8)
#define BIT_AON_APB_PUB0_DFS_SWITCH_WAIT_TIME(x)                 (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_EMC_CKG_SEL
// Register Offset : 0x01E4
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_EMC_CKG_SEL_DEFAULT_PUB1(x)                  (((x) & 0x7F) << 7)
#define BIT_AON_APB_EMC_CKG_SEL_DEFAULT_PUB0(x)                  (((x) & 0x7F))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_ANA_PHY_CTRL0
// Register Offset : 0x01E8
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_CGM_CSI2_RXDFE_EN                            BIT(11)
#define BIT_AON_APB_CGM_CSI1_RXDFE_EN                            BIT(10)
#define BIT_AON_APB_CGM_CSI0_RXDFE_EN                            BIT(9)
#define BIT_AON_APB_BIA_PLL_MUX_1(x)                             (((x) & 0x3) << 7)
#define BIT_AON_APB_BIA_PLL_MUX_0(x)                             (((x) & 0x3) << 5)
#define BIT_AON_APB_R2G_CSI1_SEL(x)                              (((x) & 0x3) << 3)
#define BIT_AON_APB_R2G_CSI0_SEL(x)                              (((x) & 0x3) << 1)
#define BIT_AON_APB_R2G_DSI0_SEL                                 BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_GPU_DDR_CTRL
// Register Offset : 0x01EC
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_GPU_INTERLEAVE_MODE(x)                       (((x) & 0x3) << 5)
#define BIT_AON_APB_GPU_DDR1_DISABLE_ACCESS                      BIT(4)
#define BIT_AON_APB_GPU_DDR0_DISABLE_ACCESS                      BIT(3)
#define BIT_AON_APB_GPU_CHIP_DDR_CAPACITY_I(x)                   (((x) & 0x7))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_GPU_BOUDING_PIN
// Register Offset : 0x01F0
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_GPU_BOUDING_PIN(x)                           (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_SP_EIC_IN_SEL
// Register Offset : 0x01F4
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_SP_EIC_IN_7_SEL                              BIT(7)
#define BIT_AON_APB_SP_EIC_IN_6_SEL                              BIT(6)
#define BIT_AON_APB_SP_EIC_IN_5_SEL                              BIT(5)
#define BIT_AON_APB_SP_EIC_IN_4_SEL                              BIT(4)
#define BIT_AON_APB_SP_EIC_IN_3_SEL                              BIT(3)
#define BIT_AON_APB_SP_EIC_IN_2_SEL                              BIT(2)
#define BIT_AON_APB_SP_EIC_IN_1_SEL                              BIT(1)
#define BIT_AON_APB_SP_EIC_IN_0_SEL                              BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_BIA_WDG_WARM_RST_EN
// Register Offset : 0x01F8
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_BIA_WDG_WARM_RST_EN                          BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON_UFS_CTRL
// Register Offset : 0x0200
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_CGM_UFS_BUS_AUTO_SEL                         BIT(5)
#define BIT_AON_APB_UFS_DEVICE_SOFT_RST_N                        BIT(4)
#define BIT_AON_APB_UFS_CLK_MUX_SEL                              BIT(3)
#define BIT_AON_APB_AON_UFS_EB                                   BIT(2)
#define BIT_AON_APB_LP_CLK_T_SOFT_RST                            BIT(1)
#define BIT_AON_APB_AON_UFS_SYSCLK_EB                            BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_VSP_DDR_CTRL
// Register Offset : 0x0204
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_CHIP_DDR_CAPACITY_I_VSP(x)                   (((x) & 0x7) << 2)
#define BIT_AON_APB_DDR1_DISABLE_ACCESS_VSP                      BIT(1)
#define BIT_AON_APB_DDR0_DISABLE_ACCESS_VSP                      BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_CAM_DDR_CTRL
// Register Offset : 0x0208
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_CHIP_DDR_CAPACITY_I_CAM(x)                   (((x) & 0x7) << 2)
#define BIT_AON_APB_DDR1_DISABLE_ACCESS_CAM                      BIT(1)
#define BIT_AON_APB_DDR0_DISABLE_ACCESS_CAM                      BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_PRE_DBG_BUS_5_0_SEL
// Register Offset : 0x0214
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PRE_DBG_BUS5_SEL(x)                          (((x) & 0x1F) << 25)
#define BIT_AON_APB_PRE_DBG_BUS4_SEL(x)                          (((x) & 0x1F) << 20)
#define BIT_AON_APB_PRE_DBG_BUS3_SEL(x)                          (((x) & 0x1F) << 15)
#define BIT_AON_APB_PRE_DBG_BUS2_SEL(x)                          (((x) & 0x1F) << 10)
#define BIT_AON_APB_PRE_DBG_BUS1_SEL(x)                          (((x) & 0x1F) << 5)
#define BIT_AON_APB_PRE_DBG_BUS0_SEL(x)                          (((x) & 0x1F))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_PRE_DBG_BUS_11_6_SEL
// Register Offset : 0x0218
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PRE_DBG_BUS11_SEL(x)                         (((x) & 0x1F) << 25)
#define BIT_AON_APB_PRE_DBG_BUS10_SEL(x)                         (((x) & 0x1F) << 20)
#define BIT_AON_APB_PRE_DBG_BUS9_SEL(x)                          (((x) & 0x1F) << 15)
#define BIT_AON_APB_PRE_DBG_BUS8_SEL(x)                          (((x) & 0x1F) << 10)
#define BIT_AON_APB_PRE_DBG_BUS7_SEL(x)                          (((x) & 0x1F) << 5)
#define BIT_AON_APB_PRE_DBG_BUS6_SEL(x)                          (((x) & 0x1F))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_PRE_DBG_BUS_17_12_SEL
// Register Offset : 0x021C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PRE_DBG_BUS17_SEL(x)                         (((x) & 0x1F) << 25)
#define BIT_AON_APB_PRE_DBG_BUS16_SEL(x)                         (((x) & 0x1F) << 20)
#define BIT_AON_APB_PRE_DBG_BUS15_SEL(x)                         (((x) & 0x1F) << 15)
#define BIT_AON_APB_PRE_DBG_BUS14_SEL(x)                         (((x) & 0x1F) << 10)
#define BIT_AON_APB_PRE_DBG_BUS13_SEL(x)                         (((x) & 0x1F) << 5)
#define BIT_AON_APB_PRE_DBG_BUS12_SEL(x)                         (((x) & 0x1F))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_PRE_DBG_BUS_23_18_SEL
// Register Offset : 0x0220
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PRE_DBG_BUS23_SEL(x)                         (((x) & 0x1F) << 25)
#define BIT_AON_APB_PRE_DBG_BUS22_SEL(x)                         (((x) & 0x1F) << 20)
#define BIT_AON_APB_PRE_DBG_BUS21_SEL(x)                         (((x) & 0x1F) << 15)
#define BIT_AON_APB_PRE_DBG_BUS20_SEL(x)                         (((x) & 0x1F) << 10)
#define BIT_AON_APB_PRE_DBG_BUS19_SEL(x)                         (((x) & 0x1F) << 5)
#define BIT_AON_APB_PRE_DBG_BUS18_SEL(x)                         (((x) & 0x1F))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_PRE_DBG_BUS_29_24_SEL
// Register Offset : 0x0224
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PRE_DBG_BUS29_SEL(x)                         (((x) & 0x1F) << 25)
#define BIT_AON_APB_PRE_DBG_BUS28_SEL(x)                         (((x) & 0x1F) << 20)
#define BIT_AON_APB_PRE_DBG_BUS27_SEL(x)                         (((x) & 0x1F) << 15)
#define BIT_AON_APB_PRE_DBG_BUS26_SEL(x)                         (((x) & 0x1F) << 10)
#define BIT_AON_APB_PRE_DBG_BUS25_SEL(x)                         (((x) & 0x1F) << 5)
#define BIT_AON_APB_PRE_DBG_BUS24_SEL(x)                         (((x) & 0x1F))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_PRE_DBG_BUS_31_30_SEL
// Register Offset : 0x0228
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PRE_DBG_BUS31_SEL(x)                         (((x) & 0x1F) << 5)
#define BIT_AON_APB_PRE_DBG_BUS30_SEL(x)                         (((x) & 0x1F))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_SUB_SYS_DBG_SIG0
// Register Offset : 0x029C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PRE_AON_DBG_SIG_SEL(x)                       (((x) & 0xFF) << 24)
#define BIT_AON_APB_PRE_AON_DBG_MOD_SEL(x)                       (((x) & 0xFF) << 16)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_SUB_SYS_DBG_SIG1
// Register Offset : 0x02A0
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PRE_DISP_DBG_SIG_SEL(x)                      (((x) & 0xFF) << 24)
#define BIT_AON_APB_PRE_DISP_DBG_MOD_SEL(x)                      (((x) & 0xFF) << 16)
#define BIT_AON_APB_PRE_VSP_DBG_SIG_SEL(x)                       (((x) & 0xFF) << 8)
#define BIT_AON_APB_PRE_VSP_DBG_MOD_SEL(x)                       (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_SUB_SYS_DBG_SIG2
// Register Offset : 0x02A4
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PRE_WTL_DBG_SIG_SEL(x)                       (((x) & 0xFF) << 24)
#define BIT_AON_APB_PRE_WTL_DBG_MOD_SEL(x)                       (((x) & 0xFF) << 16)
#define BIT_AON_APB_PRE_ANLG_DBG_SIG_SEL(x)                      (((x) & 0xFF) << 8)
#define BIT_AON_APB_PRE_ANLG_DBG_MOD_SEL(x)                      (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_SUB_SYS_DBG_SIG3
// Register Offset : 0x02A8
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PRE_PUB1_DBG_SIG_SEL(x)                      (((x) & 0xFF) << 24)
#define BIT_AON_APB_PRE_PUB1_DBG_MOD_SEL(x)                      (((x) & 0xFF) << 16)
#define BIT_AON_APB_PRE_PUBCP_DBG_SIG_SEL(x)                     (((x) & 0xFF) << 8)
#define BIT_AON_APB_PRE_PUBCP_DBG_MOD_SEL(x)                     (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_SUB_SYS_DBG_SIG4
// Register Offset : 0x02AC
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PRE_GPU_DBG_SIG_SEL(x)                       (((x) & 0xFF) << 24)
#define BIT_AON_APB_PRE_GPU_DBG_MOD_SEL(x)                       (((x) & 0xFF) << 16)
#define BIT_AON_APB_PRE_PUB0_DBG_SIG_SEL(x)                      (((x) & 0xFF) << 8)
#define BIT_AON_APB_PRE_PUB0_DBG_MOD_SEL(x)                      (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_SUB_SYS_DBG_SIG5
// Register Offset : 0x02B0
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PRE_CAM_DBG_SIG_SEL(x)                       (((x) & 0xFF) << 24)
#define BIT_AON_APB_PRE_CAM_DBG_MOD_SEL(x)                       (((x) & 0xFF) << 16)
#define BIT_AON_APB_PRE_AGCP_DBG_SIG_SEL(x)                      (((x) & 0xFF) << 8)
#define BIT_AON_APB_PRE_AGCP_DBG_MOD_SEL(x)                      (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_SUB_SYS_DBG_SIG6
// Register Offset : 0x02B4
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PRE_AP_DBG_SIG_SEL(x)                        (((x) & 0xFF) << 24)
#define BIT_AON_APB_PRE_AP_DBG_MOD_SEL(x)                        (((x) & 0xFF) << 16)
#define BIT_AON_APB_PRE_DBG_SIG_RESERVED(x)                      (((x) & 0xFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_SUB_SYS_DBG_SIG7
// Register Offset : 0x02B8
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PRE_MDAR_DBG_SIG_SEL(x)                      (((x) & 0xFF) << 24)
#define BIT_AON_APB_PRE_MDAR_DBG_MOD_SEL(x)                      (((x) & 0xFF) << 16)
#define BIT_AON_APB_PRE_DBG_MOD_RESERVED(x)                      (((x) & 0xFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_DEBUG_SUBSYS_SEL
// Register Offset : 0x02BC
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PRE_DBG_SUBSYS_SEL(x)                        (((x) & 0xFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_MDAR_SYS_HSDL_CFG
// Register Offset : 0x02C0
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_MDAR_SYS_HSDL_CFG(x)                         (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_WTLCP2AON_R_FIFO_CH_1
// Register Offset : 0x02C4
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_WTLCP2AON_R_FIFO_AF_LVL_AW_CH(x)             (((x) & 0xF) << 28)
#define BIT_AON_APB_WTLCP2AON_R_FIFO_AE_LVL_AW_CH(x)             (((x) & 0xF) << 24)
#define BIT_AON_APB_WTLCP2AON_R_FIFO_AF_LVL_W_CH(x)              (((x) & 0x1F) << 19)
#define BIT_AON_APB_WTLCP2AON_R_FIFO_AE_LVL_W_CH(x)              (((x) & 0x1F) << 14)
#define BIT_AON_APB_WTLCP2AON_R_FIFO_AF_LVL_B_CH(x)              (((x) & 0x1F) << 9)
#define BIT_AON_APB_WTLCP2AON_R_FIFO_AE_LVL_B_CH(x)              (((x) & 0x1F) << 4)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_WTLCP2AON_R_FIFO_CH0_2
// Register Offset : 0x02C8
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_WTLCP2AON_R_FIFO_AF_LVL_R_CH(x)              (((x) & 0x1F) << 27)
#define BIT_AON_APB_WTLCP2AON_R_FIFO_AE_LVL_R_CH(x)              (((x) & 0x1F) << 22)
#define BIT_AON_APB_WTLCP2AON_R_FIFO_AF_LVL_AR_CH(x)             (((x) & 0xF) << 4)
#define BIT_AON_APB_WTLCP2AON_R_FIFO_AE_LVL_AR_CH(x)             (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_WTLCP2AON_W_FIFO_CH_1
// Register Offset : 0x02CC
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_WTLCP2AON_W_FIFO_AF_LVL_AW_CH(x)             (((x) & 0xF) << 28)
#define BIT_AON_APB_WTLCP2AON_W_FIFO_AE_LVL_AW_CH(x)             (((x) & 0xF) << 24)
#define BIT_AON_APB_WTLCP2AON_W_FIFO_AF_LVL_W_CH(x)              (((x) & 0x1F) << 19)
#define BIT_AON_APB_WTLCP2AON_W_FIFO_AE_LVL_W_CH(x)              (((x) & 0x1F) << 14)
#define BIT_AON_APB_WTLCP2AON_W_FIFO_AF_LVL_B_CH(x)              (((x) & 0x1F) << 9)
#define BIT_AON_APB_WTLCP2AON_W_FIFO_AE_LVL_B_CH(x)              (((x) & 0x1F) << 4)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_WTLCP2AON_W_FIFO_CH0_2
// Register Offset : 0x02D0
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_WTLCP2AON_W_FIFO_AF_LVL_R_CH(x)              (((x) & 0x1F) << 27)
#define BIT_AON_APB_WTLCP2AON_W_FIFO_AE_LVL_R_CH(x)              (((x) & 0x1F) << 22)
#define BIT_AON_APB_WTLCP2AON_W_FIFO_AF_LVL_AR_CH(x)             (((x) & 0xF) << 4)
#define BIT_AON_APB_WTLCP2AON_W_FIFO_AE_LVL_AR_CH(x)             (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AGCP2AON_R_FIFO_CH_1
// Register Offset : 0x02D4
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AGCP2AON_R_FIFO_AF_LVL_AW_CH(x)              (((x) & 0xF) << 28)
#define BIT_AON_APB_AGCP2AON_R_FIFO_AE_LVL_AW_CH(x)              (((x) & 0xF) << 24)
#define BIT_AON_APB_AGCP2AON_R_FIFO_AF_LVL_W_CH(x)               (((x) & 0x1F) << 19)
#define BIT_AON_APB_AGCP2AON_R_FIFO_AE_LVL_W_CH(x)               (((x) & 0x1F) << 14)
#define BIT_AON_APB_AGCP2AON_R_FIFO_AF_LVL_B_CH(x)               (((x) & 0x1F) << 9)
#define BIT_AON_APB_AGCP2AON_R_FIFO_AE_LVL_B_CH(x)               (((x) & 0x1F) << 4)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AGCP2AON_R_FIFO_CH0_2
// Register Offset : 0x02D8
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AGCP2AON_R_FIFO_AF_LVL_R_CH(x)               (((x) & 0x1F) << 27)
#define BIT_AON_APB_AGCP2AON_R_FIFO_AE_LVL_R_CH(x)               (((x) & 0x1F) << 22)
#define BIT_AON_APB_AGCP2AON_R_FIFO_AF_LVL_AR_CH(x)              (((x) & 0xF) << 4)
#define BIT_AON_APB_AGCP2AON_R_FIFO_AE_LVL_AR_CH(x)              (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AGCP2AON_W_FIFO_CH_1
// Register Offset : 0x02DC
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AGCP2AON_W_FIFO_AF_LVL_AW_CH(x)              (((x) & 0xF) << 28)
#define BIT_AON_APB_AGCP2AON_W_FIFO_AE_LVL_AW_CH(x)              (((x) & 0xF) << 24)
#define BIT_AON_APB_AGCP2AON_W_FIFO_AF_LVL_W_CH(x)               (((x) & 0x1F) << 19)
#define BIT_AON_APB_AGCP2AON_W_FIFO_AE_LVL_W_CH(x)               (((x) & 0x1F) << 14)
#define BIT_AON_APB_AGCP2AON_W_FIFO_AF_LVL_B_CH(x)               (((x) & 0x1F) << 9)
#define BIT_AON_APB_AGCP2AON_W_FIFO_AE_LVL_B_CH(x)               (((x) & 0x1F) << 4)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AGCP2AON_W_FIFO_CH0_2
// Register Offset : 0x02E0
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AGCP2AON_W_FIFO_AF_LVL_R_CH(x)               (((x) & 0x1F) << 27)
#define BIT_AON_APB_AGCP2AON_W_FIFO_AE_LVL_R_CH(x)               (((x) & 0x1F) << 22)
#define BIT_AON_APB_AGCP2AON_W_FIFO_AF_LVL_AR_CH(x)              (((x) & 0xF) << 4)
#define BIT_AON_APB_AGCP2AON_W_FIFO_AE_LVL_AR_CH(x)              (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_PUBCP2AON_R_FIFO_CH_1
// Register Offset : 0x02E4
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PUBCP2AON_R_FIFO_AF_LVL_AW_CH(x)             (((x) & 0xF) << 28)
#define BIT_AON_APB_PUBCP2AON_R_FIFO_AE_LVL_AW_CH(x)             (((x) & 0xF) << 24)
#define BIT_AON_APB_PUBCP2AON_R_FIFO_AF_LVL_W_CH(x)              (((x) & 0x1F) << 19)
#define BIT_AON_APB_PUBCP2AON_R_FIFO_AE_LVL_W_CH(x)              (((x) & 0x1F) << 14)
#define BIT_AON_APB_PUBCP2AON_R_FIFO_AF_LVL_B_CH(x)              (((x) & 0x1F) << 9)
#define BIT_AON_APB_PUBCP2AON_R_FIFO_AE_LVL_B_CH(x)              (((x) & 0x1F) << 4)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_PUBCP2AON_R_FIFO_CH0_2
// Register Offset : 0x02E8
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PUBCP2AON_R_FIFO_AF_LVL_R_CH(x)              (((x) & 0x1F) << 27)
#define BIT_AON_APB_PUBCP2AON_R_FIFO_AE_LVL_R_CH(x)              (((x) & 0x1F) << 22)
#define BIT_AON_APB_PUBCP2AON_R_FIFO_AF_LVL_AR_CH(x)             (((x) & 0xF) << 4)
#define BIT_AON_APB_PUBCP2AON_R_FIFO_AE_LVL_AR_CH(x)             (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_PUBCP2AON_W_FIFO_CH_1
// Register Offset : 0x02EC
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PUBCP2AON_W_FIFO_AF_LVL_AW_CH(x)             (((x) & 0xF) << 28)
#define BIT_AON_APB_PUBCP2AON_W_FIFO_AE_LVL_AW_CH(x)             (((x) & 0xF) << 24)
#define BIT_AON_APB_PUBCP2AON_W_FIFO_AF_LVL_W_CH(x)              (((x) & 0x1F) << 19)
#define BIT_AON_APB_PUBCP2AON_W_FIFO_AE_LVL_W_CH(x)              (((x) & 0x1F) << 14)
#define BIT_AON_APB_PUBCP2AON_W_FIFO_AF_LVL_B_CH(x)              (((x) & 0x1F) << 9)
#define BIT_AON_APB_PUBCP2AON_W_FIFO_AE_LVL_B_CH(x)              (((x) & 0x1F) << 4)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_PUBCP2AON_W_FIFO_CH0_2
// Register Offset : 0x02F0
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PUBCP2AON_W_FIFO_AF_LVL_R_CH(x)              (((x) & 0x1F) << 27)
#define BIT_AON_APB_PUBCP2AON_W_FIFO_AE_LVL_R_CH(x)              (((x) & 0x1F) << 22)
#define BIT_AON_APB_PUBCP2AON_W_FIFO_AF_LVL_AR_CH(x)             (((x) & 0xF) << 4)
#define BIT_AON_APB_PUBCP2AON_W_FIFO_AE_LVL_AR_CH(x)             (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON2PUBCP_R_FIFO_CH_1
// Register Offset : 0x02F4
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AON2PUBCP_R_FIFO_AF_LVL_AW_CH(x)             (((x) & 0xF) << 28)
#define BIT_AON_APB_AON2PUBCP_R_FIFO_AE_LVL_AW_CH(x)             (((x) & 0xF) << 24)
#define BIT_AON_APB_AON2PUBCP_R_FIFO_AF_LVL_W_CH(x)              (((x) & 0x1F) << 19)
#define BIT_AON_APB_AON2PUBCP_R_FIFO_AE_LVL_W_CH(x)              (((x) & 0x1F) << 14)
#define BIT_AON_APB_AON2PUBCP_R_FIFO_AF_LVL_B_CH(x)              (((x) & 0x1F) << 9)
#define BIT_AON_APB_AON2PUBCP_R_FIFO_AE_LVL_B_CH(x)              (((x) & 0x1F) << 4)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON2PUBCP_R_FIFO_CH0_2
// Register Offset : 0x02F8
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AON2PUBCP_R_FIFO_AF_LVL_R_CH(x)              (((x) & 0x1F) << 27)
#define BIT_AON_APB_AON2PUBCP_R_FIFO_AE_LVL_R_CH(x)              (((x) & 0x1F) << 22)
#define BIT_AON_APB_AON2PUBCP_R_FIFO_AF_LVL_AR_CH(x)             (((x) & 0xF) << 4)
#define BIT_AON_APB_AON2PUBCP_R_FIFO_AE_LVL_AR_CH(x)             (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON2PUBCP_W_FIFO_CH_1
// Register Offset : 0x02FC
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AON2PUBCP_W_FIFO_AF_LVL_AW_CH(x)             (((x) & 0xF) << 28)
#define BIT_AON_APB_AON2PUBCP_W_FIFO_AE_LVL_AW_CH(x)             (((x) & 0xF) << 24)
#define BIT_AON_APB_AON2PUBCP_W_FIFO_AF_LVL_W_CH(x)              (((x) & 0x1F) << 19)
#define BIT_AON_APB_AON2PUBCP_W_FIFO_AE_LVL_W_CH(x)              (((x) & 0x1F) << 14)
#define BIT_AON_APB_AON2PUBCP_W_FIFO_AF_LVL_B_CH(x)              (((x) & 0x1F) << 9)
#define BIT_AON_APB_AON2PUBCP_W_FIFO_AE_LVL_B_CH(x)              (((x) & 0x1F) << 4)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON2PUBCP_W_FIFO_CH0_2
// Register Offset : 0x0300
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AON2PUBCP_W_FIFO_AF_LVL_R_CH(x)              (((x) & 0x1F) << 27)
#define BIT_AON_APB_AON2PUBCP_W_FIFO_AE_LVL_R_CH(x)              (((x) & 0x1F) << 22)
#define BIT_AON_APB_AON2PUBCP_W_FIFO_AF_LVL_AR_CH(x)             (((x) & 0xF) << 4)
#define BIT_AON_APB_AON2PUBCP_W_FIFO_AE_LVL_AR_CH(x)             (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON2AGCP_R_FIFO_CH_1
// Register Offset : 0x0304
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AON2AGCP_R_FIFO_AF_LVL_AW_CH(x)              (((x) & 0xF) << 28)
#define BIT_AON_APB_AON2AGCP_R_FIFO_AE_LVL_AW_CH(x)              (((x) & 0xF) << 24)
#define BIT_AON_APB_AON2AGCP_R_FIFO_AF_LVL_W_CH(x)               (((x) & 0x1F) << 19)
#define BIT_AON_APB_AON2AGCP_R_FIFO_AE_LVL_W_CH(x)               (((x) & 0x1F) << 14)
#define BIT_AON_APB_AON2AGCP_R_FIFO_AF_LVL_B_CH(x)               (((x) & 0x1F) << 9)
#define BIT_AON_APB_AON2AGCP_R_FIFO_AE_LVL_B_CH(x)               (((x) & 0x1F) << 4)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON2AGCP_R_FIFO_CH0_2
// Register Offset : 0x0308
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AON2AGCP_R_FIFO_AF_LVL_R_CH(x)               (((x) & 0x1F) << 27)
#define BIT_AON_APB_AON2AGCP_R_FIFO_AE_LVL_R_CH(x)               (((x) & 0x1F) << 22)
#define BIT_AON_APB_AON2AGCP_R_FIFO_AF_LVL_AR_CH(x)              (((x) & 0xF) << 4)
#define BIT_AON_APB_AON2AGCP_R_FIFO_AE_LVL_AR_CH(x)              (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON2AGCP_W_FIFO_CH_1
// Register Offset : 0x030C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AON2AGCP_W_FIFO_AF_LVL_AW_CH(x)              (((x) & 0xF) << 28)
#define BIT_AON_APB_AON2AGCP_W_FIFO_AE_LVL_AW_CH(x)              (((x) & 0xF) << 24)
#define BIT_AON_APB_AON2AGCP_W_FIFO_AF_LVL_W_CH(x)               (((x) & 0x1F) << 19)
#define BIT_AON_APB_AON2AGCP_W_FIFO_AE_LVL_W_CH(x)               (((x) & 0x1F) << 14)
#define BIT_AON_APB_AON2AGCP_W_FIFO_AF_LVL_B_CH(x)               (((x) & 0x1F) << 9)
#define BIT_AON_APB_AON2AGCP_W_FIFO_AE_LVL_B_CH(x)               (((x) & 0x1F) << 4)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON2AGCP_W_FIFO_CH0_2
// Register Offset : 0x0310
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AON2AGCP_W_FIFO_AF_LVL_R_CH(x)               (((x) & 0x1F) << 27)
#define BIT_AON_APB_AON2AGCP_W_FIFO_AE_LVL_R_CH(x)               (((x) & 0x1F) << 22)
#define BIT_AON_APB_AON2AGCP_W_FIFO_AF_LVL_AR_CH(x)              (((x) & 0xF) << 4)
#define BIT_AON_APB_AON2AGCP_W_FIFO_AE_LVL_AR_CH(x)              (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON2PUB0_R_FIFO_CH_1
// Register Offset : 0x0314
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AON2PUB0_R_FIFO_AF_LVL_AW_CH(x)              (((x) & 0xF) << 28)
#define BIT_AON_APB_AON2PUB0_R_FIFO_AE_LVL_AW_CH(x)              (((x) & 0xF) << 24)
#define BIT_AON_APB_AON2PUB0_R_FIFO_AF_LVL_W_CH(x)               (((x) & 0x1F) << 19)
#define BIT_AON_APB_AON2PUB0_R_FIFO_AE_LVL_W_CH(x)               (((x) & 0x1F) << 14)
#define BIT_AON_APB_AON2PUB0_R_FIFO_AF_LVL_B_CH(x)               (((x) & 0x1F) << 9)
#define BIT_AON_APB_AON2PUB0_R_FIFO_AE_LVL_B_CH(x)               (((x) & 0x1F) << 4)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON2PUB0_R_FIFO_CH0_2
// Register Offset : 0x031C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AON2PUB0_R_FIFO_AF_LVL_R_CH(x)               (((x) & 0x1F) << 27)
#define BIT_AON_APB_AON2PUB0_R_FIFO_AE_LVL_R_CH(x)               (((x) & 0x1F) << 22)
#define BIT_AON_APB_AON2PUB0_R_FIFO_AF_LVL_AR_CH(x)              (((x) & 0xF) << 4)
#define BIT_AON_APB_AON2PUB0_R_FIFO_AE_LVL_AR_CH(x)              (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON2PUB0_W_FIFO_CH_1
// Register Offset : 0x0320
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AON2PUB0_W_FIFO_AF_LVL_AW_CH(x)              (((x) & 0xF) << 28)
#define BIT_AON_APB_AON2PUB0_W_FIFO_AE_LVL_AW_CH(x)              (((x) & 0xF) << 24)
#define BIT_AON_APB_AON2PUB0_W_FIFO_AF_LVL_W_CH(x)               (((x) & 0x1F) << 19)
#define BIT_AON_APB_AON2PUB0_W_FIFO_AE_LVL_W_CH(x)               (((x) & 0x1F) << 14)
#define BIT_AON_APB_AON2PUB0_W_FIFO_AF_LVL_B_CH(x)               (((x) & 0x1F) << 9)
#define BIT_AON_APB_AON2PUB0_W_FIFO_AE_LVL_B_CH(x)               (((x) & 0x1F) << 4)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON2PUB0_W_FIFO_CH0_2
// Register Offset : 0x0324
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AON2PUB0_W_FIFO_AF_LVL_R_CH(x)               (((x) & 0x1F) << 27)
#define BIT_AON_APB_AON2PUB0_W_FIFO_AE_LVL_R_CH(x)               (((x) & 0x1F) << 22)
#define BIT_AON_APB_AON2PUB0_W_FIFO_AF_LVL_AR_CH(x)              (((x) & 0xF) << 4)
#define BIT_AON_APB_AON2PUB0_W_FIFO_AE_LVL_AR_CH(x)              (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON2PUB1_R_FIFO_CH_1
// Register Offset : 0x0328
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AON2PUB1_R_FIFO_AF_LVL_AW_CH(x)              (((x) & 0xF) << 28)
#define BIT_AON_APB_AON2PUB1_R_FIFO_AE_LVL_AW_CH(x)              (((x) & 0xF) << 24)
#define BIT_AON_APB_AON2PUB1_R_FIFO_AF_LVL_W_CH(x)               (((x) & 0x1F) << 19)
#define BIT_AON_APB_AON2PUB1_R_FIFO_AE_LVL_W_CH(x)               (((x) & 0x1F) << 14)
#define BIT_AON_APB_AON2PUB1_R_FIFO_AF_LVL_B_CH(x)               (((x) & 0x1F) << 9)
#define BIT_AON_APB_AON2PUB1_R_FIFO_AE_LVL_B_CH(x)               (((x) & 0x1F) << 4)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON2PUB1_R_FIFO_CH0_2
// Register Offset : 0x032C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AON2PUB1_R_FIFO_AF_LVL_R_CH(x)               (((x) & 0x1F) << 27)
#define BIT_AON_APB_AON2PUB1_R_FIFO_AE_LVL_R_CH(x)               (((x) & 0x1F) << 22)
#define BIT_AON_APB_AON2PUB1_R_FIFO_AF_LVL_AR_CH(x)              (((x) & 0xF) << 4)
#define BIT_AON_APB_AON2PUB1_R_FIFO_AE_LVL_AR_CH(x)              (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON2PUB1_W_FIFO_CH_1
// Register Offset : 0x0330
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AON2PUB1_W_FIFO_AF_LVL_AW_CH(x)              (((x) & 0xF) << 28)
#define BIT_AON_APB_AON2PUB1_W_FIFO_AE_LVL_AW_CH(x)              (((x) & 0xF) << 24)
#define BIT_AON_APB_AON2PUB1_W_FIFO_AF_LVL_W_CH(x)               (((x) & 0x1F) << 19)
#define BIT_AON_APB_AON2PUB1_W_FIFO_AE_LVL_W_CH(x)               (((x) & 0x1F) << 14)
#define BIT_AON_APB_AON2PUB1_W_FIFO_AF_LVL_B_CH(x)               (((x) & 0x1F) << 9)
#define BIT_AON_APB_AON2PUB1_W_FIFO_AE_LVL_B_CH(x)               (((x) & 0x1F) << 4)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON2PUB1_W_FIFO_CH0_2
// Register Offset : 0x0334
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AON2PUB1_W_FIFO_AF_LVL_R_CH(x)               (((x) & 0x1F) << 27)
#define BIT_AON_APB_AON2PUB1_W_FIFO_AE_LVL_R_CH(x)               (((x) & 0x1F) << 22)
#define BIT_AON_APB_AON2PUB1_W_FIFO_AF_LVL_AR_CH(x)              (((x) & 0xF) << 4)
#define BIT_AON_APB_AON2PUB1_W_FIFO_AE_LVL_AR_CH(x)              (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_BIA_NOC_CTRL
// Register Offset : 0x0338
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_BIA_PUB_CHIP_DDR_CAPACITY_I(x)               (((x) & 0x7) << 9)
#define BIT_AON_APB_BIA_PUB_INTERLEAVE_MODE(x)                   (((x) & 0x3) << 1)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AP_NOC_CTRL
// Register Offset : 0x033C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AP_M0_IDLE                                   BIT(8)
#define BIT_AON_APB_AP_PUB_CHIP_DDR_CAPACITY_I(x)                (((x) & 0x7) << 5)
#define BIT_AON_APB_AP_INTERLEAVE_SIZE_SEL                       BIT(4)
#define BIT_AON_APB_AP_PUB_INTERLEAVE_MODE(x)                    (((x) & 0x3) << 1)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_DMC_CLK_PLL_SEL
// Register Offset : 0x0340
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_DMC_PLL_SEC_SEL                              BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_USB30_SLEEP
// Register Offset : 0x0344
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_USB30_SLEEP                                  BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_ACCDDR_LATENCY_REDUCE_SEL
// Register Offset : 0x0348
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_VSP2DDR_LATENCY_REDUCE_SEL                   BIT(2)
#define BIT_AON_APB_CAM2DDR_LATENCY_REDUCE_SEL                   BIT(1)
#define BIT_AON_APB_GPU2DDR_LATENCY_REDUCE_SEL                   BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_BIA_DVFS_CHANNEL_SEL
// Register Offset : 0x034C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_DVFS1_CHANNEL_SEL                            BIT(1)
#define BIT_AON_APB_DVFS0_CHANNEL_SEL                            BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_IO_DLY_CTRL
// Register Offset : 0x3014
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_CLK_CP1DSP_DLY_SEL(x)                        (((x) & 0xF) << 4)
#define BIT_AON_APB_CLK_CP0DSP_DLY_SEL(x)                        (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_WDG_RST_FLAG
// Register Offset : 0x3024
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PCP_WDG_RST_FLAG                             BIT(5)
#define BIT_AON_APB_WTLCP_LTE_WDG_RST_FLAG                       BIT(4)
#define BIT_AON_APB_WTLCP_TG_WDG_RST_FLAG                        BIT(3)
#define BIT_AON_APB_AGCP_WDG_RST_FLAG                            BIT(2)
#define BIT_AON_APB_AP_WDG_RST_FLAG                              BIT(1)
#define BIT_AON_APB_SEC_WDG_RST_FLAG                             BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_PMU_RST_MONITOR
// Register Offset : 0x302C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PMU_RST_MONITOR(x)                           (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_THM_RST_MONITOR
// Register Offset : 0x3030
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_THM_RST_MONITOR(x)                           (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AP_RST_MONITOR
// Register Offset : 0x3034
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AP_RST_MONITOR(x)                            (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_BOND_OPT0
// Register Offset : 0x303C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_BOND_OPTION0(x)                              (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_BOND_OPT1
// Register Offset : 0x3040
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_BOND_OPTION1(x)                              (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_RES_REG0
// Register Offset : 0x3044
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_RES_REG0(x)                                  (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_RES_REG1
// Register Offset : 0x3048
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_RES_REG1(x)                                  (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON_QOS_CFG
// Register Offset : 0x304C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_QOS_R_GPU(x)                                 (((x) & 0xF) << 12)
#define BIT_AON_APB_QOS_W_GPU(x)                                 (((x) & 0xF) << 8)
#define BIT_AON_APB_QOS_R_GSP(x)                                 (((x) & 0xF) << 4)
#define BIT_AON_APB_QOS_W_GSP(x)                                 (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON_MTX_PROT_CFG
// Register Offset : 0x3058
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_HPROT_DMAW(x)                                (((x) & 0xF) << 4)
#define BIT_AON_APB_HPROT_DMAR(x)                                (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_PLL_LOCK_OUT_SEL
// Register Offset : 0x3064
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_REC_26MHZ_0_BUF_PD                           BIT(8)
#define BIT_AON_APB_SLEEP_PLLLOCK_SEL                            BIT(7)
#define BIT_AON_APB_PLL_LOCK_SEL(x)                              (((x) & 0x7) << 4)
#define BIT_AON_APB_SLEEP_DBG_SEL(x)                             (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_RTC4M_RC_VAL
// Register Offset : 0x3068
// Description     : 
---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_DDR_STAT_FLAG
// Register Offset : 0x306C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_DDR1_DISABLE_ACCESS                          BIT(5)
#define BIT_AON_APB_DDR0_DISABLE_ACCESS                          BIT(4)
#define BIT_AON_APB_INT_DFS_PUB0_COMPLETE                        BIT(3)
#define BIT_AON_APB_INT_DFS_PUB1_COMPLETE                        BIT(2)
#define BIT_AON_APB_INT_REQ_PUB1_CTRL_FLAG                       BIT(1)
#define BIT_AON_APB_INT_REQ_PUB0_CTRL_FLAG                       BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON_QOS_CFG1
// Register Offset : 0x3070
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_NOC_BIA_AP_M1_AR_QOS(x)                      (((x) & 0xF) << 28)
#define BIT_AON_APB_NOC_BIA_AP_M1_AW_QOS(x)                      (((x) & 0xF) << 24)
#define BIT_AON_APB_NOC_BIA_AP_M0_AR_QOS(x)                      (((x) & 0xF) << 20)
#define BIT_AON_APB_NOC_BIA_AP_M0_AW_QOS(x)                      (((x) & 0xF) << 16)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_USER_COMM_FLAG
// Register Offset : 0x3074
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_USER_COMM_FLAG(x)                            (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON_MAIN_MTX_QOS1
// Register Offset : 0x3078
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_ARQOS_M4_SP_AON(x)                           (((x) & 0xF) << 4)
#define BIT_AON_APB_AWQOS_M4_SP_AON(x)                           (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON_MAIN_MTX_QOS2
// Register Offset : 0x307C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_ARQOS_M3_AGCP_AON(x)                         (((x) & 0xF) << 28)
#define BIT_AON_APB_AWQOS_M3_AGCP_AON(x)                         (((x) & 0xF) << 24)
#define BIT_AON_APB_ARQOS_M2_PUBCP_AON(x)                        (((x) & 0xF) << 20)
#define BIT_AON_APB_AWQOS_M2_PUBCP_AON(x)                        (((x) & 0xF) << 16)
#define BIT_AON_APB_ARQOS_M1_WTLCP_AON(x)                        (((x) & 0xF) << 12)
#define BIT_AON_APB_AWQOS_M1_WTLCP_AON(x)                        (((x) & 0xF) << 8)
#define BIT_AON_APB_ARQOS_M0_AP_AON(x)                           (((x) & 0xF) << 4)
#define BIT_AON_APB_AWQOS_M0_AP_AON(x)                           (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_RC100M_RC_VAL
// Register Offset : 0x3080
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_RC100M_RC_SEL                                BIT(15)
#define BIT_AON_APB_RC100M_RC_VAL(x)                             (((x) & 0x1FF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON_APB_RSV
// Register Offset : 0x30F0
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AON_APB_RSV(x)                               (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_GLB_CLK_AUTO_GATE_EN
// Register Offset : 0x30F4
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_GLB_CLK_AUTO_GATE_EN(x)                      (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON_OSC_FUNC_CNT
// Register Offset : 0x3100
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_OSC_CHNL1_FUNC_CNT(x)                        (((x) & 0xFFFF) << 16)
#define BIT_AON_APB_OSC_CHNL2_FUNC_CNT(x)                        (((x) & 0xFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON_OSC_FUNC_CTRL
// Register Offset : 0x3104
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_RST_BINING_FUNC_N                            BIT(25)
#define BIT_AON_APB_OSC_BINING_SEL_FUNC(x)                       (((x) & 0x7F) << 18)
#define BIT_AON_APB_OSC_BINING_CTRL_FUNC_EN                      BIT(17)
#define BIT_AON_APB_OSC_BINING_CLK_FUNC_NUM(x)                   (((x) & 0x1FFF) << 4)
#define BIT_AON_APB_OSC_CHNL2_FUNC_OVERFLOW                      BIT(3)
#define BIT_AON_APB_OSC_CHNL1_FUNC_OVERFLOW                      BIT(2)
#define BIT_AON_APB_OSC_CHNL2_FUNC_CNT_VALID                     BIT(1)
#define BIT_AON_APB_OSC_CHNL1_FUNC_CNT_VALID                     BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_DEEP_SLEEP_MUX_SEL
// Register Offset : 0x3108
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AP_DEEP_SLEEP_SEL                            BIT(3)
#define BIT_AON_APB_WTLCP_DEEP_SLEEP_SEL                         BIT(2)
#define BIT_AON_APB_PUBCP_DEEP_SLEEP_SEL                         BIT(1)
#define BIT_AON_APB_AGCP_DEEP_SLEEP_SEL                          BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_NIU_IDLE_DET_DISABLE
// Register Offset : 0x310C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_VSP_NIU_IDLE_DET_DISABLE                     BIT(4)
#define BIT_AON_APB_DISP_NIU_IDLE_DET_DISABLE                    BIT(3)
#define BIT_AON_APB_GSP_NIU_IDLE_DET_DISABLE                     BIT(2)
#define BIT_AON_APB_CAM_NIU_IDLE_DET_DISABLE                     BIT(1)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_FUNC_TEST_BOOT_ADDR
// Register Offset : 0x3110
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_FUNC_TEST_BOOT_ADDR(x)                       (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_CGM_RESCUE
// Register Offset : 0x3114
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_CGM_RESCUE(x)                                (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON_PLAT_ID0
// Register Offset : 0x3118
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AON_PLAT_ID0(x)                              (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON_PLAT_ID1
// Register Offset : 0x311C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AON_PLAT_ID1(x)                              (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON_VER_ID
// Register Offset : 0x3120
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AON_VER_ID(x)                                (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON_MFT_ID
// Register Offset : 0x3124
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AON_MFT_ID(x)                                (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON_MPL_ID
// Register Offset : 0x3128
// Description     : IMPL_ID
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AON_IMPL_ID(x)                               (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AON_SDIO
// Register Offset : 0x312C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_CP_SDIO_ENABLE                               BIT(2)
#define BIT_AON_APB_AP_SDIO_ENABLE                               BIT(1)
#define BIT_AON_APB_SDIO_MODULE_SEL                              BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_PWM_3D_MUX
// Register Offset : 0x3134
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PWM_3DTN_STATUS(x)                           (((x) & 0x1F) << 6)
#define BIT_AON_APB_PWM_3DTN_EN_CTRL(x)                          (((x) & 0x3) << 4)
#define BIT_AON_APB_PWM_3DTN_IO_CTRL(x)                          (((x) & 0xF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_VSP_NOC_CTRL
// Register Offset : 0x315C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_VSP_M0_IDLE                                  BIT(8)
#define BIT_AON_APB_VSP_SERVICE_ACCESS_EN                        BIT(3)
#define BIT_AON_APB_VSP_INTERLEAVE_MODE(x)                       (((x) & 0x3) << 1)
#define BIT_AON_APB_VSP_INTERLEAVE_SEL                           BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_CAM_NOC_CTRL
// Register Offset : 0x3160
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_CAM_M0_IDLE                                  BIT(8)
#define BIT_AON_APB_CAM_SERVICE_ACCESS_EN                        BIT(3)
#define BIT_AON_APB_CAM_INTERLEAVE_MODE(x)                       (((x) & 0x3) << 1)
#define BIT_AON_APB_CAM_INTERLEAVE_SEL                           BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_DISP_NOC_CTRL
// Register Offset : 0x3164
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_DISP_M0_IDLE                                 BIT(8)
#define BIT_AON_APB_DISP_SERVICE_ACCESS_EN                       BIT(3)
#define BIT_AON_APB_DISP_INTERLEAVE_MODE(x)                      (((x) & 0x3) << 1)
#define BIT_AON_APB_DISP_INTERLEAVE_SEL                          BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_GSP_NOC_CTRL
// Register Offset : 0x3168
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_GSP_M0_IDLE                                  BIT(8)
#define BIT_AON_APB_GSP_SERVICE_ACCESS_EN                        BIT(3)
#define BIT_AON_APB_GSP_INTERLEAVE_MODE(x)                       (((x) & 0x3) << 1)
#define BIT_AON_APB_GSP_INTERLEAVE_SEL                           BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_WTL_CP_NOC_CTRL
// Register Offset : 0x316C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_WTL_CP_M0_IDLE                               BIT(8)
#define BIT_AON_APB_WTL_CP_ADDR_REMAP_BIT(x)                     (((x) & 0x7) << 4)
#define BIT_AON_APB_WTL_CP_SERVICE_ACCESS_EN                     BIT(3)
#define BIT_AON_APB_WTL_CP_INTERLEAVE_MODE(x)                    (((x) & 0x3) << 1)
#define BIT_AON_APB_WTL_CP_INTERLEAVE_SEL                        BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_LACC_NOC_CTRL
// Register Offset : 0x3170
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_LACC_M0_IDLE                                 BIT(8)
#define BIT_AON_APB_LACC_ADDR_REMAP_BIT(x)                       (((x) & 0x7) << 4)
#define BIT_AON_APB_LACC_SERVICE_ACCESS_EN                       BIT(3)
#define BIT_AON_APB_LACC_INTERLEAVE_MODE(x)                      (((x) & 0x3) << 1)
#define BIT_AON_APB_LACC_INTERLEAVE_SEL                          BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_AGCP_NOC_CTRL
// Register Offset : 0x3174
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_AGCP_M0_IDLE                                 BIT(8)
#define BIT_AON_APB_AGCP_ADDR_REMAP_BIT(x)                       (((x) & 0x7) << 4)
#define BIT_AON_APB_AGCP_SERVICE_ACCESS_EN                       BIT(3)
#define BIT_AON_APB_AGCP_INTERLEAVE_MODE(x)                      (((x) & 0x3) << 1)
#define BIT_AON_APB_AGCP_INTERLEAVE_SEL                          BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_PCP_NOC_CTRL
// Register Offset : 0x3178
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_PCP_M0_IDLE                                  BIT(8)
#define BIT_AON_APB_PCP_ADDR_REMAP_BIT(x)                        (((x) & 0x7) << 4)
#define BIT_AON_APB_PCP_SERVICE_ACCESS_EN                        BIT(3)
#define BIT_AON_APB_PCP_INTERLEAVE_MODE(x)                       (((x) & 0x3) << 1)
#define BIT_AON_APB_PCP_INTERLEAVE_SEL                           BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_PCP_TIMESTAMP_CTRL
// Register Offset : 0x3180
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_EVENT_RESTARTREQ_TS0                         BIT(1)
#define BIT_AON_APB_EVENT_HALTREQ_TS0                            BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_SP_WAKEUP_MASK_EN1
// Register Offset : 0x3184
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_SP_PUB1_DFI_BUSMON_INT_WAKE_EN               BIT(31)
#define BIT_AON_APB_SP_PUB1_AXI_BUSMON_INT_WAKE_EN               BIT(30)
#define BIT_AON_APB_SP_THM_INT_WAKE_EN                           BIT(29)
#define BIT_AON_APB_SP_PUB0_CTRL_INT_WAKE_EN                     BIT(28)
#define BIT_AON_APB_SP_PUB1_CTRL_INT_WAKE_EN                     BIT(27)
#define BIT_AON_APB_SP_DFS_PUB0_INT_WAKE_EN                      BIT(26)
#define BIT_AON_APB_SP_DFS_PUB1_INT_WAKE_EN                      BIT(25)
#define BIT_AON_APB_SP_BUSMON_WTLCP_INT_WAKE_EN                  BIT(24)
#define BIT_AON_APB_SP_BUSMON_PUBCP_INT_WAKE_EN                  BIT(23)
#define BIT_AON_APB_SP_BUSMON_AGCP_INT_WAKE_EN                   BIT(22)
#define BIT_AON_APB_SP_BUSMON_AP_INT_WAKE_EN                     BIT(21)
#define BIT_AON_APB_BIA_VOLTAGE1_BAD_INT_WAKE_EN                 BIT(20)
#define BIT_AON_APB_BIA_VOLTAGE0_BAD_INT_WAKE_EN                 BIT(19)
#define BIT_AON_APB_SP_AVS_GPU0_INT_WAKE_EN                      BIT(18)
#define BIT_AON_APB_SP_EIC_NOTLAT_INT_WAKE_EN                    BIT(17)
#define BIT_AON_APB_SP_CA53_BUSMON_INT_WAKE_EN                   BIT(16)
#define BIT_AON_APB_SP_ADI_INT_WAKE_EN                           BIT(15)
#define BIT_AON_APB_SP_ANA_INT_WAKE_EN                           BIT(14)
#define BIT_AON_APB_SP_AON_I2C_INT_WAKE_EN                       BIT(13)
#define BIT_AON_APB_SP_GPIO_INT_WAKE_EN                          BIT(12)
#define BIT_AON_APB_SP_DMA_INT_WAKE_EN                           BIT(11)
#define BIT_AON_APB_SP_MBOX_SRC_INT_WAKE_EN                      BIT(10)
#define BIT_AON_APB_SP_MBOX_TAR_INT_WAKE_EN                      BIT(9)
#define BIT_AON_APB_SP_WDG_INT_WAKE_EN                           BIT(8)
#define BIT_AON_APB_SP_I2C1_INT_WAKE_EN                          BIT(7)
#define BIT_AON_APB_SP_I2C0_INT_WAKE_EN                          BIT(6)
#define BIT_AON_APB_SP_UART1_INT_WAKE_EN                         BIT(5)
#define BIT_AON_APB_SP_UART0_INT_WAKE_EN                         BIT(4)
#define BIT_AON_APB_SP_SYST_INT_WAKE_EN                          BIT(3)
#define BIT_AON_APB_SP_TMR_INT_WAKE_EN                           BIT(2)
#define BIT_AON_APB_SP_EIC_LAT_INT_WAKE_EN                       BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_SP_WAKEUP_MASK_EN2
// Register Offset : 0x3188
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_SP_CM4_SPI_INT_WAKE_EN                       BIT(22)
#define BIT_AON_APB_SP_BUSMON_INT_WAKE_EN                        BIT(21)
#define BIT_AON_APB_SP_FPU_INT_WAKE_EN                           BIT(20)
#define BIT_AON_APB_SP_GPU_VDROP_OUT_1_INT_WAKE_EN               BIT(19)
#define BIT_AON_APB_SP_GPU_VDROP_OUT_0_INT_WAKE_EN               BIT(18)
#define BIT_AON_APB_SP_AGCP_DMA_INT_WAKE_EN                      BIT(15)
#define BIT_AON_APB_BUTTER_WDG_INT_WAKE_EN                       BIT(14)
#define BIT_AON_APB_SP_PUB1_DFS_ERROR_INT_WAKE_EN                BIT(13)
#define BIT_AON_APB_SP_PUB0_DFS_ERROR_INT_WAKE_EN                BIT(12)
#define BIT_AON_APB_SP_PUB1_DJTAG_INT_WAKE_EN                    BIT(11)
#define BIT_AON_APB_SP_PUB0_DJTAG_INT_WAKE_EN                    BIT(10)
#define BIT_AON_APB_SP_AON_DEF_SLV_INT_WAKE_EN                   BIT(9)
#define BIT_AON_APB_SP_BUSMON_M0_INT_WAKE_EN                     BIT(8)
#define BIT_AON_APB_SP_BUSMON_M1_INT_WAKE_EN                     BIT(7)
#define BIT_AON_APB_SP_BUSMON_M2_INT_WAKE_EN                     BIT(6)
#define BIT_AON_APB_SP_BUSMON_M3_INT_WAKE_EN                     BIT(5)
#define BIT_AON_APB_SP_BUSMON_M4_INT_WAKE_EN                     BIT(4)
#define BIT_AON_APB_SP_PUB0_DFS_EXIT_INT_WAKE_EN                 BIT(3)
#define BIT_AON_APB_SP_PUB1_DFS_EXIT_INT_WAKE_EN                 BIT(2)
#define BIT_AON_APB_SP_PUB0_DFI_BUSMON_INT_WAKE_EN               BIT(1)
#define BIT_AON_APB_SP_PUB0_AXI_BUSMON_INT_WAKE_EN               BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_THERMAL_OVERHEAT_EN
// Register Offset : 0x318C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_SP_THM_OVERHEAT_ALL_EN                       BIT(4)
#define BIT_AON_APB_ITHM_OVERHEAT1_EN                            BIT(3)
#define BIT_AON_APB_ITHM_OVERHEAT0_EN                            BIT(2)
#define BIT_AON_APB_SP_THM_OVERHEAT1_EN                          BIT(1)
#define BIT_AON_APB_SP_THM_OVERHEAT0_EN                          BIT(0)

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_BIA_REG_DAT0
// Register Offset : 0x3278
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_BIA_REG_DAT0(x)                              (((x) & 0xFFFFFFFF))

/*---------------------------------------------------------------------------
// Register Name   : REG_AON_APB_BIA_REG_DAT1
// Register Offset : 0x327C
// Description     : 
---------------------------------------------------------------------------*/

#define BIT_AON_APB_BIA_REG_DAT1(x)                              (((x) & 0xFFFFFFFF))


#endif // _AON_APB_REG_H