// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2023 Anhui Yunshu Zhihui Technology Co., Ltd.

#include<linux/gpio/consumer.h>
#include<linux/regulator/consumer.h>
#include"../sprd_panel.h"
#include"../sprd_dsi.h"
//#include "panel_cfg.h"

extern int sprd_panel_send_cmds(struct mipi_dsi_device *dsi,
				const void *data, int size);

static int dummy_mipi_init(struct sprd_panel* panel)
{
	printk("lcd init\n");

	sprd_panel_send_cmds(panel->slave,
			     panel->info.cmds[CMD_CODE_INIT],
			     panel->info.cmds_len[CMD_CODE_INIT]);

	return 0;
}

static int dummy_mipi_uninit(struct sprd_panel* panel) {

	printk("lcd uninit\n");

	sprd_panel_send_cmds(panel->slave,
			     panel->info.cmds[CMD_CODE_SLEEP_IN],
			     panel->info.cmds_len[CMD_CODE_SLEEP_IN]);

    return 0;
}

static int dummy_mipi_readid(struct sprd_panel* panel)
{

	return 0;
}

static int power_on(struct sprd_panel* panel)
{

	return 0;
}

static int power_off(struct sprd_panel* panel)
{

	return 0;
}

static int dummy_mipi_power(struct sprd_panel* panel, int on)
{

	if (on) {
		power_on(panel);
	} else {
		power_off(panel);
	}

	return 0;
}

static int dummy_mipi_get_backlight_delay_time(void)
{
	return 150;
}

static struct panel_ops dummy_mipi_ops = {
	.init = dummy_mipi_init,
	.read_id = dummy_mipi_readid,
	.power = dummy_mipi_power,
    .uninit = dummy_mipi_uninit,
	.get_backlight_delay_time = dummy_mipi_get_backlight_delay_time,
};


struct panel_driver dummy_mipi_driver = {
	.lcd_name = "lcd_dummy_mipi_hd",
	.ops = &dummy_mipi_ops,
};
