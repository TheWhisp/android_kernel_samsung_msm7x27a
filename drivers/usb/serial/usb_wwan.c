/*
  USB Driver layer for GSM modems

  Copyright (C) 2005  Matthias Urlichs <smurf@smurf.noris.de>

  This driver is free software; you can redistribute it and/or modify
  it under the terms of Version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  Portions copied from the Keyspan driver by Hugh Blemings <hugh@blemings.org>

  History: see the git log.

  Work sponsored by: Sigos GmbH, Germany <info@sigos.de>

  This driver exists because the "normal" serial driver doesn't work too well
  with GSM modems. Issues:
  - data loss -- one single Receive URB is not nearly enough
  - controlling the baud rate doesn't make sense
*/

<<<<<<< HEAD
#define DRIVER_VERSION "v0.7.2"
=======
>>>>>>> refs/remotes/origin/master
#define DRIVER_AUTHOR "Matthias Urlichs <smurf@smurf.noris.de>"
#define DRIVER_DESC "USB Driver for GSM modems"

#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/module.h>
#include <linux/bitops.h>
#include <linux/uaccess.h>
#include <linux/usb.h>
#include <linux/usb/serial.h>
#include <linux/serial.h>
#include "usb-wwan.h"

<<<<<<< HEAD
<<<<<<< HEAD
static int debug;

void usb_wwan_dtr_rts(struct usb_serial_port *port, int on)
{
<<<<<<< HEAD
	struct usb_serial *serial = port->serial;
=======
static bool debug;

void usb_wwan_dtr_rts(struct usb_serial_port *port, int on)
{
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	struct usb_wwan_port_private *portdata;

	struct usb_wwan_intf_private *intfdata;

	dbg("%s", __func__);

=======
void usb_wwan_dtr_rts(struct usb_serial_port *port, int on)
{
	struct usb_wwan_port_private *portdata;
	struct usb_wwan_intf_private *intfdata;

>>>>>>> refs/remotes/origin/master
	intfdata = port->serial->private;

	if (!intfdata->send_setup)
		return;

	portdata = usb_get_serial_port_data(port);
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	mutex_lock(&serial->disc_mutex);
	portdata->rts_state = on;
	portdata->dtr_state = on;
	if (serial->dev)
		intfdata->send_setup(port);
	mutex_unlock(&serial->disc_mutex);
=======
=======
>>>>>>> refs/remotes/origin/master
	/* FIXME: locking */
	portdata->rts_state = on;
	portdata->dtr_state = on;

	intfdata->send_setup(port);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
	/* FIXME: locking */
	portdata->rts_state = on;
	portdata->dtr_state = on;

	intfdata->send_setup(port);
>>>>>>> refs/remotes/origin/cm-11.0
}
EXPORT_SYMBOL(usb_wwan_dtr_rts);

void usb_wwan_set_termios(struct tty_struct *tty,
			  struct usb_serial_port *port,
			  struct ktermios *old_termios)
{
	struct usb_wwan_intf_private *intfdata = port->serial->private;

<<<<<<< HEAD
	dbg("%s", __func__);

	/* Doesn't support option setting */
	tty_termios_copy_hw(tty->termios, old_termios);
=======
	/* Doesn't support option setting */
	tty_termios_copy_hw(&tty->termios, old_termios);
>>>>>>> refs/remotes/origin/master

	if (intfdata->send_setup)
		intfdata->send_setup(port);
}
EXPORT_SYMBOL(usb_wwan_set_termios);

int usb_wwan_tiocmget(struct tty_struct *tty)
{
	struct usb_serial_port *port = tty->driver_data;
	unsigned int value;
	struct usb_wwan_port_private *portdata;

	portdata = usb_get_serial_port_data(port);

	value = ((portdata->rts_state) ? TIOCM_RTS : 0) |
	    ((portdata->dtr_state) ? TIOCM_DTR : 0) |
	    ((portdata->cts_state) ? TIOCM_CTS : 0) |
	    ((portdata->dsr_state) ? TIOCM_DSR : 0) |
	    ((portdata->dcd_state) ? TIOCM_CAR : 0) |
	    ((portdata->ri_state) ? TIOCM_RNG : 0);

	return value;
}
EXPORT_SYMBOL(usb_wwan_tiocmget);

int usb_wwan_tiocmset(struct tty_struct *tty,
		      unsigned int set, unsigned int clear)
{
	struct usb_serial_port *port = tty->driver_data;
	struct usb_wwan_port_private *portdata;
	struct usb_wwan_intf_private *intfdata;

	portdata = usb_get_serial_port_data(port);
	intfdata = port->serial->private;

	if (!intfdata->send_setup)
		return -EINVAL;

	/* FIXME: what locks portdata fields ? */
	if (set & TIOCM_RTS)
		portdata->rts_state = 1;
	if (set & TIOCM_DTR)
		portdata->dtr_state = 1;

	if (clear & TIOCM_RTS)
		portdata->rts_state = 0;
	if (clear & TIOCM_DTR)
		portdata->dtr_state = 0;
	return intfdata->send_setup(port);
}
EXPORT_SYMBOL(usb_wwan_tiocmset);

static int get_serial_info(struct usb_serial_port *port,
			   struct serial_struct __user *retinfo)
{
	struct serial_struct tmp;

	if (!retinfo)
		return -EFAULT;

	memset(&tmp, 0, sizeof(tmp));
<<<<<<< HEAD
	tmp.line            = port->serial->minor;
	tmp.port            = port->number;
=======
	tmp.line            = port->minor;
	tmp.port            = port->port_number;
>>>>>>> refs/remotes/origin/master
	tmp.baud_base       = tty_get_baud_rate(port->port.tty);
	tmp.close_delay	    = port->port.close_delay / 10;
	tmp.closing_wait    = port->port.closing_wait == ASYNC_CLOSING_WAIT_NONE ?
				 ASYNC_CLOSING_WAIT_NONE :
				 port->port.closing_wait / 10;

	if (copy_to_user(retinfo, &tmp, sizeof(*retinfo)))
		return -EFAULT;
	return 0;
}

static int set_serial_info(struct usb_serial_port *port,
			   struct serial_struct __user *newinfo)
{
	struct serial_struct new_serial;
	unsigned int closing_wait, close_delay;
	int retval = 0;

	if (copy_from_user(&new_serial, newinfo, sizeof(new_serial)))
		return -EFAULT;

	close_delay = new_serial.close_delay * 10;
	closing_wait = new_serial.closing_wait == ASYNC_CLOSING_WAIT_NONE ?
			ASYNC_CLOSING_WAIT_NONE : new_serial.closing_wait * 10;

	mutex_lock(&port->port.mutex);

	if (!capable(CAP_SYS_ADMIN)) {
		if ((close_delay != port->port.close_delay) ||
		    (closing_wait != port->port.closing_wait))
			retval = -EPERM;
		else
			retval = -EOPNOTSUPP;
	} else {
		port->port.close_delay  = close_delay;
		port->port.closing_wait = closing_wait;
	}

	mutex_unlock(&port->port.mutex);
	return retval;
}

int usb_wwan_ioctl(struct tty_struct *tty,
		   unsigned int cmd, unsigned long arg)
{
	struct usb_serial_port *port = tty->driver_data;

<<<<<<< HEAD
	dbg("%s cmd 0x%04x", __func__, cmd);
=======
	dev_dbg(&port->dev, "%s cmd 0x%04x\n", __func__, cmd);
>>>>>>> refs/remotes/origin/master

	switch (cmd) {
	case TIOCGSERIAL:
		return get_serial_info(port,
				       (struct serial_struct __user *) arg);
	case TIOCSSERIAL:
		return set_serial_info(port,
				       (struct serial_struct __user *) arg);
	default:
		break;
	}

<<<<<<< HEAD
	dbg("%s arg not supported", __func__);
=======
	dev_dbg(&port->dev, "%s arg not supported\n", __func__);
>>>>>>> refs/remotes/origin/master

	return -ENOIOCTLCMD;
}
EXPORT_SYMBOL(usb_wwan_ioctl);

/* Write */
int usb_wwan_write(struct tty_struct *tty, struct usb_serial_port *port,
		   const unsigned char *buf, int count)
{
	struct usb_wwan_port_private *portdata;
	struct usb_wwan_intf_private *intfdata;
	int i;
	int left, todo;
	struct urb *this_urb = NULL;	/* spurious */
	int err;
	unsigned long flags;

	portdata = usb_get_serial_port_data(port);
	intfdata = port->serial->private;

<<<<<<< HEAD
	dbg("%s: write (%d chars)", __func__, count);
=======
	dev_dbg(&port->dev, "%s: write (%d chars)\n", __func__, count);
>>>>>>> refs/remotes/origin/master

	i = 0;
	left = count;
	for (i = 0; left > 0 && i < N_OUT_URB; i++) {
		todo = left;
		if (todo > OUT_BUFLEN)
			todo = OUT_BUFLEN;

		this_urb = portdata->out_urbs[i];
		if (test_and_set_bit(i, &portdata->out_busy)) {
			if (time_before(jiffies,
					portdata->tx_start_time[i] + 10 * HZ))
				continue;
			usb_unlink_urb(this_urb);
			continue;
		}
<<<<<<< HEAD
		dbg("%s: endpoint %d buf %d", __func__,
		    usb_pipeendpoint(this_urb->pipe), i);
=======
		dev_dbg(&port->dev, "%s: endpoint %d buf %d\n", __func__,
			usb_pipeendpoint(this_urb->pipe), i);
>>>>>>> refs/remotes/origin/master

		err = usb_autopm_get_interface_async(port->serial->interface);
		if (err < 0)
			break;

		/* send the data */
		memcpy(this_urb->transfer_buffer, buf, todo);
		this_urb->transfer_buffer_length = todo;

		spin_lock_irqsave(&intfdata->susp_lock, flags);
		if (intfdata->suspended) {
			usb_anchor_urb(this_urb, &portdata->delayed);
			spin_unlock_irqrestore(&intfdata->susp_lock, flags);
		} else {
			intfdata->in_flight++;
			spin_unlock_irqrestore(&intfdata->susp_lock, flags);
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
			usb_anchor_urb(this_urb, &portdata->submitted);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			usb_anchor_urb(this_urb, &portdata->submitted);
>>>>>>> refs/remotes/origin/cm-11.0
			err = usb_submit_urb(this_urb, GFP_ATOMIC);
			if (err) {
				dbg("usb_submit_urb %p (write bulk) failed "
				    "(%d)", this_urb, err);
<<<<<<< HEAD
<<<<<<< HEAD
=======
				usb_unanchor_urb(this_urb);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			err = usb_submit_urb(this_urb, GFP_ATOMIC);
			if (err) {
				dev_dbg(&port->dev,
					"usb_submit_urb %p (write bulk) failed (%d)\n",
					this_urb, err);
>>>>>>> refs/remotes/origin/master
=======
				usb_unanchor_urb(this_urb);
>>>>>>> refs/remotes/origin/cm-11.0
				clear_bit(i, &portdata->out_busy);
				spin_lock_irqsave(&intfdata->susp_lock, flags);
				intfdata->in_flight--;
				spin_unlock_irqrestore(&intfdata->susp_lock,
						       flags);
				usb_autopm_put_interface_async(port->serial->interface);
				break;
			}
		}

		portdata->tx_start_time[i] = jiffies;
		buf += todo;
		left -= todo;
	}

	count -= left;
<<<<<<< HEAD
	dbg("%s: wrote (did %d)", __func__, count);
=======
	dev_dbg(&port->dev, "%s: wrote (did %d)\n", __func__, count);
>>>>>>> refs/remotes/origin/master
	return count;
}
EXPORT_SYMBOL(usb_wwan_write);

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
static void usb_wwan_in_work(struct work_struct *w)
{
	struct usb_wwan_port_private *portdata =
		container_of(w, struct usb_wwan_port_private, in_work);
	struct usb_wwan_intf_private *intfdata;
	struct list_head *q = &portdata->in_urb_list;
	struct urb *urb;
	unsigned char *data;
	struct tty_struct *tty;
	struct usb_serial_port *port;
	int err;
	ssize_t len;
	ssize_t count;
	unsigned long flags;

	spin_lock_irqsave(&portdata->in_lock, flags);
	while (!list_empty(q)) {
		urb = list_first_entry(q, struct urb, urb_list);
		port = urb->context;
		if (port->throttle_req || port->throttled)
			break;

		tty = tty_port_tty_get(&port->port);
		if (!tty)
			break;

		/* list_empty() will still be false after this; it means
		 * URB is still being processed */
		list_del(&urb->urb_list);

		spin_unlock_irqrestore(&portdata->in_lock, flags);

		len = urb->actual_length - portdata->n_read;
		data = urb->transfer_buffer + portdata->n_read;
		count = tty_insert_flip_string(tty, data, len);
		tty_flip_buffer_push(tty);
		tty_kref_put(tty);

		if (count < len) {
			dbg("%s: len:%d count:%d n_read:%d\n", __func__,
					len, count, portdata->n_read);
			portdata->n_read += count;
			port->throttled = true;

			/* add request back to list */
			spin_lock_irqsave(&portdata->in_lock, flags);
			list_add(&urb->urb_list, q);
			spin_unlock_irqrestore(&portdata->in_lock, flags);
			return;
		}

		/* re-init list pointer to indicate we are done with it */
		INIT_LIST_HEAD(&urb->urb_list);

		portdata->n_read = 0;
		intfdata = port->serial->private;

		spin_lock_irqsave(&intfdata->susp_lock, flags);
		if (!intfdata->suspended && !urb->anchor) {
			usb_anchor_urb(urb, &portdata->submitted);
			err = usb_submit_urb(urb, GFP_ATOMIC);
			if (err) {
				usb_unanchor_urb(urb);
				if (err != -EPERM)
					pr_err("%s: submit read urb failed:%d",
							__func__, err);
			}

			usb_mark_last_busy(port->serial->dev);
		}
		spin_unlock_irqrestore(&intfdata->susp_lock, flags);
		spin_lock_irqsave(&portdata->in_lock, flags);
	}
	spin_unlock_irqrestore(&portdata->in_lock, flags);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
static void usb_wwan_indat_callback(struct urb *urb)
{
	int err;
	int endpoint;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct usb_wwan_port_private *portdata;
	struct usb_wwan_intf_private *intfdata;
>>>>>>> refs/remotes/origin/cm-11.0
	struct usb_serial_port *port;
	int status = urb->status;
<<<<<<< HEAD
=======
	struct usb_wwan_port_private *portdata;
	struct usb_wwan_intf_private *intfdata;
	struct usb_serial_port *port;
	int status = urb->status;
	unsigned long flags;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned long flags;
>>>>>>> refs/remotes/origin/cm-11.0

	dbg("%s: %p", __func__, urb);

	endpoint = usb_pipeendpoint(urb->pipe);
	port = urb->context;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	portdata = usb_get_serial_port_data(port);
	intfdata = port->serial->private;
>>>>>>> refs/remotes/origin/cm-11.0

	usb_mark_last_busy(port->serial->dev);

	if ((status == -ENOENT || !status) && urb->actual_length) {
		spin_lock_irqsave(&portdata->in_lock, flags);
		list_add_tail(&urb->urb_list, &portdata->in_urb_list);
		spin_unlock_irqrestore(&portdata->in_lock, flags);

		schedule_work(&portdata->in_work);

		return;
	}

	dbg("%s: nonzero status: %d on endpoint %02x.",
		__func__, status, endpoint);

	spin_lock(&intfdata->susp_lock);
	if (intfdata->suspended || !portdata->opened) {
		spin_unlock(&intfdata->susp_lock);
		return;
	}
	spin_unlock(&intfdata->susp_lock);

<<<<<<< HEAD
=======
	portdata = usb_get_serial_port_data(port);
	intfdata = port->serial->private;

	usb_mark_last_busy(port->serial->dev);

	if ((status == -ENOENT || !status) && urb->actual_length) {
		spin_lock_irqsave(&portdata->in_lock, flags);
		list_add_tail(&urb->urb_list, &portdata->in_urb_list);
		spin_unlock_irqrestore(&portdata->in_lock, flags);

		schedule_work(&portdata->in_work);

		return;
	}

	dbg("%s: nonzero status: %d on endpoint %02x.",
		__func__, status, endpoint);

	spin_lock(&intfdata->susp_lock);
	if (intfdata->suspended || !portdata->opened) {
		spin_unlock(&intfdata->susp_lock);
		return;
	}
	spin_unlock(&intfdata->susp_lock);

=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (status != -ESHUTDOWN) {
		usb_anchor_urb(urb, &portdata->submitted);
		err = usb_submit_urb(urb, GFP_ATOMIC);
		if (err) {
			usb_unanchor_urb(urb);
			if (err != -EPERM)
				pr_err("%s: submit read urb failed:%d",
						__func__, err);
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct usb_serial_port *port;
	struct device *dev;
	unsigned char *data = urb->transfer_buffer;
	int status = urb->status;

	endpoint = usb_pipeendpoint(urb->pipe);
	port = urb->context;
	dev = &port->dev;

	if (status) {
		dev_dbg(dev, "%s: nonzero status: %d on endpoint %02x.\n",
			__func__, status, endpoint);
	} else {
		if (urb->actual_length) {
			tty_insert_flip_string(&port->port, data,
					urb->actual_length);
			tty_flip_buffer_push(&port->port);
		} else
			dev_dbg(dev, "%s: empty read urb received\n", __func__);
	}
	/* Resubmit urb so we continue receiving */
	err = usb_submit_urb(urb, GFP_ATOMIC);
	if (err) {
		if (err != -EPERM) {
			dev_err(dev, "%s: resubmit read urb failed. (%d)\n",
				__func__, err);
			/* busy also in error unless we are killed */
			usb_mark_last_busy(port->serial->dev);
		}
	} else {
		usb_mark_last_busy(port->serial->dev);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	}
}

static void usb_wwan_outdat_callback(struct urb *urb)
{
	struct usb_serial_port *port;
	struct usb_wwan_port_private *portdata;
	struct usb_wwan_intf_private *intfdata;
	int i;

<<<<<<< HEAD
	dbg("%s", __func__);

=======
>>>>>>> refs/remotes/origin/master
	port = urb->context;
	intfdata = port->serial->private;

	usb_serial_port_softint(port);
	usb_autopm_put_interface_async(port->serial->interface);
	portdata = usb_get_serial_port_data(port);
	spin_lock(&intfdata->susp_lock);
	intfdata->in_flight--;
	spin_unlock(&intfdata->susp_lock);

	for (i = 0; i < N_OUT_URB; ++i) {
		if (portdata->out_urbs[i] == urb) {
			smp_mb__before_clear_bit();
			clear_bit(i, &portdata->out_busy);
			break;
		}
	}
}

int usb_wwan_write_room(struct tty_struct *tty)
{
	struct usb_serial_port *port = tty->driver_data;
	struct usb_wwan_port_private *portdata;
	int i;
	int data_len = 0;
	struct urb *this_urb;

	portdata = usb_get_serial_port_data(port);

	for (i = 0; i < N_OUT_URB; i++) {
		this_urb = portdata->out_urbs[i];
		if (this_urb && !test_bit(i, &portdata->out_busy))
			data_len += OUT_BUFLEN;
	}

<<<<<<< HEAD
	dbg("%s: %d", __func__, data_len);
=======
	dev_dbg(&port->dev, "%s: %d\n", __func__, data_len);
>>>>>>> refs/remotes/origin/master
	return data_len;
}
EXPORT_SYMBOL(usb_wwan_write_room);

int usb_wwan_chars_in_buffer(struct tty_struct *tty)
{
	struct usb_serial_port *port = tty->driver_data;
	struct usb_wwan_port_private *portdata;
	int i;
	int data_len = 0;
	struct urb *this_urb;

	portdata = usb_get_serial_port_data(port);

	for (i = 0; i < N_OUT_URB; i++) {
		this_urb = portdata->out_urbs[i];
		/* FIXME: This locking is insufficient as this_urb may
		   go unused during the test */
		if (this_urb && test_bit(i, &portdata->out_busy))
			data_len += this_urb->transfer_buffer_length;
	}
<<<<<<< HEAD
	dbg("%s: %d", __func__, data_len);
=======
	dev_dbg(&port->dev, "%s: %d\n", __func__, data_len);
>>>>>>> refs/remotes/origin/master
	return data_len;
}
EXPORT_SYMBOL(usb_wwan_chars_in_buffer);

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
void usb_wwan_throttle(struct tty_struct *tty)
{
	struct usb_serial_port *port = tty->driver_data;

	port->throttle_req = true;

	dbg("%s:\n", __func__);
}
EXPORT_SYMBOL(usb_wwan_throttle);

void usb_wwan_unthrottle(struct tty_struct *tty)
{
	struct usb_serial_port *port = tty->driver_data;
	struct usb_wwan_port_private *portdata;

	portdata = usb_get_serial_port_data(port);

	dbg("%s:\n", __func__);
	port->throttle_req = false;
	port->throttled = false;

	schedule_work(&portdata->in_work);
}
EXPORT_SYMBOL(usb_wwan_unthrottle);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
int usb_wwan_open(struct tty_struct *tty, struct usb_serial_port *port)
{
	struct usb_wwan_port_private *portdata;
	struct usb_wwan_intf_private *intfdata;
	struct usb_serial *serial = port->serial;
	int i, err;
	struct urb *urb;

	portdata = usb_get_serial_port_data(port);
	intfdata = serial->private;

<<<<<<< HEAD
<<<<<<< HEAD
	/* explicitly set the driver mode to raw */
	tty->raw = 1;
	tty->real_raw = 1;
<<<<<<< HEAD

=======
	tty->update_room_in_ldisc = 1;

	set_bit(TTY_NO_WRITE_SPLIT, &tty->flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* explicitly set the driver mode to raw */
	tty->raw = 1;
	tty->real_raw = 1;
	tty->update_room_in_ldisc = 1;

	set_bit(TTY_NO_WRITE_SPLIT, &tty->flags);
>>>>>>> refs/remotes/origin/cm-11.0
	dbg("%s", __func__);

=======
>>>>>>> refs/remotes/origin/master
	/* Start reading from the IN endpoint */
	for (i = 0; i < N_IN_URB; i++) {
		urb = portdata->in_urbs[i];
		if (!urb)
			continue;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
		err = usb_submit_urb(urb, GFP_KERNEL);
		if (err) {
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
		usb_anchor_urb(urb, &portdata->submitted);
		err = usb_submit_urb(urb, GFP_KERNEL);
		if (err) {
			usb_unanchor_urb(urb);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
			dbg("%s: submit urb %d failed (%d) %d",
			    __func__, i, err, urb->transfer_buffer_length);
=======
		err = usb_submit_urb(urb, GFP_KERNEL);
		if (err) {
			dev_dbg(&port->dev, "%s: submit urb %d failed (%d) %d\n",
				__func__, i, err, urb->transfer_buffer_length);
>>>>>>> refs/remotes/origin/master
		}
	}

	if (intfdata->send_setup)
		intfdata->send_setup(port);

	serial->interface->needs_remote_wakeup = 1;
	spin_lock_irq(&intfdata->susp_lock);
	portdata->opened = 1;
	spin_unlock_irq(&intfdata->susp_lock);
	/* this balances a get in the generic USB serial code */
	usb_autopm_put_interface(serial->interface);

	return 0;
}
EXPORT_SYMBOL(usb_wwan_open);

void usb_wwan_close(struct usb_serial_port *port)
{
	int i;
	struct usb_serial *serial = port->serial;
	struct usb_wwan_port_private *portdata;
	struct usb_wwan_intf_private *intfdata = port->serial->private;

<<<<<<< HEAD
	dbg("%s", __func__);
	portdata = usb_get_serial_port_data(port);

	if (serial->dev) {
		/* Stop reading/writing urbs */
		spin_lock_irq(&intfdata->susp_lock);
		portdata->opened = 0;
		spin_unlock_irq(&intfdata->susp_lock);

		for (i = 0; i < N_IN_URB; i++)
			usb_kill_urb(portdata->in_urbs[i]);
		for (i = 0; i < N_OUT_URB; i++)
			usb_kill_urb(portdata->out_urbs[i]);
		/* balancing - important as an error cannot be handled*/
		usb_autopm_get_interface_no_resume(serial->interface);
		serial->interface->needs_remote_wakeup = 0;
	}
=======
	portdata = usb_get_serial_port_data(port);

	/* Stop reading/writing urbs */
	spin_lock_irq(&intfdata->susp_lock);
	portdata->opened = 0;
	spin_unlock_irq(&intfdata->susp_lock);

	for (i = 0; i < N_IN_URB; i++)
		usb_kill_urb(portdata->in_urbs[i]);
	for (i = 0; i < N_OUT_URB; i++)
		usb_kill_urb(portdata->out_urbs[i]);

	/* balancing - important as an error cannot be handled*/
	usb_autopm_get_interface_no_resume(serial->interface);
	serial->interface->needs_remote_wakeup = 0;
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(usb_wwan_close);

/* Helper functions used by usb_wwan_setup_urbs */
<<<<<<< HEAD
static struct urb *usb_wwan_setup_urb(struct usb_serial *serial, int endpoint,
				      int dir, void *ctx, char *buf, int len,
				      void (*callback) (struct urb *))
{
	struct urb *urb;

	if (endpoint == -1)
		return NULL;	/* endpoint not needed */

	urb = usb_alloc_urb(0, GFP_KERNEL);	/* No ISO */
	if (urb == NULL) {
		dbg("%s: alloc for endpoint %d failed.", __func__, endpoint);
		return NULL;
	}
=======
static struct urb *usb_wwan_setup_urb(struct usb_serial_port *port,
				      int endpoint,
				      int dir, void *ctx, char *buf, int len,
				      void (*callback) (struct urb *))
{
	struct usb_serial *serial = port->serial;
	struct urb *urb;

	urb = usb_alloc_urb(0, GFP_KERNEL);	/* No ISO */
	if (!urb)
		return NULL;
>>>>>>> refs/remotes/origin/master

	/* Fill URB using supplied data. */
	usb_fill_bulk_urb(urb, serial->dev,
			  usb_sndbulkpipe(serial->dev, endpoint) | dir,
			  buf, len, callback, ctx);

	return urb;
}

<<<<<<< HEAD
/* Setup urbs */
static void usb_wwan_setup_urbs(struct usb_serial *serial)
{
	int i, j;
	struct usb_serial_port *port;
	struct usb_wwan_port_private *portdata;

	dbg("%s", __func__);

	for (i = 0; i < serial->num_ports; i++) {
		port = serial->port[i];
		portdata = usb_get_serial_port_data(port);

		/* Do indat endpoints first */
		for (j = 0; j < N_IN_URB; ++j) {
			portdata->in_urbs[j] = usb_wwan_setup_urb(serial,
								  port->
								  bulk_in_endpointAddress,
								  USB_DIR_IN,
								  port,
								  portdata->
								  in_buffer[j],
								  IN_BUFLEN,
								  usb_wwan_indat_callback);
		}

		/* outdat endpoints */
		for (j = 0; j < N_OUT_URB; ++j) {
			portdata->out_urbs[j] = usb_wwan_setup_urb(serial,
								   port->
								   bulk_out_endpointAddress,
								   USB_DIR_OUT,
								   port,
								   portdata->
								   out_buffer
								   [j],
								   OUT_BUFLEN,
								   usb_wwan_outdat_callback);
		}
	}
}

int usb_wwan_startup(struct usb_serial *serial)
{
	int i, j, err;
	struct usb_serial_port *port;
	struct usb_wwan_port_private *portdata;
	u8 *buffer;

	dbg("%s", __func__);

	/* Now setup per port private data */
	for (i = 0; i < serial->num_ports; i++) {
		port = serial->port[i];
		portdata = kzalloc(sizeof(*portdata), GFP_KERNEL);
		if (!portdata) {
			dbg("%s: kmalloc for usb_wwan_port_private (%d) failed!.",
			    __func__, i);
			return 1;
		}
		init_usb_anchor(&portdata->delayed);
<<<<<<< HEAD
<<<<<<< HEAD

		for (j = 0; j < N_IN_URB; j++) {
			buffer = (u8 *) __get_free_page(GFP_KERNEL);
=======
		init_usb_anchor(&portdata->submitted);
		INIT_WORK(&portdata->in_work, usb_wwan_in_work);
		INIT_LIST_HEAD(&portdata->in_urb_list);
		spin_lock_init(&portdata->in_lock);

		for (j = 0; j < N_IN_URB; j++) {
			buffer = kmalloc(IN_BUFLEN, GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		init_usb_anchor(&portdata->submitted);
		INIT_WORK(&portdata->in_work, usb_wwan_in_work);
		INIT_LIST_HEAD(&portdata->in_urb_list);
		spin_lock_init(&portdata->in_lock);

		for (j = 0; j < N_IN_URB; j++) {
			buffer = kmalloc(IN_BUFLEN, GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-11.0
			if (!buffer)
				goto bail_out_error;
			portdata->in_buffer[j] = buffer;
		}

		for (j = 0; j < N_OUT_URB; j++) {
			buffer = kmalloc(OUT_BUFLEN, GFP_KERNEL);
			if (!buffer)
				goto bail_out_error2;
			portdata->out_buffer[j] = buffer;
		}

		usb_set_serial_port_data(port, portdata);

		if (!port->interrupt_in_urb)
			continue;
		err = usb_submit_urb(port->interrupt_in_urb, GFP_KERNEL);
		if (err)
			dbg("%s: submit irq_in urb failed %d", __func__, err);
	}
	usb_wwan_setup_urbs(serial);
	return 0;

bail_out_error2:
	for (j = 0; j < N_OUT_URB; j++)
		kfree(portdata->out_buffer[j]);
bail_out_error:
	for (j = 0; j < N_IN_URB; j++)
<<<<<<< HEAD
<<<<<<< HEAD
		if (portdata->in_buffer[j])
			free_page((unsigned long)portdata->in_buffer[j]);
=======
		kfree(portdata->in_buffer[j]);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		kfree(portdata->in_buffer[j]);
>>>>>>> refs/remotes/origin/cm-11.0
	kfree(portdata);
	return 1;
}
EXPORT_SYMBOL(usb_wwan_startup);

static void stop_read_write_urbs(struct usb_serial *serial)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int i, j;
=======
	int i;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int i;
>>>>>>> refs/remotes/origin/cm-11.0
	struct usb_serial_port *port;
	struct usb_wwan_port_private *portdata;

	/* Stop reading/writing urbs */
	for (i = 0; i < serial->num_ports; ++i) {
		port = serial->port[i];
		portdata = usb_get_serial_port_data(port);
<<<<<<< HEAD
<<<<<<< HEAD
		for (j = 0; j < N_IN_URB; j++)
			usb_kill_urb(portdata->in_urbs[j]);
		for (j = 0; j < N_OUT_URB; j++)
			usb_kill_urb(portdata->out_urbs[j]);
=======
		usb_kill_anchored_urbs(&portdata->submitted);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		usb_kill_anchored_urbs(&portdata->submitted);
>>>>>>> refs/remotes/origin/cm-11.0
	}
}

void usb_wwan_disconnect(struct usb_serial *serial)
{
	dbg("%s", __func__);

	stop_read_write_urbs(serial);
}
EXPORT_SYMBOL(usb_wwan_disconnect);

void usb_wwan_release(struct usb_serial *serial)
=======
int usb_wwan_port_probe(struct usb_serial_port *port)
{
	struct usb_wwan_port_private *portdata;
	struct urb *urb;
	u8 *buffer;
	int err;
	int i;

	portdata = kzalloc(sizeof(*portdata), GFP_KERNEL);
	if (!portdata)
		return -ENOMEM;

	init_usb_anchor(&portdata->delayed);

	for (i = 0; i < N_IN_URB; i++) {
		if (!port->bulk_in_size)
			break;

		buffer = (u8 *)__get_free_page(GFP_KERNEL);
		if (!buffer)
			goto bail_out_error;
		portdata->in_buffer[i] = buffer;

		urb = usb_wwan_setup_urb(port, port->bulk_in_endpointAddress,
						USB_DIR_IN, port,
						buffer, IN_BUFLEN,
						usb_wwan_indat_callback);
		portdata->in_urbs[i] = urb;
	}

	for (i = 0; i < N_OUT_URB; i++) {
		if (!port->bulk_out_size)
			break;

		buffer = kmalloc(OUT_BUFLEN, GFP_KERNEL);
		if (!buffer)
			goto bail_out_error2;
		portdata->out_buffer[i] = buffer;

		urb = usb_wwan_setup_urb(port, port->bulk_out_endpointAddress,
						USB_DIR_OUT, port,
						buffer, OUT_BUFLEN,
						usb_wwan_outdat_callback);
		portdata->out_urbs[i] = urb;
	}

	usb_set_serial_port_data(port, portdata);

	if (port->interrupt_in_urb) {
		err = usb_submit_urb(port->interrupt_in_urb, GFP_KERNEL);
		if (err)
			dev_dbg(&port->dev, "%s: submit irq_in urb failed %d\n",
				__func__, err);
	}

	return 0;

bail_out_error2:
	for (i = 0; i < N_OUT_URB; i++) {
		usb_free_urb(portdata->out_urbs[i]);
		kfree(portdata->out_buffer[i]);
	}
bail_out_error:
	for (i = 0; i < N_IN_URB; i++) {
		usb_free_urb(portdata->in_urbs[i]);
		free_page((unsigned long)portdata->in_buffer[i]);
	}
	kfree(portdata);

	return -ENOMEM;
}
EXPORT_SYMBOL_GPL(usb_wwan_port_probe);

int usb_wwan_port_remove(struct usb_serial_port *port)
{
	int i;
	struct usb_wwan_port_private *portdata;

	portdata = usb_get_serial_port_data(port);
	usb_set_serial_port_data(port, NULL);

	/* Stop reading/writing urbs and free them */
	for (i = 0; i < N_IN_URB; i++) {
		usb_kill_urb(portdata->in_urbs[i]);
		usb_free_urb(portdata->in_urbs[i]);
		free_page((unsigned long)portdata->in_buffer[i]);
	}
	for (i = 0; i < N_OUT_URB; i++) {
		usb_kill_urb(portdata->out_urbs[i]);
		usb_free_urb(portdata->out_urbs[i]);
		kfree(portdata->out_buffer[i]);
	}

	/* Now free port private data */
	kfree(portdata);
	return 0;
}
EXPORT_SYMBOL(usb_wwan_port_remove);

#ifdef CONFIG_PM
static void stop_read_write_urbs(struct usb_serial *serial)
>>>>>>> refs/remotes/origin/master
{
	int i, j;
	struct usb_serial_port *port;
	struct usb_wwan_port_private *portdata;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD

	dbg("%s", __func__);
=======
	struct urb *urb;
	struct list_head *q;
	unsigned long flags;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct urb *urb;
	struct list_head *q;
	unsigned long flags;
>>>>>>> refs/remotes/origin/cm-11.0

	/* Now free them */
	for (i = 0; i < serial->num_ports; ++i) {
		port = serial->port[i];
		portdata = usb_get_serial_port_data(port);

<<<<<<< HEAD
<<<<<<< HEAD
		for (j = 0; j < N_IN_URB; j++) {
			usb_free_urb(portdata->in_urbs[j]);
			free_page((unsigned long)
				  portdata->in_buffer[j]);
=======
		cancel_work_sync(&portdata->in_work);
		/* TBD: do we really need this */
		spin_lock_irqsave(&portdata->in_lock, flags);
		q = &portdata->in_urb_list;
		while (!list_empty(q)) {
			urb = list_first_entry(q, struct urb, urb_list);
			list_del_init(&urb->urb_list);
		}
		spin_unlock_irqrestore(&portdata->in_lock, flags);

		for (j = 0; j < N_IN_URB; j++) {
			usb_free_urb(portdata->in_urbs[j]);
			kfree(portdata->in_buffer[j]);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		cancel_work_sync(&portdata->in_work);
		/* TBD: do we really need this */
		spin_lock_irqsave(&portdata->in_lock, flags);
		q = &portdata->in_urb_list;
		while (!list_empty(q)) {
			urb = list_first_entry(q, struct urb, urb_list);
			list_del_init(&urb->urb_list);
		}
		spin_unlock_irqrestore(&portdata->in_lock, flags);

		for (j = 0; j < N_IN_URB; j++) {
			usb_free_urb(portdata->in_urbs[j]);
			kfree(portdata->in_buffer[j]);
>>>>>>> refs/remotes/origin/cm-11.0
			portdata->in_urbs[j] = NULL;
		}
		for (j = 0; j < N_OUT_URB; j++) {
			usb_free_urb(portdata->out_urbs[j]);
			kfree(portdata->out_buffer[j]);
			portdata->out_urbs[j] = NULL;
		}
	}

	/* Now free per port private data */
	for (i = 0; i < serial->num_ports; i++) {
		port = serial->port[i];
		kfree(usb_get_serial_port_data(port));
	}
}
EXPORT_SYMBOL(usb_wwan_release);

#ifdef CONFIG_PM
=======

	/* Stop reading/writing urbs */
	for (i = 0; i < serial->num_ports; ++i) {
		port = serial->port[i];
		portdata = usb_get_serial_port_data(port);
		if (!portdata)
			continue;
		for (j = 0; j < N_IN_URB; j++)
			usb_kill_urb(portdata->in_urbs[j]);
		for (j = 0; j < N_OUT_URB; j++)
			usb_kill_urb(portdata->out_urbs[j]);
	}
}

>>>>>>> refs/remotes/origin/master
int usb_wwan_suspend(struct usb_serial *serial, pm_message_t message)
{
	struct usb_wwan_intf_private *intfdata = serial->private;
	int b;

<<<<<<< HEAD
	dbg("%s entered", __func__);

<<<<<<< HEAD
	if (message.event & PM_EVENT_AUTO) {
=======
	if (PMSG_IS_AUTO(message)) {
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (PMSG_IS_AUTO(message)) {
>>>>>>> refs/remotes/origin/master
		spin_lock_irq(&intfdata->susp_lock);
		b = intfdata->in_flight;
		spin_unlock_irq(&intfdata->susp_lock);

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
		if (b)
=======
		if (b || pm_runtime_autosuspend_expiration(&serial->dev->dev))
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (b)
>>>>>>> refs/remotes/origin/master
=======
		if (b || pm_runtime_autosuspend_expiration(&serial->dev->dev))
>>>>>>> refs/remotes/origin/cm-11.0
			return -EBUSY;
	}

	spin_lock_irq(&intfdata->susp_lock);
	intfdata->suspended = 1;
	spin_unlock_irq(&intfdata->susp_lock);
	stop_read_write_urbs(serial);

	return 0;
}
EXPORT_SYMBOL(usb_wwan_suspend);

static void unbusy_queued_urb(struct urb *urb, struct usb_wwan_port_private *portdata)
{
	int i;

	for (i = 0; i < N_OUT_URB; i++) {
		if (urb == portdata->out_urbs[i]) {
			clear_bit(i, &portdata->out_busy);
			break;
		}
	}
}

static void play_delayed(struct usb_serial_port *port)
{
	struct usb_wwan_intf_private *data;
	struct usb_wwan_port_private *portdata;
	struct urb *urb;
	int err;

	portdata = usb_get_serial_port_data(port);
	data = port->serial->private;
	while ((urb = usb_get_from_anchor(&portdata->delayed))) {
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
		usb_anchor_urb(urb, &portdata->submitted);
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
		usb_anchor_urb(urb, &portdata->submitted);
>>>>>>> refs/remotes/origin/cm-11.0
		err = usb_submit_urb(urb, GFP_ATOMIC);
		if (!err) {
			data->in_flight++;
		} else {
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
			usb_unanchor_urb(urb);
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
			usb_unanchor_urb(urb);
>>>>>>> refs/remotes/origin/cm-11.0
			/* we have to throw away the rest */
			do {
				unbusy_queued_urb(urb, portdata);
				usb_autopm_put_interface_no_suspend(port->serial->interface);
			} while ((urb = usb_get_from_anchor(&portdata->delayed)));
			break;
		}
	}
}

int usb_wwan_resume(struct usb_serial *serial)
{
	int i, j;
	struct usb_serial_port *port;
	struct usb_wwan_intf_private *intfdata = serial->private;
	struct usb_wwan_port_private *portdata;
	struct urb *urb;
	int err = 0;

<<<<<<< HEAD
	dbg("%s entered", __func__);
=======
>>>>>>> refs/remotes/origin/master
	/* get the interrupt URBs resubmitted unconditionally */
	for (i = 0; i < serial->num_ports; i++) {
		port = serial->port[i];
		if (!port->interrupt_in_urb) {
<<<<<<< HEAD
			dbg("%s: No interrupt URB for port %d", __func__, i);
			continue;
		}
		err = usb_submit_urb(port->interrupt_in_urb, GFP_NOIO);
		dbg("Submitted interrupt URB for port %d (result %d)", i, err);
		if (err < 0) {
			err("%s: Error %d for interrupt URB of port%d",
			    __func__, err, i);
=======
			dev_dbg(&port->dev, "%s: No interrupt URB for port\n", __func__);
			continue;
		}
		err = usb_submit_urb(port->interrupt_in_urb, GFP_NOIO);
		dev_dbg(&port->dev, "Submitted interrupt URB for port (result %d)\n", err);
		if (err < 0) {
			dev_err(&port->dev, "%s: Error %d for interrupt URB\n",
				__func__, err);
>>>>>>> refs/remotes/origin/master
			goto err_out;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
	spin_lock_irq(&intfdata->susp_lock);
	intfdata->suspended = 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
	spin_lock_irq(&intfdata->susp_lock);
	intfdata->suspended = 0;
>>>>>>> refs/remotes/origin/cm-11.0
	for (i = 0; i < serial->num_ports; i++) {
		/* walk all ports */
		port = serial->port[i];
		portdata = usb_get_serial_port_data(port);

		/* skip closed ports */
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
		spin_lock_irq(&intfdata->susp_lock);
		if (!portdata->opened) {
=======
		spin_lock_irq(&intfdata->susp_lock);
		if (!portdata || !portdata->opened) {
>>>>>>> refs/remotes/origin/master
			spin_unlock_irq(&intfdata->susp_lock);
=======
		if (!portdata->opened)
>>>>>>> refs/remotes/origin/cm-11.0
			continue;

		for (j = 0; j < N_IN_URB; j++) {
			urb = portdata->in_urbs[j];

			/* don't re-submit if it already was submitted or if
			 * it is being processed by in_work */
			if (urb->anchor || !list_empty(&urb->urb_list))
				continue;

			usb_anchor_urb(urb, &portdata->submitted);
			err = usb_submit_urb(urb, GFP_ATOMIC);
			if (err < 0) {
<<<<<<< HEAD
<<<<<<< HEAD
				err("%s: Error %d for bulk URB %d",
				    __func__, err, i);
=======
		if (!portdata->opened)
			continue;

		for (j = 0; j < N_IN_URB; j++) {
			urb = portdata->in_urbs[j];

			/* don't re-submit if it already was submitted or if
			 * it is being processed by in_work */
			if (urb->anchor || !list_empty(&urb->urb_list))
				continue;

			usb_anchor_urb(urb, &portdata->submitted);
			err = usb_submit_urb(urb, GFP_ATOMIC);
			if (err < 0) {
=======
>>>>>>> refs/remotes/origin/cm-11.0
				err("%s: Error %d for bulk URB[%d]:%p %d",
				    __func__, err, j, urb, i);
				usb_unanchor_urb(urb);
				intfdata->suspended = 1;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
				dev_err(&port->dev, "%s: Error %d for bulk URB %d\n",
					__func__, err, i);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
				spin_unlock_irq(&intfdata->susp_lock);
				goto err_out;
			}
		}
		play_delayed(port);
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
		spin_unlock_irq(&intfdata->susp_lock);
=======
>>>>>>> refs/remotes/origin/cm-11.0
	}
	spin_unlock_irq(&intfdata->susp_lock);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	}
	spin_unlock_irq(&intfdata->susp_lock);

>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======

>>>>>>> refs/remotes/origin/cm-11.0
err_out:
	return err;
}
EXPORT_SYMBOL(usb_wwan_resume);
#endif

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
<<<<<<< HEAD
MODULE_VERSION(DRIVER_VERSION);
MODULE_LICENSE("GPL");

module_param(debug, bool, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(debug, "Debug messages");
=======
MODULE_LICENSE("GPL");
>>>>>>> refs/remotes/origin/master
