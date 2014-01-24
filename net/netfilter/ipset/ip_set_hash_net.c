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

/* Kernel module implementing an IP set type: the hash:net type */

#include <linux/jhash.h>
#include <linux/module.h>
#include <linux/ip.h>
#include <linux/skbuff.h>
#include <linux/errno.h>
#include <linux/random.h>
#include <net/ip.h>
#include <net/ipv6.h>
#include <net/netlink.h>

#include <linux/netfilter.h>
#include <linux/netfilter/ipset/pfxlen.h>
#include <linux/netfilter/ipset/ip_set.h>
<<<<<<< HEAD
#include <linux/netfilter/ipset/ip_set_timeout.h>
#include <linux/netfilter/ipset/ip_set_hash.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>");
MODULE_DESCRIPTION("hash:net type of IP sets");
MODULE_ALIAS("ip_set_hash:net");

/* Type specific function prefix */
#define TYPE		hash_net

static bool
hash_net_same_set(const struct ip_set *a, const struct ip_set *b);

#define hash_net4_same_set	hash_net_same_set
#define hash_net6_same_set	hash_net_same_set

/* The type variant functions: IPv4 */

/* Member elements without timeout */
struct hash_net4_elem {
	__be32 ip;
	u16 padding0;
<<<<<<< HEAD
	u8 padding1;
=======
	u8 nomatch;
>>>>>>> refs/remotes/origin/cm-10.0
	u8 cidr;
};

/* Member elements with timeout support */
struct hash_net4_telem {
	__be32 ip;
	u16 padding0;
<<<<<<< HEAD
	u8 padding1;
=======
	u8 nomatch;
>>>>>>> refs/remotes/origin/cm-10.0
	u8 cidr;
	unsigned long timeout;
};

static inline bool
hash_net4_data_equal(const struct hash_net4_elem *ip1,
<<<<<<< HEAD
		    const struct hash_net4_elem *ip2)
{
	return ip1->ip == ip2->ip && ip1->cidr == ip2->cidr;
=======
=======
#include <linux/netfilter/ipset/ip_set_hash.h>

#define IPSET_TYPE_REV_MIN	0
/*				1    Range as input support for IPv4 added */
/*				2    nomatch flag support added */
/*				3    Counters support added */
#define IPSET_TYPE_REV_MAX	4 /* Comments support added */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>");
IP_SET_MODULE_DESC("hash:net", IPSET_TYPE_REV_MIN, IPSET_TYPE_REV_MAX);
MODULE_ALIAS("ip_set_hash:net");

/* Type specific function prefix */
#define HTYPE		hash_net
#define IP_SET_HASH_WITH_NETS

/* IPv4 variant */

/* Member elements  */
struct hash_net4_elem {
	__be32 ip;
	u16 padding0;
	u8 nomatch;
	u8 cidr;
};

/* Common functions */

static inline bool
hash_net4_data_equal(const struct hash_net4_elem *ip1,
>>>>>>> refs/remotes/origin/master
		     const struct hash_net4_elem *ip2,
		     u32 *multi)
{
	return ip1->ip == ip2->ip &&
	       ip1->cidr == ip2->cidr;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

static inline bool
hash_net4_data_isnull(const struct hash_net4_elem *elem)
{
	return elem->cidr == 0;
}

static inline void
hash_net4_data_copy(struct hash_net4_elem *dst,
		    const struct hash_net4_elem *src)
{
	dst->ip = src->ip;
	dst->cidr = src->cidr;
<<<<<<< HEAD
=======
	dst->nomatch = src->nomatch;
}

static inline void
hash_net4_data_flags(struct hash_net4_elem *dst, u32 flags)
{
	dst->nomatch = flags & IPSET_FLAG_NOMATCH;
}

static inline bool
hash_net4_data_match(const struct hash_net4_elem *elem)
{
	return !elem->nomatch;
>>>>>>> refs/remotes/origin/cm-10.0
=======
}

static inline int
hash_net4_do_data_match(const struct hash_net4_elem *elem)
{
	return elem->nomatch ? -ENOTEMPTY : 1;
}

static inline void
hash_net4_data_set_flags(struct hash_net4_elem *elem, u32 flags)
{
	elem->nomatch = (flags >> 16) & IPSET_FLAG_NOMATCH;
}

static inline void
hash_net4_data_reset_flags(struct hash_net4_elem *elem, u8 *flags)
{
	swap(*flags, elem->nomatch);
>>>>>>> refs/remotes/origin/master
}

static inline void
hash_net4_data_netmask(struct hash_net4_elem *elem, u8 cidr)
{
	elem->ip &= ip_set_netmask(cidr);
	elem->cidr = cidr;
}

<<<<<<< HEAD
/* Zero CIDR values cannot be stored */
static inline void
hash_net4_data_zero_out(struct hash_net4_elem *elem)
{
	elem->cidr = 0;
}

static bool
hash_net4_data_list(struct sk_buff *skb, const struct hash_net4_elem *data)
{
<<<<<<< HEAD
	NLA_PUT_IPADDR4(skb, IPSET_ATTR_IP, data->ip);
	NLA_PUT_U8(skb, IPSET_ATTR_CIDR, data->cidr);
=======
	u32 flags = data->nomatch ? IPSET_FLAG_NOMATCH : 0;

	NLA_PUT_IPADDR4(skb, IPSET_ATTR_IP, data->ip);
	NLA_PUT_U8(skb, IPSET_ATTR_CIDR, data->cidr);
	if (flags)
		NLA_PUT_NET32(skb, IPSET_ATTR_CADT_FLAGS, htonl(flags));
>>>>>>> refs/remotes/origin/cm-10.0
=======
static bool
hash_net4_data_list(struct sk_buff *skb, const struct hash_net4_elem *data)
{
	u32 flags = data->nomatch ? IPSET_FLAG_NOMATCH : 0;

	if (nla_put_ipaddr4(skb, IPSET_ATTR_IP, data->ip) ||
	    nla_put_u8(skb, IPSET_ATTR_CIDR, data->cidr) ||
	    (flags &&
	     nla_put_net32(skb, IPSET_ATTR_CADT_FLAGS, htonl(flags))))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master
	return 0;

nla_put_failure:
	return 1;
}

<<<<<<< HEAD
static bool
hash_net4_data_tlist(struct sk_buff *skb, const struct hash_net4_elem *data)
{
	const struct hash_net4_telem *tdata =
		(const struct hash_net4_telem *)data;
<<<<<<< HEAD
=======
	u32 flags = data->nomatch ? IPSET_FLAG_NOMATCH : 0;
>>>>>>> refs/remotes/origin/cm-10.0

	NLA_PUT_IPADDR4(skb, IPSET_ATTR_IP, tdata->ip);
	NLA_PUT_U8(skb, IPSET_ATTR_CIDR, tdata->cidr);
	NLA_PUT_NET32(skb, IPSET_ATTR_TIMEOUT,
		      htonl(ip_set_timeout_get(tdata->timeout)));
<<<<<<< HEAD
=======
	if (flags)
		NLA_PUT_NET32(skb, IPSET_ATTR_CADT_FLAGS, htonl(flags));
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;

nla_put_failure:
	return 1;
}

#define IP_SET_HASH_WITH_NETS

#define PF		4
#define HOST_MASK	32
#include <linux/netfilter/ipset/ip_set_ahash.h>

<<<<<<< HEAD
static int
hash_net4_kadt(struct ip_set *set, const struct sk_buff *skb,
	       enum ipset_adt adt, u8 pf, u8 dim, u8 flags)
=======
static inline void
hash_net4_data_next(struct ip_set_hash *h,
		    const struct hash_net4_elem *d)
{
	h->next.ip = ntohl(d->ip);
}
=======
static inline void
hash_net4_data_next(struct hash_net4_elem *next,
		    const struct hash_net4_elem *d)
{
	next->ip = d->ip;
}

#define MTYPE		hash_net4
#define PF		4
#define HOST_MASK	32
#include "ip_set_hash_gen.h"
>>>>>>> refs/remotes/origin/master

static int
hash_net4_kadt(struct ip_set *set, const struct sk_buff *skb,
	       const struct xt_action_param *par,
<<<<<<< HEAD
	       enum ipset_adt adt, const struct ip_set_adt_opt *opt)
>>>>>>> refs/remotes/origin/cm-10.0
{
	const struct ip_set_hash *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_net4_elem data = {
		.cidr = h->nets[0].cidr ? h->nets[0].cidr : HOST_MASK
	};

	if (data.cidr == 0)
		return -EINVAL;
	if (adt == IPSET_TEST)
		data.cidr = HOST_MASK;

<<<<<<< HEAD
	ip4addrptr(skb, flags & IPSET_DIM_ONE_SRC, &data.ip);
	data.ip &= ip_set_netmask(data.cidr);

	return adtfn(set, &data, h->timeout);
=======
	ip4addrptr(skb, opt->flags & IPSET_DIM_ONE_SRC, &data.ip);
	data.ip &= ip_set_netmask(data.cidr);

	return adtfn(set, &data, opt_timeout(opt, h), opt->cmdflags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	       enum ipset_adt adt, struct ip_set_adt_opt *opt)
{
	const struct hash_net *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_net4_elem e = {
		.cidr = IP_SET_INIT_CIDR(h->nets[0].cidr[0], HOST_MASK),
	};
	struct ip_set_ext ext = IP_SET_INIT_KEXT(skb, opt, set);

	if (e.cidr == 0)
		return -EINVAL;
	if (adt == IPSET_TEST)
		e.cidr = HOST_MASK;

	ip4addrptr(skb, opt->flags & IPSET_DIM_ONE_SRC, &e.ip);
	e.ip &= ip_set_netmask(e.cidr);

	return adtfn(set, &e, &ext, &opt->ext, opt->cmdflags);
>>>>>>> refs/remotes/origin/master
}

static int
hash_net4_uadt(struct ip_set *set, struct nlattr *tb[],
<<<<<<< HEAD
<<<<<<< HEAD
	       enum ipset_adt adt, u32 *lineno, u32 flags)
=======
	       enum ipset_adt adt, u32 *lineno, u32 flags, bool retried)
>>>>>>> refs/remotes/origin/cm-10.0
{
	const struct ip_set_hash *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_net4_elem data = { .cidr = HOST_MASK };
	u32 timeout = h->timeout;
<<<<<<< HEAD
	int ret;

	if (unlikely(!tb[IPSET_ATTR_IP] ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT)))
=======
	u32 ip = 0, ip_to, last;
=======
	       enum ipset_adt adt, u32 *lineno, u32 flags, bool retried)
{
	const struct hash_net *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_net4_elem e = { .cidr = HOST_MASK };
	struct ip_set_ext ext = IP_SET_INIT_UEXT(set);
	u32 ip = 0, ip_to = 0, last;
>>>>>>> refs/remotes/origin/master
	int ret;

	if (unlikely(!tb[IPSET_ATTR_IP] ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT) ||
<<<<<<< HEAD
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_CADT_FLAGS)))
>>>>>>> refs/remotes/origin/cm-10.0
=======
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_CADT_FLAGS) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_PACKETS) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_BYTES)))
>>>>>>> refs/remotes/origin/master
		return -IPSET_ERR_PROTOCOL;

	if (tb[IPSET_ATTR_LINENO])
		*lineno = nla_get_u32(tb[IPSET_ATTR_LINENO]);

<<<<<<< HEAD
<<<<<<< HEAD
	ret = ip_set_get_ipaddr4(tb[IPSET_ATTR_IP], &data.ip);
	if (ret)
		return ret;

	if (tb[IPSET_ATTR_CIDR])
		data.cidr = nla_get_u8(tb[IPSET_ATTR_CIDR]);

	if (!data.cidr)
		return -IPSET_ERR_INVALID_CIDR;

	data.ip &= ip_set_netmask(data.cidr);
=======
	ret = ip_set_get_hostipaddr4(tb[IPSET_ATTR_IP], &ip);
=======
	ret = ip_set_get_hostipaddr4(tb[IPSET_ATTR_IP], &ip) ||
	      ip_set_get_extensions(set, tb, &ext);
>>>>>>> refs/remotes/origin/master
	if (ret)
		return ret;

	if (tb[IPSET_ATTR_CIDR]) {
<<<<<<< HEAD
		data.cidr = nla_get_u8(tb[IPSET_ATTR_CIDR]);
		if (!data.cidr || data.cidr > HOST_MASK)
			return -IPSET_ERR_INVALID_CIDR;
	}
>>>>>>> refs/remotes/origin/cm-10.0

	if (tb[IPSET_ATTR_TIMEOUT]) {
		if (!with_timeout(h->timeout))
			return -IPSET_ERR_TIMEOUT;
		timeout = ip_set_timeout_uget(tb[IPSET_ATTR_TIMEOUT]);
	}

<<<<<<< HEAD
	ret = adtfn(set, &data, timeout);

	return ip_set_eexist(ret, flags) ? 0 : ret;
=======
	if (tb[IPSET_ATTR_CADT_FLAGS] && adt == IPSET_ADD) {
		u32 cadt_flags = ip_set_get_h32(tb[IPSET_ATTR_CADT_FLAGS]);
		if (cadt_flags & IPSET_FLAG_NOMATCH)
			flags |= (cadt_flags << 16);
	}

	if (adt == IPSET_TEST || !tb[IPSET_ATTR_IP_TO]) {
		data.ip = htonl(ip & ip_set_hostmask(data.cidr));
		ret = adtfn(set, &data, timeout, flags);
		return ip_set_eexist(ret, flags) ? 0 : ret;
=======
		e.cidr = nla_get_u8(tb[IPSET_ATTR_CIDR]);
		if (!e.cidr || e.cidr > HOST_MASK)
			return -IPSET_ERR_INVALID_CIDR;
	}

	if (tb[IPSET_ATTR_CADT_FLAGS]) {
		u32 cadt_flags = ip_set_get_h32(tb[IPSET_ATTR_CADT_FLAGS]);
		if (cadt_flags & IPSET_FLAG_NOMATCH)
			flags |= (IPSET_FLAG_NOMATCH << 16);
	}

	if (adt == IPSET_TEST || !tb[IPSET_ATTR_IP_TO]) {
		e.ip = htonl(ip & ip_set_hostmask(e.cidr));
		ret = adtfn(set, &e, &ext, &ext, flags);
		return ip_set_enomatch(ret, flags, adt, set) ? -ret:
		       ip_set_eexist(ret, flags) ? 0 : ret;
>>>>>>> refs/remotes/origin/master
	}

	ip_to = ip;
	if (tb[IPSET_ATTR_IP_TO]) {
		ret = ip_set_get_hostipaddr4(tb[IPSET_ATTR_IP_TO], &ip_to);
		if (ret)
			return ret;
		if (ip_to < ip)
			swap(ip, ip_to);
		if (ip + UINT_MAX == ip_to)
			return -IPSET_ERR_HASH_RANGE;
	}
	if (retried)
<<<<<<< HEAD
		ip = h->next.ip;
	while (!after(ip, ip_to)) {
		data.ip = htonl(ip);
		last = ip_set_range_to_cidr(ip, ip_to, &data.cidr);
		ret = adtfn(set, &data, timeout, flags);
=======
		ip = ntohl(h->next.ip);
	while (!after(ip, ip_to)) {
		e.ip = htonl(ip);
		last = ip_set_range_to_cidr(ip, ip_to, &e.cidr);
		ret = adtfn(set, &e, &ext, &ext, flags);
>>>>>>> refs/remotes/origin/master
		if (ret && !ip_set_eexist(ret, flags))
			return ret;
		else
			ret = 0;
		ip = last + 1;
	}
	return ret;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

static bool
hash_net_same_set(const struct ip_set *a, const struct ip_set *b)
{
	const struct ip_set_hash *x = a->data;
	const struct ip_set_hash *y = b->data;

	/* Resizing changes htable_bits, so we ignore it */
	return x->maxelem == y->maxelem &&
	       x->timeout == y->timeout;
}

/* The type variant functions: IPv6 */
=======
}

/* IPv6 variant */
>>>>>>> refs/remotes/origin/master

struct hash_net6_elem {
	union nf_inet_addr ip;
	u16 padding0;
<<<<<<< HEAD
<<<<<<< HEAD
	u8 padding1;
=======
	u8 nomatch;
>>>>>>> refs/remotes/origin/cm-10.0
	u8 cidr;
};

struct hash_net6_telem {
	union nf_inet_addr ip;
	u16 padding0;
<<<<<<< HEAD
	u8 padding1;
=======
	u8 nomatch;
>>>>>>> refs/remotes/origin/cm-10.0
	u8 cidr;
	unsigned long timeout;
};

static inline bool
hash_net6_data_equal(const struct hash_net6_elem *ip1,
<<<<<<< HEAD
		     const struct hash_net6_elem *ip2)
=======
		     const struct hash_net6_elem *ip2,
		     u32 *multi)
>>>>>>> refs/remotes/origin/cm-10.0
{
	return ipv6_addr_cmp(&ip1->ip.in6, &ip2->ip.in6) == 0 &&
	       ip1->cidr == ip2->cidr;
}

static inline bool
hash_net6_data_isnull(const struct hash_net6_elem *elem)
{
	return elem->cidr == 0;
}

static inline void
hash_net6_data_copy(struct hash_net6_elem *dst,
		    const struct hash_net6_elem *src)
{
<<<<<<< HEAD
	ipv6_addr_copy(&dst->ip.in6, &src->ip.in6);
	dst->cidr = src->cidr;
=======
	dst->ip.in6 = src->ip.in6;
	dst->cidr = src->cidr;
	dst->nomatch = src->nomatch;
}

static inline void
hash_net6_data_flags(struct hash_net6_elem *dst, u32 flags)
{
	dst->nomatch = flags & IPSET_FLAG_NOMATCH;
}

static inline bool
hash_net6_data_match(const struct hash_net6_elem *elem)
{
	return !elem->nomatch;
>>>>>>> refs/remotes/origin/cm-10.0
}

static inline void
hash_net6_data_zero_out(struct hash_net6_elem *elem)
{
	elem->cidr = 0;
}

static inline void
ip6_netmask(union nf_inet_addr *ip, u8 prefix)
{
	ip->ip6[0] &= ip_set_netmask6(prefix)[0];
	ip->ip6[1] &= ip_set_netmask6(prefix)[1];
	ip->ip6[2] &= ip_set_netmask6(prefix)[2];
	ip->ip6[3] &= ip_set_netmask6(prefix)[3];
=======
	u8 nomatch;
	u8 cidr;
};

/* Common functions */

static inline bool
hash_net6_data_equal(const struct hash_net6_elem *ip1,
		     const struct hash_net6_elem *ip2,
		     u32 *multi)
{
	return ipv6_addr_equal(&ip1->ip.in6, &ip2->ip.in6) &&
	       ip1->cidr == ip2->cidr;
}

static inline int
hash_net6_do_data_match(const struct hash_net6_elem *elem)
{
	return elem->nomatch ? -ENOTEMPTY : 1;
}

static inline void
hash_net6_data_set_flags(struct hash_net6_elem *elem, u32 flags)
{
	elem->nomatch = (flags >> 16) & IPSET_FLAG_NOMATCH;
}

static inline void
hash_net6_data_reset_flags(struct hash_net6_elem *elem, u8 *flags)
{
	swap(*flags, elem->nomatch);
>>>>>>> refs/remotes/origin/master
}

static inline void
hash_net6_data_netmask(struct hash_net6_elem *elem, u8 cidr)
{
	ip6_netmask(&elem->ip, cidr);
	elem->cidr = cidr;
}

static bool
hash_net6_data_list(struct sk_buff *skb, const struct hash_net6_elem *data)
{
<<<<<<< HEAD
<<<<<<< HEAD
	NLA_PUT_IPADDR6(skb, IPSET_ATTR_IP, &data->ip);
	NLA_PUT_U8(skb, IPSET_ATTR_CIDR, data->cidr);
=======
	u32 flags = data->nomatch ? IPSET_FLAG_NOMATCH : 0;

	NLA_PUT_IPADDR6(skb, IPSET_ATTR_IP, &data->ip);
	NLA_PUT_U8(skb, IPSET_ATTR_CIDR, data->cidr);
	if (flags)
		NLA_PUT_NET32(skb, IPSET_ATTR_CADT_FLAGS, htonl(flags));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u32 flags = data->nomatch ? IPSET_FLAG_NOMATCH : 0;

	if (nla_put_ipaddr6(skb, IPSET_ATTR_IP, &data->ip.in6) ||
	    nla_put_u8(skb, IPSET_ATTR_CIDR, data->cidr) ||
	    (flags &&
	     nla_put_net32(skb, IPSET_ATTR_CADT_FLAGS, htonl(flags))))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master
	return 0;

nla_put_failure:
	return 1;
}

<<<<<<< HEAD
static bool
hash_net6_data_tlist(struct sk_buff *skb, const struct hash_net6_elem *data)
{
	const struct hash_net6_telem *e =
		(const struct hash_net6_telem *)data;
<<<<<<< HEAD
=======
	u32 flags = data->nomatch ? IPSET_FLAG_NOMATCH : 0;
>>>>>>> refs/remotes/origin/cm-10.0

	NLA_PUT_IPADDR6(skb, IPSET_ATTR_IP, &e->ip);
	NLA_PUT_U8(skb, IPSET_ATTR_CIDR, e->cidr);
	NLA_PUT_NET32(skb, IPSET_ATTR_TIMEOUT,
		      htonl(ip_set_timeout_get(e->timeout)));
<<<<<<< HEAD
=======
	if (flags)
		NLA_PUT_NET32(skb, IPSET_ATTR_CADT_FLAGS, htonl(flags));
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;

nla_put_failure:
	return 1;
}

#undef PF
#undef HOST_MASK

#define PF		6
#define HOST_MASK	128
#include <linux/netfilter/ipset/ip_set_ahash.h>

<<<<<<< HEAD
static int
hash_net6_kadt(struct ip_set *set, const struct sk_buff *skb,
	       enum ipset_adt adt, u8 pf, u8 dim, u8 flags)
=======
static inline void
hash_net6_data_next(struct ip_set_hash *h,
		    const struct hash_net6_elem *d)
{
}
=======
static inline void
hash_net6_data_next(struct hash_net4_elem *next,
		    const struct hash_net6_elem *d)
{
}

#undef MTYPE
#undef PF
#undef HOST_MASK

#define MTYPE		hash_net6
#define PF		6
#define HOST_MASK	128
#define IP_SET_EMIT_CREATE
#include "ip_set_hash_gen.h"
>>>>>>> refs/remotes/origin/master

static int
hash_net6_kadt(struct ip_set *set, const struct sk_buff *skb,
	       const struct xt_action_param *par,
<<<<<<< HEAD
	       enum ipset_adt adt, const struct ip_set_adt_opt *opt)
>>>>>>> refs/remotes/origin/cm-10.0
{
	const struct ip_set_hash *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_net6_elem data = {
		.cidr = h->nets[0].cidr ? h->nets[0].cidr : HOST_MASK
	};

	if (data.cidr == 0)
		return -EINVAL;
	if (adt == IPSET_TEST)
		data.cidr = HOST_MASK;

<<<<<<< HEAD
	ip6addrptr(skb, flags & IPSET_DIM_ONE_SRC, &data.ip.in6);
	ip6_netmask(&data.ip, data.cidr);

	return adtfn(set, &data, h->timeout);
=======
	ip6addrptr(skb, opt->flags & IPSET_DIM_ONE_SRC, &data.ip.in6);
	ip6_netmask(&data.ip, data.cidr);

	return adtfn(set, &data, opt_timeout(opt, h), opt->cmdflags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	       enum ipset_adt adt, struct ip_set_adt_opt *opt)
{
	const struct hash_net *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_net6_elem e = {
		.cidr = IP_SET_INIT_CIDR(h->nets[0].cidr[0], HOST_MASK),
	};
	struct ip_set_ext ext = IP_SET_INIT_KEXT(skb, opt, set);

	if (e.cidr == 0)
		return -EINVAL;
	if (adt == IPSET_TEST)
		e.cidr = HOST_MASK;

	ip6addrptr(skb, opt->flags & IPSET_DIM_ONE_SRC, &e.ip.in6);
	ip6_netmask(&e.ip, e.cidr);

	return adtfn(set, &e, &ext, &opt->ext, opt->cmdflags);
>>>>>>> refs/remotes/origin/master
}

static int
hash_net6_uadt(struct ip_set *set, struct nlattr *tb[],
<<<<<<< HEAD
<<<<<<< HEAD
	       enum ipset_adt adt, u32 *lineno, u32 flags)
=======
	       enum ipset_adt adt, u32 *lineno, u32 flags, bool retried)
>>>>>>> refs/remotes/origin/cm-10.0
{
	const struct ip_set_hash *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_net6_elem data = { .cidr = HOST_MASK };
	u32 timeout = h->timeout;
	int ret;

	if (unlikely(!tb[IPSET_ATTR_IP] ||
<<<<<<< HEAD
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT)))
		return -IPSET_ERR_PROTOCOL;
=======
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_CADT_FLAGS)))
		return -IPSET_ERR_PROTOCOL;
	if (unlikely(tb[IPSET_ATTR_IP_TO]))
		return -IPSET_ERR_HASH_RANGE_UNSUPPORTED;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	       enum ipset_adt adt, u32 *lineno, u32 flags, bool retried)
{
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_net6_elem e = { .cidr = HOST_MASK };
	struct ip_set_ext ext = IP_SET_INIT_UEXT(set);
	int ret;

	if (unlikely(!tb[IPSET_ATTR_IP] ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_CADT_FLAGS) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_PACKETS) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_BYTES)))
		return -IPSET_ERR_PROTOCOL;
	if (unlikely(tb[IPSET_ATTR_IP_TO]))
		return -IPSET_ERR_HASH_RANGE_UNSUPPORTED;
>>>>>>> refs/remotes/origin/master

	if (tb[IPSET_ATTR_LINENO])
		*lineno = nla_get_u32(tb[IPSET_ATTR_LINENO]);

<<<<<<< HEAD
	ret = ip_set_get_ipaddr6(tb[IPSET_ATTR_IP], &data.ip);
=======
	ret = ip_set_get_ipaddr6(tb[IPSET_ATTR_IP], &e.ip) ||
	      ip_set_get_extensions(set, tb, &ext);
>>>>>>> refs/remotes/origin/master
	if (ret)
		return ret;

	if (tb[IPSET_ATTR_CIDR])
<<<<<<< HEAD
		data.cidr = nla_get_u8(tb[IPSET_ATTR_CIDR]);

<<<<<<< HEAD
	if (!data.cidr)
=======
	if (!data.cidr || data.cidr > HOST_MASK)
>>>>>>> refs/remotes/origin/cm-10.0
		return -IPSET_ERR_INVALID_CIDR;

	ip6_netmask(&data.ip, data.cidr);

	if (tb[IPSET_ATTR_TIMEOUT]) {
		if (!with_timeout(h->timeout))
			return -IPSET_ERR_TIMEOUT;
		timeout = ip_set_timeout_uget(tb[IPSET_ATTR_TIMEOUT]);
	}

<<<<<<< HEAD
	ret = adtfn(set, &data, timeout);
=======
	if (tb[IPSET_ATTR_CADT_FLAGS] && adt == IPSET_ADD) {
		u32 cadt_flags = ip_set_get_h32(tb[IPSET_ATTR_CADT_FLAGS]);
		if (cadt_flags & IPSET_FLAG_NOMATCH)
			flags |= (cadt_flags << 16);
	}

	ret = adtfn(set, &data, timeout, flags);
>>>>>>> refs/remotes/origin/cm-10.0

	return ip_set_eexist(ret, flags) ? 0 : ret;
}

/* Create hash:ip type of sets */

static int
hash_net_create(struct ip_set *set, struct nlattr *tb[], u32 flags)
{
	u32 hashsize = IPSET_DEFAULT_HASHSIZE, maxelem = IPSET_DEFAULT_MAXELEM;
	struct ip_set_hash *h;
	u8 hbits;
<<<<<<< HEAD

	if (!(set->family == AF_INET || set->family == AF_INET6))
=======
	size_t hsize;

	if (!(set->family == NFPROTO_IPV4 || set->family == NFPROTO_IPV6))
>>>>>>> refs/remotes/origin/cm-10.0
		return -IPSET_ERR_INVALID_FAMILY;

	if (unlikely(!ip_set_optattr_netorder(tb, IPSET_ATTR_HASHSIZE) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_MAXELEM) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT)))
		return -IPSET_ERR_PROTOCOL;

	if (tb[IPSET_ATTR_HASHSIZE]) {
		hashsize = ip_set_get_h32(tb[IPSET_ATTR_HASHSIZE]);
		if (hashsize < IPSET_MIMINAL_HASHSIZE)
			hashsize = IPSET_MIMINAL_HASHSIZE;
	}

	if (tb[IPSET_ATTR_MAXELEM])
		maxelem = ip_set_get_h32(tb[IPSET_ATTR_MAXELEM]);

	h = kzalloc(sizeof(*h)
		    + sizeof(struct ip_set_hash_nets)
<<<<<<< HEAD
		      * (set->family == AF_INET ? 32 : 128), GFP_KERNEL);
=======
		      * (set->family == NFPROTO_IPV4 ? 32 : 128), GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-10.0
	if (!h)
		return -ENOMEM;

	h->maxelem = maxelem;
	get_random_bytes(&h->initval, sizeof(h->initval));
	h->timeout = IPSET_NO_TIMEOUT;

	hbits = htable_bits(hashsize);
<<<<<<< HEAD
	h->table = ip_set_alloc(
			sizeof(struct htable)
			+ jhash_size(hbits) * sizeof(struct hbucket));
=======
	hsize = htable_size(hbits);
	if (hsize == 0) {
		kfree(h);
		return -ENOMEM;
	}
	h->table = ip_set_alloc(hsize);
>>>>>>> refs/remotes/origin/cm-10.0
	if (!h->table) {
		kfree(h);
		return -ENOMEM;
	}
	h->table->htable_bits = hbits;

	set->data = h;

	if (tb[IPSET_ATTR_TIMEOUT]) {
		h->timeout = ip_set_timeout_uget(tb[IPSET_ATTR_TIMEOUT]);

<<<<<<< HEAD
		set->variant = set->family == AF_INET
			? &hash_net4_tvariant : &hash_net6_tvariant;

		if (set->family == AF_INET)
=======
		set->variant = set->family == NFPROTO_IPV4
			? &hash_net4_tvariant : &hash_net6_tvariant;

		if (set->family == NFPROTO_IPV4)
>>>>>>> refs/remotes/origin/cm-10.0
			hash_net4_gc_init(set);
		else
			hash_net6_gc_init(set);
	} else {
<<<<<<< HEAD
		set->variant = set->family == AF_INET
=======
		set->variant = set->family == NFPROTO_IPV4
>>>>>>> refs/remotes/origin/cm-10.0
			? &hash_net4_variant : &hash_net6_variant;
	}

	pr_debug("create %s hashsize %u (%u) maxelem %u: %p(%p)\n",
		 set->name, jhash_size(h->table->htable_bits),
		 h->table->htable_bits, h->maxelem, set->data, h->table);

	return 0;
=======
		e.cidr = nla_get_u8(tb[IPSET_ATTR_CIDR]);

	if (!e.cidr || e.cidr > HOST_MASK)
		return -IPSET_ERR_INVALID_CIDR;

	ip6_netmask(&e.ip, e.cidr);

	if (tb[IPSET_ATTR_CADT_FLAGS]) {
		u32 cadt_flags = ip_set_get_h32(tb[IPSET_ATTR_CADT_FLAGS]);
		if (cadt_flags & IPSET_FLAG_NOMATCH)
			flags |= (IPSET_FLAG_NOMATCH << 16);
	}

	ret = adtfn(set, &e, &ext, &ext, flags);

	return ip_set_enomatch(ret, flags, adt, set) ? -ret :
	       ip_set_eexist(ret, flags) ? 0 : ret;
>>>>>>> refs/remotes/origin/master
}

static struct ip_set_type hash_net_type __read_mostly = {
	.name		= "hash:net",
	.protocol	= IPSET_PROTOCOL,
<<<<<<< HEAD
	.features	= IPSET_TYPE_IP,
	.dimension	= IPSET_DIM_ONE,
<<<<<<< HEAD
	.family		= AF_UNSPEC,
	.revision	= 0,
=======
	.family		= NFPROTO_UNSPEC,
	.revision_min	= 0,
	/*		= 1 	   Range as input support for IPv4 added */
	.revision_max	= 2,	/* nomatch flag support added */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.features	= IPSET_TYPE_IP | IPSET_TYPE_NOMATCH,
	.dimension	= IPSET_DIM_ONE,
	.family		= NFPROTO_UNSPEC,
	.revision_min	= IPSET_TYPE_REV_MIN,
	.revision_max	= IPSET_TYPE_REV_MAX,
>>>>>>> refs/remotes/origin/master
	.create		= hash_net_create,
	.create_policy	= {
		[IPSET_ATTR_HASHSIZE]	= { .type = NLA_U32 },
		[IPSET_ATTR_MAXELEM]	= { .type = NLA_U32 },
		[IPSET_ATTR_PROBES]	= { .type = NLA_U8 },
		[IPSET_ATTR_RESIZE]	= { .type = NLA_U8  },
		[IPSET_ATTR_TIMEOUT]	= { .type = NLA_U32 },
<<<<<<< HEAD
	},
	.adt_policy	= {
		[IPSET_ATTR_IP]		= { .type = NLA_NESTED },
<<<<<<< HEAD
		[IPSET_ATTR_CIDR]	= { .type = NLA_U8 },
		[IPSET_ATTR_TIMEOUT]	= { .type = NLA_U32 },
=======
=======
		[IPSET_ATTR_CADT_FLAGS]	= { .type = NLA_U32 },
	},
	.adt_policy	= {
		[IPSET_ATTR_IP]		= { .type = NLA_NESTED },
>>>>>>> refs/remotes/origin/master
		[IPSET_ATTR_IP_TO]	= { .type = NLA_NESTED },
		[IPSET_ATTR_CIDR]	= { .type = NLA_U8 },
		[IPSET_ATTR_TIMEOUT]	= { .type = NLA_U32 },
		[IPSET_ATTR_CADT_FLAGS]	= { .type = NLA_U32 },
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
		[IPSET_ATTR_BYTES]	= { .type = NLA_U64 },
		[IPSET_ATTR_PACKETS]	= { .type = NLA_U64 },
		[IPSET_ATTR_COMMENT]	= { .type = NLA_NUL_STRING },
>>>>>>> refs/remotes/origin/master
	},
	.me		= THIS_MODULE,
};

static int __init
hash_net_init(void)
{
	return ip_set_type_register(&hash_net_type);
}

static void __exit
hash_net_fini(void)
{
	ip_set_type_unregister(&hash_net_type);
}

module_init(hash_net_init);
module_exit(hash_net_fini);
