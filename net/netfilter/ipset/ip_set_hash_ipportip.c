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

/* Kernel module implementing an IP set type: the hash:ip,port,ip type */

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
#include <linux/netfilter/ipset/ip_set_getport.h>
#include <linux/netfilter/ipset/ip_set_hash.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>");
MODULE_DESCRIPTION("hash:ip,port,ip type of IP sets");
MODULE_ALIAS("ip_set_hash:ip,port,ip");

/* Type specific function prefix */
#define TYPE		hash_ipportip

static bool
hash_ipportip_same_set(const struct ip_set *a, const struct ip_set *b);

#define hash_ipportip4_same_set	hash_ipportip_same_set
#define hash_ipportip6_same_set	hash_ipportip_same_set

/* The type variant functions: IPv4 */

/* Member elements without timeout */
=======
#include <linux/netfilter/ipset/ip_set_getport.h>
#include <linux/netfilter/ipset/ip_set_hash.h>

#define IPSET_TYPE_REV_MIN	0
/*				1    SCTP and UDPLITE support added */
/*				2    Counters support added */
#define IPSET_TYPE_REV_MAX	3 /* Comments support added */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>");
IP_SET_MODULE_DESC("hash:ip,port,ip", IPSET_TYPE_REV_MIN, IPSET_TYPE_REV_MAX);
MODULE_ALIAS("ip_set_hash:ip,port,ip");

/* Type specific function prefix */
#define HTYPE		hash_ipportip

/* IPv4 variant */

/* Member elements  */
>>>>>>> refs/remotes/origin/master
struct hash_ipportip4_elem {
	__be32 ip;
	__be32 ip2;
	__be16 port;
	u8 proto;
	u8 padding;
};

<<<<<<< HEAD
/* Member elements with timeout support */
struct hash_ipportip4_telem {
	__be32 ip;
	__be32 ip2;
	__be16 port;
	u8 proto;
	u8 padding;
	unsigned long timeout;
};

static inline bool
hash_ipportip4_data_equal(const struct hash_ipportip4_elem *ip1,
<<<<<<< HEAD
			  const struct hash_ipportip4_elem *ip2)
=======
			  const struct hash_ipportip4_elem *ip2,
			  u32 *multi)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline bool
hash_ipportip4_data_equal(const struct hash_ipportip4_elem *ip1,
			  const struct hash_ipportip4_elem *ip2,
			  u32 *multi)
>>>>>>> refs/remotes/origin/master
{
	return ip1->ip == ip2->ip &&
	       ip1->ip2 == ip2->ip2 &&
	       ip1->port == ip2->port &&
	       ip1->proto == ip2->proto;
}

<<<<<<< HEAD
static inline bool
hash_ipportip4_data_isnull(const struct hash_ipportip4_elem *elem)
{
	return elem->proto == 0;
}

static inline void
hash_ipportip4_data_copy(struct hash_ipportip4_elem *dst,
			 const struct hash_ipportip4_elem *src)
{
	memcpy(dst, src, sizeof(*dst));
}

static inline void
hash_ipportip4_data_zero_out(struct hash_ipportip4_elem *elem)
{
	elem->proto = 0;
}

=======
>>>>>>> refs/remotes/origin/master
static bool
hash_ipportip4_data_list(struct sk_buff *skb,
		       const struct hash_ipportip4_elem *data)
{
<<<<<<< HEAD
	NLA_PUT_IPADDR4(skb, IPSET_ATTR_IP, data->ip);
	NLA_PUT_IPADDR4(skb, IPSET_ATTR_IP2, data->ip2);
	NLA_PUT_NET16(skb, IPSET_ATTR_PORT, data->port);
	NLA_PUT_U8(skb, IPSET_ATTR_PROTO, data->proto);
=======
	if (nla_put_ipaddr4(skb, IPSET_ATTR_IP, data->ip) ||
	    nla_put_ipaddr4(skb, IPSET_ATTR_IP2, data->ip2) ||
	    nla_put_net16(skb, IPSET_ATTR_PORT, data->port) ||
	    nla_put_u8(skb, IPSET_ATTR_PROTO, data->proto))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master
	return 0;

nla_put_failure:
	return 1;
}

<<<<<<< HEAD
static bool
hash_ipportip4_data_tlist(struct sk_buff *skb,
			const struct hash_ipportip4_elem *data)
{
	const struct hash_ipportip4_telem *tdata =
		(const struct hash_ipportip4_telem *)data;

	NLA_PUT_IPADDR4(skb, IPSET_ATTR_IP, tdata->ip);
	NLA_PUT_IPADDR4(skb, IPSET_ATTR_IP2, tdata->ip2);
	NLA_PUT_NET16(skb, IPSET_ATTR_PORT, tdata->port);
	NLA_PUT_U8(skb, IPSET_ATTR_PROTO, data->proto);
	NLA_PUT_NET32(skb, IPSET_ATTR_TIMEOUT,
		      htonl(ip_set_timeout_get(tdata->timeout)));

	return 0;

nla_put_failure:
	return 1;
}

#define PF		4
#define HOST_MASK	32
#include <linux/netfilter/ipset/ip_set_ahash.h>

<<<<<<< HEAD
static int
hash_ipportip4_kadt(struct ip_set *set, const struct sk_buff *skb,
		    enum ipset_adt adt, u8 pf, u8 dim, u8 flags)
=======
static inline void
hash_ipportip4_data_next(struct ip_set_hash *h,
			 const struct hash_ipportip4_elem *d)
{
	h->next.ip = ntohl(d->ip);
	h->next.port = ntohs(d->port);
}
=======
static inline void
hash_ipportip4_data_next(struct hash_ipportip4_elem *next,
			 const struct hash_ipportip4_elem *d)
{
	next->ip = d->ip;
	next->port = d->port;
}

/* Common functions */
#define MTYPE		hash_ipportip4
#define PF		4
#define HOST_MASK	32
#include "ip_set_hash_gen.h"
>>>>>>> refs/remotes/origin/master

static int
hash_ipportip4_kadt(struct ip_set *set, const struct sk_buff *skb,
		    const struct xt_action_param *par,
<<<<<<< HEAD
		    enum ipset_adt adt, const struct ip_set_adt_opt *opt)
>>>>>>> refs/remotes/origin/cm-10.0
{
	const struct ip_set_hash *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_ipportip4_elem data = { };

<<<<<<< HEAD
	if (!ip_set_get_ip4_port(skb, flags & IPSET_DIM_TWO_SRC,
				 &data.port, &data.proto))
		return -EINVAL;

	ip4addrptr(skb, flags & IPSET_DIM_ONE_SRC, &data.ip);
	ip4addrptr(skb, flags & IPSET_DIM_THREE_SRC, &data.ip2);

	return adtfn(set, &data, h->timeout);
=======
	if (!ip_set_get_ip4_port(skb, opt->flags & IPSET_DIM_TWO_SRC,
				 &data.port, &data.proto))
		return -EINVAL;

	ip4addrptr(skb, opt->flags & IPSET_DIM_ONE_SRC, &data.ip);
	ip4addrptr(skb, opt->flags & IPSET_DIM_THREE_SRC, &data.ip2);

	return adtfn(set, &data, opt_timeout(opt, h), opt->cmdflags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		    enum ipset_adt adt, struct ip_set_adt_opt *opt)
{
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_ipportip4_elem e = { };
	struct ip_set_ext ext = IP_SET_INIT_KEXT(skb, opt, set);

	if (!ip_set_get_ip4_port(skb, opt->flags & IPSET_DIM_TWO_SRC,
				 &e.port, &e.proto))
		return -EINVAL;

	ip4addrptr(skb, opt->flags & IPSET_DIM_ONE_SRC, &e.ip);
	ip4addrptr(skb, opt->flags & IPSET_DIM_THREE_SRC, &e.ip2);
	return adtfn(set, &e, &ext, &opt->ext, opt->cmdflags);
>>>>>>> refs/remotes/origin/master
}

static int
hash_ipportip4_uadt(struct ip_set *set, struct nlattr *tb[],
<<<<<<< HEAD
<<<<<<< HEAD
		    enum ipset_adt adt, u32 *lineno, u32 flags)
=======
		    enum ipset_adt adt, u32 *lineno, u32 flags, bool retried)
>>>>>>> refs/remotes/origin/cm-10.0
{
	const struct ip_set_hash *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_ipportip4_elem data = { };
<<<<<<< HEAD
	u32 ip, ip_to, p, port, port_to;
=======
	u32 ip, ip_to = 0, p = 0, port, port_to;
>>>>>>> refs/remotes/origin/cm-10.0
	u32 timeout = h->timeout;
=======
		    enum ipset_adt adt, u32 *lineno, u32 flags, bool retried)
{
	const struct hash_ipportip *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_ipportip4_elem e = { };
	struct ip_set_ext ext = IP_SET_INIT_UEXT(set);
	u32 ip, ip_to = 0, p = 0, port, port_to;
>>>>>>> refs/remotes/origin/master
	bool with_ports = false;
	int ret;

	if (unlikely(!tb[IPSET_ATTR_IP] || !tb[IPSET_ATTR_IP2] ||
		     !ip_set_attr_netorder(tb, IPSET_ATTR_PORT) ||
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

<<<<<<< HEAD
	ret = ip_set_get_ipaddr4(tb[IPSET_ATTR_IP], &data.ip);
	if (ret)
		return ret;

	ret = ip_set_get_ipaddr4(tb[IPSET_ATTR_IP2], &data.ip2);
=======
	ret = ip_set_get_ipaddr4(tb[IPSET_ATTR_IP], &e.ip) ||
	      ip_set_get_extensions(set, tb, &ext);
	if (ret)
		return ret;

	ret = ip_set_get_ipaddr4(tb[IPSET_ATTR_IP2], &e.ip2);
>>>>>>> refs/remotes/origin/master
	if (ret)
		return ret;

	if (tb[IPSET_ATTR_PORT])
<<<<<<< HEAD
		data.port = nla_get_be16(tb[IPSET_ATTR_PORT]);
=======
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
=======
	if (!(with_ports || e.proto == IPPROTO_ICMP))
		e.port = 0;
>>>>>>> refs/remotes/origin/master

	if (adt == IPSET_TEST ||
	    !(tb[IPSET_ATTR_IP_TO] || tb[IPSET_ATTR_CIDR] ||
	      tb[IPSET_ATTR_PORT_TO])) {
<<<<<<< HEAD
<<<<<<< HEAD
		ret = adtfn(set, &data, timeout);
=======
		ret = adtfn(set, &data, timeout, flags);
>>>>>>> refs/remotes/origin/cm-10.0
		return ip_set_eexist(ret, flags) ? 0 : ret;
	}

	ip = ntohl(data.ip);
=======
		ret = adtfn(set, &e, &ext, &ext, flags);
		return ip_set_eexist(ret, flags) ? 0 : ret;
	}

	ip_to = ip = ntohl(e.ip);
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

	port_to = port = ntohs(data.port);
=======
		if (!cidr || cidr > 32)
			return -IPSET_ERR_INVALID_CIDR;
		ip_set_mask_from_to(ip, ip_to, cidr);
	}

	port_to = port = ntohs(e.port);
>>>>>>> refs/remotes/origin/master
	if (with_ports && tb[IPSET_ATTR_PORT_TO]) {
		port_to = ip_set_get_h16(tb[IPSET_ATTR_PORT_TO]);
		if (port > port_to)
			swap(port, port_to);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	for (; !before(ip_to, ip); ip++)
		for (p = port; p <= port_to; p++) {
			data.ip = htonl(ip);
			data.port = htons(p);
			ret = adtfn(set, &data, timeout);
=======
	if (retried)
		ip = h->next.ip;
	for (; !before(ip_to, ip); ip++) {
		p = retried && ip == h->next.ip ? h->next.port : port;
		for (; p <= port_to; p++) {
			data.ip = htonl(ip);
			data.port = htons(p);
			ret = adtfn(set, &data, timeout, flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (retried)
		ip = ntohl(h->next.ip);
	for (; !before(ip_to, ip); ip++) {
		p = retried && ip == ntohl(h->next.ip) ? ntohs(h->next.port)
						       : port;
		for (; p <= port_to; p++) {
			e.ip = htonl(ip);
			e.port = htons(p);
			ret = adtfn(set, &e, &ext, &ext, flags);
>>>>>>> refs/remotes/origin/master

			if (ret && !ip_set_eexist(ret, flags))
				return ret;
			else
				ret = 0;
		}
<<<<<<< HEAD
<<<<<<< HEAD
=======
	}
>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
}

static bool
hash_ipportip_same_set(const struct ip_set *a, const struct ip_set *b)
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
	return ret;
}

/* IPv6 variant */
>>>>>>> refs/remotes/origin/master

struct hash_ipportip6_elem {
	union nf_inet_addr ip;
	union nf_inet_addr ip2;
	__be16 port;
	u8 proto;
	u8 padding;
};

<<<<<<< HEAD
struct hash_ipportip6_telem {
	union nf_inet_addr ip;
	union nf_inet_addr ip2;
	__be16 port;
	u8 proto;
	u8 padding;
	unsigned long timeout;
};

static inline bool
hash_ipportip6_data_equal(const struct hash_ipportip6_elem *ip1,
<<<<<<< HEAD
			  const struct hash_ipportip6_elem *ip2)
=======
			  const struct hash_ipportip6_elem *ip2,
			  u32 *multi)
>>>>>>> refs/remotes/origin/cm-10.0
{
	return ipv6_addr_cmp(&ip1->ip.in6, &ip2->ip.in6) == 0 &&
	       ipv6_addr_cmp(&ip1->ip2.in6, &ip2->ip2.in6) == 0 &&
=======
/* Common functions */

static inline bool
hash_ipportip6_data_equal(const struct hash_ipportip6_elem *ip1,
			  const struct hash_ipportip6_elem *ip2,
			  u32 *multi)
{
	return ipv6_addr_equal(&ip1->ip.in6, &ip2->ip.in6) &&
	       ipv6_addr_equal(&ip1->ip2.in6, &ip2->ip2.in6) &&
>>>>>>> refs/remotes/origin/master
	       ip1->port == ip2->port &&
	       ip1->proto == ip2->proto;
}

<<<<<<< HEAD
static inline bool
hash_ipportip6_data_isnull(const struct hash_ipportip6_elem *elem)
{
	return elem->proto == 0;
}

static inline void
hash_ipportip6_data_copy(struct hash_ipportip6_elem *dst,
			 const struct hash_ipportip6_elem *src)
{
	memcpy(dst, src, sizeof(*dst));
}

static inline void
hash_ipportip6_data_zero_out(struct hash_ipportip6_elem *elem)
{
	elem->proto = 0;
}

=======
>>>>>>> refs/remotes/origin/master
static bool
hash_ipportip6_data_list(struct sk_buff *skb,
			 const struct hash_ipportip6_elem *data)
{
<<<<<<< HEAD
	NLA_PUT_IPADDR6(skb, IPSET_ATTR_IP, &data->ip);
	NLA_PUT_IPADDR6(skb, IPSET_ATTR_IP2, &data->ip2);
	NLA_PUT_NET16(skb, IPSET_ATTR_PORT, data->port);
	NLA_PUT_U8(skb, IPSET_ATTR_PROTO, data->proto);
=======
	if (nla_put_ipaddr6(skb, IPSET_ATTR_IP, &data->ip.in6) ||
	    nla_put_ipaddr6(skb, IPSET_ATTR_IP2, &data->ip2.in6) ||
	    nla_put_net16(skb, IPSET_ATTR_PORT, data->port) ||
	    nla_put_u8(skb, IPSET_ATTR_PROTO, data->proto))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master
	return 0;

nla_put_failure:
	return 1;
}

<<<<<<< HEAD
static bool
hash_ipportip6_data_tlist(struct sk_buff *skb,
			  const struct hash_ipportip6_elem *data)
{
	const struct hash_ipportip6_telem *e =
		(const struct hash_ipportip6_telem *)data;

	NLA_PUT_IPADDR6(skb, IPSET_ATTR_IP, &e->ip);
	NLA_PUT_IPADDR6(skb, IPSET_ATTR_IP2, &data->ip2);
	NLA_PUT_NET16(skb, IPSET_ATTR_PORT, data->port);
	NLA_PUT_U8(skb, IPSET_ATTR_PROTO, data->proto);
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
hash_ipportip6_kadt(struct ip_set *set, const struct sk_buff *skb,
		    enum ipset_adt adt, u8 pf, u8 dim, u8 flags)
=======
static inline void
hash_ipportip6_data_next(struct ip_set_hash *h,
			 const struct hash_ipportip6_elem *d)
{
	h->next.port = ntohs(d->port);
}
=======
static inline void
hash_ipportip6_data_next(struct hash_ipportip4_elem *next,
			 const struct hash_ipportip6_elem *d)
{
	next->port = d->port;
}

#undef MTYPE
#undef PF
#undef HOST_MASK

#define MTYPE		hash_ipportip6
#define PF		6
#define HOST_MASK	128
#define IP_SET_EMIT_CREATE
#include "ip_set_hash_gen.h"
>>>>>>> refs/remotes/origin/master

static int
hash_ipportip6_kadt(struct ip_set *set, const struct sk_buff *skb,
		    const struct xt_action_param *par,
<<<<<<< HEAD
		    enum ipset_adt adt, const struct ip_set_adt_opt *opt)
>>>>>>> refs/remotes/origin/cm-10.0
{
	const struct ip_set_hash *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_ipportip6_elem data = { };

<<<<<<< HEAD
	if (!ip_set_get_ip6_port(skb, flags & IPSET_DIM_TWO_SRC,
				 &data.port, &data.proto))
		return -EINVAL;

	ip6addrptr(skb, flags & IPSET_DIM_ONE_SRC, &data.ip.in6);
	ip6addrptr(skb, flags & IPSET_DIM_THREE_SRC, &data.ip2.in6);

	return adtfn(set, &data, h->timeout);
=======
	if (!ip_set_get_ip6_port(skb, opt->flags & IPSET_DIM_TWO_SRC,
				 &data.port, &data.proto))
		return -EINVAL;

	ip6addrptr(skb, opt->flags & IPSET_DIM_ONE_SRC, &data.ip.in6);
	ip6addrptr(skb, opt->flags & IPSET_DIM_THREE_SRC, &data.ip2.in6);

	return adtfn(set, &data, opt_timeout(opt, h), opt->cmdflags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		    enum ipset_adt adt, struct ip_set_adt_opt *opt)
{
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_ipportip6_elem e = { };
	struct ip_set_ext ext = IP_SET_INIT_KEXT(skb, opt, set);

	if (!ip_set_get_ip6_port(skb, opt->flags & IPSET_DIM_TWO_SRC,
				 &e.port, &e.proto))
		return -EINVAL;

	ip6addrptr(skb, opt->flags & IPSET_DIM_ONE_SRC, &e.ip.in6);
	ip6addrptr(skb, opt->flags & IPSET_DIM_THREE_SRC, &e.ip2.in6);
	return adtfn(set, &e, &ext, &opt->ext, opt->cmdflags);
>>>>>>> refs/remotes/origin/master
}

static int
hash_ipportip6_uadt(struct ip_set *set, struct nlattr *tb[],
<<<<<<< HEAD
<<<<<<< HEAD
		    enum ipset_adt adt, u32 *lineno, u32 flags)
=======
		    enum ipset_adt adt, u32 *lineno, u32 flags, bool retried)
>>>>>>> refs/remotes/origin/cm-10.0
{
	const struct ip_set_hash *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_ipportip6_elem data = { };
	u32 port, port_to;
	u32 timeout = h->timeout;
=======
		    enum ipset_adt adt, u32 *lineno, u32 flags, bool retried)
{
	const struct hash_ipportip *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_ipportip6_elem e = { };
	struct ip_set_ext ext = IP_SET_INIT_UEXT(set);
	u32 port, port_to;
>>>>>>> refs/remotes/origin/master
	bool with_ports = false;
	int ret;

	if (unlikely(!tb[IPSET_ATTR_IP] || !tb[IPSET_ATTR_IP2] ||
		     !ip_set_attr_netorder(tb, IPSET_ATTR_PORT) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_PORT_TO) ||
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
	ret = ip_set_get_ipaddr6(tb[IPSET_ATTR_IP], &data.ip);
	if (ret)
		return ret;

	ret = ip_set_get_ipaddr6(tb[IPSET_ATTR_IP2], &data.ip2);
=======
	ret = ip_set_get_ipaddr6(tb[IPSET_ATTR_IP], &e.ip) ||
	      ip_set_get_extensions(set, tb, &ext);
	if (ret)
		return ret;

	ret = ip_set_get_ipaddr6(tb[IPSET_ATTR_IP2], &e.ip2);
>>>>>>> refs/remotes/origin/master
	if (ret)
		return ret;

	if (tb[IPSET_ATTR_PORT])
<<<<<<< HEAD
		data.port = nla_get_be16(tb[IPSET_ATTR_PORT]);
=======
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

	if (adt == IPSET_TEST || !with_ports || !tb[IPSET_ATTR_PORT_TO]) {
<<<<<<< HEAD
		ret = adtfn(set, &data, timeout);
=======
		ret = adtfn(set, &data, timeout, flags);
>>>>>>> refs/remotes/origin/cm-10.0
		return ip_set_eexist(ret, flags) ? 0 : ret;
	}

	port = ntohs(data.port);
=======
	if (!(with_ports || e.proto == IPPROTO_ICMPV6))
		e.port = 0;

	if (adt == IPSET_TEST || !with_ports || !tb[IPSET_ATTR_PORT_TO]) {
		ret = adtfn(set, &e, &ext, &ext, flags);
		return ip_set_eexist(ret, flags) ? 0 : ret;
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
hash_ipportip_create(struct ip_set *set, struct nlattr *tb[], u32 flags)
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

	h = kzalloc(sizeof(*h), GFP_KERNEL);
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
			? &hash_ipportip4_tvariant : &hash_ipportip6_tvariant;

		if (set->family == AF_INET)
=======
		set->variant = set->family == NFPROTO_IPV4
			? &hash_ipportip4_tvariant : &hash_ipportip6_tvariant;

		if (set->family == NFPROTO_IPV4)
>>>>>>> refs/remotes/origin/cm-10.0
			hash_ipportip4_gc_init(set);
		else
			hash_ipportip6_gc_init(set);
	} else {
<<<<<<< HEAD
		set->variant = set->family == AF_INET
=======
		set->variant = set->family == NFPROTO_IPV4
>>>>>>> refs/remotes/origin/cm-10.0
			? &hash_ipportip4_variant : &hash_ipportip6_variant;
	}

	pr_debug("create %s hashsize %u (%u) maxelem %u: %p(%p)\n",
		 set->name, jhash_size(h->table->htable_bits),
		 h->table->htable_bits, h->maxelem, set->data, h->table);

	return 0;
}

=======
>>>>>>> refs/remotes/origin/master
static struct ip_set_type hash_ipportip_type __read_mostly = {
	.name		= "hash:ip,port,ip",
	.protocol	= IPSET_PROTOCOL,
	.features	= IPSET_TYPE_IP | IPSET_TYPE_PORT | IPSET_TYPE_IP2,
	.dimension	= IPSET_DIM_THREE,
<<<<<<< HEAD
<<<<<<< HEAD
	.family		= AF_UNSPEC,
	.revision	= 1,
=======
	.family		= NFPROTO_UNSPEC,
	.revision_min	= 0,
	.revision_max	= 1,	/* SCTP and UDPLITE support added */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.family		= NFPROTO_UNSPEC,
	.revision_min	= IPSET_TYPE_REV_MIN,
	.revision_max	= IPSET_TYPE_REV_MAX,
>>>>>>> refs/remotes/origin/master
	.create		= hash_ipportip_create,
	.create_policy	= {
		[IPSET_ATTR_HASHSIZE]	= { .type = NLA_U32 },
		[IPSET_ATTR_MAXELEM]	= { .type = NLA_U32 },
		[IPSET_ATTR_PROBES]	= { .type = NLA_U8 },
		[IPSET_ATTR_RESIZE]	= { .type = NLA_U8  },
		[IPSET_ATTR_TIMEOUT]	= { .type = NLA_U32 },
<<<<<<< HEAD
=======
		[IPSET_ATTR_CADT_FLAGS]	= { .type = NLA_U32 },
>>>>>>> refs/remotes/origin/master
	},
	.adt_policy	= {
		[IPSET_ATTR_IP]		= { .type = NLA_NESTED },
		[IPSET_ATTR_IP_TO]	= { .type = NLA_NESTED },
		[IPSET_ATTR_IP2]	= { .type = NLA_NESTED },
		[IPSET_ATTR_PORT]	= { .type = NLA_U16 },
		[IPSET_ATTR_PORT_TO]	= { .type = NLA_U16 },
		[IPSET_ATTR_CIDR]	= { .type = NLA_U8 },
		[IPSET_ATTR_PROTO]	= { .type = NLA_U8 },
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
hash_ipportip_init(void)
{
	return ip_set_type_register(&hash_ipportip_type);
}

static void __exit
hash_ipportip_fini(void)
{
	ip_set_type_unregister(&hash_ipportip_type);
}

module_init(hash_ipportip_init);
module_exit(hash_ipportip_fini);
