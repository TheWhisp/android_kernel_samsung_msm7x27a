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
 * File: mac.c
 *
 * Purpose:  MAC routines
 *
 * Author: Tevin Chen
 *
 * Date: May 21, 1996
 *
 * Functions:
 *
 * Revision History:
 */

#include "tmacro.h"
#include "tether.h"
#include "desc.h"
#include "mac.h"
#include "80211hdr.h"
#include "rndis.h"
#include "control.h"

<<<<<<< HEAD
/*---------------------  Static Definitions -------------------------*/
//static int          msglevel                =MSG_LEVEL_DEBUG;
static int          msglevel                =MSG_LEVEL_INFO;
/*---------------------  Static Classes  ----------------------------*/

/*---------------------  Static Variables  --------------------------*/

/*---------------------  Static Functions  --------------------------*/

/*---------------------  Export Variables  --------------------------*/

/*---------------------  Export Functions  --------------------------*/





/*
 * Description:
 *      Set this hash index into multicast address register bit
 *
 * Parameters:
 *  In:
 *      byHashIdx   - Hash index to set
 *  Out:
 *      none
 *
 * Return Value: none
 *
 */
void MACvSetMultiAddrByHash (PSDevice pDevice, BYTE byHashIdx)
{
    unsigned int            uByteIdx;
    BYTE            byBitMask;
    BYTE            pbyData[2];


    // calculate byte position
    uByteIdx = byHashIdx / 8;

    // calculate bit position
    byBitMask = 1;
    byBitMask <<= (byHashIdx % 8);
    // turn on the bit

    pbyData[0] = byBitMask;
    pbyData[1] = byBitMask;

    CONTROLnsRequestOut(pDevice,
                        MESSAGE_TYPE_WRITE_MASK,
                        (WORD) (MAC_REG_MAR0 + uByteIdx),
                        MESSAGE_REQUEST_MACREG,
                        2,
                        pbyData);
}


=======
//static int          msglevel                =MSG_LEVEL_DEBUG;
static int          msglevel                =MSG_LEVEL_INFO;
>>>>>>> refs/remotes/origin/master

/*
 * Description:
 *      Write MAC Multicast Address Mask
 *
 * Parameters:
 *  In:
 *      uByteidx    - Index of Mask
 *      byData      - Mask Value to write
 *  Out:
 *      none
 *
 * Return Value: none
 *
 */
<<<<<<< HEAD
void MACvWriteMultiAddr(PSDevice pDevice, unsigned int uByteIdx, BYTE byData)
{
    BYTE            byData1;
=======
void MACvWriteMultiAddr(struct vnt_private *pDevice, u32 uByteIdx, u8 byData)
{
	u8 byData1;
>>>>>>> refs/remotes/origin/master

    byData1 = byData;
    CONTROLnsRequestOut(pDevice,
                        MESSAGE_TYPE_WRITE,
<<<<<<< HEAD
                        (WORD) (MAC_REG_MAR0 + uByteIdx),
=======
                        (u16) (MAC_REG_MAR0 + uByteIdx),
>>>>>>> refs/remotes/origin/master
                        MESSAGE_REQUEST_MACREG,
                        1,
                        &byData1);
}

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
/*
 * Description:
 *      Shut Down MAC
 *
 * Parameters:
 *  In:
 *  Out:
 *      none
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Return Value: TRUE if success; otherwise FALSE
 *
 */
BOOL MACbShutdown (PSDevice pDevice)
=======
 *
 */
void MACbShutdown(PSDevice pDevice)
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *
 */
void MACbShutdown(struct vnt_private *pDevice)
>>>>>>> refs/remotes/origin/master
{
    CONTROLnsRequestOutAsyn(pDevice,
                        MESSAGE_TYPE_MACSHUTDOWN,
                        0,
                        0,
                        0,
                        NULL
                        );
<<<<<<< HEAD
<<<<<<< HEAD
    return TRUE;
=======
>>>>>>> refs/remotes/origin/cm-10.0
}

void MACvSetBBType(PSDevice pDevice,BYTE byType)
{
BYTE            pbyData[2];

=======
}

void MACvSetBBType(struct vnt_private *pDevice, u8 byType)
{
	u8 pbyData[2];
>>>>>>> refs/remotes/origin/master

    pbyData[0] = byType;
    pbyData[1] = EnCFG_BBType_MASK;

    CONTROLnsRequestOut(pDevice,
                        MESSAGE_TYPE_WRITE_MASK,
                        MAC_REG_ENCFG0,
                        MESSAGE_REQUEST_MACREG,
<<<<<<< HEAD
                        2,
                        pbyData
                        );
}

void MACvSetMISCFifo (PSDevice pDevice, WORD wOffset, DWORD dwData)
{
BYTE    pbyData[4];

    if (wOffset > 273)
        return;
    pbyData[0] = (BYTE)dwData;
    pbyData[1] = (BYTE)(dwData>>8);
    pbyData[2] = (BYTE)(dwData>>16);
    pbyData[3] = (BYTE)(dwData>>24);

    CONTROLnsRequestOut(pDevice,
                        MESSAGE_TYPE_WRITE_MISCFF,
                        wOffset,
                        0,
                        4,
=======
			ARRAY_SIZE(pbyData),
>>>>>>> refs/remotes/origin/master
                        pbyData
                        );
}

/*
 * Description:
 *      Disable the Key Entry by MISCFIFO
 *
 * Parameters:
 *  In:
 *      dwIoBase        - Base Address for MAC
 *
 *  Out:
 *      none
 *
 * Return Value: none
 *
 */
<<<<<<< HEAD
void MACvDisableKeyEntry(PSDevice pDevice, unsigned int uEntryIdx)
{
WORD    wOffset;
BYTE            byData;


    byData = (BYTE) uEntryIdx;

    wOffset = MISCFIFO_KEYETRY0;
    wOffset += (uEntryIdx * MISCFIFO_KEYENTRYSIZE);

    //VNSvOutPortW(dwIoBase + MAC_REG_MISCFFNDEX, wOffset);
    //VNSvOutPortD(dwIoBase + MAC_REG_MISCFFDATA, 0);
    //VNSvOutPortW(dwIoBase + MAC_REG_MISCFFCTL, MISCFFCTL_WRITE);
=======
void MACvDisableKeyEntry(struct vnt_private *pDevice, u32 uEntryIdx)
{
	u8 byData;

    byData = (u8) uEntryIdx;
>>>>>>> refs/remotes/origin/master

    //issue write misc fifo command to device
    CONTROLnsRequestOut(pDevice,
                        MESSAGE_TYPE_CLRKEYENTRY,
                        0,
                        0,
                        1,
                        &byData
                        );
}

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
/*
 * Description:
 *      Set the Key by MISCFIFO
 *
 * Parameters:
 *  In:
 *      dwIoBase        - Base Address for MAC
 *
 *  Out:
 *      none
 *
 * Return Value: none
 *
 */
<<<<<<< HEAD
void MACvSetKeyEntry(PSDevice pDevice, WORD wKeyCtl,
		     unsigned int uEntryIdx, unsigned int uKeyIdx,
		     PBYTE pbyAddr, PDWORD pdwKey)
{
PBYTE           pbyKey;
WORD            wOffset;
DWORD           dwData1,dwData2;
int             ii;
BYTE            pbyData[24];

    if ( pDevice->byLocalID <= MAC_REVISION_A1 ) {
        if ( pDevice->sMgmtObj.byCSSPK == KEY_CTL_CCMP )
            return;
    }
=======
void MACvSetKeyEntry(struct vnt_private *pDevice, u16 wKeyCtl, u32 uEntryIdx,
	u32 uKeyIdx, u8 *pbyAddr, u32 *pdwKey)
{
	u8 *pbyKey;
	u16 wOffset;
	u32 dwData1, dwData2;
	int ii;
	u8 pbyData[24];

	if (pDevice->byLocalID <= MAC_REVISION_A1)
		if (pDevice->vnt_mgmt.byCSSPK == KEY_CTL_CCMP)
			return;
>>>>>>> refs/remotes/origin/master

    wOffset = MISCFIFO_KEYETRY0;
    wOffset += (uEntryIdx * MISCFIFO_KEYENTRYSIZE);

    dwData1 = 0;
    dwData1 |= wKeyCtl;
    dwData1 <<= 16;
    dwData1 |= MAKEWORD(*(pbyAddr+4), *(pbyAddr+5));

	DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"1. wOffset: %d, Data: %X,"\
		" KeyCtl:%X\n", wOffset, dwData1, wKeyCtl);

<<<<<<< HEAD
    //VNSvOutPortW(dwIoBase + MAC_REG_MISCFFNDEX, wOffset);
    //VNSvOutPortD(dwIoBase + MAC_REG_MISCFFDATA, dwData);
    //VNSvOutPortW(dwIoBase + MAC_REG_MISCFFCTL, MISCFFCTL_WRITE);

    //wOffset++;

=======
>>>>>>> refs/remotes/origin/master
    dwData2 = 0;
    dwData2 |= *(pbyAddr+3);
    dwData2 <<= 8;
    dwData2 |= *(pbyAddr+2);
    dwData2 <<= 8;
    dwData2 |= *(pbyAddr+1);
    dwData2 <<= 8;
    dwData2 |= *(pbyAddr+0);

	DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"2. wOffset: %d, Data: %X\n",
		wOffset, dwData2);

<<<<<<< HEAD
    //VNSvOutPortW(dwIoBase + MAC_REG_MISCFFNDEX, wOffset);
    //VNSvOutPortD(dwIoBase + MAC_REG_MISCFFDATA, dwData);
    //VNSvOutPortW(dwIoBase + MAC_REG_MISCFFCTL, MISCFFCTL_WRITE);

    //wOffset++;

    //wOffset += (uKeyIdx * 4);
/*    for (ii=0;ii<4;ii++) {
        // alway push 128 bits
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"3.(%d) wOffset: %d, Data: %lX\n", ii, wOffset+ii, *pdwKey);
        VNSvOutPortW(dwIoBase + MAC_REG_MISCFFNDEX, wOffset+ii);
        VNSvOutPortD(dwIoBase + MAC_REG_MISCFFDATA, *pdwKey++);
        VNSvOutPortW(dwIoBase + MAC_REG_MISCFFCTL, MISCFFCTL_WRITE);
    }
*/
    pbyKey = (PBYTE)pdwKey;

    pbyData[0] = (BYTE)dwData1;
    pbyData[1] = (BYTE)(dwData1>>8);
    pbyData[2] = (BYTE)(dwData1>>16);
    pbyData[3] = (BYTE)(dwData1>>24);
    pbyData[4] = (BYTE)dwData2;
    pbyData[5] = (BYTE)(dwData2>>8);
    pbyData[6] = (BYTE)(dwData2>>16);
    pbyData[7] = (BYTE)(dwData2>>24);
=======
    pbyKey = (u8 *)pdwKey;

    pbyData[0] = (u8)dwData1;
    pbyData[1] = (u8)(dwData1>>8);
    pbyData[2] = (u8)(dwData1>>16);
    pbyData[3] = (u8)(dwData1>>24);
    pbyData[4] = (u8)dwData2;
    pbyData[5] = (u8)(dwData2>>8);
    pbyData[6] = (u8)(dwData2>>16);
    pbyData[7] = (u8)(dwData2>>24);
>>>>>>> refs/remotes/origin/master
    for (ii = 8; ii < 24; ii++)
	pbyData[ii] = *pbyKey++;

    CONTROLnsRequestOut(pDevice,
                        MESSAGE_TYPE_SETKEY,
                        wOffset,
<<<<<<< HEAD
                        (WORD)uKeyIdx,
                        24,
                        pbyData
                        );


}


void MACvRegBitsOff(PSDevice pDevice, BYTE byRegOfs, BYTE byBits)
{
BYTE            pbyData[2];
=======
                        (u16)uKeyIdx,
			ARRAY_SIZE(pbyData),
                        pbyData
                        );

}

void MACvRegBitsOff(struct vnt_private *pDevice, u8 byRegOfs, u8 byBits)
{
	u8 pbyData[2];
>>>>>>> refs/remotes/origin/master

    pbyData[0] = 0;
    pbyData[1] = byBits;

    CONTROLnsRequestOut(pDevice,
                        MESSAGE_TYPE_WRITE_MASK,
                        byRegOfs,
                        MESSAGE_REQUEST_MACREG,
<<<<<<< HEAD
                        2,
=======
			ARRAY_SIZE(pbyData),
>>>>>>> refs/remotes/origin/master
                        pbyData
                        );
}

<<<<<<< HEAD

void MACvRegBitsOn(PSDevice pDevice, BYTE byRegOfs, BYTE byBits)
{
BYTE            pbyData[2];

=======
void MACvRegBitsOn(struct vnt_private *pDevice, u8 byRegOfs, u8 byBits)
{
	u8 pbyData[2];
>>>>>>> refs/remotes/origin/master

    pbyData[0] = byBits;
    pbyData[1] = byBits;

    CONTROLnsRequestOut(pDevice,
                        MESSAGE_TYPE_WRITE_MASK,
                        byRegOfs,
                        MESSAGE_REQUEST_MACREG,
<<<<<<< HEAD
                        2,
=======
			ARRAY_SIZE(pbyData),
>>>>>>> refs/remotes/origin/master
                        pbyData
                        );
}

<<<<<<< HEAD
void MACvWriteWord(PSDevice pDevice, BYTE byRegOfs, WORD wData)
{
BYTE            pbyData[2];


    pbyData[0] = (BYTE)(wData & 0xff);
    pbyData[1] = (BYTE)(wData >> 8);
=======
void MACvWriteWord(struct vnt_private *pDevice, u8 byRegOfs, u16 wData)
{
	u8 pbyData[2];

    pbyData[0] = (u8)(wData & 0xff);
    pbyData[1] = (u8)(wData >> 8);
>>>>>>> refs/remotes/origin/master

    CONTROLnsRequestOut(pDevice,
                        MESSAGE_TYPE_WRITE,
                        byRegOfs,
                        MESSAGE_REQUEST_MACREG,
<<<<<<< HEAD
                        2,
=======
			ARRAY_SIZE(pbyData),
>>>>>>> refs/remotes/origin/master
                        pbyData
                        );

}

<<<<<<< HEAD
void MACvWriteBSSIDAddress(PSDevice pDevice, PBYTE pbyEtherAddr)
{
BYTE            pbyData[6];


    pbyData[0] = *((PBYTE)pbyEtherAddr);
    pbyData[1] = *((PBYTE)pbyEtherAddr+1);
    pbyData[2] = *((PBYTE)pbyEtherAddr+2);
    pbyData[3] = *((PBYTE)pbyEtherAddr+3);
    pbyData[4] = *((PBYTE)pbyEtherAddr+4);
    pbyData[5] = *((PBYTE)pbyEtherAddr+5);
=======
void MACvWriteBSSIDAddress(struct vnt_private *pDevice, u8 *pbyEtherAddr)
{
	u8 pbyData[6];

    pbyData[0] = *((u8 *)pbyEtherAddr);
    pbyData[1] = *((u8 *)pbyEtherAddr+1);
    pbyData[2] = *((u8 *)pbyEtherAddr+2);
    pbyData[3] = *((u8 *)pbyEtherAddr+3);
    pbyData[4] = *((u8 *)pbyEtherAddr+4);
    pbyData[5] = *((u8 *)pbyEtherAddr+5);
>>>>>>> refs/remotes/origin/master

    CONTROLnsRequestOut(pDevice,
                        MESSAGE_TYPE_WRITE,
                        MAC_REG_BSSID0,
                        MESSAGE_REQUEST_MACREG,
<<<<<<< HEAD
                        6,
=======
			ARRAY_SIZE(pbyData),
>>>>>>> refs/remotes/origin/master
                        pbyData
                        );
}

<<<<<<< HEAD
void MACvEnableProtectMD(PSDevice pDevice)
{
BYTE            pbyData[2];

=======
void MACvEnableProtectMD(struct vnt_private *pDevice)
{
	u8 pbyData[2];
>>>>>>> refs/remotes/origin/master

    pbyData[0] = EnCFG_ProtectMd;
    pbyData[1] = EnCFG_ProtectMd;

    CONTROLnsRequestOut(pDevice,
                        MESSAGE_TYPE_WRITE_MASK,
                        MAC_REG_ENCFG0,
                        MESSAGE_REQUEST_MACREG,
<<<<<<< HEAD
                        2,
=======
			ARRAY_SIZE(pbyData),
>>>>>>> refs/remotes/origin/master
                        pbyData
                        );
}

<<<<<<< HEAD
void MACvDisableProtectMD(PSDevice pDevice)
{
BYTE            pbyData[2];

=======
void MACvDisableProtectMD(struct vnt_private *pDevice)
{
	u8 pbyData[2];
>>>>>>> refs/remotes/origin/master

    pbyData[0] = 0;
    pbyData[1] = EnCFG_ProtectMd;

    CONTROLnsRequestOut(pDevice,
                        MESSAGE_TYPE_WRITE_MASK,
                        MAC_REG_ENCFG0,
                        MESSAGE_REQUEST_MACREG,
<<<<<<< HEAD
                        2,
=======
			ARRAY_SIZE(pbyData),
>>>>>>> refs/remotes/origin/master
                        pbyData
                        );
}

<<<<<<< HEAD
void MACvEnableBarkerPreambleMd(PSDevice pDevice)
{
BYTE            pbyData[2];

=======
void MACvEnableBarkerPreambleMd(struct vnt_private *pDevice)
{
	u8 pbyData[2];
>>>>>>> refs/remotes/origin/master

    pbyData[0] = EnCFG_BarkerPream;
    pbyData[1] = EnCFG_BarkerPream;

    CONTROLnsRequestOut(pDevice,
                        MESSAGE_TYPE_WRITE_MASK,
                        MAC_REG_ENCFG2,
                        MESSAGE_REQUEST_MACREG,
<<<<<<< HEAD
                        2,
=======
			ARRAY_SIZE(pbyData),
>>>>>>> refs/remotes/origin/master
                        pbyData
                        );
}

<<<<<<< HEAD
void MACvDisableBarkerPreambleMd(PSDevice pDevice)
{
BYTE            pbyData[2];

=======
void MACvDisableBarkerPreambleMd(struct vnt_private *pDevice)
{
	u8 pbyData[2];
>>>>>>> refs/remotes/origin/master

    pbyData[0] = 0;
    pbyData[1] = EnCFG_BarkerPream;

    CONTROLnsRequestOut(pDevice,
                        MESSAGE_TYPE_WRITE_MASK,
                        MAC_REG_ENCFG2,
                        MESSAGE_REQUEST_MACREG,
<<<<<<< HEAD
                        2,
=======
			ARRAY_SIZE(pbyData),
>>>>>>> refs/remotes/origin/master
                        pbyData
                        );
}

<<<<<<< HEAD

void MACvWriteBeaconInterval(PSDevice pDevice, WORD wInterval)
{
BYTE            pbyData[2];

    pbyData[0] = (BYTE) (wInterval & 0xff);
    pbyData[1] = (BYTE) (wInterval >> 8);
=======
void MACvWriteBeaconInterval(struct vnt_private *pDevice, u16 wInterval)
{
	u8 pbyData[2];

	pbyData[0] = (u8)(wInterval & 0xff);
	pbyData[1] = (u8)(wInterval >> 8);
>>>>>>> refs/remotes/origin/master

    CONTROLnsRequestOut(pDevice,
			MESSAGE_TYPE_WRITE,
			MAC_REG_BI,
			MESSAGE_REQUEST_MACREG,
<<<<<<< HEAD
			2,
=======
			ARRAY_SIZE(pbyData),
>>>>>>> refs/remotes/origin/master
			pbyData
			);
}
