/*
* SPDX-License-Identifier: GPL-2.0-only OR MIT
*****************************************************************************
* Copyright by ams AG                                                       *
* All rights are reserved.                                                  *
*                                                                           *
* IMPORTANT - PLEASE READ CAREFULLY BEFORE COPYING, INSTALLING OR USING     *
* THE SOFTWARE.                                                             *
*                                                                           *
* THIS SOFTWARE IS PROVIDED FOR USE ONLY IN CONJUNCTION WITH AMS PRODUCTS.  *
* USE OF THE SOFTWARE IN CONJUNCTION WITH NON-AMS-PRODUCTS IS EXPLICITLY    *
* EXCLUDED.                                                                 *
*                                                                           *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       *
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT         *
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS         *
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT  *
* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,     *
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT          *
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     *
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY     *
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT       *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE     *
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.      *
*****************************************************************************
*/

#include <linux/module.h>
#include <linux/ctype.h>
#include <linux/workqueue.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>

#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/mutex.h>
#include <linux/unistd.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/slab.h>
#include <linux/pm.h>
#include <linux/module.h>
#include <linux/device.h>
//#include <linux/ams_tcs3430.h>
#include "tcs3430_regs.h"
#include "ams_tcs3430.h"
#include <linux/of_irq.h>
#include <linux/of_platform.h>

#ifdef CONFIG_SENSORS_TCS3430_WITH_LUX_CCT
#include "tcs3430_als.h"
#endif

#include <linux/delay.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/serio.h>
#include <linux/workqueue.h>
#include <linux/libps2.h>
#include <linux/mutex.h>
#include <linux/dmi.h>


#define TCS3430_CMD_ALS_INT_CLR 0x10
#define TCS3430_CMD_ALL_INT_CLR	0x98
#define tcs_info(fmt, arg...)	pr_info("[TCS-INF][%s:%d] "fmt"\n", __func__, __LINE__, ##arg)
static int tcs3430_read_all(struct tcs3430_chip *chip);
static void tcs3430_get_als(struct tcs3430_chip *chip);
static int tcs3430_als_mux_toggle(struct tcs3430_chip *chip);

enum tsc3430_amux_operation {
	TCS3430_X_ONLY,
	TCS3430_IR2_ONLY,
	TCS3430_X_IR2_TOGGLE,
};

enum tcs3430_ir_mux {
	TCS3430_AMUX_IR2_DATA = (1 << 3),
	TCS3430_AMUX_X_DATA   = (0 << 3),
};

enum tcs3430_en_reg {
	TCS3430_EN_PON        = (1 << 0),
	TCS3430_EN_AEN        = (1 << 1),
	TCS3430_EN_WEN        = (1 << 3),
};

enum tcs3430_status {
	TCS3430_ST_CINT       = (1 << 3),
	TCS3430_ST_AINT       = (1 << 4),
	TCS3430_ST_ASAT       = (1 << 7),
};

enum tcs3430_ctrl_reg {
	AGAIN_MASK_1   = (0 << 0),
	AGAIN_MASK_4   = (1 << 0),
	AGAIN_MASK_16  = (2 << 0),
	AGAIN_MASK_64  = (3 << 0),
};

static u8 const tcs3430_ids[] = {
	0xdc,
};

static char const *tcs3430_names[] = {
	"tcs34301",
};

static u8 const restorable_regs[] = {
	TCS3430_REG_ATIME, //ATIME Register
	TCS3430_REG_PERS,  //PERS Register 
	TCS3430_REG_CFG1,	//CFG1 Register 
};

u8 const als_gains[] = {
	1,
	4,
	16,
	64
};

static u8 const reg_addr[] = {
	0x80,  /* ENABLE */
	0x81,  /* ATIME */
	0x83,  /* WTIME */
	0x84,  /* AILTL */
	0x85,  /* AILTH */
	0x86,  /* AIHTL */
	0x87,  /* AIHTH */
	0x8C,  /* PERS */
	0x90,  /* CFG1 */
	0x91,  /* REVID */
	0x92,  /* ID */
	0x93,  /* WTIME */
	0x94,  /* CH0DATAL */
	0x95,  /* CH0DATAH */
	0x96,  /* CH1DATAL */
	0x97,  /* CH1DATAH */
	0x98,  /* CH2DATAL */
	0x99,  /* CH2DATAH */
	0x9A,  /* CH3DATAL */
	0x9B,  /* CH3DATAH */
	0x9F,  /* CFG2 */
	0xAB,  /* CFG3 */
	0xD6,  /* AZ_CONFIG */
	0xDD,  /* INTENAB */
};

static int tcs3430_i2c_blk_read(struct tcs3430_chip *chip, u8 reg,
				u8 *val, int size)
{
	s32 ret;
	u8 addr;
	struct i2c_client *client = chip->client;
	
	addr = reg_addr[reg];
	ret = i2c_smbus_read_i2c_block_data(client, addr, size, val);
	if (ret < 0)
		dev_err(&client->dev, "%s: failed at address %x (%d bytes)\n",
			__func__, addr, size);
	return ret;
}

static int tcs3430_i2c_read(struct tcs3430_chip *chip, u8 reg, u8 *val)
{
	int ret;
	u8 addr;
	s32 read;
	struct i2c_client *client = chip->client;

	addr = reg_addr[reg];
	ret = i2c_smbus_write_byte(client, addr);
	if (ret < 0) {
		mdelay(3);
		ret = i2c_smbus_write_byte(client, addr);
		if (ret < 0) {
			dev_err(&client->dev,
				"%s: failed 2x to write register %x\n",
				__func__, addr);
			return ret;
		}
	}

	read = i2c_smbus_read_byte(client);
	if (read < 0) {
		mdelay(3);
		read = i2c_smbus_read_byte(client);
		if (read < 0) {
			dev_err(&client->dev,
				"%s: failed read from register %x\n",
				__func__, addr);
		}
		return ret;
	}

	*val = (u8)read;
	return 0;
}

static int tcs3430_i2c_write(struct tcs3430_chip *chip, u8 reg, u8 val)
{
	int ret;
	u8 addr;
	struct i2c_client *client = chip->client;

	addr = reg_addr[reg];
	ret = i2c_smbus_write_byte_data(client, addr, val);
	if (ret < 0) {
		mdelay(3);
		ret = i2c_smbus_write_byte_data(client, addr, val);
		if (ret < 0) {
			dev_err(&client->dev,
				"%s: failed to write register %x err= %d\n",
				__func__, addr, ret);
		}
	}
	return ret;
}

static int tcs3430_flush_regs(struct tcs3430_chip *chip)
{
	unsigned i;
	int rc;
	u8 reg;

	dev_err(&chip->client->dev, "%s\n", __func__);

	for (i = 0; i < ARRAY_SIZE(restorable_regs); i++) {//3  循环3次  就是给我们的那三个 //ATIME Register //PERS Register  //CFG1 Register 寄存器赋值
		reg = restorable_regs[i];
		rc = tcs3430_i2c_write(chip, reg, chip->shadow[reg]);//
		if (rc) {
			dev_err(&chip->client->dev, "%s: err on reg 0x%02x\n",
					__func__, reg_addr[reg]);
			break;
		}
	}

	return rc;

}

static int tcs3430_update_enable_reg(struct tcs3430_chip *chip)
{
	return	tcs3430_i2c_write(chip, TCS3430_REG_ENABLE,
			chip->shadow[TCS3430_REG_ENABLE]);
}

static u8 tcs3430_gain_to_mask(int gain)
{
	u8 mask;

	switch (gain) {
	case 1:
		mask = AGAIN_MASK_1;
		break;
	case 4:
		mask = AGAIN_MASK_4;
		break;
	case 16:
		mask = AGAIN_MASK_16;
		break;
	case 64:
		mask = AGAIN_MASK_64;
		break;
	default:
		mask = 0xFF;
	}
	return mask;
}

static int tcs3430_set_als_gain(struct tcs3430_chip *chip, int gain)
{
	int rc;
	u8 gain_mask;
	u8 ctrl_reg;

	gain_mask = tcs3430_gain_to_mask(gain);
	if (gain_mask == 0xFF) {
		dev_err(&chip->client->dev, "%s: illegal als gain %d\n",
			__func__, gain);
		return -EINVAL;
	}

	ctrl_reg = chip->shadow[TCS3430_REG_CFG1] & ~TCS3430_ALS_GAIN_MASK;
	ctrl_reg |= gain_mask;
	rc = tcs3430_i2c_write(chip, TCS3430_REG_CFG1, ctrl_reg);
	if (!rc) {
		chip->shadow[TCS3430_REG_CFG1] = ctrl_reg;
		chip->params.als_gain = gain;
		dev_err(&chip->client->dev, "%s: new als gain %d\n",
			 __func__, gain);
	}
	return rc;
}

#ifdef CONFIG_SENSORS_TCS3430_WITH_LUX_CCT
static int tcs3430_get_lux(struct tcs3430_chip *chip)
{
	s32 lux = 0;
	s32 cct = 0;
	u32 sat = MAX_ALS_VALUE;//0Xffff

	if (chip->als_inf.y_raw <= MIN_ALS_VALUE) {//MIN_ALS_VALUE 10
#ifdef LUX_MESSAGES
		dev_err(&chip->client->dev,
			 "%s: darkness\n", __func__);
#endif
		lux = 0;
		goto exit;
	} else if (chip->als_inf.y_raw >= sat) {//sat 0Xffff
#ifdef LUX_MESSAGES
		dev_err(&chip->client->dev,
			 "%s: saturation, keep lux & cct\n", __func__);
#endif
		lux = chip->als_inf.lux;
		cct = chip->als_inf.cct;
		goto exit;
	}

	lux = tcs3430_calc_lux_fixed(chip);
	cct = tcs3430_calc_cct_fixed(chip);
exit:
	chip->als_inf.lux = (u16) lux;
	chip->als_inf.cct = (u16) cct;
	return 0;
}
#endif

static int tcs3430_pltf_power_on(struct tcs3430_chip *chip)
{

return 0;
/*
	int rc = 0;
	if (chip->pdata->platform_power) {
		rc = chip->pdata->platform_power(&chip->client->dev,POWER_ON);
		mdelay(10);
	}
	chip->unpowered = rc != 0;
	return rc;
	*/
}

static int tcs3430_pltf_power_off(struct tcs3430_chip *chip)
{

	return 0;
/*
	int rc = 0;
	if (chip->pdata->platform_power) {
		rc = chip->pdata->platform_power(&chip->client->dev,
			POWER_OFF);
		chip->unpowered = rc == 0;
	} else {
		chip->unpowered = false;
	}
	return rc;
*/
}

static int tcs3430_irq_clr(struct tcs3430_chip *chip, u8 int2clr)
{
	int ret, ret2;
	ret = tcs3430_i2c_write(chip, TCS3430_REG_STATUS, int2clr);
	if (ret < 0) {
		mdelay(3);
	    ret2 = tcs3430_i2c_write(chip, TCS3430_REG_STATUS, int2clr);
		if (ret2 < 0) {
			dev_err(&chip->client->dev,
				"%s: failed 2x, to clear irq %02x\n",
				__func__, int2clr);
		}
		return ret2;
	}
	return ret;

}

static void tcs3430_get_als(struct tcs3430_chip *chip)
{
	u8 *buf = &chip->shadow[TCS3430_REG_CH0DATAL];

	if (chip->rev == 0) {//rev是0x01
		/* extract raw X channel data */
		chip->als_inf.x_raw = le16_to_cpup((const __le16 *)&buf[0]);

		/* extract raw Y channel data */
		buf = &chip->shadow[TCS3430_REG_CH1DATAL];
		chip->als_inf.y_raw = le16_to_cpup((const __le16 *)&buf[0]);

		/* extract raw IR2 channel data */
		buf = &chip->shadow[TCS3430_REG_CH2DATAL];
		chip->als_inf.ir2_raw = le16_to_cpup((const __le16 *)&buf[0]);

		/* extract raw Z/IR1 channel data */
		buf = &chip->shadow[TCS3430_REG_CH3DATAL];
		if (chip->amux == 0) {
			chip->als_inf.z_raw =
				le16_to_cpup((const __le16 *)&buf[0]);
		} else {
			chip->als_inf.ir1_raw =
				le16_to_cpup((const __le16 *)&buf[0]);
		}

	} else {
		/* extract raw Z channel data */
		chip->als_inf.z_raw = le16_to_cpup((const __le16 *)&buf[0]);

		/* extract raw Y channel data */
		buf = &chip->shadow[TCS3430_REG_CH1DATAL];
		chip->als_inf.y_raw = le16_to_cpup((const __le16 *)&buf[0]);

		/* extract raw IR1 channel data */
		buf = &chip->shadow[TCS3430_REG_CH2DATAL];
		chip->als_inf.ir1_raw = le16_to_cpup((const __le16 *)&buf[0]);

		/* extract raw X/IR2 channel data */
		buf = &chip->shadow[TCS3430_REG_CH3DATAL];
		if (chip->amux == 0) {
			chip->als_inf.x_raw =
				le16_to_cpup((const __le16 *)&buf[0]);
		} else {
			chip->als_inf.ir2_raw =
				le16_to_cpup((const __le16 *)&buf[0]);
		}

	}
}

static int tcs3430_read_all(struct tcs3430_chip *chip)
{
	int ret = 0;

	ret = tcs3430_i2c_blk_read(chip, TCS3430_REG_CH0DATAL,
				   &chip->shadow[TCS3430_REG_CH0DATAL], 8);

	return (ret < 0) ? ret : 0;
}

#ifdef CONFIG_SENSORS_TCS3430_WITH_LUX_CCT
static void tcs3430_report_als(struct tcs3430_chip *chip)
{
	if (chip->a_idev) {//如果有此设备的话
		int rc = tcs3430_get_lux(chip);
		if (!rc) {
			int lux = chip->als_inf.lux;
			input_report_abs(chip->a_idev, ABS_MISC, lux);
			input_report_abs(chip->a_idev, ABS_X, chip->als_inf.x_raw);
			input_report_abs(chip->a_idev, ABS_Y, chip->als_inf.y_raw);
			input_report_abs(chip->a_idev, ABS_Z, chip->als_inf.z_raw);
			input_report_abs(chip->a_idev, ABS_PRESSURE, chip->als_inf.ir1_raw);
			input_report_abs(chip->a_idev, ABS_RUDDER, chip->als_inf.ir2_raw);	
			input_report_abs(chip->a_idev, ABS_DISTANCE,chip->als_inf.cct);
			printk("tcs3430_report_als lux:%d,cct:%d \n",lux,chip->als_inf.cct);
			input_sync(chip->a_idev);
		}
	}
}
#endif

static int tcs3430_irq_handler(struct tcs3430_chip *chip)
{
	u8 status;

	/* Read status first and check to see if there
	 * are pending interrupts for this device since
	 * the interrupt line is shared on the host platform.
	 */
	int ret = tcs3430_i2c_read(chip, TCS3430_REG_STATUS,
				   &chip->shadow[TCS3430_REG_STATUS]);//读取0x93寄存器的值 并保留至

	if (ret) {
		ret = 0;
		goto exit_clr;
	}

	status = chip->shadow[TCS3430_REG_STATUS];
	if ((status & (TCS3430_ST_AINT)) == (TCS3430_ST_AINT)) {// TCS3430_ST_AINT： ALS中断。表明设备正在断言ALS中断。写入1将清除此状态标志。
		tcs3430_read_all(chip);  /* read ADC channels 读ADC渠道  从0x94寄存器到0x9B寄存器*/
		tcs3430_get_als(chip);//不用管  反正就是通过算法获得了x,y,z,r1,r2的值
#ifdef CONFIG_SENSORS_TCS3430_WITH_LUX_CCT
		tcs3430_report_als(chip);
#endif
		/* Toggle mode? */  //切换模式
		if (chip->amux_state == TCS3430_X_IR2_TOGGLE)//如果是2的话
			tcs3430_als_mux_toggle(chip);

		tcs3430_irq_clr(chip, TCS3430_CMD_ALS_INT_CLR);//0x10 其实就是  ：写入1将清除此ALS中断状态标志
		ret = 1;
	} else {
		ret = 0;
	}

exit_clr:
	tcs3430_irq_clr(chip, TCS3430_CMD_ALL_INT_CLR);

	return ret;
}

static irqreturn_t tcs3430_irq(int irq, void *handle)
{
	struct tcs3430_chip *chip = handle;
	struct device *dev = &chip->client->dev;
	int ret;

	mutex_lock(&chip->lock);
	if (chip->in_suspend) {
		dev_err(dev, "%s: in suspend\n", __func__);
		chip->irq_pending = 1;
		disable_irq_nosync(chip->client->irq);
		ret = 0;
		goto bypass;
	}
	ret = tcs3430_irq_handler(chip);
bypass:
	mutex_unlock(&chip->lock);
	return ret ? IRQ_HANDLED : IRQ_NONE;
}

static void tcs3430_set_defaults(struct tcs3430_chip *chip)
{
	struct device *dev = &chip->client->dev;
	u8 gain_mask;
/*
	if (chip->pdata) {
		dev_err(dev, "%s: Loading platform data\n", __func__);
		chip->params.als_time_ms = chip->pdata->parameters.als_time_ms;
		chip->params.persist  = chip->pdata->parameters.persist;
		chip->params.als_gain = chip->pdata->parameters.als_gain;
	} else {
*/	
		dev_err(dev, "%s: use defaults\n", __func__);
		chip->params.als_time_ms = TCS3430_DEFAULT_ATIME;//400ms
		chip->params.persist  = TCS3430_DEFAULT_PERSIST;//0  每个ALS周期
		chip->params.als_gain = TCS3430_DEFAULT_AGAIN;//16
//	}

	chip->shadow[TCS3430_REG_ATIME] =  //0x81 
		TCS3430_MS_TO_ATIME(chip->params.als_time_ms);//毫秒转化为atime积分值  //#define TCS3430_MS_TO_ATIME(x) ((uint8_t)((x * 100) / 278))  //这里是400ms所以转化为atime积分值

	chip->shadow[TCS3430_REG_PERS]  = ALS_PERSIST(chip->params.persist);//0x8C //#define ALS_PERSIST(p) (((p) & 0xf) << 0) 为什么要这样子做，因为此寄存器是4位有效数据
	
	gain_mask = tcs3430_gain_to_mask(chip->params.als_gain);//将增益去掩码
	if (gain_mask != 0xFF) {
		chip->shadow[TCS3430_REG_CFG1]  = gain_mask;//CFG1寄存器就是存放gain的地方
	} else {
		dev_err(&chip->client->dev,
			"%s: illegal platform data gain %d\n",
			__func__, chip->params.als_gain);

		/* Set valid default */
		chip->params.als_gain = TCS3430_DEFAULT_AGAIN;
		chip->shadow[TCS3430_REG_CFG1]  = AGAIN_MASK_16;
	}

	tcs3430_flush_regs(chip);//然后将我们的  atime寄存器，CFG1寄存器,以及PERS寄存器赋值

/* ENABLE INTERRUPTS */   //INTENAB 寄存器 赋值  AIEN ：写入'1'到该位会启用ALS中断。
	tcs3430_i2c_write(chip, TCS3430_REG_INTENAB, 0x10);  /* set AIEN */ 
}

static int tcs3430_als_enable(struct tcs3430_chip *chip, int on)
{
	int rc;

	dev_err(&chip->client->dev, "%s: on = %d\n", __func__, on);
	if (on) {
		tcs3430_irq_clr(chip, TCS3430_CMD_ALS_INT_CLR);
		chip->shadow[TCS3430_REG_ENABLE] |=
				(TCS3430_EN_PON | TCS3430_EN_AEN);

		rc = tcs3430_update_enable_reg(chip);
		if (rc)
			return rc;
		mdelay(3);
	} else {
		chip->shadow[TCS3430_REG_ENABLE] &=
			~(TCS3430_EN_AEN);
		rc = tcs3430_update_enable_reg(chip);
		tcs3430_irq_clr(chip, TCS3430_CMD_ALS_INT_CLR);
	}

	chip->als_enabled = on;

	return rc;
}

#ifdef CONFIG_SENSORS_TCS3430_WITH_LUX_CCT
static ssize_t tcs3430_device_als_lux(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct tcs3430_chip *chip = dev_get_drvdata(dev);

	dev_err(&chip->client->dev, "%s\n", __func__);

	mutex_lock(&chip->lock);
	tcs3430_read_all(chip);
	tcs3430_get_als(chip);
	tcs3430_get_lux(chip);
	mutex_unlock(&chip->lock);

	return snprintf(buf, PAGE_SIZE, "%d\n", chip->als_inf.lux);
}
#endif


static ssize_t tcs3430_device_enable_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct tcs3430_chip *chip = dev_get_drvdata(dev);
	return snprintf(buf, PAGE_SIZE, "%d\n", chip->als_enabled);
}

static ssize_t tcs3430_device_enable_store(struct device *dev,
		struct device_attribute *attr,const char *buf, size_t size)
{
    struct tcs3430_chip *chip = dev_get_drvdata(dev);
    int handle, enabled;
                
    if (sscanf(buf, "%d %d\n", &handle, &enabled) != 2)
        return -EINVAL;
                
    tcs3430_als_enable(chip, enabled);
                
    return 1;
}

static ssize_t tcs3430_device_batch_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{	
	return 1;
}
static ssize_t tcs3430_device_batch_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t size)
{

	return 1;

}

static ssize_t tcs3430_als_enable_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct tcs3430_chip *chip = dev_get_drvdata(dev);
	return snprintf(buf, PAGE_SIZE, "%d\n", chip->als_enabled);
}

static ssize_t tcs3430_als_enable_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t size)
{
	struct tcs3430_chip *chip = dev_get_drvdata(dev);
	bool value;

	if (strtobool(buf, &value))
		return -EINVAL;

	if (value)
		tcs3430_als_enable(chip, 1);
	else
		tcs3430_als_enable(chip, 0);

	return size;
}

static ssize_t tcs3430_als_gain_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct tcs3430_chip *chip = dev_get_drvdata(dev);
	return snprintf(buf, PAGE_SIZE, "%d\n",	chip->params.als_gain);
}

static ssize_t tcs3430_als_y_show(struct device *dev,
	struct device_attribute *attr, char *buf)
		{
	struct tcs3430_chip *chip = dev_get_drvdata(dev);
	return snprintf(buf, PAGE_SIZE, "%d\n", chip->als_inf.y_raw);
}

static ssize_t tcs3430_als_z_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct tcs3430_chip *chip = dev_get_drvdata(dev);
	return snprintf(buf, PAGE_SIZE, "%d\n", chip->als_inf.z_raw);
}

static ssize_t tcs3430_als_ir2_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct tcs3430_chip *chip = dev_get_drvdata(dev);
	return snprintf(buf, PAGE_SIZE, "%d\n", chip->als_inf.ir2_raw);
}

static ssize_t tcs3430_als_x_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct tcs3430_chip *chip = dev_get_drvdata(dev);
	return snprintf(buf, PAGE_SIZE, "%d\n", chip->als_inf.x_raw);
}

static ssize_t tcs3430_als_ir1_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct tcs3430_chip *chip = dev_get_drvdata(dev);
	return snprintf(buf, PAGE_SIZE, "%d\n", chip->als_inf.ir1_raw);
}

#ifdef CONFIG_SENSORS_TCS3430_WITH_LUX_CCT
static ssize_t tcs3430_als_cct_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct tcs3430_chip *chip = dev_get_drvdata(dev);
	tcs3430_read_all(chip);
	tcs3430_get_als(chip);
	tcs3430_get_lux(chip);
	return snprintf(buf, PAGE_SIZE, "%d\n", chip->als_inf.cct);
}
#endif

static ssize_t tcs3430_als_gain_store(struct device *dev,
				      struct device_attribute *attr,
				      const char *buf, size_t size)
{
	unsigned long gain;
	int i = 0;
	int rc;
	struct tcs3430_chip *chip = dev_get_drvdata(dev);

	rc = kstrtoul(buf, 10, &gain);

	if (rc)
		return -EINVAL;

	if (gain != 1 && gain != 4 && gain != 16 && gain != 64) {
		dev_err(&chip->client->dev, "%s: illegal als gain %d\n",
			__func__, (int)gain);
		return -EINVAL;
	}

	while (i < sizeof(als_gains)) {
		if (gain == als_gains[i])
			break;
		i++;
	}

	mutex_lock(&chip->lock);
	if (gain)
		rc = tcs3430_set_als_gain(chip, als_gains[i]);

	tcs3430_flush_regs(chip);
	mutex_unlock(&chip->lock);
	return rc ? rc : size;
}

static ssize_t tcs3430_als_persist_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct tcs3430_chip *chip = dev_get_drvdata(dev);
	return snprintf(buf, PAGE_SIZE, "%d\n",
			(((chip->shadow[TCS3430_REG_PERS]) & 0x0f)));
}

static ssize_t tcs3430_als_persist_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t size)
{
	long persist;	int rc;
	struct tcs3430_chip *chip = dev_get_drvdata(dev);

	rc = kstrtoul(buf, 10, &persist);
	if (rc)
		return -EINVAL;

	mutex_lock(&chip->lock);
	chip->shadow[TCS3430_REG_PERS] &= 0xF0;
	chip->shadow[TCS3430_REG_PERS] |= ((u8)persist & 0x0F);

	tcs3430_flush_regs(chip);

	mutex_unlock(&chip->lock);
	return size;
}

static ssize_t tcs3430_als_atime_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct tcs3430_chip *chip = dev_get_drvdata(dev);
	int t;
	t = TCS3430_ATIME_TO_MS(chip->shadow[TCS3430_REG_ATIME]);
	return snprintf(buf, PAGE_SIZE, "%d (in ms)\n", t);
}

static ssize_t tcs3430_als_atime_store(struct device *dev,
				       struct device_attribute *attr,
				       const char *buf, size_t size)
{
	long milliseconds;
	int rc;
	uint8_t atime;
	struct tcs3430_chip *chip = dev_get_drvdata(dev);

	rc = kstrtoul(buf, 10, &milliseconds);
	if (rc)
		return -EINVAL;

	atime = TCS3430_MS_TO_ATIME(milliseconds);
	mutex_lock(&chip->lock);
	chip->shadow[TCS3430_REG_ATIME] = atime;
	tcs3430_flush_regs(chip);
	mutex_unlock(&chip->lock);
	return size;
}

static ssize_t tcs3430_als_mux_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct tcs3430_chip *chip = dev_get_drvdata(dev);
	return snprintf(buf, PAGE_SIZE,
			"%d\n", chip->amux_state);
}

static ssize_t tcs3430_als_mux_store(struct device *dev,
				     struct device_attribute *attr,
				     const char *buf, size_t size)
{
	unsigned long mux_op;
	int rc;
	u8 cfg1;
	struct tcs3430_chip *chip = dev_get_drvdata(dev);

	rc = kstrtoul(buf, 10, &mux_op);
	if (rc || mux_op > 2)
		return -EINVAL;
	mutex_lock(&chip->lock);
	cfg1 = chip->shadow[TCS3430_REG_CFG1];
	chip->amux_state = mux_op;
	if (mux_op > 0) {
		chip->amux = 0xff;

		/* IR2 Mode
		   If mux_op is greater than 0, then
		   the mode is either IR2 only or toggle.
		   Only IR2 mode or X mode will cause a write to
		   CFG1:amux in this function.  If toggle
		   mode, the CFG1:amux bit is toggled in the
		   IRQ handler function after the channel data
		   has been read and stored in the appropriate
		   tcs3430_als_info field.
		*/
		if (mux_op == TCS3430_IR2_ONLY)
			cfg1 |= TCS3430_AMUX_IR2_DATA;
	} else {
		chip->amux = 0x00;
		/* X mode */
		cfg1 &= ~TCS3430_AMUX_IR2_DATA;
		cfg1 |= TCS3430_AMUX_X_DATA;
	}
	chip->shadow[TCS3430_REG_CFG1] = cfg1;
	tcs3430_flush_regs(chip);
	mutex_unlock(&chip->lock);
	return size;
}



static int tcs3430_als_mux_toggle(struct tcs3430_chip *chip)
{
	u8 cfg1;
	int ret;

	cfg1 = chip->shadow[TCS3430_REG_CFG1];

	if (cfg1 & TCS3430_AMUX_IR2_DATA) {  //ALS多路复用器。设置CH3输入。默认值= 0 (X通道)。设置为1读取IR2
		cfg1 &= ~TCS3430_AMUX_IR2_DATA;  /* clear */
		cfg1 |= TCS3430_AMUX_X_DATA; //切换到x通道
	} else {
		cfg1 &= ~TCS3430_AMUX_IR2_DATA;
		cfg1 |= TCS3430_AMUX_IR2_DATA;//切换到ir2通道
	}

	chip->shadow[TCS3430_REG_CFG1] = cfg1;
	ret = tcs3430_i2c_write(chip, TCS3430_REG_CFG1,
				chip->shadow[TCS3430_REG_CFG1]);

	if (ret >= 0)
		chip->amux = ~chip->amux;

	return (ret < 0) ? ret : 0;
}
/*
static struct device_attribute als_attrs[] = {
	__ATTR(als_atime, 0666, tcs3430_als_atime_show,
		tcs3430_als_atime_store),
	__ATTR(als_x, 0666, tcs3430_als_x_show, NULL),
	__ATTR(als_y, 0666, tcs3430_als_y_show, NULL),
	__ATTR(als_z, 0666, tcs3430_als_z_show, NULL),
	__ATTR(als_ir1, 0666, tcs3430_als_ir1_show, NULL),
	__ATTR(als_ir2, 0666, tcs3430_als_ir2_show, NULL),
	__ATTR(als_gain, 0666, tcs3430_als_gain_show,
			tcs3430_als_gain_store),
	__ATTR(als_power_state, 0666, tcs3430_als_enable_show,
			tcs3430_als_enable_store),
	__ATTR(als_persist, 0666, tcs3430_als_persist_show,
			tcs3430_als_persist_store),
	__ATTR(als_mux_op, 0666, tcs3430_als_mux_show,
			tcs3430_als_mux_store),
#ifdef CONFIG_SENSORS_TCS3430_WITH_LUX_CCT
	__ATTR(als_lux, 0666, tcs3430_device_als_lux, NULL),
	__ATTR(als_cct, 0666, tcs3430_als_cct_show, NULL),
#endif
};
*/
static DEVICE_ATTR(als_atime, 0664, tcs3430_als_atime_show,tcs3430_als_atime_store);
static DEVICE_ATTR(als_x, 0664, tcs3430_als_x_show, NULL);
static DEVICE_ATTR(als_y, 0664, tcs3430_als_y_show, NULL);
static DEVICE_ATTR(als_z, 0664, tcs3430_als_z_show, NULL);
static DEVICE_ATTR(als_ir1, 0664, tcs3430_als_ir1_show, NULL);
static DEVICE_ATTR(als_ir2, 0664, tcs3430_als_ir2_show, NULL);
static DEVICE_ATTR(als_gain, 0664, tcs3430_als_gain_show,tcs3430_als_gain_store);
static DEVICE_ATTR(als_power_state, 0664, tcs3430_als_enable_show,tcs3430_als_enable_store);
static DEVICE_ATTR(als_persist, 0664, tcs3430_als_persist_show,tcs3430_als_persist_store);
static DEVICE_ATTR(als_mux_op, 0664, tcs3430_als_mux_show,tcs3430_als_mux_store);
static DEVICE_ATTR(als_lux, 0664, tcs3430_device_als_lux, NULL);
static DEVICE_ATTR(als_cct, 0664, tcs3430_als_cct_show, NULL);
static DEVICE_ATTR(batch, 0664, tcs3430_device_batch_show, tcs3430_device_batch_store);
static DEVICE_ATTR(enable, 0664, tcs3430_device_enable_show, tcs3430_device_enable_store);

static struct attribute *tcs3430_attributes[] = {
	&dev_attr_als_atime.attr,
	&dev_attr_als_x.attr,
	&dev_attr_als_y.attr,
	&dev_attr_als_z.attr,
	&dev_attr_als_ir1.attr,
	&dev_attr_als_ir2.attr,
	&dev_attr_als_gain.attr,
	&dev_attr_als_power_state.attr,
	&dev_attr_als_persist.attr,
	&dev_attr_als_mux_op.attr,
	&dev_attr_als_lux.attr,
	&dev_attr_als_cct.attr,
	&dev_attr_batch.attr,
	&dev_attr_enable.attr,
	NULL
};



static const struct attribute_group tcs3430_attr_group = {
	.attrs = tcs3430_attributes,
};
/*
static int tcs3430_add_sysfs_interfaces(struct device *dev,
	struct device_attribute *a, int size)
{
	int i;
	for (i = 0; i < size; i++)
		if (device_create_file(dev, a + i))
			goto undo;
	return 0;
undo:
	for (; i >= 0 ; i--)
		device_remove_file(dev, a + i);
	dev_err(dev, "%s: failed to create sysfs interface\n", __func__);
	return -ENODEV;
}
*/

/*
static void tcs3430_remove_sysfs_interfaces(struct device *dev,
	struct device_attribute *a, int size)
{
	int i;
	for (i = 0; i < size; i++)
		device_remove_file(dev, a + i);
}
*/

static int tcs3430_get_id(struct tcs3430_chip *chip, u8 *id, u8 *rev)
{
	u8 revid_raw;
	tcs3430_i2c_read(chip, TCS3430_REG_REVID, &revid_raw);
	*rev = revid_raw & 0x07;    /* Rev is in bits 2:0 */
	return tcs3430_i2c_read(chip, TCS3430_REG_ID, id);
}

static int tcs3430_power_on(struct tcs3430_chip *chip)
{
	int rc;
		printk("wbw %s: calledLINE=%d \n", __func__,__LINE__);
	rc = tcs3430_pltf_power_on(chip);
	if (rc)
		return rc;
	dev_err(&chip->client->dev, "%s: chip was off, restoring regs\n",
			__func__);
	return tcs3430_flush_regs(chip);
}

 static int tcs3430_als_idev_open(struct input_dev *idev)
{
	struct tcs3430_chip *chip = dev_get_drvdata(&idev->dev);
	int rc;
	bool als = chip->a_idev && chip->a_idev->users;
	printk("wbw %s: calledLINE=%d \n", __func__,__LINE__);

	dev_err(&idev->dev, "%s\n", __func__);
	mutex_lock(&chip->lock);
	if (chip->unpowered) {
		rc = tcs3430_power_on(chip);
		if (rc)
			goto chip_on_err;
	}
	rc = tcs3430_als_enable(chip, 1);//关键在此处
	if (rc && !als)
		tcs3430_pltf_power_off(chip);
chip_on_err:
	mutex_unlock(&chip->lock);
	return rc;
}

static void tcs3430_als_idev_close(struct input_dev *idev)
{
	struct tcs3430_chip *chip = dev_get_drvdata(&idev->dev);

	dev_err(&idev->dev, "%s\n", __func__);
	mutex_lock(&chip->lock);
	tcs3430_als_enable(chip, 0);
	mutex_unlock(&chip->lock);
}

static int tcs3430_probe(struct i2c_client *client,
				   const struct i2c_device_id *idp)
{
	int i, ret;
	u8 id, rev;
//	bool powered = false;
	unsigned int irq_gpio;
	
	
	struct device *dev = &client->dev;
	static struct tcs3430_chip *chip;
	//struct ams_tcs3430_platform_data *pdata = dev->platform_data;
//	struct ams_tcs3430_platform_data *pdata ;
	printk("wbw tcs3430_probe I2C Address: 0x%02x\n", client->addr);
	printk("wbw %s: calledLINE=%d \n", __func__,__LINE__);
	//dev_err(dev, "%s: platformALS name  = %s\n",
	//	 __func__, pdata->als_name);

	printk("wbw %s: calledLINE=%d \n", __func__,__LINE__);
	printk("wbw %s: calledLINE=%d \n", __func__,__LINE__);
	printk("wbw %s: calledLINE=%d \n", __func__,__LINE__);



	if (!client || !client->dev.of_node) { //判断是否有此设备树的结点
		pr_err("wbw invalid input\n");
		return -EINVAL;
	}
	printk("wbw %s: calledLINE=%d \n", __func__,__LINE__);

    if(!i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_BYTE_DATA)) {  //检查 adapter 是否支持某个i2c协议的传输
        dev_err(&client->dev, "wbw Failed check I2C functionality");
        return -ENODEV;
    }	
	printk("wbw %s: calledLINE=%d \n", __func__,__LINE__);

/*
	pdata = devm_kzalloc(&client->dev,sizeof (*pdata), GFP_KERNEL);
	if (!pdata)
	return -ENOMEM;
	pdata->i2c_client=client;
	pdata->dev = &client->dev;
*/
	
/*
	dev_err(dev, "%s: client->irq = %d\n", __func__, client->irq);

	if (!pdata) {
		dev_err(dev, "%s: platform data required\n", __func__);
		ret = -EINVAL;
		goto init_failed;
	}

	if (!(pdata->als_name) || client->irq < 0) {
		dev_err(dev, "%s: no reason to run.\n", __func__);
		ret = -EINVAL;
		goto init_failed;
	}

	if (pdata->platform_init) {
		ret = pdata->platform_init();
		if (ret)
			goto init_failed;
	}

	if (pdata->platform_power) {
		ret = pdata->platform_power(dev, POWER_ON);
		if (ret) {
			dev_err(dev, "%s: pltf power on failed\n", __func__);
			goto pon_failed;
		}
		powered = true;
		mdelay(10);
	}
*/
	printk("wbw %s: calledLINE=%d \n", __func__,__LINE__);

	chip = kzalloc(sizeof(struct tcs3430_chip), GFP_KERNEL);
	if (!chip) {
		ret = -ENOMEM;
		goto malloc_failed;
	}
	chip->client = client;
	//printk("wbw %s: calledLINE=%d \n", __func__,__LINE__);

//	pdata = dev->platform_data;
//	printk("wbw %s: calledLINE=%d \n", __func__,__LINE__);	
//	chip->pdata = pdata;
	
	//printk("wbw %s: calledLINE=%d \n", __func__,__LINE__);
	i2c_set_clientdata(client, chip);
	//printk("wbw %s: calledLINE=%d \n", __func__,__LINE__);

//读id
	ret = tcs3430_get_id(chip, &id, &rev);          
	chip->rev = rev;
	dev_err(dev, "%s: device id:%02x device rev:%02x\n",
		 __func__, id, rev);// tcs3430_probe: device id:dc device rev:01
	tcs_info("device id:%02x device rev:%02x\n",id,rev);
	for (i = 0; i < ARRAY_SIZE(tcs3430_ids); i++) {
		if (id == tcs3430_ids[i])
			break;
	}
	if (i < ARRAY_SIZE(tcs3430_names)) {
		dev_err(dev, "%s: '%s rev. %d' detected\n", __func__,
			 tcs3430_names[i], rev);
		chip->device_index = i;
	} else {
		dev_err(dev, "%s: not supported chip id\n", __func__);
		ret = -EOPNOTSUPP;
		goto id_failed;
	}
//end	
	
	
	mutex_init(&chip->lock);//初始化自旋锁
	tcs3430_set_defaults(chip);//设置 tcs3430 设备给寄存器设置默认值  并开启ALS中断
	ret = tcs3430_flush_regs(chip);//然后将一些默认值  发送给此设备
	if (ret)
		goto flush_regs_failed;

//	if (pdata->platform_power) {
//		pdata->platform_power(dev, POWER_OFF);
//		powered = false;
//		chip->unpowered = true;
//	}

//	if (!pdata->als_name)
//		goto input_a_alloc_failed;

//下面是创建Input设备
	chip->a_idev = input_allocate_device();//创建input设备
	if (!chip->a_idev) {
	//	dev_err(dev, "%s: no memory for input_dev '%s'\n",
	//		__func__, pdata->als_name);
		ret = -ENODEV;
		goto input_a_alloc_failed;
	}
	//printk("wbw %s: calledLINE=%d \n", __func__,__LINE__);
	chip->a_idev->name = "tcs3430";
	chip->a_idev->id.bustype = BUS_I2C;
	set_bit(EV_ABS, chip->a_idev->evbit);//绝对坐标事件
	
	set_bit(ABS_X, chip->a_idev->absbit);//杂项事件
	set_bit(ABS_Y, chip->a_idev->absbit);//杂项事件	
	set_bit(ABS_Z, chip->a_idev->absbit);//杂项事件

	set_bit(ABS_MISC, chip->a_idev->absbit);//杂项事件
	set_bit(ABS_DISTANCE, chip->a_idev->absbit);//杂项事件
	
	input_set_abs_params(chip->a_idev, ABS_MISC, 0, 65535, 0, 0);//通常在probe函数中会调用 input_set_abs_params 给设备的input_dev结构体初始化    //参数 chip->a_idev 设备ABS_MISC 范围是0到 +65535，数据误差是0到0，中心平滑位置是0
	input_set_abs_params(chip->a_idev, ABS_DISTANCE, 0, 65535, 0, 0);
	input_set_abs_params(chip->a_idev, ABS_X, 0, 65535, 0, 0);	
	input_set_abs_params(chip->a_idev, ABS_Y, 0, 65535, 0, 0);
	input_set_abs_params(chip->a_idev, ABS_Z, 0, 65535, 0, 0);
	input_set_abs_params(chip->a_idev, ABS_RUDDER, 0, 65535, 0, 0);
	input_set_abs_params(chip->a_idev, ABS_PRESSURE, 0, 65535, 0, 0);	

	chip->a_idev->open = tcs3430_als_idev_open;
	chip->a_idev->close = tcs3430_als_idev_close;
	dev_set_drvdata(&chip->a_idev->dev, chip);
	ret = input_register_device(chip->a_idev); //创建Input设备
	if (ret) {
		input_free_device(chip->a_idev);
//		dev_err(dev, "%s: cant register input '%s'\n",
//			__func__, pdata->als_name);
		goto input_a_alloc_failed;
	}
//end


	
//	ret = tcs3430_add_sysfs_interfaces(&chip->a_idev->dev,
//					   als_attrs, ARRAY_SIZE(als_attrs));
	ret=sysfs_create_group(&client->dev.kobj, &tcs3430_attr_group);
	printk("wbw %s: calledLINE=%d \n", __func__,__LINE__);
	if (ret)
		goto input_a_sysfs_failed;

	
//获得gpio以及irq并申请中断	
 	irq_gpio = of_get_named_gpio(client->dev.of_node, "ctsintr-gpio", 0);
 	if (irq_gpio < 0) {
		dev_err(dev,"invalid irq-gpio in dt: %d",irq_gpio);
		return -EINVAL;
	}   
	 printk("of_get_named_gpio=%d \n",irq_gpio);
	 client->irq = gpio_to_irq(irq_gpio);
	 printk("of_get_named_gpio=%d \n",client->irq);
	 
	printk("%s: calledLINE=%d \n", __func__,__LINE__);
	
	ret = request_threaded_irq(client->irq, NULL, &tcs3430_irq,
				   IRQF_TRIGGER_FALLING | IRQF_SHARED |IRQF_ONESHOT,
				   dev_name(dev), chip);
	if (ret) {
		dev_err(dev, "Failed to request irq %d\n", client->irq);
		goto irq_register_fail;
	}
//end
	

	// Power ON。这个场激活内部振荡器来允许定时器和ADC通道操作。写一个1就会激活振荡器。写入0将禁用振荡器。
	//	ALS启用。激活ALS功能。在同一个命令中设置aen=1和pon=1以确保 autozero 函数在第一次测量之前运行。
	chip->shadow[TCS3430_REG_ENABLE] =//0x80寄存器设置PON和AEN  
		(TCS3430_EN_PON | TCS3430_EN_AEN);
	tcs3430_update_enable_reg(chip);//TCS3430_REG_ENABLE   就是将PON和AEN  通过i2c发送出去
	
	
	chip->amux_state = TCS3430_X_IR2_TOGGLE;//2
	chip->amux = 0x00;
	dev_err(dev, "Probe ok.\n");
	return 0;

irq_register_fail:
//	if (chip->a_idev) {
//		tcs3430_remove_sysfs_interfaces(&chip->a_idev->dev,
//						als_attrs,
//						ARRAY_SIZE(als_attrs));
input_a_sysfs_failed:
		input_unregister_device(chip->a_idev);
//	}
input_a_alloc_failed:
flush_regs_failed:
id_failed:
	i2c_set_clientdata(client, NULL);
	kfree(chip);
malloc_failed:
//	if (powered && pdata->platform_power)
//		pdata->platform_power(dev, POWER_OFF);
//pon_failed:
//	if (pdata->platform_teardown)
//		pdata->platform_teardown(dev);
//init_failed:
	dev_err(dev, "Probe failed.\n");
	return ret;
}

/*
static int tcs3430_suspend(struct device *dev)
{
	struct tcs3430_chip *chip = dev_get_drvdata(dev);
	struct ams_tcs3430_platform_data *pdata = dev->platform_data;
	dev_err(dev, "%s\n", __func__);
	mutex_lock(&chip->lock);
	chip->in_suspend = 1;
	printk("wbw %s: calledLINE=%d \n", __func__,__LINE__);

	// Blocks if work is active 
	if (chip->a_idev && chip->a_idev->users) {
		if (pdata->als_can_wake) {
			dev_err(dev, "set wake on als\n");
			chip->wake_irq = 1;
		} else {
			dev_err(dev, "als off\n");
			tcs3430_als_enable(chip, 0);
		}
	}
	if (chip->wake_irq) {
		irq_set_irq_wake(chip->client->irq, 1);
	} else if (!chip->unpowered) {
		dev_err(dev, "powering off\n");
		tcs3430_pltf_power_off(chip);
	}
	mutex_unlock(&chip->lock);

	return 0;
}

static int tcs3430_resume(struct device *dev)
{

	struct tcs3430_chip *chip = dev_get_drvdata(dev);
	bool als_on;
	int rc = 0;
	mutex_lock(&chip->lock);
	als_on = chip->a_idev && chip->a_idev->users;
	chip->in_suspend = 0;
	printk("wbw %s: calledLINE=%d \n", __func__,__LINE__);

	dev_err(dev, "%s: powerd %d, als: needed %d  enabled %d",
			__func__, !chip->unpowered, als_on,
			chip->als_enabled);

	if (chip->wake_irq) {
		irq_set_irq_wake(chip->client->irq, 0);
		chip->wake_irq = 0;
	}
	if (chip->unpowered && (als_on)) {
		dev_err(dev, "powering on\n");
		rc = tcs3430_power_on(chip);
		if (rc)
			goto err_power;
	}

	if (als_on && !chip->als_enabled)
		(void)tcs3430_als_enable(chip, 1);
	if (chip->irq_pending) {
		dev_err(dev, "%s: pending interrupt\n", __func__);
		chip->irq_pending = 0;
		(void)tcs3430_irq_handler(chip);
		enable_irq(chip->client->irq);
	}
err_power:
	mutex_unlock(&chip->lock);

	return 0;
}
*/
static int  tcs3430_remove(struct i2c_client *client)
{
	struct tcs3430_chip *chip = i2c_get_clientdata(client);	printk("wbw %s: calledLINE=%d \n", __func__,__LINE__);
	free_irq(client->irq, chip);
	if (chip->a_idev) {
	//	tcs3430_remove_sysfs_interfaces(&chip->a_idev->dev,
	//		als_attrs, ARRAY_SIZE(als_attrs));
		input_unregister_device(chip->a_idev);
	}

//	if (chip->pdata->platform_teardown)
//		chip->pdata->platform_teardown(&client->dev);
	i2c_set_clientdata(client, NULL);
	kfree(chip);
	return 0;
}

static struct i2c_device_id tcs3430_idtable[] = {
	{ "tcs3430", 0 },
	{}
};

static const struct of_device_id tcs3430_of_id_table[] = {
	{ .compatible = "rgb,cts3430" },	
	{},
};

MODULE_DEVICE_TABLE(i2c, tcs3430_idtable);
/*
static const struct dev_pm_ops tcs3430_pm_ops = {
	.suspend = tcs3430_suspend,
	.resume  = tcs3430_resume,
};
*/
static struct i2c_driver tcs3430_driver = {
	.driver = {
		.name = "tcs3430",
		//.pm = &tcs3430_pm_ops,
		.owner    = THIS_MODULE,
		.of_match_table = tcs3430_of_id_table,
	},
	.id_table = tcs3430_idtable,
	.probe = tcs3430_probe,
	.remove = tcs3430_remove,
};

static int __init tcs3430_init(void)
{
	return i2c_add_driver(&tcs3430_driver);
}

static void __exit tcs3430_exit(void)
{
	i2c_del_driver(&tcs3430_driver);
}

module_init(tcs3430_init);
module_exit(tcs3430_exit);

MODULE_AUTHOR("AMS AOS Software<cs.americas@ams.com>");
MODULE_DESCRIPTION("AMS TCS3430 ambient light and color (XYZ) sensor");
MODULE_LICENSE("Dual MIT/GPL");
