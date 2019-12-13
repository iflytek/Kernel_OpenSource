/*
 * PCIe Endpoint controller driver for Spreadtrum SoCs
 *
 * Copyright (C) 2019 Spreadtrum corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/clk.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>
#include <linux/mfd/syscon.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/platform_device.h>
#include <linux/property.h>
#include <linux/soc/sprd/hwfeature.h>
#include <linux/wait.h>
#include <uapi/linux/sched/types.h>

#include "pcie-designware.h"
#include "pcie-sprd.h"

enum {
	SPRD_PCIE_REINIT_ACTION = 0,
	SPRD_PCIE_SHUTDOWN_ACTION = 1,
};

struct sprd_pcie_action {
	unsigned int action;
	struct list_head entry;
};

static DEFINE_SPINLOCK(pcie_action_lock);

/*
 * WORKAROUND: set CX_FLT_MASK_UR_POIS bit to fix pam ipa pcie access error
 * problem.
 */
static void sprd_pcie_setup_ep(struct dw_pcie_ep *ep)
{
	u32 val;
	struct dw_pcie *pci = to_dw_pcie_from_ep(ep);

	val = dw_pcie_readl_dbi(pci, PCIE_SYMBOL_TIMER_FILTER_1_OFF);
	val |= CX_FLT_MASK_UR_POIS;
	dw_pcie_writel_dbi(pci, PCIE_SYMBOL_TIMER_FILTER_1_OFF, val);
}

static void sprd_pcie_ep_init(struct dw_pcie_ep *ep)
{
	sprd_pcie_setup_ep(ep);
}

static int sprd_pcie_ep_raise_legacy_irq(struct dw_pcie_ep *ep, u8 irq_num)
{
	u16 cmd;
	int ret;
	struct dw_pcie *pci = to_dw_pcie_from_ep(ep);
	struct device *dev = pci->dev;
	struct platform_device *pdev = to_platform_device(dev);

	cmd = dw_pcie_readw_dbi(pci, PCI_COMMAND);
	if (cmd & PCI_COMMAND_INTX_DISABLE) {
		dev_err(dev,
			"can't generate INTx interrupts, please enable it\n");
		return -EINVAL;
	}

	ret = sprd_pcie_syscon_setting(pdev, "sprd,pcie-inta-assert-syscons");
	if (ret < 0) {
		dev_err(dev, "please add inta assert property in dts\n");
		return ret;
	}
	/*
	 * TODO: software can't sure the delay value, maybe it doesn't need
	 * to delay after asserting INTA
	 */
	mdelay(1);
	ret = sprd_pcie_syscon_setting(pdev, "sprd,pcie-inta-deassert-syscons");
	if (ret < 0) {
		dev_err(dev, "please add inta deassert property in dts\n");
		return ret;
	}

	return 0;
}

static int sprd_pcie_ep_raise_irq(struct dw_pcie_ep *ep,
				  enum pci_epc_irq_type type,
				  u8 interrupt_num)
{
	struct dw_pcie *pci = to_dw_pcie_from_ep(ep);

	switch (type) {
	case PCI_EPC_IRQ_LEGACY:
		return sprd_pcie_ep_raise_legacy_irq(ep, interrupt_num);
	case  PCI_EPC_IRQ_MSI:
		return dw_pcie_ep_raise_msi_irq(ep, interrupt_num);
	default:
		dev_err(pci->dev, "UNKNOWN IRQ type\n");
	}

	return 0;
}

static struct dw_pcie_ep_ops pcie_ep_ops = {
	.ep_init = sprd_pcie_ep_init,
	.raise_irq = sprd_pcie_ep_raise_irq,
};

#ifdef CONFIG_PCIE_SPRD_SPLIT_BAR
static int sprd_pcie_split_bar(struct dw_pcie *pci)
{
	u32 val, bar_offset;
	int i;

	for (i = 0; i < PCI_BAR_NUM; i++) {
		bar_offset = PCI_BASE_ADDRESS_0 + (i * 4);
		val = dw_pcie_readl_dbi(pci, bar_offset);
		if ((val & PCI_BASE_ADDRESS_MEM_TYPE_MASK) ==
		    PCI_BASE_ADDRESS_MEM_TYPE_64) {
			val &= ~PCI_BASE_ADDRESS_MEM_TYPE_MASK;
			val |= PCI_BASE_ADDRESS_MEM_TYPE_32;
			dw_pcie_dbi_ro_wr_en(pci);
			dw_pcie_writel_dbi(pci, bar_offset, val);
			dw_pcie_dbi_ro_wr_dis(pci);
			dw_pcie_writel_dbi2(pci, bar_offset + 4, PCI_BAR_EN);
		}
		val = dw_pcie_readl_dbi(pci, bar_offset);
		if ((val & PCI_BASE_ADDRESS_MEM_TYPE_MASK) ==
		    PCI_BASE_ADDRESS_MEM_TYPE_64){
			dev_err(pci->dev, "split bar fail\n");
			return -EINVAL;
		}
	}
	return 0;
}
#endif

/*
 * SPRD PCIe wakeup pin related registers cannot be operated after PCIe
 * controller EB  is disabled. So this func set the PCIe controller EB before
 * operate the wakeup pin.
 */
static int sprd_pcie_wakeup(struct dw_pcie *pci, bool enable)
{
	u32 val;
	int ret;
	struct platform_device *pdev = to_platform_device(pci->dev);

	dev_emerg(pci->dev, "%s enter, enable:%d\n", __func__, enable);
	if (enable) {
		ret = sprd_pcie_syscon_setting(pdev,
					       "sprd,pcie-startup-syscons");
		if (ret < 0) {
			dev_emerg(&pdev->dev,
				"set pcie enable syscons fail, return %d\n",
				ret);
			return ret;
		}
		val = dw_pcie_readl_dbi(pci, SPRD_PCIE_RST_CTRL);
		val &= ~SPRD_SOFT_WAKE;
		dw_pcie_writel_dbi(pci, SPRD_PCIE_RST_CTRL, val);
		ret = sprd_pcie_syscon_setting(pdev,
					       "sprd,pcie-shutdown-syscons");
		if (ret < 0) {
			dev_emerg(&pdev->dev,
				"set pcie disable syscons fail, return %d\n",
				ret);
			return ret;
		}
	} else {
		val = dw_pcie_readl_dbi(pci, SPRD_PCIE_RST_CTRL);
		val |= SPRD_SOFT_WAKE;
		dw_pcie_writel_dbi(pci, SPRD_PCIE_RST_CTRL, val);
	}

	return 0;
}

static void sprd_pcie_ep_reinit(struct sprd_pcie *ep)
{
	struct dw_pcie *pci = ep->pci;
	struct platform_device *pdev = to_platform_device(pci->dev);
	int ret;
#ifdef CONFIG_PCIE_SPRD_SPLIT_BAR
	int val;
#endif

	dev_emerg(pci->dev, "%s++\n", __func__);

	ret = sprd_pcie_syscon_setting(pdev, "sprd,pcie-startup-syscons");
	if (ret < 0) {
		dev_emerg(&pdev->dev,
			"set pcie reinit syscons fail, return %d\n", ret);
		return;
	}

	sprd_pcie_setup_ep(&pci->ep);
	dw_pcie_setup(pci);

#ifdef CONFIG_PCIE_SPRD_SPLIT_BAR
	if (sprd_kproperty_chipid("UD710-AB") == 0) {
		val = dw_pcie_readl_dbi(ep->pci,
					PCIE_SS_REG_BASE + PE0_GEN_CTRL_3);
		dw_pcie_writel_dbi(ep->pci, PCIE_SS_REG_BASE + PE0_GEN_CTRL_3,
				   (val & ~LTSSM_EN));

		ret = sprd_pcie_split_bar(ep->pci);
		if (ret) {
			dev_err(&pdev->dev, "failed to split bar\n");
			return;
		}

		val = dw_pcie_readl_dbi(ep->pci,
					PCIE_SS_REG_BASE + PE0_GEN_CTRL_3);
		dw_pcie_writel_dbi(ep->pci, PCIE_SS_REG_BASE + PE0_GEN_CTRL_3,
				   (val | LTSSM_EN));
	}
#endif

	ret = dw_pcie_wait_for_link(pci);
	if (ret < 0)
		dev_emerg(pci->dev, "link fail after perst\n");
	else {
		ep->is_powered = true;
		sprd_pcie_wakeup(pci, false);
		dw_pcie_ep_linkup(&pci->ep);
	}
}

static void sprd_pcie_ep_shutdown(struct sprd_pcie *ep)
{
	int ret;
	struct dw_pcie *pci = ep->pci;
	struct platform_device *pdev = to_platform_device(pci->dev);
	struct device *dev = &pdev->dev;

	dev_emerg(pci->dev, "%s++\n", __func__);
	ret = sprd_pcie_syscon_setting(pdev, "sprd,pcie-shutdown-syscons");
	if (ret < 0)
		dev_emerg(&pdev->dev,
			"set pcie uninit syscons fail, return %d\n", ret);
	else {
		ep->is_powered = false;
		dw_pcie_ep_unlink(&pci->ep);
	}

	pm_relax(dev);
}

static void sprd_pcie_do_actions(struct sprd_pcie *ep)
{
	struct sprd_pcie_action *pos, *tmp;

	list_for_each_entry_safe(pos, tmp, &ep->action_list, entry) {
		if (pos->action == SPRD_PCIE_REINIT_ACTION)
			sprd_pcie_ep_reinit(ep);
		else if (pos->action == SPRD_PCIE_SHUTDOWN_ACTION)
			sprd_pcie_ep_shutdown(ep);

		list_del(&pos->entry);
		kfree(pos);
	}
}

static int sprd_pcie_thread(void *data)
{
	struct sprd_pcie *ep = data;
	unsigned long flags;

	while (!kthread_should_stop()) {
		spin_lock_irqsave(&pcie_action_lock, flags);
		wait_event_cmd(ep->action_wait,
			       !list_empty(&ep->action_list),
			       spin_unlock_irqrestore(&pcie_action_lock, flags),
			       spin_lock_irqsave(&pcie_action_lock, flags));
		sprd_pcie_do_actions(ep);
		spin_unlock_irqrestore(&pcie_action_lock, flags);
	}

	return 0;
}

static int sprd_pcie_add_action(struct sprd_pcie *ep, unsigned int action)
{
	struct sprd_pcie_action *pos;
	unsigned long flags;

	pos = kzalloc(sizeof(*pos), GFP_KERNEL);
	if (!pos)
		return -ENOMEM;

	pos->action = action;
	spin_lock_irqsave(&pcie_action_lock, flags);
	list_add_tail(&pos->entry, &ep->action_list);
	spin_unlock_irqrestore(&pcie_action_lock, flags);

	wake_up_interruptible_all(&ep->action_wait);

	return 0;
}

/*
 * PERST# (high active)from root complex to wake up CP.
 *
 * When CP is in deep state, an root complex can wakeup CP by pulling the perst
 * signal.
 */
static irqreturn_t sprd_pcie_perst_irq(int irq, void *data)
{
	struct sprd_pcie *ep = data;
	int val = gpiod_get_value(ep->gpiod_perst);
	u32 irq_flags = irq_get_trigger_type(irq);
	u32 expect = (irq_flags & IRQF_TRIGGER_HIGH) ? 1 : 0;
	struct dw_pcie *pci = ep->pci;
	struct platform_device *pdev = to_platform_device(pci->dev);
	int i;

	disable_irq_nosync(irq);
	dev_emerg(&pdev->dev,
		  "pci perst irq enter ori val: %d\n", val);

	/* WORKAROUND: At present, if setting high-lever trigger, when intterupt
	 * occurs the value read from gpio is low lever occasinally. When
	 * setting low-level trigger has the similar problem. Temporarily read
	 * multiple times gpio value to ensure the value is consistent with the
	 * trigger level, and this workaround will be removed when the problem
	 * fixed.
	 */
	for (i = 0; i < 100; i++) {
		if (val != expect)
			val = gpiod_get_value(ep->gpiod_perst);
		else {
			if (!val) {
				irq_flags &= ~IRQF_TRIGGER_LOW;
				irq_flags |= IRQF_TRIGGER_HIGH;
			} else {
				irq_flags &= ~IRQF_TRIGGER_HIGH;
				irq_flags |= IRQF_TRIGGER_LOW;
			}
			irq_set_irq_type(irq, irq_flags);

			return IRQ_WAKE_THREAD;
		}
	}

	dev_emerg(&pdev->dev,
		  "pci perst irq cannot get expect val: %d, expect: %d\n",
		  val, expect);

	return IRQ_NONE;
}

static irqreturn_t sprd_pcie_perst_thread_irq(int irq, void *data)
{
	struct sprd_pcie *ep = data;
	int val = gpiod_get_value(ep->gpiod_perst);
	struct dw_pcie *pci = ep->pci;
	struct platform_device *pdev = to_platform_device(pci->dev);
	struct device *dev = &pdev->dev;

	dev_emerg(&pdev->dev,
		  "pci perst thread irq queue val: %d\n", val);
	if (!val)
		sprd_pcie_add_action(ep, SPRD_PCIE_SHUTDOWN_ACTION);
	else {
		pm_stay_awake(dev);
		ep->is_powered = true;
		sprd_pcie_add_action(ep, SPRD_PCIE_REINIT_ACTION);
	}
	enable_irq(irq);

	return IRQ_HANDLED;
}

static int sprd_add_pcie_ep(struct sprd_pcie *sprd_pcie,
			    struct platform_device *pdev)
{
	int ret;
	struct dw_pcie_ep *ep;
	struct resource *res;
	struct device *dev = &pdev->dev;
	struct dw_pcie *pci = sprd_pcie->pci;

	dev_emerg(pci->dev, "%s++\n", __func__);
	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "dbi");
	pci->dbi_base = devm_ioremap(dev, res->start, resource_size(res));
	if (!pci->dbi_base)
		return -ENOMEM;

	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "dbi2");
	pci->dbi_base2 = devm_ioremap(dev, res->start, resource_size(res));
	if (!pci->dbi_base2)
		return -ENOMEM;

	ep = &pci->ep;
	ep->ops = &pcie_ep_ops;
	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "addr_space");
	if (!res) {
		dev_err(dev, "pci can't get addr space\n");
		return -EINVAL;
	}

	ep->phys_base = res->start;
	ep->addr_size = resource_size(res);

	ret = dw_pcie_ep_init(ep);
	if (ret) {
		dev_err(dev, "failed to initialize endpoint\n");
		return ret;
	}

	return 0;
}

static int sprd_pcie_establish_link(struct dw_pcie *pci)
{
	struct sprd_pcie *ep = dev_get_drvdata(pci->dev);

	/* if ep is already powered on,  unallowed operation */
	if (ep->is_powered)
		return -EPERM;

	sprd_pcie_wakeup(pci, true);
	return 0;
}

static void sprd_pcie_stop_link(struct dw_pcie *pci)
{
	/* TODO */
}

static const struct dw_pcie_ops dw_pcie_ops = {
	.start_link = sprd_pcie_establish_link,
	.stop_link = sprd_pcie_stop_link,
};

static int sprd_pcie_ep_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct dw_pcie *pci;
	struct sprd_pcie *ep;
	struct sched_param param = {.sched_priority = 10};
	int ret;

	ret = sprd_pcie_syscon_setting(pdev, "sprd,pcie-startup-syscons");
	if (ret < 0) {
		dev_err(dev, "get pcie syscons fail, return %d\n", ret);
		return ret;
	}

	ep = devm_kzalloc(dev, sizeof(*ep), GFP_KERNEL);
	if (!ep)
		return -ENOMEM;

	ep->is_powered = true;

	pci = devm_kzalloc(dev, sizeof(*pci), GFP_KERNEL);
	if (!pci)
		return -ENOMEM;

	pci->dev = dev;
	pci->ops = &dw_pcie_ops;
	ep->pci = pci;

	platform_set_drvdata(pdev, ep);

	ep->gpiod_perst =
		devm_gpiod_get_index(dev, "pcie-perst", 0, GPIOD_IN);
	if (IS_ERR(ep->gpiod_perst)) {
		dev_err(dev, "Please set pcie-perst gpio in DTS\n");
		return PTR_ERR(ep->gpiod_perst);
	}

	ep->perst_irq = gpiod_to_irq(ep->gpiod_perst);
	if (ep->perst_irq < 0) {
		dev_err(dev, "cannot get perst irq\n");
		return ep->perst_irq;
	}

	ret = devm_request_threaded_irq(dev, ep->perst_irq,
					sprd_pcie_perst_irq,
					sprd_pcie_perst_thread_irq,
					IRQF_TRIGGER_LOW | IRQF_NO_SUSPEND,
					"pcie_perst", ep);
	enable_irq_wake(ep->wakeup_irq);

	if (ret < 0) {
		dev_err(dev, "cannot request perst irq\n");
		return ret;
	}
	enable_irq_wake(ep->perst_irq);

	ret = sprd_add_pcie_ep(ep, pdev);
	if (ret) {
		dev_err(dev, "cannot initialize ep host\n");
		return ret;
	}

	init_waitqueue_head(&ep->action_wait);
	INIT_LIST_HEAD(&ep->action_list);

	ep->action_thread = kthread_create(sprd_pcie_thread, ep,
					   "sprd_pcie_ep");
	if (IS_ERR(ep->action_thread)) {
		dev_err(dev, "Failed to create kthread\n");
		return PTR_ERR(ep->action_thread);
	}
	/* set the thread as a real time thread, and its priority is 10 */
	sched_setscheduler(ep->action_thread, SCHED_FIFO, &param);
	wake_up_process(ep->action_thread);

	return 0;
}

static int sprd_pcie_ep_remove(struct platform_device *pdev)
{
	struct sprd_pcie *ep = platform_get_drvdata(pdev);
	struct sprd_pcie_action *pos, *tmp;

	if (!IS_ERR(ep->action_thread))
		kthread_stop(ep->action_thread);

	list_for_each_entry_safe(pos, tmp, &ep->action_list, entry) {
		list_del(&pos->entry);
		kfree(pos);
	}

	return 0;
}

static const struct of_device_id sprd_pcie_ep_of_match[] = {
	{
		.compatible = "sprd,pcie-ep",
	},
	{},
};
MODULE_DEVICE_TABLE(of, sprd_pcie_ep_of_match);

static struct platform_driver sprd_pcie_ep_driver = {
	.probe = sprd_pcie_ep_probe,
	.remove = sprd_pcie_ep_remove,
	.driver = {
		.name = "sprd-pcie-ep",
		.suppress_bind_attrs = true,
		.of_match_table = sprd_pcie_ep_of_match,
	},
};

module_platform_driver(sprd_pcie_ep_driver);

MODULE_DESCRIPTION("Spreadtrum pcie ep controller driver");
MODULE_LICENSE("GPL");
