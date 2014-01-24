/**************************************************************************
 *
 * Copyright 2006 Tungsten Graphics, Inc., Bismarck, ND. USA.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 **************************************************************************/
/*
 * Simple open hash tab implementation.
 *
 * Authors:
 * Thomas Hellström <thomas-at-tungstengraphics-dot-com>
 */

<<<<<<< HEAD
#include "drmP.h"
#include "drm_hashtab.h"
#include <linux/hash.h>
#include <linux/slab.h>
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <drm/drmP.h>
#include <drm/drm_hashtab.h>
#include <linux/hash.h>
#include <linux/slab.h>
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master

int drm_ht_create(struct drm_open_hash *ht, unsigned int order)
{
	unsigned int size = 1 << order;

	ht->order = order;
	ht->table = NULL;
	if (size <= PAGE_SIZE / sizeof(*ht->table))
		ht->table = kcalloc(size, sizeof(*ht->table), GFP_KERNEL);
	else
		ht->table = vzalloc(size*sizeof(*ht->table));
	if (!ht->table) {
		DRM_ERROR("Out of memory for hash table\n");
		return -ENOMEM;
	}
	return 0;
}
EXPORT_SYMBOL(drm_ht_create);

void drm_ht_verbose_list(struct drm_open_hash *ht, unsigned long key)
{
	struct drm_hash_item *entry;
	struct hlist_head *h_list;
<<<<<<< HEAD
	struct hlist_node *list;
=======
>>>>>>> refs/remotes/origin/master
	unsigned int hashed_key;
	int count = 0;

	hashed_key = hash_long(key, ht->order);
	DRM_DEBUG("Key is 0x%08lx, Hashed key is 0x%08x\n", key, hashed_key);
	h_list = &ht->table[hashed_key];
<<<<<<< HEAD
	hlist_for_each(list, h_list) {
		entry = hlist_entry(list, struct drm_hash_item, head);
		DRM_DEBUG("count %d, key: 0x%08lx\n", count++, entry->key);
	}
=======
	hlist_for_each_entry(entry, h_list, head)
		DRM_DEBUG("count %d, key: 0x%08lx\n", count++, entry->key);
>>>>>>> refs/remotes/origin/master
}

static struct hlist_node *drm_ht_find_key(struct drm_open_hash *ht,
					  unsigned long key)
{
	struct drm_hash_item *entry;
	struct hlist_head *h_list;
<<<<<<< HEAD
	struct hlist_node *list;
=======
>>>>>>> refs/remotes/origin/master
	unsigned int hashed_key;

	hashed_key = hash_long(key, ht->order);
	h_list = &ht->table[hashed_key];
<<<<<<< HEAD
	hlist_for_each(list, h_list) {
		entry = hlist_entry(list, struct drm_hash_item, head);
		if (entry->key == key)
			return list;
=======
	hlist_for_each_entry(entry, h_list, head) {
		if (entry->key == key)
			return &entry->head;
>>>>>>> refs/remotes/origin/master
		if (entry->key > key)
			break;
	}
	return NULL;
}

<<<<<<< HEAD
=======
static struct hlist_node *drm_ht_find_key_rcu(struct drm_open_hash *ht,
					      unsigned long key)
{
	struct drm_hash_item *entry;
	struct hlist_head *h_list;
	unsigned int hashed_key;

	hashed_key = hash_long(key, ht->order);
	h_list = &ht->table[hashed_key];
	hlist_for_each_entry_rcu(entry, h_list, head) {
		if (entry->key == key)
			return &entry->head;
		if (entry->key > key)
			break;
	}
	return NULL;
}
>>>>>>> refs/remotes/origin/master

int drm_ht_insert_item(struct drm_open_hash *ht, struct drm_hash_item *item)
{
	struct drm_hash_item *entry;
	struct hlist_head *h_list;
<<<<<<< HEAD
	struct hlist_node *list, *parent;
=======
	struct hlist_node *parent;
>>>>>>> refs/remotes/origin/master
	unsigned int hashed_key;
	unsigned long key = item->key;

	hashed_key = hash_long(key, ht->order);
	h_list = &ht->table[hashed_key];
	parent = NULL;
<<<<<<< HEAD
	hlist_for_each(list, h_list) {
		entry = hlist_entry(list, struct drm_hash_item, head);
=======
	hlist_for_each_entry(entry, h_list, head) {
>>>>>>> refs/remotes/origin/master
		if (entry->key == key)
			return -EINVAL;
		if (entry->key > key)
			break;
<<<<<<< HEAD
		parent = list;
	}
	if (parent) {
		hlist_add_after(parent, &item->head);
	} else {
		hlist_add_head(&item->head, h_list);
=======
		parent = &entry->head;
	}
	if (parent) {
		hlist_add_after_rcu(parent, &item->head);
	} else {
		hlist_add_head_rcu(&item->head, h_list);
>>>>>>> refs/remotes/origin/master
	}
	return 0;
}
EXPORT_SYMBOL(drm_ht_insert_item);

/*
 * Just insert an item and return any "bits" bit key that hasn't been
 * used before.
 */
int drm_ht_just_insert_please(struct drm_open_hash *ht, struct drm_hash_item *item,
			      unsigned long seed, int bits, int shift,
			      unsigned long add)
{
	int ret;
	unsigned long mask = (1 << bits) - 1;
	unsigned long first, unshifted_key;

	unshifted_key = hash_long(seed, bits);
	first = unshifted_key;
	do {
		item->key = (unshifted_key << shift) + add;
		ret = drm_ht_insert_item(ht, item);
		if (ret)
			unshifted_key = (unshifted_key + 1) & mask;
	} while(ret && (unshifted_key != first));

	if (ret) {
		DRM_ERROR("Available key bit space exhausted\n");
		return -EINVAL;
	}
	return 0;
}
EXPORT_SYMBOL(drm_ht_just_insert_please);

int drm_ht_find_item(struct drm_open_hash *ht, unsigned long key,
		     struct drm_hash_item **item)
{
	struct hlist_node *list;

<<<<<<< HEAD
	list = drm_ht_find_key(ht, key);
=======
	list = drm_ht_find_key_rcu(ht, key);
>>>>>>> refs/remotes/origin/master
	if (!list)
		return -EINVAL;

	*item = hlist_entry(list, struct drm_hash_item, head);
	return 0;
}
EXPORT_SYMBOL(drm_ht_find_item);

int drm_ht_remove_key(struct drm_open_hash *ht, unsigned long key)
{
	struct hlist_node *list;

	list = drm_ht_find_key(ht, key);
	if (list) {
<<<<<<< HEAD
		hlist_del_init(list);
=======
		hlist_del_init_rcu(list);
>>>>>>> refs/remotes/origin/master
		return 0;
	}
	return -EINVAL;
}

int drm_ht_remove_item(struct drm_open_hash *ht, struct drm_hash_item *item)
{
<<<<<<< HEAD
	hlist_del_init(&item->head);
=======
	hlist_del_init_rcu(&item->head);
>>>>>>> refs/remotes/origin/master
	return 0;
}
EXPORT_SYMBOL(drm_ht_remove_item);

void drm_ht_remove(struct drm_open_hash *ht)
{
	if (ht->table) {
		if ((PAGE_SIZE / sizeof(*ht->table)) >> ht->order)
			kfree(ht->table);
		else
			vfree(ht->table);
		ht->table = NULL;
	}
}
EXPORT_SYMBOL(drm_ht_remove);
