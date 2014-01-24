/*
 * dir.c
 *
 * Copyright (c) 1999 Al Smith
 */

#include <linux/buffer_head.h>
#include "efs.h"

<<<<<<< HEAD
static int efs_readdir(struct file *, void *, filldir_t);
=======
static int efs_readdir(struct file *, struct dir_context *);
>>>>>>> refs/remotes/origin/master

const struct file_operations efs_dir_operations = {
	.llseek		= generic_file_llseek,
	.read		= generic_read_dir,
<<<<<<< HEAD
	.readdir	= efs_readdir,
=======
	.iterate	= efs_readdir,
>>>>>>> refs/remotes/origin/master
};

const struct inode_operations efs_dir_inode_operations = {
	.lookup		= efs_lookup,
};

<<<<<<< HEAD
static int efs_readdir(struct file *filp, void *dirent, filldir_t filldir) {
	struct inode *inode = filp->f_path.dentry->d_inode;
	struct buffer_head *bh;

	struct efs_dir		*dirblock;
	struct efs_dentry	*dirslot;
	efs_ino_t		inodenum;
	efs_block_t		block;
	int			slot, namelen;
	char			*nameptr;
=======
static int efs_readdir(struct file *file, struct dir_context *ctx)
{
	struct inode *inode = file_inode(file);
	efs_block_t		block;
	int			slot;
>>>>>>> refs/remotes/origin/master

	if (inode->i_size & (EFS_DIRBSIZE-1))
		printk(KERN_WARNING "EFS: WARNING: readdir(): directory size not a multiple of EFS_DIRBSIZE\n");

	/* work out where this entry can be found */
<<<<<<< HEAD
	block = filp->f_pos >> EFS_DIRBSIZE_BITS;

	/* each block contains at most 256 slots */
	slot  = filp->f_pos & 0xff;

	/* look at all blocks */
	while (block < inode->i_blocks) {
=======
	block = ctx->pos >> EFS_DIRBSIZE_BITS;

	/* each block contains at most 256 slots */
	slot  = ctx->pos & 0xff;

	/* look at all blocks */
	while (block < inode->i_blocks) {
		struct efs_dir		*dirblock;
		struct buffer_head *bh;

>>>>>>> refs/remotes/origin/master
		/* read the dir block */
		bh = sb_bread(inode->i_sb, efs_bmap(inode, block));

		if (!bh) {
			printk(KERN_ERR "EFS: readdir(): failed to read dir block %d\n", block);
			break;
		}

		dirblock = (struct efs_dir *) bh->b_data; 

		if (be16_to_cpu(dirblock->magic) != EFS_DIRBLK_MAGIC) {
			printk(KERN_ERR "EFS: readdir(): invalid directory block\n");
			brelse(bh);
			break;
		}

<<<<<<< HEAD
		while (slot < dirblock->slots) {
			if (dirblock->space[slot] == 0) {
				slot++;
				continue;
			}
=======
		for (; slot < dirblock->slots; slot++) {
			struct efs_dentry *dirslot;
			efs_ino_t inodenum;
			const char *nameptr;
			int namelen;

			if (dirblock->space[slot] == 0)
				continue;
>>>>>>> refs/remotes/origin/master

			dirslot  = (struct efs_dentry *) (((char *) bh->b_data) + EFS_SLOTAT(dirblock, slot));

			inodenum = be32_to_cpu(dirslot->inode);
			namelen  = dirslot->namelen;
			nameptr  = dirslot->name;

#ifdef DEBUG
			printk(KERN_DEBUG "EFS: readdir(): block %d slot %d/%d: inode %u, name \"%s\", namelen %u\n", block, slot, dirblock->slots-1, inodenum, nameptr, namelen);
#endif
<<<<<<< HEAD
			if (namelen > 0) {
				/* found the next entry */
				filp->f_pos = (block << EFS_DIRBSIZE_BITS) | slot;

				/* copy filename and data in dirslot */
				filldir(dirent, nameptr, namelen, filp->f_pos, inodenum, DT_UNKNOWN);

				/* sanity check */
				if (nameptr - (char *) dirblock + namelen > EFS_DIRBSIZE) {
					printk(KERN_WARNING "EFS: directory entry %d exceeds directory block\n", slot);
					slot++;
					continue;
				}

				/* store position of next slot */
				if (++slot == dirblock->slots) {
					slot = 0;
					block++;
				}
				brelse(bh);
				filp->f_pos = (block << EFS_DIRBSIZE_BITS) | slot;
				goto out;
			}
			slot++;
=======
			if (!namelen)
				continue;
			/* found the next entry */
			ctx->pos = (block << EFS_DIRBSIZE_BITS) | slot;

			/* sanity check */
			if (nameptr - (char *) dirblock + namelen > EFS_DIRBSIZE) {
				printk(KERN_WARNING "EFS: directory entry %d exceeds directory block\n", slot);
				continue;
			}

			/* copy filename and data in dirslot */
			if (!dir_emit(ctx, nameptr, namelen, inodenum, DT_UNKNOWN)) {
				brelse(bh);
				return 0;
			}
>>>>>>> refs/remotes/origin/master
		}
		brelse(bh);

		slot = 0;
		block++;
	}
<<<<<<< HEAD

	filp->f_pos = (block << EFS_DIRBSIZE_BITS) | slot;
out:
=======
	ctx->pos = (block << EFS_DIRBSIZE_BITS) | slot;
>>>>>>> refs/remotes/origin/master
	return 0;
}

