/*
<<<<<<< HEAD
 *  linux/arch/arm/mach-ebsa110/leds.c
 *
 *  Copyright (C) 1998 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  EBSA-110 LED control routines.  We use the led as follows:
 *
 *   - Red - toggles state every 50 timer interrupts
 */
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/init.h>

#include <mach/hardware.h>
#include <asm/leds.h>
<<<<<<< HEAD
#include <asm/system.h>
#include <asm/mach-types.h>

=======
=======
 * Driver for the LED found on the EBSA110 machine
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

>>>>>>> refs/remotes/origin/master
#include <asm/mach-types.h>

#include "core.h"

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static spinlock_t leds_lock;

static void ebsa110_leds_event(led_event_t ledevt)
{
	unsigned long flags;

	spin_lock_irqsave(&leds_lock, flags);

	switch(ledevt) {
	case led_timer:
		*(volatile unsigned char *)SOFT_BASE ^= 128;
		break;

	default:
		break;
	}

	spin_unlock_irqrestore(&leds_lock, flags);
}

static int __init leds_init(void)
{
	if (machine_is_ebsa110())
		leds_event = ebsa110_leds_event;
=======
#if defined(CONFIG_NEW_LEDS) && defined(CONFIG_LEDS_CLASS)
static void ebsa110_led_set(struct led_classdev *cdev,
			      enum led_brightness b)
{
	u8 reg = __raw_readb(SOFT_BASE);

	if (b != LED_OFF)
		reg |= 0x80;
	else
		reg &= ~0x80;

	__raw_writeb(reg, SOFT_BASE);
}

static enum led_brightness ebsa110_led_get(struct led_classdev *cdev)
{
	u8 reg = __raw_readb(SOFT_BASE);

	return (reg & 0x80) ? LED_FULL : LED_OFF;
}

static int __init ebsa110_leds_init(void)
{

	struct led_classdev *cdev;
	int ret;

	if (!machine_is_ebsa110())
		return -ENODEV;

	cdev = kzalloc(sizeof(*cdev), GFP_KERNEL);
	if (!cdev)
		return -ENOMEM;

	cdev->name = "ebsa110:0";
	cdev->brightness_set = ebsa110_led_set;
	cdev->brightness_get = ebsa110_led_get;
	cdev->default_trigger = "heartbeat";

	ret = led_classdev_register(NULL, cdev);
	if (ret	< 0) {
		kfree(cdev);
		return ret;
	}
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
__initcall(leds_init);
=======
/*
 * Since we may have triggers on any subsystem, defer registration
 * until after subsystem_init.
 */
fs_initcall(ebsa110_leds_init);
#endif
>>>>>>> refs/remotes/origin/master
