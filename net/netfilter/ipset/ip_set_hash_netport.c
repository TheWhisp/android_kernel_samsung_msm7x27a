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

/* Kernel module implementing an IP set type: the hash:net,port type */

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
#include <linux/netfilter/ipset/ip_set_getport.h>
#include <linux/netfilter/ipset/ip_set_hash.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>");
MODULE_DESCRIPTION("hash:net,port type of IP sets");
MODULE_ALIAS("ip_set_hash:net,port");

/* Type specific function prefix */
#define TYPE		hash_netport

static bool
hash_netport_same_set(const struct ip_set *a, const struct ip_set *b);

#define hash_netport4_same_set	hash_netport_same_set
#define hash_netport6_same_set	hash_netport_same_set

/* The type variant functions: IPv4 */

<<<<<<< HEAD
=======
=======
#include <linux/netfilter/ipset/ip_set_getport.h>
#include <linux/netfilter/ipset/ip_set_hash.h>

#define IPSET_TYPE_REV_MIN	0
/*				1    SCTP and UDPLITE support added */
/*				2    Range as input support for IPv4 added */
/*				3    nomatch flag support added */
/*				4    Counters support added */
#define IPSET_TYPE_REV_MAX	5 /* Comments support added */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>");
IP_SET_MODULE_DESC("hash:net,port", IPSET_TYPE_REV_MIN, IPSET_TYPE_REV_MAX);
MODULE_ALIAS("ip_set_hash:net,port");

/* Type specific function prefix */
#define HTYPE		hash_netport
#define IP_SET_HASH_WITH_PROTO
#define IP_SET_HASH_WITH_NETS

>>>>>>> refs/remotes/origin/master
/* We squeeze the "nomatch" flag into cidr: we don't support cidr == 0
 * However this way we have to store internally cidr - 1,
 * dancing back and forth.
 */
#define IP_SET_HASH_WITH_NETS_PACKED

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
/* Member elements without timeout */
=======
/* IPv4 variant */

/* Member elements */
>>>>>>> refs/remotes/origin/master
struct hash_netport4_elem {
	__be32 ip;
	__be16 port;
	u8 proto;
<<<<<<< HEAD
<<<<<<< HEAD
	u8 cidr;
=======
	u8 cidr:7;
	u8 nomatch:1;
>>>>>>> refs/remotes/origin/cm-10.0
};

/* Member elements with timeout support */
struct hash_netport4_telem {
	__be32 ip;
	__be16 port;
	u8 proto;
<<<<<<< HEAD
	u8 cidr;
=======
	u8 cidr:7;
	u8 nomatch:1;
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned long timeout;
};

static inline bool
hash_netport4_data_equal(const struct hash_netport4_elem *ip1,
<<<<<<< HEAD
			 const struct hash_netport4_elem *ip2)
=======
			 const struct hash_netport4_elem *ip2,
			 u32 *multi)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u8 cidr:7;
	u8 nomatch:1;
};

/* Common functions */

static inline bool
hash_netport4_data_equal(const struct hash_netport4_elem *ip1,
			 const struct hash_netport4_elem *ip2,
			 u32 *multi)
>>>>>>> refs/remotes/origin/master
{
	return ip1->ip == ip2->ip &&
	       ip1->port == ip2->port &&
	       ip1->proto == ip2->proto &&
	       ip1->cidr == ip2->cidr;
}

<<<<<<< HEAD
static inline bool
hash_netport4_data_isnull(const struct hash_netport4_elem *elem)
{
	return elem->proto == 0;
}

static inline void
hash_netport4_data_copy(struct hash_netport4_elem *dst,
			const struct hash_netport4_elem *src)
{
	dst->ip = src->ip;
	dst->port = src->port;
	dst->proto = src->proto;
	dst->cidr = src->cidr;
<<<<<<< HEAD
=======
	dst->nomatch = src->nomatch;
}

static inline void
hash_netport4_data_flags(struct hash_netport4_elem *dst, u32 flags)
{
	dst->nomatch = !!(flags & IPSET_FLAG_NOMATCH);
}

static inline bool
hash_netport4_data_match(const struct hash_netport4_elem *elem)
{
	return !elem->nomatch;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline int
hash_netport4_do_data_match(const struct hash_netport4_elem *elem)
{
	return elem->nomatch ? -ENOTEMPTY : 1;
}

static inline void
hash_netport4_data_set_flags(struct hash_netport4_elem *elem, u32 flags)
{
	elem->nomatch = !!((flags >> 16) & IPSET_FLAG_NOMATCH);
}

static inline void
hash_netport4_data_reset_flags(struct hash_netport4_elem *elem, u8 *flags)
{
	swap(*flags, elem->nomatch);
>>>>>>> refs/remotes/origin/master
}

static inline void
hash_netport4_data_netmask(struct hash_netport4_elem *elem, u8 cidr)
{
	elem->ip &= ip_set_netmask(cidr);
<<<<<<< HEAD
<<<<<<< HEAD
	elem->cidr = cidr;
=======
	elem->cidr = cidr - 1;
>>>>>>> refs/remotes/origin/cm-10.0
}

static inline void
hash_netport4_data_zero_out(struct hash_netport4_elem *elem)
{
	elem->proto = 0;
=======
	elem->cidr = cidr - 1;
>>>>>>> refs/remotes/origin/master
}

static bool
hash_netport4_data_list(struct sk_buff *skb,
			const struct hash_netport4_elem *data)
{
<<<<<<< HEAD
<<<<<<< HEAD
	NLA_PUT_IPADDR4(skb, IPSET_ATTR_IP, data->ip);
	NLA_PUT_NET16(skb, IPSET_ATTR_PORT, data->port);
	NLA_PUT_U8(skb, IPSET_ATTR_CIDR, data->cidr);
	NLA_PUT_U8(skb, IPSET_ATTR_PROTO, data->proto);
=======
	u32 flags = data->nomatch ? IPSET_FLAG_NOMATCH : 0;

	NLA_PUT_IPADDR4(skb, IPSET_ATTR_IP, data->ip);
	NLA_PUT_NET16(skb, IPSET_ATTR_PORT, data->port);
	NLA_PUT_U8(skb, IPSET_ATTR_CIDR, data->cidr + 1);
	NLA_PUT_U8(skb, IPSET_ATTR_PROTO, data->proto);
	if (flags)
		NLA_PUT_NET32(skb, IPSET_ATTR_CADT_FLAGS, htonl(flags));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u32 flags = data->nomatch ? IPSET_FLAG_NOMATCH : 0;

	if (nla_put_ipaddr4(skb, IPSET_ATTR_IP, data->ip) ||
	    nla_put_net16(skb, IPSET_ATTR_PORT, data->port) ||
	    nla_put_u8(skb, IPSET_ATTR_CIDR, data->cidr + 1) ||
	    nla_put_u8(skb, IPSET_ATTR_PROTO, data->proto) ||
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
hash_netport4_data_tlist(struct sk_buff *skb,
			 const struct hash_netport4_elem *data)
{
	const struct hash_netport4_telem *tdata =
		(const struct hash_netport4_telem *)data;
<<<<<<< HEAD

	NLA_PUT_IPADDR4(skb, IPSET_ATTR_IP, tdata->ip);
	NLA_PUT_NET16(skb, IPSET_ATTR_PORT, tdata->port);
	NLA_PUT_U8(skb, IPSET_ATTR_CIDR, data->cidr);
	NLA_PUT_U8(skb, IPSET_ATTR_PROTO, data->proto);
	NLA_PUT_NET32(skb, IPSET_ATTR_TIMEOUT,
		      htonl(ip_set_timeout_get(tdata->timeout)));
=======
	u32 flags = data->nomatch ? IPSET_FLAG_NOMATCH : 0;

	NLA_PUT_IPADDR4(skb, IPSET_ATTR_IP, tdata->ip);
	NLA_PUT_NET16(skb, IPSET_ATTR_PORT, tdata->port);
	NLA_PUT_U8(skb, IPSET_ATTR_CIDR, data->cidr + 1);
	NLA_PUT_U8(skb, IPSET_ATTR_PROTO, data->proto);
	NLA_PUT_NET32(skb, IPSET_ATTR_TIMEOUT,
		      htonl(ip_set_timeout_get(tdata->timeout)));
	if (flags)
		NLA_PUT_NET32(skb, IPSET_ATTR_CADT_FLAGS, htonl(flags));
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;

nla_put_failure:
	return 1;
}

#define IP_SET_HASH_WITH_PROTO
#define IP_SET_HASH_WITH_NETS

#define PF		4
#define HOST_MASK	32
#include <linux/netfilter/ipset/ip_set_ahash.h>

<<<<<<< HEAD
static int
hash_netport4_kadt(struct ip_set *set, const struct sk_buff *skb,
		   enum ipset_adt adt, u8 pf, u8 dim, u8 flags)
=======
static inline void
hash_netport4_data_next(struct ip_set_hash *h,
			const struct hash_netport4_elem *d)
{
	h->next.ip = ntohl(d->ip);
	h->next.port = ntohs(d->port);
}
=======
static inline void
hash_netport4_data_next(struct hash_netport4_elem *next,
			const struct hash_netport4_elem *d)
{
	next->ip = d->ip;
	next->port = d->port;
}

#define MTYPE		hash_netport4
#define PF		4
#define HOST_MASK	32
#include "ip_set_hash_gen.h"
>>>>>>> refs/remotes/origin/master

static int
hash_netport4_kadt(struct ip_set *set, const struct sk_buff *skb,
		   const struct xt_action_param *par,
<<<<<<< HEAD
		   enum ipset_adt adt, const struct ip_set_adt_opt *opt)
>>>>>>> refs/remotes/origin/cm-10.0
{
	const struct ip_set_hash *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_netport4_elem data = {
<<<<<<< HEAD
		.cidr = h->nets[0].cidr ? h->nets[0].cidr : HOST_MASK
	};

	if (data.cidr == 0)
		return -EINVAL;
	if (adt == IPSET_TEST)
		data.cidr = HOST_MASK;

	if (!ip_set_get_ip4_port(skb, flags & IPSET_DIM_TWO_SRC,
				 &data.port, &data.proto))
		return -EINVAL;

	ip4addrptr(skb, flags & IPSET_DIM_ONE_SRC, &data.ip);
	data.ip &= ip_set_netmask(data.cidr);

	return adtfn(set, &data, h->timeout);
=======
		.cidr = h->nets[0].cidr ? h->nets[0].cidr - 1 : HOST_MASK - 1
	};

	if (adt == IPSET_TEST)
		data.cidr = HOST_MASK - 1;

	if (!ip_set_get_ip4_port(skb, opt->flags & IPSET_DIM_TWO_SRC,
				 &data.port, &data.proto))
		return -EINVAL;

	ip4addrptr(skb, opt->flags & IPSET_DIM_ONE_SRC, &data.ip);
	data.ip &= ip_set_netmask(data.cidr + 1);

	return adtfn(set, &data, opt_timeout(opt, h), opt->cmdflags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		   enum ipset_adt adt, struct ip_set_adt_opt *opt)
{
	const struct hash_netport *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_netport4_elem e = {
		.cidr = IP_SET_INIT_CIDR(h->nets[0].cidr[0], HOST_MASK) - 1,
	};
	struct ip_set_ext ext = IP_SET_INIT_KEXT(skb, opt, set);

	if (adt == IPSET_TEST)
		e.cidr = HOST_MASK - 1;

	if (!ip_set_get_ip4_port(skb, opt->flags & IPSET_DIM_TWO_SRC,
				 &e.port, &e.proto))
		return -EINVAL;

	ip4addrptr(skb, opt->flags & IPSET_DIM_ONE_SRC, &e.ip);
	e.ip &= ip_set_netmask(e.cidr + 1);

	return adtfn(set, &e, &ext, &opt->ext, opt->cmdflags);
>>>>>>> refs/remotes/origin/master
}

static int
hash_netport4_uadt(struct ip_set *set, struct nlattr *tb[],
<<<<<<< HEAD
<<<<<<< HEAD
		   enum ipset_adt adt, u32 *lineno, u32 flags)
{
	const struct ip_set_hash *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_netport4_elem data = { .cidr = HOST_MASK };
	u32 port, port_to;
	u32 timeout = h->timeout;
	bool with_ports = false;
=======
		   enum ipset_adt adt, u32 *lineno, u32 flags, bool retried)
{
	const struct ip_set_hash *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_netport4_elem data = { .cidr = HOST_MASK - 1 };
	u32 port, port_to, p = 0, ip = 0, ip_to, last;
	u32 timeout = h->timeout;
	bool with_ports = false;
	u8 cidr;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		   enum ipset_adt adt, u32 *lineno, u32 flags, bool retried)
{
	const struct hash_netport *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_netport4_elem e = { .cidr = HOST_MASK - 1 };
	struct ip_set_ext ext = IP_SET_INIT_UEXT(set);
	u32 port, port_to, p = 0, ip = 0, ip_to = 0, last;
	bool with_ports = false;
	u8 cidr;
>>>>>>> refs/remotes/origin/master
	int ret;

	if (unlikely(!tb[IPSET_ATTR_IP] ||
		     !ip_set_attr_netorder(tb, IPSET_ATTR_PORT) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_PORT_TO) ||
<<<<<<< HEAD
<<<<<<< HEAD
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT)))
=======
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_CADT_FLAGS)))
>>>>>>> refs/remotes/origin/cm-10.0
=======
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT) ||
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
		cidr = nla_get_u8(tb[IPSET_ATTR_CIDR]);
		if (!cidr || cidr > HOST_MASK)
			return -IPSET_ERR_INVALID_CIDR;
<<<<<<< HEAD
		data.cidr = cidr - 1;
	}
>>>>>>> refs/remotes/origin/cm-10.0

	if (tb[IPSET_ATTR_PORT])
		data.port = nla_get_be16(tb[IPSET_ATTR_PORT]);
=======
		e.cidr = cidr - 1;
	}

	if (tb[IPSET_ATTR_PORT])
		e.port = nla_get_be16(tb[IPSET_ATTR_PORT]);
>>>>>>> refs/remotes/origin/master
	else
		return -IPSET_ERR_PROTOCOL;

	if (tb[IPSET_ATTR_PROTO]) {
<<<<<<< HEAD
		data.proto = nla_get_u8(tb[IPSET_ATTR_PROTO]);
		with_ports = ip_set_proto_with_ports(data.proto);

		if (data.proto == 0)
=======
		e.proto = nla_get_u8(tb[IPSET_ATTR_PROTO]);
		with_ports = ip_set_proto_with_ports(e.proto);

		if (e.proto == 0)
>>>>>>> refs/remotes/origin/master
			return -IPSET_ERR_INVALID_PROTO;
	} else
		return -IPSET_ERR_MISSING_PROTO;

<<<<<<< HEAD
	if (!(with_ports || data.proto == IPPROTO_ICMP))
		data.port = 0;

	if (tb[IPSET_ATTR_TIMEOUT]) {
		if (!with_timeout(h->timeout))
			return -IPSET_ERR_TIMEOUT;
		timeout = ip_set_timeout_uget(tb[IPSET_ATTR_TIMEOUT]);
	}

<<<<<<< HEAD
	if (adt == IPSET_TEST || !with_ports || !tb[IPSET_ATTR_PORT_TO]) {
		ret = adtfn(set, &data, timeout);
		return ip_set_eexist(ret, flags) ? 0 : ret;
	}

	port = ntohs(data.port);
	port_to = ip_set_get_h16(tb[IPSET_ATTR_PORT_TO]);
	if (port > port_to)
		swap(port, port_to);

	for (; port <= port_to; port++) {
		data.port = htons(port);
		ret = adtfn(set, &data, timeout);

		if (ret && !ip_set_eexist(ret, flags))
			return ret;
		else
			ret = 0;
=======
	with_ports = with_ports && tb[IPSET_ATTR_PORT_TO];

	if (tb[IPSET_ATTR_CADT_FLAGS] && adt == IPSET_ADD) {
		u32 cadt_flags = ip_set_get_h32(tb[IPSET_ATTR_CADT_FLAGS]);
		if (cadt_flags & IPSET_FLAG_NOMATCH)
			flags |= (cadt_flags << 16);
	}

	if (adt == IPSET_TEST || !(with_ports || tb[IPSET_ATTR_IP_TO])) {
		data.ip = htonl(ip & ip_set_hostmask(data.cidr + 1));
		ret = adtfn(set, &data, timeout, flags);
		return ip_set_eexist(ret, flags) ? 0 : ret;
	}

	port = port_to = ntohs(data.port);
=======
	if (!(with_ports || e.proto == IPPROTO_ICMP))
		e.port = 0;

	with_ports = with_ports && tb[IPSET_ATTR_PORT_TO];

	if (tb[IPSET_ATTR_CADT_FLAGS]) {
		u32 cadt_flags = ip_set_get_h32(tb[IPSET_ATTR_CADT_FLAGS]);
		if (cadt_flags & IPSET_FLAG_NOMATCH)
			flags |= (IPSET_FLAG_NOMATCH << 16);
	}

	if (adt == IPSET_TEST || !(with_ports || tb[IPSET_ATTR_IP_TO])) {
		e.ip = htonl(ip & ip_set_hostmask(e.cidr + 1));
		ret = adtfn(set, &e, &ext, &ext, flags);
		return ip_set_enomatch(ret, flags, adt, set) ? -ret :
		       ip_set_eexist(ret, flags) ? 0 : ret;
	}

	port = port_to = ntohs(e.port);
>>>>>>> refs/remotes/origin/master
	if (tb[IPSET_ATTR_PORT_TO]) {
		port_to = ip_set_get_h16(tb[IPSET_ATTR_PORT_TO]);
		if (port_to < port)
			swap(port, port_to);
	}
	if (tb[IPSET_ATTR_IP_TO]) {
		ret = ip_set_get_hostipaddr4(tb[IPSET_ATTR_IP_TO], &ip_to);
		if (ret)
			return ret;
		if (ip_to < ip)
			swap(ip, ip_to);
		if (ip + UINT_MAX == ip_to)
			return -IPSET_ERR_HASH_RANGE;
<<<<<<< HEAD
	} else {
		ip_set_mask_from_to(ip, ip_to, data.cidr + 1);
	}

	if (retried)
		ip = h->next.ip;
	while (!after(ip, ip_to)) {
		data.ip = htonl(ip);
		last = ip_set_range_to_cidr(ip, ip_to, &cidr);
		data.cidr = cidr - 1;
		p = retried && ip == h->next.ip ? h->next.port : port;
		for (; p <= port_to; p++) {
			data.port = htons(p);
			ret = adtfn(set, &data, timeout, flags);
=======
	} else
		ip_set_mask_from_to(ip, ip_to, e.cidr + 1);

	if (retried)
		ip = ntohl(h->next.ip);
	while (!after(ip, ip_to)) {
		e.ip = htonl(ip);
		last = ip_set_range_to_cidr(ip, ip_to, &cidr);
		e.cidr = cidr - 1;
		p = retried && ip == ntohl(h->next.ip) ? ntohs(h->next.port)
						       : port;
		for (; p <= port_to; p++) {
			e.port = htons(p);
			ret = adtfn(set, &e, &ext, &ext, flags);
>>>>>>> refs/remotes/origin/master

			if (ret && !ip_set_eexist(ret, flags))
				return ret;
			else
				ret = 0;
		}
		ip = last + 1;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}
	return ret;
}

<<<<<<< HEAD
static bool
hash_netport_same_set(const struct ip_set *a, const struct ip_set *b)
{
	const struct ip_set_hash *x = a->data;
	const struct ip_set_hash *y = b->data;

	/* Resizing changes htable_bits, so we ignore it */
	return x->maxelem == y->maxelem &&
	       x->timeout == y->timeout;
}

/* The type variant functions: IPv6 */
=======
/* IPv6 variant */
>>>>>>> refs/remotes/origin/master

struct hash_netport6_elem {
	union nf_inet_addr ip;
	__be16 port;
	u8 proto;
<<<<<<< HEAD
<<<<<<< HEAD
	u8 cidr;
=======
	u8 cidr:7;
	u8 nomatch:1;
>>>>>>> refs/remotes/origin/cm-10.0
};

struct hash_netport6_telem {
	union nf_inet_addr ip;
	__be16 port;
	u8 proto;
<<<<<<< HEAD
	u8 cidr;
=======
	u8 cidr:7;
	u8 nomatch:1;
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned long timeout;
};

static inline bool
hash_netport6_data_equal(const struct hash_netport6_elem *ip1,
<<<<<<< HEAD
			 const struct hash_netport6_elem *ip2)
=======
			 const struct hash_netport6_elem *ip2,
			 u32 *multi)
>>>>>>> refs/remotes/origin/cm-10.0
{
	return ipv6_addr_cmp(&ip1->ip.in6, &ip2->ip.in6) == 0 &&
=======
	u8 cidr:7;
	u8 nomatch:1;
};

/* Common functions */

static inline bool
hash_netport6_data_equal(const struct hash_netport6_elem *ip1,
			 const struct hash_netport6_elem *ip2,
			 u32 *multi)
{
	return ipv6_addr_equal(&ip1->ip.in6, &ip2->ip.in6) &&
>>>>>>> refs/remotes/origin/master
	       ip1->port == ip2->port &&
	       ip1->proto == ip2->proto &&
	       ip1->cidr == ip2->cidr;
}

<<<<<<< HEAD
static inline bool
hash_netport6_data_isnull(const struct hash_netport6_elem *elem)
{
	return elem->proto == 0;
}

static inline void
hash_netport6_data_copy(struct hash_netport6_elem *dst,
			const struct hash_netport6_elem *src)
{
	memcpy(dst, src, sizeof(*dst));
}

static inline void
<<<<<<< HEAD
=======
hash_netport6_data_flags(struct hash_netport6_elem *dst, u32 flags)
{
	dst->nomatch = !!(flags & IPSET_FLAG_NOMATCH);
}

static inline bool
hash_netport6_data_match(const struct hash_netport6_elem *elem)
{
	return !elem->nomatch;
}

static inline void
>>>>>>> refs/remotes/origin/cm-10.0
hash_netport6_data_zero_out(struct hash_netport6_elem *elem)
{
	elem->proto = 0;
}

static inline void
ip6_netmask(union nf_inet_addr *ip, u8 prefix)
{
	ip->ip6[0] &= ip_set_netmask6(prefix)[0];
	ip->ip6[1] &= ip_set_netmask6(prefix)[1];
	ip->ip6[2] &= ip_set_netmask6(prefix)[2];
	ip->ip6[3] &= ip_set_netmask6(prefix)[3];
=======
static inline int
hash_netport6_do_data_match(const struct hash_netport6_elem *elem)
{
	return elem->nomatch ? -ENOTEMPTY : 1;
}

static inline void
hash_netport6_data_set_flags(struct hash_netport6_elem *elem, u32 flags)
{
	elem->nomatch = !!((flags >> 16) & IPSET_FLAG_NOMATCH);
}

static inline void
hash_netport6_data_reset_flags(struct hash_netport6_elem *elem, u8 *flags)
{
	swap(*flags, elem->nomatch);
>>>>>>> refs/remotes/origin/master
}

static inline void
hash_netport6_data_netmask(struct hash_netport6_elem *elem, u8 cidr)
{
	ip6_netmask(&elem->ip, cidr);
<<<<<<< HEAD
<<<<<<< HEAD
	elem->cidr = cidr;
=======
	elem->cidr = cidr - 1;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	elem->cidr = cidr - 1;
>>>>>>> refs/remotes/origin/master
}

static bool
hash_netport6_data_list(struct sk_buff *skb,
			const struct hash_netport6_elem *data)
{
<<<<<<< HEAD
<<<<<<< HEAD
	NLA_PUT_IPADDR6(skb, IPSET_ATTR_IP, &data->ip);
	NLA_PUT_NET16(skb, IPSET_ATTR_PORT, data->port);
	NLA_PUT_U8(skb, IPSET_ATTR_CIDR, data->cidr);
	NLA_PUT_U8(skb, IPSET_ATTR_PROTO, data->proto);
=======
	u32 flags = data->nomatch ? IPSET_FLAG_NOMATCH : 0;

	NLA_PUT_IPADDR6(skb, IPSET_ATTR_IP, &data->ip);
	NLA_PUT_NET16(skb, IPSET_ATTR_PORT, data->port);
	NLA_PUT_U8(skb, IPSET_ATTR_CIDR, data->cidr + 1);
	NLA_PUT_U8(skb, IPSET_ATTR_PROTO, data->proto);
	if (flags)
		NLA_PUT_NET32(skb, IPSET_ATTR_CADT_FLAGS, htonl(flags));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u32 flags = data->nomatch ? IPSET_FLAG_NOMATCH : 0;

	if (nla_put_ipaddr6(skb, IPSET_ATTR_IP, &data->ip.in6) ||
	    nla_put_net16(skb, IPSET_ATTR_PORT, data->port) ||
	    nla_put_u8(skb, IPSET_ATTR_CIDR, data->cidr + 1) ||
	    nla_put_u8(skb, IPSET_ATTR_PROTO, data->proto) ||
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
hash_netport6_data_tlist(struct sk_buff *skb,
			 const struct hash_netport6_elem *data)
{
	const struct hash_netport6_telem *e =
		(const struct hash_netport6_telem *)data;
<<<<<<< HEAD

	NLA_PUT_IPADDR6(skb, IPSET_ATTR_IP, &e->ip);
	NLA_PUT_NET16(skb, IPSET_ATTR_PORT, data->port);
	NLA_PUT_U8(skb, IPSET_ATTR_CIDR, data->cidr);
	NLA_PUT_U8(skb, IPSET_ATTR_PROTO, data->proto);
	NLA_PUT_NET32(skb, IPSET_ATTR_TIMEOUT,
		      htonl(ip_set_timeout_get(e->timeout)));
=======
	u32 flags = data->nomatch ? IPSET_FLAG_NOMATCH : 0;

	NLA_PUT_IPADDR6(skb, IPSET_ATTR_IP, &e->ip);
	NLA_PUT_NET16(skb, IPSET_ATTR_PORT, data->port);
	NLA_PUT_U8(skb, IPSET_ATTR_CIDR, data->cidr + 1);
	NLA_PUT_U8(skb, IPSET_ATTR_PROTO, data->proto);
	NLA_PUT_NET32(skb, IPSET_ATTR_TIMEOUT,
		      htonl(ip_set_timeout_get(e->timeout)));
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
hash_netport6_kadt(struct ip_set *set, const struct sk_buff *skb,
		   enum ipset_adt adt, u8 pf, u8 dim, u8 flags)
=======
static inline void
hash_netport6_data_next(struct ip_set_hash *h,
			const struct hash_netport6_elem *d)
{
	h->next.port = ntohs(d->port);
}
=======
static inline void
hash_netport6_data_next(struct hash_netport4_elem *next,
			const struct hash_netport6_elem *d)
{
	next->port = d->port;
}

#undef MTYPE
#undef PF
#undef HOST_MASK

#define MTYPE		hash_netport6
#define PF		6
#define HOST_MASK	128
#define IP_SET_EMIT_CREATE
#include "ip_set_hash_gen.h"
>>>>>>> refs/remotes/origin/master

static int
hash_netport6_kadt(struct ip_set *set, const struct sk_buff *skb,
		   const struct xt_action_param *par,
<<<<<<< HEAD
		   enum ipset_adt adt, const struct ip_set_adt_opt *opt)
>>>>>>> refs/remotes/origin/cm-10.0
{
	const struct ip_set_hash *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_netport6_elem data = {
<<<<<<< HEAD
		.cidr = h->nets[0].cidr ? h->nets[0].cidr : HOST_MASK
	};

	if (data.cidr == 0)
		return -EINVAL;
	if (adt == IPSET_TEST)
		data.cidr = HOST_MASK;

	if (!ip_set_get_ip6_port(skb, flags & IPSET_DIM_TWO_SRC,
				 &data.port, &data.proto))
		return -EINVAL;

	ip6addrptr(skb, flags & IPSET_DIM_ONE_SRC, &data.ip.in6);
	ip6_netmask(&data.ip, data.cidr);

	return adtfn(set, &data, h->timeout);
=======
		.cidr = h->nets[0].cidr ? h->nets[0].cidr - 1 : HOST_MASK - 1,
	};

	if (adt == IPSET_TEST)
		data.cidr = HOST_MASK - 1;

	if (!ip_set_get_ip6_port(skb, opt->flags & IPSET_DIM_TWO_SRC,
				 &data.port, &data.proto))
		return -EINVAL;

	ip6addrptr(skb, opt->flags & IPSET_DIM_ONE_SRC, &data.ip.in6);
	ip6_netmask(&data.ip, data.cidr + 1);

	return adtfn(set, &data, opt_timeout(opt, h), opt->cmdflags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		   enum ipset_adt adt, struct ip_set_adt_opt *opt)
{
	const struct hash_netport *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_netport6_elem e = {
		.cidr = IP_SET_INIT_CIDR(h->nets[0].cidr[0], HOST_MASK) - 1,
	};
	struct ip_set_ext ext = IP_SET_INIT_KEXT(skb, opt, set);

	if (adt == IPSET_TEST)
		e.cidr = HOST_MASK - 1;

	if (!ip_set_get_ip6_port(skb, opt->flags & IPSET_DIM_TWO_SRC,
				 &e.port, &e.proto))
		return -EINVAL;

	ip6addrptr(skb, opt->flags & IPSET_DIM_ONE_SRC, &e.ip.in6);
	ip6_netmask(&e.ip, e.cidr + 1);

	return adtfn(set, &e, &ext, &opt->ext, opt->cmdflags);
>>>>>>> refs/remotes/origin/master
}

static int
hash_netport6_uadt(struct ip_set *set, struct nlattr *tb[],
<<<<<<< HEAD
<<<<<<< HEAD
		   enum ipset_adt adt, u32 *lineno, u32 flags)
{
	const struct ip_set_hash *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_netport6_elem data = { .cidr = HOST_MASK };
	u32 port, port_to;
	u32 timeout = h->timeout;
	bool with_ports = false;
=======
		   enum ipset_adt adt, u32 *lineno, u32 flags, bool retried)
{
	const struct ip_set_hash *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_netport6_elem data = { .cidr = HOST_MASK  - 1 };
	u32 port, port_to;
	u32 timeout = h->timeout;
	bool with_ports = false;
	u8 cidr;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		   enum ipset_adt adt, u32 *lineno, u32 flags, bool retried)
{
	const struct hash_netport *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_netport6_elem e = { .cidr = HOST_MASK  - 1 };
	struct ip_set_ext ext = IP_SET_INIT_UEXT(set);
	u32 port, port_to;
	bool with_ports = false;
	u8 cidr;
>>>>>>> refs/remotes/origin/master
	int ret;

	if (unlikely(!tb[IPSET_ATTR_IP] ||
		     !ip_set_attr_netorder(tb, IPSET_ATTR_PORT) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_PORT_TO) ||
<<<<<<< HEAD
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
	if (ret)
		return ret;

<<<<<<< HEAD
	if (tb[IPSET_ATTR_CIDR])
		data.cidr = nla_get_u8(tb[IPSET_ATTR_CIDR]);
	if (!data.cidr)
		return -IPSET_ERR_INVALID_CIDR;
	ip6_netmask(&data.ip, data.cidr);
=======
=======
	ret = ip_set_get_ipaddr6(tb[IPSET_ATTR_IP], &e.ip) ||
	      ip_set_get_extensions(set, tb, &ext);
	if (ret)
		return ret;

>>>>>>> refs/remotes/origin/master
	if (tb[IPSET_ATTR_CIDR]) {
		cidr = nla_get_u8(tb[IPSET_ATTR_CIDR]);
		if (!cidr || cidr > HOST_MASK)
			return -IPSET_ERR_INVALID_CIDR;
<<<<<<< HEAD
		data.cidr = cidr - 1;
	}
	ip6_netmask(&data.ip, data.cidr + 1);
>>>>>>> refs/remotes/origin/cm-10.0

	if (tb[IPSET_ATTR_PORT])
		data.port = nla_get_be16(tb[IPSET_ATTR_PORT]);
=======
		e.cidr = cidr - 1;
	}
	ip6_netmask(&e.ip, e.cidr + 1);

	if (tb[IPSET_ATTR_PORT])
		e.port = nla_get_be16(tb[IPSET_ATTR_PORT]);
>>>>>>> refs/remotes/origin/master
	else
		return -IPSET_ERR_PROTOCOL;

	if (tb[IPSET_ATTR_PROTO]) {
<<<<<<< HEAD
		data.proto = nla_get_u8(tb[IPSET_ATTR_PROTO]);
		with_ports = ip_set_proto_with_ports(data.proto);

		if (data.proto == 0)
=======
		e.proto = nla_get_u8(tb[IPSET_ATTR_PROTO]);
		with_ports = ip_set_proto_with_ports(e.proto);

		if (e.proto == 0)
>>>>>>> refs/remotes/origin/master
			return -IPSET_ERR_INVALID_PROTO;
	} else
		return -IPSET_ERR_MISSING_PROTO;

<<<<<<< HEAD
	if (!(with_ports || data.proto == IPPROTO_ICMPV6))
		data.port = 0;

	if (tb[IPSET_ATTR_TIMEOUT]) {
		if (!with_timeout(h->timeout))
			return -IPSET_ERR_TIMEOUT;
		timeout = ip_set_timeout_uget(tb[IPSET_ATTR_TIMEOUT]);
	}

<<<<<<< HEAD
	if (adt == IPSET_TEST || !with_ports || !tb[IPSET_ATTR_PORT_TO]) {
		ret = adtfn(set, &data, timeout);
=======
	if (tb[IPSET_ATTR_CADT_FLAGS] && adt == IPSET_ADD) {
		u32 cadt_flags = ip_set_get_h32(tb[IPSET_ATTR_CADT_FLAGS]);
		if (cadt_flags & IPSET_FLAG_NOMATCH)
			flags |= (cadt_flags << 16);
	}

	if (adt == IPSET_TEST || !with_ports || !tb[IPSET_ATTR_PORT_TO]) {
		ret = adtfn(set, &data, timeout, flags);
>>>>>>> refs/remotes/origin/cm-10.0
		return ip_set_eexist(ret, flags) ? 0 : ret;
	}

	port = ntohs(data.port);
=======
	if (!(with_ports || e.proto == IPPROTO_ICMPV6))
		e.port = 0;

	if (tb[IPSET_ATTR_CADT_FLAGS]) {
		u32 cadt_flags = ip_set_get_h32(tb[IPSET_ATTR_CADT_FLAGS]);
		if (cadt_flags & IPSET_FLAG_NOMATCH)
			flags |= (IPSET_FLAG_NOMATCH << 16);
	}

	if (adt == IPSET_TEST || !with_ports || !tb[IPSET_ATTR_PORT_TO]) {
		ret = adtfn(set, &e, &ext, &ext, flags);
		return ip_set_enomatch(ret, flags, adt, set) ? -ret :
		       ip_set_eexist(ret, flags) ? 0 : ret;
	}

	port = ntohs(e.port);
>>>>>>> refs/remotes/origin/master
	port_to = ip_set_get_h16(tb[IPSET_ATTR_PORT_TO]);
	if (port > port_to)
		swap(port, port_to);

<<<<<<< HEAD
<<<<<<< HEAD
	for (; port <= port_to; port++) {
		data.port = htons(port);
		ret = adtfn(set, &data, timeout);
=======
	if (retried)
		port = h->next.port;
	for (; port <= port_to; port++) {
		data.port = htons(port);
		ret = adtfn(set, &data, timeout, flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (retried)
		port = ntohs(h->next.port);
	for (; port <= port_to; port++) {
		e.port = htons(port);
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
/* Create hash:ip type of sets */

static int
hash_netport_create(struct ip_set *set, struct nlattr *tb[], u32 flags)
{
	struct ip_set_hash *h;
	u32 hashsize = IPSET_DEFAULT_HASHSIZE, maxelem = IPSET_DEFAULT_MAXELEM;
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
			? &hash_netport4_tvariant : &hash_netport6_tvariant;

		if (set->family == AF_INET)
=======
		set->variant = set->family == NFPROTO_IPV4
			? &hash_netport4_tvariant : &hash_netport6_tvariant;

		if (set->family == NFPROTO_IPV4)
>>>>>>> refs/remotes/origin/cm-10.0
			hash_netport4_gc_init(set);
		else
			hash_netport6_gc_init(set);
	} else {
<<<<<<< HEAD
		set->variant = set->family == AF_INET
=======
		set->variant = set->family == NFPROTO_IPV4
>>>>>>> refs/remotes/origin/cm-10.0
			? &hash_netport4_variant : &hash_netport6_variant;
	}

	pr_debug("create %s hashsize %u (%u) maxelem %u: %p(%p)\n",
		 set->name, jhash_size(h->table->htable_bits),
		 h->table->htable_bits, h->maxelem, set->data, h->table);

	return 0;
}

static struct ip_set_type hash_netport_type __read_mostly = {
	.name		= "hash:net,port",
	.protocol	= IPSET_PROTOCOL,
	.features	= IPSET_TYPE_IP | IPSET_TYPE_PORT,
	.dimension	= IPSET_DIM_TWO,
<<<<<<< HEAD
	.family		= AF_UNSPEC,
	.revision	= 1,
=======
	.family		= NFPROTO_UNSPEC,
	.revision_min	= 0,
	/*		  1	   SCTP and UDPLITE support added */
	/*		  2,	   Range as input support for IPv4 added */
	.revision_max	= 3,	/* nomatch flag support added */
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct ip_set_type hash_netport_type __read_mostly = {
	.name		= "hash:net,port",
	.protocol	= IPSET_PROTOCOL,
	.features	= IPSET_TYPE_IP | IPSET_TYPE_PORT | IPSET_TYPE_NOMATCH,
	.dimension	= IPSET_DIM_TWO,
	.family		= NFPROTO_UNSPEC,
	.revision_min	= IPSET_TYPE_REV_MIN,
	.revision_max	= IPSET_TYPE_REV_MAX,
>>>>>>> refs/remotes/origin/master
	.create		= hash_netport_create,
	.create_policy	= {
		[IPSET_ATTR_HASHSIZE]	= { .type = NLA_U32 },
		[IPSET_ATTR_MAXELEM]	= { .type = NLA_U32 },
		[IPSET_ATTR_PROBES]	= { .type = NLA_U8 },
		[IPSET_ATTR_RESIZE]	= { .type = NLA_U8  },
		[IPSET_ATTR_PROTO]	= { .type = NLA_U8 },
		[IPSET_ATTR_TIMEOUT]	= { .type = NLA_U32 },
<<<<<<< HEAD
	},
	.adt_policy	= {
		[IPSET_ATTR_IP]		= { .type = NLA_NESTED },
<<<<<<< HEAD
=======
		[IPSET_ATTR_IP_TO]	= { .type = NLA_NESTED },
>>>>>>> refs/remotes/origin/cm-10.0
=======
		[IPSET_ATTR_CADT_FLAGS]	= { .type = NLA_U32 },
	},
	.adt_policy	= {
		[IPSET_ATTR_IP]		= { .type = NLA_NESTED },
		[IPSET_ATTR_IP_TO]	= { .type = NLA_NESTED },
>>>>>>> refs/remotes/origin/master
		[IPSET_ATTR_PORT]	= { .type = NLA_U16 },
		[IPSET_ATTR_PORT_TO]	= { .type = NLA_U16 },
		[IPSET_ATTR_PROTO]	= { .type = NLA_U8 },
		[IPSET_ATTR_CIDR]	= { .type = NLA_U8 },
		[IPSET_ATTR_TIMEOUT]	= { .type = NLA_U32 },
		[IPSET_ATTR_LINENO]	= { .type = NLA_U32 },
<<<<<<< HEAD
<<<<<<< HEAD
=======
		[IPSET_ATTR_CADT_FLAGS]	= { .type = NLA_U32 },
>>>>>>> refs/remotes/origin/cm-10.0
=======
		[IPSET_ATTR_CADT_FLAGS]	= { .type = NLA_U32 },
		[IPSET_ATTR_BYTES]	= { .type = NLA_U64 },
		[IPSET_ATTR_PACKETS]	= { .type = NLA_U64 },
		[IPSET_ATTR_COMMENT]	= { .type = NLA_NUL_STRING },
>>>>>>> refs/remotes/origin/master
	},
	.me		= THIS_MODULE,
};

static int __init
hash_netport_init(void)
{
	return ip_set_type_register(&hash_netport_type);
}

static void __exit
hash_netport_fini(void)
{
	ip_set_type_unregister(&hash_netport_type);
}

module_init(hash_netport_init);
module_exit(hash_netport_fini);
