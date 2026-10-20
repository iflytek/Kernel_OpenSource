// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2023 Anhui Yunshu Zhihui Technology Co., Ltd.

#include<linux/gpio/consumer.h>
#include<linux/regulator/consumer.h>
#include"../sprd_panel.h"
#include"../sprd_dsi.h"
//#include "panel_cfg.h"

extern int sprd_panel_send_cmds(struct mipi_dsi_device *dsi,
				const void *data, int size);

static int cblite_5th_option_init(struct sprd_panel* panel)
{
	printk("lcd init\n");

	sprd_panel_send_cmds(panel->slave,
			     panel->info.cmds[CMD_CODE_INIT],
			     panel->info.cmds_len[CMD_CODE_INIT]);

	return 0;
}

static int cblite_5th_option_uninit(struct sprd_panel* panel) {

	printk("lcd uninit\n");

	sprd_panel_send_cmds(panel->slave,
			     panel->info.cmds[CMD_CODE_SLEEP_IN],
			     panel->info.cmds_len[CMD_CODE_SLEEP_IN]);

    return 0;
}

static int cblite_5th_option_readid(struct sprd_panel* panel)
{

	return 0;
}

static int power_on(struct sprd_panel* panel)
{
	int ret = 0;

	ret = gpiod_direction_output(panel->info.en3v3_gpio, 1);
	mdelay(20);

	return 0;
}

static int power_off(struct sprd_panel* panel)
{
	int ret = 0;

	ret = gpiod_direction_output(panel->info.en3v3_gpio, 0);
	mdelay(20);

	return 0;
}

static int cblite_5th_option_power(struct sprd_panel* panel, int on)
{

	if (on) {
		power_on(panel);
	} else {
		power_off(panel);
	}

	return 0;
}

static int cblite_5th_option_get_backlight_delay_time(void)
{
	return 150;
}

static struct panel_ops cblite_5th_option_ops = {
	.init = cblite_5th_option_init,
	.read_id = cblite_5th_option_readid,
	.power = cblite_5th_option_power,
    .uninit = cblite_5th_option_uninit,
	.get_backlight_delay_time = cblite_5th_option_get_backlight_delay_time,
};


struct panel_driver cblite_5th_option_driver = {
	.lcd_name = "lcd_cblite_5th_option_mipi_hd",
	.ops = &cblite_5th_option_ops,
};
