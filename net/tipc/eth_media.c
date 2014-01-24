/*
 * net/tipc/eth_media.c: Ethernet bearer support for TIPC
 *
 * Copyright (c) 2001-2007, Ericsson AB
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (c) 2005-2007, Wind River Systems
=======
 * Copyright (c) 2005-2008, 2011, Wind River Systems
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright (c) 2005-2008, 2011-2013, Wind River Systems
>>>>>>> refs/remotes/origin/master
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the names of the copyright holders nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "core.h"
#include "bearer.h"

<<<<<<< HEAD
<<<<<<< HEAD
#define MAX_ETH_BEARERS		2
#define ETH_LINK_PRIORITY	TIPC_DEF_LINK_PRI
#define ETH_LINK_TOLERANCE	TIPC_DEF_LINK_TOL
#define ETH_LINK_WINDOW		TIPC_DEF_LINK_WIN
=======
#define MAX_ETH_BEARERS		MAX_BEARERS

#define ETH_ADDR_OFFSET	4	/* message header offset of MAC address */
>>>>>>> refs/remotes/origin/cm-10.0

/**
 * struct eth_bearer - Ethernet bearer data structure
 * @bearer: ptr to associated "generic" bearer structure
 * @dev: ptr to associated Ethernet network device
 * @tipc_packet_type: used in binding TIPC to Ethernet driver
<<<<<<< HEAD
=======
 * @cleanup: work item used when disabling bearer
>>>>>>> refs/remotes/origin/cm-10.0
 */

struct eth_bearer {
=======
#define MAX_ETH_MEDIA		MAX_BEARERS

#define ETH_ADDR_OFFSET	4	/* message header offset of MAC address */

/**
 * struct eth_media - Ethernet bearer data structure
 * @bearer: ptr to associated "generic" bearer structure
 * @dev: ptr to associated Ethernet network device
 * @tipc_packet_type: used in binding TIPC to Ethernet driver
 * @setup: work item used when enabling bearer
 * @cleanup: work item used when disabling bearer
 */
struct eth_media {
>>>>>>> refs/remotes/origin/master
	struct tipc_bearer *bearer;
	struct net_device *dev;
	struct packet_type tipc_packet_type;
	struct work_struct setup;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
};

=======
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	struct work_struct cleanup;
};

static struct tipc_media eth_media_info;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static struct eth_bearer eth_bearers[MAX_ETH_BEARERS];
static int eth_started;
static struct notifier_block notifier;

/**
<<<<<<< HEAD
=======
=======
static struct eth_media eth_media_array[MAX_ETH_MEDIA];
static int eth_started;

static int recv_notification(struct notifier_block *nb, unsigned long evt,
			     void *dv);
/*
 * Network device notifier info
 */
static struct notifier_block notifier = {
	.notifier_call	= recv_notification,
	.priority	= 0
};

/**
>>>>>>> refs/remotes/origin/master
 * eth_media_addr_set - initialize Ethernet media address structure
 *
 * Media-dependent "value" field stores MAC address in first 6 bytes
 * and zeroes out the remaining bytes.
 */
<<<<<<< HEAD

static void eth_media_addr_set(struct tipc_media_addr *a, char *mac)
=======
static void eth_media_addr_set(const struct tipc_bearer *tb_ptr,
			       struct tipc_media_addr *a, char *mac)
>>>>>>> refs/remotes/origin/master
{
	memcpy(a->value, mac, ETH_ALEN);
	memset(a->value + ETH_ALEN, 0, sizeof(a->value) - ETH_ALEN);
	a->media_id = TIPC_MEDIA_TYPE_ETH;
<<<<<<< HEAD
	a->broadcast = !memcmp(mac, eth_media_info.bcast_addr.value, ETH_ALEN);
}

/**
>>>>>>> refs/remotes/origin/cm-10.0
 * send_msg - send a TIPC message out over an Ethernet interface
 */

=======
	a->broadcast = !memcmp(mac, tb_ptr->bcast_addr.value, ETH_ALEN);
}

/**
 * send_msg - send a TIPC message out over an Ethernet interface
 */
>>>>>>> refs/remotes/origin/master
static int send_msg(struct sk_buff *buf, struct tipc_bearer *tb_ptr,
		    struct tipc_media_addr *dest)
{
	struct sk_buff *clone;
	struct net_device *dev;
	int delta;

	clone = skb_clone(buf, GFP_ATOMIC);
	if (!clone)
		return 0;

<<<<<<< HEAD
	dev = ((struct eth_bearer *)(tb_ptr->usr_handle))->dev;
=======
	dev = ((struct eth_media *)(tb_ptr->usr_handle))->dev;
>>>>>>> refs/remotes/origin/master
	delta = dev->hard_header_len - skb_headroom(buf);

	if ((delta > 0) &&
	    pskb_expand_head(clone, SKB_DATA_ALIGN(delta), 0, GFP_ATOMIC)) {
		kfree_skb(clone);
		return 0;
	}

	skb_reset_network_header(clone);
	clone->dev = dev;
<<<<<<< HEAD
<<<<<<< HEAD
	dev_hard_header(clone, dev, ETH_P_TIPC, &dest->dev_addr.eth_addr,
=======
	dev_hard_header(clone, dev, ETH_P_TIPC, dest->value,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	clone->protocol = htons(ETH_P_TIPC);
	dev_hard_header(clone, dev, ETH_P_TIPC, dest->value,
>>>>>>> refs/remotes/origin/master
			dev->dev_addr, clone->len);
	dev_queue_xmit(clone);
	return 0;
}

/**
 * recv_msg - handle incoming TIPC message from an Ethernet interface
 *
 * Accept only packets explicitly sent to this node, or broadcast packets;
 * ignores packets sent using Ethernet multicast, and traffic sent to other
 * nodes (which can happen if interface is running in promiscuous mode).
 */
<<<<<<< HEAD

static int recv_msg(struct sk_buff *buf, struct net_device *dev,
		    struct packet_type *pt, struct net_device *orig_dev)
{
	struct eth_bearer *eb_ptr = (struct eth_bearer *)pt->af_packet_priv;

	if (!net_eq(dev_net(dev), &init_net)) {
		kfree_skb(buf);
		return 0;
=======
static int recv_msg(struct sk_buff *buf, struct net_device *dev,
		    struct packet_type *pt, struct net_device *orig_dev)
{
	struct eth_media *eb_ptr = (struct eth_media *)pt->af_packet_priv;

	if (!net_eq(dev_net(dev), &init_net)) {
		kfree_skb(buf);
		return NET_RX_DROP;
>>>>>>> refs/remotes/origin/master
	}

	if (likely(eb_ptr->bearer)) {
		if (likely(buf->pkt_type <= PACKET_BROADCAST)) {
			buf->next = NULL;
			tipc_recv_msg(buf, eb_ptr->bearer);
<<<<<<< HEAD
			return 0;
		}
	}
	kfree_skb(buf);
	return 0;
}

/**
 * setup_bearer - setup association between Ethernet bearer and interface
 */
static void setup_bearer(struct work_struct *work)
{
	struct eth_bearer *eb_ptr =
		container_of(work, struct eth_bearer, setup);
<<<<<<< HEAD
=======
			return NET_RX_SUCCESS;
		}
	}
	kfree_skb(buf);
	return NET_RX_DROP;
}

/**
 * setup_media - setup association between Ethernet bearer and interface
 */
static void setup_media(struct work_struct *work)
{
	struct eth_media *eb_ptr =
		container_of(work, struct eth_media, setup);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	dev_add_pack(&eb_ptr->tipc_packet_type);
}

/**
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
 * enable_bearer - attach TIPC bearer to an Ethernet interface
 */

static int enable_bearer(struct tipc_bearer *tb_ptr)
{
	struct net_device *dev = NULL;
	struct net_device *pdev = NULL;
	struct eth_bearer *eb_ptr = &eth_bearers[0];
	struct eth_bearer *stop = &eth_bearers[MAX_ETH_BEARERS];
=======
 * enable_media - attach TIPC bearer to an Ethernet interface
 */
static int enable_media(struct tipc_bearer *tb_ptr)
{
	struct net_device *dev;
	struct eth_media *eb_ptr = &eth_media_array[0];
	struct eth_media *stop = &eth_media_array[MAX_ETH_MEDIA];
>>>>>>> refs/remotes/origin/master
	char *driver_name = strchr((const char *)tb_ptr->name, ':') + 1;
	int pending_dev = 0;

	/* Find unused Ethernet bearer structure */
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
	while (eb_ptr->dev) {
		if (!eb_ptr->bearer)
			pending_dev++;
		if (++eb_ptr == stop)
			return pending_dev ? -EAGAIN : -EDQUOT;
	}

	/* Find device with specified name */
<<<<<<< HEAD

<<<<<<< HEAD
	for_each_netdev(&init_net, pdev) {
		if (!strncmp(pdev->name, driver_name, IFNAMSIZ)) {
			dev = pdev;
			break;
		}
	}
	if (!dev)
		return -ENODEV;

	/* Find Ethernet bearer for device (or create one) */

	while ((eb_ptr != stop) && eb_ptr->dev && (eb_ptr->dev != dev))
		eb_ptr++;
	if (eb_ptr == stop)
		return -EDQUOT;
	if (!eb_ptr->dev) {
		eb_ptr->dev = dev;
		eb_ptr->tipc_packet_type.type = htons(ETH_P_TIPC);
		eb_ptr->tipc_packet_type.dev = dev;
		eb_ptr->tipc_packet_type.func = recv_msg;
		eb_ptr->tipc_packet_type.af_packet_priv = eb_ptr;
		INIT_LIST_HEAD(&(eb_ptr->tipc_packet_type.list));
		dev_hold(dev);
		INIT_WORK(&eb_ptr->setup, setup_bearer);
		schedule_work(&eb_ptr->setup);
	}
=======
	read_lock(&dev_base_lock);
	for_each_netdev(&init_net, pdev) {
		if (!strncmp(pdev->name, driver_name, IFNAMSIZ)) {
			dev = pdev;
			dev_hold(dev);
			break;
		}
	}
	read_unlock(&dev_base_lock);
=======
	dev = dev_get_by_name(&init_net, driver_name);
>>>>>>> refs/remotes/origin/master
	if (!dev)
		return -ENODEV;

	/* Create Ethernet bearer for device */
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
	eb_ptr->dev = dev;
	eb_ptr->tipc_packet_type.type = htons(ETH_P_TIPC);
	eb_ptr->tipc_packet_type.dev = dev;
	eb_ptr->tipc_packet_type.func = recv_msg;
	eb_ptr->tipc_packet_type.af_packet_priv = eb_ptr;
	INIT_LIST_HEAD(&(eb_ptr->tipc_packet_type.list));
<<<<<<< HEAD
<<<<<<< HEAD
	INIT_WORK(&eb_ptr->setup, setup_bearer);
	schedule_work(&eb_ptr->setup);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	INIT_WORK(&eb_ptr->setup, setup_bearer);
	schedule_work(&eb_ptr->setup);
>>>>>>> refs/remotes/origin/cm-11.0

	/* Associate TIPC bearer with Ethernet bearer */

	eb_ptr->bearer = tb_ptr;
	tb_ptr->usr_handle = (void *)eb_ptr;
	tb_ptr->mtu = dev->mtu;
	tb_ptr->blocked = 0;
<<<<<<< HEAD
	tb_ptr->addr.type = htonl(TIPC_MEDIA_TYPE_ETH);
	memcpy(&tb_ptr->addr.dev_addr, dev->dev_addr, ETH_ALEN);
=======
	eth_media_addr_set(&tb_ptr->addr, (char *)dev->dev_addr);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	INIT_WORK(&eb_ptr->setup, setup_media);
	schedule_work(&eb_ptr->setup);

	/* Associate TIPC bearer with Ethernet bearer */
	eb_ptr->bearer = tb_ptr;
	tb_ptr->usr_handle = (void *)eb_ptr;
	memset(tb_ptr->bcast_addr.value, 0, sizeof(tb_ptr->bcast_addr.value));
	memcpy(tb_ptr->bcast_addr.value, dev->broadcast, ETH_ALEN);
	tb_ptr->bcast_addr.media_id = TIPC_MEDIA_TYPE_ETH;
	tb_ptr->bcast_addr.broadcast = 1;
	tb_ptr->mtu = dev->mtu;
	tb_ptr->blocked = 0;
	eth_media_addr_set(tb_ptr, &tb_ptr->addr, (char *)dev->dev_addr);
>>>>>>> refs/remotes/origin/master
	return 0;
}

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * disable_bearer - detach TIPC bearer from an Ethernet interface
 *
 * We really should do dev_remove_pack() here, but this function can not be
 * called at tasklet level. => Use eth_bearer->bearer as a flag to throw away
 * incoming buffers, & postpone dev_remove_pack() to eth_media_stop() on exit.
=======
 * cleanup_bearer - break association between Ethernet bearer and interface
 *
 * This routine must be invoked from a work queue because it can sleep.
 */

static void cleanup_bearer(struct work_struct *work)
{
	struct eth_bearer *eb_ptr =
		container_of(work, struct eth_bearer, cleanup);
=======
 * cleanup_media - break association between Ethernet bearer and interface
 *
 * This routine must be invoked from a work queue because it can sleep.
 */
static void cleanup_media(struct work_struct *work)
{
	struct eth_media *eb_ptr =
		container_of(work, struct eth_media, cleanup);
>>>>>>> refs/remotes/origin/master

	dev_remove_pack(&eb_ptr->tipc_packet_type);
	dev_put(eb_ptr->dev);
	eb_ptr->dev = NULL;
}

/**
<<<<<<< HEAD
 * disable_bearer - detach TIPC bearer from an Ethernet interface
=======
 * disable_media - detach TIPC bearer from an Ethernet interface
>>>>>>> refs/remotes/origin/master
 *
 * Mark Ethernet bearer as inactive so that incoming buffers are thrown away,
 * then get worker thread to complete bearer cleanup.  (Can't do cleanup
 * here because cleanup code needs to sleep and caller holds spinlocks.)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
 */

static void disable_bearer(struct tipc_bearer *tb_ptr)
{
<<<<<<< HEAD
	((struct eth_bearer *)tb_ptr->usr_handle)->bearer = NULL;
=======
	struct eth_bearer *eb_ptr = (struct eth_bearer *)tb_ptr->usr_handle;

	eb_ptr->bearer = NULL;
	INIT_WORK(&eb_ptr->cleanup, cleanup_bearer);
	schedule_work(&eb_ptr->cleanup);
>>>>>>> refs/remotes/origin/cm-10.0
=======
 */
static void disable_media(struct tipc_bearer *tb_ptr)
{
	struct eth_media *eb_ptr = (struct eth_media *)tb_ptr->usr_handle;

	eb_ptr->bearer = NULL;
	INIT_WORK(&eb_ptr->cleanup, cleanup_media);
	schedule_work(&eb_ptr->cleanup);
>>>>>>> refs/remotes/origin/master
}

/**
 * recv_notification - handle device updates from OS
 *
 * Change the state of the Ethernet bearer (if any) associated with the
 * specified device.
 */
<<<<<<< HEAD

static int recv_notification(struct notifier_block *nb, unsigned long evt,
			     void *dv)
{
	struct net_device *dev = (struct net_device *)dv;
	struct eth_bearer *eb_ptr = &eth_bearers[0];
	struct eth_bearer *stop = &eth_bearers[MAX_ETH_BEARERS];
=======
static int recv_notification(struct notifier_block *nb, unsigned long evt,
			     void *ptr)
{
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);
	struct eth_media *eb_ptr = &eth_media_array[0];
	struct eth_media *stop = &eth_media_array[MAX_ETH_MEDIA];
>>>>>>> refs/remotes/origin/master

	if (!net_eq(dev_net(dev), &init_net))
		return NOTIFY_DONE;

	while ((eb_ptr->dev != dev)) {
		if (++eb_ptr == stop)
			return NOTIFY_DONE;	/* couldn't find device */
	}
	if (!eb_ptr->bearer)
		return NOTIFY_DONE;		/* bearer had been disabled */

	eb_ptr->bearer->mtu = dev->mtu;

	switch (evt) {
	case NETDEV_CHANGE:
		if (netif_carrier_ok(dev))
			tipc_continue(eb_ptr->bearer);
		else
<<<<<<< HEAD
			tipc_block_bearer(eb_ptr->bearer->name);
=======
			tipc_block_bearer(eb_ptr->bearer);
>>>>>>> refs/remotes/origin/master
		break;
	case NETDEV_UP:
		tipc_continue(eb_ptr->bearer);
		break;
	case NETDEV_DOWN:
<<<<<<< HEAD
		tipc_block_bearer(eb_ptr->bearer->name);
		break;
	case NETDEV_CHANGEMTU:
	case NETDEV_CHANGEADDR:
		tipc_block_bearer(eb_ptr->bearer->name);
=======
		tipc_block_bearer(eb_ptr->bearer);
		break;
	case NETDEV_CHANGEMTU:
	case NETDEV_CHANGEADDR:
		tipc_block_bearer(eb_ptr->bearer);
>>>>>>> refs/remotes/origin/master
		tipc_continue(eb_ptr->bearer);
		break;
	case NETDEV_UNREGISTER:
	case NETDEV_CHANGENAME:
		tipc_disable_bearer(eb_ptr->bearer->name);
		break;
	}
	return NOTIFY_OK;
}

/**
 * eth_addr2str - convert Ethernet address to string
 */
<<<<<<< HEAD

<<<<<<< HEAD
static char *eth_addr2str(struct tipc_media_addr *a, char *str_buf, int str_size)
{
	unchar *addr = (unchar *)&a->dev_addr;

	if (str_size < 18)
		*str_buf = '\0';
	else
		sprintf(str_buf, "%pM", addr);
	return str_buf;
}

/**
=======
=======
>>>>>>> refs/remotes/origin/master
static int eth_addr2str(struct tipc_media_addr *a, char *str_buf, int str_size)
{
	if (str_size < 18)	/* 18 = strlen("aa:bb:cc:dd:ee:ff\0") */
		return 1;

	sprintf(str_buf, "%pM", a->value);
	return 0;
}

/**
<<<<<<< HEAD
 * eth_str2addr - convert string to Ethernet address
 */

static int eth_str2addr(struct tipc_media_addr *a, char *str_buf)
{
	char mac[ETH_ALEN];
	int r;

	r = sscanf(str_buf, "%02x:%02x:%02x:%02x:%02x:%02x",
		       (u32 *)&mac[0], (u32 *)&mac[1], (u32 *)&mac[2],
		       (u32 *)&mac[3], (u32 *)&mac[4], (u32 *)&mac[5]);

	if (r != ETH_ALEN)
		return 1;

	eth_media_addr_set(a, mac);
	return 0;
}

/**
 * eth_str2addr - convert Ethernet address format to message header format
 */

=======
 * eth_str2addr - convert Ethernet address format to message header format
 */
>>>>>>> refs/remotes/origin/master
static int eth_addr2msg(struct tipc_media_addr *a, char *msg_area)
{
	memset(msg_area, 0, TIPC_MEDIA_ADDR_SIZE);
	msg_area[TIPC_MEDIA_TYPE_OFFSET] = TIPC_MEDIA_TYPE_ETH;
	memcpy(msg_area + ETH_ADDR_OFFSET, a->value, ETH_ALEN);
	return 0;
}

/**
 * eth_str2addr - convert message header address format to Ethernet format
 */
<<<<<<< HEAD

static int eth_msg2addr(struct tipc_media_addr *a, char *msg_area)
=======
static int eth_msg2addr(const struct tipc_bearer *tb_ptr,
			struct tipc_media_addr *a, char *msg_area)
>>>>>>> refs/remotes/origin/master
{
	if (msg_area[TIPC_MEDIA_TYPE_OFFSET] != TIPC_MEDIA_TYPE_ETH)
		return 1;

<<<<<<< HEAD
	eth_media_addr_set(a, msg_area + ETH_ADDR_OFFSET);
=======
	eth_media_addr_set(tb_ptr, a, msg_area + ETH_ADDR_OFFSET);
>>>>>>> refs/remotes/origin/master
	return 0;
}

/*
 * Ethernet media registration info
 */
<<<<<<< HEAD

static struct tipc_media eth_media_info = {
	.send_msg	= send_msg,
	.enable_bearer	= enable_bearer,
	.disable_bearer	= disable_bearer,
	.addr2str	= eth_addr2str,
	.str2addr	= eth_str2addr,
	.addr2msg	= eth_addr2msg,
	.msg2addr	= eth_msg2addr,
	.bcast_addr	= { { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },
			    TIPC_MEDIA_TYPE_ETH, 1 },
=======
static struct tipc_media eth_media_info = {
	.send_msg	= send_msg,
	.enable_media	= enable_media,
	.disable_media	= disable_media,
	.addr2str	= eth_addr2str,
	.addr2msg	= eth_addr2msg,
	.msg2addr	= eth_msg2addr,
>>>>>>> refs/remotes/origin/master
	.priority	= TIPC_DEF_LINK_PRI,
	.tolerance	= TIPC_DEF_LINK_TOL,
	.window		= TIPC_DEF_LINK_WIN,
	.type_id	= TIPC_MEDIA_TYPE_ETH,
	.name		= "eth"
};

/**
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * tipc_eth_media_start - activate Ethernet bearer support
 *
 * Register Ethernet media type with TIPC bearer code.  Also register
 * with OS for notifications about device state changes.
 */
<<<<<<< HEAD

int tipc_eth_media_start(void)
{
<<<<<<< HEAD
	struct tipc_media_addr bcast_addr;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
int tipc_eth_media_start(void)
{
>>>>>>> refs/remotes/origin/master
	int res;

	if (eth_started)
		return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
	bcast_addr.type = htonl(TIPC_MEDIA_TYPE_ETH);
	memset(&bcast_addr.dev_addr, 0xff, ETH_ALEN);

	memset(eth_bearers, 0, sizeof(eth_bearers));

	res = tipc_register_media(TIPC_MEDIA_TYPE_ETH, "eth",
				  enable_bearer, disable_bearer, send_msg,
				  eth_addr2str, &bcast_addr, ETH_LINK_PRIORITY,
				  ETH_LINK_TOLERANCE, ETH_LINK_WINDOW);
=======
	res = tipc_register_media(&eth_media_info);
>>>>>>> refs/remotes/origin/cm-10.0
	if (res)
		return res;

	notifier.notifier_call = &recv_notification;
	notifier.priority = 0;
=======
	res = tipc_register_media(&eth_media_info);
	if (res)
		return res;

>>>>>>> refs/remotes/origin/master
	res = register_netdevice_notifier(&notifier);
	if (!res)
		eth_started = 1;
	return res;
}

/**
 * tipc_eth_media_stop - deactivate Ethernet bearer support
 */
<<<<<<< HEAD

void tipc_eth_media_stop(void)
{
<<<<<<< HEAD
	int i;

	if (!eth_started)
		return;

	unregister_netdevice_notifier(&notifier);
	for (i = 0; i < MAX_ETH_BEARERS ; i++) {
		if (eth_bearers[i].bearer) {
			eth_bearers[i].bearer->blocked = 1;
			eth_bearers[i].bearer = NULL;
		}
		if (eth_bearers[i].dev) {
			dev_remove_pack(&eth_bearers[i].tipc_packet_type);
			dev_put(eth_bearers[i].dev);
		}
	}
	memset(&eth_bearers, 0, sizeof(eth_bearers));
=======
=======
void tipc_eth_media_stop(void)
{
>>>>>>> refs/remotes/origin/master
	if (!eth_started)
		return;

	flush_scheduled_work();
	unregister_netdevice_notifier(&notifier);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	eth_started = 0;
}
