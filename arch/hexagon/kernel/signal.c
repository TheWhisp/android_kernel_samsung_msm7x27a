/*
 * Signal support for Hexagon processor
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (c) 2010-2011, The Linux Foundation. All rights reserved.
=======
 * Copyright (c) 2010-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/master
=======
 * Copyright (c) 2010-2011, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-11.0
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include <linux/linkage.h>
#include <linux/syscalls.h>
<<<<<<< HEAD
#include <linux/freezer.h>
=======
>>>>>>> refs/remotes/origin/master
#include <linux/tracehook.h>
#include <asm/registers.h>
#include <asm/thread_info.h>
#include <asm/unistd.h>
#include <asm/uaccess.h>
#include <asm/ucontext.h>
#include <asm/cacheflush.h>
#include <asm/signal.h>
#include <asm/vdso.h>

<<<<<<< HEAD
#define _BLOCKABLE (~(sigmask(SIGKILL) | sigmask(SIGSTOP)))

=======
>>>>>>> refs/remotes/origin/master
struct rt_sigframe {
	unsigned long tramp[2];
	struct siginfo info;
	struct ucontext uc;
};

static void __user *get_sigframe(struct k_sigaction *ka, struct pt_regs *regs,
			  size_t frame_size)
{
	unsigned long sp = regs->r29;

<<<<<<< HEAD
=======
	/* check if we would overflow the alt stack */
	if (on_sig_stack(sp) && !likely(on_sig_stack(sp - frame_size)))
		return (void __user __force *)-1UL;

>>>>>>> refs/remotes/origin/master
	/* Switch to signal stack if appropriate */
	if ((ka->sa.sa_flags & SA_ONSTACK) && (sas_ss_flags(sp) == 0))
		sp = current->sas_ss_sp + current->sas_ss_size;

	return (void __user *)((sp - frame_size) & ~(sizeof(long long) - 1));
}

static int setup_sigcontext(struct pt_regs *regs, struct sigcontext __user *sc)
{
	unsigned long tmp;
	int err = 0;

	err |= copy_to_user(&sc->sc_regs.r0, &regs->r00,
			    32*sizeof(unsigned long));

	err |= __put_user(regs->sa0, &sc->sc_regs.sa0);
	err |= __put_user(regs->lc0, &sc->sc_regs.lc0);
	err |= __put_user(regs->sa1, &sc->sc_regs.sa1);
	err |= __put_user(regs->lc1, &sc->sc_regs.lc1);
	err |= __put_user(regs->m0, &sc->sc_regs.m0);
	err |= __put_user(regs->m1, &sc->sc_regs.m1);
	err |= __put_user(regs->usr, &sc->sc_regs.usr);
	err |= __put_user(regs->preds, &sc->sc_regs.p3_0);
	err |= __put_user(regs->gp, &sc->sc_regs.gp);
	err |= __put_user(regs->ugp, &sc->sc_regs.ugp);
<<<<<<< HEAD

=======
#if CONFIG_HEXAGON_ARCH_VERSION >= 4
	err |= __put_user(regs->cs0, &sc->sc_regs.cs0);
	err |= __put_user(regs->cs1, &sc->sc_regs.cs1);
#endif
>>>>>>> refs/remotes/origin/master
	tmp = pt_elr(regs); err |= __put_user(tmp, &sc->sc_regs.pc);
	tmp = pt_cause(regs); err |= __put_user(tmp, &sc->sc_regs.cause);
	tmp = pt_badva(regs); err |= __put_user(tmp, &sc->sc_regs.badva);

	return err;
}

static int restore_sigcontext(struct pt_regs *regs,
			      struct sigcontext __user *sc)
{
	unsigned long tmp;
	int err = 0;

	err |= copy_from_user(&regs->r00, &sc->sc_regs.r0,
			      32 * sizeof(unsigned long));

	err |= __get_user(regs->sa0, &sc->sc_regs.sa0);
	err |= __get_user(regs->lc0, &sc->sc_regs.lc0);
	err |= __get_user(regs->sa1, &sc->sc_regs.sa1);
	err |= __get_user(regs->lc1, &sc->sc_regs.lc1);
	err |= __get_user(regs->m0, &sc->sc_regs.m0);
	err |= __get_user(regs->m1, &sc->sc_regs.m1);
	err |= __get_user(regs->usr, &sc->sc_regs.usr);
	err |= __get_user(regs->preds, &sc->sc_regs.p3_0);
	err |= __get_user(regs->gp, &sc->sc_regs.gp);
	err |= __get_user(regs->ugp, &sc->sc_regs.ugp);
<<<<<<< HEAD

=======
#if CONFIG_HEXAGON_ARCH_VERSION >= 4
	err |= __get_user(regs->cs0, &sc->sc_regs.cs0);
	err |= __get_user(regs->cs1, &sc->sc_regs.cs1);
#endif
>>>>>>> refs/remotes/origin/master
	err |= __get_user(tmp, &sc->sc_regs.pc); pt_set_elr(regs, tmp);

	return err;
}

/*
 * Setup signal stack frame with siginfo structure
 */
static int setup_rt_frame(int signr, struct k_sigaction *ka, siginfo_t *info,
			  sigset_t *set,  struct pt_regs *regs)
{
	int err = 0;
	struct rt_sigframe __user *frame;
	struct hexagon_vdso *vdso = current->mm->context.vdso;

	frame = get_sigframe(ka, regs, sizeof(struct rt_sigframe));

	if (!access_ok(VERIFY_WRITE, frame, sizeof(struct rt_sigframe)))
		goto	sigsegv;

	if (copy_siginfo_to_user(&frame->info, info))
		goto	sigsegv;

	/* The on-stack signal trampoline is no longer executed;
	 * however, the libgcc signal frame unwinding code checks for
	 * the presence of these two numeric magic values.
	 */
	err |= __put_user(0x7800d166, &frame->tramp[0]);
	err |= __put_user(0x5400c004, &frame->tramp[1]);
	err |= setup_sigcontext(regs, &frame->uc.uc_mcontext);
	err |= __copy_to_user(&frame->uc.uc_sigmask, set, sizeof(*set));
<<<<<<< HEAD
=======
	err |= __save_altstack(&frame->uc.uc_stack, user_stack_pointer(regs));
>>>>>>> refs/remotes/origin/master
	if (err)
		goto sigsegv;

	/* Load r0/r1 pair with signumber/siginfo pointer... */
	regs->r0100 = ((unsigned long long)((unsigned long)&frame->info) << 32)
		| (unsigned long long)signr;
	regs->r02 = (unsigned long) &frame->uc;
	regs->r31 = (unsigned long) vdso->rt_signal_trampoline;
	pt_psp(regs) = (unsigned long) frame;
	pt_set_elr(regs, (unsigned long)ka->sa.sa_handler);

	return 0;

sigsegv:
	force_sigsegv(signr, current);
	return -EFAULT;
}

/*
 * Setup invocation of signal handler
 */
<<<<<<< HEAD
static int handle_signal(int sig, siginfo_t *info, struct k_sigaction *ka,
			 sigset_t *oldset, struct pt_regs *regs)
{
	int rc;

=======
static void handle_signal(int sig, siginfo_t *info, struct k_sigaction *ka,
			 struct pt_regs *regs)
{
>>>>>>> refs/remotes/origin/master
	/*
	 * If we're handling a signal that aborted a system call,
	 * set up the error return value before adding the signal
	 * frame to the stack.
	 */

	if (regs->syscall_nr >= 0) {
		switch (regs->r00) {
		case -ERESTART_RESTARTBLOCK:
		case -ERESTARTNOHAND:
			regs->r00 = -EINTR;
			break;
		case -ERESTARTSYS:
			if (!(ka->sa.sa_flags & SA_RESTART)) {
				regs->r00 = -EINTR;
				break;
			}
			/* Fall through */
		case -ERESTARTNOINTR:
			regs->r06 = regs->syscall_nr;
			pt_set_elr(regs, pt_elr(regs) - 4);
			regs->r00 = regs->restart_r0;
			break;
		default:
			break;
		}
	}

	/*
	 * Set up the stack frame; not doing the SA_SIGINFO thing.  We
	 * only set up the rt_frame flavor.
	 */
<<<<<<< HEAD
	rc = setup_rt_frame(sig, ka, info, oldset, regs);

	/* If there was an error on setup, no signal was delivered. */
	if (rc)
		return rc;

	block_sigmask(ka, sig);

	return 0;
=======
	/* If there was an error on setup, no signal was delivered. */
	if (setup_rt_frame(sig, ka, info, sigmask_to_save(), regs) < 0)
		return;

	signal_delivered(sig, info, ka, regs,
			test_thread_flag(TIF_SINGLESTEP));
>>>>>>> refs/remotes/origin/master
}

/*
 * Called from return-from-event code.
 */
<<<<<<< HEAD
static void do_signal(struct pt_regs *regs)
=======
void do_signal(struct pt_regs *regs)
>>>>>>> refs/remotes/origin/master
{
	struct k_sigaction sigact;
	siginfo_t info;
	int signo;

	if (!user_mode(regs))
		return;

<<<<<<< HEAD
	if (try_to_freeze())
		goto no_signal;

	signo = get_signal_to_deliver(&info, &sigact, regs, NULL);

	if (signo > 0) {
		sigset_t *oldset;

		if (test_thread_flag(TIF_RESTORE_SIGMASK))
			oldset = &current->saved_sigmask;
		else
			oldset = &current->blocked;

		if (handle_signal(signo, &info, &sigact, oldset, regs) == 0) {
			/*
			 * Successful delivery case.  The saved sigmask is
			 * stored in the signal frame, and will be restored
			 * by sigreturn.  We can clear the TIF flag.
			 */
			clear_thread_flag(TIF_RESTORE_SIGMASK);

			tracehook_signal_handler(signo, &info, &sigact, regs,
				test_thread_flag(TIF_SINGLESTEP));
		}
		return;
	}

no_signal:
	/*
	 * If we came from a system call, handle the restart.
	 */
=======
	signo = get_signal_to_deliver(&info, &sigact, regs, NULL);

	if (signo > 0) {
		handle_signal(signo, &info, &sigact, regs);
		return;
	}

	/*
	 * No (more) signals; if we came from a system call, handle the restart.
	 */

>>>>>>> refs/remotes/origin/master
	if (regs->syscall_nr >= 0) {
		switch (regs->r00) {
		case -ERESTARTNOHAND:
		case -ERESTARTSYS:
		case -ERESTARTNOINTR:
			regs->r06 = regs->syscall_nr;
			break;
		case -ERESTART_RESTARTBLOCK:
			regs->r06 = __NR_restart_syscall;
			break;
		default:
			goto no_restart;
		}
		pt_set_elr(regs, pt_elr(regs) - 4);
		regs->r00 = regs->restart_r0;
	}

no_restart:
	/* If there's no signal to deliver, put the saved sigmask back */
<<<<<<< HEAD
	if (test_thread_flag(TIF_RESTORE_SIGMASK)) {
		clear_thread_flag(TIF_RESTORE_SIGMASK);
		sigprocmask(SIG_SETMASK, &current->saved_sigmask, NULL);
	}
}

void do_notify_resume(struct pt_regs *regs, unsigned long thread_info_flags)
{
	if (thread_info_flags & _TIF_SIGPENDING)
		do_signal(regs);

	if (thread_info_flags & _TIF_NOTIFY_RESUME) {
		clear_thread_flag(TIF_NOTIFY_RESUME);
		if (current->replacement_session_keyring)
			key_replace_session_keyring();
	}
=======
	restore_saved_sigmask();
>>>>>>> refs/remotes/origin/master
}

/*
 * Architecture-specific wrappers for signal-related system calls
 */
<<<<<<< HEAD
asmlinkage int sys_sigaltstack(const stack_t __user *uss, stack_t __user *uoss)
{
	struct pt_regs *regs = current_thread_info()->regs;

	return do_sigaltstack(uss, uoss, regs->r29);
}

asmlinkage int sys_rt_sigreturn(void)
{
	struct pt_regs *regs = current_thread_info()->regs;
	struct rt_sigframe __user *frame;
	sigset_t blocked;

=======

asmlinkage int sys_rt_sigreturn(void)
{
	struct pt_regs *regs = current_pt_regs();
	struct rt_sigframe __user *frame;
	sigset_t blocked;

	/* Always make any pending restarted system calls return -EINTR */
	current_thread_info()->restart_block.fn = do_no_restart_syscall;

>>>>>>> refs/remotes/origin/master
	frame = (struct rt_sigframe __user *)pt_psp(regs);
	if (!access_ok(VERIFY_READ, frame, sizeof(*frame)))
		goto badframe;
	if (__copy_from_user(&blocked, &frame->uc.uc_sigmask, sizeof(blocked)))
		goto badframe;

<<<<<<< HEAD
	sigdelsetmask(&blocked, ~_BLOCKABLE);
=======
>>>>>>> refs/remotes/origin/master
	set_current_blocked(&blocked);

	if (restore_sigcontext(regs, &frame->uc.uc_mcontext))
		goto badframe;

	/* Restore the user's stack as well */
	pt_psp(regs) = regs->r29;

<<<<<<< HEAD
	/*
	 * Leave a trace in the stack frame that this was a sigreturn.
	 * If the system call is to replay, we've already restored the
	 * number in the GPR slot and it will be regenerated on the
	 * new system call trap entry. Note that if restore_sigcontext()
	 * did something other than a bulk copy of the pt_regs struct,
	 * we could avoid this assignment by simply not overwriting
	 * regs->syscall_nr.
	 */
	regs->syscall_nr = __NR_rt_sigreturn;

	/*
	 * If we were meticulous, we'd only call this if we knew that
	 * we were actually going to use an alternate stack, and we'd
	 * consider any error to be fatal.  What we do here, in common
	 * with many other architectures, is call it blindly and only
	 * consider the -EFAULT return case to be proof of a problem.
	 */
	if (do_sigaltstack(&frame->uc.uc_stack, NULL, pt_psp(regs)) == -EFAULT)
		goto badframe;

	return 0;
=======
	regs->syscall_nr = -1;

	if (restore_altstack(&frame->uc.uc_stack))
		goto badframe;

	return regs->r00;
>>>>>>> refs/remotes/origin/master

badframe:
	force_sig(SIGSEGV, current);
	return 0;
}
