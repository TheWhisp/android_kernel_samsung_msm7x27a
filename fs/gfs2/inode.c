/*
 * Copyright (C) Sistina Software, Inc.  1997-2003 All rights reserved.
 * Copyright (C) 2004-2011 Red Hat, Inc.  All rights reserved.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU General Public License version 2.
 */

#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/completion.h>
#include <linux/buffer_head.h>
#include <linux/namei.h>
#include <linux/mm.h>
#include <linux/xattr.h>
#include <linux/posix_acl.h>
#include <linux/gfs2_ondisk.h>
#include <linux/crc32.h>
#include <linux/fiemap.h>
#include <linux/security.h>
#include <asm/uaccess.h>

#include "gfs2.h"
#include "incore.h"
#include "acl.h"
#include "bmap.h"
#include "dir.h"
#include "xattr.h"
#include "glock.h"
#include "inode.h"
#include "meta_io.h"
#include "quota.h"
#include "rgrp.h"
#include "trans.h"
#include "util.h"
#include "super.h"
#include "glops.h"

struct gfs2_skip_data {
	u64 no_addr;
	int skipped;
	int non_block;
};

static int iget_test(struct inode *inode, void *opaque)
{
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_skip_data *data = opaque;

	if (ip->i_no_addr == data->no_addr) {
		if (data->non_block &&
		    inode->i_state & (I_FREEING|I_CLEAR|I_WILL_FREE)) {
			data->skipped = 1;
			return 0;
		}
		return 1;
	}
	return 0;
}

static int iget_set(struct inode *inode, void *opaque)
{
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_skip_data *data = opaque;

	if (data->skipped)
		return -ENOENT;
	inode->i_ino = (unsigned long)(data->no_addr);
	ip->i_no_addr = data->no_addr;
	return 0;
}

struct inode *gfs2_ilookup(struct super_block *sb, u64 no_addr, int non_block)
{
	unsigned long hash = (unsigned long)no_addr;
	struct gfs2_skip_data data;

	data.no_addr = no_addr;
	data.skipped = 0;
	data.non_block = non_block;
	return ilookup5(sb, hash, iget_test, &data);
}

static struct inode *gfs2_iget(struct super_block *sb, u64 no_addr,
			       int non_block)
{
	struct gfs2_skip_data data;
	unsigned long hash = (unsigned long)no_addr;

	data.no_addr = no_addr;
	data.skipped = 0;
	data.non_block = non_block;
	return iget5_locked(sb, hash, iget_test, iget_set, &data);
}

/**
 * gfs2_set_iop - Sets inode operations
 * @inode: The inode with correct i_mode filled in
 *
 * GFS2 lookup code fills in vfs inode contents based on info obtained
 * from directory entry inside gfs2_inode_lookup().
 */

static void gfs2_set_iop(struct inode *inode)
{
	struct gfs2_sbd *sdp = GFS2_SB(inode);
	umode_t mode = inode->i_mode;

	if (S_ISREG(mode)) {
		inode->i_op = &gfs2_file_iops;
		if (gfs2_localflocks(sdp))
			inode->i_fop = &gfs2_file_fops_nolock;
		else
			inode->i_fop = &gfs2_file_fops;
	} else if (S_ISDIR(mode)) {
		inode->i_op = &gfs2_dir_iops;
		if (gfs2_localflocks(sdp))
			inode->i_fop = &gfs2_dir_fops_nolock;
		else
			inode->i_fop = &gfs2_dir_fops;
	} else if (S_ISLNK(mode)) {
		inode->i_op = &gfs2_symlink_iops;
	} else {
		inode->i_op = &gfs2_file_iops;
		init_special_inode(inode, inode->i_mode, inode->i_rdev);
	}
}

/**
 * gfs2_inode_lookup - Lookup an inode
 * @sb: The super block
 * @no_addr: The inode number
 * @type: The type of the inode
 * non_block: Can we block on inodes that are being freed?
 *
 * Returns: A VFS inode, or an error
 */

struct inode *gfs2_inode_lookup(struct super_block *sb, unsigned int type,
				u64 no_addr, u64 no_formal_ino, int non_block)
{
	struct inode *inode;
	struct gfs2_inode *ip;
	struct gfs2_glock *io_gl = NULL;
	int error;

	inode = gfs2_iget(sb, no_addr, non_block);
	ip = GFS2_I(inode);

	if (!inode)
<<<<<<< HEAD
		return ERR_PTR(-ENOBUFS);
=======
		return ERR_PTR(-ENOMEM);
>>>>>>> refs/remotes/origin/master

	if (inode->i_state & I_NEW) {
		struct gfs2_sbd *sdp = GFS2_SB(inode);
		ip->i_no_formal_ino = no_formal_ino;

		error = gfs2_glock_get(sdp, no_addr, &gfs2_inode_glops, CREATE, &ip->i_gl);
		if (unlikely(error))
			goto fail;
		ip->i_gl->gl_object = ip;

		error = gfs2_glock_get(sdp, no_addr, &gfs2_iopen_glops, CREATE, &io_gl);
		if (unlikely(error))
			goto fail_put;

		set_bit(GIF_INVALID, &ip->i_flags);
		error = gfs2_glock_nq_init(io_gl, LM_ST_SHARED, GL_EXACT, &ip->i_iopen_gh);
		if (unlikely(error))
			goto fail_iopen;

		ip->i_iopen_gh.gh_gl->gl_object = ip;
		gfs2_glock_put(io_gl);
		io_gl = NULL;

		if (type == DT_UNKNOWN) {
			/* Inode glock must be locked already */
			error = gfs2_inode_refresh(GFS2_I(inode));
			if (error)
				goto fail_refresh;
		} else {
			inode->i_mode = DT2IF(type);
		}

		gfs2_set_iop(inode);
		unlock_new_inode(inode);
	}

	return inode;

fail_refresh:
<<<<<<< HEAD
=======
	ip->i_iopen_gh.gh_flags |= GL_NOCACHE;
>>>>>>> refs/remotes/origin/master
	ip->i_iopen_gh.gh_gl->gl_object = NULL;
	gfs2_glock_dq_uninit(&ip->i_iopen_gh);
fail_iopen:
	if (io_gl)
		gfs2_glock_put(io_gl);
fail_put:
	ip->i_gl->gl_object = NULL;
	gfs2_glock_put(ip->i_gl);
fail:
	iget_failed(inode);
	return ERR_PTR(error);
}

struct inode *gfs2_lookup_by_inum(struct gfs2_sbd *sdp, u64 no_addr,
				  u64 *no_formal_ino, unsigned int blktype)
{
	struct super_block *sb = sdp->sd_vfs;
	struct gfs2_holder i_gh;
	struct inode *inode = NULL;
	int error;

	/* Must not read in block until block type is verified */
	error = gfs2_glock_nq_num(sdp, no_addr, &gfs2_inode_glops,
				  LM_ST_EXCLUSIVE, GL_SKIP, &i_gh);
	if (error)
		return ERR_PTR(error);

	error = gfs2_check_blk_type(sdp, no_addr, blktype);
	if (error)
		goto fail;

	inode = gfs2_inode_lookup(sb, DT_UNKNOWN, no_addr, 0, 1);
	if (IS_ERR(inode))
		goto fail;

	/* Two extra checks for NFS only */
	if (no_formal_ino) {
		error = -ESTALE;
		if (GFS2_I(inode)->i_no_formal_ino != *no_formal_ino)
			goto fail_iput;

		error = -EIO;
		if (GFS2_I(inode)->i_diskflags & GFS2_DIF_SYSTEM)
			goto fail_iput;

		error = 0;
	}

fail:
	gfs2_glock_dq_uninit(&i_gh);
	return error ? ERR_PTR(error) : inode;
fail_iput:
	iput(inode);
	goto fail;
}


struct inode *gfs2_lookup_simple(struct inode *dip, const char *name)
{
	struct qstr qstr;
	struct inode *inode;
	gfs2_str2qstr(&qstr, name);
	inode = gfs2_lookupi(dip, &qstr, 1);
	/* gfs2_lookupi has inconsistent callers: vfs
	 * related routines expect NULL for no entry found,
	 * gfs2_lookup_simple callers expect ENOENT
	 * and do not check for NULL.
	 */
	if (inode == NULL)
		return ERR_PTR(-ENOENT);
	else
		return inode;
}


/**
 * gfs2_lookupi - Look up a filename in a directory and return its inode
 * @d_gh: An initialized holder for the directory glock
 * @name: The name of the inode to look for
 * @is_root: If 1, ignore the caller's permissions
 * @i_gh: An uninitialized holder for the new inode glock
 *
 * This can be called via the VFS filldir function when NFS is doing
 * a readdirplus and the inode which its intending to stat isn't
 * already in cache. In this case we must not take the directory glock
 * again, since the readdir call will have already taken that lock.
 *
 * Returns: errno
 */

struct inode *gfs2_lookupi(struct inode *dir, const struct qstr *name,
			   int is_root)
{
	struct super_block *sb = dir->i_sb;
	struct gfs2_inode *dip = GFS2_I(dir);
	struct gfs2_holder d_gh;
	int error = 0;
	struct inode *inode = NULL;
	int unlock = 0;

	if (!name->len || name->len > GFS2_FNAMESIZE)
		return ERR_PTR(-ENAMETOOLONG);

	if ((name->len == 1 && memcmp(name->name, ".", 1) == 0) ||
	    (name->len == 2 && memcmp(name->name, "..", 2) == 0 &&
	     dir == sb->s_root->d_inode)) {
		igrab(dir);
		return dir;
	}

	if (gfs2_glock_is_locked_by_me(dip->i_gl) == NULL) {
		error = gfs2_glock_nq_init(dip->i_gl, LM_ST_SHARED, 0, &d_gh);
		if (error)
			return ERR_PTR(error);
		unlock = 1;
	}

	if (!is_root) {
<<<<<<< HEAD
<<<<<<< HEAD
		error = gfs2_permission(dir, MAY_EXEC, 0);
=======
		error = gfs2_permission(dir, MAY_EXEC);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		error = gfs2_permission(dir, MAY_EXEC);
>>>>>>> refs/remotes/origin/master
		if (error)
			goto out;
	}

<<<<<<< HEAD
	inode = gfs2_dir_search(dir, name);
=======
	inode = gfs2_dir_search(dir, name, false);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(inode))
		error = PTR_ERR(inode);
out:
	if (unlock)
		gfs2_glock_dq_uninit(&d_gh);
	if (error == -ENOENT)
		return NULL;
	return inode ? inode : ERR_PTR(error);
}

/**
 * create_ok - OK to create a new on-disk inode here?
 * @dip:  Directory in which dinode is to be created
 * @name:  Name of new dinode
 * @mode:
 *
 * Returns: errno
 */

static int create_ok(struct gfs2_inode *dip, const struct qstr *name,
<<<<<<< HEAD
<<<<<<< HEAD
		     unsigned int mode)
{
	int error;

	error = gfs2_permission(&dip->i_inode, MAY_WRITE | MAY_EXEC, 0);
=======
=======
>>>>>>> refs/remotes/origin/master
		     umode_t mode)
{
	int error;

	error = gfs2_permission(&dip->i_inode, MAY_WRITE | MAY_EXEC);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (error)
		return error;

	/*  Don't create entries in an unlinked directory  */
	if (!dip->i_inode.i_nlink)
		return -ENOENT;

<<<<<<< HEAD
	error = gfs2_dir_check(&dip->i_inode, name, NULL);
	switch (error) {
	case -ENOENT:
		error = 0;
		break;
	case 0:
		return -EEXIST;
	default:
		return error;
	}

=======
>>>>>>> refs/remotes/origin/master
	if (dip->i_entries == (u32)-1)
		return -EFBIG;
	if (S_ISDIR(mode) && dip->i_inode.i_nlink == (u32)-1)
		return -EMLINK;

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void munge_mode_uid_gid(struct gfs2_inode *dip, unsigned int *mode,
=======
static void munge_mode_uid_gid(struct gfs2_inode *dip, umode_t *mode,
>>>>>>> refs/remotes/origin/cm-10.0
			       unsigned int *uid, unsigned int *gid)
{
	if (GFS2_SB(&dip->i_inode)->sd_args.ar_suiddir &&
	    (dip->i_inode.i_mode & S_ISUID) && dip->i_inode.i_uid) {
		if (S_ISDIR(*mode))
			*mode |= S_ISUID;
		else if (dip->i_inode.i_uid != current_fsuid())
			*mode &= ~07111;
		*uid = dip->i_inode.i_uid;
	} else
		*uid = current_fsuid();

	if (dip->i_inode.i_mode & S_ISGID) {
		if (S_ISDIR(*mode))
			*mode |= S_ISGID;
		*gid = dip->i_inode.i_gid;
	} else
		*gid = current_fsgid();
}

static int alloc_dinode(struct gfs2_inode *dip, u64 *no_addr, u64 *generation)
{
	struct gfs2_sbd *sdp = GFS2_SB(&dip->i_inode);
	int error;
<<<<<<< HEAD

	if (gfs2_alloc_get(dip) == NULL)
		return -ENOMEM;

	dip->i_alloc->al_requested = RES_DINODE;
	error = gfs2_inplace_reserve(dip);
=======
	int dblocks = 1;

	error = gfs2_inplace_reserve(dip, RES_DINODE);
>>>>>>> refs/remotes/origin/cm-10.0
	if (error)
		goto out;

	error = gfs2_trans_begin(sdp, RES_RG_BIT + RES_STATFS, 0);
	if (error)
		goto out_ipreserv;

<<<<<<< HEAD
	error = gfs2_alloc_di(dip, no_addr, generation);
=======
	error = gfs2_alloc_blocks(dip, no_addr, &dblocks, 1, generation);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void munge_mode_uid_gid(const struct gfs2_inode *dip,
			       struct inode *inode)
{
	if (GFS2_SB(&dip->i_inode)->sd_args.ar_suiddir &&
	    (dip->i_inode.i_mode & S_ISUID) &&
	    !uid_eq(dip->i_inode.i_uid, GLOBAL_ROOT_UID)) {
		if (S_ISDIR(inode->i_mode))
			inode->i_mode |= S_ISUID;
		else if (!uid_eq(dip->i_inode.i_uid, current_fsuid()))
			inode->i_mode &= ~07111;
		inode->i_uid = dip->i_inode.i_uid;
	} else
		inode->i_uid = current_fsuid();

	if (dip->i_inode.i_mode & S_ISGID) {
		if (S_ISDIR(inode->i_mode))
			inode->i_mode |= S_ISGID;
		inode->i_gid = dip->i_inode.i_gid;
	} else
		inode->i_gid = current_fsgid();
}

static int alloc_dinode(struct gfs2_inode *ip, u32 flags)
{
	struct gfs2_sbd *sdp = GFS2_SB(&ip->i_inode);
	struct gfs2_alloc_parms ap = { .target = RES_DINODE, .aflags = flags, };
	int error;
	int dblocks = 1;

	error = gfs2_quota_lock_check(ip);
	if (error)
		goto out;

	error = gfs2_inplace_reserve(ip, &ap);
	if (error)
		goto out_quota;

	error = gfs2_trans_begin(sdp, RES_RG_BIT + RES_STATFS + RES_QUOTA, 0);
	if (error)
		goto out_ipreserv;

	error = gfs2_alloc_blocks(ip, &ip->i_no_addr, &dblocks, 1, &ip->i_generation);
	ip->i_no_formal_ino = ip->i_generation;
	ip->i_inode.i_ino = ip->i_no_addr;
	ip->i_goal = ip->i_no_addr;
>>>>>>> refs/remotes/origin/master

	gfs2_trans_end(sdp);

out_ipreserv:
<<<<<<< HEAD
	gfs2_inplace_release(dip);
out:
<<<<<<< HEAD
	gfs2_alloc_put(dip);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	gfs2_inplace_release(ip);
out_quota:
	gfs2_quota_unlock(ip);
out:
>>>>>>> refs/remotes/origin/master
	return error;
}

static void gfs2_init_dir(struct buffer_head *dibh,
			  const struct gfs2_inode *parent)
{
	struct gfs2_dinode *di = (struct gfs2_dinode *)dibh->b_data;
	struct gfs2_dirent *dent = (struct gfs2_dirent *)(di+1);

	gfs2_qstr2dirent(&gfs2_qdot, GFS2_DIRENT_SIZE(gfs2_qdot.len), dent);
	dent->de_inum = di->di_num; /* already GFS2 endian */
	dent->de_type = cpu_to_be16(DT_DIR);

	dent = (struct gfs2_dirent *)((char*)dent + GFS2_DIRENT_SIZE(1));
	gfs2_qstr2dirent(&gfs2_qdotdot, dibh->b_size - GFS2_DIRENT_SIZE(1) - sizeof(struct gfs2_dinode), dent);
	gfs2_inum_out(parent, dent);
	dent->de_type = cpu_to_be16(DT_DIR);
	
}

/**
 * init_dinode - Fill in a new dinode structure
 * @dip: The directory this inode is being created in
<<<<<<< HEAD
 * @gl: The glock covering the new inode
 * @inum: The inode number
 * @mode: The file permissions
 * @uid: The uid of the new inode
 * @gid: The gid of the new inode
 * @generation: The generation number of the new inode
 * @dev: The device number (if a device node)
 * @symname: The symlink destination (if a symlink)
 * @size: The inode size (ignored for directories)
=======
 * @ip: The inode
 * @symname: The symlink destination (if a symlink)
>>>>>>> refs/remotes/origin/master
 * @bhp: The buffer head (returned to caller)
 *
 */

<<<<<<< HEAD
static void init_dinode(struct gfs2_inode *dip, struct gfs2_glock *gl,
<<<<<<< HEAD
			const struct gfs2_inum_host *inum, unsigned int mode,
=======
			const struct gfs2_inum_host *inum, umode_t mode,
>>>>>>> refs/remotes/origin/cm-10.0
			unsigned int uid, unsigned int gid,
			const u64 *generation, dev_t dev, const char *symname,
			unsigned size, struct buffer_head **bhp)
{
	struct gfs2_sbd *sdp = GFS2_SB(&dip->i_inode);
	struct gfs2_dinode *di;
	struct buffer_head *dibh;
	struct timespec tv = CURRENT_TIME;

	dibh = gfs2_meta_new(gl, inum->no_addr);
	gfs2_trans_add_bh(gl, dibh, 1);
	gfs2_metatype_set(dibh, GFS2_METATYPE_DI, GFS2_FORMAT_DI);
	gfs2_buffer_clear_tail(dibh, sizeof(struct gfs2_dinode));
	di = (struct gfs2_dinode *)dibh->b_data;

	di->di_num.no_formal_ino = cpu_to_be64(inum->no_formal_ino);
	di->di_num.no_addr = cpu_to_be64(inum->no_addr);
	di->di_mode = cpu_to_be32(mode);
	di->di_uid = cpu_to_be32(uid);
	di->di_gid = cpu_to_be32(gid);
	di->di_nlink = 0;
	di->di_size = cpu_to_be64(size);
	di->di_blocks = cpu_to_be64(1);
	di->di_atime = di->di_mtime = di->di_ctime = cpu_to_be64(tv.tv_sec);
	di->di_major = cpu_to_be32(MAJOR(dev));
	di->di_minor = cpu_to_be32(MINOR(dev));
	di->di_goal_meta = di->di_goal_data = cpu_to_be64(inum->no_addr);
	di->di_generation = cpu_to_be64(*generation);
	di->di_flags = 0;
	di->__pad1 = 0;
	di->di_payload_format = cpu_to_be32(S_ISDIR(mode) ? GFS2_FORMAT_DE : 0);
	di->di_height = 0;
	di->__pad2 = 0;
	di->__pad3 = 0;
	di->di_depth = 0;
	di->di_entries = 0;
	memset(&di->__pad4, 0, sizeof(di->__pad4));
	di->di_eattr = 0;
	di->di_atime_nsec = cpu_to_be32(tv.tv_nsec);
	di->di_mtime_nsec = cpu_to_be32(tv.tv_nsec);
	di->di_ctime_nsec = cpu_to_be32(tv.tv_nsec);
	memset(&di->di_reserved, 0, sizeof(di->di_reserved));

	switch(mode & S_IFMT) {	
	case S_IFREG:
		if ((dip->i_diskflags & GFS2_DIF_INHERIT_JDATA) ||
		    gfs2_tune_get(sdp, gt_new_files_jdata))
			di->di_flags |= cpu_to_be32(GFS2_DIF_JDATA);
		break;
	case S_IFDIR:
		di->di_flags |= cpu_to_be32(dip->i_diskflags &
					    GFS2_DIF_INHERIT_JDATA);
		di->di_flags |= cpu_to_be32(GFS2_DIF_JDATA);
		di->di_size = cpu_to_be64(sdp->sd_sb.sb_bsize - sizeof(struct gfs2_dinode));
		di->di_entries = cpu_to_be32(2);
		gfs2_init_dir(dibh, dip);
		break;
	case S_IFLNK:
		memcpy(dibh->b_data + sizeof(struct gfs2_dinode), symname, size);
=======
static void init_dinode(struct gfs2_inode *dip, struct gfs2_inode *ip,
			const char *symname)
{
	struct gfs2_dinode *di;
	struct buffer_head *dibh;

	dibh = gfs2_meta_new(ip->i_gl, ip->i_no_addr);
	gfs2_trans_add_meta(ip->i_gl, dibh);
	di = (struct gfs2_dinode *)dibh->b_data;
	gfs2_dinode_out(ip, di);

	di->di_major = cpu_to_be32(MAJOR(ip->i_inode.i_rdev));
	di->di_minor = cpu_to_be32(MINOR(ip->i_inode.i_rdev));
	di->__pad1 = 0;
	di->__pad2 = 0;
	di->__pad3 = 0;
	memset(&di->__pad4, 0, sizeof(di->__pad4));
	memset(&di->di_reserved, 0, sizeof(di->di_reserved));
	gfs2_buffer_clear_tail(dibh, sizeof(struct gfs2_dinode));

	switch(ip->i_inode.i_mode & S_IFMT) {
	case S_IFDIR:
		gfs2_init_dir(dibh, dip);
		break;
	case S_IFLNK:
		memcpy(dibh->b_data + sizeof(struct gfs2_dinode), symname, ip->i_inode.i_size);
>>>>>>> refs/remotes/origin/master
		break;
	}

	set_buffer_uptodate(dibh);
<<<<<<< HEAD

	*bhp = dibh;
}

static int make_dinode(struct gfs2_inode *dip, struct gfs2_glock *gl,
<<<<<<< HEAD
		       unsigned int mode, const struct gfs2_inum_host *inum,
=======
		       umode_t mode, const struct gfs2_inum_host *inum,
>>>>>>> refs/remotes/origin/cm-10.0
		       const u64 *generation, dev_t dev, const char *symname,
		       unsigned int size, struct buffer_head **bhp)
{
	struct gfs2_sbd *sdp = GFS2_SB(&dip->i_inode);
	unsigned int uid, gid;
	int error;

	munge_mode_uid_gid(dip, &mode, &uid, &gid);
<<<<<<< HEAD
	if (!gfs2_alloc_get(dip))
=======
	if (!gfs2_qadata_get(dip))
>>>>>>> refs/remotes/origin/cm-10.0
		return -ENOMEM;

	error = gfs2_quota_lock(dip, uid, gid);
	if (error)
		goto out;

	error = gfs2_quota_check(dip, uid, gid);
	if (error)
		goto out_quota;

	error = gfs2_trans_begin(sdp, RES_DINODE + RES_QUOTA, 0);
	if (error)
		goto out_quota;

	init_dinode(dip, gl, inum, mode, uid, gid, generation, dev, symname, size, bhp);
	gfs2_quota_change(dip, +1, uid, gid);
	gfs2_trans_end(sdp);

out_quota:
	gfs2_quota_unlock(dip);
out:
<<<<<<< HEAD
	gfs2_alloc_put(dip);
=======
	gfs2_qadata_put(dip);
>>>>>>> refs/remotes/origin/cm-10.0
	return error;
}

static int link_dinode(struct gfs2_inode *dip, const struct qstr *name,
		       struct gfs2_inode *ip)
{
	struct gfs2_sbd *sdp = GFS2_SB(&dip->i_inode);
<<<<<<< HEAD
	struct gfs2_alloc *al;
=======
	struct gfs2_qadata *qa;
>>>>>>> refs/remotes/origin/cm-10.0
	int alloc_required;
	struct buffer_head *dibh;
	int error;

<<<<<<< HEAD
	al = gfs2_alloc_get(dip);
	if (!al)
=======
	qa = gfs2_qadata_get(dip);
	if (!qa)
>>>>>>> refs/remotes/origin/cm-10.0
		return -ENOMEM;

	error = gfs2_quota_lock(dip, NO_QUOTA_CHANGE, NO_QUOTA_CHANGE);
	if (error)
		goto fail;

	error = alloc_required = gfs2_diradd_alloc_required(&dip->i_inode, name);
	if (alloc_required < 0)
		goto fail_quota_locks;
	if (alloc_required) {
		error = gfs2_quota_check(dip, dip->i_inode.i_uid, dip->i_inode.i_gid);
		if (error)
			goto fail_quota_locks;

<<<<<<< HEAD
		al->al_requested = sdp->sd_max_dirres;

		error = gfs2_inplace_reserve(dip);
=======
		error = gfs2_inplace_reserve(dip, sdp->sd_max_dirres);
>>>>>>> refs/remotes/origin/cm-10.0
		if (error)
			goto fail_quota_locks;

		error = gfs2_trans_begin(sdp, sdp->sd_max_dirres +
<<<<<<< HEAD
					 al->al_rgd->rd_length +
=======
					 dip->i_rgd->rd_length +
>>>>>>> refs/remotes/origin/cm-10.0
					 2 * RES_DINODE +
					 RES_STATFS + RES_QUOTA, 0);
=======
	brelse(dibh);
}

/**
 * gfs2_trans_da_blocks - Calculate number of blocks to link inode
 * @dip: The directory we are linking into
 * @da: The dir add information
 * @nr_inodes: The number of inodes involved
 *
 * This calculate the number of blocks we need to reserve in a
 * transaction to link @nr_inodes into a directory. In most cases
 * @nr_inodes will be 2 (the directory plus the inode being linked in)
 * but in case of rename, 4 may be required.
 *
 * Returns: Number of blocks
 */

static unsigned gfs2_trans_da_blks(const struct gfs2_inode *dip,
				   const struct gfs2_diradd *da,
				   unsigned nr_inodes)
{
	return da->nr_blocks + gfs2_rg_blocks(dip, da->nr_blocks) +
	       (nr_inodes * RES_DINODE) + RES_QUOTA + RES_STATFS;
}

static int link_dinode(struct gfs2_inode *dip, const struct qstr *name,
		       struct gfs2_inode *ip, struct gfs2_diradd *da)
{
	struct gfs2_sbd *sdp = GFS2_SB(&dip->i_inode);
	struct gfs2_alloc_parms ap = { .target = da->nr_blocks, };
	int error;

	if (da->nr_blocks) {
		error = gfs2_quota_lock_check(dip);
		if (error)
			goto fail_quota_locks;

		error = gfs2_inplace_reserve(dip, &ap);
		if (error)
			goto fail_quota_locks;

		error = gfs2_trans_begin(sdp, gfs2_trans_da_blks(dip, da, 2), 0);
>>>>>>> refs/remotes/origin/master
		if (error)
			goto fail_ipreserv;
	} else {
		error = gfs2_trans_begin(sdp, RES_LEAF + 2 * RES_DINODE, 0);
		if (error)
			goto fail_quota_locks;
	}

<<<<<<< HEAD
	error = gfs2_dir_add(&dip->i_inode, name, ip);
	if (error)
		goto fail_end_trans;

	error = gfs2_meta_inode_buffer(ip, &dibh);
	if (error)
		goto fail_end_trans;
<<<<<<< HEAD
	inc_nlink(&ip->i_inode);
	if (S_ISDIR(ip->i_inode.i_mode))
		inc_nlink(&ip->i_inode);
=======
	set_nlink(&ip->i_inode, S_ISDIR(ip->i_inode.i_mode) ? 2 : 1);
>>>>>>> refs/remotes/origin/cm-10.0
	gfs2_trans_add_bh(ip->i_gl, dibh, 1);
	gfs2_dinode_out(ip, dibh->b_data);
	brelse(dibh);
	return 0;

fail_end_trans:
	gfs2_trans_end(sdp);

fail_ipreserv:
<<<<<<< HEAD
	if (dip->i_alloc->al_rgd)
		gfs2_inplace_release(dip);
=======
	gfs2_inplace_release(dip);
>>>>>>> refs/remotes/origin/cm-10.0

fail_quota_locks:
	gfs2_quota_unlock(dip);

fail:
<<<<<<< HEAD
	gfs2_alloc_put(dip);
	return error;
}

static int gfs2_security_init(struct gfs2_inode *dip, struct gfs2_inode *ip,
			      const struct qstr *qstr)
{
	int err;
	size_t len;
	void *value;
	char *name;

	err = security_inode_init_security(&ip->i_inode, &dip->i_inode, qstr,
					   &name, &value, &len);

	if (err) {
		if (err == -EOPNOTSUPP)
			return 0;
		return err;
	}

	err = __gfs2_xattr_set(&ip->i_inode, name, value, len, 0,
			       GFS2_EATYPE_SECURITY);
	kfree(value);
	kfree(name);

	return err;
}

=======
	gfs2_qadata_put(dip);
=======
	error = gfs2_dir_add(&dip->i_inode, name, ip, da);
	if (error)
		goto fail_end_trans;

fail_end_trans:
	gfs2_trans_end(sdp);
fail_ipreserv:
	gfs2_inplace_release(dip);
fail_quota_locks:
	gfs2_quota_unlock(dip);
>>>>>>> refs/remotes/origin/master
	return error;
}

static int gfs2_initxattrs(struct inode *inode, const struct xattr *xattr_array,
		    void *fs_info)
{
	const struct xattr *xattr;
	int err = 0;

	for (xattr = xattr_array; xattr->name != NULL; xattr++) {
		err = __gfs2_xattr_set(inode, xattr->name, xattr->value,
				       xattr->value_len, 0,
				       GFS2_EATYPE_SECURITY);
		if (err < 0)
			break;
	}
	return err;
}

static int gfs2_security_init(struct gfs2_inode *dip, struct gfs2_inode *ip,
			      const struct qstr *qstr)
{
	return security_inode_init_security(&ip->i_inode, &dip->i_inode, qstr,
					    &gfs2_initxattrs, NULL);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/**
 * gfs2_create_inode - Create a new inode
 * @dir: The parent directory
 * @dentry: The new dentry
<<<<<<< HEAD
=======
 * @file: If non-NULL, the file which is being opened
>>>>>>> refs/remotes/origin/master
 * @mode: The permissions on the new inode
 * @dev: For device nodes, this is the device number
 * @symname: For symlinks, this is the link destination
 * @size: The initial size of the inode (ignored for directories)
 *
 * Returns: 0 on success, or error code
 */

static int gfs2_create_inode(struct inode *dir, struct dentry *dentry,
<<<<<<< HEAD
<<<<<<< HEAD
			     unsigned int mode, dev_t dev, const char *symname,
			     unsigned int size)
=======
			     umode_t mode, dev_t dev, const char *symname,
			     unsigned int size, int excl)
>>>>>>> refs/remotes/origin/cm-10.0
=======
			     struct file *file,
			     umode_t mode, dev_t dev, const char *symname,
			     unsigned int size, int excl, int *opened)
>>>>>>> refs/remotes/origin/master
{
	const struct qstr *name = &dentry->d_name;
	struct gfs2_holder ghs[2];
	struct inode *inode = NULL;
<<<<<<< HEAD
	struct gfs2_inode *dip = GFS2_I(dir);
	struct gfs2_sbd *sdp = GFS2_SB(&dip->i_inode);
	struct gfs2_inum_host inum = { .no_addr = 0, .no_formal_ino = 0 };
	int error;
	u64 generation;
	struct buffer_head *bh = NULL;
=======
	struct gfs2_inode *dip = GFS2_I(dir), *ip;
	struct gfs2_sbd *sdp = GFS2_SB(&dip->i_inode);
	struct gfs2_glock *io_gl;
	struct dentry *d;
	int error;
	u32 aflags = 0;
	struct gfs2_diradd da = { .bh = NULL, };
>>>>>>> refs/remotes/origin/master

	if (!name->len || name->len > GFS2_FNAMESIZE)
		return -ENAMETOOLONG;

<<<<<<< HEAD
=======
	error = gfs2_rs_alloc(dip);
	if (error)
		return error;

	error = gfs2_rindex_update(sdp);
	if (error)
		return error;

>>>>>>> refs/remotes/origin/master
	error = gfs2_glock_nq_init(dip->i_gl, LM_ST_EXCLUSIVE, 0, ghs);
	if (error)
		goto fail;

	error = create_ok(dip, name, mode);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	if ((error == -EEXIST) && S_ISREG(mode) && !excl) {
		inode = gfs2_lookupi(dir, &dentry->d_name, 0);
		gfs2_glock_dq_uninit(ghs);
		d_instantiate(dentry, inode);
		return IS_ERR(inode) ? PTR_ERR(inode) : 0;
	}
>>>>>>> refs/remotes/origin/cm-10.0
	if (error)
		goto fail_gunlock;

	error = alloc_dinode(dip, &inum.no_addr, &generation);
	if (error)
		goto fail_gunlock;
	inum.no_formal_ino = generation;

	error = gfs2_glock_nq_num(sdp, inum.no_addr, &gfs2_inode_glops,
				  LM_ST_EXCLUSIVE, GL_SKIP, ghs + 1);
	if (error)
		goto fail_gunlock;

	error = make_dinode(dip, ghs[1].gh_gl, mode, &inum, &generation, dev, symname, size, &bh);
	if (error)
		goto fail_gunlock2;

	inode = gfs2_inode_lookup(dir->i_sb, IF2DT(mode), inum.no_addr,
				  inum.no_formal_ino, 0);
	if (IS_ERR(inode))
		goto fail_gunlock2;

	error = gfs2_inode_refresh(GFS2_I(inode));
	if (error)
		goto fail_gunlock2;

	error = gfs2_acl_create(dip, inode);
	if (error)
		goto fail_gunlock2;

	error = gfs2_security_init(dip, GFS2_I(inode), name);
	if (error)
		goto fail_gunlock2;

	error = link_dinode(dip, name, GFS2_I(inode));
	if (error)
		goto fail_gunlock2;

	if (bh)
		brelse(bh);

	gfs2_trans_end(sdp);
<<<<<<< HEAD
	if (dip->i_alloc->al_rgd)
		gfs2_inplace_release(dip);
	gfs2_quota_unlock(dip);
	gfs2_alloc_put(dip);
	gfs2_glock_dq_uninit_m(2, ghs);
	mark_inode_dirty(inode);
=======
	/* Check if we reserved space in the rgrp. Function link_dinode may
	   not, depending on whether alloc is required. */
	if (dip->i_res)
		gfs2_inplace_release(dip);
	gfs2_quota_unlock(dip);
	gfs2_qadata_put(dip);
	mark_inode_dirty(inode);
	gfs2_glock_dq_uninit_m(2, ghs);
>>>>>>> refs/remotes/origin/cm-10.0
	d_instantiate(dentry, inode);
	return 0;

fail_gunlock2:
	gfs2_glock_dq_uninit(ghs + 1);
<<<<<<< HEAD
	if (inode && !IS_ERR(inode))
		iput(inode);
fail_gunlock:
	gfs2_glock_dq_uninit(ghs);
=======
fail_gunlock:
	gfs2_glock_dq_uninit(ghs);
	if (inode && !IS_ERR(inode)) {
		set_bit(GIF_ALLOC_FAILED, &GFS2_I(inode)->i_flags);
		iput(inode);
	}
>>>>>>> refs/remotes/origin/cm-10.0
fail:
	if (bh)
		brelse(bh);
=======
	if (error)
		goto fail_gunlock;

	inode = gfs2_dir_search(dir, &dentry->d_name, !S_ISREG(mode) || excl);
	error = PTR_ERR(inode);
	if (!IS_ERR(inode)) {
		d = d_splice_alias(inode, dentry);
		error = PTR_ERR(d);
		if (IS_ERR(d))
			goto fail_gunlock;
		error = 0;
		if (file) {
			if (S_ISREG(inode->i_mode)) {
				WARN_ON(d != NULL);
				error = finish_open(file, dentry, gfs2_open_common, opened);
			} else {
				error = finish_no_open(file, d);
			}
		} else {
			dput(d);
		}
		gfs2_glock_dq_uninit(ghs);
		return error;
	} else if (error != -ENOENT) {
		goto fail_gunlock;
	}

	error = gfs2_diradd_alloc_required(dir, name, &da);
	if (error < 0)
		goto fail_gunlock;

	inode = new_inode(sdp->sd_vfs);
	error = -ENOMEM;
	if (!inode)
		goto fail_gunlock;

	ip = GFS2_I(inode);
	error = gfs2_rs_alloc(ip);
	if (error)
		goto fail_free_inode;

	inode->i_mode = mode;
	set_nlink(inode, S_ISDIR(mode) ? 2 : 1);
	inode->i_rdev = dev;
	inode->i_size = size;
	inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;
	gfs2_set_inode_blocks(inode, 1);
	munge_mode_uid_gid(dip, inode);
	ip->i_goal = dip->i_goal;
	ip->i_diskflags = 0;
	ip->i_eattr = 0;
	ip->i_height = 0;
	ip->i_depth = 0;
	ip->i_entries = 0;

	switch(mode & S_IFMT) {
	case S_IFREG:
		if ((dip->i_diskflags & GFS2_DIF_INHERIT_JDATA) ||
		    gfs2_tune_get(sdp, gt_new_files_jdata))
			ip->i_diskflags |= GFS2_DIF_JDATA;
		gfs2_set_aops(inode);
		break;
	case S_IFDIR:
		ip->i_diskflags |= (dip->i_diskflags & GFS2_DIF_INHERIT_JDATA);
		ip->i_diskflags |= GFS2_DIF_JDATA;
		ip->i_entries = 2;
		break;
	}
	gfs2_set_inode_flags(inode);

	if ((GFS2_I(sdp->sd_root_dir->d_inode) == dip) ||
	    (dip->i_diskflags & GFS2_DIF_TOPDIR))
		aflags |= GFS2_AF_ORLOV;

	error = alloc_dinode(ip, aflags);
	if (error)
		goto fail_free_inode;

	error = gfs2_glock_get(sdp, ip->i_no_addr, &gfs2_inode_glops, CREATE, &ip->i_gl);
	if (error)
		goto fail_free_inode;

	ip->i_gl->gl_object = ip;
	error = gfs2_glock_nq_init(ip->i_gl, LM_ST_EXCLUSIVE, GL_SKIP, ghs + 1);
	if (error)
		goto fail_free_inode;

	error = gfs2_trans_begin(sdp, RES_DINODE, 0);
	if (error)
		goto fail_gunlock2;

	init_dinode(dip, ip, symname);
	gfs2_trans_end(sdp);

	error = gfs2_glock_get(sdp, ip->i_no_addr, &gfs2_iopen_glops, CREATE, &io_gl);
	if (error)
		goto fail_gunlock2;

	error = gfs2_glock_nq_init(io_gl, LM_ST_SHARED, GL_EXACT, &ip->i_iopen_gh);
	if (error)
		goto fail_gunlock2;

	ip->i_iopen_gh.gh_gl->gl_object = ip;
	gfs2_glock_put(io_gl);
	gfs2_set_iop(inode);
	insert_inode_hash(inode);

	error = gfs2_acl_create(dip, inode);
	if (error)
		goto fail_gunlock3;

	error = gfs2_security_init(dip, ip, name);
	if (error)
		goto fail_gunlock3;

	error = link_dinode(dip, name, ip, &da);
	if (error)
		goto fail_gunlock3;

	mark_inode_dirty(inode);
	d_instantiate(dentry, inode);
	if (file) {
		*opened |= FILE_CREATED;
		error = finish_open(file, dentry, gfs2_open_common, opened);
	}
	gfs2_glock_dq_uninit(ghs);
	gfs2_glock_dq_uninit(ghs + 1);
	return error;

fail_gunlock3:
	gfs2_glock_dq_uninit(ghs + 1);
	if (ip->i_gl)
		gfs2_glock_put(ip->i_gl);
	goto fail_gunlock;

fail_gunlock2:
	gfs2_glock_dq_uninit(ghs + 1);
fail_free_inode:
	if (ip->i_gl)
		gfs2_glock_put(ip->i_gl);
	gfs2_rs_delete(ip, NULL);
	free_inode_nonrcu(inode);
	inode = NULL;
fail_gunlock:
	gfs2_dir_no_add(&da);
	gfs2_glock_dq_uninit(ghs);
	if (inode && !IS_ERR(inode)) {
		clear_nlink(inode);
		mark_inode_dirty(inode);
		set_bit(GIF_ALLOC_FAILED, &GFS2_I(inode)->i_flags);
		iput(inode);
	}
fail:
>>>>>>> refs/remotes/origin/master
	return error;
}

/**
 * gfs2_create - Create a file
 * @dir: The directory in which to create the file
 * @dentry: The dentry of the new file
 * @mode: The mode of the new file
 *
 * Returns: errno
 */

static int gfs2_create(struct inode *dir, struct dentry *dentry,
<<<<<<< HEAD
<<<<<<< HEAD
		       int mode, struct nameidata *nd)
{
	struct inode *inode;
	int ret;

	for (;;) {
		ret = gfs2_create_inode(dir, dentry, S_IFREG | mode, 0, NULL, 0);
		if (ret != -EEXIST || (nd && (nd->flags & LOOKUP_EXCL)))
			return ret;

		inode = gfs2_lookupi(dir, &dentry->d_name, 0);
		if (inode) {
			if (!IS_ERR(inode))
				break;
			return PTR_ERR(inode);
		}
	}

	d_instantiate(dentry, inode);
	return 0;
=======
		       umode_t mode, struct nameidata *nd)
{
	int excl = 0;
	if (nd && (nd->flags & LOOKUP_EXCL))
		excl = 1;
	return gfs2_create_inode(dir, dentry, S_IFREG | mode, 0, NULL, 0, excl);
>>>>>>> refs/remotes/origin/cm-10.0
}

/**
 * gfs2_lookup - Look up a filename in a directory and return its inode
 * @dir: The directory inode
 * @dentry: The dentry of the new inode
 * @nd: passed from Linux VFS, ignored by us
 *
 * Called by the VFS layer. Lock dir and call gfs2_lookupi()
=======
		       umode_t mode, bool excl)
{
	return gfs2_create_inode(dir, dentry, NULL, S_IFREG | mode, 0, NULL, 0, excl, NULL);
}

/**
 * __gfs2_lookup - Look up a filename in a directory and return its inode
 * @dir: The directory inode
 * @dentry: The dentry of the new inode
 * @file: File to be opened
 * @opened: atomic_open flags
 *
>>>>>>> refs/remotes/origin/master
 *
 * Returns: errno
 */

<<<<<<< HEAD
static struct dentry *gfs2_lookup(struct inode *dir, struct dentry *dentry,
				  struct nameidata *nd)
{
<<<<<<< HEAD
	struct inode *inode = NULL;

	inode = gfs2_lookupi(dir, &dentry->d_name, 0);
	if (inode && IS_ERR(inode))
		return ERR_CAST(inode);

	if (inode) {
=======
	struct inode *inode = gfs2_lookupi(dir, &dentry->d_name, 0);
	if (inode && !IS_ERR(inode)) {
>>>>>>> refs/remotes/origin/cm-10.0
		struct gfs2_glock *gl = GFS2_I(inode)->i_gl;
		struct gfs2_holder gh;
		int error;
		error = gfs2_glock_nq_init(gl, LM_ST_SHARED, LM_FLAG_ANY, &gh);
		if (error) {
			iput(inode);
			return ERR_PTR(error);
		}
		gfs2_glock_dq_uninit(&gh);
<<<<<<< HEAD
		return d_splice_alias(inode, dentry);
	}
	d_add(dentry, inode);

	return NULL;
=======
	}
	return d_splice_alias(inode, dentry);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct dentry *__gfs2_lookup(struct inode *dir, struct dentry *dentry,
				    struct file *file, int *opened)
{
	struct inode *inode;
	struct dentry *d;
	struct gfs2_holder gh;
	struct gfs2_glock *gl;
	int error;

	inode = gfs2_lookupi(dir, &dentry->d_name, 0);
	if (!inode)
		return NULL;
	if (IS_ERR(inode))
		return ERR_CAST(inode);

	gl = GFS2_I(inode)->i_gl;
	error = gfs2_glock_nq_init(gl, LM_ST_SHARED, LM_FLAG_ANY, &gh);
	if (error) {
		iput(inode);
		return ERR_PTR(error);
	}

	d = d_splice_alias(inode, dentry);
	if (IS_ERR(d)) {
		iput(inode);
		gfs2_glock_dq_uninit(&gh);
		return d;
	}
	if (file && S_ISREG(inode->i_mode))
		error = finish_open(file, dentry, gfs2_open_common, opened);

	gfs2_glock_dq_uninit(&gh);
	if (error) {
		dput(d);
		return ERR_PTR(error);
	}
	return d;
}

static struct dentry *gfs2_lookup(struct inode *dir, struct dentry *dentry,
				  unsigned flags)
{
	return __gfs2_lookup(dir, dentry, NULL, NULL);
>>>>>>> refs/remotes/origin/master
}

/**
 * gfs2_link - Link to a file
 * @old_dentry: The inode to link
 * @dir: Add link to this directory
 * @dentry: The name of the link
 *
 * Link the inode in "old_dentry" into the directory "dir" with the
 * name in "dentry".
 *
 * Returns: errno
 */

static int gfs2_link(struct dentry *old_dentry, struct inode *dir,
		     struct dentry *dentry)
{
	struct gfs2_inode *dip = GFS2_I(dir);
	struct gfs2_sbd *sdp = GFS2_SB(dir);
	struct inode *inode = old_dentry->d_inode;
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_holder ghs[2];
	struct buffer_head *dibh;
<<<<<<< HEAD
	int alloc_required;
=======
	struct gfs2_diradd da = { .bh = NULL, };
>>>>>>> refs/remotes/origin/master
	int error;

	if (S_ISDIR(inode->i_mode))
		return -EPERM;

<<<<<<< HEAD
=======
	error = gfs2_rs_alloc(dip);
	if (error)
		return error;

>>>>>>> refs/remotes/origin/master
	gfs2_holder_init(dip->i_gl, LM_ST_EXCLUSIVE, 0, ghs);
	gfs2_holder_init(ip->i_gl, LM_ST_EXCLUSIVE, 0, ghs + 1);

	error = gfs2_glock_nq(ghs); /* parent */
	if (error)
		goto out_parent;

	error = gfs2_glock_nq(ghs + 1); /* child */
	if (error)
		goto out_child;

	error = -ENOENT;
	if (inode->i_nlink == 0)
		goto out_gunlock;

<<<<<<< HEAD
<<<<<<< HEAD
	error = gfs2_permission(dir, MAY_WRITE | MAY_EXEC, 0);
=======
	error = gfs2_permission(dir, MAY_WRITE | MAY_EXEC);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	error = gfs2_permission(dir, MAY_WRITE | MAY_EXEC);
>>>>>>> refs/remotes/origin/master
	if (error)
		goto out_gunlock;

	error = gfs2_dir_check(dir, &dentry->d_name, NULL);
	switch (error) {
	case -ENOENT:
		break;
	case 0:
		error = -EEXIST;
	default:
		goto out_gunlock;
	}

	error = -EINVAL;
	if (!dip->i_inode.i_nlink)
		goto out_gunlock;
	error = -EFBIG;
	if (dip->i_entries == (u32)-1)
		goto out_gunlock;
	error = -EPERM;
	if (IS_IMMUTABLE(inode) || IS_APPEND(inode))
		goto out_gunlock;
	error = -EINVAL;
	if (!ip->i_inode.i_nlink)
		goto out_gunlock;
	error = -EMLINK;
	if (ip->i_inode.i_nlink == (u32)-1)
		goto out_gunlock;

<<<<<<< HEAD
	alloc_required = error = gfs2_diradd_alloc_required(dir, &dentry->d_name);
	if (error < 0)
		goto out_gunlock;
	error = 0;

	if (alloc_required) {
<<<<<<< HEAD
		struct gfs2_alloc *al = gfs2_alloc_get(dip);
		if (!al) {
=======
		struct gfs2_qadata *qa = gfs2_qadata_get(dip);

		if (!qa) {
>>>>>>> refs/remotes/origin/cm-10.0
			error = -ENOMEM;
			goto out_gunlock;
		}

		error = gfs2_quota_lock_check(dip);
		if (error)
			goto out_alloc;

<<<<<<< HEAD
		al->al_requested = sdp->sd_max_dirres;

		error = gfs2_inplace_reserve(dip);
=======
		error = gfs2_inplace_reserve(dip, sdp->sd_max_dirres);
>>>>>>> refs/remotes/origin/cm-10.0
		if (error)
			goto out_gunlock_q;

		error = gfs2_trans_begin(sdp, sdp->sd_max_dirres +
<<<<<<< HEAD
					 gfs2_rg_blocks(al) +
=======
					 gfs2_rg_blocks(dip) +
>>>>>>> refs/remotes/origin/cm-10.0
					 2 * RES_DINODE + RES_STATFS +
					 RES_QUOTA, 0);
=======
	error = gfs2_diradd_alloc_required(dir, &dentry->d_name, &da);
	if (error < 0)
		goto out_gunlock;

	if (da.nr_blocks) {
		struct gfs2_alloc_parms ap = { .target = da.nr_blocks, };
		error = gfs2_quota_lock_check(dip);
		if (error)
			goto out_gunlock;

		error = gfs2_inplace_reserve(dip, &ap);
		if (error)
			goto out_gunlock_q;

		error = gfs2_trans_begin(sdp, gfs2_trans_da_blks(dip, &da, 2), 0);
>>>>>>> refs/remotes/origin/master
		if (error)
			goto out_ipres;
	} else {
		error = gfs2_trans_begin(sdp, 2 * RES_DINODE + RES_LEAF, 0);
		if (error)
			goto out_ipres;
	}

	error = gfs2_meta_inode_buffer(ip, &dibh);
	if (error)
		goto out_end_trans;

<<<<<<< HEAD
	error = gfs2_dir_add(dir, &dentry->d_name, ip);
	if (error)
		goto out_brelse;

	gfs2_trans_add_bh(ip->i_gl, dibh, 1);
	inc_nlink(&ip->i_inode);
	ip->i_inode.i_ctime = CURRENT_TIME;
<<<<<<< HEAD
	gfs2_dinode_out(ip, dibh->b_data);
	mark_inode_dirty(&ip->i_inode);
=======
	ihold(inode);
	d_instantiate(dentry, inode);
	mark_inode_dirty(inode);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	error = gfs2_dir_add(dir, &dentry->d_name, ip, &da);
	if (error)
		goto out_brelse;

	gfs2_trans_add_meta(ip->i_gl, dibh);
	inc_nlink(&ip->i_inode);
	ip->i_inode.i_ctime = CURRENT_TIME;
	ihold(inode);
	d_instantiate(dentry, inode);
	mark_inode_dirty(inode);
>>>>>>> refs/remotes/origin/master

out_brelse:
	brelse(dibh);
out_end_trans:
	gfs2_trans_end(sdp);
out_ipres:
<<<<<<< HEAD
	if (alloc_required)
		gfs2_inplace_release(dip);
out_gunlock_q:
	if (alloc_required)
		gfs2_quota_unlock(dip);
out_alloc:
	if (alloc_required)
<<<<<<< HEAD
		gfs2_alloc_put(dip);
=======
		gfs2_qadata_put(dip);
>>>>>>> refs/remotes/origin/cm-10.0
out_gunlock:
=======
	if (da.nr_blocks)
		gfs2_inplace_release(dip);
out_gunlock_q:
	if (da.nr_blocks)
		gfs2_quota_unlock(dip);
out_gunlock:
	gfs2_dir_no_add(&da);
>>>>>>> refs/remotes/origin/master
	gfs2_glock_dq(ghs + 1);
out_child:
	gfs2_glock_dq(ghs);
out_parent:
	gfs2_holder_uninit(ghs);
	gfs2_holder_uninit(ghs + 1);
<<<<<<< HEAD
<<<<<<< HEAD
	if (!error) {
		ihold(inode);
		d_instantiate(dentry, inode);
		mark_inode_dirty(inode);
	}
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return error;
}

/*
 * gfs2_unlink_ok - check to see that a inode is still in a directory
 * @dip: the directory
 * @name: the name of the file
 * @ip: the inode
 *
 * Assumes that the lock on (at least) @dip is held.
 *
 * Returns: 0 if the parent/child relationship is correct, errno if it isn't
 */

static int gfs2_unlink_ok(struct gfs2_inode *dip, const struct qstr *name,
			  const struct gfs2_inode *ip)
{
	int error;

	if (IS_IMMUTABLE(&ip->i_inode) || IS_APPEND(&ip->i_inode))
		return -EPERM;

	if ((dip->i_inode.i_mode & S_ISVTX) &&
<<<<<<< HEAD
	    dip->i_inode.i_uid != current_fsuid() &&
	    ip->i_inode.i_uid != current_fsuid() && !capable(CAP_FOWNER))
=======
	    !uid_eq(dip->i_inode.i_uid, current_fsuid()) &&
	    !uid_eq(ip->i_inode.i_uid, current_fsuid()) && !capable(CAP_FOWNER))
>>>>>>> refs/remotes/origin/master
		return -EPERM;

	if (IS_APPEND(&dip->i_inode))
		return -EPERM;

<<<<<<< HEAD
<<<<<<< HEAD
	error = gfs2_permission(&dip->i_inode, MAY_WRITE | MAY_EXEC, 0);
=======
	error = gfs2_permission(&dip->i_inode, MAY_WRITE | MAY_EXEC);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	error = gfs2_permission(&dip->i_inode, MAY_WRITE | MAY_EXEC);
>>>>>>> refs/remotes/origin/master
	if (error)
		return error;

	error = gfs2_dir_check(&dip->i_inode, name, ip);
	if (error)
		return error;

	return 0;
}

/**
 * gfs2_unlink_inode - Removes an inode from its parent dir and unlinks it
 * @dip: The parent directory
 * @name: The name of the entry in the parent directory
<<<<<<< HEAD
 * @bh: The inode buffer for the inode to be removed
=======
>>>>>>> refs/remotes/origin/master
 * @inode: The inode to be removed
 *
 * Called with all the locks and in a transaction. This will only be
 * called for a directory after it has been checked to ensure it is empty.
 *
 * Returns: 0 on success, or an error
 */

static int gfs2_unlink_inode(struct gfs2_inode *dip,
<<<<<<< HEAD
			     const struct dentry *dentry,
			     struct buffer_head *bh)
=======
			     const struct dentry *dentry)
>>>>>>> refs/remotes/origin/master
{
	struct inode *inode = dentry->d_inode;
	struct gfs2_inode *ip = GFS2_I(inode);
	int error;

	error = gfs2_dir_del(dip, dentry);
	if (error)
		return error;

	ip->i_entries = 0;
	inode->i_ctime = CURRENT_TIME;
	if (S_ISDIR(inode->i_mode))
		clear_nlink(inode);
	else
		drop_nlink(inode);
<<<<<<< HEAD
<<<<<<< HEAD
	gfs2_trans_add_bh(ip->i_gl, bh, 1);
	gfs2_dinode_out(ip, bh->b_data);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	mark_inode_dirty(inode);
	if (inode->i_nlink == 0)
		gfs2_unlink_di(inode);
	return 0;
}


/**
 * gfs2_unlink - Unlink an inode (this does rmdir as well)
 * @dir: The inode of the directory containing the inode to unlink
 * @dentry: The file itself
 *
 * This routine uses the type of the inode as a flag to figure out
 * whether this is an unlink or an rmdir.
 *
 * Returns: errno
 */

static int gfs2_unlink(struct inode *dir, struct dentry *dentry)
{
	struct gfs2_inode *dip = GFS2_I(dir);
	struct gfs2_sbd *sdp = GFS2_SB(dir);
	struct inode *inode = dentry->d_inode;
	struct gfs2_inode *ip = GFS2_I(inode);
<<<<<<< HEAD
	struct buffer_head *bh;
	struct gfs2_holder ghs[3];
	struct gfs2_rgrpd *rgd;
<<<<<<< HEAD
	struct gfs2_holder ri_gh;
	int error;

	error = gfs2_rindex_hold(sdp, &ri_gh);
	if (error)
		return error;

	gfs2_holder_init(dip->i_gl, LM_ST_EXCLUSIVE, 0, ghs);
	gfs2_holder_init(ip->i_gl,  LM_ST_EXCLUSIVE, 0, ghs + 1);

	rgd = gfs2_blk2rgrpd(sdp, ip->i_no_addr);
=======
=======
	struct gfs2_holder ghs[3];
	struct gfs2_rgrpd *rgd;
>>>>>>> refs/remotes/origin/master
	int error;

	error = gfs2_rindex_update(sdp);
	if (error)
		return error;

	error = -EROFS;

	gfs2_holder_init(dip->i_gl, LM_ST_EXCLUSIVE, 0, ghs);
	gfs2_holder_init(ip->i_gl,  LM_ST_EXCLUSIVE, 0, ghs + 1);

	rgd = gfs2_blk2rgrpd(sdp, ip->i_no_addr, 1);
	if (!rgd)
		goto out_inodes;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	gfs2_holder_init(rgd->rd_gl, LM_ST_EXCLUSIVE, 0, ghs + 2);


	error = gfs2_glock_nq(ghs); /* parent */
	if (error)
		goto out_parent;

	error = gfs2_glock_nq(ghs + 1); /* child */
	if (error)
		goto out_child;

	error = -ENOENT;
	if (inode->i_nlink == 0)
		goto out_rgrp;

	if (S_ISDIR(inode->i_mode)) {
		error = -ENOTEMPTY;
		if (ip->i_entries > 2 || inode->i_nlink > 2)
			goto out_rgrp;
	}

	error = gfs2_glock_nq(ghs + 2); /* rgrp */
	if (error)
		goto out_rgrp;

	error = gfs2_unlink_ok(dip, &dentry->d_name, ip);
	if (error)
		goto out_gunlock;

	error = gfs2_trans_begin(sdp, 2*RES_DINODE + 3*RES_LEAF + RES_RG_BIT, 0);
	if (error)
<<<<<<< HEAD
		goto out_gunlock;

	error = gfs2_meta_inode_buffer(ip, &bh);
	if (error)
		goto out_end_trans;

	error = gfs2_unlink_inode(dip, dentry, bh);
	brelse(bh);
=======
		goto out_end_trans;

	error = gfs2_unlink_inode(dip, dentry);
>>>>>>> refs/remotes/origin/master

out_end_trans:
	gfs2_trans_end(sdp);
out_gunlock:
	gfs2_glock_dq(ghs + 2);
out_rgrp:
<<<<<<< HEAD
<<<<<<< HEAD
	gfs2_holder_uninit(ghs + 2);
	gfs2_glock_dq(ghs + 1);
out_child:
	gfs2_holder_uninit(ghs + 1);
	gfs2_glock_dq(ghs);
out_parent:
	gfs2_holder_uninit(ghs);
	gfs2_glock_dq_uninit(&ri_gh);
=======
=======
>>>>>>> refs/remotes/origin/master
	gfs2_glock_dq(ghs + 1);
out_child:
	gfs2_glock_dq(ghs);
out_parent:
	gfs2_holder_uninit(ghs + 2);
out_inodes:
	gfs2_holder_uninit(ghs + 1);
	gfs2_holder_uninit(ghs);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return error;
}

/**
 * gfs2_symlink - Create a symlink
 * @dir: The directory to create the symlink in
 * @dentry: The dentry to put the symlink in
 * @symname: The thing which the link points to
 *
 * Returns: errno
 */

static int gfs2_symlink(struct inode *dir, struct dentry *dentry,
			const char *symname)
{
	struct gfs2_sbd *sdp = GFS2_SB(dir);
	unsigned int size;

	size = strlen(symname);
	if (size > sdp->sd_sb.sb_bsize - sizeof(struct gfs2_dinode) - 1)
		return -ENAMETOOLONG;

<<<<<<< HEAD
<<<<<<< HEAD
	return gfs2_create_inode(dir, dentry, S_IFLNK | S_IRWXUGO, 0, symname, size);
=======
	return gfs2_create_inode(dir, dentry, S_IFLNK | S_IRWXUGO, 0, symname, size, 0);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return gfs2_create_inode(dir, dentry, NULL, S_IFLNK | S_IRWXUGO, 0, symname, size, 0, NULL);
>>>>>>> refs/remotes/origin/master
}

/**
 * gfs2_mkdir - Make a directory
 * @dir: The parent directory of the new one
 * @dentry: The dentry of the new directory
 * @mode: The mode of the new directory
 *
 * Returns: errno
 */

<<<<<<< HEAD
<<<<<<< HEAD
static int gfs2_mkdir(struct inode *dir, struct dentry *dentry, int mode)
{
	return gfs2_create_inode(dir, dentry, S_IFDIR | mode, 0, NULL, 0);
=======
static int gfs2_mkdir(struct inode *dir, struct dentry *dentry, umode_t mode)
{
	return gfs2_create_inode(dir, dentry, S_IFDIR | mode, 0, NULL, 0, 0);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int gfs2_mkdir(struct inode *dir, struct dentry *dentry, umode_t mode)
{
	struct gfs2_sbd *sdp = GFS2_SB(dir);
	unsigned dsize = sdp->sd_sb.sb_bsize - sizeof(struct gfs2_dinode);
	return gfs2_create_inode(dir, dentry, NULL, S_IFDIR | mode, 0, NULL, dsize, 0, NULL);
>>>>>>> refs/remotes/origin/master
}

/**
 * gfs2_mknod - Make a special file
 * @dir: The directory in which the special file will reside
 * @dentry: The dentry of the special file
 * @mode: The mode of the special file
 * @dev: The device specification of the special file
 *
 */

<<<<<<< HEAD
<<<<<<< HEAD
static int gfs2_mknod(struct inode *dir, struct dentry *dentry, int mode,
		      dev_t dev)
{
	return gfs2_create_inode(dir, dentry, mode, dev, NULL, 0);
=======
static int gfs2_mknod(struct inode *dir, struct dentry *dentry, umode_t mode,
		      dev_t dev)
{
	return gfs2_create_inode(dir, dentry, mode, dev, NULL, 0, 0);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int gfs2_mknod(struct inode *dir, struct dentry *dentry, umode_t mode,
		      dev_t dev)
{
	return gfs2_create_inode(dir, dentry, NULL, mode, dev, NULL, 0, 0, NULL);
}

/**
 * gfs2_atomic_open - Atomically open a file
 * @dir: The directory
 * @dentry: The proposed new entry
 * @file: The proposed new struct file
 * @flags: open flags
 * @mode: File mode
 * @opened: Flag to say whether the file has been opened or not
 *
 * Returns: error code or 0 for success
 */

static int gfs2_atomic_open(struct inode *dir, struct dentry *dentry,
                            struct file *file, unsigned flags,
                            umode_t mode, int *opened)
{
	struct dentry *d;
	bool excl = !!(flags & O_EXCL);

	d = __gfs2_lookup(dir, dentry, file, opened);
	if (IS_ERR(d))
		return PTR_ERR(d);
	if (d != NULL)
		dentry = d;
	if (dentry->d_inode) {
		if (!(*opened & FILE_OPENED)) {
			if (d == NULL)
				dget(dentry);
			return finish_no_open(file, dentry);
		}
		dput(d);
		return 0;
	}

	BUG_ON(d != NULL);
	if (!(flags & O_CREAT))
		return -ENOENT;

	return gfs2_create_inode(dir, dentry, file, S_IFREG | mode, 0, NULL, 0, excl, opened);
>>>>>>> refs/remotes/origin/master
}

/*
 * gfs2_ok_to_move - check if it's ok to move a directory to another directory
 * @this: move this
 * @to: to here
 *
 * Follow @to back to the root and make sure we don't encounter @this
 * Assumes we already hold the rename lock.
 *
 * Returns: errno
 */

static int gfs2_ok_to_move(struct gfs2_inode *this, struct gfs2_inode *to)
{
	struct inode *dir = &to->i_inode;
	struct super_block *sb = dir->i_sb;
	struct inode *tmp;
	int error = 0;

	igrab(dir);

	for (;;) {
		if (dir == &this->i_inode) {
			error = -EINVAL;
			break;
		}
		if (dir == sb->s_root->d_inode) {
			error = 0;
			break;
		}

		tmp = gfs2_lookupi(dir, &gfs2_qdotdot, 1);
		if (IS_ERR(tmp)) {
			error = PTR_ERR(tmp);
			break;
		}

		iput(dir);
		dir = tmp;
	}

	iput(dir);

	return error;
}

/**
 * gfs2_rename - Rename a file
 * @odir: Parent directory of old file name
 * @odentry: The old dentry of the file
 * @ndir: Parent directory of new file name
 * @ndentry: The new dentry of the file
 *
 * Returns: errno
 */

static int gfs2_rename(struct inode *odir, struct dentry *odentry,
		       struct inode *ndir, struct dentry *ndentry)
{
	struct gfs2_inode *odip = GFS2_I(odir);
	struct gfs2_inode *ndip = GFS2_I(ndir);
	struct gfs2_inode *ip = GFS2_I(odentry->d_inode);
	struct gfs2_inode *nip = NULL;
	struct gfs2_sbd *sdp = GFS2_SB(odir);
<<<<<<< HEAD
<<<<<<< HEAD
	struct gfs2_holder ghs[5], r_gh = { .gh_gl = NULL, }, ri_gh;
=======
	struct gfs2_holder ghs[5], r_gh = { .gh_gl = NULL, };
>>>>>>> refs/remotes/origin/cm-10.0
	struct gfs2_rgrpd *nrgd;
	unsigned int num_gh;
	int dir_rename = 0;
	int alloc_required = 0;
=======
	struct gfs2_holder ghs[5], r_gh = { .gh_gl = NULL, };
	struct gfs2_rgrpd *nrgd;
	unsigned int num_gh;
	int dir_rename = 0;
	struct gfs2_diradd da = { .nr_blocks = 0, };
>>>>>>> refs/remotes/origin/master
	unsigned int x;
	int error;

	if (ndentry->d_inode) {
		nip = GFS2_I(ndentry->d_inode);
		if (ip == nip)
			return 0;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	error = gfs2_rindex_hold(sdp, &ri_gh);
=======
	error = gfs2_rindex_update(sdp);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	error = gfs2_rindex_update(sdp);
	if (error)
		return error;

	error = gfs2_rs_alloc(ndip);
>>>>>>> refs/remotes/origin/master
	if (error)
		return error;

	if (odip != ndip) {
		error = gfs2_glock_nq_init(sdp->sd_rename_gl, LM_ST_EXCLUSIVE,
					   0, &r_gh);
		if (error)
			goto out;

		if (S_ISDIR(ip->i_inode.i_mode)) {
			dir_rename = 1;
			/* don't move a dirctory into it's subdir */
			error = gfs2_ok_to_move(ip, ndip);
			if (error)
				goto out_gunlock_r;
		}
	}

	num_gh = 1;
	gfs2_holder_init(odip->i_gl, LM_ST_EXCLUSIVE, 0, ghs);
	if (odip != ndip) {
		gfs2_holder_init(ndip->i_gl, LM_ST_EXCLUSIVE, 0, ghs + num_gh);
		num_gh++;
	}
	gfs2_holder_init(ip->i_gl, LM_ST_EXCLUSIVE, 0, ghs + num_gh);
	num_gh++;

	if (nip) {
		gfs2_holder_init(nip->i_gl, LM_ST_EXCLUSIVE, 0, ghs + num_gh);
		num_gh++;
		/* grab the resource lock for unlink flag twiddling 
		 * this is the case of the target file already existing
		 * so we unlink before doing the rename
		 */
<<<<<<< HEAD
<<<<<<< HEAD
		nrgd = gfs2_blk2rgrpd(sdp, nip->i_no_addr);
=======
		nrgd = gfs2_blk2rgrpd(sdp, nip->i_no_addr, 1);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		nrgd = gfs2_blk2rgrpd(sdp, nip->i_no_addr, 1);
>>>>>>> refs/remotes/origin/master
		if (nrgd)
			gfs2_holder_init(nrgd->rd_gl, LM_ST_EXCLUSIVE, 0, ghs + num_gh++);
	}

	for (x = 0; x < num_gh; x++) {
		error = gfs2_glock_nq(ghs + x);
		if (error)
			goto out_gunlock;
	}

	error = -ENOENT;
	if (ip->i_inode.i_nlink == 0)
		goto out_gunlock;

	/* Check out the old directory */

	error = gfs2_unlink_ok(odip, &odentry->d_name, ip);
	if (error)
		goto out_gunlock;

	/* Check out the new directory */

	if (nip) {
		error = gfs2_unlink_ok(ndip, &ndentry->d_name, nip);
		if (error)
			goto out_gunlock;

		if (nip->i_inode.i_nlink == 0) {
			error = -EAGAIN;
			goto out_gunlock;
		}

		if (S_ISDIR(nip->i_inode.i_mode)) {
			if (nip->i_entries < 2) {
				gfs2_consist_inode(nip);
				error = -EIO;
				goto out_gunlock;
			}
			if (nip->i_entries > 2) {
				error = -ENOTEMPTY;
				goto out_gunlock;
			}
		}
	} else {
<<<<<<< HEAD
<<<<<<< HEAD
		error = gfs2_permission(ndir, MAY_WRITE | MAY_EXEC, 0);
=======
		error = gfs2_permission(ndir, MAY_WRITE | MAY_EXEC);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		error = gfs2_permission(ndir, MAY_WRITE | MAY_EXEC);
>>>>>>> refs/remotes/origin/master
		if (error)
			goto out_gunlock;

		error = gfs2_dir_check(ndir, &ndentry->d_name, NULL);
		switch (error) {
		case -ENOENT:
			error = 0;
			break;
		case 0:
			error = -EEXIST;
		default:
			goto out_gunlock;
		};

		if (odip != ndip) {
			if (!ndip->i_inode.i_nlink) {
				error = -ENOENT;
				goto out_gunlock;
			}
			if (ndip->i_entries == (u32)-1) {
				error = -EFBIG;
				goto out_gunlock;
			}
			if (S_ISDIR(ip->i_inode.i_mode) &&
			    ndip->i_inode.i_nlink == (u32)-1) {
				error = -EMLINK;
				goto out_gunlock;
			}
		}
	}

	/* Check out the dir to be renamed */

	if (dir_rename) {
<<<<<<< HEAD
<<<<<<< HEAD
		error = gfs2_permission(odentry->d_inode, MAY_WRITE, 0);
=======
		error = gfs2_permission(odentry->d_inode, MAY_WRITE);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		error = gfs2_permission(odentry->d_inode, MAY_WRITE);
>>>>>>> refs/remotes/origin/master
		if (error)
			goto out_gunlock;
	}

<<<<<<< HEAD
	if (nip == NULL)
		alloc_required = gfs2_diradd_alloc_required(ndir, &ndentry->d_name);
	error = alloc_required;
	if (error < 0)
		goto out_gunlock;
<<<<<<< HEAD
	error = 0;

	if (alloc_required) {
		struct gfs2_alloc *al = gfs2_alloc_get(ndip);
		if (!al) {
=======

	if (alloc_required) {
		struct gfs2_qadata *qa = gfs2_qadata_get(ndip);

		if (!qa) {
>>>>>>> refs/remotes/origin/cm-10.0
			error = -ENOMEM;
			goto out_gunlock;
		}

		error = gfs2_quota_lock_check(ndip);
		if (error)
			goto out_alloc;

<<<<<<< HEAD
		al->al_requested = sdp->sd_max_dirres;

		error = gfs2_inplace_reserve_ri(ndip);
=======
		error = gfs2_inplace_reserve(ndip, sdp->sd_max_dirres);
>>>>>>> refs/remotes/origin/cm-10.0
		if (error)
			goto out_gunlock_q;

		error = gfs2_trans_begin(sdp, sdp->sd_max_dirres +
<<<<<<< HEAD
					 gfs2_rg_blocks(al) +
=======
					 gfs2_rg_blocks(ndip) +
>>>>>>> refs/remotes/origin/cm-10.0
					 4 * RES_DINODE + 4 * RES_LEAF +
					 RES_STATFS + RES_QUOTA + 4, 0);
=======
	if (nip == NULL) {
		error = gfs2_diradd_alloc_required(ndir, &ndentry->d_name, &da);
		if (error)
			goto out_gunlock;
	}

	if (da.nr_blocks) {
		struct gfs2_alloc_parms ap = { .target = da.nr_blocks, };
		error = gfs2_quota_lock_check(ndip);
		if (error)
			goto out_gunlock;

		error = gfs2_inplace_reserve(ndip, &ap);
		if (error)
			goto out_gunlock_q;

		error = gfs2_trans_begin(sdp, gfs2_trans_da_blks(ndip, &da, 4) +
					 4 * RES_LEAF + 4, 0);
>>>>>>> refs/remotes/origin/master
		if (error)
			goto out_ipreserv;
	} else {
		error = gfs2_trans_begin(sdp, 4 * RES_DINODE +
					 5 * RES_LEAF + 4, 0);
		if (error)
			goto out_gunlock;
	}

	/* Remove the target file, if it exists */

<<<<<<< HEAD
	if (nip) {
		struct buffer_head *bh;
		error = gfs2_meta_inode_buffer(nip, &bh);
		if (error)
			goto out_end_trans;
		error = gfs2_unlink_inode(ndip, ndentry, bh);
		brelse(bh);
	}
=======
	if (nip)
		error = gfs2_unlink_inode(ndip, ndentry);
>>>>>>> refs/remotes/origin/master

	if (dir_rename) {
		error = gfs2_dir_mvino(ip, &gfs2_qdotdot, ndip, DT_DIR);
		if (error)
			goto out_end_trans;
	} else {
		struct buffer_head *dibh;
		error = gfs2_meta_inode_buffer(ip, &dibh);
		if (error)
			goto out_end_trans;
		ip->i_inode.i_ctime = CURRENT_TIME;
<<<<<<< HEAD
		gfs2_trans_add_bh(ip->i_gl, dibh, 1);
=======
		gfs2_trans_add_meta(ip->i_gl, dibh);
>>>>>>> refs/remotes/origin/master
		gfs2_dinode_out(ip, dibh->b_data);
		brelse(dibh);
	}

	error = gfs2_dir_del(odip, odentry);
	if (error)
		goto out_end_trans;

<<<<<<< HEAD
	error = gfs2_dir_add(ndir, &ndentry->d_name, ip);
=======
	error = gfs2_dir_add(ndir, &ndentry->d_name, ip, &da);
>>>>>>> refs/remotes/origin/master
	if (error)
		goto out_end_trans;

out_end_trans:
	gfs2_trans_end(sdp);
out_ipreserv:
<<<<<<< HEAD
	if (alloc_required)
		gfs2_inplace_release(ndip);
out_gunlock_q:
	if (alloc_required)
		gfs2_quota_unlock(ndip);
out_alloc:
	if (alloc_required)
<<<<<<< HEAD
		gfs2_alloc_put(ndip);
=======
		gfs2_qadata_put(ndip);
>>>>>>> refs/remotes/origin/cm-10.0
out_gunlock:
=======
	if (da.nr_blocks)
		gfs2_inplace_release(ndip);
out_gunlock_q:
	if (da.nr_blocks)
		gfs2_quota_unlock(ndip);
out_gunlock:
	gfs2_dir_no_add(&da);
>>>>>>> refs/remotes/origin/master
	while (x--) {
		gfs2_glock_dq(ghs + x);
		gfs2_holder_uninit(ghs + x);
	}
out_gunlock_r:
	if (r_gh.gh_gl)
		gfs2_glock_dq_uninit(&r_gh);
out:
<<<<<<< HEAD
<<<<<<< HEAD
	gfs2_glock_dq_uninit(&ri_gh);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return error;
}

/**
 * gfs2_follow_link - Follow a symbolic link
 * @dentry: The dentry of the link
 * @nd: Data that we pass to vfs_follow_link()
 *
 * This can handle symlinks of any size.
 *
 * Returns: 0 on success or error code
 */

static void *gfs2_follow_link(struct dentry *dentry, struct nameidata *nd)
{
	struct gfs2_inode *ip = GFS2_I(dentry->d_inode);
	struct gfs2_holder i_gh;
	struct buffer_head *dibh;
	unsigned int size;
	char *buf;
	int error;

	gfs2_holder_init(ip->i_gl, LM_ST_SHARED, 0, &i_gh);
	error = gfs2_glock_nq(&i_gh);
	if (error) {
		gfs2_holder_uninit(&i_gh);
		nd_set_link(nd, ERR_PTR(error));
		return NULL;
	}

	size = (unsigned int)i_size_read(&ip->i_inode);
	if (size == 0) {
		gfs2_consist_inode(ip);
		buf = ERR_PTR(-EIO);
		goto out;
	}

	error = gfs2_meta_inode_buffer(ip, &dibh);
	if (error) {
		buf = ERR_PTR(error);
		goto out;
	}

	buf = kzalloc(size + 1, GFP_NOFS);
	if (!buf)
		buf = ERR_PTR(-ENOMEM);
	else
		memcpy(buf, dibh->b_data + sizeof(struct gfs2_dinode), size);
	brelse(dibh);
out:
	gfs2_glock_dq_uninit(&i_gh);
	nd_set_link(nd, buf);
	return NULL;
}

<<<<<<< HEAD
static void gfs2_put_link(struct dentry *dentry, struct nameidata *nd, void *p)
{
	char *s = nd_get_link(nd);
	if (!IS_ERR(s))
		kfree(s);
}

=======
>>>>>>> refs/remotes/origin/master
/**
 * gfs2_permission -
 * @inode: The inode
 * @mask: The mask to be tested
 * @flags: Indicates whether this is an RCU path walk or not
 *
 * This may be called from the VFS directly, or from within GFS2 with the
 * inode locked, so we look to see if the glock is already locked and only
 * lock the glock if its not already been done.
 *
 * Returns: errno
 */

<<<<<<< HEAD
<<<<<<< HEAD
int gfs2_permission(struct inode *inode, int mask, unsigned int flags)
=======
int gfs2_permission(struct inode *inode, int mask)
>>>>>>> refs/remotes/origin/cm-10.0
=======
int gfs2_permission(struct inode *inode, int mask)
>>>>>>> refs/remotes/origin/master
{
	struct gfs2_inode *ip;
	struct gfs2_holder i_gh;
	int error;
	int unlock = 0;


	ip = GFS2_I(inode);
	if (gfs2_glock_is_locked_by_me(ip->i_gl) == NULL) {
<<<<<<< HEAD
<<<<<<< HEAD
		if (flags & IPERM_FLAG_RCU)
=======
		if (mask & MAY_NOT_BLOCK)
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (mask & MAY_NOT_BLOCK)
>>>>>>> refs/remotes/origin/master
			return -ECHILD;
		error = gfs2_glock_nq_init(ip->i_gl, LM_ST_SHARED, LM_FLAG_ANY, &i_gh);
		if (error)
			return error;
		unlock = 1;
	}

	if ((mask & MAY_WRITE) && IS_IMMUTABLE(inode))
		error = -EACCES;
	else
<<<<<<< HEAD
<<<<<<< HEAD
		error = generic_permission(inode, mask, flags, gfs2_check_acl);
=======
		error = generic_permission(inode, mask);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		error = generic_permission(inode, mask);
>>>>>>> refs/remotes/origin/master
	if (unlock)
		gfs2_glock_dq_uninit(&i_gh);

	return error;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int __gfs2_setattr_simple(struct gfs2_inode *ip, struct iattr *attr)
{
	struct inode *inode = &ip->i_inode;
	struct buffer_head *dibh;
	int error;

	error = gfs2_meta_inode_buffer(ip, &dibh);
	if (error)
		return error;

	setattr_copy(inode, attr);
	mark_inode_dirty(inode);
	gfs2_trans_add_bh(ip->i_gl, dibh, 1);
	gfs2_dinode_out(ip, dibh->b_data);
	brelse(dibh);
=======
=======
>>>>>>> refs/remotes/origin/master
static int __gfs2_setattr_simple(struct inode *inode, struct iattr *attr)
{
	setattr_copy(inode, attr);
	mark_inode_dirty(inode);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}

/**
 * gfs2_setattr_simple -
 * @ip:
 * @attr:
 *
 * Returns: errno
 */

<<<<<<< HEAD
<<<<<<< HEAD
int gfs2_setattr_simple(struct gfs2_inode *ip, struct iattr *attr)
=======
int gfs2_setattr_simple(struct inode *inode, struct iattr *attr)
>>>>>>> refs/remotes/origin/cm-10.0
=======
int gfs2_setattr_simple(struct inode *inode, struct iattr *attr)
>>>>>>> refs/remotes/origin/master
{
	int error;

	if (current->journal_info)
<<<<<<< HEAD
<<<<<<< HEAD
		return __gfs2_setattr_simple(ip, attr);

	error = gfs2_trans_begin(GFS2_SB(&ip->i_inode), RES_DINODE, 0);
	if (error)
		return error;

	error = __gfs2_setattr_simple(ip, attr);
	gfs2_trans_end(GFS2_SB(&ip->i_inode));
=======
=======
>>>>>>> refs/remotes/origin/master
		return __gfs2_setattr_simple(inode, attr);

	error = gfs2_trans_begin(GFS2_SB(inode), RES_DINODE, 0);
	if (error)
		return error;

	error = __gfs2_setattr_simple(inode, attr);
	gfs2_trans_end(GFS2_SB(inode));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return error;
}

static int setattr_chown(struct inode *inode, struct iattr *attr)
{
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_sbd *sdp = GFS2_SB(inode);
<<<<<<< HEAD
	u32 ouid, ogid, nuid, ngid;
=======
	kuid_t ouid, nuid;
	kgid_t ogid, ngid;
>>>>>>> refs/remotes/origin/master
	int error;

	ouid = inode->i_uid;
	ogid = inode->i_gid;
	nuid = attr->ia_uid;
	ngid = attr->ia_gid;

<<<<<<< HEAD
	if (!(attr->ia_valid & ATTR_UID) || ouid == nuid)
		ouid = nuid = NO_QUOTA_CHANGE;
	if (!(attr->ia_valid & ATTR_GID) || ogid == ngid)
		ogid = ngid = NO_QUOTA_CHANGE;

<<<<<<< HEAD
	if (!gfs2_alloc_get(ip))
=======
	if (!gfs2_qadata_get(ip))
>>>>>>> refs/remotes/origin/cm-10.0
		return -ENOMEM;

	error = gfs2_quota_lock(ip, nuid, ngid);
	if (error)
		goto out_alloc;

	if (ouid != NO_QUOTA_CHANGE || ogid != NO_QUOTA_CHANGE) {
=======
	if (!(attr->ia_valid & ATTR_UID) || uid_eq(ouid, nuid))
		ouid = nuid = NO_UID_QUOTA_CHANGE;
	if (!(attr->ia_valid & ATTR_GID) || gid_eq(ogid, ngid))
		ogid = ngid = NO_GID_QUOTA_CHANGE;

	error = get_write_access(inode);
	if (error)
		return error;

	error = gfs2_rs_alloc(ip);
	if (error)
		goto out;

	error = gfs2_rindex_update(sdp);
	if (error)
		goto out;

	error = gfs2_quota_lock(ip, nuid, ngid);
	if (error)
		goto out;

	if (!uid_eq(ouid, NO_UID_QUOTA_CHANGE) ||
	    !gid_eq(ogid, NO_GID_QUOTA_CHANGE)) {
>>>>>>> refs/remotes/origin/master
		error = gfs2_quota_check(ip, nuid, ngid);
		if (error)
			goto out_gunlock_q;
	}

	error = gfs2_trans_begin(sdp, RES_DINODE + 2 * RES_QUOTA, 0);
	if (error)
		goto out_gunlock_q;

<<<<<<< HEAD
<<<<<<< HEAD
	error = gfs2_setattr_simple(ip, attr);
=======
	error = gfs2_setattr_simple(inode, attr);
>>>>>>> refs/remotes/origin/cm-10.0
	if (error)
		goto out_end_trans;

	if (ouid != NO_QUOTA_CHANGE || ogid != NO_QUOTA_CHANGE) {
=======
	error = gfs2_setattr_simple(inode, attr);
	if (error)
		goto out_end_trans;

	if (!uid_eq(ouid, NO_UID_QUOTA_CHANGE) ||
	    !gid_eq(ogid, NO_GID_QUOTA_CHANGE)) {
>>>>>>> refs/remotes/origin/master
		u64 blocks = gfs2_get_inode_blocks(&ip->i_inode);
		gfs2_quota_change(ip, -blocks, ouid, ogid);
		gfs2_quota_change(ip, blocks, nuid, ngid);
	}

out_end_trans:
	gfs2_trans_end(sdp);
out_gunlock_q:
	gfs2_quota_unlock(ip);
<<<<<<< HEAD
out_alloc:
<<<<<<< HEAD
	gfs2_alloc_put(ip);
=======
	gfs2_qadata_put(ip);
>>>>>>> refs/remotes/origin/cm-10.0
=======
out:
	put_write_access(inode);
>>>>>>> refs/remotes/origin/master
	return error;
}

/**
 * gfs2_setattr - Change attributes on an inode
 * @dentry: The dentry which is changing
 * @attr: The structure describing the change
 *
 * The VFS layer wants to change one or more of an inodes attributes.  Write
 * that change out to disk.
 *
 * Returns: errno
 */

static int gfs2_setattr(struct dentry *dentry, struct iattr *attr)
{
	struct inode *inode = dentry->d_inode;
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_holder i_gh;
	int error;

<<<<<<< HEAD
=======
	error = gfs2_rs_alloc(ip);
	if (error)
		return error;

>>>>>>> refs/remotes/origin/master
	error = gfs2_glock_nq_init(ip->i_gl, LM_ST_EXCLUSIVE, 0, &i_gh);
	if (error)
		return error;

	error = -EPERM;
	if (IS_IMMUTABLE(inode) || IS_APPEND(inode))
		goto out;

	error = inode_change_ok(inode, attr);
	if (error)
		goto out;

	if (attr->ia_valid & ATTR_SIZE)
		error = gfs2_setattr_size(inode, attr->ia_size);
	else if (attr->ia_valid & (ATTR_UID | ATTR_GID))
		error = setattr_chown(inode, attr);
	else if ((attr->ia_valid & ATTR_MODE) && IS_POSIXACL(inode))
		error = gfs2_acl_chmod(ip, attr);
	else
<<<<<<< HEAD
<<<<<<< HEAD
		error = gfs2_setattr_simple(ip, attr);

out:
	gfs2_glock_dq_uninit(&i_gh);
	if (!error)
		mark_inode_dirty(inode);
=======
=======
>>>>>>> refs/remotes/origin/master
		error = gfs2_setattr_simple(inode, attr);

out:
	if (!error)
		mark_inode_dirty(inode);
	gfs2_glock_dq_uninit(&i_gh);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return error;
}

/**
 * gfs2_getattr - Read out an inode's attributes
 * @mnt: The vfsmount the inode is being accessed from
 * @dentry: The dentry to stat
 * @stat: The inode's stats
 *
 * This may be called from the VFS directly, or from within GFS2 with the
 * inode locked, so we look to see if the glock is already locked and only
 * lock the glock if its not already been done. Note that its the NFS
 * readdirplus operation which causes this to be called (from filldir)
 * with the glock already held.
 *
 * Returns: errno
 */

static int gfs2_getattr(struct vfsmount *mnt, struct dentry *dentry,
			struct kstat *stat)
{
	struct inode *inode = dentry->d_inode;
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_holder gh;
	int error;
	int unlock = 0;

	if (gfs2_glock_is_locked_by_me(ip->i_gl) == NULL) {
		error = gfs2_glock_nq_init(ip->i_gl, LM_ST_SHARED, LM_FLAG_ANY, &gh);
		if (error)
			return error;
		unlock = 1;
	}

	generic_fillattr(inode, stat);
	if (unlock)
		gfs2_glock_dq_uninit(&gh);

	return 0;
}

static int gfs2_setxattr(struct dentry *dentry, const char *name,
			 const void *data, size_t size, int flags)
{
	struct inode *inode = dentry->d_inode;
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_holder gh;
	int ret;

	gfs2_holder_init(ip->i_gl, LM_ST_EXCLUSIVE, 0, &gh);
	ret = gfs2_glock_nq(&gh);
	if (ret == 0) {
<<<<<<< HEAD
		ret = generic_setxattr(dentry, name, data, size, flags);
=======
		ret = gfs2_rs_alloc(ip);
		if (ret == 0)
			ret = generic_setxattr(dentry, name, data, size, flags);
>>>>>>> refs/remotes/origin/master
		gfs2_glock_dq(&gh);
	}
	gfs2_holder_uninit(&gh);
	return ret;
}

static ssize_t gfs2_getxattr(struct dentry *dentry, const char *name,
			     void *data, size_t size)
{
	struct inode *inode = dentry->d_inode;
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_holder gh;
	int ret;

<<<<<<< HEAD
=======
	/* For selinux during lookup */
	if (gfs2_glock_is_locked_by_me(ip->i_gl))
		return generic_getxattr(dentry, name, data, size);

>>>>>>> refs/remotes/origin/master
	gfs2_holder_init(ip->i_gl, LM_ST_SHARED, LM_FLAG_ANY, &gh);
	ret = gfs2_glock_nq(&gh);
	if (ret == 0) {
		ret = generic_getxattr(dentry, name, data, size);
		gfs2_glock_dq(&gh);
	}
	gfs2_holder_uninit(&gh);
	return ret;
}

static int gfs2_removexattr(struct dentry *dentry, const char *name)
{
	struct inode *inode = dentry->d_inode;
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_holder gh;
	int ret;

	gfs2_holder_init(ip->i_gl, LM_ST_EXCLUSIVE, 0, &gh);
	ret = gfs2_glock_nq(&gh);
	if (ret == 0) {
<<<<<<< HEAD
		ret = generic_removexattr(dentry, name);
=======
		ret = gfs2_rs_alloc(ip);
		if (ret == 0)
			ret = generic_removexattr(dentry, name);
>>>>>>> refs/remotes/origin/master
		gfs2_glock_dq(&gh);
	}
	gfs2_holder_uninit(&gh);
	return ret;
}

static int gfs2_fiemap(struct inode *inode, struct fiemap_extent_info *fieinfo,
		       u64 start, u64 len)
{
	struct gfs2_inode *ip = GFS2_I(inode);
	struct gfs2_holder gh;
	int ret;

	ret = fiemap_check_flags(fieinfo, FIEMAP_FLAG_SYNC);
	if (ret)
		return ret;

	mutex_lock(&inode->i_mutex);

	ret = gfs2_glock_nq_init(ip->i_gl, LM_ST_SHARED, 0, &gh);
	if (ret)
		goto out;

	if (gfs2_is_stuffed(ip)) {
		u64 phys = ip->i_no_addr << inode->i_blkbits;
		u64 size = i_size_read(inode);
		u32 flags = FIEMAP_EXTENT_LAST|FIEMAP_EXTENT_NOT_ALIGNED|
			    FIEMAP_EXTENT_DATA_INLINE;
		phys += sizeof(struct gfs2_dinode);
		phys += start;
		if (start + len > size)
			len = size - start;
		if (start < size)
			ret = fiemap_fill_next_extent(fieinfo, start, phys,
						      len, flags);
		if (ret == 1)
			ret = 0;
	} else {
		ret = __generic_block_fiemap(inode, fieinfo, start, len,
					     gfs2_block_map);
	}

	gfs2_glock_dq_uninit(&gh);
out:
	mutex_unlock(&inode->i_mutex);
	return ret;
}

const struct inode_operations gfs2_file_iops = {
	.permission = gfs2_permission,
	.setattr = gfs2_setattr,
	.getattr = gfs2_getattr,
	.setxattr = gfs2_setxattr,
	.getxattr = gfs2_getxattr,
	.listxattr = gfs2_listxattr,
	.removexattr = gfs2_removexattr,
	.fiemap = gfs2_fiemap,
<<<<<<< HEAD
<<<<<<< HEAD
=======
	.get_acl = gfs2_get_acl,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.get_acl = gfs2_get_acl,
>>>>>>> refs/remotes/origin/master
};

const struct inode_operations gfs2_dir_iops = {
	.create = gfs2_create,
	.lookup = gfs2_lookup,
	.link = gfs2_link,
	.unlink = gfs2_unlink,
	.symlink = gfs2_symlink,
	.mkdir = gfs2_mkdir,
	.rmdir = gfs2_unlink,
	.mknod = gfs2_mknod,
	.rename = gfs2_rename,
	.permission = gfs2_permission,
	.setattr = gfs2_setattr,
	.getattr = gfs2_getattr,
	.setxattr = gfs2_setxattr,
	.getxattr = gfs2_getxattr,
	.listxattr = gfs2_listxattr,
	.removexattr = gfs2_removexattr,
	.fiemap = gfs2_fiemap,
<<<<<<< HEAD
<<<<<<< HEAD
=======
	.get_acl = gfs2_get_acl,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.get_acl = gfs2_get_acl,
	.atomic_open = gfs2_atomic_open,
>>>>>>> refs/remotes/origin/master
};

const struct inode_operations gfs2_symlink_iops = {
	.readlink = generic_readlink,
	.follow_link = gfs2_follow_link,
<<<<<<< HEAD
	.put_link = gfs2_put_link,
=======
	.put_link = kfree_put_link,
>>>>>>> refs/remotes/origin/master
	.permission = gfs2_permission,
	.setattr = gfs2_setattr,
	.getattr = gfs2_getattr,
	.setxattr = gfs2_setxattr,
	.getxattr = gfs2_getxattr,
	.listxattr = gfs2_listxattr,
	.removexattr = gfs2_removexattr,
	.fiemap = gfs2_fiemap,
<<<<<<< HEAD
<<<<<<< HEAD
=======
	.get_acl = gfs2_get_acl,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.get_acl = gfs2_get_acl,
>>>>>>> refs/remotes/origin/master
};

