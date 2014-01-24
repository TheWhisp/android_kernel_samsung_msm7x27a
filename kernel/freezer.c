/*
 * kernel/freezer.c - Function to freeze a process
 *
 * Originally from kernel/power/process.c
 */

#include <linux/interrupt.h>
#include <linux/suspend.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/freezer.h>

/*
 * freezing is complete, mark current process as frozen
 */
static inline void frozen_process(void)
{
	if (!unlikely(current->flags & PF_NOFREEZE)) {
		current->flags |= PF_FROZEN;
		smp_wmb();
	}
	clear_freeze_flag(current);
}

/* Refrigerator is place where frozen processes are stored :-). */
void refrigerator(void)
{
	/* Hmm, should we be allowed to suspend when there are realtime
	   processes around? */
	long save;

	task_lock(current);
	if (freezing(current)) {
		frozen_process();
		task_unlock(current);
	} else {
		task_unlock(current);
		return;
	}
	save = current->state;
	pr_debug("%s entered refrigerator\n", current->comm);

	spin_lock_irq(&current->sighand->siglock);
	recalc_sigpending(); /* We sent fake signal, clean it up */
	spin_unlock_irq(&current->sighand->siglock);

	/* prevent accounting of that task to load */
	current->flags |= PF_FREEZING;

	for (;;) {
		set_current_state(TASK_UNINTERRUPTIBLE);
		if (!frozen(current))
			break;
		schedule();
	}

	/* Remove the accounting blocker */
	current->flags &= ~PF_FREEZING;

	pr_debug("%s left refrigerator\n", current->comm);
	__set_current_state(save);
}
EXPORT_SYMBOL(refrigerator);
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/export.h>
#include <linux/syscalls.h>
#include <linux/freezer.h>
#include <linux/kthread.h>

/* total number of freezing conditions in effect */
atomic_t system_freezing_cnt = ATOMIC_INIT(0);
EXPORT_SYMBOL(system_freezing_cnt);

/* indicate whether PM freezing is in effect, protected by pm_mutex */
bool pm_freezing;
bool pm_nosig_freezing;

<<<<<<< HEAD
=======
/*
 * Temporary export for the deadlock workaround in ata_scsi_hotplug().
 * Remove once the hack becomes unnecessary.
 */
EXPORT_SYMBOL_GPL(pm_freezing);

>>>>>>> refs/remotes/origin/master
/* protects freezing and frozen transitions */
static DEFINE_SPINLOCK(freezer_lock);

/**
 * freezing_slow_path - slow path for testing whether a task needs to be frozen
 * @p: task to be tested
 *
 * This function is called by freezing() if system_freezing_cnt isn't zero
 * and tests whether @p needs to enter and stay in frozen state.  Can be
 * called under any context.  The freezers are responsible for ensuring the
 * target tasks see the updated state.
 */
bool freezing_slow_path(struct task_struct *p)
{
<<<<<<< HEAD
	if (p->flags & PF_NOFREEZE)
=======
	if (p->flags & (PF_NOFREEZE | PF_SUSPEND_TASK))
>>>>>>> refs/remotes/origin/master
		return false;

	if (pm_nosig_freezing || cgroup_freezing(p))
		return true;

	if (pm_freezing && !(p->flags & PF_KTHREAD))
		return true;

	return false;
}
EXPORT_SYMBOL(freezing_slow_path);

/* Refrigerator is place where frozen processes are stored :-). */
bool __refrigerator(bool check_kthr_stop)
{
	/* Hmm, should we be allowed to suspend when there are realtime
	   processes around? */
	bool was_frozen = false;
	long save = current->state;

	pr_debug("%s entered refrigerator\n", current->comm);

	for (;;) {
		set_current_state(TASK_UNINTERRUPTIBLE);

		spin_lock_irq(&freezer_lock);
		current->flags |= PF_FROZEN;
		if (!freezing(current) ||
		    (check_kthr_stop && kthread_should_stop()))
			current->flags &= ~PF_FROZEN;
		spin_unlock_irq(&freezer_lock);

		if (!(current->flags & PF_FROZEN))
			break;
		was_frozen = true;
		schedule();
	}

	pr_debug("%s left refrigerator\n", current->comm);

	/*
	 * Restore saved task state before returning.  The mb'd version
	 * needs to be used; otherwise, it might silently break
	 * synchronization which depends on ordered task state change.
	 */
	set_current_state(save);

	return was_frozen;
}
EXPORT_SYMBOL(__refrigerator);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

static void fake_signal_wake_up(struct task_struct *p)
{
	unsigned long flags;

<<<<<<< HEAD
<<<<<<< HEAD
	spin_lock_irqsave(&p->sighand->siglock, flags);
	signal_wake_up(p, 0);
	spin_unlock_irqrestore(&p->sighand->siglock, flags);
}

/**
 *	freeze_task - send a freeze request to given task
 *	@p: task to send the request to
 *	@sig_only: if set, the request will only be sent if the task has the
 *		PF_FREEZER_NOSIG flag unset
 *	Return value: 'false', if @sig_only is set and the task has
 *		PF_FREEZER_NOSIG set or the task is frozen, 'true', otherwise
 *
 *	The freeze request is sent by setting the tasks's TIF_FREEZE flag and
 *	either sending a fake signal to it or waking it up, depending on whether
 *	or not it has PF_FREEZER_NOSIG set.  If @sig_only is set and the task
 *	has PF_FREEZER_NOSIG set (ie. it is a typical kernel thread), its
 *	TIF_FREEZE flag will not be set.
 */
bool freeze_task(struct task_struct *p, bool sig_only)
{
	/*
	 * We first check if the task is freezing and next if it has already
	 * been frozen to avoid the race with frozen_process() which first marks
	 * the task as frozen and next clears its TIF_FREEZE.
	 */
	if (!freezing(p)) {
		smp_rmb();
		if (frozen(p))
			return false;

		if (!sig_only || should_send_signal(p))
			set_freeze_flag(p);
		else
			return false;
	}

	if (should_send_signal(p)) {
=======
=======
>>>>>>> refs/remotes/origin/master
	if (lock_task_sighand(p, &flags)) {
		signal_wake_up(p, 0);
		unlock_task_sighand(p, &flags);
	}
}

/**
 * freeze_task - send a freeze request to given task
 * @p: task to send the request to
 *
 * If @p is freezing, the freeze request is sent either by sending a fake
 * signal (if it's not a kernel thread) or waking it up (if it's a kernel
 * thread).
 *
 * RETURNS:
 * %false, if @p is not freezing or already frozen; %true, otherwise
 */
bool freeze_task(struct task_struct *p)
{
	unsigned long flags;

<<<<<<< HEAD
=======
	/*
	 * This check can race with freezer_do_not_count, but worst case that
	 * will result in an extra wakeup being sent to the task.  It does not
	 * race with freezer_count(), the barriers in freezer_count() and
	 * freezer_should_skip() ensure that either freezer_count() sees
	 * freezing == true in try_to_freeze() and freezes, or
	 * freezer_should_skip() sees !PF_FREEZE_SKIP and freezes the task
	 * normally.
	 */
	if (freezer_should_skip(p))
		return false;

>>>>>>> refs/remotes/origin/master
	spin_lock_irqsave(&freezer_lock, flags);
	if (!freezing(p) || frozen(p)) {
		spin_unlock_irqrestore(&freezer_lock, flags);
		return false;
	}

<<<<<<< HEAD
	if (!(p->flags & PF_KTHREAD)) {
>>>>>>> refs/remotes/origin/cm-10.0
		fake_signal_wake_up(p);
		/*
		 * fake_signal_wake_up() goes through p's scheduler
		 * lock and guarantees that TASK_STOPPED/TRACED ->
		 * TASK_RUNNING transition can't race with task state
		 * testing in try_to_freeze_tasks().
		 */
<<<<<<< HEAD
	} else if (sig_only) {
		return false;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	} else {
		wake_up_state(p, TASK_INTERRUPTIBLE);
	}

<<<<<<< HEAD
	return true;
}

void cancel_freezing(struct task_struct *p)
{
	unsigned long flags;

	if (freezing(p)) {
		pr_debug("  clean up: %s\n", p->comm);
		clear_freeze_flag(p);
		spin_lock_irqsave(&p->sighand->siglock, flags);
		recalc_sigpending_and_wake(p);
		spin_unlock_irqrestore(&p->sighand->siglock, flags);
	}
}

static int __thaw_process(struct task_struct *p)
{
	if (frozen(p)) {
		p->flags &= ~PF_FROZEN;
		return 1;
	}
	clear_freeze_flag(p);
	return 0;
}

/*
 * Wake up a frozen process
 *
 * task_lock() is needed to prevent the race with refrigerator() which may
 * occur if the freezing of tasks fails.  Namely, without the lock, if the
 * freezing of tasks failed, thaw_tasks() might have run before a task in
 * refrigerator() could call frozen_process(), in which case the task would be
 * frozen and no one would thaw it.
 */
int thaw_process(struct task_struct *p)
{
	task_lock(p);
	if (__thaw_process(p) == 1) {
		task_unlock(p);
		wake_up_process(p);
		return 1;
	}
	task_unlock(p);
	return 0;
}
EXPORT_SYMBOL(thaw_process);
=======
=======
	if (!(p->flags & PF_KTHREAD))
		fake_signal_wake_up(p);
	else
		wake_up_state(p, TASK_INTERRUPTIBLE);

>>>>>>> refs/remotes/origin/master
	spin_unlock_irqrestore(&freezer_lock, flags);
	return true;
}

void __thaw_task(struct task_struct *p)
{
	unsigned long flags;

	/*
	 * Clear freezing and kick @p if FROZEN.  Clearing is guaranteed to
	 * be visible to @p as waking up implies wmb.  Waking up inside
	 * freezer_lock also prevents wakeups from leaking outside
	 * refrigerator.
	 */
	spin_lock_irqsave(&freezer_lock, flags);
	if (frozen(p))
		wake_up_process(p);
	spin_unlock_irqrestore(&freezer_lock, flags);
}

/**
 * set_freezable - make %current freezable
 *
 * Mark %current freezable and enter refrigerator if necessary.
 */
bool set_freezable(void)
{
	might_sleep();

	/*
	 * Modify flags while holding freezer_lock.  This ensures the
	 * freezer notices that we aren't frozen yet or the freezing
	 * condition is visible to try_to_freeze() below.
	 */
	spin_lock_irq(&freezer_lock);
	current->flags &= ~PF_NOFREEZE;
	spin_unlock_irq(&freezer_lock);

	return try_to_freeze();
}
EXPORT_SYMBOL(set_freezable);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
