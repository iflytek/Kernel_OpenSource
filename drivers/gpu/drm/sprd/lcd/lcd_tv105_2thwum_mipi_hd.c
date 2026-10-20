// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2023 Anhui Yunshu Zhihui Technology Co., Ltd.
#include<linux/gpio/consumer.h>
#include<linux/regulator/consumer.h>
#include"../sprd_panel.h"
#include"../sprd_dsi.h"
//#include "panel_cfg.h"

extern int sprd_panel_send_cmds(struct mipi_dsi_device *dsi,
				const void *data, int size);

static int tv105_2thwum_init(struct sprd_panel* panel)
{
	printk("lcd init\n");

	sprd_panel_send_cmds(panel->slave,
			     panel->info.cmds[CMD_CODE_INIT],
			     panel->info.cmds_len[CMD_CODE_INIT]);

	return 0;
}

static int tv105_2thwum_uninit(struct sprd_panel* panel) {

	printk("lcd uninit\n");

	sprd_panel_send_cmds(panel->slave,
			     panel->info.cmds[CMD_CODE_SLEEP_IN],
			     panel->info.cmds_len[CMD_CODE_SLEEP_IN]);

    return 0;
}

static int tv105_2thwum_readid(struct sprd_panel* panel)
{

	return 0;
}

static int power_on(struct sprd_panel* panel)
{
	int ret = 0;

	printk("lcd power on\n");

	// vddio en delay > 0ms
	ret = regulator_enable(panel->supply_1v8);

	// +-5v en delay > 6ms
	ret = regulator_enable(panel->supply);
	mdelay(7);

	// reset en delay > 5ms
	ret = gpiod_direction_output(panel->info.reset_gpio,1);
	mdelay(6);
	ret = gpiod_direction_output(panel->info.reset_gpio,0);
	mdelay(6);
	ret = gpiod_direction_output(panel->info.reset_gpio,1);
	mdelay(6);

	return 0;
}

static int power_off(struct sprd_panel* panel)
{
	int ret = 0;

	// disable reset delay > 0ms
	ret = gpiod_direction_output(panel->info.reset_gpio, 0);

	// disable +-5v delay > 0ms
	ret = regulator_disable(panel->supply);

	// disable vddio delay > 0ms
	ret = regulator_disable(panel->supply_1v8);
	mdelay(10);

	return 0;
}

static int tv105_2thwum_power(struct sprd_panel* panel, int on)
{

	if (on) {
		power_on(panel);
	} else {
		power_off(panel);
	}

	return 0;
}

static int tv105_2thwum_get_backlight_delay_time(void)
{
	return 150;
}

static struct panel_ops tv105_2thwum_ops = {
	.init = tv105_2thwum_init,
	.read_id = tv105_2thwum_readid,
	.power = tv105_2thwum_power,
    .uninit = tv105_2thwum_uninit,
	.get_backlight_delay_time = tv105_2thwum_get_backlight_delay_time,
};


struct panel_driver tv105_2thwum_driver = {
	.lcd_name = "lcd_tv105_2thwum_mipi_hd",
	.ops = &tv105_2thwum_ops,
};
