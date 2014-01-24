/*
 *  REINER SCT cyberJack pinpad/e-com USB Chipcard Reader Driver
 *
 *  Copyright (C) 2001  REINER SCT
 *  Author: Matthias Bruestle
 *
 *  Contact: support@reiner-sct.com (see MAINTAINERS)
 *
 *  This program is largely derived from work by the linux-usb group
 *  and associated source files.  Please see the usb/serial files for
 *  individual credits and copyrights.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Thanks to Greg Kroah-Hartman (greg@kroah.com) for his help and
 *  patience.
 *
 *  In case of problems, please write to the contact e-mail address
 *  mentioned above.
 *
 *  Please note that later models of the cyberjack reader family are
 *  supported by a libusb-based userspace device driver.
 *
 *  Homepage: http://www.reiner-sct.de/support/treiber_cyberjack.php#linux
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
#include <linux/spinlock.h>
#include <linux/uaccess.h>
#include <linux/usb.h>
#include <linux/usb/serial.h>

#define CYBERJACK_LOCAL_BUF_SIZE 32

<<<<<<< HEAD
<<<<<<< HEAD
static int debug;
=======
static bool debug;
>>>>>>> refs/remotes/origin/cm-10.0

/*
 * Version Information
 */
#define DRIVER_VERSION "v1.01"
=======
>>>>>>> refs/remotes/origin/master
#define DRIVER_AUTHOR "Matthias Bruestle"
#define DRIVER_DESC "REINER SCT cyberJack pinpad/e-com USB Chipcard Reader Driver"


#define CYBERJACK_VENDOR_ID	0x0C4B
#define CYBERJACK_PRODUCT_ID	0x0100

/* Function prototypes */
<<<<<<< HEAD
static int cyberjack_startup(struct usb_serial *serial);
static void cyberjack_disconnect(struct usb_serial *serial);
static void cyberjack_release(struct usb_serial *serial);
=======
static int cyberjack_port_probe(struct usb_serial_port *port);
static int cyberjack_port_remove(struct usb_serial_port *port);
>>>>>>> refs/remotes/origin/master
static int  cyberjack_open(struct tty_struct *tty,
	struct usb_serial_port *port);
static void cyberjack_close(struct usb_serial_port *port);
static int cyberjack_write(struct tty_struct *tty,
	struct usb_serial_port *port, const unsigned char *buf, int count);
static int cyberjack_write_room(struct tty_struct *tty);
static void cyberjack_read_int_callback(struct urb *urb);
static void cyberjack_read_bulk_callback(struct urb *urb);
static void cyberjack_write_bulk_callback(struct urb *urb);

static const struct usb_device_id id_table[] = {
	{ USB_DEVICE(CYBERJACK_VENDOR_ID, CYBERJACK_PRODUCT_ID) },
	{ }			/* Terminating entry */
};

MODULE_DEVICE_TABLE(usb, id_table);

<<<<<<< HEAD
static struct usb_driver cyberjack_driver = {
	.name =		"cyberjack",
	.probe =	usb_serial_probe,
	.disconnect =	usb_serial_disconnect,
	.id_table =	id_table,
<<<<<<< HEAD
	.no_dynamic_id = 	1,
=======
>>>>>>> refs/remotes/origin/cm-10.0
};

=======
>>>>>>> refs/remotes/origin/master
static struct usb_serial_driver cyberjack_device = {
	.driver = {
		.owner =	THIS_MODULE,
		.name =		"cyberjack",
	},
	.description =		"Reiner SCT Cyberjack USB card reader",
<<<<<<< HEAD
<<<<<<< HEAD
	.usb_driver = 		&cyberjack_driver,
=======
>>>>>>> refs/remotes/origin/cm-10.0
	.id_table =		id_table,
	.num_ports =		1,
	.attach =		cyberjack_startup,
	.disconnect =		cyberjack_disconnect,
	.release =		cyberjack_release,
=======
	.id_table =		id_table,
	.num_ports =		1,
	.port_probe =		cyberjack_port_probe,
	.port_remove =		cyberjack_port_remove,
>>>>>>> refs/remotes/origin/master
	.open =			cyberjack_open,
	.close =		cyberjack_close,
	.write =		cyberjack_write,
	.write_room =		cyberjack_write_room,
	.read_int_callback =	cyberjack_read_int_callback,
	.read_bulk_callback =	cyberjack_read_bulk_callback,
	.write_bulk_callback =	cyberjack_write_bulk_callback,
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static struct usb_serial_driver * const serial_drivers[] = {
	&cyberjack_device, NULL
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
struct cyberjack_private {
	spinlock_t	lock;		/* Lock for SMP */
	short		rdtodo;		/* Bytes still to read */
	unsigned char	wrbuf[5*64];	/* Buffer for collecting data to write */
	short		wrfilled;	/* Overall data size we already got */
	short		wrsent;		/* Data already sent */
};

<<<<<<< HEAD
/* do some startup allocations not currently performed by usb_serial_probe() */
static int cyberjack_startup(struct usb_serial *serial)
{
	struct cyberjack_private *priv;
	int i;

	dbg("%s", __func__);

	/* allocate the private data structure */
=======
static int cyberjack_port_probe(struct usb_serial_port *port)
{
	struct cyberjack_private *priv;
	int result;

>>>>>>> refs/remotes/origin/master
	priv = kmalloc(sizeof(struct cyberjack_private), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

<<<<<<< HEAD
	/* set initial values */
=======
>>>>>>> refs/remotes/origin/master
	spin_lock_init(&priv->lock);
	priv->rdtodo = 0;
	priv->wrfilled = 0;
	priv->wrsent = 0;
<<<<<<< HEAD
	usb_set_serial_port_data(serial->port[0], priv);

	init_waitqueue_head(&serial->port[0]->write_wait);

	for (i = 0; i < serial->num_ports; ++i) {
		int result;
<<<<<<< HEAD
		serial->port[i]->interrupt_in_urb->dev = serial->dev;
=======
>>>>>>> refs/remotes/origin/cm-10.0
		result = usb_submit_urb(serial->port[i]->interrupt_in_urb,
					GFP_KERNEL);
		if (result)
			dev_err(&serial->dev->dev,
				"usb_submit_urb(read int) failed\n");
		dbg("%s - usb_submit_urb(int urb)", __func__);
	}

	return 0;
}

static void cyberjack_disconnect(struct usb_serial *serial)
{
	int i;

	dbg("%s", __func__);

	for (i = 0; i < serial->num_ports; ++i)
		usb_kill_urb(serial->port[i]->interrupt_in_urb);
}

static void cyberjack_release(struct usb_serial *serial)
{
	int i;

	dbg("%s", __func__);

	for (i = 0; i < serial->num_ports; ++i) {
		/* My special items, the standard routines free my urbs */
		kfree(usb_get_serial_port_data(serial->port[i]));
	}
=======

	usb_set_serial_port_data(port, priv);

	result = usb_submit_urb(port->interrupt_in_urb, GFP_KERNEL);
	if (result)
		dev_err(&port->dev, "usb_submit_urb(read int) failed\n");

	return 0;
}

static int cyberjack_port_remove(struct usb_serial_port *port)
{
	struct cyberjack_private *priv;

	usb_kill_urb(port->interrupt_in_urb);

	priv = usb_get_serial_port_data(port);
	kfree(priv);

	return 0;
>>>>>>> refs/remotes/origin/master
}

static int  cyberjack_open(struct tty_struct *tty,
					struct usb_serial_port *port)
{
	struct cyberjack_private *priv;
	unsigned long flags;
	int result = 0;

<<<<<<< HEAD
	dbg("%s - port %d", __func__, port->number);

	dbg("%s - usb_clear_halt", __func__);
=======
	dev_dbg(&port->dev, "%s - usb_clear_halt\n", __func__);
>>>>>>> refs/remotes/origin/master
	usb_clear_halt(port->serial->dev, port->write_urb->pipe);

	priv = usb_get_serial_port_data(port);
	spin_lock_irqsave(&priv->lock, flags);
	priv->rdtodo = 0;
	priv->wrfilled = 0;
	priv->wrsent = 0;
	spin_unlock_irqrestore(&priv->lock, flags);

	return result;
}

static void cyberjack_close(struct usb_serial_port *port)
{
<<<<<<< HEAD
	dbg("%s - port %d", __func__, port->number);

	if (port->serial->dev) {
		/* shutdown any bulk reads that might be going on */
		usb_kill_urb(port->write_urb);
		usb_kill_urb(port->read_urb);
	}
=======
	usb_kill_urb(port->write_urb);
	usb_kill_urb(port->read_urb);
>>>>>>> refs/remotes/origin/master
}

static int cyberjack_write(struct tty_struct *tty,
	struct usb_serial_port *port, const unsigned char *buf, int count)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct usb_serial *serial = port->serial;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct device *dev = &port->dev;
>>>>>>> refs/remotes/origin/master
	struct cyberjack_private *priv = usb_get_serial_port_data(port);
	unsigned long flags;
	int result;
	int wrexpected;

<<<<<<< HEAD
	dbg("%s - port %d", __func__, port->number);

	if (count == 0) {
		dbg("%s - write request of 0 bytes", __func__);
		return 0;
	}

<<<<<<< HEAD
	spin_lock_bh(&port->lock);
	if (port->write_urb_busy) {
		spin_unlock_bh(&port->lock);
		dbg("%s - already writing", __func__);
		return 0;
	}
	port->write_urb_busy = 1;
	spin_unlock_bh(&port->lock);
=======
	if (!test_and_clear_bit(0, &port->write_urbs_free)) {
		dbg("%s - already writing", __func__);
		return 0;
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (count == 0) {
		dev_dbg(dev, "%s - write request of 0 bytes\n", __func__);
		return 0;
	}

	if (!test_and_clear_bit(0, &port->write_urbs_free)) {
		dev_dbg(dev, "%s - already writing\n", __func__);
		return 0;
	}
>>>>>>> refs/remotes/origin/master

	spin_lock_irqsave(&priv->lock, flags);

	if (count+priv->wrfilled > sizeof(priv->wrbuf)) {
		/* To much data for buffer. Reset buffer. */
		priv->wrfilled = 0;
<<<<<<< HEAD
<<<<<<< HEAD
		port->write_urb_busy = 0;
		spin_unlock_irqrestore(&priv->lock, flags);
=======
		spin_unlock_irqrestore(&priv->lock, flags);
		set_bit(0, &port->write_urbs_free);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		spin_unlock_irqrestore(&priv->lock, flags);
		set_bit(0, &port->write_urbs_free);
>>>>>>> refs/remotes/origin/master
		return 0;
	}

	/* Copy data */
	memcpy(priv->wrbuf + priv->wrfilled, buf, count);

<<<<<<< HEAD
	usb_serial_debug_data(debug, &port->dev, __func__, count,
		priv->wrbuf + priv->wrfilled);
=======
	usb_serial_debug_data(dev, __func__, count, priv->wrbuf + priv->wrfilled);
>>>>>>> refs/remotes/origin/master
	priv->wrfilled += count;

	if (priv->wrfilled >= 3) {
		wrexpected = ((int)priv->wrbuf[2]<<8)+priv->wrbuf[1]+3;
<<<<<<< HEAD
		dbg("%s - expected data: %d", __func__, wrexpected);
=======
		dev_dbg(dev, "%s - expected data: %d\n", __func__, wrexpected);
>>>>>>> refs/remotes/origin/master
	} else
		wrexpected = sizeof(priv->wrbuf);

	if (priv->wrfilled >= wrexpected) {
		/* We have enough data to begin transmission */
		int length;

<<<<<<< HEAD
		dbg("%s - transmitting data (frame 1)", __func__);
=======
		dev_dbg(dev, "%s - transmitting data (frame 1)\n", __func__);
>>>>>>> refs/remotes/origin/master
		length = (wrexpected > port->bulk_out_size) ?
					port->bulk_out_size : wrexpected;

		memcpy(port->write_urb->transfer_buffer, priv->wrbuf, length);
		priv->wrsent = length;

		/* set up our urb */
<<<<<<< HEAD
<<<<<<< HEAD
		usb_fill_bulk_urb(port->write_urb, serial->dev,
			      usb_sndbulkpipe(serial->dev, port->bulk_out_endpointAddress),
			      port->write_urb->transfer_buffer, length,
			      ((serial->type->write_bulk_callback) ?
			       serial->type->write_bulk_callback :
			       cyberjack_write_bulk_callback),
			      port);
=======
		port->write_urb->transfer_buffer_length = length;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		port->write_urb->transfer_buffer_length = length;
>>>>>>> refs/remotes/origin/master

		/* send the data out the bulk port */
		result = usb_submit_urb(port->write_urb, GFP_ATOMIC);
		if (result) {
			dev_err(&port->dev,
				"%s - failed submitting write urb, error %d",
				__func__, result);
			/* Throw away data. No better idea what to do with it. */
			priv->wrfilled = 0;
			priv->wrsent = 0;
			spin_unlock_irqrestore(&priv->lock, flags);
<<<<<<< HEAD
<<<<<<< HEAD
			port->write_urb_busy = 0;
=======
			set_bit(0, &port->write_urbs_free);
>>>>>>> refs/remotes/origin/cm-10.0
			return 0;
		}

		dbg("%s - priv->wrsent=%d", __func__, priv->wrsent);
		dbg("%s - priv->wrfilled=%d", __func__, priv->wrfilled);

		if (priv->wrsent >= priv->wrfilled) {
			dbg("%s - buffer cleaned", __func__);
=======
			set_bit(0, &port->write_urbs_free);
			return 0;
		}

		dev_dbg(dev, "%s - priv->wrsent=%d\n", __func__, priv->wrsent);
		dev_dbg(dev, "%s - priv->wrfilled=%d\n", __func__, priv->wrfilled);

		if (priv->wrsent >= priv->wrfilled) {
			dev_dbg(dev, "%s - buffer cleaned\n", __func__);
>>>>>>> refs/remotes/origin/master
			memset(priv->wrbuf, 0, sizeof(priv->wrbuf));
			priv->wrfilled = 0;
			priv->wrsent = 0;
		}
	}

	spin_unlock_irqrestore(&priv->lock, flags);

	return count;
}

static int cyberjack_write_room(struct tty_struct *tty)
{
	/* FIXME: .... */
	return CYBERJACK_LOCAL_BUF_SIZE;
}

static void cyberjack_read_int_callback(struct urb *urb)
{
	struct usb_serial_port *port = urb->context;
	struct cyberjack_private *priv = usb_get_serial_port_data(port);
<<<<<<< HEAD
=======
	struct device *dev = &port->dev;
>>>>>>> refs/remotes/origin/master
	unsigned char *data = urb->transfer_buffer;
	int status = urb->status;
	int result;

<<<<<<< HEAD
	dbg("%s - port %d", __func__, port->number);

=======
>>>>>>> refs/remotes/origin/master
	/* the urb might have been killed. */
	if (status)
		return;

<<<<<<< HEAD
	usb_serial_debug_data(debug, &port->dev, __func__,
						urb->actual_length, data);
=======
	usb_serial_debug_data(dev, __func__, urb->actual_length, data);
>>>>>>> refs/remotes/origin/master

	/* React only to interrupts signaling a bulk_in transfer */
	if (urb->actual_length == 4 && data[0] == 0x01) {
		short old_rdtodo;

		/* This is a announcement of coming bulk_ins. */
		unsigned short size = ((unsigned short)data[3]<<8)+data[2]+3;

		spin_lock(&priv->lock);

		old_rdtodo = priv->rdtodo;

<<<<<<< HEAD
		if (old_rdtodo + size < old_rdtodo) {
			dbg("To many bulk_in urbs to do.");
=======
		if (old_rdtodo > SHRT_MAX - size) {
			dev_dbg(dev, "To many bulk_in urbs to do.\n");
>>>>>>> refs/remotes/origin/master
			spin_unlock(&priv->lock);
			goto resubmit;
		}

<<<<<<< HEAD
		/* "+=" is probably more fault tollerant than "=" */
		priv->rdtodo += size;

		dbg("%s - rdtodo: %d", __func__, priv->rdtodo);
=======
		/* "+=" is probably more fault tolerant than "=" */
		priv->rdtodo += size;

		dev_dbg(dev, "%s - rdtodo: %d\n", __func__, priv->rdtodo);
>>>>>>> refs/remotes/origin/master

		spin_unlock(&priv->lock);

		if (!old_rdtodo) {
<<<<<<< HEAD
<<<<<<< HEAD
			port->read_urb->dev = port->serial->dev;
=======
>>>>>>> refs/remotes/origin/cm-10.0
			result = usb_submit_urb(port->read_urb, GFP_ATOMIC);
			if (result)
				dev_err(&port->dev, "%s - failed resubmitting "
					"read urb, error %d\n",
					__func__, result);
			dbg("%s - usb_submit_urb(read urb)", __func__);
=======
			result = usb_submit_urb(port->read_urb, GFP_ATOMIC);
			if (result)
				dev_err(dev, "%s - failed resubmitting read urb, error %d\n",
					__func__, result);
			dev_dbg(dev, "%s - usb_submit_urb(read urb)\n", __func__);
>>>>>>> refs/remotes/origin/master
		}
	}

resubmit:
<<<<<<< HEAD
<<<<<<< HEAD
	port->interrupt_in_urb->dev = port->serial->dev;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	result = usb_submit_urb(port->interrupt_in_urb, GFP_ATOMIC);
	if (result)
		dev_err(&port->dev, "usb_submit_urb(read int) failed\n");
	dbg("%s - usb_submit_urb(int urb)", __func__);
=======
	result = usb_submit_urb(port->interrupt_in_urb, GFP_ATOMIC);
	if (result)
		dev_err(&port->dev, "usb_submit_urb(read int) failed\n");
	dev_dbg(dev, "%s - usb_submit_urb(int urb)\n", __func__);
>>>>>>> refs/remotes/origin/master
}

static void cyberjack_read_bulk_callback(struct urb *urb)
{
	struct usb_serial_port *port = urb->context;
	struct cyberjack_private *priv = usb_get_serial_port_data(port);
<<<<<<< HEAD
	struct tty_struct *tty;
=======
	struct device *dev = &port->dev;
>>>>>>> refs/remotes/origin/master
	unsigned char *data = urb->transfer_buffer;
	short todo;
	int result;
	int status = urb->status;

<<<<<<< HEAD
	dbg("%s - port %d", __func__, port->number);

	usb_serial_debug_data(debug, &port->dev, __func__,
						urb->actual_length, data);
	if (status) {
		dbg("%s - nonzero read bulk status received: %d",
		    __func__, status);
		return;
	}

	tty = tty_port_tty_get(&port->port);
	if (!tty) {
		dbg("%s - ignoring since device not open", __func__);
		return;
	}
	if (urb->actual_length) {
		tty_insert_flip_string(tty, data, urb->actual_length);
		tty_flip_buffer_push(tty);
	}
	tty_kref_put(tty);
=======
	usb_serial_debug_data(dev, __func__, urb->actual_length, data);
	if (status) {
		dev_dbg(dev, "%s - nonzero read bulk status received: %d\n",
			__func__, status);
		return;
	}

	if (urb->actual_length) {
		tty_insert_flip_string(&port->port, data, urb->actual_length);
		tty_flip_buffer_push(&port->port);
	}
>>>>>>> refs/remotes/origin/master

	spin_lock(&priv->lock);

	/* Reduce urbs to do by one. */
	priv->rdtodo -= urb->actual_length;
	/* Just to be sure */
	if (priv->rdtodo < 0)
		priv->rdtodo = 0;
	todo = priv->rdtodo;

	spin_unlock(&priv->lock);

<<<<<<< HEAD
	dbg("%s - rdtodo: %d", __func__, todo);

	/* Continue to read if we have still urbs to do. */
	if (todo /* || (urb->actual_length==port->bulk_in_endpointAddress)*/) {
<<<<<<< HEAD
		port->read_urb->dev = port->serial->dev;
=======
>>>>>>> refs/remotes/origin/cm-10.0
		result = usb_submit_urb(port->read_urb, GFP_ATOMIC);
		if (result)
			dev_err(&port->dev, "%s - failed resubmitting read "
				"urb, error %d\n", __func__, result);
		dbg("%s - usb_submit_urb(read urb)", __func__);
=======
	dev_dbg(dev, "%s - rdtodo: %d\n", __func__, todo);

	/* Continue to read if we have still urbs to do. */
	if (todo /* || (urb->actual_length==port->bulk_in_endpointAddress)*/) {
		result = usb_submit_urb(port->read_urb, GFP_ATOMIC);
		if (result)
			dev_err(dev, "%s - failed resubmitting read urb, error %d\n",
				__func__, result);
		dev_dbg(dev, "%s - usb_submit_urb(read urb)\n", __func__);
>>>>>>> refs/remotes/origin/master
	}
}

static void cyberjack_write_bulk_callback(struct urb *urb)
{
	struct usb_serial_port *port = urb->context;
	struct cyberjack_private *priv = usb_get_serial_port_data(port);
<<<<<<< HEAD
	int status = urb->status;

	dbg("%s - port %d", __func__, port->number);

<<<<<<< HEAD
	port->write_urb_busy = 0;
=======
	set_bit(0, &port->write_urbs_free);
>>>>>>> refs/remotes/origin/cm-10.0
	if (status) {
		dbg("%s - nonzero write bulk status received: %d",
		    __func__, status);
=======
	struct device *dev = &port->dev;
	int status = urb->status;

	set_bit(0, &port->write_urbs_free);
	if (status) {
		dev_dbg(dev, "%s - nonzero write bulk status received: %d\n",
			__func__, status);
>>>>>>> refs/remotes/origin/master
		return;
	}

	spin_lock(&priv->lock);

	/* only do something if we have more data to send */
	if (priv->wrfilled) {
		int length, blksize, result;

<<<<<<< HEAD
		dbg("%s - transmitting data (frame n)", __func__);
=======
		dev_dbg(dev, "%s - transmitting data (frame n)\n", __func__);
>>>>>>> refs/remotes/origin/master

		length = ((priv->wrfilled - priv->wrsent) > port->bulk_out_size) ?
			port->bulk_out_size : (priv->wrfilled - priv->wrsent);

		memcpy(port->write_urb->transfer_buffer,
					priv->wrbuf + priv->wrsent, length);
		priv->wrsent += length;

		/* set up our urb */
<<<<<<< HEAD
<<<<<<< HEAD
		usb_fill_bulk_urb(port->write_urb, port->serial->dev,
			      usb_sndbulkpipe(port->serial->dev, port->bulk_out_endpointAddress),
			      port->write_urb->transfer_buffer, length,
			      ((port->serial->type->write_bulk_callback) ?
			       port->serial->type->write_bulk_callback :
			       cyberjack_write_bulk_callback),
			      port);
=======
		port->write_urb->transfer_buffer_length = length;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		port->write_urb->transfer_buffer_length = length;
>>>>>>> refs/remotes/origin/master

		/* send the data out the bulk port */
		result = usb_submit_urb(port->write_urb, GFP_ATOMIC);
		if (result) {
<<<<<<< HEAD
			dev_err(&port->dev,
				"%s - failed submitting write urb, error %d\n",
=======
			dev_err(dev, "%s - failed submitting write urb, error %d\n",
>>>>>>> refs/remotes/origin/master
				__func__, result);
			/* Throw away data. No better idea what to do with it. */
			priv->wrfilled = 0;
			priv->wrsent = 0;
			goto exit;
		}

<<<<<<< HEAD
		dbg("%s - priv->wrsent=%d", __func__, priv->wrsent);
		dbg("%s - priv->wrfilled=%d", __func__, priv->wrfilled);
=======
		dev_dbg(dev, "%s - priv->wrsent=%d\n", __func__, priv->wrsent);
		dev_dbg(dev, "%s - priv->wrfilled=%d\n", __func__, priv->wrfilled);
>>>>>>> refs/remotes/origin/master

		blksize = ((int)priv->wrbuf[2]<<8)+priv->wrbuf[1]+3;

		if (priv->wrsent >= priv->wrfilled ||
					priv->wrsent >= blksize) {
<<<<<<< HEAD
			dbg("%s - buffer cleaned", __func__);
=======
			dev_dbg(dev, "%s - buffer cleaned\n", __func__);
>>>>>>> refs/remotes/origin/master
			memset(priv->wrbuf, 0, sizeof(priv->wrbuf));
			priv->wrfilled = 0;
			priv->wrsent = 0;
		}
	}

exit:
	spin_unlock(&priv->lock);
	usb_serial_port_softint(port);
}

<<<<<<< HEAD
<<<<<<< HEAD
static int __init cyberjack_init(void)
{
	int retval;
	retval  = usb_serial_register(&cyberjack_device);
	if (retval)
		goto failed_usb_serial_register;
	retval = usb_register(&cyberjack_driver);
	if (retval)
		goto failed_usb_register;

	printk(KERN_INFO KBUILD_MODNAME ": " DRIVER_VERSION " "
	       DRIVER_AUTHOR "\n");
	printk(KERN_INFO KBUILD_MODNAME ": " DRIVER_DESC "\n");

	return 0;
failed_usb_register:
	usb_serial_deregister(&cyberjack_device);
failed_usb_serial_register:
	return retval;
}

static void __exit cyberjack_exit(void)
{
	usb_deregister(&cyberjack_driver);
	usb_serial_deregister(&cyberjack_device);
}

module_init(cyberjack_init);
module_exit(cyberjack_exit);
=======
module_usb_serial_driver(cyberjack_driver, serial_drivers);
>>>>>>> refs/remotes/origin/cm-10.0

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERSION);
MODULE_LICENSE("GPL");

module_param(debug, bool, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(debug, "Debug enabled or not");
=======
module_usb_serial_driver(serial_drivers, id_table);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
>>>>>>> refs/remotes/origin/master
