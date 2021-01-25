
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2020, The Linux Foundation. All rights reserved.
 * Copyright (C) 2021, Anhui Yunshu Zhihui Technology Co., Ltd.
 */

#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>

// #include <linux/platform_device.h>
// #include <linux/fs.h>
 #include <linux/delay.h>
 #include <linux/i2c.h>
 #include <linux/gpio.h>
 #include <linux/interrupt.h>
// #include <linux/component.h>
// #include <linux/workqueue.h>
// #include <linux/of_gpio.h>
// #include <linux/of_graph.h>
// #include <linux/of_irq.h>
// #include <linux/regulator/consumer.h>
 #include <linux/firmware.h>
//#include <linux/hdmi.h>
#include <linux/string.h>
// #include <drm/drmP.h>
// #include <drm/drm_atomic.h>
// #include <drm/drm_atomic_helper.h>
// #include <drm/drm_edid.h>
// #include <drm/drm_mipi_dsi.h>
// #include <drm/drm_crtc_helper.h>
#include "lt9711exb.h"
#include "lt9711_firmware.h"

struct kobject *lt9711_node_obj = NULL;
struct lt9711 *pdata_global = NULL;

#ifdef CONFIG_CUSTOM_HARDWARE_CHECK
static int g_lcd_hardware_check_flag = -1;
#endif

/*
 * Write one reg with more values;
 * Reg -> value0, value1, value2.
 * reg是寄存器的地址
 */

static int lt9711_write(struct lt9711 *pdata, u8 reg,const u8 *buf, int size)
{
	struct i2c_client *client = pdata->i2c_client;
	struct i2c_msg msg = {
		.addr = client->addr,
		.flags = 0,
		.len = size + 1,
		.buf = pdata->i2c_wbuf,
	};

	pdata->i2c_wbuf[0] = reg; //msg[0]代表的是寄存器地址
	if (size > (WRITE_BUF_MAX_SIZE - 1)) {  //写的字节大小不能超过64字节
		pr_err("invalid write buffer size %d\n", size);
		return -EINVAL;
	}

	memcpy(pdata->i2c_wbuf + 1, buf, size);//将传递过来的数据保存到msg[1]以及之后的缓冲区
	if (i2c_transfer(client->adapter, &msg, 1) < 1) {//发送数据
		pr_err("i2c write failed\n");
		return -EIO;
	}
	return 0;
}



/*
 * Write one reg with one value;  发送一个字节的数据
 * Reg -> value
 */
static int lt9711_write_byte(struct lt9711 *pdata, const u8 reg, u8 value)
{
	struct i2c_client *client = pdata->i2c_client;
	struct i2c_msg msg = {
		.addr = client->addr,
		.flags = 0,
		.len = 2,
		.buf = pdata->i2c_wbuf,
	};
   // printk("lt9711_write_byte  1 success\n");
	//printk("lt9711_write_byte  2 success\n %d",msg.len);
 	//printk("lt9711_write_byte  1 success =%ld  \n",sizeof(pdata->i2c_wbuf));
	memset(pdata->i2c_wbuf, 0,  sizeof(pdata->i2c_wbuf));
	//bzero(pdata->i2c_wbuf,sizeof(pdata->i2c_wbuf));
	//printk("lt9711_write_byte  3 success\n");

	pdata->i2c_wbuf[0] = reg;
	pdata->i2c_wbuf[1] = value;
	//printk("lt9711_write_byte  3 success\n");
	if (i2c_transfer(client->adapter, &msg, 1) < 1) {
		pr_err("i2c write failed\n");
		return -EIO;
	}
	//printk("lt9711_write_byte  5 success\n");
	return 0;
}

/*
 * Write more regs with more values;
 * Reg1 -> value1
 * Reg2 -> value2
 */


static void lt9711_write_array(struct lt9711 *pdata,
	struct lt9711_reg_cfg *reg_arry, int size)
{
	int i = 0;
	//printk("lt9711_write_array size=%d",size);
	for (i = 0; i < size; i++)
		lt9711_write_byte(pdata, reg_arry[i].reg, reg_arry[i].val);
}



static int lt9711_read(struct lt9711 *pdata, u8 reg, char *buf, u32 size)
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


 void lt9711_flash_write_config(struct lt9711 *pdata)
{
	struct lt9711_reg_cfg reg_cfg[] = {
		{0xFF, 0x80},
		{0xEE, 0x01},
		{0x5A, 0x80},
		{0x5E, 0xDF},
		{0x58, 0x01},
		{0x59, 0x51},
		{0x5A, 0x90},
		{0x5A, 0x80},
		{0x58, 0x21},
	};


	lt9711_write_array(pdata, reg_cfg, ARRAY_SIZE(reg_cfg));
	msleep(100);
}




 void lt9711_block_erase(struct lt9711 *pdata)
{
	struct lt9711_reg_cfg reg_cfg[] = {
		{0xFF, 0x80},
		{0xEE, 0x01},
		{0x5A, 0x80},
		{0x5A, 0x84},
		{0x5A, 0x80},
		{0x5B, 0x00},
		{0x5C, 0x00},
		{0x5D, 0x00},
		{0x5A, 0x81},
		{0x5A, 0x80},
	};

	printk("LT9711 block erase\n");
	lt9711_write_array(pdata, reg_cfg, ARRAY_SIZE(reg_cfg));
	msleep(1500);
	printk("LT9711 block erase end\n");
}


static int lt9711_read_device_id(struct lt9711 *pdata)
{
	u8 rev0 = 0, rev1 = 0;
	int ret = 0;
printk("lt9711_read_device_id  1 success\n");
	lt9711_write_byte(pdata, 0xFF, 0x80);//切换到0x80块
	lt9711_write_byte(pdata, 0xEE, 0x01);//enable I2C access 外部
	lt9711_write_byte(pdata, 0xFF, 0x81);//切换到0x81块

	if (!lt9711_read(pdata, 0x00, &rev0, 1) &&
		!lt9711_read(pdata, 0x01, &rev1, 1)) {
		printk("lt9711_read_device_id rev0=%x rev1=%x \n",rev0,rev1);
	} else {
		pr_err("LT9711 get id failed\n");
		ret = -1;
	}

	lt9711_write_byte(pdata, 0xFF, 0x80);//切换到0x80块
	lt9711_write_byte(pdata, 0xEE, 0x00);//enable I2C access 内部
	msleep(50);
	printk("lt9711_read_device_id  2 success\n");
	return ret;
}
static int lt9711_get_version(struct lt9711 *pdata)
{
	u8 revison1 = 0;
	u8 revison2 = 0;
	u8 revison3 = 0;
	u8 revison4 = 0;
	u8 revison5 = 0;
	u8 revison6 = 0;
	int ret=0;
	lt9711_write_byte(pdata, 0xFF, 0x80);
	if (!lt9711_read(pdata, 0xd0, &revison1, 1) &&
		!lt9711_read(pdata, 0xd1, &revison2, 1)	&&
		!lt9711_read(pdata, 0xd2, &revison3, 1)	&&
		!lt9711_read(pdata, 0xd3, &revison4, 1)	&&
		!lt9711_read(pdata, 0xd4, &revison5, 1)	&&
		!lt9711_read(pdata, 0xd5, &revison6, 1))
		{
			if(revison1==LT9711_YEAR_HIGH && revison2 == LT9711_YEAR_LOW && revison3 == LT9711_MONTH&& revison4 == LT9711_DAY && revison5 == LT9711_VERSION_HIGH && revison6 == LT9711_VERSION_LOW){
				printk("lt9711_get_version FW=%x %x %x %x %x %x \n",revison1,revison2,revison3,revison4,revison5,revison6);
				ret = 0;
#ifdef CONFIG_CUSTOM_HARDWARE_CHECK
				g_lcd_hardware_check_flag = 0;
#endif
			}
			else {
				printk("lt9711 get revison value is wrong \n");
				printk("lt9711_get_version FW=%x %x %x %x %x %x \n",revison1,revison2,revison3,revison4,revison5,revison6);
				ret = 1;
#ifdef CONFIG_CUSTOM_HARDWARE_CHECK
				g_lcd_hardware_check_flag = -1;
#endif
			}
		}
	else{
			printk("lt9711 get revison failed \n");
			ret = 1;
#ifdef CONFIG_CUSTOM_HARDWARE_CHECK
			g_lcd_hardware_check_flag = -1;
#endif
	}

	msleep(20);

	return ret;
}
void lt9711_firmware_write(struct lt9711 *pdata,const u8 *Codedata,u16 Length,u32 Addrss)
{
	u8 addr1=0;
	u8 addr2=0;
	u8 addr3=0;
	u32 LT9711_FlashAdd=0;
	u16 LT9711_Count=0;
	int rest_data = 0;
	u8 last_buf[32];

	rest_data = Length % 32; //剩余字节
	memset(last_buf, 0xFF, 32);
	printk("lt9711_firmware_write begin Length = %x\n",Length);
	printk("lt9711_firmware_write begin\n");
	LT9711_FlashAdd=Addrss;
	lt9711_write_byte(pdata,0xFF,0x80);
	lt9711_write_byte(pdata,0xEE,0x01);
	lt9711_write_byte(pdata,0xFF,0x81);
	lt9711_write_byte(pdata,0x08,0xbf);
	lt9711_write_byte(pdata,0x08,0xff);
	lt9711_write_byte(pdata,0xFF,0x80);

	while((LT9711_Count+32)<Length)  //满32字节的
	{
	    addr1=LT9711_FlashAdd;
		addr2=LT9711_FlashAdd>>8;
		addr3=LT9711_FlashAdd>>16;
		lt9711_write_byte(pdata,0x5A,0x84);
		lt9711_write_byte(pdata,0x5A,0x80);
	    lt9711_write_byte(pdata,0x5E,0xDF);
		lt9711_write_byte(pdata,0x5A,0xA0);
		lt9711_write_byte(pdata,0x5A,0x80);
		lt9711_write_byte(pdata,0x58,0x21);
		lt9711_write(pdata, 0x59,Codedata+LT9711_Count, 32);//  (0x59,Codedata+LT9711_Count,32);
		lt9711_write_byte(pdata,0x5b,addr3);
		lt9711_write_byte(pdata,0x5c,addr2);
		lt9711_write_byte(pdata,0x5d,addr1);
		lt9711_write_byte(pdata,0x5E,0xC0);
		lt9711_write_byte(pdata,0x5A,0x90);
		lt9711_write_byte(pdata,0x5A,0x80);
		LT9711_FlashAdd=LT9711_FlashAdd+0x20;
		LT9711_Count = LT9711_Count+32;
		//msleep(20);
		//printk("lt9711_firmware_write while %x\n",LT9711_Count);
  }
	if(rest_data==0){
		//printk("lt9711_firmware_write while %x\n",LT9711_Count);
		//printk("lt9711_firmware_write %x",*(Codedata+LT9711_Count));
	    addr1=LT9711_FlashAdd;
		addr2=LT9711_FlashAdd>>8;
		addr3=LT9711_FlashAdd>>16;
		lt9711_write_byte(pdata,0x5A,0x84);
		lt9711_write_byte(pdata,0x5A,0x80);
	    lt9711_write_byte(pdata,0x5E,0xDF);
		lt9711_write_byte(pdata,0x5A,0xA0);
		lt9711_write_byte(pdata,0x5A,0x80);
		lt9711_write_byte(pdata,0x58,0x21);
		lt9711_write(pdata, 0x59,Codedata+LT9711_Count, 32);//  (0x59,Codedata+LT9711_Count,32);
		lt9711_write_byte(pdata,0x5b,addr3);
		lt9711_write_byte(pdata,0x5c,addr2);
		lt9711_write_byte(pdata,0x5d,addr1);
		lt9711_write_byte(pdata,0x5E,0xC0);
		lt9711_write_byte(pdata,0x5A,0x90);
		lt9711_write_byte(pdata,0x5A,0x80);
		//msleep(20);
		}
	if(rest_data>0){

		  memcpy(last_buf, Codedata+LT9711_Count, rest_data);
		  //剩余不满32字节的
		  addr1=LT9711_FlashAdd;
			addr2=LT9711_FlashAdd>>8;
			addr3=LT9711_FlashAdd>>16;
			lt9711_write_byte(pdata,0x5A,0x84);
			lt9711_write_byte(pdata,0x5A,0x80);
			lt9711_write_byte(pdata,0x5E,0xDF);
			lt9711_write_byte(pdata,0x5A,0xA0);
			lt9711_write_byte(pdata,0x5A,0x80);
			lt9711_write_byte(pdata,0x58,0x21);

		    lt9711_write(pdata, 0x59,last_buf, 32);//(0x59,Codedata+LT9711_Count,Length-LT9711_Count);
			lt9711_write_byte(pdata,0x5b,addr3);
			lt9711_write_byte(pdata,0x5c,addr2);
			lt9711_write_byte(pdata,0x5d,addr1);
			lt9711_write_byte(pdata,0x5E,0xC0);
			lt9711_write_byte(pdata,0x5A,0x90);
			lt9711_write_byte(pdata,0x5A,0x80);
			//msleep(20);
	}
	printk("lt9711_firmware_write end\n");

}


void lt9711_flash_read_addr_set(struct lt9711 *pdata, u32 addr)
{
	struct lt9711_reg_cfg reg_cfg[] = {
		{0x5E, 0x5F},
		{0x5A, 0xA0},
		{0x5A, 0x80},
		{0x5B, (addr & 0xFF0000) >> 16},
		{0x5C, (addr & 0xFF00) >> 8},
		{0x5D, addr & 0xFF},
		{0x5A, 0x90},
		{0x5A, 0x80},
		{0x58, 0x21},
	};

	lt9711_write_array(pdata, reg_cfg, ARRAY_SIZE(reg_cfg));
}


//CRC位置的值
int lt9711_fw_read_crc(struct lt9711 *pdata)
{
	u8 page_data[2]={0,0};
	int addr = 24575; //CRC地址
	u8 crc_value = 0x06;

	struct lt9711_reg_cfg reg_cfg[] = {
		{0xFF, 0x80},
		{0xEE, 0x01},
		{0x5a, 0x80},
		{0x5e, 0xdf},
		{0x58, 0x01},
		{0x59, 0x51},
		{0x5a, 0x90},
		{0x5a, 0x80},
		{0x58, 0x21},
		{0xFF, 0x81},
		{0x08, 0xbf},
		{0x08, 0xff},
		{0xff, 0x80},
		{0x5a, 0x84},
		{0x5a, 0x80},
	};
	lt9711_write_array(pdata, reg_cfg, ARRAY_SIZE(reg_cfg));
	lt9711_flash_read_addr_set(pdata, addr);
	lt9711_read(pdata, 0x5F, page_data, 1);

	if(page_data[0]==crc_value){
		printk("lt9711_fw_read_crc success\n");
		return 0;
	}else{
		printk("lt9711_fw_read_crc fail\n");
		return 1;
	}
}



void lt9711_fw_read_back(struct lt9711 *pdata, u8 *buff, int size, u32 Addrcode)
{
	u8 page_data[32];
	int page_number = 0, i = 0, addr = 0;

	struct lt9711_reg_cfg reg_cfg[] = {
		{0xFF, 0x80},
		{0xEE, 0x01},
		{0x5a, 0x80},
		{0x5e, 0xdf},
		{0x58, 0x01},
		{0x59, 0x51},
		{0x5a, 0x90},
		{0x5a, 0x80},
		{0x58, 0x21},
		{0xFF, 0x81},
		{0x08, 0xbf},
		{0x08, 0xff},
		{0xff, 0x80},
		{0x5a, 0x84},
		{0x5a, 0x80},
	};
	/*
	 * Read 32 bytes once.
	 */
	page_number = size / 32;
	if (size % 32)//判断是否再多读一次
		page_number++;
	addr=Addrcode;
	//开始真正的读
	lt9711_write_array(pdata, reg_cfg, ARRAY_SIZE(reg_cfg));

	for (i = 0; i < page_number; i++) {
		memset(page_data, 0x0, 32);
		lt9711_flash_read_addr_set(pdata, addr);
		lt9711_read(pdata, 0x5F, page_data, 32);
		memcpy(buff, page_data, 32);
		buff += 32;
		addr += 32;
	}
}


int lt9711_firmware_upgrade(struct lt9711 *pdata,const u8 *cfg,int size)
{
 	int i = 0;
	u8 *fw_read_data = NULL;
	int data_len = size; //文件内容的大小

	printk("LT9711 FW total size %d\n", data_len);
	fw_read_data = kzalloc(ALIGN(data_len, 32), GFP_KERNEL);//就是将里面的内容以32字节形式对齐
	if (!fw_read_data)
		return -1;

	pdata->fw_status = UPDATE_RUNNING;
	//升级开始 LT19711 Firmware update flow description
	lt9711_flash_write_config(pdata);


	for (i = 0; i < 2; i++)
		lt9711_block_erase(pdata);

	lt9711_firmware_write(pdata, cfg, data_len,0x00);//参数2 数据的内容的首地址    参数3 数据总共字节   参数4 首地址
	msleep(20);
	printk("lt9711_firmware_write success.\n");
 	lt9711_fw_read_back(pdata, fw_read_data, data_len,0x00);//参数2

	 if (!memcmp(cfg, fw_read_data, data_len)) {
		 pdata->fw_status = UPDATE_SUCCESS;
		 kfree(fw_read_data);
		 printk("LT9711 Firmware upgrade success.\n");
		 //烧入成功后reset 50 gpio
		 if(gpio_request(114,NULL) == 0){
			 gpio_direction_output(114,0);
			 msleep(1);
			 gpio_direction_output(114,1);
			 gpio_free(114);
		 }

		 return 1;

	 } else {
		 pdata->fw_status = UPDATE_FAILED;
		 kfree(fw_read_data);
		 printk("LT9711 Firmware upgrade failed\n");
		 //烧入失败后reset 50 gpio
		 if(gpio_request(114,NULL) == 0){
			 gpio_direction_output(114,0);
			 msleep(1500);
			 gpio_direction_output(114,1);
			 gpio_free(114);
		 }
		 return -1;
	 }


}

static void lt9711_firmware_cb(const u8 *cfg, void *data)
{
	struct lt9711 *pdata = (struct lt9711 *)data;
	int ret=0,i=0;
	printk("lt9711_firmware_cb enter 1 success\n");
	if (!cfg) {
		pr_err("LT9711 get firmware failed\n");
		return;
	}
	printk("lt9711_firmware_cb enter 2 success\n");

	for(i=0;i<3;i++){
	  ret=lt9711_firmware_upgrade(pdata, cfg,24576);//关键是此处 //24576是data大小
	  if(ret>0)
		  break;
	}
	//release_firmware(cfg);
	printk("lt9711_firmware_cb enter 3 success\n");
}

static int lt9711exb_remove(struct i2c_client * client)
{


    dev_info(&client->dev, "goodix lt8911exb driver removed");

    return 0;
}
static int lt9711_read_node(struct lt9711 *info, u8 reg, u8 *data)
{
	int ret;


	ret = i2c_smbus_read_byte_data(info->i2c_client, reg);
	if (ret < 0)
		return ret;

	*data = ret;
	return 0;
}
static ssize_t lt9711_node_show(struct kobject *kobj,
							 struct kobj_attribute *attr, char *buf)
{
	 ssize_t len = 0;
	unsigned char i = 0;
   unsigned char reg_val = 0;
	if(pdata_global==NULL)
		return 0;
	lt9711_write_byte(pdata_global, 0xFF, 0x80);

   for(i = 0xd0; i < 0xd6; i++) {
           lt9711_read_node(pdata_global, i, &reg_val);
	   len += snprintf(buf+len, PAGE_SIZE-len, "reg:0x%02x=0x%02x \n", i, reg_val);
   }
   return len;
}

static ssize_t lt9711_screen_node_show(struct kobject *kobj,
							 struct kobj_attribute *attr, char *buf)
{
	 ssize_t len = 0;
	//unsigned char i = 0;
	unsigned char reg_val = 0;
   unsigned char reg_val_1 = 0;
   unsigned char reg_val_2 = 0;
   unsigned char reg_val_3 = 0;
   unsigned char reg_val_4 = 0;
   unsigned char reg_val_5 = 0;
   int lane0 = 0;
   int lane1 = 0;
	if(pdata_global==NULL)
		return 0;
	lt9711_write_byte(pdata_global, 0xFF, 0x80);
	lt9711_write_byte(pdata_global, 0xEE, 0x00);
	lt9711_write_byte(pdata_global, 0xFF, 0x80);
	lt9711_write_byte(pdata_global, 0xE2, 0x06);
	msleep(30);
	lt9711_read_node(pdata_global, 0xdc, &reg_val);
	if (reg_val==0x11)
	{
		lt9711_read_node(pdata_global, 0xd7, &reg_val_1);
		lt9711_read_node(pdata_global, 0xd8, &reg_val_2);
		lt9711_read_node(pdata_global, 0xd9, &reg_val_3);
		lt9711_read_node(pdata_global, 0xda, &reg_val_4);
		lt9711_read_node(pdata_global, 0xdb, &reg_val_5);
		lane0 = reg_val_2;
		lane0 = (lane0<<8)+reg_val_1-0x8000;
		lane1 = reg_val_4;
		lane1 = (lane1<<8)+reg_val_3-0x8000;

		len += snprintf(buf+len, PAGE_SIZE-len, "lane0=%d\n",lane0);//lane0
		len += snprintf(buf+len, PAGE_SIZE-len, "lane1=%d\n",lane1);//lane1
		len += snprintf(buf+len, PAGE_SIZE-len, "0x0201=0x%02x\n",reg_val_5);//lane1
/* 	   for(i = 0xd7; i < 0xdc; i++) {
			lt9711_read_node(pdata_global, i, &reg_val);
			len += snprintf(buf+len, PAGE_SIZE-len, "0x%02x=0x%02x\n", i, reg_val);
		} */
	}
	lt9711_write_byte(pdata_global, 0xE2, 0x00);
   return len;
}

#ifdef CONFIG_CUSTOM_HARDWARE_CHECK
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

static struct kobj_attribute dev_attr_lt9711_node =
	__ATTR(lt9711_show_version, 0664, lt9711_node_show, NULL);

static struct kobj_attribute dev_attr_lt9711_screen_node =
	__ATTR(lt9711_show_screen, 0664, lt9711_screen_node_show, NULL);
#ifdef CONFIG_CUSTOM_HARDWARE_CHECK
static struct kobj_attribute dev_attr_lcd_hardware_self_check =
	__ATTR(lcd_hardware_self_check, 0664, lcd_hardware_self_check_show, NULL);
#endif

static struct attribute *lt9711_node_attrs[] = {
	&dev_attr_lt9711_node.attr,
	&dev_attr_lt9711_screen_node.attr,
#ifdef CONFIG_CUSTOM_HARDWARE_CHECK
	&dev_attr_lcd_hardware_self_check.attr,
#endif
	NULL,
};
static struct attribute_group lt9711_node_attr_group = {
	.attrs = lt9711_node_attrs,
};
static int lt9711_node(void)
{
	int err;
	struct kobject *sys_kobj=NULL;

	printk("%s: [cjw] creat lt9711_node \n",__func__);
	lt9711_node_obj = kobject_create_and_add("lt9711", sys_kobj);
	if (!lt9711_node_obj) {
		printk("%s:	kobject creation failed.\n",__func__);
	}

	err = sysfs_create_group(lt9711_node_obj, &lt9711_node_attr_group);
	if (err) {
		printk( "%s:failed to create lt9711 attributes group\n",__func__);
	}
	printk("%s: [cjw] creat lt9711_node success \n",__func__);
	return err;
}
static int lt9711exb_probe(struct i2c_client *client,
	 const struct i2c_device_id *id)
{
	int ret = 0;
	int res = 0;
	struct lt9711 *pdata;
	//u8 chip_version = 0;

	printk("lt9711exb_probe success\n ");
    /* do NOT remove these logs */

    printk("LT9711EXB I2C Address: 0x%02x\n", client->addr);

	if (!client || !client->dev.of_node) {
		pr_err("invalid input\n");
		return -EINVAL;
	}
	printk("i2c_check_functionality success\n");
    if(!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
        dev_err(&client->dev, "Failed check I2C functionality");
        return -ENODEV;
    }

	printk("i2c_check_functionality success\n");


	pdata = devm_kzalloc(&client->dev,
		sizeof(struct lt9711), GFP_KERNEL);  //创建本地空间
	if (!pdata)
		return -ENOMEM;

	pdata->i2c_client=client;
	pdata->dev = &client->dev;

	ret = lt9711_read_device_id(pdata); //读取设备id
 	if (ret) {
		pr_err("failed to read chip rev\n");
		goto err_i2c_prog;
	}
	/*
	 * Need erase block 2 timess here.
	 * Sometimes, erase can fail.
	 * This is a workaroud.
	 */

	 ret = lt9711_get_version(pdata);
	 res = lt9711_fw_read_crc(pdata);
	 //lt9711_firmware_cb(LT9711_firmware_code,pdata);
 	 if(1==ret || res==1){
	  lt9711_firmware_cb(LT9711_firmware_code,pdata);
	 }else{
		 lt9711_get_version(pdata);
	 }


	 pdata_global = pdata;
	 lt9711_node();
	 //kernel_restart(NULL);
	 //if(1==ret)
	/*
	 {
		ret = request_firmware_nowait(THIS_MODULE, true,
			"LT9711_KDXF_20210611_V3.bin", &client->dev, GFP_KERNEL, pdata,
				lt9711_firmware_cb);
		if (ret) {
			dev_err(&client->dev,
				"Failed to invoke firmware loader: %d\n", ret);
			printk("Failed to invoke firmware loader %d\n",ret);
			goto err_i2c_prog;
		} else{
			 lt9711_get_version(pdata);
			return 0;
		}
	 }
	 */
	printk(" lt9711_get_version ret = %d \n",ret);
	return 0;
 err_i2c_prog:
	devm_kfree(&client->dev, pdata);
	return ret;
};



static struct i2c_device_id lt9711exb_id[] = {
	{ "lt,lt9711uxc", 0},
	{}
};

static const struct of_device_id lt9711exb_match_table[] = {
    {.compatible = "lontium,lt9711exb",},
    { },
};
MODULE_DEVICE_TABLE(of, lt9711exb_match_table);

static struct i2c_driver lt9711exb_driver = {
	.driver = {
		.name = "lt9711exb",
		.owner    = THIS_MODULE,
		.of_match_table = lt9711exb_match_table,
	},
	.probe = lt9711exb_probe,
	.remove = lt9711exb_remove,
	.id_table = lt9711exb_id,
};


static int __init lt9711exb_init(void)
{

	s32 ret;
	printk("Lontium LT9711EXB driver installing....\n ");
    ret = i2c_add_driver(&lt9711exb_driver);
    return ret;

}

static void __exit lt9711exb_exit(void)
{

	printk("Lontium LT9711EXB driver exited\n");
    i2c_del_driver(&lt9711exb_driver);

}

fs_initcall(lt9711exb_init);
module_exit(lt9711exb_exit);

MODULE_DESCRIPTION("Lontium LT9711EXB Driver");
MODULE_LICENSE("GPL V2");
