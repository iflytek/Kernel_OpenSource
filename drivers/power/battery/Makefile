#
# Copyright (C) 2012 Samsung Electronics
# Lukasz Majewski <l.majewski@samsung.com>
#
# SPDX-License-Identifier:	GPL-2.0+
#

obj-$(CONFIG_POWER_BATTERY_TRATS) += bat_trats.o
obj-$(CONFIG_POWER_BATTERY_TRATS2) += bat_trats2.o
ifdef CONFIG_ADIE_SC2731
obj-y += sprd_chg_2731.o
else
ifdef CONFIG_ADIE_SC2721
obj-y += sprd_chg_2721.o
else
obj-y += sprd_chg.o
endif
endif
obj-$(CONFIG_FAN54015_CHARGE_IC)+= fan54015.o
obj-$(CONFIG_SPRD2701_CHARGE_IC)+= sprd_2701.o
obj-$(CONFIG_BQ25896_CHARGE_IC)+= bq25896.o
obj-$(CONFIG_SPRD2700_CHARGE_IC)+= sprd_2700.o
obj-y += sprd_chg_logo.o sprd_chg_helper.o sprd_battery.o sprd_fgu.o

