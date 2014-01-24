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

/* Kernel module implementing an IP set type: the hash:ip type */

#include <linux/jhash.h>
#include <linux/module.h>
#include <linux/ip.h>
#include <linux/skbuff.h>
#include <linux/errno.h>
#include <linux/random.h>
#include <net/ip.h>
#include <net/ipv6.h>
#include <net/netlink.h>
#include <net/tcp.h>

#include <linux/netfilter.h>
#include <linux/netfilter/ipset/pfxlen.h>
#include <linux/netfilter/ipset/ip_set.h>
<<<<<<< HEAD
#include <linux/netfilter/ipset/ip_set_timeout.h>
#include <linux/netfilter/ipset/ip_set_hash.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>");
MODULE_DESCRIPTION("hash:ip type of IP sets");
MODULE_ALIAS("ip_set_hash:ip");

/* Type specific function prefix */
#define TYPE		hash_ip

static bool
hash_ip_same_set(const struct ip_set *a, const struct ip_set *b);

#define hash_ip4_same_set	hash_ip_same_set
#define hash_ip6_same_set	hash_ip_same_set

/* The type variant functions: IPv4 */

/* Member elements without timeout */
struct hash_ip4_elem {
	__be32 ip;
};

/* Member elements with timeout support */
struct hash_ip4_telem {
	__be32 ip;
	unsigned long timeout;
};

static inline bool
hash_ip4_data_equal(const struct hash_ip4_elem *ip1,
<<<<<<< HEAD
		    const struct hash_ip4_elem *ip2)
=======
		    const struct hash_ip4_elem *ip2,
		    u32 *multi)
>>>>>>> refs/remotes/origin/cm-10.0
{
	return ip1->ip == ip2->ip;
}

static inline bool
hash_ip4_data_isnull(const struct hash_ip4_elem *elem)
{
	return elem->ip == 0;
}

static inline void
hash_ip4_data_copy(struct hash_ip4_elem *dst, const struct hash_ip4_elem *src)
{
	dst->ip = src->ip;
}

/* Zero valued IP addresses cannot be stored */
static inline void
hash_ip4_data_zero_out(struct hash_ip4_elem *elem)
{
	elem->ip = 0;
}

static inline bool
hash_ip4_data_list(struct sk_buff *skb, const struct hash_ip4_elem *data)
{
	NLA_PUT_IPADDR4(skb, IPSET_ATTR_IP, data->ip);
=======
#include <linux/netfilter/ipset/ip_set_hash.h>

#define IPSET_TYPE_REV_MIN	0
/*				1	   Counters support */
#define IPSET_TYPE_REV_MAX	2	/* Comments support */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>");
IP_SET_MODULE_DESC("hash:ip", IPSET_TYPE_REV_MIN, IPSET_TYPE_REV_MAX);
MODULE_ALIAS("ip_set_hash:ip");

/* Type specific function prefix */
#define HTYPE		hash_ip
#define IP_SET_HASH_WITH_NETMASK

/* IPv4 variant */

/* Member elements */
struct hash_ip4_elem {
	/* Zero valued IP addresses cannot be stored */
	__be32 ip;
};

/* Common functions */

static inline bool
hash_ip4_data_equal(const struct hash_ip4_elem *e1,
		    const struct hash_ip4_elem *e2,
		    u32 *multi)
{
	return e1->ip == e2->ip;
}

static inline bool
hash_ip4_data_list(struct sk_buff *skb, const struct hash_ip4_elem *e)
{
	if (nla_put_ipaddr4(skb, IPSET_ATTR_IP, e->ip))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master
	return 0;

nla_put_failure:
	return 1;
}

<<<<<<< HEAD
static bool
hash_ip4_data_tlist(struct sk_buff *skb, const struct hash_ip4_elem *data)
{
	const struct hash_ip4_telem *tdata =
		(const struct hash_ip4_telem *)data;

	NLA_PUT_IPADDR4(skb, IPSET_ATTR_IP, tdata->ip);
	NLA_PUT_NET32(skb, IPSET_ATTR_TIMEOUT,
		      htonl(ip_set_timeout_get(tdata->timeout)));

	return 0;

nla_put_failure:
	return 1;
}

#define IP_SET_HASH_WITH_NETMASK
#define PF		4
#define HOST_MASK	32
#include <linux/netfilter/ipset/ip_set_ahash.h>

<<<<<<< HEAD
static int
hash_ip4_kadt(struct ip_set *set, const struct sk_buff *skb,
	      enum ipset_adt adt, u8 pf, u8 dim, u8 flags)
=======
static inline void
hash_ip4_data_next(struct ip_set_hash *h, const struct hash_ip4_elem *d)
{
	h->next.ip = ntohl(d->ip);
}
=======
static inline void
hash_ip4_data_next(struct hash_ip4_elem *next, const struct hash_ip4_elem *e)
{
	next->ip = e->ip;
}

#define MTYPE		hash_ip4
#define PF		4
#define HOST_MASK	32
#include "ip_set_hash_gen.h"
>>>>>>> refs/remotes/origin/master

static int
hash_ip4_kadt(struct ip_set *set, const struct sk_buff *skb,
	      const struct xt_action_param *par,
<<<<<<< HEAD
	      enum ipset_adt adt, const struct ip_set_adt_opt *opt)
>>>>>>> refs/remotes/origin/cm-10.0
{
	const struct ip_set_hash *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	__be32 ip;

<<<<<<< HEAD
	ip4addrptr(skb, flags & IPSET_DIM_ONE_SRC, &ip);
=======
	ip4addrptr(skb, opt->flags & IPSET_DIM_ONE_SRC, &ip);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	      enum ipset_adt adt, struct ip_set_adt_opt *opt)
{
	const struct hash_ip *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_ip4_elem e = {};
	struct ip_set_ext ext = IP_SET_INIT_KEXT(skb, opt, set);
	__be32 ip;

	ip4addrptr(skb, opt->flags & IPSET_DIM_ONE_SRC, &ip);
>>>>>>> refs/remotes/origin/master
	ip &= ip_set_netmask(h->netmask);
	if (ip == 0)
		return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
	return adtfn(set, &ip, h->timeout);
=======
	return adtfn(set, &ip, opt_timeout(opt, h), opt->cmdflags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	e.ip = ip;
	return adtfn(set, &e, &ext, &opt->ext, opt->cmdflags);
>>>>>>> refs/remotes/origin/master
}

static int
hash_ip4_uadt(struct ip_set *set, struct nlattr *tb[],
<<<<<<< HEAD
<<<<<<< HEAD
	      enum ipset_adt adt, u32 *lineno, u32 flags)
=======
	      enum ipset_adt adt, u32 *lineno, u32 flags, bool retried)
>>>>>>> refs/remotes/origin/cm-10.0
{
	const struct ip_set_hash *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	u32 ip, ip_to, hosts, timeout = h->timeout;
	__be32 nip;
	int ret = 0;

	if (unlikely(!tb[IPSET_ATTR_IP] ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT)))
=======
	      enum ipset_adt adt, u32 *lineno, u32 flags, bool retried)
{
	const struct hash_ip *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_ip4_elem e = {};
	struct ip_set_ext ext = IP_SET_INIT_UEXT(set);
	u32 ip = 0, ip_to = 0, hosts;
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
	ret = ip_set_get_hostipaddr4(tb[IPSET_ATTR_IP], &ip);
=======
	ret = ip_set_get_hostipaddr4(tb[IPSET_ATTR_IP], &ip) ||
	      ip_set_get_extensions(set, tb, &ext);
>>>>>>> refs/remotes/origin/master
	if (ret)
		return ret;

	ip &= ip_set_hostmask(h->netmask);

<<<<<<< HEAD
	if (tb[IPSET_ATTR_TIMEOUT]) {
		if (!with_timeout(h->timeout))
			return -IPSET_ERR_TIMEOUT;
		timeout = ip_set_timeout_uget(tb[IPSET_ATTR_TIMEOUT]);
	}

	if (adt == IPSET_TEST) {
		nip = htonl(ip);
		if (nip == 0)
			return -IPSET_ERR_HASH_ELEM;
<<<<<<< HEAD
		return adtfn(set, &nip, timeout);
=======
		return adtfn(set, &nip, timeout, flags);
>>>>>>> refs/remotes/origin/cm-10.0
	}

=======
	if (adt == IPSET_TEST) {
		e.ip = htonl(ip);
		if (e.ip == 0)
			return -IPSET_ERR_HASH_ELEM;
		return adtfn(set, &e, &ext, &ext, flags);
	}

	ip_to = ip;
>>>>>>> refs/remotes/origin/master
	if (tb[IPSET_ATTR_IP_TO]) {
		ret = ip_set_get_hostipaddr4(tb[IPSET_ATTR_IP_TO], &ip_to);
		if (ret)
			return ret;
		if (ip > ip_to)
			swap(ip, ip_to);
	} else if (tb[IPSET_ATTR_CIDR]) {
		u8 cidr = nla_get_u8(tb[IPSET_ATTR_CIDR]);

<<<<<<< HEAD
		if (cidr > 32)
			return -IPSET_ERR_INVALID_CIDR;
<<<<<<< HEAD
		ip &= ip_set_hostmask(cidr);
		ip_to = ip | ~ip_set_hostmask(cidr);
=======
		ip_set_mask_from_to(ip, ip_to, cidr);
>>>>>>> refs/remotes/origin/cm-10.0
	} else
		ip_to = ip;

	hosts = h->netmask == 32 ? 1 : 2 << (32 - h->netmask - 1);

<<<<<<< HEAD
=======
	if (retried)
		ip = h->next.ip;
>>>>>>> refs/remotes/origin/cm-10.0
	for (; !before(ip_to, ip); ip += hosts) {
		nip = htonl(ip);
		if (nip == 0)
			return -IPSET_ERR_HASH_ELEM;
<<<<<<< HEAD
		ret = adtfn(set, &nip, timeout);
=======
		ret = adtfn(set, &nip, timeout, flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (!cidr || cidr > 32)
			return -IPSET_ERR_INVALID_CIDR;
		ip_set_mask_from_to(ip, ip_to, cidr);
	}

	hosts = h->netmask == 32 ? 1 : 2 << (32 - h->netmask - 1);

	if (retried)
		ip = ntohl(h->next.ip);
	for (; !before(ip_to, ip); ip += hosts) {
		e.ip = htonl(ip);
		if (e.ip == 0)
			return -IPSET_ERR_HASH_ELEM;
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
static bool
hash_ip_same_set(const struct ip_set *a, const struct ip_set *b)
{
	const struct ip_set_hash *x = a->data;
	const struct ip_set_hash *y = b->data;

	/* Resizing changes htable_bits, so we ignore it */
	return x->maxelem == y->maxelem &&
	       x->timeout == y->timeout &&
	       x->netmask == y->netmask;
}

/* The type variant functions: IPv6 */

=======
/* IPv6 variant */

/* Member elements */
>>>>>>> refs/remotes/origin/master
struct hash_ip6_elem {
	union nf_inet_addr ip;
};

<<<<<<< HEAD
struct hash_ip6_telem {
	union nf_inet_addr ip;
	unsigned long timeout;
};

static inline bool
hash_ip6_data_equal(const struct hash_ip6_elem *ip1,
<<<<<<< HEAD
		    const struct hash_ip6_elem *ip2)
=======
		    const struct hash_ip6_elem *ip2,
		    u32 *multi)
>>>>>>> refs/remotes/origin/cm-10.0
{
	return ipv6_addr_cmp(&ip1->ip.in6, &ip2->ip.in6) == 0;
}

static inline bool
hash_ip6_data_isnull(const struct hash_ip6_elem *elem)
{
	return ipv6_addr_any(&elem->ip.in6);
}

static inline void
hash_ip6_data_copy(struct hash_ip6_elem *dst, const struct hash_ip6_elem *src)
{
<<<<<<< HEAD
	ipv6_addr_copy(&dst->ip.in6, &src->ip.in6);
=======
	dst->ip.in6 = src->ip.in6;
>>>>>>> refs/remotes/origin/cm-10.0
}

static inline void
hash_ip6_data_zero_out(struct hash_ip6_elem *elem)
{
	ipv6_addr_set(&elem->ip.in6, 0, 0, 0, 0);
}

static inline void
ip6_netmask(union nf_inet_addr *ip, u8 prefix)
{
	ip->ip6[0] &= ip_set_netmask6(prefix)[0];
	ip->ip6[1] &= ip_set_netmask6(prefix)[1];
	ip->ip6[2] &= ip_set_netmask6(prefix)[2];
	ip->ip6[3] &= ip_set_netmask6(prefix)[3];
}

static bool
hash_ip6_data_list(struct sk_buff *skb, const struct hash_ip6_elem *data)
{
	NLA_PUT_IPADDR6(skb, IPSET_ATTR_IP, &data->ip);
=======
/* Common functions */

static inline bool
hash_ip6_data_equal(const struct hash_ip6_elem *ip1,
		    const struct hash_ip6_elem *ip2,
		    u32 *multi)
{
	return ipv6_addr_equal(&ip1->ip.in6, &ip2->ip.in6);
}

static inline void
hash_ip6_netmask(union nf_inet_addr *ip, u8 prefix)
{
	ip6_netmask(ip, prefix);
}

static bool
hash_ip6_data_list(struct sk_buff *skb, const struct hash_ip6_elem *e)
{
	if (nla_put_ipaddr6(skb, IPSET_ATTR_IP, &e->ip.in6))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master
	return 0;

nla_put_failure:
	return 1;
}

<<<<<<< HEAD
static bool
hash_ip6_data_tlist(struct sk_buff *skb, const struct hash_ip6_elem *data)
{
	const struct hash_ip6_telem *e =
		(const struct hash_ip6_telem *)data;

	NLA_PUT_IPADDR6(skb, IPSET_ATTR_IP, &e->ip);
	NLA_PUT_NET32(skb, IPSET_ATTR_TIMEOUT,
		      htonl(ip_set_timeout_get(e->timeout)));
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
hash_ip6_kadt(struct ip_set *set, const struct sk_buff *skb,
	      enum ipset_adt adt, u8 pf, u8 dim, u8 flags)
=======
static inline void
hash_ip6_data_next(struct ip_set_hash *h, const struct hash_ip6_elem *d)
{
}
=======
static inline void
hash_ip6_data_next(struct hash_ip4_elem *next, const struct hash_ip6_elem *e)
{
}

#undef MTYPE
#undef PF
#undef HOST_MASK
#undef HKEY_DATALEN

#define MTYPE		hash_ip6
#define PF		6
#define HOST_MASK	128

#define IP_SET_EMIT_CREATE
#include "ip_set_hash_gen.h"
>>>>>>> refs/remotes/origin/master

static int
hash_ip6_kadt(struct ip_set *set, const struct sk_buff *skb,
	      const struct xt_action_param *par,
<<<<<<< HEAD
	      enum ipset_adt adt, const struct ip_set_adt_opt *opt)
>>>>>>> refs/remotes/origin/cm-10.0
{
	const struct ip_set_hash *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	union nf_inet_addr ip;

<<<<<<< HEAD
	ip6addrptr(skb, flags & IPSET_DIM_ONE_SRC, &ip.in6);
=======
	ip6addrptr(skb, opt->flags & IPSET_DIM_ONE_SRC, &ip.in6);
>>>>>>> refs/remotes/origin/cm-10.0
	ip6_netmask(&ip, h->netmask);
	if (ipv6_addr_any(&ip.in6))
		return -EINVAL;

<<<<<<< HEAD
	return adtfn(set, &ip, h->timeout);
=======
	return adtfn(set, &ip, opt_timeout(opt, h), opt->cmdflags);
>>>>>>> refs/remotes/origin/cm-10.0
}

static const struct nla_policy hash_ip6_adt_policy[IPSET_ATTR_ADT_MAX + 1] = {
	[IPSET_ATTR_IP]		= { .type = NLA_NESTED },
	[IPSET_ATTR_TIMEOUT]	= { .type = NLA_U32 },
	[IPSET_ATTR_LINENO]	= { .type = NLA_U32 },
};

static int
hash_ip6_uadt(struct ip_set *set, struct nlattr *tb[],
<<<<<<< HEAD
	      enum ipset_adt adt, u32 *lineno, u32 flags)
=======
	      enum ipset_adt adt, u32 *lineno, u32 flags, bool retried)
>>>>>>> refs/remotes/origin/cm-10.0
{
	const struct ip_set_hash *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	union nf_inet_addr ip;
	u32 timeout = h->timeout;
=======
	      enum ipset_adt adt, struct ip_set_adt_opt *opt)
{
	const struct hash_ip *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_ip6_elem e = {};
	struct ip_set_ext ext = IP_SET_INIT_KEXT(skb, opt, set);

	ip6addrptr(skb, opt->flags & IPSET_DIM_ONE_SRC, &e.ip.in6);
	hash_ip6_netmask(&e.ip, h->netmask);
	if (ipv6_addr_any(&e.ip.in6))
		return -EINVAL;

	return adtfn(set, &e, &ext, &opt->ext, opt->cmdflags);
}

static int
hash_ip6_uadt(struct ip_set *set, struct nlattr *tb[],
	      enum ipset_adt adt, u32 *lineno, u32 flags, bool retried)
{
	const struct hash_ip *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_ip6_elem e = {};
	struct ip_set_ext ext = IP_SET_INIT_UEXT(set);
>>>>>>> refs/remotes/origin/master
	int ret;

	if (unlikely(!tb[IPSET_ATTR_IP] ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT) ||
<<<<<<< HEAD
=======
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_PACKETS) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_BYTES) ||
>>>>>>> refs/remotes/origin/master
		     tb[IPSET_ATTR_IP_TO] ||
		     tb[IPSET_ATTR_CIDR]))
		return -IPSET_ERR_PROTOCOL;

	if (tb[IPSET_ATTR_LINENO])
		*lineno = nla_get_u32(tb[IPSET_ATTR_LINENO]);

<<<<<<< HEAD
	ret = ip_set_get_ipaddr6(tb[IPSET_ATTR_IP], &ip);
	if (ret)
		return ret;

	ip6_netmask(&ip, h->netmask);
	if (ipv6_addr_any(&ip.in6))
		return -IPSET_ERR_HASH_ELEM;

	if (tb[IPSET_ATTR_TIMEOUT]) {
		if (!with_timeout(h->timeout))
			return -IPSET_ERR_TIMEOUT;
		timeout = ip_set_timeout_uget(tb[IPSET_ATTR_TIMEOUT]);
	}

<<<<<<< HEAD
	ret = adtfn(set, &ip, timeout);
=======
	ret = adtfn(set, &ip, timeout, flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = ip_set_get_ipaddr6(tb[IPSET_ATTR_IP], &e.ip) ||
	      ip_set_get_extensions(set, tb, &ext);
	if (ret)
		return ret;

	hash_ip6_netmask(&e.ip, h->netmask);
	if (ipv6_addr_any(&e.ip.in6))
		return -IPSET_ERR_HASH_ELEM;

	ret = adtfn(set, &e, &ext, &ext, flags);
>>>>>>> refs/remotes/origin/master

	return ip_set_eexist(ret, flags) ? 0 : ret;
}

<<<<<<< HEAD
/* Create hash:ip type of sets */

static int
hash_ip_create(struct ip_set *set, struct nlattr *tb[], u32 flags)
{
	u32 hashsize = IPSET_DEFAULT_HASHSIZE, maxelem = IPSET_DEFAULT_MAXELEM;
	u8 netmask, hbits;
<<<<<<< HEAD
	struct ip_set_hash *h;

	if (!(set->family == AF_INET || set->family == AF_INET6))
		return -IPSET_ERR_INVALID_FAMILY;
	netmask = set->family == AF_INET ? 32 : 128;
	pr_debug("Create set %s with family %s\n",
		 set->name, set->family == AF_INET ? "inet" : "inet6");
=======
	size_t hsize;
	struct ip_set_hash *h;

	if (!(set->family == NFPROTO_IPV4 || set->family == NFPROTO_IPV6))
		return -IPSET_ERR_INVALID_FAMILY;
	netmask = set->family == NFPROTO_IPV4 ? 32 : 128;
	pr_debug("Create set %s with family %s\n",
		 set->name, set->family == NFPROTO_IPV4 ? "inet" : "inet6");
>>>>>>> refs/remotes/origin/cm-10.0

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

	if (tb[IPSET_ATTR_NETMASK]) {
		netmask = nla_get_u8(tb[IPSET_ATTR_NETMASK]);

<<<<<<< HEAD
		if ((set->family == AF_INET && netmask > 32) ||
		    (set->family == AF_INET6 && netmask > 128) ||
=======
		if ((set->family == NFPROTO_IPV4 && netmask > 32) ||
		    (set->family == NFPROTO_IPV6 && netmask > 128) ||
>>>>>>> refs/remotes/origin/cm-10.0
		    netmask == 0)
			return -IPSET_ERR_INVALID_NETMASK;
	}

	h = kzalloc(sizeof(*h), GFP_KERNEL);
	if (!h)
		return -ENOMEM;

	h->maxelem = maxelem;
	h->netmask = netmask;
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
			? &hash_ip4_tvariant : &hash_ip6_tvariant;

		if (set->family == AF_INET)
=======
		set->variant = set->family == NFPROTO_IPV4
			? &hash_ip4_tvariant : &hash_ip6_tvariant;

		if (set->family == NFPROTO_IPV4)
>>>>>>> refs/remotes/origin/cm-10.0
			hash_ip4_gc_init(set);
		else
			hash_ip6_gc_init(set);
	} else {
<<<<<<< HEAD
		set->variant = set->family == AF_INET
=======
		set->variant = set->family == NFPROTO_IPV4
>>>>>>> refs/remotes/origin/cm-10.0
			? &hash_ip4_variant : &hash_ip6_variant;
	}

	pr_debug("create %s hashsize %u (%u) maxelem %u: %p(%p)\n",
		 set->name, jhash_size(h->table->htable_bits),
		 h->table->htable_bits, h->maxelem, set->data, h->table);

	return 0;
}

=======
>>>>>>> refs/remotes/origin/master
static struct ip_set_type hash_ip_type __read_mostly = {
	.name		= "hash:ip",
	.protocol	= IPSET_PROTOCOL,
	.features	= IPSET_TYPE_IP,
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
	.create		= hash_ip_create,
	.create_policy	= {
		[IPSET_ATTR_HASHSIZE]	= { .type = NLA_U32 },
		[IPSET_ATTR_MAXELEM]	= { .type = NLA_U32 },
		[IPSET_ATTR_PROBES]	= { .type = NLA_U8 },
		[IPSET_ATTR_RESIZE]	= { .type = NLA_U8  },
		[IPSET_ATTR_TIMEOUT]	= { .type = NLA_U32 },
		[IPSET_ATTR_NETMASK]	= { .type = NLA_U8  },
<<<<<<< HEAD
=======
		[IPSET_ATTR_CADT_FLAGS]	= { .type = NLA_U32 },
>>>>>>> refs/remotes/origin/master
	},
	.adt_policy	= {
		[IPSET_ATTR_IP]		= { .type = NLA_NESTED },
		[IPSET_ATTR_IP_TO]	= { .type = NLA_NESTED },
		[IPSET_ATTR_CIDR]	= { .type = NLA_U8 },
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
hash_ip_init(void)
{
	return ip_set_type_register(&hash_ip_type);
}

static void __exit
hash_ip_fini(void)
{
	ip_set_type_unregister(&hash_ip_type);
}

module_init(hash_ip_init);
module_exit(hash_ip_fini);
