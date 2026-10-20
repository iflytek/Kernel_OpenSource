// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2023 Anhui Yunshu Zhihui Technology Co., Ltd.
#include<linux/gpio/consumer.h>
#include<linux/regulator/consumer.h>
#include"../sprd_panel.h"
#include"../sprd_dsi.h"
//#include "panel_cfg.h"

extern int sprd_panel_send_cmds(struct mipi_dsi_device *dsi,
				const void *data, int size);

static int lcd_tv105wum_aw3_init(struct sprd_panel* panel)
{
	printk("lcd init\n");

	sprd_panel_send_cmds(panel->slave,
			     panel->info.cmds[CMD_CODE_INIT],
			     panel->info.cmds_len[CMD_CODE_INIT]);

	return 0;
}

static int lcd_tv105wum_aw3_uninit(struct sprd_panel* panel) {

	printk("lcd uninit\n");

	sprd_panel_send_cmds(panel->slave,
			     panel->info.cmds[CMD_CODE_SLEEP_IN],
			     panel->info.cmds_len[CMD_CODE_SLEEP_IN]);

    return 0;
}

static int lcd_tv105wum_aw3_readid(struct sprd_panel* panel)
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
	mdelay(10);
	ret = gpiod_direction_output(panel->info.reset_gpio,0);
	mdelay(10);
	ret = gpiod_direction_output(panel->info.reset_gpio,1);
	mdelay(10);

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

static int lcd_tv105wum_aw3_power(struct sprd_panel* panel, int on)
{

	if (on) {
		power_on(panel);
	} else {
		power_off(panel);
	}

	return 0;
}

static int lcd_tv105wum_aw3_get_backlight_delay_time(void)
{
	return 150;
}

static struct panel_ops lcd_tv105wum_aw3_ops = {
	.init = lcd_tv105wum_aw3_init,
	.read_id = lcd_tv105wum_aw3_readid,
	.power = lcd_tv105wum_aw3_power,
    .uninit = lcd_tv105wum_aw3_uninit,
	.get_backlight_delay_time = lcd_tv105wum_aw3_get_backlight_delay_time,
};


struct panel_driver lcd_tv105wum_aw3_driver = {
	.lcd_name = "lcd_tv105wum_aw3_mipi_hd",
	.ops = &lcd_tv105wum_aw3_ops,
};
