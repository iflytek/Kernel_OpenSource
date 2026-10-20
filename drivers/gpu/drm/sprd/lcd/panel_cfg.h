/*
 * Copyright (C) 2017 Spreadtrum Communications Inc.
 * Copyright (C) 2026 Anhui Yunshu Zhihui Technology Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "../sprd_panel.h"

extern struct panel_driver cblite_5th_option_driver;
extern struct panel_driver tv105_2thwum_driver;
extern struct panel_driver tv105wum_driver;
extern struct panel_driver dummy_mipi_driver;
extern struct panel_driver tv101wum_aw2_option_driver;
extern struct panel_driver lcd_tv105wum_aw3_driver;
extern struct panel_driver hx83102_boe_option_driver;
extern struct panel_driver hx83102_md_option_driver;
extern struct panel_driver hx83102_1197_boe_option_driver;
extern struct panel_driver hx83102_1197_jt_option_driver;

static struct panel_cfg supported_panel[] = {
#ifdef CONFIG_LCD_HX83102_BOE_MIPI_HD
	{
		.lcd_id = 0x8310201,
		.drv = &hx83102_boe_option_driver,
	},
#endif
#ifdef CONFIG_LCD_HX83102_MD_MIPI_HD
	{
		.lcd_id = 0x8310202,
		.drv = &hx83102_md_option_driver,
	},
#endif
#ifdef CONFIG_LCD_TV101WUM_AW3_MIPI_HD
	{
		.drv = &lcd_tv105wum_aw3_driver,
	},
#endif
#ifdef CONFIG_LCD_CBLITE_5TH_OPTION_MIPI_HD
	{
		.drv = &cblite_5th_option_driver,
	},
#endif
#ifdef CONFIG_TV105_2THWUM_DRIVER
	{
		.drv = &tv105_2thwum_driver,
	},
#endif
#ifdef CONFIG_LCD_TV105WUM_MIPI_HD
	{
		.drv = &tv105wum_driver,
	},
#endif
#ifdef CONFIG_LCD_TV101WUM_AW2_MIPI_HD
	{
		.drv = &tv101wum_aw2_option_driver,
	},
#endif
#ifdef CONFIG_LCD_HX83102_1197_BOE_MIPI_HD
	{
		.drv = &hx83102_1197_boe_option_driver,
	},
#endif
#ifdef CONFIG_LCD_HX83102_1197_JT_MIPI_HD
	{
		.drv = &hx83102_1197_jt_option_driver,
	},
#endif
	{
		.lcd_id = 0xFFFF,
		.drv = &dummy_mipi_driver,
	},
};
