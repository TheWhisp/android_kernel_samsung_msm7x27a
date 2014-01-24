/*
 * Copyright (C) 2004 PathScale, Inc
 * Copyright (C) 2004 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <signal.h>
#include <strings.h>
<<<<<<< HEAD
#include "as-layout.h"
#include "kern_util.h"
#include "os.h"
<<<<<<< HEAD
#include "process.h"
#include "sysdep/barrier.h"
#include "sysdep/sigcontext.h"
#include "user.h"

/* Copied from linux/compiler-gcc.h since we can't include it directly */
#define barrier() __asm__ __volatile__("": : :"memory")
=======
#include "sysdep/mcontext.h"
>>>>>>> refs/remotes/origin/cm-10.0

void (*sig_info[NSIG])(int, struct uml_pt_regs *) = {
=======
#include <as-layout.h>
#include <kern_util.h>
#include <os.h>
#include <sysdep/mcontext.h>
#include "internal.h"

void (*sig_info[NSIG])(int, struct siginfo *, struct uml_pt_regs *) = {
>>>>>>> refs/remotes/origin/master
	[SIGTRAP]	= relay_signal,
	[SIGFPE]	= relay_signal,
	[SIGILL]	= relay_signal,
	[SIGWINCH]	= winch,
	[SIGBUS]	= bus_handler,
	[SIGSEGV]	= segv_handler,
	[SIGIO]		= sigio_handler,
	[SIGVTALRM]	= timer_handler };

<<<<<<< HEAD
<<<<<<< HEAD
static void sig_handler_common(int sig, struct sigcontext *sc)
=======
static void sig_handler_common(int sig, mcontext_t *mc)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void sig_handler_common(int sig, struct siginfo *si, mcontext_t *mc)
>>>>>>> refs/remotes/origin/master
{
	struct uml_pt_regs r;
	int save_errno = errno;

	r.is_user = 0;
	if (sig == SIGSEGV) {
		/* For segfaults, we want the data from the sigcontext. */
<<<<<<< HEAD
<<<<<<< HEAD
		copy_sc(&r, sc);
		GET_FAULTINFO_FROM_SC(r.faultinfo, sc);
=======
		get_regs_from_mc(&r, mc);
		GET_FAULTINFO_FROM_MC(r.faultinfo, mc);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		get_regs_from_mc(&r, mc);
		GET_FAULTINFO_FROM_MC(r.faultinfo, mc);
>>>>>>> refs/remotes/origin/master
	}

	/* enable signals if sig isn't IRQ signal */
	if ((sig != SIGIO) && (sig != SIGWINCH) && (sig != SIGVTALRM))
		unblock_signals();

<<<<<<< HEAD
	(*sig_info[sig])(sig, &r);
=======
	(*sig_info[sig])(sig, si, &r);
>>>>>>> refs/remotes/origin/master

	errno = save_errno;
}

/*
 * These are the asynchronous signals.  SIGPROF is excluded because we want to
 * be able to profile all of UML, not just the non-critical sections.  If
 * profiling is not thread-safe, then that is not my problem.  We can disable
 * profiling when SMP is enabled in that case.
 */
#define SIGIO_BIT 0
#define SIGIO_MASK (1 << SIGIO_BIT)

#define SIGVTALRM_BIT 1
#define SIGVTALRM_MASK (1 << SIGVTALRM_BIT)

static int signals_enabled;
static unsigned int signals_pending;

<<<<<<< HEAD
<<<<<<< HEAD
void sig_handler(int sig, struct sigcontext *sc)
=======
void sig_handler(int sig, mcontext_t *mc)
>>>>>>> refs/remotes/origin/cm-10.0
=======
void sig_handler(int sig, struct siginfo *si, mcontext_t *mc)
>>>>>>> refs/remotes/origin/master
{
	int enabled;

	enabled = signals_enabled;
	if (!enabled && (sig == SIGIO)) {
		signals_pending |= SIGIO_MASK;
		return;
	}

	block_signals();

<<<<<<< HEAD
<<<<<<< HEAD
	sig_handler_common(sig, sc);
=======
	sig_handler_common(sig, mc);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	sig_handler_common(sig, si, mc);
>>>>>>> refs/remotes/origin/master

	set_signals(enabled);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void real_alarm_handler(struct sigcontext *sc)
{
	struct uml_pt_regs regs;

	if (sc != NULL)
		copy_sc(&regs, sc);
=======
=======
>>>>>>> refs/remotes/origin/master
static void real_alarm_handler(mcontext_t *mc)
{
	struct uml_pt_regs regs;

	if (mc != NULL)
		get_regs_from_mc(&regs, mc);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	regs.is_user = 0;
	unblock_signals();
	timer_handler(SIGVTALRM, &regs);
}

<<<<<<< HEAD
void alarm_handler(int sig, struct sigcontext *sc)
=======
void alarm_handler(int sig, mcontext_t *mc)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	regs.is_user = 0;
	unblock_signals();
	timer_handler(SIGVTALRM, NULL, &regs);
}

void alarm_handler(int sig, struct siginfo *unused_si, mcontext_t *mc)
>>>>>>> refs/remotes/origin/master
{
	int enabled;

	enabled = signals_enabled;
	if (!signals_enabled) {
		signals_pending |= SIGVTALRM_MASK;
		return;
	}

	block_signals();

<<<<<<< HEAD
<<<<<<< HEAD
	real_alarm_handler(sc);
=======
	real_alarm_handler(mc);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	real_alarm_handler(mc);
>>>>>>> refs/remotes/origin/master
	set_signals(enabled);
}

void timer_init(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	set_handler(SIGVTALRM, (__sighandler_t) alarm_handler,
		    SA_ONSTACK | SA_RESTART, SIGUSR1, SIGIO, SIGWINCH, -1);
=======
	set_handler(SIGVTALRM);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	set_handler(SIGVTALRM);
>>>>>>> refs/remotes/origin/master
}

void set_sigstack(void *sig_stack, int size)
{
	stack_t stack = ((stack_t) { .ss_flags	= 0,
				     .ss_sp	= (__ptr_t) sig_stack,
				     .ss_size 	= size - sizeof(void *) });

	if (sigaltstack(&stack, NULL) != 0)
		panic("enabling signal stack failed, errno = %d\n", errno);
}

<<<<<<< HEAD
<<<<<<< HEAD
static void (*handlers[_NSIG])(int sig, struct sigcontext *sc);

void handle_signal(int sig, struct sigcontext *sc)
{
=======
static void (*handlers[_NSIG])(int sig, mcontext_t *mc) = {
=======
static void (*handlers[_NSIG])(int sig, struct siginfo *si, mcontext_t *mc) = {
>>>>>>> refs/remotes/origin/master
	[SIGSEGV] = sig_handler,
	[SIGBUS] = sig_handler,
	[SIGILL] = sig_handler,
	[SIGFPE] = sig_handler,
	[SIGTRAP] = sig_handler,

	[SIGIO] = sig_handler,
	[SIGWINCH] = sig_handler,
	[SIGVTALRM] = alarm_handler
};


<<<<<<< HEAD
static void hard_handler(int sig, siginfo_t *info, void *p)
{
	struct ucontext *uc = p;
	mcontext_t *mc = &uc->uc_mcontext;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void hard_handler(int sig, siginfo_t *si, void *p)
{
	struct ucontext *uc = p;
	mcontext_t *mc = &uc->uc_mcontext;
>>>>>>> refs/remotes/origin/master
	unsigned long pending = 1UL << sig;

	do {
		int nested, bail;

		/*
		 * pending comes back with one bit set for each
		 * interrupt that arrived while setting up the stack,
		 * plus a bit for this interrupt, plus the zero bit is
		 * set if this is a nested interrupt.
		 * If bail is true, then we interrupted another
		 * handler setting up the stack.  In this case, we
		 * have to return, and the upper handler will deal
		 * with this interrupt.
		 */
		bail = to_irq_stack(&pending);
		if (bail)
			return;

		nested = pending & 1;
		pending &= ~1;

		while ((sig = ffs(pending)) != 0){
			sig--;
			pending &= ~(1 << sig);
<<<<<<< HEAD
<<<<<<< HEAD
			(*handlers[sig])(sig, sc);
=======
			(*handlers[sig])(sig, mc);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			(*handlers[sig])(sig, (struct siginfo *)si, mc);
>>>>>>> refs/remotes/origin/master
		}

		/*
		 * Again, pending comes back with a mask of signals
		 * that arrived while tearing down the stack.  If this
		 * is non-zero, we just go back, set up the stack
		 * again, and handle the new interrupts.
		 */
		if (!nested)
			pending = from_irq_stack(nested);
	} while (pending);
}

<<<<<<< HEAD
<<<<<<< HEAD
extern void hard_handler(int sig);

void set_handler(int sig, void (*handler)(int), int flags, ...)
{
	struct sigaction action;
	va_list ap;
	sigset_t sig_mask;
	int mask;

	handlers[sig] = (void (*)(int, struct sigcontext *)) handler;
	action.sa_handler = hard_handler;

	sigemptyset(&action.sa_mask);

	va_start(ap, flags);
	while ((mask = va_arg(ap, int)) != -1)
		sigaddset(&action.sa_mask, mask);
	va_end(ap);
=======
=======
>>>>>>> refs/remotes/origin/master
void set_handler(int sig)
{
	struct sigaction action;
	int flags = SA_SIGINFO | SA_ONSTACK;
	sigset_t sig_mask;

	action.sa_sigaction = hard_handler;

	/* block irq ones */
	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask, SIGVTALRM);
	sigaddset(&action.sa_mask, SIGIO);
	sigaddset(&action.sa_mask, SIGWINCH);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (sig == SIGSEGV)
		flags |= SA_NODEFER;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (sigismember(&action.sa_mask, sig))
		flags |= SA_RESTART; /* if it's an irq signal */

>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (sigismember(&action.sa_mask, sig))
		flags |= SA_RESTART; /* if it's an irq signal */

>>>>>>> refs/remotes/origin/master
	action.sa_flags = flags;
	action.sa_restorer = NULL;
	if (sigaction(sig, &action, NULL) < 0)
		panic("sigaction failed - errno = %d\n", errno);

	sigemptyset(&sig_mask);
	sigaddset(&sig_mask, sig);
	if (sigprocmask(SIG_UNBLOCK, &sig_mask, NULL) < 0)
		panic("sigprocmask failed - errno = %d\n", errno);
}

int change_sig(int signal, int on)
{
	sigset_t sigset;

	sigemptyset(&sigset);
	sigaddset(&sigset, signal);
	if (sigprocmask(on ? SIG_UNBLOCK : SIG_BLOCK, &sigset, NULL) < 0)
		return -errno;

	return 0;
}

void block_signals(void)
{
	signals_enabled = 0;
	/*
	 * This must return with signals disabled, so this barrier
	 * ensures that writes are flushed out before the return.
	 * This might matter if gcc figures out how to inline this and
	 * decides to shuffle this code into the caller.
	 */
	barrier();
}

void unblock_signals(void)
{
	int save_pending;

	if (signals_enabled == 1)
		return;

	/*
	 * We loop because the IRQ handler returns with interrupts off.  So,
	 * interrupts may have arrived and we need to re-enable them and
	 * recheck signals_pending.
	 */
	while (1) {
		/*
		 * Save and reset save_pending after enabling signals.  This
		 * way, signals_pending won't be changed while we're reading it.
		 */
		signals_enabled = 1;

		/*
		 * Setting signals_enabled and reading signals_pending must
		 * happen in this order.
		 */
		barrier();

		save_pending = signals_pending;
		if (save_pending == 0)
			return;

		signals_pending = 0;

		/*
		 * We have pending interrupts, so disable signals, as the
		 * handlers expect them off when they are called.  They will
		 * be enabled again above.
		 */

		signals_enabled = 0;

		/*
		 * Deal with SIGIO first because the alarm handler might
		 * schedule, leaving the pending SIGIO stranded until we come
		 * back here.
<<<<<<< HEAD
		 */
		if (save_pending & SIGIO_MASK)
			sig_handler_common(SIGIO, NULL);
=======
		 *
		 * SIGIO's handler doesn't use siginfo or mcontext,
		 * so they can be NULL.
		 */
		if (save_pending & SIGIO_MASK)
			sig_handler_common(SIGIO, NULL, NULL);
>>>>>>> refs/remotes/origin/master

		if (save_pending & SIGVTALRM_MASK)
			real_alarm_handler(NULL);
	}
}

int get_signals(void)
{
	return signals_enabled;
}

int set_signals(int enable)
{
	int ret;
	if (signals_enabled == enable)
		return enable;

	ret = signals_enabled;
	if (enable)
		unblock_signals();
	else block_signals();

	return ret;
}
<<<<<<< HEAD
=======

int os_is_signal_stack(void)
{
	stack_t ss;
	sigaltstack(NULL, &ss);

	return ss.ss_flags & SS_ONSTACK;
}
>>>>>>> refs/remotes/origin/master
