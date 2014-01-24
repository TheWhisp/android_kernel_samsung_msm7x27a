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
 * File: power.c
 *
<<<<<<< HEAD
 * Purpose: Handles 802.11 power management  functions
=======
 * Purpose: Handles 802.11 power management functions
>>>>>>> refs/remotes/origin/master
 *
 * Author: Lyndon Chen
 *
 * Date: July 17, 2002
 *
 * Functions:
 *      PSvEnablePowerSaving - Enable Power Saving Mode
 *      PSvDiasblePowerSaving - Disable Power Saving Mode
 *      PSbConsiderPowerDown - Decide if we can Power Down
 *      PSvSendPSPOLL - Send PS-POLL packet
 *      PSbSendNullPacket - Send Null packet
 *      PSbIsNextTBTTWakeUp - Decide if we need to wake up at next Beacon
 *
 * Revision History:
 *
 */

<<<<<<< HEAD
#include "ttype.h"
=======
>>>>>>> refs/remotes/origin/master
#include "mac.h"
#include "device.h"
#include "wmgr.h"
#include "power.h"
#include "wcmd.h"
#include "rxtx.h"
#include "card.h"
#include "control.h"
#include "rndis.h"

<<<<<<< HEAD
/*---------------------  Static Definitions -------------------------*/

/*---------------------  Static Classes  ----------------------------*/

/*---------------------  Static Variables  --------------------------*/
static int msglevel = MSG_LEVEL_INFO;
/*---------------------  Static Functions  --------------------------*/

/*---------------------  Export Variables  --------------------------*/

/*---------------------  Export Functions  --------------------------*/
=======
static int msglevel = MSG_LEVEL_INFO;
>>>>>>> refs/remotes/origin/master

/*
 *
 * Routine Description:
 * Enable hw power saving functions
 *
 * Return Value:
 *    None.
 *
 */

<<<<<<< HEAD
void PSvEnablePowerSaving(void *hDeviceContext,
			  WORD wListenInterval)
{
	PSDevice pDevice = (PSDevice)hDeviceContext;
	PSMgmtObject pMgmt = &(pDevice->sMgmtObj);
	WORD wAID = pMgmt->wCurrAID | BIT14 | BIT15;
=======
void PSvEnablePowerSaving(struct vnt_private *pDevice, u16 wListenInterval)
{
	struct vnt_manager *pMgmt = &pDevice->vnt_mgmt;
	u16 wAID = pMgmt->wCurrAID | BIT14 | BIT15;
>>>>>>> refs/remotes/origin/master

	/* set period of power up before TBTT */
	MACvWriteWord(pDevice, MAC_REG_PWBT, C_PWBT);

	if (pDevice->eOPMode != OP_MODE_ADHOC) {
		/* set AID */
		MACvWriteWord(pDevice, MAC_REG_AIDATIM, wAID);
	} else {
		/* set ATIM Window */
		/* MACvWriteATIMW(pDevice->PortOffset, pMgmt->wCurrATIMWindow); */
	}

	/* Warren:06-18-2004,the sequence must follow PSEN->AUTOSLEEP->GO2DOZE */
	/* enable power saving hw function */
	MACvRegBitsOn(pDevice, MAC_REG_PSCTL, PSCTL_PSEN);

	/* Set AutoSleep */
	MACvRegBitsOn(pDevice, MAC_REG_PSCFG, PSCFG_AUTOSLEEP);

	/* Warren:MUST turn on this once before turn on AUTOSLEEP ,or the AUTOSLEEP doesn't work */
	MACvRegBitsOn(pDevice, MAC_REG_PSCTL, PSCTL_GO2DOZE);

	if (wListenInterval >= 2) {

		/* clear always listen beacon */
		MACvRegBitsOff(pDevice, MAC_REG_PSCTL, PSCTL_ALBCN);

		/* first time set listen next beacon */
		MACvRegBitsOn(pDevice, MAC_REG_PSCTL, PSCTL_LNBCN);

		pMgmt->wCountToWakeUp = wListenInterval;

	} else {

		/* always listen beacon */
		MACvRegBitsOn(pDevice, MAC_REG_PSCTL, PSCTL_ALBCN);

		pMgmt->wCountToWakeUp = 0;
	}

<<<<<<< HEAD
	pDevice->bEnablePSMode = TRUE;
=======
	pDevice->bEnablePSMode = true;
>>>>>>> refs/remotes/origin/master

	/* We don't send null pkt in ad hoc mode since beacon will handle this. */
	if (pDevice->eOPMode == OP_MODE_INFRASTRUCTURE)
		PSbSendNullPacket(pDevice);

<<<<<<< HEAD
	pDevice->bPWBitOn = TRUE;
=======
	pDevice->bPWBitOn = true;
>>>>>>> refs/remotes/origin/master
	DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "PS:Power Saving Mode Enable...\n");
}

/*
 *
 * Routine Description:
 * Disable hw power saving functions
 *
 * Return Value:
 *    None.
 *
 */

<<<<<<< HEAD
void PSvDisablePowerSaving(void *hDeviceContext)
{
	PSDevice pDevice = (PSDevice)hDeviceContext;
	/* PSMgmtObject pMgmt = &(pDevice->sMgmtObj); */
=======
void PSvDisablePowerSaving(struct vnt_private *pDevice)
{
>>>>>>> refs/remotes/origin/master

	/* disable power saving hw function */
	CONTROLnsRequestOut(pDevice, MESSAGE_TYPE_DISABLE_PS, 0,
						0, 0, NULL);

	/* clear AutoSleep */
	MACvRegBitsOff(pDevice, MAC_REG_PSCFG, PSCFG_AUTOSLEEP);

	/* set always listen beacon */
	MACvRegBitsOn(pDevice, MAC_REG_PSCTL, PSCTL_ALBCN);
<<<<<<< HEAD
	pDevice->bEnablePSMode = FALSE;
=======
	pDevice->bEnablePSMode = false;
>>>>>>> refs/remotes/origin/master

	if (pDevice->eOPMode == OP_MODE_INFRASTRUCTURE)
		PSbSendNullPacket(pDevice);

<<<<<<< HEAD
	pDevice->bPWBitOn = FALSE;
=======
	pDevice->bPWBitOn = false;
>>>>>>> refs/remotes/origin/master
}

/*
 *
 * Routine Description:
 * Consider to power down when no more packets to tx or rx.
 *
 * Return Value:
<<<<<<< HEAD
 *    TRUE, if power down success
 *    FALSE, if fail
 */

BOOL PSbConsiderPowerDown(void *hDeviceContext,
			  BOOL bCheckRxDMA,
			  BOOL bCheckCountToWakeUp)
{
	PSDevice pDevice = (PSDevice)hDeviceContext;
	PSMgmtObject pMgmt = &(pDevice->sMgmtObj);
	BYTE byData;
=======
 *    true, if power down success
 *    false, if fail
 */

int PSbConsiderPowerDown(struct vnt_private *pDevice, int bCheckRxDMA,
	int bCheckCountToWakeUp)
{
	struct vnt_manager *pMgmt = &pDevice->vnt_mgmt;
	u8 byData;
>>>>>>> refs/remotes/origin/master

	/* check if already in Doze mode */
	ControlvReadByte(pDevice, MESSAGE_REQUEST_MACREG,
					MAC_REG_PSCTL, &byData);

	if ((byData & PSCTL_PS) != 0)
<<<<<<< HEAD
		return TRUE;
=======
		return true;
>>>>>>> refs/remotes/origin/master

	if (pMgmt->eCurrMode != WMAC_MODE_IBSS_STA) {
		/* check if in TIM wake period */
		if (pMgmt->bInTIMWake)
<<<<<<< HEAD
			return FALSE;
=======
			return false;
>>>>>>> refs/remotes/origin/master
	}

	/* check scan state */
	if (pDevice->bCmdRunning)
<<<<<<< HEAD
		return FALSE;

	/* Tx Burst */
	if (pDevice->bPSModeTxBurst)
		return FALSE;
=======
		return false;

	/* Tx Burst */
	if (pDevice->bPSModeTxBurst)
		return false;
>>>>>>> refs/remotes/origin/master

	/* Froce PSEN on */
	MACvRegBitsOn(pDevice, MAC_REG_PSCTL, PSCTL_PSEN);

	if (pMgmt->eCurrMode != WMAC_MODE_IBSS_STA) {
		if (bCheckCountToWakeUp && (pMgmt->wCountToWakeUp == 0
			|| pMgmt->wCountToWakeUp == 1)) {
<<<<<<< HEAD
				return FALSE;
		}
	}

	pDevice->bPSRxBeacon = TRUE;
=======
				return false;
		}
	}

	pDevice->bPSRxBeacon = true;
>>>>>>> refs/remotes/origin/master

	/* no Tx, no Rx isr, now go to Doze */
	MACvRegBitsOn(pDevice, MAC_REG_PSCTL, PSCTL_GO2DOZE);
	DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Go to Doze ZZZZZZZZZZZZZZZ\n");
<<<<<<< HEAD
	return TRUE;
=======
	return true;
>>>>>>> refs/remotes/origin/master
}

/*
 *
 * Routine Description:
 * Send PS-POLL packet
 *
 * Return Value:
 *    None.
 *
 */

<<<<<<< HEAD
void PSvSendPSPOLL(void *hDeviceContext)
{
	PSDevice pDevice = (PSDevice)hDeviceContext;
	PSMgmtObject pMgmt = &(pDevice->sMgmtObj);
	PSTxMgmtPacket pTxPacket = NULL;

	memset(pMgmt->pbyPSPacketPool, 0, sizeof(STxMgmtPacket) + WLAN_HDR_ADDR2_LEN);
	pTxPacket = (PSTxMgmtPacket)pMgmt->pbyPSPacketPool;
	pTxPacket->p80211Header = (PUWLAN_80211HDR)((PBYTE)pTxPacket + sizeof(STxMgmtPacket));
=======
void PSvSendPSPOLL(struct vnt_private *pDevice)
{
	struct vnt_manager *pMgmt = &pDevice->vnt_mgmt;
	struct vnt_tx_mgmt *pTxPacket = NULL;

	memset(pMgmt->pbyPSPacketPool, 0, sizeof(struct vnt_tx_mgmt)
		+ WLAN_HDR_ADDR2_LEN);
	pTxPacket = (struct vnt_tx_mgmt *)pMgmt->pbyPSPacketPool;
	pTxPacket->p80211Header = (PUWLAN_80211HDR)((u8 *)pTxPacket
		+ sizeof(struct vnt_tx_mgmt));

>>>>>>> refs/remotes/origin/master
	pTxPacket->p80211Header->sA2.wFrameCtl = cpu_to_le16(
		(
			WLAN_SET_FC_FTYPE(WLAN_TYPE_CTL) |
			WLAN_SET_FC_FSTYPE(WLAN_FSTYPE_PSPOLL) |
			WLAN_SET_FC_PWRMGT(0)
		));

	pTxPacket->p80211Header->sA2.wDurationID = pMgmt->wCurrAID | BIT14 | BIT15;
	memcpy(pTxPacket->p80211Header->sA2.abyAddr1, pMgmt->abyCurrBSSID, WLAN_ADDR_LEN);
	memcpy(pTxPacket->p80211Header->sA2.abyAddr2, pMgmt->abyMACAddr, WLAN_ADDR_LEN);
	pTxPacket->cbMPDULen = WLAN_HDR_ADDR2_LEN;
	pTxPacket->cbPayloadLen = 0;

	/* log failure if sending failed */
<<<<<<< HEAD
	if (csMgmt_xmit(pDevice, pTxPacket) != CMD_STATUS_PENDING) {
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Send PS-Poll packet failed..\n");
	}
=======
	if (csMgmt_xmit(pDevice, pTxPacket) != CMD_STATUS_PENDING)
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Send PS-Poll packet failed..\n");
>>>>>>> refs/remotes/origin/master
}

/*
 *
 * Routine Description:
 * Send NULL packet to AP for notification power state of STA
 *
 * Return Value:
 *    None.
 *
 */

<<<<<<< HEAD
BOOL PSbSendNullPacket(void *hDeviceContext)
{
	PSDevice pDevice = (PSDevice)hDeviceContext;
	PSTxMgmtPacket pTxPacket = NULL;
	PSMgmtObject pMgmt = &(pDevice->sMgmtObj);
	u16 flags = 0;

	if (pDevice->bLinkPass == FALSE)
		return FALSE;

	if ((pDevice->bEnablePSMode == FALSE) &&
		(pDevice->fTxDataInSleep == FALSE)) {
			return FALSE;
	}

	memset(pMgmt->pbyPSPacketPool, 0, sizeof(STxMgmtPacket) + WLAN_NULLDATA_FR_MAXLEN);
	pTxPacket = (PSTxMgmtPacket)pMgmt->pbyPSPacketPool;
	pTxPacket->p80211Header = (PUWLAN_80211HDR)((PBYTE)pTxPacket + sizeof(STxMgmtPacket));

	flags = WLAN_SET_FC_FTYPE(WLAN_TYPE_DATA) |
                        WLAN_SET_FC_FSTYPE(WLAN_FSTYPE_NULL);
=======
int PSbSendNullPacket(struct vnt_private *pDevice)
{
	struct vnt_tx_mgmt *pTxPacket = NULL;
	struct vnt_manager *pMgmt = &pDevice->vnt_mgmt;
	u16 flags = 0;

	if (pDevice->bLinkPass == false)
		return false;

	if (pDevice->bEnablePSMode == false && pDevice->tx_trigger == false)
		return false;

	memset(pMgmt->pbyPSPacketPool, 0, sizeof(struct vnt_tx_mgmt)
		+ WLAN_NULLDATA_FR_MAXLEN);
	pTxPacket = (struct vnt_tx_mgmt *)pMgmt->pbyPSPacketPool;
	pTxPacket->p80211Header = (PUWLAN_80211HDR)((u8 *)pTxPacket
		+ sizeof(struct vnt_tx_mgmt));

	flags = WLAN_SET_FC_FTYPE(WLAN_TYPE_DATA) |
			WLAN_SET_FC_FSTYPE(WLAN_FSTYPE_NULL);
>>>>>>> refs/remotes/origin/master

	if (pDevice->bEnablePSMode)
		flags |= WLAN_SET_FC_PWRMGT(1);
	else
		flags |= WLAN_SET_FC_PWRMGT(0);

	pTxPacket->p80211Header->sA3.wFrameCtl = cpu_to_le16(flags);

	if (pMgmt->eCurrMode != WMAC_MODE_IBSS_STA)
<<<<<<< HEAD
		pTxPacket->p80211Header->sA3.wFrameCtl |= cpu_to_le16((WORD)WLAN_SET_FC_TODS(1));
=======
		pTxPacket->p80211Header->sA3.wFrameCtl |= cpu_to_le16((u16)WLAN_SET_FC_TODS(1));
>>>>>>> refs/remotes/origin/master

	memcpy(pTxPacket->p80211Header->sA3.abyAddr1, pMgmt->abyCurrBSSID, WLAN_ADDR_LEN);
	memcpy(pTxPacket->p80211Header->sA3.abyAddr2, pMgmt->abyMACAddr, WLAN_ADDR_LEN);
	memcpy(pTxPacket->p80211Header->sA3.abyAddr3, pMgmt->abyCurrBSSID, WLAN_BSSID_LEN);
	pTxPacket->cbMPDULen = WLAN_HDR_ADDR3_LEN;
	pTxPacket->cbPayloadLen = 0;
	/* log error if sending failed */
	if (csMgmt_xmit(pDevice, pTxPacket) != CMD_STATUS_PENDING) {
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO "Send Null Packet failed !\n");
<<<<<<< HEAD
		return FALSE;
	}
	return TRUE;
=======
		return false;
	}
	return true;
>>>>>>> refs/remotes/origin/master
}

/*
 *
 * Routine Description:
 * Check if Next TBTT must wake up
 *
 * Return Value:
 *    None.
 *
 */

<<<<<<< HEAD
BOOL PSbIsNextTBTTWakeUp(void *hDeviceContext)
{
	PSDevice pDevice = (PSDevice)hDeviceContext;
	PSMgmtObject pMgmt = &(pDevice->sMgmtObj);
	BOOL bWakeUp = FALSE;
=======
int PSbIsNextTBTTWakeUp(struct vnt_private *pDevice)
{
	struct vnt_manager *pMgmt = &pDevice->vnt_mgmt;
	int bWakeUp = false;
>>>>>>> refs/remotes/origin/master

	if (pMgmt->wListenInterval >= 2) {
		if (pMgmt->wCountToWakeUp == 0)
			pMgmt->wCountToWakeUp = pMgmt->wListenInterval;

		pMgmt->wCountToWakeUp--;

		if (pMgmt->wCountToWakeUp == 1) {
			/* Turn on wake up to listen next beacon */
			MACvRegBitsOn(pDevice, MAC_REG_PSCTL, PSCTL_LNBCN);
<<<<<<< HEAD
			pDevice->bPSRxBeacon = FALSE;
			bWakeUp = TRUE;
=======
			pDevice->bPSRxBeacon = false;
			bWakeUp = true;
>>>>>>> refs/remotes/origin/master
		} else if (!pDevice->bPSRxBeacon) {
			/* Listen until RxBeacon */
			MACvRegBitsOn(pDevice, MAC_REG_PSCTL, PSCTL_LNBCN);
		}
	}
	return bWakeUp;
}

