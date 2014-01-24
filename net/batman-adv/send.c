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
#include "send.h"
#include "routing.h"
#include "translation-table.h"
#include "soft-interface.h"
#include "hard-interface.h"
<<<<<<< HEAD
#include "vis.h"
<<<<<<< HEAD
#include "aggregation.h"
=======
>>>>>>> refs/remotes/origin/cm-10.0
#include "gateway_common.h"
#include "originator.h"

static void send_outstanding_bcast_packet(struct work_struct *work);

<<<<<<< HEAD
/* apply hop penalty for a normal link */
static uint8_t hop_penalty(const uint8_t tq, struct bat_priv *bat_priv)
{
	int hop_penalty = atomic_read(&bat_priv->hop_penalty);
	return (tq * (TQ_MAX_VALUE - hop_penalty)) / (TQ_MAX_VALUE);
}

/* when do we schedule our own packet to be sent */
static unsigned long own_send_time(struct bat_priv *bat_priv)
{
	return jiffies + msecs_to_jiffies(
		   atomic_read(&bat_priv->orig_interval) -
		   JITTER + (random32() % 2*JITTER));
}

/* when do we schedule a forwarded packet to be sent */
static unsigned long forward_send_time(void)
{
	return jiffies + msecs_to_jiffies(random32() % (JITTER/2));
}

/* send out an already prepared packet to the given address via the
 * specified batman interface */
int send_skb_packet(struct sk_buff *skb,
				struct hard_iface *hard_iface,
				uint8_t *dst_addr)
=======
/* send out an already prepared packet to the given address via the
 * specified batman interface */
int send_skb_packet(struct sk_buff *skb, struct hard_iface *hard_iface,
		    const uint8_t *dst_addr)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct ethhdr *ethhdr;

	if (hard_iface->if_status != IF_ACTIVE)
=======
#include "gateway_common.h"
#include "gateway_client.h"
#include "originator.h"
#include "network-coding.h"
#include "fragmentation.h"

static void batadv_send_outstanding_bcast_packet(struct work_struct *work);

/* send out an already prepared packet to the given address via the
 * specified batman interface
 */
int batadv_send_skb_packet(struct sk_buff *skb,
			   struct batadv_hard_iface *hard_iface,
			   const uint8_t *dst_addr)
{
	struct batadv_priv *bat_priv = netdev_priv(hard_iface->soft_iface);
	struct ethhdr *ethhdr;

	if (hard_iface->if_status != BATADV_IF_ACTIVE)
>>>>>>> refs/remotes/origin/master
		goto send_skb_err;

	if (unlikely(!hard_iface->net_dev))
		goto send_skb_err;

	if (!(hard_iface->net_dev->flags & IFF_UP)) {
<<<<<<< HEAD
<<<<<<< HEAD
		pr_warning("Interface %s is not up - can't send packet via "
			   "that interface!\n", hard_iface->net_dev->name);
=======
		pr_warning("Interface %s is not up - can't send packet via that interface!\n",
			   hard_iface->net_dev->name);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		pr_warn("Interface %s is not up - can't send packet via that interface!\n",
			hard_iface->net_dev->name);
>>>>>>> refs/remotes/origin/master
		goto send_skb_err;
	}

	/* push to the ethernet header. */
<<<<<<< HEAD
<<<<<<< HEAD
	if (my_skb_head_push(skb, sizeof(struct ethhdr)) < 0)
=======
	if (my_skb_head_push(skb, sizeof(*ethhdr)) < 0)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (batadv_skb_head_push(skb, ETH_HLEN) < 0)
>>>>>>> refs/remotes/origin/master
		goto send_skb_err;

	skb_reset_mac_header(skb);

<<<<<<< HEAD
<<<<<<< HEAD
	ethhdr = (struct ethhdr *) skb_mac_header(skb);
=======
	ethhdr = (struct ethhdr *)skb_mac_header(skb);
>>>>>>> refs/remotes/origin/cm-10.0
	memcpy(ethhdr->h_source, hard_iface->net_dev->dev_addr, ETH_ALEN);
	memcpy(ethhdr->h_dest, dst_addr, ETH_ALEN);
	ethhdr->h_proto = __constant_htons(ETH_P_BATMAN);

	skb_set_network_header(skb, ETH_HLEN);
	skb->priority = TC_PRIO_CONTROL;
	skb->protocol = __constant_htons(ETH_P_BATMAN);

	skb->dev = hard_iface->net_dev;

	/* dev_queue_xmit() returns a negative result on error.	 However on
	 * congestion and traffic shaping, it drops and returns NET_XMIT_DROP
	 * (which is > 0). This will not be treated as an error. */

=======
	ethhdr = eth_hdr(skb);
	memcpy(ethhdr->h_source, hard_iface->net_dev->dev_addr, ETH_ALEN);
	memcpy(ethhdr->h_dest, dst_addr, ETH_ALEN);
	ethhdr->h_proto = htons(ETH_P_BATMAN);

	skb_set_network_header(skb, ETH_HLEN);
	skb->protocol = htons(ETH_P_BATMAN);

	skb->dev = hard_iface->net_dev;

	/* Save a clone of the skb to use when decoding coded packets */
	batadv_nc_skb_store_for_decoding(bat_priv, skb);

	/* dev_queue_xmit() returns a negative result on error.	 However on
	 * congestion and traffic shaping, it drops and returns NET_XMIT_DROP
	 * (which is > 0). This will not be treated as an error.
	 */
>>>>>>> refs/remotes/origin/master
	return dev_queue_xmit(skb);
send_skb_err:
	kfree_skb(skb);
	return NET_XMIT_DROP;
}

<<<<<<< HEAD
<<<<<<< HEAD
/* Send a packet to a given interface */
static void send_packet_to_if(struct forw_packet *forw_packet,
			      struct hard_iface *hard_iface)
{
	struct bat_priv *bat_priv = netdev_priv(hard_iface->soft_iface);
	char *fwd_str;
	uint8_t packet_num;
	int16_t buff_pos;
	struct batman_packet *batman_packet;
	struct sk_buff *skb;

	if (hard_iface->if_status != IF_ACTIVE)
		return;

	packet_num = 0;
	buff_pos = 0;
	batman_packet = (struct batman_packet *)forw_packet->skb->data;

	/* adjust all flags and log packets */
	while (aggregated_packet(buff_pos,
				 forw_packet->packet_len,
				 batman_packet->num_tt)) {

		/* we might have aggregated direct link packets with an
		 * ordinary base packet */
		if ((forw_packet->direct_link_flags & (1 << packet_num)) &&
		    (forw_packet->if_incoming == hard_iface))
			batman_packet->flags |= DIRECTLINK;
		else
			batman_packet->flags &= ~DIRECTLINK;

		fwd_str = (packet_num > 0 ? "Forwarding" : (forw_packet->own ?
							    "Sending own" :
							    "Forwarding"));
		bat_dbg(DBG_BATMAN, bat_priv,
			"%s %spacket (originator %pM, seqno %d, TQ %d, TTL %d,"
			" IDF %s) on interface %s [%pM]\n",
			fwd_str, (packet_num > 0 ? "aggregated " : ""),
			batman_packet->orig, ntohl(batman_packet->seqno),
			batman_packet->tq, batman_packet->ttl,
			(batman_packet->flags & DIRECTLINK ?
			 "on" : "off"),
			hard_iface->net_dev->name,
			hard_iface->net_dev->dev_addr);

		buff_pos += sizeof(struct batman_packet) +
			(batman_packet->num_tt * ETH_ALEN);
		packet_num++;
		batman_packet = (struct batman_packet *)
			(forw_packet->skb->data + buff_pos);
	}

	/* create clone because function is called more than once */
	skb = skb_clone(forw_packet->skb, GFP_ATOMIC);
	if (skb)
		send_skb_packet(skb, hard_iface, broadcast_addr);
}

/* send a batman packet */
static void send_packet(struct forw_packet *forw_packet)
{
	struct hard_iface *hard_iface;
	struct net_device *soft_iface;
	struct bat_priv *bat_priv;
	struct batman_packet *batman_packet =
		(struct batman_packet *)(forw_packet->skb->data);
	unsigned char directlink = (batman_packet->flags & DIRECTLINK ? 1 : 0);

	if (!forw_packet->if_incoming) {
		pr_err("Error - can't forward packet: incoming iface not "
		       "specified\n");
		return;
	}

	soft_iface = forw_packet->if_incoming->soft_iface;
	bat_priv = netdev_priv(soft_iface);

	if (forw_packet->if_incoming->if_status != IF_ACTIVE)
		return;

	/* multihomed peer assumed */
	/* non-primary OGMs are only broadcasted on their interface */
	if ((directlink && (batman_packet->ttl == 1)) ||
	    (forw_packet->own && (forw_packet->if_incoming->if_num > 0))) {

		/* FIXME: what about aggregated packets ? */
		bat_dbg(DBG_BATMAN, bat_priv,
			"%s packet (originator %pM, seqno %d, TTL %d) "
			"on interface %s [%pM]\n",
			(forw_packet->own ? "Sending own" : "Forwarding"),
			batman_packet->orig, ntohl(batman_packet->seqno),
			batman_packet->ttl,
			forw_packet->if_incoming->net_dev->name,
			forw_packet->if_incoming->net_dev->dev_addr);

		/* skb is only used once and than forw_packet is free'd */
		send_skb_packet(forw_packet->skb, forw_packet->if_incoming,
				broadcast_addr);
		forw_packet->skb = NULL;

		return;
	}

	/* broadcast on every interface */
	rcu_read_lock();
	list_for_each_entry_rcu(hard_iface, &hardif_list, list) {
		if (hard_iface->soft_iface != soft_iface)
			continue;

		send_packet_to_if(forw_packet, hard_iface);
	}
	rcu_read_unlock();
}

static void rebuild_batman_packet(struct bat_priv *bat_priv,
				  struct hard_iface *hard_iface)
{
	int new_len;
	unsigned char *new_buff;
	struct batman_packet *batman_packet;

	new_len = sizeof(struct batman_packet) +
			(bat_priv->num_local_tt * ETH_ALEN);
	new_buff = kmalloc(new_len, GFP_ATOMIC);

	/* keep old buffer if kmalloc should fail */
	if (new_buff) {
		memcpy(new_buff, hard_iface->packet_buff,
		       sizeof(struct batman_packet));
		batman_packet = (struct batman_packet *)new_buff;

		batman_packet->num_tt = tt_local_fill_buffer(bat_priv,
				new_buff + sizeof(struct batman_packet),
				new_len - sizeof(struct batman_packet));

		kfree(hard_iface->packet_buff);
		hard_iface->packet_buff = new_buff;
		hard_iface->packet_len = new_len;
	}
}

void schedule_own_packet(struct hard_iface *hard_iface)
{
	struct bat_priv *bat_priv = netdev_priv(hard_iface->soft_iface);
	struct hard_iface *primary_if;
	unsigned long send_time;
	struct batman_packet *batman_packet;
	int vis_server;
=======
static void realloc_packet_buffer(struct hard_iface *hard_iface,
				  int new_len)
{
	unsigned char *new_buff;

	new_buff = kmalloc(new_len, GFP_ATOMIC);

	/* keep old buffer if kmalloc should fail */
	if (new_buff) {
		memcpy(new_buff, hard_iface->packet_buff,
		       BATMAN_OGM_LEN);

		kfree(hard_iface->packet_buff);
		hard_iface->packet_buff = new_buff;
		hard_iface->packet_len = new_len;
	}
}

/* when calling this function (hard_iface == primary_if) has to be true */
static int prepare_packet_buffer(struct bat_priv *bat_priv,
				  struct hard_iface *hard_iface)
{
	int new_len;

	new_len = BATMAN_OGM_LEN +
		  tt_len((uint8_t)atomic_read(&bat_priv->tt_local_changes));

	/* if we have too many changes for one packet don't send any
	 * and wait for the tt table request which will be fragmented */
	if (new_len > hard_iface->soft_iface->mtu)
		new_len = BATMAN_OGM_LEN;

	realloc_packet_buffer(hard_iface, new_len);

	atomic_set(&bat_priv->tt_crc, tt_local_crc(bat_priv));

	/* reset the sending counter */
	atomic_set(&bat_priv->tt_ogm_append_cnt, TT_OGM_APPEND_MAX);

	return tt_changes_fill_buffer(bat_priv,
				      hard_iface->packet_buff + BATMAN_OGM_LEN,
				      hard_iface->packet_len - BATMAN_OGM_LEN);
}

static int reset_packet_buffer(struct bat_priv *bat_priv,
				struct hard_iface *hard_iface)
{
	realloc_packet_buffer(hard_iface, BATMAN_OGM_LEN);
	return 0;
}

void schedule_bat_ogm(struct hard_iface *hard_iface)
{
	struct bat_priv *bat_priv = netdev_priv(hard_iface->soft_iface);
	struct hard_iface *primary_if;
	int tt_num_changes = -1;
>>>>>>> refs/remotes/origin/cm-10.0

	if ((hard_iface->if_status == IF_NOT_IN_USE) ||
	    (hard_iface->if_status == IF_TO_BE_REMOVED))
		return;

<<<<<<< HEAD
	vis_server = atomic_read(&bat_priv->vis_mode);
	primary_if = primary_if_get_selected(bat_priv);

=======
>>>>>>> refs/remotes/origin/cm-10.0
	/**
	 * the interface gets activated here to avoid race conditions between
	 * the moment of activating the interface in
	 * hardif_activate_interface() where the originator mac is set and
	 * outdated packets (especially uninitialized mac addresses) in the
	 * packet queue
	 */
	if (hard_iface->if_status == IF_TO_BE_ACTIVATED)
		hard_iface->if_status = IF_ACTIVE;

<<<<<<< HEAD
	/* if local tt has changed and interface is a primary interface */
	if ((atomic_read(&bat_priv->tt_local_changed)) &&
	    (hard_iface == primary_if))
		rebuild_batman_packet(bat_priv, hard_iface);

	/**
	 * NOTE: packet_buff might just have been re-allocated in
	 * rebuild_batman_packet()
	 */
	batman_packet = (struct batman_packet *)hard_iface->packet_buff;

	/* change sequence number to network order */
	batman_packet->seqno =
		htonl((uint32_t)atomic_read(&hard_iface->seqno));

	if (vis_server == VIS_TYPE_SERVER_SYNC)
		batman_packet->flags |= VIS_SERVER;
	else
		batman_packet->flags &= ~VIS_SERVER;

	if ((hard_iface == primary_if) &&
	    (atomic_read(&bat_priv->gw_mode) == GW_MODE_SERVER))
		batman_packet->gw_flags =
				(uint8_t)atomic_read(&bat_priv->gw_bandwidth);
	else
		batman_packet->gw_flags = 0;

	atomic_inc(&hard_iface->seqno);

	slide_own_bcast_window(hard_iface);
	send_time = own_send_time(bat_priv);
	add_bat_packet_to_list(bat_priv,
			       hard_iface->packet_buff,
			       hard_iface->packet_len,
			       hard_iface, 1, send_time);

	if (primary_if)
		hardif_free_ref(primary_if);
}

void schedule_forward_packet(struct orig_node *orig_node,
			     struct ethhdr *ethhdr,
			     struct batman_packet *batman_packet,
			     uint8_t directlink, int tt_buff_len,
			     struct hard_iface *if_incoming)
{
	struct bat_priv *bat_priv = netdev_priv(if_incoming->soft_iface);
	struct neigh_node *router;
	unsigned char in_tq, in_ttl, tq_avg = 0;
	unsigned long send_time;

	if (batman_packet->ttl <= 1) {
		bat_dbg(DBG_BATMAN, bat_priv, "ttl exceeded\n");
		return;
	}

	router = orig_node_get_router(orig_node);

	in_tq = batman_packet->tq;
	in_ttl = batman_packet->ttl;

	batman_packet->ttl--;
	memcpy(batman_packet->prev_sender, ethhdr->h_source, ETH_ALEN);

	/* rebroadcast tq of our best ranking neighbor to ensure the rebroadcast
	 * of our best tq value */
	if (router && router->tq_avg != 0) {

		/* rebroadcast ogm of best ranking neighbor as is */
		if (!compare_eth(router->addr, ethhdr->h_source)) {
			batman_packet->tq = router->tq_avg;

			if (router->last_ttl)
				batman_packet->ttl = router->last_ttl - 1;
		}

		tq_avg = router->tq_avg;
	}

	if (router)
		neigh_node_free_ref(router);

	/* apply hop penalty */
	batman_packet->tq = hop_penalty(batman_packet->tq, bat_priv);

	bat_dbg(DBG_BATMAN, bat_priv,
		"Forwarding packet: tq_orig: %i, tq_avg: %i, "
		"tq_forw: %i, ttl_orig: %i, ttl_forw: %i\n",
		in_tq, tq_avg, batman_packet->tq, in_ttl - 1,
		batman_packet->ttl);

	batman_packet->seqno = htonl(batman_packet->seqno);

	/* switch of primaries first hop flag when forwarding */
	batman_packet->flags &= ~PRIMARIES_FIRST_HOP;
	if (directlink)
		batman_packet->flags |= DIRECTLINK;
	else
		batman_packet->flags &= ~DIRECTLINK;

	send_time = forward_send_time();
	add_bat_packet_to_list(bat_priv,
			       (unsigned char *)batman_packet,
			       sizeof(struct batman_packet) + tt_buff_len,
			       if_incoming, 0, send_time);
=======
	primary_if = primary_if_get_selected(bat_priv);

	if (hard_iface == primary_if) {
		/* if at least one change happened */
		if (atomic_read(&bat_priv->tt_local_changes) > 0) {
			tt_commit_changes(bat_priv);
			tt_num_changes = prepare_packet_buffer(bat_priv,
							       hard_iface);
		}

		/* if the changes have been sent often enough */
		if (!atomic_dec_not_zero(&bat_priv->tt_ogm_append_cnt))
			tt_num_changes = reset_packet_buffer(bat_priv,
							     hard_iface);
	}

	if (primary_if)
		hardif_free_ref(primary_if);

	bat_priv->bat_algo_ops->bat_ogm_schedule(hard_iface, tt_num_changes);
>>>>>>> refs/remotes/origin/cm-10.0
}

static void forw_packet_free(struct forw_packet *forw_packet)
=======
/**
 * batadv_send_skb_to_orig - Lookup next-hop and transmit skb.
 * @skb: Packet to be transmitted.
 * @orig_node: Final destination of the packet.
 * @recv_if: Interface used when receiving the packet (can be NULL).
 *
 * Looks up the best next-hop towards the passed originator and passes the
 * skb on for preparation of MAC header. If the packet originated from this
 * host, NULL can be passed as recv_if and no interface alternating is
 * attempted.
 *
 * Returns NET_XMIT_SUCCESS on success, NET_XMIT_DROP on failure, or
 * NET_XMIT_POLICED if the skb is buffered for later transmit.
 */
int batadv_send_skb_to_orig(struct sk_buff *skb,
			    struct batadv_orig_node *orig_node,
			    struct batadv_hard_iface *recv_if)
{
	struct batadv_priv *bat_priv = orig_node->bat_priv;
	struct batadv_neigh_node *neigh_node;
	int ret = NET_XMIT_DROP;

	/* batadv_find_router() increases neigh_nodes refcount if found. */
	neigh_node = batadv_find_router(bat_priv, orig_node, recv_if);
	if (!neigh_node)
		goto out;

	/* Check if the skb is too large to send in one piece and fragment
	 * it if needed.
	 */
	if (atomic_read(&bat_priv->fragmentation) &&
	    skb->len > neigh_node->if_incoming->net_dev->mtu) {
		/* Fragment and send packet. */
		if (batadv_frag_send_packet(skb, orig_node, neigh_node))
			ret = NET_XMIT_SUCCESS;

		goto out;
	}

	/* try to network code the packet, if it is received on an interface
	 * (i.e. being forwarded). If the packet originates from this node or if
	 * network coding fails, then send the packet as usual.
	 */
	if (recv_if && batadv_nc_skb_forward(skb, neigh_node)) {
		ret = NET_XMIT_POLICED;
	} else {
		batadv_send_skb_packet(skb, neigh_node->if_incoming,
				       neigh_node->addr);
		ret = NET_XMIT_SUCCESS;
	}

out:
	if (neigh_node)
		batadv_neigh_node_free_ref(neigh_node);

	return ret;
}

/**
 * batadv_send_skb_push_fill_unicast - extend the buffer and initialize the
 *  common fields for unicast packets
 * @skb: the skb carrying the unicast header to initialize
 * @hdr_size: amount of bytes to push at the beginning of the skb
 * @orig_node: the destination node
 *
 * Returns false if the buffer extension was not possible or true otherwise.
 */
static bool
batadv_send_skb_push_fill_unicast(struct sk_buff *skb, int hdr_size,
				  struct batadv_orig_node *orig_node)
{
	struct batadv_unicast_packet *unicast_packet;
	uint8_t ttvn = (uint8_t)atomic_read(&orig_node->last_ttvn);

	if (batadv_skb_head_push(skb, hdr_size) < 0)
		return false;

	unicast_packet = (struct batadv_unicast_packet *)skb->data;
	unicast_packet->version = BATADV_COMPAT_VERSION;
	/* batman packet type: unicast */
	unicast_packet->packet_type = BATADV_UNICAST;
	/* set unicast ttl */
	unicast_packet->ttl = BATADV_TTL;
	/* copy the destination for faster routing */
	memcpy(unicast_packet->dest, orig_node->orig, ETH_ALEN);
	/* set the destination tt version number */
	unicast_packet->ttvn = ttvn;

	return true;
}

/**
 * batadv_send_skb_prepare_unicast - encapsulate an skb with a unicast header
 * @skb: the skb containing the payload to encapsulate
 * @orig_node: the destination node
 *
 * Returns false if the payload could not be encapsulated or true otherwise.
 */
static bool batadv_send_skb_prepare_unicast(struct sk_buff *skb,
					    struct batadv_orig_node *orig_node)
{
	size_t uni_size = sizeof(struct batadv_unicast_packet);

	return batadv_send_skb_push_fill_unicast(skb, uni_size, orig_node);
}

/**
 * batadv_send_skb_prepare_unicast_4addr - encapsulate an skb with a
 *  unicast 4addr header
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: the skb containing the payload to encapsulate
 * @orig_node: the destination node
 * @packet_subtype: the unicast 4addr packet subtype to use
 *
 * Returns false if the payload could not be encapsulated or true otherwise.
 */
bool batadv_send_skb_prepare_unicast_4addr(struct batadv_priv *bat_priv,
					   struct sk_buff *skb,
					   struct batadv_orig_node *orig,
					   int packet_subtype)
{
	struct batadv_hard_iface *primary_if;
	struct batadv_unicast_4addr_packet *uc_4addr_packet;
	bool ret = false;

	primary_if = batadv_primary_if_get_selected(bat_priv);
	if (!primary_if)
		goto out;

	/* Pull the header space and fill the unicast_packet substructure.
	 * We can do that because the first member of the uc_4addr_packet
	 * is of type struct unicast_packet
	 */
	if (!batadv_send_skb_push_fill_unicast(skb, sizeof(*uc_4addr_packet),
					       orig))
		goto out;

	uc_4addr_packet = (struct batadv_unicast_4addr_packet *)skb->data;
	uc_4addr_packet->u.packet_type = BATADV_UNICAST_4ADDR;
	memcpy(uc_4addr_packet->src, primary_if->net_dev->dev_addr, ETH_ALEN);
	uc_4addr_packet->subtype = packet_subtype;
	uc_4addr_packet->reserved = 0;

	ret = true;
out:
	if (primary_if)
		batadv_hardif_free_ref(primary_if);
	return ret;
}

/**
 * batadv_send_skb_unicast - encapsulate and send an skb via unicast
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: payload to send
 * @packet_type: the batman unicast packet type to use
 * @packet_subtype: the unicast 4addr packet subtype (only relevant for unicast
 *  4addr packets)
 * @orig_node: the originator to send the packet to
 * @vid: the vid to be used to search the translation table
 *
 * Wrap the given skb into a batman-adv unicast or unicast-4addr header
 * depending on whether BATADV_UNICAST or BATADV_UNICAST_4ADDR was supplied
 * as packet_type. Then send this frame to the given orig_node and release a
 * reference to this orig_node.
 *
 * Returns NET_XMIT_DROP in case of error or NET_XMIT_SUCCESS otherwise.
 */
static int batadv_send_skb_unicast(struct batadv_priv *bat_priv,
				   struct sk_buff *skb, int packet_type,
				   int packet_subtype,
				   struct batadv_orig_node *orig_node,
				   unsigned short vid)
{
	struct ethhdr *ethhdr = (struct ethhdr *)skb->data;
	struct batadv_unicast_packet *unicast_packet;
	int ret = NET_XMIT_DROP;

	if (!orig_node)
		goto out;

	switch (packet_type) {
	case BATADV_UNICAST:
		if (!batadv_send_skb_prepare_unicast(skb, orig_node))
			goto out;
		break;
	case BATADV_UNICAST_4ADDR:
		if (!batadv_send_skb_prepare_unicast_4addr(bat_priv, skb,
							   orig_node,
							   packet_subtype))
			goto out;
		break;
	default:
		/* this function supports UNICAST and UNICAST_4ADDR only. It
		 * should never be invoked with any other packet type
		 */
		goto out;
	}

	unicast_packet = (struct batadv_unicast_packet *)skb->data;

	/* inform the destination node that we are still missing a correct route
	 * for this client. The destination will receive this packet and will
	 * try to reroute it because the ttvn contained in the header is less
	 * than the current one
	 */
	if (batadv_tt_global_client_is_roaming(bat_priv, ethhdr->h_dest, vid))
		unicast_packet->ttvn = unicast_packet->ttvn - 1;

	if (batadv_send_skb_to_orig(skb, orig_node, NULL) != NET_XMIT_DROP)
		ret = NET_XMIT_SUCCESS;

out:
	if (orig_node)
		batadv_orig_node_free_ref(orig_node);
	if (ret == NET_XMIT_DROP)
		kfree_skb(skb);
	return ret;
}

/**
 * batadv_send_skb_via_tt_generic - send an skb via TT lookup
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: payload to send
 * @packet_type: the batman unicast packet type to use
 * @packet_subtype: the unicast 4addr packet subtype (only relevant for unicast
 *  4addr packets)
 * @vid: the vid to be used to search the translation table
 *
 * Look up the recipient node for the destination address in the ethernet
 * header via the translation table. Wrap the given skb into a batman-adv
 * unicast or unicast-4addr header depending on whether BATADV_UNICAST or
 * BATADV_UNICAST_4ADDR was supplied as packet_type. Then send this frame
 * to the according destination node.
 *
 * Returns NET_XMIT_DROP in case of error or NET_XMIT_SUCCESS otherwise.
 */
int batadv_send_skb_via_tt_generic(struct batadv_priv *bat_priv,
				   struct sk_buff *skb, int packet_type,
				   int packet_subtype, unsigned short vid)
{
	struct ethhdr *ethhdr = (struct ethhdr *)skb->data;
	struct batadv_orig_node *orig_node;

	orig_node = batadv_transtable_search(bat_priv, ethhdr->h_source,
					     ethhdr->h_dest, vid);
	return batadv_send_skb_unicast(bat_priv, skb, packet_type,
				       packet_subtype, orig_node, vid);
}

/**
 * batadv_send_skb_via_gw - send an skb via gateway lookup
 * @bat_priv: the bat priv with all the soft interface information
 * @skb: payload to send
 * @vid: the vid to be used to search the translation table
 *
 * Look up the currently selected gateway. Wrap the given skb into a batman-adv
 * unicast header and send this frame to this gateway node.
 *
 * Returns NET_XMIT_DROP in case of error or NET_XMIT_SUCCESS otherwise.
 */
int batadv_send_skb_via_gw(struct batadv_priv *bat_priv, struct sk_buff *skb,
			   unsigned short vid)
{
	struct batadv_orig_node *orig_node;

	orig_node = batadv_gw_get_selected_orig(bat_priv);
	return batadv_send_skb_unicast(bat_priv, skb, BATADV_UNICAST, 0,
				       orig_node, vid);
}

void batadv_schedule_bat_ogm(struct batadv_hard_iface *hard_iface)
{
	struct batadv_priv *bat_priv = netdev_priv(hard_iface->soft_iface);

	if ((hard_iface->if_status == BATADV_IF_NOT_IN_USE) ||
	    (hard_iface->if_status == BATADV_IF_TO_BE_REMOVED))
		return;

	/* the interface gets activated here to avoid race conditions between
	 * the moment of activating the interface in
	 * hardif_activate_interface() where the originator mac is set and
	 * outdated packets (especially uninitialized mac addresses) in the
	 * packet queue
	 */
	if (hard_iface->if_status == BATADV_IF_TO_BE_ACTIVATED)
		hard_iface->if_status = BATADV_IF_ACTIVE;

	bat_priv->bat_algo_ops->bat_ogm_schedule(hard_iface);
}

static void batadv_forw_packet_free(struct batadv_forw_packet *forw_packet)
>>>>>>> refs/remotes/origin/master
{
	if (forw_packet->skb)
		kfree_skb(forw_packet->skb);
	if (forw_packet->if_incoming)
<<<<<<< HEAD
		hardif_free_ref(forw_packet->if_incoming);
	kfree(forw_packet);
}

static void _add_bcast_packet_to_list(struct bat_priv *bat_priv,
				      struct forw_packet *forw_packet,
				      unsigned long send_time)
{
	INIT_HLIST_NODE(&forw_packet->list);

=======
		batadv_hardif_free_ref(forw_packet->if_incoming);
	kfree(forw_packet);
}

static void
_batadv_add_bcast_packet_to_list(struct batadv_priv *bat_priv,
				 struct batadv_forw_packet *forw_packet,
				 unsigned long send_time)
{
>>>>>>> refs/remotes/origin/master
	/* add new packet to packet list */
	spin_lock_bh(&bat_priv->forw_bcast_list_lock);
	hlist_add_head(&forw_packet->list, &bat_priv->forw_bcast_list);
	spin_unlock_bh(&bat_priv->forw_bcast_list_lock);

	/* start timer for this packet */
<<<<<<< HEAD
	INIT_DELAYED_WORK(&forw_packet->delayed_work,
			  send_outstanding_bcast_packet);
	queue_delayed_work(bat_event_workqueue, &forw_packet->delayed_work,
=======
	queue_delayed_work(batadv_event_workqueue, &forw_packet->delayed_work,
>>>>>>> refs/remotes/origin/master
			   send_time);
}

/* add a broadcast packet to the queue and setup timers. broadcast packets
<<<<<<< HEAD
<<<<<<< HEAD
 * are sent multiple times to increase probability for beeing received.
=======
 * are sent multiple times to increase probability for being received.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * are sent multiple times to increase probability for being received.
>>>>>>> refs/remotes/origin/master
 *
 * This function returns NETDEV_TX_OK on success and NETDEV_TX_BUSY on
 * errors.
 *
 * The skb is not consumed, so the caller should make sure that the
<<<<<<< HEAD
 * skb is freed. */
<<<<<<< HEAD
int add_bcast_packet_to_list(struct bat_priv *bat_priv, struct sk_buff *skb)
=======
int add_bcast_packet_to_list(struct bat_priv *bat_priv,
			     const struct sk_buff *skb, unsigned long delay)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct hard_iface *primary_if = NULL;
	struct forw_packet *forw_packet;
	struct bcast_packet *bcast_packet;
<<<<<<< HEAD
=======
	struct sk_buff *newskb;
>>>>>>> refs/remotes/origin/cm-10.0

	if (!atomic_dec_not_zero(&bat_priv->bcast_queue_left)) {
		bat_dbg(DBG_BATMAN, bat_priv, "bcast packet queue full\n");
		goto out;
	}

	primary_if = primary_if_get_selected(bat_priv);
	if (!primary_if)
		goto out_and_inc;

<<<<<<< HEAD
	forw_packet = kmalloc(sizeof(struct forw_packet), GFP_ATOMIC);
=======
	forw_packet = kmalloc(sizeof(*forw_packet), GFP_ATOMIC);
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * skb is freed.
 */
int batadv_add_bcast_packet_to_list(struct batadv_priv *bat_priv,
				    const struct sk_buff *skb,
				    unsigned long delay)
{
	struct batadv_hard_iface *primary_if = NULL;
	struct batadv_forw_packet *forw_packet;
	struct batadv_bcast_packet *bcast_packet;
	struct sk_buff *newskb;

	if (!batadv_atomic_dec_not_zero(&bat_priv->bcast_queue_left)) {
		batadv_dbg(BATADV_DBG_BATMAN, bat_priv,
			   "bcast packet queue full\n");
		goto out;
	}

	primary_if = batadv_primary_if_get_selected(bat_priv);
	if (!primary_if)
		goto out_and_inc;

	forw_packet = kmalloc(sizeof(*forw_packet), GFP_ATOMIC);
>>>>>>> refs/remotes/origin/master

	if (!forw_packet)
		goto out_and_inc;

<<<<<<< HEAD
<<<<<<< HEAD
	skb = skb_copy(skb, GFP_ATOMIC);
	if (!skb)
		goto packet_free;

	/* as we have a copy now, it is safe to decrease the TTL */
	bcast_packet = (struct bcast_packet *)skb->data;
	bcast_packet->ttl--;

	skb_reset_mac_header(skb);

	forw_packet->skb = skb;
=======
=======
>>>>>>> refs/remotes/origin/master
	newskb = skb_copy(skb, GFP_ATOMIC);
	if (!newskb)
		goto packet_free;

	/* as we have a copy now, it is safe to decrease the TTL */
<<<<<<< HEAD
	bcast_packet = (struct bcast_packet *)newskb->data;
	bcast_packet->header.ttl--;
=======
	bcast_packet = (struct batadv_bcast_packet *)newskb->data;
	bcast_packet->ttl--;
>>>>>>> refs/remotes/origin/master

	skb_reset_mac_header(newskb);

	forw_packet->skb = newskb;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	forw_packet->if_incoming = primary_if;

	/* how often did we send the bcast packet ? */
	forw_packet->num_packets = 0;

<<<<<<< HEAD
<<<<<<< HEAD
	_add_bcast_packet_to_list(bat_priv, forw_packet, 1);
=======
	_add_bcast_packet_to_list(bat_priv, forw_packet, delay);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	INIT_DELAYED_WORK(&forw_packet->delayed_work,
			  batadv_send_outstanding_bcast_packet);

	_batadv_add_bcast_packet_to_list(bat_priv, forw_packet, delay);
>>>>>>> refs/remotes/origin/master
	return NETDEV_TX_OK;

packet_free:
	kfree(forw_packet);
out_and_inc:
	atomic_inc(&bat_priv->bcast_queue_left);
out:
	if (primary_if)
<<<<<<< HEAD
		hardif_free_ref(primary_if);
	return NETDEV_TX_BUSY;
}

static void send_outstanding_bcast_packet(struct work_struct *work)
{
	struct hard_iface *hard_iface;
	struct delayed_work *delayed_work =
		container_of(work, struct delayed_work, work);
	struct forw_packet *forw_packet =
		container_of(delayed_work, struct forw_packet, delayed_work);
	struct sk_buff *skb1;
	struct net_device *soft_iface = forw_packet->if_incoming->soft_iface;
	struct bat_priv *bat_priv = netdev_priv(soft_iface);
=======
		batadv_hardif_free_ref(primary_if);
	return NETDEV_TX_BUSY;
}

static void batadv_send_outstanding_bcast_packet(struct work_struct *work)
{
	struct batadv_hard_iface *hard_iface;
	struct delayed_work *delayed_work;
	struct batadv_forw_packet *forw_packet;
	struct sk_buff *skb1;
	struct net_device *soft_iface;
	struct batadv_priv *bat_priv;

	delayed_work = container_of(work, struct delayed_work, work);
	forw_packet = container_of(delayed_work, struct batadv_forw_packet,
				   delayed_work);
	soft_iface = forw_packet->if_incoming->soft_iface;
	bat_priv = netdev_priv(soft_iface);
>>>>>>> refs/remotes/origin/master

	spin_lock_bh(&bat_priv->forw_bcast_list_lock);
	hlist_del(&forw_packet->list);
	spin_unlock_bh(&bat_priv->forw_bcast_list_lock);

<<<<<<< HEAD
	if (atomic_read(&bat_priv->mesh_state) == MESH_DEACTIVATING)
=======
	if (atomic_read(&bat_priv->mesh_state) == BATADV_MESH_DEACTIVATING)
		goto out;

	if (batadv_dat_drop_broadcast_packet(bat_priv, forw_packet))
>>>>>>> refs/remotes/origin/master
		goto out;

	/* rebroadcast packet */
	rcu_read_lock();
<<<<<<< HEAD
	list_for_each_entry_rcu(hard_iface, &hardif_list, list) {
		if (hard_iface->soft_iface != soft_iface)
			continue;

		/* send a copy of the saved skb */
		skb1 = skb_clone(forw_packet->skb, GFP_ATOMIC);
		if (skb1)
			send_skb_packet(skb1, hard_iface, broadcast_addr);
=======
	list_for_each_entry_rcu(hard_iface, &batadv_hardif_list, list) {
		if (hard_iface->soft_iface != soft_iface)
			continue;

		if (forw_packet->num_packets >= hard_iface->num_bcasts)
			continue;

		/* send a copy of the saved skb */
		skb1 = skb_clone(forw_packet->skb, GFP_ATOMIC);
		if (skb1)
			batadv_send_skb_packet(skb1, hard_iface,
					       batadv_broadcast_addr);
>>>>>>> refs/remotes/origin/master
	}
	rcu_read_unlock();

	forw_packet->num_packets++;

	/* if we still have some more bcasts to send */
<<<<<<< HEAD
	if (forw_packet->num_packets < 3) {
		_add_bcast_packet_to_list(bat_priv, forw_packet,
					  ((5 * HZ) / 1000));
=======
	if (forw_packet->num_packets < BATADV_NUM_BCASTS_MAX) {
		_batadv_add_bcast_packet_to_list(bat_priv, forw_packet,
						 msecs_to_jiffies(5));
>>>>>>> refs/remotes/origin/master
		return;
	}

out:
<<<<<<< HEAD
	forw_packet_free(forw_packet);
	atomic_inc(&bat_priv->bcast_queue_left);
}

<<<<<<< HEAD
void send_outstanding_bat_packet(struct work_struct *work)
=======
void send_outstanding_bat_ogm_packet(struct work_struct *work)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct delayed_work *delayed_work =
		container_of(work, struct delayed_work, work);
	struct forw_packet *forw_packet =
		container_of(delayed_work, struct forw_packet, delayed_work);
	struct bat_priv *bat_priv;

=======
	batadv_forw_packet_free(forw_packet);
	atomic_inc(&bat_priv->bcast_queue_left);
}

void batadv_send_outstanding_bat_ogm_packet(struct work_struct *work)
{
	struct delayed_work *delayed_work;
	struct batadv_forw_packet *forw_packet;
	struct batadv_priv *bat_priv;

	delayed_work = container_of(work, struct delayed_work, work);
	forw_packet = container_of(delayed_work, struct batadv_forw_packet,
				   delayed_work);
>>>>>>> refs/remotes/origin/master
	bat_priv = netdev_priv(forw_packet->if_incoming->soft_iface);
	spin_lock_bh(&bat_priv->forw_bat_list_lock);
	hlist_del(&forw_packet->list);
	spin_unlock_bh(&bat_priv->forw_bat_list_lock);

<<<<<<< HEAD
	if (atomic_read(&bat_priv->mesh_state) == MESH_DEACTIVATING)
		goto out;

<<<<<<< HEAD
	send_packet(forw_packet);
=======
	bat_priv->bat_algo_ops->bat_ogm_emit(forw_packet);
>>>>>>> refs/remotes/origin/cm-10.0

	/**
	 * we have to have at least one packet in the queue
=======
	if (atomic_read(&bat_priv->mesh_state) == BATADV_MESH_DEACTIVATING)
		goto out;

	bat_priv->bat_algo_ops->bat_ogm_emit(forw_packet);

	/* we have to have at least one packet in the queue
>>>>>>> refs/remotes/origin/master
	 * to determine the queues wake up time unless we are
	 * shutting down
	 */
	if (forw_packet->own)
<<<<<<< HEAD
<<<<<<< HEAD
		schedule_own_packet(forw_packet->if_incoming);
=======
		schedule_bat_ogm(forw_packet->if_incoming);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		batadv_schedule_bat_ogm(forw_packet->if_incoming);
>>>>>>> refs/remotes/origin/master

out:
	/* don't count own packet */
	if (!forw_packet->own)
		atomic_inc(&bat_priv->batman_queue_left);

<<<<<<< HEAD
	forw_packet_free(forw_packet);
}

void purge_outstanding_packets(struct bat_priv *bat_priv,
<<<<<<< HEAD
			       struct hard_iface *hard_iface)
=======
			       const struct hard_iface *hard_iface)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct forw_packet *forw_packet;
	struct hlist_node *tmp_node, *safe_tmp_node;
	bool pending;

	if (hard_iface)
		bat_dbg(DBG_BATMAN, bat_priv,
			"purge_outstanding_packets(): %s\n",
			hard_iface->net_dev->name);
	else
		bat_dbg(DBG_BATMAN, bat_priv,
			"purge_outstanding_packets()\n");

	/* free bcast list */
	spin_lock_bh(&bat_priv->forw_bcast_list_lock);
	hlist_for_each_entry_safe(forw_packet, tmp_node, safe_tmp_node,
				  &bat_priv->forw_bcast_list, list) {

		/**
<<<<<<< HEAD
		 * if purge_outstanding_packets() was called with an argmument
=======
		 * if purge_outstanding_packets() was called with an argument
>>>>>>> refs/remotes/origin/cm-10.0
=======
	batadv_forw_packet_free(forw_packet);
}

void
batadv_purge_outstanding_packets(struct batadv_priv *bat_priv,
				 const struct batadv_hard_iface *hard_iface)
{
	struct batadv_forw_packet *forw_packet;
	struct hlist_node *safe_tmp_node;
	bool pending;

	if (hard_iface)
		batadv_dbg(BATADV_DBG_BATMAN, bat_priv,
			   "purge_outstanding_packets(): %s\n",
			   hard_iface->net_dev->name);
	else
		batadv_dbg(BATADV_DBG_BATMAN, bat_priv,
			   "purge_outstanding_packets()\n");

	/* free bcast list */
	spin_lock_bh(&bat_priv->forw_bcast_list_lock);
	hlist_for_each_entry_safe(forw_packet, safe_tmp_node,
				  &bat_priv->forw_bcast_list, list) {
		/* if purge_outstanding_packets() was called with an argument
>>>>>>> refs/remotes/origin/master
		 * we delete only packets belonging to the given interface
		 */
		if ((hard_iface) &&
		    (forw_packet->if_incoming != hard_iface))
			continue;

		spin_unlock_bh(&bat_priv->forw_bcast_list_lock);

<<<<<<< HEAD
		/**
		 * send_outstanding_bcast_packet() will lock the list to
=======
		/* batadv_send_outstanding_bcast_packet() will lock the list to
>>>>>>> refs/remotes/origin/master
		 * delete the item from the list
		 */
		pending = cancel_delayed_work_sync(&forw_packet->delayed_work);
		spin_lock_bh(&bat_priv->forw_bcast_list_lock);

		if (pending) {
			hlist_del(&forw_packet->list);
<<<<<<< HEAD
			forw_packet_free(forw_packet);
=======
			batadv_forw_packet_free(forw_packet);
>>>>>>> refs/remotes/origin/master
		}
	}
	spin_unlock_bh(&bat_priv->forw_bcast_list_lock);

	/* free batman packet list */
	spin_lock_bh(&bat_priv->forw_bat_list_lock);
<<<<<<< HEAD
	hlist_for_each_entry_safe(forw_packet, tmp_node, safe_tmp_node,
				  &bat_priv->forw_bat_list, list) {

		/**
<<<<<<< HEAD
		 * if purge_outstanding_packets() was called with an argmument
=======
		 * if purge_outstanding_packets() was called with an argument
>>>>>>> refs/remotes/origin/cm-10.0
=======
	hlist_for_each_entry_safe(forw_packet, safe_tmp_node,
				  &bat_priv->forw_bat_list, list) {
		/* if purge_outstanding_packets() was called with an argument
>>>>>>> refs/remotes/origin/master
		 * we delete only packets belonging to the given interface
		 */
		if ((hard_iface) &&
		    (forw_packet->if_incoming != hard_iface))
			continue;

		spin_unlock_bh(&bat_priv->forw_bat_list_lock);

<<<<<<< HEAD
		/**
		 * send_outstanding_bat_packet() will lock the list to
=======
		/* send_outstanding_bat_packet() will lock the list to
>>>>>>> refs/remotes/origin/master
		 * delete the item from the list
		 */
		pending = cancel_delayed_work_sync(&forw_packet->delayed_work);
		spin_lock_bh(&bat_priv->forw_bat_list_lock);

		if (pending) {
			hlist_del(&forw_packet->list);
<<<<<<< HEAD
			forw_packet_free(forw_packet);
=======
			batadv_forw_packet_free(forw_packet);
>>>>>>> refs/remotes/origin/master
		}
	}
	spin_unlock_bh(&bat_priv->forw_bat_list_lock);
}
