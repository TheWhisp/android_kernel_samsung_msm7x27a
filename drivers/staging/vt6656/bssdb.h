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
 * File: bssdb.h
 *
 * Purpose: Handles the Basic Service Set & Node Database functions
 *
 * Author: Lyndon Chen
 *
 * Date: July 16, 2002
 *
 */

#ifndef __BSSDB_H__
#define __BSSDB_H__

#include <linux/skbuff.h>
#include "80211hdr.h"
#include "80211mgr.h"
#include "card.h"
<<<<<<< HEAD
#include "mib.h"

/*---------------------  Export Definitions -------------------------*/
=======
>>>>>>> refs/remotes/origin/master

#define MAX_NODE_NUM             64
#define MAX_BSS_NUM              42
#define LOST_BEACON_COUNT        10   /* 10 sec, XP defined */
#define MAX_PS_TX_BUF            32   // sta max power saving tx buf
#define ADHOC_LOST_BEACON_COUNT  30   // 30 sec, beacon lost for adhoc only
#define MAX_INACTIVE_COUNT       300  // 300 sec, inactive STA node refresh

#define USE_PROTECT_PERIOD       10   // 10 sec, Use protect mode check period
#define ERP_RECOVER_COUNT        30   // 30 sec, ERP support callback check
#define BSS_CLEAR_COUNT           1

#define RSSI_STAT_COUNT          10
#define MAX_CHECK_RSSI_COUNT     8

// STA dwflags
#define WLAN_STA_AUTH            BIT0
#define WLAN_STA_ASSOC           BIT1
#define WLAN_STA_PS              BIT2
#define WLAN_STA_TIM             BIT3
// permanent; do not remove entry on expiration
#define WLAN_STA_PERM            BIT4
// If 802.1X is used, this flag is
// controlling whether STA is authorized to
// send and receive non-IEEE 802.1X frames
#define WLAN_STA_AUTHORIZED      BIT5

<<<<<<< HEAD
//#define MAX_RATE            12

#define MAX_WPA_IE_LEN      64


/*---------------------  Export Classes  ----------------------------*/

/*---------------------  Export Variables  --------------------------*/


/*---------------------  Export Types  ------------------------------*/

=======
#define MAX_WPA_IE_LEN      64

>>>>>>> refs/remotes/origin/master
//
// IEEE 802.11 Structures and definitions
//

typedef struct tagSERPObject {
<<<<<<< HEAD
    BOOL    bERPExist;
    BYTE    byERP;
} ERPObject, *PERPObject;


typedef struct tagSRSNCapObject {
    BOOL    bRSNCapExist;
    WORD    wRSNCap;
=======
    bool    bERPExist;
    u8    byERP;
} ERPObject, *PERPObject;

typedef struct tagSRSNCapObject {
    bool    bRSNCapExist;
    u16    wRSNCap;
>>>>>>> refs/remotes/origin/master
} SRSNCapObject, *PSRSNCapObject;

// BSS info(AP)
typedef struct tagKnownBSS {
    // BSS info
<<<<<<< HEAD
    BOOL            bActive;
    BYTE            abyBSSID[WLAN_BSSID_LEN];
    unsigned int            uChannel;
    BYTE            abySuppRates[WLAN_IEHDR_LEN + WLAN_RATES_MAXLEN + 1];
    BYTE            abyExtSuppRates[WLAN_IEHDR_LEN + WLAN_RATES_MAXLEN + 1];
    unsigned int            uRSSI;
    BYTE            bySQ;
    WORD            wBeaconInterval;
    WORD            wCapInfo;
    BYTE            abySSID[WLAN_IEHDR_LEN + WLAN_SSID_MAXLEN + 1];
    BYTE            byRxRate;

//    WORD            wATIMWindow;
    BYTE            byRSSIStatCnt;
=======
    bool            bActive;
    u8            abyBSSID[WLAN_BSSID_LEN];
    unsigned int            uChannel;
    u8            abySuppRates[WLAN_IEHDR_LEN + WLAN_RATES_MAXLEN + 1];
    u8            abyExtSuppRates[WLAN_IEHDR_LEN + WLAN_RATES_MAXLEN + 1];
    unsigned int            uRSSI;
    u8            bySQ;
    u16            wBeaconInterval;
    u16            wCapInfo;
    u8            abySSID[WLAN_IEHDR_LEN + WLAN_SSID_MAXLEN + 1];
    u8            byRxRate;

//    u16            wATIMWindow;
    u8            byRSSIStatCnt;
>>>>>>> refs/remotes/origin/master
    signed long            ldBmMAX;
    signed long            ldBmAverage[RSSI_STAT_COUNT];
    signed long            ldBmAverRange;
    //For any BSSID selection improvment
<<<<<<< HEAD
    BOOL            bSelected;

    //++ WPA informations
    BOOL            bWPAValid;
    BYTE            byGKType;
    BYTE            abyPKType[4];
    WORD            wPKCount;
    BYTE            abyAuthType[4];
    WORD            wAuthCount;
    BYTE            byDefaultK_as_PK;
    BYTE            byReplayIdx;
    //--

    //++ WPA2 informations
    BOOL            bWPA2Valid;
    BYTE            byCSSGK;
    WORD            wCSSPKCount;
    BYTE            abyCSSPK[4];
    WORD            wAKMSSAuthCount;
    BYTE            abyAKMSSAuthType[4];

    //++  wpactl
    BYTE            byWPAIE[MAX_WPA_IE_LEN];
    BYTE            byRSNIE[MAX_WPA_IE_LEN];
    WORD            wWPALen;
    WORD            wRSNLen;

    // Clear count
    unsigned int            uClearCount;
//    BYTE            abyIEs[WLAN_BEACON_FR_MAXLEN];
    unsigned int            uIELength;
    QWORD           qwBSSTimestamp;
    QWORD           qwLocalTSF;     // local TSF timer
=======
    bool            bSelected;

    //++ WPA informations
    bool            bWPAValid;
    u8            byGKType;
    u8            abyPKType[4];
    u16            wPKCount;
    u8            abyAuthType[4];
    u16            wAuthCount;
    u8            byDefaultK_as_PK;
    u8            byReplayIdx;
    //--

    //++ WPA2 informations
    bool            bWPA2Valid;
    u8            byCSSGK;
    u16            wCSSPKCount;
    u8            abyCSSPK[4];
    u16            wAKMSSAuthCount;
    u8            abyAKMSSAuthType[4];

    //++  wpactl
    u8            byWPAIE[MAX_WPA_IE_LEN];
    u8            byRSNIE[MAX_WPA_IE_LEN];
    u16            wWPALen;
    u16            wRSNLen;

    // Clear count
    unsigned int            uClearCount;
//    u8            abyIEs[WLAN_BEACON_FR_MAXLEN];
    unsigned int            uIELength;
	u64 qwBSSTimestamp;
	u64 qwLocalTSF;/* local TSF timer */
>>>>>>> refs/remotes/origin/master

    CARD_PHY_TYPE   eNetworkTypeInUse;

    ERPObject       sERP;
    SRSNCapObject   sRSNCapObj;
<<<<<<< HEAD
    BYTE            abyIEs[1024];   // don't move this field !!
=======
    u8            abyIEs[1024];   // don't move this field !!
>>>>>>> refs/remotes/origin/master

} __attribute__ ((__packed__))
KnownBSS , *PKnownBSS;

<<<<<<< HEAD


=======
>>>>>>> refs/remotes/origin/master
typedef enum tagNODE_STATE {
    NODE_FREE,
    NODE_AGED,
    NODE_KNOWN,
    NODE_AUTH,
    NODE_ASSOC
} NODE_STATE, *PNODE_STATE;

<<<<<<< HEAD

// STA node info
typedef struct tagKnownNodeDB {
    // STA info
    BOOL            bActive;
    BYTE            abyMACAddr[WLAN_ADDR_LEN];
    BYTE            abyCurrSuppRates[WLAN_IEHDR_LEN + WLAN_RATES_MAXLEN];
    BYTE            abyCurrExtSuppRates[WLAN_IEHDR_LEN + WLAN_RATES_MAXLEN];
    WORD            wTxDataRate;
    BOOL            bShortPreamble;
    BOOL            bERPExist;
    BOOL            bShortSlotTime;
    unsigned int            uInActiveCount;
    WORD            wMaxBasicRate;     //Get from byTopOFDMBasicRate or byTopCCKBasicRate which depends on packetTyp.
    WORD            wMaxSuppRate;      //Records the highest supported rate getting from SuppRates IE and ExtSuppRates IE in Beacon.
    WORD            wSuppRate;
    BYTE            byTopOFDMBasicRate;//Records the highest basic rate in OFDM mode
    BYTE            byTopCCKBasicRate; //Records the highest basic rate in CCK mode

    // For AP mode
    struct sk_buff_head sTxPSQueue;
    WORD            wCapInfo;
    WORD            wListenInterval;
    WORD            wAID;
    NODE_STATE      eNodeState;
    BOOL            bPSEnable;
    BOOL            bRxPSPoll;
    BYTE            byAuthSequence;
    unsigned long           ulLastRxJiffer;
    BYTE            bySuppRate;
    DWORD           dwFlags;
    WORD            wEnQueueCnt;

    BOOL            bOnFly;
    unsigned long long       KeyRSC;
    BYTE            byKeyIndex;
    DWORD           dwKeyIndex;
    BYTE            byCipherSuite;
    DWORD           dwTSC47_16;
    WORD            wTSC15_0;
    unsigned int            uWepKeyLength;
    BYTE            abyWepKey[WLAN_WEPMAX_KEYLEN];
    //
    // Auto rate fallback vars
    BOOL            bIsInFallback;
=======
// STA node info
typedef struct tagKnownNodeDB {
    // STA info
    bool            bActive;
    u8            abyMACAddr[WLAN_ADDR_LEN];
    u8            abyCurrSuppRates[WLAN_IEHDR_LEN + WLAN_RATES_MAXLEN];
    u8            abyCurrExtSuppRates[WLAN_IEHDR_LEN + WLAN_RATES_MAXLEN];
    u16            wTxDataRate;
    bool            bShortPreamble;
    bool            bERPExist;
    bool            bShortSlotTime;
    unsigned int            uInActiveCount;
    u16            wMaxBasicRate;     //Get from byTopOFDMBasicRate or byTopCCKBasicRate which depends on packetTyp.
    u16            wMaxSuppRate;      //Records the highest supported rate getting from SuppRates IE and ExtSuppRates IE in Beacon.
    u16            wSuppRate;
    u8            byTopOFDMBasicRate;//Records the highest basic rate in OFDM mode
    u8            byTopCCKBasicRate; //Records the highest basic rate in CCK mode

    // For AP mode
    struct sk_buff_head sTxPSQueue;
    u16            wCapInfo;
    u16            wListenInterval;
    u16            wAID;
    NODE_STATE      eNodeState;
    bool            bPSEnable;
    bool            bRxPSPoll;
    u8            byAuthSequence;
    unsigned long           ulLastRxJiffer;
    u8            bySuppRate;
    u32           dwFlags;
    u16            wEnQueueCnt;

    bool            bOnFly;
    unsigned long long       KeyRSC;
    u8            byKeyIndex;
    u32           dwKeyIndex;
    u8            byCipherSuite;
    u32           dwTSC47_16;
    u16            wTSC15_0;
    unsigned int            uWepKeyLength;
    u8            abyWepKey[WLAN_WEPMAX_KEYLEN];
    //
    // Auto rate fallback vars
    bool            bIsInFallback;
>>>>>>> refs/remotes/origin/master
    unsigned int            uAverageRSSI;
    unsigned int            uRateRecoveryTimeout;
    unsigned int            uRatePollTimeout;
    unsigned int            uTxFailures;
    unsigned int            uTxAttempts;

    unsigned int            uTxRetry;
    unsigned int            uFailureRatio;
    unsigned int            uRetryRatio;
    unsigned int            uTxOk[MAX_RATE+1];
    unsigned int            uTxFail[MAX_RATE+1];
    unsigned int            uTimeCount;

} KnownNodeDB, *PKnownNodeDB;

<<<<<<< HEAD
/*---------------------  Export Functions  --------------------------*/

PKnownBSS BSSpSearchBSSList(void *hDeviceContext,
			    PBYTE pbyDesireBSSID,
			    PBYTE pbyDesireSSID,
			    CARD_PHY_TYPE ePhyType);

PKnownBSS BSSpAddrIsInBSSList(void *hDeviceContext,
			      PBYTE abyBSSID,
			      PWLAN_IE_SSID pSSID);

void BSSvClearBSSList(void *hDeviceContext, BOOL bKeepCurrBSSID);

BOOL BSSbInsertToBSSList(void *hDeviceContext,
			 PBYTE abyBSSIDAddr,
			 QWORD qwTimestamp,
			 WORD wBeaconInterval,
			 WORD wCapInfo,
			 BYTE byCurrChannel,
			 PWLAN_IE_SSID pSSID,
			 PWLAN_IE_SUPP_RATES pSuppRates,
			 PWLAN_IE_SUPP_RATES pExtSuppRates,
			 PERPObject psERP,
			 PWLAN_IE_RSN pRSN,
			 PWLAN_IE_RSN_EXT pRSNWPA,
			 PWLAN_IE_COUNTRY pIE_Country,
			 PWLAN_IE_QUIET pIE_Quiet,
			 unsigned int uIELength,
			 PBYTE pbyIEs,
			 void *pRxPacketContext);

BOOL BSSbUpdateToBSSList(void *hDeviceContext,
			 QWORD qwTimestamp,
			 WORD wBeaconInterval,
			 WORD wCapInfo,
			 BYTE byCurrChannel,
			 BOOL bChannelHit,
			 PWLAN_IE_SSID pSSID,
			 PWLAN_IE_SUPP_RATES pSuppRates,
			 PWLAN_IE_SUPP_RATES pExtSuppRates,
			 PERPObject psERP,
			 PWLAN_IE_RSN pRSN,
			 PWLAN_IE_RSN_EXT pRSNWPA,
			 PWLAN_IE_COUNTRY pIE_Country,
			 PWLAN_IE_QUIET pIE_Quiet,
			 PKnownBSS pBSSList,
			 unsigned int uIELength,
			 PBYTE pbyIEs,
			 void *pRxPacketContext);

BOOL BSSbIsSTAInNodeDB(void *hDeviceContext,
		       PBYTE abyDstAddr,
		       unsigned int *puNodeIndex);

void BSSvCreateOneNode(void *hDeviceContext, unsigned int *puNodeIndex);

void BSSvUpdateAPNode(void *hDeviceContext,
		      PWORD pwCapInfo,
		      PWLAN_IE_SUPP_RATES pItemRates,
		      PWLAN_IE_SUPP_RATES pExtSuppRates);

void BSSvSecondCallBack(void *hDeviceContext);

void BSSvUpdateNodeTxCounter(void *hDeviceContext,
			     PSStatCounter pStatistic,
			     BYTE byTSR,
			     BYTE byPktNO);

void BSSvRemoveOneNode(void *hDeviceContext,
		       unsigned int uNodeIndex);

void BSSvAddMulticastNode(void *hDeviceContext);

void BSSvClearNodeDBTable(void *hDeviceContext,
			  unsigned int uStartIndex);

void BSSvClearAnyBSSJoinRecord(void *hDeviceContext);
=======
PKnownBSS BSSpSearchBSSList(struct vnt_private *, u8 *pbyDesireBSSID,
	u8 *pbyDesireSSID, CARD_PHY_TYPE ePhyType);

PKnownBSS BSSpAddrIsInBSSList(struct vnt_private *, u8 *abyBSSID,
	PWLAN_IE_SSID pSSID);

void BSSvClearBSSList(struct vnt_private *, int bKeepCurrBSSID);

int BSSbInsertToBSSList(struct vnt_private *,
			u8 *abyBSSIDAddr,
			u64 qwTimestamp,
			u16 wBeaconInterval,
			u16 wCapInfo,
			u8 byCurrChannel,
			PWLAN_IE_SSID pSSID,
			PWLAN_IE_SUPP_RATES pSuppRates,
			PWLAN_IE_SUPP_RATES pExtSuppRates,
			PERPObject psERP,
			PWLAN_IE_RSN pRSN,
			PWLAN_IE_RSN_EXT pRSNWPA,
			PWLAN_IE_COUNTRY pIE_Country,
			PWLAN_IE_QUIET pIE_Quiet,
			u32 uIELength,
			u8 *pbyIEs,
			void *pRxPacketContext);

int BSSbUpdateToBSSList(struct vnt_private *,
			u64 qwTimestamp,
			u16 wBeaconInterval,
			u16 wCapInfo,
			u8 byCurrChannel,
			int bChannelHit,
			PWLAN_IE_SSID pSSID,
			PWLAN_IE_SUPP_RATES pSuppRates,
			PWLAN_IE_SUPP_RATES pExtSuppRates,
			PERPObject psERP,
			PWLAN_IE_RSN pRSN,
			PWLAN_IE_RSN_EXT pRSNWPA,
			PWLAN_IE_COUNTRY pIE_Country,
			PWLAN_IE_QUIET pIE_Quiet,
			PKnownBSS pBSSList,
			u32 uIELength,
			u8 *pbyIEs,
			void *pRxPacketContext);

int BSSbIsSTAInNodeDB(struct vnt_private *, u8 * abyDstAddr,
	u32 *puNodeIndex);

void BSSvCreateOneNode(struct vnt_private *, u32 *puNodeIndex);

void BSSvUpdateAPNode(struct vnt_private *, u16 *pwCapInfo,
	PWLAN_IE_SUPP_RATES pItemRates, PWLAN_IE_SUPP_RATES pExtSuppRates);

void BSSvSecondCallBack(struct work_struct *work);

void BSSvUpdateNodeTxCounter(struct vnt_private *, u8 byTSR, u8 byPktNO);

void BSSvRemoveOneNode(struct vnt_private *, u32 uNodeIndex);

void BSSvAddMulticastNode(struct vnt_private *);

void BSSvClearNodeDBTable(struct vnt_private *, u32 uStartIndex);

void BSSvClearAnyBSSJoinRecord(struct vnt_private *);
>>>>>>> refs/remotes/origin/master

#endif /* __BSSDB_H__ */
