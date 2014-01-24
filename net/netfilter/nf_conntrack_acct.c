/* Accouting handling for netfilter. */

/*
 * (C) 2008 Krzysztof Piotr Oledzki <ole@ans.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/netfilter.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master

#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_extend.h>
#include <net/netfilter/nf_conntrack_acct.h>

<<<<<<< HEAD
<<<<<<< HEAD
static int nf_ct_acct __read_mostly;
=======
static bool nf_ct_acct __read_mostly;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static bool nf_ct_acct __read_mostly;
>>>>>>> refs/remotes/origin/master

module_param_named(acct, nf_ct_acct, bool, 0644);
MODULE_PARM_DESC(acct, "Enable connection tracking flow accounting.");

#ifdef CONFIG_SYSCTL
static struct ctl_table acct_sysctl_table[] = {
	{
		.procname	= "nf_conntrack_acct",
		.data		= &init_net.ct.sysctl_acct,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{}
};
#endif /* CONFIG_SYSCTL */

unsigned int
seq_print_acct(struct seq_file *s, const struct nf_conn *ct, int dir)
{
<<<<<<< HEAD
	struct nf_conn_counter *acct;
=======
	struct nf_conn_acct *acct;
	struct nf_conn_counter *counter;
>>>>>>> refs/remotes/origin/master

	acct = nf_conn_acct_find(ct);
	if (!acct)
		return 0;

<<<<<<< HEAD
	return seq_printf(s, "packets=%llu bytes=%llu ",
<<<<<<< HEAD
			  (unsigned long long)acct[dir].packets,
			  (unsigned long long)acct[dir].bytes);
=======
			  (unsigned long long)atomic64_read(&acct[dir].packets),
			  (unsigned long long)atomic64_read(&acct[dir].bytes));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	counter = acct->counter;
	return seq_printf(s, "packets=%llu bytes=%llu ",
			  (unsigned long long)atomic64_read(&counter[dir].packets),
			  (unsigned long long)atomic64_read(&counter[dir].bytes));
>>>>>>> refs/remotes/origin/master
};
EXPORT_SYMBOL_GPL(seq_print_acct);

static struct nf_ct_ext_type acct_extend __read_mostly = {
<<<<<<< HEAD
	.len	= sizeof(struct nf_conn_counter[IP_CT_DIR_MAX]),
	.align	= __alignof__(struct nf_conn_counter[IP_CT_DIR_MAX]),
=======
	.len	= sizeof(struct nf_conn_acct),
	.align	= __alignof__(struct nf_conn_acct),
>>>>>>> refs/remotes/origin/master
	.id	= NF_CT_EXT_ACCT,
};

#ifdef CONFIG_SYSCTL
static int nf_conntrack_acct_init_sysctl(struct net *net)
{
	struct ctl_table *table;

	table = kmemdup(acct_sysctl_table, sizeof(acct_sysctl_table),
			GFP_KERNEL);
	if (!table)
		goto out;

	table[0].data = &net->ct.sysctl_acct;

<<<<<<< HEAD
	net->ct.acct_sysctl_header = register_net_sysctl_table(net,
			nf_net_netfilter_sysctl_path, table);
=======
	/* Don't export sysctls to unprivileged users */
	if (net->user_ns != &init_user_ns)
		table[0].procname = NULL;

	net->ct.acct_sysctl_header = register_net_sysctl(net, "net/netfilter",
							 table);
>>>>>>> refs/remotes/origin/master
	if (!net->ct.acct_sysctl_header) {
		printk(KERN_ERR "nf_conntrack_acct: can't register to sysctl.\n");
		goto out_register;
	}
	return 0;

out_register:
	kfree(table);
out:
	return -ENOMEM;
}

static void nf_conntrack_acct_fini_sysctl(struct net *net)
{
	struct ctl_table *table;

	table = net->ct.acct_sysctl_header->ctl_table_arg;
	unregister_net_sysctl_table(net->ct.acct_sysctl_header);
	kfree(table);
}
#else
static int nf_conntrack_acct_init_sysctl(struct net *net)
{
	return 0;
}

static void nf_conntrack_acct_fini_sysctl(struct net *net)
{
}
#endif

<<<<<<< HEAD
int nf_conntrack_acct_init(struct net *net)
{
	int ret;

	net->ct.sysctl_acct = nf_ct_acct;

	if (net_eq(net, &init_net)) {
		ret = nf_ct_extend_register(&acct_extend);
		if (ret < 0) {
			printk(KERN_ERR "nf_conntrack_acct: Unable to register extension\n");
			goto out_extend_register;
		}
	}

	ret = nf_conntrack_acct_init_sysctl(net);
	if (ret < 0)
		goto out_sysctl;

	return 0;

out_sysctl:
	if (net_eq(net, &init_net))
		nf_ct_extend_unregister(&acct_extend);
out_extend_register:
	return ret;
}

void nf_conntrack_acct_fini(struct net *net)
{
	nf_conntrack_acct_fini_sysctl(net);
	if (net_eq(net, &init_net))
		nf_ct_extend_unregister(&acct_extend);
=======
int nf_conntrack_acct_pernet_init(struct net *net)
{
	net->ct.sysctl_acct = nf_ct_acct;
	return nf_conntrack_acct_init_sysctl(net);
}

void nf_conntrack_acct_pernet_fini(struct net *net)
{
	nf_conntrack_acct_fini_sysctl(net);
}

int nf_conntrack_acct_init(void)
{
	int ret = nf_ct_extend_register(&acct_extend);
	if (ret < 0)
		pr_err("nf_conntrack_acct: Unable to register extension\n");
	return ret;
}

void nf_conntrack_acct_fini(void)
{
	nf_ct_extend_unregister(&acct_extend);
>>>>>>> refs/remotes/origin/master
}
