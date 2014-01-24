/*
 * Navman Serial USB driver
 *
 * Copyright (C) 2006 Greg Kroah-Hartman <gregkh@suse.de>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	version 2 as published by the Free Software Foundation.
 *
 * TODO:
 *	Add termios method that uses copy_hw but also kills all echo
 *	flags as the navman is rx only so cannot echo.
 */

#include <linux/gfp.h>
#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> refs/remotes/origin/master
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/usb/serial.h>

<<<<<<< HEAD
<<<<<<< HEAD
static int debug;
=======
static bool debug;
>>>>>>> refs/remotes/origin/cm-10.0

=======
>>>>>>> refs/remotes/origin/master
static const struct usb_device_id id_table[] = {
	{ USB_DEVICE(0x0a99, 0x0001) },	/* Talon Technology device */
	{ USB_DEVICE(0x0df7, 0x0900) },	/* Mobile Action i-gotU */
	{ },
};
MODULE_DEVICE_TABLE(usb, id_table);

<<<<<<< HEAD
static struct usb_driver navman_driver = {
	.name =		"navman",
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
static void navman_read_int_callback(struct urb *urb)
{
	struct usb_serial_port *port = urb->context;
	unsigned char *data = urb->transfer_buffer;
<<<<<<< HEAD
	struct tty_struct *tty;
=======
>>>>>>> refs/remotes/origin/master
	int status = urb->status;
	int result;

	switch (status) {
	case 0:
		/* success */
		break;
	case -ECONNRESET:
	case -ENOENT:
	case -ESHUTDOWN:
		/* this urb is terminated, clean up */
<<<<<<< HEAD
		dbg("%s - urb shutting down with status: %d",
		    __func__, status);
		return;
	default:
		dbg("%s - nonzero urb status received: %d",
		    __func__, status);
		goto exit;
	}

	usb_serial_debug_data(debug, &port->dev, __func__,
			      urb->actual_length, data);

	tty = tty_port_tty_get(&port->port);
	if (tty && urb->actual_length) {
		tty_insert_flip_string(tty, data, urb->actual_length);
		tty_flip_buffer_push(tty);
	}
	tty_kref_put(tty);
=======
		dev_dbg(&port->dev, "%s - urb shutting down with status: %d\n",
			__func__, status);
		return;
	default:
		dev_dbg(&port->dev, "%s - nonzero urb status received: %d\n",
			__func__, status);
		goto exit;
	}

	usb_serial_debug_data(&port->dev, __func__, urb->actual_length, data);

	if (urb->actual_length) {
		tty_insert_flip_string(&port->port, data, urb->actual_length);
		tty_flip_buffer_push(&port->port);
	}
>>>>>>> refs/remotes/origin/master

exit:
	result = usb_submit_urb(urb, GFP_ATOMIC);
	if (result)
		dev_err(&urb->dev->dev,
			"%s - Error %d submitting interrupt urb\n",
			__func__, result);
}

static int navman_open(struct tty_struct *tty, struct usb_serial_port *port)
{
	int result = 0;

<<<<<<< HEAD
	dbg("%s - port %d", __func__, port->number);

	if (port->interrupt_in_urb) {
		dbg("%s - adding interrupt input for treo", __func__);
=======
	if (port->interrupt_in_urb) {
		dev_dbg(&port->dev, "%s - adding interrupt input for treo\n",
			__func__);
>>>>>>> refs/remotes/origin/master
		result = usb_submit_urb(port->interrupt_in_urb, GFP_KERNEL);
		if (result)
			dev_err(&port->dev,
				"%s - failed submitting interrupt urb, error %d\n",
				__func__, result);
	}
	return result;
}

static void navman_close(struct usb_serial_port *port)
{
<<<<<<< HEAD
	dbg("%s - port %d", __func__, port->number);

=======
>>>>>>> refs/remotes/origin/master
	usb_kill_urb(port->interrupt_in_urb);
}

static int navman_write(struct tty_struct *tty, struct usb_serial_port *port,
			const unsigned char *buf, int count)
{
<<<<<<< HEAD
	dbg("%s - port %d", __func__, port->number);

=======
>>>>>>> refs/remotes/origin/master
	/*
	 * This device can't write any data, only read from the device
	 */
	return -EOPNOTSUPP;
}

static struct usb_serial_driver navman_device = {
	.driver = {
		.owner =	THIS_MODULE,
		.name =		"navman",
	},
	.id_table =		id_table,
<<<<<<< HEAD
<<<<<<< HEAD
	.usb_driver =		&navman_driver,
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	.num_ports =		1,
	.open =			navman_open,
	.close = 		navman_close,
	.write = 		navman_write,
	.read_int_callback =	navman_read_int_callback,
};

<<<<<<< HEAD
<<<<<<< HEAD
static int __init navman_init(void)
{
	int retval;

	retval = usb_serial_register(&navman_device);
	if (retval)
		return retval;
	retval = usb_register(&navman_driver);
	if (retval)
		usb_serial_deregister(&navman_device);
	return retval;
}

static void __exit navman_exit(void)
{
	usb_deregister(&navman_driver);
	usb_serial_deregister(&navman_device);
}

module_init(navman_init);
module_exit(navman_exit);
=======
=======
>>>>>>> refs/remotes/origin/master
static struct usb_serial_driver * const serial_drivers[] = {
	&navman_device, NULL
};

<<<<<<< HEAD
module_usb_serial_driver(navman_driver, serial_drivers);

>>>>>>> refs/remotes/origin/cm-10.0
MODULE_LICENSE("GPL");

module_param(debug, bool, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(debug, "Debug enabled or not");
=======
module_usb_serial_driver(serial_drivers, id_table);

MODULE_LICENSE("GPL");
>>>>>>> refs/remotes/origin/master
