<<<<<<< HEAD
/*
<<<<<<< HEAD
 * Copyright (C) 2007-2011 B.A.T.M.A.N. contributors:
=======
 * Copyright (C) 2007-2012 B.A.T.M.A.N. contributors:
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * Marek Lindner, Simon Wunderlich
=======
/* Copyright (C) 2007-2013 B.A.T.M.A.N. contributors:
 *
 * Marek Lindner, Simon Wunderlich, Antonio Quartulli
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
<<<<<<< HEAD
 *
=======
>>>>>>> refs/remotes/origin/master
 */

#include "main.h"
#include "translation-table.h"
#include "soft-interface.h"
#include "hard-interface.h"
<<<<<<< HEAD
<<<<<<< HEAD
#include "hash.h"
#include "originator.h"

static void tt_local_purge(struct work_struct *work);
static void _tt_global_del_orig(struct bat_priv *bat_priv,
				 struct tt_global_entry *tt_global_entry,
				 char *message);

/* returns 1 if they are the same mac addr */
static int compare_ltt(struct hlist_node *node, void *data2)
{
	void *data1 = container_of(node, struct tt_local_entry, hash_entry);

	return (memcmp(data1, data2, ETH_ALEN) == 0 ? 1 : 0);
}

/* returns 1 if they are the same mac addr */
static int compare_gtt(struct hlist_node *node, void *data2)
{
	void *data1 = container_of(node, struct tt_global_entry, hash_entry);
=======
=======
>>>>>>> refs/remotes/origin/master
#include "send.h"
#include "hash.h"
#include "originator.h"
#include "routing.h"
<<<<<<< HEAD

#include <linux/crc16.h>

static void _tt_global_del(struct bat_priv *bat_priv,
			   struct tt_global_entry *tt_global_entry,
			   const char *message);
static void tt_purge(struct work_struct *work);

/* returns 1 if they are the same mac addr */
static int compare_tt(const struct hlist_node *node, const void *data2)
{
	const void *data1 = container_of(node, struct tt_common_entry,
					 hash_entry);
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "bridge_loop_avoidance.h"

#include <linux/crc32c.h>

/* hash class keys */
static struct lock_class_key batadv_tt_local_hash_lock_class_key;
static struct lock_class_key batadv_tt_global_hash_lock_class_key;

static void batadv_send_roam_adv(struct batadv_priv *bat_priv, uint8_t *client,
				 unsigned short vid,
				 struct batadv_orig_node *orig_node);
static void batadv_tt_purge(struct work_struct *work);
static void
batadv_tt_global_del_orig_list(struct batadv_tt_global_entry *tt_global_entry);
static void batadv_tt_global_del(struct batadv_priv *bat_priv,
				 struct batadv_orig_node *orig_node,
				 const unsigned char *addr,
				 unsigned short vid, const char *message,
				 bool roaming);

/* returns 1 if they are the same mac addr */
static int batadv_compare_tt(const struct hlist_node *node, const void *data2)
{
	const void *data1 = container_of(node, struct batadv_tt_common_entry,
					 hash_entry);
>>>>>>> refs/remotes/origin/master

	return (memcmp(data1, data2, ETH_ALEN) == 0 ? 1 : 0);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void tt_local_start_timer(struct bat_priv *bat_priv)
{
	INIT_DELAYED_WORK(&bat_priv->tt_work, tt_local_purge);
	queue_delayed_work(bat_event_workqueue, &bat_priv->tt_work, 10 * HZ);
}

static struct tt_local_entry *tt_local_hash_find(struct bat_priv *bat_priv,
						   void *data)
{
	struct hashtable_t *hash = bat_priv->tt_local_hash;
	struct hlist_head *head;
	struct hlist_node *node;
	struct tt_local_entry *tt_local_entry, *tt_local_entry_tmp = NULL;
	int index;
=======
static void tt_start_timer(struct bat_priv *bat_priv)
{
	INIT_DELAYED_WORK(&bat_priv->tt_work, tt_purge);
	queue_delayed_work(bat_event_workqueue, &bat_priv->tt_work,
			   msecs_to_jiffies(5000));
}

static struct tt_common_entry *tt_hash_find(struct hashtable_t *hash,
					    const void *data)
{
	struct hlist_head *head;
	struct hlist_node *node;
	struct tt_common_entry *tt_common_entry, *tt_common_entry_tmp = NULL;
	uint32_t index;
>>>>>>> refs/remotes/origin/cm-10.0
=======
/**
 * batadv_choose_tt - return the index of the tt entry in the hash table
 * @data: pointer to the tt_common_entry object to map
 * @size: the size of the hash table
 *
 * Returns the hash index where the object represented by 'data' should be
 * stored at.
 */
static inline uint32_t batadv_choose_tt(const void *data, uint32_t size)
{
	struct batadv_tt_common_entry *tt;
	uint32_t hash = 0;

	tt = (struct batadv_tt_common_entry *)data;
	hash = batadv_hash_bytes(hash, &tt->addr, ETH_ALEN);
	hash = batadv_hash_bytes(hash, &tt->vid, sizeof(tt->vid));

	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);

	return hash % size;
}

/**
 * batadv_tt_hash_find - look for a client in the given hash table
 * @hash: the hash table to search
 * @addr: the mac address of the client to look for
 * @vid: VLAN identifier
 *
 * Returns a pointer to the tt_common struct belonging to the searched client if
 * found, NULL otherwise.
 */
static struct batadv_tt_common_entry *
batadv_tt_hash_find(struct batadv_hashtable *hash, const uint8_t *addr,
		    unsigned short vid)
{
	struct hlist_head *head;
	struct batadv_tt_common_entry to_search, *tt, *tt_tmp = NULL;
	uint32_t index;
>>>>>>> refs/remotes/origin/master

	if (!hash)
		return NULL;

<<<<<<< HEAD
	index = choose_orig(data, hash->size);
	head = &hash->table[index];

	rcu_read_lock();
<<<<<<< HEAD
	hlist_for_each_entry_rcu(tt_local_entry, node, head, hash_entry) {
		if (!compare_eth(tt_local_entry, data))
			continue;

		tt_local_entry_tmp = tt_local_entry;
=======
	hlist_for_each_entry_rcu(tt_common_entry, node, head, hash_entry) {
		if (!compare_eth(tt_common_entry, data))
			continue;

		if (!atomic_inc_not_zero(&tt_common_entry->refcount))
			continue;

		tt_common_entry_tmp = tt_common_entry;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	memcpy(to_search.addr, addr, ETH_ALEN);
	to_search.vid = vid;

	index = batadv_choose_tt(&to_search, hash->size);
	head = &hash->table[index];

	rcu_read_lock();
	hlist_for_each_entry_rcu(tt, head, hash_entry) {
		if (!batadv_compare_eth(tt, addr))
			continue;

		if (tt->vid != vid)
			continue;

		if (!atomic_inc_not_zero(&tt->refcount))
			continue;

		tt_tmp = tt;
>>>>>>> refs/remotes/origin/master
		break;
	}
	rcu_read_unlock();

<<<<<<< HEAD
<<<<<<< HEAD
	return tt_local_entry_tmp;
}

static struct tt_global_entry *tt_global_hash_find(struct bat_priv *bat_priv,
						     void *data)
{
	struct hashtable_t *hash = bat_priv->tt_global_hash;
	struct hlist_head *head;
	struct hlist_node *node;
	struct tt_global_entry *tt_global_entry;
	struct tt_global_entry *tt_global_entry_tmp = NULL;
	int index;

	if (!hash)
		return NULL;

	index = choose_orig(data, hash->size);
	head = &hash->table[index];

	rcu_read_lock();
	hlist_for_each_entry_rcu(tt_global_entry, node, head, hash_entry) {
		if (!compare_eth(tt_global_entry, data))
			continue;

		tt_global_entry_tmp = tt_global_entry;
		break;
	}
	rcu_read_unlock();

	return tt_global_entry_tmp;
}

int tt_local_init(struct bat_priv *bat_priv)
=======
	return tt_common_entry_tmp;
}

static struct tt_local_entry *tt_local_hash_find(struct bat_priv *bat_priv,
						 const void *data)
{
	struct tt_common_entry *tt_common_entry;
	struct tt_local_entry *tt_local_entry = NULL;

	tt_common_entry = tt_hash_find(bat_priv->tt_local_hash, data);
	if (tt_common_entry)
		tt_local_entry = container_of(tt_common_entry,
					      struct tt_local_entry, common);
	return tt_local_entry;
}

static struct tt_global_entry *tt_global_hash_find(struct bat_priv *bat_priv,
						   const void *data)
{
	struct tt_common_entry *tt_common_entry;
	struct tt_global_entry *tt_global_entry = NULL;

	tt_common_entry = tt_hash_find(bat_priv->tt_global_hash, data);
	if (tt_common_entry)
		tt_global_entry = container_of(tt_common_entry,
					       struct tt_global_entry, common);
	return tt_global_entry;

}

static void tt_local_entry_free_ref(struct tt_local_entry *tt_local_entry)
{
	if (atomic_dec_and_test(&tt_local_entry->common.refcount))
		kfree_rcu(tt_local_entry, common.rcu);
}

static void tt_global_entry_free_rcu(struct rcu_head *rcu)
{
	struct tt_common_entry *tt_common_entry;
	struct tt_global_entry *tt_global_entry;

	tt_common_entry = container_of(rcu, struct tt_common_entry, rcu);
	tt_global_entry = container_of(tt_common_entry, struct tt_global_entry,
				       common);

	if (tt_global_entry->orig_node)
		orig_node_free_ref(tt_global_entry->orig_node);

	kfree(tt_global_entry);
}

static void tt_global_entry_free_ref(struct tt_global_entry *tt_global_entry)
{
	if (atomic_dec_and_test(&tt_global_entry->common.refcount))
		call_rcu(&tt_global_entry->common.rcu,
			 tt_global_entry_free_rcu);
}

static void tt_local_event(struct bat_priv *bat_priv, const uint8_t *addr,
			   uint8_t flags)
{
	struct tt_change_node *tt_change_node;

	tt_change_node = kmalloc(sizeof(*tt_change_node), GFP_ATOMIC);

=======
	return tt_tmp;
}

/**
 * batadv_tt_local_hash_find - search the local table for a given client
 * @bat_priv: the bat priv with all the soft interface information
 * @addr: the mac address of the client to look for
 * @vid: VLAN identifier
 *
 * Returns a pointer to the corresponding tt_local_entry struct if the client is
 * found, NULL otherwise.
 */
static struct batadv_tt_local_entry *
batadv_tt_local_hash_find(struct batadv_priv *bat_priv, const uint8_t *addr,
			  unsigned short vid)
{
	struct batadv_tt_common_entry *tt_common_entry;
	struct batadv_tt_local_entry *tt_local_entry = NULL;

	tt_common_entry = batadv_tt_hash_find(bat_priv->tt.local_hash, addr,
					      vid);
	if (tt_common_entry)
		tt_local_entry = container_of(tt_common_entry,
					      struct batadv_tt_local_entry,
					      common);
	return tt_local_entry;
}

/**
 * batadv_tt_global_hash_find - search the global table for a given client
 * @bat_priv: the bat priv with all the soft interface information
 * @addr: the mac address of the client to look for
 * @vid: VLAN identifier
 *
 * Returns a pointer to the corresponding tt_global_entry struct if the client
 * is found, NULL otherwise.
 */
static struct batadv_tt_global_entry *
batadv_tt_global_hash_find(struct batadv_priv *bat_priv, const uint8_t *addr,
			   unsigned short vid)
{
	struct batadv_tt_common_entry *tt_common_entry;
	struct batadv_tt_global_entry *tt_global_entry = NULL;

	tt_common_entry = batadv_tt_hash_find(bat_priv->tt.global_hash, addr,
					      vid);
	if (tt_common_entry)
		tt_global_entry = container_of(tt_common_entry,
					       struct batadv_tt_global_entry,
					       common);
	return tt_global_entry;
}

static void
batadv_tt_local_entry_free_ref(struct batadv_tt_local_entry *tt_local_entry)
{
	if (atomic_dec_and_test(&tt_local_entry->common.refcount))
		kfree_rcu(tt_local_entry, common.rcu);
}

/**
 * batadv_tt_global_entry_free_ref - decrement the refcounter for a
 *  tt_global_entry and possibly free it
 * @tt_global_entry: the object to free
 */
static void
batadv_tt_global_entry_free_ref(struct batadv_tt_global_entry *tt_global_entry)
{
	if (atomic_dec_and_test(&tt_global_entry->common.refcount)) {
		batadv_tt_global_del_orig_list(tt_global_entry);
		kfree_rcu(tt_global_entry, common.rcu);
	}
}

static void batadv_tt_orig_list_entry_free_rcu(struct rcu_head *rcu)
{
	struct batadv_tt_orig_list_entry *orig_entry;

	orig_entry = container_of(rcu, struct batadv_tt_orig_list_entry, rcu);

	/* We are in an rcu callback here, therefore we cannot use
	 * batadv_orig_node_free_ref() and its call_rcu():
	 * An rcu_barrier() wouldn't wait for that to finish
	 */
	batadv_orig_node_free_ref_now(orig_entry->orig_node);
	kfree(orig_entry);
}

/**
 * batadv_tt_local_size_mod - change the size by v of the local table identified
 *  by vid
 * @bat_priv: the bat priv with all the soft interface information
 * @vid: the VLAN identifier of the sub-table to change
 * @v: the amount to sum to the local table size
 */
static void batadv_tt_local_size_mod(struct batadv_priv *bat_priv,
				     unsigned short vid, int v)
{
	struct batadv_softif_vlan *vlan;

	vlan = batadv_softif_vlan_get(bat_priv, vid);
	if (!vlan)
		return;

	atomic_add(v, &vlan->tt.num_entries);

	batadv_softif_vlan_free_ref(vlan);
}

/**
 * batadv_tt_local_size_inc - increase by one the local table size for the given
 *  vid
 * @bat_priv: the bat priv with all the soft interface information
 * @vid: the VLAN identifier
 */
static void batadv_tt_local_size_inc(struct batadv_priv *bat_priv,
				     unsigned short vid)
{
	batadv_tt_local_size_mod(bat_priv, vid, 1);
}

/**
 * batadv_tt_local_size_dec - decrease by one the local table size for the given
 *  vid
 * @bat_priv: the bat priv with all the soft interface information
 * @vid: the VLAN identifier
 */
static void batadv_tt_local_size_dec(struct batadv_priv *bat_priv,
				     unsigned short vid)
{
	batadv_tt_local_size_mod(bat_priv, vid, -1);
}

/**
 * batadv_tt_global_size_mod - change the size by v of the local table
 *  identified by vid
 * @bat_priv: the bat priv with all the soft interface information
 * @vid: the VLAN identifier
 * @v: the amount to sum to the global table size
 */
static void batadv_tt_global_size_mod(struct batadv_orig_node *orig_node,
				      unsigned short vid, int v)
{
	struct batadv_orig_node_vlan *vlan;

	vlan = batadv_orig_node_vlan_new(orig_node, vid);
	if (!vlan)
		return;

	if (atomic_add_return(v, &vlan->tt.num_entries) == 0) {
		spin_lock_bh(&orig_node->vlan_list_lock);
		list_del_rcu(&vlan->list);
		spin_unlock_bh(&orig_node->vlan_list_lock);
		batadv_orig_node_vlan_free_ref(vlan);
	}

	batadv_orig_node_vlan_free_ref(vlan);
}

/**
 * batadv_tt_global_size_inc - increase by one the global table size for the
 *  given vid
 * @orig_node: the originator which global table size has to be decreased
 * @vid: the vlan identifier
 */
static void batadv_tt_global_size_inc(struct batadv_orig_node *orig_node,
				      unsigned short vid)
{
	batadv_tt_global_size_mod(orig_node, vid, 1);
}

/**
 * batadv_tt_global_size_dec - decrease by one the global table size for the
 *  given vid
 * @orig_node: the originator which global table size has to be decreased
 * @vid: the vlan identifier
 */
static void batadv_tt_global_size_dec(struct batadv_orig_node *orig_node,
				      unsigned short vid)
{
	batadv_tt_global_size_mod(orig_node, vid, -1);
}

static void
batadv_tt_orig_list_entry_free_ref(struct batadv_tt_orig_list_entry *orig_entry)
{
	if (!atomic_dec_and_test(&orig_entry->refcount))
		return;

	call_rcu(&orig_entry->rcu, batadv_tt_orig_list_entry_free_rcu);
}

/**
 * batadv_tt_local_event - store a local TT event (ADD/DEL)
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_local_entry: the TT entry involved in the event
 * @event_flags: flags to store in the event structure
 */
static void batadv_tt_local_event(struct batadv_priv *bat_priv,
				  struct batadv_tt_local_entry *tt_local_entry,
				  uint8_t event_flags)
{
	struct batadv_tt_change_node *tt_change_node, *entry, *safe;
	struct batadv_tt_common_entry *common = &tt_local_entry->common;
	uint8_t flags = common->flags | event_flags;
	bool event_removed = false;
	bool del_op_requested, del_op_entry;

	tt_change_node = kmalloc(sizeof(*tt_change_node), GFP_ATOMIC);
>>>>>>> refs/remotes/origin/master
	if (!tt_change_node)
		return;

	tt_change_node->change.flags = flags;
<<<<<<< HEAD
	memcpy(tt_change_node->change.addr, addr, ETH_ALEN);

	spin_lock_bh(&bat_priv->tt_changes_list_lock);
	/* track the change in the OGMinterval list */
	list_add_tail(&tt_change_node->list, &bat_priv->tt_changes_list);
	atomic_inc(&bat_priv->tt_local_changes);
	spin_unlock_bh(&bat_priv->tt_changes_list_lock);

	atomic_set(&bat_priv->tt_ogm_append_cnt, 0);
}

int tt_len(int changes_num)
{
	return changes_num * sizeof(struct tt_change);
}

static int tt_local_init(struct bat_priv *bat_priv)
>>>>>>> refs/remotes/origin/cm-10.0
{
	if (bat_priv->tt_local_hash)
		return 1;

	bat_priv->tt_local_hash = hash_new(1024);

	if (!bat_priv->tt_local_hash)
		return 0;

<<<<<<< HEAD
	atomic_set(&bat_priv->tt_local_changed, 0);
	tt_local_start_timer(bat_priv);

	return 1;
}

void tt_local_add(struct net_device *soft_iface, uint8_t *addr)
{
	struct bat_priv *bat_priv = netdev_priv(soft_iface);
	struct tt_local_entry *tt_local_entry;
	struct tt_global_entry *tt_global_entry;
	int required_bytes;

	spin_lock_bh(&bat_priv->tt_lhash_lock);
	tt_local_entry = tt_local_hash_find(bat_priv, addr);
	spin_unlock_bh(&bat_priv->tt_lhash_lock);

	if (tt_local_entry) {
		tt_local_entry->last_seen = jiffies;
		return;
	}

	/* only announce as many hosts as possible in the batman-packet and
	   space in batman_packet->num_tt That also should give a limit to
	   MAC-flooding. */
	required_bytes = (bat_priv->num_local_tt + 1) * ETH_ALEN;
	required_bytes += BAT_PACKET_LEN;

	if ((required_bytes > ETH_DATA_LEN) ||
	    (atomic_read(&bat_priv->aggregated_ogms) &&
	     required_bytes > MAX_AGGREGATION_BYTES) ||
	    (bat_priv->num_local_tt + 1 > 255)) {
		bat_dbg(DBG_ROUTES, bat_priv,
			"Can't add new local tt entry (%pM): "
			"number of local tt entries exceeds packet size\n",
			addr);
		return;
	}

	bat_dbg(DBG_ROUTES, bat_priv,
		"Creating new local tt entry: %pM\n", addr);

	tt_local_entry = kmalloc(sizeof(struct tt_local_entry), GFP_ATOMIC);
	if (!tt_local_entry)
		return;

	memcpy(tt_local_entry->addr, addr, ETH_ALEN);
=======
	return 1;
}

void tt_local_add(struct net_device *soft_iface, const uint8_t *addr,
		  int ifindex)
{
	struct bat_priv *bat_priv = netdev_priv(soft_iface);
	struct tt_local_entry *tt_local_entry = NULL;
	struct tt_global_entry *tt_global_entry = NULL;
	int hash_added;

	tt_local_entry = tt_local_hash_find(bat_priv, addr);

	if (tt_local_entry) {
		tt_local_entry->last_seen = jiffies;
		goto out;
	}

	tt_local_entry = kmalloc(sizeof(*tt_local_entry), GFP_ATOMIC);
	if (!tt_local_entry)
		goto out;

	bat_dbg(DBG_TT, bat_priv,
		"Creating new local tt entry: %pM (ttvn: %d)\n", addr,
		(uint8_t)atomic_read(&bat_priv->ttvn));

	memcpy(tt_local_entry->common.addr, addr, ETH_ALEN);
	tt_local_entry->common.flags = NO_FLAGS;
	if (is_wifi_iface(ifindex))
		tt_local_entry->common.flags |= TT_CLIENT_WIFI;
	atomic_set(&tt_local_entry->common.refcount, 2);
>>>>>>> refs/remotes/origin/cm-10.0
	tt_local_entry->last_seen = jiffies;

	/* the batman interface mac address should never be purged */
	if (compare_eth(addr, soft_iface->dev_addr))
<<<<<<< HEAD
		tt_local_entry->never_purge = 1;
	else
		tt_local_entry->never_purge = 0;

	spin_lock_bh(&bat_priv->tt_lhash_lock);

	hash_add(bat_priv->tt_local_hash, compare_ltt, choose_orig,
		 tt_local_entry, &tt_local_entry->hash_entry);
	bat_priv->num_local_tt++;
	atomic_set(&bat_priv->tt_local_changed, 1);

	spin_unlock_bh(&bat_priv->tt_lhash_lock);

	/* remove address from global hash if present */
	spin_lock_bh(&bat_priv->tt_ghash_lock);

	tt_global_entry = tt_global_hash_find(bat_priv, addr);

	if (tt_global_entry)
		_tt_global_del_orig(bat_priv, tt_global_entry,
				     "local tt received");

	spin_unlock_bh(&bat_priv->tt_ghash_lock);
}

int tt_local_fill_buffer(struct bat_priv *bat_priv,
			  unsigned char *buff, int buff_len)
{
	struct hashtable_t *hash = bat_priv->tt_local_hash;
	struct tt_local_entry *tt_local_entry;
	struct hlist_node *node;
	struct hlist_head *head;
	int i, count = 0;

	spin_lock_bh(&bat_priv->tt_lhash_lock);

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];

		rcu_read_lock();
		hlist_for_each_entry_rcu(tt_local_entry, node,
					 head, hash_entry) {
			if (buff_len < (count + 1) * ETH_ALEN)
				break;

			memcpy(buff + (count * ETH_ALEN), tt_local_entry->addr,
			       ETH_ALEN);

			count++;
		}
		rcu_read_unlock();
	}

	/* if we did not get all new local tts see you next time  ;-) */
	if (count == bat_priv->num_local_tt)
		atomic_set(&bat_priv->tt_local_changed, 0);

	spin_unlock_bh(&bat_priv->tt_lhash_lock);
	return count;
=======
		tt_local_entry->common.flags |= TT_CLIENT_NOPURGE;

	/* The local entry has to be marked as NEW to avoid to send it in
	 * a full table response going out before the next ttvn increment
	 * (consistency check) */
	tt_local_entry->common.flags |= TT_CLIENT_NEW;

	hash_added = hash_add(bat_priv->tt_local_hash, compare_tt, choose_orig,
			 &tt_local_entry->common,
			 &tt_local_entry->common.hash_entry);

	if (unlikely(hash_added != 0)) {
		/* remove the reference for the hash */
		tt_local_entry_free_ref(tt_local_entry);
		goto out;
	}

	tt_local_event(bat_priv, addr, tt_local_entry->common.flags);

	/* remove address from global hash if present */
	tt_global_entry = tt_global_hash_find(bat_priv, addr);

	/* Check whether it is a roaming! */
	if (tt_global_entry) {
		/* This node is probably going to update its tt table */
		tt_global_entry->orig_node->tt_poss_change = true;
		/* The global entry has to be marked as ROAMING and has to be
		 * kept for consistency purpose */
		tt_global_entry->common.flags |= TT_CLIENT_ROAM;
		tt_global_entry->roam_at = jiffies;
		send_roam_adv(bat_priv, tt_global_entry->common.addr,
			      tt_global_entry->orig_node);
	}
out:
	if (tt_local_entry)
		tt_local_entry_free_ref(tt_local_entry);
	if (tt_global_entry)
		tt_global_entry_free_ref(tt_global_entry);
}

int tt_changes_fill_buffer(struct bat_priv *bat_priv,
			   unsigned char *buff, int buff_len)
{
	int count = 0, tot_changes = 0;
	struct tt_change_node *entry, *safe;

	if (buff_len > 0)
		tot_changes = buff_len / tt_len(1);

	spin_lock_bh(&bat_priv->tt_changes_list_lock);
	atomic_set(&bat_priv->tt_local_changes, 0);

	list_for_each_entry_safe(entry, safe, &bat_priv->tt_changes_list,
				 list) {
		if (count < tot_changes) {
			memcpy(buff + tt_len(count),
			       &entry->change, sizeof(struct tt_change));
			count++;
=======
	memset(tt_change_node->change.reserved, 0,
	       sizeof(tt_change_node->change.reserved));
	memcpy(tt_change_node->change.addr, common->addr, ETH_ALEN);
	tt_change_node->change.vid = htons(common->vid);

	del_op_requested = flags & BATADV_TT_CLIENT_DEL;

	/* check for ADD+DEL or DEL+ADD events */
	spin_lock_bh(&bat_priv->tt.changes_list_lock);
	list_for_each_entry_safe(entry, safe, &bat_priv->tt.changes_list,
				 list) {
		if (!batadv_compare_eth(entry->change.addr, common->addr))
			continue;

		/* DEL+ADD in the same orig interval have no effect and can be
		 * removed to avoid silly behaviour on the receiver side. The
		 * other way around (ADD+DEL) can happen in case of roaming of
		 * a client still in the NEW state. Roaming of NEW clients is
		 * now possible due to automatically recognition of "temporary"
		 * clients
		 */
		del_op_entry = entry->change.flags & BATADV_TT_CLIENT_DEL;
		if (!del_op_requested && del_op_entry)
			goto del;
		if (del_op_requested && !del_op_entry)
			goto del;

		/* this is a second add in the same originator interval. It
		 * means that flags have been changed: update them!
		 */
		if (!del_op_requested && !del_op_entry)
			entry->change.flags = flags;

		continue;
del:
		list_del(&entry->list);
		kfree(entry);
		kfree(tt_change_node);
		event_removed = true;
		goto unlock;
	}

	/* track the change in the OGMinterval list */
	list_add_tail(&tt_change_node->list, &bat_priv->tt.changes_list);

unlock:
	spin_unlock_bh(&bat_priv->tt.changes_list_lock);

	if (event_removed)
		atomic_dec(&bat_priv->tt.local_changes);
	else
		atomic_inc(&bat_priv->tt.local_changes);
}

/**
 * batadv_tt_len - compute length in bytes of given number of tt changes
 * @changes_num: number of tt changes
 *
 * Returns computed length in bytes.
 */
static int batadv_tt_len(int changes_num)
{
	return changes_num * sizeof(struct batadv_tvlv_tt_change);
}

/**
 * batadv_tt_entries - compute the number of entries fitting in tt_len bytes
 * @tt_len: available space
 *
 * Returns the number of entries.
 */
static uint16_t batadv_tt_entries(uint16_t tt_len)
{
	return tt_len / batadv_tt_len(1);
}

/**
 * batadv_tt_local_table_transmit_size - calculates the local translation table
 *  size when transmitted over the air
 * @bat_priv: the bat priv with all the soft interface information
 *
 * Returns local translation table size in bytes.
 */
static int batadv_tt_local_table_transmit_size(struct batadv_priv *bat_priv)
{
	uint16_t num_vlan = 0, tt_local_entries = 0;
	struct batadv_softif_vlan *vlan;
	int hdr_size;

	rcu_read_lock();
	hlist_for_each_entry_rcu(vlan, &bat_priv->softif_vlan_list, list) {
		num_vlan++;
		tt_local_entries += atomic_read(&vlan->tt.num_entries);
	}
	rcu_read_unlock();

	/* header size of tvlv encapsulated tt response payload */
	hdr_size = sizeof(struct batadv_unicast_tvlv_packet);
	hdr_size += sizeof(struct batadv_tvlv_hdr);
	hdr_size += sizeof(struct batadv_tvlv_tt_data);
	hdr_size += num_vlan * sizeof(struct batadv_tvlv_tt_vlan_data);

	return hdr_size + batadv_tt_len(tt_local_entries);
}

static int batadv_tt_local_init(struct batadv_priv *bat_priv)
{
	if (bat_priv->tt.local_hash)
		return 0;

	bat_priv->tt.local_hash = batadv_hash_new(1024);

	if (!bat_priv->tt.local_hash)
		return -ENOMEM;

	batadv_hash_set_lock_class(bat_priv->tt.local_hash,
				   &batadv_tt_local_hash_lock_class_key);

	return 0;
}

static void batadv_tt_global_free(struct batadv_priv *bat_priv,
				  struct batadv_tt_global_entry *tt_global,
				  const char *message)
{
	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Deleting global tt entry %pM (vid: %d): %s\n",
		   tt_global->common.addr,
		   BATADV_PRINT_VID(tt_global->common.vid), message);

	batadv_hash_remove(bat_priv->tt.global_hash, batadv_compare_tt,
			   batadv_choose_tt, &tt_global->common);
	batadv_tt_global_entry_free_ref(tt_global);
}

/**
 * batadv_tt_local_add - add a new client to the local table or update an
 *  existing client
 * @soft_iface: netdev struct of the mesh interface
 * @addr: the mac address of the client to add
 * @vid: VLAN identifier
 * @ifindex: index of the interface where the client is connected to (useful to
 *  identify wireless clients)
 *
 * Returns true if the client was successfully added, false otherwise.
 */
bool batadv_tt_local_add(struct net_device *soft_iface, const uint8_t *addr,
			 unsigned short vid, int ifindex)
{
	struct batadv_priv *bat_priv = netdev_priv(soft_iface);
	struct batadv_tt_local_entry *tt_local;
	struct batadv_tt_global_entry *tt_global;
	struct net_device *in_dev = NULL;
	struct hlist_head *head;
	struct batadv_tt_orig_list_entry *orig_entry;
	int hash_added, table_size, packet_size_max;
	bool ret = false, roamed_back = false;
	uint8_t remote_flags;

	if (ifindex != BATADV_NULL_IFINDEX)
		in_dev = dev_get_by_index(&init_net, ifindex);

	tt_local = batadv_tt_local_hash_find(bat_priv, addr, vid);
	tt_global = batadv_tt_global_hash_find(bat_priv, addr, vid);

	if (tt_local) {
		tt_local->last_seen = jiffies;
		if (tt_local->common.flags & BATADV_TT_CLIENT_PENDING) {
			batadv_dbg(BATADV_DBG_TT, bat_priv,
				   "Re-adding pending client %pM (vid: %d)\n",
				   addr, BATADV_PRINT_VID(vid));
			/* whatever the reason why the PENDING flag was set,
			 * this is a client which was enqueued to be removed in
			 * this orig_interval. Since it popped up again, the
			 * flag can be reset like it was never enqueued
			 */
			tt_local->common.flags &= ~BATADV_TT_CLIENT_PENDING;
			goto add_event;
		}

		if (tt_local->common.flags & BATADV_TT_CLIENT_ROAM) {
			batadv_dbg(BATADV_DBG_TT, bat_priv,
				   "Roaming client %pM (vid: %d) came back to its original location\n",
				   addr, BATADV_PRINT_VID(vid));
			/* the ROAM flag is set because this client roamed away
			 * and the node got a roaming_advertisement message. Now
			 * that the client popped up again at its original
			 * location such flag can be unset
			 */
			tt_local->common.flags &= ~BATADV_TT_CLIENT_ROAM;
			roamed_back = true;
		}
		goto check_roaming;
	}

	/* Ignore the client if we cannot send it in a full table response. */
	table_size = batadv_tt_local_table_transmit_size(bat_priv);
	table_size += batadv_tt_len(1);
	packet_size_max = atomic_read(&bat_priv->packet_size_max);
	if (table_size > packet_size_max) {
		net_ratelimited_function(batadv_info, soft_iface,
					 "Local translation table size (%i) exceeds maximum packet size (%i); Ignoring new local tt entry: %pM\n",
					 table_size, packet_size_max, addr);
		goto out;
	}

	tt_local = kmalloc(sizeof(*tt_local), GFP_ATOMIC);
	if (!tt_local)
		goto out;

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Creating new local tt entry: %pM (vid: %d, ttvn: %d)\n",
		   addr, BATADV_PRINT_VID(vid),
		   (uint8_t)atomic_read(&bat_priv->tt.vn));

	memcpy(tt_local->common.addr, addr, ETH_ALEN);
	/* The local entry has to be marked as NEW to avoid to send it in
	 * a full table response going out before the next ttvn increment
	 * (consistency check)
	 */
	tt_local->common.flags = BATADV_TT_CLIENT_NEW;
	tt_local->common.vid = vid;
	if (batadv_is_wifi_netdev(in_dev))
		tt_local->common.flags |= BATADV_TT_CLIENT_WIFI;
	atomic_set(&tt_local->common.refcount, 2);
	tt_local->last_seen = jiffies;
	tt_local->common.added_at = tt_local->last_seen;

	/* the batman interface mac address should never be purged */
	if (batadv_compare_eth(addr, soft_iface->dev_addr))
		tt_local->common.flags |= BATADV_TT_CLIENT_NOPURGE;

	hash_added = batadv_hash_add(bat_priv->tt.local_hash, batadv_compare_tt,
				     batadv_choose_tt, &tt_local->common,
				     &tt_local->common.hash_entry);

	if (unlikely(hash_added != 0)) {
		/* remove the reference for the hash */
		batadv_tt_local_entry_free_ref(tt_local);
		goto out;
	}

add_event:
	batadv_tt_local_event(bat_priv, tt_local, BATADV_NO_FLAGS);

check_roaming:
	/* Check whether it is a roaming, but don't do anything if the roaming
	 * process has already been handled
	 */
	if (tt_global && !(tt_global->common.flags & BATADV_TT_CLIENT_ROAM)) {
		/* These node are probably going to update their tt table */
		head = &tt_global->orig_list;
		rcu_read_lock();
		hlist_for_each_entry_rcu(orig_entry, head, list) {
			batadv_send_roam_adv(bat_priv, tt_global->common.addr,
					     tt_global->common.vid,
					     orig_entry->orig_node);
		}
		rcu_read_unlock();
		if (roamed_back) {
			batadv_tt_global_free(bat_priv, tt_global,
					      "Roaming canceled");
			tt_global = NULL;
		} else {
			/* The global entry has to be marked as ROAMING and
			 * has to be kept for consistency purpose
			 */
			tt_global->common.flags |= BATADV_TT_CLIENT_ROAM;
			tt_global->roam_at = jiffies;
		}
	}

	/* store the current remote flags before altering them. This helps
	 * understanding is flags are changing or not
	 */
	remote_flags = tt_local->common.flags & BATADV_TT_REMOTE_MASK;

	if (batadv_is_wifi_netdev(in_dev))
		tt_local->common.flags |= BATADV_TT_CLIENT_WIFI;
	else
		tt_local->common.flags &= ~BATADV_TT_CLIENT_WIFI;

	/* if any "dynamic" flag has been modified, resend an ADD event for this
	 * entry so that all the nodes can get the new flags
	 */
	if (remote_flags ^ (tt_local->common.flags & BATADV_TT_REMOTE_MASK))
		batadv_tt_local_event(bat_priv, tt_local, BATADV_NO_FLAGS);

	ret = true;
out:
	if (in_dev)
		dev_put(in_dev);
	if (tt_local)
		batadv_tt_local_entry_free_ref(tt_local);
	if (tt_global)
		batadv_tt_global_entry_free_ref(tt_global);
	return ret;
}

/**
 * batadv_tt_prepare_tvlv_global_data - prepare the TVLV TT header to send
 *  within a TT Response directed to another node
 * @orig_node: originator for which the TT data has to be prepared
 * @tt_data: uninitialised pointer to the address of the TVLV buffer
 * @tt_change: uninitialised pointer to the address of the area where the TT
 *  changed can be stored
 * @tt_len: pointer to the length to reserve to the tt_change. if -1 this
 *  function reserves the amount of space needed to send the entire global TT
 *  table. In case of success the value is updated with the real amount of
 *  reserved bytes

 * Allocate the needed amount of memory for the entire TT TVLV and write its
 * header made up by one tvlv_tt_data object and a series of tvlv_tt_vlan_data
 * objects, one per active VLAN served by the originator node.
 *
 * Return the size of the allocated buffer or 0 in case of failure.
 */
static uint16_t
batadv_tt_prepare_tvlv_global_data(struct batadv_orig_node *orig_node,
				   struct batadv_tvlv_tt_data **tt_data,
				   struct batadv_tvlv_tt_change **tt_change,
				   int32_t *tt_len)
{
	uint16_t num_vlan = 0, num_entries = 0, change_offset, tvlv_len;
	struct batadv_tvlv_tt_vlan_data *tt_vlan;
	struct batadv_orig_node_vlan *vlan;
	uint8_t *tt_change_ptr;

	rcu_read_lock();
	list_for_each_entry_rcu(vlan, &orig_node->vlan_list, list) {
		num_vlan++;
		num_entries += atomic_read(&vlan->tt.num_entries);
	}

	change_offset = sizeof(**tt_data);
	change_offset += num_vlan * sizeof(*tt_vlan);

	/* if tt_len is negative, allocate the space needed by the full table */
	if (*tt_len < 0)
		*tt_len = batadv_tt_len(num_entries);

	tvlv_len = *tt_len;
	tvlv_len += change_offset;

	*tt_data = kmalloc(tvlv_len, GFP_ATOMIC);
	if (!*tt_data) {
		*tt_len = 0;
		goto out;
	}

	(*tt_data)->flags = BATADV_NO_FLAGS;
	(*tt_data)->ttvn = atomic_read(&orig_node->last_ttvn);
	(*tt_data)->num_vlan = htons(num_vlan);

	tt_vlan = (struct batadv_tvlv_tt_vlan_data *)(*tt_data + 1);
	list_for_each_entry_rcu(vlan, &orig_node->vlan_list, list) {
		tt_vlan->vid = htons(vlan->vid);
		tt_vlan->crc = htonl(vlan->tt.crc);

		tt_vlan++;
	}

	tt_change_ptr = (uint8_t *)*tt_data + change_offset;
	*tt_change = (struct batadv_tvlv_tt_change *)tt_change_ptr;

out:
	rcu_read_unlock();
	return tvlv_len;
}

/**
 * batadv_tt_prepare_tvlv_local_data - allocate and prepare the TT TVLV for this
 *  node
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_data: uninitialised pointer to the address of the TVLV buffer
 * @tt_change: uninitialised pointer to the address of the area where the TT
 *  changes can be stored
 * @tt_len: pointer to the length to reserve to the tt_change. if -1 this
 *  function reserves the amount of space needed to send the entire local TT
 *  table. In case of success the value is updated with the real amount of
 *  reserved bytes
 *
 * Allocate the needed amount of memory for the entire TT TVLV and write its
 * header made up by one tvlv_tt_data object and a series of tvlv_tt_vlan_data
 * objects, one per active VLAN.
 *
 * Return the size of the allocated buffer or 0 in case of failure.
 */
static uint16_t
batadv_tt_prepare_tvlv_local_data(struct batadv_priv *bat_priv,
				  struct batadv_tvlv_tt_data **tt_data,
				  struct batadv_tvlv_tt_change **tt_change,
				  int32_t *tt_len)
{
	struct batadv_tvlv_tt_vlan_data *tt_vlan;
	struct batadv_softif_vlan *vlan;
	uint16_t num_vlan = 0, num_entries = 0, tvlv_len;
	uint8_t *tt_change_ptr;
	int change_offset;

	rcu_read_lock();
	hlist_for_each_entry_rcu(vlan, &bat_priv->softif_vlan_list, list) {
		num_vlan++;
		num_entries += atomic_read(&vlan->tt.num_entries);
	}

	change_offset = sizeof(**tt_data);
	change_offset += num_vlan * sizeof(*tt_vlan);

	/* if tt_len is negative, allocate the space needed by the full table */
	if (*tt_len < 0)
		*tt_len = batadv_tt_len(num_entries);

	tvlv_len = *tt_len;
	tvlv_len += change_offset;

	*tt_data = kmalloc(tvlv_len, GFP_ATOMIC);
	if (!*tt_data) {
		tvlv_len = 0;
		goto out;
	}

	(*tt_data)->flags = BATADV_NO_FLAGS;
	(*tt_data)->ttvn = atomic_read(&bat_priv->tt.vn);
	(*tt_data)->num_vlan = htons(num_vlan);

	tt_vlan = (struct batadv_tvlv_tt_vlan_data *)(*tt_data + 1);
	hlist_for_each_entry_rcu(vlan, &bat_priv->softif_vlan_list, list) {
		tt_vlan->vid = htons(vlan->vid);
		tt_vlan->crc = htonl(vlan->tt.crc);

		tt_vlan++;
	}

	tt_change_ptr = (uint8_t *)*tt_data + change_offset;
	*tt_change = (struct batadv_tvlv_tt_change *)tt_change_ptr;

out:
	rcu_read_unlock();
	return tvlv_len;
}

/**
 * batadv_tt_tvlv_container_update - update the translation table tvlv container
 *  after local tt changes have been committed
 * @bat_priv: the bat priv with all the soft interface information
 */
static void batadv_tt_tvlv_container_update(struct batadv_priv *bat_priv)
{
	struct batadv_tt_change_node *entry, *safe;
	struct batadv_tvlv_tt_data *tt_data;
	struct batadv_tvlv_tt_change *tt_change;
	int tt_diff_len, tt_change_len = 0;
	int tt_diff_entries_num = 0, tt_diff_entries_count = 0;
	uint16_t tvlv_len;

	tt_diff_entries_num = atomic_read(&bat_priv->tt.local_changes);
	tt_diff_len = batadv_tt_len(tt_diff_entries_num);

	/* if we have too many changes for one packet don't send any
	 * and wait for the tt table request which will be fragmented
	 */
	if (tt_diff_len > bat_priv->soft_iface->mtu)
		tt_diff_len = 0;

	tvlv_len = batadv_tt_prepare_tvlv_local_data(bat_priv, &tt_data,
						     &tt_change, &tt_diff_len);
	if (!tvlv_len)
		return;

	tt_data->flags = BATADV_TT_OGM_DIFF;

	if (tt_diff_len == 0)
		goto container_register;

	spin_lock_bh(&bat_priv->tt.changes_list_lock);
	atomic_set(&bat_priv->tt.local_changes, 0);

	list_for_each_entry_safe(entry, safe, &bat_priv->tt.changes_list,
				 list) {
		if (tt_diff_entries_count < tt_diff_entries_num) {
			memcpy(tt_change + tt_diff_entries_count,
			       &entry->change,
			       sizeof(struct batadv_tvlv_tt_change));
			tt_diff_entries_count++;
>>>>>>> refs/remotes/origin/master
		}
		list_del(&entry->list);
		kfree(entry);
	}
<<<<<<< HEAD
	spin_unlock_bh(&bat_priv->tt_changes_list_lock);

	/* Keep the buffer for possible tt_request */
	spin_lock_bh(&bat_priv->tt_buff_lock);
	kfree(bat_priv->tt_buff);
	bat_priv->tt_buff_len = 0;
	bat_priv->tt_buff = NULL;
	/* We check whether this new OGM has no changes due to size
	 * problems */
	if (buff_len > 0) {
		/**
		 * if kmalloc() fails we will reply with the full table
		 * instead of providing the diff
		 */
		bat_priv->tt_buff = kmalloc(buff_len, GFP_ATOMIC);
		if (bat_priv->tt_buff) {
			memcpy(bat_priv->tt_buff, buff, buff_len);
			bat_priv->tt_buff_len = buff_len;
		}
	}
	spin_unlock_bh(&bat_priv->tt_buff_lock);

	return tot_changes;
>>>>>>> refs/remotes/origin/cm-10.0
}

int tt_local_seq_print_text(struct seq_file *seq, void *offset)
{
	struct net_device *net_dev = (struct net_device *)seq->private;
	struct bat_priv *bat_priv = netdev_priv(net_dev);
	struct hashtable_t *hash = bat_priv->tt_local_hash;
<<<<<<< HEAD
	struct tt_local_entry *tt_local_entry;
	struct hard_iface *primary_if;
	struct hlist_node *node;
	struct hlist_head *head;
	size_t buf_size, pos;
	char *buff;
	int i, ret = 0;

	primary_if = primary_if_get_selected(bat_priv);
	if (!primary_if) {
		ret = seq_printf(seq, "BATMAN mesh %s disabled - "
				 "please specify interfaces to enable it\n",
=======
	struct tt_common_entry *tt_common_entry;
	struct hard_iface *primary_if;
	struct hlist_node *node;
	struct hlist_head *head;
	uint32_t i;
	int ret = 0;

	primary_if = primary_if_get_selected(bat_priv);
	if (!primary_if) {
		ret = seq_printf(seq,
				 "BATMAN mesh %s disabled - please specify interfaces to enable it\n",
>>>>>>> refs/remotes/origin/cm-10.0
				 net_dev->name);
		goto out;
	}

	if (primary_if->if_status != IF_ACTIVE) {
<<<<<<< HEAD
		ret = seq_printf(seq, "BATMAN mesh %s disabled - "
				 "primary interface not active\n",
=======
		ret = seq_printf(seq,
				 "BATMAN mesh %s disabled - primary interface not active\n",
>>>>>>> refs/remotes/origin/cm-10.0
				 net_dev->name);
		goto out;
	}

<<<<<<< HEAD
	seq_printf(seq, "Locally retrieved addresses (from %s) "
		   "announced via TT:\n",
		   net_dev->name);

	spin_lock_bh(&bat_priv->tt_lhash_lock);

	buf_size = 1;
	/* Estimate length for: " * xx:xx:xx:xx:xx:xx\n" */
	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];

		rcu_read_lock();
		__hlist_for_each_rcu(node, head)
			buf_size += 21;
		rcu_read_unlock();
	}

	buff = kmalloc(buf_size, GFP_ATOMIC);
	if (!buff) {
		spin_unlock_bh(&bat_priv->tt_lhash_lock);
		ret = -ENOMEM;
		goto out;
	}

	buff[0] = '\0';
	pos = 0;
=======
	seq_printf(seq,
		   "Locally retrieved addresses (from %s) announced via TT (TTVN: %u):\n",
		   net_dev->name, (uint8_t)atomic_read(&bat_priv->ttvn));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	spin_unlock_bh(&bat_priv->tt.changes_list_lock);

	/* Keep the buffer for possible tt_request */
	spin_lock_bh(&bat_priv->tt.last_changeset_lock);
	kfree(bat_priv->tt.last_changeset);
	bat_priv->tt.last_changeset_len = 0;
	bat_priv->tt.last_changeset = NULL;
	tt_change_len = batadv_tt_len(tt_diff_entries_count);
	/* check whether this new OGM has no changes due to size problems */
	if (tt_diff_entries_count > 0) {
		/* if kmalloc() fails we will reply with the full table
		 * instead of providing the diff
		 */
		bat_priv->tt.last_changeset = kzalloc(tt_diff_len, GFP_ATOMIC);
		if (bat_priv->tt.last_changeset) {
			memcpy(bat_priv->tt.last_changeset,
			       tt_change, tt_change_len);
			bat_priv->tt.last_changeset_len = tt_diff_len;
		}
	}
	spin_unlock_bh(&bat_priv->tt.last_changeset_lock);

container_register:
	batadv_tvlv_container_register(bat_priv, BATADV_TVLV_TT, 1, tt_data,
				       tvlv_len);
	kfree(tt_data);
}

int batadv_tt_local_seq_print_text(struct seq_file *seq, void *offset)
{
	struct net_device *net_dev = (struct net_device *)seq->private;
	struct batadv_priv *bat_priv = netdev_priv(net_dev);
	struct batadv_hashtable *hash = bat_priv->tt.local_hash;
	struct batadv_tt_common_entry *tt_common_entry;
	struct batadv_tt_local_entry *tt_local;
	struct batadv_hard_iface *primary_if;
	struct batadv_softif_vlan *vlan;
	struct hlist_head *head;
	unsigned short vid;
	uint32_t i;
	int last_seen_secs;
	int last_seen_msecs;
	unsigned long last_seen_jiffies;
	bool no_purge;
	uint16_t np_flag = BATADV_TT_CLIENT_NOPURGE;

	primary_if = batadv_seq_print_text_primary_if_get(seq);
	if (!primary_if)
		goto out;

	seq_printf(seq,
		   "Locally retrieved addresses (from %s) announced via TT (TTVN: %u):\n",
		   net_dev->name, (uint8_t)atomic_read(&bat_priv->tt.vn));
	seq_printf(seq, "       %-13s  %s %-7s %-9s (%-10s)\n", "Client", "VID",
		   "Flags", "Last seen", "CRC");
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];

		rcu_read_lock();
<<<<<<< HEAD
<<<<<<< HEAD
		hlist_for_each_entry_rcu(tt_local_entry, node,
					 head, hash_entry) {
			pos += snprintf(buff + pos, 22, " * %pM\n",
					tt_local_entry->addr);
		}
		rcu_read_unlock();
	}

	spin_unlock_bh(&bat_priv->tt_lhash_lock);

	seq_printf(seq, "%s", buff);
	kfree(buff);
=======
		hlist_for_each_entry_rcu(tt_common_entry, node,
					 head, hash_entry) {
			seq_printf(seq, " * %pM [%c%c%c%c%c]\n",
				   tt_common_entry->addr,
				   (tt_common_entry->flags &
				    TT_CLIENT_ROAM ? 'R' : '.'),
				   (tt_common_entry->flags &
				    TT_CLIENT_NOPURGE ? 'P' : '.'),
				   (tt_common_entry->flags &
				    TT_CLIENT_NEW ? 'N' : '.'),
				   (tt_common_entry->flags &
				    TT_CLIENT_PENDING ? 'X' : '.'),
				   (tt_common_entry->flags &
				    TT_CLIENT_WIFI ? 'W' : '.'));
		}
		rcu_read_unlock();
	}
>>>>>>> refs/remotes/origin/cm-10.0
out:
	if (primary_if)
		hardif_free_ref(primary_if);
	return ret;
}

<<<<<<< HEAD
static void _tt_local_del(struct hlist_node *node, void *arg)
{
	struct bat_priv *bat_priv = (struct bat_priv *)arg;
	void *data = container_of(node, struct tt_local_entry, hash_entry);

	kfree(data);
	bat_priv->num_local_tt--;
	atomic_set(&bat_priv->tt_local_changed, 1);
}

static void tt_local_del(struct bat_priv *bat_priv,
			  struct tt_local_entry *tt_local_entry,
			  char *message)
{
	bat_dbg(DBG_ROUTES, bat_priv, "Deleting local tt entry (%pM): %s\n",
		tt_local_entry->addr, message);

	hash_remove(bat_priv->tt_local_hash, compare_ltt, choose_orig,
		    tt_local_entry->addr);
	_tt_local_del(&tt_local_entry->hash_entry, bat_priv);
}

void tt_local_remove(struct bat_priv *bat_priv,
		      uint8_t *addr, char *message)
{
	struct tt_local_entry *tt_local_entry;

	spin_lock_bh(&bat_priv->tt_lhash_lock);

	tt_local_entry = tt_local_hash_find(bat_priv, addr);

	if (tt_local_entry)
		tt_local_del(bat_priv, tt_local_entry, message);

	spin_unlock_bh(&bat_priv->tt_lhash_lock);
}

static void tt_local_purge(struct work_struct *work)
{
	struct delayed_work *delayed_work =
		container_of(work, struct delayed_work, work);
	struct bat_priv *bat_priv =
		container_of(delayed_work, struct bat_priv, tt_work);
	struct hashtable_t *hash = bat_priv->tt_local_hash;
	struct tt_local_entry *tt_local_entry;
	struct hlist_node *node, *node_tmp;
	struct hlist_head *head;
	unsigned long timeout;
	int i;

	spin_lock_bh(&bat_priv->tt_lhash_lock);

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];

		hlist_for_each_entry_safe(tt_local_entry, node, node_tmp,
					  head, hash_entry) {
			if (tt_local_entry->never_purge)
				continue;

			timeout = tt_local_entry->last_seen;
			timeout += TT_LOCAL_TIMEOUT * HZ;

			if (time_before(jiffies, timeout))
				continue;

			tt_local_del(bat_priv, tt_local_entry,
				      "address timed out");
		}
	}

	spin_unlock_bh(&bat_priv->tt_lhash_lock);
	tt_local_start_timer(bat_priv);
}

void tt_local_free(struct bat_priv *bat_priv)
{
	if (!bat_priv->tt_local_hash)
		return;

	cancel_delayed_work_sync(&bat_priv->tt_work);
	hash_delete(bat_priv->tt_local_hash, _tt_local_del, bat_priv);
	bat_priv->tt_local_hash = NULL;
}

int tt_global_init(struct bat_priv *bat_priv)
=======
static void tt_local_set_pending(struct bat_priv *bat_priv,
				 struct tt_local_entry *tt_local_entry,
				 uint16_t flags, const char *message)
{
	tt_local_event(bat_priv, tt_local_entry->common.addr,
		       tt_local_entry->common.flags | flags);

	/* The local client has to be marked as "pending to be removed" but has
	 * to be kept in the table in order to send it in a full table
	 * response issued before the net ttvn increment (consistency check) */
	tt_local_entry->common.flags |= TT_CLIENT_PENDING;

	bat_dbg(DBG_TT, bat_priv,
		"Local tt entry (%pM) pending to be removed: %s\n",
		tt_local_entry->common.addr, message);
}

void tt_local_remove(struct bat_priv *bat_priv, const uint8_t *addr,
		     const char *message, bool roaming)
{
	struct tt_local_entry *tt_local_entry = NULL;

	tt_local_entry = tt_local_hash_find(bat_priv, addr);
	if (!tt_local_entry)
		goto out;

	tt_local_set_pending(bat_priv, tt_local_entry, TT_CLIENT_DEL |
			     (roaming ? TT_CLIENT_ROAM : NO_FLAGS), message);
out:
	if (tt_local_entry)
		tt_local_entry_free_ref(tt_local_entry);
}

static void tt_local_purge(struct bat_priv *bat_priv)
{
	struct hashtable_t *hash = bat_priv->tt_local_hash;
	struct tt_local_entry *tt_local_entry;
	struct tt_common_entry *tt_common_entry;
	struct hlist_node *node, *node_tmp;
=======
		hlist_for_each_entry_rcu(tt_common_entry,
					 head, hash_entry) {
			tt_local = container_of(tt_common_entry,
						struct batadv_tt_local_entry,
						common);
			vid = tt_common_entry->vid;
			last_seen_jiffies = jiffies - tt_local->last_seen;
			last_seen_msecs = jiffies_to_msecs(last_seen_jiffies);
			last_seen_secs = last_seen_msecs / 1000;
			last_seen_msecs = last_seen_msecs % 1000;

			no_purge = tt_common_entry->flags & np_flag;

			vlan = batadv_softif_vlan_get(bat_priv, vid);
			if (!vlan) {
				seq_printf(seq, "Cannot retrieve VLAN %d\n",
					   BATADV_PRINT_VID(vid));
				continue;
			}

			seq_printf(seq,
				   " * %pM %4i [%c%c%c%c%c] %3u.%03u   (%#.8x)\n",
				   tt_common_entry->addr,
				   BATADV_PRINT_VID(tt_common_entry->vid),
				   (tt_common_entry->flags &
				    BATADV_TT_CLIENT_ROAM ? 'R' : '.'),
				   no_purge ? 'P' : '.',
				   (tt_common_entry->flags &
				    BATADV_TT_CLIENT_NEW ? 'N' : '.'),
				   (tt_common_entry->flags &
				    BATADV_TT_CLIENT_PENDING ? 'X' : '.'),
				   (tt_common_entry->flags &
				    BATADV_TT_CLIENT_WIFI ? 'W' : '.'),
				   no_purge ? 0 : last_seen_secs,
				   no_purge ? 0 : last_seen_msecs,
				   vlan->tt.crc);

			batadv_softif_vlan_free_ref(vlan);
		}
		rcu_read_unlock();
	}
out:
	if (primary_if)
		batadv_hardif_free_ref(primary_if);
	return 0;
}

static void
batadv_tt_local_set_pending(struct batadv_priv *bat_priv,
			    struct batadv_tt_local_entry *tt_local_entry,
			    uint16_t flags, const char *message)
{
	batadv_tt_local_event(bat_priv, tt_local_entry, flags);

	/* The local client has to be marked as "pending to be removed" but has
	 * to be kept in the table in order to send it in a full table
	 * response issued before the net ttvn increment (consistency check)
	 */
	tt_local_entry->common.flags |= BATADV_TT_CLIENT_PENDING;

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Local tt entry (%pM, vid: %d) pending to be removed: %s\n",
		   tt_local_entry->common.addr,
		   BATADV_PRINT_VID(tt_local_entry->common.vid), message);
}

/**
 * batadv_tt_local_remove - logically remove an entry from the local table
 * @bat_priv: the bat priv with all the soft interface information
 * @addr: the MAC address of the client to remove
 * @vid: VLAN identifier
 * @message: message to append to the log on deletion
 * @roaming: true if the deletion is due to a roaming event
 *
 * Returns the flags assigned to the local entry before being deleted
 */
uint16_t batadv_tt_local_remove(struct batadv_priv *bat_priv,
				const uint8_t *addr, unsigned short vid,
				const char *message, bool roaming)
{
	struct batadv_tt_local_entry *tt_local_entry;
	uint16_t flags, curr_flags = BATADV_NO_FLAGS;

	tt_local_entry = batadv_tt_local_hash_find(bat_priv, addr, vid);
	if (!tt_local_entry)
		goto out;

	curr_flags = tt_local_entry->common.flags;

	flags = BATADV_TT_CLIENT_DEL;
	/* if this global entry addition is due to a roaming, the node has to
	 * mark the local entry as "roamed" in order to correctly reroute
	 * packets later
	 */
	if (roaming) {
		flags |= BATADV_TT_CLIENT_ROAM;
		/* mark the local client as ROAMed */
		tt_local_entry->common.flags |= BATADV_TT_CLIENT_ROAM;
	}

	if (!(tt_local_entry->common.flags & BATADV_TT_CLIENT_NEW)) {
		batadv_tt_local_set_pending(bat_priv, tt_local_entry, flags,
					    message);
		goto out;
	}
	/* if this client has been added right now, it is possible to
	 * immediately purge it
	 */
	batadv_tt_local_event(bat_priv, tt_local_entry, BATADV_TT_CLIENT_DEL);
	hlist_del_rcu(&tt_local_entry->common.hash_entry);
	batadv_tt_local_entry_free_ref(tt_local_entry);

out:
	if (tt_local_entry)
		batadv_tt_local_entry_free_ref(tt_local_entry);

	return curr_flags;
}

/**
 * batadv_tt_local_purge_list - purge inactive tt local entries
 * @bat_priv: the bat priv with all the soft interface information
 * @head: pointer to the list containing the local tt entries
 * @timeout: parameter deciding whether a given tt local entry is considered
 *  inactive or not
 */
static void batadv_tt_local_purge_list(struct batadv_priv *bat_priv,
				       struct hlist_head *head,
				       int timeout)
{
	struct batadv_tt_local_entry *tt_local_entry;
	struct batadv_tt_common_entry *tt_common_entry;
	struct hlist_node *node_tmp;

	hlist_for_each_entry_safe(tt_common_entry, node_tmp, head,
				  hash_entry) {
		tt_local_entry = container_of(tt_common_entry,
					      struct batadv_tt_local_entry,
					      common);
		if (tt_local_entry->common.flags & BATADV_TT_CLIENT_NOPURGE)
			continue;

		/* entry already marked for deletion */
		if (tt_local_entry->common.flags & BATADV_TT_CLIENT_PENDING)
			continue;

		if (!batadv_has_timed_out(tt_local_entry->last_seen, timeout))
			continue;

		batadv_tt_local_set_pending(bat_priv, tt_local_entry,
					    BATADV_TT_CLIENT_DEL, "timed out");
	}
}

/**
 * batadv_tt_local_purge - purge inactive tt local entries
 * @bat_priv: the bat priv with all the soft interface information
 * @timeout: parameter deciding whether a given tt local entry is considered
 *  inactive or not
 */
static void batadv_tt_local_purge(struct batadv_priv *bat_priv,
				  int timeout)
{
	struct batadv_hashtable *hash = bat_priv->tt.local_hash;
>>>>>>> refs/remotes/origin/master
	struct hlist_head *head;
	spinlock_t *list_lock; /* protects write access to the hash lists */
	uint32_t i;

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];
		list_lock = &hash->list_locks[i];

		spin_lock_bh(list_lock);
<<<<<<< HEAD
		hlist_for_each_entry_safe(tt_common_entry, node, node_tmp,
					  head, hash_entry) {
			tt_local_entry = container_of(tt_common_entry,
						      struct tt_local_entry,
						      common);
			if (tt_local_entry->common.flags & TT_CLIENT_NOPURGE)
				continue;

			/* entry already marked for deletion */
			if (tt_local_entry->common.flags & TT_CLIENT_PENDING)
				continue;

			if (!has_timed_out(tt_local_entry->last_seen,
					   TT_LOCAL_TIMEOUT))
				continue;

			tt_local_set_pending(bat_priv, tt_local_entry,
					     TT_CLIENT_DEL, "timed out");
		}
		spin_unlock_bh(list_lock);
	}

}

static void tt_local_table_free(struct bat_priv *bat_priv)
{
	struct hashtable_t *hash;
	spinlock_t *list_lock; /* protects write access to the hash lists */
	struct tt_common_entry *tt_common_entry;
	struct tt_local_entry *tt_local_entry;
	struct hlist_node *node, *node_tmp;
	struct hlist_head *head;
	uint32_t i;

	if (!bat_priv->tt_local_hash)
		return;

	hash = bat_priv->tt_local_hash;
=======
		batadv_tt_local_purge_list(bat_priv, head, timeout);
		spin_unlock_bh(list_lock);
	}
}

static void batadv_tt_local_table_free(struct batadv_priv *bat_priv)
{
	struct batadv_hashtable *hash;
	spinlock_t *list_lock; /* protects write access to the hash lists */
	struct batadv_tt_common_entry *tt_common_entry;
	struct batadv_tt_local_entry *tt_local;
	struct hlist_node *node_tmp;
	struct hlist_head *head;
	uint32_t i;

	if (!bat_priv->tt.local_hash)
		return;

	hash = bat_priv->tt.local_hash;
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];
		list_lock = &hash->list_locks[i];

		spin_lock_bh(list_lock);
<<<<<<< HEAD
		hlist_for_each_entry_safe(tt_common_entry, node, node_tmp,
					  head, hash_entry) {
			hlist_del_rcu(node);
			tt_local_entry = container_of(tt_common_entry,
						      struct tt_local_entry,
						      common);
			tt_local_entry_free_ref(tt_local_entry);
=======
		hlist_for_each_entry_safe(tt_common_entry, node_tmp,
					  head, hash_entry) {
			hlist_del_rcu(&tt_common_entry->hash_entry);
			tt_local = container_of(tt_common_entry,
						struct batadv_tt_local_entry,
						common);
			batadv_tt_local_entry_free_ref(tt_local);
>>>>>>> refs/remotes/origin/master
		}
		spin_unlock_bh(list_lock);
	}

<<<<<<< HEAD
	hash_destroy(hash);

	bat_priv->tt_local_hash = NULL;
}

static int tt_global_init(struct bat_priv *bat_priv)
>>>>>>> refs/remotes/origin/cm-10.0
{
	if (bat_priv->tt_global_hash)
		return 1;

	bat_priv->tt_global_hash = hash_new(1024);

	if (!bat_priv->tt_global_hash)
		return 0;

	return 1;
}

<<<<<<< HEAD
void tt_global_add_orig(struct bat_priv *bat_priv,
			 struct orig_node *orig_node,
			 unsigned char *tt_buff, int tt_buff_len)
{
	struct tt_global_entry *tt_global_entry;
	struct tt_local_entry *tt_local_entry;
	int tt_buff_count = 0;
	unsigned char *tt_ptr;

	while ((tt_buff_count + 1) * ETH_ALEN <= tt_buff_len) {
		spin_lock_bh(&bat_priv->tt_ghash_lock);

		tt_ptr = tt_buff + (tt_buff_count * ETH_ALEN);
		tt_global_entry = tt_global_hash_find(bat_priv, tt_ptr);

		if (!tt_global_entry) {
			spin_unlock_bh(&bat_priv->tt_ghash_lock);

			tt_global_entry =
				kmalloc(sizeof(struct tt_global_entry),
					GFP_ATOMIC);

			if (!tt_global_entry)
				break;

			memcpy(tt_global_entry->addr, tt_ptr, ETH_ALEN);

			bat_dbg(DBG_ROUTES, bat_priv,
				"Creating new global tt entry: "
				"%pM (via %pM)\n",
				tt_global_entry->addr, orig_node->orig);

			spin_lock_bh(&bat_priv->tt_ghash_lock);
			hash_add(bat_priv->tt_global_hash, compare_gtt,
				 choose_orig, tt_global_entry,
				 &tt_global_entry->hash_entry);

		}

		tt_global_entry->orig_node = orig_node;
		spin_unlock_bh(&bat_priv->tt_ghash_lock);

		/* remove address from local hash if present */
		spin_lock_bh(&bat_priv->tt_lhash_lock);

		tt_ptr = tt_buff + (tt_buff_count * ETH_ALEN);
		tt_local_entry = tt_local_hash_find(bat_priv, tt_ptr);

		if (tt_local_entry)
			tt_local_del(bat_priv, tt_local_entry,
				      "global tt received");

		spin_unlock_bh(&bat_priv->tt_lhash_lock);

		tt_buff_count++;
	}

	/* initialize, and overwrite if malloc succeeds */
	orig_node->tt_buff = NULL;
	orig_node->tt_buff_len = 0;

	if (tt_buff_len > 0) {
		orig_node->tt_buff = kmalloc(tt_buff_len, GFP_ATOMIC);
		if (orig_node->tt_buff) {
			memcpy(orig_node->tt_buff, tt_buff, tt_buff_len);
			orig_node->tt_buff_len = tt_buff_len;
		}
	}
=======
static void tt_changes_list_free(struct bat_priv *bat_priv)
{
	struct tt_change_node *entry, *safe;

	spin_lock_bh(&bat_priv->tt_changes_list_lock);

	list_for_each_entry_safe(entry, safe, &bat_priv->tt_changes_list,
				 list) {
		list_del(&entry->list);
		kfree(entry);
	}

	atomic_set(&bat_priv->tt_local_changes, 0);
	spin_unlock_bh(&bat_priv->tt_changes_list_lock);
}

/* caller must hold orig_node refcount */
int tt_global_add(struct bat_priv *bat_priv, struct orig_node *orig_node,
		  const unsigned char *tt_addr, uint8_t ttvn, bool roaming,
		  bool wifi)
{
	struct tt_global_entry *tt_global_entry;
	struct orig_node *orig_node_tmp;
	int ret = 0;
	int hash_added;

	tt_global_entry = tt_global_hash_find(bat_priv, tt_addr);

	if (!tt_global_entry) {
		tt_global_entry =
			kmalloc(sizeof(*tt_global_entry),
				GFP_ATOMIC);
		if (!tt_global_entry)
			goto out;

		memcpy(tt_global_entry->common.addr, tt_addr, ETH_ALEN);
		tt_global_entry->common.flags = NO_FLAGS;
		atomic_set(&tt_global_entry->common.refcount, 2);
		/* Assign the new orig_node */
		atomic_inc(&orig_node->refcount);
		tt_global_entry->orig_node = orig_node;
		tt_global_entry->ttvn = ttvn;
		tt_global_entry->roam_at = 0;

		hash_added = hash_add(bat_priv->tt_global_hash, compare_tt,
				 choose_orig, &tt_global_entry->common,
				 &tt_global_entry->common.hash_entry);

		if (unlikely(hash_added != 0)) {
			/* remove the reference for the hash */
			tt_global_entry_free_ref(tt_global_entry);
			goto out_remove;
		}
		atomic_inc(&orig_node->tt_size);
	} else {
		if (tt_global_entry->orig_node != orig_node) {
			atomic_dec(&tt_global_entry->orig_node->tt_size);
			orig_node_tmp = tt_global_entry->orig_node;
			atomic_inc(&orig_node->refcount);
			tt_global_entry->orig_node = orig_node;
			orig_node_free_ref(orig_node_tmp);
			atomic_inc(&orig_node->tt_size);
		}
		tt_global_entry->common.flags = NO_FLAGS;
		tt_global_entry->ttvn = ttvn;
		tt_global_entry->roam_at = 0;
	}

	if (wifi)
		tt_global_entry->common.flags |= TT_CLIENT_WIFI;

	bat_dbg(DBG_TT, bat_priv,
		"Creating new global tt entry: %pM (via %pM)\n",
		tt_global_entry->common.addr, orig_node->orig);

out_remove:
	/* remove address from local hash if present */
	tt_local_remove(bat_priv, tt_global_entry->common.addr,
			"global tt received", roaming);
	ret = 1;
out:
	if (tt_global_entry)
		tt_global_entry_free_ref(tt_global_entry);
	return ret;
>>>>>>> refs/remotes/origin/cm-10.0
}

int tt_global_seq_print_text(struct seq_file *seq, void *offset)
{
	struct net_device *net_dev = (struct net_device *)seq->private;
	struct bat_priv *bat_priv = netdev_priv(net_dev);
	struct hashtable_t *hash = bat_priv->tt_global_hash;
<<<<<<< HEAD
=======
	struct tt_common_entry *tt_common_entry;
>>>>>>> refs/remotes/origin/cm-10.0
	struct tt_global_entry *tt_global_entry;
	struct hard_iface *primary_if;
	struct hlist_node *node;
	struct hlist_head *head;
<<<<<<< HEAD
	size_t buf_size, pos;
	char *buff;
	int i, ret = 0;

	primary_if = primary_if_get_selected(bat_priv);
	if (!primary_if) {
		ret = seq_printf(seq, "BATMAN mesh %s disabled - please "
				 "specify interfaces to enable it\n",
=======
	uint32_t i;
	int ret = 0;

	primary_if = primary_if_get_selected(bat_priv);
	if (!primary_if) {
		ret = seq_printf(seq,
				 "BATMAN mesh %s disabled - please specify interfaces to enable it\n",
>>>>>>> refs/remotes/origin/cm-10.0
				 net_dev->name);
		goto out;
	}

	if (primary_if->if_status != IF_ACTIVE) {
<<<<<<< HEAD
		ret = seq_printf(seq, "BATMAN mesh %s disabled - "
				 "primary interface not active\n",
=======
		ret = seq_printf(seq,
				 "BATMAN mesh %s disabled - primary interface not active\n",
>>>>>>> refs/remotes/origin/cm-10.0
				 net_dev->name);
		goto out;
	}

	seq_printf(seq,
		   "Globally announced TT entries received via the mesh %s\n",
		   net_dev->name);
<<<<<<< HEAD

	spin_lock_bh(&bat_priv->tt_ghash_lock);

	buf_size = 1;
	/* Estimate length for: " * xx:xx:xx:xx:xx:xx via xx:xx:xx:xx:xx:xx\n"*/
	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];

		rcu_read_lock();
		__hlist_for_each_rcu(node, head)
			buf_size += 43;
		rcu_read_unlock();
	}

	buff = kmalloc(buf_size, GFP_ATOMIC);
	if (!buff) {
		spin_unlock_bh(&bat_priv->tt_ghash_lock);
		ret = -ENOMEM;
		goto out;
	}
	buff[0] = '\0';
	pos = 0;
=======
	seq_printf(seq, "       %-13s %s       %-15s %s %s\n",
		   "Client", "(TTVN)", "Originator", "(Curr TTVN)", "Flags");
>>>>>>> refs/remotes/origin/cm-10.0

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];

		rcu_read_lock();
<<<<<<< HEAD
		hlist_for_each_entry_rcu(tt_global_entry, node,
					 head, hash_entry) {
			pos += snprintf(buff + pos, 44,
					" * %pM via %pM\n",
					tt_global_entry->addr,
					tt_global_entry->orig_node->orig);
		}
		rcu_read_unlock();
	}

	spin_unlock_bh(&bat_priv->tt_ghash_lock);

	seq_printf(seq, "%s", buff);
	kfree(buff);
=======
		hlist_for_each_entry_rcu(tt_common_entry, node,
					 head, hash_entry) {
			tt_global_entry = container_of(tt_common_entry,
						       struct tt_global_entry,
						       common);
			seq_printf(seq,
				   " * %pM  (%3u) via %pM     (%3u)   [%c%c]\n",
				   tt_global_entry->common.addr,
				   tt_global_entry->ttvn,
				   tt_global_entry->orig_node->orig,
				   (uint8_t) atomic_read(
						&tt_global_entry->orig_node->
						last_ttvn),
				   (tt_global_entry->common.flags &
				    TT_CLIENT_ROAM ? 'R' : '.'),
				   (tt_global_entry->common.flags &
				    TT_CLIENT_WIFI ? 'W' : '.'));
		}
		rcu_read_unlock();
	}
>>>>>>> refs/remotes/origin/cm-10.0
out:
	if (primary_if)
		hardif_free_ref(primary_if);
	return ret;
}

<<<<<<< HEAD
static void _tt_global_del_orig(struct bat_priv *bat_priv,
				 struct tt_global_entry *tt_global_entry,
				 char *message)
{
	bat_dbg(DBG_ROUTES, bat_priv,
		"Deleting global tt entry %pM (via %pM): %s\n",
		tt_global_entry->addr, tt_global_entry->orig_node->orig,
		message);

	hash_remove(bat_priv->tt_global_hash, compare_gtt, choose_orig,
		    tt_global_entry->addr);
	kfree(tt_global_entry);
}

void tt_global_del_orig(struct bat_priv *bat_priv,
			 struct orig_node *orig_node, char *message)
{
	struct tt_global_entry *tt_global_entry;
	int tt_buff_count = 0;
	unsigned char *tt_ptr;

	if (orig_node->tt_buff_len == 0)
		return;

	spin_lock_bh(&bat_priv->tt_ghash_lock);

	while ((tt_buff_count + 1) * ETH_ALEN <= orig_node->tt_buff_len) {
		tt_ptr = orig_node->tt_buff + (tt_buff_count * ETH_ALEN);
		tt_global_entry = tt_global_hash_find(bat_priv, tt_ptr);

		if ((tt_global_entry) &&
		    (tt_global_entry->orig_node == orig_node))
			_tt_global_del_orig(bat_priv, tt_global_entry,
					     message);

		tt_buff_count++;
	}

	spin_unlock_bh(&bat_priv->tt_ghash_lock);

	orig_node->tt_buff_len = 0;
	kfree(orig_node->tt_buff);
	orig_node->tt_buff = NULL;
}

static void tt_global_del(struct hlist_node *node, void *arg)
{
	void *data = container_of(node, struct tt_global_entry, hash_entry);

	kfree(data);
}

void tt_global_free(struct bat_priv *bat_priv)
{
	if (!bat_priv->tt_global_hash)
		return;

	hash_delete(bat_priv->tt_global_hash, tt_global_del, NULL);
	bat_priv->tt_global_hash = NULL;
}

struct orig_node *transtable_search(struct bat_priv *bat_priv, uint8_t *addr)
{
	struct tt_global_entry *tt_global_entry;
	struct orig_node *orig_node = NULL;

	spin_lock_bh(&bat_priv->tt_ghash_lock);
	tt_global_entry = tt_global_hash_find(bat_priv, addr);

	if (!tt_global_entry)
		goto out;

=======
static void _tt_global_del(struct bat_priv *bat_priv,
			   struct tt_global_entry *tt_global_entry,
			   const char *message)
{
	if (!tt_global_entry)
		goto out;

	bat_dbg(DBG_TT, bat_priv,
		"Deleting global tt entry %pM (via %pM): %s\n",
		tt_global_entry->common.addr, tt_global_entry->orig_node->orig,
		message);

	atomic_dec(&tt_global_entry->orig_node->tt_size);

	hash_remove(bat_priv->tt_global_hash, compare_tt, choose_orig,
		    tt_global_entry->common.addr);
out:
	if (tt_global_entry)
		tt_global_entry_free_ref(tt_global_entry);
}

void tt_global_del(struct bat_priv *bat_priv,
		   struct orig_node *orig_node, const unsigned char *addr,
		   const char *message, bool roaming)
{
	struct tt_global_entry *tt_global_entry = NULL;
	struct tt_local_entry *tt_local_entry = NULL;

	tt_global_entry = tt_global_hash_find(bat_priv, addr);
	if (!tt_global_entry || tt_global_entry->orig_node != orig_node)
		goto out;

	if (!roaming)
		goto out_del;

	/* if we are deleting a global entry due to a roam
	 * event, there are two possibilities:
	 * 1) the client roamed from node A to node B => we mark
	 *    it with TT_CLIENT_ROAM, we start a timer and we
	 *    wait for node B to claim it. In case of timeout
	 *    the entry is purged.
	 * 2) the client roamed to us => we can directly delete
	 *    the global entry, since it is useless now. */
	tt_local_entry = tt_local_hash_find(bat_priv,
					    tt_global_entry->common.addr);
	if (!tt_local_entry) {
		tt_global_entry->common.flags |= TT_CLIENT_ROAM;
		tt_global_entry->roam_at = jiffies;
		goto out;
	}

out_del:
	_tt_global_del(bat_priv, tt_global_entry, message);

out:
	if (tt_global_entry)
		tt_global_entry_free_ref(tt_global_entry);
	if (tt_local_entry)
		tt_local_entry_free_ref(tt_local_entry);
}

void tt_global_del_orig(struct bat_priv *bat_priv,
			struct orig_node *orig_node, const char *message)
{
	struct tt_global_entry *tt_global_entry;
	struct tt_common_entry *tt_common_entry;
	uint32_t i;
	struct hashtable_t *hash = bat_priv->tt_global_hash;
	struct hlist_node *node, *safe;
	struct hlist_head *head;
	spinlock_t *list_lock; /* protects write access to the hash lists */
=======
	batadv_hash_destroy(hash);

	bat_priv->tt.local_hash = NULL;
}

static int batadv_tt_global_init(struct batadv_priv *bat_priv)
{
	if (bat_priv->tt.global_hash)
		return 0;

	bat_priv->tt.global_hash = batadv_hash_new(1024);

	if (!bat_priv->tt.global_hash)
		return -ENOMEM;

	batadv_hash_set_lock_class(bat_priv->tt.global_hash,
				   &batadv_tt_global_hash_lock_class_key);

	return 0;
}

static void batadv_tt_changes_list_free(struct batadv_priv *bat_priv)
{
	struct batadv_tt_change_node *entry, *safe;

	spin_lock_bh(&bat_priv->tt.changes_list_lock);

	list_for_each_entry_safe(entry, safe, &bat_priv->tt.changes_list,
				 list) {
		list_del(&entry->list);
		kfree(entry);
	}

	atomic_set(&bat_priv->tt.local_changes, 0);
	spin_unlock_bh(&bat_priv->tt.changes_list_lock);
}

/* retrieves the orig_tt_list_entry belonging to orig_node from the
 * batadv_tt_global_entry list
 *
 * returns it with an increased refcounter, NULL if not found
 */
static struct batadv_tt_orig_list_entry *
batadv_tt_global_orig_entry_find(const struct batadv_tt_global_entry *entry,
				 const struct batadv_orig_node *orig_node)
{
	struct batadv_tt_orig_list_entry *tmp_orig_entry, *orig_entry = NULL;
	const struct hlist_head *head;

	rcu_read_lock();
	head = &entry->orig_list;
	hlist_for_each_entry_rcu(tmp_orig_entry, head, list) {
		if (tmp_orig_entry->orig_node != orig_node)
			continue;
		if (!atomic_inc_not_zero(&tmp_orig_entry->refcount))
			continue;

		orig_entry = tmp_orig_entry;
		break;
	}
	rcu_read_unlock();

	return orig_entry;
}

/* find out if an orig_node is already in the list of a tt_global_entry.
 * returns true if found, false otherwise
 */
static bool
batadv_tt_global_entry_has_orig(const struct batadv_tt_global_entry *entry,
				const struct batadv_orig_node *orig_node)
{
	struct batadv_tt_orig_list_entry *orig_entry;
	bool found = false;

	orig_entry = batadv_tt_global_orig_entry_find(entry, orig_node);
	if (orig_entry) {
		found = true;
		batadv_tt_orig_list_entry_free_ref(orig_entry);
	}

	return found;
}

static void
batadv_tt_global_orig_entry_add(struct batadv_tt_global_entry *tt_global,
				struct batadv_orig_node *orig_node, int ttvn)
{
	struct batadv_tt_orig_list_entry *orig_entry;

	orig_entry = batadv_tt_global_orig_entry_find(tt_global, orig_node);
	if (orig_entry) {
		/* refresh the ttvn: the current value could be a bogus one that
		 * was added during a "temporary client detection"
		 */
		orig_entry->ttvn = ttvn;
		goto out;
	}

	orig_entry = kzalloc(sizeof(*orig_entry), GFP_ATOMIC);
	if (!orig_entry)
		goto out;

	INIT_HLIST_NODE(&orig_entry->list);
	atomic_inc(&orig_node->refcount);
	batadv_tt_global_size_inc(orig_node, tt_global->common.vid);
	orig_entry->orig_node = orig_node;
	orig_entry->ttvn = ttvn;
	atomic_set(&orig_entry->refcount, 2);

	spin_lock_bh(&tt_global->list_lock);
	hlist_add_head_rcu(&orig_entry->list,
			   &tt_global->orig_list);
	spin_unlock_bh(&tt_global->list_lock);
out:
	if (orig_entry)
		batadv_tt_orig_list_entry_free_ref(orig_entry);
}

/**
 * batadv_tt_global_add - add a new TT global entry or update an existing one
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: the originator announcing the client
 * @tt_addr: the mac address of the non-mesh client
 * @vid: VLAN identifier
 * @flags: TT flags that have to be set for this non-mesh client
 * @ttvn: the tt version number ever announcing this non-mesh client
 *
 * Add a new TT global entry for the given originator. If the entry already
 * exists add a new reference to the given originator (a global entry can have
 * references to multiple originators) and adjust the flags attribute to reflect
 * the function argument.
 * If a TT local entry exists for this non-mesh client remove it.
 *
 * The caller must hold orig_node refcount.
 *
 * Return true if the new entry has been added, false otherwise
 */
static bool batadv_tt_global_add(struct batadv_priv *bat_priv,
				 struct batadv_orig_node *orig_node,
				 const unsigned char *tt_addr,
				 unsigned short vid, uint16_t flags,
				 uint8_t ttvn)
{
	struct batadv_tt_global_entry *tt_global_entry;
	struct batadv_tt_local_entry *tt_local_entry;
	bool ret = false;
	int hash_added;
	struct batadv_tt_common_entry *common;
	uint16_t local_flags;

	/* ignore global entries from backbone nodes */
	if (batadv_bla_is_backbone_gw_orig(bat_priv, orig_node->orig, vid))
		return true;

	tt_global_entry = batadv_tt_global_hash_find(bat_priv, tt_addr, vid);
	tt_local_entry = batadv_tt_local_hash_find(bat_priv, tt_addr, vid);

	/* if the node already has a local client for this entry, it has to wait
	 * for a roaming advertisement instead of manually messing up the global
	 * table
	 */
	if ((flags & BATADV_TT_CLIENT_TEMP) && tt_local_entry &&
	    !(tt_local_entry->common.flags & BATADV_TT_CLIENT_NEW))
		goto out;

	if (!tt_global_entry) {
		tt_global_entry = kzalloc(sizeof(*tt_global_entry), GFP_ATOMIC);
		if (!tt_global_entry)
			goto out;

		common = &tt_global_entry->common;
		memcpy(common->addr, tt_addr, ETH_ALEN);
		common->vid = vid;

		common->flags = flags;
		tt_global_entry->roam_at = 0;
		/* node must store current time in case of roaming. This is
		 * needed to purge this entry out on timeout (if nobody claims
		 * it)
		 */
		if (flags & BATADV_TT_CLIENT_ROAM)
			tt_global_entry->roam_at = jiffies;
		atomic_set(&common->refcount, 2);
		common->added_at = jiffies;

		INIT_HLIST_HEAD(&tt_global_entry->orig_list);
		spin_lock_init(&tt_global_entry->list_lock);

		hash_added = batadv_hash_add(bat_priv->tt.global_hash,
					     batadv_compare_tt,
					     batadv_choose_tt, common,
					     &common->hash_entry);

		if (unlikely(hash_added != 0)) {
			/* remove the reference for the hash */
			batadv_tt_global_entry_free_ref(tt_global_entry);
			goto out_remove;
		}
	} else {
		common = &tt_global_entry->common;
		/* If there is already a global entry, we can use this one for
		 * our processing.
		 * But if we are trying to add a temporary client then here are
		 * two options at this point:
		 * 1) the global client is not a temporary client: the global
		 *    client has to be left as it is, temporary information
		 *    should never override any already known client state
		 * 2) the global client is a temporary client: purge the
		 *    originator list and add the new one orig_entry
		 */
		if (flags & BATADV_TT_CLIENT_TEMP) {
			if (!(common->flags & BATADV_TT_CLIENT_TEMP))
				goto out;
			if (batadv_tt_global_entry_has_orig(tt_global_entry,
							    orig_node))
				goto out_remove;
			batadv_tt_global_del_orig_list(tt_global_entry);
			goto add_orig_entry;
		}

		/* if the client was temporary added before receiving the first
		 * OGM announcing it, we have to clear the TEMP flag
		 */
		common->flags &= ~BATADV_TT_CLIENT_TEMP;

		/* the change can carry possible "attribute" flags like the
		 * TT_CLIENT_WIFI, therefore they have to be copied in the
		 * client entry
		 */
		tt_global_entry->common.flags |= flags;

		/* If there is the BATADV_TT_CLIENT_ROAM flag set, there is only
		 * one originator left in the list and we previously received a
		 * delete + roaming change for this originator.
		 *
		 * We should first delete the old originator before adding the
		 * new one.
		 */
		if (common->flags & BATADV_TT_CLIENT_ROAM) {
			batadv_tt_global_del_orig_list(tt_global_entry);
			common->flags &= ~BATADV_TT_CLIENT_ROAM;
			tt_global_entry->roam_at = 0;
		}
	}
add_orig_entry:
	/* add the new orig_entry (if needed) or update it */
	batadv_tt_global_orig_entry_add(tt_global_entry, orig_node, ttvn);

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Creating new global tt entry: %pM (vid: %d, via %pM)\n",
		   common->addr, BATADV_PRINT_VID(common->vid),
		   orig_node->orig);
	ret = true;

out_remove:

	/* remove address from local hash if present */
	local_flags = batadv_tt_local_remove(bat_priv, tt_addr, vid,
					     "global tt received",
					     flags & BATADV_TT_CLIENT_ROAM);
	tt_global_entry->common.flags |= local_flags & BATADV_TT_CLIENT_WIFI;

	if (!(flags & BATADV_TT_CLIENT_ROAM))
		/* this is a normal global add. Therefore the client is not in a
		 * roaming state anymore.
		 */
		tt_global_entry->common.flags &= ~BATADV_TT_CLIENT_ROAM;

out:
	if (tt_global_entry)
		batadv_tt_global_entry_free_ref(tt_global_entry);
	if (tt_local_entry)
		batadv_tt_local_entry_free_ref(tt_local_entry);
	return ret;
}

/* batadv_transtable_best_orig - Get best originator list entry from tt entry
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_global_entry: global translation table entry to be analyzed
 *
 * This functon assumes the caller holds rcu_read_lock().
 * Returns best originator list entry or NULL on errors.
 */
static struct batadv_tt_orig_list_entry *
batadv_transtable_best_orig(struct batadv_priv *bat_priv,
			    struct batadv_tt_global_entry *tt_global_entry)
{
	struct batadv_neigh_node *router, *best_router = NULL;
	struct batadv_algo_ops *bao = bat_priv->bat_algo_ops;
	struct hlist_head *head;
	struct batadv_tt_orig_list_entry *orig_entry, *best_entry = NULL;

	head = &tt_global_entry->orig_list;
	hlist_for_each_entry_rcu(orig_entry, head, list) {
		router = batadv_orig_node_get_router(orig_entry->orig_node);
		if (!router)
			continue;

		if (best_router &&
		    bao->bat_neigh_cmp(router, best_router) <= 0) {
			batadv_neigh_node_free_ref(router);
			continue;
		}

		/* release the refcount for the "old" best */
		if (best_router)
			batadv_neigh_node_free_ref(best_router);

		best_entry = orig_entry;
		best_router = router;
	}

	if (best_router)
		batadv_neigh_node_free_ref(best_router);

	return best_entry;
}

/* batadv_tt_global_print_entry - print all orig nodes who announce the address
 * for this global entry
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_global_entry: global translation table entry to be printed
 * @seq: debugfs table seq_file struct
 *
 * This functon assumes the caller holds rcu_read_lock().
 */
static void
batadv_tt_global_print_entry(struct batadv_priv *bat_priv,
			     struct batadv_tt_global_entry *tt_global_entry,
			     struct seq_file *seq)
{
	struct batadv_tt_orig_list_entry *orig_entry, *best_entry;
	struct batadv_tt_common_entry *tt_common_entry;
	struct batadv_orig_node_vlan *vlan;
	struct hlist_head *head;
	uint8_t last_ttvn;
	uint16_t flags;

	tt_common_entry = &tt_global_entry->common;
	flags = tt_common_entry->flags;

	best_entry = batadv_transtable_best_orig(bat_priv, tt_global_entry);
	if (best_entry) {
		vlan = batadv_orig_node_vlan_get(best_entry->orig_node,
						 tt_common_entry->vid);
		if (!vlan) {
			seq_printf(seq,
				   " * Cannot retrieve VLAN %d for originator %pM\n",
				   BATADV_PRINT_VID(tt_common_entry->vid),
				   best_entry->orig_node->orig);
			goto print_list;
		}

		last_ttvn = atomic_read(&best_entry->orig_node->last_ttvn);
		seq_printf(seq,
			   " %c %pM %4i   (%3u) via %pM     (%3u)   (%#.8x) [%c%c%c]\n",
			   '*', tt_global_entry->common.addr,
			   BATADV_PRINT_VID(tt_global_entry->common.vid),
			   best_entry->ttvn, best_entry->orig_node->orig,
			   last_ttvn, vlan->tt.crc,
			   (flags & BATADV_TT_CLIENT_ROAM ? 'R' : '.'),
			   (flags & BATADV_TT_CLIENT_WIFI ? 'W' : '.'),
			   (flags & BATADV_TT_CLIENT_TEMP ? 'T' : '.'));

		batadv_orig_node_vlan_free_ref(vlan);
	}

print_list:
	head = &tt_global_entry->orig_list;

	hlist_for_each_entry_rcu(orig_entry, head, list) {
		if (best_entry == orig_entry)
			continue;

		vlan = batadv_orig_node_vlan_get(orig_entry->orig_node,
						 tt_common_entry->vid);
		if (!vlan) {
			seq_printf(seq,
				   " + Cannot retrieve VLAN %d for originator %pM\n",
				   BATADV_PRINT_VID(tt_common_entry->vid),
				   orig_entry->orig_node->orig);
			continue;
		}

		last_ttvn = atomic_read(&orig_entry->orig_node->last_ttvn);
		seq_printf(seq,
			   " %c %pM %4d   (%3u) via %pM     (%3u)   (%#.8x) [%c%c%c]\n",
			   '+', tt_global_entry->common.addr,
			   BATADV_PRINT_VID(tt_global_entry->common.vid),
			   orig_entry->ttvn, orig_entry->orig_node->orig,
			   last_ttvn, vlan->tt.crc,
			   (flags & BATADV_TT_CLIENT_ROAM ? 'R' : '.'),
			   (flags & BATADV_TT_CLIENT_WIFI ? 'W' : '.'),
			   (flags & BATADV_TT_CLIENT_TEMP ? 'T' : '.'));

		batadv_orig_node_vlan_free_ref(vlan);
	}
}

int batadv_tt_global_seq_print_text(struct seq_file *seq, void *offset)
{
	struct net_device *net_dev = (struct net_device *)seq->private;
	struct batadv_priv *bat_priv = netdev_priv(net_dev);
	struct batadv_hashtable *hash = bat_priv->tt.global_hash;
	struct batadv_tt_common_entry *tt_common_entry;
	struct batadv_tt_global_entry *tt_global;
	struct batadv_hard_iface *primary_if;
	struct hlist_head *head;
	uint32_t i;

	primary_if = batadv_seq_print_text_primary_if_get(seq);
	if (!primary_if)
		goto out;

	seq_printf(seq,
		   "Globally announced TT entries received via the mesh %s\n",
		   net_dev->name);
	seq_printf(seq, "       %-13s  %s  %s       %-15s %s (%-10s) %s\n",
		   "Client", "VID", "(TTVN)", "Originator", "(Curr TTVN)",
		   "CRC", "Flags");

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];

		rcu_read_lock();
		hlist_for_each_entry_rcu(tt_common_entry,
					 head, hash_entry) {
			tt_global = container_of(tt_common_entry,
						 struct batadv_tt_global_entry,
						 common);
			batadv_tt_global_print_entry(bat_priv, tt_global, seq);
		}
		rcu_read_unlock();
	}
out:
	if (primary_if)
		batadv_hardif_free_ref(primary_if);
	return 0;
}

/* deletes the orig list of a tt_global_entry */
static void
batadv_tt_global_del_orig_list(struct batadv_tt_global_entry *tt_global_entry)
{
	struct hlist_head *head;
	struct hlist_node *safe;
	struct batadv_tt_orig_list_entry *orig_entry;

	spin_lock_bh(&tt_global_entry->list_lock);
	head = &tt_global_entry->orig_list;
	hlist_for_each_entry_safe(orig_entry, safe, head, list) {
		hlist_del_rcu(&orig_entry->list);
		batadv_tt_global_size_dec(orig_entry->orig_node,
					  tt_global_entry->common.vid);
		batadv_tt_orig_list_entry_free_ref(orig_entry);
	}
	spin_unlock_bh(&tt_global_entry->list_lock);
}

static void
batadv_tt_global_del_orig_entry(struct batadv_priv *bat_priv,
				struct batadv_tt_global_entry *tt_global_entry,
				struct batadv_orig_node *orig_node,
				const char *message)
{
	struct hlist_head *head;
	struct hlist_node *safe;
	struct batadv_tt_orig_list_entry *orig_entry;
	unsigned short vid;

	spin_lock_bh(&tt_global_entry->list_lock);
	head = &tt_global_entry->orig_list;
	hlist_for_each_entry_safe(orig_entry, safe, head, list) {
		if (orig_entry->orig_node == orig_node) {
			vid = tt_global_entry->common.vid;
			batadv_dbg(BATADV_DBG_TT, bat_priv,
				   "Deleting %pM from global tt entry %pM (vid: %d): %s\n",
				   orig_node->orig,
				   tt_global_entry->common.addr,
				   BATADV_PRINT_VID(vid), message);
			hlist_del_rcu(&orig_entry->list);
			batadv_tt_global_size_dec(orig_node,
						  tt_global_entry->common.vid);
			batadv_tt_orig_list_entry_free_ref(orig_entry);
		}
	}
	spin_unlock_bh(&tt_global_entry->list_lock);
}

/* If the client is to be deleted, we check if it is the last origantor entry
 * within tt_global entry. If yes, we set the BATADV_TT_CLIENT_ROAM flag and the
 * timer, otherwise we simply remove the originator scheduled for deletion.
 */
static void
batadv_tt_global_del_roaming(struct batadv_priv *bat_priv,
			     struct batadv_tt_global_entry *tt_global_entry,
			     struct batadv_orig_node *orig_node,
			     const char *message)
{
	bool last_entry = true;
	struct hlist_head *head;
	struct batadv_tt_orig_list_entry *orig_entry;

	/* no local entry exists, case 1:
	 * Check if this is the last one or if other entries exist.
	 */

	rcu_read_lock();
	head = &tt_global_entry->orig_list;
	hlist_for_each_entry_rcu(orig_entry, head, list) {
		if (orig_entry->orig_node != orig_node) {
			last_entry = false;
			break;
		}
	}
	rcu_read_unlock();

	if (last_entry) {
		/* its the last one, mark for roaming. */
		tt_global_entry->common.flags |= BATADV_TT_CLIENT_ROAM;
		tt_global_entry->roam_at = jiffies;
	} else
		/* there is another entry, we can simply delete this
		 * one and can still use the other one.
		 */
		batadv_tt_global_del_orig_entry(bat_priv, tt_global_entry,
						orig_node, message);
}

/**
 * batadv_tt_global_del - remove a client from the global table
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: an originator serving this client
 * @addr: the mac address of the client
 * @vid: VLAN identifier
 * @message: a message explaining the reason for deleting the client to print
 *  for debugging purpose
 * @roaming: true if the deletion has been triggered by a roaming event
 */
static void batadv_tt_global_del(struct batadv_priv *bat_priv,
				 struct batadv_orig_node *orig_node,
				 const unsigned char *addr, unsigned short vid,
				 const char *message, bool roaming)
{
	struct batadv_tt_global_entry *tt_global_entry;
	struct batadv_tt_local_entry *local_entry = NULL;

	tt_global_entry = batadv_tt_global_hash_find(bat_priv, addr, vid);
	if (!tt_global_entry)
		goto out;

	if (!roaming) {
		batadv_tt_global_del_orig_entry(bat_priv, tt_global_entry,
						orig_node, message);

		if (hlist_empty(&tt_global_entry->orig_list))
			batadv_tt_global_free(bat_priv, tt_global_entry,
					      message);

		goto out;
	}

	/* if we are deleting a global entry due to a roam
	 * event, there are two possibilities:
	 * 1) the client roamed from node A to node B => if there
	 *    is only one originator left for this client, we mark
	 *    it with BATADV_TT_CLIENT_ROAM, we start a timer and we
	 *    wait for node B to claim it. In case of timeout
	 *    the entry is purged.
	 *
	 *    If there are other originators left, we directly delete
	 *    the originator.
	 * 2) the client roamed to us => we can directly delete
	 *    the global entry, since it is useless now.
	 */
	local_entry = batadv_tt_local_hash_find(bat_priv,
						tt_global_entry->common.addr,
						vid);
	if (local_entry) {
		/* local entry exists, case 2: client roamed to us. */
		batadv_tt_global_del_orig_list(tt_global_entry);
		batadv_tt_global_free(bat_priv, tt_global_entry, message);
	} else
		/* no local entry exists, case 1: check for roaming */
		batadv_tt_global_del_roaming(bat_priv, tt_global_entry,
					     orig_node, message);


out:
	if (tt_global_entry)
		batadv_tt_global_entry_free_ref(tt_global_entry);
	if (local_entry)
		batadv_tt_local_entry_free_ref(local_entry);
}

/**
 * batadv_tt_global_del_orig - remove all the TT global entries belonging to the
 *  given originator matching the provided vid
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: the originator owning the entries to remove
 * @match_vid: the VLAN identifier to match. If negative all the entries will be
 *  removed
 * @message: debug message to print as "reason"
 */
void batadv_tt_global_del_orig(struct batadv_priv *bat_priv,
			       struct batadv_orig_node *orig_node,
			       int32_t match_vid,
			       const char *message)
{
	struct batadv_tt_global_entry *tt_global;
	struct batadv_tt_common_entry *tt_common_entry;
	uint32_t i;
	struct batadv_hashtable *hash = bat_priv->tt.global_hash;
	struct hlist_node *safe;
	struct hlist_head *head;
	spinlock_t *list_lock; /* protects write access to the hash lists */
	unsigned short vid;
>>>>>>> refs/remotes/origin/master

	if (!hash)
		return;

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];
		list_lock = &hash->list_locks[i];

		spin_lock_bh(list_lock);
<<<<<<< HEAD
		hlist_for_each_entry_safe(tt_common_entry, node, safe,
					  head, hash_entry) {
			tt_global_entry = container_of(tt_common_entry,
						       struct tt_global_entry,
						       common);
			if (tt_global_entry->orig_node == orig_node) {
				bat_dbg(DBG_TT, bat_priv,
					"Deleting global tt entry %pM (via %pM): %s\n",
					tt_global_entry->common.addr,
					tt_global_entry->orig_node->orig,
					message);
				hlist_del_rcu(node);
				tt_global_entry_free_ref(tt_global_entry);
=======
		hlist_for_each_entry_safe(tt_common_entry, safe,
					  head, hash_entry) {
			/* remove only matching entries */
			if (match_vid >= 0 && tt_common_entry->vid != match_vid)
				continue;

			tt_global = container_of(tt_common_entry,
						 struct batadv_tt_global_entry,
						 common);

			batadv_tt_global_del_orig_entry(bat_priv, tt_global,
							orig_node, message);

			if (hlist_empty(&tt_global->orig_list)) {
				vid = tt_global->common.vid;
				batadv_dbg(BATADV_DBG_TT, bat_priv,
					   "Deleting global tt entry %pM (vid: %d): %s\n",
					   tt_global->common.addr,
					   BATADV_PRINT_VID(vid), message);
				hlist_del_rcu(&tt_common_entry->hash_entry);
				batadv_tt_global_entry_free_ref(tt_global);
>>>>>>> refs/remotes/origin/master
			}
		}
		spin_unlock_bh(list_lock);
	}
<<<<<<< HEAD
	atomic_set(&orig_node->tt_size, 0);
	orig_node->tt_initialised = false;
}

static void tt_global_roam_purge(struct bat_priv *bat_priv)
{
	struct hashtable_t *hash = bat_priv->tt_global_hash;
	struct tt_common_entry *tt_common_entry;
	struct tt_global_entry *tt_global_entry;
	struct hlist_node *node, *node_tmp;
	struct hlist_head *head;
	spinlock_t *list_lock; /* protects write access to the hash lists */
	uint32_t i;
=======
	orig_node->tt_initialised = false;
}

static bool batadv_tt_global_to_purge(struct batadv_tt_global_entry *tt_global,
				      char **msg)
{
	bool purge = false;
	unsigned long roam_timeout = BATADV_TT_CLIENT_ROAM_TIMEOUT;
	unsigned long temp_timeout = BATADV_TT_CLIENT_TEMP_TIMEOUT;

	if ((tt_global->common.flags & BATADV_TT_CLIENT_ROAM) &&
	    batadv_has_timed_out(tt_global->roam_at, roam_timeout)) {
		purge = true;
		*msg = "Roaming timeout\n";
	}

	if ((tt_global->common.flags & BATADV_TT_CLIENT_TEMP) &&
	    batadv_has_timed_out(tt_global->common.added_at, temp_timeout)) {
		purge = true;
		*msg = "Temporary client timeout\n";
	}

	return purge;
}

static void batadv_tt_global_purge(struct batadv_priv *bat_priv)
{
	struct batadv_hashtable *hash = bat_priv->tt.global_hash;
	struct hlist_head *head;
	struct hlist_node *node_tmp;
	spinlock_t *list_lock; /* protects write access to the hash lists */
	uint32_t i;
	char *msg = NULL;
	struct batadv_tt_common_entry *tt_common;
	struct batadv_tt_global_entry *tt_global;
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];
		list_lock = &hash->list_locks[i];

		spin_lock_bh(list_lock);
<<<<<<< HEAD
		hlist_for_each_entry_safe(tt_common_entry, node, node_tmp,
					  head, hash_entry) {
			tt_global_entry = container_of(tt_common_entry,
						       struct tt_global_entry,
						       common);
			if (!(tt_global_entry->common.flags & TT_CLIENT_ROAM))
				continue;
			if (!has_timed_out(tt_global_entry->roam_at,
					   TT_CLIENT_ROAM_TIMEOUT))
				continue;

			bat_dbg(DBG_TT, bat_priv,
				"Deleting global tt entry (%pM): Roaming timeout\n",
				tt_global_entry->common.addr);
			atomic_dec(&tt_global_entry->orig_node->tt_size);
			hlist_del_rcu(node);
			tt_global_entry_free_ref(tt_global_entry);
		}
		spin_unlock_bh(list_lock);
	}

}

static void tt_global_table_free(struct bat_priv *bat_priv)
{
	struct hashtable_t *hash;
	spinlock_t *list_lock; /* protects write access to the hash lists */
	struct tt_common_entry *tt_common_entry;
	struct tt_global_entry *tt_global_entry;
	struct hlist_node *node, *node_tmp;
	struct hlist_head *head;
	uint32_t i;

	if (!bat_priv->tt_global_hash)
		return;

	hash = bat_priv->tt_global_hash;
=======
		hlist_for_each_entry_safe(tt_common, node_tmp, head,
					  hash_entry) {
			tt_global = container_of(tt_common,
						 struct batadv_tt_global_entry,
						 common);

			if (!batadv_tt_global_to_purge(tt_global, &msg))
				continue;

			batadv_dbg(BATADV_DBG_TT, bat_priv,
				   "Deleting global tt entry %pM (vid: %d): %s\n",
				   tt_global->common.addr,
				   BATADV_PRINT_VID(tt_global->common.vid),
				   msg);

			hlist_del_rcu(&tt_common->hash_entry);

			batadv_tt_global_entry_free_ref(tt_global);
		}
		spin_unlock_bh(list_lock);
	}
}

static void batadv_tt_global_table_free(struct batadv_priv *bat_priv)
{
	struct batadv_hashtable *hash;
	spinlock_t *list_lock; /* protects write access to the hash lists */
	struct batadv_tt_common_entry *tt_common_entry;
	struct batadv_tt_global_entry *tt_global;
	struct hlist_node *node_tmp;
	struct hlist_head *head;
	uint32_t i;

	if (!bat_priv->tt.global_hash)
		return;

	hash = bat_priv->tt.global_hash;
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];
		list_lock = &hash->list_locks[i];

		spin_lock_bh(list_lock);
<<<<<<< HEAD
		hlist_for_each_entry_safe(tt_common_entry, node, node_tmp,
					  head, hash_entry) {
			hlist_del_rcu(node);
			tt_global_entry = container_of(tt_common_entry,
						       struct tt_global_entry,
						       common);
			tt_global_entry_free_ref(tt_global_entry);
=======
		hlist_for_each_entry_safe(tt_common_entry, node_tmp,
					  head, hash_entry) {
			hlist_del_rcu(&tt_common_entry->hash_entry);
			tt_global = container_of(tt_common_entry,
						 struct batadv_tt_global_entry,
						 common);
			batadv_tt_global_entry_free_ref(tt_global);
>>>>>>> refs/remotes/origin/master
		}
		spin_unlock_bh(list_lock);
	}

<<<<<<< HEAD
	hash_destroy(hash);

	bat_priv->tt_global_hash = NULL;
}

static bool _is_ap_isolated(struct tt_local_entry *tt_local_entry,
			    struct tt_global_entry *tt_global_entry)
{
	bool ret = false;

	if (tt_local_entry->common.flags & TT_CLIENT_WIFI &&
	    tt_global_entry->common.flags & TT_CLIENT_WIFI)
=======
	batadv_hash_destroy(hash);

	bat_priv->tt.global_hash = NULL;
}

static bool
_batadv_is_ap_isolated(struct batadv_tt_local_entry *tt_local_entry,
		       struct batadv_tt_global_entry *tt_global_entry)
{
	bool ret = false;

	if (tt_local_entry->common.flags & BATADV_TT_CLIENT_WIFI &&
	    tt_global_entry->common.flags & BATADV_TT_CLIENT_WIFI)
>>>>>>> refs/remotes/origin/master
		ret = true;

	return ret;
}

<<<<<<< HEAD
struct orig_node *transtable_search(struct bat_priv *bat_priv,
				    const uint8_t *src, const uint8_t *addr)
{
	struct tt_local_entry *tt_local_entry = NULL;
	struct tt_global_entry *tt_global_entry = NULL;
	struct orig_node *orig_node = NULL;

	if (src && atomic_read(&bat_priv->ap_isolation)) {
		tt_local_entry = tt_local_hash_find(bat_priv, src);
		if (!tt_local_entry)
			goto out;
	}

	tt_global_entry = tt_global_hash_find(bat_priv, addr);
=======
/**
 * batadv_transtable_search - get the mesh destination for a given client
 * @bat_priv: the bat priv with all the soft interface information
 * @src: mac address of the source client
 * @addr: mac address of the destination client
 * @vid: VLAN identifier
 *
 * Returns a pointer to the originator that was selected as destination in the
 * mesh for contacting the client 'addr', NULL otherwise.
 * In case of multiple originators serving the same client, the function returns
 * the best one (best in terms of metric towards the destination node).
 *
 * If the two clients are AP isolated the function returns NULL.
 */
struct batadv_orig_node *batadv_transtable_search(struct batadv_priv *bat_priv,
						  const uint8_t *src,
						  const uint8_t *addr,
						  unsigned short vid)
{
	struct batadv_tt_local_entry *tt_local_entry = NULL;
	struct batadv_tt_global_entry *tt_global_entry = NULL;
	struct batadv_orig_node *orig_node = NULL;
	struct batadv_tt_orig_list_entry *best_entry;
	bool ap_isolation_enabled = false;
	struct batadv_softif_vlan *vlan;

	/* if the AP isolation is requested on a VLAN, then check for its
	 * setting in the proper VLAN private data structure
	 */
	vlan = batadv_softif_vlan_get(bat_priv, vid);
	if (vlan) {
		ap_isolation_enabled = atomic_read(&vlan->ap_isolation);
		batadv_softif_vlan_free_ref(vlan);
	}

	if (src && ap_isolation_enabled) {
		tt_local_entry = batadv_tt_local_hash_find(bat_priv, src, vid);
		if (!tt_local_entry ||
		    (tt_local_entry->common.flags & BATADV_TT_CLIENT_PENDING))
			goto out;
	}

	tt_global_entry = batadv_tt_global_hash_find(bat_priv, addr, vid);
>>>>>>> refs/remotes/origin/master
	if (!tt_global_entry)
		goto out;

	/* check whether the clients should not communicate due to AP
<<<<<<< HEAD
	 * isolation */
	if (tt_local_entry && _is_ap_isolated(tt_local_entry, tt_global_entry))
		goto out;

>>>>>>> refs/remotes/origin/cm-10.0
	if (!atomic_inc_not_zero(&tt_global_entry->orig_node->refcount))
		goto out;

	orig_node = tt_global_entry->orig_node;

out:
<<<<<<< HEAD
	spin_unlock_bh(&bat_priv->tt_ghash_lock);
	return orig_node;
}
=======
	if (tt_global_entry)
		tt_global_entry_free_ref(tt_global_entry);
	if (tt_local_entry)
		tt_local_entry_free_ref(tt_local_entry);
=======
	 * isolation
	 */
	if (tt_local_entry &&
	    _batadv_is_ap_isolated(tt_local_entry, tt_global_entry))
		goto out;

	rcu_read_lock();
	best_entry = batadv_transtable_best_orig(bat_priv, tt_global_entry);
	/* found anything? */
	if (best_entry)
		orig_node = best_entry->orig_node;
	if (orig_node && !atomic_inc_not_zero(&orig_node->refcount))
		orig_node = NULL;
	rcu_read_unlock();

out:
	if (tt_global_entry)
		batadv_tt_global_entry_free_ref(tt_global_entry);
	if (tt_local_entry)
		batadv_tt_local_entry_free_ref(tt_local_entry);
>>>>>>> refs/remotes/origin/master

	return orig_node;
}

<<<<<<< HEAD
/* Calculates the checksum of the local table of a given orig_node */
uint16_t tt_global_crc(struct bat_priv *bat_priv, struct orig_node *orig_node)
{
	uint16_t total = 0, total_one;
	struct hashtable_t *hash = bat_priv->tt_global_hash;
	struct tt_common_entry *tt_common_entry;
	struct tt_global_entry *tt_global_entry;
	struct hlist_node *node;
	struct hlist_head *head;
	uint32_t i;
	int j;
=======
/**
 * batadv_tt_global_crc - calculates the checksum of the local table belonging
 *  to the given orig_node
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: originator for which the CRC should be computed
 * @vid: VLAN identifier for which the CRC32 has to be computed
 *
 * This function computes the checksum for the global table corresponding to a
 * specific originator. In particular, the checksum is computed as follows: For
 * each client connected to the originator the CRC32C of the MAC address and the
 * VID is computed and then all the CRC32Cs of the various clients are xor'ed
 * together.
 *
 * The idea behind is that CRC32C should be used as much as possible in order to
 * produce a unique hash of the table, but since the order which is used to feed
 * the CRC32C function affects the result and since every node in the network
 * probably sorts the clients differently, the hash function cannot be directly
 * computed over the entire table. Hence the CRC32C is used only on
 * the single client entry, while all the results are then xor'ed together
 * because the XOR operation can combine them all while trying to reduce the
 * noise as much as possible.
 *
 * Returns the checksum of the global table of a given originator.
 */
static uint32_t batadv_tt_global_crc(struct batadv_priv *bat_priv,
				     struct batadv_orig_node *orig_node,
				     unsigned short vid)
{
	struct batadv_hashtable *hash = bat_priv->tt.global_hash;
	struct batadv_tt_common_entry *tt_common;
	struct batadv_tt_global_entry *tt_global;
	struct hlist_head *head;
	uint32_t i, crc_tmp, crc = 0;
	uint8_t flags;
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];

		rcu_read_lock();
<<<<<<< HEAD
		hlist_for_each_entry_rcu(tt_common_entry, node,
					 head, hash_entry) {
			tt_global_entry = container_of(tt_common_entry,
						       struct tt_global_entry,
						       common);
			if (compare_eth(tt_global_entry->orig_node,
					orig_node)) {
				/* Roaming clients are in the global table for
				 * consistency only. They don't have to be
				 * taken into account while computing the
				 * global crc */
				if (tt_common_entry->flags & TT_CLIENT_ROAM)
					continue;
				total_one = 0;
				for (j = 0; j < ETH_ALEN; j++)
					total_one = crc16_byte(total_one,
						tt_common_entry->addr[j]);
				total ^= total_one;
			}
=======
		hlist_for_each_entry_rcu(tt_common, head, hash_entry) {
			tt_global = container_of(tt_common,
						 struct batadv_tt_global_entry,
						 common);
			/* compute the CRC only for entries belonging to the
			 * VLAN identified by the vid passed as parameter
			 */
			if (tt_common->vid != vid)
				continue;

			/* Roaming clients are in the global table for
			 * consistency only. They don't have to be
			 * taken into account while computing the
			 * global crc
			 */
			if (tt_common->flags & BATADV_TT_CLIENT_ROAM)
				continue;
			/* Temporary clients have not been announced yet, so
			 * they have to be skipped while computing the global
			 * crc
			 */
			if (tt_common->flags & BATADV_TT_CLIENT_TEMP)
				continue;

			/* find out if this global entry is announced by this
			 * originator
			 */
			if (!batadv_tt_global_entry_has_orig(tt_global,
							     orig_node))
				continue;

			crc_tmp = crc32c(0, &tt_common->vid,
					 sizeof(tt_common->vid));

			/* compute the CRC on flags that have to be kept in sync
			 * among nodes
			 */
			flags = tt_common->flags & BATADV_TT_SYNC_MASK;
			crc_tmp = crc32c(crc_tmp, &flags, sizeof(flags));

			crc ^= crc32c(crc_tmp, tt_common->addr, ETH_ALEN);
>>>>>>> refs/remotes/origin/master
		}
		rcu_read_unlock();
	}

<<<<<<< HEAD
	return total;
}

/* Calculates the checksum of the local table */
uint16_t tt_local_crc(struct bat_priv *bat_priv)
{
	uint16_t total = 0, total_one;
	struct hashtable_t *hash = bat_priv->tt_local_hash;
	struct tt_common_entry *tt_common_entry;
	struct hlist_node *node;
	struct hlist_head *head;
	uint32_t i;
	int j;
=======
	return crc;
}

/**
 * batadv_tt_local_crc - calculates the checksum of the local table
 * @bat_priv: the bat priv with all the soft interface information
 * @vid: VLAN identifier for which the CRC32 has to be computed
 *
 * For details about the computation, please refer to the documentation for
 * batadv_tt_global_crc().
 *
 * Returns the checksum of the local table
 */
static uint32_t batadv_tt_local_crc(struct batadv_priv *bat_priv,
				    unsigned short vid)
{
	struct batadv_hashtable *hash = bat_priv->tt.local_hash;
	struct batadv_tt_common_entry *tt_common;
	struct hlist_head *head;
	uint32_t i, crc_tmp, crc = 0;
	uint8_t flags;
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];

		rcu_read_lock();
<<<<<<< HEAD
		hlist_for_each_entry_rcu(tt_common_entry, node,
					 head, hash_entry) {
			/* not yet committed clients have not to be taken into
			 * account while computing the CRC */
			if (tt_common_entry->flags & TT_CLIENT_NEW)
				continue;
			total_one = 0;
			for (j = 0; j < ETH_ALEN; j++)
				total_one = crc16_byte(total_one,
						   tt_common_entry->addr[j]);
			total ^= total_one;
=======
		hlist_for_each_entry_rcu(tt_common, head, hash_entry) {
			/* compute the CRC only for entries belonging to the
			 * VLAN identified by vid
			 */
			if (tt_common->vid != vid)
				continue;

			/* not yet committed clients have not to be taken into
			 * account while computing the CRC
			 */
			if (tt_common->flags & BATADV_TT_CLIENT_NEW)
				continue;

			crc_tmp = crc32c(0, &tt_common->vid,
					 sizeof(tt_common->vid));

			/* compute the CRC on flags that have to be kept in sync
			 * among nodes
			 */
			flags = tt_common->flags & BATADV_TT_SYNC_MASK;
			crc_tmp = crc32c(crc_tmp, &flags, sizeof(flags));

			crc ^= crc32c(crc_tmp, tt_common->addr, ETH_ALEN);
>>>>>>> refs/remotes/origin/master
		}
		rcu_read_unlock();
	}

<<<<<<< HEAD
	return total;
}

static void tt_req_list_free(struct bat_priv *bat_priv)
{
	struct tt_req_node *node, *safe;

	spin_lock_bh(&bat_priv->tt_req_list_lock);

	list_for_each_entry_safe(node, safe, &bat_priv->tt_req_list, list) {
=======
	return crc;
}

static void batadv_tt_req_list_free(struct batadv_priv *bat_priv)
{
	struct batadv_tt_req_node *node, *safe;

	spin_lock_bh(&bat_priv->tt.req_list_lock);

	list_for_each_entry_safe(node, safe, &bat_priv->tt.req_list, list) {
>>>>>>> refs/remotes/origin/master
		list_del(&node->list);
		kfree(node);
	}

<<<<<<< HEAD
	spin_unlock_bh(&bat_priv->tt_req_list_lock);
}

void tt_save_orig_buffer(struct bat_priv *bat_priv, struct orig_node *orig_node,
			 const unsigned char *tt_buff, uint8_t tt_num_changes)
{
	uint16_t tt_buff_len = tt_len(tt_num_changes);

	/* Replace the old buffer only if I received something in the
	 * last OGM (the OGM could carry no changes) */
=======
	spin_unlock_bh(&bat_priv->tt.req_list_lock);
}

static void batadv_tt_save_orig_buffer(struct batadv_priv *bat_priv,
				       struct batadv_orig_node *orig_node,
				       const void *tt_buff,
				       uint16_t tt_buff_len)
{
	/* Replace the old buffer only if I received something in the
	 * last OGM (the OGM could carry no changes)
	 */
>>>>>>> refs/remotes/origin/master
	spin_lock_bh(&orig_node->tt_buff_lock);
	if (tt_buff_len > 0) {
		kfree(orig_node->tt_buff);
		orig_node->tt_buff_len = 0;
		orig_node->tt_buff = kmalloc(tt_buff_len, GFP_ATOMIC);
		if (orig_node->tt_buff) {
			memcpy(orig_node->tt_buff, tt_buff, tt_buff_len);
			orig_node->tt_buff_len = tt_buff_len;
		}
	}
	spin_unlock_bh(&orig_node->tt_buff_lock);
}

<<<<<<< HEAD
static void tt_req_purge(struct bat_priv *bat_priv)
{
	struct tt_req_node *node, *safe;

	spin_lock_bh(&bat_priv->tt_req_list_lock);
	list_for_each_entry_safe(node, safe, &bat_priv->tt_req_list, list) {
		if (has_timed_out(node->issued_at, TT_REQUEST_TIMEOUT)) {
=======
static void batadv_tt_req_purge(struct batadv_priv *bat_priv)
{
	struct batadv_tt_req_node *node, *safe;

	spin_lock_bh(&bat_priv->tt.req_list_lock);
	list_for_each_entry_safe(node, safe, &bat_priv->tt.req_list, list) {
		if (batadv_has_timed_out(node->issued_at,
					 BATADV_TT_REQUEST_TIMEOUT)) {
>>>>>>> refs/remotes/origin/master
			list_del(&node->list);
			kfree(node);
		}
	}
<<<<<<< HEAD
	spin_unlock_bh(&bat_priv->tt_req_list_lock);
}

/* returns the pointer to the new tt_req_node struct if no request
 * has already been issued for this orig_node, NULL otherwise */
static struct tt_req_node *new_tt_req_node(struct bat_priv *bat_priv,
					  struct orig_node *orig_node)
{
	struct tt_req_node *tt_req_node_tmp, *tt_req_node = NULL;

	spin_lock_bh(&bat_priv->tt_req_list_lock);
	list_for_each_entry(tt_req_node_tmp, &bat_priv->tt_req_list, list) {
		if (compare_eth(tt_req_node_tmp, orig_node) &&
		    !has_timed_out(tt_req_node_tmp->issued_at,
				   TT_REQUEST_TIMEOUT))
=======
	spin_unlock_bh(&bat_priv->tt.req_list_lock);
}

/* returns the pointer to the new tt_req_node struct if no request
 * has already been issued for this orig_node, NULL otherwise
 */
static struct batadv_tt_req_node *
batadv_new_tt_req_node(struct batadv_priv *bat_priv,
		       struct batadv_orig_node *orig_node)
{
	struct batadv_tt_req_node *tt_req_node_tmp, *tt_req_node = NULL;

	spin_lock_bh(&bat_priv->tt.req_list_lock);
	list_for_each_entry(tt_req_node_tmp, &bat_priv->tt.req_list, list) {
		if (batadv_compare_eth(tt_req_node_tmp, orig_node) &&
		    !batadv_has_timed_out(tt_req_node_tmp->issued_at,
					  BATADV_TT_REQUEST_TIMEOUT))
>>>>>>> refs/remotes/origin/master
			goto unlock;
	}

	tt_req_node = kmalloc(sizeof(*tt_req_node), GFP_ATOMIC);
	if (!tt_req_node)
		goto unlock;

	memcpy(tt_req_node->addr, orig_node->orig, ETH_ALEN);
	tt_req_node->issued_at = jiffies;

<<<<<<< HEAD
	list_add(&tt_req_node->list, &bat_priv->tt_req_list);
unlock:
	spin_unlock_bh(&bat_priv->tt_req_list_lock);
	return tt_req_node;
}

/* data_ptr is useless here, but has to be kept to respect the prototype */
static int tt_local_valid_entry(const void *entry_ptr, const void *data_ptr)
{
	const struct tt_common_entry *tt_common_entry = entry_ptr;

	if (tt_common_entry->flags & TT_CLIENT_NEW)
=======
	list_add(&tt_req_node->list, &bat_priv->tt.req_list);
unlock:
	spin_unlock_bh(&bat_priv->tt.req_list_lock);
	return tt_req_node;
}

/**
 * batadv_tt_local_valid - verify that given tt entry is a valid one
 * @entry_ptr: to be checked local tt entry
 * @data_ptr: not used but definition required to satisfy the callback prototype
 *
 * Returns 1 if the entry is a valid, 0 otherwise.
 */
static int batadv_tt_local_valid(const void *entry_ptr, const void *data_ptr)
{
	const struct batadv_tt_common_entry *tt_common_entry = entry_ptr;

	if (tt_common_entry->flags & BATADV_TT_CLIENT_NEW)
>>>>>>> refs/remotes/origin/master
		return 0;
	return 1;
}

<<<<<<< HEAD
static int tt_global_valid_entry(const void *entry_ptr, const void *data_ptr)
{
	const struct tt_common_entry *tt_common_entry = entry_ptr;
	const struct tt_global_entry *tt_global_entry;
	const struct orig_node *orig_node = data_ptr;

	if (tt_common_entry->flags & TT_CLIENT_ROAM)
		return 0;

	tt_global_entry = container_of(tt_common_entry, struct tt_global_entry,
				       common);

	return (tt_global_entry->orig_node == orig_node);
}

static struct sk_buff *tt_response_fill_table(uint16_t tt_len, uint8_t ttvn,
					      struct hashtable_t *hash,
					      struct hard_iface *primary_if,
					      int (*valid_cb)(const void *,
							      const void *),
					      void *cb_data)
{
	struct tt_common_entry *tt_common_entry;
	struct tt_query_packet *tt_response;
	struct tt_change *tt_change;
	struct hlist_node *node;
	struct hlist_head *head;
	struct sk_buff *skb = NULL;
	uint16_t tt_tot, tt_count;
	ssize_t tt_query_size = sizeof(struct tt_query_packet);
	uint32_t i;

	if (tt_query_size + tt_len > primary_if->soft_iface->mtu) {
		tt_len = primary_if->soft_iface->mtu - tt_query_size;
		tt_len -= tt_len % sizeof(struct tt_change);
	}
	tt_tot = tt_len / sizeof(struct tt_change);

	skb = dev_alloc_skb(tt_query_size + tt_len + ETH_HLEN);
	if (!skb)
		goto out;

	skb_reserve(skb, ETH_HLEN);
	tt_response = (struct tt_query_packet *)skb_put(skb,
						     tt_query_size + tt_len);
	tt_response->ttvn = ttvn;

	tt_change = (struct tt_change *)(skb->data + tt_query_size);
	tt_count = 0;
=======
static int batadv_tt_global_valid(const void *entry_ptr,
				  const void *data_ptr)
{
	const struct batadv_tt_common_entry *tt_common_entry = entry_ptr;
	const struct batadv_tt_global_entry *tt_global_entry;
	const struct batadv_orig_node *orig_node = data_ptr;

	if (tt_common_entry->flags & BATADV_TT_CLIENT_ROAM ||
	    tt_common_entry->flags & BATADV_TT_CLIENT_TEMP)
		return 0;

	tt_global_entry = container_of(tt_common_entry,
				       struct batadv_tt_global_entry,
				       common);

	return batadv_tt_global_entry_has_orig(tt_global_entry, orig_node);
}

/**
 * batadv_tt_tvlv_generate - fill the tvlv buff with the tt entries from the
 *  specified tt hash
 * @bat_priv: the bat priv with all the soft interface information
 * @hash: hash table containing the tt entries
 * @tt_len: expected tvlv tt data buffer length in number of bytes
 * @tvlv_buff: pointer to the buffer to fill with the TT data
 * @valid_cb: function to filter tt change entries
 * @cb_data: data passed to the filter function as argument
 */
static void batadv_tt_tvlv_generate(struct batadv_priv *bat_priv,
				    struct batadv_hashtable *hash,
				    void *tvlv_buff, uint16_t tt_len,
				    int (*valid_cb)(const void *, const void *),
				    void *cb_data)
{
	struct batadv_tt_common_entry *tt_common_entry;
	struct batadv_tvlv_tt_change *tt_change;
	struct hlist_head *head;
	uint16_t tt_tot, tt_num_entries = 0;
	uint32_t i;

	tt_tot = batadv_tt_entries(tt_len);
	tt_change = (struct batadv_tvlv_tt_change *)tvlv_buff;
>>>>>>> refs/remotes/origin/master

	rcu_read_lock();
	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];

<<<<<<< HEAD
		hlist_for_each_entry_rcu(tt_common_entry, node,
					 head, hash_entry) {
			if (tt_count == tt_tot)
=======
		hlist_for_each_entry_rcu(tt_common_entry,
					 head, hash_entry) {
			if (tt_tot == tt_num_entries)
>>>>>>> refs/remotes/origin/master
				break;

			if ((valid_cb) && (!valid_cb(tt_common_entry, cb_data)))
				continue;

			memcpy(tt_change->addr, tt_common_entry->addr,
			       ETH_ALEN);
<<<<<<< HEAD
			tt_change->flags = NO_FLAGS;

			tt_count++;
=======
			tt_change->flags = tt_common_entry->flags;
			tt_change->vid = htons(tt_common_entry->vid);
			memset(tt_change->reserved, 0,
			       sizeof(tt_change->reserved));

			tt_num_entries++;
>>>>>>> refs/remotes/origin/master
			tt_change++;
		}
	}
	rcu_read_unlock();
<<<<<<< HEAD

	/* store in the message the number of entries we have successfully
	 * copied */
	tt_response->tt_data = htons(tt_count);

out:
	return skb;
}

static int send_tt_request(struct bat_priv *bat_priv,
			   struct orig_node *dst_orig_node,
			   uint8_t ttvn, uint16_t tt_crc, bool full_table)
{
	struct sk_buff *skb = NULL;
	struct tt_query_packet *tt_request;
	struct neigh_node *neigh_node = NULL;
	struct hard_iface *primary_if;
	struct tt_req_node *tt_req_node = NULL;
	int ret = 1;

	primary_if = primary_if_get_selected(bat_priv);
=======
}

/**
 * batadv_tt_global_check_crc - check if all the CRCs are correct
 * @orig_node: originator for which the CRCs have to be checked
 * @tt_vlan: pointer to the first tvlv VLAN entry
 * @num_vlan: number of tvlv VLAN entries
 * @create: if true, create VLAN objects if not found
 *
 * Return true if all the received CRCs match the locally stored ones, false
 * otherwise
 */
static bool batadv_tt_global_check_crc(struct batadv_orig_node *orig_node,
				       struct batadv_tvlv_tt_vlan_data *tt_vlan,
				       uint16_t num_vlan)
{
	struct batadv_tvlv_tt_vlan_data *tt_vlan_tmp;
	struct batadv_orig_node_vlan *vlan;
	int i;

	/* check if each received CRC matches the locally stored one */
	for (i = 0; i < num_vlan; i++) {
		tt_vlan_tmp = tt_vlan + i;

		/* if orig_node is a backbone node for this VLAN, don't check
		 * the CRC as we ignore all the global entries over it
		 */
		if (batadv_bla_is_backbone_gw_orig(orig_node->bat_priv,
						   orig_node->orig,
						   ntohs(tt_vlan_tmp->vid)))
			continue;

		vlan = batadv_orig_node_vlan_get(orig_node,
						 ntohs(tt_vlan_tmp->vid));
		if (!vlan)
			return false;

		if (vlan->tt.crc != ntohl(tt_vlan_tmp->crc))
			return false;
	}

	return true;
}

/**
 * batadv_tt_local_update_crc - update all the local CRCs
 * @bat_priv: the bat priv with all the soft interface information
 */
static void batadv_tt_local_update_crc(struct batadv_priv *bat_priv)
{
	struct batadv_softif_vlan *vlan;

	/* recompute the global CRC for each VLAN */
	rcu_read_lock();
	hlist_for_each_entry_rcu(vlan, &bat_priv->softif_vlan_list, list) {
		vlan->tt.crc = batadv_tt_local_crc(bat_priv, vlan->vid);
	}
	rcu_read_unlock();
}

/**
 * batadv_tt_global_update_crc - update all the global CRCs for this orig_node
 * @bat_priv: the bat priv with all the soft interface information
 * @orig_node: the orig_node for which the CRCs have to be updated
 */
static void batadv_tt_global_update_crc(struct batadv_priv *bat_priv,
					struct batadv_orig_node *orig_node)
{
	struct batadv_orig_node_vlan *vlan;
	uint32_t crc;

	/* recompute the global CRC for each VLAN */
	rcu_read_lock();
	list_for_each_entry_rcu(vlan, &orig_node->vlan_list, list) {
		/* if orig_node is a backbone node for this VLAN, don't compute
		 * the CRC as we ignore all the global entries over it
		 */
		if (batadv_bla_is_backbone_gw_orig(bat_priv, orig_node->orig,
						   vlan->vid))
			continue;

		crc = batadv_tt_global_crc(bat_priv, orig_node, vlan->vid);
		vlan->tt.crc = crc;
	}
	rcu_read_unlock();
}

/**
 * batadv_send_tt_request - send a TT Request message to a given node
 * @bat_priv: the bat priv with all the soft interface information
 * @dst_orig_node: the destination of the message
 * @ttvn: the version number that the source of the message is looking for
 * @tt_vlan: pointer to the first tvlv VLAN object to request
 * @num_vlan: number of tvlv VLAN entries
 * @full_table: ask for the entire translation table if true, while only for the
 *  last TT diff otherwise
 */
static int batadv_send_tt_request(struct batadv_priv *bat_priv,
				  struct batadv_orig_node *dst_orig_node,
				  uint8_t ttvn,
				  struct batadv_tvlv_tt_vlan_data *tt_vlan,
				  uint16_t num_vlan, bool full_table)
{
	struct batadv_tvlv_tt_data *tvlv_tt_data = NULL;
	struct batadv_tt_req_node *tt_req_node = NULL;
	struct batadv_tvlv_tt_vlan_data *tt_vlan_req;
	struct batadv_hard_iface *primary_if;
	bool ret = false;
	int i, size;

	primary_if = batadv_primary_if_get_selected(bat_priv);
>>>>>>> refs/remotes/origin/master
	if (!primary_if)
		goto out;

	/* The new tt_req will be issued only if I'm not waiting for a
<<<<<<< HEAD
	 * reply from the same orig_node yet */
	tt_req_node = new_tt_req_node(bat_priv, dst_orig_node);
	if (!tt_req_node)
		goto out;

	skb = dev_alloc_skb(sizeof(struct tt_query_packet) + ETH_HLEN);
	if (!skb)
		goto out;

	skb_reserve(skb, ETH_HLEN);

	tt_request = (struct tt_query_packet *)skb_put(skb,
				sizeof(struct tt_query_packet));

	tt_request->header.packet_type = BAT_TT_QUERY;
	tt_request->header.version = COMPAT_VERSION;
	memcpy(tt_request->src, primary_if->net_dev->dev_addr, ETH_ALEN);
	memcpy(tt_request->dst, dst_orig_node->orig, ETH_ALEN);
	tt_request->header.ttl = TTL;
	tt_request->ttvn = ttvn;
	tt_request->tt_data = tt_crc;
	tt_request->flags = TT_REQUEST;

	if (full_table)
		tt_request->flags |= TT_FULL_TABLE;

	neigh_node = orig_node_get_router(dst_orig_node);
	if (!neigh_node)
		goto out;

	bat_dbg(DBG_TT, bat_priv,
		"Sending TT_REQUEST to %pM via %pM [%c]\n",
		dst_orig_node->orig, neigh_node->addr,
		(full_table ? 'F' : '.'));

	send_skb_packet(skb, neigh_node->if_incoming, neigh_node->addr);
	ret = 0;

out:
	if (neigh_node)
		neigh_node_free_ref(neigh_node);
	if (primary_if)
		hardif_free_ref(primary_if);
	if (ret)
		kfree_skb(skb);
	if (ret && tt_req_node) {
		spin_lock_bh(&bat_priv->tt_req_list_lock);
		list_del(&tt_req_node->list);
		spin_unlock_bh(&bat_priv->tt_req_list_lock);
		kfree(tt_req_node);
	}
	return ret;
}

static bool send_other_tt_response(struct bat_priv *bat_priv,
				   struct tt_query_packet *tt_request)
{
	struct orig_node *req_dst_orig_node = NULL, *res_dst_orig_node = NULL;
	struct neigh_node *neigh_node = NULL;
	struct hard_iface *primary_if = NULL;
	uint8_t orig_ttvn, req_ttvn, ttvn;
	int ret = false;
	unsigned char *tt_buff;
	bool full_table;
	uint16_t tt_len, tt_tot;
	struct sk_buff *skb = NULL;
	struct tt_query_packet *tt_response;

	bat_dbg(DBG_TT, bat_priv,
		"Received TT_REQUEST from %pM for ttvn: %u (%pM) [%c]\n",
		tt_request->src, tt_request->ttvn, tt_request->dst,
		(tt_request->flags & TT_FULL_TABLE ? 'F' : '.'));

	/* Let's get the orig node of the REAL destination */
	req_dst_orig_node = orig_hash_find(bat_priv, tt_request->dst);
	if (!req_dst_orig_node)
		goto out;

	res_dst_orig_node = orig_hash_find(bat_priv, tt_request->src);
	if (!res_dst_orig_node)
		goto out;

	neigh_node = orig_node_get_router(res_dst_orig_node);
	if (!neigh_node)
		goto out;

	primary_if = primary_if_get_selected(bat_priv);
	if (!primary_if)
		goto out;

	orig_ttvn = (uint8_t)atomic_read(&req_dst_orig_node->last_ttvn);
	req_ttvn = tt_request->ttvn;

	/* I don't have the requested data */
	if (orig_ttvn != req_ttvn ||
	    tt_request->tt_data != req_dst_orig_node->tt_crc)
		goto out;

	/* If the full table has been explicitly requested */
	if (tt_request->flags & TT_FULL_TABLE ||
=======
	 * reply from the same orig_node yet
	 */
	tt_req_node = batadv_new_tt_req_node(bat_priv, dst_orig_node);
	if (!tt_req_node)
		goto out;

	size = sizeof(*tvlv_tt_data) + sizeof(*tt_vlan_req) * num_vlan;
	tvlv_tt_data = kzalloc(size, GFP_ATOMIC);
	if (!tvlv_tt_data)
		goto out;

	tvlv_tt_data->flags = BATADV_TT_REQUEST;
	tvlv_tt_data->ttvn = ttvn;
	tvlv_tt_data->num_vlan = htons(num_vlan);

	/* send all the CRCs within the request. This is needed by intermediate
	 * nodes to ensure they have the correct table before replying
	 */
	tt_vlan_req = (struct batadv_tvlv_tt_vlan_data *)(tvlv_tt_data + 1);
	for (i = 0; i < num_vlan; i++) {
		tt_vlan_req->vid = tt_vlan->vid;
		tt_vlan_req->crc = tt_vlan->crc;

		tt_vlan_req++;
		tt_vlan++;
	}

	if (full_table)
		tvlv_tt_data->flags |= BATADV_TT_FULL_TABLE;

	batadv_dbg(BATADV_DBG_TT, bat_priv, "Sending TT_REQUEST to %pM [%c]\n",
		   dst_orig_node->orig, full_table ? 'F' : '.');

	batadv_inc_counter(bat_priv, BATADV_CNT_TT_REQUEST_TX);
	batadv_tvlv_unicast_send(bat_priv, primary_if->net_dev->dev_addr,
				 dst_orig_node->orig, BATADV_TVLV_TT, 1,
				 tvlv_tt_data, size);
	ret = true;

out:
	if (primary_if)
		batadv_hardif_free_ref(primary_if);
	if (ret && tt_req_node) {
		spin_lock_bh(&bat_priv->tt.req_list_lock);
		list_del(&tt_req_node->list);
		spin_unlock_bh(&bat_priv->tt.req_list_lock);
		kfree(tt_req_node);
	}
	kfree(tvlv_tt_data);
	return ret;
}

/**
 * batadv_send_other_tt_response - send reply to tt request concerning another
 *  node's translation table
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_data: tt data containing the tt request information
 * @req_src: mac address of tt request sender
 * @req_dst: mac address of tt request recipient
 *
 * Returns true if tt request reply was sent, false otherwise.
 */
static bool batadv_send_other_tt_response(struct batadv_priv *bat_priv,
					  struct batadv_tvlv_tt_data *tt_data,
					  uint8_t *req_src, uint8_t *req_dst)
{
	struct batadv_orig_node *req_dst_orig_node;
	struct batadv_orig_node *res_dst_orig_node = NULL;
	struct batadv_tvlv_tt_change *tt_change;
	struct batadv_tvlv_tt_data *tvlv_tt_data = NULL;
	struct batadv_tvlv_tt_vlan_data *tt_vlan;
	bool ret = false, full_table;
	uint8_t orig_ttvn, req_ttvn;
	uint16_t tvlv_len;
	int32_t tt_len;

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Received TT_REQUEST from %pM for ttvn: %u (%pM) [%c]\n",
		   req_src, tt_data->ttvn, req_dst,
		   (tt_data->flags & BATADV_TT_FULL_TABLE ? 'F' : '.'));

	/* Let's get the orig node of the REAL destination */
	req_dst_orig_node = batadv_orig_hash_find(bat_priv, req_dst);
	if (!req_dst_orig_node)
		goto out;

	res_dst_orig_node = batadv_orig_hash_find(bat_priv, req_src);
	if (!res_dst_orig_node)
		goto out;

	orig_ttvn = (uint8_t)atomic_read(&req_dst_orig_node->last_ttvn);
	req_ttvn = tt_data->ttvn;

	tt_vlan = (struct batadv_tvlv_tt_vlan_data *)(tt_data + 1);
	/* this node doesn't have the requested data */
	if (orig_ttvn != req_ttvn ||
	    !batadv_tt_global_check_crc(req_dst_orig_node, tt_vlan,
					ntohs(tt_data->num_vlan)))
		goto out;

	/* If the full table has been explicitly requested */
	if (tt_data->flags & BATADV_TT_FULL_TABLE ||
>>>>>>> refs/remotes/origin/master
	    !req_dst_orig_node->tt_buff)
		full_table = true;
	else
		full_table = false;

<<<<<<< HEAD
	/* In this version, fragmentation is not implemented, then
	 * I'll send only one packet with as much TT entries as I can */
	if (!full_table) {
		spin_lock_bh(&req_dst_orig_node->tt_buff_lock);
		tt_len = req_dst_orig_node->tt_buff_len;
		tt_tot = tt_len / sizeof(struct tt_change);

		skb = dev_alloc_skb(sizeof(struct tt_query_packet) +
				    tt_len + ETH_HLEN);
		if (!skb)
			goto unlock;

		skb_reserve(skb, ETH_HLEN);
		tt_response = (struct tt_query_packet *)skb_put(skb,
				sizeof(struct tt_query_packet) + tt_len);
		tt_response->ttvn = req_ttvn;
		tt_response->tt_data = htons(tt_tot);

		tt_buff = skb->data + sizeof(struct tt_query_packet);
		/* Copy the last orig_node's OGM buffer */
		memcpy(tt_buff, req_dst_orig_node->tt_buff,
		       req_dst_orig_node->tt_buff_len);

		spin_unlock_bh(&req_dst_orig_node->tt_buff_lock);
	} else {
		tt_len = (uint16_t)atomic_read(&req_dst_orig_node->tt_size) *
						sizeof(struct tt_change);
		ttvn = (uint8_t)atomic_read(&req_dst_orig_node->last_ttvn);

		skb = tt_response_fill_table(tt_len, ttvn,
					     bat_priv->tt_global_hash,
					     primary_if, tt_global_valid_entry,
					     req_dst_orig_node);
		if (!skb)
			goto out;

		tt_response = (struct tt_query_packet *)skb->data;
	}

	tt_response->header.packet_type = BAT_TT_QUERY;
	tt_response->header.version = COMPAT_VERSION;
	tt_response->header.ttl = TTL;
	memcpy(tt_response->src, req_dst_orig_node->orig, ETH_ALEN);
	memcpy(tt_response->dst, tt_request->src, ETH_ALEN);
	tt_response->flags = TT_RESPONSE;

	if (full_table)
		tt_response->flags |= TT_FULL_TABLE;

	bat_dbg(DBG_TT, bat_priv,
		"Sending TT_RESPONSE %pM via %pM for %pM (ttvn: %u)\n",
		res_dst_orig_node->orig, neigh_node->addr,
		req_dst_orig_node->orig, req_ttvn);

	send_skb_packet(skb, neigh_node->if_incoming, neigh_node->addr);
=======
	/* TT fragmentation hasn't been implemented yet, so send as many
	 * TT entries fit a single packet as possible only
	 */
	if (!full_table) {
		spin_lock_bh(&req_dst_orig_node->tt_buff_lock);
		tt_len = req_dst_orig_node->tt_buff_len;

		tvlv_len = batadv_tt_prepare_tvlv_global_data(req_dst_orig_node,
							      &tvlv_tt_data,
							      &tt_change,
							      &tt_len);
		if (!tt_len)
			goto unlock;

		/* Copy the last orig_node's OGM buffer */
		memcpy(tt_change, req_dst_orig_node->tt_buff,
		       req_dst_orig_node->tt_buff_len);
		spin_unlock_bh(&req_dst_orig_node->tt_buff_lock);
	} else {
		/* allocate the tvlv, put the tt_data and all the tt_vlan_data
		 * in the initial part
		 */
		tt_len = -1;
		tvlv_len = batadv_tt_prepare_tvlv_global_data(req_dst_orig_node,
							      &tvlv_tt_data,
							      &tt_change,
							      &tt_len);
		if (!tt_len)
			goto out;

		/* fill the rest of the tvlv with the real TT entries */
		batadv_tt_tvlv_generate(bat_priv, bat_priv->tt.global_hash,
					tt_change, tt_len,
					batadv_tt_global_valid,
					req_dst_orig_node);
	}

	/* Don't send the response, if larger than fragmented packet. */
	tt_len = sizeof(struct batadv_unicast_tvlv_packet) + tvlv_len;
	if (tt_len > atomic_read(&bat_priv->packet_size_max)) {
		net_ratelimited_function(batadv_info, bat_priv->soft_iface,
					 "Ignoring TT_REQUEST from %pM; Response size exceeds max packet size.\n",
					 res_dst_orig_node->orig);
		goto out;
	}

	tvlv_tt_data->flags = BATADV_TT_RESPONSE;
	tvlv_tt_data->ttvn = req_ttvn;

	if (full_table)
		tvlv_tt_data->flags |= BATADV_TT_FULL_TABLE;

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Sending TT_RESPONSE %pM for %pM [%c] (ttvn: %u)\n",
		   res_dst_orig_node->orig, req_dst_orig_node->orig,
		   full_table ? 'F' : '.', req_ttvn);

	batadv_inc_counter(bat_priv, BATADV_CNT_TT_RESPONSE_TX);

	batadv_tvlv_unicast_send(bat_priv, req_dst_orig_node->orig,
				 req_src, BATADV_TVLV_TT, 1, tvlv_tt_data,
				 tvlv_len);

>>>>>>> refs/remotes/origin/master
	ret = true;
	goto out;

unlock:
	spin_unlock_bh(&req_dst_orig_node->tt_buff_lock);

out:
	if (res_dst_orig_node)
<<<<<<< HEAD
		orig_node_free_ref(res_dst_orig_node);
	if (req_dst_orig_node)
		orig_node_free_ref(req_dst_orig_node);
	if (neigh_node)
		neigh_node_free_ref(neigh_node);
	if (primary_if)
		hardif_free_ref(primary_if);
	if (!ret)
		kfree_skb(skb);
	return ret;

}
static bool send_my_tt_response(struct bat_priv *bat_priv,
				struct tt_query_packet *tt_request)
{
	struct orig_node *orig_node = NULL;
	struct neigh_node *neigh_node = NULL;
	struct hard_iface *primary_if = NULL;
	uint8_t my_ttvn, req_ttvn, ttvn;
	int ret = false;
	unsigned char *tt_buff;
	bool full_table;
	uint16_t tt_len, tt_tot;
	struct sk_buff *skb = NULL;
	struct tt_query_packet *tt_response;

	bat_dbg(DBG_TT, bat_priv,
		"Received TT_REQUEST from %pM for ttvn: %u (me) [%c]\n",
		tt_request->src, tt_request->ttvn,
		(tt_request->flags & TT_FULL_TABLE ? 'F' : '.'));


	my_ttvn = (uint8_t)atomic_read(&bat_priv->ttvn);
	req_ttvn = tt_request->ttvn;

	orig_node = orig_hash_find(bat_priv, tt_request->src);
	if (!orig_node)
		goto out;

	neigh_node = orig_node_get_router(orig_node);
	if (!neigh_node)
		goto out;

	primary_if = primary_if_get_selected(bat_priv);
=======
		batadv_orig_node_free_ref(res_dst_orig_node);
	if (req_dst_orig_node)
		batadv_orig_node_free_ref(req_dst_orig_node);
	kfree(tvlv_tt_data);
	return ret;
}

/**
 * batadv_send_my_tt_response - send reply to tt request concerning this node's
 *  translation table
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_data: tt data containing the tt request information
 * @req_src: mac address of tt request sender
 *
 * Returns true if tt request reply was sent, false otherwise.
 */
static bool batadv_send_my_tt_response(struct batadv_priv *bat_priv,
				       struct batadv_tvlv_tt_data *tt_data,
				       uint8_t *req_src)
{
	struct batadv_tvlv_tt_data *tvlv_tt_data = NULL;
	struct batadv_hard_iface *primary_if = NULL;
	struct batadv_tvlv_tt_change *tt_change;
	struct batadv_orig_node *orig_node;
	uint8_t my_ttvn, req_ttvn;
	uint16_t tvlv_len;
	bool full_table;
	int32_t tt_len;

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Received TT_REQUEST from %pM for ttvn: %u (me) [%c]\n",
		   req_src, tt_data->ttvn,
		   (tt_data->flags & BATADV_TT_FULL_TABLE ? 'F' : '.'));

	spin_lock_bh(&bat_priv->tt.commit_lock);

	my_ttvn = (uint8_t)atomic_read(&bat_priv->tt.vn);
	req_ttvn = tt_data->ttvn;

	orig_node = batadv_orig_hash_find(bat_priv, req_src);
	if (!orig_node)
		goto out;

	primary_if = batadv_primary_if_get_selected(bat_priv);
>>>>>>> refs/remotes/origin/master
	if (!primary_if)
		goto out;

	/* If the full table has been explicitly requested or the gap
<<<<<<< HEAD
	 * is too big send the whole local translation table */
	if (tt_request->flags & TT_FULL_TABLE || my_ttvn != req_ttvn ||
	    !bat_priv->tt_buff)
=======
	 * is too big send the whole local translation table
	 */
	if (tt_data->flags & BATADV_TT_FULL_TABLE || my_ttvn != req_ttvn ||
	    !bat_priv->tt.last_changeset)
>>>>>>> refs/remotes/origin/master
		full_table = true;
	else
		full_table = false;

<<<<<<< HEAD
	/* In this version, fragmentation is not implemented, then
	 * I'll send only one packet with as much TT entries as I can */
	if (!full_table) {
		spin_lock_bh(&bat_priv->tt_buff_lock);
		tt_len = bat_priv->tt_buff_len;
		tt_tot = tt_len / sizeof(struct tt_change);

		skb = dev_alloc_skb(sizeof(struct tt_query_packet) +
				    tt_len + ETH_HLEN);
		if (!skb)
			goto unlock;

		skb_reserve(skb, ETH_HLEN);
		tt_response = (struct tt_query_packet *)skb_put(skb,
				sizeof(struct tt_query_packet) + tt_len);
		tt_response->ttvn = req_ttvn;
		tt_response->tt_data = htons(tt_tot);

		tt_buff = skb->data + sizeof(struct tt_query_packet);
		memcpy(tt_buff, bat_priv->tt_buff,
		       bat_priv->tt_buff_len);
		spin_unlock_bh(&bat_priv->tt_buff_lock);
	} else {
		tt_len = (uint16_t)atomic_read(&bat_priv->num_local_tt) *
						sizeof(struct tt_change);
		ttvn = (uint8_t)atomic_read(&bat_priv->ttvn);

		skb = tt_response_fill_table(tt_len, ttvn,
					     bat_priv->tt_local_hash,
					     primary_if, tt_local_valid_entry,
					     NULL);
		if (!skb)
			goto out;

		tt_response = (struct tt_query_packet *)skb->data;
	}

	tt_response->header.packet_type = BAT_TT_QUERY;
	tt_response->header.version = COMPAT_VERSION;
	tt_response->header.ttl = TTL;
	memcpy(tt_response->src, primary_if->net_dev->dev_addr, ETH_ALEN);
	memcpy(tt_response->dst, tt_request->src, ETH_ALEN);
	tt_response->flags = TT_RESPONSE;

	if (full_table)
		tt_response->flags |= TT_FULL_TABLE;

	bat_dbg(DBG_TT, bat_priv,
		"Sending TT_RESPONSE to %pM via %pM [%c]\n",
		orig_node->orig, neigh_node->addr,
		(tt_response->flags & TT_FULL_TABLE ? 'F' : '.'));

	send_skb_packet(skb, neigh_node->if_incoming, neigh_node->addr);
	ret = true;
	goto out;

unlock:
	spin_unlock_bh(&bat_priv->tt_buff_lock);
out:
	if (orig_node)
		orig_node_free_ref(orig_node);
	if (neigh_node)
		neigh_node_free_ref(neigh_node);
	if (primary_if)
		hardif_free_ref(primary_if);
	if (!ret)
		kfree_skb(skb);
	/* This packet was for me, so it doesn't need to be re-routed */
	return true;
}

bool send_tt_response(struct bat_priv *bat_priv,
		      struct tt_query_packet *tt_request)
{
	if (is_my_mac(tt_request->dst))
		return send_my_tt_response(bat_priv, tt_request);
	else
		return send_other_tt_response(bat_priv, tt_request);
}

static void _tt_update_changes(struct bat_priv *bat_priv,
			       struct orig_node *orig_node,
			       struct tt_change *tt_change,
			       uint16_t tt_num_changes, uint8_t ttvn)
{
	int i;

	for (i = 0; i < tt_num_changes; i++) {
		if ((tt_change + i)->flags & TT_CLIENT_DEL)
			tt_global_del(bat_priv, orig_node,
				      (tt_change + i)->addr,
				      "tt removed by changes",
				      (tt_change + i)->flags & TT_CLIENT_ROAM);
		else
			if (!tt_global_add(bat_priv, orig_node,
					   (tt_change + i)->addr, ttvn, false,
					   (tt_change + i)->flags &
							TT_CLIENT_WIFI))
=======
	/* TT fragmentation hasn't been implemented yet, so send as many
	 * TT entries fit a single packet as possible only
	 */
	if (!full_table) {
		spin_lock_bh(&bat_priv->tt.last_changeset_lock);

		tt_len = bat_priv->tt.last_changeset_len;
		tvlv_len = batadv_tt_prepare_tvlv_local_data(bat_priv,
							     &tvlv_tt_data,
							     &tt_change,
							     &tt_len);
		if (!tt_len)
			goto unlock;

		/* Copy the last orig_node's OGM buffer */
		memcpy(tt_change, bat_priv->tt.last_changeset,
		       bat_priv->tt.last_changeset_len);
		spin_unlock_bh(&bat_priv->tt.last_changeset_lock);
	} else {
		req_ttvn = (uint8_t)atomic_read(&bat_priv->tt.vn);

		/* allocate the tvlv, put the tt_data and all the tt_vlan_data
		 * in the initial part
		 */
		tt_len = -1;
		tvlv_len = batadv_tt_prepare_tvlv_local_data(bat_priv,
							     &tvlv_tt_data,
							     &tt_change,
							     &tt_len);
		if (!tt_len)
			goto out;

		/* fill the rest of the tvlv with the real TT entries */
		batadv_tt_tvlv_generate(bat_priv, bat_priv->tt.local_hash,
					tt_change, tt_len,
					batadv_tt_local_valid, NULL);
	}

	tvlv_tt_data->flags = BATADV_TT_RESPONSE;
	tvlv_tt_data->ttvn = req_ttvn;

	if (full_table)
		tvlv_tt_data->flags |= BATADV_TT_FULL_TABLE;

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Sending TT_RESPONSE to %pM [%c] (ttvn: %u)\n",
		   orig_node->orig, full_table ? 'F' : '.', req_ttvn);

	batadv_inc_counter(bat_priv, BATADV_CNT_TT_RESPONSE_TX);

	batadv_tvlv_unicast_send(bat_priv, primary_if->net_dev->dev_addr,
				 req_src, BATADV_TVLV_TT, 1, tvlv_tt_data,
				 tvlv_len);

	goto out;

unlock:
	spin_unlock_bh(&bat_priv->tt.last_changeset_lock);
out:
	spin_unlock_bh(&bat_priv->tt.commit_lock);
	if (orig_node)
		batadv_orig_node_free_ref(orig_node);
	if (primary_if)
		batadv_hardif_free_ref(primary_if);
	kfree(tvlv_tt_data);
	/* The packet was for this host, so it doesn't need to be re-routed */
	return true;
}

/**
 * batadv_send_tt_response - send reply to tt request
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_data: tt data containing the tt request information
 * @req_src: mac address of tt request sender
 * @req_dst: mac address of tt request recipient
 *
 * Returns true if tt request reply was sent, false otherwise.
 */
static bool batadv_send_tt_response(struct batadv_priv *bat_priv,
				    struct batadv_tvlv_tt_data *tt_data,
				    uint8_t *req_src, uint8_t *req_dst)
{
	if (batadv_is_my_mac(bat_priv, req_dst))
		return batadv_send_my_tt_response(bat_priv, tt_data, req_src);
	else
		return batadv_send_other_tt_response(bat_priv, tt_data,
						     req_src, req_dst);
}

static void _batadv_tt_update_changes(struct batadv_priv *bat_priv,
				      struct batadv_orig_node *orig_node,
				      struct batadv_tvlv_tt_change *tt_change,
				      uint16_t tt_num_changes, uint8_t ttvn)
{
	int i;
	int roams;

	for (i = 0; i < tt_num_changes; i++) {
		if ((tt_change + i)->flags & BATADV_TT_CLIENT_DEL) {
			roams = (tt_change + i)->flags & BATADV_TT_CLIENT_ROAM;
			batadv_tt_global_del(bat_priv, orig_node,
					     (tt_change + i)->addr,
					     ntohs((tt_change + i)->vid),
					     "tt removed by changes",
					     roams);
		} else {
			if (!batadv_tt_global_add(bat_priv, orig_node,
						  (tt_change + i)->addr,
						  ntohs((tt_change + i)->vid),
						  (tt_change + i)->flags, ttvn))
>>>>>>> refs/remotes/origin/master
				/* In case of problem while storing a
				 * global_entry, we stop the updating
				 * procedure without committing the
				 * ttvn change. This will avoid to send
				 * corrupted data on tt_request
				 */
				return;
<<<<<<< HEAD
=======
		}
>>>>>>> refs/remotes/origin/master
	}
	orig_node->tt_initialised = true;
}

<<<<<<< HEAD
static void tt_fill_gtable(struct bat_priv *bat_priv,
			   struct tt_query_packet *tt_response)
{
	struct orig_node *orig_node = NULL;

	orig_node = orig_hash_find(bat_priv, tt_response->src);
=======
static void batadv_tt_fill_gtable(struct batadv_priv *bat_priv,
				  struct batadv_tvlv_tt_change *tt_change,
				  uint8_t ttvn, uint8_t *resp_src,
				  uint16_t num_entries)
{
	struct batadv_orig_node *orig_node;

	orig_node = batadv_orig_hash_find(bat_priv, resp_src);
>>>>>>> refs/remotes/origin/master
	if (!orig_node)
		goto out;

	/* Purge the old table first.. */
<<<<<<< HEAD
	tt_global_del_orig(bat_priv, orig_node, "Received full table");

	_tt_update_changes(bat_priv, orig_node,
			   (struct tt_change *)(tt_response + 1),
			   tt_response->tt_data, tt_response->ttvn);
=======
	batadv_tt_global_del_orig(bat_priv, orig_node, -1,
				  "Received full table");

	_batadv_tt_update_changes(bat_priv, orig_node, tt_change, num_entries,
				  ttvn);
>>>>>>> refs/remotes/origin/master

	spin_lock_bh(&orig_node->tt_buff_lock);
	kfree(orig_node->tt_buff);
	orig_node->tt_buff_len = 0;
	orig_node->tt_buff = NULL;
	spin_unlock_bh(&orig_node->tt_buff_lock);

<<<<<<< HEAD
	atomic_set(&orig_node->last_ttvn, tt_response->ttvn);

out:
	if (orig_node)
		orig_node_free_ref(orig_node);
}

static void tt_update_changes(struct bat_priv *bat_priv,
			      struct orig_node *orig_node,
			      uint16_t tt_num_changes, uint8_t ttvn,
			      struct tt_change *tt_change)
{
	_tt_update_changes(bat_priv, orig_node, tt_change, tt_num_changes,
			   ttvn);

	tt_save_orig_buffer(bat_priv, orig_node, (unsigned char *)tt_change,
			    tt_num_changes);
	atomic_set(&orig_node->last_ttvn, ttvn);
}

bool is_my_client(struct bat_priv *bat_priv, const uint8_t *addr)
{
	struct tt_local_entry *tt_local_entry = NULL;
	bool ret = false;

	tt_local_entry = tt_local_hash_find(bat_priv, addr);
	if (!tt_local_entry)
		goto out;
	/* Check if the client has been logically deleted (but is kept for
	 * consistency purpose) */
	if (tt_local_entry->common.flags & TT_CLIENT_PENDING)
=======
	atomic_set(&orig_node->last_ttvn, ttvn);

out:
	if (orig_node)
		batadv_orig_node_free_ref(orig_node);
}

static void batadv_tt_update_changes(struct batadv_priv *bat_priv,
				     struct batadv_orig_node *orig_node,
				     uint16_t tt_num_changes, uint8_t ttvn,
				     struct batadv_tvlv_tt_change *tt_change)
{
	_batadv_tt_update_changes(bat_priv, orig_node, tt_change,
				  tt_num_changes, ttvn);

	batadv_tt_save_orig_buffer(bat_priv, orig_node, tt_change,
				   batadv_tt_len(tt_num_changes));
	atomic_set(&orig_node->last_ttvn, ttvn);
}

/**
 * batadv_is_my_client - check if a client is served by the local node
 * @bat_priv: the bat priv with all the soft interface information
 * @addr: the mac adress of the client to check
 * @vid: VLAN identifier
 *
 * Returns true if the client is served by this node, false otherwise.
 */
bool batadv_is_my_client(struct batadv_priv *bat_priv, const uint8_t *addr,
			 unsigned short vid)
{
	struct batadv_tt_local_entry *tt_local_entry;
	bool ret = false;

	tt_local_entry = batadv_tt_local_hash_find(bat_priv, addr, vid);
	if (!tt_local_entry)
		goto out;
	/* Check if the client has been logically deleted (but is kept for
	 * consistency purpose)
	 */
	if ((tt_local_entry->common.flags & BATADV_TT_CLIENT_PENDING) ||
	    (tt_local_entry->common.flags & BATADV_TT_CLIENT_ROAM))
>>>>>>> refs/remotes/origin/master
		goto out;
	ret = true;
out:
	if (tt_local_entry)
<<<<<<< HEAD
		tt_local_entry_free_ref(tt_local_entry);
	return ret;
}

void handle_tt_response(struct bat_priv *bat_priv,
			struct tt_query_packet *tt_response)
{
	struct tt_req_node *node, *safe;
	struct orig_node *orig_node = NULL;

	bat_dbg(DBG_TT, bat_priv,
		"Received TT_RESPONSE from %pM for ttvn %d t_size: %d [%c]\n",
		tt_response->src, tt_response->ttvn, tt_response->tt_data,
		(tt_response->flags & TT_FULL_TABLE ? 'F' : '.'));

	orig_node = orig_hash_find(bat_priv, tt_response->src);
	if (!orig_node)
		goto out;

	if (tt_response->flags & TT_FULL_TABLE)
		tt_fill_gtable(bat_priv, tt_response);
	else
		tt_update_changes(bat_priv, orig_node, tt_response->tt_data,
				  tt_response->ttvn,
				  (struct tt_change *)(tt_response + 1));

	/* Delete the tt_req_node from pending tt_requests list */
	spin_lock_bh(&bat_priv->tt_req_list_lock);
	list_for_each_entry_safe(node, safe, &bat_priv->tt_req_list, list) {
		if (!compare_eth(node->addr, tt_response->src))
=======
		batadv_tt_local_entry_free_ref(tt_local_entry);
	return ret;
}

/**
 * batadv_handle_tt_response - process incoming tt reply
 * @bat_priv: the bat priv with all the soft interface information
 * @tt_data: tt data containing the tt request information
 * @resp_src: mac address of tt reply sender
 * @num_entries: number of tt change entries appended to the tt data
 */
static void batadv_handle_tt_response(struct batadv_priv *bat_priv,
				      struct batadv_tvlv_tt_data *tt_data,
				      uint8_t *resp_src, uint16_t num_entries)
{
	struct batadv_tt_req_node *node, *safe;
	struct batadv_orig_node *orig_node = NULL;
	struct batadv_tvlv_tt_change *tt_change;
	uint8_t *tvlv_ptr = (uint8_t *)tt_data;
	uint16_t change_offset;

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Received TT_RESPONSE from %pM for ttvn %d t_size: %d [%c]\n",
		   resp_src, tt_data->ttvn, num_entries,
		   (tt_data->flags & BATADV_TT_FULL_TABLE ? 'F' : '.'));

	orig_node = batadv_orig_hash_find(bat_priv, resp_src);
	if (!orig_node)
		goto out;

	spin_lock_bh(&orig_node->tt_lock);

	change_offset = sizeof(struct batadv_tvlv_tt_vlan_data);
	change_offset *= ntohs(tt_data->num_vlan);
	change_offset += sizeof(*tt_data);
	tvlv_ptr += change_offset;

	tt_change = (struct batadv_tvlv_tt_change *)tvlv_ptr;
	if (tt_data->flags & BATADV_TT_FULL_TABLE) {
		batadv_tt_fill_gtable(bat_priv, tt_change, tt_data->ttvn,
				      resp_src, num_entries);
	} else {
		batadv_tt_update_changes(bat_priv, orig_node, num_entries,
					 tt_data->ttvn, tt_change);
	}

	/* Recalculate the CRC for this orig_node and store it */
	batadv_tt_global_update_crc(bat_priv, orig_node);

	spin_unlock_bh(&orig_node->tt_lock);

	/* Delete the tt_req_node from pending tt_requests list */
	spin_lock_bh(&bat_priv->tt.req_list_lock);
	list_for_each_entry_safe(node, safe, &bat_priv->tt.req_list, list) {
		if (!batadv_compare_eth(node->addr, resp_src))
>>>>>>> refs/remotes/origin/master
			continue;
		list_del(&node->list);
		kfree(node);
	}
<<<<<<< HEAD
	spin_unlock_bh(&bat_priv->tt_req_list_lock);

	/* Recalculate the CRC for this orig_node and store it */
	orig_node->tt_crc = tt_global_crc(bat_priv, orig_node);
	/* Roaming phase is over: tables are in sync again. I can
	 * unset the flag */
	orig_node->tt_poss_change = false;
out:
	if (orig_node)
		orig_node_free_ref(orig_node);
}

int tt_init(struct bat_priv *bat_priv)
{
	if (!tt_local_init(bat_priv))
		return 0;

	if (!tt_global_init(bat_priv))
		return 0;

	tt_start_timer(bat_priv);

	return 1;
}

static void tt_roam_list_free(struct bat_priv *bat_priv)
{
	struct tt_roam_node *node, *safe;

	spin_lock_bh(&bat_priv->tt_roam_list_lock);

	list_for_each_entry_safe(node, safe, &bat_priv->tt_roam_list, list) {
=======

	spin_unlock_bh(&bat_priv->tt.req_list_lock);
out:
	if (orig_node)
		batadv_orig_node_free_ref(orig_node);
}

static void batadv_tt_roam_list_free(struct batadv_priv *bat_priv)
{
	struct batadv_tt_roam_node *node, *safe;

	spin_lock_bh(&bat_priv->tt.roam_list_lock);

	list_for_each_entry_safe(node, safe, &bat_priv->tt.roam_list, list) {
>>>>>>> refs/remotes/origin/master
		list_del(&node->list);
		kfree(node);
	}

<<<<<<< HEAD
	spin_unlock_bh(&bat_priv->tt_roam_list_lock);
}

static void tt_roam_purge(struct bat_priv *bat_priv)
{
	struct tt_roam_node *node, *safe;

	spin_lock_bh(&bat_priv->tt_roam_list_lock);
	list_for_each_entry_safe(node, safe, &bat_priv->tt_roam_list, list) {
		if (!has_timed_out(node->first_time, ROAMING_MAX_TIME))
=======
	spin_unlock_bh(&bat_priv->tt.roam_list_lock);
}

static void batadv_tt_roam_purge(struct batadv_priv *bat_priv)
{
	struct batadv_tt_roam_node *node, *safe;

	spin_lock_bh(&bat_priv->tt.roam_list_lock);
	list_for_each_entry_safe(node, safe, &bat_priv->tt.roam_list, list) {
		if (!batadv_has_timed_out(node->first_time,
					  BATADV_ROAMING_MAX_TIME))
>>>>>>> refs/remotes/origin/master
			continue;

		list_del(&node->list);
		kfree(node);
	}
<<<<<<< HEAD
	spin_unlock_bh(&bat_priv->tt_roam_list_lock);
=======
	spin_unlock_bh(&bat_priv->tt.roam_list_lock);
>>>>>>> refs/remotes/origin/master
}

/* This function checks whether the client already reached the
 * maximum number of possible roaming phases. In this case the ROAMING_ADV
 * will not be sent.
 *
<<<<<<< HEAD
 * returns true if the ROAMING_ADV can be sent, false otherwise */
static bool tt_check_roam_count(struct bat_priv *bat_priv,
				uint8_t *client)
{
	struct tt_roam_node *tt_roam_node;
	bool ret = false;

	spin_lock_bh(&bat_priv->tt_roam_list_lock);
	/* The new tt_req will be issued only if I'm not waiting for a
	 * reply from the same orig_node yet */
	list_for_each_entry(tt_roam_node, &bat_priv->tt_roam_list, list) {
		if (!compare_eth(tt_roam_node->addr, client))
			continue;

		if (has_timed_out(tt_roam_node->first_time, ROAMING_MAX_TIME))
			continue;

		if (!atomic_dec_not_zero(&tt_roam_node->counter))
=======
 * returns true if the ROAMING_ADV can be sent, false otherwise
 */
static bool batadv_tt_check_roam_count(struct batadv_priv *bat_priv,
				       uint8_t *client)
{
	struct batadv_tt_roam_node *tt_roam_node;
	bool ret = false;

	spin_lock_bh(&bat_priv->tt.roam_list_lock);
	/* The new tt_req will be issued only if I'm not waiting for a
	 * reply from the same orig_node yet
	 */
	list_for_each_entry(tt_roam_node, &bat_priv->tt.roam_list, list) {
		if (!batadv_compare_eth(tt_roam_node->addr, client))
			continue;

		if (batadv_has_timed_out(tt_roam_node->first_time,
					 BATADV_ROAMING_MAX_TIME))
			continue;

		if (!batadv_atomic_dec_not_zero(&tt_roam_node->counter))
>>>>>>> refs/remotes/origin/master
			/* Sorry, you roamed too many times! */
			goto unlock;
		ret = true;
		break;
	}

	if (!ret) {
		tt_roam_node = kmalloc(sizeof(*tt_roam_node), GFP_ATOMIC);
		if (!tt_roam_node)
			goto unlock;

		tt_roam_node->first_time = jiffies;
<<<<<<< HEAD
		atomic_set(&tt_roam_node->counter, ROAMING_MAX_COUNT - 1);
		memcpy(tt_roam_node->addr, client, ETH_ALEN);

		list_add(&tt_roam_node->list, &bat_priv->tt_roam_list);
=======
		atomic_set(&tt_roam_node->counter,
			   BATADV_ROAMING_MAX_COUNT - 1);
		memcpy(tt_roam_node->addr, client, ETH_ALEN);

		list_add(&tt_roam_node->list, &bat_priv->tt.roam_list);
>>>>>>> refs/remotes/origin/master
		ret = true;
	}

unlock:
<<<<<<< HEAD
	spin_unlock_bh(&bat_priv->tt_roam_list_lock);
	return ret;
}

void send_roam_adv(struct bat_priv *bat_priv, uint8_t *client,
		   struct orig_node *orig_node)
{
	struct neigh_node *neigh_node = NULL;
	struct sk_buff *skb = NULL;
	struct roam_adv_packet *roam_adv_packet;
	int ret = 1;
	struct hard_iface *primary_if;

	/* before going on we have to check whether the client has
	 * already roamed to us too many times */
	if (!tt_check_roam_count(bat_priv, client))
		goto out;

	skb = dev_alloc_skb(sizeof(struct roam_adv_packet) + ETH_HLEN);
	if (!skb)
		goto out;

	skb_reserve(skb, ETH_HLEN);

	roam_adv_packet = (struct roam_adv_packet *)skb_put(skb,
					sizeof(struct roam_adv_packet));

	roam_adv_packet->header.packet_type = BAT_ROAM_ADV;
	roam_adv_packet->header.version = COMPAT_VERSION;
	roam_adv_packet->header.ttl = TTL;
	primary_if = primary_if_get_selected(bat_priv);
	if (!primary_if)
		goto out;
	memcpy(roam_adv_packet->src, primary_if->net_dev->dev_addr, ETH_ALEN);
	hardif_free_ref(primary_if);
	memcpy(roam_adv_packet->dst, orig_node->orig, ETH_ALEN);
	memcpy(roam_adv_packet->client, client, ETH_ALEN);

	neigh_node = orig_node_get_router(orig_node);
	if (!neigh_node)
		goto out;

	bat_dbg(DBG_TT, bat_priv,
		"Sending ROAMING_ADV to %pM (client %pM) via %pM\n",
		orig_node->orig, client, neigh_node->addr);

	send_skb_packet(skb, neigh_node->if_incoming, neigh_node->addr);
	ret = 0;

out:
	if (neigh_node)
		neigh_node_free_ref(neigh_node);
	if (ret)
		kfree_skb(skb);
	return;
}

static void tt_purge(struct work_struct *work)
{
	struct delayed_work *delayed_work =
		container_of(work, struct delayed_work, work);
	struct bat_priv *bat_priv =
		container_of(delayed_work, struct bat_priv, tt_work);

	tt_local_purge(bat_priv);
	tt_global_roam_purge(bat_priv);
	tt_req_purge(bat_priv);
	tt_roam_purge(bat_priv);

	tt_start_timer(bat_priv);
}

void tt_free(struct bat_priv *bat_priv)
{
	cancel_delayed_work_sync(&bat_priv->tt_work);

	tt_local_table_free(bat_priv);
	tt_global_table_free(bat_priv);
	tt_req_list_free(bat_priv);
	tt_changes_list_free(bat_priv);
	tt_roam_list_free(bat_priv);

	kfree(bat_priv->tt_buff);
}

/* This function will enable or disable the specified flags for all the entries
 * in the given hash table and returns the number of modified entries */
static uint16_t tt_set_flags(struct hashtable_t *hash, uint16_t flags,
			     bool enable)
{
	uint32_t i;
	uint16_t changed_num = 0;
	struct hlist_head *head;
	struct hlist_node *node;
	struct tt_common_entry *tt_common_entry;

	if (!hash)
		goto out;
=======
	spin_unlock_bh(&bat_priv->tt.roam_list_lock);
	return ret;
}

/**
 * batadv_send_roam_adv - send a roaming advertisement message
 * @bat_priv: the bat priv with all the soft interface information
 * @client: mac address of the roaming client
 * @vid: VLAN identifier
 * @orig_node: message destination
 *
 * Send a ROAMING_ADV message to the node which was previously serving this
 * client. This is done to inform the node that from now on all traffic destined
 * for this particular roamed client has to be forwarded to the sender of the
 * roaming message.
 */
static void batadv_send_roam_adv(struct batadv_priv *bat_priv, uint8_t *client,
				 unsigned short vid,
				 struct batadv_orig_node *orig_node)
{
	struct batadv_hard_iface *primary_if;
	struct batadv_tvlv_roam_adv tvlv_roam;

	primary_if = batadv_primary_if_get_selected(bat_priv);
	if (!primary_if)
		goto out;

	/* before going on we have to check whether the client has
	 * already roamed to us too many times
	 */
	if (!batadv_tt_check_roam_count(bat_priv, client))
		goto out;

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Sending ROAMING_ADV to %pM (client %pM, vid: %d)\n",
		   orig_node->orig, client, BATADV_PRINT_VID(vid));

	batadv_inc_counter(bat_priv, BATADV_CNT_TT_ROAM_ADV_TX);

	memcpy(tvlv_roam.client, client, sizeof(tvlv_roam.client));
	tvlv_roam.vid = htons(vid);

	batadv_tvlv_unicast_send(bat_priv, primary_if->net_dev->dev_addr,
				 orig_node->orig, BATADV_TVLV_ROAM, 1,
				 &tvlv_roam, sizeof(tvlv_roam));

out:
	if (primary_if)
		batadv_hardif_free_ref(primary_if);
}

static void batadv_tt_purge(struct work_struct *work)
{
	struct delayed_work *delayed_work;
	struct batadv_priv_tt *priv_tt;
	struct batadv_priv *bat_priv;

	delayed_work = container_of(work, struct delayed_work, work);
	priv_tt = container_of(delayed_work, struct batadv_priv_tt, work);
	bat_priv = container_of(priv_tt, struct batadv_priv, tt);

	batadv_tt_local_purge(bat_priv, BATADV_TT_LOCAL_TIMEOUT);
	batadv_tt_global_purge(bat_priv);
	batadv_tt_req_purge(bat_priv);
	batadv_tt_roam_purge(bat_priv);

	queue_delayed_work(batadv_event_workqueue, &bat_priv->tt.work,
			   msecs_to_jiffies(BATADV_TT_WORK_PERIOD));
}

void batadv_tt_free(struct batadv_priv *bat_priv)
{
	batadv_tvlv_container_unregister(bat_priv, BATADV_TVLV_TT, 1);
	batadv_tvlv_handler_unregister(bat_priv, BATADV_TVLV_TT, 1);

	cancel_delayed_work_sync(&bat_priv->tt.work);

	batadv_tt_local_table_free(bat_priv);
	batadv_tt_global_table_free(bat_priv);
	batadv_tt_req_list_free(bat_priv);
	batadv_tt_changes_list_free(bat_priv);
	batadv_tt_roam_list_free(bat_priv);

	kfree(bat_priv->tt.last_changeset);
}

/**
 * batadv_tt_local_set_flags - set or unset the specified flags on the local
 *  table and possibly count them in the TT size
 * @bat_priv: the bat priv with all the soft interface information
 * @flags: the flag to switch
 * @enable: whether to set or unset the flag
 * @count: whether to increase the TT size by the number of changed entries
 */
static void batadv_tt_local_set_flags(struct batadv_priv *bat_priv,
				      uint16_t flags, bool enable, bool count)
{
	struct batadv_hashtable *hash = bat_priv->tt.local_hash;
	struct batadv_tt_common_entry *tt_common_entry;
	uint16_t changed_num = 0;
	struct hlist_head *head;
	uint32_t i;

	if (!hash)
		return;
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];

		rcu_read_lock();
<<<<<<< HEAD
		hlist_for_each_entry_rcu(tt_common_entry, node,
=======
		hlist_for_each_entry_rcu(tt_common_entry,
>>>>>>> refs/remotes/origin/master
					 head, hash_entry) {
			if (enable) {
				if ((tt_common_entry->flags & flags) == flags)
					continue;
				tt_common_entry->flags |= flags;
			} else {
				if (!(tt_common_entry->flags & flags))
					continue;
				tt_common_entry->flags &= ~flags;
			}
			changed_num++;
<<<<<<< HEAD
		}
		rcu_read_unlock();
	}
out:
	return changed_num;
}

/* Purge out all the tt local entries marked with TT_CLIENT_PENDING */
static void tt_local_purge_pending_clients(struct bat_priv *bat_priv)
{
	struct hashtable_t *hash = bat_priv->tt_local_hash;
	struct tt_common_entry *tt_common_entry;
	struct tt_local_entry *tt_local_entry;
	struct hlist_node *node, *node_tmp;
=======

			if (!count)
				continue;

			batadv_tt_local_size_inc(bat_priv,
						 tt_common_entry->vid);
		}
		rcu_read_unlock();
	}
}

/* Purge out all the tt local entries marked with BATADV_TT_CLIENT_PENDING */
static void batadv_tt_local_purge_pending_clients(struct batadv_priv *bat_priv)
{
	struct batadv_hashtable *hash = bat_priv->tt.local_hash;
	struct batadv_tt_common_entry *tt_common;
	struct batadv_tt_local_entry *tt_local;
	struct hlist_node *node_tmp;
>>>>>>> refs/remotes/origin/master
	struct hlist_head *head;
	spinlock_t *list_lock; /* protects write access to the hash lists */
	uint32_t i;

	if (!hash)
		return;

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];
		list_lock = &hash->list_locks[i];

		spin_lock_bh(list_lock);
<<<<<<< HEAD
		hlist_for_each_entry_safe(tt_common_entry, node, node_tmp,
					  head, hash_entry) {
			if (!(tt_common_entry->flags & TT_CLIENT_PENDING))
				continue;

			bat_dbg(DBG_TT, bat_priv,
				"Deleting local tt entry (%pM): pending\n",
				tt_common_entry->addr);

			atomic_dec(&bat_priv->num_local_tt);
			hlist_del_rcu(node);
			tt_local_entry = container_of(tt_common_entry,
						      struct tt_local_entry,
						      common);
			tt_local_entry_free_ref(tt_local_entry);
		}
		spin_unlock_bh(list_lock);
	}

}

void tt_commit_changes(struct bat_priv *bat_priv)
{
	uint16_t changed_num = tt_set_flags(bat_priv->tt_local_hash,
					    TT_CLIENT_NEW, false);
	/* all the reset entries have now to be effectively counted as local
	 * entries */
	atomic_add(changed_num, &bat_priv->num_local_tt);
	tt_local_purge_pending_clients(bat_priv);

	/* Increment the TTVN only once per OGM interval */
	atomic_inc(&bat_priv->ttvn);
	bat_priv->tt_poss_change = false;
}

bool is_ap_isolated(struct bat_priv *bat_priv, uint8_t *src, uint8_t *dst)
{
	struct tt_local_entry *tt_local_entry = NULL;
	struct tt_global_entry *tt_global_entry = NULL;
	bool ret = false;

	if (!atomic_read(&bat_priv->ap_isolation))
		goto out;

	tt_local_entry = tt_local_hash_find(bat_priv, dst);
	if (!tt_local_entry)
		goto out;

	tt_global_entry = tt_global_hash_find(bat_priv, src);
	if (!tt_global_entry)
		goto out;

	if (!_is_ap_isolated(tt_local_entry, tt_global_entry))
=======
		hlist_for_each_entry_safe(tt_common, node_tmp, head,
					  hash_entry) {
			if (!(tt_common->flags & BATADV_TT_CLIENT_PENDING))
				continue;

			batadv_dbg(BATADV_DBG_TT, bat_priv,
				   "Deleting local tt entry (%pM, vid: %d): pending\n",
				   tt_common->addr,
				   BATADV_PRINT_VID(tt_common->vid));

			batadv_tt_local_size_dec(bat_priv, tt_common->vid);
			hlist_del_rcu(&tt_common->hash_entry);
			tt_local = container_of(tt_common,
						struct batadv_tt_local_entry,
						common);
			batadv_tt_local_entry_free_ref(tt_local);
		}
		spin_unlock_bh(list_lock);
	}
}

/**
 * batadv_tt_local_commit_changes_nolock - commit all pending local tt changes
 *  which have been queued in the time since the last commit
 * @bat_priv: the bat priv with all the soft interface information
 *
 * Caller must hold tt->commit_lock.
 */
static void batadv_tt_local_commit_changes_nolock(struct batadv_priv *bat_priv)
{
	if (atomic_read(&bat_priv->tt.local_changes) < 1) {
		if (!batadv_atomic_dec_not_zero(&bat_priv->tt.ogm_append_cnt))
			batadv_tt_tvlv_container_update(bat_priv);
		return;
	}

	batadv_tt_local_set_flags(bat_priv, BATADV_TT_CLIENT_NEW, false, true);

	batadv_tt_local_purge_pending_clients(bat_priv);
	batadv_tt_local_update_crc(bat_priv);

	/* Increment the TTVN only once per OGM interval */
	atomic_inc(&bat_priv->tt.vn);
	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Local changes committed, updating to ttvn %u\n",
		   (uint8_t)atomic_read(&bat_priv->tt.vn));

	/* reset the sending counter */
	atomic_set(&bat_priv->tt.ogm_append_cnt, BATADV_TT_OGM_APPEND_MAX);
	batadv_tt_tvlv_container_update(bat_priv);
}

/**
 * batadv_tt_local_commit_changes - commit all pending local tt changes which
 *  have been queued in the time since the last commit
 * @bat_priv: the bat priv with all the soft interface information
 */
void batadv_tt_local_commit_changes(struct batadv_priv *bat_priv)
{
	spin_lock_bh(&bat_priv->tt.commit_lock);
	batadv_tt_local_commit_changes_nolock(bat_priv);
	spin_unlock_bh(&bat_priv->tt.commit_lock);
}

bool batadv_is_ap_isolated(struct batadv_priv *bat_priv, uint8_t *src,
			   uint8_t *dst, unsigned short vid)
{
	struct batadv_tt_local_entry *tt_local_entry = NULL;
	struct batadv_tt_global_entry *tt_global_entry = NULL;
	struct batadv_softif_vlan *vlan;
	bool ret = false;

	vlan = batadv_softif_vlan_get(bat_priv, vid);
	if (!vlan || !atomic_read(&vlan->ap_isolation))
		goto out;

	tt_local_entry = batadv_tt_local_hash_find(bat_priv, dst, vid);
	if (!tt_local_entry)
		goto out;

	tt_global_entry = batadv_tt_global_hash_find(bat_priv, src, vid);
	if (!tt_global_entry)
		goto out;

	if (!_batadv_is_ap_isolated(tt_local_entry, tt_global_entry))
>>>>>>> refs/remotes/origin/master
		goto out;

	ret = true;

out:
<<<<<<< HEAD
	if (tt_global_entry)
		tt_global_entry_free_ref(tt_global_entry);
	if (tt_local_entry)
		tt_local_entry_free_ref(tt_local_entry);
	return ret;
}

void tt_update_orig(struct bat_priv *bat_priv, struct orig_node *orig_node,
		    const unsigned char *tt_buff, uint8_t tt_num_changes,
		    uint8_t ttvn, uint16_t tt_crc)
{
	uint8_t orig_ttvn = (uint8_t)atomic_read(&orig_node->last_ttvn);
	bool full_table = true;

	/* orig table not initialised AND first diff is in the OGM OR the ttvn
	 * increased by one -> we can apply the attached changes */
	if ((!orig_node->tt_initialised && ttvn == 1) ||
	    ttvn - orig_ttvn == 1) {
		/* the OGM could not contain the changes due to their size or
		 * because they have already been sent TT_OGM_APPEND_MAX times.
		 * In this case send a tt request */
=======
	if (vlan)
		batadv_softif_vlan_free_ref(vlan);
	if (tt_global_entry)
		batadv_tt_global_entry_free_ref(tt_global_entry);
	if (tt_local_entry)
		batadv_tt_local_entry_free_ref(tt_local_entry);
	return ret;
}

/**
 * batadv_tt_update_orig - update global translation table with new tt
 *  information received via ogms
 * @bat_priv: the bat priv with all the soft interface information
 * @orig: the orig_node of the ogm
 * @tt_vlan: pointer to the first tvlv VLAN entry
 * @tt_num_vlan: number of tvlv VLAN entries
 * @tt_change: pointer to the first entry in the TT buffer
 * @tt_num_changes: number of tt changes inside the tt buffer
 * @ttvn: translation table version number of this changeset
 * @tt_crc: crc32 checksum of orig node's translation table
 */
static void batadv_tt_update_orig(struct batadv_priv *bat_priv,
				  struct batadv_orig_node *orig_node,
				  const void *tt_buff, uint16_t tt_num_vlan,
				  struct batadv_tvlv_tt_change *tt_change,
				  uint16_t tt_num_changes, uint8_t ttvn)
{
	uint8_t orig_ttvn = (uint8_t)atomic_read(&orig_node->last_ttvn);
	struct batadv_tvlv_tt_vlan_data *tt_vlan;
	bool full_table = true;

	tt_vlan = (struct batadv_tvlv_tt_vlan_data *)tt_buff;
	/* orig table not initialised AND first diff is in the OGM OR the ttvn
	 * increased by one -> we can apply the attached changes
	 */
	if ((!orig_node->tt_initialised && ttvn == 1) ||
	    ttvn - orig_ttvn == 1) {
		/* the OGM could not contain the changes due to their size or
		 * because they have already been sent BATADV_TT_OGM_APPEND_MAX
		 * times.
		 * In this case send a tt request
		 */
>>>>>>> refs/remotes/origin/master
		if (!tt_num_changes) {
			full_table = false;
			goto request_table;
		}

<<<<<<< HEAD
		tt_update_changes(bat_priv, orig_node, tt_num_changes, ttvn,
				  (struct tt_change *)tt_buff);

		/* Even if we received the precomputed crc with the OGM, we
		 * prefer to recompute it to spot any possible inconsistency
		 * in the global table */
		orig_node->tt_crc = tt_global_crc(bat_priv, orig_node);
=======
		spin_lock_bh(&orig_node->tt_lock);

		tt_change = (struct batadv_tvlv_tt_change *)tt_buff;
		batadv_tt_update_changes(bat_priv, orig_node, tt_num_changes,
					 ttvn, tt_change);

		/* Even if we received the precomputed crc with the OGM, we
		 * prefer to recompute it to spot any possible inconsistency
		 * in the global table
		 */
		batadv_tt_global_update_crc(bat_priv, orig_node);

		spin_unlock_bh(&orig_node->tt_lock);
>>>>>>> refs/remotes/origin/master

		/* The ttvn alone is not enough to guarantee consistency
		 * because a single value could represent different states
		 * (due to the wrap around). Thus a node has to check whether
		 * the resulting table (after applying the changes) is still
		 * consistent or not. E.g. a node could disconnect while its
		 * ttvn is X and reconnect on ttvn = X + TTVN_MAX: in this case
		 * checking the CRC value is mandatory to detect the
<<<<<<< HEAD
		 * inconsistency */
		if (orig_node->tt_crc != tt_crc)
			goto request_table;

		/* Roaming phase is over: tables are in sync again. I can
		 * unset the flag */
		orig_node->tt_poss_change = false;
	} else {
		/* if we missed more than one change or our tables are not
		 * in sync anymore -> request fresh tt data */
		if (!orig_node->tt_initialised || ttvn != orig_ttvn ||
		    orig_node->tt_crc != tt_crc) {
request_table:
			bat_dbg(DBG_TT, bat_priv,
				"TT inconsistency for %pM. Need to retrieve the correct information (ttvn: %u last_ttvn: %u crc: %u last_crc: %u num_changes: %u)\n",
				orig_node->orig, ttvn, orig_ttvn, tt_crc,
				orig_node->tt_crc, tt_num_changes);
			send_tt_request(bat_priv, orig_node, ttvn, tt_crc,
					full_table);
=======
		 * inconsistency
		 */
		if (!batadv_tt_global_check_crc(orig_node, tt_vlan,
						tt_num_vlan))
			goto request_table;
	} else {
		/* if we missed more than one change or our tables are not
		 * in sync anymore -> request fresh tt data
		 */
		if (!orig_node->tt_initialised || ttvn != orig_ttvn ||
		    !batadv_tt_global_check_crc(orig_node, tt_vlan,
						tt_num_vlan)) {
request_table:
			batadv_dbg(BATADV_DBG_TT, bat_priv,
				   "TT inconsistency for %pM. Need to retrieve the correct information (ttvn: %u last_ttvn: %u num_changes: %u)\n",
				   orig_node->orig, ttvn, orig_ttvn,
				   tt_num_changes);
			batadv_send_tt_request(bat_priv, orig_node, ttvn,
					       tt_vlan, tt_num_vlan,
					       full_table);
>>>>>>> refs/remotes/origin/master
			return;
		}
	}
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======

/**
 * batadv_tt_global_client_is_roaming - check if a client is marked as roaming
 * @bat_priv: the bat priv with all the soft interface information
 * @addr: the mac address of the client to check
 * @vid: VLAN identifier
 *
 * Returns true if we know that the client has moved from its old originator
 * to another one. This entry is still kept for consistency purposes and will be
 * deleted later by a DEL or because of timeout
 */
bool batadv_tt_global_client_is_roaming(struct batadv_priv *bat_priv,
					uint8_t *addr, unsigned short vid)
{
	struct batadv_tt_global_entry *tt_global_entry;
	bool ret = false;

	tt_global_entry = batadv_tt_global_hash_find(bat_priv, addr, vid);
	if (!tt_global_entry)
		goto out;

	ret = tt_global_entry->common.flags & BATADV_TT_CLIENT_ROAM;
	batadv_tt_global_entry_free_ref(tt_global_entry);
out:
	return ret;
}

/**
 * batadv_tt_local_client_is_roaming - tells whether the client is roaming
 * @bat_priv: the bat priv with all the soft interface information
 * @addr: the mac address of the local client to query
 * @vid: VLAN identifier
 *
 * Returns true if the local client is known to be roaming (it is not served by
 * this node anymore) or not. If yes, the client is still present in the table
 * to keep the latter consistent with the node TTVN
 */
bool batadv_tt_local_client_is_roaming(struct batadv_priv *bat_priv,
				       uint8_t *addr, unsigned short vid)
{
	struct batadv_tt_local_entry *tt_local_entry;
	bool ret = false;

	tt_local_entry = batadv_tt_local_hash_find(bat_priv, addr, vid);
	if (!tt_local_entry)
		goto out;

	ret = tt_local_entry->common.flags & BATADV_TT_CLIENT_ROAM;
	batadv_tt_local_entry_free_ref(tt_local_entry);
out:
	return ret;
}

bool batadv_tt_add_temporary_global_entry(struct batadv_priv *bat_priv,
					  struct batadv_orig_node *orig_node,
					  const unsigned char *addr,
					  unsigned short vid)
{
	bool ret = false;

	if (!batadv_tt_global_add(bat_priv, orig_node, addr, vid,
				  BATADV_TT_CLIENT_TEMP,
				  atomic_read(&orig_node->last_ttvn)))
		goto out;

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Added temporary global client (addr: %pM, vid: %d, orig: %pM)\n",
		   addr, BATADV_PRINT_VID(vid), orig_node->orig);
	ret = true;
out:
	return ret;
}

/**
 * batadv_tt_local_resize_to_mtu - resize the local translation table fit the
 *  maximum packet size that can be transported through the mesh
 * @soft_iface: netdev struct of the mesh interface
 *
 * Remove entries older than 'timeout' and half timeout if more entries need
 * to be removed.
 */
void batadv_tt_local_resize_to_mtu(struct net_device *soft_iface)
{
	struct batadv_priv *bat_priv = netdev_priv(soft_iface);
	int packet_size_max = atomic_read(&bat_priv->packet_size_max);
	int table_size, timeout = BATADV_TT_LOCAL_TIMEOUT / 2;
	bool reduced = false;

	spin_lock_bh(&bat_priv->tt.commit_lock);

	while (true) {
		table_size = batadv_tt_local_table_transmit_size(bat_priv);
		if (packet_size_max >= table_size)
			break;

		batadv_tt_local_purge(bat_priv, timeout);
		batadv_tt_local_purge_pending_clients(bat_priv);

		timeout /= 2;
		reduced = true;
		net_ratelimited_function(batadv_info, soft_iface,
					 "Forced to purge local tt entries to fit new maximum fragment MTU (%i)\n",
					 packet_size_max);
	}

	/* commit these changes immediately, to avoid synchronization problem
	 * with the TTVN
	 */
	if (reduced)
		batadv_tt_local_commit_changes_nolock(bat_priv);

	spin_unlock_bh(&bat_priv->tt.commit_lock);
}

/**
 * batadv_tt_tvlv_ogm_handler_v1 - process incoming tt tvlv container
 * @bat_priv: the bat priv with all the soft interface information
 * @orig: the orig_node of the ogm
 * @flags: flags indicating the tvlv state (see batadv_tvlv_handler_flags)
 * @tvlv_value: tvlv buffer containing the gateway data
 * @tvlv_value_len: tvlv buffer length
 */
static void batadv_tt_tvlv_ogm_handler_v1(struct batadv_priv *bat_priv,
					  struct batadv_orig_node *orig,
					  uint8_t flags, void *tvlv_value,
					  uint16_t tvlv_value_len)
{
	struct batadv_tvlv_tt_vlan_data *tt_vlan;
	struct batadv_tvlv_tt_change *tt_change;
	struct batadv_tvlv_tt_data *tt_data;
	uint16_t num_entries, num_vlan;

	if (tvlv_value_len < sizeof(*tt_data))
		return;

	tt_data = (struct batadv_tvlv_tt_data *)tvlv_value;
	tvlv_value_len -= sizeof(*tt_data);

	num_vlan = ntohs(tt_data->num_vlan);

	if (tvlv_value_len < sizeof(*tt_vlan) * num_vlan)
		return;

	tt_vlan = (struct batadv_tvlv_tt_vlan_data *)(tt_data + 1);
	tt_change = (struct batadv_tvlv_tt_change *)(tt_vlan + num_vlan);
	tvlv_value_len -= sizeof(*tt_vlan) * num_vlan;

	num_entries = batadv_tt_entries(tvlv_value_len);

	batadv_tt_update_orig(bat_priv, orig, tt_vlan, num_vlan, tt_change,
			      num_entries, tt_data->ttvn);
}

/**
 * batadv_tt_tvlv_unicast_handler_v1 - process incoming (unicast) tt tvlv
 *  container
 * @bat_priv: the bat priv with all the soft interface information
 * @src: mac address of tt tvlv sender
 * @dst: mac address of tt tvlv recipient
 * @tvlv_value: tvlv buffer containing the tt data
 * @tvlv_value_len: tvlv buffer length
 *
 * Returns NET_RX_DROP if the tt tvlv is to be re-routed, NET_RX_SUCCESS
 * otherwise.
 */
static int batadv_tt_tvlv_unicast_handler_v1(struct batadv_priv *bat_priv,
					     uint8_t *src, uint8_t *dst,
					     void *tvlv_value,
					     uint16_t tvlv_value_len)
{
	struct batadv_tvlv_tt_data *tt_data;
	uint16_t tt_vlan_len, tt_num_entries;
	char tt_flag;
	bool ret;

	if (tvlv_value_len < sizeof(*tt_data))
		return NET_RX_SUCCESS;

	tt_data = (struct batadv_tvlv_tt_data *)tvlv_value;
	tvlv_value_len -= sizeof(*tt_data);

	tt_vlan_len = sizeof(struct batadv_tvlv_tt_vlan_data);
	tt_vlan_len *= ntohs(tt_data->num_vlan);

	if (tvlv_value_len < tt_vlan_len)
		return NET_RX_SUCCESS;

	tvlv_value_len -= tt_vlan_len;
	tt_num_entries = batadv_tt_entries(tvlv_value_len);

	switch (tt_data->flags & BATADV_TT_DATA_TYPE_MASK) {
	case BATADV_TT_REQUEST:
		batadv_inc_counter(bat_priv, BATADV_CNT_TT_REQUEST_RX);

		/* If this node cannot provide a TT response the tt_request is
		 * forwarded
		 */
		ret = batadv_send_tt_response(bat_priv, tt_data, src, dst);
		if (!ret) {
			if (tt_data->flags & BATADV_TT_FULL_TABLE)
				tt_flag = 'F';
			else
				tt_flag = '.';

			batadv_dbg(BATADV_DBG_TT, bat_priv,
				   "Routing TT_REQUEST to %pM [%c]\n",
				   dst, tt_flag);
			/* tvlv API will re-route the packet */
			return NET_RX_DROP;
		}
		break;
	case BATADV_TT_RESPONSE:
		batadv_inc_counter(bat_priv, BATADV_CNT_TT_RESPONSE_RX);

		if (batadv_is_my_mac(bat_priv, dst)) {
			batadv_handle_tt_response(bat_priv, tt_data,
						  src, tt_num_entries);
			return NET_RX_SUCCESS;
		}

		if (tt_data->flags & BATADV_TT_FULL_TABLE)
			tt_flag =  'F';
		else
			tt_flag = '.';

		batadv_dbg(BATADV_DBG_TT, bat_priv,
			   "Routing TT_RESPONSE to %pM [%c]\n", dst, tt_flag);

		/* tvlv API will re-route the packet */
		return NET_RX_DROP;
	}

	return NET_RX_SUCCESS;
}

/**
 * batadv_roam_tvlv_unicast_handler_v1 - process incoming tt roam tvlv container
 * @bat_priv: the bat priv with all the soft interface information
 * @src: mac address of tt tvlv sender
 * @dst: mac address of tt tvlv recipient
 * @tvlv_value: tvlv buffer containing the tt data
 * @tvlv_value_len: tvlv buffer length
 *
 * Returns NET_RX_DROP if the tt roam tvlv is to be re-routed, NET_RX_SUCCESS
 * otherwise.
 */
static int batadv_roam_tvlv_unicast_handler_v1(struct batadv_priv *bat_priv,
					       uint8_t *src, uint8_t *dst,
					       void *tvlv_value,
					       uint16_t tvlv_value_len)
{
	struct batadv_tvlv_roam_adv *roaming_adv;
	struct batadv_orig_node *orig_node = NULL;

	/* If this node is not the intended recipient of the
	 * roaming advertisement the packet is forwarded
	 * (the tvlv API will re-route the packet).
	 */
	if (!batadv_is_my_mac(bat_priv, dst))
		return NET_RX_DROP;

	if (tvlv_value_len < sizeof(*roaming_adv))
		goto out;

	orig_node = batadv_orig_hash_find(bat_priv, src);
	if (!orig_node)
		goto out;

	batadv_inc_counter(bat_priv, BATADV_CNT_TT_ROAM_ADV_RX);
	roaming_adv = (struct batadv_tvlv_roam_adv *)tvlv_value;

	batadv_dbg(BATADV_DBG_TT, bat_priv,
		   "Received ROAMING_ADV from %pM (client %pM)\n",
		   src, roaming_adv->client);

	batadv_tt_global_add(bat_priv, orig_node, roaming_adv->client,
			     ntohs(roaming_adv->vid), BATADV_TT_CLIENT_ROAM,
			     atomic_read(&orig_node->last_ttvn) + 1);

out:
	if (orig_node)
		batadv_orig_node_free_ref(orig_node);
	return NET_RX_SUCCESS;
}

/**
 * batadv_tt_init - initialise the translation table internals
 * @bat_priv: the bat priv with all the soft interface information
 *
 * Return 0 on success or negative error number in case of failure.
 */
int batadv_tt_init(struct batadv_priv *bat_priv)
{
	int ret;

	/* synchronized flags must be remote */
	BUILD_BUG_ON(!(BATADV_TT_SYNC_MASK & BATADV_TT_REMOTE_MASK));

	ret = batadv_tt_local_init(bat_priv);
	if (ret < 0)
		return ret;

	ret = batadv_tt_global_init(bat_priv);
	if (ret < 0)
		return ret;

	batadv_tvlv_handler_register(bat_priv, batadv_tt_tvlv_ogm_handler_v1,
				     batadv_tt_tvlv_unicast_handler_v1,
				     BATADV_TVLV_TT, 1, BATADV_NO_FLAGS);

	batadv_tvlv_handler_register(bat_priv, NULL,
				     batadv_roam_tvlv_unicast_handler_v1,
				     BATADV_TVLV_ROAM, 1, BATADV_NO_FLAGS);

	INIT_DELAYED_WORK(&bat_priv->tt.work, batadv_tt_purge);
	queue_delayed_work(batadv_event_workqueue, &bat_priv->tt.work,
			   msecs_to_jiffies(BATADV_TT_WORK_PERIOD));

	return 1;
}
>>>>>>> refs/remotes/origin/master
