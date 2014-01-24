/*
 * include/linux/cpu.h - generic cpu definition
 *
 * This is mainly for topological representation. We define the 
 * basic 'struct cpu' here, which can be embedded in per-arch 
 * definitions of processors.
 *
 * Basic handling of the devices is done in drivers/base/cpu.c
<<<<<<< HEAD
 * and system devices are handled in drivers/base/sys.c. 
 *
 * CPUs are exported via sysfs in the class/cpu/devices/
=======
 *
 * CPUs are exported via sysfs in the devices/system/cpu
>>>>>>> refs/remotes/origin/master
 * directory. 
 */
#ifndef _LINUX_CPU_H_
#define _LINUX_CPU_H_

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/sysdev.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <linux/node.h>
#include <linux/compiler.h>
#include <linux/cpumask.h>

<<<<<<< HEAD
<<<<<<< HEAD
struct cpu {
	int node_id;		/* The node which contains the CPU */
	int hotpluggable;	/* creates sysfs control file if hotpluggable */
	struct sys_device sysdev;
};

extern int register_cpu(struct cpu *cpu, int num);
extern struct sys_device *get_cpu_sysdev(unsigned cpu);

extern int cpu_add_sysdev_attr(struct sysdev_attribute *attr);
extern void cpu_remove_sysdev_attr(struct sysdev_attribute *attr);

extern int cpu_add_sysdev_attr_group(struct attribute_group *attrs);
extern void cpu_remove_sysdev_attr_group(struct attribute_group *attrs);

extern int sched_create_sysfs_power_savings_entries(struct sysdev_class *cls);
=======
struct device;
=======
struct device;
struct device_node;
>>>>>>> refs/remotes/origin/master

struct cpu {
	int node_id;		/* The node which contains the CPU */
	int hotpluggable;	/* creates sysfs control file if hotpluggable */
	struct device dev;
};

extern int register_cpu(struct cpu *cpu, int num);
extern struct device *get_cpu_device(unsigned cpu);
extern bool cpu_is_hotpluggable(unsigned cpu);
<<<<<<< HEAD
=======
extern bool arch_match_cpu_phys_id(int cpu, u64 phys_id);
extern bool arch_find_n_match_cpu_physical_id(struct device_node *cpun,
					      int cpu, unsigned int *thread);
>>>>>>> refs/remotes/origin/master

extern int cpu_add_dev_attr(struct device_attribute *attr);
extern void cpu_remove_dev_attr(struct device_attribute *attr);

extern int cpu_add_dev_attr_group(struct attribute_group *attrs);
extern void cpu_remove_dev_attr_group(struct attribute_group *attrs);

<<<<<<< HEAD
extern int sched_create_sysfs_power_savings_entries(struct device *dev);
>>>>>>> refs/remotes/origin/cm-10.0

=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_HOTPLUG_CPU
extern void unregister_cpu(struct cpu *cpu);
extern ssize_t arch_cpu_probe(const char *, size_t);
extern ssize_t arch_cpu_release(const char *, size_t);
#endif
struct notifier_block;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_ARCH_HAS_CPU_AUTOPROBE
extern int arch_cpu_uevent(struct device *dev, struct kobj_uevent_env *env);
extern ssize_t arch_print_cpu_modalias(struct device *dev,
				       struct device_attribute *attr,
				       char *bufptr);
#endif

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * CPU notifier priorities.
 */
enum {
	/*
	 * SCHED_ACTIVE marks a cpu which is coming up active during
	 * CPU_ONLINE and CPU_DOWN_FAILED and must be the first
	 * notifier.  CPUSET_ACTIVE adjusts cpuset according to
	 * cpu_active mask right after SCHED_ACTIVE.  During
	 * CPU_DOWN_PREPARE, SCHED_INACTIVE and CPUSET_INACTIVE are
	 * ordered in the similar way.
	 *
	 * This ordering guarantees consistent cpu_active mask and
	 * migration behavior to all cpu notifiers.
	 */
	CPU_PRI_SCHED_ACTIVE	= INT_MAX,
	CPU_PRI_CPUSET_ACTIVE	= INT_MAX - 1,
	CPU_PRI_SCHED_INACTIVE	= INT_MIN + 1,
	CPU_PRI_CPUSET_INACTIVE	= INT_MIN,

	/* migration should happen before other stuff but after perf */
	CPU_PRI_PERF		= 20,
	CPU_PRI_MIGRATION	= 10,
	/* bring up workqueues before normal notifiers and down after */
	CPU_PRI_WORKQUEUE_UP	= 5,
	CPU_PRI_WORKQUEUE_DOWN	= -5,
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#define CPU_ONLINE		0x0002 /* CPU (unsigned)v is up */
#define CPU_UP_PREPARE		0x0003 /* CPU (unsigned)v coming up */
#define CPU_UP_CANCELED		0x0004 /* CPU (unsigned)v NOT coming up */
#define CPU_DOWN_PREPARE	0x0005 /* CPU (unsigned)v going down */
#define CPU_DOWN_FAILED		0x0006 /* CPU (unsigned)v NOT going down */
#define CPU_DEAD		0x0007 /* CPU (unsigned)v dead */
#define CPU_DYING		0x0008 /* CPU (unsigned)v not running any task,
					* not handling interrupts, soon dead.
					* Called on the dying cpu, interrupts
					* are already disabled. Must not
					* sleep, must not fail */
#define CPU_POST_DEAD		0x0009 /* CPU (unsigned)v dead, cpu_hotplug
					* lock is dropped */
#define CPU_STARTING		0x000A /* CPU (unsigned)v soon running.
					* Called on the new cpu, just before
					* enabling interrupts. Must not sleep,
					* must not fail */

/* Used for CPU hotplug events occurring while tasks are frozen due to a suspend
 * operation in progress
 */
#define CPU_TASKS_FROZEN	0x0010

#define CPU_ONLINE_FROZEN	(CPU_ONLINE | CPU_TASKS_FROZEN)
#define CPU_UP_PREPARE_FROZEN	(CPU_UP_PREPARE | CPU_TASKS_FROZEN)
#define CPU_UP_CANCELED_FROZEN	(CPU_UP_CANCELED | CPU_TASKS_FROZEN)
#define CPU_DOWN_PREPARE_FROZEN	(CPU_DOWN_PREPARE | CPU_TASKS_FROZEN)
#define CPU_DOWN_FAILED_FROZEN	(CPU_DOWN_FAILED | CPU_TASKS_FROZEN)
#define CPU_DEAD_FROZEN		(CPU_DEAD | CPU_TASKS_FROZEN)
#define CPU_DYING_FROZEN	(CPU_DYING | CPU_TASKS_FROZEN)
#define CPU_STARTING_FROZEN	(CPU_STARTING | CPU_TASKS_FROZEN)


<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_SMP
/* Need to know about CPUs going up/down? */
#if defined(CONFIG_HOTPLUG_CPU) || !defined(MODULE)
#define cpu_notifier(fn, pri) {					\
<<<<<<< HEAD
	static struct notifier_block fn##_nb __cpuinitdata =	\
=======
	static struct notifier_block fn##_nb =			\
>>>>>>> refs/remotes/origin/master
		{ .notifier_call = fn, .priority = pri };	\
	register_cpu_notifier(&fn##_nb);			\
}
#else /* #if defined(CONFIG_HOTPLUG_CPU) || !defined(MODULE) */
#define cpu_notifier(fn, pri)	do { (void)(fn); } while (0)
#endif /* #else #if defined(CONFIG_HOTPLUG_CPU) || !defined(MODULE) */
#ifdef CONFIG_HOTPLUG_CPU
extern int register_cpu_notifier(struct notifier_block *nb);
extern void unregister_cpu_notifier(struct notifier_block *nb);
#else

#ifndef MODULE
extern int register_cpu_notifier(struct notifier_block *nb);
#else
static inline int register_cpu_notifier(struct notifier_block *nb)
{
	return 0;
}
#endif

static inline void unregister_cpu_notifier(struct notifier_block *nb)
{
}
#endif

int cpu_up(unsigned int cpu);
void notify_cpu_starting(unsigned int cpu);
extern void cpu_maps_update_begin(void);
extern void cpu_maps_update_done(void);

#else	/* CONFIG_SMP */

#define cpu_notifier(fn, pri)	do { (void)(fn); } while (0)

static inline int register_cpu_notifier(struct notifier_block *nb)
{
	return 0;
}

static inline void unregister_cpu_notifier(struct notifier_block *nb)
{
}

static inline void cpu_maps_update_begin(void)
{
}

static inline void cpu_maps_update_done(void)
{
}

#endif /* CONFIG_SMP */
<<<<<<< HEAD
<<<<<<< HEAD
extern struct sysdev_class cpu_sysdev_class;
=======
extern struct bus_type cpu_subsys;
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern struct bus_type cpu_subsys;
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_HOTPLUG_CPU
/* Stop CPUs going up and down. */

<<<<<<< HEAD
extern void get_online_cpus(void);
extern void put_online_cpus(void);
<<<<<<< HEAD
<<<<<<< HEAD
=======
extern void cpu_hotplug_disable(void);
extern void cpu_hotplug_enable(void);
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern void cpu_hotplug_disable(void);
extern void cpu_hotplug_enable(void);
>>>>>>> refs/remotes/origin/cm-11.0
#define hotcpu_notifier(fn, pri)	cpu_notifier(fn, pri)
#define register_hotcpu_notifier(nb)	register_cpu_notifier(nb)
#define unregister_hotcpu_notifier(nb)	unregister_cpu_notifier(nb)
int cpu_down(unsigned int cpu);

#ifdef CONFIG_ARCH_CPU_PROBE_RELEASE
extern void cpu_hotplug_driver_lock(void);
extern void cpu_hotplug_driver_unlock(void);
#else
static inline void cpu_hotplug_driver_lock(void)
{
}

static inline void cpu_hotplug_driver_unlock(void)
{
}
#endif

#else		/* CONFIG_HOTPLUG_CPU */

#define get_online_cpus()	do { } while (0)
#define put_online_cpus()	do { } while (0)
<<<<<<< HEAD
<<<<<<< HEAD
=======
#define cpu_hotplug_disable()	do { } while (0)
#define cpu_hotplug_enable()	do { } while (0)
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern void cpu_hotplug_begin(void);
extern void cpu_hotplug_done(void);
extern void get_online_cpus(void);
extern void put_online_cpus(void);
extern void cpu_hotplug_disable(void);
extern void cpu_hotplug_enable(void);
#define hotcpu_notifier(fn, pri)	cpu_notifier(fn, pri)
#define register_hotcpu_notifier(nb)	register_cpu_notifier(nb)
#define unregister_hotcpu_notifier(nb)	unregister_cpu_notifier(nb)
void clear_tasks_mm_cpumask(int cpu);
int cpu_down(unsigned int cpu);

#else		/* CONFIG_HOTPLUG_CPU */

static inline void cpu_hotplug_begin(void) {}
static inline void cpu_hotplug_done(void) {}
#define get_online_cpus()	do { } while (0)
#define put_online_cpus()	do { } while (0)
#define cpu_hotplug_disable()	do { } while (0)
#define cpu_hotplug_enable()	do { } while (0)
>>>>>>> refs/remotes/origin/master
=======
#define cpu_hotplug_disable()	do { } while (0)
#define cpu_hotplug_enable()	do { } while (0)
>>>>>>> refs/remotes/origin/cm-11.0
#define hotcpu_notifier(fn, pri)	do { (void)(fn); } while (0)
/* These aren't inline functions due to a GCC bug. */
#define register_hotcpu_notifier(nb)	({ (void)(nb); 0; })
#define unregister_hotcpu_notifier(nb)	({ (void)(nb); })
#endif		/* CONFIG_HOTPLUG_CPU */

#ifdef CONFIG_PM_SLEEP_SMP
<<<<<<< HEAD
<<<<<<< HEAD
extern int suspend_cpu_hotplug;

extern int disable_nonboot_cpus(void);
extern void enable_nonboot_cpus(void);
#else /* !CONFIG_PM_SLEEP_SMP */
#define suspend_cpu_hotplug	0

=======
extern int disable_nonboot_cpus(void);
extern void enable_nonboot_cpus(void);
#else /* !CONFIG_PM_SLEEP_SMP */
>>>>>>> refs/remotes/origin/cm-10.0
=======
extern int disable_nonboot_cpus(void);
extern void enable_nonboot_cpus(void);
#else /* !CONFIG_PM_SLEEP_SMP */
>>>>>>> refs/remotes/origin/master
static inline int disable_nonboot_cpus(void) { return 0; }
static inline void enable_nonboot_cpus(void) {}
#endif /* !CONFIG_PM_SLEEP_SMP */

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
#define IDLE_START 1
#define IDLE_END 2

void idle_notifier_register(struct notifier_block *n);
void idle_notifier_unregister(struct notifier_block *n);
void idle_notifier_call_chain(unsigned long val);
<<<<<<< HEAD
=======
enum cpuhp_state {
	CPUHP_OFFLINE,
	CPUHP_ONLINE,
};

void cpu_startup_entry(enum cpuhp_state state);
void cpu_idle(void);

void cpu_idle_poll_ctrl(bool enable);

void arch_cpu_idle(void);
void arch_cpu_idle_prepare(void);
void arch_cpu_idle_enter(void);
void arch_cpu_idle_exit(void);
void arch_cpu_idle_dead(void);
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0

#endif /* _LINUX_CPU_H_ */
