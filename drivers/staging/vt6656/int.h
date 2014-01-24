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
 *
 * File: int.h
 *
 * Purpose:
 *
 * Author: Jerry Chen
 *
 * Date: Apr. 2, 2004
 *
 */

#ifndef __INT_H__
#define __INT_H__

<<<<<<< HEAD
#include "ttype.h"
#include "device.h"

/*---------------------  Export Definitions -------------------------*/
typedef struct tagSINTData {
    BYTE    byTSR0;
    BYTE    byPkt0;
    WORD    wTime0;
    BYTE    byTSR1;
    BYTE    byPkt1;
    WORD    wTime1;
    BYTE    byTSR2;
    BYTE    byPkt2;
    WORD    wTime2;
    BYTE    byTSR3;
    BYTE    byPkt3;
    WORD    wTime3;
    DWORD   dwLoTSF;
    DWORD   dwHiTSF;
    BYTE    byISR0;
    BYTE    byISR1;
    BYTE    byRTSSuccess;
    BYTE    byRTSFail;
    BYTE    byACKFail;
    BYTE    byFCSErr;
    BYTE    abySW[2];
} __attribute__ ((__packed__))
SINTData, *PSINTData;


/*---------------------  Export Classes  ----------------------------*/

/*---------------------  Export Variables  --------------------------*/

/*---------------------  Export Functions  --------------------------*/

void INTvWorkItem(void *Context);
<<<<<<< HEAD
int INTnsProcessData(PSDevice pDevice);
=======
void INTnsProcessData(PSDevice pDevice);
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "device.h"

typedef struct tagSINTData {
	u8 byTSR0;
	u8 byPkt0;
	u16 wTime0;
	u8 byTSR1;
	u8 byPkt1;
	u16 wTime1;
	u8 byTSR2;
	u8 byPkt2;
	u16 wTime2;
	u8 byTSR3;
	u8 byPkt3;
	u16 wTime3;
	u64 qwTSF;
	u8 byISR0;
	u8 byISR1;
	u8 byRTSSuccess;
	u8 byRTSFail;
	u8 byACKFail;
	u8 byFCSErr;
	u8 abySW[2];
} __attribute__ ((__packed__))
SINTData, *PSINTData;

void INTvWorkItem(struct vnt_private *);
void INTnsProcessData(struct vnt_private *);
>>>>>>> refs/remotes/origin/master

#endif /* __INT_H__ */
