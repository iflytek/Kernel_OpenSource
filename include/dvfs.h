/*
 * Copyright (C) 2016, Intel Corporation
 *
 * SPDX-License-Identifier:     GPL-2.0+
 *
 */

#ifndef __DVFS_H__
#define __DVFS_H__

#define NO_OF_PSTATES                   0x6
#define LFM_INDEX                       0x0
#define HFM_INDEX                       0x5
#define FSB_FREQ_MHZ			0x4E
#define VT_BASE_OFF			0x20
#define MODULE0				0x00
#define MODULE1				0x01
#define ADDR_DVFS_EN_MOD0		0xE44B0000
#define ADDR_DVFS_EN_MOD1		0xE44C0000
#define VT_MOD0				(ADDR_DVFS_EN_MOD0 + VT_BASE_OFF)
#define VT_MOD1				(ADDR_DVFS_EN_MOD1 + VT_BASE_OFF)
#define OFF_ADI_DLY1			0x14
#define OFF_ADI_DLY2			0x60
#define OFF_ADI_DLY3			0x64

#define DVFS_ENABLE		((ADI_MODE << VLT_MAP_WORK_MODE_BIT) | \
				(0x1 << VDDCPU_SHUTDWN_BIT) | \
				(BAD_CFG_VLT << BAD_CFG_BIT))

/* 100mV stepping 100us	*/
#define LAT_2_UPDATE_VDDCPU		0x00000100
#define LAT_2_RAMPDN_VDDCPU_TO_0	0x00000200
#define LAT_2_RAMPUP_VDDCPU_TO_V2	0x00000300

#define VLT_MAP_WORK_MODE_BIT		0x0
#define	ADI_MODE			0x1
#define SOFT_MODE			0x3

#define VDDCPU_SHUTDWN_BIT		0x5
#define BAD_CFG_VLT			0xBAD
#define BAD_CFG_BIT			16

#define VF_TABLE_LOCK_BIT		7
#define	VF_TABLE_LOCK			1
#define	VF_TABLE_UNLOCK			0

#define ADI_BASE_ADDR			0xE4030000
#define OFF_ADI_CHNLS			0x40
#define ENABLE_ADI_CHNL5		(0x1 << 5)
#define ENABLE_ADI_CHNL11		(0x1 << 11)

#define OFF_ADI_RF_GSSI			0x24
/* Enable 2 wire + 4 dummy clocks + sample tx at negative edge */
#define ENABLE_2WIRE			((0x1 << RF_GSSI_STRTBIT_MODE_BIT) | \
					(0x3 << RF_GSSI_DUMMY_LEN_BIT) | \
					(0x1 << RF_GSSI_NG_TX))

#define RF_GSSI_STRTBIT_MODE_BIT	31
#define RF_GSSI_DUMMY_LEN_BIT		16
#define RF_GSSI_NG_TX			0

#define OFF_PMU_CHNL02			0x44
#define CHNL02_ADDR_BIT			0x0
#define CHNL02_ADDR			0x08D78

#define OFF_PMU_CHNL05			0x50
#define CHNL05_ADDR_BIT			0x0
#define CHNL05_ADDR			0x08C54

#define OFF_PMU_CHNL11			0x68
#define CHNL11_ADDR_BIT			0x0
#define CHNL11_ADDR			0x08C64

#define PMIC_VLT_AVAIL_BIT		31
#define PMIC_CTRL_BIT			5
#define PMIC_CAL_BIT			0

#define PMIC_VLT_INTERCEPT		400
#define PMIC_CTRL_MUL			100

/* FIXED POINT CAL PRECISION	3.125 * 256*/
#define PMIC_CAL_PRECISION		800

#define FRAC_BITS 8
#define int_tofp(X) ((int64_t)(X) << FRAC_BITS)
#define fp_toint(X) ((X) >> FRAC_BITS)

enum vid_encode {
	VID_V0 = 0x0,
	VID_V1,
	VID_V2,
	VID_V3,
	VID_V4,
	VID_V5,
	VID_V6,
	VID_V7,
	VID_INVALID,
};

struct pmic_vid {
	u32 cal:5;
	u32 ctrl:5;
	u32 res:21;
	u32 avail:1;
};

static inline int32_t mul_fp(int32_t x, int32_t y)
{
	return ((int64_t)x * (int64_t)y) >> FRAC_BITS;
}

static inline int32_t div_fp(int32_t x, int32_t y)
{
	return ((int64_t)x << FRAC_BITS) / y;
}

static inline int ceiling_fp(int32_t x)
{
	int mask, ret;

	ret = fp_toint(x);
	mask = (1 << FRAC_BITS) - 1;
	if (x & mask)
		ret += 1;
	return ret;
}
#endif
