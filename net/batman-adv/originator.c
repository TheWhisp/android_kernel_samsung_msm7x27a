<<<<<<< HEAD
/*
<<<<<<< HEAD
 * Copyright (C) 2009-2011 B.A.T.M.A.N. contributors:
=======
 * Copyright (C) 2009-2012 B.A.T.M.A.N. contributors:
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* Copyright (C) 2009-2013 B.A.T.M.A.N. contributors:
>>>>>>> refs/remotes/origin/master
 *
 * Marek Lindner, Simon Wunderlich
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
 */

#include "main.h"
=======
 */

#include "main.h"
#include "distributed-arp-table.h"
>>>>>>> refs/remotes/origin/master
#include "originator.h"
#include "hash.h"
#include "translation-table.h"
#include "routing.h"
#include "gateway_client.h"
#include "hard-interface.h"
<<<<<<< HEAD
#include "unicast.h"
#include "soft-interface.h"

static void purge_orig(struct work_struct *work);

static void start_purge_timer(struct bat_priv *bat_priv)
{
	INIT_DELAYED_WORK(&bat_priv->orig_work, purge_orig);
	queue_delayed_work(bat_event_workqueue, &bat_priv->orig_work, 1 * HZ);
}

<<<<<<< HEAD
=======
/* returns 1 if they are the same originator */
static int compare_orig(const struct hlist_node *node, const void *data2)
{
	const void *data1 = container_of(node, struct orig_node, hash_entry);
=======
#include "soft-interface.h"
#include "bridge_loop_avoidance.h"
#include "network-coding.h"
#include "fragmentation.h"

/* hash class keys */
static struct lock_class_key batadv_orig_hash_lock_class_key;

static void batadv_purge_orig(struct work_struct *work);

/* returns 1 if they are the same originator */
int batadv_compare_orig(const struct hlist_node *node, const void *data2)
{
	const void *data1 = container_of(node, struct batadv_orig_node,
					 hash_entry);
>>>>>>> refs/remotes/origin/master

	return (memcmp(data1, data2, ETH_ALEN) == 0 ? 1 : 0);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
int originator_init(struct bat_priv *bat_priv)
{
	if (bat_priv->orig_hash)
		return 1;

	bat_priv->orig_hash = hash_new(1024);
=======
/**
 * batadv_orig_node_vlan_get - get an orig_node_vlan object
 * @orig_node: the originator serving the VLAN
 * @vid: the VLAN identifier
 *
 * Returns the vlan object identified by vid and belonging to orig_node or NULL
 * if it does not exist.
 */
struct batadv_orig_node_vlan *
batadv_orig_node_vlan_get(struct batadv_orig_node *orig_node,
			  unsigned short vid)
{
	struct batadv_orig_node_vlan *vlan = NULL, *tmp;

	rcu_read_lock();
	list_for_each_entry_rcu(tmp, &orig_node->vlan_list, list) {
		if (tmp->vid != vid)
			continue;

		if (!atomic_inc_not_zero(&tmp->refcount))
			continue;

		vlan = tmp;

		break;
	}
	rcu_read_unlock();

	return vlan;
}

/**
 * batadv_orig_node_vlan_new - search and possibly create an orig_node_vlan
 *  object
 * @orig_node: the originator serving the VLAN
 * @vid: the VLAN identifier
 *
 * Returns NULL in case of failure or the vlan object identified by vid and
 * belonging to orig_node otherwise. The object is created and added to the list
 * if it does not exist.
 *
 * The object is returned with refcounter increased by 1.
 */
struct batadv_orig_node_vlan *
batadv_orig_node_vlan_new(struct batadv_orig_node *orig_node,
			  unsigned short vid)
{
	struct batadv_orig_node_vlan *vlan;

	spin_lock_bh(&orig_node->vlan_list_lock);

	/* first look if an object for this vid already exists */
	vlan = batadv_orig_node_vlan_get(orig_node, vid);
	if (vlan)
		goto out;

	vlan = kzalloc(sizeof(*vlan), GFP_ATOMIC);
	if (!vlan)
		goto out;

	atomic_set(&vlan->refcount, 2);
	vlan->vid = vid;

	list_add_rcu(&vlan->list, &orig_node->vlan_list);

out:
	spin_unlock_bh(&orig_node->vlan_list_lock);

	return vlan;
}

/**
 * batadv_orig_node_vlan_free_ref - decrement the refcounter and possibly free
 *  the originator-vlan object
 * @orig_vlan: the originator-vlan object to release
 */
void batadv_orig_node_vlan_free_ref(struct batadv_orig_node_vlan *orig_vlan)
{
	if (atomic_dec_and_test(&orig_vlan->refcount))
		kfree_rcu(orig_vlan, rcu);
}

int batadv_originator_init(struct batadv_priv *bat_priv)
{
	if (bat_priv->orig_hash)
		return 0;

	bat_priv->orig_hash = batadv_hash_new(1024);
>>>>>>> refs/remotes/origin/master

	if (!bat_priv->orig_hash)
		goto err;

<<<<<<< HEAD
	start_purge_timer(bat_priv);
	return 1;

err:
	return 0;
}

void neigh_node_free_ref(struct neigh_node *neigh_node)
=======
	batadv_hash_set_lock_class(bat_priv->orig_hash,
				   &batadv_orig_hash_lock_class_key);

	INIT_DELAYED_WORK(&bat_priv->orig_work, batadv_purge_orig);
	queue_delayed_work(batadv_event_workqueue,
			   &bat_priv->orig_work,
			   msecs_to_jiffies(BATADV_ORIG_WORK_PERIOD));

	return 0;

err:
	return -ENOMEM;
}

void batadv_neigh_node_free_ref(struct batadv_neigh_node *neigh_node)
>>>>>>> refs/remotes/origin/master
{
	if (atomic_dec_and_test(&neigh_node->refcount))
		kfree_rcu(neigh_node, rcu);
}

/* increases the refcounter of a found router */
<<<<<<< HEAD
struct neigh_node *orig_node_get_router(struct orig_node *orig_node)
{
	struct neigh_node *router;
=======
struct batadv_neigh_node *
batadv_orig_node_get_router(struct batadv_orig_node *orig_node)
{
	struct batadv_neigh_node *router;
>>>>>>> refs/remotes/origin/master

	rcu_read_lock();
	router = rcu_dereference(orig_node->router);

	if (router && !atomic_inc_not_zero(&router->refcount))
		router = NULL;

	rcu_read_unlock();
	return router;
}

<<<<<<< HEAD
struct neigh_node *create_neighbor(struct orig_node *orig_node,
				   struct orig_node *orig_neigh_node,
<<<<<<< HEAD
				   uint8_t *neigh,
=======
				   const uint8_t *neigh,
>>>>>>> refs/remotes/origin/cm-10.0
				   struct hard_iface *if_incoming)
{
	struct bat_priv *bat_priv = netdev_priv(if_incoming->soft_iface);
	struct neigh_node *neigh_node;

	bat_dbg(DBG_BATMAN, bat_priv,
		"Creating new last-hop neighbor of originator\n");

<<<<<<< HEAD
	neigh_node = kzalloc(sizeof(struct neigh_node), GFP_ATOMIC);
=======
	neigh_node = kzalloc(sizeof(*neigh_node), GFP_ATOMIC);
>>>>>>> refs/remotes/origin/cm-10.0
	if (!neigh_node)
		return NULL;

	INIT_HLIST_NODE(&neigh_node->list);
	INIT_LIST_HEAD(&neigh_node->bonding_list);
	spin_lock_init(&neigh_node->tq_lock);

	memcpy(neigh_node->addr, neigh, ETH_ALEN);
	neigh_node->orig_node = orig_neigh_node;
	neigh_node->if_incoming = if_incoming;
=======
/**
 * batadv_neigh_node_new - create and init a new neigh_node object
 * @hard_iface: the interface where the neighbour is connected to
 * @neigh_addr: the mac address of the neighbour interface
 * @orig_node: originator object representing the neighbour
 *
 * Allocates a new neigh_node object and initialises all the generic fields.
 * Returns the new object or NULL on failure.
 */
struct batadv_neigh_node *
batadv_neigh_node_new(struct batadv_hard_iface *hard_iface,
		      const uint8_t *neigh_addr,
		      struct batadv_orig_node *orig_node)
{
	struct batadv_neigh_node *neigh_node;

	neigh_node = kzalloc(sizeof(*neigh_node), GFP_ATOMIC);
	if (!neigh_node)
		goto out;

	INIT_HLIST_NODE(&neigh_node->list);

	memcpy(neigh_node->addr, neigh_addr, ETH_ALEN);
	neigh_node->if_incoming = hard_iface;
	neigh_node->orig_node = orig_node;

	INIT_LIST_HEAD(&neigh_node->bonding_list);
>>>>>>> refs/remotes/origin/master

	/* extra reference for return */
	atomic_set(&neigh_node->refcount, 2);

<<<<<<< HEAD
	spin_lock_bh(&orig_node->neigh_list_lock);
	hlist_add_head_rcu(&neigh_node->list, &orig_node->neigh_list);
	spin_unlock_bh(&orig_node->neigh_list_lock);
	return neigh_node;
}

static void orig_node_free_rcu(struct rcu_head *rcu)
{
	struct hlist_node *node, *node_tmp;
	struct neigh_node *neigh_node, *tmp_neigh_node;
	struct orig_node *orig_node;

	orig_node = container_of(rcu, struct orig_node, rcu);
=======
out:
	return neigh_node;
}

static void batadv_orig_node_free_rcu(struct rcu_head *rcu)
{
	struct hlist_node *node_tmp;
	struct batadv_neigh_node *neigh_node, *tmp_neigh_node;
	struct batadv_orig_node *orig_node;

	orig_node = container_of(rcu, struct batadv_orig_node, rcu);
>>>>>>> refs/remotes/origin/master

	spin_lock_bh(&orig_node->neigh_list_lock);

	/* for all bonding members ... */
	list_for_each_entry_safe(neigh_node, tmp_neigh_node,
				 &orig_node->bond_list, bonding_list) {
		list_del_rcu(&neigh_node->bonding_list);
<<<<<<< HEAD
		neigh_node_free_ref(neigh_node);
	}

	/* for all neighbors towards this originator ... */
	hlist_for_each_entry_safe(neigh_node, node, node_tmp,
				  &orig_node->neigh_list, list) {
		hlist_del_rcu(&neigh_node->list);
		neigh_node_free_ref(neigh_node);
=======
		batadv_neigh_node_free_ref(neigh_node);
	}

	/* for all neighbors towards this originator ... */
	hlist_for_each_entry_safe(neigh_node, node_tmp,
				  &orig_node->neigh_list, list) {
		hlist_del_rcu(&neigh_node->list);
		batadv_neigh_node_free_ref(neigh_node);
>>>>>>> refs/remotes/origin/master
	}

	spin_unlock_bh(&orig_node->neigh_list_lock);

<<<<<<< HEAD
	frag_list_free(&orig_node->frag_list);
	tt_global_del_orig(orig_node->bat_priv, orig_node,
<<<<<<< HEAD
			    "originator timed out");

=======
			   "originator timed out");

	kfree(orig_node->tt_buff);
>>>>>>> refs/remotes/origin/cm-10.0
	kfree(orig_node->bcast_own);
	kfree(orig_node->bcast_own_sum);
	kfree(orig_node);
}

void orig_node_free_ref(struct orig_node *orig_node)
{
	if (atomic_dec_and_test(&orig_node->refcount))
		call_rcu(&orig_node->rcu, orig_node_free_rcu);
}

void originator_free(struct bat_priv *bat_priv)
{
	struct hashtable_t *hash = bat_priv->orig_hash;
	struct hlist_node *node, *node_tmp;
	struct hlist_head *head;
	spinlock_t *list_lock; /* spinlock to protect write access */
	struct orig_node *orig_node;
<<<<<<< HEAD
	int i;
=======
	uint32_t i;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* Free nc_nodes */
	batadv_nc_purge_orig(orig_node->bat_priv, orig_node, NULL);

	batadv_frag_purge_orig(orig_node, NULL);

	batadv_tt_global_del_orig(orig_node->bat_priv, orig_node, -1,
				  "originator timed out");

	if (orig_node->bat_priv->bat_algo_ops->bat_orig_free)
		orig_node->bat_priv->bat_algo_ops->bat_orig_free(orig_node);

	kfree(orig_node->tt_buff);
	kfree(orig_node);
}

/**
 * batadv_orig_node_free_ref - decrement the orig node refcounter and possibly
 * schedule an rcu callback for freeing it
 * @orig_node: the orig node to free
 */
void batadv_orig_node_free_ref(struct batadv_orig_node *orig_node)
{
	if (atomic_dec_and_test(&orig_node->refcount))
		call_rcu(&orig_node->rcu, batadv_orig_node_free_rcu);
}

/**
 * batadv_orig_node_free_ref_now - decrement the orig node refcounter and
 * possibly free it (without rcu callback)
 * @orig_node: the orig node to free
 */
void batadv_orig_node_free_ref_now(struct batadv_orig_node *orig_node)
{
	if (atomic_dec_and_test(&orig_node->refcount))
		batadv_orig_node_free_rcu(&orig_node->rcu);
}

void batadv_originator_free(struct batadv_priv *bat_priv)
{
	struct batadv_hashtable *hash = bat_priv->orig_hash;
	struct hlist_node *node_tmp;
	struct hlist_head *head;
	spinlock_t *list_lock; /* spinlock to protect write access */
	struct batadv_orig_node *orig_node;
	uint32_t i;
>>>>>>> refs/remotes/origin/master

	if (!hash)
		return;

	cancel_delayed_work_sync(&bat_priv->orig_work);

	bat_priv->orig_hash = NULL;

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];
		list_lock = &hash->list_locks[i];

		spin_lock_bh(list_lock);
<<<<<<< HEAD
		hlist_for_each_entry_safe(orig_node, node, node_tmp,
					  head, hash_entry) {

			hlist_del_rcu(node);
			orig_node_free_ref(orig_node);
=======
		hlist_for_each_entry_safe(orig_node, node_tmp,
					  head, hash_entry) {
			hlist_del_rcu(&orig_node->hash_entry);
			batadv_orig_node_free_ref(orig_node);
>>>>>>> refs/remotes/origin/master
		}
		spin_unlock_bh(list_lock);
	}

<<<<<<< HEAD
	hash_destroy(hash);
}

/* this function finds or creates an originator entry for the given
 * address if it does not exits */
<<<<<<< HEAD
struct orig_node *get_orig_node(struct bat_priv *bat_priv, uint8_t *addr)
=======
struct orig_node *get_orig_node(struct bat_priv *bat_priv, const uint8_t *addr)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct orig_node *orig_node;
	int size;
	int hash_added;

	orig_node = orig_hash_find(bat_priv, addr);
	if (orig_node)
		return orig_node;

	bat_dbg(DBG_BATMAN, bat_priv,
		"Creating new originator: %pM\n", addr);

<<<<<<< HEAD
	orig_node = kzalloc(sizeof(struct orig_node), GFP_ATOMIC);
=======
	orig_node = kzalloc(sizeof(*orig_node), GFP_ATOMIC);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	batadv_hash_destroy(hash);
}

/**
 * batadv_orig_node_new - creates a new orig_node
 * @bat_priv: the bat priv with all the soft interface information
 * @addr: the mac address of the originator
 *
 * Creates a new originator object and initialise all the generic fields.
 * The new object is not added to the originator list.
 * Returns the newly created object or NULL on failure.
 */
struct batadv_orig_node *batadv_orig_node_new(struct batadv_priv *bat_priv,
					      const uint8_t *addr)
{
	struct batadv_orig_node *orig_node;
	struct batadv_orig_node_vlan *vlan;
	unsigned long reset_time;
	int i;

	batadv_dbg(BATADV_DBG_BATMAN, bat_priv,
		   "Creating new originator: %pM\n", addr);

	orig_node = kzalloc(sizeof(*orig_node), GFP_ATOMIC);
>>>>>>> refs/remotes/origin/master
	if (!orig_node)
		return NULL;

	INIT_HLIST_HEAD(&orig_node->neigh_list);
	INIT_LIST_HEAD(&orig_node->bond_list);
<<<<<<< HEAD
	spin_lock_init(&orig_node->ogm_cnt_lock);
	spin_lock_init(&orig_node->bcast_seqno_lock);
	spin_lock_init(&orig_node->neigh_list_lock);
<<<<<<< HEAD
=======
	spin_lock_init(&orig_node->tt_buff_lock);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	INIT_LIST_HEAD(&orig_node->vlan_list);
	spin_lock_init(&orig_node->bcast_seqno_lock);
	spin_lock_init(&orig_node->neigh_list_lock);
	spin_lock_init(&orig_node->tt_buff_lock);
	spin_lock_init(&orig_node->tt_lock);
	spin_lock_init(&orig_node->vlan_list_lock);

	batadv_nc_init_orig(orig_node);
>>>>>>> refs/remotes/origin/master

	/* extra reference for return */
	atomic_set(&orig_node->refcount, 2);

<<<<<<< HEAD
<<<<<<< HEAD
	orig_node->bat_priv = bat_priv;
	memcpy(orig_node->orig, addr, ETH_ALEN);
	orig_node->router = NULL;
	orig_node->tt_buff = NULL;
=======
	orig_node->tt_initialised = false;
	orig_node->tt_poss_change = false;
	orig_node->bat_priv = bat_priv;
	memcpy(orig_node->orig, addr, ETH_ALEN);
	orig_node->router = NULL;
	orig_node->tt_crc = 0;
	atomic_set(&orig_node->last_ttvn, 0);
	orig_node->tt_buff = NULL;
	orig_node->tt_buff_len = 0;
	atomic_set(&orig_node->tt_size, 0);
>>>>>>> refs/remotes/origin/cm-10.0
	orig_node->bcast_seqno_reset = jiffies - 1
					- msecs_to_jiffies(RESET_PROTECTION_MS);
	orig_node->batman_seqno_reset = jiffies - 1
					- msecs_to_jiffies(RESET_PROTECTION_MS);

	atomic_set(&orig_node->bond_candidates, 0);

	size = bat_priv->num_ifaces * sizeof(unsigned long) * NUM_WORDS;

	orig_node->bcast_own = kzalloc(size, GFP_ATOMIC);
	if (!orig_node->bcast_own)
		goto free_orig_node;

	size = bat_priv->num_ifaces * sizeof(uint8_t);
	orig_node->bcast_own_sum = kzalloc(size, GFP_ATOMIC);

	INIT_LIST_HEAD(&orig_node->frag_list);
	orig_node->last_frag_packet = 0;

	if (!orig_node->bcast_own_sum)
		goto free_bcast_own;

	hash_added = hash_add(bat_priv->orig_hash, compare_orig,
			      choose_orig, orig_node, &orig_node->hash_entry);
<<<<<<< HEAD
	if (hash_added < 0)
=======
	if (hash_added != 0)
>>>>>>> refs/remotes/origin/cm-10.0
		goto free_bcast_own_sum;

	return orig_node;
free_bcast_own_sum:
	kfree(orig_node->bcast_own_sum);
free_bcast_own:
	kfree(orig_node->bcast_own);
=======
	orig_node->tt_initialised = false;
	orig_node->bat_priv = bat_priv;
	memcpy(orig_node->orig, addr, ETH_ALEN);
	batadv_dat_init_orig_node_addr(orig_node);
	orig_node->router = NULL;
	atomic_set(&orig_node->last_ttvn, 0);
	orig_node->tt_buff = NULL;
	orig_node->tt_buff_len = 0;
	reset_time = jiffies - 1 - msecs_to_jiffies(BATADV_RESET_PROTECTION_MS);
	orig_node->bcast_seqno_reset = reset_time;
	orig_node->batman_seqno_reset = reset_time;

	atomic_set(&orig_node->bond_candidates, 0);

	/* create a vlan object for the "untagged" LAN */
	vlan = batadv_orig_node_vlan_new(orig_node, BATADV_NO_FLAGS);
	if (!vlan)
		goto free_orig_node;
	/* batadv_orig_node_vlan_new() increases the refcounter.
	 * Immediately release vlan since it is not needed anymore in this
	 * context
	 */
	batadv_orig_node_vlan_free_ref(vlan);

	for (i = 0; i < BATADV_FRAG_BUFFER_COUNT; i++) {
		INIT_HLIST_HEAD(&orig_node->fragments[i].head);
		spin_lock_init(&orig_node->fragments[i].lock);
		orig_node->fragments[i].size = 0;
	}

	return orig_node;
>>>>>>> refs/remotes/origin/master
free_orig_node:
	kfree(orig_node);
	return NULL;
}

<<<<<<< HEAD
static bool purge_orig_neighbors(struct bat_priv *bat_priv,
				 struct orig_node *orig_node,
				 struct neigh_node **best_neigh_node)
{
	struct hlist_node *node, *node_tmp;
	struct neigh_node *neigh_node;
	bool neigh_purged = false;

	*best_neigh_node = NULL;
=======
static bool
batadv_purge_orig_neighbors(struct batadv_priv *bat_priv,
			    struct batadv_orig_node *orig_node,
			    struct batadv_neigh_node **best_neigh)
{
	struct batadv_algo_ops *bao = bat_priv->bat_algo_ops;
	struct hlist_node *node_tmp;
	struct batadv_neigh_node *neigh_node;
	bool neigh_purged = false;
	unsigned long last_seen;
	struct batadv_hard_iface *if_incoming;

	*best_neigh = NULL;
>>>>>>> refs/remotes/origin/master

	spin_lock_bh(&orig_node->neigh_list_lock);

	/* for all neighbors towards this originator ... */
<<<<<<< HEAD
	hlist_for_each_entry_safe(neigh_node, node, node_tmp,
				  &orig_node->neigh_list, list) {

<<<<<<< HEAD
		if ((time_after(jiffies,
			neigh_node->last_valid + PURGE_TIMEOUT * HZ)) ||
=======
		if ((has_timed_out(neigh_node->last_valid, PURGE_TIMEOUT)) ||
>>>>>>> refs/remotes/origin/cm-10.0
		    (neigh_node->if_incoming->if_status == IF_INACTIVE) ||
		    (neigh_node->if_incoming->if_status == IF_NOT_IN_USE) ||
		    (neigh_node->if_incoming->if_status == IF_TO_BE_REMOVED)) {

			if ((neigh_node->if_incoming->if_status ==
								IF_INACTIVE) ||
			    (neigh_node->if_incoming->if_status ==
							IF_NOT_IN_USE) ||
			    (neigh_node->if_incoming->if_status ==
							IF_TO_BE_REMOVED))
				bat_dbg(DBG_BATMAN, bat_priv,
<<<<<<< HEAD
					"neighbor purge: originator %pM, "
					"neighbor: %pM, iface: %s\n",
=======
					"neighbor purge: originator %pM, neighbor: %pM, iface: %s\n",
>>>>>>> refs/remotes/origin/cm-10.0
					orig_node->orig, neigh_node->addr,
					neigh_node->if_incoming->net_dev->name);
			else
				bat_dbg(DBG_BATMAN, bat_priv,
<<<<<<< HEAD
					"neighbor timeout: originator %pM, "
					"neighbor: %pM, last_valid: %lu\n",
=======
					"neighbor timeout: originator %pM, neighbor: %pM, last_valid: %lu\n",
>>>>>>> refs/remotes/origin/cm-10.0
					orig_node->orig, neigh_node->addr,
					(neigh_node->last_valid / HZ));
=======
	hlist_for_each_entry_safe(neigh_node, node_tmp,
				  &orig_node->neigh_list, list) {
		last_seen = neigh_node->last_seen;
		if_incoming = neigh_node->if_incoming;

		if ((batadv_has_timed_out(last_seen, BATADV_PURGE_TIMEOUT)) ||
		    (if_incoming->if_status == BATADV_IF_INACTIVE) ||
		    (if_incoming->if_status == BATADV_IF_NOT_IN_USE) ||
		    (if_incoming->if_status == BATADV_IF_TO_BE_REMOVED)) {
			if ((if_incoming->if_status == BATADV_IF_INACTIVE) ||
			    (if_incoming->if_status == BATADV_IF_NOT_IN_USE) ||
			    (if_incoming->if_status == BATADV_IF_TO_BE_REMOVED))
				batadv_dbg(BATADV_DBG_BATMAN, bat_priv,
					   "neighbor purge: originator %pM, neighbor: %pM, iface: %s\n",
					   orig_node->orig, neigh_node->addr,
					   if_incoming->net_dev->name);
			else
				batadv_dbg(BATADV_DBG_BATMAN, bat_priv,
					   "neighbor timeout: originator %pM, neighbor: %pM, last_seen: %u\n",
					   orig_node->orig, neigh_node->addr,
					   jiffies_to_msecs(last_seen));
>>>>>>> refs/remotes/origin/master

			neigh_purged = true;

			hlist_del_rcu(&neigh_node->list);
<<<<<<< HEAD
			bonding_candidate_del(orig_node, neigh_node);
			neigh_node_free_ref(neigh_node);
		} else {
			if ((!*best_neigh_node) ||
			    (neigh_node->tq_avg > (*best_neigh_node)->tq_avg))
				*best_neigh_node = neigh_node;
=======
			batadv_bonding_candidate_del(orig_node, neigh_node);
			batadv_neigh_node_free_ref(neigh_node);
		} else {
			/* store the best_neighbour if this is the first
			 * iteration or if a better neighbor has been found
			 */
			if (!*best_neigh ||
			    bao->bat_neigh_cmp(neigh_node, *best_neigh) > 0)
				*best_neigh = neigh_node;
>>>>>>> refs/remotes/origin/master
		}
	}

	spin_unlock_bh(&orig_node->neigh_list_lock);
	return neigh_purged;
}

<<<<<<< HEAD
static bool purge_orig_node(struct bat_priv *bat_priv,
			    struct orig_node *orig_node)
{
	struct neigh_node *best_neigh_node;

<<<<<<< HEAD
	if (time_after(jiffies,
		orig_node->last_valid + 2 * PURGE_TIMEOUT * HZ)) {

=======
	if (has_timed_out(orig_node->last_valid, 2 * PURGE_TIMEOUT)) {
>>>>>>> refs/remotes/origin/cm-10.0
		bat_dbg(DBG_BATMAN, bat_priv,
			"Originator timeout: originator %pM, last_valid %lu\n",
			orig_node->orig, (orig_node->last_valid / HZ));
		return true;
	} else {
		if (purge_orig_neighbors(bat_priv, orig_node,
<<<<<<< HEAD
							&best_neigh_node)) {
			update_routes(bat_priv, orig_node,
				      best_neigh_node,
				      orig_node->tt_buff,
				      orig_node->tt_buff_len);
		}
=======
					 &best_neigh_node))
			update_route(bat_priv, orig_node, best_neigh_node);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static bool batadv_purge_orig_node(struct batadv_priv *bat_priv,
				   struct batadv_orig_node *orig_node)
{
	struct batadv_neigh_node *best_neigh_node;

	if (batadv_has_timed_out(orig_node->last_seen,
				 2 * BATADV_PURGE_TIMEOUT)) {
		batadv_dbg(BATADV_DBG_BATMAN, bat_priv,
			   "Originator timeout: originator %pM, last_seen %u\n",
			   orig_node->orig,
			   jiffies_to_msecs(orig_node->last_seen));
		return true;
	} else {
		if (batadv_purge_orig_neighbors(bat_priv, orig_node,
						&best_neigh_node))
			batadv_update_route(bat_priv, orig_node,
					    best_neigh_node);
>>>>>>> refs/remotes/origin/master
	}

	return false;
}

<<<<<<< HEAD
static void _purge_orig(struct bat_priv *bat_priv)
{
	struct hashtable_t *hash = bat_priv->orig_hash;
	struct hlist_node *node, *node_tmp;
	struct hlist_head *head;
	spinlock_t *list_lock; /* spinlock to protect write access */
	struct orig_node *orig_node;
<<<<<<< HEAD
	int i;
=======
	uint32_t i;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void _batadv_purge_orig(struct batadv_priv *bat_priv)
{
	struct batadv_hashtable *hash = bat_priv->orig_hash;
	struct hlist_node *node_tmp;
	struct hlist_head *head;
	spinlock_t *list_lock; /* spinlock to protect write access */
	struct batadv_orig_node *orig_node;
	uint32_t i;
>>>>>>> refs/remotes/origin/master

	if (!hash)
		return;

	/* for all origins... */
	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];
		list_lock = &hash->list_locks[i];

		spin_lock_bh(list_lock);
<<<<<<< HEAD
		hlist_for_each_entry_safe(orig_node, node, node_tmp,
					  head, hash_entry) {
			if (purge_orig_node(bat_priv, orig_node)) {
				if (orig_node->gw_flags)
					gw_node_delete(bat_priv, orig_node);
				hlist_del_rcu(node);
				orig_node_free_ref(orig_node);
				continue;
			}

<<<<<<< HEAD
			if (time_after(jiffies, orig_node->last_frag_packet +
						msecs_to_jiffies(FRAG_TIMEOUT)))
=======
			if (has_timed_out(orig_node->last_frag_packet,
					  FRAG_TIMEOUT))
>>>>>>> refs/remotes/origin/cm-10.0
				frag_list_free(&orig_node->frag_list);
=======
		hlist_for_each_entry_safe(orig_node, node_tmp,
					  head, hash_entry) {
			if (batadv_purge_orig_node(bat_priv, orig_node)) {
				batadv_gw_node_delete(bat_priv, orig_node);
				hlist_del_rcu(&orig_node->hash_entry);
				batadv_orig_node_free_ref(orig_node);
				continue;
			}

			batadv_frag_purge_orig(orig_node,
					       batadv_frag_check_entry);
>>>>>>> refs/remotes/origin/master
		}
		spin_unlock_bh(list_lock);
	}

<<<<<<< HEAD
	gw_node_purge(bat_priv);
	gw_election(bat_priv);

	softif_neigh_purge(bat_priv);
}

static void purge_orig(struct work_struct *work)
{
	struct delayed_work *delayed_work =
		container_of(work, struct delayed_work, work);
	struct bat_priv *bat_priv =
		container_of(delayed_work, struct bat_priv, orig_work);

	_purge_orig(bat_priv);
	start_purge_timer(bat_priv);
}

void purge_orig_ref(struct bat_priv *bat_priv)
{
	_purge_orig(bat_priv);
}

int orig_seq_print_text(struct seq_file *seq, void *offset)
{
	struct net_device *net_dev = (struct net_device *)seq->private;
	struct bat_priv *bat_priv = netdev_priv(net_dev);
	struct hashtable_t *hash = bat_priv->orig_hash;
	struct hlist_node *node, *node_tmp;
	struct hlist_head *head;
	struct hard_iface *primary_if;
	struct orig_node *orig_node;
	struct neigh_node *neigh_node, *neigh_node_tmp;
	int batman_count = 0;
	int last_seen_secs;
	int last_seen_msecs;
<<<<<<< HEAD
	int i, ret = 0;
=======
	uint32_t i;
	int ret = 0;
>>>>>>> refs/remotes/origin/cm-10.0

	primary_if = primary_if_get_selected(bat_priv);

	if (!primary_if) {
<<<<<<< HEAD
		ret = seq_printf(seq, "BATMAN mesh %s disabled - "
				 "please specify interfaces to enable it\n",
=======
		ret = seq_printf(seq,
				 "BATMAN mesh %s disabled - please specify interfaces to enable it\n",
>>>>>>> refs/remotes/origin/cm-10.0
				 net_dev->name);
		goto out;
	}

	if (primary_if->if_status != IF_ACTIVE) {
<<<<<<< HEAD
		ret = seq_printf(seq, "BATMAN mesh %s "
				 "disabled - primary interface not active\n",
=======
		ret = seq_printf(seq,
				 "BATMAN mesh %s disabled - primary interface not active\n",
>>>>>>> refs/remotes/origin/cm-10.0
				 net_dev->name);
		goto out;
	}

<<<<<<< HEAD
	seq_printf(seq, "[B.A.T.M.A.N. adv %s%s, MainIF/MAC: %s/%pM (%s)]\n",
		   SOURCE_VERSION, REVISION_VERSION_STR,
		   primary_if->net_dev->name,
=======
	seq_printf(seq, "[B.A.T.M.A.N. adv %s, MainIF/MAC: %s/%pM (%s)]\n",
		   SOURCE_VERSION, primary_if->net_dev->name,
>>>>>>> refs/remotes/origin/cm-10.0
		   primary_if->net_dev->dev_addr, net_dev->name);
	seq_printf(seq, "  %-15s %s (%s/%i) %17s [%10s]: %20s ...\n",
		   "Originator", "last-seen", "#", TQ_MAX_VALUE, "Nexthop",
		   "outgoingIF", "Potential nexthops");

	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];

		rcu_read_lock();
		hlist_for_each_entry_rcu(orig_node, node, head, hash_entry) {
			neigh_node = orig_node_get_router(orig_node);
			if (!neigh_node)
				continue;

			if (neigh_node->tq_avg == 0)
				goto next;

			last_seen_secs = jiffies_to_msecs(jiffies -
						orig_node->last_valid) / 1000;
			last_seen_msecs = jiffies_to_msecs(jiffies -
						orig_node->last_valid) % 1000;

			seq_printf(seq, "%pM %4i.%03is   (%3i) %pM [%10s]:",
				   orig_node->orig, last_seen_secs,
				   last_seen_msecs, neigh_node->tq_avg,
				   neigh_node->addr,
				   neigh_node->if_incoming->net_dev->name);

			hlist_for_each_entry_rcu(neigh_node_tmp, node_tmp,
						 &orig_node->neigh_list, list) {
				seq_printf(seq, " %pM (%3i)",
					   neigh_node_tmp->addr,
					   neigh_node_tmp->tq_avg);
			}

			seq_printf(seq, "\n");
			batman_count++;

next:
			neigh_node_free_ref(neigh_node);
		}
		rcu_read_unlock();
	}

	if (batman_count == 0)
		seq_printf(seq, "No batman nodes in range ...\n");

out:
	if (primary_if)
		hardif_free_ref(primary_if);
	return ret;
}

static int orig_node_add_if(struct orig_node *orig_node, int max_if_num)
{
	void *data_ptr;

	data_ptr = kmalloc(max_if_num * sizeof(unsigned long) * NUM_WORDS,
			   GFP_ATOMIC);
<<<<<<< HEAD
	if (!data_ptr) {
		pr_err("Can't resize orig: out of memory\n");
		return -1;
	}
=======
	if (!data_ptr)
		return -1;
>>>>>>> refs/remotes/origin/cm-10.0

	memcpy(data_ptr, orig_node->bcast_own,
	       (max_if_num - 1) * sizeof(unsigned long) * NUM_WORDS);
	kfree(orig_node->bcast_own);
	orig_node->bcast_own = data_ptr;

	data_ptr = kmalloc(max_if_num * sizeof(uint8_t), GFP_ATOMIC);
<<<<<<< HEAD
	if (!data_ptr) {
		pr_err("Can't resize orig: out of memory\n");
		return -1;
	}
=======
	if (!data_ptr)
		return -1;
>>>>>>> refs/remotes/origin/cm-10.0

	memcpy(data_ptr, orig_node->bcast_own_sum,
	       (max_if_num - 1) * sizeof(uint8_t));
	kfree(orig_node->bcast_own_sum);
	orig_node->bcast_own_sum = data_ptr;
=======
	batadv_gw_node_purge(bat_priv);
	batadv_gw_election(bat_priv);
}

static void batadv_purge_orig(struct work_struct *work)
{
	struct delayed_work *delayed_work;
	struct batadv_priv *bat_priv;

	delayed_work = container_of(work, struct delayed_work, work);
	bat_priv = container_of(delayed_work, struct batadv_priv, orig_work);
	_batadv_purge_orig(bat_priv);
	queue_delayed_work(batadv_event_workqueue,
			   &bat_priv->orig_work,
			   msecs_to_jiffies(BATADV_ORIG_WORK_PERIOD));
}

void batadv_purge_orig_ref(struct batadv_priv *bat_priv)
{
	_batadv_purge_orig(bat_priv);
}

int batadv_orig_seq_print_text(struct seq_file *seq, void *offset)
{
	struct net_device *net_dev = (struct net_device *)seq->private;
	struct batadv_priv *bat_priv = netdev_priv(net_dev);
	struct batadv_hard_iface *primary_if;

	primary_if = batadv_seq_print_text_primary_if_get(seq);
	if (!primary_if)
		return 0;

	seq_printf(seq, "[B.A.T.M.A.N. adv %s, MainIF/MAC: %s/%pM (%s %s)]\n",
		   BATADV_SOURCE_VERSION, primary_if->net_dev->name,
		   primary_if->net_dev->dev_addr, net_dev->name,
		   bat_priv->bat_algo_ops->name);

	batadv_hardif_free_ref(primary_if);

	if (!bat_priv->bat_algo_ops->bat_orig_print) {
		seq_puts(seq,
			 "No printing function for this routing protocol\n");
		return 0;
	}

	bat_priv->bat_algo_ops->bat_orig_print(bat_priv, seq);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
int orig_hash_add_if(struct hard_iface *hard_iface, int max_if_num)
{
	struct bat_priv *bat_priv = netdev_priv(hard_iface->soft_iface);
	struct hashtable_t *hash = bat_priv->orig_hash;
	struct hlist_node *node;
	struct hlist_head *head;
	struct orig_node *orig_node;
<<<<<<< HEAD
	int i, ret;
=======
	uint32_t i;
	int ret;
>>>>>>> refs/remotes/origin/cm-10.0

	/* resize all orig nodes because orig_node->bcast_own(_sum) depend on
	 * if_num */
=======
int batadv_orig_hash_add_if(struct batadv_hard_iface *hard_iface,
			    int max_if_num)
{
	struct batadv_priv *bat_priv = netdev_priv(hard_iface->soft_iface);
	struct batadv_algo_ops *bao = bat_priv->bat_algo_ops;
	struct batadv_hashtable *hash = bat_priv->orig_hash;
	struct hlist_head *head;
	struct batadv_orig_node *orig_node;
	uint32_t i;
	int ret;

	/* resize all orig nodes because orig_node->bcast_own(_sum) depend on
	 * if_num
	 */
>>>>>>> refs/remotes/origin/master
	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];

		rcu_read_lock();
<<<<<<< HEAD
		hlist_for_each_entry_rcu(orig_node, node, head, hash_entry) {
			spin_lock_bh(&orig_node->ogm_cnt_lock);
			ret = orig_node_add_if(orig_node, max_if_num);
			spin_unlock_bh(&orig_node->ogm_cnt_lock);

			if (ret == -1)
=======
		hlist_for_each_entry_rcu(orig_node, head, hash_entry) {
			ret = 0;
			if (bao->bat_orig_add_if)
				ret = bao->bat_orig_add_if(orig_node,
							   max_if_num);
			if (ret == -ENOMEM)
>>>>>>> refs/remotes/origin/master
				goto err;
		}
		rcu_read_unlock();
	}

	return 0;

err:
	rcu_read_unlock();
	return -ENOMEM;
}

<<<<<<< HEAD
static int orig_node_del_if(struct orig_node *orig_node,
		     int max_if_num, int del_if_num)
{
	void *data_ptr = NULL;
	int chunk_size;

	/* last interface was removed */
	if (max_if_num == 0)
		goto free_bcast_own;

	chunk_size = sizeof(unsigned long) * NUM_WORDS;
	data_ptr = kmalloc(max_if_num * chunk_size, GFP_ATOMIC);
<<<<<<< HEAD
	if (!data_ptr) {
		pr_err("Can't resize orig: out of memory\n");
		return -1;
	}
=======
	if (!data_ptr)
		return -1;
>>>>>>> refs/remotes/origin/cm-10.0

	/* copy first part */
	memcpy(data_ptr, orig_node->bcast_own, del_if_num * chunk_size);

	/* copy second part */
<<<<<<< HEAD
	memcpy(data_ptr + del_if_num * chunk_size,
=======
	memcpy((char *)data_ptr + del_if_num * chunk_size,
>>>>>>> refs/remotes/origin/cm-10.0
	       orig_node->bcast_own + ((del_if_num + 1) * chunk_size),
	       (max_if_num - del_if_num) * chunk_size);

free_bcast_own:
	kfree(orig_node->bcast_own);
	orig_node->bcast_own = data_ptr;

	if (max_if_num == 0)
		goto free_own_sum;

	data_ptr = kmalloc(max_if_num * sizeof(uint8_t), GFP_ATOMIC);
<<<<<<< HEAD
	if (!data_ptr) {
		pr_err("Can't resize orig: out of memory\n");
		return -1;
	}
=======
	if (!data_ptr)
		return -1;
>>>>>>> refs/remotes/origin/cm-10.0

	memcpy(data_ptr, orig_node->bcast_own_sum,
	       del_if_num * sizeof(uint8_t));

<<<<<<< HEAD
	memcpy(data_ptr + del_if_num * sizeof(uint8_t),
=======
	memcpy((char *)data_ptr + del_if_num * sizeof(uint8_t),
>>>>>>> refs/remotes/origin/cm-10.0
	       orig_node->bcast_own_sum + ((del_if_num + 1) * sizeof(uint8_t)),
	       (max_if_num - del_if_num) * sizeof(uint8_t));

free_own_sum:
	kfree(orig_node->bcast_own_sum);
	orig_node->bcast_own_sum = data_ptr;

	return 0;
}

int orig_hash_del_if(struct hard_iface *hard_iface, int max_if_num)
{
	struct bat_priv *bat_priv = netdev_priv(hard_iface->soft_iface);
	struct hashtable_t *hash = bat_priv->orig_hash;
	struct hlist_node *node;
	struct hlist_head *head;
	struct hard_iface *hard_iface_tmp;
	struct orig_node *orig_node;
<<<<<<< HEAD
	int i, ret;
=======
	uint32_t i;
	int ret;
>>>>>>> refs/remotes/origin/cm-10.0

	/* resize all orig nodes because orig_node->bcast_own(_sum) depend on
	 * if_num */
=======
int batadv_orig_hash_del_if(struct batadv_hard_iface *hard_iface,
			    int max_if_num)
{
	struct batadv_priv *bat_priv = netdev_priv(hard_iface->soft_iface);
	struct batadv_hashtable *hash = bat_priv->orig_hash;
	struct hlist_head *head;
	struct batadv_hard_iface *hard_iface_tmp;
	struct batadv_orig_node *orig_node;
	struct batadv_algo_ops *bao = bat_priv->bat_algo_ops;
	uint32_t i;
	int ret;

	/* resize all orig nodes because orig_node->bcast_own(_sum) depend on
	 * if_num
	 */
>>>>>>> refs/remotes/origin/master
	for (i = 0; i < hash->size; i++) {
		head = &hash->table[i];

		rcu_read_lock();
<<<<<<< HEAD
		hlist_for_each_entry_rcu(orig_node, node, head, hash_entry) {
			spin_lock_bh(&orig_node->ogm_cnt_lock);
			ret = orig_node_del_if(orig_node, max_if_num,
					hard_iface->if_num);
			spin_unlock_bh(&orig_node->ogm_cnt_lock);

			if (ret == -1)
=======
		hlist_for_each_entry_rcu(orig_node, head, hash_entry) {
			ret = 0;
			if (bao->bat_orig_del_if)
				ret = bao->bat_orig_del_if(orig_node,
							   max_if_num,
							   hard_iface->if_num);
			if (ret == -ENOMEM)
>>>>>>> refs/remotes/origin/master
				goto err;
		}
		rcu_read_unlock();
	}

	/* renumber remaining batman interfaces _inside_ of orig_hash_lock */
	rcu_read_lock();
<<<<<<< HEAD
	list_for_each_entry_rcu(hard_iface_tmp, &hardif_list, list) {
		if (hard_iface_tmp->if_status == IF_NOT_IN_USE)
=======
	list_for_each_entry_rcu(hard_iface_tmp, &batadv_hardif_list, list) {
		if (hard_iface_tmp->if_status == BATADV_IF_NOT_IN_USE)
>>>>>>> refs/remotes/origin/master
			continue;

		if (hard_iface == hard_iface_tmp)
			continue;

		if (hard_iface->soft_iface != hard_iface_tmp->soft_iface)
			continue;

		if (hard_iface_tmp->if_num > hard_iface->if_num)
			hard_iface_tmp->if_num--;
	}
	rcu_read_unlock();

	hard_iface->if_num = -1;
	return 0;

err:
	rcu_read_unlock();
	return -ENOMEM;
}
