/*
 * Copyright (C) 2007 Oracle.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License v2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 021110-1307, USA.
 */

#include <linux/slab.h>
#include <linux/blkdev.h>
#include <linux/writeback.h>
#include <linux/pagevec.h>
#include "ctree.h"
#include "transaction.h"
#include "btrfs_inode.h"
#include "extent_io.h"
<<<<<<< HEAD
=======
#include "disk-io.h"

static struct kmem_cache *btrfs_ordered_extent_cache;
>>>>>>> refs/remotes/origin/master

static u64 entry_end(struct btrfs_ordered_extent *entry)
{
	if (entry->file_offset + entry->len < entry->file_offset)
		return (u64)-1;
	return entry->file_offset + entry->len;
}

/* returns NULL if the insertion worked, or it returns the node it did find
 * in the tree
 */
static struct rb_node *tree_insert(struct rb_root *root, u64 file_offset,
				   struct rb_node *node)
{
	struct rb_node **p = &root->rb_node;
	struct rb_node *parent = NULL;
	struct btrfs_ordered_extent *entry;

	while (*p) {
		parent = *p;
		entry = rb_entry(parent, struct btrfs_ordered_extent, rb_node);

		if (file_offset < entry->file_offset)
			p = &(*p)->rb_left;
		else if (file_offset >= entry_end(entry))
			p = &(*p)->rb_right;
		else
			return parent;
	}

	rb_link_node(node, parent, p);
	rb_insert_color(node, root);
	return NULL;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static void ordered_data_tree_panic(struct inode *inode, int errno,
					       u64 offset)
{
	struct btrfs_fs_info *fs_info = btrfs_sb(inode->i_sb);
	btrfs_panic(fs_info, errno, "Inconsistency in ordered tree at offset "
<<<<<<< HEAD
		    "%llu\n", (unsigned long long)offset);
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
		    "%llu\n", offset);
}

>>>>>>> refs/remotes/origin/master
/*
 * look for a given offset in the tree, and if it can't be found return the
 * first lesser offset
 */
static struct rb_node *__tree_search(struct rb_root *root, u64 file_offset,
				     struct rb_node **prev_ret)
{
	struct rb_node *n = root->rb_node;
	struct rb_node *prev = NULL;
	struct rb_node *test;
	struct btrfs_ordered_extent *entry;
	struct btrfs_ordered_extent *prev_entry = NULL;

	while (n) {
		entry = rb_entry(n, struct btrfs_ordered_extent, rb_node);
		prev = n;
		prev_entry = entry;

		if (file_offset < entry->file_offset)
			n = n->rb_left;
		else if (file_offset >= entry_end(entry))
			n = n->rb_right;
		else
			return n;
	}
	if (!prev_ret)
		return NULL;

	while (prev && file_offset >= entry_end(prev_entry)) {
		test = rb_next(prev);
		if (!test)
			break;
		prev_entry = rb_entry(test, struct btrfs_ordered_extent,
				      rb_node);
		if (file_offset < entry_end(prev_entry))
			break;

		prev = test;
	}
	if (prev)
		prev_entry = rb_entry(prev, struct btrfs_ordered_extent,
				      rb_node);
	while (prev && file_offset < entry_end(prev_entry)) {
		test = rb_prev(prev);
		if (!test)
			break;
		prev_entry = rb_entry(test, struct btrfs_ordered_extent,
				      rb_node);
		prev = test;
	}
	*prev_ret = prev;
	return NULL;
}

/*
 * helper to check if a given offset is inside a given entry
 */
static int offset_in_entry(struct btrfs_ordered_extent *entry, u64 file_offset)
{
	if (file_offset < entry->file_offset ||
	    entry->file_offset + entry->len <= file_offset)
		return 0;
	return 1;
}

static int range_overlaps(struct btrfs_ordered_extent *entry, u64 file_offset,
			  u64 len)
{
	if (file_offset + len <= entry->file_offset ||
	    entry->file_offset + entry->len <= file_offset)
		return 0;
	return 1;
}

/*
 * look find the first ordered struct that has this offset, otherwise
 * the first one less than this offset
 */
static inline struct rb_node *tree_search(struct btrfs_ordered_inode_tree *tree,
					  u64 file_offset)
{
	struct rb_root *root = &tree->tree;
	struct rb_node *prev = NULL;
	struct rb_node *ret;
	struct btrfs_ordered_extent *entry;

	if (tree->last) {
		entry = rb_entry(tree->last, struct btrfs_ordered_extent,
				 rb_node);
		if (offset_in_entry(entry, file_offset))
			return tree->last;
	}
	ret = __tree_search(root, file_offset, &prev);
	if (!ret)
		ret = prev;
	if (ret)
		tree->last = ret;
	return ret;
}

/* allocate and add a new ordered_extent into the per-inode tree.
 * file_offset is the logical offset in the file
 *
 * start is the disk block number of an extent already reserved in the
 * extent allocation tree
 *
 * len is the length of the extent
 *
 * The tree is given a single reference on the ordered extent that was
 * inserted.
 */
static int __btrfs_add_ordered_extent(struct inode *inode, u64 file_offset,
				      u64 start, u64 len, u64 disk_len,
				      int type, int dio, int compress_type)
{
<<<<<<< HEAD
=======
	struct btrfs_root *root = BTRFS_I(inode)->root;
>>>>>>> refs/remotes/origin/master
	struct btrfs_ordered_inode_tree *tree;
	struct rb_node *node;
	struct btrfs_ordered_extent *entry;

	tree = &BTRFS_I(inode)->ordered_tree;
<<<<<<< HEAD
	entry = kzalloc(sizeof(*entry), GFP_NOFS);
=======
	entry = kmem_cache_zalloc(btrfs_ordered_extent_cache, GFP_NOFS);
>>>>>>> refs/remotes/origin/master
	if (!entry)
		return -ENOMEM;

	entry->file_offset = file_offset;
	entry->start = start;
	entry->len = len;
<<<<<<< HEAD
	entry->disk_len = disk_len;
	entry->bytes_left = len;
	entry->inode = inode;
	entry->compress_type = compress_type;
=======
	if (!(BTRFS_I(inode)->flags & BTRFS_INODE_NODATASUM) &&
	    !(type == BTRFS_ORDERED_NOCOW))
		entry->csum_bytes_left = disk_len;
	entry->disk_len = disk_len;
	entry->bytes_left = len;
	entry->inode = igrab(inode);
	entry->compress_type = compress_type;
	entry->truncated_len = (u64)-1;
>>>>>>> refs/remotes/origin/master
	if (type != BTRFS_ORDERED_IO_DONE && type != BTRFS_ORDERED_COMPLETE)
		set_bit(type, &entry->flags);

	if (dio)
		set_bit(BTRFS_ORDERED_DIRECT, &entry->flags);

	/* one ref for the tree */
	atomic_set(&entry->refs, 1);
	init_waitqueue_head(&entry->wait);
	INIT_LIST_HEAD(&entry->list);
	INIT_LIST_HEAD(&entry->root_extent_list);
<<<<<<< HEAD

	trace_btrfs_ordered_extent_add(inode, entry);

	spin_lock(&tree->lock);
	node = tree_insert(&tree->tree, file_offset,
			   &entry->rb_node);
<<<<<<< HEAD
	BUG_ON(node);
=======
	if (node)
		ordered_data_tree_panic(inode, -EEXIST, file_offset);
>>>>>>> refs/remotes/origin/cm-10.0
	spin_unlock(&tree->lock);

	spin_lock(&BTRFS_I(inode)->root->fs_info->ordered_extent_lock);
	list_add_tail(&entry->root_extent_list,
		      &BTRFS_I(inode)->root->fs_info->ordered_extents);
	spin_unlock(&BTRFS_I(inode)->root->fs_info->ordered_extent_lock);

<<<<<<< HEAD
	BUG_ON(node);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	INIT_LIST_HEAD(&entry->work_list);
	init_completion(&entry->completion);
	INIT_LIST_HEAD(&entry->log_list);

	trace_btrfs_ordered_extent_add(inode, entry);

	spin_lock_irq(&tree->lock);
	node = tree_insert(&tree->tree, file_offset,
			   &entry->rb_node);
	if (node)
		ordered_data_tree_panic(inode, -EEXIST, file_offset);
	spin_unlock_irq(&tree->lock);

	spin_lock(&root->ordered_extent_lock);
	list_add_tail(&entry->root_extent_list,
		      &root->ordered_extents);
	root->nr_ordered_extents++;
	if (root->nr_ordered_extents == 1) {
		spin_lock(&root->fs_info->ordered_root_lock);
		BUG_ON(!list_empty(&root->ordered_root));
		list_add_tail(&root->ordered_root,
			      &root->fs_info->ordered_roots);
		spin_unlock(&root->fs_info->ordered_root_lock);
	}
	spin_unlock(&root->ordered_extent_lock);

>>>>>>> refs/remotes/origin/master
	return 0;
}

int btrfs_add_ordered_extent(struct inode *inode, u64 file_offset,
			     u64 start, u64 len, u64 disk_len, int type)
{
	return __btrfs_add_ordered_extent(inode, file_offset, start, len,
					  disk_len, type, 0,
					  BTRFS_COMPRESS_NONE);
}

int btrfs_add_ordered_extent_dio(struct inode *inode, u64 file_offset,
				 u64 start, u64 len, u64 disk_len, int type)
{
	return __btrfs_add_ordered_extent(inode, file_offset, start, len,
					  disk_len, type, 1,
					  BTRFS_COMPRESS_NONE);
}

int btrfs_add_ordered_extent_compress(struct inode *inode, u64 file_offset,
				      u64 start, u64 len, u64 disk_len,
				      int type, int compress_type)
{
	return __btrfs_add_ordered_extent(inode, file_offset, start, len,
					  disk_len, type, 0,
					  compress_type);
}

/*
 * Add a struct btrfs_ordered_sum into the list of checksums to be inserted
 * when an ordered extent is finished.  If the list covers more than one
 * ordered extent, it is split across multiples.
 */
<<<<<<< HEAD
<<<<<<< HEAD
int btrfs_add_ordered_sum(struct inode *inode,
			  struct btrfs_ordered_extent *entry,
			  struct btrfs_ordered_sum *sum)
=======
void btrfs_add_ordered_sum(struct inode *inode,
			   struct btrfs_ordered_extent *entry,
			   struct btrfs_ordered_sum *sum)
>>>>>>> refs/remotes/origin/cm-10.0
=======
void btrfs_add_ordered_sum(struct inode *inode,
			   struct btrfs_ordered_extent *entry,
			   struct btrfs_ordered_sum *sum)
>>>>>>> refs/remotes/origin/master
{
	struct btrfs_ordered_inode_tree *tree;

	tree = &BTRFS_I(inode)->ordered_tree;
<<<<<<< HEAD
	spin_lock(&tree->lock);
	list_add_tail(&sum->list, &entry->list);
	spin_unlock(&tree->lock);
<<<<<<< HEAD
	return 0;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	spin_lock_irq(&tree->lock);
	list_add_tail(&sum->list, &entry->list);
	WARN_ON(entry->csum_bytes_left < sum->len);
	entry->csum_bytes_left -= sum->len;
	if (entry->csum_bytes_left == 0)
		wake_up(&entry->wait);
	spin_unlock_irq(&tree->lock);
>>>>>>> refs/remotes/origin/master
}

/*
 * this is used to account for finished IO across a given range
 * of the file.  The IO may span ordered extents.  If
 * a given ordered_extent is completely done, 1 is returned, otherwise
 * 0.
 *
 * test_and_set_bit on a flag in the struct btrfs_ordered_extent is used
 * to make sure this function only returns 1 once for a given ordered extent.
 *
 * file_offset is updated to one byte past the range that is recorded as
 * complete.  This allows you to walk forward in the file.
 */
int btrfs_dec_test_first_ordered_pending(struct inode *inode,
				   struct btrfs_ordered_extent **cached,
<<<<<<< HEAD
				   u64 *file_offset, u64 io_size)
=======
				   u64 *file_offset, u64 io_size, int uptodate)
>>>>>>> refs/remotes/origin/master
{
	struct btrfs_ordered_inode_tree *tree;
	struct rb_node *node;
	struct btrfs_ordered_extent *entry = NULL;
	int ret;
<<<<<<< HEAD
=======
	unsigned long flags;
>>>>>>> refs/remotes/origin/master
	u64 dec_end;
	u64 dec_start;
	u64 to_dec;

	tree = &BTRFS_I(inode)->ordered_tree;
<<<<<<< HEAD
	spin_lock(&tree->lock);
=======
	spin_lock_irqsave(&tree->lock, flags);
>>>>>>> refs/remotes/origin/master
	node = tree_search(tree, *file_offset);
	if (!node) {
		ret = 1;
		goto out;
	}

	entry = rb_entry(node, struct btrfs_ordered_extent, rb_node);
	if (!offset_in_entry(entry, *file_offset)) {
		ret = 1;
		goto out;
	}

	dec_start = max(*file_offset, entry->file_offset);
	dec_end = min(*file_offset + io_size, entry->file_offset +
		      entry->len);
	*file_offset = dec_end;
	if (dec_start > dec_end) {
		printk(KERN_CRIT "bad ordering dec_start %llu end %llu\n",
<<<<<<< HEAD
		       (unsigned long long)dec_start,
		       (unsigned long long)dec_end);
=======
		       dec_start, dec_end);
>>>>>>> refs/remotes/origin/master
	}
	to_dec = dec_end - dec_start;
	if (to_dec > entry->bytes_left) {
		printk(KERN_CRIT "bad ordered accounting left %llu size %llu\n",
<<<<<<< HEAD
		       (unsigned long long)entry->bytes_left,
		       (unsigned long long)to_dec);
	}
	entry->bytes_left -= to_dec;
=======
		       entry->bytes_left, to_dec);
	}
	entry->bytes_left -= to_dec;
	if (!uptodate)
		set_bit(BTRFS_ORDERED_IOERR, &entry->flags);

>>>>>>> refs/remotes/origin/master
	if (entry->bytes_left == 0)
		ret = test_and_set_bit(BTRFS_ORDERED_IO_DONE, &entry->flags);
	else
		ret = 1;
out:
	if (!ret && cached && entry) {
		*cached = entry;
		atomic_inc(&entry->refs);
	}
<<<<<<< HEAD
	spin_unlock(&tree->lock);
=======
	spin_unlock_irqrestore(&tree->lock, flags);
>>>>>>> refs/remotes/origin/master
	return ret == 0;
}

/*
 * this is used to account for finished IO across a given range
 * of the file.  The IO should not span ordered extents.  If
 * a given ordered_extent is completely done, 1 is returned, otherwise
 * 0.
 *
 * test_and_set_bit on a flag in the struct btrfs_ordered_extent is used
 * to make sure this function only returns 1 once for a given ordered extent.
 */
int btrfs_dec_test_ordered_pending(struct inode *inode,
				   struct btrfs_ordered_extent **cached,
<<<<<<< HEAD
				   u64 file_offset, u64 io_size)
=======
				   u64 file_offset, u64 io_size, int uptodate)
>>>>>>> refs/remotes/origin/master
{
	struct btrfs_ordered_inode_tree *tree;
	struct rb_node *node;
	struct btrfs_ordered_extent *entry = NULL;
<<<<<<< HEAD
	int ret;

	tree = &BTRFS_I(inode)->ordered_tree;
	spin_lock(&tree->lock);
=======
	unsigned long flags;
	int ret;

	tree = &BTRFS_I(inode)->ordered_tree;
	spin_lock_irqsave(&tree->lock, flags);
	if (cached && *cached) {
		entry = *cached;
		goto have_entry;
	}

>>>>>>> refs/remotes/origin/master
	node = tree_search(tree, file_offset);
	if (!node) {
		ret = 1;
		goto out;
	}

	entry = rb_entry(node, struct btrfs_ordered_extent, rb_node);
<<<<<<< HEAD
=======
have_entry:
>>>>>>> refs/remotes/origin/master
	if (!offset_in_entry(entry, file_offset)) {
		ret = 1;
		goto out;
	}

	if (io_size > entry->bytes_left) {
		printk(KERN_CRIT "bad ordered accounting left %llu size %llu\n",
<<<<<<< HEAD
		       (unsigned long long)entry->bytes_left,
		       (unsigned long long)io_size);
	}
	entry->bytes_left -= io_size;
=======
		       entry->bytes_left, io_size);
	}
	entry->bytes_left -= io_size;
	if (!uptodate)
		set_bit(BTRFS_ORDERED_IOERR, &entry->flags);

>>>>>>> refs/remotes/origin/master
	if (entry->bytes_left == 0)
		ret = test_and_set_bit(BTRFS_ORDERED_IO_DONE, &entry->flags);
	else
		ret = 1;
out:
	if (!ret && cached && entry) {
		*cached = entry;
		atomic_inc(&entry->refs);
	}
<<<<<<< HEAD
	spin_unlock(&tree->lock);
	return ret == 0;
}

=======
	spin_unlock_irqrestore(&tree->lock, flags);
	return ret == 0;
}

/* Needs to either be called under a log transaction or the log_mutex */
void btrfs_get_logged_extents(struct btrfs_root *log, struct inode *inode)
{
	struct btrfs_ordered_inode_tree *tree;
	struct btrfs_ordered_extent *ordered;
	struct rb_node *n;
	int index = log->log_transid % 2;

	tree = &BTRFS_I(inode)->ordered_tree;
	spin_lock_irq(&tree->lock);
	for (n = rb_first(&tree->tree); n; n = rb_next(n)) {
		ordered = rb_entry(n, struct btrfs_ordered_extent, rb_node);
		spin_lock(&log->log_extents_lock[index]);
		if (list_empty(&ordered->log_list)) {
			list_add_tail(&ordered->log_list, &log->logged_list[index]);
			atomic_inc(&ordered->refs);
		}
		spin_unlock(&log->log_extents_lock[index]);
	}
	spin_unlock_irq(&tree->lock);
}

void btrfs_wait_logged_extents(struct btrfs_root *log, u64 transid)
{
	struct btrfs_ordered_extent *ordered;
	int index = transid % 2;

	spin_lock_irq(&log->log_extents_lock[index]);
	while (!list_empty(&log->logged_list[index])) {
		ordered = list_first_entry(&log->logged_list[index],
					   struct btrfs_ordered_extent,
					   log_list);
		list_del_init(&ordered->log_list);
		spin_unlock_irq(&log->log_extents_lock[index]);
		wait_event(ordered->wait, test_bit(BTRFS_ORDERED_IO_DONE,
						   &ordered->flags));
		btrfs_put_ordered_extent(ordered);
		spin_lock_irq(&log->log_extents_lock[index]);
	}
	spin_unlock_irq(&log->log_extents_lock[index]);
}

void btrfs_free_logged_extents(struct btrfs_root *log, u64 transid)
{
	struct btrfs_ordered_extent *ordered;
	int index = transid % 2;

	spin_lock_irq(&log->log_extents_lock[index]);
	while (!list_empty(&log->logged_list[index])) {
		ordered = list_first_entry(&log->logged_list[index],
					   struct btrfs_ordered_extent,
					   log_list);
		list_del_init(&ordered->log_list);
		spin_unlock_irq(&log->log_extents_lock[index]);
		btrfs_put_ordered_extent(ordered);
		spin_lock_irq(&log->log_extents_lock[index]);
	}
	spin_unlock_irq(&log->log_extents_lock[index]);
}

>>>>>>> refs/remotes/origin/master
/*
 * used to drop a reference on an ordered extent.  This will free
 * the extent if the last reference is dropped
 */
<<<<<<< HEAD
<<<<<<< HEAD
int btrfs_put_ordered_extent(struct btrfs_ordered_extent *entry)
=======
void btrfs_put_ordered_extent(struct btrfs_ordered_extent *entry)
>>>>>>> refs/remotes/origin/cm-10.0
=======
void btrfs_put_ordered_extent(struct btrfs_ordered_extent *entry)
>>>>>>> refs/remotes/origin/master
{
	struct list_head *cur;
	struct btrfs_ordered_sum *sum;

	trace_btrfs_ordered_extent_put(entry->inode, entry);

	if (atomic_dec_and_test(&entry->refs)) {
<<<<<<< HEAD
=======
		if (entry->inode)
			btrfs_add_delayed_iput(entry->inode);
>>>>>>> refs/remotes/origin/master
		while (!list_empty(&entry->list)) {
			cur = entry->list.next;
			sum = list_entry(cur, struct btrfs_ordered_sum, list);
			list_del(&sum->list);
			kfree(sum);
		}
<<<<<<< HEAD
		kfree(entry);
	}
<<<<<<< HEAD
	return 0;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
		kmem_cache_free(btrfs_ordered_extent_cache, entry);
	}
>>>>>>> refs/remotes/origin/master
}

/*
 * remove an ordered extent from the tree.  No references are dropped
<<<<<<< HEAD
 * and you must wake_up entry->wait.  You must hold the tree lock
 * while you call this function.
 */
<<<<<<< HEAD
static int __btrfs_remove_ordered_extent(struct inode *inode,
				struct btrfs_ordered_extent *entry)
=======
static void __btrfs_remove_ordered_extent(struct inode *inode,
					  struct btrfs_ordered_extent *entry)
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * and waiters are woken up.
 */
void btrfs_remove_ordered_extent(struct inode *inode,
				 struct btrfs_ordered_extent *entry)
>>>>>>> refs/remotes/origin/master
{
	struct btrfs_ordered_inode_tree *tree;
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct rb_node *node;

	tree = &BTRFS_I(inode)->ordered_tree;
<<<<<<< HEAD
=======
	spin_lock_irq(&tree->lock);
>>>>>>> refs/remotes/origin/master
	node = &entry->rb_node;
	rb_erase(node, &tree->tree);
	tree->last = NULL;
	set_bit(BTRFS_ORDERED_COMPLETE, &entry->flags);
<<<<<<< HEAD

	spin_lock(&root->fs_info->ordered_extent_lock);
	list_del_init(&entry->root_extent_list);
=======
	spin_unlock_irq(&tree->lock);

	spin_lock(&root->ordered_extent_lock);
	list_del_init(&entry->root_extent_list);
	root->nr_ordered_extents--;
>>>>>>> refs/remotes/origin/master

	trace_btrfs_ordered_extent_remove(inode, entry);

	/*
	 * we have no more ordered extents for this inode and
	 * no dirty pages.  We can safely remove it from the
	 * list of ordered extents
	 */
	if (RB_EMPTY_ROOT(&tree->tree) &&
	    !mapping_tagged(inode->i_mapping, PAGECACHE_TAG_DIRTY)) {
<<<<<<< HEAD
		list_del_init(&BTRFS_I(inode)->ordered_operations);
	}
	spin_unlock(&root->fs_info->ordered_extent_lock);
<<<<<<< HEAD

	return 0;
=======
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * remove an ordered extent from the tree.  No references are dropped
 * but any waiters are woken.
 */
<<<<<<< HEAD
int btrfs_remove_ordered_extent(struct inode *inode,
				struct btrfs_ordered_extent *entry)
{
	struct btrfs_ordered_inode_tree *tree;
	int ret;

	tree = &BTRFS_I(inode)->ordered_tree;
	spin_lock(&tree->lock);
	ret = __btrfs_remove_ordered_extent(inode, entry);
	spin_unlock(&tree->lock);
	wake_up(&entry->wait);

	return ret;
=======
void btrfs_remove_ordered_extent(struct inode *inode,
				 struct btrfs_ordered_extent *entry)
{
	struct btrfs_ordered_inode_tree *tree;

	tree = &BTRFS_I(inode)->ordered_tree;
	spin_lock(&tree->lock);
	__btrfs_remove_ordered_extent(inode, entry);
	spin_unlock(&tree->lock);
	wake_up(&entry->wait);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		spin_lock(&root->fs_info->ordered_root_lock);
		list_del_init(&BTRFS_I(inode)->ordered_operations);
		spin_unlock(&root->fs_info->ordered_root_lock);
	}

	if (!root->nr_ordered_extents) {
		spin_lock(&root->fs_info->ordered_root_lock);
		BUG_ON(list_empty(&root->ordered_root));
		list_del_init(&root->ordered_root);
		spin_unlock(&root->fs_info->ordered_root_lock);
	}
	spin_unlock(&root->ordered_extent_lock);
	wake_up(&entry->wait);
}

static void btrfs_run_ordered_extent_work(struct btrfs_work *work)
{
	struct btrfs_ordered_extent *ordered;

	ordered = container_of(work, struct btrfs_ordered_extent, flush_work);
	btrfs_start_ordered_extent(ordered->inode, ordered, 1);
	complete(&ordered->completion);
>>>>>>> refs/remotes/origin/master
}

/*
 * wait for all the ordered extents in a root.  This is done when balancing
 * space between drives.
 */
<<<<<<< HEAD
<<<<<<< HEAD
int btrfs_wait_ordered_extents(struct btrfs_root *root,
			       int nocow_only, int delay_iput)
=======
void btrfs_wait_ordered_extents(struct btrfs_root *root,
				int nocow_only, int delay_iput)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct list_head splice;
	struct list_head *cur;
	struct btrfs_ordered_extent *ordered;
	struct inode *inode;

	INIT_LIST_HEAD(&splice);

	spin_lock(&root->fs_info->ordered_extent_lock);
	list_splice_init(&root->fs_info->ordered_extents, &splice);
	while (!list_empty(&splice)) {
		cur = splice.next;
		ordered = list_entry(cur, struct btrfs_ordered_extent,
				     root_extent_list);
		if (nocow_only &&
		    !test_bit(BTRFS_ORDERED_NOCOW, &ordered->flags) &&
		    !test_bit(BTRFS_ORDERED_PREALLOC, &ordered->flags)) {
			list_move(&ordered->root_extent_list,
				  &root->fs_info->ordered_extents);
			cond_resched_lock(&root->fs_info->ordered_extent_lock);
			continue;
		}

		list_del_init(&ordered->root_extent_list);
		atomic_inc(&ordered->refs);

		/*
		 * the inode may be getting freed (in sys_unlink path).
		 */
		inode = igrab(ordered->inode);

		spin_unlock(&root->fs_info->ordered_extent_lock);

		if (inode) {
			btrfs_start_ordered_extent(inode, ordered, 1);
			btrfs_put_ordered_extent(ordered);
			if (delay_iput)
				btrfs_add_delayed_iput(inode);
			else
				iput(inode);
		} else {
			btrfs_put_ordered_extent(ordered);
		}

		spin_lock(&root->fs_info->ordered_extent_lock);
	}
	spin_unlock(&root->fs_info->ordered_extent_lock);
<<<<<<< HEAD
	return 0;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
int btrfs_wait_ordered_extents(struct btrfs_root *root, int nr)
{
	struct list_head splice, works;
	struct btrfs_ordered_extent *ordered, *next;
	int count = 0;

	INIT_LIST_HEAD(&splice);
	INIT_LIST_HEAD(&works);

	mutex_lock(&root->fs_info->ordered_operations_mutex);
	spin_lock(&root->ordered_extent_lock);
	list_splice_init(&root->ordered_extents, &splice);
	while (!list_empty(&splice) && nr) {
		ordered = list_first_entry(&splice, struct btrfs_ordered_extent,
					   root_extent_list);
		list_move_tail(&ordered->root_extent_list,
			       &root->ordered_extents);
		atomic_inc(&ordered->refs);
		spin_unlock(&root->ordered_extent_lock);

		ordered->flush_work.func = btrfs_run_ordered_extent_work;
		list_add_tail(&ordered->work_list, &works);
		btrfs_queue_worker(&root->fs_info->flush_workers,
				   &ordered->flush_work);

		cond_resched();
		spin_lock(&root->ordered_extent_lock);
		if (nr != -1)
			nr--;
		count++;
	}
	list_splice_tail(&splice, &root->ordered_extents);
	spin_unlock(&root->ordered_extent_lock);

	list_for_each_entry_safe(ordered, next, &works, work_list) {
		list_del_init(&ordered->work_list);
		wait_for_completion(&ordered->completion);
		btrfs_put_ordered_extent(ordered);
		cond_resched();
	}
	mutex_unlock(&root->fs_info->ordered_operations_mutex);

	return count;
}

void btrfs_wait_ordered_roots(struct btrfs_fs_info *fs_info, int nr)
{
	struct btrfs_root *root;
	struct list_head splice;
	int done;

	INIT_LIST_HEAD(&splice);

	spin_lock(&fs_info->ordered_root_lock);
	list_splice_init(&fs_info->ordered_roots, &splice);
	while (!list_empty(&splice) && nr) {
		root = list_first_entry(&splice, struct btrfs_root,
					ordered_root);
		root = btrfs_grab_fs_root(root);
		BUG_ON(!root);
		list_move_tail(&root->ordered_root,
			       &fs_info->ordered_roots);
		spin_unlock(&fs_info->ordered_root_lock);

		done = btrfs_wait_ordered_extents(root, nr);
		btrfs_put_fs_root(root);

		spin_lock(&fs_info->ordered_root_lock);
		if (nr != -1) {
			nr -= done;
			WARN_ON(nr < 0);
		}
	}
	list_splice_tail(&splice, &fs_info->ordered_roots);
	spin_unlock(&fs_info->ordered_root_lock);
>>>>>>> refs/remotes/origin/master
}

/*
 * this is used during transaction commit to write all the inodes
 * added to the ordered operation list.  These files must be fully on
 * disk before the transaction commits.
 *
 * we have two modes here, one is to just start the IO via filemap_flush
 * and the other is to wait for all the io.  When we wait, we have an
 * extra check to make sure the ordered operation list really is empty
 * before we return
 */
<<<<<<< HEAD
<<<<<<< HEAD
int btrfs_run_ordered_operations(struct btrfs_root *root, int wait)
=======
void btrfs_run_ordered_operations(struct btrfs_root *root, int wait)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct btrfs_inode *btrfs_inode;
	struct inode *inode;
	struct list_head splice;

	INIT_LIST_HEAD(&splice);

	mutex_lock(&root->fs_info->ordered_operations_mutex);
	spin_lock(&root->fs_info->ordered_extent_lock);
again:
	list_splice_init(&root->fs_info->ordered_operations, &splice);

	while (!list_empty(&splice)) {
		btrfs_inode = list_entry(splice.next, struct btrfs_inode,
				   ordered_operations);

=======
int btrfs_run_ordered_operations(struct btrfs_trans_handle *trans,
				 struct btrfs_root *root, int wait)
{
	struct btrfs_inode *btrfs_inode;
	struct inode *inode;
	struct btrfs_transaction *cur_trans = trans->transaction;
	struct list_head splice;
	struct list_head works;
	struct btrfs_delalloc_work *work, *next;
	int ret = 0;

	INIT_LIST_HEAD(&splice);
	INIT_LIST_HEAD(&works);

	mutex_lock(&root->fs_info->ordered_extent_flush_mutex);
	spin_lock(&root->fs_info->ordered_root_lock);
	list_splice_init(&cur_trans->ordered_operations, &splice);
	while (!list_empty(&splice)) {
		btrfs_inode = list_entry(splice.next, struct btrfs_inode,
				   ordered_operations);
>>>>>>> refs/remotes/origin/master
		inode = &btrfs_inode->vfs_inode;

		list_del_init(&btrfs_inode->ordered_operations);

		/*
		 * the inode may be getting freed (in sys_unlink path).
		 */
		inode = igrab(inode);
<<<<<<< HEAD

		if (!wait && inode) {
			list_add_tail(&BTRFS_I(inode)->ordered_operations,
			      &root->fs_info->ordered_operations);
		}
		spin_unlock(&root->fs_info->ordered_extent_lock);

		if (inode) {
			if (wait)
				btrfs_wait_ordered_range(inode, 0, (u64)-1);
			else
				filemap_flush(inode->i_mapping);
			btrfs_add_delayed_iput(inode);
		}

		cond_resched();
		spin_lock(&root->fs_info->ordered_extent_lock);
	}
	if (wait && !list_empty(&root->fs_info->ordered_operations))
		goto again;

	spin_unlock(&root->fs_info->ordered_extent_lock);
	mutex_unlock(&root->fs_info->ordered_operations_mutex);
<<<<<<< HEAD

	return 0;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (!inode)
			continue;

		if (!wait)
			list_add_tail(&BTRFS_I(inode)->ordered_operations,
				      &cur_trans->ordered_operations);
		spin_unlock(&root->fs_info->ordered_root_lock);

		work = btrfs_alloc_delalloc_work(inode, wait, 1);
		if (!work) {
			spin_lock(&root->fs_info->ordered_root_lock);
			if (list_empty(&BTRFS_I(inode)->ordered_operations))
				list_add_tail(&btrfs_inode->ordered_operations,
					      &splice);
			list_splice_tail(&splice,
					 &cur_trans->ordered_operations);
			spin_unlock(&root->fs_info->ordered_root_lock);
			ret = -ENOMEM;
			goto out;
		}
		list_add_tail(&work->list, &works);
		btrfs_queue_worker(&root->fs_info->flush_workers,
				   &work->work);

		cond_resched();
		spin_lock(&root->fs_info->ordered_root_lock);
	}
	spin_unlock(&root->fs_info->ordered_root_lock);
out:
	list_for_each_entry_safe(work, next, &works, list) {
		list_del_init(&work->list);
		btrfs_wait_and_free_delalloc_work(work);
	}
	mutex_unlock(&root->fs_info->ordered_extent_flush_mutex);
	return ret;
>>>>>>> refs/remotes/origin/master
}

/*
 * Used to start IO or wait for a given ordered extent to finish.
 *
 * If wait is one, this effectively waits on page writeback for all the pages
 * in the extent, and it waits on the io completion code to insert
 * metadata into the btree corresponding to the extent
 */
void btrfs_start_ordered_extent(struct inode *inode,
				       struct btrfs_ordered_extent *entry,
				       int wait)
{
	u64 start = entry->file_offset;
	u64 end = start + entry->len - 1;

	trace_btrfs_ordered_extent_start(inode, entry);

	/*
	 * pages in the range can be dirty, clean or writeback.  We
	 * start IO on any dirty ones so the wait doesn't stall waiting
<<<<<<< HEAD
	 * for pdflush to find them
=======
	 * for the flusher thread to find them
>>>>>>> refs/remotes/origin/master
	 */
	if (!test_bit(BTRFS_ORDERED_DIRECT, &entry->flags))
		filemap_fdatawrite_range(inode->i_mapping, start, end);
	if (wait) {
		wait_event(entry->wait, test_bit(BTRFS_ORDERED_COMPLETE,
						 &entry->flags));
	}
}

/*
 * Used to wait on ordered extents across a large range of bytes.
 */
<<<<<<< HEAD
<<<<<<< HEAD
int btrfs_wait_ordered_range(struct inode *inode, u64 start, u64 len)
=======
void btrfs_wait_ordered_range(struct inode *inode, u64 start, u64 len)
>>>>>>> refs/remotes/origin/cm-10.0
{
	u64 end;
	u64 orig_end;
	struct btrfs_ordered_extent *ordered;
	int found;
=======
int btrfs_wait_ordered_range(struct inode *inode, u64 start, u64 len)
{
	int ret = 0;
	u64 end;
	u64 orig_end;
	struct btrfs_ordered_extent *ordered;
>>>>>>> refs/remotes/origin/master

	if (start + len < start) {
		orig_end = INT_LIMIT(loff_t);
	} else {
		orig_end = start + len - 1;
		if (orig_end > INT_LIMIT(loff_t))
			orig_end = INT_LIMIT(loff_t);
	}
<<<<<<< HEAD
again:
	/* start IO across the range first to instantiate any delalloc
	 * extents
	 */
	filemap_fdatawrite_range(inode->i_mapping, start, orig_end);

	/* The compression code will leave pages locked but return from
	 * writepage without setting the page writeback.  Starting again
	 * with WB_SYNC_ALL will end up waiting for the IO to actually start.
	 */
	filemap_fdatawrite_range(inode->i_mapping, start, orig_end);

	filemap_fdatawait_range(inode->i_mapping, start, orig_end);

	end = orig_end;
	found = 0;
=======

	/* start IO across the range first to instantiate any delalloc
	 * extents
	 */
	ret = filemap_fdatawrite_range(inode->i_mapping, start, orig_end);
	if (ret)
		return ret;
	/*
	 * So with compression we will find and lock a dirty page and clear the
	 * first one as dirty, setup an async extent, and immediately return
	 * with the entire range locked but with nobody actually marked with
	 * writeback.  So we can't just filemap_write_and_wait_range() and
	 * expect it to work since it will just kick off a thread to do the
	 * actual work.  So we need to call filemap_fdatawrite_range _again_
	 * since it will wait on the page lock, which won't be unlocked until
	 * after the pages have been marked as writeback and so we're good to go
	 * from there.  We have to do this otherwise we'll miss the ordered
	 * extents and that results in badness.  Please Josef, do not think you
	 * know better and pull this out at some point in the future, it is
	 * right and you are wrong.
	 */
	if (test_bit(BTRFS_INODE_HAS_ASYNC_EXTENT,
		     &BTRFS_I(inode)->runtime_flags)) {
		ret = filemap_fdatawrite_range(inode->i_mapping, start,
					       orig_end);
		if (ret)
			return ret;
	}
	ret = filemap_fdatawait_range(inode->i_mapping, start, orig_end);
	if (ret)
		return ret;

	end = orig_end;
>>>>>>> refs/remotes/origin/master
	while (1) {
		ordered = btrfs_lookup_first_ordered_extent(inode, end);
		if (!ordered)
			break;
		if (ordered->file_offset > orig_end) {
			btrfs_put_ordered_extent(ordered);
			break;
		}
<<<<<<< HEAD
		if (ordered->file_offset + ordered->len < start) {
			btrfs_put_ordered_extent(ordered);
			break;
		}
		found++;
		btrfs_start_ordered_extent(inode, ordered, 1);
		end = ordered->file_offset;
		btrfs_put_ordered_extent(ordered);
		if (end == 0 || end == start)
			break;
		end--;
	}
	if (found || test_range_bit(&BTRFS_I(inode)->io_tree, start, orig_end,
			   EXTENT_DELALLOC, 0, NULL)) {
		schedule_timeout(1);
		goto again;
	}
<<<<<<< HEAD
	return 0;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (ordered->file_offset + ordered->len <= start) {
			btrfs_put_ordered_extent(ordered);
			break;
		}
		btrfs_start_ordered_extent(inode, ordered, 1);
		end = ordered->file_offset;
		if (test_bit(BTRFS_ORDERED_IOERR, &ordered->flags))
			ret = -EIO;
		btrfs_put_ordered_extent(ordered);
		if (ret || end == 0 || end == start)
			break;
		end--;
	}
	return ret;
>>>>>>> refs/remotes/origin/master
}

/*
 * find an ordered extent corresponding to file_offset.  return NULL if
 * nothing is found, otherwise take a reference on the extent and return it
 */
struct btrfs_ordered_extent *btrfs_lookup_ordered_extent(struct inode *inode,
							 u64 file_offset)
{
	struct btrfs_ordered_inode_tree *tree;
	struct rb_node *node;
	struct btrfs_ordered_extent *entry = NULL;

	tree = &BTRFS_I(inode)->ordered_tree;
<<<<<<< HEAD
	spin_lock(&tree->lock);
=======
	spin_lock_irq(&tree->lock);
>>>>>>> refs/remotes/origin/master
	node = tree_search(tree, file_offset);
	if (!node)
		goto out;

	entry = rb_entry(node, struct btrfs_ordered_extent, rb_node);
	if (!offset_in_entry(entry, file_offset))
		entry = NULL;
	if (entry)
		atomic_inc(&entry->refs);
out:
<<<<<<< HEAD
	spin_unlock(&tree->lock);
=======
	spin_unlock_irq(&tree->lock);
>>>>>>> refs/remotes/origin/master
	return entry;
}

/* Since the DIO code tries to lock a wide area we need to look for any ordered
 * extents that exist in the range, rather than just the start of the range.
 */
struct btrfs_ordered_extent *btrfs_lookup_ordered_range(struct inode *inode,
							u64 file_offset,
							u64 len)
{
	struct btrfs_ordered_inode_tree *tree;
	struct rb_node *node;
	struct btrfs_ordered_extent *entry = NULL;

	tree = &BTRFS_I(inode)->ordered_tree;
<<<<<<< HEAD
	spin_lock(&tree->lock);
=======
	spin_lock_irq(&tree->lock);
>>>>>>> refs/remotes/origin/master
	node = tree_search(tree, file_offset);
	if (!node) {
		node = tree_search(tree, file_offset + len);
		if (!node)
			goto out;
	}

	while (1) {
		entry = rb_entry(node, struct btrfs_ordered_extent, rb_node);
		if (range_overlaps(entry, file_offset, len))
			break;

		if (entry->file_offset >= file_offset + len) {
			entry = NULL;
			break;
		}
		entry = NULL;
		node = rb_next(node);
		if (!node)
			break;
	}
out:
	if (entry)
		atomic_inc(&entry->refs);
<<<<<<< HEAD
	spin_unlock(&tree->lock);
=======
	spin_unlock_irq(&tree->lock);
>>>>>>> refs/remotes/origin/master
	return entry;
}

/*
 * lookup and return any extent before 'file_offset'.  NULL is returned
 * if none is found
 */
struct btrfs_ordered_extent *
btrfs_lookup_first_ordered_extent(struct inode *inode, u64 file_offset)
{
	struct btrfs_ordered_inode_tree *tree;
	struct rb_node *node;
	struct btrfs_ordered_extent *entry = NULL;

	tree = &BTRFS_I(inode)->ordered_tree;
<<<<<<< HEAD
	spin_lock(&tree->lock);
=======
	spin_lock_irq(&tree->lock);
>>>>>>> refs/remotes/origin/master
	node = tree_search(tree, file_offset);
	if (!node)
		goto out;

	entry = rb_entry(node, struct btrfs_ordered_extent, rb_node);
	atomic_inc(&entry->refs);
out:
<<<<<<< HEAD
	spin_unlock(&tree->lock);
=======
	spin_unlock_irq(&tree->lock);
>>>>>>> refs/remotes/origin/master
	return entry;
}

/*
 * After an extent is done, call this to conditionally update the on disk
 * i_size.  i_size is updated to cover any fully written part of the file.
 */
int btrfs_ordered_update_i_size(struct inode *inode, u64 offset,
				struct btrfs_ordered_extent *ordered)
{
	struct btrfs_ordered_inode_tree *tree = &BTRFS_I(inode)->ordered_tree;
<<<<<<< HEAD
	struct extent_io_tree *io_tree = &BTRFS_I(inode)->io_tree;
	u64 disk_i_size;
	u64 new_i_size;
	u64 i_size_test;
=======
	u64 disk_i_size;
	u64 new_i_size;
>>>>>>> refs/remotes/origin/master
	u64 i_size = i_size_read(inode);
	struct rb_node *node;
	struct rb_node *prev = NULL;
	struct btrfs_ordered_extent *test;
	int ret = 1;

<<<<<<< HEAD
	if (ordered)
		offset = entry_end(ordered);
	else
		offset = ALIGN(offset, BTRFS_I(inode)->root->sectorsize);

	spin_lock(&tree->lock);
=======
	spin_lock_irq(&tree->lock);
	if (ordered) {
		offset = entry_end(ordered);
		if (test_bit(BTRFS_ORDERED_TRUNCATED, &ordered->flags))
			offset = min(offset,
				     ordered->file_offset +
				     ordered->truncated_len);
	} else {
		offset = ALIGN(offset, BTRFS_I(inode)->root->sectorsize);
	}
>>>>>>> refs/remotes/origin/master
	disk_i_size = BTRFS_I(inode)->disk_i_size;

	/* truncate file */
	if (disk_i_size > i_size) {
		BTRFS_I(inode)->disk_i_size = i_size;
		ret = 0;
		goto out;
	}

	/*
	 * if the disk i_size is already at the inode->i_size, or
	 * this ordered extent is inside the disk i_size, we're done
	 */
<<<<<<< HEAD
	if (disk_i_size == i_size || offset <= disk_i_size) {
		goto out;
	}

	/*
	 * we can't update the disk_isize if there are delalloc bytes
	 * between disk_i_size and  this ordered extent
	 */
	if (test_range_bit(io_tree, disk_i_size, offset - 1,
			   EXTENT_DELALLOC, 0, NULL)) {
		goto out;
	}
=======
	if (disk_i_size == i_size)
		goto out;

	/*
	 * We still need to update disk_i_size if outstanding_isize is greater
	 * than disk_i_size.
	 */
	if (offset <= disk_i_size &&
	    (!ordered || ordered->outstanding_isize <= disk_i_size))
		goto out;

>>>>>>> refs/remotes/origin/master
	/*
	 * walk backward from this ordered extent to disk_i_size.
	 * if we find an ordered extent then we can't update disk i_size
	 * yet
	 */
	if (ordered) {
		node = rb_prev(&ordered->rb_node);
	} else {
		prev = tree_search(tree, offset);
		/*
		 * we insert file extents without involving ordered struct,
		 * so there should be no ordered struct cover this offset
		 */
		if (prev) {
			test = rb_entry(prev, struct btrfs_ordered_extent,
					rb_node);
			BUG_ON(offset_in_entry(test, offset));
		}
		node = prev;
	}
<<<<<<< HEAD
	while (node) {
		test = rb_entry(node, struct btrfs_ordered_extent, rb_node);
=======
	for (; node; node = rb_prev(node)) {
		test = rb_entry(node, struct btrfs_ordered_extent, rb_node);

		/* We treat this entry as if it doesnt exist */
		if (test_bit(BTRFS_ORDERED_UPDATED_ISIZE, &test->flags))
			continue;
>>>>>>> refs/remotes/origin/master
		if (test->file_offset + test->len <= disk_i_size)
			break;
		if (test->file_offset >= i_size)
			break;
<<<<<<< HEAD
		if (test->file_offset >= disk_i_size)
			goto out;
		node = rb_prev(node);
=======
		if (entry_end(test) > disk_i_size) {
			/*
			 * we don't update disk_i_size now, so record this
			 * undealt i_size. Or we will not know the real
			 * i_size.
			 */
			if (test->outstanding_isize < offset)
				test->outstanding_isize = offset;
			if (ordered &&
			    ordered->outstanding_isize >
			    test->outstanding_isize)
				test->outstanding_isize =
						ordered->outstanding_isize;
			goto out;
		}
>>>>>>> refs/remotes/origin/master
	}
	new_i_size = min_t(u64, offset, i_size);

	/*
<<<<<<< HEAD
	 * at this point, we know we can safely update i_size to at least
	 * the offset from this ordered extent.  But, we need to
	 * walk forward and see if ios from higher up in the file have
	 * finished.
	 */
	if (ordered) {
		node = rb_next(&ordered->rb_node);
	} else {
		if (prev)
			node = rb_next(prev);
		else
			node = rb_first(&tree->tree);
	}
	i_size_test = 0;
	if (node) {
		/*
		 * do we have an area where IO might have finished
		 * between our ordered extent and the next one.
		 */
		test = rb_entry(node, struct btrfs_ordered_extent, rb_node);
		if (test->file_offset > offset)
			i_size_test = test->file_offset;
	} else {
		i_size_test = i_size;
	}

	/*
	 * i_size_test is the end of a region after this ordered
	 * extent where there are no ordered extents.  As long as there
	 * are no delalloc bytes in this area, it is safe to update
	 * disk_i_size to the end of the region.
	 */
	if (i_size_test > offset &&
	    !test_range_bit(io_tree, offset, i_size_test - 1,
			    EXTENT_DELALLOC, 0, NULL)) {
		new_i_size = min_t(u64, i_size_test, i_size);
	}
=======
	 * Some ordered extents may completed before the current one, and
	 * we hold the real i_size in ->outstanding_isize.
	 */
	if (ordered && ordered->outstanding_isize > new_i_size)
		new_i_size = min_t(u64, ordered->outstanding_isize, i_size);
>>>>>>> refs/remotes/origin/master
	BTRFS_I(inode)->disk_i_size = new_i_size;
	ret = 0;
out:
	/*
<<<<<<< HEAD
	 * we need to remove the ordered extent with the tree lock held
	 * so that other people calling this function don't find our fully
	 * processed ordered entry and skip updating the i_size
	 */
	if (ordered)
		__btrfs_remove_ordered_extent(inode, ordered);
	spin_unlock(&tree->lock);
	if (ordered)
		wake_up(&ordered->wait);
=======
	 * We need to do this because we can't remove ordered extents until
	 * after the i_disk_size has been updated and then the inode has been
	 * updated to reflect the change, so we need to tell anybody who finds
	 * this ordered extent that we've already done all the real work, we
	 * just haven't completed all the other work.
	 */
	if (ordered)
		set_bit(BTRFS_ORDERED_UPDATED_ISIZE, &ordered->flags);
	spin_unlock_irq(&tree->lock);
>>>>>>> refs/remotes/origin/master
	return ret;
}

/*
 * search the ordered extents for one corresponding to 'offset' and
 * try to find a checksum.  This is used because we allow pages to
 * be reclaimed before their checksum is actually put into the btree
 */
int btrfs_find_ordered_sum(struct inode *inode, u64 offset, u64 disk_bytenr,
<<<<<<< HEAD
			   u32 *sum)
{
	struct btrfs_ordered_sum *ordered_sum;
	struct btrfs_sector_sum *sector_sums;
=======
			   u32 *sum, int len)
{
	struct btrfs_ordered_sum *ordered_sum;
>>>>>>> refs/remotes/origin/master
	struct btrfs_ordered_extent *ordered;
	struct btrfs_ordered_inode_tree *tree = &BTRFS_I(inode)->ordered_tree;
	unsigned long num_sectors;
	unsigned long i;
	u32 sectorsize = BTRFS_I(inode)->root->sectorsize;
<<<<<<< HEAD
	int ret = 1;

	ordered = btrfs_lookup_ordered_extent(inode, offset);
	if (!ordered)
		return 1;

	spin_lock(&tree->lock);
	list_for_each_entry_reverse(ordered_sum, &ordered->list, list) {
		if (disk_bytenr >= ordered_sum->bytenr) {
			num_sectors = ordered_sum->len / sectorsize;
			sector_sums = ordered_sum->sums;
			for (i = 0; i < num_sectors; i++) {
				if (sector_sums[i].bytenr == disk_bytenr) {
					*sum = sector_sums[i].sum;
					ret = 0;
					goto out;
				}
			}
		}
	}
out:
	spin_unlock(&tree->lock);
	btrfs_put_ordered_extent(ordered);
	return ret;
=======
	int index = 0;

	ordered = btrfs_lookup_ordered_extent(inode, offset);
	if (!ordered)
		return 0;

	spin_lock_irq(&tree->lock);
	list_for_each_entry_reverse(ordered_sum, &ordered->list, list) {
		if (disk_bytenr >= ordered_sum->bytenr &&
		    disk_bytenr < ordered_sum->bytenr + ordered_sum->len) {
			i = (disk_bytenr - ordered_sum->bytenr) >>
			    inode->i_sb->s_blocksize_bits;
			num_sectors = ordered_sum->len >>
				      inode->i_sb->s_blocksize_bits;
			num_sectors = min_t(int, len - index, num_sectors - i);
			memcpy(sum + index, ordered_sum->sums + i,
			       num_sectors);

			index += (int)num_sectors;
			if (index == len)
				goto out;
			disk_bytenr += num_sectors * sectorsize;
		}
	}
out:
	spin_unlock_irq(&tree->lock);
	btrfs_put_ordered_extent(ordered);
	return index;
>>>>>>> refs/remotes/origin/master
}


/*
 * add a given inode to the list of inodes that must be fully on
 * disk before a transaction commit finishes.
 *
 * This basically gives us the ext3 style data=ordered mode, and it is mostly
 * used to make sure renamed files are fully on disk.
 *
 * It is a noop if the inode is already fully on disk.
 *
 * If trans is not null, we'll do a friendly check for a transaction that
 * is already flushing things and force the IO down ourselves.
 */
<<<<<<< HEAD
<<<<<<< HEAD
int btrfs_add_ordered_operation(struct btrfs_trans_handle *trans,
				struct btrfs_root *root,
				struct inode *inode)
=======
void btrfs_add_ordered_operation(struct btrfs_trans_handle *trans,
				 struct btrfs_root *root, struct inode *inode)
>>>>>>> refs/remotes/origin/cm-10.0
{
=======
void btrfs_add_ordered_operation(struct btrfs_trans_handle *trans,
				 struct btrfs_root *root, struct inode *inode)
{
	struct btrfs_transaction *cur_trans = trans->transaction;
>>>>>>> refs/remotes/origin/master
	u64 last_mod;

	last_mod = max(BTRFS_I(inode)->generation, BTRFS_I(inode)->last_trans);

	/*
	 * if this file hasn't been changed since the last transaction
	 * commit, we can safely return without doing anything
	 */
<<<<<<< HEAD
	if (last_mod < root->fs_info->last_trans_committed)
<<<<<<< HEAD
		return 0;
=======
		return;
>>>>>>> refs/remotes/origin/cm-10.0

	/*
	 * the transaction is already committing.  Just start the IO and
	 * don't bother with all of this list nonsense
	 */
	if (trans && root->fs_info->running_transaction->blocked) {
		btrfs_wait_ordered_range(inode, 0, (u64)-1);
<<<<<<< HEAD
		return 0;
=======
		return;
>>>>>>> refs/remotes/origin/cm-10.0
	}

	spin_lock(&root->fs_info->ordered_extent_lock);
	if (list_empty(&BTRFS_I(inode)->ordered_operations)) {
		list_add_tail(&BTRFS_I(inode)->ordered_operations,
			      &root->fs_info->ordered_operations);
	}
	spin_unlock(&root->fs_info->ordered_extent_lock);
<<<<<<< HEAD

	return 0;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (last_mod <= root->fs_info->last_trans_committed)
		return;

	spin_lock(&root->fs_info->ordered_root_lock);
	if (list_empty(&BTRFS_I(inode)->ordered_operations)) {
		list_add_tail(&BTRFS_I(inode)->ordered_operations,
			      &cur_trans->ordered_operations);
	}
	spin_unlock(&root->fs_info->ordered_root_lock);
}

int __init ordered_data_init(void)
{
	btrfs_ordered_extent_cache = kmem_cache_create("btrfs_ordered_extent",
				     sizeof(struct btrfs_ordered_extent), 0,
				     SLAB_RECLAIM_ACCOUNT | SLAB_MEM_SPREAD,
				     NULL);
	if (!btrfs_ordered_extent_cache)
		return -ENOMEM;

	return 0;
}

void ordered_data_exit(void)
{
	if (btrfs_ordered_extent_cache)
		kmem_cache_destroy(btrfs_ordered_extent_cache);
>>>>>>> refs/remotes/origin/master
}
