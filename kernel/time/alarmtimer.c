/*
 * Alarmtimer interface
 *
 * This interface provides a timer which is similarto hrtimers,
 * but triggers a RTC alarm if the box is suspend.
 *
 * This interface is influenced by the Android RTC Alarm timer
 * interface.
 *
 * Copyright (C) 2010 IBM Corperation
 *
 * Author: John Stultz <john.stultz@linaro.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/time.h>
#include <linux/hrtimer.h>
#include <linux/timerqueue.h>
#include <linux/rtc.h>
#include <linux/alarmtimer.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>
#include <linux/posix-timers.h>
#include <linux/workqueue.h>
#include <linux/freezer.h>

/**
 * struct alarm_base - Alarm timer bases
 * @lock:		Lock for syncrhonized access to the base
 * @timerqueue:		Timerqueue head managing the list of events
 * @timer: 		hrtimer used to schedule events while running
 * @gettime:		Function to read the time correlating to the base
 * @base_clockid:	clockid for the base
 */
static struct alarm_base {
	spinlock_t		lock;
	struct timerqueue_head	timerqueue;
<<<<<<< HEAD
	struct hrtimer		timer;
=======
>>>>>>> refs/remotes/origin/master
	ktime_t			(*gettime)(void);
	clockid_t		base_clockid;
} alarm_bases[ALARM_NUMTYPE];

/* freezer delta & lock used to handle clock_nanosleep triggered wakeups */
static ktime_t freezer_delta;
static DEFINE_SPINLOCK(freezer_delta_lock);

<<<<<<< HEAD
=======
static struct wakeup_source *ws;

>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_RTC_CLASS
/* rtc timer and device for setting alarm wakeups at suspend */
static struct rtc_timer		rtctimer;
static struct rtc_device	*rtcdev;
static DEFINE_SPINLOCK(rtcdev_lock);

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * has_wakealarm - check rtc device has wakealarm ability
 * @dev: current device
 * @name_ptr: name to be returned
 *
 * This helper function checks to see if the rtc device can wake
 * from suspend.
 */
static int has_wakealarm(struct device *dev, void *name_ptr)
{
	struct rtc_device *candidate = to_rtc_device(dev);

	if (!candidate->ops->set_alarm)
		return 0;
	if (!device_may_wakeup(candidate->dev.parent))
		return 0;

	*(const char **)name_ptr = dev_name(dev);
	return 1;
}

/**
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * alarmtimer_get_rtcdev - Return selected rtcdevice
 *
 * This function returns the rtc device to use for wakealarms.
 * If one has not already been chosen, it checks to see if a
 * functional rtc device is available.
 */
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
static struct rtc_device *alarmtimer_get_rtcdev(void)
=======
struct rtc_device *alarmtimer_get_rtcdev(void)
>>>>>>> refs/remotes/origin/cm-11.0
{
	struct device *dev;
	char *str;
=======
struct rtc_device *alarmtimer_get_rtcdev(void)
{
>>>>>>> refs/remotes/origin/cm-10.0
=======
struct rtc_device *alarmtimer_get_rtcdev(void)
{
>>>>>>> refs/remotes/origin/master
	unsigned long flags;
	struct rtc_device *ret;

	spin_lock_irqsave(&rtcdev_lock, flags);
<<<<<<< HEAD
<<<<<<< HEAD
	if (!rtcdev) {
		/* Find an rtc device and init the rtc_timer */
		dev = class_find_device(rtc_class, NULL, &str, has_wakealarm);
		/* If we have a device then str is valid. See has_wakealarm() */
		if (dev) {
			rtcdev = rtc_class_open(str);
			/*
			 * Drop the reference we got in class_find_device,
			 * rtc_open takes its own.
			 */
			put_device(dev);
			rtc_timer_init(&rtctimer, NULL, NULL);
		}
	}
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	ret = rtcdev;
	spin_unlock_irqrestore(&rtcdev_lock, flags);

	return ret;
}
<<<<<<< HEAD
<<<<<<< HEAD
#else
#define alarmtimer_get_rtcdev() (0)
#define rtcdev (0)
#endif


=======
=======
>>>>>>> refs/remotes/origin/master


static int alarmtimer_rtc_add_device(struct device *dev,
				struct class_interface *class_intf)
{
	unsigned long flags;
	struct rtc_device *rtc = to_rtc_device(dev);

	if (rtcdev)
		return -EBUSY;

	if (!rtc->ops->set_alarm)
		return -1;
	if (!device_may_wakeup(rtc->dev.parent))
		return -1;

	spin_lock_irqsave(&rtcdev_lock, flags);
	if (!rtcdev) {
		rtcdev = rtc;
		/* hold a reference so it doesn't go away */
		get_device(dev);
	}
	spin_unlock_irqrestore(&rtcdev_lock, flags);
	return 0;
}

static inline void alarmtimer_rtc_timer_init(void)
{
	rtc_timer_init(&rtctimer, NULL, NULL);
}

static struct class_interface alarmtimer_rtc_interface = {
	.add_dev = &alarmtimer_rtc_add_device,
};

static int alarmtimer_rtc_interface_setup(void)
{
	alarmtimer_rtc_interface.class = rtc_class;
	return class_interface_register(&alarmtimer_rtc_interface);
}
static void alarmtimer_rtc_interface_remove(void)
{
	class_interface_unregister(&alarmtimer_rtc_interface);
}
#else
struct rtc_device *alarmtimer_get_rtcdev(void)
{
	return NULL;
}
#define rtcdev (NULL)
static inline int alarmtimer_rtc_interface_setup(void) { return 0; }
static inline void alarmtimer_rtc_interface_remove(void) { }
static inline void alarmtimer_rtc_timer_init(void) { }
#endif

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/**
 * alarmtimer_enqueue - Adds an alarm timer to an alarm_base timerqueue
 * @base: pointer to the base where the timer is being run
 * @alarm: pointer to alarm being enqueued.
 *
<<<<<<< HEAD
 * Adds alarm to a alarm_base timerqueue and if necessary sets
 * an hrtimer to run.
=======
 * Adds alarm to a alarm_base timerqueue
>>>>>>> refs/remotes/origin/master
 *
 * Must hold base->lock when calling.
 */
static void alarmtimer_enqueue(struct alarm_base *base, struct alarm *alarm)
{
<<<<<<< HEAD
	timerqueue_add(&base->timerqueue, &alarm->node);
<<<<<<< HEAD
=======
	alarm->state |= ALARMTIMER_STATE_ENQUEUED;

>>>>>>> refs/remotes/origin/cm-10.0
	if (&alarm->node == timerqueue_getnext(&base->timerqueue)) {
		hrtimer_try_to_cancel(&base->timer);
		hrtimer_start(&base->timer, alarm->node.expires,
				HRTIMER_MODE_ABS);
	}
}

/**
 * alarmtimer_remove - Removes an alarm timer from an alarm_base timerqueue
 * @base: pointer to the base where the timer is running
 * @alarm: pointer to alarm being removed
 *
 * Removes alarm to a alarm_base timerqueue and if necessary sets
 * a new timer to run.
 *
 * Must hold base->lock when calling.
 */
static void alarmtimer_remove(struct alarm_base *base, struct alarm *alarm)
{
	struct timerqueue_node *next = timerqueue_getnext(&base->timerqueue);

<<<<<<< HEAD
	timerqueue_del(&base->timerqueue, &alarm->node);
=======
=======
	if (alarm->state & ALARMTIMER_STATE_ENQUEUED)
		timerqueue_del(&base->timerqueue, &alarm->node);

	timerqueue_add(&base->timerqueue, &alarm->node);
	alarm->state |= ALARMTIMER_STATE_ENQUEUED;
}

/**
 * alarmtimer_dequeue - Removes an alarm timer from an alarm_base timerqueue
 * @base: pointer to the base where the timer is running
 * @alarm: pointer to alarm being removed
 *
 * Removes alarm to a alarm_base timerqueue
 *
 * Must hold base->lock when calling.
 */
static void alarmtimer_dequeue(struct alarm_base *base, struct alarm *alarm)
{
>>>>>>> refs/remotes/origin/master
	if (!(alarm->state & ALARMTIMER_STATE_ENQUEUED))
		return;

	timerqueue_del(&base->timerqueue, &alarm->node);
	alarm->state &= ~ALARMTIMER_STATE_ENQUEUED;
<<<<<<< HEAD

>>>>>>> refs/remotes/origin/cm-10.0
	if (next == &alarm->node) {
		hrtimer_try_to_cancel(&base->timer);
		next = timerqueue_getnext(&base->timerqueue);
		if (!next)
			return;
		hrtimer_start(&base->timer, next->expires, HRTIMER_MODE_ABS);
	}
=======
>>>>>>> refs/remotes/origin/master
}


/**
 * alarmtimer_fired - Handles alarm hrtimer being fired.
 * @timer: pointer to hrtimer being run
 *
 * When a alarm timer fires, this runs through the timerqueue to
 * see which alarms expired, and runs those. If there are more alarm
 * timers queued for the future, we set the hrtimer to fire when
 * when the next future alarm timer expires.
 */
static enum hrtimer_restart alarmtimer_fired(struct hrtimer *timer)
{
<<<<<<< HEAD
	struct alarm_base *base = container_of(timer, struct alarm_base, timer);
	struct timerqueue_node *next;
	unsigned long flags;
	ktime_t now;
	int ret = HRTIMER_NORESTART;
<<<<<<< HEAD
=======
	int restart = ALARMTIMER_NORESTART;
>>>>>>> refs/remotes/origin/cm-10.0

	spin_lock_irqsave(&base->lock, flags);
	now = base->gettime();
	while ((next = timerqueue_getnext(&base->timerqueue))) {
		struct alarm *alarm;
		ktime_t expired = next->expires;

		if (expired.tv64 > now.tv64)
			break;

		alarm = container_of(next, struct alarm, node);

		timerqueue_del(&base->timerqueue, &alarm->node);
<<<<<<< HEAD
		alarm->enabled = 0;
		/* Re-add periodic timers */
		if (alarm->period.tv64) {
			alarm->node.expires = ktime_add(expired, alarm->period);
			timerqueue_add(&base->timerqueue, &alarm->node);
			alarm->enabled = 1;
		}
		spin_unlock_irqrestore(&base->lock, flags);
		if (alarm->function)
			alarm->function(alarm);
		spin_lock_irqsave(&base->lock, flags);
=======
		alarm->state &= ~ALARMTIMER_STATE_ENQUEUED;

		alarm->state |= ALARMTIMER_STATE_CALLBACK;
		spin_unlock_irqrestore(&base->lock, flags);
		if (alarm->function)
			restart = alarm->function(alarm, now);
		spin_lock_irqsave(&base->lock, flags);
		alarm->state &= ~ALARMTIMER_STATE_CALLBACK;

		if (restart != ALARMTIMER_NORESTART) {
			timerqueue_add(&base->timerqueue, &alarm->node);
			alarm->state |= ALARMTIMER_STATE_ENQUEUED;
		}
>>>>>>> refs/remotes/origin/cm-10.0
	}

	if (next) {
		hrtimer_set_expires(&base->timer, next->expires);
=======
	struct alarm *alarm = container_of(timer, struct alarm, timer);
	struct alarm_base *base = &alarm_bases[alarm->type];
	unsigned long flags;
	int ret = HRTIMER_NORESTART;
	int restart = ALARMTIMER_NORESTART;

	spin_lock_irqsave(&base->lock, flags);
	alarmtimer_dequeue(base, alarm);
	spin_unlock_irqrestore(&base->lock, flags);

	if (alarm->function)
		restart = alarm->function(alarm, base->gettime());

	spin_lock_irqsave(&base->lock, flags);
	if (restart != ALARMTIMER_NORESTART) {
		hrtimer_set_expires(&alarm->timer, alarm->node.expires);
		alarmtimer_enqueue(base, alarm);
>>>>>>> refs/remotes/origin/master
		ret = HRTIMER_RESTART;
	}
	spin_unlock_irqrestore(&base->lock, flags);

	return ret;

}

<<<<<<< HEAD
=======
ktime_t alarm_expires_remaining(const struct alarm *alarm)
{
	struct alarm_base *base = &alarm_bases[alarm->type];
	return ktime_sub(alarm->node.expires, base->gettime());
}
EXPORT_SYMBOL_GPL(alarm_expires_remaining);

>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_RTC_CLASS
/**
 * alarmtimer_suspend - Suspend time callback
 * @dev: unused
 * @state: unused
 *
 * When we are going into suspend, we look through the bases
 * to see which is the soonest timer to expire. We then
 * set an rtc timer to fire that far into the future, which
 * will wake us from suspend.
 */
static int alarmtimer_suspend(struct device *dev)
{
	struct rtc_time tm;
	ktime_t min, now;
	unsigned long flags;
	struct rtc_device *rtc;
	int i;
<<<<<<< HEAD
=======
	int ret;
>>>>>>> refs/remotes/origin/master

	spin_lock_irqsave(&freezer_delta_lock, flags);
	min = freezer_delta;
	freezer_delta = ktime_set(0, 0);
	spin_unlock_irqrestore(&freezer_delta_lock, flags);

<<<<<<< HEAD
<<<<<<< HEAD
	rtc = rtcdev;
=======
	rtc = alarmtimer_get_rtcdev();
>>>>>>> refs/remotes/origin/cm-10.0
=======
	rtc = alarmtimer_get_rtcdev();
>>>>>>> refs/remotes/origin/master
	/* If we have no rtcdev, just return */
	if (!rtc)
		return 0;

	/* Find the soonest timer to expire*/
	for (i = 0; i < ALARM_NUMTYPE; i++) {
		struct alarm_base *base = &alarm_bases[i];
		struct timerqueue_node *next;
		ktime_t delta;

		spin_lock_irqsave(&base->lock, flags);
		next = timerqueue_getnext(&base->timerqueue);
		spin_unlock_irqrestore(&base->lock, flags);
		if (!next)
			continue;
		delta = ktime_sub(next->expires, base->gettime());
		if (!min.tv64 || (delta.tv64 < min.tv64))
			min = delta;
	}
	if (min.tv64 == 0)
		return 0;

<<<<<<< HEAD
	/* XXX - Should we enforce a minimum sleep time? */
	WARN_ON(min.tv64 < NSEC_PER_SEC);
=======
	if (ktime_to_ns(min) < 2 * NSEC_PER_SEC) {
		__pm_wakeup_event(ws, 2 * MSEC_PER_SEC);
		return -EBUSY;
	}
>>>>>>> refs/remotes/origin/master

	/* Setup an rtc timer to fire that far in the future */
	rtc_timer_cancel(rtc, &rtctimer);
	rtc_read_time(rtc, &tm);
	now = rtc_tm_to_ktime(tm);
	now = ktime_add(now, min);

<<<<<<< HEAD
	rtc_timer_start(rtc, &rtctimer, now, ktime_set(0, 0));

	return 0;
=======
	/* Set alarm, if in the past reject suspend briefly to handle */
	ret = rtc_timer_start(rtc, &rtctimer, now, ktime_set(0, 0));
	if (ret < 0)
		__pm_wakeup_event(ws, MSEC_PER_SEC);
	return ret;
>>>>>>> refs/remotes/origin/master
}
#else
static int alarmtimer_suspend(struct device *dev)
{
	return 0;
}
#endif

static void alarmtimer_freezerset(ktime_t absexp, enum alarmtimer_type type)
{
	ktime_t delta;
	unsigned long flags;
	struct alarm_base *base = &alarm_bases[type];

	delta = ktime_sub(absexp, base->gettime());

	spin_lock_irqsave(&freezer_delta_lock, flags);
	if (!freezer_delta.tv64 || (delta.tv64 < freezer_delta.tv64))
		freezer_delta = delta;
	spin_unlock_irqrestore(&freezer_delta_lock, flags);
}


/**
 * alarm_init - Initialize an alarm structure
 * @alarm: ptr to alarm to be initialized
 * @type: the type of the alarm
 * @function: callback that is run when the alarm fires
 */
void alarm_init(struct alarm *alarm, enum alarmtimer_type type,
<<<<<<< HEAD
<<<<<<< HEAD
		void (*function)(struct alarm *))
{
	timerqueue_init(&alarm->node);
	alarm->period = ktime_set(0, 0);
	alarm->function = function;
	alarm->type = type;
	alarm->enabled = 0;
=======
		enum alarmtimer_restart (*function)(struct alarm *, ktime_t))
{
	timerqueue_init(&alarm->node);
	alarm->function = function;
	alarm->type = type;
	alarm->state = ALARMTIMER_STATE_INACTIVE;
>>>>>>> refs/remotes/origin/cm-10.0
}

/**
 * alarm_start - Sets an alarm to fire
 * @alarm: ptr to alarm to set
 * @start: time to run the alarm
<<<<<<< HEAD
 * @period: period at which the alarm will recur
 */
void alarm_start(struct alarm *alarm, ktime_t start, ktime_t period)
=======
 */
void alarm_start(struct alarm *alarm, ktime_t start)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct alarm_base *base = &alarm_bases[alarm->type];
	unsigned long flags;

	spin_lock_irqsave(&base->lock, flags);
<<<<<<< HEAD
	if (alarm->enabled)
		alarmtimer_remove(base, alarm);
	alarm->node.expires = start;
	alarm->period = period;
	alarmtimer_enqueue(base, alarm);
	alarm->enabled = 1;
=======
	if (alarmtimer_active(alarm))
		alarmtimer_remove(base, alarm);
	alarm->node.expires = start;
	alarmtimer_enqueue(base, alarm);
>>>>>>> refs/remotes/origin/cm-10.0
	spin_unlock_irqrestore(&base->lock, flags);
}

/**
<<<<<<< HEAD
 * alarm_cancel - Tries to cancel an alarm timer
 * @alarm: ptr to alarm to be canceled
 */
void alarm_cancel(struct alarm *alarm)
=======
		enum alarmtimer_restart (*function)(struct alarm *, ktime_t))
{
	timerqueue_init(&alarm->node);
	hrtimer_init(&alarm->timer, alarm_bases[type].base_clockid,
			HRTIMER_MODE_ABS);
	alarm->timer.function = alarmtimer_fired;
	alarm->function = function;
	alarm->type = type;
	alarm->state = ALARMTIMER_STATE_INACTIVE;
}
EXPORT_SYMBOL_GPL(alarm_init);

/**
 * alarm_start - Sets an absolute alarm to fire
 * @alarm: ptr to alarm to set
 * @start: time to run the alarm
 */
int alarm_start(struct alarm *alarm, ktime_t start)
{
	struct alarm_base *base = &alarm_bases[alarm->type];
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&base->lock, flags);
	alarm->node.expires = start;
	alarmtimer_enqueue(base, alarm);
	ret = hrtimer_start(&alarm->timer, alarm->node.expires,
				HRTIMER_MODE_ABS);
	spin_unlock_irqrestore(&base->lock, flags);
	return ret;
}
EXPORT_SYMBOL_GPL(alarm_start);

/**
 * alarm_start_relative - Sets a relative alarm to fire
 * @alarm: ptr to alarm to set
 * @start: time relative to now to run the alarm
 */
int alarm_start_relative(struct alarm *alarm, ktime_t start)
{
	struct alarm_base *base = &alarm_bases[alarm->type];

	start = ktime_add(start, base->gettime());
	return alarm_start(alarm, start);
}
EXPORT_SYMBOL_GPL(alarm_start_relative);

void alarm_restart(struct alarm *alarm)
>>>>>>> refs/remotes/origin/master
{
	struct alarm_base *base = &alarm_bases[alarm->type];
	unsigned long flags;

	spin_lock_irqsave(&base->lock, flags);
<<<<<<< HEAD
	if (alarm->enabled)
		alarmtimer_remove(base, alarm);
	alarm->enabled = 0;
	spin_unlock_irqrestore(&base->lock, flags);
}


=======
=======
	hrtimer_set_expires(&alarm->timer, alarm->node.expires);
	hrtimer_restart(&alarm->timer);
	alarmtimer_enqueue(base, alarm);
	spin_unlock_irqrestore(&base->lock, flags);
}
EXPORT_SYMBOL_GPL(alarm_restart);

/**
>>>>>>> refs/remotes/origin/master
 * alarm_try_to_cancel - Tries to cancel an alarm timer
 * @alarm: ptr to alarm to be canceled
 *
 * Returns 1 if the timer was canceled, 0 if it was not running,
 * and -1 if the callback was running
 */
int alarm_try_to_cancel(struct alarm *alarm)
{
	struct alarm_base *base = &alarm_bases[alarm->type];
	unsigned long flags;
<<<<<<< HEAD
	int ret = -1;
	spin_lock_irqsave(&base->lock, flags);

	if (alarmtimer_callback_running(alarm))
		goto out;

	if (alarmtimer_is_queued(alarm)) {
		alarmtimer_remove(base, alarm);
		ret = 1;
	} else
		ret = 0;
out:
	spin_unlock_irqrestore(&base->lock, flags);
	return ret;
}
=======
	int ret;

	spin_lock_irqsave(&base->lock, flags);
	ret = hrtimer_try_to_cancel(&alarm->timer);
	if (ret >= 0)
		alarmtimer_dequeue(base, alarm);
	spin_unlock_irqrestore(&base->lock, flags);
	return ret;
}
EXPORT_SYMBOL_GPL(alarm_try_to_cancel);
>>>>>>> refs/remotes/origin/master


/**
 * alarm_cancel - Spins trying to cancel an alarm timer until it is done
 * @alarm: ptr to alarm to be canceled
 *
 * Returns 1 if the timer was canceled, 0 if it was not active.
 */
int alarm_cancel(struct alarm *alarm)
{
	for (;;) {
		int ret = alarm_try_to_cancel(alarm);
		if (ret >= 0)
			return ret;
		cpu_relax();
	}
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(alarm_cancel);
>>>>>>> refs/remotes/origin/master


u64 alarm_forward(struct alarm *alarm, ktime_t now, ktime_t interval)
{
	u64 overrun = 1;
	ktime_t delta;

	delta = ktime_sub(now, alarm->node.expires);

	if (delta.tv64 < 0)
		return 0;

	if (unlikely(delta.tv64 >= interval.tv64)) {
		s64 incr = ktime_to_ns(interval);

		overrun = ktime_divns(delta, incr);

		alarm->node.expires = ktime_add_ns(alarm->node.expires,
							incr*overrun);

		if (alarm->node.expires.tv64 > now.tv64)
			return overrun;
		/*
		 * This (and the ktime_add() below) is the
		 * correction for exact:
		 */
		overrun++;
	}

	alarm->node.expires = ktime_add(alarm->node.expires, interval);
	return overrun;
}
<<<<<<< HEAD




>>>>>>> refs/remotes/origin/cm-10.0
=======
EXPORT_SYMBOL_GPL(alarm_forward);

u64 alarm_forward_now(struct alarm *alarm, ktime_t interval)
{
	struct alarm_base *base = &alarm_bases[alarm->type];

	return alarm_forward(alarm, base->gettime(), interval);
}
EXPORT_SYMBOL_GPL(alarm_forward_now);


>>>>>>> refs/remotes/origin/master
/**
 * clock2alarm - helper that converts from clockid to alarmtypes
 * @clockid: clockid.
 */
static enum alarmtimer_type clock2alarm(clockid_t clockid)
{
	if (clockid == CLOCK_REALTIME_ALARM)
		return ALARM_REALTIME;
	if (clockid == CLOCK_BOOTTIME_ALARM)
		return ALARM_BOOTTIME;
	return -1;
}

/**
 * alarm_handle_timer - Callback for posix timers
 * @alarm: alarm that fired
 *
 * Posix timer callback for expired alarm timers.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static void alarm_handle_timer(struct alarm *alarm)
{
	struct k_itimer *ptr = container_of(alarm, struct k_itimer,
						it.alarmtimer);
	if (posix_timer_event(ptr, 0) != 0)
		ptr->it_overrun++;
=======
=======
>>>>>>> refs/remotes/origin/master
static enum alarmtimer_restart alarm_handle_timer(struct alarm *alarm,
							ktime_t now)
{
	struct k_itimer *ptr = container_of(alarm, struct k_itimer,
						it.alarm.alarmtimer);
	if (posix_timer_event(ptr, 0) != 0)
		ptr->it_overrun++;

	/* Re-add periodic timers */
	if (ptr->it.alarm.interval.tv64) {
		ptr->it_overrun += alarm_forward(alarm, now,
						ptr->it.alarm.interval);
		return ALARMTIMER_RESTART;
	}
	return ALARMTIMER_NORESTART;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/**
 * alarm_clock_getres - posix getres interface
 * @which_clock: clockid
 * @tp: timespec to fill
 *
 * Returns the granularity of underlying alarm base clock
 */
static int alarm_clock_getres(const clockid_t which_clock, struct timespec *tp)
{
	clockid_t baseid = alarm_bases[clock2alarm(which_clock)].base_clockid;

	if (!alarmtimer_get_rtcdev())
<<<<<<< HEAD
<<<<<<< HEAD
		return -ENOTSUPP;
=======
		return -EINVAL;
>>>>>>> refs/remotes/origin/master
=======
		return -EINVAL;
>>>>>>> refs/remotes/origin/cm-11.0

	return hrtimer_get_res(baseid, tp);
}

/**
 * alarm_clock_get - posix clock_get interface
 * @which_clock: clockid
 * @tp: timespec to fill.
 *
 * Provides the underlying alarm base time.
 */
static int alarm_clock_get(clockid_t which_clock, struct timespec *tp)
{
	struct alarm_base *base = &alarm_bases[clock2alarm(which_clock)];

	if (!alarmtimer_get_rtcdev())
<<<<<<< HEAD
<<<<<<< HEAD
		return -ENOTSUPP;
=======
		return -EINVAL;
>>>>>>> refs/remotes/origin/master
=======
		return -EINVAL;
>>>>>>> refs/remotes/origin/cm-11.0

	*tp = ktime_to_timespec(base->gettime());
	return 0;
}

/**
 * alarm_timer_create - posix timer_create interface
 * @new_timer: k_itimer pointer to manage
 *
 * Initializes the k_itimer structure.
 */
static int alarm_timer_create(struct k_itimer *new_timer)
{
	enum  alarmtimer_type type;
	struct alarm_base *base;

	if (!alarmtimer_get_rtcdev())
		return -ENOTSUPP;

	if (!capable(CAP_WAKE_ALARM))
		return -EPERM;

	type = clock2alarm(new_timer->it_clock);
	base = &alarm_bases[type];
<<<<<<< HEAD
<<<<<<< HEAD
	alarm_init(&new_timer->it.alarmtimer, type, alarm_handle_timer);
=======
	alarm_init(&new_timer->it.alarm.alarmtimer, type, alarm_handle_timer);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	alarm_init(&new_timer->it.alarm.alarmtimer, type, alarm_handle_timer);
>>>>>>> refs/remotes/origin/master
	return 0;
}

/**
 * alarm_timer_get - posix timer_get interface
 * @new_timer: k_itimer pointer
 * @cur_setting: itimerspec data to fill
 *
 * Copies the itimerspec data out from the k_itimer
 */
static void alarm_timer_get(struct k_itimer *timr,
				struct itimerspec *cur_setting)
{
	memset(cur_setting, 0, sizeof(struct itimerspec));

	cur_setting->it_interval =
<<<<<<< HEAD
<<<<<<< HEAD
			ktime_to_timespec(timr->it.alarmtimer.period);
	cur_setting->it_value =
			ktime_to_timespec(timr->it.alarmtimer.node.expires);
=======
			ktime_to_timespec(timr->it.alarm.interval);
	cur_setting->it_value =
		ktime_to_timespec(timr->it.alarm.alarmtimer.node.expires);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			ktime_to_timespec(timr->it.alarm.interval);
	cur_setting->it_value =
		ktime_to_timespec(timr->it.alarm.alarmtimer.node.expires);
>>>>>>> refs/remotes/origin/master
	return;
}

/**
 * alarm_timer_del - posix timer_del interface
 * @timr: k_itimer pointer to be deleted
 *
 * Cancels any programmed alarms for the given timer.
 */
static int alarm_timer_del(struct k_itimer *timr)
{
	if (!rtcdev)
		return -ENOTSUPP;

<<<<<<< HEAD
<<<<<<< HEAD
	alarm_cancel(&timr->it.alarmtimer);
=======
	if (alarm_try_to_cancel(&timr->it.alarm.alarmtimer) < 0)
		return TIMER_RETRY;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (alarm_try_to_cancel(&timr->it.alarm.alarmtimer) < 0)
		return TIMER_RETRY;

>>>>>>> refs/remotes/origin/master
	return 0;
}

/**
 * alarm_timer_set - posix timer_set interface
 * @timr: k_itimer pointer to be deleted
 * @flags: timer flags
 * @new_setting: itimerspec to be used
 * @old_setting: itimerspec being replaced
 *
 * Sets the timer to new_setting, and starts the timer.
 */
static int alarm_timer_set(struct k_itimer *timr, int flags,
				struct itimerspec *new_setting,
				struct itimerspec *old_setting)
{
	if (!rtcdev)
		return -ENOTSUPP;

<<<<<<< HEAD
<<<<<<< HEAD
	/*
	 * XXX HACK! Currently we can DOS a system if the interval
	 * period on alarmtimers is too small. Cap the interval here
	 * to 100us and solve this properly in a future patch! -jstultz
	 */
	if ((new_setting->it_interval.tv_sec == 0) &&
			(new_setting->it_interval.tv_nsec < 100000))
		new_setting->it_interval.tv_nsec = 100000;

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (old_setting)
		alarm_timer_get(timr, old_setting);

	/* If the timer was already set, cancel it */
<<<<<<< HEAD
<<<<<<< HEAD
	alarm_cancel(&timr->it.alarmtimer);

	/* start the timer */
	alarm_start(&timr->it.alarmtimer,
			timespec_to_ktime(new_setting->it_value),
			timespec_to_ktime(new_setting->it_interval));
=======
=======
>>>>>>> refs/remotes/origin/master
	if (alarm_try_to_cancel(&timr->it.alarm.alarmtimer) < 0)
		return TIMER_RETRY;

	/* start the timer */
	timr->it.alarm.interval = timespec_to_ktime(new_setting->it_interval);
	alarm_start(&timr->it.alarm.alarmtimer,
			timespec_to_ktime(new_setting->it_value));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}

/**
 * alarmtimer_nsleep_wakeup - Wakeup function for alarm_timer_nsleep
 * @alarm: ptr to alarm that fired
 *
 * Wakes up the task that set the alarmtimer
 */
<<<<<<< HEAD
<<<<<<< HEAD
static void alarmtimer_nsleep_wakeup(struct alarm *alarm)
=======
static enum alarmtimer_restart alarmtimer_nsleep_wakeup(struct alarm *alarm,
								ktime_t now)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static enum alarmtimer_restart alarmtimer_nsleep_wakeup(struct alarm *alarm,
								ktime_t now)
>>>>>>> refs/remotes/origin/master
{
	struct task_struct *task = (struct task_struct *)alarm->data;

	alarm->data = NULL;
	if (task)
		wake_up_process(task);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	return ALARMTIMER_NORESTART;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return ALARMTIMER_NORESTART;
>>>>>>> refs/remotes/origin/master
}

/**
 * alarmtimer_do_nsleep - Internal alarmtimer nsleep implementation
 * @alarm: ptr to alarmtimer
 * @absexp: absolute expiration time
 *
 * Sets the alarm timer and sleeps until it is fired or interrupted.
 */
static int alarmtimer_do_nsleep(struct alarm *alarm, ktime_t absexp)
{
	alarm->data = (void *)current;
	do {
		set_current_state(TASK_INTERRUPTIBLE);
<<<<<<< HEAD
<<<<<<< HEAD
		alarm_start(alarm, absexp, ktime_set(0, 0));
=======
		alarm_start(alarm, absexp);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		alarm_start(alarm, absexp);
>>>>>>> refs/remotes/origin/master
		if (likely(alarm->data))
			schedule();

		alarm_cancel(alarm);
	} while (alarm->data && !signal_pending(current));

	__set_current_state(TASK_RUNNING);

	return (alarm->data == NULL);
}


/**
 * update_rmtp - Update remaining timespec value
 * @exp: expiration time
 * @type: timer type
 * @rmtp: user pointer to remaining timepsec value
 *
 * Helper function that fills in rmtp value with time between
 * now and the exp value
 */
static int update_rmtp(ktime_t exp, enum  alarmtimer_type type,
			struct timespec __user *rmtp)
{
	struct timespec rmt;
	ktime_t rem;

	rem = ktime_sub(exp, alarm_bases[type].gettime());

	if (rem.tv64 <= 0)
		return 0;
	rmt = ktime_to_timespec(rem);

	if (copy_to_user(rmtp, &rmt, sizeof(*rmtp)))
		return -EFAULT;

	return 1;

}

/**
 * alarm_timer_nsleep_restart - restartblock alarmtimer nsleep
 * @restart: ptr to restart block
 *
 * Handles restarted clock_nanosleep calls
 */
static long __sched alarm_timer_nsleep_restart(struct restart_block *restart)
{
	enum  alarmtimer_type type = restart->nanosleep.clockid;
	ktime_t exp;
	struct timespec __user  *rmtp;
	struct alarm alarm;
	int ret = 0;

	exp.tv64 = restart->nanosleep.expires;
	alarm_init(&alarm, type, alarmtimer_nsleep_wakeup);

	if (alarmtimer_do_nsleep(&alarm, exp))
		goto out;

	if (freezing(current))
		alarmtimer_freezerset(exp, type);

	rmtp = restart->nanosleep.rmtp;
	if (rmtp) {
		ret = update_rmtp(exp, type, rmtp);
		if (ret <= 0)
			goto out;
	}


	/* The other values in restart are already filled in */
	ret = -ERESTART_RESTARTBLOCK;
out:
	return ret;
}

/**
 * alarm_timer_nsleep - alarmtimer nanosleep
 * @which_clock: clockid
 * @flags: determins abstime or relative
 * @tsreq: requested sleep time (abs or rel)
 * @rmtp: remaining sleep time saved
 *
 * Handles clock_nanosleep calls against _ALARM clockids
 */
static int alarm_timer_nsleep(const clockid_t which_clock, int flags,
		     struct timespec *tsreq, struct timespec __user *rmtp)
{
	enum  alarmtimer_type type = clock2alarm(which_clock);
	struct alarm alarm;
	ktime_t exp;
	int ret = 0;
	struct restart_block *restart;

	if (!alarmtimer_get_rtcdev())
		return -ENOTSUPP;

	if (!capable(CAP_WAKE_ALARM))
		return -EPERM;

	alarm_init(&alarm, type, alarmtimer_nsleep_wakeup);

	exp = timespec_to_ktime(*tsreq);
	/* Convert (if necessary) to absolute time */
	if (flags != TIMER_ABSTIME) {
		ktime_t now = alarm_bases[type].gettime();
		exp = ktime_add(now, exp);
	}

	if (alarmtimer_do_nsleep(&alarm, exp))
		goto out;

	if (freezing(current))
		alarmtimer_freezerset(exp, type);

	/* abs timers don't set remaining time or restart */
	if (flags == TIMER_ABSTIME) {
		ret = -ERESTARTNOHAND;
		goto out;
	}

	if (rmtp) {
		ret = update_rmtp(exp, type, rmtp);
		if (ret <= 0)
			goto out;
	}

	restart = &current_thread_info()->restart_block;
	restart->fn = alarm_timer_nsleep_restart;
	restart->nanosleep.clockid = type;
	restart->nanosleep.expires = exp.tv64;
	restart->nanosleep.rmtp = rmtp;
	ret = -ERESTART_RESTARTBLOCK;

out:
	return ret;
}


/* Suspend hook structures */
static const struct dev_pm_ops alarmtimer_pm_ops = {
	.suspend = alarmtimer_suspend,
};

static struct platform_driver alarmtimer_driver = {
	.driver = {
		.name = "alarmtimer",
		.pm = &alarmtimer_pm_ops,
	}
};

/**
 * alarmtimer_init - Initialize alarm timer code
 *
 * This function initializes the alarm bases and registers
 * the posix clock ids.
 */
static int __init alarmtimer_init(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct platform_device *pdev;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct platform_device *pdev;
>>>>>>> refs/remotes/origin/master
	int error = 0;
	int i;
	struct k_clock alarm_clock = {
		.clock_getres	= alarm_clock_getres,
		.clock_get	= alarm_clock_get,
		.timer_create	= alarm_timer_create,
		.timer_set	= alarm_timer_set,
		.timer_del	= alarm_timer_del,
		.timer_get	= alarm_timer_get,
		.nsleep		= alarm_timer_nsleep,
	};

<<<<<<< HEAD
<<<<<<< HEAD
=======
	alarmtimer_rtc_timer_init();

>>>>>>> refs/remotes/origin/cm-10.0
=======
	alarmtimer_rtc_timer_init();

>>>>>>> refs/remotes/origin/master
	posix_timers_register_clock(CLOCK_REALTIME_ALARM, &alarm_clock);
	posix_timers_register_clock(CLOCK_BOOTTIME_ALARM, &alarm_clock);

	/* Initialize alarm bases */
	alarm_bases[ALARM_REALTIME].base_clockid = CLOCK_REALTIME;
	alarm_bases[ALARM_REALTIME].gettime = &ktime_get_real;
	alarm_bases[ALARM_BOOTTIME].base_clockid = CLOCK_BOOTTIME;
	alarm_bases[ALARM_BOOTTIME].gettime = &ktime_get_boottime;
	for (i = 0; i < ALARM_NUMTYPE; i++) {
		timerqueue_init_head(&alarm_bases[i].timerqueue);
		spin_lock_init(&alarm_bases[i].lock);
<<<<<<< HEAD
		hrtimer_init(&alarm_bases[i].timer,
				alarm_bases[i].base_clockid,
				HRTIMER_MODE_ABS);
		alarm_bases[i].timer.function = alarmtimer_fired;
	}
<<<<<<< HEAD
	error = platform_driver_register(&alarmtimer_driver);
	platform_device_register_simple("alarmtimer", -1, NULL, 0);

	return error;
}
device_initcall(alarmtimer_init);

=======
=======
	}
>>>>>>> refs/remotes/origin/master

	error = alarmtimer_rtc_interface_setup();
	if (error)
		return error;

	error = platform_driver_register(&alarmtimer_driver);
	if (error)
		goto out_if;

	pdev = platform_device_register_simple("alarmtimer", -1, NULL, 0);
	if (IS_ERR(pdev)) {
		error = PTR_ERR(pdev);
		goto out_drv;
	}
<<<<<<< HEAD
=======
	ws = wakeup_source_register("alarmtimer");
>>>>>>> refs/remotes/origin/master
	return 0;

out_drv:
	platform_driver_unregister(&alarmtimer_driver);
out_if:
	alarmtimer_rtc_interface_remove();
	return error;
}
device_initcall(alarmtimer_init);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
