/*
 * linux/drivers/video/stspin220/pwm_bl.c
 *
 * simple PWM based stspin220 control, board code has to setup
 * 1) pin configuration so PWM waveforms can output
 * 2) platform_data being correctly configured
 *
 * Copyright (C) 2021, Anhui Yunshu Zhihui Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/gpio/consumer.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/fb.h>
#include <linux/backlight.h>
#include <linux/err.h>
#include <linux/pwm.h>
#include <linux/backlight.h>
#include <linux/regulator/consumer.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/hrtimer.h>
#include <linux/wakelock.h>


//#include <pthread.h>

enum motor_pwm_mode {
	MOTOR_PWM_STOP,
	MOTOR_PWM_FAST,
	MOTOR_PWM_SLOW
};


struct pwm_bl_data {
	struct pwm_device	*pwm;
	struct regulator *avdd;
	struct regulator *avddsdio;
	unsigned int reset_gpio;
	unsigned int motoen_gpio;
	unsigned int mode1_gpio;
	unsigned int mode2_gpio;
	unsigned int dir_gpio;
	unsigned int pwmen_gpio;
	unsigned int fault_gpio;
	unsigned int pwm_period_ns;
};
#define PWM_TIME 4480
#define PWM_MID_TIME 3500
#define PWM_INIT_TIME 128
#define PWM_CYCLE_TIME 433

struct pwm_bl_data *ddata;
struct kobject *pwm_stspin220_node_obj = NULL;
struct task_struct *init_thrd_top;
struct task_struct *init_thrd_mid;
struct task_struct *init_thrd_top_back;
struct task_struct *init_thrd_mid_back;
//static struct hrtimer task_stop_timer;
static struct hrtimer task_pwm_timer;

struct pwm_bl_data *gdata;
struct wake_lock wakelock;

static void pwm_stspin220_resume(struct pwm_bl_data *data);
static void pwm_stspin220_suspend(struct pwm_bl_data *data);

static int swith = 0;
static int nums = 0;

static int motor_mode=0;
static int motor_ison=0;
static int front_camera_ison=0;
static enum motor_pwm_mode pwmmode;

static unsigned long UP_TO_TOP_TIME = 0;
static unsigned long UP_TO_TOP_TIME_FAST = 0;
static unsigned long UP_TO_TOP_TIME_SLOW = 0;
static unsigned long UP_TO_MID_TIME = 0;
static unsigned long UP_MID_TO_TOP_TIME = 0;
static unsigned long UP_MID_TO_TOP_TIME_FAST = 0;
static unsigned long UP_MID_TO_TOP_TIME_SLOW = 0;
static unsigned long BACK_TOP_TO_HOME_TIME = 0;
static unsigned long BACK_TOP_TO_HOME_TIME_FAST = 0;
static unsigned long BACK_TOP_TO_HOME_TIME_SLOW = 0;
static unsigned long BACK_MID_TO_HOME_TIME = 0;
static unsigned long BACK_TOP_TO_MID_TIME = 0;
static unsigned long BACK_TOP_TO_MID_TIME_FAST = 0;
static unsigned long BACK_TOP_TO_MID_TIME_SLOW = 0;

enum motor_control_type {
	MOTOR_CONTROL_STOP = 0,
	MOTOR_CONTROL_UP_TO_TOP,
	MOTOR_CONTROL_BACK_TOP_TO_HOME,
	MOTOR_CONTROL_UP_TO_MID,
	MOTOR_CONTROL_BACK_MID_TO_HOME,
	MOTOR_CONTROL_UP_MID_TO_TOP,
	MOTOR_CONTROL_BACK_TOP_TO_MID,
	MOTOR_CONTROL_END_STOP

};
static enum motor_control_type motor_control_mode;



static int trip_mode = 0;

static void init_time(void)
{
	unsigned long fast_cycle_ns;
	unsigned long slow_cycle_ns;

    pr_err(" wbwmotor  wbwmotor  =%s =%d \n",__func__, __LINE__);

	fast_cycle_ns = ((3+1)*(0x2aa+1)*1000)/26;
//	slow_cycle_ns = ((5+1)*(0x386+1)*1000)/26;
	slow_cycle_ns = ((6+1)*(0x3a0+1)*1000)/26;
	pr_err(" wbwmotor pwm motor init_time fast_cycle_ns = %ld\n",fast_cycle_ns);
	pr_err(" wbwmotor pwm motor init_time slow_cycle_ns = %ld\n",slow_cycle_ns);


	UP_TO_TOP_TIME_FAST = fast_cycle_ns*4221*4;
	UP_TO_TOP_TIME_SLOW = slow_cycle_ns*500*4;
	UP_TO_TOP_TIME = UP_TO_TOP_TIME_FAST + UP_TO_TOP_TIME_SLOW;

	UP_TO_MID_TIME = fast_cycle_ns*3700*4;

	UP_MID_TO_TOP_TIME_FAST = (fast_cycle_ns*(4221-3700))*4;
	UP_MID_TO_TOP_TIME_SLOW = slow_cycle_ns*500*4;
	UP_MID_TO_TOP_TIME = UP_MID_TO_TOP_TIME_FAST + UP_MID_TO_TOP_TIME_SLOW;

	BACK_TOP_TO_HOME_TIME_FAST = fast_cycle_ns*4221*4;
	BACK_TOP_TO_HOME_TIME_SLOW = slow_cycle_ns*500*4;
	BACK_TOP_TO_HOME_TIME = BACK_TOP_TO_HOME_TIME_FAST + BACK_TOP_TO_HOME_TIME_SLOW;

    BACK_MID_TO_HOME_TIME = fast_cycle_ns*(3700+20)*4;

	BACK_TOP_TO_MID_TIME_FAST = (fast_cycle_ns*(4221-3700))*4;
	BACK_TOP_TO_MID_TIME_SLOW = slow_cycle_ns*500*4;
	BACK_TOP_TO_MID_TIME = BACK_TOP_TO_MID_TIME_FAST + BACK_TOP_TO_MID_TIME_SLOW;

	pr_err(" wbwmotor pwm motor init_time UP_TO_TOP_TIME_FAST = %ld\n",UP_TO_TOP_TIME_FAST);
	pr_err(" wbwmotor pwm motor init_time UP_TO_TOP_TIME_SLOW = %ld\n",UP_TO_TOP_TIME_SLOW);
	pr_err(" wbwmotor pwm motor init_time UP_TO_TOP_TIME = %ld\n",UP_TO_TOP_TIME);

	pr_err(" wbwmotor pwm motor init_time UP_TO_MID_TIME = %ld\n",UP_TO_MID_TIME);

	pr_err(" wbwmotor pwm motor init_time UP_MID_TO_TOP_TIME_FAST = %ld\n",UP_MID_TO_TOP_TIME_FAST);
	pr_err(" wbwmotor pwm motor init_time UP_MID_TO_TOP_TIME_SLOW = %ld\n",UP_MID_TO_TOP_TIME_SLOW);
	pr_err(" wbwmotor pwm motor init_time UP_MID_TO_TOP_TIME = %ld\n",UP_MID_TO_TOP_TIME);

	pr_err(" wbwmotor pwm motor init_time BACK_TOP_TO_HOME_TIME_FAST = %ld\n",BACK_TOP_TO_HOME_TIME_FAST);
	pr_err(" wbwmotor pwm motor init_time BACK_TOP_TO_HOME_TIME_SLOW = %ld\n",BACK_TOP_TO_HOME_TIME_SLOW);
	pr_err(" wbwmotor pwm motor init_time BACK_TOP_TO_HOME_TIME = %ld\n",BACK_TOP_TO_HOME_TIME);

	pr_err(" wbwmotor pwm motor init_time BACK_MID_TO_HOME_TIME = %ld\n",BACK_MID_TO_HOME_TIME);

	pr_err(" wbwmotor pwm motor init_time BACK_TOP_TO_MID_TIME_FAST = %ld\n",BACK_TOP_TO_MID_TIME_FAST);
	pr_err(" wbwmotor pwm motor init_time BACK_TOP_TO_MID_TIME_SLOW = %ld\n",BACK_TOP_TO_MID_TIME_SLOW);
	pr_err(" wbwmotor pwm motor init_time BACK_TOP_TO_MID_TIME = %ld\n",BACK_TOP_TO_MID_TIME);


}


static void pwm_stop(struct pwm_bl_data *data)
{
/*
	struct pwm_state state;
	int ret;
    pr_err(" wbwmotor  wbwmotor  =%s =%d \n",__func__, __LINE__);

	pwmmode = MOTOR_PWM_STOP;
	pwm_get_state(data->pwm, &state);
	state.enabled = false;
	ret = pwm_apply_state(data->pwm, &state);
	if (ret) {
		pr_err(" wbwmotor [wfp] failed to apply initial PWM state: %d\n",ret);
		return ;
	}
*/
	return ;
}

static void pwm_start_fast(struct pwm_bl_data *data)
{
	struct pwm_state state;
	int ret;

    pr_err(" wbwmotor  wbwmotor  =%s =%d \n",__func__, __LINE__);

	pwmmode = MOTOR_PWM_FAST;
	pwm_get_state(data->pwm, &state);
	state.period = 104666;
	state.duty_cycle = state.period/2; //50%��ռ�ձ�
	state.enabled = true;
	ret = pwm_apply_state(data->pwm, &state);

	return ;
}

static void pwm_start_slow(struct pwm_bl_data *data)
{
	struct pwm_state state;
	int ret;
    pr_err(" wbwmotor  wbwmotor  =%s =%d \n",__func__, __LINE__);

	pwmmode = MOTOR_PWM_SLOW;
	pwm_get_state(data->pwm, &state);
	state.period = 250000;
	state.duty_cycle = state.period/2; //50%��ռ�ձ�
	state.enabled = true;
	ret = pwm_apply_state(data->pwm, &state);

	return ;
}



static int pwm_stspin220_init_step(void *sdata)
{

	struct pwm_bl_data *data = sdata;

	int pwm_time = 0;
	pwm_time = 11;
	motor_ison = 1;
	gpio_direction_output(data->motoen_gpio,1);
	mdelay(pwm_time);
	gpio_direction_output(data->motoen_gpio,0);
	motor_ison = 0;
	return 0;
}

static void pwm_stspin220_power_off(struct pwm_bl_data *data)
{

	struct pwm_state state;
	int ret;

	motor_ison = 0;
	gpio_direction_output(data->motoen_gpio,0);

	pwmmode = MOTOR_PWM_STOP;
	pwm_get_state(data->pwm, &state);
	state.enabled = false;
	ret = pwm_apply_state(data->pwm, &state);
	if (ret) {
		pr_err(" wbwmotor [wfp] failed to apply initial PWM state: %d\n",ret);
		return ;
	}
}
#if 0

static int pwm_stspin220_remove(struct platform_device *pdev)
{

	pwm_stspin220_power_off();
	return 0;
}

static void pwm_stspin220_shutdown(struct platform_device *pdev)
{
	pr_err(" wbwmotor stspin220 request IO port failed.");
}

#endif




static int pwm_stspin220_power_on(void *sdata)
{
//	struct pwm_state state;
	struct pwm_bl_data *data = sdata;
//	int i;
	u64 delay_time = 0;

	pr_err(" wbwmotor pwm_stspin220_power_on s-----");
	while(1)
	{

		if(pwmmode == MOTOR_PWM_STOP)
		{
			gpio_direction_output(ddata->motoen_gpio,0);
			motor_ison = 0;
			pwm_stop(ddata);
			pr_err(" wbwmotor pwm_stspin220_power_on pwmmode == MOTOR_PWM_STOP out 2");
			break;
		}

		if(motor_control_mode == MOTOR_CONTROL_STOP)
		{
			pr_err(" wbwmotor pwm_stspin220_power_on motor_control_mode == MOTOR_CONTROL_STOP");
			pwm_stop(data);
			pwmmode = MOTOR_PWM_STOP;
			gpio_direction_output(data->motoen_gpio,0);
			break;
		}else if (motor_control_mode == MOTOR_CONTROL_UP_TO_TOP &&( pwmmode == MOTOR_PWM_SLOW))
		{


			pr_err(" wbwmotor  wbwmotor  =%s =%d  \n",__func__, __LINE__);
			pwm_stop(data);
			pwm_start_slow(data);
			if (hrtimer_active(&task_pwm_timer))
			{
				hrtimer_cancel(&task_pwm_timer);
			}
 			motor_control_mode = MOTOR_CONTROL_END_STOP;
			pr_err(" wbwmotor  wbwmotor  =%s =%d UP_TO_TOP_TIME_SLOW=%ld  \n",__func__, __LINE__,UP_TO_TOP_TIME_SLOW);
			hrtimer_start(&task_pwm_timer, ktime_set(UP_TO_TOP_TIME_SLOW/1000000000, (UP_TO_TOP_TIME_SLOW)%1000000000), HRTIMER_MODE_REL);

			break;

		}else if (motor_control_mode == MOTOR_CONTROL_BACK_TOP_TO_HOME && pwmmode == MOTOR_PWM_FAST)
		{

			pwm_stop(data);
			pwm_start_fast(data);
			if (hrtimer_active(&task_pwm_timer))
			{
				hrtimer_cancel(&task_pwm_timer);
			}
			motor_control_mode = MOTOR_CONTROL_END_STOP;
			hrtimer_start(&task_pwm_timer, ktime_set(BACK_TOP_TO_HOME_TIME_FAST/1000000000, (BACK_TOP_TO_HOME_TIME_FAST)%1000000000), HRTIMER_MODE_REL);

			break;

		}else if (motor_control_mode == MOTOR_CONTROL_UP_MID_TO_TOP && pwmmode == MOTOR_PWM_SLOW)
		{

			pwm_stop(data);
			pwm_start_slow(data);
			if (hrtimer_active(&task_pwm_timer))
			{
				hrtimer_cancel(&task_pwm_timer);
			}
			motor_control_mode = MOTOR_CONTROL_END_STOP;
			hrtimer_start(&task_pwm_timer, ktime_set(BACK_TOP_TO_MID_TIME_SLOW/1000000000, (BACK_TOP_TO_MID_TIME_SLOW)%1000000000), HRTIMER_MODE_REL);

			break;

		}else if (motor_control_mode == MOTOR_CONTROL_BACK_TOP_TO_MID && pwmmode == MOTOR_PWM_FAST)
		{

			pwm_stop(data);
			pwm_start_fast(data);
			if (hrtimer_active(&task_pwm_timer))
			{
				hrtimer_cancel(&task_pwm_timer);
			}
			motor_control_mode = MOTOR_CONTROL_END_STOP;
			hrtimer_start(&task_pwm_timer, ktime_set(BACK_TOP_TO_MID_TIME_FAST/1000000000, (BACK_TOP_TO_MID_TIME_FAST)%1000000000), HRTIMER_MODE_REL);

			break;

		}


		if(delay_time>3000000)
		{
			pr_err(" wbwmotor pwm_stspin220_power_on timer out1");
			pwm_stop(data);
			pwmmode = MOTOR_PWM_STOP;
			gpio_direction_output(data->motoen_gpio,0);

			break;

		}
		udelay(1);
		delay_time++;
	}



	delay_time = 0;
	while(1)
	{

	if(pwmmode == MOTOR_PWM_STOP)
	{
		gpio_direction_output(ddata->motoen_gpio,0);
		motor_ison = 0;
		pwm_stop(ddata);
		pr_err(" wbwmotor wbwmotor pwm_stspin220_power_on pwmmode == MOTOR_PWM_STOP out 2");

		break;
	}

	udelay(1);
	delay_time++;
	if(delay_time>3000000)
	{
		pr_err(" wbwmotor wbwmotor pwm_stspin220_power_on timer out2");
		pwm_stop(data);
		pwmmode = MOTOR_PWM_STOP;
		gpio_direction_output(data->motoen_gpio,0);

		break;
	}

	}
	pr_err(" wbwmotor  wbwmotor  =%s =%d  end \n",__func__, __LINE__);

	return 0;
}


static int start_pwm_init(struct pwm_bl_data *data)
{
	struct task_struct *init_thrd;
	/* create and run update thread */
	init_thrd = kthread_run(pwm_stspin220_power_on,
			data, "stspin220_pwm_init_thread");
	if (IS_ERR_OR_NULL(init_thrd)) {
		pr_err(" wbwmotor Failed to create update thread:%ld",PTR_ERR(init_thrd));
		return -EFAULT;
	}
	return 0;
}


static void stspin220_io_direction_set(struct pwm_bl_data *data)
{
	gpio_direction_output(data->pwmen_gpio,1);
	udelay(2000);
	gpio_direction_output(data->motoen_gpio,0);
	gpio_direction_output(data->reset_gpio,0);
	gpio_direction_output(data->mode1_gpio,0);
	gpio_direction_output(data->mode2_gpio,1);
	gpio_direction_output(data->dir_gpio,1);
	udelay(5);
	gpio_direction_output(data->reset_gpio,1);

	pr_err(" wbwmotor %s: [wfp] gpio set success\n",__func__);
}

#ifdef CONFIG_OF
static int pwm_stspin220_parse_dt(struct device *dev,
				  struct pwm_bl_data *data)
{
	struct device_node *node = dev->of_node;
	int r;

	if (!node)
		return -ENODEV;

	memset(data, 0, sizeof(*data));
	r = of_get_named_gpio(node, "reset-gpios", 0);
	if (r < 0) {
		pr_err(" wbwmotor [wfp]invalid reset-gpios in dt: %d", r);
		return -EINVAL;
	}
	pr_err(" wbwmotor [wfp]get reset-gpios[%d] from dt", r);
	data->reset_gpio = r;

	r = of_get_named_gpio(node, "motoen-gpios", 0);
	if (r < 0) {
		pr_err(" wbwmotor [wfp]invalid motoen-gpios in dt: %d", r);
		return -EINVAL;
	}
	pr_err(" wbwmotor [wfp]get motoen-gpios[%d] from dt", r);
	data->motoen_gpio = r;

	r = of_get_named_gpio(node, "mode1-gpios", 0);
	if (r < 0) {
		pr_err(" wbwmotor [wfp]invalid mode1-gpios in dt: %d", r);
		return -EINVAL;
	}
	pr_err(" wbwmotor [wfp]get mode1-gpios[%d] from dt", r);
	data->mode1_gpio = r;

	r = of_get_named_gpio(node, "mode2-gpios", 0);
	if (r < 0) {
		pr_err(" wbwmotor [wfp]invalid mode2-gpios in dt: %d", r);
		return -EINVAL;
	}
	pr_err(" wbwmotor [wfp]get mode2-gpios[%d] from dt", r);
	data->mode2_gpio = r;

	r = of_get_named_gpio(node, "dir-gpios", 0);
	if (r < 0) {
		pr_err(" wbwmotor [wfp]invalid dir-gpios in dt: %d", r);
		return -EINVAL;
	}
	pr_err(" wbwmotor [wfp]get dir-gpios[%d] from dt", r);
	data->dir_gpio = r;

	r = of_get_named_gpio(node, "pwmen-gpios", 0);
	if (r < 0) {
		pr_err(" wbwmotor [wfp]invalid pwmen-gpios in dt: %d", r);
		return -EINVAL;
	}
	pr_err(" wbwmotor [wfp]get pwmen-gpios[%d] from dt", r);
	data->pwmen_gpio = r;

	r = of_get_named_gpio(node, "fault-gpios", 0);
	if (r < 0) {
		pr_err(" wbwmotor [wfp]invalid fault-gpios in dt: %d", r);
		return -EINVAL;
	}
	pr_err(" wbwmotor [wfp]get fault-gpios[%d] from dt", r);
	data->fault_gpio = r;
	return 0;
}

static const struct of_device_id pwm_stspin220_of_match[] = {
	{ .compatible = "pwm-stspin220" },
	{ }
};

MODULE_DEVICE_TABLE(of, pwm_stspin220_of_match);
#else
static int pwm_stspin220_parse_dt(struct device *dev,
				  struct pwm_bl_data *data)
{
	return -ENODEV;
}
#endif

static s32 stspin220_request_io_port(struct pwm_bl_data *data)
{
	s32 ret = 0;

	pr_err(" wbwmotor [wfp] request gpio");
	ret = gpio_request(data->reset_gpio, "PWM_RST");
	if (ret < 0) {
		pr_err(" wbwmotor Failed to request GPIO:%d, ERRNO:%d", data->reset_gpio, ret);
		ret = -ENODEV;
	}

	ret = gpio_request(data->motoen_gpio, "MOTO_EN");
	if (ret < 0) {
		pr_err(" wbwmotor Failed to request GPIO:%d, ERRNO:%d", data->motoen_gpio, ret);
		ret = -ENODEV;
	}
	ret = gpio_request(data->mode1_gpio, "PWM_MODE1");
	if (ret < 0) {
		pr_err(" wbwmotor Failed to request GPIO:%d, ERRNO:%d", data->mode1_gpio, ret);
		ret = -ENODEV;
		return ret;
	}

	ret = gpio_request(data->mode2_gpio, "PWM_MODE2");
	if (ret < 0) {
		pr_err(" wbwmotor Failed to request GPIO:%d, ERRNO:%d", data->mode2_gpio, ret);
		ret = -ENODEV;
		return ret;
	}

	ret = gpio_request(data->dir_gpio, "PWM_DIR");
	if (ret < 0) {
		pr_err(" wbwmotor Failed to request GPIO:%d, ERRNO:%d", data->dir_gpio, ret);
		ret = -ENODEV;
		return ret;
	}

	ret = gpio_request(data->pwmen_gpio, "PWM_POWEREN");
	if (ret < 0) {
		pr_err(" wbwmotor Failed to request GPIO:%d, ERRNO:%d", data->pwmen_gpio, ret);
		ret = -ENODEV;
		return ret;
	}

	ret = gpio_request(data->fault_gpio, "PWM_FAULT");
	if (ret < 0) {
		pr_err(" wbwmotor Failed to request GPIO:%d, ERRNO:%d", data->fault_gpio, ret);
		ret = -ENODEV;
		return ret;
	}


	pr_err(" wbwmotor [wfp] request gpio success\n");

	return ret;
}

/*
static enum hrtimer_restart task_stop_timer_func(struct hrtimer *timer)
{
   gpio_direction_output(ddata->motoen_gpio,0);
   motor_ison = 0;
   pwm_stop(ddata);
   pwmmode == MOTOR_PWM_STOP;
   pr_err(" wbwmotor wbw	func=%s  line=%d",__func__,__LINE__);
   return HRTIMER_NORESTART;
}
*/

static enum hrtimer_restart task_pwm_timer_func(struct hrtimer *timer)
{

	pr_err(" wbwmotor  wbwmotor  =%s =%d motor_control_mode=%d \n",__func__, __LINE__,motor_control_mode);
	switch (motor_control_mode) {

		case MOTOR_CONTROL_UP_TO_TOP:
			pwmmode = MOTOR_PWM_SLOW;
		break;
		case MOTOR_CONTROL_BACK_TOP_TO_HOME:
			pwmmode = MOTOR_PWM_FAST;
		break;
		case MOTOR_CONTROL_UP_TO_MID:
			pwmmode = MOTOR_PWM_STOP;
		break;
		case MOTOR_CONTROL_BACK_MID_TO_HOME:
			pwmmode = MOTOR_PWM_STOP;
		break;
		case MOTOR_CONTROL_UP_MID_TO_TOP:
			pwmmode = MOTOR_PWM_SLOW;
		break;
		case MOTOR_CONTROL_BACK_TOP_TO_MID:
			pwmmode = MOTOR_PWM_FAST;
		break;
		case MOTOR_CONTROL_END_STOP:
			gpio_direction_output(ddata->motoen_gpio,0);
			pwmmode = MOTOR_PWM_STOP;
		break;
		default:
			pwmmode = MOTOR_PWM_STOP;
		break;
	}

   return HRTIMER_NORESTART;
}


static ssize_t stspin220_node_show(struct kobject *kobj,
							 struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n",nums);
}

//ns
//unsigned long trip_timer[6] = {1857604000, 1879458000, 1901312000, 1923166000, 1945020000, 1966874000};
//unsigned long trip_timer[6] = {1966874000, 1966874000, 1966874000, 1923166000, 1945020000, 1966874000};
unsigned long trip_timer[6] = {1966874000, 1966874000, 1966874000, 1923166000, 1945020000, 1984708000};



/*
0  stop
1  up to top
2  top back to home
3  up to mid
4  mid back to home
5  up 0.1mm
6  resume
7  suspend
8  mid up to top
9  top back to mid
*/
static ssize_t stspin220_node_store(struct kobject *kobj,
									  struct kobj_attribute *attr, const char *buf, size_t count)
{
	struct pwm_bl_data *data = ddata;

	pr_err(" wbwmotor wbw  stspin220_node_store:%d",buf[0]);

	if (buf[0] == '0') {
		swith=1;
		motor_control_mode = MOTOR_CONTROL_STOP;
		if (hrtimer_active(&task_pwm_timer))
		{
			hrtimer_cancel(&task_pwm_timer);
		}
		pwm_stspin220_power_off(data);
	} else if (buf[0] == '1') {
		motor_control_mode = MOTOR_CONTROL_UP_TO_TOP;
		gpio_direction_output(data->dir_gpio,0);
		hrtimer_cancel(&task_pwm_timer);
		motor_ison = 1;
		gpio_direction_output(data->motoen_gpio,1);
		pwm_start_fast(data);
		start_pwm_init(data);

		pr_err(" wbwmotor  wbwmotor  =%s =%d UP_TO_TOP_TIME_FAST =%ld \n",__func__, __LINE__,UP_TO_TOP_TIME_FAST);
		hrtimer_start(&task_pwm_timer, ktime_set(UP_TO_TOP_TIME_FAST/1000000000, (UP_TO_TOP_TIME_FAST)%1000000000), HRTIMER_MODE_REL);

	} else if (buf[0] == '2') {
		motor_control_mode = MOTOR_CONTROL_BACK_TOP_TO_HOME;
		gpio_direction_output(data->dir_gpio,1);
		hrtimer_cancel(&task_pwm_timer);
		motor_ison = 1;
		gpio_direction_output(data->motoen_gpio,1);
		pwm_start_slow(data);
		start_pwm_init(data);
		hrtimer_start(&task_pwm_timer, ktime_set(BACK_TOP_TO_HOME_TIME_SLOW/1000000000, BACK_TOP_TO_HOME_TIME_SLOW%1000000000), HRTIMER_MODE_REL);

	}else if (buf[0] == '3') {
		motor_control_mode = MOTOR_CONTROL_UP_TO_MID;
		gpio_direction_output(data->dir_gpio,0);
		hrtimer_cancel(&task_pwm_timer);
		motor_ison = 1;
		gpio_direction_output(data->motoen_gpio,1);
		pwm_start_fast(data);
		start_pwm_init(data);
		hrtimer_start(&task_pwm_timer, ktime_set(UP_TO_MID_TIME/1000000000, UP_TO_MID_TIME%1000000000), HRTIMER_MODE_REL);

	}else if (buf[0] == '4') {
		motor_control_mode = MOTOR_CONTROL_BACK_MID_TO_HOME;
		gpio_direction_output(data->dir_gpio,1);
		hrtimer_cancel(&task_pwm_timer);
		motor_ison = 1;
		gpio_direction_output(data->motoen_gpio,1);
		pwm_start_fast(data);
		start_pwm_init(data);
		hrtimer_start(&task_pwm_timer, ktime_set(BACK_MID_TO_HOME_TIME/1000000000, BACK_MID_TO_HOME_TIME%1000000000), HRTIMER_MODE_REL);

	}else if (buf[0] == '5') {
		gpio_direction_output(data->dir_gpio,0);
		motor_ison = 1;
		gpio_direction_output(data->motoen_gpio,1);
		pwm_start_fast(data);
		pwm_stspin220_init_step(data);
		gpio_direction_output(data->motoen_gpio,0);
		pwm_stop(data);
		motor_ison = 0;
	}else if (buf[0] == '6') {
		pwm_stspin220_resume(data);
	}else if (buf[0] == '7') {
		pwm_stspin220_suspend(data);
	} else if (buf[0] == '8') {
		motor_control_mode = MOTOR_CONTROL_UP_MID_TO_TOP;
		gpio_direction_output(data->dir_gpio,0);
		hrtimer_cancel(&task_pwm_timer);
		motor_ison = 1;
		gpio_direction_output(data->motoen_gpio,1);
		pwm_start_fast(data);
		start_pwm_init(data);
		hrtimer_start(&task_pwm_timer, ktime_set(UP_MID_TO_TOP_TIME_FAST/1000000000, UP_MID_TO_TOP_TIME_FAST%1000000000), HRTIMER_MODE_REL);

	} else if (buf[0] == '9') {
		motor_control_mode = MOTOR_CONTROL_BACK_TOP_TO_MID;
		gpio_direction_output(data->dir_gpio,1);
		hrtimer_cancel(&task_pwm_timer);
		motor_ison = 1;
		gpio_direction_output(data->motoen_gpio,1);
		pwm_start_slow(data);
		start_pwm_init(data);
		hrtimer_start(&task_pwm_timer, ktime_set(BACK_TOP_TO_MID_TIME_SLOW/1000000000, BACK_TOP_TO_MID_TIME_SLOW%1000000000), HRTIMER_MODE_REL);
	}

	return count;
}


static ssize_t motor_node_show(struct kobject *kobj,
							 struct kobj_attribute *attr, char *buf)
{
	pr_err(" wbwmotor wbw  motor_node_show:%d",motor_mode);

	return sprintf(buf, "%d\n",motor_mode);
}

static ssize_t motor_node_store(struct kobject *kobj,
									  struct kobj_attribute *attr, const char *buf, size_t count)
{
	//struct pwm_bl_data *data = ddata;

 if (buf[0] == '0')
	motor_mode = 0;
 if (buf[0] == '1')
	motor_mode = 1;
 if (buf[0] == '2')
	motor_mode = 2;
 if (buf[0] == '3')
	motor_mode = 3;
 if (buf[0] == '4')
	motor_mode = 4;

 pr_err(" wbwmotor wbw  motor_node_store:=%d  buf[0]=%d",motor_mode,buf[0]);
	return count;
}


static ssize_t motor_status_show(struct kobject *kobj,
							 struct kobj_attribute *attr, char *buf)
{
	pr_err(" wbwmotor   motor_status_node_show:%d",motor_ison);

	return sprintf(buf, "%d\n",motor_ison);
}


static ssize_t motor_status_store(struct kobject *kobj,
									  struct kobj_attribute *attr, const char *buf, size_t count)
{

	return count;
}

static ssize_t front_camera_show(struct kobject *kobj,
							 struct kobj_attribute *attr, char *buf)
{
	pr_err(" wbwmotor   front_camera_show:%d",front_camera_ison);
	return sprintf(buf, "%d\n",front_camera_ison);
}


static ssize_t front_camera_store(struct kobject *kobj,
									  struct kobj_attribute *attr, const char *buf, size_t count)
{

	if (buf[0] == '0')
	   front_camera_ison = 0;
	else
	   front_camera_ison = 1;

	return count;
}

static ssize_t trip_show(struct kobject *kobj,
							 struct kobj_attribute *attr, char *buf)
{
	pr_err(" wbwmotor   trip_show:%d",trip_mode);
	return sprintf(buf, "%d\n",trip_mode);
}

static ssize_t trip_store(struct kobject *kobj,
									  struct kobj_attribute *attr, const char *buf, size_t count)
{

	switch (buf[0])
	{
		case '0':
			trip_mode = 0;
			break;
		case '1':
			trip_mode = 1;
			break;
		case '2':
			trip_mode = 2;
			break;
		case '3':
			trip_mode = 3;
			break;
		case '4':
			trip_mode = 4;
			break;
		case '5':
			trip_mode = 5;
			break;
		default:
			trip_mode = 0;
			break;
	}

	pr_err(" wbwmotor   trip_store:%d  buf[0]0x=%x",trip_mode,buf[0]);

//	UP_TO_TOP_TIME = trip_timer[trip_mode];
//	UP_TO_MID_TIME = 1529790000;
//	UP_MID_TO_TOP_TIME = UP_TO_TOP_TIME - UP_TO_MID_TIME;
//	BACK_TOP_TO_HOME_TIME = UP_TO_TOP_TIME;
//	BACK_MID_TO_HOME_TIME = UP_TO_MID_TIME + 20000000;
//	BACK_TOP_TO_MID_TIME = UP_TO_TOP_TIME - UP_TO_MID_TIME;

	return count;
}

static ssize_t motor_lock_show(struct kobject *kobj,
							 struct kobj_attribute *attr, char *buf)
{
	return 0;
}

static ssize_t motor_lock_store(struct kobject *kobj,
									  struct kobj_attribute *attr, const char *buf, size_t count)
{

	switch (buf[0])
	{
		case '0':
			pr_err(" wbwmotor   motor_lock_store unlock");
			wake_unlock(&wakelock);
			break;

		case '1':
			pr_err(" wbwmotor   motor_lock_store lock");
			wake_lock(&wakelock);
			break;
		default:
			break;
	}
	return count;
}

static struct kobj_attribute dev_attr_stspin220_node =
	__ATTR(pwm_stspin220_node, 0644, stspin220_node_show, stspin220_node_store);
static struct kobj_attribute dev_attr_motor_node =
	__ATTR(motor_node, 0664, motor_node_show, motor_node_store);
static struct kobj_attribute dev_attr_motor_status =
	__ATTR(motor_status, 0664, motor_status_show, motor_status_store);
static struct kobj_attribute dev_attr_front_camera_status =
	__ATTR(front_camera_status, 0664, front_camera_show, front_camera_store);
static struct kobj_attribute dev_attr_trip =
	__ATTR(trip, 0664, trip_show, trip_store);
static struct kobj_attribute dev_attr_motor_lock =
	__ATTR(motor_lock, 0664, motor_lock_show, motor_lock_store);



static struct attribute *stspin220_node_attrs[] = {
	&dev_attr_stspin220_node.attr,
	&dev_attr_motor_node.attr,
	&dev_attr_motor_status.attr,
	&dev_attr_front_camera_status.attr,
	&dev_attr_trip.attr,
	&dev_attr_motor_lock.attr,
	NULL,
};

static struct attribute_group stspin220_node_attr_group = {
	.attrs = stspin220_node_attrs,
};

static int stspin220_node(void)
{
	int err;
	struct kobject *sys_kobj=NULL;

	pr_err(" wbwmotor %s: [wfp] creat stspin220_node \n",__func__);
	pwm_stspin220_node_obj = kobject_create_and_add("stspin220", sys_kobj);
	if (!pwm_stspin220_node_obj) {
		pr_err(" wbwmotor %s:	kobject creation failed.\n",__func__);
	}

	err = sysfs_create_group(pwm_stspin220_node_obj, &stspin220_node_attr_group);
	if (err) {
		printk( "%s:failed to create pwm stspin220 attributes group\n",__func__);
	}
	pr_err(" wbwmotor %s: [wfp] creat stspin220_node success \n",__func__);
	return err;
}
//end add

static void pwm_stspin220_gpio_free(struct pwm_bl_data *data)
{
	gpio_free(data->reset_gpio);
	gpio_free(data->motoen_gpio);
	gpio_free(data->mode1_gpio);
	gpio_free(data->mode2_gpio);
	gpio_free(data->dir_gpio);
	gpio_free(data->fault_gpio);
	gpio_free(data->pwmen_gpio);
}

static void pwm_stspin220_suspend(struct pwm_bl_data *data)
{
	struct pwm_state state;
	int ret;

	pwm_get_state(data->pwm, &state);
	//state.period = 440000;
	//state.duty_cycle = state.period/2; //50%��ռ�ձ�
	state.enabled = false;
	ret = pwm_apply_state(data->pwm, &state);
	if (ret) {
		pr_err(" wbwmotor [wfp] failed to apply initial PWM state: %d\n",ret);
		return ;
	}

	gpio_direction_output(data->pwmen_gpio,0);
	gpio_direction_output(data->dir_gpio,0);
	gpio_direction_output(data->reset_gpio,0);

	return ;
}

static void pwm_stspin220_resume(struct pwm_bl_data *data)
{
	struct pwm_state state;
	int ret;

	stspin220_io_direction_set(data);

	pwm_get_state(data->pwm, &state);
	state.period = 104666;
	//state.period = 840000;
	state.duty_cycle = state.period/2; //50%��ռ�ձ�
	state.enabled = true;
	ret = pwm_apply_state(data->pwm, &state);
	if (ret) {
		pr_err(" wbwmotor [wfp] failed to apply initial PWM state: %d\n",ret);
		return ;
	}

	return ;
}

static int pwm_stspin220_probe(struct platform_device *pdev)
{
	struct pwm_bl_data *data;
	struct pwm_state state;
	int ret;

	pr_err(" wbwmotor %s: [wfp] pwm_stspin220_probe\n",__func__);

	data = devm_kzalloc(&pdev->dev, sizeof(*data), GFP_KERNEL);
	if (!data) {
		ret = -ENOMEM;
		goto err_alloc;
	}

	ret = pwm_stspin220_parse_dt(&pdev->dev, data);
	if (ret < 0) {
		dev_err(&pdev->dev, "[wfp] failed to find platform data\n");
		return ret;
	}
	data->pwm = devm_pwm_get(&pdev->dev, NULL);

	if (IS_ERR(data->pwm)) {
		pr_err(" wbwmotor [wfp] get pwm1 failed \n");
		ret = PTR_ERR(data->pwm);
		if (ret != -EPROBE_DEFER)
			dev_err(&pdev->dev, "[wfp] unable to request PWM\n");
		goto err_alloc;
	}

	dev_dbg(&pdev->dev, "[wfp] got pwm for stspin220\n");

	/* Sync up PWM state. */
	pwm_init_state(data->pwm, &state);

	data->avddsdio = regulator_get(&pdev->dev, "vddsdio");
	if (IS_ERR(data->avddsdio)) {
	 ret = PTR_ERR(data->avddsdio);
	 //return ret;
	}
	mdelay(5);
	ret = regulator_set_voltage(data->avddsdio,1800000,2000000);
        if (ret) {
            regulator_put(data->avddsdio);
            data->avddsdio = NULL;
			pr_err(" wbwmotor [wfp] Regulator set 2.8V failed avdd ret=%d\n", ret);
            return ret;
        }

	ret = regulator_enable(data->avddsdio);
	if (ret){
		pr_err(" wbwmotor [wfp] can not enable avdd 2.8V: %d", ret);
		return ret;
	}
	//#endif

	ret = stspin220_request_io_port(data);
	if (ret < 0) {
		pr_err(" wbwmotor [wfp] stspin220 request IO port failed.");
		return ret;
	}
	init_time();
	pwm_stspin220_resume(data);
	pwm_stop(data);

	#if 0
	ret = regulator_disable(data->avddsdio);
	if (ret){
		pr_err(" wbwmotor ktf can not disable avdd 2.8V: %d", ret);
		return ret;
	}
	#endif
	ddata = data;
/*
	hrtimer_init(&task_stop_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	task_stop_timer.function = task_stop_timer_func;
*/

	hrtimer_init(&task_pwm_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	task_pwm_timer.function = task_pwm_timer_func;

	ret = stspin220_node();

//	UP_TO_TOP_TIME = trip_timer[trip_mode];
//	UP_TO_MID_TIME = 1529790000;
//	UP_MID_TO_TOP_TIME = UP_TO_TOP_TIME - UP_TO_MID_TIME;
//	BACK_TOP_TO_HOME_TIME = UP_TO_TOP_TIME;
//	BACK_MID_TO_HOME_TIME = UP_TO_MID_TIME + 20000000;;
//	BACK_TOP_TO_MID_TIME = UP_TO_TOP_TIME - UP_TO_MID_TIME;
	wake_lock_init(&wakelock,WAKE_LOCK_SUSPEND,"motorlock");

/*
	ret = regulator_enable(data->avdd);
	if (ret){
		pr_err(" wbwmotor ktf can not enable avdd 2.8V: %d", ret);
		return ret;
	}

	//mdelay(10000);

	ret = regulator_disable(data->avdd);
	if (ret){
		pr_err(" wbwmotor ktf can not disable avdd 2.8V: %d", ret);
		return ret;
	}
*/
	pr_err(" wbwmotor %s: [wfp] pwm_stspin220_probe success\n",__func__);
	return 0;

err_alloc:
	pr_err(" wbwmotor %s: [wfp] pwm_stspin220_probe failed\n",__func__);
	pwm_stspin220_gpio_free(data);
	return ret;
}

static struct platform_driver pwm_stspin220_driver = {
	.driver		= {
		.name		= "pwm-stspin220",
		.of_match_table	= of_match_ptr(pwm_stspin220_of_match),
	},
	.probe		= pwm_stspin220_probe,
	//.remove		= pwm_stspin220_remove,
	//.shutdown	= pwm_stspin220_shutdown,
};

#if 0
static int __init pwm_stspin220_init(void)
{
	pr_err(" wbwmotor wfpwfp!!!!!!!!!\n");
	return platform_driver_register(&pwm_stspin220_driver);
}

static void __exit pwm_stspin220_exit(void)
{
	platform_driver_unregister(&pwm_stspin220_driver);
}


module_init(pwm_stspin220_init);
module_exit(pwm_stspin220_exit);
#endif

module_platform_driver(pwm_stspin220_driver);



MODULE_DESCRIPTION("PWM based stspin220 Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:pwm-stspin220");
