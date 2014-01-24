/*
 * drivers/base/power/main.c - Where the driver meets power management.
 *
 * Copyright (c) 2003 Patrick Mochel
 * Copyright (c) 2003 Open Source Development Lab
 *
 * This file is released under the GPLv2
 *
 *
 * The driver model core calls device_pm_add() when a device is registered.
 * This will initialize the embedded device_pm_info object in the device
 * and add it to the list of power-controlled devices. sysfs entries for
 * controlling device power management will also be added.
 *
 * A separate list is used for keeping track of power info, because the power
 * domain dependencies may differ from the ancestral dependencies that the
 * subsystem list maintains.
 */

#include <linux/device.h>
#include <linux/kallsyms.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <linux/mutex.h>
#include <linux/pm.h>
#include <linux/pm_runtime.h>
#include <linux/resume-trace.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/async.h>
#include <linux/suspend.h>
<<<<<<< HEAD
=======
#include <trace/events/power.h>
#include <linux/cpuidle.h>
>>>>>>> refs/remotes/origin/master
#include <linux/timer.h>

#include "../base.h"
#include "power.h"

<<<<<<< HEAD
<<<<<<< HEAD
=======
typedef int (*pm_callback_t)(struct device *);

>>>>>>> refs/remotes/origin/cm-10.0
=======
typedef int (*pm_callback_t)(struct device *);

>>>>>>> refs/remotes/origin/master
/*
 * The entries in the dpm_list list are in a depth first order, simply
 * because children are guaranteed to be discovered after parents, and
 * are inserted at the back of the list on discovery.
 *
 * Since device_pm_add() may be called with a device lock held,
 * we must never try to acquire a device lock while holding
 * dpm_list_mutex.
 */

LIST_HEAD(dpm_list);
<<<<<<< HEAD
LIST_HEAD(dpm_prepared_list);
LIST_HEAD(dpm_suspended_list);
<<<<<<< HEAD
LIST_HEAD(dpm_noirq_list);

=======
LIST_HEAD(dpm_late_early_list);
LIST_HEAD(dpm_noirq_list);

struct suspend_stats suspend_stats;
>>>>>>> refs/remotes/origin/cm-10.0
static DEFINE_MUTEX(dpm_list_mtx);
static pm_message_t pm_transition;

static void dpm_drv_timeout(unsigned long data);
struct dpm_drv_wd_data {
	struct device *dev;
	struct task_struct *tsk;
};

static int async_error;

/**
 * device_pm_init - Initialize the PM-related part of a device object.
 * @dev: Device object being initialized.
 */
void device_pm_init(struct device *dev)
=======
static LIST_HEAD(dpm_prepared_list);
static LIST_HEAD(dpm_suspended_list);
static LIST_HEAD(dpm_late_early_list);
static LIST_HEAD(dpm_noirq_list);

struct suspend_stats suspend_stats;
static DEFINE_MUTEX(dpm_list_mtx);
static pm_message_t pm_transition;

static int async_error;

static char *pm_verb(int event)
{
	switch (event) {
	case PM_EVENT_SUSPEND:
		return "suspend";
	case PM_EVENT_RESUME:
		return "resume";
	case PM_EVENT_FREEZE:
		return "freeze";
	case PM_EVENT_QUIESCE:
		return "quiesce";
	case PM_EVENT_HIBERNATE:
		return "hibernate";
	case PM_EVENT_THAW:
		return "thaw";
	case PM_EVENT_RESTORE:
		return "restore";
	case PM_EVENT_RECOVER:
		return "recover";
	default:
		return "(unknown PM event)";
	}
}

/**
 * device_pm_sleep_init - Initialize system suspend-related device fields.
 * @dev: Device object being initialized.
 */
void device_pm_sleep_init(struct device *dev)
>>>>>>> refs/remotes/origin/master
{
	dev->power.is_prepared = false;
	dev->power.is_suspended = false;
	init_completion(&dev->power.completion);
	complete_all(&dev->power.completion);
	dev->power.wakeup = NULL;
<<<<<<< HEAD
	spin_lock_init(&dev->power.lock);
	pm_runtime_init(dev);
	INIT_LIST_HEAD(&dev->power.entry);
<<<<<<< HEAD
=======
	dev->power.power_state = PMSG_INVALID;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	INIT_LIST_HEAD(&dev->power.entry);
>>>>>>> refs/remotes/origin/master
}

/**
 * device_pm_lock - Lock the list of active devices used by the PM core.
 */
void device_pm_lock(void)
{
	mutex_lock(&dpm_list_mtx);
}

/**
 * device_pm_unlock - Unlock the list of active devices used by the PM core.
 */
void device_pm_unlock(void)
{
	mutex_unlock(&dpm_list_mtx);
}

/**
 * device_pm_add - Add a device to the PM core's list of active devices.
 * @dev: Device to add to the list.
 */
void device_pm_add(struct device *dev)
{
	pr_debug("PM: Adding info for %s:%s\n",
		 dev->bus ? dev->bus->name : "No Bus", dev_name(dev));
	mutex_lock(&dpm_list_mtx);
	if (dev->parent && dev->parent->power.is_prepared)
		dev_warn(dev, "parent %s should not be sleeping\n",
			dev_name(dev->parent));
	list_add_tail(&dev->power.entry, &dpm_list);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	dev_pm_qos_constraints_init(dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	mutex_unlock(&dpm_list_mtx);
}

/**
 * device_pm_remove - Remove a device from the PM core's list of active devices.
 * @dev: Device to be removed from the list.
 */
void device_pm_remove(struct device *dev)
{
	pr_debug("PM: Removing info for %s:%s\n",
		 dev->bus ? dev->bus->name : "No Bus", dev_name(dev));
	complete_all(&dev->power.completion);
	mutex_lock(&dpm_list_mtx);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	dev_pm_qos_constraints_destroy(dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	list_del_init(&dev->power.entry);
	mutex_unlock(&dpm_list_mtx);
	device_wakeup_disable(dev);
	pm_runtime_remove(dev);
}

/**
 * device_pm_move_before - Move device in the PM core's list of active devices.
 * @deva: Device to move in dpm_list.
 * @devb: Device @deva should come before.
 */
void device_pm_move_before(struct device *deva, struct device *devb)
{
	pr_debug("PM: Moving %s:%s before %s:%s\n",
		 deva->bus ? deva->bus->name : "No Bus", dev_name(deva),
		 devb->bus ? devb->bus->name : "No Bus", dev_name(devb));
	/* Delete deva from dpm_list and reinsert before devb. */
	list_move_tail(&deva->power.entry, &devb->power.entry);
}

/**
 * device_pm_move_after - Move device in the PM core's list of active devices.
 * @deva: Device to move in dpm_list.
 * @devb: Device @deva should come after.
 */
void device_pm_move_after(struct device *deva, struct device *devb)
{
	pr_debug("PM: Moving %s:%s after %s:%s\n",
		 deva->bus ? deva->bus->name : "No Bus", dev_name(deva),
		 devb->bus ? devb->bus->name : "No Bus", dev_name(devb));
	/* Delete deva from dpm_list and reinsert after devb. */
	list_move(&deva->power.entry, &devb->power.entry);
}

/**
 * device_pm_move_last - Move device to end of the PM core's list of devices.
 * @dev: Device to move in dpm_list.
 */
void device_pm_move_last(struct device *dev)
{
	pr_debug("PM: Moving %s:%s to end of list\n",
		 dev->bus ? dev->bus->name : "No Bus", dev_name(dev));
	list_move_tail(&dev->power.entry, &dpm_list);
}

static ktime_t initcall_debug_start(struct device *dev)
{
	ktime_t calltime = ktime_set(0, 0);

<<<<<<< HEAD
	if (initcall_debug) {
<<<<<<< HEAD
		pr_info("calling  %s+ @ %i\n",
				dev_name(dev), task_pid_nr(current));
=======
		pr_info("calling  %s+ @ %i, parent: %s\n",
			dev_name(dev), task_pid_nr(current),
			dev->parent ? dev_name(dev->parent) : "none");
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (pm_print_times_enabled) {
		pr_info("calling  %s+ @ %i, parent: %s\n",
			dev_name(dev), task_pid_nr(current),
			dev->parent ? dev_name(dev->parent) : "none");
>>>>>>> refs/remotes/origin/master
		calltime = ktime_get();
	}

	return calltime;
}

static void initcall_debug_report(struct device *dev, ktime_t calltime,
<<<<<<< HEAD
				  int error)
{
	ktime_t delta, rettime;

	if (initcall_debug) {
		rettime = ktime_get();
		delta = ktime_sub(rettime, calltime);
		pr_info("call %s+ returned %d after %Ld usecs\n", dev_name(dev),
			error, (unsigned long long)ktime_to_ns(delta) >> 10);
	}
=======
				  int error, pm_message_t state, char *info)
{
	ktime_t rettime;
	s64 nsecs;

	rettime = ktime_get();
	nsecs = (s64) ktime_to_ns(ktime_sub(rettime, calltime));

	if (pm_print_times_enabled) {
		pr_info("call %s+ returned %d after %Ld usecs\n", dev_name(dev),
			error, (unsigned long long)nsecs >> 10);
	}

	trace_device_pm_report_time(dev, info, nsecs, pm_verb(state.event),
				    error);
>>>>>>> refs/remotes/origin/master
}

/**
 * dpm_wait - Wait for a PM operation to complete.
 * @dev: Device to wait for.
 * @async: If unset, wait only if the device's power.async_suspend flag is set.
 */
static void dpm_wait(struct device *dev, bool async)
{
	if (!dev)
		return;

	if (async || (pm_async_enabled && dev->power.async_suspend))
		wait_for_completion(&dev->power.completion);
}

static int dpm_wait_fn(struct device *dev, void *async_ptr)
{
	dpm_wait(dev, *((bool *)async_ptr));
	return 0;
}

static void dpm_wait_for_children(struct device *dev, bool async)
{
       device_for_each_child(dev, &async, dpm_wait_fn);
}

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * pm_op - Execute the PM operation appropriate for given PM event.
 * @dev: Device to handle.
 * @ops: PM operations to choose from.
 * @state: PM transition of the system being carried out.
 */
static int pm_op(struct device *dev,
		 const struct dev_pm_ops *ops,
		 pm_message_t state)
{
	int error = 0;
	ktime_t calltime;

	calltime = initcall_debug_start(dev);

	switch (state.event) {
#ifdef CONFIG_SUSPEND
	case PM_EVENT_SUSPEND:
		if (ops->suspend) {
			error = ops->suspend(dev);
			suspend_report_result(ops->suspend, error);
		}
		break;
	case PM_EVENT_RESUME:
		if (ops->resume) {
			error = ops->resume(dev);
			suspend_report_result(ops->resume, error);
		}
		break;
=======
=======
>>>>>>> refs/remotes/origin/master
 * pm_op - Return the PM operation appropriate for given PM event.
 * @ops: PM operations to choose from.
 * @state: PM transition of the system being carried out.
 */
static pm_callback_t pm_op(const struct dev_pm_ops *ops, pm_message_t state)
{
	switch (state.event) {
#ifdef CONFIG_SUSPEND
	case PM_EVENT_SUSPEND:
		return ops->suspend;
	case PM_EVENT_RESUME:
		return ops->resume;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#endif /* CONFIG_SUSPEND */
#ifdef CONFIG_HIBERNATE_CALLBACKS
	case PM_EVENT_FREEZE:
	case PM_EVENT_QUIESCE:
<<<<<<< HEAD
<<<<<<< HEAD
		if (ops->freeze) {
			error = ops->freeze(dev);
			suspend_report_result(ops->freeze, error);
		}
		break;
	case PM_EVENT_HIBERNATE:
		if (ops->poweroff) {
			error = ops->poweroff(dev);
			suspend_report_result(ops->poweroff, error);
		}
		break;
	case PM_EVENT_THAW:
	case PM_EVENT_RECOVER:
		if (ops->thaw) {
			error = ops->thaw(dev);
			suspend_report_result(ops->thaw, error);
		}
		break;
	case PM_EVENT_RESTORE:
		if (ops->restore) {
			error = ops->restore(dev);
			suspend_report_result(ops->restore, error);
		}
		break;
#endif /* CONFIG_HIBERNATE_CALLBACKS */
	default:
		error = -EINVAL;
	}

	initcall_debug_report(dev, calltime, error);

	return error;
}

/**
 * pm_noirq_op - Execute the PM operation appropriate for given PM event.
 * @dev: Device to handle.
=======
=======
>>>>>>> refs/remotes/origin/master
		return ops->freeze;
	case PM_EVENT_HIBERNATE:
		return ops->poweroff;
	case PM_EVENT_THAW:
	case PM_EVENT_RECOVER:
		return ops->thaw;
		break;
	case PM_EVENT_RESTORE:
		return ops->restore;
#endif /* CONFIG_HIBERNATE_CALLBACKS */
	}

	return NULL;
}

/**
 * pm_late_early_op - Return the PM operation appropriate for given PM event.
 * @ops: PM operations to choose from.
 * @state: PM transition of the system being carried out.
 *
 * Runtime PM is disabled for @dev while this function is being executed.
 */
static pm_callback_t pm_late_early_op(const struct dev_pm_ops *ops,
				      pm_message_t state)
{
	switch (state.event) {
#ifdef CONFIG_SUSPEND
	case PM_EVENT_SUSPEND:
		return ops->suspend_late;
	case PM_EVENT_RESUME:
		return ops->resume_early;
#endif /* CONFIG_SUSPEND */
#ifdef CONFIG_HIBERNATE_CALLBACKS
	case PM_EVENT_FREEZE:
	case PM_EVENT_QUIESCE:
		return ops->freeze_late;
	case PM_EVENT_HIBERNATE:
		return ops->poweroff_late;
	case PM_EVENT_THAW:
	case PM_EVENT_RECOVER:
		return ops->thaw_early;
	case PM_EVENT_RESTORE:
		return ops->restore_early;
#endif /* CONFIG_HIBERNATE_CALLBACKS */
	}

	return NULL;
}

/**
 * pm_noirq_op - Return the PM operation appropriate for given PM event.
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * @ops: PM operations to choose from.
 * @state: PM transition of the system being carried out.
 *
 * The driver of @dev will not receive interrupts while this function is being
 * executed.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int pm_noirq_op(struct device *dev,
			const struct dev_pm_ops *ops,
			pm_message_t state)
{
	int error = 0;
	ktime_t calltime = ktime_set(0, 0), delta, rettime;

	if (initcall_debug) {
		pr_info("calling  %s+ @ %i, parent: %s\n",
				dev_name(dev), task_pid_nr(current),
				dev->parent ? dev_name(dev->parent) : "none");
		calltime = ktime_get();
	}

	switch (state.event) {
#ifdef CONFIG_SUSPEND
	case PM_EVENT_SUSPEND:
		if (ops->suspend_noirq) {
			error = ops->suspend_noirq(dev);
			suspend_report_result(ops->suspend_noirq, error);
		}
		break;
	case PM_EVENT_RESUME:
		if (ops->resume_noirq) {
			error = ops->resume_noirq(dev);
			suspend_report_result(ops->resume_noirq, error);
		}
		break;
=======
=======
>>>>>>> refs/remotes/origin/master
static pm_callback_t pm_noirq_op(const struct dev_pm_ops *ops, pm_message_t state)
{
	switch (state.event) {
#ifdef CONFIG_SUSPEND
	case PM_EVENT_SUSPEND:
		return ops->suspend_noirq;
	case PM_EVENT_RESUME:
		return ops->resume_noirq;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#endif /* CONFIG_SUSPEND */
#ifdef CONFIG_HIBERNATE_CALLBACKS
	case PM_EVENT_FREEZE:
	case PM_EVENT_QUIESCE:
<<<<<<< HEAD
<<<<<<< HEAD
		if (ops->freeze_noirq) {
			error = ops->freeze_noirq(dev);
			suspend_report_result(ops->freeze_noirq, error);
		}
		break;
	case PM_EVENT_HIBERNATE:
		if (ops->poweroff_noirq) {
			error = ops->poweroff_noirq(dev);
			suspend_report_result(ops->poweroff_noirq, error);
		}
		break;
	case PM_EVENT_THAW:
	case PM_EVENT_RECOVER:
		if (ops->thaw_noirq) {
			error = ops->thaw_noirq(dev);
			suspend_report_result(ops->thaw_noirq, error);
		}
		break;
	case PM_EVENT_RESTORE:
		if (ops->restore_noirq) {
			error = ops->restore_noirq(dev);
			suspend_report_result(ops->restore_noirq, error);
		}
		break;
#endif /* CONFIG_HIBERNATE_CALLBACKS */
	default:
		error = -EINVAL;
	}

	if (initcall_debug) {
		rettime = ktime_get();
		delta = ktime_sub(rettime, calltime);
		printk("initcall %s_i+ returned %d after %Ld usecs\n",
			dev_name(dev), error,
			(unsigned long long)ktime_to_ns(delta) >> 10);
	}

	return error;
=======
=======
>>>>>>> refs/remotes/origin/master
		return ops->freeze_noirq;
	case PM_EVENT_HIBERNATE:
		return ops->poweroff_noirq;
	case PM_EVENT_THAW:
	case PM_EVENT_RECOVER:
		return ops->thaw_noirq;
	case PM_EVENT_RESTORE:
		return ops->restore_noirq;
#endif /* CONFIG_HIBERNATE_CALLBACKS */
	}

	return NULL;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

static char *pm_verb(int event)
{
	switch (event) {
	case PM_EVENT_SUSPEND:
		return "suspend";
	case PM_EVENT_RESUME:
		return "resume";
	case PM_EVENT_FREEZE:
		return "freeze";
	case PM_EVENT_QUIESCE:
		return "quiesce";
	case PM_EVENT_HIBERNATE:
		return "hibernate";
	case PM_EVENT_THAW:
		return "thaw";
	case PM_EVENT_RESTORE:
		return "restore";
	case PM_EVENT_RECOVER:
		return "recover";
	default:
		return "(unknown PM event)";
	}
=======
>>>>>>> refs/remotes/origin/master
}

static void pm_dev_dbg(struct device *dev, pm_message_t state, char *info)
{
	dev_dbg(dev, "%s%s%s\n", info, pm_verb(state.event),
		((state.event & PM_EVENT_SLEEP) && device_may_wakeup(dev)) ?
		", may wakeup" : "");
}

static void pm_dev_err(struct device *dev, pm_message_t state, char *info,
			int error)
{
	printk(KERN_ERR "PM: Device %s failed to %s%s: error %d\n",
		dev_name(dev), pm_verb(state.event), info, error);
}

static void dpm_show_time(ktime_t starttime, pm_message_t state, char *info)
{
	ktime_t calltime;
	u64 usecs64;
	int usecs;

	calltime = ktime_get();
	usecs64 = ktime_to_ns(ktime_sub(calltime, starttime));
	do_div(usecs64, NSEC_PER_USEC);
	usecs = usecs64;
	if (usecs == 0)
		usecs = 1;
	pr_info("PM: %s%s%s of devices complete after %ld.%03ld msecs\n",
		info ?: "", info ? " " : "", pm_verb(state.event),
		usecs / USEC_PER_MSEC, usecs % USEC_PER_MSEC);
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static int dpm_run_callback(pm_callback_t cb, struct device *dev,
			    pm_message_t state, char *info)
{
	ktime_t calltime;
	int error;

	if (!cb)
		return 0;

	calltime = initcall_debug_start(dev);

	pm_dev_dbg(dev, state, info);
	error = cb(dev);
	suspend_report_result(cb, error);

<<<<<<< HEAD
	initcall_debug_report(dev, calltime, error);
=======
	initcall_debug_report(dev, calltime, error, state, info);
>>>>>>> refs/remotes/origin/master

	return error;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef CONFIG_DPM_WATCHDOG
struct dpm_watchdog {
	struct device		*dev;
	struct task_struct	*tsk;
	struct timer_list	timer;
};

#define DECLARE_DPM_WATCHDOG_ON_STACK(wd) \
	struct dpm_watchdog wd

/**
 * dpm_watchdog_handler - Driver suspend / resume watchdog handler.
 * @data: Watchdog object address.
 *
 * Called when a driver has timed out suspending or resuming.
 * There's not much we can do here to recover so panic() to
 * capture a crash-dump in pstore.
 */
static void dpm_watchdog_handler(unsigned long data)
{
	struct dpm_watchdog *wd = (void *)data;

	dev_emerg(wd->dev, "**** DPM device timeout ****\n");
	show_stack(wd->tsk, NULL);
	panic("%s %s: unrecoverable failure\n",
		dev_driver_string(wd->dev), dev_name(wd->dev));
}

/**
 * dpm_watchdog_set - Enable pm watchdog for given device.
 * @wd: Watchdog. Must be allocated on the stack.
 * @dev: Device to handle.
 */
static void dpm_watchdog_set(struct dpm_watchdog *wd, struct device *dev)
{
	struct timer_list *timer = &wd->timer;

	wd->dev = dev;
	wd->tsk = current;

	init_timer_on_stack(timer);
	/* use same timeout value for both suspend and resume */
	timer->expires = jiffies + HZ * CONFIG_DPM_WATCHDOG_TIMEOUT;
	timer->function = dpm_watchdog_handler;
	timer->data = (unsigned long)wd;
	add_timer(timer);
}

/**
 * dpm_watchdog_clear - Disable suspend/resume watchdog.
 * @wd: Watchdog to disable.
 */
static void dpm_watchdog_clear(struct dpm_watchdog *wd)
{
	struct timer_list *timer = &wd->timer;

	del_timer_sync(timer);
	destroy_timer_on_stack(timer);
}
#else
#define DECLARE_DPM_WATCHDOG_ON_STACK(wd)
#define dpm_watchdog_set(x, y)
#define dpm_watchdog_clear(x)
#endif

>>>>>>> refs/remotes/origin/master
/*------------------------- Resume routines -------------------------*/

/**
 * device_resume_noirq - Execute an "early resume" callback for given device.
 * @dev: Device to handle.
 * @state: PM transition of the system being carried out.
 *
 * The driver of @dev will not receive interrupts while this function is being
 * executed.
 */
static int device_resume_noirq(struct device *dev, pm_message_t state)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	pm_callback_t callback = NULL;
	char *info = NULL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pm_callback_t callback = NULL;
	char *info = NULL;
>>>>>>> refs/remotes/origin/master
	int error = 0;

	TRACE_DEVICE(dev);
	TRACE_RESUME(0);

<<<<<<< HEAD
<<<<<<< HEAD
	if (dev->pwr_domain) {
		pm_dev_dbg(dev, state, "EARLY power domain ");
		error = pm_noirq_op(dev, &dev->pwr_domain->ops, state);
	} else if (dev->type && dev->type->pm) {
		pm_dev_dbg(dev, state, "EARLY type ");
		error = pm_noirq_op(dev, dev->type->pm, state);
	} else if (dev->class && dev->class->pm) {
		pm_dev_dbg(dev, state, "EARLY class ");
		error = pm_noirq_op(dev, dev->class->pm, state);
	} else if (dev->bus && dev->bus->pm) {
		pm_dev_dbg(dev, state, "EARLY ");
		error = pm_noirq_op(dev, dev->bus->pm, state);
	}

=======
=======
	if (dev->power.syscore)
		goto Out;

>>>>>>> refs/remotes/origin/master
	if (dev->pm_domain) {
		info = "noirq power domain ";
		callback = pm_noirq_op(&dev->pm_domain->ops, state);
	} else if (dev->type && dev->type->pm) {
		info = "noirq type ";
		callback = pm_noirq_op(dev->type->pm, state);
	} else if (dev->class && dev->class->pm) {
		info = "noirq class ";
		callback = pm_noirq_op(dev->class->pm, state);
	} else if (dev->bus && dev->bus->pm) {
		info = "noirq bus ";
		callback = pm_noirq_op(dev->bus->pm, state);
	}

	if (!callback && dev->driver && dev->driver->pm) {
		info = "noirq driver ";
		callback = pm_noirq_op(dev->driver->pm, state);
	}

	error = dpm_run_callback(callback, dev, state, info);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
 Out:
>>>>>>> refs/remotes/origin/master
	TRACE_RESUME(error);
	return error;
}

/**
<<<<<<< HEAD
<<<<<<< HEAD
 * dpm_resume_noirq - Execute "early resume" callbacks for non-sysdev devices.
 * @state: PM transition of the system being carried out.
 *
 * Call the "noirq" resume handlers for all devices marked as DPM_OFF_IRQ and
 * enable device drivers to receive interrupts.
 */
void dpm_resume_noirq(pm_message_t state)
=======
=======
>>>>>>> refs/remotes/origin/master
 * dpm_resume_noirq - Execute "noirq resume" callbacks for all devices.
 * @state: PM transition of the system being carried out.
 *
 * Call the "noirq" resume handlers for all devices in dpm_noirq_list and
 * enable device drivers to receive interrupts.
 */
static void dpm_resume_noirq(pm_message_t state)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	ktime_t starttime = ktime_get();

	mutex_lock(&dpm_list_mtx);
	while (!list_empty(&dpm_noirq_list)) {
		struct device *dev = to_device(dpm_noirq_list.next);
		int error;

		get_device(dev);
<<<<<<< HEAD
<<<<<<< HEAD
		list_move_tail(&dev->power.entry, &dpm_suspended_list);
		mutex_unlock(&dpm_list_mtx);

		error = device_resume_noirq(dev, state);
		if (error)
			pm_dev_err(dev, state, " early", error);
=======
=======
>>>>>>> refs/remotes/origin/master
		list_move_tail(&dev->power.entry, &dpm_late_early_list);
		mutex_unlock(&dpm_list_mtx);

		error = device_resume_noirq(dev, state);
		if (error) {
			suspend_stats.failed_resume_noirq++;
			dpm_save_failed_step(SUSPEND_RESUME_NOIRQ);
			dpm_save_failed_dev(dev_name(dev));
			pm_dev_err(dev, state, " noirq", error);
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

		mutex_lock(&dpm_list_mtx);
		put_device(dev);
	}
	mutex_unlock(&dpm_list_mtx);
<<<<<<< HEAD
<<<<<<< HEAD
	dpm_show_time(starttime, state, "early");
	resume_device_irqs();
}
EXPORT_SYMBOL_GPL(dpm_resume_noirq);

/**
 * legacy_resume - Execute a legacy (bus or class) resume callback for device.
 * @dev: Device to resume.
 * @cb: Resume callback to execute.
 */
static int legacy_resume(struct device *dev, int (*cb)(struct device *dev))
{
	int error;
	ktime_t calltime;

	calltime = initcall_debug_start(dev);

	error = cb(dev);
	suspend_report_result(cb, error);

	initcall_debug_report(dev, calltime, error);

=======
	dpm_show_time(starttime, state, "noirq");
	resume_device_irqs();
=======
	dpm_show_time(starttime, state, "noirq");
	resume_device_irqs();
	cpuidle_resume();
>>>>>>> refs/remotes/origin/master
}

/**
 * device_resume_early - Execute an "early resume" callback for given device.
 * @dev: Device to handle.
 * @state: PM transition of the system being carried out.
 *
 * Runtime PM is disabled for @dev while this function is being executed.
 */
static int device_resume_early(struct device *dev, pm_message_t state)
{
	pm_callback_t callback = NULL;
	char *info = NULL;
	int error = 0;

	TRACE_DEVICE(dev);
	TRACE_RESUME(0);

<<<<<<< HEAD
=======
	if (dev->power.syscore)
		goto Out;

>>>>>>> refs/remotes/origin/master
	if (dev->pm_domain) {
		info = "early power domain ";
		callback = pm_late_early_op(&dev->pm_domain->ops, state);
	} else if (dev->type && dev->type->pm) {
		info = "early type ";
		callback = pm_late_early_op(dev->type->pm, state);
	} else if (dev->class && dev->class->pm) {
		info = "early class ";
		callback = pm_late_early_op(dev->class->pm, state);
	} else if (dev->bus && dev->bus->pm) {
		info = "early bus ";
		callback = pm_late_early_op(dev->bus->pm, state);
	}

	if (!callback && dev->driver && dev->driver->pm) {
		info = "early driver ";
		callback = pm_late_early_op(dev->driver->pm, state);
	}

	error = dpm_run_callback(callback, dev, state, info);

<<<<<<< HEAD
	TRACE_RESUME(error);
>>>>>>> refs/remotes/origin/cm-10.0
=======
 Out:
	TRACE_RESUME(error);

	pm_runtime_enable(dev);
>>>>>>> refs/remotes/origin/master
	return error;
}

/**
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
 * dpm_resume_early - Execute "early resume" callbacks for all devices.
 * @state: PM transition of the system being carried out.
 */
static void dpm_resume_early(pm_message_t state)
{
	ktime_t starttime = ktime_get();

	mutex_lock(&dpm_list_mtx);
	while (!list_empty(&dpm_late_early_list)) {
		struct device *dev = to_device(dpm_late_early_list.next);
		int error;

		get_device(dev);
		list_move_tail(&dev->power.entry, &dpm_suspended_list);
		mutex_unlock(&dpm_list_mtx);

		error = device_resume_early(dev, state);
		if (error) {
			suspend_stats.failed_resume_early++;
			dpm_save_failed_step(SUSPEND_RESUME_EARLY);
			dpm_save_failed_dev(dev_name(dev));
			pm_dev_err(dev, state, " early", error);
		}

		mutex_lock(&dpm_list_mtx);
		put_device(dev);
	}
	mutex_unlock(&dpm_list_mtx);
	dpm_show_time(starttime, state, "early");
}

/**
 * dpm_resume_start - Execute "noirq" and "early" device callbacks.
 * @state: PM transition of the system being carried out.
 */
void dpm_resume_start(pm_message_t state)
{
	dpm_resume_noirq(state);
	dpm_resume_early(state);
}
EXPORT_SYMBOL_GPL(dpm_resume_start);

/**
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * device_resume - Execute "resume" callbacks for given device.
 * @dev: Device to handle.
 * @state: PM transition of the system being carried out.
 * @async: If true, the device is being resumed asynchronously.
 */
static int device_resume(struct device *dev, pm_message_t state, bool async)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int error = 0;
=======
	pm_callback_t callback = NULL;
	char *info = NULL;
	int error = 0;
	bool put = false;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pm_callback_t callback = NULL;
	char *info = NULL;
	int error = 0;
	DECLARE_DPM_WATCHDOG_ON_STACK(wd);
>>>>>>> refs/remotes/origin/master

	TRACE_DEVICE(dev);
	TRACE_RESUME(0);

<<<<<<< HEAD
	dpm_wait(dev->parent, async);
=======
	if (dev->power.syscore)
		goto Complete;

	dpm_wait(dev->parent, async);
	dpm_watchdog_set(&wd, dev);
>>>>>>> refs/remotes/origin/master
	device_lock(dev);

	/*
	 * This is a fib.  But we'll allow new children to be added below
	 * a resumed device, even if the device hasn't been completed yet.
	 */
	dev->power.is_prepared = false;

	if (!dev->power.is_suspended)
		goto Unlock;

<<<<<<< HEAD
<<<<<<< HEAD
	if (dev->pwr_domain) {
		pm_dev_dbg(dev, state, "power domain ");
		error = pm_op(dev, &dev->pwr_domain->ops, state);
		goto End;
	}

	if (dev->type && dev->type->pm) {
		pm_dev_dbg(dev, state, "type ");
		error = pm_op(dev, dev->type->pm, state);
		goto End;
=======
	pm_runtime_enable(dev);
	put = true;

=======
>>>>>>> refs/remotes/origin/master
	if (dev->pm_domain) {
		info = "power domain ";
		callback = pm_op(&dev->pm_domain->ops, state);
		goto Driver;
	}

	if (dev->type && dev->type->pm) {
		info = "type ";
		callback = pm_op(dev->type->pm, state);
		goto Driver;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	if (dev->class) {
		if (dev->class->pm) {
<<<<<<< HEAD
<<<<<<< HEAD
			pm_dev_dbg(dev, state, "class ");
			error = pm_op(dev, dev->class->pm, state);
			goto End;
		} else if (dev->class->resume) {
			pm_dev_dbg(dev, state, "legacy class ");
			error = legacy_resume(dev, dev->class->resume);
=======
=======
>>>>>>> refs/remotes/origin/master
			info = "class ";
			callback = pm_op(dev->class->pm, state);
			goto Driver;
		} else if (dev->class->resume) {
			info = "legacy class ";
			callback = dev->class->resume;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			goto End;
		}
	}

	if (dev->bus) {
		if (dev->bus->pm) {
<<<<<<< HEAD
<<<<<<< HEAD
			pm_dev_dbg(dev, state, "");
			error = pm_op(dev, dev->bus->pm, state);
		} else if (dev->bus->resume) {
			pm_dev_dbg(dev, state, "legacy ");
			error = legacy_resume(dev, dev->bus->resume);
		}
	}

 End:
=======
=======
>>>>>>> refs/remotes/origin/master
			info = "bus ";
			callback = pm_op(dev->bus->pm, state);
		} else if (dev->bus->resume) {
			info = "legacy bus ";
			callback = dev->bus->resume;
			goto End;
		}
	}

 Driver:
	if (!callback && dev->driver && dev->driver->pm) {
		info = "driver ";
		callback = pm_op(dev->driver->pm, state);
	}

 End:
	error = dpm_run_callback(callback, dev, state, info);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	dev->power.is_suspended = false;

 Unlock:
	device_unlock(dev);
<<<<<<< HEAD
	complete_all(&dev->power.completion);

	TRACE_RESUME(error);
<<<<<<< HEAD
=======

	if (put)
		pm_runtime_put_sync(dev);

>>>>>>> refs/remotes/origin/cm-10.0
=======
	dpm_watchdog_clear(&wd);

 Complete:
	complete_all(&dev->power.completion);

	TRACE_RESUME(error);

>>>>>>> refs/remotes/origin/master
	return error;
}

static void async_resume(void *data, async_cookie_t cookie)
{
	struct device *dev = (struct device *)data;
	int error;

	error = device_resume(dev, pm_transition, true);
	if (error)
		pm_dev_err(dev, pm_transition, " async", error);
	put_device(dev);
}

static bool is_async(struct device *dev)
{
	return dev->power.async_suspend && pm_async_enabled
		&& !pm_trace_is_enabled();
}

/**
<<<<<<< HEAD
 *	dpm_drv_timeout - Driver suspend / resume watchdog handler
 *	@data: struct device which timed out
 *
 * 	Called when a driver has timed out suspending or resuming.
 * 	There's not much we can do here to recover so
 * 	BUG() out for a crash-dump
 *
 */
static void dpm_drv_timeout(unsigned long data)
{
	struct dpm_drv_wd_data *wd_data = (void *)data;
	struct device *dev = wd_data->dev;
	struct task_struct *tsk = wd_data->tsk;

	printk(KERN_EMERG "**** DPM device timeout: %s (%s)\n", dev_name(dev),
	       (dev->driver ? dev->driver->name : "no driver"));

	printk(KERN_EMERG "dpm suspend stack:\n");
	show_stack(tsk, NULL);

	BUG();
}

/**
=======
>>>>>>> refs/remotes/origin/master
 * dpm_resume - Execute "resume" callbacks for non-sysdev devices.
 * @state: PM transition of the system being carried out.
 *
 * Execute the appropriate "resume" callback for all devices whose status
 * indicates that they are suspended.
 */
void dpm_resume(pm_message_t state)
{
	struct device *dev;
	ktime_t starttime = ktime_get();

	might_sleep();

	mutex_lock(&dpm_list_mtx);
	pm_transition = state;
	async_error = 0;

	list_for_each_entry(dev, &dpm_suspended_list, power.entry) {
<<<<<<< HEAD
		INIT_COMPLETION(dev->power.completion);
=======
		reinit_completion(&dev->power.completion);
>>>>>>> refs/remotes/origin/master
		if (is_async(dev)) {
			get_device(dev);
			async_schedule(async_resume, dev);
		}
	}

	while (!list_empty(&dpm_suspended_list)) {
		dev = to_device(dpm_suspended_list.next);
		get_device(dev);
		if (!is_async(dev)) {
			int error;

			mutex_unlock(&dpm_list_mtx);

			error = device_resume(dev, state, false);
<<<<<<< HEAD
<<<<<<< HEAD
			if (error)
				pm_dev_err(dev, state, "", error);
=======
=======
>>>>>>> refs/remotes/origin/master
			if (error) {
				suspend_stats.failed_resume++;
				dpm_save_failed_step(SUSPEND_RESUME);
				dpm_save_failed_dev(dev_name(dev));
				pm_dev_err(dev, state, "", error);
			}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

			mutex_lock(&dpm_list_mtx);
		}
		if (!list_empty(&dev->power.entry))
			list_move_tail(&dev->power.entry, &dpm_prepared_list);
		put_device(dev);
	}
	mutex_unlock(&dpm_list_mtx);
	async_synchronize_full();
	dpm_show_time(starttime, state, NULL);
}

/**
 * device_complete - Complete a PM transition for given device.
 * @dev: Device to handle.
 * @state: PM transition of the system being carried out.
 */
static void device_complete(struct device *dev, pm_message_t state)
{
<<<<<<< HEAD
<<<<<<< HEAD
	device_lock(dev);

	if (dev->pwr_domain) {
		pm_dev_dbg(dev, state, "completing power domain ");
		if (dev->pwr_domain->ops.complete)
			dev->pwr_domain->ops.complete(dev);
	} else if (dev->type && dev->type->pm) {
		pm_dev_dbg(dev, state, "completing type ");
		if (dev->type->pm->complete)
			dev->type->pm->complete(dev);
	} else if (dev->class && dev->class->pm) {
		pm_dev_dbg(dev, state, "completing class ");
		if (dev->class->pm->complete)
			dev->class->pm->complete(dev);
	} else if (dev->bus && dev->bus->pm) {
		pm_dev_dbg(dev, state, "completing ");
		if (dev->bus->pm->complete)
			dev->bus->pm->complete(dev);
=======
	void (*callback)(struct device *) = NULL;
	char *info = NULL;

=======
	void (*callback)(struct device *) = NULL;
	char *info = NULL;

	if (dev->power.syscore)
		return;

>>>>>>> refs/remotes/origin/master
	device_lock(dev);

	if (dev->pm_domain) {
		info = "completing power domain ";
		callback = dev->pm_domain->ops.complete;
	} else if (dev->type && dev->type->pm) {
		info = "completing type ";
		callback = dev->type->pm->complete;
	} else if (dev->class && dev->class->pm) {
		info = "completing class ";
		callback = dev->class->pm->complete;
	} else if (dev->bus && dev->bus->pm) {
		info = "completing bus ";
		callback = dev->bus->pm->complete;
	}

	if (!callback && dev->driver && dev->driver->pm) {
		info = "completing driver ";
		callback = dev->driver->pm->complete;
	}

	if (callback) {
		pm_dev_dbg(dev, state, info);
		callback(dev);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	}

	device_unlock(dev);
=======
	}

	device_unlock(dev);

	pm_runtime_put(dev);
>>>>>>> refs/remotes/origin/master
}

/**
 * dpm_complete - Complete a PM transition for all non-sysdev devices.
 * @state: PM transition of the system being carried out.
 *
 * Execute the ->complete() callbacks for all devices whose PM status is not
 * DPM_ON (this allows new devices to be registered).
 */
void dpm_complete(pm_message_t state)
{
	struct list_head list;

	might_sleep();

	INIT_LIST_HEAD(&list);
	mutex_lock(&dpm_list_mtx);
	while (!list_empty(&dpm_prepared_list)) {
		struct device *dev = to_device(dpm_prepared_list.prev);

		get_device(dev);
		dev->power.is_prepared = false;
		list_move(&dev->power.entry, &list);
		mutex_unlock(&dpm_list_mtx);

		device_complete(dev, state);

		mutex_lock(&dpm_list_mtx);
		put_device(dev);
	}
	list_splice(&list, &dpm_list);
	mutex_unlock(&dpm_list_mtx);
}

/**
 * dpm_resume_end - Execute "resume" callbacks and complete system transition.
 * @state: PM transition of the system being carried out.
 *
 * Execute "resume" callbacks for all devices and complete the PM transition of
 * the system.
 */
void dpm_resume_end(pm_message_t state)
{
	dpm_resume(state);
	dpm_complete(state);
}
EXPORT_SYMBOL_GPL(dpm_resume_end);


/*------------------------- Suspend routines -------------------------*/

/**
 * resume_event - Return a "resume" message for given "suspend" sleep state.
 * @sleep_state: PM message representing a sleep state.
 *
 * Return a PM message representing the resume event corresponding to given
 * sleep state.
 */
static pm_message_t resume_event(pm_message_t sleep_state)
{
	switch (sleep_state.event) {
	case PM_EVENT_SUSPEND:
		return PMSG_RESUME;
	case PM_EVENT_FREEZE:
	case PM_EVENT_QUIESCE:
		return PMSG_RECOVER;
	case PM_EVENT_HIBERNATE:
		return PMSG_RESTORE;
	}
	return PMSG_ON;
}

/**
 * device_suspend_noirq - Execute a "late suspend" callback for given device.
 * @dev: Device to handle.
 * @state: PM transition of the system being carried out.
 *
 * The driver of @dev will not receive interrupts while this function is being
 * executed.
 */
static int device_suspend_noirq(struct device *dev, pm_message_t state)
{
<<<<<<< HEAD
<<<<<<< HEAD
	int error;

	if (dev->pwr_domain) {
		pm_dev_dbg(dev, state, "LATE power domain ");
		error = pm_noirq_op(dev, &dev->pwr_domain->ops, state);
		if (error)
			return error;
	} else if (dev->type && dev->type->pm) {
		pm_dev_dbg(dev, state, "LATE type ");
		error = pm_noirq_op(dev, dev->type->pm, state);
		if (error)
			return error;
	} else if (dev->class && dev->class->pm) {
		pm_dev_dbg(dev, state, "LATE class ");
		error = pm_noirq_op(dev, dev->class->pm, state);
		if (error)
			return error;
	} else if (dev->bus && dev->bus->pm) {
		pm_dev_dbg(dev, state, "LATE ");
		error = pm_noirq_op(dev, dev->bus->pm, state);
		if (error)
			return error;
	}

	return 0;
}

/**
 * dpm_suspend_noirq - Execute "late suspend" callbacks for non-sysdev devices.
=======
	pm_callback_t callback = NULL;
	char *info = NULL;

=======
	pm_callback_t callback = NULL;
	char *info = NULL;

	if (dev->power.syscore)
		return 0;

>>>>>>> refs/remotes/origin/master
	if (dev->pm_domain) {
		info = "noirq power domain ";
		callback = pm_noirq_op(&dev->pm_domain->ops, state);
	} else if (dev->type && dev->type->pm) {
		info = "noirq type ";
		callback = pm_noirq_op(dev->type->pm, state);
	} else if (dev->class && dev->class->pm) {
		info = "noirq class ";
		callback = pm_noirq_op(dev->class->pm, state);
	} else if (dev->bus && dev->bus->pm) {
		info = "noirq bus ";
		callback = pm_noirq_op(dev->bus->pm, state);
	}

	if (!callback && dev->driver && dev->driver->pm) {
		info = "noirq driver ";
		callback = pm_noirq_op(dev->driver->pm, state);
	}

	return dpm_run_callback(callback, dev, state, info);
}

/**
 * dpm_suspend_noirq - Execute "noirq suspend" callbacks for all devices.
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * @state: PM transition of the system being carried out.
 *
 * Prevent device drivers from receiving interrupts and call the "noirq" suspend
 * handlers for all non-sysdev devices.
 */
<<<<<<< HEAD
<<<<<<< HEAD
int dpm_suspend_noirq(pm_message_t state)
=======
static int dpm_suspend_noirq(pm_message_t state)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int dpm_suspend_noirq(pm_message_t state)
>>>>>>> refs/remotes/origin/master
{
	ktime_t starttime = ktime_get();
	int error = 0;

<<<<<<< HEAD
	suspend_device_irqs();
	mutex_lock(&dpm_list_mtx);
<<<<<<< HEAD
	while (!list_empty(&dpm_suspended_list)) {
		struct device *dev = to_device(dpm_suspended_list.prev);
=======
	while (!list_empty(&dpm_late_early_list)) {
		struct device *dev = to_device(dpm_late_early_list.prev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	cpuidle_pause();
	suspend_device_irqs();
	mutex_lock(&dpm_list_mtx);
	while (!list_empty(&dpm_late_early_list)) {
		struct device *dev = to_device(dpm_late_early_list.prev);
>>>>>>> refs/remotes/origin/master

		get_device(dev);
		mutex_unlock(&dpm_list_mtx);

		error = device_suspend_noirq(dev, state);

		mutex_lock(&dpm_list_mtx);
		if (error) {
<<<<<<< HEAD
<<<<<<< HEAD
			pm_dev_err(dev, state, " late", error);
=======
=======
>>>>>>> refs/remotes/origin/master
			pm_dev_err(dev, state, " noirq", error);
			suspend_stats.failed_suspend_noirq++;
			dpm_save_failed_step(SUSPEND_SUSPEND_NOIRQ);
			dpm_save_failed_dev(dev_name(dev));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			put_device(dev);
			break;
		}
		if (!list_empty(&dev->power.entry))
			list_move(&dev->power.entry, &dpm_noirq_list);
		put_device(dev);
<<<<<<< HEAD
=======

		if (pm_wakeup_pending()) {
			error = -EBUSY;
			break;
		}
>>>>>>> refs/remotes/origin/master
	}
	mutex_unlock(&dpm_list_mtx);
	if (error)
		dpm_resume_noirq(resume_event(state));
	else
<<<<<<< HEAD
<<<<<<< HEAD
		dpm_show_time(starttime, state, "late");
	return error;
}
EXPORT_SYMBOL_GPL(dpm_suspend_noirq);
=======
=======
>>>>>>> refs/remotes/origin/master
		dpm_show_time(starttime, state, "noirq");
	return error;
}

/**
 * device_suspend_late - Execute a "late suspend" callback for given device.
 * @dev: Device to handle.
 * @state: PM transition of the system being carried out.
 *
 * Runtime PM is disabled for @dev while this function is being executed.
 */
static int device_suspend_late(struct device *dev, pm_message_t state)
{
	pm_callback_t callback = NULL;
	char *info = NULL;

<<<<<<< HEAD
=======
	__pm_runtime_disable(dev, false);

	if (dev->power.syscore)
		return 0;

>>>>>>> refs/remotes/origin/master
	if (dev->pm_domain) {
		info = "late power domain ";
		callback = pm_late_early_op(&dev->pm_domain->ops, state);
	} else if (dev->type && dev->type->pm) {
		info = "late type ";
		callback = pm_late_early_op(dev->type->pm, state);
	} else if (dev->class && dev->class->pm) {
		info = "late class ";
		callback = pm_late_early_op(dev->class->pm, state);
	} else if (dev->bus && dev->bus->pm) {
		info = "late bus ";
		callback = pm_late_early_op(dev->bus->pm, state);
	}

	if (!callback && dev->driver && dev->driver->pm) {
		info = "late driver ";
		callback = pm_late_early_op(dev->driver->pm, state);
	}

	return dpm_run_callback(callback, dev, state, info);
}

/**
 * dpm_suspend_late - Execute "late suspend" callbacks for all devices.
 * @state: PM transition of the system being carried out.
 */
static int dpm_suspend_late(pm_message_t state)
{
	ktime_t starttime = ktime_get();
	int error = 0;

	mutex_lock(&dpm_list_mtx);
	while (!list_empty(&dpm_suspended_list)) {
		struct device *dev = to_device(dpm_suspended_list.prev);

		get_device(dev);
		mutex_unlock(&dpm_list_mtx);

		error = device_suspend_late(dev, state);

		mutex_lock(&dpm_list_mtx);
		if (error) {
			pm_dev_err(dev, state, " late", error);
			suspend_stats.failed_suspend_late++;
			dpm_save_failed_step(SUSPEND_SUSPEND_LATE);
			dpm_save_failed_dev(dev_name(dev));
			put_device(dev);
			break;
		}
		if (!list_empty(&dev->power.entry))
			list_move(&dev->power.entry, &dpm_late_early_list);
		put_device(dev);
<<<<<<< HEAD
=======

		if (pm_wakeup_pending()) {
			error = -EBUSY;
			break;
		}
>>>>>>> refs/remotes/origin/master
	}
	mutex_unlock(&dpm_list_mtx);
	if (error)
		dpm_resume_early(resume_event(state));
	else
		dpm_show_time(starttime, state, "late");

	return error;
}

/**
 * dpm_suspend_end - Execute "late" and "noirq" device suspend callbacks.
 * @state: PM transition of the system being carried out.
 */
int dpm_suspend_end(pm_message_t state)
{
	int error = dpm_suspend_late(state);
	if (error)
		return error;

	error = dpm_suspend_noirq(state);
	if (error) {
		dpm_resume_early(resume_event(state));
		return error;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(dpm_suspend_end);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

/**
 * legacy_suspend - Execute a legacy (bus or class) suspend callback for device.
 * @dev: Device to suspend.
 * @state: PM transition of the system being carried out.
 * @cb: Suspend callback to execute.
 */
static int legacy_suspend(struct device *dev, pm_message_t state,
<<<<<<< HEAD
			  int (*cb)(struct device *dev, pm_message_t state))
=======
			  int (*cb)(struct device *dev, pm_message_t state),
			  char *info)
>>>>>>> refs/remotes/origin/master
{
	int error;
	ktime_t calltime;

	calltime = initcall_debug_start(dev);

	error = cb(dev, state);
	suspend_report_result(cb, error);

<<<<<<< HEAD
	initcall_debug_report(dev, calltime, error);
=======
	initcall_debug_report(dev, calltime, error, state, info);
>>>>>>> refs/remotes/origin/master

	return error;
}

/**
 * device_suspend - Execute "suspend" callbacks for given device.
 * @dev: Device to handle.
 * @state: PM transition of the system being carried out.
 * @async: If true, the device is being suspended asynchronously.
 */
static int __device_suspend(struct device *dev, pm_message_t state, bool async)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	pm_callback_t callback = NULL;
	char *info = NULL;
>>>>>>> refs/remotes/origin/cm-10.0
	int error = 0;
	struct timer_list timer;
	struct dpm_drv_wd_data data;

	dpm_wait_for_children(dev, async);

<<<<<<< HEAD
=======
	if (async_error)
		goto Complete;

	pm_runtime_get_noresume(dev);
=======
	pm_callback_t callback = NULL;
	char *info = NULL;
	int error = 0;
	DECLARE_DPM_WATCHDOG_ON_STACK(wd);

	dpm_wait_for_children(dev, async);

	if (async_error)
		goto Complete;

	/*
	 * If a device configured to wake up the system from sleep states
	 * has been suspended at run time and there's a resume request pending
	 * for it, this is equivalent to the device signaling wakeup, so the
	 * system suspend operation should be aborted.
	 */
>>>>>>> refs/remotes/origin/master
	if (pm_runtime_barrier(dev) && device_may_wakeup(dev))
		pm_wakeup_event(dev, 0);

	if (pm_wakeup_pending()) {
<<<<<<< HEAD
		pm_runtime_put_sync(dev);
=======
>>>>>>> refs/remotes/origin/master
		async_error = -EBUSY;
		goto Complete;
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	data.dev = dev;
	data.tsk = get_current();
	init_timer_on_stack(&timer);
	timer.expires = jiffies + HZ * 12;
	timer.function = dpm_drv_timeout;
	timer.data = (unsigned long)&data;
	add_timer(&timer);

	device_lock(dev);

<<<<<<< HEAD
	if (async_error)
		goto Unlock;

	if (pm_wakeup_pending()) {
		async_error = -EBUSY;
		goto Unlock;
	}

	if (dev->pwr_domain) {
		pm_dev_dbg(dev, state, "power domain ");
		error = pm_op(dev, &dev->pwr_domain->ops, state);
		goto End;
	}

	if (dev->type && dev->type->pm) {
		pm_dev_dbg(dev, state, "type ");
		error = pm_op(dev, dev->type->pm, state);
		goto End;
=======
=======
	if (dev->power.syscore)
		goto Complete;

	dpm_watchdog_set(&wd, dev);
	device_lock(dev);

>>>>>>> refs/remotes/origin/master
	if (dev->pm_domain) {
		info = "power domain ";
		callback = pm_op(&dev->pm_domain->ops, state);
		goto Run;
	}

	if (dev->type && dev->type->pm) {
		info = "type ";
		callback = pm_op(dev->type->pm, state);
		goto Run;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	if (dev->class) {
		if (dev->class->pm) {
<<<<<<< HEAD
<<<<<<< HEAD
			pm_dev_dbg(dev, state, "class ");
			error = pm_op(dev, dev->class->pm, state);
			goto End;
=======
			info = "class ";
			callback = pm_op(dev->class->pm, state);
			goto Run;
>>>>>>> refs/remotes/origin/cm-10.0
		} else if (dev->class->suspend) {
			pm_dev_dbg(dev, state, "legacy class ");
			error = legacy_suspend(dev, state, dev->class->suspend);
=======
			info = "class ";
			callback = pm_op(dev->class->pm, state);
			goto Run;
		} else if (dev->class->suspend) {
			pm_dev_dbg(dev, state, "legacy class ");
			error = legacy_suspend(dev, state, dev->class->suspend,
						"legacy class ");
>>>>>>> refs/remotes/origin/master
			goto End;
		}
	}

	if (dev->bus) {
		if (dev->bus->pm) {
<<<<<<< HEAD
<<<<<<< HEAD
			pm_dev_dbg(dev, state, "");
			error = pm_op(dev, dev->bus->pm, state);
		} else if (dev->bus->suspend) {
			pm_dev_dbg(dev, state, "legacy ");
			error = legacy_suspend(dev, state, dev->bus->suspend);
		}
	}

 End:
	dev->power.is_suspended = !error;

 Unlock:
=======
=======
>>>>>>> refs/remotes/origin/master
			info = "bus ";
			callback = pm_op(dev->bus->pm, state);
		} else if (dev->bus->suspend) {
			pm_dev_dbg(dev, state, "legacy bus ");
<<<<<<< HEAD
			error = legacy_suspend(dev, state, dev->bus->suspend);
=======
			error = legacy_suspend(dev, state, dev->bus->suspend,
						"legacy bus ");
>>>>>>> refs/remotes/origin/master
			goto End;
		}
	}

 Run:
	if (!callback && dev->driver && dev->driver->pm) {
		info = "driver ";
		callback = pm_op(dev->driver->pm, state);
	}

	error = dpm_run_callback(callback, dev, state, info);

 End:
	if (!error) {
		dev->power.is_suspended = true;
		if (dev->power.wakeup_path
		    && dev->parent && !dev->parent->power.ignore_children)
			dev->parent->power.wakeup_path = true;
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	device_unlock(dev);

	del_timer_sync(&timer);
	destroy_timer_on_stack(&timer);

<<<<<<< HEAD
	complete_all(&dev->power.completion);

	if (error)
		async_error = error;
=======
 Complete:
	complete_all(&dev->power.completion);

	if (error) {
		pm_runtime_put_sync(dev);
		async_error = error;
	} else if (dev->power.is_suspended) {
		__pm_runtime_disable(dev, false);
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
	device_unlock(dev);
	dpm_watchdog_clear(&wd);

 Complete:
	complete_all(&dev->power.completion);
	if (error)
		async_error = error;
>>>>>>> refs/remotes/origin/master

	return error;
}

static void async_suspend(void *data, async_cookie_t cookie)
{
	struct device *dev = (struct device *)data;
	int error;

	error = __device_suspend(dev, pm_transition, true);
<<<<<<< HEAD
<<<<<<< HEAD
	if (error)
		pm_dev_err(dev, pm_transition, " async", error);
=======
=======
>>>>>>> refs/remotes/origin/master
	if (error) {
		dpm_save_failed_dev(dev_name(dev));
		pm_dev_err(dev, pm_transition, " async", error);
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	put_device(dev);
}

static int device_suspend(struct device *dev)
{
<<<<<<< HEAD
	INIT_COMPLETION(dev->power.completion);
=======
	reinit_completion(&dev->power.completion);
>>>>>>> refs/remotes/origin/master

	if (pm_async_enabled && dev->power.async_suspend) {
		get_device(dev);
		async_schedule(async_suspend, dev);
		return 0;
	}

	return __device_suspend(dev, pm_transition, false);
}

/**
 * dpm_suspend - Execute "suspend" callbacks for all non-sysdev devices.
 * @state: PM transition of the system being carried out.
 */
int dpm_suspend(pm_message_t state)
{
	ktime_t starttime = ktime_get();
	int error = 0;

	might_sleep();

	mutex_lock(&dpm_list_mtx);
	pm_transition = state;
	async_error = 0;
	while (!list_empty(&dpm_prepared_list)) {
		struct device *dev = to_device(dpm_prepared_list.prev);

		get_device(dev);
		mutex_unlock(&dpm_list_mtx);

		error = device_suspend(dev);

		mutex_lock(&dpm_list_mtx);
		if (error) {
			pm_dev_err(dev, state, "", error);
<<<<<<< HEAD
<<<<<<< HEAD
=======
			dpm_save_failed_dev(dev_name(dev));
>>>>>>> refs/remotes/origin/cm-10.0
=======
			dpm_save_failed_dev(dev_name(dev));
>>>>>>> refs/remotes/origin/master
			put_device(dev);
			break;
		}
		if (!list_empty(&dev->power.entry))
			list_move(&dev->power.entry, &dpm_suspended_list);
		put_device(dev);
		if (async_error)
			break;
	}
	mutex_unlock(&dpm_list_mtx);
	async_synchronize_full();
	if (!error)
		error = async_error;
<<<<<<< HEAD
<<<<<<< HEAD
	if (!error)
=======
=======
>>>>>>> refs/remotes/origin/master
	if (error) {
		suspend_stats.failed_suspend++;
		dpm_save_failed_step(SUSPEND_SUSPEND);
	} else
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		dpm_show_time(starttime, state, NULL);
	return error;
}

/**
 * device_prepare - Prepare a device for system power transition.
 * @dev: Device to handle.
 * @state: PM transition of the system being carried out.
 *
 * Execute the ->prepare() callback(s) for given device.  No new children of the
 * device may be registered after this function has returned.
 */
static int device_prepare(struct device *dev, pm_message_t state)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	int (*callback)(struct device *) = NULL;
	char *info = NULL;
>>>>>>> refs/remotes/origin/cm-10.0
	int error = 0;

	device_lock(dev);

<<<<<<< HEAD
	if (dev->pwr_domain) {
		pm_dev_dbg(dev, state, "preparing power domain ");
		if (dev->pwr_domain->ops.prepare)
			error = dev->pwr_domain->ops.prepare(dev);
		suspend_report_result(dev->pwr_domain->ops.prepare, error);
		if (error)
			goto End;
	} else if (dev->type && dev->type->pm) {
		pm_dev_dbg(dev, state, "preparing type ");
		if (dev->type->pm->prepare)
			error = dev->type->pm->prepare(dev);
		suspend_report_result(dev->type->pm->prepare, error);
		if (error)
			goto End;
	} else if (dev->class && dev->class->pm) {
		pm_dev_dbg(dev, state, "preparing class ");
		if (dev->class->pm->prepare)
			error = dev->class->pm->prepare(dev);
		suspend_report_result(dev->class->pm->prepare, error);
		if (error)
			goto End;
	} else if (dev->bus && dev->bus->pm) {
		pm_dev_dbg(dev, state, "preparing ");
		if (dev->bus->pm->prepare)
			error = dev->bus->pm->prepare(dev);
		suspend_report_result(dev->bus->pm->prepare, error);
	}

 End:
=======
=======
	int (*callback)(struct device *) = NULL;
	char *info = NULL;
	int error = 0;

	if (dev->power.syscore)
		return 0;

	/*
	 * If a device's parent goes into runtime suspend at the wrong time,
	 * it won't be possible to resume the device.  To prevent this we
	 * block runtime suspend here, during the prepare phase, and allow
	 * it again during the complete phase.
	 */
	pm_runtime_get_noresume(dev);

	device_lock(dev);

>>>>>>> refs/remotes/origin/master
	dev->power.wakeup_path = device_may_wakeup(dev);

	if (dev->pm_domain) {
		info = "preparing power domain ";
		callback = dev->pm_domain->ops.prepare;
	} else if (dev->type && dev->type->pm) {
		info = "preparing type ";
		callback = dev->type->pm->prepare;
	} else if (dev->class && dev->class->pm) {
		info = "preparing class ";
		callback = dev->class->pm->prepare;
	} else if (dev->bus && dev->bus->pm) {
		info = "preparing bus ";
		callback = dev->bus->pm->prepare;
	}

	if (!callback && dev->driver && dev->driver->pm) {
		info = "preparing driver ";
		callback = dev->driver->pm->prepare;
	}

	if (callback) {
		error = callback(dev);
		suspend_report_result(callback, error);
	}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	device_unlock(dev);

=======
	device_unlock(dev);

	if (error)
		pm_runtime_put(dev);

>>>>>>> refs/remotes/origin/master
	return error;
}

/**
 * dpm_prepare - Prepare all non-sysdev devices for a system PM transition.
 * @state: PM transition of the system being carried out.
 *
 * Execute the ->prepare() callback(s) for all devices.
 */
int dpm_prepare(pm_message_t state)
{
	int error = 0;

	might_sleep();

	mutex_lock(&dpm_list_mtx);
	while (!list_empty(&dpm_list)) {
		struct device *dev = to_device(dpm_list.next);

		get_device(dev);
		mutex_unlock(&dpm_list_mtx);

<<<<<<< HEAD
<<<<<<< HEAD
		pm_runtime_get_noresume(dev);
		if (pm_runtime_barrier(dev) && device_may_wakeup(dev))
			pm_wakeup_event(dev, 0);

		pm_runtime_put_sync(dev);
		error = pm_wakeup_pending() ?
				-EBUSY : device_prepare(dev, state);
=======
		error = device_prepare(dev, state);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		error = device_prepare(dev, state);
>>>>>>> refs/remotes/origin/master

		mutex_lock(&dpm_list_mtx);
		if (error) {
			if (error == -EAGAIN) {
				put_device(dev);
				error = 0;
				continue;
			}
			printk(KERN_INFO "PM: Device %s not prepared "
				"for power transition: code %d\n",
				dev_name(dev), error);
			put_device(dev);
			break;
		}
		dev->power.is_prepared = true;
		if (!list_empty(&dev->power.entry))
			list_move_tail(&dev->power.entry, &dpm_prepared_list);
		put_device(dev);
	}
	mutex_unlock(&dpm_list_mtx);
	return error;
}

/**
 * dpm_suspend_start - Prepare devices for PM transition and suspend them.
 * @state: PM transition of the system being carried out.
 *
 * Prepare all non-sysdev devices for system PM transition and execute "suspend"
 * callbacks for them.
 */
int dpm_suspend_start(pm_message_t state)
{
	int error;

	error = dpm_prepare(state);
<<<<<<< HEAD
<<<<<<< HEAD
	if (!error)
=======
=======
>>>>>>> refs/remotes/origin/master
	if (error) {
		suspend_stats.failed_prepare++;
		dpm_save_failed_step(SUSPEND_PREPARE);
	} else
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		error = dpm_suspend(state);
	return error;
}
EXPORT_SYMBOL_GPL(dpm_suspend_start);

void __suspend_report_result(const char *function, void *fn, int ret)
{
	if (ret)
		printk(KERN_ERR "%s(): %pF returns %d\n", function, fn, ret);
}
EXPORT_SYMBOL_GPL(__suspend_report_result);

/**
 * device_pm_wait_for_dev - Wait for suspend/resume of a device to complete.
 * @dev: Device to wait for.
 * @subordinate: Device that needs to wait for @dev.
 */
int device_pm_wait_for_dev(struct device *subordinate, struct device *dev)
{
	dpm_wait(dev, subordinate->power.async_suspend);
	return async_error;
}
EXPORT_SYMBOL_GPL(device_pm_wait_for_dev);
<<<<<<< HEAD
=======

/**
 * dpm_for_each_dev - device iterator.
 * @data: data for the callback.
 * @fn: function to be called for each device.
 *
 * Iterate over devices in dpm_list, and call @fn for each device,
 * passing it @data.
 */
void dpm_for_each_dev(void *data, void (*fn)(struct device *, void *))
{
	struct device *dev;

	if (!fn)
		return;

	device_pm_lock();
	list_for_each_entry(dev, &dpm_list, power.entry)
		fn(dev, data);
	device_pm_unlock();
}
EXPORT_SYMBOL_GPL(dpm_for_each_dev);
>>>>>>> refs/remotes/origin/master
