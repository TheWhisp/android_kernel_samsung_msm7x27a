/* Event cache for netfilter. */

<<<<<<< HEAD
/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2006 Netfilter Core Team <coreteam@netfilter.org>
 * (C) 2003,2004 USAGI/WIDE Project <http://www.linux-ipv6.org>
=======
/*
 * (C) 2005 Harald Welte <laforge@gnumonks.org>
 * (C) 2005 Patrick McHardy <kaber@trash.net>
 * (C) 2005-2006 Netfilter Core Team <coreteam@netfilter.org>
 * (C) 2005 USAGI/WIDE Project <http://www.linux-ipv6.org>
>>>>>>> refs/remotes/origin/master
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/types.h>
#include <linux/netfilter.h>
#include <linux/skbuff.h>
#include <linux/vmalloc.h>
#include <linux/stddef.h>
#include <linux/err.h>
#include <linux/percpu.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master

#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_core.h>
#include <net/netfilter/nf_conntrack_extend.h>

static DEFINE_MUTEX(nf_ct_ecache_mutex);

<<<<<<< HEAD
<<<<<<< HEAD
struct nf_ct_event_notifier __rcu *nf_conntrack_event_cb __read_mostly;
EXPORT_SYMBOL_GPL(nf_conntrack_event_cb);

struct nf_exp_event_notifier __rcu *nf_expect_event_cb __read_mostly;
EXPORT_SYMBOL_GPL(nf_expect_event_cb);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/* deliver cached events and clear cache entry - must be called with locally
 * disabled softirqs */
void nf_ct_deliver_cached_events(struct nf_conn *ct)
{
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long events;
	struct nf_ct_event_notifier *notify;
	struct nf_conntrack_ecache *e;

	rcu_read_lock();
	notify = rcu_dereference(nf_conntrack_event_cb);
=======
=======
>>>>>>> refs/remotes/origin/master
	struct net *net = nf_ct_net(ct);
	unsigned long events, missed;
	struct nf_ct_event_notifier *notify;
	struct nf_conntrack_ecache *e;
	struct nf_ct_event item;
	int ret;

	rcu_read_lock();
	notify = rcu_dereference(net->ct.nf_conntrack_event_cb);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (notify == NULL)
		goto out_unlock;

	e = nf_ct_ecache_find(ct);
	if (e == NULL)
		goto out_unlock;

	events = xchg(&e->cache, 0);

<<<<<<< HEAD
<<<<<<< HEAD
	if (nf_ct_is_confirmed(ct) && !nf_ct_is_dying(ct) && events) {
		struct nf_ct_event item = {
			.ct	= ct,
			.pid	= 0,
			.report	= 0
		};
		int ret;
		/* We make a copy of the missed event cache without taking
		 * the lock, thus we may send missed events twice. However,
		 * this does not harm and it happens very rarely. */
		unsigned long missed = e->missed;

		if (!((events | missed) & e->ctmask))
			goto out_unlock;

		ret = notify->fcn(events | missed, &item);
		if (unlikely(ret < 0 || missed)) {
			spin_lock_bh(&ct->lock);
			if (ret < 0)
				e->missed |= events;
			else
				e->missed &= ~missed;
			spin_unlock_bh(&ct->lock);
		} 
	}
=======
=======
>>>>>>> refs/remotes/origin/master
	if (!nf_ct_is_confirmed(ct) || nf_ct_is_dying(ct) || !events)
		goto out_unlock;

	/* We make a copy of the missed event cache without taking
	 * the lock, thus we may send missed events twice. However,
	 * this does not harm and it happens very rarely. */
	missed = e->missed;

	if (!((events | missed) & e->ctmask))
		goto out_unlock;

	item.ct = ct;
<<<<<<< HEAD
	item.pid = 0;
=======
	item.portid = 0;
>>>>>>> refs/remotes/origin/master
	item.report = 0;

	ret = notify->fcn(events | missed, &item);

	if (likely(ret >= 0 && !missed))
		goto out_unlock;

	spin_lock_bh(&ct->lock);
	if (ret < 0)
		e->missed |= events;
	else
		e->missed &= ~missed;
	spin_unlock_bh(&ct->lock);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

out_unlock:
	rcu_read_unlock();
}
EXPORT_SYMBOL_GPL(nf_ct_deliver_cached_events);

<<<<<<< HEAD
<<<<<<< HEAD
int nf_conntrack_register_notifier(struct nf_ct_event_notifier *new)
=======
int nf_conntrack_register_notifier(struct net *net,
				   struct nf_ct_event_notifier *new)
>>>>>>> refs/remotes/origin/cm-10.0
{
	int ret = 0;
	struct nf_ct_event_notifier *notify;

	mutex_lock(&nf_ct_ecache_mutex);
<<<<<<< HEAD
	notify = rcu_dereference_protected(nf_conntrack_event_cb,
=======
	notify = rcu_dereference_protected(net->ct.nf_conntrack_event_cb,
>>>>>>> refs/remotes/origin/cm-10.0
=======
int nf_conntrack_register_notifier(struct net *net,
				   struct nf_ct_event_notifier *new)
{
	int ret;
	struct nf_ct_event_notifier *notify;

	mutex_lock(&nf_ct_ecache_mutex);
	notify = rcu_dereference_protected(net->ct.nf_conntrack_event_cb,
>>>>>>> refs/remotes/origin/master
					   lockdep_is_held(&nf_ct_ecache_mutex));
	if (notify != NULL) {
		ret = -EBUSY;
		goto out_unlock;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	rcu_assign_pointer(nf_conntrack_event_cb, new);
=======
	rcu_assign_pointer(net->ct.nf_conntrack_event_cb, new);
>>>>>>> refs/remotes/origin/cm-10.0
	mutex_unlock(&nf_ct_ecache_mutex);
	return ret;
=======
	rcu_assign_pointer(net->ct.nf_conntrack_event_cb, new);
	ret = 0;
>>>>>>> refs/remotes/origin/master

out_unlock:
	mutex_unlock(&nf_ct_ecache_mutex);
	return ret;
}
EXPORT_SYMBOL_GPL(nf_conntrack_register_notifier);

<<<<<<< HEAD
<<<<<<< HEAD
void nf_conntrack_unregister_notifier(struct nf_ct_event_notifier *new)
=======
void nf_conntrack_unregister_notifier(struct net *net,
				      struct nf_ct_event_notifier *new)
>>>>>>> refs/remotes/origin/cm-10.0
=======
void nf_conntrack_unregister_notifier(struct net *net,
				      struct nf_ct_event_notifier *new)
>>>>>>> refs/remotes/origin/master
{
	struct nf_ct_event_notifier *notify;

	mutex_lock(&nf_ct_ecache_mutex);
<<<<<<< HEAD
<<<<<<< HEAD
	notify = rcu_dereference_protected(nf_conntrack_event_cb,
					   lockdep_is_held(&nf_ct_ecache_mutex));
	BUG_ON(notify != new);
	rcu_assign_pointer(nf_conntrack_event_cb, NULL);
=======
=======
>>>>>>> refs/remotes/origin/master
	notify = rcu_dereference_protected(net->ct.nf_conntrack_event_cb,
					   lockdep_is_held(&nf_ct_ecache_mutex));
	BUG_ON(notify != new);
	RCU_INIT_POINTER(net->ct.nf_conntrack_event_cb, NULL);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	mutex_unlock(&nf_ct_ecache_mutex);
}
EXPORT_SYMBOL_GPL(nf_conntrack_unregister_notifier);

<<<<<<< HEAD
<<<<<<< HEAD
int nf_ct_expect_register_notifier(struct nf_exp_event_notifier *new)
=======
int nf_ct_expect_register_notifier(struct net *net,
				   struct nf_exp_event_notifier *new)
>>>>>>> refs/remotes/origin/cm-10.0
{
	int ret = 0;
	struct nf_exp_event_notifier *notify;

	mutex_lock(&nf_ct_ecache_mutex);
<<<<<<< HEAD
	notify = rcu_dereference_protected(nf_expect_event_cb,
=======
	notify = rcu_dereference_protected(net->ct.nf_expect_event_cb,
>>>>>>> refs/remotes/origin/cm-10.0
=======
int nf_ct_expect_register_notifier(struct net *net,
				   struct nf_exp_event_notifier *new)
{
	int ret;
	struct nf_exp_event_notifier *notify;

	mutex_lock(&nf_ct_ecache_mutex);
	notify = rcu_dereference_protected(net->ct.nf_expect_event_cb,
>>>>>>> refs/remotes/origin/master
					   lockdep_is_held(&nf_ct_ecache_mutex));
	if (notify != NULL) {
		ret = -EBUSY;
		goto out_unlock;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	rcu_assign_pointer(nf_expect_event_cb, new);
=======
	rcu_assign_pointer(net->ct.nf_expect_event_cb, new);
>>>>>>> refs/remotes/origin/cm-10.0
	mutex_unlock(&nf_ct_ecache_mutex);
	return ret;
=======
	rcu_assign_pointer(net->ct.nf_expect_event_cb, new);
	ret = 0;
>>>>>>> refs/remotes/origin/master

out_unlock:
	mutex_unlock(&nf_ct_ecache_mutex);
	return ret;
}
EXPORT_SYMBOL_GPL(nf_ct_expect_register_notifier);

<<<<<<< HEAD
<<<<<<< HEAD
void nf_ct_expect_unregister_notifier(struct nf_exp_event_notifier *new)
=======
void nf_ct_expect_unregister_notifier(struct net *net,
				      struct nf_exp_event_notifier *new)
>>>>>>> refs/remotes/origin/cm-10.0
=======
void nf_ct_expect_unregister_notifier(struct net *net,
				      struct nf_exp_event_notifier *new)
>>>>>>> refs/remotes/origin/master
{
	struct nf_exp_event_notifier *notify;

	mutex_lock(&nf_ct_ecache_mutex);
<<<<<<< HEAD
<<<<<<< HEAD
	notify = rcu_dereference_protected(nf_expect_event_cb,
					   lockdep_is_held(&nf_ct_ecache_mutex));
	BUG_ON(notify != new);
	rcu_assign_pointer(nf_expect_event_cb, NULL);
=======
=======
>>>>>>> refs/remotes/origin/master
	notify = rcu_dereference_protected(net->ct.nf_expect_event_cb,
					   lockdep_is_held(&nf_ct_ecache_mutex));
	BUG_ON(notify != new);
	RCU_INIT_POINTER(net->ct.nf_expect_event_cb, NULL);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	mutex_unlock(&nf_ct_ecache_mutex);
}
EXPORT_SYMBOL_GPL(nf_ct_expect_unregister_notifier);

#define NF_CT_EVENTS_DEFAULT 1
static int nf_ct_events __read_mostly = NF_CT_EVENTS_DEFAULT;
static int nf_ct_events_retry_timeout __read_mostly = 15*HZ;

#ifdef CONFIG_SYSCTL
static struct ctl_table event_sysctl_table[] = {
	{
		.procname	= "nf_conntrack_events",
		.data		= &init_net.ct.sysctl_events,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "nf_conntrack_events_retry_timeout",
		.data		= &init_net.ct.sysctl_events_retry_timeout,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{}
};
#endif /* CONFIG_SYSCTL */

static struct nf_ct_ext_type event_extend __read_mostly = {
	.len	= sizeof(struct nf_conntrack_ecache),
	.align	= __alignof__(struct nf_conntrack_ecache),
	.id	= NF_CT_EXT_ECACHE,
};

#ifdef CONFIG_SYSCTL
static int nf_conntrack_event_init_sysctl(struct net *net)
{
	struct ctl_table *table;

	table = kmemdup(event_sysctl_table, sizeof(event_sysctl_table),
			GFP_KERNEL);
	if (!table)
		goto out;

	table[0].data = &net->ct.sysctl_events;
	table[1].data = &net->ct.sysctl_events_retry_timeout;

<<<<<<< HEAD
	net->ct.event_sysctl_header =
		register_net_sysctl_table(net,
					  nf_net_netfilter_sysctl_path, table);
=======
	/* Don't export sysctls to unprivileged users */
	if (net->user_ns != &init_user_ns)
		table[0].procname = NULL;

	net->ct.event_sysctl_header =
		register_net_sysctl(net, "net/netfilter", table);
>>>>>>> refs/remotes/origin/master
	if (!net->ct.event_sysctl_header) {
		printk(KERN_ERR "nf_ct_event: can't register to sysctl.\n");
		goto out_register;
	}
	return 0;

out_register:
	kfree(table);
out:
	return -ENOMEM;
}

static void nf_conntrack_event_fini_sysctl(struct net *net)
{
	struct ctl_table *table;

	table = net->ct.event_sysctl_header->ctl_table_arg;
	unregister_net_sysctl_table(net->ct.event_sysctl_header);
	kfree(table);
}
#else
static int nf_conntrack_event_init_sysctl(struct net *net)
{
	return 0;
}

static void nf_conntrack_event_fini_sysctl(struct net *net)
{
}
#endif /* CONFIG_SYSCTL */

<<<<<<< HEAD
int nf_conntrack_ecache_init(struct net *net)
{
	int ret;

	net->ct.sysctl_events = nf_ct_events;
	net->ct.sysctl_events_retry_timeout = nf_ct_events_retry_timeout;

	if (net_eq(net, &init_net)) {
		ret = nf_ct_extend_register(&event_extend);
		if (ret < 0) {
			printk(KERN_ERR "nf_ct_event: Unable to register "
					"event extension.\n");
			goto out_extend_register;
		}
	}

	ret = nf_conntrack_event_init_sysctl(net);
	if (ret < 0)
		goto out_sysctl;

	return 0;

out_sysctl:
	if (net_eq(net, &init_net))
		nf_ct_extend_unregister(&event_extend);
out_extend_register:
	return ret;
}

void nf_conntrack_ecache_fini(struct net *net)
{
	nf_conntrack_event_fini_sysctl(net);
	if (net_eq(net, &init_net))
		nf_ct_extend_unregister(&event_extend);
=======
int nf_conntrack_ecache_pernet_init(struct net *net)
{
	net->ct.sysctl_events = nf_ct_events;
	net->ct.sysctl_events_retry_timeout = nf_ct_events_retry_timeout;
	return nf_conntrack_event_init_sysctl(net);
}

void nf_conntrack_ecache_pernet_fini(struct net *net)
{
	nf_conntrack_event_fini_sysctl(net);
}

int nf_conntrack_ecache_init(void)
{
	int ret = nf_ct_extend_register(&event_extend);
	if (ret < 0)
		pr_err("nf_ct_event: Unable to register event extension.\n");
	return ret;
}

void nf_conntrack_ecache_fini(void)
{
	nf_ct_extend_unregister(&event_extend);
>>>>>>> refs/remotes/origin/master
}
