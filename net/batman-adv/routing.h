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

#ifndef _NET_BATMAN_ADV_ROUTING_H_
#define _NET_BATMAN_ADV_ROUTING_H_

<<<<<<< HEAD
void slide_own_bcast_window(struct hard_iface *hard_iface);
<<<<<<< HEAD
void receive_bat_packet(struct ethhdr *ethhdr,
				struct batman_packet *batman_packet,
				unsigned char *tt_buff, int tt_buff_len,
				struct hard_iface *if_incoming);
void update_routes(struct bat_priv *bat_priv, struct orig_node *orig_node,
		   struct neigh_node *neigh_node, unsigned char *tt_buff,
		   int tt_buff_len);
=======
void update_route(struct bat_priv *bat_priv, struct orig_node *orig_node,
		  struct neigh_node *neigh_node);
>>>>>>> refs/remotes/origin/cm-10.0
int route_unicast_packet(struct sk_buff *skb, struct hard_iface *recv_if);
int recv_icmp_packet(struct sk_buff *skb, struct hard_iface *recv_if);
int recv_unicast_packet(struct sk_buff *skb, struct hard_iface *recv_if);
int recv_ucast_frag_packet(struct sk_buff *skb, struct hard_iface *recv_if);
int recv_bcast_packet(struct sk_buff *skb, struct hard_iface *recv_if);
int recv_vis_packet(struct sk_buff *skb, struct hard_iface *recv_if);
<<<<<<< HEAD
int recv_bat_packet(struct sk_buff *skb, struct hard_iface *recv_if);
struct neigh_node *find_router(struct bat_priv *bat_priv,
			       struct orig_node *orig_node,
			       struct hard_iface *recv_if);
void bonding_candidate_del(struct orig_node *orig_node,
			   struct neigh_node *neigh_node);
=======
int recv_bat_ogm_packet(struct sk_buff *skb, struct hard_iface *recv_if);
int recv_tt_query(struct sk_buff *skb, struct hard_iface *recv_if);
int recv_roam_adv(struct sk_buff *skb, struct hard_iface *recv_if);
struct neigh_node *find_router(struct bat_priv *bat_priv,
			       struct orig_node *orig_node,
			       const struct hard_iface *recv_if);
void bonding_candidate_del(struct orig_node *orig_node,
			   struct neigh_node *neigh_node);
void bonding_candidate_add(struct orig_node *orig_node,
			   struct neigh_node *neigh_node);
void bonding_save_primary(const struct orig_node *orig_node,
			  struct orig_node *orig_neigh_node,
			  const struct batman_ogm_packet *batman_ogm_packet);
int window_protected(struct bat_priv *bat_priv, int32_t seq_num_diff,
		     unsigned long *last_reset);
>>>>>>> refs/remotes/origin/cm-10.0
=======
bool batadv_check_management_packet(struct sk_buff *skb,
				    struct batadv_hard_iface *hard_iface,
				    int header_len);
void batadv_update_route(struct batadv_priv *bat_priv,
			 struct batadv_orig_node *orig_node,
			 struct batadv_neigh_node *neigh_node);
int batadv_recv_icmp_packet(struct sk_buff *skb,
			    struct batadv_hard_iface *recv_if);
int batadv_recv_unicast_packet(struct sk_buff *skb,
			       struct batadv_hard_iface *recv_if);
int batadv_recv_frag_packet(struct sk_buff *skb,
			    struct batadv_hard_iface *iface);
int batadv_recv_bcast_packet(struct sk_buff *skb,
			     struct batadv_hard_iface *recv_if);
int batadv_recv_tt_query(struct sk_buff *skb,
			 struct batadv_hard_iface *recv_if);
int batadv_recv_roam_adv(struct sk_buff *skb,
			 struct batadv_hard_iface *recv_if);
int batadv_recv_unicast_tvlv(struct sk_buff *skb,
			     struct batadv_hard_iface *recv_if);
int batadv_recv_unhandled_unicast_packet(struct sk_buff *skb,
					 struct batadv_hard_iface *recv_if);
struct batadv_neigh_node *
batadv_find_router(struct batadv_priv *bat_priv,
		   struct batadv_orig_node *orig_node,
		   const struct batadv_hard_iface *recv_if);
void batadv_bonding_candidate_del(struct batadv_orig_node *orig_node,
				  struct batadv_neigh_node *neigh_node);
void batadv_bonding_candidate_add(struct batadv_priv *bat_priv,
				  struct batadv_orig_node *orig_node,
				  struct batadv_neigh_node *neigh_node);
void batadv_bonding_save_primary(const struct batadv_orig_node *orig_node,
				 struct batadv_orig_node *orig_neigh_node,
				 const struct batadv_ogm_packet
				 *batman_ogm_packet);
int batadv_window_protected(struct batadv_priv *bat_priv, int32_t seq_num_diff,
			    unsigned long *last_reset);
>>>>>>> refs/remotes/origin/master

#endif /* _NET_BATMAN_ADV_ROUTING_H_ */
