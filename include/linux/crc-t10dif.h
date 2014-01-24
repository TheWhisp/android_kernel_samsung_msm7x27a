#ifndef _LINUX_CRC_T10DIF_H
#define _LINUX_CRC_T10DIF_H

#include <linux/types.h>

<<<<<<< HEAD
=======
#define CRC_T10DIF_DIGEST_SIZE 2
#define CRC_T10DIF_BLOCK_SIZE 1

__u16 crc_t10dif_generic(__u16 crc, const unsigned char *buffer, size_t len);
>>>>>>> refs/remotes/origin/master
__u16 crc_t10dif(unsigned char const *, size_t);

#endif
