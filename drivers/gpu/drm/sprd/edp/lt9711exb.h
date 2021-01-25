// Copyright (C) 2021, Anhui Yunshu Zhihui Technology Co., Ltd.
// SPDX-License-Identifier: GPL-2.0-only
#ifndef __LT9711_H__
#define __LT9711_H__


#define EDID_SEG_SIZE 256
#define READ_BUF_MAX_SIZE 64
#define WRITE_BUF_MAX_SIZE 64

#define LT9711_YEAR_HIGH 0x20
#define LT9711_YEAR_LOW 0x21
#define LT9711_MONTH 0x10
#define LT9711_DAY 0x14
#define LT9711_VERSION_HIGH 0x01
#define LT9711_VERSION_LOW 0x01

struct lt9711_reg_cfg {
	u8 reg;
	u8 val;
};
enum lt9711_fw_upgrade_status {
	UPDATE_SUCCESS = 0,
	UPDATE_RUNNING = 1,
	UPDATE_FAILED = 2,
};
struct lt9711 {
	struct device *dev;
	//struct drm_bridge bridge;

	//struct device_node *host_node;
	//struct mipi_dsi_device *dsi;
	//struct drm_connector connector;

	//u8 i2c_addr;
	//int irq;
	//bool ac_mode;

	//u32 irq_gpio;
	//u32 reset_gpio;
	//u32 hdmi_ps_gpio;
	//u32 hdmi_en_gpio;

	//unsigned int num_vreg;
	//struct lt9711_vreg *vreg_config;

	struct i2c_client *i2c_client;  //需要

	//enum drm_connector_status status;
	//bool power_on;

	/* get display modes from device tree */
	//bool non_pluggable;
	//u32 num_of_modes;
	//struct list_head mode_list;

	//struct drm_display_mode curr_mode;
	//struct lt9711_video_cfg video_cfg;

	struct workqueue_struct *wq;
	struct work_struct work;

	//u8 edid_buf[EDID_SEG_SIZE];
	u8 i2c_wbuf[WRITE_BUF_MAX_SIZE];  //需要   i2c写缓冲区
	u8 i2c_rbuf[READ_BUF_MAX_SIZE]; //需要   i2c读缓冲区
	//bool hdmi_mode;
	//bool hpd_support;
	enum lt9711_fw_upgrade_status fw_status;
};

#endif