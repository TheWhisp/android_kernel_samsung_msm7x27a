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
 * Marek Lindner
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

#ifndef _NET_BATMAN_ADV_SOFT_INTERFACE_H_
#define _NET_BATMAN_ADV_SOFT_INTERFACE_H_

<<<<<<< HEAD
int my_skb_head_push(struct sk_buff *skb, unsigned int len);
int softif_neigh_seq_print_text(struct seq_file *seq, void *offset);
void softif_neigh_purge(struct bat_priv *bat_priv);
<<<<<<< HEAD
int interface_tx(struct sk_buff *skb, struct net_device *soft_iface);
void interface_rx(struct net_device *soft_iface,
		  struct sk_buff *skb, struct hard_iface *recv_if,
		  int hdr_size);
struct net_device *softif_create(char *name);
void softif_destroy(struct net_device *soft_iface);
int softif_is_valid(struct net_device *net_dev);
=======
void interface_rx(struct net_device *soft_iface,
		  struct sk_buff *skb, struct hard_iface *recv_if,
		  int hdr_size);
struct net_device *softif_create(const char *name);
void softif_destroy(struct net_device *soft_iface);
int softif_is_valid(const struct net_device *net_dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
int batadv_skb_head_push(struct sk_buff *skb, unsigned int len);
void batadv_interface_rx(struct net_device *soft_iface,
			 struct sk_buff *skb, struct batadv_hard_iface *recv_if,
			 int hdr_size, struct batadv_orig_node *orig_node);
struct net_device *batadv_softif_create(const char *name);
void batadv_softif_destroy_sysfs(struct net_device *soft_iface);
int batadv_softif_is_valid(const struct net_device *net_dev);
extern struct rtnl_link_ops batadv_link_ops;
int batadv_softif_create_vlan(struct batadv_priv *bat_priv, unsigned short vid);
void batadv_softif_vlan_free_ref(struct batadv_softif_vlan *softif_vlan);
struct batadv_softif_vlan *batadv_softif_vlan_get(struct batadv_priv *bat_priv,
						  unsigned short vid);
>>>>>>> refs/remotes/origin/master

#endif /* _NET_BATMAN_ADV_SOFT_INTERFACE_H_ */
