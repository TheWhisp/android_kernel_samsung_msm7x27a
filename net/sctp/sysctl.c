/* SCTP kernel implementation
 * (C) Copyright IBM Corp. 2002, 2004
 * Copyright (c) 2002 Intel Corp.
 *
 * This file is part of the SCTP kernel implementation
 *
 * Sysctl related interfaces for SCTP.
 *
 * This SCTP implementation is free software;
 * you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This SCTP implementation is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 *                 ************************
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU CC; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Please send any bug reports or fixes you make to the
 * email address(es):
<<<<<<< HEAD
 *    lksctp developers <lksctp-developers@lists.sourceforge.net>
 *
 * Or submit a bug report through the following website:
 *    http://www.sf.net/projects/lksctp
=======
 *    lksctp developers <linux-sctp@vger.kernel.org>
>>>>>>> refs/remotes/origin/master
 *
 * Written or modified by:
 *    Mingqin Liu           <liuming@us.ibm.com>
 *    Jon Grimm             <jgrimm@us.ibm.com>
 *    Ardelle Fan           <ardelle.fan@intel.com>
 *    Ryan Layer            <rmlayer@us.ibm.com>
 *    Sridhar Samudrala     <sri@us.ibm.com>
<<<<<<< HEAD
 *
 * Any bugs reported given to us we will try to fix... any fixes shared will
 * be incorporated into the next SCTP release.
=======
>>>>>>> refs/remotes/origin/master
 */

#include <net/sctp/structs.h>
#include <net/sctp/sctp.h>
#include <linux/sysctl.h>

static int zero = 0;
static int one = 1;
static int timer_max = 86400000; /* ms in one day */
static int int_max = INT_MAX;
static int sack_timer_min = 1;
static int sack_timer_max = 500;
static int addr_scope_max = 3; /* check sctp_scope_policy_t in include/net/sctp/constants.h for max entries */
static int rwnd_scale_max = 16;
static unsigned long max_autoclose_min = 0;
static unsigned long max_autoclose_max =
	(MAX_SCHEDULE_TIMEOUT / HZ > UINT_MAX)
	? UINT_MAX : MAX_SCHEDULE_TIMEOUT / HZ;

extern long sysctl_sctp_mem[3];
extern int sysctl_sctp_rmem[3];
extern int sysctl_sctp_wmem[3];

<<<<<<< HEAD
static ctl_table sctp_table[] = {
	{
		.procname	= "rto_initial",
		.data		= &sctp_rto_initial,
=======
static int proc_sctp_do_hmac_alg(struct ctl_table *ctl, int write,
				void __user *buffer, size_t *lenp,
				loff_t *ppos);
static int proc_sctp_do_rto_min(struct ctl_table *ctl, int write,
				void __user *buffer, size_t *lenp,
				loff_t *ppos);
static int proc_sctp_do_rto_max(struct ctl_table *ctl, int write,
				void __user *buffer, size_t *lenp,
				loff_t *ppos);

static struct ctl_table sctp_table[] = {
	{
		.procname	= "sctp_mem",
		.data		= &sysctl_sctp_mem,
		.maxlen		= sizeof(sysctl_sctp_mem),
		.mode		= 0644,
		.proc_handler	= proc_doulongvec_minmax
	},
	{
		.procname	= "sctp_rmem",
		.data		= &sysctl_sctp_rmem,
		.maxlen		= sizeof(sysctl_sctp_rmem),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "sctp_wmem",
		.data		= &sysctl_sctp_wmem,
		.maxlen		= sizeof(sysctl_sctp_wmem),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},

	{ /* sentinel */ }
};

static struct ctl_table sctp_net_table[] = {
	{
		.procname	= "rto_initial",
		.data		= &init_net.sctp.rto_initial,
>>>>>>> refs/remotes/origin/master
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1         = &one,
		.extra2         = &timer_max
	},
	{
		.procname	= "rto_min",
<<<<<<< HEAD
		.data		= &sctp_rto_min,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1         = &one,
		.extra2         = &timer_max
	},
	{
		.procname	= "rto_max",
		.data		= &sctp_rto_max,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1         = &one,
		.extra2         = &timer_max
	},
	{
		.procname	= "valid_cookie_life",
		.data		= &sctp_valid_cookie_life,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1         = &one,
		.extra2         = &timer_max
	},
	{
		.procname	= "max_burst",
		.data		= &sctp_max_burst,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &zero,
		.extra2		= &int_max
	},
	{
		.procname	= "association_max_retrans",
		.data		= &sctp_max_retrans_association,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &one,
		.extra2		= &int_max
	},
	{
		.procname	= "sndbuf_policy",
		.data		= &sctp_sndbuf_policy,
=======
		.data		= &init_net.sctp.rto_min,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_sctp_do_rto_min,
		.extra1         = &one,
		.extra2         = &init_net.sctp.rto_max
	},
	{
		.procname	= "rto_max",
		.data		= &init_net.sctp.rto_max,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_sctp_do_rto_max,
		.extra1         = &init_net.sctp.rto_min,
		.extra2         = &timer_max
	},
	{
		.procname	= "rto_alpha_exp_divisor",
		.data		= &init_net.sctp.rto_alpha,
		.maxlen		= sizeof(int),
		.mode		= 0444,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "rto_beta_exp_divisor",
		.data		= &init_net.sctp.rto_beta,
		.maxlen		= sizeof(int),
		.mode		= 0444,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "max_burst",
		.data		= &init_net.sctp.max_burst,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &zero,
		.extra2		= &int_max
	},
	{
		.procname	= "cookie_preserve_enable",
		.data		= &init_net.sctp.cookie_preserve_enable,
>>>>>>> refs/remotes/origin/master
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
<<<<<<< HEAD
		.procname	= "rcvbuf_policy",
		.data		= &sctp_rcvbuf_policy,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "path_max_retrans",
		.data		= &sctp_max_retrans_path,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &one,
		.extra2		= &int_max
	},
	{
		.procname	= "max_init_retransmits",
		.data		= &sctp_max_retrans_init,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &one,
		.extra2		= &int_max
	},
	{
		.procname	= "hb_interval",
		.data		= &sctp_hb_interval,
=======
		.procname	= "cookie_hmac_alg",
		.maxlen		= 8,
		.mode		= 0644,
		.proc_handler	= proc_sctp_do_hmac_alg,
	},
	{
		.procname	= "valid_cookie_life",
		.data		= &init_net.sctp.valid_cookie_life,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1         = &one,
		.extra2         = &timer_max
	},
	{
		.procname	= "sack_timeout",
		.data		= &init_net.sctp.sack_timeout,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1         = &sack_timer_min,
		.extra2         = &sack_timer_max,
	},
	{
		.procname	= "hb_interval",
		.data		= &init_net.sctp.hb_interval,
>>>>>>> refs/remotes/origin/master
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1         = &one,
		.extra2         = &timer_max
	},
	{
<<<<<<< HEAD
		.procname	= "cookie_preserve_enable",
		.data		= &sctp_cookie_preserve_enable,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "rto_alpha_exp_divisor",
		.data		= &sctp_rto_alpha,
		.maxlen		= sizeof(int),
		.mode		= 0444,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "rto_beta_exp_divisor",
		.data		= &sctp_rto_beta,
		.maxlen		= sizeof(int),
		.mode		= 0444,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "addip_enable",
		.data		= &sctp_addip_enable,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
<<<<<<< HEAD
=======
		.procname	= "default_auto_asconf",
		.data		= &sctp_default_auto_asconf,
=======
		.procname	= "association_max_retrans",
		.data		= &init_net.sctp.max_retrans_association,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &one,
		.extra2		= &int_max
	},
	{
		.procname	= "path_max_retrans",
		.data		= &init_net.sctp.max_retrans_path,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &one,
		.extra2		= &int_max
	},
	{
		.procname	= "max_init_retransmits",
		.data		= &init_net.sctp.max_retrans_init,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &one,
		.extra2		= &int_max
	},
	{
		.procname	= "pf_retrans",
		.data		= &init_net.sctp.pf_retrans,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &zero,
		.extra2		= &int_max
	},
	{
		.procname	= "sndbuf_policy",
		.data		= &init_net.sctp.sndbuf_policy,
>>>>>>> refs/remotes/origin/master
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		.procname	= "prsctp_enable",
		.data		= &sctp_prsctp_enable,
=======
		.procname	= "rcvbuf_policy",
		.data		= &init_net.sctp.rcvbuf_policy,
>>>>>>> refs/remotes/origin/master
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
<<<<<<< HEAD
		.procname	= "sack_timeout",
		.data		= &sctp_sack_timeout,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1         = &sack_timer_min,
		.extra2         = &sack_timer_max,
	},
	{
		.procname	= "sctp_mem",
		.data		= &sysctl_sctp_mem,
		.maxlen		= sizeof(sysctl_sctp_mem),
		.mode		= 0644,
		.proc_handler	= proc_doulongvec_minmax
	},
	{
		.procname	= "sctp_rmem",
		.data		= &sysctl_sctp_rmem,
		.maxlen		= sizeof(sysctl_sctp_rmem),
=======
		.procname	= "default_auto_asconf",
		.data		= &init_net.sctp.default_auto_asconf,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "addip_enable",
		.data		= &init_net.sctp.addip_enable,
		.maxlen		= sizeof(int),
>>>>>>> refs/remotes/origin/master
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
<<<<<<< HEAD
		.procname	= "sctp_wmem",
		.data		= &sysctl_sctp_wmem,
		.maxlen		= sizeof(sysctl_sctp_wmem),
=======
		.procname	= "addip_noauth_enable",
		.data		= &init_net.sctp.addip_noauth,
		.maxlen		= sizeof(int),
>>>>>>> refs/remotes/origin/master
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
<<<<<<< HEAD
		.procname	= "auth_enable",
		.data		= &sctp_auth_enable,
=======
		.procname	= "prsctp_enable",
		.data		= &init_net.sctp.prsctp_enable,
>>>>>>> refs/remotes/origin/master
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
<<<<<<< HEAD
		.procname	= "addip_noauth_enable",
		.data		= &sctp_addip_noauth,
=======
		.procname	= "auth_enable",
		.data		= &init_net.sctp.auth_enable,
>>>>>>> refs/remotes/origin/master
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "addr_scope_policy",
<<<<<<< HEAD
		.data		= &sctp_scope_policy,
=======
		.data		= &init_net.sctp.scope_policy,
>>>>>>> refs/remotes/origin/master
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &zero,
		.extra2		= &addr_scope_max,
	},
	{
		.procname	= "rwnd_update_shift",
<<<<<<< HEAD
		.data		= &sctp_rwnd_upd_shift,
=======
		.data		= &init_net.sctp.rwnd_upd_shift,
>>>>>>> refs/remotes/origin/master
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= &proc_dointvec_minmax,
		.extra1		= &one,
		.extra2		= &rwnd_scale_max,
	},
	{
		.procname	= "max_autoclose",
<<<<<<< HEAD
		.data		= &sctp_max_autoclose,
=======
		.data		= &init_net.sctp.max_autoclose,
>>>>>>> refs/remotes/origin/master
		.maxlen		= sizeof(unsigned long),
		.mode		= 0644,
		.proc_handler	= &proc_doulongvec_minmax,
		.extra1		= &max_autoclose_min,
		.extra2		= &max_autoclose_max,
	},

	{ /* sentinel */ }
};

<<<<<<< HEAD
static struct ctl_path sctp_path[] = {
	{ .procname = "net", },
	{ .procname = "sctp", },
	{ }
};
=======
static int proc_sctp_do_hmac_alg(struct ctl_table *ctl, int write,
				void __user *buffer, size_t *lenp,
				loff_t *ppos)
{
	struct net *net = current->nsproxy->net_ns;
	char tmp[8];
	struct ctl_table tbl;
	int ret;
	int changed = 0;
	char *none = "none";

	memset(&tbl, 0, sizeof(struct ctl_table));

	if (write) {
		tbl.data = tmp;
		tbl.maxlen = 8;
	} else {
		tbl.data = net->sctp.sctp_hmac_alg ? : none;
		tbl.maxlen = strlen(tbl.data);
	}
		ret = proc_dostring(&tbl, write, buffer, lenp, ppos);

	if (write) {
#ifdef CONFIG_CRYPTO_MD5
		if (!strncmp(tmp, "md5", 3)) {
			net->sctp.sctp_hmac_alg = "md5";
			changed = 1;
		}
#endif
#ifdef CONFIG_CRYPTO_SHA1
		if (!strncmp(tmp, "sha1", 4)) {
			net->sctp.sctp_hmac_alg = "sha1";
			changed = 1;
		}
#endif
		if (!strncmp(tmp, "none", 4)) {
			net->sctp.sctp_hmac_alg = NULL;
			changed = 1;
		}

		if (!changed)
			ret = -EINVAL;
	}

	return ret;
}

static int proc_sctp_do_rto_min(struct ctl_table *ctl, int write,
				void __user *buffer, size_t *lenp,
				loff_t *ppos)
{
	struct net *net = current->nsproxy->net_ns;
	int new_value;
	struct ctl_table tbl;
	unsigned int min = *(unsigned int *) ctl->extra1;
	unsigned int max = *(unsigned int *) ctl->extra2;
	int ret;

	memset(&tbl, 0, sizeof(struct ctl_table));
	tbl.maxlen = sizeof(unsigned int);

	if (write)
		tbl.data = &new_value;
	else
		tbl.data = &net->sctp.rto_min;
	ret = proc_dointvec(&tbl, write, buffer, lenp, ppos);
	if (write) {
		if (ret || new_value > max || new_value < min)
			return -EINVAL;
		net->sctp.rto_min = new_value;
	}
	return ret;
}

static int proc_sctp_do_rto_max(struct ctl_table *ctl, int write,
				void __user *buffer, size_t *lenp,
				loff_t *ppos)
{
	struct net *net = current->nsproxy->net_ns;
	int new_value;
	struct ctl_table tbl;
	unsigned int min = *(unsigned int *) ctl->extra1;
	unsigned int max = *(unsigned int *) ctl->extra2;
	int ret;

	memset(&tbl, 0, sizeof(struct ctl_table));
	tbl.maxlen = sizeof(unsigned int);

	if (write)
		tbl.data = &new_value;
	else
		tbl.data = &net->sctp.rto_max;
	ret = proc_dointvec(&tbl, write, buffer, lenp, ppos);
	if (write) {
		if (ret || new_value > max || new_value < min)
			return -EINVAL;
		net->sctp.rto_max = new_value;
	}
	return ret;
}

int sctp_sysctl_net_register(struct net *net)
{
	struct ctl_table *table;
	int i;

	table = kmemdup(sctp_net_table, sizeof(sctp_net_table), GFP_KERNEL);
	if (!table)
		return -ENOMEM;

	for (i = 0; table[i].data; i++)
		table[i].data += (char *)(&net->sctp) - (char *)&init_net.sctp;

	net->sctp.sysctl_header = register_net_sysctl(net, "net/sctp", table);
	return 0;
}

void sctp_sysctl_net_unregister(struct net *net)
{
	struct ctl_table *table;

	table = net->sctp.sysctl_header->ctl_table_arg;
	unregister_net_sysctl_table(net->sctp.sysctl_header);
	kfree(table);
}
>>>>>>> refs/remotes/origin/master

static struct ctl_table_header * sctp_sysctl_header;

/* Sysctl registration.  */
void sctp_sysctl_register(void)
{
<<<<<<< HEAD
	sctp_sysctl_header = register_sysctl_paths(sctp_path, sctp_table);
=======
	sctp_sysctl_header = register_net_sysctl(&init_net, "net/sctp", sctp_table);
>>>>>>> refs/remotes/origin/master
}

/* Sysctl deregistration.  */
void sctp_sysctl_unregister(void)
{
<<<<<<< HEAD
	unregister_sysctl_table(sctp_sysctl_header);
=======
	unregister_net_sysctl_table(sctp_sysctl_header);
>>>>>>> refs/remotes/origin/master
}
