#ifndef _SPRD_REG_H_
#define _SPRD_REG_H_

#ifndef BIT
#define BIT(x) (1<<x)
#endif

#include <config.h>
#include "hardware.h"
#include "sprd_module_config.h"
#include <power/sprd_pmic/pmic_glb_reg.h>

#if defined(CONFIG_SOC_SHARKLE)
#include "./chip_sharkle/__regs_anlg_phy_g1.h"
#include "./chip_sharkle/__regs_anlg_phy_g2.h"
#include "./chip_sharkle/__regs_anlg_phy_g3.h"
#include "./chip_sharkle/__regs_anlg_phy_g4.h"
#include "./chip_sharkle/__regs_anlg_phy_g5.h"
#include "./chip_sharkle/__regs_aon_apb.h"
#include "./chip_sharkle/__regs_aon_clk.h"
#include "./chip_sharkle/__regs_aon_sec_apb.h"
#include "./chip_sharkle/__regs_ap_ahb.h"
#include "./chip_sharkle/__regs_ap_apb.h"
#include "./chip_sharkle/__regs_ap_clk.h"
#include "./chip_sharkle/__regs_arm7_ahb.h"
#include "./chip_sharkle/__regs_disp_sys_ahb.h"
#include "./chip_sharkle/__regs_gpu_apb.h"
#include "./chip_sharkle/__regs_mm_ahb.h"
#include "./chip_sharkle/__regs_mm_clk.h"
#include "./chip_sharkle/__regs_pmu_apb.h"
#include "./chip_sharkle/__regs_pre_div_clk_gen.h"
#include "./chip_sharkle/__regs_scc_apb.h"
#endif

#endif

