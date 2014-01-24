/*
 * QNX4 file system, Linux implementation.
 *
 * Version : 0.2.1
 *
 * Using parts of the xiafs filesystem.
 *
 * History :
 *
 * 28-05-1998 by Richard Frowijn : first release.
 * 20-06-1998 by Frank Denis : basic optimisations.
 * 25-06-1998 by Frank Denis : qnx4_is_free, qnx4_set_bitmap, qnx4_bmap .
 * 28-06-1998 by Frank Denis : qnx4_free_inode (to be fixed) .
 */

#include <linux/buffer_head.h>
#include <linux/bitops.h>
#include "qnx4.h"

<<<<<<< HEAD
static void count_bits(register const char *bmPart, register int size,
		       int *const tf)
{
	char b;
	int tot = *tf;

	if (size > QNX4_BLOCK_SIZE) {
		size = QNX4_BLOCK_SIZE;
	}
	do {
		b = *bmPart++;
		tot += 8 - hweight8(b);
		size--;
	} while (size != 0);
	*tf = tot;
}

=======
>>>>>>> refs/remotes/origin/master
unsigned long qnx4_count_free_blocks(struct super_block *sb)
{
	int start = le32_to_cpu(qnx4_sb(sb)->BitMap->di_first_xtnt.xtnt_blk) - 1;
	int total = 0;
	int total_free = 0;
	int offset = 0;
	int size = le32_to_cpu(qnx4_sb(sb)->BitMap->di_size);
	struct buffer_head *bh;

	while (total < size) {
<<<<<<< HEAD
=======
		int bytes = min(size - total, QNX4_BLOCK_SIZE);

>>>>>>> refs/remotes/origin/master
		if ((bh = sb_bread(sb, start + offset)) == NULL) {
			printk(KERN_ERR "qnx4: I/O error in counting free blocks\n");
			break;
		}
<<<<<<< HEAD
		count_bits(bh->b_data, size - total, &total_free);
		brelse(bh);
		total += QNX4_BLOCK_SIZE;
=======
		total_free += bytes * BITS_PER_BYTE -
				memweight(bh->b_data, bytes);
		brelse(bh);
		total += bytes;
>>>>>>> refs/remotes/origin/master
		offset++;
	}

	return total_free;
}
