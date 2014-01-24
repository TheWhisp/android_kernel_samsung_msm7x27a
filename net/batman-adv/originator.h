<<<<<<< HEAD
/*
<<<<<<< HEAD
 * Copyright (C) 2007-2011 B.A.T.M.A.N. contributors:
=======
 * Copyright (C) 2007-2012 B.A.T.M.A.N. contributors:
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* Copyright (C) 2007-2013 B.A.T.M.A.N. contributors:
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
=======
>>>>>>> refs/remotes/origin/master
 */

#ifndef _NET_BATMAN_ADV_ORIGINATOR_H_
#define _NET_BATMAN_ADV_ORIGINATOR_H_

#include "hash.h"

<<<<<<< HEAD
int originator_init(struct bat_priv *bat_priv);
void originator_free(struct bat_priv *bat_priv);
void purge_orig_ref(struct bat_priv *bat_priv);
void orig_node_free_ref(struct orig_node *orig_node);
<<<<<<< HEAD
struct orig_node *get_orig_node(struct bat_priv *bat_priv, uint8_t *addr);
struct neigh_node *create_neighbor(struct orig_node *orig_node,
				   struct orig_node *orig_neigh_node,
				   uint8_t *neigh,
=======
struct orig_node *get_orig_node(struct bat_priv *bat_priv, const uint8_t *addr);
struct neigh_node *create_neighbor(struct orig_node *orig_node,
				   struct orig_node *orig_neigh_node,
				   const uint8_t *neigh,
>>>>>>> refs/remotes/origin/cm-10.0
				   struct hard_iface *if_incoming);
void neigh_node_free_ref(struct neigh_node *neigh_node);
struct neigh_node *orig_node_get_router(struct orig_node *orig_node);
int orig_seq_print_text(struct seq_file *seq, void *offset);
int orig_hash_add_if(struct hard_iface *hard_iface, int max_if_num);
int orig_hash_del_if(struct hard_iface *hard_iface, int max_if_num);


<<<<<<< HEAD
/* returns 1 if they are the same originator */
static inline int compare_orig(struct hlist_node *node, void *data2)
{
	void *data1 = container_of(node, struct orig_node, hash_entry);

	return (memcmp(data1, data2, ETH_ALEN) == 0 ? 1 : 0);
}

/* hashfunction to choose an entry in a hash table of given size */
/* hash algorithm from http://en.wikipedia.org/wiki/Hash_table */
static inline int choose_orig(void *data, int32_t size)
{
	unsigned char *key = data;
=======
/* hashfunction to choose an entry in a hash table of given size */
/* hash algorithm from http://en.wikipedia.org/wiki/Hash_table */
static inline uint32_t choose_orig(const void *data, uint32_t size)
{
	const unsigned char *key = data;
>>>>>>> refs/remotes/origin/cm-10.0
=======
int batadv_compare_orig(const struct hlist_node *node, const void *data2);
int batadv_originator_init(struct batadv_priv *bat_priv);
void batadv_originator_free(struct batadv_priv *bat_priv);
void batadv_purge_orig_ref(struct batadv_priv *bat_priv);
void batadv_orig_node_free_ref(struct batadv_orig_node *orig_node);
void batadv_orig_node_free_ref_now(struct batadv_orig_node *orig_node);
struct batadv_orig_node *batadv_orig_node_new(struct batadv_priv *bat_priv,
					      const uint8_t *addr);
struct batadv_neigh_node *
batadv_neigh_node_new(struct batadv_hard_iface *hard_iface,
		      const uint8_t *neigh_addr,
		      struct batadv_orig_node *orig_node);
void batadv_neigh_node_free_ref(struct batadv_neigh_node *neigh_node);
struct batadv_neigh_node *
batadv_orig_node_get_router(struct batadv_orig_node *orig_node);
int batadv_orig_seq_print_text(struct seq_file *seq, void *offset);
int batadv_orig_hash_add_if(struct batadv_hard_iface *hard_iface,
			    int max_if_num);
int batadv_orig_hash_del_if(struct batadv_hard_iface *hard_iface,
			    int max_if_num);
struct batadv_orig_node_vlan *
batadv_orig_node_vlan_new(struct batadv_orig_node *orig_node,
			  unsigned short vid);
struct batadv_orig_node_vlan *
batadv_orig_node_vlan_get(struct batadv_orig_node *orig_node,
			  unsigned short vid);
void batadv_orig_node_vlan_free_ref(struct batadv_orig_node_vlan *orig_vlan);


/* hashfunction to choose an entry in a hash table of given size
 * hash algorithm from http://en.wikipedia.org/wiki/Hash_table
 */
static inline uint32_t batadv_choose_orig(const void *data, uint32_t size)
{
	const unsigned char *key = data;
>>>>>>> refs/remotes/origin/master
	uint32_t hash = 0;
	size_t i;

	for (i = 0; i < 6; i++) {
		hash += key[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}

	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);

	return hash % size;
}

<<<<<<< HEAD
static inline struct orig_node *orig_hash_find(struct bat_priv *bat_priv,
<<<<<<< HEAD
					       void *data)
=======
					       const void *data)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct hashtable_t *hash = bat_priv->orig_hash;
	struct hlist_head *head;
	struct hlist_node *node;
	struct orig_node *orig_node, *orig_node_tmp = NULL;
=======
static inline struct batadv_orig_node *
batadv_orig_hash_find(struct batadv_priv *bat_priv, const void *data)
{
	struct batadv_hashtable *hash = bat_priv->orig_hash;
	struct hlist_head *head;
	struct batadv_orig_node *orig_node, *orig_node_tmp = NULL;
>>>>>>> refs/remotes/origin/master
	int index;

	if (!hash)
		return NULL;

<<<<<<< HEAD
	index = choose_orig(data, hash->size);
	head = &hash->table[index];

	rcu_read_lock();
	hlist_for_each_entry_rcu(orig_node, node, head, hash_entry) {
		if (!compare_eth(orig_node, data))
=======
	index = batadv_choose_orig(data, hash->size);
	head = &hash->table[index];

	rcu_read_lock();
	hlist_for_each_entry_rcu(orig_node, head, hash_entry) {
		if (!batadv_compare_eth(orig_node, data))
>>>>>>> refs/remotes/origin/master
			continue;

		if (!atomic_inc_not_zero(&orig_node->refcount))
			continue;

		orig_node_tmp = orig_node;
		break;
	}
	rcu_read_unlock();

	return orig_node_tmp;
}

#endif /* _NET_BATMAN_ADV_ORIGINATOR_H_ */
