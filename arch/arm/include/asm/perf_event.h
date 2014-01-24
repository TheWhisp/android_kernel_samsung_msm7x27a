/*
 *  linux/arch/arm/include/asm/perf_event.h
 *
 *  Copyright (C) 2009 picoChip Designs Ltd, Jamie Iles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#ifndef __ARM_PERF_EVENT_H__
#define __ARM_PERF_EVENT_H__

<<<<<<< HEAD
<<<<<<< HEAD
/* ARM performance counters start from 1 (in the cp15 accesses) so use the
 * same indexes here for consistency. */
#define PERF_EVENT_INDEX_OFFSET 1

=======
>>>>>>> refs/remotes/origin/cm-10.0
/* ARM perf PMU IDs for use by internal perf clients. */
enum arm_perf_pmu_ids {
	ARM_PERF_PMU_ID_XSCALE1	= 0,
	ARM_PERF_PMU_ID_XSCALE2,
	ARM_PERF_PMU_ID_V6,
	ARM_PERF_PMU_ID_V6MP,
	ARM_PERF_PMU_ID_CA8,
	ARM_PERF_PMU_ID_CA9,
	ARM_PERF_PMU_ID_CA5,
	ARM_PERF_PMU_ID_CA15,
<<<<<<< HEAD
	ARM_PERF_PMU_ID_SCORPION,
	ARM_PERF_PMU_ID_SCORPIONMP,
	ARM_PERF_PMU_ID_KRAIT,
=======
	ARM_PERF_PMU_ID_CA7,
	ARM_PERF_PMU_ID_SCORPION,
	ARM_PERF_PMU_ID_SCORPIONMP,
	ARM_PERF_PMU_ID_SCORPIONMP_L2,
	ARM_PERF_PMU_ID_KRAIT,
	ARM_PERF_PMU_ID_KRAIT_L2,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	ARM_NUM_PMU_IDS,
};

extern enum arm_perf_pmu_ids
armpmu_get_pmu_id(void);

<<<<<<< HEAD
extern int
armpmu_get_max_events(void);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
/*
 * The ARMv7 CPU PMU supports up to 32 event counters.
 */
#define ARMPMU_MAX_HWEVENTS		32

#define HW_OP_UNSUPPORTED		0xFFFF
#define C(_x)				PERF_COUNT_HW_CACHE_##_x
#define CACHE_OP_UNSUPPORTED		0xFFFF

#ifdef CONFIG_HW_PERF_EVENTS
struct pt_regs;
extern unsigned long perf_instruction_pointer(struct pt_regs *regs);
extern unsigned long perf_misc_flags(struct pt_regs *regs);
#define perf_misc_flags(regs)	perf_misc_flags(regs)
#endif

>>>>>>> refs/remotes/origin/master
#endif /* __ARM_PERF_EVENT_H__ */
