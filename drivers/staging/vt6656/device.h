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
 * File: device.h
 *
 * Purpose: MAC Data structure
 *
 * Author: Tevin Chen
 *
 * Date: Mar 17, 1997
 *
 */

#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <linux/module.h>
<<<<<<< HEAD
#include <linux/types.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/errno.h>
#include <linux/ioport.h>
#include <linux/pci.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/string.h>
#include <linux/wait.h>
#include <linux/if_arp.h>
#include <linux/sched.h>
#include <linux/if.h>
#include <linux/rtnetlink.h>//James
#include <linux/proc_fs.h>
#include <linux/inetdevice.h>
#include <linux/reboot.h>
#include <linux/usb.h>
#include <linux/signal.h>
#include <linux/firmware.h>
#include <asm/io.h>
#include <asm/uaccess.h>
=======
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/firmware.h>
#include <linux/etherdevice.h>
#include <linux/suspend.h>
#include <linux/if_arp.h>
#include <linux/wireless.h>
#include <net/iw_handler.h>
#include <net/cfg80211.h>
#include <linux/timer.h>
#include <linux/usb.h>
#include <linux/crc32.h>

>>>>>>> refs/remotes/origin/master
#ifdef SIOCETHTOOL
#define DEVICE_ETHTOOL_IOCTL_SUPPORT
#include <linux/ethtool.h>
#else
#undef DEVICE_ETHTOOL_IOCTL_SUPPORT
#endif
<<<<<<< HEAD
/* Include Wireless Extension definition and check version - Jean II */
#include <linux/wireless.h>
#include <net/iw_handler.h>	// New driver API

#ifndef WPA_SUPPLICANT_DRIVER_WEXT_SUPPORT
#define WPA_SUPPLICANT_DRIVER_WEXT_SUPPORT
#endif

//please copy below macro to driver_event.c for API
=======

/* please copy below macro to driver_event.c for API */
>>>>>>> refs/remotes/origin/master
#define RT_INSMOD_EVENT_FLAG                             0x0101
#define RT_UPDEV_EVENT_FLAG                               0x0102
#define RT_DISCONNECTED_EVENT_FLAG               0x0103
#define RT_WPACONNECTED_EVENT_FLAG             0x0104
#define RT_DOWNDEV_EVENT_FLAG                        0x0105
#define RT_RMMOD_EVENT_FLAG                              0x0106

<<<<<<< HEAD
//
// device specific
//

#include "device_cfg.h"
#include "ttype.h"
=======
/*
 * device specific
 */

#include "device_cfg.h"
>>>>>>> refs/remotes/origin/master
#include "80211hdr.h"
#include "tether.h"
#include "wmgr.h"
#include "wcmd.h"
<<<<<<< HEAD
#include "mib.h"
=======
>>>>>>> refs/remotes/origin/master
#include "srom.h"
#include "rc4.h"
#include "desc.h"
#include "key.h"
#include "card.h"
<<<<<<< HEAD

/*---------------------  Export Definitions -------------------------*/
=======
#include "rndis.h"

>>>>>>> refs/remotes/origin/master
#define VNT_USB_VENDOR_ID                     0x160a
#define VNT_USB_PRODUCT_ID                    0x3184

#define MAC_MAX_CONTEXT_REG     (256+128)

#define MAX_MULTICAST_ADDRESS_NUM       32
#define MULTICAST_ADDRESS_LIST_SIZE     (MAX_MULTICAST_ADDRESS_NUM * ETH_ALEN)

<<<<<<< HEAD
//#define OP_MODE_INFRASTRUCTURE  0
//#define OP_MODE_ADHOC           1
//#define OP_MODE_AP              2

=======
>>>>>>> refs/remotes/origin/master
#define DUPLICATE_RX_CACHE_LENGTH       5

#define NUM_KEY_ENTRY                   11

#define TX_WEP_NONE                     0
#define TX_WEP_OTF                      1
#define TX_WEP_SW                       2
#define TX_WEP_SWOTP                    3
#define TX_WEP_OTPSW                    4
#define TX_WEP_SW232                    5

#define KEYSEL_WEP40                    0
#define KEYSEL_WEP104                   1
#define KEYSEL_TKIP                     2
#define KEYSEL_CCMP                     3

#define AUTO_FB_NONE            0
#define AUTO_FB_0               1
#define AUTO_FB_1               2

#define FB_RATE0                0
#define FB_RATE1                1

<<<<<<< HEAD
// Antenna Mode
=======
/* Antenna Mode */
>>>>>>> refs/remotes/origin/master
#define ANT_A                   0
#define ANT_B                   1
#define ANT_DIVERSITY           2
#define ANT_RXD_TXA             3
#define ANT_RXD_TXB             4
#define ANT_UNKNOWN             0xFF
#define ANT_TXA                 0
#define ANT_TXB                 1
#define ANT_RXA                 2
#define ANT_RXB                 3

<<<<<<< HEAD

#define MAXCHECKHANGCNT         4

//Packet type
=======
#define MAXCHECKHANGCNT         4

/* Packet type */
>>>>>>> refs/remotes/origin/master
#define TX_PKT_UNI              0x00
#define TX_PKT_MULTI            0x01
#define TX_PKT_BROAD            0x02

#define BB_VGA_LEVEL            4
#define BB_VGA_CHANGE_THRESHOLD 3

#ifndef RUN_AT
#define RUN_AT(x)                       (jiffies+(x))
#endif

<<<<<<< HEAD
// DMA related
=======
/* DMA related */
>>>>>>> refs/remotes/origin/master
#define RESERV_AC0DMA                   4

#define PRIVATE_Message                 0

<<<<<<< HEAD
/*---------------------  Export Types  ------------------------------*/

=======
>>>>>>> refs/remotes/origin/master
#define DBG_PRT(l, p, args...) { if (l <= msglevel) printk(p, ##args); }
#define PRINT_K(p, args...) { if (PRIVATE_Message) printk(p, ##args); }

typedef enum __device_msg_level {
	MSG_LEVEL_ERR = 0,            /* Errors causing abnormal operation */
	MSG_LEVEL_NOTICE = 1,         /* Errors needing user notification */
	MSG_LEVEL_INFO = 2,           /* Normal message. */
	MSG_LEVEL_VERBOSE = 3,        /* Will report all trival errors. */
	MSG_LEVEL_DEBUG = 4           /* Only for debug purpose. */
} DEVICE_MSG_LEVEL, *PDEVICE_MSG_LEVEL;

<<<<<<< HEAD
typedef enum __device_init_type {
	DEVICE_INIT_COLD = 0,       /* cold init */
	DEVICE_INIT_RESET,          /* reset init or Dx to D0 power remain */
	DEVICE_INIT_DXPL            /* Dx to D0 power lost init */
} DEVICE_INIT_TYPE, *PDEVICE_INIT_TYPE;

//USB

//
// Enum of context types for SendPacket
//
=======
#define DEVICE_INIT_COLD	0x0 /* cold init */
#define DEVICE_INIT_RESET	0x1 /* reset init or Dx to D0 power remain */
#define DEVICE_INIT_DXPL	0x2 /* Dx to D0 power lost init */

/* USB */

/*
 * Enum of context types for SendPacket
 */
>>>>>>> refs/remotes/origin/master
typedef enum _CONTEXT_TYPE {
    CONTEXT_DATA_PACKET = 1,
    CONTEXT_MGMT_PACKET
} CONTEXT_TYPE;

<<<<<<< HEAD
// RCB (Receive Control Block)
typedef struct _RCB
{
    void *Next;
    signed long                    Ref;
    void *pDevice;
    struct urb              *pUrb;
    SRxMgmtPacket           sMngPacket;
    struct sk_buff*         skb;
    BOOL                    bBoolInUse;

} RCB, *PRCB;

// used to track bulk out irps
typedef struct _USB_SEND_CONTEXT {
    void *pDevice;
    struct sk_buff *pPacket;
    struct urb      *pUrb;
    unsigned int            uBufLen;
    CONTEXT_TYPE    Type;
    SEthernetHeader sEthHeader;
    void *Next;
    BOOL            bBoolInUse;
    unsigned char           Data[MAX_TOTAL_SIZE_WITH_ALL_HEADERS];
} USB_SEND_CONTEXT, *PUSB_SEND_CONTEXT;
=======
/* RCB (Receive Control Block) */
struct vnt_rcb {
	void *Next;
	signed long Ref;
	void *pDevice;
	struct urb *pUrb;
	struct vnt_rx_mgmt sMngPacket;
	struct sk_buff *skb;
	int bBoolInUse;
};

/* used to track bulk out irps */
struct vnt_usb_send_context {
	void *pDevice;
	struct sk_buff *pPacket;
	struct urb *pUrb;
	unsigned int uBufLen;
	CONTEXT_TYPE Type;
	struct ethhdr sEthHeader;
	void *Next;
	bool bBoolInUse;
	unsigned char Data[MAX_TOTAL_SIZE_WITH_ALL_HEADERS];
};

/* tx packet info for rxtx */
struct vnt_tx_pkt_info {
	u16 fifo_ctl;
	u8 dest_addr[ETH_ALEN];
};
>>>>>>> refs/remotes/origin/master

/* structure got from configuration file as user-desired default settings */
typedef struct _DEFAULT_CONFIG {
	signed int    ZoneType;
	signed int    eConfigMode;
	signed int    eAuthenMode;        /* open/wep/wpa */
	signed int    bShareKeyAlgorithm; /* open-open/{open,wep}-sharekey */
	signed int    keyidx;             /* wepkey index */
	signed int    eEncryptionStatus;
} DEFAULT_CONFIG, *PDEFAULT_CONFIG;

<<<<<<< HEAD
//
// Structure to keep track of usb interrupt packets
//
typedef struct {
    unsigned int            uDataLen;
    PBYTE           pDataBuf;
//    struct urb      *pUrb;
    BOOL            bInUse;
} INT_BUFFER, *PINT_BUFFER;

//0:11A 1:11B 2:11G
=======
/*
 * Structure to keep track of USB interrupt packets
 */
typedef struct {
    unsigned int            uDataLen;
    u8 *           pDataBuf;
  /* struct urb *pUrb; */
    bool            bInUse;
} INT_BUFFER, *PINT_BUFFER;

/* 0:11A 1:11B 2:11G */
>>>>>>> refs/remotes/origin/master
typedef enum _VIA_BB_TYPE
{
    BB_TYPE_11A = 0,
    BB_TYPE_11B,
    BB_TYPE_11G
} VIA_BB_TYPE, *PVIA_BB_TYPE;

<<<<<<< HEAD
//0:11a,1:11b,2:11gb(only CCK in BasicRate),3:11ga(OFDM in Basic Rate)
=======
/* 0:11a, 1:11b, 2:11gb (only CCK in BasicRate), 3:11ga(OFDM in BasicRate) */
>>>>>>> refs/remotes/origin/master
typedef enum _VIA_PKT_TYPE
{
    PK_TYPE_11A = 0,
    PK_TYPE_11B,
    PK_TYPE_11GB,
    PK_TYPE_11GA
} VIA_PKT_TYPE, *PVIA_PKT_TYPE;

<<<<<<< HEAD
//++ NDIS related
=======
/*++ NDIS related */
>>>>>>> refs/remotes/origin/master

typedef enum __DEVICE_NDIS_STATUS {
    STATUS_SUCCESS = 0,
    STATUS_FAILURE,
    STATUS_RESOURCES,
    STATUS_PENDING,
} DEVICE_NDIS_STATUS, *PDEVICE_NDIS_STATUS;

#define MAX_BSSIDINFO_4_PMKID   16
#define MAX_PMKIDLIST           5
<<<<<<< HEAD
//Flags for PMKID Candidate list structure
#define NDIS_802_11_PMKID_CANDIDATE_PREAUTH_ENABLED	0x01

// PMKID Structures
typedef unsigned char   NDIS_802_11_PMKID_VALUE[16];


=======
/* flags for PMKID Candidate list structure */
#define NDIS_802_11_PMKID_CANDIDATE_PREAUTH_ENABLED	0x01

/* PMKID Structures */
typedef unsigned char   NDIS_802_11_PMKID_VALUE[16];

>>>>>>> refs/remotes/origin/master
typedef enum _NDIS_802_11_WEP_STATUS
{
    Ndis802_11WEPEnabled,
    Ndis802_11Encryption1Enabled = Ndis802_11WEPEnabled,
    Ndis802_11WEPDisabled,
    Ndis802_11EncryptionDisabled = Ndis802_11WEPDisabled,
    Ndis802_11WEPKeyAbsent,
    Ndis802_11Encryption1KeyAbsent = Ndis802_11WEPKeyAbsent,
    Ndis802_11WEPNotSupported,
    Ndis802_11EncryptionNotSupported = Ndis802_11WEPNotSupported,
    Ndis802_11Encryption2Enabled,
    Ndis802_11Encryption2KeyAbsent,
    Ndis802_11Encryption3Enabled,
    Ndis802_11Encryption3KeyAbsent
} NDIS_802_11_WEP_STATUS, *PNDIS_802_11_WEP_STATUS,
  NDIS_802_11_ENCRYPTION_STATUS, *PNDIS_802_11_ENCRYPTION_STATUS;

<<<<<<< HEAD

typedef enum _NDIS_802_11_STATUS_TYPE
{
    Ndis802_11StatusType_Authentication,
    Ndis802_11StatusType_MediaStreamMode,
    Ndis802_11StatusType_PMKID_CandidateList,
    Ndis802_11StatusTypeMax    // not a real type, defined as an upper bound
} NDIS_802_11_STATUS_TYPE, *PNDIS_802_11_STATUS_TYPE;

//Added new types for PMKID Candidate lists.
=======
typedef enum _NDIS_802_11_STATUS_TYPE
{
	Ndis802_11StatusType_Authentication,
	Ndis802_11StatusType_MediaStreamMode,
	Ndis802_11StatusType_PMKID_CandidateList,
	Ndis802_11StatusTypeMax, /* not a real type, defined as upper bound */
} NDIS_802_11_STATUS_TYPE, *PNDIS_802_11_STATUS_TYPE;

/* added new types for PMKID Candidate lists */
>>>>>>> refs/remotes/origin/master
typedef struct _PMKID_CANDIDATE {
    NDIS_802_11_MAC_ADDRESS BSSID;
    unsigned long Flags;
} PMKID_CANDIDATE, *PPMKID_CANDIDATE;

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
typedef struct _BSSID_INFO
{
    NDIS_802_11_MAC_ADDRESS BSSID;
    NDIS_802_11_PMKID_VALUE PMKID;
} BSSID_INFO, *PBSSID_INFO;

typedef struct tagSPMKID {
    unsigned long Length;
    unsigned long BSSIDInfoCount;
    BSSID_INFO BSSIDInfo[MAX_BSSIDINFO_4_PMKID];
} SPMKID, *PSPMKID;

typedef struct tagSPMKIDCandidateEvent {
    NDIS_802_11_STATUS_TYPE     StatusType;
	unsigned long Version;       /* Version of the structure */
	unsigned long NumCandidates; /* No. of pmkid candidates */
    PMKID_CANDIDATE CandidateList[MAX_PMKIDLIST];
} SPMKIDCandidateEvent, *PSPMKIDCandidateEvent;

<<<<<<< HEAD
//--

//++ 802.11h related
#define MAX_QUIET_COUNT     8

typedef struct tagSQuietControl {
    BOOL        bEnable;
    DWORD       dwStartTime;
    BYTE        byPeriod;
    WORD        wDuration;
} SQuietControl, *PSQuietControl;

//--


// The receive duplicate detection cache entry
typedef struct tagSCacheEntry{
    WORD        wFmSequence;
    BYTE        abyAddr2[ETH_ALEN];
    WORD        wFrameCtl;
=======
/*++ 802.11h related */
#define MAX_QUIET_COUNT     8

typedef struct tagSQuietControl {
    bool        bEnable;
    u32       dwStartTime;
    u8        byPeriod;
    u16        wDuration;
} SQuietControl, *PSQuietControl;

/* The receive duplicate detection cache entry */
typedef struct tagSCacheEntry{
    u16        wFmSequence;
    u8        abyAddr2[ETH_ALEN];
    u16        wFrameCtl;
>>>>>>> refs/remotes/origin/master
} SCacheEntry, *PSCacheEntry;

typedef struct tagSCache{
/* The receive cache is updated circularly.  The next entry to be written is
 * indexed by the "InPtr".
<<<<<<< HEAD
*/
=======
 */
>>>>>>> refs/remotes/origin/master
	unsigned int uInPtr; /* Place to use next */
    SCacheEntry     asCacheEntry[DUPLICATE_RX_CACHE_LENGTH];
} SCache, *PSCache;

#define CB_MAX_RX_FRAG                 64
<<<<<<< HEAD
// DeFragment Control Block, used for collecting fragments prior to reassembly
typedef struct tagSDeFragControlBlock
{
    WORD            wSequence;
    WORD            wFragNum;
    BYTE            abyAddr2[ETH_ALEN];
	unsigned int            uLifetime;
    struct sk_buff* skb;
    PBYTE           pbyRxBuffer;
    unsigned int            cbFrameLength;
    BOOL            bInUse;
} SDeFragControlBlock, *PSDeFragControlBlock;



//flags for options
=======
/*
 * DeFragment Control Block, used for collecting fragments prior to reassembly
 */
typedef struct tagSDeFragControlBlock
{
    u16            wSequence;
    u16            wFragNum;
    u8            abyAddr2[ETH_ALEN];
	unsigned int            uLifetime;
    struct sk_buff* skb;
    u8 *           pbyRxBuffer;
    unsigned int            cbFrameLength;
    bool            bInUse;
} SDeFragControlBlock, *PSDeFragControlBlock;

/* flags for options */
>>>>>>> refs/remotes/origin/master
#define     DEVICE_FLAGS_UNPLUG          0x00000001UL
#define     DEVICE_FLAGS_PREAMBLE_TYPE   0x00000002UL
#define     DEVICE_FLAGS_OP_MODE         0x00000004UL
#define     DEVICE_FLAGS_PS_MODE         0x00000008UL
#define		DEVICE_FLAGS_80211h_MODE	 0x00000010UL

<<<<<<< HEAD
//flags for driver status
#define     DEVICE_FLAGS_OPENED          0x00010000UL
#define     DEVICE_FLAGS_WOL_ENABLED     0x00080000UL
//flags for capbilities
=======
/* flags for driver status */
#define     DEVICE_FLAGS_OPENED          0x00010000UL
#define     DEVICE_FLAGS_WOL_ENABLED     0x00080000UL
/* flags for capabilities */
>>>>>>> refs/remotes/origin/master
#define     DEVICE_FLAGS_TX_ALIGN        0x01000000UL
#define     DEVICE_FLAGS_HAVE_CAM        0x02000000UL
#define     DEVICE_FLAGS_FLOW_CTRL       0x04000000UL

<<<<<<< HEAD
//flags for MII status
=======
/* flags for MII status */
>>>>>>> refs/remotes/origin/master
#define     DEVICE_LINK_FAIL             0x00000001UL
#define     DEVICE_SPEED_10              0x00000002UL
#define     DEVICE_SPEED_100             0x00000004UL
#define     DEVICE_SPEED_1000            0x00000008UL
#define     DEVICE_DUPLEX_FULL           0x00000010UL
#define     DEVICE_AUTONEG_ENABLE        0x00000020UL
#define     DEVICE_FORCED_BY_EEPROM      0x00000040UL
<<<<<<< HEAD
//for device_set_media_duplex
#define     DEVICE_LINK_CHANGE           0x00000001UL


typedef struct __device_opt {
    int         nRxDescs0;    //Number of RX descriptors0
    int         nTxDescs0;    //Number of TX descriptors 0, 1, 2, 3
    int         rts_thresh;   //rts threshold
=======
/* for device_set_media_duplex */
#define     DEVICE_LINK_CHANGE           0x00000001UL

typedef struct __device_opt {
	int nRxDescs0;  /* number of RX descriptors 0 */
	int nTxDescs0;  /* number of TX descriptors 0, 1, 2, 3 */
	int rts_thresh; /* RTS threshold */
>>>>>>> refs/remotes/origin/master
    int         frag_thresh;
    int         OpMode;
    int         data_rate;
    int         channel_num;
    int         short_retry;
    int         long_retry;
    int         bbp_type;
    u32         flags;
} OPTIONS, *POPTIONS;

<<<<<<< HEAD

typedef struct __device_info {

// netdev
	struct usb_device*          usb;
    struct net_device*          dev;
    struct net_device_stats     stats;

    const struct firmware	*firmware;

    OPTIONS                     sOpts;

	struct tasklet_struct       CmdWorkItem;
	struct tasklet_struct       EventWorkItem;
	struct tasklet_struct       ReadWorkItem;
	struct tasklet_struct       RxMngWorkItem;

    u32                         rx_buf_sz;
    int                         multicast_limit;
    BYTE                        byRxMode;

    spinlock_t                  lock;

    u32                         rx_bytes;

    BYTE                        byRevId;

    u32                         flags;
    unsigned long                       Flags;

    SCache                      sDupRxCache;

    SDeFragControlBlock         sRxDFCB[CB_MAX_RX_FRAG];
    unsigned int                        cbDFCB;
    unsigned int                        cbFreeDFCB;
    unsigned int                        uCurrentDFCBIdx;

    // +++USB

    struct urb                  *pControlURB;
    struct urb                  *pInterruptURB;
	struct usb_ctrlrequest      sUsbCtlRequest;

    unsigned int                        int_interval;
    //
    // Variables to track resources for the BULK In Pipe
    //
    PRCB                        pRCBMem;
    PRCB                        apRCB[CB_MAX_RX_DESC];
    unsigned int                        cbRD;
    PRCB                        FirstRecvFreeList;
    PRCB                        LastRecvFreeList;
    unsigned int                        NumRecvFreeList;
    PRCB                        FirstRecvMngList;
    PRCB                        LastRecvMngList;
    unsigned int                        NumRecvMngList;
    BOOL                        bIsRxWorkItemQueued;
    BOOL                        bIsRxMngWorkItemQueued;
	unsigned long ulRcvRefCount; /* packets that have not returned back */

    //
    //  Variables to track resources for the BULK Out Pipe
    //

    PUSB_SEND_CONTEXT           apTD[CB_MAX_TX_DESC];
    unsigned int                        cbTD;

    //
    //  Variables to track resources for the Interript In Pipe
    //
    INT_BUFFER                  intBuf;
    BOOL                        fKillEventPollingThread;
    BOOL                        bEventAvailable;


  //default config from file by user setting
    DEFAULT_CONFIG    config_file;


    //
    // Statistic for USB
    // protect with spinlock
    unsigned long                       ulBulkInPosted;
    unsigned long                       ulBulkInError;
    unsigned long                       ulBulkInContCRCError;
    unsigned long                       ulBulkInBytesRead;

    unsigned long                       ulBulkOutPosted;
    unsigned long                       ulBulkOutError;
    unsigned long                       ulBulkOutContCRCError;
    unsigned long                       ulBulkOutBytesWrite;

    unsigned long                       ulIntInPosted;
    unsigned long                       ulIntInError;
    unsigned long                       ulIntInContCRCError;
    unsigned long                       ulIntInBytesRead;


    // Version control
    WORD                        wFirmwareVersion;
    BYTE                        byLocalID;
    BYTE                        byRFType;
    BYTE                        byBBRxConf;


    BYTE                        byZoneType;
    BOOL                        bZoneRegExist;

    BYTE                        byOriginalZonetype;

    BOOL                        bLinkPass;          // link status: OK or fail
    BYTE                        abyCurrentNetAddr[ETH_ALEN];
    BYTE                        abyPermanentNetAddr[ETH_ALEN];
    // SW network address
	/* u8 abySoftwareNetAddr[ETH_ALEN]; */
    BOOL                        bExistSWNetAddr;

    // Adapter statistics
    SStatCounter                scStatistic;
    // 802.11 counter
    SDot11Counters              s802_11Counter;

    //
    // Maintain statistical debug info.
    //
    unsigned long                       packetsReceived;
    unsigned long                       packetsReceivedDropped;
    unsigned long                       packetsReceivedOverflow;
    unsigned long                       packetsSent;
    unsigned long                       packetsSentDropped;
    unsigned long                       SendContextsInUse;
    unsigned long                       RcvBuffersInUse;


    // 802.11 management
    SMgmtObject                 sMgmtObj;

    QWORD                       qwCurrTSF;
    unsigned int                        cbBulkInMax;
    BOOL                        bPSRxBeacon;

    // 802.11 MAC specific
    unsigned int                        uCurrRSSI;
    BYTE                        byCurrSQ;


    //Antenna Diversity
    BOOL                        bTxRxAntInv;
    DWORD                       dwRxAntennaSel;
    DWORD                       dwTxAntennaSel;
    BYTE                        byAntennaCount;
    BYTE                        byRxAntennaMode;
    BYTE                        byTxAntennaMode;
    BYTE                        byRadioCtl;
    BYTE                        bHWRadioOff;

    //SQ3 functions for antenna diversity
    struct timer_list           TimerSQ3Tmax1;
    struct timer_list           TimerSQ3Tmax2;
    struct timer_list           TimerSQ3Tmax3;

    BOOL                        bDiversityRegCtlON;
    BOOL                        bDiversityEnable;
    unsigned long                       ulDiversityNValue;
    unsigned long                       ulDiversityMValue;
    BYTE                        byTMax;
    BYTE                        byTMax2;
    BYTE                        byTMax3;
    unsigned long                       ulSQ3TH;

    unsigned long                       uDiversityCnt;
    BYTE                        byAntennaState;
    unsigned long                       ulRatio_State0;
    unsigned long                       ulRatio_State1;
    unsigned long                       ulSQ3_State0;
    unsigned long                       ulSQ3_State1;

    unsigned long                       aulSQ3Val[MAX_RATE];
    unsigned long                       aulPktNum[MAX_RATE];

	/* IFS & Cw */
	unsigned int uSIFS;  /* Current SIFS */
	unsigned int uDIFS;  /* Current DIFS */
	unsigned int uEIFS;  /* Current EIFS */
	unsigned int uSlot;  /* Current SlotTime */
	unsigned int uCwMin; /* Current CwMin */
	unsigned int uCwMax; /* CwMax is fixed on 1023 */

    // PHY parameter
    BYTE                        bySIFS;
    BYTE                        byDIFS;
    BYTE                        byEIFS;
    BYTE                        bySlot;
    BYTE                        byCWMaxMin;

    // Rate
    VIA_BB_TYPE                 byBBType; //0: 11A, 1:11B, 2:11G
    VIA_PKT_TYPE                byPacketType; //0:11a,1:11b,2:11gb(only CCK in BasicRate),3:11ga(OFDM in Basic Rate)
    WORD                        wBasicRate;
    BYTE                        byACKRate;
    BYTE                        byTopOFDMBasicRate;
    BYTE                        byTopCCKBasicRate;


    DWORD                       dwAotoRateTxOkCnt;
    DWORD                       dwAotoRateTxFailCnt;
    DWORD                       dwErrorRateThreshold[13];
    DWORD                       dwTPTable[MAX_RATE];
    BYTE                        abyEEPROM[EEP_MAX_CONTEXT_SIZE];  //DWORD alignment

    BYTE                        byMinChannel;
    BYTE                        byMaxChannel;
    unsigned int                        uConnectionRate;

    BYTE                        byPreambleType;
    BYTE                        byShortPreamble;
    // CARD_PHY_TYPE
    BYTE                        eConfigPHYMode;

    // For RF Power table
    BYTE                        byCCKPwr;
    BYTE                        byOFDMPwrG;
    BYTE                        byOFDMPwrA;
    BYTE                        byCurPwr;
    BYTE                        abyCCKPwrTbl[14];
    BYTE                        abyOFDMPwrTbl[14];
    BYTE                        abyOFDMAPwrTbl[42];

    WORD                        wCurrentRate;
    WORD                        wRTSThreshold;
    WORD                        wFragmentationThreshold;
    BYTE                        byShortRetryLimit;
    BYTE                        byLongRetryLimit;
    CARD_OP_MODE                eOPMode;
    BOOL                        bBSSIDFilter;
    WORD                        wMaxTransmitMSDULifetime;
    BYTE                        abyBSSID[ETH_ALEN];
    BYTE                        abyDesireBSSID[ETH_ALEN];
    WORD                        wCTSDuration;       // update while speed change
    WORD                        wACKDuration;       // update while speed change
    WORD                        wRTSTransmitLen;    // update while speed change
    BYTE                        byRTSServiceField;  // update while speed change
    BYTE                        byRTSSignalField;   // update while speed change

    DWORD                       dwMaxReceiveLifetime;       // dot11MaxReceiveLifetime

    BOOL                        bCCK;
    BOOL                        bEncryptionEnable;
    BOOL                        bLongHeader;
    BOOL                        bSoftwareGenCrcErr;
    BOOL                        bShortSlotTime;
    BOOL                        bProtectMode;
    BOOL                        bNonERPPresent;
    BOOL                        bBarkerPreambleMd;

    BYTE                        byERPFlag;
    WORD                        wUseProtectCntDown;

    BOOL                        bRadioControlOff;
    BOOL                        bRadioOff;

    // Power save
    BOOL                        bEnablePSMode;
    WORD                        wListenInterval;
    BOOL                        bPWBitOn;
    WMAC_POWER_MODE             ePSMode;
    unsigned long                       ulPSModeWaitTx;
    BOOL                        bPSModeTxBurst;

    // Beacon releated
    WORD                    wSeqCounter;
    BOOL                    bBeaconBufReady;
    BOOL                    bBeaconSent;
    BOOL                    bFixRate;
    BYTE                    byCurrentCh;
    unsigned int                    uScanTime;

    CMD_STATE               eCommandState;

    CMD_CODE                eCommand;
    BOOL                    bBeaconTx;
    BYTE                    byScanBBType;

    BOOL                    bStopBeacon;
    BOOL                    bStopDataPkt;
    BOOL                    bStopTx0Pkt;
    unsigned int                    uAutoReConnectTime;
    unsigned int                    uIsroamingTime;

    // 802.11 counter

    CMD_ITEM                eCmdQueue[CMD_Q_SIZE];
    unsigned int                    uCmdDequeueIdx;
    unsigned int                    uCmdEnqueueIdx;
    unsigned int                    cbFreeCmdQueue;
    BOOL                    bCmdRunning;
    BOOL                    bCmdClear;
    BOOL                    bNeedRadioOFF;

    BOOL                    bEnableRoaming;
    BOOL                    bIsRoaming;
    BOOL                    bFastRoaming;
    BYTE                    bSameBSSMaxNum;
    BYTE                    bSameBSSCurNum;
    BOOL                    bRoaming;
    BOOL                    b11hEable;
    unsigned long                   ulTxPower;

    // Encryption
    NDIS_802_11_WEP_STATUS  eEncryptionStatus;
    BOOL                    bTransmitKey;

//mike add :save old Encryption
    NDIS_802_11_WEP_STATUS  eOldEncryptionStatus;

    SKeyManagement          sKey;
    DWORD                   dwIVCounter;


    RC4Ext                  SBox;
    BYTE                    abyPRNG[WLAN_WEPMAX_KEYLEN+3];
    BYTE                    byKeyIndex;

    BOOL                    bAES;
    BYTE                    byCntMeasure;

    unsigned int                    uKeyLength;
    BYTE                    abyKey[WLAN_WEP232_KEYLEN];

    // for AP mode
    unsigned int                    uAssocCount;
    BOOL                    bMoreData;

    // QoS
    BOOL                    bGrpAckPolicy;


    BYTE                    byAutoFBCtrl;

    BOOL                    bTxMICFail;
    BOOL                    bRxMICFail;


    // For Update BaseBand VGA Gain Offset
    BOOL                    bUpdateBBVGA;
    unsigned int                    uBBVGADiffCount;
    BYTE                    byBBVGANew;
    BYTE                    byBBVGACurrent;
    BYTE                    abyBBVGA[BB_VGA_LEVEL];
    signed long                    ldBmThreshold[BB_VGA_LEVEL];

    BYTE                    byBBPreEDRSSI;
    BYTE                    byBBPreEDIndex;


    BOOL                    bRadioCmd;
    DWORD                   dwDiagRefCount;

    // For FOE Tuning
    BYTE                    byFOETuning;

    // For Auto Power Tunning

    BYTE                    byAutoPwrTunning;

    // BaseBand Loopback Use
    BYTE                    byBBCR4d;
    BYTE                    byBBCRc9;
    BYTE                    byBBCR88;
    BYTE                    byBBCR09;

    // command timer
    struct timer_list       sTimerCommand;

     struct timer_list       sTimerTxData;
     unsigned long                       nTxDataTimeCout;
     BOOL  fTxDataInSleep;
     BOOL  IsTxDataTrigger;

    BOOL  fWPA_Authened;           //is WPA/WPA-PSK or WPA2/WPA2-PSK authen??
    BYTE            byReAssocCount;   //mike add:re-association retry times!
    BYTE            byLinkWaitCount;

    SEthernetHeader         sTxEthHeader;
    SEthernetHeader         sRxEthHeader;
    BYTE                    abyBroadcastAddr[ETH_ALEN];
    BYTE                    abySNAP_RFC1042[ETH_ALEN];
    BYTE                    abySNAP_Bridgetunnel[ETH_ALEN];

    // Pre-Authentication & PMK cache
    SPMKID                  gsPMKID;
    SPMKIDCandidateEvent    gsPMKIDCandidate;


    // for 802.11h
    BOOL                    b11hEnable;

    BOOL                    bChannelSwitch;
    BYTE                    byNewChannel;
    BYTE                    byChannelSwitchCount;

    //WPA supplicant daemon
	struct net_device       *wpadev;
	BOOL                    bWPADEVUp;
    struct sk_buff          *skb;
    //--

#ifdef WPA_SUPPLICANT_DRIVER_WEXT_SUPPORT
        BOOL                 bwextstep0;
        BOOL                 bwextstep1;
        BOOL                 bwextstep2;
        BOOL                 bwextstep3;
        BOOL                 bWPASuppWextEnabled;
#endif

#ifdef HOSTAP
    // user space daemon: hostapd, is used for HOSTAP
	BOOL                    bEnableHostapd;
	BOOL                    bEnable8021x;
	BOOL                    bEnableHostWEP;
	struct net_device       *apdev;
	int (*tx_80211)(struct sk_buff *skb, struct net_device *dev);
#endif
    unsigned int                    uChannel;

	struct iw_statistics	wstats;		// wireless stats
    BOOL                    bCommit;

} DEVICE_INFO, *PSDevice;



=======
struct vnt_private {
	/* netdev */
	struct usb_device *usb;
	struct net_device *dev;
	struct net_device_stats stats;

	OPTIONS sOpts;

	struct tasklet_struct CmdWorkItem;
	struct tasklet_struct EventWorkItem;
	struct work_struct read_work_item;
	struct work_struct rx_mng_work_item;

	u32 rx_buf_sz;
	int multicast_limit;
	u8 byRxMode;

	spinlock_t lock;

	u32 rx_bytes;

	u8 byRevId;

	u32 flags;
	unsigned long Flags;

	SCache sDupRxCache;

	SDeFragControlBlock sRxDFCB[CB_MAX_RX_FRAG];
	u32 cbDFCB;
	u32 cbFreeDFCB;
	u32 uCurrentDFCBIdx;

	/* USB */
	struct urb *pControlURB;
	struct urb *pInterruptURB;
	struct usb_ctrlrequest sUsbCtlRequest;
	u32 int_interval;

	/* Variables to track resources for the BULK In Pipe */
	struct vnt_rcb *pRCBMem;
	struct vnt_rcb *apRCB[CB_MAX_RX_DESC];
	u32 cbRD;
	struct vnt_rcb *FirstRecvFreeList;
	struct vnt_rcb *LastRecvFreeList;
	u32 NumRecvFreeList;
	struct vnt_rcb *FirstRecvMngList;
	struct vnt_rcb *LastRecvMngList;
	u32 NumRecvMngList;
	int bIsRxWorkItemQueued;
	int bIsRxMngWorkItemQueued;
	unsigned long ulRcvRefCount; /* packets that have not returned back */

	/* Variables to track resources for the BULK Out Pipe */
	struct vnt_usb_send_context *apTD[CB_MAX_TX_DESC];
	u32 cbTD;
	struct vnt_tx_pkt_info pkt_info[16];

	/* Variables to track resources for the Interrupt In Pipe */
	INT_BUFFER intBuf;
	int fKillEventPollingThread;
	int bEventAvailable;

	/* default config from file by user setting */
	DEFAULT_CONFIG config_file;

	/* Statistic for USB */
	unsigned long ulBulkInPosted;
	unsigned long ulBulkInError;
	unsigned long ulBulkInContCRCError;
	unsigned long ulBulkInBytesRead;

	unsigned long ulBulkOutPosted;
	unsigned long ulBulkOutError;
	unsigned long ulBulkOutContCRCError;
	unsigned long ulBulkOutBytesWrite;

	unsigned long ulIntInPosted;
	unsigned long ulIntInError;
	unsigned long ulIntInContCRCError;
	unsigned long ulIntInBytesRead;

	/* Version control */
	u16 wFirmwareVersion;
	u8 byLocalID;
	u8 byRFType;
	u8 byBBRxConf;

	u8 byZoneType;
	int bZoneRegExist;

	u8 byOriginalZonetype;

	int bLinkPass; /* link status: OK or fail */
	struct vnt_cmd_card_init init_command;
	struct vnt_rsp_card_init init_response;
	u8 abyCurrentNetAddr[ETH_ALEN];
	u8 abyPermanentNetAddr[ETH_ALEN];

	int bExistSWNetAddr;

	/* Maintain statistical debug info. */
	unsigned long packetsReceived;
	unsigned long packetsReceivedDropped;
	unsigned long packetsReceivedOverflow;
	unsigned long packetsSent;
	unsigned long packetsSentDropped;
	unsigned long SendContextsInUse;
	unsigned long RcvBuffersInUse;

	/* 802.11 management */
	struct vnt_manager vnt_mgmt;

	u64 qwCurrTSF;
	u32 cbBulkInMax;
	int bPSRxBeacon;

	/* 802.11 MAC specific */
	u32 uCurrRSSI;
	u8 byCurrSQ;

	/* Antenna Diversity */
	int bTxRxAntInv;
	u32 dwRxAntennaSel;
	u32 dwTxAntennaSel;
	u8 byAntennaCount;
	u8 byRxAntennaMode;
	u8 byTxAntennaMode;
	u8 byRadioCtl;
	u8 bHWRadioOff;

	/* SQ3 functions for antenna diversity */
	struct timer_list TimerSQ3Tmax1;
	struct timer_list TimerSQ3Tmax2;
	struct timer_list TimerSQ3Tmax3;

	int bDiversityRegCtlON;
	int bDiversityEnable;
	unsigned long ulDiversityNValue;
	unsigned long ulDiversityMValue;
	u8 byTMax;
	u8 byTMax2;
	u8 byTMax3;
	unsigned long ulSQ3TH;

	unsigned long uDiversityCnt;
	u8 byAntennaState;
	unsigned long ulRatio_State0;
	unsigned long ulRatio_State1;
	unsigned long ulSQ3_State0;
	unsigned long ulSQ3_State1;

	unsigned long aulSQ3Val[MAX_RATE];
	unsigned long aulPktNum[MAX_RATE];

	/* IFS & Cw */
	u32 uSIFS;  /* Current SIFS */
	u32 uDIFS;  /* Current DIFS */
	u32 uEIFS;  /* Current EIFS */
	u32 uSlot;  /* Current SlotTime */
	u32 uCwMin; /* Current CwMin */
	u32 uCwMax; /* CwMax is fixed on 1023 */

	/* PHY parameter */
	u8  bySIFS;
	u8  byDIFS;
	u8  byEIFS;
	u8  bySlot;
	u8  byCWMaxMin;

	/* Rate */
	VIA_BB_TYPE byBBType; /* 0: 11A, 1:11B, 2:11G */
	VIA_PKT_TYPE byPacketType; /* 0:11a 1:11b 2:11gb 3:11ga */
	u16 wBasicRate;
	u8 byACKRate;
	u8 byTopOFDMBasicRate;
	u8 byTopCCKBasicRate;

	u32 dwAotoRateTxOkCnt;
	u32 dwAotoRateTxFailCnt;
	u32 dwErrorRateThreshold[13];
	u32 dwTPTable[MAX_RATE];
	u8 abyEEPROM[EEP_MAX_CONTEXT_SIZE];  /*u32 alignment */

	u8 byMinChannel;
	u8 byMaxChannel;
	u32 uConnectionRate;

	u8 byPreambleType;
	u8 byShortPreamble;
	/* CARD_PHY_TYPE */
	u8 eConfigPHYMode;

	/* For RF Power table */
	u8 byCCKPwr;
	u8 byOFDMPwrG;
	u8 byOFDMPwrA;
	u8 byCurPwr;
	u8 abyCCKPwrTbl[14];
	u8 abyOFDMPwrTbl[14];
	u8 abyOFDMAPwrTbl[42];

	u16 wCurrentRate;
	u16 tx_rate_fb0;
	u16 tx_rate_fb1;

	u16 wRTSThreshold;
	u16 wFragmentationThreshold;
	u8 byShortRetryLimit;
	u8 byLongRetryLimit;
	CARD_OP_MODE eOPMode;
	int bBSSIDFilter;
	u16 wMaxTransmitMSDULifetime;
	u8 abyBSSID[ETH_ALEN];
	u8 abyDesireBSSID[ETH_ALEN];

	u32 dwMaxReceiveLifetime;  /* dot11MaxReceiveLifetime */

	int bCCK;
	int bEncryptionEnable;
	int bShortSlotTime;
	int bProtectMode;
	int bNonERPPresent;
	int bBarkerPreambleMd;

	u8 byERPFlag;
	u16 wUseProtectCntDown;

	int bRadioControlOff;
	int bRadioOff;

	/* Power save */
	int bEnablePSMode;
	u16 wListenInterval;
	int bPWBitOn;
	WMAC_POWER_MODE ePSMode;
	unsigned long ulPSModeWaitTx;
	int bPSModeTxBurst;

	/* Beacon releated */
	u16 wSeqCounter;
	int bBeaconBufReady;
	int bBeaconSent;
	int bFixRate;
	u8 byCurrentCh;
	u32 uScanTime;

	CMD_STATE eCommandState;

	CMD_CODE eCommand;
	int bBeaconTx;
	u8 byScanBBType;

	int bStopBeacon;
	int bStopDataPkt;
	int bStopTx0Pkt;
	u32 uAutoReConnectTime;
	u32 uIsroamingTime;

	/* 802.11 counter */

	CMD_ITEM eCmdQueue[CMD_Q_SIZE];
	u32 uCmdDequeueIdx;
	u32 uCmdEnqueueIdx;
	u32 cbFreeCmdQueue;
	int bCmdRunning;
	int bCmdClear;
	int bNeedRadioOFF;

	int bEnableRoaming;
	int bIsRoaming;
	int bFastRoaming;
	u8 bSameBSSMaxNum;
	u8 bSameBSSCurNum;
	int bRoaming;
	int b11hEable;
	unsigned long ulTxPower;

	/* Encryption */
	NDIS_802_11_WEP_STATUS eEncryptionStatus;
	int  bTransmitKey;
	NDIS_802_11_WEP_STATUS eOldEncryptionStatus;
	SKeyManagement sKey;
	u32 dwIVCounter;

	RC4Ext SBox;
	u8 abyPRNG[WLAN_WEPMAX_KEYLEN+3];
	u8 byKeyIndex;

	u32 uKeyLength;
	u8 abyKey[WLAN_WEP232_KEYLEN];

	/* for AP mode */
	u32 uAssocCount;
	int bMoreData;

	/* QoS */
	int bGrpAckPolicy;

	u8 byAutoFBCtrl;

	int bTxMICFail;
	int bRxMICFail;

	/* For Update BaseBand VGA Gain Offset */
	int bUpdateBBVGA;
	u32 uBBVGADiffCount;
	u8 byBBVGANew;
	u8 byBBVGACurrent;
	u8 abyBBVGA[BB_VGA_LEVEL];
	signed long ldBmThreshold[BB_VGA_LEVEL];

	u8 byBBPreEDRSSI;
	u8 byBBPreEDIndex;

	int bRadioCmd;

	/* For FOE Tuning */
	u8  byFOETuning;

	/* For Auto Power Tunning */
	u8  byAutoPwrTunning;

	/* BaseBand Loopback Use */
	u8 byBBCR4d;
	u8 byBBCRc9;
	u8 byBBCR88;
	u8 byBBCR09;

	/* command timer */
	struct delayed_work run_command_work;
	/* One second callback */
	struct delayed_work second_callback_work;

	u8 tx_data_time_out;
	bool tx_trigger;
	int fWPA_Authened; /*is WPA/WPA-PSK or WPA2/WPA2-PSK authen?? */
	u8 byReAssocCount;
	u8 byLinkWaitCount;

	struct ethhdr sTxEthHeader;
	struct ethhdr sRxEthHeader;
	u8 abyBroadcastAddr[ETH_ALEN];
	u8 abySNAP_RFC1042[ETH_ALEN];
	u8 abySNAP_Bridgetunnel[ETH_ALEN];

	/* Pre-Authentication & PMK cache */
	SPMKID gsPMKID;
	SPMKIDCandidateEvent gsPMKIDCandidate;

	/* for 802.11h */
	int b11hEnable;

	int bChannelSwitch;
	u8 byNewChannel;
	u8 byChannelSwitchCount;

	/* WPA supplicant daemon */
	int bWPADEVUp;
	int bwextstep0;
	int bwextstep1;
	int bwextstep2;
	int bwextstep3;
	int bWPASuppWextEnabled;

	/* user space daemon: hostapd, is used for HOSTAP */
	int bEnableHostapd;
	int bEnable8021x;
	int bEnableHostWEP;
	struct net_device *apdev;
	int (*tx_80211)(struct sk_buff *skb, struct net_device *dev);

	u32 uChannel;

	struct iw_statistics wstats; /* wireless stats */

	int bCommit;

};
>>>>>>> refs/remotes/origin/master

#define EnqueueRCB(_Head, _Tail, _RCB)                  \
{                                                       \
    if (!_Head) {                                       \
        _Head = _RCB;                                   \
    }                                                   \
    else {                                              \
        _Tail->Next = _RCB;                             \
    }                                                   \
    _RCB->Next = NULL;                                  \
    _Tail = _RCB;                                       \
}

#define DequeueRCB(Head, Tail)                          \
{                                                       \
<<<<<<< HEAD
    PRCB   RCB = Head;                                  \
=======
    struct vnt_rcb *RCB = Head;                         \
>>>>>>> refs/remotes/origin/master
    if (!RCB->Next) {                                   \
        Tail = NULL;                                    \
    }                                                   \
    Head = RCB->Next;                                   \
}

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
#define ADD_ONE_WITH_WRAP_AROUND(uVar, uModulo) {   \
    if ((uVar) >= ((uModulo) - 1))                  \
        (uVar) = 0;                                 \
    else                                            \
        (uVar)++;                                   \
}

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
#define fMP_RESET_IN_PROGRESS               0x00000001
#define fMP_DISCONNECTED                    0x00000002
#define fMP_HALT_IN_PROGRESS                0x00000004
#define fMP_SURPRISE_REMOVED                0x00000008
#define fMP_RECV_LOOKASIDE                  0x00000010
#define fMP_INIT_IN_PROGRESS                0x00000020
#define fMP_SEND_SIDE_RESOURCE_ALLOCATED    0x00000040
#define fMP_RECV_SIDE_RESOURCE_ALLOCATED    0x00000080
#define fMP_POST_READS                      0x00000100
#define fMP_POST_WRITES                     0x00000200
#define fMP_CONTROL_READS                   0x00000400
#define fMP_CONTROL_WRITES                  0x00000800

#define MP_SET_FLAG(_M, _F)             ((_M)->Flags |= (_F))
#define MP_CLEAR_FLAG(_M, _F)            ((_M)->Flags &= ~(_F))
#define MP_TEST_FLAGS(_M, _F)            (((_M)->Flags & (_F)) == (_F))

#define MP_IS_READY(_M)        (((_M)->Flags & \
                                 (fMP_DISCONNECTED | fMP_RESET_IN_PROGRESS | fMP_HALT_IN_PROGRESS | fMP_INIT_IN_PROGRESS | fMP_SURPRISE_REMOVED)) == 0)

<<<<<<< HEAD
/*---------------------  Export Functions  --------------------------*/

/* BOOL device_dma0_xmit(PSDevice pDevice, struct sk_buff *skb,
 *                       unsigned int uNodeIndex);
 */
BOOL device_alloc_frag_buf(PSDevice pDevice, PSDeFragControlBlock pDeF);
=======
int device_alloc_frag_buf(struct vnt_private *, PSDeFragControlBlock pDeF);
>>>>>>> refs/remotes/origin/master

#endif
