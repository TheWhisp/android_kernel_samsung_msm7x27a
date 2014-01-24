/*
 *  Derived from arch/i386/kernel/irq.c
 *    Copyright (C) 1992 Linus Torvalds
 *  Adapted from arch/i386 by Gary Thomas
 *    Copyright (C) 1995-1996 Gary Thomas (gdt@linuxppc.org)
 *  Updated and modified by Cort Dougan <cort@fsmlabs.com>
 *    Copyright (C) 1996-2001 Cort Dougan
 *  Adapted for Power Macintosh by Paul Mackerras
 *    Copyright (C) 1996 Paul Mackerras (paulus@cs.anu.edu.au)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
 * This file contains the code used by various IRQ handling routines:
 * asking for different IRQ's should be done through these routines
 * instead of just grabbing them. Thus setups with different IRQ numbers
 * shouldn't result in any weird surprises, and installing new handlers
 * should be easier.
 *
 * The MPC8xx has an interrupt mask in the SIU.  If a bit is set, the
 * interrupt is _enabled_.  As expected, IRQ0 is bit 0 in the 32-bit
 * mask register (of which only 16 are defined), hence the weird shifting
 * and complement of the cached_irq_mask.  I want to be able to stuff
 * this right into the SIU SMASK register.
 * Many of the prep/chrp functions are conditional compiled on CONFIG_8xx
 * to reduce code space and undefined function references.
 */

#undef DEBUG

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <linux/threads.h>
#include <linux/kernel_stat.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/ptrace.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/timex.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <linux/seq_file.h>
#include <linux/cpumask.h>
#include <linux/profile.h>
#include <linux/bitops.h>
#include <linux/list.h>
#include <linux/radix-tree.h>
#include <linux/mutex.h>
#include <linux/bootmem.h>
#include <linux/pci.h>
#include <linux/debugfs.h>
#include <linux/of.h>
#include <linux/of_irq.h>

#include <asm/uaccess.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/system.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <asm/io.h>
#include <asm/pgtable.h>
#include <asm/irq.h>
#include <asm/cache.h>
#include <asm/prom.h>
#include <asm/ptrace.h>
#include <asm/machdep.h>
#include <asm/udbg.h>
#include <asm/smp.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <asm/debug.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm/debug.h>
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_PPC64
#include <asm/paca.h>
#include <asm/firmware.h>
#include <asm/lv1call.h>
#endif
#define CREATE_TRACE_POINTS
#include <asm/trace.h>

DEFINE_PER_CPU_SHARED_ALIGNED(irq_cpustat_t, irq_stat);
EXPORT_PER_CPU_SYMBOL(irq_stat);

int __irq_offset_value;

#ifdef CONFIG_PPC32
EXPORT_SYMBOL(__irq_offset_value);
atomic_t ppc_n_lost_interrupts;

#ifdef CONFIG_TAU_INT
extern int tau_initialized;
extern int tau_interrupts(int);
#endif
#endif /* CONFIG_PPC32 */

#ifdef CONFIG_PPC64

<<<<<<< HEAD
<<<<<<< HEAD
#ifndef CONFIG_SPARSE_IRQ
EXPORT_SYMBOL(irq_desc);
#endif

int distribute_irqs = 1;

static inline notrace unsigned long get_hard_enabled(void)
{
	unsigned long enabled;

	__asm__ __volatile__("lbz %0,%1(13)"
	: "=r" (enabled) : "i" (offsetof(struct paca_struct, hard_enabled)));

	return enabled;
=======
=======
>>>>>>> refs/remotes/origin/master
int distribute_irqs = 1;

static inline notrace unsigned long get_irq_happened(void)
{
	unsigned long happened;

	__asm__ __volatile__("lbz %0,%1(13)"
	: "=r" (happened) : "i" (offsetof(struct paca_struct, irq_happened)));

	return happened;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static inline notrace void set_soft_enabled(unsigned long enable)
{
	__asm__ __volatile__("stb %0,%1(13)"
	: : "r" (enable), "i" (offsetof(struct paca_struct, soft_enabled)));
}

<<<<<<< HEAD
<<<<<<< HEAD
notrace void arch_local_irq_restore(unsigned long en)
{
	/*
	 * get_paca()->soft_enabled = en;
	 * Is it ever valid to use local_irq_restore(0) when soft_enabled is 1?
	 * That was allowed before, and in such a case we do need to take care
	 * that gcc will set soft_enabled directly via r13, not choose to use
	 * an intermediate register, lest we're preempted to a different cpu.
	 */
	set_soft_enabled(en);
	if (!en)
		return;

#ifdef CONFIG_PPC_STD_MMU_64
	if (firmware_has_feature(FW_FEATURE_ISERIES)) {
		/*
		 * Do we need to disable preemption here?  Not really: in the
		 * unlikely event that we're preempted to a different cpu in
		 * between getting r13, loading its lppaca_ptr, and loading
		 * its any_int, we might call iseries_handle_interrupts without
		 * an interrupt pending on the new cpu, but that's no disaster,
		 * is it?  And the business of preempting us off the old cpu
		 * would itself involve a local_irq_restore which handles the
		 * interrupt to that cpu.
		 *
		 * But use "local_paca->lppaca_ptr" instead of "get_lppaca()"
		 * to avoid any preemption checking added into get_paca().
		 */
		if (local_paca->lppaca_ptr->int_dword.any_int)
			iseries_handle_interrupts();
	}
#endif /* CONFIG_PPC_STD_MMU_64 */

	/*
	 * if (get_paca()->hard_enabled) return;
	 * But again we need to take care that gcc gets hard_enabled directly
	 * via r13, not choose to use an intermediate register, lest we're
	 * preempted to a different cpu in between the two instructions.
	 */
	if (get_hard_enabled())
		return;

#if defined(CONFIG_BOOKE) && defined(CONFIG_SMP)
	/* Check for pending doorbell interrupts and resend to ourself */
	if (cpu_has_feature(CPU_FTR_DBELL))
		smp_muxed_ipi_resend();
#endif

	/*
	 * Need to hard-enable interrupts here.  Since currently disabled,
	 * no need to take further asm precautions against preemption; but
	 * use local_paca instead of get_paca() to avoid preemption checking.
	 */
	local_paca->hard_enabled = en;

	/*
	 * Trigger the decrementer if we have a pending event. Some processors
	 * only trigger on edge transitions of the sign bit. We might also
	 * have disabled interrupts long enough that the decrementer wrapped
	 * to positive.
	 */
	decrementer_check_overflow();

	/*
	 * Force the delivery of pending soft-disabled interrupts on PS3.
	 * Any HV call will have this side effect.
	 */
	if (firmware_has_feature(FW_FEATURE_PS3_LV1)) {
		u64 tmp;
		lv1_get_version_info(&tmp);
	}

	__hard_irq_enable();
}
EXPORT_SYMBOL(arch_local_irq_restore);
=======
=======
>>>>>>> refs/remotes/origin/master
static inline notrace int decrementer_check_overflow(void)
{
 	u64 now = get_tb_or_rtc();
 	u64 *next_tb = &__get_cpu_var(decrementers_next_tb);
 
<<<<<<< HEAD
	if (now >= *next_tb)
		set_dec(1);
=======
>>>>>>> refs/remotes/origin/master
	return now >= *next_tb;
}

/* This is called whenever we are re-enabling interrupts
<<<<<<< HEAD
 * and returns either 0 (nothing to do) or 500/900 if there's
 * either an EE or a DEC to generate.
=======
 * and returns either 0 (nothing to do) or 500/900/280/a00/e80 if
 * there's an EE, DEC or DBELL to generate.
>>>>>>> refs/remotes/origin/master
 *
 * This is called in two contexts: From arch_local_irq_restore()
 * before soft-enabling interrupts, and from the exception exit
 * path when returning from an interrupt from a soft-disabled to
 * a soft enabled context. In both case we have interrupts hard
 * disabled.
 *
 * We take care of only clearing the bits we handled in the
 * PACA irq_happened field since we can only re-emit one at a
 * time and we don't want to "lose" one.
 */
notrace unsigned int __check_irq_replay(void)
{
	/*
	 * We use local_paca rather than get_paca() to avoid all
	 * the debug_smp_processor_id() business in this low level
	 * function
	 */
	unsigned char happened = local_paca->irq_happened;

	/* Clear bit 0 which we wouldn't clear otherwise */
	local_paca->irq_happened &= ~PACA_IRQ_HARD_DIS;

	/*
	 * Force the delivery of pending soft-disabled interrupts on PS3.
	 * Any HV call will have this side effect.
	 */
	if (firmware_has_feature(FW_FEATURE_PS3_LV1)) {
		u64 tmp, tmp2;
		lv1_get_version_info(&tmp, &tmp2);
	}

	/*
	 * We may have missed a decrementer interrupt. We check the
	 * decrementer itself rather than the paca irq_happened field
	 * in case we also had a rollover while hard disabled
	 */
	local_paca->irq_happened &= ~PACA_IRQ_DEC;
	if ((happened & PACA_IRQ_DEC) || decrementer_check_overflow())
		return 0x900;

	/* Finally check if an external interrupt happened */
	local_paca->irq_happened &= ~PACA_IRQ_EE;
	if (happened & PACA_IRQ_EE)
		return 0x500;

#ifdef CONFIG_PPC_BOOK3E
	/* Finally check if an EPR external interrupt happened
	 * this bit is typically set if we need to handle another
	 * "edge" interrupt from within the MPIC "EPR" handler
	 */
	local_paca->irq_happened &= ~PACA_IRQ_EE_EDGE;
	if (happened & PACA_IRQ_EE_EDGE)
		return 0x500;

	local_paca->irq_happened &= ~PACA_IRQ_DBELL;
	if (happened & PACA_IRQ_DBELL)
		return 0x280;
<<<<<<< HEAD
=======
#else
	local_paca->irq_happened &= ~PACA_IRQ_DBELL;
	if (happened & PACA_IRQ_DBELL) {
		if (cpu_has_feature(CPU_FTR_HVMODE))
			return 0xe80;
		return 0xa00;
	}
>>>>>>> refs/remotes/origin/master
#endif /* CONFIG_PPC_BOOK3E */

	/* There should be nothing left ! */
	BUG_ON(local_paca->irq_happened != 0);

	return 0;
}

notrace void arch_local_irq_restore(unsigned long en)
{
	unsigned char irq_happened;
	unsigned int replay;

	/* Write the new soft-enabled value */
	set_soft_enabled(en);
	if (!en)
		return;
	/*
	 * From this point onward, we can take interrupts, preempt,
	 * etc... unless we got hard-disabled. We check if an event
	 * happened. If none happened, we know we can just return.
	 *
	 * We may have preempted before the check below, in which case
	 * we are checking the "new" CPU instead of the old one. This
	 * is only a problem if an event happened on the "old" CPU.
	 *
	 * External interrupt events will have caused interrupts to
	 * be hard-disabled, so there is no problem, we
	 * cannot have preempted.
	 */
	irq_happened = get_irq_happened();
	if (!irq_happened)
		return;

	/*
	 * We need to hard disable to get a trusted value from
	 * __check_irq_replay(). We also need to soft-disable
	 * again to avoid warnings in there due to the use of
	 * per-cpu variables.
	 *
	 * We know that if the value in irq_happened is exactly 0x01
	 * then we are already hard disabled (there are other less
	 * common cases that we'll ignore for now), so we skip the
	 * (expensive) mtmsrd.
	 */
	if (unlikely(irq_happened != PACA_IRQ_HARD_DIS))
		__hard_irq_disable();
#ifdef CONFIG_TRACE_IRQFLAGS
	else {
		/*
		 * We should already be hard disabled here. We had bugs
		 * where that wasn't the case so let's dbl check it and
		 * warn if we are wrong. Only do that when IRQ tracing
		 * is enabled as mfmsr() can be costly.
		 */
		if (WARN_ON(mfmsr() & MSR_EE))
			__hard_irq_disable();
	}
#endif /* CONFIG_TRACE_IRQFLAG */

	set_soft_enabled(0);

	/*
	 * Check if anything needs to be re-emitted. We haven't
	 * soft-enabled yet to avoid warnings in decrementer_check_overflow
	 * accessing per-cpu variables
	 */
	replay = __check_irq_replay();

	/* We can soft-enable now */
	set_soft_enabled(1);

	/*
	 * And replay if we have to. This will return with interrupts
	 * hard-enabled.
	 */
	if (replay) {
		__replay_interrupt(replay);
		return;
	}

	/* Finally, let's ensure we are hard enabled */
	__hard_irq_enable();
}
EXPORT_SYMBOL(arch_local_irq_restore);

/*
 * This is specifically called by assembly code to re-enable interrupts
 * if they are currently disabled. This is typically called before
 * schedule() or do_signal() when returning to userspace. We do it
 * in C to avoid the burden of dealing with lockdep etc...
 *
 * NOTE: This is called with interrupts hard disabled but not marked
 * as such in paca->irq_happened, so we need to resync this.
 */
void notrace restore_interrupts(void)
{
	if (irqs_disabled()) {
		local_paca->irq_happened |= PACA_IRQ_HARD_DIS;
		local_irq_enable();
	} else
		__hard_irq_enable();
}

/*
 * This is a helper to use when about to go into idle low-power
 * when the latter has the side effect of re-enabling interrupts
 * (such as calling H_CEDE under pHyp).
 *
 * You call this function with interrupts soft-disabled (this is
 * already the case when ppc_md.power_save is called). The function
 * will return whether to enter power save or just return.
 *
 * In the former case, it will have notified lockdep of interrupts
 * being re-enabled and generally sanitized the lazy irq state,
 * and in the latter case it will leave with interrupts hard
 * disabled and marked as such, so the local_irq_enable() call
 * in cpu_idle() will properly re-enable everything.
 */
bool prep_irq_for_idle(void)
{
	/*
	 * First we need to hard disable to ensure no interrupt
	 * occurs before we effectively enter the low power state
	 */
	hard_irq_disable();

	/*
	 * If anything happened while we were soft-disabled,
	 * we return now and do not enter the low power state.
	 */
	if (lazy_irq_pending())
		return false;

	/* Tell lockdep we are about to re-enable */
	trace_hardirqs_on();

	/*
	 * Mark interrupts as soft-enabled and clear the
	 * PACA_IRQ_HARD_DIS from the pending mask since we
	 * are about to hard enable as well as a side effect
	 * of entering the low power state.
	 */
	local_paca->irq_happened &= ~PACA_IRQ_HARD_DIS;
	local_paca->soft_enabled = 1;

	/* Tell the caller to enter the low power state */
	return true;
}

<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
#endif /* CONFIG_PPC64 */

int arch_show_interrupts(struct seq_file *p, int prec)
{
	int j;

#if defined(CONFIG_PPC32) && defined(CONFIG_TAU_INT)
	if (tau_initialized) {
		seq_printf(p, "%*s: ", prec, "TAU");
		for_each_online_cpu(j)
			seq_printf(p, "%10u ", tau_interrupts(j));
		seq_puts(p, "  PowerPC             Thermal Assist (cpu temp)\n");
	}
#endif /* CONFIG_PPC32 && CONFIG_TAU_INT */

	seq_printf(p, "%*s: ", prec, "LOC");
	for_each_online_cpu(j)
		seq_printf(p, "%10u ", per_cpu(irq_stat, j).timer_irqs);
        seq_printf(p, "  Local timer interrupts\n");

	seq_printf(p, "%*s: ", prec, "SPU");
	for_each_online_cpu(j)
		seq_printf(p, "%10u ", per_cpu(irq_stat, j).spurious_irqs);
	seq_printf(p, "  Spurious interrupts\n");

<<<<<<< HEAD
	seq_printf(p, "%*s: ", prec, "CNT");
=======
	seq_printf(p, "%*s: ", prec, "PMI");
>>>>>>> refs/remotes/origin/master
	for_each_online_cpu(j)
		seq_printf(p, "%10u ", per_cpu(irq_stat, j).pmu_irqs);
	seq_printf(p, "  Performance monitoring interrupts\n");

	seq_printf(p, "%*s: ", prec, "MCE");
	for_each_online_cpu(j)
		seq_printf(p, "%10u ", per_cpu(irq_stat, j).mce_exceptions);
	seq_printf(p, "  Machine check exceptions\n");

<<<<<<< HEAD
=======
#ifdef CONFIG_PPC_DOORBELL
	if (cpu_has_feature(CPU_FTR_DBELL)) {
		seq_printf(p, "%*s: ", prec, "DBL");
		for_each_online_cpu(j)
			seq_printf(p, "%10u ", per_cpu(irq_stat, j).doorbell_irqs);
		seq_printf(p, "  Doorbell interrupts\n");
	}
#endif

>>>>>>> refs/remotes/origin/master
	return 0;
}

/*
 * /proc/stat helpers
 */
u64 arch_irq_stat_cpu(unsigned int cpu)
{
	u64 sum = per_cpu(irq_stat, cpu).timer_irqs;

	sum += per_cpu(irq_stat, cpu).pmu_irqs;
	sum += per_cpu(irq_stat, cpu).mce_exceptions;
	sum += per_cpu(irq_stat, cpu).spurious_irqs;
<<<<<<< HEAD
=======
#ifdef CONFIG_PPC_DOORBELL
	sum += per_cpu(irq_stat, cpu).doorbell_irqs;
#endif
>>>>>>> refs/remotes/origin/master

	return sum;
}

#ifdef CONFIG_HOTPLUG_CPU
void migrate_irqs(void)
{
	struct irq_desc *desc;
	unsigned int irq;
	static int warned;
	cpumask_var_t mask;
	const struct cpumask *map = cpu_online_mask;

	alloc_cpumask_var(&mask, GFP_KERNEL);

<<<<<<< HEAD
<<<<<<< HEAD
	for_each_irq(irq) {
		struct irq_data *data;
		struct irq_chip *chip;

		desc = irq_to_desc(irq);
		if (!desc)
			continue;

=======
=======
>>>>>>> refs/remotes/origin/master
	for_each_irq_desc(irq, desc) {
		struct irq_data *data;
		struct irq_chip *chip;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		data = irq_desc_get_irq_data(desc);
		if (irqd_is_per_cpu(data))
			continue;

		chip = irq_data_get_irq_chip(data);

		cpumask_and(mask, data->affinity, map);
		if (cpumask_any(mask) >= nr_cpu_ids) {
			printk("Breaking affinity for irq %i\n", irq);
			cpumask_copy(mask, map);
		}
		if (chip->irq_set_affinity)
			chip->irq_set_affinity(data, mask, true);
		else if (desc->action && !(warned++))
			printk("Cannot set affinity for irq %i\n", irq);
	}

	free_cpumask_var(mask);

	local_irq_enable();
	mdelay(1);
	local_irq_disable();
}
#endif

<<<<<<< HEAD
static inline void handle_one_irq(unsigned int irq)
{
	struct thread_info *curtp, *irqtp;
	unsigned long saved_sp_limit;
	struct irq_desc *desc;

	desc = irq_to_desc(irq);
	if (!desc)
		return;

	/* Switch to the irq stack to handle this */
	curtp = current_thread_info();
	irqtp = hardirq_ctx[smp_processor_id()];

	if (curtp == irqtp) {
		/* We're already on the irq stack, just handle it */
		desc->handle_irq(irq, desc);
		return;
	}

	saved_sp_limit = current->thread.ksp_limit;

	irqtp->task = curtp->task;
	irqtp->flags = 0;

	/* Copy the softirq bits in preempt_count so that the
	 * softirq checks work in the hardirq context. */
	irqtp->preempt_count = (irqtp->preempt_count & ~SOFTIRQ_MASK) |
			       (curtp->preempt_count & SOFTIRQ_MASK);

	current->thread.ksp_limit = (unsigned long)irqtp +
		_ALIGN_UP(sizeof(struct thread_info), 16);

	call_handle_irq(irq, desc, irqtp, desc->handle_irq);
	current->thread.ksp_limit = saved_sp_limit;
	irqtp->task = NULL;

	/* Set any flag that may have been set on the
	 * alternate stack
	 */
	if (irqtp->flags)
		set_bits(irqtp->flags, &curtp->flags);
}

=======
>>>>>>> refs/remotes/origin/master
static inline void check_stack_overflow(void)
{
#ifdef CONFIG_DEBUG_STACKOVERFLOW
	long sp;

	sp = __get_SP() & (THREAD_SIZE-1);

	/* check for stack overflow: is there less than 2KB free? */
	if (unlikely(sp < (sizeof(struct thread_info) + 2048))) {
		printk("do_IRQ: stack overflow: %ld\n",
			sp - sizeof(struct thread_info));
		dump_stack();
	}
#endif
}

<<<<<<< HEAD
void do_IRQ(struct pt_regs *regs)
{
	struct pt_regs *old_regs = set_irq_regs(regs);
	unsigned int irq;

	trace_irq_entry(regs);

	irq_enter();

	check_stack_overflow();

<<<<<<< HEAD
	irq = ppc_md.get_irq();

	if (irq != NO_IRQ && irq != NO_IRQ_IGNORE)
		handle_one_irq(irq);
	else if (irq != NO_IRQ_IGNORE)
=======
=======
void __do_irq(struct pt_regs *regs)
{
	struct irq_desc *desc;
	unsigned int irq;

	irq_enter();

	trace_irq_entry(regs);

	check_stack_overflow();

>>>>>>> refs/remotes/origin/master
	/*
	 * Query the platform PIC for the interrupt & ack it.
	 *
	 * This will typically lower the interrupt line to the CPU
	 */
	irq = ppc_md.get_irq();

<<<<<<< HEAD
	/* We can hard enable interrupts now */
	may_hard_irq_enable();

	/* And finally process it */
	if (irq != NO_IRQ)
		handle_one_irq(irq);
	else
>>>>>>> refs/remotes/origin/cm-10.0
		__get_cpu_var(irq_stat).spurious_irqs++;

	irq_exit();
	set_irq_regs(old_regs);

<<<<<<< HEAD
#ifdef CONFIG_PPC_ISERIES
	if (firmware_has_feature(FW_FEATURE_ISERIES) &&
			get_lppaca()->int_dword.fields.decr_int) {
		get_lppaca()->int_dword.fields.decr_int = 0;
		/* Signal a fake decrementer interrupt */
		timer_interrupt(regs);
	}
#endif

=======
>>>>>>> refs/remotes/origin/cm-10.0
	trace_irq_exit(regs);
=======
	/* We can hard enable interrupts now to allow perf interrupts */
	may_hard_irq_enable();

	/* And finally process it */
	if (unlikely(irq == NO_IRQ))
		__get_cpu_var(irq_stat).spurious_irqs++;
	else {
		desc = irq_to_desc(irq);
		if (likely(desc))
			desc->handle_irq(irq, desc);
	}

	trace_irq_exit(regs);

	irq_exit();
}

void do_IRQ(struct pt_regs *regs)
{
	struct pt_regs *old_regs = set_irq_regs(regs);
	struct thread_info *curtp, *irqtp, *sirqtp;

	/* Switch to the irq stack to handle this */
	curtp = current_thread_info();
	irqtp = hardirq_ctx[raw_smp_processor_id()];
	sirqtp = softirq_ctx[raw_smp_processor_id()];

	/* Already there ? */
	if (unlikely(curtp == irqtp || curtp == sirqtp)) {
		__do_irq(regs);
		set_irq_regs(old_regs);
		return;
	}

	/* Prepare the thread_info in the irq stack */
	irqtp->task = curtp->task;
	irqtp->flags = 0;

	/* Copy the preempt_count so that the [soft]irq checks work. */
	irqtp->preempt_count = curtp->preempt_count;

	/* Switch stack and call */
	call_do_irq(regs, irqtp);

	/* Restore stack limit */
	irqtp->task = NULL;

	/* Copy back updates to the thread_info */
	if (irqtp->flags)
		set_bits(irqtp->flags, &curtp->flags);

	set_irq_regs(old_regs);
>>>>>>> refs/remotes/origin/master
}

void __init init_IRQ(void)
{
	if (ppc_md.init_IRQ)
		ppc_md.init_IRQ();

	exc_lvl_ctx_init();

	irq_ctx_init();
}

#if defined(CONFIG_BOOKE) || defined(CONFIG_40x)
struct thread_info   *critirq_ctx[NR_CPUS] __read_mostly;
struct thread_info    *dbgirq_ctx[NR_CPUS] __read_mostly;
struct thread_info *mcheckirq_ctx[NR_CPUS] __read_mostly;

void exc_lvl_ctx_init(void)
{
	struct thread_info *tp;
	int i, cpu_nr;

	for_each_possible_cpu(i) {
#ifdef CONFIG_PPC64
		cpu_nr = i;
#else
		cpu_nr = get_hard_smp_processor_id(i);
#endif
		memset((void *)critirq_ctx[cpu_nr], 0, THREAD_SIZE);
		tp = critirq_ctx[cpu_nr];
		tp->cpu = cpu_nr;
		tp->preempt_count = 0;

#ifdef CONFIG_BOOKE
		memset((void *)dbgirq_ctx[cpu_nr], 0, THREAD_SIZE);
		tp = dbgirq_ctx[cpu_nr];
		tp->cpu = cpu_nr;
		tp->preempt_count = 0;

		memset((void *)mcheckirq_ctx[cpu_nr], 0, THREAD_SIZE);
		tp = mcheckirq_ctx[cpu_nr];
		tp->cpu = cpu_nr;
		tp->preempt_count = HARDIRQ_OFFSET;
#endif
	}
}
#endif

struct thread_info *softirq_ctx[NR_CPUS] __read_mostly;
struct thread_info *hardirq_ctx[NR_CPUS] __read_mostly;

void irq_ctx_init(void)
{
	struct thread_info *tp;
	int i;

	for_each_possible_cpu(i) {
		memset((void *)softirq_ctx[i], 0, THREAD_SIZE);
		tp = softirq_ctx[i];
		tp->cpu = i;
<<<<<<< HEAD
		tp->preempt_count = 0;
=======
>>>>>>> refs/remotes/origin/master

		memset((void *)hardirq_ctx[i], 0, THREAD_SIZE);
		tp = hardirq_ctx[i];
		tp->cpu = i;
<<<<<<< HEAD
		tp->preempt_count = HARDIRQ_OFFSET;
	}
}

static inline void do_softirq_onstack(void)
{
	struct thread_info *curtp, *irqtp;
	unsigned long saved_sp_limit = current->thread.ksp_limit;
=======
	}
}

void do_softirq_own_stack(void)
{
	struct thread_info *curtp, *irqtp;
>>>>>>> refs/remotes/origin/master

	curtp = current_thread_info();
	irqtp = softirq_ctx[smp_processor_id()];
	irqtp->task = curtp->task;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	irqtp->flags = 0;
>>>>>>> refs/remotes/origin/cm-10.0
	current->thread.ksp_limit = (unsigned long)irqtp +
				    _ALIGN_UP(sizeof(struct thread_info), 16);
	call_do_softirq(irqtp);
	current->thread.ksp_limit = saved_sp_limit;
	irqtp->task = NULL;
<<<<<<< HEAD
=======
=======
	irqtp->flags = 0;
	call_do_softirq(irqtp);
	irqtp->task = NULL;
>>>>>>> refs/remotes/origin/master

	/* Set any flag that may have been set on the
	 * alternate stack
	 */
	if (irqtp->flags)
		set_bits(irqtp->flags, &curtp->flags);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

void do_softirq(void)
{
	unsigned long flags;

	if (in_interrupt())
		return;

	local_irq_save(flags);

	if (local_softirq_pending())
		do_softirq_onstack();

	local_irq_restore(flags);
}

<<<<<<< HEAD

/*
 * IRQ controller and virtual interrupts
 */

/* The main irq map itself is an array of NR_IRQ entries containing the
 * associate host and irq number. An entry with a host of NULL is free.
 * An entry can be allocated if it's free, the allocator always then sets
 * hwirq first to the host's invalid irq number and then fills ops.
 */
struct irq_map_entry {
	irq_hw_number_t	hwirq;
	struct irq_host	*host;
};

static LIST_HEAD(irq_hosts);
static DEFINE_RAW_SPINLOCK(irq_big_lock);
static DEFINE_MUTEX(revmap_trees_mutex);
static struct irq_map_entry irq_map[NR_IRQS];
static unsigned int irq_virq_count = NR_IRQS;
static struct irq_host *irq_default_host;

irq_hw_number_t irqd_to_hwirq(struct irq_data *d)
{
	return irq_map[d->irq].hwirq;
}
EXPORT_SYMBOL_GPL(irqd_to_hwirq);

irq_hw_number_t virq_to_hw(unsigned int virq)
{
	return irq_map[virq].hwirq;
}
EXPORT_SYMBOL_GPL(virq_to_hw);

bool virq_is_host(unsigned int virq, struct irq_host *host)
{
	return irq_map[virq].host == host;
}
EXPORT_SYMBOL_GPL(virq_is_host);

static int default_irq_host_match(struct irq_host *h, struct device_node *np)
{
	return h->of_node != NULL && h->of_node == np;
}

struct irq_host *irq_alloc_host(struct device_node *of_node,
				unsigned int revmap_type,
				unsigned int revmap_arg,
				struct irq_host_ops *ops,
				irq_hw_number_t inval_irq)
{
	struct irq_host *host;
	unsigned int size = sizeof(struct irq_host);
	unsigned int i;
	unsigned int *rmap;
	unsigned long flags;

	/* Allocate structure and revmap table if using linear mapping */
	if (revmap_type == IRQ_HOST_MAP_LINEAR)
		size += revmap_arg * sizeof(unsigned int);
	host = kzalloc(size, GFP_KERNEL);
	if (host == NULL)
		return NULL;

	/* Fill structure */
	host->revmap_type = revmap_type;
	host->inval_irq = inval_irq;
	host->ops = ops;
	host->of_node = of_node_get(of_node);

	if (host->ops->match == NULL)
		host->ops->match = default_irq_host_match;

	raw_spin_lock_irqsave(&irq_big_lock, flags);

	/* If it's a legacy controller, check for duplicates and
	 * mark it as allocated (we use irq 0 host pointer for that
	 */
	if (revmap_type == IRQ_HOST_MAP_LEGACY) {
		if (irq_map[0].host != NULL) {
			raw_spin_unlock_irqrestore(&irq_big_lock, flags);
			of_node_put(host->of_node);
			kfree(host);
			return NULL;
		}
		irq_map[0].host = host;
	}

	list_add(&host->link, &irq_hosts);
	raw_spin_unlock_irqrestore(&irq_big_lock, flags);

	/* Additional setups per revmap type */
	switch(revmap_type) {
	case IRQ_HOST_MAP_LEGACY:
		/* 0 is always the invalid number for legacy */
		host->inval_irq = 0;
		/* setup us as the host for all legacy interrupts */
		for (i = 1; i < NUM_ISA_INTERRUPTS; i++) {
			irq_map[i].hwirq = i;
			smp_wmb();
			irq_map[i].host = host;
			smp_wmb();

			/* Legacy flags are left to default at this point,
			 * one can then use irq_create_mapping() to
			 * explicitly change them
			 */
			ops->map(host, i, i);

			/* Clear norequest flags */
			irq_clear_status_flags(i, IRQ_NOREQUEST);
		}
		break;
	case IRQ_HOST_MAP_LINEAR:
		rmap = (unsigned int *)(host + 1);
		for (i = 0; i < revmap_arg; i++)
			rmap[i] = NO_IRQ;
		host->revmap_data.linear.size = revmap_arg;
		smp_wmb();
		host->revmap_data.linear.revmap = rmap;
		break;
	case IRQ_HOST_MAP_TREE:
		INIT_RADIX_TREE(&host->revmap_data.tree, GFP_KERNEL);
		break;
	default:
		break;
	}

	pr_debug("irq: Allocated host of type %d @0x%p\n", revmap_type, host);

	return host;
}

struct irq_host *irq_find_host(struct device_node *node)
{
	struct irq_host *h, *found = NULL;
	unsigned long flags;

	/* We might want to match the legacy controller last since
	 * it might potentially be set to match all interrupts in
	 * the absence of a device node. This isn't a problem so far
	 * yet though...
	 */
	raw_spin_lock_irqsave(&irq_big_lock, flags);
	list_for_each_entry(h, &irq_hosts, link)
		if (h->ops->match(h, node)) {
			found = h;
			break;
		}
	raw_spin_unlock_irqrestore(&irq_big_lock, flags);
	return found;
}
EXPORT_SYMBOL_GPL(irq_find_host);

void irq_set_default_host(struct irq_host *host)
{
	pr_debug("irq: Default host set to @0x%p\n", host);

	irq_default_host = host;
}

void irq_set_virq_count(unsigned int count)
{
	pr_debug("irq: Trying to set virq count to %d\n", count);

	BUG_ON(count < NUM_ISA_INTERRUPTS);
	if (count < NR_IRQS)
		irq_virq_count = count;
}

static int irq_setup_virq(struct irq_host *host, unsigned int virq,
			    irq_hw_number_t hwirq)
{
	int res;

	res = irq_alloc_desc_at(virq, 0);
	if (res != virq) {
		pr_debug("irq: -> allocating desc failed\n");
		goto error;
	}

	/* map it */
	smp_wmb();
	irq_map[virq].hwirq = hwirq;
	smp_mb();

	if (host->ops->map(host, virq, hwirq)) {
		pr_debug("irq: -> mapping failed, freeing\n");
		goto errdesc;
	}

	irq_clear_status_flags(virq, IRQ_NOREQUEST);

	return 0;

errdesc:
	irq_free_descs(virq, 1);
error:
	irq_free_virt(virq, 1);
	return -1;
}

unsigned int irq_create_direct_mapping(struct irq_host *host)
{
	unsigned int virq;

	if (host == NULL)
		host = irq_default_host;

	BUG_ON(host == NULL);
	WARN_ON(host->revmap_type != IRQ_HOST_MAP_NOMAP);

	virq = irq_alloc_virt(host, 1, 0);
	if (virq == NO_IRQ) {
		pr_debug("irq: create_direct virq allocation failed\n");
		return NO_IRQ;
	}

	pr_debug("irq: create_direct obtained virq %d\n", virq);

	if (irq_setup_virq(host, virq, virq))
		return NO_IRQ;

	return virq;
}

unsigned int irq_create_mapping(struct irq_host *host,
				irq_hw_number_t hwirq)
{
	unsigned int virq, hint;

	pr_debug("irq: irq_create_mapping(0x%p, 0x%lx)\n", host, hwirq);

	/* Look for default host if nececssary */
	if (host == NULL)
		host = irq_default_host;
	if (host == NULL) {
		printk(KERN_WARNING "irq_create_mapping called for"
		       " NULL host, hwirq=%lx\n", hwirq);
		WARN_ON(1);
		return NO_IRQ;
	}
	pr_debug("irq: -> using host @%p\n", host);

	/* Check if mapping already exists */
	virq = irq_find_mapping(host, hwirq);
	if (virq != NO_IRQ) {
		pr_debug("irq: -> existing mapping on virq %d\n", virq);
		return virq;
	}

	/* Get a virtual interrupt number */
	if (host->revmap_type == IRQ_HOST_MAP_LEGACY) {
		/* Handle legacy */
		virq = (unsigned int)hwirq;
		if (virq == 0 || virq >= NUM_ISA_INTERRUPTS)
			return NO_IRQ;
		return virq;
	} else {
		/* Allocate a virtual interrupt number */
		hint = hwirq % irq_virq_count;
		virq = irq_alloc_virt(host, 1, hint);
		if (virq == NO_IRQ) {
			pr_debug("irq: -> virq allocation failed\n");
			return NO_IRQ;
		}
	}

	if (irq_setup_virq(host, virq, hwirq))
		return NO_IRQ;

	printk(KERN_DEBUG "irq: irq %lu on host %s mapped to virtual irq %u\n",
		hwirq, host->of_node ? host->of_node->full_name : "null", virq);

	return virq;
}
EXPORT_SYMBOL_GPL(irq_create_mapping);

unsigned int irq_create_of_mapping(struct device_node *controller,
				   const u32 *intspec, unsigned int intsize)
{
	struct irq_host *host;
	irq_hw_number_t hwirq;
	unsigned int type = IRQ_TYPE_NONE;
	unsigned int virq;

	if (controller == NULL)
		host = irq_default_host;
	else
		host = irq_find_host(controller);
	if (host == NULL) {
		printk(KERN_WARNING "irq: no irq host found for %s !\n",
		       controller->full_name);
		return NO_IRQ;
	}

	/* If host has no translation, then we assume interrupt line */
	if (host->ops->xlate == NULL)
		hwirq = intspec[0];
	else {
		if (host->ops->xlate(host, controller, intspec, intsize,
				     &hwirq, &type))
			return NO_IRQ;
	}

	/* Create mapping */
	virq = irq_create_mapping(host, hwirq);
	if (virq == NO_IRQ)
		return virq;

	/* Set type if specified and different than the current one */
	if (type != IRQ_TYPE_NONE &&
	    type != (irqd_get_trigger_type(irq_get_irq_data(virq))))
		irq_set_irq_type(virq, type);
	return virq;
}
EXPORT_SYMBOL_GPL(irq_create_of_mapping);

void irq_dispose_mapping(unsigned int virq)
{
	struct irq_host *host;
	irq_hw_number_t hwirq;

	if (virq == NO_IRQ)
		return;

	host = irq_map[virq].host;
	if (WARN_ON(host == NULL))
		return;

	/* Never unmap legacy interrupts */
	if (host->revmap_type == IRQ_HOST_MAP_LEGACY)
		return;

	irq_set_status_flags(virq, IRQ_NOREQUEST);

	/* remove chip and handler */
	irq_set_chip_and_handler(virq, NULL, NULL);

	/* Make sure it's completed */
	synchronize_irq(virq);

	/* Tell the PIC about it */
	if (host->ops->unmap)
		host->ops->unmap(host, virq);
	smp_mb();

	/* Clear reverse map */
	hwirq = irq_map[virq].hwirq;
	switch(host->revmap_type) {
	case IRQ_HOST_MAP_LINEAR:
		if (hwirq < host->revmap_data.linear.size)
			host->revmap_data.linear.revmap[hwirq] = NO_IRQ;
		break;
	case IRQ_HOST_MAP_TREE:
		mutex_lock(&revmap_trees_mutex);
		radix_tree_delete(&host->revmap_data.tree, hwirq);
		mutex_unlock(&revmap_trees_mutex);
		break;
	}

	/* Destroy map */
	smp_mb();
	irq_map[virq].hwirq = host->inval_irq;

	irq_free_descs(virq, 1);
	/* Free it */
	irq_free_virt(virq, 1);
}
EXPORT_SYMBOL_GPL(irq_dispose_mapping);

unsigned int irq_find_mapping(struct irq_host *host,
			      irq_hw_number_t hwirq)
{
	unsigned int i;
	unsigned int hint = hwirq % irq_virq_count;

	/* Look for default host if nececssary */
	if (host == NULL)
		host = irq_default_host;
	if (host == NULL)
		return NO_IRQ;

	/* legacy -> bail early */
	if (host->revmap_type == IRQ_HOST_MAP_LEGACY)
		return hwirq;

	/* Slow path does a linear search of the map */
	if (hint < NUM_ISA_INTERRUPTS)
		hint = NUM_ISA_INTERRUPTS;
	i = hint;
	do  {
		if (irq_map[i].host == host &&
		    irq_map[i].hwirq == hwirq)
			return i;
		i++;
		if (i >= irq_virq_count)
			i = NUM_ISA_INTERRUPTS;
	} while(i != hint);
	return NO_IRQ;
}
EXPORT_SYMBOL_GPL(irq_find_mapping);


unsigned int irq_radix_revmap_lookup(struct irq_host *host,
				     irq_hw_number_t hwirq)
{
	struct irq_map_entry *ptr;
	unsigned int virq;

	if (WARN_ON_ONCE(host->revmap_type != IRQ_HOST_MAP_TREE))
		return irq_find_mapping(host, hwirq);

	/*
	 * The ptr returned references the static global irq_map.
	 * but freeing an irq can delete nodes along the path to
	 * do the lookup via call_rcu.
	 */
	rcu_read_lock();
	ptr = radix_tree_lookup(&host->revmap_data.tree, hwirq);
	rcu_read_unlock();

	/*
	 * If found in radix tree, then fine.
	 * Else fallback to linear lookup - this should not happen in practice
	 * as it means that we failed to insert the node in the radix tree.
	 */
	if (ptr)
		virq = ptr - irq_map;
	else
		virq = irq_find_mapping(host, hwirq);

	return virq;
}

void irq_radix_revmap_insert(struct irq_host *host, unsigned int virq,
			     irq_hw_number_t hwirq)
{
	if (WARN_ON(host->revmap_type != IRQ_HOST_MAP_TREE))
		return;

	if (virq != NO_IRQ) {
		mutex_lock(&revmap_trees_mutex);
		radix_tree_insert(&host->revmap_data.tree, hwirq,
				  &irq_map[virq]);
		mutex_unlock(&revmap_trees_mutex);
	}
}

unsigned int irq_linear_revmap(struct irq_host *host,
			       irq_hw_number_t hwirq)
{
	unsigned int *revmap;

	if (WARN_ON_ONCE(host->revmap_type != IRQ_HOST_MAP_LINEAR))
		return irq_find_mapping(host, hwirq);

	/* Check revmap bounds */
	if (unlikely(hwirq >= host->revmap_data.linear.size))
		return irq_find_mapping(host, hwirq);

	/* Check if revmap was allocated */
	revmap = host->revmap_data.linear.revmap;
	if (unlikely(revmap == NULL))
		return irq_find_mapping(host, hwirq);

	/* Fill up revmap with slow path if no mapping found */
	if (unlikely(revmap[hwirq] == NO_IRQ))
		revmap[hwirq] = irq_find_mapping(host, hwirq);

	return revmap[hwirq];
}

unsigned int irq_alloc_virt(struct irq_host *host,
			    unsigned int count,
			    unsigned int hint)
{
	unsigned long flags;
	unsigned int i, j, found = NO_IRQ;

	if (count == 0 || count > (irq_virq_count - NUM_ISA_INTERRUPTS))
		return NO_IRQ;

	raw_spin_lock_irqsave(&irq_big_lock, flags);

	/* Use hint for 1 interrupt if any */
	if (count == 1 && hint >= NUM_ISA_INTERRUPTS &&
	    hint < irq_virq_count && irq_map[hint].host == NULL) {
		found = hint;
		goto hint_found;
	}

	/* Look for count consecutive numbers in the allocatable
	 * (non-legacy) space
	 */
	for (i = NUM_ISA_INTERRUPTS, j = 0; i < irq_virq_count; i++) {
		if (irq_map[i].host != NULL)
			j = 0;
		else
			j++;

		if (j == count) {
			found = i - count + 1;
			break;
		}
	}
	if (found == NO_IRQ) {
		raw_spin_unlock_irqrestore(&irq_big_lock, flags);
		return NO_IRQ;
	}
 hint_found:
	for (i = found; i < (found + count); i++) {
		irq_map[i].hwirq = host->inval_irq;
		smp_wmb();
		irq_map[i].host = host;
	}
	raw_spin_unlock_irqrestore(&irq_big_lock, flags);
	return found;
}

void irq_free_virt(unsigned int virq, unsigned int count)
{
	unsigned long flags;
	unsigned int i;

	WARN_ON (virq < NUM_ISA_INTERRUPTS);
	WARN_ON (count == 0 || (virq + count) > irq_virq_count);

	if (virq < NUM_ISA_INTERRUPTS) {
		if (virq + count < NUM_ISA_INTERRUPTS)
			return;
		count  =- NUM_ISA_INTERRUPTS - virq;
		virq = NUM_ISA_INTERRUPTS;
	}

	if (count > irq_virq_count || virq > irq_virq_count - count) {
		if (virq > irq_virq_count)
			return;
		count = irq_virq_count - virq;
	}

	raw_spin_lock_irqsave(&irq_big_lock, flags);
	for (i = virq; i < (virq + count); i++) {
		struct irq_host *host;

		host = irq_map[i].host;
		irq_map[i].hwirq = host->inval_irq;
		smp_wmb();
		irq_map[i].host = NULL;
	}
	raw_spin_unlock_irqrestore(&irq_big_lock, flags);
}
=======
=======
}

>>>>>>> refs/remotes/origin/master
irq_hw_number_t virq_to_hw(unsigned int virq)
{
	struct irq_data *irq_data = irq_get_irq_data(virq);
	return WARN_ON(!irq_data) ? 0 : irq_data->hwirq;
}
EXPORT_SYMBOL_GPL(virq_to_hw);

#ifdef CONFIG_SMP
int irq_choose_cpu(const struct cpumask *mask)
{
	int cpuid;

<<<<<<< HEAD
	if (cpumask_equal(mask, cpu_all_mask)) {
=======
	if (cpumask_equal(mask, cpu_online_mask)) {
>>>>>>> refs/remotes/origin/master
		static int irq_rover;
		static DEFINE_RAW_SPINLOCK(irq_rover_lock);
		unsigned long flags;

		/* Round-robin distribution... */
do_round_robin:
		raw_spin_lock_irqsave(&irq_rover_lock, flags);

		irq_rover = cpumask_next(irq_rover, cpu_online_mask);
		if (irq_rover >= nr_cpu_ids)
			irq_rover = cpumask_first(cpu_online_mask);

		cpuid = irq_rover;

		raw_spin_unlock_irqrestore(&irq_rover_lock, flags);
	} else {
		cpuid = cpumask_first_and(mask, cpu_online_mask);
		if (cpuid >= nr_cpu_ids)
			goto do_round_robin;
	}

	return get_hard_smp_processor_id(cpuid);
}
#else
int irq_choose_cpu(const struct cpumask *mask)
{
	return hard_smp_processor_id();
}
#endif
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

int arch_early_irq_init(void)
{
	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_VIRQ_DEBUG
static int virq_debug_show(struct seq_file *m, void *private)
{
	unsigned long flags;
	struct irq_desc *desc;
	const char *p;
	static const char none[] = "none";
	void *data;
	int i;

	seq_printf(m, "%-5s  %-7s  %-15s  %-18s  %s\n", "virq", "hwirq",
		      "chip name", "chip data", "host name");

	for (i = 1; i < nr_irqs; i++) {
		desc = irq_to_desc(i);
		if (!desc)
			continue;

		raw_spin_lock_irqsave(&desc->lock, flags);

		if (desc->action && desc->action->handler) {
			struct irq_chip *chip;

			seq_printf(m, "%5d  ", i);
			seq_printf(m, "0x%05lx  ", irq_map[i].hwirq);

			chip = irq_desc_get_chip(desc);
			if (chip && chip->name)
				p = chip->name;
			else
				p = none;
			seq_printf(m, "%-15s  ", p);

			data = irq_desc_get_chip_data(desc);
			seq_printf(m, "0x%16p  ", data);

			if (irq_map[i].host && irq_map[i].host->of_node)
				p = irq_map[i].host->of_node->full_name;
			else
				p = none;
			seq_printf(m, "%s\n", p);
		}

		raw_spin_unlock_irqrestore(&desc->lock, flags);
	}

	return 0;
}

static int virq_debug_open(struct inode *inode, struct file *file)
{
	return single_open(file, virq_debug_show, inode->i_private);
}

static const struct file_operations virq_debug_fops = {
	.open = virq_debug_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static int __init irq_debugfs_init(void)
{
	if (debugfs_create_file("virq_mapping", S_IRUGO, powerpc_debugfs_root,
				 NULL, &virq_debug_fops) == NULL)
		return -ENOMEM;

	return 0;
}
__initcall(irq_debugfs_init);
#endif /* CONFIG_VIRQ_DEBUG */

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_PPC64
static int __init setup_noirqdistrib(char *str)
{
	distribute_irqs = 0;
	return 1;
}

__setup("noirqdistrib", setup_noirqdistrib);
#endif /* CONFIG_PPC64 */
