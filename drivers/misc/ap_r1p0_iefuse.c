/*
 * Copyright (C) 2015 Spreadtrum Communications Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

/* IMPORTANT:
 * The electrical fuse is a type of non-volatile memory fabricated
 * in standard CMOS logic process. This electrical fuse macro is widely
 * used in chip ID, memory redundancy, security code, configuration setting,
 * and feature selection, etc.
 */

#include <asm/io.h>
#include <sprd_glb.h>
#include <asm/arch/sprd_reg.h>
#include <common.h>
#include <linux/types.h>
#include <linux/err.h>

typedef unsigned int u32;

/* registers definitions for controller REGS_EFUSE */

#define REG_EFUSE_DATA_RD               SCI_ADDR(SPRD_UIDEFUSE_BASE, 0x0000)
#define REG_EFUSE_DATA_WR               SCI_ADDR(SPRD_UIDEFUSE_BASE, 0x0004)
#define REG_EFUSE_BLOCK_INDEX      		SCI_ADDR(SPRD_UIDEFUSE_BASE, 0x0008)
#define REG_EFUSE_CFG0	                SCI_ADDR(SPRD_UIDEFUSE_BASE, 0x000c)
#define REG_EFUSE_DC_CFG0               SCI_ADDR(SPRD_UIDEFUSE_BASE, 0x0010)
#define REG_EFUSE_DC_CFG1               SCI_ADDR(SPRD_UIDEFUSE_BASE, 0x0014)
#define REG_EFUSE_RD_CFG              	SCI_ADDR(SPRD_UIDEFUSE_BASE, 0x0018)
#define REG_EFUSE_OP_CFG               	SCI_ADDR(SPRD_UIDEFUSE_BASE, 0x001c)
#define REG_EFUSE_KAR             		SCI_ADDR(SPRD_UIDEFUSE_BASE, 0x0020)
#define REG_EFUSE_MOD_CTRL             	SCI_ADDR(SPRD_UIDEFUSE_BASE, 0x0024)
#define REG_EFUSE_STATUS                SCI_ADDR(SPRD_UIDEFUSE_BASE, 0x0028)
#define REG_EFUSE_IP_REVISION           SCI_ADDR(SPRD_UIDEFUSE_BASE, 0x002c)
#define REG_EFUSE_SECURE_STATUS         SCI_ADDR(SPRD_UIDEFUSE_BASE, 0x0030)
#define REG_EFUSE_CMD_HI32         		SCI_ADDR(SPRD_UIDEFUSE_BASE, 0x0034)
#define REG_EFUSE_CMD_LO32         		SCI_ADDR(SPRD_UIDEFUSE_BASE, 0x0038)
#define REG_EFUSE_LDO_RAISE_TIME        SCI_ADDR(SPRD_UIDEFUSE_BASE, 0x003c)
#define REG_EFUSE_LDO_DROP_TIME         SCI_ADDR(SPRD_UIDEFUSE_BASE, 0x0040)

/* bits definitions for register REG_EFUSE_CFG0 */
#define BIT_PGM_EN                    ( BIT(31) )
#define BIT_EFS_VDD_ON                ( BIT(30) )
#define BIT_EFS_VCCRAM_ON             ( BIT(29) )
#define BIT_EFS_VCCFHV_ON             ( BIT(28) )

/* bits definitions for register REG_EFUSE_DC_CFG1 */
#define BIT_DC_NUM(_x_)         ( (_x_) << 0 & (BIT(0) | BIT(1) | BIT(2) | BIT(3)))
#define BIT_DC_LEN(_x_)         ( (_x_) << 0 & (BIT(5) | BIT(6) | BIT(7) | BIT(8) | BIT(9)))

/* bits definitions for register REG_EFUSE_RD_CFG */
#define BIT_ISENSELV(_x_)         ( (_x_) << 0 & (BIT(0) | BIT(1) | BIT(2) | BIT(3) | BIT(4) | BIT(5)))
#define BIT_IFAVOR_LEN(_x_)       ( (_x_) << 0 & (BIT(6) | BIT(7) | BIT(8)))

/* bits definitions for register REG_EFUSE_OP_CFG */
#define BIT_OP_MODE(_x_)         ( (_x_) << 0 & (BIT(0) | BIT(1) | BIT(2) | BIT(3) | BIT(4)))

/* bits definitions for register REG_EFUSE_MOD_CTRL */
#define BIT_MARGIN_EN                 ( BIT(3) )
#define BIT_STANDBY_EN                ( BIT(2) )
#define BIT_CMD_DERECT_INPUT_SEL      ( BIT(1) )
#define BIT_CMD_INPUT                 ( BIT(0) )

/* bits definitions for register REG_EFUSE_STATUS */
#define BIT_PGM_BUSY                ( BIT(0) )
#define BIT_READ_BUSY               ( BIT(1) )
#define BIT_STANDY_BUSY             ( BIT(2) )
#define BIT_DATA_READY              ( BIT(4) )
#define BIT_CTL_BUSY                ( BIT(24) )

#define ISENSELV 0x000
#define IFAVOR 0x000
#define SHIFT_ISENSELV 0
#define READ_MODE 0x0015

#define KEY_ACCESS_VALUE             ( 0x55aa55)
#define CLRBIT(nr)			(0UL << (nr))
#define CLR_BIT_AON_APB_EFUSE_SOFT_RST CLRBIT(14)
#define CLR_BIT_AON_APB_EFUSE_EB           CLRBIT(13)
#define WHALE_LEAK_BIN_BLOCK 15
#define HWSPINLOCK_TIMEOUT		(5000)

#define EFUSE_BLOCK_MAX                 ( 72 )
#define EFUSE_BLOCK_WIDTH               ( 32 )

#define jiffies (SCI_GetTickCount())	/* return msec count */
#define msecs_to_jiffies(a) (a)
#define time_after(a,b)	((int)(b) - (int)(a) < 0)
#define cpu_relax()

static void msleep(u32 ms)
{
	u32 timeout = jiffies + ms;
	while (!time_after(jiffies, timeout)) {
		;
	}
}

static void ap_efuse_lookat(void)
{
	volatile u32 *reg = (volatile u32 *)SPRD_UIDEFUSE_BASE;
	int cnt = 16;
	while (cnt--) {
		printf("[%p] = %08x\n", reg, *reg);
		reg++;
	}
	printf("\n");
}

static void ap_efuse_reset(void)
{
	/* should enable module before soft reset efuse */
	WARN_ON(!sci_glb_read(REG_AON_APB_APB_EB0, BIT_AON_APB_EFUSE_EB));
	sci_glb_set(REG_AON_APB_APB_RST0, BIT_AON_APB_EFUSE_SOFT_RST);
	msleep(10);
	sci_glb_clr(REG_AON_APB_APB_RST0, BIT_AON_APB_EFUSE_SOFT_RST);
	msleep(10);
}

static __inline int ap_efuse_wait_clear(u32 bits)
{
	int ret = 0;
	unsigned long timeout;

	printf("wait %x\n", __raw_readl((void *)REG_EFUSE_STATUS));

	/* wait for maximum of 300 msec */
	timeout = jiffies + msecs_to_jiffies(300);
	while (__raw_readl((void *)REG_EFUSE_STATUS) & bits) {
		if (time_after(jiffies, timeout)) {
			WARN_ON(1);
			ret = -ETIMEDOUT;
			break;
		}
		cpu_relax();
	}
	return ret;
}

static void ap_efuse_enable(void)
{
	sci_glb_set(REG_AON_APB_APB_EB0, BIT_AON_APB_EFUSE_EB);
	msleep(5);
}

static void ap_efuse_disable(void)
{
	msleep(1);
	sci_glb_clr(REG_AON_APB_APB_EB0, BIT_AON_APB_EFUSE_EB);
}

static void ap_efuse_margin_mode(void)
{
	u32 cfg = __raw_readl((void *)REG_EFUSE_MOD_CTRL);
	cfg &= ~BIT_MARGIN_EN;
	__raw_writel(cfg, (void *)REG_EFUSE_MOD_CTRL);
	msleep(1);
}

static u32 ap_efuse_read(int bank, int blk)
{
	u32 val = 0;
	u32 cfg_temp = 0;
	u32 cfg = 0;

	__raw_writel(READ_MODE, REG_EFUSE_OP_CFG);
	cfg_temp = (ISENSELV << SHIFT_ISENSELV) | IFAVOR;
	__raw_writel(cfg_temp, REG_EFUSE_RD_CFG);
	msleep(1);
	__raw_writel(blk, REG_EFUSE_BLOCK_INDEX);
	msleep(1);
	cfg = __raw_readl((void *)REG_EFUSE_MOD_CTRL);
	cfg |= BIT_CMD_INPUT;
	__raw_writel(cfg, REG_EFUSE_MOD_CTRL);

	if (IS_ERR_VALUE(ap_efuse_wait_clear(BIT_READ_BUSY)))
		goto out;
	val = __raw_readl(REG_EFUSE_DATA_RD);

out:
	return val;
}

u32 sprd_ap_efuse_read(int blk_index)
{
	u32 val;
	int bank_index = 0;

	printf("iefuse read bank ,block %d\n", bank_index, blk_index);
	ap_efuse_enable();
	ap_efuse_margin_mode();
	val = ap_efuse_read(bank_index, blk_index);
	ap_efuse_disable();
	return val;
}

int sprd_ap_efuse_prog(int blk_index, u32 val)
{
	int ret = 0;
	printf("iefuse prog %d %08x\n", blk_index, val);
	return ret;
}

void ap_efuse_block_dump(void)
{
	int idx;
	printf("ap iefuse base %08x\n", SPRD_UIDEFUSE_BASE);
	printf("ap iefuse blocks dump:\n");
	for (idx = 0; idx < EFUSE_BLOCK_MAX; idx++) {
		printf("[%02d] %08x\n", idx, sprd_ap_efuse_read(idx));
	}
}
