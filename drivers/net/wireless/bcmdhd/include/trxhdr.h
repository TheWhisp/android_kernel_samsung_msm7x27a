/*
 * TRX image file header format.
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (C) 1999-2011, Broadcom Corporation
 * 
 *         Unless you and Broadcom execute a separate written software license
=======
 * Copyright (C) 1999-2012, Broadcom Corporation
 * 
 *      Unless you and Broadcom execute a separate written software license
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright (C) 1999-2012, Broadcom Corporation
 * 
 *      Unless you and Broadcom execute a separate written software license
>>>>>>> refs/remotes/origin/cm-11.0
 * agreement governing use of this software, this software is licensed to you
 * under the terms of the GNU General Public License version 2 (the "GPL"),
 * available at http://www.broadcom.com/licenses/GPLv2.php, with the
 * following added to such license:
 * 
 *      As a special exception, the copyright holders of this software give you
 * permission to link this software with independent modules, and to copy and
 * distribute the resulting executable under terms of your choice, provided that
 * you also meet, for each linked independent module, the terms and conditions of
 * the license of that module.  An independent module is a module which is not
 * derived from this software.  The special exception does not apply to any
 * modifications of the software.
 * 
 *      Notwithstanding the above, under no circumstances may you combine this
 * software in any way with any other Broadcom software provided under a license
 * other than the GPL, without Broadcom's express prior written consent.
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * $Id: trxhdr.h 286295 2011-09-27 06:39:43Z $
 */

#ifndef	_TRX_HDR_H_
#define	_TRX_HDR_H_
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
 * $Id: trxhdr.h 260898 2011-05-20 23:11:12Z $
 */

#ifndef _TRX_HDR_H
#define _TRX_HDR_H
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0

#include <typedefs.h>

#define TRX_MAGIC	0x30524448	/* "HDR0" */
#define TRX_VERSION	1		/* Version 1 */
#define TRX_MAX_LEN	0x3B0000	/* Max length */
#define TRX_NO_HEADER	1		/* Do not write TRX header */
#define TRX_GZ_FILES	0x2     /* Contains up to TRX_MAX_OFFSET individual gzip files */
<<<<<<< HEAD
<<<<<<< HEAD
#define TRX_OVERLAYS	0x4     /* Contains an overlay header after the trx header */
#define TRX_MAX_OFFSET	3		/* Max number of individual files */
#define TRX_UNCOMP_IMAGE	0x20	/* Trx contains uncompressed rtecdc.bin image */
#define TRX_ROMSIM_IMAGE	0x10	/* Trx contains ROM simulation image */
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
#define TRX_EMBED_UCODE	0x8	/* Trx contains embedded ucode image */
#define TRX_ROMSIM_IMAGE	0x10	/* Trx contains ROM simulation image */
#define TRX_UNCOMP_IMAGE	0x20	/* Trx contains uncompressed rtecdc.bin image */
#define TRX_MAX_OFFSET	3		/* Max number of individual files */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0

struct trx_header {
	uint32 magic;		/* "HDR0" */
	uint32 len;		/* Length of file including header */
	uint32 crc32;		/* 32-bit CRC from flag_version to end of file */
	uint32 flag_version;	/* 0:15 flags, 16:31 version */
	uint32 offsets[TRX_MAX_OFFSET];	/* Offsets of partitions from start of header */
};

/* Compatibility */
typedef struct trx_header TRXHDR, *PTRXHDR;

<<<<<<< HEAD
<<<<<<< HEAD
#endif /* _TRX_HDR_H_ */
=======
#endif /* _TRX_HDR_H */
>>>>>>> refs/remotes/origin/cm-10.0
=======
#endif /* _TRX_HDR_H */
>>>>>>> refs/remotes/origin/cm-11.0
