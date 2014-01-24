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
 */

#include "main.h"
=======
 */

#include "main.h"
#include "distributed-arp-table.h"
>>>>>>> refs/remotes/origin/master
#include "hard-interface.h"
#include "soft-interface.h"
#include "send.h"
#include "translation-table.h"
#include "routing.h"
<<<<<<< HEAD
#include "bat_sysfs.h"
#include "originator.h"
#include "hash.h"

#include <linux/if_arp.h>


static int batman_skb_recv(struct sk_buff *skb,
			   struct net_device *dev,
			   struct packet_type *ptype,
			   struct net_device *orig_dev);

void hardif_free_rcu(struct rcu_head *rcu)
{
	struct hard_iface *hard_iface;

	hard_iface = container_of(rcu, struct hard_iface, rcu);
=======
#include "sysfs.h"
#include "originator.h"
#include "hash.h"
#include "bridge_loop_avoidance.h"
#include "gateway_client.h"

#include <linux/if_arp.h>
#include <linux/if_ether.h>

void batadv_hardif_free_rcu(struct rcu_head *rcu)
{
	struct batadv_hard_iface *hard_iface;

	hard_iface = container_of(rcu, struct batadv_hard_iface, rcu);
>>>>>>> refs/remotes/origin/master
	dev_put(hard_iface->net_dev);
	kfree(hard_iface);
}

<<<<<<< HEAD
<<<<<<< HEAD
struct hard_iface *hardif_get_by_netdev(struct net_device *net_dev)
=======
struct hard_iface *hardif_get_by_netdev(const struct net_device *net_dev)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct hard_iface *hard_iface;

	rcu_read_lock();
	list_for_each_entry_rcu(hard_iface, &hardif_list, list) {
=======
struct batadv_hard_iface *
batadv_hardif_get_by_netdev(const struct net_device *net_dev)
{
	struct batadv_hard_iface *hard_iface;

	rcu_read_lock();
	list_for_each_entry_rcu(hard_iface, &batadv_hardif_list, list) {
>>>>>>> refs/remotes/origin/master
		if (hard_iface->net_dev == net_dev &&
		    atomic_inc_not_zero(&hard_iface->refcount))
			goto out;
	}

	hard_iface = NULL;

out:
	rcu_read_unlock();
	return hard_iface;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int is_valid_iface(struct net_device *net_dev)
=======
static int is_valid_iface(const struct net_device *net_dev)
>>>>>>> refs/remotes/origin/cm-10.0
=======
/**
 * batadv_is_on_batman_iface - check if a device is a batman iface descendant
 * @net_dev: the device to check
 *
 * If the user creates any virtual device on top of a batman-adv interface, it
 * is important to prevent this new interface to be used to create a new mesh
 * network (this behaviour would lead to a batman-over-batman configuration).
 * This function recursively checks all the fathers of the device passed as
 * argument looking for a batman-adv soft interface.
 *
 * Returns true if the device is descendant of a batman-adv mesh interface (or
 * if it is a batman-adv interface itself), false otherwise
 */
static bool batadv_is_on_batman_iface(const struct net_device *net_dev)
{
	struct net_device *parent_dev;
	bool ret;

	/* check if this is a batman-adv mesh interface */
	if (batadv_softif_is_valid(net_dev))
		return true;

	/* no more parents..stop recursion */
	if (net_dev->iflink == net_dev->ifindex)
		return false;

	/* recurse over the parent device */
	parent_dev = dev_get_by_index(&init_net, net_dev->iflink);
	/* if we got a NULL parent_dev there is something broken.. */
	if (WARN(!parent_dev, "Cannot find parent device"))
		return false;

	ret = batadv_is_on_batman_iface(parent_dev);

	if (parent_dev)
		dev_put(parent_dev);
	return ret;
}

static int batadv_is_valid_iface(const struct net_device *net_dev)
>>>>>>> refs/remotes/origin/master
{
	if (net_dev->flags & IFF_LOOPBACK)
		return 0;

	if (net_dev->type != ARPHRD_ETHER)
		return 0;

	if (net_dev->addr_len != ETH_ALEN)
		return 0;

	/* no batman over batman */
<<<<<<< HEAD
	if (softif_is_valid(net_dev))
		return 0;

	/* Device is being bridged */
	/* if (net_dev->priv_flags & IFF_BRIDGE_PORT)
		return 0; */

	return 1;
}

<<<<<<< HEAD
static struct hard_iface *hardif_get_active(struct net_device *soft_iface)
=======
static struct hard_iface *hardif_get_active(const struct net_device *soft_iface)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct hard_iface *hard_iface;

	rcu_read_lock();
	list_for_each_entry_rcu(hard_iface, &hardif_list, list) {
		if (hard_iface->soft_iface != soft_iface)
			continue;

		if (hard_iface->if_status == IF_ACTIVE &&
=======
	if (batadv_is_on_batman_iface(net_dev))
		return 0;

	return 1;
}

/**
 * batadv_is_wifi_netdev - check if the given net_device struct is a wifi
 *  interface
 * @net_device: the device to check
 *
 * Returns true if the net device is a 802.11 wireless device, false otherwise.
 */
bool batadv_is_wifi_netdev(struct net_device *net_device)
{
	if (!net_device)
		return false;

#ifdef CONFIG_WIRELESS_EXT
	/* pre-cfg80211 drivers have to implement WEXT, so it is possible to
	 * check for wireless_handlers != NULL
	 */
	if (net_device->wireless_handlers)
		return true;
#endif

	/* cfg80211 drivers have to set ieee80211_ptr */
	if (net_device->ieee80211_ptr)
		return true;

	return false;
}

static struct batadv_hard_iface *
batadv_hardif_get_active(const struct net_device *soft_iface)
{
	struct batadv_hard_iface *hard_iface;

	rcu_read_lock();
	list_for_each_entry_rcu(hard_iface, &batadv_hardif_list, list) {
		if (hard_iface->soft_iface != soft_iface)
			continue;

		if (hard_iface->if_status == BATADV_IF_ACTIVE &&
>>>>>>> refs/remotes/origin/master
		    atomic_inc_not_zero(&hard_iface->refcount))
			goto out;
	}

	hard_iface = NULL;

out:
	rcu_read_unlock();
	return hard_iface;
}

<<<<<<< HEAD
static void primary_if_update_addr(struct bat_priv *bat_priv)
{
	struct vis_packet *vis_packet;
	struct hard_iface *primary_if;

	primary_if = primary_if_get_selected(bat_priv);
	if (!primary_if)
		goto out;

	vis_packet = (struct vis_packet *)
				bat_priv->my_vis_info->skb_packet->data;
	memcpy(vis_packet->vis_orig, primary_if->net_dev->dev_addr, ETH_ALEN);
	memcpy(vis_packet->sender_orig,
	       primary_if->net_dev->dev_addr, ETH_ALEN);

out:
	if (primary_if)
		hardif_free_ref(primary_if);
}

static void primary_if_select(struct bat_priv *bat_priv,
			      struct hard_iface *new_hard_iface)
{
	struct hard_iface *curr_hard_iface;
<<<<<<< HEAD
	struct batman_packet *batman_packet;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void batadv_primary_if_update_addr(struct batadv_priv *bat_priv,
					  struct batadv_hard_iface *oldif)
{
	struct batadv_hard_iface *primary_if;

	primary_if = batadv_primary_if_get_selected(bat_priv);
	if (!primary_if)
		goto out;

	batadv_dat_init_own_addr(bat_priv, primary_if);
	batadv_bla_update_orig_address(bat_priv, primary_if, oldif);
out:
	if (primary_if)
		batadv_hardif_free_ref(primary_if);
}

static void batadv_primary_if_select(struct batadv_priv *bat_priv,
				     struct batadv_hard_iface *new_hard_iface)
{
	struct batadv_hard_iface *curr_hard_iface;
>>>>>>> refs/remotes/origin/master

	ASSERT_RTNL();

	if (new_hard_iface && !atomic_inc_not_zero(&new_hard_iface->refcount))
		new_hard_iface = NULL;

<<<<<<< HEAD
<<<<<<< HEAD
	curr_hard_iface = bat_priv->primary_if;
=======
	curr_hard_iface = rcu_dereference_protected(bat_priv->primary_if, 1);
>>>>>>> refs/remotes/origin/cm-10.0
	rcu_assign_pointer(bat_priv->primary_if, new_hard_iface);

	if (curr_hard_iface)
		hardif_free_ref(curr_hard_iface);

	if (!new_hard_iface)
		return;

<<<<<<< HEAD
	batman_packet = (struct batman_packet *)(new_hard_iface->packet_buff);
	batman_packet->flags = PRIMARIES_FIRST_HOP;
	batman_packet->ttl = TTL;

	primary_if_update_addr(bat_priv);

	/***
	 * hacky trick to make sure that we send the TT information via
	 * our new primary interface
	 */
	atomic_set(&bat_priv->tt_local_changed, 1);
}

static bool hardif_is_iface_up(struct hard_iface *hard_iface)
=======
	bat_priv->bat_algo_ops->bat_ogm_init_primary(new_hard_iface);
	primary_if_update_addr(bat_priv);
}

static bool hardif_is_iface_up(const struct hard_iface *hard_iface)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	curr_hard_iface = rcu_dereference_protected(bat_priv->primary_if, 1);
	rcu_assign_pointer(bat_priv->primary_if, new_hard_iface);

	if (!new_hard_iface)
		goto out;

	bat_priv->bat_algo_ops->bat_primary_iface_set(new_hard_iface);
	batadv_primary_if_update_addr(bat_priv, curr_hard_iface);

out:
	if (curr_hard_iface)
		batadv_hardif_free_ref(curr_hard_iface);
}

static bool
batadv_hardif_is_iface_up(const struct batadv_hard_iface *hard_iface)
>>>>>>> refs/remotes/origin/master
{
	if (hard_iface->net_dev->flags & IFF_UP)
		return true;

	return false;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void update_mac_addresses(struct hard_iface *hard_iface)
{
	memcpy(((struct batman_packet *)(hard_iface->packet_buff))->orig,
	       hard_iface->net_dev->dev_addr, ETH_ALEN);
	memcpy(((struct batman_packet *)(hard_iface->packet_buff))->prev_sender,
	       hard_iface->net_dev->dev_addr, ETH_ALEN);
}

static void check_known_mac_addr(struct net_device *net_dev)
{
	struct hard_iface *hard_iface;
=======
static void check_known_mac_addr(const struct net_device *net_dev)
{
	const struct hard_iface *hard_iface;
>>>>>>> refs/remotes/origin/cm-10.0

	rcu_read_lock();
	list_for_each_entry_rcu(hard_iface, &hardif_list, list) {
		if ((hard_iface->if_status != IF_ACTIVE) &&
		    (hard_iface->if_status != IF_TO_BE_ACTIVATED))
=======
static void batadv_check_known_mac_addr(const struct net_device *net_dev)
{
	const struct batadv_hard_iface *hard_iface;

	rcu_read_lock();
	list_for_each_entry_rcu(hard_iface, &batadv_hardif_list, list) {
		if ((hard_iface->if_status != BATADV_IF_ACTIVE) &&
		    (hard_iface->if_status != BATADV_IF_TO_BE_ACTIVATED))
>>>>>>> refs/remotes/origin/master
			continue;

		if (hard_iface->net_dev == net_dev)
			continue;

<<<<<<< HEAD
		if (!compare_eth(hard_iface->net_dev->dev_addr,
				 net_dev->dev_addr))
			continue;

<<<<<<< HEAD
		pr_warning("The newly added mac address (%pM) already exists "
			   "on: %s\n", net_dev->dev_addr,
			   hard_iface->net_dev->name);
		pr_warning("It is strongly recommended to keep mac addresses "
			   "unique to avoid problems!\n");
=======
		pr_warning("The newly added mac address (%pM) already exists on: %s\n",
			   net_dev->dev_addr, hard_iface->net_dev->name);
		pr_warning("It is strongly recommended to keep mac addresses unique to avoid problems!\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (!batadv_compare_eth(hard_iface->net_dev->dev_addr,
					net_dev->dev_addr))
			continue;

		pr_warn("The newly added mac address (%pM) already exists on: %s\n",
			net_dev->dev_addr, hard_iface->net_dev->name);
		pr_warn("It is strongly recommended to keep mac addresses unique to avoid problems!\n");
>>>>>>> refs/remotes/origin/master
	}
	rcu_read_unlock();
}

<<<<<<< HEAD
int hardif_min_mtu(struct net_device *soft_iface)
{
<<<<<<< HEAD
	struct bat_priv *bat_priv = netdev_priv(soft_iface);
	struct hard_iface *hard_iface;
=======
	const struct bat_priv *bat_priv = netdev_priv(soft_iface);
	const struct hard_iface *hard_iface;
>>>>>>> refs/remotes/origin/cm-10.0
	/* allow big frames if all devices are capable to do so
	 * (have MTU > 1500 + BAT_HEADER_LEN) */
	int min_mtu = ETH_DATA_LEN;

	if (atomic_read(&bat_priv->fragmentation))
		goto out;

	rcu_read_lock();
	list_for_each_entry_rcu(hard_iface, &hardif_list, list) {
		if ((hard_iface->if_status != IF_ACTIVE) &&
		    (hard_iface->if_status != IF_TO_BE_ACTIVATED))
=======
int batadv_hardif_min_mtu(struct net_device *soft_iface)
{
	struct batadv_priv *bat_priv = netdev_priv(soft_iface);
	const struct batadv_hard_iface *hard_iface;
	int min_mtu = ETH_DATA_LEN;

	rcu_read_lock();
	list_for_each_entry_rcu(hard_iface, &batadv_hardif_list, list) {
		if ((hard_iface->if_status != BATADV_IF_ACTIVE) &&
		    (hard_iface->if_status != BATADV_IF_TO_BE_ACTIVATED))
>>>>>>> refs/remotes/origin/master
			continue;

		if (hard_iface->soft_iface != soft_iface)
			continue;

<<<<<<< HEAD
		min_mtu = min_t(int, hard_iface->net_dev->mtu - BAT_HEADER_LEN,
				min_mtu);
	}
	rcu_read_unlock();
out:
	return min_mtu;
}

/* adjusts the MTU if a new interface with a smaller MTU appeared. */
void update_min_mtu(struct net_device *soft_iface)
{
	int min_mtu;

	min_mtu = hardif_min_mtu(soft_iface);
	if (soft_iface->mtu != min_mtu)
		soft_iface->mtu = min_mtu;
}

static void hardif_activate_interface(struct hard_iface *hard_iface)
{
	struct bat_priv *bat_priv;
	struct hard_iface *primary_if = NULL;

	if (hard_iface->if_status != IF_INACTIVE)
=======
		min_mtu = min_t(int, hard_iface->net_dev->mtu, min_mtu);
	}
	rcu_read_unlock();

	atomic_set(&bat_priv->packet_size_max, min_mtu);

	if (atomic_read(&bat_priv->fragmentation) == 0)
		goto out;

	/* with fragmentation enabled the maximum size of internally generated
	 * packets such as translation table exchanges or tvlv containers, etc
	 * has to be calculated
	 */
	min_mtu = min_t(int, min_mtu, BATADV_FRAG_MAX_FRAG_SIZE);
	min_mtu -= sizeof(struct batadv_frag_packet);
	min_mtu *= BATADV_FRAG_MAX_FRAGMENTS;
	atomic_set(&bat_priv->packet_size_max, min_mtu);

	/* with fragmentation enabled we can fragment external packets easily */
	min_mtu = min_t(int, min_mtu, ETH_DATA_LEN);

out:
	return min_mtu - batadv_max_header_len();
}

/* adjusts the MTU if a new interface with a smaller MTU appeared. */
void batadv_update_min_mtu(struct net_device *soft_iface)
{
	soft_iface->mtu = batadv_hardif_min_mtu(soft_iface);

	/* Check if the local translate table should be cleaned up to match a
	 * new (and smaller) MTU.
	 */
	batadv_tt_local_resize_to_mtu(soft_iface);
}

static void
batadv_hardif_activate_interface(struct batadv_hard_iface *hard_iface)
{
	struct batadv_priv *bat_priv;
	struct batadv_hard_iface *primary_if = NULL;

	if (hard_iface->if_status != BATADV_IF_INACTIVE)
>>>>>>> refs/remotes/origin/master
		goto out;

	bat_priv = netdev_priv(hard_iface->soft_iface);

<<<<<<< HEAD
<<<<<<< HEAD
	update_mac_addresses(hard_iface);
=======
	bat_priv->bat_algo_ops->bat_ogm_update_mac(hard_iface);
>>>>>>> refs/remotes/origin/cm-10.0
	hard_iface->if_status = IF_TO_BE_ACTIVATED;

	/**
	 * the first active interface becomes our primary interface or
<<<<<<< HEAD
	 * the next active interface after the old primay interface was removed
=======
	 * the next active interface after the old primary interface was removed
>>>>>>> refs/remotes/origin/cm-10.0
	 */
	primary_if = primary_if_get_selected(bat_priv);
	if (!primary_if)
		primary_if_select(bat_priv, hard_iface);

	bat_info(hard_iface->soft_iface, "Interface activated: %s\n",
		 hard_iface->net_dev->name);

	update_min_mtu(hard_iface->soft_iface);

out:
	if (primary_if)
		hardif_free_ref(primary_if);
}

static void hardif_deactivate_interface(struct hard_iface *hard_iface)
{
	if ((hard_iface->if_status != IF_ACTIVE) &&
	    (hard_iface->if_status != IF_TO_BE_ACTIVATED))
		return;

	hard_iface->if_status = IF_INACTIVE;

	bat_info(hard_iface->soft_iface, "Interface deactivated: %s\n",
		 hard_iface->net_dev->name);

	update_min_mtu(hard_iface->soft_iface);
}

<<<<<<< HEAD
int hardif_enable_interface(struct hard_iface *hard_iface, char *iface_name)
{
	struct bat_priv *bat_priv;
	struct batman_packet *batman_packet;
=======
int hardif_enable_interface(struct hard_iface *hard_iface,
			    const char *iface_name)
{
	struct bat_priv *bat_priv;
>>>>>>> refs/remotes/origin/cm-10.0
	struct net_device *soft_iface;
	int ret;

	if (hard_iface->if_status != IF_NOT_IN_USE)
=======
	bat_priv->bat_algo_ops->bat_iface_update_mac(hard_iface);
	hard_iface->if_status = BATADV_IF_TO_BE_ACTIVATED;

	/* the first active interface becomes our primary interface or
	 * the next active interface after the old primary interface was removed
	 */
	primary_if = batadv_primary_if_get_selected(bat_priv);
	if (!primary_if)
		batadv_primary_if_select(bat_priv, hard_iface);

	batadv_info(hard_iface->soft_iface, "Interface activated: %s\n",
		    hard_iface->net_dev->name);

	batadv_update_min_mtu(hard_iface->soft_iface);

out:
	if (primary_if)
		batadv_hardif_free_ref(primary_if);
}

static void
batadv_hardif_deactivate_interface(struct batadv_hard_iface *hard_iface)
{
	if ((hard_iface->if_status != BATADV_IF_ACTIVE) &&
	    (hard_iface->if_status != BATADV_IF_TO_BE_ACTIVATED))
		return;

	hard_iface->if_status = BATADV_IF_INACTIVE;

	batadv_info(hard_iface->soft_iface, "Interface deactivated: %s\n",
		    hard_iface->net_dev->name);

	batadv_update_min_mtu(hard_iface->soft_iface);
}

/**
 * batadv_master_del_slave - remove hard_iface from the current master interface
 * @slave: the interface enslaved in another master
 * @master: the master from which slave has to be removed
 *
 * Invoke ndo_del_slave on master passing slave as argument. In this way slave
 * is free'd and master can correctly change its internal state.
 * Return 0 on success, a negative value representing the error otherwise
 */
static int batadv_master_del_slave(struct batadv_hard_iface *slave,
				   struct net_device *master)
{
	int ret;

	if (!master)
		return 0;

	ret = -EBUSY;
	if (master->netdev_ops->ndo_del_slave)
		ret = master->netdev_ops->ndo_del_slave(master, slave->net_dev);

	return ret;
}

int batadv_hardif_enable_interface(struct batadv_hard_iface *hard_iface,
				   const char *iface_name)
{
	struct batadv_priv *bat_priv;
	struct net_device *soft_iface, *master;
	__be16 ethertype = htons(ETH_P_BATMAN);
	int max_header_len = batadv_max_header_len();
	int ret;

	if (hard_iface->if_status != BATADV_IF_NOT_IN_USE)
>>>>>>> refs/remotes/origin/master
		goto out;

	if (!atomic_inc_not_zero(&hard_iface->refcount))
		goto out;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	/* hard-interface is part of a bridge */
	if (hard_iface->net_dev->priv_flags & IFF_BRIDGE_PORT)
		pr_err("You are about to enable batman-adv on '%s' which already is part of a bridge. Unless you know exactly what you are doing this is probably wrong and won't work the way you think it would.\n",
		       hard_iface->net_dev->name);

>>>>>>> refs/remotes/origin/cm-10.0
	soft_iface = dev_get_by_name(&init_net, iface_name);

	if (!soft_iface) {
		soft_iface = softif_create(iface_name);
=======
	soft_iface = dev_get_by_name(&init_net, iface_name);

	if (!soft_iface) {
		soft_iface = batadv_softif_create(iface_name);
>>>>>>> refs/remotes/origin/master

		if (!soft_iface) {
			ret = -ENOMEM;
			goto err;
		}

		/* dev_get_by_name() increases the reference counter for us */
		dev_hold(soft_iface);
	}

<<<<<<< HEAD
	if (!softif_is_valid(soft_iface)) {
<<<<<<< HEAD
		pr_err("Can't create batman mesh interface %s: "
		       "already exists as regular interface\n",
=======
		pr_err("Can't create batman mesh interface %s: already exists as regular interface\n",
>>>>>>> refs/remotes/origin/cm-10.0
		       soft_iface->name);
		dev_put(soft_iface);
		ret = -EINVAL;
		goto err;
	}

	hard_iface->soft_iface = soft_iface;
	bat_priv = netdev_priv(hard_iface->soft_iface);
<<<<<<< HEAD
	hard_iface->packet_len = BAT_PACKET_LEN;
	hard_iface->packet_buff = kmalloc(hard_iface->packet_len, GFP_ATOMIC);

	if (!hard_iface->packet_buff) {
		bat_err(hard_iface->soft_iface, "Can't add interface packet "
			"(%s): out of memory\n", hard_iface->net_dev->name);
=======

	bat_priv->bat_algo_ops->bat_ogm_init(hard_iface);

	if (!hard_iface->packet_buff) {
		bat_err(hard_iface->soft_iface,
			"Can't add interface packet (%s): out of memory\n",
			hard_iface->net_dev->name);
>>>>>>> refs/remotes/origin/cm-10.0
		ret = -ENOMEM;
		goto err;
	}

<<<<<<< HEAD
	batman_packet = (struct batman_packet *)(hard_iface->packet_buff);
	batman_packet->packet_type = BAT_PACKET;
	batman_packet->version = COMPAT_VERSION;
	batman_packet->flags = 0;
	batman_packet->ttl = 2;
	batman_packet->tq = TQ_MAX_VALUE;
	batman_packet->num_tt = 0;

=======
>>>>>>> refs/remotes/origin/cm-10.0
	hard_iface->if_num = bat_priv->num_ifaces;
	bat_priv->num_ifaces++;
	hard_iface->if_status = IF_INACTIVE;
	orig_hash_add_if(hard_iface, bat_priv->num_ifaces);

	hard_iface->batman_adv_ptype.type = __constant_htons(ETH_P_BATMAN);
	hard_iface->batman_adv_ptype.func = batman_skb_recv;
	hard_iface->batman_adv_ptype.dev = hard_iface->net_dev;
	dev_add_pack(&hard_iface->batman_adv_ptype);

	atomic_set(&hard_iface->seqno, 1);
	atomic_set(&hard_iface->frag_seqno, 1);
	bat_info(hard_iface->soft_iface, "Adding interface: %s\n",
		 hard_iface->net_dev->name);

	if (atomic_read(&bat_priv->fragmentation) && hard_iface->net_dev->mtu <
		ETH_DATA_LEN + BAT_HEADER_LEN)
		bat_info(hard_iface->soft_iface,
<<<<<<< HEAD
			"The MTU of interface %s is too small (%i) to handle "
			"the transport of batman-adv packets. Packets going "
			"over this interface will be fragmented on layer2 "
			"which could impact the performance. Setting the MTU "
			"to %zi would solve the problem.\n",
			hard_iface->net_dev->name, hard_iface->net_dev->mtu,
			ETH_DATA_LEN + BAT_HEADER_LEN);
=======
			 "The MTU of interface %s is too small (%i) to handle the transport of batman-adv packets. Packets going over this interface will be fragmented on layer2 which could impact the performance. Setting the MTU to %zi would solve the problem.\n",
			 hard_iface->net_dev->name, hard_iface->net_dev->mtu,
			 ETH_DATA_LEN + BAT_HEADER_LEN);
>>>>>>> refs/remotes/origin/cm-10.0

	if (!atomic_read(&bat_priv->fragmentation) && hard_iface->net_dev->mtu <
		ETH_DATA_LEN + BAT_HEADER_LEN)
		bat_info(hard_iface->soft_iface,
<<<<<<< HEAD
			"The MTU of interface %s is too small (%i) to handle "
			"the transport of batman-adv packets. If you experience"
			" problems getting traffic through try increasing the "
			"MTU to %zi.\n",
			hard_iface->net_dev->name, hard_iface->net_dev->mtu,
			ETH_DATA_LEN + BAT_HEADER_LEN);
=======
			 "The MTU of interface %s is too small (%i) to handle the transport of batman-adv packets. If you experience problems getting traffic through try increasing the MTU to %zi.\n",
			 hard_iface->net_dev->name, hard_iface->net_dev->mtu,
			 ETH_DATA_LEN + BAT_HEADER_LEN);
>>>>>>> refs/remotes/origin/cm-10.0

	if (hardif_is_iface_up(hard_iface))
		hardif_activate_interface(hard_iface);
	else
<<<<<<< HEAD
		bat_err(hard_iface->soft_iface, "Not using interface %s "
			"(retrying later): interface not active\n",
			hard_iface->net_dev->name);

	/* begin scheduling originator messages on that interface */
	schedule_own_packet(hard_iface);
=======
		bat_err(hard_iface->soft_iface,
			"Not using interface %s (retrying later): interface not active\n",
			hard_iface->net_dev->name);

	/* begin scheduling originator messages on that interface */
	schedule_bat_ogm(hard_iface);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!batadv_softif_is_valid(soft_iface)) {
		pr_err("Can't create batman mesh interface %s: already exists as regular interface\n",
		       soft_iface->name);
		ret = -EINVAL;
		goto err_dev;
	}

	/* check if the interface is enslaved in another virtual one and
	 * in that case unlink it first
	 */
	master = netdev_master_upper_dev_get(hard_iface->net_dev);
	ret = batadv_master_del_slave(hard_iface, master);
	if (ret)
		goto err_dev;

	hard_iface->soft_iface = soft_iface;
	bat_priv = netdev_priv(hard_iface->soft_iface);

	ret = netdev_master_upper_dev_link(hard_iface->net_dev, soft_iface);
	if (ret)
		goto err_dev;

	ret = bat_priv->bat_algo_ops->bat_iface_enable(hard_iface);
	if (ret < 0)
		goto err_upper;

	hard_iface->if_num = bat_priv->num_ifaces;
	bat_priv->num_ifaces++;
	hard_iface->if_status = BATADV_IF_INACTIVE;
	ret = batadv_orig_hash_add_if(hard_iface, bat_priv->num_ifaces);
	if (ret < 0) {
		bat_priv->bat_algo_ops->bat_iface_disable(hard_iface);
		bat_priv->num_ifaces--;
		hard_iface->if_status = BATADV_IF_NOT_IN_USE;
		goto err_upper;
	}

	hard_iface->batman_adv_ptype.type = ethertype;
	hard_iface->batman_adv_ptype.func = batadv_batman_skb_recv;
	hard_iface->batman_adv_ptype.dev = hard_iface->net_dev;
	dev_add_pack(&hard_iface->batman_adv_ptype);

	batadv_info(hard_iface->soft_iface, "Adding interface: %s\n",
		    hard_iface->net_dev->name);

	if (atomic_read(&bat_priv->fragmentation) &&
	    hard_iface->net_dev->mtu < ETH_DATA_LEN + max_header_len)
		batadv_info(hard_iface->soft_iface,
			    "The MTU of interface %s is too small (%i) to handle the transport of batman-adv packets. Packets going over this interface will be fragmented on layer2 which could impact the performance. Setting the MTU to %i would solve the problem.\n",
			    hard_iface->net_dev->name, hard_iface->net_dev->mtu,
			    ETH_DATA_LEN + max_header_len);

	if (!atomic_read(&bat_priv->fragmentation) &&
	    hard_iface->net_dev->mtu < ETH_DATA_LEN + max_header_len)
		batadv_info(hard_iface->soft_iface,
			    "The MTU of interface %s is too small (%i) to handle the transport of batman-adv packets. If you experience problems getting traffic through try increasing the MTU to %i.\n",
			    hard_iface->net_dev->name, hard_iface->net_dev->mtu,
			    ETH_DATA_LEN + max_header_len);

	if (batadv_hardif_is_iface_up(hard_iface))
		batadv_hardif_activate_interface(hard_iface);
	else
		batadv_err(hard_iface->soft_iface,
			   "Not using interface %s (retrying later): interface not active\n",
			   hard_iface->net_dev->name);

	/* begin scheduling originator messages on that interface */
	batadv_schedule_bat_ogm(hard_iface);
>>>>>>> refs/remotes/origin/master

out:
	return 0;

<<<<<<< HEAD
err:
	hardif_free_ref(hard_iface);
	return ret;
}

void hardif_disable_interface(struct hard_iface *hard_iface)
{
	struct bat_priv *bat_priv = netdev_priv(hard_iface->soft_iface);
	struct hard_iface *primary_if = NULL;

	if (hard_iface->if_status == IF_ACTIVE)
		hardif_deactivate_interface(hard_iface);

	if (hard_iface->if_status != IF_INACTIVE)
		goto out;

	bat_info(hard_iface->soft_iface, "Removing interface: %s\n",
		 hard_iface->net_dev->name);
	dev_remove_pack(&hard_iface->batman_adv_ptype);

	bat_priv->num_ifaces--;
	orig_hash_del_if(hard_iface, bat_priv->num_ifaces);

	primary_if = primary_if_get_selected(bat_priv);
	if (hard_iface == primary_if) {
		struct hard_iface *new_if;

		new_if = hardif_get_active(hard_iface->soft_iface);
		primary_if_select(bat_priv, new_if);

		if (new_if)
			hardif_free_ref(new_if);
	}

	kfree(hard_iface->packet_buff);
	hard_iface->packet_buff = NULL;
	hard_iface->if_status = IF_NOT_IN_USE;

	/* delete all references to this hard_iface */
	purge_orig_ref(bat_priv);
	purge_outstanding_packets(bat_priv, hard_iface);
	dev_put(hard_iface->soft_iface);

	/* nobody uses this interface anymore */
	if (!bat_priv->num_ifaces)
		softif_destroy(hard_iface->soft_iface);

	hard_iface->soft_iface = NULL;
	hardif_free_ref(hard_iface);

out:
	if (primary_if)
		hardif_free_ref(primary_if);
}

static struct hard_iface *hardif_add_interface(struct net_device *net_dev)
{
	struct hard_iface *hard_iface;
=======
err_upper:
	netdev_upper_dev_unlink(hard_iface->net_dev, soft_iface);
err_dev:
	hard_iface->soft_iface = NULL;
	dev_put(soft_iface);
err:
	batadv_hardif_free_ref(hard_iface);
	return ret;
}

void batadv_hardif_disable_interface(struct batadv_hard_iface *hard_iface,
				     enum batadv_hard_if_cleanup autodel)
{
	struct batadv_priv *bat_priv = netdev_priv(hard_iface->soft_iface);
	struct batadv_hard_iface *primary_if = NULL;

	if (hard_iface->if_status == BATADV_IF_ACTIVE)
		batadv_hardif_deactivate_interface(hard_iface);

	if (hard_iface->if_status != BATADV_IF_INACTIVE)
		goto out;

	batadv_info(hard_iface->soft_iface, "Removing interface: %s\n",
		    hard_iface->net_dev->name);
	dev_remove_pack(&hard_iface->batman_adv_ptype);

	bat_priv->num_ifaces--;
	batadv_orig_hash_del_if(hard_iface, bat_priv->num_ifaces);

	primary_if = batadv_primary_if_get_selected(bat_priv);
	if (hard_iface == primary_if) {
		struct batadv_hard_iface *new_if;

		new_if = batadv_hardif_get_active(hard_iface->soft_iface);
		batadv_primary_if_select(bat_priv, new_if);

		if (new_if)
			batadv_hardif_free_ref(new_if);
	}

	bat_priv->bat_algo_ops->bat_iface_disable(hard_iface);
	hard_iface->if_status = BATADV_IF_NOT_IN_USE;

	/* delete all references to this hard_iface */
	batadv_purge_orig_ref(bat_priv);
	batadv_purge_outstanding_packets(bat_priv, hard_iface);
	dev_put(hard_iface->soft_iface);

	/* nobody uses this interface anymore */
	if (!bat_priv->num_ifaces) {
		batadv_gw_check_client_stop(bat_priv);

		if (autodel == BATADV_IF_CLEANUP_AUTO)
			batadv_softif_destroy_sysfs(hard_iface->soft_iface);
	}

	netdev_upper_dev_unlink(hard_iface->net_dev, hard_iface->soft_iface);
	hard_iface->soft_iface = NULL;
	batadv_hardif_free_ref(hard_iface);

out:
	if (primary_if)
		batadv_hardif_free_ref(primary_if);
}

/**
 * batadv_hardif_remove_interface_finish - cleans up the remains of a hardif
 * @work: work queue item
 *
 * Free the parts of the hard interface which can not be removed under
 * rtnl lock (to prevent deadlock situations).
 */
static void batadv_hardif_remove_interface_finish(struct work_struct *work)
{
	struct batadv_hard_iface *hard_iface;

	hard_iface = container_of(work, struct batadv_hard_iface,
				  cleanup_work);

	batadv_sysfs_del_hardif(&hard_iface->hardif_obj);
	batadv_hardif_free_ref(hard_iface);
}

static struct batadv_hard_iface *
batadv_hardif_add_interface(struct net_device *net_dev)
{
	struct batadv_hard_iface *hard_iface;
>>>>>>> refs/remotes/origin/master
	int ret;

	ASSERT_RTNL();

<<<<<<< HEAD
	ret = is_valid_iface(net_dev);
=======
	ret = batadv_is_valid_iface(net_dev);
>>>>>>> refs/remotes/origin/master
	if (ret != 1)
		goto out;

	dev_hold(net_dev);

<<<<<<< HEAD
<<<<<<< HEAD
	hard_iface = kmalloc(sizeof(struct hard_iface), GFP_ATOMIC);
	if (!hard_iface) {
		pr_err("Can't add interface (%s): out of memory\n",
		       net_dev->name);
		goto release_dev;
	}
=======
	hard_iface = kmalloc(sizeof(*hard_iface), GFP_ATOMIC);
	if (!hard_iface)
		goto release_dev;
>>>>>>> refs/remotes/origin/cm-10.0

	ret = sysfs_add_hardif(&hard_iface->hardif_obj, net_dev);
=======
	hard_iface = kzalloc(sizeof(*hard_iface), GFP_ATOMIC);
	if (!hard_iface)
		goto release_dev;

	ret = batadv_sysfs_add_hardif(&hard_iface->hardif_obj, net_dev);
>>>>>>> refs/remotes/origin/master
	if (ret)
		goto free_if;

	hard_iface->if_num = -1;
	hard_iface->net_dev = net_dev;
	hard_iface->soft_iface = NULL;
<<<<<<< HEAD
	hard_iface->if_status = IF_NOT_IN_USE;
	INIT_LIST_HEAD(&hard_iface->list);
	/* extra reference for return */
	atomic_set(&hard_iface->refcount, 2);

	check_known_mac_addr(hard_iface->net_dev);
	list_add_tail_rcu(&hard_iface->list, &hardif_list);
=======
	hard_iface->if_status = BATADV_IF_NOT_IN_USE;
	INIT_LIST_HEAD(&hard_iface->list);
	INIT_WORK(&hard_iface->cleanup_work,
		  batadv_hardif_remove_interface_finish);

	hard_iface->num_bcasts = BATADV_NUM_BCASTS_DEFAULT;
	if (batadv_is_wifi_netdev(net_dev))
		hard_iface->num_bcasts = BATADV_NUM_BCASTS_WIRELESS;

	/* extra reference for return */
	atomic_set(&hard_iface->refcount, 2);

	batadv_check_known_mac_addr(hard_iface->net_dev);
	list_add_tail_rcu(&hard_iface->list, &batadv_hardif_list);
>>>>>>> refs/remotes/origin/master

	return hard_iface;

free_if:
	kfree(hard_iface);
release_dev:
	dev_put(net_dev);
out:
	return NULL;
}

<<<<<<< HEAD
static void hardif_remove_interface(struct hard_iface *hard_iface)
=======
static void batadv_hardif_remove_interface(struct batadv_hard_iface *hard_iface)
>>>>>>> refs/remotes/origin/master
{
	ASSERT_RTNL();

	/* first deactivate interface */
<<<<<<< HEAD
	if (hard_iface->if_status != IF_NOT_IN_USE)
		hardif_disable_interface(hard_iface);

	if (hard_iface->if_status != IF_NOT_IN_USE)
		return;

	hard_iface->if_status = IF_TO_BE_REMOVED;
	sysfs_del_hardif(&hard_iface->hardif_obj);
	hardif_free_ref(hard_iface);
}

void hardif_remove_interfaces(void)
{
	struct hard_iface *hard_iface, *hard_iface_tmp;

	rtnl_lock();
	list_for_each_entry_safe(hard_iface, hard_iface_tmp,
				 &hardif_list, list) {
		list_del_rcu(&hard_iface->list);
		hardif_remove_interface(hard_iface);
=======
	if (hard_iface->if_status != BATADV_IF_NOT_IN_USE)
		batadv_hardif_disable_interface(hard_iface,
						BATADV_IF_CLEANUP_AUTO);

	if (hard_iface->if_status != BATADV_IF_NOT_IN_USE)
		return;

	hard_iface->if_status = BATADV_IF_TO_BE_REMOVED;
	queue_work(batadv_event_workqueue, &hard_iface->cleanup_work);
}

void batadv_hardif_remove_interfaces(void)
{
	struct batadv_hard_iface *hard_iface, *hard_iface_tmp;

	rtnl_lock();
	list_for_each_entry_safe(hard_iface, hard_iface_tmp,
				 &batadv_hardif_list, list) {
		list_del_rcu(&hard_iface->list);
		batadv_hardif_remove_interface(hard_iface);
>>>>>>> refs/remotes/origin/master
	}
	rtnl_unlock();
}

<<<<<<< HEAD
static int hard_if_event(struct notifier_block *this,
			 unsigned long event, void *ptr)
{
<<<<<<< HEAD
	struct net_device *net_dev = (struct net_device *)ptr;
=======
	struct net_device *net_dev = ptr;
>>>>>>> refs/remotes/origin/cm-10.0
	struct hard_iface *hard_iface = hardif_get_by_netdev(net_dev);
	struct hard_iface *primary_if = NULL;
	struct bat_priv *bat_priv;

	if (!hard_iface && event == NETDEV_REGISTER)
		hard_iface = hardif_add_interface(net_dev);
=======
static int batadv_hard_if_event(struct notifier_block *this,
				unsigned long event, void *ptr)
{
	struct net_device *net_dev = netdev_notifier_info_to_dev(ptr);
	struct batadv_hard_iface *hard_iface;
	struct batadv_hard_iface *primary_if = NULL;
	struct batadv_priv *bat_priv;

	if (batadv_softif_is_valid(net_dev) && event == NETDEV_REGISTER) {
		batadv_sysfs_add_meshif(net_dev);
		bat_priv = netdev_priv(net_dev);
		batadv_softif_create_vlan(bat_priv, BATADV_NO_FLAGS);
		return NOTIFY_DONE;
	}

	hard_iface = batadv_hardif_get_by_netdev(net_dev);
	if (!hard_iface && event == NETDEV_REGISTER)
		hard_iface = batadv_hardif_add_interface(net_dev);
>>>>>>> refs/remotes/origin/master

	if (!hard_iface)
		goto out;

	switch (event) {
	case NETDEV_UP:
<<<<<<< HEAD
		hardif_activate_interface(hard_iface);
		break;
	case NETDEV_GOING_DOWN:
	case NETDEV_DOWN:
		hardif_deactivate_interface(hard_iface);
=======
		batadv_hardif_activate_interface(hard_iface);
		break;
	case NETDEV_GOING_DOWN:
	case NETDEV_DOWN:
		batadv_hardif_deactivate_interface(hard_iface);
>>>>>>> refs/remotes/origin/master
		break;
	case NETDEV_UNREGISTER:
		list_del_rcu(&hard_iface->list);

<<<<<<< HEAD
		hardif_remove_interface(hard_iface);
		break;
	case NETDEV_CHANGEMTU:
		if (hard_iface->soft_iface)
			update_min_mtu(hard_iface->soft_iface);
		break;
	case NETDEV_CHANGEADDR:
		if (hard_iface->if_status == IF_NOT_IN_USE)
			goto hardif_put;

		check_known_mac_addr(hard_iface->net_dev);
<<<<<<< HEAD
		update_mac_addresses(hard_iface);

		bat_priv = netdev_priv(hard_iface->soft_iface);
=======

		bat_priv = netdev_priv(hard_iface->soft_iface);
		bat_priv->bat_algo_ops->bat_ogm_update_mac(hard_iface);

>>>>>>> refs/remotes/origin/cm-10.0
		primary_if = primary_if_get_selected(bat_priv);
=======
		batadv_hardif_remove_interface(hard_iface);
		break;
	case NETDEV_CHANGEMTU:
		if (hard_iface->soft_iface)
			batadv_update_min_mtu(hard_iface->soft_iface);
		break;
	case NETDEV_CHANGEADDR:
		if (hard_iface->if_status == BATADV_IF_NOT_IN_USE)
			goto hardif_put;

		batadv_check_known_mac_addr(hard_iface->net_dev);

		bat_priv = netdev_priv(hard_iface->soft_iface);
		bat_priv->bat_algo_ops->bat_iface_update_mac(hard_iface);

		primary_if = batadv_primary_if_get_selected(bat_priv);
>>>>>>> refs/remotes/origin/master
		if (!primary_if)
			goto hardif_put;

		if (hard_iface == primary_if)
<<<<<<< HEAD
			primary_if_update_addr(bat_priv);
		break;
	default:
		break;
<<<<<<< HEAD
	};
=======
	}
>>>>>>> refs/remotes/origin/cm-10.0

hardif_put:
	hardif_free_ref(hard_iface);
out:
	if (primary_if)
		hardif_free_ref(primary_if);
	return NOTIFY_DONE;
}

<<<<<<< HEAD
/* receive a packet with the batman ethertype coming on a hard
=======
/* incoming packets with the batman ethertype received on any active hard
>>>>>>> refs/remotes/origin/cm-10.0
 * interface */
static int batman_skb_recv(struct sk_buff *skb, struct net_device *dev,
			   struct packet_type *ptype,
			   struct net_device *orig_dev)
{
	struct bat_priv *bat_priv;
<<<<<<< HEAD
	struct batman_packet *batman_packet;
=======
	struct batman_ogm_packet *batman_ogm_packet;
>>>>>>> refs/remotes/origin/cm-10.0
	struct hard_iface *hard_iface;
	int ret;

	hard_iface = container_of(ptype, struct hard_iface, batman_adv_ptype);
	skb = skb_share_check(skb, GFP_ATOMIC);

	/* skb was released by skb_share_check() */
	if (!skb)
		goto err_out;

	/* packet should hold at least type and version */
	if (unlikely(!pskb_may_pull(skb, 2)))
		goto err_free;

	/* expect a valid ethernet header here. */
<<<<<<< HEAD
	if (unlikely(skb->mac_len != sizeof(struct ethhdr)
				|| !skb_mac_header(skb)))
=======
	if (unlikely(skb->mac_len != sizeof(struct ethhdr) ||
		     !skb_mac_header(skb)))
>>>>>>> refs/remotes/origin/cm-10.0
		goto err_free;

	if (!hard_iface->soft_iface)
		goto err_free;

	bat_priv = netdev_priv(hard_iface->soft_iface);

	if (atomic_read(&bat_priv->mesh_state) != MESH_ACTIVE)
		goto err_free;

	/* discard frames on not active interfaces */
	if (hard_iface->if_status != IF_ACTIVE)
		goto err_free;

<<<<<<< HEAD
	batman_packet = (struct batman_packet *)skb->data;

	if (batman_packet->version != COMPAT_VERSION) {
		bat_dbg(DBG_BATMAN, bat_priv,
			"Drop packet: incompatible batman version (%i)\n",
			batman_packet->version);
=======
	batman_ogm_packet = (struct batman_ogm_packet *)skb->data;

	if (batman_ogm_packet->header.version != COMPAT_VERSION) {
		bat_dbg(DBG_BATMAN, bat_priv,
			"Drop packet: incompatible batman version (%i)\n",
			batman_ogm_packet->header.version);
>>>>>>> refs/remotes/origin/cm-10.0
		goto err_free;
	}

	/* all receive handlers return whether they received or reused
	 * the supplied skb. if not, we have to free the skb. */

<<<<<<< HEAD
	switch (batman_packet->packet_type) {
		/* batman originator packet */
	case BAT_PACKET:
		ret = recv_bat_packet(skb, hard_iface);
=======
	switch (batman_ogm_packet->header.packet_type) {
		/* batman originator packet */
	case BAT_OGM:
		ret = recv_bat_ogm_packet(skb, hard_iface);
>>>>>>> refs/remotes/origin/cm-10.0
		break;

		/* batman icmp packet */
	case BAT_ICMP:
		ret = recv_icmp_packet(skb, hard_iface);
		break;

		/* unicast packet */
	case BAT_UNICAST:
		ret = recv_unicast_packet(skb, hard_iface);
		break;

		/* fragmented unicast packet */
	case BAT_UNICAST_FRAG:
		ret = recv_ucast_frag_packet(skb, hard_iface);
		break;

		/* broadcast packet */
	case BAT_BCAST:
		ret = recv_bcast_packet(skb, hard_iface);
		break;

		/* vis packet */
	case BAT_VIS:
		ret = recv_vis_packet(skb, hard_iface);
		break;
<<<<<<< HEAD
=======
		/* Translation table query (request or response) */
	case BAT_TT_QUERY:
		ret = recv_tt_query(skb, hard_iface);
		break;
		/* Roaming advertisement */
	case BAT_ROAM_ADV:
		ret = recv_roam_adv(skb, hard_iface);
		break;
>>>>>>> refs/remotes/origin/cm-10.0
	default:
		ret = NET_RX_DROP;
	}

	if (ret == NET_RX_DROP)
		kfree_skb(skb);

	/* return NET_RX_SUCCESS in any case as we
	 * most probably dropped the packet for
	 * routing-logical reasons. */

	return NET_RX_SUCCESS;

err_free:
	kfree_skb(skb);
err_out:
	return NET_RX_DROP;
}

<<<<<<< HEAD
=======
/* This function returns true if the interface represented by ifindex is a
 * 802.11 wireless device */
bool is_wifi_iface(int ifindex)
{
	struct net_device *net_device = NULL;
	bool ret = false;

	if (ifindex == NULL_IFINDEX)
		goto out;

	net_device = dev_get_by_index(&init_net, ifindex);
	if (!net_device)
		goto out;

#ifdef CONFIG_WIRELESS_EXT
	/* pre-cfg80211 drivers have to implement WEXT, so it is possible to
	 * check for wireless_handlers != NULL */
	if (net_device->wireless_handlers)
		ret = true;
	else
#endif
		/* cfg80211 drivers have to set ieee80211_ptr */
		if (net_device->ieee80211_ptr)
			ret = true;
out:
	if (net_device)
		dev_put(net_device);
	return ret;
}

>>>>>>> refs/remotes/origin/cm-10.0
struct notifier_block hard_if_notifier = {
	.notifier_call = hard_if_event,
=======
			batadv_primary_if_update_addr(bat_priv, NULL);
		break;
	default:
		break;
	}

hardif_put:
	batadv_hardif_free_ref(hard_iface);
out:
	if (primary_if)
		batadv_hardif_free_ref(primary_if);
	return NOTIFY_DONE;
}

struct notifier_block batadv_hard_if_notifier = {
	.notifier_call = batadv_hard_if_event,
>>>>>>> refs/remotes/origin/master
};
