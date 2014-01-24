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
 * File: baseband.c
 *
 * Purpose: Implement functions to access baseband
 *
 * Author: Yiching Chen
 *
 * Date: May 20, 2004
 *
 * Functions:
 *
 * Revision History:
 *
 */

#include "firmware.h"
#include "control.h"
#include "rndis.h"

<<<<<<< HEAD
/*---------------------  Static Definitions -------------------------*/

static int          msglevel                =MSG_LEVEL_INFO;
//static int          msglevel                =MSG_LEVEL_DEBUG;
=======
static int msglevel = MSG_LEVEL_INFO;
/* static int msglevel = MSG_LEVEL_DEBUG; */
>>>>>>> refs/remotes/origin/master

#define FIRMWARE_VERSION	0x133		/* version 1.51 */
#define FIRMWARE_NAME		"vntwusb.fw"

#define FIRMWARE_CHUNK_SIZE	0x400

<<<<<<< HEAD
/*---------------------  Static Classes  ----------------------------*/

/*---------------------  Static Variables  --------------------------*/

/*---------------------  Static Functions  --------------------------*/

/*---------------------  Export Variables  --------------------------*/

/*---------------------  Export Functions  --------------------------*/


BOOL
FIRMWAREbDownload(
     PSDevice pDevice
    )
{
	const struct firmware *fw;
	int NdisStatus;
	void *pBuffer = NULL;
	BOOL result = FALSE;
	u16 wLength;
	int ii;
=======
int FIRMWAREbDownload(struct vnt_private *pDevice)
{
	struct device *dev = &pDevice->usb->dev;
	const struct firmware *fw;
	int NdisStatus;
	void *pBuffer = NULL;
	bool result = false;
	u16 wLength;
	int ii, rc;
>>>>>>> refs/remotes/origin/master

	DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"---->Download firmware\n");
	spin_unlock_irq(&pDevice->lock);

<<<<<<< HEAD
	if (!pDevice->firmware) {
		struct device *dev = &pDevice->usb->dev;
		int rc;

		rc = request_firmware(&pDevice->firmware, FIRMWARE_NAME, dev);
		if (rc) {
			dev_err(dev, "firmware file %s request failed (%d)\n",
				FIRMWARE_NAME, rc);
			goto out;
		}
	}
	fw = pDevice->firmware;
=======
	rc = request_firmware(&fw, FIRMWARE_NAME, dev);
	if (rc) {
		dev_err(dev, "firmware file %s request failed (%d)\n",
			FIRMWARE_NAME, rc);
			goto out;
	}
>>>>>>> refs/remotes/origin/master

	pBuffer = kmalloc(FIRMWARE_CHUNK_SIZE, GFP_KERNEL);
	if (!pBuffer)
		goto out;

	for (ii = 0; ii < fw->size; ii += FIRMWARE_CHUNK_SIZE) {
		wLength = min_t(int, fw->size - ii, FIRMWARE_CHUNK_SIZE);
		memcpy(pBuffer, fw->data + ii, wLength);

		NdisStatus = CONTROLnsRequestOutAsyn(pDevice,
<<<<<<< HEAD
                                            0,
                                            0x1200+ii,
                                            0x0000,
                                            wLength,
                                            pBuffer
                                            );
=======
						0,
						0x1200+ii,
						0x0000,
						wLength,
						pBuffer);
>>>>>>> refs/remotes/origin/master

		DBG_PRT(MSG_LEVEL_DEBUG,
			KERN_INFO"Download firmware...%d %zu\n", ii, fw->size);
		if (NdisStatus != STATUS_SUCCESS)
<<<<<<< HEAD
			goto out;
        }

	result = TRUE;
=======
			goto free_fw;
	}

	result = true;
free_fw:
	release_firmware(fw);
>>>>>>> refs/remotes/origin/master

out:
	kfree(pBuffer);

	spin_lock_irq(&pDevice->lock);
	return result;
}
MODULE_FIRMWARE(FIRMWARE_NAME);

<<<<<<< HEAD
BOOL
FIRMWAREbBrach2Sram(
     PSDevice pDevice
    )
{
    int NdisStatus;

    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"---->Branch to Sram\n");

    NdisStatus = CONTROLnsRequestOut(pDevice,
                                    1,
                                    0x1200,
                                    0x0000,
                                    0,
                                    NULL
                                    );

    if (NdisStatus != STATUS_SUCCESS) {
        return (FALSE);
    } else {
        return (TRUE);
    }
}


BOOL
FIRMWAREbCheckVersion(
     PSDevice pDevice
    )
{
	int ntStatus;

    ntStatus = CONTROLnsRequestIn(pDevice,
                                    MESSAGE_TYPE_READ,
                                    0,
                                    MESSAGE_REQUEST_VERSION,
                                    2,
                                    (PBYTE) &(pDevice->wFirmwareVersion));

    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Firmware Version [%04x]\n", pDevice->wFirmwareVersion);
    if (ntStatus != STATUS_SUCCESS) {
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Firmware Invalid.\n");
        return FALSE;
    }
    if (pDevice->wFirmwareVersion == 0xFFFF) {
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"In Loader.\n");
        return FALSE;
    }
    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Firmware Version [%04x]\n", pDevice->wFirmwareVersion);
    if (pDevice->wFirmwareVersion < FIRMWARE_VERSION) {
        // branch to loader for download new firmware
        FIRMWAREbBrach2Sram(pDevice);
        return FALSE;
    }
    return TRUE;
=======
int FIRMWAREbBrach2Sram(struct vnt_private *pDevice)
{
	int NdisStatus;

	DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"---->Branch to Sram\n");

	NdisStatus = CONTROLnsRequestOut(pDevice,
					1,
					0x1200,
					0x0000,
					0,
					NULL);
	if (NdisStatus != STATUS_SUCCESS)
		return false;
	else
		return true;
}

int FIRMWAREbCheckVersion(struct vnt_private *pDevice)
{
	int ntStatus;

	ntStatus = CONTROLnsRequestIn(pDevice,
					MESSAGE_TYPE_READ,
					0,
					MESSAGE_REQUEST_VERSION,
					2,
					(u8 *) &(pDevice->wFirmwareVersion));

	DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Firmware Version [%04x]\n",
						pDevice->wFirmwareVersion);
	if (ntStatus != STATUS_SUCCESS) {
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Firmware Invalid.\n");
		return false;
	}
	if (pDevice->wFirmwareVersion == 0xFFFF) {
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"In Loader.\n");
		return false;
	}
	DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Firmware Version [%04x]\n",
						pDevice->wFirmwareVersion);
	if (pDevice->wFirmwareVersion < FIRMWARE_VERSION) {
		/* branch to loader for download new firmware */
		FIRMWAREbBrach2Sram(pDevice);
		return false;
	}
	return true;
>>>>>>> refs/remotes/origin/master
}
