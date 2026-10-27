/*
 * SPDX-License-Identifier: GPL-2.0-only
 * Copyright (C) 2023, Anhui Yunshu Zhihui Technology Co., Ltd.
 */
extern void lcd_printf(const char *fmt, ...);

void __attribute__((weak)) fastboot_lcd_printf(void)
{
    lcd_printf("   fastboot mode");
    return;
}
