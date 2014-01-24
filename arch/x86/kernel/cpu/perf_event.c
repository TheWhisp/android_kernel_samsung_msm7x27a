/*
 * Performance events x86 architecture code
 *
 *  Copyright (C) 2008 Thomas Gleixner <tglx@linutronix.de>
 *  Copyright (C) 2008-2009 Red Hat, Inc., Ingo Molnar
 *  Copyright (C) 2009 Jaswinder Singh Rajput
 *  Copyright (C) 2009 Advanced Micro Devices, Inc., Robert Richter
 *  Copyright (C) 2008-2009 Red Hat, Inc., Peter Zijlstra <pzijlstr@redhat.com>
 *  Copyright (C) 2009 Intel Corporation, <markus.t.metzger@intel.com>
 *  Copyright (C) 2009 Google, Inc., Stephane Eranian
 *
 *  For licencing details see kernel-base/COPYING
 */

#include <linux/perf_event.h>
#include <linux/capability.h>
#include <linux/notifier.h>
#include <linux/hardirq.h>
#include <linux/kprobes.h>
#include <linux/module.h>
#include <linux/kdebug.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/highmem.h>
#include <linux/cpu.h>
#include <linux/bitops.h>
=======
#include <linux/cpu.h>
#include <linux/bitops.h>
#include <linux/device.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/cpu.h>
#include <linux/bitops.h>
#include <linux/device.h>
>>>>>>> refs/remotes/origin/master

#include <asm/apic.h>
#include <asm/stacktrace.h>
#include <asm/nmi.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/compat.h>
#include <asm/smp.h>
#include <asm/alternative.h>
=======
#include <asm/smp.h>
#include <asm/alternative.h>
#include <asm/timer.h>

#include "perf_event.h"
>>>>>>> refs/remotes/origin/cm-10.0

#if 0
#undef wrmsrl
#define wrmsrl(msr, val) 					\
do {								\
	trace_printk("wrmsrl(%lx, %lx)\n", (unsigned long)(msr),\
			(unsigned long)(val));			\
	native_write_msr((msr), (u32)((u64)(val)), 		\
			(u32)((u64)(val) >> 32));		\
} while (0)
#endif

<<<<<<< HEAD
/*
 * best effort, GUP based copy_from_user() that assumes IRQ or NMI context
 */
static unsigned long
copy_from_user_nmi(void *to, const void __user *from, unsigned long n)
{
	unsigned long offset, addr = (unsigned long)from;
	unsigned long size, len = 0;
	struct page *page;
	void *map;
	int ret;

	do {
		ret = __get_user_pages_fast(addr, 1, 0, &page);
		if (!ret)
			break;

		offset = addr & (PAGE_SIZE - 1);
		size = min(PAGE_SIZE - offset, n - len);

		map = kmap_atomic(page);
		memcpy(to, map+offset, size);
		kunmap_atomic(map);
		put_page(page);

		len  += size;
		to   += size;
		addr += size;

	} while (len < n);

	return len;
}

struct event_constraint {
	union {
		unsigned long	idxmsk[BITS_TO_LONGS(X86_PMC_IDX_MAX)];
		u64		idxmsk64;
	};
	u64	code;
	u64	cmask;
	int	weight;
};

struct amd_nb {
	int nb_id;  /* NorthBridge id */
	int refcnt; /* reference count */
	struct perf_event *owners[X86_PMC_IDX_MAX];
	struct event_constraint event_constraints[X86_PMC_IDX_MAX];
};

struct intel_percore;

#define MAX_LBR_ENTRIES		16

struct cpu_hw_events {
	/*
	 * Generic x86 PMC bits
	 */
	struct perf_event	*events[X86_PMC_IDX_MAX]; /* in counter order */
	unsigned long		active_mask[BITS_TO_LONGS(X86_PMC_IDX_MAX)];
	unsigned long		running[BITS_TO_LONGS(X86_PMC_IDX_MAX)];
	int			enabled;

	int			n_events;
	int			n_added;
	int			n_txn;
	int			assign[X86_PMC_IDX_MAX]; /* event to counter assignment */
	u64			tags[X86_PMC_IDX_MAX];
	struct perf_event	*event_list[X86_PMC_IDX_MAX]; /* in enabled order */

	unsigned int		group_flag;

	/*
	 * Intel DebugStore bits
	 */
	struct debug_store	*ds;
	u64			pebs_enabled;

	/*
	 * Intel LBR bits
	 */
	int				lbr_users;
	void				*lbr_context;
	struct perf_branch_stack	lbr_stack;
	struct perf_branch_entry	lbr_entries[MAX_LBR_ENTRIES];

	/*
	 * Intel percore register state.
	 * Coordinate shared resources between HT threads.
	 */
	int				percore_used; /* Used by this CPU? */
	struct intel_percore		*per_core;

	/*
	 * AMD specific bits
	 */
	struct amd_nb		*amd_nb;
};

#define __EVENT_CONSTRAINT(c, n, m, w) {\
	{ .idxmsk64 = (n) },		\
	.code = (c),			\
	.cmask = (m),			\
	.weight = (w),			\
}

#define EVENT_CONSTRAINT(c, n, m)	\
	__EVENT_CONSTRAINT(c, n, m, HWEIGHT(n))

/*
 * Constraint on the Event code.
 */
#define INTEL_EVENT_CONSTRAINT(c, n)	\
	EVENT_CONSTRAINT(c, n, ARCH_PERFMON_EVENTSEL_EVENT)

/*
 * Constraint on the Event code + UMask + fixed-mask
 *
 * filter mask to validate fixed counter events.
 * the following filters disqualify for fixed counters:
 *  - inv
 *  - edge
 *  - cnt-mask
 *  The other filters are supported by fixed counters.
 *  The any-thread option is supported starting with v3.
 */
#define FIXED_EVENT_CONSTRAINT(c, n)	\
	EVENT_CONSTRAINT(c, (1ULL << (32+n)), X86_RAW_EVENT_MASK)

/*
 * Constraint on the Event code + UMask
 */
#define INTEL_UEVENT_CONSTRAINT(c, n)	\
	EVENT_CONSTRAINT(c, n, INTEL_ARCH_EVENT_MASK)

#define EVENT_CONSTRAINT_END		\
	EVENT_CONSTRAINT(0, 0, 0)

#define for_each_event_constraint(e, c)	\
	for ((e) = (c); (e)->weight; (e)++)

/*
 * Extra registers for specific events.
 * Some events need large masks and require external MSRs.
 * Define a mapping to these extra registers.
 */
struct extra_reg {
	unsigned int		event;
	unsigned int		msr;
	u64			config_mask;
	u64			valid_mask;
};

#define EVENT_EXTRA_REG(e, ms, m, vm) {	\
	.event = (e),		\
	.msr = (ms),		\
	.config_mask = (m),	\
	.valid_mask = (vm),	\
	}
#define INTEL_EVENT_EXTRA_REG(event, msr, vm)	\
	EVENT_EXTRA_REG(event, msr, ARCH_PERFMON_EVENTSEL_EVENT, vm)
#define EVENT_EXTRA_END EVENT_EXTRA_REG(0, 0, 0, 0)

union perf_capabilities {
	struct {
		u64	lbr_format    : 6;
		u64	pebs_trap     : 1;
		u64	pebs_arch_reg : 1;
		u64	pebs_format   : 4;
		u64	smm_freeze    : 1;
	};
	u64	capabilities;
};

/*
 * struct x86_pmu - generic x86 pmu
 */
struct x86_pmu {
	/*
	 * Generic x86 PMC bits
	 */
	const char	*name;
	int		version;
	int		(*handle_irq)(struct pt_regs *);
	void		(*disable_all)(void);
	void		(*enable_all)(int added);
	void		(*enable)(struct perf_event *);
	void		(*disable)(struct perf_event *);
	int		(*hw_config)(struct perf_event *event);
	int		(*schedule_events)(struct cpu_hw_events *cpuc, int n, int *assign);
	unsigned	eventsel;
	unsigned	perfctr;
	u64		(*event_map)(int);
	int		max_events;
	int		num_counters;
	int		num_counters_fixed;
	int		cntval_bits;
	u64		cntval_mask;
	int		apic;
	u64		max_period;
	struct event_constraint *
			(*get_event_constraints)(struct cpu_hw_events *cpuc,
						 struct perf_event *event);

	void		(*put_event_constraints)(struct cpu_hw_events *cpuc,
						 struct perf_event *event);
	struct event_constraint *event_constraints;
	struct event_constraint *percore_constraints;
	void		(*quirks)(void);
	int		perfctr_second_write;

	int		(*cpu_prepare)(int cpu);
	void		(*cpu_starting)(int cpu);
	void		(*cpu_dying)(int cpu);
	void		(*cpu_dead)(int cpu);

	/*
	 * Intel Arch Perfmon v2+
	 */
	u64			intel_ctrl;
	union perf_capabilities intel_cap;

	/*
	 * Intel DebugStore bits
	 */
	int		bts, pebs;
	int		bts_active, pebs_active;
	int		pebs_record_size;
	void		(*drain_pebs)(struct pt_regs *regs);
	struct event_constraint *pebs_constraints;

	/*
	 * Intel LBR
	 */
	unsigned long	lbr_tos, lbr_from, lbr_to; /* MSR base regs       */
	int		lbr_nr;			   /* hardware stack size */

	/*
	 * Extra registers for events
	 */
	struct extra_reg *extra_regs;
};

static struct x86_pmu x86_pmu __read_mostly;

static DEFINE_PER_CPU(struct cpu_hw_events, cpu_hw_events) = {
	.enabled = 1,
};

static int x86_perf_event_set_period(struct perf_event *event);

/*
 * Generalized hw caching related hw_event table, filled
 * in on a per model basis. A value of 0 means
 * 'not supported', -1 means 'hw_event makes no sense on
 * this CPU', any other value means the raw hw_event
 * ID.
 */

#define C(x) PERF_COUNT_HW_CACHE_##x

static u64 __read_mostly hw_cache_event_ids
				[PERF_COUNT_HW_CACHE_MAX]
				[PERF_COUNT_HW_CACHE_OP_MAX]
				[PERF_COUNT_HW_CACHE_RESULT_MAX];
static u64 __read_mostly hw_cache_extra_regs
=======
=======
#include <asm/smp.h>
#include <asm/alternative.h>
#include <asm/timer.h>
#include <asm/desc.h>
#include <asm/ldt.h>

#include "perf_event.h"

>>>>>>> refs/remotes/origin/master
struct x86_pmu x86_pmu __read_mostly;

DEFINE_PER_CPU(struct cpu_hw_events, cpu_hw_events) = {
	.enabled = 1,
};

u64 __read_mostly hw_cache_event_ids
				[PERF_COUNT_HW_CACHE_MAX]
				[PERF_COUNT_HW_CACHE_OP_MAX]
				[PERF_COUNT_HW_CACHE_RESULT_MAX];
u64 __read_mostly hw_cache_extra_regs
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
				[PERF_COUNT_HW_CACHE_MAX]
				[PERF_COUNT_HW_CACHE_OP_MAX]
				[PERF_COUNT_HW_CACHE_RESULT_MAX];

/*
 * Propagate event elapsed time into the generic event.
 * Can only be executed on the CPU where the event is active.
 * Returns the delta events processed.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static u64
x86_perf_event_update(struct perf_event *event)
=======
u64 x86_perf_event_update(struct perf_event *event)
>>>>>>> refs/remotes/origin/cm-10.0
=======
u64 x86_perf_event_update(struct perf_event *event)
>>>>>>> refs/remotes/origin/master
{
	struct hw_perf_event *hwc = &event->hw;
	int shift = 64 - x86_pmu.cntval_bits;
	u64 prev_raw_count, new_raw_count;
	int idx = hwc->idx;
	s64 delta;

<<<<<<< HEAD
	if (idx == X86_PMC_IDX_FIXED_BTS)
=======
	if (idx == INTEL_PMC_IDX_FIXED_BTS)
>>>>>>> refs/remotes/origin/master
		return 0;

	/*
	 * Careful: an NMI might modify the previous event value.
	 *
	 * Our tactic to handle this is to first atomically read and
	 * exchange a new raw count - then add that new-prev delta
	 * count to the generic event atomically:
	 */
again:
	prev_raw_count = local64_read(&hwc->prev_count);
<<<<<<< HEAD
	rdmsrl(hwc->event_base, new_raw_count);
=======
	rdpmcl(hwc->event_base_rdpmc, new_raw_count);
>>>>>>> refs/remotes/origin/master

	if (local64_cmpxchg(&hwc->prev_count, prev_raw_count,
					new_raw_count) != prev_raw_count)
		goto again;

	/*
	 * Now we have the new raw value and have updated the prev
	 * timestamp already. We can now calculate the elapsed delta
	 * (event-)time and add that to the generic event.
	 *
	 * Careful, not all hw sign-extends above the physical width
	 * of the count.
	 */
	delta = (new_raw_count << shift) - (prev_raw_count << shift);
	delta >>= shift;

	local64_add(delta, &event->count);
	local64_sub(delta, &hwc->period_left);

	return new_raw_count;
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline int x86_pmu_addr_offset(int index)
{
	int offset;

	/* offset = X86_FEATURE_PERFCTR_CORE ? index << 1 : index */
	alternative_io(ASM_NOP2,
		       "shll $1, %%eax",
		       X86_FEATURE_PERFCTR_CORE,
		       "=a" (offset),
		       "a"  (index));

	return offset;
}

static inline unsigned int x86_pmu_config_addr(int index)
{
	return x86_pmu.eventsel + x86_pmu_addr_offset(index);
}

static inline unsigned int x86_pmu_event_addr(int index)
{
	return x86_pmu.perfctr + x86_pmu_addr_offset(index);
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * Find and validate any extra registers to set up.
 */
static int x86_pmu_extra_regs(u64 config, struct perf_event *event)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct extra_reg *er;

	event->hw.extra_reg = 0;
	event->hw.extra_config = 0;
=======
=======
>>>>>>> refs/remotes/origin/master
	struct hw_perf_event_extra *reg;
	struct extra_reg *er;

	reg = &event->hw.extra_reg;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (!x86_pmu.extra_regs)
		return 0;

	for (er = x86_pmu.extra_regs; er->msr; er++) {
		if (er->event != (config & er->config_mask))
			continue;
		if (event->attr.config1 & ~er->valid_mask)
			return -EINVAL;
<<<<<<< HEAD
<<<<<<< HEAD
		event->hw.extra_reg = er->msr;
		event->hw.extra_config = event->attr.config1;
=======
=======
>>>>>>> refs/remotes/origin/master

		reg->idx = er->idx;
		reg->config = event->attr.config1;
		reg->reg = er->msr;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		break;
	}
	return 0;
}

static atomic_t active_events;
static DEFINE_MUTEX(pmc_reserve_mutex);

#ifdef CONFIG_X86_LOCAL_APIC

static bool reserve_pmc_hardware(void)
{
	int i;

	for (i = 0; i < x86_pmu.num_counters; i++) {
		if (!reserve_perfctr_nmi(x86_pmu_event_addr(i)))
			goto perfctr_fail;
	}

	for (i = 0; i < x86_pmu.num_counters; i++) {
		if (!reserve_evntsel_nmi(x86_pmu_config_addr(i)))
			goto eventsel_fail;
	}

	return true;

eventsel_fail:
	for (i--; i >= 0; i--)
		release_evntsel_nmi(x86_pmu_config_addr(i));

	i = x86_pmu.num_counters;

perfctr_fail:
	for (i--; i >= 0; i--)
		release_perfctr_nmi(x86_pmu_event_addr(i));

	return false;
}

static void release_pmc_hardware(void)
{
	int i;

	for (i = 0; i < x86_pmu.num_counters; i++) {
		release_perfctr_nmi(x86_pmu_event_addr(i));
		release_evntsel_nmi(x86_pmu_config_addr(i));
	}
}

#else

static bool reserve_pmc_hardware(void) { return true; }
static void release_pmc_hardware(void) {}

#endif

static bool check_hw_exists(void)
{
<<<<<<< HEAD
	u64 val, val_new = 0;
	int i, reg, ret = 0;
=======
	u64 val, val_fail, val_new= ~0;
	int i, reg, reg_fail, ret = 0;
	int bios_fail = 0;
>>>>>>> refs/remotes/origin/master

	/*
	 * Check to see if the BIOS enabled any of the counters, if so
	 * complain and bail.
	 */
	for (i = 0; i < x86_pmu.num_counters; i++) {
		reg = x86_pmu_config_addr(i);
		ret = rdmsrl_safe(reg, &val);
		if (ret)
			goto msr_fail;
<<<<<<< HEAD
		if (val & ARCH_PERFMON_EVENTSEL_ENABLE)
			goto bios_fail;
=======
		if (val & ARCH_PERFMON_EVENTSEL_ENABLE) {
			bios_fail = 1;
			val_fail = val;
			reg_fail = reg;
		}
>>>>>>> refs/remotes/origin/master
	}

	if (x86_pmu.num_counters_fixed) {
		reg = MSR_ARCH_PERFMON_FIXED_CTR_CTRL;
		ret = rdmsrl_safe(reg, &val);
		if (ret)
			goto msr_fail;
		for (i = 0; i < x86_pmu.num_counters_fixed; i++) {
<<<<<<< HEAD
			if (val & (0x03 << i*4))
				goto bios_fail;
=======
			if (val & (0x03 << i*4)) {
				bios_fail = 1;
				val_fail = val;
				reg_fail = reg;
			}
>>>>>>> refs/remotes/origin/master
		}
	}

	/*
<<<<<<< HEAD
	 * Now write a value and read it back to see if it matches,
	 * this is needed to detect certain hardware emulators (qemu/kvm)
	 * that don't trap on the MSR access and always return 0s.
	 */
	val = 0xabcdUL;
	ret = checking_wrmsrl(x86_pmu_event_addr(0), val);
	ret |= rdmsrl_safe(x86_pmu_event_addr(0), &val_new);
	if (ret || val != val_new)
		goto msr_fail;

	return true;

bios_fail:
	/*
	 * We still allow the PMU driver to operate:
	 */
	printk(KERN_CONT "Broken BIOS detected, complain to your hardware vendor.\n");
	printk(KERN_ERR FW_BUG "the BIOS has corrupted hw-PMU resources (MSR %x is %Lx)\n", reg, val);
=======
	 * Read the current value, change it and read it back to see if it
	 * matches, this is needed to detect certain hardware emulators
	 * (qemu/kvm) that don't trap on the MSR access and always return 0s.
	 */
	reg = x86_pmu_event_addr(0);
	if (rdmsrl_safe(reg, &val))
		goto msr_fail;
	val ^= 0xffffUL;
	ret = wrmsrl_safe(reg, val);
	ret |= rdmsrl_safe(reg, &val_new);
	if (ret || val != val_new)
		goto msr_fail;

	/*
	 * We still allow the PMU driver to operate:
	 */
	if (bios_fail) {
		printk(KERN_CONT "Broken BIOS detected, complain to your hardware vendor.\n");
		printk(KERN_ERR FW_BUG "the BIOS has corrupted hw-PMU resources (MSR %x is %Lx)\n", reg_fail, val_fail);
	}
>>>>>>> refs/remotes/origin/master

	return true;

msr_fail:
	printk(KERN_CONT "Broken PMU hardware detected, using software events only.\n");
<<<<<<< HEAD
=======
	printk(KERN_ERR "Failed to access perfctr msr (MSR %x is %Lx)\n", reg, val_new);
>>>>>>> refs/remotes/origin/master

	return false;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void reserve_ds_buffers(void);
static void release_ds_buffers(void);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static void hw_perf_event_destroy(struct perf_event *event)
{
	if (atomic_dec_and_mutex_lock(&active_events, &pmc_reserve_mutex)) {
		release_pmc_hardware();
		release_ds_buffers();
		mutex_unlock(&pmc_reserve_mutex);
	}
}

static inline int x86_pmu_initialized(void)
{
	return x86_pmu.handle_irq != NULL;
}

static inline int
set_ext_hw_attr(struct hw_perf_event *hwc, struct perf_event *event)
{
	struct perf_event_attr *attr = &event->attr;
	unsigned int cache_type, cache_op, cache_result;
	u64 config, val;

	config = attr->config;

	cache_type = (config >>  0) & 0xff;
	if (cache_type >= PERF_COUNT_HW_CACHE_MAX)
		return -EINVAL;

	cache_op = (config >>  8) & 0xff;
	if (cache_op >= PERF_COUNT_HW_CACHE_OP_MAX)
		return -EINVAL;

	cache_result = (config >> 16) & 0xff;
	if (cache_result >= PERF_COUNT_HW_CACHE_RESULT_MAX)
		return -EINVAL;

	val = hw_cache_event_ids[cache_type][cache_op][cache_result];

	if (val == 0)
		return -ENOENT;

	if (val == -1)
		return -EINVAL;

	hwc->config |= val;
	attr->config1 = hw_cache_extra_regs[cache_type][cache_op][cache_result];
	return x86_pmu_extra_regs(val, event);
}

<<<<<<< HEAD
<<<<<<< HEAD
static int x86_setup_perfctr(struct perf_event *event)
=======
int x86_setup_perfctr(struct perf_event *event)
>>>>>>> refs/remotes/origin/cm-10.0
=======
int x86_setup_perfctr(struct perf_event *event)
>>>>>>> refs/remotes/origin/master
{
	struct perf_event_attr *attr = &event->attr;
	struct hw_perf_event *hwc = &event->hw;
	u64 config;

	if (!is_sampling_event(event)) {
		hwc->sample_period = x86_pmu.max_period;
		hwc->last_period = hwc->sample_period;
		local64_set(&hwc->period_left, hwc->sample_period);
	} else {
		/*
		 * If we have a PMU initialized but no APIC
		 * interrupts, we cannot sample hardware
		 * events (user-space has to fall back and
		 * sample via a hrtimer based software event):
		 */
		if (!x86_pmu.apic)
			return -EOPNOTSUPP;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	/*
	 * Do not allow config1 (extended registers) to propagate,
	 * there's no sane user-space generalization yet:
	 */
	if (attr->type == PERF_TYPE_RAW)
		return 0;
=======
	if (attr->type == PERF_TYPE_RAW)
		return x86_pmu_extra_regs(event->attr.config, event);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (attr->type == PERF_TYPE_RAW)
		return x86_pmu_extra_regs(event->attr.config, event);
>>>>>>> refs/remotes/origin/master

	if (attr->type == PERF_TYPE_HW_CACHE)
		return set_ext_hw_attr(hwc, event);

	if (attr->config >= x86_pmu.max_events)
		return -EINVAL;

	/*
	 * The generic map:
	 */
	config = x86_pmu.event_map(attr->config);

	if (config == 0)
		return -ENOENT;

	if (config == -1LL)
		return -EINVAL;

	/*
	 * Branch tracing:
	 */
	if (attr->config == PERF_COUNT_HW_BRANCH_INSTRUCTIONS &&
	    !attr->freq && hwc->sample_period == 1) {
		/* BTS is not supported by this architecture. */
		if (!x86_pmu.bts_active)
			return -EOPNOTSUPP;

		/* BTS is currently only allowed for user-mode. */
		if (!attr->exclude_kernel)
			return -EOPNOTSUPP;
	}

	hwc->config |= config;

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int x86_pmu_hw_config(struct perf_event *event)
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * check that branch_sample_type is compatible with
 * settings needed for precise_ip > 1 which implies
 * using the LBR to capture ALL taken branches at the
 * priv levels of the measurement
 */
static inline int precise_br_compat(struct perf_event *event)
{
	u64 m = event->attr.branch_sample_type;
	u64 b = 0;

	/* must capture all branches */
	if (!(m & PERF_SAMPLE_BRANCH_ANY))
		return 0;

	m &= PERF_SAMPLE_BRANCH_KERNEL | PERF_SAMPLE_BRANCH_USER;

	if (!event->attr.exclude_user)
		b |= PERF_SAMPLE_BRANCH_USER;

	if (!event->attr.exclude_kernel)
		b |= PERF_SAMPLE_BRANCH_KERNEL;

	/*
	 * ignore PERF_SAMPLE_BRANCH_HV, not supported on x86
	 */

	return m == b;
}

int x86_pmu_hw_config(struct perf_event *event)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	if (event->attr.precise_ip) {
		int precise = 0;

		/* Support for constant skid */
<<<<<<< HEAD
		if (x86_pmu.pebs_active) {
=======
		if (x86_pmu.pebs_active && !x86_pmu.pebs_broken) {
>>>>>>> refs/remotes/origin/master
			precise++;

			/* Support for IP fixup */
			if (x86_pmu.lbr_nr)
				precise++;
		}

		if (event->attr.precise_ip > precise)
			return -EOPNOTSUPP;
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
		/*
		 * check that PEBS LBR correction does not conflict with
		 * whatever the user is asking with attr->branch_sample_type
		 */
<<<<<<< HEAD
		if (event->attr.precise_ip > 1) {
=======
		if (event->attr.precise_ip > 1 &&
		    x86_pmu.intel_cap.pebs_format < 2) {
>>>>>>> refs/remotes/origin/master
			u64 *br_type = &event->attr.branch_sample_type;

			if (has_branch_stack(event)) {
				if (!precise_br_compat(event))
					return -EOPNOTSUPP;

				/* branch_sample_type is compatible */

			} else {
				/*
				 * user did not specify  branch_sample_type
				 *
				 * For PEBS fixups, we capture all
				 * the branches at the priv level of the
				 * event.
				 */
				*br_type = PERF_SAMPLE_BRANCH_ANY;

				if (!event->attr.exclude_user)
					*br_type |= PERF_SAMPLE_BRANCH_USER;

				if (!event->attr.exclude_kernel)
					*br_type |= PERF_SAMPLE_BRANCH_KERNEL;
			}
		}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * Generate PMC IRQs:
	 * (keep 'enabled' bit clear for now)
	 */
	event->hw.config = ARCH_PERFMON_EVENTSEL_INT;

	/*
	 * Count user and OS events unless requested not to
	 */
	if (!event->attr.exclude_user)
		event->hw.config |= ARCH_PERFMON_EVENTSEL_USR;
	if (!event->attr.exclude_kernel)
		event->hw.config |= ARCH_PERFMON_EVENTSEL_OS;

	if (event->attr.type == PERF_TYPE_RAW)
		event->hw.config |= event->attr.config & X86_RAW_EVENT_MASK;

	return x86_setup_perfctr(event);
}

/*
 * Setup the hardware configuration for a given attr_type
 */
static int __x86_pmu_event_init(struct perf_event *event)
{
	int err;

	if (!x86_pmu_initialized())
		return -ENODEV;

	err = 0;
	if (!atomic_inc_not_zero(&active_events)) {
		mutex_lock(&pmc_reserve_mutex);
		if (atomic_read(&active_events) == 0) {
			if (!reserve_pmc_hardware())
				err = -EBUSY;
			else
				reserve_ds_buffers();
		}
		if (!err)
			atomic_inc(&active_events);
		mutex_unlock(&pmc_reserve_mutex);
	}
	if (err)
		return err;

	event->destroy = hw_perf_event_destroy;

	event->hw.idx = -1;
	event->hw.last_cpu = -1;
	event->hw.last_tag = ~0ULL;

<<<<<<< HEAD
<<<<<<< HEAD
	return x86_pmu.hw_config(event);
}

static void x86_pmu_disable_all(void)
=======
	/* mark unused */
	event->hw.extra_reg.idx = EXTRA_REG_NONE;

	/* mark not used */
	event->hw.extra_reg.idx = EXTRA_REG_NONE;
=======
	/* mark unused */
	event->hw.extra_reg.idx = EXTRA_REG_NONE;
>>>>>>> refs/remotes/origin/master
	event->hw.branch_reg.idx = EXTRA_REG_NONE;

	return x86_pmu.hw_config(event);
}

void x86_pmu_disable_all(void)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	struct cpu_hw_events *cpuc = &__get_cpu_var(cpu_hw_events);
	int idx;

	for (idx = 0; idx < x86_pmu.num_counters; idx++) {
		u64 val;

		if (!test_bit(idx, cpuc->active_mask))
			continue;
		rdmsrl(x86_pmu_config_addr(idx), val);
		if (!(val & ARCH_PERFMON_EVENTSEL_ENABLE))
			continue;
		val &= ~ARCH_PERFMON_EVENTSEL_ENABLE;
		wrmsrl(x86_pmu_config_addr(idx), val);
	}
}

static void x86_pmu_disable(struct pmu *pmu)
{
	struct cpu_hw_events *cpuc = &__get_cpu_var(cpu_hw_events);

	if (!x86_pmu_initialized())
		return;

	if (!cpuc->enabled)
		return;

	cpuc->n_added = 0;
	cpuc->enabled = 0;
	barrier();

	x86_pmu.disable_all();
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline void __x86_pmu_enable_event(struct hw_perf_event *hwc,
					  u64 enable_mask)
{
	if (hwc->extra_reg)
		wrmsrl(hwc->extra_reg, hwc->extra_config);
	wrmsrl(hwc->config_base, hwc->config | enable_mask);
}

static void x86_pmu_enable_all(int added)
=======
void x86_pmu_enable_all(int added)
>>>>>>> refs/remotes/origin/cm-10.0
=======
void x86_pmu_enable_all(int added)
>>>>>>> refs/remotes/origin/master
{
	struct cpu_hw_events *cpuc = &__get_cpu_var(cpu_hw_events);
	int idx;

	for (idx = 0; idx < x86_pmu.num_counters; idx++) {
		struct hw_perf_event *hwc = &cpuc->events[idx]->hw;

		if (!test_bit(idx, cpuc->active_mask))
			continue;

		__x86_pmu_enable_event(hwc, ARCH_PERFMON_EVENTSEL_ENABLE);
	}
}

static struct pmu pmu;

static inline int is_x86_event(struct perf_event *event)
{
	return event->pmu == &pmu;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int x86_schedule_events(struct cpu_hw_events *cpuc, int n, int *assign)
{
	struct event_constraint *c, *constraints[X86_PMC_IDX_MAX];
	unsigned long used_mask[BITS_TO_LONGS(X86_PMC_IDX_MAX)];
	int i, j, w, wmax, num = 0;
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * Event scheduler state:
 *
 * Assign events iterating over all events and counters, beginning
 * with events with least weights first. Keep the current iterator
 * state in struct sched_state.
 */
struct sched_state {
	int	weight;
	int	event;		/* event index */
	int	counter;	/* counter index */
	int	unassigned;	/* number of events to be assigned left */
	unsigned long used[BITS_TO_LONGS(X86_PMC_IDX_MAX)];
};

/* Total max is X86_PMC_IDX_MAX, but we are O(n!) limited */
#define	SCHED_STATES_MAX	2

struct perf_sched {
	int			max_weight;
	int			max_events;
<<<<<<< HEAD
	struct event_constraint	**constraints;
=======
	struct perf_event	**events;
>>>>>>> refs/remotes/origin/master
	struct sched_state	state;
	int			saved_states;
	struct sched_state	saved[SCHED_STATES_MAX];
};

/*
 * Initialize interator that runs through all events and counters.
 */
<<<<<<< HEAD
static void perf_sched_init(struct perf_sched *sched, struct event_constraint **c,
=======
static void perf_sched_init(struct perf_sched *sched, struct perf_event **events,
>>>>>>> refs/remotes/origin/master
			    int num, int wmin, int wmax)
{
	int idx;

	memset(sched, 0, sizeof(*sched));
	sched->max_events	= num;
	sched->max_weight	= wmax;
<<<<<<< HEAD
	sched->constraints	= c;

	for (idx = 0; idx < num; idx++) {
		if (c[idx]->weight == wmin)
=======
	sched->events		= events;

	for (idx = 0; idx < num; idx++) {
		if (events[idx]->hw.constraint->weight == wmin)
>>>>>>> refs/remotes/origin/master
			break;
	}

	sched->state.event	= idx;		/* start with min weight */
	sched->state.weight	= wmin;
	sched->state.unassigned	= num;
}

static void perf_sched_save_state(struct perf_sched *sched)
{
	if (WARN_ON_ONCE(sched->saved_states >= SCHED_STATES_MAX))
		return;

	sched->saved[sched->saved_states] = sched->state;
	sched->saved_states++;
}

static bool perf_sched_restore_state(struct perf_sched *sched)
{
	if (!sched->saved_states)
		return false;

	sched->saved_states--;
	sched->state = sched->saved[sched->saved_states];

	/* continue with next counter: */
	clear_bit(sched->state.counter++, sched->state.used);

	return true;
}

/*
 * Select a counter for the current event to schedule. Return true on
 * success.
 */
static bool __perf_sched_find_counter(struct perf_sched *sched)
{
	struct event_constraint *c;
	int idx;

	if (!sched->state.unassigned)
		return false;

	if (sched->state.event >= sched->max_events)
		return false;

<<<<<<< HEAD
	c = sched->constraints[sched->state.event];

	/* Prefer fixed purpose counters */
	if (x86_pmu.num_counters_fixed) {
		idx = X86_PMC_IDX_FIXED;
=======
	c = sched->events[sched->state.event]->hw.constraint;
	/* Prefer fixed purpose counters */
	if (c->idxmsk64 & (~0ULL << INTEL_PMC_IDX_FIXED)) {
		idx = INTEL_PMC_IDX_FIXED;
>>>>>>> refs/remotes/origin/master
		for_each_set_bit_from(idx, c->idxmsk, X86_PMC_IDX_MAX) {
			if (!__test_and_set_bit(idx, sched->state.used))
				goto done;
		}
	}
	/* Grab the first unused counter starting with idx */
	idx = sched->state.counter;
<<<<<<< HEAD
	for_each_set_bit_from(idx, c->idxmsk, X86_PMC_IDX_FIXED) {
=======
	for_each_set_bit_from(idx, c->idxmsk, INTEL_PMC_IDX_FIXED) {
>>>>>>> refs/remotes/origin/master
		if (!__test_and_set_bit(idx, sched->state.used))
			goto done;
	}

	return false;

done:
	sched->state.counter = idx;

	if (c->overlap)
		perf_sched_save_state(sched);

	return true;
}

static bool perf_sched_find_counter(struct perf_sched *sched)
{
	while (!__perf_sched_find_counter(sched)) {
		if (!perf_sched_restore_state(sched))
			return false;
	}

	return true;
}

/*
 * Go through all unassigned events and find the next one to schedule.
 * Take events with the least weight first. Return true on success.
 */
static bool perf_sched_next_event(struct perf_sched *sched)
{
	struct event_constraint *c;

	if (!sched->state.unassigned || !--sched->state.unassigned)
		return false;

	do {
		/* next event */
		sched->state.event++;
		if (sched->state.event >= sched->max_events) {
			/* next weight */
			sched->state.event = 0;
			sched->state.weight++;
			if (sched->state.weight > sched->max_weight)
				return false;
		}
<<<<<<< HEAD
		c = sched->constraints[sched->state.event];
=======
		c = sched->events[sched->state.event]->hw.constraint;
>>>>>>> refs/remotes/origin/master
	} while (c->weight != sched->state.weight);

	sched->state.counter = 0;	/* start with first counter */

	return true;
}

/*
 * Assign a counter for each event.
 */
<<<<<<< HEAD
static int perf_assign_events(struct event_constraint **constraints, int n,
			      int wmin, int wmax, int *assign)
{
	struct perf_sched sched;

	perf_sched_init(&sched, constraints, n, wmin, wmax);
=======
int perf_assign_events(struct perf_event **events, int n,
			int wmin, int wmax, int *assign)
{
	struct perf_sched sched;

	perf_sched_init(&sched, events, n, wmin, wmax);
>>>>>>> refs/remotes/origin/master

	do {
		if (!perf_sched_find_counter(&sched))
			break;	/* failed */
		if (assign)
			assign[sched.state.event] = sched.state.counter;
	} while (perf_sched_next_event(&sched));

	return sched.state.unassigned;
}

int x86_schedule_events(struct cpu_hw_events *cpuc, int n, int *assign)
{
<<<<<<< HEAD
	struct event_constraint *c, *constraints[X86_PMC_IDX_MAX];
	unsigned long used_mask[BITS_TO_LONGS(X86_PMC_IDX_MAX)];
	int i, wmin, wmax, num = 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct event_constraint *c;
	unsigned long used_mask[BITS_TO_LONGS(X86_PMC_IDX_MAX)];
	struct perf_event *e;
	int i, wmin, wmax, num = 0;
>>>>>>> refs/remotes/origin/master
	struct hw_perf_event *hwc;

	bitmap_zero(used_mask, X86_PMC_IDX_MAX);

<<<<<<< HEAD
<<<<<<< HEAD
	for (i = 0; i < n; i++) {
		c = x86_pmu.get_event_constraints(cpuc, cpuc->event_list[i]);
		constraints[i] = c;
=======
	for (i = 0, wmin = X86_PMC_IDX_MAX, wmax = 0; i < n; i++) {
		c = x86_pmu.get_event_constraints(cpuc, cpuc->event_list[i]);
		constraints[i] = c;
		wmin = min(wmin, c->weight);
		wmax = max(wmax, c->weight);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	for (i = 0, wmin = X86_PMC_IDX_MAX, wmax = 0; i < n; i++) {
		hwc = &cpuc->event_list[i]->hw;
		c = x86_pmu.get_event_constraints(cpuc, cpuc->event_list[i]);
		hwc->constraint = c;

		wmin = min(wmin, c->weight);
		wmax = max(wmax, c->weight);
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * fastpath, try to reuse previous register
	 */
	for (i = 0; i < n; i++) {
		hwc = &cpuc->event_list[i]->hw;
<<<<<<< HEAD
		c = constraints[i];
=======
		c = hwc->constraint;
>>>>>>> refs/remotes/origin/master

		/* never assigned */
		if (hwc->idx == -1)
			break;

		/* constraint still honored */
		if (!test_bit(hwc->idx, c->idxmsk))
			break;

		/* not already used */
		if (test_bit(hwc->idx, used_mask))
			break;

		__set_bit(hwc->idx, used_mask);
		if (assign)
			assign[i] = hwc->idx;
	}
<<<<<<< HEAD
<<<<<<< HEAD
	if (i == n)
		goto done;

	/*
	 * begin slow path
	 */

	bitmap_zero(used_mask, X86_PMC_IDX_MAX);

	/*
	 * weight = number of possible counters
	 *
	 * 1    = most constrained, only works on one counter
	 * wmax = least constrained, works on any counter
	 *
	 * assign events to counters starting with most
	 * constrained events.
	 */
	wmax = x86_pmu.num_counters;

	/*
	 * when fixed event counters are present,
	 * wmax is incremented by 1 to account
	 * for one more choice
	 */
	if (x86_pmu.num_counters_fixed)
		wmax++;

	for (w = 1, num = n; num && w <= wmax; w++) {
		/* for each event */
		for (i = 0; num && i < n; i++) {
			c = constraints[i];
			hwc = &cpuc->event_list[i]->hw;

			if (c->weight != w)
				continue;

			for_each_set_bit(j, c->idxmsk, X86_PMC_IDX_MAX) {
				if (!test_bit(j, used_mask))
					break;
			}

			if (j == X86_PMC_IDX_MAX)
				break;

			__set_bit(j, used_mask);

			if (assign)
				assign[i] = j;
			num--;
		}
	}
done:
	/*
=======

	/* slow path */
	if (i != n)
		num = perf_assign_events(constraints, n, wmin, wmax, assign);

	/*
>>>>>>> refs/remotes/origin/cm-10.0
=======

	/* slow path */
	if (i != n)
		num = perf_assign_events(cpuc->event_list, n, wmin,
					 wmax, assign);

	/*
	 * Mark the event as committed, so we do not put_constraint()
	 * in case new events are added and fail scheduling.
	 */
	if (!num && assign) {
		for (i = 0; i < n; i++) {
			e = cpuc->event_list[i];
			e->hw.flags |= PERF_X86_EVENT_COMMITTED;
		}
	}
	/*
>>>>>>> refs/remotes/origin/master
	 * scheduling failed or is just a simulation,
	 * free resources if necessary
	 */
	if (!assign || num) {
		for (i = 0; i < n; i++) {
<<<<<<< HEAD
			if (x86_pmu.put_event_constraints)
				x86_pmu.put_event_constraints(cpuc, cpuc->event_list[i]);
		}
	}
<<<<<<< HEAD
	return num ? -ENOSPC : 0;
=======
	return num ? -EINVAL : 0;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			e = cpuc->event_list[i];
			/*
			 * do not put_constraint() on comitted events,
			 * because they are good to go
			 */
			if ((e->hw.flags & PERF_X86_EVENT_COMMITTED))
				continue;

			if (x86_pmu.put_event_constraints)
				x86_pmu.put_event_constraints(cpuc, e);
		}
	}
	return num ? -EINVAL : 0;
>>>>>>> refs/remotes/origin/master
}

/*
 * dogrp: true if must collect siblings events (group)
 * returns total number of events and error code
 */
static int collect_events(struct cpu_hw_events *cpuc, struct perf_event *leader, bool dogrp)
{
	struct perf_event *event;
	int n, max_count;

	max_count = x86_pmu.num_counters + x86_pmu.num_counters_fixed;

	/* current number of events already accepted */
	n = cpuc->n_events;

	if (is_x86_event(leader)) {
		if (n >= max_count)
<<<<<<< HEAD
<<<<<<< HEAD
			return -ENOSPC;
=======
			return -EINVAL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			return -EINVAL;
>>>>>>> refs/remotes/origin/master
		cpuc->event_list[n] = leader;
		n++;
	}
	if (!dogrp)
		return n;

	list_for_each_entry(event, &leader->sibling_list, group_entry) {
		if (!is_x86_event(event) ||
		    event->state <= PERF_EVENT_STATE_OFF)
			continue;

		if (n >= max_count)
<<<<<<< HEAD
<<<<<<< HEAD
			return -ENOSPC;
=======
			return -EINVAL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			return -EINVAL;
>>>>>>> refs/remotes/origin/master

		cpuc->event_list[n] = event;
		n++;
	}
	return n;
}

static inline void x86_assign_hw_event(struct perf_event *event,
				struct cpu_hw_events *cpuc, int i)
{
	struct hw_perf_event *hwc = &event->hw;

	hwc->idx = cpuc->assign[i];
	hwc->last_cpu = smp_processor_id();
	hwc->last_tag = ++cpuc->tags[i];

<<<<<<< HEAD
	if (hwc->idx == X86_PMC_IDX_FIXED_BTS) {
		hwc->config_base = 0;
		hwc->event_base	= 0;
	} else if (hwc->idx >= X86_PMC_IDX_FIXED) {
		hwc->config_base = MSR_ARCH_PERFMON_FIXED_CTR_CTRL;
		hwc->event_base = MSR_ARCH_PERFMON_FIXED_CTR0 + (hwc->idx - X86_PMC_IDX_FIXED);
	} else {
		hwc->config_base = x86_pmu_config_addr(hwc->idx);
		hwc->event_base  = x86_pmu_event_addr(hwc->idx);
=======
	if (hwc->idx == INTEL_PMC_IDX_FIXED_BTS) {
		hwc->config_base = 0;
		hwc->event_base	= 0;
	} else if (hwc->idx >= INTEL_PMC_IDX_FIXED) {
		hwc->config_base = MSR_ARCH_PERFMON_FIXED_CTR_CTRL;
		hwc->event_base = MSR_ARCH_PERFMON_FIXED_CTR0 + (hwc->idx - INTEL_PMC_IDX_FIXED);
		hwc->event_base_rdpmc = (hwc->idx - INTEL_PMC_IDX_FIXED) | 1<<30;
	} else {
		hwc->config_base = x86_pmu_config_addr(hwc->idx);
		hwc->event_base  = x86_pmu_event_addr(hwc->idx);
		hwc->event_base_rdpmc = x86_pmu_rdpmc_index(hwc->idx);
>>>>>>> refs/remotes/origin/master
	}
}

static inline int match_prev_assignment(struct hw_perf_event *hwc,
					struct cpu_hw_events *cpuc,
					int i)
{
	return hwc->idx == cpuc->assign[i] &&
		hwc->last_cpu == smp_processor_id() &&
		hwc->last_tag == cpuc->tags[i];
}

static void x86_pmu_start(struct perf_event *event, int flags);
<<<<<<< HEAD
<<<<<<< HEAD
static void x86_pmu_stop(struct perf_event *event, int flags);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

static void x86_pmu_enable(struct pmu *pmu)
{
	struct cpu_hw_events *cpuc = &__get_cpu_var(cpu_hw_events);
	struct perf_event *event;
	struct hw_perf_event *hwc;
	int i, added = cpuc->n_added;

	if (!x86_pmu_initialized())
		return;

	if (cpuc->enabled)
		return;

	if (cpuc->n_added) {
		int n_running = cpuc->n_events - cpuc->n_added;
		/*
		 * apply assignment obtained either from
		 * hw_perf_group_sched_in() or x86_pmu_enable()
		 *
		 * step1: save events moving to new counters
		 * step2: reprogram moved events into new counters
		 */
		for (i = 0; i < n_running; i++) {
			event = cpuc->event_list[i];
			hwc = &event->hw;

			/*
			 * we can avoid reprogramming counter if:
			 * - assigned same counter as last time
			 * - running on same CPU as last time
			 * - no other event has used the counter since
			 */
			if (hwc->idx == -1 ||
			    match_prev_assignment(hwc, cpuc, i))
				continue;

			/*
			 * Ensure we don't accidentally enable a stopped
			 * counter simply because we rescheduled.
			 */
			if (hwc->state & PERF_HES_STOPPED)
				hwc->state |= PERF_HES_ARCH;

			x86_pmu_stop(event, PERF_EF_UPDATE);
		}

		for (i = 0; i < cpuc->n_events; i++) {
			event = cpuc->event_list[i];
			hwc = &event->hw;

			if (!match_prev_assignment(hwc, cpuc, i))
				x86_assign_hw_event(event, cpuc, i);
			else if (i < n_running)
				continue;

			if (hwc->state & PERF_HES_ARCH)
				continue;

			x86_pmu_start(event, PERF_EF_RELOAD);
		}
		cpuc->n_added = 0;
		perf_events_lapic_init();
	}

	cpuc->enabled = 1;
	barrier();

	x86_pmu.enable_all(added);
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline void x86_pmu_disable_event(struct perf_event *event)
{
	struct hw_perf_event *hwc = &event->hw;

	wrmsrl(hwc->config_base, hwc->config);
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static DEFINE_PER_CPU(u64 [X86_PMC_IDX_MAX], pmc_prev_left);

/*
 * Set the next IRQ period, based on the hwc->period_left value.
 * To be called with the event disabled in hw:
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int
x86_perf_event_set_period(struct perf_event *event)
=======
int x86_perf_event_set_period(struct perf_event *event)
>>>>>>> refs/remotes/origin/cm-10.0
=======
int x86_perf_event_set_period(struct perf_event *event)
>>>>>>> refs/remotes/origin/master
{
	struct hw_perf_event *hwc = &event->hw;
	s64 left = local64_read(&hwc->period_left);
	s64 period = hwc->sample_period;
	int ret = 0, idx = hwc->idx;

<<<<<<< HEAD
	if (idx == X86_PMC_IDX_FIXED_BTS)
=======
	if (idx == INTEL_PMC_IDX_FIXED_BTS)
>>>>>>> refs/remotes/origin/master
		return 0;

	/*
	 * If we are way outside a reasonable range then just skip forward:
	 */
	if (unlikely(left <= -period)) {
		left = period;
		local64_set(&hwc->period_left, left);
		hwc->last_period = period;
		ret = 1;
	}

	if (unlikely(left <= 0)) {
		left += period;
		local64_set(&hwc->period_left, left);
		hwc->last_period = period;
		ret = 1;
	}
	/*
	 * Quirk: certain CPUs dont like it if just 1 hw_event is left:
	 */
	if (unlikely(left < 2))
		left = 2;

	if (left > x86_pmu.max_period)
		left = x86_pmu.max_period;

	per_cpu(pmc_prev_left[idx], smp_processor_id()) = left;

	/*
	 * The hw event starts counting from this event offset,
	 * mark it to be able to extra future deltas:
	 */
	local64_set(&hwc->prev_count, (u64)-left);

	wrmsrl(hwc->event_base, (u64)(-left) & x86_pmu.cntval_mask);

	/*
	 * Due to erratum on certan cpu we need
	 * a second write to be sure the register
	 * is updated properly
	 */
	if (x86_pmu.perfctr_second_write) {
		wrmsrl(hwc->event_base,
			(u64)(-left) & x86_pmu.cntval_mask);
	}

	perf_event_update_userpage(event);

	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void x86_pmu_enable_event(struct perf_event *event)
=======
void x86_pmu_enable_event(struct perf_event *event)
>>>>>>> refs/remotes/origin/cm-10.0
=======
void x86_pmu_enable_event(struct perf_event *event)
>>>>>>> refs/remotes/origin/master
{
	if (__this_cpu_read(cpu_hw_events.enabled))
		__x86_pmu_enable_event(&event->hw,
				       ARCH_PERFMON_EVENTSEL_ENABLE);
}

/*
 * Add a single event to the PMU.
 *
 * The event is added to the group of enabled events
 * but only if it can be scehduled with existing events.
 */
static int x86_pmu_add(struct perf_event *event, int flags)
{
	struct cpu_hw_events *cpuc = &__get_cpu_var(cpu_hw_events);
	struct hw_perf_event *hwc;
	int assign[X86_PMC_IDX_MAX];
	int n, n0, ret;

	hwc = &event->hw;

	perf_pmu_disable(event->pmu);
	n0 = cpuc->n_events;
	ret = n = collect_events(cpuc, event, false);
	if (ret < 0)
		goto out;

	hwc->state = PERF_HES_UPTODATE | PERF_HES_STOPPED;
	if (!(flags & PERF_EF_START))
		hwc->state |= PERF_HES_ARCH;

	/*
	 * If group events scheduling transaction was started,
	 * skip the schedulability test here, it will be performed
	 * at commit time (->commit_txn) as a whole
	 */
	if (cpuc->group_flag & PERF_EVENT_TXN)
		goto done_collect;

	ret = x86_pmu.schedule_events(cpuc, n, assign);
	if (ret)
		goto out;
	/*
	 * copy new assignment, now we know it is possible
	 * will be used by hw_perf_enable()
	 */
	memcpy(cpuc->assign, assign, n*sizeof(int));

done_collect:
	cpuc->n_events = n;
	cpuc->n_added += n - n0;
	cpuc->n_txn += n - n0;

	ret = 0;
out:
	perf_pmu_enable(event->pmu);
	return ret;
}

static void x86_pmu_start(struct perf_event *event, int flags)
{
	struct cpu_hw_events *cpuc = &__get_cpu_var(cpu_hw_events);
	int idx = event->hw.idx;

	if (WARN_ON_ONCE(!(event->hw.state & PERF_HES_STOPPED)))
		return;

	if (WARN_ON_ONCE(idx == -1))
		return;

	if (flags & PERF_EF_RELOAD) {
		WARN_ON_ONCE(!(event->hw.state & PERF_HES_UPTODATE));
		x86_perf_event_set_period(event);
	}

	event->hw.state = 0;

	cpuc->events[idx] = event;
	__set_bit(idx, cpuc->active_mask);
	__set_bit(idx, cpuc->running);
	x86_pmu.enable(event);
	perf_event_update_userpage(event);
}

void perf_event_print_debug(void)
{
	u64 ctrl, status, overflow, pmc_ctrl, pmc_count, prev_left, fixed;
	u64 pebs;
	struct cpu_hw_events *cpuc;
	unsigned long flags;
	int cpu, idx;

	if (!x86_pmu.num_counters)
		return;

	local_irq_save(flags);

	cpu = smp_processor_id();
	cpuc = &per_cpu(cpu_hw_events, cpu);

	if (x86_pmu.version >= 2) {
		rdmsrl(MSR_CORE_PERF_GLOBAL_CTRL, ctrl);
		rdmsrl(MSR_CORE_PERF_GLOBAL_STATUS, status);
		rdmsrl(MSR_CORE_PERF_GLOBAL_OVF_CTRL, overflow);
		rdmsrl(MSR_ARCH_PERFMON_FIXED_CTR_CTRL, fixed);
		rdmsrl(MSR_IA32_PEBS_ENABLE, pebs);

		pr_info("\n");
		pr_info("CPU#%d: ctrl:       %016llx\n", cpu, ctrl);
		pr_info("CPU#%d: status:     %016llx\n", cpu, status);
		pr_info("CPU#%d: overflow:   %016llx\n", cpu, overflow);
		pr_info("CPU#%d: fixed:      %016llx\n", cpu, fixed);
		pr_info("CPU#%d: pebs:       %016llx\n", cpu, pebs);
	}
	pr_info("CPU#%d: active:     %016llx\n", cpu, *(u64 *)cpuc->active_mask);

	for (idx = 0; idx < x86_pmu.num_counters; idx++) {
		rdmsrl(x86_pmu_config_addr(idx), pmc_ctrl);
		rdmsrl(x86_pmu_event_addr(idx), pmc_count);

		prev_left = per_cpu(pmc_prev_left[idx], cpu);

		pr_info("CPU#%d:   gen-PMC%d ctrl:  %016llx\n",
			cpu, idx, pmc_ctrl);
		pr_info("CPU#%d:   gen-PMC%d count: %016llx\n",
			cpu, idx, pmc_count);
		pr_info("CPU#%d:   gen-PMC%d left:  %016llx\n",
			cpu, idx, prev_left);
	}
	for (idx = 0; idx < x86_pmu.num_counters_fixed; idx++) {
		rdmsrl(MSR_ARCH_PERFMON_FIXED_CTR0 + idx, pmc_count);

		pr_info("CPU#%d: fixed-PMC%d count: %016llx\n",
			cpu, idx, pmc_count);
	}
	local_irq_restore(flags);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void x86_pmu_stop(struct perf_event *event, int flags)
=======
void x86_pmu_stop(struct perf_event *event, int flags)
>>>>>>> refs/remotes/origin/cm-10.0
=======
void x86_pmu_stop(struct perf_event *event, int flags)
>>>>>>> refs/remotes/origin/master
{
	struct cpu_hw_events *cpuc = &__get_cpu_var(cpu_hw_events);
	struct hw_perf_event *hwc = &event->hw;

	if (__test_and_clear_bit(hwc->idx, cpuc->active_mask)) {
		x86_pmu.disable(event);
		cpuc->events[hwc->idx] = NULL;
		WARN_ON_ONCE(hwc->state & PERF_HES_STOPPED);
		hwc->state |= PERF_HES_STOPPED;
	}

	if ((flags & PERF_EF_UPDATE) && !(hwc->state & PERF_HES_UPTODATE)) {
		/*
		 * Drain the remaining delta count out of a event
		 * that we are disabling:
		 */
		x86_perf_event_update(event);
		hwc->state |= PERF_HES_UPTODATE;
	}
}

static void x86_pmu_del(struct perf_event *event, int flags)
{
	struct cpu_hw_events *cpuc = &__get_cpu_var(cpu_hw_events);
	int i;

	/*
<<<<<<< HEAD
=======
	 * event is descheduled
	 */
	event->hw.flags &= ~PERF_X86_EVENT_COMMITTED;

	/*
>>>>>>> refs/remotes/origin/master
	 * If we're called during a txn, we don't need to do anything.
	 * The events never got scheduled and ->cancel_txn will truncate
	 * the event_list.
	 */
	if (cpuc->group_flag & PERF_EVENT_TXN)
		return;

	x86_pmu_stop(event, PERF_EF_UPDATE);

	for (i = 0; i < cpuc->n_events; i++) {
		if (event == cpuc->event_list[i]) {

			if (x86_pmu.put_event_constraints)
				x86_pmu.put_event_constraints(cpuc, event);

			while (++i < cpuc->n_events)
				cpuc->event_list[i-1] = cpuc->event_list[i];

			--cpuc->n_events;
			break;
		}
	}
	perf_event_update_userpage(event);
}

<<<<<<< HEAD
<<<<<<< HEAD
static int x86_pmu_handle_irq(struct pt_regs *regs)
=======
int x86_pmu_handle_irq(struct pt_regs *regs)
>>>>>>> refs/remotes/origin/cm-10.0
=======
int x86_pmu_handle_irq(struct pt_regs *regs)
>>>>>>> refs/remotes/origin/master
{
	struct perf_sample_data data;
	struct cpu_hw_events *cpuc;
	struct perf_event *event;
	int idx, handled = 0;
	u64 val;

<<<<<<< HEAD
	perf_sample_data_init(&data, 0);

=======
>>>>>>> refs/remotes/origin/master
	cpuc = &__get_cpu_var(cpu_hw_events);

	/*
	 * Some chipsets need to unmask the LVTPC in a particular spot
	 * inside the nmi handler.  As a result, the unmasking was pushed
	 * into all the nmi handlers.
	 *
	 * This generic handler doesn't seem to have any issues where the
	 * unmasking occurs so it was left at the top.
	 */
	apic_write(APIC_LVTPC, APIC_DM_NMI);

	for (idx = 0; idx < x86_pmu.num_counters; idx++) {
		if (!test_bit(idx, cpuc->active_mask)) {
			/*
			 * Though we deactivated the counter some cpus
			 * might still deliver spurious interrupts still
			 * in flight. Catch them:
			 */
			if (__test_and_clear_bit(idx, cpuc->running))
				handled++;
			continue;
		}

		event = cpuc->events[idx];

		val = x86_perf_event_update(event);
		if (val & (1ULL << (x86_pmu.cntval_bits - 1)))
			continue;

		/*
		 * event overflow
		 */
		handled++;
<<<<<<< HEAD
		data.period	= event->hw.last_period;
=======
		perf_sample_data_init(&data, 0, event->hw.last_period);
>>>>>>> refs/remotes/origin/master

		if (!x86_perf_event_set_period(event))
			continue;

<<<<<<< HEAD
<<<<<<< HEAD
		if (perf_event_overflow(event, 1, &data, regs))
=======
		if (perf_event_overflow(event, &data, regs))
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (perf_event_overflow(event, &data, regs))
>>>>>>> refs/remotes/origin/master
			x86_pmu_stop(event, 0);
	}

	if (handled)
		inc_irq_stat(apic_perf_irqs);

	return handled;
}

void perf_events_lapic_init(void)
{
	if (!x86_pmu.apic || !x86_pmu_initialized())
		return;

	/*
	 * Always use NMI for PMU
	 */
	apic_write(APIC_LVTPC, APIC_DM_NMI);
}

<<<<<<< HEAD
<<<<<<< HEAD
struct pmu_nmi_state {
	unsigned int	marked;
	int		handled;
};

static DEFINE_PER_CPU(struct pmu_nmi_state, pmu_nmi);

static int __kprobes
perf_event_nmi_handler(struct notifier_block *self,
			 unsigned long cmd, void *__args)
{
	struct die_args *args = __args;
	unsigned int this_nmi;
	int handled;

	if (!atomic_read(&active_events))
		return NOTIFY_DONE;

	switch (cmd) {
	case DIE_NMI:
		break;
	case DIE_NMIUNKNOWN:
		this_nmi = percpu_read(irq_stat.__nmi_count);
		if (this_nmi != __this_cpu_read(pmu_nmi.marked))
			/* let the kernel handle the unknown nmi */
			return NOTIFY_DONE;
		/*
		 * This one is a PMU back-to-back nmi. Two events
		 * trigger 'simultaneously' raising two back-to-back
		 * NMIs. If the first NMI handles both, the latter
		 * will be empty and daze the CPU. So, we drop it to
		 * avoid false-positive 'unknown nmi' messages.
		 */
		return NOTIFY_STOP;
	default:
		return NOTIFY_DONE;
	}

	handled = x86_pmu.handle_irq(args->regs);
	if (!handled)
		return NOTIFY_DONE;

	this_nmi = percpu_read(irq_stat.__nmi_count);
	if ((handled > 1) ||
		/* the next nmi could be a back-to-back nmi */
	    ((__this_cpu_read(pmu_nmi.marked) == this_nmi) &&
	     (__this_cpu_read(pmu_nmi.handled) > 1))) {
		/*
		 * We could have two subsequent back-to-back nmis: The
		 * first handles more than one counter, the 2nd
		 * handles only one counter and the 3rd handles no
		 * counter.
		 *
		 * This is the 2nd nmi because the previous was
		 * handling more than one counter. We will mark the
		 * next (3rd) and then drop it if unhandled.
		 */
		__this_cpu_write(pmu_nmi.marked, this_nmi + 1);
		__this_cpu_write(pmu_nmi.handled, handled);
	}

	return NOTIFY_STOP;
}

static __read_mostly struct notifier_block perf_event_nmi_notifier = {
	.notifier_call		= perf_event_nmi_handler,
	.next			= NULL,
	.priority		= NMI_LOCAL_LOW_PRIOR,
};

static struct event_constraint unconstrained;
static struct event_constraint emptyconstraint;

static struct event_constraint *
x86_get_event_constraints(struct cpu_hw_events *cpuc, struct perf_event *event)
{
	struct event_constraint *c;

	if (x86_pmu.event_constraints) {
		for_each_event_constraint(c, x86_pmu.event_constraints) {
			if ((event->hw.config & c->cmask) == c->code)
				return c;
		}
	}

	return &unconstrained;
}

#include "perf_event_amd.c"
#include "perf_event_p6.c"
#include "perf_event_p4.c"
#include "perf_event_intel_lbr.c"
#include "perf_event_intel_ds.c"
#include "perf_event_intel.c"
=======
static int __kprobes
perf_event_nmi_handler(unsigned int cmd, struct pt_regs *regs)
{
	if (!atomic_read(&active_events))
		return NMI_DONE;

	return x86_pmu.handle_irq(regs);
=======
static int __kprobes
perf_event_nmi_handler(unsigned int cmd, struct pt_regs *regs)
{
	u64 start_clock;
	u64 finish_clock;
	int ret;

	if (!atomic_read(&active_events))
		return NMI_DONE;

	start_clock = sched_clock();
	ret = x86_pmu.handle_irq(regs);
	finish_clock = sched_clock();

	perf_sample_event_took(finish_clock - start_clock);

	return ret;
>>>>>>> refs/remotes/origin/master
}

struct event_constraint emptyconstraint;
struct event_constraint unconstrained;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

static int __cpuinit
x86_pmu_notifier(struct notifier_block *self, unsigned long action, void *hcpu)
{
	unsigned int cpu = (long)hcpu;
<<<<<<< HEAD
=======
	struct cpu_hw_events *cpuc = &per_cpu(cpu_hw_events, cpu);
>>>>>>> refs/remotes/origin/cm-10.0
=======

static int
x86_pmu_notifier(struct notifier_block *self, unsigned long action, void *hcpu)
{
	unsigned int cpu = (long)hcpu;
	struct cpu_hw_events *cpuc = &per_cpu(cpu_hw_events, cpu);
>>>>>>> refs/remotes/origin/master
	int ret = NOTIFY_OK;

	switch (action & ~CPU_TASKS_FROZEN) {
	case CPU_UP_PREPARE:
<<<<<<< HEAD
<<<<<<< HEAD
=======
		cpuc->kfree_on_online = NULL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		cpuc->kfree_on_online = NULL;
>>>>>>> refs/remotes/origin/master
		if (x86_pmu.cpu_prepare)
			ret = x86_pmu.cpu_prepare(cpu);
		break;

	case CPU_STARTING:
<<<<<<< HEAD
<<<<<<< HEAD
=======
		if (x86_pmu.attr_rdpmc)
			set_in_cr4(X86_CR4_PCE);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		if (x86_pmu.attr_rdpmc)
			set_in_cr4(X86_CR4_PCE);
>>>>>>> refs/remotes/origin/master
		if (x86_pmu.cpu_starting)
			x86_pmu.cpu_starting(cpu);
		break;

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	case CPU_ONLINE:
		kfree(cpuc->kfree_on_online);
		break;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	case CPU_DYING:
		if (x86_pmu.cpu_dying)
			x86_pmu.cpu_dying(cpu);
		break;

	case CPU_UP_CANCELED:
	case CPU_DEAD:
		if (x86_pmu.cpu_dead)
			x86_pmu.cpu_dead(cpu);
		break;

	default:
		break;
	}

	return ret;
}

static void __init pmu_check_apic(void)
{
	if (cpu_has_apic)
		return;

	x86_pmu.apic = 0;
	pr_info("no APIC, boot with the \"lapic\" boot parameter to force-enable it.\n");
	pr_info("no hardware sampling interrupt available.\n");
}

<<<<<<< HEAD
<<<<<<< HEAD
static int __init init_hw_perf_events(void)
{
=======
=======
>>>>>>> refs/remotes/origin/master
static struct attribute_group x86_pmu_format_group = {
	.name = "format",
	.attrs = NULL,
};

<<<<<<< HEAD
static int __init init_hw_perf_events(void)
{
	struct x86_pmu_quirk *quirk;
>>>>>>> refs/remotes/origin/cm-10.0
	struct event_constraint *c;
=======
/*
 * Remove all undefined events (x86_pmu.event_map(id) == 0)
 * out of events_attr attributes.
 */
static void __init filter_events(struct attribute **attrs)
{
	struct device_attribute *d;
	struct perf_pmu_events_attr *pmu_attr;
	int i, j;

	for (i = 0; attrs[i]; i++) {
		d = (struct device_attribute *)attrs[i];
		pmu_attr = container_of(d, struct perf_pmu_events_attr, attr);
		/* str trumps id */
		if (pmu_attr->event_str)
			continue;
		if (x86_pmu.event_map(i))
			continue;

		for (j = i; attrs[j]; j++)
			attrs[j] = attrs[j + 1];

		/* Check the shifted attr. */
		i--;
	}
}

/* Merge two pointer arrays */
static __init struct attribute **merge_attr(struct attribute **a, struct attribute **b)
{
	struct attribute **new;
	int j, i;

	for (j = 0; a[j]; j++)
		;
	for (i = 0; b[i]; i++)
		j++;
	j++;

	new = kmalloc(sizeof(struct attribute *) * j, GFP_KERNEL);
	if (!new)
		return NULL;

	j = 0;
	for (i = 0; a[i]; i++)
		new[j++] = a[i];
	for (i = 0; b[i]; i++)
		new[j++] = b[i];
	new[j] = NULL;

	return new;
}

ssize_t events_sysfs_show(struct device *dev, struct device_attribute *attr,
			  char *page)
{
	struct perf_pmu_events_attr *pmu_attr = \
		container_of(attr, struct perf_pmu_events_attr, attr);
	u64 config = x86_pmu.event_map(pmu_attr->id);

	/* string trumps id */
	if (pmu_attr->event_str)
		return sprintf(page, "%s", pmu_attr->event_str);

	return x86_pmu.events_sysfs_show(page, config);
}

EVENT_ATTR(cpu-cycles,			CPU_CYCLES		);
EVENT_ATTR(instructions,		INSTRUCTIONS		);
EVENT_ATTR(cache-references,		CACHE_REFERENCES	);
EVENT_ATTR(cache-misses, 		CACHE_MISSES		);
EVENT_ATTR(branch-instructions,		BRANCH_INSTRUCTIONS	);
EVENT_ATTR(branch-misses,		BRANCH_MISSES		);
EVENT_ATTR(bus-cycles,			BUS_CYCLES		);
EVENT_ATTR(stalled-cycles-frontend,	STALLED_CYCLES_FRONTEND	);
EVENT_ATTR(stalled-cycles-backend,	STALLED_CYCLES_BACKEND	);
EVENT_ATTR(ref-cycles,			REF_CPU_CYCLES		);

static struct attribute *empty_attrs;

static struct attribute *events_attr[] = {
	EVENT_PTR(CPU_CYCLES),
	EVENT_PTR(INSTRUCTIONS),
	EVENT_PTR(CACHE_REFERENCES),
	EVENT_PTR(CACHE_MISSES),
	EVENT_PTR(BRANCH_INSTRUCTIONS),
	EVENT_PTR(BRANCH_MISSES),
	EVENT_PTR(BUS_CYCLES),
	EVENT_PTR(STALLED_CYCLES_FRONTEND),
	EVENT_PTR(STALLED_CYCLES_BACKEND),
	EVENT_PTR(REF_CPU_CYCLES),
	NULL,
};

static struct attribute_group x86_pmu_events_group = {
	.name = "events",
	.attrs = events_attr,
};

ssize_t x86_event_sysfs_show(char *page, u64 config, u64 event)
{
	u64 umask  = (config & ARCH_PERFMON_EVENTSEL_UMASK) >> 8;
	u64 cmask  = (config & ARCH_PERFMON_EVENTSEL_CMASK) >> 24;
	bool edge  = (config & ARCH_PERFMON_EVENTSEL_EDGE);
	bool pc    = (config & ARCH_PERFMON_EVENTSEL_PIN_CONTROL);
	bool any   = (config & ARCH_PERFMON_EVENTSEL_ANY);
	bool inv   = (config & ARCH_PERFMON_EVENTSEL_INV);
	ssize_t ret;

	/*
	* We have whole page size to spend and just little data
	* to write, so we can safely use sprintf.
	*/
	ret = sprintf(page, "event=0x%02llx", event);

	if (umask)
		ret += sprintf(page + ret, ",umask=0x%02llx", umask);

	if (edge)
		ret += sprintf(page + ret, ",edge");

	if (pc)
		ret += sprintf(page + ret, ",pc");

	if (any)
		ret += sprintf(page + ret, ",any");

	if (inv)
		ret += sprintf(page + ret, ",inv");

	if (cmask)
		ret += sprintf(page + ret, ",cmask=0x%02llx", cmask);

	ret += sprintf(page + ret, "\n");

	return ret;
}

static int __init init_hw_perf_events(void)
{
	struct x86_pmu_quirk *quirk;
>>>>>>> refs/remotes/origin/master
	int err;

	pr_info("Performance Events: ");

	switch (boot_cpu_data.x86_vendor) {
	case X86_VENDOR_INTEL:
		err = intel_pmu_init();
		break;
	case X86_VENDOR_AMD:
		err = amd_pmu_init();
		break;
	default:
<<<<<<< HEAD
		return 0;
=======
		err = -ENOTSUPP;
>>>>>>> refs/remotes/origin/master
	}
	if (err != 0) {
		pr_cont("no PMU driver, software events only.\n");
		return 0;
	}

	pmu_check_apic();

	/* sanity check that the hardware exists or is emulated */
	if (!check_hw_exists())
		return 0;

	pr_cont("%s PMU driver.\n", x86_pmu.name);

<<<<<<< HEAD
<<<<<<< HEAD
	if (x86_pmu.quirks)
		x86_pmu.quirks();
=======
	for (quirk = x86_pmu.quirks; quirk; quirk = quirk->next)
		quirk->func();
>>>>>>> refs/remotes/origin/cm-10.0

	if (x86_pmu.num_counters > X86_PMC_MAX_GENERIC) {
		WARN(1, KERN_ERR "hw perf events %d > max(%d), clipping!",
		     x86_pmu.num_counters, X86_PMC_MAX_GENERIC);
		x86_pmu.num_counters = X86_PMC_MAX_GENERIC;
	}
	x86_pmu.intel_ctrl = (1 << x86_pmu.num_counters) - 1;

	if (x86_pmu.num_counters_fixed > X86_PMC_MAX_FIXED) {
		WARN(1, KERN_ERR "hw perf events fixed %d > max(%d), clipping!",
		     x86_pmu.num_counters_fixed, X86_PMC_MAX_FIXED);
		x86_pmu.num_counters_fixed = X86_PMC_MAX_FIXED;
	}

	x86_pmu.intel_ctrl |=
		((1LL << x86_pmu.num_counters_fixed)-1) << X86_PMC_IDX_FIXED;

	perf_events_lapic_init();
<<<<<<< HEAD
	register_die_notifier(&perf_event_nmi_notifier);

	unconstrained = (struct event_constraint)
		__EVENT_CONSTRAINT(0, (1ULL << x86_pmu.num_counters) - 1,
				   0, x86_pmu.num_counters);

	if (x86_pmu.event_constraints) {
		for_each_event_constraint(c, x86_pmu.event_constraints) {
			if (c->cmask != X86_RAW_EVENT_MASK)
				continue;
=======
	register_nmi_handler(NMI_LOCAL, perf_event_nmi_handler, 0, "PMI");

	unconstrained = (struct event_constraint)
		__EVENT_CONSTRAINT(0, (1ULL << x86_pmu.num_counters) - 1,
				   0, x86_pmu.num_counters, 0);

	if (x86_pmu.event_constraints) {
		/*
		 * event on fixed counter2 (REF_CYCLES) only works on this
		 * counter, so do not extend mask to generic counters
		 */
		for_each_event_constraint(c, x86_pmu.event_constraints) {
			if (c->cmask != X86_RAW_EVENT_MASK
			    || c->idxmsk64 == X86_PMC_MSK_FIXED_REF_CYCLES) {
				continue;
			}
>>>>>>> refs/remotes/origin/cm-10.0

			c->idxmsk64 |= (1ULL << x86_pmu.num_counters) - 1;
			c->weight += x86_pmu.num_counters;
		}
	}

<<<<<<< HEAD
=======
	x86_pmu.attr_rdpmc = 1; /* enable userspace RDPMC usage by default */
	x86_pmu_format_group.attrs = x86_pmu.format_attrs;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	for (quirk = x86_pmu.quirks; quirk; quirk = quirk->next)
		quirk->func();

	if (!x86_pmu.intel_ctrl)
		x86_pmu.intel_ctrl = (1 << x86_pmu.num_counters) - 1;

	perf_events_lapic_init();
	register_nmi_handler(NMI_LOCAL, perf_event_nmi_handler, 0, "PMI");

	unconstrained = (struct event_constraint)
		__EVENT_CONSTRAINT(0, (1ULL << x86_pmu.num_counters) - 1,
				   0, x86_pmu.num_counters, 0, 0);

	x86_pmu.attr_rdpmc = 1; /* enable userspace RDPMC usage by default */
	x86_pmu_format_group.attrs = x86_pmu.format_attrs;

	if (x86_pmu.event_attrs)
		x86_pmu_events_group.attrs = x86_pmu.event_attrs;

	if (!x86_pmu.events_sysfs_show)
		x86_pmu_events_group.attrs = &empty_attrs;
	else
		filter_events(x86_pmu_events_group.attrs);

	if (x86_pmu.cpu_events) {
		struct attribute **tmp;

		tmp = merge_attr(x86_pmu_events_group.attrs, x86_pmu.cpu_events);
		if (!WARN_ON(!tmp))
			x86_pmu_events_group.attrs = tmp;
	}

>>>>>>> refs/remotes/origin/master
	pr_info("... version:                %d\n",     x86_pmu.version);
	pr_info("... bit width:              %d\n",     x86_pmu.cntval_bits);
	pr_info("... generic registers:      %d\n",     x86_pmu.num_counters);
	pr_info("... value mask:             %016Lx\n", x86_pmu.cntval_mask);
	pr_info("... max period:             %016Lx\n", x86_pmu.max_period);
	pr_info("... fixed-purpose events:   %d\n",     x86_pmu.num_counters_fixed);
	pr_info("... event mask:             %016Lx\n", x86_pmu.intel_ctrl);

	perf_pmu_register(&pmu, "cpu", PERF_TYPE_RAW);
	perf_cpu_notifier(x86_pmu_notifier);

	return 0;
}
early_initcall(init_hw_perf_events);

static inline void x86_pmu_read(struct perf_event *event)
{
	x86_perf_event_update(event);
}

/*
 * Start group events scheduling transaction
 * Set the flag to make pmu::enable() not perform the
 * schedulability test, it will be performed at commit time
 */
static void x86_pmu_start_txn(struct pmu *pmu)
{
	perf_pmu_disable(pmu);
	__this_cpu_or(cpu_hw_events.group_flag, PERF_EVENT_TXN);
	__this_cpu_write(cpu_hw_events.n_txn, 0);
}

/*
 * Stop group events scheduling transaction
 * Clear the flag and pmu::enable() will perform the
 * schedulability test.
 */
static void x86_pmu_cancel_txn(struct pmu *pmu)
{
	__this_cpu_and(cpu_hw_events.group_flag, ~PERF_EVENT_TXN);
	/*
	 * Truncate the collected events.
	 */
	__this_cpu_sub(cpu_hw_events.n_added, __this_cpu_read(cpu_hw_events.n_txn));
	__this_cpu_sub(cpu_hw_events.n_events, __this_cpu_read(cpu_hw_events.n_txn));
	perf_pmu_enable(pmu);
}

/*
 * Commit group events scheduling transaction
 * Perform the group schedulability test as a whole
 * Return 0 if success
 */
static int x86_pmu_commit_txn(struct pmu *pmu)
{
	struct cpu_hw_events *cpuc = &__get_cpu_var(cpu_hw_events);
	int assign[X86_PMC_IDX_MAX];
	int n, ret;

	n = cpuc->n_events;

	if (!x86_pmu_initialized())
		return -EAGAIN;

	ret = x86_pmu.schedule_events(cpuc, n, assign);
	if (ret)
		return ret;

	/*
	 * copy new assignment, now we know it is possible
	 * will be used by hw_perf_enable()
	 */
	memcpy(cpuc->assign, assign, n*sizeof(int));

	cpuc->group_flag &= ~PERF_EVENT_TXN;
	perf_pmu_enable(pmu);
	return 0;
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * a fake_cpuc is used to validate event groups. Due to
 * the extra reg logic, we need to also allocate a fake
 * per_core and per_cpu structure. Otherwise, group events
 * using extra reg may conflict without the kernel being
 * able to catch this when the last event gets added to
 * the group.
 */
static void free_fake_cpuc(struct cpu_hw_events *cpuc)
{
	kfree(cpuc->shared_regs);
	kfree(cpuc);
}

static struct cpu_hw_events *allocate_fake_cpuc(void)
{
	struct cpu_hw_events *cpuc;
	int cpu = raw_smp_processor_id();

	cpuc = kzalloc(sizeof(*cpuc), GFP_KERNEL);
	if (!cpuc)
		return ERR_PTR(-ENOMEM);

	/* only needed, if we have extra_regs */
	if (x86_pmu.extra_regs) {
		cpuc->shared_regs = allocate_shared_regs(cpu);
		if (!cpuc->shared_regs)
			goto error;
	}
<<<<<<< HEAD
=======
	cpuc->is_fake = 1;
>>>>>>> refs/remotes/origin/master
	return cpuc;
error:
	free_fake_cpuc(cpuc);
	return ERR_PTR(-ENOMEM);
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

/*
 * validate that we can schedule this event
 */
static int validate_event(struct perf_event *event)
{
	struct cpu_hw_events *fake_cpuc;
	struct event_constraint *c;
	int ret = 0;

<<<<<<< HEAD
<<<<<<< HEAD
	fake_cpuc = kmalloc(sizeof(*fake_cpuc), GFP_KERNEL | __GFP_ZERO);
	if (!fake_cpuc)
		return -ENOMEM;
=======
	fake_cpuc = allocate_fake_cpuc();
	if (IS_ERR(fake_cpuc))
		return PTR_ERR(fake_cpuc);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	fake_cpuc = allocate_fake_cpuc();
	if (IS_ERR(fake_cpuc))
		return PTR_ERR(fake_cpuc);
>>>>>>> refs/remotes/origin/master

	c = x86_pmu.get_event_constraints(fake_cpuc, event);

	if (!c || !c->weight)
<<<<<<< HEAD
<<<<<<< HEAD
		ret = -ENOSPC;
=======
		ret = -EINVAL;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		ret = -EINVAL;
>>>>>>> refs/remotes/origin/master

	if (x86_pmu.put_event_constraints)
		x86_pmu.put_event_constraints(fake_cpuc, event);

<<<<<<< HEAD
<<<<<<< HEAD
	kfree(fake_cpuc);
=======
	free_fake_cpuc(fake_cpuc);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	free_fake_cpuc(fake_cpuc);
>>>>>>> refs/remotes/origin/master

	return ret;
}

/*
 * validate a single event group
 *
 * validation include:
 *	- check events are compatible which each other
 *	- events do not compete for the same counter
 *	- number of events <= number of counters
 *
 * validation ensures the group can be loaded onto the
 * PMU if it was the only group available.
 */
static int validate_group(struct perf_event *event)
{
	struct perf_event *leader = event->group_leader;
	struct cpu_hw_events *fake_cpuc;
<<<<<<< HEAD
<<<<<<< HEAD
	int ret, n;

	ret = -ENOMEM;
	fake_cpuc = kmalloc(sizeof(*fake_cpuc), GFP_KERNEL | __GFP_ZERO);
	if (!fake_cpuc)
		goto out;

=======
=======
>>>>>>> refs/remotes/origin/master
	int ret = -EINVAL, n;

	fake_cpuc = allocate_fake_cpuc();
	if (IS_ERR(fake_cpuc))
		return PTR_ERR(fake_cpuc);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * the event is not yet connected with its
	 * siblings therefore we must first collect
	 * existing siblings, then add the new event
	 * before we can simulate the scheduling
	 */
<<<<<<< HEAD
<<<<<<< HEAD
	ret = -ENOSPC;
	n = collect_events(fake_cpuc, leader, true);
	if (n < 0)
		goto out_free;
=======
	n = collect_events(fake_cpuc, leader, true);
	if (n < 0)
		goto out;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	n = collect_events(fake_cpuc, leader, true);
	if (n < 0)
		goto out;
>>>>>>> refs/remotes/origin/master

	fake_cpuc->n_events = n;
	n = collect_events(fake_cpuc, event, false);
	if (n < 0)
<<<<<<< HEAD
<<<<<<< HEAD
		goto out_free;
=======
		goto out;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		goto out;
>>>>>>> refs/remotes/origin/master

	fake_cpuc->n_events = n;

	ret = x86_pmu.schedule_events(fake_cpuc, n, NULL);

<<<<<<< HEAD
<<<<<<< HEAD
out_free:
	kfree(fake_cpuc);
out:
=======
out:
	free_fake_cpuc(fake_cpuc);
>>>>>>> refs/remotes/origin/cm-10.0
=======
out:
	free_fake_cpuc(fake_cpuc);
>>>>>>> refs/remotes/origin/master
	return ret;
}

static int x86_pmu_event_init(struct perf_event *event)
{
	struct pmu *tmp;
	int err;

	switch (event->attr.type) {
	case PERF_TYPE_RAW:
	case PERF_TYPE_HARDWARE:
	case PERF_TYPE_HW_CACHE:
		break;

	default:
		return -ENOENT;
	}

	err = __x86_pmu_event_init(event);
	if (!err) {
		/*
		 * we temporarily connect event to its pmu
		 * such that validate_group() can classify
		 * it as an x86 event using is_x86_event()
		 */
		tmp = event->pmu;
		event->pmu = &pmu;

		if (event->group_leader != event)
			err = validate_group(event);
		else
			err = validate_event(event);

		event->pmu = tmp;
	}
	if (err) {
		if (event->destroy)
			event->destroy(event);
	}

	return err;
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct pmu pmu = {
	.pmu_enable	= x86_pmu_enable,
	.pmu_disable	= x86_pmu_disable,

	.event_init	= x86_pmu_event_init,

	.add		= x86_pmu_add,
	.del		= x86_pmu_del,
	.start		= x86_pmu_start,
	.stop		= x86_pmu_stop,
	.read		= x86_pmu_read,
=======
=======
>>>>>>> refs/remotes/origin/master
static int x86_pmu_event_idx(struct perf_event *event)
{
	int idx = event->hw.idx;

	if (!x86_pmu.attr_rdpmc)
		return 0;

<<<<<<< HEAD
	if (x86_pmu.num_counters_fixed && idx >= X86_PMC_IDX_FIXED) {
		idx -= X86_PMC_IDX_FIXED;
=======
	if (x86_pmu.num_counters_fixed && idx >= INTEL_PMC_IDX_FIXED) {
		idx -= INTEL_PMC_IDX_FIXED;
>>>>>>> refs/remotes/origin/master
		idx |= 1 << 30;
	}

	return idx + 1;
}

static ssize_t get_attr_rdpmc(struct device *cdev,
			      struct device_attribute *attr,
			      char *buf)
{
	return snprintf(buf, 40, "%d\n", x86_pmu.attr_rdpmc);
}

static void change_rdpmc(void *info)
{
	bool enable = !!(unsigned long)info;

	if (enable)
		set_in_cr4(X86_CR4_PCE);
	else
		clear_in_cr4(X86_CR4_PCE);
}

static ssize_t set_attr_rdpmc(struct device *cdev,
			      struct device_attribute *attr,
			      const char *buf, size_t count)
{
<<<<<<< HEAD
	unsigned long val = simple_strtoul(buf, NULL, 0);
=======
	unsigned long val;
	ssize_t ret;

	ret = kstrtoul(buf, 0, &val);
	if (ret)
		return ret;
>>>>>>> refs/remotes/origin/master

	if (!!val != !!x86_pmu.attr_rdpmc) {
		x86_pmu.attr_rdpmc = !!val;
		smp_call_function(change_rdpmc, (void *)val, 1);
	}

	return count;
}

static DEVICE_ATTR(rdpmc, S_IRUSR | S_IWUSR, get_attr_rdpmc, set_attr_rdpmc);

static struct attribute *x86_pmu_attrs[] = {
	&dev_attr_rdpmc.attr,
	NULL,
};

static struct attribute_group x86_pmu_attr_group = {
	.attrs = x86_pmu_attrs,
};

static const struct attribute_group *x86_pmu_attr_groups[] = {
	&x86_pmu_attr_group,
	&x86_pmu_format_group,
<<<<<<< HEAD
=======
	&x86_pmu_events_group,
>>>>>>> refs/remotes/origin/master
	NULL,
};

static void x86_pmu_flush_branch_stack(void)
{
	if (x86_pmu.flush_branch_stack)
		x86_pmu.flush_branch_stack();
}

<<<<<<< HEAD
=======
void perf_check_microcode(void)
{
	if (x86_pmu.check_microcode)
		x86_pmu.check_microcode();
}
EXPORT_SYMBOL_GPL(perf_check_microcode);

>>>>>>> refs/remotes/origin/master
static struct pmu pmu = {
	.pmu_enable		= x86_pmu_enable,
	.pmu_disable		= x86_pmu_disable,

<<<<<<< HEAD
	.attr_groups	= x86_pmu_attr_groups,

	.event_init	= x86_pmu_event_init,
=======
	.attr_groups		= x86_pmu_attr_groups,

	.event_init		= x86_pmu_event_init,
>>>>>>> refs/remotes/origin/master

	.add			= x86_pmu_add,
	.del			= x86_pmu_del,
	.start			= x86_pmu_start,
	.stop			= x86_pmu_stop,
	.read			= x86_pmu_read,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	.start_txn	= x86_pmu_start_txn,
	.cancel_txn	= x86_pmu_cancel_txn,
	.commit_txn	= x86_pmu_commit_txn,
<<<<<<< HEAD
};

=======

	.event_idx	= x86_pmu_event_idx,
=======

	.start_txn		= x86_pmu_start_txn,
	.cancel_txn		= x86_pmu_cancel_txn,
	.commit_txn		= x86_pmu_commit_txn,

	.event_idx		= x86_pmu_event_idx,
>>>>>>> refs/remotes/origin/master
	.flush_branch_stack	= x86_pmu_flush_branch_stack,
};

void arch_perf_update_userpage(struct perf_event_mmap_page *userpg, u64 now)
{
<<<<<<< HEAD
	userpg->cap_usr_time = 0;
	userpg->cap_usr_rdpmc = x86_pmu.attr_rdpmc;
	userpg->pmc_width = x86_pmu.cntval_bits;

	if (!boot_cpu_has(X86_FEATURE_CONSTANT_TSC))
		return;

	if (!boot_cpu_has(X86_FEATURE_NONSTOP_TSC))
		return;

	userpg->cap_usr_time = 1;
	userpg->time_mult = this_cpu_read(cyc2ns);
	userpg->time_shift = CYC2NS_SCALE_FACTOR;
	userpg->time_offset = this_cpu_read(cyc2ns_offset) - now;
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct cyc2ns_data *data;

	userpg->cap_user_time = 0;
	userpg->cap_user_time_zero = 0;
	userpg->cap_user_rdpmc = x86_pmu.attr_rdpmc;
	userpg->pmc_width = x86_pmu.cntval_bits;

	if (!sched_clock_stable())
		return;

	data = cyc2ns_read_begin();

	userpg->cap_user_time = 1;
	userpg->time_mult = data->cyc2ns_mul;
	userpg->time_shift = data->cyc2ns_shift;
	userpg->time_offset = data->cyc2ns_offset - now;

	userpg->cap_user_time_zero = 1;
	userpg->time_zero = data->cyc2ns_offset;

	cyc2ns_read_end(data);
}

>>>>>>> refs/remotes/origin/master
/*
 * callchain support
 */

static int backtrace_stack(void *data, char *name)
{
	return 0;
}

static void backtrace_address(void *data, unsigned long addr, int reliable)
{
	struct perf_callchain_entry *entry = data;

	perf_callchain_store(entry, addr);
}

static const struct stacktrace_ops backtrace_ops = {
	.stack			= backtrace_stack,
	.address		= backtrace_address,
	.walk_stack		= print_context_stack_bp,
};

void
perf_callchain_kernel(struct perf_callchain_entry *entry, struct pt_regs *regs)
{
	if (perf_guest_cbs && perf_guest_cbs->is_in_guest()) {
		/* TODO: We don't support guest os callchain now */
		return;
	}

	perf_callchain_store(entry, regs->ip);

	dump_trace(NULL, regs, NULL, 0, &backtrace_ops, entry);
}

<<<<<<< HEAD
#ifdef CONFIG_COMPAT
<<<<<<< HEAD
=======

#include <asm/compat.h>

>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline int
valid_user_frame(const void __user *fp, unsigned long size)
{
	return (__range_not_ok(fp, size, TASK_SIZE) == 0);
}

static unsigned long get_segment_base(unsigned int segment)
{
	struct desc_struct *desc;
	int idx = segment >> 3;

	if ((segment & SEGMENT_TI_MASK) == SEGMENT_LDT) {
		if (idx > LDT_ENTRIES)
			return 0;

		if (idx > current->active_mm->context.size)
			return 0;

		desc = current->active_mm->context.ldt;
	} else {
		if (idx > GDT_ENTRIES)
			return 0;

		desc = __this_cpu_ptr(&gdt_page.gdt[0]);
	}

	return get_desc_base(desc + idx);
}

#ifdef CONFIG_COMPAT

#include <asm/compat.h>

>>>>>>> refs/remotes/origin/master
static inline int
perf_callchain_user32(struct pt_regs *regs, struct perf_callchain_entry *entry)
{
	/* 32-bit process in 64-bit kernel. */
<<<<<<< HEAD
=======
	unsigned long ss_base, cs_base;
>>>>>>> refs/remotes/origin/master
	struct stack_frame_ia32 frame;
	const void __user *fp;

	if (!test_thread_flag(TIF_IA32))
		return 0;

<<<<<<< HEAD
	fp = compat_ptr(regs->bp);
=======
	cs_base = get_segment_base(regs->cs);
	ss_base = get_segment_base(regs->ss);

	fp = compat_ptr(ss_base + regs->bp);
>>>>>>> refs/remotes/origin/master
	while (entry->nr < PERF_MAX_STACK_DEPTH) {
		unsigned long bytes;
		frame.next_frame     = 0;
		frame.return_address = 0;

		bytes = copy_from_user_nmi(&frame, fp, sizeof(frame));
<<<<<<< HEAD
		if (bytes != sizeof(frame))
			break;

		if (fp < compat_ptr(regs->sp))
			break;

		perf_callchain_store(entry, frame.return_address);
		fp = compat_ptr(frame.next_frame);
=======
		if (bytes != 0)
			break;

		if (!valid_user_frame(fp, sizeof(frame)))
			break;

		perf_callchain_store(entry, cs_base + frame.return_address);
		fp = compat_ptr(ss_base + frame.next_frame);
>>>>>>> refs/remotes/origin/master
	}
	return 1;
}
#else
static inline int
perf_callchain_user32(struct pt_regs *regs, struct perf_callchain_entry *entry)
{
    return 0;
}
#endif

void
perf_callchain_user(struct perf_callchain_entry *entry, struct pt_regs *regs)
{
	struct stack_frame frame;
	const void __user *fp;

	if (perf_guest_cbs && perf_guest_cbs->is_in_guest()) {
		/* TODO: We don't support guest os callchain now */
		return;
	}

<<<<<<< HEAD
=======
	/*
	 * We don't know what to do with VM86 stacks.. ignore them for now.
	 */
	if (regs->flags & (X86_VM_MASK | PERF_EFLAGS_VM))
		return;

>>>>>>> refs/remotes/origin/master
	fp = (void __user *)regs->bp;

	perf_callchain_store(entry, regs->ip);

	if (!current->mm)
		return;

	if (perf_callchain_user32(regs, entry))
		return;

	while (entry->nr < PERF_MAX_STACK_DEPTH) {
		unsigned long bytes;
		frame.next_frame	     = NULL;
		frame.return_address = 0;

		bytes = copy_from_user_nmi(&frame, fp, sizeof(frame));
<<<<<<< HEAD
		if (bytes != sizeof(frame))
			break;

		if ((unsigned long)fp < regs->sp)
=======
		if (bytes != 0)
			break;

		if (!valid_user_frame(fp, sizeof(frame)))
>>>>>>> refs/remotes/origin/master
			break;

		perf_callchain_store(entry, frame.return_address);
		fp = frame.next_frame;
	}
}

<<<<<<< HEAD
unsigned long perf_instruction_pointer(struct pt_regs *regs)
{
	unsigned long ip;

	if (perf_guest_cbs && perf_guest_cbs->is_in_guest())
		ip = perf_guest_cbs->get_guest_ip();
	else
		ip = instruction_pointer(regs);

	return ip;
=======
/*
 * Deal with code segment offsets for the various execution modes:
 *
 *   VM86 - the good olde 16 bit days, where the linear address is
 *          20 bits and we use regs->ip + 0x10 * regs->cs.
 *
 *   IA32 - Where we need to look at GDT/LDT segment descriptor tables
 *          to figure out what the 32bit base address is.
 *
 *    X32 - has TIF_X32 set, but is running in x86_64
 *
 * X86_64 - CS,DS,SS,ES are all zero based.
 */
static unsigned long code_segment_base(struct pt_regs *regs)
{
	/*
	 * If we are in VM86 mode, add the segment offset to convert to a
	 * linear address.
	 */
	if (regs->flags & X86_VM_MASK)
		return 0x10 * regs->cs;

	/*
	 * For IA32 we look at the GDT/LDT segment base to convert the
	 * effective IP to a linear address.
	 */
#ifdef CONFIG_X86_32
	if (user_mode(regs) && regs->cs != __USER_CS)
		return get_segment_base(regs->cs);
#else
	if (test_thread_flag(TIF_IA32)) {
		if (user_mode(regs) && regs->cs != __USER32_CS)
			return get_segment_base(regs->cs);
	}
#endif
	return 0;
}

unsigned long perf_instruction_pointer(struct pt_regs *regs)
{
	if (perf_guest_cbs && perf_guest_cbs->is_in_guest())
		return perf_guest_cbs->get_guest_ip();

	return regs->ip + code_segment_base(regs);
>>>>>>> refs/remotes/origin/master
}

unsigned long perf_misc_flags(struct pt_regs *regs)
{
	int misc = 0;

	if (perf_guest_cbs && perf_guest_cbs->is_in_guest()) {
		if (perf_guest_cbs->is_user_mode())
			misc |= PERF_RECORD_MISC_GUEST_USER;
		else
			misc |= PERF_RECORD_MISC_GUEST_KERNEL;
	} else {
		if (user_mode(regs))
			misc |= PERF_RECORD_MISC_USER;
		else
			misc |= PERF_RECORD_MISC_KERNEL;
	}

	if (regs->flags & PERF_EFLAGS_EXACT)
		misc |= PERF_RECORD_MISC_EXACT_IP;

	return misc;
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master

void perf_get_x86_pmu_capability(struct x86_pmu_capability *cap)
{
	cap->version		= x86_pmu.version;
	cap->num_counters_gp	= x86_pmu.num_counters;
	cap->num_counters_fixed	= x86_pmu.num_counters_fixed;
	cap->bit_width_gp	= x86_pmu.cntval_bits;
	cap->bit_width_fixed	= x86_pmu.cntval_bits;
	cap->events_mask	= (unsigned int)x86_pmu.events_maskl;
	cap->events_mask_len	= x86_pmu.events_mask_len;
}
EXPORT_SYMBOL_GPL(perf_get_x86_pmu_capability);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
