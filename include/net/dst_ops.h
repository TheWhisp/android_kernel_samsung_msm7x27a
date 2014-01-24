#ifndef _NET_DST_OPS_H
#define _NET_DST_OPS_H
#include <linux/types.h>
#include <linux/percpu_counter.h>
#include <linux/cache.h>

struct dst_entry;
struct kmem_cachep;
struct net_device;
struct sk_buff;
<<<<<<< HEAD
=======
struct sock;
>>>>>>> refs/remotes/origin/master

struct dst_ops {
	unsigned short		family;
	__be16			protocol;
<<<<<<< HEAD
	unsigned		gc_thresh;
=======
	unsigned int		gc_thresh;
>>>>>>> refs/remotes/origin/master

	int			(*gc)(struct dst_ops *ops);
	struct dst_entry *	(*check)(struct dst_entry *, __u32 cookie);
	unsigned int		(*default_advmss)(const struct dst_entry *);
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned int		(*default_mtu)(const struct dst_entry *);
=======
	unsigned int		(*mtu)(const struct dst_entry *);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned int		(*mtu)(const struct dst_entry *);
>>>>>>> refs/remotes/origin/master
	u32 *			(*cow_metrics)(struct dst_entry *, unsigned long);
	void			(*destroy)(struct dst_entry *);
	void			(*ifdown)(struct dst_entry *,
					  struct net_device *dev, int how);
	struct dst_entry *	(*negative_advice)(struct dst_entry *);
	void			(*link_failure)(struct sk_buff *);
<<<<<<< HEAD
	void			(*update_pmtu)(struct dst_entry *dst, u32 mtu);
	int			(*local_out)(struct sk_buff *skb);
<<<<<<< HEAD
=======
	struct neighbour *	(*neigh_lookup)(const struct dst_entry *dst, const void *daddr);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	void			(*update_pmtu)(struct dst_entry *dst, struct sock *sk,
					       struct sk_buff *skb, u32 mtu);
	void			(*redirect)(struct dst_entry *dst, struct sock *sk,
					    struct sk_buff *skb);
	int			(*local_out)(struct sk_buff *skb);
	struct neighbour *	(*neigh_lookup)(const struct dst_entry *dst,
						struct sk_buff *skb,
						const void *daddr);
>>>>>>> refs/remotes/origin/master

	struct kmem_cache	*kmem_cachep;

	struct percpu_counter	pcpuc_entries ____cacheline_aligned_in_smp;
};

static inline int dst_entries_get_fast(struct dst_ops *dst)
{
	return percpu_counter_read_positive(&dst->pcpuc_entries);
}

static inline int dst_entries_get_slow(struct dst_ops *dst)
{
	int res;

	local_bh_disable();
	res = percpu_counter_sum_positive(&dst->pcpuc_entries);
	local_bh_enable();
	return res;
}

static inline void dst_entries_add(struct dst_ops *dst, int val)
{
	local_bh_disable();
	percpu_counter_add(&dst->pcpuc_entries, val);
	local_bh_enable();
}

static inline int dst_entries_init(struct dst_ops *dst)
{
	return percpu_counter_init(&dst->pcpuc_entries, 0);
}

static inline void dst_entries_destroy(struct dst_ops *dst)
{
	percpu_counter_destroy(&dst->pcpuc_entries);
}

#endif
