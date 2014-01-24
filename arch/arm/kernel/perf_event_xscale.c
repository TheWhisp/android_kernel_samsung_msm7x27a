/*
 * ARMv5 [xscale] Performance counter handling code.
 *
 * Copyright (C) 2010, ARM Ltd., Will Deacon <will.deacon@arm.com>
 *
 * Based on the previous xscale OProfile code.
 *
 * There are two variants of the xscale PMU that we support:
 * 	- xscale1pmu: 2 event counters and a cycle counter
 * 	- xscale2pmu: 4 event counters and a cycle counter
 * The two variants share event definitions, but have different
 * PMU structures.
 */

#ifdef CONFIG_CPU_XSCALE
enum xscale_perf_types {
	XSCALE_PERFCTR_ICACHE_MISS		= 0x00,
	XSCALE_PERFCTR_ICACHE_NO_DELIVER	= 0x01,
	XSCALE_PERFCTR_DATA_STALL		= 0x02,
	XSCALE_PERFCTR_ITLB_MISS		= 0x03,
	XSCALE_PERFCTR_DTLB_MISS		= 0x04,
	XSCALE_PERFCTR_BRANCH			= 0x05,
	XSCALE_PERFCTR_BRANCH_MISS		= 0x06,
	XSCALE_PERFCTR_INSTRUCTION		= 0x07,
	XSCALE_PERFCTR_DCACHE_FULL_STALL	= 0x08,
	XSCALE_PERFCTR_DCACHE_FULL_STALL_CONTIG	= 0x09,
	XSCALE_PERFCTR_DCACHE_ACCESS		= 0x0A,
	XSCALE_PERFCTR_DCACHE_MISS		= 0x0B,
	XSCALE_PERFCTR_DCACHE_WRITE_BACK	= 0x0C,
	XSCALE_PERFCTR_PC_CHANGED		= 0x0D,
	XSCALE_PERFCTR_BCU_REQUEST		= 0x10,
	XSCALE_PERFCTR_BCU_FULL			= 0x11,
	XSCALE_PERFCTR_BCU_DRAIN		= 0x12,
	XSCALE_PERFCTR_BCU_ECC_NO_ELOG		= 0x14,
	XSCALE_PERFCTR_BCU_1_BIT_ERR		= 0x15,
	XSCALE_PERFCTR_RMW			= 0x16,
	/* XSCALE_PERFCTR_CCNT is not hardware defined */
	XSCALE_PERFCTR_CCNT			= 0xFE,
	XSCALE_PERFCTR_UNUSED			= 0xFF,
};

enum xscale_counters {
<<<<<<< HEAD
<<<<<<< HEAD
	XSCALE_CYCLE_COUNTER	= 1,
=======
	XSCALE_CYCLE_COUNTER	= 0,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	XSCALE_CYCLE_COUNTER	= 0,
>>>>>>> refs/remotes/origin/master
	XSCALE_COUNTER0,
	XSCALE_COUNTER1,
	XSCALE_COUNTER2,
	XSCALE_COUNTER3,
};

static const unsigned xscale_perf_map[PERF_COUNT_HW_MAX] = {
<<<<<<< HEAD
<<<<<<< HEAD
	[PERF_COUNT_HW_CPU_CYCLES]	    = XSCALE_PERFCTR_CCNT,
	[PERF_COUNT_HW_INSTRUCTIONS]	    = XSCALE_PERFCTR_INSTRUCTION,
	[PERF_COUNT_HW_CACHE_REFERENCES]    = HW_OP_UNSUPPORTED,
	[PERF_COUNT_HW_CACHE_MISSES]	    = HW_OP_UNSUPPORTED,
	[PERF_COUNT_HW_BRANCH_INSTRUCTIONS] = XSCALE_PERFCTR_BRANCH,
	[PERF_COUNT_HW_BRANCH_MISSES]	    = XSCALE_PERFCTR_BRANCH_MISS,
	[PERF_COUNT_HW_BUS_CYCLES]	    = HW_OP_UNSUPPORTED,
=======
=======
>>>>>>> refs/remotes/origin/master
	[PERF_COUNT_HW_CPU_CYCLES]		= XSCALE_PERFCTR_CCNT,
	[PERF_COUNT_HW_INSTRUCTIONS]		= XSCALE_PERFCTR_INSTRUCTION,
	[PERF_COUNT_HW_CACHE_REFERENCES]	= HW_OP_UNSUPPORTED,
	[PERF_COUNT_HW_CACHE_MISSES]		= HW_OP_UNSUPPORTED,
	[PERF_COUNT_HW_BRANCH_INSTRUCTIONS]	= XSCALE_PERFCTR_BRANCH,
	[PERF_COUNT_HW_BRANCH_MISSES]		= XSCALE_PERFCTR_BRANCH_MISS,
	[PERF_COUNT_HW_BUS_CYCLES]		= HW_OP_UNSUPPORTED,
	[PERF_COUNT_HW_STALLED_CYCLES_FRONTEND]	= XSCALE_PERFCTR_ICACHE_NO_DELIVER,
	[PERF_COUNT_HW_STALLED_CYCLES_BACKEND]	= HW_OP_UNSUPPORTED,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
};

static unsigned xscale_perf_cache_map[PERF_COUNT_HW_CACHE_MAX]
=======
};

<<<<<<< HEAD
static const unsigned xscale_perf_cache_map[PERF_COUNT_HW_CACHE_MAX]
>>>>>>> refs/remotes/origin/master
=======
static unsigned xscale_perf_cache_map[PERF_COUNT_HW_CACHE_MAX]
>>>>>>> refs/remotes/origin/cm-11.0
					   [PERF_COUNT_HW_CACHE_OP_MAX]
					   [PERF_COUNT_HW_CACHE_RESULT_MAX] = {
	[C(L1D)] = {
		[C(OP_READ)] = {
			[C(RESULT_ACCESS)]	= XSCALE_PERFCTR_DCACHE_ACCESS,
			[C(RESULT_MISS)]	= XSCALE_PERFCTR_DCACHE_MISS,
		},
		[C(OP_WRITE)] = {
			[C(RESULT_ACCESS)]	= XSCALE_PERFCTR_DCACHE_ACCESS,
			[C(RESULT_MISS)]	= XSCALE_PERFCTR_DCACHE_MISS,
		},
		[C(OP_PREFETCH)] = {
			[C(RESULT_ACCESS)]	= CACHE_OP_UNSUPPORTED,
			[C(RESULT_MISS)]	= CACHE_OP_UNSUPPORTED,
		},
	},
	[C(L1I)] = {
		[C(OP_READ)] = {
			[C(RESULT_ACCESS)]	= CACHE_OP_UNSUPPORTED,
			[C(RESULT_MISS)]	= XSCALE_PERFCTR_ICACHE_MISS,
		},
		[C(OP_WRITE)] = {
			[C(RESULT_ACCESS)]	= CACHE_OP_UNSUPPORTED,
<<<<<<< HEAD
			[C(RESULT_MISS)]	= XSCALE_PERFCTR_ICACHE_MISS,
=======
			[C(RESULT_MISS)]	= CACHE_OP_UNSUPPORTED,
>>>>>>> refs/remotes/origin/master
		},
		[C(OP_PREFETCH)] = {
			[C(RESULT_ACCESS)]	= CACHE_OP_UNSUPPORTED,
			[C(RESULT_MISS)]	= CACHE_OP_UNSUPPORTED,
		},
	},
	[C(LL)] = {
		[C(OP_READ)] = {
			[C(RESULT_ACCESS)]	= CACHE_OP_UNSUPPORTED,
			[C(RESULT_MISS)]	= CACHE_OP_UNSUPPORTED,
		},
		[C(OP_WRITE)] = {
			[C(RESULT_ACCESS)]	= CACHE_OP_UNSUPPORTED,
			[C(RESULT_MISS)]	= CACHE_OP_UNSUPPORTED,
		},
		[C(OP_PREFETCH)] = {
			[C(RESULT_ACCESS)]	= CACHE_OP_UNSUPPORTED,
			[C(RESULT_MISS)]	= CACHE_OP_UNSUPPORTED,
		},
	},
	[C(DTLB)] = {
		[C(OP_READ)] = {
			[C(RESULT_ACCESS)]	= CACHE_OP_UNSUPPORTED,
			[C(RESULT_MISS)]	= XSCALE_PERFCTR_DTLB_MISS,
		},
		[C(OP_WRITE)] = {
			[C(RESULT_ACCESS)]	= CACHE_OP_UNSUPPORTED,
			[C(RESULT_MISS)]	= XSCALE_PERFCTR_DTLB_MISS,
		},
		[C(OP_PREFETCH)] = {
			[C(RESULT_ACCESS)]	= CACHE_OP_UNSUPPORTED,
			[C(RESULT_MISS)]	= CACHE_OP_UNSUPPORTED,
		},
	},
	[C(ITLB)] = {
		[C(OP_READ)] = {
			[C(RESULT_ACCESS)]	= CACHE_OP_UNSUPPORTED,
			[C(RESULT_MISS)]	= XSCALE_PERFCTR_ITLB_MISS,
		},
		[C(OP_WRITE)] = {
			[C(RESULT_ACCESS)]	= CACHE_OP_UNSUPPORTED,
			[C(RESULT_MISS)]	= XSCALE_PERFCTR_ITLB_MISS,
		},
		[C(OP_PREFETCH)] = {
			[C(RESULT_ACCESS)]	= CACHE_OP_UNSUPPORTED,
			[C(RESULT_MISS)]	= CACHE_OP_UNSUPPORTED,
		},
	},
	[C(BPU)] = {
		[C(OP_READ)] = {
			[C(RESULT_ACCESS)]	= CACHE_OP_UNSUPPORTED,
			[C(RESULT_MISS)]	= CACHE_OP_UNSUPPORTED,
		},
		[C(OP_WRITE)] = {
			[C(RESULT_ACCESS)]	= CACHE_OP_UNSUPPORTED,
			[C(RESULT_MISS)]	= CACHE_OP_UNSUPPORTED,
		},
		[C(OP_PREFETCH)] = {
			[C(RESULT_ACCESS)]	= CACHE_OP_UNSUPPORTED,
			[C(RESULT_MISS)]	= CACHE_OP_UNSUPPORTED,
		},
	},
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	[C(NODE)] = {
		[C(OP_READ)] = {
			[C(RESULT_ACCESS)]	= CACHE_OP_UNSUPPORTED,
			[C(RESULT_MISS)]	= CACHE_OP_UNSUPPORTED,
		},
		[C(OP_WRITE)] = {
			[C(RESULT_ACCESS)]	= CACHE_OP_UNSUPPORTED,
			[C(RESULT_MISS)]	= CACHE_OP_UNSUPPORTED,
		},
		[C(OP_PREFETCH)] = {
			[C(RESULT_ACCESS)]	= CACHE_OP_UNSUPPORTED,
			[C(RESULT_MISS)]	= CACHE_OP_UNSUPPORTED,
		},
	},
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

#define	XSCALE_PMU_ENABLE	0x001
#define XSCALE_PMN_RESET	0x002
#define	XSCALE_CCNT_RESET	0x004
#define	XSCALE_PMU_RESET	(CCNT_RESET | PMN_RESET)
#define XSCALE_PMU_CNT64	0x008

#define XSCALE1_OVERFLOWED_MASK	0x700
#define XSCALE1_CCOUNT_OVERFLOW	0x400
#define XSCALE1_COUNT0_OVERFLOW	0x100
#define XSCALE1_COUNT1_OVERFLOW	0x200
#define XSCALE1_CCOUNT_INT_EN	0x040
#define XSCALE1_COUNT0_INT_EN	0x010
#define XSCALE1_COUNT1_INT_EN	0x020
#define XSCALE1_COUNT0_EVT_SHFT	12
#define XSCALE1_COUNT0_EVT_MASK	(0xff << XSCALE1_COUNT0_EVT_SHFT)
#define XSCALE1_COUNT1_EVT_SHFT	20
#define XSCALE1_COUNT1_EVT_MASK	(0xff << XSCALE1_COUNT1_EVT_SHFT)

static inline u32
xscale1pmu_read_pmnc(void)
{
	u32 val;
	asm volatile("mrc p14, 0, %0, c0, c0, 0" : "=r" (val));
	return val;
}

static inline void
xscale1pmu_write_pmnc(u32 val)
{
	/* upper 4bits and 7, 11 are write-as-0 */
	val &= 0xffff77f;
	asm volatile("mcr p14, 0, %0, c0, c0, 0" : : "r" (val));
}

static inline int
xscale1_pmnc_counter_has_overflowed(unsigned long pmnc,
					enum xscale_counters counter)
{
	int ret = 0;

	switch (counter) {
	case XSCALE_CYCLE_COUNTER:
		ret = pmnc & XSCALE1_CCOUNT_OVERFLOW;
		break;
	case XSCALE_COUNTER0:
		ret = pmnc & XSCALE1_COUNT0_OVERFLOW;
		break;
	case XSCALE_COUNTER1:
		ret = pmnc & XSCALE1_COUNT1_OVERFLOW;
		break;
	default:
		WARN_ONCE(1, "invalid counter number (%d)\n", counter);
	}

	return ret;
}

static irqreturn_t
xscale1pmu_handle_irq(int irq_num, void *dev)
{
	unsigned long pmnc;
	struct perf_sample_data data;
<<<<<<< HEAD
<<<<<<< HEAD
	struct cpu_hw_events *cpuc;
=======
	struct pmu_hw_events *cpuc;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct arm_pmu *cpu_pmu = (struct arm_pmu *)dev;
	struct pmu_hw_events *cpuc = cpu_pmu->get_hw_events();
>>>>>>> refs/remotes/origin/master
	struct pt_regs *regs;
	int idx;

	/*
	 * NOTE: there's an A stepping erratum that states if an overflow
	 *       bit already exists and another occurs, the previous
	 *       Overflow bit gets cleared. There's no workaround.
	 *	 Fixed in B stepping or later.
	 */
	pmnc = xscale1pmu_read_pmnc();

	/*
	 * Write the value back to clear the overflow flags. Overflow
	 * flags remain in pmnc for use below. We also disable the PMU
	 * while we process the interrupt.
	 */
	xscale1pmu_write_pmnc(pmnc & ~XSCALE_PMU_ENABLE);

	if (!(pmnc & XSCALE1_OVERFLOWED_MASK))
		return IRQ_NONE;

	regs = get_irq_regs();

<<<<<<< HEAD
	perf_sample_data_init(&data, 0);

	cpuc = &__get_cpu_var(cpu_hw_events);
<<<<<<< HEAD
	for (idx = 0; idx <= armpmu->num_events; ++idx) {
		struct perf_event *event = cpuc->events[idx];
		struct hw_perf_event *hwc;

		if (!test_bit(idx, cpuc->active_mask))
=======
=======
>>>>>>> refs/remotes/origin/master
	for (idx = 0; idx < cpu_pmu->num_events; ++idx) {
		struct perf_event *event = cpuc->events[idx];
		struct hw_perf_event *hwc;

		if (!event)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			continue;

		if (!xscale1_pmnc_counter_has_overflowed(pmnc, idx))
			continue;

		hwc = &event->hw;
<<<<<<< HEAD
<<<<<<< HEAD
		armpmu_event_update(event, hwc, idx, 1);
=======
		armpmu_event_update(event, hwc, idx);
>>>>>>> refs/remotes/origin/cm-10.0
		data.period = event->hw.last_period;
		if (!armpmu_event_set_period(event, hwc, idx))
			continue;

<<<<<<< HEAD
		if (perf_event_overflow(event, 0, &data, regs))
			armpmu->disable(hwc, idx);
=======
		if (perf_event_overflow(event, &data, regs))
			cpu_pmu->disable(hwc, idx);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		armpmu_event_update(event);
		perf_sample_data_init(&data, 0, hwc->last_period);
		if (!armpmu_event_set_period(event))
			continue;

		if (perf_event_overflow(event, &data, regs))
			cpu_pmu->disable(event);
>>>>>>> refs/remotes/origin/master
	}

	irq_work_run();

	/*
	 * Re-enable the PMU.
	 */
	pmnc = xscale1pmu_read_pmnc() | XSCALE_PMU_ENABLE;
	xscale1pmu_write_pmnc(pmnc);

	return IRQ_HANDLED;
}

<<<<<<< HEAD
static void
xscale1pmu_enable_event(struct hw_perf_event *hwc, int idx)
{
	unsigned long val, mask, evt, flags;
<<<<<<< HEAD
=======
	struct pmu_hw_events *events = cpu_pmu->get_hw_events();
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void xscale1pmu_enable_event(struct perf_event *event)
{
	unsigned long val, mask, evt, flags;
	struct arm_pmu *cpu_pmu = to_arm_pmu(event->pmu);
	struct hw_perf_event *hwc = &event->hw;
	struct pmu_hw_events *events = cpu_pmu->get_hw_events();
	int idx = hwc->idx;
>>>>>>> refs/remotes/origin/master

	switch (idx) {
	case XSCALE_CYCLE_COUNTER:
		mask = 0;
		evt = XSCALE1_CCOUNT_INT_EN;
		break;
	case XSCALE_COUNTER0:
		mask = XSCALE1_COUNT0_EVT_MASK;
		evt = (hwc->config_base << XSCALE1_COUNT0_EVT_SHFT) |
			XSCALE1_COUNT0_INT_EN;
		break;
	case XSCALE_COUNTER1:
		mask = XSCALE1_COUNT1_EVT_MASK;
		evt = (hwc->config_base << XSCALE1_COUNT1_EVT_SHFT) |
			XSCALE1_COUNT1_INT_EN;
		break;
	default:
		WARN_ONCE(1, "invalid counter number (%d)\n", idx);
		return;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	raw_spin_lock_irqsave(&pmu_lock, flags);
=======
	raw_spin_lock_irqsave(&events->pmu_lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	raw_spin_lock_irqsave(&events->pmu_lock, flags);
>>>>>>> refs/remotes/origin/master
	val = xscale1pmu_read_pmnc();
	val &= ~mask;
	val |= evt;
	xscale1pmu_write_pmnc(val);
<<<<<<< HEAD
<<<<<<< HEAD
	raw_spin_unlock_irqrestore(&pmu_lock, flags);
=======
	raw_spin_unlock_irqrestore(&events->pmu_lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0
}

static void
xscale1pmu_disable_event(struct hw_perf_event *hwc, int idx)
{
	unsigned long val, mask, evt, flags;
<<<<<<< HEAD
=======
	struct pmu_hw_events *events = cpu_pmu->get_hw_events();
>>>>>>> refs/remotes/origin/cm-10.0
=======
	raw_spin_unlock_irqrestore(&events->pmu_lock, flags);
}

static void xscale1pmu_disable_event(struct perf_event *event)
{
	unsigned long val, mask, evt, flags;
	struct arm_pmu *cpu_pmu = to_arm_pmu(event->pmu);
	struct hw_perf_event *hwc = &event->hw;
	struct pmu_hw_events *events = cpu_pmu->get_hw_events();
	int idx = hwc->idx;
>>>>>>> refs/remotes/origin/master

	switch (idx) {
	case XSCALE_CYCLE_COUNTER:
		mask = XSCALE1_CCOUNT_INT_EN;
		evt = 0;
		break;
	case XSCALE_COUNTER0:
		mask = XSCALE1_COUNT0_INT_EN | XSCALE1_COUNT0_EVT_MASK;
		evt = XSCALE_PERFCTR_UNUSED << XSCALE1_COUNT0_EVT_SHFT;
		break;
	case XSCALE_COUNTER1:
		mask = XSCALE1_COUNT1_INT_EN | XSCALE1_COUNT1_EVT_MASK;
		evt = XSCALE_PERFCTR_UNUSED << XSCALE1_COUNT1_EVT_SHFT;
		break;
	default:
		WARN_ONCE(1, "invalid counter number (%d)\n", idx);
		return;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	raw_spin_lock_irqsave(&pmu_lock, flags);
=======
	raw_spin_lock_irqsave(&events->pmu_lock, flags);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	raw_spin_lock_irqsave(&events->pmu_lock, flags);
>>>>>>> refs/remotes/origin/master
	val = xscale1pmu_read_pmnc();
	val &= ~mask;
	val |= evt;
	xscale1pmu_write_pmnc(val);
<<<<<<< HEAD
<<<<<<< HEAD
	raw_spin_unlock_irqrestore(&pmu_lock, flags);
}

static int
xscale1pmu_get_event_idx(struct cpu_hw_events *cpuc,
=======
=======
>>>>>>> refs/remotes/origin/master
	raw_spin_unlock_irqrestore(&events->pmu_lock, flags);
}

static int
xscale1pmu_get_event_idx(struct pmu_hw_events *cpuc,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
			struct hw_perf_event *event)
{
	if (XSCALE_PERFCTR_CCNT == event->config_base) {
=======
				struct perf_event *event)
{
	struct hw_perf_event *hwc = &event->hw;
	if (XSCALE_PERFCTR_CCNT == hwc->config_base) {
>>>>>>> refs/remotes/origin/master
		if (test_and_set_bit(XSCALE_CYCLE_COUNTER, cpuc->used_mask))
			return -EAGAIN;

		return XSCALE_CYCLE_COUNTER;
	} else {
		if (!test_and_set_bit(XSCALE_COUNTER1, cpuc->used_mask))
			return XSCALE_COUNTER1;

		if (!test_and_set_bit(XSCALE_COUNTER0, cpuc->used_mask))
			return XSCALE_COUNTER0;

		return -EAGAIN;
	}
}

<<<<<<< HEAD
static void
xscale1pmu_start(void)
{
	unsigned long flags, val;
<<<<<<< HEAD

	raw_spin_lock_irqsave(&pmu_lock, flags);
	val = xscale1pmu_read_pmnc();
	val |= XSCALE_PMU_ENABLE;
	xscale1pmu_write_pmnc(val);
	raw_spin_unlock_irqrestore(&pmu_lock, flags);
=======
=======
static void xscale1pmu_start(struct arm_pmu *cpu_pmu)
{
	unsigned long flags, val;
>>>>>>> refs/remotes/origin/master
	struct pmu_hw_events *events = cpu_pmu->get_hw_events();

	raw_spin_lock_irqsave(&events->pmu_lock, flags);
	val = xscale1pmu_read_pmnc();
	val |= XSCALE_PMU_ENABLE;
	xscale1pmu_write_pmnc(val);
	raw_spin_unlock_irqrestore(&events->pmu_lock, flags);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

static void
xscale1pmu_stop(void)
{
	unsigned long flags, val;
<<<<<<< HEAD

	raw_spin_lock_irqsave(&pmu_lock, flags);
	val = xscale1pmu_read_pmnc();
	val &= ~XSCALE_PMU_ENABLE;
	xscale1pmu_write_pmnc(val);
	raw_spin_unlock_irqrestore(&pmu_lock, flags);
=======
=======
}

static void xscale1pmu_stop(struct arm_pmu *cpu_pmu)
{
	unsigned long flags, val;
>>>>>>> refs/remotes/origin/master
	struct pmu_hw_events *events = cpu_pmu->get_hw_events();

	raw_spin_lock_irqsave(&events->pmu_lock, flags);
	val = xscale1pmu_read_pmnc();
	val &= ~XSCALE_PMU_ENABLE;
	xscale1pmu_write_pmnc(val);
	raw_spin_unlock_irqrestore(&events->pmu_lock, flags);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

static inline u32
xscale1pmu_read_counter(int counter)
{
=======
}

static inline u32 xscale1pmu_read_counter(struct perf_event *event)
{
	struct hw_perf_event *hwc = &event->hw;
	int counter = hwc->idx;
>>>>>>> refs/remotes/origin/master
	u32 val = 0;

	switch (counter) {
	case XSCALE_CYCLE_COUNTER:
		asm volatile("mrc p14, 0, %0, c1, c0, 0" : "=r" (val));
		break;
	case XSCALE_COUNTER0:
		asm volatile("mrc p14, 0, %0, c2, c0, 0" : "=r" (val));
		break;
	case XSCALE_COUNTER1:
		asm volatile("mrc p14, 0, %0, c3, c0, 0" : "=r" (val));
		break;
	}

	return val;
}

<<<<<<< HEAD
static inline void
xscale1pmu_write_counter(int counter, u32 val)
{
=======
static inline void xscale1pmu_write_counter(struct perf_event *event, u32 val)
{
	struct hw_perf_event *hwc = &event->hw;
	int counter = hwc->idx;

>>>>>>> refs/remotes/origin/master
	switch (counter) {
	case XSCALE_CYCLE_COUNTER:
		asm volatile("mcr p14, 0, %0, c1, c0, 0" : : "r" (val));
		break;
	case XSCALE_COUNTER0:
		asm volatile("mcr p14, 0, %0, c2, c0, 0" : : "r" (val));
		break;
	case XSCALE_COUNTER1:
		asm volatile("mcr p14, 0, %0, c3, c0, 0" : : "r" (val));
		break;
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static const struct arm_pmu xscale1pmu = {
=======
static int xscale_map_event(struct perf_event *event)
{
	return map_cpu_event(event, &xscale_perf_map,
				&xscale_perf_cache_map, 0xFF);
}

static struct arm_pmu xscale1pmu = {
>>>>>>> refs/remotes/origin/cm-10.0
	.id		= ARM_PERF_PMU_ID_XSCALE1,
	.name		= "xscale1",
	.handle_irq	= xscale1pmu_handle_irq,
	.enable		= xscale1pmu_enable_event,
	.disable	= xscale1pmu_disable_event,
	.read_counter	= xscale1pmu_read_counter,
	.write_counter	= xscale1pmu_write_counter,
	.get_event_idx	= xscale1pmu_get_event_idx,
	.start		= xscale1pmu_start,
	.stop		= xscale1pmu_stop,
<<<<<<< HEAD
	.cache_map	= &xscale_perf_cache_map,
	.event_map	= &xscale_perf_map,
	.raw_event_mask	= 0xFF,
=======
	.map_event	= xscale_map_event,
>>>>>>> refs/remotes/origin/cm-10.0
	.num_events	= 3,
	.max_period	= (1LLU << 32) - 1,
};

<<<<<<< HEAD
static const struct arm_pmu *__init xscale1pmu_init(void)
=======
static struct arm_pmu *__init xscale1pmu_init(void)
>>>>>>> refs/remotes/origin/cm-10.0
{
	return &xscale1pmu;
=======
static int xscale_map_event(struct perf_event *event)
{
	return armpmu_map_event(event, &xscale_perf_map,
				&xscale_perf_cache_map, 0xFF);
}

static int xscale1pmu_init(struct arm_pmu *cpu_pmu)
{
	cpu_pmu->name		= "xscale1";
	cpu_pmu->handle_irq	= xscale1pmu_handle_irq;
	cpu_pmu->enable		= xscale1pmu_enable_event;
	cpu_pmu->disable	= xscale1pmu_disable_event;
	cpu_pmu->read_counter	= xscale1pmu_read_counter;
	cpu_pmu->write_counter	= xscale1pmu_write_counter;
	cpu_pmu->get_event_idx	= xscale1pmu_get_event_idx;
	cpu_pmu->start		= xscale1pmu_start;
	cpu_pmu->stop		= xscale1pmu_stop;
	cpu_pmu->map_event	= xscale_map_event;
	cpu_pmu->num_events	= 3;
	cpu_pmu->max_period	= (1LLU << 32) - 1;

	return 0;
>>>>>>> refs/remotes/origin/master
}

#define XSCALE2_OVERFLOWED_MASK	0x01f
#define XSCALE2_CCOUNT_OVERFLOW	0x001
#define XSCALE2_COUNT0_OVERFLOW	0x002
#define XSCALE2_COUNT1_OVERFLOW	0x004
#define XSCALE2_COUNT2_OVERFLOW	0x008
#define XSCALE2_COUNT3_OVERFLOW	0x010
#define XSCALE2_CCOUNT_INT_EN	0x001
#define XSCALE2_COUNT0_INT_EN	0x002
#define XSCALE2_COUNT1_INT_EN	0x004
#define XSCALE2_COUNT2_INT_EN	0x008
#define XSCALE2_COUNT3_INT_EN	0x010
#define XSCALE2_COUNT0_EVT_SHFT	0
#define XSCALE2_COUNT0_EVT_MASK	(0xff << XSCALE2_COUNT0_EVT_SHFT)
#define XSCALE2_COUNT1_EVT_SHFT	8
#define XSCALE2_COUNT1_EVT_MASK	(0xff << XSCALE2_COUNT1_EVT_SHFT)
#define XSCALE2_COUNT2_EVT_SHFT	16
#define XSCALE2_COUNT2_EVT_MASK	(0xff << XSCALE2_COUNT2_EVT_SHFT)
#define XSCALE2_COUNT3_EVT_SHFT	24
#define XSCALE2_COUNT3_EVT_MASK	(0xff << XSCALE2_COUNT3_EVT_SHFT)

static inline u32
xscale2pmu_read_pmnc(void)
{
	u32 val;
	asm volatile("mrc p14, 0, %0, c0, c1, 0" : "=r" (val));
	/* bits 1-2 and 4-23 are read-unpredictable */
	return val & 0xff000009;
}

static inline void
xscale2pmu_write_pmnc(u32 val)
{
	/* bits 4-23 are write-as-0, 24-31 are write ignored */
	val &= 0xf;
	asm volatile("mcr p14, 0, %0, c0, c1, 0" : : "r" (val));
}

static inline u32
xscale2pmu_read_overflow_flags(void)
{
	u32 val;
	asm volatile("mrc p14, 0, %0, c5, c1, 0" : "=r" (val));
	return val;
}

static inline void
xscale2pmu_write_overflow_flags(u32 val)
{
	asm volatile("mcr p14, 0, %0, c5, c1, 0" : : "r" (val));
}

static inline u32
xscale2pmu_read_event_select(void)
{
	u32 val;
	asm volatile("mrc p14, 0, %0, c8, c1, 0" : "=r" (val));
	return val;
}

static inline void
xscale2pmu_write_event_select(u32 val)
{
	asm volatile("mcr p14, 0, %0, c8, c1, 0" : : "r"(val));
}

static inline u32
xscale2pmu_read_int_enable(void)
{
	u32 val;
	asm volatile("mrc p14, 0, %0, c4, c1, 0" : "=r" (val));
	return val;
}

static void
xscale2pmu_write_int_enable(u32 val)
{
	asm volatile("mcr p14, 0, %0, c4, c1, 0" : : "r" (val));
}

static inline int
xscale2_pmnc_counter_has_overflowed(unsigned long of_flags,
					enum xscale_counters counter)
{
	int ret = 0;

	switch (counter) {
	case XSCALE_CYCLE_COUNTER:
		ret = of_flags & XSCALE2_CCOUNT_OVERFLOW;
		break;
	case XSCALE_COUNTER0:
		ret = of_flags & XSCALE2_COUNT0_OVERFLOW;
		break;
	case XSCALE_COUNTER1:
		ret = of_flags & XSCALE2_COUNT1_OVERFLOW;
		break;
	case XSCALE_COUNTER2:
		ret = of_flags & XSCALE2_COUNT2_OVERFLOW;
		break;
	case XSCALE_COUNTER3:
		ret = of_flags & XSCALE2_COUNT3_OVERFLOW;
		break;
	default:
		WARN_ONCE(1, "invalid counter number (%d)\n", counter);
	}

	return ret;
}

static irqreturn_t
xscale2pmu_handle_irq(int irq_num, void *dev)
{
	unsigned long pmnc, of_flags;
	struct perf_sample_data data;
<<<<<<< HEAD
<<<<<<< HEAD
	struct cpu_hw_events *cpuc;
=======
	struct pmu_hw_events *cpuc;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct arm_pmu *cpu_pmu = (struct arm_pmu *)dev;
	struct pmu_hw_events *cpuc = cpu_pmu->get_hw_events();
>>>>>>> refs/remotes/origin/master
	struct pt_regs *regs;
	int idx;

	/* Disable the PMU. */
	pmnc = xscale2pmu_read_pmnc();
	xscale2pmu_write_pmnc(pmnc & ~XSCALE_PMU_ENABLE);

	/* Check the overflow flag register. */
	of_flags = xscale2pmu_read_overflow_flags();
	if (!(of_flags & XSCALE2_OVERFLOWED_MASK))
		return IRQ_NONE;

	/* Clear the overflow bits. */
	xscale2pmu_write_overflow_flags(of_flags);

	regs = get_irq_regs();

<<<<<<< HEAD
	perf_sample_data_init(&data, 0);

	cpuc = &__get_cpu_var(cpu_hw_events);
<<<<<<< HEAD
	for (idx = 0; idx <= armpmu->num_events; ++idx) {
		struct perf_event *event = cpuc->events[idx];
		struct hw_perf_event *hwc;

		if (!test_bit(idx, cpuc->active_mask))
			continue;

		if (!xscale2_pmnc_counter_has_overflowed(pmnc, idx))
			continue;

		hwc = &event->hw;
		armpmu_event_update(event, hwc, idx, 1);
=======
=======
>>>>>>> refs/remotes/origin/master
	for (idx = 0; idx < cpu_pmu->num_events; ++idx) {
		struct perf_event *event = cpuc->events[idx];
		struct hw_perf_event *hwc;

		if (!event)
			continue;

		if (!xscale2_pmnc_counter_has_overflowed(of_flags, idx))
			continue;

		hwc = &event->hw;
<<<<<<< HEAD
		armpmu_event_update(event, hwc, idx);
>>>>>>> refs/remotes/origin/cm-10.0
		data.period = event->hw.last_period;
		if (!armpmu_event_set_period(event, hwc, idx))
			continue;

<<<<<<< HEAD
		if (perf_event_overflow(event, 0, &data, regs))
			armpmu->disable(hwc, idx);
=======
		if (perf_event_overflow(event, &data, regs))
			cpu_pmu->disable(hwc, idx);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		armpmu_event_update(event);
		perf_sample_data_init(&data, 0, hwc->last_period);
		if (!armpmu_event_set_period(event))
			continue;

		if (perf_event_overflow(event, &data, regs))
			cpu_pmu->disable(event);
>>>>>>> refs/remotes/origin/master
	}

	irq_work_run();

	/*
	 * Re-enable the PMU.
	 */
	pmnc = xscale2pmu_read_pmnc() | XSCALE_PMU_ENABLE;
	xscale2pmu_write_pmnc(pmnc);

	return IRQ_HANDLED;
}

<<<<<<< HEAD
static void
xscale2pmu_enable_event(struct hw_perf_event *hwc, int idx)
{
	unsigned long flags, ien, evtsel;
<<<<<<< HEAD
=======
	struct pmu_hw_events *events = cpu_pmu->get_hw_events();
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void xscale2pmu_enable_event(struct perf_event *event)
{
	unsigned long flags, ien, evtsel;
	struct arm_pmu *cpu_pmu = to_arm_pmu(event->pmu);
	struct hw_perf_event *hwc = &event->hw;
	struct pmu_hw_events *events = cpu_pmu->get_hw_events();
	int idx = hwc->idx;
>>>>>>> refs/remotes/origin/master

	ien = xscale2pmu_read_int_enable();
	evtsel = xscale2pmu_read_event_select();

	switch (idx) {
	case XSCALE_CYCLE_COUNTER:
		ien |= XSCALE2_CCOUNT_INT_EN;
		break;
	case XSCALE_COUNTER0:
		ien |= XSCALE2_COUNT0_INT_EN;
		evtsel &= ~XSCALE2_COUNT0_EVT_MASK;
		evtsel |= hwc->config_base << XSCALE2_COUNT0_EVT_SHFT;
		break;
	case XSCALE_COUNTER1:
		ien |= XSCALE2_COUNT1_INT_EN;
		evtsel &= ~XSCALE2_COUNT1_EVT_MASK;
		evtsel |= hwc->config_base << XSCALE2_COUNT1_EVT_SHFT;
		break;
	case XSCALE_COUNTER2:
		ien |= XSCALE2_COUNT2_INT_EN;
		evtsel &= ~XSCALE2_COUNT2_EVT_MASK;
		evtsel |= hwc->config_base << XSCALE2_COUNT2_EVT_SHFT;
		break;
	case XSCALE_COUNTER3:
		ien |= XSCALE2_COUNT3_INT_EN;
		evtsel &= ~XSCALE2_COUNT3_EVT_MASK;
		evtsel |= hwc->config_base << XSCALE2_COUNT3_EVT_SHFT;
		break;
	default:
		WARN_ONCE(1, "invalid counter number (%d)\n", idx);
		return;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	raw_spin_lock_irqsave(&pmu_lock, flags);
	xscale2pmu_write_event_select(evtsel);
	xscale2pmu_write_int_enable(ien);
	raw_spin_unlock_irqrestore(&pmu_lock, flags);
=======
=======
>>>>>>> refs/remotes/origin/master
	raw_spin_lock_irqsave(&events->pmu_lock, flags);
	xscale2pmu_write_event_select(evtsel);
	xscale2pmu_write_int_enable(ien);
	raw_spin_unlock_irqrestore(&events->pmu_lock, flags);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

static void
xscale2pmu_disable_event(struct hw_perf_event *hwc, int idx)
{
<<<<<<< HEAD
	unsigned long flags, ien, evtsel;
=======
	unsigned long flags, ien, evtsel, of_flags;
	struct pmu_hw_events *events = cpu_pmu->get_hw_events();
>>>>>>> refs/remotes/origin/cm-10.0
=======
}

static void xscale2pmu_disable_event(struct perf_event *event)
{
	unsigned long flags, ien, evtsel, of_flags;
	struct arm_pmu *cpu_pmu = to_arm_pmu(event->pmu);
	struct hw_perf_event *hwc = &event->hw;
	struct pmu_hw_events *events = cpu_pmu->get_hw_events();
	int idx = hwc->idx;
>>>>>>> refs/remotes/origin/master

	ien = xscale2pmu_read_int_enable();
	evtsel = xscale2pmu_read_event_select();

	switch (idx) {
	case XSCALE_CYCLE_COUNTER:
		ien &= ~XSCALE2_CCOUNT_INT_EN;
<<<<<<< HEAD
<<<<<<< HEAD
=======
		of_flags = XSCALE2_CCOUNT_OVERFLOW;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		of_flags = XSCALE2_CCOUNT_OVERFLOW;
>>>>>>> refs/remotes/origin/master
		break;
	case XSCALE_COUNTER0:
		ien &= ~XSCALE2_COUNT0_INT_EN;
		evtsel &= ~XSCALE2_COUNT0_EVT_MASK;
		evtsel |= XSCALE_PERFCTR_UNUSED << XSCALE2_COUNT0_EVT_SHFT;
<<<<<<< HEAD
<<<<<<< HEAD
=======
		of_flags = XSCALE2_COUNT0_OVERFLOW;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		of_flags = XSCALE2_COUNT0_OVERFLOW;
>>>>>>> refs/remotes/origin/master
		break;
	case XSCALE_COUNTER1:
		ien &= ~XSCALE2_COUNT1_INT_EN;
		evtsel &= ~XSCALE2_COUNT1_EVT_MASK;
		evtsel |= XSCALE_PERFCTR_UNUSED << XSCALE2_COUNT1_EVT_SHFT;
<<<<<<< HEAD
<<<<<<< HEAD
=======
		of_flags = XSCALE2_COUNT1_OVERFLOW;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		of_flags = XSCALE2_COUNT1_OVERFLOW;
>>>>>>> refs/remotes/origin/master
		break;
	case XSCALE_COUNTER2:
		ien &= ~XSCALE2_COUNT2_INT_EN;
		evtsel &= ~XSCALE2_COUNT2_EVT_MASK;
		evtsel |= XSCALE_PERFCTR_UNUSED << XSCALE2_COUNT2_EVT_SHFT;
<<<<<<< HEAD
<<<<<<< HEAD
=======
		of_flags = XSCALE2_COUNT2_OVERFLOW;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		of_flags = XSCALE2_COUNT2_OVERFLOW;
>>>>>>> refs/remotes/origin/master
		break;
	case XSCALE_COUNTER3:
		ien &= ~XSCALE2_COUNT3_INT_EN;
		evtsel &= ~XSCALE2_COUNT3_EVT_MASK;
		evtsel |= XSCALE_PERFCTR_UNUSED << XSCALE2_COUNT3_EVT_SHFT;
<<<<<<< HEAD
<<<<<<< HEAD
=======
		of_flags = XSCALE2_COUNT3_OVERFLOW;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		of_flags = XSCALE2_COUNT3_OVERFLOW;
>>>>>>> refs/remotes/origin/master
		break;
	default:
		WARN_ONCE(1, "invalid counter number (%d)\n", idx);
		return;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	raw_spin_lock_irqsave(&pmu_lock, flags);
	xscale2pmu_write_event_select(evtsel);
	xscale2pmu_write_int_enable(ien);
	raw_spin_unlock_irqrestore(&pmu_lock, flags);
}

static int
xscale2pmu_get_event_idx(struct cpu_hw_events *cpuc,
=======
=======
>>>>>>> refs/remotes/origin/master
	raw_spin_lock_irqsave(&events->pmu_lock, flags);
	xscale2pmu_write_event_select(evtsel);
	xscale2pmu_write_int_enable(ien);
	xscale2pmu_write_overflow_flags(of_flags);
	raw_spin_unlock_irqrestore(&events->pmu_lock, flags);
}

static int
xscale2pmu_get_event_idx(struct pmu_hw_events *cpuc,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
			struct hw_perf_event *event)
=======
				struct perf_event *event)
>>>>>>> refs/remotes/origin/master
{
	int idx = xscale1pmu_get_event_idx(cpuc, event);
	if (idx >= 0)
		goto out;

	if (!test_and_set_bit(XSCALE_COUNTER3, cpuc->used_mask))
		idx = XSCALE_COUNTER3;
	else if (!test_and_set_bit(XSCALE_COUNTER2, cpuc->used_mask))
		idx = XSCALE_COUNTER2;
out:
	return idx;
}

<<<<<<< HEAD
static void
xscale2pmu_start(void)
{
	unsigned long flags, val;
<<<<<<< HEAD

	raw_spin_lock_irqsave(&pmu_lock, flags);
	val = xscale2pmu_read_pmnc() & ~XSCALE_PMU_CNT64;
	val |= XSCALE_PMU_ENABLE;
	xscale2pmu_write_pmnc(val);
	raw_spin_unlock_irqrestore(&pmu_lock, flags);
=======
=======
static void xscale2pmu_start(struct arm_pmu *cpu_pmu)
{
	unsigned long flags, val;
>>>>>>> refs/remotes/origin/master
	struct pmu_hw_events *events = cpu_pmu->get_hw_events();

	raw_spin_lock_irqsave(&events->pmu_lock, flags);
	val = xscale2pmu_read_pmnc() & ~XSCALE_PMU_CNT64;
	val |= XSCALE_PMU_ENABLE;
	xscale2pmu_write_pmnc(val);
	raw_spin_unlock_irqrestore(&events->pmu_lock, flags);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

static void
xscale2pmu_stop(void)
{
	unsigned long flags, val;
<<<<<<< HEAD

	raw_spin_lock_irqsave(&pmu_lock, flags);
	val = xscale2pmu_read_pmnc();
	val &= ~XSCALE_PMU_ENABLE;
	xscale2pmu_write_pmnc(val);
	raw_spin_unlock_irqrestore(&pmu_lock, flags);
=======
=======
}

static void xscale2pmu_stop(struct arm_pmu *cpu_pmu)
{
	unsigned long flags, val;
>>>>>>> refs/remotes/origin/master
	struct pmu_hw_events *events = cpu_pmu->get_hw_events();

	raw_spin_lock_irqsave(&events->pmu_lock, flags);
	val = xscale2pmu_read_pmnc();
	val &= ~XSCALE_PMU_ENABLE;
	xscale2pmu_write_pmnc(val);
	raw_spin_unlock_irqrestore(&events->pmu_lock, flags);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

static inline u32
xscale2pmu_read_counter(int counter)
{
=======
}

static inline u32 xscale2pmu_read_counter(struct perf_event *event)
{
	struct hw_perf_event *hwc = &event->hw;
	int counter = hwc->idx;
>>>>>>> refs/remotes/origin/master
	u32 val = 0;

	switch (counter) {
	case XSCALE_CYCLE_COUNTER:
		asm volatile("mrc p14, 0, %0, c1, c1, 0" : "=r" (val));
		break;
	case XSCALE_COUNTER0:
		asm volatile("mrc p14, 0, %0, c0, c2, 0" : "=r" (val));
		break;
	case XSCALE_COUNTER1:
		asm volatile("mrc p14, 0, %0, c1, c2, 0" : "=r" (val));
		break;
	case XSCALE_COUNTER2:
		asm volatile("mrc p14, 0, %0, c2, c2, 0" : "=r" (val));
		break;
	case XSCALE_COUNTER3:
		asm volatile("mrc p14, 0, %0, c3, c2, 0" : "=r" (val));
		break;
	}

	return val;
}

<<<<<<< HEAD
static inline void
xscale2pmu_write_counter(int counter, u32 val)
{
=======
static inline void xscale2pmu_write_counter(struct perf_event *event, u32 val)
{
	struct hw_perf_event *hwc = &event->hw;
	int counter = hwc->idx;

>>>>>>> refs/remotes/origin/master
	switch (counter) {
	case XSCALE_CYCLE_COUNTER:
		asm volatile("mcr p14, 0, %0, c1, c1, 0" : : "r" (val));
		break;
	case XSCALE_COUNTER0:
		asm volatile("mcr p14, 0, %0, c0, c2, 0" : : "r" (val));
		break;
	case XSCALE_COUNTER1:
		asm volatile("mcr p14, 0, %0, c1, c2, 0" : : "r" (val));
		break;
	case XSCALE_COUNTER2:
		asm volatile("mcr p14, 0, %0, c2, c2, 0" : : "r" (val));
		break;
	case XSCALE_COUNTER3:
		asm volatile("mcr p14, 0, %0, c3, c2, 0" : : "r" (val));
		break;
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static const struct arm_pmu xscale2pmu = {
=======
static struct arm_pmu xscale2pmu = {
>>>>>>> refs/remotes/origin/cm-10.0
	.id		= ARM_PERF_PMU_ID_XSCALE2,
	.name		= "xscale2",
	.handle_irq	= xscale2pmu_handle_irq,
	.request_pmu_irq = armpmu_generic_request_irq,
	.free_pmu_irq	= armpmu_generic_free_irq,
	.enable		= xscale2pmu_enable_event,
	.disable	= xscale2pmu_disable_event,
	.read_counter	= xscale2pmu_read_counter,
	.write_counter	= xscale2pmu_write_counter,
	.get_event_idx	= xscale2pmu_get_event_idx,
	.start		= xscale2pmu_start,
	.stop		= xscale2pmu_stop,
<<<<<<< HEAD
	.cache_map	= &xscale_perf_cache_map,
	.event_map	= &xscale_perf_map,
	.raw_event_mask	= 0xFF,
=======
	.map_event	= xscale_map_event,
>>>>>>> refs/remotes/origin/cm-10.0
	.num_events	= 5,
	.max_period	= (1LLU << 32) - 1,
};

<<<<<<< HEAD
static const struct arm_pmu *__init xscale2pmu_init(void)
=======
static struct arm_pmu *__init xscale2pmu_init(void)
>>>>>>> refs/remotes/origin/cm-10.0
{
	return &xscale2pmu;
}
#else
<<<<<<< HEAD
static const struct arm_pmu *__init xscale1pmu_init(void)
=======
static struct arm_pmu *__init xscale1pmu_init(void)
>>>>>>> refs/remotes/origin/cm-10.0
{
	return NULL;
}

<<<<<<< HEAD
static const struct arm_pmu *__init xscale2pmu_init(void)
=======
static struct arm_pmu *__init xscale2pmu_init(void)
>>>>>>> refs/remotes/origin/cm-10.0
{
	return NULL;
=======
static int xscale2pmu_init(struct arm_pmu *cpu_pmu)
{
	cpu_pmu->name		= "xscale2";
	cpu_pmu->handle_irq	= xscale2pmu_handle_irq;
	cpu_pmu->enable		= xscale2pmu_enable_event;
	cpu_pmu->disable	= xscale2pmu_disable_event;
	cpu_pmu->read_counter	= xscale2pmu_read_counter;
	cpu_pmu->write_counter	= xscale2pmu_write_counter;
	cpu_pmu->get_event_idx	= xscale2pmu_get_event_idx;
	cpu_pmu->start		= xscale2pmu_start;
	cpu_pmu->stop		= xscale2pmu_stop;
	cpu_pmu->map_event	= xscale_map_event;
	cpu_pmu->num_events	= 5;
	cpu_pmu->max_period	= (1LLU << 32) - 1;

	return 0;
}
#else
static inline int xscale1pmu_init(struct arm_pmu *cpu_pmu)
{
	return -ENODEV;
}

static inline int xscale2pmu_init(struct arm_pmu *cpu_pmu)
{
	return -ENODEV;
>>>>>>> refs/remotes/origin/master
}
#endif	/* CONFIG_CPU_XSCALE */
