#ifndef __ARCH_ARM_ASM_HW_I2C__
#define __ARCH_ARM_ASM_HW_I2C__

#include <asm/arch/sprd_reg.h>

struct sprd_hw_i2c {
	void *apb_base;
	unsigned apb_eb;
	void *base;
};

#define SPRD_I2C_NUM		1

#define	DVFS_RX_ADDR		0x18000
#define DVFS_TX_ADDR		0x18000
#define	DVFS_RX_CHNL		5
#define	DVFS_TX_CHNL		6

struct sprd_hw_i2c sprd_hw_i2c[SPRD_I2C_NUM] = {
	{
		.apb_base = (void *)REG_AON_APB_AON_CLK_TOP_CFG,
		.apb_eb = BIT_AON_APB_CLK_HW_I2C_EB,
		.base = (void *)SPRD_I2C_PHYS,
	},
};

#endif /*__ARCH_ARM_ASM_HW_I2C__*/
