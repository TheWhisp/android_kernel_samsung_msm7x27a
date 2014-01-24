<<<<<<< HEAD
/*
<<<<<<< HEAD
 * Copyright (C) 2007-2011 B.A.T.M.A.N. contributors:
=======
 * Copyright (C) 2007-2012 B.A.T.M.A.N. contributors:
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* Copyright (C) 2007-2013 B.A.T.M.A.N. contributors:
>>>>>>> refs/remotes/origin/master
 *
 * Marek Lindner, Simon Wunderlich
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
<<<<<<< HEAD
 *
=======
>>>>>>> refs/remotes/origin/master
 */

#include "main.h"
#include "soft-interface.h"
#include "hard-interface.h"
<<<<<<< HEAD
#include "routing.h"
#include "send.h"
#include "bat_debugfs.h"
=======
#include "distributed-arp-table.h"
#include "routing.h"
#include "send.h"
#include "debugfs.h"
>>>>>>> refs/remotes/origin/master
#include "translation-table.h"
#include "hash.h"
#include "gateway_common.h"
#include "gateway_client.h"
<<<<<<< HEAD
#include "bat_sysfs.h"
<<<<<<< HEAD
=======
#include "originator.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "sysfs.h"
#include "originator.h"
>>>>>>> refs/remotes/origin/master
#include <linux/slab.h>
#include <linux/ethtool.h>
#include <linux/etherdevice.h>
#include <linux/if_vlan.h>
<<<<<<< HEAD
#include "unicast.h"


static int bat_get_settings(struct net_device *dev, struct ethtool_cmd *cmd);
static void bat_get_drvinfo(struct net_device *dev,
			    struct ethtool_drvinfo *info);
static u32 bat_get_msglevel(struct net_device *dev);
static void bat_set_msglevel(struct net_device *dev, u32 value);
static u32 bat_get_link(struct net_device *dev);

static const struct ethtool_ops bat_ethtool_ops = {
	.get_settings = bat_get_settings,
	.get_drvinfo = bat_get_drvinfo,
	.get_msglevel = bat_get_msglevel,
	.set_msglevel = bat_set_msglevel,
	.get_link = bat_get_link,
};

int my_skb_head_push(struct sk_buff *skb, unsigned int len)
{
	int result;

	/**
	 * TODO: We must check if we can release all references to non-payload
=======
#include "bridge_loop_avoidance.h"
#include "network-coding.h"


static int batadv_get_settings(struct net_device *dev, struct ethtool_cmd *cmd);
static void batadv_get_drvinfo(struct net_device *dev,
			       struct ethtool_drvinfo *info);
static u32 batadv_get_msglevel(struct net_device *dev);
static void batadv_set_msglevel(struct net_device *dev, u32 value);
static u32 batadv_get_link(struct net_device *dev);
static void batadv_get_strings(struct net_device *dev, u32 stringset, u8 *data);
static void batadv_get_ethtool_stats(struct net_device *dev,
				     struct ethtool_stats *stats, u64 *data);
static int batadv_get_sset_count(struct net_device *dev, int stringset);

static const struct ethtool_ops batadv_ethtool_ops = {
	.get_settings = batadv_get_settings,
	.get_drvinfo = batadv_get_drvinfo,
	.get_msglevel = batadv_get_msglevel,
	.set_msglevel = batadv_set_msglevel,
	.get_link = batadv_get_link,
	.get_strings = batadv_get_strings,
	.get_ethtool_stats = batadv_get_ethtool_stats,
	.get_sset_count = batadv_get_sset_count,
};

int batadv_skb_head_push(struct sk_buff *skb, unsigned int len)
{
	int result;

	/* TODO: We must check if we can release all references to non-payload
>>>>>>> refs/remotes/origin/master
	 * data using skb_header_release in our skbs to allow skb_cow_header to
	 * work optimally. This means that those skbs are not allowed to read
	 * or write any data which is before the current position of skb->data
	 * after that call and thus allow other skbs with the same data buffer
	 * to write freely in that area.
	 */
	result = skb_cow_head(skb, len);
	if (result < 0)
		return result;

	skb_push(skb, len);
	return 0;
}

<<<<<<< HEAD
static void softif_neigh_free_ref(struct softif_neigh *softif_neigh)
{
	if (atomic_dec_and_test(&softif_neigh->refcount))
		kfree_rcu(softif_neigh, rcu);
}

static void softif_neigh_vid_free_rcu(struct rcu_head *rcu)
{
	struct softif_neigh_vid *softif_neigh_vid;
	struct softif_neigh *softif_neigh;
	struct hlist_node *node, *node_tmp;
	struct bat_priv *bat_priv;

	softif_neigh_vid = container_of(rcu, struct softif_neigh_vid, rcu);
	bat_priv = softif_neigh_vid->bat_priv;

	spin_lock_bh(&bat_priv->softif_neigh_lock);
	hlist_for_each_entry_safe(softif_neigh, node, node_tmp,
				  &softif_neigh_vid->softif_neigh_list, list) {
		hlist_del_rcu(&softif_neigh->list);
		softif_neigh_free_ref(softif_neigh);
	}
	spin_unlock_bh(&bat_priv->softif_neigh_lock);

	kfree(softif_neigh_vid);
}

static void softif_neigh_vid_free_ref(struct softif_neigh_vid *softif_neigh_vid)
{
	if (atomic_dec_and_test(&softif_neigh_vid->refcount))
		call_rcu(&softif_neigh_vid->rcu, softif_neigh_vid_free_rcu);
}

static struct softif_neigh_vid *softif_neigh_vid_get(struct bat_priv *bat_priv,
						     short vid)
{
	struct softif_neigh_vid *softif_neigh_vid;
	struct hlist_node *node;

	rcu_read_lock();
	hlist_for_each_entry_rcu(softif_neigh_vid, node,
				 &bat_priv->softif_neigh_vids, list) {
		if (softif_neigh_vid->vid != vid)
			continue;

		if (!atomic_inc_not_zero(&softif_neigh_vid->refcount))
			continue;

		goto out;
	}

<<<<<<< HEAD
	softif_neigh_vid = kzalloc(sizeof(struct softif_neigh_vid),
				   GFP_ATOMIC);
=======
	softif_neigh_vid = kzalloc(sizeof(*softif_neigh_vid), GFP_ATOMIC);
>>>>>>> refs/remotes/origin/cm-10.0
	if (!softif_neigh_vid)
		goto out;

	softif_neigh_vid->vid = vid;
	softif_neigh_vid->bat_priv = bat_priv;

	/* initialize with 2 - caller decrements counter by one */
	atomic_set(&softif_neigh_vid->refcount, 2);
	INIT_HLIST_HEAD(&softif_neigh_vid->softif_neigh_list);
	INIT_HLIST_NODE(&softif_neigh_vid->list);
	spin_lock_bh(&bat_priv->softif_neigh_vid_lock);
	hlist_add_head_rcu(&softif_neigh_vid->list,
			   &bat_priv->softif_neigh_vids);
	spin_unlock_bh(&bat_priv->softif_neigh_vid_lock);

out:
	rcu_read_unlock();
	return softif_neigh_vid;
}

static struct softif_neigh *softif_neigh_get(struct bat_priv *bat_priv,
<<<<<<< HEAD
					     uint8_t *addr, short vid)
=======
					     const uint8_t *addr, short vid)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct softif_neigh_vid *softif_neigh_vid;
	struct softif_neigh *softif_neigh = NULL;
	struct hlist_node *node;

	softif_neigh_vid = softif_neigh_vid_get(bat_priv, vid);
	if (!softif_neigh_vid)
		goto out;

	rcu_read_lock();
	hlist_for_each_entry_rcu(softif_neigh, node,
				 &softif_neigh_vid->softif_neigh_list,
				 list) {
		if (!compare_eth(softif_neigh->addr, addr))
			continue;

		if (!atomic_inc_not_zero(&softif_neigh->refcount))
			continue;

		softif_neigh->last_seen = jiffies;
		goto unlock;
	}

<<<<<<< HEAD
	softif_neigh = kzalloc(sizeof(struct softif_neigh), GFP_ATOMIC);
=======
	softif_neigh = kzalloc(sizeof(*softif_neigh), GFP_ATOMIC);
>>>>>>> refs/remotes/origin/cm-10.0
	if (!softif_neigh)
		goto unlock;

	memcpy(softif_neigh->addr, addr, ETH_ALEN);
	softif_neigh->last_seen = jiffies;
	/* initialize with 2 - caller decrements counter by one */
	atomic_set(&softif_neigh->refcount, 2);

	INIT_HLIST_NODE(&softif_neigh->list);
	spin_lock_bh(&bat_priv->softif_neigh_lock);
	hlist_add_head_rcu(&softif_neigh->list,
			   &softif_neigh_vid->softif_neigh_list);
	spin_unlock_bh(&bat_priv->softif_neigh_lock);

unlock:
	rcu_read_unlock();
out:
	if (softif_neigh_vid)
		softif_neigh_vid_free_ref(softif_neigh_vid);
	return softif_neigh;
}

static struct softif_neigh *softif_neigh_get_selected(
				struct softif_neigh_vid *softif_neigh_vid)
{
	struct softif_neigh *softif_neigh;

	rcu_read_lock();
	softif_neigh = rcu_dereference(softif_neigh_vid->softif_neigh);

	if (softif_neigh && !atomic_inc_not_zero(&softif_neigh->refcount))
		softif_neigh = NULL;

	rcu_read_unlock();
	return softif_neigh;
}

static struct softif_neigh *softif_neigh_vid_get_selected(
						struct bat_priv *bat_priv,
						short vid)
{
	struct softif_neigh_vid *softif_neigh_vid;
	struct softif_neigh *softif_neigh = NULL;

	softif_neigh_vid = softif_neigh_vid_get(bat_priv, vid);
	if (!softif_neigh_vid)
		goto out;

	softif_neigh = softif_neigh_get_selected(softif_neigh_vid);
out:
	if (softif_neigh_vid)
		softif_neigh_vid_free_ref(softif_neigh_vid);
	return softif_neigh;
}

static void softif_neigh_vid_select(struct bat_priv *bat_priv,
				    struct softif_neigh *new_neigh,
				    short vid)
{
	struct softif_neigh_vid *softif_neigh_vid;
	struct softif_neigh *curr_neigh;

	softif_neigh_vid = softif_neigh_vid_get(bat_priv, vid);
	if (!softif_neigh_vid)
		goto out;

	spin_lock_bh(&bat_priv->softif_neigh_lock);

	if (new_neigh && !atomic_inc_not_zero(&new_neigh->refcount))
		new_neigh = NULL;

<<<<<<< HEAD
	curr_neigh = softif_neigh_vid->softif_neigh;
=======
	curr_neigh = rcu_dereference_protected(softif_neigh_vid->softif_neigh,
					       1);
>>>>>>> refs/remotes/origin/cm-10.0
	rcu_assign_pointer(softif_neigh_vid->softif_neigh, new_neigh);

	if ((curr_neigh) && (!new_neigh))
		bat_dbg(DBG_ROUTES, bat_priv,
			"Removing mesh exit point on vid: %d (prev: %pM).\n",
			vid, curr_neigh->addr);
	else if ((curr_neigh) && (new_neigh))
		bat_dbg(DBG_ROUTES, bat_priv,
<<<<<<< HEAD
			"Changing mesh exit point on vid: %d from %pM "
			"to %pM.\n", vid, curr_neigh->addr, new_neigh->addr);
=======
			"Changing mesh exit point on vid: %d from %pM to %pM.\n",
			vid, curr_neigh->addr, new_neigh->addr);
>>>>>>> refs/remotes/origin/cm-10.0
	else if ((!curr_neigh) && (new_neigh))
		bat_dbg(DBG_ROUTES, bat_priv,
			"Setting mesh exit point on vid: %d to %pM.\n",
			vid, new_neigh->addr);

	if (curr_neigh)
		softif_neigh_free_ref(curr_neigh);

	spin_unlock_bh(&bat_priv->softif_neigh_lock);

out:
	if (softif_neigh_vid)
		softif_neigh_vid_free_ref(softif_neigh_vid);
}

static void softif_neigh_vid_deselect(struct bat_priv *bat_priv,
				      struct softif_neigh_vid *softif_neigh_vid)
{
	struct softif_neigh *curr_neigh;
	struct softif_neigh *softif_neigh = NULL, *softif_neigh_tmp;
	struct hard_iface *primary_if = NULL;
	struct hlist_node *node;

	primary_if = primary_if_get_selected(bat_priv);
	if (!primary_if)
		goto out;

	/* find new softif_neigh immediately to avoid temporary loops */
	rcu_read_lock();
	curr_neigh = rcu_dereference(softif_neigh_vid->softif_neigh);

	hlist_for_each_entry_rcu(softif_neigh_tmp, node,
				 &softif_neigh_vid->softif_neigh_list,
				 list) {
		if (softif_neigh_tmp == curr_neigh)
			continue;

		/* we got a neighbor but its mac is 'bigger' than ours  */
		if (memcmp(primary_if->net_dev->dev_addr,
			   softif_neigh_tmp->addr, ETH_ALEN) < 0)
			continue;

		if (!atomic_inc_not_zero(&softif_neigh_tmp->refcount))
			continue;

		softif_neigh = softif_neigh_tmp;
		goto unlock;
	}

unlock:
	rcu_read_unlock();
out:
	softif_neigh_vid_select(bat_priv, softif_neigh, softif_neigh_vid->vid);

	if (primary_if)
		hardif_free_ref(primary_if);
	if (softif_neigh)
		softif_neigh_free_ref(softif_neigh);
}

int softif_neigh_seq_print_text(struct seq_file *seq, void *offset)
{
	struct net_device *net_dev = (struct net_device *)seq->private;
	struct bat_priv *bat_priv = netdev_priv(net_dev);
	struct softif_neigh_vid *softif_neigh_vid;
	struct softif_neigh *softif_neigh;
	struct hard_iface *primary_if;
	struct hlist_node *node, *node_tmp;
	struct softif_neigh *curr_softif_neigh;
	int ret = 0, last_seen_secs, last_seen_msecs;

	primary_if = primary_if_get_selected(bat_priv);
	if (!primary_if) {
<<<<<<< HEAD
		ret = seq_printf(seq, "BATMAN mesh %s disabled - "
				 "please specify interfaces to enable it\n",
=======
		ret = seq_printf(seq,
				 "BATMAN mesh %s disabled - please specify interfaces to enable it\n",
>>>>>>> refs/remotes/origin/cm-10.0
				 net_dev->name);
		goto out;
	}

	if (primary_if->if_status != IF_ACTIVE) {
<<<<<<< HEAD
		ret = seq_printf(seq, "BATMAN mesh %s "
				 "disabled - primary interface not active\n",
=======
		ret = seq_printf(seq,
				 "BATMAN mesh %s disabled - primary interface not active\n",
>>>>>>> refs/remotes/origin/cm-10.0
				 net_dev->name);
		goto out;
	}

	seq_printf(seq, "Softif neighbor list (%s)\n", net_dev->name);

	rcu_read_lock();
	hlist_for_each_entry_rcu(softif_neigh_vid, node,
				 &bat_priv->softif_neigh_vids, list) {
		seq_printf(seq, "     %-15s %s on vid: %d\n",
			   "Originator", "last-seen", softif_neigh_vid->vid);

		curr_softif_neigh = softif_neigh_get_selected(softif_neigh_vid);

		hlist_for_each_entry_rcu(softif_neigh, node_tmp,
					 &softif_neigh_vid->softif_neigh_list,
					 list) {
			last_seen_secs = jiffies_to_msecs(jiffies -
						softif_neigh->last_seen) / 1000;
			last_seen_msecs = jiffies_to_msecs(jiffies -
						softif_neigh->last_seen) % 1000;
			seq_printf(seq, "%s %pM  %3i.%03is\n",
				   curr_softif_neigh == softif_neigh
				   ? "=>" : "  ", softif_neigh->addr,
				   last_seen_secs, last_seen_msecs);
		}

		if (curr_softif_neigh)
			softif_neigh_free_ref(curr_softif_neigh);

		seq_printf(seq, "\n");
	}
	rcu_read_unlock();

out:
	if (primary_if)
		hardif_free_ref(primary_if);
	return ret;
}

void softif_neigh_purge(struct bat_priv *bat_priv)
{
	struct softif_neigh *softif_neigh, *curr_softif_neigh;
	struct softif_neigh_vid *softif_neigh_vid;
	struct hlist_node *node, *node_tmp, *node_tmp2;
<<<<<<< HEAD
	char do_deselect;
=======
	int do_deselect;
>>>>>>> refs/remotes/origin/cm-10.0

	rcu_read_lock();
	hlist_for_each_entry_rcu(softif_neigh_vid, node,
				 &bat_priv->softif_neigh_vids, list) {
		if (!atomic_inc_not_zero(&softif_neigh_vid->refcount))
			continue;

		curr_softif_neigh = softif_neigh_get_selected(softif_neigh_vid);
		do_deselect = 0;

		spin_lock_bh(&bat_priv->softif_neigh_lock);
		hlist_for_each_entry_safe(softif_neigh, node_tmp, node_tmp2,
					  &softif_neigh_vid->softif_neigh_list,
					  list) {
<<<<<<< HEAD
			if ((!time_after(jiffies, softif_neigh->last_seen +
				msecs_to_jiffies(SOFTIF_NEIGH_TIMEOUT))) &&
=======
			if ((!has_timed_out(softif_neigh->last_seen,
					    SOFTIF_NEIGH_TIMEOUT)) &&
>>>>>>> refs/remotes/origin/cm-10.0
			    (atomic_read(&bat_priv->mesh_state) == MESH_ACTIVE))
				continue;

			if (curr_softif_neigh == softif_neigh) {
				bat_dbg(DBG_ROUTES, bat_priv,
<<<<<<< HEAD
					"Current mesh exit point on vid: %d "
					"'%pM' vanished.\n",
=======
					"Current mesh exit point on vid: %d '%pM' vanished.\n",
>>>>>>> refs/remotes/origin/cm-10.0
					softif_neigh_vid->vid,
					softif_neigh->addr);
				do_deselect = 1;
			}

			hlist_del_rcu(&softif_neigh->list);
			softif_neigh_free_ref(softif_neigh);
		}
		spin_unlock_bh(&bat_priv->softif_neigh_lock);

		/* soft_neigh_vid_deselect() needs to acquire the
		 * softif_neigh_lock */
		if (do_deselect)
			softif_neigh_vid_deselect(bat_priv, softif_neigh_vid);

		if (curr_softif_neigh)
			softif_neigh_free_ref(curr_softif_neigh);

		softif_neigh_vid_free_ref(softif_neigh_vid);
	}
	rcu_read_unlock();

	spin_lock_bh(&bat_priv->softif_neigh_vid_lock);
	hlist_for_each_entry_safe(softif_neigh_vid, node, node_tmp,
				  &bat_priv->softif_neigh_vids, list) {
		if (!hlist_empty(&softif_neigh_vid->softif_neigh_list))
			continue;

		hlist_del_rcu(&softif_neigh_vid->list);
		softif_neigh_vid_free_ref(softif_neigh_vid);
	}
	spin_unlock_bh(&bat_priv->softif_neigh_vid_lock);

}

static void softif_batman_recv(struct sk_buff *skb, struct net_device *dev,
			       short vid)
{
	struct bat_priv *bat_priv = netdev_priv(dev);
	struct ethhdr *ethhdr = (struct ethhdr *)skb->data;
<<<<<<< HEAD
	struct batman_packet *batman_packet;
=======
	struct batman_ogm_packet *batman_ogm_packet;
>>>>>>> refs/remotes/origin/cm-10.0
	struct softif_neigh *softif_neigh = NULL;
	struct hard_iface *primary_if = NULL;
	struct softif_neigh *curr_softif_neigh = NULL;

	if (ntohs(ethhdr->h_proto) == ETH_P_8021Q)
<<<<<<< HEAD
		batman_packet = (struct batman_packet *)
					(skb->data + ETH_HLEN + VLAN_HLEN);
	else
		batman_packet = (struct batman_packet *)(skb->data + ETH_HLEN);

	if (batman_packet->version != COMPAT_VERSION)
		goto out;

	if (batman_packet->packet_type != BAT_PACKET)
		goto out;

	if (!(batman_packet->flags & PRIMARIES_FIRST_HOP))
		goto out;

	if (is_my_mac(batman_packet->orig))
		goto out;

	softif_neigh = softif_neigh_get(bat_priv, batman_packet->orig, vid);
=======
		batman_ogm_packet = (struct batman_ogm_packet *)
					(skb->data + ETH_HLEN + VLAN_HLEN);
	else
		batman_ogm_packet = (struct batman_ogm_packet *)
							(skb->data + ETH_HLEN);

	if (batman_ogm_packet->header.version != COMPAT_VERSION)
		goto out;

	if (batman_ogm_packet->header.packet_type != BAT_OGM)
		goto out;

	if (!(batman_ogm_packet->flags & PRIMARIES_FIRST_HOP))
		goto out;

	if (is_my_mac(batman_ogm_packet->orig))
		goto out;

	softif_neigh = softif_neigh_get(bat_priv, batman_ogm_packet->orig, vid);
>>>>>>> refs/remotes/origin/cm-10.0
	if (!softif_neigh)
		goto out;

	curr_softif_neigh = softif_neigh_vid_get_selected(bat_priv, vid);
	if (curr_softif_neigh == softif_neigh)
		goto out;

	primary_if = primary_if_get_selected(bat_priv);
	if (!primary_if)
		goto out;

	/* we got a neighbor but its mac is 'bigger' than ours  */
	if (memcmp(primary_if->net_dev->dev_addr,
		   softif_neigh->addr, ETH_ALEN) < 0)
		goto out;

	/* close own batX device and use softif_neigh as exit node */
	if (!curr_softif_neigh) {
		softif_neigh_vid_select(bat_priv, softif_neigh, vid);
		goto out;
	}

	/* switch to new 'smallest neighbor' */
	if (memcmp(softif_neigh->addr, curr_softif_neigh->addr, ETH_ALEN) < 0)
		softif_neigh_vid_select(bat_priv, softif_neigh, vid);

out:
	kfree_skb(skb);
	if (softif_neigh)
		softif_neigh_free_ref(softif_neigh);
	if (curr_softif_neigh)
		softif_neigh_free_ref(curr_softif_neigh);
	if (primary_if)
		hardif_free_ref(primary_if);
	return;
}

static int interface_open(struct net_device *dev)
=======
static int batadv_interface_open(struct net_device *dev)
>>>>>>> refs/remotes/origin/master
{
	netif_start_queue(dev);
	return 0;
}

<<<<<<< HEAD
static int interface_release(struct net_device *dev)
=======
static int batadv_interface_release(struct net_device *dev)
>>>>>>> refs/remotes/origin/master
{
	netif_stop_queue(dev);
	return 0;
}

<<<<<<< HEAD
static struct net_device_stats *interface_stats(struct net_device *dev)
{
	struct bat_priv *bat_priv = netdev_priv(dev);
	return &bat_priv->stats;
}

static int interface_set_mac_addr(struct net_device *dev, void *p)
{
	struct bat_priv *bat_priv = netdev_priv(dev);
	struct sockaddr *addr = p;
=======
static struct net_device_stats *batadv_interface_stats(struct net_device *dev)
{
	struct batadv_priv *bat_priv = netdev_priv(dev);
	struct net_device_stats *stats = &bat_priv->stats;

	stats->tx_packets = batadv_sum_counter(bat_priv, BATADV_CNT_TX);
	stats->tx_bytes = batadv_sum_counter(bat_priv, BATADV_CNT_TX_BYTES);
	stats->tx_dropped = batadv_sum_counter(bat_priv, BATADV_CNT_TX_DROPPED);
	stats->rx_packets = batadv_sum_counter(bat_priv, BATADV_CNT_RX);
	stats->rx_bytes = batadv_sum_counter(bat_priv, BATADV_CNT_RX_BYTES);
	return stats;
}

static int batadv_interface_set_mac_addr(struct net_device *dev, void *p)
{
	struct batadv_priv *bat_priv = netdev_priv(dev);
	struct sockaddr *addr = p;
	uint8_t old_addr[ETH_ALEN];
>>>>>>> refs/remotes/origin/master

	if (!is_valid_ether_addr(addr->sa_data))
		return -EADDRNOTAVAIL;

<<<<<<< HEAD
<<<<<<< HEAD
	/* only modify transtable if it has been initialised before */
	if (atomic_read(&bat_priv->mesh_state) == MESH_ACTIVE) {
		tt_local_remove(bat_priv, dev->dev_addr,
				 "mac address changed");
		tt_local_add(dev, addr->sa_data);
	}

	memcpy(dev->dev_addr, addr->sa_data, ETH_ALEN);
=======
	/* only modify transtable if it has been initialized before */
	if (atomic_read(&bat_priv->mesh_state) == MESH_ACTIVE) {
		tt_local_remove(bat_priv, dev->dev_addr,
				"mac address changed", false);
		tt_local_add(dev, addr->sa_data, NULL_IFINDEX);
	}

	memcpy(dev->dev_addr, addr->sa_data, ETH_ALEN);
	dev->addr_assign_type &= ~NET_ADDR_RANDOM;
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

static int interface_change_mtu(struct net_device *dev, int new_mtu)
{
	/* check ranges */
	if ((new_mtu < 68) || (new_mtu > hardif_min_mtu(dev)))
=======
	memcpy(old_addr, dev->dev_addr, ETH_ALEN);
	memcpy(dev->dev_addr, addr->sa_data, ETH_ALEN);

	/* only modify transtable if it has been initialized before */
	if (atomic_read(&bat_priv->mesh_state) == BATADV_MESH_ACTIVE) {
		batadv_tt_local_remove(bat_priv, old_addr, BATADV_NO_FLAGS,
				       "mac address changed", false);
		batadv_tt_local_add(dev, addr->sa_data, BATADV_NO_FLAGS,
				    BATADV_NULL_IFINDEX);
	}

	return 0;
}

static int batadv_interface_change_mtu(struct net_device *dev, int new_mtu)
{
	/* check ranges */
	if ((new_mtu < 68) || (new_mtu > batadv_hardif_min_mtu(dev)))
>>>>>>> refs/remotes/origin/master
		return -EINVAL;

	dev->mtu = new_mtu;

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
int interface_tx(struct sk_buff *skb, struct net_device *soft_iface)
=======
static int interface_tx(struct sk_buff *skb, struct net_device *soft_iface)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct ethhdr *ethhdr = (struct ethhdr *)skb->data;
	struct bat_priv *bat_priv = netdev_priv(soft_iface);
	struct hard_iface *primary_if = NULL;
	struct bcast_packet *bcast_packet;
	struct vlan_ethhdr *vhdr;
	struct softif_neigh *curr_softif_neigh = NULL;
<<<<<<< HEAD
=======
	unsigned int header_len = 0;
>>>>>>> refs/remotes/origin/cm-10.0
	int data_len = skb->len, ret;
	short vid = -1;
	bool do_bcast = false;

	if (atomic_read(&bat_priv->mesh_state) != MESH_ACTIVE)
		goto dropped;

	soft_iface->trans_start = jiffies;
=======
/**
 * batadv_interface_set_rx_mode - set the rx mode of a device
 * @dev: registered network device to modify
 *
 * We do not actually need to set any rx filters for the virtual batman
 * soft interface. However a dummy handler enables a user to set static
 * multicast listeners for instance.
 */
static void batadv_interface_set_rx_mode(struct net_device *dev)
{
}

static int batadv_interface_tx(struct sk_buff *skb,
			       struct net_device *soft_iface)
{
	struct ethhdr *ethhdr;
	struct batadv_priv *bat_priv = netdev_priv(soft_iface);
	struct batadv_hard_iface *primary_if = NULL;
	struct batadv_bcast_packet *bcast_packet;
	__be16 ethertype = htons(ETH_P_BATMAN);
	static const uint8_t stp_addr[ETH_ALEN] = {0x01, 0x80, 0xC2, 0x00,
						   0x00, 0x00};
	static const uint8_t ectp_addr[ETH_ALEN] = {0xCF, 0x00, 0x00, 0x00,
						    0x00, 0x00};
	struct vlan_ethhdr *vhdr;
	unsigned int header_len = 0;
	int data_len = skb->len, ret;
	unsigned long brd_delay = 1;
	bool do_bcast = false, client_added;
	unsigned short vid;
	uint32_t seqno;

	if (atomic_read(&bat_priv->mesh_state) != BATADV_MESH_ACTIVE)
		goto dropped;

	soft_iface->trans_start = jiffies;
	vid = batadv_get_vid(skb, 0);
	ethhdr = (struct ethhdr *)skb->data;
>>>>>>> refs/remotes/origin/master

	switch (ntohs(ethhdr->h_proto)) {
	case ETH_P_8021Q:
		vhdr = (struct vlan_ethhdr *)skb->data;
<<<<<<< HEAD
		vid = ntohs(vhdr->h_vlan_TCI) & VLAN_VID_MASK;

		if (ntohs(vhdr->h_vlan_encapsulated_proto) != ETH_P_BATMAN)
=======

		if (vhdr->h_vlan_encapsulated_proto != ethertype)
>>>>>>> refs/remotes/origin/master
			break;

		/* fall through */
	case ETH_P_BATMAN:
<<<<<<< HEAD
		softif_batman_recv(skb, soft_iface, vid);
		goto end;
	}

	/**
	 * if we have a another chosen mesh exit node in range
	 * it will transport the packets to the mesh
	 */
	curr_softif_neigh = softif_neigh_vid_get_selected(bat_priv, vid);
	if (curr_softif_neigh)
		goto dropped;

<<<<<<< HEAD
	/* TODO: check this for locks */
	tt_local_add(soft_iface, ethhdr->h_source);

	if (is_multicast_ether_addr(ethhdr->h_dest)) {
		ret = gw_is_target(bat_priv, skb);

		if (ret < 0)
			goto dropped;

		if (ret == 0)
			do_bcast = true;
=======
	/* Register the client MAC in the transtable */
	tt_local_add(soft_iface, ethhdr->h_source, skb->skb_iif);
=======
		goto dropped;
	}

	if (batadv_bla_tx(bat_priv, skb, vid))
		goto dropped;

	/* skb->data might have been reallocated by batadv_bla_tx() */
	ethhdr = (struct ethhdr *)skb->data;

	/* Register the client MAC in the transtable */
	if (!is_multicast_ether_addr(ethhdr->h_source)) {
		client_added = batadv_tt_local_add(soft_iface, ethhdr->h_source,
						   vid, skb->skb_iif);
		if (!client_added)
			goto dropped;
	}

	/* don't accept stp packets. STP does not help in meshes.
	 * better use the bridge loop avoidance ...
	 *
	 * The same goes for ECTP sent at least by some Cisco Switches,
	 * it might confuse the mesh when used with bridge loop avoidance.
	 */
	if (batadv_compare_eth(ethhdr->h_dest, stp_addr))
		goto dropped;

	if (batadv_compare_eth(ethhdr->h_dest, ectp_addr))
		goto dropped;
>>>>>>> refs/remotes/origin/master

	if (is_multicast_ether_addr(ethhdr->h_dest)) {
		do_bcast = true;

		switch (atomic_read(&bat_priv->gw_mode)) {
<<<<<<< HEAD
		case GW_MODE_SERVER:
			/* gateway servers should not send dhcp
			 * requests into the mesh */
			ret = gw_is_dhcp_target(skb, &header_len);
			if (ret)
				goto dropped;
			break;
		case GW_MODE_CLIENT:
			/* gateway clients should send dhcp requests
			 * via unicast to their gateway */
			ret = gw_is_dhcp_target(skb, &header_len);
			if (ret)
				do_bcast = false;
			break;
		case GW_MODE_OFF:
		default:
			break;
		}
>>>>>>> refs/remotes/origin/cm-10.0
	}

	/* ethernet packet should be broadcasted */
	if (do_bcast) {
		primary_if = primary_if_get_selected(bat_priv);
		if (!primary_if)
			goto dropped;

<<<<<<< HEAD
		if (my_skb_head_push(skb, sizeof(struct bcast_packet)) < 0)
			goto dropped;

		bcast_packet = (struct bcast_packet *)skb->data;
		bcast_packet->version = COMPAT_VERSION;
		bcast_packet->ttl = TTL;

		/* batman packet type: broadcast */
		bcast_packet->packet_type = BAT_BCAST;
=======
		if (my_skb_head_push(skb, sizeof(*bcast_packet)) < 0)
			goto dropped;

		bcast_packet = (struct bcast_packet *)skb->data;
		bcast_packet->header.version = COMPAT_VERSION;
		bcast_packet->header.ttl = TTL;

		/* batman packet type: broadcast */
		bcast_packet->header.packet_type = BAT_BCAST;
>>>>>>> refs/remotes/origin/cm-10.0

		/* hw address of first interface is the orig mac because only
		 * this mac is known throughout the mesh */
=======
		case BATADV_GW_MODE_SERVER:
			/* gateway servers should not send dhcp
			 * requests into the mesh
			 */
			ret = batadv_gw_is_dhcp_target(skb, &header_len);
			if (ret)
				goto dropped;
			break;
		case BATADV_GW_MODE_CLIENT:
			/* gateway clients should send dhcp requests
			 * via unicast to their gateway
			 */
			ret = batadv_gw_is_dhcp_target(skb, &header_len);
			if (ret)
				do_bcast = false;
			break;
		case BATADV_GW_MODE_OFF:
		default:
			break;
		}

		/* reminder: ethhdr might have become unusable from here on
		 * (batadv_gw_is_dhcp_target() might have reallocated skb data)
		 */
	}

	batadv_skb_set_priority(skb, 0);

	/* ethernet packet should be broadcasted */
	if (do_bcast) {
		primary_if = batadv_primary_if_get_selected(bat_priv);
		if (!primary_if)
			goto dropped;

		/* in case of ARP request, we do not immediately broadcasti the
		 * packet, instead we first wait for DAT to try to retrieve the
		 * correct ARP entry
		 */
		if (batadv_dat_snoop_outgoing_arp_request(bat_priv, skb))
			brd_delay = msecs_to_jiffies(ARP_REQ_DELAY);

		if (batadv_skb_head_push(skb, sizeof(*bcast_packet)) < 0)
			goto dropped;

		bcast_packet = (struct batadv_bcast_packet *)skb->data;
		bcast_packet->version = BATADV_COMPAT_VERSION;
		bcast_packet->ttl = BATADV_TTL;

		/* batman packet type: broadcast */
		bcast_packet->packet_type = BATADV_BCAST;
		bcast_packet->reserved = 0;

		/* hw address of first interface is the orig mac because only
		 * this mac is known throughout the mesh
		 */
>>>>>>> refs/remotes/origin/master
		memcpy(bcast_packet->orig,
		       primary_if->net_dev->dev_addr, ETH_ALEN);

		/* set broadcast sequence number */
<<<<<<< HEAD
		bcast_packet->seqno =
			htonl(atomic_inc_return(&bat_priv->bcast_seqno));

<<<<<<< HEAD
		add_bcast_packet_to_list(bat_priv, skb);
=======
		add_bcast_packet_to_list(bat_priv, skb, 1);
>>>>>>> refs/remotes/origin/cm-10.0

		/* a copy is stored in the bcast list, therefore removing
		 * the original skb. */
=======
		seqno = atomic_inc_return(&bat_priv->bcast_seqno);
		bcast_packet->seqno = htonl(seqno);

		batadv_add_bcast_packet_to_list(bat_priv, skb, brd_delay);

		/* a copy is stored in the bcast list, therefore removing
		 * the original skb.
		 */
>>>>>>> refs/remotes/origin/master
		kfree_skb(skb);

	/* unicast packet */
	} else {
<<<<<<< HEAD
<<<<<<< HEAD
=======
		if (atomic_read(&bat_priv->gw_mode) != GW_MODE_OFF) {
			ret = gw_out_of_range(bat_priv, skb, ethhdr);
=======
		if (atomic_read(&bat_priv->gw_mode) != BATADV_GW_MODE_OFF) {
			ret = batadv_gw_out_of_range(bat_priv, skb);
>>>>>>> refs/remotes/origin/master
			if (ret)
				goto dropped;
		}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		ret = unicast_send_skb(skb, bat_priv);
		if (ret != 0)
			goto dropped_freed;
	}

	bat_priv->stats.tx_packets++;
	bat_priv->stats.tx_bytes += data_len;
=======
		if (batadv_dat_snoop_outgoing_arp_request(bat_priv, skb))
			goto dropped;

		batadv_dat_snoop_outgoing_arp_reply(bat_priv, skb);

		if (is_multicast_ether_addr(ethhdr->h_dest))
			ret = batadv_send_skb_via_gw(bat_priv, skb, vid);
		else
			ret = batadv_send_skb_via_tt(bat_priv, skb, vid);

		if (ret == NET_XMIT_DROP)
			goto dropped_freed;
	}

	batadv_inc_counter(bat_priv, BATADV_CNT_TX);
	batadv_add_counter(bat_priv, BATADV_CNT_TX_BYTES, data_len);
>>>>>>> refs/remotes/origin/master
	goto end;

dropped:
	kfree_skb(skb);
dropped_freed:
<<<<<<< HEAD
	bat_priv->stats.tx_dropped++;
end:
	if (curr_softif_neigh)
		softif_neigh_free_ref(curr_softif_neigh);
	if (primary_if)
		hardif_free_ref(primary_if);
	return NETDEV_TX_OK;
}

void interface_rx(struct net_device *soft_iface,
		  struct sk_buff *skb, struct hard_iface *recv_if,
		  int hdr_size)
{
	struct bat_priv *bat_priv = netdev_priv(soft_iface);
	struct unicast_packet *unicast_packet;
	struct ethhdr *ethhdr;
	struct vlan_ethhdr *vhdr;
	struct softif_neigh *curr_softif_neigh = NULL;
	short vid = -1;
	int ret;
=======
	batadv_inc_counter(bat_priv, BATADV_CNT_TX_DROPPED);
end:
	if (primary_if)
		batadv_hardif_free_ref(primary_if);
	return NETDEV_TX_OK;
}

void batadv_interface_rx(struct net_device *soft_iface,
			 struct sk_buff *skb, struct batadv_hard_iface *recv_if,
			 int hdr_size, struct batadv_orig_node *orig_node)
{
	struct batadv_bcast_packet *batadv_bcast_packet;
	struct batadv_priv *bat_priv = netdev_priv(soft_iface);
	__be16 ethertype = htons(ETH_P_BATMAN);
	struct vlan_ethhdr *vhdr;
	struct ethhdr *ethhdr;
	unsigned short vid;
	bool is_bcast;

	batadv_bcast_packet = (struct batadv_bcast_packet *)skb->data;
	is_bcast = (batadv_bcast_packet->packet_type == BATADV_BCAST);
>>>>>>> refs/remotes/origin/master

	/* check if enough space is available for pulling, and pull */
	if (!pskb_may_pull(skb, hdr_size))
		goto dropped;

	skb_pull_rcsum(skb, hdr_size);
	skb_reset_mac_header(skb);

<<<<<<< HEAD
	ethhdr = (struct ethhdr *)skb_mac_header(skb);
=======
	/* clean the netfilter state now that the batman-adv header has been
	 * removed
	 */
	nf_reset(skb);

	vid = batadv_get_vid(skb, 0);
	ethhdr = eth_hdr(skb);
>>>>>>> refs/remotes/origin/master

	switch (ntohs(ethhdr->h_proto)) {
	case ETH_P_8021Q:
		vhdr = (struct vlan_ethhdr *)skb->data;
<<<<<<< HEAD
		vid = ntohs(vhdr->h_vlan_TCI) & VLAN_VID_MASK;

		if (ntohs(vhdr->h_vlan_encapsulated_proto) != ETH_P_BATMAN)
=======

		if (vhdr->h_vlan_encapsulated_proto != ethertype)
>>>>>>> refs/remotes/origin/master
			break;

		/* fall through */
	case ETH_P_BATMAN:
		goto dropped;
	}

<<<<<<< HEAD
	/**
	 * if we have a another chosen mesh exit node in range
	 * it will transport the packets to the non-mesh network
	 */
	curr_softif_neigh = softif_neigh_vid_get_selected(bat_priv, vid);
	if (curr_softif_neigh) {
		skb_push(skb, hdr_size);
		unicast_packet = (struct unicast_packet *)skb->data;

<<<<<<< HEAD
		if ((unicast_packet->packet_type != BAT_UNICAST) &&
		    (unicast_packet->packet_type != BAT_UNICAST_FRAG))
=======
		if ((unicast_packet->header.packet_type != BAT_UNICAST) &&
		    (unicast_packet->header.packet_type != BAT_UNICAST_FRAG))
>>>>>>> refs/remotes/origin/cm-10.0
			goto dropped;

		skb_reset_mac_header(skb);

		memcpy(unicast_packet->dest,
		       curr_softif_neigh->addr, ETH_ALEN);
		ret = route_unicast_packet(skb, recv_if);
		if (ret == NET_RX_DROP)
			goto dropped;

		goto out;
	}

=======
>>>>>>> refs/remotes/origin/master
	/* skb->dev & skb->pkt_type are set here */
	if (unlikely(!pskb_may_pull(skb, ETH_HLEN)))
		goto dropped;
	skb->protocol = eth_type_trans(skb, soft_iface);

	/* should not be necessary anymore as we use skb_pull_rcsum()
	 * TODO: please verify this and remove this TODO
<<<<<<< HEAD
	 * -- Dec 21st 2009, Simon Wunderlich */

/*	skb->ip_summed = CHECKSUM_UNNECESSARY;*/

	bat_priv->stats.rx_packets++;
	bat_priv->stats.rx_bytes += skb->len + sizeof(struct ethhdr);

	soft_iface->last_rx = jiffies;

<<<<<<< HEAD
=======
	if (is_ap_isolated(bat_priv, ethhdr->h_source, ethhdr->h_dest))
		goto dropped;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	 * -- Dec 21st 2009, Simon Wunderlich
	 */

	/* skb->ip_summed = CHECKSUM_UNNECESSARY; */

	batadv_inc_counter(bat_priv, BATADV_CNT_RX);
	batadv_add_counter(bat_priv, BATADV_CNT_RX_BYTES,
			   skb->len + ETH_HLEN);

	soft_iface->last_rx = jiffies;

	/* Let the bridge loop avoidance check the packet. If will
	 * not handle it, we can safely push it up.
	 */
	if (batadv_bla_rx(bat_priv, skb, vid, is_bcast))
		goto out;

	if (orig_node)
		batadv_tt_add_temporary_global_entry(bat_priv, orig_node,
						     ethhdr->h_source, vid);

	if (batadv_is_ap_isolated(bat_priv, ethhdr->h_source, ethhdr->h_dest,
				  vid))
		goto dropped;

>>>>>>> refs/remotes/origin/master
	netif_rx(skb);
	goto out;

dropped:
	kfree_skb(skb);
out:
<<<<<<< HEAD
	if (curr_softif_neigh)
		softif_neigh_free_ref(curr_softif_neigh);
	return;
}

<<<<<<< HEAD
#ifdef HAVE_NET_DEVICE_OPS
=======
>>>>>>> refs/remotes/origin/cm-10.0
static const struct net_device_ops bat_netdev_ops = {
	.ndo_open = interface_open,
	.ndo_stop = interface_release,
	.ndo_get_stats = interface_stats,
	.ndo_set_mac_address = interface_set_mac_addr,
	.ndo_change_mtu = interface_change_mtu,
	.ndo_start_xmit = interface_tx,
	.ndo_validate_addr = eth_validate_addr
};
<<<<<<< HEAD
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0

static void interface_setup(struct net_device *dev)
{
	struct bat_priv *priv = netdev_priv(dev);
<<<<<<< HEAD
	char dev_addr[ETH_ALEN];

	ether_setup(dev);

#ifdef HAVE_NET_DEVICE_OPS
	dev->netdev_ops = &bat_netdev_ops;
#else
	dev->open = interface_open;
	dev->stop = interface_release;
	dev->get_stats = interface_stats;
	dev->set_mac_address = interface_set_mac_addr;
	dev->change_mtu = interface_change_mtu;
	dev->hard_start_xmit = interface_tx;
#endif
=======

	ether_setup(dev);

	dev->netdev_ops = &bat_netdev_ops;
>>>>>>> refs/remotes/origin/cm-10.0
	dev->destructor = free_netdev;
	dev->tx_queue_len = 0;

	/**
	 * can't call min_mtu, because the needed variables
	 * have not been initialized yet
	 */
	dev->mtu = ETH_DATA_LEN;
	/* reserve more space in the skbuff for our header */
	dev->hard_header_len = BAT_HEADER_LEN;

	/* generate random address */
<<<<<<< HEAD
	random_ether_addr(dev_addr);
	memcpy(dev->dev_addr, dev_addr, ETH_ALEN);

	SET_ETHTOOL_OPS(dev, &bat_ethtool_ops);

	memset(priv, 0, sizeof(struct bat_priv));
}

struct net_device *softif_create(char *name)
=======
	eth_hw_addr_random(dev);

	SET_ETHTOOL_OPS(dev, &bat_ethtool_ops);

	memset(priv, 0, sizeof(*priv));
}

struct net_device *softif_create(const char *name)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct net_device *soft_iface;
	struct bat_priv *bat_priv;
	int ret;

<<<<<<< HEAD
	soft_iface = alloc_netdev(sizeof(struct bat_priv) , name,
				   interface_setup);

	if (!soft_iface) {
		pr_err("Unable to allocate the batman interface: %s\n", name);
		goto out;
	}
=======
	soft_iface = alloc_netdev(sizeof(*bat_priv), name, interface_setup);

	if (!soft_iface)
		goto out;
>>>>>>> refs/remotes/origin/cm-10.0

	ret = register_netdevice(soft_iface);
	if (ret < 0) {
		pr_err("Unable to register the batman interface '%s': %i\n",
		       name, ret);
		goto free_soft_iface;
	}

	bat_priv = netdev_priv(soft_iface);

	atomic_set(&bat_priv->aggregated_ogms, 1);
	atomic_set(&bat_priv->bonding, 0);
<<<<<<< HEAD
=======
	atomic_set(&bat_priv->ap_isolation, 0);
>>>>>>> refs/remotes/origin/cm-10.0
	atomic_set(&bat_priv->vis_mode, VIS_TYPE_CLIENT_UPDATE);
	atomic_set(&bat_priv->gw_mode, GW_MODE_OFF);
	atomic_set(&bat_priv->gw_sel_class, 20);
	atomic_set(&bat_priv->gw_bandwidth, 41);
	atomic_set(&bat_priv->orig_interval, 1000);
	atomic_set(&bat_priv->hop_penalty, 10);
	atomic_set(&bat_priv->log_level, 0);
	atomic_set(&bat_priv->fragmentation, 1);
	atomic_set(&bat_priv->bcast_queue_left, BCAST_QUEUE_LEN);
	atomic_set(&bat_priv->batman_queue_left, BATMAN_QUEUE_LEN);

	atomic_set(&bat_priv->mesh_state, MESH_INACTIVE);
	atomic_set(&bat_priv->bcast_seqno, 1);
<<<<<<< HEAD
	atomic_set(&bat_priv->tt_local_changed, 0);
=======
	atomic_set(&bat_priv->ttvn, 0);
	atomic_set(&bat_priv->tt_local_changes, 0);
	atomic_set(&bat_priv->tt_ogm_append_cnt, 0);

	bat_priv->tt_buff = NULL;
	bat_priv->tt_buff_len = 0;
	bat_priv->tt_poss_change = false;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return;
}

/**
 * batadv_softif_vlan_free_ref - decrease the vlan object refcounter and
 *  possibly free it
 * @softif_vlan: the vlan object to release
 */
void batadv_softif_vlan_free_ref(struct batadv_softif_vlan *softif_vlan)
{
	if (atomic_dec_and_test(&softif_vlan->refcount))
		kfree_rcu(softif_vlan, rcu);
}

/**
 * batadv_softif_vlan_get - get the vlan object for a specific vid
 * @bat_priv: the bat priv with all the soft interface information
 * @vid: the identifier of the vlan object to retrieve
 *
 * Returns the private data of the vlan matching the vid passed as argument or
 * NULL otherwise. The refcounter of the returned object is incremented by 1.
 */
struct batadv_softif_vlan *batadv_softif_vlan_get(struct batadv_priv *bat_priv,
						  unsigned short vid)
{
	struct batadv_softif_vlan *vlan_tmp, *vlan = NULL;

	rcu_read_lock();
	hlist_for_each_entry_rcu(vlan_tmp, &bat_priv->softif_vlan_list, list) {
		if (vlan_tmp->vid != vid)
			continue;

		if (!atomic_inc_not_zero(&vlan_tmp->refcount))
			continue;

		vlan = vlan_tmp;
		break;
	}
	rcu_read_unlock();

	return vlan;
}

/**
 * batadv_create_vlan - allocate the needed resources for a new vlan
 * @bat_priv: the bat priv with all the soft interface information
 * @vid: the VLAN identifier
 *
 * Returns 0 on success, a negative error otherwise.
 */
int batadv_softif_create_vlan(struct batadv_priv *bat_priv, unsigned short vid)
{
	struct batadv_softif_vlan *vlan;
	int err;

	vlan = batadv_softif_vlan_get(bat_priv, vid);
	if (vlan) {
		batadv_softif_vlan_free_ref(vlan);
		return -EEXIST;
	}

	vlan = kzalloc(sizeof(*vlan), GFP_ATOMIC);
	if (!vlan)
		return -ENOMEM;

	vlan->vid = vid;
	atomic_set(&vlan->refcount, 1);

	atomic_set(&vlan->ap_isolation, 0);

	err = batadv_sysfs_add_vlan(bat_priv->soft_iface, vlan);
	if (err) {
		kfree(vlan);
		return err;
	}

	/* add a new TT local entry. This one will be marked with the NOPURGE
	 * flag
	 */
	batadv_tt_local_add(bat_priv->soft_iface,
			    bat_priv->soft_iface->dev_addr, vid,
			    BATADV_NULL_IFINDEX);

	spin_lock_bh(&bat_priv->softif_vlan_list_lock);
	hlist_add_head_rcu(&vlan->list, &bat_priv->softif_vlan_list);
	spin_unlock_bh(&bat_priv->softif_vlan_list_lock);

	return 0;
}

/**
 * batadv_softif_destroy_vlan - remove and destroy a softif_vlan object
 * @bat_priv: the bat priv with all the soft interface information
 * @vlan: the object to remove
 */
static void batadv_softif_destroy_vlan(struct batadv_priv *bat_priv,
				       struct batadv_softif_vlan *vlan)
{
	spin_lock_bh(&bat_priv->softif_vlan_list_lock);
	hlist_del_rcu(&vlan->list);
	spin_unlock_bh(&bat_priv->softif_vlan_list_lock);

	batadv_sysfs_del_vlan(bat_priv, vlan);

	/* explicitly remove the associated TT local entry because it is marked
	 * with the NOPURGE flag
	 */
	batadv_tt_local_remove(bat_priv, bat_priv->soft_iface->dev_addr,
			       vlan->vid, "vlan interface destroyed", false);

	batadv_softif_vlan_free_ref(vlan);
}

/**
 * batadv_interface_add_vid - ndo_add_vid API implementation
 * @dev: the netdev of the mesh interface
 * @vid: identifier of the new vlan
 *
 * Set up all the internal structures for handling the new vlan on top of the
 * mesh interface
 *
 * Returns 0 on success or a negative error code in case of failure.
 */
static int batadv_interface_add_vid(struct net_device *dev, __be16 proto,
				    unsigned short vid)
{
	struct batadv_priv *bat_priv = netdev_priv(dev);

	/* only 802.1Q vlans are supported.
	 * batman-adv does not know how to handle other types
	 */
	if (proto != htons(ETH_P_8021Q))
		return -EINVAL;

	vid |= BATADV_VLAN_HAS_TAG;

	return batadv_softif_create_vlan(bat_priv, vid);
}

/**
 * batadv_interface_kill_vid - ndo_kill_vid API implementation
 * @dev: the netdev of the mesh interface
 * @vid: identifier of the deleted vlan
 *
 * Destroy all the internal structures used to handle the vlan identified by vid
 * on top of the mesh interface
 *
 * Returns 0 on success, -EINVAL if the specified prototype is not ETH_P_8021Q
 * or -ENOENT if the specified vlan id wasn't registered.
 */
static int batadv_interface_kill_vid(struct net_device *dev, __be16 proto,
				     unsigned short vid)
{
	struct batadv_priv *bat_priv = netdev_priv(dev);
	struct batadv_softif_vlan *vlan;

	/* only 802.1Q vlans are supported. batman-adv does not know how to
	 * handle other types
	 */
	if (proto != htons(ETH_P_8021Q))
		return -EINVAL;

	vlan = batadv_softif_vlan_get(bat_priv, vid | BATADV_VLAN_HAS_TAG);
	if (!vlan)
		return -ENOENT;

	batadv_softif_destroy_vlan(bat_priv, vlan);

	/* finally free the vlan object */
	batadv_softif_vlan_free_ref(vlan);

	return 0;
}

/* batman-adv network devices have devices nesting below it and are a special
 * "super class" of normal network devices; split their locks off into a
 * separate class since they always nest.
 */
static struct lock_class_key batadv_netdev_xmit_lock_key;
static struct lock_class_key batadv_netdev_addr_lock_key;

/**
 * batadv_set_lockdep_class_one - Set lockdep class for a single tx queue
 * @dev: device which owns the tx queue
 * @txq: tx queue to modify
 * @_unused: always NULL
 */
static void batadv_set_lockdep_class_one(struct net_device *dev,
					 struct netdev_queue *txq,
					 void *_unused)
{
	lockdep_set_class(&txq->_xmit_lock, &batadv_netdev_xmit_lock_key);
}

/**
 * batadv_set_lockdep_class - Set txq and addr_list lockdep class
 * @dev: network device to modify
 */
static void batadv_set_lockdep_class(struct net_device *dev)
{
	lockdep_set_class(&dev->addr_list_lock, &batadv_netdev_addr_lock_key);
	netdev_for_each_tx_queue(dev, batadv_set_lockdep_class_one, NULL);
}

/**
 * batadv_softif_destroy_finish - cleans up the remains of a softif
 * @work: work queue item
 *
 * Free the parts of the soft interface which can not be removed under
 * rtnl lock (to prevent deadlock situations).
 */
static void batadv_softif_destroy_finish(struct work_struct *work)
{
	struct batadv_softif_vlan *vlan;
	struct batadv_priv *bat_priv;
	struct net_device *soft_iface;

	bat_priv = container_of(work, struct batadv_priv,
				cleanup_work);
	soft_iface = bat_priv->soft_iface;

	/* destroy the "untagged" VLAN */
	vlan = batadv_softif_vlan_get(bat_priv, BATADV_NO_FLAGS);
	if (vlan) {
		batadv_softif_destroy_vlan(bat_priv, vlan);
		batadv_softif_vlan_free_ref(vlan);
	}

	batadv_sysfs_del_meshif(soft_iface);

	rtnl_lock();
	unregister_netdevice(soft_iface);
	rtnl_unlock();
}

/**
 * batadv_softif_init_late - late stage initialization of soft interface
 * @dev: registered network device to modify
 *
 * Returns error code on failures
 */
static int batadv_softif_init_late(struct net_device *dev)
{
	struct batadv_priv *bat_priv;
	uint32_t random_seqno;
	int ret;
	size_t cnt_len = sizeof(uint64_t) * BATADV_CNT_NUM;

	batadv_set_lockdep_class(dev);

	bat_priv = netdev_priv(dev);
	bat_priv->soft_iface = dev;
	INIT_WORK(&bat_priv->cleanup_work, batadv_softif_destroy_finish);

	/* batadv_interface_stats() needs to be available as soon as
	 * register_netdevice() has been called
	 */
	bat_priv->bat_counters = __alloc_percpu(cnt_len, __alignof__(uint64_t));
	if (!bat_priv->bat_counters)
		return -ENOMEM;

	atomic_set(&bat_priv->aggregated_ogms, 1);
	atomic_set(&bat_priv->bonding, 0);
#ifdef CONFIG_BATMAN_ADV_BLA
	atomic_set(&bat_priv->bridge_loop_avoidance, 0);
#endif
#ifdef CONFIG_BATMAN_ADV_DAT
	atomic_set(&bat_priv->distributed_arp_table, 1);
#endif
	atomic_set(&bat_priv->gw_mode, BATADV_GW_MODE_OFF);
	atomic_set(&bat_priv->gw_sel_class, 20);
	atomic_set(&bat_priv->gw.bandwidth_down, 100);
	atomic_set(&bat_priv->gw.bandwidth_up, 20);
	atomic_set(&bat_priv->orig_interval, 1000);
	atomic_set(&bat_priv->hop_penalty, 30);
#ifdef CONFIG_BATMAN_ADV_DEBUG
	atomic_set(&bat_priv->log_level, 0);
#endif
	atomic_set(&bat_priv->fragmentation, 1);
	atomic_set(&bat_priv->packet_size_max, ETH_DATA_LEN);
	atomic_set(&bat_priv->bcast_queue_left, BATADV_BCAST_QUEUE_LEN);
	atomic_set(&bat_priv->batman_queue_left, BATADV_BATMAN_QUEUE_LEN);

	atomic_set(&bat_priv->mesh_state, BATADV_MESH_INACTIVE);
	atomic_set(&bat_priv->bcast_seqno, 1);
	atomic_set(&bat_priv->tt.vn, 0);
	atomic_set(&bat_priv->tt.local_changes, 0);
	atomic_set(&bat_priv->tt.ogm_append_cnt, 0);
#ifdef CONFIG_BATMAN_ADV_BLA
	atomic_set(&bat_priv->bla.num_requests, 0);
#endif
	bat_priv->tt.last_changeset = NULL;
	bat_priv->tt.last_changeset_len = 0;

	/* randomize initial seqno to avoid collision */
	get_random_bytes(&random_seqno, sizeof(random_seqno));
	atomic_set(&bat_priv->frag_seqno, random_seqno);
>>>>>>> refs/remotes/origin/master

	bat_priv->primary_if = NULL;
	bat_priv->num_ifaces = 0;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	ret = bat_algo_select(bat_priv, bat_routing_algo);
	if (ret < 0)
		goto unreg_soft_iface;

>>>>>>> refs/remotes/origin/cm-10.0
	ret = sysfs_add_meshif(soft_iface);
	if (ret < 0)
		goto unreg_soft_iface;

	ret = debugfs_add_meshif(soft_iface);
	if (ret < 0)
		goto unreg_sysfs;

	ret = mesh_init(soft_iface);
	if (ret < 0)
		goto unreg_debugfs;

	return soft_iface;

unreg_debugfs:
	debugfs_del_meshif(soft_iface);
unreg_sysfs:
	sysfs_del_meshif(soft_iface);
unreg_soft_iface:
<<<<<<< HEAD
	unregister_netdev(soft_iface);
=======
	unregister_netdevice(soft_iface);
>>>>>>> refs/remotes/origin/cm-10.0
	return NULL;

free_soft_iface:
	free_netdev(soft_iface);
out:
	return NULL;
}

void softif_destroy(struct net_device *soft_iface)
{
	debugfs_del_meshif(soft_iface);
	sysfs_del_meshif(soft_iface);
	mesh_free(soft_iface);
	unregister_netdevice(soft_iface);
}

<<<<<<< HEAD
int softif_is_valid(struct net_device *net_dev)
{
#ifdef HAVE_NET_DEVICE_OPS
	if (net_dev->netdev_ops->ndo_start_xmit == interface_tx)
		return 1;
#else
	if (net_dev->hard_start_xmit == interface_tx)
		return 1;
#endif
=======
int softif_is_valid(const struct net_device *net_dev)
{
	if (net_dev->netdev_ops->ndo_start_xmit == interface_tx)
		return 1;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	batadv_nc_init_bat_priv(bat_priv);

	ret = batadv_algo_select(bat_priv, batadv_routing_algo);
	if (ret < 0)
		goto free_bat_counters;

	ret = batadv_debugfs_add_meshif(dev);
	if (ret < 0)
		goto free_bat_counters;

	ret = batadv_mesh_init(dev);
	if (ret < 0)
		goto unreg_debugfs;

	return 0;

unreg_debugfs:
	batadv_debugfs_del_meshif(dev);
free_bat_counters:
	free_percpu(bat_priv->bat_counters);
	bat_priv->bat_counters = NULL;

	return ret;
}

/**
 * batadv_softif_slave_add - Add a slave interface to a batadv_soft_interface
 * @dev: batadv_soft_interface used as master interface
 * @slave_dev: net_device which should become the slave interface
 *
 * Return 0 if successful or error otherwise.
 */
static int batadv_softif_slave_add(struct net_device *dev,
				   struct net_device *slave_dev)
{
	struct batadv_hard_iface *hard_iface;
	int ret = -EINVAL;

	hard_iface = batadv_hardif_get_by_netdev(slave_dev);
	if (!hard_iface || hard_iface->soft_iface != NULL)
		goto out;

	ret = batadv_hardif_enable_interface(hard_iface, dev->name);

out:
	if (hard_iface)
		batadv_hardif_free_ref(hard_iface);
	return ret;
}

/**
 * batadv_softif_slave_del - Delete a slave iface from a batadv_soft_interface
 * @dev: batadv_soft_interface used as master interface
 * @slave_dev: net_device which should be removed from the master interface
 *
 * Return 0 if successful or error otherwise.
 */
static int batadv_softif_slave_del(struct net_device *dev,
				   struct net_device *slave_dev)
{
	struct batadv_hard_iface *hard_iface;
	int ret = -EINVAL;

	hard_iface = batadv_hardif_get_by_netdev(slave_dev);

	if (!hard_iface || hard_iface->soft_iface != dev)
		goto out;

	batadv_hardif_disable_interface(hard_iface, BATADV_IF_CLEANUP_KEEP);
	ret = 0;

out:
	if (hard_iface)
		batadv_hardif_free_ref(hard_iface);
	return ret;
}

static const struct net_device_ops batadv_netdev_ops = {
	.ndo_init = batadv_softif_init_late,
	.ndo_open = batadv_interface_open,
	.ndo_stop = batadv_interface_release,
	.ndo_get_stats = batadv_interface_stats,
	.ndo_vlan_rx_add_vid = batadv_interface_add_vid,
	.ndo_vlan_rx_kill_vid = batadv_interface_kill_vid,
	.ndo_set_mac_address = batadv_interface_set_mac_addr,
	.ndo_change_mtu = batadv_interface_change_mtu,
	.ndo_set_rx_mode = batadv_interface_set_rx_mode,
	.ndo_start_xmit = batadv_interface_tx,
	.ndo_validate_addr = eth_validate_addr,
	.ndo_add_slave = batadv_softif_slave_add,
	.ndo_del_slave = batadv_softif_slave_del,
};

/**
 * batadv_softif_free - Deconstructor of batadv_soft_interface
 * @dev: Device to cleanup and remove
 */
static void batadv_softif_free(struct net_device *dev)
{
	batadv_debugfs_del_meshif(dev);
	batadv_mesh_free(dev);

	/* some scheduled RCU callbacks need the bat_priv struct to accomplish
	 * their tasks. Wait for them all to be finished before freeing the
	 * netdev and its private data (bat_priv)
	 */
	rcu_barrier();

	free_netdev(dev);
}

/**
 * batadv_softif_init_early - early stage initialization of soft interface
 * @dev: registered network device to modify
 */
static void batadv_softif_init_early(struct net_device *dev)
{
	struct batadv_priv *priv = netdev_priv(dev);

	ether_setup(dev);

	dev->netdev_ops = &batadv_netdev_ops;
	dev->destructor = batadv_softif_free;
	dev->features |= NETIF_F_HW_VLAN_CTAG_FILTER;
	dev->tx_queue_len = 0;

	/* can't call min_mtu, because the needed variables
	 * have not been initialized yet
	 */
	dev->mtu = ETH_DATA_LEN;
	/* reserve more space in the skbuff for our header */
	dev->hard_header_len = batadv_max_header_len();

	/* generate random address */
	eth_hw_addr_random(dev);

	SET_ETHTOOL_OPS(dev, &batadv_ethtool_ops);

	memset(priv, 0, sizeof(*priv));
}

struct net_device *batadv_softif_create(const char *name)
{
	struct net_device *soft_iface;
	int ret;

	soft_iface = alloc_netdev(sizeof(struct batadv_priv), name,
				  batadv_softif_init_early);
	if (!soft_iface)
		return NULL;

	soft_iface->rtnl_link_ops = &batadv_link_ops;

	ret = register_netdevice(soft_iface);
	if (ret < 0) {
		pr_err("Unable to register the batman interface '%s': %i\n",
		       name, ret);
		free_netdev(soft_iface);
		return NULL;
	}

	return soft_iface;
}

/**
 * batadv_softif_destroy_sysfs - deletion of batadv_soft_interface via sysfs
 * @soft_iface: the to-be-removed batman-adv interface
 */
void batadv_softif_destroy_sysfs(struct net_device *soft_iface)
{
	struct batadv_priv *bat_priv = netdev_priv(soft_iface);

	queue_work(batadv_event_workqueue, &bat_priv->cleanup_work);
}

/**
 * batadv_softif_destroy_netlink - deletion of batadv_soft_interface via netlink
 * @soft_iface: the to-be-removed batman-adv interface
 * @head: list pointer
 */
static void batadv_softif_destroy_netlink(struct net_device *soft_iface,
					  struct list_head *head)
{
	struct batadv_hard_iface *hard_iface;

	list_for_each_entry(hard_iface, &batadv_hardif_list, list) {
		if (hard_iface->soft_iface == soft_iface)
			batadv_hardif_disable_interface(hard_iface,
							BATADV_IF_CLEANUP_KEEP);
	}

	batadv_sysfs_del_meshif(soft_iface);
	unregister_netdevice_queue(soft_iface, head);
}

int batadv_softif_is_valid(const struct net_device *net_dev)
{
	if (net_dev->netdev_ops->ndo_start_xmit == batadv_interface_tx)
		return 1;
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
/* ethtool */
static int bat_get_settings(struct net_device *dev, struct ethtool_cmd *cmd)
=======
struct rtnl_link_ops batadv_link_ops __read_mostly = {
	.kind		= "batadv",
	.priv_size	= sizeof(struct batadv_priv),
	.setup		= batadv_softif_init_early,
	.dellink	= batadv_softif_destroy_netlink,
};

/* ethtool */
static int batadv_get_settings(struct net_device *dev, struct ethtool_cmd *cmd)
>>>>>>> refs/remotes/origin/master
{
	cmd->supported = 0;
	cmd->advertising = 0;
	ethtool_cmd_speed_set(cmd, SPEED_10);
	cmd->duplex = DUPLEX_FULL;
	cmd->port = PORT_TP;
	cmd->phy_address = 0;
	cmd->transceiver = XCVR_INTERNAL;
	cmd->autoneg = AUTONEG_DISABLE;
	cmd->maxtxpkt = 0;
	cmd->maxrxpkt = 0;

	return 0;
}

<<<<<<< HEAD
static void bat_get_drvinfo(struct net_device *dev,
			    struct ethtool_drvinfo *info)
{
	strcpy(info->driver, "B.A.T.M.A.N. advanced");
	strcpy(info->version, SOURCE_VERSION);
	strcpy(info->fw_version, "N/A");
	strcpy(info->bus_info, "batman");
}

static u32 bat_get_msglevel(struct net_device *dev)
=======
static void batadv_get_drvinfo(struct net_device *dev,
			       struct ethtool_drvinfo *info)
{
	strlcpy(info->driver, "B.A.T.M.A.N. advanced", sizeof(info->driver));
	strlcpy(info->version, BATADV_SOURCE_VERSION, sizeof(info->version));
	strlcpy(info->fw_version, "N/A", sizeof(info->fw_version));
	strlcpy(info->bus_info, "batman", sizeof(info->bus_info));
}

static u32 batadv_get_msglevel(struct net_device *dev)
>>>>>>> refs/remotes/origin/master
{
	return -EOPNOTSUPP;
}

<<<<<<< HEAD
static void bat_set_msglevel(struct net_device *dev, u32 value)
{
}

static u32 bat_get_link(struct net_device *dev)
{
	return 1;
}
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void batadv_set_msglevel(struct net_device *dev, u32 value)
{
}

static u32 batadv_get_link(struct net_device *dev)
{
	return 1;
}

/* Inspired by drivers/net/ethernet/dlink/sundance.c:1702
 * Declare each description string in struct.name[] to get fixed sized buffer
 * and compile time checking for strings longer than ETH_GSTRING_LEN.
 */
static const struct {
	const char name[ETH_GSTRING_LEN];
} batadv_counters_strings[] = {
	{ "tx" },
	{ "tx_bytes" },
	{ "tx_dropped" },
	{ "rx" },
	{ "rx_bytes" },
	{ "forward" },
	{ "forward_bytes" },
	{ "mgmt_tx" },
	{ "mgmt_tx_bytes" },
	{ "mgmt_rx" },
	{ "mgmt_rx_bytes" },
	{ "frag_tx" },
	{ "frag_tx_bytes" },
	{ "frag_rx" },
	{ "frag_rx_bytes" },
	{ "frag_fwd" },
	{ "frag_fwd_bytes" },
	{ "tt_request_tx" },
	{ "tt_request_rx" },
	{ "tt_response_tx" },
	{ "tt_response_rx" },
	{ "tt_roam_adv_tx" },
	{ "tt_roam_adv_rx" },
#ifdef CONFIG_BATMAN_ADV_DAT
	{ "dat_get_tx" },
	{ "dat_get_rx" },
	{ "dat_put_tx" },
	{ "dat_put_rx" },
	{ "dat_cached_reply_tx" },
#endif
#ifdef CONFIG_BATMAN_ADV_NC
	{ "nc_code" },
	{ "nc_code_bytes" },
	{ "nc_recode" },
	{ "nc_recode_bytes" },
	{ "nc_buffer" },
	{ "nc_decode" },
	{ "nc_decode_bytes" },
	{ "nc_decode_failed" },
	{ "nc_sniffed" },
#endif
};

static void batadv_get_strings(struct net_device *dev, uint32_t stringset,
			       uint8_t *data)
{
	if (stringset == ETH_SS_STATS)
		memcpy(data, batadv_counters_strings,
		       sizeof(batadv_counters_strings));
}

static void batadv_get_ethtool_stats(struct net_device *dev,
				     struct ethtool_stats *stats,
				     uint64_t *data)
{
	struct batadv_priv *bat_priv = netdev_priv(dev);
	int i;

	for (i = 0; i < BATADV_CNT_NUM; i++)
		data[i] = batadv_sum_counter(bat_priv, i);
}

static int batadv_get_sset_count(struct net_device *dev, int stringset)
{
	if (stringset == ETH_SS_STATS)
		return BATADV_CNT_NUM;

	return -EOPNOTSUPP;
}
>>>>>>> refs/remotes/origin/master
