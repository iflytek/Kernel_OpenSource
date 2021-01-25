/*
 * Copyright (C) 2015 MediaTek Inc.
 * Copyright (C) 2021, Anhui Yunshu Zhihui Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/*
 * Author: yucong xiong <yucong.xion@mediatek.com>
 *
 */

#include <linux/delay.h>
#include <linux/hrtimer.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of_gpio.h>
#include <linux/slab.h>
#include <linux/debugfs.h>
#include <linux/types.h>
#include <linux/pm.h>


#include <linux/module.h>
#include <linux/ctype.h>
#include <linux/workqueue.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>

#include <linux/kernel.h>

#include <linux/errno.h>

#include <linux/string.h>

#include <linux/unistd.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/input.h>

#include <linux/pm.h>
#include <linux/module.h>
#include <linux/device.h>

#include <linux/of_irq.h>
#include <linux/of_platform.h>

#define CONFIG_OF 1
#define CONFIG_UCS12CM0_PM_SLEEP   0

#define APS_PR_ERR(fmt, args...) pr_err("[UCS12CM0-ERR][%s:%d] "fmt"\n", __func__, __LINE__, ##args)
#define APS_PR_INFO(fmt, args...)	pr_info("[UCS12CM0-ERR][%s:%d] "fmt"\n", __func__, __LINE__, ##args)
/*----------------------------------------------------------------------------*/
#define C_CUST_I2C_ADDR_NUM  4
#define C_CUST_ALS_LEVEL    16
struct alsps_hw {
	int i2c_num;                                    /*!< the i2c bus used by ALS/PS */
	int power_id;                                   /*!< the VDD power id of the als chip */
	int power_vol;                                  /*!< the VDD power voltage of the als chip */
	int polling_mode;                               /*!< 1: polling mode ; 0:interrupt mode*/
	//int polling_mode_ps;                               /*!< 1: polling mode ; 0:interrupt mode*/
	int polling_mode_als;                               /*!< 1: polling mode ; 0:interrupt mode*/
	unsigned char   i2c_addr[C_CUST_I2C_ADDR_NUM];/*!< i2c address list, some chip will have multiple address */
	/*!< (C_CUST_ALS_LEVEL-1) levels divides all range into C_CUST_ALS_LEVEL levels*/
	unsigned int    als_level[C_CUST_ALS_LEVEL-1];
	unsigned int    als_value[C_CUST_ALS_LEVEL];    /*!< the value reported in each level */
	//unsigned int    ps_threshold;                   /*!< the threshold of proximity sensor */
	unsigned int    als_window_loss;                /*!< the window loss  */
	//unsigned int    ps_threshold_high;
	//unsigned int    ps_threshold_low;
	unsigned int    als_threshold_high;
	unsigned int    als_threshold_low;
	int als_power_vio_id;                                   /*!< the VIO power id of the als chip */
	int als_power_vio_vol;                                  /*!< the VIO power voltage of the als chip */
	//int ps_power_vdd_id;                                   /*!< the VDD power id of the ps chip */
	//int ps_power_vdd_vol;                                  /*!< the VDD power voltage of the ps chip */
	//int ps_power_vio_id;                                   /*!< the VIO power id of the ps chip */
	//int ps_power_vio_vol;                                  /*!< the VIO power voltage of the ps chip */
	int power_lp_mode_ctrl;         /*!< 1: disable ldo low power mode when p sensor enabled ; 0: no action*/
	//bool is_batch_supported_ps;
	bool is_batch_supported_als;
};

struct UCS12CM0_priv {
	struct alsps_hw hw;
	struct i2c_client *client;
	struct work_struct eint_work;
	struct input_dev *a_idev;
	/*misc */
	u16 als_modulus;
	atomic_t i2c_retry;
	atomic_t als_suspend;
	atomic_t als_debounce;	/*debounce time after enabling als */
	atomic_t als_deb_on;	/*indicates if the debounce is on */
	atomic_t als_deb_end;	/*the jiffies representing the end of debounce */
	//atomic_t ps_mask;	/*mask ps: always return far away */
	//atomic_t ps_debounce;	/*debounce time after enabling ps */
	//atomic_t ps_deb_on;	/*indicates if the debounce is on */
	//atomic_t ps_deb_end;	/*the jiffies representing the end of debounce */
	//atomic_t ps_suspend;
	atomic_t trace;
	atomic_t init_done;
	struct device_node *irq_node;
	int irq;

	/*data */
	u16 als;//上报cct值
	u16 r_als;//上报r值
	u16 g_als;//上报g值
	u16 b_als;//上报b值
	u16 w_als;//上报b值


	u16 ps;
	u8 _align;
	u16 als_level_num;
	u16 als_value_num;
	u32 als_level[C_CUST_ALS_LEVEL - 1];
	u32 als_value[C_CUST_ALS_LEVEL];
	int ps_cali;

	atomic_t als_cmd_val;	/*the cmd value can't be read, stored in ram */
	//atomic_t ps_cmd_val;	/*the cmd value can't be read, stored in ram */
	//atomic_t ps_thd_val_high;	/*the cmd value can't be read, stored in ram */
	//atomic_t ps_thd_val_low;	/*the cmd value can't be read, stored in ram */
	atomic_t als_thd_val_high;	/*the cmd value can't be read, stored in ram */
	atomic_t als_thd_val_low;	/*the cmd value can't be read, stored in ram */
	//atomic_t ps_thd_val;
	ulong enable;		/*enable mask */
	ulong pending_intr;	/*pending interrupt */
};


#define X_A  -0.121714589
#define X_B  6.266768206
#define X_C  -4.368804016

#define Y_A  -2.360768324
#define Y_B  8.38127456
#define Y_C  -4.476796103

#define Z_A  -5.097694084
#define Z_B  -1.42651259
#define Z_C  10.32534666

/*-----------------------------CMC for debugging-------------------------------*/
enum {
	CMC_BIT_ALS = 1,
	CMC_BIT_PS = 2,
} CMC_BIT;

enum {
	CMC_TRC_ALS_DATA = 0x0001,
	CMC_TRC_PS_DATA = 0x0002,
	CMC_TRC_EINT = 0x0004,
	CMC_TRC_IOCTL = 0x0008,
	CMC_TRC_I2C = 0x0010,
	CMC_TRC_CVT_ALS = 0x0020,
	CMC_TRC_CVT_PS = 0x0040,
	CMC_TRC_CVT_AAL = 0x0080,
	CMC_TRC_DEBUG = 0x8000,
} CMC_TRC;
/*-----------------------------------------------------------------------------*/


/*UCS12CM0 als/ps sensor register related macro*/
#define UCS12CM0_REG_SYSM_CTRL		0X00
#define UCS12CM0_REG_INT_CTRL		0X01
#define UCS12CM0_REG_INT_FLAG		0X02
#define UCS12CM0_REG_WAIT		0X03
#define UCS12CM0_REG_ALS_GAIN		0X04
#define UCS12CM0_REG_ALS_TIME		0X05
#define UCS12CM0_REG_PS_LED		0X06
#define UCS12CM0_REG_PS_GAIN		0X07
#define UCS12CM0_REG_PS_PULSE		0X08
#define UCS12CM0_REG_PS_TIME		0X09
#define UCS12CM0_REG_PS_AVERAGE		0X0A
#define UCS12CM0_REG_PS_PERSIST		0X0B
#define UCS12CM0_REG_ALS_THDLL		0X0C
#define UCS12CM0_REG_ALS_THDLH		0X0D
#define UCS12CM0_REG_ALS_THDHL		0X0E
#define UCS12CM0_REG_ALS_THDHH		0X0F
#define UCS12CM0_REG_PS_THDLL		0X10
#define UCS12CM0_REG_PS_THDLH		0X11
#define UCS12CM0_REG_PS_THDHL		0X12
#define UCS12CM0_REG_PS_THDHH		0X13
#define UCS12CM0_REG_PS_OFFSET_L	0X14
#define UCS12CM0_REG_PS_OFFSET_H	0X15

#define UCS12CM0_REG_PS_DATA_L		0X18
#define UCS12CM0_REG_PS_DATA_H		0X19

#define UCS12CM0_REG_CLS_R_DATA_L	0X1C
#define UCS12CM0_REG_CLS_R_DATA_H	0X1D
#define UCS12CM0_REG_CLS_G_DATA_L	0X1E
#define UCS12CM0_REG_CLS_G_DATA_H	0X1F
#define UCS12CM0_REG_CLS_B_DATA_L	0X20
#define UCS12CM0_REG_CLS_B_DATA_H	0X21
#define UCS12CM0_REG_CLS_W_DATA_L	0X22
#define UCS12CM0_REG_CLS_W_DATA_H	0X23
#define UCS12CM0_REG_IR_DATA_L	        0X24
#define UCS12CM0_REG_IR_DATA_H	        0X25

#define UCS12CM0_REG_ID                 0XBC
/*----------------------------------------------------------------------------*/
#define I2C_FLAG_WRITE	0
#define I2C_FLAG_READ	1

/*UCS12CM0 related driver tag macro*/
#define UCS12CM0_SUCCESS						0
#define UCS12CM0_ERR_I2C						-1
#define UCS12CM0_ERR_STATUS					-3
#define UCS12CM0_ERR_SETUP_FAILURE			-4
#define UCS12CM0_ERR_GETGSENSORDATA			-5
#define UCS12CM0_ERR_IDENTIFICATION			-6

static struct UCS12CM0_priv *UCS12CM0_obj;
static struct i2c_client *UCS12CM0_i2c_client;

/*----------------------------------------------------------------------------*/
static int UCS12CM0_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int UCS12CM0_i2c_remove(struct i2c_client *client);

#if CONFIG_UCS12CM0_PM_SLEEP
static int UCS12CM0_i2c_suspend(struct device *dev);
static int UCS12CM0_i2c_resume(struct device *dev);
#endif

/*----------------------------------------------------------------------------*/


#if CONFIG_UCS12CM0_PM_SLEEP
static const struct dev_pm_ops UCS12CM0_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(UCS12CM0_i2c_suspend, UCS12CM0_i2c_resume)
};
#endif

#ifdef CONFIG_OF
static const struct of_device_id alsps_of_match[] = {
	{.compatible = "ucs12cm0,alsps"},
	{},
};
#endif
static struct i2c_device_id UCS12CM0_i2c_id[] = {
	{ "ucs12cm0,alsps", 0 },
	{}
};
MODULE_DEVICE_TABLE(i2c, UCS12CM0_i2c_id);


static struct i2c_driver UCS12CM0_i2c_driver = {
	.probe = UCS12CM0_i2c_probe,
	.remove = UCS12CM0_i2c_remove,
	.id_table = UCS12CM0_i2c_id,
	.driver = {
		.name = "UCS12CM0",
		.owner = THIS_MODULE,

#if CONFIG_UCS12CM0_PM_SLEEP
		.pm   = &UCS12CM0_pm_ops,
#endif

#ifdef CONFIG_OF
		.of_match_table = alsps_of_match,
#endif
	},
};
static DEFINE_MUTEX(UCS12CM0_mutex);
/********************************************************************/
int UCS12CM0_i2c_master_operate(struct i2c_client *client, char *buf, int count, int i2c_flag)
{
	int res = 0;
//#ifndef CONFIG_MTK_I2C_EXTENSION
	struct i2c_msg msg[2];
//#endif
	mutex_lock(&UCS12CM0_mutex);
	switch (i2c_flag) {
	case I2C_FLAG_WRITE:
/* #ifdef CONFIG_MTK_I2C_EXTENSION
		client->addr &= I2C_MASK_FLAG;
		res = i2c_master_send(client, buf, count);
		client->addr &= I2C_MASK_FLAG;
#else */
		res = i2c_master_send(client, buf, count);
//#endif
		break;

	case I2C_FLAG_READ:
/* #ifdef CONFIG_MTK_I2C_EXTENSION
		client->addr &= I2C_MASK_FLAG;
		client->addr |= I2C_WR_FLAG;
		client->addr |= I2C_RS_FLAG;
		res = i2c_master_send(client, buf, (count << 8) | 1);
		client->addr &= I2C_MASK_FLAG;
#else */
		msg[0].addr = client->addr;
		msg[0].flags = 0;
		msg[0].len = 1;
		msg[0].buf = buf;

		msg[1].addr = client->addr;
		msg[1].flags = I2C_M_RD;
		msg[1].len = count;
		msg[1].buf = buf;
		res = i2c_transfer(client->adapter, msg, ARRAY_SIZE(msg));
//#endif
		break;
	default:
		printk("UCS12CM0_i2c_master_operate i2c_flag command not support!\n");
		break;
	}
	if (res < 0)
		goto EXIT_ERR;
	mutex_unlock(&UCS12CM0_mutex);
	return res;
EXIT_ERR:
	mutex_unlock(&UCS12CM0_mutex);
	//APS_PR_ERR("UCS12CM0_i2c_master_operate fail\n");
	return res;
}

/********************************************************************/





#if CONFIG_UCS12CM0_PM_SLEEP
static int UCS12CM0_i2c_suspend(struct device *dev)
{

	return 0;
}
static int UCS12CM0_i2c_resume(struct device *dev)
{

	return 0;
}
#endif













/*----------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------*/
static int UCS12CM0_init_client(struct i2c_client *client)
{
	struct UCS12CM0_priv *obj = i2c_get_clientdata(client);
	u8 databuf[3];
	int res = 0;


	//Reg 0x00
	databuf[0] = UCS12CM0_REG_SYSM_CTRL;
	databuf[1] = 0x00;
	res = UCS12CM0_i2c_master_operate(client, databuf, 0x2, I2C_FLAG_WRITE);
	if (res <= 0) {
		APS_PR_ERR("i2c_master_send function err\n");
		goto EXIT_ERR;
	}
	printk("UCS12CM0_REG_SYSM_CTRL command!\n");


	//Reg 0x01
	databuf[0] = UCS12CM0_REG_INT_CTRL;
	databuf[1] = 0x13;//-------------------0x03;
	res = UCS12CM0_i2c_master_operate(client, databuf, 0x2, I2C_FLAG_WRITE);
	if (res <= 0) {
		APS_PR_ERR("i2c_master_send function err\n");
		goto EXIT_ERR;
	}
	printk("UCS12CM0_REG_INT_CTRL command!\n");


	//Reg 0x02
	databuf[0] = UCS12CM0_REG_INT_FLAG;
	databuf[1] = 0x00;
	res = UCS12CM0_i2c_master_operate(client, databuf, 0x2, I2C_FLAG_WRITE);
	if (res <= 0) {
		APS_PR_ERR("i2c_master_send function err\n");
		goto EXIT_ERR;
	}
	printk("UCS12CM0 ps UCS12CM0_REG_INT_FLAG command!\n");


	//Reg 0x03
	databuf[0] = UCS12CM0_REG_WAIT;
	databuf[1] = 0x00;
	res = UCS12CM0_i2c_master_operate(client, databuf, 0x2, I2C_FLAG_WRITE);
	if (res <= 0) {
		APS_PR_ERR("i2c_master_send function err\n");
		goto EXIT_ERR;
	}
	printk("UCS12CM0 ps UCS12CM0_REG_WAIT command!\n");

        //Reg 0x04
	databuf[0] = UCS12CM0_REG_ALS_GAIN;
	databuf[1] = 0x84;
	res = UCS12CM0_i2c_master_operate(client, databuf, 0x2, I2C_FLAG_WRITE);
	if (res <= 0) {
		APS_PR_ERR("i2c_master_send function err\n");
		goto EXIT_ERR;
	}
	printk("UCS12CM0 ps UCS12CM0_REG_ALS_GAIN command!\n");

        //Reg 0x05
	databuf[0] = UCS12CM0_REG_ALS_TIME;
	databuf[1] = 0x33;//---------0x3F;	//16 bit
	res = UCS12CM0_i2c_master_operate(client, databuf, 0x2, I2C_FLAG_WRITE);
	if (res <= 0) {
		APS_PR_ERR("i2c_master_send function err\n");
		goto EXIT_ERR;
	}
	printk("UCS12CM0 ps UCS12CM0_REG_ALS_TIME command!\n");

	//Reg 0x06
	databuf[0] = UCS12CM0_REG_PS_LED;
	databuf[1] = 0x00;
	res = UCS12CM0_i2c_master_operate(client, databuf, 0x2, I2C_FLAG_WRITE);
	if (res <= 0) {
		APS_PR_ERR("i2c_master_send function err\n");
		goto EXIT_ERR;
	}
	printk("UCS12CM0 UCS12CM0_REG_PS_LED command!\n");

	//Reg 0x07
	 databuf[0] = UCS12CM0_REG_PS_GAIN;
	databuf[1] = 0x00;
	res = UCS12CM0_i2c_master_operate(client, databuf, 0x2, I2C_FLAG_WRITE);
	if (res <= 0) {
		APS_PR_ERR("i2c_master_send function err\n");
		goto EXIT_ERR;
	}
	printk("UCS12CM0_REG_PS_GAIN command!\n");
	//Reg 0x08
	databuf[0] = UCS12CM0_REG_PS_PULSE;
	databuf[1] = 0x00;
	res = UCS12CM0_i2c_master_operate(client, databuf, 0x2, I2C_FLAG_WRITE);
	if (res <= 0) {
		APS_PR_ERR("i2c_master_send function err\n");
		goto EXIT_ERR;
	}
	printk("UCS12CM0_REG_PS_PULSE command!\n");

	//Reg 0x09
	databuf[0] = UCS12CM0_REG_PS_TIME;
	databuf[1] = 0x0F; //12 bit
	res = UCS12CM0_i2c_master_operate(client, databuf, 0x2, I2C_FLAG_WRITE);
	if (res <= 0) {
		APS_PR_ERR("i2c_master_send function err\n");
		goto EXIT_ERR;
	}
	printk("UCS12CM0_REG_PS_PULSE command!\n");

	//Reg 0X0A
	databuf[0] = UCS12CM0_REG_PS_AVERAGE;
	databuf[1] = 0x0F;
	res = UCS12CM0_i2c_master_operate(client, databuf, 0x2, I2C_FLAG_WRITE);
	if (res <= 0) {
		APS_PR_ERR("i2c_master_send function err\n");
		goto EXIT_ERR;
	}
	printk("UCS12CM0_REG_PS_AVERAGE command!\n");

	//Reg 0X0B   设置事件是何时上报  0x01 每个中断事件上报
	databuf[0] = UCS12CM0_REG_PS_PERSIST;
	databuf[1] = 0x00;//------------------------0x11;
	res = UCS12CM0_i2c_master_operate(client, databuf, 0x2, I2C_FLAG_WRITE);
	if (res <= 0) {
		APS_PR_ERR("i2c_master_send function err\n");
		goto EXIT_ERR;
	}
	printk("UCS12CM0_REG_PS_PERSIST command!\n");


        //Reg 0x14,15
	databuf[0] = UCS12CM0_REG_PS_OFFSET_L;
	databuf[1] = 0x00; //Reg 0x14
	databuf[2] = 0x00; //Reg 0x15
	res = UCS12CM0_i2c_master_operate(client, databuf, 0x3, I2C_FLAG_WRITE);
	if (res <= 0) {
		APS_PR_ERR("i2c_master_send function err\n");
		goto EXIT_ERR;
	}

	printk("UCS12CM0 ps UCS12CM0_REG_PS_CANC command!\n");

	//因为不在设备树那边赋值,只能在这里赋值
	obj->hw.polling_mode_als = 0;
	atomic_set(&obj->als_thd_val_high, 0xFFFF);
	atomic_set(&obj->als_thd_val_low, 0x0000);

	if (obj->hw.polling_mode_als == 0) {
                //Reg 0x0E,0x0F
		databuf[0] = UCS12CM0_REG_ALS_THDHL;
		databuf[1] = (u8) (atomic_read(&obj->als_thd_val_high) & 0xFF);
		databuf[2] = (u8) (atomic_read(&obj->als_thd_val_high) >> 8);
		res = UCS12CM0_i2c_master_operate(client, databuf, 0x3, I2C_FLAG_WRITE);
		if (res <= 0) {
			APS_PR_ERR("i2c_master_send function err\n");
			goto EXIT_ERR;
		}
                //Reg 0x0C,0x0D
		databuf[0] = UCS12CM0_REG_ALS_THDLL;
		databuf[1] = (u8) (atomic_read(&obj->als_thd_val_low) & 0xFF);
		databuf[2] = (u8) (atomic_read(&obj->als_thd_val_low) >> 8);
		res = UCS12CM0_i2c_master_operate(client, databuf, 0x3, I2C_FLAG_WRITE);
		if (res <= 0) {
			APS_PR_ERR("i2c_master_send function err\n");
			goto EXIT_ERR;
		}
	}
	/*if (obj->hw.polling_mode_ps == 0)
	{
		//0x10,0x11
		databuf[0] = UCS12CM0_REG_PS_THDLL;
		databuf[1] = (u8) (atomic_read(&obj->ps_thd_val_low) & 0xFF);
		databuf[2] = (u8) (atomic_read(&obj->ps_thd_val_low) >> 8);
		res = UCS12CM0_i2c_master_operate(client, databuf, 0x3, I2C_FLAG_WRITE);
		if (res <= 0) {
			APS_PR_ERR("i2c_master_send function err\n");
			goto EXIT_ERR;
		}
		//0x12,0x13
		databuf[0] = UCS12CM0_REG_PS_THDHL;
		databuf[1] = (u8) (atomic_read(&obj->ps_thd_val_high) & 0xFF);
		databuf[2] = (u8) (atomic_read(&obj->ps_thd_val_high) >> 8);
		res = UCS12CM0_i2c_master_operate(client, databuf, 0x3, I2C_FLAG_WRITE);
		if (res <= 0) {
			APS_PR_ERR("i2c_master_send function err\n");
			goto EXIT_ERR;
		}
	}
	*/
	//res = UCS12CM0_setup_eint(client);
	//if (res != 0) {
	//	APS_PR_ERR("setup eint: %d\n", res);
	//	return res;
	//}

	return UCS12CM0_SUCCESS;

 EXIT_ERR:
	APS_PR_ERR("init dev: %d\n", res);
	return res;
}


#ifdef CONFIG_CUSTOM_HARDWARE_CHECK
static int g_rgb_hardware_check_flag = -2;

static void rgb_ucs_file_store(int data)
{

	struct file *pfile = NULL;
	int err,nwrite = 0;
	loff_t pos = 0;
	int count = 0;

	pfile = filp_open("/mnt/vendor/productinfo/sensor_calibration_data/hw_check/rgb_hardware_check", O_WRONLY | O_TRUNC, 0666);
	if (IS_ERR(pfile)) {
		err = PTR_ERR(pfile);
		printk("%s open file error=%d\n",__func__, err);
		return;
	}

	count =1;
	if(data == 0)
		nwrite = kernel_write(pfile, "0", count,&pos);
	else
		nwrite = kernel_write(pfile, "1", count,&pos);

	if (nwrite < 0) {
		printk("%s nwrite=%d\n", __func__, nwrite);
	}

	if (pfile)
		filp_close(pfile, NULL);

}
#endif


/********************************************************************/
int UCS12CM0_enable_als(struct i2c_client *client, int enable)
{
	struct UCS12CM0_priv *obj = i2c_get_clientdata(client);
	int res = 0;
	u8 databuf[3];

	if (enable == 1) {
		printk("UCS12CM0_enable_als enable_als\n");
		databuf[0] = UCS12CM0_REG_SYSM_CTRL;
		res = UCS12CM0_i2c_master_operate(client, databuf, 1, I2C_FLAG_READ);
		if (res < 0) {
			APS_PR_ERR("i2c_master_send function err\n");
			goto ENABLE_ALS_EXIT_ERR;
		}

		printk("UCS12CM0_REG_SYSM_CTRL value value_low = %x, value_high = %x\n", databuf[0], databuf[1]);

//		databuf[2] = databuf[1];
		databuf[1] = databuf[0] | 0x01;
		databuf[0] = UCS12CM0_REG_SYSM_CTRL;

		res = UCS12CM0_i2c_master_operate(client, databuf, 0x2, I2C_FLAG_WRITE);
		if (res < 0) {
			APS_PR_ERR("i2c_master_send function err\n");
			goto ENABLE_ALS_EXIT_ERR;
		}
		atomic_set(&obj->als_deb_on, 1);
		atomic_set(&obj->als_deb_end, jiffies + atomic_read(&obj->als_debounce) / (1000 / HZ));
	} else {
		printk("UCS12CM0_enable_als disable_als\n");
		databuf[0] = UCS12CM0_REG_SYSM_CTRL;
		res = UCS12CM0_i2c_master_operate(client, databuf, 1, I2C_FLAG_READ);
		if (res < 0) {
			APS_PR_ERR("i2c_master_send function err\n");
			goto ENABLE_ALS_EXIT_ERR;
		}

		printk("UCS12CM0_REG_SYSM_CTRL value value_low = %x, value_high = %x\n", databuf[0], databuf[1]);

//		databuf[2] = databuf[1];
		databuf[1] = databuf[0] & 0xFE;
		databuf[0] = UCS12CM0_REG_SYSM_CTRL;

		res = UCS12CM0_i2c_master_operate(client, databuf, 0x2, I2C_FLAG_WRITE);
		if (res < 0) {
			APS_PR_ERR("i2c_master_send function err\n");
			goto ENABLE_ALS_EXIT_ERR;
		}
		atomic_set(&obj->als_deb_on, 0);
	}

#ifdef CONFIG_CUSTOM_HARDWARE_CHECK

	if(g_rgb_hardware_check_flag!= 0)
		rgb_ucs_file_store(0);
	g_rgb_hardware_check_flag = 0;

#endif

	return 0;
ENABLE_ALS_EXIT_ERR:

#ifdef CONFIG_CUSTOM_HARDWARE_CHECK

	if(g_rgb_hardware_check_flag!= -1)
		rgb_ucs_file_store(1);
	g_rgb_hardware_check_flag = -1;

#endif

	return res;
}

/********************************************************************/

/********************************************************************/

static int als_enable_nodata(int en)
{
	int res = 0;

	printk("UCS12CM0_obj als enable value = %d\n", en);

	mutex_lock(&UCS12CM0_mutex);
	if (en)
		set_bit(CMC_BIT_ALS, &UCS12CM0_obj->enable);
	else
		clear_bit(CMC_BIT_ALS, &UCS12CM0_obj->enable);
	mutex_unlock(&UCS12CM0_mutex);
	if (!UCS12CM0_obj) {
		APS_PR_ERR("UCS12CM0_obj is null!!\n");
		return -1;
	}
	res = UCS12CM0_enable_als(UCS12CM0_obj->client, en);
	if (res) {
		APS_PR_ERR("als_enable_nodata is failed!!\n");
		return -1;
	}
	return 0;
}
/********************************************************************/
long UCS12CM0_read_als(struct i2c_client *client, u16 *data)
{

    long res = 0;

    u8 databuf[2];
    volatile u16 red_data;
    volatile u16 green_data;
    volatile u16 blue_data;
    volatile u16 white_data;
    volatile u16 ir_data;
    //volatile u16 cyan_data;
    //volatile u16 mata_data;
    //volatile u16 yeow_data;
    //volatile u32 new_r;
    //volatile u32 new_g;
    //volatile u32 new_b;

#if 0
	volatile float xmax_data;
	volatile float ymax_data;
	volatile float zmax_data;
	volatile float xmin_data;
	volatile float ymin_data;
	volatile float nmin_data;
	volatile float sens_ccts;
#endif

	//struct UCS12CM0_priv *obj = i2c_get_clientdata(client);

	databuf[0] = UCS12CM0_REG_CLS_R_DATA_L;
	res = UCS12CM0_i2c_master_operate(client, databuf, 2, I2C_FLAG_READ);
	if (res < 0) {
		APS_PR_ERR("i2c_master_send function err\n");
		goto READ_ALS_EXIT_ERR;
	}

        red_data = ((databuf[1] << 8) | databuf[0]);

	databuf[0] = UCS12CM0_REG_CLS_G_DATA_L;
	res = UCS12CM0_i2c_master_operate(client, databuf, 2, I2C_FLAG_READ);
	if (res < 0) {
		pr_err("i2c_master_send function err\n");
		goto READ_ALS_EXIT_ERR;
	}
	green_data = ((databuf[1] << 8) | databuf[0]);

	databuf[0] = UCS12CM0_REG_CLS_B_DATA_L;
	res = UCS12CM0_i2c_master_operate(client, databuf, 2, I2C_FLAG_READ);
	if (res < 0) {
		pr_err("i2c_master_send function err\n");
		goto READ_ALS_EXIT_ERR;
	}
	blue_data = ((databuf[1] << 8) | databuf[0]);

	databuf[0] = UCS12CM0_REG_CLS_W_DATA_L;
	res = UCS12CM0_i2c_master_operate(client, databuf, 2, I2C_FLAG_READ);
	if (res < 0) {
		pr_err("i2c_master_send function err\n");
		goto READ_ALS_EXIT_ERR;
	}
	white_data = ((databuf[1] << 8) | databuf[0]);

	databuf[0] = UCS12CM0_REG_IR_DATA_L;
	res = UCS12CM0_i2c_master_operate(client, databuf, 2, I2C_FLAG_READ);
	if (res < 0) {
		pr_err("i2c_master_send function err\n");
		goto READ_ALS_EXIT_ERR;
	}
	ir_data = ((databuf[1] << 8) | databuf[0]);

        //cyan_data = white_data - red_data;
        //mata_data = white_data - green_data;
        //yeow_data = white_data - blue_data;

        //new_r = mata_data + yeow_data;
        //new_g = cyan_data + yeow_data;
       // new_b = cyan_data + mata_data;

		UCS12CM0_obj->r_als=red_data;
		UCS12CM0_obj->g_als=green_data;
		UCS12CM0_obj->b_als=blue_data;
		UCS12CM0_obj->w_als=white_data;
		APS_PR_INFO("UCS12CM0_read_als value: red_data:%d g:%d b:%d  w:%d \n", red_data,green_data,blue_data,white_data);

		//printk("UCS12CM0_read_als value: new_r:%d g:%d b:%d  w:%d \n", new_r,new_g,new_b,white_data);



#if 0
        xmax_data = new_r * X_A + new_g * X_B + new_b * X_C;
        ymax_data = new_r * Y_A + new_g * Y_B + new_b * Y_C;
        zmax_data = new_r * Z_A + new_g * Z_B + new_b * Z_C;
        xmin_data = xmax_data / (xmax_data + ymax_data + zmax_data);
        ymin_data = ymax_data / (xmax_data + ymax_data + zmax_data);
        nmin_data = (xmin_data - 0.332)/(ymin_data - 0.1858);

        //sens_ccts = -449* pow( nmin_data,3)+3525*pow( nmin_data,2)-6823.3*nmin_data+5520.33;
		sens_ccts = -449* ( nmin_data*nmin_data*nmin_data)+3525*( nmin_data*nmin_data)-6823.3*nmin_data+5520.33;




#endif

	//if (atomic_read(&obj->trace) & CMC_TRC_DEBUG)

		//printk("UCS12CM0_REG_R_DATA value: %d\n", red_data);
		//printk("UCS12CM0_REG_G_DATA value: %d\n", green_data);
		//printk("UCS12CM0_REG_B_DATA value: %d\n", blue_data);
		//printk("UCS12CM0_REG_W_DATA value: %d\n", white_data);

//		APS_LOG("UCS12CM0_REG_C_DATA value: %d\n", cyan_data);
//		APS_LOG("UCS12CM0_REG_M_DATA value: %d\n", mata_data);
//		APS_LOG("UCS12CM0_REG_Y_DATA value: %d\n", yeow_data);
//		APS_LOG("UCS12CM0_REG_New_R_DATA value: %d\n", new_r);
//		APS_LOG("UCS12CM0_REG_New_G_DATA value: %d\n", new_g);
//		APS_LOG("UCS12CM0_REG_New_B_DATA value: %d\n", new_b);

//	*data = ymax_data; // Lux
	*data = green_data;

	return 0;
READ_ALS_EXIT_ERR:
	return res;
}

/********************************************************************/
/*---------------------------------------------------------------------------------*/
static ssize_t UCS12CM0_als_show(struct device *dev,
	struct device_attribute *attr, char *buf)
//static ssize_t UCS12CM0_als_show(struct device_driver *ddri, char *buf)
{

	int res = 0;

	if (!UCS12CM0_obj) {
		APS_PR_ERR("UCS12CM0_obj is null!!\n");
		return 0;
	}
	res = UCS12CM0_read_als(UCS12CM0_obj->client, &UCS12CM0_obj->als);
	if (res)
		return snprintf(buf, PAGE_SIZE, "ERROR: %d\n", res);
	else
		return snprintf(buf, PAGE_SIZE, "0x%04X\n", UCS12CM0_obj->als);

	return 0;
}
/*----------------------------------------------------------------------------*/

static ssize_t UCS12CM0_device_enable_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int res = 0;

	if (!UCS12CM0_obj) {
		APS_PR_ERR("UCS12CM0_obj is null!!\n");
		return 0;
	}
	if ( UCS12CM0_obj->enable !=0)
		res=1;
	else
		res=0;

	return snprintf(buf, PAGE_SIZE, "%d\n", res);
}

static ssize_t UCS12CM0_device_enable_store(struct device *dev,
		struct device_attribute *attr,const char *buf, size_t size)
{
	int handle, enabled;

	if (!UCS12CM0_obj) {
		APS_PR_ERR("UCS12CM0_obj is null!!\n");
		return 0;
	}

	if (sscanf(buf, "%d %d\n", &handle, &enabled) != 2)
		return -EINVAL;

	als_enable_nodata(enabled);

	return 1;
}

static ssize_t UCS12CM0_enable_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
 	int res = 0;

 	if (!UCS12CM0_obj) {
 		APS_PR_ERR("UCS12CM0_obj is null!!\n");
 		return 0;
 	}
 	if ( UCS12CM0_obj->enable !=0)
 		res=1;
 	else
 		res=0;

 	return snprintf(buf, PAGE_SIZE, "%d\n", res);

}

static ssize_t UCS12CM0_enable_store(struct device *dev,
		struct device_attribute *attr,const char *buf, size_t size)
{
	bool value;

	if (!UCS12CM0_obj) {
		APS_PR_ERR("UCS12CM0_obj is null!!\n");
		return 0;
	}


	if (strtobool(buf, &value))
		return -EINVAL;

	if (value)
		als_enable_nodata(1);
	else
		als_enable_nodata(0);

	return size;

}

static ssize_t UCS12CM0_show_status(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	ssize_t len = 0;

	if (!UCS12CM0_obj) {
		APS_PR_ERR("UCS12CM0_obj is null!!\n");
		return 0;
	}


	len +=snprintf(buf + len, PAGE_SIZE - len, "MISC: als_suspend=%d\n", atomic_read(&UCS12CM0_obj->als_suspend)
			 );

	return len;
}

static ssize_t UCS12CM0_show_reg(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	u8 _bIndex = 0;
	u8 databuf[2] = { 0 };
	ssize_t _tLength = 0;
	int res = 0;

	if (!UCS12CM0_obj) {
		APS_PR_ERR("UCS12CM0_obj is null!!\n");
		return 0;
	}

	for (_bIndex = 0; _bIndex <= 0x1F; _bIndex++) {
		databuf[0] = _bIndex;
		res = UCS12CM0_i2c_master_operate(UCS12CM0_obj->client, databuf, 1, I2C_FLAG_READ);
		if (res < 0)
			APS_PR_ERR("UCS12CM0_i2c_master_operate err res = %d\n", res);

		_tLength +=
			snprintf((buf + _tLength), (PAGE_SIZE - _tLength), "Reg[0x%02X]: 0x%02X\n", _bIndex,
				 databuf[0] );
	}

	return _tLength;

}
static ssize_t UCS12CM0_store_reg(struct device *dev,
		struct device_attribute *attr,const char *buf, size_t size)
{
    u8 databuf[2] ={0,0};
    int ret = 0;
    int err = 0;

 if (!UCS12CM0_obj) {
  APS_PR_ERR("UCS12CM0_obj is null!!\n");
  return -1;
 }
 ret = sscanf(buf, "%hhx %hhx", &databuf[0],&databuf[1] );
 if (ret == 2) {
  err = UCS12CM0_i2c_master_operate(UCS12CM0_obj->client, databuf, 2, I2C_FLAG_WRITE);
 }
 if (err <= 0)
  APS_PR_ERR("UCS12CM0 set reg fail\n");
 return size;
}

static ssize_t ucs12cm0_device_batch_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return 1;
}
static ssize_t ucs12cm0_device_batch_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t size)
{

	return 1;

}

static ssize_t UCS12CM0_calibration_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return 1;
}


static ssize_t UCS12CM0_calibration_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t size)
{

	bool value;
	static int cct=0;
	if (!UCS12CM0_obj) {
		APS_PR_ERR("UCS12CM0_obj is null!!\n");
		return 0;
	}


	if (strtobool(buf, &value))
		return -EINVAL;

	if (value){
			if(cct==0)
			{
				cct=1;
				input_report_abs(UCS12CM0_obj->a_idev, ABS_GAS, cct);//lux
				printk("%s: UCS12CM0_calibration_store calledLINE=%d \n", __func__,__LINE__);

			}else{
				cct=0;
				input_report_abs(UCS12CM0_obj->a_idev, ABS_GAS, cct);//lux
				printk("%s: UCS12CM0_calibration_store calledLINE=%d \n", __func__,__LINE__);
			}

	}


	input_sync(UCS12CM0_obj->a_idev);
	return size;

}

static DEVICE_ATTR(als, 0664, UCS12CM0_als_show, NULL);

static DEVICE_ATTR(enable, 0664, UCS12CM0_device_enable_show, UCS12CM0_device_enable_store);
static DEVICE_ATTR(batch, 0664, ucs12cm0_device_batch_show, ucs12cm0_device_batch_store);

static DEVICE_ATTR(UCS12CM0_enable, 0664, UCS12CM0_enable_show, UCS12CM0_enable_store);

static DEVICE_ATTR(calibration, 0664, UCS12CM0_calibration_show, UCS12CM0_calibration_store);

//static DRIVER_ATTR(ps, S_IWUSR | S_IRUGO, UCS12CM0_show_ps, NULL);
//static DEVICE_ATTR(config, 0664, UCS12CM0_show_config, UCS12CM0_store_config);
//static DEVICE_ATTR(alslv, 0664, UCS12CM0_show_alslv, UCS12CM0_store_alslv);

//static DEVICE_ATTR(alsval, 0664, UCS12CM0_show_alsval, UCS12CM0_store_alsval);
//static DEVICE_ATTR(trace, 0664, UCS12CM0_show_trace, UCS12CM0_store_trace);

static DEVICE_ATTR(status, 0664, UCS12CM0_show_status, NULL);

//static DEVICE_ATTR(send, 0664, UCS12CM0_show_send, UCS12CM0_store_send);
//static DEVICE_ATTR(recv, 0664, UCS12CM0_show_recv, UCS12CM0_store_recv);
//static DRIVER_ATTR(reg, S_IWUSR | S_IRUGO, UCS12CM0_show_reg, NULL);


static DEVICE_ATTR(reg, 0664, UCS12CM0_show_reg, UCS12CM0_store_reg);


static struct attribute *ucs12cm0_attributes[] = {
		&dev_attr_als.attr,
		&dev_attr_enable.attr,
		&dev_attr_batch.attr,
		&dev_attr_UCS12CM0_enable.attr,
		&dev_attr_status.attr,
		&dev_attr_reg.attr,
		&dev_attr_calibration.attr,
		/*
		&dev_attr_trace.attr,
		&dev_attr_config.attr,
		&dev_attr_alslv.attr,
		&dev_attr_alsval.attr,
		&dev_attr_status.attr,
		//&dev_attr_send.attr,
		//&dev_attr_recv.attr,
		&dev_attr_reg.attr,
		*/
		NULL
};
static const struct attribute_group ucs12cm0_attr_group = {
	.attrs = ucs12cm0_attributes,
};

static int UCS12CM0_create_attr(struct i2c_client *client)
{	int err = 0;

	err=sysfs_create_group(&client->dev.kobj, &ucs12cm0_attr_group);
	printk("%s: calledLINE=%d \n", __func__,__LINE__);
	return err;
}
/*----------------------------------------------------------------------------*/
int get_alsps_dts_func(struct device_node *node, struct alsps_hw *hw)
{

	return 0;
}


int int_FLAG_zero(void)
{
    u8 databuf[2] ={0,0};
    int ret = 0;
    //int err = 0;

	databuf[1] = 0x00;
	databuf[0] = UCS12CM0_REG_INT_FLAG;

	ret = UCS12CM0_i2c_master_operate(UCS12CM0_obj->client, databuf, 0x2, I2C_FLAG_WRITE);
	if (ret < 0) {
		APS_PR_ERR("i2c_master_send function err\n");
		return -1;
	}
	//printk("%s: calledLINE=%d \n", __func__,__LINE__);
	return 0;
}



/*-----------------------------------i2c operations----------------------------------*/
static irqreturn_t ucs12cm0_irq(int irq, void *handle)
{
	int ret = 0;
	static u16 cct=0;

	if (UCS12CM0_obj->a_idev) {//如果有此设备的话
			//int lux = UCS12CM0_obj->als_inf.lux;
			//input_report_abs(chip->a_idev, ABS_MISC, lux);
			UCS12CM0_read_als(UCS12CM0_obj->client, &UCS12CM0_obj->als);

			input_report_abs(UCS12CM0_obj->a_idev, ABS_X, UCS12CM0_obj->r_als);//r
			input_report_abs(UCS12CM0_obj->a_idev, ABS_Y, UCS12CM0_obj->g_als);//g
			input_report_abs(UCS12CM0_obj->a_idev, ABS_Z, UCS12CM0_obj->b_als);//b
			input_report_abs(UCS12CM0_obj->a_idev, ABS_THROTTLE,UCS12CM0_obj->w_als);//w

		/*if(UCS12CM0_obj->b_als !=cct){

			cct = UCS12CM0_obj->b_als;
			input_report_abs(UCS12CM0_obj->a_idev, ABS_MISC, UCS12CM0_obj->b_als);//lux
		}*/

			if(cct==0)
			{
				cct=1;
				input_report_abs(UCS12CM0_obj->a_idev, ABS_MISC, cct);//lux
				input_report_abs(UCS12CM0_obj->a_idev, ABS_DISTANCE, cct);//cct

			}else
			{
				cct=0;
				input_report_abs(UCS12CM0_obj->a_idev, ABS_MISC, cct);//lux
				input_report_abs(UCS12CM0_obj->a_idev, ABS_DISTANCE, cct);//cct
			}

			//input_report_abs(UCS12CM0_obj->a_idev, ABS_PRESSURE, UCS12CM0_obj->b_als);//ir1
			//input_report_abs(UCS12CM0_obj->a_idev, ABS_RUDDER,UCS12CM0_obj->b_als);//ir2


			APS_PR_INFO("%s: calledLINE=%d \n", __func__,__LINE__);
			input_sync(UCS12CM0_obj->a_idev);

	}

   	ret=int_FLAG_zero();
	if (ret < 0) {
		APS_PR_ERR("int_FLAG_zero function err\n");
		return -1;
	}


return 0;
}


static int UCS12CM0_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	 struct UCS12CM0_priv *obj = NULL;
	 struct device *dev = &client->dev;
	 int err = 0;
	 int ret =0;
	 unsigned int irq_gpio;

	if (!client || !client->dev.of_node) { //判断是否有此设备树的结点
		pr_err("UCS12CM0_i2c_probe invalid input\n");
		return -EINVAL;
	}
	printk("%s: calledLINE=%d \n", __func__,__LINE__);



    if(!i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_BYTE_DATA)) {  //检查 adapter 是否支持某个i2c协议的传输
        dev_err(&client->dev, "wbw Failed check I2C functionality");
        return -ENODEV;
    }
	printk("%s: calledLINE=%d \n", __func__,__LINE__);


	/* get customization and power on */  //创建本地空间
	obj = kzalloc(sizeof(*obj), GFP_KERNEL);
	if (!obj) {
		err = -ENOMEM;
		printk("%s: calledLINE=%d \n", __func__,__LINE__);
		goto exit;
	}
	printk("%s: calledLINE=%d \n", __func__,__LINE__);
	obj->client =client;
	i2c_set_clientdata(client, obj);//保存对象的值
	UCS12CM0_obj = obj;

	printk("%s: calledLINE=%d \n", __func__,__LINE__);


	err = get_alsps_dts_func(client->dev.of_node, &obj->hw);//给obj本地对象的hw结构体赋值
	if (err < 0) {
		APS_PR_ERR("get customization info from dts failed\n");
		goto exit_init_failed;
	}

	//INIT_WORK(&obj->eint_work, UCS12CM0_eint_work);//初始化工作队列


	/*-----------------------------value need to be confirmed-----------------------------------------*/
	atomic_set(&obj->als_debounce, 200);
	atomic_set(&obj->als_deb_on, 0);
	atomic_set(&obj->als_deb_end, 0);
	atomic_set(&obj->als_suspend, 0);
	obj->enable = 0;


	UCS12CM0_i2c_client = client;
	err = UCS12CM0_init_client(client);
	if (err)
		goto exit_init_failed;
	//APS_LOG("UCS12CM0_init_client() OK!\n");





	//下面是创建Input设备
		UCS12CM0_obj->a_idev = input_allocate_device();//创建input设备
		if (!UCS12CM0_obj->a_idev) {
		//	dev_err(dev, "%s: no memory for input_dev '%s'\n",
		//		__func__, pdata->als_name);
			ret = -ENODEV;
			goto input_a_alloc_failed;
		}
		//printk("wbw %s: calledLINE=%d \n", __func__,__LINE__);
		UCS12CM0_obj->a_idev->name = "ucs12cm0";
		UCS12CM0_obj->a_idev->id.bustype = BUS_I2C;
		set_bit(EV_ABS, UCS12CM0_obj->a_idev->evbit);//绝对坐标事件

		set_bit(ABS_X, UCS12CM0_obj->a_idev->absbit);//杂项事件
		set_bit(ABS_Y, UCS12CM0_obj->a_idev->absbit);//杂项事件
		set_bit(ABS_Z, UCS12CM0_obj->a_idev->absbit);//杂项事件

		set_bit(ABS_MISC, UCS12CM0_obj->a_idev->absbit);//杂项事件
		set_bit(ABS_DISTANCE, UCS12CM0_obj->a_idev->absbit);//杂项事件
		set_bit(ABS_THROTTLE, UCS12CM0_obj->a_idev->absbit);//杂项事件
		set_bit(ABS_GAS, UCS12CM0_obj->a_idev->absbit);//杂项事件

		//set_bit(LED_MISC,
		input_set_abs_params(UCS12CM0_obj->a_idev, ABS_MISC, 0, 65535, 0, 0);//通常在probe函数中会调用 input_set_abs_params 给设备的input_dev结构体初始化	//参数 chip->a_idev 设备ABS_MISC 范围是0到 +65535，数据误差是0到0，中心平滑位置是0
		input_set_abs_params(UCS12CM0_obj->a_idev, ABS_DISTANCE, 0, 65535, 0, 0);
		input_set_abs_params(UCS12CM0_obj->a_idev, ABS_X, 0, 65535, 0, 0);
		input_set_abs_params(UCS12CM0_obj->a_idev, ABS_Y, 0, 65535, 0, 0);
		input_set_abs_params(UCS12CM0_obj->a_idev, ABS_Z, 0, 65535, 0, 0);
		input_set_abs_params(UCS12CM0_obj->a_idev, ABS_RUDDER, 0, 65535, 0, 0);
		input_set_abs_params(UCS12CM0_obj->a_idev, ABS_PRESSURE, 0, 65535, 0, 0);
		input_set_abs_params(UCS12CM0_obj->a_idev, ABS_THROTTLE, 0, 65535, 0, 0);
		input_set_abs_params(UCS12CM0_obj->a_idev, ABS_GAS, 0, 1, 0, 0);
		dev_set_drvdata(&UCS12CM0_obj->a_idev->dev, UCS12CM0_obj);
		ret = input_register_device(UCS12CM0_obj->a_idev); //创建Input设备
		if (ret) {
				input_free_device(UCS12CM0_obj->a_idev);
		//		dev_err(dev, "%s: cant register input '%s'\n",
		//			__func__, pdata->als_name);
				goto input_a_alloc_failed;
			}
		//end









	//获得gpio以及irq并申请中断
		irq_gpio = of_get_named_gpio(client->dev.of_node, "ucsintr-gpio", 0);
		if (irq_gpio < 0) {
			dev_err(dev,"invalid irq-gpio in dt: %d",irq_gpio);
			goto exit_init_failed;
		}
		 printk("of_get_named_gpio=%d \n",irq_gpio);

		 client->irq = gpio_to_irq(irq_gpio);
		  UCS12CM0_obj->irq=client->irq;
		 printk("of_get_named_gpio=%d \n",client->irq);

		printk("%s: calledLINE=%d \n", __func__,__LINE__);

		ret = request_threaded_irq(client->irq, NULL, &ucs12cm0_irq,
					   IRQF_TRIGGER_FALLING | IRQF_SHARED |IRQF_ONESHOT,
					   dev_name(dev), obj);
		if (ret) {
			dev_err(dev, "Failed to request irq %d\n", client->irq);
			goto exit_init_failed;
		}
	//end



	/*------------------------UCS12CM0 attribute file for debug--------------------------------------*/
	err = UCS12CM0_create_attr(client);
	if (err) {
		APS_PR_ERR("create attribute err = %d\n", err);
		goto exit_create_attr_failed;
	}
	/*------------------------UCS12CM0 attribute file for debug--------------------------------------*/

	printk("%s: calledLINE=%d \n", __func__,__LINE__);

	return 0;


exit_create_attr_failed:
	input_unregister_device(UCS12CM0_obj->a_idev);
exit_init_failed:
input_a_alloc_failed:
	kfree(obj);
exit:
	obj = NULL;
	UCS12CM0_obj = NULL;
	UCS12CM0_i2c_client = NULL;
	APS_PR_ERR("err %s: calledLINE=%d \n", __func__,__LINE__);
	return err;
}
/*----------------------------------------------------------------------------*/
static int UCS12CM0_i2c_remove(struct i2c_client *client)
{

	return 0;
}


/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
static int __init UCS12CM0_init(void)
{
	if (i2c_add_driver(&UCS12CM0_i2c_driver)) {
		printk("add driver error\n");
		return -1;
	}
	printk("UCS12CM0  i2c_add_driver  successfully\n");
	return 0;
}

/*----------------------------------------------------------------------------*/
static void __exit UCS12CM0_exit(void)
{
	i2c_del_driver(&UCS12CM0_i2c_driver);
	printk("%s: calledLINE=%d \n", __func__,__LINE__);
}

/*----------------------------------------------------------------------------*/
module_init(UCS12CM0_init);
module_exit(UCS12CM0_exit);
/*----------------------------------------------------------------------------*/
MODULE_AUTHOR("Chen");
MODULE_DESCRIPTION("UCS12CM0 driver");
MODULE_LICENSE("GPL");
