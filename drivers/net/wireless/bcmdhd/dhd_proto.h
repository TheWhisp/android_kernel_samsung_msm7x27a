/*
 * Header file describing the internal (inter-module) DHD interfaces.
 *
 * Provides type definitions and function prototypes used to link the
 * DHD OS, bus, and protocol modules.
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
 * $Id: dhd_proto.h,v 1.8.10.6 2010-12-22 23:47:24 Exp $
=======
 * $Id: dhd_proto.h 303834 2011-12-20 06:17:39Z $
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * $Id: dhd_proto.h 303834 2011-12-20 06:17:39Z $
>>>>>>> refs/remotes/origin/cm-11.0
 */

#ifndef _dhd_proto_h_
#define _dhd_proto_h_

#include <dhdioctl.h>
#include <wlioctl.h>

#ifndef IOCTL_RESP_TIMEOUT
<<<<<<< HEAD
<<<<<<< HEAD
#define IOCTL_RESP_TIMEOUT  2000 /* In milli second */
#endif
=======
#define IOCTL_RESP_TIMEOUT  20000 /* In milli second */
#endif /* IOCTL_RESP_TIMEOUT */
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define IOCTL_RESP_TIMEOUT  20000 /* In milli second */
#endif /* IOCTL_RESP_TIMEOUT */
>>>>>>> refs/remotes/origin/cm-11.0

/*
 * Exported from the dhd protocol module (dhd_cdc, dhd_rndis)
 */

/* Linkage, sets prot link and updates hdrlen in pub */
extern int dhd_prot_attach(dhd_pub_t *dhdp);

/* Unlink, frees allocated protocol memory (including dhd_prot) */
extern void dhd_prot_detach(dhd_pub_t *dhdp);

/* Initialize protocol: sync w/dongle state.
 * Sets dongle media info (iswl, drv_version, mac address).
 */
extern int dhd_prot_init(dhd_pub_t *dhdp);

/* Stop protocol: sync w/dongle state. */
extern void dhd_prot_stop(dhd_pub_t *dhdp);

/* Add any protocol-specific data header.
 * Caller must reserve prot_hdrlen prepend space.
 */
extern void dhd_prot_hdrpush(dhd_pub_t *, int ifidx, void *txp);

/* Remove any protocol-specific data header. */
<<<<<<< HEAD
<<<<<<< HEAD
extern int dhd_prot_hdrpull(dhd_pub_t *, int *ifidx, void *rxp);
=======
extern int dhd_prot_hdrpull(dhd_pub_t *, int *ifidx, void *rxp, uchar *buf, uint *len);
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern int dhd_prot_hdrpull(dhd_pub_t *, int *ifidx, void *rxp, uchar *buf, uint *len);
>>>>>>> refs/remotes/origin/cm-11.0

/* Use protocol to issue ioctl to dongle */
extern int dhd_prot_ioctl(dhd_pub_t *dhd, int ifidx, wl_ioctl_t * ioc, void * buf, int len);

/* Handles a protocol control response asynchronously */
extern int dhd_prot_ctl_complete(dhd_pub_t *dhd);

/* Check for and handle local prot-specific iovar commands */
extern int dhd_prot_iovar_op(dhd_pub_t *dhdp, const char *name,
                             void *params, int plen, void *arg, int len, bool set);

/* Add prot dump output to a buffer */
extern void dhd_prot_dump(dhd_pub_t *dhdp, struct bcmstrbuf *strbuf);

/* Update local copy of dongle statistics */
extern void dhd_prot_dstats(dhd_pub_t *dhdp);

extern int dhd_ioctl(dhd_pub_t * dhd_pub, dhd_ioctl_t *ioc, void * buf, uint buflen);

extern int dhd_preinit_ioctls(dhd_pub_t *dhd);

#ifdef PROP_TXSTATUS
extern int dhd_wlfc_enque_sendq(void* state, int prec, void* p);
extern int dhd_wlfc_commit_packets(void* state, f_commitpkt_t fcommit, void* commit_ctx);
extern void dhd_wlfc_cleanup(dhd_pub_t *dhd);
#endif /* PROP_TXSTATUS */

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
extern int dhd_process_pkt_reorder_info(dhd_pub_t *dhd, uchar *reorder_info_buf,
	uint reorder_info_len, void **pkt, uint32 *free_buf_count);


<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
/********************************
 * For version-string expansion *
 */
#if defined(BDC)
#define DHD_PROTOCOL "bdc"
#elif defined(CDC)
#define DHD_PROTOCOL "cdc"
#elif defined(RNDIS)
#define DHD_PROTOCOL "rndis"
#else
#define DHD_PROTOCOL "unknown"
#endif /* proto */

#endif /* _dhd_proto_h_ */
