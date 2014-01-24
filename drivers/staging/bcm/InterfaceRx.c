#include "headers.h"

<<<<<<< HEAD
static int SearchVcid(PMINI_ADAPTER Adapter,unsigned short usVcid)
{
	int iIndex=0;

	for(iIndex=(NO_OF_QUEUES-1);iIndex>=0;iIndex--)
		if(Adapter->PackInfo[iIndex].usVCID_Value == usVcid)
=======
static int SearchVcid(struct bcm_mini_adapter *Adapter, unsigned short usVcid)
{
	int iIndex = 0;

	for (iIndex = (NO_OF_QUEUES-1); iIndex >= 0; iIndex--)
		if (Adapter->PackInfo[iIndex].usVCID_Value == usVcid)
>>>>>>> refs/remotes/origin/master
			return iIndex;
	return NO_OF_QUEUES+1;

}


<<<<<<< HEAD
static PUSB_RCB
GetBulkInRcb(PS_INTERFACE_ADAPTER psIntfAdapter)
{
	PUSB_RCB pRcb = NULL;
	UINT index = 0;

	if((atomic_read(&psIntfAdapter->uNumRcbUsed) < MAXIMUM_USB_RCB) &&
		(psIntfAdapter->psAdapter->StopAllXaction == FALSE))
	{
		index = atomic_read(&psIntfAdapter->uCurrRcb);
		pRcb = &psIntfAdapter->asUsbRcb[index];
		pRcb->bUsed = TRUE;
		pRcb->psIntfAdapter= psIntfAdapter;
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_RX, RX_DPC, DBG_LVL_ALL, "Got Rx desc %d used %d",
			index, atomic_read(&psIntfAdapter->uNumRcbUsed));
=======
static struct bcm_usb_rcb *
GetBulkInRcb(struct bcm_interface_adapter *psIntfAdapter)
{
	struct bcm_usb_rcb *pRcb = NULL;
	UINT index = 0;

	if ((atomic_read(&psIntfAdapter->uNumRcbUsed) < MAXIMUM_USB_RCB) &&
	    (psIntfAdapter->psAdapter->StopAllXaction == false)) {
		index = atomic_read(&psIntfAdapter->uCurrRcb);
		pRcb = &psIntfAdapter->asUsbRcb[index];
		pRcb->bUsed = TRUE;
		pRcb->psIntfAdapter = psIntfAdapter;
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_RX, RX_DPC, DBG_LVL_ALL, "Got Rx desc %d used %d",
				index, atomic_read(&psIntfAdapter->uNumRcbUsed));
>>>>>>> refs/remotes/origin/master
		index = (index + 1) % MAXIMUM_USB_RCB;
		atomic_set(&psIntfAdapter->uCurrRcb, index);
		atomic_inc(&psIntfAdapter->uNumRcbUsed);
	}
	return pRcb;
}

/*this is receive call back - when pkt available for receive (BULK IN- end point)*/
static void read_bulk_callback(struct urb *urb)
{
	struct sk_buff *skb = NULL;
<<<<<<< HEAD
	BOOLEAN bHeaderSupressionEnabled = FALSE;
	int QueueIndex = NO_OF_QUEUES + 1;
	UINT uiIndex=0;
	int process_done = 1;
	//int idleflag = 0 ;
	PUSB_RCB pRcb = (PUSB_RCB)urb->context;
	PS_INTERFACE_ADAPTER psIntfAdapter = pRcb->psIntfAdapter;
	PMINI_ADAPTER Adapter = psIntfAdapter->psAdapter;
	PLEADER pLeader = urb->transfer_buffer;
=======
	bool bHeaderSupressionEnabled = false;
	int QueueIndex = NO_OF_QUEUES + 1;
	UINT uiIndex = 0;
	int process_done = 1;
	struct bcm_usb_rcb *pRcb = (struct bcm_usb_rcb *)urb->context;
	struct bcm_interface_adapter *psIntfAdapter = pRcb->psIntfAdapter;
	struct bcm_mini_adapter *Adapter = psIntfAdapter->psAdapter;
	struct bcm_leader *pLeader = urb->transfer_buffer;
>>>>>>> refs/remotes/origin/master

	if (unlikely(netif_msg_rx_status(Adapter)))
		pr_info(PFX "%s: rx urb status %d length %d\n",
			Adapter->dev->name, urb->status, urb->actual_length);

<<<<<<< HEAD
	if((Adapter->device_removed == TRUE)  ||
		(TRUE == Adapter->bEndPointHalted) ||
		(0 == urb->actual_length)
		)
	{
	 	pRcb->bUsed = FALSE;
 		atomic_dec(&psIntfAdapter->uNumRcbUsed);
		return;
	}

	if(urb->status != STATUS_SUCCESS)
	{
		if(urb->status == -EPIPE)
		{
			Adapter->bEndPointHalted = TRUE ;
			wake_up(&Adapter->tx_packet_wait_queue);
		}
		else
		{
			BCM_DEBUG_PRINT(Adapter,DBG_TYPE_RX, RX_DPC, DBG_LVL_ALL,"Rx URB has got cancelled. status :%d", urb->status);
		}
		pRcb->bUsed = FALSE;
 		atomic_dec(&psIntfAdapter->uNumRcbUsed);
		urb->status = STATUS_SUCCESS ;
		return ;
	}

	if(Adapter->bDoSuspend && (Adapter->bPreparingForLowPowerMode))
	{
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_RX, RX_DPC, DBG_LVL_ALL,"device is going in low power mode while PMU option selected..hence rx packet should not be process");
		return ;
	}

	BCM_DEBUG_PRINT(Adapter,DBG_TYPE_RX, RX_DPC, DBG_LVL_ALL, "Read back done len %d\n", pLeader->PLength);
	if(!pLeader->PLength)
	{
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_RX, RX_DPC, DBG_LVL_ALL, "Leader Length 0");
		atomic_dec(&psIntfAdapter->uNumRcbUsed);
		return;
	}
	BCM_DEBUG_PRINT(Adapter,DBG_TYPE_RX, RX_DPC, DBG_LVL_ALL, "Leader Status:0x%hX, Length:0x%hX, VCID:0x%hX", pLeader->Status,pLeader->PLength,pLeader->Vcid);
	if(MAX_CNTL_PKT_SIZE < pLeader->PLength)
	{
=======
	if ((Adapter->device_removed == TRUE) ||
	    (TRUE == Adapter->bEndPointHalted) ||
	    (0 == urb->actual_length)) {
		pRcb->bUsed = false;
		atomic_dec(&psIntfAdapter->uNumRcbUsed);
		return;
	}

	if (urb->status != STATUS_SUCCESS) {
		if (urb->status == -EPIPE) {
			Adapter->bEndPointHalted = TRUE;
			wake_up(&Adapter->tx_packet_wait_queue);
		} else {
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_RX, RX_DPC, DBG_LVL_ALL, "Rx URB has got cancelled. status :%d", urb->status);
		}
		pRcb->bUsed = false;
		atomic_dec(&psIntfAdapter->uNumRcbUsed);
		urb->status = STATUS_SUCCESS;
		return;
	}

	if (Adapter->bDoSuspend && (Adapter->bPreparingForLowPowerMode)) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_RX, RX_DPC, DBG_LVL_ALL, "device is going in low power mode while PMU option selected..hence rx packet should not be process");
		return;
	}

	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_RX, RX_DPC, DBG_LVL_ALL, "Read back done len %d\n", pLeader->PLength);
	if (!pLeader->PLength) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_RX, RX_DPC, DBG_LVL_ALL, "Leader Length 0");
		atomic_dec(&psIntfAdapter->uNumRcbUsed);
		return;
	}
	BCM_DEBUG_PRINT(Adapter, DBG_TYPE_RX, RX_DPC, DBG_LVL_ALL, "Leader Status:0x%hX, Length:0x%hX, VCID:0x%hX", pLeader->Status, pLeader->PLength, pLeader->Vcid);
	if (MAX_CNTL_PKT_SIZE < pLeader->PLength) {
>>>>>>> refs/remotes/origin/master
		if (netif_msg_rx_err(Adapter))
			pr_info(PFX "%s: corrupted leader length...%d\n",
				Adapter->dev->name, pLeader->PLength);
		++Adapter->dev->stats.rx_dropped;
		atomic_dec(&psIntfAdapter->uNumRcbUsed);
		return;
	}

<<<<<<< HEAD
	QueueIndex = SearchVcid( Adapter,pLeader->Vcid);
	if(QueueIndex < NO_OF_QUEUES)
	{
=======
	QueueIndex = SearchVcid(Adapter, pLeader->Vcid);
	if (QueueIndex < NO_OF_QUEUES) {
>>>>>>> refs/remotes/origin/master
		bHeaderSupressionEnabled =
			Adapter->PackInfo[QueueIndex].bHeaderSuppressionEnabled;
		bHeaderSupressionEnabled =
			bHeaderSupressionEnabled & Adapter->bPHSEnabled;
	}

<<<<<<< HEAD
	skb = dev_alloc_skb (pLeader->PLength + SKB_RESERVE_PHS_BYTES + SKB_RESERVE_ETHERNET_HEADER);//2   //2 for allignment
	if(!skb)
	{
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_PRINTK, 0, 0, "NO SKBUFF!!! Dropping the Packet");
		atomic_dec(&psIntfAdapter->uNumRcbUsed);
		return;
	}
    /* If it is a control Packet, then call handle_bcm_packet ()*/
	if((ntohs(pLeader->Vcid) == VCID_CONTROL_PACKET) ||
	    (!(pLeader->Status >= 0x20  &&  pLeader->Status <= 0x3F)))
	{
	    BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_RX, RX_CTRL, DBG_LVL_ALL, "Received control pkt...");
		*(PUSHORT)skb->data = pLeader->Status;
       	memcpy(skb->data+sizeof(USHORT), urb->transfer_buffer +
			(sizeof(LEADER)), pLeader->PLength);
		skb->len = pLeader->PLength + sizeof(USHORT);

		spin_lock(&Adapter->control_queue_lock);
		ENQUEUEPACKET(Adapter->RxControlHead,Adapter->RxControlTail,skb);
=======
	skb = dev_alloc_skb(pLeader->PLength + SKB_RESERVE_PHS_BYTES + SKB_RESERVE_ETHERNET_HEADER);
	if (!skb) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_PRINTK, 0, 0, "NO SKBUFF!!! Dropping the Packet");
		atomic_dec(&psIntfAdapter->uNumRcbUsed);
		return;
	}
	/* If it is a control Packet, then call handle_bcm_packet ()*/
	if ((ntohs(pLeader->Vcid) == VCID_CONTROL_PACKET) ||
	    (!(pLeader->Status >= 0x20  &&  pLeader->Status <= 0x3F))) {
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_RX, RX_CTRL, DBG_LVL_ALL, "Received control pkt...");
		*(PUSHORT)skb->data = pLeader->Status;
		memcpy(skb->data+sizeof(USHORT), urb->transfer_buffer +
		       (sizeof(struct bcm_leader)), pLeader->PLength);
		skb->len = pLeader->PLength + sizeof(USHORT);

		spin_lock(&Adapter->control_queue_lock);
		ENQUEUEPACKET(Adapter->RxControlHead, Adapter->RxControlTail, skb);
>>>>>>> refs/remotes/origin/master
		spin_unlock(&Adapter->control_queue_lock);

		atomic_inc(&Adapter->cntrlpktCnt);
		wake_up(&Adapter->process_rx_cntrlpkt);
<<<<<<< HEAD
	}
	else
	{
		/*
		  * Data Packet, Format a proper Ethernet Header
        	  * and give it to the stack
		  */
        BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_RX, RX_DATA, DBG_LVL_ALL, "Received Data pkt...");
		skb_reserve(skb, 2 + SKB_RESERVE_PHS_BYTES);
		memcpy(skb->data+ETH_HLEN, (PUCHAR)urb->transfer_buffer + sizeof(LEADER), pLeader->PLength);
		skb->dev = Adapter->dev;

		/* currently skb->len has extra ETH_HLEN bytes in the beginning */
		skb_put (skb, pLeader->PLength + ETH_HLEN);
		Adapter->PackInfo[QueueIndex].uiTotalRxBytes+=pLeader->PLength;
		Adapter->PackInfo[QueueIndex].uiThisPeriodRxBytes+= pLeader->PLength;
        BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_RX, RX_DATA, DBG_LVL_ALL, "Received Data pkt of len :0x%X", pLeader->PLength);

		if(netif_running(Adapter->dev))
		{
			/* Moving ahead by ETH_HLEN to the data ptr as received from FW */
			skb_pull(skb, ETH_HLEN);
<<<<<<< HEAD
			PHSRecieve(Adapter, pLeader->Vcid, skb, &skb->len,
=======
			PHSReceive(Adapter, pLeader->Vcid, skb, &skb->len,
>>>>>>> refs/remotes/origin/cm-10.0
					NULL,bHeaderSupressionEnabled);

			if(!Adapter->PackInfo[QueueIndex].bEthCSSupport)
			{
=======
	} else {
		/*
		 * Data Packet, Format a proper Ethernet Header
		 * and give it to the stack
		 */
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_RX, RX_DATA, DBG_LVL_ALL, "Received Data pkt...");
		skb_reserve(skb, 2 + SKB_RESERVE_PHS_BYTES);
		memcpy(skb->data+ETH_HLEN, (PUCHAR)urb->transfer_buffer + sizeof(struct bcm_leader), pLeader->PLength);
		skb->dev = Adapter->dev;

		/* currently skb->len has extra ETH_HLEN bytes in the beginning */
		skb_put(skb, pLeader->PLength + ETH_HLEN);
		Adapter->PackInfo[QueueIndex].uiTotalRxBytes += pLeader->PLength;
		Adapter->PackInfo[QueueIndex].uiThisPeriodRxBytes += pLeader->PLength;
		BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_RX, RX_DATA, DBG_LVL_ALL, "Received Data pkt of len :0x%X", pLeader->PLength);

		if (netif_running(Adapter->dev)) {
			/* Moving ahead by ETH_HLEN to the data ptr as received from FW */
			skb_pull(skb, ETH_HLEN);
			PHSReceive(Adapter, pLeader->Vcid, skb, &skb->len,
				   NULL, bHeaderSupressionEnabled);

			if (!Adapter->PackInfo[QueueIndex].bEthCSSupport) {
>>>>>>> refs/remotes/origin/master
				skb_push(skb, ETH_HLEN);

				memcpy(skb->data, skb->dev->dev_addr, 6);
				memcpy(skb->data+6, skb->dev->dev_addr, 6);
				(*(skb->data+11))++;
				*(skb->data+12) = 0x08;
				*(skb->data+13) = 0x00;
<<<<<<< HEAD
				pLeader->PLength+=ETH_HLEN;
=======
				pLeader->PLength += ETH_HLEN;
>>>>>>> refs/remotes/origin/master
			}

			skb->protocol = eth_type_trans(skb, Adapter->dev);
			process_done = netif_rx(skb);
<<<<<<< HEAD
		}
		else
		{
		    BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_RX, RX_DATA, DBG_LVL_ALL, "i/f not up hance freeing SKB...");
=======
		} else {
			BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_RX, RX_DATA, DBG_LVL_ALL, "i/f not up hance freeing SKB...");
>>>>>>> refs/remotes/origin/master
			dev_kfree_skb(skb);
		}

		++Adapter->dev->stats.rx_packets;
		Adapter->dev->stats.rx_bytes += pLeader->PLength;

<<<<<<< HEAD
		for(uiIndex = 0 ; uiIndex < MIBS_MAX_HIST_ENTRIES ; uiIndex++)
		{
			if((pLeader->PLength <= MIBS_PKTSIZEHIST_RANGE*(uiIndex+1))
				&& (pLeader->PLength > MIBS_PKTSIZEHIST_RANGE*(uiIndex)))
				Adapter->aRxPktSizeHist[uiIndex]++;
		}
	}
 	Adapter->PrevNumRecvDescs++;
	pRcb->bUsed = FALSE;
	atomic_dec(&psIntfAdapter->uNumRcbUsed);
}

static int ReceiveRcb(PS_INTERFACE_ADAPTER psIntfAdapter, PUSB_RCB pRcb)
=======
		for (uiIndex = 0; uiIndex < MIBS_MAX_HIST_ENTRIES; uiIndex++) {
			if ((pLeader->PLength <= MIBS_PKTSIZEHIST_RANGE*(uiIndex+1)) &&
			    (pLeader->PLength > MIBS_PKTSIZEHIST_RANGE*(uiIndex)))
				Adapter->aRxPktSizeHist[uiIndex]++;
		}
	}
	Adapter->PrevNumRecvDescs++;
	pRcb->bUsed = false;
	atomic_dec(&psIntfAdapter->uNumRcbUsed);
}

static int ReceiveRcb(struct bcm_interface_adapter *psIntfAdapter, struct bcm_usb_rcb *pRcb)
>>>>>>> refs/remotes/origin/master
{
	struct urb *urb = pRcb->urb;
	int retval = 0;

<<<<<<< HEAD
	usb_fill_bulk_urb(urb, psIntfAdapter->udev, usb_rcvbulkpipe(
			psIntfAdapter->udev, psIntfAdapter->sBulkIn.bulk_in_endpointAddr),
		  	urb->transfer_buffer, BCM_USB_MAX_READ_LENGTH, read_bulk_callback,
			pRcb);
	if(FALSE == psIntfAdapter->psAdapter->device_removed &&
	   FALSE == psIntfAdapter->psAdapter->bEndPointHalted &&
	   FALSE == psIntfAdapter->bSuspended &&
	   FALSE == psIntfAdapter->bPreparingForBusSuspend)
	{
		retval = usb_submit_urb(urb, GFP_ATOMIC);
		if (retval)
		{
			BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_RX, RX_DPC, DBG_LVL_ALL, "failed submitting read urb, error %d", retval);
			//if this return value is because of pipe halt. need to clear this.
			if(retval == -EPIPE)
			{
				psIntfAdapter->psAdapter->bEndPointHalted = TRUE ;
=======
	usb_fill_bulk_urb(urb, psIntfAdapter->udev, usb_rcvbulkpipe(psIntfAdapter->udev, psIntfAdapter->sBulkIn.bulk_in_endpointAddr),
			  urb->transfer_buffer, BCM_USB_MAX_READ_LENGTH, read_bulk_callback, pRcb);
	if (false == psIntfAdapter->psAdapter->device_removed &&
	    false == psIntfAdapter->psAdapter->bEndPointHalted &&
	    false == psIntfAdapter->bSuspended &&
	    false == psIntfAdapter->bPreparingForBusSuspend) {
		retval = usb_submit_urb(urb, GFP_ATOMIC);
		if (retval) {
			BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_RX, RX_DPC, DBG_LVL_ALL, "failed submitting read urb, error %d", retval);
			/* if this return value is because of pipe halt. need to clear this. */
			if (retval == -EPIPE) {
				psIntfAdapter->psAdapter->bEndPointHalted = TRUE;
>>>>>>> refs/remotes/origin/master
				wake_up(&psIntfAdapter->psAdapter->tx_packet_wait_queue);
			}

		}
	}
	return retval;
}

/*
Function:				InterfaceRx

<<<<<<< HEAD
<<<<<<< HEAD
Description:			This is the hardware specific Function for Recieveing
=======
Description:			This is the hardware specific Function for Receiving
>>>>>>> refs/remotes/origin/cm-10.0
						data packet/control packets from the device.

Input parameters:		IN PMINI_ADAPTER Adapter   - Miniport Adapter Context
=======
Description:			This is the hardware specific Function for Receiving
						data packet/control packets from the device.

Input parameters:		IN struct bcm_mini_adapter *Adapter   - Miniport Adapter Context
>>>>>>> refs/remotes/origin/master



Return:				TRUE  - If Rx was successful.
					Other - If an error occurred.
*/

<<<<<<< HEAD
BOOLEAN InterfaceRx (PS_INTERFACE_ADAPTER psIntfAdapter)
{
	USHORT RxDescCount = NUM_RX_DESC - atomic_read(&psIntfAdapter->uNumRcbUsed);
	PUSB_RCB pRcb = NULL;

//	RxDescCount = psIntfAdapter->psAdapter->CurrNumRecvDescs -
//				psIntfAdapter->psAdapter->PrevNumRecvDescs;
	while(RxDescCount)
	{
		pRcb = GetBulkInRcb(psIntfAdapter);
		if(pRcb == NULL)
		{
			BCM_DEBUG_PRINT(psIntfAdapter->psAdapter,DBG_TYPE_PRINTK, 0, 0, "Unable to get Rcb pointer");
			return FALSE;
		}
		//atomic_inc(&psIntfAdapter->uNumRcbUsed);
		ReceiveRcb(psIntfAdapter, pRcb);
		RxDescCount--;
    }
=======
bool InterfaceRx(struct bcm_interface_adapter *psIntfAdapter)
{
	USHORT RxDescCount = NUM_RX_DESC - atomic_read(&psIntfAdapter->uNumRcbUsed);
	struct bcm_usb_rcb *pRcb = NULL;

	while (RxDescCount) {
		pRcb = GetBulkInRcb(psIntfAdapter);
		if (pRcb == NULL) {
			BCM_DEBUG_PRINT(psIntfAdapter->psAdapter, DBG_TYPE_PRINTK, 0, 0, "Unable to get Rcb pointer");
			return false;
		}
		ReceiveRcb(psIntfAdapter, pRcb);
		RxDescCount--;
	}
>>>>>>> refs/remotes/origin/master
	return TRUE;
}

