/*
 *  linux/arch/arm/kernel/arch_timer.c
 *
 *  Copyright (C) 2011 ARM Ltd.
 *  All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/init.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/timex.h>
#include <linux/device.h>
#include <linux/smp.h>
#include <linux/cpu.h>
#include <linux/jiffies.h>
#include <linux/clockchips.h>
#include <linux/interrupt.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/of_irq.h>
#include <linux/of_address.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/of_irq.h>
#include <linux/of_address.h>
>>>>>>> refs/remotes/origin/cm-11.0
#include <linux/io.h>
#include <linux/irq.h>

#include <asm/cputype.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/sched_clock.h>
#include <asm/hardware/gic.h>
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
#include <asm/localtimer.h>
#include <asm/arch_timer.h>
#include <asm/sched_clock.h>
#include <asm/hardware/gic.h>
#include <asm/system_info.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0

static unsigned long arch_timer_rate;
static int arch_timer_ppi;
static int arch_timer_ppi2;
<<<<<<< HEAD
<<<<<<< HEAD
static DEFINE_CLOCK_DATA(cd);

static struct clock_event_device __percpu *arch_timer_evt;
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
static int is_irq_percpu;

static struct clock_event_device __percpu **arch_timer_evt;
static void __iomem *timer_base;

static u32 timer_reg_read_cp15(int reg);
static void timer_reg_write_cp15(int reg, u32 val);
static inline cycle_t counter_get_cntpct_cp15(void);
static inline cycle_t counter_get_cntvct_cp15(void);

static u32 timer_reg_read_mem(int reg);
static void timer_reg_write_mem(int reg, u32 val);
static inline cycle_t counter_get_cntpct_mem(void);
static inline cycle_t counter_get_cntvct_mem(void);

struct arch_timer_operations {
	void (*reg_write)(int, u32);
	u32 (*reg_read)(int);
	cycle_t (*get_cntpct)(void);
	cycle_t (*get_cntvct)(void);
};

static struct arch_timer_operations arch_timer_ops_cp15 = {
	.reg_read = &timer_reg_read_cp15,
	.reg_write = &timer_reg_write_cp15,
	.get_cntpct = &counter_get_cntpct_cp15,
	.get_cntvct = &counter_get_cntvct_cp15,
};

static struct arch_timer_operations arch_timer_ops_mem = {
	.reg_read = &timer_reg_read_mem,
	.reg_write = &timer_reg_write_mem,
	.get_cntpct = &counter_get_cntpct_mem,
	.get_cntvct = &counter_get_cntvct_mem,
};

static struct arch_timer_operations *arch_specific_timer = &arch_timer_ops_cp15;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0

/*
 * Architected system timer support.
 */

#define ARCH_TIMER_CTRL_ENABLE		(1 << 0)
#define ARCH_TIMER_CTRL_IT_MASK		(1 << 1)
<<<<<<< HEAD
<<<<<<< HEAD
=======
#define ARCH_TIMER_CTRL_IT_STAT		(1 << 2)
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define ARCH_TIMER_CTRL_IT_STAT		(1 << 2)
>>>>>>> refs/remotes/origin/cm-11.0

#define ARCH_TIMER_REG_CTRL		0
#define ARCH_TIMER_REG_FREQ		1
#define ARCH_TIMER_REG_TVAL		2

<<<<<<< HEAD
<<<<<<< HEAD
static void arch_timer_reg_write(int reg, u32 val)
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
/* Iomapped Register Offsets */
#define QTIMER_CNTP_LOW_REG		0x000
#define QTIMER_CNTP_HIGH_REG		0x004
#define QTIMER_CNTV_LOW_REG		0x008
#define QTIMER_CNTV_HIGH_REG		0x00C
#define QTIMER_CTRL_REG			0x02C
#define QTIMER_FREQ_REG			0x010
#define QTIMER_CNTP_TVAL_REG		0x028
#define QTIMER_CNTV_TVAL_REG		0x038

static void timer_reg_write_mem(int reg, u32 val)
{
	switch (reg) {
	case ARCH_TIMER_REG_CTRL:
		__raw_writel(val, timer_base + QTIMER_CTRL_REG);
		break;
	case ARCH_TIMER_REG_TVAL:
		__raw_writel(val, timer_base + QTIMER_CNTP_TVAL_REG);
		break;
	}
}

static void timer_reg_write_cp15(int reg, u32 val)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
{
	switch (reg) {
	case ARCH_TIMER_REG_CTRL:
		asm volatile("mcr p15, 0, %0, c14, c2, 1" : : "r" (val));
		break;
	case ARCH_TIMER_REG_TVAL:
		asm volatile("mcr p15, 0, %0, c14, c2, 0" : : "r" (val));
		break;
	}

	isb();
}

<<<<<<< HEAD
<<<<<<< HEAD
static u32 arch_timer_reg_read(int reg)
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
static u32 timer_reg_read_mem(int reg)
{
	u32 val;

	switch (reg) {
	case ARCH_TIMER_REG_CTRL:
		val = __raw_readl(timer_base + QTIMER_CTRL_REG);
		break;
	case ARCH_TIMER_REG_FREQ:
		val = __raw_readl(timer_base + QTIMER_FREQ_REG);
		break;
	case ARCH_TIMER_REG_TVAL:
		val = __raw_readl(timer_base + QTIMER_CNTP_TVAL_REG);
		break;
	default:
		BUG();
	}

	return val;
}

static u32 timer_reg_read_cp15(int reg)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
{
	u32 val;

	switch (reg) {
	case ARCH_TIMER_REG_CTRL:
		asm volatile("mrc p15, 0, %0, c14, c2, 1" : "=r" (val));
		break;
	case ARCH_TIMER_REG_FREQ:
		asm volatile("mrc p15, 0, %0, c14, c0, 0" : "=r" (val));
		break;
	case ARCH_TIMER_REG_TVAL:
		asm volatile("mrc p15, 0, %0, c14, c2, 0" : "=r" (val));
		break;
	default:
		BUG();
	}

	return val;
}

static irqreturn_t arch_timer_handler(int irq, void *dev_id)
{
	struct clock_event_device *evt;
	unsigned long ctrl;

<<<<<<< HEAD
<<<<<<< HEAD
	ctrl = arch_timer_reg_read(ARCH_TIMER_REG_CTRL);
	if (ctrl & 0x4) {
		ctrl |= ARCH_TIMER_CTRL_IT_MASK;
		arch_timer_reg_write(ARCH_TIMER_REG_CTRL, ctrl);
		evt = per_cpu_ptr(arch_timer_evt, smp_processor_id());
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	ctrl = arch_specific_timer->reg_read(ARCH_TIMER_REG_CTRL);
	if (ctrl & ARCH_TIMER_CTRL_IT_STAT) {
		ctrl |= ARCH_TIMER_CTRL_IT_MASK;
		arch_specific_timer->reg_write(ARCH_TIMER_REG_CTRL,
							ctrl);
		evt = *__this_cpu_ptr(arch_timer_evt);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
		evt->event_handler(evt);
		return IRQ_HANDLED;
	}

	return IRQ_NONE;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void arch_timer_stop(void)
{
	unsigned long ctrl;

	ctrl = arch_timer_reg_read(ARCH_TIMER_REG_CTRL);
	ctrl &= ~ARCH_TIMER_CTRL_ENABLE;
	arch_timer_reg_write(ARCH_TIMER_REG_CTRL, ctrl);
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
static void arch_timer_disable(void)
{
	unsigned long ctrl;

	ctrl = arch_specific_timer->reg_read(ARCH_TIMER_REG_CTRL);
	ctrl &= ~ARCH_TIMER_CTRL_ENABLE;
	arch_specific_timer->reg_write(ARCH_TIMER_REG_CTRL, ctrl);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
}

static void arch_timer_set_mode(enum clock_event_mode mode,
				struct clock_event_device *clk)
{
	switch (mode) {
	case CLOCK_EVT_MODE_UNUSED:
	case CLOCK_EVT_MODE_SHUTDOWN:
<<<<<<< HEAD
<<<<<<< HEAD
		arch_timer_stop();
=======
		arch_timer_disable();
>>>>>>> refs/remotes/origin/cm-10.0
=======
		arch_timer_disable();
>>>>>>> refs/remotes/origin/cm-11.0
		break;
	default:
		break;
	}
}

static int arch_timer_set_next_event(unsigned long evt,
				     struct clock_event_device *unused)
{
	unsigned long ctrl;

<<<<<<< HEAD
<<<<<<< HEAD
	ctrl = arch_timer_reg_read(ARCH_TIMER_REG_CTRL);
	ctrl |= ARCH_TIMER_CTRL_ENABLE;
	ctrl &= ~ARCH_TIMER_CTRL_IT_MASK;

	arch_timer_reg_write(ARCH_TIMER_REG_CTRL, ctrl);
	arch_timer_reg_write(ARCH_TIMER_REG_TVAL, evt);
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	ctrl = arch_specific_timer->reg_read(ARCH_TIMER_REG_CTRL);
	ctrl |= ARCH_TIMER_CTRL_ENABLE;
	ctrl &= ~ARCH_TIMER_CTRL_IT_MASK;

	arch_specific_timer->reg_write(ARCH_TIMER_REG_CTRL, ctrl);
	arch_specific_timer->reg_write(ARCH_TIMER_REG_TVAL, evt);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void __cpuinit arch_timer_setup(void *data)
{
	struct clock_event_device *clk = data;

	/* Be safe... */
	arch_timer_stop();
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
static int __cpuinit arch_timer_setup(struct clock_event_device *clk)
{
	/* setup clock event only once for CPU 0 */
	if (!smp_processor_id() && clk->irq == arch_timer_ppi)
		return 0;

	/* Be safe... */
	arch_timer_disable();
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0

	clk->features = CLOCK_EVT_FEAT_ONESHOT;
	clk->name = "arch_sys_timer";
	clk->rating = 450;
	clk->set_mode = arch_timer_set_mode;
	clk->set_next_event = arch_timer_set_next_event;
	clk->irq = arch_timer_ppi;
<<<<<<< HEAD
<<<<<<< HEAD
	clk->cpumask = cpumask_of(smp_processor_id());
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0

	clockevents_config_and_register(clk, arch_timer_rate,
					0xf, 0x7fffffff);

<<<<<<< HEAD
<<<<<<< HEAD
	enable_percpu_irq(arch_timer_ppi, 0);
	if (arch_timer_ppi2 > 0)
		enable_percpu_irq(arch_timer_ppi2, 0);
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	*__this_cpu_ptr(arch_timer_evt) = clk;

	enable_percpu_irq(clk->irq, 0);
	if (arch_timer_ppi2)
		enable_percpu_irq(arch_timer_ppi2, 0);

	return 0;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
}

/* Is the optional system timer available? */
static int local_timer_is_architected(void)
{
	return (cpu_architecture() >= CPU_ARCH_ARMv7) &&
<<<<<<< HEAD
<<<<<<< HEAD
	       ((read_cpuid_ext(CPUID_EXT_PFR1) >> 16) & 0xf) == 1;
=======
		((read_cpuid_ext(CPUID_EXT_PFR1) >> 16) & 0xf) == 1;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		((read_cpuid_ext(CPUID_EXT_PFR1) >> 16) & 0xf) == 1;
>>>>>>> refs/remotes/origin/cm-11.0
}

static int arch_timer_available(void)
{
	unsigned long freq;

<<<<<<< HEAD
<<<<<<< HEAD
	if (!local_timer_is_architected())
		return -ENXIO;

	if (arch_timer_rate == 0) {
		arch_timer_reg_write(ARCH_TIMER_REG_CTRL, 0);
		freq = arch_timer_reg_read(ARCH_TIMER_REG_FREQ);
=======
	if (arch_timer_rate == 0) {
		arch_specific_timer->reg_write(ARCH_TIMER_REG_CTRL, 0);
		freq = arch_specific_timer->reg_read(ARCH_TIMER_REG_FREQ);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (arch_timer_rate == 0) {
		arch_specific_timer->reg_write(ARCH_TIMER_REG_CTRL, 0);
		freq = arch_specific_timer->reg_read(ARCH_TIMER_REG_FREQ);
>>>>>>> refs/remotes/origin/cm-11.0

		/* Check the timer frequency. */
		if (freq == 0) {
			pr_warn("Architected timer frequency not available\n");
			return -EINVAL;
		}

		arch_timer_rate = freq;
		pr_info("Architected local timer running at %lu.%02luMHz.\n",
<<<<<<< HEAD
<<<<<<< HEAD
			arch_timer_rate / 1000000, (arch_timer_rate % 100000) / 100);
=======
			freq / 1000000, (freq / 10000) % 100);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			freq / 1000000, (freq / 10000) % 100);
>>>>>>> refs/remotes/origin/cm-11.0
	}

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline cycle_t arch_counter_get_cntpct(void)
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
static inline cycle_t counter_get_cntpct_mem(void)
{
	u32 cvall, cvalh, thigh;

	do {
		cvalh = __raw_readl(timer_base + QTIMER_CNTP_HIGH_REG);
		cvall = __raw_readl(timer_base + QTIMER_CNTP_LOW_REG);
		thigh = __raw_readl(timer_base + QTIMER_CNTP_HIGH_REG);
	} while (cvalh != thigh);

	return ((cycle_t) cvalh << 32) | cvall;
}

static inline cycle_t counter_get_cntpct_cp15(void)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
{
	u32 cvall, cvalh;

	asm volatile("mrrc p15, 0, %0, %1, c14" : "=r" (cvall), "=r" (cvalh));
<<<<<<< HEAD
<<<<<<< HEAD

	return ((u64) cvalh << 32) | cvall;
}

static inline cycle_t arch_counter_get_cntvct(void)
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	return ((cycle_t) cvalh << 32) | cvall;
}

static inline cycle_t counter_get_cntvct_mem(void)
{
	u32 cvall, cvalh, thigh;

	do {
		cvalh = __raw_readl(timer_base + QTIMER_CNTV_HIGH_REG);
		cvall = __raw_readl(timer_base + QTIMER_CNTV_LOW_REG);
		thigh = __raw_readl(timer_base + QTIMER_CNTV_HIGH_REG);
	} while (cvalh != thigh);

	return ((cycle_t) cvalh << 32) | cvall;
}

static inline cycle_t counter_get_cntvct_cp15(void)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
{
	u32 cvall, cvalh;

	asm volatile("mrrc p15, 1, %0, %1, c14" : "=r" (cvall), "=r" (cvalh));
<<<<<<< HEAD
<<<<<<< HEAD

	return ((u64) cvalh << 32) | cvall;
=======
	return ((cycle_t) cvalh << 32) | cvall;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return ((cycle_t) cvalh << 32) | cvall;
>>>>>>> refs/remotes/origin/cm-11.0
}

static cycle_t arch_counter_read(struct clocksource *cs)
{
<<<<<<< HEAD
<<<<<<< HEAD
	return arch_counter_get_cntpct();
=======
	return arch_specific_timer->get_cntpct();
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return arch_specific_timer->get_cntpct();
>>>>>>> refs/remotes/origin/cm-11.0
}

#ifdef ARCH_HAS_READ_CURRENT_TIMER
int read_current_timer(unsigned long *timer_val)
{
<<<<<<< HEAD
<<<<<<< HEAD
	*timer_val = (unsigned long)arch_counter_get_cntpct();
=======
	*timer_val = (unsigned long)arch_specific_timer->get_cntpct();
>>>>>>> refs/remotes/origin/cm-10.0
=======
	*timer_val = (unsigned long)arch_specific_timer->get_cntpct();
>>>>>>> refs/remotes/origin/cm-11.0
	return 0;
}
#endif

static struct clocksource clocksource_counter = {
	.name	= "arch_sys_counter",
	.rating	= 400,
	.read	= arch_counter_read,
	.mask	= CLOCKSOURCE_MASK(56),
	.flags	= CLOCK_SOURCE_IS_CONTINUOUS,
};

static u32 arch_counter_get_cntvct32(void)
{
	cycle_t cntvct;

<<<<<<< HEAD
<<<<<<< HEAD
	cntvct = arch_counter_get_cntvct();
=======
	cntvct = arch_specific_timer->get_cntvct();
>>>>>>> refs/remotes/origin/cm-10.0
=======
	cntvct = arch_specific_timer->get_cntvct();
>>>>>>> refs/remotes/origin/cm-11.0

	/*
	 * The sched_clock infrastructure only knows about counters
	 * with at most 32bits. Forget about the upper 24 bits for the
	 * time being...
	 */
	return (u32)(cntvct & (u32)~0);
}

<<<<<<< HEAD
<<<<<<< HEAD
unsigned long long notrace sched_clock(void)
{
	return cyc_to_sched_clock(&cd, arch_counter_get_cntvct32(), (u32)~0);
}

static void notrace arch_timer_update_sched_clock(void)
{
	update_sched_clock(&cd, arch_counter_get_cntvct32(), (u32)~0);
}

static void __cpuinit arch_timer_teardown(void *data)
{
	struct clock_event_device *clk = data;
	pr_debug("arch_timer_teardown disable IRQ%d cpu #%d\n",
		 clk->irq, smp_processor_id());
	disable_percpu_irq(arch_timer_ppi);
	if (arch_timer_ppi2 > 0)
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
static u32 notrace arch_timer_update_sched_clock(void)
{
	return arch_counter_get_cntvct32();
}

static void __cpuinit arch_timer_stop(struct clock_event_device *clk)
{
	pr_debug("arch_timer_teardown disable IRQ%d cpu #%d\n",
		 clk->irq, smp_processor_id());
	disable_percpu_irq(clk->irq);
	if (arch_timer_ppi2)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
		disable_percpu_irq(arch_timer_ppi2);
	arch_timer_set_mode(CLOCK_EVT_MODE_UNUSED, clk);
}

<<<<<<< HEAD
<<<<<<< HEAD
static int __cpuinit arch_timer_cpu_notify(struct notifier_block *self,
					   unsigned long action, void *data)
{
	int cpu = (int)data;
	struct clock_event_device *clk = per_cpu_ptr(arch_timer_evt, cpu);

	switch(action) {
	case CPU_ONLINE:
	case CPU_ONLINE_FROZEN:
		smp_call_function_single(cpu, arch_timer_setup, clk, 1);
		break;

	case CPU_DOWN_PREPARE:
	case CPU_DOWN_PREPARE_FROZEN:
		smp_call_function_single(cpu, arch_timer_teardown, clk, 1);
		break;
	}

	return NOTIFY_OK;
}

static struct notifier_block __cpuinitdata arch_timer_cpu_nb = {
	.notifier_call = arch_timer_cpu_notify,
};

int __init arch_timer_register(struct resource *res, int res_nr)
{
	int err;

	if (!res_nr || res[0].start < 0 || !(res[0].flags & IORESOURCE_IRQ))
		return -EINVAL;
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
static struct local_timer_ops arch_timer_ops __cpuinitdata = {
	.setup	= arch_timer_setup,
	.stop	= arch_timer_stop,
};

static int __init arch_timer_common_register(void)
{
	int err;

	if (timer_base)
		arch_specific_timer = &arch_timer_ops_mem;
	else if (!local_timer_is_architected())
		return -ENXIO;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0

	err = arch_timer_available();
	if (err)
		return err;

<<<<<<< HEAD
<<<<<<< HEAD
	arch_timer_evt = alloc_percpu(struct clock_event_device);
	if (!arch_timer_evt)
		return -ENOMEM;

	arch_timer_ppi = res[0].start;
	if (res_nr > 1 && (res[1].flags & IORESOURCE_IRQ))
		arch_timer_ppi2 = res[1].start;

	clocksource_register_hz(&clocksource_counter, arch_timer_rate);

	init_sched_clock(&cd, arch_timer_update_sched_clock, 32,
			arch_timer_rate);
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	arch_timer_evt = alloc_percpu(struct clock_event_device *);
	if (!arch_timer_evt)
		return -ENOMEM;

	clocksource_register_hz(&clocksource_counter, arch_timer_rate);

	setup_sched_clock(arch_timer_update_sched_clock, 32, arch_timer_rate);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0

#ifdef ARCH_HAS_READ_CURRENT_TIMER
	set_delay_fn(read_current_timer_delay_loop);
#endif

<<<<<<< HEAD
<<<<<<< HEAD
	err = request_percpu_irq(arch_timer_ppi, arch_timer_handler,
				"arch_sys_timer", arch_timer_evt);
	if (err) {
		pr_err("%s: can't register interrupt %d (%d)\n",
		       "arch_sys_timer", arch_timer_ppi, err);
		return err;
	}

	if (arch_timer_ppi2 > 0) {
		err = request_percpu_irq(arch_timer_ppi2, arch_timer_handler,
					"arch_sys_timer", arch_timer_evt);
		if (err)
			pr_warn("%s: can't register interrupt %d (%d)\n",
				"arch_sys_timer", arch_timer_ppi2, err);
	}

	/* Immediately configure the timer on the boot CPU */
	arch_timer_setup(per_cpu_ptr(arch_timer_evt, smp_processor_id()));

	register_cpu_notifier(&arch_timer_cpu_nb);

	return 0;
}
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
	if (is_irq_percpu)
		err = request_percpu_irq(arch_timer_ppi, arch_timer_handler,
				 "arch_timer", arch_timer_evt);
	else
		err = request_irq(arch_timer_ppi, arch_timer_handler, 0,
			"arch_timer", arch_timer_evt);
	if (err) {
		pr_err("arch_timer: can't register interrupt %d (%d)\n",
		       arch_timer_ppi, err);
		goto out_free;
	}

	if (arch_timer_ppi2) {
		if (is_irq_percpu)
			err = request_percpu_irq(arch_timer_ppi2,
					arch_timer_handler, "arch_timer",
					arch_timer_evt);
		else
			err = request_irq(arch_timer_ppi2, arch_timer_handler,
					0, "arch_timer", arch_timer_evt);
		if (err) {
			pr_err("arch_timer: can't register interrupt %d (%d)\n",
			       arch_timer_ppi2, err);
			arch_timer_ppi2 = 0;
			goto out_free_irq;
		}
	}

	err = local_timer_register(&arch_timer_ops);
	if (err)
		goto out_free_irq;
	percpu_timer_setup();

	return 0;

out_free_irq:
	free_percpu_irq(arch_timer_ppi, arch_timer_evt);
	if (arch_timer_ppi2)
		free_percpu_irq(arch_timer_ppi2, arch_timer_evt);

out_free:
	free_percpu(arch_timer_evt);

	return err;
}

int __init arch_timer_register(struct arch_timer *at)
{
	if (at->res[0].start <= 0 || !(at->res[0].flags & IORESOURCE_IRQ))
		return -EINVAL;

	arch_timer_ppi = at->res[0].start;

	if (at->res[1].start > 0 && (at->res[1].flags & IORESOURCE_IRQ))
		arch_timer_ppi2 = at->res[1].start;

	if (at->res[2].start > 0 && at->res[2].end > 0 &&
					(at->res[2].flags & IORESOURCE_MEM))
		timer_base = ioremap(at->res[2].start,
				resource_size(&at->res[2]));

	if (!timer_base) {
		pr_err("arch_timer: cant map timer base\n");
		return -ENOMEM;
	}

	return arch_timer_common_register();
}

#ifdef CONFIG_OF
static const struct of_device_id arch_timer_of_match[] __initconst = {
	{ .compatible	= "arm,armv7-timer",	},
	{},
};

int __init arch_timer_of_register(void)
{
	struct device_node *np;
	u32 freq;
	int ret;

	np = of_find_matching_node(NULL, arch_timer_of_match);
	if (!np) {
		pr_err("arch_timer: can't find DT node\n");
		return -ENODEV;
	}

	/* Try to determine the frequency from the device tree or CNTFRQ */
	if (!of_property_read_u32(np, "clock-frequency", &freq))
		arch_timer_rate = freq;

	ret = irq_of_parse_and_map(np, 0);
	if (ret <= 0) {
		pr_err("arch_timer: interrupt not specified in timer node\n");
		return -ENODEV;
	}

	if (of_get_address(np, 0, NULL, NULL)) {
		timer_base = of_iomap(np, 0);
		if (!timer_base) {
			pr_err("arch_timer: cant map timer base\n");
			return -ENOMEM;
		}
	}

	if (of_get_property(np, "irq-is-not-percpu", NULL))
		is_irq_percpu = 0;
	else
		is_irq_percpu = 1;

	arch_timer_ppi = ret;
	ret = irq_of_parse_and_map(np, 1);
	if (ret > 0)
		arch_timer_ppi2 = ret;
	pr_info("arch_timer: found %s irqs %d %d\n",
		np->name, arch_timer_ppi, arch_timer_ppi2);

	return arch_timer_common_register();
}
#endif
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/types.h>
#include <linux/errno.h>

#include <asm/delay.h>

#include <clocksource/arm_arch_timer.h>

static unsigned long arch_timer_read_counter_long(void)
{
	return arch_timer_read_counter();
}

static struct delay_timer arch_delay_timer;

static void __init arch_timer_delay_timer_register(void)
{
	/* Use the architected timer for the delay loop. */
	arch_delay_timer.read_current_timer = arch_timer_read_counter_long;
	arch_delay_timer.freq = arch_timer_get_rate();
	register_current_timer_delay(&arch_delay_timer);
}

int __init arch_timer_arch_init(void)
{
        u32 arch_timer_rate = arch_timer_get_rate();

	if (arch_timer_rate == 0)
		return -ENXIO;

	arch_timer_delay_timer_register();

	return 0;
}
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
