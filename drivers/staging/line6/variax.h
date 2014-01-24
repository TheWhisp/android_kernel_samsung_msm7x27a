/*
 * Line6 Linux USB driver - 0.9.1beta
 *
 * Copyright (C) 2004-2010 Markus Grabner (grabner@icg.tugraz.at)
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation, version 2.
 *
 */

#ifndef VARIAX_H
#define VARIAX_H

#include <linux/spinlock.h>
#include <linux/usb.h>
#include <linux/wait.h>
#include <sound/core.h>

#include "driver.h"
<<<<<<< HEAD
#include "dumprequest.h"
=======
>>>>>>> refs/remotes/origin/master

#define VARIAX_STARTUP_DELAY1 1000
#define VARIAX_STARTUP_DELAY3 100
#define VARIAX_STARTUP_DELAY4 100

/*
	Stages of Variax startup procedure
*/
enum {
	VARIAX_STARTUP_INIT = 1,
	VARIAX_STARTUP_VERSIONREQ,
	VARIAX_STARTUP_WAIT,
	VARIAX_STARTUP_ACTIVATE,
<<<<<<< HEAD
	VARIAX_STARTUP_DUMPREQ,
=======
>>>>>>> refs/remotes/origin/master
	VARIAX_STARTUP_WORKQUEUE,
	VARIAX_STARTUP_SETUP,
	VARIAX_STARTUP_LAST = VARIAX_STARTUP_SETUP - 1
};

<<<<<<< HEAD
enum {
	VARIAX_DUMP_PASS1 = LINE6_DUMP_CURRENT,
	VARIAX_DUMP_PASS2,
	VARIAX_DUMP_PASS3
};

/**
	Binary Variax model dump
*/
struct variax_model {
	/**
		Header information (including program name).
	*/
	unsigned char name[18];

	/**
		Model parameters.
	*/
	unsigned char control[78 * 2];
};

=======
>>>>>>> refs/remotes/origin/master
struct usb_line6_variax {
	/**
		Generic Line6 USB data.
	*/
	struct usb_line6 line6;

	/**
<<<<<<< HEAD
		Dump request structure.
		Append two extra buffers for 3-pass data query.
	*/
	struct line6_dump_request dumpreq;
	struct line6_dump_reqbuf extrabuf[2];

	/**
=======
>>>>>>> refs/remotes/origin/master
		Buffer for activation code.
	*/
	unsigned char *buffer_activate;

	/**
<<<<<<< HEAD
		Model number.
	*/
	int model;

	/**
		Current model settings.
	*/
	struct variax_model model_data;

	/**
		Name of connected guitar.
	*/
	unsigned char guitar[18];

	/**
		Name of current model bank.
	*/
	unsigned char bank[18];

	/**
		Position of volume dial.
	*/
	int volume;

	/**
		Position of tone control dial.
	*/
	int tone;

	/**
=======
>>>>>>> refs/remotes/origin/master
		Handler for device initializaton.
	*/
	struct work_struct startup_work;

	/**
		Timers for device initializaton.
	*/
	struct timer_list startup_timer1;
	struct timer_list startup_timer2;

	/**
		Current progress in startup procedure.
	*/
	int startup_progress;
};

extern void line6_variax_disconnect(struct usb_interface *interface);
extern int line6_variax_init(struct usb_interface *interface,
			     struct usb_line6_variax *variax);
extern void line6_variax_process_message(struct usb_line6_variax *variax);

#endif
