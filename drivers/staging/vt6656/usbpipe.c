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
 * File: usbpipe.c
 *
 * Purpose: Handle USB control endpoint
 *
 * Author: Warren Hsu
 *
 * Date: Mar. 29, 2005
 *
 * Functions:
 *      CONTROLnsRequestOut - Write variable length bytes to MEM/BB/MAC/EEPROM
 *      CONTROLnsRequestIn - Read variable length bytes from MEM/BB/MAC/EEPROM
 *      ControlvWriteByte - Write one byte to MEM/BB/MAC/EEPROM
 *      ControlvReadByte - Read one byte from MEM/BB/MAC/EEPROM
 *      ControlvMaskByte - Read one byte from MEM/BB/MAC/EEPROM and clear/set some bits in the same address
 *
 * Revision History:
 *      04-05-2004 Jerry Chen:  Initial release
 *      11-24-2004 Warren Hsu: Add ControlvWriteByte,ControlvReadByte,ControlvMaskByte
 *
 */

#include "int.h"
#include "rxtx.h"
#include "dpc.h"
#include "control.h"
#include "desc.h"
#include "device.h"

<<<<<<< HEAD
/*---------------------  Static Definitions -------------------------*/
=======
>>>>>>> refs/remotes/origin/master
//endpoint def
//endpoint 0: control
//endpoint 1: interrupt
//endpoint 2: read bulk
//endpoint 3: write bulk

<<<<<<< HEAD
//RequestType:
//#define REQUEST_OUT       (USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE) // 0x40
//#define REQUEST_IN        (USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_DEVICE )  //0xc0
//static int          msglevel                =MSG_LEVEL_DEBUG;
static int          msglevel                =MSG_LEVEL_INFO;


=======
//static int          msglevel                =MSG_LEVEL_DEBUG;
static int          msglevel                =MSG_LEVEL_INFO;

>>>>>>> refs/remotes/origin/master
#define USB_CTL_WAIT   500 //ms

#ifndef URB_ASYNC_UNLINK
#define URB_ASYNC_UNLINK    0
#endif

<<<<<<< HEAD
/*---------------------  Static Classes  ----------------------------*/

/*---------------------  Static Variables  --------------------------*/

/*---------------------  Static Functions  --------------------------*/
static
void
s_nsInterruptUsbIoCompleteRead(
     struct urb *urb
    );


static
void
s_nsBulkInUsbIoCompleteRead(
     struct urb *urb
    );


static
void
s_nsBulkOutIoCompleteWrite(
     struct urb *urb
    );


static
void
s_nsControlInUsbIoCompleteRead(
     struct urb *urb
    );

static
void
s_nsControlInUsbIoCompleteWrite(
     struct urb *urb
    );

/*---------------------  Export Variables  --------------------------*/

/*---------------------  Export Functions  --------------------------*/

int PIPEnsControlOutAsyn(
     PSDevice     pDevice,
     BYTE         byRequest,
     WORD         wValue,
     WORD         wIndex,
     WORD         wLength,
     PBYTE        pbyBuffer
    )
=======
static void s_nsInterruptUsbIoCompleteRead(struct urb *urb);
static void s_nsBulkInUsbIoCompleteRead(struct urb *urb);
static void s_nsBulkOutIoCompleteWrite(struct urb *urb);
static void s_nsControlInUsbIoCompleteRead(struct urb *urb);
static void s_nsControlInUsbIoCompleteWrite(struct urb *urb);

int PIPEnsControlOutAsyn(struct vnt_private *pDevice, u8 byRequest,
	u16 wValue, u16 wIndex, u16 wLength, u8 *pbyBuffer)
>>>>>>> refs/remotes/origin/master
{
	int ntStatus;

    if (pDevice->Flags & fMP_DISCONNECTED)
        return STATUS_FAILURE;

    if (pDevice->Flags & fMP_CONTROL_WRITES)
        return STATUS_FAILURE;

    if (in_interrupt()) {
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"in_interrupt return ..byRequest %x\n", byRequest);
        return STATUS_FAILURE;
    }

    ntStatus = usb_control_msg(
                            pDevice->usb,
                            usb_sndctrlpipe(pDevice->usb , 0),
                            byRequest,
                            0x40, // RequestType
                            wValue,
                            wIndex,
			    (void *) pbyBuffer,
                            wLength,
                            HZ
                          );
    if (ntStatus >= 0) {
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"usb_sndctrlpipe ntStatus= %d\n", ntStatus);
        ntStatus = 0;
    } else {
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"usb_sndctrlpipe fail, ntStatus= %d\n", ntStatus);
    }

    return ntStatus;
}

<<<<<<< HEAD
int PIPEnsControlOut(
     PSDevice     pDevice,
     BYTE         byRequest,
     WORD         wValue,
     WORD         wIndex,
     WORD         wLength,
     PBYTE        pbyBuffer
    )
{
	int ntStatus = 0;
    int ii;
=======
int PIPEnsControlOut(struct vnt_private *pDevice, u8 byRequest, u16 wValue,
		u16 wIndex, u16 wLength, u8 *pbyBuffer)
{
	int ntStatus = 0;
	int ii;
>>>>>>> refs/remotes/origin/master

    if (pDevice->Flags & fMP_DISCONNECTED)
        return STATUS_FAILURE;

    if (pDevice->Flags & fMP_CONTROL_WRITES)
        return STATUS_FAILURE;

	if (pDevice->Flags & fMP_CONTROL_READS)
		return STATUS_FAILURE;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (pDevice->pControlURB->hcpriv)
		return STATUS_FAILURE;

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	MP_SET_FLAG(pDevice, fMP_CONTROL_WRITES);

	pDevice->sUsbCtlRequest.bRequestType = 0x40;
	pDevice->sUsbCtlRequest.bRequest = byRequest;
	pDevice->sUsbCtlRequest.wValue = cpu_to_le16p(&wValue);
	pDevice->sUsbCtlRequest.wIndex = cpu_to_le16p(&wIndex);
	pDevice->sUsbCtlRequest.wLength = cpu_to_le16p(&wLength);
	pDevice->pControlURB->transfer_flags |= URB_ASYNC_UNLINK;
    pDevice->pControlURB->actual_length = 0;
    // Notice, pbyBuffer limited point to variable buffer, can't be constant.
  	usb_fill_control_urb(pDevice->pControlURB, pDevice->usb,
			 usb_sndctrlpipe(pDevice->usb , 0), (char *) &pDevice->sUsbCtlRequest,
			 pbyBuffer, wLength, s_nsControlInUsbIoCompleteWrite, pDevice);

	ntStatus = usb_submit_urb(pDevice->pControlURB, GFP_ATOMIC);
	if (ntStatus != 0) {
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO
			"control send request submission failed: %d\n",
				ntStatus);
		MP_CLEAR_FLAG(pDevice, fMP_CONTROL_WRITES);
		return STATUS_FAILURE;
	}

	spin_unlock_irq(&pDevice->lock);
    for (ii = 0; ii <= USB_CTL_WAIT; ii ++) {

	if (pDevice->Flags & fMP_CONTROL_WRITES)
		mdelay(1);
        else
		break;

        if (ii >= USB_CTL_WAIT) {
		DBG_PRT(MSG_LEVEL_DEBUG,
			KERN_INFO "control send request submission timeout\n");
            spin_lock_irq(&pDevice->lock);
            MP_CLEAR_FLAG(pDevice, fMP_CONTROL_WRITES);
            return STATUS_FAILURE;
        }
    }
	spin_lock_irq(&pDevice->lock);

    return STATUS_SUCCESS;
}

<<<<<<< HEAD
int PIPEnsControlIn(
     PSDevice     pDevice,
     BYTE         byRequest,
     WORD         wValue,
     WORD         wIndex,
     WORD         wLength,
       PBYTE   pbyBuffer
    )
{
	int ntStatus = 0;
    int ii;
=======
int PIPEnsControlIn(struct vnt_private *pDevice, u8 byRequest, u16 wValue,
	u16 wIndex, u16 wLength,  u8 *pbyBuffer)
{
	int ntStatus = 0;
	int ii;
>>>>>>> refs/remotes/origin/master

    if (pDevice->Flags & fMP_DISCONNECTED)
        return STATUS_FAILURE;

    if (pDevice->Flags & fMP_CONTROL_READS)
	return STATUS_FAILURE;

	if (pDevice->Flags & fMP_CONTROL_WRITES)
		return STATUS_FAILURE;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (pDevice->pControlURB->hcpriv)
		return STATUS_FAILURE;

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	MP_SET_FLAG(pDevice, fMP_CONTROL_READS);

	pDevice->sUsbCtlRequest.bRequestType = 0xC0;
	pDevice->sUsbCtlRequest.bRequest = byRequest;
	pDevice->sUsbCtlRequest.wValue = cpu_to_le16p(&wValue);
	pDevice->sUsbCtlRequest.wIndex = cpu_to_le16p(&wIndex);
	pDevice->sUsbCtlRequest.wLength = cpu_to_le16p(&wLength);
	pDevice->pControlURB->transfer_flags |= URB_ASYNC_UNLINK;
    pDevice->pControlURB->actual_length = 0;
	usb_fill_control_urb(pDevice->pControlURB, pDevice->usb,
			 usb_rcvctrlpipe(pDevice->usb , 0), (char *) &pDevice->sUsbCtlRequest,
			 pbyBuffer, wLength, s_nsControlInUsbIoCompleteRead, pDevice);

	ntStatus = usb_submit_urb(pDevice->pControlURB, GFP_ATOMIC);
	if (ntStatus != 0) {
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO
			"control request submission failed: %d\n", ntStatus);
		MP_CLEAR_FLAG(pDevice, fMP_CONTROL_READS);
		return STATUS_FAILURE;
	}

	spin_unlock_irq(&pDevice->lock);
    for (ii = 0; ii <= USB_CTL_WAIT; ii ++) {

	if (pDevice->Flags & fMP_CONTROL_READS)
		mdelay(1);
	else
		break;

	if (ii >= USB_CTL_WAIT) {
		DBG_PRT(MSG_LEVEL_DEBUG,
			KERN_INFO "control rcv request submission timeout\n");
            spin_lock_irq(&pDevice->lock);
            MP_CLEAR_FLAG(pDevice, fMP_CONTROL_READS);
            return STATUS_FAILURE;
        }
    }
	spin_lock_irq(&pDevice->lock);

    return ntStatus;
}

<<<<<<< HEAD
static
void
s_nsControlInUsbIoCompleteWrite(
     struct urb *urb
    )
{
    PSDevice        pDevice;
=======
static void s_nsControlInUsbIoCompleteWrite(struct urb *urb)
{
	struct vnt_private *pDevice = (struct vnt_private *)urb->context;
>>>>>>> refs/remotes/origin/master

	pDevice = urb->context;
	switch (urb->status) {
	case 0:
		break;
	case -EINPROGRESS:
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"ctrl write urb status EINPROGRESS%d\n", urb->status);
		break;
	case -ENOENT:
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"ctrl write urb status ENOENT %d\n", urb->status);
		break;
	default:
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"ctrl write urb status %d\n", urb->status);
	}

    MP_CLEAR_FLAG(pDevice, fMP_CONTROL_WRITES);
}

<<<<<<< HEAD


=======
>>>>>>> refs/remotes/origin/master
/*
 * Description:
 *      Complete function of usb Control callback
 *
 * Parameters:
 *  In:
 *      pDevice     - Pointer to the adapter
 *
 *  Out:
 *      none
 *
 * Return Value: STATUS_INSUFFICIENT_RESOURCES or result of IoCallDriver
 *
 */
<<<<<<< HEAD
static
void
s_nsControlInUsbIoCompleteRead(
     struct urb *urb
    )
{
    PSDevice        pDevice;

	pDevice = urb->context;
=======

static void s_nsControlInUsbIoCompleteRead(struct urb *urb)
{
	struct vnt_private *pDevice = (struct vnt_private *)urb->context;

>>>>>>> refs/remotes/origin/master
	switch (urb->status) {
	case 0:
		break;
	case -EINPROGRESS:
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"ctrl read urb status EINPROGRESS%d\n", urb->status);
		break;
	case -ENOENT:
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"ctrl read urb status = ENOENT %d\n", urb->status);
		break;
	default:
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"ctrl read urb status %d\n", urb->status);
	}

    MP_CLEAR_FLAG(pDevice, fMP_CONTROL_READS);
}

<<<<<<< HEAD



=======
>>>>>>> refs/remotes/origin/master
/*
 * Description:
 *      Allocates an usb interrupt in irp and calls USBD.
 *
 * Parameters:
 *  In:
 *      pDevice     - Pointer to the adapter
 *  Out:
 *      none
 *
 * Return Value: STATUS_INSUFFICIENT_RESOURCES or result of IoCallDriver
 *
 */
<<<<<<< HEAD
int PIPEnsInterruptRead(PSDevice pDevice)
{
    int ntStatus = STATUS_FAILURE;

    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"---->s_nsStartInterruptUsbRead()\n");

    if(pDevice->intBuf.bInUse == TRUE){
        return (STATUS_FAILURE);
    }
    pDevice->intBuf.bInUse = TRUE;
//    pDevice->bEventAvailable = FALSE;
=======

int PIPEnsInterruptRead(struct vnt_private *pDevice)
{
	int ntStatus = STATUS_FAILURE;

    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"---->s_nsStartInterruptUsbRead()\n");

    if(pDevice->intBuf.bInUse == true){
        return (STATUS_FAILURE);
    }
    pDevice->intBuf.bInUse = true;
//    pDevice->bEventAvailable = false;
>>>>>>> refs/remotes/origin/master
    pDevice->ulIntInPosted++;

    //
    // Now that we have created the urb, we will send a
    // request to the USB device object.
    //
    pDevice->pInterruptURB->interval = pDevice->int_interval;

usb_fill_bulk_urb(pDevice->pInterruptURB,
		pDevice->usb,
		usb_rcvbulkpipe(pDevice->usb, 1),
		(void *) pDevice->intBuf.pDataBuf,
		MAX_INTERRUPT_SIZE,
		s_nsInterruptUsbIoCompleteRead,
		pDevice);

	ntStatus = usb_submit_urb(pDevice->pInterruptURB, GFP_ATOMIC);
	if (ntStatus != 0) {
	    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Submit int URB failed %d\n", ntStatus);
    }

    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"<----s_nsStartInterruptUsbRead Return(%x)\n",ntStatus);
    return ntStatus;
}

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
/*
 * Description:
 *      Complete function of usb interrupt in irp.
 *
 * Parameters:
 *  In:
 *      pDevice     - Pointer to the adapter
 *
 *  Out:
 *      none
 *
 * Return Value: STATUS_INSUFFICIENT_RESOURCES or result of IoCallDriver
 *
 */
<<<<<<< HEAD
static
void
s_nsInterruptUsbIoCompleteRead(
     struct urb *urb
    )

{
    PSDevice        pDevice;
    int ntStatus;
=======

static void s_nsInterruptUsbIoCompleteRead(struct urb *urb)
{
	struct vnt_private *pDevice = (struct vnt_private *)urb->context;
	int ntStatus;
>>>>>>> refs/remotes/origin/master

    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"---->s_nsInterruptUsbIoCompleteRead\n");
    //
    // The context given to IoSetCompletionRoutine is the receive buffer object
    //
<<<<<<< HEAD
    pDevice = (PSDevice)urb->context;
=======
>>>>>>> refs/remotes/origin/master

    //
    // We have a number of cases:
    //      1) The USB read timed out and we received no data.
    //      2) The USB read timed out and we received some data.
    //      3) The USB read was successful and fully filled our irp buffer.
    //      4) The irp was cancelled.
    //      5) Some other failure from the USB device object.
    //
    ntStatus = urb->status;

    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"s_nsInterruptUsbIoCompleteRead Status %d\n", ntStatus);

    // if we were not successful, we need to free the int buffer for future use right here
    // otherwise interrupt data handler will free int buffer after it handle it.
    if (( ntStatus != STATUS_SUCCESS )) {
        pDevice->ulBulkInError++;
<<<<<<< HEAD
        pDevice->intBuf.bInUse = FALSE;
=======
        pDevice->intBuf.bInUse = false;
>>>>>>> refs/remotes/origin/master

//        if (ntStatus == USBD_STATUS_CRC) {
//            pDevice->ulIntInContCRCError++;
//        }

//        if (ntStatus == STATUS_NOT_CONNECTED )
//        {
<<<<<<< HEAD
            pDevice->fKillEventPollingThread = TRUE;
=======
            pDevice->fKillEventPollingThread = true;
>>>>>>> refs/remotes/origin/master
//        }
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"IntUSBIoCompleteControl STATUS = %d\n", ntStatus );
    } else {
	    pDevice->ulIntInBytesRead += (unsigned long) urb->actual_length;
	    pDevice->ulIntInContCRCError = 0;
<<<<<<< HEAD
	    pDevice->bEventAvailable = TRUE;
	    INTnsProcessData(pDevice);
    }

    STAvUpdateUSBCounter(&pDevice->scStatistic.USB_InterruptStat, ntStatus);


    if (pDevice->fKillEventPollingThread != TRUE) {
=======
	    pDevice->bEventAvailable = true;
	    INTnsProcessData(pDevice);
    }

    if (pDevice->fKillEventPollingThread != true) {
>>>>>>> refs/remotes/origin/master
       usb_fill_bulk_urb(pDevice->pInterruptURB,
		      pDevice->usb,
		      usb_rcvbulkpipe(pDevice->usb, 1),
		     (void *) pDevice->intBuf.pDataBuf,
		     MAX_INTERRUPT_SIZE,
		     s_nsInterruptUsbIoCompleteRead,
		     pDevice);

	ntStatus = usb_submit_urb(pDevice->pInterruptURB, GFP_ATOMIC);
	if (ntStatus != 0) {
	    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Submit int URB failed %d\n", ntStatus);
           }
    }
    //
    // We return STATUS_MORE_PROCESSING_REQUIRED so that the completion
    // routine (IofCompleteRequest) will stop working on the irp.
    //
    return ;
}

/*
 * Description:
 *      Allocates an usb BulkIn  irp and calls USBD.
 *
 * Parameters:
 *  In:
 *      pDevice     - Pointer to the adapter
 *  Out:
 *      none
 *
 * Return Value: STATUS_INSUFFICIENT_RESOURCES or result of IoCallDriver
 *
 */
<<<<<<< HEAD
int PIPEnsBulkInUsbRead(PSDevice pDevice, PRCB pRCB)
{
	int ntStatus = 0;
    struct urb          *pUrb;

=======

int PIPEnsBulkInUsbRead(struct vnt_private *pDevice, struct vnt_rcb *pRCB)
{
	int ntStatus = 0;
	struct urb *pUrb;
>>>>>>> refs/remotes/origin/master

    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"---->s_nsStartBulkInUsbRead\n");

    if (pDevice->Flags & fMP_DISCONNECTED)
        return STATUS_FAILURE;

    pDevice->ulBulkInPosted++;

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
	pUrb = pRCB->pUrb;
    //
    // Now that we have created the urb, we will send a
    // request to the USB device object.
    //
    if (pRCB->skb == NULL) {
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"pRCB->skb is null \n");
        return ntStatus;
    }

	usb_fill_bulk_urb(pUrb,
		pDevice->usb,
		usb_rcvbulkpipe(pDevice->usb, 2),
		(void *) (pRCB->skb->data),
		MAX_TOTAL_SIZE_WITH_ALL_HEADERS,
		s_nsBulkInUsbIoCompleteRead,
		pRCB);

	ntStatus = usb_submit_urb(pUrb, GFP_ATOMIC);
	if (ntStatus != 0) {
		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Submit Rx URB failed %d\n", ntStatus);
		return STATUS_FAILURE ;
	}
    pRCB->Ref = 1;
<<<<<<< HEAD
    pRCB->bBoolInUse= TRUE;
=======
    pRCB->bBoolInUse= true;
>>>>>>> refs/remotes/origin/master

    return ntStatus;
}

<<<<<<< HEAD



=======
>>>>>>> refs/remotes/origin/master
/*
 * Description:
 *      Complete function of usb BulkIn irp.
 *
 * Parameters:
 *  In:
 *      pDevice     - Pointer to the adapter
 *
 *  Out:
 *      none
 *
 * Return Value: STATUS_INSUFFICIENT_RESOURCES or result of IoCallDriver
 *
 */
<<<<<<< HEAD
static
void
s_nsBulkInUsbIoCompleteRead(
     struct urb *urb
    )

{
    PRCB    pRCB = (PRCB)urb->context;
    PSDevice pDevice = (PSDevice)pRCB->pDevice;
    unsigned long   bytesRead;
    BOOL    bIndicateReceive = FALSE;
    BOOL    bReAllocSkb = FALSE;
    int status;
=======

static void s_nsBulkInUsbIoCompleteRead(struct urb *urb)
{
	struct vnt_rcb *pRCB = (struct vnt_rcb *)urb->context;
	struct vnt_private *pDevice = pRCB->pDevice;
	unsigned long   bytesRead;
	int bIndicateReceive = false;
	int bReAllocSkb = false;
	int status;
>>>>>>> refs/remotes/origin/master

    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"---->s_nsBulkInUsbIoCompleteRead\n");
    status = urb->status;
    bytesRead = urb->actual_length;

    if (status) {
        pDevice->ulBulkInError++;
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"BULK In failed %d\n", status);
<<<<<<< HEAD

           pDevice->scStatistic.RxFcsErrCnt ++;
=======
>>>>>>> refs/remotes/origin/master
//todo...xxxxxx
//        if (status == USBD_STATUS_CRC) {
//            pDevice->ulBulkInContCRCError++;
//        }
//        if (status == STATUS_DEVICE_NOT_CONNECTED )
//        {
//            MP_SET_FLAG(pDevice, fMP_DISCONNECTED);
//        }
    } else {
<<<<<<< HEAD
        bIndicateReceive = TRUE;
        pDevice->ulBulkInContCRCError = 0;
        pDevice->ulBulkInBytesRead += bytesRead;

           pDevice->scStatistic.RxOkCnt ++;
    }


    STAvUpdateUSBCounter(&pDevice->scStatistic.USB_BulkInStat, status);

    if (bIndicateReceive) {
        spin_lock(&pDevice->lock);
        if (RXbBulkInProcessData(pDevice, pRCB, bytesRead) == TRUE)
            bReAllocSkb = TRUE;
=======
	if (bytesRead)
		bIndicateReceive = true;
        pDevice->ulBulkInContCRCError = 0;
        pDevice->ulBulkInBytesRead += bytesRead;
    }

    if (bIndicateReceive) {
        spin_lock(&pDevice->lock);
        if (RXbBulkInProcessData(pDevice, pRCB, bytesRead) == true)
            bReAllocSkb = true;
>>>>>>> refs/remotes/origin/master
        spin_unlock(&pDevice->lock);
    }
    pRCB->Ref--;
    if (pRCB->Ref == 0)
    {
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"RxvFreeNormal %d \n",pDevice->NumRecvFreeList);
        spin_lock(&pDevice->lock);
        RXvFreeRCB(pRCB, bReAllocSkb);
        spin_unlock(&pDevice->lock);
    }

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
    return;
}

/*
 * Description:
 *      Allocates an usb BulkOut  irp and calls USBD.
 *
 * Parameters:
 *  In:
 *      pDevice     - Pointer to the adapter
 *  Out:
 *      none
 *
 * Return Value: STATUS_INSUFFICIENT_RESOURCES or result of IoCallDriver
 *
 */
<<<<<<< HEAD
int
PIPEnsSendBulkOut(
      PSDevice pDevice,
      PUSB_SEND_CONTEXT pContext
    )
{
    int status;
    struct urb          *pUrb;



    pDevice->bPWBitOn = FALSE;
=======

int PIPEnsSendBulkOut(struct vnt_private *pDevice,
				struct vnt_usb_send_context *pContext)
{
	int status;
	struct urb          *pUrb;

    pDevice->bPWBitOn = false;
>>>>>>> refs/remotes/origin/master

/*
    if (pDevice->pPendingBulkOutContext != NULL) {
        pDevice->NumContextsQueued++;
        EnqueueContext(pDevice->FirstTxContextQueue, pDevice->LastTxContextQueue, pContext);
        status = STATUS_PENDING;
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Send pending!\n");
        return status;
    }
*/

    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"s_nsSendBulkOut\n");

    if (MP_IS_READY(pDevice) && (pDevice->Flags & fMP_POST_WRITES)) {

        pUrb = pContext->pUrb;
        pDevice->ulBulkOutPosted++;
//        pDevice->pPendingBulkOutContext = pContext;
        usb_fill_bulk_urb(
        	    pUrb,
        		pDevice->usb,
		    usb_sndbulkpipe(pDevice->usb, 3),
		    (void *) &(pContext->Data[0]),
        		pContext->uBufLen,
        		s_nsBulkOutIoCompleteWrite,
        		pContext);

    	status = usb_submit_urb(pUrb, GFP_ATOMIC);
    	if (status != 0)
    	{
    		DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Submit Tx URB failed %d\n", status);
<<<<<<< HEAD
=======
		pContext->bBoolInUse = false;
>>>>>>> refs/remotes/origin/master
    		return STATUS_FAILURE;
    	}
        return STATUS_PENDING;
    }
    else {
<<<<<<< HEAD
        pContext->bBoolInUse = FALSE;
=======
        pContext->bBoolInUse = false;
>>>>>>> refs/remotes/origin/master
        return STATUS_RESOURCES;
    }
}

/*
 * Description: s_nsBulkOutIoCompleteWrite
 *     1a) Indicate to the protocol the status of the write.
 *     1b) Return ownership of the packet to the protocol.
 *
 *     2)  If any more packets are queue for sending, send another packet
 *         to USBD.
 *         If the attempt to send the packet to the driver fails,
 *         return ownership of the packet to the protocol and
 *         try another packet (until one succeeds).
 *
 * Parameters:
 *  In:
 *      pdoUsbDevObj  - pointer to the USB device object which
 *                      completed the irp
 *      pIrp          - the irp which was completed by the
 *                      device object
 *      pContext      - the context given to IoSetCompletionRoutine
 *                      before calling IoCallDriver on the irp
 *                      The pContext is a pointer to the USB device object.
 *  Out:
 *      none
 *
 * Return Value: STATUS_MORE_PROCESSING_REQUIRED - allows the completion routine
 *               (IofCompleteRequest) to stop working on the irp.
 *
 */
<<<<<<< HEAD
static
void
s_nsBulkOutIoCompleteWrite(
     struct urb *urb
    )
{
    PSDevice            pDevice;
    int status;
    CONTEXT_TYPE        ContextType;
    unsigned long               ulBufLen;
    PUSB_SEND_CONTEXT   pContext;

=======

static void s_nsBulkOutIoCompleteWrite(struct urb *urb)
{
	struct vnt_private *pDevice;
	int status;
	CONTEXT_TYPE ContextType;
	unsigned long ulBufLen;
	struct vnt_usb_send_context *pContext;
>>>>>>> refs/remotes/origin/master

    DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"---->s_nsBulkOutIoCompleteWrite\n");
    //
    // The context given to IoSetCompletionRoutine is an USB_CONTEXT struct
    //
<<<<<<< HEAD
    pContext = (PUSB_SEND_CONTEXT) urb->context;
    ASSERT( NULL != pContext );
=======
	pContext = (struct vnt_usb_send_context *)urb->context;
>>>>>>> refs/remotes/origin/master

    pDevice = pContext->pDevice;
    ContextType = pContext->Type;
    ulBufLen = pContext->uBufLen;

    if (!netif_device_present(pDevice->dev))
	    return;

   //
    // Perform various IRP, URB, and buffer 'sanity checks'
    //

    status = urb->status;
<<<<<<< HEAD
    //we should have failed, succeeded, or cancelled, but NOT be pending
    STAvUpdateUSBCounter(&pDevice->scStatistic.USB_BulkOutStat, status);
=======
>>>>>>> refs/remotes/origin/master

    if(status == STATUS_SUCCESS) {
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Write %d bytes\n",(int)ulBufLen);
        pDevice->ulBulkOutBytesWrite += ulBufLen;
        pDevice->ulBulkOutContCRCError = 0;
<<<<<<< HEAD
	pDevice->nTxDataTimeCout = 0;

=======
>>>>>>> refs/remotes/origin/master
    } else {
        DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"BULK Out failed %d\n", status);
        pDevice->ulBulkOutError++;
    }

//    pDevice->ulCheckForHangCount = 0;
//    pDevice->pPendingBulkOutContext = NULL;

    if ( CONTEXT_DATA_PACKET == ContextType ) {
        // Indicate to the protocol the status of the sent packet and return
        // ownership of the packet.
	    if (pContext->pPacket != NULL) {
	        dev_kfree_skb_irq(pContext->pPacket);
	        pContext->pPacket = NULL;
            DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"tx  %d bytes\n",(int)ulBufLen);
	    }

        pDevice->dev->trans_start = jiffies;

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
        if (status == STATUS_SUCCESS) {
            pDevice->packetsSent++;
        }
        else {
            DBG_PRT(MSG_LEVEL_DEBUG, KERN_INFO"Send USB error! [%08xh]\n", status);
            pDevice->packetsSentDropped++;
        }

    }
<<<<<<< HEAD
    if (pDevice->bLinkPass == TRUE) {
        if (netif_queue_stopped(pDevice->dev))
            netif_wake_queue(pDevice->dev);
    }
    pContext->bBoolInUse = FALSE;
=======
    if (pDevice->bLinkPass == true) {
        if (netif_queue_stopped(pDevice->dev))
            netif_wake_queue(pDevice->dev);
    }
    pContext->bBoolInUse = false;
>>>>>>> refs/remotes/origin/master

    return;
}
