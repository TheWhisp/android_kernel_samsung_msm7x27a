/*
 * Copyright (C) 2008 Oracle.  All rights reserved.
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
#ifndef __DELAYED_REF__
#define __DELAYED_REF__

<<<<<<< HEAD
/* these are the possible values of struct btrfs_delayed_ref->action */
=======
/* these are the possible values of struct btrfs_delayed_ref_node->action */
>>>>>>> refs/remotes/origin/master
#define BTRFS_ADD_DELAYED_REF    1 /* add one backref to the tree */
#define BTRFS_DROP_DELAYED_REF   2 /* delete one backref from the tree */
#define BTRFS_ADD_DELAYED_EXTENT 3 /* record a full extent allocation */
#define BTRFS_UPDATE_DELAYED_HEAD 4 /* not changing ref count on head ref */

struct btrfs_delayed_ref_node {
	struct rb_node rb_node;

	/* the starting bytenr of the extent */
	u64 bytenr;

	/* the size of the extent */
	u64 num_bytes;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	/* seq number to keep track of insertion order */
	u64 seq;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* seq number to keep track of insertion order */
	u64 seq;

>>>>>>> refs/remotes/origin/master
	/* ref count on this data structure */
	atomic_t refs;

	/*
	 * how many refs is this entry adding or deleting.  For
	 * head refs, this may be a negative number because it is keeping
	 * track of the total mods done to the reference count.
	 * For individual refs, this will always be a positive number
	 *
	 * It may be more than one, since it is possible for a single
	 * parent to have more than one ref on an extent
	 */
	int ref_mod;

	unsigned int action:8;
	unsigned int type:8;
	/* is this node still in the rbtree? */
	unsigned int is_head:1;
	unsigned int in_tree:1;
};

struct btrfs_delayed_extent_op {
	struct btrfs_disk_key key;
	u64 flags_to_set;
<<<<<<< HEAD
=======
	int level;
>>>>>>> refs/remotes/origin/master
	unsigned int update_key:1;
	unsigned int update_flags:1;
	unsigned int is_data:1;
};

/*
 * the head refs are used to hold a lock on a given extent, which allows us
 * to make sure that only one process is running the delayed refs
 * at a time for a single extent.  They also store the sum of all the
 * reference count modifications we've queued up.
 */
struct btrfs_delayed_ref_head {
	struct btrfs_delayed_ref_node node;

	/*
	 * the mutex is held while running the refs, and it is also
	 * held when checking the sum of reference modifications.
	 */
	struct mutex mutex;

	struct list_head cluster;

	struct btrfs_delayed_extent_op *extent_op;
	/*
	 * when a new extent is allocated, it is just reserved in memory
	 * The actual extent isn't inserted into the extent allocation tree
	 * until the delayed ref is processed.  must_insert_reserved is
	 * used to flag a delayed ref so the accounting can be updated
	 * when a full insert is done.
	 *
	 * It is possible the extent will be freed before it is ever
	 * inserted into the extent allocation tree.  In this case
	 * we need to update the in ram accounting to properly reflect
	 * the free has happened.
	 */
	unsigned int must_insert_reserved:1;
	unsigned int is_data:1;
};

struct btrfs_delayed_tree_ref {
	struct btrfs_delayed_ref_node node;
<<<<<<< HEAD
<<<<<<< HEAD
	union {
		u64 root;
		u64 parent;
	};
=======
	u64 root;
	u64 parent;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u64 root;
	u64 parent;
>>>>>>> refs/remotes/origin/master
	int level;
};

struct btrfs_delayed_data_ref {
	struct btrfs_delayed_ref_node node;
<<<<<<< HEAD
<<<<<<< HEAD
	union {
		u64 root;
		u64 parent;
	};
=======
	u64 root;
	u64 parent;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u64 root;
	u64 parent;
>>>>>>> refs/remotes/origin/master
	u64 objectid;
	u64 offset;
};

struct btrfs_delayed_ref_root {
	struct rb_root root;

	/* this spin lock protects the rbtree and the entries inside */
	spinlock_t lock;

	/* how many delayed ref updates we've queued, used by the
	 * throttling code
	 */
	unsigned long num_entries;

	/* total number of head nodes in tree */
	unsigned long num_heads;

	/* total number of head nodes ready for processing */
	unsigned long num_heads_ready;

	/*
<<<<<<< HEAD
=======
	 * bumped when someone is making progress on the delayed
	 * refs, so that other procs know they are just adding to
	 * contention intead of helping
	 */
	atomic_t procs_running_refs;
	atomic_t ref_seq;
	wait_queue_head_t wait;

	/*
>>>>>>> refs/remotes/origin/master
	 * set when the tree is flushing before a transaction commit,
	 * used by the throttling code to decide if new updates need
	 * to be run right away
	 */
	int flushing;

	u64 run_delayed_start;
<<<<<<< HEAD
<<<<<<< HEAD
=======

	/*
	 * seq number of delayed refs. We need to know if a backref was being
	 * added before the currently processed ref or afterwards.
	 */
	u64 seq;

	/*
	 * seq_list holds a list of all seq numbers that are currently being
	 * added to the list. While walking backrefs (btrfs_find_all_roots,
	 * qgroups), which might take some time, no newer ref must be processed,
	 * as it might influence the outcome of the walk.
	 */
	struct list_head seq_head;

	/*
	 * when the only refs we have in the list must not be processed, we want
	 * to wait for more refs to show up or for the end of backref walking.
	 */
	wait_queue_head_t seq_wait;
>>>>>>> refs/remotes/origin/cm-10.0
};
=======
};

extern struct kmem_cache *btrfs_delayed_ref_head_cachep;
extern struct kmem_cache *btrfs_delayed_tree_ref_cachep;
extern struct kmem_cache *btrfs_delayed_data_ref_cachep;
extern struct kmem_cache *btrfs_delayed_extent_op_cachep;

int btrfs_delayed_ref_init(void);
void btrfs_delayed_ref_exit(void);

static inline struct btrfs_delayed_extent_op *
btrfs_alloc_delayed_extent_op(void)
{
	return kmem_cache_alloc(btrfs_delayed_extent_op_cachep, GFP_NOFS);
}

static inline void
btrfs_free_delayed_extent_op(struct btrfs_delayed_extent_op *op)
{
	if (op)
		kmem_cache_free(btrfs_delayed_extent_op_cachep, op);
}
>>>>>>> refs/remotes/origin/master

static inline void btrfs_put_delayed_ref(struct btrfs_delayed_ref_node *ref)
{
	WARN_ON(atomic_read(&ref->refs) == 0);
	if (atomic_dec_and_test(&ref->refs)) {
		WARN_ON(ref->in_tree);
<<<<<<< HEAD
		kfree(ref);
	}
}

<<<<<<< HEAD
int btrfs_add_delayed_tree_ref(struct btrfs_trans_handle *trans,
			       u64 bytenr, u64 num_bytes, u64 parent,
			       u64 ref_root, int level, int action,
			       struct btrfs_delayed_extent_op *extent_op);
int btrfs_add_delayed_data_ref(struct btrfs_trans_handle *trans,
			       u64 bytenr, u64 num_bytes,
			       u64 parent, u64 ref_root,
			       u64 owner, u64 offset, int action,
			       struct btrfs_delayed_extent_op *extent_op);
int btrfs_add_delayed_extent_op(struct btrfs_trans_handle *trans,
=======
=======
		switch (ref->type) {
		case BTRFS_TREE_BLOCK_REF_KEY:
		case BTRFS_SHARED_BLOCK_REF_KEY:
			kmem_cache_free(btrfs_delayed_tree_ref_cachep, ref);
			break;
		case BTRFS_EXTENT_DATA_REF_KEY:
		case BTRFS_SHARED_DATA_REF_KEY:
			kmem_cache_free(btrfs_delayed_data_ref_cachep, ref);
			break;
		case 0:
			kmem_cache_free(btrfs_delayed_ref_head_cachep, ref);
			break;
		default:
			BUG();
		}
	}
}

>>>>>>> refs/remotes/origin/master
int btrfs_add_delayed_tree_ref(struct btrfs_fs_info *fs_info,
			       struct btrfs_trans_handle *trans,
			       u64 bytenr, u64 num_bytes, u64 parent,
			       u64 ref_root, int level, int action,
			       struct btrfs_delayed_extent_op *extent_op,
			       int for_cow);
int btrfs_add_delayed_data_ref(struct btrfs_fs_info *fs_info,
			       struct btrfs_trans_handle *trans,
			       u64 bytenr, u64 num_bytes,
			       u64 parent, u64 ref_root,
			       u64 owner, u64 offset, int action,
			       struct btrfs_delayed_extent_op *extent_op,
			       int for_cow);
int btrfs_add_delayed_extent_op(struct btrfs_fs_info *fs_info,
				struct btrfs_trans_handle *trans,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
				u64 bytenr, u64 num_bytes,
				struct btrfs_delayed_extent_op *extent_op);
=======
				u64 bytenr, u64 num_bytes,
				struct btrfs_delayed_extent_op *extent_op);
void btrfs_merge_delayed_refs(struct btrfs_trans_handle *trans,
			      struct btrfs_fs_info *fs_info,
			      struct btrfs_delayed_ref_root *delayed_refs,
			      struct btrfs_delayed_ref_head *head);
>>>>>>> refs/remotes/origin/master

struct btrfs_delayed_ref_head *
btrfs_find_delayed_ref_head(struct btrfs_trans_handle *trans, u64 bytenr);
int btrfs_delayed_ref_lock(struct btrfs_trans_handle *trans,
			   struct btrfs_delayed_ref_head *head);
<<<<<<< HEAD
int btrfs_find_ref_cluster(struct btrfs_trans_handle *trans,
			   struct list_head *cluster, u64 search_start);
<<<<<<< HEAD
=======

struct seq_list {
	struct list_head list;
	u64 seq;
};

static inline u64 inc_delayed_seq(struct btrfs_delayed_ref_root *delayed_refs)
{
	assert_spin_locked(&delayed_refs->lock);
	++delayed_refs->seq;
	return delayed_refs->seq;
}

static inline void
btrfs_get_delayed_seq(struct btrfs_delayed_ref_root *delayed_refs,
		      struct seq_list *elem)
{
	assert_spin_locked(&delayed_refs->lock);
	elem->seq = delayed_refs->seq;
	list_add_tail(&elem->list, &delayed_refs->seq_head);
}

static inline void
btrfs_put_delayed_seq(struct btrfs_delayed_ref_root *delayed_refs,
		      struct seq_list *elem)
{
	spin_lock(&delayed_refs->lock);
	list_del(&elem->list);
	wake_up(&delayed_refs->seq_wait);
	spin_unlock(&delayed_refs->lock);
}

int btrfs_check_delayed_seq(struct btrfs_delayed_ref_root *delayed_refs,
=======
static inline void btrfs_delayed_ref_unlock(struct btrfs_delayed_ref_head *head)
{
	mutex_unlock(&head->mutex);
}

int btrfs_find_ref_cluster(struct btrfs_trans_handle *trans,
			   struct list_head *cluster, u64 search_start);
void btrfs_release_ref_cluster(struct list_head *cluster);

int btrfs_check_delayed_seq(struct btrfs_fs_info *fs_info,
			    struct btrfs_delayed_ref_root *delayed_refs,
>>>>>>> refs/remotes/origin/master
			    u64 seq);

/*
 * delayed refs with a ref_seq > 0 must be held back during backref walking.
 * this only applies to items in one of the fs-trees. for_cow items never need
 * to be held back, so they won't get a ref_seq number.
 */
static inline int need_ref_seq(int for_cow, u64 rootid)
{
	if (for_cow)
		return 0;

	if (rootid == BTRFS_FS_TREE_OBJECTID)
		return 1;

	if ((s64)rootid >= (s64)BTRFS_FIRST_FREE_OBJECTID)
		return 1;

	return 0;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * a node might live in a head or a regular ref, this lets you
 * test for the proper type to use.
 */
static int btrfs_delayed_ref_is_head(struct btrfs_delayed_ref_node *node)
{
	return node->is_head;
}

/*
 * helper functions to cast a node into its container
 */
static inline struct btrfs_delayed_tree_ref *
btrfs_delayed_node_to_tree_ref(struct btrfs_delayed_ref_node *node)
{
	WARN_ON(btrfs_delayed_ref_is_head(node));
	return container_of(node, struct btrfs_delayed_tree_ref, node);
}

static inline struct btrfs_delayed_data_ref *
btrfs_delayed_node_to_data_ref(struct btrfs_delayed_ref_node *node)
{
	WARN_ON(btrfs_delayed_ref_is_head(node));
	return container_of(node, struct btrfs_delayed_data_ref, node);
}

static inline struct btrfs_delayed_ref_head *
btrfs_delayed_node_to_head(struct btrfs_delayed_ref_node *node)
{
	WARN_ON(!btrfs_delayed_ref_is_head(node));
	return container_of(node, struct btrfs_delayed_ref_head, node);
}
#endif
