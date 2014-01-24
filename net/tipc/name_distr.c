/*
 * net/tipc/name_distr.c: TIPC name distribution code
 *
 * Copyright (c) 2000-2006, Ericsson AB
 * Copyright (c) 2005, 2010-2011, Wind River Systems
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
#include "link.h"
#include "name_distr.h"

#define ITEM_SIZE sizeof(struct distr_item)

/**
 * struct distr_item - publication info distributed to other nodes
 * @type: name sequence type
 * @lower: name sequence lower bound
 * @upper: name sequence upper bound
 * @ref: publishing port reference
 * @key: publication key
 *
 * ===> All fields are stored in network byte order. <===
 *
 * First 3 fields identify (name or) name sequence being published.
 * Reference field uniquely identifies port that published name sequence.
 * Key field uniquely identifies publication, in the event a port has
 * multiple publications of the same name sequence.
 *
 * Note: There is no field that identifies the publishing node because it is
 * the same for all items contained within a publication message.
 */
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
struct distr_item {
	__be32 type;
	__be32 lower;
	__be32 upper;
	__be32 ref;
	__be32 key;
};

/**
<<<<<<< HEAD
 * List of externally visible publications by this node --
 * that is, all publications having scope > TIPC_NODE_SCOPE.
 */

static LIST_HEAD(publ_root);
static u32 publ_cnt;
=======
 * struct publ_list - list of publications made by this node
 * @list: circular list of publications
 * @list_size: number of entries in list
 */
struct publ_list {
	struct list_head list;
	u32 size;
};

static struct publ_list publ_zone = {
	.list = LIST_HEAD_INIT(publ_zone.list),
	.size = 0,
};

static struct publ_list publ_cluster = {
	.list = LIST_HEAD_INIT(publ_cluster.list),
	.size = 0,
};

static struct publ_list publ_node = {
	.list = LIST_HEAD_INIT(publ_node.list),
	.size = 0,
};

static struct publ_list *publ_lists[] = {
	NULL,
	&publ_zone,	/* publ_lists[TIPC_ZONE_SCOPE]		*/
	&publ_cluster,	/* publ_lists[TIPC_CLUSTER_SCOPE]	*/
	&publ_node	/* publ_lists[TIPC_NODE_SCOPE]		*/
};

>>>>>>> refs/remotes/origin/master

/**
 * publ_to_item - add publication info to a publication message
 */
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
static void publ_to_item(struct distr_item *i, struct publication *p)
{
	i->type = htonl(p->type);
	i->lower = htonl(p->lower);
	i->upper = htonl(p->upper);
	i->ref = htonl(p->ref);
	i->key = htonl(p->key);
}

/**
 * named_prepare_buf - allocate & initialize a publication message
 */
<<<<<<< HEAD

static struct sk_buff *named_prepare_buf(u32 type, u32 size, u32 dest)
{
<<<<<<< HEAD
	struct sk_buff *buf = tipc_buf_acquire(LONG_H_SIZE + size);
=======
	struct sk_buff *buf = tipc_buf_acquire(INT_H_SIZE + size);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct sk_buff *named_prepare_buf(u32 type, u32 size, u32 dest)
{
	struct sk_buff *buf = tipc_buf_acquire(INT_H_SIZE + size);
>>>>>>> refs/remotes/origin/master
	struct tipc_msg *msg;

	if (buf != NULL) {
		msg = buf_msg(buf);
<<<<<<< HEAD
<<<<<<< HEAD
		tipc_msg_init(msg, NAME_DISTRIBUTOR, type, LONG_H_SIZE, dest);
		msg_set_size(msg, LONG_H_SIZE + size);
=======
		tipc_msg_init(msg, NAME_DISTRIBUTOR, type, INT_H_SIZE, dest);
		msg_set_size(msg, INT_H_SIZE + size);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		tipc_msg_init(msg, NAME_DISTRIBUTOR, type, INT_H_SIZE, dest);
		msg_set_size(msg, INT_H_SIZE + size);
>>>>>>> refs/remotes/origin/master
	}
	return buf;
}

static void named_cluster_distribute(struct sk_buff *buf)
{
	struct sk_buff *buf_copy;
	struct tipc_node *n_ptr;

	list_for_each_entry(n_ptr, &tipc_node_list, list) {
		if (tipc_node_active_links(n_ptr)) {
			buf_copy = skb_copy(buf, GFP_ATOMIC);
			if (!buf_copy)
				break;
			msg_set_destnode(buf_msg(buf_copy), n_ptr->addr);
			tipc_link_send(buf_copy, n_ptr->addr, n_ptr->addr);
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	buf_discard(buf);
=======
	kfree_skb(buf);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	kfree_skb(buf);
>>>>>>> refs/remotes/origin/master
}

/**
 * tipc_named_publish - tell other nodes about a new publication by this node
 */
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
void tipc_named_publish(struct publication *publ)
{
	struct sk_buff *buf;
	struct distr_item *item;

<<<<<<< HEAD
	list_add_tail(&publ->local_list, &publ_root);
	publ_cnt++;

	buf = named_prepare_buf(PUBLICATION, ITEM_SIZE, 0);
	if (!buf) {
		warn("Publication distribution failure\n");
=======
	list_add_tail(&publ->local_list, &publ_lists[publ->scope]->list);
	publ_lists[publ->scope]->size++;

	if (publ->scope == TIPC_NODE_SCOPE)
		return;

	buf = named_prepare_buf(PUBLICATION, ITEM_SIZE, 0);
	if (!buf) {
		pr_warn("Publication distribution failure\n");
>>>>>>> refs/remotes/origin/master
		return;
	}

	item = (struct distr_item *)msg_data(buf_msg(buf));
	publ_to_item(item, publ);
	named_cluster_distribute(buf);
}

/**
 * tipc_named_withdraw - tell other nodes about a withdrawn publication by this node
 */
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
void tipc_named_withdraw(struct publication *publ)
{
	struct sk_buff *buf;
	struct distr_item *item;

	list_del(&publ->local_list);
<<<<<<< HEAD
	publ_cnt--;

	buf = named_prepare_buf(WITHDRAWAL, ITEM_SIZE, 0);
	if (!buf) {
		warn("Withdrawal distribution failure\n");
=======
	publ_lists[publ->scope]->size--;

	if (publ->scope == TIPC_NODE_SCOPE)
		return;

	buf = named_prepare_buf(WITHDRAWAL, ITEM_SIZE, 0);
	if (!buf) {
		pr_warn("Withdrawal distribution failure\n");
>>>>>>> refs/remotes/origin/master
		return;
	}

	item = (struct distr_item *)msg_data(buf_msg(buf));
	publ_to_item(item, publ);
	named_cluster_distribute(buf);
}

<<<<<<< HEAD
/**
 * tipc_named_node_up - tell specified node about all publications by this node
 */

<<<<<<< HEAD
void tipc_named_node_up(unsigned long node)
{
	struct publication *publ;
	struct distr_item *item = NULL;
	struct sk_buff *buf = NULL;
	u32 left = 0;
	u32 rest;
	u32 max_item_buf;

	read_lock_bh(&tipc_nametbl_lock);
	max_item_buf = TIPC_MAX_USER_MSG_SIZE / ITEM_SIZE;
	max_item_buf *= ITEM_SIZE;
=======
=======
/*
 * named_distribute - prepare name info for bulk distribution to another node
 */
static void named_distribute(struct list_head *message_list, u32 node,
			     struct publ_list *pls, u32 max_item_buf)
{
	struct publication *publ;
	struct sk_buff *buf = NULL;
	struct distr_item *item = NULL;
	u32 left = 0;
	u32 rest = pls->size * ITEM_SIZE;

	list_for_each_entry(publ, &pls->list, local_list) {
		if (!buf) {
			left = (rest <= max_item_buf) ? rest : max_item_buf;
			rest -= left;
			buf = named_prepare_buf(PUBLICATION, left, node);
			if (!buf) {
				pr_warn("Bulk publication failure\n");
				return;
			}
			item = (struct distr_item *)msg_data(buf_msg(buf));
		}
		publ_to_item(item, publ);
		item++;
		left -= ITEM_SIZE;
		if (!left) {
			list_add_tail((struct list_head *)buf, message_list);
			buf = NULL;
		}
	}
}

/**
 * tipc_named_node_up - tell specified node about all publications by this node
 */
>>>>>>> refs/remotes/origin/master
void tipc_named_node_up(unsigned long nodearg)
{
	struct tipc_node *n_ptr;
	struct tipc_link *l_ptr;
<<<<<<< HEAD
	struct publication *publ;
	struct distr_item *item = NULL;
	struct sk_buff *buf = NULL;
	struct list_head message_list;
	u32 node = (u32)nodearg;
	u32 left = 0;
	u32 rest;
	u32 max_item_buf = 0;

	/* compute maximum amount of publication data to send per message */

=======
	struct list_head message_list;
	u32 node = (u32)nodearg;
	u32 max_item_buf = 0;

	/* compute maximum amount of publication data to send per message */
>>>>>>> refs/remotes/origin/master
	read_lock_bh(&tipc_net_lock);
	n_ptr = tipc_node_find(node);
	if (n_ptr) {
		tipc_node_lock(n_ptr);
		l_ptr = n_ptr->active_links[0];
		if (l_ptr)
			max_item_buf = ((l_ptr->max_pkt - INT_H_SIZE) /
				ITEM_SIZE) * ITEM_SIZE;
		tipc_node_unlock(n_ptr);
	}
	read_unlock_bh(&tipc_net_lock);
	if (!max_item_buf)
		return;

	/* create list of publication messages, then send them as a unit */
<<<<<<< HEAD

	INIT_LIST_HEAD(&message_list);

	read_lock_bh(&tipc_nametbl_lock);
>>>>>>> refs/remotes/origin/cm-10.0
	rest = publ_cnt * ITEM_SIZE;

	list_for_each_entry(publ, &publ_root, local_list) {
		if (!buf) {
			left = (rest <= max_item_buf) ? rest : max_item_buf;
			rest -= left;
			buf = named_prepare_buf(PUBLICATION, left, node);
			if (!buf) {
				warn("Bulk publication distribution failure\n");
				goto exit;
			}
			item = (struct distr_item *)msg_data(buf_msg(buf));
		}
		publ_to_item(item, publ);
		item++;
		left -= ITEM_SIZE;
		if (!left) {
<<<<<<< HEAD
			msg_set_link_selector(buf_msg(buf), node);
			tipc_link_send(buf, node, node);
=======
			list_add_tail((struct list_head *)buf, &message_list);
>>>>>>> refs/remotes/origin/cm-10.0
			buf = NULL;
		}
	}
exit:
	read_unlock_bh(&tipc_nametbl_lock);
<<<<<<< HEAD
=======

	tipc_link_send_names(&message_list, (u32)node);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	INIT_LIST_HEAD(&message_list);

	read_lock_bh(&tipc_nametbl_lock);
	named_distribute(&message_list, node, &publ_cluster, max_item_buf);
	named_distribute(&message_list, node, &publ_zone, max_item_buf);
	read_unlock_bh(&tipc_nametbl_lock);

	tipc_link_send_names(&message_list, node);
>>>>>>> refs/remotes/origin/master
}

/**
 * named_purge_publ - remove publication associated with a failed node
 *
 * Invoked for each publication issued by a newly failed node.
 * Removes publication structure from name table & deletes it.
<<<<<<< HEAD
<<<<<<< HEAD
 * In rare cases the link may have come back up again when this
 * function is called, and we have two items representing the same
 * publication. Nudge this item's key to distinguish it from the other.
=======
>>>>>>> refs/remotes/origin/cm-10.0
 */

=======
 */
>>>>>>> refs/remotes/origin/master
static void named_purge_publ(struct publication *publ)
{
	struct publication *p;

	write_lock_bh(&tipc_nametbl_lock);
<<<<<<< HEAD
<<<<<<< HEAD
	publ->key += 1222345;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	p = tipc_nametbl_remove_publ(publ->type, publ->lower,
				     publ->node, publ->ref, publ->key);
	if (p)
		tipc_nodesub_unsubscribe(&p->subscr);
	write_unlock_bh(&tipc_nametbl_lock);

	if (p != publ) {
<<<<<<< HEAD
		err("Unable to remove publication from failed node\n"
		    "(type=%u, lower=%u, node=0x%x, ref=%u, key=%u)\n",
		    publ->type, publ->lower, publ->node, publ->ref, publ->key);
=======
		pr_err("Unable to remove publication from failed node\n"
		       " (type=%u, lower=%u, node=0x%x, ref=%u, key=%u)\n",
		       publ->type, publ->lower, publ->node, publ->ref,
		       publ->key);
>>>>>>> refs/remotes/origin/master
	}

	kfree(p);
}

/**
 * tipc_named_recv - process name table update message sent by another node
 */
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
void tipc_named_recv(struct sk_buff *buf)
{
	struct publication *publ;
	struct tipc_msg *msg = buf_msg(buf);
	struct distr_item *item = (struct distr_item *)msg_data(msg);
	u32 count = msg_data_sz(msg) / ITEM_SIZE;

	write_lock_bh(&tipc_nametbl_lock);
	while (count--) {
		if (msg_type(msg) == PUBLICATION) {
			publ = tipc_nametbl_insert_publ(ntohl(item->type),
							ntohl(item->lower),
							ntohl(item->upper),
							TIPC_CLUSTER_SCOPE,
							msg_orignode(msg),
							ntohl(item->ref),
							ntohl(item->key));
			if (publ) {
				tipc_nodesub_subscribe(&publ->subscr,
						       msg_orignode(msg),
						       publ,
						       (net_ev_handler)
						       named_purge_publ);
			}
		} else if (msg_type(msg) == WITHDRAWAL) {
			publ = tipc_nametbl_remove_publ(ntohl(item->type),
							ntohl(item->lower),
							msg_orignode(msg),
							ntohl(item->ref),
							ntohl(item->key));

			if (publ) {
				tipc_nodesub_unsubscribe(&publ->subscr);
				kfree(publ);
			} else {
<<<<<<< HEAD
				err("Unable to remove publication by node 0x%x\n"
				    "(type=%u, lower=%u, ref=%u, key=%u)\n",
				    msg_orignode(msg),
				    ntohl(item->type), ntohl(item->lower),
				    ntohl(item->ref), ntohl(item->key));
			}
		} else {
			warn("Unrecognized name table message received\n");
=======
				pr_err("Unable to remove publication by node 0x%x\n"
				       " (type=%u, lower=%u, ref=%u, key=%u)\n",
				       msg_orignode(msg), ntohl(item->type),
				       ntohl(item->lower), ntohl(item->ref),
				       ntohl(item->key));
			}
		} else {
			pr_warn("Unrecognized name table message received\n");
>>>>>>> refs/remotes/origin/master
		}
		item++;
	}
	write_unlock_bh(&tipc_nametbl_lock);
<<<<<<< HEAD
<<<<<<< HEAD
	buf_discard(buf);
=======
	kfree_skb(buf);
>>>>>>> refs/remotes/origin/cm-10.0
}

/**
 * tipc_named_reinit - re-initialize local publication list
 *
<<<<<<< HEAD
 * This routine is called whenever TIPC networking is (re)enabled.
 * All existing publications by this node that have "cluster" or "zone" scope
 * are updated to reflect the node's current network address.
 * (If the node's address is unchanged, the update loop terminates immediately.)
=======
 * This routine is called whenever TIPC networking is enabled.
 * All existing publications by this node that have "cluster" or "zone" scope
 * are updated to reflect the node's new network address.
>>>>>>> refs/remotes/origin/cm-10.0
 */

void tipc_named_reinit(void)
{
	struct publication *publ;

	write_lock_bh(&tipc_nametbl_lock);
<<<<<<< HEAD
	list_for_each_entry(publ, &publ_root, local_list) {
		if (publ->node == tipc_own_addr)
			break;
		publ->node = tipc_own_addr;
	}
=======

	list_for_each_entry(publ, &publ_root, local_list)
		publ->node = tipc_own_addr;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	kfree_skb(buf);
}

/**
 * tipc_named_reinit - re-initialize local publications
 *
 * This routine is called whenever TIPC networking is enabled.
 * All name table entries published by this node are updated to reflect
 * the node's new network address.
 */
void tipc_named_reinit(void)
{
	struct publication *publ;
	int scope;

	write_lock_bh(&tipc_nametbl_lock);

	for (scope = TIPC_ZONE_SCOPE; scope <= TIPC_NODE_SCOPE; scope++)
		list_for_each_entry(publ, &publ_lists[scope]->list, local_list)
			publ->node = tipc_own_addr;

>>>>>>> refs/remotes/origin/master
	write_unlock_bh(&tipc_nametbl_lock);
}
