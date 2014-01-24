/*
 *  linux/fs/namespace.c
 *
 * (C) Copyright Al Viro 2000, 2001
 *	Released under GPL v2.
 *
 * Based on code from fs/super.c, copyright Linus Torvalds and others.
 * Heavily rewritten.
 */

#include <linux/syscalls.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/percpu.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/acct.h>
#include <linux/capability.h>
#include <linux/cpumask.h>
#include <linux/module.h>
#include <linux/sysfs.h>
#include <linux/seq_file.h>
#include <linux/mnt_namespace.h>
#include <linux/namei.h>
#include <linux/nsproxy.h>
#include <linux/security.h>
#include <linux/mount.h>
#include <linux/ramfs.h>
#include <linux/log2.h>
#include <linux/idr.h>
#include <linux/fs_struct.h>
#include <linux/fsnotify.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>
=======
#include <linux/export.h>
#include <linux/capability.h>
#include <linux/mnt_namespace.h>
=======
#include <linux/export.h>
#include <linux/capability.h>
#include <linux/mnt_namespace.h>
#include <linux/user_namespace.h>
>>>>>>> refs/remotes/origin/master
#include <linux/namei.h>
#include <linux/security.h>
#include <linux/idr.h>
#include <linux/acct.h>		/* acct_auto_close_mnt */
<<<<<<< HEAD
#include <linux/ramfs.h>	/* init_rootfs */
#include <linux/fs_struct.h>	/* get_fs_root et.al. */
#include <linux/fsnotify.h>	/* fsnotify_vfsmount_delete */
#include <linux/uaccess.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/init.h>		/* init_rootfs */
#include <linux/fs_struct.h>	/* get_fs_root et.al. */
#include <linux/fsnotify.h>	/* fsnotify_vfsmount_delete */
#include <linux/uaccess.h>
#include <linux/proc_ns.h>
#include <linux/magic.h>
>>>>>>> refs/remotes/origin/master
#include "pnode.h"
#include "internal.h"

#define HASH_SHIFT ilog2(PAGE_SIZE / sizeof(struct list_head))
#define HASH_SIZE (1UL << HASH_SHIFT)

static int event;
static DEFINE_IDA(mnt_id_ida);
static DEFINE_IDA(mnt_group_ida);
static DEFINE_SPINLOCK(mnt_id_lock);
static int mnt_id_start = 0;
static int mnt_group_start = 1;

static struct list_head *mount_hashtable __read_mostly;
<<<<<<< HEAD
static struct kmem_cache *mnt_cache __read_mostly;
static struct rw_semaphore namespace_sem;
=======
static struct list_head *mountpoint_hashtable __read_mostly;
static struct kmem_cache *mnt_cache __read_mostly;
static DECLARE_RWSEM(namespace_sem);
>>>>>>> refs/remotes/origin/master

/* /sys/fs */
struct kobject *fs_kobj;
EXPORT_SYMBOL_GPL(fs_kobj);

/*
 * vfsmount lock may be taken for read to prevent changes to the
 * vfsmount hash, ie. during mountpoint lookups or walking back
 * up the tree.
 *
 * It should be taken for write in all cases where the vfsmount
 * tree or hash is modified or when a vfsmount structure is modified.
 */
<<<<<<< HEAD
DEFINE_BRLOCK(vfsmount_lock);
=======
__cacheline_aligned_in_smp DEFINE_SEQLOCK(mount_lock);
>>>>>>> refs/remotes/origin/master

static inline unsigned long hash(struct vfsmount *mnt, struct dentry *dentry)
{
	unsigned long tmp = ((unsigned long)mnt / L1_CACHE_BYTES);
	tmp += ((unsigned long)dentry / L1_CACHE_BYTES);
	tmp = tmp + (tmp >> HASH_SHIFT);
	return tmp & (HASH_SIZE - 1);
}

<<<<<<< HEAD
#define MNT_WRITER_UNDERFLOW_LIMIT -(1<<16)

=======
>>>>>>> refs/remotes/origin/master
/*
 * allocation is serialized by namespace_sem, but we need the spinlock to
 * serialize with freeing.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int mnt_alloc_id(struct vfsmount *mnt)
=======
static int mnt_alloc_id(struct mount *mnt)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int mnt_alloc_id(struct mount *mnt)
>>>>>>> refs/remotes/origin/master
{
	int res;

retry:
	ida_pre_get(&mnt_id_ida, GFP_KERNEL);
	spin_lock(&mnt_id_lock);
	res = ida_get_new_above(&mnt_id_ida, mnt_id_start, &mnt->mnt_id);
	if (!res)
		mnt_id_start = mnt->mnt_id + 1;
	spin_unlock(&mnt_id_lock);
	if (res == -EAGAIN)
		goto retry;

	return res;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void mnt_free_id(struct vfsmount *mnt)
=======
static void mnt_free_id(struct mount *mnt)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void mnt_free_id(struct mount *mnt)
>>>>>>> refs/remotes/origin/master
{
	int id = mnt->mnt_id;
	spin_lock(&mnt_id_lock);
	ida_remove(&mnt_id_ida, id);
	if (mnt_id_start > id)
		mnt_id_start = id;
	spin_unlock(&mnt_id_lock);
}

/*
 * Allocate a new peer group ID
 *
 * mnt_group_ida is protected by namespace_sem
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int mnt_alloc_group_id(struct vfsmount *mnt)
=======
static int mnt_alloc_group_id(struct mount *mnt)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int mnt_alloc_group_id(struct mount *mnt)
>>>>>>> refs/remotes/origin/master
{
	int res;

	if (!ida_pre_get(&mnt_group_ida, GFP_KERNEL))
		return -ENOMEM;

	res = ida_get_new_above(&mnt_group_ida,
				mnt_group_start,
				&mnt->mnt_group_id);
	if (!res)
		mnt_group_start = mnt->mnt_group_id + 1;

	return res;
}

/*
 * Release a peer group ID
 */
<<<<<<< HEAD
<<<<<<< HEAD
void mnt_release_group_id(struct vfsmount *mnt)
=======
void mnt_release_group_id(struct mount *mnt)
>>>>>>> refs/remotes/origin/cm-10.0
=======
void mnt_release_group_id(struct mount *mnt)
>>>>>>> refs/remotes/origin/master
{
	int id = mnt->mnt_group_id;
	ida_remove(&mnt_group_ida, id);
	if (mnt_group_start > id)
		mnt_group_start = id;
	mnt->mnt_group_id = 0;
}

/*
 * vfsmount lock must be held for read
 */
<<<<<<< HEAD
<<<<<<< HEAD
static inline void mnt_add_count(struct vfsmount *mnt, int n)
=======
static inline void mnt_add_count(struct mount *mnt, int n)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline void mnt_add_count(struct mount *mnt, int n)
>>>>>>> refs/remotes/origin/master
{
#ifdef CONFIG_SMP
	this_cpu_add(mnt->mnt_pcp->mnt_count, n);
#else
	preempt_disable();
	mnt->mnt_count += n;
	preempt_enable();
#endif
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline void mnt_set_count(struct vfsmount *mnt, int n)
{
#ifdef CONFIG_SMP
	this_cpu_write(mnt->mnt_pcp->mnt_count, n);
#else
	mnt->mnt_count = n;
#endif
}

/*
 * vfsmount lock must be held for read
 */
static inline void mnt_inc_count(struct vfsmount *mnt)
{
	mnt_add_count(mnt, 1);
}

/*
 * vfsmount lock must be held for read
 */
static inline void mnt_dec_count(struct vfsmount *mnt)
{
	mnt_add_count(mnt, -1);
}

/*
 * vfsmount lock must be held for write
 */
unsigned int mnt_get_count(struct vfsmount *mnt)
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * vfsmount lock must be held for write
 */
unsigned int mnt_get_count(struct mount *mnt)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
#ifdef CONFIG_SMP
	unsigned int count = 0;
	int cpu;

	for_each_possible_cpu(cpu) {
		count += per_cpu_ptr(mnt->mnt_pcp, cpu)->mnt_count;
	}

	return count;
#else
	return mnt->mnt_count;
#endif
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct vfsmount *alloc_vfsmnt(const char *name)
{
	struct vfsmount *mnt = kmem_cache_zalloc(mnt_cache, GFP_KERNEL);
=======
static struct mount *alloc_vfsmnt(const char *name)
{
	struct mount *mnt = kmem_cache_zalloc(mnt_cache, GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct mount *alloc_vfsmnt(const char *name)
{
	struct mount *mnt = kmem_cache_zalloc(mnt_cache, GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (mnt) {
		int err;

		err = mnt_alloc_id(mnt);
		if (err)
			goto out_free_cache;

		if (name) {
			mnt->mnt_devname = kstrdup(name, GFP_KERNEL);
			if (!mnt->mnt_devname)
				goto out_free_id;
		}

#ifdef CONFIG_SMP
		mnt->mnt_pcp = alloc_percpu(struct mnt_pcp);
		if (!mnt->mnt_pcp)
			goto out_free_devname;

		this_cpu_add(mnt->mnt_pcp->mnt_count, 1);
#else
		mnt->mnt_count = 1;
		mnt->mnt_writers = 0;
#endif

		INIT_LIST_HEAD(&mnt->mnt_hash);
		INIT_LIST_HEAD(&mnt->mnt_child);
		INIT_LIST_HEAD(&mnt->mnt_mounts);
		INIT_LIST_HEAD(&mnt->mnt_list);
		INIT_LIST_HEAD(&mnt->mnt_expire);
		INIT_LIST_HEAD(&mnt->mnt_share);
		INIT_LIST_HEAD(&mnt->mnt_slave_list);
		INIT_LIST_HEAD(&mnt->mnt_slave);
#ifdef CONFIG_FSNOTIFY
		INIT_HLIST_HEAD(&mnt->mnt_fsnotify_marks);
#endif
	}
	return mnt;

#ifdef CONFIG_SMP
out_free_devname:
	kfree(mnt->mnt_devname);
#endif
out_free_id:
	mnt_free_id(mnt);
out_free_cache:
	kmem_cache_free(mnt_cache, mnt);
	return NULL;
}

/*
 * Most r/o checks on a fs are for operations that take
 * discrete amounts of time, like a write() or unlink().
 * We must keep track of when those operations start
 * (for permission checks) and when they end, so that
 * we can determine when writes are able to occur to
 * a filesystem.
 */
/*
 * __mnt_is_readonly: check whether a mount is read-only
 * @mnt: the mount to check for its write status
 *
 * This shouldn't be used directly ouside of the VFS.
 * It does not guarantee that the filesystem will stay
 * r/w, just that it is right *now*.  This can not and
 * should not be used in place of IS_RDONLY(inode).
 * mnt_want/drop_write() will _keep_ the filesystem
 * r/w.
 */
int __mnt_is_readonly(struct vfsmount *mnt)
{
	if (mnt->mnt_flags & MNT_READONLY)
		return 1;
	if (mnt->mnt_sb->s_flags & MS_RDONLY)
		return 1;
	return 0;
}
EXPORT_SYMBOL_GPL(__mnt_is_readonly);

<<<<<<< HEAD
<<<<<<< HEAD
static inline void mnt_inc_writers(struct vfsmount *mnt)
=======
static inline void mnt_inc_writers(struct mount *mnt)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline void mnt_inc_writers(struct mount *mnt)
>>>>>>> refs/remotes/origin/master
{
#ifdef CONFIG_SMP
	this_cpu_inc(mnt->mnt_pcp->mnt_writers);
#else
	mnt->mnt_writers++;
#endif
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline void mnt_dec_writers(struct vfsmount *mnt)
=======
static inline void mnt_dec_writers(struct mount *mnt)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline void mnt_dec_writers(struct mount *mnt)
>>>>>>> refs/remotes/origin/master
{
#ifdef CONFIG_SMP
	this_cpu_dec(mnt->mnt_pcp->mnt_writers);
#else
	mnt->mnt_writers--;
#endif
}

<<<<<<< HEAD
<<<<<<< HEAD
static unsigned int mnt_get_writers(struct vfsmount *mnt)
=======
static unsigned int mnt_get_writers(struct mount *mnt)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static unsigned int mnt_get_writers(struct mount *mnt)
>>>>>>> refs/remotes/origin/master
{
#ifdef CONFIG_SMP
	unsigned int count = 0;
	int cpu;

	for_each_possible_cpu(cpu) {
		count += per_cpu_ptr(mnt->mnt_pcp, cpu)->mnt_writers;
	}

	return count;
#else
	return mnt->mnt_writers;
#endif
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static int mnt_is_readonly(struct vfsmount *mnt)
{
	if (mnt->mnt_sb->s_readonly_remount)
		return 1;
	/* Order wrt setting s_flags/s_readonly_remount in do_remount() */
	smp_rmb();
	return __mnt_is_readonly(mnt);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
/*
 * Most r/o checks on a fs are for operations that take
 * discrete amounts of time, like a write() or unlink().
 * We must keep track of when those operations start
 * (for permission checks) and when they end, so that
 * we can determine when writes are able to occur to
 * a filesystem.
 */
/**
 * mnt_want_write - get write access to a mount
<<<<<<< HEAD
 * @mnt: the mount on which to take a write
=======
 * @m: the mount on which to take a write
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * This tells the low-level filesystem that a write is
 * about to be performed to it, and makes sure that
 * writes are allowed before returning success.  When
 * the write operation is finished, mnt_drop_write()
 * must be called.  This is effectively a refcount.
 */
<<<<<<< HEAD
int mnt_want_write(struct vfsmount *mnt)
{
=======
int mnt_want_write(struct vfsmount *m)
{
	struct mount *mnt = real_mount(m);
>>>>>>> refs/remotes/origin/cm-10.0
=======
/*
 * Most r/o & frozen checks on a fs are for operations that take discrete
 * amounts of time, like a write() or unlink().  We must keep track of when
 * those operations start (for permission checks) and when they end, so that we
 * can determine when writes are able to occur to a filesystem.
 */
/**
 * __mnt_want_write - get write access to a mount without freeze protection
 * @m: the mount on which to take a write
 *
 * This tells the low-level filesystem that a write is about to be performed to
 * it, and makes sure that writes are allowed (mnt it read-write) before
 * returning success. This operation does not protect against filesystem being
 * frozen. When the write operation is finished, __mnt_drop_write() must be
 * called. This is effectively a refcount.
 */
int __mnt_want_write(struct vfsmount *m)
{
	struct mount *mnt = real_mount(m);
>>>>>>> refs/remotes/origin/master
	int ret = 0;

	preempt_disable();
	mnt_inc_writers(mnt);
	/*
	 * The store to mnt_inc_writers must be visible before we pass
	 * MNT_WRITE_HOLD loop below, so that the slowpath can see our
	 * incremented count after it has set MNT_WRITE_HOLD.
	 */
	smp_mb();
<<<<<<< HEAD
<<<<<<< HEAD
	while (mnt->mnt_flags & MNT_WRITE_HOLD)
=======
	while (mnt->mnt.mnt_flags & MNT_WRITE_HOLD)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	while (ACCESS_ONCE(mnt->mnt.mnt_flags) & MNT_WRITE_HOLD)
>>>>>>> refs/remotes/origin/master
		cpu_relax();
	/*
	 * After the slowpath clears MNT_WRITE_HOLD, mnt_is_readonly will
	 * be set to match its requirements. So we must not load that until
	 * MNT_WRITE_HOLD is cleared.
	 */
	smp_rmb();
<<<<<<< HEAD
<<<<<<< HEAD
	if (__mnt_is_readonly(mnt)) {
		mnt_dec_writers(mnt);
		ret = -EROFS;
		goto out;
	}
out:
=======
=======
>>>>>>> refs/remotes/origin/master
	if (mnt_is_readonly(m)) {
		mnt_dec_writers(mnt);
		ret = -EROFS;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	preempt_enable();
=======
	preempt_enable();

	return ret;
}

/**
 * mnt_want_write - get write access to a mount
 * @m: the mount on which to take a write
 *
 * This tells the low-level filesystem that a write is about to be performed to
 * it, and makes sure that writes are allowed (mount is read-write, filesystem
 * is not frozen) before returning success.  When the write operation is
 * finished, mnt_drop_write() must be called.  This is effectively a refcount.
 */
int mnt_want_write(struct vfsmount *m)
{
	int ret;

	sb_start_write(m->mnt_sb);
	ret = __mnt_want_write(m);
	if (ret)
		sb_end_write(m->mnt_sb);
>>>>>>> refs/remotes/origin/master
	return ret;
}
EXPORT_SYMBOL_GPL(mnt_want_write);

/**
 * mnt_clone_write - get write access to a mount
 * @mnt: the mount on which to take a write
 *
 * This is effectively like mnt_want_write, except
 * it must only be used to take an extra write reference
 * on a mountpoint that we already know has a write reference
 * on it. This allows some optimisation.
 *
 * After finished, mnt_drop_write must be called as usual to
 * drop the reference.
 */
int mnt_clone_write(struct vfsmount *mnt)
{
	/* superblock may be r/o */
	if (__mnt_is_readonly(mnt))
		return -EROFS;
	preempt_disable();
<<<<<<< HEAD
<<<<<<< HEAD
	mnt_inc_writers(mnt);
=======
	mnt_inc_writers(real_mount(mnt));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	mnt_inc_writers(real_mount(mnt));
>>>>>>> refs/remotes/origin/master
	preempt_enable();
	return 0;
}
EXPORT_SYMBOL_GPL(mnt_clone_write);

/**
<<<<<<< HEAD
 * mnt_want_write_file - get write access to a file's mount
 * @file: the file who's mount on which to take a write
 *
 * This is like mnt_want_write, but it takes a file and can
 * do some optimisations if the file is open for write already
 */
int mnt_want_write_file(struct file *file)
{
	struct inode *inode = file->f_dentry->d_inode;
	if (!(file->f_mode & FMODE_WRITE) || special_file(inode->i_mode))
		return mnt_want_write(file->f_path.mnt);
	else
		return mnt_clone_write(file->f_path.mnt);
}
EXPORT_SYMBOL_GPL(mnt_want_write_file);

/**
 * mnt_drop_write - give up write access to a mount
=======
 * __mnt_want_write_file - get write access to a file's mount
 * @file: the file who's mount on which to take a write
 *
 * This is like __mnt_want_write, but it takes a file and can
 * do some optimisations if the file is open for write already
 */
int __mnt_want_write_file(struct file *file)
{
	struct inode *inode = file_inode(file);

	if (!(file->f_mode & FMODE_WRITE) || special_file(inode->i_mode))
		return __mnt_want_write(file->f_path.mnt);
	else
		return mnt_clone_write(file->f_path.mnt);
}

/**
 * mnt_want_write_file - get write access to a file's mount
 * @file: the file who's mount on which to take a write
 *
 * This is like mnt_want_write, but it takes a file and can
 * do some optimisations if the file is open for write already
 */
int mnt_want_write_file(struct file *file)
{
	int ret;

	sb_start_write(file->f_path.mnt->mnt_sb);
	ret = __mnt_want_write_file(file);
	if (ret)
		sb_end_write(file->f_path.mnt->mnt_sb);
	return ret;
}
EXPORT_SYMBOL_GPL(mnt_want_write_file);

/**
 * __mnt_drop_write - give up write access to a mount
>>>>>>> refs/remotes/origin/master
 * @mnt: the mount on which to give up write access
 *
 * Tells the low-level filesystem that we are done
 * performing writes to it.  Must be matched with
<<<<<<< HEAD
 * mnt_want_write() call above.
 */
void mnt_drop_write(struct vfsmount *mnt)
{
	preempt_disable();
<<<<<<< HEAD
	mnt_dec_writers(mnt);
=======
	mnt_dec_writers(real_mount(mnt));
>>>>>>> refs/remotes/origin/cm-10.0
	preempt_enable();
}
EXPORT_SYMBOL_GPL(mnt_drop_write);

<<<<<<< HEAD
static int mnt_make_readonly(struct vfsmount *mnt)
=======
=======
 * __mnt_want_write() call above.
 */
void __mnt_drop_write(struct vfsmount *mnt)
{
	preempt_disable();
	mnt_dec_writers(real_mount(mnt));
	preempt_enable();
}

/**
 * mnt_drop_write - give up write access to a mount
 * @mnt: the mount on which to give up write access
 *
 * Tells the low-level filesystem that we are done performing writes to it and
 * also allows filesystem to be frozen again.  Must be matched with
 * mnt_want_write() call above.
 */
void mnt_drop_write(struct vfsmount *mnt)
{
	__mnt_drop_write(mnt);
	sb_end_write(mnt->mnt_sb);
}
EXPORT_SYMBOL_GPL(mnt_drop_write);

void __mnt_drop_write_file(struct file *file)
{
	__mnt_drop_write(file->f_path.mnt);
}

>>>>>>> refs/remotes/origin/master
void mnt_drop_write_file(struct file *file)
{
	mnt_drop_write(file->f_path.mnt);
}
EXPORT_SYMBOL(mnt_drop_write_file);

static int mnt_make_readonly(struct mount *mnt)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
{
	int ret = 0;

	br_write_lock(vfsmount_lock);
<<<<<<< HEAD
	mnt->mnt_flags |= MNT_WRITE_HOLD;
=======
	mnt->mnt.mnt_flags |= MNT_WRITE_HOLD;
>>>>>>> refs/remotes/origin/cm-10.0
=======
{
	int ret = 0;

	lock_mount_hash();
	mnt->mnt.mnt_flags |= MNT_WRITE_HOLD;
>>>>>>> refs/remotes/origin/master
	/*
	 * After storing MNT_WRITE_HOLD, we'll read the counters. This store
	 * should be visible before we do.
	 */
	smp_mb();

	/*
	 * With writers on hold, if this value is zero, then there are
	 * definitely no active writers (although held writers may subsequently
	 * increment the count, they'll have to wait, and decrement it after
	 * seeing MNT_READONLY).
	 *
	 * It is OK to have counter incremented on one CPU and decremented on
	 * another: the sum will add up correctly. The danger would be when we
	 * sum up each counter, if we read a counter before it is incremented,
	 * but then read another CPU's count which it has been subsequently
	 * decremented from -- we would see more decrements than we should.
	 * MNT_WRITE_HOLD protects against this scenario, because
	 * mnt_want_write first increments count, then smp_mb, then spins on
	 * MNT_WRITE_HOLD, so it can't be decremented by another CPU while
	 * we're counting up here.
	 */
	if (mnt_get_writers(mnt) > 0)
		ret = -EBUSY;
	else
<<<<<<< HEAD
<<<<<<< HEAD
		mnt->mnt_flags |= MNT_READONLY;
=======
		mnt->mnt.mnt_flags |= MNT_READONLY;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		mnt->mnt.mnt_flags |= MNT_READONLY;
>>>>>>> refs/remotes/origin/master
	/*
	 * MNT_READONLY must become visible before ~MNT_WRITE_HOLD, so writers
	 * that become unheld will see MNT_READONLY.
	 */
	smp_wmb();
<<<<<<< HEAD
<<<<<<< HEAD
	mnt->mnt_flags &= ~MNT_WRITE_HOLD;
=======
	mnt->mnt.mnt_flags &= ~MNT_WRITE_HOLD;
>>>>>>> refs/remotes/origin/cm-10.0
	br_write_unlock(vfsmount_lock);
	return ret;
}

<<<<<<< HEAD
static void __mnt_unmake_readonly(struct vfsmount *mnt)
{
	br_write_lock(vfsmount_lock);
	mnt->mnt_flags &= ~MNT_READONLY;
	br_write_unlock(vfsmount_lock);
}

static void free_vfsmnt(struct vfsmount *mnt)
=======
static void __mnt_unmake_readonly(struct mount *mnt)
{
	br_write_lock(vfsmount_lock);
	mnt->mnt.mnt_flags &= ~MNT_READONLY;
	br_write_unlock(vfsmount_lock);
=======
	mnt->mnt.mnt_flags &= ~MNT_WRITE_HOLD;
	unlock_mount_hash();
	return ret;
}

static void __mnt_unmake_readonly(struct mount *mnt)
{
	lock_mount_hash();
	mnt->mnt.mnt_flags &= ~MNT_READONLY;
	unlock_mount_hash();
>>>>>>> refs/remotes/origin/master
}

int sb_prepare_remount_readonly(struct super_block *sb)
{
	struct mount *mnt;
	int err = 0;

	/* Racy optimization.  Recheck the counter under MNT_WRITE_HOLD */
	if (atomic_long_read(&sb->s_remove_count))
		return -EBUSY;

<<<<<<< HEAD
	br_write_lock(vfsmount_lock);
=======
	lock_mount_hash();
>>>>>>> refs/remotes/origin/master
	list_for_each_entry(mnt, &sb->s_mounts, mnt_instance) {
		if (!(mnt->mnt.mnt_flags & MNT_READONLY)) {
			mnt->mnt.mnt_flags |= MNT_WRITE_HOLD;
			smp_mb();
			if (mnt_get_writers(mnt) > 0) {
				err = -EBUSY;
				break;
			}
		}
	}
	if (!err && atomic_long_read(&sb->s_remove_count))
		err = -EBUSY;

	if (!err) {
		sb->s_readonly_remount = 1;
		smp_wmb();
	}
	list_for_each_entry(mnt, &sb->s_mounts, mnt_instance) {
		if (mnt->mnt.mnt_flags & MNT_WRITE_HOLD)
			mnt->mnt.mnt_flags &= ~MNT_WRITE_HOLD;
	}
<<<<<<< HEAD
	br_write_unlock(vfsmount_lock);
=======
	unlock_mount_hash();
>>>>>>> refs/remotes/origin/master

	return err;
}

static void free_vfsmnt(struct mount *mnt)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	kfree(mnt->mnt_devname);
	mnt_free_id(mnt);
#ifdef CONFIG_SMP
	free_percpu(mnt->mnt_pcp);
#endif
	kmem_cache_free(mnt_cache, mnt);
}

<<<<<<< HEAD
/*
 * find the first or last mount at @dentry on vfsmount @mnt depending on
 * @dir. If @dir is set return the first mount else return the last mount.
 * vfsmount_lock must be held for read or write.
 */
<<<<<<< HEAD
struct vfsmount *__lookup_mnt(struct vfsmount *mnt, struct dentry *dentry,
=======
struct mount *__lookup_mnt(struct vfsmount *mnt, struct dentry *dentry,
>>>>>>> refs/remotes/origin/cm-10.0
			      int dir)
{
	struct list_head *head = mount_hashtable + hash(mnt, dentry);
	struct list_head *tmp = head;
<<<<<<< HEAD
	struct vfsmount *p, *found = NULL;
=======
	struct mount *p, *found = NULL;
>>>>>>> refs/remotes/origin/cm-10.0

	for (;;) {
		tmp = dir ? tmp->next : tmp->prev;
		p = NULL;
		if (tmp == head)
			break;
<<<<<<< HEAD
		p = list_entry(tmp, struct vfsmount, mnt_hash);
		if (p->mnt_parent == mnt && p->mnt_mountpoint == dentry) {
=======
		p = list_entry(tmp, struct mount, mnt_hash);
		if (&p->mnt_parent->mnt == mnt && p->mnt_mountpoint == dentry) {
>>>>>>> refs/remotes/origin/cm-10.0
			found = p;
			break;
		}
	}
	return found;
}

/*
 * lookup_mnt increments the ref count before returning
 * the vfsmount struct.
 */
struct vfsmount *lookup_mnt(struct path *path)
{
<<<<<<< HEAD
	struct vfsmount *child_mnt;

	br_read_lock(vfsmount_lock);
	if ((child_mnt = __lookup_mnt(path->mnt, path->dentry, 1)))
		mntget(child_mnt);
	br_read_unlock(vfsmount_lock);
	return child_mnt;
}

static inline int check_mnt(struct vfsmount *mnt)
=======
	struct mount *child_mnt;

	br_read_lock(vfsmount_lock);
	child_mnt = __lookup_mnt(path->mnt, path->dentry, 1);
	if (child_mnt) {
		mnt_add_count(child_mnt, 1);
		br_read_unlock(vfsmount_lock);
		return &child_mnt->mnt;
	} else {
		br_read_unlock(vfsmount_lock);
		return NULL;
=======
/* call under rcu_read_lock */
bool legitimize_mnt(struct vfsmount *bastard, unsigned seq)
{
	struct mount *mnt;
	if (read_seqretry(&mount_lock, seq))
		return false;
	if (bastard == NULL)
		return true;
	mnt = real_mount(bastard);
	mnt_add_count(mnt, 1);
	if (likely(!read_seqretry(&mount_lock, seq)))
		return true;
	if (bastard->mnt_flags & MNT_SYNC_UMOUNT) {
		mnt_add_count(mnt, -1);
		return false;
	}
	rcu_read_unlock();
	mntput(bastard);
	rcu_read_lock();
	return false;
}

/*
 * find the first mount at @dentry on vfsmount @mnt.
 * call under rcu_read_lock()
 */
struct mount *__lookup_mnt(struct vfsmount *mnt, struct dentry *dentry)
{
	struct list_head *head = mount_hashtable + hash(mnt, dentry);
	struct mount *p;

	list_for_each_entry_rcu(p, head, mnt_hash)
		if (&p->mnt_parent->mnt == mnt && p->mnt_mountpoint == dentry)
			return p;
	return NULL;
}

/*
 * find the last mount at @dentry on vfsmount @mnt.
 * mount_lock must be held.
 */
struct mount *__lookup_mnt_last(struct vfsmount *mnt, struct dentry *dentry)
{
	struct list_head *head = mount_hashtable + hash(mnt, dentry);
	struct mount *p;

	list_for_each_entry_reverse(p, head, mnt_hash)
		if (&p->mnt_parent->mnt == mnt && p->mnt_mountpoint == dentry)
			return p;
	return NULL;
}

/*
 * lookup_mnt - Return the first child mount mounted at path
 *
 * "First" means first mounted chronologically.  If you create the
 * following mounts:
 *
 * mount /dev/sda1 /mnt
 * mount /dev/sda2 /mnt
 * mount /dev/sda3 /mnt
 *
 * Then lookup_mnt() on the base /mnt dentry in the root mount will
 * return successively the root dentry and vfsmount of /dev/sda1, then
 * /dev/sda2, then /dev/sda3, then NULL.
 *
 * lookup_mnt takes a reference to the found vfsmount.
 */
struct vfsmount *lookup_mnt(struct path *path)
{
	struct mount *child_mnt;
	struct vfsmount *m;
	unsigned seq;

	rcu_read_lock();
	do {
		seq = read_seqbegin(&mount_lock);
		child_mnt = __lookup_mnt(path->mnt, path->dentry);
		m = child_mnt ? &child_mnt->mnt : NULL;
	} while (!legitimize_mnt(m, seq));
	rcu_read_unlock();
	return m;
}

static struct mountpoint *new_mountpoint(struct dentry *dentry)
{
	struct list_head *chain = mountpoint_hashtable + hash(NULL, dentry);
	struct mountpoint *mp;
	int ret;

	list_for_each_entry(mp, chain, m_hash) {
		if (mp->m_dentry == dentry) {
			/* might be worth a WARN_ON() */
			if (d_unlinked(dentry))
				return ERR_PTR(-ENOENT);
			mp->m_count++;
			return mp;
		}
	}

	mp = kmalloc(sizeof(struct mountpoint), GFP_KERNEL);
	if (!mp)
		return ERR_PTR(-ENOMEM);

	ret = d_set_mounted(dentry);
	if (ret) {
		kfree(mp);
		return ERR_PTR(ret);
	}

	mp->m_dentry = dentry;
	mp->m_count = 1;
	list_add(&mp->m_hash, chain);
	return mp;
}

static void put_mountpoint(struct mountpoint *mp)
{
	if (!--mp->m_count) {
		struct dentry *dentry = mp->m_dentry;
		spin_lock(&dentry->d_lock);
		dentry->d_flags &= ~DCACHE_MOUNTED;
		spin_unlock(&dentry->d_lock);
		list_del(&mp->m_hash);
		kfree(mp);
>>>>>>> refs/remotes/origin/master
	}
}

static inline int check_mnt(struct mount *mnt)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	return mnt->mnt_ns == current->nsproxy->mnt_ns;
}

/*
 * vfsmount lock must be held for write
 */
static void touch_mnt_namespace(struct mnt_namespace *ns)
{
	if (ns) {
		ns->event = ++event;
		wake_up_interruptible(&ns->poll);
	}
}

/*
 * vfsmount lock must be held for write
 */
static void __touch_mnt_namespace(struct mnt_namespace *ns)
{
	if (ns && ns->event != event) {
		ns->event = event;
		wake_up_interruptible(&ns->poll);
	}
}

/*
<<<<<<< HEAD
 * Clear dentry's mounted state if it has no remaining mounts.
 * vfsmount_lock must be held for write.
 */
<<<<<<< HEAD
static void dentry_reset_mounted(struct vfsmount *mnt, struct dentry *dentry)
=======
static void dentry_reset_mounted(struct dentry *dentry)
>>>>>>> refs/remotes/origin/cm-10.0
{
	unsigned u;

	for (u = 0; u < HASH_SIZE; u++) {
<<<<<<< HEAD
		struct vfsmount *p;
=======
		struct mount *p;
>>>>>>> refs/remotes/origin/cm-10.0

		list_for_each_entry(p, &mount_hashtable[u], mnt_hash) {
			if (p->mnt_mountpoint == dentry)
				return;
		}
	}
	spin_lock(&dentry->d_lock);
	dentry->d_flags &= ~DCACHE_MOUNTED;
	spin_unlock(&dentry->d_lock);
}

/*
 * vfsmount lock must be held for write
 */
<<<<<<< HEAD
static void detach_mnt(struct vfsmount *mnt, struct path *old_path)
{
	old_path->dentry = mnt->mnt_mountpoint;
	old_path->mnt = mnt->mnt_parent;
	mnt->mnt_parent = mnt;
	mnt->mnt_mountpoint = mnt->mnt_root;
	list_del_init(&mnt->mnt_child);
	list_del_init(&mnt->mnt_hash);
	dentry_reset_mounted(old_path->mnt, old_path->dentry);
=======
=======
 * vfsmount lock must be held for write
 */
>>>>>>> refs/remotes/origin/master
static void detach_mnt(struct mount *mnt, struct path *old_path)
{
	old_path->dentry = mnt->mnt_mountpoint;
	old_path->mnt = &mnt->mnt_parent->mnt;
	mnt->mnt_parent = mnt;
	mnt->mnt_mountpoint = mnt->mnt.mnt_root;
	list_del_init(&mnt->mnt_child);
	list_del_init(&mnt->mnt_hash);
<<<<<<< HEAD
	dentry_reset_mounted(old_path->dentry);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	put_mountpoint(mnt->mnt_mp);
	mnt->mnt_mp = NULL;
>>>>>>> refs/remotes/origin/master
}

/*
 * vfsmount lock must be held for write
 */
<<<<<<< HEAD
<<<<<<< HEAD
void mnt_set_mountpoint(struct vfsmount *mnt, struct dentry *dentry,
			struct vfsmount *child_mnt)
{
	child_mnt->mnt_parent = mntget(mnt);
	child_mnt->mnt_mountpoint = dget(dentry);
=======
void mnt_set_mountpoint(struct mount *mnt, struct dentry *dentry,
			struct mount *child_mnt)
{
	mnt_add_count(mnt, 1);	/* essentially, that's mntget */
	child_mnt->mnt_mountpoint = dget(dentry);
	child_mnt->mnt_parent = mnt;
>>>>>>> refs/remotes/origin/cm-10.0
	spin_lock(&dentry->d_lock);
	dentry->d_flags |= DCACHE_MOUNTED;
	spin_unlock(&dentry->d_lock);
=======
void mnt_set_mountpoint(struct mount *mnt,
			struct mountpoint *mp,
			struct mount *child_mnt)
{
	mp->m_count++;
	mnt_add_count(mnt, 1);	/* essentially, that's mntget */
	child_mnt->mnt_mountpoint = dget(mp->m_dentry);
	child_mnt->mnt_parent = mnt;
	child_mnt->mnt_mp = mp;
>>>>>>> refs/remotes/origin/master
}

/*
 * vfsmount lock must be held for write
 */
<<<<<<< HEAD
<<<<<<< HEAD
static void attach_mnt(struct vfsmount *mnt, struct path *path)
{
	mnt_set_mountpoint(path->mnt, path->dentry, mnt);
	list_add_tail(&mnt->mnt_hash, mount_hashtable +
			hash(path->mnt, path->dentry));
	list_add_tail(&mnt->mnt_child, &path->mnt->mnt_mounts);
}

static inline void __mnt_make_longterm(struct vfsmount *mnt)
=======
static void attach_mnt(struct mount *mnt, struct path *path)
{
	mnt_set_mountpoint(real_mount(path->mnt), path->dentry, mnt);
	list_add_tail(&mnt->mnt_hash, mount_hashtable +
			hash(path->mnt, path->dentry));
	list_add_tail(&mnt->mnt_child, &real_mount(path->mnt)->mnt_mounts);
}

static inline void __mnt_make_longterm(struct mount *mnt)
>>>>>>> refs/remotes/origin/cm-10.0
{
#ifdef CONFIG_SMP
	atomic_inc(&mnt->mnt_longterm);
#endif
}

/* needs vfsmount lock for write */
<<<<<<< HEAD
static inline void __mnt_make_shortterm(struct vfsmount *mnt)
=======
static inline void __mnt_make_shortterm(struct mount *mnt)
>>>>>>> refs/remotes/origin/cm-10.0
{
#ifdef CONFIG_SMP
	atomic_dec(&mnt->mnt_longterm);
#endif
=======
static void attach_mnt(struct mount *mnt,
			struct mount *parent,
			struct mountpoint *mp)
{
	mnt_set_mountpoint(parent, mp, mnt);
	list_add_tail(&mnt->mnt_hash, mount_hashtable +
			hash(&parent->mnt, mp->m_dentry));
	list_add_tail(&mnt->mnt_child, &parent->mnt_mounts);
>>>>>>> refs/remotes/origin/master
}

/*
 * vfsmount lock must be held for write
 */
<<<<<<< HEAD
<<<<<<< HEAD
static void commit_tree(struct vfsmount *mnt)
{
	struct vfsmount *parent = mnt->mnt_parent;
	struct vfsmount *m;
=======
=======
>>>>>>> refs/remotes/origin/master
static void commit_tree(struct mount *mnt)
{
	struct mount *parent = mnt->mnt_parent;
	struct mount *m;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	LIST_HEAD(head);
	struct mnt_namespace *n = parent->mnt_ns;

	BUG_ON(parent == mnt);

	list_add_tail(&head, &mnt->mnt_list);
<<<<<<< HEAD
	list_for_each_entry(m, &head, mnt_list) {
		m->mnt_ns = n;
		__mnt_make_longterm(m);
	}
=======
	list_for_each_entry(m, &head, mnt_list)
		m->mnt_ns = n;
>>>>>>> refs/remotes/origin/master

	list_splice(&head, n->list.prev);

	list_add_tail(&mnt->mnt_hash, mount_hashtable +
<<<<<<< HEAD
<<<<<<< HEAD
				hash(parent, mnt->mnt_mountpoint));
=======
				hash(&parent->mnt, mnt->mnt_mountpoint));
>>>>>>> refs/remotes/origin/cm-10.0
=======
				hash(&parent->mnt, mnt->mnt_mountpoint));
>>>>>>> refs/remotes/origin/master
	list_add_tail(&mnt->mnt_child, &parent->mnt_mounts);
	touch_mnt_namespace(n);
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct vfsmount *next_mnt(struct vfsmount *p, struct vfsmount *root)
=======
static struct mount *next_mnt(struct mount *p, struct mount *root)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct mount *next_mnt(struct mount *p, struct mount *root)
>>>>>>> refs/remotes/origin/master
{
	struct list_head *next = p->mnt_mounts.next;
	if (next == &p->mnt_mounts) {
		while (1) {
			if (p == root)
				return NULL;
			next = p->mnt_child.next;
			if (next != &p->mnt_parent->mnt_mounts)
				break;
			p = p->mnt_parent;
		}
	}
<<<<<<< HEAD
<<<<<<< HEAD
	return list_entry(next, struct vfsmount, mnt_child);
}

static struct vfsmount *skip_mnt_tree(struct vfsmount *p)
{
	struct list_head *prev = p->mnt_mounts.prev;
	while (prev != &p->mnt_mounts) {
		p = list_entry(prev, struct vfsmount, mnt_child);
=======
=======
>>>>>>> refs/remotes/origin/master
	return list_entry(next, struct mount, mnt_child);
}

static struct mount *skip_mnt_tree(struct mount *p)
{
	struct list_head *prev = p->mnt_mounts.prev;
	while (prev != &p->mnt_mounts) {
		p = list_entry(prev, struct mount, mnt_child);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		prev = p->mnt_mounts.prev;
	}
	return p;
}

struct vfsmount *
vfs_kern_mount(struct file_system_type *type, int flags, const char *name, void *data)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct vfsmount *mnt;
=======
	struct mount *mnt;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct mount *mnt;
>>>>>>> refs/remotes/origin/master
	struct dentry *root;

	if (!type)
		return ERR_PTR(-ENODEV);

	mnt = alloc_vfsmnt(name);
	if (!mnt)
		return ERR_PTR(-ENOMEM);

	if (flags & MS_KERNMOUNT)
<<<<<<< HEAD
<<<<<<< HEAD
		mnt->mnt_flags = MNT_INTERNAL;
=======
		mnt->mnt.mnt_flags = MNT_INTERNAL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		mnt->mnt.mnt_flags = MNT_INTERNAL;
>>>>>>> refs/remotes/origin/master

	root = mount_fs(type, flags, name, data);
	if (IS_ERR(root)) {
		free_vfsmnt(mnt);
		return ERR_CAST(root);
	}

<<<<<<< HEAD
<<<<<<< HEAD
	mnt->mnt_root = root;
	mnt->mnt_sb = root->d_sb;
	mnt->mnt_mountpoint = mnt->mnt_root;
	mnt->mnt_parent = mnt;
	return mnt;
}
EXPORT_SYMBOL_GPL(vfs_kern_mount);

static struct vfsmount *clone_mnt(struct vfsmount *old, struct dentry *root,
					int flag)
{
	struct super_block *sb = old->mnt_sb;
	struct vfsmount *mnt = alloc_vfsmnt(old->mnt_devname);
=======
=======
>>>>>>> refs/remotes/origin/master
	mnt->mnt.mnt_root = root;
	mnt->mnt.mnt_sb = root->d_sb;
	mnt->mnt_mountpoint = mnt->mnt.mnt_root;
	mnt->mnt_parent = mnt;
<<<<<<< HEAD
	br_write_lock(vfsmount_lock);
	list_add_tail(&mnt->mnt_instance, &root->d_sb->s_mounts);
	br_write_unlock(vfsmount_lock);
=======
	lock_mount_hash();
	list_add_tail(&mnt->mnt_instance, &root->d_sb->s_mounts);
	unlock_mount_hash();
>>>>>>> refs/remotes/origin/master
	return &mnt->mnt;
}
EXPORT_SYMBOL_GPL(vfs_kern_mount);

static struct mount *clone_mnt(struct mount *old, struct dentry *root,
					int flag)
{
	struct super_block *sb = old->mnt.mnt_sb;
<<<<<<< HEAD
	struct mount *mnt = alloc_vfsmnt(old->mnt_devname);
>>>>>>> refs/remotes/origin/cm-10.0

	if (mnt) {
		if (flag & (CL_SLAVE | CL_PRIVATE))
			mnt->mnt_group_id = 0; /* not a peer of original */
		else
			mnt->mnt_group_id = old->mnt_group_id;

		if ((flag & CL_MAKE_SHARED) && !mnt->mnt_group_id) {
			int err = mnt_alloc_group_id(mnt);
			if (err)
				goto out_free;
		}

<<<<<<< HEAD
		mnt->mnt_flags = old->mnt_flags & ~MNT_WRITE_HOLD;
		atomic_inc(&sb->s_active);
		mnt->mnt_sb = sb;
		mnt->mnt_root = dget(root);
		mnt->mnt_mountpoint = mnt->mnt_root;
		mnt->mnt_parent = mnt;
=======
		mnt->mnt.mnt_flags = old->mnt.mnt_flags & ~MNT_WRITE_HOLD;
		atomic_inc(&sb->s_active);
		mnt->mnt.mnt_sb = sb;
		mnt->mnt.mnt_root = dget(root);
		mnt->mnt_mountpoint = mnt->mnt.mnt_root;
		mnt->mnt_parent = mnt;
		br_write_lock(vfsmount_lock);
		list_add_tail(&mnt->mnt_instance, &sb->s_mounts);
		br_write_unlock(vfsmount_lock);
>>>>>>> refs/remotes/origin/cm-10.0

		if (flag & CL_SLAVE) {
			list_add(&mnt->mnt_slave, &old->mnt_slave_list);
			mnt->mnt_master = old;
			CLEAR_MNT_SHARED(mnt);
		} else if (!(flag & CL_PRIVATE)) {
			if ((flag & CL_MAKE_SHARED) || IS_MNT_SHARED(old))
				list_add(&mnt->mnt_share, &old->mnt_share);
			if (IS_MNT_SLAVE(old))
				list_add(&mnt->mnt_slave, &old->mnt_slave);
			mnt->mnt_master = old->mnt_master;
		}
		if (flag & CL_MAKE_SHARED)
			set_mnt_shared(mnt);

		/* stick the duplicate mount on the same expiry list
		 * as the original if that was on one */
		if (flag & CL_EXPIRE) {
			if (!list_empty(&old->mnt_expire))
				list_add(&mnt->mnt_expire, &old->mnt_expire);
		}
	}
=======
	struct mount *mnt;
	int err;

	mnt = alloc_vfsmnt(old->mnt_devname);
	if (!mnt)
		return ERR_PTR(-ENOMEM);

	if (flag & (CL_SLAVE | CL_PRIVATE | CL_SHARED_TO_SLAVE))
		mnt->mnt_group_id = 0; /* not a peer of original */
	else
		mnt->mnt_group_id = old->mnt_group_id;

	if ((flag & CL_MAKE_SHARED) && !mnt->mnt_group_id) {
		err = mnt_alloc_group_id(mnt);
		if (err)
			goto out_free;
	}

	mnt->mnt.mnt_flags = old->mnt.mnt_flags & ~MNT_WRITE_HOLD;
	/* Don't allow unprivileged users to change mount flags */
	if ((flag & CL_UNPRIVILEGED) && (mnt->mnt.mnt_flags & MNT_READONLY))
		mnt->mnt.mnt_flags |= MNT_LOCK_READONLY;

	/* Don't allow unprivileged users to reveal what is under a mount */
	if ((flag & CL_UNPRIVILEGED) && list_empty(&old->mnt_expire))
		mnt->mnt.mnt_flags |= MNT_LOCKED;

	atomic_inc(&sb->s_active);
	mnt->mnt.mnt_sb = sb;
	mnt->mnt.mnt_root = dget(root);
	mnt->mnt_mountpoint = mnt->mnt.mnt_root;
	mnt->mnt_parent = mnt;
	lock_mount_hash();
	list_add_tail(&mnt->mnt_instance, &sb->s_mounts);
	unlock_mount_hash();

	if ((flag & CL_SLAVE) ||
	    ((flag & CL_SHARED_TO_SLAVE) && IS_MNT_SHARED(old))) {
		list_add(&mnt->mnt_slave, &old->mnt_slave_list);
		mnt->mnt_master = old;
		CLEAR_MNT_SHARED(mnt);
	} else if (!(flag & CL_PRIVATE)) {
		if ((flag & CL_MAKE_SHARED) || IS_MNT_SHARED(old))
			list_add(&mnt->mnt_share, &old->mnt_share);
		if (IS_MNT_SLAVE(old))
			list_add(&mnt->mnt_slave, &old->mnt_slave);
		mnt->mnt_master = old->mnt_master;
	}
	if (flag & CL_MAKE_SHARED)
		set_mnt_shared(mnt);

	/* stick the duplicate mount on the same expiry list
	 * as the original if that was on one */
	if (flag & CL_EXPIRE) {
		if (!list_empty(&old->mnt_expire))
			list_add(&mnt->mnt_expire, &old->mnt_expire);
	}

>>>>>>> refs/remotes/origin/master
	return mnt;

 out_free:
	free_vfsmnt(mnt);
<<<<<<< HEAD
	return NULL;
}

<<<<<<< HEAD
static inline void mntfree(struct vfsmount *mnt)
{
	struct super_block *sb = mnt->mnt_sb;
=======
static inline void mntfree(struct mount *mnt)
{
	struct vfsmount *m = &mnt->mnt;
	struct super_block *sb = m->mnt_sb;
>>>>>>> refs/remotes/origin/cm-10.0

	/*
	 * This probably indicates that somebody messed
	 * up a mnt_want/drop_write() pair.  If this
	 * happens, the filesystem was probably unable
	 * to make r/w->r/o transitions.
	 */
	/*
	 * The locking used to deal with mnt_count decrement provides barriers,
	 * so mnt_get_writers() below is safe.
	 */
	WARN_ON(mnt_get_writers(mnt));
<<<<<<< HEAD
	fsnotify_vfsmount_delete(mnt);
	dput(mnt->mnt_root);
=======
	fsnotify_vfsmount_delete(m);
	dput(m->mnt_root);
>>>>>>> refs/remotes/origin/cm-10.0
	free_vfsmnt(mnt);
	deactivate_super(sb);
}

<<<<<<< HEAD
static void mntput_no_expire(struct vfsmount *mnt)
=======
static void mntput_no_expire(struct mount *mnt)
>>>>>>> refs/remotes/origin/cm-10.0
{
put_again:
#ifdef CONFIG_SMP
	br_read_lock(vfsmount_lock);
	if (likely(atomic_read(&mnt->mnt_longterm))) {
<<<<<<< HEAD
		mnt_dec_count(mnt);
=======
		mnt_add_count(mnt, -1);
>>>>>>> refs/remotes/origin/cm-10.0
		br_read_unlock(vfsmount_lock);
		return;
	}
	br_read_unlock(vfsmount_lock);

	br_write_lock(vfsmount_lock);
<<<<<<< HEAD
	mnt_dec_count(mnt);
=======
	mnt_add_count(mnt, -1);
>>>>>>> refs/remotes/origin/cm-10.0
	if (mnt_get_count(mnt)) {
		br_write_unlock(vfsmount_lock);
		return;
	}
#else
<<<<<<< HEAD
	mnt_dec_count(mnt);
=======
	mnt_add_count(mnt, -1);
>>>>>>> refs/remotes/origin/cm-10.0
	if (likely(mnt_get_count(mnt)))
		return;
	br_write_lock(vfsmount_lock);
#endif
	if (unlikely(mnt->mnt_pinned)) {
		mnt_add_count(mnt, mnt->mnt_pinned + 1);
		mnt->mnt_pinned = 0;
		br_write_unlock(vfsmount_lock);
<<<<<<< HEAD
		acct_auto_close_mnt(mnt);
		goto put_again;
	}
=======
		acct_auto_close_mnt(&mnt->mnt);
		goto put_again;
	}
	list_del(&mnt->mnt_instance);
>>>>>>> refs/remotes/origin/cm-10.0
	br_write_unlock(vfsmount_lock);
	mntfree(mnt);
=======
	return ERR_PTR(err);
}

static void delayed_free(struct rcu_head *head)
{
	struct mount *mnt = container_of(head, struct mount, mnt_rcu);
	kfree(mnt->mnt_devname);
#ifdef CONFIG_SMP
	free_percpu(mnt->mnt_pcp);
#endif
	kmem_cache_free(mnt_cache, mnt);
}

static void mntput_no_expire(struct mount *mnt)
{
put_again:
	rcu_read_lock();
	mnt_add_count(mnt, -1);
	if (likely(mnt->mnt_ns)) { /* shouldn't be the last one */
		rcu_read_unlock();
		return;
	}
	lock_mount_hash();
	if (mnt_get_count(mnt)) {
		rcu_read_unlock();
		unlock_mount_hash();
		return;
	}
	if (unlikely(mnt->mnt_pinned)) {
		mnt_add_count(mnt, mnt->mnt_pinned + 1);
		mnt->mnt_pinned = 0;
		rcu_read_unlock();
		unlock_mount_hash();
		acct_auto_close_mnt(&mnt->mnt);
		goto put_again;
	}
	if (unlikely(mnt->mnt.mnt_flags & MNT_DOOMED)) {
		rcu_read_unlock();
		unlock_mount_hash();
		return;
	}
	mnt->mnt.mnt_flags |= MNT_DOOMED;
	rcu_read_unlock();

	list_del(&mnt->mnt_instance);
	unlock_mount_hash();

	/*
	 * This probably indicates that somebody messed
	 * up a mnt_want/drop_write() pair.  If this
	 * happens, the filesystem was probably unable
	 * to make r/w->r/o transitions.
	 */
	/*
	 * The locking used to deal with mnt_count decrement provides barriers,
	 * so mnt_get_writers() below is safe.
	 */
	WARN_ON(mnt_get_writers(mnt));
	fsnotify_vfsmount_delete(&mnt->mnt);
	dput(mnt->mnt.mnt_root);
	deactivate_super(mnt->mnt.mnt_sb);
	mnt_free_id(mnt);
	call_rcu(&mnt->mnt_rcu, delayed_free);
>>>>>>> refs/remotes/origin/master
}

void mntput(struct vfsmount *mnt)
{
	if (mnt) {
<<<<<<< HEAD
<<<<<<< HEAD
		/* avoid cacheline pingpong, hope gcc doesn't get "smart" */
		if (unlikely(mnt->mnt_expiry_mark))
			mnt->mnt_expiry_mark = 0;
		mntput_no_expire(mnt);
=======
=======
>>>>>>> refs/remotes/origin/master
		struct mount *m = real_mount(mnt);
		/* avoid cacheline pingpong, hope gcc doesn't get "smart" */
		if (unlikely(m->mnt_expiry_mark))
			m->mnt_expiry_mark = 0;
		mntput_no_expire(m);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}
}
EXPORT_SYMBOL(mntput);

struct vfsmount *mntget(struct vfsmount *mnt)
{
	if (mnt)
<<<<<<< HEAD
<<<<<<< HEAD
		mnt_inc_count(mnt);
=======
		mnt_add_count(real_mount(mnt), 1);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		mnt_add_count(real_mount(mnt), 1);
>>>>>>> refs/remotes/origin/master
	return mnt;
}
EXPORT_SYMBOL(mntget);

void mnt_pin(struct vfsmount *mnt)
{
<<<<<<< HEAD
	br_write_lock(vfsmount_lock);
<<<<<<< HEAD
	mnt->mnt_pinned++;
=======
	real_mount(mnt)->mnt_pinned++;
>>>>>>> refs/remotes/origin/cm-10.0
	br_write_unlock(vfsmount_lock);
}
EXPORT_SYMBOL(mnt_pin);

<<<<<<< HEAD
void mnt_unpin(struct vfsmount *mnt)
{
	br_write_lock(vfsmount_lock);
	if (mnt->mnt_pinned) {
		mnt_inc_count(mnt);
=======
void mnt_unpin(struct vfsmount *m)
{
	struct mount *mnt = real_mount(m);
	br_write_lock(vfsmount_lock);
	if (mnt->mnt_pinned) {
		mnt_add_count(mnt, 1);
>>>>>>> refs/remotes/origin/cm-10.0
		mnt->mnt_pinned--;
	}
	br_write_unlock(vfsmount_lock);
=======
	lock_mount_hash();
	real_mount(mnt)->mnt_pinned++;
	unlock_mount_hash();
}
EXPORT_SYMBOL(mnt_pin);

void mnt_unpin(struct vfsmount *m)
{
	struct mount *mnt = real_mount(m);
	lock_mount_hash();
	if (mnt->mnt_pinned) {
		mnt_add_count(mnt, 1);
		mnt->mnt_pinned--;
	}
	unlock_mount_hash();
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(mnt_unpin);

static inline void mangle(struct seq_file *m, const char *s)
{
	seq_escape(m, s, " \t\n\\");
}

/*
 * Simple .show_options callback for filesystems which don't want to
 * implement more complex mount option showing.
 *
 * See also save_mount_options().
 */
<<<<<<< HEAD
<<<<<<< HEAD
int generic_show_options(struct seq_file *m, struct vfsmount *mnt)
=======
int generic_show_options(struct seq_file *m, struct dentry *root)
>>>>>>> refs/remotes/origin/cm-10.0
=======
int generic_show_options(struct seq_file *m, struct dentry *root)
>>>>>>> refs/remotes/origin/master
{
	const char *options;

	rcu_read_lock();
<<<<<<< HEAD
<<<<<<< HEAD
	options = rcu_dereference(mnt->mnt_sb->s_options);
=======
	options = rcu_dereference(root->d_sb->s_options);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	options = rcu_dereference(root->d_sb->s_options);
>>>>>>> refs/remotes/origin/master

	if (options != NULL && options[0]) {
		seq_putc(m, ',');
		mangle(m, options);
	}
	rcu_read_unlock();

	return 0;
}
EXPORT_SYMBOL(generic_show_options);

/*
 * If filesystem uses generic_show_options(), this function should be
 * called from the fill_super() callback.
 *
 * The .remount_fs callback usually needs to be handled in a special
 * way, to make sure, that previous options are not overwritten if the
 * remount fails.
 *
 * Also note, that if the filesystem's .remount_fs function doesn't
 * reset all options to their default value, but changes only newly
 * given options, then the displayed options will not reflect reality
 * any more.
 */
void save_mount_options(struct super_block *sb, char *options)
{
	BUG_ON(sb->s_options);
	rcu_assign_pointer(sb->s_options, kstrdup(options, GFP_KERNEL));
}
EXPORT_SYMBOL(save_mount_options);

void replace_mount_options(struct super_block *sb, char *options)
{
	char *old = sb->s_options;
	rcu_assign_pointer(sb->s_options, options);
	if (old) {
		synchronize_rcu();
		kfree(old);
	}
}
EXPORT_SYMBOL(replace_mount_options);

#ifdef CONFIG_PROC_FS
<<<<<<< HEAD
<<<<<<< HEAD
/* iterator */
static void *m_start(struct seq_file *m, loff_t *pos)
{
	struct proc_mounts *p = m->private;
=======
/* iterator; we want it to have access to namespace_sem, thus here... */
static void *m_start(struct seq_file *m, loff_t *pos)
{
	struct proc_mounts *p = container_of(m, struct proc_mounts, m);
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* iterator; we want it to have access to namespace_sem, thus here... */
static void *m_start(struct seq_file *m, loff_t *pos)
{
	struct proc_mounts *p = proc_mounts(m);
>>>>>>> refs/remotes/origin/master

	down_read(&namespace_sem);
	return seq_list_start(&p->ns->list, *pos);
}

static void *m_next(struct seq_file *m, void *v, loff_t *pos)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct proc_mounts *p = m->private;
=======
	struct proc_mounts *p = container_of(m, struct proc_mounts, m);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct proc_mounts *p = proc_mounts(m);
>>>>>>> refs/remotes/origin/master

	return seq_list_next(v, &p->ns->list, pos);
}

static void m_stop(struct seq_file *m, void *v)
{
	up_read(&namespace_sem);
}

<<<<<<< HEAD
<<<<<<< HEAD
int mnt_had_events(struct proc_mounts *p)
{
	struct mnt_namespace *ns = p->ns;
	int res = 0;

	br_read_lock(vfsmount_lock);
	if (p->event != ns->event) {
		p->event = ns->event;
		res = 1;
	}
	br_read_unlock(vfsmount_lock);

	return res;
}

struct proc_fs_info {
	int flag;
	const char *str;
};

static int show_sb_opts(struct seq_file *m, struct super_block *sb)
{
	static const struct proc_fs_info fs_info[] = {
		{ MS_SYNCHRONOUS, ",sync" },
		{ MS_DIRSYNC, ",dirsync" },
		{ MS_MANDLOCK, ",mand" },
		{ 0, NULL }
	};
	const struct proc_fs_info *fs_infop;

	for (fs_infop = fs_info; fs_infop->flag; fs_infop++) {
		if (sb->s_flags & fs_infop->flag)
			seq_puts(m, fs_infop->str);
	}

	return security_sb_show_options(m, sb);
}

static void show_mnt_opts(struct seq_file *m, struct vfsmount *mnt)
{
	static const struct proc_fs_info mnt_info[] = {
		{ MNT_NOSUID, ",nosuid" },
		{ MNT_NODEV, ",nodev" },
		{ MNT_NOEXEC, ",noexec" },
		{ MNT_NOATIME, ",noatime" },
		{ MNT_NODIRATIME, ",nodiratime" },
		{ MNT_RELATIME, ",relatime" },
		{ 0, NULL }
	};
	const struct proc_fs_info *fs_infop;

	for (fs_infop = mnt_info; fs_infop->flag; fs_infop++) {
		if (mnt->mnt_flags & fs_infop->flag)
			seq_puts(m, fs_infop->str);
	}
}

static void show_type(struct seq_file *m, struct super_block *sb)
{
	mangle(m, sb->s_type->name);
	if (sb->s_subtype && sb->s_subtype[0]) {
		seq_putc(m, '.');
		mangle(m, sb->s_subtype);
	}
}

static int show_vfsmnt(struct seq_file *m, void *v)
{
	struct vfsmount *mnt = list_entry(v, struct vfsmount, mnt_list);
	int err = 0;
	struct path mnt_path = { .dentry = mnt->mnt_root, .mnt = mnt };

	if (mnt->mnt_sb->s_op->show_devname) {
		err = mnt->mnt_sb->s_op->show_devname(m, mnt);
		if (err)
			goto out;
	} else {
		mangle(m, mnt->mnt_devname ? mnt->mnt_devname : "none");
	}
	seq_putc(m, ' ');
	seq_path(m, &mnt_path, " \t\n\\");
	seq_putc(m, ' ');
	show_type(m, mnt->mnt_sb);
	seq_puts(m, __mnt_is_readonly(mnt) ? " ro" : " rw");
	err = show_sb_opts(m, mnt->mnt_sb);
	if (err)
		goto out;
	show_mnt_opts(m, mnt);
	if (mnt->mnt_sb->s_op->show_options)
		err = mnt->mnt_sb->s_op->show_options(m, mnt);
	seq_puts(m, " 0 0\n");
out:
	return err;
=======
static int m_show(struct seq_file *m, void *v)
{
	struct proc_mounts *p = container_of(m, struct proc_mounts, m);
	struct mount *r = list_entry(v, struct mount, mnt_list);
	return p->show(m, &r->mnt);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int m_show(struct seq_file *m, void *v)
{
	struct proc_mounts *p = proc_mounts(m);
	struct mount *r = list_entry(v, struct mount, mnt_list);
	return p->show(m, &r->mnt);
>>>>>>> refs/remotes/origin/master
}

const struct seq_operations mounts_op = {
	.start	= m_start,
	.next	= m_next,
	.stop	= m_stop,
<<<<<<< HEAD
<<<<<<< HEAD
	.show	= show_vfsmnt
};

static int show_mountinfo(struct seq_file *m, void *v)
{
	struct proc_mounts *p = m->private;
	struct vfsmount *mnt = list_entry(v, struct vfsmount, mnt_list);
	struct super_block *sb = mnt->mnt_sb;
	struct path mnt_path = { .dentry = mnt->mnt_root, .mnt = mnt };
	struct path root = p->root;
	int err = 0;

	seq_printf(m, "%i %i %u:%u ", mnt->mnt_id, mnt->mnt_parent->mnt_id,
		   MAJOR(sb->s_dev), MINOR(sb->s_dev));
	if (sb->s_op->show_path)
		err = sb->s_op->show_path(m, mnt);
	else
		seq_dentry(m, mnt->mnt_root, " \t\n\\");
	if (err)
		goto out;
	seq_putc(m, ' ');

	/* mountpoints outside of chroot jail will give SEQ_SKIP on this */
	err = seq_path_root(m, &mnt_path, &root, " \t\n\\");
	if (err)
		goto out;

	seq_puts(m, mnt->mnt_flags & MNT_READONLY ? " ro" : " rw");
	show_mnt_opts(m, mnt);

	/* Tagged fields ("foo:X" or "bar") */
	if (IS_MNT_SHARED(mnt))
		seq_printf(m, " shared:%i", mnt->mnt_group_id);
	if (IS_MNT_SLAVE(mnt)) {
		int master = mnt->mnt_master->mnt_group_id;
		int dom = get_dominating_id(mnt, &p->root);
		seq_printf(m, " master:%i", master);
		if (dom && dom != master)
			seq_printf(m, " propagate_from:%i", dom);
	}
	if (IS_MNT_UNBINDABLE(mnt))
		seq_puts(m, " unbindable");

	/* Filesystem specific data */
	seq_puts(m, " - ");
	show_type(m, sb);
	seq_putc(m, ' ');
	if (sb->s_op->show_devname)
		err = sb->s_op->show_devname(m, mnt);
	else
		mangle(m, mnt->mnt_devname ? mnt->mnt_devname : "none");
	if (err)
		goto out;
	seq_puts(m, sb->s_flags & MS_RDONLY ? " ro" : " rw");
	err = show_sb_opts(m, sb);
	if (err)
		goto out;
	if (sb->s_op->show_options)
		err = sb->s_op->show_options(m, mnt);
	seq_putc(m, '\n');
out:
	return err;
}

const struct seq_operations mountinfo_op = {
	.start	= m_start,
	.next	= m_next,
	.stop	= m_stop,
	.show	= show_mountinfo,
};

static int show_vfsstat(struct seq_file *m, void *v)
{
	struct vfsmount *mnt = list_entry(v, struct vfsmount, mnt_list);
	struct path mnt_path = { .dentry = mnt->mnt_root, .mnt = mnt };
	int err = 0;

	/* device */
	if (mnt->mnt_sb->s_op->show_devname) {
		seq_puts(m, "device ");
		err = mnt->mnt_sb->s_op->show_devname(m, mnt);
	} else {
		if (mnt->mnt_devname) {
			seq_puts(m, "device ");
			mangle(m, mnt->mnt_devname);
		} else
			seq_puts(m, "no device");
	}

	/* mount point */
	seq_puts(m, " mounted on ");
	seq_path(m, &mnt_path, " \t\n\\");
	seq_putc(m, ' ');

	/* file system type */
	seq_puts(m, "with fstype ");
	show_type(m, mnt->mnt_sb);

	/* optional statistics */
	if (mnt->mnt_sb->s_op->show_stats) {
		seq_putc(m, ' ');
		if (!err)
			err = mnt->mnt_sb->s_op->show_stats(m, mnt);
	}

	seq_putc(m, '\n');
	return err;
}

const struct seq_operations mountstats_op = {
	.start	= m_start,
	.next	= m_next,
	.stop	= m_stop,
	.show	= show_vfsstat,
=======
	.show	= m_show,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.show	= m_show,
>>>>>>> refs/remotes/origin/master
};
#endif  /* CONFIG_PROC_FS */

/**
 * may_umount_tree - check if a mount tree is busy
 * @mnt: root of mount tree
 *
 * This is called to check if a tree of mounts has any
 * open files, pwds, chroots or sub mounts that are
 * busy.
 */
<<<<<<< HEAD
<<<<<<< HEAD
int may_umount_tree(struct vfsmount *mnt)
{
	int actual_refs = 0;
	int minimum_refs = 0;
	struct vfsmount *p;
=======
=======
>>>>>>> refs/remotes/origin/master
int may_umount_tree(struct vfsmount *m)
{
	struct mount *mnt = real_mount(m);
	int actual_refs = 0;
	int minimum_refs = 0;
	struct mount *p;
	BUG_ON(!m);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	/* write lock needed for mnt_get_count */
	br_write_lock(vfsmount_lock);
=======

	/* write lock needed for mnt_get_count */
	lock_mount_hash();
>>>>>>> refs/remotes/origin/master
	for (p = mnt; p; p = next_mnt(p, mnt)) {
		actual_refs += mnt_get_count(p);
		minimum_refs += 2;
	}
<<<<<<< HEAD
	br_write_unlock(vfsmount_lock);
=======
	unlock_mount_hash();
>>>>>>> refs/remotes/origin/master

	if (actual_refs > minimum_refs)
		return 0;

	return 1;
}

EXPORT_SYMBOL(may_umount_tree);

/**
 * may_umount - check if a mount point is busy
 * @mnt: root of mount
 *
 * This is called to check if a mount point has any
 * open files, pwds, chroots or sub mounts. If the
 * mount has sub mounts this will return busy
 * regardless of whether the sub mounts are busy.
 *
 * Doesn't take quota and stuff into account. IOW, in some cases it will
 * give false negatives. The main reason why it's here is that we need
 * a non-destructive way to look for easily umountable filesystems.
 */
int may_umount(struct vfsmount *mnt)
{
	int ret = 1;
	down_read(&namespace_sem);
<<<<<<< HEAD
	br_write_lock(vfsmount_lock);
<<<<<<< HEAD
	if (propagate_mount_busy(mnt, 2))
=======
	if (propagate_mount_busy(real_mount(mnt), 2))
>>>>>>> refs/remotes/origin/cm-10.0
		ret = 0;
	br_write_unlock(vfsmount_lock);
=======
	lock_mount_hash();
	if (propagate_mount_busy(real_mount(mnt), 2))
		ret = 0;
	unlock_mount_hash();
>>>>>>> refs/remotes/origin/master
	up_read(&namespace_sem);
	return ret;
}

EXPORT_SYMBOL(may_umount);

<<<<<<< HEAD
void release_mounts(struct list_head *head)
{
<<<<<<< HEAD
	struct vfsmount *mnt;
	while (!list_empty(head)) {
		mnt = list_first_entry(head, struct vfsmount, mnt_hash);
		list_del_init(&mnt->mnt_hash);
		if (mnt->mnt_parent != mnt) {
			struct dentry *dentry;
			struct vfsmount *m;
=======
	struct mount *mnt;
	while (!list_empty(head)) {
		mnt = list_first_entry(head, struct mount, mnt_hash);
		list_del_init(&mnt->mnt_hash);
		if (mnt_has_parent(mnt)) {
			struct dentry *dentry;
			struct mount *m;
>>>>>>> refs/remotes/origin/cm-10.0

			br_write_lock(vfsmount_lock);
			dentry = mnt->mnt_mountpoint;
			m = mnt->mnt_parent;
<<<<<<< HEAD
			mnt->mnt_mountpoint = mnt->mnt_root;
=======
			mnt->mnt_mountpoint = mnt->mnt.mnt_root;
>>>>>>> refs/remotes/origin/cm-10.0
			mnt->mnt_parent = mnt;
			m->mnt_ghosts--;
			br_write_unlock(vfsmount_lock);
			dput(dentry);
<<<<<<< HEAD
			mntput(m);
		}
		mntput(mnt);
=======
			mntput(&m->mnt);
		}
		mntput(&mnt->mnt);
>>>>>>> refs/remotes/origin/cm-10.0
	}
}

/*
 * vfsmount lock must be held for write
 * namespace_sem must be held for write
 */
<<<<<<< HEAD
void umount_tree(struct vfsmount *mnt, int propagate, struct list_head *kill)
{
	LIST_HEAD(tmp_list);
	struct vfsmount *p;
=======
void umount_tree(struct mount *mnt, int propagate, struct list_head *kill)
{
	LIST_HEAD(tmp_list);
	struct mount *p;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static LIST_HEAD(unmounted);	/* protected by namespace_sem */

static void namespace_unlock(void)
{
	struct mount *mnt;
	LIST_HEAD(head);

	if (likely(list_empty(&unmounted))) {
		up_write(&namespace_sem);
		return;
	}

	list_splice_init(&unmounted, &head);
	up_write(&namespace_sem);

	synchronize_rcu();

	while (!list_empty(&head)) {
		mnt = list_first_entry(&head, struct mount, mnt_hash);
		list_del_init(&mnt->mnt_hash);
		if (mnt->mnt_ex_mountpoint.mnt)
			path_put(&mnt->mnt_ex_mountpoint);
		mntput(&mnt->mnt);
	}
}

static inline void namespace_lock(void)
{
	down_write(&namespace_sem);
}

/*
 * mount_lock must be held
 * namespace_sem must be held for write
 * how = 0 => just this tree, don't propagate
 * how = 1 => propagate; we know that nobody else has reference to any victims
 * how = 2 => lazy umount
 */
void umount_tree(struct mount *mnt, int how)
{
	LIST_HEAD(tmp_list);
	struct mount *p;
>>>>>>> refs/remotes/origin/master

	for (p = mnt; p; p = next_mnt(p, mnt))
		list_move(&p->mnt_hash, &tmp_list);

<<<<<<< HEAD
	if (propagate)
=======
	if (how)
>>>>>>> refs/remotes/origin/master
		propagate_umount(&tmp_list);

	list_for_each_entry(p, &tmp_list, mnt_hash) {
		list_del_init(&p->mnt_expire);
		list_del_init(&p->mnt_list);
		__touch_mnt_namespace(p->mnt_ns);
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
		if (p->mnt_ns)
			__mnt_make_shortterm(p);
		p->mnt_ns = NULL;
		list_del_init(&p->mnt_child);
<<<<<<< HEAD
		if (p->mnt_parent != p) {
			p->mnt_parent->mnt_ghosts++;
			dentry_reset_mounted(p->mnt_parent, p->mnt_mountpoint);
=======
		if (mnt_has_parent(p)) {
			p->mnt_parent->mnt_ghosts++;
			dentry_reset_mounted(p->mnt_mountpoint);
>>>>>>> refs/remotes/origin/cm-10.0
		}
		change_mnt_propagation(p, MS_PRIVATE);
	}
	list_splice(&tmp_list, kill);
}

<<<<<<< HEAD
static void shrink_submounts(struct vfsmount *mnt, struct list_head *umounts);

static int do_umount(struct vfsmount *mnt, int flags)
{
	struct super_block *sb = mnt->mnt_sb;
	int retval;
	LIST_HEAD(umount_list);

	retval = security_sb_umount(mnt, flags);
=======
static void shrink_submounts(struct mount *mnt, struct list_head *umounts);
=======
		p->mnt_ns = NULL;
		if (how < 2)
			p->mnt.mnt_flags |= MNT_SYNC_UMOUNT;
		list_del_init(&p->mnt_child);
		if (mnt_has_parent(p)) {
			put_mountpoint(p->mnt_mp);
			/* move the reference to mountpoint into ->mnt_ex_mountpoint */
			p->mnt_ex_mountpoint.dentry = p->mnt_mountpoint;
			p->mnt_ex_mountpoint.mnt = &p->mnt_parent->mnt;
			p->mnt_mountpoint = p->mnt.mnt_root;
			p->mnt_parent = p;
			p->mnt_mp = NULL;
		}
		change_mnt_propagation(p, MS_PRIVATE);
	}
	list_splice(&tmp_list, &unmounted);
}

static void shrink_submounts(struct mount *mnt);
>>>>>>> refs/remotes/origin/master

static int do_umount(struct mount *mnt, int flags)
{
	struct super_block *sb = mnt->mnt.mnt_sb;
	int retval;
<<<<<<< HEAD
	LIST_HEAD(umount_list);

	retval = security_sb_umount(&mnt->mnt, flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======

	retval = security_sb_umount(&mnt->mnt, flags);
>>>>>>> refs/remotes/origin/master
	if (retval)
		return retval;

	/*
	 * Allow userspace to request a mountpoint be expired rather than
	 * unmounting unconditionally. Unmount only happens if:
	 *  (1) the mark is already set (the mark is cleared by mntput())
	 *  (2) the usage count == 1 [parent vfsmount] + 1 [sys_umount]
	 */
	if (flags & MNT_EXPIRE) {
<<<<<<< HEAD
<<<<<<< HEAD
		if (mnt == current->fs->root.mnt ||
=======
		if (&mnt->mnt == current->fs->root.mnt ||
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (&mnt->mnt == current->fs->root.mnt ||
>>>>>>> refs/remotes/origin/master
		    flags & (MNT_FORCE | MNT_DETACH))
			return -EINVAL;

		/*
		 * probably don't strictly need the lock here if we examined
		 * all race cases, but it's a slowpath.
		 */
<<<<<<< HEAD
		br_write_lock(vfsmount_lock);
		if (mnt_get_count(mnt) != 2) {
			br_write_unlock(vfsmount_lock);
			return -EBUSY;
		}
		br_write_unlock(vfsmount_lock);
=======
		lock_mount_hash();
		if (mnt_get_count(mnt) != 2) {
			unlock_mount_hash();
			return -EBUSY;
		}
		unlock_mount_hash();
>>>>>>> refs/remotes/origin/master

		if (!xchg(&mnt->mnt_expiry_mark, 1))
			return -EAGAIN;
	}

	/*
	 * If we may have to abort operations to get out of this
	 * mount, and they will themselves hold resources we must
	 * allow the fs to do things. In the Unix tradition of
	 * 'Gee thats tricky lets do it in userspace' the umount_begin
	 * might fail to complete on the first run through as other tasks
	 * must return, and the like. Thats for the mount program to worry
	 * about for the moment.
	 */

	if (flags & MNT_FORCE && sb->s_op->umount_begin) {
		sb->s_op->umount_begin(sb);
	}

	/*
	 * No sense to grab the lock for this test, but test itself looks
	 * somewhat bogus. Suggestions for better replacement?
	 * Ho-hum... In principle, we might treat that as umount + switch
	 * to rootfs. GC would eventually take care of the old vfsmount.
	 * Actually it makes sense, especially if rootfs would contain a
	 * /reboot - static binary that would close all descriptors and
	 * call reboot(9). Then init(8) could umount root and exec /reboot.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (mnt == current->fs->root.mnt && !(flags & MNT_DETACH)) {
=======
	if (&mnt->mnt == current->fs->root.mnt && !(flags & MNT_DETACH)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (&mnt->mnt == current->fs->root.mnt && !(flags & MNT_DETACH)) {
>>>>>>> refs/remotes/origin/master
		/*
		 * Special case for "unmounting" root ...
		 * we just try to remount it readonly.
		 */
		down_write(&sb->s_umount);
		if (!(sb->s_flags & MS_RDONLY))
			retval = do_remount_sb(sb, MS_RDONLY, NULL, 0);
		up_write(&sb->s_umount);
		return retval;
	}

<<<<<<< HEAD
	down_write(&namespace_sem);
	br_write_lock(vfsmount_lock);
	event++;

	if (!(flags & MNT_DETACH))
		shrink_submounts(mnt, &umount_list);

	retval = -EBUSY;
	if (flags & MNT_DETACH || !propagate_mount_busy(mnt, 2)) {
		if (!list_empty(&mnt->mnt_list))
			umount_tree(mnt, 1, &umount_list);
		retval = 0;
	}
	br_write_unlock(vfsmount_lock);
	up_write(&namespace_sem);
	release_mounts(&umount_list);
	return retval;
}

=======
	namespace_lock();
	lock_mount_hash();
	event++;

	if (flags & MNT_DETACH) {
		if (!list_empty(&mnt->mnt_list))
			umount_tree(mnt, 2);
		retval = 0;
	} else {
		shrink_submounts(mnt);
		retval = -EBUSY;
		if (!propagate_mount_busy(mnt, 2)) {
			if (!list_empty(&mnt->mnt_list))
				umount_tree(mnt, 1);
			retval = 0;
		}
	}
	unlock_mount_hash();
	namespace_unlock();
	return retval;
}

/* 
 * Is the caller allowed to modify his namespace?
 */
static inline bool may_mount(void)
{
	return ns_capable(current->nsproxy->mnt_ns->user_ns, CAP_SYS_ADMIN);
}

>>>>>>> refs/remotes/origin/master
/*
 * Now umount can handle mount points as well as block devices.
 * This is important for filesystems which use unnamed block devices.
 *
 * We now support a flag for forced unmount like the other 'big iron'
 * unixes. Our API is identical to OSF/1 to avoid making a mess of AMD
 */

SYSCALL_DEFINE2(umount, char __user *, name, int, flags)
{
	struct path path;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct mount *mnt;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct mount *mnt;
>>>>>>> refs/remotes/origin/master
	int retval;
	int lookup_flags = 0;

	if (flags & ~(MNT_FORCE | MNT_DETACH | MNT_EXPIRE | UMOUNT_NOFOLLOW))
		return -EINVAL;

<<<<<<< HEAD
	if (!(flags & UMOUNT_NOFOLLOW))
		lookup_flags |= LOOKUP_FOLLOW;

	retval = user_path_at(AT_FDCWD, name, lookup_flags, &path);
	if (retval)
		goto out;
<<<<<<< HEAD
	retval = -EINVAL;
	if (path.dentry != path.mnt->mnt_root)
		goto dput_and_out;
	if (!check_mnt(path.mnt))
=======
=======
	if (!may_mount())
		return -EPERM;

	if (!(flags & UMOUNT_NOFOLLOW))
		lookup_flags |= LOOKUP_FOLLOW;

	retval = user_path_mountpoint_at(AT_FDCWD, name, lookup_flags, &path);
	if (retval)
		goto out;
>>>>>>> refs/remotes/origin/master
	mnt = real_mount(path.mnt);
	retval = -EINVAL;
	if (path.dentry != path.mnt->mnt_root)
		goto dput_and_out;
	if (!check_mnt(mnt))
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		goto dput_and_out;

	retval = -EPERM;
	if (!capable(CAP_SYS_ADMIN))
		goto dput_and_out;

<<<<<<< HEAD
	retval = do_umount(path.mnt, flags);
dput_and_out:
	/* we mustn't call path_put() as that would clear mnt_expiry_mark */
	dput(path.dentry);
	mntput_no_expire(path.mnt);
=======
=======
		goto dput_and_out;
	if (mnt->mnt.mnt_flags & MNT_LOCKED)
		goto dput_and_out;

>>>>>>> refs/remotes/origin/master
	retval = do_umount(mnt, flags);
dput_and_out:
	/* we mustn't call path_put() as that would clear mnt_expiry_mark */
	dput(path.dentry);
	mntput_no_expire(mnt);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
out:
	return retval;
}

#ifdef __ARCH_WANT_SYS_OLDUMOUNT

/*
 *	The 2.0 compatible umount. No flags.
 */
SYSCALL_DEFINE1(oldumount, char __user *, name)
{
	return sys_umount(name, 0);
}

#endif

<<<<<<< HEAD
static int mount_is_safe(struct path *path)
{
	if (capable(CAP_SYS_ADMIN))
		return 0;
	return -EPERM;
#ifdef notyet
	if (S_ISLNK(path->dentry->d_inode->i_mode))
		return -EPERM;
	if (path->dentry->d_inode->i_mode & S_ISVTX) {
		if (current_uid() != path->dentry->d_inode->i_uid)
			return -EPERM;
	}
	if (inode_permission(path->dentry->d_inode, MAY_WRITE))
		return -EPERM;
	return 0;
#endif
}

<<<<<<< HEAD
struct vfsmount *copy_tree(struct vfsmount *mnt, struct dentry *dentry,
					int flag)
{
	struct vfsmount *res, *p, *q, *r, *s;
=======
struct mount *copy_tree(struct mount *mnt, struct dentry *dentry,
					int flag)
{
	struct mount *res, *p, *q, *r;
>>>>>>> refs/remotes/origin/cm-10.0
	struct path path;

	if (!(flag & CL_COPY_ALL) && IS_MNT_UNBINDABLE(mnt))
		return NULL;

	res = q = clone_mnt(mnt, dentry, flag);
	if (!q)
		goto Enomem;
=======
static bool is_mnt_ns_file(struct dentry *dentry)
{
	/* Is this a proxy for a mount namespace? */
	struct inode *inode = dentry->d_inode;
	struct proc_ns *ei;

	if (!proc_ns_inode(inode))
		return false;

	ei = get_proc_ns(inode);
	if (ei->ns_ops != &mntns_operations)
		return false;

	return true;
}

static bool mnt_ns_loop(struct dentry *dentry)
{
	/* Could bind mounting the mount namespace inode cause a
	 * mount namespace loop?
	 */
	struct mnt_namespace *mnt_ns;
	if (!is_mnt_ns_file(dentry))
		return false;

	mnt_ns = get_proc_ns(dentry->d_inode)->ns;
	return current->nsproxy->mnt_ns->seq >= mnt_ns->seq;
}

struct mount *copy_tree(struct mount *mnt, struct dentry *dentry,
					int flag)
{
	struct mount *res, *p, *q, *r, *parent;

	if (!(flag & CL_COPY_UNBINDABLE) && IS_MNT_UNBINDABLE(mnt))
		return ERR_PTR(-EINVAL);

	if (!(flag & CL_COPY_MNT_NS_FILE) && is_mnt_ns_file(dentry))
		return ERR_PTR(-EINVAL);

	res = q = clone_mnt(mnt, dentry, flag);
	if (IS_ERR(q))
		return q;

	q->mnt.mnt_flags &= ~MNT_LOCKED;
>>>>>>> refs/remotes/origin/master
	q->mnt_mountpoint = mnt->mnt_mountpoint;

	p = mnt;
	list_for_each_entry(r, &mnt->mnt_mounts, mnt_child) {
<<<<<<< HEAD
<<<<<<< HEAD
=======
		struct mount *s;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		struct mount *s;
>>>>>>> refs/remotes/origin/master
		if (!is_subdir(r->mnt_mountpoint, dentry))
			continue;

		for (s = r; s; s = next_mnt(s, r)) {
<<<<<<< HEAD
			if (!(flag & CL_COPY_ALL) && IS_MNT_UNBINDABLE(s)) {
=======
			if (!(flag & CL_COPY_UNBINDABLE) &&
			    IS_MNT_UNBINDABLE(s)) {
				s = skip_mnt_tree(s);
				continue;
			}
			if (!(flag & CL_COPY_MNT_NS_FILE) &&
			    is_mnt_ns_file(s->mnt.mnt_root)) {
>>>>>>> refs/remotes/origin/master
				s = skip_mnt_tree(s);
				continue;
			}
			while (p != s->mnt_parent) {
				p = p->mnt_parent;
				q = q->mnt_parent;
			}
			p = s;
<<<<<<< HEAD
<<<<<<< HEAD
			path.mnt = q;
			path.dentry = p->mnt_mountpoint;
			q = clone_mnt(p, p->mnt_root, flag);
=======
			path.mnt = &q->mnt;
			path.dentry = p->mnt_mountpoint;
			q = clone_mnt(p, p->mnt.mnt_root, flag);
>>>>>>> refs/remotes/origin/cm-10.0
			if (!q)
				goto Enomem;
			br_write_lock(vfsmount_lock);
			list_add_tail(&q->mnt_list, &res->mnt_list);
			attach_mnt(q, &path);
			br_write_unlock(vfsmount_lock);
		}
	}
	return res;
Enomem:
	if (res) {
		LIST_HEAD(umount_list);
		br_write_lock(vfsmount_lock);
		umount_tree(res, 0, &umount_list);
		br_write_unlock(vfsmount_lock);
		release_mounts(&umount_list);
	}
	return NULL;
}

struct vfsmount *collect_mounts(struct path *path)
{
<<<<<<< HEAD
	struct vfsmount *tree;
	down_write(&namespace_sem);
	tree = copy_tree(path->mnt, path->dentry, CL_COPY_ALL | CL_PRIVATE);
	up_write(&namespace_sem);
	return tree;
=======
	struct mount *tree;
	down_write(&namespace_sem);
	tree = copy_tree(real_mount(path->mnt), path->dentry,
			 CL_COPY_ALL | CL_PRIVATE);
	up_write(&namespace_sem);
	return tree ? &tree->mnt : NULL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			parent = q;
			q = clone_mnt(p, p->mnt.mnt_root, flag);
			if (IS_ERR(q))
				goto out;
			lock_mount_hash();
			list_add_tail(&q->mnt_list, &res->mnt_list);
			attach_mnt(q, parent, p->mnt_mp);
			unlock_mount_hash();
		}
	}
	return res;
out:
	if (res) {
		lock_mount_hash();
		umount_tree(res, 0);
		unlock_mount_hash();
	}
	return q;
}

/* Caller should check returned pointer for errors */

struct vfsmount *collect_mounts(struct path *path)
{
	struct mount *tree;
	namespace_lock();
	tree = copy_tree(real_mount(path->mnt), path->dentry,
			 CL_COPY_ALL | CL_PRIVATE);
	namespace_unlock();
	if (IS_ERR(tree))
		return ERR_CAST(tree);
	return &tree->mnt;
>>>>>>> refs/remotes/origin/master
}

void drop_collected_mounts(struct vfsmount *mnt)
{
<<<<<<< HEAD
	LIST_HEAD(umount_list);
	down_write(&namespace_sem);
	br_write_lock(vfsmount_lock);
<<<<<<< HEAD
	umount_tree(mnt, 0, &umount_list);
=======
	umount_tree(real_mount(mnt), 0, &umount_list);
>>>>>>> refs/remotes/origin/cm-10.0
	br_write_unlock(vfsmount_lock);
	up_write(&namespace_sem);
	release_mounts(&umount_list);
=======
	namespace_lock();
	lock_mount_hash();
	umount_tree(real_mount(mnt), 0);
	unlock_mount_hash();
	namespace_unlock();
>>>>>>> refs/remotes/origin/master
}

int iterate_mounts(int (*f)(struct vfsmount *, void *), void *arg,
		   struct vfsmount *root)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct vfsmount *mnt;
	int res = f(root, arg);
	if (res)
		return res;
	list_for_each_entry(mnt, &root->mnt_list, mnt_list) {
		res = f(mnt, arg);
=======
=======
>>>>>>> refs/remotes/origin/master
	struct mount *mnt;
	int res = f(root, arg);
	if (res)
		return res;
	list_for_each_entry(mnt, &real_mount(root)->mnt_list, mnt_list) {
		res = f(&mnt->mnt, arg);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		if (res)
			return res;
	}
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void cleanup_group_ids(struct vfsmount *mnt, struct vfsmount *end)
{
	struct vfsmount *p;
=======
static void cleanup_group_ids(struct mount *mnt, struct mount *end)
{
	struct mount *p;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void cleanup_group_ids(struct mount *mnt, struct mount *end)
{
	struct mount *p;
>>>>>>> refs/remotes/origin/master

	for (p = mnt; p != end; p = next_mnt(p, mnt)) {
		if (p->mnt_group_id && !IS_MNT_SHARED(p))
			mnt_release_group_id(p);
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static int invent_group_ids(struct vfsmount *mnt, bool recurse)
{
	struct vfsmount *p;
=======
static int invent_group_ids(struct mount *mnt, bool recurse)
{
	struct mount *p;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int invent_group_ids(struct mount *mnt, bool recurse)
{
	struct mount *p;
>>>>>>> refs/remotes/origin/master

	for (p = mnt; p; p = recurse ? next_mnt(p, mnt) : NULL) {
		if (!p->mnt_group_id && !IS_MNT_SHARED(p)) {
			int err = mnt_alloc_group_id(p);
			if (err) {
				cleanup_group_ids(mnt, p);
				return err;
			}
		}
	}

	return 0;
}

/*
 *  @source_mnt : mount tree to be attached
 *  @nd         : place the mount tree @source_mnt is attached
 *  @parent_nd  : if non-null, detach the source_mnt from its parent and
 *  		   store the parent mount and mountpoint dentry.
 *  		   (done when source_mnt is moved)
 *
 *  NOTE: in the table below explains the semantics when a source mount
 *  of a given type is attached to a destination mount of a given type.
 * ---------------------------------------------------------------------------
 * |         BIND MOUNT OPERATION                                            |
 * |**************************************************************************
 * | source-->| shared        |       private  |       slave    | unbindable |
 * | dest     |               |                |                |            |
 * |   |      |               |                |                |            |
 * |   v      |               |                |                |            |
 * |**************************************************************************
 * |  shared  | shared (++)   |     shared (+) |     shared(+++)|  invalid   |
 * |          |               |                |                |            |
 * |non-shared| shared (+)    |      private   |      slave (*) |  invalid   |
 * ***************************************************************************
 * A bind operation clones the source mount and mounts the clone on the
 * destination mount.
 *
 * (++)  the cloned mount is propagated to all the mounts in the propagation
 * 	 tree of the destination mount and the cloned mount is added to
 * 	 the peer group of the source mount.
 * (+)   the cloned mount is created under the destination mount and is marked
 *       as shared. The cloned mount is added to the peer group of the source
 *       mount.
 * (+++) the mount is propagated to all the mounts in the propagation tree
 *       of the destination mount and the cloned mount is made slave
 *       of the same master as that of the source mount. The cloned mount
 *       is marked as 'shared and slave'.
 * (*)   the cloned mount is made a slave of the same master as that of the
 * 	 source mount.
 *
 * ---------------------------------------------------------------------------
 * |         		MOVE MOUNT OPERATION                                 |
 * |**************************************************************************
 * | source-->| shared        |       private  |       slave    | unbindable |
 * | dest     |               |                |                |            |
 * |   |      |               |                |                |            |
 * |   v      |               |                |                |            |
 * |**************************************************************************
 * |  shared  | shared (+)    |     shared (+) |    shared(+++) |  invalid   |
 * |          |               |                |                |            |
 * |non-shared| shared (+*)   |      private   |    slave (*)   | unbindable |
 * ***************************************************************************
 *
 * (+)  the mount is moved to the destination. And is then propagated to
 * 	all the mounts in the propagation tree of the destination mount.
 * (+*)  the mount is moved to the destination.
 * (+++)  the mount is moved to the destination and is then propagated to
 * 	all the mounts belonging to the destination mount's propagation tree.
 * 	the mount is marked as 'shared and slave'.
 * (*)	the mount continues to be a slave at the new location.
 *
 * if the source mount is a tree, the operations explained above is
 * applied to each mount in the tree.
 * Must be called without spinlocks held, since this function can sleep
 * in allocations.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int attach_recursive_mnt(struct vfsmount *source_mnt,
			struct path *path, struct path *parent_path)
{
	LIST_HEAD(tree_list);
	struct vfsmount *dest_mnt = path->mnt;
	struct dentry *dest_dentry = path->dentry;
	struct vfsmount *child, *p;
=======
static int attach_recursive_mnt(struct mount *source_mnt,
			struct path *path, struct path *parent_path)
{
	LIST_HEAD(tree_list);
	struct mount *dest_mnt = real_mount(path->mnt);
	struct dentry *dest_dentry = path->dentry;
	struct mount *child, *p;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int attach_recursive_mnt(struct mount *source_mnt,
			struct mount *dest_mnt,
			struct mountpoint *dest_mp,
			struct path *parent_path)
{
	LIST_HEAD(tree_list);
	struct mount *child, *p;
>>>>>>> refs/remotes/origin/master
	int err;

	if (IS_MNT_SHARED(dest_mnt)) {
		err = invent_group_ids(source_mnt, true);
		if (err)
			goto out;
	}
<<<<<<< HEAD
	err = propagate_mnt(dest_mnt, dest_dentry, source_mnt, &tree_list);
	if (err)
		goto out_cleanup_ids;

	br_write_lock(vfsmount_lock);
=======
	err = propagate_mnt(dest_mnt, dest_mp, source_mnt, &tree_list);
	if (err)
		goto out_cleanup_ids;

	lock_mount_hash();
>>>>>>> refs/remotes/origin/master

	if (IS_MNT_SHARED(dest_mnt)) {
		for (p = source_mnt; p; p = next_mnt(p, source_mnt))
			set_mnt_shared(p);
	}
	if (parent_path) {
		detach_mnt(source_mnt, parent_path);
<<<<<<< HEAD
		attach_mnt(source_mnt, path);
<<<<<<< HEAD
		touch_mnt_namespace(parent_path->mnt->mnt_ns);
=======
		touch_mnt_namespace(source_mnt->mnt_ns);
>>>>>>> refs/remotes/origin/cm-10.0
	} else {
		mnt_set_mountpoint(dest_mnt, dest_dentry, source_mnt);
=======
		attach_mnt(source_mnt, dest_mnt, dest_mp);
		touch_mnt_namespace(source_mnt->mnt_ns);
	} else {
		mnt_set_mountpoint(dest_mnt, dest_mp, source_mnt);
>>>>>>> refs/remotes/origin/master
		commit_tree(source_mnt);
	}

	list_for_each_entry_safe(child, p, &tree_list, mnt_hash) {
		list_del_init(&child->mnt_hash);
		commit_tree(child);
	}
<<<<<<< HEAD
	br_write_unlock(vfsmount_lock);
=======
	unlock_mount_hash();
>>>>>>> refs/remotes/origin/master

	return 0;

 out_cleanup_ids:
	if (IS_MNT_SHARED(dest_mnt))
		cleanup_group_ids(source_mnt, NULL);
 out:
	return err;
}

<<<<<<< HEAD
static int lock_mount(struct path *path)
{
	struct vfsmount *mnt;
retry:
	mutex_lock(&path->dentry->d_inode->i_mutex);
	if (unlikely(cant_mount(path->dentry))) {
		mutex_unlock(&path->dentry->d_inode->i_mutex);
		return -ENOENT;
	}
	down_write(&namespace_sem);
	mnt = lookup_mnt(path);
	if (likely(!mnt))
		return 0;
	up_write(&namespace_sem);
	mutex_unlock(&path->dentry->d_inode->i_mutex);
	path_put(path);
	path->mnt = mnt;
	path->dentry = dget(mnt->mnt_root);
	goto retry;
}

static void unlock_mount(struct path *path)
{
	up_write(&namespace_sem);
	mutex_unlock(&path->dentry->d_inode->i_mutex);
}

<<<<<<< HEAD
static int graft_tree(struct vfsmount *mnt, struct path *path)
{
	if (mnt->mnt_sb->s_flags & MS_NOUSER)
		return -EINVAL;

	if (S_ISDIR(path->dentry->d_inode->i_mode) !=
	      S_ISDIR(mnt->mnt_root->d_inode->i_mode))
=======
static int graft_tree(struct mount *mnt, struct path *path)
=======
static struct mountpoint *lock_mount(struct path *path)
{
	struct vfsmount *mnt;
	struct dentry *dentry = path->dentry;
retry:
	mutex_lock(&dentry->d_inode->i_mutex);
	if (unlikely(cant_mount(dentry))) {
		mutex_unlock(&dentry->d_inode->i_mutex);
		return ERR_PTR(-ENOENT);
	}
	namespace_lock();
	mnt = lookup_mnt(path);
	if (likely(!mnt)) {
		struct mountpoint *mp = new_mountpoint(dentry);
		if (IS_ERR(mp)) {
			namespace_unlock();
			mutex_unlock(&dentry->d_inode->i_mutex);
			return mp;
		}
		return mp;
	}
	namespace_unlock();
	mutex_unlock(&path->dentry->d_inode->i_mutex);
	path_put(path);
	path->mnt = mnt;
	dentry = path->dentry = dget(mnt->mnt_root);
	goto retry;
}

static void unlock_mount(struct mountpoint *where)
{
	struct dentry *dentry = where->m_dentry;
	put_mountpoint(where);
	namespace_unlock();
	mutex_unlock(&dentry->d_inode->i_mutex);
}

static int graft_tree(struct mount *mnt, struct mount *p, struct mountpoint *mp)
>>>>>>> refs/remotes/origin/master
{
	if (mnt->mnt.mnt_sb->s_flags & MS_NOUSER)
		return -EINVAL;

<<<<<<< HEAD
	if (S_ISDIR(path->dentry->d_inode->i_mode) !=
	      S_ISDIR(mnt->mnt.mnt_root->d_inode->i_mode))
>>>>>>> refs/remotes/origin/cm-10.0
		return -ENOTDIR;

	if (d_unlinked(path->dentry))
		return -ENOENT;

	return attach_recursive_mnt(mnt, path, NULL);
=======
	if (S_ISDIR(mp->m_dentry->d_inode->i_mode) !=
	      S_ISDIR(mnt->mnt.mnt_root->d_inode->i_mode))
		return -ENOTDIR;

	return attach_recursive_mnt(mnt, p, mp, NULL);
>>>>>>> refs/remotes/origin/master
}

/*
 * Sanity check the flags to change_mnt_propagation.
 */

static int flags_to_propagation_type(int flags)
{
	int type = flags & ~(MS_REC | MS_SILENT);

	/* Fail if any non-propagation flags are set */
	if (type & ~(MS_SHARED | MS_PRIVATE | MS_SLAVE | MS_UNBINDABLE))
		return 0;
	/* Only one propagation flag should be set */
	if (!is_power_of_2(type))
		return 0;
	return type;
}

/*
 * recursively change the type of the mountpoint.
 */
static int do_change_type(struct path *path, int flag)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct vfsmount *m, *mnt = path->mnt;
=======
	struct mount *m;
	struct mount *mnt = real_mount(path->mnt);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct mount *m;
	struct mount *mnt = real_mount(path->mnt);
>>>>>>> refs/remotes/origin/master
	int recurse = flag & MS_REC;
	int type;
	int err = 0;

<<<<<<< HEAD
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

=======
>>>>>>> refs/remotes/origin/master
	if (path->dentry != path->mnt->mnt_root)
		return -EINVAL;

	type = flags_to_propagation_type(flag);
	if (!type)
		return -EINVAL;

<<<<<<< HEAD
	down_write(&namespace_sem);
=======
	namespace_lock();
>>>>>>> refs/remotes/origin/master
	if (type == MS_SHARED) {
		err = invent_group_ids(mnt, recurse);
		if (err)
			goto out_unlock;
	}

<<<<<<< HEAD
	br_write_lock(vfsmount_lock);
	for (m = mnt; m; m = (recurse ? next_mnt(m, mnt) : NULL))
		change_mnt_propagation(m, type);
	br_write_unlock(vfsmount_lock);

 out_unlock:
	up_write(&namespace_sem);
	return err;
}

/*
 * do loopback mount.
 */
static int do_loopback(struct path *path, char *old_name,
				int recurse)
{
	LIST_HEAD(umount_list);
	struct path old_path;
<<<<<<< HEAD
	struct vfsmount *mnt = NULL;
=======
	struct mount *mnt = NULL, *old;
>>>>>>> refs/remotes/origin/cm-10.0
	int err = mount_is_safe(path);
	if (err)
		return err;
=======
	lock_mount_hash();
	for (m = mnt; m; m = (recurse ? next_mnt(m, mnt) : NULL))
		change_mnt_propagation(m, type);
	unlock_mount_hash();

 out_unlock:
	namespace_unlock();
	return err;
}

static bool has_locked_children(struct mount *mnt, struct dentry *dentry)
{
	struct mount *child;
	list_for_each_entry(child, &mnt->mnt_mounts, mnt_child) {
		if (!is_subdir(child->mnt_mountpoint, dentry))
			continue;

		if (child->mnt.mnt_flags & MNT_LOCKED)
			return true;
	}
	return false;
}

/*
 * do loopback mount.
 */
static int do_loopback(struct path *path, const char *old_name,
				int recurse)
{
	struct path old_path;
	struct mount *mnt = NULL, *old, *parent;
	struct mountpoint *mp;
	int err;
>>>>>>> refs/remotes/origin/master
	if (!old_name || !*old_name)
		return -EINVAL;
	err = kern_path(old_name, LOOKUP_FOLLOW|LOOKUP_AUTOMOUNT, &old_path);
	if (err)
		return err;

<<<<<<< HEAD
	err = lock_mount(path);
	if (err)
		goto out;

<<<<<<< HEAD
	err = -EINVAL;
	if (IS_MNT_UNBINDABLE(old_path.mnt))
		goto out2;

	if (!check_mnt(path->mnt) || !check_mnt(old_path.mnt))
=======
	old = real_mount(old_path.mnt);
=======
	err = -EINVAL;
	if (mnt_ns_loop(old_path.dentry))
		goto out; 

	mp = lock_mount(path);
	err = PTR_ERR(mp);
	if (IS_ERR(mp))
		goto out;

	old = real_mount(old_path.mnt);
	parent = real_mount(path->mnt);
>>>>>>> refs/remotes/origin/master

	err = -EINVAL;
	if (IS_MNT_UNBINDABLE(old))
		goto out2;

<<<<<<< HEAD
	if (!check_mnt(real_mount(path->mnt)) || !check_mnt(old))
>>>>>>> refs/remotes/origin/cm-10.0
		goto out2;

	err = -ENOMEM;
	if (recurse)
<<<<<<< HEAD
		mnt = copy_tree(old_path.mnt, old_path.dentry, 0);
	else
		mnt = clone_mnt(old_path.mnt, old_path.dentry, 0);
=======
		mnt = copy_tree(old, old_path.dentry, 0);
	else
		mnt = clone_mnt(old, old_path.dentry, 0);
>>>>>>> refs/remotes/origin/cm-10.0

	if (!mnt)
		goto out2;

	err = graft_tree(mnt, path);
	if (err) {
		br_write_lock(vfsmount_lock);
		umount_tree(mnt, 0, &umount_list);
		br_write_unlock(vfsmount_lock);
	}
out2:
	unlock_mount(path);
	release_mounts(&umount_list);
=======
	if (!check_mnt(parent) || !check_mnt(old))
		goto out2;

	if (!recurse && has_locked_children(old, old_path.dentry))
		goto out2;

	if (recurse)
		mnt = copy_tree(old, old_path.dentry, CL_COPY_MNT_NS_FILE);
	else
		mnt = clone_mnt(old, old_path.dentry, 0);

	if (IS_ERR(mnt)) {
		err = PTR_ERR(mnt);
		goto out2;
	}

	mnt->mnt.mnt_flags &= ~MNT_LOCKED;

	err = graft_tree(mnt, parent, mp);
	if (err) {
		lock_mount_hash();
		umount_tree(mnt, 0);
		unlock_mount_hash();
	}
out2:
	unlock_mount(mp);
>>>>>>> refs/remotes/origin/master
out:
	path_put(&old_path);
	return err;
}

static int change_mount_flags(struct vfsmount *mnt, int ms_flags)
{
	int error = 0;
	int readonly_request = 0;

	if (ms_flags & MS_RDONLY)
		readonly_request = 1;
	if (readonly_request == __mnt_is_readonly(mnt))
		return 0;

<<<<<<< HEAD
	if (readonly_request)
<<<<<<< HEAD
		error = mnt_make_readonly(mnt);
	else
		__mnt_unmake_readonly(mnt);
=======
		error = mnt_make_readonly(real_mount(mnt));
	else
		__mnt_unmake_readonly(real_mount(mnt));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (mnt->mnt_flags & MNT_LOCK_READONLY)
		return -EPERM;

	if (readonly_request)
		error = mnt_make_readonly(real_mount(mnt));
	else
		__mnt_unmake_readonly(real_mount(mnt));
>>>>>>> refs/remotes/origin/master
	return error;
}

/*
 * change filesystem flags. dir should be a physical root of filesystem.
 * If you've mounted a non-root directory somewhere and want to do remount
 * on it - tough luck.
 */
static int do_remount(struct path *path, int flags, int mnt_flags,
		      void *data)
{
	int err;
	struct super_block *sb = path->mnt->mnt_sb;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct mount *mnt = real_mount(path->mnt);
>>>>>>> refs/remotes/origin/cm-10.0

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

<<<<<<< HEAD
	if (!check_mnt(path->mnt))
=======
	if (!check_mnt(mnt))
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct mount *mnt = real_mount(path->mnt);

	if (!check_mnt(mnt))
>>>>>>> refs/remotes/origin/master
		return -EINVAL;

	if (path->dentry != path->mnt->mnt_root)
		return -EINVAL;

	err = security_sb_remount(sb, data);
	if (err)
		return err;

	down_write(&sb->s_umount);
	if (flags & MS_BIND)
		err = change_mount_flags(path->mnt, flags);
<<<<<<< HEAD
	else
		err = do_remount_sb(sb, flags, data, 0);
	if (!err) {
		br_write_lock(vfsmount_lock);
<<<<<<< HEAD
		mnt_flags |= path->mnt->mnt_flags & MNT_PROPAGATION_MASK;
		path->mnt->mnt_flags = mnt_flags;
=======
		mnt_flags |= mnt->mnt.mnt_flags & MNT_PROPAGATION_MASK;
		mnt->mnt.mnt_flags = mnt_flags;
>>>>>>> refs/remotes/origin/cm-10.0
		br_write_unlock(vfsmount_lock);
	}
	up_write(&sb->s_umount);
	if (!err) {
		br_write_lock(vfsmount_lock);
<<<<<<< HEAD
		touch_mnt_namespace(path->mnt->mnt_ns);
=======
		touch_mnt_namespace(mnt->mnt_ns);
>>>>>>> refs/remotes/origin/cm-10.0
		br_write_unlock(vfsmount_lock);
	}
	return err;
}

<<<<<<< HEAD
static inline int tree_contains_unbindable(struct vfsmount *mnt)
{
	struct vfsmount *p;
=======
static inline int tree_contains_unbindable(struct mount *mnt)
{
	struct mount *p;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	else if (!capable(CAP_SYS_ADMIN))
		err = -EPERM;
	else
		err = do_remount_sb(sb, flags, data, 0);
	if (!err) {
		lock_mount_hash();
		mnt_flags |= mnt->mnt.mnt_flags & MNT_PROPAGATION_MASK;
		mnt->mnt.mnt_flags = mnt_flags;
		touch_mnt_namespace(mnt->mnt_ns);
		unlock_mount_hash();
	}
	up_write(&sb->s_umount);
	return err;
}

static inline int tree_contains_unbindable(struct mount *mnt)
{
	struct mount *p;
>>>>>>> refs/remotes/origin/master
	for (p = mnt; p; p = next_mnt(p, mnt)) {
		if (IS_MNT_UNBINDABLE(p))
			return 1;
	}
	return 0;
}

<<<<<<< HEAD
static int do_move_mount(struct path *path, char *old_name)
{
	struct path old_path, parent_path;
<<<<<<< HEAD
	struct vfsmount *p;
=======
	struct mount *p;
	struct mount *old;
>>>>>>> refs/remotes/origin/cm-10.0
	int err = 0;
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
=======
static int do_move_mount(struct path *path, const char *old_name)
{
	struct path old_path, parent_path;
	struct mount *p;
	struct mount *old;
	struct mountpoint *mp;
	int err;
>>>>>>> refs/remotes/origin/master
	if (!old_name || !*old_name)
		return -EINVAL;
	err = kern_path(old_name, LOOKUP_FOLLOW, &old_path);
	if (err)
		return err;

<<<<<<< HEAD
	err = lock_mount(path);
	if (err < 0)
		goto out;

<<<<<<< HEAD
	err = -EINVAL;
	if (!check_mnt(path->mnt) || !check_mnt(old_path.mnt))
=======
=======
	mp = lock_mount(path);
	err = PTR_ERR(mp);
	if (IS_ERR(mp))
		goto out;

>>>>>>> refs/remotes/origin/master
	old = real_mount(old_path.mnt);
	p = real_mount(path->mnt);

	err = -EINVAL;
	if (!check_mnt(p) || !check_mnt(old))
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		goto out1;

	if (d_unlinked(path->dentry))
=======
		goto out1;

	if (old->mnt.mnt_flags & MNT_LOCKED)
>>>>>>> refs/remotes/origin/master
		goto out1;

	err = -EINVAL;
	if (old_path.dentry != old_path.mnt->mnt_root)
		goto out1;

<<<<<<< HEAD
<<<<<<< HEAD
	if (old_path.mnt == old_path.mnt->mnt_parent)
=======
	if (!mnt_has_parent(old))
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!mnt_has_parent(old))
>>>>>>> refs/remotes/origin/master
		goto out1;

	if (S_ISDIR(path->dentry->d_inode->i_mode) !=
	      S_ISDIR(old_path.dentry->d_inode->i_mode))
		goto out1;
	/*
	 * Don't move a mount residing in a shared parent.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (old_path.mnt->mnt_parent &&
	    IS_MNT_SHARED(old_path.mnt->mnt_parent))
=======
	if (IS_MNT_SHARED(old->mnt_parent))
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (IS_MNT_SHARED(old->mnt_parent))
>>>>>>> refs/remotes/origin/master
		goto out1;
	/*
	 * Don't move a mount tree containing unbindable mounts to a destination
	 * mount which is shared.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (IS_MNT_SHARED(path->mnt) &&
	    tree_contains_unbindable(old_path.mnt))
		goto out1;
	err = -ELOOP;
	for (p = path->mnt; p->mnt_parent != p; p = p->mnt_parent)
		if (p == old_path.mnt)
			goto out1;

	err = attach_recursive_mnt(old_path.mnt, path, &parent_path);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (IS_MNT_SHARED(p) && tree_contains_unbindable(old))
		goto out1;
	err = -ELOOP;
	for (; mnt_has_parent(p); p = p->mnt_parent)
		if (p == old)
			goto out1;

<<<<<<< HEAD
	err = attach_recursive_mnt(old, path, &parent_path);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	err = attach_recursive_mnt(old, real_mount(path->mnt), mp, &parent_path);
>>>>>>> refs/remotes/origin/master
	if (err)
		goto out1;

	/* if the mount is moved, it should no longer be expire
	 * automatically */
<<<<<<< HEAD
<<<<<<< HEAD
	list_del_init(&old_path.mnt->mnt_expire);
=======
	list_del_init(&old->mnt_expire);
>>>>>>> refs/remotes/origin/cm-10.0
out1:
	unlock_mount(path);
=======
	list_del_init(&old->mnt_expire);
out1:
	unlock_mount(mp);
>>>>>>> refs/remotes/origin/master
out:
	if (!err)
		path_put(&parent_path);
	path_put(&old_path);
	return err;
}

static struct vfsmount *fs_set_subtype(struct vfsmount *mnt, const char *fstype)
{
	int err;
	const char *subtype = strchr(fstype, '.');
	if (subtype) {
		subtype++;
		err = -EINVAL;
		if (!subtype[0])
			goto err;
	} else
		subtype = "";

	mnt->mnt_sb->s_subtype = kstrdup(subtype, GFP_KERNEL);
	err = -ENOMEM;
	if (!mnt->mnt_sb->s_subtype)
		goto err;
	return mnt;

 err:
	mntput(mnt);
	return ERR_PTR(err);
}

<<<<<<< HEAD
<<<<<<< HEAD
struct vfsmount *
=======
static struct vfsmount *
>>>>>>> refs/remotes/origin/cm-10.0
do_kern_mount(const char *fstype, int flags, const char *name, void *data)
{
	struct file_system_type *type = get_fs_type(fstype);
	struct vfsmount *mnt;
	if (!type)
		return ERR_PTR(-ENODEV);
	mnt = vfs_kern_mount(type, flags, name, data);
	if (!IS_ERR(mnt) && (type->fs_flags & FS_HAS_SUBTYPE) &&
	    !mnt->mnt_sb->s_subtype)
		mnt = fs_set_subtype(mnt, fstype);
	put_filesystem(type);
	return mnt;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(do_kern_mount);
=======
>>>>>>> refs/remotes/origin/cm-10.0

/*
 * add a mount into a namespace's mount tree
 */
<<<<<<< HEAD
static int do_add_mount(struct vfsmount *newmnt, struct path *path, int mnt_flags)
=======
static int do_add_mount(struct mount *newmnt, struct path *path, int mnt_flags)
>>>>>>> refs/remotes/origin/cm-10.0
{
	int err;

	mnt_flags &= ~(MNT_SHARED | MNT_WRITE_HOLD | MNT_INTERNAL);

	err = lock_mount(path);
	if (err)
		return err;

	err = -EINVAL;
<<<<<<< HEAD
	if (!(mnt_flags & MNT_SHRINKABLE) && !check_mnt(path->mnt))
=======
	if (!(mnt_flags & MNT_SHRINKABLE) && !check_mnt(real_mount(path->mnt)))
>>>>>>> refs/remotes/origin/cm-10.0
		goto unlock;

	/* Refuse the same filesystem on the same mount point */
	err = -EBUSY;
<<<<<<< HEAD
	if (path->mnt->mnt_sb == newmnt->mnt_sb &&
=======
	if (path->mnt->mnt_sb == newmnt->mnt.mnt_sb &&
>>>>>>> refs/remotes/origin/cm-10.0
=======
/*
 * add a mount into a namespace's mount tree
 */
static int do_add_mount(struct mount *newmnt, struct path *path, int mnt_flags)
{
	struct mountpoint *mp;
	struct mount *parent;
	int err;

	mnt_flags &= ~(MNT_SHARED | MNT_WRITE_HOLD | MNT_INTERNAL | MNT_DOOMED | MNT_SYNC_UMOUNT);

	mp = lock_mount(path);
	if (IS_ERR(mp))
		return PTR_ERR(mp);

	parent = real_mount(path->mnt);
	err = -EINVAL;
	if (unlikely(!check_mnt(parent))) {
		/* that's acceptable only for automounts done in private ns */
		if (!(mnt_flags & MNT_SHRINKABLE))
			goto unlock;
		/* ... and for those we'd better have mountpoint still alive */
		if (!parent->mnt_ns)
			goto unlock;
	}

	/* Refuse the same filesystem on the same mount point */
	err = -EBUSY;
	if (path->mnt->mnt_sb == newmnt->mnt.mnt_sb &&
>>>>>>> refs/remotes/origin/master
	    path->mnt->mnt_root == path->dentry)
		goto unlock;

	err = -EINVAL;
<<<<<<< HEAD
<<<<<<< HEAD
	if (S_ISLNK(newmnt->mnt_root->d_inode->i_mode))
		goto unlock;

	newmnt->mnt_flags = mnt_flags;
=======
=======
>>>>>>> refs/remotes/origin/master
	if (S_ISLNK(newmnt->mnt.mnt_root->d_inode->i_mode))
		goto unlock;

	newmnt->mnt.mnt_flags = mnt_flags;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	err = graft_tree(newmnt, path);

unlock:
	unlock_mount(path);
=======
	err = graft_tree(newmnt, parent, mp);

unlock:
	unlock_mount(mp);
>>>>>>> refs/remotes/origin/master
	return err;
}

/*
 * create a new mount for userspace and request it to be added into the
 * namespace's tree
 */
<<<<<<< HEAD
static int do_new_mount(struct path *path, char *type, int flags,
			int mnt_flags, char *name, void *data)
{
	struct vfsmount *mnt;
	int err;

	if (!type)
		return -EINVAL;

	/* we need capabilities... */
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	mnt = do_kern_mount(type, flags, name, data);
	if (IS_ERR(mnt))
		return PTR_ERR(mnt);

<<<<<<< HEAD
	err = do_add_mount(mnt, path, mnt_flags);
=======
	err = do_add_mount(real_mount(mnt), path, mnt_flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int do_new_mount(struct path *path, const char *fstype, int flags,
			int mnt_flags, const char *name, void *data)
{
	struct file_system_type *type;
	struct user_namespace *user_ns = current->nsproxy->mnt_ns->user_ns;
	struct vfsmount *mnt;
	int err;

	if (!fstype)
		return -EINVAL;

	type = get_fs_type(fstype);
	if (!type)
		return -ENODEV;

	if (user_ns != &init_user_ns) {
		if (!(type->fs_flags & FS_USERNS_MOUNT)) {
			put_filesystem(type);
			return -EPERM;
		}
		/* Only in special cases allow devices from mounts
		 * created outside the initial user namespace.
		 */
		if (!(type->fs_flags & FS_USERNS_DEV_MOUNT)) {
			flags |= MS_NODEV;
			mnt_flags |= MNT_NODEV;
		}
	}

	mnt = vfs_kern_mount(type, flags, name, data);
	if (!IS_ERR(mnt) && (type->fs_flags & FS_HAS_SUBTYPE) &&
	    !mnt->mnt_sb->s_subtype)
		mnt = fs_set_subtype(mnt, fstype);

	put_filesystem(type);
	if (IS_ERR(mnt))
		return PTR_ERR(mnt);

	err = do_add_mount(real_mount(mnt), path, mnt_flags);
>>>>>>> refs/remotes/origin/master
	if (err)
		mntput(mnt);
	return err;
}

int finish_automount(struct vfsmount *m, struct path *path)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct mount *mnt = real_mount(m);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct mount *mnt = real_mount(m);
>>>>>>> refs/remotes/origin/master
	int err;
	/* The new mount record should have at least 2 refs to prevent it being
	 * expired before we get a chance to add it
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	BUG_ON(mnt_get_count(m) < 2);
=======
	BUG_ON(mnt_get_count(mnt) < 2);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	BUG_ON(mnt_get_count(mnt) < 2);
>>>>>>> refs/remotes/origin/master

	if (m->mnt_sb == path->mnt->mnt_sb &&
	    m->mnt_root == path->dentry) {
		err = -ELOOP;
		goto fail;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	err = do_add_mount(m, path, path->mnt->mnt_flags | MNT_SHRINKABLE);
=======
	err = do_add_mount(mnt, path, path->mnt->mnt_flags | MNT_SHRINKABLE);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	err = do_add_mount(mnt, path, path->mnt->mnt_flags | MNT_SHRINKABLE);
>>>>>>> refs/remotes/origin/master
	if (!err)
		return 0;
fail:
	/* remove m from any expiration list it may be on */
<<<<<<< HEAD
<<<<<<< HEAD
	if (!list_empty(&m->mnt_expire)) {
		down_write(&namespace_sem);
		br_write_lock(vfsmount_lock);
		list_del_init(&m->mnt_expire);
=======
	if (!list_empty(&mnt->mnt_expire)) {
		down_write(&namespace_sem);
		br_write_lock(vfsmount_lock);
		list_del_init(&mnt->mnt_expire);
>>>>>>> refs/remotes/origin/cm-10.0
		br_write_unlock(vfsmount_lock);
		up_write(&namespace_sem);
=======
	if (!list_empty(&mnt->mnt_expire)) {
		namespace_lock();
		list_del_init(&mnt->mnt_expire);
		namespace_unlock();
>>>>>>> refs/remotes/origin/master
	}
	mntput(m);
	mntput(m);
	return err;
}

/**
 * mnt_set_expiry - Put a mount on an expiration list
 * @mnt: The mount to list.
 * @expiry_list: The list to add the mount to.
 */
void mnt_set_expiry(struct vfsmount *mnt, struct list_head *expiry_list)
{
<<<<<<< HEAD
	down_write(&namespace_sem);
	br_write_lock(vfsmount_lock);

<<<<<<< HEAD
	list_add_tail(&mnt->mnt_expire, expiry_list);
=======
	list_add_tail(&real_mount(mnt)->mnt_expire, expiry_list);
>>>>>>> refs/remotes/origin/cm-10.0

	br_write_unlock(vfsmount_lock);
	up_write(&namespace_sem);
=======
	namespace_lock();

	list_add_tail(&real_mount(mnt)->mnt_expire, expiry_list);

	namespace_unlock();
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(mnt_set_expiry);

/*
 * process a list of expirable mountpoints with the intent of discarding any
 * mountpoints that aren't in use and haven't been touched since last we came
 * here
 */
void mark_mounts_for_expiry(struct list_head *mounts)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct vfsmount *mnt, *next;
=======
	struct mount *mnt, *next;
>>>>>>> refs/remotes/origin/cm-10.0
	LIST_HEAD(graveyard);
	LIST_HEAD(umounts);
=======
	struct mount *mnt, *next;
	LIST_HEAD(graveyard);
>>>>>>> refs/remotes/origin/master

	if (list_empty(mounts))
		return;

<<<<<<< HEAD
	down_write(&namespace_sem);
	br_write_lock(vfsmount_lock);
=======
	namespace_lock();
	lock_mount_hash();
>>>>>>> refs/remotes/origin/master

	/* extract from the expiration list every vfsmount that matches the
	 * following criteria:
	 * - only referenced by its parent vfsmount
	 * - still marked for expiry (marked on the last call here; marks are
	 *   cleared by mntput())
	 */
	list_for_each_entry_safe(mnt, next, mounts, mnt_expire) {
		if (!xchg(&mnt->mnt_expiry_mark, 1) ||
			propagate_mount_busy(mnt, 1))
			continue;
		list_move(&mnt->mnt_expire, &graveyard);
	}
	while (!list_empty(&graveyard)) {
<<<<<<< HEAD
<<<<<<< HEAD
		mnt = list_first_entry(&graveyard, struct vfsmount, mnt_expire);
=======
		mnt = list_first_entry(&graveyard, struct mount, mnt_expire);
>>>>>>> refs/remotes/origin/cm-10.0
		touch_mnt_namespace(mnt->mnt_ns);
		umount_tree(mnt, 1, &umounts);
	}
	br_write_unlock(vfsmount_lock);
	up_write(&namespace_sem);

	release_mounts(&umounts);
=======
		mnt = list_first_entry(&graveyard, struct mount, mnt_expire);
		touch_mnt_namespace(mnt->mnt_ns);
		umount_tree(mnt, 1);
	}
	unlock_mount_hash();
	namespace_unlock();
>>>>>>> refs/remotes/origin/master
}

EXPORT_SYMBOL_GPL(mark_mounts_for_expiry);

/*
 * Ripoff of 'select_parent()'
 *
 * search the list of submounts for a given mountpoint, and move any
 * shrinkable submounts to the 'graveyard' list.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int select_submounts(struct vfsmount *parent, struct list_head *graveyard)
{
	struct vfsmount *this_parent = parent;
=======
static int select_submounts(struct mount *parent, struct list_head *graveyard)
{
	struct mount *this_parent = parent;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int select_submounts(struct mount *parent, struct list_head *graveyard)
{
	struct mount *this_parent = parent;
>>>>>>> refs/remotes/origin/master
	struct list_head *next;
	int found = 0;

repeat:
	next = this_parent->mnt_mounts.next;
resume:
	while (next != &this_parent->mnt_mounts) {
		struct list_head *tmp = next;
<<<<<<< HEAD
<<<<<<< HEAD
		struct vfsmount *mnt = list_entry(tmp, struct vfsmount, mnt_child);

		next = tmp->next;
		if (!(mnt->mnt_flags & MNT_SHRINKABLE))
=======
=======
>>>>>>> refs/remotes/origin/master
		struct mount *mnt = list_entry(tmp, struct mount, mnt_child);

		next = tmp->next;
		if (!(mnt->mnt.mnt_flags & MNT_SHRINKABLE))
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			continue;
		/*
		 * Descend a level if the d_mounts list is non-empty.
		 */
		if (!list_empty(&mnt->mnt_mounts)) {
			this_parent = mnt;
			goto repeat;
		}

		if (!propagate_mount_busy(mnt, 1)) {
			list_move_tail(&mnt->mnt_expire, graveyard);
			found++;
		}
	}
	/*
	 * All done at this level ... ascend and resume the search
	 */
	if (this_parent != parent) {
		next = this_parent->mnt_child.next;
		this_parent = this_parent->mnt_parent;
		goto resume;
	}
	return found;
}

/*
 * process a list of expirable mountpoints with the intent of discarding any
 * submounts of a specific parent mountpoint
 *
<<<<<<< HEAD
 * vfsmount_lock must be held for write
 */
<<<<<<< HEAD
static void shrink_submounts(struct vfsmount *mnt, struct list_head *umounts)
{
	LIST_HEAD(graveyard);
	struct vfsmount *m;
=======
static void shrink_submounts(struct mount *mnt, struct list_head *umounts)
{
	LIST_HEAD(graveyard);
	struct mount *m;
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * mount_lock must be held for write
 */
static void shrink_submounts(struct mount *mnt)
{
	LIST_HEAD(graveyard);
	struct mount *m;
>>>>>>> refs/remotes/origin/master

	/* extract submounts of 'mountpoint' from the expiration list */
	while (select_submounts(mnt, &graveyard)) {
		while (!list_empty(&graveyard)) {
<<<<<<< HEAD
<<<<<<< HEAD
			m = list_first_entry(&graveyard, struct vfsmount,
=======
			m = list_first_entry(&graveyard, struct mount,
>>>>>>> refs/remotes/origin/cm-10.0
						mnt_expire);
			touch_mnt_namespace(m->mnt_ns);
			umount_tree(m, 1, umounts);
=======
			m = list_first_entry(&graveyard, struct mount,
						mnt_expire);
			touch_mnt_namespace(m->mnt_ns);
			umount_tree(m, 1);
>>>>>>> refs/remotes/origin/master
		}
	}
}

/*
 * Some copy_from_user() implementations do not return the exact number of
 * bytes remaining to copy on a fault.  But copy_mount_options() requires that.
 * Note that this function differs from copy_from_user() in that it will oops
 * on bad values of `to', rather than returning a short copy.
 */
static long exact_copy_from_user(void *to, const void __user * from,
				 unsigned long n)
{
	char *t = to;
	const char __user *f = from;
	char c;

	if (!access_ok(VERIFY_READ, from, n))
		return n;

	while (n) {
		if (__get_user(c, f)) {
			memset(t, 0, n);
			break;
		}
		*t++ = c;
		f++;
		n--;
	}
	return n;
}

int copy_mount_options(const void __user * data, unsigned long *where)
{
	int i;
	unsigned long page;
	unsigned long size;

	*where = 0;
	if (!data)
		return 0;

	if (!(page = __get_free_page(GFP_KERNEL)))
		return -ENOMEM;

	/* We only care that *some* data at the address the user
	 * gave us is valid.  Just in case, we'll zero
	 * the remainder of the page.
	 */
	/* copy_from_user cannot cross TASK_SIZE ! */
	size = TASK_SIZE - (unsigned long)data;
	if (size > PAGE_SIZE)
		size = PAGE_SIZE;

	i = size - exact_copy_from_user((void *)page, data, size);
	if (!i) {
		free_page(page);
		return -EFAULT;
	}
	if (i != PAGE_SIZE)
		memset((char *)page + i, 0, PAGE_SIZE - i);
	*where = page;
	return 0;
}

int copy_mount_string(const void __user *data, char **where)
{
	char *tmp;

	if (!data) {
		*where = NULL;
		return 0;
	}

	tmp = strndup_user(data, PAGE_SIZE);
	if (IS_ERR(tmp))
		return PTR_ERR(tmp);

	*where = tmp;
	return 0;
}

/*
 * Flags is a 32-bit value that allows up to 31 non-fs dependent flags to
 * be given to the mount() call (ie: read-only, no-dev, no-suid etc).
 *
 * data is a (void *) that can point to any structure up to
 * PAGE_SIZE-1 bytes, which can contain arbitrary fs-dependent
 * information (or be NULL).
 *
 * Pre-0.97 versions of mount() didn't have a flags word.
 * When the flags word was introduced its top half was required
 * to have the magic value 0xC0ED, and this remained so until 2.4.0-test9.
 * Therefore, if this magic number is present, it carries no information
 * and must be discarded.
 */
<<<<<<< HEAD
long do_mount(char *dev_name, char *dir_name, char *type_page,
		  unsigned long flags, void *data_page)
=======
long do_mount(const char *dev_name, const char *dir_name,
		const char *type_page, unsigned long flags, void *data_page)
>>>>>>> refs/remotes/origin/master
{
	struct path path;
	int retval = 0;
	int mnt_flags = 0;

	/* Discard magic */
	if ((flags & MS_MGC_MSK) == MS_MGC_VAL)
		flags &= ~MS_MGC_MSK;

	/* Basic sanity checks */

	if (!dir_name || !*dir_name || !memchr(dir_name, 0, PAGE_SIZE))
		return -EINVAL;

	if (data_page)
		((char *)data_page)[PAGE_SIZE - 1] = 0;

	/* ... and get the mountpoint */
	retval = kern_path(dir_name, LOOKUP_FOLLOW, &path);
	if (retval)
		return retval;

	retval = security_sb_mount(dev_name, &path,
				   type_page, flags, data_page);
<<<<<<< HEAD
=======
	if (!retval && !may_mount())
		retval = -EPERM;
>>>>>>> refs/remotes/origin/master
	if (retval)
		goto dput_out;

	/* Default to relatime unless overriden */
	if (!(flags & MS_NOATIME))
		mnt_flags |= MNT_RELATIME;

	/* Separate the per-mountpoint flags */
	if (flags & MS_NOSUID)
		mnt_flags |= MNT_NOSUID;
	if (flags & MS_NODEV)
		mnt_flags |= MNT_NODEV;
	if (flags & MS_NOEXEC)
		mnt_flags |= MNT_NOEXEC;
	if (flags & MS_NOATIME)
		mnt_flags |= MNT_NOATIME;
	if (flags & MS_NODIRATIME)
		mnt_flags |= MNT_NODIRATIME;
	if (flags & MS_STRICTATIME)
		mnt_flags &= ~(MNT_RELATIME | MNT_NOATIME);
	if (flags & MS_RDONLY)
		mnt_flags |= MNT_READONLY;

	flags &= ~(MS_NOSUID | MS_NOEXEC | MS_NODEV | MS_ACTIVE | MS_BORN |
		   MS_NOATIME | MS_NODIRATIME | MS_RELATIME| MS_KERNMOUNT |
		   MS_STRICTATIME);

	if (flags & MS_REMOUNT)
		retval = do_remount(&path, flags & ~MS_REMOUNT, mnt_flags,
				    data_page);
	else if (flags & MS_BIND)
		retval = do_loopback(&path, dev_name, flags & MS_REC);
	else if (flags & (MS_SHARED | MS_PRIVATE | MS_SLAVE | MS_UNBINDABLE))
		retval = do_change_type(&path, flags);
	else if (flags & MS_MOVE)
		retval = do_move_mount(&path, dev_name);
	else
		retval = do_new_mount(&path, type_page, flags, mnt_flags,
				      dev_name, data_page);
dput_out:
	path_put(&path);
	return retval;
}

<<<<<<< HEAD
static struct mnt_namespace *alloc_mnt_ns(void)
{
	struct mnt_namespace *new_ns;
=======
static void free_mnt_ns(struct mnt_namespace *ns)
{
	proc_free_inum(ns->proc_inum);
	put_user_ns(ns->user_ns);
	kfree(ns);
}

/*
 * Assign a sequence number so we can detect when we attempt to bind
 * mount a reference to an older mount namespace into the current
 * mount namespace, preventing reference counting loops.  A 64bit
 * number incrementing at 10Ghz will take 12,427 years to wrap which
 * is effectively never, so we can ignore the possibility.
 */
static atomic64_t mnt_ns_seq = ATOMIC64_INIT(1);

static struct mnt_namespace *alloc_mnt_ns(struct user_namespace *user_ns)
{
	struct mnt_namespace *new_ns;
	int ret;
>>>>>>> refs/remotes/origin/master

	new_ns = kmalloc(sizeof(struct mnt_namespace), GFP_KERNEL);
	if (!new_ns)
		return ERR_PTR(-ENOMEM);
<<<<<<< HEAD
=======
	ret = proc_alloc_inum(&new_ns->proc_inum);
	if (ret) {
		kfree(new_ns);
		return ERR_PTR(ret);
	}
	new_ns->seq = atomic64_add_return(1, &mnt_ns_seq);
>>>>>>> refs/remotes/origin/master
	atomic_set(&new_ns->count, 1);
	new_ns->root = NULL;
	INIT_LIST_HEAD(&new_ns->list);
	init_waitqueue_head(&new_ns->poll);
	new_ns->event = 0;
<<<<<<< HEAD
	return new_ns;
}

void mnt_make_longterm(struct vfsmount *mnt)
{
<<<<<<< HEAD
	__mnt_make_longterm(mnt);
}

void mnt_make_shortterm(struct vfsmount *mnt)
{
#ifdef CONFIG_SMP
=======
	__mnt_make_longterm(real_mount(mnt));
}

void mnt_make_shortterm(struct vfsmount *m)
{
#ifdef CONFIG_SMP
	struct mount *mnt = real_mount(m);
>>>>>>> refs/remotes/origin/cm-10.0
	if (atomic_add_unless(&mnt->mnt_longterm, -1, 1))
		return;
	br_write_lock(vfsmount_lock);
	atomic_dec(&mnt->mnt_longterm);
	br_write_unlock(vfsmount_lock);
#endif
}

/*
 * Allocate a new namespace structure and populate it with contents
 * copied from the namespace of the passed in task structure.
 */
static struct mnt_namespace *dup_mnt_ns(struct mnt_namespace *mnt_ns,
		struct fs_struct *fs)
{
	struct mnt_namespace *new_ns;
	struct vfsmount *rootmnt = NULL, *pwdmnt = NULL;
<<<<<<< HEAD
	struct vfsmount *p, *q;
=======
	struct mount *p, *q;
	struct mount *old = mnt_ns->root;
	struct mount *new;
>>>>>>> refs/remotes/origin/cm-10.0

	new_ns = alloc_mnt_ns();
	if (IS_ERR(new_ns))
		return new_ns;

	down_write(&namespace_sem);
	/* First pass: copy the tree topology */
<<<<<<< HEAD
	new_ns->root = copy_tree(mnt_ns->root, mnt_ns->root->mnt_root,
					CL_COPY_ALL | CL_EXPIRE);
	if (!new_ns->root) {
=======
	new = copy_tree(old, old->mnt.mnt_root, CL_COPY_ALL | CL_EXPIRE);
	if (!new) {
>>>>>>> refs/remotes/origin/cm-10.0
		up_write(&namespace_sem);
		kfree(new_ns);
		return ERR_PTR(-ENOMEM);
	}
<<<<<<< HEAD
	br_write_lock(vfsmount_lock);
	list_add_tail(&new_ns->list, &new_ns->root->mnt_list);
=======
	new_ns->root = new;
	br_write_lock(vfsmount_lock);
	list_add_tail(&new_ns->list, &new->mnt_list);
>>>>>>> refs/remotes/origin/cm-10.0
	br_write_unlock(vfsmount_lock);
=======
	new_ns->user_ns = get_user_ns(user_ns);
	return new_ns;
}

struct mnt_namespace *copy_mnt_ns(unsigned long flags, struct mnt_namespace *ns,
		struct user_namespace *user_ns, struct fs_struct *new_fs)
{
	struct mnt_namespace *new_ns;
	struct vfsmount *rootmnt = NULL, *pwdmnt = NULL;
	struct mount *p, *q;
	struct mount *old;
	struct mount *new;
	int copy_flags;

	BUG_ON(!ns);

	if (likely(!(flags & CLONE_NEWNS))) {
		get_mnt_ns(ns);
		return ns;
	}

	old = ns->root;

	new_ns = alloc_mnt_ns(user_ns);
	if (IS_ERR(new_ns))
		return new_ns;

	namespace_lock();
	/* First pass: copy the tree topology */
	copy_flags = CL_COPY_UNBINDABLE | CL_EXPIRE;
	if (user_ns != ns->user_ns)
		copy_flags |= CL_SHARED_TO_SLAVE | CL_UNPRIVILEGED;
	new = copy_tree(old, old->mnt.mnt_root, copy_flags);
	if (IS_ERR(new)) {
		namespace_unlock();
		free_mnt_ns(new_ns);
		return ERR_CAST(new);
	}
	new_ns->root = new;
	list_add_tail(&new_ns->list, &new->mnt_list);
>>>>>>> refs/remotes/origin/master

	/*
	 * Second pass: switch the tsk->fs->* elements and mark new vfsmounts
	 * as belonging to new namespace.  We have already acquired a private
	 * fs_struct, so tsk->fs->lock is not needed.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	p = mnt_ns->root;
	q = new_ns->root;
=======
	p = old;
	q = new;
>>>>>>> refs/remotes/origin/cm-10.0
	while (p) {
		q->mnt_ns = new_ns;
		__mnt_make_longterm(q);
		if (fs) {
<<<<<<< HEAD
			if (p == fs->root.mnt) {
				fs->root.mnt = mntget(q);
				__mnt_make_longterm(q);
				mnt_make_shortterm(p);
				rootmnt = p;
			}
			if (p == fs->pwd.mnt) {
				fs->pwd.mnt = mntget(q);
				__mnt_make_longterm(q);
				mnt_make_shortterm(p);
				pwdmnt = p;
			}
		}
		p = next_mnt(p, mnt_ns->root);
		q = next_mnt(q, new_ns->root);
=======
			if (&p->mnt == fs->root.mnt) {
				fs->root.mnt = mntget(&q->mnt);
				__mnt_make_longterm(q);
				mnt_make_shortterm(&p->mnt);
				rootmnt = &p->mnt;
			}
			if (&p->mnt == fs->pwd.mnt) {
				fs->pwd.mnt = mntget(&q->mnt);
				__mnt_make_longterm(q);
				mnt_make_shortterm(&p->mnt);
=======
	p = old;
	q = new;
	while (p) {
		q->mnt_ns = new_ns;
		if (new_fs) {
			if (&p->mnt == new_fs->root.mnt) {
				new_fs->root.mnt = mntget(&q->mnt);
				rootmnt = &p->mnt;
			}
			if (&p->mnt == new_fs->pwd.mnt) {
				new_fs->pwd.mnt = mntget(&q->mnt);
>>>>>>> refs/remotes/origin/master
				pwdmnt = &p->mnt;
			}
		}
		p = next_mnt(p, old);
		q = next_mnt(q, new);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	}
	up_write(&namespace_sem);
=======
		if (!q)
			break;
		while (p->mnt.mnt_root != q->mnt.mnt_root)
			p = next_mnt(p, old);
	}
	namespace_unlock();
>>>>>>> refs/remotes/origin/master

	if (rootmnt)
		mntput(rootmnt);
	if (pwdmnt)
		mntput(pwdmnt);

	return new_ns;
}

<<<<<<< HEAD
struct mnt_namespace *copy_mnt_ns(unsigned long flags, struct mnt_namespace *ns,
		struct fs_struct *new_fs)
{
	struct mnt_namespace *new_ns;

	BUG_ON(!ns);
	get_mnt_ns(ns);

	if (!(flags & CLONE_NEWNS))
		return ns;

	new_ns = dup_mnt_ns(ns, new_fs);

	put_mnt_ns(ns);
	return new_ns;
}

=======
>>>>>>> refs/remotes/origin/master
/**
 * create_mnt_ns - creates a private namespace and adds a root filesystem
 * @mnt: pointer to the new root filesystem mountpoint
 */
<<<<<<< HEAD
<<<<<<< HEAD
struct mnt_namespace *create_mnt_ns(struct vfsmount *mnt)
{
	struct mnt_namespace *new_ns;

	new_ns = alloc_mnt_ns();
	if (!IS_ERR(new_ns)) {
		mnt->mnt_ns = new_ns;
		__mnt_make_longterm(mnt);
		new_ns->root = mnt;
		list_add(&new_ns->list, &new_ns->root->mnt_list);
	}
	return new_ns;
}
EXPORT_SYMBOL(create_mnt_ns);
=======
static struct mnt_namespace *create_mnt_ns(struct vfsmount *m)
{
	struct mnt_namespace *new_ns = alloc_mnt_ns();
	if (!IS_ERR(new_ns)) {
		struct mount *mnt = real_mount(m);
		mnt->mnt_ns = new_ns;
		__mnt_make_longterm(mnt);
		new_ns->root = mnt;
		list_add(&new_ns->list, &mnt->mnt_list);
=======
static struct mnt_namespace *create_mnt_ns(struct vfsmount *m)
{
	struct mnt_namespace *new_ns = alloc_mnt_ns(&init_user_ns);
	if (!IS_ERR(new_ns)) {
		struct mount *mnt = real_mount(m);
		mnt->mnt_ns = new_ns;
		new_ns->root = mnt;
		list_add(&mnt->mnt_list, &new_ns->list);
>>>>>>> refs/remotes/origin/master
	} else {
		mntput(m);
	}
	return new_ns;
}

struct dentry *mount_subtree(struct vfsmount *mnt, const char *name)
{
	struct mnt_namespace *ns;
	struct super_block *s;
	struct path path;
	int err;

	ns = create_mnt_ns(mnt);
	if (IS_ERR(ns))
		return ERR_CAST(ns);

	err = vfs_path_lookup(mnt->mnt_root, mnt,
			name, LOOKUP_FOLLOW|LOOKUP_AUTOMOUNT, &path);

	put_mnt_ns(ns);

	if (err)
		return ERR_PTR(err);

	/* trade a vfsmount reference for active sb one */
	s = path.mnt->mnt_sb;
	atomic_inc(&s->s_active);
	mntput(path.mnt);
	/* lock the sucker */
	down_write(&s->s_umount);
	/* ... and return the root of (sub)tree on it */
	return path.dentry;
}
EXPORT_SYMBOL(mount_subtree);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

SYSCALL_DEFINE5(mount, char __user *, dev_name, char __user *, dir_name,
		char __user *, type, unsigned long, flags, void __user *, data)
{
	int ret;
	char *kernel_type;
<<<<<<< HEAD
	char *kernel_dir;
=======
	struct filename *kernel_dir;
>>>>>>> refs/remotes/origin/master
	char *kernel_dev;
	unsigned long data_page;

	ret = copy_mount_string(type, &kernel_type);
	if (ret < 0)
		goto out_type;

	kernel_dir = getname(dir_name);
	if (IS_ERR(kernel_dir)) {
		ret = PTR_ERR(kernel_dir);
		goto out_dir;
	}

	ret = copy_mount_string(dev_name, &kernel_dev);
	if (ret < 0)
		goto out_dev;

	ret = copy_mount_options(data, &data_page);
	if (ret < 0)
		goto out_data;

<<<<<<< HEAD
	ret = do_mount(kernel_dev, kernel_dir, kernel_type, flags,
=======
	ret = do_mount(kernel_dev, kernel_dir->name, kernel_type, flags,
>>>>>>> refs/remotes/origin/master
		(void *) data_page);

	free_page(data_page);
out_data:
	kfree(kernel_dev);
out_dev:
	putname(kernel_dir);
out_dir:
	kfree(kernel_type);
out_type:
	return ret;
}

/*
<<<<<<< HEAD
<<<<<<< HEAD
=======
 * Return true if path is reachable from root
 *
 * namespace_sem or vfsmount_lock is held
=======
 * Return true if path is reachable from root
 *
 * namespace_sem or mount_lock is held
>>>>>>> refs/remotes/origin/master
 */
bool is_path_reachable(struct mount *mnt, struct dentry *dentry,
			 const struct path *root)
{
	while (&mnt->mnt != root->mnt && mnt_has_parent(mnt)) {
		dentry = mnt->mnt_mountpoint;
		mnt = mnt->mnt_parent;
	}
	return &mnt->mnt == root->mnt && is_subdir(dentry, root->dentry);
}

int path_is_under(struct path *path1, struct path *path2)
{
	int res;
<<<<<<< HEAD
	br_read_lock(vfsmount_lock);
	res = is_path_reachable(real_mount(path1->mnt), path1->dentry, path2);
	br_read_unlock(vfsmount_lock);
=======
	read_seqlock_excl(&mount_lock);
	res = is_path_reachable(real_mount(path1->mnt), path1->dentry, path2);
	read_sequnlock_excl(&mount_lock);
>>>>>>> refs/remotes/origin/master
	return res;
}
EXPORT_SYMBOL(path_is_under);

/*
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * pivot_root Semantics:
 * Moves the root file system of the current process to the directory put_old,
 * makes new_root as the new root file system of the current process, and sets
 * root/cwd of all processes which had them on the current root to new_root.
 *
 * Restrictions:
 * The new_root and put_old must be directories, and  must not be on the
 * same file  system as the current process root. The put_old  must  be
 * underneath new_root,  i.e. adding a non-zero number of /.. to the string
 * pointed to by put_old must yield the same directory as new_root. No other
 * file system may be mounted on put_old. After all, new_root is a mountpoint.
 *
 * Also, the current root cannot be on the 'rootfs' (initial ramfs) filesystem.
 * See Documentation/filesystems/ramfs-rootfs-initramfs.txt for alternatives
 * in this situation.
 *
 * Notes:
 *  - we don't move root/cwd if they are not at the root (reason: if something
 *    cared enough to change them, it's probably wrong to force them elsewhere)
 *  - it's okay to pick a root that isn't the root of a file system, e.g.
 *    /nfs/my_root where /nfs is the mount point. It must be a mountpoint,
 *    though, so you may need to say mount --bind /nfs/my_root /nfs/my_root
 *    first.
 */
SYSCALL_DEFINE2(pivot_root, const char __user *, new_root,
		const char __user *, put_old)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct vfsmount *tmp;
	struct path new, old, parent_path, root_parent, root;
=======
	struct path new, old, parent_path, root_parent, root;
	struct mount *new_mnt, *root_mnt;
>>>>>>> refs/remotes/origin/cm-10.0
	int error;

	if (!capable(CAP_SYS_ADMIN))
=======
	struct path new, old, parent_path, root_parent, root;
	struct mount *new_mnt, *root_mnt, *old_mnt;
	struct mountpoint *old_mp, *root_mp;
	int error;

	if (!may_mount())
>>>>>>> refs/remotes/origin/master
		return -EPERM;

	error = user_path_dir(new_root, &new);
	if (error)
		goto out0;

	error = user_path_dir(put_old, &old);
	if (error)
		goto out1;

	error = security_sb_pivotroot(&old, &new);
	if (error)
		goto out2;

	get_fs_root(current->fs, &root);
<<<<<<< HEAD
	error = lock_mount(&old);
	if (error)
		goto out3;

	error = -EINVAL;
<<<<<<< HEAD
	if (IS_MNT_SHARED(old.mnt) ||
		IS_MNT_SHARED(new.mnt->mnt_parent) ||
		IS_MNT_SHARED(root.mnt->mnt_parent))
		goto out4;
	if (!check_mnt(root.mnt) || !check_mnt(new.mnt))
=======
	new_mnt = real_mount(new.mnt);
	root_mnt = real_mount(root.mnt);
	if (IS_MNT_SHARED(real_mount(old.mnt)) ||
=======
	old_mp = lock_mount(&old);
	error = PTR_ERR(old_mp);
	if (IS_ERR(old_mp))
		goto out3;

	error = -EINVAL;
	new_mnt = real_mount(new.mnt);
	root_mnt = real_mount(root.mnt);
	old_mnt = real_mount(old.mnt);
	if (IS_MNT_SHARED(old_mnt) ||
>>>>>>> refs/remotes/origin/master
		IS_MNT_SHARED(new_mnt->mnt_parent) ||
		IS_MNT_SHARED(root_mnt->mnt_parent))
		goto out4;
	if (!check_mnt(root_mnt) || !check_mnt(new_mnt))
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
		goto out4;
	if (new_mnt->mnt.mnt_flags & MNT_LOCKED)
>>>>>>> refs/remotes/origin/master
		goto out4;
	error = -ENOENT;
	if (d_unlinked(new.dentry))
		goto out4;
<<<<<<< HEAD
	if (d_unlinked(old.dentry))
		goto out4;
	error = -EBUSY;
	if (new.mnt == root.mnt ||
	    old.mnt == root.mnt)
=======
	error = -EBUSY;
	if (new_mnt == root_mnt || old_mnt == root_mnt)
>>>>>>> refs/remotes/origin/master
		goto out4; /* loop, on the same file system  */
	error = -EINVAL;
	if (root.mnt->mnt_root != root.dentry)
		goto out4; /* not a mountpoint */
<<<<<<< HEAD
<<<<<<< HEAD
	if (root.mnt->mnt_parent == root.mnt)
		goto out4; /* not attached */
	if (new.mnt->mnt_root != new.dentry)
		goto out4; /* not a mountpoint */
	if (new.mnt->mnt_parent == new.mnt)
		goto out4; /* not attached */
	/* make sure we can reach put_old from new_root */
	tmp = old.mnt;
	if (tmp != new.mnt) {
		for (;;) {
			if (tmp->mnt_parent == tmp)
				goto out4; /* already mounted on put_old */
			if (tmp->mnt_parent == new.mnt)
				break;
			tmp = tmp->mnt_parent;
		}
		if (!is_subdir(tmp->mnt_mountpoint, new.dentry))
			goto out4;
	} else if (!is_subdir(old.dentry, new.dentry))
		goto out4;
	br_write_lock(vfsmount_lock);
	detach_mnt(new.mnt, &parent_path);
	detach_mnt(root.mnt, &root_parent);
	/* mount old root on put_old */
	attach_mnt(root.mnt, &old);
	/* mount new_root on / */
	attach_mnt(new.mnt, &root_parent);
=======
	if (!mnt_has_parent(root_mnt))
		goto out4; /* not attached */
=======
	if (!mnt_has_parent(root_mnt))
		goto out4; /* not attached */
	root_mp = root_mnt->mnt_mp;
>>>>>>> refs/remotes/origin/master
	if (new.mnt->mnt_root != new.dentry)
		goto out4; /* not a mountpoint */
	if (!mnt_has_parent(new_mnt))
		goto out4; /* not attached */
	/* make sure we can reach put_old from new_root */
<<<<<<< HEAD
	if (!is_path_reachable(real_mount(old.mnt), old.dentry, &new))
		goto out4;
	br_write_lock(vfsmount_lock);
	detach_mnt(new_mnt, &parent_path);
	detach_mnt(root_mnt, &root_parent);
	/* mount old root on put_old */
	attach_mnt(root_mnt, &old);
	/* mount new_root on / */
	attach_mnt(new_mnt, &root_parent);
>>>>>>> refs/remotes/origin/cm-10.0
	touch_mnt_namespace(current->nsproxy->mnt_ns);
	br_write_unlock(vfsmount_lock);
	chroot_fs_refs(&root, &new);
	error = 0;
out4:
	unlock_mount(&old);
=======
	if (!is_path_reachable(old_mnt, old.dentry, &new))
		goto out4;
	root_mp->m_count++; /* pin it so it won't go away */
	lock_mount_hash();
	detach_mnt(new_mnt, &parent_path);
	detach_mnt(root_mnt, &root_parent);
	if (root_mnt->mnt.mnt_flags & MNT_LOCKED) {
		new_mnt->mnt.mnt_flags |= MNT_LOCKED;
		root_mnt->mnt.mnt_flags &= ~MNT_LOCKED;
	}
	/* mount old root on put_old */
	attach_mnt(root_mnt, old_mnt, old_mp);
	/* mount new_root on / */
	attach_mnt(new_mnt, real_mount(root_parent.mnt), root_mp);
	touch_mnt_namespace(current->nsproxy->mnt_ns);
	unlock_mount_hash();
	chroot_fs_refs(&root, &new);
	put_mountpoint(root_mp);
	error = 0;
out4:
	unlock_mount(old_mp);
>>>>>>> refs/remotes/origin/master
	if (!error) {
		path_put(&root_parent);
		path_put(&parent_path);
	}
out3:
	path_put(&root);
out2:
	path_put(&old);
out1:
	path_put(&new);
out0:
	return error;
}

static void __init init_mount_tree(void)
{
	struct vfsmount *mnt;
	struct mnt_namespace *ns;
	struct path root;
<<<<<<< HEAD

	mnt = do_kern_mount("rootfs", 0, "rootfs", NULL);
=======
	struct file_system_type *type;

	type = get_fs_type("rootfs");
	if (!type)
		panic("Can't find rootfs type");
	mnt = vfs_kern_mount(type, 0, "rootfs", NULL);
	put_filesystem(type);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(mnt))
		panic("Can't create rootfs");

	ns = create_mnt_ns(mnt);
	if (IS_ERR(ns))
		panic("Can't allocate initial namespace");

	init_task.nsproxy->mnt_ns = ns;
	get_mnt_ns(ns);

<<<<<<< HEAD
<<<<<<< HEAD
	root.mnt = ns->root;
	root.dentry = ns->root->mnt_root;
=======
	root.mnt = mnt;
	root.dentry = mnt->mnt_root;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	root.mnt = mnt;
	root.dentry = mnt->mnt_root;
>>>>>>> refs/remotes/origin/master

	set_fs_pwd(current->fs, &root);
	set_fs_root(current->fs, &root);
}

void __init mnt_init(void)
{
	unsigned u;
	int err;

<<<<<<< HEAD
	init_rwsem(&namespace_sem);

<<<<<<< HEAD
	mnt_cache = kmem_cache_create("mnt_cache", sizeof(struct vfsmount),
=======
	mnt_cache = kmem_cache_create("mnt_cache", sizeof(struct mount),
>>>>>>> refs/remotes/origin/cm-10.0
			0, SLAB_HWCACHE_ALIGN | SLAB_PANIC, NULL);

	mount_hashtable = (struct list_head *)__get_free_page(GFP_ATOMIC);

	if (!mount_hashtable)
=======
	mnt_cache = kmem_cache_create("mnt_cache", sizeof(struct mount),
			0, SLAB_HWCACHE_ALIGN | SLAB_PANIC, NULL);

	mount_hashtable = (struct list_head *)__get_free_page(GFP_ATOMIC);
	mountpoint_hashtable = (struct list_head *)__get_free_page(GFP_ATOMIC);

	if (!mount_hashtable || !mountpoint_hashtable)
>>>>>>> refs/remotes/origin/master
		panic("Failed to allocate mount hash table\n");

	printk(KERN_INFO "Mount-cache hash table entries: %lu\n", HASH_SIZE);

	for (u = 0; u < HASH_SIZE; u++)
		INIT_LIST_HEAD(&mount_hashtable[u]);
<<<<<<< HEAD

	br_lock_init(vfsmount_lock);
=======
	for (u = 0; u < HASH_SIZE; u++)
		INIT_LIST_HEAD(&mountpoint_hashtable[u]);

	kernfs_init();
>>>>>>> refs/remotes/origin/master

	err = sysfs_init();
	if (err)
		printk(KERN_WARNING "%s: sysfs_init error: %d\n",
			__func__, err);
	fs_kobj = kobject_create_and_add("fs", NULL);
	if (!fs_kobj)
		printk(KERN_WARNING "%s: kobj create error\n", __func__);
	init_rootfs();
	init_mount_tree();
}

void put_mnt_ns(struct mnt_namespace *ns)
{
<<<<<<< HEAD
	LIST_HEAD(umount_list);

	if (!atomic_dec_and_test(&ns->count))
		return;
	down_write(&namespace_sem);
	br_write_lock(vfsmount_lock);
	umount_tree(ns->root, 0, &umount_list);
	br_write_unlock(vfsmount_lock);
	up_write(&namespace_sem);
	release_mounts(&umount_list);
	kfree(ns);
}
<<<<<<< HEAD
EXPORT_SYMBOL(put_mnt_ns);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!atomic_dec_and_test(&ns->count))
		return;
	drop_collected_mounts(&ns->root->mnt);
	free_mnt_ns(ns);
}
>>>>>>> refs/remotes/origin/master

struct vfsmount *kern_mount_data(struct file_system_type *type, void *data)
{
	struct vfsmount *mnt;
	mnt = vfs_kern_mount(type, MS_KERNMOUNT, type->name, data);
	if (!IS_ERR(mnt)) {
		/*
		 * it is a longterm mount, don't release mnt until
		 * we unmount before file sys is unregistered
		*/
<<<<<<< HEAD
		mnt_make_longterm(mnt);
=======
		real_mount(mnt)->mnt_ns = MNT_NS_INTERNAL;
>>>>>>> refs/remotes/origin/master
	}
	return mnt;
}
EXPORT_SYMBOL_GPL(kern_mount_data);

void kern_unmount(struct vfsmount *mnt)
{
	/* release long term mount so mount point can be released */
	if (!IS_ERR_OR_NULL(mnt)) {
<<<<<<< HEAD
		mnt_make_shortterm(mnt);
=======
		real_mount(mnt)->mnt_ns = NULL;
		synchronize_rcu();	/* yecchhh... */
>>>>>>> refs/remotes/origin/master
		mntput(mnt);
	}
}
EXPORT_SYMBOL(kern_unmount);

bool our_mnt(struct vfsmount *mnt)
{
<<<<<<< HEAD
<<<<<<< HEAD
	return check_mnt(mnt);
=======
	return check_mnt(real_mount(mnt));
>>>>>>> refs/remotes/origin/cm-10.0
}
=======
	return check_mnt(real_mount(mnt));
}

bool current_chrooted(void)
{
	/* Does the current process have a non-standard root */
	struct path ns_root;
	struct path fs_root;
	bool chrooted;

	/* Find the namespace root */
	ns_root.mnt = &current->nsproxy->mnt_ns->root->mnt;
	ns_root.dentry = ns_root.mnt->mnt_root;
	path_get(&ns_root);
	while (d_mountpoint(ns_root.dentry) && follow_down_one(&ns_root))
		;

	get_fs_root(current->fs, &fs_root);

	chrooted = !path_equal(&fs_root, &ns_root);

	path_put(&fs_root);
	path_put(&ns_root);

	return chrooted;
}

bool fs_fully_visible(struct file_system_type *type)
{
	struct mnt_namespace *ns = current->nsproxy->mnt_ns;
	struct mount *mnt;
	bool visible = false;

	if (unlikely(!ns))
		return false;

	down_read(&namespace_sem);
	list_for_each_entry(mnt, &ns->list, mnt_list) {
		struct mount *child;
		if (mnt->mnt.mnt_sb->s_type != type)
			continue;

		/* This mount is not fully visible if there are any child mounts
		 * that cover anything except for empty directories.
		 */
		list_for_each_entry(child, &mnt->mnt_mounts, mnt_child) {
			struct inode *inode = child->mnt_mountpoint->d_inode;
			if (!S_ISDIR(inode->i_mode))
				goto next;
			if (inode->i_nlink > 2)
				goto next;
		}
		visible = true;
		goto found;
	next:	;
	}
found:
	up_read(&namespace_sem);
	return visible;
}

static void *mntns_get(struct task_struct *task)
{
	struct mnt_namespace *ns = NULL;
	struct nsproxy *nsproxy;

	rcu_read_lock();
	nsproxy = task_nsproxy(task);
	if (nsproxy) {
		ns = nsproxy->mnt_ns;
		get_mnt_ns(ns);
	}
	rcu_read_unlock();

	return ns;
}

static void mntns_put(void *ns)
{
	put_mnt_ns(ns);
}

static int mntns_install(struct nsproxy *nsproxy, void *ns)
{
	struct fs_struct *fs = current->fs;
	struct mnt_namespace *mnt_ns = ns;
	struct path root;

	if (!ns_capable(mnt_ns->user_ns, CAP_SYS_ADMIN) ||
	    !ns_capable(current_user_ns(), CAP_SYS_CHROOT) ||
	    !ns_capable(current_user_ns(), CAP_SYS_ADMIN))
		return -EPERM;

	if (fs->users != 1)
		return -EINVAL;

	get_mnt_ns(mnt_ns);
	put_mnt_ns(nsproxy->mnt_ns);
	nsproxy->mnt_ns = mnt_ns;

	/* Find the root */
	root.mnt    = &mnt_ns->root->mnt;
	root.dentry = mnt_ns->root->mnt.mnt_root;
	path_get(&root);
	while(d_mountpoint(root.dentry) && follow_down_one(&root))
		;

	/* Update the pwd and root */
	set_fs_pwd(fs, &root);
	set_fs_root(fs, &root);

	path_put(&root);
	return 0;
}

static unsigned int mntns_inum(void *ns)
{
	struct mnt_namespace *mnt_ns = ns;
	return mnt_ns->proc_inum;
}

const struct proc_ns_operations mntns_operations = {
	.name		= "mnt",
	.type		= CLONE_NEWNS,
	.get		= mntns_get,
	.put		= mntns_put,
	.install	= mntns_install,
	.inum		= mntns_inum,
};
>>>>>>> refs/remotes/origin/master
