/*
 * IPVS:        Weighted Round-Robin Scheduling module
 *
 * Authors:     Wensong Zhang <wensong@linuxvirtualserver.org>
 *
 *              This program is free software; you can redistribute it and/or
 *              modify it under the terms of the GNU General Public License
 *              as published by the Free Software Foundation; either version
 *              2 of the License, or (at your option) any later version.
 *
 * Changes:
 *     Wensong Zhang            :     changed the ip_vs_wrr_schedule to return dest
 *     Wensong Zhang            :     changed some comestics things for debugging
 *     Wensong Zhang            :     changed for the d-linked destination list
 *     Wensong Zhang            :     added the ip_vs_wrr_update_svc
 *     Julian Anastasov         :     fixed the bug of returning destination
 *                                    with weight 0 when all weights are zero
 *
 */

#define KMSG_COMPONENT "IPVS"
#define pr_fmt(fmt) KMSG_COMPONENT ": " fmt

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/net.h>
#include <linux/gcd.h>

#include <net/ip_vs.h>

<<<<<<< HEAD
=======
/* The WRR algorithm depends on some caclulations:
 * - mw: maximum weight
 * - di: weight step, greatest common divisor from all weights
 * - cw: current required weight
 * As result, all weights are in the [di..mw] range with a step=di.
 *
 * First, we start with cw = mw and select dests with weight >= cw.
 * Then cw is reduced with di and all dests are checked again.
 * Last pass should be with cw = di. We have mw/di passes in total:
 *
 * pass 1: cw = max weight
 * pass 2: cw = max weight - di
 * pass 3: cw = max weight - 2 * di
 * ...
 * last pass: cw = di
 *
 * Weights are supposed to be >= di but we run in parallel with
 * weight changes, it is possible some dest weight to be reduced
 * below di, bad if it is the only available dest.
 *
 * So, we modify how mw is calculated, now it is reduced with (di - 1),
 * so that last cw is 1 to catch such dests with weight below di:
 * pass 1: cw = max weight - (di - 1)
 * pass 2: cw = max weight - di - (di - 1)
 * pass 3: cw = max weight - 2 * di - (di - 1)
 * ...
 * last pass: cw = 1
 *
 */

>>>>>>> refs/remotes/origin/master
/*
 * current destination pointer for weighted round-robin scheduling
 */
struct ip_vs_wrr_mark {
<<<<<<< HEAD
	struct list_head *cl;	/* current list head */
	int cw;			/* current weight */
	int mw;			/* maximum weight */
	int di;			/* decreasing interval */
=======
	struct ip_vs_dest *cl;	/* current dest or head */
	int cw;			/* current weight */
	int mw;			/* maximum weight */
	int di;			/* decreasing interval */
	struct rcu_head		rcu_head;
>>>>>>> refs/remotes/origin/master
};


static int ip_vs_wrr_gcd_weight(struct ip_vs_service *svc)
{
	struct ip_vs_dest *dest;
	int weight;
	int g = 0;

	list_for_each_entry(dest, &svc->destinations, n_list) {
		weight = atomic_read(&dest->weight);
		if (weight > 0) {
			if (g > 0)
				g = gcd(weight, g);
			else
				g = weight;
		}
	}
	return g ? g : 1;
}


/*
 *    Get the maximum weight of the service destinations.
 */
static int ip_vs_wrr_max_weight(struct ip_vs_service *svc)
{
	struct ip_vs_dest *dest;
	int new_weight, weight = 0;

	list_for_each_entry(dest, &svc->destinations, n_list) {
		new_weight = atomic_read(&dest->weight);
		if (new_weight > weight)
			weight = new_weight;
	}

	return weight;
}


static int ip_vs_wrr_init_svc(struct ip_vs_service *svc)
{
	struct ip_vs_wrr_mark *mark;

	/*
	 *    Allocate the mark variable for WRR scheduling
	 */
<<<<<<< HEAD
	mark = kmalloc(sizeof(struct ip_vs_wrr_mark), GFP_ATOMIC);
<<<<<<< HEAD
	if (mark == NULL) {
		pr_err("%s(): no memory\n", __func__);
		return -ENOMEM;
	}
=======
	if (mark == NULL)
		return -ENOMEM;

>>>>>>> refs/remotes/origin/cm-10.0
	mark->cl = &svc->destinations;
	mark->cw = 0;
	mark->mw = ip_vs_wrr_max_weight(svc);
	mark->di = ip_vs_wrr_gcd_weight(svc);
=======
	mark = kmalloc(sizeof(struct ip_vs_wrr_mark), GFP_KERNEL);
	if (mark == NULL)
		return -ENOMEM;

	mark->cl = list_entry(&svc->destinations, struct ip_vs_dest, n_list);
	mark->di = ip_vs_wrr_gcd_weight(svc);
	mark->mw = ip_vs_wrr_max_weight(svc) - (mark->di - 1);
	mark->cw = mark->mw;
>>>>>>> refs/remotes/origin/master
	svc->sched_data = mark;

	return 0;
}


<<<<<<< HEAD
static int ip_vs_wrr_done_svc(struct ip_vs_service *svc)
{
	/*
	 *    Release the mark variable
	 */
	kfree(svc->sched_data);

	return 0;
}


static int ip_vs_wrr_update_svc(struct ip_vs_service *svc)
{
	struct ip_vs_wrr_mark *mark = svc->sched_data;

	mark->cl = &svc->destinations;
	mark->mw = ip_vs_wrr_max_weight(svc);
	mark->di = ip_vs_wrr_gcd_weight(svc);
	if (mark->cw > mark->mw)
		mark->cw = 0;
=======
static void ip_vs_wrr_done_svc(struct ip_vs_service *svc)
{
	struct ip_vs_wrr_mark *mark = svc->sched_data;

	/*
	 *    Release the mark variable
	 */
	kfree_rcu(mark, rcu_head);
}


static int ip_vs_wrr_dest_changed(struct ip_vs_service *svc,
				  struct ip_vs_dest *dest)
{
	struct ip_vs_wrr_mark *mark = svc->sched_data;

	spin_lock_bh(&svc->sched_lock);
	mark->cl = list_entry(&svc->destinations, struct ip_vs_dest, n_list);
	mark->di = ip_vs_wrr_gcd_weight(svc);
	mark->mw = ip_vs_wrr_max_weight(svc) - (mark->di - 1);
	if (mark->cw > mark->mw || !mark->cw)
		mark->cw = mark->mw;
	else if (mark->di > 1)
		mark->cw = (mark->cw / mark->di) * mark->di + 1;
	spin_unlock_bh(&svc->sched_lock);
>>>>>>> refs/remotes/origin/master
	return 0;
}


/*
 *    Weighted Round-Robin Scheduling
 */
static struct ip_vs_dest *
<<<<<<< HEAD
ip_vs_wrr_schedule(struct ip_vs_service *svc, const struct sk_buff *skb)
{
	struct ip_vs_dest *dest;
	struct ip_vs_wrr_mark *mark = svc->sched_data;
	struct list_head *p;

	IP_VS_DBG(6, "%s(): Scheduling...\n", __func__);

	/*
	 * This loop will always terminate, because mark->cw in (0, max_weight]
	 * and at least one server has its weight equal to max_weight.
	 */
	write_lock(&svc->sched_lock);
	p = mark->cl;
	while (1) {
		if (mark->cl == &svc->destinations) {
			/* it is at the head of the destination list */

			if (mark->cl == mark->cl->next) {
				/* no dest entry */
				ip_vs_scheduler_err(svc,
					"no destination available: "
					"no destinations present");
				dest = NULL;
				goto out;
			}

			mark->cl = svc->destinations.next;
			mark->cw -= mark->di;
			if (mark->cw <= 0) {
				mark->cw = mark->mw;
				/*
				 * Still zero, which means no available servers.
				 */
				if (mark->cw == 0) {
					mark->cl = &svc->destinations;
					ip_vs_scheduler_err(svc,
						"no destination available");
					dest = NULL;
					goto out;
				}
			}
		} else
			mark->cl = mark->cl->next;

		if (mark->cl != &svc->destinations) {
			/* not at the head of the list */
			dest = list_entry(mark->cl, struct ip_vs_dest, n_list);
			if (!(dest->flags & IP_VS_DEST_F_OVERLOAD) &&
			    atomic_read(&dest->weight) >= mark->cw) {
				/* got it */
				break;
			}
		}

		if (mark->cl == p && mark->cw == mark->di) {
			/* back to the start, and no dest is found.
			   It is only possible when all dests are OVERLOADED */
			dest = NULL;
			ip_vs_scheduler_err(svc,
				"no destination available: "
				"all destinations are overloaded");
			goto out;
		}
	}

=======
ip_vs_wrr_schedule(struct ip_vs_service *svc, const struct sk_buff *skb,
		   struct ip_vs_iphdr *iph)
{
	struct ip_vs_dest *dest, *last, *stop = NULL;
	struct ip_vs_wrr_mark *mark = svc->sched_data;
	bool last_pass = false, restarted = false;

	IP_VS_DBG(6, "%s(): Scheduling...\n", __func__);

	spin_lock_bh(&svc->sched_lock);
	dest = mark->cl;
	/* No available dests? */
	if (mark->mw == 0)
		goto err_noavail;
	last = dest;
	/* Stop only after all dests were checked for weight >= 1 (last pass) */
	while (1) {
		list_for_each_entry_continue_rcu(dest,
						 &svc->destinations,
						 n_list) {
			if (!(dest->flags & IP_VS_DEST_F_OVERLOAD) &&
			    atomic_read(&dest->weight) >= mark->cw)
				goto found;
			if (dest == stop)
				goto err_over;
		}
		mark->cw -= mark->di;
		if (mark->cw <= 0) {
			mark->cw = mark->mw;
			/* Stop if we tried last pass from first dest:
			 * 1. last_pass: we started checks when cw > di but
			 *	then all dests were checked for w >= 1
			 * 2. last was head: the first and only traversal
			 *	was for weight >= 1, for all dests.
			 */
			if (last_pass ||
			    &last->n_list == &svc->destinations)
				goto err_over;
			restarted = true;
		}
		last_pass = mark->cw <= mark->di;
		if (last_pass && restarted &&
		    &last->n_list != &svc->destinations) {
			/* First traversal was for w >= 1 but only
			 * for dests after 'last', now do the same
			 * for all dests up to 'last'.
			 */
			stop = last;
		}
	}

found:
>>>>>>> refs/remotes/origin/master
	IP_VS_DBG_BUF(6, "WRR: server %s:%u "
		      "activeconns %d refcnt %d weight %d\n",
		      IP_VS_DBG_ADDR(svc->af, &dest->addr), ntohs(dest->port),
		      atomic_read(&dest->activeconns),
		      atomic_read(&dest->refcnt),
		      atomic_read(&dest->weight));
<<<<<<< HEAD

  out:
	write_unlock(&svc->sched_lock);
	return dest;
=======
	mark->cl = dest;

  out:
	spin_unlock_bh(&svc->sched_lock);
	return dest;

err_noavail:
	mark->cl = dest;
	dest = NULL;
	ip_vs_scheduler_err(svc, "no destination available");
	goto out;

err_over:
	mark->cl = dest;
	dest = NULL;
	ip_vs_scheduler_err(svc, "no destination available: "
			    "all destinations are overloaded");
	goto out;
>>>>>>> refs/remotes/origin/master
}


static struct ip_vs_scheduler ip_vs_wrr_scheduler = {
	.name =			"wrr",
	.refcnt =		ATOMIC_INIT(0),
	.module =		THIS_MODULE,
	.n_list =		LIST_HEAD_INIT(ip_vs_wrr_scheduler.n_list),
	.init_service =		ip_vs_wrr_init_svc,
	.done_service =		ip_vs_wrr_done_svc,
<<<<<<< HEAD
	.update_service =	ip_vs_wrr_update_svc,
=======
	.add_dest =		ip_vs_wrr_dest_changed,
	.del_dest =		ip_vs_wrr_dest_changed,
	.upd_dest =		ip_vs_wrr_dest_changed,
>>>>>>> refs/remotes/origin/master
	.schedule =		ip_vs_wrr_schedule,
};

static int __init ip_vs_wrr_init(void)
{
	return register_ip_vs_scheduler(&ip_vs_wrr_scheduler) ;
}

static void __exit ip_vs_wrr_cleanup(void)
{
	unregister_ip_vs_scheduler(&ip_vs_wrr_scheduler);
<<<<<<< HEAD
=======
	synchronize_rcu();
>>>>>>> refs/remotes/origin/master
}

module_init(ip_vs_wrr_init);
module_exit(ip_vs_wrr_cleanup);
MODULE_LICENSE("GPL");
