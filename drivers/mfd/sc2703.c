/*
 * SPDX-License-Identifier: GPL-2.0
 * Core MFD(Charger, ADC, Flash and GPIO) driver for SC2703
 *
 * Copyright (c) 2018 Dialog Semiconductor.
 * Copyright (C) 2021, Anhui Yunshu Zhihui Technology Co., Ltd.
 */

#include <linux/debugfs.h>
#include <linux/i2c.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/regmap.h>
#include <linux/of_platform.h>
#include <linux/mfd/core.h>
#include <linux/mfd/sc2703_regs.h>

#define DEBUG_ROOT_DIR		"sc2703_chg"
#define SC2703_NUM_IRQ_REGS	6

struct sc2703_pdata {
	int irq_base;
};

struct sc2703_data {
	struct device *dev;
	struct regmap *regmap;
	struct dentry *debugfs_root;
	int irq_base;
	u32 addr;
	u32 read_cnt;
};

enum sc2703_irq_defs {
	SC2703_IRQ_ADC_DONE = 0,
	SC2703_IRQ_GPI0,
	SC2703_IRQ_GPI1,
	SC2703_IRQ_GPI2,
	SC2703_IRQ_GPI3,
};

static bool sc2703_volatile_reg(struct device *dev, u32 reg)
{
	switch (reg) {
	case SC2703_STATUS_A:
	case SC2703_STATUS_B:
	case SC2703_STATUS_C:
	case SC2703_STATUS_D:
	case SC2703_STATUS_E:
	case SC2703_STATUS_F:
	case SC2703_STATUS_G:
	case SC2703_STATUS_H:
	case SC2703_STATUS_I:
	case SC2703_EVENT_A:
	case SC2703_EVENT_B:
	case SC2703_EVENT_C:
	case SC2703_EVENT_D:
	case SC2703_EVENT_E:
	case SC2703_EVENT_F:
	case SC2703_DCDC_CTRL_A:
	case SC2703_ADC_CTRL_A:
	case SC2703_ADC_RES_0:
	case SC2703_ADC_RES_1:
	case SC2703_ADC_RES_2:
	case SC2703_ADC_RES_3:
	case SC2703_ADC_RES_4:
	case SC2703_ADC_RES_5:
	case SC2703_CHG_TIMER_CTRL_B:
	case SC2703_CHG_TIMER_CTRL_C:
		return true;
	default:
		return false;
	}
}

static const struct regmap_config sc2703_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
	.max_register = SC2703_MAX_REG,
	.cache_type = REGCACHE_RBTREE,
	.volatile_reg = sc2703_volatile_reg,
};

/* Helper macro to automatically populate resource name */
#define SC2703_RES_IRQ_NAMED(_name)	\
	DEFINE_RES_IRQ_NAMED(SC2703_IRQ_##_name, #_name)

/* ADC IRQs */
static struct resource sc2703_adc_resources[] = {
	SC2703_RES_IRQ_NAMED(ADC_DONE),
};


static struct resource sc2703_gpio_resources[] = {
	SC2703_RES_IRQ_NAMED(GPI0),
	SC2703_RES_IRQ_NAMED(GPI1),
	SC2703_RES_IRQ_NAMED(GPI2),
	SC2703_RES_IRQ_NAMED(GPI3),
};

enum sc2703_dev_idx {
	SC2703_ADC_IDX = 0,
	SC2703_CHARGER_IDX,
	SC2703_GPIO_IDX,
};

static struct mfd_cell sc2703_devs[] = {
	[SC2703_ADC_IDX] = {
		.name = "sc2703-adc",
		.of_compatible = "sprd,sc2703-adc",
		.resources = sc2703_adc_resources,
		.num_resources = ARRAY_SIZE(sc2703_adc_resources),
	},

	[SC2703_CHARGER_IDX] = {
		.name = "sc2703-charger",
		.of_compatible = "sprd,sc2703-charger",
	},

	[SC2703_GPIO_IDX] = {
		.name = "sc2703-gpio",
		.of_compatible = "sprd,sc2703-gpio",
		.resources = sc2703_gpio_resources,
		.num_resources = ARRAY_SIZE(sc2703_gpio_resources),
	},
};

static int debugfs_addr_get(void *data, u64 *val)
{
	struct sc2703_data *sc2703 = data;

	*val = sc2703->addr;
	return 0;
}

static int debugfs_addr_set(void *data, u64 val)
{
	struct sc2703_data *sc2703 = data;

	sc2703->addr = (unsigned int)val;
	return 0;
}

static int debugfs_cnt_get(void *data, u64 *val)
{
	struct sc2703_data *sc2703 = data;

	*val = sc2703->read_cnt;
	return 0;
}

static int debugfs_cnt_set(void *data, u64 val)
{
	struct sc2703_data *sc2703 = data;

	sc2703->read_cnt = val;
	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(sc2703_addr,
			debugfs_addr_get, debugfs_addr_set, "%llu\n");
DEFINE_SIMPLE_ATTRIBUTE(sc2703_cnt,
			debugfs_cnt_get, debugfs_cnt_set, "%llu\n");

static int sc2703_debug_show(struct seq_file *m, void *private)
{
	struct sc2703_data *sc2703 = m->private;
	unsigned int reg = sc2703->addr;
	unsigned int val, i;

	seq_puts(m, "******************************\n");

	for (i = 0; i < sc2703->read_cnt; i++) {
		if (regmap_read(sc2703->regmap, reg, &val)) {
			seq_puts(m, "read i2c err!\n");
			return 0;
		}

		seq_printf(m, "reg:0x%x --- value:0x%x\n", reg, val);
		reg++;
	}

	seq_puts(m, "------------------------------\n");

	return 0;
}

static int sc2703_debug_open(struct inode *inode, struct file *file)
{
	return single_open(file, sc2703_debug_show, inode->i_private);
}

static ssize_t sc2703_debug_write(struct file *filep,
				  const char __user *buf,
				  size_t len,
				  loff_t *ppos)
{
	struct seq_file *m = filep->private_data;
	struct sc2703_data *sc2703 = m->private;
	u32 val;
	int ret;

	ret = kstrtouint_from_user(buf, len, 0, &val);
	if (ret)
		return ret;

	ret = regmap_write(sc2703->regmap, sc2703->addr, val);
	if (ret)
		return ret;

	return len;
}

static const struct file_operations sc2703_value_fops = {
	.open = sc2703_debug_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.write = sc2703_debug_write,
	.release = single_release,
};

static int sc2703_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	struct device *dev = &client->dev;
	struct sc2703_data *sc2703;
	struct sc2703_pdata *pdata = dev_get_platdata(&client->dev);
	struct regmap *regmap;
	u32 event_b, event_d;
	int ret;

	sc2703 = devm_kzalloc(dev, sizeof(*sc2703), GFP_KERNEL);
	if (!sc2703)
		return -ENOMEM;

	sc2703->dev = dev;
	i2c_set_clientdata(client, sc2703);

	if (pdata)
		sc2703->irq_base = pdata->irq_base;

	regmap = devm_regmap_init_i2c(client, &sc2703_regmap_config);
	if (IS_ERR(regmap)) {
		ret = PTR_ERR(regmap);
		dev_err(dev, "Failed to allocate register map: %d\n", ret);
		return ret;
	}
	sc2703->regmap = regmap;
	sc2703->read_cnt = 1;

	/* Report and clear POR related fault events before enabling IRQs */
	ret = regmap_read(regmap, SC2703_EVENT_B, &event_b);
	if (ret)
		return ret;

	if (event_b & SC2703_E_TJUNC_POR_MASK) {
		dev_info(dev, "Reset due to TJUNC POR event\n");
		ret = regmap_update_bits(regmap, SC2703_EVENT_B,
					 SC2703_E_TJUNC_POR_MASK,
					 SC2703_E_TJUNC_POR_MASK);
		if (ret)
			return ret;
	}

	ret = regmap_read(regmap, SC2703_EVENT_D, &event_d);
	if (ret)
		return ret;

	if (event_d & SC2703_E_VSYS_POR_MASK) {
		dev_info(dev, "Reset due to VSYS POR event\n");
		ret = regmap_update_bits(regmap, SC2703_EVENT_D,
					 SC2703_E_VSYS_POR_MASK,
					 SC2703_E_VSYS_POR_MASK);
		if (ret)
			return ret;
	}

	ret = devm_mfd_add_devices(dev, PLATFORM_DEVID_NONE, sc2703_devs,
			      ARRAY_SIZE(sc2703_devs), NULL,
			      sc2703->irq_base, NULL);
	if (ret) {
		dev_err(dev, "Failed to add child devices: %d\n", ret);
		return ret;
	}

	sc2703->debugfs_root = debugfs_create_dir(DEBUG_ROOT_DIR, NULL);
	if (!sc2703->debugfs_root) {
		dev_err(dev, "Failed to create sc2703 i2c debugfs directory\n");
		ret = -ENOENT;
		return ret;
	}

	debugfs_create_file("addr", S_IRUGO | S_IWUSR,
			    sc2703->debugfs_root, sc2703, &sc2703_addr);

	debugfs_create_file("read_cnt", S_IRUGO | S_IWUSR,
			    sc2703->debugfs_root, sc2703, &sc2703_cnt);

	debugfs_create_file("value", S_IRUGO | S_IWUSR,
			    sc2703->debugfs_root, sc2703, &sc2703_value_fops);

	return 0;

}

static const struct of_device_id sc2703_of_match[] = {
	{ .compatible = "sprd,sc2703", },
	{ }
};

MODULE_DEVICE_TABLE(of, sc2703_of_match);

static const struct i2c_device_id sc2703_i2c_id[] = {
	{ "sc2703", },
	{ }
};
MODULE_DEVICE_TABLE(i2c, sc2703_i2c_id);

static struct i2c_driver sc2703_driver = {
	.driver	= {
		.name	= "sc2703",
		.of_match_table = sc2703_of_match,
	},
	.probe		= sc2703_probe,
	.id_table	= sc2703_i2c_id,
};

static int __init sc2703_init(void)
{
	return i2c_add_driver(&sc2703_driver);
}
module_init(sc2703_init);

static void __exit sc2703_exit(void)
{
	i2c_del_driver(&sc2703_driver);
}
module_exit(sc2703_exit);

MODULE_DESCRIPTION("MFD Core Driver for SC2703");
MODULE_AUTHOR("Roy Im <Roy.Im.Opensource@diasemi.com>");
MODULE_LICENSE("GPL v2");
