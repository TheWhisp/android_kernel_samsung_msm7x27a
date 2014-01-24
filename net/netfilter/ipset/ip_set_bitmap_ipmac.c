/* Copyright (C) 2000-2002 Joakim Axelsson <gozem@linux.nu>
 *                         Patrick Schaaf <bof@bof.de>
 *			   Martin Josefsson <gandalf@wlug.westbo.se>
<<<<<<< HEAD
 * Copyright (C) 2003-2011 Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>
=======
 * Copyright (C) 2003-2013 Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>
>>>>>>> refs/remotes/origin/master
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/* Kernel module implementing an IP set type: the bitmap:ip,mac type */

#include <linux/module.h>
#include <linux/ip.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/errno.h>
#include <linux/if_ether.h>
#include <linux/netlink.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <net/netlink.h>

#include <linux/netfilter/ipset/pfxlen.h>
#include <linux/netfilter/ipset/ip_set.h>
<<<<<<< HEAD
#include <linux/netfilter/ipset/ip_set_timeout.h>
#include <linux/netfilter/ipset/ip_set_bitmap.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>");
MODULE_DESCRIPTION("bitmap:ip,mac type of IP sets");
MODULE_ALIAS("ip_set_bitmap:ip,mac");

enum {
	MAC_EMPTY,		/* element is not set */
	MAC_FILLED,		/* element is set with MAC */
	MAC_UNSET,		/* element is set, without MAC */
=======
#include <linux/netfilter/ipset/ip_set_bitmap.h>

#define IPSET_TYPE_REV_MIN	0
/*				1	   Counter support added */
#define IPSET_TYPE_REV_MAX	2	/* Comment support added */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>");
IP_SET_MODULE_DESC("bitmap:ip,mac", IPSET_TYPE_REV_MIN, IPSET_TYPE_REV_MAX);
MODULE_ALIAS("ip_set_bitmap:ip,mac");

#define MTYPE		bitmap_ipmac
#define IP_SET_BITMAP_STORED_TIMEOUT

enum {
	MAC_UNSET,		/* element is set, without MAC */
	MAC_FILLED,		/* element is set with MAC */
>>>>>>> refs/remotes/origin/master
};

/* Type structure */
struct bitmap_ipmac {
	void *members;		/* the set members */
<<<<<<< HEAD
	u32 first_ip;		/* host byte order, included in range */
	u32 last_ip;		/* host byte order, included in range */
	u32 timeout;		/* timeout value */
	struct timer_list gc;	/* garbage collector */
	size_t dsize;		/* size of element */
};

/* ADT structure for generic function args */
struct ipmac {
	u32 id;			/* id in array */
	unsigned char *ether;	/* ethernet address */
};

/* Member element without and with timeout */

struct ipmac_elem {
	unsigned char ether[ETH_ALEN];
	unsigned char match;
} __attribute__ ((aligned));

struct ipmac_telem {
	unsigned char ether[ETH_ALEN];
	unsigned char match;
	unsigned long timeout;
} __attribute__ ((aligned));

static inline void *
bitmap_ipmac_elem(const struct bitmap_ipmac *map, u32 id)
{
	return (void *)((char *)map->members + id * map->dsize);
}

static inline bool
bitmap_timeout(const struct bitmap_ipmac *map, u32 id)
{
	const struct ipmac_telem *elem = bitmap_ipmac_elem(map, id);

	return ip_set_timeout_test(elem->timeout);
}

static inline bool
bitmap_expired(const struct bitmap_ipmac *map, u32 id)
{
	const struct ipmac_telem *elem = bitmap_ipmac_elem(map, id);

	return ip_set_timeout_expired(elem->timeout);
}

static inline int
bitmap_ipmac_exist(const struct ipmac_telem *elem)
{
	return elem->match == MAC_UNSET ||
	       (elem->match == MAC_FILLED &&
		!ip_set_timeout_expired(elem->timeout));
}

/* Base variant */

static int
<<<<<<< HEAD
bitmap_ipmac_test(struct ip_set *set, void *value, u32 timeout)
=======
bitmap_ipmac_test(struct ip_set *set, void *value, u32 timeout, u32 flags)
>>>>>>> refs/remotes/origin/cm-10.0
{
	const struct bitmap_ipmac *map = set->data;
	const struct ipmac *data = value;
	const struct ipmac_elem *elem = bitmap_ipmac_elem(map, data->id);

	switch (elem->match) {
	case MAC_UNSET:
		/* Trigger kernel to fill out the ethernet address */
		return -EAGAIN;
	case MAC_FILLED:
		return data->ether == NULL ||
		       compare_ether_addr(data->ether, elem->ether) == 0;
	}
	return 0;
}

static int
<<<<<<< HEAD
bitmap_ipmac_add(struct ip_set *set, void *value, u32 timeout)
=======
bitmap_ipmac_add(struct ip_set *set, void *value, u32 timeout, u32 flags)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct bitmap_ipmac *map = set->data;
	const struct ipmac *data = value;
	struct ipmac_elem *elem = bitmap_ipmac_elem(map, data->id);

	switch (elem->match) {
	case MAC_UNSET:
		if (!data->ether)
			/* Already added without ethernet address */
			return -IPSET_ERR_EXIST;
		/* Fill the MAC address */
		memcpy(elem->ether, data->ether, ETH_ALEN);
		elem->match = MAC_FILLED;
		break;
	case MAC_FILLED:
		return -IPSET_ERR_EXIST;
	case MAC_EMPTY:
		if (data->ether) {
			memcpy(elem->ether, data->ether, ETH_ALEN);
			elem->match = MAC_FILLED;
		} else
			elem->match = MAC_UNSET;
	}

	return 0;
}

static int
<<<<<<< HEAD
bitmap_ipmac_del(struct ip_set *set, void *value, u32 timeout)
=======
bitmap_ipmac_del(struct ip_set *set, void *value, u32 timeout, u32 flags)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct bitmap_ipmac *map = set->data;
	const struct ipmac *data = value;
	struct ipmac_elem *elem = bitmap_ipmac_elem(map, data->id);

	if (elem->match == MAC_EMPTY)
		return -IPSET_ERR_EXIST;

	elem->match = MAC_EMPTY;

	return 0;
}

static int
bitmap_ipmac_list(const struct ip_set *set,
		  struct sk_buff *skb, struct netlink_callback *cb)
{
	const struct bitmap_ipmac *map = set->data;
	const struct ipmac_elem *elem;
	struct nlattr *atd, *nested;
	u32 id, first = cb->args[2];
	u32 last = map->last_ip - map->first_ip;

	atd = ipset_nest_start(skb, IPSET_ATTR_ADT);
	if (!atd)
		return -EMSGSIZE;
	for (; cb->args[2] <= last; cb->args[2]++) {
		id = cb->args[2];
		elem = bitmap_ipmac_elem(map, id);
		if (elem->match == MAC_EMPTY)
			continue;
		nested = ipset_nest_start(skb, IPSET_ATTR_DATA);
		if (!nested) {
			if (id == first) {
				nla_nest_cancel(skb, atd);
				return -EMSGSIZE;
			} else
				goto nla_put_failure;
		}
		NLA_PUT_IPADDR4(skb, IPSET_ATTR_IP,
				htonl(map->first_ip + id));
		if (elem->match == MAC_FILLED)
			NLA_PUT(skb, IPSET_ATTR_ETHER, ETH_ALEN,
				elem->ether);
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
bitmap_ipmac_ttest(struct ip_set *set, void *value, u32 timeout)
=======
bitmap_ipmac_ttest(struct ip_set *set, void *value, u32 timeout, u32 flags)
>>>>>>> refs/remotes/origin/cm-10.0
{
	const struct bitmap_ipmac *map = set->data;
	const struct ipmac *data = value;
	const struct ipmac_elem *elem = bitmap_ipmac_elem(map, data->id);

	switch (elem->match) {
	case MAC_UNSET:
		/* Trigger kernel to fill out the ethernet address */
		return -EAGAIN;
	case MAC_FILLED:
		return (data->ether == NULL ||
			compare_ether_addr(data->ether, elem->ether) == 0) &&
		       !bitmap_expired(map, data->id);
	}
	return 0;
}

static int
<<<<<<< HEAD
bitmap_ipmac_tadd(struct ip_set *set, void *value, u32 timeout)
=======
bitmap_ipmac_tadd(struct ip_set *set, void *value, u32 timeout, u32 flags)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct bitmap_ipmac *map = set->data;
	const struct ipmac *data = value;
	struct ipmac_telem *elem = bitmap_ipmac_elem(map, data->id);
<<<<<<< HEAD

	switch (elem->match) {
	case MAC_UNSET:
		if (!data->ether)
=======
	bool flag_exist = flags & IPSET_FLAG_EXIST;

	switch (elem->match) {
	case MAC_UNSET:
		if (!(data->ether || flag_exist))
>>>>>>> refs/remotes/origin/cm-10.0
			/* Already added without ethernet address */
			return -IPSET_ERR_EXIST;
		/* Fill the MAC address and activate the timer */
		memcpy(elem->ether, data->ether, ETH_ALEN);
		elem->match = MAC_FILLED;
		if (timeout == map->timeout)
			/* Timeout was not specified, get stored one */
			timeout = elem->timeout;
		elem->timeout = ip_set_timeout_set(timeout);
		break;
	case MAC_FILLED:
<<<<<<< HEAD
		if (!bitmap_expired(map, data->id))
=======
		if (!(bitmap_expired(map, data->id) || flag_exist))
>>>>>>> refs/remotes/origin/cm-10.0
			return -IPSET_ERR_EXIST;
		/* Fall through */
	case MAC_EMPTY:
		if (data->ether) {
			memcpy(elem->ether, data->ether, ETH_ALEN);
			elem->match = MAC_FILLED;
		} else
			elem->match = MAC_UNSET;
=======
	void *extensions;	/* MAC + data extensions */
	u32 first_ip;		/* host byte order, included in range */
	u32 last_ip;		/* host byte order, included in range */
	u32 elements;		/* number of max elements in the set */
	size_t memsize;		/* members size */
	struct timer_list gc;	/* garbage collector */
};

/* ADT structure for generic function args */
struct bitmap_ipmac_adt_elem {
	u16 id;
	unsigned char *ether;
};

struct bitmap_ipmac_elem {
	unsigned char ether[ETH_ALEN];
	unsigned char filled;
} __attribute__ ((aligned));

static inline u32
ip_to_id(const struct bitmap_ipmac *m, u32 ip)
{
	return ip - m->first_ip;
}

static inline struct bitmap_ipmac_elem *
get_elem(void *extensions, u16 id, size_t dsize)
{
	return (struct bitmap_ipmac_elem *)(extensions + id * dsize);
}

/* Common functions */

static inline int
bitmap_ipmac_do_test(const struct bitmap_ipmac_adt_elem *e,
		     const struct bitmap_ipmac *map, size_t dsize)
{
	const struct bitmap_ipmac_elem *elem;

	if (!test_bit(e->id, map->members))
		return 0;
	elem = get_elem(map->extensions, e->id, dsize);
	if (elem->filled == MAC_FILLED)
		return e->ether == NULL ||
		       ether_addr_equal(e->ether, elem->ether);
	/* Trigger kernel to fill out the ethernet address */
	return -EAGAIN;
}

static inline int
bitmap_ipmac_gc_test(u16 id, const struct bitmap_ipmac *map, size_t dsize)
{
	const struct bitmap_ipmac_elem *elem;

	if (!test_bit(id, map->members))
		return 0;
	elem = get_elem(map->extensions, id, dsize);
	/* Timer not started for the incomplete elements */
	return elem->filled == MAC_FILLED;
}

static inline int
bitmap_ipmac_is_filled(const struct bitmap_ipmac_elem *elem)
{
	return elem->filled == MAC_FILLED;
}

static inline int
bitmap_ipmac_add_timeout(unsigned long *timeout,
			 const struct bitmap_ipmac_adt_elem *e,
			 const struct ip_set_ext *ext, struct ip_set *set,
			 struct bitmap_ipmac *map, int mode)
{
	u32 t = ext->timeout;

	if (mode == IPSET_ADD_START_STORED_TIMEOUT) {
		if (t == set->timeout)
			/* Timeout was not specified, get stored one */
			t = *timeout;
		ip_set_timeout_set(timeout, t);
	} else {
>>>>>>> refs/remotes/origin/master
		/* If MAC is unset yet, we store plain timeout value
		 * because the timer is not activated yet
		 * and we can reuse it later when MAC is filled out,
		 * possibly by the kernel */
<<<<<<< HEAD
		elem->timeout = data->ether ? ip_set_timeout_set(timeout)
					    : timeout;
		break;
	}

	return 0;
}

static int
<<<<<<< HEAD
bitmap_ipmac_tdel(struct ip_set *set, void *value, u32 timeout)
=======
bitmap_ipmac_tdel(struct ip_set *set, void *value, u32 timeout, u32 flags)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct bitmap_ipmac *map = set->data;
	const struct ipmac *data = value;
	struct ipmac_telem *elem = bitmap_ipmac_elem(map, data->id);

	if (elem->match == MAC_EMPTY || bitmap_expired(map, data->id))
		return -IPSET_ERR_EXIST;

	elem->match = MAC_EMPTY;

	return 0;
}

static int
bitmap_ipmac_tlist(const struct ip_set *set,
		   struct sk_buff *skb, struct netlink_callback *cb)
{
	const struct bitmap_ipmac *map = set->data;
	const struct ipmac_telem *elem;
	struct nlattr *atd, *nested;
	u32 id, first = cb->args[2];
	u32 timeout, last = map->last_ip - map->first_ip;

	atd = ipset_nest_start(skb, IPSET_ATTR_ADT);
	if (!atd)
		return -EMSGSIZE;
	for (; cb->args[2] <= last; cb->args[2]++) {
		id = cb->args[2];
		elem = bitmap_ipmac_elem(map, id);
		if (!bitmap_ipmac_exist(elem))
			continue;
		nested = ipset_nest_start(skb, IPSET_ATTR_DATA);
		if (!nested) {
			if (id == first) {
				nla_nest_cancel(skb, atd);
				return -EMSGSIZE;
			} else
				goto nla_put_failure;
		}
		NLA_PUT_IPADDR4(skb, IPSET_ATTR_IP,
				htonl(map->first_ip + id));
		if (elem->match == MAC_FILLED)
			NLA_PUT(skb, IPSET_ATTR_ETHER, ETH_ALEN,
				elem->ether);
		timeout = elem->match == MAC_UNSET ? elem->timeout
				: ip_set_timeout_get(elem->timeout);
		NLA_PUT_NET32(skb, IPSET_ATTR_TIMEOUT, htonl(timeout));
		ipset_nest_end(skb, nested);
	}
	ipset_nest_end(skb, atd);
	/* Set listing finished */
	cb->args[2] = 0;

	return 0;

nla_put_failure:
	nla_nest_cancel(skb, nested);
	ipset_nest_end(skb, atd);
	return -EMSGSIZE;
=======
		if (e->ether)
			ip_set_timeout_set(timeout, t);
		else
			*timeout = t;
	}
	return 0;
}

static inline int
bitmap_ipmac_do_add(const struct bitmap_ipmac_adt_elem *e,
		    struct bitmap_ipmac *map, u32 flags, size_t dsize)
{
	struct bitmap_ipmac_elem *elem;

	elem = get_elem(map->extensions, e->id, dsize);
	if (test_and_set_bit(e->id, map->members)) {
		if (elem->filled == MAC_FILLED) {
			if (e->ether && (flags & IPSET_FLAG_EXIST))
				memcpy(elem->ether, e->ether, ETH_ALEN);
			return IPSET_ADD_FAILED;
		} else if (!e->ether)
			/* Already added without ethernet address */
			return IPSET_ADD_FAILED;
		/* Fill the MAC address and trigger the timer activation */
		memcpy(elem->ether, e->ether, ETH_ALEN);
		elem->filled = MAC_FILLED;
		return IPSET_ADD_START_STORED_TIMEOUT;
	} else if (e->ether) {
		/* We can store MAC too */
		memcpy(elem->ether, e->ether, ETH_ALEN);
		elem->filled = MAC_FILLED;
		return 0;
	} else {
		elem->filled = MAC_UNSET;
		/* MAC is not stored yet, don't start timer */
		return IPSET_ADD_STORE_PLAIN_TIMEOUT;
	}
}

static inline int
bitmap_ipmac_do_del(const struct bitmap_ipmac_adt_elem *e,
		    struct bitmap_ipmac *map)
{
	return !test_and_clear_bit(e->id, map->members);
}

static inline int
bitmap_ipmac_do_list(struct sk_buff *skb, const struct bitmap_ipmac *map,
		     u32 id, size_t dsize)
{
	const struct bitmap_ipmac_elem *elem =
		get_elem(map->extensions, id, dsize);

	return nla_put_ipaddr4(skb, IPSET_ATTR_IP,
			       htonl(map->first_ip + id)) ||
	       (elem->filled == MAC_FILLED &&
		nla_put(skb, IPSET_ATTR_ETHER, ETH_ALEN, elem->ether));
}

static inline int
bitmap_ipmac_do_head(struct sk_buff *skb, const struct bitmap_ipmac *map)
{
	return nla_put_ipaddr4(skb, IPSET_ATTR_IP, htonl(map->first_ip)) ||
	       nla_put_ipaddr4(skb, IPSET_ATTR_IP_TO, htonl(map->last_ip));
>>>>>>> refs/remotes/origin/master
}

static int
bitmap_ipmac_kadt(struct ip_set *set, const struct sk_buff *skb,
<<<<<<< HEAD
<<<<<<< HEAD
		  enum ipset_adt adt, u8 pf, u8 dim, u8 flags)
=======
		  const struct xt_action_param *par,
		  enum ipset_adt adt, const struct ip_set_adt_opt *opt)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct bitmap_ipmac *map = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct ipmac data;

	/* MAC can be src only */
<<<<<<< HEAD
	if (!(flags & IPSET_DIM_TWO_SRC))
		return 0;

	data.id = ntohl(ip4addr(skb, flags & IPSET_DIM_ONE_SRC));
=======
	if (!(opt->flags & IPSET_DIM_TWO_SRC))
		return 0;

	data.id = ntohl(ip4addr(skb, opt->flags & IPSET_DIM_ONE_SRC));
>>>>>>> refs/remotes/origin/cm-10.0
	if (data.id < map->first_ip || data.id > map->last_ip)
=======
		  const struct xt_action_param *par,
		  enum ipset_adt adt, struct ip_set_adt_opt *opt)
{
	struct bitmap_ipmac *map = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct bitmap_ipmac_adt_elem e = {};
	struct ip_set_ext ext = IP_SET_INIT_KEXT(skb, opt, set);
	u32 ip;

	/* MAC can be src only */
	if (!(opt->flags & IPSET_DIM_TWO_SRC))
		return 0;

	ip = ntohl(ip4addr(skb, opt->flags & IPSET_DIM_ONE_SRC));
	if (ip < map->first_ip || ip > map->last_ip)
>>>>>>> refs/remotes/origin/master
		return -IPSET_ERR_BITMAP_RANGE;

	/* Backward compatibility: we don't check the second flag */
	if (skb_mac_header(skb) < skb->head ||
	    (skb_mac_header(skb) + ETH_HLEN) > skb->data)
		return -EINVAL;

<<<<<<< HEAD
	data.id -= map->first_ip;
	data.ether = eth_hdr(skb)->h_source;

<<<<<<< HEAD
	return adtfn(set, &data, map->timeout);
=======
	return adtfn(set, &data, opt_timeout(opt, map), opt->cmdflags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	e.id = ip_to_id(map, ip);
	e.ether = eth_hdr(skb)->h_source;

	return adtfn(set, &e, &ext, &opt->ext, opt->cmdflags);
>>>>>>> refs/remotes/origin/master
}

static int
bitmap_ipmac_uadt(struct ip_set *set, struct nlattr *tb[],
<<<<<<< HEAD
<<<<<<< HEAD
		  enum ipset_adt adt, u32 *lineno, u32 flags)
=======
		  enum ipset_adt adt, u32 *lineno, u32 flags, bool retried)
>>>>>>> refs/remotes/origin/cm-10.0
{
	const struct bitmap_ipmac *map = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct ipmac data;
	u32 timeout = map->timeout;
	int ret = 0;

	if (unlikely(!tb[IPSET_ATTR_IP] ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT)))
=======
		  enum ipset_adt adt, u32 *lineno, u32 flags, bool retried)
{
	const struct bitmap_ipmac *map = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct bitmap_ipmac_adt_elem e = {};
	struct ip_set_ext ext = IP_SET_INIT_UEXT(set);
	u32 ip = 0;
	int ret = 0;

	if (unlikely(!tb[IPSET_ATTR_IP] ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_PACKETS) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_BYTES)))
>>>>>>> refs/remotes/origin/master
		return -IPSET_ERR_PROTOCOL;

	if (tb[IPSET_ATTR_LINENO])
		*lineno = nla_get_u32(tb[IPSET_ATTR_LINENO]);

<<<<<<< HEAD
	ret = ip_set_get_hostipaddr4(tb[IPSET_ATTR_IP], &data.id);
	if (ret)
		return ret;

	if (data.id < map->first_ip || data.id > map->last_ip)
		return -IPSET_ERR_BITMAP_RANGE;

	if (tb[IPSET_ATTR_ETHER])
		data.ether = nla_data(tb[IPSET_ATTR_ETHER]);
	else
		data.ether = NULL;

	if (tb[IPSET_ATTR_TIMEOUT]) {
		if (!with_timeout(map->timeout))
			return -IPSET_ERR_TIMEOUT;
		timeout = ip_set_timeout_uget(tb[IPSET_ATTR_TIMEOUT]);
	}

	data.id -= map->first_ip;

<<<<<<< HEAD
	ret = adtfn(set, &data, timeout);
=======
	ret = adtfn(set, &data, timeout, flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = ip_set_get_hostipaddr4(tb[IPSET_ATTR_IP], &ip) ||
	      ip_set_get_extensions(set, tb, &ext);
	if (ret)
		return ret;

	if (ip < map->first_ip || ip > map->last_ip)
		return -IPSET_ERR_BITMAP_RANGE;

	e.id = ip_to_id(map, ip);
	if (tb[IPSET_ATTR_ETHER])
		e.ether = nla_data(tb[IPSET_ATTR_ETHER]);
	else
		e.ether = NULL;

	ret = adtfn(set, &e, &ext, &ext, flags);
>>>>>>> refs/remotes/origin/master

	return ip_set_eexist(ret, flags) ? 0 : ret;
}

<<<<<<< HEAD
static void
bitmap_ipmac_destroy(struct ip_set *set)
{
	struct bitmap_ipmac *map = set->data;

	if (with_timeout(map->timeout))
		del_timer_sync(&map->gc);

	ip_set_free(map->members);
	kfree(map);

	set->data = NULL;
}

static void
bitmap_ipmac_flush(struct ip_set *set)
{
	struct bitmap_ipmac *map = set->data;

	memset(map->members, 0,
	       (map->last_ip - map->first_ip + 1) * map->dsize);
}

static int
bitmap_ipmac_head(struct ip_set *set, struct sk_buff *skb)
{
	const struct bitmap_ipmac *map = set->data;
	struct nlattr *nested;

	nested = ipset_nest_start(skb, IPSET_ATTR_DATA);
	if (!nested)
		goto nla_put_failure;
	NLA_PUT_IPADDR4(skb, IPSET_ATTR_IP, htonl(map->first_ip));
	NLA_PUT_IPADDR4(skb, IPSET_ATTR_IP_TO, htonl(map->last_ip));
	NLA_PUT_NET32(skb, IPSET_ATTR_REFERENCES, htonl(set->ref - 1));
	NLA_PUT_NET32(skb, IPSET_ATTR_MEMSIZE,
		      htonl(sizeof(*map)
			    + (map->last_ip - map->first_ip + 1) * map->dsize));
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
bitmap_ipmac_same_set(const struct ip_set *a, const struct ip_set *b)
{
	const struct bitmap_ipmac *x = a->data;
	const struct bitmap_ipmac *y = b->data;

	return x->first_ip == y->first_ip &&
	       x->last_ip == y->last_ip &&
<<<<<<< HEAD
	       x->timeout == y->timeout;
}

static const struct ip_set_type_variant bitmap_ipmac = {
	.kadt	= bitmap_ipmac_kadt,
	.uadt	= bitmap_ipmac_uadt,
	.adt	= {
		[IPSET_ADD] = bitmap_ipmac_add,
		[IPSET_DEL] = bitmap_ipmac_del,
		[IPSET_TEST] = bitmap_ipmac_test,
	},
	.destroy = bitmap_ipmac_destroy,
	.flush	= bitmap_ipmac_flush,
	.head	= bitmap_ipmac_head,
	.list	= bitmap_ipmac_list,
	.same_set = bitmap_ipmac_same_set,
};

static const struct ip_set_type_variant bitmap_tipmac = {
	.kadt	= bitmap_ipmac_kadt,
	.uadt	= bitmap_ipmac_uadt,
	.adt	= {
		[IPSET_ADD] = bitmap_ipmac_tadd,
		[IPSET_DEL] = bitmap_ipmac_tdel,
		[IPSET_TEST] = bitmap_ipmac_ttest,
	},
	.destroy = bitmap_ipmac_destroy,
	.flush	= bitmap_ipmac_flush,
	.head	= bitmap_ipmac_head,
	.list	= bitmap_ipmac_tlist,
	.same_set = bitmap_ipmac_same_set,
};

static void
bitmap_ipmac_gc(unsigned long ul_set)
{
	struct ip_set *set = (struct ip_set *) ul_set;
	struct bitmap_ipmac *map = set->data;
	struct ipmac_telem *elem;
	u32 id, last = map->last_ip - map->first_ip;

	/* We run parallel with other readers (test element)
	 * but adding/deleting new entries is locked out */
	read_lock_bh(&set->lock);
	for (id = 0; id <= last; id++) {
		elem = bitmap_ipmac_elem(map, id);
		if (elem->match == MAC_FILLED &&
		    ip_set_timeout_expired(elem->timeout))
			elem->match = MAC_EMPTY;
	}
	read_unlock_bh(&set->lock);

	map->gc.expires = jiffies + IPSET_GC_PERIOD(map->timeout) * HZ;
	add_timer(&map->gc);
}

static void
bitmap_ipmac_gc_init(struct ip_set *set)
{
	struct bitmap_ipmac *map = set->data;

	init_timer(&map->gc);
	map->gc.data = (unsigned long) set;
	map->gc.function = bitmap_ipmac_gc;
	map->gc.expires = jiffies + IPSET_GC_PERIOD(map->timeout) * HZ;
	add_timer(&map->gc);
}
=======
	       a->timeout == b->timeout &&
	       a->extensions == b->extensions;
}

/* Plain variant */

#include "ip_set_bitmap_gen.h"
>>>>>>> refs/remotes/origin/master

/* Create bitmap:ip,mac type of sets */

static bool
init_map_ipmac(struct ip_set *set, struct bitmap_ipmac *map,
<<<<<<< HEAD
	       u32 first_ip, u32 last_ip)
{
	map->members = ip_set_alloc((last_ip - first_ip + 1) * map->dsize);
	if (!map->members)
		return false;
	map->first_ip = first_ip;
	map->last_ip = last_ip;
	map->timeout = IPSET_NO_TIMEOUT;

	set->data = map;
<<<<<<< HEAD
	set->family = AF_INET;
=======
	set->family = NFPROTO_IPV4;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	       u32 first_ip, u32 last_ip, u32 elements)
{
	map->members = ip_set_alloc(map->memsize);
	if (!map->members)
		return false;
	if (set->dsize) {
		map->extensions = ip_set_alloc(set->dsize * elements);
		if (!map->extensions) {
			kfree(map->members);
			return false;
		}
	}
	map->first_ip = first_ip;
	map->last_ip = last_ip;
	map->elements = elements;
	set->timeout = IPSET_NO_TIMEOUT;

	set->data = map;
	set->family = NFPROTO_IPV4;
>>>>>>> refs/remotes/origin/master

	return true;
}

static int
<<<<<<< HEAD
bitmap_ipmac_create(struct ip_set *set, struct nlattr *tb[],
		    u32 flags)
{
	u32 first_ip, last_ip, elements;
=======
bitmap_ipmac_create(struct net *net, struct ip_set *set, struct nlattr *tb[],
		    u32 flags)
{
	u32 first_ip = 0, last_ip = 0;
	u64 elements;
>>>>>>> refs/remotes/origin/master
	struct bitmap_ipmac *map;
	int ret;

	if (unlikely(!tb[IPSET_ATTR_IP] ||
<<<<<<< HEAD
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT)))
=======
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_CADT_FLAGS)))
>>>>>>> refs/remotes/origin/master
		return -IPSET_ERR_PROTOCOL;

	ret = ip_set_get_hostipaddr4(tb[IPSET_ATTR_IP], &first_ip);
	if (ret)
		return ret;

	if (tb[IPSET_ATTR_IP_TO]) {
		ret = ip_set_get_hostipaddr4(tb[IPSET_ATTR_IP_TO], &last_ip);
		if (ret)
			return ret;
		if (first_ip > last_ip) {
			u32 tmp = first_ip;

			first_ip = last_ip;
			last_ip = tmp;
		}
	} else if (tb[IPSET_ATTR_CIDR]) {
		u8 cidr = nla_get_u8(tb[IPSET_ATTR_CIDR]);

		if (cidr >= 32)
			return -IPSET_ERR_INVALID_CIDR;
<<<<<<< HEAD
<<<<<<< HEAD
		last_ip = first_ip | ~ip_set_hostmask(cidr);
=======
		ip_set_mask_from_to(first_ip, last_ip, cidr);
>>>>>>> refs/remotes/origin/cm-10.0
	} else
		return -IPSET_ERR_PROTOCOL;

	elements = last_ip - first_ip + 1;
=======
		ip_set_mask_from_to(first_ip, last_ip, cidr);
	} else
		return -IPSET_ERR_PROTOCOL;

	elements = (u64)last_ip - first_ip + 1;
>>>>>>> refs/remotes/origin/master

	if (elements > IPSET_BITMAP_MAX_RANGE + 1)
		return -IPSET_ERR_BITMAP_RANGE_SIZE;

	map = kzalloc(sizeof(*map), GFP_KERNEL);
	if (!map)
		return -ENOMEM;

<<<<<<< HEAD
	if (tb[IPSET_ATTR_TIMEOUT]) {
		map->dsize = sizeof(struct ipmac_telem);

		if (!init_map_ipmac(set, map, first_ip, last_ip)) {
			kfree(map);
			return -ENOMEM;
		}

		map->timeout = ip_set_timeout_uget(tb[IPSET_ATTR_TIMEOUT]);

		set->variant = &bitmap_tipmac;

		bitmap_ipmac_gc_init(set);
	} else {
		map->dsize = sizeof(struct ipmac_elem);

		if (!init_map_ipmac(set, map, first_ip, last_ip)) {
			kfree(map);
			return -ENOMEM;
		}
		set->variant = &bitmap_ipmac;

=======
	map->memsize = bitmap_bytes(0, elements - 1);
	set->variant = &bitmap_ipmac;
	set->dsize = ip_set_elem_len(set, tb,
				     sizeof(struct bitmap_ipmac_elem));
	if (!init_map_ipmac(set, map, first_ip, last_ip, elements)) {
		kfree(map);
		return -ENOMEM;
	}
	if (tb[IPSET_ATTR_TIMEOUT]) {
		set->timeout = ip_set_timeout_uget(tb[IPSET_ATTR_TIMEOUT]);
		bitmap_ipmac_gc_init(set, bitmap_ipmac_gc);
>>>>>>> refs/remotes/origin/master
	}
	return 0;
}

static struct ip_set_type bitmap_ipmac_type = {
	.name		= "bitmap:ip,mac",
	.protocol	= IPSET_PROTOCOL,
	.features	= IPSET_TYPE_IP | IPSET_TYPE_MAC,
	.dimension	= IPSET_DIM_TWO,
<<<<<<< HEAD
<<<<<<< HEAD
	.family		= AF_INET,
	.revision	= 0,
=======
	.family		= NFPROTO_IPV4,
	.revision_min	= 0,
	.revision_max	= 0,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.family		= NFPROTO_IPV4,
	.revision_min	= IPSET_TYPE_REV_MIN,
	.revision_max	= IPSET_TYPE_REV_MAX,
>>>>>>> refs/remotes/origin/master
	.create		= bitmap_ipmac_create,
	.create_policy	= {
		[IPSET_ATTR_IP]		= { .type = NLA_NESTED },
		[IPSET_ATTR_IP_TO]	= { .type = NLA_NESTED },
		[IPSET_ATTR_CIDR]	= { .type = NLA_U8 },
		[IPSET_ATTR_TIMEOUT]	= { .type = NLA_U32 },
<<<<<<< HEAD
	},
	.adt_policy	= {
		[IPSET_ATTR_IP]		= { .type = NLA_NESTED },
<<<<<<< HEAD
		[IPSET_ATTR_ETHER]	= { .type = NLA_BINARY, .len  = ETH_ALEN },
=======
		[IPSET_ATTR_ETHER]	= { .type = NLA_BINARY,
					    .len  = ETH_ALEN },
>>>>>>> refs/remotes/origin/cm-10.0
		[IPSET_ATTR_TIMEOUT]	= { .type = NLA_U32 },
		[IPSET_ATTR_LINENO]	= { .type = NLA_U32 },
=======
		[IPSET_ATTR_CADT_FLAGS]	= { .type = NLA_U32 },
	},
	.adt_policy	= {
		[IPSET_ATTR_IP]		= { .type = NLA_NESTED },
		[IPSET_ATTR_ETHER]	= { .type = NLA_BINARY,
					    .len  = ETH_ALEN },
		[IPSET_ATTR_TIMEOUT]	= { .type = NLA_U32 },
		[IPSET_ATTR_LINENO]	= { .type = NLA_U32 },
		[IPSET_ATTR_BYTES]	= { .type = NLA_U64 },
		[IPSET_ATTR_PACKETS]	= { .type = NLA_U64 },
		[IPSET_ATTR_COMMENT]	= { .type = NLA_NUL_STRING },
>>>>>>> refs/remotes/origin/master
	},
	.me		= THIS_MODULE,
};

static int __init
bitmap_ipmac_init(void)
{
	return ip_set_type_register(&bitmap_ipmac_type);
}

static void __exit
bitmap_ipmac_fini(void)
{
	ip_set_type_unregister(&bitmap_ipmac_type);
}

module_init(bitmap_ipmac_init);
module_exit(bitmap_ipmac_fini);
