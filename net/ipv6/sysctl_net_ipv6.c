/*
 * sysctl_net_ipv6.c: sysctl interface to net IPV6 subsystem.
 *
 * Changes:
 * YOSHIFUJI Hideaki @USAGI:	added icmp sysctl table.
 */

#include <linux/mm.h>
#include <linux/sysctl.h>
#include <linux/in6.h>
#include <linux/ipv6.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <net/ndisc.h>
#include <net/ipv6.h>
#include <net/addrconf.h>
#include <net/inet_frag.h>

<<<<<<< HEAD
static struct ctl_table empty[1];

static ctl_table ipv6_static_skeleton[] = {
	{
		.procname	= "neigh",
		.maxlen		= 0,
		.mode		= 0555,
		.child		= empty,
	},
	{ }
};

static ctl_table ipv6_table_template[] = {
	{
		.procname	= "route",
		.maxlen		= 0,
		.mode		= 0555,
		.child		= ipv6_route_table_template
	},
	{
		.procname	= "icmp",
		.maxlen		= 0,
		.mode		= 0555,
		.child		= ipv6_icmp_table_template
	},
=======
static struct ctl_table ipv6_table_template[] = {
>>>>>>> refs/remotes/origin/master
	{
		.procname	= "bindv6only",
		.data		= &init_net.ipv6.sysctl.bindv6only,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{ }
};

<<<<<<< HEAD
static ctl_table ipv6_rotable[] = {
=======
static struct ctl_table ipv6_rotable[] = {
>>>>>>> refs/remotes/origin/master
	{
		.procname	= "mld_max_msf",
		.data		= &sysctl_mld_max_msf,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{ }
};

<<<<<<< HEAD
struct ctl_path net_ipv6_ctl_path[] = {
	{ .procname = "net", },
	{ .procname = "ipv6", },
	{ },
};
EXPORT_SYMBOL_GPL(net_ipv6_ctl_path);

=======
>>>>>>> refs/remotes/origin/master
static int __net_init ipv6_sysctl_net_init(struct net *net)
{
	struct ctl_table *ipv6_table;
	struct ctl_table *ipv6_route_table;
	struct ctl_table *ipv6_icmp_table;
	int err;

	err = -ENOMEM;
	ipv6_table = kmemdup(ipv6_table_template, sizeof(ipv6_table_template),
			     GFP_KERNEL);
	if (!ipv6_table)
		goto out;
<<<<<<< HEAD
=======
	ipv6_table[0].data = &net->ipv6.sysctl.bindv6only;
>>>>>>> refs/remotes/origin/master

	ipv6_route_table = ipv6_route_sysctl_init(net);
	if (!ipv6_route_table)
		goto out_ipv6_table;
<<<<<<< HEAD
	ipv6_table[0].child = ipv6_route_table;
=======
>>>>>>> refs/remotes/origin/master

	ipv6_icmp_table = ipv6_icmp_sysctl_init(net);
	if (!ipv6_icmp_table)
		goto out_ipv6_route_table;
<<<<<<< HEAD
	ipv6_table[1].child = ipv6_icmp_table;

	ipv6_table[2].data = &net->ipv6.sysctl.bindv6only;

	net->ipv6.sysctl.table = register_net_sysctl_table(net, net_ipv6_ctl_path,
							   ipv6_table);
	if (!net->ipv6.sysctl.table)
		goto out_ipv6_icmp_table;

	err = 0;
out:
	return err;

=======

	net->ipv6.sysctl.hdr = register_net_sysctl(net, "net/ipv6", ipv6_table);
	if (!net->ipv6.sysctl.hdr)
		goto out_ipv6_icmp_table;

	net->ipv6.sysctl.route_hdr =
		register_net_sysctl(net, "net/ipv6/route", ipv6_route_table);
	if (!net->ipv6.sysctl.route_hdr)
		goto out_unregister_ipv6_table;

	net->ipv6.sysctl.icmp_hdr =
		register_net_sysctl(net, "net/ipv6/icmp", ipv6_icmp_table);
	if (!net->ipv6.sysctl.icmp_hdr)
		goto out_unregister_route_table;

	err = 0;
out:
	return err;
out_unregister_route_table:
	unregister_net_sysctl_table(net->ipv6.sysctl.route_hdr);
out_unregister_ipv6_table:
	unregister_net_sysctl_table(net->ipv6.sysctl.hdr);
>>>>>>> refs/remotes/origin/master
out_ipv6_icmp_table:
	kfree(ipv6_icmp_table);
out_ipv6_route_table:
	kfree(ipv6_route_table);
out_ipv6_table:
	kfree(ipv6_table);
	goto out;
}

static void __net_exit ipv6_sysctl_net_exit(struct net *net)
{
	struct ctl_table *ipv6_table;
	struct ctl_table *ipv6_route_table;
	struct ctl_table *ipv6_icmp_table;

<<<<<<< HEAD
	ipv6_table = net->ipv6.sysctl.table->ctl_table_arg;
	ipv6_route_table = ipv6_table[0].child;
	ipv6_icmp_table = ipv6_table[1].child;

	unregister_net_sysctl_table(net->ipv6.sysctl.table);
=======
	ipv6_table = net->ipv6.sysctl.hdr->ctl_table_arg;
	ipv6_route_table = net->ipv6.sysctl.route_hdr->ctl_table_arg;
	ipv6_icmp_table = net->ipv6.sysctl.icmp_hdr->ctl_table_arg;

	unregister_net_sysctl_table(net->ipv6.sysctl.icmp_hdr);
	unregister_net_sysctl_table(net->ipv6.sysctl.route_hdr);
	unregister_net_sysctl_table(net->ipv6.sysctl.hdr);
>>>>>>> refs/remotes/origin/master

	kfree(ipv6_table);
	kfree(ipv6_route_table);
	kfree(ipv6_icmp_table);
}

static struct pernet_operations ipv6_sysctl_net_ops = {
	.init = ipv6_sysctl_net_init,
	.exit = ipv6_sysctl_net_exit,
};

static struct ctl_table_header *ip6_header;

int ipv6_sysctl_register(void)
{
	int err = -ENOMEM;

<<<<<<< HEAD
	ip6_header = register_net_sysctl_rotable(net_ipv6_ctl_path, ipv6_rotable);
=======
	ip6_header = register_net_sysctl(&init_net, "net/ipv6", ipv6_rotable);
>>>>>>> refs/remotes/origin/master
	if (ip6_header == NULL)
		goto out;

	err = register_pernet_subsys(&ipv6_sysctl_net_ops);
	if (err)
		goto err_pernet;
out:
	return err;

err_pernet:
	unregister_net_sysctl_table(ip6_header);
	goto out;
}

void ipv6_sysctl_unregister(void)
{
	unregister_net_sysctl_table(ip6_header);
	unregister_pernet_subsys(&ipv6_sysctl_net_ops);
}
<<<<<<< HEAD

static struct ctl_table_header *ip6_base;

int ipv6_static_sysctl_register(void)
{
	ip6_base = register_sysctl_paths(net_ipv6_ctl_path, ipv6_static_skeleton);
	if (ip6_base == NULL)
		return -ENOMEM;
	return 0;
}

void ipv6_static_sysctl_unregister(void)
{
	unregister_net_sysctl_table(ip6_base);
}
=======
>>>>>>> refs/remotes/origin/master
