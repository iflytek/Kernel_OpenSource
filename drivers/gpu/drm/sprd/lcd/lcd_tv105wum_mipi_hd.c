// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2023 Anhui Yunshu Zhihui Technology Co., Ltd.
#include<linux/gpio/consumer.h>
#include<linux/regulator/consumer.h>
#include"../sprd_panel.h"
#include"../sprd_dsi.h"
//#include "panel_cfg.h"

extern int sprd_panel_send_cmds(struct mipi_dsi_device *dsi,
				const void *data, int size);

static int tv105wum_init(struct sprd_panel* panel)
{
	printk("lcd init\n");

	sprd_panel_send_cmds(panel->slave,
			     panel->info.cmds[CMD_CODE_INIT],
			     panel->info.cmds_len[CMD_CODE_INIT]);

	return 0;
}

static int tv105wum_uninit(struct sprd_panel* panel) {

	printk("lcd uninit\n");

	sprd_panel_send_cmds(panel->slave,
			     panel->info.cmds[CMD_CODE_SLEEP_IN],
			     panel->info.cmds_len[CMD_CODE_SLEEP_IN]);

    return 0;
}

static int tv105wum_readid(struct sprd_panel* panel)
{

	return 0;
}
extern int boardid;
static int power_on(struct sprd_panel* panel)
{
	int ret = 0;
	struct gpio_timing *timing;
	int items, i;

	//原来的代码时序,原封不动的抄过来的

	if ((boardid == 0) || (boardid == 1)) {
		if(!regulator_is_enabled(panel->supply)) {
			ret = regulator_enable(panel->supply);
			if (ret < 0)
				DRM_ERROR("enable lcd regulator failed\n");
		}
	} else if (boardid == 2) {
		ret = regulator_enable(panel->supply_1v8);
		if (ret < 0)
			DRM_ERROR("enable lcd regulator 1v8 failed\n");

		ret = regulator_enable(panel->supply);
		if (ret < 0)
			DRM_ERROR("enable lcd regulator failed\n");
	} else {
		if(!IS_ERR_OR_NULL(panel->supply)) {
			if(!regulator_is_enabled(panel->supply))
				ret = regulator_enable(panel->supply);
				if (ret < 0)
					DRM_ERROR("enable lcd regulator failed\n");

			}
	}

	if (panel->info.avdd_gpio) {
		gpiod_direction_output(panel->info.avdd_gpio, 1);
		mdelay(5);
	}

	if (panel->info.avee_gpio) {
		gpiod_direction_output(panel->info.avee_gpio, 1);
		mdelay(5);
	}

	if (panel->info.reset_gpio) {
		items = panel->info.rst_on_seq.items;
		timing = panel->info.rst_on_seq.timing;
		for (i = 0; i < items; i++) {
			gpiod_direction_output(panel->info.reset_gpio,
						timing[i].level);
			mdelay(timing[i].delay);
		}
	}


	return 0;
}

static int power_off(struct sprd_panel* panel)
{
	struct gpio_timing *timing;
	int items, i;

	if (panel->info.avee_gpio) {
		gpiod_direction_output(panel->info.avee_gpio, 0);
		mdelay(5);
	}

	if (panel->info.avdd_gpio) {
		gpiod_direction_output(panel->info.avdd_gpio, 0);
		mdelay(5);
	}

	if (panel->info.reset_gpio) {
		items = panel->info.rst_off_seq.items;
		timing = panel->info.rst_off_seq.timing;
		for (i = 0; i < items; i++) {
			gpiod_direction_output(panel->info.reset_gpio,
						timing[i].level);
			mdelay(timing[i].delay);
		}
	}

	if ((boardid == 0) || (boardid == 1)) {

		DRM_INFO("%s(), now shutdown\n", __func__);
		if(!IS_ERR_OR_NULL(panel->supply))
			regulator_disable(panel->supply);

	}  else if (boardid == 2) {
		if((!IS_ERR_OR_NULL(panel->supply)) && (!IS_ERR_OR_NULL(panel->supply_1v8))) {
			regulator_disable(panel->supply);
			regulator_disable(panel->supply_1v8);
		}
	} else {
		if(!IS_ERR_OR_NULL(panel->supply)) {
			if(regulator_is_enabled(panel->supply))
				regulator_disable(panel->supply);
		}
	}

	return 0;
}

static int tv105wum_power(struct sprd_panel* panel, int on)
{

	if (on) {
		power_on(panel);
	} else {
		power_off(panel);
	}

	return 0;
}

static int tv105wum_get_backlight_delay_time(void)
{
	return 150;
}

static struct panel_ops tv105wum_ops = {
	.init = tv105wum_init,
	.read_id = tv105wum_readid,
	.power = tv105wum_power,
    .uninit = tv105wum_uninit,
	.get_backlight_delay_time = tv105wum_get_backlight_delay_time,
};


struct panel_driver tv105wum_driver = {
	.lcd_name = "lcd_tv105wum_mipi_hd",
	.ops = &tv105wum_ops,
};
