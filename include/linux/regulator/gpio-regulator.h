<<<<<<< HEAD
<<<<<<< HEAD
/* Copyright (c) 2011, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __GPIO_REGULATOR_H__
#define __GPIO_REGULATOR_H__

#include <linux/regulator/machine.h>

#define GPIO_REGULATOR_DEV_NAME "gpio-regulator"

/**
 * struct gpio_regulator_platform_data - GPIO regulator platform data
 * @init_data:		regulator constraints
 * @gpio_label:		label to use when requesting the GPIO
 * @regulator_name:	name for regulator used during registration
 * @gpio:		gpio number
 * @active_low:		0 = regulator is enabled when GPIO outputs high
 *			1 = regulator is enabled when GPIO outputs low
 */
struct gpio_regulator_platform_data {
	struct regulator_init_data	init_data;
	char				*gpio_label;
	char				*regulator_name;
	unsigned			gpio;
	int				active_low;
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * gpio-regulator.h
 *
 * Copyright 2011 Heiko Stuebner <heiko@sntech.de>
 *
 * based on fixed.h
 *
 * Copyright 2008 Wolfson Microelectronics PLC.
 *
 * Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
 *
 * Copyright (c) 2009 Nokia Corporation
 * Roger Quadros <ext-roger.quadros@nokia.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 */

#ifndef __REGULATOR_GPIO_H
#define __REGULATOR_GPIO_H

struct regulator_init_data;

enum regulator_type;

/**
 * struct gpio_regulator_state - state description
 * @value:		microvolts or microamps
 * @gpios:		bitfield of gpio target-states for the value
 *
 * This structure describes a supported setting of the regulator
 * and the necessary gpio-state to achieve it.
 *
 * The n-th bit in the bitfield describes the state of the n-th GPIO
 * from the gpios-array defined in gpio_regulator_config below.
 */
struct gpio_regulator_state {
	int value;
	int gpios;
};

/**
 * struct gpio_regulator_config - config structure
 * @supply_name:	Name of the regulator supply
 * @enable_gpio:	GPIO to use for enable control
 *			set to -EINVAL if not used
 * @enable_high:	Polarity of enable GPIO
 *			1 = Active high, 0 = Active low
 * @enabled_at_boot:	Whether regulator has been enabled at
 *			boot or not. 1 = Yes, 0 = No
 *			This is used to keep the regulator at
 *			the default state
 * @startup_delay:	Start-up time in microseconds
 * @gpios:		Array containing the gpios needed to control
 *			the setting of the regulator
 * @nr_gpios:		Number of gpios
 * @states:		Array of gpio_regulator_state entries describing
 *			the gpio state for specific voltages
 * @nr_states:		Number of states available
 * @regulator_type:	either REGULATOR_CURRENT or REGULATOR_VOLTAGE
 * @init_data:		regulator_init_data
 *
 * This structure contains gpio-voltage regulator configuration
 * information that must be passed by platform code to the
 * gpio-voltage regulator driver.
 */
struct gpio_regulator_config {
	const char *supply_name;

	int enable_gpio;
	unsigned enable_high:1;
	unsigned enabled_at_boot:1;
	unsigned startup_delay;

	struct gpio *gpios;
	int nr_gpios;

	struct gpio_regulator_state *states;
	int nr_states;

	enum regulator_type type;
	struct regulator_init_data *init_data;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

#endif
