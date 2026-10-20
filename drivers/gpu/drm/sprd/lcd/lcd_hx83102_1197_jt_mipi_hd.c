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
extern void himax_int_enable(int enable);
extern bool if_himax_smwp_enabled(void);
extern volatile bool if_shutdown;

static int hx83102_1197_jt_option_init(struct sprd_panel* panel)
{
	printk("lcd init\n");

	sprd_panel_send_cmds(panel->slave,
				panel->info.cmds[CMD_CODE_INIT],
				panel->info.cmds_len[CMD_CODE_INIT]);

	mdelay(10);
	return 0;
}

static int hx83102_1197_jt_option_uninit(struct sprd_panel* panel) {

	printk("lcd uninit\n");

	sprd_panel_send_cmds(panel->slave,
				panel->info.cmds[CMD_CODE_SLEEP_IN],
				panel->info.cmds_len[CMD_CODE_SLEEP_IN]);
	return 0;
}

static int hx83102_1197_jt_option_readid(struct sprd_panel* panel)
{
	return 0;
}

static int power_on(struct sprd_panel* panel)
{
	int ret = 0;

	himax_int_enable(0);

	ret = regulator_is_enabled(panel->supply_1v8);
	if (ret == 0) {
		ret = regulator_enable(panel->supply_1v8);
		if (ret)
			return ret;
		mdelay(3);
	} else if (ret < 0) {
		return ret;
	}

	gpiod_direction_output(panel->info.en3v3_gpio, 1);
	mdelay(2);
	gpiod_direction_output(panel->info.avdd_gpio, 1);
	mdelay(2);
	gpiod_direction_output(panel->info.avee_gpio, 1);
	mdelay(10);

	gpiod_direction_output(panel->info.reset_gpio, 1);
	mdelay(2);

	gpiod_direction_output(panel->info.reset_gpio, 0);
	mdelay(5);

	gpiod_direction_output(panel->info.reset_gpio, 1);

	gpiod_direction_output(panel->info.bl_enable_gpio, 1);
	mdelay(10);

	mdelay(80);

	printk("lcd power on ok\n");
	return 0;
}

static int power_off(struct sprd_panel* panel)
{
	int ret = 0;
	bool if_smwp_enabled = if_himax_smwp_enabled();

	// reset off delay > 3 ms
	if(!if_smwp_enabled || if_shutdown)
	{
		himax_int_enable(0);
		mdelay(5);
	}

	gpiod_direction_output(panel->info.en3v3_gpio, 0);

	if(!if_smwp_enabled || if_shutdown)
	{
		mdelay(2);
		gpiod_direction_output(panel->info.reset_gpio, 0);
		mdelay(2);

		gpiod_direction_output(panel->info.avee_gpio, 0);
		mdelay(10);
		gpiod_direction_output(panel->info.avdd_gpio, 0);
		mdelay(7);
	}

	if(!if_smwp_enabled || if_shutdown)
	{
		ret = regulator_is_enabled(panel->supply_1v8);
		if(0 < ret)
		{
			ret = regulator_disable(panel->supply_1v8);
		}
	}

	gpiod_direction_output(panel->info.bl_enable_gpio, 0);
	if(!if_smwp_enabled || if_shutdown)
		mdelay(10);

	printk("lcd power off ok\n");

	return 0;
}

static int hx83102_1197_jt_option_power(struct sprd_panel* panel, int on)
{

	if (on) {
		return power_on(panel);
	} else {
		return power_off(panel);
	}
}

static int hx83102_1197_jt_option_get_backlight_delay_time(void)
{
	return 150;
}

static struct panel_ops hx83102_1197_jt_option_ops = {
	.init = hx83102_1197_jt_option_init,
	.read_id = hx83102_1197_jt_option_readid,
	.power = hx83102_1197_jt_option_power,
	.uninit = hx83102_1197_jt_option_uninit,
	.get_backlight_delay_time = hx83102_1197_jt_option_get_backlight_delay_time,
};


struct panel_driver hx83102_1197_jt_option_driver = {
	.lcd_name = "lcd_hx83102_1197_jt_option_mipi_hd",
	.ops = &hx83102_1197_jt_option_ops,
};
