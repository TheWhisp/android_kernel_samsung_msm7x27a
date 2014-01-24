/*
 *  linux/drivers/cpufreq/cpufreq.c
 *
 *  Copyright (C) 2001 Russell King
 *            (C) 2002 - 2003 Dominik Brodowski <linux@brodo.de>
<<<<<<< HEAD
=======
 *            (C) 2013 Viresh Kumar <viresh.kumar@linaro.org>
>>>>>>> refs/remotes/origin/master
 *
 *  Oct 2005 - Ashok Raj <ashok.raj@intel.com>
 *	Added handling for CPU hotplug
 *  Feb 2006 - Jacob Shin <jacob.shin@amd.com>
 *	Fix handling for CPU hotplug -- affected CPUs
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
<<<<<<< HEAD
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/notifier.h>
#include <linux/cpufreq.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/cpu.h>
#include <linux/completion.h>
#include <linux/mutex.h>
#include <linux/syscore_ops.h>

=======
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/cpu.h>
#include <linux/cpufreq.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/kernel_stat.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/syscore_ops.h>
#include <linux/tick.h>
>>>>>>> refs/remotes/origin/master
#include <trace/events/power.h>

/**
 * The "cpufreq driver" - the arch- or hardware-dependent low
 * level driver of CPUFreq support, and its spinlock. This lock
 * also protects the cpufreq_cpu_data array.
 */
static struct cpufreq_driver *cpufreq_driver;
static DEFINE_PER_CPU(struct cpufreq_policy *, cpufreq_cpu_data);
<<<<<<< HEAD
#ifdef CONFIG_HOTPLUG_CPU
/* This one keeps track of the previously set governor of a removed CPU */
struct cpufreq_cpu_save_data {
	char gov[CPUFREQ_NAME_LEN];
	unsigned int max, min;
};
static DEFINE_PER_CPU(struct cpufreq_cpu_save_data, cpufreq_policy_save);
#endif
static DEFINE_SPINLOCK(cpufreq_driver_lock);

/*
 * cpu_policy_rwsem is a per CPU reader-writer semaphore designed to cure
 * all cpufreq/hotplug/workqueue/etc related lock issues.
 *
 * The rules for this semaphore:
 * - Any routine that wants to read from the policy structure will
 *   do a down_read on this semaphore.
 * - Any routine that will write to the policy structure and/or may take away
 *   the policy altogether (eg. CPU hotplug), will hold this lock in write
 *   mode before doing so.
 *
 * Additional rules:
 * - All holders of the lock should check to make sure that the CPU they
 *   are concerned with are online after they get the lock.
 * - Governor routines that can be called in cpufreq hotplug path should not
 *   take this sem as top level hotplug notifier handler takes this.
 * - Lock should not be held across
 *     __cpufreq_governor(data, CPUFREQ_GOV_STOP);
 */
static DEFINE_PER_CPU(int, cpufreq_policy_cpu);
static DEFINE_PER_CPU(struct rw_semaphore, cpu_policy_rwsem);

#define lock_policy_rwsem(mode, cpu)					\
int lock_policy_rwsem_##mode						\
(int cpu)								\
{									\
	int policy_cpu = per_cpu(cpufreq_policy_cpu, cpu);		\
	BUG_ON(policy_cpu == -1);					\
	down_##mode(&per_cpu(cpu_policy_rwsem, policy_cpu));		\
	if (unlikely(!cpu_online(cpu))) {				\
		up_##mode(&per_cpu(cpu_policy_rwsem, policy_cpu));	\
		return -1;						\
	}								\
									\
	return 0;							\
}

lock_policy_rwsem(read, cpu);

lock_policy_rwsem(write, cpu);

static void unlock_policy_rwsem_read(int cpu)
{
	int policy_cpu = per_cpu(cpufreq_policy_cpu, cpu);
	BUG_ON(policy_cpu == -1);
	up_read(&per_cpu(cpu_policy_rwsem, policy_cpu));
}

void unlock_policy_rwsem_write(int cpu)
{
	int policy_cpu = per_cpu(cpufreq_policy_cpu, cpu);
	BUG_ON(policy_cpu == -1);
	up_write(&per_cpu(cpu_policy_rwsem, policy_cpu));
}

=======
static DEFINE_PER_CPU(struct cpufreq_policy *, cpufreq_cpu_data_fallback);
static DEFINE_RWLOCK(cpufreq_driver_lock);
static DEFINE_MUTEX(cpufreq_governor_lock);
static LIST_HEAD(cpufreq_policy_list);

#ifdef CONFIG_HOTPLUG_CPU
/* This one keeps track of the previously set governor of a removed CPU */
static DEFINE_PER_CPU(char[CPUFREQ_NAME_LEN], cpufreq_cpu_governor);
#endif

static inline bool has_target(void)
{
	return cpufreq_driver->target_index || cpufreq_driver->target;
}

/*
 * rwsem to guarantee that cpufreq driver module doesn't unload during critical
 * sections
 */
static DECLARE_RWSEM(cpufreq_rwsem);
>>>>>>> refs/remotes/origin/master

/* internal prototypes */
static int __cpufreq_governor(struct cpufreq_policy *policy,
		unsigned int event);
static unsigned int __cpufreq_get(unsigned int cpu);
static void handle_update(struct work_struct *work);

/**
 * Two notifier lists: the "policy" list is involved in the
 * validation process for a new CPU frequency policy; the
 * "transition" list for kernel code that needs to handle
 * changes to devices when the CPU clock speed changes.
 * The mutex locks both lists.
 */
static BLOCKING_NOTIFIER_HEAD(cpufreq_policy_notifier_list);
static struct srcu_notifier_head cpufreq_transition_notifier_list;

static bool init_cpufreq_transition_notifier_list_called;
static int __init init_cpufreq_transition_notifier_list(void)
{
	srcu_init_notifier_head(&cpufreq_transition_notifier_list);
	init_cpufreq_transition_notifier_list_called = true;
	return 0;
}
pure_initcall(init_cpufreq_transition_notifier_list);

<<<<<<< HEAD
<<<<<<< HEAD
=======
static int off __read_mostly;
int cpufreq_disabled(void)
=======
static int off __read_mostly;
static int cpufreq_disabled(void)
>>>>>>> refs/remotes/origin/master
{
	return off;
}
void disable_cpufreq(void)
{
	off = 1;
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static LIST_HEAD(cpufreq_governor_list);
static DEFINE_MUTEX(cpufreq_governor_mutex);

static struct cpufreq_policy *__cpufreq_cpu_get(unsigned int cpu, int sysfs)
{
	struct cpufreq_policy *data;
	unsigned long flags;

	if (cpu >= nr_cpu_ids)
		goto err_out;

	/* get the cpufreq driver */
	spin_lock_irqsave(&cpufreq_driver_lock, flags);

	if (!cpufreq_driver)
		goto err_out_unlock;

	if (!try_module_get(cpufreq_driver->owner))
		goto err_out_unlock;


	/* get the CPU */
	data = per_cpu(cpufreq_cpu_data, cpu);

	if (!data)
		goto err_out_put_module;

	if (!sysfs && !kobject_get(&data->kobj))
		goto err_out_put_module;

	spin_unlock_irqrestore(&cpufreq_driver_lock, flags);
	return data;

err_out_put_module:
	module_put(cpufreq_driver->owner);
err_out_unlock:
	spin_unlock_irqrestore(&cpufreq_driver_lock, flags);
err_out:
	return NULL;
}

struct cpufreq_policy *cpufreq_cpu_get(unsigned int cpu)
{
	return __cpufreq_cpu_get(cpu, 0);
}
EXPORT_SYMBOL_GPL(cpufreq_cpu_get);

static struct cpufreq_policy *cpufreq_cpu_get_sysfs(unsigned int cpu)
{
	return __cpufreq_cpu_get(cpu, 1);
}
<<<<<<< HEAD

static void __cpufreq_cpu_put(struct cpufreq_policy *data, int sysfs)
{
	if (!sysfs)
		kobject_put(&data->kobj);
	module_put(cpufreq_driver->owner);
}
=======
>>>>>>> refs/remotes/origin/cm-11.0

static void __cpufreq_cpu_put(struct cpufreq_policy *data, int sysfs)
{
<<<<<<< HEAD
	__cpufreq_cpu_put(data, 0);
=======
	if (!sysfs)
		kobject_put(&data->kobj);
	module_put(cpufreq_driver->owner);
>>>>>>> refs/remotes/origin/cm-11.0
}

void cpufreq_cpu_put(struct cpufreq_policy *data)
{
	__cpufreq_cpu_put(data, 0);
}
EXPORT_SYMBOL_GPL(cpufreq_cpu_put);

static void cpufreq_cpu_put_sysfs(struct cpufreq_policy *data)
{
	__cpufreq_cpu_put(data, 1);
}
<<<<<<< HEAD
=======
static LIST_HEAD(cpufreq_governor_list);
static DEFINE_MUTEX(cpufreq_governor_mutex);

bool have_governor_per_policy(void)
{
	return !!(cpufreq_driver->flags & CPUFREQ_HAVE_GOVERNOR_PER_POLICY);
}
EXPORT_SYMBOL_GPL(have_governor_per_policy);

struct kobject *get_governor_parent_kobj(struct cpufreq_policy *policy)
{
	if (have_governor_per_policy())
		return &policy->kobj;
	else
		return cpufreq_global_kobject;
}
EXPORT_SYMBOL_GPL(get_governor_parent_kobj);

static inline u64 get_cpu_idle_time_jiffy(unsigned int cpu, u64 *wall)
{
	u64 idle_time;
	u64 cur_wall_time;
	u64 busy_time;

	cur_wall_time = jiffies64_to_cputime64(get_jiffies_64());

	busy_time = kcpustat_cpu(cpu).cpustat[CPUTIME_USER];
	busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_SYSTEM];
	busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_IRQ];
	busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_SOFTIRQ];
	busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_STEAL];
	busy_time += kcpustat_cpu(cpu).cpustat[CPUTIME_NICE];

	idle_time = cur_wall_time - busy_time;
	if (wall)
		*wall = cputime_to_usecs(cur_wall_time);

	return cputime_to_usecs(idle_time);
}

u64 get_cpu_idle_time(unsigned int cpu, u64 *wall, int io_busy)
{
	u64 idle_time = get_cpu_idle_time_us(cpu, io_busy ? wall : NULL);

	if (idle_time == -1ULL)
		return get_cpu_idle_time_jiffy(cpu, wall);
	else if (!io_busy)
		idle_time += get_cpu_iowait_time_us(cpu, wall);

	return idle_time;
}
EXPORT_SYMBOL_GPL(get_cpu_idle_time);

/*
 * This is a generic cpufreq init() routine which can be used by cpufreq
 * drivers of SMP systems. It will do following:
 * - validate & show freq table passed
 * - set policies transition latency
 * - policy->cpus with all possible CPUs
 */
int cpufreq_generic_init(struct cpufreq_policy *policy,
		struct cpufreq_frequency_table *table,
		unsigned int transition_latency)
{
	int ret;

	ret = cpufreq_table_validate_and_show(policy, table);
	if (ret) {
		pr_err("%s: invalid frequency table: %d\n", __func__, ret);
		return ret;
	}

	policy->cpuinfo.transition_latency = transition_latency;

	/*
	 * The driver only supports the SMP configuartion where all processors
	 * share the clock and voltage and clock.
	 */
	cpumask_setall(policy->cpus);

	return 0;
}
EXPORT_SYMBOL_GPL(cpufreq_generic_init);

struct cpufreq_policy *cpufreq_cpu_get(unsigned int cpu)
{
	struct cpufreq_policy *policy = NULL;
	unsigned long flags;

	if (cpufreq_disabled() || (cpu >= nr_cpu_ids))
		return NULL;

	if (!down_read_trylock(&cpufreq_rwsem))
		return NULL;

	/* get the cpufreq driver */
	read_lock_irqsave(&cpufreq_driver_lock, flags);

	if (cpufreq_driver) {
		/* get the CPU */
		policy = per_cpu(cpufreq_cpu_data, cpu);
		if (policy)
			kobject_get(&policy->kobj);
	}

	read_unlock_irqrestore(&cpufreq_driver_lock, flags);

	if (!policy)
		up_read(&cpufreq_rwsem);

	return policy;
}
EXPORT_SYMBOL_GPL(cpufreq_cpu_get);

void cpufreq_cpu_put(struct cpufreq_policy *policy)
{
	if (cpufreq_disabled())
		return;

	kobject_put(&policy->kobj);
	up_read(&cpufreq_rwsem);
}
EXPORT_SYMBOL_GPL(cpufreq_cpu_put);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

/*********************************************************************
 *            EXTERNALLY AFFECTING FREQUENCY CHANGES                 *
 *********************************************************************/

/**
 * adjust_jiffies - adjust the system "loops_per_jiffy"
 *
 * This function alters the system "loops_per_jiffy" for the clock
 * speed change. Note that loops_per_jiffy cannot be updated on SMP
 * systems as each CPU might be scaled differently. So, use the arch
 * per-CPU loops_per_jiffy value wherever possible.
 */
#ifndef CONFIG_SMP
static unsigned long l_p_j_ref;
<<<<<<< HEAD
static unsigned int  l_p_j_ref_freq;
=======
static unsigned int l_p_j_ref_freq;
>>>>>>> refs/remotes/origin/master

static void adjust_jiffies(unsigned long val, struct cpufreq_freqs *ci)
{
	if (ci->flags & CPUFREQ_CONST_LOOPS)
		return;

	if (!l_p_j_ref_freq) {
		l_p_j_ref = loops_per_jiffy;
		l_p_j_ref_freq = ci->old;
		pr_debug("saving %lu as reference value for loops_per_jiffy; "
			"freq is %u kHz\n", l_p_j_ref, l_p_j_ref_freq);
	}
<<<<<<< HEAD
<<<<<<< HEAD
	if ((val == CPUFREQ_PRECHANGE  && ci->old < ci->new) ||
	    (val == CPUFREQ_POSTCHANGE && ci->old > ci->new) ||
=======
	if ((val == CPUFREQ_POSTCHANGE  && ci->old != ci->new) ||
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if ((val == CPUFREQ_POSTCHANGE && ci->old != ci->new) ||
>>>>>>> refs/remotes/origin/master
	    (val == CPUFREQ_RESUMECHANGE || val == CPUFREQ_SUSPENDCHANGE)) {
		loops_per_jiffy = cpufreq_scale(l_p_j_ref, l_p_j_ref_freq,
								ci->new);
		pr_debug("scaling loops_per_jiffy to %lu "
			"for frequency %u kHz\n", loops_per_jiffy, ci->new);
	}
}
#else
static inline void adjust_jiffies(unsigned long val, struct cpufreq_freqs *ci)
{
	return;
}
#endif

<<<<<<< HEAD

/**
 * cpufreq_notify_transition - call notifier chain and adjust_jiffies
 * on frequency transition.
 *
 * This function calls the transition notifiers and the "adjust_jiffies"
 * function. It is called twice on all CPU frequency changes that have
 * external effects.
 */
void cpufreq_notify_transition(struct cpufreq_freqs *freqs, unsigned int state)
{
	struct cpufreq_policy *policy;

	BUG_ON(irqs_disabled());

=======
static void __cpufreq_notify_transition(struct cpufreq_policy *policy,
		struct cpufreq_freqs *freqs, unsigned int state)
{
	BUG_ON(irqs_disabled());

	if (cpufreq_disabled())
		return;

>>>>>>> refs/remotes/origin/master
	freqs->flags = cpufreq_driver->flags;
	pr_debug("notification %u of frequency transition to %u kHz\n",
		state, freqs->new);

<<<<<<< HEAD
	policy = per_cpu(cpufreq_cpu_data, freqs->cpu);
=======
>>>>>>> refs/remotes/origin/master
	switch (state) {

	case CPUFREQ_PRECHANGE:
		/* detect if the driver reported a value as "old frequency"
		 * which is not equal to what the cpufreq core thinks is
		 * "old frequency".
		 */
		if (!(cpufreq_driver->flags & CPUFREQ_CONST_LOOPS)) {
			if ((policy) && (policy->cpu == freqs->cpu) &&
			    (policy->cur) && (policy->cur != freqs->old)) {
				pr_debug("Warning: CPU frequency is"
					" %u, cpufreq assumed %u kHz.\n",
					freqs->old, policy->cur);
				freqs->old = policy->cur;
			}
		}
		srcu_notifier_call_chain(&cpufreq_transition_notifier_list,
				CPUFREQ_PRECHANGE, freqs);
		adjust_jiffies(CPUFREQ_PRECHANGE, freqs);
		break;

	case CPUFREQ_POSTCHANGE:
		adjust_jiffies(CPUFREQ_POSTCHANGE, freqs);
		pr_debug("FREQ: %lu - CPU: %lu", (unsigned long)freqs->new,
			(unsigned long)freqs->cpu);
<<<<<<< HEAD
		trace_power_frequency(POWER_PSTATE, freqs->new, freqs->cpu);
		trace_cpu_frequency(freqs->new, freqs->cpu);
		srcu_notifier_call_chain(&cpufreq_transition_notifier_list,
				CPUFREQ_POSTCHANGE, freqs);
		if (likely(policy) && likely(policy->cpu == freqs->cpu)) {
			policy->cur = freqs->new;
			sysfs_notify(&policy->kobj, NULL, "scaling_cur_freq");
		}
		break;
	}
}
EXPORT_SYMBOL_GPL(cpufreq_notify_transition);
/**
 * cpufreq_notify_utilization - notify CPU userspace about CPU utilization
 * change
 *
 * This function is called everytime the CPU load is evaluated by the
 * ondemand governor. It notifies userspace of cpu load changes via sysfs.
 */
void cpufreq_notify_utilization(struct cpufreq_policy *policy,
		unsigned int util)
{
	if (policy)
		policy->util = util;
<<<<<<< HEAD

	if (policy->util >= MIN_CPU_UTIL_NOTIFY)
		sysfs_notify(&policy->kobj, NULL, "cpu_utilization");

}
=======
		trace_cpu_frequency(freqs->new, freqs->cpu);
		srcu_notifier_call_chain(&cpufreq_transition_notifier_list,
				CPUFREQ_POSTCHANGE, freqs);
		if (likely(policy) && likely(policy->cpu == freqs->cpu))
			policy->cur = freqs->new;
		break;
	}
}

/**
 * cpufreq_notify_transition - call notifier chain and adjust_jiffies
 * on frequency transition.
 *
 * This function calls the transition notifiers and the "adjust_jiffies"
 * function. It is called twice on all CPU frequency changes that have
 * external effects.
 */
void cpufreq_notify_transition(struct cpufreq_policy *policy,
		struct cpufreq_freqs *freqs, unsigned int state)
{
	for_each_cpu(freqs->cpu, policy->cpus)
		__cpufreq_notify_transition(policy, freqs, state);
}
EXPORT_SYMBOL_GPL(cpufreq_notify_transition);

>>>>>>> refs/remotes/origin/master
=======

	if (policy->util >= MIN_CPU_UTIL_NOTIFY)
		sysfs_notify(&policy->kobj, NULL, "cpu_utilization");

}
>>>>>>> refs/remotes/origin/cm-11.0

/*********************************************************************
 *                          SYSFS INTERFACE                          *
 *********************************************************************/

static struct cpufreq_governor *__find_governor(const char *str_governor)
{
	struct cpufreq_governor *t;

	list_for_each_entry(t, &cpufreq_governor_list, governor_list)
		if (!strnicmp(str_governor, t->name, CPUFREQ_NAME_LEN))
			return t;

	return NULL;
}

/**
 * cpufreq_parse_governor - parse a governor string
 */
static int cpufreq_parse_governor(char *str_governor, unsigned int *policy,
				struct cpufreq_governor **governor)
{
	int err = -EINVAL;

	if (!cpufreq_driver)
		goto out;

	if (cpufreq_driver->setpolicy) {
		if (!strnicmp(str_governor, "performance", CPUFREQ_NAME_LEN)) {
			*policy = CPUFREQ_POLICY_PERFORMANCE;
			err = 0;
		} else if (!strnicmp(str_governor, "powersave",
						CPUFREQ_NAME_LEN)) {
			*policy = CPUFREQ_POLICY_POWERSAVE;
			err = 0;
		}
<<<<<<< HEAD
	} else if (cpufreq_driver->target) {
=======
	} else if (has_target()) {
>>>>>>> refs/remotes/origin/master
		struct cpufreq_governor *t;

		mutex_lock(&cpufreq_governor_mutex);

		t = __find_governor(str_governor);

		if (t == NULL) {
			int ret;

			mutex_unlock(&cpufreq_governor_mutex);
			ret = request_module("cpufreq_%s", str_governor);
			mutex_lock(&cpufreq_governor_mutex);

			if (ret == 0)
				t = __find_governor(str_governor);
		}

		if (t != NULL) {
			*governor = t;
			err = 0;
		}

		mutex_unlock(&cpufreq_governor_mutex);
	}
out:
	return err;
}

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
/**
 * cpufreq_per_cpu_attr_read() / show_##file_name() -
 * print out cpufreq information
 *
 * Write out information from cpufreq_driver->policy[cpu]; object must be
 * "unsigned int".
 */

#define show_one(file_name, object)			\
static ssize_t show_##file_name				\
(struct cpufreq_policy *policy, char *buf)		\
{							\
	return sprintf(buf, "%u\n", policy->object);	\
}

show_one(cpuinfo_min_freq, cpuinfo.min_freq);
show_one(cpuinfo_max_freq, cpuinfo.max_freq);
show_one(cpuinfo_transition_latency, cpuinfo.transition_latency);
show_one(scaling_min_freq, min);
show_one(scaling_max_freq, max);
show_one(scaling_cur_freq, cur);
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
show_one(cpu_utilization, util);

static int __cpufreq_set_policy(struct cpufreq_policy *data,
				struct cpufreq_policy *policy);
=======

static int cpufreq_set_policy(struct cpufreq_policy *policy,
				struct cpufreq_policy *new_policy);
>>>>>>> refs/remotes/origin/master

/**
 * cpufreq_per_cpu_attr_write() / store_##file_name() - sysfs write access
 */
#define store_one(file_name, object)			\
static ssize_t store_##file_name					\
(struct cpufreq_policy *policy, const char *buf, size_t count)		\
{									\
<<<<<<< HEAD
	unsigned int ret = -EINVAL;					\
=======
	int ret;							\
>>>>>>> refs/remotes/origin/master
	struct cpufreq_policy new_policy;				\
									\
	ret = cpufreq_get_policy(&new_policy, policy->cpu);		\
	if (ret)							\
		return -EINVAL;						\
									\
	ret = sscanf(buf, "%u", &new_policy.object);			\
	if (ret != 1)							\
		return -EINVAL;						\
									\
<<<<<<< HEAD
	ret = __cpufreq_set_policy(policy, &new_policy);		\
=======
	ret = cpufreq_set_policy(policy, &new_policy);		\
>>>>>>> refs/remotes/origin/master
	policy->user_policy.object = policy->object;			\
									\
	return ret ? ret : count;					\
}

store_one(scaling_min_freq, min);
store_one(scaling_max_freq, max);

/**
 * show_cpuinfo_cur_freq - current CPU frequency as detected by hardware
 */
static ssize_t show_cpuinfo_cur_freq(struct cpufreq_policy *policy,
					char *buf)
{
	unsigned int cur_freq = __cpufreq_get(policy->cpu);
	if (!cur_freq)
		return sprintf(buf, "<unknown>");
	return sprintf(buf, "%u\n", cur_freq);
}

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
/**
 * show_scaling_governor - show the current policy for the specified CPU
 */
static ssize_t show_scaling_governor(struct cpufreq_policy *policy, char *buf)
{
	if (policy->policy == CPUFREQ_POLICY_POWERSAVE)
		return sprintf(buf, "powersave\n");
	else if (policy->policy == CPUFREQ_POLICY_PERFORMANCE)
		return sprintf(buf, "performance\n");
	else if (policy->governor)
<<<<<<< HEAD
		return scnprintf(buf, CPUFREQ_NAME_LEN, "%s\n",
=======
		return scnprintf(buf, CPUFREQ_NAME_PLEN, "%s\n",
>>>>>>> refs/remotes/origin/master
				policy->governor->name);
	return -EINVAL;
}

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
/**
 * store_scaling_governor - store policy for the specified CPU
 */
static ssize_t store_scaling_governor(struct cpufreq_policy *policy,
					const char *buf, size_t count)
{
<<<<<<< HEAD
	unsigned int ret = -EINVAL;
	char	str_governor[16];
	struct cpufreq_policy new_policy;
	char *envp[3];
	char buf1[64];
	char buf2[64];
<<<<<<< HEAD
=======
	int ret;
	char	str_governor[16];
	struct cpufreq_policy new_policy;
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

	ret = cpufreq_get_policy(&new_policy, policy->cpu);
	if (ret)
		return ret;

	ret = sscanf(buf, "%15s", str_governor);
	if (ret != 1)
		return -EINVAL;

	if (cpufreq_parse_governor(str_governor, &new_policy.policy,
						&new_policy.governor))
		return -EINVAL;

<<<<<<< HEAD
	/* Do not use cpufreq_set_policy here or the user_policy.max
	   will be wrongly overridden */
	ret = __cpufreq_set_policy(policy, &new_policy);
=======
	ret = cpufreq_set_policy(policy, &new_policy);
>>>>>>> refs/remotes/origin/master

	policy->user_policy.policy = policy->policy;
	policy->user_policy.governor = policy->governor;

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	sysfs_notify(&policy->kobj, NULL, "scaling_governor");

	snprintf(buf1, sizeof(buf1), "GOV=%s", policy->governor->name);
	snprintf(buf2, sizeof(buf2), "CPU=%u", policy->cpu);
	envp[0] = buf1;
	envp[1] = buf2;
	envp[2] = NULL;
	kobject_uevent_env(cpufreq_global_kobject, KOBJ_ADD, envp);

<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (ret)
		return ret;
	else
		return count;
}

/**
 * show_scaling_driver - show the cpufreq driver currently loaded
 */
static ssize_t show_scaling_driver(struct cpufreq_policy *policy, char *buf)
{
<<<<<<< HEAD
	return scnprintf(buf, CPUFREQ_NAME_LEN, "%s\n", cpufreq_driver->name);
=======
	return scnprintf(buf, CPUFREQ_NAME_PLEN, "%s\n", cpufreq_driver->name);
>>>>>>> refs/remotes/origin/master
}

/**
 * show_scaling_available_governors - show the available CPUfreq governors
 */
static ssize_t show_scaling_available_governors(struct cpufreq_policy *policy,
						char *buf)
{
	ssize_t i = 0;
	struct cpufreq_governor *t;

<<<<<<< HEAD
	if (!cpufreq_driver->target) {
=======
	if (!has_target()) {
>>>>>>> refs/remotes/origin/master
		i += sprintf(buf, "performance powersave");
		goto out;
	}

	list_for_each_entry(t, &cpufreq_governor_list, governor_list) {
		if (i >= (ssize_t) ((PAGE_SIZE / sizeof(char))
		    - (CPUFREQ_NAME_LEN + 2)))
			goto out;
<<<<<<< HEAD
		i += scnprintf(&buf[i], CPUFREQ_NAME_LEN, "%s ", t->name);
=======
		i += scnprintf(&buf[i], CPUFREQ_NAME_PLEN, "%s ", t->name);
>>>>>>> refs/remotes/origin/master
	}
out:
	i += sprintf(&buf[i], "\n");
	return i;
}

<<<<<<< HEAD
static ssize_t show_cpus(const struct cpumask *mask, char *buf)
=======
ssize_t cpufreq_show_cpus(const struct cpumask *mask, char *buf)
>>>>>>> refs/remotes/origin/master
{
	ssize_t i = 0;
	unsigned int cpu;

	for_each_cpu(cpu, mask) {
		if (i)
			i += scnprintf(&buf[i], (PAGE_SIZE - i - 2), " ");
		i += scnprintf(&buf[i], (PAGE_SIZE - i - 2), "%u", cpu);
		if (i >= (PAGE_SIZE - 5))
			break;
	}
	i += sprintf(&buf[i], "\n");
	return i;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(cpufreq_show_cpus);
>>>>>>> refs/remotes/origin/master

/**
 * show_related_cpus - show the CPUs affected by each transition even if
 * hw coordination is in use
 */
static ssize_t show_related_cpus(struct cpufreq_policy *policy, char *buf)
{
<<<<<<< HEAD
	if (cpumask_empty(policy->related_cpus))
		return show_cpus(policy->cpus, buf);
	return show_cpus(policy->related_cpus, buf);
=======
	return cpufreq_show_cpus(policy->related_cpus, buf);
>>>>>>> refs/remotes/origin/master
}

/**
 * show_affected_cpus - show the CPUs affected by each transition
 */
static ssize_t show_affected_cpus(struct cpufreq_policy *policy, char *buf)
{
<<<<<<< HEAD
	return show_cpus(policy->cpus, buf);
=======
	return cpufreq_show_cpus(policy->cpus, buf);
>>>>>>> refs/remotes/origin/master
}

static ssize_t store_scaling_setspeed(struct cpufreq_policy *policy,
					const char *buf, size_t count)
{
	unsigned int freq = 0;
	unsigned int ret;

	if (!policy->governor || !policy->governor->store_setspeed)
		return -EINVAL;

	ret = sscanf(buf, "%u", &freq);
	if (ret != 1)
		return -EINVAL;

	policy->governor->store_setspeed(policy, freq);

	return count;
}

static ssize_t show_scaling_setspeed(struct cpufreq_policy *policy, char *buf)
{
	if (!policy->governor || !policy->governor->show_setspeed)
		return sprintf(buf, "<unsupported>\n");

	return policy->governor->show_setspeed(policy, buf);
}

/**
<<<<<<< HEAD
 * show_scaling_driver - show the current cpufreq HW/BIOS limitation
=======
 * show_bios_limit - show the current cpufreq HW/BIOS limitation
>>>>>>> refs/remotes/origin/master
 */
static ssize_t show_bios_limit(struct cpufreq_policy *policy, char *buf)
{
	unsigned int limit;
	int ret;
	if (cpufreq_driver->bios_limit) {
		ret = cpufreq_driver->bios_limit(policy->cpu, &limit);
		if (!ret)
			return sprintf(buf, "%u\n", limit);
	}
	return sprintf(buf, "%u\n", policy->cpuinfo.max_freq);
}

cpufreq_freq_attr_ro_perm(cpuinfo_cur_freq, 0400);
cpufreq_freq_attr_ro(cpuinfo_min_freq);
cpufreq_freq_attr_ro(cpuinfo_max_freq);
cpufreq_freq_attr_ro(cpuinfo_transition_latency);
cpufreq_freq_attr_ro(scaling_available_governors);
cpufreq_freq_attr_ro(scaling_driver);
cpufreq_freq_attr_ro(scaling_cur_freq);
cpufreq_freq_attr_ro(bios_limit);
cpufreq_freq_attr_ro(related_cpus);
cpufreq_freq_attr_ro(affected_cpus);
<<<<<<< HEAD
<<<<<<< HEAD
cpufreq_freq_attr_ro(cpu_utilization);
=======
>>>>>>> refs/remotes/origin/master
=======
cpufreq_freq_attr_ro(cpu_utilization);
>>>>>>> refs/remotes/origin/cm-11.0
cpufreq_freq_attr_rw(scaling_min_freq);
cpufreq_freq_attr_rw(scaling_max_freq);
cpufreq_freq_attr_rw(scaling_governor);
cpufreq_freq_attr_rw(scaling_setspeed);

static struct attribute *default_attrs[] = {
	&cpuinfo_min_freq.attr,
	&cpuinfo_max_freq.attr,
	&cpuinfo_transition_latency.attr,
	&scaling_min_freq.attr,
	&scaling_max_freq.attr,
	&affected_cpus.attr,
<<<<<<< HEAD
<<<<<<< HEAD
	&cpu_utilization.attr,
=======
>>>>>>> refs/remotes/origin/master
=======
	&cpu_utilization.attr,
>>>>>>> refs/remotes/origin/cm-11.0
	&related_cpus.attr,
	&scaling_governor.attr,
	&scaling_driver.attr,
	&scaling_available_governors.attr,
	&scaling_setspeed.attr,
	NULL
};

<<<<<<< HEAD
struct kobject *cpufreq_global_kobject;
EXPORT_SYMBOL(cpufreq_global_kobject);

=======
>>>>>>> refs/remotes/origin/master
#define to_policy(k) container_of(k, struct cpufreq_policy, kobj)
#define to_attr(a) container_of(a, struct freq_attr, attr)

static ssize_t show(struct kobject *kobj, struct attribute *attr, char *buf)
{
	struct cpufreq_policy *policy = to_policy(kobj);
	struct freq_attr *fattr = to_attr(attr);
<<<<<<< HEAD
	ssize_t ret = -EINVAL;
	policy = cpufreq_cpu_get_sysfs(policy->cpu);
	if (!policy)
		goto no_policy;

	if (lock_policy_rwsem_read(policy->cpu) < 0)
		goto fail;
=======
	ssize_t ret;

	if (!down_read_trylock(&cpufreq_rwsem))
		return -EINVAL;

	down_read(&policy->rwsem);
>>>>>>> refs/remotes/origin/master

	if (fattr->show)
		ret = fattr->show(policy, buf);
	else
		ret = -EIO;

<<<<<<< HEAD
	unlock_policy_rwsem_read(policy->cpu);
fail:
	cpufreq_cpu_put_sysfs(policy);
no_policy:
=======
	up_read(&policy->rwsem);
	up_read(&cpufreq_rwsem);

>>>>>>> refs/remotes/origin/master
	return ret;
}

static ssize_t store(struct kobject *kobj, struct attribute *attr,
		     const char *buf, size_t count)
{
	struct cpufreq_policy *policy = to_policy(kobj);
	struct freq_attr *fattr = to_attr(attr);
	ssize_t ret = -EINVAL;
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
	policy = cpufreq_cpu_get_sysfs(policy->cpu);
	if (!policy)
		goto no_policy;

	if (lock_policy_rwsem_write(policy->cpu) < 0)
		goto fail;
=======

	get_online_cpus();

	if (!cpu_online(policy->cpu))
		goto unlock;

	if (!down_read_trylock(&cpufreq_rwsem))
		goto unlock;

	down_write(&policy->rwsem);
>>>>>>> refs/remotes/origin/master

	if (fattr->store)
		ret = fattr->store(policy, buf, count);
	else
		ret = -EIO;

<<<<<<< HEAD
	unlock_policy_rwsem_write(policy->cpu);
fail:
	cpufreq_cpu_put_sysfs(policy);
no_policy:
=======
	up_write(&policy->rwsem);

	up_read(&cpufreq_rwsem);
unlock:
	put_online_cpus();

>>>>>>> refs/remotes/origin/master
	return ret;
}

static void cpufreq_sysfs_release(struct kobject *kobj)
{
	struct cpufreq_policy *policy = to_policy(kobj);
	pr_debug("last reference is dropped\n");
	complete(&policy->kobj_unregister);
}

static const struct sysfs_ops sysfs_ops = {
	.show	= show,
	.store	= store,
};

static struct kobj_type ktype_cpufreq = {
	.sysfs_ops	= &sysfs_ops,
	.default_attrs	= default_attrs,
	.release	= cpufreq_sysfs_release,
};

<<<<<<< HEAD
/*
 * Returns:
 *   Negative: Failure
 *   0:        Success
 *   Positive: When we have a managed CPU and the sysfs got symlinked
 */
static int cpufreq_add_dev_policy(unsigned int cpu,
				  struct cpufreq_policy *policy,
<<<<<<< HEAD
				  struct sys_device *sys_dev)
=======
				  struct device *dev)
>>>>>>> refs/remotes/origin/cm-10.0
{
	int ret = 0;
#ifdef CONFIG_SMP
	unsigned long flags;
	unsigned int j;
#ifdef CONFIG_HOTPLUG_CPU
	struct cpufreq_governor *gov;

	gov = __find_governor(per_cpu(cpufreq_policy_save, cpu).gov);
	if (gov) {
		policy->governor = gov;
		pr_debug("Restoring governor %s for cpu %d\n",
		       policy->governor->name, cpu);
	}
	if (per_cpu(cpufreq_policy_save, cpu).min) {
		policy->min = per_cpu(cpufreq_policy_save, cpu).min;
		policy->user_policy.min = policy->min;
	}
	if (per_cpu(cpufreq_policy_save, cpu).max) {
		policy->max = per_cpu(cpufreq_policy_save, cpu).max;
		policy->user_policy.max = policy->max;
	}
	pr_debug("Restoring CPU%d min %d and max %d\n",
		cpu, policy->min, policy->max);
#endif

	for_each_cpu(j, policy->cpus) {
		struct cpufreq_policy *managed_policy;

		if (cpu == j)
			continue;

		/* Check for existing affected CPUs.
		 * They may not be aware of it due to CPU Hotplug.
		 * cpufreq_cpu_put is called when the device is removed
		 * in __cpufreq_remove_dev()
		 */
		managed_policy = cpufreq_cpu_get(j);
		if (unlikely(managed_policy)) {

			/* Set proper policy_cpu */
			unlock_policy_rwsem_write(cpu);
			per_cpu(cpufreq_policy_cpu, cpu) = managed_policy->cpu;

			if (lock_policy_rwsem_write(cpu) < 0) {
				/* Should not go through policy unlock path */
				if (cpufreq_driver->exit)
					cpufreq_driver->exit(policy);
				cpufreq_cpu_put(managed_policy);
				return -EBUSY;
			}

			spin_lock_irqsave(&cpufreq_driver_lock, flags);
			cpumask_copy(managed_policy->cpus, policy->cpus);
			per_cpu(cpufreq_cpu_data, cpu) = managed_policy;
			spin_unlock_irqrestore(&cpufreq_driver_lock, flags);

			pr_debug("CPU already managed, adding link\n");
<<<<<<< HEAD
			ret = sysfs_create_link(&sys_dev->kobj,
=======
			ret = sysfs_create_link(&dev->kobj,
>>>>>>> refs/remotes/origin/cm-10.0
						&managed_policy->kobj,
						"cpufreq");
			if (ret)
				cpufreq_cpu_put(managed_policy);
			/*
			 * Success. We only needed to be added to the mask.
			 * Call driver->exit() because only the cpu parent of
			 * the kobj needed to call init().
			 */
			if (cpufreq_driver->exit)
				cpufreq_driver->exit(policy);

			if (!ret)
				return 1;
			else
				return ret;
		}
	}
#endif
	return ret;
}


/* symlink affected CPUs */
static int cpufreq_add_dev_symlink(unsigned int cpu,
				   struct cpufreq_policy *policy)
=======
struct kobject *cpufreq_global_kobject;
EXPORT_SYMBOL(cpufreq_global_kobject);

static int cpufreq_global_kobject_usage;

int cpufreq_get_global_kobject(void)
{
	if (!cpufreq_global_kobject_usage++)
		return kobject_add(cpufreq_global_kobject,
				&cpu_subsys.dev_root->kobj, "%s", "cpufreq");

	return 0;
}
EXPORT_SYMBOL(cpufreq_get_global_kobject);

void cpufreq_put_global_kobject(void)
{
	if (!--cpufreq_global_kobject_usage)
		kobject_del(cpufreq_global_kobject);
}
EXPORT_SYMBOL(cpufreq_put_global_kobject);

int cpufreq_sysfs_create_file(const struct attribute *attr)
{
	int ret = cpufreq_get_global_kobject();

	if (!ret) {
		ret = sysfs_create_file(cpufreq_global_kobject, attr);
		if (ret)
			cpufreq_put_global_kobject();
	}

	return ret;
}
EXPORT_SYMBOL(cpufreq_sysfs_create_file);

void cpufreq_sysfs_remove_file(const struct attribute *attr)
{
	sysfs_remove_file(cpufreq_global_kobject, attr);
	cpufreq_put_global_kobject();
}
EXPORT_SYMBOL(cpufreq_sysfs_remove_file);

/* symlink affected CPUs */
static int cpufreq_add_dev_symlink(struct cpufreq_policy *policy)
>>>>>>> refs/remotes/origin/master
{
	unsigned int j;
	int ret = 0;

	for_each_cpu(j, policy->cpus) {
<<<<<<< HEAD
		struct cpufreq_policy *managed_policy;
<<<<<<< HEAD
		struct sys_device *cpu_sys_dev;
=======
		struct device *cpu_dev;
>>>>>>> refs/remotes/origin/cm-10.0

		if (j == cpu)
			continue;
		if (!cpu_online(j))
			continue;

		pr_debug("CPU %u already managed, adding link\n", j);
		managed_policy = cpufreq_cpu_get(cpu);
<<<<<<< HEAD
		cpu_sys_dev = get_cpu_sysdev(j);
		ret = sysfs_create_link(&cpu_sys_dev->kobj, &policy->kobj,
=======
		cpu_dev = get_cpu_device(j);
		ret = sysfs_create_link(&cpu_dev->kobj, &policy->kobj,
>>>>>>> refs/remotes/origin/cm-10.0
					"cpufreq");
		if (ret) {
			cpufreq_cpu_put(managed_policy);
			return ret;
		}
=======
		struct device *cpu_dev;

		if (j == policy->cpu)
			continue;

		pr_debug("Adding link for CPU: %u\n", j);
		cpu_dev = get_cpu_device(j);
		ret = sysfs_create_link(&cpu_dev->kobj, &policy->kobj,
					"cpufreq");
		if (ret)
			break;
>>>>>>> refs/remotes/origin/master
	}
	return ret;
}

<<<<<<< HEAD
static int cpufreq_add_dev_interface(unsigned int cpu,
				     struct cpufreq_policy *policy,
<<<<<<< HEAD
				     struct sys_device *sys_dev)
=======
				     struct device *dev)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct cpufreq_policy new_policy;
	struct freq_attr **drv_attr;
	unsigned long flags;
	int ret = 0;
	unsigned int j;

	/* prepare interface data */
	ret = kobject_init_and_add(&policy->kobj, &ktype_cpufreq,
<<<<<<< HEAD
				   &sys_dev->kobj, "cpufreq");
=======
				   &dev->kobj, "cpufreq");
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int cpufreq_add_dev_interface(struct cpufreq_policy *policy,
				     struct device *dev)
{
	struct freq_attr **drv_attr;
	int ret = 0;

	/* prepare interface data */
	ret = kobject_init_and_add(&policy->kobj, &ktype_cpufreq,
				   &dev->kobj, "cpufreq");
>>>>>>> refs/remotes/origin/master
	if (ret)
		return ret;

	/* set up files for this cpu device */
	drv_attr = cpufreq_driver->attr;
	while ((drv_attr) && (*drv_attr)) {
		ret = sysfs_create_file(&policy->kobj, &((*drv_attr)->attr));
		if (ret)
			goto err_out_kobj_put;
		drv_attr++;
	}
	if (cpufreq_driver->get) {
		ret = sysfs_create_file(&policy->kobj, &cpuinfo_cur_freq.attr);
		if (ret)
			goto err_out_kobj_put;
	}
<<<<<<< HEAD
	if (cpufreq_driver->target) {
=======
	if (has_target()) {
>>>>>>> refs/remotes/origin/master
		ret = sysfs_create_file(&policy->kobj, &scaling_cur_freq.attr);
		if (ret)
			goto err_out_kobj_put;
	}
	if (cpufreq_driver->bios_limit) {
		ret = sysfs_create_file(&policy->kobj, &bios_limit.attr);
		if (ret)
			goto err_out_kobj_put;
	}

<<<<<<< HEAD
	spin_lock_irqsave(&cpufreq_driver_lock, flags);
	for_each_cpu(j, policy->cpus) {
		if (!cpu_online(j))
			continue;
		per_cpu(cpufreq_cpu_data, j) = policy;
		per_cpu(cpufreq_policy_cpu, j) = policy->cpu;
	}
	spin_unlock_irqrestore(&cpufreq_driver_lock, flags);

	ret = cpufreq_add_dev_symlink(cpu, policy);
	if (ret)
		goto err_out_kobj_put;

	memcpy(&new_policy, policy, sizeof(struct cpufreq_policy));
	/* assure that the starting sequence is run in __cpufreq_set_policy */
	policy->governor = NULL;

	/* set default policy */
	ret = __cpufreq_set_policy(policy, &new_policy);
	policy->user_policy.policy = policy->policy;
	policy->user_policy.governor = policy->governor;

=======
	ret = cpufreq_add_dev_symlink(policy);
	if (ret)
		goto err_out_kobj_put;

	return ret;

err_out_kobj_put:
	kobject_put(&policy->kobj);
	wait_for_completion(&policy->kobj_unregister);
	return ret;
}

static void cpufreq_init_policy(struct cpufreq_policy *policy)
{
	struct cpufreq_policy new_policy;
	int ret = 0;

	memcpy(&new_policy, policy, sizeof(*policy));

	/* Use the default policy if its valid. */
	if (cpufreq_driver->setpolicy)
		cpufreq_parse_governor(policy->governor->name,
					&new_policy.policy, NULL);

	/* assure that the starting sequence is run in cpufreq_set_policy */
	policy->governor = NULL;

	/* set default policy */
	ret = cpufreq_set_policy(policy, &new_policy);
>>>>>>> refs/remotes/origin/master
	if (ret) {
		pr_debug("setting policy failed\n");
		if (cpufreq_driver->exit)
			cpufreq_driver->exit(policy);
	}
<<<<<<< HEAD
	return ret;

err_out_kobj_put:
	kobject_put(&policy->kobj);
	wait_for_completion(&policy->kobj_unregister);
	return ret;
}


/**
 * cpufreq_add_dev - add a CPU device
 *
 * Adds the cpufreq interface for a CPU device.
 *
 * The Oracle says: try running cpufreq registration/unregistration concurrently
 * with with cpu hotplugging and all hell will break loose. Tried to clean this
 * mess up, but more thorough testing is needed. - Mathieu
 */
<<<<<<< HEAD
static int cpufreq_add_dev(struct sys_device *sys_dev)
{
	unsigned int cpu = sys_dev->id;
=======
static int cpufreq_add_dev(struct device *dev, struct subsys_interface *sif)
{
	unsigned int cpu = dev->id;
>>>>>>> refs/remotes/origin/cm-10.0
	int ret = 0, found = 0;
	struct cpufreq_policy *policy;
	unsigned long flags;
	unsigned int j;
#ifdef CONFIG_HOTPLUG_CPU
	int sibling;
=======
}

#ifdef CONFIG_HOTPLUG_CPU
static int cpufreq_add_policy_cpu(struct cpufreq_policy *policy,
				  unsigned int cpu, struct device *dev)
{
	int ret = 0;
	unsigned long flags;

	if (has_target()) {
		ret = __cpufreq_governor(policy, CPUFREQ_GOV_STOP);
		if (ret) {
			pr_err("%s: Failed to stop governor\n", __func__);
			return ret;
		}
	}

	down_write(&policy->rwsem);

	write_lock_irqsave(&cpufreq_driver_lock, flags);

	cpumask_set_cpu(cpu, policy->cpus);
	per_cpu(cpufreq_cpu_data, cpu) = policy;
	write_unlock_irqrestore(&cpufreq_driver_lock, flags);

	up_write(&policy->rwsem);

	if (has_target()) {
		if ((ret = __cpufreq_governor(policy, CPUFREQ_GOV_START)) ||
			(ret = __cpufreq_governor(policy, CPUFREQ_GOV_LIMITS))) {
			pr_err("%s: Failed to start governor\n", __func__);
			return ret;
		}
	}

	return sysfs_create_link(&dev->kobj, &policy->kobj, "cpufreq");
}
#endif

static struct cpufreq_policy *cpufreq_policy_restore(unsigned int cpu)
{
	struct cpufreq_policy *policy;
	unsigned long flags;

	read_lock_irqsave(&cpufreq_driver_lock, flags);

	policy = per_cpu(cpufreq_cpu_data_fallback, cpu);

	read_unlock_irqrestore(&cpufreq_driver_lock, flags);

	return policy;
}

static struct cpufreq_policy *cpufreq_policy_alloc(void)
{
	struct cpufreq_policy *policy;

	policy = kzalloc(sizeof(*policy), GFP_KERNEL);
	if (!policy)
		return NULL;

	if (!alloc_cpumask_var(&policy->cpus, GFP_KERNEL))
		goto err_free_policy;

	if (!zalloc_cpumask_var(&policy->related_cpus, GFP_KERNEL))
		goto err_free_cpumask;

	INIT_LIST_HEAD(&policy->policy_list);
	init_rwsem(&policy->rwsem);

	return policy;

err_free_cpumask:
	free_cpumask_var(policy->cpus);
err_free_policy:
	kfree(policy);

	return NULL;
}

static void cpufreq_policy_put_kobj(struct cpufreq_policy *policy)
{
	struct kobject *kobj;
	struct completion *cmp;

	down_read(&policy->rwsem);
	kobj = &policy->kobj;
	cmp = &policy->kobj_unregister;
	up_read(&policy->rwsem);
	kobject_put(kobj);

	/*
	 * We need to make sure that the underlying kobj is
	 * actually not referenced anymore by anybody before we
	 * proceed with unloading.
	 */
	pr_debug("waiting for dropping of refcount\n");
	wait_for_completion(cmp);
	pr_debug("wait complete\n");
}

static void cpufreq_policy_free(struct cpufreq_policy *policy)
{
	free_cpumask_var(policy->related_cpus);
	free_cpumask_var(policy->cpus);
	kfree(policy);
}

static void update_policy_cpu(struct cpufreq_policy *policy, unsigned int cpu)
{
	if (WARN_ON(cpu == policy->cpu))
		return;

	down_write(&policy->rwsem);

	policy->last_cpu = policy->cpu;
	policy->cpu = cpu;

	up_write(&policy->rwsem);

	cpufreq_frequency_table_update_policy_cpu(policy);
	blocking_notifier_call_chain(&cpufreq_policy_notifier_list,
			CPUFREQ_UPDATE_POLICY_CPU, policy);
}

static int __cpufreq_add_dev(struct device *dev, struct subsys_interface *sif,
			     bool frozen)
{
	unsigned int j, cpu = dev->id;
	int ret = -ENOMEM;
	struct cpufreq_policy *policy;
	unsigned long flags;
#ifdef CONFIG_HOTPLUG_CPU
	struct cpufreq_policy *tpolicy;
	struct cpufreq_governor *gov;
>>>>>>> refs/remotes/origin/master
#endif

	if (cpu_is_offline(cpu))
		return 0;

	pr_debug("adding CPU %u\n", cpu);

#ifdef CONFIG_SMP
	/* check whether a different CPU already registered this
	 * CPU because it is in the same boat. */
	policy = cpufreq_cpu_get(cpu);
	if (unlikely(policy)) {
		cpufreq_cpu_put(policy);
		return 0;
	}
#endif

<<<<<<< HEAD
	if (!try_module_get(cpufreq_driver->owner)) {
		ret = -EINVAL;
		goto module_out;
	}

	ret = -ENOMEM;
	policy = kzalloc(sizeof(struct cpufreq_policy), GFP_KERNEL);
	if (!policy)
		goto nomem_out;

	if (!alloc_cpumask_var(&policy->cpus, GFP_KERNEL))
		goto err_free_policy;

	if (!zalloc_cpumask_var(&policy->related_cpus, GFP_KERNEL))
		goto err_free_cpumask;

	policy->cpu = cpu;
	cpumask_copy(policy->cpus, cpumask_of(cpu));

	/* Initially set CPU itself as the policy_cpu */
	per_cpu(cpufreq_policy_cpu, cpu) = cpu;
	ret = (lock_policy_rwsem_write(cpu) < 0);
	WARN_ON(ret);

	init_completion(&policy->kobj_unregister);
	INIT_WORK(&policy->update, handle_update);

	/* Set governor before ->init, so that driver could check it */
#ifdef CONFIG_HOTPLUG_CPU
	for_each_online_cpu(sibling) {
		struct cpufreq_policy *cp = per_cpu(cpufreq_cpu_data, sibling);
		if (cp && cp->governor &&
		    (cpumask_test_cpu(cpu, cp->related_cpus))) {
			policy->governor = cp->governor;
			found = 1;
			break;
		}
	}
#endif
	if (!found)
		policy->governor = CPUFREQ_DEFAULT_GOVERNOR;
=======
	if (!down_read_trylock(&cpufreq_rwsem))
		return 0;

#ifdef CONFIG_HOTPLUG_CPU
	/* Check if this cpu was hot-unplugged earlier and has siblings */
	read_lock_irqsave(&cpufreq_driver_lock, flags);
	list_for_each_entry(tpolicy, &cpufreq_policy_list, policy_list) {
		if (cpumask_test_cpu(cpu, tpolicy->related_cpus)) {
			read_unlock_irqrestore(&cpufreq_driver_lock, flags);
			ret = cpufreq_add_policy_cpu(tpolicy, cpu, dev);
			up_read(&cpufreq_rwsem);
			return ret;
		}
	}
	read_unlock_irqrestore(&cpufreq_driver_lock, flags);
#endif

	/*
	 * Restore the saved policy when doing light-weight init and fall back
	 * to the full init if that fails.
	 */
	policy = frozen ? cpufreq_policy_restore(cpu) : NULL;
	if (!policy) {
		frozen = false;
		policy = cpufreq_policy_alloc();
		if (!policy)
			goto nomem_out;
	}

	/*
	 * In the resume path, since we restore a saved policy, the assignment
	 * to policy->cpu is like an update of the existing policy, rather than
	 * the creation of a brand new one. So we need to perform this update
	 * by invoking update_policy_cpu().
	 */
	if (frozen && cpu != policy->cpu)
		update_policy_cpu(policy, cpu);
	else
		policy->cpu = cpu;

	policy->governor = CPUFREQ_DEFAULT_GOVERNOR;
	cpumask_copy(policy->cpus, cpumask_of(cpu));

	init_completion(&policy->kobj_unregister);
	INIT_WORK(&policy->update, handle_update);

>>>>>>> refs/remotes/origin/master
	/* call driver. From then on the cpufreq must be able
	 * to accept all calls to ->verify and ->setpolicy for this CPU
	 */
	ret = cpufreq_driver->init(policy);
	if (ret) {
		pr_debug("initialization failed\n");
<<<<<<< HEAD
		goto err_unlock_policy;
	}
	policy->user_policy.min = policy->min;
	policy->user_policy.max = policy->max;
=======
		goto err_set_policy_cpu;
	}

	if (cpufreq_driver->get) {
		policy->cur = cpufreq_driver->get(policy->cpu);
		if (!policy->cur) {
			pr_err("%s: ->get() failed\n", __func__);
			goto err_get_freq;
		}
	}

	/* related cpus should atleast have policy->cpus */
	cpumask_or(policy->related_cpus, policy->related_cpus, policy->cpus);

	/*
	 * affected cpus must always be the one, which are online. We aren't
	 * managing offline cpus here.
	 */
	cpumask_and(policy->cpus, policy->cpus, cpu_online_mask);

	if (!frozen) {
		policy->user_policy.min = policy->min;
		policy->user_policy.max = policy->max;
	}
>>>>>>> refs/remotes/origin/master

	blocking_notifier_call_chain(&cpufreq_policy_notifier_list,
				     CPUFREQ_START, policy);

<<<<<<< HEAD
<<<<<<< HEAD
	ret = cpufreq_add_dev_policy(cpu, policy, sys_dev);
=======
	ret = cpufreq_add_dev_policy(cpu, policy, dev);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret) {
		if (ret > 0)
			/* This is a managed cpu, symlink created,
			   exit with 0 */
			ret = 0;
		goto err_unlock_policy;
	}

<<<<<<< HEAD
	ret = cpufreq_add_dev_interface(cpu, policy, sys_dev);
=======
	ret = cpufreq_add_dev_interface(cpu, policy, dev);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret)
		goto err_out_unregister;

	unlock_policy_rwsem_write(cpu);

	kobject_uevent(&policy->kobj, KOBJ_ADD);
	module_put(cpufreq_driver->owner);
=======
#ifdef CONFIG_HOTPLUG_CPU
	gov = __find_governor(per_cpu(cpufreq_cpu_governor, cpu));
	if (gov) {
		policy->governor = gov;
		pr_debug("Restoring governor %s for cpu %d\n",
		       policy->governor->name, cpu);
	}
#endif

	write_lock_irqsave(&cpufreq_driver_lock, flags);
	for_each_cpu(j, policy->cpus)
		per_cpu(cpufreq_cpu_data, j) = policy;
	write_unlock_irqrestore(&cpufreq_driver_lock, flags);

	if (!frozen) {
		ret = cpufreq_add_dev_interface(policy, dev);
		if (ret)
			goto err_out_unregister;
	}

	write_lock_irqsave(&cpufreq_driver_lock, flags);
	list_add(&policy->policy_list, &cpufreq_policy_list);
	write_unlock_irqrestore(&cpufreq_driver_lock, flags);

	cpufreq_init_policy(policy);

	if (!frozen) {
		policy->user_policy.policy = policy->policy;
		policy->user_policy.governor = policy->governor;
	}

	kobject_uevent(&policy->kobj, KOBJ_ADD);
	up_read(&cpufreq_rwsem);

>>>>>>> refs/remotes/origin/master
	pr_debug("initialization complete\n");

	return 0;

<<<<<<< HEAD

err_out_unregister:
	spin_lock_irqsave(&cpufreq_driver_lock, flags);
	for_each_cpu(j, policy->cpus)
		per_cpu(cpufreq_cpu_data, j) = NULL;
	spin_unlock_irqrestore(&cpufreq_driver_lock, flags);

	kobject_put(&policy->kobj);
	wait_for_completion(&policy->kobj_unregister);

err_unlock_policy:
	unlock_policy_rwsem_write(cpu);
	free_cpumask_var(policy->related_cpus);
err_free_cpumask:
	free_cpumask_var(policy->cpus);
err_free_policy:
	kfree(policy);
nomem_out:
	module_put(cpufreq_driver->owner);
module_out:
	return ret;
}


/**
 * __cpufreq_remove_dev - remove a CPU device
 *
 * Removes the cpufreq interface for a CPU device.
 * Caller should already have policy_rwsem in write mode for this CPU.
 * This routine frees the rwsem before returning.
 */
<<<<<<< HEAD
static int __cpufreq_remove_dev(struct sys_device *sys_dev)
{
	unsigned int cpu = sys_dev->id;
=======
static int __cpufreq_remove_dev(struct device *dev, struct subsys_interface *sif)
{
	unsigned int cpu = dev->id;
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned long flags;
	struct cpufreq_policy *data;
	struct kobject *kobj;
	struct completion *cmp;
#ifdef CONFIG_SMP
<<<<<<< HEAD
	struct sys_device *cpu_sys_dev;
=======
	struct device *cpu_dev;
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned int j;
#endif

	pr_debug("unregistering CPU %u\n", cpu);

	spin_lock_irqsave(&cpufreq_driver_lock, flags);
	data = per_cpu(cpufreq_cpu_data, cpu);

	if (!data) {
		spin_unlock_irqrestore(&cpufreq_driver_lock, flags);
		unlock_policy_rwsem_write(cpu);
		return -EINVAL;
	}
	per_cpu(cpufreq_cpu_data, cpu) = NULL;


#ifdef CONFIG_SMP
	/* if this isn't the CPU which is the parent of the kobj, we
	 * only need to unlink, put and exit
	 */
	if (unlikely(cpu != data->cpu)) {
		pr_debug("removing link\n");
		cpumask_clear_cpu(cpu, data->cpus);
		spin_unlock_irqrestore(&cpufreq_driver_lock, flags);
<<<<<<< HEAD
		kobj = &sys_dev->kobj;
=======
		kobj = &dev->kobj;
>>>>>>> refs/remotes/origin/cm-10.0
		cpufreq_cpu_put(data);
		unlock_policy_rwsem_write(cpu);
		sysfs_remove_link(kobj, "cpufreq");
		return 0;
	}
#endif

#ifdef CONFIG_SMP

#ifdef CONFIG_HOTPLUG_CPU
	strncpy(per_cpu(cpufreq_policy_save, cpu).gov, data->governor->name,
			CPUFREQ_NAME_LEN);
	per_cpu(cpufreq_policy_save, cpu).min = data->min;
	per_cpu(cpufreq_policy_save, cpu).max = data->max;
	pr_debug("Saving CPU%d policy min %d and max %d\n",
			cpu, data->min, data->max);
#endif

	/* if we have other CPUs still registered, we need to unlink them,
	 * or else wait_for_completion below will lock up. Clean the
	 * per_cpu(cpufreq_cpu_data) while holding the lock, and remove
	 * the sysfs links afterwards.
	 */
	if (unlikely(cpumask_weight(data->cpus) > 1)) {
		for_each_cpu(j, data->cpus) {
			if (j == cpu)
				continue;
			per_cpu(cpufreq_cpu_data, j) = NULL;
		}
	}

	spin_unlock_irqrestore(&cpufreq_driver_lock, flags);

	if (unlikely(cpumask_weight(data->cpus) > 1)) {
		for_each_cpu(j, data->cpus) {
			if (j == cpu)
				continue;
			pr_debug("removing link for cpu %u\n", j);
#ifdef CONFIG_HOTPLUG_CPU
			strncpy(per_cpu(cpufreq_policy_save, j).gov,
				data->governor->name, CPUFREQ_NAME_LEN);
			per_cpu(cpufreq_policy_save, j).min = data->min;
			per_cpu(cpufreq_policy_save, j).max = data->max;
			pr_debug("Saving CPU%d policy min %d and max %d\n",
					j, data->min, data->max);
#endif
<<<<<<< HEAD
			cpu_sys_dev = get_cpu_sysdev(j);
			kobj = &cpu_sys_dev->kobj;
=======
			cpu_dev = get_cpu_device(j);
			kobj = &cpu_dev->kobj;
>>>>>>> refs/remotes/origin/cm-10.0
			unlock_policy_rwsem_write(cpu);
			sysfs_remove_link(kobj, "cpufreq");
			lock_policy_rwsem_write(cpu);
			cpufreq_cpu_put(data);
		}
	}
#else
	spin_unlock_irqrestore(&cpufreq_driver_lock, flags);
#endif

	if (cpufreq_driver->target)
		__cpufreq_governor(data, CPUFREQ_GOV_STOP);

	kobj = &data->kobj;
	cmp = &data->kobj_unregister;
	unlock_policy_rwsem_write(cpu);
	kobject_put(kobj);

	/* we need to make sure that the underlying kobj is actually
	 * not referenced anymore by anybody before we proceed with
	 * unloading.
	 */
	pr_debug("waiting for dropping of refcount\n");
	wait_for_completion(cmp);
	pr_debug("wait complete\n");

	lock_policy_rwsem_write(cpu);
	if (cpufreq_driver->exit)
		cpufreq_driver->exit(data);
	unlock_policy_rwsem_write(cpu);

#ifdef CONFIG_HOTPLUG_CPU
	/* when the CPU which is the parent of the kobj is hotplugged
	 * offline, check for siblings, and create cpufreq sysfs interface
	 * and symlinks
	 */
	if (unlikely(cpumask_weight(data->cpus) > 1)) {
		/* first sibling now owns the new sysfs dir */
		cpumask_clear_cpu(cpu, data->cpus);
<<<<<<< HEAD
		cpufreq_add_dev(get_cpu_sysdev(cpumask_first(data->cpus)));

		/* finally remove our own symlink */
		lock_policy_rwsem_write(cpu);
		__cpufreq_remove_dev(sys_dev);
=======
		cpufreq_add_dev(get_cpu_device(cpumask_first(data->cpus)), NULL);

		/* finally remove our own symlink */
		lock_policy_rwsem_write(cpu);
		__cpufreq_remove_dev(dev, sif);
>>>>>>> refs/remotes/origin/cm-10.0
	}
#endif

	free_cpumask_var(data->related_cpus);
	free_cpumask_var(data->cpus);
	kfree(data);

	return 0;
}


<<<<<<< HEAD
static int cpufreq_remove_dev(struct sys_device *sys_dev)
{
	unsigned int cpu = sys_dev->id;
=======
static int cpufreq_remove_dev(struct device *dev, struct subsys_interface *sif)
{
	unsigned int cpu = dev->id;
>>>>>>> refs/remotes/origin/cm-10.0
	int retval;
=======
err_out_unregister:
	write_lock_irqsave(&cpufreq_driver_lock, flags);
	for_each_cpu(j, policy->cpus)
		per_cpu(cpufreq_cpu_data, j) = NULL;
	write_unlock_irqrestore(&cpufreq_driver_lock, flags);

err_get_freq:
	if (cpufreq_driver->exit)
		cpufreq_driver->exit(policy);
err_set_policy_cpu:
	if (frozen) {
		/* Do not leave stale fallback data behind. */
		per_cpu(cpufreq_cpu_data_fallback, cpu) = NULL;
		cpufreq_policy_put_kobj(policy);
	}
	cpufreq_policy_free(policy);

nomem_out:
	up_read(&cpufreq_rwsem);

	return ret;
}

/**
 * cpufreq_add_dev - add a CPU device
 *
 * Adds the cpufreq interface for a CPU device.
 *
 * The Oracle says: try running cpufreq registration/unregistration concurrently
 * with with cpu hotplugging and all hell will break loose. Tried to clean this
 * mess up, but more thorough testing is needed. - Mathieu
 */
static int cpufreq_add_dev(struct device *dev, struct subsys_interface *sif)
{
	return __cpufreq_add_dev(dev, sif, false);
}

static int cpufreq_nominate_new_policy_cpu(struct cpufreq_policy *policy,
					   unsigned int old_cpu)
{
	struct device *cpu_dev;
	int ret;

	/* first sibling now owns the new sysfs dir */
	cpu_dev = get_cpu_device(cpumask_any_but(policy->cpus, old_cpu));

	sysfs_remove_link(&cpu_dev->kobj, "cpufreq");
	ret = kobject_move(&policy->kobj, &cpu_dev->kobj);
	if (ret) {
		pr_err("%s: Failed to move kobj: %d", __func__, ret);

		down_write(&policy->rwsem);
		cpumask_set_cpu(old_cpu, policy->cpus);
		up_write(&policy->rwsem);

		ret = sysfs_create_link(&cpu_dev->kobj, &policy->kobj,
					"cpufreq");

		return -EINVAL;
	}

	return cpu_dev->id;
}

static int __cpufreq_remove_dev_prepare(struct device *dev,
					struct subsys_interface *sif,
					bool frozen)
{
	unsigned int cpu = dev->id, cpus;
	int new_cpu, ret;
	unsigned long flags;
	struct cpufreq_policy *policy;

	pr_debug("%s: unregistering CPU %u\n", __func__, cpu);

	write_lock_irqsave(&cpufreq_driver_lock, flags);

	policy = per_cpu(cpufreq_cpu_data, cpu);

	/* Save the policy somewhere when doing a light-weight tear-down */
	if (frozen)
		per_cpu(cpufreq_cpu_data_fallback, cpu) = policy;

	write_unlock_irqrestore(&cpufreq_driver_lock, flags);

	if (!policy) {
		pr_debug("%s: No cpu_data found\n", __func__);
		return -EINVAL;
	}

	if (has_target()) {
		ret = __cpufreq_governor(policy, CPUFREQ_GOV_STOP);
		if (ret) {
			pr_err("%s: Failed to stop governor\n", __func__);
			return ret;
		}
	}

#ifdef CONFIG_HOTPLUG_CPU
	if (!cpufreq_driver->setpolicy)
		strncpy(per_cpu(cpufreq_cpu_governor, cpu),
			policy->governor->name, CPUFREQ_NAME_LEN);
#endif

	down_read(&policy->rwsem);
	cpus = cpumask_weight(policy->cpus);
	up_read(&policy->rwsem);

	if (cpu != policy->cpu) {
		if (!frozen)
			sysfs_remove_link(&dev->kobj, "cpufreq");
	} else if (cpus > 1) {
		new_cpu = cpufreq_nominate_new_policy_cpu(policy, cpu);
		if (new_cpu >= 0) {
			update_policy_cpu(policy, new_cpu);

			if (!frozen) {
				pr_debug("%s: policy Kobject moved to cpu: %d from: %d\n",
						__func__, new_cpu, cpu);
			}
		}
	}

	return 0;
}

static int __cpufreq_remove_dev_finish(struct device *dev,
				       struct subsys_interface *sif,
				       bool frozen)
{
	unsigned int cpu = dev->id, cpus;
	int ret;
	unsigned long flags;
	struct cpufreq_policy *policy;

	read_lock_irqsave(&cpufreq_driver_lock, flags);
	policy = per_cpu(cpufreq_cpu_data, cpu);
	read_unlock_irqrestore(&cpufreq_driver_lock, flags);

	if (!policy) {
		pr_debug("%s: No cpu_data found\n", __func__);
		return -EINVAL;
	}

	down_write(&policy->rwsem);
	cpus = cpumask_weight(policy->cpus);

	if (cpus > 1)
		cpumask_clear_cpu(cpu, policy->cpus);
	up_write(&policy->rwsem);

	/* If cpu is last user of policy, free policy */
	if (cpus == 1) {
		if (has_target()) {
			ret = __cpufreq_governor(policy,
					CPUFREQ_GOV_POLICY_EXIT);
			if (ret) {
				pr_err("%s: Failed to exit governor\n",
						__func__);
				return ret;
			}
		}

		if (!frozen)
			cpufreq_policy_put_kobj(policy);

		/*
		 * Perform the ->exit() even during light-weight tear-down,
		 * since this is a core component, and is essential for the
		 * subsequent light-weight ->init() to succeed.
		 */
		if (cpufreq_driver->exit)
			cpufreq_driver->exit(policy);

		/* Remove policy from list of active policies */
		write_lock_irqsave(&cpufreq_driver_lock, flags);
		list_del(&policy->policy_list);
		write_unlock_irqrestore(&cpufreq_driver_lock, flags);

		if (!frozen)
			cpufreq_policy_free(policy);
	} else {
		if (has_target()) {
			if ((ret = __cpufreq_governor(policy, CPUFREQ_GOV_START)) ||
					(ret = __cpufreq_governor(policy, CPUFREQ_GOV_LIMITS))) {
				pr_err("%s: Failed to start governor\n",
						__func__);
				return ret;
			}
		}
	}

	per_cpu(cpufreq_cpu_data, cpu) = NULL;
	return 0;
}

/**
 * cpufreq_remove_dev - remove a CPU device
 *
 * Removes the cpufreq interface for a CPU device.
 */
static int cpufreq_remove_dev(struct device *dev, struct subsys_interface *sif)
{
	unsigned int cpu = dev->id;
	int ret;
>>>>>>> refs/remotes/origin/master

	if (cpu_is_offline(cpu))
		return 0;

<<<<<<< HEAD
	if (unlikely(lock_policy_rwsem_write(cpu)))
		BUG();

<<<<<<< HEAD
	retval = __cpufreq_remove_dev(sys_dev);
=======
	retval = __cpufreq_remove_dev(dev, sif);
>>>>>>> refs/remotes/origin/cm-10.0
	return retval;
}

=======
	ret = __cpufreq_remove_dev_prepare(dev, sif, false);

	if (!ret)
		ret = __cpufreq_remove_dev_finish(dev, sif, false);

	return ret;
}
>>>>>>> refs/remotes/origin/master

static void handle_update(struct work_struct *work)
{
	struct cpufreq_policy *policy =
		container_of(work, struct cpufreq_policy, update);
	unsigned int cpu = policy->cpu;
	pr_debug("handle_update for cpu %u called\n", cpu);
	cpufreq_update_policy(cpu);
}

/**
<<<<<<< HEAD
 *	cpufreq_out_of_sync - If actual and saved CPU frequency differs, we're in deep trouble.
=======
 *	cpufreq_out_of_sync - If actual and saved CPU frequency differs, we're
 *	in deep trouble.
>>>>>>> refs/remotes/origin/master
 *	@cpu: cpu number
 *	@old_freq: CPU frequency the kernel thinks the CPU runs at
 *	@new_freq: CPU frequency the CPU actually runs at
 *
 *	We adjust to current frequency first, and need to clean up later.
 *	So either call to cpufreq_update_policy() or schedule handle_update()).
 */
static void cpufreq_out_of_sync(unsigned int cpu, unsigned int old_freq,
				unsigned int new_freq)
{
<<<<<<< HEAD
	struct cpufreq_freqs freqs;
=======
	struct cpufreq_policy *policy;
	struct cpufreq_freqs freqs;
	unsigned long flags;
>>>>>>> refs/remotes/origin/master

	pr_debug("Warning: CPU frequency out of sync: cpufreq and timing "
	       "core thinks of %u, is %u kHz.\n", old_freq, new_freq);

<<<<<<< HEAD
	freqs.cpu = cpu;
	freqs.old = old_freq;
	freqs.new = new_freq;
	cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);
	cpufreq_notify_transition(&freqs, CPUFREQ_POSTCHANGE);
}

=======
	freqs.old = old_freq;
	freqs.new = new_freq;

	read_lock_irqsave(&cpufreq_driver_lock, flags);
	policy = per_cpu(cpufreq_cpu_data, cpu);
	read_unlock_irqrestore(&cpufreq_driver_lock, flags);

	cpufreq_notify_transition(policy, &freqs, CPUFREQ_PRECHANGE);
	cpufreq_notify_transition(policy, &freqs, CPUFREQ_POSTCHANGE);
}
>>>>>>> refs/remotes/origin/master

/**
 * cpufreq_quick_get - get the CPU frequency (in kHz) from policy->cur
 * @cpu: CPU number
 *
 * This is the last known freq, without actually getting it from the driver.
 * Return value will be same as what is shown in scaling_cur_freq in sysfs.
 */
unsigned int cpufreq_quick_get(unsigned int cpu)
{
<<<<<<< HEAD
	struct cpufreq_policy *policy = cpufreq_cpu_get(cpu);
	unsigned int ret_freq = 0;

=======
	struct cpufreq_policy *policy;
	unsigned int ret_freq = 0;

	if (cpufreq_driver && cpufreq_driver->setpolicy && cpufreq_driver->get)
		return cpufreq_driver->get(cpu);

	policy = cpufreq_cpu_get(cpu);
>>>>>>> refs/remotes/origin/master
	if (policy) {
		ret_freq = policy->cur;
		cpufreq_cpu_put(policy);
	}

	return ret_freq;
}
EXPORT_SYMBOL(cpufreq_quick_get);

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
/**
 * cpufreq_quick_get_max - get the max reported CPU frequency for this CPU
 * @cpu: CPU number
 *
 * Just return the max possible frequency for a given CPU.
 */
unsigned int cpufreq_quick_get_max(unsigned int cpu)
{
	struct cpufreq_policy *policy = cpufreq_cpu_get(cpu);
	unsigned int ret_freq = 0;

	if (policy) {
		ret_freq = policy->max;
		cpufreq_cpu_put(policy);
	}

	return ret_freq;
}
EXPORT_SYMBOL(cpufreq_quick_get_max);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

=======
>>>>>>> refs/remotes/origin/master
static unsigned int __cpufreq_get(unsigned int cpu)
{
	struct cpufreq_policy *policy = per_cpu(cpufreq_cpu_data, cpu);
	unsigned int ret_freq = 0;

	if (!cpufreq_driver->get)
		return ret_freq;

	ret_freq = cpufreq_driver->get(cpu);

	if (ret_freq && policy->cur &&
		!(cpufreq_driver->flags & CPUFREQ_CONST_LOOPS)) {
		/* verify no discrepancy between actual and
					saved value exists */
		if (unlikely(ret_freq != policy->cur)) {
			cpufreq_out_of_sync(cpu, policy->cur, ret_freq);
			schedule_work(&policy->update);
		}
	}

	return ret_freq;
}

/**
 * cpufreq_get - get the current CPU frequency (in kHz)
 * @cpu: CPU number
 *
 * Get the CPU current (static) CPU frequency
 */
unsigned int cpufreq_get(unsigned int cpu)
{
<<<<<<< HEAD
	unsigned int ret_freq = 0;
	struct cpufreq_policy *policy = cpufreq_cpu_get(cpu);

	if (!policy)
		goto out;

	if (unlikely(lock_policy_rwsem_read(cpu)))
		goto out_policy;

	ret_freq = __cpufreq_get(cpu);

	unlock_policy_rwsem_read(cpu);

out_policy:
	cpufreq_cpu_put(policy);
out:
=======
	struct cpufreq_policy *policy = per_cpu(cpufreq_cpu_data, cpu);
	unsigned int ret_freq = 0;

	if (cpufreq_disabled() || !cpufreq_driver)
		return -ENOENT;

	BUG_ON(!policy);

	if (!down_read_trylock(&cpufreq_rwsem))
		return 0;

	down_read(&policy->rwsem);

	ret_freq = __cpufreq_get(cpu);

	up_read(&policy->rwsem);
	up_read(&cpufreq_rwsem);

>>>>>>> refs/remotes/origin/master
	return ret_freq;
}
EXPORT_SYMBOL(cpufreq_get);

<<<<<<< HEAD
<<<<<<< HEAD
static struct sysdev_driver cpufreq_sysdev_driver = {
	.add		= cpufreq_add_dev,
	.remove		= cpufreq_remove_dev,
=======
=======
>>>>>>> refs/remotes/origin/master
static struct subsys_interface cpufreq_interface = {
	.name		= "cpufreq",
	.subsys		= &cpu_subsys,
	.add_dev	= cpufreq_add_dev,
	.remove_dev	= cpufreq_remove_dev,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
};


=======
};

>>>>>>> refs/remotes/origin/master
/**
 * cpufreq_bp_suspend - Prepare the boot CPU for system suspend.
 *
 * This function is only executed for the boot processor.  The other CPUs
 * have been put offline by means of CPU hotplug.
 */
static int cpufreq_bp_suspend(void)
{
	int ret = 0;

	int cpu = smp_processor_id();
<<<<<<< HEAD
	struct cpufreq_policy *cpu_policy;
=======
	struct cpufreq_policy *policy;
>>>>>>> refs/remotes/origin/master

	pr_debug("suspending cpu %u\n", cpu);

	/* If there's no policy for the boot CPU, we have nothing to do. */
<<<<<<< HEAD
	cpu_policy = cpufreq_cpu_get(cpu);
	if (!cpu_policy)
		return 0;

	if (cpufreq_driver->suspend) {
		ret = cpufreq_driver->suspend(cpu_policy);
		if (ret)
			printk(KERN_ERR "cpufreq: suspend failed in ->suspend "
					"step on CPU %u\n", cpu_policy->cpu);
	}

	cpufreq_cpu_put(cpu_policy);
=======
	policy = cpufreq_cpu_get(cpu);
	if (!policy)
		return 0;

	if (cpufreq_driver->suspend) {
		ret = cpufreq_driver->suspend(policy);
		if (ret)
			printk(KERN_ERR "cpufreq: suspend failed in ->suspend "
					"step on CPU %u\n", policy->cpu);
	}

	cpufreq_cpu_put(policy);
>>>>>>> refs/remotes/origin/master
	return ret;
}

/**
 * cpufreq_bp_resume - Restore proper frequency handling of the boot CPU.
 *
 *	1.) resume CPUfreq hardware support (cpufreq_driver->resume())
 *	2.) schedule call cpufreq_update_policy() ASAP as interrupts are
 *	    restored. It will verify that the current freq is in sync with
 *	    what we believe it to be. This is a bit later than when it
 *	    should be, but nonethteless it's better than calling
 *	    cpufreq_driver->get() here which might re-enable interrupts...
 *
 * This function is only executed for the boot CPU.  The other CPUs have not
 * been turned on yet.
 */
static void cpufreq_bp_resume(void)
{
	int ret = 0;

	int cpu = smp_processor_id();
<<<<<<< HEAD
	struct cpufreq_policy *cpu_policy;
=======
	struct cpufreq_policy *policy;
>>>>>>> refs/remotes/origin/master

	pr_debug("resuming cpu %u\n", cpu);

	/* If there's no policy for the boot CPU, we have nothing to do. */
<<<<<<< HEAD
	cpu_policy = cpufreq_cpu_get(cpu);
	if (!cpu_policy)
		return;

	if (cpufreq_driver->resume) {
		ret = cpufreq_driver->resume(cpu_policy);
		if (ret) {
			printk(KERN_ERR "cpufreq: resume failed in ->resume "
					"step on CPU %u\n", cpu_policy->cpu);
=======
	policy = cpufreq_cpu_get(cpu);
	if (!policy)
		return;

	if (cpufreq_driver->resume) {
		ret = cpufreq_driver->resume(policy);
		if (ret) {
			printk(KERN_ERR "cpufreq: resume failed in ->resume "
					"step on CPU %u\n", policy->cpu);
>>>>>>> refs/remotes/origin/master
			goto fail;
		}
	}

<<<<<<< HEAD
	schedule_work(&cpu_policy->update);

fail:
	cpufreq_cpu_put(cpu_policy);
=======
	schedule_work(&policy->update);

fail:
	cpufreq_cpu_put(policy);
>>>>>>> refs/remotes/origin/master
}

static struct syscore_ops cpufreq_syscore_ops = {
	.suspend	= cpufreq_bp_suspend,
	.resume		= cpufreq_bp_resume,
};

<<<<<<< HEAD
=======
/**
 *	cpufreq_get_current_driver - return current driver's name
 *
 *	Return the name string of the currently loaded cpufreq driver
 *	or NULL, if none.
 */
const char *cpufreq_get_current_driver(void)
{
	if (cpufreq_driver)
		return cpufreq_driver->name;

	return NULL;
}
EXPORT_SYMBOL_GPL(cpufreq_get_current_driver);
>>>>>>> refs/remotes/origin/master

/*********************************************************************
 *                     NOTIFIER LISTS INTERFACE                      *
 *********************************************************************/

/**
 *	cpufreq_register_notifier - register a driver with cpufreq
 *	@nb: notifier function to register
 *      @list: CPUFREQ_TRANSITION_NOTIFIER or CPUFREQ_POLICY_NOTIFIER
 *
 *	Add a driver to one of two lists: either a list of drivers that
 *      are notified about clock rate changes (once before and once after
 *      the transition), or a list of drivers that are notified about
 *      changes in cpufreq policy.
 *
 *	This function may sleep, and has the same return conditions as
 *	blocking_notifier_chain_register.
 */
int cpufreq_register_notifier(struct notifier_block *nb, unsigned int list)
{
	int ret;

<<<<<<< HEAD
=======
	if (cpufreq_disabled())
		return -EINVAL;

>>>>>>> refs/remotes/origin/master
	WARN_ON(!init_cpufreq_transition_notifier_list_called);

	switch (list) {
	case CPUFREQ_TRANSITION_NOTIFIER:
		ret = srcu_notifier_chain_register(
				&cpufreq_transition_notifier_list, nb);
		break;
	case CPUFREQ_POLICY_NOTIFIER:
		ret = blocking_notifier_chain_register(
				&cpufreq_policy_notifier_list, nb);
		break;
	default:
		ret = -EINVAL;
	}

	return ret;
}
EXPORT_SYMBOL(cpufreq_register_notifier);

<<<<<<< HEAD

/**
 *	cpufreq_unregister_notifier - unregister a driver with cpufreq
 *	@nb: notifier block to be unregistered
 *      @list: CPUFREQ_TRANSITION_NOTIFIER or CPUFREQ_POLICY_NOTIFIER
=======
/**
 *	cpufreq_unregister_notifier - unregister a driver with cpufreq
 *	@nb: notifier block to be unregistered
 *	@list: CPUFREQ_TRANSITION_NOTIFIER or CPUFREQ_POLICY_NOTIFIER
>>>>>>> refs/remotes/origin/master
 *
 *	Remove a driver from the CPU frequency notifier list.
 *
 *	This function may sleep, and has the same return conditions as
 *	blocking_notifier_chain_unregister.
 */
int cpufreq_unregister_notifier(struct notifier_block *nb, unsigned int list)
{
	int ret;

<<<<<<< HEAD
=======
	if (cpufreq_disabled())
		return -EINVAL;

>>>>>>> refs/remotes/origin/master
	switch (list) {
	case CPUFREQ_TRANSITION_NOTIFIER:
		ret = srcu_notifier_chain_unregister(
				&cpufreq_transition_notifier_list, nb);
		break;
	case CPUFREQ_POLICY_NOTIFIER:
		ret = blocking_notifier_chain_unregister(
				&cpufreq_policy_notifier_list, nb);
		break;
	default:
		ret = -EINVAL;
	}

	return ret;
}
EXPORT_SYMBOL(cpufreq_unregister_notifier);


/*********************************************************************
 *                              GOVERNORS                            *
 *********************************************************************/

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
int __cpufreq_driver_target(struct cpufreq_policy *policy,
			    unsigned int target_freq,
			    unsigned int relation)
{
	int retval = -EINVAL;
<<<<<<< HEAD

<<<<<<< HEAD
=======
	if (cpufreq_disabled())
		return -ENODEV;

>>>>>>> refs/remotes/origin/cm-10.0
	pr_debug("target for CPU %u: %u kHz, relation %u\n", policy->cpu,
		target_freq, relation);
	if (cpu_online(policy->cpu) && cpufreq_driver->target)
		retval = cpufreq_driver->target(policy, target_freq, relation);

=======
	unsigned int old_target_freq = target_freq;

	if (cpufreq_disabled())
		return -ENODEV;

	/* Make sure that target_freq is within supported range */
	if (target_freq > policy->max)
		target_freq = policy->max;
	if (target_freq < policy->min)
		target_freq = policy->min;

	pr_debug("target for CPU %u: %u kHz, relation %u, requested %u kHz\n",
			policy->cpu, target_freq, relation, old_target_freq);

	/*
	 * This might look like a redundant call as we are checking it again
	 * after finding index. But it is left intentionally for cases where
	 * exactly same freq is called again and so we can save on few function
	 * calls.
	 */
	if (target_freq == policy->cur)
		return 0;

	if (cpufreq_driver->target)
		retval = cpufreq_driver->target(policy, target_freq, relation);
	else if (cpufreq_driver->target_index) {
		struct cpufreq_frequency_table *freq_table;
		struct cpufreq_freqs freqs;
		bool notify;
		int index;

		freq_table = cpufreq_frequency_get_table(policy->cpu);
		if (unlikely(!freq_table)) {
			pr_err("%s: Unable to find freq_table\n", __func__);
			goto out;
		}

		retval = cpufreq_frequency_table_target(policy, freq_table,
				target_freq, relation, &index);
		if (unlikely(retval)) {
			pr_err("%s: Unable to find matching freq\n", __func__);
			goto out;
		}

		if (freq_table[index].frequency == policy->cur) {
			retval = 0;
			goto out;
		}

		notify = !(cpufreq_driver->flags & CPUFREQ_ASYNC_NOTIFICATION);

		if (notify) {
			freqs.old = policy->cur;
			freqs.new = freq_table[index].frequency;
			freqs.flags = 0;

			pr_debug("%s: cpu: %d, oldfreq: %u, new freq: %u\n",
					__func__, policy->cpu, freqs.old,
					freqs.new);

			cpufreq_notify_transition(policy, &freqs,
					CPUFREQ_PRECHANGE);
		}

		retval = cpufreq_driver->target_index(policy, index);
		if (retval)
			pr_err("%s: Failed to change cpu frequency: %d\n",
					__func__, retval);

		if (notify) {
			/*
			 * Notify with old freq in case we failed to change
			 * frequency
			 */
			if (retval)
				freqs.new = freqs.old;

			cpufreq_notify_transition(policy, &freqs,
					CPUFREQ_POSTCHANGE);
		}
	}

out:
>>>>>>> refs/remotes/origin/master
	return retval;
}
EXPORT_SYMBOL_GPL(__cpufreq_driver_target);

int cpufreq_driver_target(struct cpufreq_policy *policy,
			  unsigned int target_freq,
			  unsigned int relation)
{
	int ret = -EINVAL;

<<<<<<< HEAD
	policy = cpufreq_cpu_get(policy->cpu);
	if (!policy)
		goto no_policy;

	if (unlikely(lock_policy_rwsem_write(policy->cpu)))
		goto fail;

	ret = __cpufreq_driver_target(policy, target_freq, relation);

	unlock_policy_rwsem_write(policy->cpu);

fail:
	cpufreq_cpu_put(policy);
no_policy:
=======
	down_write(&policy->rwsem);

	ret = __cpufreq_driver_target(policy, target_freq, relation);

	up_write(&policy->rwsem);

>>>>>>> refs/remotes/origin/master
	return ret;
}
EXPORT_SYMBOL_GPL(cpufreq_driver_target);

<<<<<<< HEAD
int __cpufreq_driver_getavg(struct cpufreq_policy *policy, unsigned int cpu)
{
	int ret = 0;

	policy = cpufreq_cpu_get(policy->cpu);
	if (!policy)
		return -EINVAL;

	if (cpu_online(cpu) && cpufreq_driver->getavg)
		ret = cpufreq_driver->getavg(policy, cpu);

	cpufreq_cpu_put(policy);
	return ret;
}
EXPORT_SYMBOL_GPL(__cpufreq_driver_getavg);

=======
>>>>>>> refs/remotes/origin/master
/*
 * when "event" is CPUFREQ_GOV_LIMITS
 */

static int __cpufreq_governor(struct cpufreq_policy *policy,
					unsigned int event)
{
	int ret;

	/* Only must be defined when default governor is known to have latency
	   restrictions, like e.g. conservative or ondemand.
	   That this is the case is already ensured in Kconfig
	*/
#ifdef CONFIG_CPU_FREQ_GOV_PERFORMANCE
	struct cpufreq_governor *gov = &cpufreq_gov_performance;
#else
	struct cpufreq_governor *gov = NULL;
#endif

	if (policy->governor->max_transition_latency &&
	    policy->cpuinfo.transition_latency >
	    policy->governor->max_transition_latency) {
		if (!gov)
			return -EINVAL;
		else {
			printk(KERN_WARNING "%s governor failed, too long"
			       " transition latency of HW, fallback"
			       " to %s governor\n",
			       policy->governor->name,
			       gov->name);
			policy->governor = gov;
		}
	}

<<<<<<< HEAD
	if (!try_module_get(policy->governor->owner))
		return -EINVAL;

	pr_debug("__cpufreq_governor for CPU %u, event %u\n",
						policy->cpu, event);
	ret = policy->governor->governor(policy, event);

	/* we keep one module reference alive for
			each CPU governed by this CPU */
	if ((event != CPUFREQ_GOV_START) || ret)
		module_put(policy->governor->owner);
	if ((event == CPUFREQ_GOV_STOP) && !ret)
=======
	if (event == CPUFREQ_GOV_POLICY_INIT)
		if (!try_module_get(policy->governor->owner))
			return -EINVAL;

	pr_debug("__cpufreq_governor for CPU %u, event %u\n",
						policy->cpu, event);

	mutex_lock(&cpufreq_governor_lock);
	if ((policy->governor_enabled && event == CPUFREQ_GOV_START)
	    || (!policy->governor_enabled
	    && (event == CPUFREQ_GOV_LIMITS || event == CPUFREQ_GOV_STOP))) {
		mutex_unlock(&cpufreq_governor_lock);
		return -EBUSY;
	}

	if (event == CPUFREQ_GOV_STOP)
		policy->governor_enabled = false;
	else if (event == CPUFREQ_GOV_START)
		policy->governor_enabled = true;

	mutex_unlock(&cpufreq_governor_lock);

	ret = policy->governor->governor(policy, event);

	if (!ret) {
		if (event == CPUFREQ_GOV_POLICY_INIT)
			policy->governor->initialized++;
		else if (event == CPUFREQ_GOV_POLICY_EXIT)
			policy->governor->initialized--;
	} else {
		/* Restore original values */
		mutex_lock(&cpufreq_governor_lock);
		if (event == CPUFREQ_GOV_STOP)
			policy->governor_enabled = true;
		else if (event == CPUFREQ_GOV_START)
			policy->governor_enabled = false;
		mutex_unlock(&cpufreq_governor_lock);
	}

	if (((event == CPUFREQ_GOV_POLICY_INIT) && ret) ||
			((event == CPUFREQ_GOV_POLICY_EXIT) && !ret))
>>>>>>> refs/remotes/origin/master
		module_put(policy->governor->owner);

	return ret;
}

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
int cpufreq_register_governor(struct cpufreq_governor *governor)
{
	int err;

	if (!governor)
		return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (cpufreq_disabled())
		return -ENODEV;

>>>>>>> refs/remotes/origin/cm-10.0
	mutex_lock(&cpufreq_governor_mutex);

=======
	if (cpufreq_disabled())
		return -ENODEV;

	mutex_lock(&cpufreq_governor_mutex);

	governor->initialized = 0;
>>>>>>> refs/remotes/origin/master
	err = -EBUSY;
	if (__find_governor(governor->name) == NULL) {
		err = 0;
		list_add(&governor->governor_list, &cpufreq_governor_list);
	}

	mutex_unlock(&cpufreq_governor_mutex);
	return err;
}
EXPORT_SYMBOL_GPL(cpufreq_register_governor);

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
void cpufreq_unregister_governor(struct cpufreq_governor *governor)
{
#ifdef CONFIG_HOTPLUG_CPU
	int cpu;
#endif

	if (!governor)
		return;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (cpufreq_disabled())
		return;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (cpufreq_disabled())
		return;

>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_HOTPLUG_CPU
	for_each_present_cpu(cpu) {
		if (cpu_online(cpu))
			continue;
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
		if (!strcmp(per_cpu(cpufreq_policy_save, cpu).gov,
					governor->name))
			strcpy(per_cpu(cpufreq_policy_save, cpu).gov, "\0");
		per_cpu(cpufreq_policy_save, cpu).min = 0;
		per_cpu(cpufreq_policy_save, cpu).max = 0;
<<<<<<< HEAD
=======
		if (!strcmp(per_cpu(cpufreq_cpu_governor, cpu), governor->name))
			strcpy(per_cpu(cpufreq_cpu_governor, cpu), "\0");
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
	}
#endif

	mutex_lock(&cpufreq_governor_mutex);
	list_del(&governor->governor_list);
	mutex_unlock(&cpufreq_governor_mutex);
	return;
}
EXPORT_SYMBOL_GPL(cpufreq_unregister_governor);


<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
/*********************************************************************
 *                          POLICY INTERFACE                         *
 *********************************************************************/

/**
 * cpufreq_get_policy - get the current cpufreq_policy
 * @policy: struct cpufreq_policy into which the current cpufreq_policy
 *	is written
 *
 * Reads the current cpufreq policy.
 */
int cpufreq_get_policy(struct cpufreq_policy *policy, unsigned int cpu)
{
	struct cpufreq_policy *cpu_policy;
	if (!policy)
		return -EINVAL;

	cpu_policy = cpufreq_cpu_get(cpu);
	if (!cpu_policy)
		return -EINVAL;

<<<<<<< HEAD
	memcpy(policy, cpu_policy, sizeof(struct cpufreq_policy));
=======
	memcpy(policy, cpu_policy, sizeof(*policy));
>>>>>>> refs/remotes/origin/master

	cpufreq_cpu_put(cpu_policy);
	return 0;
}
EXPORT_SYMBOL(cpufreq_get_policy);

<<<<<<< HEAD

/*
 * data   : current policy.
 * policy : policy to be set.
 */
static int __cpufreq_set_policy(struct cpufreq_policy *data,
				struct cpufreq_policy *policy)
{
	int ret = 0;

	pr_debug("setting new policy for CPU %u: %u - %u kHz\n", policy->cpu,
		policy->min, policy->max);

	memcpy(&policy->cpuinfo, &data->cpuinfo,
				sizeof(struct cpufreq_cpuinfo));

	if (policy->min > data->max || policy->max < data->min) {
=======
/*
 * policy : current policy.
 * new_policy: policy to be set.
 */
static int cpufreq_set_policy(struct cpufreq_policy *policy,
				struct cpufreq_policy *new_policy)
{
	int ret = 0, failed = 1;

	pr_debug("setting new policy for CPU %u: %u - %u kHz\n", new_policy->cpu,
		new_policy->min, new_policy->max);

	memcpy(&new_policy->cpuinfo, &policy->cpuinfo, sizeof(policy->cpuinfo));

	if (new_policy->min > policy->max || new_policy->max < policy->min) {
>>>>>>> refs/remotes/origin/master
		ret = -EINVAL;
		goto error_out;
	}

	/* verify the cpu speed can be set within this limit */
<<<<<<< HEAD
	ret = cpufreq_driver->verify(policy);
=======
	ret = cpufreq_driver->verify(new_policy);
>>>>>>> refs/remotes/origin/master
	if (ret)
		goto error_out;

	/* adjust if necessary - all reasons */
	blocking_notifier_call_chain(&cpufreq_policy_notifier_list,
<<<<<<< HEAD
			CPUFREQ_ADJUST, policy);

	/* adjust if necessary - hardware incompatibility*/
	blocking_notifier_call_chain(&cpufreq_policy_notifier_list,
			CPUFREQ_INCOMPATIBLE, policy);

	/* verify the cpu speed can be set within this limit,
	   which might be different to the first one */
	ret = cpufreq_driver->verify(policy);
=======
			CPUFREQ_ADJUST, new_policy);

	/* adjust if necessary - hardware incompatibility*/
	blocking_notifier_call_chain(&cpufreq_policy_notifier_list,
			CPUFREQ_INCOMPATIBLE, new_policy);

	/*
	 * verify the cpu speed can be set within this limit, which might be
	 * different to the first one
	 */
	ret = cpufreq_driver->verify(new_policy);
>>>>>>> refs/remotes/origin/master
	if (ret)
		goto error_out;

	/* notification of the new policy */
	blocking_notifier_call_chain(&cpufreq_policy_notifier_list,
<<<<<<< HEAD
			CPUFREQ_NOTIFY, policy);

	data->min = policy->min;
	data->max = policy->max;

	pr_debug("new min and max freqs are %u - %u kHz\n",
					data->min, data->max);

	if (cpufreq_driver->setpolicy) {
		data->policy = policy->policy;
		pr_debug("setting range\n");
		ret = cpufreq_driver->setpolicy(policy);
	} else {
		if (policy->governor != data->governor) {
			/* save old, working values */
			struct cpufreq_governor *old_gov = data->governor;
=======
			CPUFREQ_NOTIFY, new_policy);

	policy->min = new_policy->min;
	policy->max = new_policy->max;

	pr_debug("new min and max freqs are %u - %u kHz\n",
					policy->min, policy->max);

	if (cpufreq_driver->setpolicy) {
		policy->policy = new_policy->policy;
		pr_debug("setting range\n");
		ret = cpufreq_driver->setpolicy(new_policy);
	} else {
		if (new_policy->governor != policy->governor) {
			/* save old, working values */
			struct cpufreq_governor *old_gov = policy->governor;
>>>>>>> refs/remotes/origin/master

			pr_debug("governor switch\n");

			/* end old governor */
<<<<<<< HEAD
			if (data->governor)
				__cpufreq_governor(data, CPUFREQ_GOV_STOP);

			/* start new governor */
			data->governor = policy->governor;
			if (__cpufreq_governor(data, CPUFREQ_GOV_START)) {
				/* new governor failed, so re-start old one */
				pr_debug("starting governor %s failed\n",
							data->governor->name);
				if (old_gov) {
					data->governor = old_gov;
					__cpufreq_governor(data,
=======
			if (policy->governor) {
				__cpufreq_governor(policy, CPUFREQ_GOV_STOP);
				up_write(&policy->rwsem);
				__cpufreq_governor(policy,
						CPUFREQ_GOV_POLICY_EXIT);
				down_write(&policy->rwsem);
			}

			/* start new governor */
			policy->governor = new_policy->governor;
			if (!__cpufreq_governor(policy, CPUFREQ_GOV_POLICY_INIT)) {
				if (!__cpufreq_governor(policy, CPUFREQ_GOV_START)) {
					failed = 0;
				} else {
					up_write(&policy->rwsem);
					__cpufreq_governor(policy,
							CPUFREQ_GOV_POLICY_EXIT);
					down_write(&policy->rwsem);
				}
			}

			if (failed) {
				/* new governor failed, so re-start old one */
				pr_debug("starting governor %s failed\n",
							policy->governor->name);
				if (old_gov) {
					policy->governor = old_gov;
					__cpufreq_governor(policy,
							CPUFREQ_GOV_POLICY_INIT);
					__cpufreq_governor(policy,
>>>>>>> refs/remotes/origin/master
							   CPUFREQ_GOV_START);
				}
				ret = -EINVAL;
				goto error_out;
			}
			/* might be a policy change, too, so fall through */
		}
		pr_debug("governor: change or update limits\n");
<<<<<<< HEAD
		__cpufreq_governor(data, CPUFREQ_GOV_LIMITS);
=======
		ret = __cpufreq_governor(policy, CPUFREQ_GOV_LIMITS);
>>>>>>> refs/remotes/origin/master
	}

error_out:
	return ret;
}

/**
 *	cpufreq_update_policy - re-evaluate an existing cpufreq policy
 *	@cpu: CPU which shall be re-evaluated
 *
 *	Useful for policy notifiers which have different necessities
 *	at different times.
 */
int cpufreq_update_policy(unsigned int cpu)
{
<<<<<<< HEAD
	struct cpufreq_policy *data = cpufreq_cpu_get(cpu);
	struct cpufreq_policy policy;
	int ret;

	if (!data) {
=======
	struct cpufreq_policy *policy = cpufreq_cpu_get(cpu);
	struct cpufreq_policy new_policy;
	int ret;

	if (!policy) {
>>>>>>> refs/remotes/origin/master
		ret = -ENODEV;
		goto no_policy;
	}

<<<<<<< HEAD
	if (unlikely(lock_policy_rwsem_write(cpu))) {
		ret = -EINVAL;
		goto fail;
	}

	pr_debug("updating policy for CPU %u\n", cpu);
	memcpy(&policy, data, sizeof(struct cpufreq_policy));
	policy.min = data->user_policy.min;
	policy.max = data->user_policy.max;
	policy.policy = data->user_policy.policy;
	policy.governor = data->user_policy.governor;

	/* BIOS might change freq behind our back
	  -> ask driver for current freq and notify governors about a change */
	if (cpufreq_driver->get) {
		policy.cur = cpufreq_driver->get(cpu);
		if (!data->cur) {
			pr_debug("Driver did not initialize current freq");
			data->cur = policy.cur;
		} else {
			if (data->cur != policy.cur)
				cpufreq_out_of_sync(cpu, data->cur,
								policy.cur);
		}
	}

	ret = __cpufreq_set_policy(data, &policy);

	unlock_policy_rwsem_write(cpu);

fail:
	cpufreq_cpu_put(data);
=======
	down_write(&policy->rwsem);

	pr_debug("updating policy for CPU %u\n", cpu);
	memcpy(&new_policy, policy, sizeof(*policy));
	new_policy.min = policy->user_policy.min;
	new_policy.max = policy->user_policy.max;
	new_policy.policy = policy->user_policy.policy;
	new_policy.governor = policy->user_policy.governor;

	/*
	 * BIOS might change freq behind our back
	 * -> ask driver for current freq and notify governors about a change
	 */
	if (cpufreq_driver->get) {
		new_policy.cur = cpufreq_driver->get(cpu);
		if (!policy->cur) {
			pr_debug("Driver did not initialize current freq");
			policy->cur = new_policy.cur;
		} else {
			if (policy->cur != new_policy.cur && has_target())
				cpufreq_out_of_sync(cpu, policy->cur,
								new_policy.cur);
		}
	}

	ret = cpufreq_set_policy(policy, &new_policy);

	up_write(&policy->rwsem);

	cpufreq_cpu_put(policy);
>>>>>>> refs/remotes/origin/master
no_policy:
	return ret;
}
EXPORT_SYMBOL(cpufreq_update_policy);

<<<<<<< HEAD
static int __cpuinit cpufreq_cpu_callback(struct notifier_block *nfb,
					unsigned long action, void *hcpu)
{
	unsigned int cpu = (unsigned long)hcpu;
<<<<<<< HEAD
	struct sys_device *sys_dev;

	sys_dev = get_cpu_sysdev(cpu);
	if (sys_dev) {
		switch (action) {
		case CPU_ONLINE:
		case CPU_ONLINE_FROZEN:
			cpufreq_add_dev(sys_dev);
=======
	struct device *dev;

	dev = get_cpu_device(cpu);
	if (dev) {
		switch (action) {
		case CPU_ONLINE:
		case CPU_ONLINE_FROZEN:
			cpufreq_add_dev(dev, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
			break;
		case CPU_DOWN_PREPARE:
		case CPU_DOWN_PREPARE_FROZEN:
			if (unlikely(lock_policy_rwsem_write(cpu)))
				BUG();

<<<<<<< HEAD
			__cpufreq_remove_dev(sys_dev);
			break;
		case CPU_DOWN_FAILED:
		case CPU_DOWN_FAILED_FROZEN:
			cpufreq_add_dev(sys_dev);
=======
			__cpufreq_remove_dev(dev, NULL);
			break;
		case CPU_DOWN_FAILED:
		case CPU_DOWN_FAILED_FROZEN:
			cpufreq_add_dev(dev, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int cpufreq_cpu_callback(struct notifier_block *nfb,
					unsigned long action, void *hcpu)
{
	unsigned int cpu = (unsigned long)hcpu;
	struct device *dev;
	bool frozen = false;

	dev = get_cpu_device(cpu);
	if (dev) {

		if (action & CPU_TASKS_FROZEN)
			frozen = true;

		switch (action & ~CPU_TASKS_FROZEN) {
		case CPU_ONLINE:
			__cpufreq_add_dev(dev, NULL, frozen);
			cpufreq_update_policy(cpu);
			break;

		case CPU_DOWN_PREPARE:
			__cpufreq_remove_dev_prepare(dev, NULL, frozen);
			break;

		case CPU_POST_DEAD:
			__cpufreq_remove_dev_finish(dev, NULL, frozen);
			break;

		case CPU_DOWN_FAILED:
			__cpufreq_add_dev(dev, NULL, frozen);
>>>>>>> refs/remotes/origin/master
			break;
		}
	}
	return NOTIFY_OK;
}

static struct notifier_block __refdata cpufreq_cpu_notifier = {
<<<<<<< HEAD
    .notifier_call = cpufreq_cpu_callback,
=======
	.notifier_call = cpufreq_cpu_callback,
>>>>>>> refs/remotes/origin/master
};

/*********************************************************************
 *               REGISTER / UNREGISTER CPUFREQ DRIVER                *
 *********************************************************************/

/**
 * cpufreq_register_driver - register a CPU Frequency driver
 * @driver_data: A struct cpufreq_driver containing the values#
 * submitted by the CPU Frequency driver.
 *
<<<<<<< HEAD
 *   Registers a CPU Frequency driver to this core code. This code
=======
 * Registers a CPU Frequency driver to this core code. This code
>>>>>>> refs/remotes/origin/master
 * returns zero on success, -EBUSY when another driver got here first
 * (and isn't unregistered in the meantime).
 *
 */
int cpufreq_register_driver(struct cpufreq_driver *driver_data)
{
	unsigned long flags;
	int ret;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (cpufreq_disabled())
		return -ENODEV;

>>>>>>> refs/remotes/origin/cm-10.0
	if (!driver_data || !driver_data->verify || !driver_data->init ||
	    ((!driver_data->setpolicy) && (!driver_data->target)))
=======
	if (cpufreq_disabled())
		return -ENODEV;

	if (!driver_data || !driver_data->verify || !driver_data->init ||
	    !(driver_data->setpolicy || driver_data->target_index ||
		    driver_data->target))
>>>>>>> refs/remotes/origin/master
		return -EINVAL;

	pr_debug("trying to register driver %s\n", driver_data->name);

	if (driver_data->setpolicy)
		driver_data->flags |= CPUFREQ_CONST_LOOPS;

<<<<<<< HEAD
	spin_lock_irqsave(&cpufreq_driver_lock, flags);
	if (cpufreq_driver) {
		spin_unlock_irqrestore(&cpufreq_driver_lock, flags);
		return -EBUSY;
	}
	cpufreq_driver = driver_data;
	spin_unlock_irqrestore(&cpufreq_driver_lock, flags);

<<<<<<< HEAD
	ret = sysdev_driver_register(&cpu_sysdev_class,
					&cpufreq_sysdev_driver);
=======
	ret = subsys_interface_register(&cpufreq_interface);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	write_lock_irqsave(&cpufreq_driver_lock, flags);
	if (cpufreq_driver) {
		write_unlock_irqrestore(&cpufreq_driver_lock, flags);
		return -EEXIST;
	}
	cpufreq_driver = driver_data;
	write_unlock_irqrestore(&cpufreq_driver_lock, flags);

	ret = subsys_interface_register(&cpufreq_interface);
>>>>>>> refs/remotes/origin/master
	if (ret)
		goto err_null_driver;

	if (!(cpufreq_driver->flags & CPUFREQ_STICKY)) {
		int i;
		ret = -ENODEV;

		/* check for at least one working CPU */
		for (i = 0; i < nr_cpu_ids; i++)
			if (cpu_possible(i) && per_cpu(cpufreq_cpu_data, i)) {
				ret = 0;
				break;
			}

		/* if all ->init() calls failed, unregister */
		if (ret) {
			pr_debug("no CPU initialized for driver %s\n",
							driver_data->name);
<<<<<<< HEAD
<<<<<<< HEAD
			goto err_sysdev_unreg;
=======
			goto err_if_unreg;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			goto err_if_unreg;
>>>>>>> refs/remotes/origin/master
		}
	}

	register_hotcpu_notifier(&cpufreq_cpu_notifier);
	pr_debug("driver %s up and running\n", driver_data->name);

	return 0;
<<<<<<< HEAD
<<<<<<< HEAD
err_sysdev_unreg:
	sysdev_driver_unregister(&cpu_sysdev_class,
			&cpufreq_sysdev_driver);
=======
err_if_unreg:
	subsys_interface_unregister(&cpufreq_interface);
>>>>>>> refs/remotes/origin/cm-10.0
err_null_driver:
	spin_lock_irqsave(&cpufreq_driver_lock, flags);
	cpufreq_driver = NULL;
	spin_unlock_irqrestore(&cpufreq_driver_lock, flags);
=======
err_if_unreg:
	subsys_interface_unregister(&cpufreq_interface);
err_null_driver:
	write_lock_irqsave(&cpufreq_driver_lock, flags);
	cpufreq_driver = NULL;
	write_unlock_irqrestore(&cpufreq_driver_lock, flags);
>>>>>>> refs/remotes/origin/master
	return ret;
}
EXPORT_SYMBOL_GPL(cpufreq_register_driver);

<<<<<<< HEAD

/**
 * cpufreq_unregister_driver - unregister the current CPUFreq driver
 *
 *    Unregister the current CPUFreq driver. Only call this if you have
=======
/**
 * cpufreq_unregister_driver - unregister the current CPUFreq driver
 *
 * Unregister the current CPUFreq driver. Only call this if you have
>>>>>>> refs/remotes/origin/master
 * the right to do so, i.e. if you have succeeded in initialising before!
 * Returns zero if successful, and -EINVAL if the cpufreq_driver is
 * currently not initialised.
 */
int cpufreq_unregister_driver(struct cpufreq_driver *driver)
{
	unsigned long flags;

	if (!cpufreq_driver || (driver != cpufreq_driver))
		return -EINVAL;

	pr_debug("unregistering driver %s\n", driver->name);

<<<<<<< HEAD
<<<<<<< HEAD
	sysdev_driver_unregister(&cpu_sysdev_class, &cpufreq_sysdev_driver);
=======
	subsys_interface_unregister(&cpufreq_interface);
>>>>>>> refs/remotes/origin/cm-10.0
	unregister_hotcpu_notifier(&cpufreq_cpu_notifier);

	spin_lock_irqsave(&cpufreq_driver_lock, flags);
	cpufreq_driver = NULL;
	spin_unlock_irqrestore(&cpufreq_driver_lock, flags);
=======
	subsys_interface_unregister(&cpufreq_interface);
	unregister_hotcpu_notifier(&cpufreq_cpu_notifier);

	down_write(&cpufreq_rwsem);
	write_lock_irqsave(&cpufreq_driver_lock, flags);

	cpufreq_driver = NULL;

	write_unlock_irqrestore(&cpufreq_driver_lock, flags);
	up_write(&cpufreq_rwsem);
>>>>>>> refs/remotes/origin/master

	return 0;
}
EXPORT_SYMBOL_GPL(cpufreq_unregister_driver);

static int __init cpufreq_core_init(void)
{
<<<<<<< HEAD
	int cpu;

<<<<<<< HEAD
=======
	if (cpufreq_disabled())
		return -ENODEV;

>>>>>>> refs/remotes/origin/cm-10.0
	for_each_possible_cpu(cpu) {
		per_cpu(cpufreq_policy_cpu, cpu) = -1;
		init_rwsem(&per_cpu(cpu_policy_rwsem, cpu));
	}

<<<<<<< HEAD
	cpufreq_global_kobject = kobject_create_and_add("cpufreq",
						&cpu_sysdev_class.kset.kobj);
=======
	cpufreq_global_kobject = kobject_create_and_add("cpufreq", &cpu_subsys.dev_root->kobj);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (cpufreq_disabled())
		return -ENODEV;

	cpufreq_global_kobject = kobject_create();
>>>>>>> refs/remotes/origin/master
	BUG_ON(!cpufreq_global_kobject);
	register_syscore_ops(&cpufreq_syscore_ops);

	return 0;
}
core_initcall(cpufreq_core_init);
