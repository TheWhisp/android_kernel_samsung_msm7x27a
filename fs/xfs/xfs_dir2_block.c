/*
 * Copyright (c) 2000-2003,2005 Silicon Graphics, Inc.
<<<<<<< HEAD
=======
 * Copyright (c) 2013 Red Hat, Inc.
>>>>>>> refs/remotes/origin/master
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write the Free Software Foundation,
 * Inc.,  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include "xfs.h"
#include "xfs_fs.h"
<<<<<<< HEAD
#include "xfs_types.h"
#include "xfs_log.h"
#include "xfs_inum.h"
#include "xfs_trans.h"
#include "xfs_sb.h"
#include "xfs_ag.h"
<<<<<<< HEAD
#include "xfs_dir2.h"
#include "xfs_mount.h"
#include "xfs_da_btree.h"
#include "xfs_bmap_btree.h"
#include "xfs_dir2_sf.h"
#include "xfs_dinode.h"
#include "xfs_inode.h"
#include "xfs_inode_item.h"
#include "xfs_dir2_data.h"
#include "xfs_dir2_leaf.h"
#include "xfs_dir2_block.h"
=======
#include "xfs_mount.h"
#include "xfs_da_btree.h"
#include "xfs_bmap_btree.h"
#include "xfs_dinode.h"
#include "xfs_inode.h"
#include "xfs_inode_item.h"
#include "xfs_dir2.h"
#include "xfs_dir2_format.h"
#include "xfs_dir2_priv.h"
>>>>>>> refs/remotes/origin/cm-10.0
#include "xfs_error.h"
#include "xfs_trace.h"
=======
#include "xfs_format.h"
#include "xfs_log_format.h"
#include "xfs_trans_resv.h"
#include "xfs_sb.h"
#include "xfs_ag.h"
#include "xfs_mount.h"
#include "xfs_da_format.h"
#include "xfs_da_btree.h"
#include "xfs_inode.h"
#include "xfs_trans.h"
#include "xfs_inode_item.h"
#include "xfs_bmap.h"
#include "xfs_buf_item.h"
#include "xfs_dir2.h"
#include "xfs_dir2_priv.h"
#include "xfs_error.h"
#include "xfs_trace.h"
#include "xfs_cksum.h"
#include "xfs_dinode.h"
>>>>>>> refs/remotes/origin/master

/*
 * Local function prototypes.
 */
<<<<<<< HEAD
static void xfs_dir2_block_log_leaf(xfs_trans_t *tp, xfs_dabuf_t *bp, int first,
				    int last);
static void xfs_dir2_block_log_tail(xfs_trans_t *tp, xfs_dabuf_t *bp);
static int xfs_dir2_block_lookup_int(xfs_da_args_t *args, xfs_dabuf_t **bpp,
=======
static void xfs_dir2_block_log_leaf(xfs_trans_t *tp, struct xfs_buf *bp,
				    int first, int last);
static void xfs_dir2_block_log_tail(xfs_trans_t *tp, struct xfs_buf *bp);
static int xfs_dir2_block_lookup_int(xfs_da_args_t *args, struct xfs_buf **bpp,
>>>>>>> refs/remotes/origin/master
				     int *entno);
static int xfs_dir2_block_sort(const void *a, const void *b);

static xfs_dahash_t xfs_dir_hash_dot, xfs_dir_hash_dotdot;

/*
 * One-time startup routine called from xfs_init().
 */
void
xfs_dir_startup(void)
{
	xfs_dir_hash_dot = xfs_da_hashname((unsigned char *)".", 1);
	xfs_dir_hash_dotdot = xfs_da_hashname((unsigned char *)"..", 2);
}

<<<<<<< HEAD
=======
static bool
xfs_dir3_block_verify(
	struct xfs_buf		*bp)
{
	struct xfs_mount	*mp = bp->b_target->bt_mount;
	struct xfs_dir3_blk_hdr	*hdr3 = bp->b_addr;

	if (xfs_sb_version_hascrc(&mp->m_sb)) {
		if (hdr3->magic != cpu_to_be32(XFS_DIR3_BLOCK_MAGIC))
			return false;
		if (!uuid_equal(&hdr3->uuid, &mp->m_sb.sb_uuid))
			return false;
		if (be64_to_cpu(hdr3->blkno) != bp->b_bn)
			return false;
	} else {
		if (hdr3->magic != cpu_to_be32(XFS_DIR2_BLOCK_MAGIC))
			return false;
	}
	if (__xfs_dir3_data_check(NULL, bp))
		return false;
	return true;
}

static void
xfs_dir3_block_read_verify(
	struct xfs_buf	*bp)
{
	struct xfs_mount	*mp = bp->b_target->bt_mount;

	if ((xfs_sb_version_hascrc(&mp->m_sb) &&
	     !xfs_verify_cksum(bp->b_addr, BBTOB(bp->b_length),
					  XFS_DIR3_DATA_CRC_OFF)) ||
	    !xfs_dir3_block_verify(bp)) {
		XFS_CORRUPTION_ERROR(__func__, XFS_ERRLEVEL_LOW, mp, bp->b_addr);
		xfs_buf_ioerror(bp, EFSCORRUPTED);
	}
}

static void
xfs_dir3_block_write_verify(
	struct xfs_buf	*bp)
{
	struct xfs_mount	*mp = bp->b_target->bt_mount;
	struct xfs_buf_log_item	*bip = bp->b_fspriv;
	struct xfs_dir3_blk_hdr	*hdr3 = bp->b_addr;

	if (!xfs_dir3_block_verify(bp)) {
		XFS_CORRUPTION_ERROR(__func__, XFS_ERRLEVEL_LOW, mp, bp->b_addr);
		xfs_buf_ioerror(bp, EFSCORRUPTED);
		return;
	}

	if (!xfs_sb_version_hascrc(&mp->m_sb))
		return;

	if (bip)
		hdr3->lsn = cpu_to_be64(bip->bli_item.li_lsn);

	xfs_update_cksum(bp->b_addr, BBTOB(bp->b_length), XFS_DIR3_DATA_CRC_OFF);
}

const struct xfs_buf_ops xfs_dir3_block_buf_ops = {
	.verify_read = xfs_dir3_block_read_verify,
	.verify_write = xfs_dir3_block_write_verify,
};

int
xfs_dir3_block_read(
	struct xfs_trans	*tp,
	struct xfs_inode	*dp,
	struct xfs_buf		**bpp)
{
	struct xfs_mount	*mp = dp->i_mount;
	int			err;

	err = xfs_da_read_buf(tp, dp, mp->m_dirdatablk, -1, bpp,
				XFS_DATA_FORK, &xfs_dir3_block_buf_ops);
	if (!err && tp)
		xfs_trans_buf_set_type(tp, *bpp, XFS_BLFT_DIR_BLOCK_BUF);
	return err;
}

static void
xfs_dir3_block_init(
	struct xfs_mount	*mp,
	struct xfs_trans	*tp,
	struct xfs_buf		*bp,
	struct xfs_inode	*dp)
{
	struct xfs_dir3_blk_hdr *hdr3 = bp->b_addr;

	bp->b_ops = &xfs_dir3_block_buf_ops;
	xfs_trans_buf_set_type(tp, bp, XFS_BLFT_DIR_BLOCK_BUF);

	if (xfs_sb_version_hascrc(&mp->m_sb)) {
		memset(hdr3, 0, sizeof(*hdr3));
		hdr3->magic = cpu_to_be32(XFS_DIR3_BLOCK_MAGIC);
		hdr3->blkno = cpu_to_be64(bp->b_bn);
		hdr3->owner = cpu_to_be64(dp->i_ino);
		uuid_copy(&hdr3->uuid, &mp->m_sb.sb_uuid);
		return;

	}
	hdr3->magic = cpu_to_be32(XFS_DIR2_BLOCK_MAGIC);
}

static void
xfs_dir2_block_need_space(
	struct xfs_inode		*dp,
	struct xfs_dir2_data_hdr	*hdr,
	struct xfs_dir2_block_tail	*btp,
	struct xfs_dir2_leaf_entry	*blp,
	__be16				**tagpp,
	struct xfs_dir2_data_unused	**dupp,
	struct xfs_dir2_data_unused	**enddupp,
	int				*compact,
	int				len)
{
	struct xfs_dir2_data_free	*bf;
	__be16				*tagp = NULL;
	struct xfs_dir2_data_unused	*dup = NULL;
	struct xfs_dir2_data_unused	*enddup = NULL;

	*compact = 0;
	bf = dp->d_ops->data_bestfree_p(hdr);

	/*
	 * If there are stale entries we'll use one for the leaf.
	 */
	if (btp->stale) {
		if (be16_to_cpu(bf[0].length) >= len) {
			/*
			 * The biggest entry enough to avoid compaction.
			 */
			dup = (xfs_dir2_data_unused_t *)
			      ((char *)hdr + be16_to_cpu(bf[0].offset));
			goto out;
		}

		/*
		 * Will need to compact to make this work.
		 * Tag just before the first leaf entry.
		 */
		*compact = 1;
		tagp = (__be16 *)blp - 1;

		/* Data object just before the first leaf entry.  */
		dup = (xfs_dir2_data_unused_t *)((char *)hdr + be16_to_cpu(*tagp));

		/*
		 * If it's not free then the data will go where the
		 * leaf data starts now, if it works at all.
		 */
		if (be16_to_cpu(dup->freetag) == XFS_DIR2_DATA_FREE_TAG) {
			if (be16_to_cpu(dup->length) + (be32_to_cpu(btp->stale) - 1) *
			    (uint)sizeof(*blp) < len)
				dup = NULL;
		} else if ((be32_to_cpu(btp->stale) - 1) * (uint)sizeof(*blp) < len)
			dup = NULL;
		else
			dup = (xfs_dir2_data_unused_t *)blp;
		goto out;
	}

	/*
	 * no stale entries, so just use free space.
	 * Tag just before the first leaf entry.
	 */
	tagp = (__be16 *)blp - 1;

	/* Data object just before the first leaf entry.  */
	enddup = (xfs_dir2_data_unused_t *)((char *)hdr + be16_to_cpu(*tagp));

	/*
	 * If it's not free then can't do this add without cleaning up:
	 * the space before the first leaf entry needs to be free so it
	 * can be expanded to hold the pointer to the new entry.
	 */
	if (be16_to_cpu(enddup->freetag) == XFS_DIR2_DATA_FREE_TAG) {
		/*
		 * Check out the biggest freespace and see if it's the same one.
		 */
		dup = (xfs_dir2_data_unused_t *)
		      ((char *)hdr + be16_to_cpu(bf[0].offset));
		if (dup != enddup) {
			/*
			 * Not the same free entry, just check its length.
			 */
			if (be16_to_cpu(dup->length) < len)
				dup = NULL;
			goto out;
		}

		/*
		 * It is the biggest freespace, can it hold the leaf too?
		 */
		if (be16_to_cpu(dup->length) < len + (uint)sizeof(*blp)) {
			/*
			 * Yes, use the second-largest entry instead if it works.
			 */
			if (be16_to_cpu(bf[1].length) >= len)
				dup = (xfs_dir2_data_unused_t *)
				      ((char *)hdr + be16_to_cpu(bf[1].offset));
			else
				dup = NULL;
		}
	}
out:
	*tagpp = tagp;
	*dupp = dup;
	*enddupp = enddup;
}

/*
 * compact the leaf entries.
 * Leave the highest-numbered stale entry stale.
 * XXX should be the one closest to mid but mid is not yet computed.
 */
static void
xfs_dir2_block_compact(
	struct xfs_trans		*tp,
	struct xfs_inode		*dp,
	struct xfs_buf			*bp,
	struct xfs_dir2_data_hdr	*hdr,
	struct xfs_dir2_block_tail	*btp,
	struct xfs_dir2_leaf_entry	*blp,
	int				*needlog,
	int				*lfloghigh,
	int				*lfloglow)
{
	int			fromidx;	/* source leaf index */
	int			toidx;		/* target leaf index */
	int			needscan = 0;
	int			highstale;	/* high stale index */

	fromidx = toidx = be32_to_cpu(btp->count) - 1;
	highstale = *lfloghigh = -1;
	for (; fromidx >= 0; fromidx--) {
		if (blp[fromidx].address == cpu_to_be32(XFS_DIR2_NULL_DATAPTR)) {
			if (highstale == -1)
				highstale = toidx;
			else {
				if (*lfloghigh == -1)
					*lfloghigh = toidx;
				continue;
			}
		}
		if (fromidx < toidx)
			blp[toidx] = blp[fromidx];
		toidx--;
	}
	*lfloglow = toidx + 1 - (be32_to_cpu(btp->stale) - 1);
	*lfloghigh -= be32_to_cpu(btp->stale) - 1;
	be32_add_cpu(&btp->count, -(be32_to_cpu(btp->stale) - 1));
	xfs_dir2_data_make_free(tp, dp, bp,
		(xfs_dir2_data_aoff_t)((char *)blp - (char *)hdr),
		(xfs_dir2_data_aoff_t)((be32_to_cpu(btp->stale) - 1) * sizeof(*blp)),
		needlog, &needscan);
	blp += be32_to_cpu(btp->stale) - 1;
	btp->stale = cpu_to_be32(1);
	/*
	 * If we now need to rebuild the bestfree map, do so.
	 * This needs to happen before the next call to use_free.
	 */
	if (needscan)
		xfs_dir2_data_freescan(dp, hdr, needlog);
}

>>>>>>> refs/remotes/origin/master
/*
 * Add an entry to a block directory.
 */
int						/* error */
xfs_dir2_block_addname(
	xfs_da_args_t		*args)		/* directory op arguments */
{
<<<<<<< HEAD
	xfs_dir2_data_free_t	*bf;		/* bestfree table in block */
<<<<<<< HEAD
	xfs_dir2_block_t	*block;		/* directory block structure */
=======
	xfs_dir2_data_hdr_t	*hdr;		/* block header */
>>>>>>> refs/remotes/origin/cm-10.0
	xfs_dir2_leaf_entry_t	*blp;		/* block leaf entries */
	xfs_dabuf_t		*bp;		/* buffer for block */
=======
	xfs_dir2_data_hdr_t	*hdr;		/* block header */
	xfs_dir2_leaf_entry_t	*blp;		/* block leaf entries */
	struct xfs_buf		*bp;		/* buffer for block */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_block_tail_t	*btp;		/* block tail */
	int			compact;	/* need to compact leaf ents */
	xfs_dir2_data_entry_t	*dep;		/* block data entry */
	xfs_inode_t		*dp;		/* directory inode */
	xfs_dir2_data_unused_t	*dup;		/* block unused entry */
	int			error;		/* error return value */
	xfs_dir2_data_unused_t	*enddup=NULL;	/* unused at end of data */
	xfs_dahash_t		hash;		/* hash value of found entry */
	int			high;		/* high index for binary srch */
	int			highstale;	/* high stale index */
	int			lfloghigh=0;	/* last final leaf to log */
	int			lfloglow=0;	/* first final leaf to log */
	int			len;		/* length of the new entry */
	int			low;		/* low index for binary srch */
	int			lowstale;	/* low stale index */
	int			mid=0;		/* midpoint for binary srch */
	xfs_mount_t		*mp;		/* filesystem mount point */
	int			needlog;	/* need to log header */
	int			needscan;	/* need to rescan freespace */
	__be16			*tagp;		/* pointer to tag value */
	xfs_trans_t		*tp;		/* transaction structure */

	trace_xfs_dir2_block_addname(args);

	dp = args->dp;
	tp = args->trans;
	mp = dp->i_mount;
<<<<<<< HEAD
	/*
	 * Read the (one and only) directory block into dabuf bp.
	 */
	if ((error =
	    xfs_da_read_buf(tp, dp, mp->m_dirdatablk, -1, &bp, XFS_DATA_FORK))) {
		return error;
	}
	ASSERT(bp != NULL);
<<<<<<< HEAD
	block = bp->data;
	/*
	 * Check the magic number, corrupted if wrong.
	 */
	if (unlikely(be32_to_cpu(block->hdr.magic) != XFS_DIR2_BLOCK_MAGIC)) {
		XFS_CORRUPTION_ERROR("xfs_dir2_block_addname",
				     XFS_ERRLEVEL_LOW, mp, block);
=======
	hdr = bp->data;
	/*
	 * Check the magic number, corrupted if wrong.
	 */
	if (unlikely(hdr->magic != cpu_to_be32(XFS_DIR2_BLOCK_MAGIC))) {
		XFS_CORRUPTION_ERROR("xfs_dir2_block_addname",
				     XFS_ERRLEVEL_LOW, mp, hdr);
>>>>>>> refs/remotes/origin/cm-10.0
		xfs_da_brelse(tp, bp);
		return XFS_ERROR(EFSCORRUPTED);
	}
	len = xfs_dir2_data_entsize(args->namelen);
	/*
	 * Set up pointers to parts of the block.
	 */
<<<<<<< HEAD
	bf = block->hdr.bestfree;
	btp = xfs_dir2_block_tail_p(mp, block);
=======
	bf = hdr->bestfree;
	btp = xfs_dir2_block_tail_p(mp, hdr);
>>>>>>> refs/remotes/origin/cm-10.0
	blp = xfs_dir2_block_leaf_p(btp);
	/*
	 * No stale entries?  Need space for entry and new leaf.
	 */
	if (!btp->stale) {
		/*
		 * Tag just before the first leaf entry.
		 */
		tagp = (__be16 *)blp - 1;
		/*
		 * Data object just before the first leaf entry.
		 */
<<<<<<< HEAD
		enddup = (xfs_dir2_data_unused_t *)((char *)block + be16_to_cpu(*tagp));
=======
		enddup = (xfs_dir2_data_unused_t *)((char *)hdr + be16_to_cpu(*tagp));
>>>>>>> refs/remotes/origin/cm-10.0
		/*
		 * If it's not free then can't do this add without cleaning up:
		 * the space before the first leaf entry needs to be free so it
		 * can be expanded to hold the pointer to the new entry.
		 */
		if (be16_to_cpu(enddup->freetag) != XFS_DIR2_DATA_FREE_TAG)
			dup = enddup = NULL;
		/*
		 * Check out the biggest freespace and see if it's the same one.
		 */
		else {
			dup = (xfs_dir2_data_unused_t *)
<<<<<<< HEAD
			      ((char *)block + be16_to_cpu(bf[0].offset));
=======
			      ((char *)hdr + be16_to_cpu(bf[0].offset));
>>>>>>> refs/remotes/origin/cm-10.0
			if (dup == enddup) {
				/*
				 * It is the biggest freespace, is it too small
				 * to hold the new leaf too?
				 */
				if (be16_to_cpu(dup->length) < len + (uint)sizeof(*blp)) {
					/*
					 * Yes, we use the second-largest
					 * entry instead if it works.
					 */
					if (be16_to_cpu(bf[1].length) >= len)
						dup = (xfs_dir2_data_unused_t *)
<<<<<<< HEAD
						      ((char *)block +
=======
						      ((char *)hdr +
>>>>>>> refs/remotes/origin/cm-10.0
						       be16_to_cpu(bf[1].offset));
					else
						dup = NULL;
				}
			} else {
				/*
				 * Not the same free entry,
				 * just check its length.
				 */
				if (be16_to_cpu(dup->length) < len) {
					dup = NULL;
				}
			}
		}
		compact = 0;
	}
	/*
	 * If there are stale entries we'll use one for the leaf.
	 * Is the biggest entry enough to avoid compaction?
	 */
	else if (be16_to_cpu(bf[0].length) >= len) {
		dup = (xfs_dir2_data_unused_t *)
<<<<<<< HEAD
		      ((char *)block + be16_to_cpu(bf[0].offset));
=======
		      ((char *)hdr + be16_to_cpu(bf[0].offset));
>>>>>>> refs/remotes/origin/cm-10.0
		compact = 0;
	}
	/*
	 * Will need to compact to make this work.
	 */
	else {
		/*
		 * Tag just before the first leaf entry.
		 */
		tagp = (__be16 *)blp - 1;
		/*
		 * Data object just before the first leaf entry.
		 */
<<<<<<< HEAD
		dup = (xfs_dir2_data_unused_t *)((char *)block + be16_to_cpu(*tagp));
=======
		dup = (xfs_dir2_data_unused_t *)((char *)hdr + be16_to_cpu(*tagp));
>>>>>>> refs/remotes/origin/cm-10.0
		/*
		 * If it's not free then the data will go where the
		 * leaf data starts now, if it works at all.
		 */
		if (be16_to_cpu(dup->freetag) == XFS_DIR2_DATA_FREE_TAG) {
			if (be16_to_cpu(dup->length) + (be32_to_cpu(btp->stale) - 1) *
			    (uint)sizeof(*blp) < len)
				dup = NULL;
		} else if ((be32_to_cpu(btp->stale) - 1) * (uint)sizeof(*blp) < len)
			dup = NULL;
		else
			dup = (xfs_dir2_data_unused_t *)blp;
		compact = 1;
	}
	/*
	 * If this isn't a real add, we're done with the buffer.
	 */
	if (args->op_flags & XFS_DA_OP_JUSTCHECK)
		xfs_da_brelse(tp, bp);
=======

	/* Read the (one and only) directory block into bp. */
	error = xfs_dir3_block_read(tp, dp, &bp);
	if (error)
		return error;

	len = dp->d_ops->data_entsize(args->namelen);

	/*
	 * Set up pointers to parts of the block.
	 */
	hdr = bp->b_addr;
	btp = xfs_dir2_block_tail_p(mp, hdr);
	blp = xfs_dir2_block_leaf_p(btp);

	/*
	 * Find out if we can reuse stale entries or whether we need extra
	 * space for entry and new leaf.
	 */
	xfs_dir2_block_need_space(dp, hdr, btp, blp, &tagp, &dup,
				  &enddup, &compact, len);

	/*
	 * Done everything we need for a space check now.
	 */
	if (args->op_flags & XFS_DA_OP_JUSTCHECK) {
		xfs_trans_brelse(tp, bp);
		if (!dup)
			return XFS_ERROR(ENOSPC);
		return 0;
	}

>>>>>>> refs/remotes/origin/master
	/*
	 * If we don't have space for the new entry & leaf ...
	 */
	if (!dup) {
<<<<<<< HEAD
		/*
		 * Not trying to actually do anything, or don't have
		 * a space reservation: return no-space.
		 */
		if ((args->op_flags & XFS_DA_OP_JUSTCHECK) || args->total == 0)
=======
		/* Don't have a space reservation: return no-space.  */
		if (args->total == 0)
>>>>>>> refs/remotes/origin/master
			return XFS_ERROR(ENOSPC);
		/*
		 * Convert to the next larger format.
		 * Then add the new entry in that format.
		 */
		error = xfs_dir2_block_to_leaf(args, bp);
<<<<<<< HEAD
		xfs_da_buf_done(bp);
=======
>>>>>>> refs/remotes/origin/master
		if (error)
			return error;
		return xfs_dir2_leaf_addname(args);
	}
<<<<<<< HEAD
	/*
	 * Just checking, and it would work, so say so.
	 */
	if (args->op_flags & XFS_DA_OP_JUSTCHECK)
		return 0;
	needlog = needscan = 0;
	/*
	 * If need to compact the leaf entries, do it now.
	 * Leave the highest-numbered stale entry stale.
	 * XXX should be the one closest to mid but mid is not yet computed.
	 */
	if (compact) {
		int	fromidx;		/* source leaf index */
		int	toidx;			/* target leaf index */

		for (fromidx = toidx = be32_to_cpu(btp->count) - 1,
			highstale = lfloghigh = -1;
		     fromidx >= 0;
		     fromidx--) {
<<<<<<< HEAD
			if (be32_to_cpu(blp[fromidx].address) == XFS_DIR2_NULL_DATAPTR) {
=======
			if (blp[fromidx].address ==
			    cpu_to_be32(XFS_DIR2_NULL_DATAPTR)) {
>>>>>>> refs/remotes/origin/cm-10.0
				if (highstale == -1)
					highstale = toidx;
				else {
					if (lfloghigh == -1)
						lfloghigh = toidx;
					continue;
				}
			}
			if (fromidx < toidx)
				blp[toidx] = blp[fromidx];
			toidx--;
		}
		lfloglow = toidx + 1 - (be32_to_cpu(btp->stale) - 1);
		lfloghigh -= be32_to_cpu(btp->stale) - 1;
		be32_add_cpu(&btp->count, -(be32_to_cpu(btp->stale) - 1));
		xfs_dir2_data_make_free(tp, bp,
<<<<<<< HEAD
			(xfs_dir2_data_aoff_t)((char *)blp - (char *)block),
=======
			(xfs_dir2_data_aoff_t)((char *)blp - (char *)hdr),
>>>>>>> refs/remotes/origin/cm-10.0
			(xfs_dir2_data_aoff_t)((be32_to_cpu(btp->stale) - 1) * sizeof(*blp)),
			&needlog, &needscan);
		blp += be32_to_cpu(btp->stale) - 1;
		btp->stale = cpu_to_be32(1);
		/*
		 * If we now need to rebuild the bestfree map, do so.
		 * This needs to happen before the next call to use_free.
		 */
		if (needscan) {
<<<<<<< HEAD
			xfs_dir2_data_freescan(mp, (xfs_dir2_data_t *)block, &needlog);
=======
			xfs_dir2_data_freescan(mp, hdr, &needlog);
>>>>>>> refs/remotes/origin/cm-10.0
			needscan = 0;
		}
	}
	/*
	 * Set leaf logging boundaries to impossible state.
	 * For the no-stale case they're set explicitly.
	 */
	else if (btp->stale) {
		lfloglow = be32_to_cpu(btp->count);
		lfloghigh = -1;
	}
=======

	needlog = needscan = 0;

	/*
	 * If need to compact the leaf entries, do it now.
	 */
	if (compact) {
		xfs_dir2_block_compact(tp, dp, bp, hdr, btp, blp, &needlog,
				      &lfloghigh, &lfloglow);
		/* recalculate blp post-compaction */
		blp = xfs_dir2_block_leaf_p(btp);
	} else if (btp->stale) {
		/*
		 * Set leaf logging boundaries to impossible state.
		 * For the no-stale case they're set explicitly.
		 */
		lfloglow = be32_to_cpu(btp->count);
		lfloghigh = -1;
	}

>>>>>>> refs/remotes/origin/master
	/*
	 * Find the slot that's first lower than our hash value, -1 if none.
	 */
	for (low = 0, high = be32_to_cpu(btp->count) - 1; low <= high; ) {
		mid = (low + high) >> 1;
		if ((hash = be32_to_cpu(blp[mid].hashval)) == args->hashval)
			break;
		if (hash < args->hashval)
			low = mid + 1;
		else
			high = mid - 1;
	}
	while (mid >= 0 && be32_to_cpu(blp[mid].hashval) >= args->hashval) {
		mid--;
	}
	/*
	 * No stale entries, will use enddup space to hold new leaf.
	 */
	if (!btp->stale) {
		/*
		 * Mark the space needed for the new leaf entry, now in use.
		 */
<<<<<<< HEAD
		xfs_dir2_data_use_free(tp, bp, enddup,
			(xfs_dir2_data_aoff_t)
<<<<<<< HEAD
			((char *)enddup - (char *)block + be16_to_cpu(enddup->length) -
=======
			((char *)enddup - (char *)hdr + be16_to_cpu(enddup->length) -
>>>>>>> refs/remotes/origin/cm-10.0
=======
		xfs_dir2_data_use_free(tp, dp, bp, enddup,
			(xfs_dir2_data_aoff_t)
			((char *)enddup - (char *)hdr + be16_to_cpu(enddup->length) -
>>>>>>> refs/remotes/origin/master
			 sizeof(*blp)),
			(xfs_dir2_data_aoff_t)sizeof(*blp),
			&needlog, &needscan);
		/*
		 * Update the tail (entry count).
		 */
		be32_add_cpu(&btp->count, 1);
		/*
		 * If we now need to rebuild the bestfree map, do so.
		 * This needs to happen before the next call to use_free.
		 */
		if (needscan) {
<<<<<<< HEAD
<<<<<<< HEAD
			xfs_dir2_data_freescan(mp, (xfs_dir2_data_t *)block,
				&needlog);
=======
			xfs_dir2_data_freescan(mp, hdr, &needlog);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			xfs_dir2_data_freescan(dp, hdr, &needlog);
>>>>>>> refs/remotes/origin/master
			needscan = 0;
		}
		/*
		 * Adjust pointer to the first leaf entry, we're about to move
		 * the table up one to open up space for the new leaf entry.
		 * Then adjust our index to match.
		 */
		blp--;
		mid++;
		if (mid)
			memmove(blp, &blp[1], mid * sizeof(*blp));
		lfloglow = 0;
		lfloghigh = mid;
	}
	/*
	 * Use a stale leaf for our new entry.
	 */
	else {
		for (lowstale = mid;
		     lowstale >= 0 &&
<<<<<<< HEAD
<<<<<<< HEAD
			be32_to_cpu(blp[lowstale].address) != XFS_DIR2_NULL_DATAPTR;
=======
			blp[lowstale].address !=
			cpu_to_be32(XFS_DIR2_NULL_DATAPTR);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			blp[lowstale].address !=
			cpu_to_be32(XFS_DIR2_NULL_DATAPTR);
>>>>>>> refs/remotes/origin/master
		     lowstale--)
			continue;
		for (highstale = mid + 1;
		     highstale < be32_to_cpu(btp->count) &&
<<<<<<< HEAD
<<<<<<< HEAD
			be32_to_cpu(blp[highstale].address) != XFS_DIR2_NULL_DATAPTR &&
=======
			blp[highstale].address !=
			cpu_to_be32(XFS_DIR2_NULL_DATAPTR) &&
>>>>>>> refs/remotes/origin/cm-10.0
=======
			blp[highstale].address !=
			cpu_to_be32(XFS_DIR2_NULL_DATAPTR) &&
>>>>>>> refs/remotes/origin/master
			(lowstale < 0 || mid - lowstale > highstale - mid);
		     highstale++)
			continue;
		/*
		 * Move entries toward the low-numbered stale entry.
		 */
		if (lowstale >= 0 &&
		    (highstale == be32_to_cpu(btp->count) ||
		     mid - lowstale <= highstale - mid)) {
			if (mid - lowstale)
				memmove(&blp[lowstale], &blp[lowstale + 1],
					(mid - lowstale) * sizeof(*blp));
			lfloglow = MIN(lowstale, lfloglow);
			lfloghigh = MAX(mid, lfloghigh);
		}
		/*
		 * Move entries toward the high-numbered stale entry.
		 */
		else {
			ASSERT(highstale < be32_to_cpu(btp->count));
			mid++;
			if (highstale - mid)
				memmove(&blp[mid + 1], &blp[mid],
					(highstale - mid) * sizeof(*blp));
			lfloglow = MIN(mid, lfloglow);
			lfloghigh = MAX(highstale, lfloghigh);
		}
		be32_add_cpu(&btp->stale, -1);
	}
	/*
	 * Point to the new data entry.
	 */
	dep = (xfs_dir2_data_entry_t *)dup;
	/*
	 * Fill in the leaf entry.
	 */
	blp[mid].hashval = cpu_to_be32(args->hashval);
	blp[mid].address = cpu_to_be32(xfs_dir2_byte_to_dataptr(mp,
<<<<<<< HEAD
<<<<<<< HEAD
				(char *)dep - (char *)block));
=======
				(char *)dep - (char *)hdr));
>>>>>>> refs/remotes/origin/cm-10.0
=======
				(char *)dep - (char *)hdr));
>>>>>>> refs/remotes/origin/master
	xfs_dir2_block_log_leaf(tp, bp, lfloglow, lfloghigh);
	/*
	 * Mark space for the data entry used.
	 */
<<<<<<< HEAD
	xfs_dir2_data_use_free(tp, bp, dup,
<<<<<<< HEAD
		(xfs_dir2_data_aoff_t)((char *)dup - (char *)block),
=======
		(xfs_dir2_data_aoff_t)((char *)dup - (char *)hdr),
>>>>>>> refs/remotes/origin/cm-10.0
=======
	xfs_dir2_data_use_free(tp, dp, bp, dup,
		(xfs_dir2_data_aoff_t)((char *)dup - (char *)hdr),
>>>>>>> refs/remotes/origin/master
		(xfs_dir2_data_aoff_t)len, &needlog, &needscan);
	/*
	 * Create the new data entry.
	 */
	dep->inumber = cpu_to_be64(args->inumber);
	dep->namelen = args->namelen;
	memcpy(dep->name, args->name, args->namelen);
<<<<<<< HEAD
	tagp = xfs_dir2_data_entry_tag_p(dep);
<<<<<<< HEAD
	*tagp = cpu_to_be16((char *)dep - (char *)block);
=======
	*tagp = cpu_to_be16((char *)dep - (char *)hdr);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dp->d_ops->data_put_ftype(dep, args->filetype);
	tagp = dp->d_ops->data_entry_tag_p(dep);
	*tagp = cpu_to_be16((char *)dep - (char *)hdr);
>>>>>>> refs/remotes/origin/master
	/*
	 * Clean up the bestfree array and log the header, tail, and entry.
	 */
	if (needscan)
<<<<<<< HEAD
<<<<<<< HEAD
		xfs_dir2_data_freescan(mp, (xfs_dir2_data_t *)block, &needlog);
=======
		xfs_dir2_data_freescan(mp, hdr, &needlog);
>>>>>>> refs/remotes/origin/cm-10.0
	if (needlog)
		xfs_dir2_data_log_header(tp, bp);
	xfs_dir2_block_log_tail(tp, bp);
	xfs_dir2_data_log_entry(tp, bp, dep);
	xfs_dir2_data_check(dp, bp);
	xfs_da_buf_done(bp);
	return 0;
}

/*
 * Readdir for block directories.
 */
int						/* error */
xfs_dir2_block_getdents(
	xfs_inode_t		*dp,		/* incore inode */
	void			*dirent,
	xfs_off_t		*offset,
	filldir_t		filldir)
{
<<<<<<< HEAD
	xfs_dir2_block_t	*block;		/* directory block structure */
=======
	xfs_dir2_data_hdr_t	*hdr;		/* block header */
>>>>>>> refs/remotes/origin/cm-10.0
	xfs_dabuf_t		*bp;		/* buffer for block */
	xfs_dir2_block_tail_t	*btp;		/* block tail */
	xfs_dir2_data_entry_t	*dep;		/* block data entry */
	xfs_dir2_data_unused_t	*dup;		/* block unused entry */
	char			*endptr;	/* end of the data entries */
	int			error;		/* error return value */
	xfs_mount_t		*mp;		/* filesystem mount point */
	char			*ptr;		/* current data entry */
	int			wantoff;	/* starting block offset */
	xfs_off_t		cook;

	mp = dp->i_mount;
	/*
	 * If the block number in the offset is out of range, we're done.
	 */
	if (xfs_dir2_dataptr_to_db(mp, *offset) > mp->m_dirdatablk) {
		return 0;
	}
	/*
	 * Can't read the block, give up, else get dabuf in bp.
	 */
	error = xfs_da_read_buf(NULL, dp, mp->m_dirdatablk, -1,
				&bp, XFS_DATA_FORK);
	if (error)
		return error;

	ASSERT(bp != NULL);
	/*
	 * Extract the byte offset we start at from the seek pointer.
	 * We'll skip entries before this.
	 */
	wantoff = xfs_dir2_dataptr_to_off(mp, *offset);
<<<<<<< HEAD
	block = bp->data;
=======
	hdr = bp->data;
>>>>>>> refs/remotes/origin/cm-10.0
	xfs_dir2_data_check(dp, bp);
	/*
	 * Set up values for the loop.
	 */
<<<<<<< HEAD
	btp = xfs_dir2_block_tail_p(mp, block);
	ptr = (char *)block->u;
=======
	btp = xfs_dir2_block_tail_p(mp, hdr);
	ptr = (char *)(hdr + 1);
>>>>>>> refs/remotes/origin/cm-10.0
	endptr = (char *)xfs_dir2_block_leaf_p(btp);

	/*
	 * Loop over the data portion of the block.
	 * Each object is a real entry (dep) or an unused one (dup).
	 */
	while (ptr < endptr) {
		dup = (xfs_dir2_data_unused_t *)ptr;
		/*
		 * Unused, skip it.
		 */
		if (be16_to_cpu(dup->freetag) == XFS_DIR2_DATA_FREE_TAG) {
			ptr += be16_to_cpu(dup->length);
			continue;
		}

		dep = (xfs_dir2_data_entry_t *)ptr;

		/*
		 * Bump pointer for the next iteration.
		 */
		ptr += xfs_dir2_data_entsize(dep->namelen);
		/*
		 * The entry is before the desired starting point, skip it.
		 */
<<<<<<< HEAD
		if ((char *)dep - (char *)block < wantoff)
			continue;

		cook = xfs_dir2_db_off_to_dataptr(mp, mp->m_dirdatablk,
					    (char *)dep - (char *)block);
=======
		if ((char *)dep - (char *)hdr < wantoff)
			continue;

		cook = xfs_dir2_db_off_to_dataptr(mp, mp->m_dirdatablk,
					    (char *)dep - (char *)hdr);
>>>>>>> refs/remotes/origin/cm-10.0

		/*
		 * If it didn't fit, set the final offset to here & return.
		 */
		if (filldir(dirent, (char *)dep->name, dep->namelen,
			    cook & 0x7fffffff, be64_to_cpu(dep->inumber),
			    DT_UNKNOWN)) {
			*offset = cook & 0x7fffffff;
			xfs_da_brelse(NULL, bp);
			return 0;
		}
	}

	/*
	 * Reached the end of the block.
	 * Set the offset to a non-existent block 1 and return.
	 */
	*offset = xfs_dir2_db_off_to_dataptr(mp, mp->m_dirdatablk + 1, 0) &
			0x7fffffff;
	xfs_da_brelse(NULL, bp);
=======
		xfs_dir2_data_freescan(dp, hdr, &needlog);
	if (needlog)
		xfs_dir2_data_log_header(tp, dp, bp);
	xfs_dir2_block_log_tail(tp, bp);
	xfs_dir2_data_log_entry(tp, dp, bp, dep);
	xfs_dir3_data_check(dp, bp);
>>>>>>> refs/remotes/origin/master
	return 0;
}

/*
 * Log leaf entries from the block.
 */
static void
xfs_dir2_block_log_leaf(
	xfs_trans_t		*tp,		/* transaction structure */
<<<<<<< HEAD
	xfs_dabuf_t		*bp,		/* block buffer */
	int			first,		/* index of first logged leaf */
	int			last)		/* index of last logged leaf */
{
<<<<<<< HEAD
	xfs_dir2_block_t	*block;		/* directory block structure */
	xfs_dir2_leaf_entry_t	*blp;		/* block leaf entries */
	xfs_dir2_block_tail_t	*btp;		/* block tail */
	xfs_mount_t		*mp;		/* filesystem mount point */

	mp = tp->t_mountp;
	block = bp->data;
	btp = xfs_dir2_block_tail_p(mp, block);
	blp = xfs_dir2_block_leaf_p(btp);
	xfs_da_log_buf(tp, bp, (uint)((char *)&blp[first] - (char *)block),
		(uint)((char *)&blp[last + 1] - (char *)block - 1));
=======
	xfs_dir2_data_hdr_t	*hdr = bp->data;
=======
	struct xfs_buf		*bp,		/* block buffer */
	int			first,		/* index of first logged leaf */
	int			last)		/* index of last logged leaf */
{
	xfs_dir2_data_hdr_t	*hdr = bp->b_addr;
>>>>>>> refs/remotes/origin/master
	xfs_dir2_leaf_entry_t	*blp;
	xfs_dir2_block_tail_t	*btp;

	btp = xfs_dir2_block_tail_p(tp->t_mountp, hdr);
	blp = xfs_dir2_block_leaf_p(btp);
<<<<<<< HEAD
	xfs_da_log_buf(tp, bp, (uint)((char *)&blp[first] - (char *)hdr),
		(uint)((char *)&blp[last + 1] - (char *)hdr - 1));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	xfs_trans_log_buf(tp, bp, (uint)((char *)&blp[first] - (char *)hdr),
		(uint)((char *)&blp[last + 1] - (char *)hdr - 1));
>>>>>>> refs/remotes/origin/master
}

/*
 * Log the block tail.
 */
static void
xfs_dir2_block_log_tail(
	xfs_trans_t		*tp,		/* transaction structure */
<<<<<<< HEAD
	xfs_dabuf_t		*bp)		/* block buffer */
{
<<<<<<< HEAD
	xfs_dir2_block_t	*block;		/* directory block structure */
	xfs_dir2_block_tail_t	*btp;		/* block tail */
	xfs_mount_t		*mp;		/* filesystem mount point */

	mp = tp->t_mountp;
	block = bp->data;
	btp = xfs_dir2_block_tail_p(mp, block);
	xfs_da_log_buf(tp, bp, (uint)((char *)btp - (char *)block),
		(uint)((char *)(btp + 1) - (char *)block - 1));
=======
	xfs_dir2_data_hdr_t	*hdr = bp->data;
	xfs_dir2_block_tail_t	*btp;

	btp = xfs_dir2_block_tail_p(tp->t_mountp, hdr);
	xfs_da_log_buf(tp, bp, (uint)((char *)btp - (char *)hdr),
		(uint)((char *)(btp + 1) - (char *)hdr - 1));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct xfs_buf		*bp)		/* block buffer */
{
	xfs_dir2_data_hdr_t	*hdr = bp->b_addr;
	xfs_dir2_block_tail_t	*btp;

	btp = xfs_dir2_block_tail_p(tp->t_mountp, hdr);
	xfs_trans_log_buf(tp, bp, (uint)((char *)btp - (char *)hdr),
		(uint)((char *)(btp + 1) - (char *)hdr - 1));
>>>>>>> refs/remotes/origin/master
}

/*
 * Look up an entry in the block.  This is the external routine,
 * xfs_dir2_block_lookup_int does the real work.
 */
int						/* error */
xfs_dir2_block_lookup(
	xfs_da_args_t		*args)		/* dir lookup arguments */
{
<<<<<<< HEAD
<<<<<<< HEAD
	xfs_dir2_block_t	*block;		/* block structure */
=======
	xfs_dir2_data_hdr_t	*hdr;		/* block header */
>>>>>>> refs/remotes/origin/cm-10.0
	xfs_dir2_leaf_entry_t	*blp;		/* block leaf entries */
	xfs_dabuf_t		*bp;		/* block buffer */
=======
	xfs_dir2_data_hdr_t	*hdr;		/* block header */
	xfs_dir2_leaf_entry_t	*blp;		/* block leaf entries */
	struct xfs_buf		*bp;		/* block buffer */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_block_tail_t	*btp;		/* block tail */
	xfs_dir2_data_entry_t	*dep;		/* block data entry */
	xfs_inode_t		*dp;		/* incore inode */
	int			ent;		/* entry index */
	int			error;		/* error return value */
	xfs_mount_t		*mp;		/* filesystem mount point */

	trace_xfs_dir2_block_lookup(args);

	/*
	 * Get the buffer, look up the entry.
	 * If not found (ENOENT) then return, have no buffer.
	 */
	if ((error = xfs_dir2_block_lookup_int(args, &bp, &ent)))
		return error;
	dp = args->dp;
	mp = dp->i_mount;
<<<<<<< HEAD
<<<<<<< HEAD
	block = bp->data;
	xfs_dir2_data_check(dp, bp);
	btp = xfs_dir2_block_tail_p(mp, block);
=======
	hdr = bp->data;
	xfs_dir2_data_check(dp, bp);
	btp = xfs_dir2_block_tail_p(mp, hdr);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	hdr = bp->b_addr;
	xfs_dir3_data_check(dp, bp);
	btp = xfs_dir2_block_tail_p(mp, hdr);
>>>>>>> refs/remotes/origin/master
	blp = xfs_dir2_block_leaf_p(btp);
	/*
	 * Get the offset from the leaf entry, to point to the data.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	dep = (xfs_dir2_data_entry_t *)((char *)block +
=======
	dep = (xfs_dir2_data_entry_t *)((char *)hdr +
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dep = (xfs_dir2_data_entry_t *)((char *)hdr +
>>>>>>> refs/remotes/origin/master
		xfs_dir2_dataptr_to_off(mp, be32_to_cpu(blp[ent].address)));
	/*
	 * Fill in inode number, CI name if appropriate, release the block.
	 */
	args->inumber = be64_to_cpu(dep->inumber);
<<<<<<< HEAD
	error = xfs_dir_cilookup_result(args, dep->name, dep->namelen);
	xfs_da_brelse(args->trans, bp);
=======
	args->filetype = dp->d_ops->data_get_ftype(dep);
	error = xfs_dir_cilookup_result(args, dep->name, dep->namelen);
	xfs_trans_brelse(args->trans, bp);
>>>>>>> refs/remotes/origin/master
	return XFS_ERROR(error);
}

/*
 * Internal block lookup routine.
 */
static int					/* error */
xfs_dir2_block_lookup_int(
	xfs_da_args_t		*args,		/* dir lookup arguments */
<<<<<<< HEAD
	xfs_dabuf_t		**bpp,		/* returned block buffer */
	int			*entno)		/* returned entry number */
{
	xfs_dir2_dataptr_t	addr;		/* data entry address */
<<<<<<< HEAD
	xfs_dir2_block_t	*block;		/* block structure */
=======
	xfs_dir2_data_hdr_t	*hdr;		/* block header */
>>>>>>> refs/remotes/origin/cm-10.0
	xfs_dir2_leaf_entry_t	*blp;		/* block leaf entries */
	xfs_dabuf_t		*bp;		/* block buffer */
=======
	struct xfs_buf		**bpp,		/* returned block buffer */
	int			*entno)		/* returned entry number */
{
	xfs_dir2_dataptr_t	addr;		/* data entry address */
	xfs_dir2_data_hdr_t	*hdr;		/* block header */
	xfs_dir2_leaf_entry_t	*blp;		/* block leaf entries */
	struct xfs_buf		*bp;		/* block buffer */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_block_tail_t	*btp;		/* block tail */
	xfs_dir2_data_entry_t	*dep;		/* block data entry */
	xfs_inode_t		*dp;		/* incore inode */
	int			error;		/* error return value */
	xfs_dahash_t		hash;		/* found hash value */
	int			high;		/* binary search high index */
	int			low;		/* binary search low index */
	int			mid;		/* binary search current idx */
	xfs_mount_t		*mp;		/* filesystem mount point */
	xfs_trans_t		*tp;		/* transaction pointer */
	enum xfs_dacmp		cmp;		/* comparison result */

	dp = args->dp;
	tp = args->trans;
	mp = dp->i_mount;
<<<<<<< HEAD
	/*
	 * Read the buffer, return error if we can't get it.
	 */
	if ((error =
	    xfs_da_read_buf(tp, dp, mp->m_dirdatablk, -1, &bp, XFS_DATA_FORK))) {
		return error;
	}
	ASSERT(bp != NULL);
<<<<<<< HEAD
	block = bp->data;
	xfs_dir2_data_check(dp, bp);
	btp = xfs_dir2_block_tail_p(mp, block);
=======
	hdr = bp->data;
	xfs_dir2_data_check(dp, bp);
	btp = xfs_dir2_block_tail_p(mp, hdr);
>>>>>>> refs/remotes/origin/cm-10.0
=======

	error = xfs_dir3_block_read(tp, dp, &bp);
	if (error)
		return error;

	hdr = bp->b_addr;
	xfs_dir3_data_check(dp, bp);
	btp = xfs_dir2_block_tail_p(mp, hdr);
>>>>>>> refs/remotes/origin/master
	blp = xfs_dir2_block_leaf_p(btp);
	/*
	 * Loop doing a binary search for our hash value.
	 * Find our entry, ENOENT if it's not there.
	 */
	for (low = 0, high = be32_to_cpu(btp->count) - 1; ; ) {
		ASSERT(low <= high);
		mid = (low + high) >> 1;
		if ((hash = be32_to_cpu(blp[mid].hashval)) == args->hashval)
			break;
		if (hash < args->hashval)
			low = mid + 1;
		else
			high = mid - 1;
		if (low > high) {
			ASSERT(args->op_flags & XFS_DA_OP_OKNOENT);
<<<<<<< HEAD
			xfs_da_brelse(tp, bp);
=======
			xfs_trans_brelse(tp, bp);
>>>>>>> refs/remotes/origin/master
			return XFS_ERROR(ENOENT);
		}
	}
	/*
	 * Back up to the first one with the right hash value.
	 */
	while (mid > 0 && be32_to_cpu(blp[mid - 1].hashval) == args->hashval) {
		mid--;
	}
	/*
	 * Now loop forward through all the entries with the
	 * right hash value looking for our name.
	 */
	do {
		if ((addr = be32_to_cpu(blp[mid].address)) == XFS_DIR2_NULL_DATAPTR)
			continue;
		/*
		 * Get pointer to the entry from the leaf.
		 */
		dep = (xfs_dir2_data_entry_t *)
<<<<<<< HEAD
<<<<<<< HEAD
			((char *)block + xfs_dir2_dataptr_to_off(mp, addr));
=======
			((char *)hdr + xfs_dir2_dataptr_to_off(mp, addr));
>>>>>>> refs/remotes/origin/cm-10.0
=======
			((char *)hdr + xfs_dir2_dataptr_to_off(mp, addr));
>>>>>>> refs/remotes/origin/master
		/*
		 * Compare name and if it's an exact match, return the index
		 * and buffer. If it's the first case-insensitive match, store
		 * the index and buffer and continue looking for an exact match.
		 */
		cmp = mp->m_dirnameops->compname(args, dep->name, dep->namelen);
		if (cmp != XFS_CMP_DIFFERENT && cmp != args->cmpresult) {
			args->cmpresult = cmp;
			*bpp = bp;
			*entno = mid;
			if (cmp == XFS_CMP_EXACT)
				return 0;
		}
	} while (++mid < be32_to_cpu(btp->count) &&
			be32_to_cpu(blp[mid].hashval) == hash);

	ASSERT(args->op_flags & XFS_DA_OP_OKNOENT);
	/*
	 * Here, we can only be doing a lookup (not a rename or replace).
	 * If a case-insensitive match was found earlier, return success.
	 */
	if (args->cmpresult == XFS_CMP_CASE)
		return 0;
	/*
	 * No match, release the buffer and return ENOENT.
	 */
<<<<<<< HEAD
	xfs_da_brelse(tp, bp);
=======
	xfs_trans_brelse(tp, bp);
>>>>>>> refs/remotes/origin/master
	return XFS_ERROR(ENOENT);
}

/*
 * Remove an entry from a block format directory.
 * If that makes the block small enough to fit in shortform, transform it.
 */
int						/* error */
xfs_dir2_block_removename(
	xfs_da_args_t		*args)		/* directory operation args */
{
<<<<<<< HEAD
<<<<<<< HEAD
	xfs_dir2_block_t	*block;		/* block structure */
=======
	xfs_dir2_data_hdr_t	*hdr;		/* block header */
>>>>>>> refs/remotes/origin/cm-10.0
	xfs_dir2_leaf_entry_t	*blp;		/* block leaf pointer */
	xfs_dabuf_t		*bp;		/* block buffer */
=======
	xfs_dir2_data_hdr_t	*hdr;		/* block header */
	xfs_dir2_leaf_entry_t	*blp;		/* block leaf pointer */
	struct xfs_buf		*bp;		/* block buffer */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_block_tail_t	*btp;		/* block tail */
	xfs_dir2_data_entry_t	*dep;		/* block data entry */
	xfs_inode_t		*dp;		/* incore inode */
	int			ent;		/* block leaf entry index */
	int			error;		/* error return value */
	xfs_mount_t		*mp;		/* filesystem mount point */
	int			needlog;	/* need to log block header */
	int			needscan;	/* need to fixup bestfree */
	xfs_dir2_sf_hdr_t	sfh;		/* shortform header */
	int			size;		/* shortform size */
	xfs_trans_t		*tp;		/* transaction pointer */

	trace_xfs_dir2_block_removename(args);

	/*
	 * Look up the entry in the block.  Gets the buffer and entry index.
	 * It will always be there, the vnodeops level does a lookup first.
	 */
	if ((error = xfs_dir2_block_lookup_int(args, &bp, &ent))) {
		return error;
	}
	dp = args->dp;
	tp = args->trans;
	mp = dp->i_mount;
<<<<<<< HEAD
<<<<<<< HEAD
	block = bp->data;
	btp = xfs_dir2_block_tail_p(mp, block);
=======
	hdr = bp->data;
	btp = xfs_dir2_block_tail_p(mp, hdr);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	hdr = bp->b_addr;
	btp = xfs_dir2_block_tail_p(mp, hdr);
>>>>>>> refs/remotes/origin/master
	blp = xfs_dir2_block_leaf_p(btp);
	/*
	 * Point to the data entry using the leaf entry.
	 */
	dep = (xfs_dir2_data_entry_t *)
<<<<<<< HEAD
<<<<<<< HEAD
	      ((char *)block + xfs_dir2_dataptr_to_off(mp, be32_to_cpu(blp[ent].address)));
=======
	      ((char *)hdr + xfs_dir2_dataptr_to_off(mp, be32_to_cpu(blp[ent].address)));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	      ((char *)hdr + xfs_dir2_dataptr_to_off(mp, be32_to_cpu(blp[ent].address)));
>>>>>>> refs/remotes/origin/master
	/*
	 * Mark the data entry's space free.
	 */
	needlog = needscan = 0;
<<<<<<< HEAD
	xfs_dir2_data_make_free(tp, bp,
<<<<<<< HEAD
		(xfs_dir2_data_aoff_t)((char *)dep - (char *)block),
=======
		(xfs_dir2_data_aoff_t)((char *)dep - (char *)hdr),
>>>>>>> refs/remotes/origin/cm-10.0
		xfs_dir2_data_entsize(dep->namelen), &needlog, &needscan);
=======
	xfs_dir2_data_make_free(tp, dp, bp,
		(xfs_dir2_data_aoff_t)((char *)dep - (char *)hdr),
		dp->d_ops->data_entsize(dep->namelen), &needlog, &needscan);
>>>>>>> refs/remotes/origin/master
	/*
	 * Fix up the block tail.
	 */
	be32_add_cpu(&btp->stale, 1);
	xfs_dir2_block_log_tail(tp, bp);
	/*
	 * Remove the leaf entry by marking it stale.
	 */
	blp[ent].address = cpu_to_be32(XFS_DIR2_NULL_DATAPTR);
	xfs_dir2_block_log_leaf(tp, bp, ent, ent);
	/*
	 * Fix up bestfree, log the header if necessary.
	 */
	if (needscan)
<<<<<<< HEAD
<<<<<<< HEAD
		xfs_dir2_data_freescan(mp, (xfs_dir2_data_t *)block, &needlog);
=======
		xfs_dir2_data_freescan(mp, hdr, &needlog);
>>>>>>> refs/remotes/origin/cm-10.0
	if (needlog)
		xfs_dir2_data_log_header(tp, bp);
	xfs_dir2_data_check(dp, bp);
	/*
	 * See if the size as a shortform is good enough.
	 */
<<<<<<< HEAD
	if ((size = xfs_dir2_block_sfsize(dp, block, &sfh)) >
	    XFS_IFORK_DSIZE(dp)) {
=======
	size = xfs_dir2_block_sfsize(dp, hdr, &sfh);
	if (size > XFS_IFORK_DSIZE(dp)) {
>>>>>>> refs/remotes/origin/cm-10.0
		xfs_da_buf_done(bp);
		return 0;
	}
=======
		xfs_dir2_data_freescan(dp, hdr, &needlog);
	if (needlog)
		xfs_dir2_data_log_header(tp, dp, bp);
	xfs_dir3_data_check(dp, bp);
	/*
	 * See if the size as a shortform is good enough.
	 */
	size = xfs_dir2_block_sfsize(dp, hdr, &sfh);
	if (size > XFS_IFORK_DSIZE(dp))
		return 0;

>>>>>>> refs/remotes/origin/master
	/*
	 * If it works, do the conversion.
	 */
	return xfs_dir2_block_to_sf(args, bp, size, &sfh);
}

/*
 * Replace an entry in a V2 block directory.
 * Change the inode number to the new value.
 */
int						/* error */
xfs_dir2_block_replace(
	xfs_da_args_t		*args)		/* directory operation args */
{
<<<<<<< HEAD
<<<<<<< HEAD
	xfs_dir2_block_t	*block;		/* block structure */
=======
	xfs_dir2_data_hdr_t	*hdr;		/* block header */
>>>>>>> refs/remotes/origin/cm-10.0
	xfs_dir2_leaf_entry_t	*blp;		/* block leaf entries */
	xfs_dabuf_t		*bp;		/* block buffer */
=======
	xfs_dir2_data_hdr_t	*hdr;		/* block header */
	xfs_dir2_leaf_entry_t	*blp;		/* block leaf entries */
	struct xfs_buf		*bp;		/* block buffer */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_block_tail_t	*btp;		/* block tail */
	xfs_dir2_data_entry_t	*dep;		/* block data entry */
	xfs_inode_t		*dp;		/* incore inode */
	int			ent;		/* leaf entry index */
	int			error;		/* error return value */
	xfs_mount_t		*mp;		/* filesystem mount point */

	trace_xfs_dir2_block_replace(args);

	/*
	 * Lookup the entry in the directory.  Get buffer and entry index.
	 * This will always succeed since the caller has already done a lookup.
	 */
	if ((error = xfs_dir2_block_lookup_int(args, &bp, &ent))) {
		return error;
	}
	dp = args->dp;
	mp = dp->i_mount;
<<<<<<< HEAD
<<<<<<< HEAD
	block = bp->data;
	btp = xfs_dir2_block_tail_p(mp, block);
=======
	hdr = bp->data;
	btp = xfs_dir2_block_tail_p(mp, hdr);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	hdr = bp->b_addr;
	btp = xfs_dir2_block_tail_p(mp, hdr);
>>>>>>> refs/remotes/origin/master
	blp = xfs_dir2_block_leaf_p(btp);
	/*
	 * Point to the data entry we need to change.
	 */
	dep = (xfs_dir2_data_entry_t *)
<<<<<<< HEAD
<<<<<<< HEAD
	      ((char *)block + xfs_dir2_dataptr_to_off(mp, be32_to_cpu(blp[ent].address)));
=======
	      ((char *)hdr + xfs_dir2_dataptr_to_off(mp, be32_to_cpu(blp[ent].address)));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	      ((char *)hdr + xfs_dir2_dataptr_to_off(mp, be32_to_cpu(blp[ent].address)));
>>>>>>> refs/remotes/origin/master
	ASSERT(be64_to_cpu(dep->inumber) != args->inumber);
	/*
	 * Change the inode number to the new value.
	 */
	dep->inumber = cpu_to_be64(args->inumber);
<<<<<<< HEAD
	xfs_dir2_data_log_entry(args->trans, bp, dep);
	xfs_dir2_data_check(dp, bp);
	xfs_da_buf_done(bp);
=======
	dp->d_ops->data_put_ftype(dep, args->filetype);
	xfs_dir2_data_log_entry(args->trans, dp, bp, dep);
	xfs_dir3_data_check(dp, bp);
>>>>>>> refs/remotes/origin/master
	return 0;
}

/*
 * Qsort comparison routine for the block leaf entries.
 */
static int					/* sort order */
xfs_dir2_block_sort(
	const void			*a,	/* first leaf entry */
	const void			*b)	/* second leaf entry */
{
	const xfs_dir2_leaf_entry_t	*la;	/* first leaf entry */
	const xfs_dir2_leaf_entry_t	*lb;	/* second leaf entry */

	la = a;
	lb = b;
	return be32_to_cpu(la->hashval) < be32_to_cpu(lb->hashval) ? -1 :
		(be32_to_cpu(la->hashval) > be32_to_cpu(lb->hashval) ? 1 : 0);
}

/*
 * Convert a V2 leaf directory to a V2 block directory if possible.
 */
int						/* error */
xfs_dir2_leaf_to_block(
	xfs_da_args_t		*args,		/* operation arguments */
<<<<<<< HEAD
	xfs_dabuf_t		*lbp,		/* leaf buffer */
	xfs_dabuf_t		*dbp)		/* data buffer */
{
	__be16			*bestsp;	/* leaf bests table */
<<<<<<< HEAD
	xfs_dir2_block_t	*block;		/* block structure */
=======
	xfs_dir2_data_hdr_t	*hdr;		/* block header */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct xfs_buf		*lbp,		/* leaf buffer */
	struct xfs_buf		*dbp)		/* data buffer */
{
	__be16			*bestsp;	/* leaf bests table */
	xfs_dir2_data_hdr_t	*hdr;		/* block header */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_block_tail_t	*btp;		/* block tail */
	xfs_inode_t		*dp;		/* incore directory inode */
	xfs_dir2_data_unused_t	*dup;		/* unused data entry */
	int			error;		/* error return value */
	int			from;		/* leaf from index */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	xfs_dir2_leaf_entry_t	*lep;		/* leaf entry */
	xfs_dir2_leaf_tail_t	*ltp;		/* leaf tail structure */
	xfs_mount_t		*mp;		/* file system mount point */
	int			needlog;	/* need to log data header */
	int			needscan;	/* need to scan for bestfree */
	xfs_dir2_sf_hdr_t	sfh;		/* shortform header */
	int			size;		/* bytes used */
	__be16			*tagp;		/* end of entry (tag) */
	int			to;		/* block/leaf to index */
	xfs_trans_t		*tp;		/* transaction pointer */
<<<<<<< HEAD
=======
	struct xfs_dir2_leaf_entry *ents;
	struct xfs_dir3_icleaf_hdr leafhdr;
>>>>>>> refs/remotes/origin/master

	trace_xfs_dir2_leaf_to_block(args);

	dp = args->dp;
	tp = args->trans;
	mp = dp->i_mount;
<<<<<<< HEAD
	leaf = lbp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_DIR2_LEAF1_MAGIC);
=======
	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAF1_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	ltp = xfs_dir2_leaf_tail_p(mp, leaf);
=======
	leaf = lbp->b_addr;
	dp->d_ops->leaf_hdr_from_disk(&leafhdr, leaf);
	ents = dp->d_ops->leaf_ents_p(leaf);
	ltp = xfs_dir2_leaf_tail_p(mp, leaf);

	ASSERT(leafhdr.magic == XFS_DIR2_LEAF1_MAGIC ||
	       leafhdr.magic == XFS_DIR3_LEAF1_MAGIC);
>>>>>>> refs/remotes/origin/master
	/*
	 * If there are data blocks other than the first one, take this
	 * opportunity to remove trailing empty data blocks that may have
	 * been left behind during no-space-reservation operations.
	 * These will show up in the leaf bests table.
	 */
	while (dp->i_d.di_size > mp->m_dirblksize) {
<<<<<<< HEAD
		bestsp = xfs_dir2_leaf_bests_p(ltp);
		if (be16_to_cpu(bestsp[be32_to_cpu(ltp->bestcount) - 1]) ==
<<<<<<< HEAD
		    mp->m_dirblksize - (uint)sizeof(block->hdr)) {
=======
		    mp->m_dirblksize - (uint)sizeof(*hdr)) {
>>>>>>> refs/remotes/origin/cm-10.0
			if ((error =
			    xfs_dir2_leaf_trim_data(args, lbp,
				    (xfs_dir2_db_t)(be32_to_cpu(ltp->bestcount) - 1))))
				goto out;
		} else {
			error = 0;
			goto out;
		}
=======
		int hdrsz;

		hdrsz = dp->d_ops->data_entry_offset;
		bestsp = xfs_dir2_leaf_bests_p(ltp);
		if (be16_to_cpu(bestsp[be32_to_cpu(ltp->bestcount) - 1]) ==
					    mp->m_dirblksize - hdrsz) {
			if ((error =
			    xfs_dir2_leaf_trim_data(args, lbp,
				    (xfs_dir2_db_t)(be32_to_cpu(ltp->bestcount) - 1))))
				return error;
		} else
			return 0;
>>>>>>> refs/remotes/origin/master
	}
	/*
	 * Read the data block if we don't already have it, give up if it fails.
	 */
<<<<<<< HEAD
	if (dbp == NULL &&
	    (error = xfs_da_read_buf(tp, dp, mp->m_dirdatablk, -1, &dbp,
		    XFS_DATA_FORK))) {
		goto out;
	}
<<<<<<< HEAD
	block = dbp->data;
	ASSERT(be32_to_cpu(block->hdr.magic) == XFS_DIR2_DATA_MAGIC);
	/*
	 * Size of the "leaf" area in the block.
	 */
	size = (uint)sizeof(block->tail) +
=======
	hdr = dbp->data;
	ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC));
=======
	if (!dbp) {
		error = xfs_dir3_data_read(tp, dp, mp->m_dirdatablk, -1, &dbp);
		if (error)
			return error;
	}
	hdr = dbp->b_addr;
	ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR3_DATA_MAGIC));

>>>>>>> refs/remotes/origin/master
	/*
	 * Size of the "leaf" area in the block.
	 */
	size = (uint)sizeof(xfs_dir2_block_tail_t) +
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	       (uint)sizeof(*lep) * (be16_to_cpu(leaf->hdr.count) - be16_to_cpu(leaf->hdr.stale));
	/*
	 * Look at the last data entry.
	 */
<<<<<<< HEAD
	tagp = (__be16 *)((char *)block + mp->m_dirblksize) - 1;
	dup = (xfs_dir2_data_unused_t *)((char *)block + be16_to_cpu(*tagp));
=======
	tagp = (__be16 *)((char *)hdr + mp->m_dirblksize) - 1;
	dup = (xfs_dir2_data_unused_t *)((char *)hdr + be16_to_cpu(*tagp));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	       (uint)sizeof(*lep) * (leafhdr.count - leafhdr.stale);
	/*
	 * Look at the last data entry.
	 */
	tagp = (__be16 *)((char *)hdr + mp->m_dirblksize) - 1;
	dup = (xfs_dir2_data_unused_t *)((char *)hdr + be16_to_cpu(*tagp));
>>>>>>> refs/remotes/origin/master
	/*
	 * If it's not free or is too short we can't do it.
	 */
	if (be16_to_cpu(dup->freetag) != XFS_DIR2_DATA_FREE_TAG ||
<<<<<<< HEAD
	    be16_to_cpu(dup->length) < size) {
		error = 0;
		goto out;
	}
	/*
	 * Start converting it to block form.
	 */
<<<<<<< HEAD
	block->hdr.magic = cpu_to_be32(XFS_DIR2_BLOCK_MAGIC);
=======
	hdr->magic = cpu_to_be32(XFS_DIR2_BLOCK_MAGIC);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	    be16_to_cpu(dup->length) < size)
		return 0;

	/*
	 * Start converting it to block form.
	 */
	xfs_dir3_block_init(mp, tp, dbp, dp);

>>>>>>> refs/remotes/origin/master
	needlog = 1;
	needscan = 0;
	/*
	 * Use up the space at the end of the block (blp/btp).
	 */
<<<<<<< HEAD
	xfs_dir2_data_use_free(tp, dbp, dup, mp->m_dirblksize - size, size,
=======
	xfs_dir2_data_use_free(tp, dp, dbp, dup, mp->m_dirblksize - size, size,
>>>>>>> refs/remotes/origin/master
		&needlog, &needscan);
	/*
	 * Initialize the block tail.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	btp = xfs_dir2_block_tail_p(mp, block);
=======
	btp = xfs_dir2_block_tail_p(mp, hdr);
>>>>>>> refs/remotes/origin/cm-10.0
	btp->count = cpu_to_be32(be16_to_cpu(leaf->hdr.count) - be16_to_cpu(leaf->hdr.stale));
=======
	btp = xfs_dir2_block_tail_p(mp, hdr);
	btp->count = cpu_to_be32(leafhdr.count - leafhdr.stale);
>>>>>>> refs/remotes/origin/master
	btp->stale = 0;
	xfs_dir2_block_log_tail(tp, dbp);
	/*
	 * Initialize the block leaf area.  We compact out stale entries.
	 */
	lep = xfs_dir2_block_leaf_p(btp);
<<<<<<< HEAD
	for (from = to = 0; from < be16_to_cpu(leaf->hdr.count); from++) {
<<<<<<< HEAD
		if (be32_to_cpu(leaf->ents[from].address) == XFS_DIR2_NULL_DATAPTR)
=======
		if (leaf->ents[from].address ==
		    cpu_to_be32(XFS_DIR2_NULL_DATAPTR))
>>>>>>> refs/remotes/origin/cm-10.0
			continue;
		lep[to++] = leaf->ents[from];
=======
	for (from = to = 0; from < leafhdr.count; from++) {
		if (ents[from].address == cpu_to_be32(XFS_DIR2_NULL_DATAPTR))
			continue;
		lep[to++] = ents[from];
>>>>>>> refs/remotes/origin/master
	}
	ASSERT(to == be32_to_cpu(btp->count));
	xfs_dir2_block_log_leaf(tp, dbp, 0, be32_to_cpu(btp->count) - 1);
	/*
	 * Scan the bestfree if we need it and log the data block header.
	 */
	if (needscan)
<<<<<<< HEAD
<<<<<<< HEAD
		xfs_dir2_data_freescan(mp, (xfs_dir2_data_t *)block, &needlog);
=======
		xfs_dir2_data_freescan(mp, hdr, &needlog);
>>>>>>> refs/remotes/origin/cm-10.0
	if (needlog)
		xfs_dir2_data_log_header(tp, dbp);
=======
		xfs_dir2_data_freescan(dp, hdr, &needlog);
	if (needlog)
		xfs_dir2_data_log_header(tp, dp, dbp);
>>>>>>> refs/remotes/origin/master
	/*
	 * Pitch the old leaf block.
	 */
	error = xfs_da_shrink_inode(args, mp->m_dirleafblk, lbp);
<<<<<<< HEAD
	lbp = NULL;
	if (error) {
		goto out;
	}
	/*
	 * Now see if the resulting block can be shrunken to shortform.
	 */
<<<<<<< HEAD
	if ((size = xfs_dir2_block_sfsize(dp, block, &sfh)) >
	    XFS_IFORK_DSIZE(dp)) {
=======
	size = xfs_dir2_block_sfsize(dp, hdr, &sfh);
	if (size > XFS_IFORK_DSIZE(dp)) {
>>>>>>> refs/remotes/origin/cm-10.0
		error = 0;
		goto out;
	}
	return xfs_dir2_block_to_sf(args, dbp, size, &sfh);
out:
	if (lbp)
		xfs_da_buf_done(lbp);
	if (dbp)
		xfs_da_buf_done(dbp);
	return error;
=======
	if (error)
		return error;

	/*
	 * Now see if the resulting block can be shrunken to shortform.
	 */
	size = xfs_dir2_block_sfsize(dp, hdr, &sfh);
	if (size > XFS_IFORK_DSIZE(dp))
		return 0;

	return xfs_dir2_block_to_sf(args, dbp, size, &sfh);
>>>>>>> refs/remotes/origin/master
}

/*
 * Convert the shortform directory to block form.
 */
int						/* error */
xfs_dir2_sf_to_block(
	xfs_da_args_t		*args)		/* operation arguments */
{
	xfs_dir2_db_t		blkno;		/* dir-relative block # (0) */
<<<<<<< HEAD
<<<<<<< HEAD
	xfs_dir2_block_t	*block;		/* block structure */
	xfs_dir2_leaf_entry_t	*blp;		/* block leaf entries */
	xfs_dabuf_t		*bp;		/* block buffer */
	xfs_dir2_block_tail_t	*btp;		/* block tail pointer */
	char			*buf;		/* sf buffer */
	int			buf_len;
=======
	xfs_dir2_data_hdr_t	*hdr;		/* block header */
	xfs_dir2_leaf_entry_t	*blp;		/* block leaf entries */
	xfs_dabuf_t		*bp;		/* block buffer */
	xfs_dir2_block_tail_t	*btp;		/* block tail pointer */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	xfs_dir2_data_hdr_t	*hdr;		/* block header */
	xfs_dir2_leaf_entry_t	*blp;		/* block leaf entries */
	struct xfs_buf		*bp;		/* block buffer */
	xfs_dir2_block_tail_t	*btp;		/* block tail pointer */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_data_entry_t	*dep;		/* data entry pointer */
	xfs_inode_t		*dp;		/* incore directory inode */
	int			dummy;		/* trash */
	xfs_dir2_data_unused_t	*dup;		/* unused entry pointer */
	int			endoffset;	/* end of data objects */
	int			error;		/* error return value */
	int			i;		/* index */
	xfs_mount_t		*mp;		/* filesystem mount point */
	int			needlog;	/* need to log block header */
	int			needscan;	/* need to scan block freespc */
	int			newoffset;	/* offset from current entry */
	int			offset;		/* target block offset */
	xfs_dir2_sf_entry_t	*sfep;		/* sf entry pointer */
<<<<<<< HEAD
<<<<<<< HEAD
	xfs_dir2_sf_t		*sfp;		/* shortform structure */
=======
	xfs_dir2_sf_hdr_t	*oldsfp;	/* old shortform header  */
	xfs_dir2_sf_hdr_t	*sfp;		/* shortform header  */
>>>>>>> refs/remotes/origin/cm-10.0
	__be16			*tagp;		/* end of data entry */
	xfs_trans_t		*tp;		/* transaction pointer */
	struct xfs_name		name;
=======
	xfs_dir2_sf_hdr_t	*oldsfp;	/* old shortform header  */
	xfs_dir2_sf_hdr_t	*sfp;		/* shortform header  */
	__be16			*tagp;		/* end of data entry */
	xfs_trans_t		*tp;		/* transaction pointer */
	struct xfs_name		name;
	struct xfs_ifork	*ifp;
>>>>>>> refs/remotes/origin/master

	trace_xfs_dir2_sf_to_block(args);

	dp = args->dp;
	tp = args->trans;
	mp = dp->i_mount;
<<<<<<< HEAD
	ASSERT(dp->i_df.if_flags & XFS_IFINLINE);
=======
	ifp = XFS_IFORK_PTR(dp, XFS_DATA_FORK);
	ASSERT(ifp->if_flags & XFS_IFINLINE);
>>>>>>> refs/remotes/origin/master
	/*
	 * Bomb out if the shortform directory is way too short.
	 */
	if (dp->i_d.di_size < offsetof(xfs_dir2_sf_hdr_t, parent)) {
		ASSERT(XFS_FORCED_SHUTDOWN(mp));
		return XFS_ERROR(EIO);
	}
<<<<<<< HEAD
<<<<<<< HEAD
	ASSERT(dp->i_df.if_bytes == dp->i_d.di_size);
	ASSERT(dp->i_df.if_u1.if_data != NULL);
	sfp = (xfs_dir2_sf_t *)dp->i_df.if_u1.if_data;
	ASSERT(dp->i_d.di_size >= xfs_dir2_sf_hdr_size(sfp->hdr.i8count));
	/*
	 * Copy the directory into the stack buffer.
	 * Then pitch the incore inode data so we can make extents.
	 */

	buf_len = dp->i_df.if_bytes;
	buf = kmem_alloc(buf_len, KM_SLEEP);

	memcpy(buf, sfp, buf_len);
	xfs_idata_realloc(dp, -buf_len, XFS_DATA_FORK);
	dp->i_d.di_size = 0;
	xfs_trans_log_inode(tp, dp, XFS_ILOG_CORE);
	/*
	 * Reset pointer - old sfp is gone.
	 */
	sfp = (xfs_dir2_sf_t *)buf;
=======

	oldsfp = (xfs_dir2_sf_hdr_t *)dp->i_df.if_u1.if_data;

	ASSERT(dp->i_df.if_bytes == dp->i_d.di_size);
	ASSERT(dp->i_df.if_u1.if_data != NULL);
	ASSERT(dp->i_d.di_size >= xfs_dir2_sf_hdr_size(oldsfp->i8count));
=======

	oldsfp = (xfs_dir2_sf_hdr_t *)ifp->if_u1.if_data;

	ASSERT(ifp->if_bytes == dp->i_d.di_size);
	ASSERT(ifp->if_u1.if_data != NULL);
	ASSERT(dp->i_d.di_size >= xfs_dir2_sf_hdr_size(oldsfp->i8count));
	ASSERT(dp->i_d.di_nextents == 0);
>>>>>>> refs/remotes/origin/master

	/*
	 * Copy the directory into a temporary buffer.
	 * Then pitch the incore inode data so we can make extents.
	 */
<<<<<<< HEAD
	sfp = kmem_alloc(dp->i_df.if_bytes, KM_SLEEP);
	memcpy(sfp, oldsfp, dp->i_df.if_bytes);

	xfs_idata_realloc(dp, -dp->i_df.if_bytes, XFS_DATA_FORK);
	dp->i_d.di_size = 0;
	xfs_trans_log_inode(tp, dp, XFS_ILOG_CORE);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	sfp = kmem_alloc(ifp->if_bytes, KM_SLEEP);
	memcpy(sfp, oldsfp, ifp->if_bytes);

	xfs_idata_realloc(dp, -ifp->if_bytes, XFS_DATA_FORK);
	xfs_bmap_local_to_extents_empty(dp, XFS_DATA_FORK);
	dp->i_d.di_size = 0;

>>>>>>> refs/remotes/origin/master
	/*
	 * Add block 0 to the inode.
	 */
	error = xfs_dir2_grow_inode(args, XFS_DIR2_DATA_SPACE, &blkno);
	if (error) {
<<<<<<< HEAD
<<<<<<< HEAD
		kmem_free(buf);
=======
		kmem_free(sfp);
>>>>>>> refs/remotes/origin/cm-10.0
		return error;
	}
	/*
	 * Initialize the data block.
	 */
	error = xfs_dir2_data_init(args, blkno, &bp);
	if (error) {
<<<<<<< HEAD
		kmem_free(buf);
		return error;
	}
	block = bp->data;
	block->hdr.magic = cpu_to_be32(XFS_DIR2_BLOCK_MAGIC);
=======
		kmem_free(sfp);
		return error;
	}
	hdr = bp->data;
	hdr->magic = cpu_to_be32(XFS_DIR2_BLOCK_MAGIC);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		kmem_free(sfp);
		return error;
	}
	/*
	 * Initialize the data block, then convert it to block format.
	 */
	error = xfs_dir3_data_init(args, blkno, &bp);
	if (error) {
		kmem_free(sfp);
		return error;
	}
	xfs_dir3_block_init(mp, tp, bp, dp);
	hdr = bp->b_addr;

>>>>>>> refs/remotes/origin/master
	/*
	 * Compute size of block "tail" area.
	 */
	i = (uint)sizeof(*btp) +
<<<<<<< HEAD
<<<<<<< HEAD
	    (sfp->hdr.count + 2) * (uint)sizeof(xfs_dir2_leaf_entry_t);
=======
	    (sfp->count + 2) * (uint)sizeof(xfs_dir2_leaf_entry_t);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	    (sfp->count + 2) * (uint)sizeof(xfs_dir2_leaf_entry_t);
>>>>>>> refs/remotes/origin/master
	/*
	 * The whole thing is initialized to free by the init routine.
	 * Say we're using the leaf and tail area.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	dup = (xfs_dir2_data_unused_t *)block->u;
=======
	dup = (xfs_dir2_data_unused_t *)(hdr + 1);
>>>>>>> refs/remotes/origin/cm-10.0
	needlog = needscan = 0;
	xfs_dir2_data_use_free(tp, bp, dup, mp->m_dirblksize - i, i, &needlog,
=======
	dup = dp->d_ops->data_unused_p(hdr);
	needlog = needscan = 0;
	xfs_dir2_data_use_free(tp, dp, bp, dup, mp->m_dirblksize - i, i, &needlog,
>>>>>>> refs/remotes/origin/master
		&needscan);
	ASSERT(needscan == 0);
	/*
	 * Fill in the tail.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	btp = xfs_dir2_block_tail_p(mp, block);
	btp->count = cpu_to_be32(sfp->hdr.count + 2);	/* ., .. */
	btp->stale = 0;
	blp = xfs_dir2_block_leaf_p(btp);
	endoffset = (uint)((char *)blp - (char *)block);
=======
=======
>>>>>>> refs/remotes/origin/master
	btp = xfs_dir2_block_tail_p(mp, hdr);
	btp->count = cpu_to_be32(sfp->count + 2);	/* ., .. */
	btp->stale = 0;
	blp = xfs_dir2_block_leaf_p(btp);
	endoffset = (uint)((char *)blp - (char *)hdr);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * Remove the freespace, we'll manage it.
	 */
	xfs_dir2_data_use_free(tp, bp, dup,
<<<<<<< HEAD
		(xfs_dir2_data_aoff_t)((char *)dup - (char *)block),
=======
		(xfs_dir2_data_aoff_t)((char *)dup - (char *)hdr),
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/*
	 * Remove the freespace, we'll manage it.
	 */
	xfs_dir2_data_use_free(tp, dp, bp, dup,
		(xfs_dir2_data_aoff_t)((char *)dup - (char *)hdr),
>>>>>>> refs/remotes/origin/master
		be16_to_cpu(dup->length), &needlog, &needscan);
	/*
	 * Create entry for .
	 */
<<<<<<< HEAD
	dep = (xfs_dir2_data_entry_t *)
<<<<<<< HEAD
	      ((char *)block + XFS_DIR2_DATA_DOT_OFFSET);
=======
	      ((char *)hdr + XFS_DIR2_DATA_DOT_OFFSET);
>>>>>>> refs/remotes/origin/cm-10.0
	dep->inumber = cpu_to_be64(dp->i_ino);
	dep->namelen = 1;
	dep->name[0] = '.';
	tagp = xfs_dir2_data_entry_tag_p(dep);
<<<<<<< HEAD
	*tagp = cpu_to_be16((char *)dep - (char *)block);
	xfs_dir2_data_log_entry(tp, bp, dep);
	blp[0].hashval = cpu_to_be32(xfs_dir_hash_dot);
	blp[0].address = cpu_to_be32(xfs_dir2_byte_to_dataptr(mp,
				(char *)dep - (char *)block));
=======
	*tagp = cpu_to_be16((char *)dep - (char *)hdr);
	xfs_dir2_data_log_entry(tp, bp, dep);
	blp[0].hashval = cpu_to_be32(xfs_dir_hash_dot);
	blp[0].address = cpu_to_be32(xfs_dir2_byte_to_dataptr(mp,
				(char *)dep - (char *)hdr));
>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * Create entry for ..
	 */
	dep = (xfs_dir2_data_entry_t *)
<<<<<<< HEAD
		((char *)block + XFS_DIR2_DATA_DOTDOT_OFFSET);
	dep->inumber = cpu_to_be64(xfs_dir2_sf_get_inumber(sfp, &sfp->hdr.parent));
	dep->namelen = 2;
	dep->name[0] = dep->name[1] = '.';
	tagp = xfs_dir2_data_entry_tag_p(dep);
	*tagp = cpu_to_be16((char *)dep - (char *)block);
	xfs_dir2_data_log_entry(tp, bp, dep);
	blp[1].hashval = cpu_to_be32(xfs_dir_hash_dotdot);
	blp[1].address = cpu_to_be32(xfs_dir2_byte_to_dataptr(mp,
				(char *)dep - (char *)block));
=======
		((char *)hdr + XFS_DIR2_DATA_DOTDOT_OFFSET);
	dep->inumber = cpu_to_be64(xfs_dir2_sf_get_parent_ino(sfp));
	dep->namelen = 2;
	dep->name[0] = dep->name[1] = '.';
	tagp = xfs_dir2_data_entry_tag_p(dep);
	*tagp = cpu_to_be16((char *)dep - (char *)hdr);
	xfs_dir2_data_log_entry(tp, bp, dep);
	blp[1].hashval = cpu_to_be32(xfs_dir_hash_dotdot);
	blp[1].address = cpu_to_be32(xfs_dir2_byte_to_dataptr(mp,
				(char *)dep - (char *)hdr));
>>>>>>> refs/remotes/origin/cm-10.0
	offset = XFS_DIR2_DATA_FIRST_OFFSET;
	/*
	 * Loop over existing entries, stuff them in.
	 */
<<<<<<< HEAD
	if ((i = 0) == sfp->hdr.count)
=======
	i = 0;
	if (!sfp->count)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dep = dp->d_ops->data_dot_entry_p(hdr);
	dep->inumber = cpu_to_be64(dp->i_ino);
	dep->namelen = 1;
	dep->name[0] = '.';
	dp->d_ops->data_put_ftype(dep, XFS_DIR3_FT_DIR);
	tagp = dp->d_ops->data_entry_tag_p(dep);
	*tagp = cpu_to_be16((char *)dep - (char *)hdr);
	xfs_dir2_data_log_entry(tp, dp, bp, dep);
	blp[0].hashval = cpu_to_be32(xfs_dir_hash_dot);
	blp[0].address = cpu_to_be32(xfs_dir2_byte_to_dataptr(mp,
				(char *)dep - (char *)hdr));
	/*
	 * Create entry for ..
	 */
	dep = dp->d_ops->data_dotdot_entry_p(hdr);
	dep->inumber = cpu_to_be64(dp->d_ops->sf_get_parent_ino(sfp));
	dep->namelen = 2;
	dep->name[0] = dep->name[1] = '.';
	dp->d_ops->data_put_ftype(dep, XFS_DIR3_FT_DIR);
	tagp = dp->d_ops->data_entry_tag_p(dep);
	*tagp = cpu_to_be16((char *)dep - (char *)hdr);
	xfs_dir2_data_log_entry(tp, dp, bp, dep);
	blp[1].hashval = cpu_to_be32(xfs_dir_hash_dotdot);
	blp[1].address = cpu_to_be32(xfs_dir2_byte_to_dataptr(mp,
				(char *)dep - (char *)hdr));
	offset = dp->d_ops->data_first_offset;
	/*
	 * Loop over existing entries, stuff them in.
	 */
	i = 0;
	if (!sfp->count)
>>>>>>> refs/remotes/origin/master
		sfep = NULL;
	else
		sfep = xfs_dir2_sf_firstentry(sfp);
	/*
	 * Need to preserve the existing offset values in the sf directory.
	 * Insert holes (unused entries) where necessary.
	 */
	while (offset < endoffset) {
		/*
		 * sfep is null when we reach the end of the list.
		 */
		if (sfep == NULL)
			newoffset = endoffset;
		else
			newoffset = xfs_dir2_sf_get_offset(sfep);
		/*
		 * There should be a hole here, make one.
		 */
		if (offset < newoffset) {
<<<<<<< HEAD
<<<<<<< HEAD
			dup = (xfs_dir2_data_unused_t *)
			      ((char *)block + offset);
			dup->freetag = cpu_to_be16(XFS_DIR2_DATA_FREE_TAG);
			dup->length = cpu_to_be16(newoffset - offset);
			*xfs_dir2_data_unused_tag_p(dup) = cpu_to_be16(
				((char *)dup - (char *)block));
			xfs_dir2_data_log_unused(tp, bp, dup);
			(void)xfs_dir2_data_freeinsert((xfs_dir2_data_t *)block,
				dup, &dummy);
=======
=======
>>>>>>> refs/remotes/origin/master
			dup = (xfs_dir2_data_unused_t *)((char *)hdr + offset);
			dup->freetag = cpu_to_be16(XFS_DIR2_DATA_FREE_TAG);
			dup->length = cpu_to_be16(newoffset - offset);
			*xfs_dir2_data_unused_tag_p(dup) = cpu_to_be16(
				((char *)dup - (char *)hdr));
			xfs_dir2_data_log_unused(tp, bp, dup);
<<<<<<< HEAD
			xfs_dir2_data_freeinsert(hdr, dup, &dummy);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			xfs_dir2_data_freeinsert(hdr,
						 dp->d_ops->data_bestfree_p(hdr),
						 dup, &dummy);
>>>>>>> refs/remotes/origin/master
			offset += be16_to_cpu(dup->length);
			continue;
		}
		/*
		 * Copy a real entry.
		 */
<<<<<<< HEAD
<<<<<<< HEAD
		dep = (xfs_dir2_data_entry_t *)((char *)block + newoffset);
		dep->inumber = cpu_to_be64(xfs_dir2_sf_get_inumber(sfp,
				xfs_dir2_sf_inumberp(sfep)));
		dep->namelen = sfep->namelen;
		memcpy(dep->name, sfep->name, dep->namelen);
		tagp = xfs_dir2_data_entry_tag_p(dep);
		*tagp = cpu_to_be16((char *)dep - (char *)block);
=======
		dep = (xfs_dir2_data_entry_t *)((char *)hdr + newoffset);
		dep->inumber = cpu_to_be64(xfs_dir2_sfe_get_ino(sfp, sfep));
		dep->namelen = sfep->namelen;
		memcpy(dep->name, sfep->name, dep->namelen);
		tagp = xfs_dir2_data_entry_tag_p(dep);
		*tagp = cpu_to_be16((char *)dep - (char *)hdr);
>>>>>>> refs/remotes/origin/cm-10.0
		xfs_dir2_data_log_entry(tp, bp, dep);
=======
		dep = (xfs_dir2_data_entry_t *)((char *)hdr + newoffset);
		dep->inumber = cpu_to_be64(dp->d_ops->sf_get_ino(sfp, sfep));
		dep->namelen = sfep->namelen;
		dp->d_ops->data_put_ftype(dep, dp->d_ops->sf_get_ftype(sfep));
		memcpy(dep->name, sfep->name, dep->namelen);
		tagp = dp->d_ops->data_entry_tag_p(dep);
		*tagp = cpu_to_be16((char *)dep - (char *)hdr);
		xfs_dir2_data_log_entry(tp, dp, bp, dep);
>>>>>>> refs/remotes/origin/master
		name.name = sfep->name;
		name.len = sfep->namelen;
		blp[2 + i].hashval = cpu_to_be32(mp->m_dirnameops->
							hashname(&name));
		blp[2 + i].address = cpu_to_be32(xfs_dir2_byte_to_dataptr(mp,
<<<<<<< HEAD
<<<<<<< HEAD
						 (char *)dep - (char *)block));
		offset = (int)((char *)(tagp + 1) - (char *)block);
		if (++i == sfp->hdr.count)
=======
						 (char *)dep - (char *)hdr));
		offset = (int)((char *)(tagp + 1) - (char *)hdr);
		if (++i == sfp->count)
>>>>>>> refs/remotes/origin/cm-10.0
			sfep = NULL;
		else
			sfep = xfs_dir2_sf_nextentry(sfp, sfep);
	}
	/* Done with the temporary buffer */
<<<<<<< HEAD
	kmem_free(buf);
=======
	kmem_free(sfp);
>>>>>>> refs/remotes/origin/cm-10.0
=======
						 (char *)dep - (char *)hdr));
		offset = (int)((char *)(tagp + 1) - (char *)hdr);
		if (++i == sfp->count)
			sfep = NULL;
		else
			sfep = dp->d_ops->sf_nextentry(sfp, sfep);
	}
	/* Done with the temporary buffer */
	kmem_free(sfp);
>>>>>>> refs/remotes/origin/master
	/*
	 * Sort the leaf entries by hash value.
	 */
	xfs_sort(blp, be32_to_cpu(btp->count), sizeof(*blp), xfs_dir2_block_sort);
	/*
	 * Log the leaf entry area and tail.
	 * Already logged the header in data_init, ignore needlog.
	 */
	ASSERT(needscan == 0);
	xfs_dir2_block_log_leaf(tp, bp, 0, be32_to_cpu(btp->count) - 1);
	xfs_dir2_block_log_tail(tp, bp);
<<<<<<< HEAD
	xfs_dir2_data_check(dp, bp);
	xfs_da_buf_done(bp);
=======
	xfs_dir3_data_check(dp, bp);
>>>>>>> refs/remotes/origin/master
	return 0;
}
