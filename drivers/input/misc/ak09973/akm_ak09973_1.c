// Driver for AK09973
//
// SPDX-License-Identifier: GPL-2.0-only

#include <linux/delay.h>
#include <linux/hrtimer.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of_gpio.h>
#include <linux/slab.h>
#include <linux/types.h>
#include "akm_ak09973.h"

#define strict_strtol       kstrtol

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif
#define HWLOG_TAG AKM_AK09973_2
//HWLOG_REGIST();

#define AK09973_1_SYSCLS_NAME "stepmotor_hall_1"
#define AK09973_1_SYSDEV_NAME "ak09973_1_dev"
#define AK09973_1_FST_CHECK_VAR_UPPER_LIMIT
#define AK09973_1_RESET_DELAY_MS 1
#define AK09973_1_DRDY_DELAY_MS 2
#define AK09973_1_DRDY_PIN_CONFIG 0x0001
#define AK09973_1_SW_FUNC_CONFIG 0x0000
#define AK09973_1_ERRXY_CONFIG 0x0080
#define AK09973_1_ERRADC_CONFIG 0x0100
#define AK09973_1_INT_CONFIG 0x0000
#define AK09973_1_ODINT_CONFIG 0x0000
/*
 * Expected standard deviation of AK09973_1 data in uT unit: 0uTrms ~ 50uTrms,
 * including the environment mag disturbance.
 * Variance of AK09973_1 data in uT unit : 0(uTrms^2) ~ 2500(uTrms^2)
 * Varianc raw register value (0 / res^2) ~ (2500 /res^2)
 */
#ifdef AK09973_1_WIDE_RANGE
	#define AK09973_1_FST_VAR_LIMIT_HIGH 260 // (50^2)/(3.1^2)
	#define AK09973_1_FST_VAR_LIMIT_LOW 0
#else
	#define AK09973_1_FST_VAR_LIMIT_HIGH 2066 // (50^2)/(1.1^2)
	#define AK09973_1_FST_VAR_LIMIT_LOW 0
#endif

#define AK09973_1_WIA_VALUE 0xC148
#define AK09973_1_WIA_SIZE 2
#define AK09973_1_ST1_SIZE 2
#define AK09973_1_BDATA_SIZE 7
#define AK09973_1_SOFT_RESET_VALUE 0x01
#define AK09973_1_ST1_IS_DRDY_VALUE 0xFF81
#define AK09973_1_ST1_NO_DRDY_VALUE 0xFF80
#define AK09973_1_FST_DATA_BUF_SIZE 100
#define AK09973_1_FST_DRDY_RETRY_MAX 10
#define AKM_BASE_NUM 10
/*
 * AK09973_1_REG_SDR_VALUE (spec 12.3.6)
 * 0x00 : Low noise drive
 * 0x01 : Low power drive
 */
#define AK09973_1_REG_SDR_VALUE 0x00
/*
 * AK09973_1_REG_SMR_VALUE (spec 12.3.6)
 * 0x00 : High sensitivity
 * 0x20 : Wide measurement range
 */
#ifdef AK09973_1_WIDE_RANGE
	#define AK09973_1_REG_SMR_VALUE 0x20
	//#define AK09973_1_SCALE_AMPLIFY_Q16 203161 // 3.1uT/LSB in Q16
	#define AK09973_1_SCALE_AMPLIFY_Q16 1 // 3.1uT/LSB in Q16
	#define AK09973_1_SCALE_DATA_RANGE 10570
#else
	#define AK09973_1_REG_SMR_VALUE 0x00
	//#define AK09973_1_SCALE_AMPLIFY_Q16 72090 // 1.1uT/LSB in Q16
	#define AK09973_1_SCALE_AMPLIFY_Q16 1 // 1.1uT/LSB in Q16
	#define AK09973_1_SCALE_DATA_RANGE 29788
#endif
#define AK09973_1_REG_WIA_ADDR 0x00
#define AK09973_1_REG_CNTL1_ADDR 0x20
#define AK09973_1_REG_CNTL2_ADDR 0x21
#define AK09973_1_REG_SRST_ADDR 0x30
#define AK09973_1_REG_ST1_ADDR 0x10
#define AK09973_1_REG_ALL_AXES_ADDR 0x17

#define AK09973_1_MODE_POWERDOWN_MODE 0x00
#define AK09973_1_MODE_SNG_MEASURE_MODE 0x01
#define AK09973_1_MODE_CNT_MEASURE_MODE1 0x02 // 0.25Hz
#define AK09973_1_MODE_CNT_MEASURE_MODE2 0x04 // 0.5Hz
#define AK09973_1_MODE_CNT_MEASURE_MODE3 0x06 // 1Hz
#define AK09973_1_MODE_CNT_MEASURE_MODE4 0x08 // 10Hz
#define AK09973_1_MODE_CNT_MEASURE_MODE5 0x0A // 20Hz
#define AK09973_1_MODE_CNT_MEASURE_MODE6 0x0C // 50Hz
#define AK09973_1_MODE_CNT_MEASURE_MODE7 0x0E // 100Hz

#define AK09973_1_ODR_10HZ 10
#define AK09973_1_ODR_20HZ 20
#define AK09973_1_ODR_50HZ 50
#define AK09973_1_ODR_100HZ 100
#define AK09973_1_DELAY_025HZ 4000000000LL
#define AK09973_1_DELAY_05HZ 2000000000LL
#define AK09973_1_DELAY_1HZ 1000000000LL
#define AK09973_1_DELAY_10HZ 100000000LL
#define AK09973_1_DELAY_20HZ 50000000LL
#define AK09973_1_DELAY_50HZ 20000000LL
#define AK09973_1_DELAY_100HZ 10000000LL
#define AK09973_1_MIN_DELAY_NS AK09973_1_DELAY_100HZ // 10ms, 100Hz
#define AK09973_1_MAX_DELAY_NS AK09973_1_DELAY_025HZ // 4s, 0.25Hz
#define AK09973_1_DEFAULT_DELAY_NS AK09973_1_DELAY_100HZ // 10ms

#define POS_HALL_NV_NUM 415
#define POS_HALL_NV_SIZE 24
#define POS_HALL_NV_NAME "MOTORPOS"
#define NV_READ_TAG 1
#define NV_WRITE_TAG 0
#define NV_WRITE_TIME_COST_MS 300

#define AXIS_COUNT 3
#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2

#define AK09973_1_RETCODE_SUCCESS 0
#define AK09973_1_RETCODE_COMMON_ERR (-1)
#define AK09973_1_RETCODE_FST_FAIL (-2)
#define AK09973_1_RETCODE_I2C_OP_FAIL (-3)
#define AK09973_1_RETCODE_RESET_FAIL (-4)
#define AK09973_1_RETCODE_ID_CHECK_FAIL (-5)
#define AK09973_1_RETCODE_FST_SET_MODE_FAIL (-6)
#define AK09973_1_RETCODE_FST_IS_DRDY_FAIL (-7)
#define AK09973_1_RETCODE_FST_NO_DRDY_FAILED (-8)
#define AK09973_1_RETCODE_FST_VAR_L_FAIL (-9)
#define AK09973_1_RETCODE_FST_VAR_H_FAILED (-10)
#define AK09973_1_RETCODE_NULL_POINT_ERR (-11)

#define AK09973_1_CNTL1_CONFIG (AK09973_1_DRDY_PIN_CONFIG | \
	AK09973_1_SW_FUNC_CONFIG | AK09973_1_ERRXY_CONFIG | \
	AK09973_1_ERRADC_CONFIG | AK09973_1_INT_CONFIG | AK09973_1_ODINT_CONFIG)
#define ak09973_1_adc_overflow(st1) ((0x0100 & (st1)) >> 8)
#define ak09973_1_set_sdr(ctrl2) (AK09973_1_REG_SDR_VALUE | (ctrl2))
#define ak09973_1_set_smr(ctrl2) (AK09973_1_REG_SMR_VALUE | (ctrl2))
#define make_u16(u8h, u8l) \
	((uint16_t)(((uint16_t)(u8h) << 8) | (uint16_t)(u8l)))
#define make_s16(u8h, u8l) \
	((int16_t)(((uint16_t)(u8h) << 8) | (uint16_t)(u8l)))

#define AK09973_1_FST_STEP_1_1 0x0101
#define AK09973_1_FST_STEP_1_2 0x0102
#define AK09973_1_FST_STEP_1_3 0x0103
#define AK09973_1_FST_STEP_1_4 0x0104
#define AK09973_1_FST_STEP_1_5 0x0105
#define AK09973_1_FST_STEP_2_1 0x0201
#define AK09973_1_FST_STEP_2_2 0x0202
#define AK09973_1_FST_STEP_2_3 0x0203
#define AK09973_1_FST_STEP_2_4 0x0204
#define AK09973_1_FST_STEP_2_5 0x0205
#define AK09973_1_FST_STEP_2_6 0x0206
#define AK09973_1_FST_STEP_2_7 0x0207
#define AK09973_1_FST_STEP_2_8 0x0208
#define AK09973_1_FST_STEP_2_9 0x0209
#define AK09973_1_FST_STEP_2_A 0x020A

struct type_nv_data {
	int close_x;
	int close_y;
	int close_z;
	int open_x;
	int open_y;
	int open_z;
};

struct type_device_data {
	struct i2c_client *i2c;
	struct class *dev_class;
	struct device *class_dev;
	struct mutex op_mutex;
	struct mutex sensor_mutex;
	struct mutex val_mutex;
	struct mutex nv_mutex;
	uint32_t odr; // Hz
	bool enable_flag;
	int64_t ns_delay; // ns
	bool data_refresh_flag;
	bool nvwrite_success_flag;
	bool nvwrite_finish_flag;
	struct type_nv_data written_calibrate_data;
	int32_t sensor_data[AXIS_COUNT]; // 0,1,2 refer to x,y,z
	struct hrtimer poll_timer;
	struct workqueue_struct *work_queue;
	struct workqueue_struct *nvwrite_queue;
	struct delayed_work dwork;
//	struct delayed_work nvwrite_work;
	wait_queue_head_t read_waitq;
//	wait_queue_head_t nvwrite_waitq;
	int16_t id;

};

struct type_mag_data {
	int16_t x;
	int16_t y;
	int16_t z;
};

static struct type_device_data *ak09973_1_dev_data = NULL;


/* select mode by delay */
static uint8_t ak09973_1_select_mode(int64_t delay)
{
	uint8_t mode;

	if (delay >= AK09973_1_DELAY_025HZ)
		mode = AK09973_1_MODE_CNT_MEASURE_MODE1;
	else if (delay >= AK09973_1_DELAY_05HZ)
		mode = AK09973_1_MODE_CNT_MEASURE_MODE2;
	else if (delay >= AK09973_1_DELAY_1HZ)
		mode = AK09973_1_MODE_CNT_MEASURE_MODE3;
	else if (delay >= AK09973_1_DELAY_10HZ)
		mode = AK09973_1_MODE_CNT_MEASURE_MODE4;
	else if (delay >= AK09973_1_DELAY_20HZ)
		mode = AK09973_1_MODE_CNT_MEASURE_MODE5;
	else if (delay >= AK09973_1_DELAY_50HZ)
		mode = AK09973_1_MODE_CNT_MEASURE_MODE6;
	else
		mode = AK09973_1_MODE_CNT_MEASURE_MODE7;

	return mode;
}

static int16_t ak09973_1_set_mode(const struct type_device_data *akm_ak09973_1,
	const uint8_t mode)
{
	uint8_t i2c_data;
	uint16_t ctrl1_data = AK09973_1_CNTL1_CONFIG;
	struct i2c_client *client = NULL;
	int ret;
	uint8_t reg_wia[AK09973_1_WIA_SIZE];
	int i = 0;

	if (akm_ak09973_1 == NULL) {
		return AK09973_1_RETCODE_NULL_POINT_ERR;
	}

	client = akm_ak09973_1->i2c;
	if (client == NULL) {
		return AK09973_1_RETCODE_NULL_POINT_ERR;
	}

	while(i<5) {
		ret = i2c_smbus_read_i2c_block_data(client, AK09973_1_REG_WIA_ADDR, AK09973_1_WIA_SIZE, reg_wia);
		if (ret >= 0)
		break;
		i++;
	}

	ret = i2c_smbus_write_i2c_block_data(client, AK09973_1_REG_CNTL1_ADDR, 2, (uint8_t *)&ctrl1_data);
	if (ret < 0) {
		return ret;
	}

	i2c_data = mode;
	if (mode != AK09973_1_MODE_POWERDOWN_MODE) {
		i2c_data = ak09973_1_set_sdr(i2c_data);
		i2c_data = ak09973_1_set_smr(i2c_data);
	}

	ret = i2c_smbus_write_i2c_block_data(client, AK09973_1_REG_CNTL2_ADDR,
		1, &i2c_data);
	if (ret < 0) {
		return ret;
	}

	return AK09973_1_RETCODE_SUCCESS;
}

static int64_t ak09973_1_get_poll_delay(struct type_device_data *akm_ak09973_1)
{
	int64_t delay;

	if (akm_ak09973_1 == NULL) {
		return AK09973_1_RETCODE_NULL_POINT_ERR;
	}

	delay = akm_ak09973_1->ns_delay;
	if (delay < AK09973_1_MIN_DELAY_NS)
		delay = AK09973_1_MIN_DELAY_NS;
	else if (delay > AK09973_1_MAX_DELAY_NS)
		delay = AK09973_1_MAX_DELAY_NS;

	return delay;
}

static void ak09973_1_enable_set(struct type_device_data *akm_ak09973_1)
{
	bool enable = false;
	uint8_t mode;
	int64_t delay;

	if (akm_ak09973_1 == NULL) {
		return;
	}

	enable = akm_ak09973_1->enable_flag;
	delay = ak09973_1_get_poll_delay(akm_ak09973_1);
	mutex_lock(&akm_ak09973_1->op_mutex);
	if (enable) {
		mutex_lock(&akm_ak09973_1->sensor_mutex);
		akm_ak09973_1->data_refresh_flag = false;
		mutex_unlock(&akm_ak09973_1->sensor_mutex);

		mode = ak09973_1_select_mode(delay);
		ak09973_1_set_mode(akm_ak09973_1, mode);
		hrtimer_start(&akm_ak09973_1->poll_timer,ns_to_ktime(delay),HRTIMER_MODE_REL);
	} else {
		hrtimer_cancel(&akm_ak09973_1->poll_timer);
		cancel_work_sync(&akm_ak09973_1->dwork.work);

		ak09973_1_set_mode(akm_ak09973_1, AK09973_1_MODE_POWERDOWN_MODE);

		mutex_lock(&akm_ak09973_1->sensor_mutex);
		akm_ak09973_1->data_refresh_flag = false;
		mutex_unlock(&akm_ak09973_1->sensor_mutex);
	}
	mutex_unlock(&akm_ak09973_1->op_mutex);
}

static void ak09973_1_poll_delay_set(struct type_device_data *akm_ak09973_1)
{
	bool enable = false;
	uint32_t freq;
	int64_t delay;
	int16_t delay_update = 0;
	uint8_t mode;

	if (akm_ak09973_1 == NULL) {
		return;
	}

	freq = akm_ak09973_1->odr;

	// 1000000000LL is 1s
	if (freq == 0)
		freq = AK09973_1_ODR_100HZ;
	delay = 1000000000LL / freq;

	if (delay < AK09973_1_MIN_DELAY_NS)
		delay = AK09973_1_MIN_DELAY_NS;
	else if (delay > AK09973_1_MAX_DELAY_NS)
		delay = AK09973_1_MAX_DELAY_NS;

	mutex_lock(&akm_ak09973_1->val_mutex);
	if (akm_ak09973_1->ns_delay != delay) {
		akm_ak09973_1->ns_delay = delay;
		delay_update = 1;
	}
	mutex_unlock(&akm_ak09973_1->val_mutex);
	enable = akm_ak09973_1->enable_flag;
	if ((delay_update == 1) && (enable)) {
		ak09973_1_set_mode(akm_ak09973_1, AK09973_1_MODE_POWERDOWN_MODE);
		mode = ak09973_1_select_mode(delay);
		ak09973_1_set_mode(akm_ak09973_1, mode);
	}
}


static int ak09973_1_check_chip_id(const struct i2c_client *client)
{

	uint8_t reg_wia[AK09973_1_WIA_SIZE];
	uint16_t chip_id;
	int ret;
	int i = 0;
	if (client == NULL) {
		return AK09973_1_RETCODE_NULL_POINT_ERR;
	}

	while(i<5) {
		ret = i2c_smbus_read_i2c_block_data(client, AK09973_1_REG_WIA_ADDR, AK09973_1_WIA_SIZE, reg_wia);
		if (ret >= 0)
		break;
		i++;
	}

	ret = i2c_smbus_read_i2c_block_data(client, AK09973_1_REG_WIA_ADDR, AK09973_1_WIA_SIZE, reg_wia);
	if (ret < 0) {

		return AK09973_1_RETCODE_I2C_OP_FAIL;
	}

	chip_id = make_u16(reg_wia[1], reg_wia[0]);
	if (chip_id != AK09973_1_WIA_VALUE) {

		return AK09973_1_RETCODE_ID_CHECK_FAIL;
	}

	return AK09973_1_RETCODE_SUCCESS;
}


static ssize_t ak09973_1_enable_show(struct device *dev,
	struct device_attribute *attr,
	char *buf)
{
	struct type_device_data *akm_ak09973_1 = NULL;
	uint8_t val;

	if ((dev == NULL) || (attr == NULL) || (buf == NULL)) {
		return -EINVAL;
	}

	akm_ak09973_1 = dev_get_drvdata(dev);
	if (akm_ak09973_1 == NULL) {
		return -EINVAL;
	}

	val = akm_ak09973_1->enable_flag ? 1 : 0;

	return scnprintf(buf, PAGE_SIZE, "%d\n", val);
}
#ifdef CONFIG_CUSTOM_HARDWARE_CHECK
static int g_ak09973_1_hardware_check_flag = -1;

static void ak09973_1_file_store(int data)
{

	struct file *pfile = NULL;
	int err,nwrite = 0;
	loff_t pos = 0;
	int count = 0;

	pfile = filp_open("/mnt/vendor/productinfo/sensor_calibration_data/hw_check/liftsensor_check_one", O_WRONLY | O_TRUNC, 0666);
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

static ssize_t ak09973_1_enable_store(struct device *dev,
	struct device_attribute *attr,
	const char *buf, size_t size)
{
	unsigned long val;
	struct type_device_data *akm_ak09973_1 = NULL;

#ifdef CONFIG_CUSTOM_HARDWARE_CHECK
	uint8_t reg_wia[AK09973_1_WIA_SIZE];
	int ret;
#endif

	if ((dev == NULL) || (attr == NULL) || (buf == NULL)) {
		return -EINVAL;
	}

	akm_ak09973_1 = dev_get_drvdata(dev);
	if (akm_ak09973_1 == NULL) {
		return -EINVAL;
	}

	if (size == 0) {
		return AK09973_1_RETCODE_COMMON_ERR;
	}

	if (strict_strtol(buf, AKM_BASE_NUM, &val))
		return -EINVAL;

	mutex_lock(&akm_ak09973_1->val_mutex);
	akm_ak09973_1->enable_flag = (val == 0) ? false : true;
	mutex_unlock(&akm_ak09973_1->val_mutex);

#ifdef CONFIG_CUSTOM_HARDWARE_CHECK
	if(akm_ak09973_1->enable_flag == true)
	{

		ret = i2c_smbus_read_i2c_block_data(akm_ak09973_1->i2c, AK09973_1_REG_WIA_ADDR, AK09973_1_WIA_SIZE, reg_wia);
		if (ret < 0) {

			if(g_ak09973_1_hardware_check_flag!= -1)
				ak09973_1_file_store(1);
			g_ak09973_1_hardware_check_flag = -1;

		}else{

			if(g_ak09973_1_hardware_check_flag!= 0)
				ak09973_1_file_store(0);
			g_ak09973_1_hardware_check_flag = 0;
		}
	}
#endif

	ak09973_1_enable_set(akm_ak09973_1);

	return size;
}

static ssize_t ak09973_1_odr_show(struct device *dev,
	struct device_attribute *attr,
	char *buf)
{
	struct type_device_data *akm_ak09973_1 = NULL;
	uint32_t val;

	if ((dev == NULL) || (attr == NULL) || (buf == NULL)) {
		return -EINVAL;
	}

	akm_ak09973_1 = dev_get_drvdata(dev);
	if (akm_ak09973_1 == NULL) {
		return -EINVAL;
	}
	val = akm_ak09973_1->odr;
	return scnprintf(buf, PAGE_SIZE, "%d\n", val);
}


static ssize_t ak09973_1_odr_store(struct device *dev,
	struct device_attribute *attr,
	const char *buf, size_t size)
{
	unsigned long val;
	struct type_device_data *akm_ak09973_1 = NULL;

	if ((dev == NULL) || (attr == NULL) || (buf == NULL)) {
		return -EINVAL;
	}

	akm_ak09973_1 = dev_get_drvdata(dev);
	if (akm_ak09973_1 == NULL) {
		return -EINVAL;
	}

	if (size == 0) {
		return AK09973_1_RETCODE_COMMON_ERR;
	}

	if (strict_strtol(buf, AKM_BASE_NUM, &val))
		return -EINVAL;

	if (val <= AK09973_1_ODR_10HZ)
		val = AK09973_1_ODR_10HZ;
	else if (val <= AK09973_1_ODR_20HZ)
		val = AK09973_1_ODR_20HZ;
	else if (val <= AK09973_1_ODR_50HZ)
		val = AK09973_1_ODR_50HZ;
	else
		val = AK09973_1_ODR_100HZ;

	mutex_lock(&akm_ak09973_1->val_mutex);
	akm_ak09973_1->odr = (uint32_t)val;
	mutex_unlock(&akm_ak09973_1->val_mutex);

	ak09973_1_poll_delay_set(akm_ak09973_1);

	return size;
}

static ssize_t ak09973_1_mag_data_show(struct device *dev,
	struct device_attribute *attr,
	char *buf)
{
	const int NS_2_MS = 1000000;
	const int TIMEOUT_SCALE = 2;
	struct type_device_data *akm_ak09973_1 = NULL;
	int timeout_ms;
	int32_t data[AXIS_COUNT];

	int ret;
	int error_flag = 0;

	if ((dev == NULL) || (attr == NULL) || (buf == NULL)) {
		return -EINVAL;
	}

	akm_ak09973_1 = dev_get_drvdata(dev);
	if (akm_ak09973_1 == NULL) {
		return -EINVAL;
	}
	// convert ns to ms, then double it as time-out parameter
	timeout_ms = ak09973_1_get_poll_delay(akm_ak09973_1) / NS_2_MS * TIMEOUT_SCALE;
	ret = wait_event_interruptible_timeout(akm_ak09973_1->read_waitq,
		akm_ak09973_1->data_refresh_flag, msecs_to_jiffies(timeout_ms));
	if (ret == 0) { // timeout
		error_flag = 1;
	}

	mutex_lock(&akm_ak09973_1->sensor_mutex);
	data[AXIS_X] = akm_ak09973_1->sensor_data[AXIS_X];
	data[AXIS_Y] = akm_ak09973_1->sensor_data[AXIS_Y];
	data[AXIS_Z] = akm_ak09973_1->sensor_data[AXIS_Z];
	akm_ak09973_1->data_refresh_flag = false;
	mutex_unlock(&akm_ak09973_1->sensor_mutex);

	scnprintf(buf, PAGE_SIZE, "%d %d %d %d \n",
			data[AXIS_X], data[AXIS_Y], data[AXIS_Z], error_flag);

	return scnprintf(buf, PAGE_SIZE, "%d %d %d %d \n",
		data[AXIS_X], data[AXIS_Y], data[AXIS_Z], error_flag);
}



static ssize_t ak09973_1_checkid_show(struct device *dev,
	struct device_attribute *attr,
	char *buf)
{
	int ret;
	if (ak09973_1_dev_data->id == 0)
		ret=0;
	else
		ret=-1;

	return scnprintf(buf, PAGE_SIZE, "%d \n",ret);
}

static ssize_t ak09973_1_checkid_store(struct device *dev,
	struct device_attribute *attr,
	const char *buf, size_t size)
{

	return size;
}

static ssize_t update_uevent_show(struct device *dev,
	struct device_attribute *attr,
	char *buf)
{

	return 0;
}

static ssize_t update_uevent_store(struct device *dev,
	struct device_attribute *attr,
	const char *buf, size_t size)
{

	struct type_device_data *akm_ak09973_1 = NULL;
	char *envp[2];

	akm_ak09973_1 = dev_get_drvdata(dev);
	if (akm_ak09973_1 == NULL) {
		return -EINVAL;
	}

	printk( "%s:update_uevent buf=%x\n",__func__,buf[0]);

	switch (buf[0])
	{
		case '0':
			envp[0] = "status=0";
			break;
		case '1':
			envp[0] = "status=1";
			break;
		case '2':
			envp[0] = "status=2";
			break;
		case '3':
			envp[0] = "status=3";
			break;
		case '4':
			envp[0] = "status=4";
			break;
		case '5':
			envp[0] = "status=5";
			break;
		case '6':
			envp[0] = "status=6";
			break;
		case '7':
			envp[0] = "status=7";
			break;
		case '8':
			envp[0] = "status=8";
			break;
		case '9':
			envp[0] = "status=9";
			break;
		case 65:
			envp[0] = "cameraflag=1";
			break;
		case 66:
			envp[0] = "cameraflag=2";
			break;
		case 67:
			envp[0] = "cameraflag=3";
			break;
		case 68:
			envp[0] = "cameraflag=4";
			break;
		default:
			envp[0] = "status=0";
			break;
	}

	envp[1] = NULL;

	kobject_uevent_env(&akm_ak09973_1->i2c->dev.kobj, KOBJ_CHANGE, envp);

	return size;

}

DEVICE_ATTR(ak09973_1_enable, 0660,
	ak09973_1_enable_show, ak09973_1_enable_store);
DEVICE_ATTR(ak09973_1_odr, 0660,
	ak09973_1_odr_show, ak09973_1_odr_store);
DEVICE_ATTR(ak09973_1_mag_data, 0440,
	ak09973_1_mag_data_show, NULL);
DEVICE_ATTR(ak09973_1_checkid, 0660,
	ak09973_1_checkid_show, ak09973_1_checkid_store);
DEVICE_ATTR(update_uevent, 0660,
	update_uevent_show, update_uevent_store);


static struct attribute *ak09973_1_attributes[] = {
	&dev_attr_ak09973_1_enable.attr,
	&dev_attr_ak09973_1_odr.attr,
	&dev_attr_ak09973_1_mag_data.attr,
	&dev_attr_ak09973_1_checkid.attr,
	&dev_attr_update_uevent.attr,
	NULL,
};

static const struct attribute_group ak09973_1_attr_group = {
	.attrs = ak09973_1_attributes,
};

static const struct attribute_group *ak09973_1_attr_groups[] = {
	&ak09973_1_attr_group,
	NULL,
};

static int create_sysfs_interfaces(struct type_device_data *akm_ak09973_1)
{
	int err = 0;

	if (akm_ak09973_1 == NULL)
		return -EINVAL;

	// class_destroy in remove_sysfs_interfaces
	akm_ak09973_1->dev_class = class_create(THIS_MODULE,
		AK09973_1_SYSCLS_NAME);
	if (IS_ERR(akm_ak09973_1->dev_class)) {
		err = PTR_ERR(akm_ak09973_1->dev_class);
		goto class_create_failed;
	}

	akm_ak09973_1->dev_class->dev_groups = ak09973_1_attr_groups;

	// device_destroy in create_sysfs_interfaces
	akm_ak09973_1->class_dev = device_create(akm_ak09973_1->dev_class,
		NULL, 0, akm_ak09973_1, AK09973_1_SYSDEV_NAME);
	if (IS_ERR(akm_ak09973_1->class_dev)) {
		err = PTR_ERR(akm_ak09973_1->class_dev);
		goto device_create_failed;
	}

	return err;
device_create_failed:
	akm_ak09973_1->class_dev = NULL;
	class_destroy(akm_ak09973_1->dev_class);
class_create_failed:
	akm_ak09973_1->dev_class = NULL;
	return err;
}

static void remove_sysfs_interfaces(struct type_device_data *akm_ak09973_1)
{
	if (akm_ak09973_1 == NULL)
		return;

	if (akm_ak09973_1->dev_class != NULL) {
		// device_create in create_sysfs_interfaces
		device_destroy(akm_ak09973_1->dev_class, 0);
		// class_create in create_sysfs_interfaces
		class_destroy(akm_ak09973_1->dev_class);
		akm_ak09973_1->dev_class = NULL;
	}

	return;
}


static int16_t check_data_overflow_in_q16(int16_t data_val)
{
	if (data_val >= AK09973_1_SCALE_DATA_RANGE)
		return AK09973_1_SCALE_DATA_RANGE;
	else if (data_val <= -AK09973_1_SCALE_DATA_RANGE)
		return -AK09973_1_SCALE_DATA_RANGE;
	else
		return data_val;
}


static void ak09973_1_dev_poll(struct work_struct *work)
{
	struct type_device_data *akm_ak09973_1 = NULL;
	struct i2c_client *client = NULL;
	uint8_t reg_data[AK09973_1_BDATA_SIZE];
	uint16_t st1;
	int16_t data[AXIS_COUNT];
	int ret;

	if (work == NULL) {
		return;
	}

	akm_ak09973_1 = container_of((struct delayed_work *)work, struct type_device_data, dwork);
	client = akm_ak09973_1->i2c;
	if (client == NULL) {
		return;
	}

	ret = i2c_smbus_read_i2c_block_data(client,
		AK09973_1_REG_ALL_AXES_ADDR, AK09973_1_BDATA_SIZE, reg_data);
	if (ret < 0) {

		return;
	}
	st1 = make_u16(reg_data[0], reg_data[1]); // 0 - ST1_H, 1 - ST1-L

	data[AXIS_X] = make_s16(reg_data[5], reg_data[6]); // 6 - X_H, 7 - X_L
	data[AXIS_X] = check_data_overflow_in_q16(data[AXIS_X]);
	data[AXIS_Y] = make_s16(reg_data[3], reg_data[4]); // 4 - Y_H, 5 - Y_L
	data[AXIS_Y] = check_data_overflow_in_q16(data[AXIS_Y]);
	data[AXIS_Z] = make_s16(reg_data[1], reg_data[2]); // 2 - Z_H, 3 - Z_L
	data[AXIS_Z] = check_data_overflow_in_q16(data[AXIS_Z]);

	//ak09973_1_adc_overflow(st1);

	mutex_lock(&akm_ak09973_1->sensor_mutex);
	akm_ak09973_1->sensor_data[AXIS_X] =
		(int32_t)(data[AXIS_X] * AK09973_1_SCALE_AMPLIFY_Q16);
	akm_ak09973_1->sensor_data[AXIS_Y] =
		(int32_t)(data[AXIS_Y] * AK09973_1_SCALE_AMPLIFY_Q16);
	akm_ak09973_1->sensor_data[AXIS_Z] =
		(int32_t)(data[AXIS_Z] * AK09973_1_SCALE_AMPLIFY_Q16);
	akm_ak09973_1->data_refresh_flag = true;
	if (waitqueue_active(&akm_ak09973_1->read_waitq))
		wake_up_interruptible(&akm_ak09973_1->read_waitq);
	mutex_unlock(&akm_ak09973_1->sensor_mutex);
}

static enum hrtimer_restart ak09973_1_timer_func(struct hrtimer *timer)
{
	struct type_device_data *akm_ak09973_1 = NULL;
	int64_t delay;

	akm_ak09973_1 = container_of(timer, struct type_device_data, poll_timer);
	delay = ak09973_1_get_poll_delay(akm_ak09973_1);
	queue_work(akm_ak09973_1->work_queue, &akm_ak09973_1->dwork.work);
	hrtimer_forward_now(&akm_ak09973_1->poll_timer, ns_to_ktime(delay));
	return HRTIMER_RESTART;
}

static int init_ak09973_1_device(struct i2c_client *client)
{
	int ret;

	if (!i2c_check_functionality(client->adapter,I2C_FUNC_SMBUS_BYTE_DATA)) {

		ret = -EOPNOTSUPP;
		goto gpio_free;
	}
	ret = ak09973_1_check_chip_id(client);
	if (ret < 0) {
		goto gpio_free;
	}
	return AK09973_1_RETCODE_SUCCESS;
gpio_free:
	//devm_gpio_free(dev, ak09973_1_gpio_rst);
	return ret;
}

static int init_ak09973_1_struct(struct i2c_client *client)
{
	ak09973_1_dev_data->i2c = client;
	i2c_set_clientdata(client, ak09973_1_dev_data);

	// Init variables
	mutex_init(&ak09973_1_dev_data->op_mutex);
	mutex_init(&ak09973_1_dev_data->sensor_mutex);
	mutex_init(&ak09973_1_dev_data->val_mutex);
	mutex_init(&ak09973_1_dev_data->nv_mutex);

	ak09973_1_dev_data->odr = 0;
	ak09973_1_dev_data->enable_flag = false;
	ak09973_1_dev_data->ns_delay = AK09973_1_DEFAULT_DELAY_NS;
	ak09973_1_dev_data->sensor_data[AXIS_X] = 0;
	ak09973_1_dev_data->sensor_data[AXIS_Y] = 0;
	ak09973_1_dev_data->sensor_data[AXIS_Z] = 0;
	ak09973_1_dev_data->data_refresh_flag = false;
	ak09973_1_dev_data->nvwrite_finish_flag = false;
	ak09973_1_dev_data->nvwrite_success_flag = false;

	// stop poll_timer in ak09973_1_remove
	hrtimer_init(&ak09973_1_dev_data->poll_timer,CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	ak09973_1_dev_data->poll_timer.function = ak09973_1_timer_func;

	// destroy_workqueue in ak09973_1_remove
	init_waitqueue_head(&ak09973_1_dev_data->read_waitq);
	ak09973_1_dev_data->work_queue = alloc_workqueue("ak09973_1_poll_work",WQ_UNBOUND | WQ_MEM_RECLAIM | WQ_HIGHPRI, 1);
	if (ak09973_1_dev_data->work_queue == NULL) {
		return AK09973_1_RETCODE_COMMON_ERR;
	}
	INIT_WORK(&ak09973_1_dev_data->dwork.work, ak09973_1_dev_poll);

	return AK09973_1_RETCODE_SUCCESS;
}

static void deinit_ak09973_1_struct(struct i2c_client *client)
{
	struct type_device_data *akm_ak09973_1 = NULL;

	akm_ak09973_1 = i2c_get_clientdata(client);
	if (akm_ak09973_1->work_queue != NULL) {
		destroy_workqueue(akm_ak09973_1->work_queue);
		akm_ak09973_1->work_queue = NULL;
	}
	if (akm_ak09973_1->nvwrite_queue != NULL) {
		destroy_workqueue(akm_ak09973_1->nvwrite_queue);
		akm_ak09973_1->nvwrite_queue = NULL;
	}
	if (hrtimer_active(&akm_ak09973_1->poll_timer))
		hrtimer_cancel(&akm_ak09973_1->poll_timer);
	i2c_set_clientdata(client, NULL);
}

static void deinit_ak09973_1_device(struct i2c_client *client)
{
	//struct device *dev = &client->dev;
	//if (ak09973_1_gpio_rst != 0) {
		//devm_gpio_free(dev, ak09973_1_gpio_rst);
	//	ak09973_1_gpio_rst = 0;
	//}
}

int ak09973_1_probe(struct i2c_client *client, const struct i2c_device_id *idp)
{
	int ret;

	(void)idp;
	if (client == NULL) {
		return AK09973_1_RETCODE_NULL_POINT_ERR;
	}

	ret = init_ak09973_1_device(client);
	if (ret < 0) {

		return ret;
	}

	ak09973_1_dev_data = kzalloc(sizeof(*ak09973_1_dev_data), GFP_KERNEL);
	if (ak09973_1_dev_data == NULL) {
		ret = -ENOMEM;
		goto ak09973_1_malloc_fail;
	}

	if(ret < 0)
	{
		ak09973_1_dev_data->id = -1;
	}

	else
	{
		ak09973_1_dev_data->id = 0;
	}

	ret = init_ak09973_1_struct(client);
	if (ret < 0) {
		goto ak09973_1_struct_init_fail;
	}

	ret = create_sysfs_interfaces(ak09973_1_dev_data);
	if (ret < 0) {
		goto ak09973_1_struct_init_fail;
	}

	return AK09973_1_RETCODE_SUCCESS;
ak09973_1_struct_init_fail:
	kfree(ak09973_1_dev_data);
ak09973_1_malloc_fail:
	deinit_ak09973_1_device(client);
	return ret;
}

int ak09973_1_remove(struct i2c_client *client)
{
	struct type_device_data *akm_ak09973_1 = NULL;

	if (client == NULL) {
		return AK09973_1_RETCODE_NULL_POINT_ERR;
	}

	akm_ak09973_1 = i2c_get_clientdata(client);
	remove_sysfs_interfaces(akm_ak09973_1);
	deinit_ak09973_1_struct(client);
	kfree(ak09973_1_dev_data);
	deinit_ak09973_1_device(client);

	return AK09973_1_RETCODE_SUCCESS;
}

static struct i2c_device_id ak09973_1_idtable[] = {
	{ "stk,ak09973_1", 0 },
	{}
};
MODULE_DEVICE_TABLE(i2c, ak09973_1_idtable);

static const struct of_device_id ak09973_1_of_id_table[] = {
	{ .compatible = "stk,ak09973_1" },
	{},
};

static struct i2c_driver ak09973_1_driver = {
	.driver = {
		.name = "ak09973_1",
		.owner = THIS_MODULE,
		.of_match_table = ak09973_1_of_id_table,
	},
	.id_table = ak09973_1_idtable,
	.probe = ak09973_1_probe,
	.remove = ak09973_1_remove,
};

static int __init ak09973_1_init(void)
{
	int rc;

	rc = i2c_add_driver(&ak09973_1_driver);

	return rc;
}

static void __exit ak09973_1_exit(void)
{
	i2c_del_driver(&ak09973_1_driver);
}

module_init(ak09973_1_init);
module_exit(ak09973_1_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("AKM09973_1 driver");
MODULE_AUTHOR("Anhui Yunshu Zhihui Technology Co., Ltd.");
