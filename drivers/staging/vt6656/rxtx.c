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
 * File: rxtx.c
 *
 * Purpose: handle WMAC/802.3/802.11 rx & tx functions
 *
 * Author: Lyndon Chen
 *
 * Date: May 20, 2003
 *
 * Functions:
 *      s_vGenerateTxParameter - Generate tx dma required parameter.
 *      s_vGenerateMACHeader - Translate 802.3 to 802.11 header
 *      csBeacon_xmit - beacon tx function
 *      csMgmt_xmit - management tx function
 *      s_uGetDataDuration - get tx data required duration
 *      s_uFillDataHead- fulfill tx data duration header
 *      s_uGetRTSCTSDuration- get rtx/cts required duration
 *      s_uGetRTSCTSRsvTime- get rts/cts reserved time
 *      s_uGetTxRsvTime- get frame reserved time
 *      s_vFillCTSHead- fulfill CTS ctl header
 *      s_vFillFragParameter- Set fragment ctl parameter.
 *      s_vFillRTSHead- fulfill RTS ctl header
 *      s_vFillTxKey- fulfill tx encrypt key
 *      s_vSWencryption- Software encrypt header
 *      vDMA0_tx_80211- tx 802.11 frame via dma0
 *      vGenerateFIFOHeader- Generate tx FIFO ctl header
 *
 * Revision History:
 *
 */

#include "device.h"
#include "rxtx.h"
#include "tether.h"
#include "card.h"
#include "bssdb.h"
#include "mac.h"
<<<<<<< HEAD
#include "baseband.h"
=======
>>>>>>> refs/remotes/origin/master
#include "michael.h"
#include "tkip.h"
#include "tcrc.h"
#include "wctl.h"
#include "hostap.h"
#include "rf.h"
#include "datarate.h"
#include "usbpipe.h"
#include "iocmd.h"

<<<<<<< HEAD
/*---------------------  Static Definitions -------------------------*/

/*---------------------  Static Classes  ----------------------------*/

/*---------------------  Static Variables  --------------------------*/
static int          msglevel                = MSG_LEVEL_INFO;

/*---------------------  Static Functions  --------------------------*/

/*---------------------  Static Definitions -------------------------*/
#define CRITICAL_PACKET_LEN      256    // if packet size < 256 -> in-direct send
                                        //    packet size >= 256 -> direct send

const WORD wTimeStampOff[2][MAX_RATE] = {
=======
static int          msglevel                = MSG_LEVEL_INFO;

static const u16 wTimeStampOff[2][MAX_RATE] = {
>>>>>>> refs/remotes/origin/master
        {384, 288, 226, 209, 54, 43, 37, 31, 28, 25, 24, 23}, // Long Preamble
        {384, 192, 130, 113, 54, 43, 37, 31, 28, 25, 24, 23}, // Short Preamble
    };

<<<<<<< HEAD
const WORD wFB_Opt0[2][5] = {
        {RATE_12M, RATE_18M, RATE_24M, RATE_36M, RATE_48M}, // fallback_rate0
        {RATE_12M, RATE_12M, RATE_18M, RATE_24M, RATE_36M}, // fallback_rate1
    };
const WORD wFB_Opt1[2][5] = {
=======
static const u16 wFB_Opt0[2][5] = {
        {RATE_12M, RATE_18M, RATE_24M, RATE_36M, RATE_48M}, // fallback_rate0
        {RATE_12M, RATE_12M, RATE_18M, RATE_24M, RATE_36M}, // fallback_rate1
    };
static const u16 wFB_Opt1[2][5] = {
>>>>>>> refs/remotes/origin/master
        {RATE_12M, RATE_18M, RATE_24M, RATE_24M, RATE_36M}, // fallback_rate0
        {RATE_6M , RATE_6M,  RATE_12M, RATE_12M, RATE_18M}, // fallback_rate1
    };

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
#define RTSDUR_BB       0
#define RTSDUR_BA       1
#define RTSDUR_AA       2
#define CTSDUR_BA       3
#define RTSDUR_BA_F0    4
#define RTSDUR_AA_F0    5
#define RTSDUR_BA_F1    6
#define RTSDUR_AA_F1    7
#define CTSDUR_BA_F0    8
#define CTSDUR_BA_F1    9
#define DATADUR_B       10
#define DATADUR_A       11
#define DATADUR_A_F0    12
#define DATADUR_A_F1    13

<<<<<<< HEAD
/*---------------------  Static Functions  --------------------------*/

static
void
s_vSaveTxPktInfo(
     PSDevice pDevice,
     BYTE byPktNum,
     PBYTE pbyDestAddr,
     WORD wPktLength,
     WORD wFIFOCtl
);

static
void *
s_vGetFreeContext(
    PSDevice pDevice
    );


static
void
s_vGenerateTxParameter(
     PSDevice         pDevice,
     BYTE             byPktType,
     WORD             wCurrentRate,
     void *pTxBufHead,
     void *pvRrvTime,
     void *pvRTS,
     void *pvCTS,
     unsigned int             cbFrameSize,
     BOOL             bNeedACK,
     unsigned int             uDMAIdx,
     PSEthernetHeader psEthHeader
    );


static unsigned int s_uFillDataHead(
     PSDevice pDevice,
     BYTE     byPktType,
     WORD     wCurrentRate,
     void *pTxDataHead,
     unsigned int     cbFrameLength,
     unsigned int     uDMAIdx,
     BOOL     bNeedAck,
     unsigned int     uFragIdx,
     unsigned int     cbLastFragmentSize,
     unsigned int     uMACfragNum,
     BYTE     byFBOption
    );




static
void
s_vGenerateMACHeader (
     PSDevice         pDevice,
     PBYTE            pbyBufferAddr,
     WORD             wDuration,
     PSEthernetHeader psEthHeader,
     BOOL             bNeedEncrypt,
     WORD             wFragType,
     unsigned int             uDMAIdx,
     unsigned int             uFragIdx
    );

static
void
s_vFillTxKey(
      PSDevice   pDevice,
      PBYTE      pbyBuf,
      PBYTE      pbyIVHead,
      PSKeyItem  pTransmitKey,
      PBYTE      pbyHdrBuf,
      WORD       wPayloadLen,
     PBYTE      pMICHDR
    );

static
void
s_vSWencryption (
      PSDevice         pDevice,
      PSKeyItem        pTransmitKey,
      PBYTE            pbyPayloadHead,
      WORD             wPayloadSize
    );

static unsigned int s_uGetTxRsvTime(
     PSDevice pDevice,
     BYTE     byPktType,
     unsigned int     cbFrameLength,
     WORD     wRate,
     BOOL     bNeedAck
    );


static unsigned int s_uGetRTSCTSRsvTime(
     PSDevice pDevice,
     BYTE byRTSRsvType,
     BYTE byPktType,
     unsigned int cbFrameLength,
     WORD wCurrentRate
    );

static
void
s_vFillCTSHead (
     PSDevice pDevice,
     unsigned int     uDMAIdx,
     BYTE     byPktType,
     void *pvCTS,
     unsigned int     cbFrameLength,
     BOOL     bNeedAck,
     BOOL     bDisCRC,
     WORD     wCurrentRate,
     BYTE     byFBOption
    );

static
void
s_vFillRTSHead(
     PSDevice         pDevice,
     BYTE             byPktType,
     void *pvRTS,
     unsigned int             cbFrameLength,
     BOOL             bNeedAck,
     BOOL             bDisCRC,
     PSEthernetHeader psEthHeader,
     WORD             wCurrentRate,
     BYTE             byFBOption
    );

static unsigned int s_uGetDataDuration(
     PSDevice pDevice,
     BYTE     byDurType,
     unsigned int     cbFrameLength,
     BYTE     byPktType,
     WORD     wRate,
     BOOL     bNeedAck,
     unsigned int     uFragIdx,
     unsigned int     cbLastFragmentSize,
     unsigned int     uMACfragNum,
     BYTE     byFBOption
    );


static
unsigned int
s_uGetRTSCTSDuration (
     PSDevice pDevice,
     BYTE byDurType,
     unsigned int cbFrameLength,
     BYTE byPktType,
     WORD wRate,
     BOOL bNeedAck,
     BYTE byFBOption
    );


/*---------------------  Export Variables  --------------------------*/

static
void *
s_vGetFreeContext(
    PSDevice pDevice
    )
{
    PUSB_SEND_CONTEXT   pContext = NULL;
    PUSB_SEND_CONTEXT   pReturnContext = NULL;
    unsigned int                ii;

    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"GetFreeContext()\n");

    for (ii = 0; ii < pDevice->cbTD; ii++) {
        pContext = pDevice->apTD[ii];
        if (pContext->bBoolInUse == FALSE) {
            pContext->bBoolInUse = TRUE;
            pReturnContext = pContext;
            break;
        }
    }
    if ( ii == pDevice->cbTD ) {
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"No Free Tx Context\n");
    }
    return (void *) pReturnContext;
}


static
void
s_vSaveTxPktInfo(PSDevice pDevice, BYTE byPktNum, PBYTE pbyDestAddr, WORD wPktLength, WORD wFIFOCtl)
{
    PSStatCounter           pStatistic=&(pDevice->scStatistic);

    if (is_broadcast_ether_addr(pbyDestAddr))
        pStatistic->abyTxPktInfo[byPktNum].byBroadMultiUni = TX_PKT_BROAD;
    else if (is_multicast_ether_addr(pbyDestAddr))
        pStatistic->abyTxPktInfo[byPktNum].byBroadMultiUni = TX_PKT_MULTI;
    else
        pStatistic->abyTxPktInfo[byPktNum].byBroadMultiUni = TX_PKT_UNI;

    pStatistic->abyTxPktInfo[byPktNum].wLength = wPktLength;
    pStatistic->abyTxPktInfo[byPktNum].wFIFOCtl = wFIFOCtl;
    memcpy(pStatistic->abyTxPktInfo[byPktNum].abyDestAddr,
	   pbyDestAddr,
	   ETH_ALEN);
}

static
void
s_vFillTxKey (
      PSDevice   pDevice,
      PBYTE      pbyBuf,
      PBYTE      pbyIVHead,
      PSKeyItem  pTransmitKey,
      PBYTE      pbyHdrBuf,
      WORD       wPayloadLen,
     PBYTE      pMICHDR
    )
{
    PDWORD          pdwIV = (PDWORD) pbyIVHead;
    PDWORD          pdwExtIV = (PDWORD) ((PBYTE)pbyIVHead+4);
    WORD            wValue;
    PS802_11Header  pMACHeader = (PS802_11Header)pbyHdrBuf;
    DWORD           dwRevIVCounter;



    //Fill TXKEY
    if (pTransmitKey == NULL)
        return;

    dwRevIVCounter = cpu_to_le32(pDevice->dwIVCounter);
    *pdwIV = pDevice->dwIVCounter;
    pDevice->byKeyIndex = pTransmitKey->dwKeyIndex & 0xf;

    if (pTransmitKey->byCipherSuite == KEY_CTL_WEP) {
        if (pTransmitKey->uKeyLength == WLAN_WEP232_KEYLEN ){
            memcpy(pDevice->abyPRNG, (PBYTE)&(dwRevIVCounter), 3);
            memcpy(pDevice->abyPRNG+3, pTransmitKey->abyKey, pTransmitKey->uKeyLength);
        } else {
            memcpy(pbyBuf, (PBYTE)&(dwRevIVCounter), 3);
            memcpy(pbyBuf+3, pTransmitKey->abyKey, pTransmitKey->uKeyLength);
            if(pTransmitKey->uKeyLength == WLAN_WEP40_KEYLEN) {
                memcpy(pbyBuf+8, (PBYTE)&(dwRevIVCounter), 3);
                memcpy(pbyBuf+11, pTransmitKey->abyKey, pTransmitKey->uKeyLength);
            }
            memcpy(pDevice->abyPRNG, pbyBuf, 16);
        }
        // Append IV after Mac Header
        *pdwIV &= WEP_IV_MASK;//00000000 11111111 11111111 11111111
        *pdwIV |= (pDevice->byKeyIndex << 30);
        *pdwIV = cpu_to_le32(*pdwIV);
        pDevice->dwIVCounter++;
        if (pDevice->dwIVCounter > WEP_IV_MASK) {
            pDevice->dwIVCounter = 0;
        }
    } else if (pTransmitKey->byCipherSuite == KEY_CTL_TKIP) {
        pTransmitKey->wTSC15_0++;
        if (pTransmitKey->wTSC15_0 == 0) {
            pTransmitKey->dwTSC47_16++;
        }
        TKIPvMixKey(pTransmitKey->abyKey, pDevice->abyCurrentNetAddr,
                    pTransmitKey->wTSC15_0, pTransmitKey->dwTSC47_16, pDevice->abyPRNG);
        memcpy(pbyBuf, pDevice->abyPRNG, 16);
        // Make IV
        memcpy(pdwIV, pDevice->abyPRNG, 3);

        *(pbyIVHead+3) = (BYTE)(((pDevice->byKeyIndex << 6) & 0xc0) | 0x20); // 0x20 is ExtIV
        // Append IV&ExtIV after Mac Header
        *pdwExtIV = cpu_to_le32(pTransmitKey->dwTSC47_16);
	DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"vFillTxKey()---- pdwExtIV: %x\n",
		*pdwExtIV);

    } else if (pTransmitKey->byCipherSuite == KEY_CTL_CCMP) {
        pTransmitKey->wTSC15_0++;
        if (pTransmitKey->wTSC15_0 == 0) {
            pTransmitKey->dwTSC47_16++;
        }
        memcpy(pbyBuf, pTransmitKey->abyKey, 16);

        // Make IV
        *pdwIV = 0;
        *(pbyIVHead+3) = (BYTE)(((pDevice->byKeyIndex << 6) & 0xc0) | 0x20); // 0x20 is ExtIV
        *pdwIV |= cpu_to_le16((WORD)(pTransmitKey->wTSC15_0));
        //Append IV&ExtIV after Mac Header
        *pdwExtIV = cpu_to_le32(pTransmitKey->dwTSC47_16);

        //Fill MICHDR0
        *pMICHDR = 0x59;
        *((PBYTE)(pMICHDR+1)) = 0; // TxPriority
        memcpy(pMICHDR+2, &(pMACHeader->abyAddr2[0]), 6);
        *((PBYTE)(pMICHDR+8)) = HIBYTE(HIWORD(pTransmitKey->dwTSC47_16));
        *((PBYTE)(pMICHDR+9)) = LOBYTE(HIWORD(pTransmitKey->dwTSC47_16));
        *((PBYTE)(pMICHDR+10)) = HIBYTE(LOWORD(pTransmitKey->dwTSC47_16));
        *((PBYTE)(pMICHDR+11)) = LOBYTE(LOWORD(pTransmitKey->dwTSC47_16));
        *((PBYTE)(pMICHDR+12)) = HIBYTE(pTransmitKey->wTSC15_0);
        *((PBYTE)(pMICHDR+13)) = LOBYTE(pTransmitKey->wTSC15_0);
        *((PBYTE)(pMICHDR+14)) = HIBYTE(wPayloadLen);
        *((PBYTE)(pMICHDR+15)) = LOBYTE(wPayloadLen);

        //Fill MICHDR1
        *((PBYTE)(pMICHDR+16)) = 0; // HLEN[15:8]
        if (pDevice->bLongHeader) {
            *((PBYTE)(pMICHDR+17)) = 28; // HLEN[7:0]
        } else {
            *((PBYTE)(pMICHDR+17)) = 22; // HLEN[7:0]
        }
        wValue = cpu_to_le16(pMACHeader->wFrameCtl & 0xC78F);
        memcpy(pMICHDR+18, (PBYTE)&wValue, 2); // MSKFRACTL
        memcpy(pMICHDR+20, &(pMACHeader->abyAddr1[0]), 6);
        memcpy(pMICHDR+26, &(pMACHeader->abyAddr2[0]), 6);

        //Fill MICHDR2
        memcpy(pMICHDR+32, &(pMACHeader->abyAddr3[0]), 6);
        wValue = pMACHeader->wSeqCtl;
        wValue &= 0x000F;
        wValue = cpu_to_le16(wValue);
        memcpy(pMICHDR+38, (PBYTE)&wValue, 2); // MSKSEQCTL
        if (pDevice->bLongHeader) {
            memcpy(pMICHDR+40, &(pMACHeader->abyAddr4[0]), 6);
        }
    }
}


static
void
s_vSWencryption (
      PSDevice            pDevice,
      PSKeyItem           pTransmitKey,
      PBYTE               pbyPayloadHead,
      WORD                wPayloadSize
    )
{
    unsigned int   cbICVlen = 4;
    DWORD  dwICV = 0xFFFFFFFFL;
    PDWORD pdwICV;
=======
static void s_vSaveTxPktInfo(struct vnt_private *pDevice, u8 byPktNum,
	u8 *pbyDestAddr, u16 wPktLength, u16 wFIFOCtl);

static struct vnt_usb_send_context *s_vGetFreeContext(struct vnt_private *);

static u16 s_vGenerateTxParameter(struct vnt_private *pDevice,
	u8 byPktType, u16 wCurrentRate,	struct vnt_tx_buffer *tx_buffer,
	struct vnt_mic_hdr **mic_hdr, u32 need_mic, u32 cbFrameSize,
	int bNeedACK, u32 uDMAIdx, struct ethhdr *psEthHeader, bool need_rts);

static void s_vGenerateMACHeader(struct vnt_private *pDevice,
	u8 *pbyBufferAddr, u16 wDuration, struct ethhdr *psEthHeader,
	int bNeedEncrypt, u16 wFragType, u32 uDMAIdx, u32 uFragIdx);

static void s_vFillTxKey(struct vnt_private *pDevice,
	struct vnt_tx_fifo_head *fifo_head, u8 *pbyIVHead,
	PSKeyItem pTransmitKey, u8 *pbyHdrBuf, u16 wPayloadLen,
	struct vnt_mic_hdr *mic_hdr);

static void s_vSWencryption(struct vnt_private *pDevice,
	PSKeyItem pTransmitKey, u8 *pbyPayloadHead, u16 wPayloadSize);

static unsigned int s_uGetTxRsvTime(struct vnt_private *pDevice, u8 byPktType,
	u32 cbFrameLength, u16 wRate, int bNeedAck);

static u16 s_uGetRTSCTSRsvTime(struct vnt_private *priv,
	u8 rsv_type, u8 pkt_type, u32 frame_lenght, u16 current_rate);

static u16 s_vFillCTSHead(struct vnt_private *pDevice, u32 uDMAIdx,
	u8 byPktType, union vnt_tx_data_head *head, u32 cbFrameLength,
	int bNeedAck, u16 wCurrentRate, u8 byFBOption);

static u16 s_vFillRTSHead(struct vnt_private *pDevice, u8 byPktType,
	union vnt_tx_data_head *head, u32 cbFrameLength, int bNeedAck,
	struct ethhdr *psEthHeader, u16 wCurrentRate, u8 byFBOption);

static u16 s_uGetDataDuration(struct vnt_private *pDevice,
	u8 byPktType, int bNeedAck);

static u16 s_uGetRTSCTSDuration(struct vnt_private *pDevice,
	u8 byDurType, u32 cbFrameLength, u8 byPktType, u16 wRate,
	int bNeedAck, u8 byFBOption);

static struct vnt_usb_send_context
	*s_vGetFreeContext(struct vnt_private *priv)
{
	struct vnt_usb_send_context *context = NULL;
	int ii;

	DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"GetFreeContext()\n");

	for (ii = 0; ii < priv->cbTD; ii++) {
		if (!priv->apTD[ii])
			return NULL;

		context = priv->apTD[ii];
		if (context->bBoolInUse == false) {
			context->bBoolInUse = true;
			memset(context->Data, 0,
					MAX_TOTAL_SIZE_WITH_ALL_HEADERS);
			return context;
		}
	}

	if (ii == priv->cbTD)
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"No Free Tx Context\n");

	return NULL;
}

static void s_vSaveTxPktInfo(struct vnt_private *pDevice, u8 byPktNum,
	u8 *pbyDestAddr, u16 wPktLength, u16 wFIFOCtl)
{
	struct net_device_stats *stats = &pDevice->stats;
	struct vnt_tx_pkt_info *pkt_info = pDevice->pkt_info;

	pkt_info[byPktNum].fifo_ctl = wFIFOCtl;
	memcpy(pkt_info[byPktNum].dest_addr, pbyDestAddr, ETH_ALEN);

	stats->tx_bytes += wPktLength;
}

static void s_vFillTxKey(struct vnt_private *pDevice,
	struct vnt_tx_fifo_head *fifo_head, u8 *pbyIVHead,
	PSKeyItem pTransmitKey, u8 *pbyHdrBuf, u16 wPayloadLen,
	struct vnt_mic_hdr *mic_hdr)
{
	u8 *pbyBuf = (u8 *)&fifo_head->adwTxKey[0];
	u32 *pdwIV = (u32 *)pbyIVHead;
	u32 *pdwExtIV = (u32 *)((u8 *)pbyIVHead + 4);
	struct ieee80211_hdr *pMACHeader = (struct ieee80211_hdr *)pbyHdrBuf;
	u32 dwRevIVCounter;

	/* Fill TXKEY */
	if (pTransmitKey == NULL)
		return;

	dwRevIVCounter = cpu_to_le32(pDevice->dwIVCounter);
	*pdwIV = pDevice->dwIVCounter;
	pDevice->byKeyIndex = pTransmitKey->dwKeyIndex & 0xf;

	switch (pTransmitKey->byCipherSuite) {
	case KEY_CTL_WEP:
		if (pTransmitKey->uKeyLength == WLAN_WEP232_KEYLEN) {
			memcpy(pDevice->abyPRNG, (u8 *)&dwRevIVCounter, 3);
			memcpy(pDevice->abyPRNG + 3, pTransmitKey->abyKey,
						pTransmitKey->uKeyLength);
		} else {
			memcpy(pbyBuf, (u8 *)&dwRevIVCounter, 3);
			memcpy(pbyBuf + 3, pTransmitKey->abyKey,
						pTransmitKey->uKeyLength);
			if (pTransmitKey->uKeyLength == WLAN_WEP40_KEYLEN) {
				memcpy(pbyBuf+8, (u8 *)&dwRevIVCounter, 3);
			memcpy(pbyBuf+11, pTransmitKey->abyKey,
						pTransmitKey->uKeyLength);
			}

			memcpy(pDevice->abyPRNG, pbyBuf, 16);
		}
		/* Append IV after Mac Header */
		*pdwIV &= WEP_IV_MASK;
		*pdwIV |= (u32)pDevice->byKeyIndex << 30;
		*pdwIV = cpu_to_le32(*pdwIV);

		pDevice->dwIVCounter++;
		if (pDevice->dwIVCounter > WEP_IV_MASK)
			pDevice->dwIVCounter = 0;

		break;
	case KEY_CTL_TKIP:
		pTransmitKey->wTSC15_0++;
		if (pTransmitKey->wTSC15_0 == 0)
			pTransmitKey->dwTSC47_16++;

		TKIPvMixKey(pTransmitKey->abyKey, pDevice->abyCurrentNetAddr,
			pTransmitKey->wTSC15_0, pTransmitKey->dwTSC47_16,
							pDevice->abyPRNG);
		memcpy(pbyBuf, pDevice->abyPRNG, 16);

		/* Make IV */
		memcpy(pdwIV, pDevice->abyPRNG, 3);

		*(pbyIVHead+3) = (u8)(((pDevice->byKeyIndex << 6) &
							0xc0) | 0x20);
		/*  Append IV&ExtIV after Mac Header */
		*pdwExtIV = cpu_to_le32(pTransmitKey->dwTSC47_16);

		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO
			"vFillTxKey()---- pdwExtIV: %x\n", *pdwExtIV);

		break;
	case KEY_CTL_CCMP:
		pTransmitKey->wTSC15_0++;
		if (pTransmitKey->wTSC15_0 == 0)
			pTransmitKey->dwTSC47_16++;

		memcpy(pbyBuf, pTransmitKey->abyKey, 16);

		/* Make IV */
		*pdwIV = 0;
		*(pbyIVHead+3) = (u8)(((pDevice->byKeyIndex << 6) &
							0xc0) | 0x20);

		*pdwIV |= cpu_to_le16((u16)(pTransmitKey->wTSC15_0));

		/* Append IV&ExtIV after Mac Header */
		*pdwExtIV = cpu_to_le32(pTransmitKey->dwTSC47_16);

		if (!mic_hdr)
			return;

		/* MICHDR0 */
		mic_hdr->id = 0x59;
		mic_hdr->payload_len = cpu_to_be16(wPayloadLen);
		memcpy(mic_hdr->mic_addr2, pMACHeader->addr2, ETH_ALEN);

		mic_hdr->tsc_47_16 = cpu_to_be32(pTransmitKey->dwTSC47_16);
		mic_hdr->tsc_15_0 = cpu_to_be16(pTransmitKey->wTSC15_0);

		/* MICHDR1 */
		if (ieee80211_has_a4(pMACHeader->frame_control))
			mic_hdr->hlen = cpu_to_be16(28);
		else
			mic_hdr->hlen = cpu_to_be16(22);

		memcpy(mic_hdr->addr1, pMACHeader->addr1, ETH_ALEN);
		memcpy(mic_hdr->addr2, pMACHeader->addr2, ETH_ALEN);

		/* MICHDR2 */
		memcpy(mic_hdr->addr3, pMACHeader->addr3, ETH_ALEN);
		mic_hdr->frame_control = cpu_to_le16(pMACHeader->frame_control
								& 0xc78f);
		mic_hdr->seq_ctrl = cpu_to_le16(pMACHeader->seq_ctrl & 0xf);

		if (ieee80211_has_a4(pMACHeader->frame_control))
			memcpy(mic_hdr->addr4, pMACHeader->addr4, ETH_ALEN);
	}
}

static void s_vSWencryption(struct vnt_private *pDevice,
	PSKeyItem pTransmitKey, u8 *pbyPayloadHead, u16 wPayloadSize)
{
	u32 cbICVlen = 4;
	u32 dwICV = 0xffffffff;
	u32 *pdwICV;
>>>>>>> refs/remotes/origin/master

    if (pTransmitKey == NULL)
        return;

    if (pTransmitKey->byCipherSuite == KEY_CTL_WEP) {
        //=======================================================================
        // Append ICV after payload
        dwICV = CRCdwGetCrc32Ex(pbyPayloadHead, wPayloadSize, dwICV);//ICV(Payload)
<<<<<<< HEAD
        pdwICV = (PDWORD)(pbyPayloadHead + wPayloadSize);
=======
        pdwICV = (u32 *)(pbyPayloadHead + wPayloadSize);
>>>>>>> refs/remotes/origin/master
        // finally, we must invert dwCRC to get the correct answer
        *pdwICV = cpu_to_le32(~dwICV);
        // RC4 encryption
        rc4_init(&pDevice->SBox, pDevice->abyPRNG, pTransmitKey->uKeyLength + 3);
        rc4_encrypt(&pDevice->SBox, pbyPayloadHead, pbyPayloadHead, wPayloadSize+cbICVlen);
        //=======================================================================
    } else if (pTransmitKey->byCipherSuite == KEY_CTL_TKIP) {
        //=======================================================================
        //Append ICV after payload
        dwICV = CRCdwGetCrc32Ex(pbyPayloadHead, wPayloadSize, dwICV);//ICV(Payload)
<<<<<<< HEAD
        pdwICV = (PDWORD)(pbyPayloadHead + wPayloadSize);
=======
        pdwICV = (u32 *)(pbyPayloadHead + wPayloadSize);
>>>>>>> refs/remotes/origin/master
        // finally, we must invert dwCRC to get the correct answer
        *pdwICV = cpu_to_le32(~dwICV);
        // RC4 encryption
        rc4_init(&pDevice->SBox, pDevice->abyPRNG, TKIP_KEY_LEN);
        rc4_encrypt(&pDevice->SBox, pbyPayloadHead, pbyPayloadHead, wPayloadSize+cbICVlen);
        //=======================================================================
    }
}

<<<<<<< HEAD


=======
static u16 vnt_time_stamp_off(struct vnt_private *priv, u16 rate)
{
	return cpu_to_le16(wTimeStampOff[priv->byPreambleType % 2]
							[rate % MAX_RATE]);
}
>>>>>>> refs/remotes/origin/master

/*byPktType : PK_TYPE_11A     0
             PK_TYPE_11B     1
             PK_TYPE_11GB    2
             PK_TYPE_11GA    3
*/
<<<<<<< HEAD
static
unsigned int
s_uGetTxRsvTime (
     PSDevice pDevice,
     BYTE     byPktType,
     unsigned int     cbFrameLength,
     WORD     wRate,
     BOOL     bNeedAck
    )
{
    unsigned int uDataTime, uAckTime;

    uDataTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, cbFrameLength, wRate);
    if (byPktType == PK_TYPE_11B) {//llb,CCK mode
        uAckTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 14, (WORD)pDevice->byTopCCKBasicRate);
    } else {//11g 2.4G OFDM mode & 11a 5G OFDM mode
        uAckTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 14, (WORD)pDevice->byTopOFDMBasicRate);
    }

    if (bNeedAck) {
        return (uDataTime + pDevice->uSIFS + uAckTime);
    }
    else {
        return uDataTime;
    }
}

//byFreqType: 0=>5GHZ 1=>2.4GHZ
static
unsigned int
s_uGetRTSCTSRsvTime (
     PSDevice pDevice,
     BYTE byRTSRsvType,
     BYTE byPktType,
     unsigned int cbFrameLength,
     WORD wCurrentRate
    )
{
    unsigned int uRrvTime  , uRTSTime, uCTSTime, uAckTime, uDataTime;

    uRrvTime = uRTSTime = uCTSTime = uAckTime = uDataTime = 0;


    uDataTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, cbFrameLength, wCurrentRate);
    if (byRTSRsvType == 0) { //RTSTxRrvTime_bb
        uRTSTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 20, pDevice->byTopCCKBasicRate);
        uCTSTime = uAckTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 14, pDevice->byTopCCKBasicRate);
    }
    else if (byRTSRsvType == 1){ //RTSTxRrvTime_ba, only in 2.4GHZ
        uRTSTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 20, pDevice->byTopCCKBasicRate);
        uCTSTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 14, pDevice->byTopCCKBasicRate);
        uAckTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 14, pDevice->byTopOFDMBasicRate);
    }
    else if (byRTSRsvType == 2) { //RTSTxRrvTime_aa
        uRTSTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 20, pDevice->byTopOFDMBasicRate);
        uCTSTime = uAckTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 14, pDevice->byTopOFDMBasicRate);
    }
    else if (byRTSRsvType == 3) { //CTSTxRrvTime_ba, only in 2.4GHZ
        uCTSTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 14, pDevice->byTopCCKBasicRate);
        uAckTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 14, pDevice->byTopOFDMBasicRate);
        uRrvTime = uCTSTime + uAckTime + uDataTime + 2*pDevice->uSIFS;
        return uRrvTime;
    }

    //RTSRrvTime
    uRrvTime = uRTSTime + uCTSTime + uAckTime + uDataTime + 3*pDevice->uSIFS;
    return uRrvTime;
}

//byFreqType 0: 5GHz, 1:2.4Ghz
static
unsigned int
s_uGetDataDuration (
     PSDevice pDevice,
     BYTE     byDurType,
     unsigned int     cbFrameLength,
     BYTE     byPktType,
     WORD     wRate,
     BOOL     bNeedAck,
     unsigned int     uFragIdx,
     unsigned int     cbLastFragmentSize,
     unsigned int     uMACfragNum,
     BYTE     byFBOption
    )
{
    BOOL bLastFrag = 0;
    unsigned int uAckTime = 0, uNextPktTime = 0;

    if (uFragIdx == (uMACfragNum-1)) {
        bLastFrag = 1;
    }

    switch (byDurType) {

    case DATADUR_B:    //DATADUR_B
        if (((uMACfragNum == 1)) || (bLastFrag == 1)) {//Non Frag or Last Frag
            if (bNeedAck) {
            	uAckTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 14, pDevice->byTopCCKBasicRate);
                return (pDevice->uSIFS + uAckTime);
            } else {
                return 0;
            }
        }
        else {//First Frag or Mid Frag
            if (uFragIdx == (uMACfragNum-2)) {
            	uNextPktTime = s_uGetTxRsvTime(pDevice, byPktType, cbLastFragmentSize, wRate, bNeedAck);
            } else {
                uNextPktTime = s_uGetTxRsvTime(pDevice, byPktType, cbFrameLength, wRate, bNeedAck);
            }
            if (bNeedAck) {
            	uAckTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 14, pDevice->byTopCCKBasicRate);
                return (pDevice->uSIFS + uAckTime + uNextPktTime);
            } else {
                return (pDevice->uSIFS + uNextPktTime);
            }
        }
        break;


    case DATADUR_A:    //DATADUR_A
        if (((uMACfragNum==1)) || (bLastFrag==1)) {//Non Frag or Last Frag
            if(bNeedAck){
            	uAckTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 14, pDevice->byTopOFDMBasicRate);
                return (pDevice->uSIFS + uAckTime);
            } else {
                return 0;
            }
        }
        else {//First Frag or Mid Frag
            if(uFragIdx == (uMACfragNum-2)){
            	uNextPktTime = s_uGetTxRsvTime(pDevice, byPktType, cbLastFragmentSize, wRate, bNeedAck);
            } else {
                uNextPktTime = s_uGetTxRsvTime(pDevice, byPktType, cbFrameLength, wRate, bNeedAck);
            }
            if(bNeedAck){
            	uAckTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 14, pDevice->byTopOFDMBasicRate);
                return (pDevice->uSIFS + uAckTime + uNextPktTime);
            } else {
                return (pDevice->uSIFS + uNextPktTime);
            }
        }
        break;

    case DATADUR_A_F0:    //DATADUR_A_F0
	    if (((uMACfragNum==1)) || (bLastFrag==1)) {//Non Frag or Last Frag
            if(bNeedAck){
            	uAckTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 14, pDevice->byTopOFDMBasicRate);
                return (pDevice->uSIFS + uAckTime);
            } else {
                return 0;
            }
        }
	    else { //First Frag or Mid Frag
	        if (byFBOption == AUTO_FB_0) {
                if (wRate < RATE_18M)
                    wRate = RATE_18M;
                else if (wRate > RATE_54M)
                    wRate = RATE_54M;

	            if(uFragIdx == (uMACfragNum-2)){
            	    uNextPktTime = s_uGetTxRsvTime(pDevice, byPktType, cbLastFragmentSize, wFB_Opt0[FB_RATE0][wRate-RATE_18M], bNeedAck);
                } else {
                    uNextPktTime = s_uGetTxRsvTime(pDevice, byPktType, cbFrameLength, wFB_Opt0[FB_RATE0][wRate-RATE_18M], bNeedAck);
                }
	        } else { // (byFBOption == AUTO_FB_1)
                if (wRate < RATE_18M)
                    wRate = RATE_18M;
                else if (wRate > RATE_54M)
                    wRate = RATE_54M;

	            if(uFragIdx == (uMACfragNum-2)){
            	    uNextPktTime = s_uGetTxRsvTime(pDevice, byPktType, cbLastFragmentSize, wFB_Opt1[FB_RATE0][wRate-RATE_18M], bNeedAck);
                } else {
                    uNextPktTime = s_uGetTxRsvTime(pDevice, byPktType, cbFrameLength, wFB_Opt1[FB_RATE0][wRate-RATE_18M], bNeedAck);
                }
	        }

	        if(bNeedAck){
            	uAckTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 14, pDevice->byTopOFDMBasicRate);
                return (pDevice->uSIFS + uAckTime + uNextPktTime);
            } else {
                return (pDevice->uSIFS + uNextPktTime);
            }
	    }
        break;

    case DATADUR_A_F1:    //DATADUR_A_F1
        if (((uMACfragNum==1)) || (bLastFrag==1)) {//Non Frag or Last Frag
            if(bNeedAck){
            	uAckTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 14, pDevice->byTopOFDMBasicRate);
                return (pDevice->uSIFS + uAckTime);
            } else {
                return 0;
            }
        }
	    else { //First Frag or Mid Frag
	        if (byFBOption == AUTO_FB_0) {
                if (wRate < RATE_18M)
                    wRate = RATE_18M;
                else if (wRate > RATE_54M)
                    wRate = RATE_54M;

	            if(uFragIdx == (uMACfragNum-2)){
            	    uNextPktTime = s_uGetTxRsvTime(pDevice, byPktType, cbLastFragmentSize, wFB_Opt0[FB_RATE1][wRate-RATE_18M], bNeedAck);
                } else {
                    uNextPktTime = s_uGetTxRsvTime(pDevice, byPktType, cbFrameLength, wFB_Opt0[FB_RATE1][wRate-RATE_18M], bNeedAck);
                }

	        } else { // (byFBOption == AUTO_FB_1)
                if (wRate < RATE_18M)
                    wRate = RATE_18M;
                else if (wRate > RATE_54M)
                    wRate = RATE_54M;

	            if(uFragIdx == (uMACfragNum-2)){
            	    uNextPktTime = s_uGetTxRsvTime(pDevice, byPktType, cbLastFragmentSize, wFB_Opt1[FB_RATE1][wRate-RATE_18M], bNeedAck);
                } else {
                    uNextPktTime = s_uGetTxRsvTime(pDevice, byPktType, cbFrameLength, wFB_Opt1[FB_RATE1][wRate-RATE_18M], bNeedAck);
                }
	        }
	        if(bNeedAck){
            	uAckTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 14, pDevice->byTopOFDMBasicRate);
                return (pDevice->uSIFS + uAckTime + uNextPktTime);
            } else {
                return (pDevice->uSIFS + uNextPktTime);
            }
	    }
        break;

    default:
        break;
    }

	ASSERT(FALSE);
	return 0;
}


//byFreqType: 0=>5GHZ 1=>2.4GHZ
static
unsigned int
s_uGetRTSCTSDuration (
     PSDevice pDevice,
     BYTE byDurType,
     unsigned int cbFrameLength,
     BYTE byPktType,
     WORD wRate,
     BOOL bNeedAck,
     BYTE byFBOption
    )
{
    unsigned int uCTSTime = 0, uDurTime = 0;


    switch (byDurType) {

    case RTSDUR_BB:    //RTSDuration_bb
        uCTSTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 14, pDevice->byTopCCKBasicRate);
        uDurTime = uCTSTime + 2*pDevice->uSIFS + s_uGetTxRsvTime(pDevice, byPktType, cbFrameLength, wRate, bNeedAck);
        break;

    case RTSDUR_BA:    //RTSDuration_ba
        uCTSTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 14, pDevice->byTopCCKBasicRate);
        uDurTime = uCTSTime + 2*pDevice->uSIFS + s_uGetTxRsvTime(pDevice, byPktType, cbFrameLength, wRate, bNeedAck);
        break;

    case RTSDUR_AA:    //RTSDuration_aa
        uCTSTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 14, pDevice->byTopOFDMBasicRate);
        uDurTime = uCTSTime + 2*pDevice->uSIFS + s_uGetTxRsvTime(pDevice, byPktType, cbFrameLength, wRate, bNeedAck);
        break;

    case CTSDUR_BA:    //CTSDuration_ba
        uDurTime = pDevice->uSIFS + s_uGetTxRsvTime(pDevice, byPktType, cbFrameLength, wRate, bNeedAck);
        break;

    case RTSDUR_BA_F0: //RTSDuration_ba_f0
        uCTSTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 14, pDevice->byTopCCKBasicRate);
        if ((byFBOption == AUTO_FB_0) && (wRate >= RATE_18M) && (wRate <=RATE_54M)) {
            uDurTime = uCTSTime + 2*pDevice->uSIFS + s_uGetTxRsvTime(pDevice, byPktType, cbFrameLength, wFB_Opt0[FB_RATE0][wRate-RATE_18M], bNeedAck);
        } else if ((byFBOption == AUTO_FB_1) && (wRate >= RATE_18M) && (wRate <=RATE_54M)) {
            uDurTime = uCTSTime + 2*pDevice->uSIFS + s_uGetTxRsvTime(pDevice, byPktType, cbFrameLength, wFB_Opt1[FB_RATE0][wRate-RATE_18M], bNeedAck);
        }
        break;

    case RTSDUR_AA_F0: //RTSDuration_aa_f0
        uCTSTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 14, pDevice->byTopOFDMBasicRate);
        if ((byFBOption == AUTO_FB_0) && (wRate >= RATE_18M) && (wRate <=RATE_54M)) {
            uDurTime = uCTSTime + 2*pDevice->uSIFS + s_uGetTxRsvTime(pDevice, byPktType, cbFrameLength, wFB_Opt0[FB_RATE0][wRate-RATE_18M], bNeedAck);
        } else if ((byFBOption == AUTO_FB_1) && (wRate >= RATE_18M) && (wRate <=RATE_54M)) {
            uDurTime = uCTSTime + 2*pDevice->uSIFS + s_uGetTxRsvTime(pDevice, byPktType, cbFrameLength, wFB_Opt1[FB_RATE0][wRate-RATE_18M], bNeedAck);
        }
        break;

    case RTSDUR_BA_F1: //RTSDuration_ba_f1
        uCTSTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 14, pDevice->byTopCCKBasicRate);
        if ((byFBOption == AUTO_FB_0) && (wRate >= RATE_18M) && (wRate <=RATE_54M)) {
            uDurTime = uCTSTime + 2*pDevice->uSIFS + s_uGetTxRsvTime(pDevice, byPktType, cbFrameLength, wFB_Opt0[FB_RATE1][wRate-RATE_18M], bNeedAck);
        } else if ((byFBOption == AUTO_FB_1) && (wRate >= RATE_18M) && (wRate <=RATE_54M)) {
            uDurTime = uCTSTime + 2*pDevice->uSIFS + s_uGetTxRsvTime(pDevice, byPktType, cbFrameLength, wFB_Opt1[FB_RATE1][wRate-RATE_18M], bNeedAck);
        }
        break;

    case RTSDUR_AA_F1: //RTSDuration_aa_f1
        uCTSTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType, 14, pDevice->byTopOFDMBasicRate);
        if ((byFBOption == AUTO_FB_0) && (wRate >= RATE_18M) && (wRate <=RATE_54M)) {
            uDurTime = uCTSTime + 2*pDevice->uSIFS + s_uGetTxRsvTime(pDevice, byPktType, cbFrameLength, wFB_Opt0[FB_RATE1][wRate-RATE_18M], bNeedAck);
        } else if ((byFBOption == AUTO_FB_1) && (wRate >= RATE_18M) && (wRate <=RATE_54M)) {
            uDurTime = uCTSTime + 2*pDevice->uSIFS + s_uGetTxRsvTime(pDevice, byPktType, cbFrameLength, wFB_Opt1[FB_RATE1][wRate-RATE_18M], bNeedAck);
        }
        break;

    case CTSDUR_BA_F0: //CTSDuration_ba_f0
        if ((byFBOption == AUTO_FB_0) && (wRate >= RATE_18M) && (wRate <=RATE_54M)) {
            uDurTime = pDevice->uSIFS + s_uGetTxRsvTime(pDevice, byPktType, cbFrameLength, wFB_Opt0[FB_RATE0][wRate-RATE_18M], bNeedAck);
        } else if ((byFBOption == AUTO_FB_1) && (wRate >= RATE_18M) && (wRate <=RATE_54M)) {
            uDurTime = pDevice->uSIFS + s_uGetTxRsvTime(pDevice, byPktType, cbFrameLength, wFB_Opt1[FB_RATE0][wRate-RATE_18M], bNeedAck);
        }
        break;

    case CTSDUR_BA_F1: //CTSDuration_ba_f1
        if ((byFBOption == AUTO_FB_0) && (wRate >= RATE_18M) && (wRate <=RATE_54M)) {
            uDurTime = pDevice->uSIFS + s_uGetTxRsvTime(pDevice, byPktType, cbFrameLength, wFB_Opt0[FB_RATE1][wRate-RATE_18M], bNeedAck);
        } else if ((byFBOption == AUTO_FB_1) && (wRate >= RATE_18M) && (wRate <=RATE_54M)) {
            uDurTime = pDevice->uSIFS + s_uGetTxRsvTime(pDevice, byPktType, cbFrameLength, wFB_Opt1[FB_RATE1][wRate-RATE_18M], bNeedAck);
        }
        break;

    default:
        break;
    }

    return uDurTime;

}




static
unsigned int
s_uFillDataHead (
     PSDevice pDevice,
     BYTE     byPktType,
     WORD     wCurrentRate,
     void *pTxDataHead,
     unsigned int     cbFrameLength,
     unsigned int     uDMAIdx,
     BOOL     bNeedAck,
     unsigned int     uFragIdx,
     unsigned int     cbLastFragmentSize,
     unsigned int     uMACfragNum,
     BYTE     byFBOption
    )
{

    if (pTxDataHead == NULL) {
        return 0;
    }

    if (byPktType == PK_TYPE_11GB || byPktType == PK_TYPE_11GA) {
	if ((uDMAIdx == TYPE_ATIMDMA) || (uDMAIdx == TYPE_BEACONDMA)) {
		PSTxDataHead_ab pBuf = (PSTxDataHead_ab) pTxDataHead;
            //Get SignalField,ServiceField,Length
            BBvCaculateParameter(pDevice, cbFrameLength, wCurrentRate, byPktType,
                (PWORD)&(pBuf->wTransmitLength), (PBYTE)&(pBuf->byServiceField), (PBYTE)&(pBuf->bySignalField)
            );
            //Get Duration and TimeStampOff
            pBuf->wDuration = (WORD)s_uGetDataDuration(pDevice, DATADUR_A, cbFrameLength, byPktType,
                                                       wCurrentRate, bNeedAck, uFragIdx,
                                                       cbLastFragmentSize, uMACfragNum,
                                                       byFBOption); //1: 2.4GHz
            if(uDMAIdx!=TYPE_ATIMDMA) {
                pBuf->wTimeStampOff = wTimeStampOff[pDevice->byPreambleType%2][wCurrentRate%MAX_RATE];
            }
            return (pBuf->wDuration);
        }
        else { // DATA & MANAGE Frame
            if (byFBOption == AUTO_FB_NONE) {
                PSTxDataHead_g pBuf = (PSTxDataHead_g)pTxDataHead;
                //Get SignalField,ServiceField,Length
                BBvCaculateParameter(pDevice, cbFrameLength, wCurrentRate, byPktType,
                    (PWORD)&(pBuf->wTransmitLength_a), (PBYTE)&(pBuf->byServiceField_a), (PBYTE)&(pBuf->bySignalField_a)
                );
                BBvCaculateParameter(pDevice, cbFrameLength, pDevice->byTopCCKBasicRate, PK_TYPE_11B,
                    (PWORD)&(pBuf->wTransmitLength_b), (PBYTE)&(pBuf->byServiceField_b), (PBYTE)&(pBuf->bySignalField_b)
                );
                //Get Duration and TimeStamp
                pBuf->wDuration_a = (WORD)s_uGetDataDuration(pDevice, DATADUR_A, cbFrameLength,
                                                             byPktType, wCurrentRate, bNeedAck, uFragIdx,
                                                             cbLastFragmentSize, uMACfragNum,
                                                             byFBOption); //1: 2.4GHz
                pBuf->wDuration_b = (WORD)s_uGetDataDuration(pDevice, DATADUR_B, cbFrameLength,
                                                             PK_TYPE_11B, pDevice->byTopCCKBasicRate,
                                                             bNeedAck, uFragIdx, cbLastFragmentSize,
                                                             uMACfragNum, byFBOption); //1: 2.4GHz

                pBuf->wTimeStampOff_a = wTimeStampOff[pDevice->byPreambleType%2][wCurrentRate%MAX_RATE];
                pBuf->wTimeStampOff_b = wTimeStampOff[pDevice->byPreambleType%2][pDevice->byTopCCKBasicRate%MAX_RATE];
                return (pBuf->wDuration_a);
             } else {
                // Auto Fallback
                PSTxDataHead_g_FB pBuf = (PSTxDataHead_g_FB)pTxDataHead;
                //Get SignalField,ServiceField,Length
                BBvCaculateParameter(pDevice, cbFrameLength, wCurrentRate, byPktType,
                    (PWORD)&(pBuf->wTransmitLength_a), (PBYTE)&(pBuf->byServiceField_a), (PBYTE)&(pBuf->bySignalField_a)
                );
                BBvCaculateParameter(pDevice, cbFrameLength, pDevice->byTopCCKBasicRate, PK_TYPE_11B,
                    (PWORD)&(pBuf->wTransmitLength_b), (PBYTE)&(pBuf->byServiceField_b), (PBYTE)&(pBuf->bySignalField_b)
                );
                //Get Duration and TimeStamp
                pBuf->wDuration_a = (WORD)s_uGetDataDuration(pDevice, DATADUR_A, cbFrameLength, byPktType,
                                             wCurrentRate, bNeedAck, uFragIdx, cbLastFragmentSize, uMACfragNum, byFBOption); //1: 2.4GHz
                pBuf->wDuration_b = (WORD)s_uGetDataDuration(pDevice, DATADUR_B, cbFrameLength, PK_TYPE_11B,
                                             pDevice->byTopCCKBasicRate, bNeedAck, uFragIdx, cbLastFragmentSize, uMACfragNum, byFBOption); //1: 2.4GHz
                pBuf->wDuration_a_f0 = (WORD)s_uGetDataDuration(pDevice, DATADUR_A_F0, cbFrameLength, byPktType,
                                             wCurrentRate, bNeedAck, uFragIdx, cbLastFragmentSize, uMACfragNum, byFBOption); //1: 2.4GHz
                pBuf->wDuration_a_f1 = (WORD)s_uGetDataDuration(pDevice, DATADUR_A_F1, cbFrameLength, byPktType,
                                             wCurrentRate, bNeedAck, uFragIdx, cbLastFragmentSize, uMACfragNum, byFBOption); //1: 2.4GHz
                pBuf->wTimeStampOff_a = wTimeStampOff[pDevice->byPreambleType%2][wCurrentRate%MAX_RATE];
                pBuf->wTimeStampOff_b = wTimeStampOff[pDevice->byPreambleType%2][pDevice->byTopCCKBasicRate%MAX_RATE];
                return (pBuf->wDuration_a);
            } //if (byFBOption == AUTO_FB_NONE)
        }
    }
    else if (byPktType == PK_TYPE_11A) {
        if ((byFBOption != AUTO_FB_NONE) && (uDMAIdx != TYPE_ATIMDMA) && (uDMAIdx != TYPE_BEACONDMA)) {
            // Auto Fallback
            PSTxDataHead_a_FB pBuf = (PSTxDataHead_a_FB)pTxDataHead;
            //Get SignalField,ServiceField,Length
            BBvCaculateParameter(pDevice, cbFrameLength, wCurrentRate, byPktType,
                (PWORD)&(pBuf->wTransmitLength), (PBYTE)&(pBuf->byServiceField), (PBYTE)&(pBuf->bySignalField)
            );
            //Get Duration and TimeStampOff
            pBuf->wDuration = (WORD)s_uGetDataDuration(pDevice, DATADUR_A, cbFrameLength, byPktType,
                                        wCurrentRate, bNeedAck, uFragIdx, cbLastFragmentSize, uMACfragNum, byFBOption); //0: 5GHz
            pBuf->wDuration_f0 = (WORD)s_uGetDataDuration(pDevice, DATADUR_A_F0, cbFrameLength, byPktType,
                                        wCurrentRate, bNeedAck, uFragIdx, cbLastFragmentSize, uMACfragNum, byFBOption); //0: 5GHz
            pBuf->wDuration_f1 = (WORD)s_uGetDataDuration(pDevice, DATADUR_A_F1, cbFrameLength, byPktType,
                                        wCurrentRate, bNeedAck, uFragIdx, cbLastFragmentSize, uMACfragNum, byFBOption); //0: 5GHz
            if(uDMAIdx!=TYPE_ATIMDMA) {
                pBuf->wTimeStampOff = wTimeStampOff[pDevice->byPreambleType%2][wCurrentRate%MAX_RATE];
            }
            return (pBuf->wDuration);
        } else {
            PSTxDataHead_ab pBuf = (PSTxDataHead_ab)pTxDataHead;
            //Get SignalField,ServiceField,Length
            BBvCaculateParameter(pDevice, cbFrameLength, wCurrentRate, byPktType,
                (PWORD)&(pBuf->wTransmitLength), (PBYTE)&(pBuf->byServiceField), (PBYTE)&(pBuf->bySignalField)
            );
            //Get Duration and TimeStampOff
            pBuf->wDuration = (WORD)s_uGetDataDuration(pDevice, DATADUR_A, cbFrameLength, byPktType,
                                                       wCurrentRate, bNeedAck, uFragIdx,
                                                       cbLastFragmentSize, uMACfragNum,
                                                       byFBOption);

            if(uDMAIdx!=TYPE_ATIMDMA) {
                pBuf->wTimeStampOff = wTimeStampOff[pDevice->byPreambleType%2][wCurrentRate%MAX_RATE];
            }
            return (pBuf->wDuration);
        }
    }
    else if (byPktType == PK_TYPE_11B) {
            PSTxDataHead_ab pBuf = (PSTxDataHead_ab)pTxDataHead;
            //Get SignalField,ServiceField,Length
            BBvCaculateParameter(pDevice, cbFrameLength, wCurrentRate, byPktType,
                (PWORD)&(pBuf->wTransmitLength), (PBYTE)&(pBuf->byServiceField), (PBYTE)&(pBuf->bySignalField)
            );
            //Get Duration and TimeStampOff
            pBuf->wDuration = (WORD)s_uGetDataDuration(pDevice, DATADUR_B, cbFrameLength, byPktType,
                                                       wCurrentRate, bNeedAck, uFragIdx,
                                                       cbLastFragmentSize, uMACfragNum,
                                                       byFBOption);
            if (uDMAIdx != TYPE_ATIMDMA) {
                pBuf->wTimeStampOff = wTimeStampOff[pDevice->byPreambleType%2][wCurrentRate%MAX_RATE];
            }
            return (pBuf->wDuration);
    }
    return 0;
}




static
void
s_vFillRTSHead (
     PSDevice         pDevice,
     BYTE             byPktType,
     void *pvRTS,
     unsigned int             cbFrameLength,
     BOOL             bNeedAck,
     BOOL             bDisCRC,
     PSEthernetHeader psEthHeader,
     WORD             wCurrentRate,
     BYTE             byFBOption
    )
{
    unsigned int uRTSFrameLen = 20;
    WORD  wLen = 0x0000;

    if (pvRTS == NULL)
    	return;

    if (bDisCRC) {
        // When CRCDIS bit is on, H/W forgot to generate FCS for RTS frame,
        // in this case we need to decrease its length by 4.
        uRTSFrameLen -= 4;
    }

    // Note: So far RTSHead dosen't appear in ATIM & Beacom DMA, so we don't need to take them into account.
    //       Otherwise, we need to modified codes for them.
    if (byPktType == PK_TYPE_11GB || byPktType == PK_TYPE_11GA) {
        if (byFBOption == AUTO_FB_NONE) {
            PSRTS_g pBuf = (PSRTS_g)pvRTS;
            //Get SignalField,ServiceField,Length
            BBvCaculateParameter(pDevice, uRTSFrameLen, pDevice->byTopCCKBasicRate, PK_TYPE_11B,
                (PWORD)&(wLen), (PBYTE)&(pBuf->byServiceField_b), (PBYTE)&(pBuf->bySignalField_b)
            );
            pBuf->wTransmitLength_b = cpu_to_le16(wLen);
            BBvCaculateParameter(pDevice, uRTSFrameLen, pDevice->byTopOFDMBasicRate, byPktType,
                (PWORD)&(wLen), (PBYTE)&(pBuf->byServiceField_a), (PBYTE)&(pBuf->bySignalField_a)
            );
            pBuf->wTransmitLength_a = cpu_to_le16(wLen);
            //Get Duration
            pBuf->wDuration_bb = cpu_to_le16((WORD)s_uGetRTSCTSDuration(pDevice, RTSDUR_BB, cbFrameLength, PK_TYPE_11B, pDevice->byTopCCKBasicRate, bNeedAck, byFBOption));    //0:RTSDuration_bb, 1:2.4G, 1:CCKData
            pBuf->wDuration_aa = cpu_to_le16((WORD)s_uGetRTSCTSDuration(pDevice, RTSDUR_AA, cbFrameLength, byPktType, wCurrentRate, bNeedAck, byFBOption)); //2:RTSDuration_aa, 1:2.4G, 2,3: 2.4G OFDMData
            pBuf->wDuration_ba = cpu_to_le16((WORD)s_uGetRTSCTSDuration(pDevice, RTSDUR_BA, cbFrameLength, byPktType, wCurrentRate, bNeedAck, byFBOption)); //1:RTSDuration_ba, 1:2.4G, 2,3:2.4G OFDM Data

            pBuf->Data.wDurationID = pBuf->wDuration_aa;
            //Get RTS Frame body
            pBuf->Data.wFrameControl = TYPE_CTL_RTS;//0x00B4

	if ((pDevice->eOPMode == OP_MODE_ADHOC) ||
	    (pDevice->eOPMode == OP_MODE_AP)) {
		memcpy(&(pBuf->Data.abyRA[0]),
		       &(psEthHeader->abyDstAddr[0]),
		       ETH_ALEN);
	}
            else {
		    memcpy(&(pBuf->Data.abyRA[0]),
			   &(pDevice->abyBSSID[0]),
			   ETH_ALEN);
	    }
	if (pDevice->eOPMode == OP_MODE_AP) {
		memcpy(&(pBuf->Data.abyTA[0]),
		       &(pDevice->abyBSSID[0]),
		       ETH_ALEN);
	}
            else {
		    memcpy(&(pBuf->Data.abyTA[0]),
			   &(psEthHeader->abySrcAddr[0]),
			   ETH_ALEN);
            }
        }
        else {
           PSRTS_g_FB pBuf = (PSRTS_g_FB)pvRTS;
            //Get SignalField,ServiceField,Length
            BBvCaculateParameter(pDevice, uRTSFrameLen, pDevice->byTopCCKBasicRate, PK_TYPE_11B,
                (PWORD)&(wLen), (PBYTE)&(pBuf->byServiceField_b), (PBYTE)&(pBuf->bySignalField_b)
            );
            pBuf->wTransmitLength_b = cpu_to_le16(wLen);
            BBvCaculateParameter(pDevice, uRTSFrameLen, pDevice->byTopOFDMBasicRate, byPktType,
                (PWORD)&(wLen), (PBYTE)&(pBuf->byServiceField_a), (PBYTE)&(pBuf->bySignalField_a)
            );
            pBuf->wTransmitLength_a = cpu_to_le16(wLen);
            //Get Duration
            pBuf->wDuration_bb = cpu_to_le16((WORD)s_uGetRTSCTSDuration(pDevice, RTSDUR_BB, cbFrameLength, PK_TYPE_11B, pDevice->byTopCCKBasicRate, bNeedAck, byFBOption));    //0:RTSDuration_bb, 1:2.4G, 1:CCKData
            pBuf->wDuration_aa = cpu_to_le16((WORD)s_uGetRTSCTSDuration(pDevice, RTSDUR_AA, cbFrameLength, byPktType, wCurrentRate, bNeedAck, byFBOption)); //2:RTSDuration_aa, 1:2.4G, 2,3:2.4G OFDMData
            pBuf->wDuration_ba = cpu_to_le16((WORD)s_uGetRTSCTSDuration(pDevice, RTSDUR_BA, cbFrameLength, byPktType, wCurrentRate, bNeedAck, byFBOption)); //1:RTSDuration_ba, 1:2.4G, 2,3:2.4G OFDMData
            pBuf->wRTSDuration_ba_f0 = cpu_to_le16((WORD)s_uGetRTSCTSDuration(pDevice, RTSDUR_BA_F0, cbFrameLength, byPktType, wCurrentRate, bNeedAck, byFBOption));    //4:wRTSDuration_ba_f0, 1:2.4G, 1:CCKData
            pBuf->wRTSDuration_aa_f0 = cpu_to_le16((WORD)s_uGetRTSCTSDuration(pDevice, RTSDUR_AA_F0, cbFrameLength, byPktType, wCurrentRate, bNeedAck, byFBOption));    //5:wRTSDuration_aa_f0, 1:2.4G, 1:CCKData
            pBuf->wRTSDuration_ba_f1 = cpu_to_le16((WORD)s_uGetRTSCTSDuration(pDevice, RTSDUR_BA_F1, cbFrameLength, byPktType, wCurrentRate, bNeedAck, byFBOption));    //6:wRTSDuration_ba_f1, 1:2.4G, 1:CCKData
            pBuf->wRTSDuration_aa_f1 = cpu_to_le16((WORD)s_uGetRTSCTSDuration(pDevice, RTSDUR_AA_F1, cbFrameLength, byPktType, wCurrentRate, bNeedAck, byFBOption));    //7:wRTSDuration_aa_f1, 1:2.4G, 1:CCKData
            pBuf->Data.wDurationID = pBuf->wDuration_aa;
            //Get RTS Frame body
            pBuf->Data.wFrameControl = TYPE_CTL_RTS;//0x00B4

	if ((pDevice->eOPMode == OP_MODE_ADHOC) ||
	    (pDevice->eOPMode == OP_MODE_AP)) {
		memcpy(&(pBuf->Data.abyRA[0]),
		       &(psEthHeader->abyDstAddr[0]),
		       ETH_ALEN);
	}
            else {
		memcpy(&(pBuf->Data.abyRA[0]),
		       &(pDevice->abyBSSID[0]),
		       ETH_ALEN);
            }

	if (pDevice->eOPMode == OP_MODE_AP) {
		memcpy(&(pBuf->Data.abyTA[0]),
		       &(pDevice->abyBSSID[0]),
		       ETH_ALEN);
	}
            else {
		    memcpy(&(pBuf->Data.abyTA[0]),
			   &(psEthHeader->abySrcAddr[0]),
			   ETH_ALEN);
            }

        } // if (byFBOption == AUTO_FB_NONE)
    }
    else if (byPktType == PK_TYPE_11A) {
        if (byFBOption == AUTO_FB_NONE) {
            PSRTS_ab pBuf = (PSRTS_ab)pvRTS;
            //Get SignalField,ServiceField,Length
            BBvCaculateParameter(pDevice, uRTSFrameLen, pDevice->byTopOFDMBasicRate, byPktType,
                (PWORD)&(wLen), (PBYTE)&(pBuf->byServiceField), (PBYTE)&(pBuf->bySignalField)
            );
            pBuf->wTransmitLength = cpu_to_le16(wLen);
            //Get Duration
            pBuf->wDuration = cpu_to_le16((WORD)s_uGetRTSCTSDuration(pDevice, RTSDUR_AA, cbFrameLength, byPktType, wCurrentRate, bNeedAck, byFBOption)); //0:RTSDuration_aa, 0:5G, 0: 5G OFDMData
    	    pBuf->Data.wDurationID = pBuf->wDuration;
            //Get RTS Frame body
            pBuf->Data.wFrameControl = TYPE_CTL_RTS;//0x00B4

	if ((pDevice->eOPMode == OP_MODE_ADHOC) ||
	    (pDevice->eOPMode == OP_MODE_AP)) {
		memcpy(&(pBuf->Data.abyRA[0]),
		       &(psEthHeader->abyDstAddr[0]),
		       ETH_ALEN);
	} else {
		memcpy(&(pBuf->Data.abyRA[0]),
		       &(pDevice->abyBSSID[0]),
		       ETH_ALEN);
	}

	if (pDevice->eOPMode == OP_MODE_AP) {
		memcpy(&(pBuf->Data.abyTA[0]),
		       &(pDevice->abyBSSID[0]),
		       ETH_ALEN);
	} else {
		memcpy(&(pBuf->Data.abyTA[0]),
		       &(psEthHeader->abySrcAddr[0]),
		       ETH_ALEN);
	}

        }
        else {
            PSRTS_a_FB pBuf = (PSRTS_a_FB)pvRTS;
            //Get SignalField,ServiceField,Length
            BBvCaculateParameter(pDevice, uRTSFrameLen, pDevice->byTopOFDMBasicRate, byPktType,
                (PWORD)&(wLen), (PBYTE)&(pBuf->byServiceField), (PBYTE)&(pBuf->bySignalField)
            );
            pBuf->wTransmitLength = cpu_to_le16(wLen);
            //Get Duration
            pBuf->wDuration = cpu_to_le16((WORD)s_uGetRTSCTSDuration(pDevice, RTSDUR_AA, cbFrameLength, byPktType, wCurrentRate, bNeedAck, byFBOption)); //0:RTSDuration_aa, 0:5G, 0: 5G OFDMData
    	    pBuf->wRTSDuration_f0 = cpu_to_le16((WORD)s_uGetRTSCTSDuration(pDevice, RTSDUR_AA_F0, cbFrameLength, byPktType, wCurrentRate, bNeedAck, byFBOption)); //5:RTSDuration_aa_f0, 0:5G, 0: 5G OFDMData
    	    pBuf->wRTSDuration_f1 = cpu_to_le16((WORD)s_uGetRTSCTSDuration(pDevice, RTSDUR_AA_F1, cbFrameLength, byPktType, wCurrentRate, bNeedAck, byFBOption)); //7:RTSDuration_aa_f1, 0:5G, 0:
    	    pBuf->Data.wDurationID = pBuf->wDuration;
    	    //Get RTS Frame body
            pBuf->Data.wFrameControl = TYPE_CTL_RTS;//0x00B4

	if ((pDevice->eOPMode == OP_MODE_ADHOC) ||
	    (pDevice->eOPMode == OP_MODE_AP)) {
		memcpy(&(pBuf->Data.abyRA[0]),
		       &(psEthHeader->abyDstAddr[0]),
		       ETH_ALEN);
	} else {
		memcpy(&(pBuf->Data.abyRA[0]),
		       &(pDevice->abyBSSID[0]),
		       ETH_ALEN);
	}
	if (pDevice->eOPMode == OP_MODE_AP) {
		memcpy(&(pBuf->Data.abyTA[0]),
		       &(pDevice->abyBSSID[0]),
		       ETH_ALEN);
	} else {
		memcpy(&(pBuf->Data.abyTA[0]),
		       &(psEthHeader->abySrcAddr[0]),
		       ETH_ALEN);
	}
        }
    }
    else if (byPktType == PK_TYPE_11B) {
        PSRTS_ab pBuf = (PSRTS_ab)pvRTS;
        //Get SignalField,ServiceField,Length
        BBvCaculateParameter(pDevice, uRTSFrameLen, pDevice->byTopCCKBasicRate, PK_TYPE_11B,
            (PWORD)&(wLen), (PBYTE)&(pBuf->byServiceField), (PBYTE)&(pBuf->bySignalField)
        );
        pBuf->wTransmitLength = cpu_to_le16(wLen);
        //Get Duration
        pBuf->wDuration = cpu_to_le16((WORD)s_uGetRTSCTSDuration(pDevice, RTSDUR_BB, cbFrameLength, byPktType, wCurrentRate, bNeedAck, byFBOption)); //0:RTSDuration_bb, 1:2.4G, 1:CCKData
        pBuf->Data.wDurationID = pBuf->wDuration;
        //Get RTS Frame body
        pBuf->Data.wFrameControl = TYPE_CTL_RTS;//0x00B4

	if ((pDevice->eOPMode == OP_MODE_ADHOC) ||
            (pDevice->eOPMode == OP_MODE_AP)) {
		memcpy(&(pBuf->Data.abyRA[0]),
		       &(psEthHeader->abyDstAddr[0]),
		       ETH_ALEN);
        }
        else {
		memcpy(&(pBuf->Data.abyRA[0]),
		       &(pDevice->abyBSSID[0]),
		       ETH_ALEN);
        }

        if (pDevice->eOPMode == OP_MODE_AP) {
		memcpy(&(pBuf->Data.abyTA[0]),
		       &(pDevice->abyBSSID[0]),
		       ETH_ALEN);
	} else {
		memcpy(&(pBuf->Data.abyTA[0]),
		       &(psEthHeader->abySrcAddr[0]),
		       ETH_ALEN);
        }
    }
}

static
void
s_vFillCTSHead (
     PSDevice pDevice,
     unsigned int     uDMAIdx,
     BYTE     byPktType,
     void *pvCTS,
     unsigned int     cbFrameLength,
     BOOL     bNeedAck,
     BOOL     bDisCRC,
     WORD     wCurrentRate,
     BYTE     byFBOption
    )
{
    unsigned int uCTSFrameLen = 14;
    WORD  wLen = 0x0000;

    if (pvCTS == NULL) {
        return;
    }

    if (bDisCRC) {
        // When CRCDIS bit is on, H/W forgot to generate FCS for CTS frame,
        // in this case we need to decrease its length by 4.
        uCTSFrameLen -= 4;
    }

    if (byPktType == PK_TYPE_11GB || byPktType == PK_TYPE_11GA) {
        if (byFBOption != AUTO_FB_NONE && uDMAIdx != TYPE_ATIMDMA && uDMAIdx != TYPE_BEACONDMA) {
            // Auto Fall back
            PSCTS_FB pBuf = (PSCTS_FB)pvCTS;
            //Get SignalField,ServiceField,Length
            BBvCaculateParameter(pDevice, uCTSFrameLen, pDevice->byTopCCKBasicRate, PK_TYPE_11B,
                (PWORD)&(wLen), (PBYTE)&(pBuf->byServiceField_b), (PBYTE)&(pBuf->bySignalField_b)
            );
            pBuf->wTransmitLength_b = cpu_to_le16(wLen);
            pBuf->wDuration_ba = (WORD)s_uGetRTSCTSDuration(pDevice, CTSDUR_BA, cbFrameLength, byPktType, wCurrentRate, bNeedAck, byFBOption); //3:CTSDuration_ba, 1:2.4G, 2,3:2.4G OFDM Data
            pBuf->wDuration_ba += pDevice->wCTSDuration;
            pBuf->wDuration_ba = cpu_to_le16(pBuf->wDuration_ba);
            //Get CTSDuration_ba_f0
            pBuf->wCTSDuration_ba_f0 = (WORD)s_uGetRTSCTSDuration(pDevice, CTSDUR_BA_F0, cbFrameLength, byPktType, wCurrentRate, bNeedAck, byFBOption); //8:CTSDuration_ba_f0, 1:2.4G, 2,3:2.4G OFDM Data
            pBuf->wCTSDuration_ba_f0 += pDevice->wCTSDuration;
            pBuf->wCTSDuration_ba_f0 = cpu_to_le16(pBuf->wCTSDuration_ba_f0);
            //Get CTSDuration_ba_f1
            pBuf->wCTSDuration_ba_f1 = (WORD)s_uGetRTSCTSDuration(pDevice, CTSDUR_BA_F1, cbFrameLength, byPktType, wCurrentRate, bNeedAck, byFBOption); //9:CTSDuration_ba_f1, 1:2.4G, 2,3:2.4G OFDM Data
            pBuf->wCTSDuration_ba_f1 += pDevice->wCTSDuration;
            pBuf->wCTSDuration_ba_f1 = cpu_to_le16(pBuf->wCTSDuration_ba_f1);
            //Get CTS Frame body
            pBuf->Data.wDurationID = pBuf->wDuration_ba;
            pBuf->Data.wFrameControl = TYPE_CTL_CTS;//0x00C4
            pBuf->Data.wReserved = 0x0000;
	memcpy(&(pBuf->Data.abyRA[0]),
	       &(pDevice->abyCurrentNetAddr[0]),
	       ETH_ALEN);
        } else { //if (byFBOption != AUTO_FB_NONE && uDMAIdx != TYPE_ATIMDMA && uDMAIdx != TYPE_BEACONDMA)
            PSCTS pBuf = (PSCTS)pvCTS;
            //Get SignalField,ServiceField,Length
            BBvCaculateParameter(pDevice, uCTSFrameLen, pDevice->byTopCCKBasicRate, PK_TYPE_11B,
                (PWORD)&(wLen), (PBYTE)&(pBuf->byServiceField_b), (PBYTE)&(pBuf->bySignalField_b)
            );
            pBuf->wTransmitLength_b = cpu_to_le16(wLen);
            //Get CTSDuration_ba
            pBuf->wDuration_ba = cpu_to_le16((WORD)s_uGetRTSCTSDuration(pDevice, CTSDUR_BA, cbFrameLength, byPktType, wCurrentRate, bNeedAck, byFBOption)); //3:CTSDuration_ba, 1:2.4G, 2,3:2.4G OFDM Data
            pBuf->wDuration_ba += pDevice->wCTSDuration;
            pBuf->wDuration_ba = cpu_to_le16(pBuf->wDuration_ba);

            //Get CTS Frame body
            pBuf->Data.wDurationID = pBuf->wDuration_ba;
            pBuf->Data.wFrameControl = TYPE_CTL_CTS;//0x00C4
            pBuf->Data.wReserved = 0x0000;
	memcpy(&(pBuf->Data.abyRA[0]),
	       &(pDevice->abyCurrentNetAddr[0]),
	       ETH_ALEN);
        }
    }
=======
static u32 s_uGetTxRsvTime(struct vnt_private *priv, u8 pkt_type,
	u32 frame_length, u16 rate, int need_ack)
{
	u32 data_time, ack_time;

	data_time = BBuGetFrameTime(priv->byPreambleType, pkt_type,
							frame_length, rate);

	if (pkt_type == PK_TYPE_11B)
		ack_time = BBuGetFrameTime(priv->byPreambleType, pkt_type, 14,
						(u16)priv->byTopCCKBasicRate);
	else
		ack_time = BBuGetFrameTime(priv->byPreambleType, pkt_type, 14,
						(u16)priv->byTopOFDMBasicRate);

	if (need_ack)
		return data_time + priv->uSIFS + ack_time;

	return data_time;
}

static u16 vnt_rxtx_rsvtime_le16(struct vnt_private *priv, u8 pkt_type,
	u32 frame_length, u16 rate, int need_ack)
{
	return cpu_to_le16((u16)s_uGetTxRsvTime(priv, pkt_type,
		frame_length, rate, need_ack));
}

//byFreqType: 0=>5GHZ 1=>2.4GHZ
static u16 s_uGetRTSCTSRsvTime(struct vnt_private *priv,
	u8 rsv_type, u8 pkt_type, u32 frame_lenght, u16 current_rate)
{
	u32 rrv_time, rts_time, cts_time, ack_time, data_time;

	rrv_time = rts_time = cts_time = ack_time = data_time = 0;

	data_time = BBuGetFrameTime(priv->byPreambleType, pkt_type,
						frame_lenght, current_rate);

	if (rsv_type == 0) {
		rts_time = BBuGetFrameTime(priv->byPreambleType,
			pkt_type, 20, priv->byTopCCKBasicRate);
		cts_time = ack_time = BBuGetFrameTime(priv->byPreambleType,
			pkt_type, 14, priv->byTopCCKBasicRate);
	} else if (rsv_type == 1) {
		rts_time = BBuGetFrameTime(priv->byPreambleType,
			pkt_type, 20, priv->byTopCCKBasicRate);
		cts_time = BBuGetFrameTime(priv->byPreambleType, pkt_type,
			14, priv->byTopCCKBasicRate);
		ack_time = BBuGetFrameTime(priv->byPreambleType, pkt_type,
			14, priv->byTopOFDMBasicRate);
	} else if (rsv_type == 2) {
		rts_time = BBuGetFrameTime(priv->byPreambleType, pkt_type,
			20, priv->byTopOFDMBasicRate);
		cts_time = ack_time = BBuGetFrameTime(priv->byPreambleType,
			pkt_type, 14, priv->byTopOFDMBasicRate);
	} else if (rsv_type == 3) {
		cts_time = BBuGetFrameTime(priv->byPreambleType, pkt_type,
			14, priv->byTopCCKBasicRate);
		ack_time = BBuGetFrameTime(priv->byPreambleType, pkt_type,
			14, priv->byTopOFDMBasicRate);

		rrv_time = cts_time + ack_time + data_time + 2 * priv->uSIFS;

		return rrv_time;
	}

	rrv_time = rts_time + cts_time + ack_time + data_time + 3 * priv->uSIFS;

	return cpu_to_le16((u16)rrv_time);
}

//byFreqType 0: 5GHz, 1:2.4Ghz
static u16 s_uGetDataDuration(struct vnt_private *pDevice,
					u8 byPktType, int bNeedAck)
{
	u32 uAckTime = 0;

	if (bNeedAck) {
		if (byPktType == PK_TYPE_11B)
			uAckTime = BBuGetFrameTime(pDevice->byPreambleType,
				byPktType, 14, pDevice->byTopCCKBasicRate);
		else
			uAckTime = BBuGetFrameTime(pDevice->byPreambleType,
				byPktType, 14, pDevice->byTopOFDMBasicRate);
		return cpu_to_le16((u16)(pDevice->uSIFS + uAckTime));
	}

	return 0;
}

//byFreqType: 0=>5GHZ 1=>2.4GHZ
static u16 s_uGetRTSCTSDuration(struct vnt_private *pDevice, u8 byDurType,
	u32 cbFrameLength, u8 byPktType, u16 wRate, int bNeedAck,
	u8 byFBOption)
{
	u32 uCTSTime = 0, uDurTime = 0;

	switch (byDurType) {
	case RTSDUR_BB:
	case RTSDUR_BA:
	case RTSDUR_BA_F0:
	case RTSDUR_BA_F1:
		uCTSTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType,
			14, pDevice->byTopCCKBasicRate);
		uDurTime = uCTSTime + 2 * pDevice->uSIFS +
			s_uGetTxRsvTime(pDevice, byPktType,
						cbFrameLength, wRate, bNeedAck);
		break;

	case RTSDUR_AA:
	case RTSDUR_AA_F0:
	case RTSDUR_AA_F1:
		uCTSTime = BBuGetFrameTime(pDevice->byPreambleType, byPktType,
			14, pDevice->byTopOFDMBasicRate);
		uDurTime = uCTSTime + 2 * pDevice->uSIFS +
			s_uGetTxRsvTime(pDevice, byPktType,
						cbFrameLength, wRate, bNeedAck);
		break;

	case CTSDUR_BA:
	case CTSDUR_BA_F0:
	case CTSDUR_BA_F1:
		uDurTime = pDevice->uSIFS + s_uGetTxRsvTime(pDevice,
				byPktType, cbFrameLength, wRate, bNeedAck);
		break;

	default:
		break;
	}

	return cpu_to_le16((u16)uDurTime);
}

static u16 vnt_rxtx_datahead_g(struct vnt_private *priv, u8 pkt_type, u16 rate,
		struct vnt_tx_datahead_g *buf, u32 frame_len, int need_ack)
{
	/* Get SignalField,ServiceField,Length */
	BBvCalculateParameter(priv, frame_len, rate, pkt_type, &buf->a);
	BBvCalculateParameter(priv, frame_len, priv->byTopCCKBasicRate,
							PK_TYPE_11B, &buf->b);

	/* Get Duration and TimeStamp */
	buf->wDuration_a = s_uGetDataDuration(priv, pkt_type, need_ack);
	buf->wDuration_b = s_uGetDataDuration(priv, PK_TYPE_11B, need_ack);

	buf->wTimeStampOff_a = vnt_time_stamp_off(priv, rate);
	buf->wTimeStampOff_b = vnt_time_stamp_off(priv,
					priv->byTopCCKBasicRate);

	return buf->wDuration_a;
}

static u16 vnt_rxtx_datahead_g_fb(struct vnt_private *priv, u8 pkt_type,
		u16 rate, struct vnt_tx_datahead_g_fb *buf,
		u32 frame_len, int need_ack)
{
	/* Get SignalField,ServiceField,Length */
	BBvCalculateParameter(priv, frame_len, rate, pkt_type, &buf->a);

	BBvCalculateParameter(priv, frame_len, priv->byTopCCKBasicRate,
						PK_TYPE_11B, &buf->b);

	/* Get Duration and TimeStamp */
	buf->wDuration_a = s_uGetDataDuration(priv, pkt_type, need_ack);
	buf->wDuration_b = s_uGetDataDuration(priv, PK_TYPE_11B, need_ack);

	buf->wDuration_a_f0 = s_uGetDataDuration(priv, pkt_type, need_ack);
	buf->wDuration_a_f1 = s_uGetDataDuration(priv, pkt_type, need_ack);

	buf->wTimeStampOff_a = vnt_time_stamp_off(priv, rate);
	buf->wTimeStampOff_b = vnt_time_stamp_off(priv,
						priv->byTopCCKBasicRate);

	return buf->wDuration_a;
}

static u16 vnt_rxtx_datahead_a_fb(struct vnt_private *priv, u8 pkt_type,
		u16 rate, struct vnt_tx_datahead_a_fb *buf,
		u32 frame_len, int need_ack)
{
	/* Get SignalField,ServiceField,Length */
	BBvCalculateParameter(priv, frame_len, rate, pkt_type, &buf->a);
	/* Get Duration and TimeStampOff */
	buf->wDuration = s_uGetDataDuration(priv, pkt_type, need_ack);

	buf->wDuration_f0 = s_uGetDataDuration(priv, pkt_type, need_ack);
	buf->wDuration_f1 = s_uGetDataDuration(priv, pkt_type, need_ack);

	buf->wTimeStampOff = vnt_time_stamp_off(priv, rate);

	return buf->wDuration;
}

static u16 vnt_rxtx_datahead_ab(struct vnt_private *priv, u8 pkt_type,
		u16 rate, struct vnt_tx_datahead_ab *buf,
		u32 frame_len, int need_ack)
{
	/* Get SignalField,ServiceField,Length */
	BBvCalculateParameter(priv, frame_len, rate, pkt_type, &buf->ab);
	/* Get Duration and TimeStampOff */
	buf->wDuration = s_uGetDataDuration(priv, pkt_type, need_ack);

	buf->wTimeStampOff = vnt_time_stamp_off(priv, rate);

	return buf->wDuration;
}

static int vnt_fill_ieee80211_rts(struct vnt_private *priv,
	struct ieee80211_rts *rts, struct ethhdr *eth_hdr,
		u16 duration)
{
	rts->duration = duration;
	rts->frame_control = TYPE_CTL_RTS;

	if (priv->eOPMode == OP_MODE_ADHOC || priv->eOPMode == OP_MODE_AP)
		memcpy(rts->ra, eth_hdr->h_dest, ETH_ALEN);
	else
		memcpy(rts->ra, priv->abyBSSID, ETH_ALEN);

	if (priv->eOPMode == OP_MODE_AP)
		memcpy(rts->ta, priv->abyBSSID, ETH_ALEN);
	else
		memcpy(rts->ta, eth_hdr->h_source, ETH_ALEN);

	return 0;
}

static u16 vnt_rxtx_rts_g_head(struct vnt_private *priv,
	struct vnt_rts_g *buf, struct ethhdr *eth_hdr,
	u8 pkt_type, u32 frame_len, int need_ack,
	u16 current_rate, u8 fb_option)
{
	u16 rts_frame_len = 20;

	BBvCalculateParameter(priv, rts_frame_len, priv->byTopCCKBasicRate,
		PK_TYPE_11B, &buf->b);
	BBvCalculateParameter(priv, rts_frame_len,
		priv->byTopOFDMBasicRate, pkt_type, &buf->a);

	buf->wDuration_bb = s_uGetRTSCTSDuration(priv, RTSDUR_BB, frame_len,
		PK_TYPE_11B, priv->byTopCCKBasicRate, need_ack, fb_option);
	buf->wDuration_aa = s_uGetRTSCTSDuration(priv, RTSDUR_AA, frame_len,
		pkt_type, current_rate, need_ack, fb_option);
	buf->wDuration_ba = s_uGetRTSCTSDuration(priv, RTSDUR_BA, frame_len,
		pkt_type, current_rate, need_ack, fb_option);

	vnt_fill_ieee80211_rts(priv, &buf->data, eth_hdr, buf->wDuration_aa);

	return vnt_rxtx_datahead_g(priv, pkt_type, current_rate,
			&buf->data_head, frame_len, need_ack);
}

static u16 vnt_rxtx_rts_g_fb_head(struct vnt_private *priv,
	struct vnt_rts_g_fb *buf, struct ethhdr *eth_hdr,
	u8 pkt_type, u32 frame_len, int need_ack,
	u16 current_rate, u8 fb_option)
{
	u16 rts_frame_len = 20;

	BBvCalculateParameter(priv, rts_frame_len, priv->byTopCCKBasicRate,
		PK_TYPE_11B, &buf->b);
	BBvCalculateParameter(priv, rts_frame_len,
		priv->byTopOFDMBasicRate, pkt_type, &buf->a);


	buf->wDuration_bb = s_uGetRTSCTSDuration(priv, RTSDUR_BB, frame_len,
		PK_TYPE_11B, priv->byTopCCKBasicRate, need_ack, fb_option);
	buf->wDuration_aa = s_uGetRTSCTSDuration(priv, RTSDUR_AA, frame_len,
		pkt_type, current_rate, need_ack, fb_option);
	buf->wDuration_ba = s_uGetRTSCTSDuration(priv, RTSDUR_BA, frame_len,
		pkt_type, current_rate, need_ack, fb_option);


	buf->wRTSDuration_ba_f0 = s_uGetRTSCTSDuration(priv, RTSDUR_BA_F0,
		frame_len, pkt_type, priv->tx_rate_fb0, need_ack, fb_option);
	buf->wRTSDuration_aa_f0 = s_uGetRTSCTSDuration(priv, RTSDUR_AA_F0,
		frame_len, pkt_type, priv->tx_rate_fb0, need_ack, fb_option);
	buf->wRTSDuration_ba_f1 = s_uGetRTSCTSDuration(priv, RTSDUR_BA_F1,
		frame_len, pkt_type, priv->tx_rate_fb1, need_ack, fb_option);
	buf->wRTSDuration_aa_f1 = s_uGetRTSCTSDuration(priv, RTSDUR_AA_F1,
		frame_len, pkt_type, priv->tx_rate_fb1, need_ack, fb_option);

	vnt_fill_ieee80211_rts(priv, &buf->data, eth_hdr, buf->wDuration_aa);

	return vnt_rxtx_datahead_g_fb(priv, pkt_type, current_rate,
			&buf->data_head, frame_len, need_ack);
}

static u16 vnt_rxtx_rts_ab_head(struct vnt_private *priv,
	struct vnt_rts_ab *buf, struct ethhdr *eth_hdr,
	u8 pkt_type, u32 frame_len, int need_ack,
	u16 current_rate, u8 fb_option)
{
	u16 rts_frame_len = 20;

	BBvCalculateParameter(priv, rts_frame_len,
		priv->byTopOFDMBasicRate, pkt_type, &buf->ab);

	buf->wDuration = s_uGetRTSCTSDuration(priv, RTSDUR_AA, frame_len,
		pkt_type, current_rate, need_ack, fb_option);

	vnt_fill_ieee80211_rts(priv, &buf->data, eth_hdr, buf->wDuration);

	return vnt_rxtx_datahead_ab(priv, pkt_type, current_rate,
			&buf->data_head, frame_len, need_ack);
}

static u16 vnt_rxtx_rts_a_fb_head(struct vnt_private *priv,
	struct vnt_rts_a_fb *buf, struct ethhdr *eth_hdr,
	u8 pkt_type, u32 frame_len, int need_ack,
	u16 current_rate, u8 fb_option)
{
	u16 rts_frame_len = 20;

	BBvCalculateParameter(priv, rts_frame_len,
		priv->byTopOFDMBasicRate, pkt_type, &buf->a);

	buf->wDuration = s_uGetRTSCTSDuration(priv, RTSDUR_AA, frame_len,
		pkt_type, current_rate, need_ack, fb_option);

	buf->wRTSDuration_f0 = s_uGetRTSCTSDuration(priv, RTSDUR_AA_F0,
		frame_len, pkt_type, priv->tx_rate_fb0, need_ack, fb_option);

	buf->wRTSDuration_f1 = s_uGetRTSCTSDuration(priv, RTSDUR_AA_F1,
		frame_len, pkt_type, priv->tx_rate_fb1, need_ack, fb_option);

	vnt_fill_ieee80211_rts(priv, &buf->data, eth_hdr, buf->wDuration);

	return vnt_rxtx_datahead_a_fb(priv, pkt_type, current_rate,
			&buf->data_head, frame_len, need_ack);
}

static u16 s_vFillRTSHead(struct vnt_private *pDevice, u8 byPktType,
	union vnt_tx_data_head *head, u32 cbFrameLength, int bNeedAck,
	struct ethhdr *psEthHeader, u16 wCurrentRate, u8 byFBOption)
{

	if (!head)
		return 0;

	/* Note: So far RTSHead doesn't appear in ATIM
	*	& Beacom DMA, so we don't need to take them
	*	into account.
	*	Otherwise, we need to modified codes for them.
	*/
	switch (byPktType) {
	case PK_TYPE_11GB:
	case PK_TYPE_11GA:
		if (byFBOption == AUTO_FB_NONE)
			return vnt_rxtx_rts_g_head(pDevice, &head->rts_g,
				psEthHeader, byPktType, cbFrameLength,
				bNeedAck, wCurrentRate, byFBOption);
		else
			return vnt_rxtx_rts_g_fb_head(pDevice, &head->rts_g_fb,
				psEthHeader, byPktType, cbFrameLength,
				bNeedAck, wCurrentRate, byFBOption);
		break;
	case PK_TYPE_11A:
		if (byFBOption) {
			return vnt_rxtx_rts_a_fb_head(pDevice, &head->rts_a_fb,
				psEthHeader, byPktType, cbFrameLength,
				bNeedAck, wCurrentRate, byFBOption);
			break;
		}
	case PK_TYPE_11B:
		return vnt_rxtx_rts_ab_head(pDevice, &head->rts_ab,
			psEthHeader, byPktType, cbFrameLength,
			bNeedAck, wCurrentRate, byFBOption);
	}

	return 0;
}

static u16 s_vFillCTSHead(struct vnt_private *pDevice, u32 uDMAIdx,
	u8 byPktType, union vnt_tx_data_head *head, u32 cbFrameLength,
	int bNeedAck, u16 wCurrentRate, u8 byFBOption)
{
	u32 uCTSFrameLen = 14;

	if (!head)
		return 0;

	if (byFBOption != AUTO_FB_NONE) {
		/* Auto Fall back */
		struct vnt_cts_fb *pBuf = &head->cts_g_fb;
		/* Get SignalField,ServiceField,Length */
		BBvCalculateParameter(pDevice, uCTSFrameLen,
			pDevice->byTopCCKBasicRate, PK_TYPE_11B, &pBuf->b);
		pBuf->wDuration_ba = s_uGetRTSCTSDuration(pDevice, CTSDUR_BA,
			cbFrameLength, byPktType,
			wCurrentRate, bNeedAck, byFBOption);
		/* Get CTSDuration_ba_f0 */
		pBuf->wCTSDuration_ba_f0 = s_uGetRTSCTSDuration(pDevice,
			CTSDUR_BA_F0, cbFrameLength, byPktType,
			pDevice->tx_rate_fb0, bNeedAck, byFBOption);
		/* Get CTSDuration_ba_f1 */
		pBuf->wCTSDuration_ba_f1 = s_uGetRTSCTSDuration(pDevice,
			CTSDUR_BA_F1, cbFrameLength, byPktType,
			pDevice->tx_rate_fb1, bNeedAck, byFBOption);
		/* Get CTS Frame body */
		pBuf->data.duration = pBuf->wDuration_ba;
		pBuf->data.frame_control = TYPE_CTL_CTS;
		memcpy(pBuf->data.ra, pDevice->abyCurrentNetAddr, ETH_ALEN);

		return vnt_rxtx_datahead_g_fb(pDevice, byPktType, wCurrentRate,
				&pBuf->data_head, cbFrameLength, bNeedAck);
	} else {
		struct vnt_cts *pBuf = &head->cts_g;
		/* Get SignalField,ServiceField,Length */
		BBvCalculateParameter(pDevice, uCTSFrameLen,
			pDevice->byTopCCKBasicRate, PK_TYPE_11B, &pBuf->b);
		/* Get CTSDuration_ba */
		pBuf->wDuration_ba = s_uGetRTSCTSDuration(pDevice,
			CTSDUR_BA, cbFrameLength, byPktType,
			wCurrentRate, bNeedAck, byFBOption);
		/*Get CTS Frame body*/
		pBuf->data.duration = pBuf->wDuration_ba;
		pBuf->data.frame_control = TYPE_CTL_CTS;
		memcpy(pBuf->data.ra, pDevice->abyCurrentNetAddr, ETH_ALEN);

		return vnt_rxtx_datahead_g(pDevice, byPktType, wCurrentRate,
				&pBuf->data_head, cbFrameLength, bNeedAck);
        }

	return 0;
>>>>>>> refs/remotes/origin/master
}

/*+
 *
 * Description:
 *      Generate FIFO control for MAC & Baseband controller
 *
 * Parameters:
 *  In:
 *      pDevice         - Pointer to adpater
 *      pTxDataHead     - Transmit Data Buffer
 *      pTxBufHead      - pTxBufHead
 *      pvRrvTime        - pvRrvTime
 *      pvRTS            - RTS Buffer
 *      pCTS            - CTS Buffer
 *      cbFrameSize     - Transmit Data Length (Hdr+Payload+FCS)
 *      bNeedACK        - If need ACK
 *      uDMAIdx         - DMA Index
 *  Out:
 *      none
 *
 * Return Value: none
 *
-*/

<<<<<<< HEAD
static
void
s_vGenerateTxParameter (
     PSDevice         pDevice,
     BYTE             byPktType,
     WORD             wCurrentRate,
     void *pTxBufHead,
     void *pvRrvTime,
     void *pvRTS,
     void *pvCTS,
     unsigned int             cbFrameSize,
     BOOL             bNeedACK,
     unsigned int             uDMAIdx,
     PSEthernetHeader psEthHeader
    )
{
	unsigned int cbMACHdLen = WLAN_HDR_ADDR3_LEN; /* 24 */
    WORD wFifoCtl;
    BOOL bDisCRC = FALSE;
    BYTE byFBOption = AUTO_FB_NONE;
//    WORD wCurrentRate = pDevice->wCurrentRate;

    //DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"s_vGenerateTxParameter...\n");
    PSTxBufHead pFifoHead = (PSTxBufHead)pTxBufHead;
    pFifoHead->wReserved = wCurrentRate;
    wFifoCtl = pFifoHead->wFIFOCtl;

    if (wFifoCtl & FIFOCTL_CRCDIS) {
        bDisCRC = TRUE;
    }

    if (wFifoCtl & FIFOCTL_AUTO_FB_0) {
        byFBOption = AUTO_FB_0;
    }
    else if (wFifoCtl & FIFOCTL_AUTO_FB_1) {
        byFBOption = AUTO_FB_1;
    }

    if (pDevice->bLongHeader)
        cbMACHdLen = WLAN_HDR_ADDR3_LEN + 6;

    if (byPktType == PK_TYPE_11GB || byPktType == PK_TYPE_11GA) {

        if (pvRTS != NULL) { //RTS_need
            //Fill RsvTime
            if (pvRrvTime) {
                PSRrvTime_gRTS pBuf = (PSRrvTime_gRTS)pvRrvTime;
                pBuf->wRTSTxRrvTime_aa = cpu_to_le16((WORD)s_uGetRTSCTSRsvTime(pDevice, 2, byPktType, cbFrameSize, wCurrentRate));//2:RTSTxRrvTime_aa, 1:2.4GHz
                pBuf->wRTSTxRrvTime_ba = cpu_to_le16((WORD)s_uGetRTSCTSRsvTime(pDevice, 1, byPktType, cbFrameSize, wCurrentRate));//1:RTSTxRrvTime_ba, 1:2.4GHz
                pBuf->wRTSTxRrvTime_bb = cpu_to_le16((WORD)s_uGetRTSCTSRsvTime(pDevice, 0, byPktType, cbFrameSize, wCurrentRate));//0:RTSTxRrvTime_bb, 1:2.4GHz
                pBuf->wTxRrvTime_a = cpu_to_le16((WORD) s_uGetTxRsvTime(pDevice, byPktType, cbFrameSize, wCurrentRate, bNeedACK));//2.4G OFDM
                pBuf->wTxRrvTime_b = cpu_to_le16((WORD) s_uGetTxRsvTime(pDevice, PK_TYPE_11B, cbFrameSize, pDevice->byTopCCKBasicRate, bNeedACK));//1:CCK
            }
            //Fill RTS
            s_vFillRTSHead(pDevice, byPktType, pvRTS, cbFrameSize, bNeedACK, bDisCRC, psEthHeader, wCurrentRate, byFBOption);
        }
        else {//RTS_needless, PCF mode

            //Fill RsvTime
            if (pvRrvTime) {
                PSRrvTime_gCTS pBuf = (PSRrvTime_gCTS)pvRrvTime;
                pBuf->wTxRrvTime_a = cpu_to_le16((WORD)s_uGetTxRsvTime(pDevice, byPktType, cbFrameSize, wCurrentRate, bNeedACK));//2.4G OFDM
                pBuf->wTxRrvTime_b = cpu_to_le16((WORD)s_uGetTxRsvTime(pDevice, PK_TYPE_11B, cbFrameSize, pDevice->byTopCCKBasicRate, bNeedACK));//1:CCK
                pBuf->wCTSTxRrvTime_ba = cpu_to_le16((WORD)s_uGetRTSCTSRsvTime(pDevice, 3, byPktType, cbFrameSize, wCurrentRate));//3:CTSTxRrvTime_Ba, 1:2.4GHz
            }
            //Fill CTS
            s_vFillCTSHead(pDevice, uDMAIdx, byPktType, pvCTS, cbFrameSize, bNeedACK, bDisCRC, wCurrentRate, byFBOption);
        }
    }
    else if (byPktType == PK_TYPE_11A) {

        if (pvRTS != NULL) {//RTS_need, non PCF mode
            //Fill RsvTime
            if (pvRrvTime) {
                PSRrvTime_ab pBuf = (PSRrvTime_ab)pvRrvTime;
                pBuf->wRTSTxRrvTime = cpu_to_le16((WORD)s_uGetRTSCTSRsvTime(pDevice, 2, byPktType, cbFrameSize, wCurrentRate));//2:RTSTxRrvTime_aa, 0:5GHz
                pBuf->wTxRrvTime = cpu_to_le16((WORD)s_uGetTxRsvTime(pDevice, byPktType, cbFrameSize, wCurrentRate, bNeedACK));//0:OFDM
            }
            //Fill RTS
            s_vFillRTSHead(pDevice, byPktType, pvRTS, cbFrameSize, bNeedACK, bDisCRC, psEthHeader, wCurrentRate, byFBOption);
        }
        else if (pvRTS == NULL) {//RTS_needless, non PCF mode
            //Fill RsvTime
            if (pvRrvTime) {
                PSRrvTime_ab pBuf = (PSRrvTime_ab)pvRrvTime;
                pBuf->wTxRrvTime = cpu_to_le16((WORD)s_uGetTxRsvTime(pDevice, PK_TYPE_11A, cbFrameSize, wCurrentRate, bNeedACK)); //0:OFDM
            }
        }
    }
    else if (byPktType == PK_TYPE_11B) {

        if ((pvRTS != NULL)) {//RTS_need, non PCF mode
            //Fill RsvTime
            if (pvRrvTime) {
                PSRrvTime_ab pBuf = (PSRrvTime_ab)pvRrvTime;
                pBuf->wRTSTxRrvTime = cpu_to_le16((WORD)s_uGetRTSCTSRsvTime(pDevice, 0, byPktType, cbFrameSize, wCurrentRate));//0:RTSTxRrvTime_bb, 1:2.4GHz
                pBuf->wTxRrvTime = cpu_to_le16((WORD)s_uGetTxRsvTime(pDevice, PK_TYPE_11B, cbFrameSize, wCurrentRate, bNeedACK));//1:CCK
            }
            //Fill RTS
            s_vFillRTSHead(pDevice, byPktType, pvRTS, cbFrameSize, bNeedACK, bDisCRC, psEthHeader, wCurrentRate, byFBOption);
        }
        else { //RTS_needless, non PCF mode
            //Fill RsvTime
            if (pvRrvTime) {
                PSRrvTime_ab pBuf = (PSRrvTime_ab)pvRrvTime;
                pBuf->wTxRrvTime = cpu_to_le16((WORD)s_uGetTxRsvTime(pDevice, PK_TYPE_11B, cbFrameSize, wCurrentRate, bNeedACK)); //1:CCK
            }
        }
    }
    //DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"s_vGenerateTxParameter END.\n");
}
/*
    PBYTE pbyBuffer,//point to pTxBufHead
    WORD  wFragType,//00:Non-Frag, 01:Start, 02:Mid, 03:Last
    unsigned int  cbFragmentSize,//Hdr+payoad+FCS
*/


BOOL
s_bPacketToWirelessUsb(
      PSDevice         pDevice,
      BYTE             byPktType,
      PBYTE            usbPacketBuf,
      BOOL             bNeedEncryption,
      unsigned int             uSkbPacketLen,
      unsigned int             uDMAIdx,
      PSEthernetHeader psEthHeader,
      PBYTE            pPacket,
      PSKeyItem        pTransmitKey,
      unsigned int             uNodeIndex,
      WORD             wCurrentRate,
     unsigned int             *pcbHeaderLen,
     unsigned int             *pcbTotalLen
    )
{
    PSMgmtObject        pMgmt = &(pDevice->sMgmtObj);
    unsigned int cbFrameSize, cbFrameBodySize;
    PTX_BUFFER          pTxBufHead;
    unsigned int cb802_1_H_len;
    unsigned int cbIVlen = 0, cbICVlen = 0, cbMIClen = 0,
	    cbMACHdLen = 0, cbFCSlen = 4;
    unsigned int cbMICHDR = 0;
    BOOL                bNeedACK,bRTS;
    PBYTE               pbyType,pbyMacHdr,pbyIVHead,pbyPayloadHead,pbyTxBufferAddr;
    BYTE abySNAP_RFC1042[ETH_ALEN] = {0xAA, 0xAA, 0x03, 0x00, 0x00, 0x00};
    BYTE abySNAP_Bridgetunnel[ETH_ALEN] = {0xAA, 0xAA, 0x03, 0x00, 0x00, 0xF8};
    unsigned int uDuration;
    unsigned int cbHeaderLength = 0, uPadding = 0;
    void *pvRrvTime;
    PSMICHDRHead        pMICHDR;
    void *pvRTS;
    void *pvCTS;
    void *pvTxDataHd;
    BYTE                byFBOption = AUTO_FB_NONE,byFragType;
    WORD                wTxBufSize;
    DWORD               dwMICKey0,dwMICKey1,dwMIC_Priority,dwCRC;
    PDWORD              pdwMIC_L,pdwMIC_R;
    BOOL                bSoftWEP = FALSE;




    pvRrvTime = pMICHDR = pvRTS = pvCTS = pvTxDataHd = NULL;
    if ((bNeedEncryption) && (pTransmitKey != NULL))  {
        if (((PSKeyTable) (pTransmitKey->pvKeyTable))->bSoftWEP == TRUE) {
            // WEP 256
            bSoftWEP = TRUE;
        }
    }

    pTxBufHead = (PTX_BUFFER) usbPacketBuf;
    memset(pTxBufHead, 0, sizeof(TX_BUFFER));

    // Get pkt type
    if (ntohs(psEthHeader->wType) > ETH_DATA_LEN) {
        if (pDevice->dwDiagRefCount == 0) {
            cb802_1_H_len = 8;
        } else {
            cb802_1_H_len = 2;
        }
    } else {
        cb802_1_H_len = 0;
    }
=======
static u16 s_vGenerateTxParameter(struct vnt_private *pDevice,
	u8 byPktType, u16 wCurrentRate,	struct vnt_tx_buffer *tx_buffer,
	struct vnt_mic_hdr **mic_hdr, u32 need_mic, u32 cbFrameSize,
	int bNeedACK, u32 uDMAIdx, struct ethhdr *psEthHeader, bool need_rts)
{
	struct vnt_tx_fifo_head *pFifoHead = &tx_buffer->fifo_head;
	union vnt_tx_data_head *head = NULL;
	u16 wFifoCtl;
	u8 byFBOption = AUTO_FB_NONE;

	pFifoHead->wReserved = wCurrentRate;
	wFifoCtl = pFifoHead->wFIFOCtl;

	if (wFifoCtl & FIFOCTL_AUTO_FB_0)
		byFBOption = AUTO_FB_0;
	else if (wFifoCtl & FIFOCTL_AUTO_FB_1)
		byFBOption = AUTO_FB_1;

	if (!pFifoHead)
		return 0;

	if (byPktType == PK_TYPE_11GB || byPktType == PK_TYPE_11GA) {
		if (need_rts) {
			struct vnt_rrv_time_rts *pBuf =
					&tx_buffer->tx_head.tx_rts.rts;

			pBuf->wRTSTxRrvTime_aa = s_uGetRTSCTSRsvTime(pDevice, 2,
					byPktType, cbFrameSize, wCurrentRate);
			pBuf->wRTSTxRrvTime_ba = s_uGetRTSCTSRsvTime(pDevice, 1,
					byPktType, cbFrameSize, wCurrentRate);
			pBuf->wRTSTxRrvTime_bb = s_uGetRTSCTSRsvTime(pDevice, 0,
				byPktType, cbFrameSize, wCurrentRate);

			pBuf->wTxRrvTime_a = vnt_rxtx_rsvtime_le16(pDevice,
				byPktType, cbFrameSize, wCurrentRate, bNeedACK);
			pBuf->wTxRrvTime_b = vnt_rxtx_rsvtime_le16(pDevice,
					PK_TYPE_11B, cbFrameSize,
					pDevice->byTopCCKBasicRate, bNeedACK);

			if (need_mic) {
				*mic_hdr = &tx_buffer->
						tx_head.tx_rts.tx.mic.hdr;
				head = &tx_buffer->tx_head.tx_rts.tx.mic.head;
			} else {
				head = &tx_buffer->tx_head.tx_rts.tx.head;
			}

			/* Fill RTS */
			return s_vFillRTSHead(pDevice, byPktType, head,
					cbFrameSize, bNeedACK, psEthHeader,
						wCurrentRate, byFBOption);

		} else {
			struct vnt_rrv_time_cts *pBuf = &tx_buffer->
							tx_head.tx_cts.cts;

			pBuf->wTxRrvTime_a = vnt_rxtx_rsvtime_le16(pDevice,
				byPktType, cbFrameSize, wCurrentRate, bNeedACK);
			pBuf->wTxRrvTime_b = vnt_rxtx_rsvtime_le16(pDevice,
				PK_TYPE_11B, cbFrameSize,
					pDevice->byTopCCKBasicRate, bNeedACK);

			pBuf->wCTSTxRrvTime_ba = s_uGetRTSCTSRsvTime(pDevice, 3,
					byPktType, cbFrameSize, wCurrentRate);

			if (need_mic) {
				*mic_hdr = &tx_buffer->
						tx_head.tx_cts.tx.mic.hdr;
				head = &tx_buffer->tx_head.tx_cts.tx.mic.head;
			} else {
				head = &tx_buffer->tx_head.tx_cts.tx.head;
			}

			/* Fill CTS */
			return s_vFillCTSHead(pDevice, uDMAIdx, byPktType,
				head, cbFrameSize, bNeedACK, wCurrentRate,
					byFBOption);
		}
	} else if (byPktType == PK_TYPE_11A) {
		if (need_mic) {
			*mic_hdr = &tx_buffer->tx_head.tx_ab.tx.mic.hdr;
			head = &tx_buffer->tx_head.tx_ab.tx.mic.head;
		} else {
			head = &tx_buffer->tx_head.tx_ab.tx.head;
		}

		if (need_rts) {
			struct vnt_rrv_time_ab *pBuf = &tx_buffer->
							tx_head.tx_ab.ab;

			pBuf->wRTSTxRrvTime = s_uGetRTSCTSRsvTime(pDevice, 2,
				byPktType, cbFrameSize, wCurrentRate);

			pBuf->wTxRrvTime = vnt_rxtx_rsvtime_le16(pDevice,
				byPktType, cbFrameSize, wCurrentRate, bNeedACK);

			/* Fill RTS */
			return s_vFillRTSHead(pDevice, byPktType, head,
				cbFrameSize, bNeedACK, psEthHeader,
					wCurrentRate, byFBOption);
		} else {
			struct vnt_rrv_time_ab *pBuf = &tx_buffer->
							tx_head.tx_ab.ab;

			pBuf->wTxRrvTime = vnt_rxtx_rsvtime_le16(pDevice,
				PK_TYPE_11A, cbFrameSize,
					wCurrentRate, bNeedACK);

			return vnt_rxtx_datahead_a_fb(pDevice, byPktType,
				wCurrentRate, &head->data_head_a_fb,
						cbFrameSize, bNeedACK);
		}
	} else if (byPktType == PK_TYPE_11B) {
		if (need_mic) {
			*mic_hdr = &tx_buffer->tx_head.tx_ab.tx.mic.hdr;
			head = &tx_buffer->tx_head.tx_ab.tx.mic.head;
		} else {
			head = &tx_buffer->tx_head.tx_ab.tx.head;
		}

		if (need_rts) {
			struct vnt_rrv_time_ab *pBuf = &tx_buffer->
							tx_head.tx_ab.ab;

			pBuf->wRTSTxRrvTime = s_uGetRTSCTSRsvTime(pDevice, 0,
				byPktType, cbFrameSize, wCurrentRate);

			pBuf->wTxRrvTime = vnt_rxtx_rsvtime_le16(pDevice,
				PK_TYPE_11B, cbFrameSize, wCurrentRate,
								bNeedACK);

			/* Fill RTS */
			return s_vFillRTSHead(pDevice, byPktType, head,
				cbFrameSize,
			bNeedACK, psEthHeader, wCurrentRate, byFBOption);
		} else {
			struct vnt_rrv_time_ab *pBuf = &tx_buffer->
							tx_head.tx_ab.ab;

			pBuf->wTxRrvTime = vnt_rxtx_rsvtime_le16(pDevice,
				PK_TYPE_11B, cbFrameSize,
					wCurrentRate, bNeedACK);

			return vnt_rxtx_datahead_ab(pDevice, byPktType,
				wCurrentRate, &head->data_head_ab,
					cbFrameSize, bNeedACK);
		}
	}

	return 0;
}
/*
    u8 * pbyBuffer,//point to pTxBufHead
    u16  wFragType,//00:Non-Frag, 01:Start, 02:Mid, 03:Last
    unsigned int  cbFragmentSize,//Hdr+payoad+FCS
*/

static int s_bPacketToWirelessUsb(struct vnt_private *pDevice, u8 byPktType,
	struct vnt_tx_buffer *tx_buffer, int bNeedEncryption,
	u32 uSkbPacketLen, u32 uDMAIdx,	struct ethhdr *psEthHeader,
	u8 *pPacket, PSKeyItem pTransmitKey, u32 uNodeIndex, u16 wCurrentRate,
	u32 *pcbHeaderLen, u32 *pcbTotalLen)
{
	struct vnt_tx_fifo_head *pTxBufHead = &tx_buffer->fifo_head;
	struct vnt_manager *pMgmt = &pDevice->vnt_mgmt;
	u32 cbFrameSize, cbFrameBodySize;
	u32 cb802_1_H_len;
	u32 cbIVlen = 0, cbICVlen = 0, cbMIClen = 0, cbMACHdLen = 0;
	u32 cbFCSlen = 4, cbMICHDR = 0;
	int bNeedACK;
	bool bRTS = false;
	u8 *pbyType, *pbyMacHdr, *pbyIVHead, *pbyPayloadHead, *pbyTxBufferAddr;
	u8 abySNAP_RFC1042[ETH_ALEN] = {0xAA, 0xAA, 0x03, 0x00, 0x00, 0x00};
	u8 abySNAP_Bridgetunnel[ETH_ALEN]
		= {0xAA, 0xAA, 0x03, 0x00, 0x00, 0xF8};
	u32 uDuration;
	u32 cbHeaderLength = 0, uPadding = 0;
	struct vnt_mic_hdr *pMICHDR;
	u8 byFBOption = AUTO_FB_NONE, byFragType;
	u16 wTxBufSize;
	u32 dwMICKey0, dwMICKey1, dwMIC_Priority;
	u32 *pdwMIC_L, *pdwMIC_R;
	int bSoftWEP = false;

	pMICHDR = NULL;

	if (bNeedEncryption && pTransmitKey->pvKeyTable) {
		if (((PSKeyTable)pTransmitKey->pvKeyTable)->bSoftWEP == true)
			bSoftWEP = true; /* WEP 256 */
	}

	/* Get pkt type */
	if (ntohs(psEthHeader->h_proto) > ETH_DATA_LEN)
		cb802_1_H_len = 8;
	else
		cb802_1_H_len = 0;
>>>>>>> refs/remotes/origin/master

    cbFrameBodySize = uSkbPacketLen - ETH_HLEN + cb802_1_H_len;

    //Set packet type
<<<<<<< HEAD
    pTxBufHead->wFIFOCtl |= (WORD)(byPktType<<8);

    if (pDevice->dwDiagRefCount != 0) {
        bNeedACK = FALSE;
        pTxBufHead->wFIFOCtl = pTxBufHead->wFIFOCtl & (~FIFOCTL_NEEDACK);
    } else { //if (pDevice->dwDiagRefCount != 0) {
	if ((pDevice->eOPMode == OP_MODE_ADHOC) ||
	    (pDevice->eOPMode == OP_MODE_AP)) {
		if (is_multicast_ether_addr(psEthHeader->abyDstAddr)) {
			bNeedACK = FALSE;
			pTxBufHead->wFIFOCtl =
				pTxBufHead->wFIFOCtl & (~FIFOCTL_NEEDACK);
		} else {
			bNeedACK = TRUE;
			pTxBufHead->wFIFOCtl |= FIFOCTL_NEEDACK;
		}
        }
        else {
            // MSDUs in Infra mode always need ACK
            bNeedACK = TRUE;
            pTxBufHead->wFIFOCtl |= FIFOCTL_NEEDACK;
        }
    } //if (pDevice->dwDiagRefCount != 0) {

    pTxBufHead->wTimeStamp = DEFAULT_MSDU_LIFETIME_RES_64us;

    //Set FIFOCTL_LHEAD
    if (pDevice->bLongHeader)
        pTxBufHead->wFIFOCtl |= FIFOCTL_LHEAD;

    if (pDevice->bSoftwareGenCrcErr) {
        pTxBufHead->wFIFOCtl |= FIFOCTL_CRCDIS; // set tx descriptors to NO hardware CRC
    }

    //Set FRAGCTL_MACHDCNT
    if (pDevice->bLongHeader) {
        cbMACHdLen = WLAN_HDR_ADDR3_LEN + 6;
    } else {
        cbMACHdLen = WLAN_HDR_ADDR3_LEN;
    }
    pTxBufHead->wFragCtl |= (WORD)(cbMACHdLen << 10);

    //Set FIFOCTL_GrpAckPolicy
    if (pDevice->bGrpAckPolicy == TRUE) {//0000 0100 0000 0000
        pTxBufHead->wFIFOCtl |=	FIFOCTL_GRPACK;
    }

    //Set Auto Fallback Ctl
    if (wCurrentRate >= RATE_18M) {
        if (pDevice->byAutoFBCtrl == AUTO_FB_0) {
            pTxBufHead->wFIFOCtl |= FIFOCTL_AUTO_FB_0;
            byFBOption = AUTO_FB_0;
        } else if (pDevice->byAutoFBCtrl == AUTO_FB_1) {
            pTxBufHead->wFIFOCtl |= FIFOCTL_AUTO_FB_1;
            byFBOption = AUTO_FB_1;
        }
    }

    if (bSoftWEP != TRUE) {
=======
    pTxBufHead->wFIFOCtl |= (u16)(byPktType<<8);

	if ((pDevice->eOPMode == OP_MODE_ADHOC) ||
			(pDevice->eOPMode == OP_MODE_AP)) {
		if (is_multicast_ether_addr(psEthHeader->h_dest)) {
			bNeedACK = false;
			pTxBufHead->wFIFOCtl =
				pTxBufHead->wFIFOCtl & (~FIFOCTL_NEEDACK);
		} else {
			bNeedACK = true;
			pTxBufHead->wFIFOCtl |= FIFOCTL_NEEDACK;
		}
	} else {
		/* MSDUs in Infra mode always need ACK */
		bNeedACK = true;
		pTxBufHead->wFIFOCtl |= FIFOCTL_NEEDACK;
	}

    pTxBufHead->wTimeStamp = DEFAULT_MSDU_LIFETIME_RES_64us;

    //Set FRAGCTL_MACHDCNT
	cbMACHdLen = WLAN_HDR_ADDR3_LEN;

    pTxBufHead->wFragCtl |= (u16)(cbMACHdLen << 10);

    //Set FIFOCTL_GrpAckPolicy
    if (pDevice->bGrpAckPolicy == true) {//0000 0100 0000 0000
        pTxBufHead->wFIFOCtl |=	FIFOCTL_GRPACK;
    }

	/* Set Auto Fallback Ctl */
	if (wCurrentRate >= RATE_18M) {
		if (pDevice->byAutoFBCtrl == AUTO_FB_0) {
			pTxBufHead->wFIFOCtl |= FIFOCTL_AUTO_FB_0;

			pDevice->tx_rate_fb0 =
				wFB_Opt0[FB_RATE0][wCurrentRate - RATE_18M];
			pDevice->tx_rate_fb1 =
				wFB_Opt0[FB_RATE1][wCurrentRate - RATE_18M];

			byFBOption = AUTO_FB_0;
		} else if (pDevice->byAutoFBCtrl == AUTO_FB_1) {
			pTxBufHead->wFIFOCtl |= FIFOCTL_AUTO_FB_1;
			pDevice->tx_rate_fb0 =
				wFB_Opt1[FB_RATE0][wCurrentRate - RATE_18M];
			pDevice->tx_rate_fb1 =
				wFB_Opt1[FB_RATE1][wCurrentRate - RATE_18M];

			byFBOption = AUTO_FB_1;
		}
	}

    if (bSoftWEP != true) {
>>>>>>> refs/remotes/origin/master
        if ((bNeedEncryption) && (pTransmitKey != NULL))  { //WEP enabled
            if (pTransmitKey->byCipherSuite == KEY_CTL_WEP) { //WEP40 or WEP104
                pTxBufHead->wFragCtl |= FRAGCTL_LEGACY;
            }
            if (pTransmitKey->byCipherSuite == KEY_CTL_TKIP) {
                DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Tx Set wFragCtl == FRAGCTL_TKIP\n");
                pTxBufHead->wFragCtl |= FRAGCTL_TKIP;
            }
            else if (pTransmitKey->byCipherSuite == KEY_CTL_CCMP) { //CCMP
                pTxBufHead->wFragCtl |= FRAGCTL_AES;
            }
        }
    }

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
    if ((bNeedEncryption) && (pTransmitKey != NULL))  {
        if (pTransmitKey->byCipherSuite == KEY_CTL_WEP) {
            cbIVlen = 4;
            cbICVlen = 4;
        }
        else if (pTransmitKey->byCipherSuite == KEY_CTL_TKIP) {
            cbIVlen = 8;//IV+ExtIV
            cbMIClen = 8;
            cbICVlen = 4;
        }
        if (pTransmitKey->byCipherSuite == KEY_CTL_CCMP) {
            cbIVlen = 8;//RSN Header
            cbICVlen = 8;//MIC
<<<<<<< HEAD
            cbMICHDR = sizeof(SMICHDRHead);
        }
        if (bSoftWEP == FALSE) {
=======
	    cbMICHDR = sizeof(struct vnt_mic_hdr);
        }
        if (bSoftWEP == false) {
>>>>>>> refs/remotes/origin/master
            //MAC Header should be padding 0 to DW alignment.
            uPadding = 4 - (cbMACHdLen%4);
            uPadding %= 4;
        }
    }

    cbFrameSize = cbMACHdLen + cbIVlen + (cbFrameBodySize + cbMIClen) + cbICVlen + cbFCSlen;

<<<<<<< HEAD
    if ( (bNeedACK == FALSE) ||(cbFrameSize < pDevice->wRTSThreshold) ) {
        bRTS = FALSE;
    } else {
        bRTS = TRUE;
        pTxBufHead->wFIFOCtl |= (FIFOCTL_RTS | FIFOCTL_LRETRY);
    }

    pbyTxBufferAddr = (PBYTE) &(pTxBufHead->adwTxKey[0]);
    wTxBufSize = sizeof(STxBufHead);
    if (byPktType == PK_TYPE_11GB || byPktType == PK_TYPE_11GA) {//802.11g packet
        if (byFBOption == AUTO_FB_NONE) {
            if (bRTS == TRUE) {//RTS_need
                pvRrvTime = (PSRrvTime_gRTS) (pbyTxBufferAddr + wTxBufSize);
                pMICHDR = (PSMICHDRHead) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_gRTS));
                pvRTS = (PSRTS_g) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_gRTS) + cbMICHDR);
                pvCTS = NULL;
                pvTxDataHd = (PSTxDataHead_g) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_gRTS) + cbMICHDR + sizeof(SRTS_g));
                cbHeaderLength = wTxBufSize + sizeof(SRrvTime_gRTS) + cbMICHDR + sizeof(SRTS_g) + sizeof(STxDataHead_g);
            }
            else { //RTS_needless
                pvRrvTime = (PSRrvTime_gCTS) (pbyTxBufferAddr + wTxBufSize);
                pMICHDR = (PSMICHDRHead) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_gCTS));
                pvRTS = NULL;
                pvCTS = (PSCTS) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_gCTS) + cbMICHDR);
                pvTxDataHd = (PSTxDataHead_g) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_gCTS) + cbMICHDR + sizeof(SCTS));
                cbHeaderLength = wTxBufSize + sizeof(SRrvTime_gCTS) + cbMICHDR + sizeof(SCTS) + sizeof(STxDataHead_g);
            }
        } else {
            // Auto Fall Back
            if (bRTS == TRUE) {//RTS_need
                pvRrvTime = (PSRrvTime_gRTS) (pbyTxBufferAddr + wTxBufSize);
                pMICHDR = (PSMICHDRHead) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_gRTS));
                pvRTS = (PSRTS_g_FB) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_gRTS) + cbMICHDR);
                pvCTS = NULL;
                pvTxDataHd = (PSTxDataHead_g_FB) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_gRTS) + cbMICHDR + sizeof(SRTS_g_FB));
                cbHeaderLength = wTxBufSize + sizeof(SRrvTime_gRTS) + cbMICHDR + sizeof(SRTS_g_FB) + sizeof(STxDataHead_g_FB);
            }
            else if (bRTS == FALSE) { //RTS_needless
                pvRrvTime = (PSRrvTime_gCTS) (pbyTxBufferAddr + wTxBufSize);
                pMICHDR = (PSMICHDRHead) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_gCTS));
                pvRTS = NULL;
                pvCTS = (PSCTS_FB) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_gCTS) + cbMICHDR);
                pvTxDataHd = (PSTxDataHead_g_FB) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_gCTS) + cbMICHDR + sizeof(SCTS_FB));
                cbHeaderLength = wTxBufSize + sizeof(SRrvTime_gCTS) + cbMICHDR + sizeof(SCTS_FB) + sizeof(STxDataHead_g_FB);
=======
    if ( (bNeedACK == false) ||(cbFrameSize < pDevice->wRTSThreshold) ) {
        bRTS = false;
    } else {
        bRTS = true;
        pTxBufHead->wFIFOCtl |= (FIFOCTL_RTS | FIFOCTL_LRETRY);
    }

    pbyTxBufferAddr = (u8 *) &(pTxBufHead->adwTxKey[0]);
	wTxBufSize = sizeof(struct vnt_tx_fifo_head);

    if (byPktType == PK_TYPE_11GB || byPktType == PK_TYPE_11GA) {//802.11g packet
        if (byFBOption == AUTO_FB_NONE) {
            if (bRTS == true) {//RTS_need
		cbHeaderLength = wTxBufSize + sizeof(struct vnt_rrv_time_rts) +
			cbMICHDR + sizeof(struct vnt_rts_g);
            }
            else { //RTS_needless
		cbHeaderLength = wTxBufSize + sizeof(struct vnt_rrv_time_cts) +
			cbMICHDR + sizeof(struct vnt_cts);
            }
        } else {
            // Auto Fall Back
            if (bRTS == true) {//RTS_need
		cbHeaderLength = wTxBufSize + sizeof(struct vnt_rrv_time_rts) +
			cbMICHDR + sizeof(struct vnt_rts_g_fb);
            }
            else if (bRTS == false) { //RTS_needless
		cbHeaderLength = wTxBufSize + sizeof(struct vnt_rrv_time_cts) +
				cbMICHDR + sizeof(struct vnt_cts_fb);
>>>>>>> refs/remotes/origin/master
            }
        } // Auto Fall Back
    }
    else {//802.11a/b packet
        if (byFBOption == AUTO_FB_NONE) {
<<<<<<< HEAD
            if (bRTS == TRUE) {//RTS_need
                pvRrvTime = (PSRrvTime_ab) (pbyTxBufferAddr + wTxBufSize);
                pMICHDR = (PSMICHDRHead) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_ab));
                pvRTS = (PSRTS_ab) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_ab) + cbMICHDR);
                pvCTS = NULL;
                pvTxDataHd = (PSTxDataHead_ab) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_ab) + cbMICHDR + sizeof(SRTS_ab));
                cbHeaderLength = wTxBufSize + sizeof(PSRrvTime_ab) + cbMICHDR + sizeof(SRTS_ab) + sizeof(STxDataHead_ab);
            }
            else if (bRTS == FALSE) { //RTS_needless, no MICHDR
                pvRrvTime = (PSRrvTime_ab) (pbyTxBufferAddr + wTxBufSize);
                pMICHDR = (PSMICHDRHead) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_ab));
                pvRTS = NULL;
                pvCTS = NULL;
                pvTxDataHd = (PSTxDataHead_ab) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_ab) + cbMICHDR);
                cbHeaderLength = wTxBufSize + sizeof(SRrvTime_ab) + cbMICHDR + sizeof(STxDataHead_ab);
            }
        } else {
            // Auto Fall Back
            if (bRTS == TRUE) {//RTS_need
                pvRrvTime = (PSRrvTime_ab) (pbyTxBufferAddr + wTxBufSize);
                pMICHDR = (PSMICHDRHead) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_ab));
                pvRTS = (PSRTS_a_FB) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_ab) + cbMICHDR);
                pvCTS = NULL;
                pvTxDataHd = (PSTxDataHead_a_FB) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_ab) + cbMICHDR + sizeof(SRTS_a_FB));
                cbHeaderLength = wTxBufSize + sizeof(PSRrvTime_ab) + cbMICHDR + sizeof(SRTS_a_FB) + sizeof(STxDataHead_a_FB);
            }
            else if (bRTS == FALSE) { //RTS_needless
                pvRrvTime = (PSRrvTime_ab) (pbyTxBufferAddr + wTxBufSize);
                pMICHDR = (PSMICHDRHead) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_ab));
                pvRTS = NULL;
                pvCTS = NULL;
                pvTxDataHd = (PSTxDataHead_a_FB) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_ab) + cbMICHDR);
                cbHeaderLength = wTxBufSize + sizeof(SRrvTime_ab) + cbMICHDR + sizeof(STxDataHead_a_FB);
=======
            if (bRTS == true) {//RTS_need
		cbHeaderLength = wTxBufSize + sizeof(struct vnt_rrv_time_ab) +
			cbMICHDR + sizeof(struct vnt_rts_ab);
            }
            else if (bRTS == false) { //RTS_needless, no MICHDR
		cbHeaderLength = wTxBufSize + sizeof(struct vnt_rrv_time_ab) +
				cbMICHDR + sizeof(struct vnt_tx_datahead_ab);
            }
        } else {
            // Auto Fall Back
            if (bRTS == true) {//RTS_need
		cbHeaderLength = wTxBufSize + sizeof(struct vnt_rrv_time_ab) +
			cbMICHDR + sizeof(struct vnt_rts_a_fb);
            }
            else if (bRTS == false) { //RTS_needless
		cbHeaderLength = wTxBufSize + sizeof(struct vnt_rrv_time_ab) +
			cbMICHDR + sizeof(struct vnt_tx_datahead_a_fb);
>>>>>>> refs/remotes/origin/master
            }
        } // Auto Fall Back
    }

<<<<<<< HEAD
    pbyMacHdr = (PBYTE)(pbyTxBufferAddr + cbHeaderLength);
    pbyIVHead = (PBYTE)(pbyMacHdr + cbMACHdLen + uPadding);
    pbyPayloadHead = (PBYTE)(pbyMacHdr + cbMACHdLen + uPadding + cbIVlen);

=======
    pbyMacHdr = (u8 *)(pbyTxBufferAddr + cbHeaderLength);
    pbyIVHead = (u8 *)(pbyMacHdr + cbMACHdLen + uPadding);
    pbyPayloadHead = (u8 *)(pbyMacHdr + cbMACHdLen + uPadding + cbIVlen);
>>>>>>> refs/remotes/origin/master

    //=========================
    //    No Fragmentation
    //=========================
    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"No Fragmentation...\n");
    byFragType = FRAGCTL_NONFRAG;
    //uDMAIdx = TYPE_AC0DMA;
    //pTxBufHead = (PSTxBufHead) &(pTxBufHead->adwTxKey[0]);

<<<<<<< HEAD

    //Fill FIFO,RrvTime,RTS,and CTS
    s_vGenerateTxParameter(pDevice, byPktType, wCurrentRate,
			   (void *)pbyTxBufferAddr, pvRrvTime, pvRTS, pvCTS,
                               cbFrameSize, bNeedACK, uDMAIdx, psEthHeader);
    //Fill DataHead
    uDuration = s_uFillDataHead(pDevice, byPktType, wCurrentRate, pvTxDataHd, cbFrameSize, uDMAIdx, bNeedACK,
                                    0, 0, 1/*uMACfragNum*/, byFBOption);
    // Generate TX MAC Header
    s_vGenerateMACHeader(pDevice, pbyMacHdr, (WORD)uDuration, psEthHeader, bNeedEncryption,
                           byFragType, uDMAIdx, 0);

    if (bNeedEncryption == TRUE) {
        //Fill TXKEY
        s_vFillTxKey(pDevice, (PBYTE)(pTxBufHead->adwTxKey), pbyIVHead, pTransmitKey,
                         pbyMacHdr, (WORD)cbFrameBodySize, (PBYTE)pMICHDR);
=======
	/* Fill FIFO, RrvTime, RTS and CTS */
	uDuration = s_vGenerateTxParameter(pDevice, byPktType, wCurrentRate,
			tx_buffer, &pMICHDR, cbMICHDR,
			cbFrameSize, bNeedACK, uDMAIdx, psEthHeader, bRTS);

    // Generate TX MAC Header
    s_vGenerateMACHeader(pDevice, pbyMacHdr, (u16)uDuration, psEthHeader, bNeedEncryption,
                           byFragType, uDMAIdx, 0);

    if (bNeedEncryption == true) {
        //Fill TXKEY
	s_vFillTxKey(pDevice, pTxBufHead, pbyIVHead, pTransmitKey,
		pbyMacHdr, (u16)cbFrameBodySize, pMICHDR);
>>>>>>> refs/remotes/origin/master

        if (pDevice->bEnableHostWEP) {
            pMgmt->sNodeDBTable[uNodeIndex].dwTSC47_16 = pTransmitKey->dwTSC47_16;
            pMgmt->sNodeDBTable[uNodeIndex].wTSC15_0 = pTransmitKey->wTSC15_0;
        }
    }

<<<<<<< HEAD
    // 802.1H
    if (ntohs(psEthHeader->wType) > ETH_DATA_LEN) {
	if (pDevice->dwDiagRefCount == 0) {
		if ((psEthHeader->wType == cpu_to_be16(ETH_P_IPX)) ||
		    (psEthHeader->wType == cpu_to_le16(0xF380))) {
			memcpy((PBYTE) (pbyPayloadHead),
			       abySNAP_Bridgetunnel, 6);
            } else {
                memcpy((PBYTE) (pbyPayloadHead), &abySNAP_RFC1042[0], 6);
            }
            pbyType = (PBYTE) (pbyPayloadHead + 6);
            memcpy(pbyType, &(psEthHeader->wType), sizeof(WORD));
        } else {
            memcpy((PBYTE) (pbyPayloadHead), &(psEthHeader->wType), sizeof(WORD));

        }

    }

=======
	/* 802.1H */
	if (ntohs(psEthHeader->h_proto) > ETH_DATA_LEN) {
		if ((psEthHeader->h_proto == cpu_to_be16(ETH_P_IPX)) ||
			(psEthHeader->h_proto == cpu_to_le16(0xF380)))
			memcpy((u8 *) (pbyPayloadHead),
					abySNAP_Bridgetunnel, 6);
		else
			memcpy((u8 *) (pbyPayloadHead), &abySNAP_RFC1042[0], 6);

		pbyType = (u8 *) (pbyPayloadHead + 6);

		memcpy(pbyType, &(psEthHeader->h_proto), sizeof(u16));
	}
>>>>>>> refs/remotes/origin/master

    if (pPacket != NULL) {
        // Copy the Packet into a tx Buffer
        memcpy((pbyPayloadHead + cb802_1_H_len),
                 (pPacket + ETH_HLEN),
                 uSkbPacketLen - ETH_HLEN
                 );

    } else {
        // while bRelayPacketSend psEthHeader is point to header+payload
<<<<<<< HEAD
        memcpy((pbyPayloadHead + cb802_1_H_len), ((PBYTE)psEthHeader) + ETH_HLEN, uSkbPacketLen - ETH_HLEN);
    }

    ASSERT(uLength == cbNdisBodySize);

    if ((bNeedEncryption == TRUE) && (pTransmitKey != NULL) && (pTransmitKey->byCipherSuite == KEY_CTL_TKIP)) {

        ///////////////////////////////////////////////////////////////////

        if (pDevice->sMgmtObj.eAuthenMode == WMAC_AUTH_WPANONE) {
            dwMICKey0 = *(PDWORD)(&pTransmitKey->abyKey[16]);
            dwMICKey1 = *(PDWORD)(&pTransmitKey->abyKey[20]);
        }
        else if ((pTransmitKey->dwKeyIndex & AUTHENTICATOR_KEY) != 0) {
            dwMICKey0 = *(PDWORD)(&pTransmitKey->abyKey[16]);
            dwMICKey1 = *(PDWORD)(&pTransmitKey->abyKey[20]);
        }
        else {
            dwMICKey0 = *(PDWORD)(&pTransmitKey->abyKey[24]);
            dwMICKey1 = *(PDWORD)(&pTransmitKey->abyKey[28]);
        }
        // DO Software Michael
        MIC_vInit(dwMICKey0, dwMICKey1);
        MIC_vAppend((PBYTE)&(psEthHeader->abyDstAddr[0]), 12);
        dwMIC_Priority = 0;
        MIC_vAppend((PBYTE)&dwMIC_Priority, 4);
<<<<<<< HEAD
=======
        memcpy((pbyPayloadHead + cb802_1_H_len), ((u8 *)psEthHeader) + ETH_HLEN, uSkbPacketLen - ETH_HLEN);
    }

    if ((bNeedEncryption == true) && (pTransmitKey != NULL) && (pTransmitKey->byCipherSuite == KEY_CTL_TKIP)) {

        ///////////////////////////////////////////////////////////////////

	if (pDevice->vnt_mgmt.eAuthenMode == WMAC_AUTH_WPANONE) {
		dwMICKey0 = *(u32 *)(&pTransmitKey->abyKey[16]);
		dwMICKey1 = *(u32 *)(&pTransmitKey->abyKey[20]);
	}
        else if ((pTransmitKey->dwKeyIndex & AUTHENTICATOR_KEY) != 0) {
            dwMICKey0 = *(u32 *)(&pTransmitKey->abyKey[16]);
            dwMICKey1 = *(u32 *)(&pTransmitKey->abyKey[20]);
        }
        else {
            dwMICKey0 = *(u32 *)(&pTransmitKey->abyKey[24]);
            dwMICKey1 = *(u32 *)(&pTransmitKey->abyKey[28]);
        }
        // DO Software Michael
        MIC_vInit(dwMICKey0, dwMICKey1);
        MIC_vAppend((u8 *)&(psEthHeader->h_dest[0]), 12);
        dwMIC_Priority = 0;
        MIC_vAppend((u8 *)&dwMIC_Priority, 4);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"MIC KEY: %X, %X\n",
		dwMICKey0, dwMICKey1);

        ///////////////////////////////////////////////////////////////////

        //DBG_PRN_GRP12(("Length:%d, %d\n", cbFrameBodySize, uFromHDtoPLDLength));
        //for (ii = 0; ii < cbFrameBodySize; ii++) {
<<<<<<< HEAD
        //    DBG_PRN_GRP12(("%02x ", *((PBYTE)((pbyPayloadHead + cb802_1_H_len) + ii))));
=======
        //    DBG_PRN_GRP12(("%02x ", *((u8 *)((pbyPayloadHead + cb802_1_H_len) + ii))));
>>>>>>> refs/remotes/origin/master
        //}
        //DBG_PRN_GRP12(("\n\n\n"));

        MIC_vAppend(pbyPayloadHead, cbFrameBodySize);

<<<<<<< HEAD
        pdwMIC_L = (PDWORD)(pbyPayloadHead + cbFrameBodySize);
        pdwMIC_R = (PDWORD)(pbyPayloadHead + cbFrameBodySize + 4);
=======
        pdwMIC_L = (u32 *)(pbyPayloadHead + cbFrameBodySize);
        pdwMIC_R = (u32 *)(pbyPayloadHead + cbFrameBodySize + 4);
>>>>>>> refs/remotes/origin/master

        MIC_vGetMIC(pdwMIC_L, pdwMIC_R);
        MIC_vUnInit();

<<<<<<< HEAD
        if (pDevice->bTxMICFail == TRUE) {
            *pdwMIC_L = 0;
            *pdwMIC_R = 0;
            pDevice->bTxMICFail = FALSE;
=======
        if (pDevice->bTxMICFail == true) {
            *pdwMIC_L = 0;
            *pdwMIC_R = 0;
            pDevice->bTxMICFail = false;
>>>>>>> refs/remotes/origin/master
        }
        //DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"uLength: %d, %d\n", uLength, cbFrameBodySize);
        //DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"cbReqCount:%d, %d, %d, %d\n", cbReqCount, cbHeaderLength, uPadding, cbIVlen);
        //DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"MIC:%lX, %lX\n", *pdwMIC_L, *pdwMIC_R);
    }

<<<<<<< HEAD

    if (bSoftWEP == TRUE) {

        s_vSWencryption(pDevice, pTransmitKey, (pbyPayloadHead), (WORD)(cbFrameBodySize + cbMIClen));

    } else if (  ((pDevice->eEncryptionStatus == Ndis802_11Encryption1Enabled) && (bNeedEncryption == TRUE))  ||
          ((pDevice->eEncryptionStatus == Ndis802_11Encryption2Enabled) && (bNeedEncryption == TRUE))   ||
          ((pDevice->eEncryptionStatus == Ndis802_11Encryption3Enabled) && (bNeedEncryption == TRUE))      ) {
        cbFrameSize -= cbICVlen;
    }

    if (pDevice->bSoftwareGenCrcErr == TRUE) {
	unsigned int cbLen;
        PDWORD pdwCRC;

        dwCRC = 0xFFFFFFFFL;
        cbLen = cbFrameSize - cbFCSlen;
        // calculate CRC, and wrtie CRC value to end of TD
        dwCRC = CRCdwGetCrc32Ex(pbyMacHdr, cbLen, dwCRC);
        pdwCRC = (PDWORD)(pbyMacHdr + cbLen);
        // finally, we must invert dwCRC to get the correct answer
        *pdwCRC = ~dwCRC;
        // Force Error
        *pdwCRC -= 1;
    } else {
        cbFrameSize -= cbFCSlen;
    }
=======
    if (bSoftWEP == true) {

        s_vSWencryption(pDevice, pTransmitKey, (pbyPayloadHead), (u16)(cbFrameBodySize + cbMIClen));

    } else if (  ((pDevice->eEncryptionStatus == Ndis802_11Encryption1Enabled) && (bNeedEncryption == true))  ||
          ((pDevice->eEncryptionStatus == Ndis802_11Encryption2Enabled) && (bNeedEncryption == true))   ||
          ((pDevice->eEncryptionStatus == Ndis802_11Encryption3Enabled) && (bNeedEncryption == true))      ) {
        cbFrameSize -= cbICVlen;
    }

        cbFrameSize -= cbFCSlen;
>>>>>>> refs/remotes/origin/master

    *pcbHeaderLen = cbHeaderLength;
    *pcbTotalLen = cbHeaderLength + cbFrameSize ;

<<<<<<< HEAD

    //Set FragCtl in TxBufferHead
    pTxBufHead->wFragCtl |= (WORD)byFragType;


    return TRUE;

}


=======
    //Set FragCtl in TxBufferHead
    pTxBufHead->wFragCtl |= (u16)byFragType;

    return true;

}

>>>>>>> refs/remotes/origin/master
/*+
 *
 * Description:
 *      Translate 802.3 to 802.11 header
 *
 * Parameters:
 *  In:
<<<<<<< HEAD
 *      pDevice         - Pointer to adpater
=======
 *      pDevice         - Pointer to adapter
>>>>>>> refs/remotes/origin/master
 *      dwTxBufferAddr  - Transmit Buffer
 *      pPacket         - Packet from upper layer
 *      cbPacketSize    - Transmit Data Length
 *  Out:
 *      pcbHeadSize         - Header size of MAC&Baseband control and 802.11 Header
 *      pcbAppendPayload    - size of append payload for 802.1H translation
 *
 * Return Value: none
 *
-*/

<<<<<<< HEAD
void
s_vGenerateMACHeader (
     PSDevice         pDevice,
     PBYTE            pbyBufferAddr,
     WORD             wDuration,
     PSEthernetHeader psEthHeader,
     BOOL             bNeedEncrypt,
     WORD             wFragType,
     unsigned int             uDMAIdx,
     unsigned int             uFragIdx
    )
{
    PS802_11Header  pMACHeader = (PS802_11Header)pbyBufferAddr;

    memset(pMACHeader, 0, (sizeof(S802_11Header)));  //- sizeof(pMACHeader->dwIV)));

    if (uDMAIdx == TYPE_ATIMDMA) {
    	pMACHeader->wFrameCtl = TYPE_802_11_ATIM;
    } else {
        pMACHeader->wFrameCtl = TYPE_802_11_DATA;
    }

    if (pDevice->eOPMode == OP_MODE_AP) {
	memcpy(&(pMACHeader->abyAddr1[0]),
	       &(psEthHeader->abyDstAddr[0]),
	       ETH_ALEN);
	memcpy(&(pMACHeader->abyAddr2[0]), &(pDevice->abyBSSID[0]), ETH_ALEN);
	memcpy(&(pMACHeader->abyAddr3[0]),
	       &(psEthHeader->abySrcAddr[0]),
	       ETH_ALEN);
        pMACHeader->wFrameCtl |= FC_FROMDS;
    } else {
	if (pDevice->eOPMode == OP_MODE_ADHOC) {
		memcpy(&(pMACHeader->abyAddr1[0]),
		       &(psEthHeader->abyDstAddr[0]),
		       ETH_ALEN);
		memcpy(&(pMACHeader->abyAddr2[0]),
		       &(psEthHeader->abySrcAddr[0]),
		       ETH_ALEN);
		memcpy(&(pMACHeader->abyAddr3[0]),
		       &(pDevice->abyBSSID[0]),
		       ETH_ALEN);
	} else {
		memcpy(&(pMACHeader->abyAddr3[0]),
		       &(psEthHeader->abyDstAddr[0]),
		       ETH_ALEN);
		memcpy(&(pMACHeader->abyAddr2[0]),
		       &(psEthHeader->abySrcAddr[0]),
		       ETH_ALEN);
		memcpy(&(pMACHeader->abyAddr1[0]),
		       &(pDevice->abyBSSID[0]),
		       ETH_ALEN);
            pMACHeader->wFrameCtl |= FC_TODS;
=======
static void s_vGenerateMACHeader(struct vnt_private *pDevice,
	u8 *pbyBufferAddr, u16 wDuration, struct ethhdr *psEthHeader,
	int bNeedEncrypt, u16 wFragType, u32 uDMAIdx, u32 uFragIdx)
{
	struct ieee80211_hdr *pMACHeader = (struct ieee80211_hdr *)pbyBufferAddr;

	pMACHeader->frame_control = TYPE_802_11_DATA;

    if (pDevice->eOPMode == OP_MODE_AP) {
	memcpy(&(pMACHeader->addr1[0]),
	       &(psEthHeader->h_dest[0]),
	       ETH_ALEN);
	memcpy(&(pMACHeader->addr2[0]), &(pDevice->abyBSSID[0]), ETH_ALEN);
	memcpy(&(pMACHeader->addr3[0]),
	       &(psEthHeader->h_source[0]),
	       ETH_ALEN);
        pMACHeader->frame_control |= FC_FROMDS;
    } else {
	if (pDevice->eOPMode == OP_MODE_ADHOC) {
		memcpy(&(pMACHeader->addr1[0]),
		       &(psEthHeader->h_dest[0]),
		       ETH_ALEN);
		memcpy(&(pMACHeader->addr2[0]),
		       &(psEthHeader->h_source[0]),
		       ETH_ALEN);
		memcpy(&(pMACHeader->addr3[0]),
		       &(pDevice->abyBSSID[0]),
		       ETH_ALEN);
	} else {
		memcpy(&(pMACHeader->addr3[0]),
		       &(psEthHeader->h_dest[0]),
		       ETH_ALEN);
		memcpy(&(pMACHeader->addr2[0]),
		       &(psEthHeader->h_source[0]),
		       ETH_ALEN);
		memcpy(&(pMACHeader->addr1[0]),
		       &(pDevice->abyBSSID[0]),
		       ETH_ALEN);
            pMACHeader->frame_control |= FC_TODS;
>>>>>>> refs/remotes/origin/master
        }
    }

    if (bNeedEncrypt)
<<<<<<< HEAD
        pMACHeader->wFrameCtl |= cpu_to_le16((WORD)WLAN_SET_FC_ISWEP(1));

    pMACHeader->wDurationID = cpu_to_le16(wDuration);

    if (pDevice->bLongHeader) {
        PWLAN_80211HDR_A4 pMACA4Header  = (PWLAN_80211HDR_A4) pbyBufferAddr;
        pMACHeader->wFrameCtl |= (FC_TODS | FC_FROMDS);
        memcpy(pMACA4Header->abyAddr4, pDevice->abyBSSID, WLAN_ADDR_LEN);
    }
    pMACHeader->wSeqCtl = cpu_to_le16(pDevice->wSeqCounter << 4);

    //Set FragNumber in Sequence Control
    pMACHeader->wSeqCtl |= cpu_to_le16((WORD)uFragIdx);
=======
        pMACHeader->frame_control |= cpu_to_le16((u16)WLAN_SET_FC_ISWEP(1));

    pMACHeader->duration_id = cpu_to_le16(wDuration);

    pMACHeader->seq_ctrl = cpu_to_le16(pDevice->wSeqCounter << 4);

    //Set FragNumber in Sequence Control
    pMACHeader->seq_ctrl |= cpu_to_le16((u16)uFragIdx);
>>>>>>> refs/remotes/origin/master

    if ((wFragType == FRAGCTL_ENDFRAG) || (wFragType == FRAGCTL_NONFRAG)) {
        pDevice->wSeqCounter++;
        if (pDevice->wSeqCounter > 0x0fff)
            pDevice->wSeqCounter = 0;
    }

    if ((wFragType == FRAGCTL_STAFRAG) || (wFragType == FRAGCTL_MIDFRAG)) { //StartFrag or MidFrag
<<<<<<< HEAD
        pMACHeader->wFrameCtl |= FC_MOREFRAG;
    }
}



=======
        pMACHeader->frame_control |= FC_MOREFRAG;
    }
}

>>>>>>> refs/remotes/origin/master
/*+
 *
 * Description:
 *      Request instructs a MAC to transmit a 802.11 management packet through
 *      the adapter onto the medium.
 *
 * Parameters:
 *  In:
 *      hDeviceContext  - Pointer to the adapter
 *      pPacket         - A pointer to a descriptor for the packet to transmit
 *  Out:
 *      none
 *
<<<<<<< HEAD
 * Return Value: CMD_STATUS_PENDING if MAC Tx resource available; otherwise FALSE
 *
-*/

CMD_STATUS csMgmt_xmit(
      PSDevice pDevice,
      PSTxMgmtPacket pPacket
    )
{
    BYTE            byPktType;
    PBYTE           pbyTxBufferAddr;
    void *pvRTS;
    PSCTS           pCTS;
    void *pvTxDataHd;
    unsigned int            uDuration;
    unsigned int            cbReqCount;
    PS802_11Header  pMACHeader;
    unsigned int            cbHeaderSize;
    unsigned int            cbFrameBodySize;
    BOOL            bNeedACK;
    BOOL            bIsPSPOLL = FALSE;
    PSTxBufHead     pTxBufHead;
    unsigned int            cbFrameSize;
    unsigned int            cbIVlen = 0;
    unsigned int            cbICVlen = 0;
    unsigned int            cbMIClen = 0;
    unsigned int            cbFCSlen = 4;
    unsigned int            uPadding = 0;
    WORD            wTxBufSize;
    unsigned int            cbMacHdLen;
    SEthernetHeader sEthHeader;
    void *pvRrvTime;
    void *pMICHDR;
    PSMgmtObject    pMgmt = &(pDevice->sMgmtObj);
    WORD            wCurrentRate = RATE_1M;
    PTX_BUFFER          pTX_Buffer;
    PUSB_SEND_CONTEXT   pContext;



    pContext = (PUSB_SEND_CONTEXT)s_vGetFreeContext(pDevice);
=======
 * Return Value: CMD_STATUS_PENDING if MAC Tx resource available; otherwise false
 *
-*/

CMD_STATUS csMgmt_xmit(struct vnt_private *pDevice,
	struct vnt_tx_mgmt *pPacket)
{
	struct vnt_manager *pMgmt = &pDevice->vnt_mgmt;
	struct vnt_tx_buffer *pTX_Buffer;
	struct vnt_usb_send_context *pContext;
	struct vnt_tx_fifo_head *pTxBufHead;
	struct ieee80211_hdr *pMACHeader;
	struct ethhdr sEthHeader;
	u8 byPktType, *pbyTxBufferAddr;
	struct vnt_mic_hdr *pMICHDR = NULL;
	u32 uDuration, cbReqCount, cbHeaderSize, cbFrameBodySize, cbFrameSize;
	int bNeedACK, bIsPSPOLL = false;
	u32 cbIVlen = 0, cbICVlen = 0, cbMIClen = 0, cbFCSlen = 4;
	u32 uPadding = 0;
	u16 wTxBufSize;
	u32 cbMacHdLen;
	u16 wCurrentRate = RATE_1M;

	pContext = s_vGetFreeContext(pDevice);
>>>>>>> refs/remotes/origin/master

    if (NULL == pContext) {
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"ManagementSend TX...NO CONTEXT!\n");
        return CMD_STATUS_RESOURCES;
    }

<<<<<<< HEAD
    pTX_Buffer = (PTX_BUFFER) (&pContext->Data[0]);
    pbyTxBufferAddr = (PBYTE)&(pTX_Buffer->adwTxKey[0]);
    cbFrameBodySize = pPacket->cbPayloadLen;
    pTxBufHead = (PSTxBufHead) pbyTxBufferAddr;
    wTxBufSize = sizeof(STxBufHead);
    memset(pTxBufHead, 0, wTxBufSize);
=======
	pTX_Buffer = (struct vnt_tx_buffer *)&pContext->Data[0];
    cbFrameBodySize = pPacket->cbPayloadLen;
	pTxBufHead = &pTX_Buffer->fifo_head;
	pbyTxBufferAddr = (u8 *)&pTxBufHead->adwTxKey[0];
	wTxBufSize = sizeof(struct vnt_tx_fifo_head);
>>>>>>> refs/remotes/origin/master

    if (pDevice->byBBType == BB_TYPE_11A) {
        wCurrentRate = RATE_6M;
        byPktType = PK_TYPE_11A;
    } else {
        wCurrentRate = RATE_1M;
        byPktType = PK_TYPE_11B;
    }

    // SetPower will cause error power TX state for OFDM Date packet in TX buffer.
    // 2004.11.11 Kyle -- Using OFDM power to tx MngPkt will decrease the connection capability.
    //                    And cmd timer will wait data pkt TX finish before scanning so it's OK
    //                    to set power here.
    if (pMgmt->eScanState != WMAC_NO_SCANNING) {
        RFbSetPower(pDevice, wCurrentRate, pDevice->byCurrentCh);
    } else {
        RFbSetPower(pDevice, wCurrentRate, pMgmt->uCurrChannel);
    }
    pDevice->wCurrentRate = wCurrentRate;

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
    //Set packet type
    if (byPktType == PK_TYPE_11A) {//0000 0000 0000 0000
        pTxBufHead->wFIFOCtl = 0;
    }
    else if (byPktType == PK_TYPE_11B) {//0000 0001 0000 0000
        pTxBufHead->wFIFOCtl |= FIFOCTL_11B;
    }
    else if (byPktType == PK_TYPE_11GB) {//0000 0010 0000 0000
        pTxBufHead->wFIFOCtl |= FIFOCTL_11GB;
    }
    else if (byPktType == PK_TYPE_11GA) {//0000 0011 0000 0000
        pTxBufHead->wFIFOCtl |= FIFOCTL_11GA;
    }

    pTxBufHead->wFIFOCtl |= FIFOCTL_TMOEN;
    pTxBufHead->wTimeStamp = cpu_to_le16(DEFAULT_MGN_LIFETIME_RES_64us);

    if (is_multicast_ether_addr(pPacket->p80211Header->sA3.abyAddr1)) {
<<<<<<< HEAD
        bNeedACK = FALSE;
    }
    else {
        bNeedACK = TRUE;
=======
        bNeedACK = false;
    }
    else {
        bNeedACK = true;
>>>>>>> refs/remotes/origin/master
        pTxBufHead->wFIFOCtl |= FIFOCTL_NEEDACK;
    };

    if ((pMgmt->eCurrMode == WMAC_MODE_ESS_AP) ||
        (pMgmt->eCurrMode == WMAC_MODE_IBSS_STA) ) {

        pTxBufHead->wFIFOCtl |= FIFOCTL_LRETRY;
        //Set Preamble type always long
        //pDevice->byPreambleType = PREAMBLE_LONG;
        // probe-response don't retry
        //if ((pPacket->p80211Header->sA4.wFrameCtl & TYPE_SUBTYPE_MASK) == TYPE_MGMT_PROBE_RSP) {
<<<<<<< HEAD
        //     bNeedACK = FALSE;
=======
        //     bNeedACK = false;
>>>>>>> refs/remotes/origin/master
        //     pTxBufHead->wFIFOCtl  &= (~FIFOCTL_NEEDACK);
        //}
    }

    pTxBufHead->wFIFOCtl |= (FIFOCTL_GENINT | FIFOCTL_ISDMA0);

    if ((pPacket->p80211Header->sA4.wFrameCtl & TYPE_SUBTYPE_MASK) == TYPE_CTL_PSPOLL) {
<<<<<<< HEAD
        bIsPSPOLL = TRUE;
=======
        bIsPSPOLL = true;
>>>>>>> refs/remotes/origin/master
        cbMacHdLen = WLAN_HDR_ADDR2_LEN;
    } else {
        cbMacHdLen = WLAN_HDR_ADDR3_LEN;
    }

    //Set FRAGCTL_MACHDCNT
<<<<<<< HEAD
    pTxBufHead->wFragCtl |= cpu_to_le16((WORD)(cbMacHdLen << 10));
=======
    pTxBufHead->wFragCtl |= cpu_to_le16((u16)(cbMacHdLen << 10));
>>>>>>> refs/remotes/origin/master

    // Notes:
    // Although spec says MMPDU can be fragmented; In most case,
    // no one will send a MMPDU under fragmentation. With RTS may occur.
<<<<<<< HEAD
    pDevice->bAES = FALSE;  //Set FRAGCTL_WEPTYP
=======
>>>>>>> refs/remotes/origin/master

    if (WLAN_GET_FC_ISWEP(pPacket->p80211Header->sA4.wFrameCtl) != 0) {
        if (pDevice->eEncryptionStatus == Ndis802_11Encryption1Enabled) {
            cbIVlen = 4;
            cbICVlen = 4;
    	    pTxBufHead->wFragCtl |= FRAGCTL_LEGACY;
        }
        else if (pDevice->eEncryptionStatus == Ndis802_11Encryption2Enabled) {
            cbIVlen = 8;//IV+ExtIV
            cbMIClen = 8;
            cbICVlen = 4;
    	    pTxBufHead->wFragCtl |= FRAGCTL_TKIP;
    	    //We need to get seed here for filling TxKey entry.
            //TKIPvMixKey(pTransmitKey->abyKey, pDevice->abyCurrentNetAddr,
            //            pTransmitKey->wTSC15_0, pTransmitKey->dwTSC47_16, pDevice->abyPRNG);
        }
        else if (pDevice->eEncryptionStatus == Ndis802_11Encryption3Enabled) {
            cbIVlen = 8;//RSN Header
            cbICVlen = 8;//MIC
            pTxBufHead->wFragCtl |= FRAGCTL_AES;
<<<<<<< HEAD
            pDevice->bAES = TRUE;
=======
>>>>>>> refs/remotes/origin/master
        }
        //MAC Header should be padding 0 to DW alignment.
        uPadding = 4 - (cbMacHdLen%4);
        uPadding %= 4;
    }

    cbFrameSize = cbMacHdLen + cbFrameBodySize + cbIVlen + cbMIClen + cbICVlen + cbFCSlen;

    //Set FIFOCTL_GrpAckPolicy
<<<<<<< HEAD
    if (pDevice->bGrpAckPolicy == TRUE) {//0000 0100 0000 0000
=======
    if (pDevice->bGrpAckPolicy == true) {//0000 0100 0000 0000
>>>>>>> refs/remotes/origin/master
        pTxBufHead->wFIFOCtl |=	FIFOCTL_GRPACK;
    }
    //the rest of pTxBufHead->wFragCtl:FragTyp will be set later in s_vFillFragParameter()

    //Set RrvTime/RTS/CTS Buffer
    if (byPktType == PK_TYPE_11GB || byPktType == PK_TYPE_11GA) {//802.11g packet
<<<<<<< HEAD

        pvRrvTime = (PSRrvTime_gCTS) (pbyTxBufferAddr + wTxBufSize);
        pMICHDR = NULL;
        pvRTS = NULL;
        pCTS = (PSCTS) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_gCTS));
        pvTxDataHd = (PSTxDataHead_g) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_gCTS) + sizeof(SCTS));
        cbHeaderSize = wTxBufSize + sizeof(SRrvTime_gCTS) + sizeof(SCTS) + sizeof(STxDataHead_g);
    }
    else { // 802.11a/b packet
        pvRrvTime = (PSRrvTime_ab) (pbyTxBufferAddr + wTxBufSize);
        pMICHDR = NULL;
        pvRTS = NULL;
        pCTS = NULL;
        pvTxDataHd = (PSTxDataHead_ab) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_ab));
        cbHeaderSize = wTxBufSize + sizeof(SRrvTime_ab) + sizeof(STxDataHead_ab);
    }

    memset((void *)(pbyTxBufferAddr + wTxBufSize), 0,
	   (cbHeaderSize - wTxBufSize));

    memcpy(&(sEthHeader.abyDstAddr[0]),
	   &(pPacket->p80211Header->sA3.abyAddr1[0]),
	   ETH_ALEN);
    memcpy(&(sEthHeader.abySrcAddr[0]),
=======
	cbHeaderSize = wTxBufSize + sizeof(struct vnt_rrv_time_cts) +
		sizeof(struct vnt_cts);
    }
    else { // 802.11a/b packet
	cbHeaderSize = wTxBufSize + sizeof(struct vnt_rrv_time_ab) +
		sizeof(struct vnt_tx_datahead_ab);
    }

    memcpy(&(sEthHeader.h_dest[0]),
	   &(pPacket->p80211Header->sA3.abyAddr1[0]),
	   ETH_ALEN);
    memcpy(&(sEthHeader.h_source[0]),
>>>>>>> refs/remotes/origin/master
	   &(pPacket->p80211Header->sA3.abyAddr2[0]),
	   ETH_ALEN);
    //=========================
    //    No Fragmentation
    //=========================
<<<<<<< HEAD
    pTxBufHead->wFragCtl |= (WORD)FRAGCTL_NONFRAG;


    //Fill FIFO,RrvTime,RTS,and CTS
    s_vGenerateTxParameter(pDevice, byPktType, wCurrentRate,  pbyTxBufferAddr, pvRrvTime, pvRTS, pCTS,
                           cbFrameSize, bNeedACK, TYPE_TXDMA0, &sEthHeader);

    //Fill DataHead
    uDuration = s_uFillDataHead(pDevice, byPktType, wCurrentRate, pvTxDataHd, cbFrameSize, TYPE_TXDMA0, bNeedACK,
                                0, 0, 1, AUTO_FB_NONE);

    pMACHeader = (PS802_11Header) (pbyTxBufferAddr + cbHeaderSize);
=======
    pTxBufHead->wFragCtl |= (u16)FRAGCTL_NONFRAG;

	/* Fill FIFO,RrvTime,RTS,and CTS */
	uDuration = s_vGenerateTxParameter(pDevice, byPktType, wCurrentRate,
		pTX_Buffer, &pMICHDR, 0,
		cbFrameSize, bNeedACK, TYPE_TXDMA0, &sEthHeader, false);

    pMACHeader = (struct ieee80211_hdr *) (pbyTxBufferAddr + cbHeaderSize);
>>>>>>> refs/remotes/origin/master

    cbReqCount = cbHeaderSize + cbMacHdLen + uPadding + cbIVlen + cbFrameBodySize;

    if (WLAN_GET_FC_ISWEP(pPacket->p80211Header->sA4.wFrameCtl) != 0) {
<<<<<<< HEAD
        PBYTE           pbyIVHead;
        PBYTE           pbyPayloadHead;
        PBYTE           pbyBSSID;
        PSKeyItem       pTransmitKey = NULL;

        pbyIVHead = (PBYTE)(pbyTxBufferAddr + cbHeaderSize + cbMacHdLen + uPadding);
        pbyPayloadHead = (PBYTE)(pbyTxBufferAddr + cbHeaderSize + cbMacHdLen + uPadding + cbIVlen);
        do {
            if ((pDevice->eOPMode == OP_MODE_INFRASTRUCTURE) &&
                (pDevice->bLinkPass == TRUE)) {
                pbyBSSID = pDevice->abyBSSID;
                // get pairwise key
                if (KeybGetTransmitKey(&(pDevice->sKey), pbyBSSID, PAIRWISE_KEY, &pTransmitKey) == FALSE) {
                    // get group key
                    if(KeybGetTransmitKey(&(pDevice->sKey), pbyBSSID, GROUP_KEY, &pTransmitKey) == TRUE) {
=======
        u8 *           pbyIVHead;
        u8 *           pbyPayloadHead;
        u8 *           pbyBSSID;
        PSKeyItem       pTransmitKey = NULL;

        pbyIVHead = (u8 *)(pbyTxBufferAddr + cbHeaderSize + cbMacHdLen + uPadding);
        pbyPayloadHead = (u8 *)(pbyTxBufferAddr + cbHeaderSize + cbMacHdLen + uPadding + cbIVlen);
        do {
            if ((pDevice->eOPMode == OP_MODE_INFRASTRUCTURE) &&
                (pDevice->bLinkPass == true)) {
                pbyBSSID = pDevice->abyBSSID;
                // get pairwise key
                if (KeybGetTransmitKey(&(pDevice->sKey), pbyBSSID, PAIRWISE_KEY, &pTransmitKey) == false) {
                    // get group key
                    if(KeybGetTransmitKey(&(pDevice->sKey), pbyBSSID, GROUP_KEY, &pTransmitKey) == true) {
>>>>>>> refs/remotes/origin/master
                        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Get GTK.\n");
                        break;
                    }
                } else {
                    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Get PTK.\n");
                    break;
                }
            }
            // get group key
            pbyBSSID = pDevice->abyBroadcastAddr;
<<<<<<< HEAD
            if(KeybGetTransmitKey(&(pDevice->sKey), pbyBSSID, GROUP_KEY, &pTransmitKey) == FALSE) {
=======
            if(KeybGetTransmitKey(&(pDevice->sKey), pbyBSSID, GROUP_KEY, &pTransmitKey) == false) {
>>>>>>> refs/remotes/origin/master
                pTransmitKey = NULL;
                DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"KEY is NULL. OP Mode[%d]\n", pDevice->eOPMode);
            } else {
                DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Get GTK.\n");
            }
<<<<<<< HEAD
        } while(FALSE);
        //Fill TXKEY
        s_vFillTxKey(pDevice, (PBYTE)(pTxBufHead->adwTxKey), pbyIVHead, pTransmitKey,
                     (PBYTE)pMACHeader, (WORD)cbFrameBodySize, NULL);

        memcpy(pMACHeader, pPacket->p80211Header, cbMacHdLen);
        memcpy(pbyPayloadHead, ((PBYTE)(pPacket->p80211Header) + cbMacHdLen),
=======
        } while(false);
        //Fill TXKEY
	s_vFillTxKey(pDevice, pTxBufHead, pbyIVHead, pTransmitKey,
                     (u8 *)pMACHeader, (u16)cbFrameBodySize, NULL);

        memcpy(pMACHeader, pPacket->p80211Header, cbMacHdLen);
        memcpy(pbyPayloadHead, ((u8 *)(pPacket->p80211Header) + cbMacHdLen),
>>>>>>> refs/remotes/origin/master
                 cbFrameBodySize);
    }
    else {
        // Copy the Packet into a tx Buffer
        memcpy(pMACHeader, pPacket->p80211Header, pPacket->cbMPDULen);
    }

<<<<<<< HEAD
    pMACHeader->wSeqCtl = cpu_to_le16(pDevice->wSeqCounter << 4);
=======
    pMACHeader->seq_ctrl = cpu_to_le16(pDevice->wSeqCounter << 4);
>>>>>>> refs/remotes/origin/master
    pDevice->wSeqCounter++ ;
    if (pDevice->wSeqCounter > 0x0fff)
        pDevice->wSeqCounter = 0;

    if (bIsPSPOLL) {
        // The MAC will automatically replace the Duration-field of MAC header by Duration-field
<<<<<<< HEAD
        // of  FIFO control header.
        // This will cause AID-field of PS-POLL packet be incorrect (Because PS-POLL's AID field is
        // in the same place of other packet's Duration-field).
        // And it will cause Cisco-AP to issue Disassociation-packet
        if (byPktType == PK_TYPE_11GB || byPktType == PK_TYPE_11GA) {
            ((PSTxDataHead_g)pvTxDataHd)->wDuration_a = cpu_to_le16(pPacket->p80211Header->sA2.wDurationID);
            ((PSTxDataHead_g)pvTxDataHd)->wDuration_b = cpu_to_le16(pPacket->p80211Header->sA2.wDurationID);
        } else {
            ((PSTxDataHead_ab)pvTxDataHd)->wDuration = cpu_to_le16(pPacket->p80211Header->sA2.wDurationID);
        }
    }


    pTX_Buffer->wTxByteCount = cpu_to_le16((WORD)(cbReqCount));
    pTX_Buffer->byPKTNO = (BYTE) (((wCurrentRate<<4) &0x00F0) | ((pDevice->wSeqCounter - 1) & 0x000F));
=======
        // of FIFO control header.
        // This will cause AID-field of PS-POLL packet be incorrect (Because PS-POLL's AID field is
        // in the same place of other packet's Duration-field).
        // And it will cause Cisco-AP to issue Disassociation-packet
	if (byPktType == PK_TYPE_11GB || byPktType == PK_TYPE_11GA) {
		struct vnt_tx_datahead_g *data_head = &pTX_Buffer->tx_head.
						tx_cts.tx.head.cts_g.data_head;
		data_head->wDuration_a =
			cpu_to_le16(pPacket->p80211Header->sA2.wDurationID);
		data_head->wDuration_b =
			cpu_to_le16(pPacket->p80211Header->sA2.wDurationID);
	} else {
		struct vnt_tx_datahead_ab *data_head = &pTX_Buffer->tx_head.
					tx_ab.tx.head.data_head_ab;
		data_head->wDuration =
			cpu_to_le16(pPacket->p80211Header->sA2.wDurationID);
	}
    }

    pTX_Buffer->wTxByteCount = cpu_to_le16((u16)(cbReqCount));
    pTX_Buffer->byPKTNO = (u8) (((wCurrentRate<<4) &0x00F0) | ((pDevice->wSeqCounter - 1) & 0x000F));
>>>>>>> refs/remotes/origin/master
    pTX_Buffer->byType = 0x00;

    pContext->pPacket = NULL;
    pContext->Type = CONTEXT_MGMT_PACKET;
<<<<<<< HEAD
    pContext->uBufLen = (WORD)cbReqCount + 4;  //USB header

    if (WLAN_GET_FC_TODS(pMACHeader->wFrameCtl) == 0) {
        s_vSaveTxPktInfo(pDevice, (BYTE) (pTX_Buffer->byPKTNO & 0x0F), &(pMACHeader->abyAddr1[0]),(WORD)cbFrameSize,pTX_Buffer->wFIFOCtl);
    }
    else {
        s_vSaveTxPktInfo(pDevice, (BYTE) (pTX_Buffer->byPKTNO & 0x0F), &(pMACHeader->abyAddr3[0]),(WORD)cbFrameSize,pTX_Buffer->wFIFOCtl);
=======
    pContext->uBufLen = (u16)cbReqCount + 4;  //USB header

    if (WLAN_GET_FC_TODS(pMACHeader->frame_control) == 0) {
	s_vSaveTxPktInfo(pDevice, (u8)(pTX_Buffer->byPKTNO & 0x0F),
			&pMACHeader->addr1[0], (u16)cbFrameSize,
			pTxBufHead->wFIFOCtl);
    }
    else {
	s_vSaveTxPktInfo(pDevice, (u8)(pTX_Buffer->byPKTNO & 0x0F),
			&pMACHeader->addr3[0], (u16)cbFrameSize,
			pTxBufHead->wFIFOCtl);
>>>>>>> refs/remotes/origin/master
    }

    PIPEnsSendBulkOut(pDevice,pContext);
    return CMD_STATUS_PENDING;
}

<<<<<<< HEAD

CMD_STATUS
csBeacon_xmit(
      PSDevice pDevice,
      PSTxMgmtPacket pPacket
    )
{

    unsigned int                cbFrameSize = pPacket->cbMPDULen + WLAN_FCS_LEN;
    unsigned int                cbHeaderSize = 0;
    WORD                wTxBufSize = sizeof(STxShortBufHead);
    PSTxShortBufHead    pTxBufHead;
    PS802_11Header      pMACHeader;
    PSTxDataHead_ab     pTxDataHead;
    WORD                wCurrentRate;
    unsigned int                cbFrameBodySize;
    unsigned int                cbReqCount;
    PBEACON_BUFFER      pTX_Buffer;
    PBYTE               pbyTxBufferAddr;
    PUSB_SEND_CONTEXT   pContext;
    CMD_STATUS          status;


    pContext = (PUSB_SEND_CONTEXT)s_vGetFreeContext(pDevice);
=======
CMD_STATUS csBeacon_xmit(struct vnt_private *pDevice,
	struct vnt_tx_mgmt *pPacket)
{
	struct vnt_beacon_buffer *pTX_Buffer;
	struct vnt_tx_short_buf_head *short_head;
	u32 cbFrameSize = pPacket->cbMPDULen + WLAN_FCS_LEN;
	u32 cbHeaderSize = 0;
	struct ieee80211_hdr *pMACHeader;
	u16 wCurrentRate;
	u32 cbFrameBodySize;
	u32 cbReqCount;
	struct vnt_usb_send_context *pContext;
	CMD_STATUS status;

	pContext = s_vGetFreeContext(pDevice);
>>>>>>> refs/remotes/origin/master
    if (NULL == pContext) {
        status = CMD_STATUS_RESOURCES;
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"ManagementSend TX...NO CONTEXT!\n");
        return status ;
    }
<<<<<<< HEAD
    pTX_Buffer = (PBEACON_BUFFER) (&pContext->Data[0]);
    pbyTxBufferAddr = (PBYTE)&(pTX_Buffer->wFIFOCtl);

    cbFrameBodySize = pPacket->cbPayloadLen;

    pTxBufHead = (PSTxShortBufHead) pbyTxBufferAddr;
    wTxBufSize = sizeof(STxShortBufHead);
    memset(pTxBufHead, 0, wTxBufSize);

    if (pDevice->byBBType == BB_TYPE_11A) {
        wCurrentRate = RATE_6M;
        pTxDataHead = (PSTxDataHead_ab) (pbyTxBufferAddr + wTxBufSize);
        //Get SignalField,ServiceField,Length
        BBvCaculateParameter(pDevice, cbFrameSize, wCurrentRate, PK_TYPE_11A,
            (PWORD)&(pTxDataHead->wTransmitLength), (PBYTE)&(pTxDataHead->byServiceField), (PBYTE)&(pTxDataHead->bySignalField)
        );
        //Get Duration and TimeStampOff
        pTxDataHead->wDuration = cpu_to_le16((WORD)s_uGetDataDuration(pDevice, DATADUR_A, cbFrameSize, PK_TYPE_11A,
                                                          wCurrentRate, FALSE, 0, 0, 1, AUTO_FB_NONE));
        pTxDataHead->wTimeStampOff = wTimeStampOff[pDevice->byPreambleType%2][wCurrentRate%MAX_RATE];
        cbHeaderSize = wTxBufSize + sizeof(STxDataHead_ab);
    } else {
        wCurrentRate = RATE_1M;
        pTxBufHead->wFIFOCtl |= FIFOCTL_11B;
        pTxDataHead = (PSTxDataHead_ab) (pbyTxBufferAddr + wTxBufSize);
        //Get SignalField,ServiceField,Length
        BBvCaculateParameter(pDevice, cbFrameSize, wCurrentRate, PK_TYPE_11B,
            (PWORD)&(pTxDataHead->wTransmitLength), (PBYTE)&(pTxDataHead->byServiceField), (PBYTE)&(pTxDataHead->bySignalField)
        );
        //Get Duration and TimeStampOff
        pTxDataHead->wDuration = cpu_to_le16((WORD)s_uGetDataDuration(pDevice, DATADUR_B, cbFrameSize, PK_TYPE_11B,
                                                          wCurrentRate, FALSE, 0, 0, 1, AUTO_FB_NONE));
        pTxDataHead->wTimeStampOff = wTimeStampOff[pDevice->byPreambleType%2][wCurrentRate%MAX_RATE];
        cbHeaderSize = wTxBufSize + sizeof(STxDataHead_ab);
    }

    //Generate Beacon Header
    pMACHeader = (PS802_11Header)(pbyTxBufferAddr + cbHeaderSize);
    memcpy(pMACHeader, pPacket->p80211Header, pPacket->cbMPDULen);

    pMACHeader->wDurationID = 0;
    pMACHeader->wSeqCtl = cpu_to_le16(pDevice->wSeqCounter << 4);
    pDevice->wSeqCounter++ ;
    if (pDevice->wSeqCounter > 0x0fff)
        pDevice->wSeqCounter = 0;

    cbReqCount = cbHeaderSize + WLAN_HDR_ADDR3_LEN + cbFrameBodySize;

    pTX_Buffer->wTxByteCount = (WORD)cbReqCount;
    pTX_Buffer->byPKTNO = (BYTE) (((wCurrentRate<<4) &0x00F0) | ((pDevice->wSeqCounter - 1) & 0x000F));
=======

	pTX_Buffer = (struct vnt_beacon_buffer *)&pContext->Data[0];
	short_head = &pTX_Buffer->short_head;

    cbFrameBodySize = pPacket->cbPayloadLen;

	cbHeaderSize = sizeof(struct vnt_tx_short_buf_head);

	if (pDevice->byBBType == BB_TYPE_11A) {
		wCurrentRate = RATE_6M;

		/* Get SignalField,ServiceField,Length */
		BBvCalculateParameter(pDevice, cbFrameSize, wCurrentRate,
			PK_TYPE_11A, &short_head->ab);

		/* Get Duration and TimeStampOff */
		short_head->duration = s_uGetDataDuration(pDevice,
							PK_TYPE_11A, false);
		short_head->time_stamp_off =
				vnt_time_stamp_off(pDevice, wCurrentRate);
	} else {
		wCurrentRate = RATE_1M;
		short_head->fifo_ctl |= FIFOCTL_11B;

		/* Get SignalField,ServiceField,Length */
		BBvCalculateParameter(pDevice, cbFrameSize, wCurrentRate,
					PK_TYPE_11B, &short_head->ab);

		/* Get Duration and TimeStampOff */
		short_head->duration = s_uGetDataDuration(pDevice,
						PK_TYPE_11B, false);
		short_head->time_stamp_off =
			vnt_time_stamp_off(pDevice, wCurrentRate);
	}


	/* Generate Beacon Header */
	pMACHeader = &pTX_Buffer->hdr;

	memcpy(pMACHeader, pPacket->p80211Header, pPacket->cbMPDULen);

	pMACHeader->duration_id = 0;
	pMACHeader->seq_ctrl = cpu_to_le16(pDevice->wSeqCounter << 4);
	pDevice->wSeqCounter++;
	if (pDevice->wSeqCounter > 0x0fff)
		pDevice->wSeqCounter = 0;

    cbReqCount = cbHeaderSize + WLAN_HDR_ADDR3_LEN + cbFrameBodySize;

    pTX_Buffer->wTxByteCount = (u16)cbReqCount;
    pTX_Buffer->byPKTNO = (u8) (((wCurrentRate<<4) &0x00F0) | ((pDevice->wSeqCounter - 1) & 0x000F));
>>>>>>> refs/remotes/origin/master
    pTX_Buffer->byType = 0x01;

    pContext->pPacket = NULL;
    pContext->Type = CONTEXT_MGMT_PACKET;
<<<<<<< HEAD
    pContext->uBufLen = (WORD)cbReqCount + 4;  //USB header
=======
    pContext->uBufLen = (u16)cbReqCount + 4;  //USB header
>>>>>>> refs/remotes/origin/master

    PIPEnsSendBulkOut(pDevice,pContext);
    return CMD_STATUS_PENDING;

}

<<<<<<< HEAD




void
vDMA0_tx_80211(PSDevice  pDevice, struct sk_buff *skb) {

    PSMgmtObject    pMgmt = &(pDevice->sMgmtObj);
    BYTE            byPktType;
    PBYTE           pbyTxBufferAddr;
    void *pvRTS;
    void *pvCTS;
    void *pvTxDataHd;
    unsigned int            uDuration;
    unsigned int            cbReqCount;
    PS802_11Header  pMACHeader;
    unsigned int            cbHeaderSize;
    unsigned int            cbFrameBodySize;
    BOOL            bNeedACK;
    BOOL            bIsPSPOLL = FALSE;
    PSTxBufHead     pTxBufHead;
    unsigned int            cbFrameSize;
    unsigned int            cbIVlen = 0;
    unsigned int            cbICVlen = 0;
    unsigned int            cbMIClen = 0;
    unsigned int            cbFCSlen = 4;
    unsigned int            uPadding = 0;
    unsigned int            cbMICHDR = 0;
    unsigned int            uLength = 0;
    DWORD           dwMICKey0, dwMICKey1;
    DWORD           dwMIC_Priority;
    PDWORD          pdwMIC_L;
    PDWORD          pdwMIC_R;
    WORD            wTxBufSize;
    unsigned int            cbMacHdLen;
    SEthernetHeader sEthHeader;
    void *pvRrvTime;
    void *pMICHDR;
    WORD            wCurrentRate = RATE_1M;
    PUWLAN_80211HDR  p80211Header;
    unsigned int             uNodeIndex = 0;
    BOOL            bNodeExist = FALSE;
    SKeyItem        STempKey;
    PSKeyItem       pTransmitKey = NULL;
    PBYTE           pbyIVHead;
    PBYTE           pbyPayloadHead;
    PBYTE           pbyMacHdr;
    unsigned int            cbExtSuppRate = 0;
    PTX_BUFFER          pTX_Buffer;
    PUSB_SEND_CONTEXT   pContext;
//    PWLAN_IE        pItem;


    pvRrvTime = pMICHDR = pvRTS = pvCTS = pvTxDataHd = NULL;
=======
void vDMA0_tx_80211(struct vnt_private *pDevice, struct sk_buff *skb)
{
	struct vnt_manager *pMgmt = &pDevice->vnt_mgmt;
	struct vnt_tx_buffer *pTX_Buffer;
	struct vnt_tx_fifo_head *pTxBufHead;
	u8 byPktType;
	u8 *pbyTxBufferAddr;
	u32 uDuration, cbReqCount;
	struct ieee80211_hdr *pMACHeader;
	u32 cbHeaderSize, cbFrameBodySize;
	int bNeedACK, bIsPSPOLL = false;
	u32 cbFrameSize;
	u32 cbIVlen = 0, cbICVlen = 0, cbMIClen = 0, cbFCSlen = 4;
	u32 uPadding = 0;
	u32 cbMICHDR = 0, uLength = 0;
	u32 dwMICKey0, dwMICKey1;
	u32 dwMIC_Priority;
	u32 *pdwMIC_L, *pdwMIC_R;
	u16 wTxBufSize;
	u32 cbMacHdLen;
	struct ethhdr sEthHeader;
	struct vnt_mic_hdr *pMICHDR;
	u32 wCurrentRate = RATE_1M;
	PUWLAN_80211HDR  p80211Header;
	u32 uNodeIndex = 0;
	int bNodeExist = false;
	SKeyItem STempKey;
	PSKeyItem pTransmitKey = NULL;
	u8 *pbyIVHead, *pbyPayloadHead, *pbyMacHdr;
	u32 cbExtSuppRate = 0;
	struct vnt_usb_send_context *pContext;

	pMICHDR = NULL;
>>>>>>> refs/remotes/origin/master

    if(skb->len <= WLAN_HDR_ADDR3_LEN) {
       cbFrameBodySize = 0;
    }
    else {
       cbFrameBodySize = skb->len - WLAN_HDR_ADDR3_LEN;
    }
    p80211Header = (PUWLAN_80211HDR)skb->data;

<<<<<<< HEAD
    pContext = (PUSB_SEND_CONTEXT)s_vGetFreeContext(pDevice);
=======
	pContext = s_vGetFreeContext(pDevice);
>>>>>>> refs/remotes/origin/master

    if (NULL == pContext) {
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"DMA0 TX...NO CONTEXT!\n");
        dev_kfree_skb_irq(skb);
        return ;
    }

<<<<<<< HEAD
    pTX_Buffer = (PTX_BUFFER)(&pContext->Data[0]);
    pbyTxBufferAddr = (PBYTE)(&pTX_Buffer->adwTxKey[0]);
    pTxBufHead = (PSTxBufHead) pbyTxBufferAddr;
    wTxBufSize = sizeof(STxBufHead);
    memset(pTxBufHead, 0, wTxBufSize);
=======
	pTX_Buffer = (struct vnt_tx_buffer *)&pContext->Data[0];
	pTxBufHead = &pTX_Buffer->fifo_head;
	pbyTxBufferAddr = (u8 *)&pTxBufHead->adwTxKey[0];
	wTxBufSize = sizeof(struct vnt_tx_fifo_head);
>>>>>>> refs/remotes/origin/master

    if (pDevice->byBBType == BB_TYPE_11A) {
        wCurrentRate = RATE_6M;
        byPktType = PK_TYPE_11A;
    } else {
        wCurrentRate = RATE_1M;
        byPktType = PK_TYPE_11B;
    }

    // SetPower will cause error power TX state for OFDM Date packet in TX buffer.
    // 2004.11.11 Kyle -- Using OFDM power to tx MngPkt will decrease the connection capability.
    //                    And cmd timer will wait data pkt TX finish before scanning so it's OK
    //                    to set power here.
    if (pMgmt->eScanState != WMAC_NO_SCANNING) {
        RFbSetPower(pDevice, wCurrentRate, pDevice->byCurrentCh);
    } else {
        RFbSetPower(pDevice, wCurrentRate, pMgmt->uCurrChannel);
    }

    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"vDMA0_tx_80211: p80211Header->sA3.wFrameCtl = %x \n", p80211Header->sA3.wFrameCtl);

    //Set packet type
    if (byPktType == PK_TYPE_11A) {//0000 0000 0000 0000
        pTxBufHead->wFIFOCtl = 0;
    }
    else if (byPktType == PK_TYPE_11B) {//0000 0001 0000 0000
        pTxBufHead->wFIFOCtl |= FIFOCTL_11B;
    }
    else if (byPktType == PK_TYPE_11GB) {//0000 0010 0000 0000
        pTxBufHead->wFIFOCtl |= FIFOCTL_11GB;
    }
    else if (byPktType == PK_TYPE_11GA) {//0000 0011 0000 0000
        pTxBufHead->wFIFOCtl |= FIFOCTL_11GA;
    }

    pTxBufHead->wFIFOCtl |= FIFOCTL_TMOEN;
    pTxBufHead->wTimeStamp = cpu_to_le16(DEFAULT_MGN_LIFETIME_RES_64us);

    if (is_multicast_ether_addr(p80211Header->sA3.abyAddr1)) {
<<<<<<< HEAD
        bNeedACK = FALSE;
        if (pDevice->bEnableHostWEP) {
            uNodeIndex = 0;
            bNodeExist = TRUE;
=======
        bNeedACK = false;
        if (pDevice->bEnableHostWEP) {
            uNodeIndex = 0;
            bNodeExist = true;
>>>>>>> refs/remotes/origin/master
        }
    }
    else {
        if (pDevice->bEnableHostWEP) {
<<<<<<< HEAD
            if (BSSbIsSTAInNodeDB(pDevice, (PBYTE)(p80211Header->sA3.abyAddr1), &uNodeIndex))
                bNodeExist = TRUE;
        }
        bNeedACK = TRUE;
=======
            if (BSSbIsSTAInNodeDB(pDevice, (u8 *)(p80211Header->sA3.abyAddr1), &uNodeIndex))
                bNodeExist = true;
        }
        bNeedACK = true;
>>>>>>> refs/remotes/origin/master
        pTxBufHead->wFIFOCtl |= FIFOCTL_NEEDACK;
    };

    if ((pMgmt->eCurrMode == WMAC_MODE_ESS_AP) ||
        (pMgmt->eCurrMode == WMAC_MODE_IBSS_STA) ) {

        pTxBufHead->wFIFOCtl |= FIFOCTL_LRETRY;
        //Set Preamble type always long
        //pDevice->byPreambleType = PREAMBLE_LONG;

        // probe-response don't retry
        //if ((p80211Header->sA4.wFrameCtl & TYPE_SUBTYPE_MASK) == TYPE_MGMT_PROBE_RSP) {
<<<<<<< HEAD
        //     bNeedACK = FALSE;
=======
        //     bNeedACK = false;
>>>>>>> refs/remotes/origin/master
        //     pTxBufHead->wFIFOCtl  &= (~FIFOCTL_NEEDACK);
        //}
    }

    pTxBufHead->wFIFOCtl |= (FIFOCTL_GENINT | FIFOCTL_ISDMA0);

    if ((p80211Header->sA4.wFrameCtl & TYPE_SUBTYPE_MASK) == TYPE_CTL_PSPOLL) {
<<<<<<< HEAD
        bIsPSPOLL = TRUE;
=======
        bIsPSPOLL = true;
>>>>>>> refs/remotes/origin/master
        cbMacHdLen = WLAN_HDR_ADDR2_LEN;
    } else {
        cbMacHdLen = WLAN_HDR_ADDR3_LEN;
    }

<<<<<<< HEAD
    // hostapd deamon ext support rate patch
=======
    // hostapd daemon ext support rate patch
>>>>>>> refs/remotes/origin/master
    if (WLAN_GET_FC_FSTYPE(p80211Header->sA4.wFrameCtl) == WLAN_FSTYPE_ASSOCRESP) {

        if (((PWLAN_IE_SUPP_RATES)pMgmt->abyCurrSuppRates)->len != 0) {
            cbExtSuppRate += ((PWLAN_IE_SUPP_RATES)pMgmt->abyCurrSuppRates)->len + WLAN_IEHDR_LEN;
         }

        if (((PWLAN_IE_SUPP_RATES)pMgmt->abyCurrExtSuppRates)->len != 0) {
            cbExtSuppRate += ((PWLAN_IE_SUPP_RATES)pMgmt->abyCurrExtSuppRates)->len + WLAN_IEHDR_LEN;
         }

         if (cbExtSuppRate >0) {
            cbFrameBodySize = WLAN_ASSOCRESP_OFF_SUPP_RATES;
         }
    }

<<<<<<< HEAD

    //Set FRAGCTL_MACHDCNT
    pTxBufHead->wFragCtl |= cpu_to_le16((WORD)cbMacHdLen << 10);
=======
    //Set FRAGCTL_MACHDCNT
    pTxBufHead->wFragCtl |= cpu_to_le16((u16)cbMacHdLen << 10);
>>>>>>> refs/remotes/origin/master

    // Notes:
    // Although spec says MMPDU can be fragmented; In most case,
    // no one will send a MMPDU under fragmentation. With RTS may occur.
<<<<<<< HEAD
    pDevice->bAES = FALSE;  //Set FRAGCTL_WEPTYP

=======
>>>>>>> refs/remotes/origin/master

    if (WLAN_GET_FC_ISWEP(p80211Header->sA4.wFrameCtl) != 0) {
        if (pDevice->eEncryptionStatus == Ndis802_11Encryption1Enabled) {
            cbIVlen = 4;
            cbICVlen = 4;
    	    pTxBufHead->wFragCtl |= FRAGCTL_LEGACY;
        }
        else if (pDevice->eEncryptionStatus == Ndis802_11Encryption2Enabled) {
            cbIVlen = 8;//IV+ExtIV
            cbMIClen = 8;
            cbICVlen = 4;
    	    pTxBufHead->wFragCtl |= FRAGCTL_TKIP;
    	    //We need to get seed here for filling TxKey entry.
            //TKIPvMixKey(pTransmitKey->abyKey, pDevice->abyCurrentNetAddr,
            //            pTransmitKey->wTSC15_0, pTransmitKey->dwTSC47_16, pDevice->abyPRNG);
        }
        else if (pDevice->eEncryptionStatus == Ndis802_11Encryption3Enabled) {
            cbIVlen = 8;//RSN Header
            cbICVlen = 8;//MIC
<<<<<<< HEAD
            cbMICHDR = sizeof(SMICHDRHead);
            pTxBufHead->wFragCtl |= FRAGCTL_AES;
            pDevice->bAES = TRUE;
=======
	    cbMICHDR = sizeof(struct vnt_mic_hdr);
            pTxBufHead->wFragCtl |= FRAGCTL_AES;
>>>>>>> refs/remotes/origin/master
        }
        //MAC Header should be padding 0 to DW alignment.
        uPadding = 4 - (cbMacHdLen%4);
        uPadding %= 4;
    }

    cbFrameSize = cbMacHdLen + cbFrameBodySize + cbIVlen + cbMIClen + cbICVlen + cbFCSlen + cbExtSuppRate;

    //Set FIFOCTL_GrpAckPolicy
<<<<<<< HEAD
    if (pDevice->bGrpAckPolicy == TRUE) {//0000 0100 0000 0000
=======
    if (pDevice->bGrpAckPolicy == true) {//0000 0100 0000 0000
>>>>>>> refs/remotes/origin/master
        pTxBufHead->wFIFOCtl |=	FIFOCTL_GRPACK;
    }
    //the rest of pTxBufHead->wFragCtl:FragTyp will be set later in s_vFillFragParameter()

<<<<<<< HEAD

    if (byPktType == PK_TYPE_11GB || byPktType == PK_TYPE_11GA) {//802.11g packet

        pvRrvTime = (PSRrvTime_gCTS) (pbyTxBufferAddr + wTxBufSize);
        pMICHDR = (PSMICHDRHead) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_gCTS));
        pvRTS = NULL;
        pvCTS = (PSCTS) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_gCTS) + cbMICHDR);
        pvTxDataHd = (PSTxDataHead_g) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_gCTS) + cbMICHDR + sizeof(SCTS));
        cbHeaderSize = wTxBufSize + sizeof(SRrvTime_gCTS) + cbMICHDR + sizeof(SCTS) + sizeof(STxDataHead_g);

    }
    else {//802.11a/b packet

        pvRrvTime = (PSRrvTime_ab) (pbyTxBufferAddr + wTxBufSize);
        pMICHDR = (PSMICHDRHead) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_ab));
        pvRTS = NULL;
        pvCTS = NULL;
        pvTxDataHd = (PSTxDataHead_ab) (pbyTxBufferAddr + wTxBufSize + sizeof(SRrvTime_ab) + cbMICHDR);
        cbHeaderSize = wTxBufSize + sizeof(SRrvTime_ab) + cbMICHDR + sizeof(STxDataHead_ab);
    }
    memset((void *)(pbyTxBufferAddr + wTxBufSize), 0,
	   (cbHeaderSize - wTxBufSize));
    memcpy(&(sEthHeader.abyDstAddr[0]),
	   &(p80211Header->sA3.abyAddr1[0]),
	   ETH_ALEN);
    memcpy(&(sEthHeader.abySrcAddr[0]),
=======
    if (byPktType == PK_TYPE_11GB || byPktType == PK_TYPE_11GA) {//802.11g packet
	cbHeaderSize = wTxBufSize + sizeof(struct vnt_rrv_time_cts) + cbMICHDR +
		sizeof(struct vnt_cts);

    }
    else {//802.11a/b packet
	cbHeaderSize = wTxBufSize + sizeof(struct vnt_rrv_time_ab) + cbMICHDR +
					sizeof(struct vnt_tx_datahead_ab);
    }
    memcpy(&(sEthHeader.h_dest[0]),
	   &(p80211Header->sA3.abyAddr1[0]),
	   ETH_ALEN);
    memcpy(&(sEthHeader.h_source[0]),
>>>>>>> refs/remotes/origin/master
	   &(p80211Header->sA3.abyAddr2[0]),
	   ETH_ALEN);
    //=========================
    //    No Fragmentation
    //=========================
<<<<<<< HEAD
    pTxBufHead->wFragCtl |= (WORD)FRAGCTL_NONFRAG;


    //Fill FIFO,RrvTime,RTS,and CTS
    s_vGenerateTxParameter(pDevice, byPktType, wCurrentRate, pbyTxBufferAddr, pvRrvTime, pvRTS, pvCTS,
                           cbFrameSize, bNeedACK, TYPE_TXDMA0, &sEthHeader);

    //Fill DataHead
    uDuration = s_uFillDataHead(pDevice, byPktType, wCurrentRate, pvTxDataHd, cbFrameSize, TYPE_TXDMA0, bNeedACK,
                                0, 0, 1, AUTO_FB_NONE);

    pMACHeader = (PS802_11Header) (pbyTxBufferAddr + cbHeaderSize);

    cbReqCount = cbHeaderSize + cbMacHdLen + uPadding + cbIVlen + (cbFrameBodySize + cbMIClen) + cbExtSuppRate;

    pbyMacHdr = (PBYTE)(pbyTxBufferAddr + cbHeaderSize);
    pbyPayloadHead = (PBYTE)(pbyMacHdr + cbMacHdLen + uPadding + cbIVlen);
    pbyIVHead = (PBYTE)(pbyMacHdr + cbMacHdLen + uPadding);
=======
    pTxBufHead->wFragCtl |= (u16)FRAGCTL_NONFRAG;

	/* Fill FIFO,RrvTime,RTS,and CTS */
	uDuration = s_vGenerateTxParameter(pDevice, byPktType, wCurrentRate,
		pTX_Buffer, &pMICHDR, cbMICHDR,
		cbFrameSize, bNeedACK, TYPE_TXDMA0, &sEthHeader, false);

	pMACHeader = (struct ieee80211_hdr *) (pbyTxBufferAddr + cbHeaderSize);

    cbReqCount = cbHeaderSize + cbMacHdLen + uPadding + cbIVlen + (cbFrameBodySize + cbMIClen) + cbExtSuppRate;

    pbyMacHdr = (u8 *)(pbyTxBufferAddr + cbHeaderSize);
    pbyPayloadHead = (u8 *)(pbyMacHdr + cbMacHdLen + uPadding + cbIVlen);
    pbyIVHead = (u8 *)(pbyMacHdr + cbMacHdLen + uPadding);
>>>>>>> refs/remotes/origin/master

    // Copy the Packet into a tx Buffer
    memcpy(pbyMacHdr, skb->data, cbMacHdLen);

    // version set to 0, patch for hostapd deamon
<<<<<<< HEAD
    pMACHeader->wFrameCtl &= cpu_to_le16(0xfffc);
    memcpy(pbyPayloadHead, (skb->data + cbMacHdLen), cbFrameBodySize);

    // replace support rate, patch for hostapd deamon( only support 11M)
=======
    pMACHeader->frame_control &= cpu_to_le16(0xfffc);
    memcpy(pbyPayloadHead, (skb->data + cbMacHdLen), cbFrameBodySize);

    // replace support rate, patch for hostapd daemon( only support 11M)
>>>>>>> refs/remotes/origin/master
    if (WLAN_GET_FC_FSTYPE(p80211Header->sA4.wFrameCtl) == WLAN_FSTYPE_ASSOCRESP) {
        if (cbExtSuppRate != 0) {
            if (((PWLAN_IE_SUPP_RATES)pMgmt->abyCurrSuppRates)->len != 0)
                memcpy((pbyPayloadHead + cbFrameBodySize),
                        pMgmt->abyCurrSuppRates,
                        ((PWLAN_IE_SUPP_RATES)pMgmt->abyCurrSuppRates)->len + WLAN_IEHDR_LEN
                       );
             if (((PWLAN_IE_SUPP_RATES)pMgmt->abyCurrExtSuppRates)->len != 0)
                memcpy((pbyPayloadHead + cbFrameBodySize) + ((PWLAN_IE_SUPP_RATES)pMgmt->abyCurrSuppRates)->len + WLAN_IEHDR_LEN,
                        pMgmt->abyCurrExtSuppRates,
                        ((PWLAN_IE_SUPP_RATES)pMgmt->abyCurrExtSuppRates)->len + WLAN_IEHDR_LEN
                       );
         }
    }

    // Set wep
    if (WLAN_GET_FC_ISWEP(p80211Header->sA4.wFrameCtl) != 0) {

        if (pDevice->bEnableHostWEP) {
            pTransmitKey = &STempKey;
            pTransmitKey->byCipherSuite = pMgmt->sNodeDBTable[uNodeIndex].byCipherSuite;
            pTransmitKey->dwKeyIndex = pMgmt->sNodeDBTable[uNodeIndex].dwKeyIndex;
            pTransmitKey->uKeyLength = pMgmt->sNodeDBTable[uNodeIndex].uWepKeyLength;
            pTransmitKey->dwTSC47_16 = pMgmt->sNodeDBTable[uNodeIndex].dwTSC47_16;
            pTransmitKey->wTSC15_0 = pMgmt->sNodeDBTable[uNodeIndex].wTSC15_0;
            memcpy(pTransmitKey->abyKey,
                &pMgmt->sNodeDBTable[uNodeIndex].abyWepKey[0],
                pTransmitKey->uKeyLength
                );
        }

        if ((pTransmitKey != NULL) && (pTransmitKey->byCipherSuite == KEY_CTL_TKIP)) {

<<<<<<< HEAD
            dwMICKey0 = *(PDWORD)(&pTransmitKey->abyKey[16]);
            dwMICKey1 = *(PDWORD)(&pTransmitKey->abyKey[20]);

            // DO Software Michael
            MIC_vInit(dwMICKey0, dwMICKey1);
            MIC_vAppend((PBYTE)&(sEthHeader.abyDstAddr[0]), 12);
            dwMIC_Priority = 0;
            MIC_vAppend((PBYTE)&dwMIC_Priority, 4);
<<<<<<< HEAD
=======
            dwMICKey0 = *(u32 *)(&pTransmitKey->abyKey[16]);
            dwMICKey1 = *(u32 *)(&pTransmitKey->abyKey[20]);

            // DO Software Michael
            MIC_vInit(dwMICKey0, dwMICKey1);
            MIC_vAppend((u8 *)&(sEthHeader.h_dest[0]), 12);
            dwMIC_Priority = 0;
            MIC_vAppend((u8 *)&dwMIC_Priority, 4);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"DMA0_tx_8021:MIC KEY:"\
			" %X, %X\n", dwMICKey0, dwMICKey1);

            uLength = cbHeaderSize + cbMacHdLen + uPadding + cbIVlen;

            MIC_vAppend((pbyTxBufferAddr + uLength), cbFrameBodySize);

<<<<<<< HEAD
            pdwMIC_L = (PDWORD)(pbyTxBufferAddr + uLength + cbFrameBodySize);
            pdwMIC_R = (PDWORD)(pbyTxBufferAddr + uLength + cbFrameBodySize + 4);
=======
            pdwMIC_L = (u32 *)(pbyTxBufferAddr + uLength + cbFrameBodySize);
            pdwMIC_R = (u32 *)(pbyTxBufferAddr + uLength + cbFrameBodySize + 4);
>>>>>>> refs/remotes/origin/master

            MIC_vGetMIC(pdwMIC_L, pdwMIC_R);
            MIC_vUnInit();

<<<<<<< HEAD
            if (pDevice->bTxMICFail == TRUE) {
                *pdwMIC_L = 0;
                *pdwMIC_R = 0;
                pDevice->bTxMICFail = FALSE;
=======
            if (pDevice->bTxMICFail == true) {
                *pdwMIC_L = 0;
                *pdwMIC_R = 0;
                pDevice->bTxMICFail = false;
>>>>>>> refs/remotes/origin/master
            }

            DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"uLength: %d, %d\n", uLength, cbFrameBodySize);
            DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"cbReqCount:%d, %d, %d, %d\n", cbReqCount, cbHeaderSize, uPadding, cbIVlen);
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"MIC:%x, %x\n",
			*pdwMIC_L, *pdwMIC_R);

        }

<<<<<<< HEAD
        s_vFillTxKey(pDevice, (PBYTE)(pTxBufHead->adwTxKey), pbyIVHead, pTransmitKey,
                     pbyMacHdr, (WORD)cbFrameBodySize, (PBYTE)pMICHDR);
=======
	s_vFillTxKey(pDevice, pTxBufHead, pbyIVHead, pTransmitKey,
		pbyMacHdr, (u16)cbFrameBodySize, pMICHDR);
>>>>>>> refs/remotes/origin/master

        if (pDevice->bEnableHostWEP) {
            pMgmt->sNodeDBTable[uNodeIndex].dwTSC47_16 = pTransmitKey->dwTSC47_16;
            pMgmt->sNodeDBTable[uNodeIndex].wTSC15_0 = pTransmitKey->wTSC15_0;
        }

        if ((pDevice->byLocalID <= REV_ID_VT3253_A1)) {
<<<<<<< HEAD
            s_vSWencryption(pDevice, pTransmitKey, pbyPayloadHead, (WORD)(cbFrameBodySize + cbMIClen));
        }
    }

    pMACHeader->wSeqCtl = cpu_to_le16(pDevice->wSeqCounter << 4);
=======
            s_vSWencryption(pDevice, pTransmitKey, pbyPayloadHead, (u16)(cbFrameBodySize + cbMIClen));
        }
    }

    pMACHeader->seq_ctrl = cpu_to_le16(pDevice->wSeqCounter << 4);
>>>>>>> refs/remotes/origin/master
    pDevice->wSeqCounter++ ;
    if (pDevice->wSeqCounter > 0x0fff)
        pDevice->wSeqCounter = 0;

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
    if (bIsPSPOLL) {
        // The MAC will automatically replace the Duration-field of MAC header by Duration-field
        // of  FIFO control header.
        // This will cause AID-field of PS-POLL packet be incorrect (Because PS-POLL's AID field is
        // in the same place of other packet's Duration-field).
        // And it will cause Cisco-AP to issue Disassociation-packet
<<<<<<< HEAD
        if (byPktType == PK_TYPE_11GB || byPktType == PK_TYPE_11GA) {
            ((PSTxDataHead_g)pvTxDataHd)->wDuration_a = cpu_to_le16(p80211Header->sA2.wDurationID);
            ((PSTxDataHead_g)pvTxDataHd)->wDuration_b = cpu_to_le16(p80211Header->sA2.wDurationID);
        } else {
            ((PSTxDataHead_ab)pvTxDataHd)->wDuration = cpu_to_le16(p80211Header->sA2.wDurationID);
        }
    }

    pTX_Buffer->wTxByteCount = cpu_to_le16((WORD)(cbReqCount));
    pTX_Buffer->byPKTNO = (BYTE) (((wCurrentRate<<4) &0x00F0) | ((pDevice->wSeqCounter - 1) & 0x000F));
=======
	if (byPktType == PK_TYPE_11GB || byPktType == PK_TYPE_11GA) {
		struct vnt_tx_datahead_g *data_head = &pTX_Buffer->tx_head.
						tx_cts.tx.head.cts_g.data_head;
		data_head->wDuration_a =
			cpu_to_le16(p80211Header->sA2.wDurationID);
		data_head->wDuration_b =
			cpu_to_le16(p80211Header->sA2.wDurationID);
	} else {
		struct vnt_tx_datahead_ab *data_head = &pTX_Buffer->tx_head.
					tx_ab.tx.head.data_head_ab;
		data_head->wDuration =
			cpu_to_le16(p80211Header->sA2.wDurationID);
	}
    }

    pTX_Buffer->wTxByteCount = cpu_to_le16((u16)(cbReqCount));
    pTX_Buffer->byPKTNO = (u8) (((wCurrentRate<<4) &0x00F0) | ((pDevice->wSeqCounter - 1) & 0x000F));
>>>>>>> refs/remotes/origin/master
    pTX_Buffer->byType = 0x00;

    pContext->pPacket = skb;
    pContext->Type = CONTEXT_MGMT_PACKET;
<<<<<<< HEAD
    pContext->uBufLen = (WORD)cbReqCount + 4;  //USB header

    if (WLAN_GET_FC_TODS(pMACHeader->wFrameCtl) == 0) {
        s_vSaveTxPktInfo(pDevice, (BYTE) (pTX_Buffer->byPKTNO & 0x0F), &(pMACHeader->abyAddr1[0]),(WORD)cbFrameSize,pTX_Buffer->wFIFOCtl);
    }
    else {
        s_vSaveTxPktInfo(pDevice, (BYTE) (pTX_Buffer->byPKTNO & 0x0F), &(pMACHeader->abyAddr3[0]),(WORD)cbFrameSize,pTX_Buffer->wFIFOCtl);
=======
    pContext->uBufLen = (u16)cbReqCount + 4;  //USB header

    if (WLAN_GET_FC_TODS(pMACHeader->frame_control) == 0) {
	s_vSaveTxPktInfo(pDevice, (u8)(pTX_Buffer->byPKTNO & 0x0F),
			&pMACHeader->addr1[0], (u16)cbFrameSize,
			pTxBufHead->wFIFOCtl);
    }
    else {
	s_vSaveTxPktInfo(pDevice, (u8)(pTX_Buffer->byPKTNO & 0x0F),
			&pMACHeader->addr3[0], (u16)cbFrameSize,
			pTxBufHead->wFIFOCtl);
>>>>>>> refs/remotes/origin/master
    }
    PIPEnsSendBulkOut(pDevice,pContext);
    return ;

}

<<<<<<< HEAD



=======
>>>>>>> refs/remotes/origin/master
//TYPE_AC0DMA data tx
/*
 * Description:
 *      Tx packet via AC0DMA(DMA1)
 *
 * Parameters:
 *  In:
 *      pDevice         - Pointer to the adapter
 *      skb             - Pointer to tx skb packet
 *  Out:
 *      void
 *
 * Return Value: NULL
 */

<<<<<<< HEAD
int nsDMA_tx_packet(PSDevice pDevice, unsigned int uDMAIdx, struct sk_buff *skb)
{
    PSMgmtObject    pMgmt = &(pDevice->sMgmtObj);
    unsigned int BytesToWrite = 0, uHeaderLen = 0;
    unsigned int            uNodeIndex = 0;
    BYTE            byMask[8] = {1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80};
    WORD            wAID;
    BYTE            byPktType;
    BOOL            bNeedEncryption = FALSE;
    PSKeyItem       pTransmitKey = NULL;
    SKeyItem        STempKey;
    unsigned int            ii;
    BOOL            bTKIP_UseGTK = FALSE;
    BOOL            bNeedDeAuth = FALSE;
    PBYTE           pbyBSSID;
    BOOL            bNodeExist = FALSE;
    PUSB_SEND_CONTEXT pContext;
    BOOL            fConvertedPacket;
    PTX_BUFFER      pTX_Buffer;
    unsigned int            status;
    WORD            wKeepRate = pDevice->wCurrentRate;
    struct net_device_stats* pStats = &pDevice->stats;
     BOOL            bTxeapol_key = FALSE;

=======
int nsDMA_tx_packet(struct vnt_private *pDevice,
	u32 uDMAIdx, struct sk_buff *skb)
{
	struct net_device_stats *pStats = &pDevice->stats;
	struct vnt_manager *pMgmt = &pDevice->vnt_mgmt;
	struct vnt_tx_buffer *pTX_Buffer;
	u32 BytesToWrite = 0, uHeaderLen = 0;
	u32 uNodeIndex = 0;
	u8 byMask[8] = {1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80};
	u16 wAID;
	u8 byPktType;
	int bNeedEncryption = false;
	PSKeyItem pTransmitKey = NULL;
	SKeyItem STempKey;
	int ii;
	int bTKIP_UseGTK = false;
	int bNeedDeAuth = false;
	u8 *pbyBSSID;
	int bNodeExist = false;
	struct vnt_usb_send_context *pContext;
	bool fConvertedPacket;
	u32 status;
	u16 wKeepRate = pDevice->wCurrentRate;
	int bTxeapol_key = false;
>>>>>>> refs/remotes/origin/master

    if (pMgmt->eCurrMode == WMAC_MODE_ESS_AP) {

        if (pDevice->uAssocCount == 0) {
            dev_kfree_skb_irq(skb);
            return 0;
        }

<<<<<<< HEAD
	if (is_multicast_ether_addr((PBYTE)(skb->data))) {
            uNodeIndex = 0;
            bNodeExist = TRUE;
=======
	if (is_multicast_ether_addr((u8 *)(skb->data))) {
            uNodeIndex = 0;
            bNodeExist = true;
>>>>>>> refs/remotes/origin/master
            if (pMgmt->sNodeDBTable[0].bPSEnable) {

                skb_queue_tail(&(pMgmt->sNodeDBTable[0].sTxPSQueue), skb);
                pMgmt->sNodeDBTable[0].wEnQueueCnt++;
                // set tx map
                pMgmt->abyPSTxMap[0] |= byMask[0];
                return 0;
            }
<<<<<<< HEAD
            // muticast/broadcast data rate
=======
            // multicast/broadcast data rate
>>>>>>> refs/remotes/origin/master

            if (pDevice->byBBType != BB_TYPE_11A)
                pDevice->wCurrentRate = RATE_2M;
            else
                pDevice->wCurrentRate = RATE_24M;
            // long preamble type
            pDevice->byPreambleType = PREAMBLE_SHORT;

        }else {

<<<<<<< HEAD
            if (BSSbIsSTAInNodeDB(pDevice, (PBYTE)(skb->data), &uNodeIndex)) {
=======
            if (BSSbIsSTAInNodeDB(pDevice, (u8 *)(skb->data), &uNodeIndex)) {
>>>>>>> refs/remotes/origin/master

                if (pMgmt->sNodeDBTable[uNodeIndex].bPSEnable) {

                    skb_queue_tail(&pMgmt->sNodeDBTable[uNodeIndex].sTxPSQueue, skb);

                    pMgmt->sNodeDBTable[uNodeIndex].wEnQueueCnt++;
                    // set tx map
                    wAID = pMgmt->sNodeDBTable[uNodeIndex].wAID;
                    pMgmt->abyPSTxMap[wAID >> 3] |=  byMask[wAID & 7];
                    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Set:pMgmt->abyPSTxMap[%d]= %d\n",
                             (wAID >> 3), pMgmt->abyPSTxMap[wAID >> 3]);

                    return 0;
                }
                // AP rate decided from node
                pDevice->wCurrentRate = pMgmt->sNodeDBTable[uNodeIndex].wTxDataRate;
                // tx preamble decided from node

                if (pMgmt->sNodeDBTable[uNodeIndex].bShortPreamble) {
                    pDevice->byPreambleType = pDevice->byShortPreamble;

                }else {
                    pDevice->byPreambleType = PREAMBLE_LONG;
                }
<<<<<<< HEAD
                bNodeExist = TRUE;
            }
        }

        if (bNodeExist == FALSE) {
=======
                bNodeExist = true;
            }
        }

        if (bNodeExist == false) {
>>>>>>> refs/remotes/origin/master
            DBG_PRT(MSG_LEVEL_DEBUG, KERN_DEBUG"Unknown STA not found in node DB \n");
            dev_kfree_skb_irq(skb);
            return 0;
        }
    }

<<<<<<< HEAD
    pContext = (PUSB_SEND_CONTEXT)s_vGetFreeContext(pDevice);
=======
	pContext = s_vGetFreeContext(pDevice);
>>>>>>> refs/remotes/origin/master

    if (pContext == NULL) {
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_DEBUG" pContext == NULL\n");
        dev_kfree_skb_irq(skb);
        return STATUS_RESOURCES;
    }

<<<<<<< HEAD
    memcpy(pDevice->sTxEthHeader.abyDstAddr, (PBYTE)(skb->data), ETH_HLEN);

//mike add:station mode check eapol-key challenge--->
{
    BYTE  Protocol_Version;    //802.1x Authentication
    BYTE  Packet_Type;           //802.1x Authentication
    BYTE  Descriptor_type;
    WORD Key_info;
=======
    memcpy(pDevice->sTxEthHeader.h_dest, (u8 *)(skb->data), ETH_HLEN);

//mike add:station mode check eapol-key challenge--->
{
    u8  Protocol_Version;    //802.1x Authentication
    u8  Packet_Type;           //802.1x Authentication
    u8  Descriptor_type;
    u16 Key_info;
>>>>>>> refs/remotes/origin/master

    Protocol_Version = skb->data[ETH_HLEN];
    Packet_Type = skb->data[ETH_HLEN+1];
    Descriptor_type = skb->data[ETH_HLEN+1+1+2];
    Key_info = (skb->data[ETH_HLEN+1+1+2+1] << 8)|(skb->data[ETH_HLEN+1+1+2+2]);
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (pDevice->sTxEthHeader.wType == cpu_to_be16(ETH_P_PAE)) {
		/* 802.1x OR eapol-key challenge frame transfer */
		if (((Protocol_Version == 1) || (Protocol_Version == 2)) &&
			(Packet_Type == 3)) {
                        bTxeapol_key = TRUE;
                       if(!(Key_info & BIT3) &&  //WPA or RSN group-key challenge
			   (Key_info & BIT8) && (Key_info & BIT9)) {    //send 2/2 key
			  if(Descriptor_type==254) {
                               pDevice->fWPA_Authened = TRUE;
			     PRINT_K("WPA ");
			  }
			  else {
                               pDevice->fWPA_Authened = TRUE;
=======
	if (pDevice->sTxEthHeader.h_proto == cpu_to_be16(ETH_P_PAE)) {
		/* 802.1x OR eapol-key challenge frame transfer */
		if (((Protocol_Version == 1) || (Protocol_Version == 2)) &&
			(Packet_Type == 3)) {
                        bTxeapol_key = true;
                       if(!(Key_info & BIT3) &&  //WPA or RSN group-key challenge
			   (Key_info & BIT8) && (Key_info & BIT9)) {    //send 2/2 key
			  if(Descriptor_type==254) {
                               pDevice->fWPA_Authened = true;
			     PRINT_K("WPA ");
			  }
			  else {
                               pDevice->fWPA_Authened = true;
>>>>>>> refs/remotes/origin/master
			     PRINT_K("WPA2(re-keying) ");
			  }
			  PRINT_K("Authentication completed!!\n");
                        }
<<<<<<< HEAD
		    else if((Key_info & BIT3) && (Descriptor_type==2) &&  //RSN pairse-key challenge
			       (Key_info & BIT8) && (Key_info & BIT9)) {
			  pDevice->fWPA_Authened = TRUE;
=======
		    else if((Key_info & BIT3) && (Descriptor_type==2) &&  //RSN pairwise-key challenge
			       (Key_info & BIT8) && (Key_info & BIT9)) {
			  pDevice->fWPA_Authened = true;
>>>>>>> refs/remotes/origin/master
                            PRINT_K("WPA2 Authentication completed!!\n");
		     }
             }
   }
}
//mike add:station mode check eapol-key challenge<---

<<<<<<< HEAD
    if (pDevice->bEncryptionEnable == TRUE) {
        bNeedEncryption = TRUE;
=======
    if (pDevice->bEncryptionEnable == true) {
        bNeedEncryption = true;
>>>>>>> refs/remotes/origin/master
        // get Transmit key
        do {
            if ((pMgmt->eCurrMode == WMAC_MODE_ESS_STA) &&
                (pMgmt->eCurrState == WMAC_STATE_ASSOC)) {
                pbyBSSID = pDevice->abyBSSID;
                // get pairwise key
<<<<<<< HEAD
                if (KeybGetTransmitKey(&(pDevice->sKey), pbyBSSID, PAIRWISE_KEY, &pTransmitKey) == FALSE) {
                    // get group key
                    if(KeybGetTransmitKey(&(pDevice->sKey), pbyBSSID, GROUP_KEY, &pTransmitKey) == TRUE) {
                        bTKIP_UseGTK = TRUE;
=======
                if (KeybGetTransmitKey(&(pDevice->sKey), pbyBSSID, PAIRWISE_KEY, &pTransmitKey) == false) {
                    // get group key
                    if(KeybGetTransmitKey(&(pDevice->sKey), pbyBSSID, GROUP_KEY, &pTransmitKey) == true) {
                        bTKIP_UseGTK = true;
>>>>>>> refs/remotes/origin/master
                        DBG_PRT(MSG_LEVEL_DEBUG, KERN_DEBUG"Get GTK.\n");
                        break;
                    }
                } else {
                    DBG_PRT(MSG_LEVEL_DEBUG, KERN_DEBUG"Get PTK.\n");
                    break;
                }
            }else if (pMgmt->eCurrMode == WMAC_MODE_IBSS_STA) {
<<<<<<< HEAD

                pbyBSSID = pDevice->sTxEthHeader.abyDstAddr;  //TO_DS = 0 and FROM_DS = 0 --> 802.11 MAC Address1
=======
	      /* TO_DS = 0 and FROM_DS = 0 --> 802.11 MAC Address1 */
                pbyBSSID = pDevice->sTxEthHeader.h_dest;
>>>>>>> refs/remotes/origin/master
                DBG_PRT(MSG_LEVEL_DEBUG, KERN_DEBUG"IBSS Serach Key: \n");
                for (ii = 0; ii< 6; ii++)
                    DBG_PRT(MSG_LEVEL_DEBUG, KERN_DEBUG"%x \n", *(pbyBSSID+ii));
                DBG_PRT(MSG_LEVEL_DEBUG, KERN_DEBUG"\n");

                // get pairwise key
<<<<<<< HEAD
                if(KeybGetTransmitKey(&(pDevice->sKey), pbyBSSID, PAIRWISE_KEY, &pTransmitKey) == TRUE)
=======
                if(KeybGetTransmitKey(&(pDevice->sKey), pbyBSSID, PAIRWISE_KEY, &pTransmitKey) == true)
>>>>>>> refs/remotes/origin/master
                    break;
            }
            // get group key
            pbyBSSID = pDevice->abyBroadcastAddr;
<<<<<<< HEAD
            if(KeybGetTransmitKey(&(pDevice->sKey), pbyBSSID, GROUP_KEY, &pTransmitKey) == FALSE) {
=======
            if(KeybGetTransmitKey(&(pDevice->sKey), pbyBSSID, GROUP_KEY, &pTransmitKey) == false) {
>>>>>>> refs/remotes/origin/master
                pTransmitKey = NULL;
                if (pMgmt->eCurrMode == WMAC_MODE_IBSS_STA) {
                    DBG_PRT(MSG_LEVEL_DEBUG, KERN_DEBUG"IBSS and KEY is NULL. [%d]\n", pMgmt->eCurrMode);
                }
                else
                    DBG_PRT(MSG_LEVEL_DEBUG, KERN_DEBUG"NOT IBSS and KEY is NULL. [%d]\n", pMgmt->eCurrMode);
            } else {
<<<<<<< HEAD
                bTKIP_UseGTK = TRUE;
                DBG_PRT(MSG_LEVEL_DEBUG, KERN_DEBUG"Get GTK.\n");
            }
        } while(FALSE);
=======
                bTKIP_UseGTK = true;
                DBG_PRT(MSG_LEVEL_DEBUG, KERN_DEBUG"Get GTK.\n");
            }
        } while(false);
>>>>>>> refs/remotes/origin/master
    }

    if (pDevice->bEnableHostWEP) {
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_DEBUG"acdma0: STA index %d\n", uNodeIndex);
<<<<<<< HEAD
        if (pDevice->bEncryptionEnable == TRUE) {
=======
        if (pDevice->bEncryptionEnable == true) {
>>>>>>> refs/remotes/origin/master
            pTransmitKey = &STempKey;
            pTransmitKey->byCipherSuite = pMgmt->sNodeDBTable[uNodeIndex].byCipherSuite;
            pTransmitKey->dwKeyIndex = pMgmt->sNodeDBTable[uNodeIndex].dwKeyIndex;
            pTransmitKey->uKeyLength = pMgmt->sNodeDBTable[uNodeIndex].uWepKeyLength;
            pTransmitKey->dwTSC47_16 = pMgmt->sNodeDBTable[uNodeIndex].dwTSC47_16;
            pTransmitKey->wTSC15_0 = pMgmt->sNodeDBTable[uNodeIndex].wTSC15_0;
            memcpy(pTransmitKey->abyKey,
                &pMgmt->sNodeDBTable[uNodeIndex].abyWepKey[0],
                pTransmitKey->uKeyLength
                );
         }
    }

<<<<<<< HEAD
    byPktType = (BYTE)pDevice->byPacketType;
=======
    byPktType = (u8)pDevice->byPacketType;
>>>>>>> refs/remotes/origin/master

    if (pDevice->bFixRate) {
        if (pDevice->byBBType == BB_TYPE_11B) {
            if (pDevice->uConnectionRate >= RATE_11M) {
                pDevice->wCurrentRate = RATE_11M;
            } else {
<<<<<<< HEAD
                pDevice->wCurrentRate = (WORD)pDevice->uConnectionRate;
=======
                pDevice->wCurrentRate = (u16)pDevice->uConnectionRate;
>>>>>>> refs/remotes/origin/master
            }
        } else {
            if ((pDevice->byBBType == BB_TYPE_11A) &&
                (pDevice->uConnectionRate <= RATE_6M)) {
                pDevice->wCurrentRate = RATE_6M;
            } else {
                if (pDevice->uConnectionRate >= RATE_54M)
                    pDevice->wCurrentRate = RATE_54M;
                else
<<<<<<< HEAD
                    pDevice->wCurrentRate = (WORD)pDevice->uConnectionRate;
=======
                    pDevice->wCurrentRate = (u16)pDevice->uConnectionRate;
>>>>>>> refs/remotes/origin/master
            }
        }
    }
    else {
        if (pDevice->eOPMode == OP_MODE_ADHOC) {
            // Adhoc Tx rate decided from node DB
<<<<<<< HEAD
	    if (is_multicast_ether_addr(pDevice->sTxEthHeader.abyDstAddr)) {
=======
	    if (is_multicast_ether_addr(pDevice->sTxEthHeader.h_dest)) {
>>>>>>> refs/remotes/origin/master
                // Multicast use highest data rate
                pDevice->wCurrentRate = pMgmt->sNodeDBTable[0].wTxDataRate;
                // preamble type
                pDevice->byPreambleType = pDevice->byShortPreamble;
            }
            else {
<<<<<<< HEAD
                if(BSSbIsSTAInNodeDB(pDevice, &(pDevice->sTxEthHeader.abyDstAddr[0]), &uNodeIndex)) {
=======
                if (BSSbIsSTAInNodeDB(pDevice, &(pDevice->sTxEthHeader.h_dest[0]), &uNodeIndex)) {
>>>>>>> refs/remotes/origin/master
                    pDevice->wCurrentRate = pMgmt->sNodeDBTable[uNodeIndex].wTxDataRate;
                    if (pMgmt->sNodeDBTable[uNodeIndex].bShortPreamble) {
                        pDevice->byPreambleType = pDevice->byShortPreamble;

                    }
                    else {
                        pDevice->byPreambleType = PREAMBLE_LONG;
                    }
                    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Found Node Index is [%d]  Tx Data Rate:[%d]\n",uNodeIndex, pDevice->wCurrentRate);
                }
                else {
                    if (pDevice->byBBType != BB_TYPE_11A)
                       pDevice->wCurrentRate = RATE_2M;
                    else
                       pDevice->wCurrentRate = RATE_24M; // refer to vMgrCreateOwnIBSS()'s
                                                         // abyCurrExtSuppRates[]
                    pDevice->byPreambleType = PREAMBLE_SHORT;
                    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Not Found Node use highest basic Rate.....\n");
                }
            }
        }
        if (pDevice->eOPMode == OP_MODE_INFRASTRUCTURE) {
            // Infra STA rate decided from AP Node, index = 0
            pDevice->wCurrentRate = pMgmt->sNodeDBTable[0].wTxDataRate;
        }
    }

<<<<<<< HEAD
<<<<<<< HEAD
	if (pDevice->sTxEthHeader.wType == cpu_to_be16(ETH_P_PAE)) {
=======
	if (pDevice->sTxEthHeader.h_proto == cpu_to_be16(ETH_P_PAE)) {
>>>>>>> refs/remotes/origin/master
=======
	if (pDevice->sTxEthHeader.wType == cpu_to_be16(ETH_P_PAE)) {
>>>>>>> refs/remotes/origin/cm-11.0
		if (pDevice->byBBType != BB_TYPE_11A) {
			pDevice->wCurrentRate = RATE_1M;
			pDevice->byACKRate = RATE_1M;
			pDevice->byTopCCKBasicRate = RATE_1M;
			pDevice->byTopOFDMBasicRate = RATE_6M;
		} else {
			pDevice->wCurrentRate = RATE_6M;
			pDevice->byACKRate = RATE_6M;
			pDevice->byTopCCKBasicRate = RATE_1M;
			pDevice->byTopOFDMBasicRate = RATE_6M;
		}
	}

    DBG_PRT(MSG_LEVEL_DEBUG,
	    KERN_INFO "dma_tx: pDevice->wCurrentRate = %d\n",
	    pDevice->wCurrentRate);

    if (wKeepRate != pDevice->wCurrentRate) {
	bScheduleCommand((void *) pDevice, WLAN_CMD_SETPOWER, NULL);
    }

    if (pDevice->wCurrentRate <= RATE_11M) {
        byPktType = PK_TYPE_11B;
    }

<<<<<<< HEAD
    if (bNeedEncryption == TRUE) {
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"ntohs Pkt Type=%04x\n", ntohs(pDevice->sTxEthHeader.wType));
	if ((pDevice->sTxEthHeader.wType) == cpu_to_be16(ETH_P_PAE)) {
		bNeedEncryption = FALSE;
            DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Pkt Type=%04x\n", (pDevice->sTxEthHeader.wType));
=======
    if (bNeedEncryption == true) {
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"ntohs Pkt Type=%04x\n", ntohs(pDevice->sTxEthHeader.h_proto));
	if ((pDevice->sTxEthHeader.h_proto) == cpu_to_be16(ETH_P_PAE)) {
		bNeedEncryption = false;
            DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Pkt Type=%04x\n", (pDevice->sTxEthHeader.h_proto));
>>>>>>> refs/remotes/origin/master
            if ((pMgmt->eCurrMode == WMAC_MODE_ESS_STA) && (pMgmt->eCurrState == WMAC_STATE_ASSOC)) {
                if (pTransmitKey == NULL) {
                    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Don't Find TX KEY\n");
                }
                else {
<<<<<<< HEAD
                    if (bTKIP_UseGTK == TRUE) {
=======
                    if (bTKIP_UseGTK == true) {
>>>>>>> refs/remotes/origin/master
                        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"error: KEY is GTK!!~~\n");
                    }
                    else {
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Find PTK [%X]\n",
				pTransmitKey->dwKeyIndex);
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
                        bNeedEncryption = TRUE;
=======
                        bNeedEncryption = true;
>>>>>>> refs/remotes/origin/master
                    }
                }
            }

<<<<<<< HEAD
            if (pDevice->byCntMeasure == 2) {
                bNeedDeAuth = TRUE;
                pDevice->s802_11Counter.TKIPCounterMeasuresInvoked++;
            }

=======
>>>>>>> refs/remotes/origin/master
            if (pDevice->bEnableHostWEP) {
                if ((uNodeIndex != 0) &&
                    (pMgmt->sNodeDBTable[uNodeIndex].dwKeyIndex & PAIRWISE_KEY)) {
			DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Find PTK [%X]\n",
				pTransmitKey->dwKeyIndex);
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
                    bNeedEncryption = TRUE;
=======
                    bNeedEncryption = true;
>>>>>>> refs/remotes/origin/master
                 }
             }
        }
        else {

            if (pTransmitKey == NULL) {
                DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"return no tx key\n");
<<<<<<< HEAD
=======
		pContext->bBoolInUse = false;
>>>>>>> refs/remotes/origin/master
                dev_kfree_skb_irq(skb);
                pStats->tx_dropped++;
                return STATUS_FAILURE;
            }
        }
    }

<<<<<<< HEAD
    fConvertedPacket = s_bPacketToWirelessUsb(pDevice, byPktType,
                        (PBYTE)(&pContext->Data[0]), bNeedEncryption,
                        skb->len, uDMAIdx, &pDevice->sTxEthHeader,
                        (PBYTE)skb->data, pTransmitKey, uNodeIndex,
=======
	pTX_Buffer = (struct vnt_tx_buffer *)&pContext->Data[0];

    fConvertedPacket = s_bPacketToWirelessUsb(pDevice, byPktType,
			pTX_Buffer, bNeedEncryption,
                        skb->len, uDMAIdx, &pDevice->sTxEthHeader,
                        (u8 *)skb->data, pTransmitKey, uNodeIndex,
>>>>>>> refs/remotes/origin/master
                        pDevice->wCurrentRate,
                        &uHeaderLen, &BytesToWrite
                       );

<<<<<<< HEAD
    if (fConvertedPacket == FALSE) {
        pContext->bBoolInUse = FALSE;
=======
    if (fConvertedPacket == false) {
        pContext->bBoolInUse = false;
>>>>>>> refs/remotes/origin/master
        dev_kfree_skb_irq(skb);
        return STATUS_FAILURE;
    }

<<<<<<< HEAD
    if ( pDevice->bEnablePSMode == TRUE ) {
=======
    if ( pDevice->bEnablePSMode == true ) {
>>>>>>> refs/remotes/origin/master
        if ( !pDevice->bPSModeTxBurst ) {
		bScheduleCommand((void *) pDevice,
				 WLAN_CMD_MAC_DISPOWERSAVING,
				 NULL);
<<<<<<< HEAD
            pDevice->bPSModeTxBurst = TRUE;
        }
    }

    pTX_Buffer = (PTX_BUFFER)&(pContext->Data[0]);
    pTX_Buffer->byPKTNO = (BYTE) (((pDevice->wCurrentRate<<4) &0x00F0) | ((pDevice->wSeqCounter - 1) & 0x000F));
    pTX_Buffer->wTxByteCount = (WORD)BytesToWrite;

    pContext->pPacket = skb;
    pContext->Type = CONTEXT_DATA_PACKET;
    pContext->uBufLen = (WORD)BytesToWrite + 4 ; //USB header

    s_vSaveTxPktInfo(pDevice, (BYTE) (pTX_Buffer->byPKTNO & 0x0F), &(pContext->sEthHeader.abyDstAddr[0]),(WORD) (BytesToWrite-uHeaderLen),pTX_Buffer->wFIFOCtl);

    status = PIPEnsSendBulkOut(pDevice,pContext);

    if (bNeedDeAuth == TRUE) {
        WORD wReason = WLAN_MGMT_REASON_MIC_FAILURE;

	bScheduleCommand((void *) pDevice, WLAN_CMD_DEAUTH, (PBYTE) &wReason);
    }

  if(status!=STATUS_PENDING) {
     pContext->bBoolInUse = FALSE;
=======
            pDevice->bPSModeTxBurst = true;
        }
    }

    pTX_Buffer->byPKTNO = (u8) (((pDevice->wCurrentRate<<4) &0x00F0) | ((pDevice->wSeqCounter - 1) & 0x000F));
    pTX_Buffer->wTxByteCount = (u16)BytesToWrite;

    pContext->pPacket = skb;
    pContext->Type = CONTEXT_DATA_PACKET;
    pContext->uBufLen = (u16)BytesToWrite + 4 ; //USB header

    s_vSaveTxPktInfo(pDevice, (u8)(pTX_Buffer->byPKTNO & 0x0F),
			&pContext->sEthHeader.h_dest[0],
			(u16)(BytesToWrite-uHeaderLen),
			pTX_Buffer->fifo_head.wFIFOCtl);

    status = PIPEnsSendBulkOut(pDevice,pContext);

    if (bNeedDeAuth == true) {
        u16 wReason = WLAN_MGMT_REASON_MIC_FAILURE;

	bScheduleCommand((void *) pDevice, WLAN_CMD_DEAUTH, (u8 *) &wReason);
    }

  if(status!=STATUS_PENDING) {
     pContext->bBoolInUse = false;
>>>>>>> refs/remotes/origin/master
    dev_kfree_skb_irq(skb);
    return STATUS_FAILURE;
  }
  else
    return 0;

}

<<<<<<< HEAD


=======
>>>>>>> refs/remotes/origin/master
/*
 * Description:
 *      Relay packet send (AC1DMA) from rx dpc.
 *
 * Parameters:
 *  In:
 *      pDevice         - Pointer to the adapter
 *      pPacket         - Pointer to rx packet
 *      cbPacketSize    - rx ethernet frame size
 *  Out:
<<<<<<< HEAD
 *      TURE, FALSE
 *
 * Return Value: Return TRUE if packet is copy to dma1; otherwise FALSE
 */


BOOL
bRelayPacketSend (
      PSDevice pDevice,
      PBYTE    pbySkbData,
      unsigned int     uDataLen,
      unsigned int     uNodeIndex
    )
{
    PSMgmtObject    pMgmt = &(pDevice->sMgmtObj);
    unsigned int BytesToWrite = 0, uHeaderLen = 0;
    BYTE            byPktType = PK_TYPE_11B;
    BOOL            bNeedEncryption = FALSE;
    SKeyItem        STempKey;
    PSKeyItem       pTransmitKey = NULL;
    PBYTE           pbyBSSID;
    PUSB_SEND_CONTEXT   pContext;
    BYTE            byPktTyp;
    BOOL            fConvertedPacket;
    PTX_BUFFER      pTX_Buffer;
    unsigned int            status;
    WORD            wKeepRate = pDevice->wCurrentRate;



    pContext = (PUSB_SEND_CONTEXT)s_vGetFreeContext(pDevice);

    if (NULL == pContext) {
        return FALSE;
    }

    memcpy(pDevice->sTxEthHeader.abyDstAddr, (PBYTE)pbySkbData, ETH_HLEN);

    if (pDevice->bEncryptionEnable == TRUE) {
        bNeedEncryption = TRUE;
        // get group key
        pbyBSSID = pDevice->abyBroadcastAddr;
        if(KeybGetTransmitKey(&(pDevice->sKey), pbyBSSID, GROUP_KEY, &pTransmitKey) == FALSE) {
=======
 *      TURE, false
 *
 * Return Value: Return true if packet is copy to dma1; otherwise false
 */

int bRelayPacketSend(struct vnt_private *pDevice, u8 *pbySkbData, u32 uDataLen,
	u32 uNodeIndex)
{
	struct vnt_manager *pMgmt = &pDevice->vnt_mgmt;
	struct vnt_tx_buffer *pTX_Buffer;
	u32 BytesToWrite = 0, uHeaderLen = 0;
	u8 byPktType = PK_TYPE_11B;
	int bNeedEncryption = false;
	SKeyItem STempKey;
	PSKeyItem pTransmitKey = NULL;
	u8 *pbyBSSID;
	struct vnt_usb_send_context *pContext;
	u8 byPktTyp;
	int fConvertedPacket;
	u32 status;
	u16 wKeepRate = pDevice->wCurrentRate;

	pContext = s_vGetFreeContext(pDevice);

    if (NULL == pContext) {
        return false;
    }

    memcpy(pDevice->sTxEthHeader.h_dest, (u8 *)pbySkbData, ETH_HLEN);

    if (pDevice->bEncryptionEnable == true) {
        bNeedEncryption = true;
        // get group key
        pbyBSSID = pDevice->abyBroadcastAddr;
        if(KeybGetTransmitKey(&(pDevice->sKey), pbyBSSID, GROUP_KEY, &pTransmitKey) == false) {
>>>>>>> refs/remotes/origin/master
            pTransmitKey = NULL;
            DBG_PRT(MSG_LEVEL_DEBUG, KERN_DEBUG"KEY is NULL. [%d]\n", pMgmt->eCurrMode);
        } else {
            DBG_PRT(MSG_LEVEL_DEBUG, KERN_DEBUG"Get GTK.\n");
        }
    }

    if (pDevice->bEnableHostWEP) {
        if (uNodeIndex < MAX_NODE_NUM + 1) {
            pTransmitKey = &STempKey;
            pTransmitKey->byCipherSuite = pMgmt->sNodeDBTable[uNodeIndex].byCipherSuite;
            pTransmitKey->dwKeyIndex = pMgmt->sNodeDBTable[uNodeIndex].dwKeyIndex;
            pTransmitKey->uKeyLength = pMgmt->sNodeDBTable[uNodeIndex].uWepKeyLength;
            pTransmitKey->dwTSC47_16 = pMgmt->sNodeDBTable[uNodeIndex].dwTSC47_16;
            pTransmitKey->wTSC15_0 = pMgmt->sNodeDBTable[uNodeIndex].wTSC15_0;
            memcpy(pTransmitKey->abyKey,
                    &pMgmt->sNodeDBTable[uNodeIndex].abyWepKey[0],
                    pTransmitKey->uKeyLength
                  );
        }
    }

    if ( bNeedEncryption && (pTransmitKey == NULL) ) {
<<<<<<< HEAD
        pContext->bBoolInUse = FALSE;
        return FALSE;
    }

    byPktTyp = (BYTE)pDevice->byPacketType;
=======
        pContext->bBoolInUse = false;
        return false;
    }

    byPktTyp = (u8)pDevice->byPacketType;
>>>>>>> refs/remotes/origin/master

    if (pDevice->bFixRate) {
        if (pDevice->byBBType == BB_TYPE_11B) {
            if (pDevice->uConnectionRate >= RATE_11M) {
                pDevice->wCurrentRate = RATE_11M;
            } else {
<<<<<<< HEAD
                pDevice->wCurrentRate = (WORD)pDevice->uConnectionRate;
=======
                pDevice->wCurrentRate = (u16)pDevice->uConnectionRate;
>>>>>>> refs/remotes/origin/master
            }
        } else {
            if ((pDevice->byBBType == BB_TYPE_11A) &&
                (pDevice->uConnectionRate <= RATE_6M)) {
                pDevice->wCurrentRate = RATE_6M;
            } else {
                if (pDevice->uConnectionRate >= RATE_54M)
                    pDevice->wCurrentRate = RATE_54M;
                else
<<<<<<< HEAD
                    pDevice->wCurrentRate = (WORD)pDevice->uConnectionRate;
=======
                    pDevice->wCurrentRate = (u16)pDevice->uConnectionRate;
>>>>>>> refs/remotes/origin/master
            }
        }
    }
    else {
        pDevice->wCurrentRate = pMgmt->sNodeDBTable[uNodeIndex].wTxDataRate;
    }

    if (wKeepRate != pDevice->wCurrentRate) {
	bScheduleCommand((void *) pDevice, WLAN_CMD_SETPOWER, NULL);
    }

    if (pDevice->wCurrentRate <= RATE_11M)
        byPktType = PK_TYPE_11B;

    BytesToWrite = uDataLen + ETH_FCS_LEN;

    // Convert the packet to an usb frame and copy into our buffer
    // and send the irp.

<<<<<<< HEAD
    fConvertedPacket = s_bPacketToWirelessUsb(pDevice, byPktType,
                         (PBYTE)(&pContext->Data[0]), bNeedEncryption,
=======
	pTX_Buffer = (struct vnt_tx_buffer *)&pContext->Data[0];

    fConvertedPacket = s_bPacketToWirelessUsb(pDevice, byPktType,
			pTX_Buffer, bNeedEncryption,
>>>>>>> refs/remotes/origin/master
                         uDataLen, TYPE_AC0DMA, &pDevice->sTxEthHeader,
                         pbySkbData, pTransmitKey, uNodeIndex,
                         pDevice->wCurrentRate,
                         &uHeaderLen, &BytesToWrite
                        );

<<<<<<< HEAD
    if (fConvertedPacket == FALSE) {
        pContext->bBoolInUse = FALSE;
        return FALSE;
    }

    pTX_Buffer = (PTX_BUFFER)&(pContext->Data[0]);
    pTX_Buffer->byPKTNO = (BYTE) (((pDevice->wCurrentRate<<4) &0x00F0) | ((pDevice->wSeqCounter - 1) & 0x000F));
    pTX_Buffer->wTxByteCount = (WORD)BytesToWrite;

    pContext->pPacket = NULL;
    pContext->Type = CONTEXT_DATA_PACKET;
    pContext->uBufLen = (WORD)BytesToWrite + 4 ; //USB header

    s_vSaveTxPktInfo(pDevice, (BYTE) (pTX_Buffer->byPKTNO & 0x0F), &(pContext->sEthHeader.abyDstAddr[0]),(WORD) (BytesToWrite-uHeaderLen),pTX_Buffer->wFIFOCtl);

    status = PIPEnsSendBulkOut(pDevice,pContext);

    return TRUE;
=======
    if (fConvertedPacket == false) {
        pContext->bBoolInUse = false;
        return false;
    }

    pTX_Buffer->byPKTNO = (u8) (((pDevice->wCurrentRate<<4) &0x00F0) | ((pDevice->wSeqCounter - 1) & 0x000F));
    pTX_Buffer->wTxByteCount = (u16)BytesToWrite;

    pContext->pPacket = NULL;
    pContext->Type = CONTEXT_DATA_PACKET;
    pContext->uBufLen = (u16)BytesToWrite + 4 ; //USB header

    s_vSaveTxPktInfo(pDevice, (u8)(pTX_Buffer->byPKTNO & 0x0F),
		&pContext->sEthHeader.h_dest[0],
		(u16)(BytesToWrite - uHeaderLen),
		pTX_Buffer->fifo_head.wFIFOCtl);

    status = PIPEnsSendBulkOut(pDevice,pContext);

    return true;
>>>>>>> refs/remotes/origin/master
}

