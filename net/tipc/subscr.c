/*
 * net/tipc/subscr.c: TIPC network topology service
 *
 * Copyright (c) 2000-2006, Ericsson AB
<<<<<<< HEAD
 * Copyright (c) 2005-2007, 2010-2011, Wind River Systems
=======
 * Copyright (c) 2005-2007, 2010-2013, Wind River Systems
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
#include "name_table.h"
#include "port.h"
#include "subscr.h"

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * struct subscriber - TIPC network topology subscriber
=======
 * struct tipc_subscriber - TIPC network topology subscriber
>>>>>>> refs/remotes/origin/cm-10.0
 * @port_ref: object reference to server port connecting to subscriber
 * @lock: pointer to spinlock controlling access to subscriber's server port
 * @subscriber_list: adjacent subscribers in top. server's list of subscribers
 * @subscription_list: list of subscription objects for this subscriber
 */

<<<<<<< HEAD
struct subscriber {
=======
struct tipc_subscriber {
>>>>>>> refs/remotes/origin/cm-10.0
	u32 port_ref;
	spinlock_t *lock;
	struct list_head subscriber_list;
	struct list_head subscription_list;
};

/**
 * struct top_srv - TIPC network topology subscription service
 * @user_ref: TIPC userid of subscription service
 * @setup_port: reference to TIPC port that handles subscription requests
 * @subscription_count: number of active subscriptions (not subscribers!)
 * @subscriber_list: list of ports subscribing to service
 * @lock: spinlock govering access to subscriber list
 */

struct top_srv {
	u32 setup_port;
	atomic_t subscription_count;
	struct list_head subscriber_list;
	spinlock_t lock;
};

static struct top_srv topsrv;
=======
 * struct tipc_subscriber - TIPC network topology subscriber
 * @conid: connection identifier to server connecting to subscriber
 * @lock: controll access to subscriber
 * @subscription_list: list of subscription objects for this subscriber
 */
struct tipc_subscriber {
	int conid;
	spinlock_t lock;
	struct list_head subscription_list;
};

static void subscr_conn_msg_event(int conid, struct sockaddr_tipc *addr,
				  void *usr_data, void *buf, size_t len);
static void *subscr_named_msg_event(int conid);
static void subscr_conn_shutdown_event(int conid, void *usr_data);

static atomic_t subscription_count = ATOMIC_INIT(0);

static struct sockaddr_tipc topsrv_addr __read_mostly = {
	.family			= AF_TIPC,
	.addrtype		= TIPC_ADDR_NAMESEQ,
	.addr.nameseq.type	= TIPC_TOP_SRV,
	.addr.nameseq.lower	= TIPC_TOP_SRV,
	.addr.nameseq.upper	= TIPC_TOP_SRV,
	.scope			= TIPC_NODE_SCOPE
};

static struct tipc_server topsrv __read_mostly = {
	.saddr			= &topsrv_addr,
	.imp			= TIPC_CRITICAL_IMPORTANCE,
	.type			= SOCK_SEQPACKET,
	.max_rcvbuf_size	= sizeof(struct tipc_subscr),
	.name			= "topology_server",
	.tipc_conn_recvmsg	= subscr_conn_msg_event,
	.tipc_conn_new		= subscr_named_msg_event,
	.tipc_conn_shutdown	= subscr_conn_shutdown_event,
};
>>>>>>> refs/remotes/origin/master

/**
 * htohl - convert value to endianness used by destination
 * @in: value to convert
 * @swap: non-zero if endianness must be reversed
 *
 * Returns converted value
 */
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
static u32 htohl(u32 in, int swap)
{
	return swap ? swab32(in) : in;
}

<<<<<<< HEAD
/**
 * subscr_send_event - send a message containing a tipc_event to the subscriber
 *
 * Note: Must not hold subscriber's server port lock, since tipc_send() will
 *       try to take the lock if the message is rejected and returned!
 */

<<<<<<< HEAD
static void subscr_send_event(struct subscription *sub,
=======
static void subscr_send_event(struct tipc_subscription *sub,
>>>>>>> refs/remotes/origin/cm-10.0
			      u32 found_lower,
			      u32 found_upper,
			      u32 event,
			      u32 port_ref,
			      u32 node)
{
	struct iovec msg_sect;
=======
static void subscr_send_event(struct tipc_subscription *sub, u32 found_lower,
			      u32 found_upper, u32 event, u32 port_ref,
			      u32 node)
{
	struct tipc_subscriber *subscriber = sub->subscriber;
	struct kvec msg_sect;
	int ret;
>>>>>>> refs/remotes/origin/master

	msg_sect.iov_base = (void *)&sub->evt;
	msg_sect.iov_len = sizeof(struct tipc_event);

	sub->evt.event = htohl(event, sub->swap);
	sub->evt.found_lower = htohl(found_lower, sub->swap);
	sub->evt.found_upper = htohl(found_upper, sub->swap);
	sub->evt.port.ref = htohl(port_ref, sub->swap);
	sub->evt.port.node = htohl(node, sub->swap);
<<<<<<< HEAD
	tipc_send(sub->server_ref, 1, &msg_sect, msg_sect.iov_len);
=======
	ret = tipc_conn_sendmsg(&topsrv, subscriber->conid, NULL,
				msg_sect.iov_base, msg_sect.iov_len);
	if (ret < 0)
		pr_err("Sending subscription event failed, no memory\n");
>>>>>>> refs/remotes/origin/master
}

/**
 * tipc_subscr_overlap - test for subscription overlap with the given values
 *
 * Returns 1 if there is overlap, otherwise 0.
 */
<<<<<<< HEAD

<<<<<<< HEAD
int tipc_subscr_overlap(struct subscription *sub,
=======
int tipc_subscr_overlap(struct tipc_subscription *sub,
>>>>>>> refs/remotes/origin/cm-10.0
			u32 found_lower,
			u32 found_upper)

=======
int tipc_subscr_overlap(struct tipc_subscription *sub, u32 found_lower,
			u32 found_upper)
>>>>>>> refs/remotes/origin/master
{
	if (found_lower < sub->seq.lower)
		found_lower = sub->seq.lower;
	if (found_upper > sub->seq.upper)
		found_upper = sub->seq.upper;
	if (found_lower > found_upper)
		return 0;
	return 1;
}

/**
 * tipc_subscr_report_overlap - issue event if there is subscription overlap
 *
 * Protected by nameseq.lock in name_table.c
 */
<<<<<<< HEAD

<<<<<<< HEAD
void tipc_subscr_report_overlap(struct subscription *sub,
=======
void tipc_subscr_report_overlap(struct tipc_subscription *sub,
>>>>>>> refs/remotes/origin/cm-10.0
				u32 found_lower,
				u32 found_upper,
				u32 event,
				u32 port_ref,
				u32 node,
				int must)
=======
void tipc_subscr_report_overlap(struct tipc_subscription *sub, u32 found_lower,
				u32 found_upper, u32 event, u32 port_ref,
				u32 node, int must)
>>>>>>> refs/remotes/origin/master
{
	if (!tipc_subscr_overlap(sub, found_lower, found_upper))
		return;
	if (!must && !(sub->filter & TIPC_SUB_PORTS))
		return;

<<<<<<< HEAD
<<<<<<< HEAD
	sub->event_cb(sub, found_lower, found_upper, event, port_ref, node);
=======
	subscr_send_event(sub, found_lower, found_upper, event, port_ref, node);
>>>>>>> refs/remotes/origin/cm-10.0
}

/**
 * subscr_timeout - subscription timeout has occurred
 */

<<<<<<< HEAD
static void subscr_timeout(struct subscription *sub)
=======
static void subscr_timeout(struct tipc_subscription *sub)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct tipc_port *server_port;

	/* Validate server port reference (in case subscriber is terminating) */

	server_port = tipc_port_lock(sub->server_ref);
	if (server_port == NULL)
		return;

	/* Validate timeout (in case subscription is being cancelled) */

	if (sub->timeout == TIPC_WAIT_FOREVER) {
		tipc_port_unlock(server_port);
=======
	subscr_send_event(sub, found_lower, found_upper, event, port_ref, node);
}

static void subscr_timeout(struct tipc_subscription *sub)
{
	struct tipc_subscriber *subscriber = sub->subscriber;

	/* The spin lock per subscriber is used to protect its members */
	spin_lock_bh(&subscriber->lock);

	/* Validate if the connection related to the subscriber is
	 * closed (in case subscriber is terminating)
	 */
	if (subscriber->conid == 0) {
		spin_unlock_bh(&subscriber->lock);
		return;
	}

	/* Validate timeout (in case subscription is being cancelled) */
	if (sub->timeout == TIPC_WAIT_FOREVER) {
		spin_unlock_bh(&subscriber->lock);
>>>>>>> refs/remotes/origin/master
		return;
	}

	/* Unlink subscription from name table */
<<<<<<< HEAD

	tipc_nametbl_unsubscribe(sub);

	/* Unlink subscription from subscriber */

	list_del(&sub->subscription_list);

	/* Release subscriber's server port */

	tipc_port_unlock(server_port);

	/* Notify subscriber of timeout */

=======
	tipc_nametbl_unsubscribe(sub);

	/* Unlink subscription from subscriber */
	list_del(&sub->subscription_list);

	spin_unlock_bh(&subscriber->lock);

	/* Notify subscriber of timeout */
>>>>>>> refs/remotes/origin/master
	subscr_send_event(sub, sub->evt.s.seq.lower, sub->evt.s.seq.upper,
			  TIPC_SUBSCR_TIMEOUT, 0, 0);

	/* Now destroy subscription */
<<<<<<< HEAD

	k_term_timer(&sub->timer);
	kfree(sub);
	atomic_dec(&topsrv.subscription_count);
=======
	k_term_timer(&sub->timer);
	kfree(sub);
	atomic_dec(&subscription_count);
>>>>>>> refs/remotes/origin/master
}

/**
 * subscr_del - delete a subscription within a subscription list
 *
<<<<<<< HEAD
 * Called with subscriber port locked.
 */

<<<<<<< HEAD
static void subscr_del(struct subscription *sub)
=======
static void subscr_del(struct tipc_subscription *sub)
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Called with subscriber lock held.
 */
static void subscr_del(struct tipc_subscription *sub)
>>>>>>> refs/remotes/origin/master
{
	tipc_nametbl_unsubscribe(sub);
	list_del(&sub->subscription_list);
	kfree(sub);
<<<<<<< HEAD
	atomic_dec(&topsrv.subscription_count);
=======
	atomic_dec(&subscription_count);
>>>>>>> refs/remotes/origin/master
}

/**
 * subscr_terminate - terminate communication with a subscriber
 *
<<<<<<< HEAD
 * Called with subscriber port locked.  Routine must temporarily release lock
 * to enable subscription timeout routine(s) to finish without deadlocking;
 * the lock is then reclaimed to allow caller to release it upon return.
 * (This should work even in the unlikely event some other thread creates
 * a new object reference in the interim that uses this lock; this routine will
 * simply wait for it to be released, then claim it.)
 */

<<<<<<< HEAD
static void subscr_terminate(struct subscriber *subscriber)
{
	u32 port_ref;
	struct subscription *sub;
	struct subscription *sub_temp;
=======
static void subscr_terminate(struct tipc_subscriber *subscriber)
{
	u32 port_ref;
	struct tipc_subscription *sub;
	struct tipc_subscription *sub_temp;
>>>>>>> refs/remotes/origin/cm-10.0

	/* Invalidate subscriber reference */

	port_ref = subscriber->port_ref;
	subscriber->port_ref = 0;
	spin_unlock_bh(subscriber->lock);

	/* Sever connection to subscriber */

	tipc_shutdown(port_ref);
	tipc_deleteport(port_ref);

	/* Destroy any existing subscriptions for subscriber */

	list_for_each_entry_safe(sub, sub_temp, &subscriber->subscription_list,
				 subscription_list) {
		if (sub->timeout != TIPC_WAIT_FOREVER) {
			k_cancel_timer(&sub->timer);
			k_term_timer(&sub->timer);
		}
		subscr_del(sub);
	}

	/* Remove subscriber from topology server's subscriber list */

	spin_lock_bh(&topsrv.lock);
	list_del(&subscriber->subscriber_list);
	spin_unlock_bh(&topsrv.lock);

	/* Reclaim subscriber lock */

	spin_lock_bh(subscriber->lock);

	/* Now destroy subscriber */

=======
 * Note: Must call it in process context since it might sleep.
 */
static void subscr_terminate(struct tipc_subscriber *subscriber)
{
	tipc_conn_terminate(&topsrv, subscriber->conid);
}

static void subscr_release(struct tipc_subscriber *subscriber)
{
	struct tipc_subscription *sub;
	struct tipc_subscription *sub_temp;

	spin_lock_bh(&subscriber->lock);

	/* Invalidate subscriber reference */
	subscriber->conid = 0;

	/* Destroy any existing subscriptions for subscriber */
	list_for_each_entry_safe(sub, sub_temp, &subscriber->subscription_list,
				 subscription_list) {
		if (sub->timeout != TIPC_WAIT_FOREVER) {
			spin_unlock_bh(&subscriber->lock);
			k_cancel_timer(&sub->timer);
			k_term_timer(&sub->timer);
			spin_lock_bh(&subscriber->lock);
		}
		subscr_del(sub);
	}
	spin_unlock_bh(&subscriber->lock);

	/* Now destroy subscriber */
>>>>>>> refs/remotes/origin/master
	kfree(subscriber);
}

/**
 * subscr_cancel - handle subscription cancellation request
 *
<<<<<<< HEAD
 * Called with subscriber port locked.  Routine must temporarily release lock
=======
 * Called with subscriber lock held. Routine must temporarily release lock
>>>>>>> refs/remotes/origin/master
 * to enable the subscription timeout routine to finish without deadlocking;
 * the lock is then reclaimed to allow caller to release it upon return.
 *
 * Note that fields of 's' use subscriber's endianness!
 */
<<<<<<< HEAD

static void subscr_cancel(struct tipc_subscr *s,
<<<<<<< HEAD
			  struct subscriber *subscriber)
{
	struct subscription *sub;
	struct subscription *sub_temp;
=======
=======
static void subscr_cancel(struct tipc_subscr *s,
>>>>>>> refs/remotes/origin/master
			  struct tipc_subscriber *subscriber)
{
	struct tipc_subscription *sub;
	struct tipc_subscription *sub_temp;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	int found = 0;

	/* Find first matching subscription, exit if not found */

=======
	int found = 0;

	/* Find first matching subscription, exit if not found */
>>>>>>> refs/remotes/origin/master
	list_for_each_entry_safe(sub, sub_temp, &subscriber->subscription_list,
				 subscription_list) {
		if (!memcmp(s, &sub->evt.s, sizeof(struct tipc_subscr))) {
			found = 1;
			break;
		}
	}
	if (!found)
		return;

	/* Cancel subscription timer (if used), then delete subscription */
<<<<<<< HEAD

	if (sub->timeout != TIPC_WAIT_FOREVER) {
		sub->timeout = TIPC_WAIT_FOREVER;
		spin_unlock_bh(subscriber->lock);
		k_cancel_timer(&sub->timer);
		k_term_timer(&sub->timer);
		spin_lock_bh(subscriber->lock);
=======
	if (sub->timeout != TIPC_WAIT_FOREVER) {
		sub->timeout = TIPC_WAIT_FOREVER;
		spin_unlock_bh(&subscriber->lock);
		k_cancel_timer(&sub->timer);
		k_term_timer(&sub->timer);
		spin_lock_bh(&subscriber->lock);
>>>>>>> refs/remotes/origin/master
	}
	subscr_del(sub);
}

/**
 * subscr_subscribe - create subscription for subscriber
 *
<<<<<<< HEAD
 * Called with subscriber port locked.
 */

<<<<<<< HEAD
static struct subscription *subscr_subscribe(struct tipc_subscr *s,
					     struct subscriber *subscriber)
{
	struct subscription *sub;
=======
=======
 * Called with subscriber lock held.
 */
>>>>>>> refs/remotes/origin/master
static struct tipc_subscription *subscr_subscribe(struct tipc_subscr *s,
					     struct tipc_subscriber *subscriber)
{
	struct tipc_subscription *sub;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	int swap;

	/* Determine subscriber's endianness */

	swap = !(s->filter & (TIPC_SUB_PORTS | TIPC_SUB_SERVICE));

	/* Detect & process a subscription cancellation request */

=======
	int swap;

	/* Determine subscriber's endianness */
	swap = !(s->filter & (TIPC_SUB_PORTS | TIPC_SUB_SERVICE));

	/* Detect & process a subscription cancellation request */
>>>>>>> refs/remotes/origin/master
	if (s->filter & htohl(TIPC_SUB_CANCEL, swap)) {
		s->filter &= ~htohl(TIPC_SUB_CANCEL, swap);
		subscr_cancel(s, subscriber);
		return NULL;
	}

	/* Refuse subscription if global limit exceeded */
<<<<<<< HEAD

	if (atomic_read(&topsrv.subscription_count) >= tipc_max_subscriptions) {
		warn("Subscription rejected, subscription limit reached (%u)\n",
		     tipc_max_subscriptions);
=======
	if (atomic_read(&subscription_count) >= TIPC_MAX_SUBSCRIPTIONS) {
		pr_warn("Subscription rejected, limit reached (%u)\n",
			TIPC_MAX_SUBSCRIPTIONS);
>>>>>>> refs/remotes/origin/master
		subscr_terminate(subscriber);
		return NULL;
	}

	/* Allocate subscription object */
<<<<<<< HEAD

	sub = kmalloc(sizeof(*sub), GFP_ATOMIC);
	if (!sub) {
		warn("Subscription rejected, no memory\n");
=======
	sub = kmalloc(sizeof(*sub), GFP_ATOMIC);
	if (!sub) {
		pr_warn("Subscription rejected, no memory\n");
>>>>>>> refs/remotes/origin/master
		subscr_terminate(subscriber);
		return NULL;
	}

	/* Initialize subscription object */
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
	sub->seq.type = htohl(s->seq.type, swap);
	sub->seq.lower = htohl(s->seq.lower, swap);
	sub->seq.upper = htohl(s->seq.upper, swap);
	sub->timeout = htohl(s->timeout, swap);
	sub->filter = htohl(s->filter, swap);
	if ((!(sub->filter & TIPC_SUB_PORTS) ==
	     !(sub->filter & TIPC_SUB_SERVICE)) ||
	    (sub->seq.lower > sub->seq.upper)) {
<<<<<<< HEAD
		warn("Subscription rejected, illegal request\n");
=======
		pr_warn("Subscription rejected, illegal request\n");
>>>>>>> refs/remotes/origin/master
		kfree(sub);
		subscr_terminate(subscriber);
		return NULL;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	sub->event_cb = subscr_send_event;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	INIT_LIST_HEAD(&sub->nameseq_list);
	list_add(&sub->subscription_list, &subscriber->subscription_list);
	sub->server_ref = subscriber->port_ref;
	sub->swap = swap;
	memcpy(&sub->evt.s, s, sizeof(struct tipc_subscr));
	atomic_inc(&topsrv.subscription_count);
=======
	INIT_LIST_HEAD(&sub->nameseq_list);
	list_add(&sub->subscription_list, &subscriber->subscription_list);
	sub->subscriber = subscriber;
	sub->swap = swap;
	memcpy(&sub->evt.s, s, sizeof(struct tipc_subscr));
	atomic_inc(&subscription_count);
>>>>>>> refs/remotes/origin/master
	if (sub->timeout != TIPC_WAIT_FOREVER) {
		k_init_timer(&sub->timer,
			     (Handler)subscr_timeout, (unsigned long)sub);
		k_start_timer(&sub->timer, sub->timeout);
	}

	return sub;
}

<<<<<<< HEAD
/**
 * subscr_conn_shutdown_event - handle termination request from subscriber
 *
 * Called with subscriber's server port unlocked.
 */

static void subscr_conn_shutdown_event(void *usr_handle,
				       u32 port_ref,
				       struct sk_buff **buf,
				       unsigned char const *data,
				       unsigned int size,
				       int reason)
{
<<<<<<< HEAD
	struct subscriber *subscriber = usr_handle;
=======
	struct tipc_subscriber *subscriber = usr_handle;
>>>>>>> refs/remotes/origin/cm-10.0
	spinlock_t *subscriber_lock;

	if (tipc_port_lock(port_ref) == NULL)
		return;

	subscriber_lock = subscriber->lock;
	subscr_terminate(subscriber);
	spin_unlock_bh(subscriber_lock);
}

/**
 * subscr_conn_msg_event - handle new subscription request from subscriber
 *
 * Called with subscriber's server port unlocked.
 */

static void subscr_conn_msg_event(void *usr_handle,
				  u32 port_ref,
				  struct sk_buff **buf,
				  const unchar *data,
				  u32 size)
{
<<<<<<< HEAD
	struct subscriber *subscriber = usr_handle;
	spinlock_t *subscriber_lock;
	struct subscription *sub;
=======
	struct tipc_subscriber *subscriber = usr_handle;
	spinlock_t *subscriber_lock;
	struct tipc_subscription *sub;
>>>>>>> refs/remotes/origin/cm-10.0

	/*
	 * Lock subscriber's server port (& make a local copy of lock pointer,
	 * in case subscriber is deleted while processing subscription request)
	 */

	if (tipc_port_lock(port_ref) == NULL)
		return;

	subscriber_lock = subscriber->lock;

	if (size != sizeof(struct tipc_subscr)) {
		subscr_terminate(subscriber);
		spin_unlock_bh(subscriber_lock);
	} else {
		sub = subscr_subscribe((struct tipc_subscr *)data, subscriber);
		spin_unlock_bh(subscriber_lock);
		if (sub != NULL) {

			/*
			 * We must release the server port lock before adding a
			 * subscription to the name table since TIPC needs to be
			 * able to (re)acquire the port lock if an event message
			 * issued by the subscription process is rejected and
			 * returned.  The subscription cannot be deleted while
			 * it is being added to the name table because:
			 * a) the single-threading of the native API port code
			 *    ensures the subscription cannot be cancelled and
			 *    the subscriber connection cannot be broken, and
			 * b) the name table lock ensures the subscription
			 *    timeout code cannot delete the subscription,
			 * so the subscription object is still protected.
			 */

			tipc_nametbl_subscribe(sub);
		}
	}
}

/**
 * subscr_named_msg_event - handle request to establish a new subscriber
 */

static void subscr_named_msg_event(void *usr_handle,
				   u32 port_ref,
				   struct sk_buff **buf,
				   const unchar *data,
				   u32 size,
				   u32 importance,
				   struct tipc_portid const *orig,
				   struct tipc_name_seq const *dest)
{
<<<<<<< HEAD
	struct subscriber *subscriber;
=======
	struct tipc_subscriber *subscriber;
>>>>>>> refs/remotes/origin/cm-10.0
	u32 server_port_ref;

	/* Create subscriber object */

<<<<<<< HEAD
	subscriber = kzalloc(sizeof(struct subscriber), GFP_ATOMIC);
=======
	subscriber = kzalloc(sizeof(struct tipc_subscriber), GFP_ATOMIC);
>>>>>>> refs/remotes/origin/cm-10.0
	if (subscriber == NULL) {
		warn("Subscriber rejected, no memory\n");
		return;
	}
	INIT_LIST_HEAD(&subscriber->subscription_list);
	INIT_LIST_HEAD(&subscriber->subscriber_list);

	/* Create server port & establish connection to subscriber */

	tipc_createport(subscriber,
			importance,
			NULL,
			NULL,
			subscr_conn_shutdown_event,
			NULL,
			NULL,
			subscr_conn_msg_event,
			NULL,
			&subscriber->port_ref);
	if (subscriber->port_ref == 0) {
		warn("Subscriber rejected, unable to create port\n");
		kfree(subscriber);
		return;
	}
	tipc_connect2port(subscriber->port_ref, orig);

	/* Lock server port (& save lock address for future use) */

	subscriber->lock = tipc_port_lock(subscriber->port_ref)->lock;

	/* Add subscriber to topology server's subscriber list */

	spin_lock_bh(&topsrv.lock);
	list_add(&subscriber->subscriber_list, &topsrv.subscriber_list);
	spin_unlock_bh(&topsrv.lock);

	/* Unlock server port */

	server_port_ref = subscriber->port_ref;
	spin_unlock_bh(subscriber->lock);

	/* Send an ACK- to complete connection handshaking */

	tipc_send(server_port_ref, 0, NULL, 0);

	/* Handle optional subscription request */

	if (size != 0) {
		subscr_conn_msg_event(subscriber, server_port_ref,
				      buf, data, size);
	}
=======
/* Handle one termination request for the subscriber */
static void subscr_conn_shutdown_event(int conid, void *usr_data)
{
	subscr_release((struct tipc_subscriber *)usr_data);
}

/* Handle one request to create a new subscription for the subscriber */
static void subscr_conn_msg_event(int conid, struct sockaddr_tipc *addr,
				  void *usr_data, void *buf, size_t len)
{
	struct tipc_subscriber *subscriber = usr_data;
	struct tipc_subscription *sub;

	spin_lock_bh(&subscriber->lock);
	sub = subscr_subscribe((struct tipc_subscr *)buf, subscriber);
	if (sub)
		tipc_nametbl_subscribe(sub);
	spin_unlock_bh(&subscriber->lock);
}


/* Handle one request to establish a new subscriber */
static void *subscr_named_msg_event(int conid)
{
	struct tipc_subscriber *subscriber;

	/* Create subscriber object */
	subscriber = kzalloc(sizeof(struct tipc_subscriber), GFP_ATOMIC);
	if (subscriber == NULL) {
		pr_warn("Subscriber rejected, no memory\n");
		return NULL;
	}
	INIT_LIST_HEAD(&subscriber->subscription_list);
	subscriber->conid = conid;
	spin_lock_init(&subscriber->lock);

	return (void *)subscriber;
>>>>>>> refs/remotes/origin/master
}

int tipc_subscr_start(void)
{
<<<<<<< HEAD
	struct tipc_name_seq seq = {TIPC_TOP_SRV, TIPC_TOP_SRV, TIPC_TOP_SRV};
	int res;

	memset(&topsrv, 0, sizeof(topsrv));
	spin_lock_init(&topsrv.lock);
	INIT_LIST_HEAD(&topsrv.subscriber_list);

	res = tipc_createport(NULL,
			      TIPC_CRITICAL_IMPORTANCE,
			      NULL,
			      NULL,
			      NULL,
			      NULL,
			      subscr_named_msg_event,
			      NULL,
			      NULL,
			      &topsrv.setup_port);
	if (res)
		goto failed;

<<<<<<< HEAD
	res = tipc_nametbl_publish_rsv(topsrv.setup_port, TIPC_NODE_SCOPE, &seq);
=======
	res = tipc_publish(topsrv.setup_port, TIPC_NODE_SCOPE, &seq);
>>>>>>> refs/remotes/origin/cm-10.0
	if (res) {
		tipc_deleteport(topsrv.setup_port);
		topsrv.setup_port = 0;
		goto failed;
	}

	return 0;

failed:
	err("Failed to create subscription service\n");
	return res;
=======
	return tipc_server_start(&topsrv);
>>>>>>> refs/remotes/origin/master
}

void tipc_subscr_stop(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct subscriber *subscriber;
	struct subscriber *subscriber_temp;
=======
	struct tipc_subscriber *subscriber;
	struct tipc_subscriber *subscriber_temp;
>>>>>>> refs/remotes/origin/cm-10.0
	spinlock_t *subscriber_lock;

	if (topsrv.setup_port) {
		tipc_deleteport(topsrv.setup_port);
		topsrv.setup_port = 0;

		list_for_each_entry_safe(subscriber, subscriber_temp,
					 &topsrv.subscriber_list,
					 subscriber_list) {
			subscriber_lock = subscriber->lock;
			spin_lock_bh(subscriber_lock);
			subscr_terminate(subscriber);
			spin_unlock_bh(subscriber_lock);
		}
	}
=======
	tipc_server_stop(&topsrv);
>>>>>>> refs/remotes/origin/master
}
