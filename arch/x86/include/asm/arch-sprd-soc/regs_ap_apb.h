/*
 * Copyright (C) 2015, Intel Corporation
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef _ASM_ARCH_REGS_AP_APB_H_
#define _ASM_ARCH_REGS_AP_APB_H_

#ifndef BIT
#define BIT(x) (1<<x)
#endif

#if defined(CONFIG_SPRD_SOC_ISHARKL)
#define REGS_AP_APB_BASE	0xe8300000
#elif defined(CONFIG_SPRD_SOC_IWHALE2)
#define REGS_AP_APB_BASE	0xe42e0000
#endif

/* registers definitions for AP_APB */
#define REG_AP_APB_APB_EB		(REGS_AP_APB_BASE + 0x0000)
#define REG_AP_APB_APB_RST		(REGS_AP_APB_BASE + 0x0004)
#define REG_AP_APB_APB_MISC_CTRL	(REGS_AP_APB_BASE + 0x0008)

/* bits definitions for register REG_AP_APB_APB_EB */
#define BIT_INTC3_EB			BIT(22)
#define BIT_INTC2_EB			BIT(21)
#define BIT_INTC1_EB			BIT(20)
#define BIT_INTC0_EB			BIT(19)
#define BIT_CKG_EB			BIT(18)
#define BIT_AP_CKG_EB			BIT_CKG_EB
#define BIT_UART4_EB			BIT(17)
#define BIT_UART3_EB			BIT(16)
#define BIT_UART2_EB			BIT(15)
#define BIT_UART1_EB			BIT(14)
#define BIT_UART0_EB			BIT(13)
#define BIT_I2C4_EB			BIT(12)
#define BIT_I2C3_EB			BIT(11)
#define BIT_I2C2_EB			BIT(10)
#define BIT_I2C1_EB			BIT(9)
#define BIT_I2C0_EB			BIT(8)
#define BIT_SPI2_EB			BIT(7)
#define BIT_SPI1_EB			BIT(6)
#define BIT_SPI0_EB			BIT(5)
#define BIT_IIS3_EB			BIT(4)
#define BIT_IIS2_EB			BIT(3)
#define BIT_IIS1_EB			BIT(2)
#define BIT_IIS0_EB			BIT(1)
#define BIT_SIM0_EB			BIT(0)

/* bits definitions for register REG_AP_APB_APB_RST */
#define BIT_INTC3_SOFT_RST		BIT(22)
#define BIT_INTC2_SOFT_RST		BIT(21)
#define BIT_INTC1_SOFT_RST		BIT(20)
#define BIT_INTC0_SOFT_RST		BIT(19)
#define BIT_CKG_SOFT_RST		BIT(18)
#define BIT_AP_CKG_SOFT_RST		BIT_CKG_SOFT_RST
#define BIT_UART4_SOFT_RST		BIT(17)
#define BIT_UART3_SOFT_RST		BIT(16)
#define BIT_UART2_SOFT_RST		BIT(15)
#define BIT_UART1_SOFT_RST		BIT(14)
#define BIT_UART0_SOFT_RST		BIT(13)
#define BIT_I2C4_SOFT_RST		BIT(12)
#define BIT_I2C3_SOFT_RST		BIT(11)
#define BIT_I2C2_SOFT_RST		BIT(10)
#define BIT_I2C1_SOFT_RST		BIT(9)
#define BIT_I2C0_SOFT_RST		BIT(8)
#define BIT_SPI2_SOFT_RST		BIT(7)
#define BIT_SPI1_SOFT_RST		BIT(6)
#define BIT_SPI0_SOFT_RST		BIT(5)
#define BIT_IIS3_SOFT_RST		BIT(4)
#define BIT_IIS2_SOFT_RST		BIT(3)
#define BIT_IIS1_SOFT_RST		BIT(2)
#define BIT_IIS0_SOFT_RST		BIT(1)
#define BIT_SIM0_SOFT_RST		BIT(0)

#endif /* _ASM_ARCH_REGS_AP_APB_H_ */
