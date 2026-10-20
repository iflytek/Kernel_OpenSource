// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2023 Anhui Yunshu Zhihui Technology Co., Ltd.
#include<linux/gpio/consumer.h>
#include<linux/regulator/consumer.h>
#include"../sprd_panel.h"
#include"../sprd_dsi.h"
//#include "panel_cfg.h"

extern int bestservice_fts_ts_resume(void);
extern int bestservice_fts_ts_suspend(void);
extern int sprd_panel_send_cmds(struct mipi_dsi_device *dsi,
				const void *data, int size);

static int tv101wum_aw2_option_init(struct sprd_panel* panel)
{
	printk("lcd init\n");

	sprd_panel_send_cmds(panel->slave,
			     panel->info.cmds[CMD_CODE_INIT],
			     panel->info.cmds_len[CMD_CODE_INIT]);

	mdelay(10);
	bestservice_fts_ts_resume();
	return 0;
}

static int tv101wum_aw2_option_uninit(struct sprd_panel* panel) {

	printk("lcd uninit\n");

	sprd_panel_send_cmds(panel->slave,
			     panel->info.cmds[CMD_CODE_SLEEP_IN],
			     panel->info.cmds_len[CMD_CODE_SLEEP_IN]);
    return 0;
}

static int tv101wum_aw2_option_readid(struct sprd_panel* panel)
{

	return 0;
}

static int power_on(struct sprd_panel* panel)
{
	int ret = 0;

	gpiod_direction_output(panel->info.reset_gpio, 0);

	// 1v8 on delay > 3ms
	ret = regulator_enable(panel->supply_1v8);
	if (ret < 0)
		DRM_ERROR("enable lcd vddio failed\n");
	mdelay(10);

	//+-5v on delay > 3ms
	ret = regulator_enable(panel->supply);
	if (ret < 0)
		DRM_ERROR("enable lcd +-5v failed\n");
	mdelay(10);

	// rest on delay > 3ms
	ret = gpiod_direction_output(panel->info.reset_gpio, 1);
	mdelay(5);
	ret = gpiod_direction_output(panel->info.reset_gpio, 0);
	mdelay(5);
	ret = gpiod_direction_output(panel->info.reset_gpio, 1);
	mdelay(10);

	printk("lcd power on ok\n");
	return 0;
}

static int power_off(struct sprd_panel* panel)
{
	int ret = 0;

	// reset off delay > 3 ms
	ret = gpiod_direction_output(panel->info.reset_gpio, 0);
	bestservice_fts_ts_suspend();
	mdelay(10);

	// +-5v off delay > 3 ms
	ret = regulator_disable(panel->supply);
	if (ret < 0)
		DRM_ERROR("disable lcd +-5v failed\n");
	mdelay(10);

	// vddio off
	ret = regulator_disable(panel->supply_1v8);
	if (ret < 0)
		DRM_ERROR("disable lcd vddio failed\n");
	mdelay(10);

	printk("lcd power off ok\n");

	return 0;
}

static int tv101wum_aw2_option_power(struct sprd_panel* panel, int on)
{

	if (on) {
		power_on(panel);
	} else {
		power_off(panel);
	}

	return 0;
}

static int tv101wum_aw2_option_get_backlight_delay_time(void)
{
	return 150;
}

static struct panel_ops tv101wum_aw2_option_ops = {
	.init = tv101wum_aw2_option_init,
	.read_id = tv101wum_aw2_option_readid,
	.power = tv101wum_aw2_option_power,
    .uninit = tv101wum_aw2_option_uninit,
	.get_backlight_delay_time = tv101wum_aw2_option_get_backlight_delay_time,
};


struct panel_driver tv101wum_aw2_option_driver = {
	.lcd_name = "lcd_tv101wum_aw2_option_mipi_hd",
	.ops = &tv101wum_aw2_option_ops,
};
