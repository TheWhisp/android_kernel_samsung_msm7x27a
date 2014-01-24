/*
<<<<<<< HEAD
 *  linux/arch/arm/mach-integrator/leds.c
 *
 *  Integrator/AP and Integrator/CP LED control routines
 *
 *  Copyright (C) 1999 ARM Limited
 *  Copyright (C) 2000 Deep Blue Solutions Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/smp.h>
#include <linux/spinlock.h>
#include <linux/io.h>

#include <mach/hardware.h>
#include <mach/platform.h>
#include <asm/leds.h>
<<<<<<< HEAD
#include <asm/system.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
#include <asm/mach-types.h>
#include <mach/cm.h>

static int saved_leds;

static void integrator_leds_event(led_event_t ledevt)
{
	unsigned long flags;
	const unsigned int dbg_base = IO_ADDRESS(INTEGRATOR_DBG_BASE);
	unsigned int update_alpha_leds;

	// yup, change the LEDs
	local_irq_save(flags);
	update_alpha_leds = 0;

	switch(ledevt) {
	case led_idle_start:
		cm_control(CM_CTRL_LED, 0);
		break;

	case led_idle_end:
		cm_control(CM_CTRL_LED, CM_CTRL_LED);
		break;

	case led_timer:
		saved_leds ^= GREEN_LED;
		update_alpha_leds = 1;
		break;

	case led_red_on:
		saved_leds |= RED_LED;
		update_alpha_leds = 1;
		break;

	case led_red_off:
		saved_leds &= ~RED_LED;
		update_alpha_leds = 1;
		break;

	default:
		break;
	}

	if (update_alpha_leds) {
		while (__raw_readl(dbg_base + INTEGRATOR_DBG_ALPHA_OFFSET) & 1);
		__raw_writel(saved_leds, dbg_base + INTEGRATOR_DBG_LEDS_OFFSET);
	}
	local_irq_restore(flags);
}

static int __init leds_init(void)
{
	if (machine_is_integrator() || machine_is_cintegrator())
		leds_event = integrator_leds_event;
=======
 * Driver for the 4 user LEDs found on the Integrator AP/CP baseboard
 * Based on Versatile and RealView machine LED code
 *
 * License terms: GNU General Public License (GPL) version 2
 * Author: Bryan Wu <bryan.wu@canonical.com>
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/leds.h>

#include <mach/hardware.h>
#include <mach/platform.h>

#include "cm.h"

#if defined(CONFIG_NEW_LEDS) && defined(CONFIG_LEDS_CLASS)

#define ALPHA_REG __io_address(INTEGRATOR_DBG_BASE)
#define LEDREG	(__io_address(INTEGRATOR_DBG_BASE) + INTEGRATOR_DBG_LEDS_OFFSET)

struct integrator_led {
	struct led_classdev	cdev;
	u8			mask;
};

/*
 * The triggers lines up below will only be used if the
 * LED triggers are compiled in.
 */
static const struct {
	const char *name;
	const char *trigger;
} integrator_leds[] = {
	{ "integrator:green0", "heartbeat", },
	{ "integrator:yellow", },
	{ "integrator:red", },
	{ "integrator:green1", },
	{ "integrator:core_module", "cpu0", },
};

static void integrator_led_set(struct led_classdev *cdev,
			      enum led_brightness b)
{
	struct integrator_led *led = container_of(cdev,
						 struct integrator_led, cdev);
	u32 reg = __raw_readl(LEDREG);

	if (b != LED_OFF)
		reg |= led->mask;
	else
		reg &= ~led->mask;

	while (__raw_readl(ALPHA_REG) & 1)
		cpu_relax();

	__raw_writel(reg, LEDREG);
}

static enum led_brightness integrator_led_get(struct led_classdev *cdev)
{
	struct integrator_led *led = container_of(cdev,
						 struct integrator_led, cdev);
	u32 reg = __raw_readl(LEDREG);

	return (reg & led->mask) ? LED_FULL : LED_OFF;
}

static void cm_led_set(struct led_classdev *cdev,
			      enum led_brightness b)
{
	if (b != LED_OFF)
		cm_control(CM_CTRL_LED, CM_CTRL_LED);
	else
		cm_control(CM_CTRL_LED, 0);
}

static enum led_brightness cm_led_get(struct led_classdev *cdev)
{
	u32 reg = cm_get();

	return (reg & CM_CTRL_LED) ? LED_FULL : LED_OFF;
}

static int __init integrator_leds_init(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(integrator_leds); i++) {
		struct integrator_led *led;

		led = kzalloc(sizeof(*led), GFP_KERNEL);
		if (!led)
			break;


		led->cdev.name = integrator_leds[i].name;

		if (i == 4) { /* Setting for LED in core module */
			led->cdev.brightness_set = cm_led_set;
			led->cdev.brightness_get = cm_led_get;
		} else {
			led->cdev.brightness_set = integrator_led_set;
			led->cdev.brightness_get = integrator_led_get;
		}

		led->cdev.default_trigger = integrator_leds[i].trigger;
		led->mask = BIT(i);

		if (led_classdev_register(NULL, &led->cdev) < 0) {
			kfree(led);
			break;
		}
	}
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
core_initcall(leds_init);
=======
/*
 * Since we may have triggers on any subsystem, defer registration
 * until after subsystem_init.
 */
fs_initcall(integrator_leds_init);
#endif
>>>>>>> refs/remotes/origin/master
