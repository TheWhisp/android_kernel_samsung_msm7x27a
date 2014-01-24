/*
 * LED Triggers Core
 *
 * Copyright 2005-2007 Openedhand Ltd.
 *
 * Author: Richard Purdie <rpurdie@openedhand.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/device.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/sysdev.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <linux/timer.h>
#include <linux/rwsem.h>
#include <linux/leds.h>
#include <linux/slab.h>
#include "leds.h"

/*
 * Nests outside led_cdev->trigger_lock
 */
static DECLARE_RWSEM(triggers_list_lock);
static LIST_HEAD(trigger_list);

 /* Used by LED Class */

ssize_t led_trigger_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t count)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	char trigger_name[TRIG_NAME_MAX];
	struct led_trigger *trig;
	size_t len;

	trigger_name[sizeof(trigger_name) - 1] = '\0';
	strncpy(trigger_name, buf, sizeof(trigger_name) - 1);
	len = strlen(trigger_name);

	if (len && trigger_name[len - 1] == '\n')
		trigger_name[len - 1] = '\0';

	if (!strcmp(trigger_name, "none")) {
		led_trigger_remove(led_cdev);
		return count;
	}

	down_read(&triggers_list_lock);
	list_for_each_entry(trig, &trigger_list, next_trig) {
		if (!strcmp(trigger_name, trig->name)) {
			down_write(&led_cdev->trigger_lock);
			led_trigger_set(led_cdev, trig);
			up_write(&led_cdev->trigger_lock);

			up_read(&triggers_list_lock);
			return count;
		}
	}
	up_read(&triggers_list_lock);

	return -EINVAL;
}
EXPORT_SYMBOL_GPL(led_trigger_store);

ssize_t led_trigger_show(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct led_trigger *trig;
	int len = 0;

	down_read(&triggers_list_lock);
	down_read(&led_cdev->trigger_lock);

	if (!led_cdev->trigger)
		len += sprintf(buf+len, "[none] ");
	else
		len += sprintf(buf+len, "none ");

	list_for_each_entry(trig, &trigger_list, next_trig) {
		if (led_cdev->trigger && !strcmp(led_cdev->trigger->name,
							trig->name))
			len += sprintf(buf+len, "[%s] ", trig->name);
		else
			len += sprintf(buf+len, "%s ", trig->name);
	}
	up_read(&led_cdev->trigger_lock);
	up_read(&triggers_list_lock);

	len += sprintf(len+buf, "\n");
	return len;
}
EXPORT_SYMBOL_GPL(led_trigger_show);

/* Caller must ensure led_cdev->trigger_lock held */
<<<<<<< HEAD
void led_trigger_set(struct led_classdev *led_cdev, struct led_trigger *trigger)
{
	unsigned long flags;
=======
void led_trigger_set(struct led_classdev *led_cdev, struct led_trigger *trig)
{
	unsigned long flags;
	char *event = NULL;
	char *envp[2];
	const char *name;

	name = trig ? trig->name : "none";
	event = kasprintf(GFP_KERNEL, "TRIGGER=%s", name);
>>>>>>> refs/remotes/origin/master

	/* Remove any existing trigger */
	if (led_cdev->trigger) {
		write_lock_irqsave(&led_cdev->trigger->leddev_list_lock, flags);
		list_del(&led_cdev->trig_list);
		write_unlock_irqrestore(&led_cdev->trigger->leddev_list_lock,
			flags);
<<<<<<< HEAD
		if (led_cdev->trigger->deactivate)
			led_cdev->trigger->deactivate(led_cdev);
		led_cdev->trigger = NULL;
		led_brightness_set(led_cdev, LED_OFF);
	}
	if (trigger) {
		write_lock_irqsave(&trigger->leddev_list_lock, flags);
		list_add_tail(&led_cdev->trig_list, &trigger->led_cdevs);
		write_unlock_irqrestore(&trigger->leddev_list_lock, flags);
		led_cdev->trigger = trigger;
		if (trigger->activate)
			trigger->activate(led_cdev);
=======
		cancel_work_sync(&led_cdev->set_brightness_work);
		led_stop_software_blink(led_cdev);
		if (led_cdev->trigger->deactivate)
			led_cdev->trigger->deactivate(led_cdev);
		led_cdev->trigger = NULL;
		led_set_brightness(led_cdev, LED_OFF);
	}
	if (trig) {
		write_lock_irqsave(&trig->leddev_list_lock, flags);
		list_add_tail(&led_cdev->trig_list, &trig->led_cdevs);
		write_unlock_irqrestore(&trig->leddev_list_lock, flags);
		led_cdev->trigger = trig;
		if (trig->activate)
			trig->activate(led_cdev);
	}

	if (event) {
		envp[0] = event;
		envp[1] = NULL;
		kobject_uevent_env(&led_cdev->dev->kobj, KOBJ_CHANGE, envp);
		kfree(event);
>>>>>>> refs/remotes/origin/master
	}
}
EXPORT_SYMBOL_GPL(led_trigger_set);

void led_trigger_remove(struct led_classdev *led_cdev)
{
	down_write(&led_cdev->trigger_lock);
	led_trigger_set(led_cdev, NULL);
	up_write(&led_cdev->trigger_lock);
}
EXPORT_SYMBOL_GPL(led_trigger_remove);

void led_trigger_set_default(struct led_classdev *led_cdev)
{
	struct led_trigger *trig;

	if (!led_cdev->default_trigger)
		return;

	down_read(&triggers_list_lock);
	down_write(&led_cdev->trigger_lock);
	list_for_each_entry(trig, &trigger_list, next_trig) {
		if (!strcmp(led_cdev->default_trigger, trig->name))
			led_trigger_set(led_cdev, trig);
	}
	up_write(&led_cdev->trigger_lock);
	up_read(&triggers_list_lock);
}
EXPORT_SYMBOL_GPL(led_trigger_set_default);

<<<<<<< HEAD
/* LED Trigger Interface */

int led_trigger_register(struct led_trigger *trigger)
{
	struct led_classdev *led_cdev;
	struct led_trigger *trig;

	rwlock_init(&trigger->leddev_list_lock);
	INIT_LIST_HEAD(&trigger->led_cdevs);

	down_write(&triggers_list_lock);
	/* Make sure the trigger's name isn't already in use */
	list_for_each_entry(trig, &trigger_list, next_trig) {
		if (!strcmp(trig->name, trigger->name)) {
=======
void led_trigger_rename_static(const char *name, struct led_trigger *trig)
{
	/* new name must be on a temporary string to prevent races */
	BUG_ON(name == trig->name);

	down_write(&triggers_list_lock);
	/* this assumes that trig->name was originaly allocated to
	 * non constant storage */
	strcpy((char *)trig->name, name);
	up_write(&triggers_list_lock);
}
EXPORT_SYMBOL_GPL(led_trigger_rename_static);

/* LED Trigger Interface */

int led_trigger_register(struct led_trigger *trig)
{
	struct led_classdev *led_cdev;
	struct led_trigger *_trig;

	rwlock_init(&trig->leddev_list_lock);
	INIT_LIST_HEAD(&trig->led_cdevs);

	down_write(&triggers_list_lock);
	/* Make sure the trigger's name isn't already in use */
	list_for_each_entry(_trig, &trigger_list, next_trig) {
		if (!strcmp(_trig->name, trig->name)) {
>>>>>>> refs/remotes/origin/master
			up_write(&triggers_list_lock);
			return -EEXIST;
		}
	}
	/* Add to the list of led triggers */
<<<<<<< HEAD
	list_add_tail(&trigger->next_trig, &trigger_list);
=======
	list_add_tail(&trig->next_trig, &trigger_list);
>>>>>>> refs/remotes/origin/master
	up_write(&triggers_list_lock);

	/* Register with any LEDs that have this as a default trigger */
	down_read(&leds_list_lock);
	list_for_each_entry(led_cdev, &leds_list, node) {
		down_write(&led_cdev->trigger_lock);
		if (!led_cdev->trigger && led_cdev->default_trigger &&
<<<<<<< HEAD
			    !strcmp(led_cdev->default_trigger, trigger->name))
			led_trigger_set(led_cdev, trigger);
=======
			    !strcmp(led_cdev->default_trigger, trig->name))
			led_trigger_set(led_cdev, trig);
>>>>>>> refs/remotes/origin/master
		up_write(&led_cdev->trigger_lock);
	}
	up_read(&leds_list_lock);

	return 0;
}
EXPORT_SYMBOL_GPL(led_trigger_register);

<<<<<<< HEAD
void led_trigger_unregister(struct led_trigger *trigger)
=======
void led_trigger_unregister(struct led_trigger *trig)
>>>>>>> refs/remotes/origin/master
{
	struct led_classdev *led_cdev;

	/* Remove from the list of led triggers */
	down_write(&triggers_list_lock);
<<<<<<< HEAD
	list_del(&trigger->next_trig);
=======
	list_del(&trig->next_trig);
>>>>>>> refs/remotes/origin/master
	up_write(&triggers_list_lock);

	/* Remove anyone actively using this trigger */
	down_read(&leds_list_lock);
	list_for_each_entry(led_cdev, &leds_list, node) {
		down_write(&led_cdev->trigger_lock);
<<<<<<< HEAD
		if (led_cdev->trigger == trigger)
=======
		if (led_cdev->trigger == trig)
>>>>>>> refs/remotes/origin/master
			led_trigger_set(led_cdev, NULL);
		up_write(&led_cdev->trigger_lock);
	}
	up_read(&leds_list_lock);
}
EXPORT_SYMBOL_GPL(led_trigger_unregister);

/* Simple LED Tigger Interface */

<<<<<<< HEAD
void led_trigger_event(struct led_trigger *trigger,
=======
void led_trigger_event(struct led_trigger *trig,
>>>>>>> refs/remotes/origin/master
			enum led_brightness brightness)
{
	struct list_head *entry;

<<<<<<< HEAD
	if (!trigger)
		return;

	read_lock(&trigger->leddev_list_lock);
	list_for_each(entry, &trigger->led_cdevs) {
=======
	if (!trig)
		return;

	read_lock(&trig->leddev_list_lock);
	list_for_each(entry, &trig->led_cdevs) {
>>>>>>> refs/remotes/origin/master
		struct led_classdev *led_cdev;

		led_cdev = list_entry(entry, struct led_classdev, trig_list);
		led_set_brightness(led_cdev, brightness);
	}
<<<<<<< HEAD
	read_unlock(&trigger->leddev_list_lock);
}
EXPORT_SYMBOL_GPL(led_trigger_event);

void led_trigger_blink(struct led_trigger *trigger,
		       unsigned long *delay_on,
		       unsigned long *delay_off)
{
	struct list_head *entry;

	if (!trigger)
		return;

	read_lock(&trigger->leddev_list_lock);
	list_for_each(entry, &trigger->led_cdevs) {
		struct led_classdev *led_cdev;

		led_cdev = list_entry(entry, struct led_classdev, trig_list);
		led_blink_set(led_cdev, delay_on, delay_off);
	}
	read_unlock(&trigger->leddev_list_lock);
}
EXPORT_SYMBOL_GPL(led_trigger_blink);

void led_trigger_register_simple(const char *name, struct led_trigger **tp)
{
	struct led_trigger *trigger;
	int err;

	trigger = kzalloc(sizeof(struct led_trigger), GFP_KERNEL);

	if (trigger) {
		trigger->name = name;
		err = led_trigger_register(trigger);
<<<<<<< HEAD
		if (err < 0)
			printk(KERN_WARNING "LED trigger %s failed to register"
				" (%d)\n", name, err);
=======
		if (err < 0) {
			kfree(trigger);
			trigger = NULL;
			printk(KERN_WARNING "LED trigger %s failed to register"
				" (%d)\n", name, err);
		}
>>>>>>> refs/remotes/origin/cm-10.0
	} else
		printk(KERN_WARNING "LED trigger %s failed to register"
			" (no memory)\n", name);

	*tp = trigger;
}
EXPORT_SYMBOL_GPL(led_trigger_register_simple);

void led_trigger_unregister_simple(struct led_trigger *trigger)
{
	if (trigger)
		led_trigger_unregister(trigger);
	kfree(trigger);
=======
	read_unlock(&trig->leddev_list_lock);
}
EXPORT_SYMBOL_GPL(led_trigger_event);

static void led_trigger_blink_setup(struct led_trigger *trig,
			     unsigned long *delay_on,
			     unsigned long *delay_off,
			     int oneshot,
			     int invert)
{
	struct list_head *entry;

	if (!trig)
		return;

	read_lock(&trig->leddev_list_lock);
	list_for_each(entry, &trig->led_cdevs) {
		struct led_classdev *led_cdev;

		led_cdev = list_entry(entry, struct led_classdev, trig_list);
		if (oneshot)
			led_blink_set_oneshot(led_cdev, delay_on, delay_off,
					      invert);
		else
			led_blink_set(led_cdev, delay_on, delay_off);
	}
	read_unlock(&trig->leddev_list_lock);
}

void led_trigger_blink(struct led_trigger *trig,
		       unsigned long *delay_on,
		       unsigned long *delay_off)
{
	led_trigger_blink_setup(trig, delay_on, delay_off, 0, 0);
}
EXPORT_SYMBOL_GPL(led_trigger_blink);

void led_trigger_blink_oneshot(struct led_trigger *trig,
			       unsigned long *delay_on,
			       unsigned long *delay_off,
			       int invert)
{
	led_trigger_blink_setup(trig, delay_on, delay_off, 1, invert);
}
EXPORT_SYMBOL_GPL(led_trigger_blink_oneshot);

void led_trigger_register_simple(const char *name, struct led_trigger **tp)
{
	struct led_trigger *trig;
	int err;

	trig = kzalloc(sizeof(struct led_trigger), GFP_KERNEL);

	if (trig) {
		trig->name = name;
		err = led_trigger_register(trig);
		if (err < 0) {
			kfree(trig);
			trig = NULL;
			pr_warn("LED trigger %s failed to register (%d)\n",
				name, err);
		}
	} else {
		pr_warn("LED trigger %s failed to register (no memory)\n",
			name);
	}
	*tp = trig;
}
EXPORT_SYMBOL_GPL(led_trigger_register_simple);

void led_trigger_unregister_simple(struct led_trigger *trig)
{
	if (trig)
		led_trigger_unregister(trig);
	kfree(trig);
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL_GPL(led_trigger_unregister_simple);

MODULE_AUTHOR("Richard Purdie");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LED Triggers Core");
