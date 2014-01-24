/*
 * This file handles the architecture dependent parts of process handling.
 *
<<<<<<< HEAD
 *    Copyright IBM Corp. 1999,2009
=======
 *    Copyright IBM Corp. 1999, 2009
>>>>>>> refs/remotes/origin/master
 *    Author(s): Martin Schwidefsky <schwidefsky@de.ibm.com>,
 *		 Hartmut Penner <hp@de.ibm.com>,
 *		 Denis Joseph Barrow,
 */

#include <linux/compiler.h>
#include <linux/cpu.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/mm.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/elfcore.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/elfcore.h>
>>>>>>> refs/remotes/origin/master
#include <linux/smp.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/tick.h>
#include <linux/personality.h>
#include <linux/syscalls.h>
#include <linux/compat.h>
#include <linux/kprobes.h>
#include <linux/random.h>
#include <linux/module.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/system.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
#include <asm/io.h>
#include <asm/processor.h>
#include <asm/irq.h>
#include <asm/timer.h>
#include <asm/nmi.h>
#include <asm/smp.h>
<<<<<<< HEAD
=======
#include <asm/switch_to.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm/io.h>
#include <asm/processor.h>
#include <asm/vtimer.h>
#include <asm/exec.h>
#include <asm/irq.h>
#include <asm/nmi.h>
#include <asm/smp.h>
#include <asm/switch_to.h>
#include <asm/runtime_instr.h>
>>>>>>> refs/remotes/origin/master
#include "entry.h"

asmlinkage void ret_from_fork(void) asm ("ret_from_fork");

/*
 * Return saved PC of a blocked thread. used in kernel/sched.
 * resume in entry.S does not create a new stack frame, it
 * just stores the registers %r6-%r15 to the frame given by
 * schedule. We want to return the address of the caller of
 * schedule, so we have to walk the backchain one time to
 * find the frame schedule() store its return address.
 */
unsigned long thread_saved_pc(struct task_struct *tsk)
{
	struct stack_frame *sf, *low, *high;

	if (!tsk || !task_stack_page(tsk))
		return 0;
	low = task_stack_page(tsk);
	high = (struct stack_frame *) task_pt_regs(tsk);
	sf = (struct stack_frame *) (tsk->thread.ksp & PSW_ADDR_INSN);
	if (sf <= low || sf > high)
		return 0;
	sf = (struct stack_frame *) (sf->back_chain & PSW_ADDR_INSN);
	if (sf <= low || sf > high)
		return 0;
	return sf->gprs[8];
}

<<<<<<< HEAD
/*
 * The idle loop on a S390...
 */
static void default_idle(void)
{
	if (cpu_is_offline(smp_processor_id()))
		cpu_die();
	local_irq_disable();
	if (need_resched()) {
		local_irq_enable();
		return;
	}
=======
void arch_cpu_idle(void)
{
>>>>>>> refs/remotes/origin/master
	local_mcck_disable();
	if (test_thread_flag(TIF_MCCK_PENDING)) {
		local_mcck_enable();
		local_irq_enable();
<<<<<<< HEAD
<<<<<<< HEAD
		s390_handle_mcck();
		return;
	}
	trace_hardirqs_on();
	/* Don't trace preempt off for idle. */
	stop_critical_timings();
	/* Stop virtual timer and halt the cpu. */
	vtime_stop_cpu();
	/* Reenable preemption tracer. */
	start_critical_timings();
=======
=======
>>>>>>> refs/remotes/origin/master
		return;
	}
	/* Halt the cpu and keep track of cpu time accounting. */
	vtime_stop_cpu();
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}

void cpu_idle(void)
{
	for (;;) {
<<<<<<< HEAD
		tick_nohz_stop_sched_tick(1);
		while (!need_resched())
			default_idle();
		tick_nohz_restart_sched_tick();
		preempt_enable_no_resched();
		schedule();
		preempt_disable();
=======
		tick_nohz_idle_enter();
		rcu_idle_enter();
		while (!need_resched() && !test_thread_flag(TIF_MCCK_PENDING))
			default_idle();
		rcu_idle_exit();
		tick_nohz_idle_exit();
		if (test_thread_flag(TIF_MCCK_PENDING))
			s390_handle_mcck();
		schedule_preempt_disabled();
>>>>>>> refs/remotes/origin/cm-10.0
	}
}

extern void __kprobes kernel_thread_starter(void);

asm(
	".section .kprobes.text, \"ax\"\n"
	".global kernel_thread_starter\n"
	"kernel_thread_starter:\n"
	"    la    2,0(10)\n"
	"    basr  14,9\n"
	"    la    2,0\n"
	"    br    11\n"
	".previous\n");

int kernel_thread(int (*fn)(void *), void * arg, unsigned long flags)
{
	struct pt_regs regs;

	memset(&regs, 0, sizeof(regs));
<<<<<<< HEAD
	regs.psw.mask = psw_kernel_bits | PSW_MASK_IO | PSW_MASK_EXT;
=======
	regs.psw.mask = psw_kernel_bits |
		PSW_MASK_DAT | PSW_MASK_IO | PSW_MASK_EXT | PSW_MASK_MCHECK;
>>>>>>> refs/remotes/origin/cm-10.0
	regs.psw.addr = (unsigned long) kernel_thread_starter | PSW_ADDR_AMODE;
	regs.gprs[9] = (unsigned long) fn;
	regs.gprs[10] = (unsigned long) arg;
	regs.gprs[11] = (unsigned long) do_exit;
	regs.orig_gpr2 = -1;

	/* Ok, create the new process.. */
	return do_fork(flags | CLONE_VM | CLONE_UNTRACED,
		       0, &regs, 0, NULL, NULL);
}
EXPORT_SYMBOL(kernel_thread);
=======
	local_irq_enable();
}

void arch_cpu_idle_exit(void)
{
	if (test_thread_flag(TIF_MCCK_PENDING))
		s390_handle_mcck();
}

void arch_cpu_idle_dead(void)
{
	cpu_die();
}

extern void __kprobes kernel_thread_starter(void);
>>>>>>> refs/remotes/origin/master

/*
 * Free current thread data structures etc..
 */
void exit_thread(void)
{
<<<<<<< HEAD
=======
	exit_thread_runtime_instr();
>>>>>>> refs/remotes/origin/master
}

void flush_thread(void)
{
}

void release_thread(struct task_struct *dead_task)
{
}

int copy_thread(unsigned long clone_flags, unsigned long new_stackp,
<<<<<<< HEAD
		unsigned long unused,
		struct task_struct *p, struct pt_regs *regs)
=======
		unsigned long arg, struct task_struct *p)
>>>>>>> refs/remotes/origin/master
{
	struct thread_info *ti;
	struct fake_frame
	{
		struct stack_frame sf;
		struct pt_regs childregs;
	} *frame;

	frame = container_of(task_pt_regs(p), struct fake_frame, childregs);
	p->thread.ksp = (unsigned long) frame;
<<<<<<< HEAD
	/* Store access registers to kernel stack of new process. */
	frame->childregs = *regs;
	frame->childregs.gprs[2] = 0;	/* child returns 0 on fork. */
	frame->childregs.gprs[15] = new_stackp;
	frame->sf.back_chain = 0;

	/* new return point is ret_from_fork */
	frame->sf.gprs[8] = (unsigned long) ret_from_fork;

	/* fake return stack for resume(), don't go back to schedule */
	frame->sf.gprs[9] = (unsigned long) frame;

	/* Save access registers to new thread structure. */
	save_access_regs(&p->thread.acrs[0]);
=======
	/* Save access registers to new thread structure. */
	save_access_regs(&p->thread.acrs[0]);
	/* start new process with ar4 pointing to the correct address space */
	p->thread.mm_segment = get_fs();
	/* Don't copy debug registers */
	memset(&p->thread.per_user, 0, sizeof(p->thread.per_user));
	memset(&p->thread.per_event, 0, sizeof(p->thread.per_event));
	clear_tsk_thread_flag(p, TIF_SINGLE_STEP);
	clear_tsk_thread_flag(p, TIF_PER_TRAP);
	/* Initialize per thread user and system timer values */
	ti = task_thread_info(p);
	ti->user_timer = 0;
	ti->system_timer = 0;

	frame->sf.back_chain = 0;
	/* new return point is ret_from_fork */
	frame->sf.gprs[8] = (unsigned long) ret_from_fork;
	/* fake return stack for resume(), don't go back to schedule */
	frame->sf.gprs[9] = (unsigned long) frame;

	/* Store access registers to kernel stack of new process. */
	if (unlikely(p->flags & PF_KTHREAD)) {
		/* kernel thread */
		memset(&frame->childregs, 0, sizeof(struct pt_regs));
		frame->childregs.psw.mask = PSW_KERNEL_BITS | PSW_MASK_DAT |
				PSW_MASK_IO | PSW_MASK_EXT | PSW_MASK_MCHECK;
		frame->childregs.psw.addr = PSW_ADDR_AMODE |
				(unsigned long) kernel_thread_starter;
		frame->childregs.gprs[9] = new_stackp; /* function */
		frame->childregs.gprs[10] = arg;
		frame->childregs.gprs[11] = (unsigned long) do_exit;
		frame->childregs.orig_gpr2 = -1;

		return 0;
	}
	frame->childregs = *current_pt_regs();
	frame->childregs.gprs[2] = 0;	/* child returns 0 on fork. */
	if (new_stackp)
		frame->childregs.gprs[15] = new_stackp;

	/* Don't copy runtime instrumentation info */
	p->thread.ri_cb = NULL;
	p->thread.ri_signum = 0;
	frame->childregs.psw.mask &= ~PSW_MASK_RI;
>>>>>>> refs/remotes/origin/master

#ifndef CONFIG_64BIT
	/*
	 * save fprs to current->thread.fp_regs to merge them with
	 * the emulated registers and then copy the result to the child.
	 */
<<<<<<< HEAD
	save_fp_regs(&current->thread.fp_regs);
=======
	save_fp_ctl(&current->thread.fp_regs.fpc);
	save_fp_regs(current->thread.fp_regs.fprs);
>>>>>>> refs/remotes/origin/master
	memcpy(&p->thread.fp_regs, &current->thread.fp_regs,
	       sizeof(s390_fp_regs));
	/* Set a new TLS ?  */
	if (clone_flags & CLONE_SETTLS)
<<<<<<< HEAD
		p->thread.acrs[0] = regs->gprs[6];
#else /* CONFIG_64BIT */
	/* Save the fpu registers to new thread structure. */
	save_fp_regs(&p->thread.fp_regs);
	/* Set a new TLS ?  */
	if (clone_flags & CLONE_SETTLS) {
		if (is_compat_task()) {
			p->thread.acrs[0] = (unsigned int) regs->gprs[6];
		} else {
			p->thread.acrs[0] = (unsigned int)(regs->gprs[6] >> 32);
			p->thread.acrs[1] = (unsigned int) regs->gprs[6];
		}
	}
#endif /* CONFIG_64BIT */
	/* start new process with ar4 pointing to the correct address space */
	p->thread.mm_segment = get_fs();
	/* Don't copy debug registers */
	memset(&p->thread.per_user, 0, sizeof(p->thread.per_user));
	memset(&p->thread.per_event, 0, sizeof(p->thread.per_event));
	clear_tsk_thread_flag(p, TIF_SINGLE_STEP);
	clear_tsk_thread_flag(p, TIF_PER_TRAP);
	/* Initialize per thread user and system timer values */
	ti = task_thread_info(p);
	ti->user_timer = 0;
	ti->system_timer = 0;
	return 0;
}

SYSCALL_DEFINE0(fork)
{
	struct pt_regs *regs = task_pt_regs(current);
	return do_fork(SIGCHLD, regs->gprs[15], regs, 0, NULL, NULL);
}

SYSCALL_DEFINE4(clone, unsigned long, newsp, unsigned long, clone_flags,
		int __user *, parent_tidptr, int __user *, child_tidptr)
{
	struct pt_regs *regs = task_pt_regs(current);

	if (!newsp)
		newsp = regs->gprs[15];
	return do_fork(clone_flags, newsp, regs, 0,
		       parent_tidptr, child_tidptr);
}

/*
 * This is trivial, and on the face of it looks like it
 * could equally well be done in user mode.
 *
 * Not so, for quite unobvious reasons - register pressure.
 * In user mode vfork() cannot have a stack frame, and if
 * done by calling the "clone()" system call directly, you
 * do not have enough call-clobbered registers to hold all
 * the information you need.
 */
SYSCALL_DEFINE0(vfork)
{
	struct pt_regs *regs = task_pt_regs(current);
	return do_fork(CLONE_VFORK | CLONE_VM | SIGCHLD,
		       regs->gprs[15], regs, 0, NULL, NULL);
}

=======
		p->thread.acrs[0] = frame->childregs.gprs[6];
#else /* CONFIG_64BIT */
	/* Save the fpu registers to new thread structure. */
	save_fp_ctl(&p->thread.fp_regs.fpc);
	save_fp_regs(p->thread.fp_regs.fprs);
	p->thread.fp_regs.pad = 0;
	/* Set a new TLS ?  */
	if (clone_flags & CLONE_SETTLS) {
		unsigned long tls = frame->childregs.gprs[6];
		if (is_compat_task()) {
			p->thread.acrs[0] = (unsigned int)tls;
		} else {
			p->thread.acrs[0] = (unsigned int)(tls >> 32);
			p->thread.acrs[1] = (unsigned int)tls;
		}
	}
#endif /* CONFIG_64BIT */
	return 0;
}

>>>>>>> refs/remotes/origin/master
asmlinkage void execve_tail(void)
{
	current->thread.fp_regs.fpc = 0;
	if (MACHINE_HAS_IEEE)
		asm volatile("sfpc %0,%0" : : "d" (0));
}

/*
<<<<<<< HEAD
 * sys_execve() executes a new program.
 */
SYSCALL_DEFINE3(execve, const char __user *, name,
		const char __user *const __user *, argv,
		const char __user *const __user *, envp)
{
	struct pt_regs *regs = task_pt_regs(current);
	char *filename;
	long rc;

	filename = getname(name);
	rc = PTR_ERR(filename);
	if (IS_ERR(filename))
		return rc;
	rc = do_execve(filename, argv, envp, regs);
	if (rc)
		goto out;
	execve_tail();
	rc = regs->gprs[2];
out:
	putname(filename);
	return rc;
}

/*
=======
>>>>>>> refs/remotes/origin/master
 * fill in the FPU structure for a core dump.
 */
int dump_fpu (struct pt_regs * regs, s390_fp_regs *fpregs)
{
#ifndef CONFIG_64BIT
	/*
	 * save fprs to current->thread.fp_regs to merge them with
	 * the emulated registers and then copy the result to the dump.
	 */
<<<<<<< HEAD
	save_fp_regs(&current->thread.fp_regs);
	memcpy(fpregs, &current->thread.fp_regs, sizeof(s390_fp_regs));
#else /* CONFIG_64BIT */
	save_fp_regs(fpregs);
=======
	save_fp_ctl(&current->thread.fp_regs.fpc);
	save_fp_regs(current->thread.fp_regs.fprs);
	memcpy(fpregs, &current->thread.fp_regs, sizeof(s390_fp_regs));
#else /* CONFIG_64BIT */
	save_fp_ctl(&fpregs->fpc);
	save_fp_regs(fpregs->fprs);
>>>>>>> refs/remotes/origin/master
#endif /* CONFIG_64BIT */
	return 1;
}
EXPORT_SYMBOL(dump_fpu);

unsigned long get_wchan(struct task_struct *p)
{
	struct stack_frame *sf, *low, *high;
	unsigned long return_address;
	int count;

	if (!p || p == current || p->state == TASK_RUNNING || !task_stack_page(p))
		return 0;
	low = task_stack_page(p);
	high = (struct stack_frame *) task_pt_regs(p);
	sf = (struct stack_frame *) (p->thread.ksp & PSW_ADDR_INSN);
	if (sf <= low || sf > high)
		return 0;
	for (count = 0; count < 16; count++) {
		sf = (struct stack_frame *) (sf->back_chain & PSW_ADDR_INSN);
		if (sf <= low || sf > high)
			return 0;
		return_address = sf->gprs[8] & PSW_ADDR_INSN;
		if (!in_sched_functions(return_address))
			return return_address;
	}
	return 0;
}

unsigned long arch_align_stack(unsigned long sp)
{
	if (!(current->personality & ADDR_NO_RANDOMIZE) && randomize_va_space)
		sp -= get_random_int() & ~PAGE_MASK;
	return sp & ~0xf;
}

static inline unsigned long brk_rnd(void)
{
	/* 8MB for 32bit, 1GB for 64bit */
	if (is_32bit_task())
		return (get_random_int() & 0x7ffUL) << PAGE_SHIFT;
	else
		return (get_random_int() & 0x3ffffUL) << PAGE_SHIFT;
}

unsigned long arch_randomize_brk(struct mm_struct *mm)
{
<<<<<<< HEAD
	unsigned long ret = PAGE_ALIGN(mm->brk + brk_rnd());

	if (ret < mm->brk)
		return mm->brk;
	return ret;
=======
	unsigned long ret;

	ret = PAGE_ALIGN(mm->brk + brk_rnd());
	return (ret > mm->brk) ? ret : mm->brk;
>>>>>>> refs/remotes/origin/master
}

unsigned long randomize_et_dyn(unsigned long base)
{
<<<<<<< HEAD
	unsigned long ret = PAGE_ALIGN(base + brk_rnd());

	if (!(current->flags & PF_RANDOMIZE))
		return base;
	if (ret < base)
		return base;
	return ret;
=======
	unsigned long ret;

	if (!(current->flags & PF_RANDOMIZE))
		return base;
	ret = PAGE_ALIGN(base + brk_rnd());
	return (ret > base) ? ret : base;
>>>>>>> refs/remotes/origin/master
}
