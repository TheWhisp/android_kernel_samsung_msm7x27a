/*
 * Driver for ST5481 USB ISDN modem
 *
 * Author       Frode Isaksen
 * Copyright    2001 by Frode Isaksen      <fisaksen@bewan.com>
 *              2001 by Kai Germaschewski  <kai.germaschewski@gmx.de>
<<<<<<< HEAD
<<<<<<< HEAD
 * 
=======
 *
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *
>>>>>>> refs/remotes/origin/master
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

#include <linux/init.h>
#include <linux/usb.h>
#include <linux/slab.h>
#include "st5481.h"

static int st5481_isoc_flatten(struct urb *urb);

/* ======================================================================
 * control pipe
 */

/*
 * Send the next endpoint 0 request stored in the FIFO.
 * Called either by the completion or by usb_ctrl_msg.
 */
static void usb_next_ctrl_msg(struct urb *urb,
			      struct st5481_adapter *adapter)
{
	struct st5481_ctrl *ctrl = &adapter->ctrl;
	int r_index;

	if (test_and_set_bit(0, &ctrl->busy)) {
		return;
	}

	if ((r_index = fifo_remove(&ctrl->msg_fifo.f)) < 0) {
<<<<<<< HEAD
<<<<<<< HEAD
		test_and_clear_bit(0,&ctrl->busy);
		return;
	} 
	urb->setup_packet = 
		(unsigned char *)&ctrl->msg_fifo.data[r_index];
	
	DBG(1,"request=0x%02x,value=0x%04x,index=%x",
=======
=======
>>>>>>> refs/remotes/origin/master
		test_and_clear_bit(0, &ctrl->busy);
		return;
	}
	urb->setup_packet =
		(unsigned char *)&ctrl->msg_fifo.data[r_index];

	DBG(1, "request=0x%02x,value=0x%04x,index=%x",
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	    ((struct ctrl_msg *)urb->setup_packet)->dr.bRequest,
	    ((struct ctrl_msg *)urb->setup_packet)->dr.wValue,
	    ((struct ctrl_msg *)urb->setup_packet)->dr.wIndex);

	// Prepare the URB
	urb->dev = adapter->usb_dev;

	SUBMIT_URB(urb, GFP_ATOMIC);
}

/*
 * Asynchronous endpoint 0 request (async version of usb_control_msg).
 * The request will be queued up in a FIFO if the endpoint is busy.
 */
static void usb_ctrl_msg(struct st5481_adapter *adapter,
			 u8 request, u8 requesttype, u16 value, u16 index,
			 ctrl_complete_t complete, void *context)
{
	struct st5481_ctrl *ctrl = &adapter->ctrl;
	int w_index;
	struct ctrl_msg *ctrl_msg;
<<<<<<< HEAD
<<<<<<< HEAD
	
=======

>>>>>>> refs/remotes/origin/cm-10.0
=======

>>>>>>> refs/remotes/origin/master
	if ((w_index = fifo_add(&ctrl->msg_fifo.f)) < 0) {
		WARNING("control msg FIFO full");
		return;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	ctrl_msg = &ctrl->msg_fifo.data[w_index]; 
   
=======
	ctrl_msg = &ctrl->msg_fifo.data[w_index];

>>>>>>> refs/remotes/origin/cm-10.0
=======
	ctrl_msg = &ctrl->msg_fifo.data[w_index];

>>>>>>> refs/remotes/origin/master
	ctrl_msg->dr.bRequestType = requesttype;
	ctrl_msg->dr.bRequest = request;
	ctrl_msg->dr.wValue = cpu_to_le16p(&value);
	ctrl_msg->dr.wIndex = cpu_to_le16p(&index);
	ctrl_msg->dr.wLength = 0;
	ctrl_msg->complete = complete;
	ctrl_msg->context = context;

	usb_next_ctrl_msg(ctrl->urb, adapter);
}

/*
 * Asynchronous endpoint 0 device request.
 */
void st5481_usb_device_ctrl_msg(struct st5481_adapter *adapter,
<<<<<<< HEAD
<<<<<<< HEAD
			 u8 request, u16 value,
			 ctrl_complete_t complete, void *context)
{
	usb_ctrl_msg(adapter, request, 
		     USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE, 
=======
=======
>>>>>>> refs/remotes/origin/master
				u8 request, u16 value,
				ctrl_complete_t complete, void *context)
{
	usb_ctrl_msg(adapter, request,
		     USB_DIR_OUT | USB_TYPE_VENDOR | USB_RECIP_DEVICE,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		     value, 0, complete, context);
}

/*
 * Asynchronous pipe reset (async version of usb_clear_halt).
 */
void st5481_usb_pipe_reset(struct st5481_adapter *adapter,
<<<<<<< HEAD
<<<<<<< HEAD
		    u_char pipe,
		    ctrl_complete_t complete, void *context)
{
	DBG(1,"pipe=%02x",pipe);
=======
=======
>>>>>>> refs/remotes/origin/master
			   u_char pipe,
			   ctrl_complete_t complete, void *context)
{
	DBG(1, "pipe=%02x", pipe);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	usb_ctrl_msg(adapter,
		     USB_REQ_CLEAR_FEATURE, USB_DIR_OUT | USB_RECIP_ENDPOINT,
		     0, pipe, complete, context);
}


/*
  Physical level functions
*/

void st5481_ph_command(struct st5481_adapter *adapter, unsigned int command)
{
<<<<<<< HEAD
<<<<<<< HEAD
	DBG(8,"command=%s", ST5481_CMD_string(command));
=======
	DBG(8, "command=%s", ST5481_CMD_string(command));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	DBG(8, "command=%s", ST5481_CMD_string(command));
>>>>>>> refs/remotes/origin/master

	st5481_usb_device_ctrl_msg(adapter, TXCI, command, NULL, NULL);
}

/*
 * The request on endpoint 0 has completed.
 * Call the user provided completion routine and try
 * to send the next request.
 */
static void usb_ctrl_complete(struct urb *urb)
{
	struct st5481_adapter *adapter = urb->context;
	struct st5481_ctrl *ctrl = &adapter->ctrl;
	struct ctrl_msg *ctrl_msg;
<<<<<<< HEAD
<<<<<<< HEAD
	
	if (unlikely(urb->status < 0)) {
		switch (urb->status) {
			case -ENOENT:
			case -ESHUTDOWN:
			case -ECONNRESET:
				DBG(1,"urb killed status %d", urb->status);
				return; // Give up
			default: 
				WARNING("urb status %d",urb->status);
				break;
=======
=======
>>>>>>> refs/remotes/origin/master

	if (unlikely(urb->status < 0)) {
		switch (urb->status) {
		case -ENOENT:
		case -ESHUTDOWN:
		case -ECONNRESET:
			DBG(1, "urb killed status %d", urb->status);
			return; // Give up
		default:
			WARNING("urb status %d", urb->status);
			break;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		}
	}

	ctrl_msg = (struct ctrl_msg *)urb->setup_packet;
<<<<<<< HEAD
<<<<<<< HEAD
	
	if (ctrl_msg->dr.bRequest == USB_REQ_CLEAR_FEATURE) {
	        /* Special case handling for pipe reset */
		le16_to_cpus(&ctrl_msg->dr.wIndex);
		usb_reset_endpoint(adapter->usb_dev, ctrl_msg->dr.wIndex);
	}
	
=======
=======
>>>>>>> refs/remotes/origin/master

	if (ctrl_msg->dr.bRequest == USB_REQ_CLEAR_FEATURE) {
		/* Special case handling for pipe reset */
		le16_to_cpus(&ctrl_msg->dr.wIndex);
		usb_reset_endpoint(adapter->usb_dev, ctrl_msg->dr.wIndex);
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (ctrl_msg->complete)
		ctrl_msg->complete(ctrl_msg->context);

	clear_bit(0, &ctrl->busy);
<<<<<<< HEAD
<<<<<<< HEAD
	
=======

>>>>>>> refs/remotes/origin/cm-10.0
=======

>>>>>>> refs/remotes/origin/master
	// Try to send next control message
	usb_next_ctrl_msg(urb, adapter);
	return;
}

/* ======================================================================
 * interrupt pipe
 */

/*
 * The interrupt endpoint will be called when any
 * of the 6 registers changes state (depending on masks).
 * Decode the register values and schedule a private event.
 * Called at interrupt.
 */
static void usb_int_complete(struct urb *urb)
{
	u8 *data = urb->transfer_buffer;
	u8 irqbyte;
	struct st5481_adapter *adapter = urb->context;
	int j;
	int status;

	switch (urb->status) {
<<<<<<< HEAD
<<<<<<< HEAD
		case 0:
			/* success */
			break;
		case -ECONNRESET:
		case -ENOENT:
		case -ESHUTDOWN:
			/* this urb is terminated, clean up */
			DBG(2, "urb shutting down with status: %d", urb->status);
			return;
		default:
			WARNING("nonzero urb status received: %d", urb->status);
			goto exit;
	}

	
	DBG_PACKET(2, data, INT_PKT_SIZE);
		
=======
=======
>>>>>>> refs/remotes/origin/master
	case 0:
		/* success */
		break;
	case -ECONNRESET:
	case -ENOENT:
	case -ESHUTDOWN:
		/* this urb is terminated, clean up */
		DBG(2, "urb shutting down with status: %d", urb->status);
		return;
	default:
		WARNING("nonzero urb status received: %d", urb->status);
		goto exit;
	}


	DBG_PACKET(2, data, INT_PKT_SIZE);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (urb->actual_length == 0) {
		goto exit;
	}

	irqbyte = data[MPINT];
	if (irqbyte & DEN_INT)
		FsmEvent(&adapter->d_out.fsm, EV_DOUT_DEN, NULL);

	if (irqbyte & DCOLL_INT)
		FsmEvent(&adapter->d_out.fsm, EV_DOUT_COLL, NULL);

	irqbyte = data[FFINT_D];
	if (irqbyte & OUT_UNDERRUN)
		FsmEvent(&adapter->d_out.fsm, EV_DOUT_UNDERRUN, NULL);

	if (irqbyte & OUT_DOWN)
<<<<<<< HEAD
<<<<<<< HEAD
;//		printk("OUT_DOWN\n");
=======
		;//		printk("OUT_DOWN\n");
>>>>>>> refs/remotes/origin/cm-10.0
=======
		;//		printk("OUT_DOWN\n");
>>>>>>> refs/remotes/origin/master

	irqbyte = data[MPINT];
	if (irqbyte & RXCI_INT)
		FsmEvent(&adapter->l1m, data[CCIST] & 0x0f, NULL);

	for (j = 0; j < 2; j++)
		adapter->bcs[j].b_out.flow_event |= data[FFINT_B1 + j];

	urb->actual_length = 0;

exit:
<<<<<<< HEAD
<<<<<<< HEAD
	status = usb_submit_urb (urb, GFP_ATOMIC);
=======
	status = usb_submit_urb(urb, GFP_ATOMIC);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	status = usb_submit_urb(urb, GFP_ATOMIC);
>>>>>>> refs/remotes/origin/master
	if (status)
		WARNING("usb_submit_urb failed with result %d", status);
}

/* ======================================================================
 * initialization
 */

int st5481_setup_usb(struct st5481_adapter *adapter)
{
	struct usb_device *dev = adapter->usb_dev;
	struct st5481_ctrl *ctrl = &adapter->ctrl;
	struct st5481_intr *intr = &adapter->intr;
	struct usb_interface *intf;
	struct usb_host_interface *altsetting = NULL;
	struct usb_host_endpoint *endpoint;
	int status;
	struct urb *urb;
	u8 *buf;
<<<<<<< HEAD
<<<<<<< HEAD
	
	DBG(2,"");
	
	if ((status = usb_reset_configuration (dev)) < 0) {
		WARNING("reset_configuration failed,status=%d",status);
=======
=======
>>>>>>> refs/remotes/origin/master

	DBG(2, "");

	if ((status = usb_reset_configuration(dev)) < 0) {
		WARNING("reset_configuration failed,status=%d", status);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		return status;
	}

	intf = usb_ifnum_to_if(dev, 0);
	if (intf)
		altsetting = usb_altnum_to_altsetting(intf, 3);
	if (!altsetting)
		return -ENXIO;

	// Check if the config is sane
<<<<<<< HEAD
<<<<<<< HEAD
	if ( altsetting->desc.bNumEndpoints != 7 ) {
=======
	if (altsetting->desc.bNumEndpoints != 7) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (altsetting->desc.bNumEndpoints != 7) {
>>>>>>> refs/remotes/origin/master
		WARNING("expecting 7 got %d endpoints!", altsetting->desc.bNumEndpoints);
		return -EINVAL;
	}

	// The descriptor is wrong for some early samples of the ST5481 chip
	altsetting->endpoint[3].desc.wMaxPacketSize = __constant_cpu_to_le16(32);
	altsetting->endpoint[4].desc.wMaxPacketSize = __constant_cpu_to_le16(32);

	// Use alternative setting 3 on interface 0 to have 2B+D
<<<<<<< HEAD
<<<<<<< HEAD
	if ((status = usb_set_interface (dev, 0, 3)) < 0) {
		WARNING("usb_set_interface failed,status=%d",status);
=======
	if ((status = usb_set_interface(dev, 0, 3)) < 0) {
		WARNING("usb_set_interface failed,status=%d", status);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if ((status = usb_set_interface(dev, 0, 3)) < 0) {
		WARNING("usb_set_interface failed,status=%d", status);
>>>>>>> refs/remotes/origin/master
		return status;
	}

	// Allocate URB for control endpoint
	urb = usb_alloc_urb(0, GFP_KERNEL);
	if (!urb) {
		return -ENOMEM;
	}
	ctrl->urb = urb;
<<<<<<< HEAD
<<<<<<< HEAD
	
	// Fill the control URB
	usb_fill_control_urb (urb, dev, 
			  usb_sndctrlpipe(dev, 0),
			  NULL, NULL, 0, usb_ctrl_complete, adapter);

		
=======
=======
>>>>>>> refs/remotes/origin/master

	// Fill the control URB
	usb_fill_control_urb(urb, dev,
			     usb_sndctrlpipe(dev, 0),
			     NULL, NULL, 0, usb_ctrl_complete, adapter);


<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	fifo_init(&ctrl->msg_fifo.f, ARRAY_SIZE(ctrl->msg_fifo.data));

	// Allocate URBs and buffers for interrupt endpoint
	urb = usb_alloc_urb(0, GFP_KERNEL);
<<<<<<< HEAD
<<<<<<< HEAD
	if (!urb) { 
		return -ENOMEM;
	}
	intr->urb = urb;
	
=======
	if (!urb) {
		return -ENOMEM;
	}
	intr->urb = urb;

>>>>>>> refs/remotes/origin/cm-10.0
	buf = kmalloc(INT_PKT_SIZE, GFP_KERNEL);
	if (!buf) {
		return -ENOMEM;
	}

	endpoint = &altsetting->endpoint[EP_INT-1];
<<<<<<< HEAD
				
	// Fill the interrupt URB
	usb_fill_int_urb(urb, dev,
		     usb_rcvintpipe(dev, endpoint->desc.bEndpointAddress),
		     buf, INT_PKT_SIZE,
		     usb_int_complete, adapter,
		     endpoint->desc.bInterval);
		
=======
=======
	if (!urb) {
		goto err1;
	}
	intr->urb = urb;

	buf = kmalloc(INT_PKT_SIZE, GFP_KERNEL);
	if (!buf) {
		goto err2;
	}

	endpoint = &altsetting->endpoint[EP_INT-1];
>>>>>>> refs/remotes/origin/master

	// Fill the interrupt URB
	usb_fill_int_urb(urb, dev,
			 usb_rcvintpipe(dev, endpoint->desc.bEndpointAddress),
			 buf, INT_PKT_SIZE,
			 usb_int_complete, adapter,
			 endpoint->desc.bInterval);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
=======
	return 0;
err2:
	usb_free_urb(intr->urb);
	intr->urb = NULL;
err1:
	usb_free_urb(ctrl->urb);
	ctrl->urb = NULL;

	return -ENOMEM;
>>>>>>> refs/remotes/origin/master
}

/*
 * Release buffers and URBs for the interrupt and control
 * endpoint.
 */
void st5481_release_usb(struct st5481_adapter *adapter)
{
	struct st5481_intr *intr = &adapter->intr;
	struct st5481_ctrl *ctrl = &adapter->ctrl;

<<<<<<< HEAD
<<<<<<< HEAD
	DBG(1,"");
=======
	DBG(1, "");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	DBG(1, "");
>>>>>>> refs/remotes/origin/master

	// Stop and free Control and Interrupt URBs
	usb_kill_urb(ctrl->urb);
	kfree(ctrl->urb->transfer_buffer);
	usb_free_urb(ctrl->urb);
	ctrl->urb = NULL;

	usb_kill_urb(intr->urb);
	kfree(intr->urb->transfer_buffer);
	usb_free_urb(intr->urb);
	intr->urb = NULL;
}

/*
 *  Initialize the adapter.
 */
void st5481_start(struct st5481_adapter *adapter)
{
<<<<<<< HEAD
<<<<<<< HEAD
	static const u8 init_cmd_table[]={
		SET_DEFAULT,0,
		STT,0,
		SDA_MIN,0x0d,
		SDA_MAX,0x29,
		SDELAY_VALUE,0x14,
		GPIO_DIR,0x01,		
		GPIO_OUT,RED_LED,
//		FFCTRL_OUT_D,4,
//		FFCTRH_OUT_D,12,
		FFCTRL_OUT_B1,6,
		FFCTRH_OUT_B1,20,
		FFCTRL_OUT_B2,6,
		FFCTRH_OUT_B2,20,
		MPMSK,RXCI_INT+DEN_INT+DCOLL_INT,
		0
	};	
	struct st5481_intr *intr = &adapter->intr;
	int i = 0;
	u8 request,value;

	DBG(8,"");

	adapter->leds = RED_LED; 

	// Start receiving on the interrupt endpoint
	SUBMIT_URB(intr->urb, GFP_KERNEL); 
=======
=======
>>>>>>> refs/remotes/origin/master
	static const u8 init_cmd_table[] = {
		SET_DEFAULT, 0,
		STT, 0,
		SDA_MIN, 0x0d,
		SDA_MAX, 0x29,
		SDELAY_VALUE, 0x14,
		GPIO_DIR, 0x01,
		GPIO_OUT, RED_LED,
//		FFCTRL_OUT_D,4,
//		FFCTRH_OUT_D,12,
		FFCTRL_OUT_B1, 6,
		FFCTRH_OUT_B1, 20,
		FFCTRL_OUT_B2, 6,
		FFCTRH_OUT_B2, 20,
		MPMSK, RXCI_INT + DEN_INT + DCOLL_INT,
		0
	};
	struct st5481_intr *intr = &adapter->intr;
	int i = 0;
	u8 request, value;

	DBG(8, "");

	adapter->leds = RED_LED;

	// Start receiving on the interrupt endpoint
	SUBMIT_URB(intr->urb, GFP_KERNEL);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	while ((request = init_cmd_table[i++])) {
		value = init_cmd_table[i++];
		st5481_usb_device_ctrl_msg(adapter, request, value, NULL, NULL);
	}
	st5481_ph_command(adapter, ST5481_CMD_PUP);
}

/*
 * Reset the adapter to default values.
 */
void st5481_stop(struct st5481_adapter *adapter)
{
<<<<<<< HEAD
<<<<<<< HEAD
	DBG(8,"");
=======
	DBG(8, "");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	DBG(8, "");
>>>>>>> refs/remotes/origin/master

	st5481_usb_device_ctrl_msg(adapter, SET_DEFAULT, 0, NULL, NULL);
}

/* ======================================================================
 * isochronous USB  helpers
 */

static void
fill_isoc_urb(struct urb *urb, struct usb_device *dev,
<<<<<<< HEAD
<<<<<<< HEAD
	      unsigned int pipe, void *buf, int num_packets, 
	      int packet_size, usb_complete_t complete,
	      void *context) 
{
	int k;

	urb->dev=dev;
	urb->pipe=pipe;
	urb->interval = 1;
	urb->transfer_buffer=buf;
	urb->number_of_packets = num_packets;
	urb->transfer_buffer_length=num_packets*packet_size;
	urb->actual_length = 0;
	urb->complete=complete;
	urb->context=context;
	urb->transfer_flags=URB_ISO_ASAP;
=======
=======
>>>>>>> refs/remotes/origin/master
	      unsigned int pipe, void *buf, int num_packets,
	      int packet_size, usb_complete_t complete,
	      void *context)
{
	int k;

	urb->dev = dev;
	urb->pipe = pipe;
	urb->interval = 1;
	urb->transfer_buffer = buf;
	urb->number_of_packets = num_packets;
	urb->transfer_buffer_length = num_packets * packet_size;
	urb->actual_length = 0;
	urb->complete = complete;
	urb->context = context;
	urb->transfer_flags = URB_ISO_ASAP;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	for (k = 0; k < num_packets; k++) {
		urb->iso_frame_desc[k].offset = packet_size * k;
		urb->iso_frame_desc[k].length = packet_size;
		urb->iso_frame_desc[k].actual_length = 0;
	}
}

int
<<<<<<< HEAD
<<<<<<< HEAD
st5481_setup_isocpipes(struct urb* urb[2], struct usb_device *dev, 
			   unsigned int pipe, int num_packets,
			   int packet_size, int buf_size,
			   usb_complete_t complete, void *context)
=======
=======
>>>>>>> refs/remotes/origin/master
st5481_setup_isocpipes(struct urb *urb[2], struct usb_device *dev,
		       unsigned int pipe, int num_packets,
		       int packet_size, int buf_size,
		       usb_complete_t complete, void *context)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	int j, retval;
	unsigned char *buf;

	for (j = 0; j < 2; j++) {
		retval = -ENOMEM;
		urb[j] = usb_alloc_urb(num_packets, GFP_KERNEL);
		if (!urb[j])
			goto err;

		// Allocate memory for 2000bytes/sec (16Kb/s)
		buf = kmalloc(buf_size, GFP_KERNEL);
		if (!buf)
			goto err;
<<<<<<< HEAD
<<<<<<< HEAD
			
		// Fill the isochronous URB
		fill_isoc_urb(urb[j], dev, pipe, buf, 
=======

		// Fill the isochronous URB
		fill_isoc_urb(urb[j], dev, pipe, buf,
>>>>>>> refs/remotes/origin/cm-10.0
=======

		// Fill the isochronous URB
		fill_isoc_urb(urb[j], dev, pipe, buf,
>>>>>>> refs/remotes/origin/master
			      num_packets, packet_size, complete,
			      context);
	}
	return 0;

<<<<<<< HEAD
<<<<<<< HEAD
 err:
=======
err:
>>>>>>> refs/remotes/origin/cm-10.0
=======
err:
>>>>>>> refs/remotes/origin/master
	for (j = 0; j < 2; j++) {
		if (urb[j]) {
			kfree(urb[j]->transfer_buffer);
			urb[j]->transfer_buffer = NULL;
			usb_free_urb(urb[j]);
			urb[j] = NULL;
		}
	}
	return retval;
}

<<<<<<< HEAD
<<<<<<< HEAD
void st5481_release_isocpipes(struct urb* urb[2])
=======
void st5481_release_isocpipes(struct urb *urb[2])
>>>>>>> refs/remotes/origin/cm-10.0
=======
void st5481_release_isocpipes(struct urb *urb[2])
>>>>>>> refs/remotes/origin/master
{
	int j;

	for (j = 0; j < 2; j++) {
		usb_kill_urb(urb[j]);
		kfree(urb[j]->transfer_buffer);
		usb_free_urb(urb[j]);
		urb[j] = NULL;
	}
}

/*
 * Decode frames received on the B/D channel.
 * Note that this function will be called continuously
<<<<<<< HEAD
<<<<<<< HEAD
 * with 64Kbit/s / 16Kbit/s of data and hence it will be 
 * called 50 times per second with 20 ISOC descriptors. 
=======
 * with 64Kbit/s / 16Kbit/s of data and hence it will be
 * called 50 times per second with 20 ISOC descriptors.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * with 64Kbit/s / 16Kbit/s of data and hence it will be
 * called 50 times per second with 20 ISOC descriptors.
>>>>>>> refs/remotes/origin/master
 * Called at interrupt.
 */
static void usb_in_complete(struct urb *urb)
{
	struct st5481_in *in = urb->context;
	unsigned char *ptr;
	struct sk_buff *skb;
	int len, count, status;

	if (unlikely(urb->status < 0)) {
		switch (urb->status) {
<<<<<<< HEAD
<<<<<<< HEAD
			case -ENOENT:
			case -ESHUTDOWN:
			case -ECONNRESET:
				DBG(1,"urb killed status %d", urb->status);
				return; // Give up
			default: 
				WARNING("urb status %d",urb->status);
				break;
		}
	}

	DBG_ISO_PACKET(0x80,urb);
=======
=======
>>>>>>> refs/remotes/origin/master
		case -ENOENT:
		case -ESHUTDOWN:
		case -ECONNRESET:
			DBG(1, "urb killed status %d", urb->status);
			return; // Give up
		default:
			WARNING("urb status %d", urb->status);
			break;
		}
	}

	DBG_ISO_PACKET(0x80, urb);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	len = st5481_isoc_flatten(urb);
	ptr = urb->transfer_buffer;
	while (len > 0) {
		if (in->mode == L1_MODE_TRANS) {
			memcpy(in->rcvbuf, ptr, len);
			status = len;
			len = 0;
		} else {
			status = isdnhdlc_decode(&in->hdlc_state, ptr, len, &count,
<<<<<<< HEAD
<<<<<<< HEAD
				in->rcvbuf, in->bufsize);
			ptr += count;
			len -= count;
		}
		
		if (status > 0) {
			// Good frame received
			DBG(4,"count=%d",status);
=======
=======
>>>>>>> refs/remotes/origin/master
						 in->rcvbuf, in->bufsize);
			ptr += count;
			len -= count;
		}

		if (status > 0) {
			// Good frame received
			DBG(4, "count=%d", status);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			DBG_PACKET(0x400, in->rcvbuf, status);
			if (!(skb = dev_alloc_skb(status))) {
				WARNING("receive out of memory\n");
				break;
			}
			memcpy(skb_put(skb, status), in->rcvbuf, status);
			in->hisax_if->l1l2(in->hisax_if, PH_DATA | INDICATION, skb);
		} else if (status == -HDLC_CRC_ERROR) {
			INFO("CRC error");
		} else if (status == -HDLC_FRAMING_ERROR) {
			INFO("framing error");
		} else if (status == -HDLC_LENGTH_ERROR) {
			INFO("length error");
		}
	}

	// Prepare URB for next transfer
	urb->dev = in->adapter->usb_dev;
	urb->actual_length = 0;

	SUBMIT_URB(urb, GFP_ATOMIC);
}

int st5481_setup_in(struct st5481_in *in)
{
	struct usb_device *dev = in->adapter->usb_dev;
	int retval;

<<<<<<< HEAD
<<<<<<< HEAD
	DBG(4,"");
=======
	DBG(4, "");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	DBG(4, "");
>>>>>>> refs/remotes/origin/master

	in->rcvbuf = kmalloc(in->bufsize, GFP_KERNEL);
	retval = -ENOMEM;
	if (!in->rcvbuf)
		goto err;

<<<<<<< HEAD
<<<<<<< HEAD
	retval = st5481_setup_isocpipes(in->urb, dev, 
=======
	retval = st5481_setup_isocpipes(in->urb, dev,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	retval = st5481_setup_isocpipes(in->urb, dev,
>>>>>>> refs/remotes/origin/master
					usb_rcvisocpipe(dev, in->ep),
					in->num_packets,  in->packet_size,
					in->num_packets * in->packet_size,
					usb_in_complete, in);
	if (retval)
		goto err_free;
	return 0;

<<<<<<< HEAD
<<<<<<< HEAD
 err_free:
	kfree(in->rcvbuf);
 err:
=======
err_free:
	kfree(in->rcvbuf);
err:
>>>>>>> refs/remotes/origin/cm-10.0
=======
err_free:
	kfree(in->rcvbuf);
err:
>>>>>>> refs/remotes/origin/master
	return retval;
}

void st5481_release_in(struct st5481_in *in)
{
<<<<<<< HEAD
<<<<<<< HEAD
	DBG(2,"");
=======
	DBG(2, "");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	DBG(2, "");
>>>>>>> refs/remotes/origin/master

	st5481_release_isocpipes(in->urb);
}

/*
 * Make the transfer_buffer contiguous by
<<<<<<< HEAD
<<<<<<< HEAD
 * copying from the iso descriptors if necessary. 
 */
static int st5481_isoc_flatten(struct urb *urb)
{
	struct usb_iso_packet_descriptor *pipd,*pend;
	unsigned char *src,*dst;
	unsigned int len;
	
=======
=======
>>>>>>> refs/remotes/origin/master
 * copying from the iso descriptors if necessary.
 */
static int st5481_isoc_flatten(struct urb *urb)
{
	struct usb_iso_packet_descriptor *pipd, *pend;
	unsigned char *src, *dst;
	unsigned int len;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (urb->status < 0) {
		return urb->status;
	}
	for (pipd = &urb->iso_frame_desc[0],
		     pend = &urb->iso_frame_desc[urb->number_of_packets],
<<<<<<< HEAD
<<<<<<< HEAD
		     dst = urb->transfer_buffer; 
	     pipd < pend; 
	     pipd++) {
		
		if (pipd->status < 0) {
			return (pipd->status);
		}
	
		len = pipd->actual_length;
		pipd->actual_length = 0;
		src = urb->transfer_buffer+pipd->offset;
=======
=======
>>>>>>> refs/remotes/origin/master
		     dst = urb->transfer_buffer;
	     pipd < pend;
	     pipd++) {

		if (pipd->status < 0) {
			return (pipd->status);
		}

		len = pipd->actual_length;
		pipd->actual_length = 0;
		src = urb->transfer_buffer + pipd->offset;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

		if (src != dst) {
			// Need to copy since isoc buffers not full
			while (len--) {
				*dst++ = *src++;
<<<<<<< HEAD
<<<<<<< HEAD
			}			
=======
			}
>>>>>>> refs/remotes/origin/cm-10.0
=======
			}
>>>>>>> refs/remotes/origin/master
		} else {
			// No need to copy, just update destination buffer
			dst += len;
		}
	}
	// Return size of flattened buffer
	return (dst - (unsigned char *)urb->transfer_buffer);
}

static void st5481_start_rcv(void *context)
{
	struct st5481_in *in = context;
	struct st5481_adapter *adapter = in->adapter;

<<<<<<< HEAD
<<<<<<< HEAD
	DBG(4,"");
=======
	DBG(4, "");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	DBG(4, "");
>>>>>>> refs/remotes/origin/master

	in->urb[0]->dev = adapter->usb_dev;
	SUBMIT_URB(in->urb[0], GFP_KERNEL);

	in->urb[1]->dev = adapter->usb_dev;
	SUBMIT_URB(in->urb[1], GFP_KERNEL);
}

void st5481_in_mode(struct st5481_in *in, int mode)
{
	if (in->mode == mode)
		return;

	in->mode = mode;

	usb_unlink_urb(in->urb[0]);
	usb_unlink_urb(in->urb[1]);

	if (in->mode != L1_MODE_NULL) {
		if (in->mode != L1_MODE_TRANS) {
			u32 features = HDLC_BITREVERSE;

			if (in->mode == L1_MODE_HDLC_56K)
				features |= HDLC_56KBIT;
			isdnhdlc_rcv_init(&in->hdlc_state, features);
		}
		st5481_usb_pipe_reset(in->adapter, in->ep, NULL, NULL);
		st5481_usb_device_ctrl_msg(in->adapter, in->counter,
					   in->packet_size,
					   NULL, NULL);
		st5481_start_rcv(in);
	} else {
		st5481_usb_device_ctrl_msg(in->adapter, in->counter,
					   0, NULL, NULL);
	}
}
<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
