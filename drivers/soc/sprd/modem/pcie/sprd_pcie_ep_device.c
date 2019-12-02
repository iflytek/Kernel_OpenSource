/**
 * SPRD ep device driver in host side for Spreadtrum SoCs
 *
 * Copyright (C) 2018 Spreadtrum Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 of
 * the License as published by the Free Software Foundation.
 *
 * This program is used to control ep device driver in host side for
 * Spreadtrum SoCs.
 */

#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/bitops.h>
#include <linux/pci.h>
#include <linux/pci_regs.h>
#include <linux/pci_ids.h>
#include <linux/pcie-rc-sprd.h>
#include <linux/sched.h>
#include <uapi/linux/sched/types.h>
#include <linux/soc/sprd/sprd_pcie_ep_device.h>
#include <linux/workqueue.h>

#include "../include/sprd_pcie_resource.h"

#define DRV_MODULE_NAME		"sprd-pcie-ep-device"

enum dev_pci_barno {
	BAR_0 = 0,
	BAR_1,
	BAR_2,
	BAR_3,
	BAR_4,
	BAR_5,
	BAR_CNT
};

#define MAX_SUPPORT_IRQ	32
#define IPA_HW_IRQ_CNT 4
#define IPA_HW_IRQ_BASE 16
#define IPA_HW_IRQ_BASE_DEFECT 0

#define REQUEST_BASE_IRQ (IPA_HW_IRQ_BASE + IPA_HW_IRQ_CNT)
#define REQUEST_BASE_IRQ_DEFECT 16
#define REQUEST_MAX_IRQ (REQUEST_BASE_IRQ + PCIE_MSI_MAX_IRQ)

#ifdef CONFIG_SPRD_IPA_PCIE_WORKROUND
/* the bar0 and the bar1 are used for ipa */
#define IPA_MEM_BAR	BAR_0
#define IPA_REG_BAR	BAR_1
#define BAR_MIN		BAR_2
#else
#define BAR_MIN		BAR_0
#endif

/* the bar4 and the bar5 are specail bars */
#define BAR_MAX BAR_4

#define PCI_VENDOR_ID_SPRD	0x16c3
#define PCI_DEVICE_ID_SPRD_ORCA	0xabcd
#define PCI_CLASS_ID_SPRD_ORCA	0x80d00

/* Parameters for the waiting for iATU enabled routine */
#define LINK_WAIT_MAX_IATU_RETRIES	5
#define LINK_WAIT_IATU_MIN		9000
#define LINK_WAIT_IATU_MAX		10000

/* ep config bar bar4 , can config ep iatu reg and door bell reg */
#define EP_CFG_BAR	BAR_4
#define DOOR_BELL_BASE	0x00000
#define IATU_REG_BASE	0x10000

#define DOOR_BELL_ENABLE	0x10
#define DOOR_BELL_STATUS	0x14
/* one bit can indicate one irq , if stauts[i] & enable[i] , irq = i */
#define DOOR_BELL_IRQ_VALUE(irq)	BIT((irq))
#define DOOR_BELL_IRQ_CNT		32
#define IATU_MAX_REGION			8
#define PCIE_ATU_VIEWPORT		0x900
#define PCIE_ATU_CR1			0x904
#define PCIE_ATU_CR2			0x908
#define PCIE_ATU_LOWER_BASE		0x90c
#define PCIE_ATU_UPPER_BASE		0x910
#define PCIE_ATU_LIMIT			0x914
#define PCIE_ATU_LOWER_TARGET		0x918
#define PCIE_ATU_UPPER_TARGET		0x91c

#define PCIE_ATU_REGION_INBOUND		BIT(31)
#define PCIE_ATU_ENABLE			BIT(31)
#define PCIE_ATU_BAR_MODE_ENABLE	BIT(30)
#define PCIE_ATU_TYPE_MEM		0x0

#define PCIE_ATU_UNR_REGION_CTRL1	0x00
#define PCIE_ATU_UNR_REGION_CTRL2	0x04
#define PCIE_ATU_UNR_LOWER_BASE		0x08
#define PCIE_ATU_UNR_UPPER_BASE		0x0c
#define PCIE_ATU_UNR_LIMIT		0x10
#define PCIE_ATU_UNR_LOWER_TARGET	0x14
#define PCIE_ATU_UNR_UPPER_TARGET	0x18

/* bar4 + 0x10000 has map to ep base + 0x18000 ((0x3 << 15)) */
#define PCIE_ATU_IB_REGION(region) (((region) << 9) | (0x1 << 8))
#define PCIE_ATU_OB_REGION(region) ((region) << 9)

#define PCIE_SAVE_REGION_NUM	8
#define PCIE_SAVE_REG_NUM	8

struct sprd_ep_dev_notify {
	void  (*notify)(int event, void *data);
	void *data;
};

struct sprd_pci_ep_dev {
	struct pci_dev	*pdev;
	void __iomem	*cfg_base;	/* ep config bar base in rc */
	spinlock_t	irq_lock;	/* irq spinlock */
	spinlock_t	bar_lock;	/* bar spinlock */
	spinlock_t	set_irq_lock;	/* set irq spinlock */
	spinlock_t	set_bar_lock;	/* set bar spinlock */
	unsigned long	bar_res;

	u32	base_irq;
	u32	ipa_base_irq;
	u32	bak_irq_status;
	u8	iatu_unroll_enabled;
	u8	ep;
	u8	irq_cnt;
	bool	need_backup;

	struct resource	*bar[BAR_CNT];
	void __iomem	*bar_vir[BAR_MAX];
	void __iomem	*cpu_vir[BAR_MAX];
#ifdef CONFIG_SPRD_SIPA
	phys_addr_t		ipa_cpu_addr[BAR_MAX];
#endif

	int	event;
	struct work_struct	notify_work;
};

struct sprd_pci_ep_dev_save {
	bool		save_succ;
	unsigned long	bar_res;
	void __iomem	*bar_vir[BAR_MAX];
	void __iomem	*cpu_vir[BAR_MAX];
#ifdef CONFIG_SPRD_SIPA
	phys_addr_t	ipa_cpu_addr[BAR_MAX];
#endif
	u32	save_reg[PCIE_SAVE_REGION_NUM][PCIE_SAVE_REG_NUM];
};

static struct sprd_pci_ep_dev_save g_ep_save[PCIE_EP_NR];
static struct sprd_pci_ep_dev *g_ep_dev[PCIE_EP_NR];
static irq_handler_t ep_dev_handler[PCIE_EP_NR][PCIE_MSI_MAX_IRQ];
static void *ep_dev_handler_data[PCIE_EP_NR][PCIE_MSI_MAX_IRQ];
static struct sprd_ep_dev_notify g_ep_dev_notify[PCIE_EP_NR];

static int sprd_ep_dev_get_bar(int ep);
static int sprd_ep_dev_put_bar(int ep, int bar);
static int sprd_ep_dev_adjust_region(struct sprd_pci_ep_dev *ep_dev,
				     int bar, dma_addr_t *cpu_addr_ptr,
				     size_t *size_ptr, dma_addr_t *offset_ptr);
static int sprd_ep_dev_just_map_bar(struct sprd_pci_ep_dev *ep_dev, int bar,
				    dma_addr_t cpu_addr, size_t size);
static int sprd_ep_dev_just_unmap_bar(struct sprd_pci_ep_dev *ep_dev, int bar);
static void __iomem *sprd_ep_dev_map_bar(int ep, int bar,
					 dma_addr_t cpu_addr,
					 size_t size);
static int sprd_ep_dev_unmap_bar(int ep, int bar);
static void sprd_pci_ep_dev_backup(struct sprd_pci_ep_dev *ep_dev);

int sprd_ep_dev_register_notify(int ep,
				void (*notify)(int event, void *data),
				void *data)
{
	struct sprd_ep_dev_notify *dev_notify;

	if (ep >= PCIE_EP_NR)
		return -EINVAL;

	dev_notify = &g_ep_dev_notify[ep];
	dev_notify->notify = notify;
	dev_notify->data = data;

	return 0;
}
EXPORT_SYMBOL_GPL(sprd_ep_dev_register_notify);

int sprd_ep_dev_unregister_notify(int ep)
{
	struct sprd_ep_dev_notify *notify;

	if (ep >= PCIE_EP_NR)
		return -EINVAL;

	notify = &g_ep_dev_notify[ep];
	notify->notify = NULL;
	notify->data = NULL;

	return 0;
}
EXPORT_SYMBOL_GPL(sprd_ep_dev_unregister_notify);

int sprd_ep_dev_register_irq_handler(int ep, int irq,
				     irq_handler_t handler, void *data)
{
	struct sprd_pci_ep_dev *ep_dev;

	if (ep >= PCIE_EP_NR || irq >= PCIE_MSI_MAX_IRQ)
		return -EINVAL;

	ep_dev_handler[ep][irq] = handler;
	ep_dev_handler_data[ep][irq] = data;
	ep_dev = g_ep_dev[ep];

	if (handler && ep_dev &&
	    (BIT(irq) & ep_dev->bak_irq_status)) {
		ep_dev->bak_irq_status &= ~BIT(irq);
		handler(irq, data);
	}

	return 0;
}
EXPORT_SYMBOL_GPL(sprd_ep_dev_register_irq_handler);

int sprd_ep_dev_unregister_irq_handler(int ep, int irq)
{
	if (ep < PCIE_EP_NR && irq < PCIE_MSI_MAX_IRQ) {
		ep_dev_handler[ep][irq] = NULL;
		ep_dev_handler_data[ep][irq] = NULL;
		return 0;
	}

	return -EINVAL;
}
EXPORT_SYMBOL_GPL(sprd_ep_dev_unregister_irq_handler);

int sprd_ep_dev_register_irq_handler_ex(int ep,
					int from_irq,
					int to_irq,
					irq_handler_t handler,
					void *data)
{
	int i, ret;

	for (i = from_irq; i < to_irq + 1; i++) {
		ret = sprd_ep_dev_register_irq_handler(ep,
							i, handler, data);
		if (ret)
			return ret;
	}

	return 0;
}

int sprd_ep_dev_unregister_irq_handler_ex(int ep,
					  int from_irq,
					  int to_irq)
{
	int i, ret;

	for (i = from_irq; i < to_irq + 1; i++) {
		ret = sprd_ep_dev_unregister_irq_handler(ep, i);
		if (ret)
			return ret;
	}

	return 0;
}

void __iomem *sprd_ep_map_memory(int ep,
				 phys_addr_t cpu_addr,
				 size_t size)
{
	int bar;
	void __iomem *bar_addr;

	bar = sprd_ep_dev_get_bar(ep);
	if (bar < 0) {
		pr_err("%s: get bar err = %d\n", __func__, bar);
		return NULL;
	}

	bar_addr = sprd_ep_dev_map_bar(ep, bar, cpu_addr, size);
	if (!bar_addr) {
		pr_err("%s: map bar err\n", __func__);
		sprd_ep_dev_put_bar(ep, bar);
		return NULL;
	}

	return bar_addr;
}
EXPORT_SYMBOL_GPL(sprd_ep_map_memory);

void sprd_ep_unmap_memory(int ep, const void __iomem *bar_addr)
{
	int bar;
	struct sprd_pci_ep_dev *ep_dev;

	if (ep >= PCIE_EP_NR || !g_ep_dev[ep])
		return;

	ep_dev = g_ep_dev[ep];

	for (bar = 0; bar < BAR_MAX; bar++) {
		if (bar_addr == ep_dev->cpu_vir[bar]) {
			sprd_ep_dev_unmap_bar(ep, bar);
			sprd_ep_dev_put_bar(ep, bar);
			break;
		}
	}
}
EXPORT_SYMBOL_GPL(sprd_ep_unmap_memory);

#ifdef CONFIG_SPRD_SIPA
phys_addr_t sprd_ep_ipa_map(int type, phys_addr_t target_addr, size_t size)
{
	int bar, ep = PCIE_EP_MODEM;
	dma_addr_t offset;
	struct sprd_pci_ep_dev *ep_dev;
	struct pci_dev *pdev;
	struct device *dev;
	struct resource *res;

	ep_dev = g_ep_dev[ep];
	if (!ep_dev)
		return 0;

	ep_dev = g_ep_dev[ep];
	pdev = ep_dev->pdev;
	dev = &pdev->dev;
#ifdef CONFIG_SPRD_IPA_PCIE_WORKROUND
	bar = type == PCIE_IPA_TYPE_MEM ? IPA_MEM_BAR : IPA_REG_BAR;
#else
	bar = sprd_ep_dev_get_bar(ep);
	if (bar < 0) {
		dev_err(dev, "ep: ipa map, get bar err = %d\n", bar);
		return 0;
	}
#endif
	res = &pdev->resource[bar];

	dev_dbg(dev, "ep: ipa map type=%d, addr=0x%lx, size=0x%lx\n",
		type,
		(unsigned long)target_addr,
		(unsigned long)size);

	/* 1st, adjust the map region */
	if (sprd_ep_dev_adjust_region(ep_dev, bar, &target_addr,
				      &size, &offset))
		return 0;

	/* than, map bar */
	if (sprd_ep_dev_just_map_bar(ep_dev, bar, target_addr, size))
		return 0;

	/* save for unmap */
	ep_dev->ipa_cpu_addr[bar] = res->start + offset;

	/*  return the cpu phy address */
	return res->start + offset;
}

int sprd_ep_ipa_unmap(int type, const phys_addr_t cpu_addr)
{
	int bar, ep = PCIE_EP_MODEM;
	struct sprd_pci_ep_dev *ep_dev;
	struct pci_dev *pdev;
	struct resource *res;

	ep_dev = g_ep_dev[ep];
	if (!ep_dev)
		return -EINVAL;

	pdev = ep_dev->pdev;
	res = &pdev->resource[bar];

#ifdef CONFIG_SPRD_IPA_PCIE_WORKROUND
	bar = type == PCIE_IPA_TYPE_MEM ? IPA_MEM_BAR : IPA_REG_BAR;
	if (ep_dev->ipa_cpu_addr[bar] == cpu_addr) {
		ep_dev->ipa_cpu_addr[bar] = 0;
		return sprd_ep_dev_just_unmap_bar(ep_dev, bar);
	}
#else
	for (bar = 0; bar < BAR_MAX; bar++) {
		if (cpu_addr == ep_dev->ipa_cpu_addr[bar]) {
			sprd_ep_dev_put_bar(ep, bar);
			ep_dev->ipa_cpu_addr[bar] = 0;
			return sprd_ep_dev_just_unmap_bar(ep_dev, bar);
		}
	}
#endif

	return -EINVAL;
}
#endif

int sprd_ep_dev_raise_irq(int ep, int irq)
{
	struct pci_dev *pdev;
	struct device *dev;
	struct sprd_pci_ep_dev *ep_dev;
	void __iomem	*base;
	u32 value;

	if (ep >= PCIE_EP_NR || !g_ep_dev[ep])
		return -ENODEV;

	ep_dev = g_ep_dev[ep];
	pdev = ep_dev->pdev;
	dev = &pdev->dev;

	dev_dbg(dev, "ep: raise, ep=%d, irq0=%d\n", ep, irq);

	if (irq >= DOOR_BELL_IRQ_CNT)
		return -EINVAL;

	spin_lock(&ep_dev->set_irq_lock);
	base = ep_dev->cfg_base + DOOR_BELL_BASE;
	value = readl_relaxed(base + DOOR_BELL_STATUS);
	writel_relaxed(value | DOOR_BELL_IRQ_VALUE(irq),
		       base + DOOR_BELL_STATUS);
	spin_unlock(&ep_dev->set_irq_lock);

	return 0;
}
EXPORT_SYMBOL_GPL(sprd_ep_dev_raise_irq);

int sprd_ep_dev_clear_doolbell_irq(int ep, int irq)
{
	struct pci_dev *pdev;
	struct device *dev;
	struct sprd_pci_ep_dev *ep_dev;
	void __iomem	*base;
	u32 value;

	if (ep >= PCIE_EP_NR || !g_ep_dev[ep])
		return -ENODEV;

	ep_dev = g_ep_dev[ep];
	pdev = ep_dev->pdev;
	dev = &pdev->dev;

	dev_dbg(dev, "ep: clear doorbell, ep=%d, irq=%d\n", ep, irq);

	if (irq >= DOOR_BELL_IRQ_CNT)
		return -EINVAL;

	spin_lock(&ep_dev->set_irq_lock);
	base = ep_dev->cfg_base + DOOR_BELL_BASE;
	value = readl_relaxed(base + DOOR_BELL_STATUS);
	if (value & DOOR_BELL_IRQ_VALUE(irq))
		writel_relaxed(value & (~DOOR_BELL_IRQ_VALUE(irq)),
			       base + DOOR_BELL_STATUS);
	spin_unlock(&ep_dev->set_irq_lock);

	return 0;
}

int sprd_ep_dev_set_backup(int ep)
{
	struct sprd_pci_ep_dev *ep_dev;

	if (ep >= PCIE_EP_NR || !g_ep_dev[ep])
		return -ENODEV;

	ep_dev = g_ep_dev[ep];
	ep_dev->need_backup = true;

	/* backup once immediately. */
	sprd_pci_ep_dev_backup(ep_dev);

	return 0;
}

int sprd_ep_dev_clear_backup(int ep)
{
	struct sprd_pci_ep_dev *ep_dev;

	if (ep >= PCIE_EP_NR || !g_ep_dev[ep])
		return -ENODEV;

	ep_dev = g_ep_dev[ep];
	ep_dev->need_backup = false;

	return 0;
}

static inline u32 sprd_pci_ep_iatu_readl(struct sprd_pci_ep_dev *ep_dev,
					 u32 offset)
{
	return readl_relaxed(ep_dev->cfg_base + IATU_REG_BASE + offset);
}

static inline void sprd_pci_ep_iatu_writel(struct sprd_pci_ep_dev *ep_dev,
					   u32 offset, u32 value)
{
	writel_relaxed(value, ep_dev->cfg_base + IATU_REG_BASE + offset);
}

static int sprd_ep_dev_get_bar(int ep)
{
	int bar;
	int ret = -EBUSY;
	struct sprd_pci_ep_dev *ep_dev;

	if (ep >= PCIE_EP_NR || !g_ep_dev[ep])
		return -ENODEV;

	ep_dev = g_ep_dev[ep];
	spin_lock(&ep_dev->bar_lock);
	for (bar = BAR_MIN; bar < BAR_MAX; bar++) {
		if (ep_dev->bar[bar] && !test_bit(bar, &ep_dev->bar_res)) {
			set_bit(bar, &ep_dev->bar_res);
			ret = bar;
			break;
		}
	}
	spin_unlock(&ep_dev->bar_lock);

	return ret;
}

static int sprd_ep_dev_put_bar(int ep, int bar)
{
	int ret = -ENODEV;
	struct sprd_pci_ep_dev *ep_dev;

	if (ep >= PCIE_EP_NR || !g_ep_dev[ep])
		return -ENODEV;

	ep_dev = g_ep_dev[ep];
	spin_lock(&ep_dev->bar_lock);
	if (test_and_clear_bit(bar, &ep_dev->bar_res))
		ret = bar;
	spin_unlock(&ep_dev->bar_lock);

	return ret;
}

static int sprd_ep_dev_unr_set_bar(struct sprd_pci_ep_dev *ep_dev,
				   int bar,
				   dma_addr_t cpu_addr, size_t size)
{
	u32 retries, val;
	struct pci_dev *pdev = ep_dev->pdev;

	spin_lock(&ep_dev->set_bar_lock);

	/* bar n use region n to map, map to bar match mode */
	sprd_pci_ep_iatu_writel(ep_dev,
				PCIE_ATU_IB_REGION(bar) +
				PCIE_ATU_UNR_LOWER_TARGET,
				lower_32_bits(cpu_addr));
	sprd_pci_ep_iatu_writel(ep_dev,
				PCIE_ATU_IB_REGION(bar) +
				PCIE_ATU_UNR_UPPER_TARGET,
				upper_32_bits(cpu_addr));

	sprd_pci_ep_iatu_writel(ep_dev,
				PCIE_ATU_IB_REGION(bar) +
				PCIE_ATU_UNR_REGION_CTRL1,
				PCIE_ATU_TYPE_MEM);
	sprd_pci_ep_iatu_writel(ep_dev,
				PCIE_ATU_IB_REGION(bar) +
				PCIE_ATU_UNR_REGION_CTRL2,
				PCIE_ATU_ENABLE |
				PCIE_ATU_BAR_MODE_ENABLE | (bar << 8));

	spin_unlock(&ep_dev->set_bar_lock);

	/*
	 * Make sure ATU enable takes effect before any subsequent config
	 * and I/O accesses.
	 */
	for (retries = 0; retries < LINK_WAIT_MAX_IATU_RETRIES; retries++) {
		val = sprd_pci_ep_iatu_readl(ep_dev,
					     PCIE_ATU_IB_REGION(bar) +
					     PCIE_ATU_UNR_REGION_CTRL2);
		if (val & PCIE_ATU_ENABLE)
			return 0;

		dev_dbg(&pdev->dev,
			"ep: unr set bar[%d],  var = 0x%x\n",
			bar,
			val);
		/* wait a moment for polling ep atu enable bit */
		usleep_range(LINK_WAIT_IATU_MIN, LINK_WAIT_IATU_MAX);
	}

	return -EINVAL;
}

static int sprd_ep_dev_unr_clear_bar(struct sprd_pci_ep_dev *ep_dev, int bar)
{
	struct pci_dev *pdev = ep_dev->pdev;

	dev_dbg(&pdev->dev, "ep: unr clear map bar=%d\n", bar);

	spin_lock(&ep_dev->set_bar_lock);

	sprd_pci_ep_iatu_writel(ep_dev,
				PCIE_ATU_IB_REGION(bar) +
				PCIE_ATU_UNR_REGION_CTRL2,
				(u32)(~PCIE_ATU_ENABLE));
	spin_unlock(&ep_dev->set_bar_lock);

	return 0;
}

static int sprd_ep_dev_adjust_region(struct sprd_pci_ep_dev *ep_dev, int bar,
					     dma_addr_t *cpu_addr_ptr,
					     size_t *size_ptr,
					     dma_addr_t *offset_ptr)
{
	dma_addr_t cpu_addr, base, offset;
	resource_size_t bar_size, size;
	struct pci_dev *pdev = ep_dev->pdev;
	struct resource *res = &pdev->resource[bar];

	size = (resource_size_t)*size_ptr;
	cpu_addr = *cpu_addr_ptr;
	bar_size = resource_size(res);

	/* size must align with page */
	size = PAGE_ALIGN(size);

	/* base must be divisible by bar size for bar match mode */
	base = cpu_addr / bar_size * bar_size;
	offset = cpu_addr - base;
	size += PAGE_ALIGN(offset);

	/* size must < bar size  */
	if (size > bar_size) {
		dev_err(&pdev->dev,
			"bar[%d]:size=0x%llx > 0x%llx\n",
			bar, size, bar_size);
		return -EINVAL;
	}

	dev_dbg(&pdev->dev,
		"bar[%d]: base=0x%llx,size=0x%llx,offset=0x%llx\n",
		bar, base, size, offset);

	*size_ptr = (size_t)size;
	*offset_ptr = offset;
	*cpu_addr_ptr = base;

	return 0;
}

static int sprd_ep_dev_just_map_bar(struct sprd_pci_ep_dev *ep_dev, int bar,
			 dma_addr_t cpu_addr, size_t size)
{
	u32 retries, val;
	struct pci_dev *pdev;
	struct device *dev;

	pdev = ep_dev->pdev;
	dev = &pdev->dev;

	dev_dbg(dev, "ep: map bar=%d, addr=0x%lx, size=0x%lx\n",
		bar,
		(unsigned long)cpu_addr,
		(unsigned long)size);

	if (ep_dev->iatu_unroll_enabled)
		return sprd_ep_dev_unr_set_bar(ep_dev, bar, cpu_addr, size);

	spin_lock(&ep_dev->set_bar_lock);

	/* bar n use region n to map, map to bar match mode */
	sprd_pci_ep_iatu_writel(ep_dev,
				PCIE_ATU_VIEWPORT,
				PCIE_ATU_REGION_INBOUND | bar);
	sprd_pci_ep_iatu_writel(ep_dev, PCIE_ATU_LOWER_TARGET,
				lower_32_bits(cpu_addr));
	sprd_pci_ep_iatu_writel(ep_dev, PCIE_ATU_UPPER_TARGET,
				upper_32_bits(cpu_addr));
	sprd_pci_ep_iatu_writel(ep_dev,
				PCIE_ATU_CR1,
				PCIE_ATU_TYPE_MEM);
	sprd_pci_ep_iatu_writel(ep_dev,
				PCIE_ATU_CR2,
				PCIE_ATU_ENABLE |
				PCIE_ATU_BAR_MODE_ENABLE | (bar << 8));

	spin_unlock(&ep_dev->set_bar_lock);

	/*
	 * Make sure ATU enable takes effect
	 * before any subsequent config  and I/O accesses.
	 */
	for (retries = 0;
	     retries < LINK_WAIT_MAX_IATU_RETRIES;
	     retries++) {
		val = sprd_pci_ep_iatu_readl(ep_dev, PCIE_ATU_CR2);
		if (val & PCIE_ATU_ENABLE)
			return 0;

		/* wait a moment for polling ep atu enable bit */
		usleep_range(LINK_WAIT_IATU_MIN, LINK_WAIT_IATU_MAX);
	}

	return -EINVAL;
}

static int sprd_ep_dev_just_unmap_bar(struct sprd_pci_ep_dev *ep_dev, int bar)
{
	struct pci_dev *pdev;
	struct device *dev;

	pdev = ep_dev->pdev;
	dev = &pdev->dev;

	dev_dbg(dev, "ep: unmap bar = %d\n", bar);

	if (ep_dev->iatu_unroll_enabled)
		return sprd_ep_dev_unr_clear_bar(ep_dev, bar);

	spin_lock(&ep_dev->set_bar_lock);

	sprd_pci_ep_iatu_writel(ep_dev, PCIE_ATU_VIEWPORT,
				PCIE_ATU_REGION_INBOUND | bar);
	sprd_pci_ep_iatu_writel(ep_dev, PCIE_ATU_CR2,
				(u32)(~PCIE_ATU_ENABLE));

	spin_unlock(&ep_dev->set_bar_lock);

	return 0;
}

static void __iomem *sprd_ep_dev_map_bar(int ep, int bar,
			 dma_addr_t cpu_addr, size_t size)
{
	resource_size_t offset;
	struct pci_dev *pdev;
	struct device *dev;
	struct sprd_pci_ep_dev *ep_dev;
	void __iomem *bar_vir;
	struct resource *res;

	if (ep >= PCIE_EP_NR || !g_ep_dev[ep])
		return NULL;

	ep_dev = g_ep_dev[ep];
	pdev = ep_dev->pdev;
	dev = &pdev->dev;

	/* bar is be used */
	if (ep_dev->bar_vir[bar])
		return NULL;

	/* 1st, adjust the map region */
	if (sprd_ep_dev_adjust_region(ep_dev, bar, &cpu_addr, &size, &offset))
		return NULL;

	/* than, ioremap, if map failed, no need to set bar */
	res = &pdev->resource[bar];
	bar_vir = ioremap_nocache(res->start, size);
	if (!bar_vir) {
		dev_err(dev, "ep: map error, bar=%d, addr=0x%lx, size=0x%lx\n",
			bar,
			(unsigned long)cpu_addr,
			(unsigned long)size);
		return NULL;
	}

	if (sprd_ep_dev_just_map_bar(ep_dev, bar, cpu_addr, size)) {
		dev_err(dev, "ep: map bar =%d\n", bar);
		iounmap(ep_dev->bar_vir[bar]);
		return NULL;
	}

	ep_dev->bar_vir[bar] = (void __iomem *)bar_vir;
	ep_dev->cpu_vir[bar] = (void __iomem *)(bar_vir + offset);

	return ep_dev->cpu_vir[bar];
}

static int sprd_ep_dev_unmap_bar(int ep, int bar)
{
	struct pci_dev *pdev;
	struct device *dev;
	struct sprd_pci_ep_dev *ep_dev;

	if (ep >= PCIE_EP_NR || !g_ep_dev[ep])
		return -ENODEV;

	ep_dev = g_ep_dev[ep];
	pdev = ep_dev->pdev;
	dev = &pdev->dev;

	dev_dbg(dev, "ep: unmap bar = %d\n", bar);

	if (!ep_dev->bar_vir[bar])
		return -ENODEV;

	sprd_ep_dev_just_unmap_bar(ep_dev, bar);

	iounmap(ep_dev->bar_vir[bar]);
	ep_dev->bar_vir[bar] = NULL;
	ep_dev->cpu_vir[bar] = NULL;

	return 0;
}

#ifdef CONFIG_SPRD_SIPA
void sprd_ep_dev_unmap_ipa_bar(struct sprd_pci_ep_dev *ep_dev)
{
	int bar;

	for (bar = 0; bar < BAR_MAX; bar++)
		if (ep_dev->ipa_cpu_addr[bar]) {
			ep_dev->ipa_cpu_addr[bar] = 0;
			sprd_ep_dev_just_unmap_bar(ep_dev, bar);
		}
}
#endif

static irqreturn_t sprd_pci_ep_dev_irqhandler(int irq, void *dev_ptr)
{
	struct sprd_pci_ep_dev *ep_dev = dev_ptr;
	struct pci_dev *pdev = ep_dev->pdev;
	struct device *dev = &pdev->dev;
	irq_handler_t handler;

	dev_dbg(dev, "ep: irq handler. irq = %d\n",  irq);

	irq -= (pdev->irq + ep_dev->base_irq);
	if (irq >= PCIE_MSI_MAX_IRQ) {
		dev_err(dev, "ep: error, irq = %d", irq);
		return IRQ_HANDLED;
	}

	handler = ep_dev_handler[ep_dev->ep][irq];
	if (handler)
		handler(irq, ep_dev_handler_data[ep_dev->ep][irq]);
	else
		ep_dev->bak_irq_status |= BIT(irq);

	return IRQ_HANDLED;
}

static void sprd_pci_ep_save_reg(struct sprd_pci_ep_dev *ep_dev)
{
	int i, j;
	u32 (*save_reg)[PCIE_SAVE_REG_NUM];

	save_reg = g_ep_save[ep_dev->ep].save_reg;
	spin_lock(&ep_dev->set_bar_lock);
	for (i = 0; i < PCIE_SAVE_REGION_NUM; i += 2) {
		for (j = 0; j < PCIE_SAVE_REG_NUM; j++) {
			save_reg[i][j] =
				sprd_pci_ep_iatu_readl(ep_dev,
						       PCIE_ATU_OB_REGION(i) +
						       j * sizeof(u32));
			save_reg[i + 1][j] =
				sprd_pci_ep_iatu_readl(ep_dev,
						       PCIE_ATU_IB_REGION(i) +
						       j * sizeof(u32));
		}
	}
	spin_unlock(&ep_dev->set_bar_lock);
}

static void sprd_pci_ep_restore_reg(struct sprd_pci_ep_dev *ep_dev)
{
	int i, j;
	u32 retries, val;
	u32 (*save_reg)[PCIE_SAVE_REG_NUM];

	save_reg = g_ep_save[ep_dev->ep].save_reg;
	spin_lock(&ep_dev->set_bar_lock);
	for (i = 0; i < PCIE_SAVE_REGION_NUM; i += 2) {
		for (j = 0; j < PCIE_SAVE_REG_NUM; j++) {
			sprd_pci_ep_iatu_writel(ep_dev,
						PCIE_ATU_OB_REGION(i) +
						j * sizeof(u32),
						save_reg[i][j]);
			sprd_pci_ep_iatu_writel(ep_dev,
						PCIE_ATU_IB_REGION(i) +
						j * sizeof(u32),
						save_reg[i + 1][j]);
		}
	}
	spin_unlock(&ep_dev->set_bar_lock);

	/*
	 * Make sure ATU enable takes effect
	 * before any subsequent config  and I/O accesses.
	 */
	for (retries = 0;
		 retries < LINK_WAIT_MAX_IATU_RETRIES;
		 retries++) {
		val = sprd_pci_ep_iatu_readl(ep_dev, PCIE_ATU_CR2);
		if (val & PCIE_ATU_ENABLE)
			break;

		/* wait a moment for polling ep atu enable bit */
		usleep_range(LINK_WAIT_IATU_MIN, LINK_WAIT_IATU_MAX);
	}
}

static void sprd_pci_ep_dev_backup(struct sprd_pci_ep_dev *ep_dev)
{
	struct sprd_pci_ep_dev_save *ep_save;
	int i;

	ep_save = &g_ep_save[ep_dev->ep];

	/* save some member */
	ep_save->bar_res = ep_dev->bar_res;
	for (i = 0; i < BAR_MAX; i++) {
		ep_save->bar_vir[i] =  ep_dev->bar_vir[i];
		ep_save->cpu_vir[i] =  ep_dev->cpu_vir[i];
#ifdef CONFIG_SPRD_SIPA
		ep_save->ipa_cpu_addr[i] =  ep_dev->ipa_cpu_addr[i];
#endif
	}

	/* save ep reg */
	sprd_pci_ep_save_reg(ep_dev);

	ep_save->save_succ = true;
}

static void sprd_pci_ep_dev_restore(struct sprd_pci_ep_dev *ep_dev)
{
	struct sprd_pci_ep_dev_save *ep_save;
	int i;

	ep_save = &g_ep_save[ep_dev->ep];
	if (!ep_save->save_succ)
		return;

	/* save some member */
	ep_dev->bar_res = ep_save->bar_res;
	for (i = 0; i < BAR_MAX; i++) {
		ep_dev->bar_vir[i] =  ep_save->bar_vir[i];
		ep_dev->cpu_vir[i] =  ep_save->cpu_vir[i];
#ifdef CONFIG_SPRD_SIPA
		ep_dev->ipa_cpu_addr[i] = ep_save->ipa_cpu_addr[i];
#endif
	}

	/* save ep reg */
	sprd_pci_ep_restore_reg(ep_dev);

	ep_save->save_succ = false;
}

static void sprd_pci_ep_notify_fn(struct work_struct *work)
{
	struct sprd_ep_dev_notify *notify;
	struct sprd_pci_ep_dev *ep_dev = container_of(work,
						      struct sprd_pci_ep_dev,
						      notify_work);

	notify = &g_ep_dev_notify[ep_dev->ep];
	if (notify->notify)
		notify->notify(ep_dev->event, notify->data);
}

static int sprd_pci_ep_dev_probe(struct pci_dev *pdev,
				 const struct pci_device_id *ent)
{
	int i, err, irq_cnt;
	u32 val;
	enum dev_pci_barno bar;
	struct device *dev = &pdev->dev;
	struct sprd_pci_ep_dev *ep_dev;
	struct resource *res;

	if (pci_is_bridge(pdev))
		return -ENODEV;

	ep_dev = devm_kzalloc(dev, sizeof(*ep_dev), GFP_KERNEL);
	if (!ep_dev)
		return -ENOMEM;

	ep_dev->pdev = pdev;

	if (ent->device == PCI_DEVICE_ID_SPRD_ORCA)
		ep_dev->ep = PCIE_EP_MODEM;
	else {
		dev_err(dev, "ep: Cannot support ep device = 0x%x\n",
			ent->device);
		return -EINVAL;
	}

	err = pci_enable_device(pdev);
	if (err) {
		dev_err(dev, "ep: Cannot enable PCI device\n");
		return err;
	}

	err = pci_request_regions(pdev, DRV_MODULE_NAME);
	if (err) {
		dev_err(dev, "ep: Cannot obtain PCI resources\n");
		goto err_disable_pdev;
	}

	pci_set_master(pdev);

#ifdef PCI_IRQ_MSI
	irq_cnt = pci_alloc_irq_vectors(pdev,
				    1,
				    MAX_SUPPORT_IRQ,
				    PCI_IRQ_MSI);
#else
	irq_cnt = pci_enable_msi_range(pdev, 1, MAX_SUPPORT_IRQ);
#endif

	if (irq_cnt < REQUEST_MAX_IRQ) {
		err = irq_cnt < 0 ? irq_cnt : -EINVAL;
		dev_err(dev, "ep: failed to get MSI interrupts, err=%d\n", err);
		goto err_disable_msi;
	}

	ep_dev->irq_cnt = irq_cnt;
	dev_info(dev, "ep: request IRQ = %d, cnt =%d\n",
		 pdev->irq,
		 ep_dev->irq_cnt);

	if (sprd_pcie_is_defective_chip()) {
		ep_dev->base_irq = REQUEST_BASE_IRQ_DEFECT;
		ep_dev->ipa_base_irq = IPA_HW_IRQ_BASE_DEFECT;
	} else {
		ep_dev->base_irq = REQUEST_BASE_IRQ;
		ep_dev->ipa_base_irq = IPA_HW_IRQ_BASE;
	}

	for (i = ep_dev->base_irq; i < REQUEST_MAX_IRQ; i++) {
		err = devm_request_irq(dev, pdev->irq + i,
				       sprd_pci_ep_dev_irqhandler,
				       IRQF_SHARED, DRV_MODULE_NAME, ep_dev);
		if (err)
			dev_warn(dev,
				"ep: failed to request IRQ %d for MSI %d\n",
				pdev->irq + i, i + 1);
	}

#ifdef CONFIG_SPRD_IPA_PCIE_WORKROUND
	for (i = ep_dev->ipa_base_irq; i < IPA_HW_IRQ_CNT; i++) {
		err = devm_request_irq(dev, pdev->irq + i,
				       sprd_pci_ep_dev_irqhandler,
				       IRQF_SHARED, DRV_MODULE_NAME, ep_dev);
		if (!err)
			sprd_pcie_teardown_msi_irq(pdev->irq + i);
	}
#endif

	for (bar = BAR_0; bar <= BAR_5; bar++) {
		res = &pdev->resource[bar];
		dev_info(dev, "ep: BAR[%d] %pR\n", bar, res);
		/* only save mem bar */
		if (resource_type(res) == IORESOURCE_MEM)
			ep_dev->bar[bar] = res;
	}

	ep_dev->cfg_base = pci_ioremap_bar(pdev, EP_CFG_BAR);
	if (!ep_dev->cfg_base) {
		dev_err(dev, "ep: failed to map cfg bar.\n");
		err = -ENOMEM;
		goto err_free_irq;
	}

	/* clear all 32 bit door bell */
	writel_relaxed(0x0,
		       ep_dev->cfg_base + DOOR_BELL_BASE + DOOR_BELL_STATUS);

	pci_set_drvdata(pdev, ep_dev);
	pci_read_config_dword(ep_dev->pdev, PCIE_ATU_VIEWPORT, &val);
	/*
	 * this atu view port reg is 0xffffffff means that the ep device
	 * doesn't support atu view port, we need unroll iatu registers
	 */
	dev_info(dev, "ep: atu_view_port val = 0x%x", val);
	ep_dev->iatu_unroll_enabled = val == 0xffffffff;

	/* default , PCIE_EP_PROBE */
	ep_dev->event = PCIE_EP_PROBE;
	g_ep_dev[ep_dev->ep] = ep_dev;

	if (!ep_dev->bar[BAR_1] || !ep_dev->bar[BAR_3]) {
		/* only 2 bar, set PCIE_EP_PROBE_BEFORE_SPLIT_BAR */
		ep_dev->event = PCIE_EP_PROBE_BEFORE_SPLIT_BAR;
		dev_info(dev, "ep:bar not ready, wait the next probe!");
	}

	/* restore all the config */
	if (ep_dev->event == PCIE_EP_PROBE)
		sprd_pci_ep_dev_restore(ep_dev);

	/* start notify work */
	INIT_WORK(&ep_dev->notify_work, sprd_pci_ep_notify_fn);
	schedule_work(&ep_dev->notify_work);

	return 0;

err_free_irq:
	for (i = ep_dev->base_irq; i < REQUEST_MAX_IRQ; i++)
		devm_free_irq(&pdev->dev, pdev->irq + i, ep_dev);

#ifdef CONFIG_SPRD_IPA_PCIE_WORKROUND
	for (i = ep_dev->ipa_base_irq; i < IPA_HW_IRQ_CNT; i++)
		devm_free_irq(&pdev->dev, pdev->irq + i, ep_dev);
#endif

err_disable_msi:
	pci_disable_msi(pdev);
	pci_release_regions(pdev);

err_disable_pdev:
	pci_disable_device(pdev);

	return err;
}

static void sprd_pci_ep_dev_remove(struct pci_dev *pdev)
{
	u32 i;
	struct sprd_ep_dev_notify *notify;
	struct sprd_pci_ep_dev *ep_dev = pci_get_drvdata(pdev);

	cancel_work_sync(&ep_dev->notify_work);

	/* first notify PCIE_EP_REMOVE */
	notify = &g_ep_dev_notify[ep_dev->ep];
	if (notify->notify)
		notify->notify(PCIE_EP_REMOVE, notify->data);

	/* back up some config before remove */
	if (ep_dev->need_backup)
		sprd_pci_ep_dev_backup(ep_dev);

	if (ep_dev->cfg_base)
		iounmap(ep_dev->cfg_base);

	for (i = ep_dev->base_irq; i < REQUEST_MAX_IRQ; i++)
		devm_free_irq(&pdev->dev, pdev->irq + i, ep_dev);

#ifdef CONFIG_SPRD_IPA_PCIE_WORKROUND
	for (i = ep_dev->ipa_base_irq; i < IPA_HW_IRQ_CNT; i++)
		devm_free_irq(&pdev->dev, pdev->irq + i, ep_dev);
#endif

	pci_disable_msi(pdev);
	pci_release_regions(pdev);
	pci_disable_device(pdev);

	g_ep_dev[ep_dev->ep] = NULL;
	ep_dev->bar_res = 0;
}

static const struct pci_device_id sprd_pci_ep_dev_tbl[] = {
	{ PCI_DEVICE(PCI_VENDOR_ID_SPRD, PCI_DEVICE_ID_SPRD_ORCA) },
	{ }
};
MODULE_DEVICE_TABLE(pci, sprd_pci_ep_dev_tbl);

#ifdef CONFIG_PM_SLEEP
static int sprd_pci_ep_suspend(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);
	int rc;

	/* Exec pci PCI_D3cold one time */
	if (pdev->current_state != PCI_D0) {
		dev_info(dev, "done for pm %d\n", pdev->current_state);
		return 0;
	}

	/*
	 * TODO: The HAL will ask the shared memory layer whether D3 is allowed.
	 */

	/* Save the PCI configuration space of a device before suspending. */
	rc = pci_save_state(pdev);
	if (rc) {
		dev_err(dev, "pci_save_state error=%d\n", rc);
		return rc;
	}

	/* Set the power state of a PCI device.
	 * Transition a device to a new power state, using the device's PCI PM
	 * registers.
	 */
	rc = pci_set_power_state(pdev, PCI_D3cold);
	if (rc) {
		dev_err(dev, "pci_set_power_state error=%d\n", rc);
		return rc;
	}
	return 0;
}

static int sprd_pci_ep_resume(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);
	int rc;

	/* Set the power state of a PCI device. */
	rc = pci_set_power_state(pdev, PCI_D0);
	if (rc) {
		dev_err(dev, "pci_set_power_state error=%d\n", rc);
		return rc;
	}

	/* Restore the saved state of a PCI device. */
	pci_restore_state(pdev);

	/* TODO: The HAL shall inform that the device is active. */
	return 0;
}
#endif /* CONFIG_PM_SLEEP */

static const struct dev_pm_ops sprd_pci_ep_pm = {
	SET_SYSTEM_SLEEP_PM_OPS(sprd_pci_ep_suspend,
				sprd_pci_ep_resume)
};

static struct pci_driver sprd_pci_ep_dev_driver = {
	.name		= DRV_MODULE_NAME,
	.id_table	= sprd_pci_ep_dev_tbl,
	.probe		= sprd_pci_ep_dev_probe,
	.remove		= sprd_pci_ep_dev_remove,
	.driver		= {
		.pm = &sprd_pci_ep_pm,
	}
};
module_pci_driver(sprd_pci_ep_dev_driver);

MODULE_DESCRIPTION("SPRD PCI EP DEVICE HOST DRIVER");
MODULE_AUTHOR("Wenping Zhou <wenping.zhou@unisoc.com>");
MODULE_LICENSE("GPL v2");
