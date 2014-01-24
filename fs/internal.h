/* fs/ internal definitions
 *
 * Copyright (C) 2006 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

<<<<<<< HEAD
#include <linux/lglock.h>

=======
>>>>>>> refs/remotes/origin/master
struct super_block;
struct file_system_type;
struct linux_binprm;
struct path;
<<<<<<< HEAD
<<<<<<< HEAD
=======
struct mount;
>>>>>>> refs/remotes/origin/cm-10.0
=======
struct mount;
>>>>>>> refs/remotes/origin/master

/*
 * block_dev.c
 */
#ifdef CONFIG_BLOCK
<<<<<<< HEAD
<<<<<<< HEAD
extern struct super_block *blockdev_superblock;
extern void __init bdev_cache_init(void);

static inline int sb_is_blkdev_sb(struct super_block *sb)
{
	return sb == blockdev_superblock;
}

=======
extern void __init bdev_cache_init(void);

>>>>>>> refs/remotes/origin/cm-10.0
=======
extern void __init bdev_cache_init(void);

>>>>>>> refs/remotes/origin/master
extern int __sync_blockdev(struct block_device *bdev, int wait);

#else
static inline void bdev_cache_init(void)
{
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline int sb_is_blkdev_sb(struct super_block *sb)
{
	return 0;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static inline int __sync_blockdev(struct block_device *bdev, int wait)
{
	return 0;
}
#endif

/*
 * char_dev.c
 */
extern void __init chrdev_init(void);

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * exec.c
 */
extern int check_unsafe_exec(struct linux_binprm *);

/*
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * namei.c
 */
extern int __inode_permission(struct inode *, int);
extern int user_path_mountpoint_at(int, const char __user *, unsigned int, struct path *);
extern int vfs_path_lookup(struct dentry *, struct vfsmount *,
			   const char *, unsigned int, struct path *);

/*
>>>>>>> refs/remotes/origin/master
 * namespace.c
 */
extern int copy_mount_options(const void __user *, unsigned long *);
extern int copy_mount_string(const void __user *, char **);

<<<<<<< HEAD
<<<<<<< HEAD
extern unsigned int mnt_get_count(struct vfsmount *mnt);
extern struct vfsmount *__lookup_mnt(struct vfsmount *, struct dentry *, int);
extern struct vfsmount *lookup_mnt(struct path *);
extern void mnt_set_mountpoint(struct vfsmount *, struct dentry *,
				struct vfsmount *);
extern void release_mounts(struct list_head *);
extern void umount_tree(struct vfsmount *, int, struct list_head *);
extern struct vfsmount *copy_tree(struct vfsmount *, struct dentry *, int);
=======
extern struct vfsmount *lookup_mnt(struct path *);
>>>>>>> refs/remotes/origin/cm-10.0
extern int finish_automount(struct vfsmount *, struct path *);

extern void mnt_make_longterm(struct vfsmount *);
extern void mnt_make_shortterm(struct vfsmount *);
<<<<<<< HEAD
=======
extern int sb_prepare_remount_readonly(struct super_block *);
>>>>>>> refs/remotes/origin/cm-10.0

extern void __init mnt_init(void);

DECLARE_BRLOCK(vfsmount_lock);

=======
extern struct vfsmount *lookup_mnt(struct path *);
extern int finish_automount(struct vfsmount *, struct path *);

extern int sb_prepare_remount_readonly(struct super_block *);

extern void __init mnt_init(void);

extern int __mnt_want_write(struct vfsmount *);
extern int __mnt_want_write_file(struct file *);
extern void __mnt_drop_write(struct vfsmount *);
extern void __mnt_drop_write_file(struct file *);
>>>>>>> refs/remotes/origin/master

/*
 * fs_struct.c
 */
<<<<<<< HEAD
extern void chroot_fs_refs(struct path *, struct path *);
=======
extern void chroot_fs_refs(const struct path *, const struct path *);
>>>>>>> refs/remotes/origin/master

/*
 * file_table.c
 */
<<<<<<< HEAD
extern void file_sb_list_add(struct file *f, struct super_block *sb);
extern void file_sb_list_del(struct file *f);
extern void mark_files_ro(struct super_block *);
=======
>>>>>>> refs/remotes/origin/master
extern struct file *get_empty_filp(void);

/*
 * super.c
 */
extern int do_remount_sb(struct super_block *, int, void *, int);
<<<<<<< HEAD
<<<<<<< HEAD
extern void __put_super(struct super_block *sb);
extern void put_super(struct super_block *sb);
extern struct dentry *mount_fs(struct file_system_type *,
			       int, const char *, void *);
=======
=======
>>>>>>> refs/remotes/origin/master
extern bool grab_super_passive(struct super_block *sb);
extern struct dentry *mount_fs(struct file_system_type *,
			       int, const char *, void *);
extern struct super_block *user_get_super(dev_t);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

/*
 * open.c
 */
<<<<<<< HEAD
struct nameidata;
extern struct file *nameidata_to_filp(struct nameidata *);
extern void release_open_intent(struct nameidata *);
struct open_flags {
	int open_flag;
<<<<<<< HEAD
	int mode;
=======
	umode_t mode;
>>>>>>> refs/remotes/origin/cm-10.0
	int acc_mode;
	int intent;
};
extern struct file *do_filp_open(int dfd, const char *pathname,
		const struct open_flags *op, int lookup_flags);
extern struct file *do_file_open_root(struct dentry *, struct vfsmount *,
		const char *, const struct open_flags *, int lookup_flags);

extern long do_handle_open(int mountdirfd,
			   struct file_handle __user *ufh, int open_flag);
=======
struct open_flags {
	int open_flag;
	umode_t mode;
	int acc_mode;
	int intent;
	int lookup_flags;
};
extern struct file *do_filp_open(int dfd, struct filename *pathname,
		const struct open_flags *op);
extern struct file *do_file_open_root(struct dentry *, struct vfsmount *,
		const char *, const struct open_flags *);

extern long do_handle_open(int mountdirfd,
			   struct file_handle __user *ufh, int open_flag);
extern int open_check_o_direct(struct file *f);
>>>>>>> refs/remotes/origin/master

/*
 * inode.c
 */
extern spinlock_t inode_sb_list_lock;
<<<<<<< HEAD
=======
extern long prune_icache_sb(struct super_block *sb, unsigned long nr_to_scan,
			    int nid);
extern void inode_add_lru(struct inode *inode);
>>>>>>> refs/remotes/origin/master

/*
 * fs-writeback.c
 */
extern void inode_wb_list_del(struct inode *inode);

<<<<<<< HEAD
extern int get_nr_dirty_inodes(void);
extern void evict_inodes(struct super_block *);
extern int invalidate_inodes(struct super_block *, bool);
<<<<<<< HEAD
=======
=======
extern long get_nr_dirty_inodes(void);
extern void evict_inodes(struct super_block *);
extern int invalidate_inodes(struct super_block *, bool);
>>>>>>> refs/remotes/origin/master

/*
 * dcache.c
 */
extern struct dentry *__d_alloc(struct super_block *, const struct qstr *);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern int d_set_mounted(struct dentry *dentry);
extern long prune_dcache_sb(struct super_block *sb, unsigned long nr_to_scan,
			    int nid);

/*
 * read_write.c
 */
extern ssize_t __kernel_write(struct file *, const char *, size_t, loff_t *);
extern int rw_verify_area(int, struct file *, const loff_t *, size_t);

/*
 * splice.c
 */
extern long do_splice_direct(struct file *in, loff_t *ppos, struct file *out,
		loff_t *opos, size_t len, unsigned int flags);

/*
 * pipe.c
 */
extern const struct file_operations pipefifo_fops;
>>>>>>> refs/remotes/origin/master
