/*
 * Copyright (C) 2018 Spreadtrum Communications Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _UFS_REG_V0_H_
#define _UFS_REG_V0_H_

struct ufs_cfg_info {
	uint32_t  ufs_regbase;
	uint32_t  unipro_regbase;

	uint32_t  apb_enable_reg;
	uint32_t  apb_enable_bit;

	uint32_t  ao_aon_apb_enable_reg;
	uint32_t  ao_aon_apb_enable_bit;

	uint32_t  ao_aon_apb_reset_reg;
	uint32_t  ao_aon_apb_reset_bit;

	uint32_t  apb_reset_reg;
	uint32_t  apb_reset_bit;

	uint32_t  ao_regbase;
	uint32_t  ao_rest_bit;

	uint32_t  utp_reset_reg;
	uint32_t  utp_reset_bit;

	uint32_t  anlg_phy_g1_reg;
	uint32_t  anlg_phy_g1_reset_bit;

	uint32_t  pmu_bypass_enable_reg;
	uint32_t  pmu_bypass_enable_bit;

	const char *ldo_core;
	const char *ldo_io;
};

struct ufs_cfg_info *get_ufscontrol_info(void);

#endif
