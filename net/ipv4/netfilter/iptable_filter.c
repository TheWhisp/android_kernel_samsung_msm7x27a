/*
 * This is the 1999 rewrite of IP Firewalling, aiming for kernel 2.3.x.
 *
 * Copyright (C) 1999 Paul `Rusty' Russell & Michael J. Neuling
 * Copyright (C) 2000-2004 Netfilter Core Team <coreteam@netfilter.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/slab.h>
#include <net/ip.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Netfilter Core Team <coreteam@netfilter.org>");
MODULE_DESCRIPTION("iptables filter table");

#define FILTER_VALID_HOOKS ((1 << NF_INET_LOCAL_IN) | \
			    (1 << NF_INET_FORWARD) | \
			    (1 << NF_INET_LOCAL_OUT))

static const struct xt_table packet_filter = {
	.name		= "filter",
	.valid_hooks	= FILTER_VALID_HOOKS,
	.me		= THIS_MODULE,
	.af		= NFPROTO_IPV4,
	.priority	= NF_IP_PRI_FILTER,
};

static unsigned int
<<<<<<< HEAD
iptable_filter_hook(unsigned int hook, struct sk_buff *skb,
=======
iptable_filter_hook(const struct nf_hook_ops *ops, struct sk_buff *skb,
>>>>>>> refs/remotes/origin/master
		    const struct net_device *in, const struct net_device *out,
		    int (*okfn)(struct sk_buff *))
{
	const struct net *net;

<<<<<<< HEAD
	if (hook == NF_INET_LOCAL_OUT &&
=======
	if (ops->hooknum == NF_INET_LOCAL_OUT &&
>>>>>>> refs/remotes/origin/master
	    (skb->len < sizeof(struct iphdr) ||
	     ip_hdrlen(skb) < sizeof(struct iphdr)))
		/* root is playing with raw sockets. */
		return NF_ACCEPT;

	net = dev_net((in != NULL) ? in : out);
<<<<<<< HEAD
	return ipt_do_table(skb, hook, in, out, net->ipv4.iptable_filter);
=======
	return ipt_do_table(skb, ops->hooknum, in, out,
			    net->ipv4.iptable_filter);
>>>>>>> refs/remotes/origin/master
}

static struct nf_hook_ops *filter_ops __read_mostly;

/* Default to forward because I got too much mail already. */
<<<<<<< HEAD
<<<<<<< HEAD
static int forward = NF_ACCEPT;
=======
static bool forward = true;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static bool forward = true;
>>>>>>> refs/remotes/origin/master
module_param(forward, bool, 0000);

static int __net_init iptable_filter_net_init(struct net *net)
{
	struct ipt_replace *repl;

	repl = ipt_alloc_initial_table(&packet_filter);
	if (repl == NULL)
		return -ENOMEM;
	/* Entry 1 is the FORWARD hook */
	((struct ipt_standard *)repl->entries)[1].target.verdict =
<<<<<<< HEAD
<<<<<<< HEAD
		-forward - 1;
=======
		forward ? -NF_ACCEPT - 1 : -NF_DROP - 1;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		forward ? -NF_ACCEPT - 1 : -NF_DROP - 1;
>>>>>>> refs/remotes/origin/master

	net->ipv4.iptable_filter =
		ipt_register_table(net, &packet_filter, repl);
	kfree(repl);
<<<<<<< HEAD
	if (IS_ERR(net->ipv4.iptable_filter))
		return PTR_ERR(net->ipv4.iptable_filter);
	return 0;
=======
	return PTR_ERR_OR_ZERO(net->ipv4.iptable_filter);
>>>>>>> refs/remotes/origin/master
}

static void __net_exit iptable_filter_net_exit(struct net *net)
{
	ipt_unregister_table(net, net->ipv4.iptable_filter);
}

static struct pernet_operations iptable_filter_net_ops = {
	.init = iptable_filter_net_init,
	.exit = iptable_filter_net_exit,
};

static int __init iptable_filter_init(void)
{
	int ret;

<<<<<<< HEAD
<<<<<<< HEAD
	if (forward < 0 || forward > NF_MAX_VERDICT) {
		pr_err("iptables forward must be 0 or 1\n");
		return -EINVAL;
	}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	ret = register_pernet_subsys(&iptable_filter_net_ops);
	if (ret < 0)
		return ret;

	/* Register hooks */
	filter_ops = xt_hook_link(&packet_filter, iptable_filter_hook);
	if (IS_ERR(filter_ops)) {
		ret = PTR_ERR(filter_ops);
<<<<<<< HEAD
		goto cleanup_table;
	}

	return ret;

 cleanup_table:
	unregister_pernet_subsys(&iptable_filter_net_ops);
	return ret;
=======
		unregister_pernet_subsys(&iptable_filter_net_ops);
	}

	return ret;
>>>>>>> refs/remotes/origin/master
}

static void __exit iptable_filter_fini(void)
{
	xt_hook_unlink(&packet_filter, filter_ops);
	unregister_pernet_subsys(&iptable_filter_net_ops);
}

module_init(iptable_filter_init);
module_exit(iptable_filter_fini);
