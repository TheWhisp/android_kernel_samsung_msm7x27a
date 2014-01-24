#include "headers.h"

/*
<<<<<<< HEAD
Function:				InterfaceIdleModeWakeup

Description:			This is the hardware specific Function for waking up HW device from Idle mode.
						A software abort pattern is written to the device to wake it and necessary power state
						transitions from host are performed here.

Input parameters:		IN PMINI_ADAPTER Adapter   - Miniport Adapter Context


Return:				BCM_STATUS_SUCCESS - If Wakeup of the HW Interface was successful.
						Other           - If an error occurred.
*/


/*
Function:				InterfaceIdleModeRespond

Description:			This is the hardware specific Function for responding to Idle mode request from target.
						Necessary power state transitions from host for idle mode or other device specific
						initializations are performed here.

Input parameters:		IN PMINI_ADAPTER Adapter   - Miniport Adapter Context


Return:				BCM_STATUS_SUCCESS - If Idle mode response related HW configuration was successful.
						Other           - If an error occurred.
=======
Function:	InterfaceIdleModeWakeup

Description:	This is the hardware specific Function for
		waking up HW device from Idle mode.
		A software abort pattern is written to the
		device to wake it and necessary power state
		transitions from host are performed here.

Input parameters: IN struct bcm_mini_adapter *Adapter
		  - Miniport Adapter Context

Return:		BCM_STATUS_SUCCESS - If Wakeup of the HW Interface
				     was successful.
		Other              - If an error occurred.
*/

/*
Function:	InterfaceIdleModeRespond

Description:	This is the hardware specific Function for
		responding to Idle mode request from target.
		Necessary power state transitions from host for
		idle mode or other device specific initializations
		are performed here.

Input parameters: IN struct bcm_mini_adapter * Adapter
		  - Miniport Adapter Context

Return:		BCM_STATUS_SUCCESS - If Idle mode response related
				     HW configuration was successful.
		Other              - If an error occurred.
>>>>>>> refs/remotes/origin/master
*/

/*
"dmem bfc02f00  100" tells how many time device went in Idle mode.
this value will be at address bfc02fa4.just before value d0ea1dle.

Set time value by writing at bfc02f98 7d0

checking the Ack timer expire on kannon by running command
<<<<<<< HEAD
d qcslog .. if it shows e means host has not send response to f/w with in 200 ms. Response should be
=======
d qcslog .. if it shows e means host has not send response
to f/w with in 200 ms. Response should be
>>>>>>> refs/remotes/origin/master
send to f/w with in 200 ms after the Idle/Shutdown req issued

*/


<<<<<<< HEAD
int InterfaceIdleModeRespond(PMINI_ADAPTER Adapter, unsigned int* puiBuffer)
{
	int	status = STATUS_SUCCESS;
	unsigned int	uiRegRead = 0;
<<<<<<< HEAD
=======
	int bytes;
>>>>>>> refs/remotes/origin/cm-10.0

	BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, IDLE_MODE, DBG_LVL_ALL,"SubType of Message :0x%X", ntohl(*puiBuffer));

	if(ntohl(*puiBuffer) == GO_TO_IDLE_MODE_PAYLOAD)
	{
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, IDLE_MODE, DBG_LVL_ALL," Got GO_TO_IDLE_MODE_PAYLOAD(210) Msg Subtype");
		if(ntohl(*(puiBuffer+1)) == 0 )
		{
			BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, IDLE_MODE, DBG_LVL_ALL,"Got IDLE MODE WAKE UP Response From F/W");

			status = wrmalt (Adapter,SW_ABORT_IDLEMODE_LOC, &uiRegRead, sizeof(uiRegRead));
			if(status)
			{
				BCM_DEBUG_PRINT(Adapter,DBG_TYPE_PRINTK, 0, 0, "wrm failed while clearing Idle Mode Reg");
				return status;
			}

			if(Adapter->ulPowerSaveMode == DEVICE_POWERSAVE_MODE_AS_MANUAL_CLOCK_GATING)
			{
				uiRegRead = 0x00000000 ;
				status = wrmalt (Adapter,DEBUG_INTERRUPT_GENERATOR_REGISTOR, &uiRegRead, sizeof(uiRegRead));
				if(status)
				{
					BCM_DEBUG_PRINT(Adapter,DBG_TYPE_PRINTK, 0, 0, "wrm failed while clearing Idle Mode	Reg");
					return status;
				}
			}
			//Below Register should not br read in case of Manual and Protocol Idle mode.
			else if(Adapter->ulPowerSaveMode != DEVICE_POWERSAVE_MODE_AS_PROTOCOL_IDLE_MODE)
			{
				//clear on read Register
<<<<<<< HEAD
				status = rdmalt(Adapter, DEVICE_INT_OUT_EP_REG0, &uiRegRead, sizeof(uiRegRead));
				if(status)
				{
=======
				bytes = rdmalt(Adapter, DEVICE_INT_OUT_EP_REG0, &uiRegRead, sizeof(uiRegRead));
				if (bytes < 0) {
					status = bytes;
>>>>>>> refs/remotes/origin/cm-10.0
					BCM_DEBUG_PRINT(Adapter,DBG_TYPE_PRINTK, 0, 0, "rdm failed while clearing H/W Abort Reg0");
					return status;
				}
				//clear on read Register
<<<<<<< HEAD
				status = rdmalt (Adapter, DEVICE_INT_OUT_EP_REG1, &uiRegRead, sizeof(uiRegRead));
				if(status)
				{
=======
				bytes = rdmalt(Adapter, DEVICE_INT_OUT_EP_REG1, &uiRegRead, sizeof(uiRegRead));
				if (bytes < 0) {
					status = bytes;
>>>>>>> refs/remotes/origin/cm-10.0
					BCM_DEBUG_PRINT(Adapter,DBG_TYPE_PRINTK, 0, 0, "rdm failed while clearing H/W Abort	Reg1");
					return status;
				}
			}
			BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, IDLE_MODE, DBG_LVL_ALL, "Device Up from Idle Mode");

			// Set Idle Mode Flag to False and Clear IdleMode reg.
			Adapter->IdleMode = FALSE;
			Adapter->bTriedToWakeUpFromlowPowerMode = FALSE;

			wake_up(&Adapter->lowpower_mode_wait_queue);

		}
		else
		{
			if(TRUE == Adapter->IdleMode)
			{
				BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, IDLE_MODE, DBG_LVL_ALL,"Device is already in Idle mode....");
				return status ;
			}

			uiRegRead = 0;
			BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, IDLE_MODE, DBG_LVL_ALL, "Got Req from F/W to go in IDLE mode \n");

			if (Adapter->chip_id== BCS220_2 ||
				Adapter->chip_id == BCS220_2BC ||
					Adapter->chip_id== BCS250_BC ||
					Adapter->chip_id== BCS220_3)
			{

<<<<<<< HEAD
				status = rdmalt(Adapter, HPM_CONFIG_MSW, &uiRegRead, sizeof(uiRegRead));
				if(status)
				{
=======
				bytes = rdmalt(Adapter, HPM_CONFIG_MSW, &uiRegRead, sizeof(uiRegRead));
				if (bytes < 0) {
					status = bytes;
>>>>>>> refs/remotes/origin/cm-10.0
					BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, IDLE_MODE, DBG_LVL_ALL, "rdm failed while Reading HPM_CONFIG_LDO145 Reg 0\n");
=======
int InterfaceIdleModeRespond(struct bcm_mini_adapter *Adapter,
			unsigned int *puiBuffer)
{
	int	status = STATUS_SUCCESS;
	unsigned int	uiRegRead = 0;
	int bytes;

	if (ntohl(*puiBuffer) == GO_TO_IDLE_MODE_PAYLOAD) {
		if (ntohl(*(puiBuffer+1)) == 0) {

			status = wrmalt(Adapter, SW_ABORT_IDLEMODE_LOC,
					&uiRegRead, sizeof(uiRegRead));
			if (status)
				return status;

			if (Adapter->ulPowerSaveMode ==
				DEVICE_POWERSAVE_MODE_AS_MANUAL_CLOCK_GATING) {
				uiRegRead = 0x00000000;
				status = wrmalt(Adapter,
					DEBUG_INTERRUPT_GENERATOR_REGISTOR,
					&uiRegRead, sizeof(uiRegRead));
				if (status)
					return status;
			}
			/* Below Register should not br read in case of
			 * Manual and Protocol Idle mode */
			else if (Adapter->ulPowerSaveMode !=
				DEVICE_POWERSAVE_MODE_AS_PROTOCOL_IDLE_MODE) {
				/* clear on read Register */
				bytes = rdmalt(Adapter, DEVICE_INT_OUT_EP_REG0,
					&uiRegRead, sizeof(uiRegRead));
				if (bytes < 0) {
					status = bytes;
					return status;
				}
				/* clear on read Register */
				bytes = rdmalt(Adapter, DEVICE_INT_OUT_EP_REG1,
					&uiRegRead, sizeof(uiRegRead));
				if (bytes < 0) {
					status = bytes;
					return status;
				}
			}

			/* Set Idle Mode Flag to False and
			 * Clear IdleMode reg. */
			Adapter->IdleMode = false;
			Adapter->bTriedToWakeUpFromlowPowerMode = false;

			wake_up(&Adapter->lowpower_mode_wait_queue);

		} else {
			if (TRUE == Adapter->IdleMode)
				return status;

			uiRegRead = 0;

			if (Adapter->chip_id == BCS220_2 ||
				Adapter->chip_id == BCS220_2BC ||
					Adapter->chip_id == BCS250_BC ||
					Adapter->chip_id == BCS220_3) {

				bytes = rdmalt(Adapter, HPM_CONFIG_MSW,
					&uiRegRead, sizeof(uiRegRead));
				if (bytes < 0) {
					status = bytes;
>>>>>>> refs/remotes/origin/master
					return status;
				}


				uiRegRead |= (1<<17);

<<<<<<< HEAD
				status = wrmalt (Adapter,HPM_CONFIG_MSW, &uiRegRead, sizeof(uiRegRead));
				if(status)
				{
					BCM_DEBUG_PRINT(Adapter,DBG_TYPE_PRINTK, 0, 0, "wrm failed while clearing Idle Mode Reg\n");
					return status;
				}

			}
			SendIdleModeResponse(Adapter);
		}
	}
	else if(ntohl(*puiBuffer) == IDLE_MODE_SF_UPDATE_MSG)
	{
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, IDLE_MODE, DBG_LVL_ALL, "OverRiding Service Flow Params");
		OverrideServiceFlowParams(Adapter,puiBuffer);
=======
				status = wrmalt(Adapter, HPM_CONFIG_MSW,
					&uiRegRead, sizeof(uiRegRead));
				if (status)
					return status;
			}
			SendIdleModeResponse(Adapter);
		}
	} else if (ntohl(*puiBuffer) == IDLE_MODE_SF_UPDATE_MSG) {
		OverrideServiceFlowParams(Adapter, puiBuffer);
>>>>>>> refs/remotes/origin/master
	}
	return status;
}

<<<<<<< HEAD
static int InterfaceAbortIdlemode(PMINI_ADAPTER Adapter, unsigned int Pattern)
{
	int 	status = STATUS_SUCCESS;
	unsigned int value;
	unsigned int chip_id ;
	unsigned long timeout = 0 ,itr = 0;

	int 	lenwritten = 0;
	unsigned char aucAbortPattern[8]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	PS_INTERFACE_ADAPTER psInterfaceAdapter = Adapter->pvInterfaceAdapter;

	//Abort Bus suspend if its already suspended
	if((TRUE == psInterfaceAdapter->bSuspended) && (TRUE == Adapter->bDoSuspend))
	{
		status = usb_autopm_get_interface(psInterfaceAdapter->interface);
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, IDLE_MODE, DBG_LVL_ALL,"Bus got wakeup..Aborting Idle mode... status:%d \n",status);

	}

	if((Adapter->ulPowerSaveMode == DEVICE_POWERSAVE_MODE_AS_MANUAL_CLOCK_GATING)
									||
	   (Adapter->ulPowerSaveMode == DEVICE_POWERSAVE_MODE_AS_PROTOCOL_IDLE_MODE))
	{
		//write the SW abort pattern.
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, IDLE_MODE, DBG_LVL_ALL, "Writing pattern<%d> to SW_ABORT_IDLEMODE_LOC\n", Pattern);
		status = wrmalt(Adapter,SW_ABORT_IDLEMODE_LOC, &Pattern, sizeof(Pattern));
		if(status)
		{
				BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, IDLE_MODE, DBG_LVL_ALL,"WRM to Register SW_ABORT_IDLEMODE_LOC failed..");
				return status;
		}
	}

	if(Adapter->ulPowerSaveMode == DEVICE_POWERSAVE_MODE_AS_MANUAL_CLOCK_GATING)
	{
		value = 0x80000000;
		status = wrmalt(Adapter,DEBUG_INTERRUPT_GENERATOR_REGISTOR, &value, sizeof(value));
		if(status)
		{
			BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, IDLE_MODE, DBG_LVL_ALL,"WRM to DEBUG_INTERRUPT_GENERATOR_REGISTOR Register failed");
			return status;
		}
	}
	else if(Adapter->ulPowerSaveMode != DEVICE_POWERSAVE_MODE_AS_PROTOCOL_IDLE_MODE)
	{
=======
static int InterfaceAbortIdlemode(struct bcm_mini_adapter *Adapter,
				unsigned int Pattern)
{
	int status = STATUS_SUCCESS;
	unsigned int value;
	unsigned int chip_id;
	unsigned long timeout = 0, itr = 0;

	int lenwritten = 0;
	unsigned char aucAbortPattern[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
						0xFF, 0xFF, 0xFF};
	struct bcm_interface_adapter *psInterfaceAdapter =
				Adapter->pvInterfaceAdapter;

	/* Abort Bus suspend if its already suspended */
	if ((TRUE == psInterfaceAdapter->bSuspended) &&
			(TRUE == Adapter->bDoSuspend))
		status = usb_autopm_get_interface(
				psInterfaceAdapter->interface);

	if ((Adapter->ulPowerSaveMode ==
			DEVICE_POWERSAVE_MODE_AS_MANUAL_CLOCK_GATING) ||
	   (Adapter->ulPowerSaveMode ==
			DEVICE_POWERSAVE_MODE_AS_PROTOCOL_IDLE_MODE)) {
		/* write the SW abort pattern. */
		status = wrmalt(Adapter, SW_ABORT_IDLEMODE_LOC,
				&Pattern, sizeof(Pattern));
		if (status)
			return status;
	}

	if (Adapter->ulPowerSaveMode ==
		DEVICE_POWERSAVE_MODE_AS_MANUAL_CLOCK_GATING) {
		value = 0x80000000;
		status = wrmalt(Adapter,
				DEBUG_INTERRUPT_GENERATOR_REGISTOR,
				&value, sizeof(value));
		if (status)
			return status;
	} else if (Adapter->ulPowerSaveMode !=
			DEVICE_POWERSAVE_MODE_AS_PROTOCOL_IDLE_MODE) {
>>>>>>> refs/remotes/origin/master
		/*
		 * Get a Interrupt Out URB and send 8 Bytes Down
		 * To be Done in Thread Context.
		 * Not using Asynchronous Mechanism.
		 */
<<<<<<< HEAD
		status = usb_interrupt_msg (psInterfaceAdapter->udev,
=======
		status = usb_interrupt_msg(psInterfaceAdapter->udev,
>>>>>>> refs/remotes/origin/master
			usb_sndintpipe(psInterfaceAdapter->udev,
			psInterfaceAdapter->sIntrOut.int_out_endpointAddr),
			aucAbortPattern,
			8,
			&lenwritten,
			5000);
<<<<<<< HEAD
		if(status)
		{
			BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, IDLE_MODE, DBG_LVL_ALL, "Sending Abort pattern down fails with status:%d..\n",status);
			return status;
		}
		else
		{
			BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, IDLE_MODE, DBG_LVL_ALL, "NOB Sent down :%d", lenwritten);
		}

		//mdelay(25);

		timeout= jiffies +  msecs_to_jiffies(50) ;
		while( timeout > jiffies )
		{
			itr++ ;
			rdmalt(Adapter, CHIP_ID_REG, &chip_id, sizeof(UINT));
			if(0xbece3200==(chip_id&~(0xF0)))
			{
				chip_id = chip_id&~(0xF0);
			}
			if(chip_id == Adapter->chip_id)
				break;
		}
		if(timeout < jiffies )
		{
			BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, IDLE_MODE, DBG_LVL_ALL,"Not able to read chip-id even after 25 msec");
		}
		else
		{
			BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, IDLE_MODE, DBG_LVL_ALL,"Number of completed iteration to read chip-id :%lu", itr);
		}

		status = wrmalt(Adapter,SW_ABORT_IDLEMODE_LOC, &Pattern, sizeof(status));
		if(status)
		{
			BCM_DEBUG_PRINT(Adapter,DBG_TYPE_PRINTK, 0, 0,"WRM to Register SW_ABORT_IDLEMODE_LOC failed..");
			return status;
		}
	}
	return status;
}
int InterfaceIdleModeWakeup(PMINI_ADAPTER Adapter)
{
	ULONG	Status = 0;
	if(Adapter->bTriedToWakeUpFromlowPowerMode)
	{
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, IDLE_MODE, DBG_LVL_ALL, "Wake up already attempted.. ignoring\n");
	}
	else
	{
		BCM_DEBUG_PRINT(Adapter,DBG_TYPE_OTHERS, IDLE_MODE, DBG_LVL_ALL,"Writing Low Power Mode Abort pattern to the Device\n");
=======
		if (status)
			return status;
		else
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS,
				IDLE_MODE, DBG_LVL_ALL,
				"NOB Sent down :%d", lenwritten);

		/* mdelay(25); */

		timeout = jiffies +  msecs_to_jiffies(50);
		while (time_after(timeout, jiffies)) {
			itr++;
			rdmalt(Adapter, CHIP_ID_REG, &chip_id, sizeof(UINT));
			if (0xbece3200 == (chip_id&~(0xF0)))
				chip_id = chip_id&~(0xF0);
			if (chip_id == Adapter->chip_id)
				break;
		}
		if (time_before(timeout, jiffies))
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS,
				IDLE_MODE, DBG_LVL_ALL,
				"Not able to read chip-id even after 25 msec");
		else
			BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS,
				IDLE_MODE, DBG_LVL_ALL,
				"Number of completed iteration to"
				"read chip-id :%lu", itr);

		status = wrmalt(Adapter, SW_ABORT_IDLEMODE_LOC,
				&Pattern, sizeof(status));
		if (status)
			return status;
	}
	return status;
}
int InterfaceIdleModeWakeup(struct bcm_mini_adapter *Adapter)
{
	ULONG	Status = 0;
	if (Adapter->bTriedToWakeUpFromlowPowerMode) {
		BCM_DEBUG_PRINT(Adapter, DBG_TYPE_OTHERS,
		IDLE_MODE, DBG_LVL_ALL,
		"Wake up already attempted.. ignoring\n");
	} else {
>>>>>>> refs/remotes/origin/master
		Adapter->bTriedToWakeUpFromlowPowerMode = TRUE;
		InterfaceAbortIdlemode(Adapter, Adapter->usIdleModePattern);

	}
	return Status;
}

<<<<<<< HEAD
void InterfaceHandleShutdownModeWakeup(PMINI_ADAPTER Adapter)
{
	unsigned int uiRegVal = 0;
	INT Status = 0;
<<<<<<< HEAD
=======
	int bytes;

>>>>>>> refs/remotes/origin/cm-10.0
	if(Adapter->ulPowerSaveMode == DEVICE_POWERSAVE_MODE_AS_MANUAL_CLOCK_GATING)
	{
		// clear idlemode interrupt.
		uiRegVal = 0;
		Status =wrmalt(Adapter,DEBUG_INTERRUPT_GENERATOR_REGISTOR, &uiRegVal, sizeof(uiRegVal));
		if(Status)
		{
			BCM_DEBUG_PRINT(Adapter,DBG_TYPE_PRINTK, 0, 0,"WRM to DEBUG_INTERRUPT_GENERATOR_REGISTOR Failed with err :%d", Status);
			return;
		}
	}

    else
	{

        //clear Interrupt EP registers.
<<<<<<< HEAD
		Status = rdmalt(Adapter,DEVICE_INT_OUT_EP_REG0, &uiRegVal, sizeof(uiRegVal));
		if(Status)
		{
=======
		bytes = rdmalt(Adapter,DEVICE_INT_OUT_EP_REG0, &uiRegVal, sizeof(uiRegVal));
		if (bytes < 0) {
			Status = bytes;
>>>>>>> refs/remotes/origin/cm-10.0
			BCM_DEBUG_PRINT(Adapter,DBG_TYPE_PRINTK, 0, 0,"RDM of DEVICE_INT_OUT_EP_REG0 failed with Err :%d", Status);
			return;
		}

<<<<<<< HEAD
        Status = rdmalt(Adapter,DEVICE_INT_OUT_EP_REG1, &uiRegVal, sizeof(uiRegVal));
		if(Status)
		{
=======
		bytes = rdmalt(Adapter,DEVICE_INT_OUT_EP_REG1, &uiRegVal, sizeof(uiRegVal));
		if (bytes < 0) {
			Status = bytes;
>>>>>>> refs/remotes/origin/cm-10.0
			BCM_DEBUG_PRINT(Adapter,DBG_TYPE_PRINTK, 0, 0,"RDM of DEVICE_INT_OUT_EP_REG1 failed with Err :%d", Status);
=======
void InterfaceHandleShutdownModeWakeup(struct bcm_mini_adapter *Adapter)
{
	unsigned int uiRegVal = 0;
	INT Status = 0;
	int bytes;

	if (Adapter->ulPowerSaveMode ==
		DEVICE_POWERSAVE_MODE_AS_MANUAL_CLOCK_GATING) {
		/* clear idlemode interrupt. */
		uiRegVal = 0;
		Status = wrmalt(Adapter,
			DEBUG_INTERRUPT_GENERATOR_REGISTOR,
			&uiRegVal, sizeof(uiRegVal));
		if (Status)
			return;
	}

	else {

/* clear Interrupt EP registers. */
		bytes = rdmalt(Adapter,
			DEVICE_INT_OUT_EP_REG0,
			&uiRegVal, sizeof(uiRegVal));
		if (bytes < 0) {
			Status = bytes;
			return;
		}

		bytes = rdmalt(Adapter,
			DEVICE_INT_OUT_EP_REG1,
			&uiRegVal, sizeof(uiRegVal));
		if (bytes < 0) {
			Status = bytes;
>>>>>>> refs/remotes/origin/master
			return;
		}
	}
}

