#ifndef __IP_SET_BITMAP_H
#define __IP_SET_BITMAP_H

<<<<<<< HEAD
/* Bitmap type specific error codes */
enum {
	/* The element is out of the range of the set */
	IPSET_ERR_BITMAP_RANGE = IPSET_ERR_TYPE_SPECIFIC,
	/* The range exceeds the size limit of the set type */
	IPSET_ERR_BITMAP_RANGE_SIZE,
};

#ifdef __KERNEL__
#define IPSET_BITMAP_MAX_RANGE	0x0000FFFF

=======
#include <uapi/linux/netfilter/ipset/ip_set_bitmap.h>

#define IPSET_BITMAP_MAX_RANGE	0x0000FFFF

enum {
	IPSET_ADD_FAILED = 1,
	IPSET_ADD_STORE_PLAIN_TIMEOUT,
	IPSET_ADD_START_STORED_TIMEOUT,
};

>>>>>>> refs/remotes/origin/master
/* Common functions */

static inline u32
range_to_mask(u32 from, u32 to, u8 *bits)
{
	u32 mask = 0xFFFFFFFE;

	*bits = 32;
	while (--(*bits) > 0 && mask && (to & mask) != from)
		mask <<= 1;

	return mask;
}

<<<<<<< HEAD
#endif /* __KERNEL__ */

=======
>>>>>>> refs/remotes/origin/master
#endif /* __IP_SET_BITMAP_H */
