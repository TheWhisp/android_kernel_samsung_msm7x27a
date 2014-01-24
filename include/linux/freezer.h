/* Freezer declarations */

#ifndef FREEZER_H_INCLUDED
#define FREEZER_H_INCLUDED

<<<<<<< HEAD
#include <linux/sched.h>
#include <linux/wait.h>
<<<<<<< HEAD

#ifdef CONFIG_FREEZER
/*
 * Check if a process has been frozen
 */
static inline int frozen(struct task_struct *p)
=======
=======
#include <linux/debug_locks.h>
#include <linux/sched.h>
#include <linux/wait.h>
>>>>>>> refs/remotes/origin/master
#include <linux/atomic.h>

#ifdef CONFIG_FREEZER
extern atomic_t system_freezing_cnt;	/* nr of freezing conds in effect */
extern bool pm_freezing;		/* PM freezing in effect */
extern bool pm_nosig_freezing;		/* PM nosig freezing in effect */

/*
<<<<<<< HEAD
 * Check if a process has been frozen
 */
static inline bool frozen(struct task_struct *p)
>>>>>>> refs/remotes/origin/cm-10.0
{
	return p->flags & PF_FROZEN;
}

<<<<<<< HEAD
/*
 * Check if there is a request to freeze a process
 */
static inline int freezing(struct task_struct *p)
{
	return test_tsk_thread_flag(p, TIF_FREEZE);
}

/*
 * Request that a process be frozen
 */
static inline void set_freeze_flag(struct task_struct *p)
{
	set_tsk_thread_flag(p, TIF_FREEZE);
}

/*
 * Sometimes we may need to cancel the previous 'freeze' request
 */
static inline void clear_freeze_flag(struct task_struct *p)
{
	clear_tsk_thread_flag(p, TIF_FREEZE);
}

static inline bool should_send_signal(struct task_struct *p)
{
	return !(p->flags & PF_FREEZER_NOSIG);
}

/* Takes and releases task alloc lock using task_lock() */
extern int thaw_process(struct task_struct *p);

extern void refrigerator(void);
extern int freeze_processes(void);
extern void thaw_processes(void);

static inline int try_to_freeze(void)
{
	if (freezing(current)) {
		refrigerator();
		return 1;
	} else
		return 0;
}

extern bool freeze_task(struct task_struct *p, bool sig_only);
extern void cancel_freezing(struct task_struct *p);

#ifdef CONFIG_CGROUP_FREEZER
extern int cgroup_freezing_or_frozen(struct task_struct *task);
#else /* !CONFIG_CGROUP_FREEZER */
static inline int cgroup_freezing_or_frozen(struct task_struct *task)
{
	return 0;
=======
=======
 * Timeout for stopping processes
 */
extern unsigned int freeze_timeout_msecs;

/*
 * Check if a process has been frozen
 */
static inline bool frozen(struct task_struct *p)
{
	return p->flags & PF_FROZEN;
}

>>>>>>> refs/remotes/origin/master
extern bool freezing_slow_path(struct task_struct *p);

/*
 * Check if there is a request to freeze a process
 */
static inline bool freezing(struct task_struct *p)
{
	if (likely(!atomic_read(&system_freezing_cnt)))
		return false;
	return freezing_slow_path(p);
}

/* Takes and releases task alloc lock using task_lock() */
extern void __thaw_task(struct task_struct *t);

extern bool __refrigerator(bool check_kthr_stop);
extern int freeze_processes(void);
extern int freeze_kernel_threads(void);
extern void thaw_processes(void);
extern void thaw_kernel_threads(void);

<<<<<<< HEAD
static inline bool try_to_freeze(void)
{
/* This causes problems for ARM targets and is a known
 * problem upstream.
 *	might_sleep();
 */
=======
/*
 * DO NOT ADD ANY NEW CALLERS OF THIS FUNCTION
 * If try_to_freeze causes a lockdep warning it means the caller may deadlock
 */
static inline bool try_to_freeze_unsafe(void)
{
	might_sleep();
>>>>>>> refs/remotes/origin/master
	if (likely(!freezing(current)))
		return false;
	return __refrigerator(false);
}

<<<<<<< HEAD
=======
static inline bool try_to_freeze(void)
{
	if (!(current->flags & PF_NOFREEZE))
		debug_check_no_locks_held();
	return try_to_freeze_unsafe();
}

>>>>>>> refs/remotes/origin/master
extern bool freeze_task(struct task_struct *p);
extern bool set_freezable(void);

#ifdef CONFIG_CGROUP_FREEZER
extern bool cgroup_freezing(struct task_struct *task);
#else /* !CONFIG_CGROUP_FREEZER */
static inline bool cgroup_freezing(struct task_struct *task)
{
	return false;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}
#endif /* !CONFIG_CGROUP_FREEZER */

/*
 * The PF_FREEZER_SKIP flag should be set by a vfork parent right before it
 * calls wait_for_completion(&vfork) and reset right after it returns from this
 * function.  Next, the parent should call try_to_freeze() to freeze itself
 * appropriately in case the child has exited before the freezing of tasks is
 * complete.  However, we don't want kernel threads to be frozen in unexpected
 * places, so we allow them to block freeze_processes() instead or to set
<<<<<<< HEAD
<<<<<<< HEAD
 * PF_NOFREEZE if needed and PF_FREEZER_SKIP is only set for userland vfork
 * parents.  Fortunately, in the ____call_usermodehelper() case the parent won't
 * really block freeze_processes(), since ____call_usermodehelper() (the child)
 * does a little before exec/exit and it can't be frozen before waking up the
 * parent.
 */

/*
 * If the current task is a user space one, tell the freezer not to count it as
 * freezable.
 */
static inline void freezer_do_not_count(void)
{
	if (current->mm)
		current->flags |= PF_FREEZER_SKIP;
}

/*
 * If the current task is a user space one, tell the freezer to count it as
 * freezable again and try to freeze it.
 */
static inline void freezer_count(void)
{
	if (current->mm) {
		current->flags &= ~PF_FREEZER_SKIP;
		try_to_freeze();
	}
}

/*
 * Check if the task should be counted as freezable by the freezer
 */
static inline int freezer_should_skip(struct task_struct *p)
{
	return !!(p->flags & PF_FREEZER_SKIP);
}

/*
 * Tell the freezer that the current task should be frozen by it
 */
static inline void set_freezable(void)
{
	current->flags &= ~PF_NOFREEZE;
}

/*
 * Tell the freezer that the current task should be frozen by it and that it
 * should send a fake signal to the task to freeze it.
 */
static inline void set_freezable_with_signal(void)
{
	current->flags &= ~(PF_NOFREEZE | PF_FREEZER_NOSIG);
}

/*
 * Freezer-friendly wrappers around wait_event_interruptible() and
 * wait_event_interruptible_timeout(), originally defined in <linux/wait.h>
 */

#define wait_event_freezable(wq, condition)				\
({									\
	int __retval;							\
	do {								\
		__retval = wait_event_interruptible(wq, 		\
				(condition) || freezing(current));	\
		if (__retval && !freezing(current))			\
			break;						\
		else if (!(condition))					\
			__retval = -ERESTARTSYS;			\
	} while (try_to_freeze());					\
	__retval;							\
})


#define wait_event_freezable_timeout(wq, condition, timeout)		\
({									\
	long __retval = timeout;					\
	do {								\
		__retval = wait_event_interruptible_timeout(wq,		\
				(condition) || freezing(current),	\
				__retval); 				\
	} while (try_to_freeze());					\
	__retval;							\
})
#else /* !CONFIG_FREEZER */
static inline int frozen(struct task_struct *p) { return 0; }
static inline int freezing(struct task_struct *p) { return 0; }
static inline void set_freeze_flag(struct task_struct *p) {}
static inline void clear_freeze_flag(struct task_struct *p) {}
static inline int thaw_process(struct task_struct *p) { return 1; }

static inline void refrigerator(void) {}
static inline int freeze_processes(void) { BUG(); return 0; }
static inline void thaw_processes(void) {}

static inline int try_to_freeze(void) { return 0; }
=======
=======
>>>>>>> refs/remotes/origin/master
 * PF_NOFREEZE if needed. Fortunately, in the ____call_usermodehelper() case the
 * parent won't really block freeze_processes(), since ____call_usermodehelper()
 * (the child) does a little before exec/exit and it can't be frozen before
 * waking up the parent.
 */


/**
 * freezer_do_not_count - tell freezer to ignore %current
 *
 * Tell freezers to ignore the current task when determining whether the
 * target frozen state is reached.  IOW, the current task will be
 * considered frozen enough by freezers.
 *
 * The caller shouldn't do anything which isn't allowed for a frozen task
 * until freezer_cont() is called.  Usually, freezer[_do_not]_count() pair
 * wrap a scheduling operation and nothing much else.
 */
static inline void freezer_do_not_count(void)
{
	current->flags |= PF_FREEZER_SKIP;
}

/**
 * freezer_count - tell freezer to stop ignoring %current
 *
 * Undo freezer_do_not_count().  It tells freezers that %current should be
 * considered again and tries to freeze if freezing condition is already in
 * effect.
 */
static inline void freezer_count(void)
{
	current->flags &= ~PF_FREEZER_SKIP;
	/*
	 * If freezing is in progress, the following paired with smp_mb()
	 * in freezer_should_skip() ensures that either we see %true
	 * freezing() or freezer_should_skip() sees !PF_FREEZER_SKIP.
	 */
	smp_mb();
	try_to_freeze();
}

<<<<<<< HEAD
=======
/* DO NOT ADD ANY NEW CALLERS OF THIS FUNCTION */
static inline void freezer_count_unsafe(void)
{
	current->flags &= ~PF_FREEZER_SKIP;
	smp_mb();
	try_to_freeze_unsafe();
}

>>>>>>> refs/remotes/origin/master
/**
 * freezer_should_skip - whether to skip a task when determining frozen
 *			 state is reached
 * @p: task in quesion
 *
 * This function is used by freezers after establishing %true freezing() to
 * test whether a task should be skipped when determining the target frozen
 * state is reached.  IOW, if this function returns %true, @p is considered
 * frozen enough.
 */
static inline bool freezer_should_skip(struct task_struct *p)
{
	/*
	 * The following smp_mb() paired with the one in freezer_count()
	 * ensures that either freezer_count() sees %true freezing() or we
	 * see cleared %PF_FREEZER_SKIP and return %false.  This makes it
	 * impossible for a task to slip frozen state testing after
	 * clearing %PF_FREEZER_SKIP.
	 */
	smp_mb();
	return p->flags & PF_FREEZER_SKIP;
}

/*
<<<<<<< HEAD
 * These macros are intended to be used whenever you want allow a task that's
 * sleeping in TASK_UNINTERRUPTIBLE or TASK_KILLABLE state to be frozen. Note
 * that neither return any clear indication of whether a freeze event happened
 * while in this function.
 */

/* Like schedule(), but should not block the freezer. */
#define freezable_schedule()						\
({									\
	freezer_do_not_count();						\
	schedule();							\
	freezer_count();						\
})

/* Like schedule_timeout_killable(), but should not block the freezer. */
#define freezable_schedule_timeout_killable(timeout)			\
({									\
	long __retval;							\
	freezer_do_not_count();						\
	__retval = schedule_timeout_killable(timeout);			\
	freezer_count();						\
	__retval;							\
})
=======
 * These functions are intended to be used whenever you want allow a sleeping
 * task to be frozen. Note that neither return any clear indication of
 * whether a freeze event happened while in this function.
 */

/* Like schedule(), but should not block the freezer. */
static inline void freezable_schedule(void)
{
	freezer_do_not_count();
	schedule();
	freezer_count();
}

/* DO NOT ADD ANY NEW CALLERS OF THIS FUNCTION */
static inline void freezable_schedule_unsafe(void)
{
	freezer_do_not_count();
	schedule();
	freezer_count_unsafe();
}

/*
 * Like freezable_schedule_timeout(), but should not block the freezer.  Do not
 * call this with locks held.
 */
static inline long freezable_schedule_timeout(long timeout)
{
	long __retval;
	freezer_do_not_count();
	__retval = schedule_timeout(timeout);
	freezer_count();
	return __retval;
}

/*
 * Like schedule_timeout_interruptible(), but should not block the freezer.  Do not
 * call this with locks held.
 */
static inline long freezable_schedule_timeout_interruptible(long timeout)
{
	long __retval;
	freezer_do_not_count();
	__retval = schedule_timeout_interruptible(timeout);
	freezer_count();
	return __retval;
}

/* Like schedule_timeout_killable(), but should not block the freezer. */
static inline long freezable_schedule_timeout_killable(long timeout)
{
	long __retval;
	freezer_do_not_count();
	__retval = schedule_timeout_killable(timeout);
	freezer_count();
	return __retval;
}

/* DO NOT ADD ANY NEW CALLERS OF THIS FUNCTION */
static inline long freezable_schedule_timeout_killable_unsafe(long timeout)
{
	long __retval;
	freezer_do_not_count();
	__retval = schedule_timeout_killable(timeout);
	freezer_count_unsafe();
	return __retval;
}

/*
 * Like schedule_hrtimeout_range(), but should not block the freezer.  Do not
 * call this with locks held.
 */
static inline int freezable_schedule_hrtimeout_range(ktime_t *expires,
		unsigned long delta, const enum hrtimer_mode mode)
{
	int __retval;
	freezer_do_not_count();
	__retval = schedule_hrtimeout_range(expires, delta, mode);
	freezer_count();
	return __retval;
}
>>>>>>> refs/remotes/origin/master

/*
 * Freezer-friendly wrappers around wait_event_interruptible(),
 * wait_event_killable() and wait_event_interruptible_timeout(), originally
 * defined in <linux/wait.h>
 */

#define wait_event_freezekillable(wq, condition)			\
({									\
	int __retval;							\
	freezer_do_not_count();						\
	__retval = wait_event_killable(wq, (condition));		\
	freezer_count();						\
	__retval;							\
})

<<<<<<< HEAD
#define wait_event_freezable(wq, condition)				\
({									\
	int __retval;							\
	for (;;) {							\
		__retval = wait_event_interruptible(wq, 		\
				(condition) || freezing(current));	\
		if (__retval || (condition))				\
			break;						\
		try_to_freeze();					\
	}								\
=======
/* DO NOT ADD ANY NEW CALLERS OF THIS FUNCTION */
#define wait_event_freezekillable_unsafe(wq, condition)			\
({									\
	int __retval;							\
	freezer_do_not_count();						\
	__retval = wait_event_killable(wq, (condition));		\
	freezer_count_unsafe();						\
	__retval;							\
})

#define wait_event_freezable(wq, condition)				\
({									\
	int __retval;							\
	freezer_do_not_count();						\
	__retval = wait_event_interruptible(wq, (condition));		\
	freezer_count();						\
>>>>>>> refs/remotes/origin/master
	__retval;							\
})

#define wait_event_freezable_timeout(wq, condition, timeout)		\
({									\
	long __retval = timeout;					\
<<<<<<< HEAD
	for (;;) {							\
		__retval = wait_event_interruptible_timeout(wq,		\
				(condition) || freezing(current),	\
				__retval); 				\
		if (__retval <= 0 || (condition))			\
			break;						\
		try_to_freeze();					\
	}								\
	__retval;							\
})

=======
	freezer_do_not_count();						\
	__retval = wait_event_interruptible_timeout(wq,	(condition),	\
				__retval);				\
	freezer_count();						\
	__retval;							\
})

#define wait_event_freezable_exclusive(wq, condition)			\
({									\
	int __retval;							\
	freezer_do_not_count();						\
	__retval = wait_event_interruptible_exclusive(wq, condition);	\
	freezer_count();						\
	__retval;							\
})


>>>>>>> refs/remotes/origin/master
#else /* !CONFIG_FREEZER */
static inline bool frozen(struct task_struct *p) { return false; }
static inline bool freezing(struct task_struct *p) { return false; }
static inline void __thaw_task(struct task_struct *t) {}

static inline bool __refrigerator(bool check_kthr_stop) { return false; }
static inline int freeze_processes(void) { return -ENOSYS; }
static inline int freeze_kernel_threads(void) { return -ENOSYS; }
static inline void thaw_processes(void) {}
static inline void thaw_kernel_threads(void) {}

<<<<<<< HEAD
static inline bool try_to_freeze(void) { return false; }
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline bool try_to_freeze_nowarn(void) { return false; }
static inline bool try_to_freeze(void) { return false; }
>>>>>>> refs/remotes/origin/master

static inline void freezer_do_not_count(void) {}
static inline void freezer_count(void) {}
static inline int freezer_should_skip(struct task_struct *p) { return 0; }
static inline void set_freezable(void) {}
<<<<<<< HEAD
<<<<<<< HEAD
static inline void set_freezable_with_signal(void) {}
=======

#define freezable_schedule()  schedule()

#define freezable_schedule_timeout_killable(timeout)			\
	schedule_timeout_killable(timeout)
>>>>>>> refs/remotes/origin/cm-10.0
=======

#define freezable_schedule()  schedule()

#define freezable_schedule_unsafe()  schedule()

#define freezable_schedule_timeout(timeout)  schedule_timeout(timeout)

#define freezable_schedule_timeout_interruptible(timeout)		\
	schedule_timeout_interruptible(timeout)

#define freezable_schedule_timeout_killable(timeout)			\
	schedule_timeout_killable(timeout)

#define freezable_schedule_timeout_killable_unsafe(timeout)		\
	schedule_timeout_killable(timeout)

#define freezable_schedule_hrtimeout_range(expires, delta, mode)	\
	schedule_hrtimeout_range(expires, delta, mode)
>>>>>>> refs/remotes/origin/master

#define wait_event_freezable(wq, condition)				\
		wait_event_interruptible(wq, condition)

#define wait_event_freezable_timeout(wq, condition, timeout)		\
		wait_event_interruptible_timeout(wq, condition, timeout)

<<<<<<< HEAD
<<<<<<< HEAD
=======
#define wait_event_freezekillable(wq, condition)		\
		wait_event_killable(wq, condition)

>>>>>>> refs/remotes/origin/cm-10.0
=======
#define wait_event_freezable_exclusive(wq, condition)			\
		wait_event_interruptible_exclusive(wq, condition)

#define wait_event_freezekillable(wq, condition)		\
		wait_event_killable(wq, condition)

#define wait_event_freezekillable_unsafe(wq, condition)			\
		wait_event_killable(wq, condition)

>>>>>>> refs/remotes/origin/master
#endif /* !CONFIG_FREEZER */

#endif	/* FREEZER_H_INCLUDED */
