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
#include "xfs_bit.h"
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
 * Local function declarations.
 */
<<<<<<< HEAD
#ifdef DEBUG
static void xfs_dir2_leaf_check(xfs_inode_t *dp, xfs_dabuf_t *bp);
#else
#define	xfs_dir2_leaf_check(dp, bp)
#endif
static int xfs_dir2_leaf_lookup_int(xfs_da_args_t *args, xfs_dabuf_t **lbpp,
				    int *indexp, xfs_dabuf_t **dbpp);
static void xfs_dir2_leaf_log_bests(struct xfs_trans *tp, struct xfs_dabuf *bp,
				    int first, int last);
static void xfs_dir2_leaf_log_tail(struct xfs_trans *tp, struct xfs_dabuf *bp);

=======
static int xfs_dir2_leaf_lookup_int(xfs_da_args_t *args, struct xfs_buf **lbpp,
				    int *indexp, struct xfs_buf **dbpp);
static void xfs_dir3_leaf_log_bests(struct xfs_trans *tp, struct xfs_buf *bp,
				    int first, int last);
static void xfs_dir3_leaf_log_tail(struct xfs_trans *tp, struct xfs_buf *bp);

/*
 * Check the internal consistency of a leaf1 block.
 * Pop an assert if something is wrong.
 */
#ifdef DEBUG
#define	xfs_dir3_leaf_check(dp, bp) \
do { \
	if (!xfs_dir3_leaf1_check((dp), (bp))) \
		ASSERT(0); \
} while (0);

STATIC bool
xfs_dir3_leaf1_check(
	struct xfs_inode	*dp,
	struct xfs_buf		*bp)
{
	struct xfs_dir2_leaf	*leaf = bp->b_addr;
	struct xfs_dir3_icleaf_hdr leafhdr;

	dp->d_ops->leaf_hdr_from_disk(&leafhdr, leaf);

	if (leafhdr.magic == XFS_DIR3_LEAF1_MAGIC) {
		struct xfs_dir3_leaf_hdr *leaf3 = bp->b_addr;
		if (be64_to_cpu(leaf3->info.blkno) != bp->b_bn)
			return false;
	} else if (leafhdr.magic != XFS_DIR2_LEAF1_MAGIC)
		return false;

	return xfs_dir3_leaf_check_int(dp->i_mount, dp, &leafhdr, leaf);
}
#else
#define	xfs_dir3_leaf_check(dp, bp)
#endif

bool
xfs_dir3_leaf_check_int(
	struct xfs_mount	*mp,
	struct xfs_inode	*dp,
	struct xfs_dir3_icleaf_hdr *hdr,
	struct xfs_dir2_leaf	*leaf)
{
	struct xfs_dir2_leaf_entry *ents;
	xfs_dir2_leaf_tail_t	*ltp;
	int			stale;
	int			i;
	const struct xfs_dir_ops *ops;
	struct xfs_dir3_icleaf_hdr leafhdr;

	/*
	 * we can be passed a null dp here from a verifier, so we need to go the
	 * hard way to get them.
	 */
	ops = xfs_dir_get_ops(mp, dp);

	if (!hdr) {
		ops->leaf_hdr_from_disk(&leafhdr, leaf);
		hdr = &leafhdr;
	}

	ents = ops->leaf_ents_p(leaf);
	ltp = xfs_dir2_leaf_tail_p(mp, leaf);

	/*
	 * XXX (dgc): This value is not restrictive enough.
	 * Should factor in the size of the bests table as well.
	 * We can deduce a value for that from di_size.
	 */
	if (hdr->count > ops->leaf_max_ents(mp))
		return false;

	/* Leaves and bests don't overlap in leaf format. */
	if ((hdr->magic == XFS_DIR2_LEAF1_MAGIC ||
	     hdr->magic == XFS_DIR3_LEAF1_MAGIC) &&
	    (char *)&ents[hdr->count] > (char *)xfs_dir2_leaf_bests_p(ltp))
		return false;

	/* Check hash value order, count stale entries.  */
	for (i = stale = 0; i < hdr->count; i++) {
		if (i + 1 < hdr->count) {
			if (be32_to_cpu(ents[i].hashval) >
					be32_to_cpu(ents[i + 1].hashval))
				return false;
		}
		if (ents[i].address == cpu_to_be32(XFS_DIR2_NULL_DATAPTR))
			stale++;
	}
	if (hdr->stale != stale)
		return false;
	return true;
}

/*
 * We verify the magic numbers before decoding the leaf header so that on debug
 * kernels we don't get assertion failures in xfs_dir3_leaf_hdr_from_disk() due
 * to incorrect magic numbers.
 */
static bool
xfs_dir3_leaf_verify(
	struct xfs_buf		*bp,
	__uint16_t		magic)
{
	struct xfs_mount	*mp = bp->b_target->bt_mount;
	struct xfs_dir2_leaf	*leaf = bp->b_addr;

	ASSERT(magic == XFS_DIR2_LEAF1_MAGIC || magic == XFS_DIR2_LEAFN_MAGIC);

	if (xfs_sb_version_hascrc(&mp->m_sb)) {
		struct xfs_dir3_leaf_hdr *leaf3 = bp->b_addr;
		__uint16_t		magic3;

		magic3 = (magic == XFS_DIR2_LEAF1_MAGIC) ? XFS_DIR3_LEAF1_MAGIC
							 : XFS_DIR3_LEAFN_MAGIC;

		if (leaf3->info.hdr.magic != cpu_to_be16(magic3))
			return false;
		if (!uuid_equal(&leaf3->info.uuid, &mp->m_sb.sb_uuid))
			return false;
		if (be64_to_cpu(leaf3->info.blkno) != bp->b_bn)
			return false;
	} else {
		if (leaf->hdr.info.magic != cpu_to_be16(magic))
			return false;
	}

	return xfs_dir3_leaf_check_int(mp, NULL, NULL, leaf);
}

static void
__read_verify(
	struct xfs_buf  *bp,
	__uint16_t	magic)
{
	struct xfs_mount	*mp = bp->b_target->bt_mount;

	if ((xfs_sb_version_hascrc(&mp->m_sb) &&
	     !xfs_verify_cksum(bp->b_addr, BBTOB(bp->b_length),
					  XFS_DIR3_LEAF_CRC_OFF)) ||
	    !xfs_dir3_leaf_verify(bp, magic)) {
		XFS_CORRUPTION_ERROR(__func__, XFS_ERRLEVEL_LOW, mp, bp->b_addr);
		xfs_buf_ioerror(bp, EFSCORRUPTED);
	}
}

static void
__write_verify(
	struct xfs_buf  *bp,
	__uint16_t	magic)
{
	struct xfs_mount	*mp = bp->b_target->bt_mount;
	struct xfs_buf_log_item	*bip = bp->b_fspriv;
	struct xfs_dir3_leaf_hdr *hdr3 = bp->b_addr;

	if (!xfs_dir3_leaf_verify(bp, magic)) {
		XFS_CORRUPTION_ERROR(__func__, XFS_ERRLEVEL_LOW, mp, bp->b_addr);
		xfs_buf_ioerror(bp, EFSCORRUPTED);
		return;
	}

	if (!xfs_sb_version_hascrc(&mp->m_sb))
		return;

	if (bip)
		hdr3->info.lsn = cpu_to_be64(bip->bli_item.li_lsn);

	xfs_update_cksum(bp->b_addr, BBTOB(bp->b_length), XFS_DIR3_LEAF_CRC_OFF);
}

static void
xfs_dir3_leaf1_read_verify(
	struct xfs_buf	*bp)
{
	__read_verify(bp, XFS_DIR2_LEAF1_MAGIC);
}

static void
xfs_dir3_leaf1_write_verify(
	struct xfs_buf	*bp)
{
	__write_verify(bp, XFS_DIR2_LEAF1_MAGIC);
}

static void
xfs_dir3_leafn_read_verify(
	struct xfs_buf	*bp)
{
	__read_verify(bp, XFS_DIR2_LEAFN_MAGIC);
}

static void
xfs_dir3_leafn_write_verify(
	struct xfs_buf	*bp)
{
	__write_verify(bp, XFS_DIR2_LEAFN_MAGIC);
}

const struct xfs_buf_ops xfs_dir3_leaf1_buf_ops = {
	.verify_read = xfs_dir3_leaf1_read_verify,
	.verify_write = xfs_dir3_leaf1_write_verify,
};

const struct xfs_buf_ops xfs_dir3_leafn_buf_ops = {
	.verify_read = xfs_dir3_leafn_read_verify,
	.verify_write = xfs_dir3_leafn_write_verify,
};

static int
xfs_dir3_leaf_read(
	struct xfs_trans	*tp,
	struct xfs_inode	*dp,
	xfs_dablk_t		fbno,
	xfs_daddr_t		mappedbno,
	struct xfs_buf		**bpp)
{
	int			err;

	err = xfs_da_read_buf(tp, dp, fbno, mappedbno, bpp,
				XFS_DATA_FORK, &xfs_dir3_leaf1_buf_ops);
	if (!err && tp)
		xfs_trans_buf_set_type(tp, *bpp, XFS_BLFT_DIR_LEAF1_BUF);
	return err;
}

int
xfs_dir3_leafn_read(
	struct xfs_trans	*tp,
	struct xfs_inode	*dp,
	xfs_dablk_t		fbno,
	xfs_daddr_t		mappedbno,
	struct xfs_buf		**bpp)
{
	int			err;

	err = xfs_da_read_buf(tp, dp, fbno, mappedbno, bpp,
				XFS_DATA_FORK, &xfs_dir3_leafn_buf_ops);
	if (!err && tp)
		xfs_trans_buf_set_type(tp, *bpp, XFS_BLFT_DIR_LEAFN_BUF);
	return err;
}

/*
 * Initialize a new leaf block, leaf1 or leafn magic accepted.
 */
static void
xfs_dir3_leaf_init(
	struct xfs_mount	*mp,
	struct xfs_trans	*tp,
	struct xfs_buf		*bp,
	xfs_ino_t		owner,
	__uint16_t		type)
{
	struct xfs_dir2_leaf	*leaf = bp->b_addr;

	ASSERT(type == XFS_DIR2_LEAF1_MAGIC || type == XFS_DIR2_LEAFN_MAGIC);

	if (xfs_sb_version_hascrc(&mp->m_sb)) {
		struct xfs_dir3_leaf_hdr *leaf3 = bp->b_addr;

		memset(leaf3, 0, sizeof(*leaf3));

		leaf3->info.hdr.magic = (type == XFS_DIR2_LEAF1_MAGIC)
					 ? cpu_to_be16(XFS_DIR3_LEAF1_MAGIC)
					 : cpu_to_be16(XFS_DIR3_LEAFN_MAGIC);
		leaf3->info.blkno = cpu_to_be64(bp->b_bn);
		leaf3->info.owner = cpu_to_be64(owner);
		uuid_copy(&leaf3->info.uuid, &mp->m_sb.sb_uuid);
	} else {
		memset(leaf, 0, sizeof(*leaf));
		leaf->hdr.info.magic = cpu_to_be16(type);
	}

	/*
	 * If it's a leaf-format directory initialize the tail.
	 * Caller is responsible for initialising the bests table.
	 */
	if (type == XFS_DIR2_LEAF1_MAGIC) {
		struct xfs_dir2_leaf_tail *ltp;

		ltp = xfs_dir2_leaf_tail_p(mp, leaf);
		ltp->bestcount = 0;
		bp->b_ops = &xfs_dir3_leaf1_buf_ops;
		xfs_trans_buf_set_type(tp, bp, XFS_BLFT_DIR_LEAF1_BUF);
	} else {
		bp->b_ops = &xfs_dir3_leafn_buf_ops;
		xfs_trans_buf_set_type(tp, bp, XFS_BLFT_DIR_LEAFN_BUF);
	}
}

int
xfs_dir3_leaf_get_buf(
	xfs_da_args_t		*args,
	xfs_dir2_db_t		bno,
	struct xfs_buf		**bpp,
	__uint16_t		magic)
{
	struct xfs_inode	*dp = args->dp;
	struct xfs_trans	*tp = args->trans;
	struct xfs_mount	*mp = dp->i_mount;
	struct xfs_buf		*bp;
	int			error;

	ASSERT(magic == XFS_DIR2_LEAF1_MAGIC || magic == XFS_DIR2_LEAFN_MAGIC);
	ASSERT(bno >= XFS_DIR2_LEAF_FIRSTDB(mp) &&
	       bno < XFS_DIR2_FREE_FIRSTDB(mp));

	error = xfs_da_get_buf(tp, dp, xfs_dir2_db_to_da(mp, bno), -1, &bp,
			       XFS_DATA_FORK);
	if (error)
		return error;

	xfs_dir3_leaf_init(mp, tp, bp, dp->i_ino, magic);
	xfs_dir3_leaf_log_header(tp, dp, bp);
	if (magic == XFS_DIR2_LEAF1_MAGIC)
		xfs_dir3_leaf_log_tail(tp, bp);
	*bpp = bp;
	return 0;
}
>>>>>>> refs/remotes/origin/master

/*
 * Convert a block form directory to a leaf form directory.
 */
int						/* error */
xfs_dir2_block_to_leaf(
	xfs_da_args_t		*args,		/* operation arguments */
<<<<<<< HEAD
	xfs_dabuf_t		*dbp)		/* input block's buffer */
{
	__be16			*bestsp;	/* leaf's bestsp entries */
	xfs_dablk_t		blkno;		/* leaf block's bno */
<<<<<<< HEAD
	xfs_dir2_block_t	*block;		/* block structure */
=======
	xfs_dir2_data_hdr_t	*hdr;		/* block header */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct xfs_buf		*dbp)		/* input block's buffer */
{
	__be16			*bestsp;	/* leaf's bestsp entries */
	xfs_dablk_t		blkno;		/* leaf block's bno */
	xfs_dir2_data_hdr_t	*hdr;		/* block header */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_leaf_entry_t	*blp;		/* block's leaf entries */
	xfs_dir2_block_tail_t	*btp;		/* block's tail */
	xfs_inode_t		*dp;		/* incore directory inode */
	int			error;		/* error return code */
<<<<<<< HEAD
	xfs_dabuf_t		*lbp;		/* leaf block's buffer */
=======
	struct xfs_buf		*lbp;		/* leaf block's buffer */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_db_t		ldb;		/* leaf block's bno */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	xfs_dir2_leaf_tail_t	*ltp;		/* leaf's tail */
	xfs_mount_t		*mp;		/* filesystem mount point */
	int			needlog;	/* need to log block header */
	int			needscan;	/* need to rescan bestfree */
	xfs_trans_t		*tp;		/* transaction pointer */
<<<<<<< HEAD
=======
	struct xfs_dir2_data_free *bf;
	struct xfs_dir2_leaf_entry *ents;
	struct xfs_dir3_icleaf_hdr leafhdr;
>>>>>>> refs/remotes/origin/master

	trace_xfs_dir2_block_to_leaf(args);

	dp = args->dp;
	mp = dp->i_mount;
	tp = args->trans;
	/*
	 * Add the leaf block to the inode.
	 * This interface will only put blocks in the leaf/node range.
	 * Since that's empty now, we'll get the root (block 0 in range).
	 */
	if ((error = xfs_da_grow_inode(args, &blkno))) {
		return error;
	}
	ldb = xfs_dir2_da_to_db(mp, blkno);
	ASSERT(ldb == XFS_DIR2_LEAF_FIRSTDB(mp));
	/*
	 * Initialize the leaf block, get a buffer for it.
	 */
<<<<<<< HEAD
	if ((error = xfs_dir2_leaf_init(args, ldb, &lbp, XFS_DIR2_LEAF1_MAGIC))) {
		return error;
	}
	ASSERT(lbp != NULL);
	leaf = lbp->data;
<<<<<<< HEAD
	block = dbp->data;
	xfs_dir2_data_check(dp, dbp);
	btp = xfs_dir2_block_tail_p(mp, block);
=======
	hdr = dbp->data;
	xfs_dir2_data_check(dp, dbp);
	btp = xfs_dir2_block_tail_p(mp, hdr);
>>>>>>> refs/remotes/origin/cm-10.0
	blp = xfs_dir2_block_leaf_p(btp);
	/*
	 * Set the counts in the leaf header.
	 */
	leaf->hdr.count = cpu_to_be16(be32_to_cpu(btp->count));
	leaf->hdr.stale = cpu_to_be16(be32_to_cpu(btp->stale));
=======
	error = xfs_dir3_leaf_get_buf(args, ldb, &lbp, XFS_DIR2_LEAF1_MAGIC);
	if (error)
		return error;

	leaf = lbp->b_addr;
	hdr = dbp->b_addr;
	xfs_dir3_data_check(dp, dbp);
	btp = xfs_dir2_block_tail_p(mp, hdr);
	blp = xfs_dir2_block_leaf_p(btp);
	bf = dp->d_ops->data_bestfree_p(hdr);
	ents = dp->d_ops->leaf_ents_p(leaf);

	/*
	 * Set the counts in the leaf header.
	 */
	dp->d_ops->leaf_hdr_from_disk(&leafhdr, leaf);
	leafhdr.count = be32_to_cpu(btp->count);
	leafhdr.stale = be32_to_cpu(btp->stale);
	dp->d_ops->leaf_hdr_to_disk(leaf, &leafhdr);
	xfs_dir3_leaf_log_header(tp, dp, lbp);

>>>>>>> refs/remotes/origin/master
	/*
	 * Could compact these but I think we always do the conversion
	 * after squeezing out stale entries.
	 */
<<<<<<< HEAD
	memcpy(leaf->ents, blp, be32_to_cpu(btp->count) * sizeof(xfs_dir2_leaf_entry_t));
	xfs_dir2_leaf_log_ents(tp, lbp, 0, be16_to_cpu(leaf->hdr.count) - 1);
=======
	memcpy(ents, blp, be32_to_cpu(btp->count) * sizeof(xfs_dir2_leaf_entry_t));
	xfs_dir3_leaf_log_ents(tp, dp, lbp, 0, leafhdr.count - 1);
>>>>>>> refs/remotes/origin/master
	needscan = 0;
	needlog = 1;
	/*
	 * Make the space formerly occupied by the leaf entries and block
	 * tail be free.
	 */
<<<<<<< HEAD
	xfs_dir2_data_make_free(tp, dbp,
<<<<<<< HEAD
		(xfs_dir2_data_aoff_t)((char *)blp - (char *)block),
		(xfs_dir2_data_aoff_t)((char *)block + mp->m_dirblksize -
=======
		(xfs_dir2_data_aoff_t)((char *)blp - (char *)hdr),
		(xfs_dir2_data_aoff_t)((char *)hdr + mp->m_dirblksize -
>>>>>>> refs/remotes/origin/cm-10.0
=======
	xfs_dir2_data_make_free(tp, dp, dbp,
		(xfs_dir2_data_aoff_t)((char *)blp - (char *)hdr),
		(xfs_dir2_data_aoff_t)((char *)hdr + mp->m_dirblksize -
>>>>>>> refs/remotes/origin/master
				       (char *)blp),
		&needlog, &needscan);
	/*
	 * Fix up the block header, make it a data block.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	block->hdr.magic = cpu_to_be32(XFS_DIR2_DATA_MAGIC);
	if (needscan)
		xfs_dir2_data_freescan(mp, (xfs_dir2_data_t *)block, &needlog);
=======
	hdr->magic = cpu_to_be32(XFS_DIR2_DATA_MAGIC);
	if (needscan)
		xfs_dir2_data_freescan(mp, hdr, &needlog);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	dbp->b_ops = &xfs_dir3_data_buf_ops;
	xfs_trans_buf_set_type(tp, dbp, XFS_BLFT_DIR_DATA_BUF);
	if (hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC))
		hdr->magic = cpu_to_be32(XFS_DIR2_DATA_MAGIC);
	else
		hdr->magic = cpu_to_be32(XFS_DIR3_DATA_MAGIC);

	if (needscan)
		xfs_dir2_data_freescan(dp, hdr, &needlog);
>>>>>>> refs/remotes/origin/master
	/*
	 * Set up leaf tail and bests table.
	 */
	ltp = xfs_dir2_leaf_tail_p(mp, leaf);
	ltp->bestcount = cpu_to_be32(1);
	bestsp = xfs_dir2_leaf_bests_p(ltp);
<<<<<<< HEAD
<<<<<<< HEAD
	bestsp[0] =  block->hdr.bestfree[0].length;
=======
	bestsp[0] =  hdr->bestfree[0].length;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bestsp[0] =  bf[0].length;
>>>>>>> refs/remotes/origin/master
	/*
	 * Log the data header and leaf bests table.
	 */
	if (needlog)
<<<<<<< HEAD
		xfs_dir2_data_log_header(tp, dbp);
	xfs_dir2_leaf_check(dp, lbp);
	xfs_dir2_data_check(dp, dbp);
	xfs_dir2_leaf_log_bests(tp, lbp, 0, 0);
	xfs_da_buf_done(lbp);
	return 0;
}

<<<<<<< HEAD
=======
STATIC void
xfs_dir2_leaf_find_stale(
	struct xfs_dir2_leaf	*leaf,
=======
		xfs_dir2_data_log_header(tp, dp, dbp);
	xfs_dir3_leaf_check(dp, lbp);
	xfs_dir3_data_check(dp, dbp);
	xfs_dir3_leaf_log_bests(tp, lbp, 0, 0);
	return 0;
}

STATIC void
xfs_dir3_leaf_find_stale(
	struct xfs_dir3_icleaf_hdr *leafhdr,
	struct xfs_dir2_leaf_entry *ents,
>>>>>>> refs/remotes/origin/master
	int			index,
	int			*lowstale,
	int			*highstale)
{
	/*
	 * Find the first stale entry before our index, if any.
	 */
	for (*lowstale = index - 1; *lowstale >= 0; --*lowstale) {
<<<<<<< HEAD
		if (leaf->ents[*lowstale].address ==
=======
		if (ents[*lowstale].address ==
>>>>>>> refs/remotes/origin/master
		    cpu_to_be32(XFS_DIR2_NULL_DATAPTR))
			break;
	}

	/*
	 * Find the first stale entry at or after our index, if any.
	 * Stop if the result would require moving more entries than using
	 * lowstale.
	 */
<<<<<<< HEAD
	for (*highstale = index;
	     *highstale < be16_to_cpu(leaf->hdr.count);
	     ++*highstale) {
		if (leaf->ents[*highstale].address ==
=======
	for (*highstale = index; *highstale < leafhdr->count; ++*highstale) {
		if (ents[*highstale].address ==
>>>>>>> refs/remotes/origin/master
		    cpu_to_be32(XFS_DIR2_NULL_DATAPTR))
			break;
		if (*lowstale >= 0 && index - *lowstale <= *highstale - index)
			break;
	}
}

struct xfs_dir2_leaf_entry *
<<<<<<< HEAD
xfs_dir2_leaf_find_entry(
	xfs_dir2_leaf_t		*leaf,		/* leaf structure */
=======
xfs_dir3_leaf_find_entry(
	struct xfs_dir3_icleaf_hdr *leafhdr,
	struct xfs_dir2_leaf_entry *ents,
>>>>>>> refs/remotes/origin/master
	int			index,		/* leaf table position */
	int			compact,	/* need to compact leaves */
	int			lowstale,	/* index of prev stale leaf */
	int			highstale,	/* index of next stale leaf */
	int			*lfloglow,	/* low leaf logging index */
	int			*lfloghigh)	/* high leaf logging index */
{
<<<<<<< HEAD
	if (!leaf->hdr.stale) {
=======
	if (!leafhdr->stale) {
>>>>>>> refs/remotes/origin/master
		xfs_dir2_leaf_entry_t	*lep;	/* leaf entry table pointer */

		/*
		 * Now we need to make room to insert the leaf entry.
		 *
		 * If there are no stale entries, just insert a hole at index.
		 */
<<<<<<< HEAD
		lep = &leaf->ents[index];
		if (index < be16_to_cpu(leaf->hdr.count))
			memmove(lep + 1, lep,
				(be16_to_cpu(leaf->hdr.count) - index) *
				 sizeof(*lep));
=======
		lep = &ents[index];
		if (index < leafhdr->count)
			memmove(lep + 1, lep,
				(leafhdr->count - index) * sizeof(*lep));
>>>>>>> refs/remotes/origin/master

		/*
		 * Record low and high logging indices for the leaf.
		 */
		*lfloglow = index;
<<<<<<< HEAD
		*lfloghigh = be16_to_cpu(leaf->hdr.count);
		be16_add_cpu(&leaf->hdr.count, 1);
=======
		*lfloghigh = leafhdr->count++;
>>>>>>> refs/remotes/origin/master
		return lep;
	}

	/*
	 * There are stale entries.
	 *
	 * We will use one of them for the new entry.  It's probably not at
	 * the right location, so we'll have to shift some up or down first.
	 *
	 * If we didn't compact before, we need to find the nearest stale
	 * entries before and after our insertion point.
	 */
	if (compact == 0)
<<<<<<< HEAD
		xfs_dir2_leaf_find_stale(leaf, index, &lowstale, &highstale);
=======
		xfs_dir3_leaf_find_stale(leafhdr, ents, index,
					 &lowstale, &highstale);
>>>>>>> refs/remotes/origin/master

	/*
	 * If the low one is better, use it.
	 */
	if (lowstale >= 0 &&
<<<<<<< HEAD
	    (highstale == be16_to_cpu(leaf->hdr.count) ||
	     index - lowstale - 1 < highstale - index)) {
		ASSERT(index - lowstale - 1 >= 0);
		ASSERT(leaf->ents[lowstale].address ==
=======
	    (highstale == leafhdr->count ||
	     index - lowstale - 1 < highstale - index)) {
		ASSERT(index - lowstale - 1 >= 0);
		ASSERT(ents[lowstale].address ==
>>>>>>> refs/remotes/origin/master
		       cpu_to_be32(XFS_DIR2_NULL_DATAPTR));

		/*
		 * Copy entries up to cover the stale entry and make room
		 * for the new entry.
		 */
		if (index - lowstale - 1 > 0) {
<<<<<<< HEAD
			memmove(&leaf->ents[lowstale],
				&leaf->ents[lowstale + 1],
				(index - lowstale - 1) *
				sizeof(xfs_dir2_leaf_entry_t));
		}
		*lfloglow = MIN(lowstale, *lfloglow);
		*lfloghigh = MAX(index - 1, *lfloghigh);
		be16_add_cpu(&leaf->hdr.stale, -1);
		return &leaf->ents[index - 1];
=======
			memmove(&ents[lowstale], &ents[lowstale + 1],
				(index - lowstale - 1) *
					sizeof(xfs_dir2_leaf_entry_t));
		}
		*lfloglow = MIN(lowstale, *lfloglow);
		*lfloghigh = MAX(index - 1, *lfloghigh);
		leafhdr->stale--;
		return &ents[index - 1];
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * The high one is better, so use that one.
	 */
	ASSERT(highstale - index >= 0);
<<<<<<< HEAD
	ASSERT(leaf->ents[highstale].address ==
	       cpu_to_be32(XFS_DIR2_NULL_DATAPTR));
=======
	ASSERT(ents[highstale].address == cpu_to_be32(XFS_DIR2_NULL_DATAPTR));
>>>>>>> refs/remotes/origin/master

	/*
	 * Copy entries down to cover the stale entry and make room for the
	 * new entry.
	 */
	if (highstale - index > 0) {
<<<<<<< HEAD
		memmove(&leaf->ents[index + 1],
			&leaf->ents[index],
=======
		memmove(&ents[index + 1], &ents[index],
>>>>>>> refs/remotes/origin/master
			(highstale - index) * sizeof(xfs_dir2_leaf_entry_t));
	}
	*lfloglow = MIN(index, *lfloglow);
	*lfloghigh = MAX(highstale, *lfloghigh);
<<<<<<< HEAD
	be16_add_cpu(&leaf->hdr.stale, -1);
	return &leaf->ents[index];
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	leafhdr->stale--;
	return &ents[index];
}

>>>>>>> refs/remotes/origin/master
/*
 * Add an entry to a leaf form directory.
 */
int						/* error */
xfs_dir2_leaf_addname(
	xfs_da_args_t		*args)		/* operation arguments */
{
	__be16			*bestsp;	/* freespace table in leaf */
	int			compact;	/* need to compact leaves */
<<<<<<< HEAD
<<<<<<< HEAD
	xfs_dir2_data_t		*data;		/* data block structure */
=======
	xfs_dir2_data_hdr_t	*hdr;		/* data block header */
>>>>>>> refs/remotes/origin/cm-10.0
	xfs_dabuf_t		*dbp;		/* data block buffer */
=======
	xfs_dir2_data_hdr_t	*hdr;		/* data block header */
	struct xfs_buf		*dbp;		/* data block buffer */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_data_entry_t	*dep;		/* data block entry */
	xfs_inode_t		*dp;		/* incore directory inode */
	xfs_dir2_data_unused_t	*dup;		/* data unused entry */
	int			error;		/* error return value */
	int			grown;		/* allocated new data block */
	int			highstale;	/* index of next stale leaf */
	int			i;		/* temporary, index */
	int			index;		/* leaf table position */
<<<<<<< HEAD
	xfs_dabuf_t		*lbp;		/* leaf's buffer */
=======
	struct xfs_buf		*lbp;		/* leaf's buffer */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	int			length;		/* length of new entry */
	xfs_dir2_leaf_entry_t	*lep;		/* leaf entry table pointer */
	int			lfloglow;	/* low leaf logging index */
	int			lfloghigh;	/* high leaf logging index */
	int			lowstale;	/* index of prev stale leaf */
	xfs_dir2_leaf_tail_t	*ltp;		/* leaf tail pointer */
	xfs_mount_t		*mp;		/* filesystem mount point */
	int			needbytes;	/* leaf block bytes needed */
	int			needlog;	/* need to log data header */
	int			needscan;	/* need to rescan data free */
	__be16			*tagp;		/* end of data entry */
	xfs_trans_t		*tp;		/* transaction pointer */
	xfs_dir2_db_t		use_block;	/* data block number */
<<<<<<< HEAD
=======
	struct xfs_dir2_data_free *bf;		/* bestfree table */
	struct xfs_dir2_leaf_entry *ents;
	struct xfs_dir3_icleaf_hdr leafhdr;
>>>>>>> refs/remotes/origin/master

	trace_xfs_dir2_leaf_addname(args);

	dp = args->dp;
	tp = args->trans;
	mp = dp->i_mount;
<<<<<<< HEAD
	/*
	 * Read the leaf block.
	 */
	error = xfs_da_read_buf(tp, dp, mp->m_dirleafblk, -1, &lbp,
		XFS_DATA_FORK);
	if (error) {
		return error;
	}
	ASSERT(lbp != NULL);
=======

	error = xfs_dir3_leaf_read(tp, dp, mp->m_dirleafblk, -1, &lbp);
	if (error)
		return error;

>>>>>>> refs/remotes/origin/master
	/*
	 * Look up the entry by hash value and name.
	 * We know it's not there, our caller has already done a lookup.
	 * So the index is of the entry to insert in front of.
	 * But if there are dup hash values the index is of the first of those.
	 */
	index = xfs_dir2_leaf_search_hash(args, lbp);
<<<<<<< HEAD
	leaf = lbp->data;
	ltp = xfs_dir2_leaf_tail_p(mp, leaf);
	bestsp = xfs_dir2_leaf_bests_p(ltp);
	length = xfs_dir2_data_entsize(args->namelen);
=======
	leaf = lbp->b_addr;
	ltp = xfs_dir2_leaf_tail_p(mp, leaf);
	ents = dp->d_ops->leaf_ents_p(leaf);
	dp->d_ops->leaf_hdr_from_disk(&leafhdr, leaf);
	bestsp = xfs_dir2_leaf_bests_p(ltp);
	length = dp->d_ops->data_entsize(args->namelen);

>>>>>>> refs/remotes/origin/master
	/*
	 * See if there are any entries with the same hash value
	 * and space in their block for the new entry.
	 * This is good because it puts multiple same-hash value entries
	 * in a data block, improving the lookup of those entries.
	 */
<<<<<<< HEAD
	for (use_block = -1, lep = &leaf->ents[index];
	     index < be16_to_cpu(leaf->hdr.count) && be32_to_cpu(lep->hashval) == args->hashval;
=======
	for (use_block = -1, lep = &ents[index];
	     index < leafhdr.count && be32_to_cpu(lep->hashval) == args->hashval;
>>>>>>> refs/remotes/origin/master
	     index++, lep++) {
		if (be32_to_cpu(lep->address) == XFS_DIR2_NULL_DATAPTR)
			continue;
		i = xfs_dir2_dataptr_to_db(mp, be32_to_cpu(lep->address));
		ASSERT(i < be32_to_cpu(ltp->bestcount));
<<<<<<< HEAD
<<<<<<< HEAD
		ASSERT(be16_to_cpu(bestsp[i]) != NULLDATAOFF);
=======
		ASSERT(bestsp[i] != cpu_to_be16(NULLDATAOFF));
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ASSERT(bestsp[i] != cpu_to_be16(NULLDATAOFF));
>>>>>>> refs/remotes/origin/master
		if (be16_to_cpu(bestsp[i]) >= length) {
			use_block = i;
			break;
		}
	}
	/*
	 * Didn't find a block yet, linear search all the data blocks.
	 */
	if (use_block == -1) {
		for (i = 0; i < be32_to_cpu(ltp->bestcount); i++) {
			/*
			 * Remember a block we see that's missing.
			 */
<<<<<<< HEAD
<<<<<<< HEAD
			if (be16_to_cpu(bestsp[i]) == NULLDATAOFF && use_block == -1)
=======
			if (bestsp[i] == cpu_to_be16(NULLDATAOFF) &&
			    use_block == -1)
>>>>>>> refs/remotes/origin/cm-10.0
=======
			if (bestsp[i] == cpu_to_be16(NULLDATAOFF) &&
			    use_block == -1)
>>>>>>> refs/remotes/origin/master
				use_block = i;
			else if (be16_to_cpu(bestsp[i]) >= length) {
				use_block = i;
				break;
			}
		}
	}
	/*
	 * How many bytes do we need in the leaf block?
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	needbytes =
		(leaf->hdr.stale ? 0 : (uint)sizeof(leaf->ents[0])) +
		(use_block != -1 ? 0 : (uint)sizeof(leaf->bests[0]));
=======
	needbytes = 0;
	if (!leaf->hdr.stale)
=======
	needbytes = 0;
	if (!leafhdr.stale)
>>>>>>> refs/remotes/origin/master
		needbytes += sizeof(xfs_dir2_leaf_entry_t);
	if (use_block == -1)
		needbytes += sizeof(xfs_dir2_data_off_t);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Now kill use_block if it refers to a missing block, so we
	 * can use it as an indication of allocation needed.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (use_block != -1 && be16_to_cpu(bestsp[use_block]) == NULLDATAOFF)
=======
	if (use_block != -1 && bestsp[use_block] == cpu_to_be16(NULLDATAOFF))
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (use_block != -1 && bestsp[use_block] == cpu_to_be16(NULLDATAOFF))
>>>>>>> refs/remotes/origin/master
		use_block = -1;
	/*
	 * If we don't have enough free bytes but we can make enough
	 * by compacting out stale entries, we'll do that.
	 */
<<<<<<< HEAD
	if ((char *)bestsp - (char *)&leaf->ents[be16_to_cpu(leaf->hdr.count)] <
				needbytes && be16_to_cpu(leaf->hdr.stale) > 1) {
		compact = 1;
	}
=======
	if ((char *)bestsp - (char *)&ents[leafhdr.count] < needbytes &&
	    leafhdr.stale > 1)
		compact = 1;

>>>>>>> refs/remotes/origin/master
	/*
	 * Otherwise if we don't have enough free bytes we need to
	 * convert to node form.
	 */
<<<<<<< HEAD
	else if ((char *)bestsp - (char *)&leaf->ents[be16_to_cpu(
						leaf->hdr.count)] < needbytes) {
=======
	else if ((char *)bestsp - (char *)&ents[leafhdr.count] < needbytes) {
>>>>>>> refs/remotes/origin/master
		/*
		 * Just checking or no space reservation, give up.
		 */
		if ((args->op_flags & XFS_DA_OP_JUSTCHECK) ||
							args->total == 0) {
<<<<<<< HEAD
			xfs_da_brelse(tp, lbp);
=======
			xfs_trans_brelse(tp, lbp);
>>>>>>> refs/remotes/origin/master
			return XFS_ERROR(ENOSPC);
		}
		/*
		 * Convert to node form.
		 */
		error = xfs_dir2_leaf_to_node(args, lbp);
<<<<<<< HEAD
		xfs_da_buf_done(lbp);
=======
>>>>>>> refs/remotes/origin/master
		if (error)
			return error;
		/*
		 * Then add the new entry.
		 */
		return xfs_dir2_node_addname(args);
	}
	/*
	 * Otherwise it will fit without compaction.
	 */
	else
		compact = 0;
	/*
	 * If just checking, then it will fit unless we needed to allocate
	 * a new data block.
	 */
	if (args->op_flags & XFS_DA_OP_JUSTCHECK) {
<<<<<<< HEAD
		xfs_da_brelse(tp, lbp);
=======
		xfs_trans_brelse(tp, lbp);
>>>>>>> refs/remotes/origin/master
		return use_block == -1 ? XFS_ERROR(ENOSPC) : 0;
	}
	/*
	 * If no allocations are allowed, return now before we've
	 * changed anything.
	 */
	if (args->total == 0 && use_block == -1) {
<<<<<<< HEAD
		xfs_da_brelse(tp, lbp);
=======
		xfs_trans_brelse(tp, lbp);
>>>>>>> refs/remotes/origin/master
		return XFS_ERROR(ENOSPC);
	}
	/*
	 * Need to compact the leaf entries, removing stale ones.
	 * Leave one stale entry behind - the one closest to our
	 * insertion index - and we'll shift that one to our insertion
	 * point later.
	 */
	if (compact) {
<<<<<<< HEAD
		xfs_dir2_leaf_compact_x1(lbp, &index, &lowstale, &highstale,
			&lfloglow, &lfloghigh);
=======
		xfs_dir3_leaf_compact_x1(&leafhdr, ents, &index, &lowstale,
			&highstale, &lfloglow, &lfloghigh);
>>>>>>> refs/remotes/origin/master
	}
	/*
	 * There are stale entries, so we'll need log-low and log-high
	 * impossibly bad values later.
	 */
<<<<<<< HEAD
	else if (be16_to_cpu(leaf->hdr.stale)) {
		lfloglow = be16_to_cpu(leaf->hdr.count);
=======
	else if (leafhdr.stale) {
		lfloglow = leafhdr.count;
>>>>>>> refs/remotes/origin/master
		lfloghigh = -1;
	}
	/*
	 * If there was no data block space found, we need to allocate
	 * a new one.
	 */
	if (use_block == -1) {
		/*
		 * Add the new data block.
		 */
		if ((error = xfs_dir2_grow_inode(args, XFS_DIR2_DATA_SPACE,
				&use_block))) {
<<<<<<< HEAD
			xfs_da_brelse(tp, lbp);
=======
			xfs_trans_brelse(tp, lbp);
>>>>>>> refs/remotes/origin/master
			return error;
		}
		/*
		 * Initialize the block.
		 */
<<<<<<< HEAD
		if ((error = xfs_dir2_data_init(args, use_block, &dbp))) {
			xfs_da_brelse(tp, lbp);
=======
		if ((error = xfs_dir3_data_init(args, use_block, &dbp))) {
			xfs_trans_brelse(tp, lbp);
>>>>>>> refs/remotes/origin/master
			return error;
		}
		/*
		 * If we're adding a new data block on the end we need to
		 * extend the bests table.  Copy it up one entry.
		 */
		if (use_block >= be32_to_cpu(ltp->bestcount)) {
			bestsp--;
			memmove(&bestsp[0], &bestsp[1],
				be32_to_cpu(ltp->bestcount) * sizeof(bestsp[0]));
			be32_add_cpu(&ltp->bestcount, 1);
<<<<<<< HEAD
			xfs_dir2_leaf_log_tail(tp, lbp);
			xfs_dir2_leaf_log_bests(tp, lbp, 0, be32_to_cpu(ltp->bestcount) - 1);
=======
			xfs_dir3_leaf_log_tail(tp, lbp);
			xfs_dir3_leaf_log_bests(tp, lbp, 0, be32_to_cpu(ltp->bestcount) - 1);
>>>>>>> refs/remotes/origin/master
		}
		/*
		 * If we're filling in a previously empty block just log it.
		 */
		else
<<<<<<< HEAD
			xfs_dir2_leaf_log_bests(tp, lbp, use_block, use_block);
<<<<<<< HEAD
		data = dbp->data;
		bestsp[use_block] = data->hdr.bestfree[0].length;
=======
		hdr = dbp->data;
		bestsp[use_block] = hdr->bestfree[0].length;
>>>>>>> refs/remotes/origin/cm-10.0
		grown = 1;
	}
	/*
	 * Already had space in some data block.
	 * Just read that one in.
	 */
	else {
		if ((error =
		    xfs_da_read_buf(tp, dp, xfs_dir2_db_to_da(mp, use_block),
			    -1, &dbp, XFS_DATA_FORK))) {
			xfs_da_brelse(tp, lbp);
			return error;
		}
<<<<<<< HEAD
		data = dbp->data;
=======
		hdr = dbp->data;
>>>>>>> refs/remotes/origin/cm-10.0
		grown = 0;
	}
	xfs_dir2_data_check(dp, dbp);
=======
			xfs_dir3_leaf_log_bests(tp, lbp, use_block, use_block);
		hdr = dbp->b_addr;
		bf = dp->d_ops->data_bestfree_p(hdr);
		bestsp[use_block] = bf[0].length;
		grown = 1;
	} else {
		/*
		 * Already had space in some data block.
		 * Just read that one in.
		 */
		error = xfs_dir3_data_read(tp, dp,
					   xfs_dir2_db_to_da(mp, use_block),
					   -1, &dbp);
		if (error) {
			xfs_trans_brelse(tp, lbp);
			return error;
		}
		hdr = dbp->b_addr;
		bf = dp->d_ops->data_bestfree_p(hdr);
		grown = 0;
	}
>>>>>>> refs/remotes/origin/master
	/*
	 * Point to the biggest freespace in our data block.
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
	ASSERT(be16_to_cpu(dup->length) >= length);
	needscan = needlog = 0;
	/*
	 * Mark the initial part of our freespace in use for the new entry.
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
	 * Initialize our new entry (at last).
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
=======
	dp->d_ops->data_put_ftype(dep, args->filetype);
	tagp = dp->d_ops->data_entry_tag_p(dep);
	*tagp = cpu_to_be16((char *)dep - (char *)hdr);
>>>>>>> refs/remotes/origin/master
	/*
	 * Need to scan fix up the bestfree table.
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
	 * Need to log the data block's header.
	 */
	if (needlog)
<<<<<<< HEAD
		xfs_dir2_data_log_header(tp, dbp);
	xfs_dir2_data_log_entry(tp, dbp, dep);
=======
		xfs_dir2_data_log_header(tp, dp, dbp);
	xfs_dir2_data_log_entry(tp, dp, dbp, dep);
>>>>>>> refs/remotes/origin/master
	/*
	 * If the bests table needs to be changed, do it.
	 * Log the change unless we've already done that.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (be16_to_cpu(bestsp[use_block]) != be16_to_cpu(data->hdr.bestfree[0].length)) {
		bestsp[use_block] = data->hdr.bestfree[0].length;
		if (!grown)
			xfs_dir2_leaf_log_bests(tp, lbp, use_block, use_block);
	}
	/*
	 * Now we need to make room to insert the leaf entry.
	 * If there are no stale entries, we just insert a hole at index.
	 */
	if (!leaf->hdr.stale) {
		/*
		 * lep is still good as the index leaf entry.
		 */
		if (index < be16_to_cpu(leaf->hdr.count))
			memmove(lep + 1, lep,
				(be16_to_cpu(leaf->hdr.count) - index) * sizeof(*lep));
		/*
		 * Record low and high logging indices for the leaf.
		 */
		lfloglow = index;
		lfloghigh = be16_to_cpu(leaf->hdr.count);
		be16_add_cpu(&leaf->hdr.count, 1);
	}
	/*
	 * There are stale entries.
	 * We will use one of them for the new entry.
	 * It's probably not at the right location, so we'll have to
	 * shift some up or down first.
	 */
	else {
		/*
		 * If we didn't compact before, we need to find the nearest
		 * stale entries before and after our insertion point.
		 */
		if (compact == 0) {
			/*
			 * Find the first stale entry before the insertion
			 * point, if any.
			 */
			for (lowstale = index - 1;
			     lowstale >= 0 &&
				be32_to_cpu(leaf->ents[lowstale].address) !=
				XFS_DIR2_NULL_DATAPTR;
			     lowstale--)
				continue;
			/*
			 * Find the next stale entry at or after the insertion
			 * point, if any.   Stop if we go so far that the
			 * lowstale entry would be better.
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
		 * If the low one is better, use it.
		 */
		if (lowstale >= 0 &&
		    (highstale == be16_to_cpu(leaf->hdr.count) ||
		     index - lowstale - 1 < highstale - index)) {
			ASSERT(index - lowstale - 1 >= 0);
			ASSERT(be32_to_cpu(leaf->ents[lowstale].address) ==
			       XFS_DIR2_NULL_DATAPTR);
			/*
			 * Copy entries up to cover the stale entry
			 * and make room for the new entry.
			 */
			if (index - lowstale - 1 > 0)
				memmove(&leaf->ents[lowstale],
					&leaf->ents[lowstale + 1],
					(index - lowstale - 1) * sizeof(*lep));
			lep = &leaf->ents[index - 1];
			lfloglow = MIN(lowstale, lfloglow);
			lfloghigh = MAX(index - 1, lfloghigh);
		}
		/*
		 * The high one is better, so use that one.
		 */
		else {
			ASSERT(highstale - index >= 0);
			ASSERT(be32_to_cpu(leaf->ents[highstale].address) ==
			       XFS_DIR2_NULL_DATAPTR);
			/*
			 * Copy entries down to cover the stale entry
			 * and make room for the new entry.
			 */
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
=======
	if (be16_to_cpu(bestsp[use_block]) != be16_to_cpu(hdr->bestfree[0].length)) {
		bestsp[use_block] = hdr->bestfree[0].length;
		if (!grown)
			xfs_dir2_leaf_log_bests(tp, lbp, use_block, use_block);
	}

	lep = xfs_dir2_leaf_find_entry(leaf, index, compact, lowstale,
				       highstale, &lfloglow, &lfloghigh);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (be16_to_cpu(bestsp[use_block]) != be16_to_cpu(bf[0].length)) {
		bestsp[use_block] = bf[0].length;
		if (!grown)
			xfs_dir3_leaf_log_bests(tp, lbp, use_block, use_block);
	}

	lep = xfs_dir3_leaf_find_entry(&leafhdr, ents, index, compact, lowstale,
				       highstale, &lfloglow, &lfloghigh);

>>>>>>> refs/remotes/origin/master
	/*
	 * Fill in the new leaf entry.
	 */
	lep->hashval = cpu_to_be32(args->hashval);
	lep->address = cpu_to_be32(xfs_dir2_db_off_to_dataptr(mp, use_block,
				be16_to_cpu(*tagp)));
	/*
	 * Log the leaf fields and give up the buffers.
	 */
<<<<<<< HEAD
	xfs_dir2_leaf_log_header(tp, lbp);
	xfs_dir2_leaf_log_ents(tp, lbp, lfloglow, lfloghigh);
	xfs_dir2_leaf_check(dp, lbp);
	xfs_da_buf_done(lbp);
	xfs_dir2_data_check(dp, dbp);
	xfs_da_buf_done(dbp);
	return 0;
}

#ifdef DEBUG
/*
 * Check the internal consistency of a leaf1 block.
 * Pop an assert if something is wrong.
 */
STATIC void
xfs_dir2_leaf_check(
	xfs_inode_t		*dp,		/* incore directory inode */
	xfs_dabuf_t		*bp)		/* leaf's buffer */
{
	int			i;		/* leaf index */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	xfs_dir2_leaf_tail_t	*ltp;		/* leaf tail pointer */
	xfs_mount_t		*mp;		/* filesystem mount point */
	int			stale;		/* count of stale leaves */

	leaf = bp->data;
	mp = dp->i_mount;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_DIR2_LEAF1_MAGIC);
=======
	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAF1_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * This value is not restrictive enough.
	 * Should factor in the size of the bests table as well.
	 * We can deduce a value for that from di_size.
	 */
	ASSERT(be16_to_cpu(leaf->hdr.count) <= xfs_dir2_max_leaf_ents(mp));
	ltp = xfs_dir2_leaf_tail_p(mp, leaf);
	/*
	 * Leaves and bests don't overlap.
	 */
	ASSERT((char *)&leaf->ents[be16_to_cpu(leaf->hdr.count)] <=
	       (char *)xfs_dir2_leaf_bests_p(ltp));
	/*
	 * Check hash value order, count stale entries.
	 */
	for (i = stale = 0; i < be16_to_cpu(leaf->hdr.count); i++) {
		if (i + 1 < be16_to_cpu(leaf->hdr.count))
			ASSERT(be32_to_cpu(leaf->ents[i].hashval) <=
			       be32_to_cpu(leaf->ents[i + 1].hashval));
<<<<<<< HEAD
		if (be32_to_cpu(leaf->ents[i].address) == XFS_DIR2_NULL_DATAPTR)
=======
		if (leaf->ents[i].address == cpu_to_be32(XFS_DIR2_NULL_DATAPTR))
>>>>>>> refs/remotes/origin/cm-10.0
			stale++;
	}
	ASSERT(be16_to_cpu(leaf->hdr.stale) == stale);
}
#endif	/* DEBUG */

=======
	dp->d_ops->leaf_hdr_to_disk(leaf, &leafhdr);
	xfs_dir3_leaf_log_header(tp, dp, lbp);
	xfs_dir3_leaf_log_ents(tp, dp, lbp, lfloglow, lfloghigh);
	xfs_dir3_leaf_check(dp, lbp);
	xfs_dir3_data_check(dp, dbp);
	return 0;
}

>>>>>>> refs/remotes/origin/master
/*
 * Compact out any stale entries in the leaf.
 * Log the header and changed leaf entries, if any.
 */
void
<<<<<<< HEAD
xfs_dir2_leaf_compact(
	xfs_da_args_t	*args,		/* operation arguments */
	xfs_dabuf_t	*bp)		/* leaf buffer */
=======
xfs_dir3_leaf_compact(
	xfs_da_args_t	*args,		/* operation arguments */
	struct xfs_dir3_icleaf_hdr *leafhdr,
	struct xfs_buf	*bp)		/* leaf buffer */
>>>>>>> refs/remotes/origin/master
{
	int		from;		/* source leaf index */
	xfs_dir2_leaf_t	*leaf;		/* leaf structure */
	int		loglow;		/* first leaf entry to log */
	int		to;		/* target leaf index */
<<<<<<< HEAD

	leaf = bp->data;
	if (!leaf->hdr.stale) {
		return;
	}
	/*
	 * Compress out the stale entries in place.
	 */
	for (from = to = 0, loglow = -1; from < be16_to_cpu(leaf->hdr.count); from++) {
<<<<<<< HEAD
		if (be32_to_cpu(leaf->ents[from].address) == XFS_DIR2_NULL_DATAPTR)
=======
		if (leaf->ents[from].address ==
		    cpu_to_be32(XFS_DIR2_NULL_DATAPTR))
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct xfs_dir2_leaf_entry *ents;
	struct xfs_inode *dp = args->dp;

	leaf = bp->b_addr;
	if (!leafhdr->stale)
		return;

	/*
	 * Compress out the stale entries in place.
	 */
	ents = dp->d_ops->leaf_ents_p(leaf);
	for (from = to = 0, loglow = -1; from < leafhdr->count; from++) {
		if (ents[from].address == cpu_to_be32(XFS_DIR2_NULL_DATAPTR))
>>>>>>> refs/remotes/origin/master
			continue;
		/*
		 * Only actually copy the entries that are different.
		 */
		if (from > to) {
			if (loglow == -1)
				loglow = to;
<<<<<<< HEAD
			leaf->ents[to] = leaf->ents[from];
=======
			ents[to] = ents[from];
>>>>>>> refs/remotes/origin/master
		}
		to++;
	}
	/*
	 * Update and log the header, log the leaf entries.
	 */
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf->hdr.stale) == from - to);
	be16_add_cpu(&leaf->hdr.count, -(be16_to_cpu(leaf->hdr.stale)));
	leaf->hdr.stale = 0;
	xfs_dir2_leaf_log_header(args->trans, bp);
	if (loglow != -1)
		xfs_dir2_leaf_log_ents(args->trans, bp, loglow, to - 1);
=======
	ASSERT(leafhdr->stale == from - to);
	leafhdr->count -= leafhdr->stale;
	leafhdr->stale = 0;

	dp->d_ops->leaf_hdr_to_disk(leaf, leafhdr);
	xfs_dir3_leaf_log_header(args->trans, dp, bp);
	if (loglow != -1)
		xfs_dir3_leaf_log_ents(args->trans, dp, bp, loglow, to - 1);
>>>>>>> refs/remotes/origin/master
}

/*
 * Compact the leaf entries, removing stale ones.
 * Leave one stale entry behind - the one closest to our
 * insertion index - and the caller will shift that one to our insertion
 * point later.
 * Return new insertion index, where the remaining stale entry is,
 * and leaf logging indices.
 */
void
<<<<<<< HEAD
xfs_dir2_leaf_compact_x1(
	xfs_dabuf_t	*bp,		/* leaf buffer */
=======
xfs_dir3_leaf_compact_x1(
	struct xfs_dir3_icleaf_hdr *leafhdr,
	struct xfs_dir2_leaf_entry *ents,
>>>>>>> refs/remotes/origin/master
	int		*indexp,	/* insertion index */
	int		*lowstalep,	/* out: stale entry before us */
	int		*highstalep,	/* out: stale entry after us */
	int		*lowlogp,	/* out: low log index */
	int		*highlogp)	/* out: high log index */
{
	int		from;		/* source copy index */
	int		highstale;	/* stale entry at/after index */
	int		index;		/* insertion index */
	int		keepstale;	/* source index of kept stale */
<<<<<<< HEAD
	xfs_dir2_leaf_t	*leaf;		/* leaf structure */
=======
>>>>>>> refs/remotes/origin/master
	int		lowstale;	/* stale entry before index */
	int		newindex=0;	/* new insertion index */
	int		to;		/* destination copy index */

<<<<<<< HEAD
	leaf = bp->data;
	ASSERT(be16_to_cpu(leaf->hdr.stale) > 1);
	index = *indexp;
<<<<<<< HEAD
	/*
	 * Find the first stale entry before our index, if any.
	 */
	for (lowstale = index - 1;
	     lowstale >= 0 &&
		be32_to_cpu(leaf->ents[lowstale].address) != XFS_DIR2_NULL_DATAPTR;
	     lowstale--)
		continue;
	/*
	 * Find the first stale entry at or after our index, if any.
	 * Stop if the answer would be worse than lowstale.
	 */
	for (highstale = index;
	     highstale < be16_to_cpu(leaf->hdr.count) &&
		be32_to_cpu(leaf->ents[highstale].address) != XFS_DIR2_NULL_DATAPTR &&
		(lowstale < 0 || index - lowstale > highstale - index);
	     highstale++)
		continue;
=======

	xfs_dir2_leaf_find_stale(leaf, index, &lowstale, &highstale);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	ASSERT(leafhdr->stale > 1);
	index = *indexp;

	xfs_dir3_leaf_find_stale(leafhdr, ents, index, &lowstale, &highstale);

>>>>>>> refs/remotes/origin/master
	/*
	 * Pick the better of lowstale and highstale.
	 */
	if (lowstale >= 0 &&
<<<<<<< HEAD
	    (highstale == be16_to_cpu(leaf->hdr.count) ||
=======
	    (highstale == leafhdr->count ||
>>>>>>> refs/remotes/origin/master
	     index - lowstale <= highstale - index))
		keepstale = lowstale;
	else
		keepstale = highstale;
	/*
	 * Copy the entries in place, removing all the stale entries
	 * except keepstale.
	 */
<<<<<<< HEAD
	for (from = to = 0; from < be16_to_cpu(leaf->hdr.count); from++) {
=======
	for (from = to = 0; from < leafhdr->count; from++) {
>>>>>>> refs/remotes/origin/master
		/*
		 * Notice the new value of index.
		 */
		if (index == from)
			newindex = to;
		if (from != keepstale &&
<<<<<<< HEAD
<<<<<<< HEAD
		    be32_to_cpu(leaf->ents[from].address) == XFS_DIR2_NULL_DATAPTR) {
=======
		    leaf->ents[from].address ==
		    cpu_to_be32(XFS_DIR2_NULL_DATAPTR)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		    ents[from].address == cpu_to_be32(XFS_DIR2_NULL_DATAPTR)) {
>>>>>>> refs/remotes/origin/master
			if (from == to)
				*lowlogp = to;
			continue;
		}
		/*
		 * Record the new keepstale value for the insertion.
		 */
		if (from == keepstale)
			lowstale = highstale = to;
		/*
		 * Copy only the entries that have moved.
		 */
		if (from > to)
<<<<<<< HEAD
			leaf->ents[to] = leaf->ents[from];
=======
			ents[to] = ents[from];
>>>>>>> refs/remotes/origin/master
		to++;
	}
	ASSERT(from > to);
	/*
	 * If the insertion point was past the last entry,
	 * set the new insertion point accordingly.
	 */
	if (index == from)
		newindex = to;
	*indexp = newindex;
	/*
	 * Adjust the leaf header values.
	 */
<<<<<<< HEAD
	be16_add_cpu(&leaf->hdr.count, -(from - to));
	leaf->hdr.stale = cpu_to_be16(1);
=======
	leafhdr->count -= from - to;
	leafhdr->stale = 1;
>>>>>>> refs/remotes/origin/master
	/*
	 * Remember the low/high stale value only in the "right"
	 * direction.
	 */
	if (lowstale >= newindex)
		lowstale = -1;
	else
<<<<<<< HEAD
		highstale = be16_to_cpu(leaf->hdr.count);
	*highlogp = be16_to_cpu(leaf->hdr.count) - 1;
=======
		highstale = leafhdr->count;
	*highlogp = leafhdr->count - 1;
>>>>>>> refs/remotes/origin/master
	*lowstalep = lowstale;
	*highstalep = highstale;
}

/*
<<<<<<< HEAD
 * Getdents (readdir) for leaf and node directories.
 * This reads the data blocks only, so is the same for both forms.
 */
int						/* error */
xfs_dir2_leaf_getdents(
	xfs_inode_t		*dp,		/* incore directory inode */
	void			*dirent,
	size_t			bufsize,
	xfs_off_t		*offset,
	filldir_t		filldir)
{
	xfs_dabuf_t		*bp;		/* data block buffer */
	int			byteoff;	/* offset in current block */
	xfs_dir2_db_t		curdb;		/* db for current block */
	xfs_dir2_off_t		curoff;		/* current overall offset */
<<<<<<< HEAD
	xfs_dir2_data_t		*data;		/* data block structure */
=======
	xfs_dir2_data_hdr_t	*hdr;		/* data block header */
>>>>>>> refs/remotes/origin/cm-10.0
	xfs_dir2_data_entry_t	*dep;		/* data entry */
	xfs_dir2_data_unused_t	*dup;		/* unused entry */
	int			error = 0;	/* error return value */
	int			i;		/* temporary loop index */
	int			j;		/* temporary loop index */
	int			length;		/* temporary length value */
	xfs_bmbt_irec_t		*map;		/* map vector for blocks */
	xfs_extlen_t		map_blocks;	/* number of fsbs in map */
	xfs_dablk_t		map_off;	/* last mapped file offset */
	int			map_size;	/* total entries in *map */
	int			map_valid;	/* valid entries in *map */
	xfs_mount_t		*mp;		/* filesystem mount point */
	xfs_dir2_off_t		newoff;		/* new curoff after new blk */
	int			nmap;		/* mappings to ask xfs_bmapi */
	char			*ptr = NULL;	/* pointer to current data */
	int			ra_current;	/* number of read-ahead blks */
	int			ra_index;	/* *map index for read-ahead */
	int			ra_offset;	/* map entry offset for ra */
	int			ra_want;	/* readahead count wanted */

	/*
	 * If the offset is at or past the largest allowed value,
	 * give up right away.
	 */
	if (*offset >= XFS_DIR2_MAX_DATAPTR)
		return 0;

	mp = dp->i_mount;

	/*
	 * Set up to bmap a number of blocks based on the caller's
	 * buffer size, the directory block size, and the filesystem
	 * block size.
	 */
	map_size = howmany(bufsize + mp->m_dirblksize, mp->m_sb.sb_blocksize);
	map = kmem_alloc(map_size * sizeof(*map), KM_SLEEP);
	map_valid = ra_index = ra_offset = ra_current = map_blocks = 0;
	bp = NULL;

	/*
	 * Inside the loop we keep the main offset value as a byte offset
	 * in the directory file.
	 */
	curoff = xfs_dir2_dataptr_to_byte(mp, *offset);

	/*
	 * Force this conversion through db so we truncate the offset
	 * down to get the start of the data block.
	 */
	map_off = xfs_dir2_db_to_da(mp, xfs_dir2_byte_to_db(mp, curoff));
	/*
	 * Loop over directory entries until we reach the end offset.
	 * Get more blocks and readahead as necessary.
	 */
	while (curoff < XFS_DIR2_LEAF_OFFSET) {
		/*
		 * If we have no buffer, or we're off the end of the
		 * current buffer, need to get another one.
		 */
		if (!bp || ptr >= (char *)bp->data + mp->m_dirblksize) {
			/*
			 * If we have a buffer, we need to release it and
			 * take it out of the mapping.
			 */
			if (bp) {
				xfs_da_brelse(NULL, bp);
				bp = NULL;
				map_blocks -= mp->m_dirblkfsbs;
				/*
				 * Loop to get rid of the extents for the
				 * directory block.
				 */
				for (i = mp->m_dirblkfsbs; i > 0; ) {
					j = MIN((int)map->br_blockcount, i);
					map->br_blockcount -= j;
					map->br_startblock += j;
					map->br_startoff += j;
					/*
					 * If mapping is done, pitch it from
					 * the table.
					 */
					if (!map->br_blockcount && --map_valid)
						memmove(&map[0], &map[1],
							sizeof(map[0]) *
							map_valid);
					i -= j;
				}
			}
			/*
			 * Recalculate the readahead blocks wanted.
			 */
			ra_want = howmany(bufsize + mp->m_dirblksize,
					  mp->m_sb.sb_blocksize) - 1;
			ASSERT(ra_want >= 0);

			/*
			 * If we don't have as many as we want, and we haven't
			 * run out of data blocks, get some more mappings.
			 */
			if (1 + ra_want > map_blocks &&
			    map_off <
			    xfs_dir2_byte_to_da(mp, XFS_DIR2_LEAF_OFFSET)) {
				/*
				 * Get more bmaps, fill in after the ones
				 * we already have in the table.
				 */
				nmap = map_size - map_valid;
<<<<<<< HEAD
				error = xfs_bmapi(NULL, dp,
					map_off,
					xfs_dir2_byte_to_da(mp,
						XFS_DIR2_LEAF_OFFSET) - map_off,
					XFS_BMAPI_METADATA, NULL, 0,
					&map[map_valid], &nmap, NULL);
=======
				error = xfs_bmapi_read(dp, map_off,
					xfs_dir2_byte_to_da(mp,
						XFS_DIR2_LEAF_OFFSET) - map_off,
					&map[map_valid], &nmap, 0);
>>>>>>> refs/remotes/origin/cm-10.0
				/*
				 * Don't know if we should ignore this or
				 * try to return an error.
				 * The trouble with returning errors
				 * is that readdir will just stop without
				 * actually passing the error through.
				 */
				if (error)
					break;	/* XXX */
				/*
				 * If we got all the mappings we asked for,
				 * set the final map offset based on the
				 * last bmap value received.
				 * Otherwise, we've reached the end.
				 */
				if (nmap == map_size - map_valid)
					map_off =
					map[map_valid + nmap - 1].br_startoff +
					map[map_valid + nmap - 1].br_blockcount;
				else
					map_off =
						xfs_dir2_byte_to_da(mp,
							XFS_DIR2_LEAF_OFFSET);
				/*
				 * Look for holes in the mapping, and
				 * eliminate them.  Count up the valid blocks.
				 */
				for (i = map_valid; i < map_valid + nmap; ) {
					if (map[i].br_startblock ==
					    HOLESTARTBLOCK) {
						nmap--;
						length = map_valid + nmap - i;
						if (length)
							memmove(&map[i],
								&map[i + 1],
								sizeof(map[i]) *
								length);
					} else {
						map_blocks +=
							map[i].br_blockcount;
						i++;
					}
				}
				map_valid += nmap;
			}
			/*
			 * No valid mappings, so no more data blocks.
			 */
			if (!map_valid) {
				curoff = xfs_dir2_da_to_byte(mp, map_off);
				break;
			}
			/*
			 * Read the directory block starting at the first
			 * mapping.
			 */
			curdb = xfs_dir2_da_to_db(mp, map->br_startoff);
			error = xfs_da_read_buf(NULL, dp, map->br_startoff,
				map->br_blockcount >= mp->m_dirblkfsbs ?
				    XFS_FSB_TO_DADDR(mp, map->br_startblock) :
				    -1,
				&bp, XFS_DATA_FORK);
			/*
			 * Should just skip over the data block instead
			 * of giving up.
			 */
			if (error)
				break;	/* XXX */
			/*
			 * Adjust the current amount of read-ahead: we just
			 * read a block that was previously ra.
			 */
			if (ra_current)
				ra_current -= mp->m_dirblkfsbs;
			/*
			 * Do we need more readahead?
			 */
			for (ra_index = ra_offset = i = 0;
			     ra_want > ra_current && i < map_blocks;
			     i += mp->m_dirblkfsbs) {
				ASSERT(ra_index < map_valid);
				/*
				 * Read-ahead a contiguous directory block.
				 */
				if (i > ra_current &&
				    map[ra_index].br_blockcount >=
				    mp->m_dirblkfsbs) {
					xfs_buf_readahead(mp->m_ddev_targp,
						XFS_FSB_TO_DADDR(mp,
						   map[ra_index].br_startblock +
						   ra_offset),
						(int)BTOBB(mp->m_dirblksize));
					ra_current = i;
				}
				/*
				 * Read-ahead a non-contiguous directory block.
				 * This doesn't use our mapping, but this
				 * is a very rare case.
				 */
				else if (i > ra_current) {
					(void)xfs_da_reada_buf(NULL, dp,
						map[ra_index].br_startoff +
						ra_offset, XFS_DATA_FORK);
					ra_current = i;
				}
				/*
				 * Advance offset through the mapping table.
				 */
				for (j = 0; j < mp->m_dirblkfsbs; j++) {
					/*
					 * The rest of this extent but not
					 * more than a dir block.
					 */
					length = MIN(mp->m_dirblkfsbs,
						(int)(map[ra_index].br_blockcount -
						ra_offset));
					j += length;
					ra_offset += length;
					/*
					 * Advance to the next mapping if
					 * this one is used up.
					 */
					if (ra_offset ==
					    map[ra_index].br_blockcount) {
						ra_offset = 0;
						ra_index++;
					}
				}
			}
			/*
			 * Having done a read, we need to set a new offset.
			 */
			newoff = xfs_dir2_db_off_to_byte(mp, curdb, 0);
			/*
			 * Start of the current block.
			 */
			if (curoff < newoff)
				curoff = newoff;
			/*
			 * Make sure we're in the right block.
			 */
			else if (curoff > newoff)
				ASSERT(xfs_dir2_byte_to_db(mp, curoff) ==
				       curdb);
<<<<<<< HEAD
			data = bp->data;
=======
			hdr = bp->data;
>>>>>>> refs/remotes/origin/cm-10.0
			xfs_dir2_data_check(dp, bp);
			/*
			 * Find our position in the block.
			 */
<<<<<<< HEAD
			ptr = (char *)&data->u;
=======
			ptr = (char *)(hdr + 1);
>>>>>>> refs/remotes/origin/cm-10.0
			byteoff = xfs_dir2_byte_to_off(mp, curoff);
			/*
			 * Skip past the header.
			 */
			if (byteoff == 0)
<<<<<<< HEAD
				curoff += (uint)sizeof(data->hdr);
=======
				curoff += (uint)sizeof(*hdr);
>>>>>>> refs/remotes/origin/cm-10.0
			/*
			 * Skip past entries until we reach our offset.
			 */
			else {
<<<<<<< HEAD
				while ((char *)ptr - (char *)data < byteoff) {
=======
				while ((char *)ptr - (char *)hdr < byteoff) {
>>>>>>> refs/remotes/origin/cm-10.0
					dup = (xfs_dir2_data_unused_t *)ptr;

					if (be16_to_cpu(dup->freetag)
						  == XFS_DIR2_DATA_FREE_TAG) {

						length = be16_to_cpu(dup->length);
						ptr += length;
						continue;
					}
					dep = (xfs_dir2_data_entry_t *)ptr;
					length =
					   xfs_dir2_data_entsize(dep->namelen);
					ptr += length;
				}
				/*
				 * Now set our real offset.
				 */
				curoff =
					xfs_dir2_db_off_to_byte(mp,
					    xfs_dir2_byte_to_db(mp, curoff),
<<<<<<< HEAD
					    (char *)ptr - (char *)data);
				if (ptr >= (char *)data + mp->m_dirblksize) {
=======
					    (char *)ptr - (char *)hdr);
				if (ptr >= (char *)hdr + mp->m_dirblksize) {
>>>>>>> refs/remotes/origin/cm-10.0
					continue;
				}
			}
		}
		/*
		 * We have a pointer to an entry.
		 * Is it a live one?
		 */
		dup = (xfs_dir2_data_unused_t *)ptr;
		/*
		 * No, it's unused, skip over it.
		 */
		if (be16_to_cpu(dup->freetag) == XFS_DIR2_DATA_FREE_TAG) {
			length = be16_to_cpu(dup->length);
			ptr += length;
			curoff += length;
			continue;
		}

		dep = (xfs_dir2_data_entry_t *)ptr;
		length = xfs_dir2_data_entsize(dep->namelen);

		if (filldir(dirent, (char *)dep->name, dep->namelen,
			    xfs_dir2_byte_to_dataptr(mp, curoff) & 0x7fffffff,
			    be64_to_cpu(dep->inumber), DT_UNKNOWN))
			break;

		/*
		 * Advance to next entry in the block.
		 */
		ptr += length;
		curoff += length;
		/* bufsize may have just been a guess; don't go negative */
		bufsize = bufsize > length ? bufsize - length : 0;
	}

	/*
	 * All done.  Set output offset value to current offset.
	 */
	if (curoff > xfs_dir2_dataptr_to_byte(mp, XFS_DIR2_MAX_DATAPTR))
		*offset = XFS_DIR2_MAX_DATAPTR & 0x7fffffff;
	else
		*offset = xfs_dir2_byte_to_dataptr(mp, curoff) & 0x7fffffff;
	kmem_free(map);
	if (bp)
		xfs_da_brelse(NULL, bp);
	return error;
}

/*
 * Initialize a new leaf block, leaf1 or leafn magic accepted.
 */
int
xfs_dir2_leaf_init(
	xfs_da_args_t		*args,		/* operation arguments */
	xfs_dir2_db_t		bno,		/* directory block number */
	xfs_dabuf_t		**bpp,		/* out: leaf buffer */
	int			magic)		/* magic number for block */
{
	xfs_dabuf_t		*bp;		/* leaf buffer */
	xfs_inode_t		*dp;		/* incore directory inode */
	int			error;		/* error return code */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	xfs_dir2_leaf_tail_t	*ltp;		/* leaf tail structure */
	xfs_mount_t		*mp;		/* filesystem mount point */
	xfs_trans_t		*tp;		/* transaction pointer */

	dp = args->dp;
	ASSERT(dp != NULL);
	tp = args->trans;
	mp = dp->i_mount;
	ASSERT(bno >= XFS_DIR2_LEAF_FIRSTDB(mp) &&
	       bno < XFS_DIR2_FREE_FIRSTDB(mp));
	/*
	 * Get the buffer for the block.
	 */
	error = xfs_da_get_buf(tp, dp, xfs_dir2_db_to_da(mp, bno), -1, &bp,
		XFS_DATA_FORK);
	if (error) {
		return error;
	}
	ASSERT(bp != NULL);
	leaf = bp->data;
	/*
	 * Initialize the header.
	 */
	leaf->hdr.info.magic = cpu_to_be16(magic);
	leaf->hdr.info.forw = 0;
	leaf->hdr.info.back = 0;
	leaf->hdr.count = 0;
	leaf->hdr.stale = 0;
	xfs_dir2_leaf_log_header(tp, bp);
	/*
	 * If it's a leaf-format directory initialize the tail.
	 * In this case our caller has the real bests table to copy into
	 * the block.
	 */
	if (magic == XFS_DIR2_LEAF1_MAGIC) {
		ltp = xfs_dir2_leaf_tail_p(mp, leaf);
		ltp->bestcount = 0;
		xfs_dir2_leaf_log_tail(tp, bp);
	}
	*bpp = bp;
	return 0;
}

/*
 * Log the bests entries indicated from a leaf1 block.
 */
static void
xfs_dir2_leaf_log_bests(
	xfs_trans_t		*tp,		/* transaction pointer */
	xfs_dabuf_t		*bp,		/* leaf buffer */
=======
 * Log the bests entries indicated from a leaf1 block.
 */
static void
xfs_dir3_leaf_log_bests(
	xfs_trans_t		*tp,		/* transaction pointer */
	struct xfs_buf		*bp,		/* leaf buffer */
>>>>>>> refs/remotes/origin/master
	int			first,		/* first entry to log */
	int			last)		/* last entry to log */
{
	__be16			*firstb;	/* pointer to first entry */
	__be16			*lastb;		/* pointer to last entry */
<<<<<<< HEAD
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	xfs_dir2_leaf_tail_t	*ltp;		/* leaf tail structure */

	leaf = bp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_DIR2_LEAF1_MAGIC);
=======
	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAF1_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	ltp = xfs_dir2_leaf_tail_p(tp->t_mountp, leaf);
	firstb = xfs_dir2_leaf_bests_p(ltp) + first;
	lastb = xfs_dir2_leaf_bests_p(ltp) + last;
	xfs_da_log_buf(tp, bp, (uint)((char *)firstb - (char *)leaf),
=======
	struct xfs_dir2_leaf	*leaf = bp->b_addr;
	xfs_dir2_leaf_tail_t	*ltp;		/* leaf tail structure */

	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAF1_MAGIC) ||
	       leaf->hdr.info.magic == cpu_to_be16(XFS_DIR3_LEAF1_MAGIC));

	ltp = xfs_dir2_leaf_tail_p(tp->t_mountp, leaf);
	firstb = xfs_dir2_leaf_bests_p(ltp) + first;
	lastb = xfs_dir2_leaf_bests_p(ltp) + last;
	xfs_trans_log_buf(tp, bp, (uint)((char *)firstb - (char *)leaf),
>>>>>>> refs/remotes/origin/master
		(uint)((char *)lastb - (char *)leaf + sizeof(*lastb) - 1));
}

/*
 * Log the leaf entries indicated from a leaf1 or leafn block.
 */
void
<<<<<<< HEAD
xfs_dir2_leaf_log_ents(
	xfs_trans_t		*tp,		/* transaction pointer */
	xfs_dabuf_t		*bp,		/* leaf buffer */
	int			first,		/* first entry to log */
	int			last)		/* last entry to log */
{
	xfs_dir2_leaf_entry_t	*firstlep;	/* pointer to first entry */
	xfs_dir2_leaf_entry_t	*lastlep;	/* pointer to last entry */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */

	leaf = bp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_DIR2_LEAF1_MAGIC ||
	       be16_to_cpu(leaf->hdr.info.magic) == XFS_DIR2_LEAFN_MAGIC);
=======
	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAF1_MAGIC) ||
	       leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAFN_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	firstlep = &leaf->ents[first];
	lastlep = &leaf->ents[last];
	xfs_da_log_buf(tp, bp, (uint)((char *)firstlep - (char *)leaf),
=======
xfs_dir3_leaf_log_ents(
	struct xfs_trans	*tp,
	struct xfs_inode	*dp,
	struct xfs_buf		*bp,
	int			first,
	int			last)
{
	xfs_dir2_leaf_entry_t	*firstlep;	/* pointer to first entry */
	xfs_dir2_leaf_entry_t	*lastlep;	/* pointer to last entry */
	struct xfs_dir2_leaf	*leaf = bp->b_addr;
	struct xfs_dir2_leaf_entry *ents;

	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAF1_MAGIC) ||
	       leaf->hdr.info.magic == cpu_to_be16(XFS_DIR3_LEAF1_MAGIC) ||
	       leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAFN_MAGIC) ||
	       leaf->hdr.info.magic == cpu_to_be16(XFS_DIR3_LEAFN_MAGIC));

	ents = dp->d_ops->leaf_ents_p(leaf);
	firstlep = &ents[first];
	lastlep = &ents[last];
	xfs_trans_log_buf(tp, bp, (uint)((char *)firstlep - (char *)leaf),
>>>>>>> refs/remotes/origin/master
		(uint)((char *)lastlep - (char *)leaf + sizeof(*lastlep) - 1));
}

/*
 * Log the header of the leaf1 or leafn block.
 */
void
<<<<<<< HEAD
xfs_dir2_leaf_log_header(
	xfs_trans_t		*tp,		/* transaction pointer */
	xfs_dabuf_t		*bp)		/* leaf buffer */
{
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */

	leaf = bp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_DIR2_LEAF1_MAGIC ||
	       be16_to_cpu(leaf->hdr.info.magic) == XFS_DIR2_LEAFN_MAGIC);
=======
	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAF1_MAGIC) ||
	       leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAFN_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	xfs_da_log_buf(tp, bp, (uint)((char *)&leaf->hdr - (char *)leaf),
		(uint)(sizeof(leaf->hdr) - 1));
=======
xfs_dir3_leaf_log_header(
	struct xfs_trans	*tp,
	struct xfs_inode	*dp,
	struct xfs_buf		*bp)
{
	struct xfs_dir2_leaf	*leaf = bp->b_addr;

	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAF1_MAGIC) ||
	       leaf->hdr.info.magic == cpu_to_be16(XFS_DIR3_LEAF1_MAGIC) ||
	       leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAFN_MAGIC) ||
	       leaf->hdr.info.magic == cpu_to_be16(XFS_DIR3_LEAFN_MAGIC));

	xfs_trans_log_buf(tp, bp, (uint)((char *)&leaf->hdr - (char *)leaf),
			  dp->d_ops->leaf_hdr_size - 1);
>>>>>>> refs/remotes/origin/master
}

/*
 * Log the tail of the leaf1 block.
 */
STATIC void
<<<<<<< HEAD
xfs_dir2_leaf_log_tail(
	xfs_trans_t		*tp,		/* transaction pointer */
	xfs_dabuf_t		*bp)		/* leaf buffer */
{
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	xfs_dir2_leaf_tail_t	*ltp;		/* leaf tail structure */
	xfs_mount_t		*mp;		/* filesystem mount point */

	mp = tp->t_mountp;
	leaf = bp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_DIR2_LEAF1_MAGIC);
=======
	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAF1_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	ltp = xfs_dir2_leaf_tail_p(mp, leaf);
	xfs_da_log_buf(tp, bp, (uint)((char *)ltp - (char *)leaf),
=======
xfs_dir3_leaf_log_tail(
	struct xfs_trans	*tp,
	struct xfs_buf		*bp)
{
	struct xfs_dir2_leaf	*leaf = bp->b_addr;
	xfs_dir2_leaf_tail_t	*ltp;		/* leaf tail structure */
	struct xfs_mount	*mp = tp->t_mountp;

	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAF1_MAGIC) ||
	       leaf->hdr.info.magic == cpu_to_be16(XFS_DIR3_LEAF1_MAGIC) ||
	       leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAFN_MAGIC) ||
	       leaf->hdr.info.magic == cpu_to_be16(XFS_DIR3_LEAFN_MAGIC));

	ltp = xfs_dir2_leaf_tail_p(mp, leaf);
	xfs_trans_log_buf(tp, bp, (uint)((char *)ltp - (char *)leaf),
>>>>>>> refs/remotes/origin/master
		(uint)(mp->m_dirblksize - 1));
}

/*
 * Look up the entry referred to by args in the leaf format directory.
 * Most of the work is done by the xfs_dir2_leaf_lookup_int routine which
 * is also used by the node-format code.
 */
int
xfs_dir2_leaf_lookup(
	xfs_da_args_t		*args)		/* operation arguments */
{
<<<<<<< HEAD
	xfs_dabuf_t		*dbp;		/* data block buffer */
=======
	struct xfs_buf		*dbp;		/* data block buffer */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_data_entry_t	*dep;		/* data block entry */
	xfs_inode_t		*dp;		/* incore directory inode */
	int			error;		/* error return code */
	int			index;		/* found entry index */
<<<<<<< HEAD
	xfs_dabuf_t		*lbp;		/* leaf buffer */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	xfs_dir2_leaf_entry_t	*lep;		/* leaf entry */
	xfs_trans_t		*tp;		/* transaction pointer */
=======
	struct xfs_buf		*lbp;		/* leaf buffer */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	xfs_dir2_leaf_entry_t	*lep;		/* leaf entry */
	xfs_trans_t		*tp;		/* transaction pointer */
	struct xfs_dir2_leaf_entry *ents;
>>>>>>> refs/remotes/origin/master

	trace_xfs_dir2_leaf_lookup(args);

	/*
	 * Look up name in the leaf block, returning both buffers and index.
	 */
	if ((error = xfs_dir2_leaf_lookup_int(args, &lbp, &index, &dbp))) {
		return error;
	}
	tp = args->trans;
	dp = args->dp;
<<<<<<< HEAD
	xfs_dir2_leaf_check(dp, lbp);
	leaf = lbp->data;
	/*
	 * Get to the leaf entry and contained data entry address.
	 */
	lep = &leaf->ents[index];
=======
	xfs_dir3_leaf_check(dp, lbp);
	leaf = lbp->b_addr;
	ents = dp->d_ops->leaf_ents_p(leaf);
	/*
	 * Get to the leaf entry and contained data entry address.
	 */
	lep = &ents[index];

>>>>>>> refs/remotes/origin/master
	/*
	 * Point to the data entry.
	 */
	dep = (xfs_dir2_data_entry_t *)
<<<<<<< HEAD
	      ((char *)dbp->data +
=======
	      ((char *)dbp->b_addr +
>>>>>>> refs/remotes/origin/master
	       xfs_dir2_dataptr_to_off(dp->i_mount, be32_to_cpu(lep->address)));
	/*
	 * Return the found inode number & CI name if appropriate
	 */
	args->inumber = be64_to_cpu(dep->inumber);
<<<<<<< HEAD
	error = xfs_dir_cilookup_result(args, dep->name, dep->namelen);
	xfs_da_brelse(tp, dbp);
	xfs_da_brelse(tp, lbp);
=======
	args->filetype = dp->d_ops->data_get_ftype(dep);
	error = xfs_dir_cilookup_result(args, dep->name, dep->namelen);
	xfs_trans_brelse(tp, dbp);
	xfs_trans_brelse(tp, lbp);
>>>>>>> refs/remotes/origin/master
	return XFS_ERROR(error);
}

/*
 * Look up name/hash in the leaf block.
 * Fill in indexp with the found index, and dbpp with the data buffer.
 * If not found dbpp will be NULL, and ENOENT comes back.
 * lbpp will always be filled in with the leaf buffer unless there's an error.
 */
static int					/* error */
xfs_dir2_leaf_lookup_int(
	xfs_da_args_t		*args,		/* operation arguments */
<<<<<<< HEAD
	xfs_dabuf_t		**lbpp,		/* out: leaf buffer */
	int			*indexp,	/* out: index in leaf block */
	xfs_dabuf_t		**dbpp)		/* out: data buffer */
{
	xfs_dir2_db_t		curdb = -1;	/* current data block number */
	xfs_dabuf_t		*dbp = NULL;	/* data buffer */
=======
	struct xfs_buf		**lbpp,		/* out: leaf buffer */
	int			*indexp,	/* out: index in leaf block */
	struct xfs_buf		**dbpp)		/* out: data buffer */
{
	xfs_dir2_db_t		curdb = -1;	/* current data block number */
	struct xfs_buf		*dbp = NULL;	/* data buffer */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_data_entry_t	*dep;		/* data entry */
	xfs_inode_t		*dp;		/* incore directory inode */
	int			error;		/* error return code */
	int			index;		/* index in leaf block */
<<<<<<< HEAD
	xfs_dabuf_t		*lbp;		/* leaf buffer */
=======
	struct xfs_buf		*lbp;		/* leaf buffer */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_leaf_entry_t	*lep;		/* leaf entry */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	xfs_mount_t		*mp;		/* filesystem mount point */
	xfs_dir2_db_t		newdb;		/* new data block number */
	xfs_trans_t		*tp;		/* transaction pointer */
	xfs_dir2_db_t		cidb = -1;	/* case match data block no. */
	enum xfs_dacmp		cmp;		/* name compare result */
<<<<<<< HEAD
=======
	struct xfs_dir2_leaf_entry *ents;
	struct xfs_dir3_icleaf_hdr leafhdr;
>>>>>>> refs/remotes/origin/master

	dp = args->dp;
	tp = args->trans;
	mp = dp->i_mount;
<<<<<<< HEAD
	/*
	 * Read the leaf block into the buffer.
	 */
	error = xfs_da_read_buf(tp, dp, mp->m_dirleafblk, -1, &lbp,
							XFS_DATA_FORK);
	if (error)
		return error;
	*lbpp = lbp;
	leaf = lbp->data;
	xfs_dir2_leaf_check(dp, lbp);
=======

	error = xfs_dir3_leaf_read(tp, dp, mp->m_dirleafblk, -1, &lbp);
	if (error)
		return error;

	*lbpp = lbp;
	leaf = lbp->b_addr;
	xfs_dir3_leaf_check(dp, lbp);
	ents = dp->d_ops->leaf_ents_p(leaf);
	dp->d_ops->leaf_hdr_from_disk(&leafhdr, leaf);

>>>>>>> refs/remotes/origin/master
	/*
	 * Look for the first leaf entry with our hash value.
	 */
	index = xfs_dir2_leaf_search_hash(args, lbp);
	/*
	 * Loop over all the entries with the right hash value
	 * looking to match the name.
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
		 * Skip over stale leaf entries.
		 */
		if (be32_to_cpu(lep->address) == XFS_DIR2_NULL_DATAPTR)
			continue;
		/*
		 * Get the new data block number.
		 */
		newdb = xfs_dir2_dataptr_to_db(mp, be32_to_cpu(lep->address));
		/*
		 * If it's not the same as the old data block number,
		 * need to pitch the old one and read the new one.
		 */
		if (newdb != curdb) {
			if (dbp)
<<<<<<< HEAD
				xfs_da_brelse(tp, dbp);
			error = xfs_da_read_buf(tp, dp,
						xfs_dir2_db_to_da(mp, newdb),
						-1, &dbp, XFS_DATA_FORK);
			if (error) {
				xfs_da_brelse(tp, lbp);
				return error;
			}
			xfs_dir2_data_check(dp, dbp);
=======
				xfs_trans_brelse(tp, dbp);
			error = xfs_dir3_data_read(tp, dp,
						   xfs_dir2_db_to_da(mp, newdb),
						   -1, &dbp);
			if (error) {
				xfs_trans_brelse(tp, lbp);
				return error;
			}
>>>>>>> refs/remotes/origin/master
			curdb = newdb;
		}
		/*
		 * Point to the data entry.
		 */
<<<<<<< HEAD
		dep = (xfs_dir2_data_entry_t *)((char *)dbp->data +
=======
		dep = (xfs_dir2_data_entry_t *)((char *)dbp->b_addr +
>>>>>>> refs/remotes/origin/master
			xfs_dir2_dataptr_to_off(mp, be32_to_cpu(lep->address)));
		/*
		 * Compare name and if it's an exact match, return the index
		 * and buffer. If it's the first case-insensitive match, store
		 * the index and buffer and continue looking for an exact match.
		 */
		cmp = mp->m_dirnameops->compname(args, dep->name, dep->namelen);
		if (cmp != XFS_CMP_DIFFERENT && cmp != args->cmpresult) {
			args->cmpresult = cmp;
			*indexp = index;
			/* case exact match: return the current buffer. */
			if (cmp == XFS_CMP_EXACT) {
				*dbpp = dbp;
				return 0;
			}
			cidb = curdb;
		}
	}
	ASSERT(args->op_flags & XFS_DA_OP_OKNOENT);
	/*
	 * Here, we can only be doing a lookup (not a rename or remove).
	 * If a case-insensitive match was found earlier, re-read the
	 * appropriate data block if required and return it.
	 */
	if (args->cmpresult == XFS_CMP_CASE) {
		ASSERT(cidb != -1);
		if (cidb != curdb) {
<<<<<<< HEAD
			xfs_da_brelse(tp, dbp);
			error = xfs_da_read_buf(tp, dp,
						xfs_dir2_db_to_da(mp, cidb),
						-1, &dbp, XFS_DATA_FORK);
			if (error) {
				xfs_da_brelse(tp, lbp);
=======
			xfs_trans_brelse(tp, dbp);
			error = xfs_dir3_data_read(tp, dp,
						   xfs_dir2_db_to_da(mp, cidb),
						   -1, &dbp);
			if (error) {
				xfs_trans_brelse(tp, lbp);
>>>>>>> refs/remotes/origin/master
				return error;
			}
		}
		*dbpp = dbp;
		return 0;
	}
	/*
	 * No match found, return ENOENT.
	 */
	ASSERT(cidb == -1);
	if (dbp)
<<<<<<< HEAD
		xfs_da_brelse(tp, dbp);
	xfs_da_brelse(tp, lbp);
=======
		xfs_trans_brelse(tp, dbp);
	xfs_trans_brelse(tp, lbp);
>>>>>>> refs/remotes/origin/master
	return XFS_ERROR(ENOENT);
}

/*
 * Remove an entry from a leaf format directory.
 */
int						/* error */
xfs_dir2_leaf_removename(
	xfs_da_args_t		*args)		/* operation arguments */
{
	__be16			*bestsp;	/* leaf block best freespace */
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
	xfs_dir2_data_entry_t	*dep;		/* data entry structure */
	xfs_inode_t		*dp;		/* incore directory inode */
	int			error;		/* error return code */
	xfs_dir2_db_t		i;		/* temporary data block # */
	int			index;		/* index into leaf entries */
<<<<<<< HEAD
	xfs_dabuf_t		*lbp;		/* leaf buffer */
=======
	struct xfs_buf		*lbp;		/* leaf buffer */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	xfs_dir2_leaf_entry_t	*lep;		/* leaf entry */
	xfs_dir2_leaf_tail_t	*ltp;		/* leaf tail structure */
	xfs_mount_t		*mp;		/* filesystem mount point */
	int			needlog;	/* need to log data header */
	int			needscan;	/* need to rescan data frees */
	xfs_dir2_data_off_t	oldbest;	/* old value of best free */
	xfs_trans_t		*tp;		/* transaction pointer */
<<<<<<< HEAD
=======
	struct xfs_dir2_data_free *bf;		/* bestfree table */
	struct xfs_dir2_leaf_entry *ents;
	struct xfs_dir3_icleaf_hdr leafhdr;
>>>>>>> refs/remotes/origin/master

	trace_xfs_dir2_leaf_removename(args);

	/*
	 * Lookup the leaf entry, get the leaf and data blocks read in.
	 */
	if ((error = xfs_dir2_leaf_lookup_int(args, &lbp, &index, &dbp))) {
		return error;
	}
	dp = args->dp;
	tp = args->trans;
	mp = dp->i_mount;
<<<<<<< HEAD
	leaf = lbp->data;
<<<<<<< HEAD
	data = dbp->data;
=======
	hdr = dbp->data;
>>>>>>> refs/remotes/origin/cm-10.0
	xfs_dir2_data_check(dp, dbp);
	/*
	 * Point to the leaf entry, use that to point to the data entry.
	 */
	lep = &leaf->ents[index];
	db = xfs_dir2_dataptr_to_db(mp, be32_to_cpu(lep->address));
	dep = (xfs_dir2_data_entry_t *)
<<<<<<< HEAD
	      ((char *)data + xfs_dir2_dataptr_to_off(mp, be32_to_cpu(lep->address)));
	needscan = needlog = 0;
	oldbest = be16_to_cpu(data->hdr.bestfree[0].length);
=======
	      ((char *)hdr + xfs_dir2_dataptr_to_off(mp, be32_to_cpu(lep->address)));
	needscan = needlog = 0;
	oldbest = be16_to_cpu(hdr->bestfree[0].length);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	leaf = lbp->b_addr;
	hdr = dbp->b_addr;
	xfs_dir3_data_check(dp, dbp);
	bf = dp->d_ops->data_bestfree_p(hdr);
	dp->d_ops->leaf_hdr_from_disk(&leafhdr, leaf);
	ents = dp->d_ops->leaf_ents_p(leaf);
	/*
	 * Point to the leaf entry, use that to point to the data entry.
	 */
	lep = &ents[index];
	db = xfs_dir2_dataptr_to_db(mp, be32_to_cpu(lep->address));
	dep = (xfs_dir2_data_entry_t *)
	      ((char *)hdr + xfs_dir2_dataptr_to_off(mp, be32_to_cpu(lep->address)));
	needscan = needlog = 0;
	oldbest = be16_to_cpu(bf[0].length);
>>>>>>> refs/remotes/origin/master
	ltp = xfs_dir2_leaf_tail_p(mp, leaf);
	bestsp = xfs_dir2_leaf_bests_p(ltp);
	ASSERT(be16_to_cpu(bestsp[db]) == oldbest);
	/*
	 * Mark the former data entry unused.
	 */
<<<<<<< HEAD
	xfs_dir2_data_make_free(tp, dbp,
<<<<<<< HEAD
		(xfs_dir2_data_aoff_t)((char *)dep - (char *)data),
=======
		(xfs_dir2_data_aoff_t)((char *)dep - (char *)hdr),
>>>>>>> refs/remotes/origin/cm-10.0
		xfs_dir2_data_entsize(dep->namelen), &needlog, &needscan);
	/*
	 * We just mark the leaf entry stale by putting a null in it.
	 */
	be16_add_cpu(&leaf->hdr.stale, 1);
	xfs_dir2_leaf_log_header(tp, lbp);
	lep->address = cpu_to_be32(XFS_DIR2_NULL_DATAPTR);
	xfs_dir2_leaf_log_ents(tp, lbp, index, index);
=======
	xfs_dir2_data_make_free(tp, dp, dbp,
		(xfs_dir2_data_aoff_t)((char *)dep - (char *)hdr),
		dp->d_ops->data_entsize(dep->namelen), &needlog, &needscan);
	/*
	 * We just mark the leaf entry stale by putting a null in it.
	 */
	leafhdr.stale++;
	dp->d_ops->leaf_hdr_to_disk(leaf, &leafhdr);
	xfs_dir3_leaf_log_header(tp, dp, lbp);

	lep->address = cpu_to_be32(XFS_DIR2_NULL_DATAPTR);
	xfs_dir3_leaf_log_ents(tp, dp, lbp, index, index);

>>>>>>> refs/remotes/origin/master
	/*
	 * Scan the freespace in the data block again if necessary,
	 * log the data block header if necessary.
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
=======
		xfs_dir2_data_freescan(dp, hdr, &needlog);
	if (needlog)
		xfs_dir2_data_log_header(tp, dp, dbp);
>>>>>>> refs/remotes/origin/master
	/*
	 * If the longest freespace in the data block has changed,
	 * put the new value in the bests table and log that.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (be16_to_cpu(data->hdr.bestfree[0].length) != oldbest) {
		bestsp[db] = data->hdr.bestfree[0].length;
=======
	if (be16_to_cpu(hdr->bestfree[0].length) != oldbest) {
		bestsp[db] = hdr->bestfree[0].length;
>>>>>>> refs/remotes/origin/cm-10.0
		xfs_dir2_leaf_log_bests(tp, lbp, db, db);
	}
	xfs_dir2_data_check(dp, dbp);
	/*
	 * If the data block is now empty then get rid of the data block.
	 */
<<<<<<< HEAD
	if (be16_to_cpu(data->hdr.bestfree[0].length) ==
	    mp->m_dirblksize - (uint)sizeof(data->hdr)) {
=======
	if (be16_to_cpu(hdr->bestfree[0].length) ==
	    mp->m_dirblksize - (uint)sizeof(*hdr)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (be16_to_cpu(bf[0].length) != oldbest) {
		bestsp[db] = bf[0].length;
		xfs_dir3_leaf_log_bests(tp, lbp, db, db);
	}
	xfs_dir3_data_check(dp, dbp);
	/*
	 * If the data block is now empty then get rid of the data block.
	 */
	if (be16_to_cpu(bf[0].length) ==
			mp->m_dirblksize - dp->d_ops->data_entry_offset) {
>>>>>>> refs/remotes/origin/master
		ASSERT(db != mp->m_dirdatablk);
		if ((error = xfs_dir2_shrink_inode(args, db, dbp))) {
			/*
			 * Nope, can't get rid of it because it caused
			 * allocation of a bmap btree block to do so.
			 * Just go on, returning success, leaving the
			 * empty block in place.
			 */
<<<<<<< HEAD
			if (error == ENOSPC && args->total == 0) {
				xfs_da_buf_done(dbp);
				error = 0;
			}
			xfs_dir2_leaf_check(dp, lbp);
			xfs_da_buf_done(lbp);
=======
			if (error == ENOSPC && args->total == 0)
				error = 0;
			xfs_dir3_leaf_check(dp, lbp);
>>>>>>> refs/remotes/origin/master
			return error;
		}
		dbp = NULL;
		/*
		 * If this is the last data block then compact the
		 * bests table by getting rid of entries.
		 */
		if (db == be32_to_cpu(ltp->bestcount) - 1) {
			/*
			 * Look for the last active entry (i).
			 */
			for (i = db - 1; i > 0; i--) {
<<<<<<< HEAD
<<<<<<< HEAD
				if (be16_to_cpu(bestsp[i]) != NULLDATAOFF)
=======
				if (bestsp[i] != cpu_to_be16(NULLDATAOFF))
>>>>>>> refs/remotes/origin/cm-10.0
=======
				if (bestsp[i] != cpu_to_be16(NULLDATAOFF))
>>>>>>> refs/remotes/origin/master
					break;
			}
			/*
			 * Copy the table down so inactive entries at the
			 * end are removed.
			 */
			memmove(&bestsp[db - i], bestsp,
				(be32_to_cpu(ltp->bestcount) - (db - i)) * sizeof(*bestsp));
			be32_add_cpu(&ltp->bestcount, -(db - i));
<<<<<<< HEAD
			xfs_dir2_leaf_log_tail(tp, lbp);
			xfs_dir2_leaf_log_bests(tp, lbp, 0, be32_to_cpu(ltp->bestcount) - 1);
=======
			xfs_dir3_leaf_log_tail(tp, lbp);
			xfs_dir3_leaf_log_bests(tp, lbp, 0, be32_to_cpu(ltp->bestcount) - 1);
>>>>>>> refs/remotes/origin/master
		} else
			bestsp[db] = cpu_to_be16(NULLDATAOFF);
	}
	/*
	 * If the data block was not the first one, drop it.
	 */
<<<<<<< HEAD
	else if (db != mp->m_dirdatablk && dbp != NULL) {
		xfs_da_buf_done(dbp);
		dbp = NULL;
	}
	xfs_dir2_leaf_check(dp, lbp);
=======
	else if (db != mp->m_dirdatablk)
		dbp = NULL;

	xfs_dir3_leaf_check(dp, lbp);
>>>>>>> refs/remotes/origin/master
	/*
	 * See if we can convert to block form.
	 */
	return xfs_dir2_leaf_to_block(args, lbp, dbp);
}

/*
 * Replace the inode number in a leaf format directory entry.
 */
int						/* error */
xfs_dir2_leaf_replace(
	xfs_da_args_t		*args)		/* operation arguments */
{
<<<<<<< HEAD
	xfs_dabuf_t		*dbp;		/* data block buffer */
=======
	struct xfs_buf		*dbp;		/* data block buffer */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_data_entry_t	*dep;		/* data block entry */
	xfs_inode_t		*dp;		/* incore directory inode */
	int			error;		/* error return code */
	int			index;		/* index of leaf entry */
<<<<<<< HEAD
	xfs_dabuf_t		*lbp;		/* leaf buffer */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	xfs_dir2_leaf_entry_t	*lep;		/* leaf entry */
	xfs_trans_t		*tp;		/* transaction pointer */
=======
	struct xfs_buf		*lbp;		/* leaf buffer */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	xfs_dir2_leaf_entry_t	*lep;		/* leaf entry */
	xfs_trans_t		*tp;		/* transaction pointer */
	struct xfs_dir2_leaf_entry *ents;
>>>>>>> refs/remotes/origin/master

	trace_xfs_dir2_leaf_replace(args);

	/*
	 * Look up the entry.
	 */
	if ((error = xfs_dir2_leaf_lookup_int(args, &lbp, &index, &dbp))) {
		return error;
	}
	dp = args->dp;
<<<<<<< HEAD
	leaf = lbp->data;
	/*
	 * Point to the leaf entry, get data address from it.
	 */
	lep = &leaf->ents[index];
=======
	leaf = lbp->b_addr;
	ents = dp->d_ops->leaf_ents_p(leaf);
	/*
	 * Point to the leaf entry, get data address from it.
	 */
	lep = &ents[index];
>>>>>>> refs/remotes/origin/master
	/*
	 * Point to the data entry.
	 */
	dep = (xfs_dir2_data_entry_t *)
<<<<<<< HEAD
	      ((char *)dbp->data +
=======
	      ((char *)dbp->b_addr +
>>>>>>> refs/remotes/origin/master
	       xfs_dir2_dataptr_to_off(dp->i_mount, be32_to_cpu(lep->address)));
	ASSERT(args->inumber != be64_to_cpu(dep->inumber));
	/*
	 * Put the new inode number in, log it.
	 */
	dep->inumber = cpu_to_be64(args->inumber);
<<<<<<< HEAD
	tp = args->trans;
	xfs_dir2_data_log_entry(tp, dbp, dep);
	xfs_da_buf_done(dbp);
	xfs_dir2_leaf_check(dp, lbp);
	xfs_da_brelse(tp, lbp);
=======
	dp->d_ops->data_put_ftype(dep, args->filetype);
	tp = args->trans;
	xfs_dir2_data_log_entry(tp, dp, dbp, dep);
	xfs_dir3_leaf_check(dp, lbp);
	xfs_trans_brelse(tp, lbp);
>>>>>>> refs/remotes/origin/master
	return 0;
}

/*
 * Return index in the leaf block (lbp) which is either the first
 * one with this hash value, or if there are none, the insert point
 * for that hash value.
 */
int						/* index value */
xfs_dir2_leaf_search_hash(
	xfs_da_args_t		*args,		/* operation arguments */
<<<<<<< HEAD
	xfs_dabuf_t		*lbp)		/* leaf buffer */
=======
	struct xfs_buf		*lbp)		/* leaf buffer */
>>>>>>> refs/remotes/origin/master
{
	xfs_dahash_t		hash=0;		/* hash from this entry */
	xfs_dahash_t		hashwant;	/* hash value looking for */
	int			high;		/* high leaf index */
	int			low;		/* low leaf index */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	xfs_dir2_leaf_entry_t	*lep;		/* leaf entry */
	int			mid=0;		/* current leaf index */
<<<<<<< HEAD

	leaf = lbp->data;
#ifndef __KERNEL__
	if (!leaf->hdr.count)
		return 0;
#endif
=======
	struct xfs_dir2_leaf_entry *ents;
	struct xfs_dir3_icleaf_hdr leafhdr;

	leaf = lbp->b_addr;
	ents = args->dp->d_ops->leaf_ents_p(leaf);
	args->dp->d_ops->leaf_hdr_from_disk(&leafhdr, leaf);

>>>>>>> refs/remotes/origin/master
	/*
	 * Note, the table cannot be empty, so we have to go through the loop.
	 * Binary search the leaf entries looking for our hash value.
	 */
<<<<<<< HEAD
	for (lep = leaf->ents, low = 0, high = be16_to_cpu(leaf->hdr.count) - 1,
=======
	for (lep = ents, low = 0, high = leafhdr.count - 1,
>>>>>>> refs/remotes/origin/master
		hashwant = args->hashval;
	     low <= high; ) {
		mid = (low + high) >> 1;
		if ((hash = be32_to_cpu(lep[mid].hashval)) == hashwant)
			break;
		if (hash < hashwant)
			low = mid + 1;
		else
			high = mid - 1;
	}
	/*
	 * Found one, back up through all the equal hash values.
	 */
	if (hash == hashwant) {
		while (mid > 0 && be32_to_cpu(lep[mid - 1].hashval) == hashwant) {
			mid--;
		}
	}
	/*
	 * Need to point to an entry higher than ours.
	 */
	else if (hash < hashwant)
		mid++;
	return mid;
}

/*
 * Trim off a trailing data block.  We know it's empty since the leaf
 * freespace table says so.
 */
int						/* error */
xfs_dir2_leaf_trim_data(
	xfs_da_args_t		*args,		/* operation arguments */
<<<<<<< HEAD
	xfs_dabuf_t		*lbp,		/* leaf buffer */
	xfs_dir2_db_t		db)		/* data block number */
{
	__be16			*bestsp;	/* leaf bests table */
<<<<<<< HEAD
#ifdef DEBUG
	xfs_dir2_data_t		*data;		/* data block structure */
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
	xfs_dabuf_t		*dbp;		/* data block buffer */
=======
	struct xfs_buf		*lbp,		/* leaf buffer */
	xfs_dir2_db_t		db)		/* data block number */
{
	__be16			*bestsp;	/* leaf bests table */
	struct xfs_buf		*dbp;		/* data block buffer */
>>>>>>> refs/remotes/origin/master
	xfs_inode_t		*dp;		/* incore directory inode */
	int			error;		/* error return value */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	xfs_dir2_leaf_tail_t	*ltp;		/* leaf tail structure */
	xfs_mount_t		*mp;		/* filesystem mount point */
	xfs_trans_t		*tp;		/* transaction pointer */

	dp = args->dp;
	mp = dp->i_mount;
	tp = args->trans;
	/*
	 * Read the offending data block.  We need its buffer.
	 */
<<<<<<< HEAD
	if ((error = xfs_da_read_buf(tp, dp, xfs_dir2_db_to_da(mp, db), -1, &dbp,
			XFS_DATA_FORK))) {
		return error;
	}
<<<<<<< HEAD
#ifdef DEBUG
	data = dbp->data;
	ASSERT(be32_to_cpu(data->hdr.magic) == XFS_DIR2_DATA_MAGIC);
#endif
	/* this seems to be an error
	 * data is only valid if DEBUG is defined?
	 * RMC 09/08/1999
	 */

	leaf = lbp->data;
	ltp = xfs_dir2_leaf_tail_p(mp, leaf);
	ASSERT(be16_to_cpu(data->hdr.bestfree[0].length) ==
	       mp->m_dirblksize - (uint)sizeof(data->hdr));
	ASSERT(db == be32_to_cpu(ltp->bestcount) - 1);
=======

	leaf = lbp->data;
=======
	error = xfs_dir3_data_read(tp, dp, xfs_dir2_db_to_da(mp, db), -1, &dbp);
	if (error)
		return error;

	leaf = lbp->b_addr;
>>>>>>> refs/remotes/origin/master
	ltp = xfs_dir2_leaf_tail_p(mp, leaf);

#ifdef DEBUG
{
<<<<<<< HEAD
	struct xfs_dir2_data_hdr *hdr = dbp->data;

	ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC));
	ASSERT(be16_to_cpu(hdr->bestfree[0].length) ==
	       mp->m_dirblksize - (uint)sizeof(*hdr));
=======
	struct xfs_dir2_data_hdr *hdr = dbp->b_addr;
	struct xfs_dir2_data_free *bf = dp->d_ops->data_bestfree_p(hdr);

	ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR3_DATA_MAGIC));
	ASSERT(be16_to_cpu(bf[0].length) ==
	       mp->m_dirblksize - dp->d_ops->data_entry_offset);
>>>>>>> refs/remotes/origin/master
	ASSERT(db == be32_to_cpu(ltp->bestcount) - 1);
}
#endif

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Get rid of the data block.
	 */
	if ((error = xfs_dir2_shrink_inode(args, db, dbp))) {
		ASSERT(error != ENOSPC);
<<<<<<< HEAD
		xfs_da_brelse(tp, dbp);
=======
		xfs_trans_brelse(tp, dbp);
>>>>>>> refs/remotes/origin/master
		return error;
	}
	/*
	 * Eliminate the last bests entry from the table.
	 */
	bestsp = xfs_dir2_leaf_bests_p(ltp);
	be32_add_cpu(&ltp->bestcount, -1);
	memmove(&bestsp[1], &bestsp[0], be32_to_cpu(ltp->bestcount) * sizeof(*bestsp));
<<<<<<< HEAD
	xfs_dir2_leaf_log_tail(tp, lbp);
	xfs_dir2_leaf_log_bests(tp, lbp, 0, be32_to_cpu(ltp->bestcount) - 1);
	return 0;
}

<<<<<<< HEAD
=======
static inline size_t
xfs_dir2_leaf_size(
	struct xfs_dir2_leaf_hdr	*hdr,
	int				counts)
{
	int			entries;

	entries = be16_to_cpu(hdr->count) - be16_to_cpu(hdr->stale);
	return sizeof(xfs_dir2_leaf_hdr_t) +
	    entries * sizeof(xfs_dir2_leaf_entry_t) +
	    counts * sizeof(xfs_dir2_data_off_t) +
	    sizeof(xfs_dir2_leaf_tail_t);
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	xfs_dir3_leaf_log_tail(tp, lbp);
	xfs_dir3_leaf_log_bests(tp, lbp, 0, be32_to_cpu(ltp->bestcount) - 1);
	return 0;
}

static inline size_t
xfs_dir3_leaf_size(
	struct xfs_dir3_icleaf_hdr	*hdr,
	int				counts)
{
	int	entries;
	int	hdrsize;

	entries = hdr->count - hdr->stale;
	if (hdr->magic == XFS_DIR2_LEAF1_MAGIC ||
	    hdr->magic == XFS_DIR2_LEAFN_MAGIC)
		hdrsize = sizeof(struct xfs_dir2_leaf_hdr);
	else
		hdrsize = sizeof(struct xfs_dir3_leaf_hdr);

	return hdrsize + entries * sizeof(xfs_dir2_leaf_entry_t)
	               + counts * sizeof(xfs_dir2_data_off_t)
		       + sizeof(xfs_dir2_leaf_tail_t);
}

>>>>>>> refs/remotes/origin/master
/*
 * Convert node form directory to leaf form directory.
 * The root of the node form dir needs to already be a LEAFN block.
 * Just return if we can't do anything.
 */
int						/* error */
xfs_dir2_node_to_leaf(
	xfs_da_state_t		*state)		/* directory operation state */
{
	xfs_da_args_t		*args;		/* operation arguments */
	xfs_inode_t		*dp;		/* incore directory inode */
	int			error;		/* error return code */
<<<<<<< HEAD
	xfs_dabuf_t		*fbp;		/* buffer for freespace block */
	xfs_fileoff_t		fo;		/* freespace file offset */
	xfs_dir2_free_t		*free;		/* freespace structure */
	xfs_dabuf_t		*lbp;		/* buffer for leaf block */
=======
	struct xfs_buf		*fbp;		/* buffer for freespace block */
	xfs_fileoff_t		fo;		/* freespace file offset */
	xfs_dir2_free_t		*free;		/* freespace structure */
	struct xfs_buf		*lbp;		/* buffer for leaf block */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_leaf_tail_t	*ltp;		/* tail of leaf structure */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	xfs_mount_t		*mp;		/* filesystem mount point */
	int			rval;		/* successful free trim? */
	xfs_trans_t		*tp;		/* transaction pointer */
<<<<<<< HEAD
=======
	struct xfs_dir3_icleaf_hdr leafhdr;
	struct xfs_dir3_icfree_hdr freehdr;
>>>>>>> refs/remotes/origin/master

	/*
	 * There's more than a leaf level in the btree, so there must
	 * be multiple leafn blocks.  Give up.
	 */
	if (state->path.active > 1)
		return 0;
	args = state->args;

	trace_xfs_dir2_node_to_leaf(args);

	mp = state->mp;
	dp = args->dp;
	tp = args->trans;
	/*
	 * Get the last offset in the file.
	 */
	if ((error = xfs_bmap_last_offset(tp, dp, &fo, XFS_DATA_FORK))) {
		return error;
	}
	fo -= mp->m_dirblkfsbs;
	/*
	 * If there are freespace blocks other than the first one,
	 * take this opportunity to remove trailing empty freespace blocks
	 * that may have been left behind during no-space-reservation
	 * operations.
	 */
	while (fo > mp->m_dirfreeblk) {
		if ((error = xfs_dir2_node_trim_free(args, fo, &rval))) {
			return error;
		}
		if (rval)
			fo -= mp->m_dirblkfsbs;
		else
			return 0;
	}
	/*
	 * Now find the block just before the freespace block.
	 */
	if ((error = xfs_bmap_last_before(tp, dp, &fo, XFS_DATA_FORK))) {
		return error;
	}
	/*
	 * If it's not the single leaf block, give up.
	 */
	if (XFS_FSB_TO_B(mp, fo) > XFS_DIR2_LEAF_OFFSET + mp->m_dirblksize)
		return 0;
	lbp = state->path.blk[0].bp;
<<<<<<< HEAD
	leaf = lbp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_DIR2_LEAFN_MAGIC);
=======
	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAFN_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * Read the freespace block.
	 */
	if ((error = xfs_da_read_buf(tp, dp, mp->m_dirfreeblk, -1, &fbp,
			XFS_DATA_FORK))) {
		return error;
	}
	free = fbp->data;
<<<<<<< HEAD
	ASSERT(be32_to_cpu(free->hdr.magic) == XFS_DIR2_FREE_MAGIC);
	ASSERT(!free->hdr.firstdb);
=======
	ASSERT(free->hdr.magic == cpu_to_be32(XFS_DIR2_FREE_MAGIC));
	ASSERT(!free->hdr.firstdb);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	leaf = lbp->b_addr;
	dp->d_ops->leaf_hdr_from_disk(&leafhdr, leaf);

	ASSERT(leafhdr.magic == XFS_DIR2_LEAFN_MAGIC ||
	       leafhdr.magic == XFS_DIR3_LEAFN_MAGIC);

	/*
	 * Read the freespace block.
	 */
	error = xfs_dir2_free_read(tp, dp,  mp->m_dirfreeblk, &fbp);
	if (error)
		return error;
	free = fbp->b_addr;
	dp->d_ops->free_hdr_from_disk(&freehdr, free);

	ASSERT(!freehdr.firstdb);

>>>>>>> refs/remotes/origin/master
	/*
	 * Now see if the leafn and free data will fit in a leaf1.
	 * If not, release the buffer and give up.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if ((uint)sizeof(leaf->hdr) +
	    (be16_to_cpu(leaf->hdr.count) - be16_to_cpu(leaf->hdr.stale)) * (uint)sizeof(leaf->ents[0]) +
	    be32_to_cpu(free->hdr.nvalid) * (uint)sizeof(leaf->bests[0]) +
	    (uint)sizeof(leaf->tail) >
	    mp->m_dirblksize) {
		xfs_da_brelse(tp, fbp);
		return 0;
	}
=======
	if (xfs_dir2_leaf_size(&leaf->hdr, be32_to_cpu(free->hdr.nvalid)) >
			mp->m_dirblksize) {
		xfs_da_brelse(tp, fbp);
		return 0;
	}

>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * If the leaf has any stale entries in it, compress them out.
	 * The compact routine will log the header.
	 */
	if (be16_to_cpu(leaf->hdr.stale))
		xfs_dir2_leaf_compact(args, lbp);
	else
		xfs_dir2_leaf_log_header(tp, lbp);
	leaf->hdr.info.magic = cpu_to_be16(XFS_DIR2_LEAF1_MAGIC);
=======
	if (xfs_dir3_leaf_size(&leafhdr, freehdr.nvalid) > mp->m_dirblksize) {
		xfs_trans_brelse(tp, fbp);
		return 0;
	}

	/*
	 * If the leaf has any stale entries in it, compress them out.
	 */
	if (leafhdr.stale)
		xfs_dir3_leaf_compact(args, &leafhdr, lbp);

	lbp->b_ops = &xfs_dir3_leaf1_buf_ops;
	xfs_trans_buf_set_type(tp, lbp, XFS_BLFT_DIR_LEAF1_BUF);
	leafhdr.magic = (leafhdr.magic == XFS_DIR2_LEAFN_MAGIC)
					? XFS_DIR2_LEAF1_MAGIC
					: XFS_DIR3_LEAF1_MAGIC;

>>>>>>> refs/remotes/origin/master
	/*
	 * Set up the leaf tail from the freespace block.
	 */
	ltp = xfs_dir2_leaf_tail_p(mp, leaf);
<<<<<<< HEAD
	ltp->bestcount = free->hdr.nvalid;
	/*
	 * Set up the leaf bests table.
	 */
	memcpy(xfs_dir2_leaf_bests_p(ltp), free->bests,
<<<<<<< HEAD
		be32_to_cpu(ltp->bestcount) * sizeof(leaf->bests[0]));
=======
		be32_to_cpu(ltp->bestcount) * sizeof(xfs_dir2_data_off_t));
>>>>>>> refs/remotes/origin/cm-10.0
	xfs_dir2_leaf_log_bests(tp, lbp, 0, be32_to_cpu(ltp->bestcount) - 1);
	xfs_dir2_leaf_log_tail(tp, lbp);
	xfs_dir2_leaf_check(dp, lbp);
=======
	ltp->bestcount = cpu_to_be32(freehdr.nvalid);

	/*
	 * Set up the leaf bests table.
	 */
	memcpy(xfs_dir2_leaf_bests_p(ltp), dp->d_ops->free_bests_p(free),
		freehdr.nvalid * sizeof(xfs_dir2_data_off_t));

	dp->d_ops->leaf_hdr_to_disk(leaf, &leafhdr);
	xfs_dir3_leaf_log_header(tp, dp, lbp);
	xfs_dir3_leaf_log_bests(tp, lbp, 0, be32_to_cpu(ltp->bestcount) - 1);
	xfs_dir3_leaf_log_tail(tp, lbp);
	xfs_dir3_leaf_check(dp, lbp);

>>>>>>> refs/remotes/origin/master
	/*
	 * Get rid of the freespace block.
	 */
	error = xfs_dir2_shrink_inode(args, XFS_DIR2_FREE_FIRSTDB(mp), fbp);
	if (error) {
		/*
		 * This can't fail here because it can only happen when
		 * punching out the middle of an extent, and this is an
		 * isolated block.
		 */
		ASSERT(error != ENOSPC);
		return error;
	}
	fbp = NULL;
	/*
	 * Now see if we can convert the single-leaf directory
	 * down to a block form directory.
	 * This routine always kills the dabuf for the leaf, so
	 * eliminate it from the path.
	 */
	error = xfs_dir2_leaf_to_block(args, lbp, NULL);
	state->path.blk[0].bp = NULL;
	return error;
}
