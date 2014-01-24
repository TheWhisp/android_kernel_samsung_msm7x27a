/*
 * Copyright (c) 1996, 2003 VIA Networking Technologies, Inc.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * File: wctl.h
 *
 * Purpose:
 *
 * Author: Jerry Chen
 *
 * Date: Jun. 27, 2002
 *
 */

#ifndef __WCTL_H__
#define __WCTL_H__

<<<<<<< HEAD
#include "ttype.h"
#include "tether.h"
#include "device.h"

/*---------------------  Export Definitions -------------------------*/

#define IS_TYPE_DATA(pMACHeader)                                                        \
    ((((PS802_11Header) pMACHeader)->wFrameCtl & TYPE_802_11_MASK) == TYPE_802_11_DATA)

#define IS_TYPE_MGMT(pMACHeader)                                                        \
    ((((PS802_11Header) pMACHeader)->wFrameCtl & TYPE_802_11_MASK) == TYPE_802_11_MGMT)

#define IS_TYPE_CONTROL(pMACHeader)                                                     \
    ((((PS802_11Header) pMACHeader)->wFrameCtl & TYPE_802_11_MASK) == TYPE_802_11_CTL)

#define IS_FC_MOREDATA(pMACHeader)                                                      \
    ((((PS802_11Header) pMACHeader)->wFrameCtl & FC_MOREDATA) == FC_MOREDATA)

#define IS_FC_POWERMGT(pMACHeader)                                                      \
    ((((PS802_11Header) pMACHeader)->wFrameCtl & FC_POWERMGT) == FC_POWERMGT)

#define IS_FC_RETRY(pMACHeader)                                                         \
    ((((PS802_11Header) pMACHeader)->wFrameCtl & FC_RETRY) == FC_RETRY)

#define IS_FC_WEP(pMACHeader)                                                           \
    ((((PS802_11Header) pMACHeader)->wFrameCtl & FC_WEP) == FC_WEP)
=======
#include "tether.h"
#include "device.h"

#define IS_TYPE_DATA(pMACHeader)                                                        \
    ((((struct ieee80211_hdr *) pMACHeader)->frame_control & TYPE_802_11_MASK) == TYPE_802_11_DATA)

#define IS_TYPE_MGMT(pMACHeader)                                                        \
    ((((struct ieee80211_hdr *) pMACHeader)->frame_control & TYPE_802_11_MASK) == TYPE_802_11_MGMT)

#define IS_TYPE_CONTROL(pMACHeader)                                                     \
    ((((struct ieee80211_hdr *) pMACHeader)->frame_control & TYPE_802_11_MASK) == TYPE_802_11_CTL)

#define IS_FC_MOREDATA(pMACHeader)                                                      \
    ((((struct ieee80211_hdr *) pMACHeader)->frame_control & FC_MOREDATA) == FC_MOREDATA)

#define IS_FC_POWERMGT(pMACHeader)                                                      \
    ((((struct ieee80211_hdr *) pMACHeader)->frame_control & FC_POWERMGT) == FC_POWERMGT)

#define IS_FC_RETRY(pMACHeader)                                                         \
    ((((struct ieee80211_hdr *) pMACHeader)->frame_control & FC_RETRY) == FC_RETRY)

#define IS_FC_WEP(pMACHeader)                                                           \
    ((((struct ieee80211_hdr *) pMACHeader)->frame_control & FC_WEP) == FC_WEP)
>>>>>>> refs/remotes/origin/master

#ifdef __BIG_ENDIAN

#define IS_FRAGMENT_PKT(pMACHeader)                                                     \
<<<<<<< HEAD
    (((((PS802_11Header) pMACHeader)->wFrameCtl & FC_MOREFRAG) != 0) |                  \
     ((((PS802_11Header) pMACHeader)->wSeqCtl & 0x0F00) != 0))

#define IS_FIRST_FRAGMENT_PKT(pMACHeader)                                               \
    ((((PS802_11Header) pMACHeader)->wSeqCtl & 0x0F00) == 0)
=======
    (((((struct ieee80211_hdr *) pMACHeader)->frame_control & FC_MOREFRAG) != 0) |                  \
     ((((struct ieee80211_hdr *) pMACHeader)->seq_ctrl & 0x0F00) != 0))

#define IS_FIRST_FRAGMENT_PKT(pMACHeader)                                               \
    ((((struct ieee80211_hdr *) pMACHeader)->seq_ctrl & 0x0F00) == 0)
>>>>>>> refs/remotes/origin/master

#else

#define IS_FRAGMENT_PKT(pMACHeader)                                                     \
<<<<<<< HEAD
    (((((PS802_11Header) pMACHeader)->wFrameCtl & FC_MOREFRAG) != 0) |                  \
     ((((PS802_11Header) pMACHeader)->wSeqCtl & 0x000F) != 0))

#define IS_FIRST_FRAGMENT_PKT(pMACHeader)                                               \
    ((((PS802_11Header) pMACHeader)->wSeqCtl & 0x000F) == 0)
=======
    (((((struct ieee80211_hdr *) pMACHeader)->frame_control & FC_MOREFRAG) != 0) |                  \
     ((((struct ieee80211_hdr *) pMACHeader)->seq_ctrl & 0x000F) != 0))

#define IS_FIRST_FRAGMENT_PKT(pMACHeader)                                               \
    ((((struct ieee80211_hdr *) pMACHeader)->seq_ctrl & 0x000F) == 0)
>>>>>>> refs/remotes/origin/master

#endif//#ifdef __BIG_ENDIAN

#define IS_LAST_FRAGMENT_PKT(pMACHeader)                                                \
<<<<<<< HEAD
    ((((PS802_11Header) pMACHeader)->wFrameCtl & FC_MOREFRAG) == 0)

#define IS_CTL_PSPOLL(pMACHeader)                                                       \
    ((((PS802_11Header) pMACHeader)->wFrameCtl & TYPE_SUBTYPE_MASK) == TYPE_CTL_PSPOLL)

=======
    ((((struct ieee80211_hdr *) pMACHeader)->frame_control & FC_MOREFRAG) == 0)

#define IS_CTL_PSPOLL(pMACHeader)                                                       \
    ((((struct ieee80211_hdr *) pMACHeader)->frame_control & TYPE_SUBTYPE_MASK) == TYPE_CTL_PSPOLL)
>>>>>>> refs/remotes/origin/master

#define ADD_ONE_WITH_WRAP_AROUND(uVar, uModulo) {   \
    if ((uVar) >= ((uModulo) - 1))                  \
        (uVar) = 0;                                 \
    else                                            \
        (uVar)++;                                   \
}

<<<<<<< HEAD
/*---------------------  Export Classes  ----------------------------*/

/*---------------------  Export Variables  --------------------------*/

/*---------------------  Export Functions  --------------------------*/

BOOL WCTLbIsDuplicate(PSCache pCache, PS802_11Header pMACHeader);
BOOL WCTLbHandleFragment(PSDevice pDevice, PS802_11Header pMACHeader,
			 unsigned int cbFrameLength, BOOL bWEP, BOOL bExtIV);
unsigned int WCTLuSearchDFCB(PSDevice pDevice, PS802_11Header pMACHeader);
unsigned int WCTLuInsertDFCB(PSDevice pDevice, PS802_11Header pMACHeader);
=======
bool WCTLbIsDuplicate(PSCache pCache, struct ieee80211_hdr *pMACHeader);
bool WCTLbHandleFragment(struct vnt_private *, struct ieee80211_hdr *pMACHeader, unsigned int cbFrameLength, bool bWEP, bool bExtIV);
unsigned int WCTLuSearchDFCB(struct vnt_private *, struct ieee80211_hdr *pMACHeader);
unsigned int WCTLuInsertDFCB(struct vnt_private *, struct ieee80211_hdr *pMACHeader);
>>>>>>> refs/remotes/origin/master

#endif /* __WCTL_H__ */
