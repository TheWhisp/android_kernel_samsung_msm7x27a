/* drivers/rtc/alarm-dev.c
 *
 * Copyright (C) 2007-2009 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/time.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/uaccess.h>
#include <linux/alarmtimer.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/wakelock.h>
=======
>>>>>>> refs/remotes/origin/master
=======
#include <linux/wakelock.h>
>>>>>>> refs/remotes/origin/cm-11.0
#include "android_alarm.h"

#define ANDROID_ALARM_PRINT_INFO (1U << 0)
#define ANDROID_ALARM_PRINT_IO (1U << 1)
#define ANDROID_ALARM_PRINT_INT (1U << 2)

static int debug_mask = ANDROID_ALARM_PRINT_INFO;
module_param_named(debug_mask, debug_mask, int, S_IRUGO | S_IWUSR | S_IWGRP);

<<<<<<< HEAD
#define pr_alarm(debug_level_mask, args...) \
	do { \
		if (debug_mask & ANDROID_ALARM_PRINT_##debug_level_mask) { \
			pr_info(args); \
		} \
	} while (0)
=======
#define alarm_dbg(debug_level_mask, fmt, ...)				\
do {									\
	if (debug_mask & ANDROID_ALARM_PRINT_##debug_level_mask)	\
		pr_info(fmt, ##__VA_ARGS__);				\
} while (0)
>>>>>>> refs/remotes/origin/master

#define ANDROID_ALARM_WAKEUP_MASK ( \
	ANDROID_ALARM_RTC_WAKEUP_MASK | \
	ANDROID_ALARM_ELAPSED_REALTIME_WAKEUP_MASK)

<<<<<<< HEAD
/* support old usespace code */
#define ANDROID_ALARM_SET_OLD               _IOW('a', 2, time_t) /* set alarm */
#define ANDROID_ALARM_SET_AND_WAIT_OLD      _IOW('a', 3, time_t)

static int alarm_opened;
static DEFINE_SPINLOCK(alarm_slock);
static struct wake_lock alarm_wake_lock;
=======
static int alarm_opened;
static DEFINE_SPINLOCK(alarm_slock);
static struct wakeup_source alarm_wake_lock;
>>>>>>> refs/remotes/origin/master
static DECLARE_WAIT_QUEUE_HEAD(alarm_wait_queue);
static uint32_t alarm_pending;
static uint32_t alarm_enabled;
static uint32_t wait_pending;

struct devalarm {
	union {
		struct hrtimer hrt;
		struct alarm alrm;
	} u;
	enum android_alarm_type type;
};

static struct devalarm alarms[ANDROID_ALARM_TYPE_COUNT];

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0

static int is_wakeup(enum android_alarm_type type)
{
	if (type == ANDROID_ALARM_RTC_WAKEUP ||
			type == ANDROID_ALARM_ELAPSED_REALTIME_WAKEUP)
		return 1;
	return 0;
}


<<<<<<< HEAD
=======
/**
 * is_wakeup() - Checks to see if this alarm can wake the device
 * @type:	 The type of alarm being checked
 *
 * Return: 1 if this is a wakeup alarm, otherwise 0
 */
static int is_wakeup(enum android_alarm_type type)
{
	return type == ANDROID_ALARM_RTC_WAKEUP ||
		type == ANDROID_ALARM_ELAPSED_REALTIME_WAKEUP;
}

>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
static void devalarm_start(struct devalarm *alrm, ktime_t exp)
{
	if (is_wakeup(alrm->type))
		alarm_start(&alrm->u.alrm, exp);
	else
		hrtimer_start(&alrm->u.hrt, exp, HRTIMER_MODE_ABS);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0

static int devalarm_try_to_cancel(struct devalarm *alrm)
{
	int ret;
	if (is_wakeup(alrm->type))
		ret = alarm_try_to_cancel(&alrm->u.alrm);
	else
		ret = hrtimer_try_to_cancel(&alrm->u.hrt);
	return ret;
<<<<<<< HEAD
=======
static int devalarm_try_to_cancel(struct devalarm *alrm)
{
	if (is_wakeup(alrm->type))
		return alarm_try_to_cancel(&alrm->u.alrm);
	return hrtimer_try_to_cancel(&alrm->u.hrt);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
}

static void devalarm_cancel(struct devalarm *alrm)
{
	if (is_wakeup(alrm->type))
		alarm_cancel(&alrm->u.alrm);
	else
		hrtimer_cancel(&alrm->u.hrt);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0

static long alarm_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int rv = 0;
	unsigned long flags;
	struct timespec new_alarm_time;
	struct timespec new_rtc_time;
	struct timespec tmp_time;
	struct rtc_time new_rtc_tm;
	struct rtc_device *rtc_dev;
	enum android_alarm_type alarm_type = ANDROID_ALARM_IOCTL_TO_TYPE(cmd);
	uint32_t alarm_type_mask = 1U << alarm_type;
=======
static void alarm_clear(enum android_alarm_type alarm_type)
{
	uint32_t alarm_type_mask = 1U << alarm_type;
	unsigned long flags;

	spin_lock_irqsave(&alarm_slock, flags);
	alarm_dbg(IO, "alarm %d clear\n", alarm_type);
	devalarm_try_to_cancel(&alarms[alarm_type]);
	if (alarm_pending) {
		alarm_pending &= ~alarm_type_mask;
		if (!alarm_pending && !wait_pending)
			__pm_relax(&alarm_wake_lock);
	}
	alarm_enabled &= ~alarm_type_mask;
	spin_unlock_irqrestore(&alarm_slock, flags);
}

static void alarm_set(enum android_alarm_type alarm_type,
							struct timespec *ts)
{
	uint32_t alarm_type_mask = 1U << alarm_type;
	unsigned long flags;

	spin_lock_irqsave(&alarm_slock, flags);
	alarm_dbg(IO, "alarm %d set %ld.%09ld\n",
			alarm_type, ts->tv_sec, ts->tv_nsec);
	alarm_enabled |= alarm_type_mask;
	devalarm_start(&alarms[alarm_type], timespec_to_ktime(*ts));
	spin_unlock_irqrestore(&alarm_slock, flags);
}

static int alarm_wait(void)
{
	unsigned long flags;
	int rv = 0;

	spin_lock_irqsave(&alarm_slock, flags);
	alarm_dbg(IO, "alarm wait\n");
	if (!alarm_pending && wait_pending) {
		__pm_relax(&alarm_wake_lock);
		wait_pending = 0;
	}
	spin_unlock_irqrestore(&alarm_slock, flags);

	rv = wait_event_interruptible(alarm_wait_queue, alarm_pending);
	if (rv)
		return rv;

	spin_lock_irqsave(&alarm_slock, flags);
	rv = alarm_pending;
	wait_pending = 1;
	alarm_pending = 0;
	spin_unlock_irqrestore(&alarm_slock, flags);

	return rv;
}

static int alarm_set_rtc(struct timespec *ts)
{
	struct rtc_time new_rtc_tm;
	struct rtc_device *rtc_dev;
	unsigned long flags;
	int rv = 0;

	rtc_time_to_tm(ts->tv_sec, &new_rtc_tm);
	rtc_dev = alarmtimer_get_rtcdev();
	rv = do_settimeofday(ts);
	if (rv < 0)
		return rv;
	if (rtc_dev)
		rv = rtc_set_time(rtc_dev, &new_rtc_tm);

	spin_lock_irqsave(&alarm_slock, flags);
	alarm_pending |= ANDROID_ALARM_TIME_CHANGE_MASK;
	wake_up(&alarm_wait_queue);
	spin_unlock_irqrestore(&alarm_slock, flags);

	return rv;
}

static int alarm_get_time(enum android_alarm_type alarm_type,
							struct timespec *ts)
{
	int rv = 0;

	switch (alarm_type) {
	case ANDROID_ALARM_RTC_WAKEUP:
	case ANDROID_ALARM_RTC:
		getnstimeofday(ts);
		break;
	case ANDROID_ALARM_ELAPSED_REALTIME_WAKEUP:
	case ANDROID_ALARM_ELAPSED_REALTIME:
		get_monotonic_boottime(ts);
		break;
	case ANDROID_ALARM_SYSTEMTIME:
		ktime_get_ts(ts);
		break;
	default:
		rv = -EINVAL;
	}
	return rv;
}

static long alarm_do_ioctl(struct file *file, unsigned int cmd,
							struct timespec *ts)
{
	int rv = 0;
	unsigned long flags;
	enum android_alarm_type alarm_type = ANDROID_ALARM_IOCTL_TO_TYPE(cmd);
>>>>>>> refs/remotes/origin/master

	if (alarm_type >= ANDROID_ALARM_TYPE_COUNT)
		return -EINVAL;

	if (ANDROID_ALARM_BASE_CMD(cmd) != ANDROID_ALARM_GET_TIME(0)) {
		if ((file->f_flags & O_ACCMODE) == O_RDONLY)
			return -EPERM;
		if (file->private_data == NULL &&
		    cmd != ANDROID_ALARM_SET_RTC) {
			spin_lock_irqsave(&alarm_slock, flags);
			if (alarm_opened) {
				spin_unlock_irqrestore(&alarm_slock, flags);
				return -EBUSY;
			}
			alarm_opened = 1;
			file->private_data = (void *)1;
			spin_unlock_irqrestore(&alarm_slock, flags);
		}
	}

	switch (ANDROID_ALARM_BASE_CMD(cmd)) {
	case ANDROID_ALARM_CLEAR(0):
<<<<<<< HEAD
		spin_lock_irqsave(&alarm_slock, flags);
		pr_alarm(IO, "alarm %d clear\n", alarm_type);
		devalarm_try_to_cancel(&alarms[alarm_type]);
		if (alarm_pending) {
			alarm_pending &= ~alarm_type_mask;
			if (!alarm_pending && !wait_pending)
				wake_unlock(&alarm_wake_lock);
		}
		alarm_enabled &= ~alarm_type_mask;
		spin_unlock_irqrestore(&alarm_slock, flags);
		break;

	case ANDROID_ALARM_SET_OLD:
	case ANDROID_ALARM_SET_AND_WAIT_OLD:
		if (get_user(new_alarm_time.tv_sec, (int __user *)arg)) {
			rv = -EFAULT;
			goto err1;
		}
		new_alarm_time.tv_nsec = 0;
		goto from_old_alarm_set;

	case ANDROID_ALARM_SET_AND_WAIT(0):
	case ANDROID_ALARM_SET(0):
		if (copy_from_user(&new_alarm_time, (void __user *)arg,
		    sizeof(new_alarm_time))) {
			rv = -EFAULT;
			goto err1;
		}
from_old_alarm_set:
		spin_lock_irqsave(&alarm_slock, flags);
		pr_alarm(IO, "alarm %d set %ld.%09ld\n", alarm_type,
			new_alarm_time.tv_sec, new_alarm_time.tv_nsec);
		alarm_enabled |= alarm_type_mask;
		devalarm_start(&alarms[alarm_type],
			timespec_to_ktime(new_alarm_time));
		spin_unlock_irqrestore(&alarm_slock, flags);
		if (ANDROID_ALARM_BASE_CMD(cmd) != ANDROID_ALARM_SET_AND_WAIT(0)
		    && cmd != ANDROID_ALARM_SET_AND_WAIT_OLD)
			break;
		/* fall though */
	case ANDROID_ALARM_WAIT:
		spin_lock_irqsave(&alarm_slock, flags);
		pr_alarm(IO, "alarm wait\n");
		if (!alarm_pending && wait_pending) {
			wake_unlock(&alarm_wake_lock);
			wait_pending = 0;
		}
		spin_unlock_irqrestore(&alarm_slock, flags);
		rv = wait_event_interruptible(alarm_wait_queue, alarm_pending);
		if (rv)
			goto err1;
		spin_lock_irqsave(&alarm_slock, flags);
		rv = alarm_pending;
		wait_pending = 1;
		alarm_pending = 0;
		spin_unlock_irqrestore(&alarm_slock, flags);
		break;
	case ANDROID_ALARM_SET_RTC:
		if (copy_from_user(&new_rtc_time, (void __user *)arg,
		    sizeof(new_rtc_time))) {
			rv = -EFAULT;
			goto err1;
		}
		rtc_time_to_tm(new_rtc_time.tv_sec, &new_rtc_tm);
		rtc_dev = alarmtimer_get_rtcdev();
		rv = do_settimeofday(&new_rtc_time);
		if (rv < 0)
			goto err1;
		if (rtc_dev)
			rv = rtc_set_time(rtc_dev, &new_rtc_tm);
		spin_lock_irqsave(&alarm_slock, flags);
		alarm_pending |= ANDROID_ALARM_TIME_CHANGE_MASK;
		wake_up(&alarm_wait_queue);
		spin_unlock_irqrestore(&alarm_slock, flags);
		if (rv < 0)
			goto err1;
		break;
	case ANDROID_ALARM_GET_TIME(0):
		switch (alarm_type) {
		case ANDROID_ALARM_RTC_WAKEUP:
		case ANDROID_ALARM_RTC:
			getnstimeofday(&tmp_time);
			break;
		case ANDROID_ALARM_ELAPSED_REALTIME_WAKEUP:
		case ANDROID_ALARM_ELAPSED_REALTIME:
			get_monotonic_boottime(&tmp_time);
			break;
		case ANDROID_ALARM_TYPE_COUNT:
		case ANDROID_ALARM_SYSTEMTIME:
			ktime_get_ts(&tmp_time);
			break;
		}
		if (copy_to_user((void __user *)arg, &tmp_time,
		    sizeof(tmp_time))) {
			rv = -EFAULT;
			goto err1;
		}
=======
		alarm_clear(alarm_type);
		break;
	case ANDROID_ALARM_SET(0):
		alarm_set(alarm_type, ts);
		break;
	case ANDROID_ALARM_SET_AND_WAIT(0):
		alarm_set(alarm_type, ts);
		/* fall though */
	case ANDROID_ALARM_WAIT:
		rv = alarm_wait();
		break;
	case ANDROID_ALARM_SET_RTC:
		rv = alarm_set_rtc(ts);
		break;
	case ANDROID_ALARM_GET_TIME(0):
		rv = alarm_get_time(alarm_type, ts);
>>>>>>> refs/remotes/origin/master
		break;

	default:
		rv = -EINVAL;
<<<<<<< HEAD
		goto err1;
	}
err1:
	return rv;
}

=======
	}
	return rv;
}

static long alarm_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{

	struct timespec ts;
	int rv;

	switch (ANDROID_ALARM_BASE_CMD(cmd)) {
	case ANDROID_ALARM_SET_AND_WAIT(0):
	case ANDROID_ALARM_SET(0):
	case ANDROID_ALARM_SET_RTC:
		if (copy_from_user(&ts, (void __user *)arg, sizeof(ts)))
			return -EFAULT;
		break;
	}

	rv = alarm_do_ioctl(file, cmd, &ts);
	if (rv)
		return rv;

	switch (ANDROID_ALARM_BASE_CMD(cmd)) {
	case ANDROID_ALARM_GET_TIME(0):
		if (copy_to_user((void __user *)arg, &ts, sizeof(ts)))
			return -EFAULT;
		break;
	}

	return 0;
}

#ifdef CONFIG_COMPAT
static long alarm_compat_ioctl(struct file *file, unsigned int cmd,
							unsigned long arg)
{

	struct timespec ts;
	int rv;

	switch (ANDROID_ALARM_BASE_CMD(cmd)) {
	case ANDROID_ALARM_SET_AND_WAIT_COMPAT(0):
	case ANDROID_ALARM_SET_COMPAT(0):
	case ANDROID_ALARM_SET_RTC_COMPAT:
		if (compat_get_timespec(&ts, (void __user *)arg))
			return -EFAULT;
		/* fall through */
	case ANDROID_ALARM_GET_TIME_COMPAT(0):
		cmd = ANDROID_ALARM_COMPAT_TO_NORM(cmd);
		break;
	}

	rv = alarm_do_ioctl(file, cmd, &ts);
	if (rv)
		return rv;

	switch (ANDROID_ALARM_BASE_CMD(cmd)) {
	case ANDROID_ALARM_GET_TIME(0): /* NOTE: we modified cmd above */
		if (compat_put_timespec(&ts, (void __user *)arg))
			return -EFAULT;
		break;
	}

	return 0;
}
#endif

>>>>>>> refs/remotes/origin/master
static int alarm_open(struct inode *inode, struct file *file)
{
	file->private_data = NULL;
	return 0;
}

static int alarm_release(struct inode *inode, struct file *file)
{
	int i;
	unsigned long flags;

	spin_lock_irqsave(&alarm_slock, flags);
<<<<<<< HEAD
	if (file->private_data != 0) {
		for (i = 0; i < ANDROID_ALARM_TYPE_COUNT; i++) {
			uint32_t alarm_type_mask = 1U << i;
			if (alarm_enabled & alarm_type_mask) {
				pr_alarm(INFO, "alarm_release: clear alarm, "
					"pending %d\n",
					!!(alarm_pending & alarm_type_mask));
=======
	if (file->private_data) {
		for (i = 0; i < ANDROID_ALARM_TYPE_COUNT; i++) {
			uint32_t alarm_type_mask = 1U << i;
			if (alarm_enabled & alarm_type_mask) {
				alarm_dbg(INFO,
					  "%s: clear alarm, pending %d\n",
					  __func__,
					  !!(alarm_pending & alarm_type_mask));
>>>>>>> refs/remotes/origin/master
				alarm_enabled &= ~alarm_type_mask;
			}
			spin_unlock_irqrestore(&alarm_slock, flags);
			devalarm_cancel(&alarms[i]);
			spin_lock_irqsave(&alarm_slock, flags);
		}
		if (alarm_pending | wait_pending) {
			if (alarm_pending)
<<<<<<< HEAD
				pr_alarm(INFO, "alarm_release: clear "
					"pending alarms %x\n", alarm_pending);
			wake_unlock(&alarm_wake_lock);
=======
				alarm_dbg(INFO, "%s: clear pending alarms %x\n",
					  __func__, alarm_pending);
			__pm_relax(&alarm_wake_lock);
>>>>>>> refs/remotes/origin/master
			wait_pending = 0;
			alarm_pending = 0;
		}
		alarm_opened = 0;
	}
	spin_unlock_irqrestore(&alarm_slock, flags);
	return 0;
}

static void devalarm_triggered(struct devalarm *alarm)
{
	unsigned long flags;
	uint32_t alarm_type_mask = 1U << alarm->type;

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	pr_alarm(INT, "devalarm_triggered type %d\n", alarm->type);
	spin_lock_irqsave(&alarm_slock, flags);
	if (alarm_enabled & alarm_type_mask) {
		wake_lock_timeout(&alarm_wake_lock, 5 * HZ);
=======
	alarm_dbg(INT, "%s: type %d\n", __func__, alarm->type);
	spin_lock_irqsave(&alarm_slock, flags);
	if (alarm_enabled & alarm_type_mask) {
		__pm_wakeup_event(&alarm_wake_lock, 5000); /* 5secs */
>>>>>>> refs/remotes/origin/master
		alarm_enabled &= ~alarm_type_mask;
		alarm_pending |= alarm_type_mask;
		wake_up(&alarm_wait_queue);
	}
	spin_unlock_irqrestore(&alarm_slock, flags);
}

<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
=======

>>>>>>> refs/remotes/origin/cm-11.0
static enum hrtimer_restart devalarm_hrthandler(struct hrtimer *hrt)
{
	struct devalarm *devalrm = container_of(hrt, struct devalarm, u.hrt);

	devalarm_triggered(devalrm);
	return HRTIMER_NORESTART;
}

static enum alarmtimer_restart devalarm_alarmhandler(struct alarm *alrm,
							ktime_t now)
{
	struct devalarm *devalrm = container_of(alrm, struct devalarm, u.alrm);

	devalarm_triggered(devalrm);
	return ALARMTIMER_NORESTART;
}


static const struct file_operations alarm_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = alarm_ioctl,
	.open = alarm_open,
	.release = alarm_release,
<<<<<<< HEAD
=======
#ifdef CONFIG_COMPAT
	.compat_ioctl = alarm_compat_ioctl,
#endif
>>>>>>> refs/remotes/origin/master
};

static struct miscdevice alarm_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "alarm",
	.fops = &alarm_fops,
};

static int __init alarm_dev_init(void)
{
	int err;
	int i;

	err = misc_register(&alarm_device);
	if (err)
		return err;

	alarm_init(&alarms[ANDROID_ALARM_RTC_WAKEUP].u.alrm,
			ALARM_REALTIME, devalarm_alarmhandler);
	hrtimer_init(&alarms[ANDROID_ALARM_RTC].u.hrt,
			CLOCK_REALTIME, HRTIMER_MODE_ABS);
	alarm_init(&alarms[ANDROID_ALARM_ELAPSED_REALTIME_WAKEUP].u.alrm,
			ALARM_BOOTTIME, devalarm_alarmhandler);
	hrtimer_init(&alarms[ANDROID_ALARM_ELAPSED_REALTIME].u.hrt,
			CLOCK_BOOTTIME, HRTIMER_MODE_ABS);
	hrtimer_init(&alarms[ANDROID_ALARM_SYSTEMTIME].u.hrt,
			CLOCK_MONOTONIC, HRTIMER_MODE_ABS);

	for (i = 0; i < ANDROID_ALARM_TYPE_COUNT; i++) {
		alarms[i].type = i;
		if (!is_wakeup(i))
			alarms[i].u.hrt.function = devalarm_hrthandler;
	}

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	wake_lock_init(&alarm_wake_lock, WAKE_LOCK_SUSPEND, "alarm");

=======
	wakeup_source_init(&alarm_wake_lock, "alarm");
>>>>>>> refs/remotes/origin/master
	return 0;
}

static void  __exit alarm_dev_exit(void)
{
	misc_deregister(&alarm_device);
<<<<<<< HEAD
	wake_lock_destroy(&alarm_wake_lock);
=======
	wakeup_source_trash(&alarm_wake_lock);
>>>>>>> refs/remotes/origin/master
}

module_init(alarm_dev_init);
module_exit(alarm_dev_exit);

