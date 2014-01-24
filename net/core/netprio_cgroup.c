/*
 * net/core/netprio_cgroup.c	Priority Control Group
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * Authors:	Neil Horman <nhorman@tuxdriver.com>
 */

<<<<<<< HEAD
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> refs/remotes/origin/master
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/skbuff.h>
#include <linux/cgroup.h>
#include <linux/rcupdate.h>
#include <linux/atomic.h>
#include <net/rtnetlink.h>
#include <net/pkt_cls.h>
#include <net/sock.h>
#include <net/netprio_cgroup.h>

<<<<<<< HEAD
static struct cgroup_subsys_state *cgrp_create(struct cgroup *cgrp);
static void cgrp_destroy(struct cgroup *cgrp);
static int cgrp_populate(struct cgroup_subsys *ss, struct cgroup *cgrp);

struct cgroup_subsys net_prio_subsys = {
	.name		= "net_prio",
	.create		= cgrp_create,
	.destroy	= cgrp_destroy,
	.populate	= cgrp_populate,
#ifdef CONFIG_NETPRIO_CGROUP
	.subsys_id	= net_prio_subsys_id,
#endif
	.module		= THIS_MODULE
};

#define PRIOIDX_SZ 128

static unsigned long prioidx_map[PRIOIDX_SZ];
static DEFINE_SPINLOCK(prioidx_map_lock);
static atomic_t max_prioidx = ATOMIC_INIT(0);

static inline struct cgroup_netprio_state *cgrp_netprio_state(struct cgroup *cgrp)
{
	return container_of(cgroup_subsys_state(cgrp, net_prio_subsys_id),
			    struct cgroup_netprio_state, css);
}

static int get_prioidx(u32 *prio)
{
	unsigned long flags;
	u32 prioidx;

	spin_lock_irqsave(&prioidx_map_lock, flags);
	prioidx = find_first_zero_bit(prioidx_map, sizeof(unsigned long) * PRIOIDX_SZ);
	if (prioidx == sizeof(unsigned long) * PRIOIDX_SZ) {
		spin_unlock_irqrestore(&prioidx_map_lock, flags);
		return -ENOSPC;
	}
	set_bit(prioidx, prioidx_map);
	spin_unlock_irqrestore(&prioidx_map_lock, flags);
	atomic_set(&max_prioidx, prioidx);
	*prio = prioidx;
	return 0;
}

static void put_prioidx(u32 idx)
{
	unsigned long flags;

	spin_lock_irqsave(&prioidx_map_lock, flags);
	clear_bit(idx, prioidx_map);
	spin_unlock_irqrestore(&prioidx_map_lock, flags);
}

static void extend_netdev_table(struct net_device *dev, u32 new_len)
{
	size_t new_size = sizeof(struct netprio_map) +
			   ((sizeof(u32) * new_len));
	struct netprio_map *new_priomap = kzalloc(new_size, GFP_KERNEL);
	struct netprio_map *old_priomap;
	int i;

	old_priomap  = rtnl_dereference(dev->priomap);

	if (!new_priomap) {
		printk(KERN_WARNING "Unable to alloc new priomap!\n");
		return;
	}

	for (i = 0;
	     old_priomap && (i < old_priomap->priomap_len);
	     i++)
		new_priomap->priomap[i] = old_priomap->priomap[i];

	new_priomap->priomap_len = new_len;

	rcu_assign_pointer(dev->priomap, new_priomap);
	if (old_priomap)
		kfree_rcu(old_priomap, rcu);
}

static void update_netdev_tables(void)
{
	struct net_device *dev;
	u32 max_len = atomic_read(&max_prioidx) + 1;
	struct netprio_map *map;

	rtnl_lock();
	for_each_netdev(&init_net, dev) {
		map = rtnl_dereference(dev->priomap);
		if ((!map) ||
		    (map->priomap_len < max_len))
			extend_netdev_table(dev, max_len);
	}
	rtnl_unlock();
}

static struct cgroup_subsys_state *cgrp_create(struct cgroup *cgrp)
{
	struct cgroup_netprio_state *cs;
	int ret;

	cs = kzalloc(sizeof(*cs), GFP_KERNEL);
	if (!cs)
		return ERR_PTR(-ENOMEM);

	if (cgrp->parent && cgrp_netprio_state(cgrp->parent)->prioidx) {
		kfree(cs);
		return ERR_PTR(-EINVAL);
	}

	ret = get_prioidx(&cs->prioidx);
	if (ret != 0) {
		printk(KERN_WARNING "No space in priority index array\n");
		kfree(cs);
		return ERR_PTR(ret);
	}

	return &cs->css;
}

static void cgrp_destroy(struct cgroup *cgrp)
{
	struct cgroup_netprio_state *cs;
	struct net_device *dev;
	struct netprio_map *map;

	cs = cgrp_netprio_state(cgrp);
	rtnl_lock();
	for_each_netdev(&init_net, dev) {
		map = rtnl_dereference(dev->priomap);
		if (map)
			map->priomap[cs->prioidx] = 0;
	}
	rtnl_unlock();
	put_prioidx(cs->prioidx);
	kfree(cs);
}

static u64 read_prioidx(struct cgroup *cgrp, struct cftype *cft)
{
	return (u64)cgrp_netprio_state(cgrp)->prioidx;
}

static int read_priomap(struct cgroup *cont, struct cftype *cft,
			struct cgroup_map_cb *cb)
{
	struct net_device *dev;
	u32 prioidx = cgrp_netprio_state(cont)->prioidx;
	u32 priority;
	struct netprio_map *map;

	rcu_read_lock();
	for_each_netdev_rcu(&init_net, dev) {
		map = rcu_dereference(dev->priomap);
		priority = map ? map->priomap[prioidx] : 0;
		cb->fill(cb, dev->name, priority);
	}
=======
#include <linux/fdtable.h>

#define PRIOMAP_MIN_SZ		128

/*
 * Extend @dev->priomap so that it's large enough to accomodate
 * @target_idx.  @dev->priomap.priomap_len > @target_idx after successful
 * return.  Must be called under rtnl lock.
 */
static int extend_netdev_table(struct net_device *dev, u32 target_idx)
{
	struct netprio_map *old, *new;
	size_t new_sz, new_len;

	/* is the existing priomap large enough? */
	old = rtnl_dereference(dev->priomap);
	if (old && old->priomap_len > target_idx)
		return 0;

	/*
	 * Determine the new size.  Let's keep it power-of-two.  We start
	 * from PRIOMAP_MIN_SZ and double it until it's large enough to
	 * accommodate @target_idx.
	 */
	new_sz = PRIOMAP_MIN_SZ;
	while (true) {
		new_len = (new_sz - offsetof(struct netprio_map, priomap)) /
			sizeof(new->priomap[0]);
		if (new_len > target_idx)
			break;
		new_sz *= 2;
		/* overflowed? */
		if (WARN_ON(new_sz < PRIOMAP_MIN_SZ))
			return -ENOSPC;
	}

	/* allocate & copy */
	new = kzalloc(new_sz, GFP_KERNEL);
	if (!new)
		return -ENOMEM;

	if (old)
		memcpy(new->priomap, old->priomap,
		       old->priomap_len * sizeof(old->priomap[0]));

	new->priomap_len = new_len;

	/* install the new priomap */
	rcu_assign_pointer(dev->priomap, new);
	if (old)
		kfree_rcu(old, rcu);
	return 0;
}

/**
 * netprio_prio - return the effective netprio of a cgroup-net_device pair
 * @css: css part of the target pair
 * @dev: net_device part of the target pair
 *
 * Should be called under RCU read or rtnl lock.
 */
static u32 netprio_prio(struct cgroup_subsys_state *css, struct net_device *dev)
{
	struct netprio_map *map = rcu_dereference_rtnl(dev->priomap);
	int id = css->cgroup->id;

	if (map && id < map->priomap_len)
		return map->priomap[id];
	return 0;
}

/**
 * netprio_set_prio - set netprio on a cgroup-net_device pair
 * @css: css part of the target pair
 * @dev: net_device part of the target pair
 * @prio: prio to set
 *
 * Set netprio to @prio on @css-@dev pair.  Should be called under rtnl
 * lock and may fail under memory pressure for non-zero @prio.
 */
static int netprio_set_prio(struct cgroup_subsys_state *css,
			    struct net_device *dev, u32 prio)
{
	struct netprio_map *map;
	int id = css->cgroup->id;
	int ret;

	/* avoid extending priomap for zero writes */
	map = rtnl_dereference(dev->priomap);
	if (!prio && (!map || map->priomap_len <= id))
		return 0;

	ret = extend_netdev_table(dev, id);
	if (ret)
		return ret;

	map = rtnl_dereference(dev->priomap);
	map->priomap[id] = prio;
	return 0;
}

static struct cgroup_subsys_state *
cgrp_css_alloc(struct cgroup_subsys_state *parent_css)
{
	struct cgroup_subsys_state *css;

	css = kzalloc(sizeof(*css), GFP_KERNEL);
	if (!css)
		return ERR_PTR(-ENOMEM);

	return css;
}

static int cgrp_css_online(struct cgroup_subsys_state *css)
{
	struct cgroup_subsys_state *parent_css = css_parent(css);
	struct net_device *dev;
	int ret = 0;

	if (!parent_css)
		return 0;

	rtnl_lock();
	/*
	 * Inherit prios from the parent.  As all prios are set during
	 * onlining, there is no need to clear them on offline.
	 */
	for_each_netdev(&init_net, dev) {
		u32 prio = netprio_prio(parent_css, dev);

		ret = netprio_set_prio(css, dev, prio);
		if (ret)
			break;
	}
	rtnl_unlock();
	return ret;
}

static void cgrp_css_free(struct cgroup_subsys_state *css)
{
	kfree(css);
}

static u64 read_prioidx(struct cgroup_subsys_state *css, struct cftype *cft)
{
	return css->cgroup->id;
}

static int read_priomap(struct seq_file *sf, void *v)
{
	struct net_device *dev;

	rcu_read_lock();
	for_each_netdev_rcu(&init_net, dev)
		seq_printf(sf, "%s %u\n", dev->name,
			   netprio_prio(seq_css(sf), dev));
>>>>>>> refs/remotes/origin/master
	rcu_read_unlock();
	return 0;
}

<<<<<<< HEAD
static int write_priomap(struct cgroup *cgrp, struct cftype *cft,
			 const char *buffer)
{
	char *devname = kstrdup(buffer, GFP_KERNEL);
	int ret = -EINVAL;
	u32 prioidx = cgrp_netprio_state(cgrp)->prioidx;
	unsigned long priority;
	char *priostr;
	struct net_device *dev;
	struct netprio_map *map;

	if (!devname)
		return -ENOMEM;

	/*
	 * Minimally sized valid priomap string
	 */
	if (strlen(devname) < 3)
		goto out_free_devname;

	priostr = strstr(devname, " ");
	if (!priostr)
		goto out_free_devname;

	/*
	 *Separate the devname from the associated priority
	 *and advance the priostr poitner to the priority value
	 */
	*priostr = '\0';
	priostr++;

	/*
	 * If the priostr points to NULL, we're at the end of the passed
	 * in string, and its not a valid write
	 */
	if (*priostr == '\0')
		goto out_free_devname;

	ret = kstrtoul(priostr, 10, &priority);
	if (ret < 0)
		goto out_free_devname;

	ret = -ENODEV;

	dev = dev_get_by_name(&init_net, devname);
	if (!dev)
		goto out_free_devname;

	update_netdev_tables();
	ret = 0;
	rcu_read_lock();
	map = rcu_dereference(dev->priomap);
	if (map)
		map->priomap[prioidx] = priority;
	rcu_read_unlock();
	dev_put(dev);

out_free_devname:
	kfree(devname);
	return ret;
=======
static int write_priomap(struct cgroup_subsys_state *css, struct cftype *cft,
			 const char *buffer)
{
	char devname[IFNAMSIZ + 1];
	struct net_device *dev;
	u32 prio;
	int ret;

	if (sscanf(buffer, "%"__stringify(IFNAMSIZ)"s %u", devname, &prio) != 2)
		return -EINVAL;

	dev = dev_get_by_name(&init_net, devname);
	if (!dev)
		return -ENODEV;

	rtnl_lock();

	ret = netprio_set_prio(css, dev, prio);

	rtnl_unlock();
	dev_put(dev);
	return ret;
}

static int update_netprio(const void *v, struct file *file, unsigned n)
{
	int err;
	struct socket *sock = sock_from_file(file, &err);
	if (sock)
		sock->sk->sk_cgrp_prioidx = (u32)(unsigned long)v;
	return 0;
}

static void net_prio_attach(struct cgroup_subsys_state *css,
			    struct cgroup_taskset *tset)
{
	struct task_struct *p;
	void *v = (void *)(unsigned long)css->cgroup->id;

	cgroup_taskset_for_each(p, css, tset) {
		task_lock(p);
		iterate_fd(p->files, 0, update_netprio, v);
		task_unlock(p);
	}
>>>>>>> refs/remotes/origin/master
}

static struct cftype ss_files[] = {
	{
		.name = "prioidx",
		.read_u64 = read_prioidx,
	},
	{
		.name = "ifpriomap",
<<<<<<< HEAD
		.read_map = read_priomap,
		.write_string = write_priomap,
	},
};

static int cgrp_populate(struct cgroup_subsys *ss, struct cgroup *cgrp)
{
	return cgroup_add_files(cgrp, ss, ss_files, ARRAY_SIZE(ss_files));
}
=======
		.seq_show = read_priomap,
		.write_string = write_priomap,
	},
	{ }	/* terminate */
};

struct cgroup_subsys net_prio_subsys = {
	.name		= "net_prio",
	.css_alloc	= cgrp_css_alloc,
	.css_online	= cgrp_css_online,
	.css_free	= cgrp_css_free,
	.attach		= net_prio_attach,
	.subsys_id	= net_prio_subsys_id,
	.base_cftypes	= ss_files,
	.module		= THIS_MODULE,
};
>>>>>>> refs/remotes/origin/master

static int netprio_device_event(struct notifier_block *unused,
				unsigned long event, void *ptr)
{
<<<<<<< HEAD
	struct net_device *dev = ptr;
=======
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);
>>>>>>> refs/remotes/origin/master
	struct netprio_map *old;

	/*
	 * Note this is called with rtnl_lock held so we have update side
	 * protection on our rcu assignments
	 */

	switch (event) {
	case NETDEV_UNREGISTER:
		old = rtnl_dereference(dev->priomap);
		RCU_INIT_POINTER(dev->priomap, NULL);
		if (old)
			kfree_rcu(old, rcu);
		break;
	}
	return NOTIFY_DONE;
}

static struct notifier_block netprio_device_notifier = {
	.notifier_call = netprio_device_event
};

static int __init init_cgroup_netprio(void)
{
	int ret;

	ret = cgroup_load_subsys(&net_prio_subsys);
	if (ret)
		goto out;
<<<<<<< HEAD
#ifndef CONFIG_NETPRIO_CGROUP
	smp_wmb();
	net_prio_subsys_id = net_prio_subsys.subsys_id;
#endif
=======
>>>>>>> refs/remotes/origin/master

	register_netdevice_notifier(&netprio_device_notifier);

out:
	return ret;
}

static void __exit exit_cgroup_netprio(void)
{
	struct netprio_map *old;
	struct net_device *dev;

	unregister_netdevice_notifier(&netprio_device_notifier);

	cgroup_unload_subsys(&net_prio_subsys);

<<<<<<< HEAD
#ifndef CONFIG_NETPRIO_CGROUP
	net_prio_subsys_id = -1;
	synchronize_rcu();
#endif

=======
>>>>>>> refs/remotes/origin/master
	rtnl_lock();
	for_each_netdev(&init_net, dev) {
		old = rtnl_dereference(dev->priomap);
		RCU_INIT_POINTER(dev->priomap, NULL);
		if (old)
			kfree_rcu(old, rcu);
	}
	rtnl_unlock();
}

module_init(init_cgroup_netprio);
module_exit(exit_cgroup_netprio);
MODULE_LICENSE("GPL v2");
