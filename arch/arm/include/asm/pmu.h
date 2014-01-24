/*
 *  linux/arch/arm/include/asm/pmu.h
 *
 *  Copyright (C) 2009 picoChip Designs Ltd, Jamie Iles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#ifndef __ARM_PMU_H__
#define __ARM_PMU_H__

#include <linux/interrupt.h>
<<<<<<< HEAD
<<<<<<< HEAD

enum arm_pmu_type {
	ARM_PMU_DEVICE_CPU	= 0,
	ARM_PMU_DEVICE_L2	= 1,
=======
#include <linux/perf_event.h>

/*
 * Types of PMUs that can be accessed directly and require mutual
 * exclusion between profiling tools.
 */
enum arm_pmu_type {
	ARM_PMU_DEVICE_CPU	= 0,
	ARM_PMU_DEVICE_L2CC	= 1,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	ARM_NUM_PMU_DEVICES,
};

/*
 * struct arm_pmu_platdata - ARM PMU platform data
 *
<<<<<<< HEAD
 * @handle_irq: an optional handler which will be called from the interrupt and
 * passed the address of the low level handler, and can be used to implement
 * any platform specific handling before or after calling it.
=======
=======
#include <linux/perf_event.h>

/*
 * struct arm_pmu_platdata - ARM PMU platform data
 *
>>>>>>> refs/remotes/origin/master
 * @handle_irq: an optional handler which will be called from the
 *	interrupt and passed the address of the low level handler,
 *	and can be used to implement any platform specific handling
 *	before or after calling it.
<<<<<<< HEAD
 * @enable_irq: an optional handler which will be called after
 *	request_irq and be used to handle some platform specific
 *	irq enablement
 * @disable_irq: an optional handler which will be called before
 *	free_irq and be used to handle some platform specific
 *	irq disablement
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * @runtime_resume: an optional handler which will be called by the
 *	runtime PM framework following a call to pm_runtime_get().
 *	Note that if pm_runtime_get() is called more than once in
 *	succession this handler will only be called once.
 * @runtime_suspend: an optional handler which will be called by the
 *	runtime PM framework following a call to pm_runtime_put().
 *	Note that if pm_runtime_get() is called more than once in
 *	succession this handler will only be called following the
 *	final call to pm_runtime_put() that actually disables the
 *	hardware.
>>>>>>> refs/remotes/origin/master
 */
struct arm_pmu_platdata {
	irqreturn_t (*handle_irq)(int irq, void *dev,
				  irq_handler_t pmu_handler);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	void (*enable_irq)(int irq);
	void (*disable_irq)(int irq);
>>>>>>> refs/remotes/origin/cm-10.0
};

#ifdef CONFIG_CPU_HAS_PMU

/**
 * reserve_pmu() - reserve the hardware performance counters
 *
 * Reserve the hardware performance counters in the system for exclusive use.
<<<<<<< HEAD
 * The platform_device for the system is returned on success, ERR_PTR()
 * encoded error on failure.
 */
extern struct platform_device *
reserve_pmu(enum arm_pmu_type device);
=======
 * Returns 0 on success or -EBUSY if the lock is already held.
 */
extern int
reserve_pmu(enum arm_pmu_type type);
>>>>>>> refs/remotes/origin/cm-10.0

/**
 * release_pmu() - Relinquish control of the performance counters
 *
 * Release the performance counters and allow someone else to use them.
<<<<<<< HEAD
 * Callers must have disabled the counters and released IRQs before calling
 * this. The platform_device returned from reserve_pmu() must be passed as
 * a cookie.
 */
extern int
release_pmu(struct platform_device *pdev);

/**
 * init_pmu() - Initialise the PMU.
 *
 * Initialise the system ready for PMU enabling. This should typically set the
 * IRQ affinity and nothing else. The users (oprofile/perf events etc) will do
 * the actual hardware initialisation.
 */
extern int
init_pmu(enum arm_pmu_type device);
=======
 */
extern void
release_pmu(enum arm_pmu_type type);
>>>>>>> refs/remotes/origin/cm-10.0

#else /* CONFIG_CPU_HAS_PMU */

#include <linux/err.h>

<<<<<<< HEAD
static inline struct platform_device *
reserve_pmu(enum arm_pmu_type device)
{
	return ERR_PTR(-ENODEV);
}

static inline int
release_pmu(struct platform_device *pdev)
=======
static inline int
reserve_pmu(enum arm_pmu_type type)
>>>>>>> refs/remotes/origin/cm-10.0
{
	return -ENODEV;
}

<<<<<<< HEAD
static inline int
init_pmu(enum arm_pmu_type device)
{
	return -ENODEV;
}

#endif /* CONFIG_CPU_HAS_PMU */

=======
static inline void
release_pmu(enum arm_pmu_type type)	{ }

#endif /* CONFIG_CPU_HAS_PMU */

=======
	int (*runtime_resume)(struct device *dev);
	int (*runtime_suspend)(struct device *dev);
};

>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_HW_PERF_EVENTS

/* The events for a given PMU register set. */
struct pmu_hw_events {
	/*
	 * The events that are active on the PMU for the given index.
	 */
	struct perf_event	**events;

	/*
	 * A 1 bit for an index indicates that the counter is being used for
	 * an event. A 0 means that the counter can be used.
	 */
	unsigned long           *used_mask;

	/*
	 * Hardware lock to serialize accesses to PMU registers. Needed for the
	 * read/modify/write sequences.
	 */
	raw_spinlock_t		pmu_lock;
};

struct arm_pmu {
	struct pmu	pmu;
<<<<<<< HEAD
	enum arm_perf_pmu_ids id;
	enum arm_pmu_type type;
	cpumask_t	active_irqs;
	const char	*name;
	irqreturn_t	(*handle_irq)(int irq_num, void *dev);
	int     	(*request_pmu_irq)(int irq, irq_handler_t *irq_h);
	void    	(*free_pmu_irq)(int irq);
	void		(*enable)(struct hw_perf_event *evt, int idx, int cpu);
	void		(*disable)(struct hw_perf_event *evt, int idx);
	int		(*get_event_idx)(struct pmu_hw_events *hw_events,
					 struct hw_perf_event *hwc);
	int		(*set_event_filter)(struct hw_perf_event *evt,
					    struct perf_event_attr *attr);
	u32		(*read_counter)(int idx);
	void		(*write_counter)(int idx, u32 val);
	void		(*start)(void);
	void		(*stop)(void);
	void		(*reset)(void *);
=======
	cpumask_t	active_irqs;
	char		*name;
	irqreturn_t	(*handle_irq)(int irq_num, void *dev);
	void		(*enable)(struct perf_event *event);
	void		(*disable)(struct perf_event *event);
	int		(*get_event_idx)(struct pmu_hw_events *hw_events,
					 struct perf_event *event);
	int		(*set_event_filter)(struct hw_perf_event *evt,
					    struct perf_event_attr *attr);
	u32		(*read_counter)(struct perf_event *event);
	void		(*write_counter)(struct perf_event *event, u32 val);
	void		(*start)(struct arm_pmu *);
	void		(*stop)(struct arm_pmu *);
	void		(*reset)(void *);
	int		(*request_irq)(struct arm_pmu *, irq_handler_t handler);
	void		(*free_irq)(struct arm_pmu *);
>>>>>>> refs/remotes/origin/master
	int		(*map_event)(struct perf_event *event);
	int		num_events;
	atomic_t	active_events;
	struct mutex	reserve_mutex;
	u64		max_period;
	struct platform_device	*plat_device;
	struct pmu_hw_events	*(*get_hw_events)(void);
<<<<<<< HEAD
<<<<<<< HEAD
	int	(*test_set_event_constraints)(struct perf_event *event);
	int	(*clear_event_constraints)(struct perf_event *event);
=======
>>>>>>> refs/remotes/origin/master
=======
	int	(*test_set_event_constraints)(struct perf_event *event);
	int	(*clear_event_constraints)(struct perf_event *event);
>>>>>>> refs/remotes/origin/cm-11.0
};

#define to_arm_pmu(p) (container_of(p, struct arm_pmu, pmu))

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
int armpmu_register(struct arm_pmu *armpmu, char *name, int type);

u64 armpmu_event_update(struct perf_event *event,
			struct hw_perf_event *hwc,
			int idx);

int armpmu_event_set_period(struct perf_event *event,
			    struct hw_perf_event *hwc,
			    int idx);

#endif /* CONFIG_HW_PERF_EVENTS */

>>>>>>> refs/remotes/origin/cm-10.0
=======
extern const struct dev_pm_ops armpmu_dev_pm_ops;

int armpmu_register(struct arm_pmu *armpmu, int type);

u64 armpmu_event_update(struct perf_event *event);

int armpmu_event_set_period(struct perf_event *event);

int armpmu_map_event(struct perf_event *event,
		     const unsigned (*event_map)[PERF_COUNT_HW_MAX],
		     const unsigned (*cache_map)[PERF_COUNT_HW_CACHE_MAX]
						[PERF_COUNT_HW_CACHE_OP_MAX]
						[PERF_COUNT_HW_CACHE_RESULT_MAX],
		     u32 raw_event_mask);

#endif /* CONFIG_HW_PERF_EVENTS */

>>>>>>> refs/remotes/origin/master
#endif /* __ARM_PMU_H__ */
