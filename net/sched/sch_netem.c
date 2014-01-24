/*
 * net/sched/sch_netem.c	Network emulator
 *
 * 		This program is free software; you can redistribute it and/or
 * 		modify it under the terms of the GNU General Public License
 * 		as published by the Free Software Foundation; either version
 * 		2 of the License.
 *
 *  		Many of the algorithms and ideas for this came from
 *		NIST Net which is not copyrighted.
 *
 * Authors:	Stephen Hemminger <shemminger@osdl.org>
 *		Catalin(ux aka Dino) BOIE <catab at umbrella dot ro>
 */

<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/mm.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/mm.h>
>>>>>>> refs/remotes/origin/master
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/skbuff.h>
#include <linux/vmalloc.h>
#include <linux/rtnetlink.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/reciprocal_div.h>
>>>>>>> refs/remotes/origin/cm-10.0

#include <net/netlink.h>
#include <net/pkt_sched.h>
=======
#include <linux/reciprocal_div.h>
#include <linux/rbtree.h>

#include <net/netlink.h>
#include <net/pkt_sched.h>
#include <net/inet_ecn.h>
>>>>>>> refs/remotes/origin/master

#define VERSION "1.3"

/*	Network Emulation Queuing algorithm.
	====================================

	Sources: [1] Mark Carson, Darrin Santay, "NIST Net - A Linux-based
		 Network Emulation Tool
		 [2] Luigi Rizzo, DummyNet for FreeBSD

	 ----------------------------------------------------------------

	 This started out as a simple way to delay outgoing packets to
	 test TCP but has grown to include most of the functionality
	 of a full blown network emulator like NISTnet. It can delay
	 packets and add random jitter (and correlation). The random
	 distribution can be loaded from a table as well to provide
	 normal, Pareto, or experimental curves. Packet loss,
	 duplication, and reordering can also be emulated.

	 This qdisc does not do classification that can be handled in
	 layering other disciplines.  It does not need to do bandwidth
	 control either since that can be handled by using token
	 bucket or other rate control.

     Correlated Loss Generator models

	Added generation of correlated loss according to the
	"Gilbert-Elliot" model, a 4-state markov model.

	References:
	[1] NetemCLG Home http://netgroup.uniroma2.it/NetemCLG
	[2] S. Salsano, F. Ludovici, A. Ordine, "Definition of a general
	and intuitive loss model for packet networks and its implementation
	in the Netem module in the Linux kernel", available in [1]

	Authors: Stefano Salsano <stefano.salsano at uniroma2.it
		 Fabio Ludovici <fabio.ludovici at yahoo.it>
*/

struct netem_sched_data {
<<<<<<< HEAD
<<<<<<< HEAD
	struct Qdisc	*qdisc;
=======
	/* internal t(ime)fifo qdisc uses sch->q and sch->limit */
=======
	/* internal t(ime)fifo qdisc uses t_root and sch->limit */
	struct rb_root t_root;
>>>>>>> refs/remotes/origin/master

	/* optional qdisc for classful handling (NULL at netem init) */
	struct Qdisc	*qdisc;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	struct qdisc_watchdog watchdog;

	psched_tdiff_t latency;
	psched_tdiff_t jitter;

	u32 loss;
<<<<<<< HEAD
=======
	u32 ecn;
>>>>>>> refs/remotes/origin/master
	u32 limit;
	u32 counter;
	u32 gap;
	u32 duplicate;
	u32 reorder;
	u32 corrupt;
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	u32 rate;
	s32 packet_overhead;
	u32 cell_size;
	u32 cell_size_reciprocal;
	s32 cell_overhead;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	struct crndstate {
		u32 last;
		u32 rho;
	} delay_cor, loss_cor, dup_cor, reorder_cor, corrupt_cor;

	struct disttable {
		u32  size;
		s16 table[0];
	} *delay_dist;

	enum  {
		CLG_RANDOM,
		CLG_4_STATES,
		CLG_GILB_ELL,
	} loss_model;

	/* Correlated Loss Generation models */
	struct clgstate {
		/* state of the Markov chain */
		u8 state;

		/* 4-states and Gilbert-Elliot models */
		u32 a1;	/* p13 for 4-states or p for GE */
		u32 a2;	/* p31 for 4-states or r for GE */
		u32 a3;	/* p32 for 4-states or h for GE */
		u32 a4;	/* p14 for 4-states or 1-k for GE */
		u32 a5; /* p23 used only in 4-states */
	} clg;

};

<<<<<<< HEAD
<<<<<<< HEAD
/* Time stamp put into socket buffer control block */
=======
/* Time stamp put into socket buffer control block
 * Only valid when skbs are in our internal t(ime)fifo queue.
 */
>>>>>>> refs/remotes/origin/cm-10.0
struct netem_skb_cb {
	psched_time_t	time_to_send;
};

static inline struct netem_skb_cb *netem_skb_cb(struct sk_buff *skb)
{
=======
/* Time stamp put into socket buffer control block
 * Only valid when skbs are in our internal t(ime)fifo queue.
 */
struct netem_skb_cb {
	psched_time_t	time_to_send;
	ktime_t		tstamp_save;
};

/* Because space in skb->cb[] is tight, netem overloads skb->next/prev/tstamp
 * to hold a rb_node structure.
 *
 * If struct sk_buff layout is changed, the following checks will complain.
 */
static struct rb_node *netem_rb_node(struct sk_buff *skb)
{
	BUILD_BUG_ON(offsetof(struct sk_buff, next) != 0);
	BUILD_BUG_ON(offsetof(struct sk_buff, prev) !=
		     offsetof(struct sk_buff, next) + sizeof(skb->next));
	BUILD_BUG_ON(offsetof(struct sk_buff, tstamp) !=
		     offsetof(struct sk_buff, prev) + sizeof(skb->prev));
	BUILD_BUG_ON(sizeof(struct rb_node) > sizeof(skb->next) +
					      sizeof(skb->prev) +
					      sizeof(skb->tstamp));
	return (struct rb_node *)&skb->next;
}

static struct sk_buff *netem_rb_to_skb(struct rb_node *rb)
{
	return (struct sk_buff *)rb;
}

static inline struct netem_skb_cb *netem_skb_cb(struct sk_buff *skb)
{
	/* we assume we can use skb next/prev/tstamp as storage for rb_node */
>>>>>>> refs/remotes/origin/master
	qdisc_cb_private_validate(skb, sizeof(struct netem_skb_cb));
	return (struct netem_skb_cb *)qdisc_skb_cb(skb)->data;
}

/* init_crandom - initialize correlated random number generator
 * Use entropy source for initial seed.
 */
static void init_crandom(struct crndstate *state, unsigned long rho)
{
	state->rho = rho;
	state->last = net_random();
}

/* get_crandom - correlated random number generator
 * Next number depends on last value.
 * rho is scaled to avoid floating point.
 */
static u32 get_crandom(struct crndstate *state)
{
	u64 value, rho;
	unsigned long answer;

	if (state->rho == 0)	/* no correlation */
		return net_random();

	value = net_random();
	rho = (u64)state->rho + 1;
	answer = (value * ((1ull<<32) - rho) + state->last * rho) >> 32;
	state->last = answer;
	return answer;
}

/* loss_4state - 4-state model loss generator
 * Generates losses according to the 4-state Markov chain adopted in
 * the GI (General and Intuitive) loss model.
 */
static bool loss_4state(struct netem_sched_data *q)
{
	struct clgstate *clg = &q->clg;
	u32 rnd = net_random();

	/*
	 * Makes a comparison between rnd and the transition
	 * probabilities outgoing from the current state, then decides the
	 * next state and if the next packet has to be transmitted or lost.
	 * The four states correspond to:
	 *   1 => successfully transmitted packets within a gap period
	 *   4 => isolated losses within a gap period
	 *   3 => lost packets within a burst period
	 *   2 => successfully transmitted packets within a burst period
	 */
	switch (clg->state) {
	case 1:
		if (rnd < clg->a4) {
			clg->state = 4;
			return true;
<<<<<<< HEAD
		} else if (clg->a4 < rnd && rnd < clg->a1) {
			clg->state = 3;
			return true;
		} else if (clg->a1 < rnd)
=======
		} else if (clg->a4 < rnd && rnd < clg->a1 + clg->a4) {
			clg->state = 3;
			return true;
		} else if (clg->a1 + clg->a4 < rnd)
>>>>>>> refs/remotes/origin/master
			clg->state = 1;

		break;
	case 2:
		if (rnd < clg->a5) {
			clg->state = 3;
			return true;
		} else
			clg->state = 2;

		break;
	case 3:
		if (rnd < clg->a3)
			clg->state = 2;
		else if (clg->a3 < rnd && rnd < clg->a2 + clg->a3) {
			clg->state = 1;
<<<<<<< HEAD
			return true;
=======
>>>>>>> refs/remotes/origin/master
		} else if (clg->a2 + clg->a3 < rnd) {
			clg->state = 3;
			return true;
		}
		break;
	case 4:
		clg->state = 1;
		break;
	}

	return false;
}

/* loss_gilb_ell - Gilbert-Elliot model loss generator
 * Generates losses according to the Gilbert-Elliot loss model or
 * its special cases  (Gilbert or Simple Gilbert)
 *
 * Makes a comparison between random number and the transition
 * probabilities outgoing from the current state, then decides the
 * next state. A second random number is extracted and the comparison
 * with the loss probability of the current state decides if the next
 * packet will be transmitted or lost.
 */
static bool loss_gilb_ell(struct netem_sched_data *q)
{
	struct clgstate *clg = &q->clg;

	switch (clg->state) {
	case 1:
		if (net_random() < clg->a1)
			clg->state = 2;
		if (net_random() < clg->a4)
			return true;
<<<<<<< HEAD
	case 2:
		if (net_random() < clg->a2)
			clg->state = 1;
		if (clg->a3 > net_random())
=======
		break;
	case 2:
		if (net_random() < clg->a2)
			clg->state = 1;
		if (net_random() > clg->a3)
>>>>>>> refs/remotes/origin/master
			return true;
	}

	return false;
}

static bool loss_event(struct netem_sched_data *q)
{
	switch (q->loss_model) {
	case CLG_RANDOM:
		/* Random packet drop 0 => none, ~0 => all */
		return q->loss && q->loss >= get_crandom(&q->loss_cor);

	case CLG_4_STATES:
		/* 4state loss model algorithm (used also for GI model)
		* Extracts a value from the markov 4 state loss generator,
		* if it is 1 drops a packet and if needed writes the event in
		* the kernel logs
		*/
		return loss_4state(q);

	case CLG_GILB_ELL:
		/* Gilbert-Elliot loss model algorithm
		* Extracts a value from the Gilbert-Elliot loss generator,
		* if it is 1 drops a packet and if needed writes the event in
		* the kernel logs
		*/
		return loss_gilb_ell(q);
	}

	return false;	/* not reached */
}


/* tabledist - return a pseudo-randomly distributed value with mean mu and
 * std deviation sigma.  Uses table lookup to approximate the desired
 * distribution, and a uniformly-distributed pseudo-random source.
 */
static psched_tdiff_t tabledist(psched_tdiff_t mu, psched_tdiff_t sigma,
				struct crndstate *state,
				const struct disttable *dist)
{
	psched_tdiff_t x;
	long t;
	u32 rnd;

	if (sigma == 0)
		return mu;

	rnd = get_crandom(state);

	/* default uniform distribution */
	if (dist == NULL)
		return (rnd % (2*sigma)) - sigma + mu;

	t = dist->table[rnd % dist->size];
	x = (sigma % NETEM_DIST_SCALE) * t;
	if (x >= 0)
		x += NETEM_DIST_SCALE/2;
	else
		x -= NETEM_DIST_SCALE/2;

	return  x / NETEM_DIST_SCALE + (sigma / NETEM_DIST_SCALE) * t + mu;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static psched_time_t packet_len_2_sched_time(unsigned int len, struct netem_sched_data *q)
{
	u64 ticks;

	len += q->packet_overhead;

	if (q->cell_size) {
		u32 cells = reciprocal_divide(len, q->cell_size_reciprocal);

		if (len > cells * q->cell_size)	/* extra cell needed for remainder */
			cells++;
		len = cells * (q->cell_size + q->cell_overhead);
	}

	ticks = (u64)len * NSEC_PER_SEC;

	do_div(ticks, q->rate);
	return PSCHED_NS2TICKS(ticks);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
static void tfifo_enqueue(struct sk_buff *nskb, struct Qdisc *sch)
{
	struct sk_buff_head *list = &sch->q;
	psched_time_t tnext = netem_skb_cb(nskb)->time_to_send;
	struct sk_buff *skb = skb_peek_tail(list);
<<<<<<< HEAD

	/* Optimize for add at tail */
	if (likely(!skb || tnext >= netem_skb_cb(skb)->time_to_send))
		return __skb_queue_tail(list, nskb);

	skb_queue_reverse_walk(list, skb) {
		if (tnext >= netem_skb_cb(skb)->time_to_send)
			break;
	}

	__skb_queue_after(list, skb, nskb);
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
static void tfifo_reset(struct Qdisc *sch)
{
	struct netem_sched_data *q = qdisc_priv(sch);
	struct rb_node *p;

	while ((p = rb_first(&q->t_root))) {
		struct sk_buff *skb = netem_rb_to_skb(p);

		rb_erase(p, &q->t_root);
		skb->next = NULL;
		skb->prev = NULL;
		kfree_skb(skb);
=======

	/* Optimize for add at tail */
	if (likely(!skb || tnext >= netem_skb_cb(skb)->time_to_send))
		return __skb_queue_tail(list, nskb);

	skb_queue_reverse_walk(list, skb) {
		if (tnext >= netem_skb_cb(skb)->time_to_send)
			break;
>>>>>>> refs/remotes/origin/cm-11.0
	}
}

<<<<<<< HEAD
static void tfifo_enqueue(struct sk_buff *nskb, struct Qdisc *sch)
{
	struct netem_sched_data *q = qdisc_priv(sch);
	psched_time_t tnext = netem_skb_cb(nskb)->time_to_send;
	struct rb_node **p = &q->t_root.rb_node, *parent = NULL;

	while (*p) {
		struct sk_buff *skb;

		parent = *p;
		skb = netem_rb_to_skb(parent);
		if (tnext >= netem_skb_cb(skb)->time_to_send)
			p = &parent->rb_right;
		else
			p = &parent->rb_left;
	}
	rb_link_node(netem_rb_node(nskb), parent, p);
	rb_insert_color(netem_rb_node(nskb), &q->t_root);
	sch->q.qlen++;
=======
	__skb_queue_after(list, skb, nskb);
>>>>>>> refs/remotes/origin/cm-11.0
}

>>>>>>> refs/remotes/origin/master
/*
 * Insert one skb into qdisc.
 * Note: parent depends on return value to account for queue length.
 * 	NET_XMIT_DROP: queue length didn't change.
 *      NET_XMIT_SUCCESS: one skb was queued.
 */
static int netem_enqueue(struct sk_buff *skb, struct Qdisc *sch)
{
	struct netem_sched_data *q = qdisc_priv(sch);
	/* We don't fill cb now as skb_unshare() may invalidate it */
	struct netem_skb_cb *cb;
	struct sk_buff *skb2;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	int ret;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	int count = 1;

	/* Random duplication */
	if (q->duplicate && q->duplicate >= get_crandom(&q->dup_cor))
		++count;

	/* Drop packet? */
<<<<<<< HEAD
	if (loss_event(q))
		--count;

=======
	if (loss_event(q)) {
		if (q->ecn && INET_ECN_set_ce(skb))
			sch->qstats.drops++; /* mark packet */
		else
			--count;
	}
>>>>>>> refs/remotes/origin/master
	if (count == 0) {
		sch->qstats.drops++;
		kfree_skb(skb);
		return NET_XMIT_SUCCESS | __NET_XMIT_BYPASS;
	}

<<<<<<< HEAD
	skb_orphan(skb);
=======
	/* If a delay is expected, orphan the skb. (orphaning usually takes
	 * place at TX completion time, so _before_ the link transit delay)
	 */
	if (q->latency || q->jitter)
		skb_orphan_partial(skb);
>>>>>>> refs/remotes/origin/master

	/*
	 * If we need to duplicate packet, then re-insert at top of the
	 * qdisc tree, since parent queuer expects that only one
	 * skb will be queued.
	 */
	if (count > 1 && (skb2 = skb_clone(skb, GFP_ATOMIC)) != NULL) {
		struct Qdisc *rootq = qdisc_root(sch);
		u32 dupsave = q->duplicate; /* prevent duplicating a dup... */
		q->duplicate = 0;

		qdisc_enqueue_root(skb2, rootq);
		q->duplicate = dupsave;
	}

	/*
	 * Randomized packet corruption.
	 * Make copy if needed since we are modifying
	 * If packet is going to be hardware checksummed, then
	 * do it now in software before we mangle it.
	 */
	if (q->corrupt && q->corrupt >= get_crandom(&q->corrupt_cor)) {
		if (!(skb = skb_unshare(skb, GFP_ATOMIC)) ||
		    (skb->ip_summed == CHECKSUM_PARTIAL &&
		     skb_checksum_help(skb)))
			return qdisc_drop(skb, sch);

		skb->data[net_random() % skb_headlen(skb)] ^= 1<<(net_random() % 8);
	}

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	cb = netem_skb_cb(skb);
	if (q->gap == 0 ||		/* not doing reordering */
	    q->counter < q->gap ||	/* inside last reordering gap */
=======
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (unlikely(skb_queue_len(&sch->q) >= sch->limit))
		return qdisc_reshape_fail(skb, sch);

	sch->qstats.backlog += qdisc_pkt_len(skb);

	cb = netem_skb_cb(skb);
	if (q->gap == 0 ||		/* not doing reordering */
	    q->counter < q->gap - 1 ||	/* inside last reordering gap */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	    q->reorder < get_crandom(&q->reorder_cor)) {
		psched_time_t now;
		psched_tdiff_t delay;

		delay = tabledist(q->latency, q->jitter,
				  &q->delay_cor, q->delay_dist);

		now = psched_get_time();
<<<<<<< HEAD
<<<<<<< HEAD
		cb->time_to_send = now + delay;
		++q->counter;
		ret = qdisc_enqueue(skb, q->qdisc);
=======

		if (q->rate) {
			struct sk_buff_head *list = &sch->q;

			delay += packet_len_2_sched_time(skb->len, q);

			if (!skb_queue_empty(list)) {
				/*
				 * Last packet in queue is reference point (now).
				 * First packet in queue is already in flight,
				 * calculate this time bonus and substract
				 * from delay.
				 */
				delay -= now - netem_skb_cb(skb_peek(list))->time_to_send;
				now = netem_skb_cb(skb_peek_tail(list))->time_to_send;
			}
		}

		cb->time_to_send = now + delay;
		++q->counter;
		tfifo_enqueue(skb, sch);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======

		if (q->rate) {
			struct sk_buff *last;

			if (!skb_queue_empty(&sch->q))
				last = skb_peek_tail(&sch->q);
			else
				last = netem_rb_to_skb(rb_last(&q->t_root));
			if (last) {
				/*
				 * Last packet in queue is reference point (now),
				 * calculate this time bonus and subtract
				 * from delay.
				 */
				delay -= netem_skb_cb(last)->time_to_send - now;
				delay = max_t(psched_tdiff_t, 0, delay);
				now = netem_skb_cb(last)->time_to_send;
			}

			delay += packet_len_2_sched_time(skb->len, q);
		}

		cb->time_to_send = now + delay;
		cb->tstamp_save = skb->tstamp;
		++q->counter;
		tfifo_enqueue(skb, sch);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	} else {
		/*
		 * Do re-ordering by putting one out of N packets at the front
		 * of the queue.
		 */
		cb->time_to_send = psched_get_time();
		q->counter = 0;

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
		__skb_queue_head(&q->qdisc->q, skb);
		sch->qstats.backlog += qdisc_pkt_len(skb);
=======
		__skb_queue_head(&sch->q, skb);
>>>>>>> refs/remotes/origin/cm-11.0
		sch->qstats.requeues++;
	}

	sch->q.qlen++;
=======
=======
>>>>>>> refs/remotes/origin/master
		__skb_queue_head(&sch->q, skb);
		sch->qstats.requeues++;
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return NET_XMIT_SUCCESS;
}

static unsigned int netem_drop(struct Qdisc *sch)
{
	struct netem_sched_data *q = qdisc_priv(sch);
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned int len = 0;

	if (q->qdisc->ops->drop && (len = q->qdisc->ops->drop(q->qdisc)) != 0) {
		sch->q.qlen--;
		sch->qstats.drops++;
	}
=======
	unsigned int len;

	len = qdisc_queue_drop(sch);
=======
	unsigned int len;

	len = qdisc_queue_drop(sch);

	if (!len) {
		struct rb_node *p = rb_first(&q->t_root);

		if (p) {
			struct sk_buff *skb = netem_rb_to_skb(p);

			rb_erase(p, &q->t_root);
			sch->q.qlen--;
			skb->next = NULL;
			skb->prev = NULL;
			len = qdisc_pkt_len(skb);
			sch->qstats.backlog -= len;
			kfree_skb(skb);
		}
	}
>>>>>>> refs/remotes/origin/master
	if (!len && q->qdisc && q->qdisc->ops->drop)
	    len = q->qdisc->ops->drop(q->qdisc);
	if (len)
		sch->qstats.drops++;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return len;
}

static struct sk_buff *netem_dequeue(struct Qdisc *sch)
{
	struct netem_sched_data *q = qdisc_priv(sch);
	struct sk_buff *skb;
<<<<<<< HEAD
=======
	struct rb_node *p;
>>>>>>> refs/remotes/origin/master

	if (qdisc_is_throttled(sch))
		return NULL;

<<<<<<< HEAD
<<<<<<< HEAD
	skb = q->qdisc->ops->peek(q->qdisc);
	if (skb) {
		const struct netem_skb_cb *cb = netem_skb_cb(skb);
		psched_time_t now = psched_get_time();

		/* if more time remaining? */
		if (cb->time_to_send <= now) {
			skb = qdisc_dequeue_peeked(q->qdisc);
			if (unlikely(!skb))
				return NULL;
=======
tfifo_dequeue:
	skb = qdisc_peek_head(sch);
	if (skb) {
		const struct netem_skb_cb *cb = netem_skb_cb(skb);

		/* if more time remaining? */
		if (cb->time_to_send <= psched_get_time()) {
			__skb_unlink(skb, &sch->q);
			sch->qstats.backlog -= qdisc_pkt_len(skb);
>>>>>>> refs/remotes/origin/cm-10.0
=======
tfifo_dequeue:
	skb = __skb_dequeue(&sch->q);
	if (skb) {
deliver:
		sch->qstats.backlog -= qdisc_pkt_len(skb);
		qdisc_unthrottled(sch);
		qdisc_bstats_update(sch, skb);
		return skb;
	}
	p = rb_first(&q->t_root);
	if (p) {
		psched_time_t time_to_send;

		skb = netem_rb_to_skb(p);

		/* if more time remaining? */
		time_to_send = netem_skb_cb(skb)->time_to_send;
		if (time_to_send <= psched_get_time()) {
			rb_erase(p, &q->t_root);

			sch->q.qlen--;
			skb->next = NULL;
			skb->prev = NULL;
			skb->tstamp = netem_skb_cb(skb)->tstamp_save;
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_NET_CLS_ACT
			/*
			 * If it's at ingress let's pretend the delay is
			 * from the network (tstamp will be updated).
			 */
			if (G_TC_FROM(skb->tc_verd) & AT_INGRESS)
				skb->tstamp.tv64 = 0;
#endif

<<<<<<< HEAD
<<<<<<< HEAD
			sch->q.qlen--;
=======
=======
>>>>>>> refs/remotes/origin/master
			if (q->qdisc) {
				int err = qdisc_enqueue(skb, q->qdisc);

				if (unlikely(err != NET_XMIT_SUCCESS)) {
					if (net_xmit_drop_count(err)) {
						sch->qstats.drops++;
						qdisc_tree_decrease_qlen(sch, 1);
					}
				}
				goto tfifo_dequeue;
			}
<<<<<<< HEAD
deliver:
>>>>>>> refs/remotes/origin/cm-10.0
			qdisc_unthrottled(sch);
			qdisc_bstats_update(sch, skb);
			return skb;
		}

<<<<<<< HEAD
		qdisc_watchdog_schedule(&q->watchdog, cb->time_to_send);
	}

=======
=======
			goto deliver;
		}

>>>>>>> refs/remotes/origin/master
		if (q->qdisc) {
			skb = q->qdisc->ops->dequeue(q->qdisc);
			if (skb)
				goto deliver;
		}
<<<<<<< HEAD
		qdisc_watchdog_schedule(&q->watchdog, cb->time_to_send);
=======
		qdisc_watchdog_schedule(&q->watchdog, time_to_send);
>>>>>>> refs/remotes/origin/master
	}

	if (q->qdisc) {
		skb = q->qdisc->ops->dequeue(q->qdisc);
		if (skb)
			goto deliver;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return NULL;
}

static void netem_reset(struct Qdisc *sch)
{
	struct netem_sched_data *q = qdisc_priv(sch);

<<<<<<< HEAD
<<<<<<< HEAD
	qdisc_reset(q->qdisc);
	sch->q.qlen = 0;
=======
	qdisc_reset_queue(sch);
	if (q->qdisc)
		qdisc_reset(q->qdisc);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	qdisc_reset_queue(sch);
	tfifo_reset(sch);
	if (q->qdisc)
		qdisc_reset(q->qdisc);
>>>>>>> refs/remotes/origin/master
	qdisc_watchdog_cancel(&q->watchdog);
}

static void dist_free(struct disttable *d)
{
	if (d) {
		if (is_vmalloc_addr(d))
			vfree(d);
		else
			kfree(d);
	}
}

/*
 * Distribution data is a variable size payload containing
 * signed 16 bit values.
 */
static int get_dist_table(struct Qdisc *sch, const struct nlattr *attr)
{
	struct netem_sched_data *q = qdisc_priv(sch);
	size_t n = nla_len(attr)/sizeof(__s16);
	const __s16 *data = nla_data(attr);
	spinlock_t *root_lock;
	struct disttable *d;
	int i;
	size_t s;

	if (n > NETEM_DIST_MAX)
		return -EINVAL;

	s = sizeof(struct disttable) + n * sizeof(s16);
<<<<<<< HEAD
<<<<<<< HEAD
	d = kmalloc(s, GFP_KERNEL);
=======
	d = kmalloc(s, GFP_KERNEL | __GFP_NOWARN);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	d = kmalloc(s, GFP_KERNEL | __GFP_NOWARN);
>>>>>>> refs/remotes/origin/master
	if (!d)
		d = vmalloc(s);
	if (!d)
		return -ENOMEM;

	d->size = n;
	for (i = 0; i < n; i++)
		d->table[i] = data[i];

	root_lock = qdisc_root_sleeping_lock(sch);

	spin_lock_bh(root_lock);
<<<<<<< HEAD
<<<<<<< HEAD
	dist_free(q->delay_dist);
	q->delay_dist = d;
	spin_unlock_bh(root_lock);
=======
=======
>>>>>>> refs/remotes/origin/master
	swap(q->delay_dist, d);
	spin_unlock_bh(root_lock);

	dist_free(d);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}

static void get_correlation(struct Qdisc *sch, const struct nlattr *attr)
{
	struct netem_sched_data *q = qdisc_priv(sch);
	const struct tc_netem_corr *c = nla_data(attr);

	init_crandom(&q->delay_cor, c->delay_corr);
	init_crandom(&q->loss_cor, c->loss_corr);
	init_crandom(&q->dup_cor, c->dup_corr);
}

static void get_reorder(struct Qdisc *sch, const struct nlattr *attr)
{
	struct netem_sched_data *q = qdisc_priv(sch);
	const struct tc_netem_reorder *r = nla_data(attr);

	q->reorder = r->probability;
	init_crandom(&q->reorder_cor, r->correlation);
}

static void get_corrupt(struct Qdisc *sch, const struct nlattr *attr)
{
	struct netem_sched_data *q = qdisc_priv(sch);
	const struct tc_netem_corrupt *r = nla_data(attr);

	q->corrupt = r->probability;
	init_crandom(&q->corrupt_cor, r->correlation);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static void get_rate(struct Qdisc *sch, const struct nlattr *attr)
{
	struct netem_sched_data *q = qdisc_priv(sch);
	const struct tc_netem_rate *r = nla_data(attr);

	q->rate = r->rate;
	q->packet_overhead = r->packet_overhead;
	q->cell_size = r->cell_size;
	if (q->cell_size)
		q->cell_size_reciprocal = reciprocal_value(q->cell_size);
	q->cell_overhead = r->cell_overhead;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int get_loss_clg(struct Qdisc *sch, const struct nlattr *attr)
{
	struct netem_sched_data *q = qdisc_priv(sch);
	const struct nlattr *la;
	int rem;

	nla_for_each_nested(la, attr, rem) {
		u16 type = nla_type(la);

		switch(type) {
		case NETEM_LOSS_GI: {
			const struct tc_netem_gimodel *gi = nla_data(la);

<<<<<<< HEAD
<<<<<<< HEAD
			if (nla_len(la) != sizeof(struct tc_netem_gimodel)) {
=======
			if (nla_len(la) < sizeof(struct tc_netem_gimodel)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
			if (nla_len(la) < sizeof(struct tc_netem_gimodel)) {
>>>>>>> refs/remotes/origin/master
				pr_info("netem: incorrect gi model size\n");
				return -EINVAL;
			}

			q->loss_model = CLG_4_STATES;

			q->clg.state = 1;
			q->clg.a1 = gi->p13;
			q->clg.a2 = gi->p31;
			q->clg.a3 = gi->p32;
			q->clg.a4 = gi->p14;
			q->clg.a5 = gi->p23;
			break;
		}

		case NETEM_LOSS_GE: {
			const struct tc_netem_gemodel *ge = nla_data(la);

<<<<<<< HEAD
<<<<<<< HEAD
			if (nla_len(la) != sizeof(struct tc_netem_gemodel)) {
				pr_info("netem: incorrect gi model size\n");
=======
			if (nla_len(la) < sizeof(struct tc_netem_gemodel)) {
				pr_info("netem: incorrect ge model size\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
			if (nla_len(la) < sizeof(struct tc_netem_gemodel)) {
				pr_info("netem: incorrect ge model size\n");
>>>>>>> refs/remotes/origin/master
				return -EINVAL;
			}

			q->loss_model = CLG_GILB_ELL;
			q->clg.state = 1;
			q->clg.a1 = ge->p;
			q->clg.a2 = ge->r;
			q->clg.a3 = ge->h;
			q->clg.a4 = ge->k1;
			break;
		}

		default:
			pr_info("netem: unknown loss type %u\n", type);
			return -EINVAL;
		}
	}

	return 0;
}

static const struct nla_policy netem_policy[TCA_NETEM_MAX + 1] = {
	[TCA_NETEM_CORR]	= { .len = sizeof(struct tc_netem_corr) },
	[TCA_NETEM_REORDER]	= { .len = sizeof(struct tc_netem_reorder) },
	[TCA_NETEM_CORRUPT]	= { .len = sizeof(struct tc_netem_corrupt) },
<<<<<<< HEAD
<<<<<<< HEAD
=======
	[TCA_NETEM_RATE]	= { .len = sizeof(struct tc_netem_rate) },
>>>>>>> refs/remotes/origin/cm-10.0
	[TCA_NETEM_LOSS]	= { .type = NLA_NESTED },
=======
	[TCA_NETEM_RATE]	= { .len = sizeof(struct tc_netem_rate) },
	[TCA_NETEM_LOSS]	= { .type = NLA_NESTED },
	[TCA_NETEM_ECN]		= { .type = NLA_U32 },
>>>>>>> refs/remotes/origin/master
};

static int parse_attr(struct nlattr *tb[], int maxtype, struct nlattr *nla,
		      const struct nla_policy *policy, int len)
{
	int nested_len = nla_len(nla) - NLA_ALIGN(len);

	if (nested_len < 0) {
		pr_info("netem: invalid attributes len %d\n", nested_len);
		return -EINVAL;
	}

	if (nested_len >= nla_attr_size(0))
		return nla_parse(tb, maxtype, nla_data(nla) + NLA_ALIGN(len),
				 nested_len, policy);

	memset(tb, 0, sizeof(struct nlattr *) * (maxtype + 1));
	return 0;
}

/* Parse netlink message to set options */
static int netem_change(struct Qdisc *sch, struct nlattr *opt)
{
	struct netem_sched_data *q = qdisc_priv(sch);
	struct nlattr *tb[TCA_NETEM_MAX + 1];
	struct tc_netem_qopt *qopt;
	int ret;

	if (opt == NULL)
		return -EINVAL;

	qopt = nla_data(opt);
	ret = parse_attr(tb, TCA_NETEM_MAX, opt, netem_policy, sizeof(*qopt));
	if (ret < 0)
		return ret;

<<<<<<< HEAD
<<<<<<< HEAD
	ret = fifo_set_limit(q->qdisc, qopt->limit);
	if (ret) {
		pr_info("netem: can't set fifo limit\n");
		return ret;
	}
=======
	sch->limit = qopt->limit;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	sch->limit = qopt->limit;
>>>>>>> refs/remotes/origin/master

	q->latency = qopt->latency;
	q->jitter = qopt->jitter;
	q->limit = qopt->limit;
	q->gap = qopt->gap;
	q->counter = 0;
	q->loss = qopt->loss;
	q->duplicate = qopt->duplicate;

	/* for compatibility with earlier versions.
	 * if gap is set, need to assume 100% probability
	 */
	if (q->gap)
		q->reorder = ~0;

	if (tb[TCA_NETEM_CORR])
		get_correlation(sch, tb[TCA_NETEM_CORR]);

	if (tb[TCA_NETEM_DELAY_DIST]) {
		ret = get_dist_table(sch, tb[TCA_NETEM_DELAY_DIST]);
		if (ret)
			return ret;
	}

	if (tb[TCA_NETEM_REORDER])
		get_reorder(sch, tb[TCA_NETEM_REORDER]);

	if (tb[TCA_NETEM_CORRUPT])
		get_corrupt(sch, tb[TCA_NETEM_CORRUPT]);

<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (tb[TCA_NETEM_RATE])
		get_rate(sch, tb[TCA_NETEM_RATE]);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (tb[TCA_NETEM_RATE])
		get_rate(sch, tb[TCA_NETEM_RATE]);

	if (tb[TCA_NETEM_ECN])
		q->ecn = nla_get_u32(tb[TCA_NETEM_ECN]);

>>>>>>> refs/remotes/origin/master
	q->loss_model = CLG_RANDOM;
	if (tb[TCA_NETEM_LOSS])
		ret = get_loss_clg(sch, tb[TCA_NETEM_LOSS]);

	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * Special case version of FIFO queue for use by netem.
 * It queues in order based on timestamps in skb's
 */
struct fifo_sched_data {
	u32 limit;
	psched_time_t oldest;
};

static int tfifo_enqueue(struct sk_buff *nskb, struct Qdisc *sch)
{
	struct fifo_sched_data *q = qdisc_priv(sch);
	struct sk_buff_head *list = &sch->q;
	psched_time_t tnext = netem_skb_cb(nskb)->time_to_send;
	struct sk_buff *skb;

	if (likely(skb_queue_len(list) < q->limit)) {
		/* Optimize for add at tail */
		if (likely(skb_queue_empty(list) || tnext >= q->oldest)) {
			q->oldest = tnext;
			return qdisc_enqueue_tail(nskb, sch);
		}

		skb_queue_reverse_walk(list, skb) {
			const struct netem_skb_cb *cb = netem_skb_cb(skb);

			if (tnext >= cb->time_to_send)
				break;
		}

		__skb_queue_after(list, skb, nskb);

		sch->qstats.backlog += qdisc_pkt_len(nskb);

		return NET_XMIT_SUCCESS;
	}

	return qdisc_reshape_fail(nskb, sch);
}

static int tfifo_init(struct Qdisc *sch, struct nlattr *opt)
{
	struct fifo_sched_data *q = qdisc_priv(sch);

	if (opt) {
		struct tc_fifo_qopt *ctl = nla_data(opt);
		if (nla_len(opt) < sizeof(*ctl))
			return -EINVAL;

		q->limit = ctl->limit;
	} else
		q->limit = max_t(u32, qdisc_dev(sch)->tx_queue_len, 1);

	q->oldest = PSCHED_PASTPERFECT;
	return 0;
}

static int tfifo_dump(struct Qdisc *sch, struct sk_buff *skb)
{
	struct fifo_sched_data *q = qdisc_priv(sch);
	struct tc_fifo_qopt opt = { .limit = q->limit };

	NLA_PUT(skb, TCA_OPTIONS, sizeof(opt), &opt);
	return skb->len;

nla_put_failure:
	return -1;
}

static struct Qdisc_ops tfifo_qdisc_ops __read_mostly = {
	.id		=	"tfifo",
	.priv_size	=	sizeof(struct fifo_sched_data),
	.enqueue	=	tfifo_enqueue,
	.dequeue	=	qdisc_dequeue_head,
	.peek		=	qdisc_peek_head,
	.drop		=	qdisc_queue_drop,
	.init		=	tfifo_init,
	.reset		=	qdisc_reset_queue,
	.change		=	tfifo_init,
	.dump		=	tfifo_dump,
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int netem_init(struct Qdisc *sch, struct nlattr *opt)
{
	struct netem_sched_data *q = qdisc_priv(sch);
	int ret;

	if (!opt)
		return -EINVAL;

	qdisc_watchdog_init(&q->watchdog, sch);

	q->loss_model = CLG_RANDOM;
<<<<<<< HEAD
<<<<<<< HEAD
	q->qdisc = qdisc_create_dflt(sch->dev_queue, &tfifo_qdisc_ops,
				     TC_H_MAKE(sch->handle, 1));
	if (!q->qdisc) {
		pr_notice("netem: qdisc create tfifo qdisc failed\n");
		return -ENOMEM;
	}

	ret = netem_change(sch, opt);
	if (ret) {
		pr_info("netem: change failed\n");
		qdisc_destroy(q->qdisc);
	}
=======
	ret = netem_change(sch, opt);
	if (ret)
		pr_info("netem: change failed\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = netem_change(sch, opt);
	if (ret)
		pr_info("netem: change failed\n");
>>>>>>> refs/remotes/origin/master
	return ret;
}

static void netem_destroy(struct Qdisc *sch)
{
	struct netem_sched_data *q = qdisc_priv(sch);

	qdisc_watchdog_cancel(&q->watchdog);
<<<<<<< HEAD
<<<<<<< HEAD
	qdisc_destroy(q->qdisc);
=======
	if (q->qdisc)
		qdisc_destroy(q->qdisc);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (q->qdisc)
		qdisc_destroy(q->qdisc);
>>>>>>> refs/remotes/origin/master
	dist_free(q->delay_dist);
}

static int dump_loss_model(const struct netem_sched_data *q,
			   struct sk_buff *skb)
{
	struct nlattr *nest;

	nest = nla_nest_start(skb, TCA_NETEM_LOSS);
	if (nest == NULL)
		goto nla_put_failure;

	switch (q->loss_model) {
	case CLG_RANDOM:
		/* legacy loss model */
		nla_nest_cancel(skb, nest);
		return 0;	/* no data */

	case CLG_4_STATES: {
		struct tc_netem_gimodel gi = {
			.p13 = q->clg.a1,
			.p31 = q->clg.a2,
			.p32 = q->clg.a3,
			.p14 = q->clg.a4,
			.p23 = q->clg.a5,
		};

<<<<<<< HEAD
		NLA_PUT(skb, NETEM_LOSS_GI, sizeof(gi), &gi);
=======
		if (nla_put(skb, NETEM_LOSS_GI, sizeof(gi), &gi))
			goto nla_put_failure;
>>>>>>> refs/remotes/origin/master
		break;
	}
	case CLG_GILB_ELL: {
		struct tc_netem_gemodel ge = {
			.p = q->clg.a1,
			.r = q->clg.a2,
			.h = q->clg.a3,
			.k1 = q->clg.a4,
		};

<<<<<<< HEAD
		NLA_PUT(skb, NETEM_LOSS_GE, sizeof(ge), &ge);
=======
		if (nla_put(skb, NETEM_LOSS_GE, sizeof(ge), &ge))
			goto nla_put_failure;
>>>>>>> refs/remotes/origin/master
		break;
	}
	}

	nla_nest_end(skb, nest);
	return 0;

nla_put_failure:
	nla_nest_cancel(skb, nest);
	return -1;
}

static int netem_dump(struct Qdisc *sch, struct sk_buff *skb)
{
	const struct netem_sched_data *q = qdisc_priv(sch);
	struct nlattr *nla = (struct nlattr *) skb_tail_pointer(skb);
	struct tc_netem_qopt qopt;
	struct tc_netem_corr cor;
	struct tc_netem_reorder reorder;
	struct tc_netem_corrupt corrupt;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct tc_netem_rate rate;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct tc_netem_rate rate;
>>>>>>> refs/remotes/origin/master

	qopt.latency = q->latency;
	qopt.jitter = q->jitter;
	qopt.limit = q->limit;
	qopt.loss = q->loss;
	qopt.gap = q->gap;
	qopt.duplicate = q->duplicate;
<<<<<<< HEAD
	NLA_PUT(skb, TCA_OPTIONS, sizeof(qopt), &qopt);
=======
	if (nla_put(skb, TCA_OPTIONS, sizeof(qopt), &qopt))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master

	cor.delay_corr = q->delay_cor.rho;
	cor.loss_corr = q->loss_cor.rho;
	cor.dup_corr = q->dup_cor.rho;
<<<<<<< HEAD
	NLA_PUT(skb, TCA_NETEM_CORR, sizeof(cor), &cor);

	reorder.probability = q->reorder;
	reorder.correlation = q->reorder_cor.rho;
	NLA_PUT(skb, TCA_NETEM_REORDER, sizeof(reorder), &reorder);

	corrupt.probability = q->corrupt;
	corrupt.correlation = q->corrupt_cor.rho;
	NLA_PUT(skb, TCA_NETEM_CORRUPT, sizeof(corrupt), &corrupt);

<<<<<<< HEAD
=======
=======
	if (nla_put(skb, TCA_NETEM_CORR, sizeof(cor), &cor))
		goto nla_put_failure;

	reorder.probability = q->reorder;
	reorder.correlation = q->reorder_cor.rho;
	if (nla_put(skb, TCA_NETEM_REORDER, sizeof(reorder), &reorder))
		goto nla_put_failure;

	corrupt.probability = q->corrupt;
	corrupt.correlation = q->corrupt_cor.rho;
	if (nla_put(skb, TCA_NETEM_CORRUPT, sizeof(corrupt), &corrupt))
		goto nla_put_failure;

>>>>>>> refs/remotes/origin/master
	rate.rate = q->rate;
	rate.packet_overhead = q->packet_overhead;
	rate.cell_size = q->cell_size;
	rate.cell_overhead = q->cell_overhead;
<<<<<<< HEAD
	NLA_PUT(skb, TCA_NETEM_RATE, sizeof(rate), &rate);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (nla_put(skb, TCA_NETEM_RATE, sizeof(rate), &rate))
		goto nla_put_failure;

	if (q->ecn && nla_put_u32(skb, TCA_NETEM_ECN, q->ecn))
		goto nla_put_failure;

>>>>>>> refs/remotes/origin/master
	if (dump_loss_model(q, skb) != 0)
		goto nla_put_failure;

	return nla_nest_end(skb, nla);

nla_put_failure:
	nlmsg_trim(skb, nla);
	return -1;
}

static int netem_dump_class(struct Qdisc *sch, unsigned long cl,
			  struct sk_buff *skb, struct tcmsg *tcm)
{
	struct netem_sched_data *q = qdisc_priv(sch);

<<<<<<< HEAD
<<<<<<< HEAD
	if (cl != 1) 	/* only one class */
=======
	if (cl != 1 || !q->qdisc) 	/* only one class */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (cl != 1 || !q->qdisc) 	/* only one class */
>>>>>>> refs/remotes/origin/master
		return -ENOENT;

	tcm->tcm_handle |= TC_H_MIN(1);
	tcm->tcm_info = q->qdisc->handle;

	return 0;
}

static int netem_graft(struct Qdisc *sch, unsigned long arg, struct Qdisc *new,
		     struct Qdisc **old)
{
	struct netem_sched_data *q = qdisc_priv(sch);

<<<<<<< HEAD
<<<<<<< HEAD
	if (new == NULL)
		new = &noop_qdisc;

	sch_tree_lock(sch);
	*old = q->qdisc;
	q->qdisc = new;
	qdisc_tree_decrease_qlen(*old, (*old)->q.qlen);
	qdisc_reset(*old);
=======
=======
>>>>>>> refs/remotes/origin/master
	sch_tree_lock(sch);
	*old = q->qdisc;
	q->qdisc = new;
	if (*old) {
		qdisc_tree_decrease_qlen(*old, (*old)->q.qlen);
		qdisc_reset(*old);
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	sch_tree_unlock(sch);

	return 0;
}

static struct Qdisc *netem_leaf(struct Qdisc *sch, unsigned long arg)
{
	struct netem_sched_data *q = qdisc_priv(sch);
	return q->qdisc;
}

static unsigned long netem_get(struct Qdisc *sch, u32 classid)
{
	return 1;
}

static void netem_put(struct Qdisc *sch, unsigned long arg)
{
}

static void netem_walk(struct Qdisc *sch, struct qdisc_walker *walker)
{
	if (!walker->stop) {
		if (walker->count >= walker->skip)
			if (walker->fn(sch, 1, walker) < 0) {
				walker->stop = 1;
				return;
			}
		walker->count++;
	}
}

static const struct Qdisc_class_ops netem_class_ops = {
	.graft		=	netem_graft,
	.leaf		=	netem_leaf,
	.get		=	netem_get,
	.put		=	netem_put,
	.walk		=	netem_walk,
	.dump		=	netem_dump_class,
};

static struct Qdisc_ops netem_qdisc_ops __read_mostly = {
	.id		=	"netem",
	.cl_ops		=	&netem_class_ops,
	.priv_size	=	sizeof(struct netem_sched_data),
	.enqueue	=	netem_enqueue,
	.dequeue	=	netem_dequeue,
	.peek		=	qdisc_peek_dequeued,
	.drop		=	netem_drop,
	.init		=	netem_init,
	.reset		=	netem_reset,
	.destroy	=	netem_destroy,
	.change		=	netem_change,
	.dump		=	netem_dump,
	.owner		=	THIS_MODULE,
};


static int __init netem_module_init(void)
{
	pr_info("netem: version " VERSION "\n");
	return register_qdisc(&netem_qdisc_ops);
}
static void __exit netem_module_exit(void)
{
	unregister_qdisc(&netem_qdisc_ops);
}
module_init(netem_module_init)
module_exit(netem_module_exit)
MODULE_LICENSE("GPL");
