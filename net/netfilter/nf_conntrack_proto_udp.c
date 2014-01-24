/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2004 Netfilter Core Team <coreteam@netfilter.org>
<<<<<<< HEAD
=======
 * (C) 2006-2012 Patrick McHardy <kaber@trash.net>
>>>>>>> refs/remotes/origin/master
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/types.h>
#include <linux/timer.h>
#include <linux/module.h>
#include <linux/udp.h>
#include <linux/seq_file.h>
#include <linux/skbuff.h>
#include <linux/ipv6.h>
#include <net/ip6_checksum.h>
#include <net/checksum.h>

#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv6.h>
#include <net/netfilter/nf_conntrack_l4proto.h>
#include <net/netfilter/nf_conntrack_ecache.h>
#include <net/netfilter/nf_log.h>
#include <net/netfilter/ipv4/nf_conntrack_ipv4.h>
#include <net/netfilter/ipv6/nf_conntrack_ipv6.h>

<<<<<<< HEAD
<<<<<<< HEAD
static unsigned int nf_ct_udp_timeout __read_mostly = 30*HZ;
static unsigned int nf_ct_udp_timeout_stream __read_mostly = 180*HZ;
=======
enum udp_conntrack {
	UDP_CT_UNREPLIED,
	UDP_CT_REPLIED,
	UDP_CT_MAX
};

=======
>>>>>>> refs/remotes/origin/master
static unsigned int udp_timeouts[UDP_CT_MAX] = {
	[UDP_CT_UNREPLIED]	= 30*HZ,
	[UDP_CT_REPLIED]	= 180*HZ,
};
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======

static inline struct nf_udp_net *udp_pernet(struct net *net)
{
	return &net->ct.nf_ct_proto.udp;
}
>>>>>>> refs/remotes/origin/master

static bool udp_pkt_to_tuple(const struct sk_buff *skb,
			     unsigned int dataoff,
			     struct nf_conntrack_tuple *tuple)
{
	const struct udphdr *hp;
	struct udphdr _hdr;

	/* Actually only need first 8 bytes. */
	hp = skb_header_pointer(skb, dataoff, sizeof(_hdr), &_hdr);
	if (hp == NULL)
		return false;

	tuple->src.u.udp.port = hp->source;
	tuple->dst.u.udp.port = hp->dest;

	return true;
}

static bool udp_invert_tuple(struct nf_conntrack_tuple *tuple,
			     const struct nf_conntrack_tuple *orig)
{
	tuple->src.u.udp.port = orig->dst.u.udp.port;
	tuple->dst.u.udp.port = orig->src.u.udp.port;
	return true;
}

/* Print out the per-protocol part of the tuple. */
static int udp_print_tuple(struct seq_file *s,
			   const struct nf_conntrack_tuple *tuple)
{
	return seq_printf(s, "sport=%hu dport=%hu ",
			  ntohs(tuple->src.u.udp.port),
			  ntohs(tuple->dst.u.udp.port));
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
static unsigned int *udp_get_timeouts(struct net *net)
{
	return udp_timeouts;
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
static unsigned int *udp_get_timeouts(struct net *net)
{
	return udp_pernet(net)->timeouts;
}

>>>>>>> refs/remotes/origin/master
/* Returns verdict for packet, and may modify conntracktype */
static int udp_packet(struct nf_conn *ct,
		      const struct sk_buff *skb,
		      unsigned int dataoff,
		      enum ip_conntrack_info ctinfo,
		      u_int8_t pf,
<<<<<<< HEAD
<<<<<<< HEAD
		      unsigned int hooknum)
=======
		      unsigned int hooknum,
		      unsigned int *timeouts)
>>>>>>> refs/remotes/origin/cm-10.0
=======
		      unsigned int hooknum,
		      unsigned int *timeouts)
>>>>>>> refs/remotes/origin/master
{
	/* If we've seen traffic both ways, this is some kind of UDP
	   stream.  Extend timeout. */
	if (test_bit(IPS_SEEN_REPLY_BIT, &ct->status)) {
<<<<<<< HEAD
<<<<<<< HEAD
		nf_ct_refresh_acct(ct, ctinfo, skb, nf_ct_udp_timeout_stream);
		/* Also, more likely to be important, and not a probe */
		if (!test_and_set_bit(IPS_ASSURED_BIT, &ct->status))
			nf_conntrack_event_cache(IPCT_ASSURED, ct);
	} else
		nf_ct_refresh_acct(ct, ctinfo, skb, nf_ct_udp_timeout);

=======
=======
>>>>>>> refs/remotes/origin/master
		nf_ct_refresh_acct(ct, ctinfo, skb,
				   timeouts[UDP_CT_REPLIED]);
		/* Also, more likely to be important, and not a probe */
		if (!test_and_set_bit(IPS_ASSURED_BIT, &ct->status))
			nf_conntrack_event_cache(IPCT_ASSURED, ct);
	} else {
		nf_ct_refresh_acct(ct, ctinfo, skb,
				   timeouts[UDP_CT_UNREPLIED]);
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return NF_ACCEPT;
}

/* Called when a new connection for this protocol found. */
static bool udp_new(struct nf_conn *ct, const struct sk_buff *skb,
<<<<<<< HEAD
<<<<<<< HEAD
		    unsigned int dataoff)
=======
		    unsigned int dataoff, unsigned int *timeouts)
>>>>>>> refs/remotes/origin/cm-10.0
=======
		    unsigned int dataoff, unsigned int *timeouts)
>>>>>>> refs/remotes/origin/master
{
	return true;
}

static int udp_error(struct net *net, struct nf_conn *tmpl, struct sk_buff *skb,
		     unsigned int dataoff, enum ip_conntrack_info *ctinfo,
		     u_int8_t pf,
		     unsigned int hooknum)
{
	unsigned int udplen = skb->len - dataoff;
	const struct udphdr *hdr;
	struct udphdr _hdr;

	/* Header is too small? */
	hdr = skb_header_pointer(skb, dataoff, sizeof(_hdr), &_hdr);
	if (hdr == NULL) {
		if (LOG_INVALID(net, IPPROTO_UDP))
<<<<<<< HEAD
			nf_log_packet(pf, 0, skb, NULL, NULL, NULL,
=======
			nf_log_packet(net, pf, 0, skb, NULL, NULL, NULL,
>>>>>>> refs/remotes/origin/master
				      "nf_ct_udp: short packet ");
		return -NF_ACCEPT;
	}

	/* Truncated/malformed packets */
	if (ntohs(hdr->len) > udplen || ntohs(hdr->len) < sizeof(*hdr)) {
		if (LOG_INVALID(net, IPPROTO_UDP))
<<<<<<< HEAD
			nf_log_packet(pf, 0, skb, NULL, NULL, NULL,
=======
			nf_log_packet(net, pf, 0, skb, NULL, NULL, NULL,
>>>>>>> refs/remotes/origin/master
				"nf_ct_udp: truncated/malformed packet ");
		return -NF_ACCEPT;
	}

	/* Packet with no checksum */
	if (!hdr->check)
		return NF_ACCEPT;

	/* Checksum invalid? Ignore.
	 * We skip checking packets on the outgoing path
	 * because the checksum is assumed to be correct.
	 * FIXME: Source route IP option packets --RR */
	if (net->ct.sysctl_checksum && hooknum == NF_INET_PRE_ROUTING &&
	    nf_checksum(skb, hooknum, dataoff, IPPROTO_UDP, pf)) {
		if (LOG_INVALID(net, IPPROTO_UDP))
<<<<<<< HEAD
			nf_log_packet(pf, 0, skb, NULL, NULL, NULL,
=======
			nf_log_packet(net, pf, 0, skb, NULL, NULL, NULL,
>>>>>>> refs/remotes/origin/master
				"nf_ct_udp: bad UDP checksum ");
		return -NF_ACCEPT;
	}

	return NF_ACCEPT;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#if IS_ENABLED(CONFIG_NF_CT_NETLINK_TIMEOUT)

#include <linux/netfilter/nfnetlink.h>
#include <linux/netfilter/nfnetlink_cttimeout.h>

<<<<<<< HEAD
static int udp_timeout_nlattr_to_obj(struct nlattr *tb[], void *data)
{
	unsigned int *timeouts = data;

	/* set default timeouts for UDP. */
	timeouts[UDP_CT_UNREPLIED] = udp_timeouts[UDP_CT_UNREPLIED];
	timeouts[UDP_CT_REPLIED] = udp_timeouts[UDP_CT_REPLIED];
=======
static int udp_timeout_nlattr_to_obj(struct nlattr *tb[],
				     struct net *net, void *data)
{
	unsigned int *timeouts = data;
	struct nf_udp_net *un = udp_pernet(net);

	/* set default timeouts for UDP. */
	timeouts[UDP_CT_UNREPLIED] = un->timeouts[UDP_CT_UNREPLIED];
	timeouts[UDP_CT_REPLIED] = un->timeouts[UDP_CT_REPLIED];
>>>>>>> refs/remotes/origin/master

	if (tb[CTA_TIMEOUT_UDP_UNREPLIED]) {
		timeouts[UDP_CT_UNREPLIED] =
			ntohl(nla_get_be32(tb[CTA_TIMEOUT_UDP_UNREPLIED])) * HZ;
	}
	if (tb[CTA_TIMEOUT_UDP_REPLIED]) {
		timeouts[UDP_CT_REPLIED] =
			ntohl(nla_get_be32(tb[CTA_TIMEOUT_UDP_REPLIED])) * HZ;
	}
	return 0;
}

static int
udp_timeout_obj_to_nlattr(struct sk_buff *skb, const void *data)
{
	const unsigned int *timeouts = data;

<<<<<<< HEAD
	NLA_PUT_BE32(skb, CTA_TIMEOUT_UDP_UNREPLIED,
			htonl(timeouts[UDP_CT_UNREPLIED] / HZ));
	NLA_PUT_BE32(skb, CTA_TIMEOUT_UDP_REPLIED,
			htonl(timeouts[UDP_CT_REPLIED] / HZ));
=======
	if (nla_put_be32(skb, CTA_TIMEOUT_UDP_UNREPLIED,
			 htonl(timeouts[UDP_CT_UNREPLIED] / HZ)) ||
	    nla_put_be32(skb, CTA_TIMEOUT_UDP_REPLIED,
			 htonl(timeouts[UDP_CT_REPLIED] / HZ)))
		goto nla_put_failure;
>>>>>>> refs/remotes/origin/master
	return 0;

nla_put_failure:
	return -ENOSPC;
}

static const struct nla_policy
udp_timeout_nla_policy[CTA_TIMEOUT_UDP_MAX+1] = {
       [CTA_TIMEOUT_UDP_UNREPLIED]	= { .type = NLA_U32 },
       [CTA_TIMEOUT_UDP_REPLIED]	= { .type = NLA_U32 },
};
#endif /* CONFIG_NF_CT_NETLINK_TIMEOUT */

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
#ifdef CONFIG_SYSCTL
static unsigned int udp_sysctl_table_users;
static struct ctl_table_header *udp_sysctl_header;
static struct ctl_table udp_sysctl_table[] = {
	{
		.procname	= "nf_conntrack_udp_timeout",
<<<<<<< HEAD
		.data		= &nf_ct_udp_timeout,
=======
		.data		= &udp_timeouts[UDP_CT_UNREPLIED],
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef CONFIG_SYSCTL
static struct ctl_table udp_sysctl_table[] = {
	{
		.procname	= "nf_conntrack_udp_timeout",
>>>>>>> refs/remotes/origin/master
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{
		.procname	= "nf_conntrack_udp_timeout_stream",
<<<<<<< HEAD
<<<<<<< HEAD
		.data		= &nf_ct_udp_timeout_stream,
=======
		.data		= &udp_timeouts[UDP_CT_REPLIED],
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{ }
};
#ifdef CONFIG_NF_CONNTRACK_PROC_COMPAT
static struct ctl_table udp_compat_sysctl_table[] = {
	{
		.procname	= "ip_conntrack_udp_timeout",
<<<<<<< HEAD
<<<<<<< HEAD
		.data		= &nf_ct_udp_timeout,
=======
		.data		= &udp_timeouts[UDP_CT_UNREPLIED],
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{
		.procname	= "ip_conntrack_udp_timeout_stream",
<<<<<<< HEAD
<<<<<<< HEAD
		.data		= &nf_ct_udp_timeout_stream,
=======
		.data		= &udp_timeouts[UDP_CT_REPLIED],
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{ }
};
#endif /* CONFIG_NF_CONNTRACK_PROC_COMPAT */
#endif /* CONFIG_SYSCTL */

<<<<<<< HEAD
=======
static int udp_kmemdup_sysctl_table(struct nf_proto_net *pn,
				    struct nf_udp_net *un)
{
#ifdef CONFIG_SYSCTL
	if (pn->ctl_table)
		return 0;
	pn->ctl_table = kmemdup(udp_sysctl_table,
				sizeof(udp_sysctl_table),
				GFP_KERNEL);
	if (!pn->ctl_table)
		return -ENOMEM;
	pn->ctl_table[0].data = &un->timeouts[UDP_CT_UNREPLIED];
	pn->ctl_table[1].data = &un->timeouts[UDP_CT_REPLIED];
#endif
	return 0;
}

static int udp_kmemdup_compat_sysctl_table(struct nf_proto_net *pn,
					   struct nf_udp_net *un)
{
#ifdef CONFIG_SYSCTL
#ifdef CONFIG_NF_CONNTRACK_PROC_COMPAT
	pn->ctl_compat_table = kmemdup(udp_compat_sysctl_table,
				       sizeof(udp_compat_sysctl_table),
				       GFP_KERNEL);
	if (!pn->ctl_compat_table)
		return -ENOMEM;

	pn->ctl_compat_table[0].data = &un->timeouts[UDP_CT_UNREPLIED];
	pn->ctl_compat_table[1].data = &un->timeouts[UDP_CT_REPLIED];
#endif
#endif
	return 0;
}

static int udp_init_net(struct net *net, u_int16_t proto)
{
	int ret;
	struct nf_udp_net *un = udp_pernet(net);
	struct nf_proto_net *pn = &un->pn;

	if (!pn->users) {
		int i;

		for (i = 0; i < UDP_CT_MAX; i++)
			un->timeouts[i] = udp_timeouts[i];
	}

	if (proto == AF_INET) {
		ret = udp_kmemdup_compat_sysctl_table(pn, un);
		if (ret < 0)
			return ret;

		ret = udp_kmemdup_sysctl_table(pn, un);
		if (ret < 0)
			nf_ct_kfree_compat_sysctl_table(pn);
	} else
		ret = udp_kmemdup_sysctl_table(pn, un);

	return ret;
}

static struct nf_proto_net *udp_get_net_proto(struct net *net)
{
	return &net->ct.nf_ct_proto.udp.pn;
}

>>>>>>> refs/remotes/origin/master
struct nf_conntrack_l4proto nf_conntrack_l4proto_udp4 __read_mostly =
{
	.l3proto		= PF_INET,
	.l4proto		= IPPROTO_UDP,
	.name			= "udp",
	.pkt_to_tuple		= udp_pkt_to_tuple,
	.invert_tuple		= udp_invert_tuple,
	.print_tuple		= udp_print_tuple,
	.packet			= udp_packet,
<<<<<<< HEAD
<<<<<<< HEAD
	.new			= udp_new,
	.error			= udp_error,
#if defined(CONFIG_NF_CT_NETLINK) || defined(CONFIG_NF_CT_NETLINK_MODULE)
=======
=======
>>>>>>> refs/remotes/origin/master
	.get_timeouts		= udp_get_timeouts,
	.new			= udp_new,
	.error			= udp_error,
#if IS_ENABLED(CONFIG_NF_CT_NETLINK)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	.tuple_to_nlattr	= nf_ct_port_tuple_to_nlattr,
	.nlattr_to_tuple	= nf_ct_port_nlattr_to_tuple,
	.nlattr_tuple_size	= nf_ct_port_nlattr_tuple_size,
	.nla_policy		= nf_ct_port_nla_policy,
#endif
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#if IS_ENABLED(CONFIG_NF_CT_NETLINK_TIMEOUT)
	.ctnl_timeout		= {
		.nlattr_to_obj	= udp_timeout_nlattr_to_obj,
		.obj_to_nlattr	= udp_timeout_obj_to_nlattr,
		.nlattr_max	= CTA_TIMEOUT_UDP_MAX,
		.obj_size	= sizeof(unsigned int) * CTA_TIMEOUT_UDP_MAX,
		.nla_policy	= udp_timeout_nla_policy,
	},
#endif /* CONFIG_NF_CT_NETLINK_TIMEOUT */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
#ifdef CONFIG_SYSCTL
	.ctl_table_users	= &udp_sysctl_table_users,
	.ctl_table_header	= &udp_sysctl_header,
	.ctl_table		= udp_sysctl_table,
#ifdef CONFIG_NF_CONNTRACK_PROC_COMPAT
	.ctl_compat_table	= udp_compat_sysctl_table,
#endif
#endif
=======
	.init_net		= udp_init_net,
	.get_net_proto		= udp_get_net_proto,
>>>>>>> refs/remotes/origin/master
};
EXPORT_SYMBOL_GPL(nf_conntrack_l4proto_udp4);

struct nf_conntrack_l4proto nf_conntrack_l4proto_udp6 __read_mostly =
{
	.l3proto		= PF_INET6,
	.l4proto		= IPPROTO_UDP,
	.name			= "udp",
	.pkt_to_tuple		= udp_pkt_to_tuple,
	.invert_tuple		= udp_invert_tuple,
	.print_tuple		= udp_print_tuple,
	.packet			= udp_packet,
<<<<<<< HEAD
<<<<<<< HEAD
	.new			= udp_new,
	.error			= udp_error,
#if defined(CONFIG_NF_CT_NETLINK) || defined(CONFIG_NF_CT_NETLINK_MODULE)
=======
=======
>>>>>>> refs/remotes/origin/master
	.get_timeouts		= udp_get_timeouts,
	.new			= udp_new,
	.error			= udp_error,
#if IS_ENABLED(CONFIG_NF_CT_NETLINK)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	.tuple_to_nlattr	= nf_ct_port_tuple_to_nlattr,
	.nlattr_to_tuple	= nf_ct_port_nlattr_to_tuple,
	.nlattr_tuple_size	= nf_ct_port_nlattr_tuple_size,
	.nla_policy		= nf_ct_port_nla_policy,
#endif
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#if IS_ENABLED(CONFIG_NF_CT_NETLINK_TIMEOUT)
	.ctnl_timeout		= {
		.nlattr_to_obj	= udp_timeout_nlattr_to_obj,
		.obj_to_nlattr	= udp_timeout_obj_to_nlattr,
		.nlattr_max	= CTA_TIMEOUT_UDP_MAX,
		.obj_size	= sizeof(unsigned int) * CTA_TIMEOUT_UDP_MAX,
		.nla_policy	= udp_timeout_nla_policy,
	},
#endif /* CONFIG_NF_CT_NETLINK_TIMEOUT */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
#ifdef CONFIG_SYSCTL
	.ctl_table_users	= &udp_sysctl_table_users,
	.ctl_table_header	= &udp_sysctl_header,
	.ctl_table		= udp_sysctl_table,
#endif
=======
	.init_net		= udp_init_net,
	.get_net_proto		= udp_get_net_proto,
>>>>>>> refs/remotes/origin/master
};
EXPORT_SYMBOL_GPL(nf_conntrack_l4proto_udp6);
