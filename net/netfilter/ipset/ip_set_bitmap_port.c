<<<<<<< HEAD
/* Copyright (C) 2003-2011 Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>
=======
/* Copyright (C) 2003-2013 Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>
>>>>>>> refs/remotes/origin/master
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/* Kernel module implementing an IP set type: the bitmap:port type */

#include <linux/module.h>
#include <linux/ip.h>
#include <linux/skbuff.h>
#include <linux/errno.h>
#include <linux/netlink.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <net/netlink.h>

#include <linux/netfilter/ipset/ip_set.h>
#include <linux/netfilter/ipset/ip_set_bitmap.h>
#include <linux/netfilter/ipset/ip_set_getport.h>
<<<<<<< HEAD
#define IP_SET_BITMAP_TIMEOUT
#include <linux/netfilter/ipset/ip_set_timeout.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>");
MODULE_DESCRIPTION("bitmap:port type of IP sets");
MODULE_ALIAS("ip_set_bitmap:port");

/* Type structure */
struct bitmap_port {
	void *members;		/* the set members */
	u16 first_port;		/* host byte order, included in range */
	u16 last_port;		/* host byte order, included in range */
	size_t memsize;		/* members size */
	u32 timeout;		/* timeout parameter */
	struct timer_list gc;	/* garbage collection */
};

/* Base variant */

static int
<<<<<<< HEAD
bitmap_port_test(struct ip_set *set, void *value, u32 timeout)
=======
bitmap_port_test(struct ip_set *set, void *value, u32 timeout, u32 flags)
>>>>>>> refs/remotes/origin/cm-10.0
{
	const struct bitmap_port *map = set->data;
	u16 id = *(u16 *)value;

	return !!test_bit(id, map->members);
}

static int
<<<<<<< HEAD
bitmap_port_add(struct ip_set *set, void *value, u32 timeout)
=======
bitmap_port_add(struct ip_set *set, void *value, u32 timeout, u32 flags)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct bitmap_port *map = set->data;
	u16 id = *(u16 *)value;

	if (test_and_set_bit(id, map->members))
		return -IPSET_ERR_EXIST;

	return 0;
}

static int
<<<<<<< HEAD
bitmap_port_del(struct ip_set *set, void *value, u32 timeout)
=======
bitmap_port_del(struct ip_set *set, void *value, u32 timeout, u32 flags)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct bitmap_port *map = set->data;
	u16 id = *(u16 *)value;

	if (!test_and_clear_bit(id, map->members))
		return -IPSET_ERR_EXIST;

	return 0;
}

static int
bitmap_port_list(const struct ip_set *set,
		 struct sk_buff *skb, struct netlink_callback *cb)
{
	const struct bitmap_port *map = set->data;
	struct nlattr *atd, *nested;
	u16 id, first = cb->args[2];
	u16 last = map->last_port - map->first_port;

	atd = ipset_nest_start(skb, IPSET_ATTR_ADT);
	if (!atd)
		return -EMSGSIZE;
	for (; cb->args[2] <= last; cb->args[2]++) {
		id = cb->args[2];
		if (!test_bit(id, map->members))
			continue;
		nested = ipset_nest_start(skb, IPSET_ATTR_DATA);
		if (!nested) {
			if (id == first) {
				nla_nest_cancel(skb, atd);
				return -EMSGSIZE;
			} else
				goto nla_put_failure;
		}
		NLA_PUT_NET16(skb, IPSET_ATTR_PORT,
			      htons(map->first_port + id));
		ipset_nest_end(skb, nested);
	}
	ipset_nest_end(skb, atd);
	/* Set listing finished */
	cb->args[2] = 0;

	return 0;

nla_put_failure:
	nla_nest_cancel(skb, nested);
	ipset_nest_end(skb, atd);
	if (unlikely(id == first)) {
		cb->args[2] = 0;
		return -EMSGSIZE;
	}
	return 0;
}

/* Timeout variant */

static int
<<<<<<< HEAD
bitmap_port_ttest(struct ip_set *set, void *value, u32 timeout)
=======
bitmap_port_ttest(struct ip_set *set, void *value, u32 timeout, u32 flags)
>>>>>>> refs/remotes/origin/cm-10.0
{
	const struct bitmap_port *map = set->data;
	const unsigned long *members = map->members;
	u16 id = *(u16 *)value;

	return ip_set_timeout_test(members[id]);
}

static int
<<<<<<< HEAD
bitmap_port_tadd(struct ip_set *set, void *value, u32 timeout)
=======
bitmap_port_tadd(struct ip_set *set, void *value, u32 timeout, u32 flags)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct bitmap_port *map = set->data;
	unsigned long *members = map->members;
	u16 id = *(u16 *)value;

<<<<<<< HEAD
	if (ip_set_timeout_test(members[id]))
=======
	if (ip_set_timeout_test(members[id]) && !(flags & IPSET_FLAG_EXIST))
>>>>>>> refs/remotes/origin/cm-10.0
		return -IPSET_ERR_EXIST;

	members[id] = ip_set_timeout_set(timeout);

	return 0;
}

static int
<<<<<<< HEAD
bitmap_port_tdel(struct ip_set *set, void *value, u32 timeout)
=======
bitmap_port_tdel(struct ip_set *set, void *value, u32 timeout, u32 flags)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct bitmap_port *map = set->data;
	unsigned long *members = map->members;
	u16 id = *(u16 *)value;
	int ret = -IPSET_ERR_EXIST;

	if (ip_set_timeout_test(members[id]))
		ret = 0;

	members[id] = IPSET_ELEM_UNSET;
	return ret;
}

static int
bitmap_port_tlist(const struct ip_set *set,
		  struct sk_buff *skb, struct netlink_callback *cb)
{
	const struct bitmap_port *map = set->data;
	struct nlattr *adt, *nested;
	u16 id, first = cb->args[2];
	u16 last = map->last_port - map->first_port;
	const unsigned long *members = map->members;

	adt = ipset_nest_start(skb, IPSET_ATTR_ADT);
	if (!adt)
		return -EMSGSIZE;
	for (; cb->args[2] <= last; cb->args[2]++) {
		id = cb->args[2];
		if (!ip_set_timeout_test(members[id]))
			continue;
		nested = ipset_nest_start(skb, IPSET_ATTR_DATA);
		if (!nested) {
			if (id == first) {
				nla_nest_cancel(skb, adt);
				return -EMSGSIZE;
			} else
				goto nla_put_failure;
		}
		NLA_PUT_NET16(skb, IPSET_ATTR_PORT,
			      htons(map->first_port + id));
		NLA_PUT_NET32(skb, IPSET_ATTR_TIMEOUT,
			      htonl(ip_set_timeout_get(members[id])));
		ipset_nest_end(skb, nested);
	}
	ipset_nest_end(skb, adt);

	/* Set listing finished */
	cb->args[2] = 0;

	return 0;

nla_put_failure:
	nla_nest_cancel(skb, nested);
	ipset_nest_end(skb, adt);
	if (unlikely(id == first)) {
		cb->args[2] = 0;
		return -EMSGSIZE;
	}
	return 0;
=======

#define IPSET_TYPE_REV_MIN	0
/*				1	   Counter support added */
#define IPSET_TYPE_REV_MAX	2	/* Comment support added */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>");
IP_SET_MODULE_DESC("bitmap:port", IPSET_TYPE_REV_MIN, IPSET_TYPE_REV_MAX);
MODULE_ALIAS("ip_set_bitmap:port");

#define MTYPE		bitmap_port

/* Type structure */
struct bitmap_port {
	void *members;		/* the set members */
	void *extensions;	/* data extensions */
	u16 first_port;		/* host byte order, included in range */
	u16 last_port;		/* host byte order, included in range */
	u32 elements;		/* number of max elements in the set */
	size_t memsize;		/* members size */
	struct timer_list gc;	/* garbage collection */
};

/* ADT structure for generic function args */
struct bitmap_port_adt_elem {
	u16 id;
};

static inline u16
port_to_id(const struct bitmap_port *m, u16 port)
{
	return port - m->first_port;
}

/* Common functions */

static inline int
bitmap_port_do_test(const struct bitmap_port_adt_elem *e,
		    const struct bitmap_port *map, size_t dsize)
{
	return !!test_bit(e->id, map->members);
}

static inline int
bitmap_port_gc_test(u16 id, const struct bitmap_port *map, size_t dsize)
{
	return !!test_bit(id, map->members);
}

static inline int
bitmap_port_do_add(const struct bitmap_port_adt_elem *e,
		   struct bitmap_port *map, u32 flags, size_t dsize)
{
	return !!test_and_set_bit(e->id, map->members);
}

static inline int
bitmap_port_do_del(const struct bitmap_port_adt_elem *e,
		   struct bitmap_port *map)
{
	return !test_and_clear_bit(e->id, map->members);
}

static inline int
bitmap_port_do_list(struct sk_buff *skb, const struct bitmap_port *map, u32 id,
		    size_t dsize)
{
	return nla_put_net16(skb, IPSET_ATTR_PORT,
			     htons(map->first_port + id));
}

static inline int
bitmap_port_do_head(struct sk_buff *skb, const struct bitmap_port *map)
{
	return nla_put_net16(skb, IPSET_ATTR_PORT, htons(map->first_port)) ||
	       nla_put_net16(skb, IPSET_ATTR_PORT_TO, htons(map->last_port));
>>>>>>> refs/remotes/origin/master
}

static int
bitmap_port_kadt(struct ip_set *set, const struct sk_buff *skb,
<<<<<<< HEAD
<<<<<<< HEAD
		 enum ipset_adt adt, u8 pf, u8 dim, u8 flags)
=======
		 const struct xt_action_param *par,
		 enum ipset_adt adt, const struct ip_set_adt_opt *opt)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct bitmap_port *map = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	__be16 __port;
	u16 port = 0;

<<<<<<< HEAD
	if (!ip_set_get_ip_port(skb, pf, flags & IPSET_DIM_ONE_SRC, &__port))
=======
	if (!ip_set_get_ip_port(skb, opt->family,
				opt->flags & IPSET_DIM_ONE_SRC, &__port))
>>>>>>> refs/remotes/origin/cm-10.0
=======
		 const struct xt_action_param *par,
		 enum ipset_adt adt, struct ip_set_adt_opt *opt)
{
	struct bitmap_port *map = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct bitmap_port_adt_elem e = {};
	struct ip_set_ext ext = IP_SET_INIT_KEXT(skb, opt, set);
	__be16 __port;
	u16 port = 0;

	if (!ip_set_get_ip_port(skb, opt->family,
				opt->flags & IPSET_DIM_ONE_SRC, &__port))
>>>>>>> refs/remotes/origin/master
		return -EINVAL;

	port = ntohs(__port);

	if (port < map->first_port || port > map->last_port)
		return -IPSET_ERR_BITMAP_RANGE;

<<<<<<< HEAD
	port -= map->first_port;

<<<<<<< HEAD
	return adtfn(set, &port, map->timeout);
=======
	return adtfn(set, &port, opt_timeout(opt, map), opt->cmdflags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	e.id = port_to_id(map, port);

	return adtfn(set, &e, &ext, &opt->ext, opt->cmdflags);
>>>>>>> refs/remotes/origin/master
}

static int
bitmap_port_uadt(struct ip_set *set, struct nlattr *tb[],
<<<<<<< HEAD
<<<<<<< HEAD
		 enum ipset_adt adt, u32 *lineno, u32 flags)
=======
		 enum ipset_adt adt, u32 *lineno, u32 flags, bool retried)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct bitmap_port *map = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	u32 timeout = map->timeout;
	u32 port;	/* wraparound */
	u16 id, port_to;
=======
		 enum ipset_adt adt, u32 *lineno, u32 flags, bool retried)
{
	struct bitmap_port *map = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct bitmap_port_adt_elem e = {};
	struct ip_set_ext ext = IP_SET_INIT_UEXT(set);
	u32 port;	/* wraparound */
	u16 port_to;
>>>>>>> refs/remotes/origin/master
	int ret = 0;

	if (unlikely(!ip_set_attr_netorder(tb, IPSET_ATTR_PORT) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_PORT_TO) ||
<<<<<<< HEAD
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT)))
=======
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_PACKETS) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_BYTES)))
>>>>>>> refs/remotes/origin/master
		return -IPSET_ERR_PROTOCOL;

	if (tb[IPSET_ATTR_LINENO])
		*lineno = nla_get_u32(tb[IPSET_ATTR_LINENO]);

	port = ip_set_get_h16(tb[IPSET_ATTR_PORT]);
	if (port < map->first_port || port > map->last_port)
		return -IPSET_ERR_BITMAP_RANGE;
<<<<<<< HEAD

	if (tb[IPSET_ATTR_TIMEOUT]) {
		if (!with_timeout(map->timeout))
			return -IPSET_ERR_TIMEOUT;
		timeout = ip_set_timeout_uget(tb[IPSET_ATTR_TIMEOUT]);
	}

	if (adt == IPSET_TEST) {
		id = port - map->first_port;
<<<<<<< HEAD
		return adtfn(set, &id, timeout);
=======
		return adtfn(set, &id, timeout, flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = ip_set_get_extensions(set, tb, &ext);
	if (ret)
		return ret;

	if (adt == IPSET_TEST) {
		e.id = port_to_id(map, port);
		return adtfn(set, &e, &ext, &ext, flags);
>>>>>>> refs/remotes/origin/master
	}

	if (tb[IPSET_ATTR_PORT_TO]) {
		port_to = ip_set_get_h16(tb[IPSET_ATTR_PORT_TO]);
		if (port > port_to) {
			swap(port, port_to);
			if (port < map->first_port)
				return -IPSET_ERR_BITMAP_RANGE;
		}
	} else
		port_to = port;

	if (port_to > map->last_port)
		return -IPSET_ERR_BITMAP_RANGE;

	for (; port <= port_to; port++) {
<<<<<<< HEAD
		id = port - map->first_port;
<<<<<<< HEAD
		ret = adtfn(set, &id, timeout);
=======
		ret = adtfn(set, &id, timeout, flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		e.id = port_to_id(map, port);
		ret = adtfn(set, &e, &ext, &ext, flags);
>>>>>>> refs/remotes/origin/master

		if (ret && !ip_set_eexist(ret, flags))
			return ret;
		else
			ret = 0;
	}
	return ret;
}

<<<<<<< HEAD
static void
bitmap_port_destroy(struct ip_set *set)
{
	struct bitmap_port *map = set->data;

	if (with_timeout(map->timeout))
		del_timer_sync(&map->gc);

	ip_set_free(map->members);
	kfree(map);

	set->data = NULL;
}

static void
bitmap_port_flush(struct ip_set *set)
{
	struct bitmap_port *map = set->data;

	memset(map->members, 0, map->memsize);
}

static int
bitmap_port_head(struct ip_set *set, struct sk_buff *skb)
{
	const struct bitmap_port *map = set->data;
	struct nlattr *nested;

	nested = ipset_nest_start(skb, IPSET_ATTR_DATA);
	if (!nested)
		goto nla_put_failure;
	NLA_PUT_NET16(skb, IPSET_ATTR_PORT, htons(map->first_port));
	NLA_PUT_NET16(skb, IPSET_ATTR_PORT_TO, htons(map->last_port));
	NLA_PUT_NET32(skb, IPSET_ATTR_REFERENCES, htonl(set->ref - 1));
	NLA_PUT_NET32(skb, IPSET_ATTR_MEMSIZE,
		      htonl(sizeof(*map) + map->memsize));
	if (with_timeout(map->timeout))
		NLA_PUT_NET32(skb, IPSET_ATTR_TIMEOUT, htonl(map->timeout));
	ipset_nest_end(skb, nested);

	return 0;
nla_put_failure:
	return -EMSGSIZE;
}

=======
>>>>>>> refs/remotes/origin/master
static bool
bitmap_port_same_set(const struct ip_set *a, const struct ip_set *b)
{
	const struct bitmap_port *x = a->data;
	const struct bitmap_port *y = b->data;

	return x->first_port == y->first_port &&
	       x->last_port == y->last_port &&
<<<<<<< HEAD
	       x->timeout == y->timeout;
}

static const struct ip_set_type_variant bitmap_port = {
	.kadt	= bitmap_port_kadt,
	.uadt	= bitmap_port_uadt,
	.adt	= {
		[IPSET_ADD] = bitmap_port_add,
		[IPSET_DEL] = bitmap_port_del,
		[IPSET_TEST] = bitmap_port_test,
	},
	.destroy = bitmap_port_destroy,
	.flush	= bitmap_port_flush,
	.head	= bitmap_port_head,
	.list	= bitmap_port_list,
	.same_set = bitmap_port_same_set,
};

static const struct ip_set_type_variant bitmap_tport = {
	.kadt	= bitmap_port_kadt,
	.uadt	= bitmap_port_uadt,
	.adt	= {
		[IPSET_ADD] = bitmap_port_tadd,
		[IPSET_DEL] = bitmap_port_tdel,
		[IPSET_TEST] = bitmap_port_ttest,
	},
	.destroy = bitmap_port_destroy,
	.flush	= bitmap_port_flush,
	.head	= bitmap_port_head,
	.list	= bitmap_port_tlist,
	.same_set = bitmap_port_same_set,
};

static void
bitmap_port_gc(unsigned long ul_set)
{
	struct ip_set *set = (struct ip_set *) ul_set;
	struct bitmap_port *map = set->data;
	unsigned long *table = map->members;
	u32 id;	/* wraparound */
	u16 last = map->last_port - map->first_port;

	/* We run parallel with other readers (test element)
	 * but adding/deleting new entries is locked out */
	read_lock_bh(&set->lock);
	for (id = 0; id <= last; id++)
		if (ip_set_timeout_expired(table[id]))
			table[id] = IPSET_ELEM_UNSET;
	read_unlock_bh(&set->lock);

	map->gc.expires = jiffies + IPSET_GC_PERIOD(map->timeout) * HZ;
	add_timer(&map->gc);
}

static void
bitmap_port_gc_init(struct ip_set *set)
{
	struct bitmap_port *map = set->data;

	init_timer(&map->gc);
	map->gc.data = (unsigned long) set;
	map->gc.function = bitmap_port_gc;
	map->gc.expires = jiffies + IPSET_GC_PERIOD(map->timeout) * HZ;
	add_timer(&map->gc);
}
=======
	       a->timeout == b->timeout &&
	       a->extensions == b->extensions;
}

/* Plain variant */

struct bitmap_port_elem {
};

#include "ip_set_bitmap_gen.h"
>>>>>>> refs/remotes/origin/master

/* Create bitmap:ip type of sets */

static bool
init_map_port(struct ip_set *set, struct bitmap_port *map,
	      u16 first_port, u16 last_port)
{
	map->members = ip_set_alloc(map->memsize);
	if (!map->members)
		return false;
<<<<<<< HEAD
	map->first_port = first_port;
	map->last_port = last_port;
	map->timeout = IPSET_NO_TIMEOUT;

	set->data = map;
<<<<<<< HEAD
	set->family = AF_UNSPEC;
=======
	set->family = NFPROTO_UNSPEC;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (set->dsize) {
		map->extensions = ip_set_alloc(set->dsize * map->elements);
		if (!map->extensions) {
			kfree(map->members);
			return false;
		}
	}
	map->first_port = first_port;
	map->last_port = last_port;
	set->timeout = IPSET_NO_TIMEOUT;

	set->data = map;
	set->family = NFPROTO_UNSPEC;
>>>>>>> refs/remotes/origin/master

	return true;
}

static int
<<<<<<< HEAD
bitmap_port_create(struct ip_set *set, struct nlattr *tb[],
		 u32 flags)
=======
bitmap_port_create(struct net *net, struct ip_set *set, struct nlattr *tb[],
		   u32 flags)
>>>>>>> refs/remotes/origin/master
{
	struct bitmap_port *map;
	u16 first_port, last_port;

	if (unlikely(!ip_set_attr_netorder(tb, IPSET_ATTR_PORT) ||
		     !ip_set_attr_netorder(tb, IPSET_ATTR_PORT_TO) ||
<<<<<<< HEAD
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT)))
=======
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_CADT_FLAGS)))
>>>>>>> refs/remotes/origin/master
		return -IPSET_ERR_PROTOCOL;

	first_port = ip_set_get_h16(tb[IPSET_ATTR_PORT]);
	last_port = ip_set_get_h16(tb[IPSET_ATTR_PORT_TO]);
	if (first_port > last_port) {
		u16 tmp = first_port;

		first_port = last_port;
		last_port = tmp;
	}

	map = kzalloc(sizeof(*map), GFP_KERNEL);
	if (!map)
		return -ENOMEM;

<<<<<<< HEAD
	if (tb[IPSET_ATTR_TIMEOUT]) {
		map->memsize = (last_port - first_port + 1)
			       * sizeof(unsigned long);

		if (!init_map_port(set, map, first_port, last_port)) {
			kfree(map);
			return -ENOMEM;
		}

		map->timeout = ip_set_timeout_uget(tb[IPSET_ATTR_TIMEOUT]);
		set->variant = &bitmap_tport;

		bitmap_port_gc_init(set);
	} else {
		map->memsize = bitmap_bytes(0, last_port - first_port);
		pr_debug("memsize: %zu\n", map->memsize);
		if (!init_map_port(set, map, first_port, last_port)) {
			kfree(map);
			return -ENOMEM;
		}

		set->variant = &bitmap_port;
=======
	map->elements = last_port - first_port + 1;
	map->memsize = bitmap_bytes(0, map->elements);
	set->variant = &bitmap_port;
	set->dsize = ip_set_elem_len(set, tb, 0);
	if (!init_map_port(set, map, first_port, last_port)) {
		kfree(map);
		return -ENOMEM;
	}
	if (tb[IPSET_ATTR_TIMEOUT]) {
		set->timeout = ip_set_timeout_uget(tb[IPSET_ATTR_TIMEOUT]);
		bitmap_port_gc_init(set, bitmap_port_gc);
>>>>>>> refs/remotes/origin/master
	}
	return 0;
}

static struct ip_set_type bitmap_port_type = {
	.name		= "bitmap:port",
	.protocol	= IPSET_PROTOCOL,
	.features	= IPSET_TYPE_PORT,
	.dimension	= IPSET_DIM_ONE,
<<<<<<< HEAD
<<<<<<< HEAD
	.family		= AF_UNSPEC,
	.revision	= 0,
=======
	.family		= NFPROTO_UNSPEC,
	.revision_min	= 0,
	.revision_max	= 0,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.family		= NFPROTO_UNSPEC,
	.revision_min	= IPSET_TYPE_REV_MIN,
	.revision_max	= IPSET_TYPE_REV_MAX,
>>>>>>> refs/remotes/origin/master
	.create		= bitmap_port_create,
	.create_policy	= {
		[IPSET_ATTR_PORT]	= { .type = NLA_U16 },
		[IPSET_ATTR_PORT_TO]	= { .type = NLA_U16 },
		[IPSET_ATTR_TIMEOUT]	= { .type = NLA_U32 },
<<<<<<< HEAD
=======
		[IPSET_ATTR_CADT_FLAGS]	= { .type = NLA_U32 },
>>>>>>> refs/remotes/origin/master
	},
	.adt_policy	= {
		[IPSET_ATTR_PORT]	= { .type = NLA_U16 },
		[IPSET_ATTR_PORT_TO]	= { .type = NLA_U16 },
		[IPSET_ATTR_TIMEOUT]	= { .type = NLA_U32 },
		[IPSET_ATTR_LINENO]	= { .type = NLA_U32 },
<<<<<<< HEAD
=======
		[IPSET_ATTR_BYTES]	= { .type = NLA_U64 },
		[IPSET_ATTR_PACKETS]	= { .type = NLA_U64 },
		[IPSET_ATTR_COMMENT]	= { .type = NLA_NUL_STRING },
>>>>>>> refs/remotes/origin/master
	},
	.me		= THIS_MODULE,
};

static int __init
bitmap_port_init(void)
{
	return ip_set_type_register(&bitmap_port_type);
}

static void __exit
bitmap_port_fini(void)
{
	ip_set_type_unregister(&bitmap_port_type);
}

module_init(bitmap_port_init);
module_exit(bitmap_port_fini);
