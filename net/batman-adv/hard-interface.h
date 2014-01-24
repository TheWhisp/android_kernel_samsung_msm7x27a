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

#ifndef _NET_BATMAN_ADV_HARD_INTERFACE_H_
#define _NET_BATMAN_ADV_HARD_INTERFACE_H_

<<<<<<< HEAD
<<<<<<< HEAD
#define IF_NOT_IN_USE 0
#define IF_TO_BE_REMOVED 1
#define IF_INACTIVE 2
#define IF_ACTIVE 3
#define IF_TO_BE_ACTIVATED 4
#define IF_I_WANT_YOU 5

extern struct notifier_block hard_if_notifier;

struct hard_iface *hardif_get_by_netdev(struct net_device *net_dev);
int hardif_enable_interface(struct hard_iface *hard_iface, char *iface_name);
=======
enum hard_if_state {
	IF_NOT_IN_USE,
	IF_TO_BE_REMOVED,
	IF_INACTIVE,
	IF_ACTIVE,
	IF_TO_BE_ACTIVATED,
	IF_I_WANT_YOU
};

extern struct notifier_block hard_if_notifier;

struct hard_iface*
hardif_get_by_netdev(const struct net_device *net_dev);
int hardif_enable_interface(struct hard_iface *hard_iface,
			    const char *iface_name);
>>>>>>> refs/remotes/origin/cm-10.0
void hardif_disable_interface(struct hard_iface *hard_iface);
void hardif_remove_interfaces(void);
int hardif_min_mtu(struct net_device *soft_iface);
void update_min_mtu(struct net_device *soft_iface);
void hardif_free_rcu(struct rcu_head *rcu);
<<<<<<< HEAD
=======
bool is_wifi_iface(int ifindex);
>>>>>>> refs/remotes/origin/cm-10.0

static inline void hardif_free_ref(struct hard_iface *hard_iface)
{
	if (atomic_dec_and_test(&hard_iface->refcount))
		call_rcu(&hard_iface->rcu, hardif_free_rcu);
}

static inline struct hard_iface *primary_if_get_selected(
						struct bat_priv *bat_priv)
{
	struct hard_iface *hard_iface;
=======
enum batadv_hard_if_state {
	BATADV_IF_NOT_IN_USE,
	BATADV_IF_TO_BE_REMOVED,
	BATADV_IF_INACTIVE,
	BATADV_IF_ACTIVE,
	BATADV_IF_TO_BE_ACTIVATED,
	BATADV_IF_I_WANT_YOU,
};

/**
 * enum batadv_hard_if_cleanup - Cleanup modi for soft_iface after slave removal
 * @BATADV_IF_CLEANUP_KEEP: Don't automatically delete soft-interface
 * @BATADV_IF_CLEANUP_AUTO: Delete soft-interface after last slave was removed
 */
enum batadv_hard_if_cleanup {
	BATADV_IF_CLEANUP_KEEP,
	BATADV_IF_CLEANUP_AUTO,
};

extern struct notifier_block batadv_hard_if_notifier;

bool batadv_is_wifi_netdev(struct net_device *net_device);
struct batadv_hard_iface*
batadv_hardif_get_by_netdev(const struct net_device *net_dev);
int batadv_hardif_enable_interface(struct batadv_hard_iface *hard_iface,
				   const char *iface_name);
void batadv_hardif_disable_interface(struct batadv_hard_iface *hard_iface,
				     enum batadv_hard_if_cleanup autodel);
void batadv_hardif_remove_interfaces(void);
int batadv_hardif_min_mtu(struct net_device *soft_iface);
void batadv_update_min_mtu(struct net_device *soft_iface);
void batadv_hardif_free_rcu(struct rcu_head *rcu);

static inline void
batadv_hardif_free_ref(struct batadv_hard_iface *hard_iface)
{
	if (atomic_dec_and_test(&hard_iface->refcount))
		call_rcu(&hard_iface->rcu, batadv_hardif_free_rcu);
}

static inline struct batadv_hard_iface *
batadv_primary_if_get_selected(struct batadv_priv *bat_priv)
{
	struct batadv_hard_iface *hard_iface;
>>>>>>> refs/remotes/origin/master

	rcu_read_lock();
	hard_iface = rcu_dereference(bat_priv->primary_if);
	if (!hard_iface)
		goto out;

	if (!atomic_inc_not_zero(&hard_iface->refcount))
		hard_iface = NULL;

out:
	rcu_read_unlock();
	return hard_iface;
}

#endif /* _NET_BATMAN_ADV_HARD_INTERFACE_H_ */
