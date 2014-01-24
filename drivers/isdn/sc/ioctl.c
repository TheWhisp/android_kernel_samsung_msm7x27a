/*
 * Copyright (C) 1996  SpellCaster Telecommunications Inc.
 *
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

#include "includes.h"
#include "hardware.h"
#include "message.h"
#include "card.h"
#include "scioc.h"

static int GetStatus(int card, boardInfo *);

/*
 * Process private IOCTL messages (typically from scctrl)
 */
int sc_ioctl(int card, scs_ioctl *data)
{
	int		status;
	RspMessage	*rcvmsg;
	char		*spid;
	char		*dn;
	char		switchtype;
	char		speed;

	rcvmsg = kmalloc(sizeof(RspMessage), GFP_KERNEL);
	if (!rcvmsg)
		return -ENOMEM;

<<<<<<< HEAD
	switch(data->command) {
	case SCIOCRESET:	/* Perform a hard reset of the adapter */
	{
		pr_debug("%s: SCIOCRESET: ioctl received\n",
			sc_adapter[card]->devicename);
=======
	switch (data->command) {
	case SCIOCRESET:	/* Perform a hard reset of the adapter */
	{
		pr_debug("%s: SCIOCRESET: ioctl received\n",
			 sc_adapter[card]->devicename);
>>>>>>> refs/remotes/origin/cm-10.0
		sc_adapter[card]->StartOnReset = 0;
		kfree(rcvmsg);
		return reset(card);
	}

	case SCIOCLOAD:
	{
		char *srec;

		srec = kmalloc(SCIOC_SRECSIZE, GFP_KERNEL);
		if (!srec) {
			kfree(rcvmsg);
			return -ENOMEM;
		}
		pr_debug("%s: SCIOLOAD: ioctl received\n",
<<<<<<< HEAD
				sc_adapter[card]->devicename);
		if(sc_adapter[card]->EngineUp) {
			pr_debug("%s: SCIOCLOAD: command failed, LoadProc while engine running.\n",
				sc_adapter[card]->devicename);
=======
			 sc_adapter[card]->devicename);
		if (sc_adapter[card]->EngineUp) {
			pr_debug("%s: SCIOCLOAD: command failed, LoadProc while engine running.\n",
				 sc_adapter[card]->devicename);
>>>>>>> refs/remotes/origin/cm-10.0
			kfree(rcvmsg);
			kfree(srec);
			return -1;
		}

		/*
		 * Get the SRec from user space
		 */
		if (copy_from_user(srec, data->dataptr, SCIOC_SRECSIZE)) {
			kfree(rcvmsg);
			kfree(srec);
			return -EFAULT;
		}

		status = send_and_receive(card, CMPID, cmReqType2, cmReqClass0, cmReqLoadProc,
<<<<<<< HEAD
				0, SCIOC_SRECSIZE, srec, rcvmsg, SAR_TIMEOUT);
		kfree(rcvmsg);
		kfree(srec);

		if(status) {
			pr_debug("%s: SCIOCLOAD: command failed, status = %d\n", 
				sc_adapter[card]->devicename, status);
=======
					  0, SCIOC_SRECSIZE, srec, rcvmsg, SAR_TIMEOUT);
		kfree(rcvmsg);
		kfree(srec);

		if (status) {
			pr_debug("%s: SCIOCLOAD: command failed, status = %d\n",
				 sc_adapter[card]->devicename, status);
>>>>>>> refs/remotes/origin/cm-10.0
			return -1;
		}
		else {
			pr_debug("%s: SCIOCLOAD: command successful\n",
<<<<<<< HEAD
					sc_adapter[card]->devicename);
=======
				 sc_adapter[card]->devicename);
>>>>>>> refs/remotes/origin/cm-10.0
			return 0;
		}
	}

	case SCIOCSTART:
	{
		kfree(rcvmsg);
		pr_debug("%s: SCIOSTART: ioctl received\n",
<<<<<<< HEAD
				sc_adapter[card]->devicename);
		if(sc_adapter[card]->EngineUp) {
			pr_debug("%s: SCIOCSTART: command failed, engine already running.\n",
				sc_adapter[card]->devicename);
=======
			 sc_adapter[card]->devicename);
		if (sc_adapter[card]->EngineUp) {
			pr_debug("%s: SCIOCSTART: command failed, engine already running.\n",
				 sc_adapter[card]->devicename);
>>>>>>> refs/remotes/origin/cm-10.0
			return -1;
		}

		sc_adapter[card]->StartOnReset = 1;
		startproc(card);
		return 0;
	}

	case SCIOCSETSWITCH:
	{
		pr_debug("%s: SCIOSETSWITCH: ioctl received\n",
<<<<<<< HEAD
				sc_adapter[card]->devicename);
=======
			 sc_adapter[card]->devicename);
>>>>>>> refs/remotes/origin/cm-10.0

		/*
		 * Get the switch type from user space
		 */
		if (copy_from_user(&switchtype, data->dataptr, sizeof(char))) {
			kfree(rcvmsg);
			return -EFAULT;
		}

		pr_debug("%s: SCIOCSETSWITCH: setting switch type to %d\n",
<<<<<<< HEAD
			sc_adapter[card]->devicename,
			switchtype);
		status = send_and_receive(card, CEPID, ceReqTypeCall, ceReqClass0, ceReqCallSetSwitchType,
						0, sizeof(char),&switchtype, rcvmsg, SAR_TIMEOUT);
		if(!status && !(rcvmsg->rsp_status)) {
			pr_debug("%s: SCIOCSETSWITCH: command successful\n",
				sc_adapter[card]->devicename);
=======
			 sc_adapter[card]->devicename,
			 switchtype);
		status = send_and_receive(card, CEPID, ceReqTypeCall, ceReqClass0, ceReqCallSetSwitchType,
					  0, sizeof(char), &switchtype, rcvmsg, SAR_TIMEOUT);
		if (!status && !(rcvmsg->rsp_status)) {
			pr_debug("%s: SCIOCSETSWITCH: command successful\n",
				 sc_adapter[card]->devicename);
>>>>>>> refs/remotes/origin/cm-10.0
			kfree(rcvmsg);
			return 0;
		}
		else {
			pr_debug("%s: SCIOCSETSWITCH: command failed (status = %d)\n",
<<<<<<< HEAD
				sc_adapter[card]->devicename, status);
=======
				 sc_adapter[card]->devicename, status);
>>>>>>> refs/remotes/origin/cm-10.0
			kfree(rcvmsg);
			return status;
		}
	}
<<<<<<< HEAD
		
	case SCIOCGETSWITCH:
	{
		pr_debug("%s: SCIOGETSWITCH: ioctl received\n",
				sc_adapter[card]->devicename);
=======

	case SCIOCGETSWITCH:
	{
		pr_debug("%s: SCIOGETSWITCH: ioctl received\n",
			 sc_adapter[card]->devicename);
>>>>>>> refs/remotes/origin/cm-10.0

		/*
		 * Get the switch type from the board
		 */
<<<<<<< HEAD
		status = send_and_receive(card, CEPID, ceReqTypeCall, ceReqClass0, 
			ceReqCallGetSwitchType, 0, 0, NULL, rcvmsg, SAR_TIMEOUT);
		if (!status && !(rcvmsg->rsp_status)) {
			pr_debug("%s: SCIOCGETSWITCH: command successful\n",
					sc_adapter[card]->devicename);
		}
		else {
			pr_debug("%s: SCIOCGETSWITCH: command failed (status = %d)\n",
				sc_adapter[card]->devicename, status);
=======
		status = send_and_receive(card, CEPID, ceReqTypeCall, ceReqClass0,
					  ceReqCallGetSwitchType, 0, 0, NULL, rcvmsg, SAR_TIMEOUT);
		if (!status && !(rcvmsg->rsp_status)) {
			pr_debug("%s: SCIOCGETSWITCH: command successful\n",
				 sc_adapter[card]->devicename);
		}
		else {
			pr_debug("%s: SCIOCGETSWITCH: command failed (status = %d)\n",
				 sc_adapter[card]->devicename, status);
>>>>>>> refs/remotes/origin/cm-10.0
			kfree(rcvmsg);
			return status;
		}

		switchtype = rcvmsg->msg_data.byte_array[0];

		/*
		 * Package the switch type and send to user space
		 */
		if (copy_to_user(data->dataptr, &switchtype,
				 sizeof(char))) {
			kfree(rcvmsg);
			return -EFAULT;
		}

		kfree(rcvmsg);
		return 0;
	}

	case SCIOCGETSPID:
	{
		pr_debug("%s: SCIOGETSPID: ioctl received\n",
<<<<<<< HEAD
				sc_adapter[card]->devicename);
=======
			 sc_adapter[card]->devicename);
>>>>>>> refs/remotes/origin/cm-10.0

		spid = kzalloc(SCIOC_SPIDSIZE, GFP_KERNEL);
		if (!spid) {
			kfree(rcvmsg);
			return -ENOMEM;
		}
		/*
		 * Get the spid from the board
		 */
		status = send_and_receive(card, CEPID, ceReqTypeCall, ceReqClass0, ceReqCallGetSPID,
<<<<<<< HEAD
					data->channel, 0, NULL, rcvmsg, SAR_TIMEOUT);
		if (!status) {
			pr_debug("%s: SCIOCGETSPID: command successful\n",
					sc_adapter[card]->devicename);
		} else {
			pr_debug("%s: SCIOCGETSPID: command failed (status = %d)\n",
				sc_adapter[card]->devicename, status);
=======
					  data->channel, 0, NULL, rcvmsg, SAR_TIMEOUT);
		if (!status) {
			pr_debug("%s: SCIOCGETSPID: command successful\n",
				 sc_adapter[card]->devicename);
		} else {
			pr_debug("%s: SCIOCGETSPID: command failed (status = %d)\n",
				 sc_adapter[card]->devicename, status);
>>>>>>> refs/remotes/origin/cm-10.0
			kfree(spid);
			kfree(rcvmsg);
			return status;
		}
		strlcpy(spid, rcvmsg->msg_data.byte_array, SCIOC_SPIDSIZE);

		/*
		 * Package the switch type and send to user space
		 */
		if (copy_to_user(data->dataptr, spid, SCIOC_SPIDSIZE)) {
			kfree(spid);
			kfree(rcvmsg);
			return -EFAULT;
		}

		kfree(spid);
		kfree(rcvmsg);
		return 0;
<<<<<<< HEAD
	}	
=======
	}
>>>>>>> refs/remotes/origin/cm-10.0

	case SCIOCSETSPID:
	{
		pr_debug("%s: DCBIOSETSPID: ioctl received\n",
<<<<<<< HEAD
				sc_adapter[card]->devicename);
=======
			 sc_adapter[card]->devicename);
>>>>>>> refs/remotes/origin/cm-10.0

		/*
		 * Get the spid from user space
		 */
		spid = memdup_user(data->dataptr, SCIOC_SPIDSIZE);
		if (IS_ERR(spid)) {
			kfree(rcvmsg);
			return PTR_ERR(spid);
		}

<<<<<<< HEAD
		pr_debug("%s: SCIOCSETSPID: setting channel %d spid to %s\n", 
			sc_adapter[card]->devicename, data->channel, spid);
		status = send_and_receive(card, CEPID, ceReqTypeCall, 
			ceReqClass0, ceReqCallSetSPID, data->channel, 
			strlen(spid), spid, rcvmsg, SAR_TIMEOUT);
		if(!status && !(rcvmsg->rsp_status)) {
			pr_debug("%s: SCIOCSETSPID: command successful\n", 
				sc_adapter[card]->devicename);
=======
		pr_debug("%s: SCIOCSETSPID: setting channel %d spid to %s\n",
			 sc_adapter[card]->devicename, data->channel, spid);
		status = send_and_receive(card, CEPID, ceReqTypeCall,
					  ceReqClass0, ceReqCallSetSPID, data->channel,
					  strlen(spid), spid, rcvmsg, SAR_TIMEOUT);
		if (!status && !(rcvmsg->rsp_status)) {
			pr_debug("%s: SCIOCSETSPID: command successful\n",
				 sc_adapter[card]->devicename);
>>>>>>> refs/remotes/origin/cm-10.0
			kfree(rcvmsg);
			kfree(spid);
			return 0;
		}
		else {
			pr_debug("%s: SCIOCSETSPID: command failed (status = %d)\n",
<<<<<<< HEAD
				sc_adapter[card]->devicename, status);
=======
				 sc_adapter[card]->devicename, status);
>>>>>>> refs/remotes/origin/cm-10.0
			kfree(rcvmsg);
			kfree(spid);
			return status;
		}
	}

	case SCIOCGETDN:
	{
		pr_debug("%s: SCIOGETDN: ioctl received\n",
<<<<<<< HEAD
				sc_adapter[card]->devicename);
=======
			 sc_adapter[card]->devicename);
>>>>>>> refs/remotes/origin/cm-10.0

		/*
		 * Get the dn from the board
		 */
		status = send_and_receive(card, CEPID, ceReqTypeCall, ceReqClass0, ceReqCallGetMyNumber,
<<<<<<< HEAD
					data->channel, 0, NULL, rcvmsg, SAR_TIMEOUT);
		if (!status) {
			pr_debug("%s: SCIOCGETDN: command successful\n",
					sc_adapter[card]->devicename);
		}
		else {
			pr_debug("%s: SCIOCGETDN: command failed (status = %d)\n",
				sc_adapter[card]->devicename, status);
=======
					  data->channel, 0, NULL, rcvmsg, SAR_TIMEOUT);
		if (!status) {
			pr_debug("%s: SCIOCGETDN: command successful\n",
				 sc_adapter[card]->devicename);
		}
		else {
			pr_debug("%s: SCIOCGETDN: command failed (status = %d)\n",
				 sc_adapter[card]->devicename, status);
>>>>>>> refs/remotes/origin/cm-10.0
			kfree(rcvmsg);
			return status;
		}

		dn = kzalloc(SCIOC_DNSIZE, GFP_KERNEL);
		if (!dn) {
			kfree(rcvmsg);
			return -ENOMEM;
		}
		strlcpy(dn, rcvmsg->msg_data.byte_array, SCIOC_DNSIZE);
		kfree(rcvmsg);

		/*
		 * Package the dn and send to user space
		 */
		if (copy_to_user(data->dataptr, dn, SCIOC_DNSIZE)) {
			kfree(dn);
			return -EFAULT;
		}
		kfree(dn);
		return 0;
<<<<<<< HEAD
	}	
=======
	}
>>>>>>> refs/remotes/origin/cm-10.0

	case SCIOCSETDN:
	{
		pr_debug("%s: SCIOSETDN: ioctl received\n",
<<<<<<< HEAD
				sc_adapter[card]->devicename);
=======
			 sc_adapter[card]->devicename);
>>>>>>> refs/remotes/origin/cm-10.0

		/*
		 * Get the spid from user space
		 */
		dn = memdup_user(data->dataptr, SCIOC_DNSIZE);
		if (IS_ERR(dn)) {
			kfree(rcvmsg);
			return PTR_ERR(dn);
		}

<<<<<<< HEAD
		pr_debug("%s: SCIOCSETDN: setting channel %d dn to %s\n", 
			sc_adapter[card]->devicename, data->channel, dn);
		status = send_and_receive(card, CEPID, ceReqTypeCall, 
			ceReqClass0, ceReqCallSetMyNumber, data->channel, 
			strlen(dn),dn,rcvmsg, SAR_TIMEOUT);
		if(!status && !(rcvmsg->rsp_status)) {
			pr_debug("%s: SCIOCSETDN: command successful\n", 
				sc_adapter[card]->devicename);
=======
		pr_debug("%s: SCIOCSETDN: setting channel %d dn to %s\n",
			 sc_adapter[card]->devicename, data->channel, dn);
		status = send_and_receive(card, CEPID, ceReqTypeCall,
					  ceReqClass0, ceReqCallSetMyNumber, data->channel,
					  strlen(dn), dn, rcvmsg, SAR_TIMEOUT);
		if (!status && !(rcvmsg->rsp_status)) {
			pr_debug("%s: SCIOCSETDN: command successful\n",
				 sc_adapter[card]->devicename);
>>>>>>> refs/remotes/origin/cm-10.0
			kfree(rcvmsg);
			kfree(dn);
			return 0;
		}
		else {
			pr_debug("%s: SCIOCSETDN: command failed (status = %d)\n",
<<<<<<< HEAD
				sc_adapter[card]->devicename, status);
=======
				 sc_adapter[card]->devicename, status);
>>>>>>> refs/remotes/origin/cm-10.0
			kfree(rcvmsg);
			kfree(dn);
			return status;
		}
	}

	case SCIOCTRACE:

		pr_debug("%s: SCIOTRACE: ioctl received\n",
<<<<<<< HEAD
				sc_adapter[card]->devicename);
/*		sc_adapter[card]->trace = !sc_adapter[card]->trace;
		pr_debug("%s: SCIOCTRACE: tracing turned %s\n",
				sc_adapter[card]->devicename,
			sc_adapter[card]->trace ? "ON" : "OFF"); */
=======
			 sc_adapter[card]->devicename);
/*		sc_adapter[card]->trace = !sc_adapter[card]->trace;
		pr_debug("%s: SCIOCTRACE: tracing turned %s\n",
		sc_adapter[card]->devicename,
		sc_adapter[card]->trace ? "ON" : "OFF"); */
>>>>>>> refs/remotes/origin/cm-10.0
		break;

	case SCIOCSTAT:
	{
		boardInfo *bi;

		pr_debug("%s: SCIOSTAT: ioctl received\n",
<<<<<<< HEAD
				sc_adapter[card]->devicename);
=======
			 sc_adapter[card]->devicename);
>>>>>>> refs/remotes/origin/cm-10.0

		bi = kzalloc(sizeof(boardInfo), GFP_KERNEL);
		if (!bi) {
			kfree(rcvmsg);
			return -ENOMEM;
		}

		kfree(rcvmsg);
		GetStatus(card, bi);

		if (copy_to_user(data->dataptr, bi, sizeof(boardInfo))) {
			kfree(bi);
			return -EFAULT;
		}

		kfree(bi);
		return 0;
	}

	case SCIOCGETSPEED:
	{
		pr_debug("%s: SCIOGETSPEED: ioctl received\n",
<<<<<<< HEAD
				sc_adapter[card]->devicename);
=======
			 sc_adapter[card]->devicename);
>>>>>>> refs/remotes/origin/cm-10.0

		/*
		 * Get the speed from the board
		 */
<<<<<<< HEAD
		status = send_and_receive(card, CEPID, ceReqTypeCall, ceReqClass0, 
			ceReqCallGetCallType, data->channel, 0, NULL, rcvmsg, SAR_TIMEOUT);
		if (!status && !(rcvmsg->rsp_status)) {
			pr_debug("%s: SCIOCGETSPEED: command successful\n",
				sc_adapter[card]->devicename);
		}
		else {
			pr_debug("%s: SCIOCGETSPEED: command failed (status = %d)\n",
				sc_adapter[card]->devicename, status);
=======
		status = send_and_receive(card, CEPID, ceReqTypeCall, ceReqClass0,
					  ceReqCallGetCallType, data->channel, 0, NULL, rcvmsg, SAR_TIMEOUT);
		if (!status && !(rcvmsg->rsp_status)) {
			pr_debug("%s: SCIOCGETSPEED: command successful\n",
				 sc_adapter[card]->devicename);
		}
		else {
			pr_debug("%s: SCIOCGETSPEED: command failed (status = %d)\n",
				 sc_adapter[card]->devicename, status);
>>>>>>> refs/remotes/origin/cm-10.0
			kfree(rcvmsg);
			return status;
		}

		speed = rcvmsg->msg_data.byte_array[0];

		kfree(rcvmsg);

		/*
		 * Package the switch type and send to user space
		 */

		if (copy_to_user(data->dataptr, &speed, sizeof(char)))
			return -EFAULT;

		return 0;
	}

	case SCIOCSETSPEED:
		pr_debug("%s: SCIOCSETSPEED: ioctl received\n",
<<<<<<< HEAD
				sc_adapter[card]->devicename);
=======
			 sc_adapter[card]->devicename);
>>>>>>> refs/remotes/origin/cm-10.0
		break;

	case SCIOCLOOPTST:
		pr_debug("%s: SCIOCLOOPTST: ioctl received\n",
<<<<<<< HEAD
				sc_adapter[card]->devicename);
=======
			 sc_adapter[card]->devicename);
>>>>>>> refs/remotes/origin/cm-10.0
		break;

	default:
		kfree(rcvmsg);
		return -1;
	}

	kfree(rcvmsg);
	return 0;
}

static int GetStatus(int card, boardInfo *bi)
{
	RspMessage rcvmsg;
	int i, status;

	/*
	 * Fill in some of the basic info about the board
	 */
	bi->modelid = sc_adapter[card]->model;
	strcpy(bi->serial_no, sc_adapter[card]->hwconfig.serial_no);
	strcpy(bi->part_no, sc_adapter[card]->hwconfig.part_no);
	bi->iobase = sc_adapter[card]->iobase;
	bi->rambase = sc_adapter[card]->rambase;
	bi->irq = sc_adapter[card]->interrupt;
	bi->ramsize = sc_adapter[card]->hwconfig.ram_size;
	bi->interface = sc_adapter[card]->hwconfig.st_u_sense;
	strcpy(bi->load_ver, sc_adapter[card]->load_ver);
	strcpy(bi->proc_ver, sc_adapter[card]->proc_ver);

	/*
	 * Get the current PhyStats and LnkStats
	 */
	status = send_and_receive(card, CEPID, ceReqTypePhy, ceReqClass2,
<<<<<<< HEAD
		ceReqPhyStatus, 0, 0, NULL, &rcvmsg, SAR_TIMEOUT);
	if(!status) {
		if(sc_adapter[card]->model < PRI_BOARD) {
			bi->l1_status = rcvmsg.msg_data.byte_array[2];
			for(i = 0 ; i < BRI_CHANNELS ; i++)
=======
				  ceReqPhyStatus, 0, 0, NULL, &rcvmsg, SAR_TIMEOUT);
	if (!status) {
		if (sc_adapter[card]->model < PRI_BOARD) {
			bi->l1_status = rcvmsg.msg_data.byte_array[2];
			for (i = 0; i < BRI_CHANNELS; i++)
>>>>>>> refs/remotes/origin/cm-10.0
				bi->status.bristats[i].phy_stat =
					rcvmsg.msg_data.byte_array[i];
		}
		else {
			bi->l1_status = rcvmsg.msg_data.byte_array[0];
			bi->l2_status = rcvmsg.msg_data.byte_array[1];
<<<<<<< HEAD
			for(i = 0 ; i < PRI_CHANNELS ; i++)
				bi->status.pristats[i].phy_stat = 
					rcvmsg.msg_data.byte_array[i+2];
		}
	}
	
	/*
	 * Get the call types for each channel
	 */
	for (i = 0 ; i < sc_adapter[card]->nChannels ; i++) {
		status = send_and_receive(card, CEPID, ceReqTypeCall, ceReqClass0,
			ceReqCallGetCallType, 0, 0, NULL, &rcvmsg, SAR_TIMEOUT);
		if(!status) {
			if (sc_adapter[card]->model == PRI_BOARD) {
				bi->status.pristats[i].call_type = 
=======
			for (i = 0; i < PRI_CHANNELS; i++)
				bi->status.pristats[i].phy_stat =
					rcvmsg.msg_data.byte_array[i + 2];
		}
	}

	/*
	 * Get the call types for each channel
	 */
	for (i = 0; i < sc_adapter[card]->nChannels; i++) {
		status = send_and_receive(card, CEPID, ceReqTypeCall, ceReqClass0,
					  ceReqCallGetCallType, 0, 0, NULL, &rcvmsg, SAR_TIMEOUT);
		if (!status) {
			if (sc_adapter[card]->model == PRI_BOARD) {
				bi->status.pristats[i].call_type =
>>>>>>> refs/remotes/origin/cm-10.0
					rcvmsg.msg_data.byte_array[0];
			}
			else {
				bi->status.bristats[i].call_type =
					rcvmsg.msg_data.byte_array[0];
			}
		}
	}
<<<<<<< HEAD
	
=======

>>>>>>> refs/remotes/origin/cm-10.0
	/*
	 * If PRI, get the call states and service states for each channel
	 */
	if (sc_adapter[card]->model == PRI_BOARD) {
		/*
		 * Get the call states
		 */
		status = send_and_receive(card, CEPID, ceReqTypeStat, ceReqClass2,
<<<<<<< HEAD
			ceReqPhyChCallState, 0, 0, NULL, &rcvmsg, SAR_TIMEOUT);
		if(!status) {
			for( i = 0 ; i < PRI_CHANNELS ; i++ )
				bi->status.pristats[i].call_state = 
=======
					  ceReqPhyChCallState, 0, 0, NULL, &rcvmsg, SAR_TIMEOUT);
		if (!status) {
			for (i = 0; i < PRI_CHANNELS; i++)
				bi->status.pristats[i].call_state =
>>>>>>> refs/remotes/origin/cm-10.0
					rcvmsg.msg_data.byte_array[i];
		}

		/*
		 * Get the service states
		 */
		status = send_and_receive(card, CEPID, ceReqTypeStat, ceReqClass2,
<<<<<<< HEAD
			ceReqPhyChServState, 0, 0, NULL, &rcvmsg, SAR_TIMEOUT);
		if(!status) {
			for( i = 0 ; i < PRI_CHANNELS ; i++ )
				bi->status.pristats[i].serv_state = 
=======
					  ceReqPhyChServState, 0, 0, NULL, &rcvmsg, SAR_TIMEOUT);
		if (!status) {
			for (i = 0; i < PRI_CHANNELS; i++)
				bi->status.pristats[i].serv_state =
>>>>>>> refs/remotes/origin/cm-10.0
					rcvmsg.msg_data.byte_array[i];
		}

		/*
		 * Get the link stats for the channels
		 */
<<<<<<< HEAD
		for (i = 1 ; i <= PRI_CHANNELS ; i++) {
			status = send_and_receive(card, CEPID, ceReqTypeLnk, ceReqClass0,
				ceReqLnkGetStats, i, 0, NULL, &rcvmsg, SAR_TIMEOUT);
			if (!status) {
				bi->status.pristats[i-1].link_stats.tx_good =
					(unsigned long)rcvmsg.msg_data.byte_array[0];
				bi->status.pristats[i-1].link_stats.tx_bad =
					(unsigned long)rcvmsg.msg_data.byte_array[4];
				bi->status.pristats[i-1].link_stats.rx_good =
					(unsigned long)rcvmsg.msg_data.byte_array[8];
				bi->status.pristats[i-1].link_stats.rx_bad =
=======
		for (i = 1; i <= PRI_CHANNELS; i++) {
			status = send_and_receive(card, CEPID, ceReqTypeLnk, ceReqClass0,
						  ceReqLnkGetStats, i, 0, NULL, &rcvmsg, SAR_TIMEOUT);
			if (!status) {
				bi->status.pristats[i - 1].link_stats.tx_good =
					(unsigned long)rcvmsg.msg_data.byte_array[0];
				bi->status.pristats[i - 1].link_stats.tx_bad =
					(unsigned long)rcvmsg.msg_data.byte_array[4];
				bi->status.pristats[i - 1].link_stats.rx_good =
					(unsigned long)rcvmsg.msg_data.byte_array[8];
				bi->status.pristats[i - 1].link_stats.rx_bad =
>>>>>>> refs/remotes/origin/cm-10.0
					(unsigned long)rcvmsg.msg_data.byte_array[12];
			}
		}

		/*
		 * Link stats for the D channel
		 */
		status = send_and_receive(card, CEPID, ceReqTypeLnk, ceReqClass0,
<<<<<<< HEAD
			ceReqLnkGetStats, 0, 0, NULL, &rcvmsg, SAR_TIMEOUT);
=======
					  ceReqLnkGetStats, 0, 0, NULL, &rcvmsg, SAR_TIMEOUT);
>>>>>>> refs/remotes/origin/cm-10.0
		if (!status) {
			bi->dch_stats.tx_good = (unsigned long)rcvmsg.msg_data.byte_array[0];
			bi->dch_stats.tx_bad = (unsigned long)rcvmsg.msg_data.byte_array[4];
			bi->dch_stats.rx_good = (unsigned long)rcvmsg.msg_data.byte_array[8];
			bi->dch_stats.rx_bad = (unsigned long)rcvmsg.msg_data.byte_array[12];
		}

		return 0;
	}

	/*
	 * If BRI or POTS, Get SPID, DN and call types for each channel
	 */

	/*
	 * Get the link stats for the channels
	 */
	status = send_and_receive(card, CEPID, ceReqTypeLnk, ceReqClass0,
<<<<<<< HEAD
		ceReqLnkGetStats, 0, 0, NULL, &rcvmsg, SAR_TIMEOUT);
=======
				  ceReqLnkGetStats, 0, 0, NULL, &rcvmsg, SAR_TIMEOUT);
>>>>>>> refs/remotes/origin/cm-10.0
	if (!status) {
		bi->dch_stats.tx_good = (unsigned long)rcvmsg.msg_data.byte_array[0];
		bi->dch_stats.tx_bad = (unsigned long)rcvmsg.msg_data.byte_array[4];
		bi->dch_stats.rx_good = (unsigned long)rcvmsg.msg_data.byte_array[8];
		bi->dch_stats.rx_bad = (unsigned long)rcvmsg.msg_data.byte_array[12];
<<<<<<< HEAD
		bi->status.bristats[0].link_stats.tx_good = 
			(unsigned long)rcvmsg.msg_data.byte_array[16];
		bi->status.bristats[0].link_stats.tx_bad = 
			(unsigned long)rcvmsg.msg_data.byte_array[20];
		bi->status.bristats[0].link_stats.rx_good = 
			(unsigned long)rcvmsg.msg_data.byte_array[24];
		bi->status.bristats[0].link_stats.rx_bad = 
			(unsigned long)rcvmsg.msg_data.byte_array[28];
		bi->status.bristats[1].link_stats.tx_good = 
			(unsigned long)rcvmsg.msg_data.byte_array[32];
		bi->status.bristats[1].link_stats.tx_bad = 
			(unsigned long)rcvmsg.msg_data.byte_array[36];
		bi->status.bristats[1].link_stats.rx_good = 
			(unsigned long)rcvmsg.msg_data.byte_array[40];
		bi->status.bristats[1].link_stats.rx_bad = 
=======
		bi->status.bristats[0].link_stats.tx_good =
			(unsigned long)rcvmsg.msg_data.byte_array[16];
		bi->status.bristats[0].link_stats.tx_bad =
			(unsigned long)rcvmsg.msg_data.byte_array[20];
		bi->status.bristats[0].link_stats.rx_good =
			(unsigned long)rcvmsg.msg_data.byte_array[24];
		bi->status.bristats[0].link_stats.rx_bad =
			(unsigned long)rcvmsg.msg_data.byte_array[28];
		bi->status.bristats[1].link_stats.tx_good =
			(unsigned long)rcvmsg.msg_data.byte_array[32];
		bi->status.bristats[1].link_stats.tx_bad =
			(unsigned long)rcvmsg.msg_data.byte_array[36];
		bi->status.bristats[1].link_stats.rx_good =
			(unsigned long)rcvmsg.msg_data.byte_array[40];
		bi->status.bristats[1].link_stats.rx_bad =
>>>>>>> refs/remotes/origin/cm-10.0
			(unsigned long)rcvmsg.msg_data.byte_array[44];
	}

	/*
	 * Get the SPIDs
	 */
<<<<<<< HEAD
	for (i = 0 ; i < BRI_CHANNELS ; i++) {
		status = send_and_receive(card, CEPID, ceReqTypeCall, ceReqClass0,
			ceReqCallGetSPID, i+1, 0, NULL, &rcvmsg, SAR_TIMEOUT);
		if (!status)
			strcpy(bi->status.bristats[i].spid, rcvmsg.msg_data.byte_array);
	}
		
	/*
	 * Get the DNs
	 */
	for (i = 0 ; i < BRI_CHANNELS ; i++) {
		status = send_and_receive(card, CEPID, ceReqTypeCall, ceReqClass0,
			ceReqCallGetMyNumber, i+1, 0, NULL, &rcvmsg, SAR_TIMEOUT);
		if (!status)
			strcpy(bi->status.bristats[i].dn, rcvmsg.msg_data.byte_array);
	}
		
=======
	for (i = 0; i < BRI_CHANNELS; i++) {
		status = send_and_receive(card, CEPID, ceReqTypeCall, ceReqClass0,
					  ceReqCallGetSPID, i + 1, 0, NULL, &rcvmsg, SAR_TIMEOUT);
		if (!status)
			strcpy(bi->status.bristats[i].spid, rcvmsg.msg_data.byte_array);
	}

	/*
	 * Get the DNs
	 */
	for (i = 0; i < BRI_CHANNELS; i++) {
		status = send_and_receive(card, CEPID, ceReqTypeCall, ceReqClass0,
					  ceReqCallGetMyNumber, i + 1, 0, NULL, &rcvmsg, SAR_TIMEOUT);
		if (!status)
			strcpy(bi->status.bristats[i].dn, rcvmsg.msg_data.byte_array);
	}

>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}
