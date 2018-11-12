#ifndef __ARCH_ARM_ASM_HW_I2C__
#define __ARCH_ARM_ASM_HW_I2C__

#include <asm/arch/sprd_reg.h>

struct sprd_hw_i2c {
	void *apb_base;
	unsigned apb_eb;
	void *base;
};

#define DVFS_ADDR(DA, RA)		(((DA) << 8 | (RA)) << 2)

#define SPRD_I2C_NUM		1

#define	DVFS_TX_5_ADDR		DVFS_ADDR(CONFIG_DVFS_ADDR, CINFIG_DVFS_TX_5_REG_ADDR)
#define	DVFS_TX_7_ADDR		DVFS_ADDR(CONFIG_DVFS_ADDR, CINFIG_DVFS_TX_7_REG_ADDR)
#define	DVFS_TX_5_CHNL		5
#define	DVFS_TX_7_CHNL		7

struct sprd_hw_i2c sprd_hw_i2c[SPRD_I2C_NUM] = {
	{
		.apb_base = (void *)REG_AON_APB_APB_EB2,
		.apb_eb = BIT_AON_APB_I2C_EB,
		.base = (void *)SPRD_I2C_PHYS,
	},
};

#endif /*__ARCH_ARM_ASM_HW_I2C__*/
