/*
 * SPDX-License-Identifier: GPL-2.0-only
 * Copyright (C) 2023, Anhui Yunshu Zhihui Technology Co., Ltd.
 */
#include "sprd_panel.h"
#include "sprd_dsi.h"
#include "dsi/mipi_dsi_api.h"
#include "sprd_dphy.h"
#include <i2c.h>

extern bool is_sc2703l_lcd_pwer_on(void);
extern void lcd_use_sc2703l_to_power_on(bool on);


static uint8_t init_data[] = {
	0x05, 0x22, 0x00, 0x01, 0x10,
	0x05, 0xC8, 0x00, 0x01, 0x11,
	0x05, 0x64, 0x00, 0x01, 0x29,
	CMD_END
};

static uint8_t sleep_in_data[] = {
    0x15, 0x0A, 0x00, 0x01, 0x28,
    0x15, 0x65, 0x00, 0x01, 0x10,
    CMD_END
};

static int mipi_dsi_send_cmds(struct sprd_dsi *dsi, void *data)
{
	uint16_t len;
	struct dsi_cmd_desc *cmds = data;

	if ((cmds == NULL) || (dsi == NULL))
		return -1;

	for (; cmds->data_type != CMD_END;) {
		len = (cmds->wc_h << 8) | cmds->wc_l;
		mipi_dsi_dcs_write(dsi, cmds->payload, len);
		if (cmds->wait)
			msleep(cmds->wait);
		cmds = (struct dsi_cmd_desc *)(cmds->payload + len);
	}
	return 0;
}

static int cblite_5th_option_init(void)
{
	struct sprd_dsi *dsi = &dsi_device;
	struct sprd_dphy *dphy = &dphy_device;

	mipi_dsi_lp_cmd_enable(dsi, true);
	mipi_dsi_send_cmds(dsi, init_data);
	mipi_dsi_set_work_mode(dsi, SPRD_MIPI_MODE_VIDEO);
	mipi_dsi_state_reset(dsi);
	mipi_dphy_hs_clk_en(dphy, true);

	return 0;
}

static int cblite_5th_option_uninit(void)
{
    struct sprd_dsi *dsi = &dsi_device;

    mipi_dsi_set_work_mode(dsi, SPRD_MIPI_MODE_CMD);
    mipi_dsi_lp_cmd_enable(dsi, true);
    mipi_dsi_send_cmds(dsi, sleep_in_data);

    return 0;
}

static int cblite_5th_option_readid(void)
{
	int gpio_value=0;

	if(is_sc2703l_lcd_pwer_on())
		lcd_use_sc2703l_to_power_on(false);
	pr_info("cblite read id begin!\n");

	// 读 id 脚需要该引脚供电
	regulator_enable("vddrf1v8");
	mdelay(100);

	sprd_gpio_request(NULL, 62);
	sprd_gpio_direction_input(NULL, 62);
	gpio_value=sprd_gpio_get(NULL,62);
	pr_info("cblite_5th_option_readid %d\n",gpio_value);

	// 用完关掉，该屏无需使用该引脚
	regulator_disable("vddrf1v8");

	if(gpio_value>10000)
		return 0;
	else
		return -1;
}

#if 0
static int cblite_5th_option_read_boardid(void)
{
	return 1;
}
#endif

static int cblite_5th_option_power(int on)
{

	if (on) {
		sprd_gpio_request(NULL, CONFIG_3V3_GPIO_ENABLE);
		sprd_gpio_direction_output(NULL, CONFIG_3V3_GPIO_ENABLE, 1);
        mdelay(16);

        sprd_gpio_request(NULL, CONFIG_LCM_GPIO_RSTN);
        sprd_gpio_direction_output(NULL, CONFIG_LCM_GPIO_RSTN, 0);
        mdelay(50);
        sprd_gpio_direction_output(NULL, CONFIG_LCM_GPIO_RSTN, 1);
        mdelay(50);
	} else {
		sprd_gpio_direction_output(NULL, CONFIG_3V3_GPIO_ENABLE, 0);
		mdelay(50);
	}

	return 0;
}

static struct panel_ops cblite_5th_option_ops = {
	.init = cblite_5th_option_init,
	.read_id = cblite_5th_option_readid,
//	.read_board_id = cblite_5th_option_read_boardid,
	.power = cblite_5th_option_power,
    .uninit = cblite_5th_option_uninit,
};

static struct panel_info cblite_5th_option_info = {
	/* common parameters */
	.lcd_name = "lcd_cblite_5th_option_mipi_hd",
	.type = SPRD_PANEL_TYPE_MIPI,
	.bpp = 24,
//	.fps = 60,
	.width = 1200,
	.height = 1920,

	/* DPI specific parameters */
	.pixel_clk = 153600000, /*Hz*/
	.rgb_timing = {
		.hfp = 42,
		.hbp = 32,
		.hsync = 1,
		.vfp = 35,
		.vbp = 25,
		.vsync = 1,
	},

	/* MIPI DSI specific parameters */
	.phy_freq = 1105000,
	.lane_num = 4,
	.work_mode = SPRD_MIPI_MODE_VIDEO,
	.burst_mode = PANEL_VIDEO_BURST_MODE,
	.nc_clk_en = false,
};

struct panel_driver cblite_5th_option_driver = {
	.info = &cblite_5th_option_info,
	.ops = &cblite_5th_option_ops,
};
