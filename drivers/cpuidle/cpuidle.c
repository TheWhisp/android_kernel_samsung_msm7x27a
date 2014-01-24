/*
 * cpuidle.c - core cpuidle infrastructure
 *
 * (C) 2006-2007 Venkatesh Pallipadi <venkatesh.pallipadi@intel.com>
 *               Shaohua Li <shaohua.li@intel.com>
 *               Adam Belay <abelay@novell.com>
 *
 * This code is licenced under the GPL.
 */

#include <linux/kernel.h>
#include <linux/mutex.h>
#include <linux/sched.h>
#include <linux/notifier.h>
<<<<<<< HEAD
#include <linux/pm_qos_params.h>
=======
#include <linux/pm_qos.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/cpu.h>
#include <linux/cpuidle.h>
#include <linux/ktime.h>
#include <linux/hrtimer.h>
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <trace/events/power.h>

#include "cpuidle.h"

DEFINE_PER_CPU(struct cpuidle_device *, cpuidle_devices);

DEFINE_MUTEX(cpuidle_lock);
LIST_HEAD(cpuidle_detected_devices);
<<<<<<< HEAD
static void (*pm_idle_old)(void);

static int enabled_devices;
=======

static int enabled_devices;
static int off __read_mostly;
static int initialized __read_mostly;

int cpuidle_disabled(void)
{
	return off;
}
void disable_cpuidle(void)
{
	off = 1;
}
>>>>>>> refs/remotes/origin/cm-10.0

#if defined(CONFIG_ARCH_HAS_CPU_IDLE_WAIT)
static void cpuidle_kick_cpus(void)
{
	cpu_idle_wait();
}
#elif defined(CONFIG_SMP)
# error "Arch needs cpu_idle_wait() equivalent here"
#else /* !CONFIG_ARCH_HAS_CPU_IDLE_WAIT && !CONFIG_SMP */
static void cpuidle_kick_cpus(void) {}
#endif

static int __cpuidle_register_device(struct cpuidle_device *dev);

<<<<<<< HEAD
=======
static inline int cpuidle_enter(struct cpuidle_device *dev,
				struct cpuidle_driver *drv, int index)
{
	struct cpuidle_state *target_state = &drv->states[index];
	return target_state->enter(dev, drv, index);
}

static inline int cpuidle_enter_tk(struct cpuidle_device *dev,
			       struct cpuidle_driver *drv, int index)
{
	return cpuidle_wrap_enter(dev, drv, index, cpuidle_enter);
}

typedef int (*cpuidle_enter_t)(struct cpuidle_device *dev,
			       struct cpuidle_driver *drv, int index);

static cpuidle_enter_t cpuidle_enter_ops;

/**
 * cpuidle_play_dead - cpu off-lining
 *
 * Returns in case of an error or no driver
 */
int cpuidle_play_dead(void)
{
	struct cpuidle_device *dev = __this_cpu_read(cpuidle_devices);
	struct cpuidle_driver *drv = cpuidle_get_driver();
	int i, dead_state = -1;
	int power_usage = -1;

	if (!drv)
		return -ENODEV;

	/* Find lowest-power state that supports long-term idle */
	for (i = CPUIDLE_DRIVER_STATE_START; i < drv->state_count; i++) {
		struct cpuidle_state *s = &drv->states[i];

		if (s->power_usage < power_usage && s->enter_dead) {
			power_usage = s->power_usage;
			dead_state = i;
		}
	}

	if (dead_state != -1)
		return drv->states[dead_state].enter_dead(dev, dead_state);

	return -ENODEV;
}

>>>>>>> refs/remotes/origin/cm-10.0
/**
 * cpuidle_idle_call - the main idle loop
 *
 * NOTE: no locks or semaphores should be used here
<<<<<<< HEAD
 */
static void cpuidle_idle_call(void)
{
	struct cpuidle_device *dev = __this_cpu_read(cpuidle_devices);
	struct cpuidle_state *target_state;
	int next_state;

	/* check if the device is ready */
	if (!dev || !dev->enabled) {
		if (pm_idle_old)
			pm_idle_old();
		else
#if defined(CONFIG_ARCH_HAS_DEFAULT_IDLE)
			default_idle();
#else
			local_irq_enable();
#endif
		return;
	}
=======
 * return non-zero on failure
 */
int cpuidle_idle_call(void)
{
	struct cpuidle_device *dev = __this_cpu_read(cpuidle_devices);
	struct cpuidle_driver *drv = cpuidle_get_driver();
	int next_state, entered_state;

	if (off)
		return -ENODEV;

	if (!initialized)
		return -ENODEV;

	/* check if the device is ready */
	if (!dev || !dev->enabled)
		return -EBUSY;
>>>>>>> refs/remotes/origin/cm-10.0

#if 0
	/* shows regressions, re-enable for 2.6.29 */
	/*
	 * run any timers that can be run now, at this point
	 * before calculating the idle duration etc.
	 */
	hrtimer_peek_ahead_timers();
#endif

<<<<<<< HEAD
	/*
	 * Call the device's prepare function before calling the
	 * governor's select function.  ->prepare gives the device's
	 * cpuidle driver a chance to update any dynamic information
	 * of its cpuidle states for the current idle period, e.g.
	 * state availability, latencies, residencies, etc.
	 */
	if (dev->prepare)
		dev->prepare(dev);

	/* ask the governor for the next state */
	next_state = cpuidle_curr_governor->select(dev);
	if (need_resched()) {
		local_irq_enable();
		return;
	}

	target_state = &dev->states[next_state];

	/* enter the state and update stats */
	dev->last_state = target_state;

	trace_power_start(POWER_CSTATE, next_state, dev->cpu);
	trace_cpu_idle(next_state, dev->cpu);

	dev->last_residency = target_state->enter(dev, target_state);

	trace_power_end(dev->cpu);
	trace_cpu_idle(PWR_EVENT_EXIT, dev->cpu);

	if (dev->last_state)
		target_state = dev->last_state;

	target_state->time += (unsigned long long)dev->last_residency;
	target_state->usage++;

	/* give the governor an opportunity to reflect on the outcome */
	if (cpuidle_curr_governor->reflect)
		cpuidle_curr_governor->reflect(dev);
=======
	/* ask the governor for the next state */
	next_state = cpuidle_curr_governor->select(drv, dev);
	if (need_resched()) {
		local_irq_enable();
		return 0;
	}

	trace_power_start_rcuidle(POWER_CSTATE, next_state, dev->cpu);
	trace_cpu_idle_rcuidle(next_state, dev->cpu);

	entered_state = cpuidle_enter_ops(dev, drv, next_state);

	trace_power_end_rcuidle(dev->cpu);
	trace_cpu_idle_rcuidle(PWR_EVENT_EXIT, dev->cpu);

	if (entered_state >= 0) {
		/* Update cpuidle counters */
		/* This can be moved to within driver enter routine
		 * but that results in multiple copies of same code.
		 */
		dev->states_usage[entered_state].time +=
				(unsigned long long)dev->last_residency;
		dev->states_usage[entered_state].usage++;
	} else {
		dev->last_residency = 0;
	}

	/* give the governor an opportunity to reflect on the outcome */
	if (cpuidle_curr_governor->reflect)
		cpuidle_curr_governor->reflect(dev, entered_state);

	return 0;
>>>>>>> refs/remotes/origin/cm-10.0
}

/**
 * cpuidle_install_idle_handler - installs the cpuidle idle loop handler
 */
void cpuidle_install_idle_handler(void)
{
<<<<<<< HEAD
	if (enabled_devices && (pm_idle != cpuidle_idle_call)) {
		/* Make sure all changes finished before we switch to new idle */
		smp_wmb();
		pm_idle = cpuidle_idle_call;
=======
	if (enabled_devices) {
		/* Make sure all changes finished before we switch to new idle */
		smp_wmb();
		initialized = 1;
>>>>>>> refs/remotes/origin/cm-10.0
	}
}

/**
 * cpuidle_uninstall_idle_handler - uninstalls the cpuidle idle loop handler
 */
void cpuidle_uninstall_idle_handler(void)
{
<<<<<<< HEAD
	if (enabled_devices && pm_idle_old && (pm_idle != pm_idle_old)) {
		pm_idle = pm_idle_old;
=======
	if (enabled_devices) {
		initialized = 0;
>>>>>>> refs/remotes/origin/cm-10.0
		cpuidle_kick_cpus();
	}
}

/**
 * cpuidle_pause_and_lock - temporarily disables CPUIDLE
 */
void cpuidle_pause_and_lock(void)
{
	mutex_lock(&cpuidle_lock);
	cpuidle_uninstall_idle_handler();
}

EXPORT_SYMBOL_GPL(cpuidle_pause_and_lock);

/**
 * cpuidle_resume_and_unlock - resumes CPUIDLE operation
 */
void cpuidle_resume_and_unlock(void)
{
	cpuidle_install_idle_handler();
	mutex_unlock(&cpuidle_lock);
}

EXPORT_SYMBOL_GPL(cpuidle_resume_and_unlock);

<<<<<<< HEAD
#ifdef CONFIG_ARCH_HAS_CPU_RELAX
static int poll_idle(struct cpuidle_device *dev, struct cpuidle_state *st)
{
	ktime_t	t1, t2;
	s64 diff;
	int ret;
=======
/**
 * cpuidle_wrap_enter - performs timekeeping and irqen around enter function
 * @dev: pointer to a valid cpuidle_device object
 * @drv: pointer to a valid cpuidle_driver object
 * @index: index of the target cpuidle state.
 */
int cpuidle_wrap_enter(struct cpuidle_device *dev,
				struct cpuidle_driver *drv, int index,
				int (*enter)(struct cpuidle_device *dev,
					struct cpuidle_driver *drv, int index))
{
	ktime_t time_start, time_end;
	s64 diff;

	time_start = ktime_get();

	index = enter(dev, drv, index);

	time_end = ktime_get();

	local_irq_enable();

	diff = ktime_to_us(ktime_sub(time_end, time_start));
	if (diff > INT_MAX)
		diff = INT_MAX;

	dev->last_residency = (int) diff;

	return index;
}

#ifdef CONFIG_ARCH_HAS_CPU_RELAX
static int poll_idle(struct cpuidle_device *dev,
		struct cpuidle_driver *drv, int index)
{
	ktime_t	t1, t2;
	s64 diff;
>>>>>>> refs/remotes/origin/cm-10.0

	t1 = ktime_get();
	local_irq_enable();
	while (!need_resched())
		cpu_relax();

	t2 = ktime_get();
	diff = ktime_to_us(ktime_sub(t2, t1));
	if (diff > INT_MAX)
		diff = INT_MAX;

<<<<<<< HEAD
	ret = (int) diff;
	return ret;
}

static void poll_idle_init(struct cpuidle_device *dev)
{
	struct cpuidle_state *state = &dev->states[0];

	cpuidle_set_statedata(state, NULL);
=======
	dev->last_residency = (int) diff;

	return index;
}

static void poll_idle_init(struct cpuidle_driver *drv)
{
	struct cpuidle_state *state = &drv->states[0];
>>>>>>> refs/remotes/origin/cm-10.0

	snprintf(state->name, CPUIDLE_NAME_LEN, "POLL");
	snprintf(state->desc, CPUIDLE_DESC_LEN, "CPUIDLE CORE POLL IDLE");
	state->exit_latency = 0;
	state->target_residency = 0;
	state->power_usage = -1;
	state->flags = 0;
	state->enter = poll_idle;
<<<<<<< HEAD
}
#else
static void poll_idle_init(struct cpuidle_device *dev) {}
=======
	state->disable = 0;
}
#else
static void poll_idle_init(struct cpuidle_driver *drv) {}
>>>>>>> refs/remotes/origin/cm-10.0
#endif /* CONFIG_ARCH_HAS_CPU_RELAX */

/**
 * cpuidle_enable_device - enables idle PM for a CPU
 * @dev: the CPU
 *
 * This function must be called between cpuidle_pause_and_lock and
 * cpuidle_resume_and_unlock when used externally.
 */
int cpuidle_enable_device(struct cpuidle_device *dev)
{
	int ret, i;
<<<<<<< HEAD

	if (dev->enabled)
		return 0;
	if (!cpuidle_get_driver() || !cpuidle_curr_governor)
		return -EIO;
	if (!dev->state_count)
		return -EINVAL;
=======
	struct cpuidle_driver *drv = cpuidle_get_driver();

	if (dev->enabled)
		return 0;
	if (!drv || !cpuidle_curr_governor)
		return -EIO;
	if (!dev->state_count)
		dev->state_count = drv->state_count;
>>>>>>> refs/remotes/origin/cm-10.0

	if (dev->registered == 0) {
		ret = __cpuidle_register_device(dev);
		if (ret)
			return ret;
	}

<<<<<<< HEAD
	poll_idle_init(dev);
=======
	cpuidle_enter_ops = drv->en_core_tk_irqen ?
		cpuidle_enter_tk : cpuidle_enter;

	poll_idle_init(drv);
>>>>>>> refs/remotes/origin/cm-10.0

	if ((ret = cpuidle_add_state_sysfs(dev)))
		return ret;

	if (cpuidle_curr_governor->enable &&
<<<<<<< HEAD
	    (ret = cpuidle_curr_governor->enable(dev)))
		goto fail_sysfs;

	for (i = 0; i < dev->state_count; i++) {
		dev->states[i].usage = 0;
		dev->states[i].time = 0;
	}
	dev->last_residency = 0;
	dev->last_state = NULL;
=======
	    (ret = cpuidle_curr_governor->enable(drv, dev)))
		goto fail_sysfs;

	for (i = 0; i < dev->state_count; i++) {
		dev->states_usage[i].usage = 0;
		dev->states_usage[i].time = 0;
	}
	dev->last_residency = 0;
>>>>>>> refs/remotes/origin/cm-10.0

	smp_wmb();

	dev->enabled = 1;

	enabled_devices++;
	return 0;

fail_sysfs:
	cpuidle_remove_state_sysfs(dev);

	return ret;
}

EXPORT_SYMBOL_GPL(cpuidle_enable_device);

/**
 * cpuidle_disable_device - disables idle PM for a CPU
 * @dev: the CPU
 *
 * This function must be called between cpuidle_pause_and_lock and
 * cpuidle_resume_and_unlock when used externally.
 */
void cpuidle_disable_device(struct cpuidle_device *dev)
{
	if (!dev->enabled)
		return;
	if (!cpuidle_get_driver() || !cpuidle_curr_governor)
		return;

	dev->enabled = 0;

	if (cpuidle_curr_governor->disable)
<<<<<<< HEAD
		cpuidle_curr_governor->disable(dev);
=======
		cpuidle_curr_governor->disable(cpuidle_get_driver(), dev);
>>>>>>> refs/remotes/origin/cm-10.0

	cpuidle_remove_state_sysfs(dev);
	enabled_devices--;
}

EXPORT_SYMBOL_GPL(cpuidle_disable_device);

/**
 * __cpuidle_register_device - internal register function called before register
 * and enable routines
 * @dev: the cpu
 *
 * cpuidle_lock mutex must be held before this is called
 */
static int __cpuidle_register_device(struct cpuidle_device *dev)
{
	int ret;
<<<<<<< HEAD
	struct sys_device *sys_dev = get_cpu_sysdev((unsigned long)dev->cpu);
	struct cpuidle_driver *cpuidle_driver = cpuidle_get_driver();

	if (!sys_dev)
=======
	struct device *cpu_dev = get_cpu_device((unsigned long)dev->cpu);
	struct cpuidle_driver *cpuidle_driver = cpuidle_get_driver();

	if (!dev)
>>>>>>> refs/remotes/origin/cm-10.0
		return -EINVAL;
	if (!try_module_get(cpuidle_driver->owner))
		return -EINVAL;

	init_completion(&dev->kobj_unregister);

<<<<<<< HEAD
	/*
	 * cpuidle driver should set the dev->power_specified bit
	 * before registering the device if the driver provides
	 * power_usage numbers.
	 *
	 * For those devices whose ->power_specified is not set,
	 * we fill in power_usage with decreasing values as the
	 * cpuidle code has an implicit assumption that state Cn
	 * uses less power than C(n-1).
	 *
	 * With CONFIG_ARCH_HAS_CPU_RELAX, C0 is already assigned
	 * an power value of -1.  So we use -2, -3, etc, for other
	 * c-states.
	 */
	if (!dev->power_specified) {
		int i;
		for (i = CPUIDLE_DRIVER_STATE_START; i < dev->state_count; i++)
			dev->states[i].power_usage = -1 - i;
	}

	per_cpu(cpuidle_devices, dev->cpu) = dev;
	list_add(&dev->device_list, &cpuidle_detected_devices);
	if ((ret = cpuidle_add_sysfs(sys_dev))) {
=======
	per_cpu(cpuidle_devices, dev->cpu) = dev;
	list_add(&dev->device_list, &cpuidle_detected_devices);
	if ((ret = cpuidle_add_sysfs(cpu_dev))) {
>>>>>>> refs/remotes/origin/cm-10.0
		module_put(cpuidle_driver->owner);
		return ret;
	}

	dev->registered = 1;
	return 0;
}

/**
 * cpuidle_register_device - registers a CPU's idle PM feature
 * @dev: the cpu
 */
int cpuidle_register_device(struct cpuidle_device *dev)
{
	int ret;

	mutex_lock(&cpuidle_lock);

	if ((ret = __cpuidle_register_device(dev))) {
		mutex_unlock(&cpuidle_lock);
		return ret;
	}

	cpuidle_enable_device(dev);
	cpuidle_install_idle_handler();

	mutex_unlock(&cpuidle_lock);

	return 0;

}

EXPORT_SYMBOL_GPL(cpuidle_register_device);

/**
 * cpuidle_unregister_device - unregisters a CPU's idle PM feature
 * @dev: the cpu
 */
void cpuidle_unregister_device(struct cpuidle_device *dev)
{
<<<<<<< HEAD
	struct sys_device *sys_dev = get_cpu_sysdev((unsigned long)dev->cpu);
=======
	struct device *cpu_dev = get_cpu_device((unsigned long)dev->cpu);
>>>>>>> refs/remotes/origin/cm-10.0
	struct cpuidle_driver *cpuidle_driver = cpuidle_get_driver();

	if (dev->registered == 0)
		return;

	cpuidle_pause_and_lock();

	cpuidle_disable_device(dev);

<<<<<<< HEAD
	cpuidle_remove_sysfs(sys_dev);
=======
	cpuidle_remove_sysfs(cpu_dev);
>>>>>>> refs/remotes/origin/cm-10.0
	list_del(&dev->device_list);
	wait_for_completion(&dev->kobj_unregister);
	per_cpu(cpuidle_devices, dev->cpu) = NULL;

	cpuidle_resume_and_unlock();

	module_put(cpuidle_driver->owner);
}

EXPORT_SYMBOL_GPL(cpuidle_unregister_device);

#ifdef CONFIG_SMP

static void smp_callback(void *v)
{
	/* we already woke the CPU up, nothing more to do */
}

/*
 * This function gets called when a part of the kernel has a new latency
 * requirement.  This means we need to get all processors out of their C-state,
 * and then recalculate a new suitable C-state. Just do a cross-cpu IPI; that
 * wakes them all right up.
 */
static int cpuidle_latency_notify(struct notifier_block *b,
		unsigned long l, void *v)
{
	smp_call_function(smp_callback, NULL, 1);
	return NOTIFY_OK;
}

static struct notifier_block cpuidle_latency_notifier = {
	.notifier_call = cpuidle_latency_notify,
};

static inline void latency_notifier_init(struct notifier_block *n)
{
	pm_qos_add_notifier(PM_QOS_CPU_DMA_LATENCY, n);
}

#else /* CONFIG_SMP */

#define latency_notifier_init(x) do { } while (0)

#endif /* CONFIG_SMP */

/**
 * cpuidle_init - core initializer
 */
static int __init cpuidle_init(void)
{
	int ret;

<<<<<<< HEAD
	pm_idle_old = pm_idle;

	ret = cpuidle_add_class_sysfs(&cpu_sysdev_class);
=======
	if (cpuidle_disabled())
		return -ENODEV;

	ret = cpuidle_add_interface(cpu_subsys.dev_root);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret)
		return ret;

	latency_notifier_init(&cpuidle_latency_notifier);

	return 0;
}

<<<<<<< HEAD
=======
module_param(off, int, 0444);
>>>>>>> refs/remotes/origin/cm-10.0
core_initcall(cpuidle_init);
