/*
 * Common signal handling code for both 32 and 64 bits
 *
 *    Copyright (c) 2007 Benjamin Herrenschmidt, IBM Coproration
 *    Extracted from signal_32.c and signal_64.c
 *
 * This file is subject to the terms and conditions of the GNU General
 * Public License.  See the file README.legal in the main directory of
 * this archive for more details.
 */

#include <linux/tracehook.h>
#include <linux/signal.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/hw_breakpoint.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>
=======
#include <linux/key.h>
=======
#include <linux/uprobes.h>
#include <linux/key.h>
#include <linux/context_tracking.h>
>>>>>>> refs/remotes/origin/master
#include <asm/hw_breakpoint.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>
#include <asm/debug.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm/tm.h>
>>>>>>> refs/remotes/origin/master

#include "signal.h"

/* Log an error when sending an unhandled signal to a process. Controlled
 * through debug.exception-trace sysctl.
 */

<<<<<<< HEAD
int show_unhandled_signals = 0;
=======
int show_unhandled_signals = 1;
>>>>>>> refs/remotes/origin/master

/*
 * Allocate space for the signal frame
 */
<<<<<<< HEAD
void __user * get_sigframe(struct k_sigaction *ka, struct pt_regs *regs,
=======
void __user * get_sigframe(struct k_sigaction *ka, unsigned long sp,
>>>>>>> refs/remotes/origin/master
			   size_t frame_size, int is_32)
{
        unsigned long oldsp, newsp;

        /* Default to using normal stack */
<<<<<<< HEAD
        oldsp = get_clean_sp(regs, is_32);
=======
        oldsp = get_clean_sp(sp, is_32);
>>>>>>> refs/remotes/origin/master

	/* Check for alt stack */
	if ((ka->sa.sa_flags & SA_ONSTACK) &&
	    current->sas_ss_size && !on_sig_stack(oldsp))
		oldsp = (current->sas_ss_sp + current->sas_ss_size);

	/* Get aligned frame */
	newsp = (oldsp - frame_size) & ~0xFUL;

	/* Check access */
	if (!access_ok(VERIFY_WRITE, (void __user *)newsp, oldsp - newsp))
		return NULL;

        return (void __user *)newsp;
}

<<<<<<< HEAD

/*
 * Restore the user process's signal mask
 */
void restore_sigmask(sigset_t *set)
{
	sigdelsetmask(set, ~_BLOCKABLE);
<<<<<<< HEAD
	spin_lock_irq(&current->sighand->siglock);
	current->blocked = *set;
	recalc_sigpending();
	spin_unlock_irq(&current->sighand->siglock);
=======
	set_current_blocked(set);
>>>>>>> refs/remotes/origin/cm-10.0
}

=======
>>>>>>> refs/remotes/origin/master
static void check_syscall_restart(struct pt_regs *regs, struct k_sigaction *ka,
				  int has_handler)
{
	unsigned long ret = regs->gpr[3];
	int restart = 1;

	/* syscall ? */
	if (TRAP(regs) != 0x0C00)
		return;

	/* error signalled ? */
	if (!(regs->ccr & 0x10000000))
		return;

	switch (ret) {
	case ERESTART_RESTARTBLOCK:
	case ERESTARTNOHAND:
		/* ERESTARTNOHAND means that the syscall should only be
		 * restarted if there was no handler for the signal, and since
		 * we only get here if there is a handler, we dont restart.
		 */
		restart = !has_handler;
		break;
	case ERESTARTSYS:
		/* ERESTARTSYS means to restart the syscall if there is no
		 * handler or the handler was registered with SA_RESTART
		 */
		restart = !has_handler || (ka->sa.sa_flags & SA_RESTART) != 0;
		break;
	case ERESTARTNOINTR:
		/* ERESTARTNOINTR means that the syscall should be
		 * called again after the signal handler returns.
		 */
		break;
	default:
		return;
	}
	if (restart) {
		if (ret == ERESTART_RESTARTBLOCK)
			regs->gpr[0] = __NR_restart_syscall;
		else
			regs->gpr[3] = regs->orig_gpr3;
		regs->nip -= 4;
		regs->result = 0;
	} else {
		regs->result = -EINTR;
		regs->gpr[3] = EINTR;
		regs->ccr |= 0x10000000;
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
static int do_signal_pending(sigset_t *oldset, struct pt_regs *regs)
{
=======
static int do_signal(struct pt_regs *regs)
{
	sigset_t *oldset;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int do_signal(struct pt_regs *regs)
{
	sigset_t *oldset = sigmask_to_save();
>>>>>>> refs/remotes/origin/master
	siginfo_t info;
	int signr;
	struct k_sigaction ka;
	int ret;
	int is32 = is_32bit_task();

<<<<<<< HEAD
	if (current_thread_info()->local_flags & _TLF_RESTORE_SIGMASK)
		oldset = &current->saved_sigmask;
<<<<<<< HEAD
	else if (!oldset)
=======
	else
>>>>>>> refs/remotes/origin/cm-10.0
		oldset = &current->blocked;

=======
>>>>>>> refs/remotes/origin/master
	signr = get_signal_to_deliver(&info, &ka, regs, NULL);

	/* Is there any syscall restart business here ? */
	check_syscall_restart(regs, &ka, signr > 0);

	if (signr <= 0) {
<<<<<<< HEAD
		struct thread_info *ti = current_thread_info();
		/* No signal to deliver -- put the saved sigmask back */
		if (ti->local_flags & _TLF_RESTORE_SIGMASK) {
			ti->local_flags &= ~_TLF_RESTORE_SIGMASK;
			sigprocmask(SIG_SETMASK, &current->saved_sigmask, NULL);
		}
=======
		/* No signal to deliver -- put the saved sigmask back */
		restore_saved_sigmask();
>>>>>>> refs/remotes/origin/master
		regs->trap = 0;
		return 0;               /* no signals delivered */
	}

#ifndef CONFIG_PPC_ADV_DEBUG_REGS
        /*
	 * Reenable the DABR before delivering the signal to
	 * user space. The DABR will have been cleared if it
	 * triggered inside the kernel.
	 */
<<<<<<< HEAD
	if (current->thread.dabr)
		set_dabr(current->thread.dabr);
=======
	if (current->thread.hw_brk.address &&
		current->thread.hw_brk.type)
		set_breakpoint(&current->thread.hw_brk);
>>>>>>> refs/remotes/origin/master
#endif
	/* Re-enable the breakpoints for the signal stack */
	thread_change_pc(current, regs);

	if (is32) {
        	if (ka.sa.sa_flags & SA_SIGINFO)
			ret = handle_rt_signal32(signr, &ka, &info, oldset,
					regs);
		else
			ret = handle_signal32(signr, &ka, &info, oldset,
					regs);
	} else {
		ret = handle_rt_signal64(signr, &ka, &info, oldset, regs);
	}

	regs->trap = 0;
	if (ret) {
<<<<<<< HEAD
<<<<<<< HEAD
		spin_lock_irq(&current->sighand->siglock);
		sigorsets(&current->blocked, &current->blocked,
			  &ka.sa.sa_mask);
		if (!(ka.sa.sa_flags & SA_NODEFER))
			sigaddset(&current->blocked, signr);
		recalc_sigpending();
		spin_unlock_irq(&current->sighand->siglock);
=======
		block_sigmask(&ka, signr);
>>>>>>> refs/remotes/origin/cm-10.0

		/*
		 * A signal was successfully delivered; the saved sigmask is in
		 * its frame, and we can clear the TLF_RESTORE_SIGMASK flag.
		 */
		current_thread_info()->local_flags &= ~_TLF_RESTORE_SIGMASK;

		/*
		 * Let tracing know that we've done the handler setup.
		 */
		tracehook_signal_handler(signr, &info, &ka, regs,
=======
		signal_delivered(signr, &info, &ka, regs,
>>>>>>> refs/remotes/origin/master
					 test_thread_flag(TIF_SINGLESTEP));
	}

	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
void do_signal(struct pt_regs *regs, unsigned long thread_info_flags)
{
	if (thread_info_flags & _TIF_SIGPENDING)
		do_signal_pending(NULL, regs);
=======
void do_notify_resume(struct pt_regs *regs, unsigned long thread_info_flags)
{
	if (thread_info_flags & _TIF_SIGPENDING)
		do_signal(regs);
>>>>>>> refs/remotes/origin/cm-10.0
=======
void do_notify_resume(struct pt_regs *regs, unsigned long thread_info_flags)
{
	user_exit();

	if (thread_info_flags & _TIF_UPROBE)
		uprobe_notify_resume(regs);

	if (thread_info_flags & _TIF_SIGPENDING)
		do_signal(regs);
>>>>>>> refs/remotes/origin/master

	if (thread_info_flags & _TIF_NOTIFY_RESUME) {
		clear_thread_flag(TIF_NOTIFY_RESUME);
		tracehook_notify_resume(regs);
<<<<<<< HEAD
<<<<<<< HEAD
=======
		if (current->replacement_session_keyring)
			key_replace_session_keyring();
>>>>>>> refs/remotes/origin/cm-10.0
	}
}

long sys_sigaltstack(const stack_t __user *uss, stack_t __user *uoss,
		unsigned long r5, unsigned long r6, unsigned long r7,
		unsigned long r8, struct pt_regs *regs)
{
	return do_sigaltstack(uss, uoss, regs->gpr[1]);
=======
	}

	user_enter();
}

unsigned long get_tm_stackpointer(struct pt_regs *regs)
{
	/* When in an active transaction that takes a signal, we need to be
	 * careful with the stack.  It's possible that the stack has moved back
	 * up after the tbegin.  The obvious case here is when the tbegin is
	 * called inside a function that returns before a tend.  In this case,
	 * the stack is part of the checkpointed transactional memory state.
	 * If we write over this non transactionally or in suspend, we are in
	 * trouble because if we get a tm abort, the program counter and stack
	 * pointer will be back at the tbegin but our in memory stack won't be
	 * valid anymore.
	 *
	 * To avoid this, when taking a signal in an active transaction, we
	 * need to use the stack pointer from the checkpointed state, rather
	 * than the speculated state.  This ensures that the signal context
	 * (written tm suspended) will be written below the stack required for
	 * the rollback.  The transaction is aborted becuase of the treclaim,
	 * so any memory written between the tbegin and the signal will be
	 * rolled back anyway.
	 *
	 * For signals taken in non-TM or suspended mode, we use the
	 * normal/non-checkpointed stack pointer.
	 */

#ifdef CONFIG_PPC_TRANSACTIONAL_MEM
	if (MSR_TM_ACTIVE(regs->msr)) {
		tm_enable();
		tm_reclaim(&current->thread, regs->msr, TM_CAUSE_SIGNAL);
		if (MSR_TM_TRANSACTIONAL(regs->msr))
			return current->thread.ckpt_regs.gpr[1];
	}
#endif
	return regs->gpr[1];
>>>>>>> refs/remotes/origin/master
}
