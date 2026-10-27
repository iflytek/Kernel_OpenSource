/*
 * SPDX-License-Identifier: GPL-2.0-only
 * Copyright (C) 2023, Anhui Yunshu Zhihui Technology Co., Ltd.
 */
#include "sprd_panel.h"
#include "sprd_dsi.h"
#include "dsi/mipi_dsi_api.h"
#include "sprd_dphy.h"
#include <i2c.h>

extern int sprd_gpio_request(struct gpio_chip *chip, unsigned offset);
int sprd_gpio_direction_output(struct gpio_chip *chip, unsigned offset, int value);

static uint8_t init_data[] = {
	//0x05, 0x22, 0x00, 0x01, 0x10,
//start of vendor init_code
	0x15, 0x00, 0x00, 0x02, 0x00, 0x00,
	0x39, 0x00, 0x00, 0x04, 0xFF, 0x82, 0x01, 0x01,
	0x15, 0x00, 0x00, 0x02, 0x00, 0x80,
	0x39, 0x00, 0x00, 0x03, 0xFF, 0x82, 0x01,
// end of vendor init code

	0x05, 0x78, 0x00, 0x01, 0x11,
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

	printf("mipi_dsi_send_cmds\n");

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

static int lcd_tv101wum_aw2_option_init(void)
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

static int lcd_tv101wum_aw2_option_uninit(void)
{
    struct sprd_dsi *dsi = &dsi_device;

    mipi_dsi_set_work_mode(dsi, SPRD_MIPI_MODE_CMD);
    mipi_dsi_lp_cmd_enable(dsi, true);
    mipi_dsi_send_cmds(dsi, sleep_in_data);

    return 0;
}

static uint8_t readid_begin[] = {
	0x15, 0x00, 0x00, 0x02, 0x00, 0x00,
	0x39, 0x00, 0x00, 0x04, 0xFF, 0x82, 0x01, 0x01,
	0x15, 0x00, 0x00, 0x02, 0x00, 0x80,
	0x39, 0x00, 0x00, 0x03, 0xFF, 0x82, 0x01,
	0x15, 0x00, 0x00, 0x02, 0x00, 0x88,
	0x15, 0x00, 0x00, 0x02, 0xB0, 0x07,
	0x15, 0x00, 0x00, 0x02, 0x00, 0x00,
	0x15, 0x00, 0x00, 0x02, 0xFA, 0x02,
	CMD_END
};

static uint8_t readid_end[] = {
	0x15, 0x00, 0x00, 0x02, 0x00, 0x00,
	0x15, 0x00, 0x00, 0x02, 0xFA, 0x5A,
	0x15, 0x00, 0x00, 0x02, 0x00, 0x88,
	0x15, 0x00, 0x00, 0x02, 0xB0, 0x01,
	CMD_END
};

static int lcd_tv101wum_aw2_option_readid(void)
{

	struct sprd_dsi *dsi = &dsi_device;
	uint8_t read_buf0;

	mipi_dsi_lp_cmd_enable(dsi, true);
	mipi_dsi_send_cmds(dsi,readid_begin);
	mipi_dsi_set_max_return_size(dsi, 1);
	mipi_dsi_dcs_read(dsi, 0xda, &read_buf0, 1);
	mipi_dsi_send_cmds(dsi,readid_end);

	if(0x5 == read_buf0) {
		pr_info("lcd_tv101wum_aw2_option read id success! 0xda==0x%x\n",read_buf0);
		mdelay(20);
		return 0;
    }

	pr_info("lcd_tv101wum_aw2_option read id fail!\n");

	return -1;
}

extern void lcd_set_power_to_5v8(void);
extern void lcd_use_sc2703l_to_power_on(bool on);

static int lcd_tv101wum_aw2_option_power(int on)
{

	printf("lcd_tv101wum_aw2_option power on! %d \n",on);

	if (on) {
		// set power to 0
		sprd_gpio_request(NULL, CONFIG_LCM_GPIO_RSTN);
		sprd_gpio_request(NULL, CONFIG_LCM_TP_RSTN);
		sprd_gpio_direction_output(NULL, CONFIG_LCM_GPIO_RSTN, 0);
		lcd_use_sc2703l_to_power_on(false);
		regulator_disable("vddrf1v8");
		regulator_enable("vdd28");
		lcd_set_power_to_5v8();
		mdelay(100);

		// power timeing begin
		// vddio enable delay > 3ms
		regulator_enable("vddrf1v8");
		mdelay(3);

		sprd_gpio_direction_output(NULL, CONFIG_LCM_TP_RSTN, 1);

		// +-5.8v enable delay > 3ms
		lcd_use_sc2703l_to_power_on(true);
		mdelay(19);

		// reset enable delay > 3
		sprd_gpio_direction_output(NULL, CONFIG_LCM_GPIO_RSTN, 1);
		mdelay(5);
		sprd_gpio_direction_output(NULL, CONFIG_LCM_GPIO_RSTN, 0);
		mdelay(5);
		sprd_gpio_direction_output(NULL, CONFIG_LCM_GPIO_RSTN, 1);
		mdelay(5);
	} else {

		// tp and lcd reset disable delay > 3ms
		sprd_gpio_direction_output(NULL, CONFIG_LCM_TP_RSTN, 0);
		sprd_gpio_direction_output(NULL, CONFIG_LCM_GPIO_RSTN, 0);
		mdelay(5);

		// +-5.8v disable delay > 3ms
        lcd_use_sc2703l_to_power_on(false);
		mdelay(5);

		// vddio off
		regulator_disable("vddrf1v8");
		mdelay(10);
	}

	return 0;
}

static struct panel_ops lcd_tv101wum_aw2_option_ops = {
	.init = lcd_tv101wum_aw2_option_init,
	.read_id = lcd_tv101wum_aw2_option_readid,
	.power = lcd_tv101wum_aw2_option_power,
    .uninit = lcd_tv101wum_aw2_option_uninit,
};

static struct panel_info lcd_tv101wum_aw2_option_info = {
	/* common parameters */
	.lcd_name = "lcd_tv101wum_aw2_option_mipi_hd",
	.type = SPRD_PANEL_TYPE_MIPI,
	.bpp = 24,
//	.fps = 60,
	.width = 1200,
	.height = 1920,

	/* DPI specific parameters */
	.pixel_clk = 153600000, /*Hz*/
	.rgb_timing = {
		.hfp = 8,
		.hbp = 6,
		.hsync = 6,
		.vfp = 185,
		.vbp = 30,
		.vsync = 6,
	},

	/* MIPI DSI specific parameters */
	.phy_freq = 1105000,
	.lane_num = 4,
	.work_mode = SPRD_MIPI_MODE_VIDEO,
	.burst_mode = PANEL_VIDEO_BURST_MODE,
	.nc_clk_en = false,
};

struct panel_driver lcd_tv101wum_aw2_option_driver = {
	.info = &lcd_tv101wum_aw2_option_info,
	.ops = &lcd_tv101wum_aw2_option_ops,
};
