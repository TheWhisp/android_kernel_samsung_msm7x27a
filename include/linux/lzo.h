#ifndef __LZO_H__
#define __LZO_H__
/*
 *  LZO Public Kernel Interface
 *  A mini subset of the LZO real-time data compression library
 *
<<<<<<< HEAD
 *  Copyright (C) 1996-2005 Markus F.X.J. Oberhumer <markus@oberhumer.com>
=======
 *  Copyright (C) 1996-2012 Markus F.X.J. Oberhumer <markus@oberhumer.com>
>>>>>>> refs/remotes/origin/master
 *
 *  The full LZO package can be found at:
 *  http://www.oberhumer.com/opensource/lzo/
 *
<<<<<<< HEAD
 *  Changed for kernel use by:
=======
 *  Changed for Linux kernel use by:
>>>>>>> refs/remotes/origin/master
 *  Nitin Gupta <nitingupta910@gmail.com>
 *  Richard Purdie <rpurdie@openedhand.com>
 */

<<<<<<< HEAD
#define LZO1X_MEM_COMPRESS	(16384 * sizeof(unsigned char *))
#define LZO1X_1_MEM_COMPRESS	LZO1X_MEM_COMPRESS

#define lzo1x_worst_compress(x) ((x) + ((x) / 16) + 64 + 3)

/* This requires 'workmem' of size LZO1X_1_MEM_COMPRESS */
int lzo1x_1_compress(const unsigned char *src, size_t src_len,
			unsigned char *dst, size_t *dst_len, void *wrkmem);

/* safe decompression with overrun testing */
int lzo1x_decompress_safe(const unsigned char *src, size_t src_len,
			unsigned char *dst, size_t *dst_len);
=======
#define LZO1X_1_MEM_COMPRESS	(8192 * sizeof(unsigned short))
#define LZO1X_MEM_COMPRESS	LZO1X_1_MEM_COMPRESS

#define lzo1x_worst_compress(x) ((x) + ((x) / 16) + 64 + 3)

/* This requires 'wrkmem' of size LZO1X_1_MEM_COMPRESS */
int lzo1x_1_compress(const unsigned char *src, size_t src_len,
		     unsigned char *dst, size_t *dst_len, void *wrkmem);

/* safe decompression with overrun testing */
int lzo1x_decompress_safe(const unsigned char *src, size_t src_len,
			  unsigned char *dst, size_t *dst_len);
>>>>>>> refs/remotes/origin/master

/*
 * Return values (< 0 = Error)
 */
#define LZO_E_OK			0
#define LZO_E_ERROR			(-1)
#define LZO_E_OUT_OF_MEMORY		(-2)
#define LZO_E_NOT_COMPRESSIBLE		(-3)
#define LZO_E_INPUT_OVERRUN		(-4)
#define LZO_E_OUTPUT_OVERRUN		(-5)
#define LZO_E_LOOKBEHIND_OVERRUN	(-6)
#define LZO_E_EOF_NOT_FOUND		(-7)
#define LZO_E_INPUT_NOT_CONSUMED	(-8)
#define LZO_E_NOT_YET_IMPLEMENTED	(-9)
<<<<<<< HEAD
=======
#define LZO_E_INVALID_ARGUMENT		(-10)
>>>>>>> refs/remotes/origin/master

#endif
