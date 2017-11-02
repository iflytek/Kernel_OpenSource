/*
 * Copyright (C) 2017 Spreadtrum Communications Inc.
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

#include "sprd_panel.h"

extern struct panel_driver dummy_mipi_driver;

static struct panel_cfg supported_panel[] = {
/* warning: the dummy lcd must be the last item in this array */
	{
		.lcd_id = 0xFFFF,
		.drv = &dummy_mipi_driver,
	},
};
