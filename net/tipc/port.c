/*
 * net/tipc/port.c: TIPC port code
 *
 * Copyright (c) 1992-2007, Ericsson AB
<<<<<<< HEAD
 * Copyright (c) 2004-2008, 2010-2011, Wind River Systems
=======
 * Copyright (c) 2004-2008, 2010-2013, Wind River Systems
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
#include "config.h"
#include "port.h"
#include "name_table.h"

/* Connection management: */
#define PROBING_INTERVAL 3600000	/* [ms] => 1 h */
#define CONFIRMED 0
#define PROBING 1

#define MAX_REJECT_SIZE 1024

<<<<<<< HEAD
static struct sk_buff *msg_queue_head;
static struct sk_buff *msg_queue_tail;

DEFINE_SPINLOCK(tipc_port_list_lock);
static DEFINE_SPINLOCK(queue_lock);
=======
DEFINE_SPINLOCK(tipc_port_list_lock);
>>>>>>> refs/remotes/origin/master

static LIST_HEAD(ports);
static void port_handle_node_down(unsigned long ref);
static struct sk_buff *port_build_self_abort_msg(struct tipc_port *, u32 err);
static struct sk_buff *port_build_peer_abort_msg(struct tipc_port *, u32 err);
static void port_timeout(unsigned long ref);


static u32 port_peernode(struct tipc_port *p_ptr)
{
	return msg_destnode(&p_ptr->phdr);
}

static u32 port_peerport(struct tipc_port *p_ptr)
{
	return msg_destport(&p_ptr->phdr);
}

/**
<<<<<<< HEAD
 * tipc_multicast - send a multicast message to local and remote destinations
 */

int tipc_multicast(u32 ref, struct tipc_name_seq const *seq,
		   u32 num_sect, struct iovec const *msg_sect,
		   unsigned int total_len)
=======
 * tipc_port_peer_msg - verify message was sent by connected port's peer
 *
 * Handles cases where the node's network address has changed from
 * the default of <0.0.0> to its configured setting.
 */
int tipc_port_peer_msg(struct tipc_port *p_ptr, struct tipc_msg *msg)
{
	u32 peernode;
	u32 orignode;

	if (msg_origport(msg) != port_peerport(p_ptr))
		return 0;

	orignode = msg_orignode(msg);
	peernode = port_peernode(p_ptr);
	return (orignode == peernode) ||
		(!orignode && (peernode == tipc_own_addr)) ||
		(!peernode && (orignode == tipc_own_addr));
}

/**
 * tipc_multicast - send a multicast message to local and remote destinations
 */
int tipc_multicast(u32 ref, struct tipc_name_seq const *seq,
		   struct iovec const *msg_sect, unsigned int len)
>>>>>>> refs/remotes/origin/master
{
	struct tipc_msg *hdr;
	struct sk_buff *buf;
	struct sk_buff *ibuf = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
	struct port_list dports = {0, NULL, };
=======
	struct tipc_port_list dports = {0, NULL, };
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct tipc_port_list dports = {0, NULL, };
>>>>>>> refs/remotes/origin/master
	struct tipc_port *oport = tipc_port_deref(ref);
	int ext_targets;
	int res;

	if (unlikely(!oport))
		return -EINVAL;

	/* Create multicast message */
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
	hdr = &oport->phdr;
	msg_set_type(hdr, TIPC_MCAST_MSG);
	msg_set_lookup_scope(hdr, TIPC_CLUSTER_SCOPE);
	msg_set_destport(hdr, 0);
	msg_set_destnode(hdr, 0);
	msg_set_nametype(hdr, seq->type);
	msg_set_namelower(hdr, seq->lower);
	msg_set_nameupper(hdr, seq->upper);
	msg_set_hdr_sz(hdr, MCAST_H_SIZE);
<<<<<<< HEAD
	res = tipc_msg_build(hdr, msg_sect, num_sect, total_len, MAX_MSG_SIZE,
			!oport->user_port, &buf);
=======
	res = tipc_msg_build(hdr, msg_sect, len, MAX_MSG_SIZE, &buf);
>>>>>>> refs/remotes/origin/master
	if (unlikely(!buf))
		return res;

	/* Figure out where to send multicast message */
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
	ext_targets = tipc_nametbl_mc_translate(seq->type, seq->lower, seq->upper,
						TIPC_NODE_SCOPE, &dports);

	/* Send message to destinations (duplicate it only if necessary) */
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
	if (ext_targets) {
		if (dports.count != 0) {
			ibuf = skb_copy(buf, GFP_ATOMIC);
			if (ibuf == NULL) {
				tipc_port_list_free(&dports);
<<<<<<< HEAD
<<<<<<< HEAD
				buf_discard(buf);
=======
				kfree_skb(buf);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				kfree_skb(buf);
>>>>>>> refs/remotes/origin/master
				return -ENOMEM;
			}
		}
		res = tipc_bclink_send_msg(buf);
		if ((res < 0) && (dports.count != 0))
<<<<<<< HEAD
<<<<<<< HEAD
			buf_discard(ibuf);
=======
			kfree_skb(ibuf);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			kfree_skb(ibuf);
>>>>>>> refs/remotes/origin/master
	} else {
		ibuf = buf;
	}

	if (res >= 0) {
		if (ibuf)
			tipc_port_recv_mcast(ibuf, &dports);
	} else {
		tipc_port_list_free(&dports);
	}
	return res;
}

/**
 * tipc_port_recv_mcast - deliver multicast message to all destination ports
 *
 * If there is no port list, perform a lookup to create one
 */
<<<<<<< HEAD

<<<<<<< HEAD
void tipc_port_recv_mcast(struct sk_buff *buf, struct port_list *dp)
{
	struct tipc_msg *msg;
	struct port_list dports = {0, NULL, };
	struct port_list *item = dp;
=======
=======
>>>>>>> refs/remotes/origin/master
void tipc_port_recv_mcast(struct sk_buff *buf, struct tipc_port_list *dp)
{
	struct tipc_msg *msg;
	struct tipc_port_list dports = {0, NULL, };
	struct tipc_port_list *item = dp;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	int cnt = 0;

	msg = buf_msg(buf);

	/* Create destination port list, if one wasn't supplied */
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
	if (dp == NULL) {
		tipc_nametbl_mc_translate(msg_nametype(msg),
				     msg_namelower(msg),
				     msg_nameupper(msg),
				     TIPC_CLUSTER_SCOPE,
				     &dports);
		item = dp = &dports;
	}

	/* Deliver a copy of message to each destination port */
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
	if (dp->count != 0) {
		msg_set_destnode(msg, tipc_own_addr);
		if (dp->count == 1) {
			msg_set_destport(msg, dp->ports[0]);
			tipc_port_recv_msg(buf);
			tipc_port_list_free(dp);
			return;
		}
		for (; cnt < dp->count; cnt++) {
			int index = cnt % PLSIZE;
			struct sk_buff *b = skb_clone(buf, GFP_ATOMIC);

			if (b == NULL) {
<<<<<<< HEAD
				warn("Unable to deliver multicast message(s)\n");
=======
				pr_warn("Unable to deliver multicast message(s)\n");
>>>>>>> refs/remotes/origin/master
				goto exit;
			}
			if ((index == 0) && (cnt != 0))
				item = item->next;
			msg_set_destport(buf_msg(b), item->ports[index]);
			tipc_port_recv_msg(b);
		}
	}
exit:
<<<<<<< HEAD
<<<<<<< HEAD
	buf_discard(buf);
=======
	kfree_skb(buf);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	kfree_skb(buf);
>>>>>>> refs/remotes/origin/master
	tipc_port_list_free(dp);
}

/**
<<<<<<< HEAD
 * tipc_createport_raw - create a generic TIPC port
 *
 * Returns pointer to (locked) TIPC port, or NULL if unable to create it
 */

struct tipc_port *tipc_createport_raw(void *usr_handle,
			u32 (*dispatcher)(struct tipc_port *, struct sk_buff *),
			void (*wakeup)(struct tipc_port *),
			const u32 importance)
=======
 * tipc_createport - create a generic TIPC port
 *
 * Returns pointer to (locked) TIPC port, or NULL if unable to create it
 */
struct tipc_port *tipc_createport(struct sock *sk,
				  u32 (*dispatcher)(struct tipc_port *,
				  struct sk_buff *),
				  void (*wakeup)(struct tipc_port *),
				  const u32 importance)
>>>>>>> refs/remotes/origin/master
{
	struct tipc_port *p_ptr;
	struct tipc_msg *msg;
	u32 ref;

	p_ptr = kzalloc(sizeof(*p_ptr), GFP_ATOMIC);
	if (!p_ptr) {
<<<<<<< HEAD
		warn("Port creation failed, no memory\n");
=======
		pr_warn("Port creation failed, no memory\n");
>>>>>>> refs/remotes/origin/master
		return NULL;
	}
	ref = tipc_ref_acquire(p_ptr, &p_ptr->lock);
	if (!ref) {
<<<<<<< HEAD
		warn("Port creation failed, reference table exhausted\n");
=======
		pr_warn("Port creation failed, ref. table exhausted\n");
>>>>>>> refs/remotes/origin/master
		kfree(p_ptr);
		return NULL;
	}

<<<<<<< HEAD
	p_ptr->usr_handle = usr_handle;
	p_ptr->max_pkt = MAX_PKT_DEFAULT;
	p_ptr->ref = ref;
	msg = &p_ptr->phdr;
<<<<<<< HEAD
	tipc_msg_init(msg, importance, TIPC_NAMED_MSG, LONG_H_SIZE, 0);
=======
	tipc_msg_init(msg, importance, TIPC_NAMED_MSG, NAMED_H_SIZE, 0);
>>>>>>> refs/remotes/origin/cm-10.0
	msg_set_origport(msg, ref);
=======
	p_ptr->sk = sk;
	p_ptr->max_pkt = MAX_PKT_DEFAULT;
	p_ptr->ref = ref;
>>>>>>> refs/remotes/origin/master
	INIT_LIST_HEAD(&p_ptr->wait_list);
	INIT_LIST_HEAD(&p_ptr->subscription.nodesub_list);
	p_ptr->dispatcher = dispatcher;
	p_ptr->wakeup = wakeup;
<<<<<<< HEAD
	p_ptr->user_port = NULL;
	k_init_timer(&p_ptr->timer, (Handler)port_timeout, ref);
	spin_lock_bh(&tipc_port_list_lock);
	INIT_LIST_HEAD(&p_ptr->publications);
	INIT_LIST_HEAD(&p_ptr->port_list);
=======
	k_init_timer(&p_ptr->timer, (Handler)port_timeout, ref);
	INIT_LIST_HEAD(&p_ptr->publications);
	INIT_LIST_HEAD(&p_ptr->port_list);

	/*
	 * Must hold port list lock while initializing message header template
	 * to ensure a change to node's own network address doesn't result
	 * in template containing out-dated network address information
	 */
	spin_lock_bh(&tipc_port_list_lock);
	msg = &p_ptr->phdr;
	tipc_msg_init(msg, importance, TIPC_NAMED_MSG, NAMED_H_SIZE, 0);
	msg_set_origport(msg, ref);
>>>>>>> refs/remotes/origin/master
	list_add_tail(&p_ptr->port_list, &ports);
	spin_unlock_bh(&tipc_port_list_lock);
	return p_ptr;
}

<<<<<<< HEAD
int tipc_deleteport(u32 ref)
{
	struct tipc_port *p_ptr;
	struct sk_buff *buf = NULL;

	tipc_withdraw(ref, 0, NULL);
	p_ptr = tipc_port_lock(ref);
	if (!p_ptr)
		return -EINVAL;

	tipc_ref_discard(ref);
	tipc_port_unlock(p_ptr);
=======
int tipc_deleteport(struct tipc_port *p_ptr)
{
	struct sk_buff *buf = NULL;

	tipc_withdraw(p_ptr, 0, NULL);

	spin_lock_bh(p_ptr->lock);
	tipc_ref_discard(p_ptr->ref);
	spin_unlock_bh(p_ptr->lock);
>>>>>>> refs/remotes/origin/master

	k_cancel_timer(&p_ptr->timer);
	if (p_ptr->connected) {
		buf = port_build_peer_abort_msg(p_ptr, TIPC_ERR_NO_PORT);
		tipc_nodesub_unsubscribe(&p_ptr->subscription);
	}
<<<<<<< HEAD
	kfree(p_ptr->user_port);
=======
>>>>>>> refs/remotes/origin/master

	spin_lock_bh(&tipc_port_list_lock);
	list_del(&p_ptr->port_list);
	list_del(&p_ptr->wait_list);
	spin_unlock_bh(&tipc_port_list_lock);
	k_term_timer(&p_ptr->timer);
	kfree(p_ptr);
	tipc_net_route_msg(buf);
	return 0;
}

static int port_unreliable(struct tipc_port *p_ptr)
{
	return msg_src_droppable(&p_ptr->phdr);
}

int tipc_portunreliable(u32 ref, unsigned int *isunreliable)
{
	struct tipc_port *p_ptr;

	p_ptr = tipc_port_lock(ref);
	if (!p_ptr)
		return -EINVAL;
	*isunreliable = port_unreliable(p_ptr);
	tipc_port_unlock(p_ptr);
	return 0;
}

int tipc_set_portunreliable(u32 ref, unsigned int isunreliable)
{
	struct tipc_port *p_ptr;

	p_ptr = tipc_port_lock(ref);
	if (!p_ptr)
		return -EINVAL;
	msg_set_src_droppable(&p_ptr->phdr, (isunreliable != 0));
	tipc_port_unlock(p_ptr);
	return 0;
}

static int port_unreturnable(struct tipc_port *p_ptr)
{
	return msg_dest_droppable(&p_ptr->phdr);
}

int tipc_portunreturnable(u32 ref, unsigned int *isunrejectable)
{
	struct tipc_port *p_ptr;

	p_ptr = tipc_port_lock(ref);
	if (!p_ptr)
		return -EINVAL;
	*isunrejectable = port_unreturnable(p_ptr);
	tipc_port_unlock(p_ptr);
	return 0;
}

int tipc_set_portunreturnable(u32 ref, unsigned int isunrejectable)
{
	struct tipc_port *p_ptr;

	p_ptr = tipc_port_lock(ref);
	if (!p_ptr)
		return -EINVAL;
	msg_set_dest_droppable(&p_ptr->phdr, (isunrejectable != 0));
	tipc_port_unlock(p_ptr);
	return 0;
}

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * port_build_proto_msg(): build a port level protocol
 * or a connection abortion message. Called with
 * tipc_port lock on.
 */
static struct sk_buff *port_build_proto_msg(u32 destport, u32 destnode,
					    u32 origport, u32 orignode,
					    u32 usr, u32 type, u32 err,
					    u32 ack)
=======
=======
>>>>>>> refs/remotes/origin/master
 * port_build_proto_msg(): create connection protocol message for port
 *
 * On entry the port must be locked and connected.
 */
static struct sk_buff *port_build_proto_msg(struct tipc_port *p_ptr,
					    u32 type, u32 ack)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	struct sk_buff *buf;
	struct tipc_msg *msg;

<<<<<<< HEAD
<<<<<<< HEAD
	buf = tipc_buf_acquire(LONG_H_SIZE);
	if (buf) {
		msg = buf_msg(buf);
		tipc_msg_init(msg, usr, type, LONG_H_SIZE, destnode);
		msg_set_errcode(msg, err);
		msg_set_destport(msg, destport);
		msg_set_origport(msg, origport);
		msg_set_orignode(msg, orignode);
=======
=======
>>>>>>> refs/remotes/origin/master
	buf = tipc_buf_acquire(INT_H_SIZE);
	if (buf) {
		msg = buf_msg(buf);
		tipc_msg_init(msg, CONN_MANAGER, type, INT_H_SIZE,
			      port_peernode(p_ptr));
		msg_set_destport(msg, port_peerport(p_ptr));
		msg_set_origport(msg, p_ptr->ref);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		msg_set_msgcnt(msg, ack);
	}
	return buf;
}

int tipc_reject_msg(struct sk_buff *buf, u32 err)
{
	struct tipc_msg *msg = buf_msg(buf);
	struct sk_buff *rbuf;
	struct tipc_msg *rmsg;
	int hdr_sz;
<<<<<<< HEAD
<<<<<<< HEAD
	u32 imp = msg_importance(msg);
	u32 data_sz = msg_data_sz(msg);

	if (data_sz > MAX_REJECT_SIZE)
		data_sz = MAX_REJECT_SIZE;
	if (msg_connected(msg) && (imp < TIPC_CRITICAL_IMPORTANCE))
		imp++;

	/* discard rejected message if it shouldn't be returned to sender */
	if (msg_errcode(msg) || msg_dest_droppable(msg)) {
		buf_discard(buf);
		return data_sz;
	}

	/* construct rejected message */
	if (msg_mcast(msg))
		hdr_sz = MCAST_H_SIZE;
	else
		hdr_sz = LONG_H_SIZE;
	rbuf = tipc_buf_acquire(data_sz + hdr_sz);
	if (rbuf == NULL) {
		buf_discard(buf);
		return data_sz;
	}
	rmsg = buf_msg(rbuf);
	tipc_msg_init(rmsg, imp, msg_type(msg), hdr_sz, msg_orignode(msg));
	msg_set_errcode(rmsg, err);
	msg_set_destport(rmsg, msg_origport(msg));
	msg_set_origport(rmsg, msg_destport(msg));
	if (msg_short(msg)) {
		msg_set_orignode(rmsg, tipc_own_addr);
		/* leave name type & instance as zeroes */
	} else {
		msg_set_orignode(rmsg, msg_destnode(msg));
		msg_set_nametype(rmsg, msg_nametype(msg));
		msg_set_nameinst(rmsg, msg_nameinst(msg));
	}
	msg_set_size(rmsg, data_sz + hdr_sz);
	skb_copy_to_linear_data_offset(rbuf, hdr_sz, msg_data(msg), data_sz);

	/* send self-abort message when rejecting on a connected port */
	if (msg_connected(msg)) {
		struct sk_buff *abuf = NULL;
		struct tipc_port *p_ptr = tipc_port_lock(msg_destport(msg));

		if (p_ptr) {
			if (p_ptr->connected)
				abuf = port_build_self_abort_msg(p_ptr, err);
			tipc_port_unlock(p_ptr);
		}
		tipc_net_route_msg(abuf);
	}

	/* send rejected message */
	buf_discard(buf);
	tipc_net_route_msg(rbuf);
=======
=======
>>>>>>> refs/remotes/origin/master
	u32 imp;
	u32 data_sz = msg_data_sz(msg);
	u32 src_node;
	u32 rmsg_sz;

	/* discard rejected message if it shouldn't be returned to sender */
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
	if (WARN(!msg_isdata(msg),
		 "attempt to reject message with user=%u", msg_user(msg))) {
		dump_stack();
		goto exit;
	}
	if (msg_errcode(msg) || msg_dest_droppable(msg))
		goto exit;

	/*
	 * construct returned message by copying rejected message header and
	 * data (or subset), then updating header fields that need adjusting
	 */
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
	hdr_sz = msg_hdr_sz(msg);
	rmsg_sz = hdr_sz + min_t(u32, data_sz, MAX_REJECT_SIZE);

	rbuf = tipc_buf_acquire(rmsg_sz);
	if (rbuf == NULL)
		goto exit;

	rmsg = buf_msg(rbuf);
	skb_copy_to_linear_data(rbuf, msg, rmsg_sz);

	if (msg_connected(rmsg)) {
		imp = msg_importance(rmsg);
		if (imp < TIPC_CRITICAL_IMPORTANCE)
			msg_set_importance(rmsg, ++imp);
	}
	msg_set_non_seq(rmsg, 0);
	msg_set_size(rmsg, rmsg_sz);
	msg_set_errcode(rmsg, err);
	msg_set_prevnode(rmsg, tipc_own_addr);
	msg_swap_words(rmsg, 4, 5);
	if (!msg_short(rmsg))
		msg_swap_words(rmsg, 6, 7);

	/* send self-abort message when rejecting on a connected port */
	if (msg_connected(msg)) {
		struct tipc_port *p_ptr = tipc_port_lock(msg_destport(msg));

		if (p_ptr) {
			struct sk_buff *abuf = NULL;

			if (p_ptr->connected)
				abuf = port_build_self_abort_msg(p_ptr, err);
			tipc_port_unlock(p_ptr);
			tipc_net_route_msg(abuf);
		}
	}

	/* send returned message & dispose of rejected message */
<<<<<<< HEAD

	src_node = msg_prevnode(msg);
	if (src_node == tipc_own_addr)
=======
	src_node = msg_prevnode(msg);
	if (in_own_node(src_node))
>>>>>>> refs/remotes/origin/master
		tipc_port_recv_msg(rbuf);
	else
		tipc_link_send(rbuf, src_node, msg_link_selector(rmsg));
exit:
	kfree_skb(buf);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return data_sz;
}

int tipc_port_reject_sections(struct tipc_port *p_ptr, struct tipc_msg *hdr,
<<<<<<< HEAD
			      struct iovec const *msg_sect, u32 num_sect,
			      unsigned int total_len, int err)
=======
			      struct iovec const *msg_sect, unsigned int len,
			      int err)
>>>>>>> refs/remotes/origin/master
{
	struct sk_buff *buf;
	int res;

<<<<<<< HEAD
	res = tipc_msg_build(hdr, msg_sect, num_sect, total_len, MAX_MSG_SIZE,
			!p_ptr->user_port, &buf);
=======
	res = tipc_msg_build(hdr, msg_sect, len, MAX_MSG_SIZE, &buf);
>>>>>>> refs/remotes/origin/master
	if (!buf)
		return res;

	return tipc_reject_msg(buf, err);
}

static void port_timeout(unsigned long ref)
{
	struct tipc_port *p_ptr = tipc_port_lock(ref);
	struct sk_buff *buf = NULL;

	if (!p_ptr)
		return;

	if (!p_ptr->connected) {
		tipc_port_unlock(p_ptr);
		return;
	}

	/* Last probe answered ? */
	if (p_ptr->probing_state == PROBING) {
		buf = port_build_self_abort_msg(p_ptr, TIPC_ERR_NO_PORT);
	} else {
<<<<<<< HEAD
<<<<<<< HEAD
		buf = port_build_proto_msg(port_peerport(p_ptr),
					   port_peernode(p_ptr),
					   p_ptr->ref,
					   tipc_own_addr,
					   CONN_MANAGER,
					   CONN_PROBE,
					   TIPC_OK,
					   0);
=======
		buf = port_build_proto_msg(p_ptr, CONN_PROBE, 0);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		buf = port_build_proto_msg(p_ptr, CONN_PROBE, 0);
>>>>>>> refs/remotes/origin/master
		p_ptr->probing_state = PROBING;
		k_start_timer(&p_ptr->timer, p_ptr->probing_interval);
	}
	tipc_port_unlock(p_ptr);
	tipc_net_route_msg(buf);
}


static void port_handle_node_down(unsigned long ref)
{
	struct tipc_port *p_ptr = tipc_port_lock(ref);
	struct sk_buff *buf = NULL;

	if (!p_ptr)
		return;
	buf = port_build_self_abort_msg(p_ptr, TIPC_ERR_NO_NODE);
	tipc_port_unlock(p_ptr);
	tipc_net_route_msg(buf);
}


static struct sk_buff *port_build_self_abort_msg(struct tipc_port *p_ptr, u32 err)
{
<<<<<<< HEAD
<<<<<<< HEAD
	u32 imp = msg_importance(&p_ptr->phdr);

	if (!p_ptr->connected)
		return NULL;
	if (imp < TIPC_CRITICAL_IMPORTANCE)
		imp++;
	return port_build_proto_msg(p_ptr->ref,
				    tipc_own_addr,
				    port_peerport(p_ptr),
				    port_peernode(p_ptr),
				    imp,
				    TIPC_CONN_MSG,
				    err,
				    0);
=======
=======
>>>>>>> refs/remotes/origin/master
	struct sk_buff *buf = port_build_peer_abort_msg(p_ptr, err);

	if (buf) {
		struct tipc_msg *msg = buf_msg(buf);
		msg_swap_words(msg, 4, 5);
		msg_swap_words(msg, 6, 7);
	}
	return buf;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}


static struct sk_buff *port_build_peer_abort_msg(struct tipc_port *p_ptr, u32 err)
{
<<<<<<< HEAD
<<<<<<< HEAD
	u32 imp = msg_importance(&p_ptr->phdr);

	if (!p_ptr->connected)
		return NULL;
	if (imp < TIPC_CRITICAL_IMPORTANCE)
		imp++;
	return port_build_proto_msg(port_peerport(p_ptr),
				    port_peernode(p_ptr),
				    p_ptr->ref,
				    tipc_own_addr,
				    imp,
				    TIPC_CONN_MSG,
				    err,
				    0);
=======
=======
>>>>>>> refs/remotes/origin/master
	struct sk_buff *buf;
	struct tipc_msg *msg;
	u32 imp;

	if (!p_ptr->connected)
		return NULL;

	buf = tipc_buf_acquire(BASIC_H_SIZE);
	if (buf) {
		msg = buf_msg(buf);
		memcpy(msg, &p_ptr->phdr, BASIC_H_SIZE);
		msg_set_hdr_sz(msg, BASIC_H_SIZE);
		msg_set_size(msg, BASIC_H_SIZE);
		imp = msg_importance(msg);
		if (imp < TIPC_CRITICAL_IMPORTANCE)
			msg_set_importance(msg, ++imp);
		msg_set_errcode(msg, err);
	}
	return buf;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

void tipc_port_recv_proto_msg(struct sk_buff *buf)
{
	struct tipc_msg *msg = buf_msg(buf);
<<<<<<< HEAD
<<<<<<< HEAD
	struct tipc_port *p_ptr = tipc_port_lock(msg_destport(msg));
	u32 err = TIPC_OK;
	struct sk_buff *r_buf = NULL;
	struct sk_buff *abort_buf = NULL;

	if (!p_ptr) {
		err = TIPC_ERR_NO_PORT;
	} else if (p_ptr->connected) {
		if ((port_peernode(p_ptr) != msg_orignode(msg)) ||
		    (port_peerport(p_ptr) != msg_origport(msg))) {
			err = TIPC_ERR_NO_PORT;
		} else if (msg_type(msg) == CONN_ACK) {
			int wakeup = tipc_port_congested(p_ptr) &&
				     p_ptr->congested &&
				     p_ptr->wakeup;
			p_ptr->acked += msg_msgcnt(msg);
			if (tipc_port_congested(p_ptr))
				goto exit;
			p_ptr->congested = 0;
			if (!wakeup)
				goto exit;
			p_ptr->wakeup(p_ptr);
			goto exit;
		}
	} else if (p_ptr->published) {
		err = TIPC_ERR_NO_PORT;
	}
	if (err) {
		r_buf = port_build_proto_msg(msg_origport(msg),
					     msg_orignode(msg),
					     msg_destport(msg),
					     tipc_own_addr,
					     TIPC_HIGH_IMPORTANCE,
					     TIPC_CONN_MSG,
					     err,
					     0);
		goto exit;
	}

	/* All is fine */
	if (msg_type(msg) == CONN_PROBE) {
		r_buf = port_build_proto_msg(msg_origport(msg),
					     msg_orignode(msg),
					     msg_destport(msg),
					     tipc_own_addr,
					     CONN_MANAGER,
					     CONN_PROBE_REPLY,
					     TIPC_OK,
					     0);
	}
	p_ptr->probing_state = CONFIRMED;
exit:
	if (p_ptr)
		tipc_port_unlock(p_ptr);
	tipc_net_route_msg(r_buf);
	tipc_net_route_msg(abort_buf);
	buf_discard(buf);
=======
	struct tipc_port *p_ptr;
	struct sk_buff *r_buf = NULL;
	u32 orignode = msg_orignode(msg);
	u32 origport = msg_origport(msg);
=======
	struct tipc_port *p_ptr;
	struct sk_buff *r_buf = NULL;
>>>>>>> refs/remotes/origin/master
	u32 destport = msg_destport(msg);
	int wakeable;

	/* Validate connection */
<<<<<<< HEAD

	p_ptr = tipc_port_lock(destport);
	if (!p_ptr || !p_ptr->connected ||
	    (port_peernode(p_ptr) != orignode) ||
	    (port_peerport(p_ptr) != origport)) {
=======
	p_ptr = tipc_port_lock(destport);
	if (!p_ptr || !p_ptr->connected || !tipc_port_peer_msg(p_ptr, msg)) {
>>>>>>> refs/remotes/origin/master
		r_buf = tipc_buf_acquire(BASIC_H_SIZE);
		if (r_buf) {
			msg = buf_msg(r_buf);
			tipc_msg_init(msg, TIPC_HIGH_IMPORTANCE, TIPC_CONN_MSG,
<<<<<<< HEAD
				      BASIC_H_SIZE, orignode);
			msg_set_errcode(msg, TIPC_ERR_NO_PORT);
			msg_set_origport(msg, destport);
			msg_set_destport(msg, origport);
=======
				      BASIC_H_SIZE, msg_orignode(msg));
			msg_set_errcode(msg, TIPC_ERR_NO_PORT);
			msg_set_origport(msg, destport);
			msg_set_destport(msg, msg_origport(msg));
>>>>>>> refs/remotes/origin/master
		}
		if (p_ptr)
			tipc_port_unlock(p_ptr);
		goto exit;
	}

	/* Process protocol message sent by peer */
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
	switch (msg_type(msg)) {
	case CONN_ACK:
		wakeable = tipc_port_congested(p_ptr) && p_ptr->congested &&
			p_ptr->wakeup;
		p_ptr->acked += msg_msgcnt(msg);
		if (!tipc_port_congested(p_ptr)) {
			p_ptr->congested = 0;
			if (wakeable)
				p_ptr->wakeup(p_ptr);
		}
		break;
	case CONN_PROBE:
		r_buf = port_build_proto_msg(p_ptr, CONN_PROBE_REPLY, 0);
		break;
	default:
		/* CONN_PROBE_REPLY or unrecognized - no action required */
		break;
	}
	p_ptr->probing_state = CONFIRMED;
	tipc_port_unlock(p_ptr);
exit:
	tipc_net_route_msg(r_buf);
	kfree_skb(buf);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

static void port_print(struct tipc_port *p_ptr, struct print_buf *buf, int full_id)
{
	struct publication *publ;

	if (full_id)
		tipc_printf(buf, "<%u.%u.%u:%u>:",
			    tipc_zone(tipc_own_addr), tipc_cluster(tipc_own_addr),
			    tipc_node(tipc_own_addr), p_ptr->ref);
	else
		tipc_printf(buf, "%-10u:", p_ptr->ref);
=======
}

static int port_print(struct tipc_port *p_ptr, char *buf, int len, int full_id)
{
	struct publication *publ;
	int ret;

	if (full_id)
		ret = tipc_snprintf(buf, len, "<%u.%u.%u:%u>:",
				    tipc_zone(tipc_own_addr),
				    tipc_cluster(tipc_own_addr),
				    tipc_node(tipc_own_addr), p_ptr->ref);
	else
		ret = tipc_snprintf(buf, len, "%-10u:", p_ptr->ref);
>>>>>>> refs/remotes/origin/master

	if (p_ptr->connected) {
		u32 dport = port_peerport(p_ptr);
		u32 destnode = port_peernode(p_ptr);

<<<<<<< HEAD
		tipc_printf(buf, " connected to <%u.%u.%u:%u>",
			    tipc_zone(destnode), tipc_cluster(destnode),
			    tipc_node(destnode), dport);
		if (p_ptr->conn_type != 0)
			tipc_printf(buf, " via {%u,%u}",
				    p_ptr->conn_type,
				    p_ptr->conn_instance);
	} else if (p_ptr->published) {
		tipc_printf(buf, " bound to");
		list_for_each_entry(publ, &p_ptr->publications, pport_list) {
			if (publ->lower == publ->upper)
				tipc_printf(buf, " {%u,%u}", publ->type,
					    publ->lower);
			else
				tipc_printf(buf, " {%u,%u,%u}", publ->type,
					    publ->lower, publ->upper);
		}
	}
	tipc_printf(buf, "\n");
}

#define MAX_PORT_QUERY 32768

=======
		ret += tipc_snprintf(buf + ret, len - ret,
				     " connected to <%u.%u.%u:%u>",
				     tipc_zone(destnode),
				     tipc_cluster(destnode),
				     tipc_node(destnode), dport);
		if (p_ptr->conn_type != 0)
			ret += tipc_snprintf(buf + ret, len - ret,
					     " via {%u,%u}", p_ptr->conn_type,
					     p_ptr->conn_instance);
	} else if (p_ptr->published) {
		ret += tipc_snprintf(buf + ret, len - ret, " bound to");
		list_for_each_entry(publ, &p_ptr->publications, pport_list) {
			if (publ->lower == publ->upper)
				ret += tipc_snprintf(buf + ret, len - ret,
						     " {%u,%u}", publ->type,
						     publ->lower);
			else
				ret += tipc_snprintf(buf + ret, len - ret,
						     " {%u,%u,%u}", publ->type,
						     publ->lower, publ->upper);
		}
	}
	ret += tipc_snprintf(buf + ret, len - ret, "\n");
	return ret;
}

>>>>>>> refs/remotes/origin/master
struct sk_buff *tipc_port_get_ports(void)
{
	struct sk_buff *buf;
	struct tlv_desc *rep_tlv;
<<<<<<< HEAD
	struct print_buf pb;
	struct tipc_port *p_ptr;
	int str_len;

	buf = tipc_cfg_reply_alloc(TLV_SPACE(MAX_PORT_QUERY));
	if (!buf)
		return NULL;
	rep_tlv = (struct tlv_desc *)buf->data;

	tipc_printbuf_init(&pb, TLV_DATA(rep_tlv), MAX_PORT_QUERY);
	spin_lock_bh(&tipc_port_list_lock);
	list_for_each_entry(p_ptr, &ports, port_list) {
		spin_lock_bh(p_ptr->lock);
		port_print(p_ptr, &pb, 0);
		spin_unlock_bh(p_ptr->lock);
	}
	spin_unlock_bh(&tipc_port_list_lock);
	str_len = tipc_printbuf_validate(&pb);

=======
	char *pb;
	int pb_len;
	struct tipc_port *p_ptr;
	int str_len = 0;

	buf = tipc_cfg_reply_alloc(TLV_SPACE(ULTRA_STRING_MAX_LEN));
	if (!buf)
		return NULL;
	rep_tlv = (struct tlv_desc *)buf->data;
	pb = TLV_DATA(rep_tlv);
	pb_len = ULTRA_STRING_MAX_LEN;

	spin_lock_bh(&tipc_port_list_lock);
	list_for_each_entry(p_ptr, &ports, port_list) {
		spin_lock_bh(p_ptr->lock);
		str_len += port_print(p_ptr, pb, pb_len, 0);
		spin_unlock_bh(p_ptr->lock);
	}
	spin_unlock_bh(&tipc_port_list_lock);
	str_len += 1;	/* for "\0" */
>>>>>>> refs/remotes/origin/master
	skb_put(buf, TLV_SPACE(str_len));
	TLV_SET(rep_tlv, TIPC_TLV_ULTRA_STRING, NULL, str_len);

	return buf;
}

void tipc_port_reinit(void)
{
	struct tipc_port *p_ptr;
	struct tipc_msg *msg;

	spin_lock_bh(&tipc_port_list_lock);
	list_for_each_entry(p_ptr, &ports, port_list) {
		msg = &p_ptr->phdr;
<<<<<<< HEAD
		if (msg_orignode(msg) == tipc_own_addr)
			break;
=======
>>>>>>> refs/remotes/origin/master
		msg_set_prevnode(msg, tipc_own_addr);
		msg_set_orignode(msg, tipc_own_addr);
	}
	spin_unlock_bh(&tipc_port_list_lock);
}

<<<<<<< HEAD

/*
 *  port_dispatcher_sigh(): Signal handler for messages destinated
 *                          to the tipc_port interface.
 */

static void port_dispatcher_sigh(void *dummy)
{
	struct sk_buff *buf;

	spin_lock_bh(&queue_lock);
	buf = msg_queue_head;
	msg_queue_head = NULL;
	spin_unlock_bh(&queue_lock);

	while (buf) {
		struct tipc_port *p_ptr;
		struct user_port *up_ptr;
		struct tipc_portid orig;
		struct tipc_name_seq dseq;
		void *usr_handle;
		int connected;
		int published;
		u32 message_type;

		struct sk_buff *next = buf->next;
		struct tipc_msg *msg = buf_msg(buf);
		u32 dref = msg_destport(msg);

		message_type = msg_type(msg);
		if (message_type > TIPC_DIRECT_MSG)
			goto reject;	/* Unsupported message type */

		p_ptr = tipc_port_lock(dref);
		if (!p_ptr)
			goto reject;	/* Port deleted while msg in queue */

		orig.ref = msg_origport(msg);
		orig.node = msg_orignode(msg);
		up_ptr = p_ptr->user_port;
		usr_handle = up_ptr->usr_handle;
		connected = p_ptr->connected;
		published = p_ptr->published;

		if (unlikely(msg_errcode(msg)))
			goto err;

		switch (message_type) {

		case TIPC_CONN_MSG:{
				tipc_conn_msg_event cb = up_ptr->conn_msg_cb;
				u32 peer_port = port_peerport(p_ptr);
				u32 peer_node = port_peernode(p_ptr);
				u32 dsz;

				tipc_port_unlock(p_ptr);
				if (unlikely(!cb))
					goto reject;
				if (unlikely(!connected)) {
					if (tipc_connect2port(dref, &orig))
						goto reject;
				} else if ((msg_origport(msg) != peer_port) ||
					   (msg_orignode(msg) != peer_node))
					goto reject;
				dsz = msg_data_sz(msg);
				if (unlikely(dsz &&
					     (++p_ptr->conn_unacked >=
					      TIPC_FLOW_CONTROL_WIN)))
					tipc_acknowledge(dref,
							 p_ptr->conn_unacked);
				skb_pull(buf, msg_hdr_sz(msg));
				cb(usr_handle, dref, &buf, msg_data(msg), dsz);
				break;
			}
		case TIPC_DIRECT_MSG:{
				tipc_msg_event cb = up_ptr->msg_cb;

				tipc_port_unlock(p_ptr);
				if (unlikely(!cb || connected))
					goto reject;
				skb_pull(buf, msg_hdr_sz(msg));
				cb(usr_handle, dref, &buf, msg_data(msg),
				   msg_data_sz(msg), msg_importance(msg),
				   &orig);
				break;
			}
		case TIPC_MCAST_MSG:
		case TIPC_NAMED_MSG:{
				tipc_named_msg_event cb = up_ptr->named_msg_cb;

				tipc_port_unlock(p_ptr);
				if (unlikely(!cb || connected || !published))
					goto reject;
				dseq.type =  msg_nametype(msg);
				dseq.lower = msg_nameinst(msg);
				dseq.upper = (message_type == TIPC_NAMED_MSG)
					? dseq.lower : msg_nameupper(msg);
				skb_pull(buf, msg_hdr_sz(msg));
				cb(usr_handle, dref, &buf, msg_data(msg),
				   msg_data_sz(msg), msg_importance(msg),
				   &orig, &dseq);
				break;
			}
		}
		if (buf)
<<<<<<< HEAD
			buf_discard(buf);
=======
			kfree_skb(buf);
>>>>>>> refs/remotes/origin/cm-10.0
		buf = next;
		continue;
err:
		switch (message_type) {

		case TIPC_CONN_MSG:{
				tipc_conn_shutdown_event cb =
					up_ptr->conn_err_cb;
				u32 peer_port = port_peerport(p_ptr);
				u32 peer_node = port_peernode(p_ptr);

				tipc_port_unlock(p_ptr);
				if (!cb || !connected)
					break;
				if ((msg_origport(msg) != peer_port) ||
				    (msg_orignode(msg) != peer_node))
					break;
				tipc_disconnect(dref);
				skb_pull(buf, msg_hdr_sz(msg));
				cb(usr_handle, dref, &buf, msg_data(msg),
				   msg_data_sz(msg), msg_errcode(msg));
				break;
			}
		case TIPC_DIRECT_MSG:{
				tipc_msg_err_event cb = up_ptr->err_cb;

				tipc_port_unlock(p_ptr);
				if (!cb || connected)
					break;
				skb_pull(buf, msg_hdr_sz(msg));
				cb(usr_handle, dref, &buf, msg_data(msg),
				   msg_data_sz(msg), msg_errcode(msg), &orig);
				break;
			}
		case TIPC_MCAST_MSG:
		case TIPC_NAMED_MSG:{
				tipc_named_msg_err_event cb =
					up_ptr->named_err_cb;

				tipc_port_unlock(p_ptr);
				if (!cb || connected)
					break;
				dseq.type =  msg_nametype(msg);
				dseq.lower = msg_nameinst(msg);
				dseq.upper = (message_type == TIPC_NAMED_MSG)
					? dseq.lower : msg_nameupper(msg);
				skb_pull(buf, msg_hdr_sz(msg));
				cb(usr_handle, dref, &buf, msg_data(msg),
				   msg_data_sz(msg), msg_errcode(msg), &dseq);
				break;
			}
		}
		if (buf)
<<<<<<< HEAD
			buf_discard(buf);
=======
			kfree_skb(buf);
>>>>>>> refs/remotes/origin/cm-10.0
		buf = next;
		continue;
reject:
		tipc_reject_msg(buf, TIPC_ERR_NO_PORT);
		buf = next;
	}
}

/*
 *  port_dispatcher(): Dispatcher for messages destinated
 *  to the tipc_port interface. Called with port locked.
 */

static u32 port_dispatcher(struct tipc_port *dummy, struct sk_buff *buf)
{
	buf->next = NULL;
	spin_lock_bh(&queue_lock);
	if (msg_queue_head) {
		msg_queue_tail->next = buf;
		msg_queue_tail = buf;
	} else {
		msg_queue_tail = msg_queue_head = buf;
		tipc_k_signal((Handler)port_dispatcher_sigh, 0);
	}
	spin_unlock_bh(&queue_lock);
	return 0;
}

/*
 * Wake up port after congestion: Called with port locked,
 *
 */

static void port_wakeup_sh(unsigned long ref)
{
	struct tipc_port *p_ptr;
	struct user_port *up_ptr;
	tipc_continue_event cb = NULL;
	void *uh = NULL;

	p_ptr = tipc_port_lock(ref);
	if (p_ptr) {
		up_ptr = p_ptr->user_port;
		if (up_ptr) {
			cb = up_ptr->continue_event_cb;
			uh = up_ptr->usr_handle;
		}
		tipc_port_unlock(p_ptr);
	}
	if (cb)
		cb(uh, ref);
}


static void port_wakeup(struct tipc_port *p_ptr)
{
	tipc_k_signal((Handler)port_wakeup_sh, p_ptr->ref);
}

=======
>>>>>>> refs/remotes/origin/master
void tipc_acknowledge(u32 ref, u32 ack)
{
	struct tipc_port *p_ptr;
	struct sk_buff *buf = NULL;

	p_ptr = tipc_port_lock(ref);
	if (!p_ptr)
		return;
	if (p_ptr->connected) {
		p_ptr->conn_unacked -= ack;
<<<<<<< HEAD
<<<<<<< HEAD
		buf = port_build_proto_msg(port_peerport(p_ptr),
					   port_peernode(p_ptr),
					   ref,
					   tipc_own_addr,
					   CONN_MANAGER,
					   CONN_ACK,
					   TIPC_OK,
					   ack);
=======
		buf = port_build_proto_msg(p_ptr, CONN_ACK, ack);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		buf = port_build_proto_msg(p_ptr, CONN_ACK, ack);
>>>>>>> refs/remotes/origin/master
	}
	tipc_port_unlock(p_ptr);
	tipc_net_route_msg(buf);
}

<<<<<<< HEAD
/*
 * tipc_createport(): user level call.
 */

int tipc_createport(void *usr_handle,
		    unsigned int importance,
		    tipc_msg_err_event error_cb,
		    tipc_named_msg_err_event named_error_cb,
		    tipc_conn_shutdown_event conn_error_cb,
		    tipc_msg_event msg_cb,
		    tipc_named_msg_event named_msg_cb,
		    tipc_conn_msg_event conn_msg_cb,
		    tipc_continue_event continue_event_cb,/* May be zero */
		    u32 *portref)
{
	struct user_port *up_ptr;
	struct tipc_port *p_ptr;

	up_ptr = kmalloc(sizeof(*up_ptr), GFP_ATOMIC);
	if (!up_ptr) {
		warn("Port creation failed, no memory\n");
		return -ENOMEM;
	}
	p_ptr = (struct tipc_port *)tipc_createport_raw(NULL, port_dispatcher,
						   port_wakeup, importance);
	if (!p_ptr) {
		kfree(up_ptr);
		return -ENOMEM;
	}

	p_ptr->user_port = up_ptr;
	up_ptr->usr_handle = usr_handle;
	up_ptr->ref = p_ptr->ref;
	up_ptr->err_cb = error_cb;
	up_ptr->named_err_cb = named_error_cb;
	up_ptr->conn_err_cb = conn_error_cb;
	up_ptr->msg_cb = msg_cb;
	up_ptr->named_msg_cb = named_msg_cb;
	up_ptr->conn_msg_cb = conn_msg_cb;
	up_ptr->continue_event_cb = continue_event_cb;
	*portref = p_ptr->ref;
	tipc_port_unlock(p_ptr);
	return 0;
}

=======
>>>>>>> refs/remotes/origin/master
int tipc_portimportance(u32 ref, unsigned int *importance)
{
	struct tipc_port *p_ptr;

	p_ptr = tipc_port_lock(ref);
	if (!p_ptr)
		return -EINVAL;
	*importance = (unsigned int)msg_importance(&p_ptr->phdr);
	tipc_port_unlock(p_ptr);
	return 0;
}

int tipc_set_portimportance(u32 ref, unsigned int imp)
{
	struct tipc_port *p_ptr;

	if (imp > TIPC_CRITICAL_IMPORTANCE)
		return -EINVAL;

	p_ptr = tipc_port_lock(ref);
	if (!p_ptr)
		return -EINVAL;
	msg_set_importance(&p_ptr->phdr, (u32)imp);
	tipc_port_unlock(p_ptr);
	return 0;
}


<<<<<<< HEAD
int tipc_publish(u32 ref, unsigned int scope, struct tipc_name_seq const *seq)
{
	struct tipc_port *p_ptr;
	struct publication *publ;
	u32 key;
	int res = -EINVAL;

	p_ptr = tipc_port_lock(ref);
	if (!p_ptr)
		return -EINVAL;

	if (p_ptr->connected)
		goto exit;
	if (seq->lower > seq->upper)
		goto exit;
	if ((scope < TIPC_ZONE_SCOPE) || (scope > TIPC_NODE_SCOPE))
		goto exit;
	key = ref + p_ptr->pub_count + 1;
	if (key == ref) {
		res = -EADDRINUSE;
		goto exit;
	}
=======
int tipc_publish(struct tipc_port *p_ptr, unsigned int scope,
		 struct tipc_name_seq const *seq)
{
	struct publication *publ;
	u32 key;

	if (p_ptr->connected)
		return -EINVAL;
	key = p_ptr->ref + p_ptr->pub_count + 1;
	if (key == p_ptr->ref)
		return -EADDRINUSE;

>>>>>>> refs/remotes/origin/master
	publ = tipc_nametbl_publish(seq->type, seq->lower, seq->upper,
				    scope, p_ptr->ref, key);
	if (publ) {
		list_add(&publ->pport_list, &p_ptr->publications);
		p_ptr->pub_count++;
		p_ptr->published = 1;
<<<<<<< HEAD
		res = 0;
	}
exit:
	tipc_port_unlock(p_ptr);
	return res;
}

int tipc_withdraw(u32 ref, unsigned int scope, struct tipc_name_seq const *seq)
{
	struct tipc_port *p_ptr;
=======
		return 0;
	}
	return -EINVAL;
}

int tipc_withdraw(struct tipc_port *p_ptr, unsigned int scope,
		  struct tipc_name_seq const *seq)
{
>>>>>>> refs/remotes/origin/master
	struct publication *publ;
	struct publication *tpubl;
	int res = -EINVAL;

<<<<<<< HEAD
	p_ptr = tipc_port_lock(ref);
	if (!p_ptr)
		return -EINVAL;
=======
>>>>>>> refs/remotes/origin/master
	if (!seq) {
		list_for_each_entry_safe(publ, tpubl,
					 &p_ptr->publications, pport_list) {
			tipc_nametbl_withdraw(publ->type, publ->lower,
					      publ->ref, publ->key);
		}
		res = 0;
	} else {
		list_for_each_entry_safe(publ, tpubl,
					 &p_ptr->publications, pport_list) {
			if (publ->scope != scope)
				continue;
			if (publ->type != seq->type)
				continue;
			if (publ->lower != seq->lower)
				continue;
			if (publ->upper != seq->upper)
				break;
			tipc_nametbl_withdraw(publ->type, publ->lower,
					      publ->ref, publ->key);
			res = 0;
			break;
		}
	}
	if (list_empty(&p_ptr->publications))
		p_ptr->published = 0;
<<<<<<< HEAD
	tipc_port_unlock(p_ptr);
	return res;
}

int tipc_connect2port(u32 ref, struct tipc_portid const *peer)
{
	struct tipc_port *p_ptr;
	struct tipc_msg *msg;
	int res = -EINVAL;
=======
	return res;
}

int tipc_connect(u32 ref, struct tipc_portid const *peer)
{
	struct tipc_port *p_ptr;
	int res;
>>>>>>> refs/remotes/origin/master

	p_ptr = tipc_port_lock(ref);
	if (!p_ptr)
		return -EINVAL;
<<<<<<< HEAD
=======
	res = __tipc_connect(ref, p_ptr, peer);
	tipc_port_unlock(p_ptr);
	return res;
}

/*
 * __tipc_connect - connect to a remote peer
 *
 * Port must be locked.
 */
int __tipc_connect(u32 ref, struct tipc_port *p_ptr,
			struct tipc_portid const *peer)
{
	struct tipc_msg *msg;
	int res = -EINVAL;

>>>>>>> refs/remotes/origin/master
	if (p_ptr->published || p_ptr->connected)
		goto exit;
	if (!peer->ref)
		goto exit;

	msg = &p_ptr->phdr;
	msg_set_destnode(msg, peer->node);
	msg_set_destport(msg, peer->ref);
<<<<<<< HEAD
<<<<<<< HEAD
	msg_set_orignode(msg, tipc_own_addr);
	msg_set_origport(msg, p_ptr->ref);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	msg_set_type(msg, TIPC_CONN_MSG);
	msg_set_lookup_scope(msg, 0);
	msg_set_hdr_sz(msg, SHORT_H_SIZE);

	p_ptr->probing_interval = PROBING_INTERVAL;
	p_ptr->probing_state = CONFIRMED;
	p_ptr->connected = 1;
	k_start_timer(&p_ptr->timer, p_ptr->probing_interval);

	tipc_nodesub_subscribe(&p_ptr->subscription, peer->node,
			  (void *)(unsigned long)ref,
			  (net_ev_handler)port_handle_node_down);
	res = 0;
exit:
<<<<<<< HEAD
	tipc_port_unlock(p_ptr);
=======
>>>>>>> refs/remotes/origin/master
	p_ptr->max_pkt = tipc_link_get_max_pkt(peer->node, ref);
	return res;
}

<<<<<<< HEAD
/**
 * tipc_disconnect_port - disconnect port from peer
 *
 * Port must be locked.
 */

int tipc_disconnect_port(struct tipc_port *tp_ptr)
=======
/*
 * __tipc_disconnect - disconnect port from peer
 *
 * Port must be locked.
 */
int __tipc_disconnect(struct tipc_port *tp_ptr)
>>>>>>> refs/remotes/origin/master
{
	int res;

	if (tp_ptr->connected) {
		tp_ptr->connected = 0;
		/* let timer expire on it's own to avoid deadlock! */
<<<<<<< HEAD
		tipc_nodesub_unsubscribe(
			&((struct tipc_port *)tp_ptr)->subscription);
=======
		tipc_nodesub_unsubscribe(&tp_ptr->subscription);
>>>>>>> refs/remotes/origin/master
		res = 0;
	} else {
		res = -ENOTCONN;
	}
	return res;
}

/*
 * tipc_disconnect(): Disconnect port form peer.
 *                    This is a node local operation.
 */
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
int tipc_disconnect(u32 ref)
{
	struct tipc_port *p_ptr;
	int res;

	p_ptr = tipc_port_lock(ref);
	if (!p_ptr)
		return -EINVAL;
<<<<<<< HEAD
	res = tipc_disconnect_port((struct tipc_port *)p_ptr);
=======
	res = __tipc_disconnect(p_ptr);
>>>>>>> refs/remotes/origin/master
	tipc_port_unlock(p_ptr);
	return res;
}

/*
 * tipc_shutdown(): Send a SHUTDOWN msg to peer and disconnect
 */
int tipc_shutdown(u32 ref)
{
	struct tipc_port *p_ptr;
	struct sk_buff *buf = NULL;

	p_ptr = tipc_port_lock(ref);
	if (!p_ptr)
		return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
	if (p_ptr->connected) {
		u32 imp = msg_importance(&p_ptr->phdr);
		if (imp < TIPC_CRITICAL_IMPORTANCE)
			imp++;
		buf = port_build_proto_msg(port_peerport(p_ptr),
					   port_peernode(p_ptr),
					   ref,
					   tipc_own_addr,
					   imp,
					   TIPC_CONN_MSG,
					   TIPC_CONN_SHUTDOWN,
					   0);
	}
=======
	buf = port_build_peer_abort_msg(p_ptr, TIPC_CONN_SHUTDOWN);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	buf = port_build_peer_abort_msg(p_ptr, TIPC_CONN_SHUTDOWN);
>>>>>>> refs/remotes/origin/master
	tipc_port_unlock(p_ptr);
	tipc_net_route_msg(buf);
	return tipc_disconnect(ref);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
/**
 * tipc_port_recv_msg - receive message from lower layer and deliver to port user
 */

=======
/**
 * tipc_port_recv_msg - receive message from lower layer and deliver to port user
 */
>>>>>>> refs/remotes/origin/master
int tipc_port_recv_msg(struct sk_buff *buf)
{
	struct tipc_port *p_ptr;
	struct tipc_msg *msg = buf_msg(buf);
	u32 destport = msg_destport(msg);
	u32 dsz = msg_data_sz(msg);
	u32 err;

	/* forward unresolved named message */
	if (unlikely(!destport)) {
		tipc_net_route_msg(buf);
		return dsz;
	}

	/* validate destination & pass to port, otherwise reject message */
	p_ptr = tipc_port_lock(destport);
	if (likely(p_ptr)) {
<<<<<<< HEAD
		if (likely(p_ptr->connected)) {
			if ((unlikely(msg_origport(msg) !=
				      tipc_peer_port(p_ptr))) ||
			    (unlikely(msg_orignode(msg) !=
				      tipc_peer_node(p_ptr))) ||
			    (unlikely(!msg_connected(msg)))) {
				err = TIPC_ERR_NO_PORT;
				tipc_port_unlock(p_ptr);
				goto reject;
			}
		}
=======
>>>>>>> refs/remotes/origin/master
		err = p_ptr->dispatcher(p_ptr, buf);
		tipc_port_unlock(p_ptr);
		if (likely(!err))
			return dsz;
	} else {
		err = TIPC_ERR_NO_PORT;
	}
<<<<<<< HEAD
reject:
	return tipc_reject_msg(buf, err);
}

>>>>>>> refs/remotes/origin/cm-10.0
=======

	return tipc_reject_msg(buf, err);
}

>>>>>>> refs/remotes/origin/master
/*
 *  tipc_port_recv_sections(): Concatenate and deliver sectioned
 *                        message for this node.
 */
<<<<<<< HEAD

static int tipc_port_recv_sections(struct tipc_port *sender, unsigned int num_sect,
				   struct iovec const *msg_sect,
				   unsigned int total_len)
=======
static int tipc_port_recv_sections(struct tipc_port *sender,
				   struct iovec const *msg_sect,
				   unsigned int len)
>>>>>>> refs/remotes/origin/master
{
	struct sk_buff *buf;
	int res;

<<<<<<< HEAD
	res = tipc_msg_build(&sender->phdr, msg_sect, num_sect, total_len,
			MAX_MSG_SIZE, !sender->user_port, &buf);
=======
	res = tipc_msg_build(&sender->phdr, msg_sect, len, MAX_MSG_SIZE, &buf);
>>>>>>> refs/remotes/origin/master
	if (likely(buf))
		tipc_port_recv_msg(buf);
	return res;
}

/**
 * tipc_send - send message sections on connection
 */
<<<<<<< HEAD

int tipc_send(u32 ref, unsigned int num_sect, struct iovec const *msg_sect,
	      unsigned int total_len)
=======
int tipc_send(u32 ref, struct iovec const *msg_sect, unsigned int len)
>>>>>>> refs/remotes/origin/master
{
	struct tipc_port *p_ptr;
	u32 destnode;
	int res;

	p_ptr = tipc_port_deref(ref);
	if (!p_ptr || !p_ptr->connected)
		return -EINVAL;

	p_ptr->congested = 1;
	if (!tipc_port_congested(p_ptr)) {
		destnode = port_peernode(p_ptr);
<<<<<<< HEAD
		if (likely(destnode != tipc_own_addr))
			res = tipc_link_send_sections_fast(p_ptr, msg_sect, num_sect,
							   total_len, destnode);
		else
			res = tipc_port_recv_sections(p_ptr, num_sect, msg_sect,
						      total_len);
=======
		if (likely(!in_own_node(destnode)))
			res = tipc_link_send_sections_fast(p_ptr, msg_sect,
							   len, destnode);
		else
			res = tipc_port_recv_sections(p_ptr, msg_sect, len);
>>>>>>> refs/remotes/origin/master

		if (likely(res != -ELINKCONG)) {
			p_ptr->congested = 0;
			if (res > 0)
				p_ptr->sent++;
			return res;
		}
	}
	if (port_unreliable(p_ptr)) {
		p_ptr->congested = 0;
<<<<<<< HEAD
		return total_len;
=======
		return len;
>>>>>>> refs/remotes/origin/master
	}
	return -ELINKCONG;
}

/**
 * tipc_send2name - send message sections to port name
 */
<<<<<<< HEAD

int tipc_send2name(u32 ref, struct tipc_name const *name, unsigned int domain,
		   unsigned int num_sect, struct iovec const *msg_sect,
		   unsigned int total_len)
=======
int tipc_send2name(u32 ref, struct tipc_name const *name, unsigned int domain,
		   struct iovec const *msg_sect, unsigned int len)
>>>>>>> refs/remotes/origin/master
{
	struct tipc_port *p_ptr;
	struct tipc_msg *msg;
	u32 destnode = domain;
	u32 destport;
	int res;

	p_ptr = tipc_port_deref(ref);
	if (!p_ptr || p_ptr->connected)
		return -EINVAL;

	msg = &p_ptr->phdr;
	msg_set_type(msg, TIPC_NAMED_MSG);
<<<<<<< HEAD
<<<<<<< HEAD
	msg_set_orignode(msg, tipc_own_addr);
	msg_set_origport(msg, ref);
	msg_set_hdr_sz(msg, LONG_H_SIZE);
=======
	msg_set_hdr_sz(msg, NAMED_H_SIZE);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	msg_set_hdr_sz(msg, NAMED_H_SIZE);
>>>>>>> refs/remotes/origin/master
	msg_set_nametype(msg, name->type);
	msg_set_nameinst(msg, name->instance);
	msg_set_lookup_scope(msg, tipc_addr_scope(domain));
	destport = tipc_nametbl_translate(name->type, name->instance, &destnode);
	msg_set_destnode(msg, destnode);
	msg_set_destport(msg, destport);

<<<<<<< HEAD
<<<<<<< HEAD
	if (likely(destport)) {
=======
	if (likely(destport || destnode)) {
>>>>>>> refs/remotes/origin/cm-10.0
		if (likely(destnode == tipc_own_addr))
			res = tipc_port_recv_sections(p_ptr, num_sect,
						      msg_sect, total_len);
		else
			res = tipc_link_send_sections_fast(p_ptr, msg_sect,
							   num_sect, total_len,
							   destnode);
=======
	if (likely(destport || destnode)) {
		if (likely(in_own_node(destnode)))
			res = tipc_port_recv_sections(p_ptr, msg_sect, len);
		else if (tipc_own_addr)
			res = tipc_link_send_sections_fast(p_ptr, msg_sect,
							   len, destnode);
		else
			res = tipc_port_reject_sections(p_ptr, msg, msg_sect,
							len, TIPC_ERR_NO_NODE);
>>>>>>> refs/remotes/origin/master
		if (likely(res != -ELINKCONG)) {
			if (res > 0)
				p_ptr->sent++;
			return res;
		}
		if (port_unreliable(p_ptr)) {
<<<<<<< HEAD
			return total_len;
		}
		return -ELINKCONG;
	}
	return tipc_port_reject_sections(p_ptr, msg, msg_sect, num_sect,
					 total_len, TIPC_ERR_NO_NAME);
=======
			return len;
		}
		return -ELINKCONG;
	}
	return tipc_port_reject_sections(p_ptr, msg, msg_sect, len,
					 TIPC_ERR_NO_NAME);
>>>>>>> refs/remotes/origin/master
}

/**
 * tipc_send2port - send message sections to port identity
 */
<<<<<<< HEAD

int tipc_send2port(u32 ref, struct tipc_portid const *dest,
		   unsigned int num_sect, struct iovec const *msg_sect,
		   unsigned int total_len)
=======
int tipc_send2port(u32 ref, struct tipc_portid const *dest,
		   struct iovec const *msg_sect, unsigned int len)
>>>>>>> refs/remotes/origin/master
{
	struct tipc_port *p_ptr;
	struct tipc_msg *msg;
	int res;

	p_ptr = tipc_port_deref(ref);
	if (!p_ptr || p_ptr->connected)
		return -EINVAL;

	msg = &p_ptr->phdr;
	msg_set_type(msg, TIPC_DIRECT_MSG);
	msg_set_lookup_scope(msg, 0);
<<<<<<< HEAD
<<<<<<< HEAD
	msg_set_orignode(msg, tipc_own_addr);
	msg_set_origport(msg, ref);
	msg_set_destnode(msg, dest->node);
	msg_set_destport(msg, dest->ref);
	msg_set_hdr_sz(msg, DIR_MSG_H_SIZE);
=======
	msg_set_destnode(msg, dest->node);
	msg_set_destport(msg, dest->ref);
	msg_set_hdr_sz(msg, BASIC_H_SIZE);
>>>>>>> refs/remotes/origin/cm-10.0

	if (dest->node == tipc_own_addr)
		res =  tipc_port_recv_sections(p_ptr, num_sect, msg_sect,
					       total_len);
	else
		res = tipc_link_send_sections_fast(p_ptr, msg_sect, num_sect,
						   total_len, dest->node);
=======
	msg_set_destnode(msg, dest->node);
	msg_set_destport(msg, dest->ref);
	msg_set_hdr_sz(msg, BASIC_H_SIZE);

	if (in_own_node(dest->node))
		res =  tipc_port_recv_sections(p_ptr, msg_sect, len);
	else if (tipc_own_addr)
		res = tipc_link_send_sections_fast(p_ptr, msg_sect, len,
						   dest->node);
	else
		res = tipc_port_reject_sections(p_ptr, msg, msg_sect, len,
						TIPC_ERR_NO_NODE);
>>>>>>> refs/remotes/origin/master
	if (likely(res != -ELINKCONG)) {
		if (res > 0)
			p_ptr->sent++;
		return res;
	}
	if (port_unreliable(p_ptr)) {
<<<<<<< HEAD
		return total_len;
	}
	return -ELINKCONG;
}

/**
 * tipc_send_buf2port - send message buffer to port identity
 */

int tipc_send_buf2port(u32 ref, struct tipc_portid const *dest,
	       struct sk_buff *buf, unsigned int dsz)
{
	struct tipc_port *p_ptr;
	struct tipc_msg *msg;
	int res;

	p_ptr = (struct tipc_port *)tipc_ref_deref(ref);
	if (!p_ptr || p_ptr->connected)
		return -EINVAL;

	msg = &p_ptr->phdr;
	msg_set_type(msg, TIPC_DIRECT_MSG);
<<<<<<< HEAD
	msg_set_orignode(msg, tipc_own_addr);
	msg_set_origport(msg, ref);
	msg_set_destnode(msg, dest->node);
	msg_set_destport(msg, dest->ref);
	msg_set_hdr_sz(msg, DIR_MSG_H_SIZE);
	msg_set_size(msg, DIR_MSG_H_SIZE + dsz);
	if (skb_cow(buf, DIR_MSG_H_SIZE))
		return -ENOMEM;

	skb_push(buf, DIR_MSG_H_SIZE);
	skb_copy_to_linear_data(buf, msg, DIR_MSG_H_SIZE);
=======
	msg_set_destnode(msg, dest->node);
	msg_set_destport(msg, dest->ref);
	msg_set_hdr_sz(msg, BASIC_H_SIZE);
	msg_set_size(msg, BASIC_H_SIZE + dsz);
	if (skb_cow(buf, BASIC_H_SIZE))
		return -ENOMEM;

	skb_push(buf, BASIC_H_SIZE);
	skb_copy_to_linear_data(buf, msg, BASIC_H_SIZE);
>>>>>>> refs/remotes/origin/cm-10.0

	if (dest->node == tipc_own_addr)
		res = tipc_port_recv_msg(buf);
	else
		res = tipc_send_buf_fast(buf, dest->node);
	if (likely(res != -ELINKCONG)) {
		if (res > 0)
			p_ptr->sent++;
		return res;
	}
	if (port_unreliable(p_ptr))
		return dsz;
	return -ELINKCONG;
}

=======
		return len;
	}
	return -ELINKCONG;
}
>>>>>>> refs/remotes/origin/master
