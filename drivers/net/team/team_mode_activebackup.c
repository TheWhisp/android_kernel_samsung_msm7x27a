/*
<<<<<<< HEAD
 * net/drivers/team/team_mode_activebackup.c - Active-backup mode for team
=======
 * drivers/net/team/team_mode_activebackup.c - Active-backup mode for team
>>>>>>> refs/remotes/origin/master
 * Copyright (c) 2011 Jiri Pirko <jpirko@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/netdevice.h>
#include <net/rtnetlink.h>
#include <linux/if_team.h>

struct ab_priv {
	struct team_port __rcu *active_port;
<<<<<<< HEAD
=======
	struct team_option_inst_info *ap_opt_inst_info;
>>>>>>> refs/remotes/origin/master
};

static struct ab_priv *ab_priv(struct team *team)
{
	return (struct ab_priv *) &team->mode_priv;
}

static rx_handler_result_t ab_receive(struct team *team, struct team_port *port,
				      struct sk_buff *skb) {
	struct team_port *active_port;

	active_port = rcu_dereference(ab_priv(team)->active_port);
	if (active_port != port)
		return RX_HANDLER_EXACT;
	return RX_HANDLER_ANOTHER;
}

static bool ab_transmit(struct team *team, struct sk_buff *skb)
{
	struct team_port *active_port;

<<<<<<< HEAD
	active_port = rcu_dereference(ab_priv(team)->active_port);
	if (unlikely(!active_port))
		goto drop;
	skb->dev = active_port->dev;
	if (dev_queue_xmit(skb))
=======
	active_port = rcu_dereference_bh(ab_priv(team)->active_port);
	if (unlikely(!active_port))
		goto drop;
	if (team_dev_queue_xmit(team, active_port, skb))
>>>>>>> refs/remotes/origin/master
		return false;
	return true;

drop:
	dev_kfree_skb_any(skb);
	return false;
}

static void ab_port_leave(struct team *team, struct team_port *port)
{
<<<<<<< HEAD
	if (ab_priv(team)->active_port == port)
		RCU_INIT_POINTER(ab_priv(team)->active_port, NULL);
}

static int ab_active_port_get(struct team *team, void *arg)
{
	u32 *ifindex = arg;

	*ifindex = 0;
	if (ab_priv(team)->active_port)
		*ifindex = ab_priv(team)->active_port->dev->ifindex;
	return 0;
}

static int ab_active_port_set(struct team *team, void *arg)
{
	u32 *ifindex = arg;
	struct team_port *port;

	list_for_each_entry_rcu(port, &team->port_list, list) {
		if (port->dev->ifindex == *ifindex) {
=======
	if (ab_priv(team)->active_port == port) {
		RCU_INIT_POINTER(ab_priv(team)->active_port, NULL);
		team_option_inst_set_change(ab_priv(team)->ap_opt_inst_info);
	}
}

static int ab_active_port_init(struct team *team,
			       struct team_option_inst_info *info)
{
	ab_priv(team)->ap_opt_inst_info = info;
	return 0;
}

static int ab_active_port_get(struct team *team, struct team_gsetter_ctx *ctx)
{
	struct team_port *active_port;

	active_port = rcu_dereference_protected(ab_priv(team)->active_port,
						lockdep_is_held(&team->lock));
	if (active_port)
		ctx->data.u32_val = active_port->dev->ifindex;
	else
		ctx->data.u32_val = 0;
	return 0;
}

static int ab_active_port_set(struct team *team, struct team_gsetter_ctx *ctx)
{
	struct team_port *port;

	list_for_each_entry(port, &team->port_list, list) {
		if (port->dev->ifindex == ctx->data.u32_val) {
>>>>>>> refs/remotes/origin/master
			rcu_assign_pointer(ab_priv(team)->active_port, port);
			return 0;
		}
	}
	return -ENOENT;
}

static const struct team_option ab_options[] = {
	{
		.name = "activeport",
		.type = TEAM_OPTION_TYPE_U32,
<<<<<<< HEAD
=======
		.init = ab_active_port_init,
>>>>>>> refs/remotes/origin/master
		.getter = ab_active_port_get,
		.setter = ab_active_port_set,
	},
};

<<<<<<< HEAD
int ab_init(struct team *team)
=======
static int ab_init(struct team *team)
>>>>>>> refs/remotes/origin/master
{
	return team_options_register(team, ab_options, ARRAY_SIZE(ab_options));
}

<<<<<<< HEAD
void ab_exit(struct team *team)
=======
static void ab_exit(struct team *team)
>>>>>>> refs/remotes/origin/master
{
	team_options_unregister(team, ab_options, ARRAY_SIZE(ab_options));
}

static const struct team_mode_ops ab_mode_ops = {
	.init			= ab_init,
	.exit			= ab_exit,
	.receive		= ab_receive,
	.transmit		= ab_transmit,
	.port_leave		= ab_port_leave,
};

<<<<<<< HEAD
static struct team_mode ab_mode = {
=======
static const struct team_mode ab_mode = {
>>>>>>> refs/remotes/origin/master
	.kind		= "activebackup",
	.owner		= THIS_MODULE,
	.priv_size	= sizeof(struct ab_priv),
	.ops		= &ab_mode_ops,
};

static int __init ab_init_module(void)
{
	return team_mode_register(&ab_mode);
}

static void __exit ab_cleanup_module(void)
{
	team_mode_unregister(&ab_mode);
}

module_init(ab_init_module);
module_exit(ab_cleanup_module);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Jiri Pirko <jpirko@redhat.com>");
MODULE_DESCRIPTION("Active-backup mode for team");
MODULE_ALIAS("team-mode-activebackup");
