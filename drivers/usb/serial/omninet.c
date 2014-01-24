/*
 * USB ZyXEL omni.net LCD PLUS driver
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License version
 *	2 as published by the Free Software Foundation.
 *
 * See Documentation/usb/usb-serial.txt for more information on using this
 * driver
 *
 * Please report both successes and troubles to the author at omninet@kroah.com
<<<<<<< HEAD
<<<<<<< HEAD
 *
 * (05/30/2001) gkh
 *	switched from using spinlock to a semaphore, which fixes lots of
 *	problems.
 *
 * (04/08/2001) gb
 *	Identify version on module load.
 *
 * (11/01/2000) Adam J. Richter
 *	usb_device_id table support
 *
 * (10/05/2000) gkh
 *	Fixed bug with urb->dev not being set properly, now that the usb
 *	core needs it.
 *
 * (08/28/2000) gkh
 *	Added locks for SMP safeness.
 *	Fixed MOD_INC and MOD_DEC logic and the ability to open a port more
 *	than once.
 *	Fixed potential race in omninet_write_bulk_callback
 *
 * (07/19/2000) gkh
 *	Added module_init and module_exit functions to handle the fact that this
 *	driver is a loadable module now.
 *
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 */

#include <linux/kernel.h>
#include <linux/errno.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> refs/remotes/origin/master
#include <linux/slab.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>
#include <linux/tty_flip.h>
#include <linux/module.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/spinlock.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <linux/uaccess.h>
#include <linux/usb.h>
#include <linux/usb/serial.h>

<<<<<<< HEAD
<<<<<<< HEAD
static int debug;
=======
static bool debug;
>>>>>>> refs/remotes/origin/cm-10.0

/*
 * Version Information
 */
#define DRIVER_VERSION "v1.1"
=======
>>>>>>> refs/remotes/origin/master
#define DRIVER_AUTHOR "Alessandro Zummo"
#define DRIVER_DESC "USB ZyXEL omni.net LCD PLUS Driver"

#define ZYXEL_VENDOR_ID		0x0586
#define ZYXEL_OMNINET_ID	0x1000
/* This one seems to be a re-branded ZyXEL device */
#define BT_IGNITIONPRO_ID	0x2000

/* function prototypes */
static int  omninet_open(struct tty_struct *tty, struct usb_serial_port *port);
<<<<<<< HEAD
static void omninet_close(struct usb_serial_port *port);
static void omninet_read_bulk_callback(struct urb *urb);
=======
static void omninet_process_read_urb(struct urb *urb);
>>>>>>> refs/remotes/origin/master
static void omninet_write_bulk_callback(struct urb *urb);
static int  omninet_write(struct tty_struct *tty, struct usb_serial_port *port,
				const unsigned char *buf, int count);
static int  omninet_write_room(struct tty_struct *tty);
static void omninet_disconnect(struct usb_serial *serial);
<<<<<<< HEAD
static void omninet_release(struct usb_serial *serial);
static int omninet_attach(struct usb_serial *serial);
=======
static int omninet_port_probe(struct usb_serial_port *port);
static int omninet_port_remove(struct usb_serial_port *port);
>>>>>>> refs/remotes/origin/master

static const struct usb_device_id id_table[] = {
	{ USB_DEVICE(ZYXEL_VENDOR_ID, ZYXEL_OMNINET_ID) },
	{ USB_DEVICE(ZYXEL_VENDOR_ID, BT_IGNITIONPRO_ID) },
	{ }						/* Terminating entry */
};
<<<<<<< HEAD

MODULE_DEVICE_TABLE(usb, id_table);

static struct usb_driver omninet_driver = {
	.name =		"omninet",
	.probe =	usb_serial_probe,
	.disconnect =	usb_serial_disconnect,
	.id_table =	id_table,
<<<<<<< HEAD
	.no_dynamic_id = 	1,
=======
>>>>>>> refs/remotes/origin/cm-10.0
};


=======
MODULE_DEVICE_TABLE(usb, id_table);

>>>>>>> refs/remotes/origin/master
static struct usb_serial_driver zyxel_omninet_device = {
	.driver = {
		.owner =	THIS_MODULE,
		.name =		"omninet",
	},
	.description =		"ZyXEL - omni.net lcd plus usb",
<<<<<<< HEAD
<<<<<<< HEAD
	.usb_driver =		&omninet_driver,
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.id_table =		id_table,
	.num_ports =		1,
	.attach =		omninet_attach,
	.open =			omninet_open,
	.close =		omninet_close,
	.write =		omninet_write,
	.write_room =		omninet_write_room,
	.read_bulk_callback =	omninet_read_bulk_callback,
	.write_bulk_callback =	omninet_write_bulk_callback,
	.disconnect =		omninet_disconnect,
	.release =		omninet_release,
};

<<<<<<< HEAD
=======
=======
	.id_table =		id_table,
	.num_ports =		1,
	.port_probe =		omninet_port_probe,
	.port_remove =		omninet_port_remove,
	.open =			omninet_open,
	.write =		omninet_write,
	.write_room =		omninet_write_room,
	.write_bulk_callback =	omninet_write_bulk_callback,
	.process_read_urb =	omninet_process_read_urb,
	.disconnect =		omninet_disconnect,
};

>>>>>>> refs/remotes/origin/master
static struct usb_serial_driver * const serial_drivers[] = {
	&zyxel_omninet_device, NULL
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

/* The protocol.
 *
 * The omni.net always exchange 64 bytes of data with the host. The first
 * four bytes are the control header, you can see it in the above structure.
=======

/*
 * The protocol.
 *
 * The omni.net always exchange 64 bytes of data with the host. The first
 * four bytes are the control header.
>>>>>>> refs/remotes/origin/master
 *
 * oh_seq is a sequence number. Don't know if/how it's used.
 * oh_len is the length of the data bytes in the packet.
 * oh_xxx Bit-mapped, related to handshaking and status info.
<<<<<<< HEAD
 *	I normally set it to 0x03 in trasmitted frames.
=======
 *	I normally set it to 0x03 in transmitted frames.
>>>>>>> refs/remotes/origin/master
 *	7: Active when the TA is in a CONNECTed state.
 *	6: unknown
 *	5: handshaking, unknown
 *	4: handshaking, unknown
 *	3: unknown, usually 0
 *	2: unknown, usually 0
<<<<<<< HEAD
 *	1: handshaking, unknown, usually set to 1 in trasmitted frames
 *	0: handshaking, unknown, usually set to 1 in trasmitted frames
 * oh_pad Probably a pad byte.
 *
 * After the header you will find data bytes if oh_len was greater than zero.
 *
 */

=======
 *	1: handshaking, unknown, usually set to 1 in transmitted frames
 *	0: handshaking, unknown, usually set to 1 in transmitted frames
 * oh_pad Probably a pad byte.
 *
 * After the header you will find data bytes if oh_len was greater than zero.
 */
>>>>>>> refs/remotes/origin/master
struct omninet_header {
	__u8	oh_seq;
	__u8	oh_len;
	__u8	oh_xxx;
	__u8	oh_pad;
};

struct omninet_data {
	__u8	od_outseq;	/* Sequence number for bulk_out URBs */
};

<<<<<<< HEAD
static int omninet_attach(struct usb_serial *serial)
{
	struct omninet_data *od;
	struct usb_serial_port *port = serial->port[0];

	od = kmalloc(sizeof(struct omninet_data), GFP_KERNEL);
	if (!od) {
		dev_err(&port->dev, "%s- kmalloc(%Zd) failed.\n",
			__func__, sizeof(struct omninet_data));
		return -ENOMEM;
	}
	usb_set_serial_port_data(port, od);
=======
static int omninet_port_probe(struct usb_serial_port *port)
{
	struct omninet_data *od;

	od = kzalloc(sizeof(*od), GFP_KERNEL);
	if (!od)
		return -ENOMEM;

	usb_set_serial_port_data(port, od);

	return 0;
}

static int omninet_port_remove(struct usb_serial_port *port)
{
	struct omninet_data *od;

	od = usb_get_serial_port_data(port);
	kfree(od);

>>>>>>> refs/remotes/origin/master
	return 0;
}

static int omninet_open(struct tty_struct *tty, struct usb_serial_port *port)
{
	struct usb_serial	*serial = port->serial;
	struct usb_serial_port	*wport;
<<<<<<< HEAD
	int			result = 0;

	dbg("%s - port %d", __func__, port->number);
=======
>>>>>>> refs/remotes/origin/master

	wport = serial->port[1];
	tty_port_tty_set(&wport->port, tty);

<<<<<<< HEAD
	/* Start reading from the device */
<<<<<<< HEAD
	usb_fill_bulk_urb(port->read_urb, serial->dev,
			usb_rcvbulkpipe(serial->dev,
				port->bulk_in_endpointAddress),
			port->read_urb->transfer_buffer,
			port->read_urb->transfer_buffer_length,
			omninet_read_bulk_callback, port);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	result = usb_submit_urb(port->read_urb, GFP_KERNEL);
	if (result)
		dev_err(&port->dev,
			"%s - failed submitting read urb, error %d\n",
			__func__, result);
	return result;
}

static void omninet_close(struct usb_serial_port *port)
{
	dbg("%s - port %d", __func__, port->number);
	usb_kill_urb(port->read_urb);
}


#define OMNINET_DATAOFFSET	0x04
#define OMNINET_HEADERLEN	sizeof(struct omninet_header)
#define OMNINET_BULKOUTSIZE 	(64 - OMNINET_HEADERLEN)

static void omninet_read_bulk_callback(struct urb *urb)
{
	struct usb_serial_port 	*port 	= urb->context;
	unsigned char 		*data 	= urb->transfer_buffer;
	struct omninet_header 	*header = (struct omninet_header *) &data[0];
	int status = urb->status;
	int result;
	int i;

	dbg("%s - port %d", __func__, port->number);

	if (status) {
		dbg("%s - nonzero read bulk status received: %d",
		    __func__, status);
		return;
	}

	if (debug && header->oh_xxx != 0x30) {
		if (urb->actual_length) {
			printk(KERN_DEBUG "%s: omninet_read %d: ",
			       __FILE__, header->oh_len);
			for (i = 0; i < (header->oh_len +
						OMNINET_HEADERLEN); i++)
				printk("%.2x ", data[i]);
			printk("\n");
		}
	}

	if (urb->actual_length && header->oh_len) {
		struct tty_struct *tty = tty_port_tty_get(&port->port);
		tty_insert_flip_string(tty, data + OMNINET_DATAOFFSET,
							header->oh_len);
		tty_flip_buffer_push(tty);
		tty_kref_put(tty);
	}

	/* Continue trying to always read  */
<<<<<<< HEAD
	usb_fill_bulk_urb(urb, port->serial->dev,
			usb_rcvbulkpipe(port->serial->dev,
					port->bulk_in_endpointAddress),
			urb->transfer_buffer, urb->transfer_buffer_length,
			omninet_read_bulk_callback, port);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	result = usb_submit_urb(urb, GFP_ATOMIC);
	if (result)
		dev_err(&port->dev,
			"%s - failed resubmitting read urb, error %d\n",
			__func__, result);
=======
	return usb_serial_generic_open(tty, port);
}

#define OMNINET_HEADERLEN	4
#define OMNINET_BULKOUTSIZE	64
#define OMNINET_PAYLOADSIZE	(OMNINET_BULKOUTSIZE - OMNINET_HEADERLEN)

static void omninet_process_read_urb(struct urb *urb)
{
	struct usb_serial_port *port = urb->context;
	const struct omninet_header *hdr = urb->transfer_buffer;
	const unsigned char *data;
	size_t data_len;

	if (urb->actual_length <= OMNINET_HEADERLEN || !hdr->oh_len)
		return;

	data = (char *)urb->transfer_buffer + OMNINET_HEADERLEN;
	data_len = min_t(size_t, urb->actual_length - OMNINET_HEADERLEN,
								hdr->oh_len);
	tty_insert_flip_string(&port->port, data, data_len);
	tty_flip_buffer_push(&port->port);
>>>>>>> refs/remotes/origin/master
}

static int omninet_write(struct tty_struct *tty, struct usb_serial_port *port,
					const unsigned char *buf, int count)
{
	struct usb_serial *serial = port->serial;
	struct usb_serial_port *wport = serial->port[1];

	struct omninet_data *od = usb_get_serial_port_data(port);
	struct omninet_header *header = (struct omninet_header *)
					wport->write_urb->transfer_buffer;

	int			result;

<<<<<<< HEAD
	dbg("%s - port %d", __func__, port->number);

	if (count == 0) {
		dbg("%s - write request of 0 bytes", __func__);
		return 0;
	}

<<<<<<< HEAD
	spin_lock_bh(&wport->lock);
	if (wport->write_urb_busy) {
		spin_unlock_bh(&wport->lock);
		dbg("%s - already writing", __func__);
		return 0;
	}
	wport->write_urb_busy = 1;
	spin_unlock_bh(&wport->lock);
=======
	if (!test_and_clear_bit(0, &port->write_urbs_free)) {
		dbg("%s - already writing", __func__);
		return 0;
	}
>>>>>>> refs/remotes/origin/cm-10.0

	count = (count > OMNINET_BULKOUTSIZE) ? OMNINET_BULKOUTSIZE : count;

	memcpy(wport->write_urb->transfer_buffer + OMNINET_DATAOFFSET,
								buf, count);

	usb_serial_debug_data(debug, &port->dev, __func__, count,
					wport->write_urb->transfer_buffer);
=======
	if (count == 0) {
		dev_dbg(&port->dev, "%s - write request of 0 bytes\n", __func__);
		return 0;
	}

	if (!test_and_clear_bit(0, &port->write_urbs_free)) {
		dev_dbg(&port->dev, "%s - already writing\n", __func__);
		return 0;
	}

	count = (count > OMNINET_PAYLOADSIZE) ? OMNINET_PAYLOADSIZE : count;

	memcpy(wport->write_urb->transfer_buffer + OMNINET_HEADERLEN,
								buf, count);

	usb_serial_debug_data(&port->dev, __func__, count,
			      wport->write_urb->transfer_buffer);
>>>>>>> refs/remotes/origin/master

	header->oh_seq 	= od->od_outseq++;
	header->oh_len 	= count;
	header->oh_xxx  = 0x03;
	header->oh_pad 	= 0x00;

	/* send the data out the bulk port, always 64 bytes */
<<<<<<< HEAD
	wport->write_urb->transfer_buffer_length = 64;

<<<<<<< HEAD
	wport->write_urb->dev = serial->dev;
	result = usb_submit_urb(wport->write_urb, GFP_ATOMIC);
	if (result) {
		wport->write_urb_busy = 0;
		dev_err(&port->dev,
=======
=======
	wport->write_urb->transfer_buffer_length = OMNINET_BULKOUTSIZE;

>>>>>>> refs/remotes/origin/master
	result = usb_submit_urb(wport->write_urb, GFP_ATOMIC);
	if (result) {
		set_bit(0, &wport->write_urbs_free);
		dev_err_console(port,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			"%s - failed submitting write urb, error %d\n",
			__func__, result);
	} else
		result = count;

	return result;
}


static int omninet_write_room(struct tty_struct *tty)
{
	struct usb_serial_port *port = tty->driver_data;
	struct usb_serial 	*serial = port->serial;
	struct usb_serial_port 	*wport 	= serial->port[1];

	int room = 0; /* Default: no room */

<<<<<<< HEAD
<<<<<<< HEAD
	/* FIXME: no consistent locking for write_urb_busy */
	if (!wport->write_urb_busy)
=======
	if (test_bit(0, &wport->write_urbs_free))
>>>>>>> refs/remotes/origin/cm-10.0
		room = wport->bulk_out_size - OMNINET_HEADERLEN;

	dbg("%s - returns %d", __func__, room);
=======
	if (test_bit(0, &wport->write_urbs_free))
		room = wport->bulk_out_size - OMNINET_HEADERLEN;

	dev_dbg(&port->dev, "%s - returns %d\n", __func__, room);
>>>>>>> refs/remotes/origin/master

	return room;
}

static void omninet_write_bulk_callback(struct urb *urb)
{
/*	struct omninet_header	*header = (struct omninet_header  *)
						urb->transfer_buffer; */
	struct usb_serial_port 	*port   =  urb->context;
	int status = urb->status;

<<<<<<< HEAD
	dbg("%s - port %0x", __func__, port->number);

<<<<<<< HEAD
	port->write_urb_busy = 0;
=======
	set_bit(0, &port->write_urbs_free);
>>>>>>> refs/remotes/origin/cm-10.0
	if (status) {
		dbg("%s - nonzero write bulk status received: %d",
		    __func__, status);
=======
	set_bit(0, &port->write_urbs_free);
	if (status) {
		dev_dbg(&port->dev, "%s - nonzero write bulk status received: %d\n",
			__func__, status);
>>>>>>> refs/remotes/origin/master
		return;
	}

	usb_serial_port_softint(port);
}


static void omninet_disconnect(struct usb_serial *serial)
{
	struct usb_serial_port *wport = serial->port[1];

<<<<<<< HEAD
	dbg("%s", __func__);

	usb_kill_urb(wport->write_urb);
}


static void omninet_release(struct usb_serial *serial)
{
	struct usb_serial_port *port = serial->port[0];

	dbg("%s", __func__);

	kfree(usb_get_serial_port_data(port));
}

<<<<<<< HEAD

static int __init omninet_init(void)
{
	int retval;
	retval = usb_serial_register(&zyxel_omninet_device);
	if (retval)
		goto failed_usb_serial_register;
	retval = usb_register(&omninet_driver);
	if (retval)
		goto failed_usb_register;
	printk(KERN_INFO KBUILD_MODNAME ": " DRIVER_VERSION ":"
	       DRIVER_DESC "\n");
	return 0;
failed_usb_register:
	usb_serial_deregister(&zyxel_omninet_device);
failed_usb_serial_register:
	return retval;
}


static void __exit omninet_exit(void)
{
	usb_deregister(&omninet_driver);
	usb_serial_deregister(&zyxel_omninet_device);
}


module_init(omninet_init);
module_exit(omninet_exit);
=======
module_usb_serial_driver(omninet_driver, serial_drivers);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	usb_kill_urb(wport->write_urb);
}

module_usb_serial_driver(serial_drivers, id_table);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
<<<<<<< HEAD

module_param(debug, bool, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(debug, "Debug enabled or not");
=======
>>>>>>> refs/remotes/origin/master
