/*
 * net/core/dev_addr_lists.c - Functions for handling net device lists
 * Copyright (c) 2010 Jiri Pirko <jpirko@redhat.com>
 *
 * This file contains functions for working with unicast, multicast and device
 * addresses lists.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/netdevice.h>
#include <linux/rtnetlink.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/list.h>
#include <linux/proc_fs.h>
=======
#include <linux/export.h>
#include <linux/list.h>
>>>>>>> refs/remotes/origin/master

/*
 * General list handling functions
 */

<<<<<<< HEAD
static int __hw_addr_add_ex(struct netdev_hw_addr_list *list,
			    unsigned char *addr, int addr_len,
			    unsigned char addr_type, bool global)
=======
static int __hw_addr_create_ex(struct netdev_hw_addr_list *list,
			       const unsigned char *addr, int addr_len,
			       unsigned char addr_type, bool global,
			       bool sync)
>>>>>>> refs/remotes/origin/master
{
	struct netdev_hw_addr *ha;
	int alloc_size;

<<<<<<< HEAD
=======
	alloc_size = sizeof(*ha);
	if (alloc_size < L1_CACHE_BYTES)
		alloc_size = L1_CACHE_BYTES;
	ha = kmalloc(alloc_size, GFP_ATOMIC);
	if (!ha)
		return -ENOMEM;
	memcpy(ha->addr, addr, addr_len);
	ha->type = addr_type;
	ha->refcount = 1;
	ha->global_use = global;
	ha->synced = sync;
	ha->sync_cnt = 0;
	list_add_tail_rcu(&ha->list, &list->list);
	list->count++;

	return 0;
}

static int __hw_addr_add_ex(struct netdev_hw_addr_list *list,
			    const unsigned char *addr, int addr_len,
			    unsigned char addr_type, bool global, bool sync)
{
	struct netdev_hw_addr *ha;

>>>>>>> refs/remotes/origin/master
	if (addr_len > MAX_ADDR_LEN)
		return -EINVAL;

	list_for_each_entry(ha, &list->list, list) {
		if (!memcmp(ha->addr, addr, addr_len) &&
		    ha->type == addr_type) {
			if (global) {
				/* check if addr is already used as global */
				if (ha->global_use)
					return 0;
				else
					ha->global_use = true;
			}
<<<<<<< HEAD
=======
			if (sync) {
				if (ha->synced)
					return -EEXIST;
				else
					ha->synced = true;
			}
>>>>>>> refs/remotes/origin/master
			ha->refcount++;
			return 0;
		}
	}

<<<<<<< HEAD

	alloc_size = sizeof(*ha);
	if (alloc_size < L1_CACHE_BYTES)
		alloc_size = L1_CACHE_BYTES;
	ha = kmalloc(alloc_size, GFP_ATOMIC);
	if (!ha)
		return -ENOMEM;
	memcpy(ha->addr, addr, addr_len);
	ha->type = addr_type;
	ha->refcount = 1;
	ha->global_use = global;
	ha->synced = 0;
	list_add_tail_rcu(&ha->list, &list->list);
	list->count++;
	return 0;
}

static int __hw_addr_add(struct netdev_hw_addr_list *list, unsigned char *addr,
			 int addr_len, unsigned char addr_type)
{
	return __hw_addr_add_ex(list, addr, addr_len, addr_type, false);
}

static int __hw_addr_del_ex(struct netdev_hw_addr_list *list,
			    unsigned char *addr, int addr_len,
			    unsigned char addr_type, bool global)
=======
	return __hw_addr_create_ex(list, addr, addr_len, addr_type, global,
				   sync);
}

static int __hw_addr_add(struct netdev_hw_addr_list *list,
			 const unsigned char *addr, int addr_len,
			 unsigned char addr_type)
{
	return __hw_addr_add_ex(list, addr, addr_len, addr_type, false, false);
}

static int __hw_addr_del_entry(struct netdev_hw_addr_list *list,
			       struct netdev_hw_addr *ha, bool global,
			       bool sync)
{
	if (global && !ha->global_use)
		return -ENOENT;

	if (sync && !ha->synced)
		return -ENOENT;

	if (global)
		ha->global_use = false;

	if (sync)
		ha->synced = false;

	if (--ha->refcount)
		return 0;
	list_del_rcu(&ha->list);
	kfree_rcu(ha, rcu_head);
	list->count--;
	return 0;
}

static int __hw_addr_del_ex(struct netdev_hw_addr_list *list,
			    const unsigned char *addr, int addr_len,
			    unsigned char addr_type, bool global, bool sync)
>>>>>>> refs/remotes/origin/master
{
	struct netdev_hw_addr *ha;

	list_for_each_entry(ha, &list->list, list) {
		if (!memcmp(ha->addr, addr, addr_len) &&
<<<<<<< HEAD
		    (ha->type == addr_type || !addr_type)) {
			if (global) {
				if (!ha->global_use)
					break;
				else
					ha->global_use = false;
			}
			if (--ha->refcount)
				return 0;
			list_del_rcu(&ha->list);
			kfree_rcu(ha, rcu_head);
			list->count--;
			return 0;
		}
=======
		    (ha->type == addr_type || !addr_type))
			return __hw_addr_del_entry(list, ha, global, sync);
>>>>>>> refs/remotes/origin/master
	}
	return -ENOENT;
}

<<<<<<< HEAD
static int __hw_addr_del(struct netdev_hw_addr_list *list, unsigned char *addr,
			 int addr_len, unsigned char addr_type)
{
	return __hw_addr_del_ex(list, addr, addr_len, addr_type, false);
=======
static int __hw_addr_del(struct netdev_hw_addr_list *list,
			 const unsigned char *addr, int addr_len,
			 unsigned char addr_type)
{
	return __hw_addr_del_ex(list, addr, addr_len, addr_type, false, false);
}

static int __hw_addr_sync_one(struct netdev_hw_addr_list *to_list,
			       struct netdev_hw_addr *ha,
			       int addr_len)
{
	int err;

	err = __hw_addr_add_ex(to_list, ha->addr, addr_len, ha->type,
			       false, true);
	if (err && err != -EEXIST)
		return err;

	if (!err) {
		ha->sync_cnt++;
		ha->refcount++;
	}

	return 0;
}

static void __hw_addr_unsync_one(struct netdev_hw_addr_list *to_list,
				 struct netdev_hw_addr_list *from_list,
				 struct netdev_hw_addr *ha,
				 int addr_len)
{
	int err;

	err = __hw_addr_del_ex(to_list, ha->addr, addr_len, ha->type,
			       false, true);
	if (err)
		return;
	ha->sync_cnt--;
	/* address on from list is not marked synced */
	__hw_addr_del_entry(from_list, ha, false, false);
}

static int __hw_addr_sync_multiple(struct netdev_hw_addr_list *to_list,
				   struct netdev_hw_addr_list *from_list,
				   int addr_len)
{
	int err = 0;
	struct netdev_hw_addr *ha, *tmp;

	list_for_each_entry_safe(ha, tmp, &from_list->list, list) {
		if (ha->sync_cnt == ha->refcount) {
			__hw_addr_unsync_one(to_list, from_list, ha, addr_len);
		} else {
			err = __hw_addr_sync_one(to_list, ha, addr_len);
			if (err)
				break;
		}
	}
	return err;
>>>>>>> refs/remotes/origin/master
}

int __hw_addr_add_multiple(struct netdev_hw_addr_list *to_list,
			   struct netdev_hw_addr_list *from_list,
			   int addr_len, unsigned char addr_type)
{
	int err;
	struct netdev_hw_addr *ha, *ha2;
	unsigned char type;

	list_for_each_entry(ha, &from_list->list, list) {
		type = addr_type ? addr_type : ha->type;
		err = __hw_addr_add(to_list, ha->addr, addr_len, type);
		if (err)
			goto unroll;
	}
	return 0;

unroll:
	list_for_each_entry(ha2, &from_list->list, list) {
		if (ha2 == ha)
			break;
		type = addr_type ? addr_type : ha2->type;
		__hw_addr_del(to_list, ha2->addr, addr_len, type);
	}
	return err;
}
EXPORT_SYMBOL(__hw_addr_add_multiple);

void __hw_addr_del_multiple(struct netdev_hw_addr_list *to_list,
			    struct netdev_hw_addr_list *from_list,
			    int addr_len, unsigned char addr_type)
{
	struct netdev_hw_addr *ha;
	unsigned char type;

	list_for_each_entry(ha, &from_list->list, list) {
		type = addr_type ? addr_type : ha->type;
		__hw_addr_del(to_list, ha->addr, addr_len, type);
	}
}
EXPORT_SYMBOL(__hw_addr_del_multiple);

<<<<<<< HEAD
=======
/* This function only works where there is a strict 1-1 relationship
 * between source and destionation of they synch. If you ever need to
 * sync addresses to more then 1 destination, you need to use
 * __hw_addr_sync_multiple().
 */
>>>>>>> refs/remotes/origin/master
int __hw_addr_sync(struct netdev_hw_addr_list *to_list,
		   struct netdev_hw_addr_list *from_list,
		   int addr_len)
{
	int err = 0;
	struct netdev_hw_addr *ha, *tmp;

	list_for_each_entry_safe(ha, tmp, &from_list->list, list) {
<<<<<<< HEAD
		if (!ha->synced) {
			err = __hw_addr_add(to_list, ha->addr,
					    addr_len, ha->type);
			if (err)
				break;
			ha->synced++;
			ha->refcount++;
		} else if (ha->refcount == 1) {
			__hw_addr_del(to_list, ha->addr, addr_len, ha->type);
			__hw_addr_del(from_list, ha->addr, addr_len, ha->type);
		}
=======
		if (!ha->sync_cnt) {
			err = __hw_addr_sync_one(to_list, ha, addr_len);
			if (err)
				break;
		} else if (ha->refcount == 1)
			__hw_addr_unsync_one(to_list, from_list, ha, addr_len);
>>>>>>> refs/remotes/origin/master
	}
	return err;
}
EXPORT_SYMBOL(__hw_addr_sync);

void __hw_addr_unsync(struct netdev_hw_addr_list *to_list,
		      struct netdev_hw_addr_list *from_list,
		      int addr_len)
{
	struct netdev_hw_addr *ha, *tmp;

	list_for_each_entry_safe(ha, tmp, &from_list->list, list) {
<<<<<<< HEAD
		if (ha->synced) {
			__hw_addr_del(to_list, ha->addr,
				      addr_len, ha->type);
			ha->synced--;
			__hw_addr_del(from_list, ha->addr,
				      addr_len, ha->type);
		}
=======
		if (ha->sync_cnt)
			__hw_addr_unsync_one(to_list, from_list, ha, addr_len);
>>>>>>> refs/remotes/origin/master
	}
}
EXPORT_SYMBOL(__hw_addr_unsync);

void __hw_addr_flush(struct netdev_hw_addr_list *list)
{
	struct netdev_hw_addr *ha, *tmp;

	list_for_each_entry_safe(ha, tmp, &list->list, list) {
		list_del_rcu(&ha->list);
		kfree_rcu(ha, rcu_head);
	}
	list->count = 0;
}
EXPORT_SYMBOL(__hw_addr_flush);

void __hw_addr_init(struct netdev_hw_addr_list *list)
{
	INIT_LIST_HEAD(&list->list);
	list->count = 0;
}
EXPORT_SYMBOL(__hw_addr_init);

/*
 * Device addresses handling functions
 */

/**
 *	dev_addr_flush - Flush device address list
 *	@dev: device
 *
 *	Flush device address list and reset ->dev_addr.
 *
 *	The caller must hold the rtnl_mutex.
 */
void dev_addr_flush(struct net_device *dev)
{
	/* rtnl_mutex must be held here */

	__hw_addr_flush(&dev->dev_addrs);
	dev->dev_addr = NULL;
}
EXPORT_SYMBOL(dev_addr_flush);

/**
 *	dev_addr_init - Init device address list
 *	@dev: device
 *
 *	Init device address list and create the first element,
 *	used by ->dev_addr.
 *
 *	The caller must hold the rtnl_mutex.
 */
int dev_addr_init(struct net_device *dev)
{
	unsigned char addr[MAX_ADDR_LEN];
	struct netdev_hw_addr *ha;
	int err;

	/* rtnl_mutex must be held here */

	__hw_addr_init(&dev->dev_addrs);
	memset(addr, 0, sizeof(addr));
	err = __hw_addr_add(&dev->dev_addrs, addr, sizeof(addr),
			    NETDEV_HW_ADDR_T_LAN);
	if (!err) {
		/*
		 * Get the first (previously created) address from the list
		 * and set dev_addr pointer to this location.
		 */
		ha = list_first_entry(&dev->dev_addrs.list,
				      struct netdev_hw_addr, list);
		dev->dev_addr = ha->addr;
	}
	return err;
}
EXPORT_SYMBOL(dev_addr_init);

/**
 *	dev_addr_add - Add a device address
 *	@dev: device
 *	@addr: address to add
 *	@addr_type: address type
 *
 *	Add a device address to the device or increase the reference count if
 *	it already exists.
 *
 *	The caller must hold the rtnl_mutex.
 */
<<<<<<< HEAD
int dev_addr_add(struct net_device *dev, unsigned char *addr,
=======
int dev_addr_add(struct net_device *dev, const unsigned char *addr,
>>>>>>> refs/remotes/origin/master
		 unsigned char addr_type)
{
	int err;

	ASSERT_RTNL();

	err = __hw_addr_add(&dev->dev_addrs, addr, dev->addr_len, addr_type);
	if (!err)
		call_netdevice_notifiers(NETDEV_CHANGEADDR, dev);
	return err;
}
EXPORT_SYMBOL(dev_addr_add);

/**
 *	dev_addr_del - Release a device address.
 *	@dev: device
 *	@addr: address to delete
 *	@addr_type: address type
 *
 *	Release reference to a device address and remove it from the device
 *	if the reference count drops to zero.
 *
 *	The caller must hold the rtnl_mutex.
 */
<<<<<<< HEAD
int dev_addr_del(struct net_device *dev, unsigned char *addr,
=======
int dev_addr_del(struct net_device *dev, const unsigned char *addr,
>>>>>>> refs/remotes/origin/master
		 unsigned char addr_type)
{
	int err;
	struct netdev_hw_addr *ha;

	ASSERT_RTNL();

	/*
	 * We can not remove the first address from the list because
	 * dev->dev_addr points to that.
	 */
	ha = list_first_entry(&dev->dev_addrs.list,
			      struct netdev_hw_addr, list);
	if (!memcmp(ha->addr, addr, dev->addr_len) &&
	    ha->type == addr_type && ha->refcount == 1)
		return -ENOENT;

	err = __hw_addr_del(&dev->dev_addrs, addr, dev->addr_len,
			    addr_type);
	if (!err)
		call_netdevice_notifiers(NETDEV_CHANGEADDR, dev);
	return err;
}
EXPORT_SYMBOL(dev_addr_del);

/**
 *	dev_addr_add_multiple - Add device addresses from another device
 *	@to_dev: device to which addresses will be added
 *	@from_dev: device from which addresses will be added
 *	@addr_type: address type - 0 means type will be used from from_dev
 *
 *	Add device addresses of the one device to another.
 **
 *	The caller must hold the rtnl_mutex.
 */
int dev_addr_add_multiple(struct net_device *to_dev,
			  struct net_device *from_dev,
			  unsigned char addr_type)
{
	int err;

	ASSERT_RTNL();

	if (from_dev->addr_len != to_dev->addr_len)
		return -EINVAL;
	err = __hw_addr_add_multiple(&to_dev->dev_addrs, &from_dev->dev_addrs,
				     to_dev->addr_len, addr_type);
	if (!err)
		call_netdevice_notifiers(NETDEV_CHANGEADDR, to_dev);
	return err;
}
EXPORT_SYMBOL(dev_addr_add_multiple);

/**
 *	dev_addr_del_multiple - Delete device addresses by another device
 *	@to_dev: device where the addresses will be deleted
 *	@from_dev: device supplying the addresses to be deleted
 *	@addr_type: address type - 0 means type will be used from from_dev
 *
 *	Deletes addresses in to device by the list of addresses in from device.
 *
 *	The caller must hold the rtnl_mutex.
 */
int dev_addr_del_multiple(struct net_device *to_dev,
			  struct net_device *from_dev,
			  unsigned char addr_type)
{
	ASSERT_RTNL();

	if (from_dev->addr_len != to_dev->addr_len)
		return -EINVAL;
	__hw_addr_del_multiple(&to_dev->dev_addrs, &from_dev->dev_addrs,
			       to_dev->addr_len, addr_type);
	call_netdevice_notifiers(NETDEV_CHANGEADDR, to_dev);
	return 0;
}
EXPORT_SYMBOL(dev_addr_del_multiple);

/*
 * Unicast list handling functions
 */

/**
<<<<<<< HEAD
=======
 *	dev_uc_add_excl - Add a global secondary unicast address
 *	@dev: device
 *	@addr: address to add
 */
int dev_uc_add_excl(struct net_device *dev, const unsigned char *addr)
{
	struct netdev_hw_addr *ha;
	int err;

	netif_addr_lock_bh(dev);
	list_for_each_entry(ha, &dev->uc.list, list) {
		if (!memcmp(ha->addr, addr, dev->addr_len) &&
		    ha->type == NETDEV_HW_ADDR_T_UNICAST) {
			err = -EEXIST;
			goto out;
		}
	}
	err = __hw_addr_create_ex(&dev->uc, addr, dev->addr_len,
				  NETDEV_HW_ADDR_T_UNICAST, true, false);
	if (!err)
		__dev_set_rx_mode(dev);
out:
	netif_addr_unlock_bh(dev);
	return err;
}
EXPORT_SYMBOL(dev_uc_add_excl);

/**
>>>>>>> refs/remotes/origin/master
 *	dev_uc_add - Add a secondary unicast address
 *	@dev: device
 *	@addr: address to add
 *
 *	Add a secondary unicast address to the device or increase
 *	the reference count if it already exists.
 */
<<<<<<< HEAD
int dev_uc_add(struct net_device *dev, unsigned char *addr)
=======
int dev_uc_add(struct net_device *dev, const unsigned char *addr)
>>>>>>> refs/remotes/origin/master
{
	int err;

	netif_addr_lock_bh(dev);
	err = __hw_addr_add(&dev->uc, addr, dev->addr_len,
			    NETDEV_HW_ADDR_T_UNICAST);
	if (!err)
		__dev_set_rx_mode(dev);
	netif_addr_unlock_bh(dev);
	return err;
}
EXPORT_SYMBOL(dev_uc_add);

/**
 *	dev_uc_del - Release secondary unicast address.
 *	@dev: device
 *	@addr: address to delete
 *
 *	Release reference to a secondary unicast address and remove it
 *	from the device if the reference count drops to zero.
 */
<<<<<<< HEAD
int dev_uc_del(struct net_device *dev, unsigned char *addr)
=======
int dev_uc_del(struct net_device *dev, const unsigned char *addr)
>>>>>>> refs/remotes/origin/master
{
	int err;

	netif_addr_lock_bh(dev);
	err = __hw_addr_del(&dev->uc, addr, dev->addr_len,
			    NETDEV_HW_ADDR_T_UNICAST);
	if (!err)
		__dev_set_rx_mode(dev);
	netif_addr_unlock_bh(dev);
	return err;
}
EXPORT_SYMBOL(dev_uc_del);

/**
 *	dev_uc_sync - Synchronize device's unicast list to another device
 *	@to: destination device
 *	@from: source device
 *
 *	Add newly added addresses to the destination device and release
 *	addresses that have no users left. The source device must be
<<<<<<< HEAD
<<<<<<< HEAD
 *	locked by netif_tx_lock_bh.
=======
 *	locked by netif_addr_lock_bh.
>>>>>>> refs/remotes/origin/cm-10.0
 *
 *	This function is intended to be called from the dev->set_rx_mode
 *	function of layered software devices.
=======
 *	locked by netif_addr_lock_bh.
 *
 *	This function is intended to be called from the dev->set_rx_mode
 *	function of layered software devices.  This function assumes that
 *	addresses will only ever be synced to the @to devices and no other.
>>>>>>> refs/remotes/origin/master
 */
int dev_uc_sync(struct net_device *to, struct net_device *from)
{
	int err = 0;

	if (to->addr_len != from->addr_len)
		return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
	netif_addr_lock_bh(to);
	err = __hw_addr_sync(&to->uc, &from->uc, to->addr_len);
	if (!err)
		__dev_set_rx_mode(to);
	netif_addr_unlock_bh(to);
=======
	netif_addr_lock_nested(to);
	err = __hw_addr_sync(&to->uc, &from->uc, to->addr_len);
	if (!err)
		__dev_set_rx_mode(to);
	netif_addr_unlock(to);
>>>>>>> refs/remotes/origin/cm-10.0
	return err;
}
EXPORT_SYMBOL(dev_uc_sync);
=======
	netif_addr_lock_nested(to);
	err = __hw_addr_sync(&to->uc, &from->uc, to->addr_len);
	if (!err)
		__dev_set_rx_mode(to);
	netif_addr_unlock(to);
	return err;
}
EXPORT_SYMBOL(dev_uc_sync);

/**
 *	dev_uc_sync_multiple - Synchronize device's unicast list to another
 *	device, but allow for multiple calls to sync to multiple devices.
 *	@to: destination device
 *	@from: source device
 *
 *	Add newly added addresses to the destination device and release
 *	addresses that have been deleted from the source. The source device
 *	must be locked by netif_addr_lock_bh.
 *
 *	This function is intended to be called from the dev->set_rx_mode
 *	function of layered software devices.  It allows for a single source
 *	device to be synced to multiple destination devices.
 */
int dev_uc_sync_multiple(struct net_device *to, struct net_device *from)
{
	int err = 0;

	if (to->addr_len != from->addr_len)
		return -EINVAL;

	netif_addr_lock_nested(to);
	err = __hw_addr_sync_multiple(&to->uc, &from->uc, to->addr_len);
	if (!err)
		__dev_set_rx_mode(to);
	netif_addr_unlock(to);
	return err;
}
EXPORT_SYMBOL(dev_uc_sync_multiple);
>>>>>>> refs/remotes/origin/master

/**
 *	dev_uc_unsync - Remove synchronized addresses from the destination device
 *	@to: destination device
 *	@from: source device
 *
 *	Remove all addresses that were added to the destination device by
 *	dev_uc_sync(). This function is intended to be called from the
 *	dev->stop function of layered software devices.
 */
void dev_uc_unsync(struct net_device *to, struct net_device *from)
{
	if (to->addr_len != from->addr_len)
		return;

	netif_addr_lock_bh(from);
<<<<<<< HEAD
<<<<<<< HEAD
	netif_addr_lock(to);
=======
	netif_addr_lock_nested(to);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	netif_addr_lock_nested(to);
>>>>>>> refs/remotes/origin/master
	__hw_addr_unsync(&to->uc, &from->uc, to->addr_len);
	__dev_set_rx_mode(to);
	netif_addr_unlock(to);
	netif_addr_unlock_bh(from);
}
EXPORT_SYMBOL(dev_uc_unsync);

/**
 *	dev_uc_flush - Flush unicast addresses
 *	@dev: device
 *
 *	Flush unicast addresses.
 */
void dev_uc_flush(struct net_device *dev)
{
	netif_addr_lock_bh(dev);
	__hw_addr_flush(&dev->uc);
	netif_addr_unlock_bh(dev);
}
EXPORT_SYMBOL(dev_uc_flush);

/**
 *	dev_uc_flush - Init unicast address list
 *	@dev: device
 *
 *	Init unicast address list.
 */
void dev_uc_init(struct net_device *dev)
{
	__hw_addr_init(&dev->uc);
}
EXPORT_SYMBOL(dev_uc_init);

/*
 * Multicast list handling functions
 */

<<<<<<< HEAD
static int __dev_mc_add(struct net_device *dev, unsigned char *addr,
=======
/**
 *	dev_mc_add_excl - Add a global secondary multicast address
 *	@dev: device
 *	@addr: address to add
 */
int dev_mc_add_excl(struct net_device *dev, const unsigned char *addr)
{
	struct netdev_hw_addr *ha;
	int err;

	netif_addr_lock_bh(dev);
	list_for_each_entry(ha, &dev->mc.list, list) {
		if (!memcmp(ha->addr, addr, dev->addr_len) &&
		    ha->type == NETDEV_HW_ADDR_T_MULTICAST) {
			err = -EEXIST;
			goto out;
		}
	}
	err = __hw_addr_create_ex(&dev->mc, addr, dev->addr_len,
				  NETDEV_HW_ADDR_T_MULTICAST, true, false);
	if (!err)
		__dev_set_rx_mode(dev);
out:
	netif_addr_unlock_bh(dev);
	return err;
}
EXPORT_SYMBOL(dev_mc_add_excl);

static int __dev_mc_add(struct net_device *dev, const unsigned char *addr,
>>>>>>> refs/remotes/origin/master
			bool global)
{
	int err;

	netif_addr_lock_bh(dev);
	err = __hw_addr_add_ex(&dev->mc, addr, dev->addr_len,
<<<<<<< HEAD
			       NETDEV_HW_ADDR_T_MULTICAST, global);
=======
			       NETDEV_HW_ADDR_T_MULTICAST, global, false);
>>>>>>> refs/remotes/origin/master
	if (!err)
		__dev_set_rx_mode(dev);
	netif_addr_unlock_bh(dev);
	return err;
}
/**
 *	dev_mc_add - Add a multicast address
 *	@dev: device
 *	@addr: address to add
 *
 *	Add a multicast address to the device or increase
 *	the reference count if it already exists.
 */
<<<<<<< HEAD
int dev_mc_add(struct net_device *dev, unsigned char *addr)
=======
int dev_mc_add(struct net_device *dev, const unsigned char *addr)
>>>>>>> refs/remotes/origin/master
{
	return __dev_mc_add(dev, addr, false);
}
EXPORT_SYMBOL(dev_mc_add);

/**
 *	dev_mc_add_global - Add a global multicast address
 *	@dev: device
 *	@addr: address to add
 *
 *	Add a global multicast address to the device.
 */
<<<<<<< HEAD
int dev_mc_add_global(struct net_device *dev, unsigned char *addr)
=======
int dev_mc_add_global(struct net_device *dev, const unsigned char *addr)
>>>>>>> refs/remotes/origin/master
{
	return __dev_mc_add(dev, addr, true);
}
EXPORT_SYMBOL(dev_mc_add_global);

<<<<<<< HEAD
static int __dev_mc_del(struct net_device *dev, unsigned char *addr,
=======
static int __dev_mc_del(struct net_device *dev, const unsigned char *addr,
>>>>>>> refs/remotes/origin/master
			bool global)
{
	int err;

	netif_addr_lock_bh(dev);
	err = __hw_addr_del_ex(&dev->mc, addr, dev->addr_len,
<<<<<<< HEAD
			       NETDEV_HW_ADDR_T_MULTICAST, global);
=======
			       NETDEV_HW_ADDR_T_MULTICAST, global, false);
>>>>>>> refs/remotes/origin/master
	if (!err)
		__dev_set_rx_mode(dev);
	netif_addr_unlock_bh(dev);
	return err;
}

/**
 *	dev_mc_del - Delete a multicast address.
 *	@dev: device
 *	@addr: address to delete
 *
 *	Release reference to a multicast address and remove it
 *	from the device if the reference count drops to zero.
 */
<<<<<<< HEAD
int dev_mc_del(struct net_device *dev, unsigned char *addr)
=======
int dev_mc_del(struct net_device *dev, const unsigned char *addr)
>>>>>>> refs/remotes/origin/master
{
	return __dev_mc_del(dev, addr, false);
}
EXPORT_SYMBOL(dev_mc_del);

/**
 *	dev_mc_del_global - Delete a global multicast address.
 *	@dev: device
 *	@addr: address to delete
 *
 *	Release reference to a multicast address and remove it
 *	from the device if the reference count drops to zero.
 */
<<<<<<< HEAD
int dev_mc_del_global(struct net_device *dev, unsigned char *addr)
=======
int dev_mc_del_global(struct net_device *dev, const unsigned char *addr)
>>>>>>> refs/remotes/origin/master
{
	return __dev_mc_del(dev, addr, true);
}
EXPORT_SYMBOL(dev_mc_del_global);

/**
<<<<<<< HEAD
 *	dev_mc_sync - Synchronize device's unicast list to another device
=======
 *	dev_mc_sync - Synchronize device's multicast list to another device
>>>>>>> refs/remotes/origin/master
 *	@to: destination device
 *	@from: source device
 *
 *	Add newly added addresses to the destination device and release
 *	addresses that have no users left. The source device must be
<<<<<<< HEAD
<<<<<<< HEAD
 *	locked by netif_tx_lock_bh.
 *
 *	This function is intended to be called from the dev->set_multicast_list
 *	or dev->set_rx_mode function of layered software devices.
=======
=======
>>>>>>> refs/remotes/origin/master
 *	locked by netif_addr_lock_bh.
 *
 *	This function is intended to be called from the ndo_set_rx_mode
 *	function of layered software devices.
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 */
int dev_mc_sync(struct net_device *to, struct net_device *from)
{
	int err = 0;

	if (to->addr_len != from->addr_len)
		return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
	netif_addr_lock_bh(to);
	err = __hw_addr_sync(&to->mc, &from->mc, to->addr_len);
	if (!err)
		__dev_set_rx_mode(to);
	netif_addr_unlock_bh(to);
=======
	netif_addr_lock_nested(to);
	err = __hw_addr_sync(&to->mc, &from->mc, to->addr_len);
	if (!err)
		__dev_set_rx_mode(to);
	netif_addr_unlock(to);
>>>>>>> refs/remotes/origin/cm-10.0
	return err;
}
EXPORT_SYMBOL(dev_mc_sync);
=======
	netif_addr_lock_nested(to);
	err = __hw_addr_sync(&to->mc, &from->mc, to->addr_len);
	if (!err)
		__dev_set_rx_mode(to);
	netif_addr_unlock(to);
	return err;
}
EXPORT_SYMBOL(dev_mc_sync);

/**
 *	dev_mc_sync_multiple - Synchronize device's multicast list to another
 *	device, but allow for multiple calls to sync to multiple devices.
 *	@to: destination device
 *	@from: source device
 *
 *	Add newly added addresses to the destination device and release
 *	addresses that have no users left. The source device must be
 *	locked by netif_addr_lock_bh.
 *
 *	This function is intended to be called from the ndo_set_rx_mode
 *	function of layered software devices.  It allows for a single
 *	source device to be synced to multiple destination devices.
 */
int dev_mc_sync_multiple(struct net_device *to, struct net_device *from)
{
	int err = 0;

	if (to->addr_len != from->addr_len)
		return -EINVAL;

	netif_addr_lock_nested(to);
	err = __hw_addr_sync_multiple(&to->mc, &from->mc, to->addr_len);
	if (!err)
		__dev_set_rx_mode(to);
	netif_addr_unlock(to);
	return err;
}
EXPORT_SYMBOL(dev_mc_sync_multiple);
>>>>>>> refs/remotes/origin/master

/**
 *	dev_mc_unsync - Remove synchronized addresses from the destination device
 *	@to: destination device
 *	@from: source device
 *
 *	Remove all addresses that were added to the destination device by
 *	dev_mc_sync(). This function is intended to be called from the
 *	dev->stop function of layered software devices.
 */
void dev_mc_unsync(struct net_device *to, struct net_device *from)
{
	if (to->addr_len != from->addr_len)
		return;

	netif_addr_lock_bh(from);
<<<<<<< HEAD
<<<<<<< HEAD
	netif_addr_lock(to);
=======
	netif_addr_lock_nested(to);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	netif_addr_lock_nested(to);
>>>>>>> refs/remotes/origin/master
	__hw_addr_unsync(&to->mc, &from->mc, to->addr_len);
	__dev_set_rx_mode(to);
	netif_addr_unlock(to);
	netif_addr_unlock_bh(from);
}
EXPORT_SYMBOL(dev_mc_unsync);

/**
 *	dev_mc_flush - Flush multicast addresses
 *	@dev: device
 *
 *	Flush multicast addresses.
 */
void dev_mc_flush(struct net_device *dev)
{
	netif_addr_lock_bh(dev);
	__hw_addr_flush(&dev->mc);
	netif_addr_unlock_bh(dev);
}
EXPORT_SYMBOL(dev_mc_flush);

/**
 *	dev_mc_flush - Init multicast address list
 *	@dev: device
 *
 *	Init multicast address list.
 */
void dev_mc_init(struct net_device *dev)
{
	__hw_addr_init(&dev->mc);
}
EXPORT_SYMBOL(dev_mc_init);
<<<<<<< HEAD

#ifdef CONFIG_PROC_FS
#include <linux/seq_file.h>

static int dev_mc_seq_show(struct seq_file *seq, void *v)
{
	struct netdev_hw_addr *ha;
	struct net_device *dev = v;

	if (v == SEQ_START_TOKEN)
		return 0;

	netif_addr_lock_bh(dev);
	netdev_for_each_mc_addr(ha, dev) {
		int i;

		seq_printf(seq, "%-4d %-15s %-5d %-5d ", dev->ifindex,
			   dev->name, ha->refcount, ha->global_use);

		for (i = 0; i < dev->addr_len; i++)
			seq_printf(seq, "%02x", ha->addr[i]);

		seq_putc(seq, '\n');
	}
	netif_addr_unlock_bh(dev);
	return 0;
}

static const struct seq_operations dev_mc_seq_ops = {
	.start = dev_seq_start,
	.next  = dev_seq_next,
	.stop  = dev_seq_stop,
	.show  = dev_mc_seq_show,
};

static int dev_mc_seq_open(struct inode *inode, struct file *file)
{
	return seq_open_net(inode, file, &dev_mc_seq_ops,
			    sizeof(struct seq_net_private));
}

static const struct file_operations dev_mc_seq_fops = {
	.owner	 = THIS_MODULE,
	.open    = dev_mc_seq_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release_net,
};

#endif

static int __net_init dev_mc_net_init(struct net *net)
{
	if (!proc_net_fops_create(net, "dev_mcast", 0, &dev_mc_seq_fops))
		return -ENOMEM;
	return 0;
}

static void __net_exit dev_mc_net_exit(struct net *net)
{
	proc_net_remove(net, "dev_mcast");
}

static struct pernet_operations __net_initdata dev_mc_net_ops = {
	.init = dev_mc_net_init,
	.exit = dev_mc_net_exit,
};

void __init dev_mcast_init(void)
{
	register_pernet_subsys(&dev_mc_net_ops);
}

=======
>>>>>>> refs/remotes/origin/master
