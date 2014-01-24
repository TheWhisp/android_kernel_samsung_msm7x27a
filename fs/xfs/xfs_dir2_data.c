/*
 * Copyright (c) 2000-2002,2005 Silicon Graphics, Inc.
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
#include "xfs_dir2_data.h"
#include "xfs_dir2_leaf.h"
#include "xfs_dir2_block.h"
#include "xfs_error.h"

=======
#include "xfs_mount.h"
#include "xfs_da_btree.h"
#include "xfs_bmap_btree.h"
#include "xfs_dinode.h"
#include "xfs_inode.h"
#include "xfs_dir2_format.h"
#include "xfs_dir2_priv.h"
#include "xfs_error.h"

STATIC xfs_dir2_data_free_t *
xfs_dir2_data_freefind(xfs_dir2_data_hdr_t *hdr, xfs_dir2_data_unused_t *dup);

>>>>>>> refs/remotes/origin/cm-10.0
#ifdef DEBUG
/*
 * Check the consistency of the data block.
 * The input can also be a block-format directory.
 * Pop an assert if we find anything bad.
 */
void
xfs_dir2_data_check(
	xfs_inode_t		*dp,		/* incore inode pointer */
	xfs_dabuf_t		*bp)		/* data block's buffer */
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
#include "xfs_dir2.h"
#include "xfs_dir2_priv.h"
#include "xfs_error.h"
#include "xfs_trans.h"
#include "xfs_buf_item.h"
#include "xfs_cksum.h"

/*
 * Check the consistency of the data block.
 * The input can also be a block-format directory.
 * Return 0 is the buffer is good, otherwise an error.
 */
int
__xfs_dir3_data_check(
	struct xfs_inode	*dp,		/* incore inode pointer */
	struct xfs_buf		*bp)		/* data block's buffer */
>>>>>>> refs/remotes/origin/master
{
	xfs_dir2_dataptr_t	addr;		/* addr for leaf lookup */
	xfs_dir2_data_free_t	*bf;		/* bestfree table */
	xfs_dir2_block_tail_t	*btp=NULL;	/* block tail */
	int			count;		/* count of entries found */
<<<<<<< HEAD
<<<<<<< HEAD
	xfs_dir2_data_t		*d;		/* data block pointer */
=======
	xfs_dir2_data_hdr_t	*hdr;		/* data block header */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	xfs_dir2_data_hdr_t	*hdr;		/* data block header */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_data_entry_t	*dep;		/* data entry */
	xfs_dir2_data_free_t	*dfp;		/* bestfree entry */
	xfs_dir2_data_unused_t	*dup;		/* unused entry */
	char			*endp;		/* end of useful data */
	int			freeseen;	/* mask of bestfrees seen */
	xfs_dahash_t		hash;		/* hash of current name */
	int			i;		/* leaf index */
	int			lastfree;	/* last entry was unused */
	xfs_dir2_leaf_entry_t	*lep=NULL;	/* block leaf entries */
	xfs_mount_t		*mp;		/* filesystem mount point */
	char			*p;		/* current data position */
	int			stale;		/* count of stale leaves */
	struct xfs_name		name;
<<<<<<< HEAD

	mp = dp->i_mount;
<<<<<<< HEAD
	d = bp->data;
	ASSERT(be32_to_cpu(d->hdr.magic) == XFS_DIR2_DATA_MAGIC ||
	       be32_to_cpu(d->hdr.magic) == XFS_DIR2_BLOCK_MAGIC);
	bf = d->hdr.bestfree;
	p = (char *)d->u;
	if (be32_to_cpu(d->hdr.magic) == XFS_DIR2_BLOCK_MAGIC) {
		btp = xfs_dir2_block_tail_p(mp, (xfs_dir2_block_t *)d);
		lep = xfs_dir2_block_leaf_p(btp);
		endp = (char *)lep;
	} else
		endp = (char *)d + mp->m_dirblksize;
=======
	hdr = bp->data;
	bf = hdr->bestfree;
	p = (char *)(hdr + 1);

	if (hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC)) {
		btp = xfs_dir2_block_tail_p(mp, hdr);
		lep = xfs_dir2_block_leaf_p(btp);
		endp = (char *)lep;
	} else {
		ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC));
		endp = (char *)hdr + mp->m_dirblksize;
	}

>>>>>>> refs/remotes/origin/cm-10.0
	count = lastfree = freeseen = 0;
	/*
	 * Account for zero bestfree entries.
	 */
	if (!bf[0].length) {
		ASSERT(!bf[0].offset);
		freeseen |= 1 << 0;
	}
	if (!bf[1].length) {
		ASSERT(!bf[1].offset);
		freeseen |= 1 << 1;
	}
	if (!bf[2].length) {
		ASSERT(!bf[2].offset);
		freeseen |= 1 << 2;
	}
	ASSERT(be16_to_cpu(bf[0].length) >= be16_to_cpu(bf[1].length));
	ASSERT(be16_to_cpu(bf[1].length) >= be16_to_cpu(bf[2].length));
=======
	const struct xfs_dir_ops *ops;

	mp = bp->b_target->bt_mount;

	/*
	 * We can be passed a null dp here from a verifier, so we need to go the
	 * hard way to get them.
	 */
	ops = xfs_dir_get_ops(mp, dp);

	hdr = bp->b_addr;
	p = (char *)ops->data_entry_p(hdr);

	switch (hdr->magic) {
	case cpu_to_be32(XFS_DIR3_BLOCK_MAGIC):
	case cpu_to_be32(XFS_DIR2_BLOCK_MAGIC):
		btp = xfs_dir2_block_tail_p(mp, hdr);
		lep = xfs_dir2_block_leaf_p(btp);
		endp = (char *)lep;

		/*
		 * The number of leaf entries is limited by the size of the
		 * block and the amount of space used by the data entries.
		 * We don't know how much space is used by the data entries yet,
		 * so just ensure that the count falls somewhere inside the
		 * block right now.
		 */
		XFS_WANT_CORRUPTED_RETURN(be32_to_cpu(btp->count) <
			((char *)btp - p) / sizeof(struct xfs_dir2_leaf_entry));
		break;
	case cpu_to_be32(XFS_DIR3_DATA_MAGIC):
	case cpu_to_be32(XFS_DIR2_DATA_MAGIC):
		endp = (char *)hdr + mp->m_dirblksize;
		break;
	default:
		XFS_ERROR_REPORT("Bad Magic", XFS_ERRLEVEL_LOW, mp);
		return EFSCORRUPTED;
	}

	/*
	 * Account for zero bestfree entries.
	 */
	bf = ops->data_bestfree_p(hdr);
	count = lastfree = freeseen = 0;
	if (!bf[0].length) {
		XFS_WANT_CORRUPTED_RETURN(!bf[0].offset);
		freeseen |= 1 << 0;
	}
	if (!bf[1].length) {
		XFS_WANT_CORRUPTED_RETURN(!bf[1].offset);
		freeseen |= 1 << 1;
	}
	if (!bf[2].length) {
		XFS_WANT_CORRUPTED_RETURN(!bf[2].offset);
		freeseen |= 1 << 2;
	}

	XFS_WANT_CORRUPTED_RETURN(be16_to_cpu(bf[0].length) >=
						be16_to_cpu(bf[1].length));
	XFS_WANT_CORRUPTED_RETURN(be16_to_cpu(bf[1].length) >=
						be16_to_cpu(bf[2].length));
>>>>>>> refs/remotes/origin/master
	/*
	 * Loop over the data/unused entries.
	 */
	while (p < endp) {
		dup = (xfs_dir2_data_unused_t *)p;
		/*
		 * If it's unused, look for the space in the bestfree table.
		 * If we find it, account for that, else make sure it
		 * doesn't need to be there.
		 */
		if (be16_to_cpu(dup->freetag) == XFS_DIR2_DATA_FREE_TAG) {
<<<<<<< HEAD
			ASSERT(lastfree == 0);
			ASSERT(be16_to_cpu(*xfs_dir2_data_unused_tag_p(dup)) ==
<<<<<<< HEAD
			       (char *)dup - (char *)d);
			dfp = xfs_dir2_data_freefind(d, dup);
=======
			       (char *)dup - (char *)hdr);
			dfp = xfs_dir2_data_freefind(hdr, dup);
>>>>>>> refs/remotes/origin/cm-10.0
			if (dfp) {
				i = (int)(dfp - bf);
				ASSERT((freeseen & (1 << i)) == 0);
				freeseen |= 1 << i;
			} else {
				ASSERT(be16_to_cpu(dup->length) <=
				       be16_to_cpu(bf[2].length));
=======
			XFS_WANT_CORRUPTED_RETURN(lastfree == 0);
			XFS_WANT_CORRUPTED_RETURN(
				be16_to_cpu(*xfs_dir2_data_unused_tag_p(dup)) ==
					       (char *)dup - (char *)hdr);
			dfp = xfs_dir2_data_freefind(hdr, bf, dup);
			if (dfp) {
				i = (int)(dfp - bf);
				XFS_WANT_CORRUPTED_RETURN(
					(freeseen & (1 << i)) == 0);
				freeseen |= 1 << i;
			} else {
				XFS_WANT_CORRUPTED_RETURN(
					be16_to_cpu(dup->length) <=
						be16_to_cpu(bf[2].length));
>>>>>>> refs/remotes/origin/master
			}
			p += be16_to_cpu(dup->length);
			lastfree = 1;
			continue;
		}
		/*
		 * It's a real entry.  Validate the fields.
		 * If this is a block directory then make sure it's
		 * in the leaf section of the block.
		 * The linear search is crude but this is DEBUG code.
		 */
		dep = (xfs_dir2_data_entry_t *)p;
<<<<<<< HEAD
		ASSERT(dep->namelen != 0);
		ASSERT(xfs_dir_ino_validate(mp, be64_to_cpu(dep->inumber)) == 0);
		ASSERT(be16_to_cpu(*xfs_dir2_data_entry_tag_p(dep)) ==
<<<<<<< HEAD
		       (char *)dep - (char *)d);
		count++;
		lastfree = 0;
		if (be32_to_cpu(d->hdr.magic) == XFS_DIR2_BLOCK_MAGIC) {
			addr = xfs_dir2_db_off_to_dataptr(mp, mp->m_dirdatablk,
				(xfs_dir2_data_aoff_t)
				((char *)dep - (char *)d));
=======
		       (char *)dep - (char *)hdr);
		count++;
		lastfree = 0;
		if (hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC)) {
			addr = xfs_dir2_db_off_to_dataptr(mp, mp->m_dirdatablk,
				(xfs_dir2_data_aoff_t)
				((char *)dep - (char *)hdr));
>>>>>>> refs/remotes/origin/cm-10.0
=======
		XFS_WANT_CORRUPTED_RETURN(dep->namelen != 0);
		XFS_WANT_CORRUPTED_RETURN(
			!xfs_dir_ino_validate(mp, be64_to_cpu(dep->inumber)));
		XFS_WANT_CORRUPTED_RETURN(
			be16_to_cpu(*ops->data_entry_tag_p(dep)) ==
					       (char *)dep - (char *)hdr);
		XFS_WANT_CORRUPTED_RETURN(
				ops->data_get_ftype(dep) < XFS_DIR3_FT_MAX);
		count++;
		lastfree = 0;
		if (hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC) ||
		    hdr->magic == cpu_to_be32(XFS_DIR3_BLOCK_MAGIC)) {
			addr = xfs_dir2_db_off_to_dataptr(mp, mp->m_dirdatablk,
				(xfs_dir2_data_aoff_t)
				((char *)dep - (char *)hdr));
>>>>>>> refs/remotes/origin/master
			name.name = dep->name;
			name.len = dep->namelen;
			hash = mp->m_dirnameops->hashname(&name);
			for (i = 0; i < be32_to_cpu(btp->count); i++) {
				if (be32_to_cpu(lep[i].address) == addr &&
				    be32_to_cpu(lep[i].hashval) == hash)
					break;
			}
<<<<<<< HEAD
			ASSERT(i < be32_to_cpu(btp->count));
		}
		p += xfs_dir2_data_entsize(dep->namelen);
=======
			XFS_WANT_CORRUPTED_RETURN(i < be32_to_cpu(btp->count));
		}
		p += ops->data_entsize(dep->namelen);
>>>>>>> refs/remotes/origin/master
	}
	/*
	 * Need to have seen all the entries and all the bestfree slots.
	 */
<<<<<<< HEAD
	ASSERT(freeseen == 7);
<<<<<<< HEAD
	if (be32_to_cpu(d->hdr.magic) == XFS_DIR2_BLOCK_MAGIC) {
		for (i = stale = 0; i < be32_to_cpu(btp->count); i++) {
			if (be32_to_cpu(lep[i].address) == XFS_DIR2_NULL_DATAPTR)
=======
	if (hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC)) {
		for (i = stale = 0; i < be32_to_cpu(btp->count); i++) {
			if (lep[i].address ==
			    cpu_to_be32(XFS_DIR2_NULL_DATAPTR))
>>>>>>> refs/remotes/origin/cm-10.0
				stale++;
			if (i > 0)
				ASSERT(be32_to_cpu(lep[i].hashval) >= be32_to_cpu(lep[i - 1].hashval));
		}
		ASSERT(count == be32_to_cpu(btp->count) - be32_to_cpu(btp->stale));
		ASSERT(stale == be32_to_cpu(btp->stale));
	}
}
#endif
=======
	XFS_WANT_CORRUPTED_RETURN(freeseen == 7);
	if (hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC) ||
	    hdr->magic == cpu_to_be32(XFS_DIR3_BLOCK_MAGIC)) {
		for (i = stale = 0; i < be32_to_cpu(btp->count); i++) {
			if (lep[i].address ==
			    cpu_to_be32(XFS_DIR2_NULL_DATAPTR))
				stale++;
			if (i > 0)
				XFS_WANT_CORRUPTED_RETURN(
					be32_to_cpu(lep[i].hashval) >=
						be32_to_cpu(lep[i - 1].hashval));
		}
		XFS_WANT_CORRUPTED_RETURN(count ==
			be32_to_cpu(btp->count) - be32_to_cpu(btp->stale));
		XFS_WANT_CORRUPTED_RETURN(stale == be32_to_cpu(btp->stale));
	}
	return 0;
}

static bool
xfs_dir3_data_verify(
	struct xfs_buf		*bp)
{
	struct xfs_mount	*mp = bp->b_target->bt_mount;
	struct xfs_dir3_blk_hdr	*hdr3 = bp->b_addr;

	if (xfs_sb_version_hascrc(&mp->m_sb)) {
		if (hdr3->magic != cpu_to_be32(XFS_DIR3_DATA_MAGIC))
			return false;
		if (!uuid_equal(&hdr3->uuid, &mp->m_sb.sb_uuid))
			return false;
		if (be64_to_cpu(hdr3->blkno) != bp->b_bn)
			return false;
	} else {
		if (hdr3->magic != cpu_to_be32(XFS_DIR2_DATA_MAGIC))
			return false;
	}
	if (__xfs_dir3_data_check(NULL, bp))
		return false;
	return true;
}

/*
 * Readahead of the first block of the directory when it is opened is completely
 * oblivious to the format of the directory. Hence we can either get a block
 * format buffer or a data format buffer on readahead.
 */
static void
xfs_dir3_data_reada_verify(
	struct xfs_buf		*bp)
{
	struct xfs_mount	*mp = bp->b_target->bt_mount;
	struct xfs_dir2_data_hdr *hdr = bp->b_addr;

	switch (hdr->magic) {
	case cpu_to_be32(XFS_DIR2_BLOCK_MAGIC):
	case cpu_to_be32(XFS_DIR3_BLOCK_MAGIC):
		bp->b_ops = &xfs_dir3_block_buf_ops;
		bp->b_ops->verify_read(bp);
		return;
	case cpu_to_be32(XFS_DIR2_DATA_MAGIC):
	case cpu_to_be32(XFS_DIR3_DATA_MAGIC):
		xfs_dir3_data_verify(bp);
		return;
	default:
		XFS_CORRUPTION_ERROR(__func__, XFS_ERRLEVEL_LOW, mp, hdr);
		xfs_buf_ioerror(bp, EFSCORRUPTED);
		break;
	}
}

static void
xfs_dir3_data_read_verify(
	struct xfs_buf	*bp)
{
	struct xfs_mount	*mp = bp->b_target->bt_mount;

	if ((xfs_sb_version_hascrc(&mp->m_sb) &&
	     !xfs_verify_cksum(bp->b_addr, BBTOB(bp->b_length),
					  XFS_DIR3_DATA_CRC_OFF)) ||
	    !xfs_dir3_data_verify(bp)) {
		XFS_CORRUPTION_ERROR(__func__, XFS_ERRLEVEL_LOW, mp, bp->b_addr);
		xfs_buf_ioerror(bp, EFSCORRUPTED);
	}
}

static void
xfs_dir3_data_write_verify(
	struct xfs_buf	*bp)
{
	struct xfs_mount	*mp = bp->b_target->bt_mount;
	struct xfs_buf_log_item	*bip = bp->b_fspriv;
	struct xfs_dir3_blk_hdr	*hdr3 = bp->b_addr;

	if (!xfs_dir3_data_verify(bp)) {
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

const struct xfs_buf_ops xfs_dir3_data_buf_ops = {
	.verify_read = xfs_dir3_data_read_verify,
	.verify_write = xfs_dir3_data_write_verify,
};

static const struct xfs_buf_ops xfs_dir3_data_reada_buf_ops = {
	.verify_read = xfs_dir3_data_reada_verify,
	.verify_write = xfs_dir3_data_write_verify,
};


int
xfs_dir3_data_read(
	struct xfs_trans	*tp,
	struct xfs_inode	*dp,
	xfs_dablk_t		bno,
	xfs_daddr_t		mapped_bno,
	struct xfs_buf		**bpp)
{
	int			err;

	err = xfs_da_read_buf(tp, dp, bno, mapped_bno, bpp,
				XFS_DATA_FORK, &xfs_dir3_data_buf_ops);
	if (!err && tp)
		xfs_trans_buf_set_type(tp, *bpp, XFS_BLFT_DIR_DATA_BUF);
	return err;
}

int
xfs_dir3_data_readahead(
	struct xfs_trans	*tp,
	struct xfs_inode	*dp,
	xfs_dablk_t		bno,
	xfs_daddr_t		mapped_bno)
{
	return xfs_da_reada_buf(tp, dp, bno, mapped_bno,
				XFS_DATA_FORK, &xfs_dir3_data_reada_buf_ops);
}
>>>>>>> refs/remotes/origin/master

/*
 * Given a data block and an unused entry from that block,
 * return the bestfree entry if any that corresponds to it.
 */
<<<<<<< HEAD
<<<<<<< HEAD
xfs_dir2_data_free_t *
xfs_dir2_data_freefind(
	xfs_dir2_data_t		*d,		/* data block */
=======
STATIC xfs_dir2_data_free_t *
xfs_dir2_data_freefind(
	xfs_dir2_data_hdr_t	*hdr,		/* data block */
>>>>>>> refs/remotes/origin/cm-10.0
	xfs_dir2_data_unused_t	*dup)		/* data unused entry */
{
	xfs_dir2_data_free_t	*dfp;		/* bestfree entry */
	xfs_dir2_data_aoff_t	off;		/* offset value needed */
#if defined(DEBUG) && defined(__KERNEL__)
=======
xfs_dir2_data_free_t *
xfs_dir2_data_freefind(
	struct xfs_dir2_data_hdr *hdr,		/* data block header */
	struct xfs_dir2_data_free *bf,		/* bestfree table pointer */
	struct xfs_dir2_data_unused *dup)	/* unused space */
{
	xfs_dir2_data_free_t	*dfp;		/* bestfree entry */
	xfs_dir2_data_aoff_t	off;		/* offset value needed */
#ifdef DEBUG
>>>>>>> refs/remotes/origin/master
	int			matched;	/* matched the value */
	int			seenzero;	/* saw a 0 bestfree entry */
#endif

<<<<<<< HEAD
<<<<<<< HEAD
	off = (xfs_dir2_data_aoff_t)((char *)dup - (char *)d);
=======
	off = (xfs_dir2_data_aoff_t)((char *)dup - (char *)hdr);
>>>>>>> refs/remotes/origin/cm-10.0
#if defined(DEBUG) && defined(__KERNEL__)
=======
	off = (xfs_dir2_data_aoff_t)((char *)dup - (char *)hdr);

#ifdef DEBUG
>>>>>>> refs/remotes/origin/master
	/*
	 * Validate some consistency in the bestfree table.
	 * Check order, non-overlapping entries, and if we find the
	 * one we're looking for it has to be exact.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	ASSERT(be32_to_cpu(d->hdr.magic) == XFS_DIR2_DATA_MAGIC ||
	       be32_to_cpu(d->hdr.magic) == XFS_DIR2_BLOCK_MAGIC);
	for (dfp = &d->hdr.bestfree[0], seenzero = matched = 0;
	     dfp < &d->hdr.bestfree[XFS_DIR2_DATA_FD_COUNT];
=======
	ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC));
	for (dfp = &hdr->bestfree[0], seenzero = matched = 0;
	     dfp < &hdr->bestfree[XFS_DIR2_DATA_FD_COUNT];
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR3_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR3_BLOCK_MAGIC));
	for (dfp = &bf[0], seenzero = matched = 0;
	     dfp < &bf[XFS_DIR2_DATA_FD_COUNT];
>>>>>>> refs/remotes/origin/master
	     dfp++) {
		if (!dfp->offset) {
			ASSERT(!dfp->length);
			seenzero = 1;
			continue;
		}
		ASSERT(seenzero == 0);
		if (be16_to_cpu(dfp->offset) == off) {
			matched = 1;
			ASSERT(dfp->length == dup->length);
		} else if (off < be16_to_cpu(dfp->offset))
			ASSERT(off + be16_to_cpu(dup->length) <= be16_to_cpu(dfp->offset));
		else
			ASSERT(be16_to_cpu(dfp->offset) + be16_to_cpu(dfp->length) <= off);
		ASSERT(matched || be16_to_cpu(dfp->length) >= be16_to_cpu(dup->length));
<<<<<<< HEAD
<<<<<<< HEAD
		if (dfp > &d->hdr.bestfree[0])
=======
		if (dfp > &hdr->bestfree[0])
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (dfp > &bf[0])
>>>>>>> refs/remotes/origin/master
			ASSERT(be16_to_cpu(dfp[-1].length) >= be16_to_cpu(dfp[0].length));
	}
#endif
	/*
	 * If this is smaller than the smallest bestfree entry,
	 * it can't be there since they're sorted.
	 */
	if (be16_to_cpu(dup->length) <
<<<<<<< HEAD
<<<<<<< HEAD
	    be16_to_cpu(d->hdr.bestfree[XFS_DIR2_DATA_FD_COUNT - 1].length))
=======
	    be16_to_cpu(hdr->bestfree[XFS_DIR2_DATA_FD_COUNT - 1].length))
>>>>>>> refs/remotes/origin/cm-10.0
=======
	    be16_to_cpu(bf[XFS_DIR2_DATA_FD_COUNT - 1].length))
>>>>>>> refs/remotes/origin/master
		return NULL;
	/*
	 * Look at the three bestfree entries for our guy.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	for (dfp = &d->hdr.bestfree[0];
	     dfp < &d->hdr.bestfree[XFS_DIR2_DATA_FD_COUNT];
=======
	for (dfp = &hdr->bestfree[0];
	     dfp < &hdr->bestfree[XFS_DIR2_DATA_FD_COUNT];
>>>>>>> refs/remotes/origin/cm-10.0
	     dfp++) {
=======
	for (dfp = &bf[0]; dfp < &bf[XFS_DIR2_DATA_FD_COUNT]; dfp++) {
>>>>>>> refs/remotes/origin/master
		if (!dfp->offset)
			return NULL;
		if (be16_to_cpu(dfp->offset) == off)
			return dfp;
	}
	/*
	 * Didn't find it.  This only happens if there are duplicate lengths.
	 */
	return NULL;
}

/*
 * Insert an unused-space entry into the bestfree table.
 */
xfs_dir2_data_free_t *				/* entry inserted */
xfs_dir2_data_freeinsert(
<<<<<<< HEAD
<<<<<<< HEAD
	xfs_dir2_data_t		*d,		/* data block pointer */
=======
	xfs_dir2_data_hdr_t	*hdr,		/* data block pointer */
>>>>>>> refs/remotes/origin/cm-10.0
	xfs_dir2_data_unused_t	*dup,		/* unused space */
	int			*loghead)	/* log the data header (out) */
{
	xfs_dir2_data_free_t	*dfp;		/* bestfree table pointer */
	xfs_dir2_data_free_t	new;		/* new bestfree entry */

#ifdef __KERNEL__
<<<<<<< HEAD
	ASSERT(be32_to_cpu(d->hdr.magic) == XFS_DIR2_DATA_MAGIC ||
	       be32_to_cpu(d->hdr.magic) == XFS_DIR2_BLOCK_MAGIC);
#endif
	dfp = d->hdr.bestfree;
	new.length = dup->length;
	new.offset = cpu_to_be16((char *)dup - (char *)d);
=======
	ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC));
#endif
	dfp = hdr->bestfree;
	new.length = dup->length;
	new.offset = cpu_to_be16((char *)dup - (char *)hdr);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct xfs_dir2_data_hdr *hdr,		/* data block pointer */
	struct xfs_dir2_data_free *dfp,		/* bestfree table pointer */
	struct xfs_dir2_data_unused *dup,	/* unused space */
	int			*loghead)	/* log the data header (out) */
{
	xfs_dir2_data_free_t	new;		/* new bestfree entry */

	ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR3_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR3_BLOCK_MAGIC));

	new.length = dup->length;
	new.offset = cpu_to_be16((char *)dup - (char *)hdr);

>>>>>>> refs/remotes/origin/master
	/*
	 * Insert at position 0, 1, or 2; or not at all.
	 */
	if (be16_to_cpu(new.length) > be16_to_cpu(dfp[0].length)) {
		dfp[2] = dfp[1];
		dfp[1] = dfp[0];
		dfp[0] = new;
		*loghead = 1;
		return &dfp[0];
	}
	if (be16_to_cpu(new.length) > be16_to_cpu(dfp[1].length)) {
		dfp[2] = dfp[1];
		dfp[1] = new;
		*loghead = 1;
		return &dfp[1];
	}
	if (be16_to_cpu(new.length) > be16_to_cpu(dfp[2].length)) {
		dfp[2] = new;
		*loghead = 1;
		return &dfp[2];
	}
	return NULL;
}

/*
 * Remove a bestfree entry from the table.
 */
STATIC void
xfs_dir2_data_freeremove(
<<<<<<< HEAD
<<<<<<< HEAD
	xfs_dir2_data_t		*d,		/* data block pointer */
=======
	xfs_dir2_data_hdr_t	*hdr,		/* data block header */
>>>>>>> refs/remotes/origin/cm-10.0
	xfs_dir2_data_free_t	*dfp,		/* bestfree entry pointer */
	int			*loghead)	/* out: log data header */
{
#ifdef __KERNEL__
<<<<<<< HEAD
	ASSERT(be32_to_cpu(d->hdr.magic) == XFS_DIR2_DATA_MAGIC ||
	       be32_to_cpu(d->hdr.magic) == XFS_DIR2_BLOCK_MAGIC);
=======
	ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
#endif
	/*
	 * It's the first entry, slide the next 2 up.
	 */
<<<<<<< HEAD
	if (dfp == &d->hdr.bestfree[0]) {
		d->hdr.bestfree[0] = d->hdr.bestfree[1];
		d->hdr.bestfree[1] = d->hdr.bestfree[2];
=======
	if (dfp == &hdr->bestfree[0]) {
		hdr->bestfree[0] = hdr->bestfree[1];
		hdr->bestfree[1] = hdr->bestfree[2];
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct xfs_dir2_data_hdr *hdr,		/* data block header */
	struct xfs_dir2_data_free *bf,		/* bestfree table pointer */
	struct xfs_dir2_data_free *dfp,		/* bestfree entry pointer */
	int			*loghead)	/* out: log data header */
{

	ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR3_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR3_BLOCK_MAGIC));

	/*
	 * It's the first entry, slide the next 2 up.
	 */
	if (dfp == &bf[0]) {
		bf[0] = bf[1];
		bf[1] = bf[2];
>>>>>>> refs/remotes/origin/master
	}
	/*
	 * It's the second entry, slide the 3rd entry up.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	else if (dfp == &d->hdr.bestfree[1])
		d->hdr.bestfree[1] = d->hdr.bestfree[2];
=======
	else if (dfp == &hdr->bestfree[1])
		hdr->bestfree[1] = hdr->bestfree[2];
>>>>>>> refs/remotes/origin/cm-10.0
=======
	else if (dfp == &bf[1])
		bf[1] = bf[2];
>>>>>>> refs/remotes/origin/master
	/*
	 * Must be the last entry.
	 */
	else
<<<<<<< HEAD
<<<<<<< HEAD
		ASSERT(dfp == &d->hdr.bestfree[2]);
	/*
	 * Clear the 3rd entry, must be zero now.
	 */
	d->hdr.bestfree[2].length = 0;
	d->hdr.bestfree[2].offset = 0;
=======
		ASSERT(dfp == &hdr->bestfree[2]);
	/*
	 * Clear the 3rd entry, must be zero now.
	 */
	hdr->bestfree[2].length = 0;
	hdr->bestfree[2].offset = 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ASSERT(dfp == &bf[2]);
	/*
	 * Clear the 3rd entry, must be zero now.
	 */
	bf[2].length = 0;
	bf[2].offset = 0;
>>>>>>> refs/remotes/origin/master
	*loghead = 1;
}

/*
 * Given a data block, reconstruct its bestfree map.
 */
void
xfs_dir2_data_freescan(
<<<<<<< HEAD
	xfs_mount_t		*mp,		/* filesystem mount point */
<<<<<<< HEAD
	xfs_dir2_data_t		*d,		/* data block pointer */
=======
	xfs_dir2_data_hdr_t	*hdr,		/* data block header */
>>>>>>> refs/remotes/origin/cm-10.0
	int			*loghead)	/* out: log data header */
=======
	struct xfs_inode	*dp,
	struct xfs_dir2_data_hdr *hdr,
	int			*loghead)
>>>>>>> refs/remotes/origin/master
{
	xfs_dir2_block_tail_t	*btp;		/* block tail */
	xfs_dir2_data_entry_t	*dep;		/* active data entry */
	xfs_dir2_data_unused_t	*dup;		/* unused data entry */
<<<<<<< HEAD
	char			*endp;		/* end of block's data */
	char			*p;		/* current entry pointer */

#ifdef __KERNEL__
<<<<<<< HEAD
	ASSERT(be32_to_cpu(d->hdr.magic) == XFS_DIR2_DATA_MAGIC ||
	       be32_to_cpu(d->hdr.magic) == XFS_DIR2_BLOCK_MAGIC);
=======
	ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC));
>>>>>>> refs/remotes/origin/cm-10.0
#endif
	/*
	 * Start by clearing the table.
	 */
<<<<<<< HEAD
	memset(d->hdr.bestfree, 0, sizeof(d->hdr.bestfree));
=======
	memset(hdr->bestfree, 0, sizeof(hdr->bestfree));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct xfs_dir2_data_free *bf;
	char			*endp;		/* end of block's data */
	char			*p;		/* current entry pointer */

	ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR3_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR3_BLOCK_MAGIC));

	/*
	 * Start by clearing the table.
	 */
	bf = dp->d_ops->data_bestfree_p(hdr);
	memset(bf, 0, sizeof(*bf) * XFS_DIR2_DATA_FD_COUNT);
>>>>>>> refs/remotes/origin/master
	*loghead = 1;
	/*
	 * Set up pointers.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	p = (char *)d->u;
	if (be32_to_cpu(d->hdr.magic) == XFS_DIR2_BLOCK_MAGIC) {
		btp = xfs_dir2_block_tail_p(mp, (xfs_dir2_block_t *)d);
		endp = (char *)xfs_dir2_block_leaf_p(btp);
	} else
		endp = (char *)d + mp->m_dirblksize;
=======
	p = (char *)(hdr + 1);
	if (hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC)) {
		btp = xfs_dir2_block_tail_p(mp, hdr);
		endp = (char *)xfs_dir2_block_leaf_p(btp);
	} else
		endp = (char *)hdr + mp->m_dirblksize;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	p = (char *)dp->d_ops->data_entry_p(hdr);
	if (hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC) ||
	    hdr->magic == cpu_to_be32(XFS_DIR3_BLOCK_MAGIC)) {
		btp = xfs_dir2_block_tail_p(dp->i_mount, hdr);
		endp = (char *)xfs_dir2_block_leaf_p(btp);
	} else
		endp = (char *)hdr + dp->i_mount->m_dirblksize;
>>>>>>> refs/remotes/origin/master
	/*
	 * Loop over the block's entries.
	 */
	while (p < endp) {
		dup = (xfs_dir2_data_unused_t *)p;
		/*
		 * If it's a free entry, insert it.
		 */
		if (be16_to_cpu(dup->freetag) == XFS_DIR2_DATA_FREE_TAG) {
<<<<<<< HEAD
<<<<<<< HEAD
			ASSERT((char *)dup - (char *)d ==
			       be16_to_cpu(*xfs_dir2_data_unused_tag_p(dup)));
			xfs_dir2_data_freeinsert(d, dup, loghead);
=======
			ASSERT((char *)dup - (char *)hdr ==
			       be16_to_cpu(*xfs_dir2_data_unused_tag_p(dup)));
			xfs_dir2_data_freeinsert(hdr, dup, loghead);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			ASSERT((char *)dup - (char *)hdr ==
			       be16_to_cpu(*xfs_dir2_data_unused_tag_p(dup)));
			xfs_dir2_data_freeinsert(hdr, bf, dup, loghead);
>>>>>>> refs/remotes/origin/master
			p += be16_to_cpu(dup->length);
		}
		/*
		 * For active entries, check their tags and skip them.
		 */
		else {
			dep = (xfs_dir2_data_entry_t *)p;
<<<<<<< HEAD
<<<<<<< HEAD
			ASSERT((char *)dep - (char *)d ==
=======
			ASSERT((char *)dep - (char *)hdr ==
>>>>>>> refs/remotes/origin/cm-10.0
			       be16_to_cpu(*xfs_dir2_data_entry_tag_p(dep)));
			p += xfs_dir2_data_entsize(dep->namelen);
=======
			ASSERT((char *)dep - (char *)hdr ==
			       be16_to_cpu(*dp->d_ops->data_entry_tag_p(dep)));
			p += dp->d_ops->data_entsize(dep->namelen);
>>>>>>> refs/remotes/origin/master
		}
	}
}

/*
 * Initialize a data block at the given block number in the directory.
 * Give back the buffer for the created block.
 */
int						/* error */
<<<<<<< HEAD
xfs_dir2_data_init(
	xfs_da_args_t		*args,		/* directory operation args */
	xfs_dir2_db_t		blkno,		/* logical dir block number */
	xfs_dabuf_t		**bpp)		/* output block buffer */
{
	xfs_dabuf_t		*bp;		/* block buffer */
<<<<<<< HEAD
	xfs_dir2_data_t		*d;		/* pointer to block */
=======
	xfs_dir2_data_hdr_t	*hdr;		/* data block header */
>>>>>>> refs/remotes/origin/cm-10.0
	xfs_inode_t		*dp;		/* incore directory inode */
	xfs_dir2_data_unused_t	*dup;		/* unused entry pointer */
=======
xfs_dir3_data_init(
	xfs_da_args_t		*args,		/* directory operation args */
	xfs_dir2_db_t		blkno,		/* logical dir block number */
	struct xfs_buf		**bpp)		/* output block buffer */
{
	struct xfs_buf		*bp;		/* block buffer */
	xfs_dir2_data_hdr_t	*hdr;		/* data block header */
	xfs_inode_t		*dp;		/* incore directory inode */
	xfs_dir2_data_unused_t	*dup;		/* unused entry pointer */
	struct xfs_dir2_data_free *bf;
>>>>>>> refs/remotes/origin/master
	int			error;		/* error return value */
	int			i;		/* bestfree index */
	xfs_mount_t		*mp;		/* filesystem mount point */
	xfs_trans_t		*tp;		/* transaction pointer */
	int                     t;              /* temp */

	dp = args->dp;
	mp = dp->i_mount;
	tp = args->trans;
	/*
	 * Get the buffer set up for the block.
	 */
	error = xfs_da_get_buf(tp, dp, xfs_dir2_db_to_da(mp, blkno), -1, &bp,
		XFS_DATA_FORK);
<<<<<<< HEAD
	if (error) {
		return error;
	}
	ASSERT(bp != NULL);
<<<<<<< HEAD
	/*
	 * Initialize the header.
	 */
	d = bp->data;
	d->hdr.magic = cpu_to_be32(XFS_DIR2_DATA_MAGIC);
	d->hdr.bestfree[0].offset = cpu_to_be16(sizeof(d->hdr));
	for (i = 1; i < XFS_DIR2_DATA_FD_COUNT; i++) {
		d->hdr.bestfree[i].length = 0;
		d->hdr.bestfree[i].offset = 0;
	}
	/*
	 * Set up an unused entry for the block's body.
	 */
	dup = &d->u[0].unused;
	dup->freetag = cpu_to_be16(XFS_DIR2_DATA_FREE_TAG);

	t=mp->m_dirblksize - (uint)sizeof(d->hdr);
	d->hdr.bestfree[0].length = cpu_to_be16(t);
	dup->length = cpu_to_be16(t);
	*xfs_dir2_data_unused_tag_p(dup) = cpu_to_be16((char *)dup - (char *)d);
=======

	/*
	 * Initialize the header.
	 */
	hdr = bp->data;
	hdr->magic = cpu_to_be32(XFS_DIR2_DATA_MAGIC);
	hdr->bestfree[0].offset = cpu_to_be16(sizeof(*hdr));
	for (i = 1; i < XFS_DIR2_DATA_FD_COUNT; i++) {
		hdr->bestfree[i].length = 0;
		hdr->bestfree[i].offset = 0;
=======
	if (error)
		return error;
	bp->b_ops = &xfs_dir3_data_buf_ops;
	xfs_trans_buf_set_type(tp, bp, XFS_BLFT_DIR_DATA_BUF);

	/*
	 * Initialize the header.
	 */
	hdr = bp->b_addr;
	if (xfs_sb_version_hascrc(&mp->m_sb)) {
		struct xfs_dir3_blk_hdr *hdr3 = bp->b_addr;

		memset(hdr3, 0, sizeof(*hdr3));
		hdr3->magic = cpu_to_be32(XFS_DIR3_DATA_MAGIC);
		hdr3->blkno = cpu_to_be64(bp->b_bn);
		hdr3->owner = cpu_to_be64(dp->i_ino);
		uuid_copy(&hdr3->uuid, &mp->m_sb.sb_uuid);

	} else
		hdr->magic = cpu_to_be32(XFS_DIR2_DATA_MAGIC);

	bf = dp->d_ops->data_bestfree_p(hdr);
	bf[0].offset = cpu_to_be16(dp->d_ops->data_entry_offset);
	for (i = 1; i < XFS_DIR2_DATA_FD_COUNT; i++) {
		bf[i].length = 0;
		bf[i].offset = 0;
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * Set up an unused entry for the block's body.
	 */
<<<<<<< HEAD
	dup = (xfs_dir2_data_unused_t *)(hdr + 1);
	dup->freetag = cpu_to_be16(XFS_DIR2_DATA_FREE_TAG);

	t = mp->m_dirblksize - (uint)sizeof(*hdr);
	hdr->bestfree[0].length = cpu_to_be16(t);
	dup->length = cpu_to_be16(t);
	*xfs_dir2_data_unused_tag_p(dup) = cpu_to_be16((char *)dup - (char *)hdr);
>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * Log it and return it.
	 */
	xfs_dir2_data_log_header(tp, bp);
=======
	dup = dp->d_ops->data_unused_p(hdr);
	dup->freetag = cpu_to_be16(XFS_DIR2_DATA_FREE_TAG);

	t = mp->m_dirblksize - (uint)dp->d_ops->data_entry_offset;
	bf[0].length = cpu_to_be16(t);
	dup->length = cpu_to_be16(t);
	*xfs_dir2_data_unused_tag_p(dup) = cpu_to_be16((char *)dup - (char *)hdr);
	/*
	 * Log it and return it.
	 */
	xfs_dir2_data_log_header(tp, dp, bp);
>>>>>>> refs/remotes/origin/master
	xfs_dir2_data_log_unused(tp, bp, dup);
	*bpp = bp;
	return 0;
}

/*
 * Log an active data entry from the block.
 */
void
xfs_dir2_data_log_entry(
<<<<<<< HEAD
	xfs_trans_t		*tp,		/* transaction pointer */
	xfs_dabuf_t		*bp,		/* block buffer */
	xfs_dir2_data_entry_t	*dep)		/* data entry pointer */
{
<<<<<<< HEAD
	xfs_dir2_data_t		*d;		/* data block pointer */

	d = bp->data;
	ASSERT(be32_to_cpu(d->hdr.magic) == XFS_DIR2_DATA_MAGIC ||
	       be32_to_cpu(d->hdr.magic) == XFS_DIR2_BLOCK_MAGIC);
	xfs_da_log_buf(tp, bp, (uint)((char *)dep - (char *)d),
		(uint)((char *)(xfs_dir2_data_entry_tag_p(dep) + 1) -
		       (char *)d - 1));
=======
	xfs_dir2_data_hdr_t	*hdr = bp->data;

	ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC));

	xfs_da_log_buf(tp, bp, (uint)((char *)dep - (char *)hdr),
		(uint)((char *)(xfs_dir2_data_entry_tag_p(dep) + 1) -
		       (char *)hdr - 1));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct xfs_trans	*tp,
	struct xfs_inode	*dp,
	struct xfs_buf		*bp,
	xfs_dir2_data_entry_t	*dep)		/* data entry pointer */
{
	struct xfs_dir2_data_hdr *hdr = bp->b_addr;

	ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR3_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR3_BLOCK_MAGIC));

	xfs_trans_log_buf(tp, bp, (uint)((char *)dep - (char *)hdr),
		(uint)((char *)(dp->d_ops->data_entry_tag_p(dep) + 1) -
		       (char *)hdr - 1));
>>>>>>> refs/remotes/origin/master
}

/*
 * Log a data block header.
 */
void
xfs_dir2_data_log_header(
<<<<<<< HEAD
	xfs_trans_t		*tp,		/* transaction pointer */
	xfs_dabuf_t		*bp)		/* block buffer */
{
<<<<<<< HEAD
	xfs_dir2_data_t		*d;		/* data block pointer */

	d = bp->data;
	ASSERT(be32_to_cpu(d->hdr.magic) == XFS_DIR2_DATA_MAGIC ||
	       be32_to_cpu(d->hdr.magic) == XFS_DIR2_BLOCK_MAGIC);
	xfs_da_log_buf(tp, bp, (uint)((char *)&d->hdr - (char *)d),
		(uint)(sizeof(d->hdr) - 1));
=======
	xfs_dir2_data_hdr_t	*hdr = bp->data;

	ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC));

	xfs_da_log_buf(tp, bp, 0, sizeof(*hdr) - 1);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct xfs_trans	*tp,
	struct xfs_inode	*dp,
	struct xfs_buf		*bp)
{
#ifdef DEBUG
	struct xfs_dir2_data_hdr *hdr = bp->b_addr;

	ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR3_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR3_BLOCK_MAGIC));
#endif

	xfs_trans_log_buf(tp, bp, 0, dp->d_ops->data_entry_offset - 1);
>>>>>>> refs/remotes/origin/master
}

/*
 * Log a data unused entry.
 */
void
xfs_dir2_data_log_unused(
<<<<<<< HEAD
	xfs_trans_t		*tp,		/* transaction pointer */
	xfs_dabuf_t		*bp,		/* block buffer */
	xfs_dir2_data_unused_t	*dup)		/* data unused pointer */
{
<<<<<<< HEAD
	xfs_dir2_data_t		*d;		/* data block pointer */

	d = bp->data;
	ASSERT(be32_to_cpu(d->hdr.magic) == XFS_DIR2_DATA_MAGIC ||
	       be32_to_cpu(d->hdr.magic) == XFS_DIR2_BLOCK_MAGIC);
	/*
	 * Log the first part of the unused entry.
	 */
	xfs_da_log_buf(tp, bp, (uint)((char *)dup - (char *)d),
		(uint)((char *)&dup->length + sizeof(dup->length) -
		       1 - (char *)d));
=======
	xfs_dir2_data_hdr_t	*hdr = bp->data;

	ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC));
=======
	struct xfs_trans	*tp,
	struct xfs_buf		*bp,
	xfs_dir2_data_unused_t	*dup)		/* data unused pointer */
{
	xfs_dir2_data_hdr_t	*hdr = bp->b_addr;

	ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR3_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR3_BLOCK_MAGIC));
>>>>>>> refs/remotes/origin/master

	/*
	 * Log the first part of the unused entry.
	 */
<<<<<<< HEAD
	xfs_da_log_buf(tp, bp, (uint)((char *)dup - (char *)hdr),
		(uint)((char *)&dup->length + sizeof(dup->length) -
		       1 - (char *)hdr));
>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * Log the end (tag) of the unused entry.
	 */
	xfs_da_log_buf(tp, bp,
<<<<<<< HEAD
		(uint)((char *)xfs_dir2_data_unused_tag_p(dup) - (char *)d),
		(uint)((char *)xfs_dir2_data_unused_tag_p(dup) - (char *)d +
=======
		(uint)((char *)xfs_dir2_data_unused_tag_p(dup) - (char *)hdr),
		(uint)((char *)xfs_dir2_data_unused_tag_p(dup) - (char *)hdr +
>>>>>>> refs/remotes/origin/cm-10.0
=======
	xfs_trans_log_buf(tp, bp, (uint)((char *)dup - (char *)hdr),
		(uint)((char *)&dup->length + sizeof(dup->length) -
		       1 - (char *)hdr));
	/*
	 * Log the end (tag) of the unused entry.
	 */
	xfs_trans_log_buf(tp, bp,
		(uint)((char *)xfs_dir2_data_unused_tag_p(dup) - (char *)hdr),
		(uint)((char *)xfs_dir2_data_unused_tag_p(dup) - (char *)hdr +
>>>>>>> refs/remotes/origin/master
		       sizeof(xfs_dir2_data_off_t) - 1));
}

/*
 * Make a byte range in the data block unused.
 * Its current contents are unimportant.
 */
void
xfs_dir2_data_make_free(
<<<<<<< HEAD
	xfs_trans_t		*tp,		/* transaction pointer */
	xfs_dabuf_t		*bp,		/* block buffer */
=======
	struct xfs_trans	*tp,
	struct xfs_inode	*dp,
	struct xfs_buf		*bp,
>>>>>>> refs/remotes/origin/master
	xfs_dir2_data_aoff_t	offset,		/* starting byte offset */
	xfs_dir2_data_aoff_t	len,		/* length in bytes */
	int			*needlogp,	/* out: log header */
	int			*needscanp)	/* out: regen bestfree */
{
<<<<<<< HEAD
<<<<<<< HEAD
	xfs_dir2_data_t		*d;		/* data block pointer */
=======
	xfs_dir2_data_hdr_t	*hdr;		/* data block pointer */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	xfs_dir2_data_hdr_t	*hdr;		/* data block pointer */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_data_free_t	*dfp;		/* bestfree pointer */
	char			*endptr;	/* end of data area */
	xfs_mount_t		*mp;		/* filesystem mount point */
	int			needscan;	/* need to regen bestfree */
	xfs_dir2_data_unused_t	*newdup;	/* new unused entry */
	xfs_dir2_data_unused_t	*postdup;	/* unused entry after us */
	xfs_dir2_data_unused_t	*prevdup;	/* unused entry before us */
<<<<<<< HEAD

	mp = tp->t_mountp;
<<<<<<< HEAD
	d = bp->data;
	/*
	 * Figure out where the end of the data area is.
	 */
	if (be32_to_cpu(d->hdr.magic) == XFS_DIR2_DATA_MAGIC)
		endptr = (char *)d + mp->m_dirblksize;
	else {
		xfs_dir2_block_tail_t	*btp;	/* block tail */

		ASSERT(be32_to_cpu(d->hdr.magic) == XFS_DIR2_BLOCK_MAGIC);
		btp = xfs_dir2_block_tail_p(mp, (xfs_dir2_block_t *)d);
=======
	hdr = bp->data;
=======
	struct xfs_dir2_data_free *bf;

	mp = tp->t_mountp;
	hdr = bp->b_addr;
>>>>>>> refs/remotes/origin/master

	/*
	 * Figure out where the end of the data area is.
	 */
<<<<<<< HEAD
	if (hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC))
=======
	if (hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC) ||
	    hdr->magic == cpu_to_be32(XFS_DIR3_DATA_MAGIC))
>>>>>>> refs/remotes/origin/master
		endptr = (char *)hdr + mp->m_dirblksize;
	else {
		xfs_dir2_block_tail_t	*btp;	/* block tail */

<<<<<<< HEAD
		ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC));
		btp = xfs_dir2_block_tail_p(mp, hdr);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC) ||
			hdr->magic == cpu_to_be32(XFS_DIR3_BLOCK_MAGIC));
		btp = xfs_dir2_block_tail_p(mp, hdr);
>>>>>>> refs/remotes/origin/master
		endptr = (char *)xfs_dir2_block_leaf_p(btp);
	}
	/*
	 * If this isn't the start of the block, then back up to
	 * the previous entry and see if it's free.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if (offset > sizeof(d->hdr)) {
		__be16			*tagp;	/* tag just before us */

		tagp = (__be16 *)((char *)d + offset) - 1;
		prevdup = (xfs_dir2_data_unused_t *)((char *)d + be16_to_cpu(*tagp));
=======
	if (offset > sizeof(*hdr)) {
=======
	if (offset > dp->d_ops->data_entry_offset) {
>>>>>>> refs/remotes/origin/master
		__be16			*tagp;	/* tag just before us */

		tagp = (__be16 *)((char *)hdr + offset) - 1;
		prevdup = (xfs_dir2_data_unused_t *)((char *)hdr + be16_to_cpu(*tagp));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		if (be16_to_cpu(prevdup->freetag) != XFS_DIR2_DATA_FREE_TAG)
			prevdup = NULL;
	} else
		prevdup = NULL;
	/*
	 * If this isn't the end of the block, see if the entry after
	 * us is free.
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	if ((char *)d + offset + len < endptr) {
		postdup =
			(xfs_dir2_data_unused_t *)((char *)d + offset + len);
=======
	if ((char *)hdr + offset + len < endptr) {
		postdup =
			(xfs_dir2_data_unused_t *)((char *)hdr + offset + len);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if ((char *)hdr + offset + len < endptr) {
		postdup =
			(xfs_dir2_data_unused_t *)((char *)hdr + offset + len);
>>>>>>> refs/remotes/origin/master
		if (be16_to_cpu(postdup->freetag) != XFS_DIR2_DATA_FREE_TAG)
			postdup = NULL;
	} else
		postdup = NULL;
	ASSERT(*needscanp == 0);
	needscan = 0;
	/*
	 * Previous and following entries are both free,
	 * merge everything into a single free entry.
	 */
<<<<<<< HEAD
=======
	bf = dp->d_ops->data_bestfree_p(hdr);
>>>>>>> refs/remotes/origin/master
	if (prevdup && postdup) {
		xfs_dir2_data_free_t	*dfp2;	/* another bestfree pointer */

		/*
		 * See if prevdup and/or postdup are in bestfree table.
		 */
<<<<<<< HEAD
<<<<<<< HEAD
		dfp = xfs_dir2_data_freefind(d, prevdup);
		dfp2 = xfs_dir2_data_freefind(d, postdup);
=======
		dfp = xfs_dir2_data_freefind(hdr, prevdup);
		dfp2 = xfs_dir2_data_freefind(hdr, postdup);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		dfp = xfs_dir2_data_freefind(hdr, bf, prevdup);
		dfp2 = xfs_dir2_data_freefind(hdr, bf, postdup);
>>>>>>> refs/remotes/origin/master
		/*
		 * We need a rescan unless there are exactly 2 free entries
		 * namely our two.  Then we know what's happening, otherwise
		 * since the third bestfree is there, there might be more
		 * entries.
		 */
<<<<<<< HEAD
<<<<<<< HEAD
		needscan = (d->hdr.bestfree[2].length != 0);
=======
		needscan = (hdr->bestfree[2].length != 0);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		needscan = (bf[2].length != 0);
>>>>>>> refs/remotes/origin/master
		/*
		 * Fix up the new big freespace.
		 */
		be16_add_cpu(&prevdup->length, len + be16_to_cpu(postdup->length));
		*xfs_dir2_data_unused_tag_p(prevdup) =
<<<<<<< HEAD
<<<<<<< HEAD
			cpu_to_be16((char *)prevdup - (char *)d);
=======
			cpu_to_be16((char *)prevdup - (char *)hdr);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			cpu_to_be16((char *)prevdup - (char *)hdr);
>>>>>>> refs/remotes/origin/master
		xfs_dir2_data_log_unused(tp, bp, prevdup);
		if (!needscan) {
			/*
			 * Has to be the case that entries 0 and 1 are
			 * dfp and dfp2 (don't know which is which), and
			 * entry 2 is empty.
			 * Remove entry 1 first then entry 0.
			 */
			ASSERT(dfp && dfp2);
<<<<<<< HEAD
<<<<<<< HEAD
			if (dfp == &d->hdr.bestfree[1]) {
				dfp = &d->hdr.bestfree[0];
				ASSERT(dfp2 == dfp);
				dfp2 = &d->hdr.bestfree[1];
			}
			xfs_dir2_data_freeremove(d, dfp2, needlogp);
			xfs_dir2_data_freeremove(d, dfp, needlogp);
			/*
			 * Now insert the new entry.
			 */
			dfp = xfs_dir2_data_freeinsert(d, prevdup, needlogp);
			ASSERT(dfp == &d->hdr.bestfree[0]);
=======
			if (dfp == &hdr->bestfree[1]) {
				dfp = &hdr->bestfree[0];
				ASSERT(dfp2 == dfp);
				dfp2 = &hdr->bestfree[1];
			}
			xfs_dir2_data_freeremove(hdr, dfp2, needlogp);
			xfs_dir2_data_freeremove(hdr, dfp, needlogp);
			/*
			 * Now insert the new entry.
			 */
			dfp = xfs_dir2_data_freeinsert(hdr, prevdup, needlogp);
			ASSERT(dfp == &hdr->bestfree[0]);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			if (dfp == &bf[1]) {
				dfp = &bf[0];
				ASSERT(dfp2 == dfp);
				dfp2 = &bf[1];
			}
			xfs_dir2_data_freeremove(hdr, bf, dfp2, needlogp);
			xfs_dir2_data_freeremove(hdr, bf, dfp, needlogp);
			/*
			 * Now insert the new entry.
			 */
			dfp = xfs_dir2_data_freeinsert(hdr, bf, prevdup,
						       needlogp);
			ASSERT(dfp == &bf[0]);
>>>>>>> refs/remotes/origin/master
			ASSERT(dfp->length == prevdup->length);
			ASSERT(!dfp[1].length);
			ASSERT(!dfp[2].length);
		}
	}
	/*
	 * The entry before us is free, merge with it.
	 */
	else if (prevdup) {
<<<<<<< HEAD
<<<<<<< HEAD
		dfp = xfs_dir2_data_freefind(d, prevdup);
		be16_add_cpu(&prevdup->length, len);
		*xfs_dir2_data_unused_tag_p(prevdup) =
			cpu_to_be16((char *)prevdup - (char *)d);
=======
		dfp = xfs_dir2_data_freefind(hdr, prevdup);
		be16_add_cpu(&prevdup->length, len);
		*xfs_dir2_data_unused_tag_p(prevdup) =
			cpu_to_be16((char *)prevdup - (char *)hdr);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		dfp = xfs_dir2_data_freefind(hdr, bf, prevdup);
		be16_add_cpu(&prevdup->length, len);
		*xfs_dir2_data_unused_tag_p(prevdup) =
			cpu_to_be16((char *)prevdup - (char *)hdr);
>>>>>>> refs/remotes/origin/master
		xfs_dir2_data_log_unused(tp, bp, prevdup);
		/*
		 * If the previous entry was in the table, the new entry
		 * is longer, so it will be in the table too.  Remove
		 * the old one and add the new one.
		 */
		if (dfp) {
<<<<<<< HEAD
<<<<<<< HEAD
			xfs_dir2_data_freeremove(d, dfp, needlogp);
			(void)xfs_dir2_data_freeinsert(d, prevdup, needlogp);
=======
			xfs_dir2_data_freeremove(hdr, dfp, needlogp);
			xfs_dir2_data_freeinsert(hdr, prevdup, needlogp);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			xfs_dir2_data_freeremove(hdr, bf, dfp, needlogp);
			xfs_dir2_data_freeinsert(hdr, bf, prevdup, needlogp);
>>>>>>> refs/remotes/origin/master
		}
		/*
		 * Otherwise we need a scan if the new entry is big enough.
		 */
		else {
			needscan = be16_to_cpu(prevdup->length) >
<<<<<<< HEAD
<<<<<<< HEAD
				   be16_to_cpu(d->hdr.bestfree[2].length);
=======
				   be16_to_cpu(hdr->bestfree[2].length);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				   be16_to_cpu(bf[2].length);
>>>>>>> refs/remotes/origin/master
		}
	}
	/*
	 * The following entry is free, merge with it.
	 */
	else if (postdup) {
<<<<<<< HEAD
<<<<<<< HEAD
		dfp = xfs_dir2_data_freefind(d, postdup);
		newdup = (xfs_dir2_data_unused_t *)((char *)d + offset);
		newdup->freetag = cpu_to_be16(XFS_DIR2_DATA_FREE_TAG);
		newdup->length = cpu_to_be16(len + be16_to_cpu(postdup->length));
		*xfs_dir2_data_unused_tag_p(newdup) =
			cpu_to_be16((char *)newdup - (char *)d);
=======
		dfp = xfs_dir2_data_freefind(hdr, postdup);
=======
		dfp = xfs_dir2_data_freefind(hdr, bf, postdup);
>>>>>>> refs/remotes/origin/master
		newdup = (xfs_dir2_data_unused_t *)((char *)hdr + offset);
		newdup->freetag = cpu_to_be16(XFS_DIR2_DATA_FREE_TAG);
		newdup->length = cpu_to_be16(len + be16_to_cpu(postdup->length));
		*xfs_dir2_data_unused_tag_p(newdup) =
			cpu_to_be16((char *)newdup - (char *)hdr);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		xfs_dir2_data_log_unused(tp, bp, newdup);
		/*
		 * If the following entry was in the table, the new entry
		 * is longer, so it will be in the table too.  Remove
		 * the old one and add the new one.
		 */
		if (dfp) {
<<<<<<< HEAD
<<<<<<< HEAD
			xfs_dir2_data_freeremove(d, dfp, needlogp);
			(void)xfs_dir2_data_freeinsert(d, newdup, needlogp);
=======
			xfs_dir2_data_freeremove(hdr, dfp, needlogp);
			xfs_dir2_data_freeinsert(hdr, newdup, needlogp);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			xfs_dir2_data_freeremove(hdr, bf, dfp, needlogp);
			xfs_dir2_data_freeinsert(hdr, bf, newdup, needlogp);
>>>>>>> refs/remotes/origin/master
		}
		/*
		 * Otherwise we need a scan if the new entry is big enough.
		 */
		else {
			needscan = be16_to_cpu(newdup->length) >
<<<<<<< HEAD
<<<<<<< HEAD
				   be16_to_cpu(d->hdr.bestfree[2].length);
=======
				   be16_to_cpu(hdr->bestfree[2].length);
>>>>>>> refs/remotes/origin/cm-10.0
=======
				   be16_to_cpu(bf[2].length);
>>>>>>> refs/remotes/origin/master
		}
	}
	/*
	 * Neither neighbor is free.  Make a new entry.
	 */
	else {
<<<<<<< HEAD
<<<<<<< HEAD
		newdup = (xfs_dir2_data_unused_t *)((char *)d + offset);
		newdup->freetag = cpu_to_be16(XFS_DIR2_DATA_FREE_TAG);
		newdup->length = cpu_to_be16(len);
		*xfs_dir2_data_unused_tag_p(newdup) =
			cpu_to_be16((char *)newdup - (char *)d);
		xfs_dir2_data_log_unused(tp, bp, newdup);
		(void)xfs_dir2_data_freeinsert(d, newdup, needlogp);
=======
=======
>>>>>>> refs/remotes/origin/master
		newdup = (xfs_dir2_data_unused_t *)((char *)hdr + offset);
		newdup->freetag = cpu_to_be16(XFS_DIR2_DATA_FREE_TAG);
		newdup->length = cpu_to_be16(len);
		*xfs_dir2_data_unused_tag_p(newdup) =
			cpu_to_be16((char *)newdup - (char *)hdr);
		xfs_dir2_data_log_unused(tp, bp, newdup);
<<<<<<< HEAD
		xfs_dir2_data_freeinsert(hdr, newdup, needlogp);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		xfs_dir2_data_freeinsert(hdr, bf, newdup, needlogp);
>>>>>>> refs/remotes/origin/master
	}
	*needscanp = needscan;
}

/*
 * Take a byte range out of an existing unused space and make it un-free.
 */
void
xfs_dir2_data_use_free(
<<<<<<< HEAD
	xfs_trans_t		*tp,		/* transaction pointer */
	xfs_dabuf_t		*bp,		/* data block buffer */
=======
	struct xfs_trans	*tp,
	struct xfs_inode	*dp,
	struct xfs_buf		*bp,
>>>>>>> refs/remotes/origin/master
	xfs_dir2_data_unused_t	*dup,		/* unused entry */
	xfs_dir2_data_aoff_t	offset,		/* starting offset to use */
	xfs_dir2_data_aoff_t	len,		/* length to use */
	int			*needlogp,	/* out: need to log header */
	int			*needscanp)	/* out: need regen bestfree */
{
<<<<<<< HEAD
<<<<<<< HEAD
	xfs_dir2_data_t		*d;		/* data block */
=======
	xfs_dir2_data_hdr_t	*hdr;		/* data block header */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	xfs_dir2_data_hdr_t	*hdr;		/* data block header */
>>>>>>> refs/remotes/origin/master
	xfs_dir2_data_free_t	*dfp;		/* bestfree pointer */
	int			matchback;	/* matches end of freespace */
	int			matchfront;	/* matches start of freespace */
	int			needscan;	/* need to regen bestfree */
	xfs_dir2_data_unused_t	*newdup;	/* new unused entry */
	xfs_dir2_data_unused_t	*newdup2;	/* another new unused entry */
	int			oldlen;		/* old unused entry's length */
<<<<<<< HEAD

<<<<<<< HEAD
	d = bp->data;
	ASSERT(be32_to_cpu(d->hdr.magic) == XFS_DIR2_DATA_MAGIC ||
	       be32_to_cpu(d->hdr.magic) == XFS_DIR2_BLOCK_MAGIC);
	ASSERT(be16_to_cpu(dup->freetag) == XFS_DIR2_DATA_FREE_TAG);
	ASSERT(offset >= (char *)dup - (char *)d);
	ASSERT(offset + len <= (char *)dup + be16_to_cpu(dup->length) - (char *)d);
	ASSERT((char *)dup - (char *)d == be16_to_cpu(*xfs_dir2_data_unused_tag_p(dup)));
	/*
	 * Look up the entry in the bestfree table.
	 */
	dfp = xfs_dir2_data_freefind(d, dup);
	oldlen = be16_to_cpu(dup->length);
	ASSERT(dfp || oldlen <= be16_to_cpu(d->hdr.bestfree[2].length));
	/*
	 * Check for alignment with front and back of the entry.
	 */
	matchfront = (char *)dup - (char *)d == offset;
	matchback = (char *)dup + oldlen - (char *)d == offset + len;
=======
	hdr = bp->data;
	ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC));
=======
	struct xfs_dir2_data_free *bf;

	hdr = bp->b_addr;
	ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR3_DATA_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR2_BLOCK_MAGIC) ||
	       hdr->magic == cpu_to_be32(XFS_DIR3_BLOCK_MAGIC));
>>>>>>> refs/remotes/origin/master
	ASSERT(be16_to_cpu(dup->freetag) == XFS_DIR2_DATA_FREE_TAG);
	ASSERT(offset >= (char *)dup - (char *)hdr);
	ASSERT(offset + len <= (char *)dup + be16_to_cpu(dup->length) - (char *)hdr);
	ASSERT((char *)dup - (char *)hdr == be16_to_cpu(*xfs_dir2_data_unused_tag_p(dup)));
	/*
	 * Look up the entry in the bestfree table.
	 */
<<<<<<< HEAD
	dfp = xfs_dir2_data_freefind(hdr, dup);
	oldlen = be16_to_cpu(dup->length);
	ASSERT(dfp || oldlen <= be16_to_cpu(hdr->bestfree[2].length));
=======
	oldlen = be16_to_cpu(dup->length);
	bf = dp->d_ops->data_bestfree_p(hdr);
	dfp = xfs_dir2_data_freefind(hdr, bf, dup);
	ASSERT(dfp || oldlen <= be16_to_cpu(bf[2].length));
>>>>>>> refs/remotes/origin/master
	/*
	 * Check for alignment with front and back of the entry.
	 */
	matchfront = (char *)dup - (char *)hdr == offset;
	matchback = (char *)dup + oldlen - (char *)hdr == offset + len;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	ASSERT(*needscanp == 0);
	needscan = 0;
	/*
	 * If we matched it exactly we just need to get rid of it from
	 * the bestfree table.
	 */
	if (matchfront && matchback) {
		if (dfp) {
<<<<<<< HEAD
<<<<<<< HEAD
			needscan = (d->hdr.bestfree[2].offset != 0);
			if (!needscan)
				xfs_dir2_data_freeremove(d, dfp, needlogp);
=======
			needscan = (hdr->bestfree[2].offset != 0);
			if (!needscan)
				xfs_dir2_data_freeremove(hdr, dfp, needlogp);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			needscan = (bf[2].offset != 0);
			if (!needscan)
				xfs_dir2_data_freeremove(hdr, bf, dfp,
							 needlogp);
>>>>>>> refs/remotes/origin/master
		}
	}
	/*
	 * We match the first part of the entry.
	 * Make a new entry with the remaining freespace.
	 */
	else if (matchfront) {
<<<<<<< HEAD
<<<<<<< HEAD
		newdup = (xfs_dir2_data_unused_t *)((char *)d + offset + len);
		newdup->freetag = cpu_to_be16(XFS_DIR2_DATA_FREE_TAG);
		newdup->length = cpu_to_be16(oldlen - len);
		*xfs_dir2_data_unused_tag_p(newdup) =
			cpu_to_be16((char *)newdup - (char *)d);
=======
=======
>>>>>>> refs/remotes/origin/master
		newdup = (xfs_dir2_data_unused_t *)((char *)hdr + offset + len);
		newdup->freetag = cpu_to_be16(XFS_DIR2_DATA_FREE_TAG);
		newdup->length = cpu_to_be16(oldlen - len);
		*xfs_dir2_data_unused_tag_p(newdup) =
			cpu_to_be16((char *)newdup - (char *)hdr);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		xfs_dir2_data_log_unused(tp, bp, newdup);
		/*
		 * If it was in the table, remove it and add the new one.
		 */
		if (dfp) {
<<<<<<< HEAD
<<<<<<< HEAD
			xfs_dir2_data_freeremove(d, dfp, needlogp);
			dfp = xfs_dir2_data_freeinsert(d, newdup, needlogp);
			ASSERT(dfp != NULL);
			ASSERT(dfp->length == newdup->length);
			ASSERT(be16_to_cpu(dfp->offset) == (char *)newdup - (char *)d);
=======
			xfs_dir2_data_freeremove(hdr, dfp, needlogp);
			dfp = xfs_dir2_data_freeinsert(hdr, newdup, needlogp);
			ASSERT(dfp != NULL);
			ASSERT(dfp->length == newdup->length);
			ASSERT(be16_to_cpu(dfp->offset) == (char *)newdup - (char *)hdr);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			xfs_dir2_data_freeremove(hdr, bf, dfp, needlogp);
			dfp = xfs_dir2_data_freeinsert(hdr, bf, newdup,
						       needlogp);
			ASSERT(dfp != NULL);
			ASSERT(dfp->length == newdup->length);
			ASSERT(be16_to_cpu(dfp->offset) == (char *)newdup - (char *)hdr);
>>>>>>> refs/remotes/origin/master
			/*
			 * If we got inserted at the last slot,
			 * that means we don't know if there was a better
			 * choice for the last slot, or not.  Rescan.
			 */
<<<<<<< HEAD
<<<<<<< HEAD
			needscan = dfp == &d->hdr.bestfree[2];
=======
			needscan = dfp == &hdr->bestfree[2];
>>>>>>> refs/remotes/origin/cm-10.0
=======
			needscan = dfp == &bf[2];
>>>>>>> refs/remotes/origin/master
		}
	}
	/*
	 * We match the last part of the entry.
	 * Trim the allocated space off the tail of the entry.
	 */
	else if (matchback) {
		newdup = dup;
<<<<<<< HEAD
<<<<<<< HEAD
		newdup->length = cpu_to_be16(((char *)d + offset) - (char *)newdup);
		*xfs_dir2_data_unused_tag_p(newdup) =
			cpu_to_be16((char *)newdup - (char *)d);
=======
		newdup->length = cpu_to_be16(((char *)hdr + offset) - (char *)newdup);
		*xfs_dir2_data_unused_tag_p(newdup) =
			cpu_to_be16((char *)newdup - (char *)hdr);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		newdup->length = cpu_to_be16(((char *)hdr + offset) - (char *)newdup);
		*xfs_dir2_data_unused_tag_p(newdup) =
			cpu_to_be16((char *)newdup - (char *)hdr);
>>>>>>> refs/remotes/origin/master
		xfs_dir2_data_log_unused(tp, bp, newdup);
		/*
		 * If it was in the table, remove it and add the new one.
		 */
		if (dfp) {
<<<<<<< HEAD
<<<<<<< HEAD
			xfs_dir2_data_freeremove(d, dfp, needlogp);
			dfp = xfs_dir2_data_freeinsert(d, newdup, needlogp);
			ASSERT(dfp != NULL);
			ASSERT(dfp->length == newdup->length);
			ASSERT(be16_to_cpu(dfp->offset) == (char *)newdup - (char *)d);
=======
			xfs_dir2_data_freeremove(hdr, dfp, needlogp);
			dfp = xfs_dir2_data_freeinsert(hdr, newdup, needlogp);
			ASSERT(dfp != NULL);
			ASSERT(dfp->length == newdup->length);
			ASSERT(be16_to_cpu(dfp->offset) == (char *)newdup - (char *)hdr);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			xfs_dir2_data_freeremove(hdr, bf, dfp, needlogp);
			dfp = xfs_dir2_data_freeinsert(hdr, bf, newdup,
						       needlogp);
			ASSERT(dfp != NULL);
			ASSERT(dfp->length == newdup->length);
			ASSERT(be16_to_cpu(dfp->offset) == (char *)newdup - (char *)hdr);
>>>>>>> refs/remotes/origin/master
			/*
			 * If we got inserted at the last slot,
			 * that means we don't know if there was a better
			 * choice for the last slot, or not.  Rescan.
			 */
<<<<<<< HEAD
<<<<<<< HEAD
			needscan = dfp == &d->hdr.bestfree[2];
=======
			needscan = dfp == &hdr->bestfree[2];
>>>>>>> refs/remotes/origin/cm-10.0
=======
			needscan = dfp == &bf[2];
>>>>>>> refs/remotes/origin/master
		}
	}
	/*
	 * Poking out the middle of an entry.
	 * Make two new entries.
	 */
	else {
		newdup = dup;
<<<<<<< HEAD
<<<<<<< HEAD
		newdup->length = cpu_to_be16(((char *)d + offset) - (char *)newdup);
		*xfs_dir2_data_unused_tag_p(newdup) =
			cpu_to_be16((char *)newdup - (char *)d);
		xfs_dir2_data_log_unused(tp, bp, newdup);
		newdup2 = (xfs_dir2_data_unused_t *)((char *)d + offset + len);
		newdup2->freetag = cpu_to_be16(XFS_DIR2_DATA_FREE_TAG);
		newdup2->length = cpu_to_be16(oldlen - len - be16_to_cpu(newdup->length));
		*xfs_dir2_data_unused_tag_p(newdup2) =
			cpu_to_be16((char *)newdup2 - (char *)d);
=======
=======
>>>>>>> refs/remotes/origin/master
		newdup->length = cpu_to_be16(((char *)hdr + offset) - (char *)newdup);
		*xfs_dir2_data_unused_tag_p(newdup) =
			cpu_to_be16((char *)newdup - (char *)hdr);
		xfs_dir2_data_log_unused(tp, bp, newdup);
		newdup2 = (xfs_dir2_data_unused_t *)((char *)hdr + offset + len);
		newdup2->freetag = cpu_to_be16(XFS_DIR2_DATA_FREE_TAG);
		newdup2->length = cpu_to_be16(oldlen - len - be16_to_cpu(newdup->length));
		*xfs_dir2_data_unused_tag_p(newdup2) =
			cpu_to_be16((char *)newdup2 - (char *)hdr);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		xfs_dir2_data_log_unused(tp, bp, newdup2);
		/*
		 * If the old entry was in the table, we need to scan
		 * if the 3rd entry was valid, since these entries
		 * are smaller than the old one.
		 * If we don't need to scan that means there were 1 or 2
		 * entries in the table, and removing the old and adding
		 * the 2 new will work.
		 */
		if (dfp) {
<<<<<<< HEAD
<<<<<<< HEAD
			needscan = (d->hdr.bestfree[2].length != 0);
			if (!needscan) {
				xfs_dir2_data_freeremove(d, dfp, needlogp);
				(void)xfs_dir2_data_freeinsert(d, newdup,
					needlogp);
				(void)xfs_dir2_data_freeinsert(d, newdup2,
					needlogp);
=======
			needscan = (hdr->bestfree[2].length != 0);
			if (!needscan) {
				xfs_dir2_data_freeremove(hdr, dfp, needlogp);
				xfs_dir2_data_freeinsert(hdr, newdup, needlogp);
				xfs_dir2_data_freeinsert(hdr, newdup2,
							 needlogp);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			needscan = (bf[2].length != 0);
			if (!needscan) {
				xfs_dir2_data_freeremove(hdr, bf, dfp,
							 needlogp);
				xfs_dir2_data_freeinsert(hdr, bf, newdup,
							 needlogp);
				xfs_dir2_data_freeinsert(hdr, bf, newdup2,
							 needlogp);
>>>>>>> refs/remotes/origin/master
			}
		}
	}
	*needscanp = needscan;
}
