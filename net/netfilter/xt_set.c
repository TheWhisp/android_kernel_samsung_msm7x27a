/* Copyright (C) 2000-2002 Joakim Axelsson <gozem@linux.nu>
 *                         Patrick Schaaf <bof@bof.de>
 *                         Martin Josefsson <gandalf@wlug.westbo.se>
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

/* Kernel module which implements the set match and SET target
 * for netfilter/iptables. */

#include <linux/module.h>
#include <linux/skbuff.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/version.h>

#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_set.h>
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master

#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_set.h>
#include <linux/netfilter/ipset/ip_set_timeout.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
#include <linux/netfilter/ipset/ip_set_timeout.h>
>>>>>>> refs/remotes/origin/cm-11.0

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>");
MODULE_DESCRIPTION("Xtables: IP set match and target module");
MODULE_ALIAS("xt_SET");
MODULE_ALIAS("ipt_set");
MODULE_ALIAS("ip6t_set");
MODULE_ALIAS("ipt_SET");
MODULE_ALIAS("ip6t_SET");

static inline int
match_set(ip_set_id_t index, const struct sk_buff *skb,
<<<<<<< HEAD
<<<<<<< HEAD
	  u8 pf, u8 dim, u8 flags, int inv)
{
	if (ip_set_test(index, skb, pf, dim, flags))
=======
	  const struct xt_action_param *par,
	  const struct ip_set_adt_opt *opt, int inv)
{
	if (ip_set_test(index, skb, par, opt))
>>>>>>> refs/remotes/origin/cm-10.0
=======
	  const struct xt_action_param *par,
	  struct ip_set_adt_opt *opt, int inv)
{
	if (ip_set_test(index, skb, par, opt))
>>>>>>> refs/remotes/origin/master
		inv = !inv;
	return inv;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
#define ADT_OPT(n, f, d, fs, cfs, t)	\
const struct ip_set_adt_opt n = {	\
	.family	= f,			\
	.dim = d,			\
	.flags = fs,			\
	.cmdflags = cfs,		\
	.timeout = t,			\
}
#define ADT_MOPT(n, f, d, fs, cfs, t)	\
<<<<<<< HEAD
=======
#define ADT_OPT(n, f, d, fs, cfs, t)	\
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
struct ip_set_adt_opt n = {		\
	.family	= f,			\
	.dim = d,			\
	.flags = fs,			\
	.cmdflags = cfs,		\
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	.timeout = t,			\
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	.ext.timeout = t,		\
}

>>>>>>> refs/remotes/origin/master
/* Revision 0 interface: backward compatible with netfilter/iptables */

static bool
set_match_v0(const struct sk_buff *skb, struct xt_action_param *par)
{
	const struct xt_set_info_match_v0 *info = par->matchinfo;
<<<<<<< HEAD
<<<<<<< HEAD

	return match_set(info->match_set.index, skb, par->family,
			 info->match_set.u.compat.dim,
			 info->match_set.u.compat.flags,
=======
=======
>>>>>>> refs/remotes/origin/master
	ADT_OPT(opt, par->family, info->match_set.u.compat.dim,
		info->match_set.u.compat.flags, 0, UINT_MAX);

	return match_set(info->match_set.index, skb, par, &opt,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			 info->match_set.u.compat.flags & IPSET_INV_MATCH);
}

static void
compat_flags(struct xt_set_info_v0 *info)
{
	u_int8_t i;

	/* Fill out compatibility data according to enum ip_set_kopt */
	info->u.compat.dim = IPSET_DIM_ZERO;
	if (info->u.flags[0] & IPSET_MATCH_INV)
		info->u.compat.flags |= IPSET_INV_MATCH;
	for (i = 0; i < IPSET_DIM_MAX-1 && info->u.flags[i]; i++) {
		info->u.compat.dim++;
		if (info->u.flags[i] & IPSET_SRC)
			info->u.compat.flags |= (1<<info->u.compat.dim);
	}
}

static int
set_match_v0_checkentry(const struct xt_mtchk_param *par)
{
	struct xt_set_info_match_v0 *info = par->matchinfo;
	ip_set_id_t index;

<<<<<<< HEAD
	index = ip_set_nfnl_get_byindex(info->match_set.index);

	if (index == IPSET_INVALID_ID) {
		pr_warning("Cannot find set indentified by id %u to match\n",
=======
	index = ip_set_nfnl_get_byindex(par->net, info->match_set.index);

	if (index == IPSET_INVALID_ID) {
		pr_warning("Cannot find set identified by id %u to match\n",
>>>>>>> refs/remotes/origin/master
			   info->match_set.index);
		return -ENOENT;
	}
	if (info->match_set.u.flags[IPSET_DIM_MAX-1] != 0) {
		pr_warning("Protocol error: set match dimension "
			   "is over the limit!\n");
<<<<<<< HEAD
		ip_set_nfnl_put(info->match_set.index);
=======
		ip_set_nfnl_put(par->net, info->match_set.index);
>>>>>>> refs/remotes/origin/master
		return -ERANGE;
	}

	/* Fill out compatibility data */
	compat_flags(&info->match_set);

	return 0;
}

static void
set_match_v0_destroy(const struct xt_mtdtor_param *par)
{
	struct xt_set_info_match_v0 *info = par->matchinfo;

<<<<<<< HEAD
	ip_set_nfnl_put(info->match_set.index);
}

=======
	ip_set_nfnl_put(par->net, info->match_set.index);
}

/* Revision 1 match */

static bool
set_match_v1(const struct sk_buff *skb, struct xt_action_param *par)
{
	const struct xt_set_info_match_v1 *info = par->matchinfo;
	ADT_OPT(opt, par->family, info->match_set.dim,
		info->match_set.flags, 0, UINT_MAX);

	if (opt.flags & IPSET_RETURN_NOMATCH)
		opt.cmdflags |= IPSET_FLAG_RETURN_NOMATCH;

	return match_set(info->match_set.index, skb, par, &opt,
			 info->match_set.flags & IPSET_INV_MATCH);
}

static int
set_match_v1_checkentry(const struct xt_mtchk_param *par)
{
	struct xt_set_info_match_v1 *info = par->matchinfo;
	ip_set_id_t index;

	index = ip_set_nfnl_get_byindex(par->net, info->match_set.index);

	if (index == IPSET_INVALID_ID) {
		pr_warning("Cannot find set identified by id %u to match\n",
			   info->match_set.index);
		return -ENOENT;
	}
	if (info->match_set.dim > IPSET_DIM_MAX) {
		pr_warning("Protocol error: set match dimension "
			   "is over the limit!\n");
		ip_set_nfnl_put(par->net, info->match_set.index);
		return -ERANGE;
	}

	return 0;
}

static void
set_match_v1_destroy(const struct xt_mtdtor_param *par)
{
	struct xt_set_info_match_v1 *info = par->matchinfo;

	ip_set_nfnl_put(par->net, info->match_set.index);
}

/* Revision 3 match */

static bool
match_counter(u64 counter, const struct ip_set_counter_match *info)
{
	switch (info->op) {
	case IPSET_COUNTER_NONE:
		return true;
	case IPSET_COUNTER_EQ:
		return counter == info->value;
	case IPSET_COUNTER_NE:
		return counter != info->value;
	case IPSET_COUNTER_LT:
		return counter < info->value;
	case IPSET_COUNTER_GT:
		return counter > info->value;
	}
	return false;
}

static bool
set_match_v3(const struct sk_buff *skb, struct xt_action_param *par)
{
	const struct xt_set_info_match_v3 *info = par->matchinfo;
	ADT_OPT(opt, par->family, info->match_set.dim,
		info->match_set.flags, info->flags, UINT_MAX);
	int ret;

	if (info->packets.op != IPSET_COUNTER_NONE ||
	    info->bytes.op != IPSET_COUNTER_NONE)
		opt.cmdflags |= IPSET_FLAG_MATCH_COUNTERS;

	ret = match_set(info->match_set.index, skb, par, &opt,
			info->match_set.flags & IPSET_INV_MATCH);

	if (!(ret && opt.cmdflags & IPSET_FLAG_MATCH_COUNTERS))
		return ret;

	if (!match_counter(opt.ext.packets, &info->packets))
		return 0;
	return match_counter(opt.ext.bytes, &info->bytes);
}

#define set_match_v3_checkentry	set_match_v1_checkentry
#define set_match_v3_destroy	set_match_v1_destroy

/* Revision 0 interface: backward compatible with netfilter/iptables */

>>>>>>> refs/remotes/origin/master
static unsigned int
set_target_v0(struct sk_buff *skb, const struct xt_action_param *par)
{
	const struct xt_set_info_target_v0 *info = par->targinfo;
<<<<<<< HEAD
<<<<<<< HEAD

	if (info->add_set.index != IPSET_INVALID_ID)
		ip_set_add(info->add_set.index, skb, par->family,
			   info->add_set.u.compat.dim,
			   info->add_set.u.compat.flags);
	if (info->del_set.index != IPSET_INVALID_ID)
		ip_set_del(info->del_set.index, skb, par->family,
			   info->del_set.u.compat.dim,
			   info->del_set.u.compat.flags);
=======
=======
>>>>>>> refs/remotes/origin/master
	ADT_OPT(add_opt, par->family, info->add_set.u.compat.dim,
		info->add_set.u.compat.flags, 0, UINT_MAX);
	ADT_OPT(del_opt, par->family, info->del_set.u.compat.dim,
		info->del_set.u.compat.flags, 0, UINT_MAX);

	if (info->add_set.index != IPSET_INVALID_ID)
		ip_set_add(info->add_set.index, skb, par, &add_opt);
	if (info->del_set.index != IPSET_INVALID_ID)
		ip_set_del(info->del_set.index, skb, par, &del_opt);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return XT_CONTINUE;
}

static int
set_target_v0_checkentry(const struct xt_tgchk_param *par)
{
	struct xt_set_info_target_v0 *info = par->targinfo;
	ip_set_id_t index;

	if (info->add_set.index != IPSET_INVALID_ID) {
<<<<<<< HEAD
		index = ip_set_nfnl_get_byindex(info->add_set.index);
=======
		index = ip_set_nfnl_get_byindex(par->net, info->add_set.index);
>>>>>>> refs/remotes/origin/master
		if (index == IPSET_INVALID_ID) {
			pr_warning("Cannot find add_set index %u as target\n",
				   info->add_set.index);
			return -ENOENT;
		}
	}

	if (info->del_set.index != IPSET_INVALID_ID) {
<<<<<<< HEAD
		index = ip_set_nfnl_get_byindex(info->del_set.index);
=======
		index = ip_set_nfnl_get_byindex(par->net, info->del_set.index);
>>>>>>> refs/remotes/origin/master
		if (index == IPSET_INVALID_ID) {
			pr_warning("Cannot find del_set index %u as target\n",
				   info->del_set.index);
			if (info->add_set.index != IPSET_INVALID_ID)
<<<<<<< HEAD
				ip_set_nfnl_put(info->add_set.index);
=======
				ip_set_nfnl_put(par->net, info->add_set.index);
>>>>>>> refs/remotes/origin/master
			return -ENOENT;
		}
	}
	if (info->add_set.u.flags[IPSET_DIM_MAX-1] != 0 ||
	    info->del_set.u.flags[IPSET_DIM_MAX-1] != 0) {
		pr_warning("Protocol error: SET target dimension "
			   "is over the limit!\n");
		if (info->add_set.index != IPSET_INVALID_ID)
<<<<<<< HEAD
			ip_set_nfnl_put(info->add_set.index);
		if (info->del_set.index != IPSET_INVALID_ID)
			ip_set_nfnl_put(info->del_set.index);
=======
			ip_set_nfnl_put(par->net, info->add_set.index);
		if (info->del_set.index != IPSET_INVALID_ID)
			ip_set_nfnl_put(par->net, info->del_set.index);
>>>>>>> refs/remotes/origin/master
		return -ERANGE;
	}

	/* Fill out compatibility data */
	compat_flags(&info->add_set);
	compat_flags(&info->del_set);

	return 0;
}

static void
set_target_v0_destroy(const struct xt_tgdtor_param *par)
{
	const struct xt_set_info_target_v0 *info = par->targinfo;

	if (info->add_set.index != IPSET_INVALID_ID)
<<<<<<< HEAD
		ip_set_nfnl_put(info->add_set.index);
	if (info->del_set.index != IPSET_INVALID_ID)
		ip_set_nfnl_put(info->del_set.index);
}

<<<<<<< HEAD
/* Revision 1: current interface to netfilter/iptables */

static bool
set_match(const struct sk_buff *skb, struct xt_action_param *par)
{
	const struct xt_set_info_match *info = par->matchinfo;

	return match_set(info->match_set.index, skb, par->family,
			 info->match_set.dim,
			 info->match_set.flags,
=======
/* Revision 1 match and target */

static bool
set_match_v1(const struct sk_buff *skb, struct xt_action_param *par)
{
	const struct xt_set_info_match_v1 *info = par->matchinfo;
	ADT_OPT(opt, par->family, info->match_set.dim,
		info->match_set.flags, 0, UINT_MAX);

	return match_set(info->match_set.index, skb, par, &opt,
>>>>>>> refs/remotes/origin/cm-10.0
			 info->match_set.flags & IPSET_INV_MATCH);
}

static int
<<<<<<< HEAD
set_match_checkentry(const struct xt_mtchk_param *par)
{
	struct xt_set_info_match *info = par->matchinfo;
=======
set_match_v1_checkentry(const struct xt_mtchk_param *par)
{
	struct xt_set_info_match_v1 *info = par->matchinfo;
>>>>>>> refs/remotes/origin/cm-10.0
	ip_set_id_t index;

	index = ip_set_nfnl_get_byindex(info->match_set.index);

	if (index == IPSET_INVALID_ID) {
		pr_warning("Cannot find set indentified by id %u to match\n",
			   info->match_set.index);
		return -ENOENT;
	}
	if (info->match_set.dim > IPSET_DIM_MAX) {
		pr_warning("Protocol error: set match dimension "
			   "is over the limit!\n");
		ip_set_nfnl_put(info->match_set.index);
		return -ERANGE;
	}

	return 0;
}

static void
<<<<<<< HEAD
set_match_destroy(const struct xt_mtdtor_param *par)
{
	struct xt_set_info_match *info = par->matchinfo;
=======
set_match_v1_destroy(const struct xt_mtdtor_param *par)
{
	struct xt_set_info_match_v1 *info = par->matchinfo;
>>>>>>> refs/remotes/origin/cm-10.0

	ip_set_nfnl_put(info->match_set.index);
}

static unsigned int
<<<<<<< HEAD
set_target(struct sk_buff *skb, const struct xt_action_param *par)
{
	const struct xt_set_info_target *info = par->targinfo;

	if (info->add_set.index != IPSET_INVALID_ID)
		ip_set_add(info->add_set.index,
			   skb, par->family,
			   info->add_set.dim,
			   info->add_set.flags);
	if (info->del_set.index != IPSET_INVALID_ID)
		ip_set_del(info->del_set.index,
			   skb, par->family,
			   info->del_set.dim,
			   info->del_set.flags);
=======
=======
		ip_set_nfnl_put(par->net, info->add_set.index);
	if (info->del_set.index != IPSET_INVALID_ID)
		ip_set_nfnl_put(par->net, info->del_set.index);
}

/* Revision 1 target */

static unsigned int
>>>>>>> refs/remotes/origin/master
set_target_v1(struct sk_buff *skb, const struct xt_action_param *par)
{
	const struct xt_set_info_target_v1 *info = par->targinfo;
	ADT_OPT(add_opt, par->family, info->add_set.dim,
		info->add_set.flags, 0, UINT_MAX);
	ADT_OPT(del_opt, par->family, info->del_set.dim,
		info->del_set.flags, 0, UINT_MAX);

	if (info->add_set.index != IPSET_INVALID_ID)
		ip_set_add(info->add_set.index, skb, par, &add_opt);
	if (info->del_set.index != IPSET_INVALID_ID)
		ip_set_del(info->del_set.index, skb, par, &del_opt);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return XT_CONTINUE;
}

static int
<<<<<<< HEAD
<<<<<<< HEAD
set_target_checkentry(const struct xt_tgchk_param *par)
{
	const struct xt_set_info_target *info = par->targinfo;
=======
set_target_v1_checkentry(const struct xt_tgchk_param *par)
{
	const struct xt_set_info_target_v1 *info = par->targinfo;
>>>>>>> refs/remotes/origin/cm-10.0
	ip_set_id_t index;

	if (info->add_set.index != IPSET_INVALID_ID) {
		index = ip_set_nfnl_get_byindex(info->add_set.index);
=======
set_target_v1_checkentry(const struct xt_tgchk_param *par)
{
	const struct xt_set_info_target_v1 *info = par->targinfo;
	ip_set_id_t index;

	if (info->add_set.index != IPSET_INVALID_ID) {
		index = ip_set_nfnl_get_byindex(par->net, info->add_set.index);
>>>>>>> refs/remotes/origin/master
		if (index == IPSET_INVALID_ID) {
			pr_warning("Cannot find add_set index %u as target\n",
				   info->add_set.index);
			return -ENOENT;
		}
	}

	if (info->del_set.index != IPSET_INVALID_ID) {
<<<<<<< HEAD
		index = ip_set_nfnl_get_byindex(info->del_set.index);
=======
		index = ip_set_nfnl_get_byindex(par->net, info->del_set.index);
>>>>>>> refs/remotes/origin/master
		if (index == IPSET_INVALID_ID) {
			pr_warning("Cannot find del_set index %u as target\n",
				   info->del_set.index);
			if (info->add_set.index != IPSET_INVALID_ID)
<<<<<<< HEAD
				ip_set_nfnl_put(info->add_set.index);
=======
				ip_set_nfnl_put(par->net, info->add_set.index);
>>>>>>> refs/remotes/origin/master
			return -ENOENT;
		}
	}
	if (info->add_set.dim > IPSET_DIM_MAX ||
	    info->del_set.dim > IPSET_DIM_MAX) {
		pr_warning("Protocol error: SET target dimension "
			   "is over the limit!\n");
		if (info->add_set.index != IPSET_INVALID_ID)
<<<<<<< HEAD
			ip_set_nfnl_put(info->add_set.index);
		if (info->del_set.index != IPSET_INVALID_ID)
			ip_set_nfnl_put(info->del_set.index);
=======
			ip_set_nfnl_put(par->net, info->add_set.index);
		if (info->del_set.index != IPSET_INVALID_ID)
			ip_set_nfnl_put(par->net, info->del_set.index);
>>>>>>> refs/remotes/origin/master
		return -ERANGE;
	}

	return 0;
}

static void
<<<<<<< HEAD
<<<<<<< HEAD
set_target_destroy(const struct xt_tgdtor_param *par)
{
	const struct xt_set_info_target *info = par->targinfo;
=======
set_target_v1_destroy(const struct xt_tgdtor_param *par)
{
	const struct xt_set_info_target_v1 *info = par->targinfo;
>>>>>>> refs/remotes/origin/cm-10.0

	if (info->add_set.index != IPSET_INVALID_ID)
		ip_set_nfnl_put(info->add_set.index);
	if (info->del_set.index != IPSET_INVALID_ID)
		ip_set_nfnl_put(info->del_set.index);
}

<<<<<<< HEAD
=======
=======
set_target_v1_destroy(const struct xt_tgdtor_param *par)
{
	const struct xt_set_info_target_v1 *info = par->targinfo;

	if (info->add_set.index != IPSET_INVALID_ID)
		ip_set_nfnl_put(par->net, info->add_set.index);
	if (info->del_set.index != IPSET_INVALID_ID)
		ip_set_nfnl_put(par->net, info->del_set.index);
}

>>>>>>> refs/remotes/origin/master
/* Revision 2 target */

static unsigned int
set_target_v2(struct sk_buff *skb, const struct xt_action_param *par)
{
	const struct xt_set_info_target_v2 *info = par->targinfo;
<<<<<<< HEAD
<<<<<<< HEAD
	ADT_MOPT(add_opt, par->family, info->add_set.dim,
		 info->add_set.flags, info->flags, info->timeout);
=======
	ADT_OPT(add_opt, par->family, info->add_set.dim,
		info->add_set.flags, info->flags, info->timeout);
>>>>>>> refs/remotes/origin/master
=======
	ADT_MOPT(add_opt, par->family, info->add_set.dim,
		 info->add_set.flags, info->flags, info->timeout);
>>>>>>> refs/remotes/origin/cm-11.0
	ADT_OPT(del_opt, par->family, info->del_set.dim,
		info->del_set.flags, 0, UINT_MAX);

	/* Normalize to fit into jiffies */
<<<<<<< HEAD
<<<<<<< HEAD
	if (add_opt.timeout != IPSET_NO_TIMEOUT &&
	    add_opt.timeout > UINT_MAX/MSEC_PER_SEC)
		add_opt.timeout = UINT_MAX/MSEC_PER_SEC;
=======
	if (add_opt.ext.timeout != IPSET_NO_TIMEOUT &&
	    add_opt.ext.timeout > UINT_MAX/MSEC_PER_SEC)
		add_opt.ext.timeout = UINT_MAX/MSEC_PER_SEC;
>>>>>>> refs/remotes/origin/master
=======
	if (add_opt.timeout != IPSET_NO_TIMEOUT &&
	    add_opt.timeout > UINT_MAX/MSEC_PER_SEC)
		add_opt.timeout = UINT_MAX/MSEC_PER_SEC;
>>>>>>> refs/remotes/origin/cm-11.0
	if (info->add_set.index != IPSET_INVALID_ID)
		ip_set_add(info->add_set.index, skb, par, &add_opt);
	if (info->del_set.index != IPSET_INVALID_ID)
		ip_set_del(info->del_set.index, skb, par, &del_opt);

	return XT_CONTINUE;
}

#define set_target_v2_checkentry	set_target_v1_checkentry
#define set_target_v2_destroy		set_target_v1_destroy

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static struct xt_match set_matches[] __read_mostly = {
	{
		.name		= "set",
		.family		= NFPROTO_IPV4,
		.revision	= 0,
		.match		= set_match_v0,
		.matchsize	= sizeof(struct xt_set_info_match_v0),
		.checkentry	= set_match_v0_checkentry,
		.destroy	= set_match_v0_destroy,
		.me		= THIS_MODULE
	},
	{
		.name		= "set",
		.family		= NFPROTO_IPV4,
		.revision	= 1,
<<<<<<< HEAD
<<<<<<< HEAD
		.match		= set_match,
		.matchsize	= sizeof(struct xt_set_info_match),
		.checkentry	= set_match_checkentry,
		.destroy	= set_match_destroy,
=======
=======
>>>>>>> refs/remotes/origin/master
		.match		= set_match_v1,
		.matchsize	= sizeof(struct xt_set_info_match_v1),
		.checkentry	= set_match_v1_checkentry,
		.destroy	= set_match_v1_destroy,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		.me		= THIS_MODULE
	},
	{
		.name		= "set",
		.family		= NFPROTO_IPV6,
		.revision	= 1,
<<<<<<< HEAD
<<<<<<< HEAD
		.match		= set_match,
		.matchsize	= sizeof(struct xt_set_info_match),
		.checkentry	= set_match_checkentry,
		.destroy	= set_match_destroy,
=======
=======
>>>>>>> refs/remotes/origin/master
		.match		= set_match_v1,
		.matchsize	= sizeof(struct xt_set_info_match_v1),
		.checkentry	= set_match_v1_checkentry,
		.destroy	= set_match_v1_destroy,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
		.me		= THIS_MODULE
	},
	/* --return-nomatch flag support */
	{
		.name		= "set",
		.family		= NFPROTO_IPV4,
		.revision	= 2,
		.match		= set_match_v1,
		.matchsize	= sizeof(struct xt_set_info_match_v1),
		.checkentry	= set_match_v1_checkentry,
		.destroy	= set_match_v1_destroy,
		.me		= THIS_MODULE
	},
	{
		.name		= "set",
		.family		= NFPROTO_IPV6,
		.revision	= 2,
		.match		= set_match_v1,
		.matchsize	= sizeof(struct xt_set_info_match_v1),
		.checkentry	= set_match_v1_checkentry,
		.destroy	= set_match_v1_destroy,
		.me		= THIS_MODULE
	},
	/* counters support: update, match */
	{
		.name		= "set",
		.family		= NFPROTO_IPV4,
		.revision	= 3,
		.match		= set_match_v3,
		.matchsize	= sizeof(struct xt_set_info_match_v3),
		.checkentry	= set_match_v3_checkentry,
		.destroy	= set_match_v3_destroy,
		.me		= THIS_MODULE
	},
	{
		.name		= "set",
		.family		= NFPROTO_IPV6,
		.revision	= 3,
		.match		= set_match_v3,
		.matchsize	= sizeof(struct xt_set_info_match_v3),
		.checkentry	= set_match_v3_checkentry,
		.destroy	= set_match_v3_destroy,
>>>>>>> refs/remotes/origin/master
		.me		= THIS_MODULE
	},
};

static struct xt_target set_targets[] __read_mostly = {
	{
		.name		= "SET",
		.revision	= 0,
		.family		= NFPROTO_IPV4,
		.target		= set_target_v0,
		.targetsize	= sizeof(struct xt_set_info_target_v0),
		.checkentry	= set_target_v0_checkentry,
		.destroy	= set_target_v0_destroy,
		.me		= THIS_MODULE
	},
	{
		.name		= "SET",
		.revision	= 1,
		.family		= NFPROTO_IPV4,
<<<<<<< HEAD
<<<<<<< HEAD
		.target		= set_target,
		.targetsize	= sizeof(struct xt_set_info_target),
		.checkentry	= set_target_checkentry,
		.destroy	= set_target_destroy,
=======
=======
>>>>>>> refs/remotes/origin/master
		.target		= set_target_v1,
		.targetsize	= sizeof(struct xt_set_info_target_v1),
		.checkentry	= set_target_v1_checkentry,
		.destroy	= set_target_v1_destroy,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		.me		= THIS_MODULE
	},
	{
		.name		= "SET",
		.revision	= 1,
		.family		= NFPROTO_IPV6,
<<<<<<< HEAD
<<<<<<< HEAD
		.target		= set_target,
		.targetsize	= sizeof(struct xt_set_info_target),
		.checkentry	= set_target_checkentry,
		.destroy	= set_target_destroy,
=======
=======
>>>>>>> refs/remotes/origin/master
		.target		= set_target_v1,
		.targetsize	= sizeof(struct xt_set_info_target_v1),
		.checkentry	= set_target_v1_checkentry,
		.destroy	= set_target_v1_destroy,
		.me		= THIS_MODULE
	},
<<<<<<< HEAD
=======
	/* --timeout and --exist flags support */
>>>>>>> refs/remotes/origin/master
	{
		.name		= "SET",
		.revision	= 2,
		.family		= NFPROTO_IPV4,
		.target		= set_target_v2,
		.targetsize	= sizeof(struct xt_set_info_target_v2),
		.checkentry	= set_target_v2_checkentry,
		.destroy	= set_target_v2_destroy,
		.me		= THIS_MODULE
	},
	{
		.name		= "SET",
		.revision	= 2,
		.family		= NFPROTO_IPV6,
		.target		= set_target_v2,
		.targetsize	= sizeof(struct xt_set_info_target_v2),
		.checkentry	= set_target_v2_checkentry,
		.destroy	= set_target_v2_destroy,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		.me		= THIS_MODULE
	},
};

static int __init xt_set_init(void)
{
	int ret = xt_register_matches(set_matches, ARRAY_SIZE(set_matches));

	if (!ret) {
		ret = xt_register_targets(set_targets,
					  ARRAY_SIZE(set_targets));
		if (ret)
			xt_unregister_matches(set_matches,
					      ARRAY_SIZE(set_matches));
	}
	return ret;
}

static void __exit xt_set_fini(void)
{
	xt_unregister_matches(set_matches, ARRAY_SIZE(set_matches));
	xt_unregister_targets(set_targets, ARRAY_SIZE(set_targets));
}

module_init(xt_set_init);
module_exit(xt_set_fini);
