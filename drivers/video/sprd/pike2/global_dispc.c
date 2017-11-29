/*
 * Copyright (C) 2017 Spreadtrum Communications Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <asm/arch/sprd_reg.h>
#include "sprd_dispc.h"

enum {
	CLK_DPU_SEL_128M = 0,
	CLK_DPU_SEL_153_6M,
	CLK_DPU_SEL_192M,
	CLK_DPU_SEL_256M,
	CLK_DPU_SEL_320M,
	CLK_DPU_SEL_384M,
};

enum {
	CLK_DPI_SEL_96M = 0,
	CLK_DPI_SEL_100M,
	CLK_DPI_SEL_128M,
	CLK_DPI_SEL_153_6M,
	CLK_DPI_SEL_192M,
};

#ifndef CONFIG_DPI_CLK_SRC
#define CONFIG_DPI_CLK_SRC 128000000
#endif

static int dispc_clk_init(struct dispc_context *ctx)
{
	uint32_t dpi_clk_src = CONFIG_DPI_CLK_SRC;

	sci_glb_write(REG_AP_CLK_DISPC0_CFG,
			CLK_DPU_SEL_384M, BIT(0) | BIT(1) | BIT(2));

	switch (dpi_clk_src) {
	case 128000000:
		sci_glb_write(REG_AP_CLK_DISPC0_DPI_CFG,
				CLK_DPI_SEL_128M, BIT(0) | BIT(1) | BIT(2));
		break;
	case 153600000:
		sci_glb_write(REG_AP_CLK_DISPC0_DPI_CFG,
				CLK_DPI_SEL_153_6M, BIT(0) | BIT(1) | BIT(2));
		break;
	case 192000000:
		sci_glb_write(REG_AP_CLK_DISPC0_DPI_CFG,
				CLK_DPI_SEL_192M, BIT(0) | BIT(1) | BIT(2));
		break;
	default:
		pr_info("invalid dpi clk source: %d\n", dpi_clk_src);
		break;
	}

	return 0;
}

static int dispc_clk_update(struct dispc_context *ctx, int clk_id, int val)
{
	uint32_t div;

	div = CONFIG_DPI_CLK_SRC / val;
	if (CONFIG_DPI_CLK_SRC - div * val > (val / 2))
		div++;
	if ((div == 0) || (div > 0x10)) {
		pr_err("invalid dpi clk dividor (%d)\n", div);
		return -1;
	}

	sci_glb_write(REG_AP_CLK_DISPC0_DPI_CFG, (div - 1) << 8, (0xF << 8));

	pr_info("the actual dpi_clk = %d\n", CONFIG_DPI_CLK_SRC / div);
	return 0;
}

static int dispc_glb_parse_dt(struct dispc_context *ctx)
{
	ctx->base = SPRD_LCDC_PHYS;

	return 0;
}

static void dispc_glb_enable(struct dispc_context *ctx)
{
	sci_glb_set(REG_AP_AHB_AHB_EB, BIT_AP_AHB_DISPC_EB);
}

static void dispc_glb_disable(struct dispc_context *ctx)
{
	sci_glb_clr(REG_AP_AHB_AHB_EB, BIT_AP_AHB_DISPC_EB);
}

static void dispc_reset(struct dispc_context *ctx)
{
	sci_glb_set(REG_AP_AHB_AHB_RST, BIT_AP_AHB_DISPC_SOFT_RST);
	udelay(10);
	sci_glb_clr(REG_AP_AHB_AHB_RST, BIT_AP_AHB_DISPC_SOFT_RST);
}

static void dispc_power_domain(struct dispc_context *ctx, int enable)
{
}

static struct dispc_clk_ops dispc_clk_ops = {
	.init = dispc_clk_init,
	.update = dispc_clk_update,
};

static struct dispc_glb_ops dispc_glb_ops = {
	.parse_dt = dispc_glb_parse_dt,
	.reset = dispc_reset,
	.enable = dispc_glb_enable,
	.disable = dispc_glb_disable,
	.power = dispc_power_domain,
};

struct dispc_clk_ops *dispc_clk_ops_attach(void)
{
	return &dispc_clk_ops;
}

struct dispc_glb_ops *dispc_glb_ops_attach(void)
{
	return &dispc_glb_ops;
}
