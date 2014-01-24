/*
 * sysctl_net_atalk.c: sysctl interface to net AppleTalk subsystem.
 *
 * Begun April 1, 1996, Mike Shaver.
 * Added /proc/sys/net/atalk directory entry (empty =) ). [MS]
 * Dynamic registration, added aarp entries. (5/30/97 Chris Horn)
 */

#include <linux/sysctl.h>
#include <net/sock.h>
#include <linux/atalk.h>

static struct ctl_table atalk_table[] = {
	{
		.procname	= "aarp-expiry-time",
		.data		= &sysctl_aarp_expiry_time,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{
		.procname	= "aarp-tick-time",
		.data		= &sysctl_aarp_tick_time,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{
		.procname	= "aarp-retransmit-limit",
		.data		= &sysctl_aarp_retransmit_limit,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "aarp-resolve-time",
		.data		= &sysctl_aarp_resolve_time,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{ },
};

<<<<<<< HEAD
static struct ctl_path atalk_path[] = {
	{ .procname = "net", },
	{ .procname = "appletalk", },
	{ }
};

=======
>>>>>>> refs/remotes/origin/master
static struct ctl_table_header *atalk_table_header;

void atalk_register_sysctl(void)
{
<<<<<<< HEAD
	atalk_table_header = register_sysctl_paths(atalk_path, atalk_table);
=======
	atalk_table_header = register_net_sysctl(&init_net, "net/appletalk", atalk_table);
>>>>>>> refs/remotes/origin/master
}

void atalk_unregister_sysctl(void)
{
<<<<<<< HEAD
	unregister_sysctl_table(atalk_table_header);
=======
	unregister_net_sysctl_table(atalk_table_header);
>>>>>>> refs/remotes/origin/master
}
