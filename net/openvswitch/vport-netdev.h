/*
<<<<<<< HEAD
 * Copyright (c) 2007-2011 Nicira Networks.
=======
 * Copyright (c) 2007-2011 Nicira, Inc.
>>>>>>> refs/remotes/origin/master
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */

#ifndef VPORT_NETDEV_H
#define VPORT_NETDEV_H 1

#include <linux/netdevice.h>
<<<<<<< HEAD
=======
#include <linux/rcupdate.h>
>>>>>>> refs/remotes/origin/master

#include "vport.h"

struct vport *ovs_netdev_get_vport(struct net_device *dev);

struct netdev_vport {
<<<<<<< HEAD
=======
	struct rcu_head rcu;

>>>>>>> refs/remotes/origin/master
	struct net_device *dev;
};

static inline struct netdev_vport *
netdev_vport_priv(const struct vport *vport)
{
	return vport_priv(vport);
}

const char *ovs_netdev_get_name(const struct vport *);
<<<<<<< HEAD
const char *ovs_netdev_get_config(const struct vport *);
int ovs_netdev_get_ifindex(const struct vport *);
=======
void ovs_netdev_detach_dev(struct vport *);
>>>>>>> refs/remotes/origin/master

#endif /* vport_netdev.h */
