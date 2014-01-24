/*
 *  linux/fs/hfs/dir.c
 *
 * Copyright (C) 1995-1997  Paul H. Hargrove
 * (C) 2003 Ardis Technologies <roman@ardistech.com>
 * This file may be distributed under the terms of the GNU General Public License.
 *
 * This file contains directory-related functions independent of which
 * scheme is being used to represent forks.
 *
 * Based on the minix file system code, (C) 1991, 1992 by Linus Torvalds
 */

#include "hfs_fs.h"
#include "btree.h"

/*
 * hfs_lookup()
 */
static struct dentry *hfs_lookup(struct inode *dir, struct dentry *dentry,
<<<<<<< HEAD
				 struct nameidata *nd)
=======
				 unsigned int flags)
>>>>>>> refs/remotes/origin/master
{
	hfs_cat_rec rec;
	struct hfs_find_data fd;
	struct inode *inode = NULL;
	int res;

<<<<<<< HEAD
	hfs_find_init(HFS_SB(dir->i_sb)->cat_tree, &fd);
=======
	res = hfs_find_init(HFS_SB(dir->i_sb)->cat_tree, &fd);
	if (res)
		return ERR_PTR(res);
>>>>>>> refs/remotes/origin/master
	hfs_cat_build_key(dir->i_sb, fd.search_key, dir->i_ino, &dentry->d_name);
	res = hfs_brec_read(&fd, &rec, sizeof(rec));
	if (res) {
		hfs_find_exit(&fd);
		if (res == -ENOENT) {
			/* No such entry */
			inode = NULL;
			goto done;
		}
		return ERR_PTR(res);
	}
	inode = hfs_iget(dir->i_sb, &fd.search_key->cat, &rec);
	hfs_find_exit(&fd);
	if (!inode)
		return ERR_PTR(-EACCES);
done:
	d_add(dentry, inode);
	return NULL;
}

/*
 * hfs_readdir
 */
<<<<<<< HEAD
static int hfs_readdir(struct file *filp, void *dirent, filldir_t filldir)
{
	struct inode *inode = filp->f_path.dentry->d_inode;
=======
static int hfs_readdir(struct file *file, struct dir_context *ctx)
{
	struct inode *inode = file_inode(file);
>>>>>>> refs/remotes/origin/master
	struct super_block *sb = inode->i_sb;
	int len, err;
	char strbuf[HFS_MAX_NAMELEN];
	union hfs_cat_rec entry;
	struct hfs_find_data fd;
	struct hfs_readdir_data *rd;
	u16 type;

<<<<<<< HEAD
	if (filp->f_pos >= inode->i_size)
		return 0;

	hfs_find_init(HFS_SB(sb)->cat_tree, &fd);
=======
	if (ctx->pos >= inode->i_size)
		return 0;

	err = hfs_find_init(HFS_SB(sb)->cat_tree, &fd);
	if (err)
		return err;
>>>>>>> refs/remotes/origin/master
	hfs_cat_build_key(sb, fd.search_key, inode->i_ino, NULL);
	err = hfs_brec_find(&fd);
	if (err)
		goto out;

<<<<<<< HEAD
	switch ((u32)filp->f_pos) {
	case 0:
		/* This is completely artificial... */
		if (filldir(dirent, ".", 1, 0, inode->i_ino, DT_DIR))
			goto out;
		filp->f_pos++;
		/* fall through */
	case 1:
=======
	if (ctx->pos == 0) {
		/* This is completely artificial... */
		if (!dir_emit_dot(file, ctx))
			goto out;
		ctx->pos = 1;
	}
	if (ctx->pos == 1) {
>>>>>>> refs/remotes/origin/master
		if (fd.entrylength > sizeof(entry) || fd.entrylength < 0) {
			err = -EIO;
			goto out;
		}

		hfs_bnode_read(fd.bnode, &entry, fd.entryoffset, fd.entrylength);
		if (entry.type != HFS_CDR_THD) {
<<<<<<< HEAD
			printk(KERN_ERR "hfs: bad catalog folder thread\n");
=======
			pr_err("bad catalog folder thread\n");
>>>>>>> refs/remotes/origin/master
			err = -EIO;
			goto out;
		}
		//if (fd.entrylength < HFS_MIN_THREAD_SZ) {
<<<<<<< HEAD
		//	printk(KERN_ERR "hfs: truncated catalog thread\n");
		//	err = -EIO;
		//	goto out;
		//}
		if (filldir(dirent, "..", 2, 1,
			    be32_to_cpu(entry.thread.ParID), DT_DIR))
			goto out;
		filp->f_pos++;
		/* fall through */
	default:
		if (filp->f_pos >= inode->i_size)
			goto out;
		err = hfs_brec_goto(&fd, filp->f_pos - 1);
		if (err)
			goto out;
	}

	for (;;) {
		if (be32_to_cpu(fd.key->cat.ParID) != inode->i_ino) {
			printk(KERN_ERR "hfs: walked past end of dir\n");
=======
		//	pr_err("truncated catalog thread\n");
		//	err = -EIO;
		//	goto out;
		//}
		if (!dir_emit(ctx, "..", 2,
			    be32_to_cpu(entry.thread.ParID), DT_DIR))
			goto out;
		ctx->pos = 2;
	}
	if (ctx->pos >= inode->i_size)
		goto out;
	err = hfs_brec_goto(&fd, ctx->pos - 1);
	if (err)
		goto out;

	for (;;) {
		if (be32_to_cpu(fd.key->cat.ParID) != inode->i_ino) {
			pr_err("walked past end of dir\n");
>>>>>>> refs/remotes/origin/master
			err = -EIO;
			goto out;
		}

		if (fd.entrylength > sizeof(entry) || fd.entrylength < 0) {
			err = -EIO;
			goto out;
		}

		hfs_bnode_read(fd.bnode, &entry, fd.entryoffset, fd.entrylength);
		type = entry.type;
		len = hfs_mac2asc(sb, strbuf, &fd.key->cat.CName);
		if (type == HFS_CDR_DIR) {
			if (fd.entrylength < sizeof(struct hfs_cat_dir)) {
<<<<<<< HEAD
				printk(KERN_ERR "hfs: small dir entry\n");
				err = -EIO;
				goto out;
			}
			if (filldir(dirent, strbuf, len, filp->f_pos,
=======
				pr_err("small dir entry\n");
				err = -EIO;
				goto out;
			}
			if (!dir_emit(ctx, strbuf, len,
>>>>>>> refs/remotes/origin/master
				    be32_to_cpu(entry.dir.DirID), DT_DIR))
				break;
		} else if (type == HFS_CDR_FIL) {
			if (fd.entrylength < sizeof(struct hfs_cat_file)) {
<<<<<<< HEAD
				printk(KERN_ERR "hfs: small file entry\n");
				err = -EIO;
				goto out;
			}
			if (filldir(dirent, strbuf, len, filp->f_pos,
				    be32_to_cpu(entry.file.FlNum), DT_REG))
				break;
		} else {
			printk(KERN_ERR "hfs: bad catalog entry type %d\n", type);
			err = -EIO;
			goto out;
		}
		filp->f_pos++;
		if (filp->f_pos >= inode->i_size)
=======
				pr_err("small file entry\n");
				err = -EIO;
				goto out;
			}
			if (!dir_emit(ctx, strbuf, len,
				    be32_to_cpu(entry.file.FlNum), DT_REG))
				break;
		} else {
			pr_err("bad catalog entry type %d\n", type);
			err = -EIO;
			goto out;
		}
		ctx->pos++;
		if (ctx->pos >= inode->i_size)
>>>>>>> refs/remotes/origin/master
			goto out;
		err = hfs_brec_goto(&fd, 1);
		if (err)
			goto out;
	}
<<<<<<< HEAD
	rd = filp->private_data;
=======
	rd = file->private_data;
>>>>>>> refs/remotes/origin/master
	if (!rd) {
		rd = kmalloc(sizeof(struct hfs_readdir_data), GFP_KERNEL);
		if (!rd) {
			err = -ENOMEM;
			goto out;
		}
<<<<<<< HEAD
		filp->private_data = rd;
		rd->file = filp;
=======
		file->private_data = rd;
		rd->file = file;
>>>>>>> refs/remotes/origin/master
		list_add(&rd->list, &HFS_I(inode)->open_dir_list);
	}
	memcpy(&rd->key, &fd.key, sizeof(struct hfs_cat_key));
out:
	hfs_find_exit(&fd);
	return err;
}

static int hfs_dir_release(struct inode *inode, struct file *file)
{
	struct hfs_readdir_data *rd = file->private_data;
	if (rd) {
<<<<<<< HEAD
		list_del(&rd->list);
=======
		mutex_lock(&inode->i_mutex);
		list_del(&rd->list);
		mutex_unlock(&inode->i_mutex);
>>>>>>> refs/remotes/origin/master
		kfree(rd);
	}
	return 0;
}

/*
 * hfs_create()
 *
 * This is the create() entry in the inode_operations structure for
 * regular HFS directories.  The purpose is to create a new file in
 * a directory and return a corresponding inode, given the inode for
 * the directory and the name (and its length) of the new file.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int hfs_create(struct inode *dir, struct dentry *dentry, int mode,
=======
static int hfs_create(struct inode *dir, struct dentry *dentry, umode_t mode,
>>>>>>> refs/remotes/origin/cm-10.0
		      struct nameidata *nd)
=======
static int hfs_create(struct inode *dir, struct dentry *dentry, umode_t mode,
		      bool excl)
>>>>>>> refs/remotes/origin/master
{
	struct inode *inode;
	int res;

	inode = hfs_new_inode(dir, &dentry->d_name, mode);
	if (!inode)
		return -ENOSPC;

	res = hfs_cat_create(inode->i_ino, dir, &dentry->d_name, inode);
	if (res) {
<<<<<<< HEAD
<<<<<<< HEAD
		inode->i_nlink = 0;
=======
		clear_nlink(inode);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		clear_nlink(inode);
>>>>>>> refs/remotes/origin/master
		hfs_delete_inode(inode);
		iput(inode);
		return res;
	}
	d_instantiate(dentry, inode);
	mark_inode_dirty(inode);
	return 0;
}

/*
 * hfs_mkdir()
 *
 * This is the mkdir() entry in the inode_operations structure for
 * regular HFS directories.  The purpose is to create a new directory
 * in a directory, given the inode for the parent directory and the
 * name (and its length) of the new directory.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int hfs_mkdir(struct inode *dir, struct dentry *dentry, int mode)
=======
static int hfs_mkdir(struct inode *dir, struct dentry *dentry, umode_t mode)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int hfs_mkdir(struct inode *dir, struct dentry *dentry, umode_t mode)
>>>>>>> refs/remotes/origin/master
{
	struct inode *inode;
	int res;

	inode = hfs_new_inode(dir, &dentry->d_name, S_IFDIR | mode);
	if (!inode)
		return -ENOSPC;

	res = hfs_cat_create(inode->i_ino, dir, &dentry->d_name, inode);
	if (res) {
<<<<<<< HEAD
<<<<<<< HEAD
		inode->i_nlink = 0;
=======
		clear_nlink(inode);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		clear_nlink(inode);
>>>>>>> refs/remotes/origin/master
		hfs_delete_inode(inode);
		iput(inode);
		return res;
	}
	d_instantiate(dentry, inode);
	mark_inode_dirty(inode);
	return 0;
}

/*
 * hfs_remove()
 *
 * This serves as both unlink() and rmdir() in the inode_operations
 * structure for regular HFS directories.  The purpose is to delete
 * an existing child, given the inode for the parent directory and
 * the name (and its length) of the existing directory.
 *
 * HFS does not have hardlinks, so both rmdir and unlink set the
 * link count to 0.  The only difference is the emptiness check.
 */
static int hfs_remove(struct inode *dir, struct dentry *dentry)
{
	struct inode *inode = dentry->d_inode;
	int res;

	if (S_ISDIR(inode->i_mode) && inode->i_size != 2)
		return -ENOTEMPTY;
	res = hfs_cat_delete(inode->i_ino, dir, &dentry->d_name);
	if (res)
		return res;
	clear_nlink(inode);
	inode->i_ctime = CURRENT_TIME_SEC;
	hfs_delete_inode(inode);
	mark_inode_dirty(inode);
	return 0;
}

/*
 * hfs_rename()
 *
 * This is the rename() entry in the inode_operations structure for
 * regular HFS directories.  The purpose is to rename an existing
 * file or directory, given the inode for the current directory and
 * the name (and its length) of the existing file/directory and the
 * inode for the new directory and the name (and its length) of the
 * new file/directory.
 * XXX: how do you handle must_be dir?
 */
static int hfs_rename(struct inode *old_dir, struct dentry *old_dentry,
		      struct inode *new_dir, struct dentry *new_dentry)
{
	int res;

	/* Unlink destination if it already exists */
	if (new_dentry->d_inode) {
		res = hfs_remove(new_dir, new_dentry);
		if (res)
			return res;
	}

	res = hfs_cat_move(old_dentry->d_inode->i_ino,
			   old_dir, &old_dentry->d_name,
			   new_dir, &new_dentry->d_name);
	if (!res)
		hfs_cat_build_key(old_dir->i_sb,
				  (btree_key *)&HFS_I(old_dentry->d_inode)->cat_key,
				  new_dir->i_ino, &new_dentry->d_name);
	return res;
}

const struct file_operations hfs_dir_operations = {
	.read		= generic_read_dir,
<<<<<<< HEAD
	.readdir	= hfs_readdir,
=======
	.iterate	= hfs_readdir,
>>>>>>> refs/remotes/origin/master
	.llseek		= generic_file_llseek,
	.release	= hfs_dir_release,
};

const struct inode_operations hfs_dir_inode_operations = {
	.create		= hfs_create,
	.lookup		= hfs_lookup,
	.unlink		= hfs_remove,
	.mkdir		= hfs_mkdir,
	.rmdir		= hfs_remove,
	.rename		= hfs_rename,
	.setattr	= hfs_inode_setattr,
};
