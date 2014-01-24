/*
 * gh.c
 *
 * DSP-BIOS Bridge driver support functions for TI OMAP processors.
 *
 * Copyright (C) 2005-2006 Texas Instruments, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

<<<<<<< HEAD
#include <linux/types.h>

#include <dspbridge/host_os.h>
#include <dspbridge/gh.h>

struct element {
	struct element *next;
	u8 data[1];
};

struct gh_t_hash_tab {
	u16 max_bucket;
	u16 val_size;
	struct element **buckets;
	 u16(*hash) (void *, u16);
	 bool(*match) (void *, void *);
	void (*delete) (void *);
};

static void noop(void *p);

=======
#include <linux/err.h>
#include <linux/hashtable.h>
#include <linux/slab.h>

struct gh_node {
	struct hlist_node hl;
	u8 data[0];
};

#define GH_HASH_ORDER 8

struct gh_t_hash_tab {
	u32 val_size;
	DECLARE_HASHTABLE(hash_table, GH_HASH_ORDER);
	u32 (*hash)(const void *key);
	bool (*match)(const void *key, const void *value);
	void (*delete)(void *key);
};

>>>>>>> refs/remotes/origin/master
/*
 *  ======== gh_create ========
 */

<<<<<<< HEAD
struct gh_t_hash_tab *gh_create(u16 max_bucket, u16 val_size,
				u16(*hash) (void *, u16), bool(*match) (void *,
									void *),
				void (*delete) (void *))
{
	struct gh_t_hash_tab *hash_tab;
	u16 i;
	hash_tab = kzalloc(sizeof(struct gh_t_hash_tab), GFP_KERNEL);
	if (hash_tab == NULL)
		return NULL;
	hash_tab->max_bucket = max_bucket;
	hash_tab->val_size = val_size;
	hash_tab->hash = hash;
	hash_tab->match = match;
	hash_tab->delete = delete == NULL ? noop : delete;

<<<<<<< HEAD
	hash_tab->buckets = (struct element **)
=======
	hash_tab->buckets =
>>>>>>> refs/remotes/origin/cm-10.0
	    kzalloc(sizeof(struct element *) * max_bucket, GFP_KERNEL);
	if (hash_tab->buckets == NULL) {
		gh_delete(hash_tab);
		return NULL;
	}

	for (i = 0; i < max_bucket; i++)
		hash_tab->buckets[i] = NULL;
=======
struct gh_t_hash_tab *gh_create(u32 val_size, u32 (*hash)(const void *),
				bool (*match)(const void *, const void *),
				void (*delete)(void *))
{
	struct gh_t_hash_tab *hash_tab;

	hash_tab = kzalloc(sizeof(struct gh_t_hash_tab), GFP_KERNEL);
	if (!hash_tab)
		return ERR_PTR(-ENOMEM);

	hash_init(hash_tab->hash_table);

	hash_tab->val_size = val_size;
	hash_tab->hash = hash;
	hash_tab->match = match;
	hash_tab->delete = delete;
>>>>>>> refs/remotes/origin/master

	return hash_tab;
}

/*
 *  ======== gh_delete ========
 */
void gh_delete(struct gh_t_hash_tab *hash_tab)
{
<<<<<<< HEAD
	struct element *elem, *next;
	u16 i;

	if (hash_tab != NULL) {
		if (hash_tab->buckets != NULL) {
			for (i = 0; i < hash_tab->max_bucket; i++) {
				for (elem = hash_tab->buckets[i]; elem != NULL;
				     elem = next) {
					next = elem->next;
					(*hash_tab->delete) (elem->data);
					kfree(elem);
				}
			}

			kfree(hash_tab->buckets);
=======
	struct gh_node *n;
	struct hlist_node *tmp;
	u32 i;

	if (hash_tab) {
		hash_for_each_safe(hash_tab->hash_table, i, tmp, n, hl) {
			hash_del(&n->hl);
			if (hash_tab->delete)
				hash_tab->delete(n->data);
			kfree(n);
>>>>>>> refs/remotes/origin/master
		}

		kfree(hash_tab);
	}
}

/*
<<<<<<< HEAD
<<<<<<< HEAD
 *  ======== gh_exit ========
 */

void gh_exit(void)
{
	/* Do nothing */
}

/*
=======
>>>>>>> refs/remotes/origin/cm-10.0
 *  ======== gh_find ========
 */

void *gh_find(struct gh_t_hash_tab *hash_tab, void *key)
{
	struct element *elem;

	elem = hash_tab->buckets[(*hash_tab->hash) (key, hash_tab->max_bucket)];

	for (; elem; elem = elem->next) {
		if ((*hash_tab->match) (key, elem->data))
			return elem->data;
	}

	return NULL;
}

/*
<<<<<<< HEAD
 *  ======== gh_init ========
 */

void gh_init(void)
{
	/* Do nothing */
}

/*
=======
>>>>>>> refs/remotes/origin/cm-10.0
 *  ======== gh_insert ========
 */

void *gh_insert(struct gh_t_hash_tab *hash_tab, void *key, void *value)
{
	struct element *elem;
	u16 i;
	char *src, *dst;

	elem = kzalloc(sizeof(struct element) - 1 + hash_tab->val_size,
			GFP_KERNEL);
	if (elem != NULL) {

		dst = (char *)elem->data;
		src = (char *)value;
		for (i = 0; i < hash_tab->val_size; i++)
			*dst++ = *src++;

		i = (*hash_tab->hash) (key, hash_tab->max_bucket);
		elem->next = hash_tab->buckets[i];
		hash_tab->buckets[i] = elem;

		return elem->data;
	}

	return NULL;
}

/*
 *  ======== noop ========
 */
/* ARGSUSED */
static void noop(void *p)
{
	p = p;			/* stifle compiler warning */
=======
 *  ======== gh_find ========
 */

void *gh_find(struct gh_t_hash_tab *hash_tab, const void *key)
{
	struct gh_node *n;
	u32 key_hash = hash_tab->hash(key);

	hash_for_each_possible(hash_tab->hash_table, n, hl, key_hash) {
		if (hash_tab->match(key, n->data))
			return n->data;
	}

	return ERR_PTR(-ENODATA);
}

/*
 *  ======== gh_insert ========
 */

void *gh_insert(struct gh_t_hash_tab *hash_tab, const void *key,
		const void *value)
{
	struct gh_node *n;

	n = kmalloc(sizeof(struct gh_node) + hash_tab->val_size,
			GFP_KERNEL);

	if (!n)
		return ERR_PTR(-ENOMEM);

	INIT_HLIST_NODE(&n->hl);
	hash_add(hash_tab->hash_table, &n->hl, hash_tab->hash(key));
	memcpy(n->data, value, hash_tab->val_size);

	return n->data;
>>>>>>> refs/remotes/origin/master
}

#ifdef CONFIG_TIDSPBRIDGE_BACKTRACE
/**
 * gh_iterate() - This function goes through all the elements in the hash table
 *		looking for the dsp symbols.
 * @hash_tab:	Hash table
 * @callback:	pointer to callback function
 * @user_data:	User data, contains the find_symbol_context pointer
 *
 */
void gh_iterate(struct gh_t_hash_tab *hash_tab,
		void (*callback)(void *, void *), void *user_data)
{
<<<<<<< HEAD
	struct element *elem;
	u32 i;

	if (hash_tab && hash_tab->buckets)
		for (i = 0; i < hash_tab->max_bucket; i++) {
			elem = hash_tab->buckets[i];
			while (elem) {
				callback(&elem->data, user_data);
				elem = elem->next;
			}
		}
=======
	struct gh_node *n;
	u32 i;

	if (!hash_tab)
		return;

	hash_for_each(hash_tab->hash_table, i, n, hl)
		callback(&n->data, user_data);
>>>>>>> refs/remotes/origin/master
}
#endif
