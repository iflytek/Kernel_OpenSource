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
#include <linux/mfd/syscon.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/platform_device.h>
#include <linux/property.h>
#include <linux/soc/sprd/hwfeature.h>

#include "pcie-designware.h"
#include "pcie-sprd.h"

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

static int sprd_pcie_ep_raise_irq(struct dw_pcie_ep *ep,
				  enum pci_epc_irq_type type,
				  u8 interrupt_num)
{
	struct dw_pcie *pci = to_dw_pcie_from_ep(ep);

	switch (type) {
	case PCI_EPC_IRQ_LEGACY:
		/* TODO*/
		break;
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

	if (enable) {
		ret = sprd_pcie_syscon_setting(pdev,
					       "sprd,pcie-startup-syscons");
		if (ret < 0) {
			dev_err(&pdev->dev,
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
			dev_err(&pdev->dev,
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

static void sprd_pcie_ep_reinit(struct work_struct *work)
{
	struct sprd_pcie *ep = container_of(work, struct sprd_pcie,
					    reinit_work);
	struct dw_pcie *pci = ep->pci;
	struct platform_device *pdev = to_platform_device(pci->dev);
	int ret;
#ifdef CONFIG_PCIE_SPRD_SPLIT_BAR
	int val;
#endif

	ret = sprd_pcie_syscon_setting(pdev, "sprd,pcie-startup-syscons");
	if (ret < 0) {
		dev_err(&pdev->dev,
			"set pcie reinit syscons fail, return %d\n", ret);
		return;
	}

	dev_info(pci->dev, "%s.\n", __func__);

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
		dev_err(pci->dev, "link fail after perst\n");
	else {
		sprd_pcie_wakeup(pci, false);
		dw_pcie_ep_linkup(&pci->ep);
	}
}

static void sprd_pcie_ep_shutdown(struct work_struct *work)
{
	int ret;
	struct sprd_pcie *ep = container_of(work, struct sprd_pcie,
					    shutdown_work);
	struct dw_pcie *pci = ep->pci;
	struct platform_device *pdev = to_platform_device(pci->dev);
	struct device *dev = &pdev->dev;

	ret = sprd_pcie_syscon_setting(pdev, "sprd,pcie-shutdown-syscons");
	if (ret < 0)
		dev_err(&pdev->dev,
			"set pcie uninit syscons fail, return %d\n", ret);
	else
		dw_pcie_ep_unlink(&pci->ep);

	pm_relax(dev);
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
	struct device *dev = &pdev->dev;

	/* WORKAROUND: At present, if setting high-lever trigger, when intterupt
	 * occurs the value read from gpio is low lever occasinally. When
	 * setting low-level trigger has the similar problem. Temporarily read
	 * multiple times gpio value to ensure the value is consistent with the
	 * trigger level, and this workaround will be removed when the problem
	 * fixed.
	 */
	while (val != expect)
		val = gpiod_get_value(ep->gpiod_perst);

	if (!val) {
		irq_flags &= ~IRQF_TRIGGER_LOW;
		irq_flags |= IRQF_TRIGGER_HIGH;
	} else {
		irq_flags &= ~IRQF_TRIGGER_HIGH;
		irq_flags |= IRQF_TRIGGER_LOW;
	}

	irq_set_irq_type(irq, irq_flags);

	if (!val)
		queue_work(ep->wq, &ep->shutdown_work);
	else {
		pm_stay_awake(dev);
		queue_work(ep->wq, &ep->reinit_work);
	}

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
	int ret;

	ret = sprd_pcie_syscon_setting(pdev, "sprd,pcie-startup-syscons");
	if (ret < 0) {
		dev_err(dev, "get pcie syscons fail, return %d\n", ret);
		return ret;
	}

	ep = devm_kzalloc(dev, sizeof(*ep), GFP_KERNEL);
	if (!ep)
		return -ENOMEM;

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
					sprd_pcie_perst_irq, NULL,
					IRQF_TRIGGER_LOW,
					"pcie_perst", ep);

	if (ret < 0) {
		dev_err(dev, "cannot request perst irq\n");
		return ret;
	}

	ep->wq = create_singlethread_workqueue("perst_irq");
	if (!ep->wq)
		return -ENOMEM;

	INIT_WORK(&ep->reinit_work, sprd_pcie_ep_reinit);
	INIT_WORK(&ep->shutdown_work, sprd_pcie_ep_shutdown);

	device_init_wakeup(dev, true);

	ret = sprd_add_pcie_ep(ep, pdev);
	if (ret) {
		dev_err(dev, "cannot initialize ep host\n");
		destroy_workqueue(ep->wq);
		return ret;
	}

	return 0;
}

static int sprd_pcie_ep_remove(struct platform_device *pdev)
{
	struct sprd_pcie *ep = platform_get_drvdata(pdev);
	struct device *dev = &pdev->dev;

	destroy_workqueue(ep->wq);
	device_init_wakeup(dev, false);

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
