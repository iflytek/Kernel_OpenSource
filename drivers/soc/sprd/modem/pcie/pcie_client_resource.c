/*
 * Copyright (C) 2019 Spreadtrum Communications Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/pcie-epf-sprd.h>
#include <linux/sched.h>
#include <linux/sipc.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/wait.h>

#include "../include/sprd_pcie_resource.h"
#ifdef CONFIG_SPRD_SIPA
#include "pcie_sipa_res.h"
#endif

struct sprd_pci_res_notify {
	void	(*notify)(void *p);
	void	*data;
};

struct sprd_pcie_res {
	u32	dst;
	u32	state;
	u32	ep_fun;
	bool	request_wait;

#ifdef CONFIG_SPRD_SIPA
	void *sipa_res;
#endif

	/*
	 * in client(Orca), The PCIE module wll blocks the chip Deep,
	 * so we must get a wake lock when pcie work to avoid this situation:
	 * the system is deep, but the PCIE is still working.
	 */
	struct wakeup_source	ws;
	wait_queue_head_t		wait_pcie_ready;
	struct sprd_pci_res_notify	first_ready_notify;
};

static struct sprd_pcie_res *g_pcie_res[SIPC_ID_NR];

static void sprd_pcie_resource_first_ready_notify(struct sprd_pcie_res *res)
{
	void (*notify)(void *p);

	pr_info("pcie res: first ready.\n");

#ifdef CONFIG_SPRD_SIPA
	/*
	 * in client side, producer res id is SIPA_RM_RES_PROD_PCIE_EP,
	 * consumer res id is SIPA_RM_RES_CONS_WWAN_DL.
	 */
	res->sipa_res = pcie_sipa_res_create(res->dst,
					     SIPA_RM_RES_PROD_PCIE_EP,
					     SIPA_RM_RES_CONS_WWAN_DL);
	if (!res->sipa_res)
		pr_err("pcie res:create ipa res failed.\n");
#endif

	notify = res->first_ready_notify.notify;
	if (notify)
		notify(res->first_ready_notify.data);
}

static void pcie_resource_client_epf_notify(int event, void *private)
{
	struct sprd_pcie_res *res = (struct sprd_pcie_res *)private;

	switch (event) {
	case SPRD_EPF_BIND:
		pr_info("pcie res: epf be binded.\n");
		if (sprd_pcie_is_defective_chip())
			sprd_pci_epf_raise_irq(res->ep_fun,
					 PCIE_MSI_EP_READY_FOR_RESCAN);
		break;

	case SPRD_EPF_UNBIND:
		pr_info("pcie res: epf be unbinded.\n");
		break;

	case SPRD_EPF_REMOVE:
		pr_info("pcie res: epf be removed.\n");
		break;

	case SPRD_EPF_LINK_UP:
		pr_info("pcie res: epf linkup.\n");
		/* first ready notify */
		if (res->state == SPRD_PCIE_WAIT_FIRST_READY)
			sprd_pcie_resource_first_ready_notify(res);
		break;

	default:
		break;
	}
}

static irqreturn_t pcie_resource_client_irq_handler(int irq, void *private)
{
	struct sprd_pcie_res *res = (struct sprd_pcie_res *)private;

	if (irq == PCIE_DBEL_EP_SCANNED) {
		/* get a wakelock */
		__pm_stay_awake(&res->ws);

		if (res->request_wait) {
			res->request_wait = false;
			pr_info("pcie res: request msi to rc.\n");
			sprd_pci_epf_raise_irq(res->ep_fun,
					     PCIE_MSI_REQUEST_RES);
		}
		/* wakeup all blocked thread */
		pr_info("pcie res: scanned, wakup all.\n");
		res->state = SPRD_PCIE_SCANNED;
		wake_up_interruptible_all(&res->wait_pcie_ready);
	} else if (irq == PCIE_DBEL_EP_REMOVED) {
		pr_info("pcie res: removed.\n");
		res->state = SPRD_PCIE_REMOVED;

		/* relax a wakelock */
		__pm_relax(&res->ws);
	}

	return IRQ_HANDLED;
}

int sprd_pcie_resource_client_init(u32 dst, u32 ep_fun)
{
	struct sprd_pcie_res *res;

	if (dst >= SIPC_ID_NR)
		return -EINVAL;

	res = kzalloc(sizeof(*res), GFP_KERNEL);
	if (!res)
		return -ENOMEM;

	res->dst = dst;
	res->state = SPRD_PCIE_WAIT_FIRST_READY;
	res->ep_fun = ep_fun;

	wakeup_source_init(&res->ws, "pcie_res");

	init_waitqueue_head(&res->wait_pcie_ready);
	sprd_pci_epf_register_irq_handler_ex(res->ep_fun,
					     PCIE_DBEL_EP_SCANNED,
					     PCIE_DBEL_EP_REMOVED,
					     pcie_resource_client_irq_handler,
					     res);
	sprd_pci_epf_register_notify(res->ep_fun,
				     pcie_resource_client_epf_notify,
				     res);

	g_pcie_res[dst] = res;

	return 0;
}

int sprd_pcie_resource_trash(u32 dst)
{
	struct sprd_pcie_res *res;

	if (dst >= SIPC_ID_NR || !g_pcie_res[dst])
		return -EINVAL;

	res = g_pcie_res[dst];

#ifdef CONFIG_SPRD_SIPA
	if (res->sipa_res)
		pcie_sipa_res_destroy(res->sipa_res);
#endif

	sprd_pci_epf_unregister_irq_handler_ex(res->ep_fun,
		PCIE_DBEL_EP_SCANNED,
		PCIE_DBEL_EP_REMOVED);
	sprd_pci_epf_unregister_notify(res->ep_fun);

	kfree(res);
	g_pcie_res[dst] = NULL;

	return 0;
}

int sprd_pcie_wait_resource(u32 dst, int timeout)
{
	struct sprd_pcie_res *res;
	int ret, wait;
	unsigned long delay;

	if (dst >= SIPC_ID_NR || !g_pcie_res[dst])
		return -EINVAL;

	res = g_pcie_res[dst];

	/*  pcie ready, return succ immediately */
	if (res->state == SPRD_PCIE_SCANNED)
		return 0;

	if (timeout == 0)
		return -ETIME;

	if (timeout < 0) {
		wait = wait_event_interruptible(
						res->wait_pcie_ready,
						res->state == SPRD_PCIE_SCANNED
						);
		ret = wait;
	} else {
		/*
		 * timeout must add 1s,
		 * because the pcie rescan may took some time.
		 */
		delay = msecs_to_jiffies(timeout + 1000);
		wait = wait_event_interruptible_timeout(res->wait_pcie_ready,
							res->state ==
							SPRD_PCIE_SCANNED,
							delay);
		if (wait == 0)
			ret = -ETIME;
		else if (wait > 0)
			ret = 0;
		else
			ret = wait;
	}

	if (ret < 0 && ret != -ERESTARTSYS)
		pr_err("pcie res: wait resource, val=%d.\n", ret);

	return ret;
}

int sprd_pcie_request_resource(u32 dst)
{
	struct sprd_pcie_res *res;
	int ret = 0;

	if (dst >= SIPC_ID_NR || !g_pcie_res[dst])
		return -EINVAL;

	res = g_pcie_res[dst];

	if (res->state == SPRD_PCIE_SCANNED) {
		/*
		 * if pcie state is SCANNED, just send PCIE_MSI_REQUEST_RES
		 * to the host. After host receive res msi interrupt,
		 * it will increase one vote in modem power manger.
		 */
		pr_info("pcie res: send request msi to rc.\n");
		ret = sprd_pci_epf_raise_irq(res->ep_fun,
					     PCIE_MSI_REQUEST_RES);
	} else if (res->state == SPRD_PCIE_REMOVED) {
		res->state = SPRD_PCIE_WAIT_SCANNED;
		/*
		 * if pcie state is removed, poll wake_up singnal to host,
		 * and he host will rescan the pcie.
		 */
		pr_info("pcie res: send wakeup to rc.\n");
		ret = sprd_pci_epf_start(res->ep_fun);
	} else if (res->state == SPRD_PCIE_WAIT_FIRST_READY) {
		pr_info("pcie res: later send request msi to rc.\n");
		res->request_wait = true;
	} else {
		pr_err("pcie res: request res, state=%d.\n", res->state);
		ret = -EPERM;
	}

	return ret;
}

int sprd_pcie_release_resource(u32 dst)
{
	struct sprd_pcie_res *res;
	int ret = 0;

	if (dst >= SIPC_ID_NR || !g_pcie_res[dst])
		return -EINVAL;

	res = g_pcie_res[dst];

	if (res->state == SPRD_PCIE_SCANNED) {
		/*
		 * if pcie state is SCANNED, send PCIE_MSI_RELEASE_RES
		 * to the host, else, do nothing. After host receive res msi
		 * interrupt, it will decrease one vote in modem power manger,
		 * and if modem power manger is idle, the host will remove
		 * the pcie.
		 */
		pr_info("pcie res: send release msi to rc.\n");
		ret = sprd_pci_epf_raise_irq(res->ep_fun,
					     PCIE_MSI_RELEASE_RES);
	} else {
		pr_err("pcie res: release res state=%d.\n", res->state);
		ret = -EPERM;
	}

	return ret;
}

int sprd_register_pcie_resource_first_ready(u32 dst,
					    void (*notify)(void *p), void *data)
{
	struct sprd_pcie_res *res;

	if (dst >= SIPC_ID_NR || !g_pcie_res[dst])
		return -EINVAL;

	res = g_pcie_res[dst];

	res->first_ready_notify.data = data;
	res->first_ready_notify.notify = notify;

	return 0;
}

bool sprd_pcie_is_defective_chip(void)
{
	static bool first_read = true, defective;

	if (first_read) {
		first_read = false;
		defective = sprd_kproperty_chipid("UD710-AB") == 0;
	}

	return defective;
}
