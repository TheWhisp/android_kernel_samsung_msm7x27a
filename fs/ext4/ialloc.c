/*
 *  linux/fs/ext4/ialloc.c
 *
 * Copyright (C) 1992, 1993, 1994, 1995
 * Remy Card (card@masi.ibp.fr)
 * Laboratoire MASI - Institut Blaise Pascal
 * Universite Pierre et Marie Curie (Paris VI)
 *
 *  BSD ufs-inspired inode and directory allocation by
 *  Stephen Tweedie (sct@redhat.com), 1993
 *  Big-endian to little-endian byte-swapping/bitmaps by
 *        David S. Miller (davem@caip.rutgers.edu), 1995
 */

#include <linux/time.h>
#include <linux/fs.h>
#include <linux/jbd2.h>
#include <linux/stat.h>
#include <linux/string.h>
#include <linux/quotaops.h>
#include <linux/buffer_head.h>
#include <linux/random.h>
#include <linux/bitops.h>
#include <linux/blkdev.h>
#include <asm/byteorder.h>

#include "ext4.h"
#include "ext4_jbd2.h"
#include "xattr.h"
#include "acl.h"

#include <trace/events/ext4.h>

/*
 * ialloc.c contains the inodes allocation and deallocation routines
 */

/*
 * The free inodes are managed by bitmaps.  A file system contains several
 * blocks groups.  Each group contains 1 bitmap block for blocks, 1 bitmap
 * block for inodes, N blocks for the inode table and data blocks.
 *
 * The file system contains group descriptors which are located after the
 * super block.  Each descriptor contains the number of the bitmap block and
 * the free blocks count in the block.
 */

/*
 * To avoid calling the atomic setbit hundreds or thousands of times, we only
 * need to use it within a single byte (to ensure we get endianness right).
 * We can use memset for the rest of the bitmap as there are no other users.
 */
void ext4_mark_bitmap_end(int start_bit, int end_bit, char *bitmap)
{
	int i;

	if (start_bit >= end_bit)
		return;

	ext4_debug("mark end bits +%d through +%d used\n", start_bit, end_bit);
	for (i = start_bit; i < ((start_bit + 7) & ~7UL); i++)
		ext4_set_bit(i, bitmap);
	if (i < end_bit)
		memset(bitmap + (i >> 3), 0xff, (end_bit - i) >> 3);
}

/* Initializes an uninitialized inode bitmap */
static unsigned ext4_init_inode_bitmap(struct super_block *sb,
				       struct buffer_head *bh,
				       ext4_group_t block_group,
				       struct ext4_group_desc *gdp)
{
<<<<<<< HEAD
	struct ext4_sb_info *sbi = EXT4_SB(sb);

=======
	struct ext4_group_info *grp;
>>>>>>> refs/remotes/origin/master
	J_ASSERT_BH(bh, buffer_locked(bh));

	/* If checksum is bad mark all blocks and inodes use to prevent
	 * allocation, essentially implementing a per-group read-only flag. */
<<<<<<< HEAD
	if (!ext4_group_desc_csum_verify(sbi, block_group, gdp)) {
		ext4_error(sb, "Checksum bad for group %u", block_group);
<<<<<<< HEAD
		ext4_free_blks_set(sb, gdp, 0);
=======
		ext4_free_group_clusters_set(sb, gdp, 0);
>>>>>>> refs/remotes/origin/cm-10.0
		ext4_free_inodes_set(sb, gdp, 0);
		ext4_itable_unused_set(sb, gdp, 0);
		memset(bh->b_data, 0xff, sb->s_blocksize);
=======
	if (!ext4_group_desc_csum_verify(sb, block_group, gdp)) {
		ext4_error(sb, "Checksum bad for group %u", block_group);
		grp = ext4_get_group_info(sb, block_group);
		set_bit(EXT4_GROUP_INFO_BBITMAP_CORRUPT_BIT, &grp->bb_state);
		set_bit(EXT4_GROUP_INFO_IBITMAP_CORRUPT_BIT, &grp->bb_state);
>>>>>>> refs/remotes/origin/master
		return 0;
	}

	memset(bh->b_data, 0, (EXT4_INODES_PER_GROUP(sb) + 7) / 8);
	ext4_mark_bitmap_end(EXT4_INODES_PER_GROUP(sb), sb->s_blocksize * 8,
			bh->b_data);
<<<<<<< HEAD
=======
	ext4_inode_bitmap_csum_set(sb, block_group, gdp, bh,
				   EXT4_INODES_PER_GROUP(sb) / 8);
	ext4_group_desc_csum_set(sb, block_group, gdp);
>>>>>>> refs/remotes/origin/master

	return EXT4_INODES_PER_GROUP(sb);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
void ext4_end_bitmap_read(struct buffer_head *bh, int uptodate)
{
	if (uptodate) {
		set_buffer_uptodate(bh);
		set_bitmap_uptodate(bh);
	}
	unlock_buffer(bh);
	put_bh(bh);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * Read the inode allocation bitmap for a given block_group, reading
 * into the specified slot in the superblock's bitmap cache.
 *
 * Return buffer_head of bitmap on success or NULL.
 */
static struct buffer_head *
ext4_read_inode_bitmap(struct super_block *sb, ext4_group_t block_group)
{
	struct ext4_group_desc *desc;
	struct buffer_head *bh = NULL;
	ext4_fsblk_t bitmap_blk;
<<<<<<< HEAD
=======
	struct ext4_group_info *grp;
>>>>>>> refs/remotes/origin/master

	desc = ext4_get_group_desc(sb, block_group, NULL);
	if (!desc)
		return NULL;

	bitmap_blk = ext4_inode_bitmap(sb, desc);
	bh = sb_getblk(sb, bitmap_blk);
	if (unlikely(!bh)) {
		ext4_error(sb, "Cannot read inode bitmap - "
			    "block_group = %u, inode_bitmap = %llu",
			    block_group, bitmap_blk);
		return NULL;
	}
	if (bitmap_uptodate(bh))
<<<<<<< HEAD
		return bh;
=======
		goto verify;
>>>>>>> refs/remotes/origin/master

	lock_buffer(bh);
	if (bitmap_uptodate(bh)) {
		unlock_buffer(bh);
<<<<<<< HEAD
		return bh;
=======
		goto verify;
>>>>>>> refs/remotes/origin/master
	}

	ext4_lock_group(sb, block_group);
	if (desc->bg_flags & cpu_to_le16(EXT4_BG_INODE_UNINIT)) {
		ext4_init_inode_bitmap(sb, bh, block_group, desc);
		set_bitmap_uptodate(bh);
		set_buffer_uptodate(bh);
<<<<<<< HEAD
=======
		set_buffer_verified(bh);
>>>>>>> refs/remotes/origin/master
		ext4_unlock_group(sb, block_group);
		unlock_buffer(bh);
		return bh;
	}
	ext4_unlock_group(sb, block_group);

	if (buffer_uptodate(bh)) {
		/*
		 * if not uninit if bh is uptodate,
		 * bitmap is also uptodate
		 */
		set_bitmap_uptodate(bh);
		unlock_buffer(bh);
<<<<<<< HEAD
		return bh;
	}
	/*
<<<<<<< HEAD
	 * submit the buffer_head for read. We can
	 * safely mark the bitmap as uptodate now.
	 * We do it here so the bitmap uptodate bit
	 * get set with buffer lock held.
	 */
	trace_ext4_load_inode_bitmap(sb, block_group);
	set_bitmap_uptodate(bh);
	if (bh_submit_read(bh) < 0) {
		put_bh(bh);
		ext4_error(sb, "Cannot read inode bitmap - "
			    "block_group = %u, inode_bitmap = %llu",
			    block_group, bitmap_blk);
=======
=======
		goto verify;
	}
	/*
>>>>>>> refs/remotes/origin/master
	 * submit the buffer_head for reading
	 */
	trace_ext4_load_inode_bitmap(sb, block_group);
	bh->b_end_io = ext4_end_bitmap_read;
	get_bh(bh);
<<<<<<< HEAD
	submit_bh(READ, bh);
=======
	submit_bh(READ | REQ_META | REQ_PRIO, bh);
>>>>>>> refs/remotes/origin/master
	wait_on_buffer(bh);
	if (!buffer_uptodate(bh)) {
		put_bh(bh);
		ext4_error(sb, "Cannot read inode bitmap - "
			   "block_group = %u, inode_bitmap = %llu",
			   block_group, bitmap_blk);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		return NULL;
	}
=======
		return NULL;
	}

verify:
	ext4_lock_group(sb, block_group);
	if (!buffer_verified(bh) &&
	    !ext4_inode_bitmap_csum_verify(sb, block_group, desc, bh,
					   EXT4_INODES_PER_GROUP(sb) / 8)) {
		ext4_unlock_group(sb, block_group);
		put_bh(bh);
		ext4_error(sb, "Corrupt inode bitmap - block_group = %u, "
			   "inode_bitmap = %llu", block_group, bitmap_blk);
		grp = ext4_get_group_info(sb, block_group);
		set_bit(EXT4_GROUP_INFO_IBITMAP_CORRUPT_BIT, &grp->bb_state);
		return NULL;
	}
	ext4_unlock_group(sb, block_group);
	set_buffer_verified(bh);
>>>>>>> refs/remotes/origin/master
	return bh;
}

/*
 * NOTE! When we get the inode, we're the only people
 * that have access to it, and as such there are no
 * race conditions we have to worry about. The inode
 * is not on the hash-lists, and it cannot be reached
 * through the filesystem because the directory entry
 * has been deleted earlier.
 *
 * HOWEVER: we must make sure that we get no aliases,
 * which means that we have to call "clear_inode()"
 * _before_ we mark the inode not in use in the inode
 * bitmaps. Otherwise a newly created file might use
 * the same inode number (not actually the same pointer
 * though), and then we'd have two inodes sharing the
 * same inode number and space on the harddisk.
 */
void ext4_free_inode(handle_t *handle, struct inode *inode)
{
	struct super_block *sb = inode->i_sb;
	int is_directory;
	unsigned long ino;
	struct buffer_head *bitmap_bh = NULL;
	struct buffer_head *bh2;
	ext4_group_t block_group;
	unsigned long bit;
	struct ext4_group_desc *gdp;
	struct ext4_super_block *es;
	struct ext4_sb_info *sbi;
	int fatal = 0, err, count, cleared;
<<<<<<< HEAD

<<<<<<< HEAD
	if (atomic_read(&inode->i_count) > 1) {
		printk(KERN_ERR "ext4_free_inode: inode has count=%d\n",
		       atomic_read(&inode->i_count));
		return;
	}
	if (inode->i_nlink) {
		printk(KERN_ERR "ext4_free_inode: inode has nlink=%d\n",
		       inode->i_nlink);
		return;
	}
	if (!sb) {
		printk(KERN_ERR "ext4_free_inode: inode on "
		       "nonexistent device\n");
=======
=======
	struct ext4_group_info *grp;

>>>>>>> refs/remotes/origin/master
	if (!sb) {
		printk(KERN_ERR "EXT4-fs: %s:%d: inode on "
		       "nonexistent device\n", __func__, __LINE__);
		return;
	}
	if (atomic_read(&inode->i_count) > 1) {
		ext4_msg(sb, KERN_ERR, "%s:%d: inode #%lu: count=%d",
			 __func__, __LINE__, inode->i_ino,
			 atomic_read(&inode->i_count));
		return;
	}
	if (inode->i_nlink) {
		ext4_msg(sb, KERN_ERR, "%s:%d: inode #%lu: nlink=%d\n",
			 __func__, __LINE__, inode->i_ino, inode->i_nlink);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		return;
	}
	sbi = EXT4_SB(sb);

	ino = inode->i_ino;
	ext4_debug("freeing inode %lu\n", ino);
	trace_ext4_free_inode(inode);

	/*
	 * Note: we must free any quota before locking the superblock,
	 * as writing the quota to disk may need the lock as well.
	 */
	dquot_initialize(inode);
	ext4_xattr_delete_inode(handle, inode);
	dquot_free_inode(inode);
	dquot_drop(inode);

	is_directory = S_ISDIR(inode->i_mode);

	/* Do this BEFORE marking the inode not in use or returning an error */
	ext4_clear_inode(inode);

	es = EXT4_SB(sb)->s_es;
	if (ino < EXT4_FIRST_INO(sb) || ino > le32_to_cpu(es->s_inodes_count)) {
		ext4_error(sb, "reserved or nonexistent inode %lu", ino);
		goto error_return;
	}
	block_group = (ino - 1) / EXT4_INODES_PER_GROUP(sb);
	bit = (ino - 1) % EXT4_INODES_PER_GROUP(sb);
	bitmap_bh = ext4_read_inode_bitmap(sb, block_group);
<<<<<<< HEAD
	if (!bitmap_bh)
=======
	/* Don't bother if the inode bitmap is corrupt. */
	grp = ext4_get_group_info(sb, block_group);
	if (unlikely(EXT4_MB_GRP_IBITMAP_CORRUPT(grp)) || !bitmap_bh)
>>>>>>> refs/remotes/origin/master
		goto error_return;

	BUFFER_TRACE(bitmap_bh, "get_write_access");
	fatal = ext4_journal_get_write_access(handle, bitmap_bh);
	if (fatal)
		goto error_return;

	fatal = -ESRCH;
	gdp = ext4_get_group_desc(sb, block_group, &bh2);
	if (gdp) {
		BUFFER_TRACE(bh2, "get_write_access");
		fatal = ext4_journal_get_write_access(handle, bh2);
	}
	ext4_lock_group(sb, block_group);
<<<<<<< HEAD
<<<<<<< HEAD
	cleared = ext4_clear_bit(bit, bitmap_bh->b_data);
=======
	cleared = ext4_test_and_clear_bit(bit, bitmap_bh->b_data);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	cleared = ext4_test_and_clear_bit(bit, bitmap_bh->b_data);
>>>>>>> refs/remotes/origin/master
	if (fatal || !cleared) {
		ext4_unlock_group(sb, block_group);
		goto out;
	}

	count = ext4_free_inodes_count(sb, gdp) + 1;
	ext4_free_inodes_set(sb, gdp, count);
	if (is_directory) {
		count = ext4_used_dirs_count(sb, gdp) - 1;
		ext4_used_dirs_set(sb, gdp, count);
		percpu_counter_dec(&sbi->s_dirs_counter);
	}
<<<<<<< HEAD
	gdp->bg_checksum = ext4_group_desc_csum(sbi, block_group, gdp);
=======
	ext4_inode_bitmap_csum_set(sb, block_group, gdp, bitmap_bh,
				   EXT4_INODES_PER_GROUP(sb) / 8);
	ext4_group_desc_csum_set(sb, block_group, gdp);
>>>>>>> refs/remotes/origin/master
	ext4_unlock_group(sb, block_group);

	percpu_counter_inc(&sbi->s_freeinodes_counter);
	if (sbi->s_log_groups_per_flex) {
		ext4_group_t f = ext4_flex_group(sbi, block_group);

		atomic_inc(&sbi->s_flex_groups[f].free_inodes);
		if (is_directory)
			atomic_dec(&sbi->s_flex_groups[f].used_dirs);
	}
	BUFFER_TRACE(bh2, "call ext4_handle_dirty_metadata");
	fatal = ext4_handle_dirty_metadata(handle, NULL, bh2);
out:
	if (cleared) {
		BUFFER_TRACE(bitmap_bh, "call ext4_handle_dirty_metadata");
		err = ext4_handle_dirty_metadata(handle, NULL, bitmap_bh);
		if (!fatal)
			fatal = err;
<<<<<<< HEAD
		ext4_mark_super_dirty(sb);
	} else
		ext4_error(sb, "bit already cleared for inode %lu", ino);
=======
	} else {
		ext4_error(sb, "bit already cleared for inode %lu", ino);
		set_bit(EXT4_GROUP_INFO_IBITMAP_CORRUPT_BIT, &grp->bb_state);
	}
>>>>>>> refs/remotes/origin/master

error_return:
	brelse(bitmap_bh);
	ext4_std_error(sb, fatal);
}

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * There are two policies for allocating an inode.  If the new inode is
 * a directory, then a forward search is made for a block group with both
 * free space and a low directory-to-inode ratio; if that fails, then of
 * the groups with above-average free space, that group with the fewest
 * directories already is chosen.
 *
 * For other inodes, search forward from the parent directory\'s block
 * group to find a free inode.
 */
static int find_group_dir(struct super_block *sb, struct inode *parent,
				ext4_group_t *best_group)
{
	ext4_group_t ngroups = ext4_get_groups_count(sb);
	unsigned int freei, avefreei;
	struct ext4_group_desc *desc, *best_desc = NULL;
	ext4_group_t group;
	int ret = -1;

	freei = percpu_counter_read_positive(&EXT4_SB(sb)->s_freeinodes_counter);
	avefreei = freei / ngroups;

	for (group = 0; group < ngroups; group++) {
		desc = ext4_get_group_desc(sb, group, NULL);
		if (!desc || !ext4_free_inodes_count(sb, desc))
			continue;
		if (ext4_free_inodes_count(sb, desc) < avefreei)
			continue;
		if (!best_desc ||
		    (ext4_free_blks_count(sb, desc) >
		     ext4_free_blks_count(sb, best_desc))) {
			*best_group = group;
			best_desc = desc;
			ret = 0;
		}
	}
	return ret;
}

#define free_block_ratio 10

static int find_group_flex(struct super_block *sb, struct inode *parent,
			   ext4_group_t *best_group)
{
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	struct ext4_group_desc *desc;
	struct flex_groups *flex_group = sbi->s_flex_groups;
	ext4_group_t parent_group = EXT4_I(parent)->i_block_group;
	ext4_group_t parent_fbg_group = ext4_flex_group(sbi, parent_group);
	ext4_group_t ngroups = ext4_get_groups_count(sb);
	int flex_size = ext4_flex_bg_size(sbi);
	ext4_group_t best_flex = parent_fbg_group;
	int blocks_per_flex = sbi->s_blocks_per_group * flex_size;
	int flexbg_free_blocks;
	int flex_freeb_ratio;
	ext4_group_t n_fbg_groups;
	ext4_group_t i;

	n_fbg_groups = (ngroups + flex_size - 1) >>
		sbi->s_log_groups_per_flex;

find_close_to_parent:
	flexbg_free_blocks = atomic_read(&flex_group[best_flex].free_blocks);
	flex_freeb_ratio = flexbg_free_blocks * 100 / blocks_per_flex;
	if (atomic_read(&flex_group[best_flex].free_inodes) &&
	    flex_freeb_ratio > free_block_ratio)
		goto found_flexbg;

	if (best_flex && best_flex == parent_fbg_group) {
		best_flex--;
		goto find_close_to_parent;
	}

	for (i = 0; i < n_fbg_groups; i++) {
		if (i == parent_fbg_group || i == parent_fbg_group - 1)
			continue;

		flexbg_free_blocks = atomic_read(&flex_group[i].free_blocks);
		flex_freeb_ratio = flexbg_free_blocks * 100 / blocks_per_flex;

		if (flex_freeb_ratio > free_block_ratio &&
		    (atomic_read(&flex_group[i].free_inodes))) {
			best_flex = i;
			goto found_flexbg;
		}

		if ((atomic_read(&flex_group[best_flex].free_inodes) == 0) ||
		    ((atomic_read(&flex_group[i].free_blocks) >
		      atomic_read(&flex_group[best_flex].free_blocks)) &&
		     atomic_read(&flex_group[i].free_inodes)))
			best_flex = i;
	}

	if (!atomic_read(&flex_group[best_flex].free_inodes) ||
	    !atomic_read(&flex_group[best_flex].free_blocks))
		return -1;

found_flexbg:
	for (i = best_flex * flex_size; i < ngroups &&
		     i < (best_flex + 1) * flex_size; i++) {
		desc = ext4_get_group_desc(sb, i, NULL);
		if (ext4_free_inodes_count(sb, desc)) {
			*best_group = i;
			goto out;
		}
	}

	return -1;
out:
	return 0;
}

struct orlov_stats {
	__u32 free_inodes;
	__u32 free_blocks;
=======
struct orlov_stats {
	__u64 free_clusters;
	__u32 free_inodes;
>>>>>>> refs/remotes/origin/cm-10.0
=======
struct orlov_stats {
	__u64 free_clusters;
	__u32 free_inodes;
>>>>>>> refs/remotes/origin/master
	__u32 used_dirs;
};

/*
 * Helper function for Orlov's allocator; returns critical information
 * for a particular block group or flex_bg.  If flex_size is 1, then g
 * is a block group number; otherwise it is flex_bg number.
 */
static void get_orlov_stats(struct super_block *sb, ext4_group_t g,
			    int flex_size, struct orlov_stats *stats)
{
	struct ext4_group_desc *desc;
	struct flex_groups *flex_group = EXT4_SB(sb)->s_flex_groups;

	if (flex_size > 1) {
		stats->free_inodes = atomic_read(&flex_group[g].free_inodes);
<<<<<<< HEAD
<<<<<<< HEAD
		stats->free_blocks = atomic_read(&flex_group[g].free_blocks);
=======
		stats->free_clusters = atomic64_read(&flex_group[g].free_clusters);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		stats->free_clusters = atomic64_read(&flex_group[g].free_clusters);
>>>>>>> refs/remotes/origin/master
		stats->used_dirs = atomic_read(&flex_group[g].used_dirs);
		return;
	}

	desc = ext4_get_group_desc(sb, g, NULL);
	if (desc) {
		stats->free_inodes = ext4_free_inodes_count(sb, desc);
<<<<<<< HEAD
<<<<<<< HEAD
		stats->free_blocks = ext4_free_blks_count(sb, desc);
		stats->used_dirs = ext4_used_dirs_count(sb, desc);
	} else {
		stats->free_inodes = 0;
		stats->free_blocks = 0;
=======
=======
>>>>>>> refs/remotes/origin/master
		stats->free_clusters = ext4_free_group_clusters(sb, desc);
		stats->used_dirs = ext4_used_dirs_count(sb, desc);
	} else {
		stats->free_inodes = 0;
		stats->free_clusters = 0;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		stats->used_dirs = 0;
	}
}

/*
 * Orlov's allocator for directories.
 *
 * We always try to spread first-level directories.
 *
 * If there are blockgroups with both free inodes and free blocks counts
 * not worse than average we return one with smallest directory count.
 * Otherwise we simply return a random group.
 *
 * For the rest rules look so:
 *
 * It's OK to put directory into a group unless
 * it has too many directories already (max_dirs) or
 * it has too few free inodes left (min_inodes) or
 * it has too few free blocks left (min_blocks) or
 * Parent's group is preferred, if it doesn't satisfy these
 * conditions we search cyclically through the rest. If none
 * of the groups look good we just look for a group with more
 * free inodes than average (starting at parent's group).
 */

static int find_group_orlov(struct super_block *sb, struct inode *parent,
<<<<<<< HEAD
<<<<<<< HEAD
			    ext4_group_t *group, int mode,
=======
			    ext4_group_t *group, umode_t mode,
>>>>>>> refs/remotes/origin/cm-10.0
=======
			    ext4_group_t *group, umode_t mode,
>>>>>>> refs/remotes/origin/master
			    const struct qstr *qstr)
{
	ext4_group_t parent_group = EXT4_I(parent)->i_block_group;
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	ext4_group_t real_ngroups = ext4_get_groups_count(sb);
	int inodes_per_group = EXT4_INODES_PER_GROUP(sb);
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned int freei, avefreei;
	ext4_fsblk_t freeb, avefreeb;
	unsigned int ndirs;
	int max_dirs, min_inodes;
	ext4_grpblk_t min_blocks;
=======
=======
>>>>>>> refs/remotes/origin/master
	unsigned int freei, avefreei, grp_free;
	ext4_fsblk_t freeb, avefreec;
	unsigned int ndirs;
	int max_dirs, min_inodes;
	ext4_grpblk_t min_clusters;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	ext4_group_t i, grp, g, ngroups;
	struct ext4_group_desc *desc;
	struct orlov_stats stats;
	int flex_size = ext4_flex_bg_size(sbi);
	struct dx_hash_info hinfo;

	ngroups = real_ngroups;
	if (flex_size > 1) {
		ngroups = (real_ngroups + flex_size - 1) >>
			sbi->s_log_groups_per_flex;
		parent_group >>= sbi->s_log_groups_per_flex;
	}

	freei = percpu_counter_read_positive(&sbi->s_freeinodes_counter);
	avefreei = freei / ngroups;
<<<<<<< HEAD
<<<<<<< HEAD
	freeb = percpu_counter_read_positive(&sbi->s_freeblocks_counter);
	avefreeb = freeb;
	do_div(avefreeb, ngroups);
=======
=======
>>>>>>> refs/remotes/origin/master
	freeb = EXT4_C2B(sbi,
		percpu_counter_read_positive(&sbi->s_freeclusters_counter));
	avefreec = freeb;
	do_div(avefreec, ngroups);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	ndirs = percpu_counter_read_positive(&sbi->s_dirs_counter);

	if (S_ISDIR(mode) &&
	    ((parent == sb->s_root->d_inode) ||
	     (ext4_test_inode_flag(parent, EXT4_INODE_TOPDIR)))) {
		int best_ndir = inodes_per_group;
		int ret = -1;

		if (qstr) {
			hinfo.hash_version = DX_HASH_HALF_MD4;
			hinfo.seed = sbi->s_hash_seed;
			ext4fs_dirhash(qstr->name, qstr->len, &hinfo);
			grp = hinfo.hash;
		} else
<<<<<<< HEAD
			get_random_bytes(&grp, sizeof(grp));
=======
			grp = prandom_u32();
>>>>>>> refs/remotes/origin/master
		parent_group = (unsigned)grp % ngroups;
		for (i = 0; i < ngroups; i++) {
			g = (parent_group + i) % ngroups;
			get_orlov_stats(sb, g, flex_size, &stats);
			if (!stats.free_inodes)
				continue;
			if (stats.used_dirs >= best_ndir)
				continue;
			if (stats.free_inodes < avefreei)
				continue;
<<<<<<< HEAD
<<<<<<< HEAD
			if (stats.free_blocks < avefreeb)
=======
			if (stats.free_clusters < avefreec)
>>>>>>> refs/remotes/origin/cm-10.0
=======
			if (stats.free_clusters < avefreec)
>>>>>>> refs/remotes/origin/master
				continue;
			grp = g;
			ret = 0;
			best_ndir = stats.used_dirs;
		}
		if (ret)
			goto fallback;
	found_flex_bg:
		if (flex_size == 1) {
			*group = grp;
			return 0;
		}

		/*
		 * We pack inodes at the beginning of the flexgroup's
		 * inode tables.  Block allocation decisions will do
		 * something similar, although regular files will
		 * start at 2nd block group of the flexgroup.  See
		 * ext4_ext_find_goal() and ext4_find_near().
		 */
		grp *= flex_size;
		for (i = 0; i < flex_size; i++) {
			if (grp+i >= real_ngroups)
				break;
			desc = ext4_get_group_desc(sb, grp+i, NULL);
			if (desc && ext4_free_inodes_count(sb, desc)) {
				*group = grp+i;
				return 0;
			}
		}
		goto fallback;
	}

	max_dirs = ndirs / ngroups + inodes_per_group / 16;
	min_inodes = avefreei - inodes_per_group*flex_size / 4;
	if (min_inodes < 1)
		min_inodes = 1;
<<<<<<< HEAD
<<<<<<< HEAD
	min_blocks = avefreeb - EXT4_BLOCKS_PER_GROUP(sb)*flex_size / 4;
=======
	min_clusters = avefreec - EXT4_CLUSTERS_PER_GROUP(sb)*flex_size / 4;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	min_clusters = avefreec - EXT4_CLUSTERS_PER_GROUP(sb)*flex_size / 4;
>>>>>>> refs/remotes/origin/master

	/*
	 * Start looking in the flex group where we last allocated an
	 * inode for this parent directory
	 */
	if (EXT4_I(parent)->i_last_alloc_group != ~0) {
		parent_group = EXT4_I(parent)->i_last_alloc_group;
		if (flex_size > 1)
			parent_group >>= sbi->s_log_groups_per_flex;
	}

	for (i = 0; i < ngroups; i++) {
		grp = (parent_group + i) % ngroups;
		get_orlov_stats(sb, grp, flex_size, &stats);
		if (stats.used_dirs >= max_dirs)
			continue;
		if (stats.free_inodes < min_inodes)
			continue;
<<<<<<< HEAD
<<<<<<< HEAD
		if (stats.free_blocks < min_blocks)
=======
		if (stats.free_clusters < min_clusters)
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (stats.free_clusters < min_clusters)
>>>>>>> refs/remotes/origin/master
			continue;
		goto found_flex_bg;
	}

fallback:
	ngroups = real_ngroups;
	avefreei = freei / ngroups;
fallback_retry:
	parent_group = EXT4_I(parent)->i_block_group;
	for (i = 0; i < ngroups; i++) {
		grp = (parent_group + i) % ngroups;
		desc = ext4_get_group_desc(sb, grp, NULL);
<<<<<<< HEAD
<<<<<<< HEAD
		if (desc && ext4_free_inodes_count(sb, desc) &&
		    ext4_free_inodes_count(sb, desc) >= avefreei) {
			*group = grp;
			return 0;
=======
=======
>>>>>>> refs/remotes/origin/master
		if (desc) {
			grp_free = ext4_free_inodes_count(sb, desc);
			if (grp_free && grp_free >= avefreei) {
				*group = grp;
				return 0;
			}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		}
	}

	if (avefreei) {
		/*
		 * The free-inodes counter is approximate, and for really small
		 * filesystems the above test can fail to find any blockgroups
		 */
		avefreei = 0;
		goto fallback_retry;
	}

	return -1;
}

static int find_group_other(struct super_block *sb, struct inode *parent,
<<<<<<< HEAD
<<<<<<< HEAD
			    ext4_group_t *group, int mode)
=======
			    ext4_group_t *group, umode_t mode)
>>>>>>> refs/remotes/origin/cm-10.0
=======
			    ext4_group_t *group, umode_t mode)
>>>>>>> refs/remotes/origin/master
{
	ext4_group_t parent_group = EXT4_I(parent)->i_block_group;
	ext4_group_t i, last, ngroups = ext4_get_groups_count(sb);
	struct ext4_group_desc *desc;
	int flex_size = ext4_flex_bg_size(EXT4_SB(sb));

	/*
	 * Try to place the inode is the same flex group as its
	 * parent.  If we can't find space, use the Orlov algorithm to
	 * find another flex group, and store that information in the
	 * parent directory's inode information so that use that flex
	 * group for future allocations.
	 */
	if (flex_size > 1) {
		int retry = 0;

	try_again:
		parent_group &= ~(flex_size-1);
		last = parent_group + flex_size;
		if (last > ngroups)
			last = ngroups;
		for  (i = parent_group; i < last; i++) {
			desc = ext4_get_group_desc(sb, i, NULL);
			if (desc && ext4_free_inodes_count(sb, desc)) {
				*group = i;
				return 0;
			}
		}
		if (!retry && EXT4_I(parent)->i_last_alloc_group != ~0) {
			retry = 1;
			parent_group = EXT4_I(parent)->i_last_alloc_group;
			goto try_again;
		}
		/*
		 * If this didn't work, use the Orlov search algorithm
		 * to find a new flex group; we pass in the mode to
		 * avoid the topdir algorithms.
		 */
		*group = parent_group + flex_size;
		if (*group > ngroups)
			*group = 0;
		return find_group_orlov(sb, parent, group, mode, NULL);
	}

	/*
	 * Try to place the inode in its parent directory
	 */
	*group = parent_group;
	desc = ext4_get_group_desc(sb, *group, NULL);
	if (desc && ext4_free_inodes_count(sb, desc) &&
<<<<<<< HEAD
<<<<<<< HEAD
			ext4_free_blks_count(sb, desc))
=======
	    ext4_free_group_clusters(sb, desc))
>>>>>>> refs/remotes/origin/cm-10.0
=======
	    ext4_free_group_clusters(sb, desc))
>>>>>>> refs/remotes/origin/master
		return 0;

	/*
	 * We're going to place this inode in a different blockgroup from its
	 * parent.  We want to cause files in a common directory to all land in
	 * the same blockgroup.  But we want files which are in a different
	 * directory which shares a blockgroup with our parent to land in a
	 * different blockgroup.
	 *
	 * So add our directory's i_ino into the starting point for the hash.
	 */
	*group = (*group + parent->i_ino) % ngroups;

	/*
	 * Use a quadratic hash to find a group with a free inode and some free
	 * blocks.
	 */
	for (i = 1; i < ngroups; i <<= 1) {
		*group += i;
		if (*group >= ngroups)
			*group -= ngroups;
		desc = ext4_get_group_desc(sb, *group, NULL);
		if (desc && ext4_free_inodes_count(sb, desc) &&
<<<<<<< HEAD
<<<<<<< HEAD
				ext4_free_blks_count(sb, desc))
=======
		    ext4_free_group_clusters(sb, desc))
>>>>>>> refs/remotes/origin/cm-10.0
=======
		    ext4_free_group_clusters(sb, desc))
>>>>>>> refs/remotes/origin/master
			return 0;
	}

	/*
	 * That failed: try linear search for a free inode, even if that group
	 * has no free blocks.
	 */
	*group = parent_group;
	for (i = 0; i < ngroups; i++) {
		if (++*group >= ngroups)
			*group = 0;
		desc = ext4_get_group_desc(sb, *group, NULL);
		if (desc && ext4_free_inodes_count(sb, desc))
			return 0;
	}

	return -1;
}

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * claim the inode from the inode bitmap. If the group
 * is uninit we need to take the groups's ext4_group_lock
 * and clear the uninit flag. The inode bitmap update
 * and group desc uninit flag clear should be done
 * after holding ext4_group_lock so that ext4_read_inode_bitmap
 * doesn't race with the ext4_claim_inode
 */
static int ext4_claim_inode(struct super_block *sb,
			struct buffer_head *inode_bitmap_bh,
			unsigned long ino, ext4_group_t group, int mode)
{
	int free = 0, retval = 0, count;
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	struct ext4_group_info *grp = ext4_get_group_info(sb, group);
	struct ext4_group_desc *gdp = ext4_get_group_desc(sb, group, NULL);

	/*
	 * We have to be sure that new inode allocation does not race with
	 * inode table initialization, because otherwise we may end up
	 * allocating and writing new inode right before sb_issue_zeroout
	 * takes place and overwriting our new inode with zeroes. So we
	 * take alloc_sem to prevent it.
	 */
	down_read(&grp->alloc_sem);
	ext4_lock_group(sb, group);
	if (ext4_set_bit(ino, inode_bitmap_bh->b_data)) {
		/* not a free inode */
		retval = 1;
		goto err_ret;
	}
	ino++;
	if ((group == 0 && ino < EXT4_FIRST_INO(sb)) ||
			ino > EXT4_INODES_PER_GROUP(sb)) {
		ext4_unlock_group(sb, group);
		up_read(&grp->alloc_sem);
		ext4_error(sb, "reserved inode or inode > inodes count - "
			   "block_group = %u, inode=%lu", group,
			   ino + group * EXT4_INODES_PER_GROUP(sb));
		return 1;
	}
	/* If we didn't allocate from within the initialized part of the inode
	 * table then we need to initialize up to this inode. */
	if (EXT4_HAS_RO_COMPAT_FEATURE(sb, EXT4_FEATURE_RO_COMPAT_GDT_CSUM)) {

		if (gdp->bg_flags & cpu_to_le16(EXT4_BG_INODE_UNINIT)) {
			gdp->bg_flags &= cpu_to_le16(~EXT4_BG_INODE_UNINIT);
			/* When marking the block group with
			 * ~EXT4_BG_INODE_UNINIT we don't want to depend
			 * on the value of bg_itable_unused even though
			 * mke2fs could have initialized the same for us.
			 * Instead we calculated the value below
			 */

			free = 0;
		} else {
			free = EXT4_INODES_PER_GROUP(sb) -
				ext4_itable_unused_count(sb, gdp);
		}

		/*
		 * Check the relative inode number against the last used
		 * relative inode number in this group. if it is greater
		 * we need to  update the bg_itable_unused count
		 *
		 */
		if (ino > free)
			ext4_itable_unused_set(sb, gdp,
					(EXT4_INODES_PER_GROUP(sb) - ino));
	}
	count = ext4_free_inodes_count(sb, gdp) - 1;
	ext4_free_inodes_set(sb, gdp, count);
	if (S_ISDIR(mode)) {
		count = ext4_used_dirs_count(sb, gdp) + 1;
		ext4_used_dirs_set(sb, gdp, count);
		if (sbi->s_log_groups_per_flex) {
			ext4_group_t f = ext4_flex_group(sbi, group);

			atomic_inc(&sbi->s_flex_groups[f].used_dirs);
		}
	}
	gdp->bg_checksum = ext4_group_desc_csum(sbi, group, gdp);
err_ret:
	ext4_unlock_group(sb, group);
	up_read(&grp->alloc_sem);
	return retval;
}

/*
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * In no journal mode, if an inode has recently been deleted, we want
 * to avoid reusing it until we're reasonably sure the inode table
 * block has been written back to disk.  (Yes, these values are
 * somewhat arbitrary...)
 */
#define RECENTCY_MIN	5
#define RECENTCY_DIRTY	30

static int recently_deleted(struct super_block *sb, ext4_group_t group, int ino)
{
	struct ext4_group_desc	*gdp;
	struct ext4_inode	*raw_inode;
	struct buffer_head	*bh;
	unsigned long		dtime, now;
	int	inodes_per_block = EXT4_SB(sb)->s_inodes_per_block;
	int	offset, ret = 0, recentcy = RECENTCY_MIN;

	gdp = ext4_get_group_desc(sb, group, NULL);
	if (unlikely(!gdp))
		return 0;

	bh = sb_getblk(sb, ext4_inode_table(sb, gdp) +
		       (ino / inodes_per_block));
	if (unlikely(!bh) || !buffer_uptodate(bh))
		/*
		 * If the block is not in the buffer cache, then it
		 * must have been written out.
		 */
		goto out;

	offset = (ino % inodes_per_block) * EXT4_INODE_SIZE(sb);
	raw_inode = (struct ext4_inode *) (bh->b_data + offset);
	dtime = le32_to_cpu(raw_inode->i_dtime);
	now = get_seconds();
	if (buffer_dirty(bh))
		recentcy += RECENTCY_DIRTY;

	if (dtime && (dtime < now) && (now < dtime + recentcy))
		ret = 1;
out:
	brelse(bh);
	return ret;
}

/*
>>>>>>> refs/remotes/origin/master
 * There are two policies for allocating an inode.  If the new inode is
 * a directory, then a forward search is made for a block group with both
 * free space and a low directory-to-inode ratio; if that fails, then of
 * the groups with above-average free space, that group with the fewest
 * directories already is chosen.
 *
 * For other inodes, search forward from the parent directory's block
 * group to find a free inode.
 */
<<<<<<< HEAD
<<<<<<< HEAD
struct inode *ext4_new_inode(handle_t *handle, struct inode *dir, int mode,
			     const struct qstr *qstr, __u32 goal)
=======
struct inode *ext4_new_inode(handle_t *handle, struct inode *dir, umode_t mode,
			     const struct qstr *qstr, __u32 goal, uid_t *owner)
>>>>>>> refs/remotes/origin/cm-10.0
=======
struct inode *__ext4_new_inode(handle_t *handle, struct inode *dir,
			       umode_t mode, const struct qstr *qstr,
			       __u32 goal, uid_t *owner, int handle_type,
			       unsigned int line_no, int nblocks)
>>>>>>> refs/remotes/origin/master
{
	struct super_block *sb;
	struct buffer_head *inode_bitmap_bh = NULL;
	struct buffer_head *group_desc_bh;
	ext4_group_t ngroups, group = 0;
	unsigned long ino = 0;
	struct inode *inode;
	struct ext4_group_desc *gdp = NULL;
	struct ext4_inode_info *ei;
	struct ext4_sb_info *sbi;
	int ret2, err = 0;
	struct inode *ret;
	ext4_group_t i;
<<<<<<< HEAD
<<<<<<< HEAD
	int free = 0;
	static int once = 1;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	ext4_group_t flex_group;
=======
	ext4_group_t flex_group;
	struct ext4_group_info *grp;
>>>>>>> refs/remotes/origin/master

	/* Cannot create files in a deleted directory */
	if (!dir || !dir->i_nlink)
		return ERR_PTR(-EPERM);

	sb = dir->i_sb;
	ngroups = ext4_get_groups_count(sb);
	trace_ext4_request_inode(dir, mode);
	inode = new_inode(sb);
	if (!inode)
		return ERR_PTR(-ENOMEM);
	ei = EXT4_I(inode);
	sbi = EXT4_SB(sb);

<<<<<<< HEAD
=======
	/*
	 * Initalize owners and quota early so that we don't have to account
	 * for quota initialization worst case in standard inode creating
	 * transaction
	 */
	if (owner) {
		inode->i_mode = mode;
		i_uid_write(inode, owner[0]);
		i_gid_write(inode, owner[1]);
	} else if (test_opt(sb, GRPID)) {
		inode->i_mode = mode;
		inode->i_uid = current_fsuid();
		inode->i_gid = dir->i_gid;
	} else
		inode_init_owner(inode, dir, mode);
	dquot_initialize(inode);

>>>>>>> refs/remotes/origin/master
	if (!goal)
		goal = sbi->s_inode_goal;

	if (goal && goal <= le32_to_cpu(sbi->s_es->s_inodes_count)) {
		group = (goal - 1) / EXT4_INODES_PER_GROUP(sb);
		ino = (goal - 1) % EXT4_INODES_PER_GROUP(sb);
		ret2 = 0;
		goto got_group;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (sbi->s_log_groups_per_flex && test_opt(sb, OLDALLOC)) {
		ret2 = find_group_flex(sb, dir, &group);
		if (ret2 == -1) {
			ret2 = find_group_other(sb, dir, &group, mode);
			if (ret2 == 0 && once) {
				once = 0;
				printk(KERN_NOTICE "ext4: find_group_flex "
				       "failed, fallback succeeded dir %lu\n",
				       dir->i_ino);
			}
		}
		goto got_group;
	}

	if (S_ISDIR(mode)) {
		if (test_opt(sb, OLDALLOC))
			ret2 = find_group_dir(sb, dir, &group);
		else
			ret2 = find_group_orlov(sb, dir, &group, mode, qstr);
	} else
=======
	if (S_ISDIR(mode))
		ret2 = find_group_orlov(sb, dir, &group, mode, qstr);
	else
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (S_ISDIR(mode))
		ret2 = find_group_orlov(sb, dir, &group, mode, qstr);
	else
>>>>>>> refs/remotes/origin/master
		ret2 = find_group_other(sb, dir, &group, mode);

got_group:
	EXT4_I(dir)->i_last_alloc_group = group;
	err = -ENOSPC;
	if (ret2 == -1)
		goto out;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Normally we will only go through one pass of this loop,
	 * unless we get unlucky and it turns out the group we selected
	 * had its last inode grabbed by someone else.
	 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	for (i = 0; i < ngroups; i++, ino = 0) {
		err = -EIO;

		gdp = ext4_get_group_desc(sb, group, &group_desc_bh);
		if (!gdp)
<<<<<<< HEAD
			goto fail;

		brelse(inode_bitmap_bh);
		inode_bitmap_bh = ext4_read_inode_bitmap(sb, group);
		if (!inode_bitmap_bh)
			goto fail;
=======
			goto out;

		/*
		 * Check free inodes count before loading bitmap.
		 */
		if (ext4_free_inodes_count(sb, gdp) == 0) {
			if (++group == ngroups)
				group = 0;
			continue;
		}

		grp = ext4_get_group_info(sb, group);
		/* Skip groups with already-known suspicious inode tables */
		if (EXT4_MB_GRP_IBITMAP_CORRUPT(grp)) {
			if (++group == ngroups)
				group = 0;
			continue;
		}

		brelse(inode_bitmap_bh);
		inode_bitmap_bh = ext4_read_inode_bitmap(sb, group);
		/* Skip groups with suspicious inode tables */
		if (EXT4_MB_GRP_IBITMAP_CORRUPT(grp) || !inode_bitmap_bh) {
			if (++group == ngroups)
				group = 0;
			continue;
		}
>>>>>>> refs/remotes/origin/master

repeat_in_this_group:
		ino = ext4_find_next_zero_bit((unsigned long *)
					      inode_bitmap_bh->b_data,
					      EXT4_INODES_PER_GROUP(sb), ino);
<<<<<<< HEAD
<<<<<<< HEAD

		if (ino < EXT4_INODES_PER_GROUP(sb)) {

			BUFFER_TRACE(inode_bitmap_bh, "get_write_access");
			err = ext4_journal_get_write_access(handle,
							    inode_bitmap_bh);
			if (err)
				goto fail;

			BUFFER_TRACE(group_desc_bh, "get_write_access");
			err = ext4_journal_get_write_access(handle,
								group_desc_bh);
			if (err)
				goto fail;
			if (!ext4_claim_inode(sb, inode_bitmap_bh,
						ino, group, mode)) {
				/* we won it */
				BUFFER_TRACE(inode_bitmap_bh,
					"call ext4_handle_dirty_metadata");
				err = ext4_handle_dirty_metadata(handle,
								 NULL,
							inode_bitmap_bh);
				if (err)
					goto fail;
				/* zero bit is inode number 1*/
				ino++;
				goto got;
			}
			/* we lost it */
			ext4_handle_release_buffer(handle, inode_bitmap_bh);
			ext4_handle_release_buffer(handle, group_desc_bh);

			if (++ino < EXT4_INODES_PER_GROUP(sb))
				goto repeat_in_this_group;
		}

		/*
		 * This case is possible in concurrent environment.  It is very
		 * rare.  We cannot repeat the find_group_xxx() call because
		 * that will simply return the same blockgroup, because the
		 * group descriptor metadata has not yet been updated.
		 * So we just go onto the next blockgroup.
		 */
=======
=======
>>>>>>> refs/remotes/origin/master
		if (ino >= EXT4_INODES_PER_GROUP(sb))
			goto next_group;
		if (group == 0 && (ino+1) < EXT4_FIRST_INO(sb)) {
			ext4_error(sb, "reserved inode found cleared - "
				   "inode=%lu", ino + 1);
			continue;
		}
<<<<<<< HEAD
		BUFFER_TRACE(inode_bitmap_bh, "get_write_access");
		err = ext4_journal_get_write_access(handle, inode_bitmap_bh);
		if (err)
			goto fail;
=======
		if ((EXT4_SB(sb)->s_journal == NULL) &&
		    recently_deleted(sb, group, ino)) {
			ino++;
			goto next_inode;
		}
		if (!handle) {
			BUG_ON(nblocks <= 0);
			handle = __ext4_journal_start_sb(dir->i_sb, line_no,
							 handle_type, nblocks,
							 0);
			if (IS_ERR(handle)) {
				err = PTR_ERR(handle);
				ext4_std_error(sb, err);
				goto out;
			}
		}
		BUFFER_TRACE(inode_bitmap_bh, "get_write_access");
		err = ext4_journal_get_write_access(handle, inode_bitmap_bh);
		if (err) {
			ext4_std_error(sb, err);
			goto out;
		}
>>>>>>> refs/remotes/origin/master
		ext4_lock_group(sb, group);
		ret2 = ext4_test_and_set_bit(ino, inode_bitmap_bh->b_data);
		ext4_unlock_group(sb, group);
		ino++;		/* the inode bitmap is zero-based */
		if (!ret2)
			goto got; /* we grabbed the inode! */
<<<<<<< HEAD
		if (ino < EXT4_INODES_PER_GROUP(sb))
			goto repeat_in_this_group;
next_group:
>>>>>>> refs/remotes/origin/cm-10.0
=======
next_inode:
		if (ino < EXT4_INODES_PER_GROUP(sb))
			goto repeat_in_this_group;
next_group:
>>>>>>> refs/remotes/origin/master
		if (++group == ngroups)
			group = 0;
	}
	err = -ENOSPC;
	goto out;

got:
<<<<<<< HEAD
<<<<<<< HEAD
=======
	BUFFER_TRACE(inode_bitmap_bh, "call ext4_handle_dirty_metadata");
	err = ext4_handle_dirty_metadata(handle, NULL, inode_bitmap_bh);
	if (err)
		goto fail;

>>>>>>> refs/remotes/origin/cm-10.0
	/* We may have to initialize the block bitmap if it isn't already */
	if (EXT4_HAS_RO_COMPAT_FEATURE(sb, EXT4_FEATURE_RO_COMPAT_GDT_CSUM) &&
=======
	BUFFER_TRACE(inode_bitmap_bh, "call ext4_handle_dirty_metadata");
	err = ext4_handle_dirty_metadata(handle, NULL, inode_bitmap_bh);
	if (err) {
		ext4_std_error(sb, err);
		goto out;
	}

	/* We may have to initialize the block bitmap if it isn't already */
	if (ext4_has_group_desc_csum(sb) &&
>>>>>>> refs/remotes/origin/master
	    gdp->bg_flags & cpu_to_le16(EXT4_BG_BLOCK_UNINIT)) {
		struct buffer_head *block_bitmap_bh;

		block_bitmap_bh = ext4_read_block_bitmap(sb, group);
		BUFFER_TRACE(block_bitmap_bh, "get block bitmap access");
		err = ext4_journal_get_write_access(handle, block_bitmap_bh);
		if (err) {
			brelse(block_bitmap_bh);
<<<<<<< HEAD
			goto fail;
		}

<<<<<<< HEAD
		free = 0;
		ext4_lock_group(sb, group);
		/* recheck and clear flag under lock if we still need to */
		if (gdp->bg_flags & cpu_to_le16(EXT4_BG_BLOCK_UNINIT)) {
			free = ext4_free_blocks_after_init(sb, group, gdp);
			gdp->bg_flags &= cpu_to_le16(~EXT4_BG_BLOCK_UNINIT);
			ext4_free_blks_set(sb, gdp, free);
=======
=======
			ext4_std_error(sb, err);
			goto out;
		}

>>>>>>> refs/remotes/origin/master
		BUFFER_TRACE(block_bitmap_bh, "dirty block bitmap");
		err = ext4_handle_dirty_metadata(handle, NULL, block_bitmap_bh);

		/* recheck and clear flag under lock if we still need to */
		ext4_lock_group(sb, group);
		if (gdp->bg_flags & cpu_to_le16(EXT4_BG_BLOCK_UNINIT)) {
			gdp->bg_flags &= cpu_to_le16(~EXT4_BG_BLOCK_UNINIT);
			ext4_free_group_clusters_set(sb, gdp,
				ext4_free_clusters_after_init(sb, group, gdp));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
			gdp->bg_checksum = ext4_group_desc_csum(sbi, group,
								gdp);
		}
		ext4_unlock_group(sb, group);
<<<<<<< HEAD

		/* Don't need to dirty bitmap block if we didn't change it */
		if (free) {
			BUFFER_TRACE(block_bitmap_bh, "dirty block bitmap");
			err = ext4_handle_dirty_metadata(handle,
							NULL, block_bitmap_bh);
		}

		brelse(block_bitmap_bh);
		if (err)
			goto fail;
	}
=======
		brelse(block_bitmap_bh);

		if (err)
			goto fail;
=======
			ext4_block_bitmap_csum_set(sb, group, gdp,
						   block_bitmap_bh);
			ext4_group_desc_csum_set(sb, group, gdp);
		}
		ext4_unlock_group(sb, group);
		brelse(block_bitmap_bh);

		if (err) {
			ext4_std_error(sb, err);
			goto out;
		}
>>>>>>> refs/remotes/origin/master
	}

	BUFFER_TRACE(group_desc_bh, "get_write_access");
	err = ext4_journal_get_write_access(handle, group_desc_bh);
<<<<<<< HEAD
	if (err)
		goto fail;

	/* Update the relevant bg descriptor fields */
	if (EXT4_HAS_RO_COMPAT_FEATURE(sb, EXT4_FEATURE_RO_COMPAT_GDT_CSUM)) {
=======
	if (err) {
		ext4_std_error(sb, err);
		goto out;
	}

	/* Update the relevant bg descriptor fields */
	if (ext4_has_group_desc_csum(sb)) {
>>>>>>> refs/remotes/origin/master
		int free;
		struct ext4_group_info *grp = ext4_get_group_info(sb, group);

		down_read(&grp->alloc_sem); /* protect vs itable lazyinit */
		ext4_lock_group(sb, group); /* while we modify the bg desc */
		free = EXT4_INODES_PER_GROUP(sb) -
			ext4_itable_unused_count(sb, gdp);
		if (gdp->bg_flags & cpu_to_le16(EXT4_BG_INODE_UNINIT)) {
			gdp->bg_flags &= cpu_to_le16(~EXT4_BG_INODE_UNINIT);
			free = 0;
		}
		/*
		 * Check the relative inode number against the last used
		 * relative inode number in this group. if it is greater
		 * we need to update the bg_itable_unused count
		 */
		if (ino > free)
			ext4_itable_unused_set(sb, gdp,
					(EXT4_INODES_PER_GROUP(sb) - ino));
		up_read(&grp->alloc_sem);
	} else {
		ext4_lock_group(sb, group);
	}

	ext4_free_inodes_set(sb, gdp, ext4_free_inodes_count(sb, gdp) - 1);
	if (S_ISDIR(mode)) {
		ext4_used_dirs_set(sb, gdp, ext4_used_dirs_count(sb, gdp) + 1);
		if (sbi->s_log_groups_per_flex) {
			ext4_group_t f = ext4_flex_group(sbi, group);

			atomic_inc(&sbi->s_flex_groups[f].used_dirs);
		}
	}
<<<<<<< HEAD
	if (EXT4_HAS_RO_COMPAT_FEATURE(sb, EXT4_FEATURE_RO_COMPAT_GDT_CSUM)) {
		gdp->bg_checksum = ext4_group_desc_csum(sbi, group, gdp);
	}
	ext4_unlock_group(sb, group);

>>>>>>> refs/remotes/origin/cm-10.0
	BUFFER_TRACE(group_desc_bh, "call ext4_handle_dirty_metadata");
	err = ext4_handle_dirty_metadata(handle, NULL, group_desc_bh);
	if (err)
		goto fail;
=======
	if (ext4_has_group_desc_csum(sb)) {
		ext4_inode_bitmap_csum_set(sb, group, gdp, inode_bitmap_bh,
					   EXT4_INODES_PER_GROUP(sb) / 8);
		ext4_group_desc_csum_set(sb, group, gdp);
	}
	ext4_unlock_group(sb, group);

	BUFFER_TRACE(group_desc_bh, "call ext4_handle_dirty_metadata");
	err = ext4_handle_dirty_metadata(handle, NULL, group_desc_bh);
	if (err) {
		ext4_std_error(sb, err);
		goto out;
	}
>>>>>>> refs/remotes/origin/master

	percpu_counter_dec(&sbi->s_freeinodes_counter);
	if (S_ISDIR(mode))
		percpu_counter_inc(&sbi->s_dirs_counter);
<<<<<<< HEAD
	ext4_mark_super_dirty(sb);
=======
>>>>>>> refs/remotes/origin/master

	if (sbi->s_log_groups_per_flex) {
		flex_group = ext4_flex_group(sbi, group);
		atomic_dec(&sbi->s_flex_groups[flex_group].free_inodes);
	}
<<<<<<< HEAD
<<<<<<< HEAD

	if (test_opt(sb, GRPID)) {
=======
	if (owner) {
		inode->i_mode = mode;
		inode->i_uid = owner[0];
		inode->i_gid = owner[1];
	} else if (test_opt(sb, GRPID)) {
>>>>>>> refs/remotes/origin/cm-10.0
		inode->i_mode = mode;
		inode->i_uid = current_fsuid();
		inode->i_gid = dir->i_gid;
	} else
		inode_init_owner(inode, dir, mode);
=======
>>>>>>> refs/remotes/origin/master

	inode->i_ino = ino + group * EXT4_INODES_PER_GROUP(sb);
	/* This is the optimal IO size (for stat), not the fs block size */
	inode->i_blocks = 0;
	inode->i_mtime = inode->i_atime = inode->i_ctime = ei->i_crtime =
						       ext4_current_time(inode);

	memset(ei->i_data, 0, sizeof(ei->i_data));
	ei->i_dir_start_lookup = 0;
	ei->i_disksize = 0;

<<<<<<< HEAD
<<<<<<< HEAD
	/*
	 * Don't inherit extent flag from directory, amongst others. We set
	 * extent flag on newly created directory and file only if -o extent
	 * mount option is specified
	 */
=======
	/* Don't inherit extent flag from directory, amongst others. */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* Don't inherit extent flag from directory, amongst others. */
>>>>>>> refs/remotes/origin/master
	ei->i_flags =
		ext4_mask_flags(mode, EXT4_I(dir)->i_flags & EXT4_FL_INHERITED);
	ei->i_file_acl = 0;
	ei->i_dtime = 0;
	ei->i_block_group = group;
	ei->i_last_alloc_group = ~0;

	ext4_set_inode_flags(inode);
	if (IS_DIRSYNC(inode))
		ext4_handle_sync(handle);
	if (insert_inode_locked(inode) < 0) {
		/*
		 * Likely a bitmap corruption causing inode to be allocated
		 * twice.
		 */
		err = -EIO;
<<<<<<< HEAD
		goto fail;
=======
		ext4_error(sb, "failed to insert inode %lu: doubly allocated?",
			   inode->i_ino);
		goto out;
>>>>>>> refs/remotes/origin/master
	}
	spin_lock(&sbi->s_next_gen_lock);
	inode->i_generation = sbi->s_next_generation++;
	spin_unlock(&sbi->s_next_gen_lock);

<<<<<<< HEAD
=======
	/* Precompute checksum seed for inode metadata */
	if (EXT4_HAS_RO_COMPAT_FEATURE(sb,
			EXT4_FEATURE_RO_COMPAT_METADATA_CSUM)) {
		__u32 csum;
		__le32 inum = cpu_to_le32(inode->i_ino);
		__le32 gen = cpu_to_le32(inode->i_generation);
		csum = ext4_chksum(sbi, sbi->s_csum_seed, (__u8 *)&inum,
				   sizeof(inum));
		ei->i_csum_seed = ext4_chksum(sbi, csum, (__u8 *)&gen,
					      sizeof(gen));
	}

>>>>>>> refs/remotes/origin/master
	ext4_clear_state_flags(ei); /* Only relevant on 32-bit archs */
	ext4_set_inode_state(inode, EXT4_STATE_NEW);

	ei->i_extra_isize = EXT4_SB(sb)->s_want_extra_isize;

<<<<<<< HEAD
	ret = inode;
	dquot_initialize(inode);
=======
	ei->i_inline_off = 0;
	if (EXT4_HAS_INCOMPAT_FEATURE(sb, EXT4_FEATURE_INCOMPAT_INLINE_DATA))
		ext4_set_inode_state(inode, EXT4_STATE_MAY_INLINE_DATA);

	ret = inode;
>>>>>>> refs/remotes/origin/master
	err = dquot_alloc_inode(inode);
	if (err)
		goto fail_drop;

	err = ext4_init_acl(handle, inode, dir);
	if (err)
		goto fail_free_drop;

	err = ext4_init_security(handle, inode, dir, qstr);
	if (err)
		goto fail_free_drop;

	if (EXT4_HAS_INCOMPAT_FEATURE(sb, EXT4_FEATURE_INCOMPAT_EXTENTS)) {
		/* set extent flag only for directory, file and normal symlink*/
		if (S_ISDIR(mode) || S_ISREG(mode) || S_ISLNK(mode)) {
			ext4_set_inode_flag(inode, EXT4_INODE_EXTENTS);
			ext4_ext_tree_init(handle, inode);
		}
	}

	if (ext4_handle_valid(handle)) {
		ei->i_sync_tid = handle->h_transaction->t_tid;
		ei->i_datasync_tid = handle->h_transaction->t_tid;
	}

	err = ext4_mark_inode_dirty(handle, inode);
	if (err) {
		ext4_std_error(sb, err);
		goto fail_free_drop;
	}

	ext4_debug("allocating inode %lu\n", inode->i_ino);
	trace_ext4_allocate_inode(inode, dir, mode);
<<<<<<< HEAD
	goto really_out;
fail:
	ext4_std_error(sb, err);
out:
	iput(inode);
	ret = ERR_PTR(err);
really_out:
=======
>>>>>>> refs/remotes/origin/master
	brelse(inode_bitmap_bh);
	return ret;

fail_free_drop:
	dquot_free_inode(inode);
<<<<<<< HEAD

fail_drop:
	dquot_drop(inode);
	inode->i_flags |= S_NOQUOTA;
<<<<<<< HEAD
	inode->i_nlink = 0;
=======
	clear_nlink(inode);
>>>>>>> refs/remotes/origin/cm-10.0
	unlock_new_inode(inode);
=======
fail_drop:
	clear_nlink(inode);
	unlock_new_inode(inode);
out:
	dquot_drop(inode);
	inode->i_flags |= S_NOQUOTA;
>>>>>>> refs/remotes/origin/master
	iput(inode);
	brelse(inode_bitmap_bh);
	return ERR_PTR(err);
}

/* Verify that we are loading a valid orphan from disk */
struct inode *ext4_orphan_get(struct super_block *sb, unsigned long ino)
{
	unsigned long max_ino = le32_to_cpu(EXT4_SB(sb)->s_es->s_inodes_count);
	ext4_group_t block_group;
	int bit;
	struct buffer_head *bitmap_bh;
	struct inode *inode = NULL;
	long err = -EIO;

	/* Error cases - e2fsck has already cleaned up for us */
	if (ino > max_ino) {
		ext4_warning(sb, "bad orphan ino %lu!  e2fsck was run?", ino);
		goto error;
	}

	block_group = (ino - 1) / EXT4_INODES_PER_GROUP(sb);
	bit = (ino - 1) % EXT4_INODES_PER_GROUP(sb);
	bitmap_bh = ext4_read_inode_bitmap(sb, block_group);
	if (!bitmap_bh) {
		ext4_warning(sb, "inode bitmap error for orphan %lu", ino);
		goto error;
	}

	/* Having the inode bit set should be a 100% indicator that this
	 * is a valid orphan (no e2fsck run on fs).  Orphans also include
	 * inodes that were being truncated, so we can't check i_nlink==0.
	 */
	if (!ext4_test_bit(bit, bitmap_bh->b_data))
		goto bad_orphan;

	inode = ext4_iget(sb, ino);
	if (IS_ERR(inode))
		goto iget_failed;

	/*
	 * If the orphans has i_nlinks > 0 then it should be able to be
	 * truncated, otherwise it won't be removed from the orphan list
	 * during processing and an infinite loop will result.
	 */
	if (inode->i_nlink && !ext4_can_truncate(inode))
		goto bad_orphan;

	if (NEXT_ORPHAN(inode) > max_ino)
		goto bad_orphan;
	brelse(bitmap_bh);
	return inode;

iget_failed:
	err = PTR_ERR(inode);
	inode = NULL;
bad_orphan:
	ext4_warning(sb, "bad orphan inode %lu!  e2fsck was run?", ino);
<<<<<<< HEAD
	printk(KERN_NOTICE "ext4_test_bit(bit=%d, block=%llu) = %d\n",
	       bit, (unsigned long long)bitmap_bh->b_blocknr,
	       ext4_test_bit(bit, bitmap_bh->b_data));
	printk(KERN_NOTICE "inode=%p\n", inode);
	if (inode) {
		printk(KERN_NOTICE "is_bad_inode(inode)=%d\n",
		       is_bad_inode(inode));
		printk(KERN_NOTICE "NEXT_ORPHAN(inode)=%u\n",
		       NEXT_ORPHAN(inode));
		printk(KERN_NOTICE "max_ino=%lu\n", max_ino);
		printk(KERN_NOTICE "i_nlink=%u\n", inode->i_nlink);
=======
	printk(KERN_WARNING "ext4_test_bit(bit=%d, block=%llu) = %d\n",
	       bit, (unsigned long long)bitmap_bh->b_blocknr,
	       ext4_test_bit(bit, bitmap_bh->b_data));
	printk(KERN_WARNING "inode=%p\n", inode);
	if (inode) {
		printk(KERN_WARNING "is_bad_inode(inode)=%d\n",
		       is_bad_inode(inode));
		printk(KERN_WARNING "NEXT_ORPHAN(inode)=%u\n",
		       NEXT_ORPHAN(inode));
		printk(KERN_WARNING "max_ino=%lu\n", max_ino);
		printk(KERN_WARNING "i_nlink=%u\n", inode->i_nlink);
>>>>>>> refs/remotes/origin/master
		/* Avoid freeing blocks if we got a bad deleted inode */
		if (inode->i_nlink == 0)
			inode->i_blocks = 0;
		iput(inode);
	}
	brelse(bitmap_bh);
error:
	return ERR_PTR(err);
}

unsigned long ext4_count_free_inodes(struct super_block *sb)
{
	unsigned long desc_count;
	struct ext4_group_desc *gdp;
	ext4_group_t i, ngroups = ext4_get_groups_count(sb);
#ifdef EXT4FS_DEBUG
	struct ext4_super_block *es;
	unsigned long bitmap_count, x;
	struct buffer_head *bitmap_bh = NULL;

	es = EXT4_SB(sb)->s_es;
	desc_count = 0;
	bitmap_count = 0;
	gdp = NULL;
	for (i = 0; i < ngroups; i++) {
		gdp = ext4_get_group_desc(sb, i, NULL);
		if (!gdp)
			continue;
		desc_count += ext4_free_inodes_count(sb, gdp);
		brelse(bitmap_bh);
		bitmap_bh = ext4_read_inode_bitmap(sb, i);
		if (!bitmap_bh)
			continue;

		x = ext4_count_free(bitmap_bh->b_data,
				    EXT4_INODES_PER_GROUP(sb) / 8);
		printk(KERN_DEBUG "group %lu: stored = %d, counted = %lu\n",
			(unsigned long) i, ext4_free_inodes_count(sb, gdp), x);
		bitmap_count += x;
	}
	brelse(bitmap_bh);
	printk(KERN_DEBUG "ext4_count_free_inodes: "
	       "stored = %u, computed = %lu, %lu\n",
	       le32_to_cpu(es->s_free_inodes_count), desc_count, bitmap_count);
	return desc_count;
#else
	desc_count = 0;
	for (i = 0; i < ngroups; i++) {
		gdp = ext4_get_group_desc(sb, i, NULL);
		if (!gdp)
			continue;
		desc_count += ext4_free_inodes_count(sb, gdp);
		cond_resched();
	}
	return desc_count;
#endif
}

/* Called at mount-time, super-block is locked */
unsigned long ext4_count_dirs(struct super_block * sb)
{
	unsigned long count = 0;
	ext4_group_t i, ngroups = ext4_get_groups_count(sb);

	for (i = 0; i < ngroups; i++) {
		struct ext4_group_desc *gdp = ext4_get_group_desc(sb, i, NULL);
		if (!gdp)
			continue;
		count += ext4_used_dirs_count(sb, gdp);
	}
	return count;
}

/*
 * Zeroes not yet zeroed inode table - just write zeroes through the whole
 * inode table. Must be called without any spinlock held. The only place
 * where it is called from on active part of filesystem is ext4lazyinit
 * thread, so we do not need any special locks, however we have to prevent
 * inode allocation from the current group, so we take alloc_sem lock, to
<<<<<<< HEAD
<<<<<<< HEAD
 * block ext4_claim_inode until we are finished.
 */
extern int ext4_init_inode_table(struct super_block *sb, ext4_group_t group,
=======
 * block ext4_new_inode() until we are finished.
 */
int ext4_init_inode_table(struct super_block *sb, ext4_group_t group,
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * block ext4_new_inode() until we are finished.
 */
int ext4_init_inode_table(struct super_block *sb, ext4_group_t group,
>>>>>>> refs/remotes/origin/master
				 int barrier)
{
	struct ext4_group_info *grp = ext4_get_group_info(sb, group);
	struct ext4_sb_info *sbi = EXT4_SB(sb);
	struct ext4_group_desc *gdp = NULL;
	struct buffer_head *group_desc_bh;
	handle_t *handle;
	ext4_fsblk_t blk;
	int num, ret = 0, used_blks = 0;

	/* This should not happen, but just to be sure check this */
	if (sb->s_flags & MS_RDONLY) {
		ret = 1;
		goto out;
	}

	gdp = ext4_get_group_desc(sb, group, &group_desc_bh);
	if (!gdp)
		goto out;

	/*
	 * We do not need to lock this, because we are the only one
	 * handling this flag.
	 */
	if (gdp->bg_flags & cpu_to_le16(EXT4_BG_INODE_ZEROED))
		goto out;

<<<<<<< HEAD
	handle = ext4_journal_start_sb(sb, 1);
=======
	handle = ext4_journal_start_sb(sb, EXT4_HT_MISC, 1);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(handle)) {
		ret = PTR_ERR(handle);
		goto out;
	}

	down_write(&grp->alloc_sem);
	/*
	 * If inode bitmap was already initialized there may be some
	 * used inodes so we need to skip blocks with used inodes in
	 * inode table.
	 */
	if (!(gdp->bg_flags & cpu_to_le16(EXT4_BG_INODE_UNINIT)))
		used_blks = DIV_ROUND_UP((EXT4_INODES_PER_GROUP(sb) -
			    ext4_itable_unused_count(sb, gdp)),
			    sbi->s_inodes_per_block);

	if ((used_blks < 0) || (used_blks > sbi->s_itb_per_group)) {
<<<<<<< HEAD
<<<<<<< HEAD
		ext4_error(sb, "Something is wrong with group %u\n"
			   "Used itable blocks: %d"
			   "itable unused count: %u\n",
			   group, used_blks,
			   ext4_itable_unused_count(sb, gdp));
		ret = 1;
		goto out;
=======
=======
>>>>>>> refs/remotes/origin/master
		ext4_error(sb, "Something is wrong with group %u: "
			   "used itable blocks: %d; "
			   "itable unused count: %u",
			   group, used_blks,
			   ext4_itable_unused_count(sb, gdp));
		ret = 1;
		goto err_out;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	blk = ext4_inode_table(sb, gdp) + used_blks;
	num = sbi->s_itb_per_group - used_blks;

	BUFFER_TRACE(group_desc_bh, "get_write_access");
	ret = ext4_journal_get_write_access(handle,
					    group_desc_bh);
	if (ret)
		goto err_out;

	/*
	 * Skip zeroout if the inode table is full. But we set the ZEROED
	 * flag anyway, because obviously, when it is full it does not need
	 * further zeroing.
	 */
	if (unlikely(num == 0))
		goto skip_zeroout;

	ext4_debug("going to zero out inode table in group %d\n",
		   group);
	ret = sb_issue_zeroout(sb, blk, num, GFP_NOFS);
	if (ret < 0)
		goto err_out;
	if (barrier)
		blkdev_issue_flush(sb->s_bdev, GFP_NOFS, NULL);

skip_zeroout:
	ext4_lock_group(sb, group);
	gdp->bg_flags |= cpu_to_le16(EXT4_BG_INODE_ZEROED);
<<<<<<< HEAD
	gdp->bg_checksum = ext4_group_desc_csum(sbi, group, gdp);
=======
	ext4_group_desc_csum_set(sb, group, gdp);
>>>>>>> refs/remotes/origin/master
	ext4_unlock_group(sb, group);

	BUFFER_TRACE(group_desc_bh,
		     "call ext4_handle_dirty_metadata");
	ret = ext4_handle_dirty_metadata(handle, NULL,
					 group_desc_bh);

err_out:
	up_write(&grp->alloc_sem);
	ext4_journal_stop(handle);
out:
	return ret;
}
