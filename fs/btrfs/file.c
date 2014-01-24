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

#include <linux/fs.h>
#include <linux/pagemap.h>
#include <linux/highmem.h>
#include <linux/time.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/backing-dev.h>
#include <linux/mpage.h>
<<<<<<< HEAD
=======
#include <linux/aio.h>
>>>>>>> refs/remotes/origin/master
#include <linux/falloc.h>
#include <linux/swap.h>
#include <linux/writeback.h>
#include <linux/statfs.h>
#include <linux/compat.h>
#include <linux/slab.h>
<<<<<<< HEAD
=======
#include <linux/btrfs.h>
>>>>>>> refs/remotes/origin/master
#include "ctree.h"
#include "disk-io.h"
#include "transaction.h"
#include "btrfs_inode.h"
<<<<<<< HEAD
#include "ioctl.h"
#include "print-tree.h"
#include "tree-log.h"
#include "locking.h"
#include "compat.h"

=======
#include "print-tree.h"
#include "tree-log.h"
#include "locking.h"
#include "volumes.h"

static struct kmem_cache *btrfs_inode_defrag_cachep;
>>>>>>> refs/remotes/origin/master
/*
 * when auto defrag is enabled we
 * queue up these defrag structs to remember which
 * inodes need defragging passes
 */
struct inode_defrag {
	struct rb_node rb_node;
	/* objectid */
	u64 ino;
	/*
	 * transid where the defrag was added, we search for
	 * extents newer than this
	 */
	u64 transid;

	/* root objectid */
	u64 root;

	/* last offset we were able to defrag */
	u64 last_offset;

	/* if we've wrapped around back to zero once already */
	int cycled;
};

<<<<<<< HEAD
=======
static int __compare_inode_defrag(struct inode_defrag *defrag1,
				  struct inode_defrag *defrag2)
{
	if (defrag1->root > defrag2->root)
		return 1;
	else if (defrag1->root < defrag2->root)
		return -1;
	else if (defrag1->ino > defrag2->ino)
		return 1;
	else if (defrag1->ino < defrag2->ino)
		return -1;
	else
		return 0;
}

>>>>>>> refs/remotes/origin/master
/* pop a record for an inode into the defrag tree.  The lock
 * must be held already
 *
 * If you're inserting a record for an older transid than an
 * existing record, the transid already in the tree is lowered
 *
 * If an existing record is found the defrag item you
 * pass in is freed
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int __btrfs_add_inode_defrag(struct inode *inode,
=======
static void __btrfs_add_inode_defrag(struct inode *inode,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int __btrfs_add_inode_defrag(struct inode *inode,
>>>>>>> refs/remotes/origin/master
				    struct inode_defrag *defrag)
{
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct inode_defrag *entry;
	struct rb_node **p;
	struct rb_node *parent = NULL;
<<<<<<< HEAD
=======
	int ret;
>>>>>>> refs/remotes/origin/master

	p = &root->fs_info->defrag_inodes.rb_node;
	while (*p) {
		parent = *p;
		entry = rb_entry(parent, struct inode_defrag, rb_node);

<<<<<<< HEAD
		if (defrag->ino < entry->ino)
			p = &parent->rb_left;
		else if (defrag->ino > entry->ino)
=======
		ret = __compare_inode_defrag(defrag, entry);
		if (ret < 0)
			p = &parent->rb_left;
		else if (ret > 0)
>>>>>>> refs/remotes/origin/master
			p = &parent->rb_right;
		else {
			/* if we're reinserting an entry for
			 * an old defrag run, make sure to
			 * lower the transid of our existing record
			 */
			if (defrag->transid < entry->transid)
				entry->transid = defrag->transid;
			if (defrag->last_offset > entry->last_offset)
				entry->last_offset = defrag->last_offset;
<<<<<<< HEAD
			goto exists;
		}
	}
	BTRFS_I(inode)->in_defrag = 1;
	rb_link_node(&defrag->rb_node, parent, p);
	rb_insert_color(&defrag->rb_node, &root->fs_info->defrag_inodes);
<<<<<<< HEAD
	return 0;

exists:
	kfree(defrag);
	return 0;
=======
	return;

exists:
	kfree(defrag);
	return;
>>>>>>> refs/remotes/origin/cm-10.0

=======
			return -EEXIST;
		}
	}
	set_bit(BTRFS_INODE_IN_DEFRAG, &BTRFS_I(inode)->runtime_flags);
	rb_link_node(&defrag->rb_node, parent, p);
	rb_insert_color(&defrag->rb_node, &root->fs_info->defrag_inodes);
	return 0;
}

static inline int __need_auto_defrag(struct btrfs_root *root)
{
	if (!btrfs_test_opt(root, AUTO_DEFRAG))
		return 0;

	if (btrfs_fs_closing(root->fs_info))
		return 0;

	return 1;
>>>>>>> refs/remotes/origin/master
}

/*
 * insert a defrag record for this inode if auto defrag is
 * enabled
 */
int btrfs_add_inode_defrag(struct btrfs_trans_handle *trans,
			   struct inode *inode)
{
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct inode_defrag *defrag;
<<<<<<< HEAD
<<<<<<< HEAD
	int ret = 0;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	u64 transid;

	if (!btrfs_test_opt(root, AUTO_DEFRAG))
		return 0;

	if (btrfs_fs_closing(root->fs_info))
		return 0;

	if (BTRFS_I(inode)->in_defrag)
=======
	u64 transid;
	int ret;

	if (!__need_auto_defrag(root))
		return 0;

	if (test_bit(BTRFS_INODE_IN_DEFRAG, &BTRFS_I(inode)->runtime_flags))
>>>>>>> refs/remotes/origin/master
		return 0;

	if (trans)
		transid = trans->transid;
	else
		transid = BTRFS_I(inode)->root->last_trans;

<<<<<<< HEAD
	defrag = kzalloc(sizeof(*defrag), GFP_NOFS);
=======
	defrag = kmem_cache_zalloc(btrfs_inode_defrag_cachep, GFP_NOFS);
>>>>>>> refs/remotes/origin/master
	if (!defrag)
		return -ENOMEM;

	defrag->ino = btrfs_ino(inode);
	defrag->transid = transid;
	defrag->root = root->root_key.objectid;

	spin_lock(&root->fs_info->defrag_inodes_lock);
<<<<<<< HEAD
	if (!BTRFS_I(inode)->in_defrag)
<<<<<<< HEAD
		ret = __btrfs_add_inode_defrag(inode, defrag);
	spin_unlock(&root->fs_info->defrag_inodes_lock);
	return ret;
=======
		__btrfs_add_inode_defrag(inode, defrag);
	else
		kfree(defrag);
	spin_unlock(&root->fs_info->defrag_inodes_lock);
	return 0;
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * must be called with the defrag_inodes lock held
 */
struct inode_defrag *btrfs_find_defrag_inode(struct btrfs_fs_info *info, u64 ino,
					     struct rb_node **next)
{
	struct inode_defrag *entry = NULL;
	struct rb_node *p;
	struct rb_node *parent = NULL;

	p = info->defrag_inodes.rb_node;
=======
	if (!test_bit(BTRFS_INODE_IN_DEFRAG, &BTRFS_I(inode)->runtime_flags)) {
		/*
		 * If we set IN_DEFRAG flag and evict the inode from memory,
		 * and then re-read this inode, this new inode doesn't have
		 * IN_DEFRAG flag. At the case, we may find the existed defrag.
		 */
		ret = __btrfs_add_inode_defrag(inode, defrag);
		if (ret)
			kmem_cache_free(btrfs_inode_defrag_cachep, defrag);
	} else {
		kmem_cache_free(btrfs_inode_defrag_cachep, defrag);
	}
	spin_unlock(&root->fs_info->defrag_inodes_lock);
	return 0;
}

/*
 * Requeue the defrag object. If there is a defrag object that points to
 * the same inode in the tree, we will merge them together (by
 * __btrfs_add_inode_defrag()) and free the one that we want to requeue.
 */
static void btrfs_requeue_inode_defrag(struct inode *inode,
				       struct inode_defrag *defrag)
{
	struct btrfs_root *root = BTRFS_I(inode)->root;
	int ret;

	if (!__need_auto_defrag(root))
		goto out;

	/*
	 * Here we don't check the IN_DEFRAG flag, because we need merge
	 * them together.
	 */
	spin_lock(&root->fs_info->defrag_inodes_lock);
	ret = __btrfs_add_inode_defrag(inode, defrag);
	spin_unlock(&root->fs_info->defrag_inodes_lock);
	if (ret)
		goto out;
	return;
out:
	kmem_cache_free(btrfs_inode_defrag_cachep, defrag);
}

/*
 * pick the defragable inode that we want, if it doesn't exist, we will get
 * the next one.
 */
static struct inode_defrag *
btrfs_pick_defrag_inode(struct btrfs_fs_info *fs_info, u64 root, u64 ino)
{
	struct inode_defrag *entry = NULL;
	struct inode_defrag tmp;
	struct rb_node *p;
	struct rb_node *parent = NULL;
	int ret;

	tmp.ino = ino;
	tmp.root = root;

	spin_lock(&fs_info->defrag_inodes_lock);
	p = fs_info->defrag_inodes.rb_node;
>>>>>>> refs/remotes/origin/master
	while (p) {
		parent = p;
		entry = rb_entry(parent, struct inode_defrag, rb_node);

<<<<<<< HEAD
		if (ino < entry->ino)
			p = parent->rb_left;
		else if (ino > entry->ino)
			p = parent->rb_right;
		else
			return entry;
	}

	if (next) {
		while (parent && ino > entry->ino) {
			parent = rb_next(parent);
			entry = rb_entry(parent, struct inode_defrag, rb_node);
		}
		*next = parent;
	}
	return NULL;
}

/*
 * run through the list of inodes in the FS that need
 * defragging
 */
int btrfs_run_defrag_inodes(struct btrfs_fs_info *fs_info)
{
	struct inode_defrag *defrag;
	struct btrfs_root *inode_root;
	struct inode *inode;
	struct rb_node *n;
	struct btrfs_key key;
	struct btrfs_ioctl_defrag_range_args range;
	u64 first_ino = 0;
	int num_defrag;
	int defrag_batch = 1024;

	memset(&range, 0, sizeof(range));
	range.len = (u64)-1;

	atomic_inc(&fs_info->defrag_running);
	spin_lock(&fs_info->defrag_inodes_lock);
	while(1) {
		n = NULL;

		/* find an inode to defrag */
		defrag = btrfs_find_defrag_inode(fs_info, first_ino, &n);
		if (!defrag) {
			if (n)
				defrag = rb_entry(n, struct inode_defrag, rb_node);
			else if (first_ino) {
=======
		ret = __compare_inode_defrag(&tmp, entry);
		if (ret < 0)
			p = parent->rb_left;
		else if (ret > 0)
			p = parent->rb_right;
		else
			goto out;
	}

	if (parent && __compare_inode_defrag(&tmp, entry) > 0) {
		parent = rb_next(parent);
		if (parent)
			entry = rb_entry(parent, struct inode_defrag, rb_node);
		else
			entry = NULL;
	}
out:
	if (entry)
		rb_erase(parent, &fs_info->defrag_inodes);
	spin_unlock(&fs_info->defrag_inodes_lock);
	return entry;
}

void btrfs_cleanup_defrag_inodes(struct btrfs_fs_info *fs_info)
{
	struct inode_defrag *defrag;
	struct rb_node *node;

	spin_lock(&fs_info->defrag_inodes_lock);
	node = rb_first(&fs_info->defrag_inodes);
	while (node) {
		rb_erase(node, &fs_info->defrag_inodes);
		defrag = rb_entry(node, struct inode_defrag, rb_node);
		kmem_cache_free(btrfs_inode_defrag_cachep, defrag);

		if (need_resched()) {
			spin_unlock(&fs_info->defrag_inodes_lock);
			cond_resched();
			spin_lock(&fs_info->defrag_inodes_lock);
		}

		node = rb_first(&fs_info->defrag_inodes);
	}
	spin_unlock(&fs_info->defrag_inodes_lock);
}

#define BTRFS_DEFRAG_BATCH	1024

static int __btrfs_run_defrag_inode(struct btrfs_fs_info *fs_info,
				    struct inode_defrag *defrag)
{
	struct btrfs_root *inode_root;
	struct inode *inode;
	struct btrfs_key key;
	struct btrfs_ioctl_defrag_range_args range;
	int num_defrag;
	int index;
	int ret;

	/* get the inode */
	key.objectid = defrag->root;
	btrfs_set_key_type(&key, BTRFS_ROOT_ITEM_KEY);
	key.offset = (u64)-1;

	index = srcu_read_lock(&fs_info->subvol_srcu);

	inode_root = btrfs_read_fs_root_no_name(fs_info, &key);
	if (IS_ERR(inode_root)) {
		ret = PTR_ERR(inode_root);
		goto cleanup;
	}

	key.objectid = defrag->ino;
	btrfs_set_key_type(&key, BTRFS_INODE_ITEM_KEY);
	key.offset = 0;
	inode = btrfs_iget(fs_info->sb, &key, inode_root, NULL);
	if (IS_ERR(inode)) {
		ret = PTR_ERR(inode);
		goto cleanup;
	}
	srcu_read_unlock(&fs_info->subvol_srcu, index);

	/* do a chunk of defrag */
	clear_bit(BTRFS_INODE_IN_DEFRAG, &BTRFS_I(inode)->runtime_flags);
	memset(&range, 0, sizeof(range));
	range.len = (u64)-1;
	range.start = defrag->last_offset;

	sb_start_write(fs_info->sb);
	num_defrag = btrfs_defrag_file(inode, NULL, &range, defrag->transid,
				       BTRFS_DEFRAG_BATCH);
	sb_end_write(fs_info->sb);
	/*
	 * if we filled the whole defrag batch, there
	 * must be more work to do.  Queue this defrag
	 * again
	 */
	if (num_defrag == BTRFS_DEFRAG_BATCH) {
		defrag->last_offset = range.start;
		btrfs_requeue_inode_defrag(inode, defrag);
	} else if (defrag->last_offset && !defrag->cycled) {
		/*
		 * we didn't fill our defrag batch, but
		 * we didn't start at zero.  Make sure we loop
		 * around to the start of the file.
		 */
		defrag->last_offset = 0;
		defrag->cycled = 1;
		btrfs_requeue_inode_defrag(inode, defrag);
	} else {
		kmem_cache_free(btrfs_inode_defrag_cachep, defrag);
	}

	iput(inode);
	return 0;
cleanup:
	srcu_read_unlock(&fs_info->subvol_srcu, index);
	kmem_cache_free(btrfs_inode_defrag_cachep, defrag);
	return ret;
}

/*
 * run through the list of inodes in the FS that need
 * defragging
 */
int btrfs_run_defrag_inodes(struct btrfs_fs_info *fs_info)
{
	struct inode_defrag *defrag;
	u64 first_ino = 0;
	u64 root_objectid = 0;

	atomic_inc(&fs_info->defrag_running);
	while (1) {
		/* Pause the auto defragger. */
		if (test_bit(BTRFS_FS_STATE_REMOUNTING,
			     &fs_info->fs_state))
			break;

		if (!__need_auto_defrag(fs_info->tree_root))
			break;

		/* find an inode to defrag */
		defrag = btrfs_pick_defrag_inode(fs_info, root_objectid,
						 first_ino);
		if (!defrag) {
			if (root_objectid || first_ino) {
				root_objectid = 0;
>>>>>>> refs/remotes/origin/master
				first_ino = 0;
				continue;
			} else {
				break;
			}
		}

<<<<<<< HEAD
		/* remove it from the rbtree */
		first_ino = defrag->ino + 1;
		rb_erase(&defrag->rb_node, &fs_info->defrag_inodes);

		if (btrfs_fs_closing(fs_info))
			goto next_free;

		spin_unlock(&fs_info->defrag_inodes_lock);

		/* get the inode */
		key.objectid = defrag->root;
		btrfs_set_key_type(&key, BTRFS_ROOT_ITEM_KEY);
		key.offset = (u64)-1;
		inode_root = btrfs_read_fs_root_no_name(fs_info, &key);
		if (IS_ERR(inode_root))
			goto next;

		key.objectid = defrag->ino;
		btrfs_set_key_type(&key, BTRFS_INODE_ITEM_KEY);
		key.offset = 0;

		inode = btrfs_iget(fs_info->sb, &key, inode_root, NULL);
		if (IS_ERR(inode))
			goto next;

		/* do a chunk of defrag */
		BTRFS_I(inode)->in_defrag = 0;
		range.start = defrag->last_offset;
		num_defrag = btrfs_defrag_file(inode, NULL, &range, defrag->transid,
					       defrag_batch);
		/*
		 * if we filled the whole defrag batch, there
		 * must be more work to do.  Queue this defrag
		 * again
		 */
		if (num_defrag == defrag_batch) {
			defrag->last_offset = range.start;
			__btrfs_add_inode_defrag(inode, defrag);
			/*
			 * we don't want to kfree defrag, we added it back to
			 * the rbtree
			 */
			defrag = NULL;
		} else if (defrag->last_offset && !defrag->cycled) {
			/*
			 * we didn't fill our defrag batch, but
			 * we didn't start at zero.  Make sure we loop
			 * around to the start of the file.
			 */
			defrag->last_offset = 0;
			defrag->cycled = 1;
			__btrfs_add_inode_defrag(inode, defrag);
			defrag = NULL;
		}

		iput(inode);
next:
		spin_lock(&fs_info->defrag_inodes_lock);
next_free:
		kfree(defrag);
	}
	spin_unlock(&fs_info->defrag_inodes_lock);

=======
		first_ino = defrag->ino + 1;
		root_objectid = defrag->root;

		__btrfs_run_defrag_inode(fs_info, defrag);
	}
>>>>>>> refs/remotes/origin/master
	atomic_dec(&fs_info->defrag_running);

	/*
	 * during unmount, we use the transaction_wait queue to
	 * wait for the defragger to stop
	 */
	wake_up(&fs_info->transaction_wait);
	return 0;
}

/* simple helper to fault in pages and copy.  This should go away
 * and be replaced with calls into generic code.
 */
static noinline int btrfs_copy_from_user(loff_t pos, int num_pages,
					 size_t write_bytes,
					 struct page **prepared_pages,
					 struct iov_iter *i)
{
	size_t copied = 0;
	size_t total_copied = 0;
	int pg = 0;
	int offset = pos & (PAGE_CACHE_SIZE - 1);

	while (write_bytes > 0) {
		size_t count = min_t(size_t,
				     PAGE_CACHE_SIZE - offset, write_bytes);
		struct page *page = prepared_pages[pg];
		/*
		 * Copy data from userspace to the current page
		 *
		 * Disable pagefault to avoid recursive lock since
		 * the pages are already locked
		 */
		pagefault_disable();
		copied = iov_iter_copy_from_user_atomic(page, i, offset, count);
		pagefault_enable();

		/* Flush processor's dcache for this page */
		flush_dcache_page(page);

		/*
		 * if we get a partial write, we can end up with
		 * partially up to date pages.  These add
		 * a lot of complexity, so make sure they don't
		 * happen by forcing this copy to be retried.
		 *
		 * The rest of the btrfs_file_write code will fall
		 * back to page at a time copies after we return 0.
		 */
		if (!PageUptodate(page) && copied < count)
			copied = 0;

		iov_iter_advance(i, copied);
		write_bytes -= copied;
		total_copied += copied;

		/* Return to btrfs_file_aio_write to fault page */
		if (unlikely(copied == 0))
			break;

		if (unlikely(copied < PAGE_CACHE_SIZE - offset)) {
			offset += copied;
		} else {
			pg++;
			offset = 0;
		}
	}
	return total_copied;
}

/*
 * unlocks pages after btrfs_file_write is done with them
 */
<<<<<<< HEAD
void btrfs_drop_pages(struct page **pages, size_t num_pages)
=======
static void btrfs_drop_pages(struct page **pages, size_t num_pages)
>>>>>>> refs/remotes/origin/master
{
	size_t i;
	for (i = 0; i < num_pages; i++) {
		/* page checked is some magic around finding pages that
		 * have been modified without going through btrfs_set_page_dirty
		 * clear it here
		 */
		ClearPageChecked(pages[i]);
		unlock_page(pages[i]);
		mark_page_accessed(pages[i]);
		page_cache_release(pages[i]);
	}
}

/*
 * after copy_from_user, pages need to be dirtied and we need to make
 * sure holes are created between the current EOF and the start of
 * any next extents (if required).
 *
 * this also makes the decision about creating an inline extent vs
 * doing real data extents, marking pages dirty and delalloc as required.
 */
int btrfs_dirty_pages(struct btrfs_root *root, struct inode *inode,
<<<<<<< HEAD
		      struct page **pages, size_t num_pages,
		      loff_t pos, size_t write_bytes,
		      struct extent_state **cached)
=======
			     struct page **pages, size_t num_pages,
			     loff_t pos, size_t write_bytes,
			     struct extent_state **cached)
>>>>>>> refs/remotes/origin/master
{
	int err = 0;
	int i;
	u64 num_bytes;
	u64 start_pos;
	u64 end_of_last_block;
	u64 end_pos = pos + write_bytes;
	loff_t isize = i_size_read(inode);

	start_pos = pos & ~((u64)root->sectorsize - 1);
<<<<<<< HEAD
	num_bytes = (write_bytes + pos - start_pos +
		    root->sectorsize - 1) & ~((u64)root->sectorsize - 1);
=======
	num_bytes = ALIGN(write_bytes + pos - start_pos, root->sectorsize);
>>>>>>> refs/remotes/origin/master

	end_of_last_block = start_pos + num_bytes - 1;
	err = btrfs_set_extent_delalloc(inode, start_pos, end_of_last_block,
					cached);
	if (err)
		return err;

	for (i = 0; i < num_pages; i++) {
		struct page *p = pages[i];
		SetPageUptodate(p);
		ClearPageChecked(p);
		set_page_dirty(p);
	}

	/*
	 * we've only changed i_size in ram, and we haven't updated
	 * the disk i_size.  There is no need to log the inode
	 * at this time.
	 */
	if (end_pos > isize)
		i_size_write(inode, end_pos);
	return 0;
}

/*
 * this drops all the extents in the cache that intersect the range
 * [start, end].  Existing extents are split as required.
 */
<<<<<<< HEAD
int btrfs_drop_extent_cache(struct inode *inode, u64 start, u64 end,
			    int skip_pinned)
=======
void btrfs_drop_extent_cache(struct inode *inode, u64 start, u64 end,
			     int skip_pinned)
>>>>>>> refs/remotes/origin/master
{
	struct extent_map *em;
	struct extent_map *split = NULL;
	struct extent_map *split2 = NULL;
	struct extent_map_tree *em_tree = &BTRFS_I(inode)->extent_tree;
	u64 len = end - start + 1;
<<<<<<< HEAD
=======
	u64 gen;
>>>>>>> refs/remotes/origin/master
	int ret;
	int testend = 1;
	unsigned long flags;
	int compressed = 0;
<<<<<<< HEAD
=======
	bool modified;
>>>>>>> refs/remotes/origin/master

	WARN_ON(end < start);
	if (end == (u64)-1) {
		len = (u64)-1;
		testend = 0;
	}
	while (1) {
<<<<<<< HEAD
=======
		int no_splits = 0;

		modified = false;
>>>>>>> refs/remotes/origin/master
		if (!split)
			split = alloc_extent_map();
		if (!split2)
			split2 = alloc_extent_map();
<<<<<<< HEAD
<<<<<<< HEAD
		BUG_ON(!split || !split2);
=======
		BUG_ON(!split || !split2); /* -ENOMEM */
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (!split || !split2)
			no_splits = 1;
>>>>>>> refs/remotes/origin/master

		write_lock(&em_tree->lock);
		em = lookup_extent_mapping(em_tree, start, len);
		if (!em) {
			write_unlock(&em_tree->lock);
			break;
		}
		flags = em->flags;
<<<<<<< HEAD
=======
		gen = em->generation;
>>>>>>> refs/remotes/origin/master
		if (skip_pinned && test_bit(EXTENT_FLAG_PINNED, &em->flags)) {
			if (testend && em->start + em->len >= start + len) {
				free_extent_map(em);
				write_unlock(&em_tree->lock);
				break;
			}
			start = em->start + em->len;
			if (testend)
				len = start + len - (em->start + em->len);
			free_extent_map(em);
			write_unlock(&em_tree->lock);
			continue;
		}
		compressed = test_bit(EXTENT_FLAG_COMPRESSED, &em->flags);
		clear_bit(EXTENT_FLAG_PINNED, &em->flags);
<<<<<<< HEAD
		remove_extent_mapping(em_tree, em);

		if (em->block_start < EXTENT_MAP_LAST_BYTE &&
		    em->start < start) {
			split->start = em->start;
			split->len = start - em->start;
			split->orig_start = em->orig_start;
			split->block_start = em->block_start;

			if (compressed)
				split->block_len = em->block_len;
			else
				split->block_len = split->len;

			split->bdev = em->bdev;
			split->flags = flags;
			split->compress_type = em->compress_type;
			ret = add_extent_mapping(em_tree, split);
<<<<<<< HEAD
			BUG_ON(ret);
=======
			BUG_ON(ret); /* Logic error */
>>>>>>> refs/remotes/origin/cm-10.0
=======
		clear_bit(EXTENT_FLAG_LOGGING, &flags);
		modified = !list_empty(&em->list);
		remove_extent_mapping(em_tree, em);
		if (no_splits)
			goto next;

		if (em->start < start) {
			split->start = em->start;
			split->len = start - em->start;

			if (em->block_start < EXTENT_MAP_LAST_BYTE) {
				split->orig_start = em->orig_start;
				split->block_start = em->block_start;

				if (compressed)
					split->block_len = em->block_len;
				else
					split->block_len = split->len;
				split->orig_block_len = max(split->block_len,
						em->orig_block_len);
				split->ram_bytes = em->ram_bytes;
			} else {
				split->orig_start = split->start;
				split->block_len = 0;
				split->block_start = em->block_start;
				split->orig_block_len = 0;
				split->ram_bytes = split->len;
			}

			split->generation = gen;
			split->bdev = em->bdev;
			split->flags = flags;
			split->compress_type = em->compress_type;
			ret = add_extent_mapping(em_tree, split, modified);
			BUG_ON(ret); /* Logic error */
>>>>>>> refs/remotes/origin/master
			free_extent_map(split);
			split = split2;
			split2 = NULL;
		}
<<<<<<< HEAD
		if (em->block_start < EXTENT_MAP_LAST_BYTE &&
		    testend && em->start + em->len > start + len) {
=======
		if (testend && em->start + em->len > start + len) {
>>>>>>> refs/remotes/origin/master
			u64 diff = start + len - em->start;

			split->start = start + len;
			split->len = em->start + em->len - (start + len);
			split->bdev = em->bdev;
			split->flags = flags;
			split->compress_type = em->compress_type;
<<<<<<< HEAD

			if (compressed) {
				split->block_len = em->block_len;
				split->block_start = em->block_start;
				split->orig_start = em->orig_start;
			} else {
				split->block_len = split->len;
				split->block_start = em->block_start + diff;
				split->orig_start = split->start;
			}

			ret = add_extent_mapping(em_tree, split);
<<<<<<< HEAD
			BUG_ON(ret);
=======
			BUG_ON(ret); /* Logic error */
>>>>>>> refs/remotes/origin/cm-10.0
			free_extent_map(split);
			split = NULL;
		}
=======
			split->generation = gen;

			if (em->block_start < EXTENT_MAP_LAST_BYTE) {
				split->orig_block_len = max(em->block_len,
						    em->orig_block_len);

				split->ram_bytes = em->ram_bytes;
				if (compressed) {
					split->block_len = em->block_len;
					split->block_start = em->block_start;
					split->orig_start = em->orig_start;
				} else {
					split->block_len = split->len;
					split->block_start = em->block_start
						+ diff;
					split->orig_start = em->orig_start;
				}
			} else {
				split->ram_bytes = split->len;
				split->orig_start = split->start;
				split->block_len = 0;
				split->block_start = em->block_start;
				split->orig_block_len = 0;
			}

			ret = add_extent_mapping(em_tree, split, modified);
			BUG_ON(ret); /* Logic error */
			free_extent_map(split);
			split = NULL;
		}
next:
>>>>>>> refs/remotes/origin/master
		write_unlock(&em_tree->lock);

		/* once for us */
		free_extent_map(em);
		/* once for the tree*/
		free_extent_map(em);
	}
	if (split)
		free_extent_map(split);
	if (split2)
		free_extent_map(split2);
<<<<<<< HEAD
	return 0;
=======
>>>>>>> refs/remotes/origin/master
}

/*
 * this is very complex, but the basic idea is to drop all extents
 * in the range start - end.  hint_block is filled in with a block number
 * that would be a good hint to the block allocator for this file.
 *
 * If an extent intersects the range but is not entirely inside the range
 * it is either truncated or split.  Anything entirely inside the range
 * is deleted from the tree.
 */
<<<<<<< HEAD
int btrfs_drop_extents(struct btrfs_trans_handle *trans, struct inode *inode,
		       u64 start, u64 end, u64 *hint_byte, int drop_cache)
{
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct extent_buffer *leaf;
	struct btrfs_file_extent_item *fi;
	struct btrfs_path *path;
=======
int __btrfs_drop_extents(struct btrfs_trans_handle *trans,
			 struct btrfs_root *root, struct inode *inode,
			 struct btrfs_path *path, u64 start, u64 end,
			 u64 *drop_end, int drop_cache)
{
	struct extent_buffer *leaf;
	struct btrfs_file_extent_item *fi;
>>>>>>> refs/remotes/origin/master
	struct btrfs_key key;
	struct btrfs_key new_key;
	u64 ino = btrfs_ino(inode);
	u64 search_start = start;
	u64 disk_bytenr = 0;
	u64 num_bytes = 0;
	u64 extent_offset = 0;
	u64 extent_end = 0;
	int del_nr = 0;
	int del_slot = 0;
	int extent_type;
	int recow;
	int ret;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	int modify_tree = -1;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int modify_tree = -1;
	int update_refs = (root->ref_cows || root == root->fs_info->tree_root);
	int found = 0;
>>>>>>> refs/remotes/origin/master

	if (drop_cache)
		btrfs_drop_extent_cache(inode, start, end - 1, 0);

<<<<<<< HEAD
	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

<<<<<<< HEAD
	while (1) {
		recow = 0;
		ret = btrfs_lookup_file_extent(trans, root, path, ino,
					       search_start, -1);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (start >= BTRFS_I(inode)->disk_i_size)
		modify_tree = 0;

	while (1) {
		recow = 0;
		ret = btrfs_lookup_file_extent(trans, root, path, ino,
					       search_start, modify_tree);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		if (ret < 0)
			break;
		if (ret > 0 && path->slots[0] > 0 && search_start == start) {
			leaf = path->nodes[0];
			btrfs_item_key_to_cpu(leaf, &key, path->slots[0] - 1);
			if (key.objectid == ino &&
			    key.type == BTRFS_EXTENT_DATA_KEY)
				path->slots[0]--;
		}
		ret = 0;
next_slot:
		leaf = path->nodes[0];
		if (path->slots[0] >= btrfs_header_nritems(leaf)) {
			BUG_ON(del_nr > 0);
			ret = btrfs_next_leaf(root, path);
			if (ret < 0)
				break;
			if (ret > 0) {
				ret = 0;
				break;
			}
			leaf = path->nodes[0];
			recow = 1;
		}

		btrfs_item_key_to_cpu(leaf, &key, path->slots[0]);
		if (key.objectid > ino ||
		    key.type > BTRFS_EXTENT_DATA_KEY || key.offset >= end)
			break;

		fi = btrfs_item_ptr(leaf, path->slots[0],
				    struct btrfs_file_extent_item);
		extent_type = btrfs_file_extent_type(leaf, fi);

		if (extent_type == BTRFS_FILE_EXTENT_REG ||
		    extent_type == BTRFS_FILE_EXTENT_PREALLOC) {
			disk_bytenr = btrfs_file_extent_disk_bytenr(leaf, fi);
			num_bytes = btrfs_file_extent_disk_num_bytes(leaf, fi);
			extent_offset = btrfs_file_extent_offset(leaf, fi);
			extent_end = key.offset +
				btrfs_file_extent_num_bytes(leaf, fi);
		} else if (extent_type == BTRFS_FILE_EXTENT_INLINE) {
			extent_end = key.offset +
				btrfs_file_extent_inline_len(leaf, fi);
		} else {
			WARN_ON(1);
			extent_end = search_start;
		}

		if (extent_end <= search_start) {
			path->slots[0]++;
			goto next_slot;
		}

<<<<<<< HEAD
		search_start = max(key.offset, start);
<<<<<<< HEAD
		if (recow) {
=======
		if (recow || !modify_tree) {
			modify_tree = -1;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		found = 1;
		search_start = max(key.offset, start);
		if (recow || !modify_tree) {
			modify_tree = -1;
>>>>>>> refs/remotes/origin/master
			btrfs_release_path(path);
			continue;
		}

		/*
		 *     | - range to drop - |
		 *  | -------- extent -------- |
		 */
		if (start > key.offset && end < extent_end) {
			BUG_ON(del_nr > 0);
			BUG_ON(extent_type == BTRFS_FILE_EXTENT_INLINE);

			memcpy(&new_key, &key, sizeof(new_key));
			new_key.offset = start;
			ret = btrfs_duplicate_item(trans, root, path,
						   &new_key);
			if (ret == -EAGAIN) {
				btrfs_release_path(path);
				continue;
			}
			if (ret < 0)
				break;

			leaf = path->nodes[0];
			fi = btrfs_item_ptr(leaf, path->slots[0] - 1,
					    struct btrfs_file_extent_item);
			btrfs_set_file_extent_num_bytes(leaf, fi,
							start - key.offset);

			fi = btrfs_item_ptr(leaf, path->slots[0],
					    struct btrfs_file_extent_item);

			extent_offset += start - key.offset;
			btrfs_set_file_extent_offset(leaf, fi, extent_offset);
			btrfs_set_file_extent_num_bytes(leaf, fi,
							extent_end - start);
			btrfs_mark_buffer_dirty(leaf);

<<<<<<< HEAD
			if (disk_bytenr > 0) {
=======
			if (update_refs && disk_bytenr > 0) {
>>>>>>> refs/remotes/origin/master
				ret = btrfs_inc_extent_ref(trans, root,
						disk_bytenr, num_bytes, 0,
						root->root_key.objectid,
						new_key.objectid,
<<<<<<< HEAD
<<<<<<< HEAD
						start - extent_offset);
				BUG_ON(ret);
=======
						start - extent_offset, 0);
				BUG_ON(ret); /* -ENOMEM */
>>>>>>> refs/remotes/origin/cm-10.0
				*hint_byte = disk_bytenr;
=======
						start - extent_offset, 0);
				BUG_ON(ret); /* -ENOMEM */
>>>>>>> refs/remotes/origin/master
			}
			key.offset = start;
		}
		/*
		 *  | ---- range to drop ----- |
		 *      | -------- extent -------- |
		 */
		if (start <= key.offset && end < extent_end) {
			BUG_ON(extent_type == BTRFS_FILE_EXTENT_INLINE);

			memcpy(&new_key, &key, sizeof(new_key));
			new_key.offset = end;
<<<<<<< HEAD
			btrfs_set_item_key_safe(trans, root, path, &new_key);
=======
			btrfs_set_item_key_safe(root, path, &new_key);
>>>>>>> refs/remotes/origin/master

			extent_offset += end - key.offset;
			btrfs_set_file_extent_offset(leaf, fi, extent_offset);
			btrfs_set_file_extent_num_bytes(leaf, fi,
							extent_end - end);
			btrfs_mark_buffer_dirty(leaf);
<<<<<<< HEAD
			if (disk_bytenr > 0) {
				inode_sub_bytes(inode, end - key.offset);
				*hint_byte = disk_bytenr;
			}
=======
			if (update_refs && disk_bytenr > 0)
				inode_sub_bytes(inode, end - key.offset);
>>>>>>> refs/remotes/origin/master
			break;
		}

		search_start = extent_end;
		/*
		 *       | ---- range to drop ----- |
		 *  | -------- extent -------- |
		 */
		if (start > key.offset && end >= extent_end) {
			BUG_ON(del_nr > 0);
			BUG_ON(extent_type == BTRFS_FILE_EXTENT_INLINE);

			btrfs_set_file_extent_num_bytes(leaf, fi,
							start - key.offset);
			btrfs_mark_buffer_dirty(leaf);
<<<<<<< HEAD
			if (disk_bytenr > 0) {
				inode_sub_bytes(inode, extent_end - start);
				*hint_byte = disk_bytenr;
			}
=======
			if (update_refs && disk_bytenr > 0)
				inode_sub_bytes(inode, extent_end - start);
>>>>>>> refs/remotes/origin/master
			if (end == extent_end)
				break;

			path->slots[0]++;
			goto next_slot;
		}

		/*
		 *  | ---- range to drop ----- |
		 *    | ------ extent ------ |
		 */
		if (start <= key.offset && end >= extent_end) {
			if (del_nr == 0) {
				del_slot = path->slots[0];
				del_nr = 1;
			} else {
				BUG_ON(del_slot + del_nr != path->slots[0]);
				del_nr++;
			}

<<<<<<< HEAD
			if (extent_type == BTRFS_FILE_EXTENT_INLINE) {
=======
			if (update_refs &&
			    extent_type == BTRFS_FILE_EXTENT_INLINE) {
>>>>>>> refs/remotes/origin/master
				inode_sub_bytes(inode,
						extent_end - key.offset);
				extent_end = ALIGN(extent_end,
						   root->sectorsize);
<<<<<<< HEAD
			} else if (disk_bytenr > 0) {
=======
			} else if (update_refs && disk_bytenr > 0) {
>>>>>>> refs/remotes/origin/master
				ret = btrfs_free_extent(trans, root,
						disk_bytenr, num_bytes, 0,
						root->root_key.objectid,
						key.objectid, key.offset -
<<<<<<< HEAD
<<<<<<< HEAD
						extent_offset);
				BUG_ON(ret);
=======
						extent_offset, 0);
				BUG_ON(ret); /* -ENOMEM */
>>>>>>> refs/remotes/origin/cm-10.0
				inode_sub_bytes(inode,
						extent_end - key.offset);
				*hint_byte = disk_bytenr;
=======
						extent_offset, 0);
				BUG_ON(ret); /* -ENOMEM */
				inode_sub_bytes(inode,
						extent_end - key.offset);
>>>>>>> refs/remotes/origin/master
			}

			if (end == extent_end)
				break;

			if (path->slots[0] + 1 < btrfs_header_nritems(leaf)) {
				path->slots[0]++;
				goto next_slot;
			}

			ret = btrfs_del_items(trans, root, path, del_slot,
					      del_nr);
<<<<<<< HEAD
<<<<<<< HEAD
			BUG_ON(ret);
=======
			if (ret) {
				btrfs_abort_transaction(trans, root, ret);
				goto out;
			}
>>>>>>> refs/remotes/origin/cm-10.0
=======
			if (ret) {
				btrfs_abort_transaction(trans, root, ret);
				break;
			}
>>>>>>> refs/remotes/origin/master

			del_nr = 0;
			del_slot = 0;

			btrfs_release_path(path);
			continue;
		}

		BUG_ON(1);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (del_nr > 0) {
		ret = btrfs_del_items(trans, root, path, del_slot, del_nr);
		BUG_ON(ret);
	}

=======
=======
>>>>>>> refs/remotes/origin/master
	if (!ret && del_nr > 0) {
		ret = btrfs_del_items(trans, root, path, del_slot, del_nr);
		if (ret)
			btrfs_abort_transaction(trans, root, ret);
	}

<<<<<<< HEAD
out:
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (drop_end)
		*drop_end = found ? min(end, extent_end) : end;
	btrfs_release_path(path);
	return ret;
}

int btrfs_drop_extents(struct btrfs_trans_handle *trans,
		       struct btrfs_root *root, struct inode *inode, u64 start,
		       u64 end, int drop_cache)
{
	struct btrfs_path *path;
	int ret;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;
	ret = __btrfs_drop_extents(trans, root, inode, path, start, end, NULL,
				   drop_cache);
>>>>>>> refs/remotes/origin/master
	btrfs_free_path(path);
	return ret;
}

static int extent_mergeable(struct extent_buffer *leaf, int slot,
			    u64 objectid, u64 bytenr, u64 orig_offset,
			    u64 *start, u64 *end)
{
	struct btrfs_file_extent_item *fi;
	struct btrfs_key key;
	u64 extent_end;

	if (slot < 0 || slot >= btrfs_header_nritems(leaf))
		return 0;

	btrfs_item_key_to_cpu(leaf, &key, slot);
	if (key.objectid != objectid || key.type != BTRFS_EXTENT_DATA_KEY)
		return 0;

	fi = btrfs_item_ptr(leaf, slot, struct btrfs_file_extent_item);
	if (btrfs_file_extent_type(leaf, fi) != BTRFS_FILE_EXTENT_REG ||
	    btrfs_file_extent_disk_bytenr(leaf, fi) != bytenr ||
	    btrfs_file_extent_offset(leaf, fi) != key.offset - orig_offset ||
	    btrfs_file_extent_compression(leaf, fi) ||
	    btrfs_file_extent_encryption(leaf, fi) ||
	    btrfs_file_extent_other_encoding(leaf, fi))
		return 0;

	extent_end = key.offset + btrfs_file_extent_num_bytes(leaf, fi);
	if ((*start && *start != key.offset) || (*end && *end != extent_end))
		return 0;

	*start = key.offset;
	*end = extent_end;
	return 1;
}

/*
 * Mark extent in the range start - end as written.
 *
 * This changes extent type from 'pre-allocated' to 'regular'. If only
 * part of extent is marked as written, the extent will be split into
 * two or three.
 */
int btrfs_mark_extent_written(struct btrfs_trans_handle *trans,
			      struct inode *inode, u64 start, u64 end)
{
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct extent_buffer *leaf;
	struct btrfs_path *path;
	struct btrfs_file_extent_item *fi;
	struct btrfs_key key;
	struct btrfs_key new_key;
	u64 bytenr;
	u64 num_bytes;
	u64 extent_end;
	u64 orig_offset;
	u64 other_start;
	u64 other_end;
	u64 split;
	int del_nr = 0;
	int del_slot = 0;
	int recow;
	int ret;
	u64 ino = btrfs_ino(inode);

<<<<<<< HEAD
	btrfs_drop_extent_cache(inode, start, end - 1, 0);

	path = btrfs_alloc_path();
<<<<<<< HEAD
	BUG_ON(!path);
=======
	if (!path)
		return -ENOMEM;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;
>>>>>>> refs/remotes/origin/master
again:
	recow = 0;
	split = start;
	key.objectid = ino;
	key.type = BTRFS_EXTENT_DATA_KEY;
	key.offset = split;

	ret = btrfs_search_slot(trans, root, &key, path, -1, 1);
	if (ret < 0)
		goto out;
	if (ret > 0 && path->slots[0] > 0)
		path->slots[0]--;

	leaf = path->nodes[0];
	btrfs_item_key_to_cpu(leaf, &key, path->slots[0]);
	BUG_ON(key.objectid != ino || key.type != BTRFS_EXTENT_DATA_KEY);
	fi = btrfs_item_ptr(leaf, path->slots[0],
			    struct btrfs_file_extent_item);
	BUG_ON(btrfs_file_extent_type(leaf, fi) !=
	       BTRFS_FILE_EXTENT_PREALLOC);
	extent_end = key.offset + btrfs_file_extent_num_bytes(leaf, fi);
	BUG_ON(key.offset > start || extent_end < end);

	bytenr = btrfs_file_extent_disk_bytenr(leaf, fi);
	num_bytes = btrfs_file_extent_disk_num_bytes(leaf, fi);
	orig_offset = key.offset - btrfs_file_extent_offset(leaf, fi);
	memcpy(&new_key, &key, sizeof(new_key));

	if (start == key.offset && end < extent_end) {
		other_start = 0;
		other_end = start;
		if (extent_mergeable(leaf, path->slots[0] - 1,
				     ino, bytenr, orig_offset,
				     &other_start, &other_end)) {
			new_key.offset = end;
<<<<<<< HEAD
			btrfs_set_item_key_safe(trans, root, path, &new_key);
			fi = btrfs_item_ptr(leaf, path->slots[0],
					    struct btrfs_file_extent_item);
=======
			btrfs_set_item_key_safe(root, path, &new_key);
			fi = btrfs_item_ptr(leaf, path->slots[0],
					    struct btrfs_file_extent_item);
			btrfs_set_file_extent_generation(leaf, fi,
							 trans->transid);
>>>>>>> refs/remotes/origin/master
			btrfs_set_file_extent_num_bytes(leaf, fi,
							extent_end - end);
			btrfs_set_file_extent_offset(leaf, fi,
						     end - orig_offset);
			fi = btrfs_item_ptr(leaf, path->slots[0] - 1,
					    struct btrfs_file_extent_item);
<<<<<<< HEAD
=======
			btrfs_set_file_extent_generation(leaf, fi,
							 trans->transid);
>>>>>>> refs/remotes/origin/master
			btrfs_set_file_extent_num_bytes(leaf, fi,
							end - other_start);
			btrfs_mark_buffer_dirty(leaf);
			goto out;
		}
	}

	if (start > key.offset && end == extent_end) {
		other_start = end;
		other_end = 0;
		if (extent_mergeable(leaf, path->slots[0] + 1,
				     ino, bytenr, orig_offset,
				     &other_start, &other_end)) {
			fi = btrfs_item_ptr(leaf, path->slots[0],
					    struct btrfs_file_extent_item);
			btrfs_set_file_extent_num_bytes(leaf, fi,
							start - key.offset);
<<<<<<< HEAD
			path->slots[0]++;
			new_key.offset = start;
			btrfs_set_item_key_safe(trans, root, path, &new_key);

			fi = btrfs_item_ptr(leaf, path->slots[0],
					    struct btrfs_file_extent_item);
=======
			btrfs_set_file_extent_generation(leaf, fi,
							 trans->transid);
			path->slots[0]++;
			new_key.offset = start;
			btrfs_set_item_key_safe(root, path, &new_key);

			fi = btrfs_item_ptr(leaf, path->slots[0],
					    struct btrfs_file_extent_item);
			btrfs_set_file_extent_generation(leaf, fi,
							 trans->transid);
>>>>>>> refs/remotes/origin/master
			btrfs_set_file_extent_num_bytes(leaf, fi,
							other_end - start);
			btrfs_set_file_extent_offset(leaf, fi,
						     start - orig_offset);
			btrfs_mark_buffer_dirty(leaf);
			goto out;
		}
	}

	while (start > key.offset || end < extent_end) {
		if (key.offset == start)
			split = end;

		new_key.offset = split;
		ret = btrfs_duplicate_item(trans, root, path, &new_key);
		if (ret == -EAGAIN) {
			btrfs_release_path(path);
			goto again;
		}
<<<<<<< HEAD
<<<<<<< HEAD
		BUG_ON(ret < 0);
=======
=======
>>>>>>> refs/remotes/origin/master
		if (ret < 0) {
			btrfs_abort_transaction(trans, root, ret);
			goto out;
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

		leaf = path->nodes[0];
		fi = btrfs_item_ptr(leaf, path->slots[0] - 1,
				    struct btrfs_file_extent_item);
<<<<<<< HEAD
=======
		btrfs_set_file_extent_generation(leaf, fi, trans->transid);
>>>>>>> refs/remotes/origin/master
		btrfs_set_file_extent_num_bytes(leaf, fi,
						split - key.offset);

		fi = btrfs_item_ptr(leaf, path->slots[0],
				    struct btrfs_file_extent_item);

<<<<<<< HEAD
=======
		btrfs_set_file_extent_generation(leaf, fi, trans->transid);
>>>>>>> refs/remotes/origin/master
		btrfs_set_file_extent_offset(leaf, fi, split - orig_offset);
		btrfs_set_file_extent_num_bytes(leaf, fi,
						extent_end - split);
		btrfs_mark_buffer_dirty(leaf);

		ret = btrfs_inc_extent_ref(trans, root, bytenr, num_bytes, 0,
					   root->root_key.objectid,
<<<<<<< HEAD
<<<<<<< HEAD
					   ino, orig_offset);
		BUG_ON(ret);
=======
					   ino, orig_offset, 0);
		BUG_ON(ret); /* -ENOMEM */
>>>>>>> refs/remotes/origin/cm-10.0
=======
					   ino, orig_offset, 0);
		BUG_ON(ret); /* -ENOMEM */
>>>>>>> refs/remotes/origin/master

		if (split == start) {
			key.offset = start;
		} else {
			BUG_ON(start != key.offset);
			path->slots[0]--;
			extent_end = end;
		}
		recow = 1;
	}

	other_start = end;
	other_end = 0;
	if (extent_mergeable(leaf, path->slots[0] + 1,
			     ino, bytenr, orig_offset,
			     &other_start, &other_end)) {
		if (recow) {
			btrfs_release_path(path);
			goto again;
		}
		extent_end = other_end;
		del_slot = path->slots[0] + 1;
		del_nr++;
		ret = btrfs_free_extent(trans, root, bytenr, num_bytes,
					0, root->root_key.objectid,
<<<<<<< HEAD
<<<<<<< HEAD
					ino, orig_offset);
		BUG_ON(ret);
=======
					ino, orig_offset, 0);
		BUG_ON(ret); /* -ENOMEM */
>>>>>>> refs/remotes/origin/cm-10.0
=======
					ino, orig_offset, 0);
		BUG_ON(ret); /* -ENOMEM */
>>>>>>> refs/remotes/origin/master
	}
	other_start = 0;
	other_end = start;
	if (extent_mergeable(leaf, path->slots[0] - 1,
			     ino, bytenr, orig_offset,
			     &other_start, &other_end)) {
		if (recow) {
			btrfs_release_path(path);
			goto again;
		}
		key.offset = other_start;
		del_slot = path->slots[0];
		del_nr++;
		ret = btrfs_free_extent(trans, root, bytenr, num_bytes,
					0, root->root_key.objectid,
<<<<<<< HEAD
<<<<<<< HEAD
					ino, orig_offset);
		BUG_ON(ret);
=======
					ino, orig_offset, 0);
		BUG_ON(ret); /* -ENOMEM */
>>>>>>> refs/remotes/origin/cm-10.0
=======
					ino, orig_offset, 0);
		BUG_ON(ret); /* -ENOMEM */
>>>>>>> refs/remotes/origin/master
	}
	if (del_nr == 0) {
		fi = btrfs_item_ptr(leaf, path->slots[0],
			   struct btrfs_file_extent_item);
		btrfs_set_file_extent_type(leaf, fi,
					   BTRFS_FILE_EXTENT_REG);
<<<<<<< HEAD
=======
		btrfs_set_file_extent_generation(leaf, fi, trans->transid);
>>>>>>> refs/remotes/origin/master
		btrfs_mark_buffer_dirty(leaf);
	} else {
		fi = btrfs_item_ptr(leaf, del_slot - 1,
			   struct btrfs_file_extent_item);
		btrfs_set_file_extent_type(leaf, fi,
					   BTRFS_FILE_EXTENT_REG);
<<<<<<< HEAD
=======
		btrfs_set_file_extent_generation(leaf, fi, trans->transid);
>>>>>>> refs/remotes/origin/master
		btrfs_set_file_extent_num_bytes(leaf, fi,
						extent_end - key.offset);
		btrfs_mark_buffer_dirty(leaf);

		ret = btrfs_del_items(trans, root, path, del_slot, del_nr);
<<<<<<< HEAD
<<<<<<< HEAD
		BUG_ON(ret);
=======
=======
>>>>>>> refs/remotes/origin/master
		if (ret < 0) {
			btrfs_abort_transaction(trans, root, ret);
			goto out;
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}
out:
	btrfs_free_path(path);
	return 0;
}

/*
 * on error we return an unlocked page and the error value
 * on success we return a locked page and 0
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int prepare_uptodate_page(struct page *page, u64 pos)
{
	int ret = 0;

	if ((pos & (PAGE_CACHE_SIZE - 1)) && !PageUptodate(page)) {
=======
=======
>>>>>>> refs/remotes/origin/master
static int prepare_uptodate_page(struct page *page, u64 pos,
				 bool force_uptodate)
{
	int ret = 0;

	if (((pos & (PAGE_CACHE_SIZE - 1)) || force_uptodate) &&
	    !PageUptodate(page)) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		ret = btrfs_readpage(NULL, page);
		if (ret)
			return ret;
		lock_page(page);
		if (!PageUptodate(page)) {
			unlock_page(page);
			return -EIO;
		}
	}
	return 0;
}

/*
 * this gets pages into the page cache and locks them down, it also properly
 * waits for data=ordered extents to finish before allowing the pages to be
 * modified.
 */
static noinline int prepare_pages(struct btrfs_root *root, struct file *file,
			 struct page **pages, size_t num_pages,
			 loff_t pos, unsigned long first_index,
<<<<<<< HEAD
<<<<<<< HEAD
			 unsigned long last_index, size_t write_bytes)
=======
			 size_t write_bytes, bool force_uptodate)
>>>>>>> refs/remotes/origin/cm-10.0
=======
			 size_t write_bytes, bool force_uptodate)
>>>>>>> refs/remotes/origin/master
{
	struct extent_state *cached_state = NULL;
	int i;
	unsigned long index = pos >> PAGE_CACHE_SHIFT;
<<<<<<< HEAD
	struct inode *inode = fdentry(file)->d_inode;
<<<<<<< HEAD
=======
	gfp_t mask = btrfs_alloc_write_mask(inode->i_mapping);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct inode *inode = file_inode(file);
	gfp_t mask = btrfs_alloc_write_mask(inode->i_mapping);
>>>>>>> refs/remotes/origin/master
	int err = 0;
	int faili = 0;
	u64 start_pos;
	u64 last_pos;

	start_pos = pos & ~((u64)root->sectorsize - 1);
	last_pos = ((u64)index + num_pages) << PAGE_CACHE_SHIFT;

<<<<<<< HEAD
<<<<<<< HEAD
	if (start_pos > inode->i_size) {
		err = btrfs_cont_expand(inode, i_size_read(inode), start_pos);
		if (err)
			return err;
	}

again:
	for (i = 0; i < num_pages; i++) {
		pages[i] = grab_cache_page(inode->i_mapping, index + i);
=======
=======
>>>>>>> refs/remotes/origin/master
again:
	for (i = 0; i < num_pages; i++) {
		pages[i] = find_or_create_page(inode->i_mapping, index + i,
					       mask | __GFP_WRITE);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		if (!pages[i]) {
			faili = i - 1;
			err = -ENOMEM;
			goto fail;
		}

		if (i == 0)
<<<<<<< HEAD
<<<<<<< HEAD
			err = prepare_uptodate_page(pages[i], pos);
		if (i == num_pages - 1)
			err = prepare_uptodate_page(pages[i],
						    pos + write_bytes);
=======
=======
>>>>>>> refs/remotes/origin/master
			err = prepare_uptodate_page(pages[i], pos,
						    force_uptodate);
		if (i == num_pages - 1)
			err = prepare_uptodate_page(pages[i],
						    pos + write_bytes, false);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		if (err) {
			page_cache_release(pages[i]);
			faili = i - 1;
			goto fail;
		}
		wait_on_page_writeback(pages[i]);
	}
<<<<<<< HEAD
=======
	faili = num_pages - 1;
>>>>>>> refs/remotes/origin/master
	err = 0;
	if (start_pos < inode->i_size) {
		struct btrfs_ordered_extent *ordered;
		lock_extent_bits(&BTRFS_I(inode)->io_tree,
<<<<<<< HEAD
<<<<<<< HEAD
				 start_pos, last_pos - 1, 0, &cached_state,
				 GFP_NOFS);
=======
				 start_pos, last_pos - 1, 0, &cached_state);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				 start_pos, last_pos - 1, 0, &cached_state);
>>>>>>> refs/remotes/origin/master
		ordered = btrfs_lookup_first_ordered_extent(inode,
							    last_pos - 1);
		if (ordered &&
		    ordered->file_offset + ordered->len > start_pos &&
		    ordered->file_offset < last_pos) {
			btrfs_put_ordered_extent(ordered);
			unlock_extent_cached(&BTRFS_I(inode)->io_tree,
					     start_pos, last_pos - 1,
					     &cached_state, GFP_NOFS);
			for (i = 0; i < num_pages; i++) {
				unlock_page(pages[i]);
				page_cache_release(pages[i]);
			}
<<<<<<< HEAD
			btrfs_wait_ordered_range(inode, start_pos,
						 last_pos - start_pos);
=======
			err = btrfs_wait_ordered_range(inode, start_pos,
						       last_pos - start_pos);
			if (err)
				goto fail;
>>>>>>> refs/remotes/origin/master
			goto again;
		}
		if (ordered)
			btrfs_put_ordered_extent(ordered);

		clear_extent_bit(&BTRFS_I(inode)->io_tree, start_pos,
				  last_pos - 1, EXTENT_DIRTY | EXTENT_DELALLOC |
<<<<<<< HEAD
				  EXTENT_DO_ACCOUNTING, 0, 0, &cached_state,
				  GFP_NOFS);
=======
				  EXTENT_DO_ACCOUNTING | EXTENT_DEFRAG,
				  0, 0, &cached_state, GFP_NOFS);
>>>>>>> refs/remotes/origin/master
		unlock_extent_cached(&BTRFS_I(inode)->io_tree,
				     start_pos, last_pos - 1, &cached_state,
				     GFP_NOFS);
	}
	for (i = 0; i < num_pages; i++) {
<<<<<<< HEAD
<<<<<<< HEAD
		clear_page_dirty_for_io(pages[i]);
=======
		if (clear_page_dirty_for_io(pages[i]))
			account_page_redirty(pages[i]);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (clear_page_dirty_for_io(pages[i]))
			account_page_redirty(pages[i]);
>>>>>>> refs/remotes/origin/master
		set_page_extent_mapped(pages[i]);
		WARN_ON(!PageLocked(pages[i]));
	}
	return 0;
fail:
	while (faili >= 0) {
		unlock_page(pages[faili]);
		page_cache_release(pages[faili]);
		faili--;
	}
	return err;

}

<<<<<<< HEAD
=======
static noinline int check_can_nocow(struct inode *inode, loff_t pos,
				    size_t *write_bytes)
{
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct btrfs_ordered_extent *ordered;
	u64 lockstart, lockend;
	u64 num_bytes;
	int ret;

	lockstart = round_down(pos, root->sectorsize);
	lockend = lockstart + round_up(*write_bytes, root->sectorsize) - 1;

	while (1) {
		lock_extent(&BTRFS_I(inode)->io_tree, lockstart, lockend);
		ordered = btrfs_lookup_ordered_range(inode, lockstart,
						     lockend - lockstart + 1);
		if (!ordered) {
			break;
		}
		unlock_extent(&BTRFS_I(inode)->io_tree, lockstart, lockend);
		btrfs_start_ordered_extent(inode, ordered, 1);
		btrfs_put_ordered_extent(ordered);
	}

	num_bytes = lockend - lockstart + 1;
	ret = can_nocow_extent(inode, lockstart, &num_bytes, NULL, NULL, NULL);
	if (ret <= 0) {
		ret = 0;
	} else {
		clear_extent_bit(&BTRFS_I(inode)->io_tree, lockstart, lockend,
				 EXTENT_DIRTY | EXTENT_DELALLOC |
				 EXTENT_DO_ACCOUNTING | EXTENT_DEFRAG, 0, 0,
				 NULL, GFP_NOFS);
		*write_bytes = min_t(size_t, *write_bytes, num_bytes);
	}

	unlock_extent(&BTRFS_I(inode)->io_tree, lockstart, lockend);

	return ret;
}

>>>>>>> refs/remotes/origin/master
static noinline ssize_t __btrfs_buffered_write(struct file *file,
					       struct iov_iter *i,
					       loff_t pos)
{
<<<<<<< HEAD
	struct inode *inode = fdentry(file)->d_inode;
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct page **pages = NULL;
	unsigned long first_index;
<<<<<<< HEAD
	unsigned long last_index;
	size_t num_written = 0;
	int nrptrs;
	int ret = 0;
=======
	size_t num_written = 0;
	int nrptrs;
	int ret = 0;
	bool force_page_uptodate = false;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct inode *inode = file_inode(file);
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct page **pages = NULL;
	u64 release_bytes = 0;
	unsigned long first_index;
	size_t num_written = 0;
	int nrptrs;
	int ret = 0;
	bool only_release_metadata = false;
	bool force_page_uptodate = false;
>>>>>>> refs/remotes/origin/master

	nrptrs = min((iov_iter_count(i) + PAGE_CACHE_SIZE - 1) /
		     PAGE_CACHE_SIZE, PAGE_CACHE_SIZE /
		     (sizeof(struct page *)));
<<<<<<< HEAD
<<<<<<< HEAD
=======
	nrptrs = min(nrptrs, current->nr_dirtied_pause - current->nr_dirtied);
	nrptrs = max(nrptrs, 8);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	nrptrs = min(nrptrs, current->nr_dirtied_pause - current->nr_dirtied);
	nrptrs = max(nrptrs, 8);
>>>>>>> refs/remotes/origin/master
	pages = kmalloc(nrptrs * sizeof(struct page *), GFP_KERNEL);
	if (!pages)
		return -ENOMEM;

	first_index = pos >> PAGE_CACHE_SHIFT;
<<<<<<< HEAD
<<<<<<< HEAD
	last_index = (pos + iov_iter_count(i)) >> PAGE_CACHE_SHIFT;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	while (iov_iter_count(i) > 0) {
		size_t offset = pos & (PAGE_CACHE_SIZE - 1);
		size_t write_bytes = min(iov_iter_count(i),
					 nrptrs * (size_t)PAGE_CACHE_SIZE -
					 offset);
		size_t num_pages = (write_bytes + offset +
				    PAGE_CACHE_SIZE - 1) >> PAGE_CACHE_SHIFT;
<<<<<<< HEAD
=======
		size_t reserve_bytes;
>>>>>>> refs/remotes/origin/master
		size_t dirty_pages;
		size_t copied;

		WARN_ON(num_pages > nrptrs);

		/*
		 * Fault pages before locking them in prepare_pages
		 * to avoid recursive lock
		 */
		if (unlikely(iov_iter_fault_in_readable(i, write_bytes))) {
			ret = -EFAULT;
			break;
		}

<<<<<<< HEAD
		ret = btrfs_delalloc_reserve_space(inode,
					num_pages << PAGE_CACHE_SHIFT);
		if (ret)
			break;

=======
		reserve_bytes = num_pages << PAGE_CACHE_SHIFT;
		ret = btrfs_check_data_free_space(inode, reserve_bytes);
		if (ret == -ENOSPC &&
		    (BTRFS_I(inode)->flags & (BTRFS_INODE_NODATACOW |
					      BTRFS_INODE_PREALLOC))) {
			ret = check_can_nocow(inode, pos, &write_bytes);
			if (ret > 0) {
				only_release_metadata = true;
				/*
				 * our prealloc extent may be smaller than
				 * write_bytes, so scale down.
				 */
				num_pages = (write_bytes + offset +
					     PAGE_CACHE_SIZE - 1) >>
					PAGE_CACHE_SHIFT;
				reserve_bytes = num_pages << PAGE_CACHE_SHIFT;
				ret = 0;
			} else {
				ret = -ENOSPC;
			}
		}

		if (ret)
			break;

		ret = btrfs_delalloc_reserve_metadata(inode, reserve_bytes);
		if (ret) {
			if (!only_release_metadata)
				btrfs_free_reserved_data_space(inode,
							       reserve_bytes);
			break;
		}

		release_bytes = reserve_bytes;

>>>>>>> refs/remotes/origin/master
		/*
		 * This is going to setup the pages array with the number of
		 * pages we want, so we don't really need to worry about the
		 * contents of pages from loop to loop
		 */
		ret = prepare_pages(root, file, pages, num_pages,
<<<<<<< HEAD
<<<<<<< HEAD
				    pos, first_index, last_index,
				    write_bytes);
=======
				    pos, first_index, write_bytes,
				    force_page_uptodate);
>>>>>>> refs/remotes/origin/cm-10.0
		if (ret) {
			btrfs_delalloc_release_space(inode,
					num_pages << PAGE_CACHE_SHIFT);
			break;
		}
=======
				    pos, first_index, write_bytes,
				    force_page_uptodate);
		if (ret)
			break;
>>>>>>> refs/remotes/origin/master

		copied = btrfs_copy_from_user(pos, num_pages,
					   write_bytes, pages, i);

		/*
		 * if we have trouble faulting in the pages, fall
		 * back to one page at a time
		 */
		if (copied < write_bytes)
			nrptrs = 1;

<<<<<<< HEAD
<<<<<<< HEAD
		if (copied == 0)
			dirty_pages = 0;
		else
			dirty_pages = (copied + offset +
				       PAGE_CACHE_SIZE - 1) >>
				       PAGE_CACHE_SHIFT;
=======
=======
>>>>>>> refs/remotes/origin/master
		if (copied == 0) {
			force_page_uptodate = true;
			dirty_pages = 0;
		} else {
			force_page_uptodate = false;
			dirty_pages = (copied + offset +
				       PAGE_CACHE_SIZE - 1) >>
				       PAGE_CACHE_SHIFT;
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

		/*
		 * If we had a short copy we need to release the excess delaloc
		 * bytes we reserved.  We need to increment outstanding_extents
		 * because btrfs_delalloc_release_space will decrement it, but
		 * we still have an outstanding extent for the chunk we actually
		 * managed to copy.
		 */
		if (num_pages > dirty_pages) {
<<<<<<< HEAD
<<<<<<< HEAD
			if (copied > 0)
				atomic_inc(
					&BTRFS_I(inode)->outstanding_extents);
=======
=======
			release_bytes = (num_pages - dirty_pages) <<
				PAGE_CACHE_SHIFT;
>>>>>>> refs/remotes/origin/master
			if (copied > 0) {
				spin_lock(&BTRFS_I(inode)->lock);
				BTRFS_I(inode)->outstanding_extents++;
				spin_unlock(&BTRFS_I(inode)->lock);
			}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
			btrfs_delalloc_release_space(inode,
					(num_pages - dirty_pages) <<
					PAGE_CACHE_SHIFT);
		}

=======
			if (only_release_metadata)
				btrfs_delalloc_release_metadata(inode,
								release_bytes);
			else
				btrfs_delalloc_release_space(inode,
							     release_bytes);
		}

		release_bytes = dirty_pages << PAGE_CACHE_SHIFT;
>>>>>>> refs/remotes/origin/master
		if (copied > 0) {
			ret = btrfs_dirty_pages(root, inode, pages,
						dirty_pages, pos, copied,
						NULL);
			if (ret) {
<<<<<<< HEAD
				btrfs_delalloc_release_space(inode,
					dirty_pages << PAGE_CACHE_SHIFT);
=======
>>>>>>> refs/remotes/origin/master
				btrfs_drop_pages(pages, num_pages);
				break;
			}
		}

<<<<<<< HEAD
		btrfs_drop_pages(pages, num_pages);

		cond_resched();

		balance_dirty_pages_ratelimited_nr(inode->i_mapping,
						   dirty_pages);
		if (dirty_pages < (root->leafsize >> PAGE_CACHE_SHIFT) + 1)
			btrfs_btree_balance_dirty(root, 1);
<<<<<<< HEAD
		btrfs_throttle(root);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
		release_bytes = 0;
		btrfs_drop_pages(pages, num_pages);

		if (only_release_metadata && copied > 0) {
			u64 lockstart = round_down(pos, root->sectorsize);
			u64 lockend = lockstart +
				(dirty_pages << PAGE_CACHE_SHIFT) - 1;

			set_extent_bit(&BTRFS_I(inode)->io_tree, lockstart,
				       lockend, EXTENT_NORESERVE, NULL,
				       NULL, GFP_NOFS);
			only_release_metadata = false;
		}

		cond_resched();

		balance_dirty_pages_ratelimited(inode->i_mapping);
		if (dirty_pages < (root->leafsize >> PAGE_CACHE_SHIFT) + 1)
			btrfs_btree_balance_dirty(root);
>>>>>>> refs/remotes/origin/master

		pos += copied;
		num_written += copied;
	}

	kfree(pages);

<<<<<<< HEAD
=======
	if (release_bytes) {
		if (only_release_metadata)
			btrfs_delalloc_release_metadata(inode, release_bytes);
		else
			btrfs_delalloc_release_space(inode, release_bytes);
	}

>>>>>>> refs/remotes/origin/master
	return num_written ? num_written : ret;
}

static ssize_t __btrfs_direct_write(struct kiocb *iocb,
				    const struct iovec *iov,
				    unsigned long nr_segs, loff_t pos,
				    loff_t *ppos, size_t count, size_t ocount)
{
	struct file *file = iocb->ki_filp;
<<<<<<< HEAD
	struct inode *inode = fdentry(file)->d_inode;
=======
>>>>>>> refs/remotes/origin/master
	struct iov_iter i;
	ssize_t written;
	ssize_t written_buffered;
	loff_t endbyte;
	int err;

	written = generic_file_direct_write(iocb, iov, &nr_segs, pos, ppos,
					    count, ocount);

<<<<<<< HEAD
	/*
	 * the generic O_DIRECT will update in-memory i_size after the
	 * DIOs are done.  But our endio handlers that update the on
	 * disk i_size never update past the in memory i_size.  So we
	 * need one more update here to catch any additions to the
	 * file
	 */
	if (inode->i_size != BTRFS_I(inode)->disk_i_size) {
		btrfs_ordered_update_i_size(inode, inode->i_size, NULL);
		mark_inode_dirty(inode);
	}

=======
>>>>>>> refs/remotes/origin/master
	if (written < 0 || written == count)
		return written;

	pos += written;
	count -= written;
	iov_iter_init(&i, iov, nr_segs, count, written);
	written_buffered = __btrfs_buffered_write(file, &i, pos);
	if (written_buffered < 0) {
		err = written_buffered;
		goto out;
	}
	endbyte = pos + written_buffered - 1;
	err = filemap_write_and_wait_range(file->f_mapping, pos, endbyte);
	if (err)
		goto out;
	written += written_buffered;
	*ppos = pos + written_buffered;
	invalidate_mapping_pages(file->f_mapping, pos >> PAGE_CACHE_SHIFT,
				 endbyte >> PAGE_CACHE_SHIFT);
out:
	return written ? written : err;
}

<<<<<<< HEAD
=======
static void update_time_for_write(struct inode *inode)
{
	struct timespec now;

	if (IS_NOCMTIME(inode))
		return;

	now = current_fs_time(inode->i_sb);
	if (!timespec_equal(&inode->i_mtime, &now))
		inode->i_mtime = now;

	if (!timespec_equal(&inode->i_ctime, &now))
		inode->i_ctime = now;

	if (IS_I_VERSION(inode))
		inode_inc_iversion(inode);
}

>>>>>>> refs/remotes/origin/master
static ssize_t btrfs_file_aio_write(struct kiocb *iocb,
				    const struct iovec *iov,
				    unsigned long nr_segs, loff_t pos)
{
	struct file *file = iocb->ki_filp;
<<<<<<< HEAD
	struct inode *inode = fdentry(file)->d_inode;
	struct btrfs_root *root = BTRFS_I(inode)->root;
	loff_t *ppos = &iocb->ki_pos;
<<<<<<< HEAD
=======
	u64 start_pos;
>>>>>>> refs/remotes/origin/cm-10.0
	ssize_t num_written = 0;
	ssize_t err = 0;
	size_t count, ocount;

	vfs_check_frozen(inode->i_sb, SB_FREEZE_WRITE);
=======
	struct inode *inode = file_inode(file);
	struct btrfs_root *root = BTRFS_I(inode)->root;
	loff_t *ppos = &iocb->ki_pos;
	u64 start_pos;
	ssize_t num_written = 0;
	ssize_t err = 0;
	size_t count, ocount;
	bool sync = (file->f_flags & O_DSYNC) || IS_SYNC(file->f_mapping->host);
>>>>>>> refs/remotes/origin/master

	mutex_lock(&inode->i_mutex);

	err = generic_segment_checks(iov, &nr_segs, &ocount, VERIFY_READ);
	if (err) {
		mutex_unlock(&inode->i_mutex);
		goto out;
	}
	count = ocount;

	current->backing_dev_info = inode->i_mapping->backing_dev_info;
	err = generic_write_checks(file, &pos, &count, S_ISBLK(inode->i_mode));
	if (err) {
		mutex_unlock(&inode->i_mutex);
		goto out;
	}

	if (count == 0) {
		mutex_unlock(&inode->i_mutex);
		goto out;
	}

	err = file_remove_suid(file);
	if (err) {
		mutex_unlock(&inode->i_mutex);
		goto out;
	}

	/*
	 * If BTRFS flips readonly due to some impossible error
	 * (fs_info->fs_state now has BTRFS_SUPER_FLAG_ERROR),
	 * although we have opened a file as writable, we have
	 * to stop this write operation to ensure FS consistency.
	 */
<<<<<<< HEAD
	if (root->fs_info->fs_state & BTRFS_SUPER_FLAG_ERROR) {
=======
	if (test_bit(BTRFS_FS_STATE_ERROR, &root->fs_info->fs_state)) {
>>>>>>> refs/remotes/origin/master
		mutex_unlock(&inode->i_mutex);
		err = -EROFS;
		goto out;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	file_update_time(file);
	BTRFS_I(inode)->sequence++;

=======
	err = btrfs_update_time(file);
	if (err) {
		mutex_unlock(&inode->i_mutex);
		goto out;
	}
	BTRFS_I(inode)->sequence++;
=======
	/*
	 * We reserve space for updating the inode when we reserve space for the
	 * extent we are going to write, so we will enospc out there.  We don't
	 * need to start yet another transaction to update the inode as we will
	 * update the inode when we finish writing whatever data we write.
	 */
	update_time_for_write(inode);
>>>>>>> refs/remotes/origin/master

	start_pos = round_down(pos, root->sectorsize);
	if (start_pos > i_size_read(inode)) {
		err = btrfs_cont_expand(inode, i_size_read(inode), start_pos);
		if (err) {
			mutex_unlock(&inode->i_mutex);
			goto out;
		}
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (sync)
		atomic_inc(&BTRFS_I(inode)->sync_writers);

>>>>>>> refs/remotes/origin/master
	if (unlikely(file->f_flags & O_DIRECT)) {
		num_written = __btrfs_direct_write(iocb, iov, nr_segs,
						   pos, ppos, count, ocount);
	} else {
		struct iov_iter i;

		iov_iter_init(&i, iov, nr_segs, count, num_written);

		num_written = __btrfs_buffered_write(file, &i, pos);
		if (num_written > 0)
			*ppos = pos + num_written;
	}

	mutex_unlock(&inode->i_mutex);

	/*
	 * we want to make sure fsync finds this change
	 * but we haven't joined a transaction running right now.
	 *
	 * Later on, someone is sure to update the inode and get the
	 * real transid recorded.
	 *
	 * We set last_trans now to the fs_info generation + 1,
	 * this will either be one more than the running transaction
	 * or the generation used for the next transaction if there isn't
	 * one running right now.
<<<<<<< HEAD
	 */
	BTRFS_I(inode)->last_trans = root->fs_info->generation + 1;
	if (num_written > 0 || num_written == -EIOCBQUEUED) {
=======
	 *
	 * We also have to set last_sub_trans to the current log transid,
	 * otherwise subsequent syncs to a file that's been synced in this
	 * transaction will appear to have already occured.
	 */
	BTRFS_I(inode)->last_trans = root->fs_info->generation + 1;
	BTRFS_I(inode)->last_sub_trans = root->log_transid;
	if (num_written > 0) {
>>>>>>> refs/remotes/origin/master
		err = generic_write_sync(file, pos, num_written);
		if (err < 0 && num_written > 0)
			num_written = err;
	}
<<<<<<< HEAD
=======

	if (sync)
		atomic_dec(&BTRFS_I(inode)->sync_writers);
>>>>>>> refs/remotes/origin/master
out:
	current->backing_dev_info = NULL;
	return num_written ? num_written : err;
}

int btrfs_release_file(struct inode *inode, struct file *filp)
{
	/*
	 * ordered_data_close is set by settattr when we are about to truncate
	 * a file from a non-zero size to a zero size.  This tries to
	 * flush down new bytes that may have been written if the
	 * application were using truncate to replace a file in place.
	 */
<<<<<<< HEAD
	if (BTRFS_I(inode)->ordered_data_close) {
		BTRFS_I(inode)->ordered_data_close = 0;
		btrfs_add_ordered_operation(NULL, BTRFS_I(inode)->root, inode);
=======
	if (test_and_clear_bit(BTRFS_INODE_ORDERED_DATA_CLOSE,
			       &BTRFS_I(inode)->runtime_flags)) {
		struct btrfs_trans_handle *trans;
		struct btrfs_root *root = BTRFS_I(inode)->root;

		/*
		 * We need to block on a committing transaction to keep us from
		 * throwing a ordered operation on to the list and causing
		 * something like sync to deadlock trying to flush out this
		 * inode.
		 */
		trans = btrfs_start_transaction(root, 0);
		if (IS_ERR(trans))
			return PTR_ERR(trans);
		btrfs_add_ordered_operation(trans, BTRFS_I(inode)->root, inode);
		btrfs_end_transaction(trans, root);
>>>>>>> refs/remotes/origin/master
		if (inode->i_size > BTRFS_ORDERED_OPERATIONS_FLUSH_LIMIT)
			filemap_flush(inode->i_mapping);
	}
	if (filp->private_data)
		btrfs_ioctl_trans_end(filp);
	return 0;
}

/*
 * fsync call for both files and directories.  This logs the inode into
 * the tree log instead of forcing full commits whenever possible.
 *
 * It needs to call filemap_fdatawait so that all ordered extent updates are
 * in the metadata btree are up to date for copying to the log.
 *
 * It drops the inode mutex before doing the tree log commit.  This is an
 * important optimization for directories because holding the mutex prevents
 * new operations on the dir while we write to disk.
 */
<<<<<<< HEAD
<<<<<<< HEAD
int btrfs_sync_file(struct file *file, int datasync)
=======
int btrfs_sync_file(struct file *file, loff_t start, loff_t end, int datasync)
>>>>>>> refs/remotes/origin/cm-10.0
=======
int btrfs_sync_file(struct file *file, loff_t start, loff_t end, int datasync)
>>>>>>> refs/remotes/origin/master
{
	struct dentry *dentry = file->f_path.dentry;
	struct inode *inode = dentry->d_inode;
	struct btrfs_root *root = BTRFS_I(inode)->root;
	int ret = 0;
	struct btrfs_trans_handle *trans;
<<<<<<< HEAD

	trace_btrfs_sync_file(file, datasync);

<<<<<<< HEAD
	/* we wait first, since the writeback may change the inode */
	root->log_batch++;
	/* the VFS called filemap_fdatawrite for us */
=======
	ret = filemap_write_and_wait_range(inode->i_mapping, start, end);
	if (ret)
		return ret;
	mutex_lock(&inode->i_mutex);

	/* we wait first, since the writeback may change the inode */
	root->log_batch++;
>>>>>>> refs/remotes/origin/cm-10.0
	btrfs_wait_ordered_range(inode, 0, (u64)-1);
	root->log_batch++;
=======
	bool full_sync = 0;

	trace_btrfs_sync_file(file, datasync);

	/*
	 * We write the dirty pages in the range and wait until they complete
	 * out of the ->i_mutex. If so, we can flush the dirty pages by
	 * multi-task, and make the performance up.  See
	 * btrfs_wait_ordered_range for an explanation of the ASYNC check.
	 */
	atomic_inc(&BTRFS_I(inode)->sync_writers);
	ret = filemap_fdatawrite_range(inode->i_mapping, start, end);
	if (!ret && test_bit(BTRFS_INODE_HAS_ASYNC_EXTENT,
			     &BTRFS_I(inode)->runtime_flags))
		ret = filemap_fdatawrite_range(inode->i_mapping, start, end);
	atomic_dec(&BTRFS_I(inode)->sync_writers);
	if (ret)
		return ret;

	mutex_lock(&inode->i_mutex);

	/*
	 * We flush the dirty pages again to avoid some dirty pages in the
	 * range being left.
	 */
	atomic_inc(&root->log_batch);
	full_sync = test_bit(BTRFS_INODE_NEEDS_FULL_SYNC,
			     &BTRFS_I(inode)->runtime_flags);
	if (full_sync) {
		ret = btrfs_wait_ordered_range(inode, start, end - start + 1);
		if (ret) {
			mutex_unlock(&inode->i_mutex);
			goto out;
		}
	}
	atomic_inc(&root->log_batch);
>>>>>>> refs/remotes/origin/master

	/*
	 * check the transaction that last modified this inode
	 * and see if its already been committed
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (!BTRFS_I(inode)->last_trans)
		goto out;
=======
=======
>>>>>>> refs/remotes/origin/master
	if (!BTRFS_I(inode)->last_trans) {
		mutex_unlock(&inode->i_mutex);
		goto out;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	/*
	 * if the last transaction that changed this file was before
	 * the current transaction, we can bail out now without any
	 * syncing
	 */
	smp_mb();
<<<<<<< HEAD
	if (BTRFS_I(inode)->last_trans <=
	    root->fs_info->last_trans_committed) {
		BTRFS_I(inode)->last_trans = 0;
<<<<<<< HEAD
=======
		mutex_unlock(&inode->i_mutex);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (btrfs_inode_in_log(inode, root->fs_info->generation) ||
	    BTRFS_I(inode)->last_trans <=
	    root->fs_info->last_trans_committed) {
		BTRFS_I(inode)->last_trans = 0;

		/*
		 * We'v had everything committed since the last time we were
		 * modified so clear this flag in case it was set for whatever
		 * reason, it's no longer relevant.
		 */
		clear_bit(BTRFS_INODE_NEEDS_FULL_SYNC,
			  &BTRFS_I(inode)->runtime_flags);
		mutex_unlock(&inode->i_mutex);
>>>>>>> refs/remotes/origin/master
		goto out;
	}

	/*
	 * ok we haven't committed the transaction yet, lets do a commit
	 */
	if (file->private_data)
		btrfs_ioctl_trans_end(file);

	trans = btrfs_start_transaction(root, 0);
	if (IS_ERR(trans)) {
		ret = PTR_ERR(trans);
<<<<<<< HEAD
<<<<<<< HEAD
=======
		mutex_unlock(&inode->i_mutex);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		mutex_unlock(&inode->i_mutex);
>>>>>>> refs/remotes/origin/master
		goto out;
	}

	ret = btrfs_log_dentry_safe(trans, root, dentry);
<<<<<<< HEAD
<<<<<<< HEAD
	if (ret < 0)
		goto out;
=======
	if (ret < 0) {
		mutex_unlock(&inode->i_mutex);
		goto out;
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (ret < 0) {
		/* Fallthrough and commit/free transaction. */
		ret = 1;
	}
>>>>>>> refs/remotes/origin/master

	/* we've logged all the items and now have a consistent
	 * version of the file in the log.  It is possible that
	 * someone will come in and modify the file, but that's
	 * fine because the log is consistent on disk, and we
	 * have references to all of the file's extents
	 *
	 * It is possible that someone will come in and log the
	 * file again, but that will end up using the synchronization
	 * inside btrfs_sync_log to keep things safe.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	mutex_unlock(&dentry->d_inode->i_mutex);
=======
	mutex_unlock(&inode->i_mutex);
>>>>>>> refs/remotes/origin/cm-10.0

	if (ret != BTRFS_NO_LOG_SYNC) {
		if (ret > 0) {
			ret = btrfs_commit_transaction(trans, root);
		} else {
			ret = btrfs_sync_log(trans, root);
			if (ret == 0)
				ret = btrfs_end_transaction(trans, root);
			else
				ret = btrfs_commit_transaction(trans, root);
		}
	} else {
		ret = btrfs_end_transaction(trans, root);
	}
<<<<<<< HEAD
	mutex_lock(&dentry->d_inode->i_mutex);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	mutex_unlock(&inode->i_mutex);

	if (ret != BTRFS_NO_LOG_SYNC) {
		if (!ret) {
			ret = btrfs_sync_log(trans, root);
			if (!ret) {
				ret = btrfs_end_transaction(trans, root);
				goto out;
			}
		}
		if (!full_sync) {
			ret = btrfs_wait_ordered_range(inode, start,
						       end - start + 1);
			if (ret)
				goto out;
		}
		ret = btrfs_commit_transaction(trans, root);
	} else {
		ret = btrfs_end_transaction(trans, root);
	}
>>>>>>> refs/remotes/origin/master
out:
	return ret > 0 ? -EIO : ret;
}

static const struct vm_operations_struct btrfs_file_vm_ops = {
	.fault		= filemap_fault,
	.page_mkwrite	= btrfs_page_mkwrite,
<<<<<<< HEAD
=======
	.remap_pages	= generic_file_remap_pages,
>>>>>>> refs/remotes/origin/master
};

static int btrfs_file_mmap(struct file	*filp, struct vm_area_struct *vma)
{
	struct address_space *mapping = filp->f_mapping;

	if (!mapping->a_ops->readpage)
		return -ENOEXEC;

	file_accessed(filp);
	vma->vm_ops = &btrfs_file_vm_ops;
<<<<<<< HEAD
	vma->vm_flags |= VM_CAN_NONLINEAR;
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
static long btrfs_fallocate(struct file *file, int mode,
			    loff_t offset, loff_t len)
{
	struct inode *inode = file->f_path.dentry->d_inode;
	struct extent_state *cached_state = NULL;
=======
static int hole_mergeable(struct inode *inode, struct extent_buffer *leaf,
			  int slot, u64 start, u64 end)
{
	struct btrfs_file_extent_item *fi;
	struct btrfs_key key;

	if (slot < 0 || slot >= btrfs_header_nritems(leaf))
		return 0;

	btrfs_item_key_to_cpu(leaf, &key, slot);
	if (key.objectid != btrfs_ino(inode) ||
	    key.type != BTRFS_EXTENT_DATA_KEY)
		return 0;

	fi = btrfs_item_ptr(leaf, slot, struct btrfs_file_extent_item);

	if (btrfs_file_extent_type(leaf, fi) != BTRFS_FILE_EXTENT_REG)
		return 0;

	if (btrfs_file_extent_disk_bytenr(leaf, fi))
		return 0;

	if (key.offset == end)
		return 1;
	if (key.offset + btrfs_file_extent_num_bytes(leaf, fi) == start)
		return 1;
	return 0;
}

static int fill_holes(struct btrfs_trans_handle *trans, struct inode *inode,
		      struct btrfs_path *path, u64 offset, u64 end)
{
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct extent_buffer *leaf;
	struct btrfs_file_extent_item *fi;
	struct extent_map *hole_em;
	struct extent_map_tree *em_tree = &BTRFS_I(inode)->extent_tree;
	struct btrfs_key key;
	int ret;

	key.objectid = btrfs_ino(inode);
	key.type = BTRFS_EXTENT_DATA_KEY;
	key.offset = offset;


	ret = btrfs_search_slot(trans, root, &key, path, 0, 1);
	if (ret < 0)
		return ret;
	BUG_ON(!ret);

	leaf = path->nodes[0];
	if (hole_mergeable(inode, leaf, path->slots[0]-1, offset, end)) {
		u64 num_bytes;

		path->slots[0]--;
		fi = btrfs_item_ptr(leaf, path->slots[0],
				    struct btrfs_file_extent_item);
		num_bytes = btrfs_file_extent_num_bytes(leaf, fi) +
			end - offset;
		btrfs_set_file_extent_num_bytes(leaf, fi, num_bytes);
		btrfs_set_file_extent_ram_bytes(leaf, fi, num_bytes);
		btrfs_set_file_extent_offset(leaf, fi, 0);
		btrfs_mark_buffer_dirty(leaf);
		goto out;
	}

	if (hole_mergeable(inode, leaf, path->slots[0]+1, offset, end)) {
		u64 num_bytes;

		path->slots[0]++;
		key.offset = offset;
		btrfs_set_item_key_safe(root, path, &key);
		fi = btrfs_item_ptr(leaf, path->slots[0],
				    struct btrfs_file_extent_item);
		num_bytes = btrfs_file_extent_num_bytes(leaf, fi) + end -
			offset;
		btrfs_set_file_extent_num_bytes(leaf, fi, num_bytes);
		btrfs_set_file_extent_ram_bytes(leaf, fi, num_bytes);
		btrfs_set_file_extent_offset(leaf, fi, 0);
		btrfs_mark_buffer_dirty(leaf);
		goto out;
	}
	btrfs_release_path(path);

	ret = btrfs_insert_file_extent(trans, root, btrfs_ino(inode), offset,
				       0, 0, end - offset, 0, end - offset,
				       0, 0, 0);
	if (ret)
		return ret;

out:
	btrfs_release_path(path);

	hole_em = alloc_extent_map();
	if (!hole_em) {
		btrfs_drop_extent_cache(inode, offset, end - 1, 0);
		set_bit(BTRFS_INODE_NEEDS_FULL_SYNC,
			&BTRFS_I(inode)->runtime_flags);
	} else {
		hole_em->start = offset;
		hole_em->len = end - offset;
		hole_em->ram_bytes = hole_em->len;
		hole_em->orig_start = offset;

		hole_em->block_start = EXTENT_MAP_HOLE;
		hole_em->block_len = 0;
		hole_em->orig_block_len = 0;
		hole_em->bdev = root->fs_info->fs_devices->latest_bdev;
		hole_em->compress_type = BTRFS_COMPRESS_NONE;
		hole_em->generation = trans->transid;

		do {
			btrfs_drop_extent_cache(inode, offset, end - 1, 0);
			write_lock(&em_tree->lock);
			ret = add_extent_mapping(em_tree, hole_em, 1);
			write_unlock(&em_tree->lock);
		} while (ret == -EEXIST);
		free_extent_map(hole_em);
		if (ret)
			set_bit(BTRFS_INODE_NEEDS_FULL_SYNC,
				&BTRFS_I(inode)->runtime_flags);
	}

	return 0;
}

static int btrfs_punch_hole(struct inode *inode, loff_t offset, loff_t len)
{
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct extent_state *cached_state = NULL;
	struct btrfs_path *path;
	struct btrfs_block_rsv *rsv;
	struct btrfs_trans_handle *trans;
	u64 lockstart = round_up(offset, BTRFS_I(inode)->root->sectorsize);
	u64 lockend = round_down(offset + len,
				 BTRFS_I(inode)->root->sectorsize) - 1;
	u64 cur_offset = lockstart;
	u64 min_size = btrfs_calc_trunc_metadata_size(root, 1);
	u64 drop_end;
	int ret = 0;
	int err = 0;
	bool same_page = ((offset >> PAGE_CACHE_SHIFT) ==
			  ((offset + len - 1) >> PAGE_CACHE_SHIFT));

	ret = btrfs_wait_ordered_range(inode, offset, len);
	if (ret)
		return ret;

	mutex_lock(&inode->i_mutex);
	/*
	 * We needn't truncate any page which is beyond the end of the file
	 * because we are sure there is no data there.
	 */
	/*
	 * Only do this if we are in the same page and we aren't doing the
	 * entire page.
	 */
	if (same_page && len < PAGE_CACHE_SIZE) {
		if (offset < round_up(inode->i_size, PAGE_CACHE_SIZE))
			ret = btrfs_truncate_page(inode, offset, len, 0);
		mutex_unlock(&inode->i_mutex);
		return ret;
	}

	/* zero back part of the first page */
	if (offset < round_up(inode->i_size, PAGE_CACHE_SIZE)) {
		ret = btrfs_truncate_page(inode, offset, 0, 0);
		if (ret) {
			mutex_unlock(&inode->i_mutex);
			return ret;
		}
	}

	/* zero the front end of the last page */
	if (offset + len < round_up(inode->i_size, PAGE_CACHE_SIZE)) {
		ret = btrfs_truncate_page(inode, offset + len, 0, 1);
		if (ret) {
			mutex_unlock(&inode->i_mutex);
			return ret;
		}
	}

	if (lockend < lockstart) {
		mutex_unlock(&inode->i_mutex);
		return 0;
	}

	while (1) {
		struct btrfs_ordered_extent *ordered;

		truncate_pagecache_range(inode, lockstart, lockend);

		lock_extent_bits(&BTRFS_I(inode)->io_tree, lockstart, lockend,
				 0, &cached_state);
		ordered = btrfs_lookup_first_ordered_extent(inode, lockend);

		/*
		 * We need to make sure we have no ordered extents in this range
		 * and nobody raced in and read a page in this range, if we did
		 * we need to try again.
		 */
		if ((!ordered ||
		    (ordered->file_offset + ordered->len < lockstart ||
		     ordered->file_offset > lockend)) &&
		     !test_range_bit(&BTRFS_I(inode)->io_tree, lockstart,
				     lockend, EXTENT_UPTODATE, 0,
				     cached_state)) {
			if (ordered)
				btrfs_put_ordered_extent(ordered);
			break;
		}
		if (ordered)
			btrfs_put_ordered_extent(ordered);
		unlock_extent_cached(&BTRFS_I(inode)->io_tree, lockstart,
				     lockend, &cached_state, GFP_NOFS);
		ret = btrfs_wait_ordered_range(inode, lockstart,
					       lockend - lockstart + 1);
		if (ret) {
			mutex_unlock(&inode->i_mutex);
			return ret;
		}
	}

	path = btrfs_alloc_path();
	if (!path) {
		ret = -ENOMEM;
		goto out;
	}

	rsv = btrfs_alloc_block_rsv(root, BTRFS_BLOCK_RSV_TEMP);
	if (!rsv) {
		ret = -ENOMEM;
		goto out_free;
	}
	rsv->size = btrfs_calc_trunc_metadata_size(root, 1);
	rsv->failfast = 1;

	/*
	 * 1 - update the inode
	 * 1 - removing the extents in the range
	 * 1 - adding the hole extent
	 */
	trans = btrfs_start_transaction(root, 3);
	if (IS_ERR(trans)) {
		err = PTR_ERR(trans);
		goto out_free;
	}

	ret = btrfs_block_rsv_migrate(&root->fs_info->trans_block_rsv, rsv,
				      min_size);
	BUG_ON(ret);
	trans->block_rsv = rsv;

	while (cur_offset < lockend) {
		ret = __btrfs_drop_extents(trans, root, inode, path,
					   cur_offset, lockend + 1,
					   &drop_end, 1);
		if (ret != -ENOSPC)
			break;

		trans->block_rsv = &root->fs_info->trans_block_rsv;

		ret = fill_holes(trans, inode, path, cur_offset, drop_end);
		if (ret) {
			err = ret;
			break;
		}

		cur_offset = drop_end;

		ret = btrfs_update_inode(trans, root, inode);
		if (ret) {
			err = ret;
			break;
		}

		btrfs_end_transaction(trans, root);
		btrfs_btree_balance_dirty(root);

		trans = btrfs_start_transaction(root, 3);
		if (IS_ERR(trans)) {
			ret = PTR_ERR(trans);
			trans = NULL;
			break;
		}

		ret = btrfs_block_rsv_migrate(&root->fs_info->trans_block_rsv,
					      rsv, min_size);
		BUG_ON(ret);	/* shouldn't happen */
		trans->block_rsv = rsv;
	}

	if (ret) {
		err = ret;
		goto out_trans;
	}

	trans->block_rsv = &root->fs_info->trans_block_rsv;
	ret = fill_holes(trans, inode, path, cur_offset, drop_end);
	if (ret) {
		err = ret;
		goto out_trans;
	}

out_trans:
	if (!trans)
		goto out_free;

	inode_inc_iversion(inode);
	inode->i_mtime = inode->i_ctime = CURRENT_TIME;

	trans->block_rsv = &root->fs_info->trans_block_rsv;
	ret = btrfs_update_inode(trans, root, inode);
	btrfs_end_transaction(trans, root);
	btrfs_btree_balance_dirty(root);
out_free:
	btrfs_free_path(path);
	btrfs_free_block_rsv(root, rsv);
out:
	unlock_extent_cached(&BTRFS_I(inode)->io_tree, lockstart, lockend,
			     &cached_state, GFP_NOFS);
	mutex_unlock(&inode->i_mutex);
	if (ret && !err)
		err = ret;
	return err;
}

static long btrfs_fallocate(struct file *file, int mode,
			    loff_t offset, loff_t len)
{
	struct inode *inode = file_inode(file);
	struct extent_state *cached_state = NULL;
	struct btrfs_root *root = BTRFS_I(inode)->root;
>>>>>>> refs/remotes/origin/master
	u64 cur_offset;
	u64 last_byte;
	u64 alloc_start;
	u64 alloc_end;
	u64 alloc_hint = 0;
	u64 locked_end;
<<<<<<< HEAD
	u64 mask = BTRFS_I(inode)->root->sectorsize - 1;
	struct extent_map *em;
	int ret;

	alloc_start = offset & ~mask;
	alloc_end =  (offset + len + mask) & ~mask;

	/* We only support the FALLOC_FL_KEEP_SIZE mode */
	if (mode & ~FALLOC_FL_KEEP_SIZE)
		return -EOPNOTSUPP;

	/*
<<<<<<< HEAD
=======
	 * Make sure we have enough space before we do the
	 * allocation.
	 */
	ret = btrfs_check_data_free_space(inode, len);
	if (ret)
		return ret;

	/*
>>>>>>> refs/remotes/origin/cm-10.0
	 * wait for ordered IO before we have any locks.  We'll loop again
	 * below with the locks held.
	 */
	btrfs_wait_ordered_range(inode, alloc_start, alloc_end - alloc_start);
=======
	struct extent_map *em;
	int blocksize = BTRFS_I(inode)->root->sectorsize;
	int ret;

	alloc_start = round_down(offset, blocksize);
	alloc_end = round_up(offset + len, blocksize);

	/* Make sure we aren't being give some crap mode */
	if (mode & ~(FALLOC_FL_KEEP_SIZE | FALLOC_FL_PUNCH_HOLE))
		return -EOPNOTSUPP;

	if (mode & FALLOC_FL_PUNCH_HOLE)
		return btrfs_punch_hole(inode, offset, len);

	/*
	 * Make sure we have enough space before we do the
	 * allocation.
	 */
	ret = btrfs_check_data_free_space(inode, alloc_end - alloc_start);
	if (ret)
		return ret;
	if (root->fs_info->quota_enabled) {
		ret = btrfs_qgroup_reserve(root, alloc_end - alloc_start);
		if (ret)
			goto out_reserve_fail;
	}
>>>>>>> refs/remotes/origin/master

	mutex_lock(&inode->i_mutex);
	ret = inode_newsize_ok(inode, alloc_end);
	if (ret)
		goto out;

	if (alloc_start > inode->i_size) {
		ret = btrfs_cont_expand(inode, i_size_read(inode),
					alloc_start);
		if (ret)
			goto out;
<<<<<<< HEAD
	}

<<<<<<< HEAD
	ret = btrfs_check_data_free_space(inode, alloc_end - alloc_start);
	if (ret)
		goto out;

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	} else {
		/*
		 * If we are fallocating from the end of the file onward we
		 * need to zero out the end of the page if i_size lands in the
		 * middle of a page.
		 */
		ret = btrfs_truncate_page(inode, inode->i_size, 0, 0);
		if (ret)
			goto out;
	}

	/*
	 * wait for ordered IO before we have any locks.  We'll loop again
	 * below with the locks held.
	 */
	ret = btrfs_wait_ordered_range(inode, alloc_start,
				       alloc_end - alloc_start);
	if (ret)
		goto out;

>>>>>>> refs/remotes/origin/master
	locked_end = alloc_end - 1;
	while (1) {
		struct btrfs_ordered_extent *ordered;

		/* the extent lock is ordered inside the running
		 * transaction
		 */
		lock_extent_bits(&BTRFS_I(inode)->io_tree, alloc_start,
<<<<<<< HEAD
<<<<<<< HEAD
				 locked_end, 0, &cached_state, GFP_NOFS);
=======
				 locked_end, 0, &cached_state);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				 locked_end, 0, &cached_state);
>>>>>>> refs/remotes/origin/master
		ordered = btrfs_lookup_first_ordered_extent(inode,
							    alloc_end - 1);
		if (ordered &&
		    ordered->file_offset + ordered->len > alloc_start &&
		    ordered->file_offset < alloc_end) {
			btrfs_put_ordered_extent(ordered);
			unlock_extent_cached(&BTRFS_I(inode)->io_tree,
					     alloc_start, locked_end,
					     &cached_state, GFP_NOFS);
			/*
			 * we can't wait on the range with the transaction
			 * running or with the extent lock held
			 */
<<<<<<< HEAD
			btrfs_wait_ordered_range(inode, alloc_start,
						 alloc_end - alloc_start);
=======
			ret = btrfs_wait_ordered_range(inode, alloc_start,
						       alloc_end - alloc_start);
			if (ret)
				goto out;
>>>>>>> refs/remotes/origin/master
		} else {
			if (ordered)
				btrfs_put_ordered_extent(ordered);
			break;
		}
	}

	cur_offset = alloc_start;
	while (1) {
<<<<<<< HEAD
<<<<<<< HEAD
		em = btrfs_get_extent(inode, NULL, 0, cur_offset,
				      alloc_end - cur_offset, 0);
		BUG_ON(IS_ERR_OR_NULL(em));
		last_byte = min(extent_map_end(em), alloc_end);
		last_byte = (last_byte + mask) & ~mask;
=======
=======
>>>>>>> refs/remotes/origin/master
		u64 actual_end;

		em = btrfs_get_extent(inode, NULL, 0, cur_offset,
				      alloc_end - cur_offset, 0);
		if (IS_ERR_OR_NULL(em)) {
			if (!em)
				ret = -ENOMEM;
			else
				ret = PTR_ERR(em);
			break;
		}
		last_byte = min(extent_map_end(em), alloc_end);
		actual_end = min_t(u64, extent_map_end(em), offset + len);
<<<<<<< HEAD
		last_byte = (last_byte + mask) & ~mask;

>>>>>>> refs/remotes/origin/cm-10.0
=======
		last_byte = ALIGN(last_byte, blocksize);

>>>>>>> refs/remotes/origin/master
		if (em->block_start == EXTENT_MAP_HOLE ||
		    (cur_offset >= inode->i_size &&
		     !test_bit(EXTENT_FLAG_PREALLOC, &em->flags))) {
			ret = btrfs_prealloc_file_range(inode, mode, cur_offset,
							last_byte - cur_offset,
							1 << inode->i_blkbits,
							offset + len,
							&alloc_hint);
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/cm-10.0
=======

>>>>>>> refs/remotes/origin/master
			if (ret < 0) {
				free_extent_map(em);
				break;
			}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
		} else if (actual_end > inode->i_size &&
			   !(mode & FALLOC_FL_KEEP_SIZE)) {
			/*
			 * We didn't need to allocate any more space, but we
			 * still extended the size of the file so we need to
			 * update i_size.
			 */
			inode->i_ctime = CURRENT_TIME;
			i_size_write(inode, actual_end);
			btrfs_ordered_update_i_size(inode, actual_end, NULL);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		}
		free_extent_map(em);

		cur_offset = last_byte;
		if (cur_offset >= alloc_end) {
			ret = 0;
			break;
		}
	}
	unlock_extent_cached(&BTRFS_I(inode)->io_tree, alloc_start, locked_end,
			     &cached_state, GFP_NOFS);
<<<<<<< HEAD
<<<<<<< HEAD

	btrfs_free_reserved_data_space(inode, alloc_end - alloc_start);
out:
	mutex_unlock(&inode->i_mutex);
	return ret;
}

const struct file_operations btrfs_file_operations = {
	.llseek		= generic_file_llseek,
=======
out:
	mutex_unlock(&inode->i_mutex);
	/* Let go of our reservation. */
	btrfs_free_reserved_data_space(inode, len);
	return ret;
}

static int find_desired_extent(struct inode *inode, loff_t *offset, int origin)
{
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct extent_map *em;
=======
out:
	mutex_unlock(&inode->i_mutex);
	if (root->fs_info->quota_enabled)
		btrfs_qgroup_free(root, alloc_end - alloc_start);
out_reserve_fail:
	/* Let go of our reservation. */
	btrfs_free_reserved_data_space(inode, alloc_end - alloc_start);
	return ret;
}

static int find_desired_extent(struct inode *inode, loff_t *offset, int whence)
{
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct extent_map *em = NULL;
>>>>>>> refs/remotes/origin/master
	struct extent_state *cached_state = NULL;
	u64 lockstart = *offset;
	u64 lockend = i_size_read(inode);
	u64 start = *offset;
<<<<<<< HEAD
	u64 orig_start = *offset;
	u64 len = i_size_read(inode);
	u64 last_end = 0;
=======
	u64 len = i_size_read(inode);
>>>>>>> refs/remotes/origin/master
	int ret = 0;

	lockend = max_t(u64, root->sectorsize, lockend);
	if (lockend <= lockstart)
		lockend = lockstart + root->sectorsize;

<<<<<<< HEAD
=======
	lockend--;
>>>>>>> refs/remotes/origin/master
	len = lockend - lockstart + 1;

	len = max_t(u64, len, root->sectorsize);
	if (inode->i_size == 0)
		return -ENXIO;

	lock_extent_bits(&BTRFS_I(inode)->io_tree, lockstart, lockend, 0,
			 &cached_state);

<<<<<<< HEAD
	/*
	 * Delalloc is such a pain.  If we have a hole and we have pending
	 * delalloc for a portion of the hole we will get back a hole that
	 * exists for the entire range since it hasn't been actually written
	 * yet.  So to take care of this case we need to look for an extent just
	 * before the position we want in case there is outstanding delalloc
	 * going on here.
	 */
	if (origin == SEEK_HOLE && start != 0) {
		if (start <= root->sectorsize)
			em = btrfs_get_extent_fiemap(inode, NULL, 0, 0,
						     root->sectorsize, 0);
		else
			em = btrfs_get_extent_fiemap(inode, NULL, 0,
						     start - root->sectorsize,
						     root->sectorsize, 0);
		if (IS_ERR(em)) {
			ret = PTR_ERR(em);
			goto out;
		}
		last_end = em->start + em->len;
		if (em->block_start == EXTENT_MAP_DELALLOC)
			last_end = min_t(u64, last_end, inode->i_size);
		free_extent_map(em);
	}

	while (1) {
		em = btrfs_get_extent_fiemap(inode, NULL, 0, start, len, 0);
		if (IS_ERR(em)) {
			ret = PTR_ERR(em);
			break;
		}

		if (em->block_start == EXTENT_MAP_HOLE) {
			if (test_bit(EXTENT_FLAG_VACANCY, &em->flags)) {
				if (last_end <= orig_start) {
					free_extent_map(em);
					ret = -ENXIO;
					break;
				}
			}

			if (origin == SEEK_HOLE) {
				*offset = start;
				free_extent_map(em);
				break;
			}
		} else {
			if (origin == SEEK_DATA) {
				if (em->block_start == EXTENT_MAP_DELALLOC) {
					if (start >= inode->i_size) {
						free_extent_map(em);
						ret = -ENXIO;
						break;
					}
				}

				*offset = start;
				free_extent_map(em);
				break;
			}
		}

		start = em->start + em->len;
		last_end = em->start + em->len;

		if (em->block_start == EXTENT_MAP_DELALLOC)
			last_end = min_t(u64, last_end, inode->i_size);

		if (test_bit(EXTENT_FLAG_VACANCY, &em->flags)) {
			free_extent_map(em);
			ret = -ENXIO;
			break;
		}
		free_extent_map(em);
		cond_resched();
	}
	if (!ret)
		*offset = min(*offset, inode->i_size);
out:
=======
	while (start < inode->i_size) {
		em = btrfs_get_extent_fiemap(inode, NULL, 0, start, len, 0);
		if (IS_ERR(em)) {
			ret = PTR_ERR(em);
			em = NULL;
			break;
		}

		if (whence == SEEK_HOLE &&
		    (em->block_start == EXTENT_MAP_HOLE ||
		     test_bit(EXTENT_FLAG_PREALLOC, &em->flags)))
			break;
		else if (whence == SEEK_DATA &&
			   (em->block_start != EXTENT_MAP_HOLE &&
			    !test_bit(EXTENT_FLAG_PREALLOC, &em->flags)))
			break;

		start = em->start + em->len;
		free_extent_map(em);
		em = NULL;
		cond_resched();
	}
	free_extent_map(em);
	if (!ret) {
		if (whence == SEEK_DATA && start >= inode->i_size)
			ret = -ENXIO;
		else
			*offset = min_t(loff_t, start, inode->i_size);
	}
>>>>>>> refs/remotes/origin/master
	unlock_extent_cached(&BTRFS_I(inode)->io_tree, lockstart, lockend,
			     &cached_state, GFP_NOFS);
	return ret;
}

<<<<<<< HEAD
static loff_t btrfs_file_llseek(struct file *file, loff_t offset, int origin)
=======
static loff_t btrfs_file_llseek(struct file *file, loff_t offset, int whence)
>>>>>>> refs/remotes/origin/master
{
	struct inode *inode = file->f_mapping->host;
	int ret;

	mutex_lock(&inode->i_mutex);
<<<<<<< HEAD
	switch (origin) {
	case SEEK_END:
	case SEEK_CUR:
		offset = generic_file_llseek(file, offset, origin);
=======
	switch (whence) {
	case SEEK_END:
	case SEEK_CUR:
		offset = generic_file_llseek(file, offset, whence);
>>>>>>> refs/remotes/origin/master
		goto out;
	case SEEK_DATA:
	case SEEK_HOLE:
		if (offset >= i_size_read(inode)) {
			mutex_unlock(&inode->i_mutex);
			return -ENXIO;
		}

<<<<<<< HEAD
		ret = find_desired_extent(inode, &offset, origin);
=======
		ret = find_desired_extent(inode, &offset, whence);
>>>>>>> refs/remotes/origin/master
		if (ret) {
			mutex_unlock(&inode->i_mutex);
			return ret;
		}
	}

<<<<<<< HEAD
	if (offset < 0 && !(file->f_mode & FMODE_UNSIGNED_OFFSET)) {
		offset = -EINVAL;
		goto out;
	}
	if (offset > inode->i_sb->s_maxbytes) {
		offset = -EINVAL;
		goto out;
	}

	/* Special lock needed here? */
	if (offset != file->f_pos) {
		file->f_pos = offset;
		file->f_version = 0;
	}
=======
	offset = vfs_setpos(file, offset, inode->i_sb->s_maxbytes);
>>>>>>> refs/remotes/origin/master
out:
	mutex_unlock(&inode->i_mutex);
	return offset;
}

const struct file_operations btrfs_file_operations = {
	.llseek		= btrfs_file_llseek,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	.read		= do_sync_read,
	.write		= do_sync_write,
	.aio_read       = generic_file_aio_read,
	.splice_read	= generic_file_splice_read,
	.aio_write	= btrfs_file_aio_write,
	.mmap		= btrfs_file_mmap,
	.open		= generic_file_open,
	.release	= btrfs_release_file,
	.fsync		= btrfs_sync_file,
	.fallocate	= btrfs_fallocate,
	.unlocked_ioctl	= btrfs_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl	= btrfs_ioctl,
#endif
};
<<<<<<< HEAD
=======

void btrfs_auto_defrag_exit(void)
{
	if (btrfs_inode_defrag_cachep)
		kmem_cache_destroy(btrfs_inode_defrag_cachep);
}

int btrfs_auto_defrag_init(void)
{
	btrfs_inode_defrag_cachep = kmem_cache_create("btrfs_inode_defrag",
					sizeof(struct inode_defrag), 0,
					SLAB_RECLAIM_ACCOUNT | SLAB_MEM_SPREAD,
					NULL);
	if (!btrfs_inode_defrag_cachep)
		return -ENOMEM;

	return 0;
}
>>>>>>> refs/remotes/origin/master
