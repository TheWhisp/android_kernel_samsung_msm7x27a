/*
 * Copyright (C) 2008-2009 Michal Simek <monstr@monstr.eu>
 * Copyright (C) 2008-2009 PetaLogix
 * Copyright (C) 2006 Atmark Techno, Inc.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <linux/sched.h>
#include <linux/pm.h>
#include <linux/tick.h>
#include <linux/bitops.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/system.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
#include <asm/pgalloc.h>
#include <asm/uaccess.h> /* for USER_DS macros */
=======
#include <linux/ptrace.h>
#include <asm/pgalloc.h>
#include <linux/uaccess.h> /* for USER_DS macros */
>>>>>>> refs/remotes/origin/master
#include <asm/cacheflush.h>

void show_regs(struct pt_regs *regs)
{
<<<<<<< HEAD
	printk(KERN_INFO " Registers dump: mode=%X\r\n", regs->pt_mode);
	printk(KERN_INFO " r1=%08lX, r2=%08lX, r3=%08lX, r4=%08lX\n",
				regs->r1, regs->r2, regs->r3, regs->r4);
	printk(KERN_INFO " r5=%08lX, r6=%08lX, r7=%08lX, r8=%08lX\n",
				regs->r5, regs->r6, regs->r7, regs->r8);
	printk(KERN_INFO " r9=%08lX, r10=%08lX, r11=%08lX, r12=%08lX\n",
				regs->r9, regs->r10, regs->r11, regs->r12);
	printk(KERN_INFO " r13=%08lX, r14=%08lX, r15=%08lX, r16=%08lX\n",
				regs->r13, regs->r14, regs->r15, regs->r16);
	printk(KERN_INFO " r17=%08lX, r18=%08lX, r19=%08lX, r20=%08lX\n",
				regs->r17, regs->r18, regs->r19, regs->r20);
	printk(KERN_INFO " r21=%08lX, r22=%08lX, r23=%08lX, r24=%08lX\n",
				regs->r21, regs->r22, regs->r23, regs->r24);
	printk(KERN_INFO " r25=%08lX, r26=%08lX, r27=%08lX, r28=%08lX\n",
				regs->r25, regs->r26, regs->r27, regs->r28);
	printk(KERN_INFO " r29=%08lX, r30=%08lX, r31=%08lX, rPC=%08lX\n",
				regs->r29, regs->r30, regs->r31, regs->pc);
	printk(KERN_INFO " msr=%08lX, ear=%08lX, esr=%08lX, fsr=%08lX\n",
				regs->msr, regs->ear, regs->esr, regs->fsr);
}

void (*pm_idle)(void);
void (*pm_power_off)(void) = NULL;
EXPORT_SYMBOL(pm_power_off);

static int hlt_counter = 1;

void disable_hlt(void)
{
	hlt_counter++;
}
EXPORT_SYMBOL(disable_hlt);

void enable_hlt(void)
{
	hlt_counter--;
}
EXPORT_SYMBOL(enable_hlt);

static int __init nohlt_setup(char *__unused)
{
	hlt_counter = 1;
	return 1;
}
__setup("nohlt", nohlt_setup);

static int __init hlt_setup(char *__unused)
{
	hlt_counter = 0;
	return 1;
}
__setup("hlt", hlt_setup);

void default_idle(void)
{
	if (likely(hlt_counter)) {
		local_irq_disable();
		stop_critical_timings();
		cpu_relax();
		start_critical_timings();
		local_irq_enable();
	} else {
		clear_thread_flag(TIF_POLLING_NRFLAG);
		smp_mb__after_clear_bit();
		local_irq_disable();
		while (!need_resched())
			cpu_sleep();
		local_irq_enable();
		set_thread_flag(TIF_POLLING_NRFLAG);
	}
}

void cpu_idle(void)
{
	set_thread_flag(TIF_POLLING_NRFLAG);

	/* endless idle loop with no priority at all */
	while (1) {
		void (*idle)(void) = pm_idle;

		if (!idle)
			idle = default_idle;

<<<<<<< HEAD
		tick_nohz_stop_sched_tick(1);
		while (!need_resched())
			idle();
		tick_nohz_restart_sched_tick();

		preempt_enable_no_resched();
		schedule();
		preempt_disable();
=======
		tick_nohz_idle_enter();
		rcu_idle_enter();
		while (!need_resched())
			idle();
		rcu_idle_exit();
		tick_nohz_idle_exit();

		schedule_preempt_disabled();
>>>>>>> refs/remotes/origin/cm-10.0
		check_pgt_cache();
	}
}

=======
	show_regs_print_info(KERN_INFO);

	pr_info(" Registers dump: mode=%X\r\n", regs->pt_mode);
	pr_info(" r1=%08lX, r2=%08lX, r3=%08lX, r4=%08lX\n",
				regs->r1, regs->r2, regs->r3, regs->r4);
	pr_info(" r5=%08lX, r6=%08lX, r7=%08lX, r8=%08lX\n",
				regs->r5, regs->r6, regs->r7, regs->r8);
	pr_info(" r9=%08lX, r10=%08lX, r11=%08lX, r12=%08lX\n",
				regs->r9, regs->r10, regs->r11, regs->r12);
	pr_info(" r13=%08lX, r14=%08lX, r15=%08lX, r16=%08lX\n",
				regs->r13, regs->r14, regs->r15, regs->r16);
	pr_info(" r17=%08lX, r18=%08lX, r19=%08lX, r20=%08lX\n",
				regs->r17, regs->r18, regs->r19, regs->r20);
	pr_info(" r21=%08lX, r22=%08lX, r23=%08lX, r24=%08lX\n",
				regs->r21, regs->r22, regs->r23, regs->r24);
	pr_info(" r25=%08lX, r26=%08lX, r27=%08lX, r28=%08lX\n",
				regs->r25, regs->r26, regs->r27, regs->r28);
	pr_info(" r29=%08lX, r30=%08lX, r31=%08lX, rPC=%08lX\n",
				regs->r29, regs->r30, regs->r31, regs->pc);
	pr_info(" msr=%08lX, ear=%08lX, esr=%08lX, fsr=%08lX\n",
				regs->msr, regs->ear, regs->esr, regs->fsr);
}

void (*pm_power_off)(void) = NULL;
EXPORT_SYMBOL(pm_power_off);

>>>>>>> refs/remotes/origin/master
void flush_thread(void)
{
}

int copy_thread(unsigned long clone_flags, unsigned long usp,
<<<<<<< HEAD
		unsigned long unused,
		struct task_struct *p, struct pt_regs *regs)
=======
		unsigned long arg, struct task_struct *p)
>>>>>>> refs/remotes/origin/master
{
	struct pt_regs *childregs = task_pt_regs(p);
	struct thread_info *ti = task_thread_info(p);

<<<<<<< HEAD
	*childregs = *regs;
	if (user_mode(regs))
		childregs->r1 = usp;
	else
		childregs->r1 = ((unsigned long) ti) + THREAD_SIZE;

#ifndef CONFIG_MMU
	memset(&ti->cpu_context, 0, sizeof(struct cpu_context));
	ti->cpu_context.r1 = (unsigned long)childregs;
	ti->cpu_context.msr = (unsigned long)childregs->msr;
#else

	/* if creating a kernel thread then update the current reg (we don't
	 * want to use the parent's value when restoring by POP_STATE) */
	if (kernel_mode(regs))
		/* save new current on stack to use POP_STATE */
		childregs->CURRENT_TASK = (unsigned long)p;
	/* if returning to user then use the parent's value of this register */

	/* if we're creating a new kernel thread then just zeroing all
	 * the registers. That's OK for a brand new thread.*/
	/* Pls. note that some of them will be restored in POP_STATE */
	if (kernel_mode(regs))
		memset(&ti->cpu_context, 0, sizeof(struct cpu_context));
	/* if this thread is created for fork/vfork/clone, then we want to
	 * restore all the parent's context */
	/* in addition to the registers which will be restored by POP_STATE */
	else {
		ti->cpu_context = *(struct cpu_context *)regs;
		childregs->msr |= MSR_UMS;
	}

	/* FIXME STATE_SAVE_PT_OFFSET; */
	ti->cpu_context.r1  = (unsigned long)childregs;
=======
	if (unlikely(p->flags & PF_KTHREAD)) {
		/* if we're creating a new kernel thread then just zeroing all
		 * the registers. That's OK for a brand new thread.*/
		memset(childregs, 0, sizeof(struct pt_regs));
		memset(&ti->cpu_context, 0, sizeof(struct cpu_context));
		ti->cpu_context.r1  = (unsigned long)childregs;
		ti->cpu_context.r20 = (unsigned long)usp; /* fn */
		ti->cpu_context.r19 = (unsigned long)arg;
		childregs->pt_mode = 1;
		local_save_flags(childregs->msr);
#ifdef CONFIG_MMU
		ti->cpu_context.msr = childregs->msr & ~MSR_IE;
#endif
		ti->cpu_context.r15 = (unsigned long)ret_from_kernel_thread - 8;
		return 0;
	}
	*childregs = *current_pt_regs();
	if (usp)
		childregs->r1 = usp;

	memset(&ti->cpu_context, 0, sizeof(struct cpu_context));
	ti->cpu_context.r1 = (unsigned long)childregs;
#ifndef CONFIG_MMU
	ti->cpu_context.msr = (unsigned long)childregs->msr;
#else
	childregs->msr |= MSR_UMS;

>>>>>>> refs/remotes/origin/master
	/* we should consider the fact that childregs is a copy of the parent
	 * regs which were saved immediately after entering the kernel state
	 * before enabling VM. This MSR will be restored in switch_to and
	 * RETURN() and we want to have the right machine state there
	 * specifically this state must have INTs disabled before and enabled
	 * after performing rtbd
	 * compose the right MSR for RETURN(). It will work for switch_to also
	 * excepting for VM and UMS
	 * don't touch UMS , CARRY and cache bits
	 * right now MSR is a copy of parent one */
<<<<<<< HEAD
	childregs->msr |= MSR_BIP;
=======
>>>>>>> refs/remotes/origin/master
	childregs->msr &= ~MSR_EIP;
	childregs->msr |= MSR_IE;
	childregs->msr &= ~MSR_VM;
	childregs->msr |= MSR_VMS;
	childregs->msr |= MSR_EE; /* exceptions will be enabled*/

	ti->cpu_context.msr = (childregs->msr|MSR_VM);
	ti->cpu_context.msr &= ~MSR_UMS; /* switch_to to kernel mode */
<<<<<<< HEAD
<<<<<<< HEAD
=======
	ti->cpu_context.msr &= ~MSR_IE;
>>>>>>> refs/remotes/origin/cm-10.0
#endif
	ti->cpu_context.r15 = (unsigned long)ret_from_fork - 8;

	if (clone_flags & CLONE_SETTLS)
		;
=======
	ti->cpu_context.msr &= ~MSR_IE;
#endif
	ti->cpu_context.r15 = (unsigned long)ret_from_fork - 8;

	/*
	 *  r21 is the thread reg, r10 is 6th arg to clone
	 *  which contains TLS area
	 */
	if (clone_flags & CLONE_SETTLS)
		childregs->r21 = childregs->r10;
>>>>>>> refs/remotes/origin/master

	return 0;
}

#ifndef CONFIG_MMU
/*
 * Return saved PC of a blocked thread.
 */
unsigned long thread_saved_pc(struct task_struct *tsk)
{
	struct cpu_context *ctx =
		&(((struct thread_info *)(tsk->stack))->cpu_context);

	/* Check whether the thread is blocked in resume() */
	if (in_sched_functions(ctx->r15))
		return (unsigned long)ctx->r15;
	else
		return ctx->r14;
}
#endif

<<<<<<< HEAD
static void kernel_thread_helper(int (*fn)(void *), void *arg)
{
	fn(arg);
	do_exit(-1);
}

int kernel_thread(int (*fn)(void *), void *arg, unsigned long flags)
{
	struct pt_regs regs;

	memset(&regs, 0, sizeof(regs));
	/* store them in non-volatile registers */
	regs.r5 = (unsigned long)fn;
	regs.r6 = (unsigned long)arg;
	local_save_flags(regs.msr);
	regs.pc = (unsigned long)kernel_thread_helper;
	regs.pt_mode = 1;

	return do_fork(flags | CLONE_VM | CLONE_UNTRACED, 0,
			&regs, 0, NULL, NULL);
}
EXPORT_SYMBOL_GPL(kernel_thread);

=======
>>>>>>> refs/remotes/origin/master
unsigned long get_wchan(struct task_struct *p)
{
/* TBD (used by procfs) */
	return 0;
}

/* Set up a thread for executing a new program */
void start_thread(struct pt_regs *regs, unsigned long pc, unsigned long usp)
{
<<<<<<< HEAD
<<<<<<< HEAD
	set_fs(USER_DS);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	regs->pc = pc;
	regs->r1 = usp;
	regs->pt_mode = 0;
#ifdef CONFIG_MMU
	regs->msr |= MSR_UMS;
<<<<<<< HEAD
=======
	regs->msr &= ~MSR_VM;
>>>>>>> refs/remotes/origin/master
#endif
}

#ifdef CONFIG_MMU
#include <linux/elfcore.h>
/*
 * Set up a thread for executing a new program
 */
int dump_fpu(struct pt_regs *regs, elf_fpregset_t *fpregs)
{
	return 0; /* MicroBlaze has no separate FPU registers */
}
#endif /* CONFIG_MMU */
<<<<<<< HEAD
=======

void arch_cpu_idle(void)
{
       local_irq_enable();
}
>>>>>>> refs/remotes/origin/master
