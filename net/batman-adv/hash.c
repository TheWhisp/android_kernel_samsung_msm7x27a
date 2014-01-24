<<<<<<< HEAD
/*
<<<<<<< HEAD
 * Copyright (C) 2006-2011 B.A.T.M.A.N. contributors:
=======
 * Copyright (C) 2006-2012 B.A.T.M.A.N. contributors:
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* Copyright (C) 2006-2013 B.A.T.M.A.N. contributors:
>>>>>>> refs/remotes/origin/master
 *
 * Simon Wunderlich, Marek Lindner
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
#include "hash.h"

/* clears the hash */
<<<<<<< HEAD
static void hash_init(struct hashtable_t *hash)
{
<<<<<<< HEAD
	int i;
=======
	uint32_t i;
>>>>>>> refs/remotes/origin/cm-10.0

	for (i = 0 ; i < hash->size; i++) {
=======
static void batadv_hash_init(struct batadv_hashtable *hash)
{
	uint32_t i;

	for (i = 0; i < hash->size; i++) {
>>>>>>> refs/remotes/origin/master
		INIT_HLIST_HEAD(&hash->table[i]);
		spin_lock_init(&hash->list_locks[i]);
	}
}

/* free only the hashtable and the hash itself. */
<<<<<<< HEAD
void hash_destroy(struct hashtable_t *hash)
=======
void batadv_hash_destroy(struct batadv_hashtable *hash)
>>>>>>> refs/remotes/origin/master
{
	kfree(hash->list_locks);
	kfree(hash->table);
	kfree(hash);
}

/* allocates and clears the hash */
<<<<<<< HEAD
<<<<<<< HEAD
struct hashtable_t *hash_new(int size)
{
	struct hashtable_t *hash;

	hash = kmalloc(sizeof(struct hashtable_t), GFP_ATOMIC);
	if (!hash)
		return NULL;

	hash->table = kmalloc(sizeof(struct element_t *) * size, GFP_ATOMIC);
	if (!hash->table)
		goto free_hash;

	hash->list_locks = kmalloc(sizeof(spinlock_t) * size, GFP_ATOMIC);
=======
struct hashtable_t *hash_new(uint32_t size)
{
	struct hashtable_t *hash;
=======
struct batadv_hashtable *batadv_hash_new(uint32_t size)
{
	struct batadv_hashtable *hash;
>>>>>>> refs/remotes/origin/master

	hash = kmalloc(sizeof(*hash), GFP_ATOMIC);
	if (!hash)
		return NULL;

	hash->table = kmalloc(sizeof(*hash->table) * size, GFP_ATOMIC);
	if (!hash->table)
		goto free_hash;

	hash->list_locks = kmalloc(sizeof(*hash->list_locks) * size,
				   GFP_ATOMIC);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (!hash->list_locks)
		goto free_table;

	hash->size = size;
<<<<<<< HEAD
	hash_init(hash);
=======
	batadv_hash_init(hash);
>>>>>>> refs/remotes/origin/master
	return hash;

free_table:
	kfree(hash->table);
free_hash:
	kfree(hash);
	return NULL;
}
<<<<<<< HEAD
=======

void batadv_hash_set_lock_class(struct batadv_hashtable *hash,
				struct lock_class_key *key)
{
	uint32_t i;

	for (i = 0; i < hash->size; i++)
		lockdep_set_class(&hash->list_locks[i], key);
}
>>>>>>> refs/remotes/origin/master
