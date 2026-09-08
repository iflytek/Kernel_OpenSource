/*
* Copyright (C) 2015-2016 Spreadtrum Communications Inc.
* Copyright (C) 2021 Anhui Yunshu Zhihui Technology Co., Ltd.
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
#include <loader_common.h>
#include <boot_mode.h>
#include <sprd_led.h>

#ifdef CONFIG_CHG_LED
void sprdbat_chg_led(int on)
{
	if(on)
		sprd_led_set_brightness(Red, 255);
	else {
		sprd_led_set_brightness(Red, 0);
		sprd_led_disable(Red);
	}
}
#else
void sprdbat_chg_led(int on){};
#endif

#ifdef CONFIG_SHOW_CHGLOGO
#define CHG_LOGO_TIME 15
void sprdbat_show_chglogo(bool is_chg_connect)
{
	static int lcd_on = 0;
	static int on_cnt = 0;
	static int init_flag = 0;
	extern int drv_lcd_init (void);
	extern void lcd_enable(void);
    int ret = 0;

    printf("%s show chg logo, is_chg_connected = %d\n",
            __func__, is_chg_connect);

	if (!init_flag) {
		init_flag = 1;
		lcd_on = 1;
		on_cnt = CHG_LOGO_TIME;
		drv_lcd_init();
        printf("%s fqchen init_flag = %d\n", __func__, init_flag);
		//lcd_splash(CHARGER_LOGO_PART);
		ret = lcd_splash("lowbatterylogo");
        if (ret)
            printf("splash lowbatterylogo failed!!!\n");
		lcd_enable();
		set_backlight(BACKLIGHT_ON);
		sprdbat_chg_led(1);
	}

	if (on_cnt > 0)
		on_cnt--;

	if (!power_button_pressed()) {
		printf("power key pressed show chg logo\n");
		on_cnt = CHG_LOGO_TIME;
	}

    if (!is_chg_connect)
        on_cnt = 0;

	if (on_cnt > 0) {
		if(!lcd_on) {
            drv_lcd_init();
            lcd_splash(CHARGER_LOGO_PART);
            lcd_enable();
			set_backlight(BACKLIGHT_ON);
			lcd_on = 1;
		}
	} else {
		if(lcd_on) {
            mdelay(2000);
			set_backlight(BACKLIGHT_OFF);
            drv_lcd_uninit();
			lcd_on = 0;
		}
	}
}
#else
void sprdbat_show_chglogo(void)
{
	static int init_flag = 0;

	if (!init_flag) {
		init_flag = 1;
		sprdbat_chg_led(1);
	}
}
#endif

