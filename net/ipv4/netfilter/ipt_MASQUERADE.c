/* Masquerade.  Simple mapping which alters range to a local IP address
   (depending on route). */

/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2006 Netfilter Core Team <coreteam@netfilter.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/types.h>
#include <linux/inetdevice.h>
#include <linux/ip.h>
#include <linux/timer.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <net/protocol.h>
#include <net/ip.h>
#include <net/checksum.h>
#include <net/route.h>
<<<<<<< HEAD
#include <net/netfilter/nf_nat_rule.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter/x_tables.h>
=======
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter/x_tables.h>
#include <net/netfilter/nf_nat.h>
>>>>>>> refs/remotes/origin/master

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Netfilter Core Team <coreteam@netfilter.org>");
MODULE_DESCRIPTION("Xtables: automatic-address SNAT");

/* FIXME: Multiple targets. --RR */
static int masquerade_tg_check(const struct xt_tgchk_param *par)
{
<<<<<<< HEAD
<<<<<<< HEAD
	const struct nf_nat_multi_range_compat *mr = par->targinfo;

	if (mr->range[0].flags & IP_NAT_RANGE_MAP_IPS) {
=======
	const struct nf_nat_ipv4_multi_range_compat *mr = par->targinfo;

	if (mr->range[0].flags & NF_NAT_RANGE_MAP_IPS) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	const struct nf_nat_ipv4_multi_range_compat *mr = par->targinfo;

	if (mr->range[0].flags & NF_NAT_RANGE_MAP_IPS) {
>>>>>>> refs/remotes/origin/master
		pr_debug("bad MAP_IPS.\n");
		return -EINVAL;
	}
	if (mr->rangesize != 1) {
		pr_debug("bad rangesize %u\n", mr->rangesize);
		return -EINVAL;
	}
	return 0;
}

static unsigned int
masquerade_tg(struct sk_buff *skb, const struct xt_action_param *par)
{
	struct nf_conn *ct;
	struct nf_conn_nat *nat;
	enum ip_conntrack_info ctinfo;
<<<<<<< HEAD
<<<<<<< HEAD
	struct nf_nat_range newrange;
	const struct nf_nat_multi_range_compat *mr;
=======
	struct nf_nat_ipv4_range newrange;
	const struct nf_nat_ipv4_multi_range_compat *mr;
>>>>>>> refs/remotes/origin/cm-10.0
	const struct rtable *rt;
	__be32 newsrc;
=======
	struct nf_nat_range newrange;
	const struct nf_nat_ipv4_multi_range_compat *mr;
	const struct rtable *rt;
	__be32 newsrc, nh;
>>>>>>> refs/remotes/origin/master

	NF_CT_ASSERT(par->hooknum == NF_INET_POST_ROUTING);

	ct = nf_ct_get(skb, &ctinfo);
	nat = nfct_nat(ct);

	NF_CT_ASSERT(ct && (ctinfo == IP_CT_NEW || ctinfo == IP_CT_RELATED ||
			    ctinfo == IP_CT_RELATED_REPLY));

	/* Source address is 0.0.0.0 - locally generated packet that is
	 * probably not supposed to be masqueraded.
	 */
	if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip == 0)
		return NF_ACCEPT;

	mr = par->targinfo;
	rt = skb_rtable(skb);
<<<<<<< HEAD
	newsrc = inet_select_addr(par->out, rt->rt_gateway, RT_SCOPE_UNIVERSE);
=======
	nh = rt_nexthop(rt, ip_hdr(skb)->daddr);
	newsrc = inet_select_addr(par->out, nh, RT_SCOPE_UNIVERSE);
>>>>>>> refs/remotes/origin/master
	if (!newsrc) {
		pr_info("%s ate my IP address\n", par->out->name);
		return NF_DROP;
	}

	nat->masq_index = par->out->ifindex;

	/* Transfer from original range. */
<<<<<<< HEAD
<<<<<<< HEAD
	newrange = ((struct nf_nat_range)
		{ mr->range[0].flags | IP_NAT_RANGE_MAP_IPS,
=======
	newrange = ((struct nf_nat_ipv4_range)
		{ mr->range[0].flags | NF_NAT_RANGE_MAP_IPS,
>>>>>>> refs/remotes/origin/cm-10.0
		  newsrc, newsrc,
		  mr->range[0].min, mr->range[0].max });

	/* Hand modified range to generic setup. */
<<<<<<< HEAD
	return nf_nat_setup_info(ct, &newrange, IP_NAT_MANIP_SRC);
=======
	return nf_nat_setup_info(ct, &newrange, NF_NAT_MANIP_SRC);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	memset(&newrange.min_addr, 0, sizeof(newrange.min_addr));
	memset(&newrange.max_addr, 0, sizeof(newrange.max_addr));
	newrange.flags       = mr->range[0].flags | NF_NAT_RANGE_MAP_IPS;
	newrange.min_addr.ip = newsrc;
	newrange.max_addr.ip = newsrc;
	newrange.min_proto   = mr->range[0].min;
	newrange.max_proto   = mr->range[0].max;

	/* Hand modified range to generic setup. */
	return nf_nat_setup_info(ct, &newrange, NF_NAT_MANIP_SRC);
>>>>>>> refs/remotes/origin/master
}

static int
device_cmp(struct nf_conn *i, void *ifindex)
{
	const struct nf_conn_nat *nat = nfct_nat(i);

	if (!nat)
		return 0;
<<<<<<< HEAD

=======
	if (nf_ct_l3num(i) != NFPROTO_IPV4)
		return 0;
>>>>>>> refs/remotes/origin/master
	return nat->masq_index == (int)(long)ifindex;
}

static int masq_device_event(struct notifier_block *this,
			     unsigned long event,
			     void *ptr)
{
<<<<<<< HEAD
	const struct net_device *dev = ptr;
=======
	const struct net_device *dev = netdev_notifier_info_to_dev(ptr);
>>>>>>> refs/remotes/origin/master
	struct net *net = dev_net(dev);

	if (event == NETDEV_DOWN) {
		/* Device was downed.  Search entire table for
		   conntracks which were associated with that device,
		   and forget them. */
		NF_CT_ASSERT(dev->ifindex != 0);

		nf_ct_iterate_cleanup(net, device_cmp,
<<<<<<< HEAD
				      (void *)(long)dev->ifindex);
=======
				      (void *)(long)dev->ifindex, 0, 0);
>>>>>>> refs/remotes/origin/master
	}

	return NOTIFY_DONE;
}

static int masq_inet_event(struct notifier_block *this,
			   unsigned long event,
			   void *ptr)
{
	struct net_device *dev = ((struct in_ifaddr *)ptr)->ifa_dev->dev;
<<<<<<< HEAD
	return masq_device_event(this, event, dev);
=======
	struct netdev_notifier_info info;

	netdev_notifier_info_init(&info, dev);
	return masq_device_event(this, event, &info);
>>>>>>> refs/remotes/origin/master
}

static struct notifier_block masq_dev_notifier = {
	.notifier_call	= masq_device_event,
};

static struct notifier_block masq_inet_notifier = {
	.notifier_call	= masq_inet_event,
};

static struct xt_target masquerade_tg_reg __read_mostly = {
	.name		= "MASQUERADE",
	.family		= NFPROTO_IPV4,
	.target		= masquerade_tg,
<<<<<<< HEAD
<<<<<<< HEAD
	.targetsize	= sizeof(struct nf_nat_multi_range_compat),
=======
	.targetsize	= sizeof(struct nf_nat_ipv4_multi_range_compat),
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.targetsize	= sizeof(struct nf_nat_ipv4_multi_range_compat),
>>>>>>> refs/remotes/origin/master
	.table		= "nat",
	.hooks		= 1 << NF_INET_POST_ROUTING,
	.checkentry	= masquerade_tg_check,
	.me		= THIS_MODULE,
};

static int __init masquerade_tg_init(void)
{
	int ret;

	ret = xt_register_target(&masquerade_tg_reg);

	if (ret == 0) {
		/* Register for device down reports */
		register_netdevice_notifier(&masq_dev_notifier);
		/* Register IP address change reports */
		register_inetaddr_notifier(&masq_inet_notifier);
	}

	return ret;
}

static void __exit masquerade_tg_exit(void)
{
	xt_unregister_target(&masquerade_tg_reg);
	unregister_netdevice_notifier(&masq_dev_notifier);
	unregister_inetaddr_notifier(&masq_inet_notifier);
}

module_init(masquerade_tg_init);
module_exit(masquerade_tg_exit);
