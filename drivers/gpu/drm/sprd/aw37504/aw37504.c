
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2020, The Linux Foundation. All rights reserved.
 */

#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/firmware.h>
#include <linux/string.h>
#include "aw37504.h"



struct kobject *aw37504_node_obj = NULL;
struct aw37504 *aw37504_pdata_global = NULL;

#ifdef CONFIG_BESTSERVICE_HARDWARE_CHECK
static int g_lcd_hardware_check_flag = -1;
#endif

/*
 * Write one reg with one value;  发送一个字节的数据
 * Reg -> value
 */
static int aw37504_write_byte(struct aw37504 *pdata, const u8 reg, u8 value)
{
	struct i2c_client *client = pdata->i2c_client;
	struct i2c_msg msg = {
		.addr = client->addr,
		.flags = 0,
		.len = 2,
		.buf = pdata->i2c_wbuf,
	};

	memset(pdata->i2c_wbuf, 0,  sizeof(pdata->i2c_wbuf));
	//bzero(pdata->i2c_wbuf,sizeof(pdata->i2c_wbuf));
	//printk("aw37504_write_byte  3 success\n");

	pdata->i2c_wbuf[0] = reg;
	pdata->i2c_wbuf[1] = value;
	//printk("aw37504_write_byte  3 success\n");
	if (i2c_transfer(client->adapter, &msg, 1) < 1) {
		pr_err("i2c write failed\n");
		return -EIO;
	}
	//printk("aw37504_write_byte  5 success\n");
	return 0;
}


static int aw37504_read(struct aw37504 *pdata, u8 reg, char *buf, u32 size)
{
	struct i2c_client *client = pdata->i2c_client;
	struct i2c_msg msg[2] = {
		{
			.addr = client->addr,
			.flags = 0,//写
			.len = 1,
			.buf = pdata->i2c_wbuf,
		},
		{
			.addr = client->addr,
			.flags = I2C_M_RD,//读
			.len = size,
			.buf = pdata->i2c_rbuf,
		}
	};

	memset(pdata->i2c_wbuf, 0x0, WRITE_BUF_MAX_SIZE);//清空
	memset(pdata->i2c_rbuf, 0x0, READ_BUF_MAX_SIZE);//清空

	pdata->i2c_wbuf[0] = reg;

	if (i2c_transfer(client->adapter, msg, 2) != 2) {
		pr_err("i2c read failed\n");
		return -EIO;
	}

	memcpy(buf, pdata->i2c_rbuf, size);

	return 0;
}


static int aw37504_read_config_reg(struct aw37504 *pdata)
{
	u8 rev0 = 0, rev1 = 0;
	int ret = 0;

	printk("aw37504_read_config_reg success begin\n");

	if (!aw37504_read(pdata, 0x00, &rev0, 1) && !aw37504_read(pdata, 0x01, &rev1, 1)) {
		printk("aw37504_read_config_reg rev0=%x rev1=%x \n", rev0, rev1);
	} else {
		pr_err("AW37504 get configure register failed\n");
		ret = -1;
	}

	aw37504_write_byte(pdata, 0x00, 0x13);
	aw37504_write_byte(pdata, 0x01, 0x13);
	aw37504_write_byte(pdata, 0x03, 0x43);
	aw37504_write_byte(pdata, 0x04, 0x09);

	if (!aw37504_read(pdata, 0x00, &rev0, 1) && !aw37504_read(pdata, 0x01, &rev1, 1)) {
		printk("aw37504_read_config_reg rev0=%x rev1=%x \n", rev0, rev1);
	} else {
		pr_err("AW37504 get configure register failed\n");
		ret = -1;
	}

	printk("aw37504_read_config_reg success end\n");

	return ret;
}


static int aw37504_remove(struct i2c_client * client)
{
	dev_info(&client->dev, "aw37504 driver removed");

	return 0;
}
static int aw37504_read_node(struct aw37504 *info, u8 reg, u8 *data)
{
	int ret;

	ret = i2c_smbus_read_byte_data(info->i2c_client, reg);
	if (ret < 0)
		return ret;

	*data = ret;
	return 0;
}

static ssize_t aw37504_config_node_show(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{
	ssize_t len = 0;
	unsigned char i = 0;
	unsigned char reg_val = 0;

	if(aw37504_pdata_global==NULL)
		return 0;

	for(i = 0x00; i <= 0x01; i++) {
		aw37504_read_node(aw37504_pdata_global, i, &reg_val);
		len += snprintf(buf+len, PAGE_SIZE-len, "reg[0x%02x]=0x%02x \n", i, reg_val);
	}

	return len;
}


#ifdef CONFIG_BESTSERVICE_HARDWARE_CHECK
static ssize_t 	lcd_hardware_self_check_show(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{

	int ret;

	if (g_lcd_hardware_check_flag >= 0)
		ret = 0;
	else
		ret = -1;

	return scnprintf(buf, PAGE_SIZE, "%d \n", ret);

}
#endif

static struct kobj_attribute dev_attr_aw37504_config_node =
__ATTR(aw37504_config_node, 0664, aw37504_config_node_show, NULL);

#ifdef CONFIG_BESTSERVICE_HARDWARE_CHECK
static struct kobj_attribute dev_attr_lcd_hardware_self_check =
__ATTR(lcd_hardware_self_check, 0664, lcd_hardware_self_check_show, NULL);
#endif

static struct attribute *aw37504_node_attrs[] = {
	&dev_attr_aw37504_config_node.attr,
#ifdef CONFIG_BESTSERVICE_HARDWARE_CHECK
	&dev_attr_lcd_hardware_self_check.attr,
#endif
	NULL,
};

static struct attribute_group aw37504_node_attr_group = {
	.attrs = aw37504_node_attrs,
};

static int aw37504_node(void)
{
	int err;
	struct kobject *sys_kobj = NULL;

	printk("%s: creat aw37504_node \n",__func__);

	aw37504_node_obj = kobject_create_and_add("aw37504", sys_kobj);
	if (!aw37504_node_obj) {
		printk("%s:	kobject creation failed.\n",__func__);
	}

	err = sysfs_create_group(aw37504_node_obj, &aw37504_node_attr_group);
	if (err) {
		printk( "%s:failed to create aw37504 attributes group\n",__func__);
	}
	printk("%s: creat aw37504_node success \n",__func__);
	return err;
}

static int aw37504_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{
	int ret = 0;
	struct aw37504 *pdata;
	//u8 chip_version = 0;

	printk("aw37504_probe success begin \n ");
	/* do NOT remove these logs */

	printk("AW37504 I2C Address: 0x%02x\n", client->addr);

	if (!client || !client->dev.of_node) {
		pr_err("invalid input\n");
		return -EINVAL;
	}

	if(!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		dev_err(&client->dev, "Failed check I2C functionality");
		return -ENODEV;
	}

	printk("i2c_check_functionality success\n");


	pdata = devm_kzalloc(&client->dev,
			sizeof(struct aw37504), GFP_KERNEL);  //创建本地空间
	if (!pdata)
		return -ENOMEM;

	pdata->i2c_client=client;
	pdata->dev = &client->dev;

	ret = aw37504_read_config_reg(pdata); //读取设备配置
	if (ret) {
		pr_err("failed to read config reg\n");
		//goto err_i2c_prog;
	}

	aw37504_pdata_global = pdata;
	aw37504_node();

	printk("aw37504_probe success end \n ");

	return 0;
};



static struct i2c_device_id aw37504_id[] = {
	{ "lt,aw37504uxc", 0},
	{}
};

static const struct of_device_id aw37504_match_table[] = {
	{.compatible = "awinic,aw37504",},
	{ },
};
MODULE_DEVICE_TABLE(of, aw37504_match_table);

static struct i2c_driver aw37504_driver = {
	.driver = {
		.name = "aw37504",
		.owner    = THIS_MODULE,
		.of_match_table = aw37504_match_table,
	},
	.probe = aw37504_probe,
	.remove = aw37504_remove,
	.id_table = aw37504_id,
};


int aw37504_init(void)
{
	s32 ret;

	printk("Lontium AW37504 driver installing....\n ");
	ret = i2c_add_driver(&aw37504_driver);

	return ret;
}

void aw37504_exit(void)
{
	printk("Lontium AW37504 driver exited\n");

	i2c_del_driver(&aw37504_driver);
}

