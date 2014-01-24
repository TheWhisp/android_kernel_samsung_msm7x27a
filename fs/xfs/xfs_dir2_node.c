/*
 * Copyright (c) 2000-2005 Silicon Graphics, Inc.
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
#include "xfs_bmap.h"
#include "xfs_dir2_data.h"
#include "xfs_dir2_leaf.h"
#include "xfs_dir2_block.h"
#include "xfs_dir2_node.h"
=======
#include "xfs_mount.h"
#include "xfs_da_btree.h"
#include "xfs_bmap_btree.h"
#include "xfs_dinode.h"
#include "xfs_inode.h"
#include "xfs_bmap.h"
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
#include "xfs_bmap.h"
#include "xfs_dir2.h"
#include "xfs_dir2_priv.h"
#include "xfs_error.h"
#include "xfs_trace.h"
#include "xfs_trans.h"
#include "xfs_buf_item.h"
#include "xfs_cksum.h"
>>>>>>> refs/remotes/origin/master

/*
 * Function declarations.
 */
<<<<<<< HEAD
static void xfs_dir2_free_log_header(xfs_trans_t *tp, xfs_dabuf_t *bp);
static int xfs_dir2_leafn_add(xfs_dabuf_t *bp, xfs_da_args_t *args, int index);
#ifdef DEBUG
static void xfs_dir2_leafn_check(xfs_inode_t *dp, xfs_dabuf_t *bp);
#else
#define	xfs_dir2_leafn_check(dp, bp)
#endif
static void xfs_dir2_leafn_moveents(xfs_da_args_t *args, xfs_dabuf_t *bp_s,
				    int start_s, xfs_dabuf_t *bp_d, int start_d,
				    int count);
static void xfs_dir2_leafn_rebalance(xfs_da_state_t *state,
				     xfs_da_state_blk_t *blk1,
				     xfs_da_state_blk_t *blk2);
static int xfs_dir2_leafn_remove(xfs_da_args_t *args, xfs_dabuf_t *bp,
=======
static int xfs_dir2_leafn_add(struct xfs_buf *bp, xfs_da_args_t *args,
			      int index);
static void xfs_dir2_leafn_rebalance(xfs_da_state_t *state,
				     xfs_da_state_blk_t *blk1,
				     xfs_da_state_blk_t *blk2);
static int xfs_dir2_leafn_remove(xfs_da_args_t *args, struct xfs_buf *bp,
>>>>>>> refs/remotes/origin/master
				 int index, xfs_da_state_blk_t *dblk,
				 int *rval);
static int xfs_dir2_node_addname_int(xfs_da_args_t *args,
				     xfs_da_state_blk_t *fblk);

/*
<<<<<<< HEAD
=======
 * Check internal consistency of a leafn block.
 */
#ifdef DEBUG
#define	xfs_dir3_leaf_check(dp, bp) \
do { \
	if (!xfs_dir3_leafn_check((dp), (bp))) \
		ASSERT(0); \
} while (0);

static bool
xfs_dir3_leafn_check(
	struct xfs_inode	*dp,
	struct xfs_buf		*bp)
{
	struct xfs_dir2_leaf	*leaf = bp->b_addr;
	struct xfs_dir3_icleaf_hdr leafhdr;

	dp->d_ops->leaf_hdr_from_disk(&leafhdr, leaf);

	if (leafhdr.magic == XFS_DIR3_LEAFN_MAGIC) {
		struct xfs_dir3_leaf_hdr *leaf3 = bp->b_addr;
		if (be64_to_cpu(leaf3->info.blkno) != bp->b_bn)
			return false;
	} else if (leafhdr.magic != XFS_DIR2_LEAFN_MAGIC)
		return false;

	return xfs_dir3_leaf_check_int(dp->i_mount, dp, &leafhdr, leaf);
}
#else
#define	xfs_dir3_leaf_check(dp, bp)
#endif

static bool
xfs_dir3_free_verify(
	struct xfs_buf		*bp)
{
	struct xfs_mount	*mp = bp->b_target->bt_mount;
	struct xfs_dir2_free_hdr *hdr = bp->b_addr;

	if (xfs_sb_version_hascrc(&mp->m_sb)) {
		struct xfs_dir3_blk_hdr *hdr3 = bp->b_addr;

		if (hdr3->magic != cpu_to_be32(XFS_DIR3_FREE_MAGIC))
			return false;
		if (!uuid_equal(&hdr3->uuid, &mp->m_sb.sb_uuid))
			return false;
		if (be64_to_cpu(hdr3->blkno) != bp->b_bn)
			return false;
	} else {
		if (hdr->magic != cpu_to_be32(XFS_DIR2_FREE_MAGIC))
			return false;
	}

	/* XXX: should bounds check the xfs_dir3_icfree_hdr here */

	return true;
}

static void
xfs_dir3_free_read_verify(
	struct xfs_buf	*bp)
{
	struct xfs_mount	*mp = bp->b_target->bt_mount;

	if ((xfs_sb_version_hascrc(&mp->m_sb) &&
	     !xfs_verify_cksum(bp->b_addr, BBTOB(bp->b_length),
					  XFS_DIR3_FREE_CRC_OFF)) ||
	    !xfs_dir3_free_verify(bp)) {
		XFS_CORRUPTION_ERROR(__func__, XFS_ERRLEVEL_LOW, mp, bp->b_addr);
		xfs_buf_ioerror(bp, EFSCORRUPTED);
	}
}

static void
xfs_dir3_free_write_verify(
	struct xfs_buf	*bp)
{
	struct xfs_mount	*mp = bp->b_target->bt_mount;
	struct xfs_buf_log_item	*bip = bp->b_fspriv;
	struct xfs_dir3_blk_hdr	*hdr3 = bp->b_addr;

	if (!xfs_dir3_free_verify(bp)) {
		XFS_CORRUPTION_ERROR(__func__, XFS_ERRLEVEL_LOW, mp, bp->b_addr);
		xfs_buf_ioerror(bp, EFSCORRUPTED);
		return;
	}

	if (!xfs_sb_version_hascrc(&mp->m_sb))
		return;

	if (bip)
		hdr3->lsn = cpu_to_be64(bip->bli_item.li_lsn);

	xfs_update_cksum(bp->b_addr, BBTOB(bp->b_length), XFS_DIR3_FREE_CRC_OFF);
}

const struct xfs_buf_ops xfs_dir3_free_buf_ops = {
	.verify_read = xfs_dir3_free_read_verify,
	.verify_write = xfs_dir3_free_write_verify,
};


static int
__xfs_dir3_free_read(
	struct xfs_trans	*tp,
	struct xfs_inode	*dp,
	xfs_dablk_t		fbno,
	xfs_daddr_t		mappedbno,
	struct xfs_buf		**bpp)
{
	int			err;

	err = xfs_da_read_buf(tp, dp, fbno, mappedbno, bpp,
				XFS_DATA_FORK, &xfs_dir3_free_buf_ops);

	/* try read returns without an error or *bpp if it lands in a hole */
	if (!err && tp && *bpp)
		xfs_trans_buf_set_type(tp, *bpp, XFS_BLFT_DIR_FREE_BUF);
	return err;
}

int
xfs_dir2_free_read(
	struct xfs_trans	*tp,
	struct xfs_inode	*dp,
	xfs_dablk_t		fbno,
	struct xfs_buf		**bpp)
{
	return __xfs_dir3_free_read(tp, dp, fbno, -1, bpp);
}

static int
xfs_dir2_free_try_read(
	struct xfs_trans	*tp,
	struct xfs_inode	*dp,
	xfs_dablk_t		fbno,
	struct xfs_buf		**bpp)
{
	return __xfs_dir3_free_read(tp, dp, fbno, -2, bpp);
}

static int
xfs_dir3_free_get_buf(
	struct xfs_trans	*tp,
	struct xfs_inode	*dp,
	xfs_dir2_db_t		fbno,
	struct xfs_buf		**bpp)
{
	struct xfs_mount	*mp = dp->i_mount;
	struct xfs_buf		*bp;
	int			error;
	struct xfs_dir3_icfree_hdr hdr;

	error = xfs_da_get_buf(tp, dp, xfs_dir2_db_to_da(mp, fbno),
				   -1, &bp, XFS_DATA_FORK);
	if (error)
		return error;

	xfs_trans_buf_set_type(tp, bp, XFS_BLFT_DIR_FREE_BUF);
	bp->b_ops = &xfs_dir3_free_buf_ops;

	/*
	 * Initialize the new block to be empty, and remember
	 * its first slot as our empty slot.
	 */
	memset(bp->b_addr, 0, sizeof(struct xfs_dir3_free_hdr));
	memset(&hdr, 0, sizeof(hdr));

	if (xfs_sb_version_hascrc(&mp->m_sb)) {
		struct xfs_dir3_free_hdr *hdr3 = bp->b_addr;

		hdr.magic = XFS_DIR3_FREE_MAGIC;

		hdr3->hdr.blkno = cpu_to_be64(bp->b_bn);
		hdr3->hdr.owner = cpu_to_be64(dp->i_ino);
		uuid_copy(&hdr3->hdr.uuid, &mp->m_sb.sb_uuid);
	} else
		hdr.magic = XFS_DIR2_FREE_MAGIC;
	dp->d_ops->free_hdr_to_disk(bp->b_addr, &hdr);
	*bpp = bp;
	return 0;
}

/*
>>>>>>> refs/remotes/origin/master
 * Log entries from a freespace block.
 */
STATIC void
xfs_dir2_free_log_bests(
<<<<<<< HEAD
	xfs_trans_t		*tp,		/* transaction pointer */
	xfs_dabuf_t		*bp,		/* freespace buffer */
=======
	struct xfs_trans	*tp,
	struct xfs_inode	*dp,
	struct xfs_buf		*bp,
>>>>>>> refs/remotes/origin/master
	int			first,		/* first entry to log */
	int			last)		/* last entry to log */
{
	xfs_dir2_free_t		*free;		/* freespace structure */
<<<<<<< HEAD

	free = bp->data;
<<<<<<< HEAD
	ASSERT(be32_to_cpu(free->hdr.magic) == XFS_DIR2_FREE_MAGIC);
=======
	ASSERT(free->hdr.magic == cpu_to_be32(XFS_DIR2_FREE_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	xfs_da_log_buf(tp, bp,
		(uint)((char *)&free->bests[first] - (char *)free),
		(uint)((char *)&free->bests[last] - (char *)free +
		       sizeof(free->bests[0]) - 1));
=======
	__be16			*bests;

	free = bp->b_addr;
	bests = dp->d_ops->free_bests_p(free);
	ASSERT(free->hdr.magic == cpu_to_be32(XFS_DIR2_FREE_MAGIC) ||
	       free->hdr.magic == cpu_to_be32(XFS_DIR3_FREE_MAGIC));
	xfs_trans_log_buf(tp, bp,
		(uint)((char *)&bests[first] - (char *)free),
		(uint)((char *)&bests[last] - (char *)free +
		       sizeof(bests[0]) - 1));
>>>>>>> refs/remotes/origin/master
}

/*
 * Log header from a freespace block.
 */
static void
xfs_dir2_free_log_header(
<<<<<<< HEAD
	xfs_trans_t		*tp,		/* transaction pointer */
	xfs_dabuf_t		*bp)		/* freespace buffer */
{
	xfs_dir2_free_t		*free;		/* freespace structure */

	free = bp->data;
<<<<<<< HEAD
	ASSERT(be32_to_cpu(free->hdr.magic) == XFS_DIR2_FREE_MAGIC);
=======
	ASSERT(free->hdr.magic == cpu_to_be32(XFS_DIR2_FREE_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	xfs_da_log_buf(tp, bp, (uint)((char *)&free->hdr - (char *)free),
		(uint)(sizeof(xfs_dir2_free_hdr_t) - 1));
=======
	struct xfs_trans	*tp,
	struct xfs_inode	*dp,
	struct xfs_buf		*bp)
{
#ifdef DEBUG
	xfs_dir2_free_t		*free;		/* freespace structure */

	free = bp->b_addr;
	ASSERT(free->hdr.magic == cpu_to_be32(XFS_DIR2_FREE_MAGIC) ||
	       free->hdr.magic == cpu_to_be32(XFS_DIR3_FREE_MAGIC));
#endif
	xfs_trans_log_buf(tp, bp, 0, dp->d_ops->free_hdr_size - 1);
>>>>>>> refs/remotes/origin/master
}

/*
 * Convert a leaf-format directory to a node-format directory.
 * We need to change the magic number of the leaf block, and copy
 * the freespace table out of the leaf block into its own block.
 */
int						/* error */
xfs_dir2_leaf_to_node(
	xfs_da_args_t		*args,		/* operation arguments */
<<<<<<< HEAD
	xfs_dabuf_t		*lbp)		/* leaf buffer */
{
	xfs_inode_t		*dp;		/* incore directory inode */
	int			error;		/* error return value */
	xfs_dabuf_t		*fbp;		/* freespace buffer */
=======
	struct xfs_buf		*lbp)		/* leaf buffer */
{
	xfs_inode_t		*dp;		/* incore directory inode */
	int			error;		/* error return value */
	struct xfs_buf		*fbp;		/* freespace buffer */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_db_t		fdb;		/* freespace block number */
	xfs_dir2_free_t		*free;		/* freespace structure */
	__be16			*from;		/* pointer to freespace entry */
	int			i;		/* leaf freespace index */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	xfs_dir2_leaf_tail_t	*ltp;		/* leaf tail structure */
	xfs_mount_t		*mp;		/* filesystem mount point */
	int			n;		/* count of live freespc ents */
	xfs_dir2_data_off_t	off;		/* freespace entry value */
	__be16			*to;		/* pointer to freespace entry */
	xfs_trans_t		*tp;		/* transaction pointer */
<<<<<<< HEAD
=======
	struct xfs_dir3_icfree_hdr freehdr;
>>>>>>> refs/remotes/origin/master

	trace_xfs_dir2_leaf_to_node(args);

	dp = args->dp;
	mp = dp->i_mount;
	tp = args->trans;
	/*
	 * Add a freespace block to the directory.
	 */
	if ((error = xfs_dir2_grow_inode(args, XFS_DIR2_FREE_SPACE, &fdb))) {
		return error;
	}
	ASSERT(fdb == XFS_DIR2_FREE_FIRSTDB(mp));
	/*
	 * Get the buffer for the new freespace block.
	 */
<<<<<<< HEAD
	if ((error = xfs_da_get_buf(tp, dp, xfs_dir2_db_to_da(mp, fdb), -1, &fbp,
			XFS_DATA_FORK))) {
		return error;
	}
	ASSERT(fbp != NULL);
	free = fbp->data;
	leaf = lbp->data;
	ltp = xfs_dir2_leaf_tail_p(mp, leaf);
	/*
	 * Initialize the freespace block header.
	 */
	free->hdr.magic = cpu_to_be32(XFS_DIR2_FREE_MAGIC);
	free->hdr.firstdb = 0;
	ASSERT(be32_to_cpu(ltp->bestcount) <= (uint)dp->i_d.di_size / mp->m_dirblksize);
	free->hdr.nvalid = ltp->bestcount;
=======
	error = xfs_dir3_free_get_buf(tp, dp, fdb, &fbp);
	if (error)
		return error;

	free = fbp->b_addr;
	dp->d_ops->free_hdr_from_disk(&freehdr, free);
	leaf = lbp->b_addr;
	ltp = xfs_dir2_leaf_tail_p(mp, leaf);
	ASSERT(be32_to_cpu(ltp->bestcount) <=
				(uint)dp->i_d.di_size / mp->m_dirblksize);

>>>>>>> refs/remotes/origin/master
	/*
	 * Copy freespace entries from the leaf block to the new block.
	 * Count active entries.
	 */
<<<<<<< HEAD
	for (i = n = 0, from = xfs_dir2_leaf_bests_p(ltp), to = free->bests;
	     i < be32_to_cpu(ltp->bestcount); i++, from++, to++) {
=======
	from = xfs_dir2_leaf_bests_p(ltp);
	to = dp->d_ops->free_bests_p(free);
	for (i = n = 0; i < be32_to_cpu(ltp->bestcount); i++, from++, to++) {
>>>>>>> refs/remotes/origin/master
		if ((off = be16_to_cpu(*from)) != NULLDATAOFF)
			n++;
		*to = cpu_to_be16(off);
	}
<<<<<<< HEAD
	free->hdr.nused = cpu_to_be32(n);
	leaf->hdr.info.magic = cpu_to_be16(XFS_DIR2_LEAFN_MAGIC);
	/*
	 * Log everything.
	 */
	xfs_dir2_leaf_log_header(tp, lbp);
	xfs_dir2_free_log_header(tp, fbp);
	xfs_dir2_free_log_bests(tp, fbp, 0, be32_to_cpu(free->hdr.nvalid) - 1);
	xfs_da_buf_done(fbp);
	xfs_dir2_leafn_check(dp, lbp);
=======

	/*
	 * Now initialize the freespace block header.
	 */
	freehdr.nused = n;
	freehdr.nvalid = be32_to_cpu(ltp->bestcount);

	dp->d_ops->free_hdr_to_disk(fbp->b_addr, &freehdr);
	xfs_dir2_free_log_bests(tp, dp, fbp, 0, freehdr.nvalid - 1);
	xfs_dir2_free_log_header(tp, dp, fbp);

	/*
	 * Converting the leaf to a leafnode is just a matter of changing the
	 * magic number and the ops. Do the change directly to the buffer as
	 * it's less work (and less code) than decoding the header to host
	 * format and back again.
	 */
	if (leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAF1_MAGIC))
		leaf->hdr.info.magic = cpu_to_be16(XFS_DIR2_LEAFN_MAGIC);
	else
		leaf->hdr.info.magic = cpu_to_be16(XFS_DIR3_LEAFN_MAGIC);
	lbp->b_ops = &xfs_dir3_leafn_buf_ops;
	xfs_trans_buf_set_type(tp, lbp, XFS_BLFT_DIR_LEAFN_BUF);
	xfs_dir3_leaf_log_header(tp, dp, lbp);
	xfs_dir3_leaf_check(dp, lbp);
>>>>>>> refs/remotes/origin/master
	return 0;
}

/*
 * Add a leaf entry to a leaf block in a node-form directory.
 * The other work necessary is done from the caller.
 */
static int					/* error */
xfs_dir2_leafn_add(
<<<<<<< HEAD
	xfs_dabuf_t		*bp,		/* leaf buffer */
=======
	struct xfs_buf		*bp,		/* leaf buffer */
>>>>>>> refs/remotes/origin/master
	xfs_da_args_t		*args,		/* operation arguments */
	int			index)		/* insertion pt for new entry */
{
	int			compact;	/* compacting stale leaves */
	xfs_inode_t		*dp;		/* incore directory inode */
	int			highstale;	/* next stale entry */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	xfs_dir2_leaf_entry_t	*lep;		/* leaf entry */
	int			lfloghigh;	/* high leaf entry logging */
	int			lfloglow;	/* low leaf entry logging */
	int			lowstale;	/* previous stale entry */
	xfs_mount_t		*mp;		/* filesystem mount point */
	xfs_trans_t		*tp;		/* transaction pointer */
<<<<<<< HEAD
=======
	struct xfs_dir3_icleaf_hdr leafhdr;
	struct xfs_dir2_leaf_entry *ents;
>>>>>>> refs/remotes/origin/master

	trace_xfs_dir2_leafn_add(args, index);

	dp = args->dp;
	mp = dp->i_mount;
	tp = args->trans;
<<<<<<< HEAD
	leaf = bp->data;
=======
	leaf = bp->b_addr;
	dp->d_ops->leaf_hdr_from_disk(&leafhdr, leaf);
	ents = dp->d_ops->leaf_ents_p(leaf);
>>>>>>> refs/remotes/origin/master

	/*
	 * Quick check just to make sure we are not going to index
	 * into other peoples memory
	 */
	if (index < 0)
		return XFS_ERROR(EFSCORRUPTED);

	/*
	 * If there are already the maximum number of leaf entries in
	 * the block, if there are no stale entries it won't fit.
	 * Caller will do a split.  If there are stale entries we'll do
	 * a compact.
	 */

<<<<<<< HEAD
	if (be16_to_cpu(leaf->hdr.count) == xfs_dir2_max_leaf_ents(mp)) {
		if (!leaf->hdr.stale)
			return XFS_ERROR(ENOSPC);
		compact = be16_to_cpu(leaf->hdr.stale) > 1;
	} else
		compact = 0;
	ASSERT(index == 0 || be32_to_cpu(leaf->ents[index - 1].hashval) <= args->hashval);
	ASSERT(index == be16_to_cpu(leaf->hdr.count) ||
	       be32_to_cpu(leaf->ents[index].hashval) >= args->hashval);
=======
	if (leafhdr.count == dp->d_ops->leaf_max_ents(mp)) {
		if (!leafhdr.stale)
			return XFS_ERROR(ENOSPC);
		compact = leafhdr.stale > 1;
	} else
		compact = 0;
	ASSERT(index == 0 || be32_to_cpu(ents[index - 1].hashval) <= args->hashval);
	ASSERT(index == leafhdr.count ||
	       be32_to_cpu(ents[index].hashval) >= args->hashval);
>>>>>>> refs/remotes/origin/master

	if (args->op_flags & XFS_DA_OP_JUSTCHECK)
		return 0;

	/*
	 * Compact out all but one stale leaf entry.  Leaves behind
	 * the entry closest to index.
	 */
<<<<<<< HEAD
	if (compact) {
		xfs_dir2_leaf_compact_x1(bp, &index, &lowstale, &highstale,
			&lfloglow, &lfloghigh);
	}
	/*
	 * Set impossible logging indices for this case.
	 */
	else if (leaf->hdr.stale) {
		lfloglow = be16_to_cpu(leaf->hdr.count);
		lfloghigh = -1;
	}
<<<<<<< HEAD
	/*
	 * No stale entries, just insert a space for the new entry.
	 */
	if (!leaf->hdr.stale) {
		lep = &leaf->ents[index];
		if (index < be16_to_cpu(leaf->hdr.count))
			memmove(lep + 1, lep,
				(be16_to_cpu(leaf->hdr.count) - index) * sizeof(*lep));
		lfloglow = index;
		lfloghigh = be16_to_cpu(leaf->hdr.count);
		be16_add_cpu(&leaf->hdr.count, 1);
	}
	/*
	 * There are stale entries.  We'll use one for the new entry.
	 */
	else {
		/*
		 * If we didn't do a compact then we need to figure out
		 * which stale entry will be used.
		 */
		if (compact == 0) {
			/*
			 * Find first stale entry before our insertion point.
			 */
			for (lowstale = index - 1;
			     lowstale >= 0 &&
				be32_to_cpu(leaf->ents[lowstale].address) !=
				XFS_DIR2_NULL_DATAPTR;
			     lowstale--)
				continue;
			/*
			 * Find next stale entry after insertion point.
			 * Stop looking if the answer would be worse than
			 * lowstale already found.
			 */
			for (highstale = index;
			     highstale < be16_to_cpu(leaf->hdr.count) &&
				be32_to_cpu(leaf->ents[highstale].address) !=
				XFS_DIR2_NULL_DATAPTR &&
				(lowstale < 0 ||
				 index - lowstale - 1 >= highstale - index);
			     highstale++)
				continue;
		}
		/*
		 * Using the low stale entry.
		 * Shift entries up toward the stale slot.
		 */
		if (lowstale >= 0 &&
		    (highstale == be16_to_cpu(leaf->hdr.count) ||
		     index - lowstale - 1 < highstale - index)) {
			ASSERT(be32_to_cpu(leaf->ents[lowstale].address) ==
			       XFS_DIR2_NULL_DATAPTR);
			ASSERT(index - lowstale - 1 >= 0);
			if (index - lowstale - 1 > 0)
				memmove(&leaf->ents[lowstale],
					&leaf->ents[lowstale + 1],
					(index - lowstale - 1) * sizeof(*lep));
			lep = &leaf->ents[index - 1];
			lfloglow = MIN(lowstale, lfloglow);
			lfloghigh = MAX(index - 1, lfloghigh);
		}
		/*
		 * Using the high stale entry.
		 * Shift entries down toward the stale slot.
		 */
		else {
			ASSERT(be32_to_cpu(leaf->ents[highstale].address) ==
			       XFS_DIR2_NULL_DATAPTR);
			ASSERT(highstale - index >= 0);
			if (highstale - index > 0)
				memmove(&leaf->ents[index + 1],
					&leaf->ents[index],
					(highstale - index) * sizeof(*lep));
			lep = &leaf->ents[index];
			lfloglow = MIN(index, lfloglow);
			lfloghigh = MAX(highstale, lfloghigh);
		}
		be16_add_cpu(&leaf->hdr.stale, -1);
	}
	/*
	 * Insert the new entry, log everything.
	 */
=======
=======
	if (compact)
		xfs_dir3_leaf_compact_x1(&leafhdr, ents, &index, &lowstale,
					 &highstale, &lfloglow, &lfloghigh);
	else if (leafhdr.stale) {
		/*
		 * Set impossible logging indices for this case.
		 */
		lfloglow = leafhdr.count;
		lfloghigh = -1;
	}
>>>>>>> refs/remotes/origin/master

	/*
	 * Insert the new entry, log everything.
	 */
<<<<<<< HEAD
	lep = xfs_dir2_leaf_find_entry(leaf, index, compact, lowstale,
				       highstale, &lfloglow, &lfloghigh);

>>>>>>> refs/remotes/origin/cm-10.0
	lep->hashval = cpu_to_be32(args->hashval);
	lep->address = cpu_to_be32(xfs_dir2_db_off_to_dataptr(mp,
				args->blkno, args->index));
	xfs_dir2_leaf_log_header(tp, bp);
	xfs_dir2_leaf_log_ents(tp, bp, lfloglow, lfloghigh);
	xfs_dir2_leafn_check(dp, bp);
=======
	lep = xfs_dir3_leaf_find_entry(&leafhdr, ents, index, compact, lowstale,
				       highstale, &lfloglow, &lfloghigh);

	lep->hashval = cpu_to_be32(args->hashval);
	lep->address = cpu_to_be32(xfs_dir2_db_off_to_dataptr(mp,
				args->blkno, args->index));

	dp->d_ops->leaf_hdr_to_disk(leaf, &leafhdr);
	xfs_dir3_leaf_log_header(tp, dp, bp);
	xfs_dir3_leaf_log_ents(tp, dp, bp, lfloglow, lfloghigh);
	xfs_dir3_leaf_check(dp, bp);
>>>>>>> refs/remotes/origin/master
	return 0;
}

#ifdef DEBUG
<<<<<<< HEAD
/*
 * Check internal consistency of a leafn block.
 */
void
xfs_dir2_leafn_check(
	xfs_inode_t	*dp,			/* incore directory inode */
	xfs_dabuf_t	*bp)			/* leaf buffer */
{
	int		i;			/* leaf index */
	xfs_dir2_leaf_t	*leaf;			/* leaf structure */
	xfs_mount_t	*mp;			/* filesystem mount point */
	int		stale;			/* count of stale leaves */

	leaf = bp->data;
	mp = dp->i_mount;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_DIR2_LEAFN_MAGIC);
=======
	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAFN_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	ASSERT(be16_to_cpu(leaf->hdr.count) <= xfs_dir2_max_leaf_ents(mp));
	for (i = stale = 0; i < be16_to_cpu(leaf->hdr.count); i++) {
		if (i + 1 < be16_to_cpu(leaf->hdr.count)) {
			ASSERT(be32_to_cpu(leaf->ents[i].hashval) <=
			       be32_to_cpu(leaf->ents[i + 1].hashval));
		}
<<<<<<< HEAD
		if (be32_to_cpu(leaf->ents[i].address) == XFS_DIR2_NULL_DATAPTR)
=======
		if (leaf->ents[i].address == cpu_to_be32(XFS_DIR2_NULL_DATAPTR))
>>>>>>> refs/remotes/origin/cm-10.0
			stale++;
	}
	ASSERT(be16_to_cpu(leaf->hdr.stale) == stale);
}
=======
static void
xfs_dir2_free_hdr_check(
	struct xfs_inode *dp,
	struct xfs_buf	*bp,
	xfs_dir2_db_t	db)
{
	struct xfs_dir3_icfree_hdr hdr;

	dp->d_ops->free_hdr_from_disk(&hdr, bp->b_addr);

	ASSERT((hdr.firstdb % dp->d_ops->free_max_bests(dp->i_mount)) == 0);
	ASSERT(hdr.firstdb <= db);
	ASSERT(db < hdr.firstdb + hdr.nvalid);
}
#else
#define xfs_dir2_free_hdr_check(dp, bp, db)
>>>>>>> refs/remotes/origin/master
#endif	/* DEBUG */

/*
 * Return the last hash value in the leaf.
 * Stale entries are ok.
 */
xfs_dahash_t					/* hash value */
xfs_dir2_leafn_lasthash(
<<<<<<< HEAD
	xfs_dabuf_t	*bp,			/* leaf buffer */
	int		*count)			/* count of entries in leaf */
{
	xfs_dir2_leaf_t	*leaf;			/* leaf structure */

	leaf = bp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_DIR2_LEAFN_MAGIC);
=======
	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAFN_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	if (count)
		*count = be16_to_cpu(leaf->hdr.count);
	if (!leaf->hdr.count)
		return 0;
	return be32_to_cpu(leaf->ents[be16_to_cpu(leaf->hdr.count) - 1].hashval);
=======
	struct xfs_inode *dp,
	struct xfs_buf	*bp,			/* leaf buffer */
	int		*count)			/* count of entries in leaf */
{
	struct xfs_dir2_leaf	*leaf = bp->b_addr;
	struct xfs_dir2_leaf_entry *ents;
	struct xfs_dir3_icleaf_hdr leafhdr;

	dp->d_ops->leaf_hdr_from_disk(&leafhdr, leaf);

	ASSERT(leafhdr.magic == XFS_DIR2_LEAFN_MAGIC ||
	       leafhdr.magic == XFS_DIR3_LEAFN_MAGIC);

	if (count)
		*count = leafhdr.count;
	if (!leafhdr.count)
		return 0;

	ents = dp->d_ops->leaf_ents_p(leaf);
	return be32_to_cpu(ents[leafhdr.count - 1].hashval);
>>>>>>> refs/remotes/origin/master
}

/*
 * Look up a leaf entry for space to add a name in a node-format leaf block.
 * The extrablk in state is a freespace block.
 */
STATIC int
xfs_dir2_leafn_lookup_for_addname(
<<<<<<< HEAD
	xfs_dabuf_t		*bp,		/* leaf buffer */
=======
	struct xfs_buf		*bp,		/* leaf buffer */
>>>>>>> refs/remotes/origin/master
	xfs_da_args_t		*args,		/* operation arguments */
	int			*indexp,	/* out: leaf entry index */
	xfs_da_state_t		*state)		/* state to fill in */
{
<<<<<<< HEAD
	xfs_dabuf_t		*curbp = NULL;	/* current data/free buffer */
=======
	struct xfs_buf		*curbp = NULL;	/* current data/free buffer */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_db_t		curdb = -1;	/* current data block number */
	xfs_dir2_db_t		curfdb = -1;	/* current free block number */
	xfs_inode_t		*dp;		/* incore directory inode */
	int			error;		/* error return value */
	int			fi;		/* free entry index */
	xfs_dir2_free_t		*free = NULL;	/* free block structure */
	int			index;		/* leaf entry index */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	int			length;		/* length of new data entry */
	xfs_dir2_leaf_entry_t	*lep;		/* leaf entry */
	xfs_mount_t		*mp;		/* filesystem mount point */
	xfs_dir2_db_t		newdb;		/* new data block number */
	xfs_dir2_db_t		newfdb;		/* new free block number */
	xfs_trans_t		*tp;		/* transaction pointer */
<<<<<<< HEAD
=======
	struct xfs_dir2_leaf_entry *ents;
	struct xfs_dir3_icleaf_hdr leafhdr;
>>>>>>> refs/remotes/origin/master

	dp = args->dp;
	tp = args->trans;
	mp = dp->i_mount;
<<<<<<< HEAD
	leaf = bp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_DIR2_LEAFN_MAGIC);
=======
	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAFN_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
#ifdef __KERNEL__
	ASSERT(be16_to_cpu(leaf->hdr.count) > 0);
#endif
	xfs_dir2_leafn_check(dp, bp);
=======
	leaf = bp->b_addr;
	dp->d_ops->leaf_hdr_from_disk(&leafhdr, leaf);
	ents = dp->d_ops->leaf_ents_p(leaf);

	xfs_dir3_leaf_check(dp, bp);
	ASSERT(leafhdr.count > 0);

>>>>>>> refs/remotes/origin/master
	/*
	 * Look up the hash value in the leaf entries.
	 */
	index = xfs_dir2_leaf_search_hash(args, bp);
	/*
	 * Do we have a buffer coming in?
	 */
	if (state->extravalid) {
		/* If so, it's a free block buffer, get the block number. */
		curbp = state->extrablk.bp;
		curfdb = state->extrablk.blkno;
<<<<<<< HEAD
		free = curbp->data;
<<<<<<< HEAD
		ASSERT(be32_to_cpu(free->hdr.magic) == XFS_DIR2_FREE_MAGIC);
=======
		ASSERT(free->hdr.magic == cpu_to_be32(XFS_DIR2_FREE_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	}
	length = xfs_dir2_data_entsize(args->namelen);
	/*
	 * Loop over leaf entries with the right hash value.
	 */
	for (lep = &leaf->ents[index]; index < be16_to_cpu(leaf->hdr.count) &&
				be32_to_cpu(lep->hashval) == args->hashval;
				lep++, index++) {
=======
		free = curbp->b_addr;
		ASSERT(free->hdr.magic == cpu_to_be32(XFS_DIR2_FREE_MAGIC) ||
		       free->hdr.magic == cpu_to_be32(XFS_DIR3_FREE_MAGIC));
	}
	length = dp->d_ops->data_entsize(args->namelen);
	/*
	 * Loop over leaf entries with the right hash value.
	 */
	for (lep = &ents[index];
	     index < leafhdr.count && be32_to_cpu(lep->hashval) == args->hashval;
	     lep++, index++) {
>>>>>>> refs/remotes/origin/master
		/*
		 * Skip stale leaf entries.
		 */
		if (be32_to_cpu(lep->address) == XFS_DIR2_NULL_DATAPTR)
			continue;
		/*
		 * Pull the data block number from the entry.
		 */
		newdb = xfs_dir2_dataptr_to_db(mp, be32_to_cpu(lep->address));
		/*
		 * For addname, we're looking for a place to put the new entry.
		 * We want to use a data block with an entry of equal
		 * hash value to ours if there is one with room.
		 *
		 * If this block isn't the data block we already have
		 * in hand, take a look at it.
		 */
		if (newdb != curdb) {
<<<<<<< HEAD
=======
			__be16 *bests;

>>>>>>> refs/remotes/origin/master
			curdb = newdb;
			/*
			 * Convert the data block to the free block
			 * holding its freespace information.
			 */
<<<<<<< HEAD
			newfdb = xfs_dir2_db_to_fdb(mp, newdb);
=======
			newfdb = dp->d_ops->db_to_fdb(mp, newdb);
>>>>>>> refs/remotes/origin/master
			/*
			 * If it's not the one we have in hand, read it in.
			 */
			if (newfdb != curfdb) {
				/*
				 * If we had one before, drop it.
				 */
				if (curbp)
<<<<<<< HEAD
					xfs_da_brelse(tp, curbp);
				/*
				 * Read the free block.
				 */
				error = xfs_da_read_buf(tp, dp,
						xfs_dir2_db_to_da(mp, newfdb),
						-1, &curbp, XFS_DATA_FORK);
				if (error)
					return error;
				free = curbp->data;
				ASSERT(be32_to_cpu(free->hdr.magic) ==
					XFS_DIR2_FREE_MAGIC);
				ASSERT((be32_to_cpu(free->hdr.firstdb) %
<<<<<<< HEAD
					XFS_DIR2_MAX_FREE_BESTS(mp)) == 0);
=======
					xfs_dir2_free_max_bests(mp)) == 0);
>>>>>>> refs/remotes/origin/cm-10.0
				ASSERT(be32_to_cpu(free->hdr.firstdb) <= curdb);
				ASSERT(curdb < be32_to_cpu(free->hdr.firstdb) +
					be32_to_cpu(free->hdr.nvalid));
=======
					xfs_trans_brelse(tp, curbp);

				error = xfs_dir2_free_read(tp, dp,
						xfs_dir2_db_to_da(mp, newfdb),
						&curbp);
				if (error)
					return error;
				free = curbp->b_addr;

				xfs_dir2_free_hdr_check(dp, curbp, curdb);
>>>>>>> refs/remotes/origin/master
			}
			/*
			 * Get the index for our entry.
			 */
<<<<<<< HEAD
			fi = xfs_dir2_db_to_fdindex(mp, curdb);
			/*
			 * If it has room, return it.
			 */
<<<<<<< HEAD
			if (unlikely(be16_to_cpu(free->bests[fi]) == NULLDATAOFF)) {
=======
			if (unlikely(free->bests[fi] ==
			    cpu_to_be16(NULLDATAOFF))) {
>>>>>>> refs/remotes/origin/cm-10.0
				XFS_ERROR_REPORT("xfs_dir2_leafn_lookup_int",
							XFS_ERRLEVEL_LOW, mp);
				if (curfdb != newfdb)
					xfs_da_brelse(tp, curbp);
				return XFS_ERROR(EFSCORRUPTED);
			}
			curfdb = newfdb;
			if (be16_to_cpu(free->bests[fi]) >= length)
=======
			fi = dp->d_ops->db_to_fdindex(mp, curdb);
			/*
			 * If it has room, return it.
			 */
			bests = dp->d_ops->free_bests_p(free);
			if (unlikely(bests[fi] == cpu_to_be16(NULLDATAOFF))) {
				XFS_ERROR_REPORT("xfs_dir2_leafn_lookup_int",
							XFS_ERRLEVEL_LOW, mp);
				if (curfdb != newfdb)
					xfs_trans_brelse(tp, curbp);
				return XFS_ERROR(EFSCORRUPTED);
			}
			curfdb = newfdb;
			if (be16_to_cpu(bests[fi]) >= length)
>>>>>>> refs/remotes/origin/master
				goto out;
		}
	}
	/* Didn't find any space */
	fi = -1;
out:
	ASSERT(args->op_flags & XFS_DA_OP_OKNOENT);
	if (curbp) {
		/* Giving back a free block. */
		state->extravalid = 1;
		state->extrablk.bp = curbp;
		state->extrablk.index = fi;
		state->extrablk.blkno = curfdb;
<<<<<<< HEAD
=======

		/*
		 * Important: this magic number is not in the buffer - it's for
		 * buffer type information and therefore only the free/data type
		 * matters here, not whether CRCs are enabled or not.
		 */
>>>>>>> refs/remotes/origin/master
		state->extrablk.magic = XFS_DIR2_FREE_MAGIC;
	} else {
		state->extravalid = 0;
	}
	/*
	 * Return the index, that will be the insertion point.
	 */
	*indexp = index;
	return XFS_ERROR(ENOENT);
}

/*
 * Look up a leaf entry in a node-format leaf block.
 * The extrablk in state a data block.
 */
STATIC int
xfs_dir2_leafn_lookup_for_entry(
<<<<<<< HEAD
	xfs_dabuf_t		*bp,		/* leaf buffer */
=======
	struct xfs_buf		*bp,		/* leaf buffer */
>>>>>>> refs/remotes/origin/master
	xfs_da_args_t		*args,		/* operation arguments */
	int			*indexp,	/* out: leaf entry index */
	xfs_da_state_t		*state)		/* state to fill in */
{
<<<<<<< HEAD
	xfs_dabuf_t		*curbp = NULL;	/* current data/free buffer */
=======
	struct xfs_buf		*curbp = NULL;	/* current data/free buffer */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_db_t		curdb = -1;	/* current data block number */
	xfs_dir2_data_entry_t	*dep;		/* data block entry */
	xfs_inode_t		*dp;		/* incore directory inode */
	int			error;		/* error return value */
	int			index;		/* leaf entry index */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	xfs_dir2_leaf_entry_t	*lep;		/* leaf entry */
	xfs_mount_t		*mp;		/* filesystem mount point */
	xfs_dir2_db_t		newdb;		/* new data block number */
	xfs_trans_t		*tp;		/* transaction pointer */
	enum xfs_dacmp		cmp;		/* comparison result */
<<<<<<< HEAD
=======
	struct xfs_dir2_leaf_entry *ents;
	struct xfs_dir3_icleaf_hdr leafhdr;
>>>>>>> refs/remotes/origin/master

	dp = args->dp;
	tp = args->trans;
	mp = dp->i_mount;
<<<<<<< HEAD
	leaf = bp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_DIR2_LEAFN_MAGIC);
=======
	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAFN_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
#ifdef __KERNEL__
	ASSERT(be16_to_cpu(leaf->hdr.count) > 0);
#endif
	xfs_dir2_leafn_check(dp, bp);
=======
	leaf = bp->b_addr;
	dp->d_ops->leaf_hdr_from_disk(&leafhdr, leaf);
	ents = dp->d_ops->leaf_ents_p(leaf);

	xfs_dir3_leaf_check(dp, bp);
	ASSERT(leafhdr.count > 0);

>>>>>>> refs/remotes/origin/master
	/*
	 * Look up the hash value in the leaf entries.
	 */
	index = xfs_dir2_leaf_search_hash(args, bp);
	/*
	 * Do we have a buffer coming in?
	 */
	if (state->extravalid) {
		curbp = state->extrablk.bp;
		curdb = state->extrablk.blkno;
	}
	/*
	 * Loop over leaf entries with the right hash value.
	 */
<<<<<<< HEAD
	for (lep = &leaf->ents[index]; index < be16_to_cpu(leaf->hdr.count) &&
				be32_to_cpu(lep->hashval) == args->hashval;
				lep++, index++) {
=======
	for (lep = &ents[index];
	     index < leafhdr.count && be32_to_cpu(lep->hashval) == args->hashval;
	     lep++, index++) {
>>>>>>> refs/remotes/origin/master
		/*
		 * Skip stale leaf entries.
		 */
		if (be32_to_cpu(lep->address) == XFS_DIR2_NULL_DATAPTR)
			continue;
		/*
		 * Pull the data block number from the entry.
		 */
		newdb = xfs_dir2_dataptr_to_db(mp, be32_to_cpu(lep->address));
		/*
		 * Not adding a new entry, so we really want to find
		 * the name given to us.
		 *
		 * If it's a different data block, go get it.
		 */
		if (newdb != curdb) {
			/*
			 * If we had a block before that we aren't saving
			 * for a CI name, drop it
			 */
			if (curbp && (args->cmpresult == XFS_CMP_DIFFERENT ||
						curdb != state->extrablk.blkno))
<<<<<<< HEAD
				xfs_da_brelse(tp, curbp);
=======
				xfs_trans_brelse(tp, curbp);
>>>>>>> refs/remotes/origin/master
			/*
			 * If needing the block that is saved with a CI match,
			 * use it otherwise read in the new data block.
			 */
			if (args->cmpresult != XFS_CMP_DIFFERENT &&
					newdb == state->extrablk.blkno) {
				ASSERT(state->extravalid);
				curbp = state->extrablk.bp;
			} else {
<<<<<<< HEAD
				error = xfs_da_read_buf(tp, dp,
						xfs_dir2_db_to_da(mp, newdb),
						-1, &curbp, XFS_DATA_FORK);
				if (error)
					return error;
			}
			xfs_dir2_data_check(dp, curbp);
=======
				error = xfs_dir3_data_read(tp, dp,
						xfs_dir2_db_to_da(mp, newdb),
						-1, &curbp);
				if (error)
					return error;
			}
			xfs_dir3_data_check(dp, curbp);
>>>>>>> refs/remotes/origin/master
			curdb = newdb;
		}
		/*
		 * Point to the data entry.
		 */
<<<<<<< HEAD
		dep = (xfs_dir2_data_entry_t *)((char *)curbp->data +
=======
		dep = (xfs_dir2_data_entry_t *)((char *)curbp->b_addr +
>>>>>>> refs/remotes/origin/master
			xfs_dir2_dataptr_to_off(mp, be32_to_cpu(lep->address)));
		/*
		 * Compare the entry and if it's an exact match, return
		 * EEXIST immediately. If it's the first case-insensitive
		 * match, store the block & inode number and continue looking.
		 */
		cmp = mp->m_dirnameops->compname(args, dep->name, dep->namelen);
		if (cmp != XFS_CMP_DIFFERENT && cmp != args->cmpresult) {
			/* If there is a CI match block, drop it */
			if (args->cmpresult != XFS_CMP_DIFFERENT &&
						curdb != state->extrablk.blkno)
<<<<<<< HEAD
				xfs_da_brelse(tp, state->extrablk.bp);
			args->cmpresult = cmp;
			args->inumber = be64_to_cpu(dep->inumber);
=======
				xfs_trans_brelse(tp, state->extrablk.bp);
			args->cmpresult = cmp;
			args->inumber = be64_to_cpu(dep->inumber);
			args->filetype = dp->d_ops->data_get_ftype(dep);
>>>>>>> refs/remotes/origin/master
			*indexp = index;
			state->extravalid = 1;
			state->extrablk.bp = curbp;
			state->extrablk.blkno = curdb;
			state->extrablk.index = (int)((char *)dep -
<<<<<<< HEAD
							(char *)curbp->data);
			state->extrablk.magic = XFS_DIR2_DATA_MAGIC;
=======
							(char *)curbp->b_addr);
			state->extrablk.magic = XFS_DIR2_DATA_MAGIC;
			curbp->b_ops = &xfs_dir3_data_buf_ops;
			xfs_trans_buf_set_type(tp, curbp, XFS_BLFT_DIR_DATA_BUF);
>>>>>>> refs/remotes/origin/master
			if (cmp == XFS_CMP_EXACT)
				return XFS_ERROR(EEXIST);
		}
	}
<<<<<<< HEAD
	ASSERT(index == be16_to_cpu(leaf->hdr.count) ||
					(args->op_flags & XFS_DA_OP_OKNOENT));
=======
	ASSERT(index == leafhdr.count || (args->op_flags & XFS_DA_OP_OKNOENT));
>>>>>>> refs/remotes/origin/master
	if (curbp) {
		if (args->cmpresult == XFS_CMP_DIFFERENT) {
			/* Giving back last used data block. */
			state->extravalid = 1;
			state->extrablk.bp = curbp;
			state->extrablk.index = -1;
			state->extrablk.blkno = curdb;
			state->extrablk.magic = XFS_DIR2_DATA_MAGIC;
<<<<<<< HEAD
		} else {
			/* If the curbp is not the CI match block, drop it */
			if (state->extrablk.bp != curbp)
				xfs_da_brelse(tp, curbp);
=======
			curbp->b_ops = &xfs_dir3_data_buf_ops;
			xfs_trans_buf_set_type(tp, curbp, XFS_BLFT_DIR_DATA_BUF);
		} else {
			/* If the curbp is not the CI match block, drop it */
			if (state->extrablk.bp != curbp)
				xfs_trans_brelse(tp, curbp);
>>>>>>> refs/remotes/origin/master
		}
	} else {
		state->extravalid = 0;
	}
	*indexp = index;
	return XFS_ERROR(ENOENT);
}

/*
 * Look up a leaf entry in a node-format leaf block.
 * If this is an addname then the extrablk in state is a freespace block,
 * otherwise it's a data block.
 */
int
xfs_dir2_leafn_lookup_int(
<<<<<<< HEAD
	xfs_dabuf_t		*bp,		/* leaf buffer */
=======
	struct xfs_buf		*bp,		/* leaf buffer */
>>>>>>> refs/remotes/origin/master
	xfs_da_args_t		*args,		/* operation arguments */
	int			*indexp,	/* out: leaf entry index */
	xfs_da_state_t		*state)		/* state to fill in */
{
	if (args->op_flags & XFS_DA_OP_ADDNAME)
		return xfs_dir2_leafn_lookup_for_addname(bp, args, indexp,
							state);
	return xfs_dir2_leafn_lookup_for_entry(bp, args, indexp, state);
}

/*
 * Move count leaf entries from source to destination leaf.
 * Log entries and headers.  Stale entries are preserved.
 */
static void
<<<<<<< HEAD
xfs_dir2_leafn_moveents(
	xfs_da_args_t	*args,			/* operation arguments */
	xfs_dabuf_t	*bp_s,			/* source leaf buffer */
	int		start_s,		/* source leaf index */
	xfs_dabuf_t	*bp_d,			/* destination leaf buffer */
	int		start_d,		/* destination leaf index */
	int		count)			/* count of leaves to copy */
{
	xfs_dir2_leaf_t	*leaf_d;		/* destination leaf structure */
	xfs_dir2_leaf_t	*leaf_s;		/* source leaf structure */
	int		stale;			/* count stale leaves copied */
	xfs_trans_t	*tp;			/* transaction pointer */
=======
xfs_dir3_leafn_moveents(
	xfs_da_args_t			*args,	/* operation arguments */
	struct xfs_buf			*bp_s,	/* source */
	struct xfs_dir3_icleaf_hdr	*shdr,
	struct xfs_dir2_leaf_entry	*sents,
	int				start_s,/* source leaf index */
	struct xfs_buf			*bp_d,	/* destination */
	struct xfs_dir3_icleaf_hdr	*dhdr,
	struct xfs_dir2_leaf_entry	*dents,
	int				start_d,/* destination leaf index */
	int				count)	/* count of leaves to copy */
{
	struct xfs_trans		*tp = args->trans;
	int				stale;	/* count stale leaves copied */
>>>>>>> refs/remotes/origin/master

	trace_xfs_dir2_leafn_moveents(args, start_s, start_d, count);

	/*
	 * Silently return if nothing to do.
	 */
<<<<<<< HEAD
	if (count == 0) {
		return;
	}
	tp = args->trans;
	leaf_s = bp_s->data;
	leaf_d = bp_d->data;
=======
	if (count == 0)
		return;

>>>>>>> refs/remotes/origin/master
	/*
	 * If the destination index is not the end of the current
	 * destination leaf entries, open up a hole in the destination
	 * to hold the new entries.
	 */
<<<<<<< HEAD
	if (start_d < be16_to_cpu(leaf_d->hdr.count)) {
		memmove(&leaf_d->ents[start_d + count], &leaf_d->ents[start_d],
			(be16_to_cpu(leaf_d->hdr.count) - start_d) *
			sizeof(xfs_dir2_leaf_entry_t));
		xfs_dir2_leaf_log_ents(tp, bp_d, start_d + count,
			count + be16_to_cpu(leaf_d->hdr.count) - 1);
=======
	if (start_d < dhdr->count) {
		memmove(&dents[start_d + count], &dents[start_d],
			(dhdr->count - start_d) * sizeof(xfs_dir2_leaf_entry_t));
		xfs_dir3_leaf_log_ents(tp, args->dp, bp_d, start_d + count,
				       count + dhdr->count - 1);
>>>>>>> refs/remotes/origin/master
	}
	/*
	 * If the source has stale leaves, count the ones in the copy range
	 * so we can update the header correctly.
	 */
<<<<<<< HEAD
	if (leaf_s->hdr.stale) {
		int	i;			/* temp leaf index */

		for (i = start_s, stale = 0; i < start_s + count; i++) {
<<<<<<< HEAD
			if (be32_to_cpu(leaf_s->ents[i].address) == XFS_DIR2_NULL_DATAPTR)
=======
			if (leaf_s->ents[i].address ==
			    cpu_to_be32(XFS_DIR2_NULL_DATAPTR))
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (shdr->stale) {
		int	i;			/* temp leaf index */

		for (i = start_s, stale = 0; i < start_s + count; i++) {
			if (sents[i].address ==
					cpu_to_be32(XFS_DIR2_NULL_DATAPTR))
>>>>>>> refs/remotes/origin/master
				stale++;
		}
	} else
		stale = 0;
	/*
	 * Copy the leaf entries from source to destination.
	 */
<<<<<<< HEAD
	memcpy(&leaf_d->ents[start_d], &leaf_s->ents[start_s],
		count * sizeof(xfs_dir2_leaf_entry_t));
	xfs_dir2_leaf_log_ents(tp, bp_d, start_d, start_d + count - 1);
=======
	memcpy(&dents[start_d], &sents[start_s],
		count * sizeof(xfs_dir2_leaf_entry_t));
	xfs_dir3_leaf_log_ents(tp, args->dp, bp_d,
			       start_d, start_d + count - 1);

>>>>>>> refs/remotes/origin/master
	/*
	 * If there are source entries after the ones we copied,
	 * delete the ones we copied by sliding the next ones down.
	 */
<<<<<<< HEAD
	if (start_s + count < be16_to_cpu(leaf_s->hdr.count)) {
		memmove(&leaf_s->ents[start_s], &leaf_s->ents[start_s + count],
			count * sizeof(xfs_dir2_leaf_entry_t));
		xfs_dir2_leaf_log_ents(tp, bp_s, start_s, start_s + count - 1);
	}
	/*
	 * Update the headers and log them.
	 */
	be16_add_cpu(&leaf_s->hdr.count, -(count));
	be16_add_cpu(&leaf_s->hdr.stale, -(stale));
	be16_add_cpu(&leaf_d->hdr.count, count);
	be16_add_cpu(&leaf_d->hdr.stale, stale);
	xfs_dir2_leaf_log_header(tp, bp_s);
	xfs_dir2_leaf_log_header(tp, bp_d);
	xfs_dir2_leafn_check(args->dp, bp_s);
	xfs_dir2_leafn_check(args->dp, bp_d);
=======
	if (start_s + count < shdr->count) {
		memmove(&sents[start_s], &sents[start_s + count],
			count * sizeof(xfs_dir2_leaf_entry_t));
		xfs_dir3_leaf_log_ents(tp, args->dp, bp_s,
				       start_s, start_s + count - 1);
	}

	/*
	 * Update the headers and log them.
	 */
	shdr->count -= count;
	shdr->stale -= stale;
	dhdr->count += count;
	dhdr->stale += stale;
>>>>>>> refs/remotes/origin/master
}

/*
 * Determine the sort order of two leaf blocks.
 * Returns 1 if both are valid and leaf2 should be before leaf1, else 0.
 */
int						/* sort order */
xfs_dir2_leafn_order(
<<<<<<< HEAD
	xfs_dabuf_t	*leaf1_bp,		/* leaf1 buffer */
	xfs_dabuf_t	*leaf2_bp)		/* leaf2 buffer */
{
	xfs_dir2_leaf_t	*leaf1;			/* leaf1 structure */
	xfs_dir2_leaf_t	*leaf2;			/* leaf2 structure */

	leaf1 = leaf1_bp->data;
	leaf2 = leaf2_bp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf1->hdr.info.magic) == XFS_DIR2_LEAFN_MAGIC);
	ASSERT(be16_to_cpu(leaf2->hdr.info.magic) == XFS_DIR2_LEAFN_MAGIC);
=======
	ASSERT(leaf1->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAFN_MAGIC));
	ASSERT(leaf2->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAFN_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	if (be16_to_cpu(leaf1->hdr.count) > 0 &&
	    be16_to_cpu(leaf2->hdr.count) > 0 &&
	    (be32_to_cpu(leaf2->ents[0].hashval) < be32_to_cpu(leaf1->ents[0].hashval) ||
	     be32_to_cpu(leaf2->ents[be16_to_cpu(leaf2->hdr.count) - 1].hashval) <
	     be32_to_cpu(leaf1->ents[be16_to_cpu(leaf1->hdr.count) - 1].hashval)))
=======
	struct xfs_inode	*dp,
	struct xfs_buf		*leaf1_bp,		/* leaf1 buffer */
	struct xfs_buf		*leaf2_bp)		/* leaf2 buffer */
{
	struct xfs_dir2_leaf	*leaf1 = leaf1_bp->b_addr;
	struct xfs_dir2_leaf	*leaf2 = leaf2_bp->b_addr;
	struct xfs_dir2_leaf_entry *ents1;
	struct xfs_dir2_leaf_entry *ents2;
	struct xfs_dir3_icleaf_hdr hdr1;
	struct xfs_dir3_icleaf_hdr hdr2;

	dp->d_ops->leaf_hdr_from_disk(&hdr1, leaf1);
	dp->d_ops->leaf_hdr_from_disk(&hdr2, leaf2);
	ents1 = dp->d_ops->leaf_ents_p(leaf1);
	ents2 = dp->d_ops->leaf_ents_p(leaf2);

	if (hdr1.count > 0 && hdr2.count > 0 &&
	    (be32_to_cpu(ents2[0].hashval) < be32_to_cpu(ents1[0].hashval) ||
	     be32_to_cpu(ents2[hdr2.count - 1].hashval) <
				be32_to_cpu(ents1[hdr1.count - 1].hashval)))
>>>>>>> refs/remotes/origin/master
		return 1;
	return 0;
}

/*
 * Rebalance leaf entries between two leaf blocks.
 * This is actually only called when the second block is new,
 * though the code deals with the general case.
 * A new entry will be inserted in one of the blocks, and that
 * entry is taken into account when balancing.
 */
static void
xfs_dir2_leafn_rebalance(
	xfs_da_state_t		*state,		/* btree cursor */
	xfs_da_state_blk_t	*blk1,		/* first btree block */
	xfs_da_state_blk_t	*blk2)		/* second btree block */
{
	xfs_da_args_t		*args;		/* operation arguments */
	int			count;		/* count (& direction) leaves */
	int			isleft;		/* new goes in left leaf */
	xfs_dir2_leaf_t		*leaf1;		/* first leaf structure */
	xfs_dir2_leaf_t		*leaf2;		/* second leaf structure */
	int			mid;		/* midpoint leaf index */
<<<<<<< HEAD
#ifdef DEBUG
=======
#if defined(DEBUG) || defined(XFS_WARN)
>>>>>>> refs/remotes/origin/master
	int			oldstale;	/* old count of stale leaves */
#endif
	int			oldsum;		/* old total leaf count */
	int			swap;		/* swapped leaf blocks */
<<<<<<< HEAD
=======
	struct xfs_dir2_leaf_entry *ents1;
	struct xfs_dir2_leaf_entry *ents2;
	struct xfs_dir3_icleaf_hdr hdr1;
	struct xfs_dir3_icleaf_hdr hdr2;
	struct xfs_inode	*dp = state->args->dp;
>>>>>>> refs/remotes/origin/master

	args = state->args;
	/*
	 * If the block order is wrong, swap the arguments.
	 */
<<<<<<< HEAD
	if ((swap = xfs_dir2_leafn_order(blk1->bp, blk2->bp))) {
=======
	if ((swap = xfs_dir2_leafn_order(dp, blk1->bp, blk2->bp))) {
>>>>>>> refs/remotes/origin/master
		xfs_da_state_blk_t	*tmp;	/* temp for block swap */

		tmp = blk1;
		blk1 = blk2;
		blk2 = tmp;
	}
<<<<<<< HEAD
	leaf1 = blk1->bp->data;
	leaf2 = blk2->bp->data;
	oldsum = be16_to_cpu(leaf1->hdr.count) + be16_to_cpu(leaf2->hdr.count);
#ifdef DEBUG
	oldstale = be16_to_cpu(leaf1->hdr.stale) + be16_to_cpu(leaf2->hdr.stale);
#endif
	mid = oldsum >> 1;
=======
	leaf1 = blk1->bp->b_addr;
	leaf2 = blk2->bp->b_addr;
	dp->d_ops->leaf_hdr_from_disk(&hdr1, leaf1);
	dp->d_ops->leaf_hdr_from_disk(&hdr2, leaf2);
	ents1 = dp->d_ops->leaf_ents_p(leaf1);
	ents2 = dp->d_ops->leaf_ents_p(leaf2);

	oldsum = hdr1.count + hdr2.count;
#if defined(DEBUG) || defined(XFS_WARN)
	oldstale = hdr1.stale + hdr2.stale;
#endif
	mid = oldsum >> 1;

>>>>>>> refs/remotes/origin/master
	/*
	 * If the old leaf count was odd then the new one will be even,
	 * so we need to divide the new count evenly.
	 */
	if (oldsum & 1) {
		xfs_dahash_t	midhash;	/* middle entry hash value */

<<<<<<< HEAD
		if (mid >= be16_to_cpu(leaf1->hdr.count))
			midhash = be32_to_cpu(leaf2->ents[mid - be16_to_cpu(leaf1->hdr.count)].hashval);
		else
			midhash = be32_to_cpu(leaf1->ents[mid].hashval);
=======
		if (mid >= hdr1.count)
			midhash = be32_to_cpu(ents2[mid - hdr1.count].hashval);
		else
			midhash = be32_to_cpu(ents1[mid].hashval);
>>>>>>> refs/remotes/origin/master
		isleft = args->hashval <= midhash;
	}
	/*
	 * If the old count is even then the new count is odd, so there's
	 * no preferred side for the new entry.
	 * Pick the left one.
	 */
	else
		isleft = 1;
	/*
	 * Calculate moved entry count.  Positive means left-to-right,
	 * negative means right-to-left.  Then move the entries.
	 */
<<<<<<< HEAD
	count = be16_to_cpu(leaf1->hdr.count) - mid + (isleft == 0);
	if (count > 0)
		xfs_dir2_leafn_moveents(args, blk1->bp,
			be16_to_cpu(leaf1->hdr.count) - count, blk2->bp, 0, count);
	else if (count < 0)
		xfs_dir2_leafn_moveents(args, blk2->bp, 0, blk1->bp,
			be16_to_cpu(leaf1->hdr.count), count);
	ASSERT(be16_to_cpu(leaf1->hdr.count) + be16_to_cpu(leaf2->hdr.count) == oldsum);
	ASSERT(be16_to_cpu(leaf1->hdr.stale) + be16_to_cpu(leaf2->hdr.stale) == oldstale);
	/*
	 * Mark whether we're inserting into the old or new leaf.
	 */
	if (be16_to_cpu(leaf1->hdr.count) < be16_to_cpu(leaf2->hdr.count))
		state->inleaf = swap;
	else if (be16_to_cpu(leaf1->hdr.count) > be16_to_cpu(leaf2->hdr.count))
		state->inleaf = !swap;
	else
		state->inleaf =
			swap ^ (blk1->index <= be16_to_cpu(leaf1->hdr.count));
=======
	count = hdr1.count - mid + (isleft == 0);
	if (count > 0)
		xfs_dir3_leafn_moveents(args, blk1->bp, &hdr1, ents1,
					hdr1.count - count, blk2->bp,
					&hdr2, ents2, 0, count);
	else if (count < 0)
		xfs_dir3_leafn_moveents(args, blk2->bp, &hdr2, ents2, 0,
					blk1->bp, &hdr1, ents1,
					hdr1.count, count);

	ASSERT(hdr1.count + hdr2.count == oldsum);
	ASSERT(hdr1.stale + hdr2.stale == oldstale);

	/* log the changes made when moving the entries */
	dp->d_ops->leaf_hdr_to_disk(leaf1, &hdr1);
	dp->d_ops->leaf_hdr_to_disk(leaf2, &hdr2);
	xfs_dir3_leaf_log_header(args->trans, dp, blk1->bp);
	xfs_dir3_leaf_log_header(args->trans, dp, blk2->bp);

	xfs_dir3_leaf_check(dp, blk1->bp);
	xfs_dir3_leaf_check(dp, blk2->bp);

	/*
	 * Mark whether we're inserting into the old or new leaf.
	 */
	if (hdr1.count < hdr2.count)
		state->inleaf = swap;
	else if (hdr1.count > hdr2.count)
		state->inleaf = !swap;
	else
		state->inleaf = swap ^ (blk1->index <= hdr1.count);
>>>>>>> refs/remotes/origin/master
	/*
	 * Adjust the expected index for insertion.
	 */
	if (!state->inleaf)
<<<<<<< HEAD
		blk2->index = blk1->index - be16_to_cpu(leaf1->hdr.count);
=======
		blk2->index = blk1->index - hdr1.count;
>>>>>>> refs/remotes/origin/master

	/*
	 * Finally sanity check just to make sure we are not returning a
	 * negative index
	 */
<<<<<<< HEAD
	if(blk2->index < 0) {
		state->inleaf = 1;
		blk2->index = 0;
		xfs_alert(args->dp->i_mount,
	"%s: picked the wrong leaf? reverting original leaf: blk1->index %d\n",
=======
	if (blk2->index < 0) {
		state->inleaf = 1;
		blk2->index = 0;
		xfs_alert(dp->i_mount,
	"%s: picked the wrong leaf? reverting original leaf: blk1->index %d",
>>>>>>> refs/remotes/origin/master
			__func__, blk1->index);
	}
}

<<<<<<< HEAD
=======
static int
xfs_dir3_data_block_free(
	xfs_da_args_t		*args,
	struct xfs_dir2_data_hdr *hdr,
	struct xfs_dir2_free	*free,
	xfs_dir2_db_t		fdb,
	int			findex,
	struct xfs_buf		*fbp,
	int			longest)
{
	struct xfs_trans	*tp = args->trans;
	int			logfree = 0;
	__be16			*bests;
	struct xfs_dir3_icfree_hdr freehdr;
	struct xfs_inode	*dp = args->dp;

	dp->d_ops->free_hdr_from_disk(&freehdr, free);
	bests = dp->d_ops->free_bests_p(free);
	if (hdr) {
		/*
		 * Data block is not empty, just set the free entry to the new
		 * value.
		 */
		bests[findex] = cpu_to_be16(longest);
		xfs_dir2_free_log_bests(tp, dp, fbp, findex, findex);
		return 0;
	}

	/* One less used entry in the free table. */
	freehdr.nused--;

	/*
	 * If this was the last entry in the table, we can trim the table size
	 * back.  There might be other entries at the end referring to
	 * non-existent data blocks, get those too.
	 */
	if (findex == freehdr.nvalid - 1) {
		int	i;		/* free entry index */

		for (i = findex - 1; i >= 0; i--) {
			if (bests[i] != cpu_to_be16(NULLDATAOFF))
				break;
		}
		freehdr.nvalid = i + 1;
		logfree = 0;
	} else {
		/* Not the last entry, just punch it out.  */
		bests[findex] = cpu_to_be16(NULLDATAOFF);
		logfree = 1;
	}

	dp->d_ops->free_hdr_to_disk(free, &freehdr);
	xfs_dir2_free_log_header(tp, dp, fbp);

	/*
	 * If there are no useful entries left in the block, get rid of the
	 * block if we can.
	 */
	if (!freehdr.nused) {
		int error;

		error = xfs_dir2_shrink_inode(args, fdb, fbp);
		if (error == 0) {
			fbp = NULL;
			logfree = 0;
		} else if (error != ENOSPC || args->total != 0)
			return error;
		/*
		 * It's possible to get ENOSPC if there is no
		 * space reservation.  In this case some one
		 * else will eventually get rid of this block.
		 */
	}

	/* Log the free entry that changed, unless we got rid of it.  */
	if (logfree)
		xfs_dir2_free_log_bests(tp, dp, fbp, findex, findex);
	return 0;
}

>>>>>>> refs/remotes/origin/master
/*
 * Remove an entry from a node directory.
 * This removes the leaf entry and the data entry,
 * and updates the free block if necessary.
 */
static int					/* error */
xfs_dir2_leafn_remove(
	xfs_da_args_t		*args,		/* operation arguments */
<<<<<<< HEAD
	xfs_dabuf_t		*bp,		/* leaf buffer */
=======
	struct xfs_buf		*bp,		/* leaf buffer */
>>>>>>> refs/remotes/origin/master
	int			index,		/* leaf entry index */
	xfs_da_state_blk_t	*dblk,		/* data block */
	int			*rval)		/* resulting block needs join */
{
<<<<<<< HEAD
<<<<<<< HEAD
	xfs_dir2_data_t		*data;		/* data block structure */
=======
	xfs_dir2_data_hdr_t	*hdr;		/* data block header */
>>>>>>> refs/remotes/origin/cm-10.0
	xfs_dir2_db_t		db;		/* data block number */
	xfs_dabuf_t		*dbp;		/* data block buffer */
=======
	xfs_dir2_data_hdr_t	*hdr;		/* data block header */
	xfs_dir2_db_t		db;		/* data block number */
	struct xfs_buf		*dbp;		/* data block buffer */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_data_entry_t	*dep;		/* data block entry */
	xfs_inode_t		*dp;		/* incore directory inode */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	xfs_dir2_leaf_entry_t	*lep;		/* leaf entry */
	int			longest;	/* longest data free entry */
	int			off;		/* data block entry offset */
	xfs_mount_t		*mp;		/* filesystem mount point */
	int			needlog;	/* need to log data header */
	int			needscan;	/* need to rescan data frees */
	xfs_trans_t		*tp;		/* transaction pointer */
<<<<<<< HEAD
=======
	struct xfs_dir2_data_free *bf;		/* bestfree table */
	struct xfs_dir3_icleaf_hdr leafhdr;
	struct xfs_dir2_leaf_entry *ents;
>>>>>>> refs/remotes/origin/master

	trace_xfs_dir2_leafn_remove(args, index);

	dp = args->dp;
	tp = args->trans;
	mp = dp->i_mount;
<<<<<<< HEAD
	leaf = bp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_DIR2_LEAFN_MAGIC);
=======
	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAFN_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * Point to the entry we're removing.
	 */
	lep = &leaf->ents[index];
=======
	leaf = bp->b_addr;
	dp->d_ops->leaf_hdr_from_disk(&leafhdr, leaf);
	ents = dp->d_ops->leaf_ents_p(leaf);

	/*
	 * Point to the entry we're removing.
	 */
	lep = &ents[index];

>>>>>>> refs/remotes/origin/master
	/*
	 * Extract the data block and offset from the entry.
	 */
	db = xfs_dir2_dataptr_to_db(mp, be32_to_cpu(lep->address));
	ASSERT(dblk->blkno == db);
	off = xfs_dir2_dataptr_to_off(mp, be32_to_cpu(lep->address));
	ASSERT(dblk->index == off);
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
	/*
	 * Kill the leaf entry by marking it stale.
	 * Log the leaf block changes.
	 */
<<<<<<< HEAD
	be16_add_cpu(&leaf->hdr.stale, 1);
	xfs_dir2_leaf_log_header(tp, bp);
	lep->address = cpu_to_be32(XFS_DIR2_NULL_DATAPTR);
	xfs_dir2_leaf_log_ents(tp, bp, index, index);
=======
	leafhdr.stale++;
	dp->d_ops->leaf_hdr_to_disk(leaf, &leafhdr);
	xfs_dir3_leaf_log_header(tp, dp, bp);

	lep->address = cpu_to_be32(XFS_DIR2_NULL_DATAPTR);
	xfs_dir3_leaf_log_ents(tp, dp, bp, index, index);

>>>>>>> refs/remotes/origin/master
	/*
	 * Make the data entry free.  Keep track of the longest freespace
	 * in the data block in case it changes.
	 */
	dbp = dblk->bp;
<<<<<<< HEAD
<<<<<<< HEAD
	data = dbp->data;
	dep = (xfs_dir2_data_entry_t *)((char *)data + off);
	longest = be16_to_cpu(data->hdr.bestfree[0].length);
=======
	hdr = dbp->data;
	dep = (xfs_dir2_data_entry_t *)((char *)hdr + off);
	longest = be16_to_cpu(hdr->bestfree[0].length);
>>>>>>> refs/remotes/origin/cm-10.0
	needlog = needscan = 0;
	xfs_dir2_data_make_free(tp, dbp, off,
		xfs_dir2_data_entsize(dep->namelen), &needlog, &needscan);
=======
	hdr = dbp->b_addr;
	dep = (xfs_dir2_data_entry_t *)((char *)hdr + off);
	bf = dp->d_ops->data_bestfree_p(hdr);
	longest = be16_to_cpu(bf[0].length);
	needlog = needscan = 0;
	xfs_dir2_data_make_free(tp, dp, dbp, off,
		dp->d_ops->data_entsize(dep->namelen), &needlog, &needscan);
>>>>>>> refs/remotes/origin/master
	/*
	 * Rescan the data block freespaces for bestfree.
	 * Log the data block header if needed.
	 */
	if (needscan)
<<<<<<< HEAD
<<<<<<< HEAD
		xfs_dir2_data_freescan(mp, data, &needlog);
=======
		xfs_dir2_data_freescan(mp, hdr, &needlog);
>>>>>>> refs/remotes/origin/cm-10.0
	if (needlog)
		xfs_dir2_data_log_header(tp, dbp);
	xfs_dir2_data_check(dp, dbp);
=======
		xfs_dir2_data_freescan(dp, hdr, &needlog);
	if (needlog)
		xfs_dir2_data_log_header(tp, dp, dbp);
	xfs_dir3_data_check(dp, dbp);
>>>>>>> refs/remotes/origin/master
	/*
	 * If the longest data block freespace changes, need to update
	 * the corresponding freeblock entry.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (longest < be16_to_cpu(data->hdr.bestfree[0].length)) {
=======
	if (longest < be16_to_cpu(hdr->bestfree[0].length)) {
>>>>>>> refs/remotes/origin/cm-10.0
		int		error;		/* error return value */
		xfs_dabuf_t	*fbp;		/* freeblock buffer */
		xfs_dir2_db_t	fdb;		/* freeblock block number */
		int		findex;		/* index in freeblock entries */
		xfs_dir2_free_t	*free;		/* freeblock structure */
		int		logfree;	/* need to log free entry */
=======
	if (longest < be16_to_cpu(bf[0].length)) {
		int		error;		/* error return value */
		struct xfs_buf	*fbp;		/* freeblock buffer */
		xfs_dir2_db_t	fdb;		/* freeblock block number */
		int		findex;		/* index in freeblock entries */
		xfs_dir2_free_t	*free;		/* freeblock structure */
>>>>>>> refs/remotes/origin/master

		/*
		 * Convert the data block number to a free block,
		 * read in the free block.
		 */
<<<<<<< HEAD
		fdb = xfs_dir2_db_to_fdb(mp, db);
		if ((error = xfs_da_read_buf(tp, dp, xfs_dir2_db_to_da(mp, fdb),
				-1, &fbp, XFS_DATA_FORK))) {
			return error;
		}
		free = fbp->data;
<<<<<<< HEAD
		ASSERT(be32_to_cpu(free->hdr.magic) == XFS_DIR2_FREE_MAGIC);
		ASSERT(be32_to_cpu(free->hdr.firstdb) ==
		       XFS_DIR2_MAX_FREE_BESTS(mp) *
=======
		ASSERT(free->hdr.magic == cpu_to_be32(XFS_DIR2_FREE_MAGIC));
		ASSERT(be32_to_cpu(free->hdr.firstdb) ==
		       xfs_dir2_free_max_bests(mp) *
>>>>>>> refs/remotes/origin/cm-10.0
		       (fdb - XFS_DIR2_FREE_FIRSTDB(mp)));
		/*
		 * Calculate which entry we need to fix.
		 */
		findex = xfs_dir2_db_to_fdindex(mp, db);
<<<<<<< HEAD
		longest = be16_to_cpu(data->hdr.bestfree[0].length);
=======
		longest = be16_to_cpu(hdr->bestfree[0].length);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		fdb = dp->d_ops->db_to_fdb(mp, db);
		error = xfs_dir2_free_read(tp, dp, xfs_dir2_db_to_da(mp, fdb),
					   &fbp);
		if (error)
			return error;
		free = fbp->b_addr;
#ifdef DEBUG
	{
		struct xfs_dir3_icfree_hdr freehdr;
		dp->d_ops->free_hdr_from_disk(&freehdr, free);
		ASSERT(freehdr.firstdb == dp->d_ops->free_max_bests(mp) *
					  (fdb - XFS_DIR2_FREE_FIRSTDB(mp)));
	}
#endif
		/*
		 * Calculate which entry we need to fix.
		 */
		findex = dp->d_ops->db_to_fdindex(mp, db);
		longest = be16_to_cpu(bf[0].length);
>>>>>>> refs/remotes/origin/master
		/*
		 * If the data block is now empty we can get rid of it
		 * (usually).
		 */
<<<<<<< HEAD
<<<<<<< HEAD
		if (longest == mp->m_dirblksize - (uint)sizeof(data->hdr)) {
=======
		if (longest == mp->m_dirblksize - (uint)sizeof(*hdr)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (longest == mp->m_dirblksize -
			       dp->d_ops->data_entry_offset) {
>>>>>>> refs/remotes/origin/master
			/*
			 * Try to punch out the data block.
			 */
			error = xfs_dir2_shrink_inode(args, db, dbp);
			if (error == 0) {
				dblk->bp = NULL;
<<<<<<< HEAD
<<<<<<< HEAD
				data = NULL;
=======
				hdr = NULL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
				hdr = NULL;
>>>>>>> refs/remotes/origin/master
			}
			/*
			 * We can get ENOSPC if there's no space reservation.
			 * In this case just drop the buffer and some one else
			 * will eventually get rid of the empty block.
			 */
<<<<<<< HEAD
			else if (error == ENOSPC && args->total == 0)
				xfs_da_buf_done(dbp);
			else
=======
			else if (!(error == ENOSPC && args->total == 0))
>>>>>>> refs/remotes/origin/master
				return error;
		}
		/*
		 * If we got rid of the data block, we can eliminate that entry
		 * in the free block.
		 */
<<<<<<< HEAD
<<<<<<< HEAD
		if (data == NULL) {
=======
		if (hdr == NULL) {
>>>>>>> refs/remotes/origin/cm-10.0
			/*
			 * One less used entry in the free table.
			 */
			be32_add_cpu(&free->hdr.nused, -1);
			xfs_dir2_free_log_header(tp, fbp);
			/*
			 * If this was the last entry in the table, we can
			 * trim the table size back.  There might be other
			 * entries at the end referring to non-existent
			 * data blocks, get those too.
			 */
			if (findex == be32_to_cpu(free->hdr.nvalid) - 1) {
				int	i;		/* free entry index */

				for (i = findex - 1;
<<<<<<< HEAD
				     i >= 0 && be16_to_cpu(free->bests[i]) == NULLDATAOFF;
=======
				     i >= 0 &&
				     free->bests[i] == cpu_to_be16(NULLDATAOFF);
>>>>>>> refs/remotes/origin/cm-10.0
				     i--)
					continue;
				free->hdr.nvalid = cpu_to_be32(i + 1);
				logfree = 0;
			}
			/*
			 * Not the last entry, just punch it out.
			 */
			else {
				free->bests[findex] = cpu_to_be16(NULLDATAOFF);
				logfree = 1;
			}
			/*
			 * If there are no useful entries left in the block,
			 * get rid of the block if we can.
			 */
			if (!free->hdr.nused) {
				error = xfs_dir2_shrink_inode(args, fdb, fbp);
				if (error == 0) {
					fbp = NULL;
					logfree = 0;
				} else if (error != ENOSPC || args->total != 0)
					return error;
				/*
				 * It's possible to get ENOSPC if there is no
				 * space reservation.  In this case some one
				 * else will eventually get rid of this block.
				 */
			}
		}
		/*
		 * Data block is not empty, just set the free entry to
		 * the new value.
		 */
		else {
			free->bests[findex] = cpu_to_be16(longest);
			logfree = 1;
		}
		/*
		 * Log the free entry that changed, unless we got rid of it.
		 */
		if (logfree)
			xfs_dir2_free_log_bests(tp, fbp, findex, findex);
		/*
		 * Drop the buffer if we still have it.
		 */
		if (fbp)
			xfs_da_buf_done(fbp);
	}
	xfs_dir2_leafn_check(dp, bp);
=======
		error = xfs_dir3_data_block_free(args, hdr, free,
						 fdb, findex, fbp, longest);
		if (error)
			return error;
	}

	xfs_dir3_leaf_check(dp, bp);
>>>>>>> refs/remotes/origin/master
	/*
	 * Return indication of whether this leaf block is empty enough
	 * to justify trying to join it with a neighbor.
	 */
<<<<<<< HEAD
	*rval =
		((uint)sizeof(leaf->hdr) +
		 (uint)sizeof(leaf->ents[0]) *
		 (be16_to_cpu(leaf->hdr.count) - be16_to_cpu(leaf->hdr.stale))) <
=======
	*rval = (dp->d_ops->leaf_hdr_size +
		 (uint)sizeof(ents[0]) * (leafhdr.count - leafhdr.stale)) <
>>>>>>> refs/remotes/origin/master
		mp->m_dir_magicpct;
	return 0;
}

/*
 * Split the leaf entries in the old block into old and new blocks.
 */
int						/* error */
xfs_dir2_leafn_split(
	xfs_da_state_t		*state,		/* btree cursor */
	xfs_da_state_blk_t	*oldblk,	/* original block */
	xfs_da_state_blk_t	*newblk)	/* newly created block */
{
	xfs_da_args_t		*args;		/* operation arguments */
	xfs_dablk_t		blkno;		/* new leaf block number */
	int			error;		/* error return value */
	xfs_mount_t		*mp;		/* filesystem mount point */
<<<<<<< HEAD
=======
	struct xfs_inode	*dp;
>>>>>>> refs/remotes/origin/master

	/*
	 * Allocate space for a new leaf node.
	 */
	args = state->args;
<<<<<<< HEAD
	mp = args->dp->i_mount;
	ASSERT(args != NULL);
=======
	dp = args->dp;
	mp = dp->i_mount;
>>>>>>> refs/remotes/origin/master
	ASSERT(oldblk->magic == XFS_DIR2_LEAFN_MAGIC);
	error = xfs_da_grow_inode(args, &blkno);
	if (error) {
		return error;
	}
	/*
	 * Initialize the new leaf block.
	 */
<<<<<<< HEAD
	error = xfs_dir2_leaf_init(args, xfs_dir2_da_to_db(mp, blkno),
		&newblk->bp, XFS_DIR2_LEAFN_MAGIC);
	if (error) {
		return error;
	}
=======
	error = xfs_dir3_leaf_get_buf(args, xfs_dir2_da_to_db(mp, blkno),
				      &newblk->bp, XFS_DIR2_LEAFN_MAGIC);
	if (error)
		return error;

>>>>>>> refs/remotes/origin/master
	newblk->blkno = blkno;
	newblk->magic = XFS_DIR2_LEAFN_MAGIC;
	/*
	 * Rebalance the entries across the two leaves, link the new
	 * block into the leaves.
	 */
	xfs_dir2_leafn_rebalance(state, oldblk, newblk);
<<<<<<< HEAD
	error = xfs_da_blk_link(state, oldblk, newblk);
=======
	error = xfs_da3_blk_link(state, oldblk, newblk);
>>>>>>> refs/remotes/origin/master
	if (error) {
		return error;
	}
	/*
	 * Insert the new entry in the correct block.
	 */
	if (state->inleaf)
		error = xfs_dir2_leafn_add(oldblk->bp, args, oldblk->index);
	else
		error = xfs_dir2_leafn_add(newblk->bp, args, newblk->index);
	/*
	 * Update last hashval in each block since we added the name.
	 */
<<<<<<< HEAD
	oldblk->hashval = xfs_dir2_leafn_lasthash(oldblk->bp, NULL);
	newblk->hashval = xfs_dir2_leafn_lasthash(newblk->bp, NULL);
	xfs_dir2_leafn_check(args->dp, oldblk->bp);
	xfs_dir2_leafn_check(args->dp, newblk->bp);
=======
	oldblk->hashval = xfs_dir2_leafn_lasthash(dp, oldblk->bp, NULL);
	newblk->hashval = xfs_dir2_leafn_lasthash(dp, newblk->bp, NULL);
	xfs_dir3_leaf_check(dp, oldblk->bp);
	xfs_dir3_leaf_check(dp, newblk->bp);
>>>>>>> refs/remotes/origin/master
	return error;
}

/*
 * Check a leaf block and its neighbors to see if the block should be
 * collapsed into one or the other neighbor.  Always keep the block
 * with the smaller block number.
 * If the current block is over 50% full, don't try to join it, return 0.
 * If the block is empty, fill in the state structure and return 2.
 * If it can be collapsed, fill in the state structure and return 1.
 * If nothing can be done, return 0.
 */
int						/* error */
xfs_dir2_leafn_toosmall(
	xfs_da_state_t		*state,		/* btree cursor */
	int			*action)	/* resulting action to take */
{
	xfs_da_state_blk_t	*blk;		/* leaf block */
	xfs_dablk_t		blkno;		/* leaf block number */
<<<<<<< HEAD
	xfs_dabuf_t		*bp;		/* leaf buffer */
=======
	struct xfs_buf		*bp;		/* leaf buffer */
>>>>>>> refs/remotes/origin/master
	int			bytes;		/* bytes in use */
	int			count;		/* leaf live entry count */
	int			error;		/* error return value */
	int			forward;	/* sibling block direction */
	int			i;		/* sibling counter */
<<<<<<< HEAD
	xfs_da_blkinfo_t	*info;		/* leaf block header */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	int			rval;		/* result from path_shift */
=======
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	int			rval;		/* result from path_shift */
	struct xfs_dir3_icleaf_hdr leafhdr;
	struct xfs_dir2_leaf_entry *ents;
	struct xfs_inode	*dp = state->args->dp;
>>>>>>> refs/remotes/origin/master

	/*
	 * Check for the degenerate case of the block being over 50% full.
	 * If so, it's not worth even looking to see if we might be able
	 * to coalesce with a sibling.
	 */
	blk = &state->path.blk[state->path.active - 1];
<<<<<<< HEAD
	info = blk->bp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(info->magic) == XFS_DIR2_LEAFN_MAGIC);
=======
	ASSERT(info->magic == cpu_to_be16(XFS_DIR2_LEAFN_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	leaf = (xfs_dir2_leaf_t *)info;
	count = be16_to_cpu(leaf->hdr.count) - be16_to_cpu(leaf->hdr.stale);
	bytes = (uint)sizeof(leaf->hdr) + count * (uint)sizeof(leaf->ents[0]);
=======
	leaf = blk->bp->b_addr;
	dp->d_ops->leaf_hdr_from_disk(&leafhdr, leaf);
	ents = dp->d_ops->leaf_ents_p(leaf);
	xfs_dir3_leaf_check(dp, blk->bp);

	count = leafhdr.count - leafhdr.stale;
	bytes = dp->d_ops->leaf_hdr_size + count * sizeof(ents[0]);
>>>>>>> refs/remotes/origin/master
	if (bytes > (state->blocksize >> 1)) {
		/*
		 * Blk over 50%, don't try to join.
		 */
		*action = 0;
		return 0;
	}
	/*
	 * Check for the degenerate case of the block being empty.
	 * If the block is empty, we'll simply delete it, no need to
	 * coalesce it with a sibling block.  We choose (arbitrarily)
	 * to merge with the forward block unless it is NULL.
	 */
	if (count == 0) {
		/*
		 * Make altpath point to the block we want to keep and
		 * path point to the block we want to drop (this one).
		 */
<<<<<<< HEAD
		forward = (info->forw != 0);
		memcpy(&state->altpath, &state->path, sizeof(state->path));
		error = xfs_da_path_shift(state, &state->altpath, forward, 0,
=======
		forward = (leafhdr.forw != 0);
		memcpy(&state->altpath, &state->path, sizeof(state->path));
		error = xfs_da3_path_shift(state, &state->altpath, forward, 0,
>>>>>>> refs/remotes/origin/master
			&rval);
		if (error)
			return error;
		*action = rval ? 2 : 0;
		return 0;
	}
	/*
	 * Examine each sibling block to see if we can coalesce with
	 * at least 25% free space to spare.  We need to figure out
	 * whether to merge with the forward or the backward block.
	 * We prefer coalescing with the lower numbered sibling so as
	 * to shrink a directory over time.
	 */
<<<<<<< HEAD
	forward = be32_to_cpu(info->forw) < be32_to_cpu(info->back);
	for (i = 0, bp = NULL; i < 2; forward = !forward, i++) {
		blkno = forward ? be32_to_cpu(info->forw) : be32_to_cpu(info->back);
=======
	forward = leafhdr.forw < leafhdr.back;
	for (i = 0, bp = NULL; i < 2; forward = !forward, i++) {
		struct xfs_dir3_icleaf_hdr hdr2;

		blkno = forward ? leafhdr.forw : leafhdr.back;
>>>>>>> refs/remotes/origin/master
		if (blkno == 0)
			continue;
		/*
		 * Read the sibling leaf block.
		 */
<<<<<<< HEAD
		if ((error =
		    xfs_da_read_buf(state->args->trans, state->args->dp, blkno,
			    -1, &bp, XFS_DATA_FORK))) {
			return error;
		}
		ASSERT(bp != NULL);
		/*
		 * Count bytes in the two blocks combined.
		 */
		leaf = (xfs_dir2_leaf_t *)info;
		count = be16_to_cpu(leaf->hdr.count) - be16_to_cpu(leaf->hdr.stale);
		bytes = state->blocksize - (state->blocksize >> 2);
		leaf = bp->data;
<<<<<<< HEAD
		ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_DIR2_LEAFN_MAGIC);
=======
		ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAFN_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
		count += be16_to_cpu(leaf->hdr.count) - be16_to_cpu(leaf->hdr.stale);
		bytes -= count * (uint)sizeof(leaf->ents[0]);
=======
		error = xfs_dir3_leafn_read(state->args->trans, dp,
					    blkno, -1, &bp);
		if (error)
			return error;

		/*
		 * Count bytes in the two blocks combined.
		 */
		count = leafhdr.count - leafhdr.stale;
		bytes = state->blocksize - (state->blocksize >> 2);

		leaf = bp->b_addr;
		dp->d_ops->leaf_hdr_from_disk(&hdr2, leaf);
		ents = dp->d_ops->leaf_ents_p(leaf);
		count += hdr2.count - hdr2.stale;
		bytes -= count * sizeof(ents[0]);

>>>>>>> refs/remotes/origin/master
		/*
		 * Fits with at least 25% to spare.
		 */
		if (bytes >= 0)
			break;
<<<<<<< HEAD
		xfs_da_brelse(state->args->trans, bp);
=======
		xfs_trans_brelse(state->args->trans, bp);
>>>>>>> refs/remotes/origin/master
	}
	/*
	 * Didn't like either block, give up.
	 */
	if (i >= 2) {
		*action = 0;
		return 0;
	}
<<<<<<< HEAD
	/*
	 * Done with the sibling leaf block here, drop the dabuf
	 * so path_shift can get it.
	 */
	xfs_da_buf_done(bp);
=======

>>>>>>> refs/remotes/origin/master
	/*
	 * Make altpath point to the block we want to keep (the lower
	 * numbered block) and path point to the block we want to drop.
	 */
	memcpy(&state->altpath, &state->path, sizeof(state->path));
	if (blkno < blk->blkno)
<<<<<<< HEAD
		error = xfs_da_path_shift(state, &state->altpath, forward, 0,
			&rval);
	else
		error = xfs_da_path_shift(state, &state->path, forward, 0,
=======
		error = xfs_da3_path_shift(state, &state->altpath, forward, 0,
			&rval);
	else
		error = xfs_da3_path_shift(state, &state->path, forward, 0,
>>>>>>> refs/remotes/origin/master
			&rval);
	if (error) {
		return error;
	}
	*action = rval ? 0 : 1;
	return 0;
}

/*
 * Move all the leaf entries from drop_blk to save_blk.
 * This is done as part of a join operation.
 */
void
xfs_dir2_leafn_unbalance(
	xfs_da_state_t		*state,		/* cursor */
	xfs_da_state_blk_t	*drop_blk,	/* dead block */
	xfs_da_state_blk_t	*save_blk)	/* surviving block */
{
	xfs_da_args_t		*args;		/* operation arguments */
	xfs_dir2_leaf_t		*drop_leaf;	/* dead leaf structure */
	xfs_dir2_leaf_t		*save_leaf;	/* surviving leaf structure */
<<<<<<< HEAD
=======
	struct xfs_dir3_icleaf_hdr savehdr;
	struct xfs_dir3_icleaf_hdr drophdr;
	struct xfs_dir2_leaf_entry *sents;
	struct xfs_dir2_leaf_entry *dents;
	struct xfs_inode	*dp = state->args->dp;
>>>>>>> refs/remotes/origin/master

	args = state->args;
	ASSERT(drop_blk->magic == XFS_DIR2_LEAFN_MAGIC);
	ASSERT(save_blk->magic == XFS_DIR2_LEAFN_MAGIC);
<<<<<<< HEAD
	drop_leaf = drop_blk->bp->data;
	save_leaf = save_blk->bp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(drop_leaf->hdr.info.magic) == XFS_DIR2_LEAFN_MAGIC);
	ASSERT(be16_to_cpu(save_leaf->hdr.info.magic) == XFS_DIR2_LEAFN_MAGIC);
=======
	ASSERT(drop_leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAFN_MAGIC));
	ASSERT(save_leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAFN_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	drop_leaf = drop_blk->bp->b_addr;
	save_leaf = save_blk->bp->b_addr;

	dp->d_ops->leaf_hdr_from_disk(&savehdr, save_leaf);
	dp->d_ops->leaf_hdr_from_disk(&drophdr, drop_leaf);
	sents = dp->d_ops->leaf_ents_p(save_leaf);
	dents = dp->d_ops->leaf_ents_p(drop_leaf);

>>>>>>> refs/remotes/origin/master
	/*
	 * If there are any stale leaf entries, take this opportunity
	 * to purge them.
	 */
<<<<<<< HEAD
	if (drop_leaf->hdr.stale)
		xfs_dir2_leaf_compact(args, drop_blk->bp);
	if (save_leaf->hdr.stale)
		xfs_dir2_leaf_compact(args, save_blk->bp);
	/*
	 * Move the entries from drop to the appropriate end of save.
	 */
	drop_blk->hashval = be32_to_cpu(drop_leaf->ents[be16_to_cpu(drop_leaf->hdr.count) - 1].hashval);
	if (xfs_dir2_leafn_order(save_blk->bp, drop_blk->bp))
		xfs_dir2_leafn_moveents(args, drop_blk->bp, 0, save_blk->bp, 0,
			be16_to_cpu(drop_leaf->hdr.count));
	else
		xfs_dir2_leafn_moveents(args, drop_blk->bp, 0, save_blk->bp,
			be16_to_cpu(save_leaf->hdr.count), be16_to_cpu(drop_leaf->hdr.count));
	save_blk->hashval = be32_to_cpu(save_leaf->ents[be16_to_cpu(save_leaf->hdr.count) - 1].hashval);
	xfs_dir2_leafn_check(args->dp, save_blk->bp);
=======
	if (drophdr.stale)
		xfs_dir3_leaf_compact(args, &drophdr, drop_blk->bp);
	if (savehdr.stale)
		xfs_dir3_leaf_compact(args, &savehdr, save_blk->bp);

	/*
	 * Move the entries from drop to the appropriate end of save.
	 */
	drop_blk->hashval = be32_to_cpu(dents[drophdr.count - 1].hashval);
	if (xfs_dir2_leafn_order(dp, save_blk->bp, drop_blk->bp))
		xfs_dir3_leafn_moveents(args, drop_blk->bp, &drophdr, dents, 0,
					save_blk->bp, &savehdr, sents, 0,
					drophdr.count);
	else
		xfs_dir3_leafn_moveents(args, drop_blk->bp, &drophdr, dents, 0,
					save_blk->bp, &savehdr, sents,
					savehdr.count, drophdr.count);
	save_blk->hashval = be32_to_cpu(sents[savehdr.count - 1].hashval);

	/* log the changes made when moving the entries */
	dp->d_ops->leaf_hdr_to_disk(save_leaf, &savehdr);
	dp->d_ops->leaf_hdr_to_disk(drop_leaf, &drophdr);
	xfs_dir3_leaf_log_header(args->trans, dp, save_blk->bp);
	xfs_dir3_leaf_log_header(args->trans, dp, drop_blk->bp);

	xfs_dir3_leaf_check(dp, save_blk->bp);
	xfs_dir3_leaf_check(dp, drop_blk->bp);
>>>>>>> refs/remotes/origin/master
}

/*
 * Top-level node form directory addname routine.
 */
int						/* error */
xfs_dir2_node_addname(
	xfs_da_args_t		*args)		/* operation arguments */
{
	xfs_da_state_blk_t	*blk;		/* leaf block for insert */
	int			error;		/* error return value */
	int			rval;		/* sub-return value */
	xfs_da_state_t		*state;		/* btree cursor */

	trace_xfs_dir2_node_addname(args);

	/*
	 * Allocate and initialize the state (btree cursor).
	 */
	state = xfs_da_state_alloc();
	state->args = args;
	state->mp = args->dp->i_mount;
	state->blocksize = state->mp->m_dirblksize;
	state->node_ents = state->mp->m_dir_node_ents;
	/*
	 * Look up the name.  We're not supposed to find it, but
	 * this gives us the insertion point.
	 */
<<<<<<< HEAD
	error = xfs_da_node_lookup_int(state, &rval);
=======
	error = xfs_da3_node_lookup_int(state, &rval);
>>>>>>> refs/remotes/origin/master
	if (error)
		rval = error;
	if (rval != ENOENT) {
		goto done;
	}
	/*
	 * Add the data entry to a data block.
	 * Extravalid is set to a freeblock found by lookup.
	 */
	rval = xfs_dir2_node_addname_int(args,
		state->extravalid ? &state->extrablk : NULL);
	if (rval) {
		goto done;
	}
	blk = &state->path.blk[state->path.active - 1];
	ASSERT(blk->magic == XFS_DIR2_LEAFN_MAGIC);
	/*
	 * Add the new leaf entry.
	 */
	rval = xfs_dir2_leafn_add(blk->bp, args, blk->index);
	if (rval == 0) {
		/*
		 * It worked, fix the hash values up the btree.
		 */
		if (!(args->op_flags & XFS_DA_OP_JUSTCHECK))
<<<<<<< HEAD
			xfs_da_fixhashpath(state, &state->path);
=======
			xfs_da3_fixhashpath(state, &state->path);
>>>>>>> refs/remotes/origin/master
	} else {
		/*
		 * It didn't work, we need to split the leaf block.
		 */
		if (args->total == 0) {
			ASSERT(rval == ENOSPC);
			goto done;
		}
		/*
		 * Split the leaf block and insert the new entry.
		 */
<<<<<<< HEAD
		rval = xfs_da_split(state);
=======
		rval = xfs_da3_split(state);
>>>>>>> refs/remotes/origin/master
	}
done:
	xfs_da_state_free(state);
	return rval;
}

/*
 * Add the data entry for a node-format directory name addition.
 * The leaf entry is added in xfs_dir2_leafn_add.
 * We may enter with a freespace block that the lookup found.
 */
static int					/* error */
xfs_dir2_node_addname_int(
	xfs_da_args_t		*args,		/* operation arguments */
	xfs_da_state_blk_t	*fblk)		/* optional freespace block */
{
<<<<<<< HEAD
<<<<<<< HEAD
	xfs_dir2_data_t		*data;		/* data block structure */
=======
	xfs_dir2_data_hdr_t	*hdr;		/* data block header */
>>>>>>> refs/remotes/origin/cm-10.0
	xfs_dir2_db_t		dbno;		/* data block number */
	xfs_dabuf_t		*dbp;		/* data block buffer */
=======
	xfs_dir2_data_hdr_t	*hdr;		/* data block header */
	xfs_dir2_db_t		dbno;		/* data block number */
	struct xfs_buf		*dbp;		/* data block buffer */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_data_entry_t	*dep;		/* data entry pointer */
	xfs_inode_t		*dp;		/* incore directory inode */
	xfs_dir2_data_unused_t	*dup;		/* data unused entry pointer */
	int			error;		/* error return value */
	xfs_dir2_db_t		fbno;		/* freespace block number */
<<<<<<< HEAD
	xfs_dabuf_t		*fbp;		/* freespace buffer */
=======
	struct xfs_buf		*fbp;		/* freespace buffer */
>>>>>>> refs/remotes/origin/master
	int			findex;		/* freespace entry index */
	xfs_dir2_free_t		*free=NULL;	/* freespace block structure */
	xfs_dir2_db_t		ifbno;		/* initial freespace block no */
	xfs_dir2_db_t		lastfbno=0;	/* highest freespace block no */
	int			length;		/* length of the new entry */
	int			logfree;	/* need to log free entry */
	xfs_mount_t		*mp;		/* filesystem mount point */
	int			needlog;	/* need to log data header */
	int			needscan;	/* need to rescan data frees */
	__be16			*tagp;		/* data entry tag pointer */
	xfs_trans_t		*tp;		/* transaction pointer */
<<<<<<< HEAD
=======
	__be16			*bests;
	struct xfs_dir3_icfree_hdr freehdr;
	struct xfs_dir2_data_free *bf;
>>>>>>> refs/remotes/origin/master

	dp = args->dp;
	mp = dp->i_mount;
	tp = args->trans;
<<<<<<< HEAD
	length = xfs_dir2_data_entsize(args->namelen);
=======
	length = dp->d_ops->data_entsize(args->namelen);
>>>>>>> refs/remotes/origin/master
	/*
	 * If we came in with a freespace block that means that lookup
	 * found an entry with our hash value.  This is the freespace
	 * block for that data entry.
	 */
	if (fblk) {
		fbp = fblk->bp;
		/*
		 * Remember initial freespace block number.
		 */
		ifbno = fblk->blkno;
<<<<<<< HEAD
		free = fbp->data;
<<<<<<< HEAD
		ASSERT(be32_to_cpu(free->hdr.magic) == XFS_DIR2_FREE_MAGIC);
=======
		ASSERT(free->hdr.magic == cpu_to_be32(XFS_DIR2_FREE_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
		findex = fblk->index;
=======
		free = fbp->b_addr;
		findex = fblk->index;
		bests = dp->d_ops->free_bests_p(free);
		dp->d_ops->free_hdr_from_disk(&freehdr, free);

>>>>>>> refs/remotes/origin/master
		/*
		 * This means the free entry showed that the data block had
		 * space for our entry, so we remembered it.
		 * Use that data block.
		 */
		if (findex >= 0) {
<<<<<<< HEAD
			ASSERT(findex < be32_to_cpu(free->hdr.nvalid));
			ASSERT(be16_to_cpu(free->bests[findex]) != NULLDATAOFF);
			ASSERT(be16_to_cpu(free->bests[findex]) >= length);
			dbno = be32_to_cpu(free->hdr.firstdb) + findex;
		}
		/*
		 * The data block looked at didn't have enough room.
		 * We'll start at the beginning of the freespace entries.
		 */
		else {
			dbno = -1;
			findex = 0;
		}
	}
	/*
	 * Didn't come in with a freespace block, so don't have a data block.
	 */
	else {
=======
			ASSERT(findex < freehdr.nvalid);
			ASSERT(be16_to_cpu(bests[findex]) != NULLDATAOFF);
			ASSERT(be16_to_cpu(bests[findex]) >= length);
			dbno = freehdr.firstdb + findex;
		} else {
			/*
			 * The data block looked at didn't have enough room.
			 * We'll start at the beginning of the freespace entries.
			 */
			dbno = -1;
			findex = 0;
		}
	} else {
		/*
		 * Didn't come in with a freespace block, so no data block.
		 */
>>>>>>> refs/remotes/origin/master
		ifbno = dbno = -1;
		fbp = NULL;
		findex = 0;
	}
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
	/*
	 * If we don't have a data block yet, we're going to scan the
	 * freespace blocks looking for one.  Figure out what the
	 * highest freespace block number is.
	 */
	if (dbno == -1) {
		xfs_fileoff_t	fo;		/* freespace block number */

		if ((error = xfs_bmap_last_offset(tp, dp, &fo, XFS_DATA_FORK)))
			return error;
		lastfbno = xfs_dir2_da_to_db(mp, (xfs_dablk_t)fo);
		fbno = ifbno;
	}
	/*
	 * While we haven't identified a data block, search the freeblock
	 * data for a good data block.  If we find a null freeblock entry,
	 * indicating a hole in the data blocks, remember that.
	 */
	while (dbno == -1) {
		/*
		 * If we don't have a freeblock in hand, get the next one.
		 */
		if (fbp == NULL) {
			/*
			 * Happens the first time through unless lookup gave
			 * us a freespace block to start with.
			 */
			if (++fbno == 0)
				fbno = XFS_DIR2_FREE_FIRSTDB(mp);
			/*
			 * If it's ifbno we already looked at it.
			 */
			if (fbno == ifbno)
				fbno++;
			/*
			 * If it's off the end we're done.
			 */
			if (fbno >= lastfbno)
				break;
			/*
			 * Read the block.  There can be holes in the
			 * freespace blocks, so this might not succeed.
			 * This should be really rare, so there's no reason
			 * to avoid it.
			 */
<<<<<<< HEAD
			if ((error = xfs_da_read_buf(tp, dp,
					xfs_dir2_db_to_da(mp, fbno), -2, &fbp,
					XFS_DATA_FORK))) {
				return error;
			}
			if (unlikely(fbp == NULL)) {
				continue;
			}
			free = fbp->data;
<<<<<<< HEAD
			ASSERT(be32_to_cpu(free->hdr.magic) == XFS_DIR2_FREE_MAGIC);
=======
			ASSERT(free->hdr.magic == cpu_to_be32(XFS_DIR2_FREE_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
=======
			error = xfs_dir2_free_try_read(tp, dp,
						xfs_dir2_db_to_da(mp, fbno),
						&fbp);
			if (error)
				return error;
			if (!fbp)
				continue;
			free = fbp->b_addr;
>>>>>>> refs/remotes/origin/master
			findex = 0;
		}
		/*
		 * Look at the current free entry.  Is it good enough?
<<<<<<< HEAD
		 */
		if (be16_to_cpu(free->bests[findex]) != NULLDATAOFF &&
		    be16_to_cpu(free->bests[findex]) >= length)
			dbno = be32_to_cpu(free->hdr.firstdb) + findex;
=======
		 *
		 * The bests initialisation should be where the bufer is read in
		 * the above branch. But gcc is too stupid to realise that bests
		 * and the freehdr are actually initialised if they are placed
		 * there, so we have to do it here to avoid warnings. Blech.
		 */
		bests = dp->d_ops->free_bests_p(free);
		dp->d_ops->free_hdr_from_disk(&freehdr, free);
		if (be16_to_cpu(bests[findex]) != NULLDATAOFF &&
		    be16_to_cpu(bests[findex]) >= length)
			dbno = freehdr.firstdb + findex;
>>>>>>> refs/remotes/origin/master
		else {
			/*
			 * Are we done with the freeblock?
			 */
<<<<<<< HEAD
			if (++findex == be32_to_cpu(free->hdr.nvalid)) {
				/*
				 * Drop the block.
				 */
				xfs_da_brelse(tp, fbp);
=======
			if (++findex == freehdr.nvalid) {
				/*
				 * Drop the block.
				 */
				xfs_trans_brelse(tp, fbp);
>>>>>>> refs/remotes/origin/master
				fbp = NULL;
				if (fblk && fblk->bp)
					fblk->bp = NULL;
			}
		}
	}
	/*
	 * If we don't have a data block, we need to allocate one and make
	 * the freespace entries refer to it.
	 */
	if (unlikely(dbno == -1)) {
		/*
		 * Not allowed to allocate, return failure.
		 */
<<<<<<< HEAD
		if ((args->op_flags & XFS_DA_OP_JUSTCHECK) ||
							args->total == 0) {
			/*
			 * Drop the freespace buffer unless it came from our
			 * caller.
			 */
			if ((fblk == NULL || fblk->bp == NULL) && fbp != NULL)
				xfs_da_buf_done(fbp);
			return XFS_ERROR(ENOSPC);
		}
=======
		if ((args->op_flags & XFS_DA_OP_JUSTCHECK) || args->total == 0)
			return XFS_ERROR(ENOSPC);

>>>>>>> refs/remotes/origin/master
		/*
		 * Allocate and initialize the new data block.
		 */
		if (unlikely((error = xfs_dir2_grow_inode(args,
							 XFS_DIR2_DATA_SPACE,
							 &dbno)) ||
<<<<<<< HEAD
		    (error = xfs_dir2_data_init(args, dbno, &dbp)))) {
			/*
			 * Drop the freespace buffer unless it came from our
			 * caller.
			 */
			if ((fblk == NULL || fblk->bp == NULL) && fbp != NULL)
				xfs_da_buf_done(fbp);
			return error;
		}
=======
		    (error = xfs_dir3_data_init(args, dbno, &dbp))))
			return error;

>>>>>>> refs/remotes/origin/master
		/*
		 * If (somehow) we have a freespace block, get rid of it.
		 */
		if (fbp)
<<<<<<< HEAD
			xfs_da_brelse(tp, fbp);
=======
			xfs_trans_brelse(tp, fbp);
>>>>>>> refs/remotes/origin/master
		if (fblk && fblk->bp)
			fblk->bp = NULL;

		/*
		 * Get the freespace block corresponding to the data block
		 * that was just allocated.
		 */
<<<<<<< HEAD
		fbno = xfs_dir2_db_to_fdb(mp, dbno);
		if (unlikely(error = xfs_da_read_buf(tp, dp,
				xfs_dir2_db_to_da(mp, fbno), -2, &fbp,
				XFS_DATA_FORK))) {
			xfs_da_buf_done(dbp);
			return error;
  		}
=======
		fbno = dp->d_ops->db_to_fdb(mp, dbno);
		error = xfs_dir2_free_try_read(tp, dp,
					       xfs_dir2_db_to_da(mp, fbno),
					       &fbp);
		if (error)
			return error;

>>>>>>> refs/remotes/origin/master
		/*
		 * If there wasn't a freespace block, the read will
		 * return a NULL fbp.  Allocate and initialize a new one.
		 */
<<<<<<< HEAD
		if( fbp == NULL ) {
			if ((error = xfs_dir2_grow_inode(args, XFS_DIR2_FREE_SPACE,
							&fbno))) {
				return error;
			}

			if (unlikely(xfs_dir2_db_to_fdb(mp, dbno) != fbno)) {
				xfs_alert(mp,
<<<<<<< HEAD
			"%s: dir ino " "%llu needed freesp block %lld for\n"
=======
			"%s: dir ino %llu needed freesp block %lld for\n"
>>>>>>> refs/remotes/origin/cm-10.0
			"  data block %lld, got %lld ifbno %llu lastfbno %d",
					__func__, (unsigned long long)dp->i_ino,
					(long long)xfs_dir2_db_to_fdb(mp, dbno),
=======
		if (!fbp) {
			error = xfs_dir2_grow_inode(args, XFS_DIR2_FREE_SPACE,
						    &fbno);
			if (error)
				return error;

			if (unlikely(dp->d_ops->db_to_fdb(mp, dbno) != fbno)) {
				xfs_alert(mp,
			"%s: dir ino %llu needed freesp block %lld for\n"
			"  data block %lld, got %lld ifbno %llu lastfbno %d",
					__func__, (unsigned long long)dp->i_ino,
					(long long)dp->d_ops->db_to_fdb(mp, dbno),
>>>>>>> refs/remotes/origin/master
					(long long)dbno, (long long)fbno,
					(unsigned long long)ifbno, lastfbno);
				if (fblk) {
					xfs_alert(mp,
				" fblk 0x%p blkno %llu index %d magic 0x%x",
						fblk,
						(unsigned long long)fblk->blkno,
						fblk->index,
						fblk->magic);
				} else {
					xfs_alert(mp, " ... fblk is NULL");
				}
				XFS_ERROR_REPORT("xfs_dir2_node_addname_int",
						 XFS_ERRLEVEL_LOW, mp);
				return XFS_ERROR(EFSCORRUPTED);
			}

			/*
			 * Get a buffer for the new block.
			 */
<<<<<<< HEAD
			if ((error = xfs_da_get_buf(tp, dp,
						   xfs_dir2_db_to_da(mp, fbno),
						   -1, &fbp, XFS_DATA_FORK))) {
				return error;
			}
			ASSERT(fbp != NULL);

			/*
			 * Initialize the new block to be empty, and remember
			 * its first slot as our empty slot.
			 */
			free = fbp->data;
			free->hdr.magic = cpu_to_be32(XFS_DIR2_FREE_MAGIC);
			free->hdr.firstdb = cpu_to_be32(
				(fbno - XFS_DIR2_FREE_FIRSTDB(mp)) *
<<<<<<< HEAD
				XFS_DIR2_MAX_FREE_BESTS(mp));
=======
				xfs_dir2_free_max_bests(mp));
>>>>>>> refs/remotes/origin/cm-10.0
			free->hdr.nvalid = 0;
			free->hdr.nused = 0;
		} else {
			free = fbp->data;
<<<<<<< HEAD
			ASSERT(be32_to_cpu(free->hdr.magic) == XFS_DIR2_FREE_MAGIC);
=======
			ASSERT(free->hdr.magic == cpu_to_be32(XFS_DIR2_FREE_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
=======
			error = xfs_dir3_free_get_buf(tp, dp, fbno, &fbp);
			if (error)
				return error;
			free = fbp->b_addr;
			bests = dp->d_ops->free_bests_p(free);
			dp->d_ops->free_hdr_from_disk(&freehdr, free);

			/*
			 * Remember the first slot as our empty slot.
			 */
			freehdr.firstdb = (fbno - XFS_DIR2_FREE_FIRSTDB(mp)) *
					dp->d_ops->free_max_bests(mp);
		} else {
			free = fbp->b_addr;
			bests = dp->d_ops->free_bests_p(free);
			dp->d_ops->free_hdr_from_disk(&freehdr, free);
>>>>>>> refs/remotes/origin/master
		}

		/*
		 * Set the freespace block index from the data block number.
		 */
<<<<<<< HEAD
		findex = xfs_dir2_db_to_fdindex(mp, dbno);
=======
		findex = dp->d_ops->db_to_fdindex(mp, dbno);
>>>>>>> refs/remotes/origin/master
		/*
		 * If it's after the end of the current entries in the
		 * freespace block, extend that table.
		 */
<<<<<<< HEAD
		if (findex >= be32_to_cpu(free->hdr.nvalid)) {
<<<<<<< HEAD
			ASSERT(findex < XFS_DIR2_MAX_FREE_BESTS(mp));
=======
			ASSERT(findex < xfs_dir2_free_max_bests(mp));
>>>>>>> refs/remotes/origin/cm-10.0
			free->hdr.nvalid = cpu_to_be32(findex + 1);
			/*
			 * Tag new entry so nused will go up.
			 */
			free->bests[findex] = cpu_to_be16(NULLDATAOFF);
=======
		if (findex >= freehdr.nvalid) {
			ASSERT(findex < dp->d_ops->free_max_bests(mp));
			freehdr.nvalid = findex + 1;
			/*
			 * Tag new entry so nused will go up.
			 */
			bests[findex] = cpu_to_be16(NULLDATAOFF);
>>>>>>> refs/remotes/origin/master
		}
		/*
		 * If this entry was for an empty data block
		 * (this should always be true) then update the header.
		 */
<<<<<<< HEAD
<<<<<<< HEAD
		if (be16_to_cpu(free->bests[findex]) == NULLDATAOFF) {
=======
		if (free->bests[findex] == cpu_to_be16(NULLDATAOFF)) {
>>>>>>> refs/remotes/origin/cm-10.0
			be32_add_cpu(&free->hdr.nused, 1);
			xfs_dir2_free_log_header(tp, fbp);
=======
		if (bests[findex] == cpu_to_be16(NULLDATAOFF)) {
			freehdr.nused++;
			dp->d_ops->free_hdr_to_disk(fbp->b_addr, &freehdr);
			xfs_dir2_free_log_header(tp, dp, fbp);
>>>>>>> refs/remotes/origin/master
		}
		/*
		 * Update the real value in the table.
		 * We haven't allocated the data entry yet so this will
		 * change again.
		 */
<<<<<<< HEAD
<<<<<<< HEAD
		data = dbp->data;
		free->bests[findex] = data->hdr.bestfree[0].length;
=======
		hdr = dbp->data;
		free->bests[findex] = hdr->bestfree[0].length;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		hdr = dbp->b_addr;
		bf = dp->d_ops->data_bestfree_p(hdr);
		bests[findex] = bf[0].length;
>>>>>>> refs/remotes/origin/master
		logfree = 1;
	}
	/*
	 * We had a data block so we don't have to make a new one.
	 */
	else {
		/*
		 * If just checking, we succeeded.
		 */
<<<<<<< HEAD
		if (args->op_flags & XFS_DA_OP_JUSTCHECK) {
			if ((fblk == NULL || fblk->bp == NULL) && fbp != NULL)
				xfs_da_buf_done(fbp);
			return 0;
		}
		/*
		 * Read the data block in.
		 */
		if (unlikely(
		    error = xfs_da_read_buf(tp, dp, xfs_dir2_db_to_da(mp, dbno),
				-1, &dbp, XFS_DATA_FORK))) {
			if ((fblk == NULL || fblk->bp == NULL) && fbp != NULL)
				xfs_da_buf_done(fbp);
			return error;
		}
<<<<<<< HEAD
		data = dbp->data;
		logfree = 0;
	}
	ASSERT(be16_to_cpu(data->hdr.bestfree[0].length) >= length);
=======
		hdr = dbp->data;
		logfree = 0;
	}
	ASSERT(be16_to_cpu(hdr->bestfree[0].length) >= length);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (args->op_flags & XFS_DA_OP_JUSTCHECK)
			return 0;

		/*
		 * Read the data block in.
		 */
		error = xfs_dir3_data_read(tp, dp, xfs_dir2_db_to_da(mp, dbno),
					   -1, &dbp);
		if (error)
			return error;
		hdr = dbp->b_addr;
		bf = dp->d_ops->data_bestfree_p(hdr);
		logfree = 0;
	}
	ASSERT(be16_to_cpu(bf[0].length) >= length);
>>>>>>> refs/remotes/origin/master
	/*
	 * Point to the existing unused space.
	 */
	dup = (xfs_dir2_data_unused_t *)
<<<<<<< HEAD
<<<<<<< HEAD
	      ((char *)data + be16_to_cpu(data->hdr.bestfree[0].offset));
=======
	      ((char *)hdr + be16_to_cpu(hdr->bestfree[0].offset));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	      ((char *)hdr + be16_to_cpu(bf[0].offset));
>>>>>>> refs/remotes/origin/master
	needscan = needlog = 0;
	/*
	 * Mark the first part of the unused space, inuse for us.
	 */
<<<<<<< HEAD
	xfs_dir2_data_use_free(tp, dbp, dup,
<<<<<<< HEAD
		(xfs_dir2_data_aoff_t)((char *)dup - (char *)data), length,
=======
		(xfs_dir2_data_aoff_t)((char *)dup - (char *)hdr), length,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	xfs_dir2_data_use_free(tp, dp, dbp, dup,
		(xfs_dir2_data_aoff_t)((char *)dup - (char *)hdr), length,
>>>>>>> refs/remotes/origin/master
		&needlog, &needscan);
	/*
	 * Fill in the new entry and log it.
	 */
	dep = (xfs_dir2_data_entry_t *)dup;
	dep->inumber = cpu_to_be64(args->inumber);
	dep->namelen = args->namelen;
	memcpy(dep->name, args->name, dep->namelen);
<<<<<<< HEAD
	tagp = xfs_dir2_data_entry_tag_p(dep);
<<<<<<< HEAD
	*tagp = cpu_to_be16((char *)dep - (char *)data);
=======
	*tagp = cpu_to_be16((char *)dep - (char *)hdr);
>>>>>>> refs/remotes/origin/cm-10.0
	xfs_dir2_data_log_entry(tp, dbp, dep);
=======
	dp->d_ops->data_put_ftype(dep, args->filetype);
	tagp = dp->d_ops->data_entry_tag_p(dep);
	*tagp = cpu_to_be16((char *)dep - (char *)hdr);
	xfs_dir2_data_log_entry(tp, dp, dbp, dep);
>>>>>>> refs/remotes/origin/master
	/*
	 * Rescan the block for bestfree if needed.
	 */
	if (needscan)
<<<<<<< HEAD
<<<<<<< HEAD
		xfs_dir2_data_freescan(mp, data, &needlog);
=======
		xfs_dir2_data_freescan(mp, hdr, &needlog);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		xfs_dir2_data_freescan(dp, hdr, &needlog);
>>>>>>> refs/remotes/origin/master
	/*
	 * Log the data block header if needed.
	 */
	if (needlog)
<<<<<<< HEAD
		xfs_dir2_data_log_header(tp, dbp);
	/*
	 * If the freespace entry is now wrong, update it.
	 */
<<<<<<< HEAD
	if (be16_to_cpu(free->bests[findex]) != be16_to_cpu(data->hdr.bestfree[0].length)) {
		free->bests[findex] = data->hdr.bestfree[0].length;
=======
	if (be16_to_cpu(free->bests[findex]) != be16_to_cpu(hdr->bestfree[0].length)) {
		free->bests[findex] = hdr->bestfree[0].length;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		xfs_dir2_data_log_header(tp, dp, dbp);
	/*
	 * If the freespace entry is now wrong, update it.
	 */
	bests = dp->d_ops->free_bests_p(free); /* gcc is so stupid */
	if (be16_to_cpu(bests[findex]) != be16_to_cpu(bf[0].length)) {
		bests[findex] = bf[0].length;
>>>>>>> refs/remotes/origin/master
		logfree = 1;
	}
	/*
	 * Log the freespace entry if needed.
	 */
	if (logfree)
<<<<<<< HEAD
		xfs_dir2_free_log_bests(tp, fbp, findex, findex);
	/*
	 * If the caller didn't hand us the freespace block, drop it.
	 */
	if ((fblk == NULL || fblk->bp == NULL) && fbp != NULL)
		xfs_da_buf_done(fbp);
=======
		xfs_dir2_free_log_bests(tp, dp, fbp, findex, findex);
>>>>>>> refs/remotes/origin/master
	/*
	 * Return the data block and offset in args, then drop the data block.
	 */
	args->blkno = (xfs_dablk_t)dbno;
	args->index = be16_to_cpu(*tagp);
<<<<<<< HEAD
	xfs_da_buf_done(dbp);
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}

/*
 * Lookup an entry in a node-format directory.
<<<<<<< HEAD
 * All the real work happens in xfs_da_node_lookup_int.
=======
 * All the real work happens in xfs_da3_node_lookup_int.
>>>>>>> refs/remotes/origin/master
 * The only real output is the inode number of the entry.
 */
int						/* error */
xfs_dir2_node_lookup(
	xfs_da_args_t	*args)			/* operation arguments */
{
	int		error;			/* error return value */
	int		i;			/* btree level */
	int		rval;			/* operation return value */
	xfs_da_state_t	*state;			/* btree cursor */

	trace_xfs_dir2_node_lookup(args);

	/*
	 * Allocate and initialize the btree cursor.
	 */
	state = xfs_da_state_alloc();
	state->args = args;
	state->mp = args->dp->i_mount;
	state->blocksize = state->mp->m_dirblksize;
	state->node_ents = state->mp->m_dir_node_ents;
	/*
	 * Fill in the path to the entry in the cursor.
	 */
<<<<<<< HEAD
	error = xfs_da_node_lookup_int(state, &rval);
=======
	error = xfs_da3_node_lookup_int(state, &rval);
>>>>>>> refs/remotes/origin/master
	if (error)
		rval = error;
	else if (rval == ENOENT && args->cmpresult == XFS_CMP_CASE) {
		/* If a CI match, dup the actual name and return EEXIST */
		xfs_dir2_data_entry_t	*dep;

<<<<<<< HEAD
		dep = (xfs_dir2_data_entry_t *)((char *)state->extrablk.bp->
						data + state->extrablk.index);
=======
		dep = (xfs_dir2_data_entry_t *)
			((char *)state->extrablk.bp->b_addr +
						 state->extrablk.index);
>>>>>>> refs/remotes/origin/master
		rval = xfs_dir_cilookup_result(args, dep->name, dep->namelen);
	}
	/*
	 * Release the btree blocks and leaf block.
	 */
	for (i = 0; i < state->path.active; i++) {
<<<<<<< HEAD
		xfs_da_brelse(args->trans, state->path.blk[i].bp);
=======
		xfs_trans_brelse(args->trans, state->path.blk[i].bp);
>>>>>>> refs/remotes/origin/master
		state->path.blk[i].bp = NULL;
	}
	/*
	 * Release the data block if we have it.
	 */
	if (state->extravalid && state->extrablk.bp) {
<<<<<<< HEAD
		xfs_da_brelse(args->trans, state->extrablk.bp);
=======
		xfs_trans_brelse(args->trans, state->extrablk.bp);
>>>>>>> refs/remotes/origin/master
		state->extrablk.bp = NULL;
	}
	xfs_da_state_free(state);
	return rval;
}

/*
 * Remove an entry from a node-format directory.
 */
int						/* error */
xfs_dir2_node_removename(
<<<<<<< HEAD
	xfs_da_args_t		*args)		/* operation arguments */
{
	xfs_da_state_blk_t	*blk;		/* leaf block */
	int			error;		/* error return value */
	int			rval;		/* operation return value */
	xfs_da_state_t		*state;		/* btree cursor */
=======
	struct xfs_da_args	*args)		/* operation arguments */
{
	struct xfs_da_state_blk	*blk;		/* leaf block */
	int			error;		/* error return value */
	int			rval;		/* operation return value */
	struct xfs_da_state	*state;		/* btree cursor */
>>>>>>> refs/remotes/origin/master

	trace_xfs_dir2_node_removename(args);

	/*
	 * Allocate and initialize the btree cursor.
	 */
	state = xfs_da_state_alloc();
	state->args = args;
	state->mp = args->dp->i_mount;
	state->blocksize = state->mp->m_dirblksize;
	state->node_ents = state->mp->m_dir_node_ents;
<<<<<<< HEAD
	/*
	 * Look up the entry we're deleting, set up the cursor.
	 */
	error = xfs_da_node_lookup_int(state, &rval);
	if (error)
		rval = error;
	/*
	 * Didn't find it, upper layer screwed up.
	 */
	if (rval != EEXIST) {
		xfs_da_state_free(state);
		return rval;
	}
=======

	/* Look up the entry we're deleting, set up the cursor. */
	error = xfs_da3_node_lookup_int(state, &rval);
	if (error)
		goto out_free;

	/* Didn't find it, upper layer screwed up. */
	if (rval != EEXIST) {
		error = rval;
		goto out_free;
	}

>>>>>>> refs/remotes/origin/master
	blk = &state->path.blk[state->path.active - 1];
	ASSERT(blk->magic == XFS_DIR2_LEAFN_MAGIC);
	ASSERT(state->extravalid);
	/*
	 * Remove the leaf and data entries.
	 * Extrablk refers to the data block.
	 */
	error = xfs_dir2_leafn_remove(args, blk->bp, blk->index,
		&state->extrablk, &rval);
	if (error)
<<<<<<< HEAD
		return error;
	/*
	 * Fix the hash values up the btree.
	 */
	xfs_da_fixhashpath(state, &state->path);
=======
		goto out_free;
	/*
	 * Fix the hash values up the btree.
	 */
	xfs_da3_fixhashpath(state, &state->path);
>>>>>>> refs/remotes/origin/master
	/*
	 * If we need to join leaf blocks, do it.
	 */
	if (rval && state->path.active > 1)
<<<<<<< HEAD
		error = xfs_da_join(state);
=======
		error = xfs_da3_join(state);
>>>>>>> refs/remotes/origin/master
	/*
	 * If no errors so far, try conversion to leaf format.
	 */
	if (!error)
		error = xfs_dir2_node_to_leaf(state);
<<<<<<< HEAD
=======
out_free:
>>>>>>> refs/remotes/origin/master
	xfs_da_state_free(state);
	return error;
}

/*
 * Replace an entry's inode number in a node-format directory.
 */
int						/* error */
xfs_dir2_node_replace(
	xfs_da_args_t		*args)		/* operation arguments */
{
	xfs_da_state_blk_t	*blk;		/* leaf block */
<<<<<<< HEAD
<<<<<<< HEAD
	xfs_dir2_data_t		*data;		/* data block structure */
=======
	xfs_dir2_data_hdr_t	*hdr;		/* data block header */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	xfs_dir2_data_hdr_t	*hdr;		/* data block header */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_data_entry_t	*dep;		/* data entry changed */
	int			error;		/* error return value */
	int			i;		/* btree level */
	xfs_ino_t		inum;		/* new inode number */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	xfs_dir2_leaf_entry_t	*lep;		/* leaf entry being changed */
	int			rval;		/* internal return value */
	xfs_da_state_t		*state;		/* btree cursor */

	trace_xfs_dir2_node_replace(args);

	/*
	 * Allocate and initialize the btree cursor.
	 */
	state = xfs_da_state_alloc();
	state->args = args;
	state->mp = args->dp->i_mount;
	state->blocksize = state->mp->m_dirblksize;
	state->node_ents = state->mp->m_dir_node_ents;
	inum = args->inumber;
	/*
	 * Lookup the entry to change in the btree.
	 */
<<<<<<< HEAD
	error = xfs_da_node_lookup_int(state, &rval);
=======
	error = xfs_da3_node_lookup_int(state, &rval);
>>>>>>> refs/remotes/origin/master
	if (error) {
		rval = error;
	}
	/*
	 * It should be found, since the vnodeops layer has looked it up
	 * and locked it.  But paranoia is good.
	 */
	if (rval == EEXIST) {
<<<<<<< HEAD
=======
		struct xfs_dir2_leaf_entry *ents;
>>>>>>> refs/remotes/origin/master
		/*
		 * Find the leaf entry.
		 */
		blk = &state->path.blk[state->path.active - 1];
		ASSERT(blk->magic == XFS_DIR2_LEAFN_MAGIC);
<<<<<<< HEAD
		leaf = blk->bp->data;
		lep = &leaf->ents[blk->index];
=======
		leaf = blk->bp->b_addr;
		ents = args->dp->d_ops->leaf_ents_p(leaf);
		lep = &ents[blk->index];
>>>>>>> refs/remotes/origin/master
		ASSERT(state->extravalid);
		/*
		 * Point to the data entry.
		 */
<<<<<<< HEAD
<<<<<<< HEAD
		data = state->extrablk.bp->data;
		ASSERT(be32_to_cpu(data->hdr.magic) == XFS_DIR2_DATA_MAGIC);
		dep = (xfs_dir2_data_entry_t *)
		      ((char *)data +
=======
		hdr = state->extrablk.bp->data;
		ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC));
		dep = (xfs_dir2_data_entry_t *)
		      ((char *)hdr +
>>>>>>> refs/remotes/origin/cm-10.0
=======
		hdr = state->extrablk.bp->b_addr;
		ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC) ||
		       hdr->magic == cpu_to_be32(XFS_DIR3_DATA_MAGIC));
		dep = (xfs_dir2_data_entry_t *)
		      ((char *)hdr +
>>>>>>> refs/remotes/origin/master
		       xfs_dir2_dataptr_to_off(state->mp, be32_to_cpu(lep->address)));
		ASSERT(inum != be64_to_cpu(dep->inumber));
		/*
		 * Fill in the new inode number and log the entry.
		 */
		dep->inumber = cpu_to_be64(inum);
<<<<<<< HEAD
		xfs_dir2_data_log_entry(args->trans, state->extrablk.bp, dep);
=======
		args->dp->d_ops->data_put_ftype(dep, args->filetype);
		xfs_dir2_data_log_entry(args->trans, args->dp,
					state->extrablk.bp, dep);
>>>>>>> refs/remotes/origin/master
		rval = 0;
	}
	/*
	 * Didn't find it, and we're holding a data block.  Drop it.
	 */
	else if (state->extravalid) {
<<<<<<< HEAD
		xfs_da_brelse(args->trans, state->extrablk.bp);
=======
		xfs_trans_brelse(args->trans, state->extrablk.bp);
>>>>>>> refs/remotes/origin/master
		state->extrablk.bp = NULL;
	}
	/*
	 * Release all the buffers in the cursor.
	 */
	for (i = 0; i < state->path.active; i++) {
<<<<<<< HEAD
		xfs_da_brelse(args->trans, state->path.blk[i].bp);
=======
		xfs_trans_brelse(args->trans, state->path.blk[i].bp);
>>>>>>> refs/remotes/origin/master
		state->path.blk[i].bp = NULL;
	}
	xfs_da_state_free(state);
	return rval;
}

/*
 * Trim off a trailing empty freespace block.
 * Return (in rvalp) 1 if we did it, 0 if not.
 */
int						/* error */
xfs_dir2_node_trim_free(
	xfs_da_args_t		*args,		/* operation arguments */
	xfs_fileoff_t		fo,		/* free block number */
	int			*rvalp)		/* out: did something */
{
<<<<<<< HEAD
	xfs_dabuf_t		*bp;		/* freespace buffer */
=======
	struct xfs_buf		*bp;		/* freespace buffer */
>>>>>>> refs/remotes/origin/master
	xfs_inode_t		*dp;		/* incore directory inode */
	int			error;		/* error return code */
	xfs_dir2_free_t		*free;		/* freespace structure */
	xfs_mount_t		*mp;		/* filesystem mount point */
	xfs_trans_t		*tp;		/* transaction pointer */
<<<<<<< HEAD
=======
	struct xfs_dir3_icfree_hdr freehdr;
>>>>>>> refs/remotes/origin/master

	dp = args->dp;
	mp = dp->i_mount;
	tp = args->trans;
	/*
	 * Read the freespace block.
	 */
<<<<<<< HEAD
	if (unlikely(error = xfs_da_read_buf(tp, dp, (xfs_dablk_t)fo, -2, &bp,
			XFS_DATA_FORK))) {
		return error;
	}

=======
	error = xfs_dir2_free_try_read(tp, dp, fo, &bp);
	if (error)
		return error;
>>>>>>> refs/remotes/origin/master
	/*
	 * There can be holes in freespace.  If fo is a hole, there's
	 * nothing to do.
	 */
<<<<<<< HEAD
	if (bp == NULL) {
		return 0;
	}
	free = bp->data;
<<<<<<< HEAD
	ASSERT(be32_to_cpu(free->hdr.magic) == XFS_DIR2_FREE_MAGIC);
=======
	ASSERT(free->hdr.magic == cpu_to_be32(XFS_DIR2_FREE_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * If there are used entries, there's nothing to do.
	 */
	if (be32_to_cpu(free->hdr.nused) > 0) {
		xfs_da_brelse(tp, bp);
=======
	if (!bp)
		return 0;
	free = bp->b_addr;
	dp->d_ops->free_hdr_from_disk(&freehdr, free);

	/*
	 * If there are used entries, there's nothing to do.
	 */
	if (freehdr.nused > 0) {
		xfs_trans_brelse(tp, bp);
>>>>>>> refs/remotes/origin/master
		*rvalp = 0;
		return 0;
	}
	/*
	 * Blow the block away.
	 */
	if ((error =
	    xfs_dir2_shrink_inode(args, xfs_dir2_da_to_db(mp, (xfs_dablk_t)fo),
		    bp))) {
		/*
		 * Can't fail with ENOSPC since that only happens with no
		 * space reservation, when breaking up an extent into two
		 * pieces.  This is the last block of an extent.
		 */
		ASSERT(error != ENOSPC);
<<<<<<< HEAD
		xfs_da_brelse(tp, bp);
=======
		xfs_trans_brelse(tp, bp);
>>>>>>> refs/remotes/origin/master
		return error;
	}
	/*
	 * Return that we succeeded.
	 */
	*rvalp = 1;
	return 0;
}
