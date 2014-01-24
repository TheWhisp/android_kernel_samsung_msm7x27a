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
 * File: rndis.h
 *
 * Purpose: Interface between firmware and driver
 *
 * Author: Warren Hsu
 *
 * Date: Nov 24, 2004
 *
 */

<<<<<<< HEAD

#ifndef __RNDIS_H__
#define __RNDIS_H__

/*---------------------  Export Definitions -------------------------*/
=======
#ifndef __RNDIS_H__
#define __RNDIS_H__

>>>>>>> refs/remotes/origin/master
#define MESSAGE_TYPE_READ               0x01
#define MESSAGE_TYPE_WRITE              0x00
#define MESSAGE_TYPE_LOCK_OR            0x02
#define MESSAGE_TYPE_LOCK_AND           0x03
#define MESSAGE_TYPE_WRITE_MASK         0x04
#define MESSAGE_TYPE_CARDINIT           0x05
#define MESSAGE_TYPE_INIT_RSP           0x06
#define MESSAGE_TYPE_MACSHUTDOWN        0x07
#define MESSAGE_TYPE_SETKEY             0x08
#define MESSAGE_TYPE_CLRKEYENTRY        0x09
#define MESSAGE_TYPE_WRITE_MISCFF       0x0A
#define MESSAGE_TYPE_SET_ANTMD          0x0B
#define MESSAGE_TYPE_SELECT_CHANNLE     0x0C
#define MESSAGE_TYPE_SET_TSFTBTT        0x0D
#define MESSAGE_TYPE_SET_SSTIFS         0x0E
#define MESSAGE_TYPE_CHANGE_BBTYPE      0x0F
#define MESSAGE_TYPE_DISABLE_PS         0x10
#define MESSAGE_TYPE_WRITE_IFRF         0x11

//used for read/write(index)
#define MESSAGE_REQUEST_MEM             0x01
#define MESSAGE_REQUEST_BBREG           0x02
#define MESSAGE_REQUEST_MACREG          0x03
#define MESSAGE_REQUEST_EEPROM          0x04
#define MESSAGE_REQUEST_TSF             0x05
#define MESSAGE_REQUEST_TBTT            0x06
#define MESSAGE_REQUEST_BBAGC           0x07
#define MESSAGE_REQUEST_VERSION         0x08
#define MESSAGE_REQUEST_RF_INIT         0x09
#define MESSAGE_REQUEST_RF_INIT2        0x0A
#define MESSAGE_REQUEST_RF_CH0          0x0B
#define MESSAGE_REQUEST_RF_CH1          0x0C
#define MESSAGE_REQUEST_RF_CH2          0x0D

<<<<<<< HEAD

#define VIAUSB20_PACKET_HEADER          0x04


/*---------------------  Export Classes  ----------------------------*/

typedef struct _CMD_MESSAGE
{
    BYTE        byData[256];
=======
#define VIAUSB20_PACKET_HEADER          0x04

#define USB_REG4	0x604

typedef struct _CMD_MESSAGE
{
    u8        byData[256];
>>>>>>> refs/remotes/origin/master
} CMD_MESSAGE, *PCMD_MESSAGE;

typedef struct _CMD_WRITE_MASK
{
<<<<<<< HEAD
    BYTE        byData;
    BYTE        byMask;
} CMD_WRITE_MASK, *PCMD_WRITE_MASK;

typedef struct _CMD_CARD_INIT
{
    BYTE        byInitClass;
    BYTE        bExistSWNetAddr;
    BYTE        bySWNetAddr[6];
    BYTE        byShortRetryLimit;
    BYTE        byLongRetryLimit;
} CMD_CARD_INIT, *PCMD_CARD_INIT;

typedef struct _RSP_CARD_INIT
{
    BYTE        byStatus;
    BYTE        byNetAddr[6];
    BYTE        byRFType;
    BYTE        byMinChannel;
    BYTE        byMaxChannel;
} RSP_CARD_INIT, *PRSP_CARD_INIT;

typedef struct _CMD_SET_KEY
{
    WORD        wKCTL;
    BYTE        abyMacAddr[6];
    BYTE        abyKey[16];
=======
    u8        byData;
    u8        byMask;
} CMD_WRITE_MASK, *PCMD_WRITE_MASK;

struct vnt_cmd_card_init
{
	u8 init_class;
	u8 exist_sw_net_addr;
	u8 sw_net_addr[6];
	u8 short_retry_limit;
	u8 long_retry_limit;
};

struct vnt_rsp_card_init
{
	u8 status;
	u8 net_addr[6];
	u8 rf_type;
	u8 min_channel;
	u8 max_channel;
};

typedef struct _CMD_SET_KEY
{
    u16        wKCTL;
    u8        abyMacAddr[6];
    u8        abyKey[16];
>>>>>>> refs/remotes/origin/master
} CMD_SET_KEY, *PCMD_SET_KEY;

typedef struct _CMD_CLRKEY_ENTRY
{
<<<<<<< HEAD
    BYTE        abyKeyEntry[11];
=======
    u8        abyKeyEntry[11];
>>>>>>> refs/remotes/origin/master
} CMD_CLRKEY_ENTRY, *PCMD_CLRKEY_ENTRY;

typedef struct _CMD_WRITE_MISCFF
{
<<<<<<< HEAD
    DWORD       adwMiscFFData[22][4];  //a key entry has only 22 dwords
=======
    u32       adwMiscFFData[22][4];  //a key entry has only 22 dwords
>>>>>>> refs/remotes/origin/master
} CMD_WRITE_MISCFF, *PCMD_WRITE_MISCFF;

typedef struct _CMD_SET_TSFTBTT
{
<<<<<<< HEAD
    BYTE        abyTSF_TBTT[8];
=======
    u8        abyTSF_TBTT[8];
>>>>>>> refs/remotes/origin/master
} CMD_SET_TSFTBTT, *PCMD_SET_TSFTBTT;

typedef struct _CMD_SET_SSTIFS
{
<<<<<<< HEAD
    BYTE        bySIFS;
    BYTE        byDIFS;
    BYTE        byEIFS;
    BYTE        bySlotTime;
    BYTE        byCwMax_Min;
    BYTE        byBBCR10;
=======
    u8        bySIFS;
    u8        byDIFS;
    u8        byEIFS;
    u8        bySlotTime;
    u8        byCwMax_Min;
    u8        byBBCR10;
>>>>>>> refs/remotes/origin/master
} CMD_SET_SSTIFS, *PCMD_SET_SSTIFS;

typedef struct _CMD_CHANGE_BBTYPE
{
<<<<<<< HEAD
    BYTE        bySIFS;
    BYTE        byDIFS;
    BYTE        byEIFS;
    BYTE        bySlotTime;
    BYTE        byCwMax_Min;
    BYTE        byBBCR10;
    BYTE        byBB_BBType;    //CR88
    BYTE        byMAC_BBType;
    DWORD       dwRSPINF_b_1;
    DWORD       dwRSPINF_b_2;
    DWORD       dwRSPINF_b_55;
    DWORD       dwRSPINF_b_11;
    WORD        wRSPINF_a[9];
} CMD_CHANGE_BBTYPE, *PCMD_CHANGE_BBTYPE;

/*---------------------  Export Macros -------------------------*/

#define EXCH_WORD(w) ((WORD)((WORD)(w)<<8) | (WORD)((WORD)(w)>>8))

/*---------------------  Export Variables  --------------------------*/

/*---------------------  Export Functions  --------------------------*/
=======
    u8        bySIFS;
    u8        byDIFS;
    u8        byEIFS;
    u8        bySlotTime;
    u8        byCwMax_Min;
    u8        byBBCR10;
    u8        byBB_BBType;    //CR88
    u8        byMAC_BBType;
    u32       dwRSPINF_b_1;
    u32       dwRSPINF_b_2;
    u32       dwRSPINF_b_55;
    u32       dwRSPINF_b_11;
    u16        wRSPINF_a[9];
} CMD_CHANGE_BBTYPE, *PCMD_CHANGE_BBTYPE;

#define EXCH_WORD(w) ((u16)((u16)(w)<<8) | (u16)((u16)(w)>>8))
>>>>>>> refs/remotes/origin/master

#endif /* _RNDIS_H_ */
