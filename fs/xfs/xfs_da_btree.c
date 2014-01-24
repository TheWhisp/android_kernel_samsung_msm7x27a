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
=======
#include "xfs_mount.h"
#include "xfs_da_btree.h"
#include "xfs_bmap_btree.h"
#include "xfs_dir2.h"
#include "xfs_dir2_format.h"
#include "xfs_dir2_priv.h"
>>>>>>> refs/remotes/origin/cm-10.0
#include "xfs_dinode.h"
#include "xfs_inode.h"
=======
#include "xfs_shared.h"
#include "xfs_format.h"
#include "xfs_log_format.h"
#include "xfs_trans_resv.h"
#include "xfs_bit.h"
#include "xfs_sb.h"
#include "xfs_ag.h"
#include "xfs_mount.h"
#include "xfs_da_format.h"
#include "xfs_da_btree.h"
#include "xfs_dir2.h"
#include "xfs_dir2_priv.h"
#include "xfs_inode.h"
#include "xfs_trans.h"
>>>>>>> refs/remotes/origin/master
#include "xfs_inode_item.h"
#include "xfs_alloc.h"
#include "xfs_bmap.h"
#include "xfs_attr.h"
#include "xfs_attr_leaf.h"
<<<<<<< HEAD
<<<<<<< HEAD
#include "xfs_dir2_data.h"
#include "xfs_dir2_leaf.h"
#include "xfs_dir2_block.h"
#include "xfs_dir2_node.h"
=======
>>>>>>> refs/remotes/origin/cm-10.0
#include "xfs_error.h"
#include "xfs_trace.h"
=======
#include "xfs_error.h"
#include "xfs_trace.h"
#include "xfs_cksum.h"
#include "xfs_buf_item.h"
>>>>>>> refs/remotes/origin/master

/*
 * xfs_da_btree.c
 *
 * Routines to implement directories as Btrees of hashed names.
 */

/*========================================================================
 * Function prototypes for the kernel.
 *========================================================================*/

/*
 * Routines used for growing the Btree.
 */
<<<<<<< HEAD
STATIC int xfs_da_root_split(xfs_da_state_t *state,
					    xfs_da_state_blk_t *existing_root,
					    xfs_da_state_blk_t *new_child);
STATIC int xfs_da_node_split(xfs_da_state_t *state,
=======
STATIC int xfs_da3_root_split(xfs_da_state_t *state,
					    xfs_da_state_blk_t *existing_root,
					    xfs_da_state_blk_t *new_child);
STATIC int xfs_da3_node_split(xfs_da_state_t *state,
>>>>>>> refs/remotes/origin/master
					    xfs_da_state_blk_t *existing_blk,
					    xfs_da_state_blk_t *split_blk,
					    xfs_da_state_blk_t *blk_to_add,
					    int treelevel,
					    int *result);
<<<<<<< HEAD
STATIC void xfs_da_node_rebalance(xfs_da_state_t *state,
					 xfs_da_state_blk_t *node_blk_1,
					 xfs_da_state_blk_t *node_blk_2);
STATIC void xfs_da_node_add(xfs_da_state_t *state,
=======
STATIC void xfs_da3_node_rebalance(xfs_da_state_t *state,
					 xfs_da_state_blk_t *node_blk_1,
					 xfs_da_state_blk_t *node_blk_2);
STATIC void xfs_da3_node_add(xfs_da_state_t *state,
>>>>>>> refs/remotes/origin/master
				   xfs_da_state_blk_t *old_node_blk,
				   xfs_da_state_blk_t *new_node_blk);

/*
 * Routines used for shrinking the Btree.
 */
<<<<<<< HEAD
STATIC int xfs_da_root_join(xfs_da_state_t *state,
					   xfs_da_state_blk_t *root_blk);
STATIC int xfs_da_node_toosmall(xfs_da_state_t *state, int *retval);
STATIC void xfs_da_node_remove(xfs_da_state_t *state,
					      xfs_da_state_blk_t *drop_blk);
STATIC void xfs_da_node_unbalance(xfs_da_state_t *state,
=======
STATIC int xfs_da3_root_join(xfs_da_state_t *state,
					   xfs_da_state_blk_t *root_blk);
STATIC int xfs_da3_node_toosmall(xfs_da_state_t *state, int *retval);
STATIC void xfs_da3_node_remove(xfs_da_state_t *state,
					      xfs_da_state_blk_t *drop_blk);
STATIC void xfs_da3_node_unbalance(xfs_da_state_t *state,
>>>>>>> refs/remotes/origin/master
					 xfs_da_state_blk_t *src_node_blk,
					 xfs_da_state_blk_t *dst_node_blk);

/*
 * Utility routines.
 */
<<<<<<< HEAD
STATIC uint	xfs_da_node_lasthash(xfs_dabuf_t *bp, int *count);
STATIC int	xfs_da_node_order(xfs_dabuf_t *node1_bp, xfs_dabuf_t *node2_bp);
<<<<<<< HEAD
STATIC xfs_dabuf_t *xfs_da_buf_make(int nbuf, xfs_buf_t **bps, inst_t *ra);
=======
STATIC xfs_dabuf_t *xfs_da_buf_make(int nbuf, xfs_buf_t **bps);
>>>>>>> refs/remotes/origin/cm-10.0
STATIC int	xfs_da_blk_unlink(xfs_da_state_t *state,
				  xfs_da_state_blk_t *drop_blk,
				  xfs_da_state_blk_t *save_blk);
STATIC void	xfs_da_state_kill_altpath(xfs_da_state_t *state);
=======
STATIC int	xfs_da3_blk_unlink(xfs_da_state_t *state,
				  xfs_da_state_blk_t *drop_blk,
				  xfs_da_state_blk_t *save_blk);


kmem_zone_t *xfs_da_state_zone;	/* anchor for state struct zone */

/*
 * Allocate a dir-state structure.
 * We don't put them on the stack since they're large.
 */
xfs_da_state_t *
xfs_da_state_alloc(void)
{
	return kmem_zone_zalloc(xfs_da_state_zone, KM_NOFS);
}

/*
 * Kill the altpath contents of a da-state structure.
 */
STATIC void
xfs_da_state_kill_altpath(xfs_da_state_t *state)
{
	int	i;

	for (i = 0; i < state->altpath.active; i++)
		state->altpath.blk[i].bp = NULL;
	state->altpath.active = 0;
}

/*
 * Free a da-state structure.
 */
void
xfs_da_state_free(xfs_da_state_t *state)
{
	xfs_da_state_kill_altpath(state);
#ifdef DEBUG
	memset((char *)state, 0, sizeof(*state));
#endif /* DEBUG */
	kmem_zone_free(xfs_da_state_zone, state);
}

static bool
xfs_da3_node_verify(
	struct xfs_buf		*bp)
{
	struct xfs_mount	*mp = bp->b_target->bt_mount;
	struct xfs_da_intnode	*hdr = bp->b_addr;
	struct xfs_da3_icnode_hdr ichdr;
	const struct xfs_dir_ops *ops;

	ops = xfs_dir_get_ops(mp, NULL);

	ops->node_hdr_from_disk(&ichdr, hdr);

	if (xfs_sb_version_hascrc(&mp->m_sb)) {
		struct xfs_da3_node_hdr *hdr3 = bp->b_addr;

		if (ichdr.magic != XFS_DA3_NODE_MAGIC)
			return false;

		if (!uuid_equal(&hdr3->info.uuid, &mp->m_sb.sb_uuid))
			return false;
		if (be64_to_cpu(hdr3->info.blkno) != bp->b_bn)
			return false;
	} else {
		if (ichdr.magic != XFS_DA_NODE_MAGIC)
			return false;
	}
	if (ichdr.level == 0)
		return false;
	if (ichdr.level > XFS_DA_NODE_MAXDEPTH)
		return false;
	if (ichdr.count == 0)
		return false;

	/*
	 * we don't know if the node is for and attribute or directory tree,
	 * so only fail if the count is outside both bounds
	 */
	if (ichdr.count > mp->m_dir_node_ents &&
	    ichdr.count > mp->m_attr_node_ents)
		return false;

	/* XXX: hash order check? */

	return true;
}

static void
xfs_da3_node_write_verify(
	struct xfs_buf	*bp)
{
	struct xfs_mount	*mp = bp->b_target->bt_mount;
	struct xfs_buf_log_item	*bip = bp->b_fspriv;
	struct xfs_da3_node_hdr *hdr3 = bp->b_addr;

	if (!xfs_da3_node_verify(bp)) {
		XFS_CORRUPTION_ERROR(__func__, XFS_ERRLEVEL_LOW, mp, bp->b_addr);
		xfs_buf_ioerror(bp, EFSCORRUPTED);
		return;
	}

	if (!xfs_sb_version_hascrc(&mp->m_sb))
		return;

	if (bip)
		hdr3->info.lsn = cpu_to_be64(bip->bli_item.li_lsn);

	xfs_update_cksum(bp->b_addr, BBTOB(bp->b_length), XFS_DA3_NODE_CRC_OFF);
}

/*
 * leaf/node format detection on trees is sketchy, so a node read can be done on
 * leaf level blocks when detection identifies the tree as a node format tree
 * incorrectly. In this case, we need to swap the verifier to match the correct
 * format of the block being read.
 */
static void
xfs_da3_node_read_verify(
	struct xfs_buf		*bp)
{
	struct xfs_mount	*mp = bp->b_target->bt_mount;
	struct xfs_da_blkinfo	*info = bp->b_addr;

	switch (be16_to_cpu(info->magic)) {
		case XFS_DA3_NODE_MAGIC:
			if (!xfs_verify_cksum(bp->b_addr, BBTOB(bp->b_length),
					      XFS_DA3_NODE_CRC_OFF))
				break;
			/* fall through */
		case XFS_DA_NODE_MAGIC:
			if (!xfs_da3_node_verify(bp))
				break;
			return;
		case XFS_ATTR_LEAF_MAGIC:
		case XFS_ATTR3_LEAF_MAGIC:
			bp->b_ops = &xfs_attr3_leaf_buf_ops;
			bp->b_ops->verify_read(bp);
			return;
		case XFS_DIR2_LEAFN_MAGIC:
		case XFS_DIR3_LEAFN_MAGIC:
			bp->b_ops = &xfs_dir3_leafn_buf_ops;
			bp->b_ops->verify_read(bp);
			return;
		default:
			break;
	}

	/* corrupt block */
	XFS_CORRUPTION_ERROR(__func__, XFS_ERRLEVEL_LOW, mp, bp->b_addr);
	xfs_buf_ioerror(bp, EFSCORRUPTED);
}

const struct xfs_buf_ops xfs_da3_node_buf_ops = {
	.verify_read = xfs_da3_node_read_verify,
	.verify_write = xfs_da3_node_write_verify,
};

int
xfs_da3_node_read(
	struct xfs_trans	*tp,
	struct xfs_inode	*dp,
	xfs_dablk_t		bno,
	xfs_daddr_t		mappedbno,
	struct xfs_buf		**bpp,
	int			which_fork)
{
	int			err;

	err = xfs_da_read_buf(tp, dp, bno, mappedbno, bpp,
					which_fork, &xfs_da3_node_buf_ops);
	if (!err && tp) {
		struct xfs_da_blkinfo	*info = (*bpp)->b_addr;
		int			type;

		switch (be16_to_cpu(info->magic)) {
		case XFS_DA_NODE_MAGIC:
		case XFS_DA3_NODE_MAGIC:
			type = XFS_BLFT_DA_NODE_BUF;
			break;
		case XFS_ATTR_LEAF_MAGIC:
		case XFS_ATTR3_LEAF_MAGIC:
			type = XFS_BLFT_ATTR_LEAF_BUF;
			break;
		case XFS_DIR2_LEAFN_MAGIC:
		case XFS_DIR3_LEAFN_MAGIC:
			type = XFS_BLFT_DIR_LEAFN_BUF;
			break;
		default:
			type = 0;
			ASSERT(0);
			break;
		}
		xfs_trans_buf_set_type(tp, *bpp, type);
	}
	return err;
}
>>>>>>> refs/remotes/origin/master

/*========================================================================
 * Routines used for growing the Btree.
 *========================================================================*/

/*
 * Create the initial contents of an intermediate node.
 */
int
<<<<<<< HEAD
xfs_da_node_create(xfs_da_args_t *args, xfs_dablk_t blkno, int level,
				 xfs_dabuf_t **bpp, int whichfork)
{
	xfs_da_intnode_t *node;
	xfs_dabuf_t *bp;
	int error;
	xfs_trans_t *tp;

<<<<<<< HEAD
=======
	trace_xfs_da_node_create(args);

>>>>>>> refs/remotes/origin/cm-10.0
	tp = args->trans;
	error = xfs_da_get_buf(tp, args->dp, blkno, -1, &bp, whichfork);
	if (error)
		return(error);
	ASSERT(bp != NULL);
	node = bp->data;
	node->hdr.info.forw = 0;
	node->hdr.info.back = 0;
	node->hdr.info.magic = cpu_to_be16(XFS_DA_NODE_MAGIC);
	node->hdr.info.pad = 0;
	node->hdr.count = 0;
	node->hdr.level = cpu_to_be16(level);

	xfs_da_log_buf(tp, bp,
		XFS_DA_LOGRANGE(node, &node->hdr, sizeof(node->hdr)));
=======
xfs_da3_node_create(
	struct xfs_da_args	*args,
	xfs_dablk_t		blkno,
	int			level,
	struct xfs_buf		**bpp,
	int			whichfork)
{
	struct xfs_da_intnode	*node;
	struct xfs_trans	*tp = args->trans;
	struct xfs_mount	*mp = tp->t_mountp;
	struct xfs_da3_icnode_hdr ichdr = {0};
	struct xfs_buf		*bp;
	int			error;
	struct xfs_inode	*dp = args->dp;

	trace_xfs_da_node_create(args);
	ASSERT(level <= XFS_DA_NODE_MAXDEPTH);

	error = xfs_da_get_buf(tp, dp, blkno, -1, &bp, whichfork);
	if (error)
		return(error);
	bp->b_ops = &xfs_da3_node_buf_ops;
	xfs_trans_buf_set_type(tp, bp, XFS_BLFT_DA_NODE_BUF);
	node = bp->b_addr;

	if (xfs_sb_version_hascrc(&mp->m_sb)) {
		struct xfs_da3_node_hdr *hdr3 = bp->b_addr;

		ichdr.magic = XFS_DA3_NODE_MAGIC;
		hdr3->info.blkno = cpu_to_be64(bp->b_bn);
		hdr3->info.owner = cpu_to_be64(args->dp->i_ino);
		uuid_copy(&hdr3->info.uuid, &mp->m_sb.sb_uuid);
	} else {
		ichdr.magic = XFS_DA_NODE_MAGIC;
	}
	ichdr.level = level;

	dp->d_ops->node_hdr_to_disk(node, &ichdr);
	xfs_trans_log_buf(tp, bp,
		XFS_DA_LOGRANGE(node, &node->hdr, dp->d_ops->node_hdr_size));
>>>>>>> refs/remotes/origin/master

	*bpp = bp;
	return(0);
}

/*
 * Split a leaf node, rebalance, then possibly split
 * intermediate nodes, rebalance, etc.
 */
int							/* error */
<<<<<<< HEAD
xfs_da_split(xfs_da_state_t *state)
{
	xfs_da_state_blk_t *oldblk, *newblk, *addblk;
	xfs_da_intnode_t *node;
	xfs_dabuf_t *bp;
	int max, action, error, i;

<<<<<<< HEAD
=======
	trace_xfs_da_split(state->args);

>>>>>>> refs/remotes/origin/cm-10.0
=======
xfs_da3_split(
	struct xfs_da_state	*state)
{
	struct xfs_da_state_blk	*oldblk;
	struct xfs_da_state_blk	*newblk;
	struct xfs_da_state_blk	*addblk;
	struct xfs_da_intnode	*node;
	struct xfs_buf		*bp;
	int			max;
	int			action = 0;
	int			error;
	int			i;

	trace_xfs_da_split(state->args);

>>>>>>> refs/remotes/origin/master
	/*
	 * Walk back up the tree splitting/inserting/adjusting as necessary.
	 * If we need to insert and there isn't room, split the node, then
	 * decide which fragment to insert the new block from below into.
	 * Note that we may split the root this way, but we need more fixup.
	 */
	max = state->path.active - 1;
	ASSERT((max >= 0) && (max < XFS_DA_NODE_MAXDEPTH));
	ASSERT(state->path.blk[max].magic == XFS_ATTR_LEAF_MAGIC ||
	       state->path.blk[max].magic == XFS_DIR2_LEAFN_MAGIC);

	addblk = &state->path.blk[max];		/* initial dummy value */
	for (i = max; (i >= 0) && addblk; state->path.active--, i--) {
		oldblk = &state->path.blk[i];
		newblk = &state->altpath.blk[i];

		/*
		 * If a leaf node then
		 *     Allocate a new leaf node, then rebalance across them.
		 * else if an intermediate node then
		 *     We split on the last layer, must we split the node?
		 */
		switch (oldblk->magic) {
		case XFS_ATTR_LEAF_MAGIC:
<<<<<<< HEAD
			error = xfs_attr_leaf_split(state, oldblk, newblk);
=======
			error = xfs_attr3_leaf_split(state, oldblk, newblk);
>>>>>>> refs/remotes/origin/master
			if ((error != 0) && (error != ENOSPC)) {
				return(error);	/* GROT: attr is inconsistent */
			}
			if (!error) {
				addblk = newblk;
				break;
			}
			/*
			 * Entry wouldn't fit, split the leaf again.
			 */
			state->extravalid = 1;
			if (state->inleaf) {
				state->extraafter = 0;	/* before newblk */
<<<<<<< HEAD
<<<<<<< HEAD
=======
				trace_xfs_attr_leaf_split_before(state->args);
>>>>>>> refs/remotes/origin/cm-10.0
				error = xfs_attr_leaf_split(state, oldblk,
							    &state->extrablk);
			} else {
				state->extraafter = 1;	/* after newblk */
<<<<<<< HEAD
=======
				trace_xfs_attr_leaf_split_after(state->args);
>>>>>>> refs/remotes/origin/cm-10.0
				error = xfs_attr_leaf_split(state, newblk,
=======
				trace_xfs_attr_leaf_split_before(state->args);
				error = xfs_attr3_leaf_split(state, oldblk,
							    &state->extrablk);
			} else {
				state->extraafter = 1;	/* after newblk */
				trace_xfs_attr_leaf_split_after(state->args);
				error = xfs_attr3_leaf_split(state, newblk,
>>>>>>> refs/remotes/origin/master
							    &state->extrablk);
			}
			if (error)
				return(error);	/* GROT: attr inconsistent */
			addblk = newblk;
			break;
		case XFS_DIR2_LEAFN_MAGIC:
			error = xfs_dir2_leafn_split(state, oldblk, newblk);
			if (error)
				return error;
			addblk = newblk;
			break;
		case XFS_DA_NODE_MAGIC:
<<<<<<< HEAD
			error = xfs_da_node_split(state, oldblk, newblk, addblk,
							 max - i, &action);
			xfs_da_buf_done(addblk->bp);
=======
			error = xfs_da3_node_split(state, oldblk, newblk, addblk,
							 max - i, &action);
>>>>>>> refs/remotes/origin/master
			addblk->bp = NULL;
			if (error)
				return(error);	/* GROT: dir is inconsistent */
			/*
			 * Record the newly split block for the next time thru?
			 */
			if (action)
				addblk = newblk;
			else
				addblk = NULL;
			break;
		}

		/*
		 * Update the btree to show the new hashval for this child.
		 */
<<<<<<< HEAD
		xfs_da_fixhashpath(state, &state->path);
		/*
		 * If we won't need this block again, it's getting dropped
		 * from the active path by the loop control, so we need
		 * to mark it done now.
		 */
		if (i > 0 || !addblk)
			xfs_da_buf_done(oldblk->bp);
=======
		xfs_da3_fixhashpath(state, &state->path);
>>>>>>> refs/remotes/origin/master
	}
	if (!addblk)
		return(0);

	/*
	 * Split the root node.
	 */
	ASSERT(state->path.active == 0);
	oldblk = &state->path.blk[0];
<<<<<<< HEAD
	error = xfs_da_root_split(state, oldblk, addblk);
	if (error) {
		xfs_da_buf_done(oldblk->bp);
		xfs_da_buf_done(addblk->bp);
=======
	error = xfs_da3_root_split(state, oldblk, addblk);
	if (error) {
>>>>>>> refs/remotes/origin/master
		addblk->bp = NULL;
		return(error);	/* GROT: dir is inconsistent */
	}

	/*
	 * Update pointers to the node which used to be block 0 and
	 * just got bumped because of the addition of a new root node.
	 * There might be three blocks involved if a double split occurred,
	 * and the original block 0 could be at any position in the list.
<<<<<<< HEAD
	 */

	node = oldblk->bp->data;
=======
	 *
	 * Note: the magic numbers and sibling pointers are in the same
	 * physical place for both v2 and v3 headers (by design). Hence it
	 * doesn't matter which version of the xfs_da_intnode structure we use
	 * here as the result will be the same using either structure.
	 */
	node = oldblk->bp->b_addr;
>>>>>>> refs/remotes/origin/master
	if (node->hdr.info.forw) {
		if (be32_to_cpu(node->hdr.info.forw) == addblk->blkno) {
			bp = addblk->bp;
		} else {
			ASSERT(state->extravalid);
			bp = state->extrablk.bp;
		}
<<<<<<< HEAD
		node = bp->data;
		node->hdr.info.back = cpu_to_be32(oldblk->blkno);
		xfs_da_log_buf(state->args->trans, bp,
		    XFS_DA_LOGRANGE(node, &node->hdr.info,
		    sizeof(node->hdr.info)));
	}
	node = oldblk->bp->data;
=======
		node = bp->b_addr;
		node->hdr.info.back = cpu_to_be32(oldblk->blkno);
		xfs_trans_log_buf(state->args->trans, bp,
		    XFS_DA_LOGRANGE(node, &node->hdr.info,
		    sizeof(node->hdr.info)));
	}
	node = oldblk->bp->b_addr;
>>>>>>> refs/remotes/origin/master
	if (node->hdr.info.back) {
		if (be32_to_cpu(node->hdr.info.back) == addblk->blkno) {
			bp = addblk->bp;
		} else {
			ASSERT(state->extravalid);
			bp = state->extrablk.bp;
		}
<<<<<<< HEAD
		node = bp->data;
		node->hdr.info.forw = cpu_to_be32(oldblk->blkno);
		xfs_da_log_buf(state->args->trans, bp,
		    XFS_DA_LOGRANGE(node, &node->hdr.info,
		    sizeof(node->hdr.info)));
	}
	xfs_da_buf_done(oldblk->bp);
	xfs_da_buf_done(addblk->bp);
=======
		node = bp->b_addr;
		node->hdr.info.forw = cpu_to_be32(oldblk->blkno);
		xfs_trans_log_buf(state->args->trans, bp,
		    XFS_DA_LOGRANGE(node, &node->hdr.info,
		    sizeof(node->hdr.info)));
	}
>>>>>>> refs/remotes/origin/master
	addblk->bp = NULL;
	return(0);
}

/*
 * Split the root.  We have to create a new root and point to the two
 * parts (the split old root) that we just created.  Copy block zero to
 * the EOF, extending the inode in process.
 */
STATIC int						/* error */
<<<<<<< HEAD
xfs_da_root_split(xfs_da_state_t *state, xfs_da_state_blk_t *blk1,
				 xfs_da_state_blk_t *blk2)
{
	xfs_da_intnode_t *node, *oldroot;
	xfs_da_args_t *args;
	xfs_dablk_t blkno;
	xfs_dabuf_t *bp;
	int error, size;
	xfs_inode_t *dp;
	xfs_trans_t *tp;
	xfs_mount_t *mp;
	xfs_dir2_leaf_t *leaf;

<<<<<<< HEAD
=======
	trace_xfs_da_root_split(state->args);

>>>>>>> refs/remotes/origin/cm-10.0
=======
xfs_da3_root_split(
	struct xfs_da_state	*state,
	struct xfs_da_state_blk	*blk1,
	struct xfs_da_state_blk	*blk2)
{
	struct xfs_da_intnode	*node;
	struct xfs_da_intnode	*oldroot;
	struct xfs_da_node_entry *btree;
	struct xfs_da3_icnode_hdr nodehdr;
	struct xfs_da_args	*args;
	struct xfs_buf		*bp;
	struct xfs_inode	*dp;
	struct xfs_trans	*tp;
	struct xfs_mount	*mp;
	struct xfs_dir2_leaf	*leaf;
	xfs_dablk_t		blkno;
	int			level;
	int			error;
	int			size;

	trace_xfs_da_root_split(state->args);

>>>>>>> refs/remotes/origin/master
	/*
	 * Copy the existing (incorrect) block from the root node position
	 * to a free space somewhere.
	 */
	args = state->args;
<<<<<<< HEAD
	ASSERT(args != NULL);
	error = xfs_da_grow_inode(args, &blkno);
	if (error)
		return(error);
=======
	error = xfs_da_grow_inode(args, &blkno);
	if (error)
		return error;

>>>>>>> refs/remotes/origin/master
	dp = args->dp;
	tp = args->trans;
	mp = state->mp;
	error = xfs_da_get_buf(tp, dp, blkno, -1, &bp, args->whichfork);
	if (error)
<<<<<<< HEAD
		return(error);
	ASSERT(bp != NULL);
	node = bp->data;
	oldroot = blk1->bp->data;
<<<<<<< HEAD
	if (be16_to_cpu(oldroot->hdr.info.magic) == XFS_DA_NODE_MAGIC) {
		size = (int)((char *)&oldroot->btree[be16_to_cpu(oldroot->hdr.count)] -
			     (char *)oldroot);
	} else {
		ASSERT(be16_to_cpu(oldroot->hdr.info.magic) == XFS_DIR2_LEAFN_MAGIC);
=======
	if (oldroot->hdr.info.magic == cpu_to_be16(XFS_DA_NODE_MAGIC)) {
		size = (int)((char *)&oldroot->btree[be16_to_cpu(oldroot->hdr.count)] -
			     (char *)oldroot);
	} else {
		ASSERT(oldroot->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAFN_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
		leaf = (xfs_dir2_leaf_t *)oldroot;
		size = (int)((char *)&leaf->ents[be16_to_cpu(leaf->hdr.count)] -
			     (char *)leaf);
	}
	memcpy(node, oldroot, size);
	xfs_da_log_buf(tp, bp, 0, size - 1);
	xfs_da_buf_done(blk1->bp);
=======
		return error;
	node = bp->b_addr;
	oldroot = blk1->bp->b_addr;
	if (oldroot->hdr.info.magic == cpu_to_be16(XFS_DA_NODE_MAGIC) ||
	    oldroot->hdr.info.magic == cpu_to_be16(XFS_DA3_NODE_MAGIC)) {
		struct xfs_da3_icnode_hdr nodehdr;

		dp->d_ops->node_hdr_from_disk(&nodehdr, oldroot);
		btree = dp->d_ops->node_tree_p(oldroot);
		size = (int)((char *)&btree[nodehdr.count] - (char *)oldroot);
		level = nodehdr.level;

		/*
		 * we are about to copy oldroot to bp, so set up the type
		 * of bp while we know exactly what it will be.
		 */
		xfs_trans_buf_set_type(tp, bp, XFS_BLFT_DA_NODE_BUF);
	} else {
		struct xfs_dir3_icleaf_hdr leafhdr;
		struct xfs_dir2_leaf_entry *ents;

		leaf = (xfs_dir2_leaf_t *)oldroot;
		dp->d_ops->leaf_hdr_from_disk(&leafhdr, leaf);
		ents = dp->d_ops->leaf_ents_p(leaf);

		ASSERT(leafhdr.magic == XFS_DIR2_LEAFN_MAGIC ||
		       leafhdr.magic == XFS_DIR3_LEAFN_MAGIC);
		size = (int)((char *)&ents[leafhdr.count] - (char *)leaf);
		level = 0;

		/*
		 * we are about to copy oldroot to bp, so set up the type
		 * of bp while we know exactly what it will be.
		 */
		xfs_trans_buf_set_type(tp, bp, XFS_BLFT_DIR_LEAFN_BUF);
	}

	/*
	 * we can copy most of the information in the node from one block to
	 * another, but for CRC enabled headers we have to make sure that the
	 * block specific identifiers are kept intact. We update the buffer
	 * directly for this.
	 */
	memcpy(node, oldroot, size);
	if (oldroot->hdr.info.magic == cpu_to_be16(XFS_DA3_NODE_MAGIC) ||
	    oldroot->hdr.info.magic == cpu_to_be16(XFS_DIR3_LEAFN_MAGIC)) {
		struct xfs_da3_intnode *node3 = (struct xfs_da3_intnode *)node;

		node3->hdr.info.blkno = cpu_to_be64(bp->b_bn);
	}
	xfs_trans_log_buf(tp, bp, 0, size - 1);

	bp->b_ops = blk1->bp->b_ops;
	xfs_trans_buf_copy_type(bp, blk1->bp);
>>>>>>> refs/remotes/origin/master
	blk1->bp = bp;
	blk1->blkno = blkno;

	/*
	 * Set up the new root node.
	 */
<<<<<<< HEAD
	error = xfs_da_node_create(args,
		(args->whichfork == XFS_DATA_FORK) ? mp->m_dirleafblk : 0,
		be16_to_cpu(node->hdr.level) + 1, &bp, args->whichfork);
	if (error)
		return(error);
	node = bp->data;
	node->btree[0].hashval = cpu_to_be32(blk1->hashval);
	node->btree[0].before = cpu_to_be32(blk1->blkno);
	node->btree[1].hashval = cpu_to_be32(blk2->hashval);
	node->btree[1].before = cpu_to_be32(blk2->blkno);
	node->hdr.count = cpu_to_be16(2);

#ifdef DEBUG
<<<<<<< HEAD
	if (be16_to_cpu(oldroot->hdr.info.magic) == XFS_DIR2_LEAFN_MAGIC) {
=======
	if (oldroot->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAFN_MAGIC)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	error = xfs_da3_node_create(args,
		(args->whichfork == XFS_DATA_FORK) ? mp->m_dirleafblk : 0,
		level + 1, &bp, args->whichfork);
	if (error)
		return error;

	node = bp->b_addr;
	dp->d_ops->node_hdr_from_disk(&nodehdr, node);
	btree = dp->d_ops->node_tree_p(node);
	btree[0].hashval = cpu_to_be32(blk1->hashval);
	btree[0].before = cpu_to_be32(blk1->blkno);
	btree[1].hashval = cpu_to_be32(blk2->hashval);
	btree[1].before = cpu_to_be32(blk2->blkno);
	nodehdr.count = 2;
	dp->d_ops->node_hdr_to_disk(node, &nodehdr);

#ifdef DEBUG
	if (oldroot->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAFN_MAGIC) ||
	    oldroot->hdr.info.magic == cpu_to_be16(XFS_DIR3_LEAFN_MAGIC)) {
>>>>>>> refs/remotes/origin/master
		ASSERT(blk1->blkno >= mp->m_dirleafblk &&
		       blk1->blkno < mp->m_dirfreeblk);
		ASSERT(blk2->blkno >= mp->m_dirleafblk &&
		       blk2->blkno < mp->m_dirfreeblk);
	}
#endif

	/* Header is already logged by xfs_da_node_create */
<<<<<<< HEAD
	xfs_da_log_buf(tp, bp,
		XFS_DA_LOGRANGE(node, node->btree,
			sizeof(xfs_da_node_entry_t) * 2));
	xfs_da_buf_done(bp);

	return(0);
=======
	xfs_trans_log_buf(tp, bp,
		XFS_DA_LOGRANGE(node, btree, sizeof(xfs_da_node_entry_t) * 2));

	return 0;
>>>>>>> refs/remotes/origin/master
}

/*
 * Split the node, rebalance, then add the new entry.
 */
STATIC int						/* error */
<<<<<<< HEAD
xfs_da_node_split(xfs_da_state_t *state, xfs_da_state_blk_t *oldblk,
				 xfs_da_state_blk_t *newblk,
				 xfs_da_state_blk_t *addblk,
				 int treelevel, int *result)
{
	xfs_da_intnode_t *node;
	xfs_dablk_t blkno;
	int newcount, error;
	int useextra;

<<<<<<< HEAD
	node = oldblk->bp->data;
	ASSERT(be16_to_cpu(node->hdr.info.magic) == XFS_DA_NODE_MAGIC);
=======
	trace_xfs_da_node_split(state->args);

	node = oldblk->bp->data;
	ASSERT(node->hdr.info.magic == cpu_to_be16(XFS_DA_NODE_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
=======
xfs_da3_node_split(
	struct xfs_da_state	*state,
	struct xfs_da_state_blk	*oldblk,
	struct xfs_da_state_blk	*newblk,
	struct xfs_da_state_blk	*addblk,
	int			treelevel,
	int			*result)
{
	struct xfs_da_intnode	*node;
	struct xfs_da3_icnode_hdr nodehdr;
	xfs_dablk_t		blkno;
	int			newcount;
	int			error;
	int			useextra;
	struct xfs_inode	*dp = state->args->dp;

	trace_xfs_da_node_split(state->args);

	node = oldblk->bp->b_addr;
	dp->d_ops->node_hdr_from_disk(&nodehdr, node);
>>>>>>> refs/remotes/origin/master

	/*
	 * With V2 dirs the extra block is data or freespace.
	 */
	useextra = state->extravalid && state->args->whichfork == XFS_ATTR_FORK;
	newcount = 1 + useextra;
	/*
	 * Do we have to split the node?
	 */
<<<<<<< HEAD
	if ((be16_to_cpu(node->hdr.count) + newcount) > state->node_ents) {
=======
	if (nodehdr.count + newcount > state->node_ents) {
>>>>>>> refs/remotes/origin/master
		/*
		 * Allocate a new node, add to the doubly linked chain of
		 * nodes, then move some of our excess entries into it.
		 */
		error = xfs_da_grow_inode(state->args, &blkno);
		if (error)
			return(error);	/* GROT: dir is inconsistent */

<<<<<<< HEAD
		error = xfs_da_node_create(state->args, blkno, treelevel,
=======
		error = xfs_da3_node_create(state->args, blkno, treelevel,
>>>>>>> refs/remotes/origin/master
					   &newblk->bp, state->args->whichfork);
		if (error)
			return(error);	/* GROT: dir is inconsistent */
		newblk->blkno = blkno;
		newblk->magic = XFS_DA_NODE_MAGIC;
<<<<<<< HEAD
		xfs_da_node_rebalance(state, oldblk, newblk);
		error = xfs_da_blk_link(state, oldblk, newblk);
=======
		xfs_da3_node_rebalance(state, oldblk, newblk);
		error = xfs_da3_blk_link(state, oldblk, newblk);
>>>>>>> refs/remotes/origin/master
		if (error)
			return(error);
		*result = 1;
	} else {
		*result = 0;
	}

	/*
	 * Insert the new entry(s) into the correct block
	 * (updating last hashval in the process).
	 *
<<<<<<< HEAD
	 * xfs_da_node_add() inserts BEFORE the given index,
=======
	 * xfs_da3_node_add() inserts BEFORE the given index,
>>>>>>> refs/remotes/origin/master
	 * and as a result of using node_lookup_int() we always
	 * point to a valid entry (not after one), but a split
	 * operation always results in a new block whose hashvals
	 * FOLLOW the current block.
	 *
	 * If we had double-split op below us, then add the extra block too.
	 */
<<<<<<< HEAD
	node = oldblk->bp->data;
	if (oldblk->index <= be16_to_cpu(node->hdr.count)) {
		oldblk->index++;
		xfs_da_node_add(state, oldblk, addblk);
		if (useextra) {
			if (state->extraafter)
				oldblk->index++;
			xfs_da_node_add(state, oldblk, &state->extrablk);
=======
	node = oldblk->bp->b_addr;
	dp->d_ops->node_hdr_from_disk(&nodehdr, node);
	if (oldblk->index <= nodehdr.count) {
		oldblk->index++;
		xfs_da3_node_add(state, oldblk, addblk);
		if (useextra) {
			if (state->extraafter)
				oldblk->index++;
			xfs_da3_node_add(state, oldblk, &state->extrablk);
>>>>>>> refs/remotes/origin/master
			state->extravalid = 0;
		}
	} else {
		newblk->index++;
<<<<<<< HEAD
		xfs_da_node_add(state, newblk, addblk);
		if (useextra) {
			if (state->extraafter)
				newblk->index++;
			xfs_da_node_add(state, newblk, &state->extrablk);
=======
		xfs_da3_node_add(state, newblk, addblk);
		if (useextra) {
			if (state->extraafter)
				newblk->index++;
			xfs_da3_node_add(state, newblk, &state->extrablk);
>>>>>>> refs/remotes/origin/master
			state->extravalid = 0;
		}
	}

	return(0);
}

/*
 * Balance the btree elements between two intermediate nodes,
 * usually one full and one empty.
 *
 * NOTE: if blk2 is empty, then it will get the upper half of blk1.
 */
STATIC void
<<<<<<< HEAD
xfs_da_node_rebalance(xfs_da_state_t *state, xfs_da_state_blk_t *blk1,
				     xfs_da_state_blk_t *blk2)
{
	xfs_da_intnode_t *node1, *node2, *tmpnode;
	xfs_da_node_entry_t *btree_s, *btree_d;
	int count, tmp;
	xfs_trans_t *tp;

<<<<<<< HEAD
=======
	trace_xfs_da_node_rebalance(state->args);

>>>>>>> refs/remotes/origin/cm-10.0
	node1 = blk1->bp->data;
	node2 = blk2->bp->data;
=======
xfs_da3_node_rebalance(
	struct xfs_da_state	*state,
	struct xfs_da_state_blk	*blk1,
	struct xfs_da_state_blk	*blk2)
{
	struct xfs_da_intnode	*node1;
	struct xfs_da_intnode	*node2;
	struct xfs_da_intnode	*tmpnode;
	struct xfs_da_node_entry *btree1;
	struct xfs_da_node_entry *btree2;
	struct xfs_da_node_entry *btree_s;
	struct xfs_da_node_entry *btree_d;
	struct xfs_da3_icnode_hdr nodehdr1;
	struct xfs_da3_icnode_hdr nodehdr2;
	struct xfs_trans	*tp;
	int			count;
	int			tmp;
	int			swap = 0;
	struct xfs_inode	*dp = state->args->dp;

	trace_xfs_da_node_rebalance(state->args);

	node1 = blk1->bp->b_addr;
	node2 = blk2->bp->b_addr;
	dp->d_ops->node_hdr_from_disk(&nodehdr1, node1);
	dp->d_ops->node_hdr_from_disk(&nodehdr2, node2);
	btree1 = dp->d_ops->node_tree_p(node1);
	btree2 = dp->d_ops->node_tree_p(node2);

>>>>>>> refs/remotes/origin/master
	/*
	 * Figure out how many entries need to move, and in which direction.
	 * Swap the nodes around if that makes it simpler.
	 */
<<<<<<< HEAD
	if ((be16_to_cpu(node1->hdr.count) > 0) && (be16_to_cpu(node2->hdr.count) > 0) &&
	    ((be32_to_cpu(node2->btree[0].hashval) < be32_to_cpu(node1->btree[0].hashval)) ||
	     (be32_to_cpu(node2->btree[be16_to_cpu(node2->hdr.count)-1].hashval) <
	      be32_to_cpu(node1->btree[be16_to_cpu(node1->hdr.count)-1].hashval)))) {
		tmpnode = node1;
		node1 = node2;
		node2 = tmpnode;
	}
<<<<<<< HEAD
	ASSERT(be16_to_cpu(node1->hdr.info.magic) == XFS_DA_NODE_MAGIC);
	ASSERT(be16_to_cpu(node2->hdr.info.magic) == XFS_DA_NODE_MAGIC);
=======
	ASSERT(node1->hdr.info.magic == cpu_to_be16(XFS_DA_NODE_MAGIC));
	ASSERT(node2->hdr.info.magic == cpu_to_be16(XFS_DA_NODE_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	count = (be16_to_cpu(node1->hdr.count) - be16_to_cpu(node2->hdr.count)) / 2;
=======
	if (nodehdr1.count > 0 && nodehdr2.count > 0 &&
	    ((be32_to_cpu(btree2[0].hashval) < be32_to_cpu(btree1[0].hashval)) ||
	     (be32_to_cpu(btree2[nodehdr2.count - 1].hashval) <
			be32_to_cpu(btree1[nodehdr1.count - 1].hashval)))) {
		tmpnode = node1;
		node1 = node2;
		node2 = tmpnode;
		dp->d_ops->node_hdr_from_disk(&nodehdr1, node1);
		dp->d_ops->node_hdr_from_disk(&nodehdr2, node2);
		btree1 = dp->d_ops->node_tree_p(node1);
		btree2 = dp->d_ops->node_tree_p(node2);
		swap = 1;
	}

	count = (nodehdr1.count - nodehdr2.count) / 2;
>>>>>>> refs/remotes/origin/master
	if (count == 0)
		return;
	tp = state->args->trans;
	/*
	 * Two cases: high-to-low and low-to-high.
	 */
	if (count > 0) {
		/*
		 * Move elements in node2 up to make a hole.
		 */
<<<<<<< HEAD
		if ((tmp = be16_to_cpu(node2->hdr.count)) > 0) {
			tmp *= (uint)sizeof(xfs_da_node_entry_t);
			btree_s = &node2->btree[0];
			btree_d = &node2->btree[count];
=======
		tmp = nodehdr2.count;
		if (tmp > 0) {
			tmp *= (uint)sizeof(xfs_da_node_entry_t);
			btree_s = &btree2[0];
			btree_d = &btree2[count];
>>>>>>> refs/remotes/origin/master
			memmove(btree_d, btree_s, tmp);
		}

		/*
		 * Move the req'd B-tree elements from high in node1 to
		 * low in node2.
		 */
<<<<<<< HEAD
		be16_add_cpu(&node2->hdr.count, count);
		tmp = count * (uint)sizeof(xfs_da_node_entry_t);
		btree_s = &node1->btree[be16_to_cpu(node1->hdr.count) - count];
		btree_d = &node2->btree[0];
		memcpy(btree_d, btree_s, tmp);
		be16_add_cpu(&node1->hdr.count, -count);
=======
		nodehdr2.count += count;
		tmp = count * (uint)sizeof(xfs_da_node_entry_t);
		btree_s = &btree1[nodehdr1.count - count];
		btree_d = &btree2[0];
		memcpy(btree_d, btree_s, tmp);
		nodehdr1.count -= count;
>>>>>>> refs/remotes/origin/master
	} else {
		/*
		 * Move the req'd B-tree elements from low in node2 to
		 * high in node1.
		 */
		count = -count;
		tmp = count * (uint)sizeof(xfs_da_node_entry_t);
<<<<<<< HEAD
		btree_s = &node2->btree[0];
		btree_d = &node1->btree[be16_to_cpu(node1->hdr.count)];
		memcpy(btree_d, btree_s, tmp);
		be16_add_cpu(&node1->hdr.count, count);
		xfs_da_log_buf(tp, blk1->bp,
=======
		btree_s = &btree2[0];
		btree_d = &btree1[nodehdr1.count];
		memcpy(btree_d, btree_s, tmp);
		nodehdr1.count += count;

		xfs_trans_log_buf(tp, blk1->bp,
>>>>>>> refs/remotes/origin/master
			XFS_DA_LOGRANGE(node1, btree_d, tmp));

		/*
		 * Move elements in node2 down to fill the hole.
		 */
<<<<<<< HEAD
		tmp  = be16_to_cpu(node2->hdr.count) - count;
		tmp *= (uint)sizeof(xfs_da_node_entry_t);
		btree_s = &node2->btree[count];
		btree_d = &node2->btree[0];
		memmove(btree_d, btree_s, tmp);
		be16_add_cpu(&node2->hdr.count, -count);
=======
		tmp  = nodehdr2.count - count;
		tmp *= (uint)sizeof(xfs_da_node_entry_t);
		btree_s = &btree2[count];
		btree_d = &btree2[0];
		memmove(btree_d, btree_s, tmp);
		nodehdr2.count -= count;
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * Log header of node 1 and all current bits of node 2.
	 */
<<<<<<< HEAD
	xfs_da_log_buf(tp, blk1->bp,
		XFS_DA_LOGRANGE(node1, &node1->hdr, sizeof(node1->hdr)));
	xfs_da_log_buf(tp, blk2->bp,
		XFS_DA_LOGRANGE(node2, &node2->hdr,
			sizeof(node2->hdr) +
			sizeof(node2->btree[0]) * be16_to_cpu(node2->hdr.count)));
=======
	dp->d_ops->node_hdr_to_disk(node1, &nodehdr1);
	xfs_trans_log_buf(tp, blk1->bp,
		XFS_DA_LOGRANGE(node1, &node1->hdr, dp->d_ops->node_hdr_size));

	dp->d_ops->node_hdr_to_disk(node2, &nodehdr2);
	xfs_trans_log_buf(tp, blk2->bp,
		XFS_DA_LOGRANGE(node2, &node2->hdr,
				dp->d_ops->node_hdr_size +
				(sizeof(btree2[0]) * nodehdr2.count)));
>>>>>>> refs/remotes/origin/master

	/*
	 * Record the last hashval from each block for upward propagation.
	 * (note: don't use the swapped node pointers)
	 */
<<<<<<< HEAD
	node1 = blk1->bp->data;
	node2 = blk2->bp->data;
	blk1->hashval = be32_to_cpu(node1->btree[be16_to_cpu(node1->hdr.count)-1].hashval);
	blk2->hashval = be32_to_cpu(node2->btree[be16_to_cpu(node2->hdr.count)-1].hashval);
=======
	if (swap) {
		node1 = blk1->bp->b_addr;
		node2 = blk2->bp->b_addr;
		dp->d_ops->node_hdr_from_disk(&nodehdr1, node1);
		dp->d_ops->node_hdr_from_disk(&nodehdr2, node2);
		btree1 = dp->d_ops->node_tree_p(node1);
		btree2 = dp->d_ops->node_tree_p(node2);
	}
	blk1->hashval = be32_to_cpu(btree1[nodehdr1.count - 1].hashval);
	blk2->hashval = be32_to_cpu(btree2[nodehdr2.count - 1].hashval);
>>>>>>> refs/remotes/origin/master

	/*
	 * Adjust the expected index for insertion.
	 */
<<<<<<< HEAD
	if (blk1->index >= be16_to_cpu(node1->hdr.count)) {
		blk2->index = blk1->index - be16_to_cpu(node1->hdr.count);
		blk1->index = be16_to_cpu(node1->hdr.count) + 1;	/* make it invalid */
=======
	if (blk1->index >= nodehdr1.count) {
		blk2->index = blk1->index - nodehdr1.count;
		blk1->index = nodehdr1.count + 1;	/* make it invalid */
>>>>>>> refs/remotes/origin/master
	}
}

/*
 * Add a new entry to an intermediate node.
 */
STATIC void
<<<<<<< HEAD
xfs_da_node_add(xfs_da_state_t *state, xfs_da_state_blk_t *oldblk,
			       xfs_da_state_blk_t *newblk)
{
	xfs_da_intnode_t *node;
	xfs_da_node_entry_t *btree;
	int tmp;

<<<<<<< HEAD
	node = oldblk->bp->data;
	ASSERT(be16_to_cpu(node->hdr.info.magic) == XFS_DA_NODE_MAGIC);
=======
	trace_xfs_da_node_add(state->args);

	node = oldblk->bp->data;
	ASSERT(node->hdr.info.magic == cpu_to_be16(XFS_DA_NODE_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	ASSERT((oldblk->index >= 0) && (oldblk->index <= be16_to_cpu(node->hdr.count)));
=======
xfs_da3_node_add(
	struct xfs_da_state	*state,
	struct xfs_da_state_blk	*oldblk,
	struct xfs_da_state_blk	*newblk)
{
	struct xfs_da_intnode	*node;
	struct xfs_da3_icnode_hdr nodehdr;
	struct xfs_da_node_entry *btree;
	int			tmp;
	struct xfs_inode	*dp = state->args->dp;

	trace_xfs_da_node_add(state->args);

	node = oldblk->bp->b_addr;
	dp->d_ops->node_hdr_from_disk(&nodehdr, node);
	btree = dp->d_ops->node_tree_p(node);

	ASSERT(oldblk->index >= 0 && oldblk->index <= nodehdr.count);
>>>>>>> refs/remotes/origin/master
	ASSERT(newblk->blkno != 0);
	if (state->args->whichfork == XFS_DATA_FORK)
		ASSERT(newblk->blkno >= state->mp->m_dirleafblk &&
		       newblk->blkno < state->mp->m_dirfreeblk);

	/*
	 * We may need to make some room before we insert the new node.
	 */
	tmp = 0;
<<<<<<< HEAD
	btree = &node->btree[ oldblk->index ];
	if (oldblk->index < be16_to_cpu(node->hdr.count)) {
		tmp = (be16_to_cpu(node->hdr.count) - oldblk->index) * (uint)sizeof(*btree);
		memmove(btree + 1, btree, tmp);
	}
	btree->hashval = cpu_to_be32(newblk->hashval);
	btree->before = cpu_to_be32(newblk->blkno);
	xfs_da_log_buf(state->args->trans, oldblk->bp,
		XFS_DA_LOGRANGE(node, btree, tmp + sizeof(*btree)));
	be16_add_cpu(&node->hdr.count, 1);
	xfs_da_log_buf(state->args->trans, oldblk->bp,
		XFS_DA_LOGRANGE(node, &node->hdr, sizeof(node->hdr)));
=======
	if (oldblk->index < nodehdr.count) {
		tmp = (nodehdr.count - oldblk->index) * (uint)sizeof(*btree);
		memmove(&btree[oldblk->index + 1], &btree[oldblk->index], tmp);
	}
	btree[oldblk->index].hashval = cpu_to_be32(newblk->hashval);
	btree[oldblk->index].before = cpu_to_be32(newblk->blkno);
	xfs_trans_log_buf(state->args->trans, oldblk->bp,
		XFS_DA_LOGRANGE(node, &btree[oldblk->index],
				tmp + sizeof(*btree)));

	nodehdr.count += 1;
	dp->d_ops->node_hdr_to_disk(node, &nodehdr);
	xfs_trans_log_buf(state->args->trans, oldblk->bp,
		XFS_DA_LOGRANGE(node, &node->hdr, dp->d_ops->node_hdr_size));
>>>>>>> refs/remotes/origin/master

	/*
	 * Copy the last hash value from the oldblk to propagate upwards.
	 */
<<<<<<< HEAD
	oldblk->hashval = be32_to_cpu(node->btree[be16_to_cpu(node->hdr.count)-1 ].hashval);
=======
	oldblk->hashval = be32_to_cpu(btree[nodehdr.count - 1].hashval);
>>>>>>> refs/remotes/origin/master
}

/*========================================================================
 * Routines used for shrinking the Btree.
 *========================================================================*/

/*
 * Deallocate an empty leaf node, remove it from its parent,
 * possibly deallocating that block, etc...
 */
int
<<<<<<< HEAD
xfs_da_join(xfs_da_state_t *state)
{
	xfs_da_state_blk_t *drop_blk, *save_blk;
	int action, error;

<<<<<<< HEAD
=======
	trace_xfs_da_join(state->args);

>>>>>>> refs/remotes/origin/cm-10.0
	action = 0;
=======
xfs_da3_join(
	struct xfs_da_state	*state)
{
	struct xfs_da_state_blk	*drop_blk;
	struct xfs_da_state_blk	*save_blk;
	int			action = 0;
	int			error;

	trace_xfs_da_join(state->args);

>>>>>>> refs/remotes/origin/master
	drop_blk = &state->path.blk[ state->path.active-1 ];
	save_blk = &state->altpath.blk[ state->path.active-1 ];
	ASSERT(state->path.blk[0].magic == XFS_DA_NODE_MAGIC);
	ASSERT(drop_blk->magic == XFS_ATTR_LEAF_MAGIC ||
	       drop_blk->magic == XFS_DIR2_LEAFN_MAGIC);

	/*
	 * Walk back up the tree joining/deallocating as necessary.
	 * When we stop dropping blocks, break out.
	 */
	for (  ; state->path.active >= 2; drop_blk--, save_blk--,
		 state->path.active--) {
		/*
		 * See if we can combine the block with a neighbor.
		 *   (action == 0) => no options, just leave
		 *   (action == 1) => coalesce, then unlink
		 *   (action == 2) => block empty, unlink it
		 */
		switch (drop_blk->magic) {
		case XFS_ATTR_LEAF_MAGIC:
<<<<<<< HEAD
			error = xfs_attr_leaf_toosmall(state, &action);
=======
			error = xfs_attr3_leaf_toosmall(state, &action);
>>>>>>> refs/remotes/origin/master
			if (error)
				return(error);
			if (action == 0)
				return(0);
<<<<<<< HEAD
			xfs_attr_leaf_unbalance(state, drop_blk, save_blk);
=======
			xfs_attr3_leaf_unbalance(state, drop_blk, save_blk);
>>>>>>> refs/remotes/origin/master
			break;
		case XFS_DIR2_LEAFN_MAGIC:
			error = xfs_dir2_leafn_toosmall(state, &action);
			if (error)
				return error;
			if (action == 0)
				return 0;
			xfs_dir2_leafn_unbalance(state, drop_blk, save_blk);
			break;
		case XFS_DA_NODE_MAGIC:
			/*
			 * Remove the offending node, fixup hashvals,
			 * check for a toosmall neighbor.
			 */
<<<<<<< HEAD
			xfs_da_node_remove(state, drop_blk);
			xfs_da_fixhashpath(state, &state->path);
			error = xfs_da_node_toosmall(state, &action);
=======
			xfs_da3_node_remove(state, drop_blk);
			xfs_da3_fixhashpath(state, &state->path);
			error = xfs_da3_node_toosmall(state, &action);
>>>>>>> refs/remotes/origin/master
			if (error)
				return(error);
			if (action == 0)
				return 0;
<<<<<<< HEAD
			xfs_da_node_unbalance(state, drop_blk, save_blk);
			break;
		}
		xfs_da_fixhashpath(state, &state->altpath);
		error = xfs_da_blk_unlink(state, drop_blk, save_blk);
=======
			xfs_da3_node_unbalance(state, drop_blk, save_blk);
			break;
		}
		xfs_da3_fixhashpath(state, &state->altpath);
		error = xfs_da3_blk_unlink(state, drop_blk, save_blk);
>>>>>>> refs/remotes/origin/master
		xfs_da_state_kill_altpath(state);
		if (error)
			return(error);
		error = xfs_da_shrink_inode(state->args, drop_blk->blkno,
							 drop_blk->bp);
		drop_blk->bp = NULL;
		if (error)
			return(error);
	}
	/*
	 * We joined all the way to the top.  If it turns out that
	 * we only have one entry in the root, make the child block
	 * the new root.
	 */
<<<<<<< HEAD
	xfs_da_node_remove(state, drop_blk);
	xfs_da_fixhashpath(state, &state->path);
	error = xfs_da_root_join(state, &state->path.blk[0]);
	return(error);
}

<<<<<<< HEAD
=======
=======
	xfs_da3_node_remove(state, drop_blk);
	xfs_da3_fixhashpath(state, &state->path);
	error = xfs_da3_root_join(state, &state->path.blk[0]);
	return(error);
}

>>>>>>> refs/remotes/origin/master
#ifdef	DEBUG
static void
xfs_da_blkinfo_onlychild_validate(struct xfs_da_blkinfo *blkinfo, __u16 level)
{
	__be16	magic = blkinfo->magic;

	if (level == 1) {
		ASSERT(magic == cpu_to_be16(XFS_DIR2_LEAFN_MAGIC) ||
<<<<<<< HEAD
		       magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC));
	} else
		ASSERT(magic == cpu_to_be16(XFS_DA_NODE_MAGIC));
=======
		       magic == cpu_to_be16(XFS_DIR3_LEAFN_MAGIC) ||
		       magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC) ||
		       magic == cpu_to_be16(XFS_ATTR3_LEAF_MAGIC));
	} else {
		ASSERT(magic == cpu_to_be16(XFS_DA_NODE_MAGIC) ||
		       magic == cpu_to_be16(XFS_DA3_NODE_MAGIC));
	}
>>>>>>> refs/remotes/origin/master
	ASSERT(!blkinfo->forw);
	ASSERT(!blkinfo->back);
}
#else	/* !DEBUG */
#define	xfs_da_blkinfo_onlychild_validate(blkinfo, level)
#endif	/* !DEBUG */

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * We have only one entry in the root.  Copy the only remaining child of
 * the old root to block 0 as the new root node.
 */
STATIC int
<<<<<<< HEAD
xfs_da_root_join(xfs_da_state_t *state, xfs_da_state_blk_t *root_blk)
{
	xfs_da_intnode_t *oldroot;
<<<<<<< HEAD
	/* REFERENCED */
	xfs_da_blkinfo_t *blkinfo;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	xfs_da_args_t *args;
	xfs_dablk_t child;
	xfs_dabuf_t *bp;
	int error;

<<<<<<< HEAD
=======
	trace_xfs_da_root_join(state->args);

>>>>>>> refs/remotes/origin/cm-10.0
	args = state->args;
	ASSERT(args != NULL);
	ASSERT(root_blk->magic == XFS_DA_NODE_MAGIC);
	oldroot = root_blk->bp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(oldroot->hdr.info.magic) == XFS_DA_NODE_MAGIC);
=======
	ASSERT(oldroot->hdr.info.magic == cpu_to_be16(XFS_DA_NODE_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	ASSERT(!oldroot->hdr.info.forw);
	ASSERT(!oldroot->hdr.info.back);
=======
xfs_da3_root_join(
	struct xfs_da_state	*state,
	struct xfs_da_state_blk	*root_blk)
{
	struct xfs_da_intnode	*oldroot;
	struct xfs_da_args	*args;
	xfs_dablk_t		child;
	struct xfs_buf		*bp;
	struct xfs_da3_icnode_hdr oldroothdr;
	struct xfs_da_node_entry *btree;
	int			error;
	struct xfs_inode	*dp = state->args->dp;

	trace_xfs_da_root_join(state->args);

	ASSERT(root_blk->magic == XFS_DA_NODE_MAGIC);

	args = state->args;
	oldroot = root_blk->bp->b_addr;
	dp->d_ops->node_hdr_from_disk(&oldroothdr, oldroot);
	ASSERT(oldroothdr.forw == 0);
	ASSERT(oldroothdr.back == 0);
>>>>>>> refs/remotes/origin/master

	/*
	 * If the root has more than one child, then don't do anything.
	 */
<<<<<<< HEAD
	if (be16_to_cpu(oldroot->hdr.count) > 1)
		return(0);
=======
	if (oldroothdr.count > 1)
		return 0;
>>>>>>> refs/remotes/origin/master

	/*
	 * Read in the (only) child block, then copy those bytes into
	 * the root block's buffer and free the original child block.
	 */
<<<<<<< HEAD
	child = be32_to_cpu(oldroot->btree[0].before);
	ASSERT(child != 0);
	error = xfs_da_read_buf(args->trans, args->dp, child, -1, &bp,
					     args->whichfork);
	if (error)
		return(error);
	ASSERT(bp != NULL);
<<<<<<< HEAD
	blkinfo = bp->data;
	if (be16_to_cpu(oldroot->hdr.level) == 1) {
		ASSERT(be16_to_cpu(blkinfo->magic) == XFS_DIR2_LEAFN_MAGIC ||
		       be16_to_cpu(blkinfo->magic) == XFS_ATTR_LEAF_MAGIC);
	} else {
		ASSERT(be16_to_cpu(blkinfo->magic) == XFS_DA_NODE_MAGIC);
	}
	ASSERT(!blkinfo->forw);
	ASSERT(!blkinfo->back);
=======
	xfs_da_blkinfo_onlychild_validate(bp->data,
					be16_to_cpu(oldroot->hdr.level));

>>>>>>> refs/remotes/origin/cm-10.0
	memcpy(root_blk->bp->data, bp->data, state->blocksize);
	xfs_da_log_buf(args->trans, root_blk->bp, 0, state->blocksize - 1);
=======
	btree = dp->d_ops->node_tree_p(oldroot);
	child = be32_to_cpu(btree[0].before);
	ASSERT(child != 0);
	error = xfs_da3_node_read(args->trans, dp, child, -1, &bp,
					     args->whichfork);
	if (error)
		return error;
	xfs_da_blkinfo_onlychild_validate(bp->b_addr, oldroothdr.level);

	/*
	 * This could be copying a leaf back into the root block in the case of
	 * there only being a single leaf block left in the tree. Hence we have
	 * to update the b_ops pointer as well to match the buffer type change
	 * that could occur. For dir3 blocks we also need to update the block
	 * number in the buffer header.
	 */
	memcpy(root_blk->bp->b_addr, bp->b_addr, state->blocksize);
	root_blk->bp->b_ops = bp->b_ops;
	xfs_trans_buf_copy_type(root_blk->bp, bp);
	if (oldroothdr.magic == XFS_DA3_NODE_MAGIC) {
		struct xfs_da3_blkinfo *da3 = root_blk->bp->b_addr;
		da3->blkno = cpu_to_be64(root_blk->bp->b_bn);
	}
	xfs_trans_log_buf(args->trans, root_blk->bp, 0, state->blocksize - 1);
>>>>>>> refs/remotes/origin/master
	error = xfs_da_shrink_inode(args, child, bp);
	return(error);
}

/*
 * Check a node block and its neighbors to see if the block should be
 * collapsed into one or the other neighbor.  Always keep the block
 * with the smaller block number.
 * If the current block is over 50% full, don't try to join it, return 0.
 * If the block is empty, fill in the state structure and return 2.
 * If it can be collapsed, fill in the state structure and return 1.
 * If nothing can be done, return 0.
 */
STATIC int
<<<<<<< HEAD
xfs_da_node_toosmall(xfs_da_state_t *state, int *action)
{
	xfs_da_intnode_t *node;
	xfs_da_state_blk_t *blk;
	xfs_da_blkinfo_t *info;
	int count, forward, error, retval, i;
	xfs_dablk_t blkno;
	xfs_dabuf_t *bp;
=======
xfs_da3_node_toosmall(
	struct xfs_da_state	*state,
	int			*action)
{
	struct xfs_da_intnode	*node;
	struct xfs_da_state_blk	*blk;
	struct xfs_da_blkinfo	*info;
	xfs_dablk_t		blkno;
	struct xfs_buf		*bp;
	struct xfs_da3_icnode_hdr nodehdr;
	int			count;
	int			forward;
	int			error;
	int			retval;
	int			i;
	struct xfs_inode	*dp = state->args->dp;

	trace_xfs_da_node_toosmall(state->args);
>>>>>>> refs/remotes/origin/master

	/*
	 * Check for the degenerate case of the block being over 50% full.
	 * If so, it's not worth even looking to see if we might be able
	 * to coalesce with a sibling.
	 */
	blk = &state->path.blk[ state->path.active-1 ];
<<<<<<< HEAD
	info = blk->bp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(info->magic) == XFS_DA_NODE_MAGIC);
=======
	ASSERT(info->magic == cpu_to_be16(XFS_DA_NODE_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	node = (xfs_da_intnode_t *)info;
	count = be16_to_cpu(node->hdr.count);
	if (count > (state->node_ents >> 1)) {
=======
	info = blk->bp->b_addr;
	node = (xfs_da_intnode_t *)info;
	dp->d_ops->node_hdr_from_disk(&nodehdr, node);
	if (nodehdr.count > (state->node_ents >> 1)) {
>>>>>>> refs/remotes/origin/master
		*action = 0;	/* blk over 50%, don't try to join */
		return(0);	/* blk over 50%, don't try to join */
	}

	/*
	 * Check for the degenerate case of the block being empty.
	 * If the block is empty, we'll simply delete it, no need to
	 * coalesce it with a sibling block.  We choose (arbitrarily)
	 * to merge with the forward block unless it is NULL.
	 */
<<<<<<< HEAD
	if (count == 0) {
=======
	if (nodehdr.count == 0) {
>>>>>>> refs/remotes/origin/master
		/*
		 * Make altpath point to the block we want to keep and
		 * path point to the block we want to drop (this one).
		 */
		forward = (info->forw != 0);
		memcpy(&state->altpath, &state->path, sizeof(state->path));
<<<<<<< HEAD
		error = xfs_da_path_shift(state, &state->altpath, forward,
=======
		error = xfs_da3_path_shift(state, &state->altpath, forward,
>>>>>>> refs/remotes/origin/master
						 0, &retval);
		if (error)
			return(error);
		if (retval) {
			*action = 0;
		} else {
			*action = 2;
		}
		return(0);
	}

	/*
	 * Examine each sibling block to see if we can coalesce with
	 * at least 25% free space to spare.  We need to figure out
	 * whether to merge with the forward or the backward block.
	 * We prefer coalescing with the lower numbered sibling so as
	 * to shrink a directory over time.
	 */
<<<<<<< HEAD
	/* start with smaller blk num */
	forward = (be32_to_cpu(info->forw) < be32_to_cpu(info->back));
	for (i = 0; i < 2; forward = !forward, i++) {
		if (forward)
			blkno = be32_to_cpu(info->forw);
		else
			blkno = be32_to_cpu(info->back);
		if (blkno == 0)
			continue;
		error = xfs_da_read_buf(state->args->trans, state->args->dp,
					blkno, -1, &bp, state->args->whichfork);
		if (error)
			return(error);
		ASSERT(bp != NULL);

		node = (xfs_da_intnode_t *)info;
		count  = state->node_ents;
		count -= state->node_ents >> 2;
		count -= be16_to_cpu(node->hdr.count);
		node = bp->data;
<<<<<<< HEAD
		ASSERT(be16_to_cpu(node->hdr.info.magic) == XFS_DA_NODE_MAGIC);
=======
		ASSERT(node->hdr.info.magic == cpu_to_be16(XFS_DA_NODE_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
		count -= be16_to_cpu(node->hdr.count);
		xfs_da_brelse(state->args->trans, bp);
		if (count >= 0)
=======
	count  = state->node_ents;
	count -= state->node_ents >> 2;
	count -= nodehdr.count;

	/* start with smaller blk num */
	forward = nodehdr.forw < nodehdr.back;
	for (i = 0; i < 2; forward = !forward, i++) {
		struct xfs_da3_icnode_hdr thdr;
		if (forward)
			blkno = nodehdr.forw;
		else
			blkno = nodehdr.back;
		if (blkno == 0)
			continue;
		error = xfs_da3_node_read(state->args->trans, dp,
					blkno, -1, &bp, state->args->whichfork);
		if (error)
			return(error);

		node = bp->b_addr;
		dp->d_ops->node_hdr_from_disk(&thdr, node);
		xfs_trans_brelse(state->args->trans, bp);

		if (count - thdr.count >= 0)
>>>>>>> refs/remotes/origin/master
			break;	/* fits with at least 25% to spare */
	}
	if (i >= 2) {
		*action = 0;
<<<<<<< HEAD
		return(0);
=======
		return 0;
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * Make altpath point to the block we want to keep (the lower
	 * numbered block) and path point to the block we want to drop.
	 */
	memcpy(&state->altpath, &state->path, sizeof(state->path));
	if (blkno < blk->blkno) {
<<<<<<< HEAD
		error = xfs_da_path_shift(state, &state->altpath, forward,
						 0, &retval);
		if (error) {
			return(error);
		}
		if (retval) {
			*action = 0;
			return(0);
		}
	} else {
		error = xfs_da_path_shift(state, &state->path, forward,
						 0, &retval);
		if (error) {
			return(error);
		}
		if (retval) {
			*action = 0;
			return(0);
		}
	}
	*action = 1;
	return(0);
=======
		error = xfs_da3_path_shift(state, &state->altpath, forward,
						 0, &retval);
	} else {
		error = xfs_da3_path_shift(state, &state->path, forward,
						 0, &retval);
	}
	if (error)
		return error;
	if (retval) {
		*action = 0;
		return 0;
	}
	*action = 1;
	return 0;
}

/*
 * Pick up the last hashvalue from an intermediate node.
 */
STATIC uint
xfs_da3_node_lasthash(
	struct xfs_inode	*dp,
	struct xfs_buf		*bp,
	int			*count)
{
	struct xfs_da_intnode	 *node;
	struct xfs_da_node_entry *btree;
	struct xfs_da3_icnode_hdr nodehdr;

	node = bp->b_addr;
	dp->d_ops->node_hdr_from_disk(&nodehdr, node);
	if (count)
		*count = nodehdr.count;
	if (!nodehdr.count)
		return 0;
	btree = dp->d_ops->node_tree_p(node);
	return be32_to_cpu(btree[nodehdr.count - 1].hashval);
>>>>>>> refs/remotes/origin/master
}

/*
 * Walk back up the tree adjusting hash values as necessary,
 * when we stop making changes, return.
 */
void
<<<<<<< HEAD
xfs_da_fixhashpath(xfs_da_state_t *state, xfs_da_state_path_t *path)
{
	xfs_da_state_blk_t *blk;
	xfs_da_intnode_t *node;
	xfs_da_node_entry_t *btree;
	xfs_dahash_t lasthash=0;
	int level, count;
=======
xfs_da3_fixhashpath(
	struct xfs_da_state	*state,
	struct xfs_da_state_path *path)
{
	struct xfs_da_state_blk	*blk;
	struct xfs_da_intnode	*node;
	struct xfs_da_node_entry *btree;
	xfs_dahash_t		lasthash=0;
	int			level;
	int			count;
	struct xfs_inode	*dp = state->args->dp;

	trace_xfs_da_fixhashpath(state->args);
>>>>>>> refs/remotes/origin/master

	level = path->active-1;
	blk = &path->blk[ level ];
	switch (blk->magic) {
	case XFS_ATTR_LEAF_MAGIC:
		lasthash = xfs_attr_leaf_lasthash(blk->bp, &count);
		if (count == 0)
			return;
		break;
	case XFS_DIR2_LEAFN_MAGIC:
<<<<<<< HEAD
		lasthash = xfs_dir2_leafn_lasthash(blk->bp, &count);
=======
		lasthash = xfs_dir2_leafn_lasthash(dp, blk->bp, &count);
>>>>>>> refs/remotes/origin/master
		if (count == 0)
			return;
		break;
	case XFS_DA_NODE_MAGIC:
<<<<<<< HEAD
		lasthash = xfs_da_node_lasthash(blk->bp, &count);
=======
		lasthash = xfs_da3_node_lasthash(dp, blk->bp, &count);
>>>>>>> refs/remotes/origin/master
		if (count == 0)
			return;
		break;
	}
	for (blk--, level--; level >= 0; blk--, level--) {
<<<<<<< HEAD
		node = blk->bp->data;
<<<<<<< HEAD
		ASSERT(be16_to_cpu(node->hdr.info.magic) == XFS_DA_NODE_MAGIC);
=======
		ASSERT(node->hdr.info.magic == cpu_to_be16(XFS_DA_NODE_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
		btree = &node->btree[ blk->index ];
		if (be32_to_cpu(btree->hashval) == lasthash)
			break;
		blk->hashval = lasthash;
		btree->hashval = cpu_to_be32(lasthash);
		xfs_da_log_buf(state->args->trans, blk->bp,
				  XFS_DA_LOGRANGE(node, btree, sizeof(*btree)));

		lasthash = be32_to_cpu(node->btree[be16_to_cpu(node->hdr.count)-1].hashval);
=======
		struct xfs_da3_icnode_hdr nodehdr;

		node = blk->bp->b_addr;
		dp->d_ops->node_hdr_from_disk(&nodehdr, node);
		btree = dp->d_ops->node_tree_p(node);
		if (be32_to_cpu(btree->hashval) == lasthash)
			break;
		blk->hashval = lasthash;
		btree[blk->index].hashval = cpu_to_be32(lasthash);
		xfs_trans_log_buf(state->args->trans, blk->bp,
				  XFS_DA_LOGRANGE(node, &btree[blk->index],
						  sizeof(*btree)));

		lasthash = be32_to_cpu(btree[nodehdr.count - 1].hashval);
>>>>>>> refs/remotes/origin/master
	}
}

/*
 * Remove an entry from an intermediate node.
 */
STATIC void
<<<<<<< HEAD
xfs_da_node_remove(xfs_da_state_t *state, xfs_da_state_blk_t *drop_blk)
{
	xfs_da_intnode_t *node;
	xfs_da_node_entry_t *btree;
	int tmp;

<<<<<<< HEAD
=======
	trace_xfs_da_node_remove(state->args);

>>>>>>> refs/remotes/origin/cm-10.0
	node = drop_blk->bp->data;
	ASSERT(drop_blk->index < be16_to_cpu(node->hdr.count));
=======
xfs_da3_node_remove(
	struct xfs_da_state	*state,
	struct xfs_da_state_blk	*drop_blk)
{
	struct xfs_da_intnode	*node;
	struct xfs_da3_icnode_hdr nodehdr;
	struct xfs_da_node_entry *btree;
	int			index;
	int			tmp;
	struct xfs_inode	*dp = state->args->dp;

	trace_xfs_da_node_remove(state->args);

	node = drop_blk->bp->b_addr;
	dp->d_ops->node_hdr_from_disk(&nodehdr, node);
	ASSERT(drop_blk->index < nodehdr.count);
>>>>>>> refs/remotes/origin/master
	ASSERT(drop_blk->index >= 0);

	/*
	 * Copy over the offending entry, or just zero it out.
	 */
<<<<<<< HEAD
	btree = &node->btree[drop_blk->index];
	if (drop_blk->index < (be16_to_cpu(node->hdr.count)-1)) {
		tmp  = be16_to_cpu(node->hdr.count) - drop_blk->index - 1;
		tmp *= (uint)sizeof(xfs_da_node_entry_t);
		memmove(btree, btree + 1, tmp);
		xfs_da_log_buf(state->args->trans, drop_blk->bp,
		    XFS_DA_LOGRANGE(node, btree, tmp));
		btree = &node->btree[be16_to_cpu(node->hdr.count)-1];
	}
	memset((char *)btree, 0, sizeof(xfs_da_node_entry_t));
	xfs_da_log_buf(state->args->trans, drop_blk->bp,
	    XFS_DA_LOGRANGE(node, btree, sizeof(*btree)));
	be16_add_cpu(&node->hdr.count, -1);
	xfs_da_log_buf(state->args->trans, drop_blk->bp,
	    XFS_DA_LOGRANGE(node, &node->hdr, sizeof(node->hdr)));
=======
	index = drop_blk->index;
	btree = dp->d_ops->node_tree_p(node);
	if (index < nodehdr.count - 1) {
		tmp  = nodehdr.count - index - 1;
		tmp *= (uint)sizeof(xfs_da_node_entry_t);
		memmove(&btree[index], &btree[index + 1], tmp);
		xfs_trans_log_buf(state->args->trans, drop_blk->bp,
		    XFS_DA_LOGRANGE(node, &btree[index], tmp));
		index = nodehdr.count - 1;
	}
	memset(&btree[index], 0, sizeof(xfs_da_node_entry_t));
	xfs_trans_log_buf(state->args->trans, drop_blk->bp,
	    XFS_DA_LOGRANGE(node, &btree[index], sizeof(btree[index])));
	nodehdr.count -= 1;
	dp->d_ops->node_hdr_to_disk(node, &nodehdr);
	xfs_trans_log_buf(state->args->trans, drop_blk->bp,
	    XFS_DA_LOGRANGE(node, &node->hdr, dp->d_ops->node_hdr_size));
>>>>>>> refs/remotes/origin/master

	/*
	 * Copy the last hash value from the block to propagate upwards.
	 */
<<<<<<< HEAD
	btree--;
	drop_blk->hashval = be32_to_cpu(btree->hashval);
}

/*
 * Unbalance the btree elements between two intermediate nodes,
 * move all Btree elements from one node into another.
 */
STATIC void
xfs_da_node_unbalance(xfs_da_state_t *state, xfs_da_state_blk_t *drop_blk,
				     xfs_da_state_blk_t *save_blk)
{
	xfs_da_intnode_t *drop_node, *save_node;
	xfs_da_node_entry_t *btree;
	int tmp;
	xfs_trans_t *tp;

<<<<<<< HEAD
	drop_node = drop_blk->bp->data;
	save_node = save_blk->bp->data;
	ASSERT(be16_to_cpu(drop_node->hdr.info.magic) == XFS_DA_NODE_MAGIC);
	ASSERT(be16_to_cpu(save_node->hdr.info.magic) == XFS_DA_NODE_MAGIC);
=======
	trace_xfs_da_node_unbalance(state->args);

	drop_node = drop_blk->bp->data;
	save_node = save_blk->bp->data;
	ASSERT(drop_node->hdr.info.magic == cpu_to_be16(XFS_DA_NODE_MAGIC));
	ASSERT(save_node->hdr.info.magic == cpu_to_be16(XFS_DA_NODE_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	drop_blk->hashval = be32_to_cpu(btree[index - 1].hashval);
}

/*
 * Unbalance the elements between two intermediate nodes,
 * move all Btree elements from one node into another.
 */
STATIC void
xfs_da3_node_unbalance(
	struct xfs_da_state	*state,
	struct xfs_da_state_blk	*drop_blk,
	struct xfs_da_state_blk	*save_blk)
{
	struct xfs_da_intnode	*drop_node;
	struct xfs_da_intnode	*save_node;
	struct xfs_da_node_entry *drop_btree;
	struct xfs_da_node_entry *save_btree;
	struct xfs_da3_icnode_hdr drop_hdr;
	struct xfs_da3_icnode_hdr save_hdr;
	struct xfs_trans	*tp;
	int			sindex;
	int			tmp;
	struct xfs_inode	*dp = state->args->dp;

	trace_xfs_da_node_unbalance(state->args);

	drop_node = drop_blk->bp->b_addr;
	save_node = save_blk->bp->b_addr;
	dp->d_ops->node_hdr_from_disk(&drop_hdr, drop_node);
	dp->d_ops->node_hdr_from_disk(&save_hdr, save_node);
	drop_btree = dp->d_ops->node_tree_p(drop_node);
	save_btree = dp->d_ops->node_tree_p(save_node);
>>>>>>> refs/remotes/origin/master
	tp = state->args->trans;

	/*
	 * If the dying block has lower hashvals, then move all the
	 * elements in the remaining block up to make a hole.
	 */
<<<<<<< HEAD
	if ((be32_to_cpu(drop_node->btree[0].hashval) < be32_to_cpu(save_node->btree[ 0 ].hashval)) ||
	    (be32_to_cpu(drop_node->btree[be16_to_cpu(drop_node->hdr.count)-1].hashval) <
	     be32_to_cpu(save_node->btree[be16_to_cpu(save_node->hdr.count)-1].hashval)))
	{
		btree = &save_node->btree[be16_to_cpu(drop_node->hdr.count)];
		tmp = be16_to_cpu(save_node->hdr.count) * (uint)sizeof(xfs_da_node_entry_t);
		memmove(btree, &save_node->btree[0], tmp);
		btree = &save_node->btree[0];
		xfs_da_log_buf(tp, save_blk->bp,
			XFS_DA_LOGRANGE(save_node, btree,
				(be16_to_cpu(save_node->hdr.count) + be16_to_cpu(drop_node->hdr.count)) *
				sizeof(xfs_da_node_entry_t)));
	} else {
		btree = &save_node->btree[be16_to_cpu(save_node->hdr.count)];
		xfs_da_log_buf(tp, save_blk->bp,
			XFS_DA_LOGRANGE(save_node, btree,
				be16_to_cpu(drop_node->hdr.count) *
				sizeof(xfs_da_node_entry_t)));
=======
	if ((be32_to_cpu(drop_btree[0].hashval) <
			be32_to_cpu(save_btree[0].hashval)) ||
	    (be32_to_cpu(drop_btree[drop_hdr.count - 1].hashval) <
			be32_to_cpu(save_btree[save_hdr.count - 1].hashval))) {
		/* XXX: check this - is memmove dst correct? */
		tmp = save_hdr.count * sizeof(xfs_da_node_entry_t);
		memmove(&save_btree[drop_hdr.count], &save_btree[0], tmp);

		sindex = 0;
		xfs_trans_log_buf(tp, save_blk->bp,
			XFS_DA_LOGRANGE(save_node, &save_btree[0],
				(save_hdr.count + drop_hdr.count) *
						sizeof(xfs_da_node_entry_t)));
	} else {
		sindex = save_hdr.count;
		xfs_trans_log_buf(tp, save_blk->bp,
			XFS_DA_LOGRANGE(save_node, &save_btree[sindex],
				drop_hdr.count * sizeof(xfs_da_node_entry_t)));
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * Move all the B-tree elements from drop_blk to save_blk.
	 */
<<<<<<< HEAD
	tmp = be16_to_cpu(drop_node->hdr.count) * (uint)sizeof(xfs_da_node_entry_t);
	memcpy(btree, &drop_node->btree[0], tmp);
	be16_add_cpu(&save_node->hdr.count, be16_to_cpu(drop_node->hdr.count));

	xfs_da_log_buf(tp, save_blk->bp,
		XFS_DA_LOGRANGE(save_node, &save_node->hdr,
			sizeof(save_node->hdr)));
=======
	tmp = drop_hdr.count * (uint)sizeof(xfs_da_node_entry_t);
	memcpy(&save_btree[sindex], &drop_btree[0], tmp);
	save_hdr.count += drop_hdr.count;

	dp->d_ops->node_hdr_to_disk(save_node, &save_hdr);
	xfs_trans_log_buf(tp, save_blk->bp,
		XFS_DA_LOGRANGE(save_node, &save_node->hdr,
				dp->d_ops->node_hdr_size));
>>>>>>> refs/remotes/origin/master

	/*
	 * Save the last hashval in the remaining block for upward propagation.
	 */
<<<<<<< HEAD
	save_blk->hashval = be32_to_cpu(save_node->btree[be16_to_cpu(save_node->hdr.count)-1].hashval);
=======
	save_blk->hashval = be32_to_cpu(save_btree[save_hdr.count - 1].hashval);
>>>>>>> refs/remotes/origin/master
}

/*========================================================================
 * Routines used for finding things in the Btree.
 *========================================================================*/

/*
 * Walk down the Btree looking for a particular filename, filling
 * in the state structure as we go.
 *
 * We will set the state structure to point to each of the elements
 * in each of the nodes where either the hashval is or should be.
 *
 * We support duplicate hashval's so for each entry in the current
 * node that could contain the desired hashval, descend.  This is a
 * pruned depth-first tree search.
 */
int							/* error */
<<<<<<< HEAD
xfs_da_node_lookup_int(xfs_da_state_t *state, int *result)
{
	xfs_da_state_blk_t *blk;
	xfs_da_blkinfo_t *curr;
	xfs_da_intnode_t *node;
	xfs_da_node_entry_t *btree;
	xfs_dablk_t blkno;
	int probe, span, max, error, retval;
	xfs_dahash_t hashval, btreehashval;
	xfs_da_args_t *args;
=======
xfs_da3_node_lookup_int(
	struct xfs_da_state	*state,
	int			*result)
{
	struct xfs_da_state_blk	*blk;
	struct xfs_da_blkinfo	*curr;
	struct xfs_da_intnode	*node;
	struct xfs_da_node_entry *btree;
	struct xfs_da3_icnode_hdr nodehdr;
	struct xfs_da_args	*args;
	xfs_dablk_t		blkno;
	xfs_dahash_t		hashval;
	xfs_dahash_t		btreehashval;
	int			probe;
	int			span;
	int			max;
	int			error;
	int			retval;
	struct xfs_inode	*dp = state->args->dp;
>>>>>>> refs/remotes/origin/master

	args = state->args;

	/*
	 * Descend thru the B-tree searching each level for the right
	 * node to use, until the right hashval is found.
	 */
	blkno = (args->whichfork == XFS_DATA_FORK)? state->mp->m_dirleafblk : 0;
	for (blk = &state->path.blk[0], state->path.active = 1;
			 state->path.active <= XFS_DA_NODE_MAXDEPTH;
			 blk++, state->path.active++) {
		/*
		 * Read the next node down in the tree.
		 */
		blk->blkno = blkno;
<<<<<<< HEAD
		error = xfs_da_read_buf(args->trans, args->dp, blkno,
=======
		error = xfs_da3_node_read(args->trans, args->dp, blkno,
>>>>>>> refs/remotes/origin/master
					-1, &blk->bp, args->whichfork);
		if (error) {
			blk->blkno = 0;
			state->path.active--;
			return(error);
		}
<<<<<<< HEAD
		curr = blk->bp->data;
		blk->magic = be16_to_cpu(curr->magic);
		ASSERT(blk->magic == XFS_DA_NODE_MAGIC ||
		       blk->magic == XFS_DIR2_LEAFN_MAGIC ||
		       blk->magic == XFS_ATTR_LEAF_MAGIC);
=======
		curr = blk->bp->b_addr;
		blk->magic = be16_to_cpu(curr->magic);

		if (blk->magic == XFS_ATTR_LEAF_MAGIC ||
		    blk->magic == XFS_ATTR3_LEAF_MAGIC) {
			blk->magic = XFS_ATTR_LEAF_MAGIC;
			blk->hashval = xfs_attr_leaf_lasthash(blk->bp, NULL);
			break;
		}

		if (blk->magic == XFS_DIR2_LEAFN_MAGIC ||
		    blk->magic == XFS_DIR3_LEAFN_MAGIC) {
			blk->magic = XFS_DIR2_LEAFN_MAGIC;
			blk->hashval = xfs_dir2_leafn_lasthash(args->dp,
							       blk->bp, NULL);
			break;
		}

		blk->magic = XFS_DA_NODE_MAGIC;

>>>>>>> refs/remotes/origin/master

		/*
		 * Search an intermediate node for a match.
		 */
<<<<<<< HEAD
		if (blk->magic == XFS_DA_NODE_MAGIC) {
			node = blk->bp->data;
			max = be16_to_cpu(node->hdr.count);
			blk->hashval = be32_to_cpu(node->btree[max-1].hashval);

			/*
			 * Binary search.  (note: small blocks will skip loop)
			 */
			probe = span = max / 2;
			hashval = args->hashval;
			for (btree = &node->btree[probe]; span > 4;
				   btree = &node->btree[probe]) {
				span /= 2;
				btreehashval = be32_to_cpu(btree->hashval);
				if (btreehashval < hashval)
					probe += span;
				else if (btreehashval > hashval)
					probe -= span;
				else
					break;
			}
			ASSERT((probe >= 0) && (probe < max));
			ASSERT((span <= 4) || (be32_to_cpu(btree->hashval) == hashval));

			/*
			 * Since we may have duplicate hashval's, find the first
			 * matching hashval in the node.
			 */
			while ((probe > 0) && (be32_to_cpu(btree->hashval) >= hashval)) {
				btree--;
				probe--;
			}
			while ((probe < max) && (be32_to_cpu(btree->hashval) < hashval)) {
				btree++;
				probe++;
			}

			/*
			 * Pick the right block to descend on.
			 */
			if (probe == max) {
				blk->index = max-1;
				blkno = be32_to_cpu(node->btree[max-1].before);
			} else {
				blk->index = probe;
				blkno = be32_to_cpu(btree->before);
			}
		} else if (blk->magic == XFS_ATTR_LEAF_MAGIC) {
			blk->hashval = xfs_attr_leaf_lasthash(blk->bp, NULL);
			break;
		} else if (blk->magic == XFS_DIR2_LEAFN_MAGIC) {
			blk->hashval = xfs_dir2_leafn_lasthash(blk->bp, NULL);
			break;
		}
	}
=======
		node = blk->bp->b_addr;
		dp->d_ops->node_hdr_from_disk(&nodehdr, node);
		btree = dp->d_ops->node_tree_p(node);

		max = nodehdr.count;
		blk->hashval = be32_to_cpu(btree[max - 1].hashval);

		/*
		 * Binary search.  (note: small blocks will skip loop)
		 */
		probe = span = max / 2;
		hashval = args->hashval;
		while (span > 4) {
			span /= 2;
			btreehashval = be32_to_cpu(btree[probe].hashval);
			if (btreehashval < hashval)
				probe += span;
			else if (btreehashval > hashval)
				probe -= span;
			else
				break;
		}
		ASSERT((probe >= 0) && (probe < max));
		ASSERT((span <= 4) ||
			(be32_to_cpu(btree[probe].hashval) == hashval));

		/*
		 * Since we may have duplicate hashval's, find the first
		 * matching hashval in the node.
		 */
		while (probe > 0 &&
		       be32_to_cpu(btree[probe].hashval) >= hashval) {
			probe--;
		}
		while (probe < max &&
		       be32_to_cpu(btree[probe].hashval) < hashval) {
			probe++;
		}

		/*
		 * Pick the right block to descend on.
		 */
		if (probe == max) {
			blk->index = max - 1;
			blkno = be32_to_cpu(btree[max - 1].before);
		} else {
			blk->index = probe;
			blkno = be32_to_cpu(btree[probe].before);
		}
	}
>>>>>>> refs/remotes/origin/master

	/*
	 * A leaf block that ends in the hashval that we are interested in
	 * (final hashval == search hashval) means that the next block may
	 * contain more entries with the same hashval, shift upward to the
	 * next leaf and keep searching.
	 */
	for (;;) {
		if (blk->magic == XFS_DIR2_LEAFN_MAGIC) {
			retval = xfs_dir2_leafn_lookup_int(blk->bp, args,
							&blk->index, state);
		} else if (blk->magic == XFS_ATTR_LEAF_MAGIC) {
<<<<<<< HEAD
			retval = xfs_attr_leaf_lookup_int(blk->bp, args);
=======
			retval = xfs_attr3_leaf_lookup_int(blk->bp, args);
>>>>>>> refs/remotes/origin/master
			blk->index = args->index;
			args->blkno = blk->blkno;
		} else {
			ASSERT(0);
			return XFS_ERROR(EFSCORRUPTED);
		}
		if (((retval == ENOENT) || (retval == ENOATTR)) &&
		    (blk->hashval == args->hashval)) {
<<<<<<< HEAD
			error = xfs_da_path_shift(state, &state->path, 1, 1,
=======
			error = xfs_da3_path_shift(state, &state->path, 1, 1,
>>>>>>> refs/remotes/origin/master
							 &retval);
			if (error)
				return(error);
			if (retval == 0) {
				continue;
			} else if (blk->magic == XFS_ATTR_LEAF_MAGIC) {
				/* path_shift() gives ENOENT */
				retval = XFS_ERROR(ENOATTR);
			}
		}
		break;
	}
	*result = retval;
	return(0);
}

/*========================================================================
 * Utility routines.
 *========================================================================*/

/*
<<<<<<< HEAD
 * Link a new block into a doubly linked list of blocks (of whatever type).
 */
int							/* error */
xfs_da_blk_link(xfs_da_state_t *state, xfs_da_state_blk_t *old_blk,
			       xfs_da_state_blk_t *new_blk)
{
	xfs_da_blkinfo_t *old_info, *new_info, *tmp_info;
	xfs_da_args_t *args;
	int before=0, error;
	xfs_dabuf_t *bp;
=======
 * Compare two intermediate nodes for "order".
 */
STATIC int
xfs_da3_node_order(
	struct xfs_inode *dp,
	struct xfs_buf	*node1_bp,
	struct xfs_buf	*node2_bp)
{
	struct xfs_da_intnode	*node1;
	struct xfs_da_intnode	*node2;
	struct xfs_da_node_entry *btree1;
	struct xfs_da_node_entry *btree2;
	struct xfs_da3_icnode_hdr node1hdr;
	struct xfs_da3_icnode_hdr node2hdr;

	node1 = node1_bp->b_addr;
	node2 = node2_bp->b_addr;
	dp->d_ops->node_hdr_from_disk(&node1hdr, node1);
	dp->d_ops->node_hdr_from_disk(&node2hdr, node2);
	btree1 = dp->d_ops->node_tree_p(node1);
	btree2 = dp->d_ops->node_tree_p(node2);

	if (node1hdr.count > 0 && node2hdr.count > 0 &&
	    ((be32_to_cpu(btree2[0].hashval) < be32_to_cpu(btree1[0].hashval)) ||
	     (be32_to_cpu(btree2[node2hdr.count - 1].hashval) <
	      be32_to_cpu(btree1[node1hdr.count - 1].hashval)))) {
		return 1;
	}
	return 0;
}

/*
 * Link a new block into a doubly linked list of blocks (of whatever type).
 */
int							/* error */
xfs_da3_blk_link(
	struct xfs_da_state	*state,
	struct xfs_da_state_blk	*old_blk,
	struct xfs_da_state_blk	*new_blk)
{
	struct xfs_da_blkinfo	*old_info;
	struct xfs_da_blkinfo	*new_info;
	struct xfs_da_blkinfo	*tmp_info;
	struct xfs_da_args	*args;
	struct xfs_buf		*bp;
	int			before = 0;
	int			error;
	struct xfs_inode	*dp = state->args->dp;
>>>>>>> refs/remotes/origin/master

	/*
	 * Set up environment.
	 */
	args = state->args;
	ASSERT(args != NULL);
<<<<<<< HEAD
	old_info = old_blk->bp->data;
	new_info = new_blk->bp->data;
	ASSERT(old_blk->magic == XFS_DA_NODE_MAGIC ||
	       old_blk->magic == XFS_DIR2_LEAFN_MAGIC ||
	       old_blk->magic == XFS_ATTR_LEAF_MAGIC);
	ASSERT(old_blk->magic == be16_to_cpu(old_info->magic));
	ASSERT(new_blk->magic == be16_to_cpu(new_info->magic));
	ASSERT(old_blk->magic == new_blk->magic);
=======
	old_info = old_blk->bp->b_addr;
	new_info = new_blk->bp->b_addr;
	ASSERT(old_blk->magic == XFS_DA_NODE_MAGIC ||
	       old_blk->magic == XFS_DIR2_LEAFN_MAGIC ||
	       old_blk->magic == XFS_ATTR_LEAF_MAGIC);
>>>>>>> refs/remotes/origin/master

	switch (old_blk->magic) {
	case XFS_ATTR_LEAF_MAGIC:
		before = xfs_attr_leaf_order(old_blk->bp, new_blk->bp);
		break;
	case XFS_DIR2_LEAFN_MAGIC:
<<<<<<< HEAD
		before = xfs_dir2_leafn_order(old_blk->bp, new_blk->bp);
		break;
	case XFS_DA_NODE_MAGIC:
		before = xfs_da_node_order(old_blk->bp, new_blk->bp);
=======
		before = xfs_dir2_leafn_order(dp, old_blk->bp, new_blk->bp);
		break;
	case XFS_DA_NODE_MAGIC:
		before = xfs_da3_node_order(dp, old_blk->bp, new_blk->bp);
>>>>>>> refs/remotes/origin/master
		break;
	}

	/*
	 * Link blocks in appropriate order.
	 */
	if (before) {
		/*
		 * Link new block in before existing block.
		 */
<<<<<<< HEAD
<<<<<<< HEAD
=======
		trace_xfs_da_link_before(args);
>>>>>>> refs/remotes/origin/cm-10.0
		new_info->forw = cpu_to_be32(old_blk->blkno);
		new_info->back = old_info->back;
		if (old_info->back) {
			error = xfs_da_read_buf(args->trans, args->dp,
=======
		trace_xfs_da_link_before(args);
		new_info->forw = cpu_to_be32(old_blk->blkno);
		new_info->back = old_info->back;
		if (old_info->back) {
			error = xfs_da3_node_read(args->trans, dp,
>>>>>>> refs/remotes/origin/master
						be32_to_cpu(old_info->back),
						-1, &bp, args->whichfork);
			if (error)
				return(error);
			ASSERT(bp != NULL);
<<<<<<< HEAD
			tmp_info = bp->data;
			ASSERT(be16_to_cpu(tmp_info->magic) == be16_to_cpu(old_info->magic));
			ASSERT(be32_to_cpu(tmp_info->forw) == old_blk->blkno);
			tmp_info->forw = cpu_to_be32(new_blk->blkno);
			xfs_da_log_buf(args->trans, bp, 0, sizeof(*tmp_info)-1);
			xfs_da_buf_done(bp);
=======
			tmp_info = bp->b_addr;
			ASSERT(tmp_info->magic == old_info->magic);
			ASSERT(be32_to_cpu(tmp_info->forw) == old_blk->blkno);
			tmp_info->forw = cpu_to_be32(new_blk->blkno);
			xfs_trans_log_buf(args->trans, bp, 0, sizeof(*tmp_info)-1);
>>>>>>> refs/remotes/origin/master
		}
		old_info->back = cpu_to_be32(new_blk->blkno);
	} else {
		/*
		 * Link new block in after existing block.
		 */
<<<<<<< HEAD
<<<<<<< HEAD
=======
		trace_xfs_da_link_after(args);
>>>>>>> refs/remotes/origin/cm-10.0
		new_info->forw = old_info->forw;
		new_info->back = cpu_to_be32(old_blk->blkno);
		if (old_info->forw) {
			error = xfs_da_read_buf(args->trans, args->dp,
=======
		trace_xfs_da_link_after(args);
		new_info->forw = old_info->forw;
		new_info->back = cpu_to_be32(old_blk->blkno);
		if (old_info->forw) {
			error = xfs_da3_node_read(args->trans, dp,
>>>>>>> refs/remotes/origin/master
						be32_to_cpu(old_info->forw),
						-1, &bp, args->whichfork);
			if (error)
				return(error);
			ASSERT(bp != NULL);
<<<<<<< HEAD
			tmp_info = bp->data;
			ASSERT(tmp_info->magic == old_info->magic);
			ASSERT(be32_to_cpu(tmp_info->back) == old_blk->blkno);
			tmp_info->back = cpu_to_be32(new_blk->blkno);
			xfs_da_log_buf(args->trans, bp, 0, sizeof(*tmp_info)-1);
			xfs_da_buf_done(bp);
=======
			tmp_info = bp->b_addr;
			ASSERT(tmp_info->magic == old_info->magic);
			ASSERT(be32_to_cpu(tmp_info->back) == old_blk->blkno);
			tmp_info->back = cpu_to_be32(new_blk->blkno);
			xfs_trans_log_buf(args->trans, bp, 0, sizeof(*tmp_info)-1);
>>>>>>> refs/remotes/origin/master
		}
		old_info->forw = cpu_to_be32(new_blk->blkno);
	}

<<<<<<< HEAD
	xfs_da_log_buf(args->trans, old_blk->bp, 0, sizeof(*tmp_info) - 1);
	xfs_da_log_buf(args->trans, new_blk->bp, 0, sizeof(*tmp_info) - 1);
=======
	xfs_trans_log_buf(args->trans, old_blk->bp, 0, sizeof(*tmp_info) - 1);
	xfs_trans_log_buf(args->trans, new_blk->bp, 0, sizeof(*tmp_info) - 1);
>>>>>>> refs/remotes/origin/master
	return(0);
}

/*
<<<<<<< HEAD
 * Compare two intermediate nodes for "order".
 */
STATIC int
xfs_da_node_order(xfs_dabuf_t *node1_bp, xfs_dabuf_t *node2_bp)
{
	xfs_da_intnode_t *node1, *node2;

	node1 = node1_bp->data;
	node2 = node2_bp->data;
<<<<<<< HEAD
	ASSERT((be16_to_cpu(node1->hdr.info.magic) == XFS_DA_NODE_MAGIC) &&
	       (be16_to_cpu(node2->hdr.info.magic) == XFS_DA_NODE_MAGIC));
=======
	ASSERT(node1->hdr.info.magic == cpu_to_be16(XFS_DA_NODE_MAGIC) &&
	       node2->hdr.info.magic == cpu_to_be16(XFS_DA_NODE_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	if ((be16_to_cpu(node1->hdr.count) > 0) && (be16_to_cpu(node2->hdr.count) > 0) &&
	    ((be32_to_cpu(node2->btree[0].hashval) <
	      be32_to_cpu(node1->btree[0].hashval)) ||
	     (be32_to_cpu(node2->btree[be16_to_cpu(node2->hdr.count)-1].hashval) <
	      be32_to_cpu(node1->btree[be16_to_cpu(node1->hdr.count)-1].hashval)))) {
		return(1);
	}
	return(0);
}

/*
 * Pick up the last hashvalue from an intermediate node.
 */
STATIC uint
xfs_da_node_lasthash(xfs_dabuf_t *bp, int *count)
{
	xfs_da_intnode_t *node;

	node = bp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(node->hdr.info.magic) == XFS_DA_NODE_MAGIC);
=======
	ASSERT(node->hdr.info.magic == cpu_to_be16(XFS_DA_NODE_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	if (count)
		*count = be16_to_cpu(node->hdr.count);
	if (!node->hdr.count)
		return(0);
	return be32_to_cpu(node->btree[be16_to_cpu(node->hdr.count)-1].hashval);
}

/*
 * Unlink a block from a doubly linked list of blocks.
 */
STATIC int						/* error */
xfs_da_blk_unlink(xfs_da_state_t *state, xfs_da_state_blk_t *drop_blk,
				 xfs_da_state_blk_t *save_blk)
{
	xfs_da_blkinfo_t *drop_info, *save_info, *tmp_info;
	xfs_da_args_t *args;
	xfs_dabuf_t *bp;
	int error;
=======
 * Unlink a block from a doubly linked list of blocks.
 */
STATIC int						/* error */
xfs_da3_blk_unlink(
	struct xfs_da_state	*state,
	struct xfs_da_state_blk	*drop_blk,
	struct xfs_da_state_blk	*save_blk)
{
	struct xfs_da_blkinfo	*drop_info;
	struct xfs_da_blkinfo	*save_info;
	struct xfs_da_blkinfo	*tmp_info;
	struct xfs_da_args	*args;
	struct xfs_buf		*bp;
	int			error;
>>>>>>> refs/remotes/origin/master

	/*
	 * Set up environment.
	 */
	args = state->args;
	ASSERT(args != NULL);
<<<<<<< HEAD
	save_info = save_blk->bp->data;
	drop_info = drop_blk->bp->data;
	ASSERT(save_blk->magic == XFS_DA_NODE_MAGIC ||
	       save_blk->magic == XFS_DIR2_LEAFN_MAGIC ||
	       save_blk->magic == XFS_ATTR_LEAF_MAGIC);
	ASSERT(save_blk->magic == be16_to_cpu(save_info->magic));
	ASSERT(drop_blk->magic == be16_to_cpu(drop_info->magic));
=======
	save_info = save_blk->bp->b_addr;
	drop_info = drop_blk->bp->b_addr;
	ASSERT(save_blk->magic == XFS_DA_NODE_MAGIC ||
	       save_blk->magic == XFS_DIR2_LEAFN_MAGIC ||
	       save_blk->magic == XFS_ATTR_LEAF_MAGIC);
>>>>>>> refs/remotes/origin/master
	ASSERT(save_blk->magic == drop_blk->magic);
	ASSERT((be32_to_cpu(save_info->forw) == drop_blk->blkno) ||
	       (be32_to_cpu(save_info->back) == drop_blk->blkno));
	ASSERT((be32_to_cpu(drop_info->forw) == save_blk->blkno) ||
	       (be32_to_cpu(drop_info->back) == save_blk->blkno));

	/*
	 * Unlink the leaf block from the doubly linked chain of leaves.
	 */
	if (be32_to_cpu(save_info->back) == drop_blk->blkno) {
<<<<<<< HEAD
<<<<<<< HEAD
=======
		trace_xfs_da_unlink_back(args);
>>>>>>> refs/remotes/origin/cm-10.0
		save_info->back = drop_info->back;
		if (drop_info->back) {
			error = xfs_da_read_buf(args->trans, args->dp,
=======
		trace_xfs_da_unlink_back(args);
		save_info->back = drop_info->back;
		if (drop_info->back) {
			error = xfs_da3_node_read(args->trans, args->dp,
>>>>>>> refs/remotes/origin/master
						be32_to_cpu(drop_info->back),
						-1, &bp, args->whichfork);
			if (error)
				return(error);
			ASSERT(bp != NULL);
<<<<<<< HEAD
			tmp_info = bp->data;
			ASSERT(tmp_info->magic == save_info->magic);
			ASSERT(be32_to_cpu(tmp_info->forw) == drop_blk->blkno);
			tmp_info->forw = cpu_to_be32(save_blk->blkno);
			xfs_da_log_buf(args->trans, bp, 0,
						    sizeof(*tmp_info) - 1);
			xfs_da_buf_done(bp);
		}
	} else {
<<<<<<< HEAD
=======
		trace_xfs_da_unlink_forward(args);
>>>>>>> refs/remotes/origin/cm-10.0
		save_info->forw = drop_info->forw;
		if (drop_info->forw) {
			error = xfs_da_read_buf(args->trans, args->dp,
=======
			tmp_info = bp->b_addr;
			ASSERT(tmp_info->magic == save_info->magic);
			ASSERT(be32_to_cpu(tmp_info->forw) == drop_blk->blkno);
			tmp_info->forw = cpu_to_be32(save_blk->blkno);
			xfs_trans_log_buf(args->trans, bp, 0,
						    sizeof(*tmp_info) - 1);
		}
	} else {
		trace_xfs_da_unlink_forward(args);
		save_info->forw = drop_info->forw;
		if (drop_info->forw) {
			error = xfs_da3_node_read(args->trans, args->dp,
>>>>>>> refs/remotes/origin/master
						be32_to_cpu(drop_info->forw),
						-1, &bp, args->whichfork);
			if (error)
				return(error);
			ASSERT(bp != NULL);
<<<<<<< HEAD
			tmp_info = bp->data;
			ASSERT(tmp_info->magic == save_info->magic);
			ASSERT(be32_to_cpu(tmp_info->back) == drop_blk->blkno);
			tmp_info->back = cpu_to_be32(save_blk->blkno);
			xfs_da_log_buf(args->trans, bp, 0,
						    sizeof(*tmp_info) - 1);
			xfs_da_buf_done(bp);
		}
	}

	xfs_da_log_buf(args->trans, save_blk->bp, 0, sizeof(*save_info) - 1);
=======
			tmp_info = bp->b_addr;
			ASSERT(tmp_info->magic == save_info->magic);
			ASSERT(be32_to_cpu(tmp_info->back) == drop_blk->blkno);
			tmp_info->back = cpu_to_be32(save_blk->blkno);
			xfs_trans_log_buf(args->trans, bp, 0,
						    sizeof(*tmp_info) - 1);
		}
	}

	xfs_trans_log_buf(args->trans, save_blk->bp, 0, sizeof(*save_info) - 1);
>>>>>>> refs/remotes/origin/master
	return(0);
}

/*
 * Move a path "forward" or "!forward" one block at the current level.
 *
 * This routine will adjust a "path" to point to the next block
 * "forward" (higher hashvalues) or "!forward" (lower hashvals) in the
 * Btree, including updating pointers to the intermediate nodes between
 * the new bottom and the root.
 */
int							/* error */
<<<<<<< HEAD
xfs_da_path_shift(xfs_da_state_t *state, xfs_da_state_path_t *path,
				 int forward, int release, int *result)
{
	xfs_da_state_blk_t *blk;
	xfs_da_blkinfo_t *info;
	xfs_da_intnode_t *node;
	xfs_da_args_t *args;
	xfs_dablk_t blkno=0;
	int level, error;
=======
xfs_da3_path_shift(
	struct xfs_da_state	*state,
	struct xfs_da_state_path *path,
	int			forward,
	int			release,
	int			*result)
{
	struct xfs_da_state_blk	*blk;
	struct xfs_da_blkinfo	*info;
	struct xfs_da_intnode	*node;
	struct xfs_da_args	*args;
	struct xfs_da_node_entry *btree;
	struct xfs_da3_icnode_hdr nodehdr;
	xfs_dablk_t		blkno = 0;
	int			level;
	int			error;
	struct xfs_inode	*dp = state->args->dp;

	trace_xfs_da_path_shift(state->args);
>>>>>>> refs/remotes/origin/master

	/*
	 * Roll up the Btree looking for the first block where our
	 * current index is not at the edge of the block.  Note that
	 * we skip the bottom layer because we want the sibling block.
	 */
	args = state->args;
	ASSERT(args != NULL);
	ASSERT(path != NULL);
	ASSERT((path->active > 0) && (path->active < XFS_DA_NODE_MAXDEPTH));
	level = (path->active-1) - 1;	/* skip bottom layer in path */
	for (blk = &path->blk[level]; level >= 0; blk--, level--) {
<<<<<<< HEAD
		ASSERT(blk->bp != NULL);
		node = blk->bp->data;
<<<<<<< HEAD
		ASSERT(be16_to_cpu(node->hdr.info.magic) == XFS_DA_NODE_MAGIC);
=======
		ASSERT(node->hdr.info.magic == cpu_to_be16(XFS_DA_NODE_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
		if (forward && (blk->index < be16_to_cpu(node->hdr.count)-1)) {
			blk->index++;
			blkno = be32_to_cpu(node->btree[blk->index].before);
			break;
		} else if (!forward && (blk->index > 0)) {
			blk->index--;
			blkno = be32_to_cpu(node->btree[blk->index].before);
=======
		node = blk->bp->b_addr;
		dp->d_ops->node_hdr_from_disk(&nodehdr, node);
		btree = dp->d_ops->node_tree_p(node);

		if (forward && (blk->index < nodehdr.count - 1)) {
			blk->index++;
			blkno = be32_to_cpu(btree[blk->index].before);
			break;
		} else if (!forward && (blk->index > 0)) {
			blk->index--;
			blkno = be32_to_cpu(btree[blk->index].before);
>>>>>>> refs/remotes/origin/master
			break;
		}
	}
	if (level < 0) {
		*result = XFS_ERROR(ENOENT);	/* we're out of our tree */
		ASSERT(args->op_flags & XFS_DA_OP_OKNOENT);
		return(0);
	}

	/*
	 * Roll down the edge of the subtree until we reach the
	 * same depth we were at originally.
	 */
	for (blk++, level++; level < path->active; blk++, level++) {
		/*
		 * Release the old block.
		 * (if it's dirty, trans won't actually let go)
		 */
		if (release)
<<<<<<< HEAD
			xfs_da_brelse(args->trans, blk->bp);
=======
			xfs_trans_brelse(args->trans, blk->bp);
>>>>>>> refs/remotes/origin/master

		/*
		 * Read the next child block.
		 */
		blk->blkno = blkno;
<<<<<<< HEAD
		error = xfs_da_read_buf(args->trans, args->dp, blkno, -1,
						     &blk->bp, args->whichfork);
		if (error)
			return(error);
		ASSERT(blk->bp != NULL);
		info = blk->bp->data;
<<<<<<< HEAD
		ASSERT(be16_to_cpu(info->magic) == XFS_DA_NODE_MAGIC ||
		       be16_to_cpu(info->magic) == XFS_DIR2_LEAFN_MAGIC ||
		       be16_to_cpu(info->magic) == XFS_ATTR_LEAF_MAGIC);
=======
		ASSERT(info->magic == cpu_to_be16(XFS_DA_NODE_MAGIC) ||
		       info->magic == cpu_to_be16(XFS_DIR2_LEAFN_MAGIC) ||
		       info->magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
		blk->magic = be16_to_cpu(info->magic);
		if (blk->magic == XFS_DA_NODE_MAGIC) {
			node = (xfs_da_intnode_t *)info;
			blk->hashval = be32_to_cpu(node->btree[be16_to_cpu(node->hdr.count)-1].hashval);
			if (forward)
				blk->index = 0;
			else
				blk->index = be16_to_cpu(node->hdr.count)-1;
			blkno = be32_to_cpu(node->btree[blk->index].before);
		} else {
			ASSERT(level == path->active-1);
			blk->index = 0;
			switch(blk->magic) {
			case XFS_ATTR_LEAF_MAGIC:
				blk->hashval = xfs_attr_leaf_lasthash(blk->bp,
								      NULL);
				break;
			case XFS_DIR2_LEAFN_MAGIC:
				blk->hashval = xfs_dir2_leafn_lasthash(blk->bp,
								       NULL);
				break;
			default:
				ASSERT(blk->magic == XFS_ATTR_LEAF_MAGIC ||
				       blk->magic == XFS_DIR2_LEAFN_MAGIC);
				break;
			}
		}
	}
	*result = 0;
	return(0);
=======
		error = xfs_da3_node_read(args->trans, dp, blkno, -1,
					&blk->bp, args->whichfork);
		if (error)
			return(error);
		info = blk->bp->b_addr;
		ASSERT(info->magic == cpu_to_be16(XFS_DA_NODE_MAGIC) ||
		       info->magic == cpu_to_be16(XFS_DA3_NODE_MAGIC) ||
		       info->magic == cpu_to_be16(XFS_DIR2_LEAFN_MAGIC) ||
		       info->magic == cpu_to_be16(XFS_DIR3_LEAFN_MAGIC) ||
		       info->magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC) ||
		       info->magic == cpu_to_be16(XFS_ATTR3_LEAF_MAGIC));


		/*
		 * Note: we flatten the magic number to a single type so we
		 * don't have to compare against crc/non-crc types elsewhere.
		 */
		switch (be16_to_cpu(info->magic)) {
		case XFS_DA_NODE_MAGIC:
		case XFS_DA3_NODE_MAGIC:
			blk->magic = XFS_DA_NODE_MAGIC;
			node = (xfs_da_intnode_t *)info;
			dp->d_ops->node_hdr_from_disk(&nodehdr, node);
			btree = dp->d_ops->node_tree_p(node);
			blk->hashval = be32_to_cpu(btree[nodehdr.count - 1].hashval);
			if (forward)
				blk->index = 0;
			else
				blk->index = nodehdr.count - 1;
			blkno = be32_to_cpu(btree[blk->index].before);
			break;
		case XFS_ATTR_LEAF_MAGIC:
		case XFS_ATTR3_LEAF_MAGIC:
			blk->magic = XFS_ATTR_LEAF_MAGIC;
			ASSERT(level == path->active-1);
			blk->index = 0;
			blk->hashval = xfs_attr_leaf_lasthash(blk->bp, NULL);
			break;
		case XFS_DIR2_LEAFN_MAGIC:
		case XFS_DIR3_LEAFN_MAGIC:
			blk->magic = XFS_DIR2_LEAFN_MAGIC;
			ASSERT(level == path->active-1);
			blk->index = 0;
			blk->hashval = xfs_dir2_leafn_lasthash(args->dp,
							       blk->bp, NULL);
			break;
		default:
			ASSERT(0);
			break;
		}
	}
	*result = 0;
	return 0;
>>>>>>> refs/remotes/origin/master
}


/*========================================================================
 * Utility routines.
 *========================================================================*/

/*
 * Implement a simple hash on a character string.
 * Rotate the hash value by 7 bits, then XOR each character in.
 * This is implemented with some source-level loop unrolling.
 */
xfs_dahash_t
xfs_da_hashname(const __uint8_t *name, int namelen)
{
	xfs_dahash_t hash;

	/*
	 * Do four characters at a time as long as we can.
	 */
	for (hash = 0; namelen >= 4; namelen -= 4, name += 4)
		hash = (name[0] << 21) ^ (name[1] << 14) ^ (name[2] << 7) ^
		       (name[3] << 0) ^ rol32(hash, 7 * 4);

	/*
	 * Now do the rest of the characters.
	 */
	switch (namelen) {
	case 3:
		return (name[0] << 14) ^ (name[1] << 7) ^ (name[2] << 0) ^
		       rol32(hash, 7 * 3);
	case 2:
		return (name[0] << 7) ^ (name[1] << 0) ^ rol32(hash, 7 * 2);
	case 1:
		return (name[0] << 0) ^ rol32(hash, 7 * 1);
	default: /* case 0: */
		return hash;
	}
}

enum xfs_dacmp
xfs_da_compname(
	struct xfs_da_args *args,
	const unsigned char *name,
	int		len)
{
	return (args->namelen == len && memcmp(args->name, name, len) == 0) ?
					XFS_CMP_EXACT : XFS_CMP_DIFFERENT;
}

static xfs_dahash_t
xfs_default_hashname(
	struct xfs_name	*name)
{
	return xfs_da_hashname(name->name, name->len);
}

const struct xfs_nameops xfs_default_nameops = {
	.hashname	= xfs_default_hashname,
	.compname	= xfs_da_compname
};

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * Add a block to the btree ahead of the file.
 * Return the new block number to the caller.
 */
int
xfs_da_grow_inode(xfs_da_args_t *args, xfs_dablk_t *new_blkno)
{
	xfs_fileoff_t bno, b;
	xfs_bmbt_irec_t map;
	xfs_bmbt_irec_t	*mapp;
	xfs_inode_t *dp;
	int nmap, error, w, count, c, got, i, mapi;
	xfs_trans_t *tp;
	xfs_mount_t *mp;
	xfs_drfsbno_t	nblks;

	dp = args->dp;
	mp = dp->i_mount;
	w = args->whichfork;
	tp = args->trans;
	nblks = dp->i_d.di_nblocks;

	/*
	 * For new directories adjust the file offset and block count.
	 */
	if (w == XFS_DATA_FORK) {
		bno = mp->m_dirleafblk;
		count = mp->m_dirblkfsbs;
	} else {
		bno = 0;
		count = 1;
	}
	/*
	 * Find a spot in the file space to put the new block.
	 */
	if ((error = xfs_bmap_first_unused(tp, dp, count, &bno, w)))
		return error;
	if (w == XFS_DATA_FORK)
		ASSERT(bno >= mp->m_dirleafblk && bno < mp->m_dirfreeblk);
=======
=======
>>>>>>> refs/remotes/origin/master
int
xfs_da_grow_inode_int(
	struct xfs_da_args	*args,
	xfs_fileoff_t		*bno,
	int			count)
{
	struct xfs_trans	*tp = args->trans;
	struct xfs_inode	*dp = args->dp;
	int			w = args->whichfork;
	xfs_drfsbno_t		nblks = dp->i_d.di_nblocks;
	struct xfs_bmbt_irec	map, *mapp;
	int			nmap, error, got, i, mapi;

	/*
	 * Find a spot in the file space to put the new block.
	 */
	error = xfs_bmap_first_unused(tp, dp, count, bno, w);
	if (error)
		return error;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Try mapping it in one filesystem block.
	 */
	nmap = 1;
	ASSERT(args->firstblock != NULL);
<<<<<<< HEAD
<<<<<<< HEAD
	if ((error = xfs_bmapi(tp, dp, bno, count,
			xfs_bmapi_aflag(w)|XFS_BMAPI_WRITE|XFS_BMAPI_METADATA|
			XFS_BMAPI_CONTIG,
			args->firstblock, args->total, &map, &nmap,
			args->flist))) {
		return error;
	}
=======
=======
>>>>>>> refs/remotes/origin/master
	error = xfs_bmapi_write(tp, dp, *bno, count,
			xfs_bmapi_aflag(w)|XFS_BMAPI_METADATA|XFS_BMAPI_CONTIG,
			args->firstblock, args->total, &map, &nmap,
			args->flist);
	if (error)
		return error;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	ASSERT(nmap <= 1);
	if (nmap == 1) {
		mapp = &map;
		mapi = 1;
<<<<<<< HEAD
<<<<<<< HEAD
	}
	/*
	 * If we didn't get it and the block might work if fragmented,
	 * try without the CONTIG flag.  Loop until we get it all.
	 */
	else if (nmap == 0 && count > 1) {
		mapp = kmem_alloc(sizeof(*mapp) * count, KM_SLEEP);
		for (b = bno, mapi = 0; b < bno + count; ) {
			nmap = MIN(XFS_BMAP_MAX_NMAP, count);
			c = (int)(bno + count - b);
			if ((error = xfs_bmapi(tp, dp, b, c,
					xfs_bmapi_aflag(w)|XFS_BMAPI_WRITE|
					XFS_BMAPI_METADATA,
					args->firstblock, args->total,
					&mapp[mapi], &nmap, args->flist))) {
				kmem_free(mapp);
				return error;
			}
=======
=======
>>>>>>> refs/remotes/origin/master
	} else if (nmap == 0 && count > 1) {
		xfs_fileoff_t		b;
		int			c;

		/*
		 * If we didn't get it and the block might work if fragmented,
		 * try without the CONTIG flag.  Loop until we get it all.
		 */
		mapp = kmem_alloc(sizeof(*mapp) * count, KM_SLEEP);
		for (b = *bno, mapi = 0; b < *bno + count; ) {
			nmap = MIN(XFS_BMAP_MAX_NMAP, count);
			c = (int)(*bno + count - b);
			error = xfs_bmapi_write(tp, dp, b, c,
					xfs_bmapi_aflag(w)|XFS_BMAPI_METADATA,
					args->firstblock, args->total,
					&mapp[mapi], &nmap, args->flist);
			if (error)
				goto out_free_map;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			if (nmap < 1)
				break;
			mapi += nmap;
			b = mapp[mapi - 1].br_startoff +
			    mapp[mapi - 1].br_blockcount;
		}
	} else {
		mapi = 0;
		mapp = NULL;
	}
<<<<<<< HEAD
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/cm-10.0
=======

>>>>>>> refs/remotes/origin/master
	/*
	 * Count the blocks we got, make sure it matches the total.
	 */
	for (i = 0, got = 0; i < mapi; i++)
		got += mapp[i].br_blockcount;
<<<<<<< HEAD
<<<<<<< HEAD
	if (got != count || mapp[0].br_startoff != bno ||
	    mapp[mapi - 1].br_startoff + mapp[mapi - 1].br_blockcount !=
	    bno + count) {
		if (mapp != &map)
			kmem_free(mapp);
		return XFS_ERROR(ENOSPC);
	}
	if (mapp != &map)
		kmem_free(mapp);
	/* account for newly allocated blocks in reserved blocks total */
	args->total -= dp->i_d.di_nblocks - nblks;
	*new_blkno = (xfs_dablk_t)bno;
	return 0;
=======
=======
>>>>>>> refs/remotes/origin/master
	if (got != count || mapp[0].br_startoff != *bno ||
	    mapp[mapi - 1].br_startoff + mapp[mapi - 1].br_blockcount !=
	    *bno + count) {
		error = XFS_ERROR(ENOSPC);
		goto out_free_map;
	}

	/* account for newly allocated blocks in reserved blocks total */
	args->total -= dp->i_d.di_nblocks - nblks;

out_free_map:
	if (mapp != &map)
		kmem_free(mapp);
	return error;
}

/*
 * Add a block to the btree ahead of the file.
 * Return the new block number to the caller.
 */
int
xfs_da_grow_inode(
	struct xfs_da_args	*args,
	xfs_dablk_t		*new_blkno)
{
	xfs_fileoff_t		bno;
	int			count;
	int			error;

	trace_xfs_da_grow_inode(args);

	if (args->whichfork == XFS_DATA_FORK) {
		bno = args->dp->i_mount->m_dirleafblk;
		count = args->dp->i_mount->m_dirblkfsbs;
	} else {
		bno = 0;
		count = 1;
	}

	error = xfs_da_grow_inode_int(args, &bno, count);
	if (!error)
		*new_blkno = (xfs_dablk_t)bno;
	return error;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/*
 * Ick.  We need to always be able to remove a btree block, even
 * if there's no space reservation because the filesystem is full.
 * This is called if xfs_bunmapi on a btree block fails due to ENOSPC.
 * It swaps the target block with the last block in the file.  The
 * last block in the file can always be removed since it can't cause
 * a bmap btree split to do that.
 */
STATIC int
<<<<<<< HEAD
xfs_da_swap_lastblock(xfs_da_args_t *args, xfs_dablk_t *dead_blknop,
		      xfs_dabuf_t **dead_bufp)
{
	xfs_dablk_t dead_blkno, last_blkno, sib_blkno, par_blkno;
	xfs_dabuf_t *dead_buf, *last_buf, *sib_buf, *par_buf;
	xfs_fileoff_t lastoff;
	xfs_inode_t *ip;
	xfs_trans_t *tp;
	xfs_mount_t *mp;
	int error, w, entno, level, dead_level;
	xfs_da_blkinfo_t *dead_info, *sib_info;
	xfs_da_intnode_t *par_node, *dead_node;
	xfs_dir2_leaf_t *dead_leaf2;
	xfs_dahash_t dead_hash;

<<<<<<< HEAD
=======
	trace_xfs_da_swap_lastblock(args);

>>>>>>> refs/remotes/origin/cm-10.0
	dead_buf = *dead_bufp;
	dead_blkno = *dead_blknop;
	tp = args->trans;
	ip = args->dp;
	w = args->whichfork;
	ASSERT(w == XFS_DATA_FORK);
	mp = ip->i_mount;
	lastoff = mp->m_dirfreeblk;
	error = xfs_bmap_last_before(tp, ip, &lastoff, w);
=======
xfs_da3_swap_lastblock(
	struct xfs_da_args	*args,
	xfs_dablk_t		*dead_blknop,
	struct xfs_buf		**dead_bufp)
{
	struct xfs_da_blkinfo	*dead_info;
	struct xfs_da_blkinfo	*sib_info;
	struct xfs_da_intnode	*par_node;
	struct xfs_da_intnode	*dead_node;
	struct xfs_dir2_leaf	*dead_leaf2;
	struct xfs_da_node_entry *btree;
	struct xfs_da3_icnode_hdr par_hdr;
	struct xfs_inode	*dp;
	struct xfs_trans	*tp;
	struct xfs_mount	*mp;
	struct xfs_buf		*dead_buf;
	struct xfs_buf		*last_buf;
	struct xfs_buf		*sib_buf;
	struct xfs_buf		*par_buf;
	xfs_dahash_t		dead_hash;
	xfs_fileoff_t		lastoff;
	xfs_dablk_t		dead_blkno;
	xfs_dablk_t		last_blkno;
	xfs_dablk_t		sib_blkno;
	xfs_dablk_t		par_blkno;
	int			error;
	int			w;
	int			entno;
	int			level;
	int			dead_level;

	trace_xfs_da_swap_lastblock(args);

	dead_buf = *dead_bufp;
	dead_blkno = *dead_blknop;
	tp = args->trans;
	dp = args->dp;
	w = args->whichfork;
	ASSERT(w == XFS_DATA_FORK);
	mp = dp->i_mount;
	lastoff = mp->m_dirfreeblk;
	error = xfs_bmap_last_before(tp, dp, &lastoff, w);
>>>>>>> refs/remotes/origin/master
	if (error)
		return error;
	if (unlikely(lastoff == 0)) {
		XFS_ERROR_REPORT("xfs_da_swap_lastblock(1)", XFS_ERRLEVEL_LOW,
				 mp);
		return XFS_ERROR(EFSCORRUPTED);
	}
	/*
	 * Read the last block in the btree space.
	 */
	last_blkno = (xfs_dablk_t)lastoff - mp->m_dirblkfsbs;
<<<<<<< HEAD
	if ((error = xfs_da_read_buf(tp, ip, last_blkno, -1, &last_buf, w)))
=======
	error = xfs_da3_node_read(tp, dp, last_blkno, -1, &last_buf, w);
	if (error)
>>>>>>> refs/remotes/origin/master
		return error;
	/*
	 * Copy the last block into the dead buffer and log it.
	 */
<<<<<<< HEAD
	memcpy(dead_buf->data, last_buf->data, mp->m_dirblksize);
	xfs_da_log_buf(tp, dead_buf, 0, mp->m_dirblksize - 1);
	dead_info = dead_buf->data;
	/*
	 * Get values from the moved block.
	 */
<<<<<<< HEAD
	if (be16_to_cpu(dead_info->magic) == XFS_DIR2_LEAFN_MAGIC) {
=======
	if (dead_info->magic == cpu_to_be16(XFS_DIR2_LEAFN_MAGIC)) {
>>>>>>> refs/remotes/origin/cm-10.0
		dead_leaf2 = (xfs_dir2_leaf_t *)dead_info;
		dead_level = 0;
		dead_hash = be32_to_cpu(dead_leaf2->ents[be16_to_cpu(dead_leaf2->hdr.count) - 1].hashval);
	} else {
<<<<<<< HEAD
		ASSERT(be16_to_cpu(dead_info->magic) == XFS_DA_NODE_MAGIC);
=======
		ASSERT(dead_info->magic == cpu_to_be16(XFS_DA_NODE_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
		dead_node = (xfs_da_intnode_t *)dead_info;
		dead_level = be16_to_cpu(dead_node->hdr.level);
		dead_hash = be32_to_cpu(dead_node->btree[be16_to_cpu(dead_node->hdr.count) - 1].hashval);
=======
	memcpy(dead_buf->b_addr, last_buf->b_addr, mp->m_dirblksize);
	xfs_trans_log_buf(tp, dead_buf, 0, mp->m_dirblksize - 1);
	dead_info = dead_buf->b_addr;
	/*
	 * Get values from the moved block.
	 */
	if (dead_info->magic == cpu_to_be16(XFS_DIR2_LEAFN_MAGIC) ||
	    dead_info->magic == cpu_to_be16(XFS_DIR3_LEAFN_MAGIC)) {
		struct xfs_dir3_icleaf_hdr leafhdr;
		struct xfs_dir2_leaf_entry *ents;

		dead_leaf2 = (xfs_dir2_leaf_t *)dead_info;
		dp->d_ops->leaf_hdr_from_disk(&leafhdr, dead_leaf2);
		ents = dp->d_ops->leaf_ents_p(dead_leaf2);
		dead_level = 0;
		dead_hash = be32_to_cpu(ents[leafhdr.count - 1].hashval);
	} else {
		struct xfs_da3_icnode_hdr deadhdr;

		dead_node = (xfs_da_intnode_t *)dead_info;
		dp->d_ops->node_hdr_from_disk(&deadhdr, dead_node);
		btree = dp->d_ops->node_tree_p(dead_node);
		dead_level = deadhdr.level;
		dead_hash = be32_to_cpu(btree[deadhdr.count - 1].hashval);
>>>>>>> refs/remotes/origin/master
	}
	sib_buf = par_buf = NULL;
	/*
	 * If the moved block has a left sibling, fix up the pointers.
	 */
	if ((sib_blkno = be32_to_cpu(dead_info->back))) {
<<<<<<< HEAD
		if ((error = xfs_da_read_buf(tp, ip, sib_blkno, -1, &sib_buf, w)))
			goto done;
		sib_info = sib_buf->data;
=======
		error = xfs_da3_node_read(tp, dp, sib_blkno, -1, &sib_buf, w);
		if (error)
			goto done;
		sib_info = sib_buf->b_addr;
>>>>>>> refs/remotes/origin/master
		if (unlikely(
		    be32_to_cpu(sib_info->forw) != last_blkno ||
		    sib_info->magic != dead_info->magic)) {
			XFS_ERROR_REPORT("xfs_da_swap_lastblock(2)",
					 XFS_ERRLEVEL_LOW, mp);
			error = XFS_ERROR(EFSCORRUPTED);
			goto done;
		}
		sib_info->forw = cpu_to_be32(dead_blkno);
<<<<<<< HEAD
		xfs_da_log_buf(tp, sib_buf,
			XFS_DA_LOGRANGE(sib_info, &sib_info->forw,
					sizeof(sib_info->forw)));
		xfs_da_buf_done(sib_buf);
=======
		xfs_trans_log_buf(tp, sib_buf,
			XFS_DA_LOGRANGE(sib_info, &sib_info->forw,
					sizeof(sib_info->forw)));
>>>>>>> refs/remotes/origin/master
		sib_buf = NULL;
	}
	/*
	 * If the moved block has a right sibling, fix up the pointers.
	 */
	if ((sib_blkno = be32_to_cpu(dead_info->forw))) {
<<<<<<< HEAD
		if ((error = xfs_da_read_buf(tp, ip, sib_blkno, -1, &sib_buf, w)))
			goto done;
		sib_info = sib_buf->data;
=======
		error = xfs_da3_node_read(tp, dp, sib_blkno, -1, &sib_buf, w);
		if (error)
			goto done;
		sib_info = sib_buf->b_addr;
>>>>>>> refs/remotes/origin/master
		if (unlikely(
		       be32_to_cpu(sib_info->back) != last_blkno ||
		       sib_info->magic != dead_info->magic)) {
			XFS_ERROR_REPORT("xfs_da_swap_lastblock(3)",
					 XFS_ERRLEVEL_LOW, mp);
			error = XFS_ERROR(EFSCORRUPTED);
			goto done;
		}
		sib_info->back = cpu_to_be32(dead_blkno);
<<<<<<< HEAD
		xfs_da_log_buf(tp, sib_buf,
			XFS_DA_LOGRANGE(sib_info, &sib_info->back,
					sizeof(sib_info->back)));
		xfs_da_buf_done(sib_buf);
=======
		xfs_trans_log_buf(tp, sib_buf,
			XFS_DA_LOGRANGE(sib_info, &sib_info->back,
					sizeof(sib_info->back)));
>>>>>>> refs/remotes/origin/master
		sib_buf = NULL;
	}
	par_blkno = mp->m_dirleafblk;
	level = -1;
	/*
	 * Walk down the tree looking for the parent of the moved block.
	 */
	for (;;) {
<<<<<<< HEAD
		if ((error = xfs_da_read_buf(tp, ip, par_blkno, -1, &par_buf, w)))
			goto done;
		par_node = par_buf->data;
<<<<<<< HEAD
		if (unlikely(
		    be16_to_cpu(par_node->hdr.info.magic) != XFS_DA_NODE_MAGIC ||
=======
		if (unlikely(par_node->hdr.info.magic !=
		    cpu_to_be16(XFS_DA_NODE_MAGIC) ||
>>>>>>> refs/remotes/origin/cm-10.0
		    (level >= 0 && level != be16_to_cpu(par_node->hdr.level) + 1))) {
=======
		error = xfs_da3_node_read(tp, dp, par_blkno, -1, &par_buf, w);
		if (error)
			goto done;
		par_node = par_buf->b_addr;
		dp->d_ops->node_hdr_from_disk(&par_hdr, par_node);
		if (level >= 0 && level != par_hdr.level + 1) {
>>>>>>> refs/remotes/origin/master
			XFS_ERROR_REPORT("xfs_da_swap_lastblock(4)",
					 XFS_ERRLEVEL_LOW, mp);
			error = XFS_ERROR(EFSCORRUPTED);
			goto done;
		}
<<<<<<< HEAD
		level = be16_to_cpu(par_node->hdr.level);
		for (entno = 0;
		     entno < be16_to_cpu(par_node->hdr.count) &&
		     be32_to_cpu(par_node->btree[entno].hashval) < dead_hash;
		     entno++)
			continue;
		if (unlikely(entno == be16_to_cpu(par_node->hdr.count))) {
=======
		level = par_hdr.level;
		btree = dp->d_ops->node_tree_p(par_node);
		for (entno = 0;
		     entno < par_hdr.count &&
		     be32_to_cpu(btree[entno].hashval) < dead_hash;
		     entno++)
			continue;
		if (entno == par_hdr.count) {
>>>>>>> refs/remotes/origin/master
			XFS_ERROR_REPORT("xfs_da_swap_lastblock(5)",
					 XFS_ERRLEVEL_LOW, mp);
			error = XFS_ERROR(EFSCORRUPTED);
			goto done;
		}
<<<<<<< HEAD
		par_blkno = be32_to_cpu(par_node->btree[entno].before);
		if (level == dead_level + 1)
			break;
		xfs_da_brelse(tp, par_buf);
=======
		par_blkno = be32_to_cpu(btree[entno].before);
		if (level == dead_level + 1)
			break;
		xfs_trans_brelse(tp, par_buf);
>>>>>>> refs/remotes/origin/master
		par_buf = NULL;
	}
	/*
	 * We're in the right parent block.
	 * Look for the right entry.
	 */
	for (;;) {
		for (;
<<<<<<< HEAD
		     entno < be16_to_cpu(par_node->hdr.count) &&
		     be32_to_cpu(par_node->btree[entno].before) != last_blkno;
		     entno++)
			continue;
		if (entno < be16_to_cpu(par_node->hdr.count))
			break;
		par_blkno = be32_to_cpu(par_node->hdr.info.forw);
		xfs_da_brelse(tp, par_buf);
=======
		     entno < par_hdr.count &&
		     be32_to_cpu(btree[entno].before) != last_blkno;
		     entno++)
			continue;
		if (entno < par_hdr.count)
			break;
		par_blkno = par_hdr.forw;
		xfs_trans_brelse(tp, par_buf);
>>>>>>> refs/remotes/origin/master
		par_buf = NULL;
		if (unlikely(par_blkno == 0)) {
			XFS_ERROR_REPORT("xfs_da_swap_lastblock(6)",
					 XFS_ERRLEVEL_LOW, mp);
			error = XFS_ERROR(EFSCORRUPTED);
			goto done;
		}
<<<<<<< HEAD
		if ((error = xfs_da_read_buf(tp, ip, par_blkno, -1, &par_buf, w)))
			goto done;
		par_node = par_buf->data;
		if (unlikely(
		    be16_to_cpu(par_node->hdr.level) != level ||
<<<<<<< HEAD
		    be16_to_cpu(par_node->hdr.info.magic) != XFS_DA_NODE_MAGIC)) {
=======
		    par_node->hdr.info.magic != cpu_to_be16(XFS_DA_NODE_MAGIC))) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
		error = xfs_da3_node_read(tp, dp, par_blkno, -1, &par_buf, w);
		if (error)
			goto done;
		par_node = par_buf->b_addr;
		dp->d_ops->node_hdr_from_disk(&par_hdr, par_node);
		if (par_hdr.level != level) {
>>>>>>> refs/remotes/origin/master
			XFS_ERROR_REPORT("xfs_da_swap_lastblock(7)",
					 XFS_ERRLEVEL_LOW, mp);
			error = XFS_ERROR(EFSCORRUPTED);
			goto done;
		}
<<<<<<< HEAD
=======
		btree = dp->d_ops->node_tree_p(par_node);
>>>>>>> refs/remotes/origin/master
		entno = 0;
	}
	/*
	 * Update the parent entry pointing to the moved block.
	 */
<<<<<<< HEAD
	par_node->btree[entno].before = cpu_to_be32(dead_blkno);
	xfs_da_log_buf(tp, par_buf,
		XFS_DA_LOGRANGE(par_node, &par_node->btree[entno].before,
				sizeof(par_node->btree[entno].before)));
	xfs_da_buf_done(par_buf);
	xfs_da_buf_done(dead_buf);
=======
	btree[entno].before = cpu_to_be32(dead_blkno);
	xfs_trans_log_buf(tp, par_buf,
		XFS_DA_LOGRANGE(par_node, &btree[entno].before,
				sizeof(btree[entno].before)));
>>>>>>> refs/remotes/origin/master
	*dead_blknop = last_blkno;
	*dead_bufp = last_buf;
	return 0;
done:
	if (par_buf)
<<<<<<< HEAD
		xfs_da_brelse(tp, par_buf);
	if (sib_buf)
		xfs_da_brelse(tp, sib_buf);
	xfs_da_brelse(tp, last_buf);
=======
		xfs_trans_brelse(tp, par_buf);
	if (sib_buf)
		xfs_trans_brelse(tp, sib_buf);
	xfs_trans_brelse(tp, last_buf);
>>>>>>> refs/remotes/origin/master
	return error;
}

/*
 * Remove a btree block from a directory or attribute.
 */
int
<<<<<<< HEAD
xfs_da_shrink_inode(xfs_da_args_t *args, xfs_dablk_t dead_blkno,
		    xfs_dabuf_t *dead_buf)
=======
xfs_da_shrink_inode(
	xfs_da_args_t	*args,
	xfs_dablk_t	dead_blkno,
	struct xfs_buf	*dead_buf)
>>>>>>> refs/remotes/origin/master
{
	xfs_inode_t *dp;
	int done, error, w, count;
	xfs_trans_t *tp;
	xfs_mount_t *mp;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	trace_xfs_da_shrink_inode(args);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	trace_xfs_da_shrink_inode(args);

>>>>>>> refs/remotes/origin/master
	dp = args->dp;
	w = args->whichfork;
	tp = args->trans;
	mp = dp->i_mount;
	if (w == XFS_DATA_FORK)
		count = mp->m_dirblkfsbs;
	else
		count = 1;
	for (;;) {
		/*
		 * Remove extents.  If we get ENOSPC for a dir we have to move
		 * the last block to the place we want to kill.
		 */
<<<<<<< HEAD
		if ((error = xfs_bunmapi(tp, dp, dead_blkno, count,
				xfs_bmapi_aflag(w)|XFS_BMAPI_METADATA,
				0, args->firstblock, args->flist,
				&done)) == ENOSPC) {
			if (w != XFS_DATA_FORK)
				break;
			if ((error = xfs_da_swap_lastblock(args, &dead_blkno,
					&dead_buf)))
=======
		error = xfs_bunmapi(tp, dp, dead_blkno, count,
				    xfs_bmapi_aflag(w)|XFS_BMAPI_METADATA,
				    0, args->firstblock, args->flist, &done);
		if (error == ENOSPC) {
			if (w != XFS_DATA_FORK)
				break;
			error = xfs_da3_swap_lastblock(args, &dead_blkno,
						      &dead_buf);
			if (error)
>>>>>>> refs/remotes/origin/master
				break;
		} else {
			break;
		}
	}
<<<<<<< HEAD
	xfs_da_binval(tp, dead_buf);
=======
	xfs_trans_binval(tp, dead_buf);
>>>>>>> refs/remotes/origin/master
	return error;
}

/*
 * See if the mapping(s) for this btree block are valid, i.e.
 * don't contain holes, are logically contiguous, and cover the whole range.
 */
STATIC int
xfs_da_map_covers_blocks(
	int		nmap,
	xfs_bmbt_irec_t	*mapp,
	xfs_dablk_t	bno,
	int		count)
{
	int		i;
	xfs_fileoff_t	off;

	for (i = 0, off = bno; i < nmap; i++) {
		if (mapp[i].br_startblock == HOLESTARTBLOCK ||
		    mapp[i].br_startblock == DELAYSTARTBLOCK) {
			return 0;
		}
		if (off != mapp[i].br_startoff) {
			return 0;
		}
		off += mapp[i].br_blockcount;
	}
	return off == bno + count;
}

/*
<<<<<<< HEAD
 * Make a dabuf.
 * Used for get_buf, read_buf, read_bufr, and reada_buf.
 */
STATIC int
xfs_da_do_buf(
	xfs_trans_t	*trans,
	xfs_inode_t	*dp,
	xfs_dablk_t	bno,
	xfs_daddr_t	*mappedbnop,
	xfs_dabuf_t	**bpp,
	int		whichfork,
<<<<<<< HEAD
	int		caller,
	inst_t		*ra)
=======
	int		caller)
>>>>>>> refs/remotes/origin/cm-10.0
{
	xfs_buf_t	*bp = NULL;
	xfs_buf_t	**bplist;
	int		error=0;
	int		i;
	xfs_bmbt_irec_t	map;
	xfs_bmbt_irec_t	*mapp;
	xfs_daddr_t	mappedbno;
	xfs_mount_t	*mp;
	int		nbplist=0;
	int		nfsb;
	int		nmap;
	xfs_dabuf_t	*rbp;

	mp = dp->i_mount;
	nfsb = (whichfork == XFS_DATA_FORK) ? mp->m_dirblkfsbs : 1;
	mappedbno = *mappedbnop;
=======
 * Convert a struct xfs_bmbt_irec to a struct xfs_buf_map.
 *
 * For the single map case, it is assumed that the caller has provided a pointer
 * to a valid xfs_buf_map.  For the multiple map case, this function will
 * allocate the xfs_buf_map to hold all the maps and replace the caller's single
 * map pointer with the allocated map.
 */
static int
xfs_buf_map_from_irec(
	struct xfs_mount	*mp,
	struct xfs_buf_map	**mapp,
	int			*nmaps,
	struct xfs_bmbt_irec	*irecs,
	int			nirecs)
{
	struct xfs_buf_map	*map;
	int			i;

	ASSERT(*nmaps == 1);
	ASSERT(nirecs >= 1);

	if (nirecs > 1) {
		map = kmem_zalloc(nirecs * sizeof(struct xfs_buf_map),
				  KM_SLEEP | KM_NOFS);
		if (!map)
			return ENOMEM;
		*mapp = map;
	}

	*nmaps = nirecs;
	map = *mapp;
	for (i = 0; i < *nmaps; i++) {
		ASSERT(irecs[i].br_startblock != DELAYSTARTBLOCK &&
		       irecs[i].br_startblock != HOLESTARTBLOCK);
		map[i].bm_bn = XFS_FSB_TO_DADDR(mp, irecs[i].br_startblock);
		map[i].bm_len = XFS_FSB_TO_BB(mp, irecs[i].br_blockcount);
	}
	return 0;
}

/*
 * Map the block we are given ready for reading. There are three possible return
 * values:
 *	-1 - will be returned if we land in a hole and mappedbno == -2 so the
 *	     caller knows not to execute a subsequent read.
 *	 0 - if we mapped the block successfully
 *	>0 - positive error number if there was an error.
 */
static int
xfs_dabuf_map(
	struct xfs_trans	*trans,
	struct xfs_inode	*dp,
	xfs_dablk_t		bno,
	xfs_daddr_t		mappedbno,
	int			whichfork,
	struct xfs_buf_map	**map,
	int			*nmaps)
{
	struct xfs_mount	*mp = dp->i_mount;
	int			nfsb;
	int			error = 0;
	struct xfs_bmbt_irec	irec;
	struct xfs_bmbt_irec	*irecs = &irec;
	int			nirecs;

	ASSERT(map && *map);
	ASSERT(*nmaps == 1);

	nfsb = (whichfork == XFS_DATA_FORK) ? mp->m_dirblkfsbs : 1;

>>>>>>> refs/remotes/origin/master
	/*
	 * Caller doesn't have a mapping.  -2 means don't complain
	 * if we land in a hole.
	 */
	if (mappedbno == -1 || mappedbno == -2) {
		/*
		 * Optimize the one-block case.
		 */
<<<<<<< HEAD
<<<<<<< HEAD
		if (nfsb == 1) {
			xfs_fsblock_t	fsb;

			if ((error =
			    xfs_bmapi_single(trans, dp, whichfork, &fsb,
				    (xfs_fileoff_t)bno))) {
				return error;
			}
			mapp = &map;
			if (fsb == NULLFSBLOCK) {
				nmap = 0;
			} else {
				map.br_startblock = fsb;
				map.br_startoff = (xfs_fileoff_t)bno;
				map.br_blockcount = 1;
				nmap = 1;
			}
		} else {
			mapp = kmem_alloc(sizeof(*mapp) * nfsb, KM_SLEEP);
			nmap = nfsb;
			if ((error = xfs_bmapi(trans, dp, (xfs_fileoff_t)bno,
					nfsb,
					XFS_BMAPI_METADATA |
						xfs_bmapi_aflag(whichfork),
					NULL, 0, mapp, &nmap, NULL)))
				goto exit0;
		}
=======
		if (nfsb == 1)
			mapp = &map;
		else
			mapp = kmem_alloc(sizeof(*mapp) * nfsb, KM_SLEEP);

		nmap = nfsb;
		error = xfs_bmapi_read(dp, (xfs_fileoff_t)bno, nfsb, mapp,
				       &nmap, xfs_bmapi_aflag(whichfork));
		if (error)
			goto exit0;
>>>>>>> refs/remotes/origin/cm-10.0
	} else {
		map.br_startblock = XFS_DADDR_TO_FSB(mp, mappedbno);
		map.br_startoff = (xfs_fileoff_t)bno;
		map.br_blockcount = nfsb;
		mapp = &map;
		nmap = 1;
	}
	if (!xfs_da_map_covers_blocks(nmap, mapp, bno, nfsb)) {
		error = mappedbno == -2 ? 0 : XFS_ERROR(EFSCORRUPTED);
		if (unlikely(error == EFSCORRUPTED)) {
			if (xfs_error_level >= XFS_ERRLEVEL_LOW) {
				xfs_alert(mp, "%s: bno %lld dir: inode %lld",
					__func__, (long long)bno,
					(long long)dp->i_ino);
				for (i = 0; i < nmap; i++) {
					xfs_alert(mp,
"[%02d] br_startoff %lld br_startblock %lld br_blockcount %lld br_state %d",
						i,
						(long long)mapp[i].br_startoff,
						(long long)mapp[i].br_startblock,
						(long long)mapp[i].br_blockcount,
						mapp[i].br_state);
=======
		if (nfsb != 1)
			irecs = kmem_zalloc(sizeof(irec) * nfsb,
					    KM_SLEEP | KM_NOFS);

		nirecs = nfsb;
		error = xfs_bmapi_read(dp, (xfs_fileoff_t)bno, nfsb, irecs,
				       &nirecs, xfs_bmapi_aflag(whichfork));
		if (error)
			goto out;
	} else {
		irecs->br_startblock = XFS_DADDR_TO_FSB(mp, mappedbno);
		irecs->br_startoff = (xfs_fileoff_t)bno;
		irecs->br_blockcount = nfsb;
		irecs->br_state = 0;
		nirecs = 1;
	}

	if (!xfs_da_map_covers_blocks(nirecs, irecs, bno, nfsb)) {
		error = mappedbno == -2 ? -1 : XFS_ERROR(EFSCORRUPTED);
		if (unlikely(error == EFSCORRUPTED)) {
			if (xfs_error_level >= XFS_ERRLEVEL_LOW) {
				int i;
				xfs_alert(mp, "%s: bno %lld dir: inode %lld",
					__func__, (long long)bno,
					(long long)dp->i_ino);
				for (i = 0; i < *nmaps; i++) {
					xfs_alert(mp,
"[%02d] br_startoff %lld br_startblock %lld br_blockcount %lld br_state %d",
						i,
						(long long)irecs[i].br_startoff,
						(long long)irecs[i].br_startblock,
						(long long)irecs[i].br_blockcount,
						irecs[i].br_state);
>>>>>>> refs/remotes/origin/master
				}
			}
			XFS_ERROR_REPORT("xfs_da_do_buf(1)",
					 XFS_ERRLEVEL_LOW, mp);
		}
<<<<<<< HEAD
		goto exit0;
	}
	if (caller != 3 && nmap > 1) {
		bplist = kmem_alloc(sizeof(*bplist) * nmap, KM_SLEEP);
		nbplist = 0;
	} else
		bplist = NULL;
	/*
	 * Turn the mapping(s) into buffer(s).
	 */
	for (i = 0; i < nmap; i++) {
		int	nmapped;

		mappedbno = XFS_FSB_TO_DADDR(mp, mapp[i].br_startblock);
		if (i == 0)
			*mappedbnop = mappedbno;
		nmapped = (int)XFS_FSB_TO_BB(mp, mapp[i].br_blockcount);
		switch (caller) {
		case 0:
			bp = xfs_trans_get_buf(trans, mp->m_ddev_targp,
				mappedbno, nmapped, 0);
<<<<<<< HEAD
			error = bp ? XFS_BUF_GETERROR(bp) : XFS_ERROR(EIO);
=======
			error = bp ? bp->b_error : XFS_ERROR(EIO);
>>>>>>> refs/remotes/origin/cm-10.0
			break;
		case 1:
		case 2:
			bp = NULL;
			error = xfs_trans_read_buf(mp, trans, mp->m_ddev_targp,
				mappedbno, nmapped, 0, &bp);
			break;
		case 3:
			xfs_buf_readahead(mp->m_ddev_targp, mappedbno, nmapped);
			error = 0;
			bp = NULL;
			break;
		}
		if (error) {
			if (bp)
				xfs_trans_brelse(trans, bp);
			goto exit1;
		}
		if (!bp)
			continue;
		if (caller == 1) {
<<<<<<< HEAD
			if (whichfork == XFS_ATTR_FORK) {
				XFS_BUF_SET_VTYPE_REF(bp, B_FS_ATTR_BTREE,
						XFS_ATTR_BTREE_REF);
			} else {
				XFS_BUF_SET_VTYPE_REF(bp, B_FS_DIR_BTREE,
						XFS_DIR_BTREE_REF);
			}
=======
			if (whichfork == XFS_ATTR_FORK)
				xfs_buf_set_ref(bp, XFS_ATTR_BTREE_REF);
			else
				xfs_buf_set_ref(bp, XFS_DIR_BTREE_REF);
>>>>>>> refs/remotes/origin/cm-10.0
		}
		if (bplist) {
			bplist[nbplist++] = bp;
		}
	}
	/*
	 * Build a dabuf structure.
	 */
	if (bplist) {
<<<<<<< HEAD
		rbp = xfs_da_buf_make(nbplist, bplist, ra);
	} else if (bp)
		rbp = xfs_da_buf_make(1, &bp, ra);
=======
		rbp = xfs_da_buf_make(nbplist, bplist);
	} else if (bp)
		rbp = xfs_da_buf_make(1, &bp);
>>>>>>> refs/remotes/origin/cm-10.0
	else
		rbp = NULL;
	/*
	 * For read_buf, check the magic number.
	 */
	if (caller == 1) {
<<<<<<< HEAD
		xfs_dir2_data_t		*data;
		xfs_dir2_free_t		*free;
		xfs_da_blkinfo_t	*info;
		uint			magic, magic1;

		info = rbp->data;
		data = rbp->data;
		free = rbp->data;
		magic = be16_to_cpu(info->magic);
		magic1 = be32_to_cpu(data->hdr.magic);
=======
		xfs_dir2_data_hdr_t	*hdr = rbp->data;
		xfs_dir2_free_t		*free = rbp->data;
		xfs_da_blkinfo_t	*info = rbp->data;
		uint			magic, magic1;

		magic = be16_to_cpu(info->magic);
		magic1 = be32_to_cpu(hdr->magic);
>>>>>>> refs/remotes/origin/cm-10.0
		if (unlikely(
		    XFS_TEST_ERROR((magic != XFS_DA_NODE_MAGIC) &&
				   (magic != XFS_ATTR_LEAF_MAGIC) &&
				   (magic != XFS_DIR2_LEAF1_MAGIC) &&
				   (magic != XFS_DIR2_LEAFN_MAGIC) &&
				   (magic1 != XFS_DIR2_BLOCK_MAGIC) &&
				   (magic1 != XFS_DIR2_DATA_MAGIC) &&
<<<<<<< HEAD
				   (be32_to_cpu(free->hdr.magic) != XFS_DIR2_FREE_MAGIC),
=======
				   (free->hdr.magic != cpu_to_be32(XFS_DIR2_FREE_MAGIC)),
>>>>>>> refs/remotes/origin/cm-10.0
				mp, XFS_ERRTAG_DA_READ_BUF,
				XFS_RANDOM_DA_READ_BUF))) {
			trace_xfs_da_btree_corrupt(rbp->bps[0], _RET_IP_);
			XFS_CORRUPTION_ERROR("xfs_da_do_buf(2)",
					     XFS_ERRLEVEL_LOW, mp, info);
			error = XFS_ERROR(EFSCORRUPTED);
			xfs_da_brelse(trans, rbp);
			nbplist = 0;
			goto exit1;
		}
	}
	if (bplist) {
		kmem_free(bplist);
	}
	if (mapp != &map) {
		kmem_free(mapp);
	}
	if (bpp)
		*bpp = rbp;
	return 0;
exit1:
	if (bplist) {
		for (i = 0; i < nbplist; i++)
			xfs_trans_brelse(trans, bplist[i]);
		kmem_free(bplist);
	}
exit0:
	if (mapp != &map)
		kmem_free(mapp);
	if (bpp)
		*bpp = NULL;
=======
		goto out;
	}
	error = xfs_buf_map_from_irec(mp, map, nmaps, irecs, nirecs);
out:
	if (irecs != &irec)
		kmem_free(irecs);
>>>>>>> refs/remotes/origin/master
	return error;
}

/*
 * Get a buffer for the dir/attr block.
 */
int
xfs_da_get_buf(
<<<<<<< HEAD
	xfs_trans_t	*trans,
	xfs_inode_t	*dp,
	xfs_dablk_t	bno,
	xfs_daddr_t		mappedbno,
	xfs_dabuf_t	**bpp,
	int		whichfork)
{
<<<<<<< HEAD
	return xfs_da_do_buf(trans, dp, bno, &mappedbno, bpp, whichfork, 0,
						 (inst_t *)__return_address);
=======
	return xfs_da_do_buf(trans, dp, bno, &mappedbno, bpp, whichfork, 0);
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * Get a buffer for the dir/attr block, fill in the contents.
 */
int
xfs_da_read_buf(
	xfs_trans_t	*trans,
	xfs_inode_t	*dp,
	xfs_dablk_t	bno,
	xfs_daddr_t		mappedbno,
	xfs_dabuf_t	**bpp,
	int		whichfork)
{
<<<<<<< HEAD
	return xfs_da_do_buf(trans, dp, bno, &mappedbno, bpp, whichfork, 1,
		(inst_t *)__return_address);
=======
	return xfs_da_do_buf(trans, dp, bno, &mappedbno, bpp, whichfork, 1);
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * Readahead the dir/attr block.
 */
xfs_daddr_t
xfs_da_reada_buf(
	xfs_trans_t	*trans,
	xfs_inode_t	*dp,
	xfs_dablk_t	bno,
	int		whichfork)
{
	xfs_daddr_t		rval;

	rval = -1;
<<<<<<< HEAD
	if (xfs_da_do_buf(trans, dp, bno, &rval, NULL, whichfork, 3,
			(inst_t *)__return_address))
=======
	if (xfs_da_do_buf(trans, dp, bno, &rval, NULL, whichfork, 3))
>>>>>>> refs/remotes/origin/cm-10.0
		return -1;
	else
		return rval;
}

kmem_zone_t *xfs_da_state_zone;	/* anchor for state struct zone */
kmem_zone_t *xfs_dabuf_zone;		/* dabuf zone */

/*
 * Allocate a dir-state structure.
 * We don't put them on the stack since they're large.
 */
xfs_da_state_t *
xfs_da_state_alloc(void)
{
	return kmem_zone_zalloc(xfs_da_state_zone, KM_NOFS);
}

/*
 * Kill the altpath contents of a da-state structure.
 */
STATIC void
xfs_da_state_kill_altpath(xfs_da_state_t *state)
{
	int	i;

	for (i = 0; i < state->altpath.active; i++) {
		if (state->altpath.blk[i].bp) {
			if (state->altpath.blk[i].bp != state->path.blk[i].bp)
				xfs_da_buf_done(state->altpath.blk[i].bp);
			state->altpath.blk[i].bp = NULL;
		}
	}
	state->altpath.active = 0;
}

/*
 * Free a da-state structure.
 */
void
xfs_da_state_free(xfs_da_state_t *state)
{
	int	i;

	xfs_da_state_kill_altpath(state);
	for (i = 0; i < state->path.active; i++) {
		if (state->path.blk[i].bp)
			xfs_da_buf_done(state->path.blk[i].bp);
	}
	if (state->extravalid && state->extrablk.bp)
		xfs_da_buf_done(state->extrablk.bp);
#ifdef DEBUG
	memset((char *)state, 0, sizeof(*state));
#endif /* DEBUG */
	kmem_zone_free(xfs_da_state_zone, state);
}

<<<<<<< HEAD
#ifdef XFS_DABUF_DEBUG
xfs_dabuf_t	*xfs_dabuf_global_list;
static DEFINE_SPINLOCK(xfs_dabuf_global_lock);
#endif

=======
>>>>>>> refs/remotes/origin/cm-10.0
/*
 * Create a dabuf.
 */
/* ARGSUSED */
STATIC xfs_dabuf_t *
<<<<<<< HEAD
xfs_da_buf_make(int nbuf, xfs_buf_t **bps, inst_t *ra)
=======
xfs_da_buf_make(int nbuf, xfs_buf_t **bps)
>>>>>>> refs/remotes/origin/cm-10.0
{
	xfs_buf_t	*bp;
	xfs_dabuf_t	*dabuf;
	int		i;
	int		off;

	if (nbuf == 1)
		dabuf = kmem_zone_alloc(xfs_dabuf_zone, KM_NOFS);
	else
		dabuf = kmem_alloc(XFS_DA_BUF_SIZE(nbuf), KM_NOFS);
	dabuf->dirty = 0;
<<<<<<< HEAD
#ifdef XFS_DABUF_DEBUG
	dabuf->ra = ra;
	dabuf->target = XFS_BUF_TARGET(bps[0]);
	dabuf->blkno = XFS_BUF_ADDR(bps[0]);
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
	if (nbuf == 1) {
		dabuf->nbuf = 1;
		bp = bps[0];
		dabuf->bbcount = (short)BTOBB(XFS_BUF_COUNT(bp));
<<<<<<< HEAD
		dabuf->data = XFS_BUF_PTR(bp);
=======
		dabuf->data = bp->b_addr;
>>>>>>> refs/remotes/origin/cm-10.0
		dabuf->bps[0] = bp;
	} else {
		dabuf->nbuf = nbuf;
		for (i = 0, dabuf->bbcount = 0; i < nbuf; i++) {
			dabuf->bps[i] = bp = bps[i];
			dabuf->bbcount += BTOBB(XFS_BUF_COUNT(bp));
		}
		dabuf->data = kmem_alloc(BBTOB(dabuf->bbcount), KM_SLEEP);
		for (i = off = 0; i < nbuf; i++, off += XFS_BUF_COUNT(bp)) {
			bp = bps[i];
<<<<<<< HEAD
			memcpy((char *)dabuf->data + off, XFS_BUF_PTR(bp),
				XFS_BUF_COUNT(bp));
		}
	}
#ifdef XFS_DABUF_DEBUG
	{
		xfs_dabuf_t	*p;

		spin_lock(&xfs_dabuf_global_lock);
		for (p = xfs_dabuf_global_list; p; p = p->next) {
			ASSERT(p->blkno != dabuf->blkno ||
			       p->target != dabuf->target);
		}
		dabuf->prev = NULL;
		if (xfs_dabuf_global_list)
			xfs_dabuf_global_list->prev = dabuf;
		dabuf->next = xfs_dabuf_global_list;
		xfs_dabuf_global_list = dabuf;
		spin_unlock(&xfs_dabuf_global_lock);
	}
#endif
=======
			memcpy((char *)dabuf->data + off, bp->b_addr,
				XFS_BUF_COUNT(bp));
		}
	}
>>>>>>> refs/remotes/origin/cm-10.0
	return dabuf;
}

/*
 * Un-dirty a dabuf.
 */
STATIC void
xfs_da_buf_clean(xfs_dabuf_t *dabuf)
{
	xfs_buf_t	*bp;
	int		i;
	int		off;

	if (dabuf->dirty) {
		ASSERT(dabuf->nbuf > 1);
		dabuf->dirty = 0;
		for (i = off = 0; i < dabuf->nbuf;
				i++, off += XFS_BUF_COUNT(bp)) {
			bp = dabuf->bps[i];
<<<<<<< HEAD
			memcpy(XFS_BUF_PTR(bp), (char *)dabuf->data + off,
				XFS_BUF_COUNT(bp));
=======
			memcpy(bp->b_addr, dabuf->data + off,
						XFS_BUF_COUNT(bp));
>>>>>>> refs/remotes/origin/cm-10.0
		}
	}
}

/*
 * Release a dabuf.
 */
void
xfs_da_buf_done(xfs_dabuf_t *dabuf)
{
	ASSERT(dabuf);
	ASSERT(dabuf->nbuf && dabuf->data && dabuf->bbcount && dabuf->bps[0]);
	if (dabuf->dirty)
		xfs_da_buf_clean(dabuf);
<<<<<<< HEAD
	if (dabuf->nbuf > 1)
		kmem_free(dabuf->data);
#ifdef XFS_DABUF_DEBUG
	{
		spin_lock(&xfs_dabuf_global_lock);
		if (dabuf->prev)
			dabuf->prev->next = dabuf->next;
		else
			xfs_dabuf_global_list = dabuf->next;
		if (dabuf->next)
			dabuf->next->prev = dabuf->prev;
		spin_unlock(&xfs_dabuf_global_lock);
	}
	memset(dabuf, 0, XFS_DA_BUF_SIZE(dabuf->nbuf));
#endif
	if (dabuf->nbuf == 1)
		kmem_zone_free(xfs_dabuf_zone, dabuf);
	else
		kmem_free(dabuf);
=======
	if (dabuf->nbuf > 1) {
		kmem_free(dabuf->data);
		kmem_free(dabuf);
	} else {
		kmem_zone_free(xfs_dabuf_zone, dabuf);
	}
>>>>>>> refs/remotes/origin/cm-10.0
}

/*
 * Log transaction from a dabuf.
 */
void
xfs_da_log_buf(xfs_trans_t *tp, xfs_dabuf_t *dabuf, uint first, uint last)
{
	xfs_buf_t	*bp;
	uint		f;
	int		i;
	uint		l;
	int		off;

	ASSERT(dabuf->nbuf && dabuf->data && dabuf->bbcount && dabuf->bps[0]);
	if (dabuf->nbuf == 1) {
<<<<<<< HEAD
		ASSERT(dabuf->data == (void *)XFS_BUF_PTR(dabuf->bps[0]));
=======
		ASSERT(dabuf->data == dabuf->bps[0]->b_addr);
>>>>>>> refs/remotes/origin/cm-10.0
		xfs_trans_log_buf(tp, dabuf->bps[0], first, last);
		return;
	}
	dabuf->dirty = 1;
	ASSERT(first <= last);
	for (i = off = 0; i < dabuf->nbuf; i++, off += XFS_BUF_COUNT(bp)) {
		bp = dabuf->bps[i];
		f = off;
		l = f + XFS_BUF_COUNT(bp) - 1;
		if (f < first)
			f = first;
		if (l > last)
			l = last;
		if (f <= l)
			xfs_trans_log_buf(tp, bp, f - off, l - off);
		/*
		 * B_DONE is set by xfs_trans_log buf.
		 * If we don't set it on a new buffer (get not read)
		 * then if we don't put anything in the buffer it won't
		 * be set, and at commit it it released into the cache,
		 * and then a read will fail.
		 */
		else if (!(XFS_BUF_ISDONE(bp)))
		  XFS_BUF_DONE(bp);
	}
	ASSERT(last < off);
}

/*
 * Release dabuf from a transaction.
 * Have to free up the dabuf before the buffers are released,
 * since the synchronization on the dabuf is really the lock on the buffer.
 */
void
xfs_da_brelse(xfs_trans_t *tp, xfs_dabuf_t *dabuf)
{
	xfs_buf_t	*bp;
	xfs_buf_t	**bplist;
	int		i;
	int		nbuf;

	ASSERT(dabuf->nbuf && dabuf->data && dabuf->bbcount && dabuf->bps[0]);
	if ((nbuf = dabuf->nbuf) == 1) {
		bplist = &bp;
		bp = dabuf->bps[0];
	} else {
		bplist = kmem_alloc(nbuf * sizeof(*bplist), KM_SLEEP);
		memcpy(bplist, dabuf->bps, nbuf * sizeof(*bplist));
	}
	xfs_da_buf_done(dabuf);
	for (i = 0; i < nbuf; i++)
		xfs_trans_brelse(tp, bplist[i]);
	if (bplist != &bp)
		kmem_free(bplist);
}

/*
 * Invalidate dabuf from a transaction.
 */
void
xfs_da_binval(xfs_trans_t *tp, xfs_dabuf_t *dabuf)
{
	xfs_buf_t	*bp;
	xfs_buf_t	**bplist;
	int		i;
	int		nbuf;

	ASSERT(dabuf->nbuf && dabuf->data && dabuf->bbcount && dabuf->bps[0]);
	if ((nbuf = dabuf->nbuf) == 1) {
		bplist = &bp;
		bp = dabuf->bps[0];
	} else {
		bplist = kmem_alloc(nbuf * sizeof(*bplist), KM_SLEEP);
		memcpy(bplist, dabuf->bps, nbuf * sizeof(*bplist));
	}
	xfs_da_buf_done(dabuf);
	for (i = 0; i < nbuf; i++)
		xfs_trans_binval(tp, bplist[i]);
	if (bplist != &bp)
		kmem_free(bplist);
}

/*
 * Get the first daddr from a dabuf.
 */
xfs_daddr_t
xfs_da_blkno(xfs_dabuf_t *dabuf)
{
	ASSERT(dabuf->nbuf);
	ASSERT(dabuf->data);
	return XFS_BUF_ADDR(dabuf->bps[0]);
=======
	struct xfs_trans	*trans,
	struct xfs_inode	*dp,
	xfs_dablk_t		bno,
	xfs_daddr_t		mappedbno,
	struct xfs_buf		**bpp,
	int			whichfork)
{
	struct xfs_buf		*bp;
	struct xfs_buf_map	map;
	struct xfs_buf_map	*mapp;
	int			nmap;
	int			error;

	*bpp = NULL;
	mapp = &map;
	nmap = 1;
	error = xfs_dabuf_map(trans, dp, bno, mappedbno, whichfork,
				&mapp, &nmap);
	if (error) {
		/* mapping a hole is not an error, but we don't continue */
		if (error == -1)
			error = 0;
		goto out_free;
	}

	bp = xfs_trans_get_buf_map(trans, dp->i_mount->m_ddev_targp,
				    mapp, nmap, 0);
	error = bp ? bp->b_error : XFS_ERROR(EIO);
	if (error) {
		xfs_trans_brelse(trans, bp);
		goto out_free;
	}

	*bpp = bp;

out_free:
	if (mapp != &map)
		kmem_free(mapp);

	return error;
}

/*
 * Get a buffer for the dir/attr block, fill in the contents.
 */
int
xfs_da_read_buf(
	struct xfs_trans	*trans,
	struct xfs_inode	*dp,
	xfs_dablk_t		bno,
	xfs_daddr_t		mappedbno,
	struct xfs_buf		**bpp,
	int			whichfork,
	const struct xfs_buf_ops *ops)
{
	struct xfs_buf		*bp;
	struct xfs_buf_map	map;
	struct xfs_buf_map	*mapp;
	int			nmap;
	int			error;

	*bpp = NULL;
	mapp = &map;
	nmap = 1;
	error = xfs_dabuf_map(trans, dp, bno, mappedbno, whichfork,
				&mapp, &nmap);
	if (error) {
		/* mapping a hole is not an error, but we don't continue */
		if (error == -1)
			error = 0;
		goto out_free;
	}

	error = xfs_trans_read_buf_map(dp->i_mount, trans,
					dp->i_mount->m_ddev_targp,
					mapp, nmap, 0, &bp, ops);
	if (error)
		goto out_free;

	if (whichfork == XFS_ATTR_FORK)
		xfs_buf_set_ref(bp, XFS_ATTR_BTREE_REF);
	else
		xfs_buf_set_ref(bp, XFS_DIR_BTREE_REF);

	/*
	 * This verification code will be moved to a CRC verification callback
	 * function so just leave it here unchanged until then.
	 */
	{
		xfs_dir2_data_hdr_t	*hdr = bp->b_addr;
		xfs_dir2_free_t		*free = bp->b_addr;
		xfs_da_blkinfo_t	*info = bp->b_addr;
		uint			magic, magic1;
		struct xfs_mount	*mp = dp->i_mount;

		magic = be16_to_cpu(info->magic);
		magic1 = be32_to_cpu(hdr->magic);
		if (unlikely(
		    XFS_TEST_ERROR((magic != XFS_DA_NODE_MAGIC) &&
				   (magic != XFS_DA3_NODE_MAGIC) &&
				   (magic != XFS_ATTR_LEAF_MAGIC) &&
				   (magic != XFS_ATTR3_LEAF_MAGIC) &&
				   (magic != XFS_DIR2_LEAF1_MAGIC) &&
				   (magic != XFS_DIR3_LEAF1_MAGIC) &&
				   (magic != XFS_DIR2_LEAFN_MAGIC) &&
				   (magic != XFS_DIR3_LEAFN_MAGIC) &&
				   (magic1 != XFS_DIR2_BLOCK_MAGIC) &&
				   (magic1 != XFS_DIR3_BLOCK_MAGIC) &&
				   (magic1 != XFS_DIR2_DATA_MAGIC) &&
				   (magic1 != XFS_DIR3_DATA_MAGIC) &&
				   (free->hdr.magic !=
					cpu_to_be32(XFS_DIR2_FREE_MAGIC)) &&
				   (free->hdr.magic !=
					cpu_to_be32(XFS_DIR3_FREE_MAGIC)),
				mp, XFS_ERRTAG_DA_READ_BUF,
				XFS_RANDOM_DA_READ_BUF))) {
			trace_xfs_da_btree_corrupt(bp, _RET_IP_);
			XFS_CORRUPTION_ERROR("xfs_da_do_buf(2)",
					     XFS_ERRLEVEL_LOW, mp, info);
			error = XFS_ERROR(EFSCORRUPTED);
			xfs_trans_brelse(trans, bp);
			goto out_free;
		}
	}
	*bpp = bp;
out_free:
	if (mapp != &map)
		kmem_free(mapp);

	return error;
}

/*
 * Readahead the dir/attr block.
 */
xfs_daddr_t
xfs_da_reada_buf(
	struct xfs_trans	*trans,
	struct xfs_inode	*dp,
	xfs_dablk_t		bno,
	xfs_daddr_t		mappedbno,
	int			whichfork,
	const struct xfs_buf_ops *ops)
{
	struct xfs_buf_map	map;
	struct xfs_buf_map	*mapp;
	int			nmap;
	int			error;

	mapp = &map;
	nmap = 1;
	error = xfs_dabuf_map(trans, dp, bno, mappedbno, whichfork,
				&mapp, &nmap);
	if (error) {
		/* mapping a hole is not an error, but we don't continue */
		if (error == -1)
			error = 0;
		goto out_free;
	}

	mappedbno = mapp[0].bm_bn;
	xfs_buf_readahead_map(dp->i_mount->m_ddev_targp, mapp, nmap, ops);

out_free:
	if (mapp != &map)
		kmem_free(mapp);

	if (error)
		return -1;
	return mappedbno;
>>>>>>> refs/remotes/origin/master
}
