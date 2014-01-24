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
#include "xfs_mount.h"
#include "xfs_da_btree.h"
#include "xfs_bmap_btree.h"
#include "xfs_alloc_btree.h"
#include "xfs_ialloc_btree.h"
#include "xfs_alloc.h"
#include "xfs_btree.h"
#include "xfs_attr_sf.h"
#include "xfs_dinode.h"
#include "xfs_inode.h"
#include "xfs_inode_item.h"
#include "xfs_bmap.h"
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
#include "xfs_inode.h"
#include "xfs_trans.h"
#include "xfs_inode_item.h"
#include "xfs_bmap_btree.h"
#include "xfs_bmap.h"
#include "xfs_attr_sf.h"
#include "xfs_attr_remote.h"
>>>>>>> refs/remotes/origin/master
#include "xfs_attr.h"
#include "xfs_attr_leaf.h"
#include "xfs_error.h"
#include "xfs_trace.h"
<<<<<<< HEAD
=======
#include "xfs_buf_item.h"
#include "xfs_cksum.h"
#include "xfs_dinode.h"
#include "xfs_dir2.h"

>>>>>>> refs/remotes/origin/master

/*
 * xfs_attr_leaf.c
 *
 * Routines to implement leaf blocks of attributes as Btrees of hashed names.
 */

/*========================================================================
 * Function prototypes for the kernel.
 *========================================================================*/

/*
 * Routines used for growing the Btree.
 */
<<<<<<< HEAD
STATIC int xfs_attr_leaf_create(xfs_da_args_t *args, xfs_dablk_t which_block,
				    xfs_dabuf_t **bpp);
STATIC int xfs_attr_leaf_add_work(xfs_dabuf_t *leaf_buffer, xfs_da_args_t *args,
					      int freemap_index);
STATIC void xfs_attr_leaf_compact(xfs_trans_t *trans, xfs_dabuf_t *leaf_buffer);
STATIC void xfs_attr_leaf_rebalance(xfs_da_state_t *state,
						   xfs_da_state_blk_t *blk1,
						   xfs_da_state_blk_t *blk2);
STATIC int xfs_attr_leaf_figure_balance(xfs_da_state_t *state,
					   xfs_da_state_blk_t *leaf_blk_1,
					   xfs_da_state_blk_t *leaf_blk_2,
					   int *number_entries_in_blk1,
					   int *number_usedbytes_in_blk1);

/*
 * Routines used for shrinking the Btree.
 */
STATIC int xfs_attr_node_inactive(xfs_trans_t **trans, xfs_inode_t *dp,
				  xfs_dabuf_t *bp, int level);
STATIC int xfs_attr_leaf_inactive(xfs_trans_t **trans, xfs_inode_t *dp,
				  xfs_dabuf_t *bp);
STATIC int xfs_attr_leaf_freextent(xfs_trans_t **trans, xfs_inode_t *dp,
				   xfs_dablk_t blkno, int blkcnt);

/*
 * Utility routines.
 */
STATIC void xfs_attr_leaf_moveents(xfs_attr_leafblock_t *src_leaf,
					 int src_start,
					 xfs_attr_leafblock_t *dst_leaf,
					 int dst_start, int move_count,
					 xfs_mount_t *mp);
STATIC int xfs_attr_leaf_entsize(xfs_attr_leafblock_t *leaf, int index);
=======
STATIC int xfs_attr3_leaf_create(struct xfs_da_args *args,
				 xfs_dablk_t which_block, struct xfs_buf **bpp);
STATIC int xfs_attr3_leaf_add_work(struct xfs_buf *leaf_buffer,
				   struct xfs_attr3_icleaf_hdr *ichdr,
				   struct xfs_da_args *args, int freemap_index);
STATIC void xfs_attr3_leaf_compact(struct xfs_da_args *args,
				   struct xfs_attr3_icleaf_hdr *ichdr,
				   struct xfs_buf *leaf_buffer);
STATIC void xfs_attr3_leaf_rebalance(xfs_da_state_t *state,
						   xfs_da_state_blk_t *blk1,
						   xfs_da_state_blk_t *blk2);
STATIC int xfs_attr3_leaf_figure_balance(xfs_da_state_t *state,
			xfs_da_state_blk_t *leaf_blk_1,
			struct xfs_attr3_icleaf_hdr *ichdr1,
			xfs_da_state_blk_t *leaf_blk_2,
			struct xfs_attr3_icleaf_hdr *ichdr2,
			int *number_entries_in_blk1,
			int *number_usedbytes_in_blk1);

/*
 * Utility routines.
 */
STATIC void xfs_attr3_leaf_moveents(struct xfs_attr_leafblock *src_leaf,
			struct xfs_attr3_icleaf_hdr *src_ichdr, int src_start,
			struct xfs_attr_leafblock *dst_leaf,
			struct xfs_attr3_icleaf_hdr *dst_ichdr, int dst_start,
			int move_count, struct xfs_mount *mp);
STATIC int xfs_attr_leaf_entsize(xfs_attr_leafblock_t *leaf, int index);

void
xfs_attr3_leaf_hdr_from_disk(
	struct xfs_attr3_icleaf_hdr	*to,
	struct xfs_attr_leafblock	*from)
{
	int	i;

	ASSERT(from->hdr.info.magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC) ||
	       from->hdr.info.magic == cpu_to_be16(XFS_ATTR3_LEAF_MAGIC));

	if (from->hdr.info.magic == cpu_to_be16(XFS_ATTR3_LEAF_MAGIC)) {
		struct xfs_attr3_leaf_hdr *hdr3 = (struct xfs_attr3_leaf_hdr *)from;

		to->forw = be32_to_cpu(hdr3->info.hdr.forw);
		to->back = be32_to_cpu(hdr3->info.hdr.back);
		to->magic = be16_to_cpu(hdr3->info.hdr.magic);
		to->count = be16_to_cpu(hdr3->count);
		to->usedbytes = be16_to_cpu(hdr3->usedbytes);
		to->firstused = be16_to_cpu(hdr3->firstused);
		to->holes = hdr3->holes;

		for (i = 0; i < XFS_ATTR_LEAF_MAPSIZE; i++) {
			to->freemap[i].base = be16_to_cpu(hdr3->freemap[i].base);
			to->freemap[i].size = be16_to_cpu(hdr3->freemap[i].size);
		}
		return;
	}
	to->forw = be32_to_cpu(from->hdr.info.forw);
	to->back = be32_to_cpu(from->hdr.info.back);
	to->magic = be16_to_cpu(from->hdr.info.magic);
	to->count = be16_to_cpu(from->hdr.count);
	to->usedbytes = be16_to_cpu(from->hdr.usedbytes);
	to->firstused = be16_to_cpu(from->hdr.firstused);
	to->holes = from->hdr.holes;

	for (i = 0; i < XFS_ATTR_LEAF_MAPSIZE; i++) {
		to->freemap[i].base = be16_to_cpu(from->hdr.freemap[i].base);
		to->freemap[i].size = be16_to_cpu(from->hdr.freemap[i].size);
	}
}

void
xfs_attr3_leaf_hdr_to_disk(
	struct xfs_attr_leafblock	*to,
	struct xfs_attr3_icleaf_hdr	*from)
{
	int	i;

	ASSERT(from->magic == XFS_ATTR_LEAF_MAGIC ||
	       from->magic == XFS_ATTR3_LEAF_MAGIC);

	if (from->magic == XFS_ATTR3_LEAF_MAGIC) {
		struct xfs_attr3_leaf_hdr *hdr3 = (struct xfs_attr3_leaf_hdr *)to;

		hdr3->info.hdr.forw = cpu_to_be32(from->forw);
		hdr3->info.hdr.back = cpu_to_be32(from->back);
		hdr3->info.hdr.magic = cpu_to_be16(from->magic);
		hdr3->count = cpu_to_be16(from->count);
		hdr3->usedbytes = cpu_to_be16(from->usedbytes);
		hdr3->firstused = cpu_to_be16(from->firstused);
		hdr3->holes = from->holes;
		hdr3->pad1 = 0;

		for (i = 0; i < XFS_ATTR_LEAF_MAPSIZE; i++) {
			hdr3->freemap[i].base = cpu_to_be16(from->freemap[i].base);
			hdr3->freemap[i].size = cpu_to_be16(from->freemap[i].size);
		}
		return;
	}
	to->hdr.info.forw = cpu_to_be32(from->forw);
	to->hdr.info.back = cpu_to_be32(from->back);
	to->hdr.info.magic = cpu_to_be16(from->magic);
	to->hdr.count = cpu_to_be16(from->count);
	to->hdr.usedbytes = cpu_to_be16(from->usedbytes);
	to->hdr.firstused = cpu_to_be16(from->firstused);
	to->hdr.holes = from->holes;
	to->hdr.pad1 = 0;

	for (i = 0; i < XFS_ATTR_LEAF_MAPSIZE; i++) {
		to->hdr.freemap[i].base = cpu_to_be16(from->freemap[i].base);
		to->hdr.freemap[i].size = cpu_to_be16(from->freemap[i].size);
	}
}

static bool
xfs_attr3_leaf_verify(
	struct xfs_buf		*bp)
{
	struct xfs_mount	*mp = bp->b_target->bt_mount;
	struct xfs_attr_leafblock *leaf = bp->b_addr;
	struct xfs_attr3_icleaf_hdr ichdr;

	xfs_attr3_leaf_hdr_from_disk(&ichdr, leaf);

	if (xfs_sb_version_hascrc(&mp->m_sb)) {
		struct xfs_da3_node_hdr *hdr3 = bp->b_addr;

		if (ichdr.magic != XFS_ATTR3_LEAF_MAGIC)
			return false;

		if (!uuid_equal(&hdr3->info.uuid, &mp->m_sb.sb_uuid))
			return false;
		if (be64_to_cpu(hdr3->info.blkno) != bp->b_bn)
			return false;
	} else {
		if (ichdr.magic != XFS_ATTR_LEAF_MAGIC)
			return false;
	}
	if (ichdr.count == 0)
		return false;

	/* XXX: need to range check rest of attr header values */
	/* XXX: hash order check? */

	return true;
}

static void
xfs_attr3_leaf_write_verify(
	struct xfs_buf	*bp)
{
	struct xfs_mount	*mp = bp->b_target->bt_mount;
	struct xfs_buf_log_item	*bip = bp->b_fspriv;
	struct xfs_attr3_leaf_hdr *hdr3 = bp->b_addr;

	if (!xfs_attr3_leaf_verify(bp)) {
		XFS_CORRUPTION_ERROR(__func__, XFS_ERRLEVEL_LOW, mp, bp->b_addr);
		xfs_buf_ioerror(bp, EFSCORRUPTED);
		return;
	}

	if (!xfs_sb_version_hascrc(&mp->m_sb))
		return;

	if (bip)
		hdr3->info.lsn = cpu_to_be64(bip->bli_item.li_lsn);

	xfs_update_cksum(bp->b_addr, BBTOB(bp->b_length), XFS_ATTR3_LEAF_CRC_OFF);
}

/*
 * leaf/node format detection on trees is sketchy, so a node read can be done on
 * leaf level blocks when detection identifies the tree as a node format tree
 * incorrectly. In this case, we need to swap the verifier to match the correct
 * format of the block being read.
 */
static void
xfs_attr3_leaf_read_verify(
	struct xfs_buf		*bp)
{
	struct xfs_mount	*mp = bp->b_target->bt_mount;

	if ((xfs_sb_version_hascrc(&mp->m_sb) &&
	     !xfs_verify_cksum(bp->b_addr, BBTOB(bp->b_length),
					  XFS_ATTR3_LEAF_CRC_OFF)) ||
	    !xfs_attr3_leaf_verify(bp)) {
		XFS_CORRUPTION_ERROR(__func__, XFS_ERRLEVEL_LOW, mp, bp->b_addr);
		xfs_buf_ioerror(bp, EFSCORRUPTED);
	}
}

const struct xfs_buf_ops xfs_attr3_leaf_buf_ops = {
	.verify_read = xfs_attr3_leaf_read_verify,
	.verify_write = xfs_attr3_leaf_write_verify,
};

int
xfs_attr3_leaf_read(
	struct xfs_trans	*tp,
	struct xfs_inode	*dp,
	xfs_dablk_t		bno,
	xfs_daddr_t		mappedbno,
	struct xfs_buf		**bpp)
{
	int			err;

	err = xfs_da_read_buf(tp, dp, bno, mappedbno, bpp,
				XFS_ATTR_FORK, &xfs_attr3_leaf_buf_ops);
	if (!err && tp)
		xfs_trans_buf_set_type(tp, *bpp, XFS_BLFT_ATTR_LEAF_BUF);
	return err;
}
>>>>>>> refs/remotes/origin/master

/*========================================================================
 * Namespace helper routines
 *========================================================================*/

/*
 * If namespace bits don't match return 0.
 * If all match then return 1.
 */
STATIC int
xfs_attr_namesp_match(int arg_flags, int ondisk_flags)
{
	return XFS_ATTR_NSP_ONDISK(ondisk_flags) == XFS_ATTR_NSP_ARGS_TO_ONDISK(arg_flags);
}


/*========================================================================
 * External routines when attribute fork size < XFS_LITINO(mp).
 *========================================================================*/

/*
 * Query whether the requested number of additional bytes of extended
 * attribute space will be able to fit inline.
 *
 * Returns zero if not, else the di_forkoff fork offset to be used in the
 * literal area for attribute data once the new bytes have been added.
 *
 * di_forkoff must be 8 byte aligned, hence is stored as a >>3 value;
 * special case for dev/uuid inodes, they have fixed size data forks.
 */
int
xfs_attr_shortform_bytesfit(xfs_inode_t *dp, int bytes)
{
	int offset;
	int minforkoff;	/* lower limit on valid forkoff locations */
	int maxforkoff;	/* upper limit on valid forkoff locations */
	int dsize;
	xfs_mount_t *mp = dp->i_mount;

<<<<<<< HEAD
	offset = (XFS_LITINO(mp) - bytes) >> 3; /* rounded down */
=======
	/* rounded down */
	offset = (XFS_LITINO(mp, dp->i_d.di_version) - bytes) >> 3;
>>>>>>> refs/remotes/origin/master

	switch (dp->i_d.di_format) {
	case XFS_DINODE_FMT_DEV:
		minforkoff = roundup(sizeof(xfs_dev_t), 8) >> 3;
		return (offset >= minforkoff) ? minforkoff : 0;
	case XFS_DINODE_FMT_UUID:
		minforkoff = roundup(sizeof(uuid_t), 8) >> 3;
		return (offset >= minforkoff) ? minforkoff : 0;
	}

	/*
	 * If the requested numbers of bytes is smaller or equal to the
	 * current attribute fork size we can always proceed.
	 *
	 * Note that if_bytes in the data fork might actually be larger than
	 * the current data fork size is due to delalloc extents. In that
	 * case either the extent count will go down when they are converted
	 * to real extents, or the delalloc conversion will take care of the
	 * literal area rebalancing.
	 */
	if (bytes <= XFS_IFORK_ASIZE(dp))
		return dp->i_d.di_forkoff;

	/*
	 * For attr2 we can try to move the forkoff if there is space in the
	 * literal area, but for the old format we are done if there is no
	 * space in the fixed attribute fork.
	 */
	if (!(mp->m_flags & XFS_MOUNT_ATTR2))
		return 0;

	dsize = dp->i_df.if_bytes;

	switch (dp->i_d.di_format) {
	case XFS_DINODE_FMT_EXTENTS:
		/*
		 * If there is no attr fork and the data fork is extents, 
		 * determine if creating the default attr fork will result
		 * in the extents form migrating to btree. If so, the
		 * minimum offset only needs to be the space required for
		 * the btree root.
		 */
		if (!dp->i_d.di_forkoff && dp->i_df.if_bytes >
		    xfs_default_attroffset(dp))
			dsize = XFS_BMDR_SPACE_CALC(MINDBTPTRS);
		break;
	case XFS_DINODE_FMT_BTREE:
		/*
		 * If we have a data btree then keep forkoff if we have one,
		 * otherwise we are adding a new attr, so then we set
		 * minforkoff to where the btree root can finish so we have
		 * plenty of room for attrs
		 */
		if (dp->i_d.di_forkoff) {
			if (offset < dp->i_d.di_forkoff)
				return 0;
			return dp->i_d.di_forkoff;
		}
<<<<<<< HEAD
		dsize = XFS_BMAP_BROOT_SPACE(dp->i_df.if_broot);
=======
		dsize = XFS_BMAP_BROOT_SPACE(mp, dp->i_df.if_broot);
>>>>>>> refs/remotes/origin/master
		break;
	}

	/*
	 * A data fork btree root must have space for at least
	 * MINDBTPTRS key/ptr pairs if the data fork is small or empty.
	 */
	minforkoff = MAX(dsize, XFS_BMDR_SPACE_CALC(MINDBTPTRS));
	minforkoff = roundup(minforkoff, 8) >> 3;

	/* attr fork btree root can have at least this many key/ptr pairs */
<<<<<<< HEAD
	maxforkoff = XFS_LITINO(mp) - XFS_BMDR_SPACE_CALC(MINABTPTRS);
=======
	maxforkoff = XFS_LITINO(mp, dp->i_d.di_version) -
			XFS_BMDR_SPACE_CALC(MINABTPTRS);
>>>>>>> refs/remotes/origin/master
	maxforkoff = maxforkoff >> 3;	/* rounded down */

	if (offset >= maxforkoff)
		return maxforkoff;
	if (offset >= minforkoff)
		return offset;
	return 0;
}

/*
 * Switch on the ATTR2 superblock bit (implies also FEATURES2)
 */
STATIC void
xfs_sbversion_add_attr2(xfs_mount_t *mp, xfs_trans_t *tp)
{
	if ((mp->m_flags & XFS_MOUNT_ATTR2) &&
	    !(xfs_sb_version_hasattr2(&mp->m_sb))) {
		spin_lock(&mp->m_sb_lock);
		if (!xfs_sb_version_hasattr2(&mp->m_sb)) {
			xfs_sb_version_addattr2(&mp->m_sb);
			spin_unlock(&mp->m_sb_lock);
			xfs_mod_sb(tp, XFS_SB_VERSIONNUM | XFS_SB_FEATURES2);
		} else
			spin_unlock(&mp->m_sb_lock);
	}
}

/*
 * Create the initial contents of a shortform attribute list.
 */
void
xfs_attr_shortform_create(xfs_da_args_t *args)
{
	xfs_attr_sf_hdr_t *hdr;
	xfs_inode_t *dp;
	xfs_ifork_t *ifp;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	trace_xfs_attr_sf_create(args);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	trace_xfs_attr_sf_create(args);

>>>>>>> refs/remotes/origin/master
	dp = args->dp;
	ASSERT(dp != NULL);
	ifp = dp->i_afp;
	ASSERT(ifp != NULL);
	ASSERT(ifp->if_bytes == 0);
	if (dp->i_d.di_aformat == XFS_DINODE_FMT_EXTENTS) {
		ifp->if_flags &= ~XFS_IFEXTENTS;	/* just in case */
		dp->i_d.di_aformat = XFS_DINODE_FMT_LOCAL;
		ifp->if_flags |= XFS_IFINLINE;
	} else {
		ASSERT(ifp->if_flags & XFS_IFINLINE);
	}
	xfs_idata_realloc(dp, sizeof(*hdr), XFS_ATTR_FORK);
	hdr = (xfs_attr_sf_hdr_t *)ifp->if_u1.if_data;
	hdr->count = 0;
	hdr->totsize = cpu_to_be16(sizeof(*hdr));
	xfs_trans_log_inode(args->trans, dp, XFS_ILOG_CORE | XFS_ILOG_ADATA);
}

/*
 * Add a name/value pair to the shortform attribute list.
 * Overflow from the inode has already been checked for.
 */
void
xfs_attr_shortform_add(xfs_da_args_t *args, int forkoff)
{
	xfs_attr_shortform_t *sf;
	xfs_attr_sf_entry_t *sfe;
	int i, offset, size;
	xfs_mount_t *mp;
	xfs_inode_t *dp;
	xfs_ifork_t *ifp;

<<<<<<< HEAD
<<<<<<< HEAD
	dp = args->dp;
	mp = dp->i_mount;
	dp->i_d.di_forkoff = forkoff;
	dp->i_df.if_ext_max =
		XFS_IFORK_DSIZE(dp) / (uint)sizeof(xfs_bmbt_rec_t);
	dp->i_afp->if_ext_max =
		XFS_IFORK_ASIZE(dp) / (uint)sizeof(xfs_bmbt_rec_t);
=======
=======
>>>>>>> refs/remotes/origin/master
	trace_xfs_attr_sf_add(args);

	dp = args->dp;
	mp = dp->i_mount;
	dp->i_d.di_forkoff = forkoff;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	ifp = dp->i_afp;
	ASSERT(ifp->if_flags & XFS_IFINLINE);
	sf = (xfs_attr_shortform_t *)ifp->if_u1.if_data;
	sfe = &sf->list[0];
	for (i = 0; i < sf->hdr.count; sfe = XFS_ATTR_SF_NEXTENTRY(sfe), i++) {
#ifdef DEBUG
		if (sfe->namelen != args->namelen)
			continue;
		if (memcmp(args->name, sfe->nameval, args->namelen) != 0)
			continue;
		if (!xfs_attr_namesp_match(args->flags, sfe->flags))
			continue;
		ASSERT(0);
#endif
	}

	offset = (char *)sfe - (char *)sf;
	size = XFS_ATTR_SF_ENTSIZE_BYNAME(args->namelen, args->valuelen);
	xfs_idata_realloc(dp, size, XFS_ATTR_FORK);
	sf = (xfs_attr_shortform_t *)ifp->if_u1.if_data;
	sfe = (xfs_attr_sf_entry_t *)((char *)sf + offset);

	sfe->namelen = args->namelen;
	sfe->valuelen = args->valuelen;
	sfe->flags = XFS_ATTR_NSP_ARGS_TO_ONDISK(args->flags);
	memcpy(sfe->nameval, args->name, args->namelen);
	memcpy(&sfe->nameval[args->namelen], args->value, args->valuelen);
	sf->hdr.count++;
	be16_add_cpu(&sf->hdr.totsize, size);
	xfs_trans_log_inode(args->trans, dp, XFS_ILOG_CORE | XFS_ILOG_ADATA);

	xfs_sbversion_add_attr2(mp, args->trans);
}

/*
 * After the last attribute is removed revert to original inode format,
 * making all literal area available to the data fork once more.
 */
STATIC void
xfs_attr_fork_reset(
	struct xfs_inode	*ip,
	struct xfs_trans	*tp)
{
	xfs_idestroy_fork(ip, XFS_ATTR_FORK);
	ip->i_d.di_forkoff = 0;
	ip->i_d.di_aformat = XFS_DINODE_FMT_EXTENTS;

	ASSERT(ip->i_d.di_anextents == 0);
	ASSERT(ip->i_afp == NULL);

<<<<<<< HEAD
<<<<<<< HEAD
	ip->i_df.if_ext_max = XFS_IFORK_DSIZE(ip) / sizeof(xfs_bmbt_rec_t);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	xfs_trans_log_inode(tp, ip, XFS_ILOG_CORE);
}

/*
 * Remove an attribute from the shortform attribute list structure.
 */
int
xfs_attr_shortform_remove(xfs_da_args_t *args)
{
	xfs_attr_shortform_t *sf;
	xfs_attr_sf_entry_t *sfe;
	int base, size=0, end, totsize, i;
	xfs_mount_t *mp;
	xfs_inode_t *dp;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	trace_xfs_attr_sf_remove(args);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	trace_xfs_attr_sf_remove(args);

>>>>>>> refs/remotes/origin/master
	dp = args->dp;
	mp = dp->i_mount;
	base = sizeof(xfs_attr_sf_hdr_t);
	sf = (xfs_attr_shortform_t *)dp->i_afp->if_u1.if_data;
	sfe = &sf->list[0];
	end = sf->hdr.count;
	for (i = 0; i < end; sfe = XFS_ATTR_SF_NEXTENTRY(sfe),
					base += size, i++) {
		size = XFS_ATTR_SF_ENTSIZE(sfe);
		if (sfe->namelen != args->namelen)
			continue;
		if (memcmp(sfe->nameval, args->name, args->namelen) != 0)
			continue;
		if (!xfs_attr_namesp_match(args->flags, sfe->flags))
			continue;
		break;
	}
	if (i == end)
		return(XFS_ERROR(ENOATTR));

	/*
	 * Fix up the attribute fork data, covering the hole
	 */
	end = base + size;
	totsize = be16_to_cpu(sf->hdr.totsize);
	if (end != totsize)
		memmove(&((char *)sf)[base], &((char *)sf)[end], totsize - end);
	sf->hdr.count--;
	be16_add_cpu(&sf->hdr.totsize, -size);

	/*
	 * Fix up the start offset of the attribute fork
	 */
	totsize -= size;
	if (totsize == sizeof(xfs_attr_sf_hdr_t) &&
	    (mp->m_flags & XFS_MOUNT_ATTR2) &&
	    (dp->i_d.di_format != XFS_DINODE_FMT_BTREE) &&
	    !(args->op_flags & XFS_DA_OP_ADDNAME)) {
		xfs_attr_fork_reset(dp, args->trans);
	} else {
		xfs_idata_realloc(dp, -size, XFS_ATTR_FORK);
		dp->i_d.di_forkoff = xfs_attr_shortform_bytesfit(dp, totsize);
		ASSERT(dp->i_d.di_forkoff);
		ASSERT(totsize > sizeof(xfs_attr_sf_hdr_t) ||
				(args->op_flags & XFS_DA_OP_ADDNAME) ||
				!(mp->m_flags & XFS_MOUNT_ATTR2) ||
				dp->i_d.di_format == XFS_DINODE_FMT_BTREE);
<<<<<<< HEAD
<<<<<<< HEAD
		dp->i_afp->if_ext_max =
			XFS_IFORK_ASIZE(dp) / (uint)sizeof(xfs_bmbt_rec_t);
		dp->i_df.if_ext_max =
			XFS_IFORK_DSIZE(dp) / (uint)sizeof(xfs_bmbt_rec_t);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		xfs_trans_log_inode(args->trans, dp,
					XFS_ILOG_CORE | XFS_ILOG_ADATA);
	}

	xfs_sbversion_add_attr2(mp, args->trans);

	return(0);
}

/*
 * Look up a name in a shortform attribute list structure.
 */
/*ARGSUSED*/
int
xfs_attr_shortform_lookup(xfs_da_args_t *args)
{
	xfs_attr_shortform_t *sf;
	xfs_attr_sf_entry_t *sfe;
	int i;
	xfs_ifork_t *ifp;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	trace_xfs_attr_sf_lookup(args);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	trace_xfs_attr_sf_lookup(args);

>>>>>>> refs/remotes/origin/master
	ifp = args->dp->i_afp;
	ASSERT(ifp->if_flags & XFS_IFINLINE);
	sf = (xfs_attr_shortform_t *)ifp->if_u1.if_data;
	sfe = &sf->list[0];
	for (i = 0; i < sf->hdr.count;
				sfe = XFS_ATTR_SF_NEXTENTRY(sfe), i++) {
		if (sfe->namelen != args->namelen)
			continue;
		if (memcmp(args->name, sfe->nameval, args->namelen) != 0)
			continue;
		if (!xfs_attr_namesp_match(args->flags, sfe->flags))
			continue;
		return(XFS_ERROR(EEXIST));
	}
	return(XFS_ERROR(ENOATTR));
}

/*
 * Look up a name in a shortform attribute list structure.
 */
/*ARGSUSED*/
int
xfs_attr_shortform_getvalue(xfs_da_args_t *args)
{
	xfs_attr_shortform_t *sf;
	xfs_attr_sf_entry_t *sfe;
	int i;

<<<<<<< HEAD
	ASSERT(args->dp->i_d.di_aformat == XFS_IFINLINE);
=======
	ASSERT(args->dp->i_afp->if_flags == XFS_IFINLINE);
>>>>>>> refs/remotes/origin/master
	sf = (xfs_attr_shortform_t *)args->dp->i_afp->if_u1.if_data;
	sfe = &sf->list[0];
	for (i = 0; i < sf->hdr.count;
				sfe = XFS_ATTR_SF_NEXTENTRY(sfe), i++) {
		if (sfe->namelen != args->namelen)
			continue;
		if (memcmp(args->name, sfe->nameval, args->namelen) != 0)
			continue;
		if (!xfs_attr_namesp_match(args->flags, sfe->flags))
			continue;
		if (args->flags & ATTR_KERNOVAL) {
			args->valuelen = sfe->valuelen;
			return(XFS_ERROR(EEXIST));
		}
		if (args->valuelen < sfe->valuelen) {
			args->valuelen = sfe->valuelen;
			return(XFS_ERROR(ERANGE));
		}
		args->valuelen = sfe->valuelen;
		memcpy(args->value, &sfe->nameval[args->namelen],
						    args->valuelen);
		return(XFS_ERROR(EEXIST));
	}
	return(XFS_ERROR(ENOATTR));
}

/*
 * Convert from using the shortform to the leaf.
 */
int
xfs_attr_shortform_to_leaf(xfs_da_args_t *args)
{
	xfs_inode_t *dp;
	xfs_attr_shortform_t *sf;
	xfs_attr_sf_entry_t *sfe;
	xfs_da_args_t nargs;
	char *tmpbuffer;
	int error, i, size;
	xfs_dablk_t blkno;
<<<<<<< HEAD
	xfs_dabuf_t *bp;
	xfs_ifork_t *ifp;

<<<<<<< HEAD
=======
	trace_xfs_attr_sf_to_leaf(args);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct xfs_buf *bp;
	xfs_ifork_t *ifp;

	trace_xfs_attr_sf_to_leaf(args);

>>>>>>> refs/remotes/origin/master
	dp = args->dp;
	ifp = dp->i_afp;
	sf = (xfs_attr_shortform_t *)ifp->if_u1.if_data;
	size = be16_to_cpu(sf->hdr.totsize);
	tmpbuffer = kmem_alloc(size, KM_SLEEP);
	ASSERT(tmpbuffer != NULL);
	memcpy(tmpbuffer, ifp->if_u1.if_data, size);
	sf = (xfs_attr_shortform_t *)tmpbuffer;

	xfs_idata_realloc(dp, -size, XFS_ATTR_FORK);
<<<<<<< HEAD
=======
	xfs_bmap_local_to_extents_empty(dp, XFS_ATTR_FORK);

>>>>>>> refs/remotes/origin/master
	bp = NULL;
	error = xfs_da_grow_inode(args, &blkno);
	if (error) {
		/*
		 * If we hit an IO error middle of the transaction inside
		 * grow_inode(), we may have inconsistent data. Bail out.
		 */
		if (error == EIO)
			goto out;
		xfs_idata_realloc(dp, size, XFS_ATTR_FORK);	/* try to put */
		memcpy(ifp->if_u1.if_data, tmpbuffer, size);	/* it back */
		goto out;
	}

	ASSERT(blkno == 0);
<<<<<<< HEAD
	error = xfs_attr_leaf_create(args, blkno, &bp);
=======
	error = xfs_attr3_leaf_create(args, blkno, &bp);
>>>>>>> refs/remotes/origin/master
	if (error) {
		error = xfs_da_shrink_inode(args, 0, bp);
		bp = NULL;
		if (error)
			goto out;
		xfs_idata_realloc(dp, size, XFS_ATTR_FORK);	/* try to put */
		memcpy(ifp->if_u1.if_data, tmpbuffer, size);	/* it back */
		goto out;
	}

	memset((char *)&nargs, 0, sizeof(nargs));
	nargs.dp = dp;
	nargs.firstblock = args->firstblock;
	nargs.flist = args->flist;
	nargs.total = args->total;
	nargs.whichfork = XFS_ATTR_FORK;
	nargs.trans = args->trans;
	nargs.op_flags = XFS_DA_OP_OKNOENT;

	sfe = &sf->list[0];
	for (i = 0; i < sf->hdr.count; i++) {
		nargs.name = sfe->nameval;
		nargs.namelen = sfe->namelen;
		nargs.value = &sfe->nameval[nargs.namelen];
		nargs.valuelen = sfe->valuelen;
		nargs.hashval = xfs_da_hashname(sfe->nameval,
						sfe->namelen);
		nargs.flags = XFS_ATTR_NSP_ONDISK_TO_ARGS(sfe->flags);
<<<<<<< HEAD
		error = xfs_attr_leaf_lookup_int(bp, &nargs); /* set a->index */
		ASSERT(error == ENOATTR);
		error = xfs_attr_leaf_add(bp, &nargs);
=======
		error = xfs_attr3_leaf_lookup_int(bp, &nargs); /* set a->index */
		ASSERT(error == ENOATTR);
		error = xfs_attr3_leaf_add(bp, &nargs);
>>>>>>> refs/remotes/origin/master
		ASSERT(error != ENOSPC);
		if (error)
			goto out;
		sfe = XFS_ATTR_SF_NEXTENTRY(sfe);
	}
	error = 0;

out:
<<<<<<< HEAD
	if(bp)
		xfs_da_buf_done(bp);
=======
>>>>>>> refs/remotes/origin/master
	kmem_free(tmpbuffer);
	return(error);
}

<<<<<<< HEAD
STATIC int
xfs_attr_shortform_compare(const void *a, const void *b)
{
	xfs_attr_sf_sort_t *sa, *sb;

	sa = (xfs_attr_sf_sort_t *)a;
	sb = (xfs_attr_sf_sort_t *)b;
	if (sa->hash < sb->hash) {
		return(-1);
	} else if (sa->hash > sb->hash) {
		return(1);
	} else {
		return(sa->entno - sb->entno);
	}
}


#define XFS_ISRESET_CURSOR(cursor) \
	(!((cursor)->initted) && !((cursor)->hashval) && \
	 !((cursor)->blkno) && !((cursor)->offset))
/*
 * Copy out entries of shortform attribute lists for attr_list().
 * Shortform attribute lists are not stored in hashval sorted order.
 * If the output buffer is not large enough to hold them all, then we
 * we have to calculate each entries' hashvalue and sort them before
 * we can begin returning them to the user.
 */
/*ARGSUSED*/
int
xfs_attr_shortform_list(xfs_attr_list_context_t *context)
{
	attrlist_cursor_kern_t *cursor;
	xfs_attr_sf_sort_t *sbuf, *sbp;
	xfs_attr_shortform_t *sf;
	xfs_attr_sf_entry_t *sfe;
	xfs_inode_t *dp;
	int sbsize, nsbuf, count, i;
	int error;

	ASSERT(context != NULL);
	dp = context->dp;
	ASSERT(dp != NULL);
	ASSERT(dp->i_afp != NULL);
	sf = (xfs_attr_shortform_t *)dp->i_afp->if_u1.if_data;
	ASSERT(sf != NULL);
	if (!sf->hdr.count)
		return(0);
	cursor = context->cursor;
	ASSERT(cursor != NULL);

	trace_xfs_attr_list_sf(context);

	/*
	 * If the buffer is large enough and the cursor is at the start,
	 * do not bother with sorting since we will return everything in
	 * one buffer and another call using the cursor won't need to be
	 * made.
	 * Note the generous fudge factor of 16 overhead bytes per entry.
	 * If bufsize is zero then put_listent must be a search function
	 * and can just scan through what we have.
	 */
	if (context->bufsize == 0 ||
	    (XFS_ISRESET_CURSOR(cursor) &&
             (dp->i_afp->if_bytes + sf->hdr.count * 16) < context->bufsize)) {
		for (i = 0, sfe = &sf->list[0]; i < sf->hdr.count; i++) {
			error = context->put_listent(context,
					   sfe->flags,
					   sfe->nameval,
					   (int)sfe->namelen,
					   (int)sfe->valuelen,
					   &sfe->nameval[sfe->namelen]);

			/*
			 * Either search callback finished early or
			 * didn't fit it all in the buffer after all.
			 */
			if (context->seen_enough)
				break;

			if (error)
				return error;
			sfe = XFS_ATTR_SF_NEXTENTRY(sfe);
		}
		trace_xfs_attr_list_sf_all(context);
		return(0);
	}

	/* do no more for a search callback */
	if (context->bufsize == 0)
		return 0;

	/*
	 * It didn't all fit, so we have to sort everything on hashval.
	 */
	sbsize = sf->hdr.count * sizeof(*sbuf);
	sbp = sbuf = kmem_alloc(sbsize, KM_SLEEP | KM_NOFS);

	/*
	 * Scan the attribute list for the rest of the entries, storing
	 * the relevant info from only those that match into a buffer.
	 */
	nsbuf = 0;
	for (i = 0, sfe = &sf->list[0]; i < sf->hdr.count; i++) {
		if (unlikely(
		    ((char *)sfe < (char *)sf) ||
		    ((char *)sfe >= ((char *)sf + dp->i_afp->if_bytes)))) {
			XFS_CORRUPTION_ERROR("xfs_attr_shortform_list",
					     XFS_ERRLEVEL_LOW,
					     context->dp->i_mount, sfe);
			kmem_free(sbuf);
			return XFS_ERROR(EFSCORRUPTED);
		}

		sbp->entno = i;
		sbp->hash = xfs_da_hashname(sfe->nameval, sfe->namelen);
		sbp->name = sfe->nameval;
		sbp->namelen = sfe->namelen;
		/* These are bytes, and both on-disk, don't endian-flip */
		sbp->valuelen = sfe->valuelen;
		sbp->flags = sfe->flags;
		sfe = XFS_ATTR_SF_NEXTENTRY(sfe);
		sbp++;
		nsbuf++;
	}

	/*
	 * Sort the entries on hash then entno.
	 */
	xfs_sort(sbuf, nsbuf, sizeof(*sbuf), xfs_attr_shortform_compare);

	/*
	 * Re-find our place IN THE SORTED LIST.
	 */
	count = 0;
	cursor->initted = 1;
	cursor->blkno = 0;
	for (sbp = sbuf, i = 0; i < nsbuf; i++, sbp++) {
		if (sbp->hash == cursor->hashval) {
			if (cursor->offset == count) {
				break;
			}
			count++;
		} else if (sbp->hash > cursor->hashval) {
			break;
		}
	}
	if (i == nsbuf) {
		kmem_free(sbuf);
		return(0);
	}

	/*
	 * Loop putting entries into the user buffer.
	 */
	for ( ; i < nsbuf; i++, sbp++) {
		if (cursor->hashval != sbp->hash) {
			cursor->hashval = sbp->hash;
			cursor->offset = 0;
		}
		error = context->put_listent(context,
					sbp->flags,
					sbp->name,
					sbp->namelen,
					sbp->valuelen,
					&sbp->name[sbp->namelen]);
		if (error)
			return error;
		if (context->seen_enough)
			break;
		cursor->offset++;
	}

	kmem_free(sbuf);
	return(0);
}

=======
>>>>>>> refs/remotes/origin/master
/*
 * Check a leaf attribute block to see if all the entries would fit into
 * a shortform attribute list.
 */
int
<<<<<<< HEAD
xfs_attr_shortform_allfit(xfs_dabuf_t *bp, xfs_inode_t *dp)
{
	xfs_attr_leafblock_t *leaf;
	xfs_attr_leaf_entry_t *entry;
	xfs_attr_leaf_name_local_t *name_loc;
	int bytes, i;

	leaf = bp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_ATTR_LEAF_MAGIC);
=======
	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0

	entry = &leaf->entries[0];
	bytes = sizeof(struct xfs_attr_sf_hdr);
	for (i = 0; i < be16_to_cpu(leaf->hdr.count); entry++, i++) {
=======
xfs_attr_shortform_allfit(
	struct xfs_buf		*bp,
	struct xfs_inode	*dp)
{
	struct xfs_attr_leafblock *leaf;
	struct xfs_attr_leaf_entry *entry;
	xfs_attr_leaf_name_local_t *name_loc;
	struct xfs_attr3_icleaf_hdr leafhdr;
	int			bytes;
	int			i;

	leaf = bp->b_addr;
	xfs_attr3_leaf_hdr_from_disk(&leafhdr, leaf);
	entry = xfs_attr3_leaf_entryp(leaf);

	bytes = sizeof(struct xfs_attr_sf_hdr);
	for (i = 0; i < leafhdr.count; entry++, i++) {
>>>>>>> refs/remotes/origin/master
		if (entry->flags & XFS_ATTR_INCOMPLETE)
			continue;		/* don't copy partial entries */
		if (!(entry->flags & XFS_ATTR_LOCAL))
			return(0);
<<<<<<< HEAD
		name_loc = xfs_attr_leaf_name_local(leaf, i);
=======
		name_loc = xfs_attr3_leaf_name_local(leaf, i);
>>>>>>> refs/remotes/origin/master
		if (name_loc->namelen >= XFS_ATTR_SF_ENTSIZE_MAX)
			return(0);
		if (be16_to_cpu(name_loc->valuelen) >= XFS_ATTR_SF_ENTSIZE_MAX)
			return(0);
<<<<<<< HEAD
		bytes += sizeof(struct xfs_attr_sf_entry)-1
=======
		bytes += sizeof(struct xfs_attr_sf_entry) - 1
>>>>>>> refs/remotes/origin/master
				+ name_loc->namelen
				+ be16_to_cpu(name_loc->valuelen);
	}
	if ((dp->i_mount->m_flags & XFS_MOUNT_ATTR2) &&
	    (dp->i_d.di_format != XFS_DINODE_FMT_BTREE) &&
	    (bytes == sizeof(struct xfs_attr_sf_hdr)))
<<<<<<< HEAD
		return(-1);
	return(xfs_attr_shortform_bytesfit(dp, bytes));
=======
		return -1;
	return xfs_attr_shortform_bytesfit(dp, bytes);
>>>>>>> refs/remotes/origin/master
}

/*
 * Convert a leaf attribute list to shortform attribute list
 */
int
<<<<<<< HEAD
xfs_attr_leaf_to_shortform(xfs_dabuf_t *bp, xfs_da_args_t *args, int forkoff)
{
	xfs_attr_leafblock_t *leaf;
	xfs_attr_leaf_entry_t *entry;
	xfs_attr_leaf_name_local_t *name_loc;
	xfs_da_args_t nargs;
	xfs_inode_t *dp;
	char *tmpbuffer;
	int error, i;

<<<<<<< HEAD
=======
	trace_xfs_attr_leaf_to_sf(args);

>>>>>>> refs/remotes/origin/cm-10.0
	dp = args->dp;
	tmpbuffer = kmem_alloc(XFS_LBSIZE(dp->i_mount), KM_SLEEP);
	ASSERT(tmpbuffer != NULL);

	ASSERT(bp != NULL);
	memcpy(tmpbuffer, bp->data, XFS_LBSIZE(dp->i_mount));
	leaf = (xfs_attr_leafblock_t *)tmpbuffer;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_ATTR_LEAF_MAGIC);
=======
	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	memset(bp->data, 0, XFS_LBSIZE(dp->i_mount));
=======
xfs_attr3_leaf_to_shortform(
	struct xfs_buf		*bp,
	struct xfs_da_args	*args,
	int			forkoff)
{
	struct xfs_attr_leafblock *leaf;
	struct xfs_attr3_icleaf_hdr ichdr;
	struct xfs_attr_leaf_entry *entry;
	struct xfs_attr_leaf_name_local *name_loc;
	struct xfs_da_args	nargs;
	struct xfs_inode	*dp = args->dp;
	char			*tmpbuffer;
	int			error;
	int			i;

	trace_xfs_attr_leaf_to_sf(args);

	tmpbuffer = kmem_alloc(XFS_LBSIZE(dp->i_mount), KM_SLEEP);
	if (!tmpbuffer)
		return ENOMEM;

	memcpy(tmpbuffer, bp->b_addr, XFS_LBSIZE(dp->i_mount));

	leaf = (xfs_attr_leafblock_t *)tmpbuffer;
	xfs_attr3_leaf_hdr_from_disk(&ichdr, leaf);
	entry = xfs_attr3_leaf_entryp(leaf);

	/* XXX (dgc): buffer is about to be marked stale - why zero it? */
	memset(bp->b_addr, 0, XFS_LBSIZE(dp->i_mount));
>>>>>>> refs/remotes/origin/master

	/*
	 * Clean out the prior contents of the attribute list.
	 */
	error = xfs_da_shrink_inode(args, 0, bp);
	if (error)
		goto out;

	if (forkoff == -1) {
		ASSERT(dp->i_mount->m_flags & XFS_MOUNT_ATTR2);
		ASSERT(dp->i_d.di_format != XFS_DINODE_FMT_BTREE);
		xfs_attr_fork_reset(dp, args->trans);
		goto out;
	}

	xfs_attr_shortform_create(args);

	/*
	 * Copy the attributes
	 */
	memset((char *)&nargs, 0, sizeof(nargs));
	nargs.dp = dp;
	nargs.firstblock = args->firstblock;
	nargs.flist = args->flist;
	nargs.total = args->total;
	nargs.whichfork = XFS_ATTR_FORK;
	nargs.trans = args->trans;
	nargs.op_flags = XFS_DA_OP_OKNOENT;
<<<<<<< HEAD
	entry = &leaf->entries[0];
	for (i = 0; i < be16_to_cpu(leaf->hdr.count); entry++, i++) {
=======

	for (i = 0; i < ichdr.count; entry++, i++) {
>>>>>>> refs/remotes/origin/master
		if (entry->flags & XFS_ATTR_INCOMPLETE)
			continue;	/* don't copy partial entries */
		if (!entry->nameidx)
			continue;
		ASSERT(entry->flags & XFS_ATTR_LOCAL);
<<<<<<< HEAD
		name_loc = xfs_attr_leaf_name_local(leaf, i);
=======
		name_loc = xfs_attr3_leaf_name_local(leaf, i);
>>>>>>> refs/remotes/origin/master
		nargs.name = name_loc->nameval;
		nargs.namelen = name_loc->namelen;
		nargs.value = &name_loc->nameval[nargs.namelen];
		nargs.valuelen = be16_to_cpu(name_loc->valuelen);
		nargs.hashval = be32_to_cpu(entry->hashval);
		nargs.flags = XFS_ATTR_NSP_ONDISK_TO_ARGS(entry->flags);
		xfs_attr_shortform_add(&nargs, forkoff);
	}
	error = 0;

out:
	kmem_free(tmpbuffer);
<<<<<<< HEAD
	return(error);
=======
	return error;
>>>>>>> refs/remotes/origin/master
}

/*
 * Convert from using a single leaf to a root node and a leaf.
 */
int
<<<<<<< HEAD
xfs_attr_leaf_to_node(xfs_da_args_t *args)
{
	xfs_attr_leafblock_t *leaf;
	xfs_da_intnode_t *node;
	xfs_inode_t *dp;
	xfs_dabuf_t *bp1, *bp2;
	xfs_dablk_t blkno;
	int error;

<<<<<<< HEAD
=======
	trace_xfs_attr_leaf_to_node(args);

>>>>>>> refs/remotes/origin/cm-10.0
	dp = args->dp;
	bp1 = bp2 = NULL;
	error = xfs_da_grow_inode(args, &blkno);
	if (error)
		goto out;
	error = xfs_da_read_buf(args->trans, args->dp, 0, -1, &bp1,
					     XFS_ATTR_FORK);
	if (error)
		goto out;
	ASSERT(bp1 != NULL);
	bp2 = NULL;
	error = xfs_da_get_buf(args->trans, args->dp, blkno, -1, &bp2,
					    XFS_ATTR_FORK);
	if (error)
		goto out;
	ASSERT(bp2 != NULL);
	memcpy(bp2->data, bp1->data, XFS_LBSIZE(dp->i_mount));
	xfs_da_buf_done(bp1);
	bp1 = NULL;
	xfs_da_log_buf(args->trans, bp2, 0, XFS_LBSIZE(dp->i_mount) - 1);
=======
xfs_attr3_leaf_to_node(
	struct xfs_da_args	*args)
{
	struct xfs_attr_leafblock *leaf;
	struct xfs_attr3_icleaf_hdr icleafhdr;
	struct xfs_attr_leaf_entry *entries;
	struct xfs_da_node_entry *btree;
	struct xfs_da3_icnode_hdr icnodehdr;
	struct xfs_da_intnode	*node;
	struct xfs_inode	*dp = args->dp;
	struct xfs_mount	*mp = dp->i_mount;
	struct xfs_buf		*bp1 = NULL;
	struct xfs_buf		*bp2 = NULL;
	xfs_dablk_t		blkno;
	int			error;

	trace_xfs_attr_leaf_to_node(args);

	error = xfs_da_grow_inode(args, &blkno);
	if (error)
		goto out;
	error = xfs_attr3_leaf_read(args->trans, dp, 0, -1, &bp1);
	if (error)
		goto out;

	error = xfs_da_get_buf(args->trans, dp, blkno, -1, &bp2, XFS_ATTR_FORK);
	if (error)
		goto out;

	/* copy leaf to new buffer, update identifiers */
	xfs_trans_buf_set_type(args->trans, bp2, XFS_BLFT_ATTR_LEAF_BUF);
	bp2->b_ops = bp1->b_ops;
	memcpy(bp2->b_addr, bp1->b_addr, XFS_LBSIZE(mp));
	if (xfs_sb_version_hascrc(&mp->m_sb)) {
		struct xfs_da3_blkinfo *hdr3 = bp2->b_addr;
		hdr3->blkno = cpu_to_be64(bp2->b_bn);
	}
	xfs_trans_log_buf(args->trans, bp2, 0, XFS_LBSIZE(mp) - 1);
>>>>>>> refs/remotes/origin/master

	/*
	 * Set up the new root node.
	 */
<<<<<<< HEAD
	error = xfs_da_node_create(args, 0, 1, &bp1, XFS_ATTR_FORK);
	if (error)
		goto out;
	node = bp1->data;
	leaf = bp2->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_ATTR_LEAF_MAGIC);
=======
	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	/* both on-disk, don't endian-flip twice */
	node->btree[0].hashval =
		leaf->entries[be16_to_cpu(leaf->hdr.count)-1 ].hashval;
	node->btree[0].before = cpu_to_be32(blkno);
	node->hdr.count = cpu_to_be16(1);
	xfs_da_log_buf(args->trans, bp1, 0, XFS_LBSIZE(dp->i_mount) - 1);
	error = 0;
out:
	if (bp1)
		xfs_da_buf_done(bp1);
	if (bp2)
		xfs_da_buf_done(bp2);
	return(error);
}


=======
	error = xfs_da3_node_create(args, 0, 1, &bp1, XFS_ATTR_FORK);
	if (error)
		goto out;
	node = bp1->b_addr;
	dp->d_ops->node_hdr_from_disk(&icnodehdr, node);
	btree = dp->d_ops->node_tree_p(node);

	leaf = bp2->b_addr;
	xfs_attr3_leaf_hdr_from_disk(&icleafhdr, leaf);
	entries = xfs_attr3_leaf_entryp(leaf);

	/* both on-disk, don't endian-flip twice */
	btree[0].hashval = entries[icleafhdr.count - 1].hashval;
	btree[0].before = cpu_to_be32(blkno);
	icnodehdr.count = 1;
	dp->d_ops->node_hdr_to_disk(node, &icnodehdr);
	xfs_trans_log_buf(args->trans, bp1, 0, XFS_LBSIZE(mp) - 1);
	error = 0;
out:
	return error;
}

>>>>>>> refs/remotes/origin/master
/*========================================================================
 * Routines used for growing the Btree.
 *========================================================================*/

/*
 * Create the initial contents of a leaf attribute list
 * or a leaf in a node attribute list.
 */
STATIC int
<<<<<<< HEAD
xfs_attr_leaf_create(xfs_da_args_t *args, xfs_dablk_t blkno, xfs_dabuf_t **bpp)
{
	xfs_attr_leafblock_t *leaf;
	xfs_attr_leaf_hdr_t *hdr;
	xfs_inode_t *dp;
	xfs_dabuf_t *bp;
	int error;

<<<<<<< HEAD
=======
	trace_xfs_attr_leaf_create(args);

>>>>>>> refs/remotes/origin/cm-10.0
	dp = args->dp;
	ASSERT(dp != NULL);
	error = xfs_da_get_buf(args->trans, args->dp, blkno, -1, &bp,
					    XFS_ATTR_FORK);
	if (error)
		return(error);
	ASSERT(bp != NULL);
	leaf = bp->data;
	memset((char *)leaf, 0, XFS_LBSIZE(dp->i_mount));
	hdr = &leaf->hdr;
	hdr->info.magic = cpu_to_be16(XFS_ATTR_LEAF_MAGIC);
	hdr->firstused = cpu_to_be16(XFS_LBSIZE(dp->i_mount));
	if (!hdr->firstused) {
		hdr->firstused = cpu_to_be16(
			XFS_LBSIZE(dp->i_mount) - XFS_ATTR_LEAF_NAME_ALIGN);
	}

	hdr->freemap[0].base = cpu_to_be16(sizeof(xfs_attr_leaf_hdr_t));
	hdr->freemap[0].size = cpu_to_be16(be16_to_cpu(hdr->firstused) -
					   sizeof(xfs_attr_leaf_hdr_t));

	xfs_da_log_buf(args->trans, bp, 0, XFS_LBSIZE(dp->i_mount) - 1);

	*bpp = bp;
	return(0);
=======
xfs_attr3_leaf_create(
	struct xfs_da_args	*args,
	xfs_dablk_t		blkno,
	struct xfs_buf		**bpp)
{
	struct xfs_attr_leafblock *leaf;
	struct xfs_attr3_icleaf_hdr ichdr;
	struct xfs_inode	*dp = args->dp;
	struct xfs_mount	*mp = dp->i_mount;
	struct xfs_buf		*bp;
	int			error;

	trace_xfs_attr_leaf_create(args);

	error = xfs_da_get_buf(args->trans, args->dp, blkno, -1, &bp,
					    XFS_ATTR_FORK);
	if (error)
		return error;
	bp->b_ops = &xfs_attr3_leaf_buf_ops;
	xfs_trans_buf_set_type(args->trans, bp, XFS_BLFT_ATTR_LEAF_BUF);
	leaf = bp->b_addr;
	memset(leaf, 0, XFS_LBSIZE(mp));

	memset(&ichdr, 0, sizeof(ichdr));
	ichdr.firstused = XFS_LBSIZE(mp);

	if (xfs_sb_version_hascrc(&mp->m_sb)) {
		struct xfs_da3_blkinfo *hdr3 = bp->b_addr;

		ichdr.magic = XFS_ATTR3_LEAF_MAGIC;

		hdr3->blkno = cpu_to_be64(bp->b_bn);
		hdr3->owner = cpu_to_be64(dp->i_ino);
		uuid_copy(&hdr3->uuid, &mp->m_sb.sb_uuid);

		ichdr.freemap[0].base = sizeof(struct xfs_attr3_leaf_hdr);
	} else {
		ichdr.magic = XFS_ATTR_LEAF_MAGIC;
		ichdr.freemap[0].base = sizeof(struct xfs_attr_leaf_hdr);
	}
	ichdr.freemap[0].size = ichdr.firstused - ichdr.freemap[0].base;

	xfs_attr3_leaf_hdr_to_disk(leaf, &ichdr);
	xfs_trans_log_buf(args->trans, bp, 0, XFS_LBSIZE(mp) - 1);

	*bpp = bp;
	return 0;
>>>>>>> refs/remotes/origin/master
}

/*
 * Split the leaf node, rebalance, then add the new entry.
 */
int
<<<<<<< HEAD
xfs_attr_leaf_split(xfs_da_state_t *state, xfs_da_state_blk_t *oldblk,
				   xfs_da_state_blk_t *newblk)
=======
xfs_attr3_leaf_split(
	struct xfs_da_state	*state,
	struct xfs_da_state_blk	*oldblk,
	struct xfs_da_state_blk	*newblk)
>>>>>>> refs/remotes/origin/master
{
	xfs_dablk_t blkno;
	int error;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	trace_xfs_attr_leaf_split(state->args);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	trace_xfs_attr_leaf_split(state->args);

>>>>>>> refs/remotes/origin/master
	/*
	 * Allocate space for a new leaf node.
	 */
	ASSERT(oldblk->magic == XFS_ATTR_LEAF_MAGIC);
	error = xfs_da_grow_inode(state->args, &blkno);
	if (error)
		return(error);
<<<<<<< HEAD
	error = xfs_attr_leaf_create(state->args, blkno, &newblk->bp);
=======
	error = xfs_attr3_leaf_create(state->args, blkno, &newblk->bp);
>>>>>>> refs/remotes/origin/master
	if (error)
		return(error);
	newblk->blkno = blkno;
	newblk->magic = XFS_ATTR_LEAF_MAGIC;

	/*
	 * Rebalance the entries across the two leaves.
	 * NOTE: rebalance() currently depends on the 2nd block being empty.
	 */
<<<<<<< HEAD
	xfs_attr_leaf_rebalance(state, oldblk, newblk);
	error = xfs_da_blk_link(state, oldblk, newblk);
=======
	xfs_attr3_leaf_rebalance(state, oldblk, newblk);
	error = xfs_da3_blk_link(state, oldblk, newblk);
>>>>>>> refs/remotes/origin/master
	if (error)
		return(error);

	/*
	 * Save info on "old" attribute for "atomic rename" ops, leaf_add()
	 * modifies the index/blkno/rmtblk/rmtblkcnt fields to show the
	 * "new" attrs info.  Will need the "old" info to remove it later.
	 *
	 * Insert the "new" entry in the correct block.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (state->inleaf)
		error = xfs_attr_leaf_add(oldblk->bp, state->args);
	else
		error = xfs_attr_leaf_add(newblk->bp, state->args);
=======
	if (state->inleaf) {
		trace_xfs_attr_leaf_add_old(state->args);
		error = xfs_attr_leaf_add(oldblk->bp, state->args);
	} else {
		trace_xfs_attr_leaf_add_new(state->args);
		error = xfs_attr_leaf_add(newblk->bp, state->args);
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (state->inleaf) {
		trace_xfs_attr_leaf_add_old(state->args);
		error = xfs_attr3_leaf_add(oldblk->bp, state->args);
	} else {
		trace_xfs_attr_leaf_add_new(state->args);
		error = xfs_attr3_leaf_add(newblk->bp, state->args);
	}
>>>>>>> refs/remotes/origin/master

	/*
	 * Update last hashval in each block since we added the name.
	 */
	oldblk->hashval = xfs_attr_leaf_lasthash(oldblk->bp, NULL);
	newblk->hashval = xfs_attr_leaf_lasthash(newblk->bp, NULL);
	return(error);
}

/*
 * Add a name to the leaf attribute list structure.
 */
int
<<<<<<< HEAD
xfs_attr_leaf_add(xfs_dabuf_t *bp, xfs_da_args_t *args)
{
	xfs_attr_leafblock_t *leaf;
	xfs_attr_leaf_hdr_t *hdr;
	xfs_attr_leaf_map_t *map;
	int tablesize, entsize, sum, tmp, i;

<<<<<<< HEAD
	leaf = bp->data;
	ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_ATTR_LEAF_MAGIC);
=======
	trace_xfs_attr_leaf_add(args);

	leaf = bp->data;
	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	ASSERT((args->index >= 0)
		&& (args->index <= be16_to_cpu(leaf->hdr.count)));
	hdr = &leaf->hdr;
=======
xfs_attr3_leaf_add(
	struct xfs_buf		*bp,
	struct xfs_da_args	*args)
{
	struct xfs_attr_leafblock *leaf;
	struct xfs_attr3_icleaf_hdr ichdr;
	int			tablesize;
	int			entsize;
	int			sum;
	int			tmp;
	int			i;

	trace_xfs_attr_leaf_add(args);

	leaf = bp->b_addr;
	xfs_attr3_leaf_hdr_from_disk(&ichdr, leaf);
	ASSERT(args->index >= 0 && args->index <= ichdr.count);
>>>>>>> refs/remotes/origin/master
	entsize = xfs_attr_leaf_newentsize(args->namelen, args->valuelen,
			   args->trans->t_mountp->m_sb.sb_blocksize, NULL);

	/*
	 * Search through freemap for first-fit on new name length.
	 * (may need to figure in size of entry struct too)
	 */
<<<<<<< HEAD
	tablesize = (be16_to_cpu(hdr->count) + 1)
					* sizeof(xfs_attr_leaf_entry_t)
					+ sizeof(xfs_attr_leaf_hdr_t);
	map = &hdr->freemap[XFS_ATTR_LEAF_MAPSIZE-1];
	for (sum = 0, i = XFS_ATTR_LEAF_MAPSIZE-1; i >= 0; map--, i--) {
		if (tablesize > be16_to_cpu(hdr->firstused)) {
			sum += be16_to_cpu(map->size);
			continue;
		}
		if (!map->size)
			continue;	/* no space in this map */
		tmp = entsize;
		if (be16_to_cpu(map->base) < be16_to_cpu(hdr->firstused))
			tmp += sizeof(xfs_attr_leaf_entry_t);
		if (be16_to_cpu(map->size) >= tmp) {
			tmp = xfs_attr_leaf_add_work(bp, args, i);
			return(tmp);
		}
		sum += be16_to_cpu(map->size);
=======
	tablesize = (ichdr.count + 1) * sizeof(xfs_attr_leaf_entry_t)
					+ xfs_attr3_leaf_hdr_size(leaf);
	for (sum = 0, i = XFS_ATTR_LEAF_MAPSIZE - 1; i >= 0; i--) {
		if (tablesize > ichdr.firstused) {
			sum += ichdr.freemap[i].size;
			continue;
		}
		if (!ichdr.freemap[i].size)
			continue;	/* no space in this map */
		tmp = entsize;
		if (ichdr.freemap[i].base < ichdr.firstused)
			tmp += sizeof(xfs_attr_leaf_entry_t);
		if (ichdr.freemap[i].size >= tmp) {
			tmp = xfs_attr3_leaf_add_work(bp, &ichdr, args, i);
			goto out_log_hdr;
		}
		sum += ichdr.freemap[i].size;
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * If there are no holes in the address space of the block,
	 * and we don't have enough freespace, then compaction will do us
	 * no good and we should just give up.
	 */
<<<<<<< HEAD
	if (!hdr->holes && (sum < entsize))
		return(XFS_ERROR(ENOSPC));
=======
	if (!ichdr.holes && sum < entsize)
		return XFS_ERROR(ENOSPC);
>>>>>>> refs/remotes/origin/master

	/*
	 * Compact the entries to coalesce free space.
	 * This may change the hdr->count via dropping INCOMPLETE entries.
	 */
<<<<<<< HEAD
	xfs_attr_leaf_compact(args->trans, bp);
=======
	xfs_attr3_leaf_compact(args, &ichdr, bp);
>>>>>>> refs/remotes/origin/master

	/*
	 * After compaction, the block is guaranteed to have only one
	 * free region, in freemap[0].  If it is not big enough, give up.
	 */
<<<<<<< HEAD
	if (be16_to_cpu(hdr->freemap[0].size)
				< (entsize + sizeof(xfs_attr_leaf_entry_t)))
		return(XFS_ERROR(ENOSPC));

	return(xfs_attr_leaf_add_work(bp, args, 0));
=======
	if (ichdr.freemap[0].size < (entsize + sizeof(xfs_attr_leaf_entry_t))) {
		tmp = ENOSPC;
		goto out_log_hdr;
	}

	tmp = xfs_attr3_leaf_add_work(bp, &ichdr, args, 0);

out_log_hdr:
	xfs_attr3_leaf_hdr_to_disk(leaf, &ichdr);
	xfs_trans_log_buf(args->trans, bp,
		XFS_DA_LOGRANGE(leaf, &leaf->hdr,
				xfs_attr3_leaf_hdr_size(leaf)));
	return tmp;
>>>>>>> refs/remotes/origin/master
}

/*
 * Add a name to a leaf attribute list structure.
 */
STATIC int
<<<<<<< HEAD
xfs_attr_leaf_add_work(xfs_dabuf_t *bp, xfs_da_args_t *args, int mapindex)
{
	xfs_attr_leafblock_t *leaf;
	xfs_attr_leaf_hdr_t *hdr;
	xfs_attr_leaf_entry_t *entry;
	xfs_attr_leaf_name_local_t *name_loc;
	xfs_attr_leaf_name_remote_t *name_rmt;
	xfs_attr_leaf_map_t *map;
	xfs_mount_t *mp;
	int tmp, i;

	leaf = bp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_ATTR_LEAF_MAGIC);
=======
	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	hdr = &leaf->hdr;
	ASSERT((mapindex >= 0) && (mapindex < XFS_ATTR_LEAF_MAPSIZE));
	ASSERT((args->index >= 0) && (args->index <= be16_to_cpu(hdr->count)));
=======
xfs_attr3_leaf_add_work(
	struct xfs_buf		*bp,
	struct xfs_attr3_icleaf_hdr *ichdr,
	struct xfs_da_args	*args,
	int			mapindex)
{
	struct xfs_attr_leafblock *leaf;
	struct xfs_attr_leaf_entry *entry;
	struct xfs_attr_leaf_name_local *name_loc;
	struct xfs_attr_leaf_name_remote *name_rmt;
	struct xfs_mount	*mp;
	int			tmp;
	int			i;

	trace_xfs_attr_leaf_add_work(args);

	leaf = bp->b_addr;
	ASSERT(mapindex >= 0 && mapindex < XFS_ATTR_LEAF_MAPSIZE);
	ASSERT(args->index >= 0 && args->index <= ichdr->count);
>>>>>>> refs/remotes/origin/master

	/*
	 * Force open some space in the entry array and fill it in.
	 */
<<<<<<< HEAD
	entry = &leaf->entries[args->index];
	if (args->index < be16_to_cpu(hdr->count)) {
		tmp  = be16_to_cpu(hdr->count) - args->index;
		tmp *= sizeof(xfs_attr_leaf_entry_t);
		memmove((char *)(entry+1), (char *)entry, tmp);
		xfs_da_log_buf(args->trans, bp,
		    XFS_DA_LOGRANGE(leaf, entry, tmp + sizeof(*entry)));
	}
	be16_add_cpu(&hdr->count, 1);
=======
	entry = &xfs_attr3_leaf_entryp(leaf)[args->index];
	if (args->index < ichdr->count) {
		tmp  = ichdr->count - args->index;
		tmp *= sizeof(xfs_attr_leaf_entry_t);
		memmove(entry + 1, entry, tmp);
		xfs_trans_log_buf(args->trans, bp,
		    XFS_DA_LOGRANGE(leaf, entry, tmp + sizeof(*entry)));
	}
	ichdr->count++;
>>>>>>> refs/remotes/origin/master

	/*
	 * Allocate space for the new string (at the end of the run).
	 */
<<<<<<< HEAD
	map = &hdr->freemap[mapindex];
	mp = args->trans->t_mountp;
	ASSERT(be16_to_cpu(map->base) < XFS_LBSIZE(mp));
	ASSERT((be16_to_cpu(map->base) & 0x3) == 0);
	ASSERT(be16_to_cpu(map->size) >=
		xfs_attr_leaf_newentsize(args->namelen, args->valuelen,
					 mp->m_sb.sb_blocksize, NULL));
	ASSERT(be16_to_cpu(map->size) < XFS_LBSIZE(mp));
	ASSERT((be16_to_cpu(map->size) & 0x3) == 0);
	be16_add_cpu(&map->size,
		-xfs_attr_leaf_newentsize(args->namelen, args->valuelen,
					  mp->m_sb.sb_blocksize, &tmp));
	entry->nameidx = cpu_to_be16(be16_to_cpu(map->base) +
				     be16_to_cpu(map->size));
=======
	mp = args->trans->t_mountp;
	ASSERT(ichdr->freemap[mapindex].base < XFS_LBSIZE(mp));
	ASSERT((ichdr->freemap[mapindex].base & 0x3) == 0);
	ASSERT(ichdr->freemap[mapindex].size >=
		xfs_attr_leaf_newentsize(args->namelen, args->valuelen,
					 mp->m_sb.sb_blocksize, NULL));
	ASSERT(ichdr->freemap[mapindex].size < XFS_LBSIZE(mp));
	ASSERT((ichdr->freemap[mapindex].size & 0x3) == 0);

	ichdr->freemap[mapindex].size -=
			xfs_attr_leaf_newentsize(args->namelen, args->valuelen,
						 mp->m_sb.sb_blocksize, &tmp);

	entry->nameidx = cpu_to_be16(ichdr->freemap[mapindex].base +
				     ichdr->freemap[mapindex].size);
>>>>>>> refs/remotes/origin/master
	entry->hashval = cpu_to_be32(args->hashval);
	entry->flags = tmp ? XFS_ATTR_LOCAL : 0;
	entry->flags |= XFS_ATTR_NSP_ARGS_TO_ONDISK(args->flags);
	if (args->op_flags & XFS_DA_OP_RENAME) {
		entry->flags |= XFS_ATTR_INCOMPLETE;
		if ((args->blkno2 == args->blkno) &&
		    (args->index2 <= args->index)) {
			args->index2++;
		}
	}
<<<<<<< HEAD
	xfs_da_log_buf(args->trans, bp,
			  XFS_DA_LOGRANGE(leaf, entry, sizeof(*entry)));
	ASSERT((args->index == 0) ||
	       (be32_to_cpu(entry->hashval) >= be32_to_cpu((entry-1)->hashval)));
	ASSERT((args->index == be16_to_cpu(hdr->count)-1) ||
	       (be32_to_cpu(entry->hashval) <= be32_to_cpu((entry+1)->hashval)));

	/*
<<<<<<< HEAD
	 * Copy the attribute name and value into the new space.
	 *
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	xfs_trans_log_buf(args->trans, bp,
			  XFS_DA_LOGRANGE(leaf, entry, sizeof(*entry)));
	ASSERT((args->index == 0) ||
	       (be32_to_cpu(entry->hashval) >= be32_to_cpu((entry-1)->hashval)));
	ASSERT((args->index == ichdr->count - 1) ||
	       (be32_to_cpu(entry->hashval) <= be32_to_cpu((entry+1)->hashval)));

	/*
>>>>>>> refs/remotes/origin/master
	 * For "remote" attribute values, simply note that we need to
	 * allocate space for the "remote" value.  We can't actually
	 * allocate the extents in this transaction, and we can't decide
	 * which blocks they should be as we might allocate more blocks
	 * as part of this transaction (a split operation for example).
	 */
	if (entry->flags & XFS_ATTR_LOCAL) {
<<<<<<< HEAD
		name_loc = xfs_attr_leaf_name_local(leaf, args->index);
=======
		name_loc = xfs_attr3_leaf_name_local(leaf, args->index);
>>>>>>> refs/remotes/origin/master
		name_loc->namelen = args->namelen;
		name_loc->valuelen = cpu_to_be16(args->valuelen);
		memcpy((char *)name_loc->nameval, args->name, args->namelen);
		memcpy((char *)&name_loc->nameval[args->namelen], args->value,
				   be16_to_cpu(name_loc->valuelen));
	} else {
<<<<<<< HEAD
		name_rmt = xfs_attr_leaf_name_remote(leaf, args->index);
=======
		name_rmt = xfs_attr3_leaf_name_remote(leaf, args->index);
>>>>>>> refs/remotes/origin/master
		name_rmt->namelen = args->namelen;
		memcpy((char *)name_rmt->name, args->name, args->namelen);
		entry->flags |= XFS_ATTR_INCOMPLETE;
		/* just in case */
		name_rmt->valuelen = 0;
		name_rmt->valueblk = 0;
		args->rmtblkno = 1;
<<<<<<< HEAD
		args->rmtblkcnt = XFS_B_TO_FSB(mp, args->valuelen);
	}
	xfs_da_log_buf(args->trans, bp,
	     XFS_DA_LOGRANGE(leaf, xfs_attr_leaf_name(leaf, args->index),
=======
		args->rmtblkcnt = xfs_attr3_rmt_blocks(mp, args->valuelen);
	}
	xfs_trans_log_buf(args->trans, bp,
	     XFS_DA_LOGRANGE(leaf, xfs_attr3_leaf_name(leaf, args->index),
>>>>>>> refs/remotes/origin/master
				   xfs_attr_leaf_entsize(leaf, args->index)));

	/*
	 * Update the control info for this leaf node
	 */
<<<<<<< HEAD
	if (be16_to_cpu(entry->nameidx) < be16_to_cpu(hdr->firstused)) {
		/* both on-disk, don't endian-flip twice */
		hdr->firstused = entry->nameidx;
	}
	ASSERT(be16_to_cpu(hdr->firstused) >=
	       ((be16_to_cpu(hdr->count) * sizeof(*entry)) + sizeof(*hdr)));
	tmp = (be16_to_cpu(hdr->count)-1) * sizeof(xfs_attr_leaf_entry_t)
					+ sizeof(xfs_attr_leaf_hdr_t);
	map = &hdr->freemap[0];
	for (i = 0; i < XFS_ATTR_LEAF_MAPSIZE; map++, i++) {
		if (be16_to_cpu(map->base) == tmp) {
			be16_add_cpu(&map->base, sizeof(xfs_attr_leaf_entry_t));
			be16_add_cpu(&map->size,
				 -((int)sizeof(xfs_attr_leaf_entry_t)));
		}
	}
	be16_add_cpu(&hdr->usedbytes, xfs_attr_leaf_entsize(leaf, args->index));
	xfs_da_log_buf(args->trans, bp,
		XFS_DA_LOGRANGE(leaf, hdr, sizeof(*hdr)));
	return(0);
=======
	if (be16_to_cpu(entry->nameidx) < ichdr->firstused)
		ichdr->firstused = be16_to_cpu(entry->nameidx);

	ASSERT(ichdr->firstused >= ichdr->count * sizeof(xfs_attr_leaf_entry_t)
					+ xfs_attr3_leaf_hdr_size(leaf));
	tmp = (ichdr->count - 1) * sizeof(xfs_attr_leaf_entry_t)
					+ xfs_attr3_leaf_hdr_size(leaf);

	for (i = 0; i < XFS_ATTR_LEAF_MAPSIZE; i++) {
		if (ichdr->freemap[i].base == tmp) {
			ichdr->freemap[i].base += sizeof(xfs_attr_leaf_entry_t);
			ichdr->freemap[i].size -= sizeof(xfs_attr_leaf_entry_t);
		}
	}
	ichdr->usedbytes += xfs_attr_leaf_entsize(leaf, args->index);
	return 0;
>>>>>>> refs/remotes/origin/master
}

/*
 * Garbage collect a leaf attribute list block by copying it to a new buffer.
 */
STATIC void
<<<<<<< HEAD
xfs_attr_leaf_compact(xfs_trans_t *trans, xfs_dabuf_t *bp)
{
	xfs_attr_leafblock_t *leaf_s, *leaf_d;
	xfs_attr_leaf_hdr_t *hdr_s, *hdr_d;
	xfs_mount_t *mp;
	char *tmpbuffer;

	mp = trans->t_mountp;
	tmpbuffer = kmem_alloc(XFS_LBSIZE(mp), KM_SLEEP);
	ASSERT(tmpbuffer != NULL);
	memcpy(tmpbuffer, bp->data, XFS_LBSIZE(mp));
	memset(bp->data, 0, XFS_LBSIZE(mp));

	/*
	 * Copy basic information
	 */
	leaf_s = (xfs_attr_leafblock_t *)tmpbuffer;
	leaf_d = bp->data;
	hdr_s = &leaf_s->hdr;
	hdr_d = &leaf_d->hdr;
	hdr_d->info = hdr_s->info;	/* struct copy */
	hdr_d->firstused = cpu_to_be16(XFS_LBSIZE(mp));
	/* handle truncation gracefully */
	if (!hdr_d->firstused) {
		hdr_d->firstused = cpu_to_be16(
				XFS_LBSIZE(mp) - XFS_ATTR_LEAF_NAME_ALIGN);
	}
	hdr_d->usedbytes = 0;
	hdr_d->count = 0;
	hdr_d->holes = 0;
	hdr_d->freemap[0].base = cpu_to_be16(sizeof(xfs_attr_leaf_hdr_t));
	hdr_d->freemap[0].size = cpu_to_be16(be16_to_cpu(hdr_d->firstused) -
					     sizeof(xfs_attr_leaf_hdr_t));
=======
xfs_attr3_leaf_compact(
	struct xfs_da_args	*args,
	struct xfs_attr3_icleaf_hdr *ichdr_dst,
	struct xfs_buf		*bp)
{
	struct xfs_attr_leafblock *leaf_src;
	struct xfs_attr_leafblock *leaf_dst;
	struct xfs_attr3_icleaf_hdr ichdr_src;
	struct xfs_trans	*trans = args->trans;
	struct xfs_mount	*mp = trans->t_mountp;
	char			*tmpbuffer;

	trace_xfs_attr_leaf_compact(args);

	tmpbuffer = kmem_alloc(XFS_LBSIZE(mp), KM_SLEEP);
	memcpy(tmpbuffer, bp->b_addr, XFS_LBSIZE(mp));
	memset(bp->b_addr, 0, XFS_LBSIZE(mp));
	leaf_src = (xfs_attr_leafblock_t *)tmpbuffer;
	leaf_dst = bp->b_addr;

	/*
	 * Copy the on-disk header back into the destination buffer to ensure
	 * all the information in the header that is not part of the incore
	 * header structure is preserved.
	 */
	memcpy(bp->b_addr, tmpbuffer, xfs_attr3_leaf_hdr_size(leaf_src));

	/* Initialise the incore headers */
	ichdr_src = *ichdr_dst;	/* struct copy */
	ichdr_dst->firstused = XFS_LBSIZE(mp);
	ichdr_dst->usedbytes = 0;
	ichdr_dst->count = 0;
	ichdr_dst->holes = 0;
	ichdr_dst->freemap[0].base = xfs_attr3_leaf_hdr_size(leaf_src);
	ichdr_dst->freemap[0].size = ichdr_dst->firstused -
						ichdr_dst->freemap[0].base;

	/* write the header back to initialise the underlying buffer */
	xfs_attr3_leaf_hdr_to_disk(leaf_dst, ichdr_dst);
>>>>>>> refs/remotes/origin/master

	/*
	 * Copy all entry's in the same (sorted) order,
	 * but allocate name/value pairs packed and in sequence.
	 */
<<<<<<< HEAD
	xfs_attr_leaf_moveents(leaf_s, 0, leaf_d, 0,
				be16_to_cpu(hdr_s->count), mp);
	xfs_da_log_buf(trans, bp, 0, XFS_LBSIZE(mp) - 1);
=======
	xfs_attr3_leaf_moveents(leaf_src, &ichdr_src, 0, leaf_dst, ichdr_dst, 0,
				ichdr_src.count, mp);
	/*
	 * this logs the entire buffer, but the caller must write the header
	 * back to the buffer when it is finished modifying it.
	 */
	xfs_trans_log_buf(trans, bp, 0, XFS_LBSIZE(mp) - 1);
>>>>>>> refs/remotes/origin/master

	kmem_free(tmpbuffer);
}

/*
<<<<<<< HEAD
=======
 * Compare two leaf blocks "order".
 * Return 0 unless leaf2 should go before leaf1.
 */
static int
xfs_attr3_leaf_order(
	struct xfs_buf	*leaf1_bp,
	struct xfs_attr3_icleaf_hdr *leaf1hdr,
	struct xfs_buf	*leaf2_bp,
	struct xfs_attr3_icleaf_hdr *leaf2hdr)
{
	struct xfs_attr_leaf_entry *entries1;
	struct xfs_attr_leaf_entry *entries2;

	entries1 = xfs_attr3_leaf_entryp(leaf1_bp->b_addr);
	entries2 = xfs_attr3_leaf_entryp(leaf2_bp->b_addr);
	if (leaf1hdr->count > 0 && leaf2hdr->count > 0 &&
	    ((be32_to_cpu(entries2[0].hashval) <
	      be32_to_cpu(entries1[0].hashval)) ||
	     (be32_to_cpu(entries2[leaf2hdr->count - 1].hashval) <
	      be32_to_cpu(entries1[leaf1hdr->count - 1].hashval)))) {
		return 1;
	}
	return 0;
}

int
xfs_attr_leaf_order(
	struct xfs_buf	*leaf1_bp,
	struct xfs_buf	*leaf2_bp)
{
	struct xfs_attr3_icleaf_hdr ichdr1;
	struct xfs_attr3_icleaf_hdr ichdr2;

	xfs_attr3_leaf_hdr_from_disk(&ichdr1, leaf1_bp->b_addr);
	xfs_attr3_leaf_hdr_from_disk(&ichdr2, leaf2_bp->b_addr);
	return xfs_attr3_leaf_order(leaf1_bp, &ichdr1, leaf2_bp, &ichdr2);
}

/*
>>>>>>> refs/remotes/origin/master
 * Redistribute the attribute list entries between two leaf nodes,
 * taking into account the size of the new entry.
 *
 * NOTE: if new block is empty, then it will get the upper half of the
 * old block.  At present, all (one) callers pass in an empty second block.
 *
 * This code adjusts the args->index/blkno and args->index2/blkno2 fields
 * to match what it is doing in splitting the attribute leaf block.  Those
 * values are used in "atomic rename" operations on attributes.  Note that
 * the "new" and "old" values can end up in different blocks.
 */
STATIC void
<<<<<<< HEAD
xfs_attr_leaf_rebalance(xfs_da_state_t *state, xfs_da_state_blk_t *blk1,
				       xfs_da_state_blk_t *blk2)
{
	xfs_da_args_t *args;
	xfs_da_state_blk_t *tmp_blk;
	xfs_attr_leafblock_t *leaf1, *leaf2;
	xfs_attr_leaf_hdr_t *hdr1, *hdr2;
	int count, totallen, max, space, swap;
=======
xfs_attr3_leaf_rebalance(
	struct xfs_da_state	*state,
	struct xfs_da_state_blk	*blk1,
	struct xfs_da_state_blk	*blk2)
{
	struct xfs_da_args	*args;
	struct xfs_attr_leafblock *leaf1;
	struct xfs_attr_leafblock *leaf2;
	struct xfs_attr3_icleaf_hdr ichdr1;
	struct xfs_attr3_icleaf_hdr ichdr2;
	struct xfs_attr_leaf_entry *entries1;
	struct xfs_attr_leaf_entry *entries2;
	int			count;
	int			totallen;
	int			max;
	int			space;
	int			swap;
>>>>>>> refs/remotes/origin/master

	/*
	 * Set up environment.
	 */
	ASSERT(blk1->magic == XFS_ATTR_LEAF_MAGIC);
	ASSERT(blk2->magic == XFS_ATTR_LEAF_MAGIC);
<<<<<<< HEAD
	leaf1 = blk1->bp->data;
	leaf2 = blk2->bp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf1->hdr.info.magic) == XFS_ATTR_LEAF_MAGIC);
	ASSERT(be16_to_cpu(leaf2->hdr.info.magic) == XFS_ATTR_LEAF_MAGIC);
	args = state->args;

=======
	ASSERT(leaf1->hdr.info.magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC));
	ASSERT(leaf2->hdr.info.magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC));
=======
	leaf1 = blk1->bp->b_addr;
	leaf2 = blk2->bp->b_addr;
	xfs_attr3_leaf_hdr_from_disk(&ichdr1, leaf1);
	xfs_attr3_leaf_hdr_from_disk(&ichdr2, leaf2);
	ASSERT(ichdr2.count == 0);
>>>>>>> refs/remotes/origin/master
	args = state->args;

	trace_xfs_attr_leaf_rebalance(args);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Check ordering of blocks, reverse if it makes things simpler.
	 *
	 * NOTE: Given that all (current) callers pass in an empty
	 * second block, this code should never set "swap".
	 */
	swap = 0;
<<<<<<< HEAD
	if (xfs_attr_leaf_order(blk1->bp, blk2->bp)) {
		tmp_blk = blk1;
		blk1 = blk2;
		blk2 = tmp_blk;
		leaf1 = blk1->bp->data;
		leaf2 = blk2->bp->data;
		swap = 1;
	}
	hdr1 = &leaf1->hdr;
	hdr2 = &leaf2->hdr;
=======
	if (xfs_attr3_leaf_order(blk1->bp, &ichdr1, blk2->bp, &ichdr2)) {
		struct xfs_da_state_blk	*tmp_blk;
		struct xfs_attr3_icleaf_hdr tmp_ichdr;

		tmp_blk = blk1;
		blk1 = blk2;
		blk2 = tmp_blk;

		/* struct copies to swap them rather than reconverting */
		tmp_ichdr = ichdr1;
		ichdr1 = ichdr2;
		ichdr2 = tmp_ichdr;

		leaf1 = blk1->bp->b_addr;
		leaf2 = blk2->bp->b_addr;
		swap = 1;
	}
>>>>>>> refs/remotes/origin/master

	/*
	 * Examine entries until we reduce the absolute difference in
	 * byte usage between the two blocks to a minimum.  Then get
	 * the direction to copy and the number of elements to move.
	 *
	 * "inleaf" is true if the new entry should be inserted into blk1.
	 * If "swap" is also true, then reverse the sense of "inleaf".
	 */
<<<<<<< HEAD
	state->inleaf = xfs_attr_leaf_figure_balance(state, blk1, blk2,
							    &count, &totallen);
=======
	state->inleaf = xfs_attr3_leaf_figure_balance(state, blk1, &ichdr1,
						      blk2, &ichdr2,
						      &count, &totallen);
>>>>>>> refs/remotes/origin/master
	if (swap)
		state->inleaf = !state->inleaf;

	/*
	 * Move any entries required from leaf to leaf:
	 */
<<<<<<< HEAD
	if (count < be16_to_cpu(hdr1->count)) {
=======
	if (count < ichdr1.count) {
>>>>>>> refs/remotes/origin/master
		/*
		 * Figure the total bytes to be added to the destination leaf.
		 */
		/* number entries being moved */
<<<<<<< HEAD
		count = be16_to_cpu(hdr1->count) - count;
		space  = be16_to_cpu(hdr1->usedbytes) - totallen;
=======
		count = ichdr1.count - count;
		space  = ichdr1.usedbytes - totallen;
>>>>>>> refs/remotes/origin/master
		space += count * sizeof(xfs_attr_leaf_entry_t);

		/*
		 * leaf2 is the destination, compact it if it looks tight.
		 */
<<<<<<< HEAD
		max  = be16_to_cpu(hdr2->firstused)
						- sizeof(xfs_attr_leaf_hdr_t);
		max -= be16_to_cpu(hdr2->count) * sizeof(xfs_attr_leaf_entry_t);
		if (space > max) {
			xfs_attr_leaf_compact(args->trans, blk2->bp);
		}
=======
		max  = ichdr2.firstused - xfs_attr3_leaf_hdr_size(leaf1);
		max -= ichdr2.count * sizeof(xfs_attr_leaf_entry_t);
		if (space > max)
			xfs_attr3_leaf_compact(args, &ichdr2, blk2->bp);
>>>>>>> refs/remotes/origin/master

		/*
		 * Move high entries from leaf1 to low end of leaf2.
		 */
<<<<<<< HEAD
		xfs_attr_leaf_moveents(leaf1, be16_to_cpu(hdr1->count) - count,
				leaf2, 0, count, state->mp);

		xfs_da_log_buf(args->trans, blk1->bp, 0, state->blocksize-1);
		xfs_da_log_buf(args->trans, blk2->bp, 0, state->blocksize-1);
	} else if (count > be16_to_cpu(hdr1->count)) {
=======
		xfs_attr3_leaf_moveents(leaf1, &ichdr1, ichdr1.count - count,
				leaf2, &ichdr2, 0, count, state->mp);

	} else if (count > ichdr1.count) {
>>>>>>> refs/remotes/origin/master
		/*
		 * I assert that since all callers pass in an empty
		 * second buffer, this code should never execute.
		 */
<<<<<<< HEAD
=======
		ASSERT(0);
>>>>>>> refs/remotes/origin/master

		/*
		 * Figure the total bytes to be added to the destination leaf.
		 */
		/* number entries being moved */
<<<<<<< HEAD
		count -= be16_to_cpu(hdr1->count);
		space  = totallen - be16_to_cpu(hdr1->usedbytes);
=======
		count -= ichdr1.count;
		space  = totallen - ichdr1.usedbytes;
>>>>>>> refs/remotes/origin/master
		space += count * sizeof(xfs_attr_leaf_entry_t);

		/*
		 * leaf1 is the destination, compact it if it looks tight.
		 */
<<<<<<< HEAD
		max  = be16_to_cpu(hdr1->firstused)
						- sizeof(xfs_attr_leaf_hdr_t);
		max -= be16_to_cpu(hdr1->count) * sizeof(xfs_attr_leaf_entry_t);
		if (space > max) {
			xfs_attr_leaf_compact(args->trans, blk1->bp);
		}
=======
		max  = ichdr1.firstused - xfs_attr3_leaf_hdr_size(leaf1);
		max -= ichdr1.count * sizeof(xfs_attr_leaf_entry_t);
		if (space > max)
			xfs_attr3_leaf_compact(args, &ichdr1, blk1->bp);
>>>>>>> refs/remotes/origin/master

		/*
		 * Move low entries from leaf2 to high end of leaf1.
		 */
<<<<<<< HEAD
		xfs_attr_leaf_moveents(leaf2, 0, leaf1,
				be16_to_cpu(hdr1->count), count, state->mp);

		xfs_da_log_buf(args->trans, blk1->bp, 0, state->blocksize-1);
		xfs_da_log_buf(args->trans, blk2->bp, 0, state->blocksize-1);
	}

	/*
	 * Copy out last hashval in each block for B-tree code.
	 */
	blk1->hashval = be32_to_cpu(
		leaf1->entries[be16_to_cpu(leaf1->hdr.count)-1].hashval);
	blk2->hashval = be32_to_cpu(
		leaf2->entries[be16_to_cpu(leaf2->hdr.count)-1].hashval);
=======
		xfs_attr3_leaf_moveents(leaf2, &ichdr2, 0, leaf1, &ichdr1,
					ichdr1.count, count, state->mp);
	}

	xfs_attr3_leaf_hdr_to_disk(leaf1, &ichdr1);
	xfs_attr3_leaf_hdr_to_disk(leaf2, &ichdr2);
	xfs_trans_log_buf(args->trans, blk1->bp, 0, state->blocksize-1);
	xfs_trans_log_buf(args->trans, blk2->bp, 0, state->blocksize-1);

	/*
	 * Copy out last hashval in each block for B-tree code.
	 */
	entries1 = xfs_attr3_leaf_entryp(leaf1);
	entries2 = xfs_attr3_leaf_entryp(leaf2);
	blk1->hashval = be32_to_cpu(entries1[ichdr1.count - 1].hashval);
	blk2->hashval = be32_to_cpu(entries2[ichdr2.count - 1].hashval);
>>>>>>> refs/remotes/origin/master

	/*
	 * Adjust the expected index for insertion.
	 * NOTE: this code depends on the (current) situation that the
	 * second block was originally empty.
	 *
	 * If the insertion point moved to the 2nd block, we must adjust
	 * the index.  We must also track the entry just following the
	 * new entry for use in an "atomic rename" operation, that entry
	 * is always the "old" entry and the "new" entry is what we are
	 * inserting.  The index/blkno fields refer to the "old" entry,
	 * while the index2/blkno2 fields refer to the "new" entry.
	 */
<<<<<<< HEAD
	if (blk1->index > be16_to_cpu(leaf1->hdr.count)) {
		ASSERT(state->inleaf == 0);
		blk2->index = blk1->index - be16_to_cpu(leaf1->hdr.count);
		args->index = args->index2 = blk2->index;
		args->blkno = args->blkno2 = blk2->blkno;
	} else if (blk1->index == be16_to_cpu(leaf1->hdr.count)) {
=======
	if (blk1->index > ichdr1.count) {
		ASSERT(state->inleaf == 0);
		blk2->index = blk1->index - ichdr1.count;
		args->index = args->index2 = blk2->index;
		args->blkno = args->blkno2 = blk2->blkno;
	} else if (blk1->index == ichdr1.count) {
>>>>>>> refs/remotes/origin/master
		if (state->inleaf) {
			args->index = blk1->index;
			args->blkno = blk1->blkno;
			args->index2 = 0;
			args->blkno2 = blk2->blkno;
		} else {
<<<<<<< HEAD
			blk2->index = blk1->index
				    - be16_to_cpu(leaf1->hdr.count);
			args->index = args->index2 = blk2->index;
			args->blkno = args->blkno2 = blk2->blkno;
=======
			/*
			 * On a double leaf split, the original attr location
			 * is already stored in blkno2/index2, so don't
			 * overwrite it overwise we corrupt the tree.
			 */
			blk2->index = blk1->index - ichdr1.count;
			args->index = blk2->index;
			args->blkno = blk2->blkno;
			if (!state->extravalid) {
				/*
				 * set the new attr location to match the old
				 * one and let the higher level split code
				 * decide where in the leaf to place it.
				 */
				args->index2 = blk2->index;
				args->blkno2 = blk2->blkno;
			}
>>>>>>> refs/remotes/origin/master
		}
	} else {
		ASSERT(state->inleaf == 1);
		args->index = args->index2 = blk1->index;
		args->blkno = args->blkno2 = blk1->blkno;
	}
}

/*
 * Examine entries until we reduce the absolute difference in
 * byte usage between the two blocks to a minimum.
 * GROT: Is this really necessary?  With other than a 512 byte blocksize,
 * GROT: there will always be enough room in either block for a new entry.
 * GROT: Do a double-split for this case?
 */
STATIC int
<<<<<<< HEAD
xfs_attr_leaf_figure_balance(xfs_da_state_t *state,
				    xfs_da_state_blk_t *blk1,
				    xfs_da_state_blk_t *blk2,
				    int *countarg, int *usedbytesarg)
{
	xfs_attr_leafblock_t *leaf1, *leaf2;
	xfs_attr_leaf_hdr_t *hdr1, *hdr2;
	xfs_attr_leaf_entry_t *entry;
	int count, max, index, totallen, half;
	int lastdelta, foundit, tmp;

	/*
	 * Set up environment.
	 */
	leaf1 = blk1->bp->data;
	leaf2 = blk2->bp->data;
	hdr1 = &leaf1->hdr;
	hdr2 = &leaf2->hdr;
	foundit = 0;
	totallen = 0;
=======
xfs_attr3_leaf_figure_balance(
	struct xfs_da_state		*state,
	struct xfs_da_state_blk		*blk1,
	struct xfs_attr3_icleaf_hdr	*ichdr1,
	struct xfs_da_state_blk		*blk2,
	struct xfs_attr3_icleaf_hdr	*ichdr2,
	int				*countarg,
	int				*usedbytesarg)
{
	struct xfs_attr_leafblock	*leaf1 = blk1->bp->b_addr;
	struct xfs_attr_leafblock	*leaf2 = blk2->bp->b_addr;
	struct xfs_attr_leaf_entry	*entry;
	int				count;
	int				max;
	int				index;
	int				totallen = 0;
	int				half;
	int				lastdelta;
	int				foundit = 0;
	int				tmp;
>>>>>>> refs/remotes/origin/master

	/*
	 * Examine entries until we reduce the absolute difference in
	 * byte usage between the two blocks to a minimum.
	 */
<<<<<<< HEAD
	max = be16_to_cpu(hdr1->count) + be16_to_cpu(hdr2->count);
	half  = (max+1) * sizeof(*entry);
	half += be16_to_cpu(hdr1->usedbytes) +
		be16_to_cpu(hdr2->usedbytes) +
		xfs_attr_leaf_newentsize(
				state->args->namelen,
				state->args->valuelen,
				state->blocksize, NULL);
	half /= 2;
	lastdelta = state->blocksize;
	entry = &leaf1->entries[0];
=======
	max = ichdr1->count + ichdr2->count;
	half = (max + 1) * sizeof(*entry);
	half += ichdr1->usedbytes + ichdr2->usedbytes +
			xfs_attr_leaf_newentsize(state->args->namelen,
						 state->args->valuelen,
						 state->blocksize, NULL);
	half /= 2;
	lastdelta = state->blocksize;
	entry = xfs_attr3_leaf_entryp(leaf1);
>>>>>>> refs/remotes/origin/master
	for (count = index = 0; count < max; entry++, index++, count++) {

#define XFS_ATTR_ABS(A)	(((A) < 0) ? -(A) : (A))
		/*
		 * The new entry is in the first block, account for it.
		 */
		if (count == blk1->index) {
			tmp = totallen + sizeof(*entry) +
				xfs_attr_leaf_newentsize(
						state->args->namelen,
						state->args->valuelen,
						state->blocksize, NULL);
			if (XFS_ATTR_ABS(half - tmp) > lastdelta)
				break;
			lastdelta = XFS_ATTR_ABS(half - tmp);
			totallen = tmp;
			foundit = 1;
		}

		/*
		 * Wrap around into the second block if necessary.
		 */
<<<<<<< HEAD
		if (count == be16_to_cpu(hdr1->count)) {
			leaf1 = leaf2;
			entry = &leaf1->entries[0];
=======
		if (count == ichdr1->count) {
			leaf1 = leaf2;
			entry = xfs_attr3_leaf_entryp(leaf1);
>>>>>>> refs/remotes/origin/master
			index = 0;
		}

		/*
		 * Figure out if next leaf entry would be too much.
		 */
		tmp = totallen + sizeof(*entry) + xfs_attr_leaf_entsize(leaf1,
									index);
		if (XFS_ATTR_ABS(half - tmp) > lastdelta)
			break;
		lastdelta = XFS_ATTR_ABS(half - tmp);
		totallen = tmp;
#undef XFS_ATTR_ABS
	}

	/*
	 * Calculate the number of usedbytes that will end up in lower block.
	 * If new entry not in lower block, fix up the count.
	 */
	totallen -= count * sizeof(*entry);
	if (foundit) {
		totallen -= sizeof(*entry) +
				xfs_attr_leaf_newentsize(
						state->args->namelen,
						state->args->valuelen,
						state->blocksize, NULL);
	}

	*countarg = count;
	*usedbytesarg = totallen;
<<<<<<< HEAD
	return(foundit);
=======
	return foundit;
>>>>>>> refs/remotes/origin/master
}

/*========================================================================
 * Routines used for shrinking the Btree.
 *========================================================================*/

/*
 * Check a leaf block and its neighbors to see if the block should be
 * collapsed into one or the other neighbor.  Always keep the block
 * with the smaller block number.
 * If the current block is over 50% full, don't try to join it, return 0.
 * If the block is empty, fill in the state structure and return 2.
 * If it can be collapsed, fill in the state structure and return 1.
 * If nothing can be done, return 0.
 *
 * GROT: allow for INCOMPLETE entries in calculation.
 */
int
<<<<<<< HEAD
xfs_attr_leaf_toosmall(xfs_da_state_t *state, int *action)
{
	xfs_attr_leafblock_t *leaf;
	xfs_da_state_blk_t *blk;
	xfs_da_blkinfo_t *info;
	int count, bytes, forward, error, retval, i;
	xfs_dablk_t blkno;
	xfs_dabuf_t *bp;
=======
xfs_attr3_leaf_toosmall(
	struct xfs_da_state	*state,
	int			*action)
{
	struct xfs_attr_leafblock *leaf;
	struct xfs_da_state_blk	*blk;
	struct xfs_attr3_icleaf_hdr ichdr;
	struct xfs_buf		*bp;
	xfs_dablk_t		blkno;
	int			bytes;
	int			forward;
	int			error;
	int			retval;
	int			i;

	trace_xfs_attr_leaf_toosmall(state->args);
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
	ASSERT(be16_to_cpu(info->magic) == XFS_ATTR_LEAF_MAGIC);
=======
	ASSERT(info->magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	leaf = (xfs_attr_leafblock_t *)info;
	count = be16_to_cpu(leaf->hdr.count);
	bytes = sizeof(xfs_attr_leaf_hdr_t) +
		count * sizeof(xfs_attr_leaf_entry_t) +
		be16_to_cpu(leaf->hdr.usedbytes);
=======
	leaf = blk->bp->b_addr;
	xfs_attr3_leaf_hdr_from_disk(&ichdr, leaf);
	bytes = xfs_attr3_leaf_hdr_size(leaf) +
		ichdr.count * sizeof(xfs_attr_leaf_entry_t) +
		ichdr.usedbytes;
>>>>>>> refs/remotes/origin/master
	if (bytes > (state->blocksize >> 1)) {
		*action = 0;	/* blk over 50%, don't try to join */
		return(0);
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
	if (ichdr.count == 0) {
>>>>>>> refs/remotes/origin/master
		/*
		 * Make altpath point to the block we want to keep and
		 * path point to the block we want to drop (this one).
		 */
<<<<<<< HEAD
		forward = (info->forw != 0);
		memcpy(&state->altpath, &state->path, sizeof(state->path));
		error = xfs_da_path_shift(state, &state->altpath, forward,
=======
		forward = (ichdr.forw != 0);
		memcpy(&state->altpath, &state->path, sizeof(state->path));
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
<<<<<<< HEAD
		return(0);
=======
		return 0;
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * Examine each sibling block to see if we can coalesce with
	 * at least 25% free space to spare.  We need to figure out
	 * whether to merge with the forward or the backward block.
	 * We prefer coalescing with the lower numbered sibling so as
	 * to shrink an attribute list over time.
	 */
	/* start with smaller blk num */
<<<<<<< HEAD
	forward = (be32_to_cpu(info->forw) < be32_to_cpu(info->back));
	for (i = 0; i < 2; forward = !forward, i++) {
		if (forward)
			blkno = be32_to_cpu(info->forw);
		else
			blkno = be32_to_cpu(info->back);
		if (blkno == 0)
			continue;
		error = xfs_da_read_buf(state->args->trans, state->args->dp,
					blkno, -1, &bp, XFS_ATTR_FORK);
		if (error)
			return(error);
		ASSERT(bp != NULL);

		leaf = (xfs_attr_leafblock_t *)info;
		count  = be16_to_cpu(leaf->hdr.count);
		bytes  = state->blocksize - (state->blocksize>>2);
		bytes -= be16_to_cpu(leaf->hdr.usedbytes);
		leaf = bp->data;
<<<<<<< HEAD
		ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_ATTR_LEAF_MAGIC);
=======
		ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
		count += be16_to_cpu(leaf->hdr.count);
		bytes -= be16_to_cpu(leaf->hdr.usedbytes);
		bytes -= count * sizeof(xfs_attr_leaf_entry_t);
		bytes -= sizeof(xfs_attr_leaf_hdr_t);
		xfs_da_brelse(state->args->trans, bp);
=======
	forward = ichdr.forw < ichdr.back;
	for (i = 0; i < 2; forward = !forward, i++) {
		struct xfs_attr3_icleaf_hdr ichdr2;
		if (forward)
			blkno = ichdr.forw;
		else
			blkno = ichdr.back;
		if (blkno == 0)
			continue;
		error = xfs_attr3_leaf_read(state->args->trans, state->args->dp,
					blkno, -1, &bp);
		if (error)
			return(error);

		xfs_attr3_leaf_hdr_from_disk(&ichdr2, bp->b_addr);

		bytes = state->blocksize - (state->blocksize >> 2) -
			ichdr.usedbytes - ichdr2.usedbytes -
			((ichdr.count + ichdr2.count) *
					sizeof(xfs_attr_leaf_entry_t)) -
			xfs_attr3_leaf_hdr_size(leaf);

		xfs_trans_brelse(state->args->trans, bp);
>>>>>>> refs/remotes/origin/master
		if (bytes >= 0)
			break;	/* fits with at least 25% to spare */
	}
	if (i >= 2) {
		*action = 0;
		return(0);
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
	} else {
		error = xfs_da_path_shift(state, &state->path, forward,
=======
		error = xfs_da3_path_shift(state, &state->altpath, forward,
						 0, &retval);
	} else {
		error = xfs_da3_path_shift(state, &state->path, forward,
>>>>>>> refs/remotes/origin/master
						 0, &retval);
	}
	if (error)
		return(error);
	if (retval) {
		*action = 0;
	} else {
		*action = 1;
	}
	return(0);
}

/*
 * Remove a name from the leaf attribute list structure.
 *
 * Return 1 if leaf is less than 37% full, 0 if >= 37% full.
 * If two leaves are 37% full, when combined they will leave 25% free.
 */
int
<<<<<<< HEAD
xfs_attr_leaf_remove(xfs_dabuf_t *bp, xfs_da_args_t *args)
{
	xfs_attr_leafblock_t *leaf;
	xfs_attr_leaf_hdr_t *hdr;
	xfs_attr_leaf_map_t *map;
	xfs_attr_leaf_entry_t *entry;
	int before, after, smallest, entsize;
	int tablesize, tmp, i;
	xfs_mount_t *mp;

	leaf = bp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_ATTR_LEAF_MAGIC);
=======
	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	hdr = &leaf->hdr;
	mp = args->trans->t_mountp;
	ASSERT((be16_to_cpu(hdr->count) > 0)
		&& (be16_to_cpu(hdr->count) < (XFS_LBSIZE(mp)/8)));
	ASSERT((args->index >= 0)
		&& (args->index < be16_to_cpu(hdr->count)));
	ASSERT(be16_to_cpu(hdr->firstused) >=
	       ((be16_to_cpu(hdr->count) * sizeof(*entry)) + sizeof(*hdr)));
	entry = &leaf->entries[args->index];
	ASSERT(be16_to_cpu(entry->nameidx) >= be16_to_cpu(hdr->firstused));
=======
xfs_attr3_leaf_remove(
	struct xfs_buf		*bp,
	struct xfs_da_args	*args)
{
	struct xfs_attr_leafblock *leaf;
	struct xfs_attr3_icleaf_hdr ichdr;
	struct xfs_attr_leaf_entry *entry;
	struct xfs_mount	*mp = args->trans->t_mountp;
	int			before;
	int			after;
	int			smallest;
	int			entsize;
	int			tablesize;
	int			tmp;
	int			i;

	trace_xfs_attr_leaf_remove(args);

	leaf = bp->b_addr;
	xfs_attr3_leaf_hdr_from_disk(&ichdr, leaf);

	ASSERT(ichdr.count > 0 && ichdr.count < XFS_LBSIZE(mp) / 8);
	ASSERT(args->index >= 0 && args->index < ichdr.count);
	ASSERT(ichdr.firstused >= ichdr.count * sizeof(*entry) +
					xfs_attr3_leaf_hdr_size(leaf));

	entry = &xfs_attr3_leaf_entryp(leaf)[args->index];

	ASSERT(be16_to_cpu(entry->nameidx) >= ichdr.firstused);
>>>>>>> refs/remotes/origin/master
	ASSERT(be16_to_cpu(entry->nameidx) < XFS_LBSIZE(mp));

	/*
	 * Scan through free region table:
	 *    check for adjacency of free'd entry with an existing one,
	 *    find smallest free region in case we need to replace it,
	 *    adjust any map that borders the entry table,
	 */
<<<<<<< HEAD
	tablesize = be16_to_cpu(hdr->count) * sizeof(xfs_attr_leaf_entry_t)
					+ sizeof(xfs_attr_leaf_hdr_t);
	map = &hdr->freemap[0];
	tmp = be16_to_cpu(map->size);
	before = after = -1;
	smallest = XFS_ATTR_LEAF_MAPSIZE - 1;
	entsize = xfs_attr_leaf_entsize(leaf, args->index);
	for (i = 0; i < XFS_ATTR_LEAF_MAPSIZE; map++, i++) {
		ASSERT(be16_to_cpu(map->base) < XFS_LBSIZE(mp));
		ASSERT(be16_to_cpu(map->size) < XFS_LBSIZE(mp));
		if (be16_to_cpu(map->base) == tablesize) {
			be16_add_cpu(&map->base,
				 -((int)sizeof(xfs_attr_leaf_entry_t)));
			be16_add_cpu(&map->size, sizeof(xfs_attr_leaf_entry_t));
		}

		if ((be16_to_cpu(map->base) + be16_to_cpu(map->size))
				== be16_to_cpu(entry->nameidx)) {
			before = i;
		} else if (be16_to_cpu(map->base)
			== (be16_to_cpu(entry->nameidx) + entsize)) {
			after = i;
		} else if (be16_to_cpu(map->size) < tmp) {
			tmp = be16_to_cpu(map->size);
=======
	tablesize = ichdr.count * sizeof(xfs_attr_leaf_entry_t)
					+ xfs_attr3_leaf_hdr_size(leaf);
	tmp = ichdr.freemap[0].size;
	before = after = -1;
	smallest = XFS_ATTR_LEAF_MAPSIZE - 1;
	entsize = xfs_attr_leaf_entsize(leaf, args->index);
	for (i = 0; i < XFS_ATTR_LEAF_MAPSIZE; i++) {
		ASSERT(ichdr.freemap[i].base < XFS_LBSIZE(mp));
		ASSERT(ichdr.freemap[i].size < XFS_LBSIZE(mp));
		if (ichdr.freemap[i].base == tablesize) {
			ichdr.freemap[i].base -= sizeof(xfs_attr_leaf_entry_t);
			ichdr.freemap[i].size += sizeof(xfs_attr_leaf_entry_t);
		}

		if (ichdr.freemap[i].base + ichdr.freemap[i].size ==
				be16_to_cpu(entry->nameidx)) {
			before = i;
		} else if (ichdr.freemap[i].base ==
				(be16_to_cpu(entry->nameidx) + entsize)) {
			after = i;
		} else if (ichdr.freemap[i].size < tmp) {
			tmp = ichdr.freemap[i].size;
>>>>>>> refs/remotes/origin/master
			smallest = i;
		}
	}

	/*
	 * Coalesce adjacent freemap regions,
	 * or replace the smallest region.
	 */
	if ((before >= 0) || (after >= 0)) {
		if ((before >= 0) && (after >= 0)) {
<<<<<<< HEAD
			map = &hdr->freemap[before];
			be16_add_cpu(&map->size, entsize);
			be16_add_cpu(&map->size,
				 be16_to_cpu(hdr->freemap[after].size));
			hdr->freemap[after].base = 0;
			hdr->freemap[after].size = 0;
		} else if (before >= 0) {
			map = &hdr->freemap[before];
			be16_add_cpu(&map->size, entsize);
		} else {
			map = &hdr->freemap[after];
			/* both on-disk, don't endian flip twice */
			map->base = entry->nameidx;
			be16_add_cpu(&map->size, entsize);
=======
			ichdr.freemap[before].size += entsize;
			ichdr.freemap[before].size += ichdr.freemap[after].size;
			ichdr.freemap[after].base = 0;
			ichdr.freemap[after].size = 0;
		} else if (before >= 0) {
			ichdr.freemap[before].size += entsize;
		} else {
			ichdr.freemap[after].base = be16_to_cpu(entry->nameidx);
			ichdr.freemap[after].size += entsize;
>>>>>>> refs/remotes/origin/master
		}
	} else {
		/*
		 * Replace smallest region (if it is smaller than free'd entry)
		 */
<<<<<<< HEAD
		map = &hdr->freemap[smallest];
		if (be16_to_cpu(map->size) < entsize) {
			map->base = cpu_to_be16(be16_to_cpu(entry->nameidx));
			map->size = cpu_to_be16(entsize);
=======
		if (ichdr.freemap[smallest].size < entsize) {
			ichdr.freemap[smallest].base = be16_to_cpu(entry->nameidx);
			ichdr.freemap[smallest].size = entsize;
>>>>>>> refs/remotes/origin/master
		}
	}

	/*
	 * Did we remove the first entry?
	 */
<<<<<<< HEAD
	if (be16_to_cpu(entry->nameidx) == be16_to_cpu(hdr->firstused))
=======
	if (be16_to_cpu(entry->nameidx) == ichdr.firstused)
>>>>>>> refs/remotes/origin/master
		smallest = 1;
	else
		smallest = 0;

	/*
	 * Compress the remaining entries and zero out the removed stuff.
	 */
<<<<<<< HEAD
	memset(xfs_attr_leaf_name(leaf, args->index), 0, entsize);
	be16_add_cpu(&hdr->usedbytes, -entsize);
	xfs_da_log_buf(args->trans, bp,
	     XFS_DA_LOGRANGE(leaf, xfs_attr_leaf_name(leaf, args->index),
				   entsize));

	tmp = (be16_to_cpu(hdr->count) - args->index)
					* sizeof(xfs_attr_leaf_entry_t);
	memmove((char *)entry, (char *)(entry+1), tmp);
	be16_add_cpu(&hdr->count, -1);
	xfs_da_log_buf(args->trans, bp,
	    XFS_DA_LOGRANGE(leaf, entry, tmp + sizeof(*entry)));
	entry = &leaf->entries[be16_to_cpu(hdr->count)];
	memset((char *)entry, 0, sizeof(xfs_attr_leaf_entry_t));
=======
	memset(xfs_attr3_leaf_name(leaf, args->index), 0, entsize);
	ichdr.usedbytes -= entsize;
	xfs_trans_log_buf(args->trans, bp,
	     XFS_DA_LOGRANGE(leaf, xfs_attr3_leaf_name(leaf, args->index),
				   entsize));

	tmp = (ichdr.count - args->index) * sizeof(xfs_attr_leaf_entry_t);
	memmove(entry, entry + 1, tmp);
	ichdr.count--;
	xfs_trans_log_buf(args->trans, bp,
	    XFS_DA_LOGRANGE(leaf, entry, tmp + sizeof(xfs_attr_leaf_entry_t)));

	entry = &xfs_attr3_leaf_entryp(leaf)[ichdr.count];
	memset(entry, 0, sizeof(xfs_attr_leaf_entry_t));
>>>>>>> refs/remotes/origin/master

	/*
	 * If we removed the first entry, re-find the first used byte
	 * in the name area.  Note that if the entry was the "firstused",
	 * then we don't have a "hole" in our block resulting from
	 * removing the name.
	 */
	if (smallest) {
		tmp = XFS_LBSIZE(mp);
<<<<<<< HEAD
		entry = &leaf->entries[0];
		for (i = be16_to_cpu(hdr->count)-1; i >= 0; entry++, i--) {
			ASSERT(be16_to_cpu(entry->nameidx) >=
			       be16_to_cpu(hdr->firstused));
=======
		entry = xfs_attr3_leaf_entryp(leaf);
		for (i = ichdr.count - 1; i >= 0; entry++, i--) {
			ASSERT(be16_to_cpu(entry->nameidx) >= ichdr.firstused);
>>>>>>> refs/remotes/origin/master
			ASSERT(be16_to_cpu(entry->nameidx) < XFS_LBSIZE(mp));

			if (be16_to_cpu(entry->nameidx) < tmp)
				tmp = be16_to_cpu(entry->nameidx);
		}
<<<<<<< HEAD
		hdr->firstused = cpu_to_be16(tmp);
		if (!hdr->firstused) {
			hdr->firstused = cpu_to_be16(
					tmp - XFS_ATTR_LEAF_NAME_ALIGN);
		}
	} else {
		hdr->holes = 1;		/* mark as needing compaction */
	}
	xfs_da_log_buf(args->trans, bp,
			  XFS_DA_LOGRANGE(leaf, hdr, sizeof(*hdr)));
=======
		ichdr.firstused = tmp;
		if (!ichdr.firstused)
			ichdr.firstused = tmp - XFS_ATTR_LEAF_NAME_ALIGN;
	} else {
		ichdr.holes = 1;	/* mark as needing compaction */
	}
	xfs_attr3_leaf_hdr_to_disk(leaf, &ichdr);
	xfs_trans_log_buf(args->trans, bp,
			  XFS_DA_LOGRANGE(leaf, &leaf->hdr,
					  xfs_attr3_leaf_hdr_size(leaf)));
>>>>>>> refs/remotes/origin/master

	/*
	 * Check if leaf is less than 50% full, caller may want to
	 * "join" the leaf with a sibling if so.
	 */
<<<<<<< HEAD
	tmp  = sizeof(xfs_attr_leaf_hdr_t);
	tmp += be16_to_cpu(leaf->hdr.count) * sizeof(xfs_attr_leaf_entry_t);
	tmp += be16_to_cpu(leaf->hdr.usedbytes);
	return(tmp < mp->m_attr_magicpct); /* leaf is < 37% full */
=======
	tmp = ichdr.usedbytes + xfs_attr3_leaf_hdr_size(leaf) +
	      ichdr.count * sizeof(xfs_attr_leaf_entry_t);

	return tmp < mp->m_attr_magicpct; /* leaf is < 37% full */
>>>>>>> refs/remotes/origin/master
}

/*
 * Move all the attribute list entries from drop_leaf into save_leaf.
 */
void
<<<<<<< HEAD
xfs_attr_leaf_unbalance(xfs_da_state_t *state, xfs_da_state_blk_t *drop_blk,
				       xfs_da_state_blk_t *save_blk)
{
	xfs_attr_leafblock_t *drop_leaf, *save_leaf, *tmp_leaf;
	xfs_attr_leaf_hdr_t *drop_hdr, *save_hdr, *tmp_hdr;
	xfs_mount_t *mp;
	char *tmpbuffer;

<<<<<<< HEAD
=======
	trace_xfs_attr_leaf_unbalance(state->args);

>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * Set up environment.
	 */
	mp = state->mp;
	ASSERT(drop_blk->magic == XFS_ATTR_LEAF_MAGIC);
	ASSERT(save_blk->magic == XFS_ATTR_LEAF_MAGIC);
	drop_leaf = drop_blk->bp->data;
	save_leaf = save_blk->bp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(drop_leaf->hdr.info.magic) == XFS_ATTR_LEAF_MAGIC);
	ASSERT(be16_to_cpu(save_leaf->hdr.info.magic) == XFS_ATTR_LEAF_MAGIC);
=======
	ASSERT(drop_leaf->hdr.info.magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC));
	ASSERT(save_leaf->hdr.info.magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	drop_hdr = &drop_leaf->hdr;
	save_hdr = &save_leaf->hdr;
=======
xfs_attr3_leaf_unbalance(
	struct xfs_da_state	*state,
	struct xfs_da_state_blk	*drop_blk,
	struct xfs_da_state_blk	*save_blk)
{
	struct xfs_attr_leafblock *drop_leaf = drop_blk->bp->b_addr;
	struct xfs_attr_leafblock *save_leaf = save_blk->bp->b_addr;
	struct xfs_attr3_icleaf_hdr drophdr;
	struct xfs_attr3_icleaf_hdr savehdr;
	struct xfs_attr_leaf_entry *entry;
	struct xfs_mount	*mp = state->mp;

	trace_xfs_attr_leaf_unbalance(state->args);

	drop_leaf = drop_blk->bp->b_addr;
	save_leaf = save_blk->bp->b_addr;
	xfs_attr3_leaf_hdr_from_disk(&drophdr, drop_leaf);
	xfs_attr3_leaf_hdr_from_disk(&savehdr, save_leaf);
	entry = xfs_attr3_leaf_entryp(drop_leaf);
>>>>>>> refs/remotes/origin/master

	/*
	 * Save last hashval from dying block for later Btree fixup.
	 */
<<<<<<< HEAD
	drop_blk->hashval = be32_to_cpu(
		drop_leaf->entries[be16_to_cpu(drop_leaf->hdr.count)-1].hashval);
=======
	drop_blk->hashval = be32_to_cpu(entry[drophdr.count - 1].hashval);
>>>>>>> refs/remotes/origin/master

	/*
	 * Check if we need a temp buffer, or can we do it in place.
	 * Note that we don't check "leaf" for holes because we will
	 * always be dropping it, toosmall() decided that for us already.
	 */
<<<<<<< HEAD
	if (save_hdr->holes == 0) {
=======
	if (savehdr.holes == 0) {
>>>>>>> refs/remotes/origin/master
		/*
		 * dest leaf has no holes, so we add there.  May need
		 * to make some room in the entry array.
		 */
<<<<<<< HEAD
		if (xfs_attr_leaf_order(save_blk->bp, drop_blk->bp)) {
			xfs_attr_leaf_moveents(drop_leaf, 0, save_leaf, 0,
			     be16_to_cpu(drop_hdr->count), mp);
		} else {
			xfs_attr_leaf_moveents(drop_leaf, 0, save_leaf,
				  be16_to_cpu(save_hdr->count),
				  be16_to_cpu(drop_hdr->count), mp);
=======
		if (xfs_attr3_leaf_order(save_blk->bp, &savehdr,
					 drop_blk->bp, &drophdr)) {
			xfs_attr3_leaf_moveents(drop_leaf, &drophdr, 0,
						save_leaf, &savehdr, 0,
						drophdr.count, mp);
		} else {
			xfs_attr3_leaf_moveents(drop_leaf, &drophdr, 0,
						save_leaf, &savehdr,
						savehdr.count, drophdr.count, mp);
>>>>>>> refs/remotes/origin/master
		}
	} else {
		/*
		 * Destination has holes, so we make a temporary copy
		 * of the leaf and add them both to that.
		 */
<<<<<<< HEAD
		tmpbuffer = kmem_alloc(state->blocksize, KM_SLEEP);
		ASSERT(tmpbuffer != NULL);
		memset(tmpbuffer, 0, state->blocksize);
		tmp_leaf = (xfs_attr_leafblock_t *)tmpbuffer;
		tmp_hdr = &tmp_leaf->hdr;
		tmp_hdr->info = save_hdr->info;	/* struct copy */
		tmp_hdr->count = 0;
		tmp_hdr->firstused = cpu_to_be16(state->blocksize);
		if (!tmp_hdr->firstused) {
			tmp_hdr->firstused = cpu_to_be16(
				state->blocksize - XFS_ATTR_LEAF_NAME_ALIGN);
		}
		tmp_hdr->usedbytes = 0;
		if (xfs_attr_leaf_order(save_blk->bp, drop_blk->bp)) {
			xfs_attr_leaf_moveents(drop_leaf, 0, tmp_leaf, 0,
				be16_to_cpu(drop_hdr->count), mp);
			xfs_attr_leaf_moveents(save_leaf, 0, tmp_leaf,
				  be16_to_cpu(tmp_leaf->hdr.count),
				  be16_to_cpu(save_hdr->count), mp);
		} else {
			xfs_attr_leaf_moveents(save_leaf, 0, tmp_leaf, 0,
				be16_to_cpu(save_hdr->count), mp);
			xfs_attr_leaf_moveents(drop_leaf, 0, tmp_leaf,
				be16_to_cpu(tmp_leaf->hdr.count),
				be16_to_cpu(drop_hdr->count), mp);
		}
		memcpy((char *)save_leaf, (char *)tmp_leaf, state->blocksize);
		kmem_free(tmpbuffer);
	}

	xfs_da_log_buf(state->args->trans, save_blk->bp, 0,
=======
		struct xfs_attr_leafblock *tmp_leaf;
		struct xfs_attr3_icleaf_hdr tmphdr;

		tmp_leaf = kmem_zalloc(state->blocksize, KM_SLEEP);

		/*
		 * Copy the header into the temp leaf so that all the stuff
		 * not in the incore header is present and gets copied back in
		 * once we've moved all the entries.
		 */
		memcpy(tmp_leaf, save_leaf, xfs_attr3_leaf_hdr_size(save_leaf));

		memset(&tmphdr, 0, sizeof(tmphdr));
		tmphdr.magic = savehdr.magic;
		tmphdr.forw = savehdr.forw;
		tmphdr.back = savehdr.back;
		tmphdr.firstused = state->blocksize;

		/* write the header to the temp buffer to initialise it */
		xfs_attr3_leaf_hdr_to_disk(tmp_leaf, &tmphdr);

		if (xfs_attr3_leaf_order(save_blk->bp, &savehdr,
					 drop_blk->bp, &drophdr)) {
			xfs_attr3_leaf_moveents(drop_leaf, &drophdr, 0,
						tmp_leaf, &tmphdr, 0,
						drophdr.count, mp);
			xfs_attr3_leaf_moveents(save_leaf, &savehdr, 0,
						tmp_leaf, &tmphdr, tmphdr.count,
						savehdr.count, mp);
		} else {
			xfs_attr3_leaf_moveents(save_leaf, &savehdr, 0,
						tmp_leaf, &tmphdr, 0,
						savehdr.count, mp);
			xfs_attr3_leaf_moveents(drop_leaf, &drophdr, 0,
						tmp_leaf, &tmphdr, tmphdr.count,
						drophdr.count, mp);
		}
		memcpy(save_leaf, tmp_leaf, state->blocksize);
		savehdr = tmphdr; /* struct copy */
		kmem_free(tmp_leaf);
	}

	xfs_attr3_leaf_hdr_to_disk(save_leaf, &savehdr);
	xfs_trans_log_buf(state->args->trans, save_blk->bp, 0,
>>>>>>> refs/remotes/origin/master
					   state->blocksize - 1);

	/*
	 * Copy out last hashval in each block for B-tree code.
	 */
<<<<<<< HEAD
	save_blk->hashval = be32_to_cpu(
		save_leaf->entries[be16_to_cpu(save_leaf->hdr.count)-1].hashval);
=======
	entry = xfs_attr3_leaf_entryp(save_leaf);
	save_blk->hashval = be32_to_cpu(entry[savehdr.count - 1].hashval);
>>>>>>> refs/remotes/origin/master
}

/*========================================================================
 * Routines used for finding things in the Btree.
 *========================================================================*/

/*
 * Look up a name in a leaf attribute list structure.
 * This is the internal routine, it uses the caller's buffer.
 *
 * Note that duplicate keys are allowed, but only check within the
 * current leaf node.  The Btree code must check in adjacent leaf nodes.
 *
 * Return in args->index the index into the entry[] array of either
 * the found entry, or where the entry should have been (insert before
 * that entry).
 *
 * Don't change the args->value unless we find the attribute.
 */
int
<<<<<<< HEAD
xfs_attr_leaf_lookup_int(xfs_dabuf_t *bp, xfs_da_args_t *args)
{
	xfs_attr_leafblock_t *leaf;
	xfs_attr_leaf_entry_t *entry;
	xfs_attr_leaf_name_local_t *name_loc;
	xfs_attr_leaf_name_remote_t *name_rmt;
	int probe, span;
	xfs_dahash_t hashval;

<<<<<<< HEAD
	leaf = bp->data;
	ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_ATTR_LEAF_MAGIC);
=======
	trace_xfs_attr_leaf_lookup(args);

	leaf = bp->data;
	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	ASSERT(be16_to_cpu(leaf->hdr.count)
					< (XFS_LBSIZE(args->dp->i_mount)/8));
=======
xfs_attr3_leaf_lookup_int(
	struct xfs_buf		*bp,
	struct xfs_da_args	*args)
{
	struct xfs_attr_leafblock *leaf;
	struct xfs_attr3_icleaf_hdr ichdr;
	struct xfs_attr_leaf_entry *entry;
	struct xfs_attr_leaf_entry *entries;
	struct xfs_attr_leaf_name_local *name_loc;
	struct xfs_attr_leaf_name_remote *name_rmt;
	xfs_dahash_t		hashval;
	int			probe;
	int			span;

	trace_xfs_attr_leaf_lookup(args);

	leaf = bp->b_addr;
	xfs_attr3_leaf_hdr_from_disk(&ichdr, leaf);
	entries = xfs_attr3_leaf_entryp(leaf);
	ASSERT(ichdr.count < XFS_LBSIZE(args->dp->i_mount) / 8);
>>>>>>> refs/remotes/origin/master

	/*
	 * Binary search.  (note: small blocks will skip this loop)
	 */
	hashval = args->hashval;
<<<<<<< HEAD
	probe = span = be16_to_cpu(leaf->hdr.count) / 2;
	for (entry = &leaf->entries[probe]; span > 4;
		   entry = &leaf->entries[probe]) {
=======
	probe = span = ichdr.count / 2;
	for (entry = &entries[probe]; span > 4; entry = &entries[probe]) {
>>>>>>> refs/remotes/origin/master
		span /= 2;
		if (be32_to_cpu(entry->hashval) < hashval)
			probe += span;
		else if (be32_to_cpu(entry->hashval) > hashval)
			probe -= span;
		else
			break;
	}
<<<<<<< HEAD
	ASSERT((probe >= 0) &&
	       (!leaf->hdr.count
	       || (probe < be16_to_cpu(leaf->hdr.count))));
	ASSERT((span <= 4) || (be32_to_cpu(entry->hashval) == hashval));
=======
	ASSERT(probe >= 0 && (!ichdr.count || probe < ichdr.count));
	ASSERT(span <= 4 || be32_to_cpu(entry->hashval) == hashval);
>>>>>>> refs/remotes/origin/master

	/*
	 * Since we may have duplicate hashval's, find the first matching
	 * hashval in the leaf.
	 */
<<<<<<< HEAD
	while ((probe > 0) && (be32_to_cpu(entry->hashval) >= hashval)) {
		entry--;
		probe--;
	}
	while ((probe < be16_to_cpu(leaf->hdr.count)) &&
	       (be32_to_cpu(entry->hashval) < hashval)) {
		entry++;
		probe++;
	}
	if ((probe == be16_to_cpu(leaf->hdr.count)) ||
	    (be32_to_cpu(entry->hashval) != hashval)) {
		args->index = probe;
		return(XFS_ERROR(ENOATTR));
=======
	while (probe > 0 && be32_to_cpu(entry->hashval) >= hashval) {
		entry--;
		probe--;
	}
	while (probe < ichdr.count &&
	       be32_to_cpu(entry->hashval) < hashval) {
		entry++;
		probe++;
	}
	if (probe == ichdr.count || be32_to_cpu(entry->hashval) != hashval) {
		args->index = probe;
		return XFS_ERROR(ENOATTR);
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * Duplicate keys may be present, so search all of them for a match.
	 */
<<<<<<< HEAD
	for (  ; (probe < be16_to_cpu(leaf->hdr.count)) &&
			(be32_to_cpu(entry->hashval) == hashval);
=======
	for (; probe < ichdr.count && (be32_to_cpu(entry->hashval) == hashval);
>>>>>>> refs/remotes/origin/master
			entry++, probe++) {
/*
 * GROT: Add code to remove incomplete entries.
 */
		/*
		 * If we are looking for INCOMPLETE entries, show only those.
		 * If we are looking for complete entries, show only those.
		 */
		if ((args->flags & XFS_ATTR_INCOMPLETE) !=
		    (entry->flags & XFS_ATTR_INCOMPLETE)) {
			continue;
		}
		if (entry->flags & XFS_ATTR_LOCAL) {
<<<<<<< HEAD
			name_loc = xfs_attr_leaf_name_local(leaf, probe);
			if (name_loc->namelen != args->namelen)
				continue;
			if (memcmp(args->name, (char *)name_loc->nameval, args->namelen) != 0)
=======
			name_loc = xfs_attr3_leaf_name_local(leaf, probe);
			if (name_loc->namelen != args->namelen)
				continue;
			if (memcmp(args->name, name_loc->nameval,
							args->namelen) != 0)
>>>>>>> refs/remotes/origin/master
				continue;
			if (!xfs_attr_namesp_match(args->flags, entry->flags))
				continue;
			args->index = probe;
<<<<<<< HEAD
			return(XFS_ERROR(EEXIST));
		} else {
			name_rmt = xfs_attr_leaf_name_remote(leaf, probe);
			if (name_rmt->namelen != args->namelen)
				continue;
			if (memcmp(args->name, (char *)name_rmt->name,
					     args->namelen) != 0)
=======
			return XFS_ERROR(EEXIST);
		} else {
			name_rmt = xfs_attr3_leaf_name_remote(leaf, probe);
			if (name_rmt->namelen != args->namelen)
				continue;
			if (memcmp(args->name, name_rmt->name,
							args->namelen) != 0)
>>>>>>> refs/remotes/origin/master
				continue;
			if (!xfs_attr_namesp_match(args->flags, entry->flags))
				continue;
			args->index = probe;
<<<<<<< HEAD
			args->rmtblkno = be32_to_cpu(name_rmt->valueblk);
			args->rmtblkcnt = XFS_B_TO_FSB(args->dp->i_mount,
						   be32_to_cpu(name_rmt->valuelen));
			return(XFS_ERROR(EEXIST));
		}
	}
	args->index = probe;
	return(XFS_ERROR(ENOATTR));
=======
			args->valuelen = be32_to_cpu(name_rmt->valuelen);
			args->rmtblkno = be32_to_cpu(name_rmt->valueblk);
			args->rmtblkcnt = xfs_attr3_rmt_blocks(
							args->dp->i_mount,
							args->valuelen);
			return XFS_ERROR(EEXIST);
		}
	}
	args->index = probe;
	return XFS_ERROR(ENOATTR);
>>>>>>> refs/remotes/origin/master
}

/*
 * Get the value associated with an attribute name from a leaf attribute
 * list structure.
 */
int
<<<<<<< HEAD
xfs_attr_leaf_getvalue(xfs_dabuf_t *bp, xfs_da_args_t *args)
{
	int valuelen;
	xfs_attr_leafblock_t *leaf;
	xfs_attr_leaf_entry_t *entry;
	xfs_attr_leaf_name_local_t *name_loc;
	xfs_attr_leaf_name_remote_t *name_rmt;

	leaf = bp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_ATTR_LEAF_MAGIC);
=======
	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	ASSERT(be16_to_cpu(leaf->hdr.count)
					< (XFS_LBSIZE(args->dp->i_mount)/8));
	ASSERT(args->index < be16_to_cpu(leaf->hdr.count));

	entry = &leaf->entries[args->index];
	if (entry->flags & XFS_ATTR_LOCAL) {
		name_loc = xfs_attr_leaf_name_local(leaf, args->index);
=======
xfs_attr3_leaf_getvalue(
	struct xfs_buf		*bp,
	struct xfs_da_args	*args)
{
	struct xfs_attr_leafblock *leaf;
	struct xfs_attr3_icleaf_hdr ichdr;
	struct xfs_attr_leaf_entry *entry;
	struct xfs_attr_leaf_name_local *name_loc;
	struct xfs_attr_leaf_name_remote *name_rmt;
	int			valuelen;

	leaf = bp->b_addr;
	xfs_attr3_leaf_hdr_from_disk(&ichdr, leaf);
	ASSERT(ichdr.count < XFS_LBSIZE(args->dp->i_mount) / 8);
	ASSERT(args->index < ichdr.count);

	entry = &xfs_attr3_leaf_entryp(leaf)[args->index];
	if (entry->flags & XFS_ATTR_LOCAL) {
		name_loc = xfs_attr3_leaf_name_local(leaf, args->index);
>>>>>>> refs/remotes/origin/master
		ASSERT(name_loc->namelen == args->namelen);
		ASSERT(memcmp(args->name, name_loc->nameval, args->namelen) == 0);
		valuelen = be16_to_cpu(name_loc->valuelen);
		if (args->flags & ATTR_KERNOVAL) {
			args->valuelen = valuelen;
<<<<<<< HEAD
			return(0);
		}
		if (args->valuelen < valuelen) {
			args->valuelen = valuelen;
			return(XFS_ERROR(ERANGE));
=======
			return 0;
		}
		if (args->valuelen < valuelen) {
			args->valuelen = valuelen;
			return XFS_ERROR(ERANGE);
>>>>>>> refs/remotes/origin/master
		}
		args->valuelen = valuelen;
		memcpy(args->value, &name_loc->nameval[args->namelen], valuelen);
	} else {
<<<<<<< HEAD
		name_rmt = xfs_attr_leaf_name_remote(leaf, args->index);
=======
		name_rmt = xfs_attr3_leaf_name_remote(leaf, args->index);
>>>>>>> refs/remotes/origin/master
		ASSERT(name_rmt->namelen == args->namelen);
		ASSERT(memcmp(args->name, name_rmt->name, args->namelen) == 0);
		valuelen = be32_to_cpu(name_rmt->valuelen);
		args->rmtblkno = be32_to_cpu(name_rmt->valueblk);
<<<<<<< HEAD
		args->rmtblkcnt = XFS_B_TO_FSB(args->dp->i_mount, valuelen);
		if (args->flags & ATTR_KERNOVAL) {
			args->valuelen = valuelen;
			return(0);
		}
		if (args->valuelen < valuelen) {
			args->valuelen = valuelen;
			return(XFS_ERROR(ERANGE));
		}
		args->valuelen = valuelen;
	}
	return(0);
=======
		args->rmtblkcnt = xfs_attr3_rmt_blocks(args->dp->i_mount,
						       valuelen);
		if (args->flags & ATTR_KERNOVAL) {
			args->valuelen = valuelen;
			return 0;
		}
		if (args->valuelen < valuelen) {
			args->valuelen = valuelen;
			return XFS_ERROR(ERANGE);
		}
		args->valuelen = valuelen;
	}
	return 0;
>>>>>>> refs/remotes/origin/master
}

/*========================================================================
 * Utility routines.
 *========================================================================*/

/*
 * Move the indicated entries from one leaf to another.
 * NOTE: this routine modifies both source and destination leaves.
 */
/*ARGSUSED*/
STATIC void
<<<<<<< HEAD
xfs_attr_leaf_moveents(xfs_attr_leafblock_t *leaf_s, int start_s,
			xfs_attr_leafblock_t *leaf_d, int start_d,
			int count, xfs_mount_t *mp)
{
	xfs_attr_leaf_hdr_t *hdr_s, *hdr_d;
	xfs_attr_leaf_entry_t *entry_s, *entry_d;
	int desti, tmp, i;
=======
xfs_attr3_leaf_moveents(
	struct xfs_attr_leafblock	*leaf_s,
	struct xfs_attr3_icleaf_hdr	*ichdr_s,
	int				start_s,
	struct xfs_attr_leafblock	*leaf_d,
	struct xfs_attr3_icleaf_hdr	*ichdr_d,
	int				start_d,
	int				count,
	struct xfs_mount		*mp)
{
	struct xfs_attr_leaf_entry	*entry_s;
	struct xfs_attr_leaf_entry	*entry_d;
	int				desti;
	int				tmp;
	int				i;
>>>>>>> refs/remotes/origin/master

	/*
	 * Check for nothing to do.
	 */
	if (count == 0)
		return;

	/*
	 * Set up environment.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf_s->hdr.info.magic) == XFS_ATTR_LEAF_MAGIC);
	ASSERT(be16_to_cpu(leaf_d->hdr.info.magic) == XFS_ATTR_LEAF_MAGIC);
=======
	ASSERT(leaf_s->hdr.info.magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC));
	ASSERT(leaf_d->hdr.info.magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	hdr_s = &leaf_s->hdr;
	hdr_d = &leaf_d->hdr;
	ASSERT((be16_to_cpu(hdr_s->count) > 0) &&
	       (be16_to_cpu(hdr_s->count) < (XFS_LBSIZE(mp)/8)));
	ASSERT(be16_to_cpu(hdr_s->firstused) >=
		((be16_to_cpu(hdr_s->count)
					* sizeof(*entry_s))+sizeof(*hdr_s)));
	ASSERT(be16_to_cpu(hdr_d->count) < (XFS_LBSIZE(mp)/8));
	ASSERT(be16_to_cpu(hdr_d->firstused) >=
		((be16_to_cpu(hdr_d->count)
					* sizeof(*entry_d))+sizeof(*hdr_d)));

	ASSERT(start_s < be16_to_cpu(hdr_s->count));
	ASSERT(start_d <= be16_to_cpu(hdr_d->count));
	ASSERT(count <= be16_to_cpu(hdr_s->count));
=======
	ASSERT(ichdr_s->magic == XFS_ATTR_LEAF_MAGIC ||
	       ichdr_s->magic == XFS_ATTR3_LEAF_MAGIC);
	ASSERT(ichdr_s->magic == ichdr_d->magic);
	ASSERT(ichdr_s->count > 0 && ichdr_s->count < XFS_LBSIZE(mp) / 8);
	ASSERT(ichdr_s->firstused >= (ichdr_s->count * sizeof(*entry_s))
					+ xfs_attr3_leaf_hdr_size(leaf_s));
	ASSERT(ichdr_d->count < XFS_LBSIZE(mp) / 8);
	ASSERT(ichdr_d->firstused >= (ichdr_d->count * sizeof(*entry_d))
					+ xfs_attr3_leaf_hdr_size(leaf_d));

	ASSERT(start_s < ichdr_s->count);
	ASSERT(start_d <= ichdr_d->count);
	ASSERT(count <= ichdr_s->count);

>>>>>>> refs/remotes/origin/master

	/*
	 * Move the entries in the destination leaf up to make a hole?
	 */
<<<<<<< HEAD
	if (start_d < be16_to_cpu(hdr_d->count)) {
		tmp  = be16_to_cpu(hdr_d->count) - start_d;
		tmp *= sizeof(xfs_attr_leaf_entry_t);
		entry_s = &leaf_d->entries[start_d];
		entry_d = &leaf_d->entries[start_d + count];
		memmove((char *)entry_d, (char *)entry_s, tmp);
=======
	if (start_d < ichdr_d->count) {
		tmp  = ichdr_d->count - start_d;
		tmp *= sizeof(xfs_attr_leaf_entry_t);
		entry_s = &xfs_attr3_leaf_entryp(leaf_d)[start_d];
		entry_d = &xfs_attr3_leaf_entryp(leaf_d)[start_d + count];
		memmove(entry_d, entry_s, tmp);
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * Copy all entry's in the same (sorted) order,
	 * but allocate attribute info packed and in sequence.
	 */
<<<<<<< HEAD
	entry_s = &leaf_s->entries[start_s];
	entry_d = &leaf_d->entries[start_d];
	desti = start_d;
	for (i = 0; i < count; entry_s++, entry_d++, desti++, i++) {
		ASSERT(be16_to_cpu(entry_s->nameidx)
				>= be16_to_cpu(hdr_s->firstused));
=======
	entry_s = &xfs_attr3_leaf_entryp(leaf_s)[start_s];
	entry_d = &xfs_attr3_leaf_entryp(leaf_d)[start_d];
	desti = start_d;
	for (i = 0; i < count; entry_s++, entry_d++, desti++, i++) {
		ASSERT(be16_to_cpu(entry_s->nameidx) >= ichdr_s->firstused);
>>>>>>> refs/remotes/origin/master
		tmp = xfs_attr_leaf_entsize(leaf_s, start_s + i);
#ifdef GROT
		/*
		 * Code to drop INCOMPLETE entries.  Difficult to use as we
		 * may also need to change the insertion index.  Code turned
		 * off for 6.2, should be revisited later.
		 */
		if (entry_s->flags & XFS_ATTR_INCOMPLETE) { /* skip partials? */
<<<<<<< HEAD
			memset(xfs_attr_leaf_name(leaf_s, start_s + i), 0, tmp);
			be16_add_cpu(&hdr_s->usedbytes, -tmp);
			be16_add_cpu(&hdr_s->count, -1);
=======
			memset(xfs_attr3_leaf_name(leaf_s, start_s + i), 0, tmp);
			ichdr_s->usedbytes -= tmp;
			ichdr_s->count -= 1;
>>>>>>> refs/remotes/origin/master
			entry_d--;	/* to compensate for ++ in loop hdr */
			desti--;
			if ((start_s + i) < offset)
				result++;	/* insertion index adjustment */
		} else {
#endif /* GROT */
<<<<<<< HEAD
			be16_add_cpu(&hdr_d->firstused, -tmp);
			/* both on-disk, don't endian flip twice */
			entry_d->hashval = entry_s->hashval;
			/* both on-disk, don't endian flip twice */
			entry_d->nameidx = hdr_d->firstused;
			entry_d->flags = entry_s->flags;
			ASSERT(be16_to_cpu(entry_d->nameidx) + tmp
							<= XFS_LBSIZE(mp));
			memmove(xfs_attr_leaf_name(leaf_d, desti),
				xfs_attr_leaf_name(leaf_s, start_s + i), tmp);
			ASSERT(be16_to_cpu(entry_s->nameidx) + tmp
							<= XFS_LBSIZE(mp));
			memset(xfs_attr_leaf_name(leaf_s, start_s + i), 0, tmp);
			be16_add_cpu(&hdr_s->usedbytes, -tmp);
			be16_add_cpu(&hdr_d->usedbytes, tmp);
			be16_add_cpu(&hdr_s->count, -1);
			be16_add_cpu(&hdr_d->count, 1);
			tmp = be16_to_cpu(hdr_d->count)
						* sizeof(xfs_attr_leaf_entry_t)
						+ sizeof(xfs_attr_leaf_hdr_t);
			ASSERT(be16_to_cpu(hdr_d->firstused) >= tmp);
=======
			ichdr_d->firstused -= tmp;
			/* both on-disk, don't endian flip twice */
			entry_d->hashval = entry_s->hashval;
			entry_d->nameidx = cpu_to_be16(ichdr_d->firstused);
			entry_d->flags = entry_s->flags;
			ASSERT(be16_to_cpu(entry_d->nameidx) + tmp
							<= XFS_LBSIZE(mp));
			memmove(xfs_attr3_leaf_name(leaf_d, desti),
				xfs_attr3_leaf_name(leaf_s, start_s + i), tmp);
			ASSERT(be16_to_cpu(entry_s->nameidx) + tmp
							<= XFS_LBSIZE(mp));
			memset(xfs_attr3_leaf_name(leaf_s, start_s + i), 0, tmp);
			ichdr_s->usedbytes -= tmp;
			ichdr_d->usedbytes += tmp;
			ichdr_s->count -= 1;
			ichdr_d->count += 1;
			tmp = ichdr_d->count * sizeof(xfs_attr_leaf_entry_t)
					+ xfs_attr3_leaf_hdr_size(leaf_d);
			ASSERT(ichdr_d->firstused >= tmp);
>>>>>>> refs/remotes/origin/master
#ifdef GROT
		}
#endif /* GROT */
	}

	/*
	 * Zero out the entries we just copied.
	 */
<<<<<<< HEAD
	if (start_s == be16_to_cpu(hdr_s->count)) {
		tmp = count * sizeof(xfs_attr_leaf_entry_t);
		entry_s = &leaf_s->entries[start_s];
		ASSERT(((char *)entry_s + tmp) <=
		       ((char *)leaf_s + XFS_LBSIZE(mp)));
		memset((char *)entry_s, 0, tmp);
=======
	if (start_s == ichdr_s->count) {
		tmp = count * sizeof(xfs_attr_leaf_entry_t);
		entry_s = &xfs_attr3_leaf_entryp(leaf_s)[start_s];
		ASSERT(((char *)entry_s + tmp) <=
		       ((char *)leaf_s + XFS_LBSIZE(mp)));
		memset(entry_s, 0, tmp);
>>>>>>> refs/remotes/origin/master
	} else {
		/*
		 * Move the remaining entries down to fill the hole,
		 * then zero the entries at the top.
		 */
<<<<<<< HEAD
		tmp  = be16_to_cpu(hdr_s->count) - count;
		tmp *= sizeof(xfs_attr_leaf_entry_t);
		entry_s = &leaf_s->entries[start_s + count];
		entry_d = &leaf_s->entries[start_s];
		memmove((char *)entry_d, (char *)entry_s, tmp);

		tmp = count * sizeof(xfs_attr_leaf_entry_t);
		entry_s = &leaf_s->entries[be16_to_cpu(hdr_s->count)];
		ASSERT(((char *)entry_s + tmp) <=
		       ((char *)leaf_s + XFS_LBSIZE(mp)));
		memset((char *)entry_s, 0, tmp);
=======
		tmp  = (ichdr_s->count - count) * sizeof(xfs_attr_leaf_entry_t);
		entry_s = &xfs_attr3_leaf_entryp(leaf_s)[start_s + count];
		entry_d = &xfs_attr3_leaf_entryp(leaf_s)[start_s];
		memmove(entry_d, entry_s, tmp);

		tmp = count * sizeof(xfs_attr_leaf_entry_t);
		entry_s = &xfs_attr3_leaf_entryp(leaf_s)[ichdr_s->count];
		ASSERT(((char *)entry_s + tmp) <=
		       ((char *)leaf_s + XFS_LBSIZE(mp)));
		memset(entry_s, 0, tmp);
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * Fill in the freemap information
	 */
<<<<<<< HEAD
	hdr_d->freemap[0].base = cpu_to_be16(sizeof(xfs_attr_leaf_hdr_t));
	be16_add_cpu(&hdr_d->freemap[0].base, be16_to_cpu(hdr_d->count) *
			sizeof(xfs_attr_leaf_entry_t));
	hdr_d->freemap[0].size = cpu_to_be16(be16_to_cpu(hdr_d->firstused)
			      - be16_to_cpu(hdr_d->freemap[0].base));
	hdr_d->freemap[1].base = 0;
	hdr_d->freemap[2].base = 0;
	hdr_d->freemap[1].size = 0;
	hdr_d->freemap[2].size = 0;
	hdr_s->holes = 1;	/* leaf may not be compact */
}

/*
 * Compare two leaf blocks "order".
 * Return 0 unless leaf2 should go before leaf1.
 */
int
xfs_attr_leaf_order(xfs_dabuf_t *leaf1_bp, xfs_dabuf_t *leaf2_bp)
{
	xfs_attr_leafblock_t *leaf1, *leaf2;

	leaf1 = leaf1_bp->data;
	leaf2 = leaf2_bp->data;
<<<<<<< HEAD
	ASSERT((be16_to_cpu(leaf1->hdr.info.magic) == XFS_ATTR_LEAF_MAGIC) &&
	       (be16_to_cpu(leaf2->hdr.info.magic) == XFS_ATTR_LEAF_MAGIC));
=======
	ASSERT((leaf1->hdr.info.magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC)) &&
	       (leaf2->hdr.info.magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC)));
>>>>>>> refs/remotes/origin/cm-10.0
	if ((be16_to_cpu(leaf1->hdr.count) > 0) &&
	    (be16_to_cpu(leaf2->hdr.count) > 0) &&
	    ((be32_to_cpu(leaf2->entries[0].hashval) <
	      be32_to_cpu(leaf1->entries[0].hashval)) ||
	     (be32_to_cpu(leaf2->entries[
			be16_to_cpu(leaf2->hdr.count)-1].hashval) <
	      be32_to_cpu(leaf1->entries[
			be16_to_cpu(leaf1->hdr.count)-1].hashval)))) {
		return(1);
	}
	return(0);
=======
	ichdr_d->freemap[0].base = xfs_attr3_leaf_hdr_size(leaf_d);
	ichdr_d->freemap[0].base += ichdr_d->count * sizeof(xfs_attr_leaf_entry_t);
	ichdr_d->freemap[0].size = ichdr_d->firstused - ichdr_d->freemap[0].base;
	ichdr_d->freemap[1].base = 0;
	ichdr_d->freemap[2].base = 0;
	ichdr_d->freemap[1].size = 0;
	ichdr_d->freemap[2].size = 0;
	ichdr_s->holes = 1;	/* leaf may not be compact */
>>>>>>> refs/remotes/origin/master
}

/*
 * Pick up the last hashvalue from a leaf block.
 */
xfs_dahash_t
<<<<<<< HEAD
xfs_attr_leaf_lasthash(xfs_dabuf_t *bp, int *count)
{
	xfs_attr_leafblock_t *leaf;

	leaf = bp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_ATTR_LEAF_MAGIC);
=======
	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	if (count)
		*count = be16_to_cpu(leaf->hdr.count);
	if (!leaf->hdr.count)
		return(0);
	return be32_to_cpu(leaf->entries[be16_to_cpu(leaf->hdr.count)-1].hashval);
=======
xfs_attr_leaf_lasthash(
	struct xfs_buf	*bp,
	int		*count)
{
	struct xfs_attr3_icleaf_hdr ichdr;
	struct xfs_attr_leaf_entry *entries;

	xfs_attr3_leaf_hdr_from_disk(&ichdr, bp->b_addr);
	entries = xfs_attr3_leaf_entryp(bp->b_addr);
	if (count)
		*count = ichdr.count;
	if (!ichdr.count)
		return 0;
	return be32_to_cpu(entries[ichdr.count - 1].hashval);
>>>>>>> refs/remotes/origin/master
}

/*
 * Calculate the number of bytes used to store the indicated attribute
 * (whether local or remote only calculate bytes in this block).
 */
STATIC int
xfs_attr_leaf_entsize(xfs_attr_leafblock_t *leaf, int index)
{
<<<<<<< HEAD
=======
	struct xfs_attr_leaf_entry *entries;
>>>>>>> refs/remotes/origin/master
	xfs_attr_leaf_name_local_t *name_loc;
	xfs_attr_leaf_name_remote_t *name_rmt;
	int size;

<<<<<<< HEAD
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_ATTR_LEAF_MAGIC);
=======
	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	if (leaf->entries[index].flags & XFS_ATTR_LOCAL) {
		name_loc = xfs_attr_leaf_name_local(leaf, index);
		size = xfs_attr_leaf_entsize_local(name_loc->namelen,
						   be16_to_cpu(name_loc->valuelen));
	} else {
		name_rmt = xfs_attr_leaf_name_remote(leaf, index);
		size = xfs_attr_leaf_entsize_remote(name_rmt->namelen);
	}
	return(size);
=======
	entries = xfs_attr3_leaf_entryp(leaf);
	if (entries[index].flags & XFS_ATTR_LOCAL) {
		name_loc = xfs_attr3_leaf_name_local(leaf, index);
		size = xfs_attr_leaf_entsize_local(name_loc->namelen,
						   be16_to_cpu(name_loc->valuelen));
	} else {
		name_rmt = xfs_attr3_leaf_name_remote(leaf, index);
		size = xfs_attr_leaf_entsize_remote(name_rmt->namelen);
	}
	return size;
>>>>>>> refs/remotes/origin/master
}

/*
 * Calculate the number of bytes that would be required to store the new
 * attribute (whether local or remote only calculate bytes in this block).
 * This routine decides as a side effect whether the attribute will be
 * a "local" or a "remote" attribute.
 */
int
xfs_attr_leaf_newentsize(int namelen, int valuelen, int blocksize, int *local)
{
	int size;

	size = xfs_attr_leaf_entsize_local(namelen, valuelen);
	if (size < xfs_attr_leaf_entsize_local_max(blocksize)) {
		if (local) {
			*local = 1;
		}
	} else {
		size = xfs_attr_leaf_entsize_remote(namelen);
		if (local) {
			*local = 0;
		}
	}
<<<<<<< HEAD
	return(size);
}

/*
 * Copy out attribute list entries for attr_list(), for leaf attribute lists.
 */
int
xfs_attr_leaf_list_int(xfs_dabuf_t *bp, xfs_attr_list_context_t *context)
{
	attrlist_cursor_kern_t *cursor;
	xfs_attr_leafblock_t *leaf;
	xfs_attr_leaf_entry_t *entry;
	int retval, i;

	ASSERT(bp != NULL);
	leaf = bp->data;
	cursor = context->cursor;
	cursor->initted = 1;

	trace_xfs_attr_list_leaf(context);

	/*
	 * Re-find our place in the leaf block if this is a new syscall.
	 */
	if (context->resynch) {
		entry = &leaf->entries[0];
		for (i = 0; i < be16_to_cpu(leaf->hdr.count); entry++, i++) {
			if (be32_to_cpu(entry->hashval) == cursor->hashval) {
				if (cursor->offset == context->dupcnt) {
					context->dupcnt = 0;
					break;
				}
				context->dupcnt++;
			} else if (be32_to_cpu(entry->hashval) >
					cursor->hashval) {
				context->dupcnt = 0;
				break;
			}
		}
		if (i == be16_to_cpu(leaf->hdr.count)) {
			trace_xfs_attr_list_notfound(context);
			return(0);
		}
	} else {
		entry = &leaf->entries[0];
		i = 0;
	}
	context->resynch = 0;

	/*
	 * We have found our place, start copying out the new attributes.
	 */
	retval = 0;
	for (  ; (i < be16_to_cpu(leaf->hdr.count)); entry++, i++) {
		if (be32_to_cpu(entry->hashval) != cursor->hashval) {
			cursor->hashval = be32_to_cpu(entry->hashval);
			cursor->offset = 0;
		}

		if (entry->flags & XFS_ATTR_INCOMPLETE)
			continue;		/* skip incomplete entries */

		if (entry->flags & XFS_ATTR_LOCAL) {
			xfs_attr_leaf_name_local_t *name_loc =
				xfs_attr_leaf_name_local(leaf, i);

			retval = context->put_listent(context,
						entry->flags,
						name_loc->nameval,
						(int)name_loc->namelen,
						be16_to_cpu(name_loc->valuelen),
						&name_loc->nameval[name_loc->namelen]);
			if (retval)
				return retval;
		} else {
			xfs_attr_leaf_name_remote_t *name_rmt =
				xfs_attr_leaf_name_remote(leaf, i);

			int valuelen = be32_to_cpu(name_rmt->valuelen);

			if (context->put_value) {
				xfs_da_args_t args;

				memset((char *)&args, 0, sizeof(args));
				args.dp = context->dp;
				args.whichfork = XFS_ATTR_FORK;
				args.valuelen = valuelen;
				args.value = kmem_alloc(valuelen, KM_SLEEP | KM_NOFS);
				args.rmtblkno = be32_to_cpu(name_rmt->valueblk);
				args.rmtblkcnt = XFS_B_TO_FSB(args.dp->i_mount, valuelen);
				retval = xfs_attr_rmtval_get(&args);
				if (retval)
					return retval;
				retval = context->put_listent(context,
						entry->flags,
						name_rmt->name,
						(int)name_rmt->namelen,
						valuelen,
						args.value);
				kmem_free(args.value);
			} else {
				retval = context->put_listent(context,
						entry->flags,
						name_rmt->name,
						(int)name_rmt->namelen,
						valuelen,
						NULL);
			}
			if (retval)
				return retval;
		}
		if (context->seen_enough)
			break;
		cursor->offset++;
	}
	trace_xfs_attr_list_leaf_end(context);
	return(retval);
=======
	return size;
>>>>>>> refs/remotes/origin/master
}


/*========================================================================
 * Manage the INCOMPLETE flag in a leaf entry
 *========================================================================*/

/*
 * Clear the INCOMPLETE flag on an entry in a leaf block.
 */
int
<<<<<<< HEAD
xfs_attr_leaf_clearflag(xfs_da_args_t *args)
{
	xfs_attr_leafblock_t *leaf;
	xfs_attr_leaf_entry_t *entry;
	xfs_attr_leaf_name_remote_t *name_rmt;
	xfs_dabuf_t *bp;
	int error;
#ifdef DEBUG
=======
xfs_attr3_leaf_clearflag(
	struct xfs_da_args	*args)
{
	struct xfs_attr_leafblock *leaf;
	struct xfs_attr_leaf_entry *entry;
	struct xfs_attr_leaf_name_remote *name_rmt;
	struct xfs_buf		*bp;
	int			error;
#ifdef DEBUG
	struct xfs_attr3_icleaf_hdr ichdr;
>>>>>>> refs/remotes/origin/master
	xfs_attr_leaf_name_local_t *name_loc;
	int namelen;
	char *name;
#endif /* DEBUG */

<<<<<<< HEAD
<<<<<<< HEAD
=======
	trace_xfs_attr_leaf_clearflag(args);
>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * Set up the operation.
	 */
	error = xfs_da_read_buf(args->trans, args->dp, args->blkno, -1, &bp,
					     XFS_ATTR_FORK);
	if (error) {
		return(error);
	}
	ASSERT(bp != NULL);

	leaf = bp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_ATTR_LEAF_MAGIC);
=======
	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	ASSERT(args->index < be16_to_cpu(leaf->hdr.count));
	ASSERT(args->index >= 0);
	entry = &leaf->entries[ args->index ];
	ASSERT(entry->flags & XFS_ATTR_INCOMPLETE);

#ifdef DEBUG
	if (entry->flags & XFS_ATTR_LOCAL) {
		name_loc = xfs_attr_leaf_name_local(leaf, args->index);
		namelen = name_loc->namelen;
		name = (char *)name_loc->nameval;
	} else {
		name_rmt = xfs_attr_leaf_name_remote(leaf, args->index);
=======
	trace_xfs_attr_leaf_clearflag(args);
	/*
	 * Set up the operation.
	 */
	error = xfs_attr3_leaf_read(args->trans, args->dp, args->blkno, -1, &bp);
	if (error)
		return(error);

	leaf = bp->b_addr;
	entry = &xfs_attr3_leaf_entryp(leaf)[args->index];
	ASSERT(entry->flags & XFS_ATTR_INCOMPLETE);

#ifdef DEBUG
	xfs_attr3_leaf_hdr_from_disk(&ichdr, leaf);
	ASSERT(args->index < ichdr.count);
	ASSERT(args->index >= 0);

	if (entry->flags & XFS_ATTR_LOCAL) {
		name_loc = xfs_attr3_leaf_name_local(leaf, args->index);
		namelen = name_loc->namelen;
		name = (char *)name_loc->nameval;
	} else {
		name_rmt = xfs_attr3_leaf_name_remote(leaf, args->index);
>>>>>>> refs/remotes/origin/master
		namelen = name_rmt->namelen;
		name = (char *)name_rmt->name;
	}
	ASSERT(be32_to_cpu(entry->hashval) == args->hashval);
	ASSERT(namelen == args->namelen);
	ASSERT(memcmp(name, args->name, namelen) == 0);
#endif /* DEBUG */

	entry->flags &= ~XFS_ATTR_INCOMPLETE;
<<<<<<< HEAD
	xfs_da_log_buf(args->trans, bp,
=======
	xfs_trans_log_buf(args->trans, bp,
>>>>>>> refs/remotes/origin/master
			 XFS_DA_LOGRANGE(leaf, entry, sizeof(*entry)));

	if (args->rmtblkno) {
		ASSERT((entry->flags & XFS_ATTR_LOCAL) == 0);
<<<<<<< HEAD
		name_rmt = xfs_attr_leaf_name_remote(leaf, args->index);
		name_rmt->valueblk = cpu_to_be32(args->rmtblkno);
		name_rmt->valuelen = cpu_to_be32(args->valuelen);
		xfs_da_log_buf(args->trans, bp,
			 XFS_DA_LOGRANGE(leaf, name_rmt, sizeof(*name_rmt)));
	}
	xfs_da_buf_done(bp);
=======
		name_rmt = xfs_attr3_leaf_name_remote(leaf, args->index);
		name_rmt->valueblk = cpu_to_be32(args->rmtblkno);
		name_rmt->valuelen = cpu_to_be32(args->valuelen);
		xfs_trans_log_buf(args->trans, bp,
			 XFS_DA_LOGRANGE(leaf, name_rmt, sizeof(*name_rmt)));
	}
>>>>>>> refs/remotes/origin/master

	/*
	 * Commit the flag value change and start the next trans in series.
	 */
	return xfs_trans_roll(&args->trans, args->dp);
}

/*
 * Set the INCOMPLETE flag on an entry in a leaf block.
 */
int
<<<<<<< HEAD
xfs_attr_leaf_setflag(xfs_da_args_t *args)
{
	xfs_attr_leafblock_t *leaf;
	xfs_attr_leaf_entry_t *entry;
	xfs_attr_leaf_name_remote_t *name_rmt;
	xfs_dabuf_t *bp;
	int error;

<<<<<<< HEAD
=======
	trace_xfs_attr_leaf_setflag(args);

>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * Set up the operation.
	 */
	error = xfs_da_read_buf(args->trans, args->dp, args->blkno, -1, &bp,
					     XFS_ATTR_FORK);
	if (error) {
		return(error);
	}
	ASSERT(bp != NULL);

	leaf = bp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_ATTR_LEAF_MAGIC);
=======
	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	ASSERT(args->index < be16_to_cpu(leaf->hdr.count));
	ASSERT(args->index >= 0);
	entry = &leaf->entries[ args->index ];

	ASSERT((entry->flags & XFS_ATTR_INCOMPLETE) == 0);
	entry->flags |= XFS_ATTR_INCOMPLETE;
	xfs_da_log_buf(args->trans, bp,
			XFS_DA_LOGRANGE(leaf, entry, sizeof(*entry)));
	if ((entry->flags & XFS_ATTR_LOCAL) == 0) {
		name_rmt = xfs_attr_leaf_name_remote(leaf, args->index);
		name_rmt->valueblk = 0;
		name_rmt->valuelen = 0;
		xfs_da_log_buf(args->trans, bp,
			 XFS_DA_LOGRANGE(leaf, name_rmt, sizeof(*name_rmt)));
	}
	xfs_da_buf_done(bp);
=======
xfs_attr3_leaf_setflag(
	struct xfs_da_args	*args)
{
	struct xfs_attr_leafblock *leaf;
	struct xfs_attr_leaf_entry *entry;
	struct xfs_attr_leaf_name_remote *name_rmt;
	struct xfs_buf		*bp;
	int error;
#ifdef DEBUG
	struct xfs_attr3_icleaf_hdr ichdr;
#endif

	trace_xfs_attr_leaf_setflag(args);

	/*
	 * Set up the operation.
	 */
	error = xfs_attr3_leaf_read(args->trans, args->dp, args->blkno, -1, &bp);
	if (error)
		return(error);

	leaf = bp->b_addr;
#ifdef DEBUG
	xfs_attr3_leaf_hdr_from_disk(&ichdr, leaf);
	ASSERT(args->index < ichdr.count);
	ASSERT(args->index >= 0);
#endif
	entry = &xfs_attr3_leaf_entryp(leaf)[args->index];

	ASSERT((entry->flags & XFS_ATTR_INCOMPLETE) == 0);
	entry->flags |= XFS_ATTR_INCOMPLETE;
	xfs_trans_log_buf(args->trans, bp,
			XFS_DA_LOGRANGE(leaf, entry, sizeof(*entry)));
	if ((entry->flags & XFS_ATTR_LOCAL) == 0) {
		name_rmt = xfs_attr3_leaf_name_remote(leaf, args->index);
		name_rmt->valueblk = 0;
		name_rmt->valuelen = 0;
		xfs_trans_log_buf(args->trans, bp,
			 XFS_DA_LOGRANGE(leaf, name_rmt, sizeof(*name_rmt)));
	}
>>>>>>> refs/remotes/origin/master

	/*
	 * Commit the flag value change and start the next trans in series.
	 */
	return xfs_trans_roll(&args->trans, args->dp);
}

/*
 * In a single transaction, clear the INCOMPLETE flag on the leaf entry
 * given by args->blkno/index and set the INCOMPLETE flag on the leaf
 * entry given by args->blkno2/index2.
 *
 * Note that they could be in different blocks, or in the same block.
 */
int
<<<<<<< HEAD
xfs_attr_leaf_flipflags(xfs_da_args_t *args)
{
	xfs_attr_leafblock_t *leaf1, *leaf2;
	xfs_attr_leaf_entry_t *entry1, *entry2;
	xfs_attr_leaf_name_remote_t *name_rmt;
	xfs_dabuf_t *bp1, *bp2;
	int error;
#ifdef DEBUG
=======
xfs_attr3_leaf_flipflags(
	struct xfs_da_args	*args)
{
	struct xfs_attr_leafblock *leaf1;
	struct xfs_attr_leafblock *leaf2;
	struct xfs_attr_leaf_entry *entry1;
	struct xfs_attr_leaf_entry *entry2;
	struct xfs_attr_leaf_name_remote *name_rmt;
	struct xfs_buf		*bp1;
	struct xfs_buf		*bp2;
	int error;
#ifdef DEBUG
	struct xfs_attr3_icleaf_hdr ichdr1;
	struct xfs_attr3_icleaf_hdr ichdr2;
>>>>>>> refs/remotes/origin/master
	xfs_attr_leaf_name_local_t *name_loc;
	int namelen1, namelen2;
	char *name1, *name2;
#endif /* DEBUG */

<<<<<<< HEAD
<<<<<<< HEAD
=======
	trace_xfs_attr_leaf_flipflags(args);

>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * Read the block containing the "old" attr
	 */
	error = xfs_da_read_buf(args->trans, args->dp, args->blkno, -1, &bp1,
					     XFS_ATTR_FORK);
	if (error) {
		return(error);
	}
	ASSERT(bp1 != NULL);
=======
	trace_xfs_attr_leaf_flipflags(args);

	/*
	 * Read the block containing the "old" attr
	 */
	error = xfs_attr3_leaf_read(args->trans, args->dp, args->blkno, -1, &bp1);
	if (error)
		return error;
>>>>>>> refs/remotes/origin/master

	/*
	 * Read the block containing the "new" attr, if it is different
	 */
	if (args->blkno2 != args->blkno) {
<<<<<<< HEAD
		error = xfs_da_read_buf(args->trans, args->dp, args->blkno2,
					-1, &bp2, XFS_ATTR_FORK);
		if (error) {
			return(error);
		}
		ASSERT(bp2 != NULL);
=======
		error = xfs_attr3_leaf_read(args->trans, args->dp, args->blkno2,
					   -1, &bp2);
		if (error)
			return error;
>>>>>>> refs/remotes/origin/master
	} else {
		bp2 = bp1;
	}

<<<<<<< HEAD
	leaf1 = bp1->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf1->hdr.info.magic) == XFS_ATTR_LEAF_MAGIC);
=======
	ASSERT(leaf1->hdr.info.magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	ASSERT(args->index < be16_to_cpu(leaf1->hdr.count));
	ASSERT(args->index >= 0);
	entry1 = &leaf1->entries[ args->index ];

	leaf2 = bp2->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf2->hdr.info.magic) == XFS_ATTR_LEAF_MAGIC);
=======
	ASSERT(leaf2->hdr.info.magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	ASSERT(args->index2 < be16_to_cpu(leaf2->hdr.count));
	ASSERT(args->index2 >= 0);
	entry2 = &leaf2->entries[ args->index2 ];

#ifdef DEBUG
	if (entry1->flags & XFS_ATTR_LOCAL) {
		name_loc = xfs_attr_leaf_name_local(leaf1, args->index);
		namelen1 = name_loc->namelen;
		name1 = (char *)name_loc->nameval;
	} else {
		name_rmt = xfs_attr_leaf_name_remote(leaf1, args->index);
=======
	leaf1 = bp1->b_addr;
	entry1 = &xfs_attr3_leaf_entryp(leaf1)[args->index];

	leaf2 = bp2->b_addr;
	entry2 = &xfs_attr3_leaf_entryp(leaf2)[args->index2];

#ifdef DEBUG
	xfs_attr3_leaf_hdr_from_disk(&ichdr1, leaf1);
	ASSERT(args->index < ichdr1.count);
	ASSERT(args->index >= 0);

	xfs_attr3_leaf_hdr_from_disk(&ichdr2, leaf2);
	ASSERT(args->index2 < ichdr2.count);
	ASSERT(args->index2 >= 0);

	if (entry1->flags & XFS_ATTR_LOCAL) {
		name_loc = xfs_attr3_leaf_name_local(leaf1, args->index);
		namelen1 = name_loc->namelen;
		name1 = (char *)name_loc->nameval;
	} else {
		name_rmt = xfs_attr3_leaf_name_remote(leaf1, args->index);
>>>>>>> refs/remotes/origin/master
		namelen1 = name_rmt->namelen;
		name1 = (char *)name_rmt->name;
	}
	if (entry2->flags & XFS_ATTR_LOCAL) {
<<<<<<< HEAD
		name_loc = xfs_attr_leaf_name_local(leaf2, args->index2);
		namelen2 = name_loc->namelen;
		name2 = (char *)name_loc->nameval;
	} else {
		name_rmt = xfs_attr_leaf_name_remote(leaf2, args->index2);
=======
		name_loc = xfs_attr3_leaf_name_local(leaf2, args->index2);
		namelen2 = name_loc->namelen;
		name2 = (char *)name_loc->nameval;
	} else {
		name_rmt = xfs_attr3_leaf_name_remote(leaf2, args->index2);
>>>>>>> refs/remotes/origin/master
		namelen2 = name_rmt->namelen;
		name2 = (char *)name_rmt->name;
	}
	ASSERT(be32_to_cpu(entry1->hashval) == be32_to_cpu(entry2->hashval));
	ASSERT(namelen1 == namelen2);
	ASSERT(memcmp(name1, name2, namelen1) == 0);
#endif /* DEBUG */

	ASSERT(entry1->flags & XFS_ATTR_INCOMPLETE);
	ASSERT((entry2->flags & XFS_ATTR_INCOMPLETE) == 0);

	entry1->flags &= ~XFS_ATTR_INCOMPLETE;
<<<<<<< HEAD
	xfs_da_log_buf(args->trans, bp1,
			  XFS_DA_LOGRANGE(leaf1, entry1, sizeof(*entry1)));
	if (args->rmtblkno) {
		ASSERT((entry1->flags & XFS_ATTR_LOCAL) == 0);
		name_rmt = xfs_attr_leaf_name_remote(leaf1, args->index);
		name_rmt->valueblk = cpu_to_be32(args->rmtblkno);
		name_rmt->valuelen = cpu_to_be32(args->valuelen);
		xfs_da_log_buf(args->trans, bp1,
=======
	xfs_trans_log_buf(args->trans, bp1,
			  XFS_DA_LOGRANGE(leaf1, entry1, sizeof(*entry1)));
	if (args->rmtblkno) {
		ASSERT((entry1->flags & XFS_ATTR_LOCAL) == 0);
		name_rmt = xfs_attr3_leaf_name_remote(leaf1, args->index);
		name_rmt->valueblk = cpu_to_be32(args->rmtblkno);
		name_rmt->valuelen = cpu_to_be32(args->valuelen);
		xfs_trans_log_buf(args->trans, bp1,
>>>>>>> refs/remotes/origin/master
			 XFS_DA_LOGRANGE(leaf1, name_rmt, sizeof(*name_rmt)));
	}

	entry2->flags |= XFS_ATTR_INCOMPLETE;
<<<<<<< HEAD
	xfs_da_log_buf(args->trans, bp2,
			  XFS_DA_LOGRANGE(leaf2, entry2, sizeof(*entry2)));
	if ((entry2->flags & XFS_ATTR_LOCAL) == 0) {
		name_rmt = xfs_attr_leaf_name_remote(leaf2, args->index2);
		name_rmt->valueblk = 0;
		name_rmt->valuelen = 0;
		xfs_da_log_buf(args->trans, bp2,
			 XFS_DA_LOGRANGE(leaf2, name_rmt, sizeof(*name_rmt)));
	}
	xfs_da_buf_done(bp1);
	if (bp1 != bp2)
		xfs_da_buf_done(bp2);
=======
	xfs_trans_log_buf(args->trans, bp2,
			  XFS_DA_LOGRANGE(leaf2, entry2, sizeof(*entry2)));
	if ((entry2->flags & XFS_ATTR_LOCAL) == 0) {
		name_rmt = xfs_attr3_leaf_name_remote(leaf2, args->index2);
		name_rmt->valueblk = 0;
		name_rmt->valuelen = 0;
		xfs_trans_log_buf(args->trans, bp2,
			 XFS_DA_LOGRANGE(leaf2, name_rmt, sizeof(*name_rmt)));
	}
>>>>>>> refs/remotes/origin/master

	/*
	 * Commit the flag value change and start the next trans in series.
	 */
	error = xfs_trans_roll(&args->trans, args->dp);

<<<<<<< HEAD
	return(error);
}

/*========================================================================
 * Indiscriminately delete the entire attribute fork
 *========================================================================*/

/*
 * Recurse (gasp!) through the attribute nodes until we find leaves.
 * We're doing a depth-first traversal in order to invalidate everything.
 */
int
xfs_attr_root_inactive(xfs_trans_t **trans, xfs_inode_t *dp)
{
	xfs_da_blkinfo_t *info;
	xfs_daddr_t blkno;
	xfs_dabuf_t *bp;
	int error;

	/*
	 * Read block 0 to see what we have to work with.
	 * We only get here if we have extents, since we remove
	 * the extents in reverse order the extent containing
	 * block 0 must still be there.
	 */
	error = xfs_da_read_buf(*trans, dp, 0, -1, &bp, XFS_ATTR_FORK);
	if (error)
		return(error);
	blkno = xfs_da_blkno(bp);

	/*
	 * Invalidate the tree, even if the "tree" is only a single leaf block.
	 * This is a depth-first traversal!
	 */
	info = bp->data;
<<<<<<< HEAD
	if (be16_to_cpu(info->magic) == XFS_DA_NODE_MAGIC) {
		error = xfs_attr_node_inactive(trans, dp, bp, 1);
	} else if (be16_to_cpu(info->magic) == XFS_ATTR_LEAF_MAGIC) {
=======
	if (info->magic == cpu_to_be16(XFS_DA_NODE_MAGIC)) {
		error = xfs_attr_node_inactive(trans, dp, bp, 1);
	} else if (info->magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC)) {
>>>>>>> refs/remotes/origin/cm-10.0
		error = xfs_attr_leaf_inactive(trans, dp, bp);
	} else {
		error = XFS_ERROR(EIO);
		xfs_da_brelse(*trans, bp);
	}
	if (error)
		return(error);

	/*
	 * Invalidate the incore copy of the root block.
	 */
	error = xfs_da_get_buf(*trans, dp, 0, blkno, &bp, XFS_ATTR_FORK);
	if (error)
		return(error);
	xfs_da_binval(*trans, bp);	/* remove from cache */
	/*
	 * Commit the invalidate and start the next transaction.
	 */
	error = xfs_trans_roll(trans, dp);

	return (error);
}

/*
 * Recurse (gasp!) through the attribute nodes until we find leaves.
 * We're doing a depth-first traversal in order to invalidate everything.
 */
STATIC int
xfs_attr_node_inactive(xfs_trans_t **trans, xfs_inode_t *dp, xfs_dabuf_t *bp,
				   int level)
{
	xfs_da_blkinfo_t *info;
	xfs_da_intnode_t *node;
	xfs_dablk_t child_fsb;
	xfs_daddr_t parent_blkno, child_blkno;
	int error, count, i;
	xfs_dabuf_t *child_bp;

	/*
	 * Since this code is recursive (gasp!) we must protect ourselves.
	 */
	if (level > XFS_DA_NODE_MAXDEPTH) {
		xfs_da_brelse(*trans, bp);	/* no locks for later trans */
		return(XFS_ERROR(EIO));
	}

	node = bp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(node->hdr.info.magic) == XFS_DA_NODE_MAGIC);
=======
	ASSERT(node->hdr.info.magic == cpu_to_be16(XFS_DA_NODE_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
	parent_blkno = xfs_da_blkno(bp);	/* save for re-read later */
	count = be16_to_cpu(node->hdr.count);
	if (!count) {
		xfs_da_brelse(*trans, bp);
		return(0);
	}
	child_fsb = be32_to_cpu(node->btree[0].before);
	xfs_da_brelse(*trans, bp);	/* no locks for later trans */

	/*
	 * If this is the node level just above the leaves, simply loop
	 * over the leaves removing all of them.  If this is higher up
	 * in the tree, recurse downward.
	 */
	for (i = 0; i < count; i++) {
		/*
		 * Read the subsidiary block to see what we have to work with.
		 * Don't do this in a transaction.  This is a depth-first
		 * traversal of the tree so we may deal with many blocks
		 * before we come back to this one.
		 */
		error = xfs_da_read_buf(*trans, dp, child_fsb, -2, &child_bp,
						XFS_ATTR_FORK);
		if (error)
			return(error);
		if (child_bp) {
						/* save for re-read later */
			child_blkno = xfs_da_blkno(child_bp);

			/*
			 * Invalidate the subtree, however we have to.
			 */
			info = child_bp->data;
<<<<<<< HEAD
			if (be16_to_cpu(info->magic) == XFS_DA_NODE_MAGIC) {
				error = xfs_attr_node_inactive(trans, dp,
						child_bp, level+1);
			} else if (be16_to_cpu(info->magic) == XFS_ATTR_LEAF_MAGIC) {
=======
			if (info->magic == cpu_to_be16(XFS_DA_NODE_MAGIC)) {
				error = xfs_attr_node_inactive(trans, dp,
						child_bp, level+1);
			} else if (info->magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC)) {
>>>>>>> refs/remotes/origin/cm-10.0
				error = xfs_attr_leaf_inactive(trans, dp,
						child_bp);
			} else {
				error = XFS_ERROR(EIO);
				xfs_da_brelse(*trans, child_bp);
			}
			if (error)
				return(error);

			/*
			 * Remove the subsidiary block from the cache
			 * and from the log.
			 */
			error = xfs_da_get_buf(*trans, dp, 0, child_blkno,
				&child_bp, XFS_ATTR_FORK);
			if (error)
				return(error);
			xfs_da_binval(*trans, child_bp);
		}

		/*
		 * If we're not done, re-read the parent to get the next
		 * child block number.
		 */
		if ((i+1) < count) {
			error = xfs_da_read_buf(*trans, dp, 0, parent_blkno,
				&bp, XFS_ATTR_FORK);
			if (error)
				return(error);
			child_fsb = be32_to_cpu(node->btree[i+1].before);
			xfs_da_brelse(*trans, bp);
		}
		/*
		 * Atomically commit the whole invalidate stuff.
		 */
		error = xfs_trans_roll(trans, dp);
		if (error)
			return (error);
	}

	return(0);
}

/*
 * Invalidate all of the "remote" value regions pointed to by a particular
 * leaf block.
 * Note that we must release the lock on the buffer so that we are not
 * caught holding something that the logging code wants to flush to disk.
 */
STATIC int
xfs_attr_leaf_inactive(xfs_trans_t **trans, xfs_inode_t *dp, xfs_dabuf_t *bp)
{
	xfs_attr_leafblock_t *leaf;
	xfs_attr_leaf_entry_t *entry;
	xfs_attr_leaf_name_remote_t *name_rmt;
	xfs_attr_inactive_list_t *list, *lp;
	int error, count, size, tmp, i;

	leaf = bp->data;
<<<<<<< HEAD
	ASSERT(be16_to_cpu(leaf->hdr.info.magic) == XFS_ATTR_LEAF_MAGIC);
=======
	ASSERT(leaf->hdr.info.magic == cpu_to_be16(XFS_ATTR_LEAF_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0

	/*
	 * Count the number of "remote" value extents.
	 */
	count = 0;
	entry = &leaf->entries[0];
	for (i = 0; i < be16_to_cpu(leaf->hdr.count); entry++, i++) {
		if (be16_to_cpu(entry->nameidx) &&
		    ((entry->flags & XFS_ATTR_LOCAL) == 0)) {
			name_rmt = xfs_attr_leaf_name_remote(leaf, i);
			if (name_rmt->valueblk)
				count++;
		}
	}

	/*
	 * If there are no "remote" values, we're done.
	 */
	if (count == 0) {
		xfs_da_brelse(*trans, bp);
		return(0);
	}

	/*
	 * Allocate storage for a list of all the "remote" value extents.
	 */
	size = count * sizeof(xfs_attr_inactive_list_t);
	list = (xfs_attr_inactive_list_t *)kmem_alloc(size, KM_SLEEP);

	/*
	 * Identify each of the "remote" value extents.
	 */
	lp = list;
	entry = &leaf->entries[0];
	for (i = 0; i < be16_to_cpu(leaf->hdr.count); entry++, i++) {
		if (be16_to_cpu(entry->nameidx) &&
		    ((entry->flags & XFS_ATTR_LOCAL) == 0)) {
			name_rmt = xfs_attr_leaf_name_remote(leaf, i);
			if (name_rmt->valueblk) {
				lp->valueblk = be32_to_cpu(name_rmt->valueblk);
				lp->valuelen = XFS_B_TO_FSB(dp->i_mount,
						    be32_to_cpu(name_rmt->valuelen));
				lp++;
			}
		}
	}
	xfs_da_brelse(*trans, bp);	/* unlock for trans. in freextent() */

	/*
	 * Invalidate each of the "remote" value extents.
	 */
	error = 0;
	for (lp = list, i = 0; i < count; i++, lp++) {
		tmp = xfs_attr_leaf_freextent(trans, dp,
				lp->valueblk, lp->valuelen);

		if (error == 0)
			error = tmp;	/* save only the 1st errno */
	}

	kmem_free((xfs_caddr_t)list);
	return(error);
}

/*
 * Look at all the extents for this logical region,
 * invalidate any buffers that are incore/in transactions.
 */
STATIC int
xfs_attr_leaf_freextent(xfs_trans_t **trans, xfs_inode_t *dp,
				    xfs_dablk_t blkno, int blkcnt)
{
	xfs_bmbt_irec_t map;
	xfs_dablk_t tblkno;
	int tblkcnt, dblkcnt, nmap, error;
	xfs_daddr_t dblkno;
	xfs_buf_t *bp;

	/*
	 * Roll through the "value", invalidating the attribute value's
	 * blocks.
	 */
	tblkno = blkno;
	tblkcnt = blkcnt;
	while (tblkcnt > 0) {
		/*
		 * Try to remember where we decided to put the value.
		 */
		nmap = 1;
<<<<<<< HEAD
		error = xfs_bmapi(*trans, dp, (xfs_fileoff_t)tblkno, tblkcnt,
					XFS_BMAPI_ATTRFORK | XFS_BMAPI_METADATA,
					NULL, 0, &map, &nmap, NULL);
=======
		error = xfs_bmapi_read(dp, (xfs_fileoff_t)tblkno, tblkcnt,
				       &map, &nmap, XFS_BMAPI_ATTRFORK);
>>>>>>> refs/remotes/origin/cm-10.0
		if (error) {
			return(error);
		}
		ASSERT(nmap == 1);
		ASSERT(map.br_startblock != DELAYSTARTBLOCK);

		/*
		 * If it's a hole, these are already unmapped
		 * so there's nothing to invalidate.
		 */
		if (map.br_startblock != HOLESTARTBLOCK) {

			dblkno = XFS_FSB_TO_DADDR(dp->i_mount,
						  map.br_startblock);
			dblkcnt = XFS_FSB_TO_BB(dp->i_mount,
						map.br_blockcount);
			bp = xfs_trans_get_buf(*trans,
					dp->i_mount->m_ddev_targp,
					dblkno, dblkcnt, XBF_LOCK);
<<<<<<< HEAD
=======
			if (!bp)
				return ENOMEM;
>>>>>>> refs/remotes/origin/cm-10.0
			xfs_trans_binval(*trans, bp);
			/*
			 * Roll to next transaction.
			 */
			error = xfs_trans_roll(trans, dp);
			if (error)
				return (error);
		}

		tblkno += map.br_blockcount;
		tblkcnt -= map.br_blockcount;
	}

	return(0);
=======
	return error;
>>>>>>> refs/remotes/origin/master
}
