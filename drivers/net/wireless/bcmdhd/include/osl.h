/*
 * OS Abstraction Layer
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
 * $Id: osl.h 277737 2011-08-16 17:54:59Z $
 */


=======
 * $Id: osl.h 320905 2012-03-13 15:33:25Z $
 */

>>>>>>> refs/remotes/origin/cm-10.0
=======
 * $Id: osl.h 320905 2012-03-13 15:33:25Z $
 */

>>>>>>> refs/remotes/origin/cm-11.0
#ifndef _osl_h_
#define _osl_h_


typedef struct osl_info osl_t;
typedef struct osl_dmainfo osldma_t;

#define OSL_PKTTAG_SZ	32 


typedef void (*pktfree_cb_fn_t)(void *ctx, void *pkt, unsigned int status);


<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
typedef unsigned int (*osl_rreg_fn_t)(void *ctx, volatile void *reg, unsigned int size);
typedef void  (*osl_wreg_fn_t)(void *ctx, volatile void *reg, unsigned int val, unsigned int size);


<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
#include <linux_osl.h>

#ifndef PKTDBG_TRACE
#define PKTDBG_TRACE(osh, pkt, bit)
#endif

<<<<<<< HEAD
<<<<<<< HEAD
=======
#define PKTCTFMAP(osh, p)

>>>>>>> refs/remotes/origin/cm-10.0
=======
#define PKTCTFMAP(osh, p)

>>>>>>> refs/remotes/origin/cm-11.0


#define	SET_REG(osh, r, mask, val)	W_REG((osh), (r), ((R_REG((osh), r) & ~(mask)) | (val)))

#ifndef AND_REG
#define AND_REG(osh, r, v)		W_REG(osh, (r), R_REG(osh, r) & (v))
#endif   

#ifndef OR_REG
#define OR_REG(osh, r, v)		W_REG(osh, (r), R_REG(osh, r) | (v))
#endif   

#if !defined(OSL_SYSUPTIME)
#define OSL_SYSUPTIME() (0)
#define OSL_SYSUPTIME_SUPPORT FALSE
#else
#define OSL_SYSUPTIME_SUPPORT TRUE
#endif 

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
#if !defined(PKTC)
#define	PKTCCNT(skb)		(0)
#define	PKTCLEN(skb)		(0)
#define	PKTCFLAGS(skb)		(0)
#define	PKTCSETCNT(skb, c)
#define	PKTCSETLEN(skb, l)
#define	PKTCSETFLAG(skb, fb)
#define	PKTCCLRFLAG(skb, fb)
#define	PKTCLINK(skb)		PKTLINK(skb)
#define	PKTSETCLINK(skb, x)	PKTSETLINK((skb), (x))
#define	PKTISCHAINED(skb)	FALSE
#define FOREACH_CHAINED_PKT(skb, nskb) \
	for ((nskb) = NULL; (skb) != NULL; (skb) = (nskb))
#define	PKTCFREE		PKTFREE
#endif


<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
#endif	
