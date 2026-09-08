/*
* Copyright (C) 2021, Anhui Yunshu Zhihui Technology Co., Ltd.
*
* This software is licensed under the terms of the GNU General Public
* License version 2, as published by the Free Software Foundation, and
* may be copied, distributed, and modified under those terms.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*/

#include "sprd_panel.h"
#include "sprd_dsi.h"
#include "dsi/mipi_dsi_api.h"
#include "sprd_dphy.h"
#include <i2c.h>
static uint8_t init_data[] = {

	0x05, 0x64, 0x00, 0x01, 0x11,
	0x05, 0x32, 0x00, 0x01, 0x29,
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

static int lt9711_init(void)
{
	struct sprd_dsi *dsi = &dsi_device;
	struct sprd_dphy *dphy = &dphy_device;

#ifdef CONFIG_MIPI_TO_HDMI_SWITCH
        sprd_gpio_request(NULL,28);
        sprd_gpio_direction_output(NULL,28,0);
        mdelay(10);

        sprd_gpio_request(NULL,31);
        sprd_gpio_direction_output(NULL,31,0);
        mdelay(10);
#endif

	pr_info("lt9711_power\n");
	mipi_dsi_lp_cmd_enable(dsi, true);
	mipi_dsi_set_work_mode(dsi, SPRD_MIPI_MODE_VIDEO);
	mipi_dsi_state_reset(dsi);
	mipi_dphy_hs_clk_en(dphy, true);

	return 0;
}

static int lt9711_readid(void)
{
	return 0;
}

static int lt9711_power(int on)
{
	u8 value1,value2;


	if (on) {

		//regulator_enable("vddldo1");
		lcd_use_sc2703l_to_power_on(true);
		mdelay(7);
/*
#ifdef CONFIG_LCM_GPIO_AVDDEN
                sprd_gpio_request(NULL, CONFIG_LCM_GPIO_AVDDEN);
                sprd_gpio_direction_output(NULL, CONFIG_LCM_GPIO_AVDDEN, 1);
                mdelay(10);
#endif
#ifdef CONFIG_LCM_GPIO_AVEEEN
                sprd_gpio_request(NULL, CONFIG_LCM_GPIO_AVEEEN);
                sprd_gpio_direction_output(NULL, CONFIG_LCM_GPIO_AVEEEN, 1);
                mdelay(20);
#endif
*/
		printf("lt9711_power yanshi\n");
		sprd_gpio_request(NULL, CONFIG_LCM_GPIO_RSTN);
		sprd_gpio_request(NULL, 157);
		sprd_gpio_request(NULL, 22);
		sprd_gpio_request(NULL, 159);
		sprd_gpio_request(NULL, 139);
		sprd_gpio_request(NULL, 9);
		sprd_gpio_request(NULL, 62);
		sprd_gpio_request(NULL, 136);
		sprd_gpio_request(NULL, 50);

		sprd_gpio_direction_output(NULL, 139, 0);
		sprd_gpio_direction_output(NULL, CONFIG_LCM_GPIO_RSTN, 0);
		sprd_gpio_direction_output(NULL, 22, 1);
		mdelay(2);
		sprd_gpio_direction_output(NULL, 157, 1);
		mdelay(2);
		sprd_gpio_direction_output(NULL, 159, 1);
		sprd_gpio_direction_output(NULL, 62, 1);
		sprd_gpio_direction_output(NULL, 136, 1);
		mdelay(100);
		sprd_gpio_direction_output(NULL, CONFIG_LCM_GPIO_RSTN, 1);
		sprd_gpio_direction_output(NULL, 50, 1);
		sprd_gpio_direction_output(NULL, 139, 1);
		mdelay(7);
		sprd_gpio_direction_output(NULL, 9, 1);

		i2c_set_bus_num(2);
		i2c_reg_write(0x41, 0xFF, 0x80);
		i2c_reg_write(0x41, 0xEE, 0x01);
		i2c_reg_write(0x41, 0xFF, 0x81);
		value1 = i2c_reg_read(0x41, 0x00);
		value2 = i2c_reg_read(0x41, 0x01);
		printk("lt9711_power value1=%x value2=%x \n",value1,value2);
		i2c_reg_write(0x41, 0xFF, 0x80);
		i2c_reg_write(0x41, 0xEE, 0x00);
		if(value1!=0x18 || value2!=0x02){
			printk("lt9711_power 50 point reset\n");
			sprd_gpio_direction_output(NULL, 50, 0);
			mdelay(1500);
			sprd_gpio_direction_output(NULL, 50, 1);
		}


	} else {
		sprd_gpio_direction_output(NULL, CONFIG_LCM_GPIO_RSTN, 0);
		sprd_gpio_direction_output(NULL, 157, 0);
		sprd_gpio_direction_output(NULL, 22, 0);
		sprd_gpio_direction_output(NULL, 159, 0);
		sprd_gpio_direction_output(NULL, 139, 0);
		mdelay(6);
        lcd_use_sc2703l_to_power_on(false);
	}

	return 0;
}
static int lt9711_uninit(void)
{
    struct sprd_dsi *dsi = &dsi_device;

    mipi_dsi_set_work_mode(dsi, SPRD_MIPI_MODE_CMD);
    mipi_dsi_lp_cmd_enable(dsi, true);
    mipi_dsi_send_cmds(dsi, sleep_in_data);

    return 0;
}

static struct panel_ops lt9711_ops = {
	.init = lt9711_init,
	.read_id = lt9711_readid,
	.power = lt9711_power,
	.uninit = lt9711_uninit,
};

static struct panel_info lt9711_info = {
	/* common parameters */
	.lcd_name = "lcd_lt9711_mipi_fhd",
	.type = SPRD_PANEL_TYPE_MIPI,
	.bpp = 24,
//	.fps = 60,
	.width = 2160,
	.height = 1350,

	/* DPI specific parameters */
	.pixel_clk = 192000000, /*Hz*/
	.rgb_timing = {
		.hfp = 120,
		.hbp = 120,
		.hsync = 129,
		.vfp = 8,
		.vbp = 8,
		.vsync = 1,
	},

	/* MIPI DSI specific parameters */
	.phy_freq = 1382000,
	.lane_num = 4,
	.work_mode = SPRD_MIPI_MODE_VIDEO,
	.burst_mode = PANEL_VIDEO_BURST_MODE,
	.nc_clk_en = false,
};

struct panel_driver lt9711_driver = {
	.info = &lt9711_info,
	.ops = &lt9711_ops,
};
