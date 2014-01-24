/*
 * Network-device interface management.
 *
 * Copyright (c) 2004-2005, Keir Fraser
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation; or, when distributed
 * separately from the Linux kernel or incorporated into other
 * software packages, subject to the following license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this source file (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "common.h"

<<<<<<< HEAD
#include <linux/ethtool.h>
#include <linux/rtnetlink.h>
#include <linux/if_vlan.h>
=======
#include <linux/kthread.h>
#include <linux/ethtool.h>
#include <linux/rtnetlink.h>
#include <linux/if_vlan.h>
#include <linux/vmalloc.h>
>>>>>>> refs/remotes/origin/master

#include <xen/events.h>
#include <asm/xen/hypercall.h>

#define XENVIF_QUEUE_LENGTH 32
<<<<<<< HEAD

void xenvif_get(struct xenvif *vif)
{
	atomic_inc(&vif->refcnt);
}

void xenvif_put(struct xenvif *vif)
{
	if (atomic_dec_and_test(&vif->refcnt))
		wake_up(&vif->waiting_to_free);
}

int xenvif_schedulable(struct xenvif *vif)
{
	return netif_running(vif->dev) && netif_carrier_ok(vif->dev);
}

static int xenvif_rx_schedulable(struct xenvif *vif)
{
	return xenvif_schedulable(vif) && !xen_netbk_rx_ring_full(vif);
}

static irqreturn_t xenvif_interrupt(int irq, void *dev_id)
{
	struct xenvif *vif = dev_id;

	if (vif->netbk == NULL)
		return IRQ_NONE;

	xen_netbk_schedule_xenvif(vif);

=======
#define XENVIF_NAPI_WEIGHT  64

int xenvif_schedulable(struct xenvif *vif)
{
	return netif_running(vif->dev) && netif_carrier_ok(vif->dev);
}

static int xenvif_rx_schedulable(struct xenvif *vif)
{
	return xenvif_schedulable(vif) && !xenvif_rx_ring_full(vif);
}

static irqreturn_t xenvif_tx_interrupt(int irq, void *dev_id)
{
	struct xenvif *vif = dev_id;

	if (RING_HAS_UNCONSUMED_REQUESTS(&vif->tx))
		napi_schedule(&vif->napi);

	return IRQ_HANDLED;
}

static int xenvif_poll(struct napi_struct *napi, int budget)
{
	struct xenvif *vif = container_of(napi, struct xenvif, napi);
	int work_done;

	work_done = xenvif_tx_action(vif, budget);

	if (work_done < budget) {
		int more_to_do = 0;
		unsigned long flags;

		/* It is necessary to disable IRQ before calling
		 * RING_HAS_UNCONSUMED_REQUESTS. Otherwise we might
		 * lose event from the frontend.
		 *
		 * Consider:
		 *   RING_HAS_UNCONSUMED_REQUESTS
		 *   <frontend generates event to trigger napi_schedule>
		 *   __napi_complete
		 *
		 * This handler is still in scheduled state so the
		 * event has no effect at all. After __napi_complete
		 * this handler is descheduled and cannot get
		 * scheduled again. We lose event in this case and the ring
		 * will be completely stalled.
		 */

		local_irq_save(flags);

		RING_FINAL_CHECK_FOR_REQUESTS(&vif->tx, more_to_do);
		if (!more_to_do)
			__napi_complete(napi);

		local_irq_restore(flags);
	}

	return work_done;
}

static irqreturn_t xenvif_rx_interrupt(int irq, void *dev_id)
{
	struct xenvif *vif = dev_id;

>>>>>>> refs/remotes/origin/master
	if (xenvif_rx_schedulable(vif))
		netif_wake_queue(vif->dev);

	return IRQ_HANDLED;
}

<<<<<<< HEAD
=======
static irqreturn_t xenvif_interrupt(int irq, void *dev_id)
{
	xenvif_tx_interrupt(irq, dev_id);
	xenvif_rx_interrupt(irq, dev_id);

	return IRQ_HANDLED;
}

>>>>>>> refs/remotes/origin/master
static int xenvif_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct xenvif *vif = netdev_priv(dev);

	BUG_ON(skb->dev != dev);

<<<<<<< HEAD
	if (vif->netbk == NULL)
=======
	/* Drop the packet if vif is not ready */
	if (vif->task == NULL)
>>>>>>> refs/remotes/origin/master
		goto drop;

	/* Drop the packet if the target domain has no receive buffers. */
	if (!xenvif_rx_schedulable(vif))
		goto drop;

	/* Reserve ring slots for the worst-case number of fragments. */
<<<<<<< HEAD
	vif->rx_req_cons_peek += xen_netbk_count_skb_slots(vif, skb);
	xenvif_get(vif);

	if (vif->can_queue && xen_netbk_must_stop_queue(vif))
		netif_stop_queue(dev);

	xen_netbk_queue_tx_skb(vif, skb);
=======
	vif->rx_req_cons_peek += xenvif_count_skb_slots(vif, skb);

	if (vif->can_queue && xenvif_must_stop_queue(vif))
		netif_stop_queue(dev);

	xenvif_queue_tx_skb(vif, skb);
>>>>>>> refs/remotes/origin/master

	return NETDEV_TX_OK;

 drop:
	vif->dev->stats.tx_dropped++;
	dev_kfree_skb(skb);
	return NETDEV_TX_OK;
}

<<<<<<< HEAD
void xenvif_receive_skb(struct xenvif *vif, struct sk_buff *skb)
{
	netif_rx_ni(skb);
}

=======
>>>>>>> refs/remotes/origin/master
void xenvif_notify_tx_completion(struct xenvif *vif)
{
	if (netif_queue_stopped(vif->dev) && xenvif_rx_schedulable(vif))
		netif_wake_queue(vif->dev);
}

static struct net_device_stats *xenvif_get_stats(struct net_device *dev)
{
	struct xenvif *vif = netdev_priv(dev);
	return &vif->dev->stats;
}

static void xenvif_up(struct xenvif *vif)
{
<<<<<<< HEAD
	xen_netbk_add_xenvif(vif);
	enable_irq(vif->irq);
	xen_netbk_check_rx_xenvif(vif);
=======
	napi_enable(&vif->napi);
	enable_irq(vif->tx_irq);
	if (vif->tx_irq != vif->rx_irq)
		enable_irq(vif->rx_irq);
	xenvif_check_rx_xenvif(vif);
>>>>>>> refs/remotes/origin/master
}

static void xenvif_down(struct xenvif *vif)
{
<<<<<<< HEAD
	disable_irq(vif->irq);
	del_timer_sync(&vif->credit_timeout);
	xen_netbk_deschedule_xenvif(vif);
	xen_netbk_remove_xenvif(vif);
=======
	napi_disable(&vif->napi);
	disable_irq(vif->tx_irq);
	if (vif->tx_irq != vif->rx_irq)
		disable_irq(vif->rx_irq);
	del_timer_sync(&vif->credit_timeout);
>>>>>>> refs/remotes/origin/master
}

static int xenvif_open(struct net_device *dev)
{
	struct xenvif *vif = netdev_priv(dev);
	if (netif_carrier_ok(dev))
		xenvif_up(vif);
	netif_start_queue(dev);
	return 0;
}

static int xenvif_close(struct net_device *dev)
{
	struct xenvif *vif = netdev_priv(dev);
	if (netif_carrier_ok(dev))
		xenvif_down(vif);
	netif_stop_queue(dev);
	return 0;
}

static int xenvif_change_mtu(struct net_device *dev, int mtu)
{
	struct xenvif *vif = netdev_priv(dev);
	int max = vif->can_sg ? 65535 - VLAN_ETH_HLEN : ETH_DATA_LEN;

	if (mtu > max)
		return -EINVAL;
	dev->mtu = mtu;
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static u32 xenvif_fix_features(struct net_device *dev, u32 features)
=======
static netdev_features_t xenvif_fix_features(struct net_device *dev,
	netdev_features_t features)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static netdev_features_t xenvif_fix_features(struct net_device *dev,
	netdev_features_t features)
>>>>>>> refs/remotes/origin/master
{
	struct xenvif *vif = netdev_priv(dev);

	if (!vif->can_sg)
		features &= ~NETIF_F_SG;
<<<<<<< HEAD
	if (!vif->gso && !vif->gso_prefix)
		features &= ~NETIF_F_TSO;
	if (!vif->csum)
		features &= ~NETIF_F_IP_CSUM;
=======
	if (~(vif->gso_mask | vif->gso_prefix_mask) & GSO_BIT(TCPV4))
		features &= ~NETIF_F_TSO;
	if (~(vif->gso_mask | vif->gso_prefix_mask) & GSO_BIT(TCPV6))
		features &= ~NETIF_F_TSO6;
	if (!vif->ip_csum)
		features &= ~NETIF_F_IP_CSUM;
	if (!vif->ipv6_csum)
		features &= ~NETIF_F_IPV6_CSUM;
>>>>>>> refs/remotes/origin/master

	return features;
}

static const struct xenvif_stat {
	char name[ETH_GSTRING_LEN];
	u16 offset;
} xenvif_stats[] = {
	{
		"rx_gso_checksum_fixup",
		offsetof(struct xenvif, rx_gso_checksum_fixup)
	},
};

static int xenvif_get_sset_count(struct net_device *dev, int string_set)
{
	switch (string_set) {
	case ETH_SS_STATS:
		return ARRAY_SIZE(xenvif_stats);
	default:
		return -EINVAL;
	}
}

static void xenvif_get_ethtool_stats(struct net_device *dev,
				     struct ethtool_stats *stats, u64 * data)
{
	void *vif = netdev_priv(dev);
	int i;

	for (i = 0; i < ARRAY_SIZE(xenvif_stats); i++)
		data[i] = *(unsigned long *)(vif + xenvif_stats[i].offset);
}

static void xenvif_get_strings(struct net_device *dev, u32 stringset, u8 * data)
{
	int i;

	switch (stringset) {
	case ETH_SS_STATS:
		for (i = 0; i < ARRAY_SIZE(xenvif_stats); i++)
			memcpy(data + i * ETH_GSTRING_LEN,
			       xenvif_stats[i].name, ETH_GSTRING_LEN);
		break;
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct ethtool_ops xenvif_ethtool_ops = {
=======
static const struct ethtool_ops xenvif_ethtool_ops = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static const struct ethtool_ops xenvif_ethtool_ops = {
>>>>>>> refs/remotes/origin/master
	.get_link	= ethtool_op_get_link,

	.get_sset_count = xenvif_get_sset_count,
	.get_ethtool_stats = xenvif_get_ethtool_stats,
	.get_strings = xenvif_get_strings,
};

<<<<<<< HEAD
<<<<<<< HEAD
static struct net_device_ops xenvif_netdev_ops = {
=======
static const struct net_device_ops xenvif_netdev_ops = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static const struct net_device_ops xenvif_netdev_ops = {
>>>>>>> refs/remotes/origin/master
	.ndo_start_xmit	= xenvif_start_xmit,
	.ndo_get_stats	= xenvif_get_stats,
	.ndo_open	= xenvif_open,
	.ndo_stop	= xenvif_close,
	.ndo_change_mtu	= xenvif_change_mtu,
	.ndo_fix_features = xenvif_fix_features,
<<<<<<< HEAD
=======
	.ndo_set_mac_address = eth_mac_addr,
	.ndo_validate_addr   = eth_validate_addr,
>>>>>>> refs/remotes/origin/master
};

struct xenvif *xenvif_alloc(struct device *parent, domid_t domid,
			    unsigned int handle)
{
	int err;
	struct net_device *dev;
	struct xenvif *vif;
	char name[IFNAMSIZ] = {};
<<<<<<< HEAD
=======
	int i;
>>>>>>> refs/remotes/origin/master

	snprintf(name, IFNAMSIZ - 1, "vif%u.%u", domid, handle);
	dev = alloc_netdev(sizeof(struct xenvif), name, ether_setup);
	if (dev == NULL) {
<<<<<<< HEAD
		pr_warn("Could not allocate netdev\n");
=======
		pr_warn("Could not allocate netdev for %s\n", name);
>>>>>>> refs/remotes/origin/master
		return ERR_PTR(-ENOMEM);
	}

	SET_NETDEV_DEV(dev, parent);

	vif = netdev_priv(dev);
<<<<<<< HEAD
	vif->domid  = domid;
	vif->handle = handle;
	vif->netbk  = NULL;
	vif->can_sg = 1;
	vif->csum = 1;
	atomic_set(&vif->refcnt, 1);
	init_waitqueue_head(&vif->waiting_to_free);
	vif->dev = dev;
	INIT_LIST_HEAD(&vif->schedule_list);
	INIT_LIST_HEAD(&vif->notify_list);
=======

	vif->grant_copy_op = vmalloc(sizeof(struct gnttab_copy) *
				     MAX_GRANT_COPY_OPS);
	if (vif->grant_copy_op == NULL) {
		pr_warn("Could not allocate grant copy space for %s\n", name);
		free_netdev(dev);
		return ERR_PTR(-ENOMEM);
	}

	vif->domid  = domid;
	vif->handle = handle;
	vif->can_sg = 1;
	vif->ip_csum = 1;
	vif->dev = dev;
>>>>>>> refs/remotes/origin/master

	vif->credit_bytes = vif->remaining_credit = ~0UL;
	vif->credit_usec  = 0UL;
	init_timer(&vif->credit_timeout);
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	/* Initialize 'expires' now: it's used to track the credit window. */
	vif->credit_timeout.expires = jiffies;
=======
	vif->credit_window_start = get_jiffies_64();
>>>>>>> refs/remotes/origin/cm-10.0
=======
	vif->credit_window_start = get_jiffies_64();
>>>>>>> refs/remotes/origin/cm-11.0

	dev->netdev_ops	= &xenvif_netdev_ops;
	dev->hw_features = NETIF_F_SG | NETIF_F_IP_CSUM | NETIF_F_TSO;
	dev->features = dev->hw_features;
=======
	vif->credit_window_start = get_jiffies_64();

	dev->netdev_ops	= &xenvif_netdev_ops;
	dev->hw_features = NETIF_F_SG |
		NETIF_F_IP_CSUM | NETIF_F_IPV6_CSUM |
		NETIF_F_TSO | NETIF_F_TSO6;
	dev->features = dev->hw_features | NETIF_F_RXCSUM;
>>>>>>> refs/remotes/origin/master
	SET_ETHTOOL_OPS(dev, &xenvif_ethtool_ops);

	dev->tx_queue_len = XENVIF_QUEUE_LENGTH;

<<<<<<< HEAD
=======
	skb_queue_head_init(&vif->rx_queue);
	skb_queue_head_init(&vif->tx_queue);

	vif->pending_cons = 0;
	vif->pending_prod = MAX_PENDING_REQS;
	for (i = 0; i < MAX_PENDING_REQS; i++)
		vif->pending_ring[i] = i;
	for (i = 0; i < MAX_PENDING_REQS; i++)
		vif->mmap_pages[i] = NULL;

>>>>>>> refs/remotes/origin/master
	/*
	 * Initialise a dummy MAC address. We choose the numerically
	 * largest non-broadcast address to prevent the address getting
	 * stolen by an Ethernet bridge for STP purposes.
	 * (FE:FF:FF:FF:FF:FF)
	 */
	memset(dev->dev_addr, 0xFF, ETH_ALEN);
	dev->dev_addr[0] &= ~0x01;

<<<<<<< HEAD
=======
	netif_napi_add(dev, &vif->napi, xenvif_poll, XENVIF_NAPI_WEIGHT);

>>>>>>> refs/remotes/origin/master
	netif_carrier_off(dev);

	err = register_netdev(dev);
	if (err) {
		netdev_warn(dev, "Could not register device: err=%d\n", err);
		free_netdev(dev);
		return ERR_PTR(err);
	}

	netdev_dbg(dev, "Successfully created xenvif\n");
<<<<<<< HEAD
=======

	__module_get(THIS_MODULE);

>>>>>>> refs/remotes/origin/master
	return vif;
}

int xenvif_connect(struct xenvif *vif, unsigned long tx_ring_ref,
<<<<<<< HEAD
		   unsigned long rx_ring_ref, unsigned int evtchn)
{
	int err = -ENOMEM;

	/* Already connected through? */
	if (vif->irq)
		return 0;

	err = xen_netbk_map_frontend_rings(vif, tx_ring_ref, rx_ring_ref);
	if (err < 0)
		goto err;

	err = bind_interdomain_evtchn_to_irqhandler(
		vif->domid, evtchn, xenvif_interrupt, 0,
		vif->dev->name, vif);
	if (err < 0)
		goto err_unmap;
	vif->irq = err;
	disable_irq(vif->irq);

	xenvif_get(vif);
=======
		   unsigned long rx_ring_ref, unsigned int tx_evtchn,
		   unsigned int rx_evtchn)
{
	struct task_struct *task;
	int err = -ENOMEM;

	BUG_ON(vif->tx_irq);
	BUG_ON(vif->task);

	err = xenvif_map_frontend_rings(vif, tx_ring_ref, rx_ring_ref);
	if (err < 0)
		goto err;

	if (tx_evtchn == rx_evtchn) {
		/* feature-split-event-channels == 0 */
		err = bind_interdomain_evtchn_to_irqhandler(
			vif->domid, tx_evtchn, xenvif_interrupt, 0,
			vif->dev->name, vif);
		if (err < 0)
			goto err_unmap;
		vif->tx_irq = vif->rx_irq = err;
		disable_irq(vif->tx_irq);
	} else {
		/* feature-split-event-channels == 1 */
		snprintf(vif->tx_irq_name, sizeof(vif->tx_irq_name),
			 "%s-tx", vif->dev->name);
		err = bind_interdomain_evtchn_to_irqhandler(
			vif->domid, tx_evtchn, xenvif_tx_interrupt, 0,
			vif->tx_irq_name, vif);
		if (err < 0)
			goto err_unmap;
		vif->tx_irq = err;
		disable_irq(vif->tx_irq);

		snprintf(vif->rx_irq_name, sizeof(vif->rx_irq_name),
			 "%s-rx", vif->dev->name);
		err = bind_interdomain_evtchn_to_irqhandler(
			vif->domid, rx_evtchn, xenvif_rx_interrupt, 0,
			vif->rx_irq_name, vif);
		if (err < 0)
			goto err_tx_unbind;
		vif->rx_irq = err;
		disable_irq(vif->rx_irq);
	}

	init_waitqueue_head(&vif->wq);
	task = kthread_create(xenvif_kthread,
			      (void *)vif, "%s", vif->dev->name);
	if (IS_ERR(task)) {
		pr_warn("Could not allocate kthread for %s\n", vif->dev->name);
		err = PTR_ERR(task);
		goto err_rx_unbind;
	}

	vif->task = task;
>>>>>>> refs/remotes/origin/master

	rtnl_lock();
	if (!vif->can_sg && vif->dev->mtu > ETH_DATA_LEN)
		dev_set_mtu(vif->dev, ETH_DATA_LEN);
	netdev_update_features(vif->dev);
	netif_carrier_on(vif->dev);
	if (netif_running(vif->dev))
		xenvif_up(vif);
	rtnl_unlock();

<<<<<<< HEAD
	return 0;
err_unmap:
	xen_netbk_unmap_frontend_rings(vif);
err:
=======
	wake_up_process(vif->task);

	return 0;

err_rx_unbind:
	unbind_from_irqhandler(vif->rx_irq, vif);
	vif->rx_irq = 0;
err_tx_unbind:
	unbind_from_irqhandler(vif->tx_irq, vif);
	vif->tx_irq = 0;
err_unmap:
	xenvif_unmap_frontend_rings(vif);
err:
	module_put(THIS_MODULE);
>>>>>>> refs/remotes/origin/master
	return err;
}

void xenvif_carrier_off(struct xenvif *vif)
{
	struct net_device *dev = vif->dev;
<<<<<<< HEAD
=======

	rtnl_lock();
	netif_carrier_off(dev); /* discard queued packets */
	if (netif_running(dev))
		xenvif_down(vif);
	rtnl_unlock();
	xenvif_put(vif);
}

void xenvif_disconnect(struct xenvif *vif)
{
	if (netif_carrier_ok(vif->dev))
		xenvif_carrier_off(vif);
>>>>>>> refs/remotes/origin/cm-11.0

	rtnl_lock();
	netif_carrier_off(dev); /* discard queued packets */
	if (netif_running(dev))
		xenvif_down(vif);
	rtnl_unlock();
<<<<<<< HEAD
	xenvif_put(vif);
=======
>>>>>>> refs/remotes/origin/master
}

void xenvif_disconnect(struct xenvif *vif)
{
	if (netif_carrier_ok(vif->dev))
		xenvif_carrier_off(vif);

<<<<<<< HEAD
	atomic_dec(&vif->refcnt);
	wait_event(vif->waiting_to_free, atomic_read(&vif->refcnt) == 0);

	if (vif->irq)
		unbind_from_irqhandler(vif->irq, vif);

	unregister_netdev(vif->dev);

	xen_netbk_unmap_frontend_rings(vif);

	free_netdev(vif->dev);
=======
	if (vif->task) {
		kthread_stop(vif->task);
		vif->task = NULL;
	}

	if (vif->tx_irq) {
		if (vif->tx_irq == vif->rx_irq)
			unbind_from_irqhandler(vif->tx_irq, vif);
		else {
			unbind_from_irqhandler(vif->tx_irq, vif);
			unbind_from_irqhandler(vif->rx_irq, vif);
		}
		vif->tx_irq = 0;
	}

	xenvif_unmap_frontend_rings(vif);
}

void xenvif_free(struct xenvif *vif)
{
	netif_napi_del(&vif->napi);

	unregister_netdev(vif->dev);

	vfree(vif->grant_copy_op);
	free_netdev(vif->dev);

	module_put(THIS_MODULE);
>>>>>>> refs/remotes/origin/master
}
