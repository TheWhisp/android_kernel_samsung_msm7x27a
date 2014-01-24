/*
 *  linux/fs/affs/dir.c
 *
 *  (c) 1996  Hans-Joachim Widmaier - Rewritten
 *
 *  (C) 1993  Ray Burr - Modified for Amiga FFS filesystem.
 *
 *  (C) 1992  Eric Youngdale Modified for ISO 9660 filesystem.
 *
 *  (C) 1991  Linus Torvalds - minix filesystem
 *
 *  affs directory handling functions
 *
 */

#include "affs.h"

<<<<<<< HEAD
static int affs_readdir(struct file *, void *, filldir_t);
=======
static int affs_readdir(struct file *, struct dir_context *);
>>>>>>> refs/remotes/origin/master

const struct file_operations affs_dir_operations = {
	.read		= generic_read_dir,
	.llseek		= generic_file_llseek,
<<<<<<< HEAD
	.readdir	= affs_readdir,
=======
	.iterate	= affs_readdir,
>>>>>>> refs/remotes/origin/master
	.fsync		= affs_file_fsync,
};

/*
 * directories can handle most operations...
 */
const struct inode_operations affs_dir_inode_operations = {
	.create		= affs_create,
	.lookup		= affs_lookup,
	.link		= affs_link,
	.unlink		= affs_unlink,
	.symlink	= affs_symlink,
	.mkdir		= affs_mkdir,
	.rmdir		= affs_rmdir,
	.rename		= affs_rename,
	.setattr	= affs_notify_change,
};

static int
<<<<<<< HEAD
affs_readdir(struct file *filp, void *dirent, filldir_t filldir)
{
	struct inode		*inode = filp->f_path.dentry->d_inode;
	struct super_block	*sb = inode->i_sb;
	struct buffer_head	*dir_bh;
	struct buffer_head	*fh_bh;
=======
affs_readdir(struct file *file, struct dir_context *ctx)
{
	struct inode		*inode = file_inode(file);
	struct super_block	*sb = inode->i_sb;
	struct buffer_head	*dir_bh = NULL;
	struct buffer_head	*fh_bh = NULL;
>>>>>>> refs/remotes/origin/master
	unsigned char		*name;
	int			 namelen;
	u32			 i;
	int			 hash_pos;
	int			 chain_pos;
<<<<<<< HEAD
	u32			 f_pos;
	u32			 ino;
	int			 stored;
	int			 res;

	pr_debug("AFFS: readdir(ino=%lu,f_pos=%lx)\n",inode->i_ino,(unsigned long)filp->f_pos);

	stored = 0;
	res    = -EIO;
	dir_bh = NULL;
	fh_bh  = NULL;
	f_pos  = filp->f_pos;

	if (f_pos == 0) {
		filp->private_data = (void *)0;
		if (filldir(dirent, ".", 1, f_pos, inode->i_ino, DT_DIR) < 0)
			return 0;
		filp->f_pos = f_pos = 1;
		stored++;
	}
	if (f_pos == 1) {
		if (filldir(dirent, "..", 2, f_pos, parent_ino(filp->f_path.dentry), DT_DIR) < 0)
			return stored;
		filp->f_pos = f_pos = 2;
		stored++;
	}

	affs_lock_dir(inode);
	chain_pos = (f_pos - 2) & 0xffff;
	hash_pos  = (f_pos - 2) >> 16;
=======
	u32			 ino;

	pr_debug("AFFS: readdir(ino=%lu,f_pos=%lx)\n",inode->i_ino,(unsigned long)ctx->pos);

	if (ctx->pos < 2) {
		file->private_data = (void *)0;
		if (!dir_emit_dots(file, ctx))
			return 0;
	}

	affs_lock_dir(inode);
	chain_pos = (ctx->pos - 2) & 0xffff;
	hash_pos  = (ctx->pos - 2) >> 16;
>>>>>>> refs/remotes/origin/master
	if (chain_pos == 0xffff) {
		affs_warning(sb, "readdir", "More than 65535 entries in chain");
		chain_pos = 0;
		hash_pos++;
<<<<<<< HEAD
		filp->f_pos = ((hash_pos << 16) | chain_pos) + 2;
=======
		ctx->pos = ((hash_pos << 16) | chain_pos) + 2;
>>>>>>> refs/remotes/origin/master
	}
	dir_bh = affs_bread(sb, inode->i_ino);
	if (!dir_bh)
		goto readdir_out;

	/* If the directory hasn't changed since the last call to readdir(),
	 * we can jump directly to where we left off.
	 */
<<<<<<< HEAD
	ino = (u32)(long)filp->private_data;
	if (ino && filp->f_version == inode->i_version) {
=======
	ino = (u32)(long)file->private_data;
	if (ino && file->f_version == inode->i_version) {
>>>>>>> refs/remotes/origin/master
		pr_debug("AFFS: readdir() left off=%d\n", ino);
		goto inside;
	}

	ino = be32_to_cpu(AFFS_HEAD(dir_bh)->table[hash_pos]);
	for (i = 0; ino && i < chain_pos; i++) {
		fh_bh = affs_bread(sb, ino);
		if (!fh_bh) {
			affs_error(sb, "readdir","Cannot read block %d", i);
<<<<<<< HEAD
			goto readdir_out;
=======
			return -EIO;
>>>>>>> refs/remotes/origin/master
		}
		ino = be32_to_cpu(AFFS_TAIL(sb, fh_bh)->hash_chain);
		affs_brelse(fh_bh);
		fh_bh = NULL;
	}
	if (ino)
		goto inside;
	hash_pos++;

	for (; hash_pos < AFFS_SB(sb)->s_hashsize; hash_pos++) {
		ino = be32_to_cpu(AFFS_HEAD(dir_bh)->table[hash_pos]);
		if (!ino)
			continue;
<<<<<<< HEAD
		f_pos = (hash_pos << 16) + 2;
=======
		ctx->pos = (hash_pos << 16) + 2;
>>>>>>> refs/remotes/origin/master
inside:
		do {
			fh_bh = affs_bread(sb, ino);
			if (!fh_bh) {
				affs_error(sb, "readdir","Cannot read block %d", ino);
<<<<<<< HEAD
				goto readdir_done;
=======
				break;
>>>>>>> refs/remotes/origin/master
			}

			namelen = min(AFFS_TAIL(sb, fh_bh)->name[0], (u8)30);
			name = AFFS_TAIL(sb, fh_bh)->name + 1;
			pr_debug("AFFS: readdir(): filldir(\"%.*s\", ino=%u), hash=%d, f_pos=%x\n",
<<<<<<< HEAD
				 namelen, name, ino, hash_pos, f_pos);
			if (filldir(dirent, name, namelen, f_pos, ino, DT_UNKNOWN) < 0)
				goto readdir_done;
			stored++;
			f_pos++;
=======
				 namelen, name, ino, hash_pos, (u32)ctx->pos);
			if (!dir_emit(ctx, name, namelen, ino, DT_UNKNOWN))
				goto readdir_done;
			ctx->pos++;
>>>>>>> refs/remotes/origin/master
			ino = be32_to_cpu(AFFS_TAIL(sb, fh_bh)->hash_chain);
			affs_brelse(fh_bh);
			fh_bh = NULL;
		} while (ino);
	}
readdir_done:
<<<<<<< HEAD
	filp->f_pos = f_pos;
	filp->f_version = inode->i_version;
	filp->private_data = (void *)(long)ino;
	res = stored;
=======
	file->f_version = inode->i_version;
	file->private_data = (void *)(long)ino;
>>>>>>> refs/remotes/origin/master

readdir_out:
	affs_brelse(dir_bh);
	affs_brelse(fh_bh);
	affs_unlock_dir(inode);
<<<<<<< HEAD
	pr_debug("AFFS: readdir()=%d\n", stored);
	return res;
=======
	return 0;
>>>>>>> refs/remotes/origin/master
}
