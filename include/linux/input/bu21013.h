/*
 * Copyright (C) ST-Ericsson SA 2010
 * Author: Naveen Kumar G <naveen.gaddipati@stericsson.com> for ST-Ericsson
 * License terms:GNU General Public License (GPL) version 2
 */

#ifndef _BU21013_H
#define _BU21013_H

/**
 * struct bu21013_platform_device - Handle the platform data
<<<<<<< HEAD
 * @cs_en:	pointer to the cs enable function
 * @cs_dis:	pointer to the cs disable function
 * @irq_read_val:    pointer to read the pen irq value function
 * @touch_x_max: touch x max
 * @touch_y_max: touch y max
 * @cs_pin: chip select pin
 * @irq: irq pin
=======
 * @touch_x_max: touch x max
 * @touch_y_max: touch y max
 * @cs_pin: chip select pin
 * @touch_pin: touch gpio pin
>>>>>>> refs/remotes/origin/master
 * @ext_clk: external clock flag
 * @x_flip: x flip flag
 * @y_flip: y flip flag
 * @wakeup: wakeup flag
 *
 * This is used to handle the platform data
 */
struct bu21013_platform_device {
<<<<<<< HEAD
	int (*cs_en)(int reset_pin);
	int (*cs_dis)(int reset_pin);
	int (*irq_read_val)(void);
	int touch_x_max;
	int touch_y_max;
	unsigned int cs_pin;
	unsigned int irq;
=======
	int touch_x_max;
	int touch_y_max;
	unsigned int cs_pin;
	unsigned int touch_pin;
>>>>>>> refs/remotes/origin/master
	bool ext_clk;
	bool x_flip;
	bool y_flip;
	bool wakeup;
};

#endif
