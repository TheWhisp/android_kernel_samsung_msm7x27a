#ifndef __LINUX_PREEMPT_H
#define __LINUX_PREEMPT_H

/*
 * include/linux/preempt.h - macros for accessing and manipulating
 * preempt_count (used for kernel preemption, interrupt count, etc.)
 */

<<<<<<< HEAD
#include <linux/thread_info.h>
#include <linux/linkage.h>
#include <linux/list.h>

#if defined(CONFIG_DEBUG_PREEMPT) || defined(CONFIG_PREEMPT_TRACER)
  extern void add_preempt_count(int val);
  extern void sub_preempt_count(int val);
#else
# define add_preempt_count(val)	do { preempt_count() += (val); } while (0)
# define sub_preempt_count(val)	do { preempt_count() -= (val); } while (0)
#endif

#define inc_preempt_count() add_preempt_count(1)
#define dec_preempt_count() sub_preempt_count(1)

#define preempt_count()	(current_thread_info()->preempt_count)

#ifdef CONFIG_PREEMPT

asmlinkage void preempt_schedule(void);

<<<<<<< HEAD
=======
#define preempt_check_resched() \
do { \
	if (unlikely(test_thread_flag(TIF_NEED_RESCHED))) \
		preempt_schedule(); \
} while (0)

#else /* !CONFIG_PREEMPT */

#define preempt_check_resched()		do { } while (0)

#endif /* CONFIG_PREEMPT */


#ifdef CONFIG_PREEMPT_COUNT

>>>>>>> refs/remotes/origin/cm-10.0
#define preempt_disable() \
do { \
	inc_preempt_count(); \
	barrier(); \
} while (0)

<<<<<<< HEAD
#define preempt_enable_no_resched() \
=======
#define sched_preempt_enable_no_resched() \
>>>>>>> refs/remotes/origin/cm-10.0
do { \
	barrier(); \
	dec_preempt_count(); \
} while (0)

<<<<<<< HEAD
#define preempt_check_resched() \
do { \
	if (unlikely(test_thread_flag(TIF_NEED_RESCHED))) \
		preempt_schedule(); \
} while (0)
=======
#define preempt_enable_no_resched()	sched_preempt_enable_no_resched()
>>>>>>> refs/remotes/origin/cm-10.0

#define preempt_enable() \
do { \
	preempt_enable_no_resched(); \
	barrier(); \
	preempt_check_resched(); \
} while (0)

/* For debugging and tracer internals only! */
#define add_preempt_count_notrace(val)			\
	do { preempt_count() += (val); } while (0)
#define sub_preempt_count_notrace(val)			\
	do { preempt_count() -= (val); } while (0)
#define inc_preempt_count_notrace() add_preempt_count_notrace(1)
#define dec_preempt_count_notrace() sub_preempt_count_notrace(1)

#define preempt_disable_notrace() \
do { \
	inc_preempt_count_notrace(); \
=======
#include <linux/linkage.h>
#include <linux/list.h>

/*
 * We use the MSB mostly because its available; see <linux/preempt_mask.h> for
 * the other bits -- can't include that header due to inclusion hell.
 */
#define PREEMPT_NEED_RESCHED	0x80000000

#include <asm/preempt.h>

#if defined(CONFIG_DEBUG_PREEMPT) || defined(CONFIG_PREEMPT_TRACER)
extern void preempt_count_add(int val);
extern void preempt_count_sub(int val);
#define preempt_count_dec_and_test() ({ preempt_count_sub(1); should_resched(); })
#else
#define preempt_count_add(val)	__preempt_count_add(val)
#define preempt_count_sub(val)	__preempt_count_sub(val)
#define preempt_count_dec_and_test() __preempt_count_dec_and_test()
#endif

#define __preempt_count_inc() __preempt_count_add(1)
#define __preempt_count_dec() __preempt_count_sub(1)

#define preempt_count_inc() preempt_count_add(1)
#define preempt_count_dec() preempt_count_sub(1)

#ifdef CONFIG_PREEMPT_COUNT

#define preempt_disable() \
do { \
	preempt_count_inc(); \
	barrier(); \
} while (0)

#define sched_preempt_enable_no_resched() \
do { \
	barrier(); \
	preempt_count_dec(); \
} while (0)

#define preempt_enable_no_resched() sched_preempt_enable_no_resched()

#ifdef CONFIG_PREEMPT
#define preempt_enable() \
do { \
	barrier(); \
	if (unlikely(preempt_count_dec_and_test())) \
		__preempt_schedule(); \
} while (0)

#define preempt_check_resched() \
do { \
	if (should_resched()) \
		__preempt_schedule(); \
} while (0)

#else
#define preempt_enable() \
do { \
	barrier(); \
	preempt_count_dec(); \
} while (0)
#define preempt_check_resched() do { } while (0)
#endif

#define preempt_disable_notrace() \
do { \
	__preempt_count_inc(); \
>>>>>>> refs/remotes/origin/master
	barrier(); \
} while (0)

#define preempt_enable_no_resched_notrace() \
do { \
	barrier(); \
<<<<<<< HEAD
	dec_preempt_count_notrace(); \
} while (0)

/* preempt_check_resched is OK to trace */
#define preempt_enable_notrace() \
do { \
	preempt_enable_no_resched_notrace(); \
	barrier(); \
	preempt_check_resched(); \
} while (0)

<<<<<<< HEAD
#else
=======
#else /* !CONFIG_PREEMPT_COUNT */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	__preempt_count_dec(); \
} while (0)

<<<<<<< HEAD
#ifdef CONFIG_PREEMPT

#ifndef CONFIG_CONTEXT_TRACKING
#define __preempt_schedule_context() __preempt_schedule()
#endif
=======
/*
 * Even if we don't have any preemption, we need preempt disable/enable
 * to be barriers, so that we don't have things like get_user/put_user
 * that can cause faults and scheduling migrate into our preempt-protected
 * region.
 */
#define preempt_disable()		barrier()
#define sched_preempt_enable_no_resched()	barrier()
#define preempt_enable_no_resched()	barrier()
#define preempt_enable()		barrier()

#define preempt_disable_notrace()		barrier()
#define preempt_enable_no_resched_notrace()	barrier()
#define preempt_enable_notrace()		barrier()
>>>>>>> refs/remotes/origin/cm-11.0

#define preempt_enable_notrace() \
do { \
	barrier(); \
	if (unlikely(__preempt_count_dec_and_test())) \
		__preempt_schedule_context(); \
} while (0)
#else
#define preempt_enable_notrace() \
do { \
	barrier(); \
	__preempt_count_dec(); \
} while (0)
#endif

#else /* !CONFIG_PREEMPT_COUNT */
>>>>>>> refs/remotes/origin/master

/*
 * Even if we don't have any preemption, we need preempt disable/enable
 * to be barriers, so that we don't have things like get_user/put_user
 * that can cause faults and scheduling migrate into our preempt-protected
 * region.
 */
<<<<<<< HEAD
#define preempt_disable()		barrier()
<<<<<<< HEAD
#define preempt_enable_no_resched()	barrier()
#define preempt_enable()		barrier()
#define preempt_check_resched()		barrier()
=======
#define sched_preempt_enable_no_resched()	barrier()
#define preempt_enable_no_resched()	barrier()
#define preempt_enable()		barrier()
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define preempt_disable()			barrier()
#define sched_preempt_enable_no_resched()	barrier()
#define preempt_enable_no_resched()		barrier()
#define preempt_enable()			barrier()
#define preempt_check_resched()			do { } while (0)
>>>>>>> refs/remotes/origin/master

#define preempt_disable_notrace()		barrier()
#define preempt_enable_no_resched_notrace()	barrier()
#define preempt_enable_notrace()		barrier()

<<<<<<< HEAD
<<<<<<< HEAD
#endif
=======
#endif /* CONFIG_PREEMPT_COUNT */
>>>>>>> refs/remotes/origin/cm-10.0
=======
#endif /* CONFIG_PREEMPT_COUNT */

#ifdef MODULE
/*
 * Modules have no business playing preemption tricks.
 */
#undef sched_preempt_enable_no_resched
#undef preempt_enable_no_resched
#undef preempt_enable_no_resched_notrace
#undef preempt_check_resched
#endif

#ifdef CONFIG_PREEMPT
#define preempt_set_need_resched() \
do { \
	set_preempt_need_resched(); \
} while (0)
#define preempt_fold_need_resched() \
do { \
	if (tif_need_resched()) \
		set_preempt_need_resched(); \
} while (0)
#else
#define preempt_set_need_resched() do { } while (0)
#define preempt_fold_need_resched() do { } while (0)
#endif
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_PREEMPT_NOTIFIERS

struct preempt_notifier;

/**
 * preempt_ops - notifiers called when a task is preempted and rescheduled
 * @sched_in: we're about to be rescheduled:
 *    notifier: struct preempt_notifier for the task being scheduled
 *    cpu:  cpu we're scheduled on
 * @sched_out: we've just been preempted
 *    notifier: struct preempt_notifier for the task being preempted
 *    next: the task that's kicking us out
 *
 * Please note that sched_in and out are called under different
 * contexts.  sched_out is called with rq lock held and irq disabled
 * while sched_in is called without rq lock and irq enabled.  This
 * difference is intentional and depended upon by its users.
 */
struct preempt_ops {
	void (*sched_in)(struct preempt_notifier *notifier, int cpu);
	void (*sched_out)(struct preempt_notifier *notifier,
			  struct task_struct *next);
};

/**
 * preempt_notifier - key for installing preemption notifiers
 * @link: internal use
 * @ops: defines the notifier functions to be called
 *
 * Usually used in conjunction with container_of().
 */
struct preempt_notifier {
	struct hlist_node link;
	struct preempt_ops *ops;
};

void preempt_notifier_register(struct preempt_notifier *notifier);
void preempt_notifier_unregister(struct preempt_notifier *notifier);

static inline void preempt_notifier_init(struct preempt_notifier *notifier,
				     struct preempt_ops *ops)
{
	INIT_HLIST_NODE(&notifier->link);
	notifier->ops = ops;
}

#endif

#endif /* __LINUX_PREEMPT_H */
