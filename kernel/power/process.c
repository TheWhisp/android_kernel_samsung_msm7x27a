/*
 * drivers/power/process.c - Functions for starting/stopping processes on 
 *                           suspend transitions.
 *
 * Originally from swsusp.
 */


#undef DEBUG

#include <linux/interrupt.h>
#include <linux/oom.h>
#include <linux/suspend.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/freezer.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
<<<<<<< HEAD
=======
#include <linux/kmod.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/wakelock.h>
#include "power.h"

/* 
 * Timeout for stopping processes
 */
#define TIMEOUT	(20 * HZ)

<<<<<<< HEAD
static inline int freezable(struct task_struct * p)
{
	if ((p == current) ||
	    (p->flags & PF_NOFREEZE) ||
	    (p->exit_state != 0))
		return 0;
	return 1;
}

static int try_to_freeze_tasks(bool sig_only)
=======
static int try_to_freeze_tasks(bool user_only)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct task_struct *g, *p;
	unsigned long end_time;
	unsigned int todo;
	bool wq_busy = false;
	struct timeval start, end;
	u64 elapsed_csecs64;
	unsigned int elapsed_csecs;
	bool wakeup = false;

	do_gettimeofday(&start);

	end_time = jiffies + TIMEOUT;

<<<<<<< HEAD
	if (!sig_only)
=======
	if (!user_only)
>>>>>>> refs/remotes/origin/cm-10.0
		freeze_workqueues_begin();

	while (true) {
		todo = 0;
		read_lock(&tasklist_lock);
		do_each_thread(g, p) {
<<<<<<< HEAD
			if (frozen(p) || !freezable(p))
				continue;

			if (!freeze_task(p, sig_only))
=======
			if (p == current || !freeze_task(p))
>>>>>>> refs/remotes/origin/cm-10.0
				continue;

			/*
			 * Now that we've done set_freeze_flag, don't
			 * perturb a task in TASK_STOPPED or TASK_TRACED.
			 * It is "frozen enough".  If the task does wake
			 * up, it will immediately call try_to_freeze.
			 *
<<<<<<< HEAD
			 * Because freeze_task() goes through p's
			 * scheduler lock after setting TIF_FREEZE, it's
			 * guaranteed that either we see TASK_RUNNING or
			 * try_to_stop() after schedule() in ptrace/signal
			 * stop sees TIF_FREEZE.
=======
			 * Because freeze_task() goes through p's scheduler lock, it's
			 * guaranteed that TASK_STOPPED/TRACED -> TASK_RUNNING
			 * transition can't race with task state testing here.
>>>>>>> refs/remotes/origin/cm-10.0
			 */
			if (!task_is_stopped_or_traced(p) &&
			    !freezer_should_skip(p))
				todo++;
		} while_each_thread(g, p);
		read_unlock(&tasklist_lock);

<<<<<<< HEAD
		if (!sig_only) {
=======
		if (!user_only) {
>>>>>>> refs/remotes/origin/cm-10.0
			wq_busy = freeze_workqueues_busy();
			todo += wq_busy;
		}

		if (todo && has_wake_lock(WAKE_LOCK_SUSPEND)) {
			wakeup = 1;
			break;
		}
		if (!todo || time_after(jiffies, end_time))
			break;

		if (pm_wakeup_pending()) {
			wakeup = true;
			break;
		}

		/*
		 * We need to retry, but first give the freezing tasks some
		 * time to enter the regrigerator.
		 */
		msleep(10);
	}

	do_gettimeofday(&end);
	elapsed_csecs64 = timeval_to_ns(&end) - timeval_to_ns(&start);
	do_div(elapsed_csecs64, NSEC_PER_SEC / 100);
	elapsed_csecs = elapsed_csecs64;

	if (todo) {
		/* This does not unfreeze processes that are already frozen
		 * (we have slightly ugly calling convention in that respect,
		 * and caller must call thaw_processes() if something fails),
		 * but it cleans up leftover PF_FREEZE requests.
		 */
		if(wakeup) {
			printk("\n");
			printk(KERN_ERR "Freezing of %s aborted\n",
<<<<<<< HEAD
					sig_only ? "user space " : "tasks ");
		}
		else {
			printk("\n");
			printk(KERN_ERR "Freezing of tasks failed after %d.%02d seconds "
			       "(%d tasks refusing to freeze, wq_busy=%d):\n",
			       elapsed_csecs / 100, elapsed_csecs % 100,
			       todo - wq_busy, wq_busy);
		}
		thaw_workqueues();

		read_lock(&tasklist_lock);
		do_each_thread(g, p) {
			task_lock(p);
			if (freezing(p) && !freezer_should_skip(p) &&
				elapsed_csecs > 100)
				sched_show_task(p);
			cancel_freezing(p);
			task_unlock(p);
		} while_each_thread(g, p);
		read_unlock(&tasklist_lock);
=======
					user_only ? "user space " : "tasks ");
		}
		else {
			printk("\n");
			printk(KERN_ERR "Freezing of tasks %s after %d.%02d seconds "
			       "(%d tasks refusing to freeze, wq_busy=%d):\n",
			       wakeup ? "aborted" : "failed",
			       elapsed_csecs / 100, elapsed_csecs % 100,
			       todo - wq_busy, wq_busy);
		}

		if (!wakeup) {
			read_lock(&tasklist_lock);
			do_each_thread(g, p) {
				if (p != current && !freezer_should_skip(p)
				    && freezing(p) && !frozen(p) &&
				    elapsed_csecs > 100)
					sched_show_task(p);
			} while_each_thread(g, p);
			read_unlock(&tasklist_lock);
		}
>>>>>>> refs/remotes/origin/cm-10.0
	} else {
		printk("(elapsed %d.%02d seconds) ", elapsed_csecs / 100,
			elapsed_csecs % 100);
	}

	return todo ? -EBUSY : 0;
}

/**
<<<<<<< HEAD
 *	freeze_processes - tell processes to enter the refrigerator
=======
 * freeze_processes - Signal user space processes to enter the refrigerator.
 *
 * On success, returns 0.  On failure, -errno and system is fully thawed.
>>>>>>> refs/remotes/origin/cm-10.0
 */
int freeze_processes(void)
{
	int error;

<<<<<<< HEAD
	printk("Freezing user space processes ... ");
	error = try_to_freeze_tasks(true);
	if (error)
		goto Exit;
	printk("done.\n");

	error = suspend_sys_sync_wait();
	if (error)
		goto Exit;

	printk("Freezing remaining freezable tasks ... ");
	error = try_to_freeze_tasks(false);
	if (error)
		goto Exit;
	printk("done.");

	oom_killer_disable();
 Exit:
	BUG_ON(in_atomic());
	printk("\n");

	return error;
}

static void thaw_tasks(bool nosig_only)
{
	struct task_struct *g, *p;

	read_lock(&tasklist_lock);
	do_each_thread(g, p) {
		if (!freezable(p))
			continue;

		if (nosig_only && should_send_signal(p))
			continue;

		if (cgroup_freezing_or_frozen(p))
			continue;

		thaw_process(p);
	} while_each_thread(g, p);
	read_unlock(&tasklist_lock);
}

void thaw_processes(void)
{
	oom_killer_enable();

	printk("Restarting tasks ... ");
	thaw_workqueues();
	thaw_tasks(true);
	thaw_tasks(false);
	schedule();
	printk("done.\n");
}

=======
	error = __usermodehelper_disable(UMH_FREEZING);
	if (error)
		return error;

	if (!pm_freezing)
		atomic_inc(&system_freezing_cnt);

	printk("Freezing user space processes ... ");
	pm_freezing = true;
	error = try_to_freeze_tasks(true);
	if (!error) {
		printk("done.");
		__usermodehelper_set_disable_depth(UMH_DISABLED);
		oom_killer_disable();
	}
	printk("\n");
	BUG_ON(in_atomic());

	if (error)
		thaw_processes();
	return error;
}

/**
 * freeze_kernel_threads - Make freezable kernel threads go to the refrigerator.
 *
 * On success, returns 0.  On failure, -errno and only the kernel threads are
 * thawed, so as to give a chance to the caller to do additional cleanups
 * (if any) before thawing the userspace tasks. So, it is the responsibility
 * of the caller to thaw the userspace tasks, when the time is right.
 */
int freeze_kernel_threads(void)
{
	int error;

	error = suspend_sys_sync_wait();
	if (error)
		return error;

	printk("Freezing remaining freezable tasks ... ");
	pm_nosig_freezing = true;
	error = try_to_freeze_tasks(false);
	if (!error)
		printk("done.");

	printk("\n");
	BUG_ON(in_atomic());

	if (error)
		thaw_kernel_threads();
	return error;
}

void thaw_processes(void)
{
	struct task_struct *g, *p;

	if (pm_freezing)
		atomic_dec(&system_freezing_cnt);
	pm_freezing = false;
	pm_nosig_freezing = false;

	oom_killer_enable();

	printk("Restarting tasks ... ");

	thaw_workqueues();

	read_lock(&tasklist_lock);
	do_each_thread(g, p) {
		__thaw_task(p);
	} while_each_thread(g, p);
	read_unlock(&tasklist_lock);

	usermodehelper_enable();

	schedule();
	printk("done.\n");
}

void thaw_kernel_threads(void)
{
	struct task_struct *g, *p;

	pm_nosig_freezing = false;
	printk("Restarting kernel threads ... ");

	thaw_workqueues();

	read_lock(&tasklist_lock);
	do_each_thread(g, p) {
		if (p->flags & (PF_KTHREAD | PF_WQ_WORKER))
			__thaw_task(p);
	} while_each_thread(g, p);
	read_unlock(&tasklist_lock);

	schedule();
	printk("done.\n");
}
>>>>>>> refs/remotes/origin/cm-10.0
