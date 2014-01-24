/*
 * LED Class Core
 *
 * Copyright 2005-2006 Openedhand Ltd.
 *
 * Author: Richard Purdie <rpurdie@openedhand.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/rwsem.h>
#include <linux/leds.h>
#include "leds.h"

DECLARE_RWSEM(leds_list_lock);
EXPORT_SYMBOL_GPL(leds_list_lock);

LIST_HEAD(leds_list);
EXPORT_SYMBOL_GPL(leds_list);
<<<<<<< HEAD
<<<<<<< HEAD
=======

static void led_stop_software_blink(struct led_classdev *led_cdev)
{
	/* deactivate previous settings */
	del_timer_sync(&led_cdev->blink_timer);
	led_cdev->blink_delay_on = 0;
	led_cdev->blink_delay_off = 0;
}
=======
>>>>>>> refs/remotes/origin/master

static void led_set_software_blink(struct led_classdev *led_cdev,
				   unsigned long delay_on,
				   unsigned long delay_off)
{
	int current_brightness;

	current_brightness = led_get_brightness(led_cdev);
	if (current_brightness)
		led_cdev->blink_brightness = current_brightness;
	if (!led_cdev->blink_brightness)
		led_cdev->blink_brightness = led_cdev->max_brightness;

<<<<<<< HEAD
	if (led_get_trigger_data(led_cdev) &&
	    delay_on == led_cdev->blink_delay_on &&
	    delay_off == led_cdev->blink_delay_off)
		return;

	led_stop_software_blink(led_cdev);

=======
>>>>>>> refs/remotes/origin/master
	led_cdev->blink_delay_on = delay_on;
	led_cdev->blink_delay_off = delay_off;

	/* never on - don't blink */
	if (!delay_on)
		return;

	/* never off - just set to brightness */
	if (!delay_off) {
<<<<<<< HEAD
		led_set_brightness(led_cdev, led_cdev->blink_brightness);
=======
		__led_set_brightness(led_cdev, led_cdev->blink_brightness);
>>>>>>> refs/remotes/origin/master
		return;
	}

	mod_timer(&led_cdev->blink_timer, jiffies + 1);
}


<<<<<<< HEAD
void led_blink_set(struct led_classdev *led_cdev,
		   unsigned long *delay_on,
		   unsigned long *delay_off)
{
	del_timer_sync(&led_cdev->blink_timer);

	if (led_cdev->blink_set &&
=======
static void led_blink_setup(struct led_classdev *led_cdev,
		     unsigned long *delay_on,
		     unsigned long *delay_off)
{
	if (!(led_cdev->flags & LED_BLINK_ONESHOT) &&
	    led_cdev->blink_set &&
>>>>>>> refs/remotes/origin/master
	    !led_cdev->blink_set(led_cdev, delay_on, delay_off))
		return;

	/* blink with 1 Hz as default if nothing specified */
	if (!*delay_on && !*delay_off)
		*delay_on = *delay_off = 500;

	led_set_software_blink(led_cdev, *delay_on, *delay_off);
}
<<<<<<< HEAD
EXPORT_SYMBOL(led_blink_set);

void led_brightness_set(struct led_classdev *led_cdev,
			enum led_brightness brightness)
{
	led_stop_software_blink(led_cdev);
	led_cdev->brightness_set(led_cdev, brightness);
}
EXPORT_SYMBOL(led_brightness_set);
>>>>>>> refs/remotes/origin/cm-10.0
=======

void led_blink_set(struct led_classdev *led_cdev,
		   unsigned long *delay_on,
		   unsigned long *delay_off)
{
	del_timer_sync(&led_cdev->blink_timer);

	led_cdev->flags &= ~LED_BLINK_ONESHOT;
	led_cdev->flags &= ~LED_BLINK_ONESHOT_STOP;

	led_blink_setup(led_cdev, delay_on, delay_off);
}
EXPORT_SYMBOL(led_blink_set);

void led_blink_set_oneshot(struct led_classdev *led_cdev,
			   unsigned long *delay_on,
			   unsigned long *delay_off,
			   int invert)
{
	if ((led_cdev->flags & LED_BLINK_ONESHOT) &&
	     timer_pending(&led_cdev->blink_timer))
		return;

	led_cdev->flags |= LED_BLINK_ONESHOT;
	led_cdev->flags &= ~LED_BLINK_ONESHOT_STOP;

	if (invert)
		led_cdev->flags |= LED_BLINK_INVERT;
	else
		led_cdev->flags &= ~LED_BLINK_INVERT;

	led_blink_setup(led_cdev, delay_on, delay_off);
}
EXPORT_SYMBOL(led_blink_set_oneshot);

void led_stop_software_blink(struct led_classdev *led_cdev)
{
	del_timer_sync(&led_cdev->blink_timer);
	led_cdev->blink_delay_on = 0;
	led_cdev->blink_delay_off = 0;
}
EXPORT_SYMBOL_GPL(led_stop_software_blink);

void led_set_brightness(struct led_classdev *led_cdev,
			enum led_brightness brightness)
{
	/* delay brightness setting if need to stop soft-blink timer */
	if (led_cdev->blink_delay_on || led_cdev->blink_delay_off) {
		led_cdev->delayed_set_value = brightness;
		schedule_work(&led_cdev->set_brightness_work);
		return;
	}

	__led_set_brightness(led_cdev, brightness);
}
EXPORT_SYMBOL(led_set_brightness);
>>>>>>> refs/remotes/origin/master
