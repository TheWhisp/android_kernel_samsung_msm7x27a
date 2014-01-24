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

#ifndef __BTRFS_I__
#define __BTRFS_I__

<<<<<<< HEAD
=======
#include <linux/hash.h>
>>>>>>> refs/remotes/origin/master
#include "extent_map.h"
#include "extent_io.h"
#include "ordered-data.h"
#include "delayed-inode.h"

<<<<<<< HEAD
=======
/*
 * ordered_data_close is set by truncate when a file that used
 * to have good data has been truncated to zero.  When it is set
 * the btrfs file release call will add this inode to the
 * ordered operations list so that we make sure to flush out any
 * new data the application may have written before commit.
 */
#define BTRFS_INODE_ORDERED_DATA_CLOSE		0
#define BTRFS_INODE_ORPHAN_META_RESERVED	1
#define BTRFS_INODE_DUMMY			2
#define BTRFS_INODE_IN_DEFRAG			3
#define BTRFS_INODE_DELALLOC_META_RESERVED	4
#define BTRFS_INODE_HAS_ORPHAN_ITEM		5
#define BTRFS_INODE_HAS_ASYNC_EXTENT		6
#define BTRFS_INODE_NEEDS_FULL_SYNC		7
#define BTRFS_INODE_COPY_EVERYTHING		8
#define BTRFS_INODE_IN_DELALLOC_LIST		9
#define BTRFS_INODE_READDIO_NEED_LOCK		10

>>>>>>> refs/remotes/origin/master
/* in memory btrfs inode */
struct btrfs_inode {
	/* which subvolume this inode belongs to */
	struct btrfs_root *root;

	/* key used to find this inode on disk.  This is used by the code
	 * to read in roots of subvolumes
	 */
	struct btrfs_key location;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	/* Lock for counters */
	spinlock_t lock;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* Lock for counters */
	spinlock_t lock;

>>>>>>> refs/remotes/origin/master
	/* the extent_tree has caches of all the extent mappings to disk */
	struct extent_map_tree extent_tree;

	/* the io_tree does range state (DIRTY, LOCKED etc) */
	struct extent_io_tree io_tree;

	/* special utility tree used to record which mirrors have already been
	 * tried when checksums fail for a given block
	 */
	struct extent_io_tree io_failure_tree;

	/* held while logging the inode in tree-log.c */
	struct mutex log_mutex;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	/* held while doing delalloc reservations */
	struct mutex delalloc_mutex;

>>>>>>> refs/remotes/origin/cm-10.0
	/* used to order data wrt metadata */
	struct btrfs_ordered_inode_tree ordered_tree;

	/* for keeping track of orphaned inodes */
	struct list_head i_orphan;

=======
	/* held while doing delalloc reservations */
	struct mutex delalloc_mutex;

	/* used to order data wrt metadata */
	struct btrfs_ordered_inode_tree ordered_tree;

>>>>>>> refs/remotes/origin/master
	/* list of all the delalloc inodes in the FS.  There are times we need
	 * to write all the delalloc pages to disk, and this list is used
	 * to walk them all.
	 */
	struct list_head delalloc_inodes;

	/*
	 * list for tracking inodes that must be sent to disk before a
	 * rename or truncate commit
	 */
	struct list_head ordered_operations;

	/* node for the red-black tree that links inodes in subvolume root */
	struct rb_node rb_node;

<<<<<<< HEAD
	/* the space_info for where this inode's data allocations are done */
	struct btrfs_space_info *space_info;
=======
	unsigned long runtime_flags;

	/* Keep track of who's O_SYNC/fsyncing currently */
	atomic_t sync_writers;
>>>>>>> refs/remotes/origin/master

	/* full 64 bit generation number, struct vfs_inode doesn't have a big
	 * enough field for this.
	 */
	u64 generation;

<<<<<<< HEAD
	/* sequence number for NFS changes */
	u64 sequence;

=======
>>>>>>> refs/remotes/origin/master
	/*
	 * transid of the trans_handle that last modified this inode
	 */
	u64 last_trans;

	/*
	 * log transid when this inode was last modified
	 */
	u64 last_sub_trans;

	/*
	 * transid that last logged this inode
	 */
	u64 logged_trans;

	/* total number of bytes pending delalloc, used by stat to calc the
	 * real block usage of the file
	 */
	u64 delalloc_bytes;

<<<<<<< HEAD
<<<<<<< HEAD
	/* total number of bytes that may be used for this inode for
	 * delalloc
	 */
	u64 reserved_bytes;

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * the size of the file stored in the metadata on disk.  data=ordered
	 * means the in-memory i_size might be larger than the size on disk
	 * because not all the blocks are written yet.
	 */
	u64 disk_i_size;

<<<<<<< HEAD
<<<<<<< HEAD
	/* flags field from the on disk inode */
	u32 flags;

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * if this is a directory then index_cnt is the counter for the index
	 * number for new files that are created
	 */
	u64 index_cnt;

	/* the fsync log has some corner cases that mean we have to check
	 * directories to see if any unlinks have been done before
	 * the directory was logged.  See tree-log.c for all the
	 * details
	 */
	u64 last_unlink_trans;

	/*
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	 * Number of bytes outstanding that are going to need csums.  This is
	 * used in ENOSPC accounting.
	 */
	u64 csum_bytes;

	/* flags field from the on disk inode */
	u32 flags;

<<<<<<< HEAD
	/*
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* a local copy of root's last_log_commit */
	unsigned long last_log_commit;

	/*
>>>>>>> refs/remotes/origin/master
	 * Counters to keep track of the number of extent item's we may use due
	 * to delalloc and such.  outstanding_extents is the number of extent
	 * items we think we'll end up using, and reserved_extents is the number
	 * of extent items we've reserved metadata for.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	atomic_t outstanding_extents;
	atomic_t reserved_extents;
=======
	unsigned outstanding_extents;
	unsigned reserved_extents;
>>>>>>> refs/remotes/origin/cm-10.0

	/*
	 * ordered_data_close is set by truncate when a file that used
	 * to have good data has been truncated to zero.  When it is set
	 * the btrfs file release call will add this inode to the
	 * ordered operations list so that we make sure to flush out any
	 * new data the application may have written before commit.
<<<<<<< HEAD
	 *
	 * yes, its silly to have a single bitflag, but we might grow more
	 * of these.
=======
>>>>>>> refs/remotes/origin/cm-10.0
	 */
	unsigned ordered_data_close:1;
	unsigned orphan_meta_reserved:1;
	unsigned dummy_inode:1;
	unsigned in_defrag:1;
<<<<<<< HEAD
=======
	unsigned delalloc_meta_reserved:1;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned outstanding_extents;
	unsigned reserved_extents;
>>>>>>> refs/remotes/origin/master

	/*
	 * always compress this one file
	 */
<<<<<<< HEAD
	unsigned force_compress:4;
=======
	unsigned force_compress;
>>>>>>> refs/remotes/origin/master

	struct btrfs_delayed_node *delayed_node;

	struct inode vfs_inode;
};

extern unsigned char btrfs_filetype_table[];

static inline struct btrfs_inode *BTRFS_I(struct inode *inode)
{
	return container_of(inode, struct btrfs_inode, vfs_inode);
}

<<<<<<< HEAD
=======
static inline unsigned long btrfs_inode_hash(u64 objectid,
					     const struct btrfs_root *root)
{
	u64 h = objectid ^ (root->objectid * GOLDEN_RATIO_PRIME);

#if BITS_PER_LONG == 32
	h = (h >> 32) ^ (h & 0xffffffff);
#endif

	return (unsigned long)h;
}

static inline void btrfs_insert_inode_hash(struct inode *inode)
{
	unsigned long h = btrfs_inode_hash(inode->i_ino, BTRFS_I(inode)->root);

	__insert_inode_hash(inode, h);
}

>>>>>>> refs/remotes/origin/master
static inline u64 btrfs_ino(struct inode *inode)
{
	u64 ino = BTRFS_I(inode)->location.objectid;

<<<<<<< HEAD
<<<<<<< HEAD
	if (ino <= BTRFS_FIRST_FREE_OBJECTID)
=======
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * !ino: btree_inode
	 * type == BTRFS_ROOT_ITEM_KEY: subvol dir
	 */
	if (!ino || BTRFS_I(inode)->location.type == BTRFS_ROOT_ITEM_KEY)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		ino = inode->i_ino;
	return ino;
}

static inline void btrfs_i_size_write(struct inode *inode, u64 size)
{
	i_size_write(inode, size);
	BTRFS_I(inode)->disk_i_size = size;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
static inline bool btrfs_is_free_space_inode(struct btrfs_root *root,
				       struct inode *inode)
{
	if (root == root->fs_info->tree_root ||
	    BTRFS_I(inode)->location.objectid == BTRFS_FREE_INO_OBJECTID)
=======
static inline bool btrfs_is_free_space_inode(struct inode *inode)
{
	struct btrfs_root *root = BTRFS_I(inode)->root;

	if (root == root->fs_info->tree_root &&
	    btrfs_ino(inode) != BTRFS_BTREE_INODE_OBJECTID)
		return true;
	if (BTRFS_I(inode)->location.objectid == BTRFS_FREE_INO_OBJECTID)
>>>>>>> refs/remotes/origin/master
		return true;
	return false;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline int btrfs_inode_in_log(struct inode *inode, u64 generation)
{
	if (BTRFS_I(inode)->logged_trans == generation &&
	    BTRFS_I(inode)->last_sub_trans <=
	    BTRFS_I(inode)->last_log_commit &&
	    BTRFS_I(inode)->last_sub_trans <=
	    BTRFS_I(inode)->root->last_log_commit)
		return 1;
	return 0;
}

struct btrfs_dio_private {
	struct inode *inode;
	u64 logical_offset;
	u64 disk_bytenr;
	u64 bytes;
	void *private;

	/* number of bios pending for this dio */
	atomic_t pending_bios;

	/* IO errors */
	int errors;

	/* orig_bio is our btrfs_io_bio */
	struct bio *orig_bio;

	/* dio_bio came from fs/direct-io.c */
	struct bio *dio_bio;
	u8 csum[0];
};

/*
 * Disable DIO read nolock optimization, so new dio readers will be forced
 * to grab i_mutex. It is used to avoid the endless truncate due to
 * nonlocked dio read.
 */
static inline void btrfs_inode_block_unlocked_dio(struct inode *inode)
{
	set_bit(BTRFS_INODE_READDIO_NEED_LOCK, &BTRFS_I(inode)->runtime_flags);
	smp_mb();
}

static inline void btrfs_inode_resume_unlocked_dio(struct inode *inode)
{
	smp_mb__before_clear_bit();
	clear_bit(BTRFS_INODE_READDIO_NEED_LOCK,
		  &BTRFS_I(inode)->runtime_flags);
}

>>>>>>> refs/remotes/origin/master
#endif
