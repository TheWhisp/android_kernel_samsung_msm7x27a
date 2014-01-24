#ifndef _ASM_PARISC_THREAD_INFO_H
#define _ASM_PARISC_THREAD_INFO_H

#ifdef __KERNEL__

#ifndef __ASSEMBLY__
#include <asm/processor.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <asm/special_insns.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm/special_insns.h>
>>>>>>> refs/remotes/origin/master

struct thread_info {
	struct task_struct *task;	/* main task structure */
	struct exec_domain *exec_domain;/* execution domain */
	unsigned long flags;		/* thread_info flags (see TIF_*) */
	mm_segment_t addr_limit;	/* user-level address space limit */
	__u32 cpu;			/* current CPU */
	int preempt_count;		/* 0=premptable, <0=BUG; will also serve as bh-counter */
	struct restart_block restart_block;
};

#define INIT_THREAD_INFO(tsk)			\
{						\
	.task		= &tsk,			\
	.exec_domain	= &default_exec_domain,	\
	.flags		= 0,			\
	.cpu		= 0,			\
	.addr_limit	= KERNEL_DS,		\
	.preempt_count	= INIT_PREEMPT_COUNT,	\
  	.restart_block	= {			\
		.fn = do_no_restart_syscall	\
	}					\
}

#define init_thread_info        (init_thread_union.thread_info)
#define init_stack              (init_thread_union.stack)

/* how to get the thread information struct from C */
#define current_thread_info()	((struct thread_info *)mfctl(30))

#endif /* !__ASSEMBLY */

/* thread information allocation */

<<<<<<< HEAD
#define THREAD_SIZE_ORDER            2
=======
#define THREAD_SIZE_ORDER	2 /* PA-RISC requires at least 16k stack */
>>>>>>> refs/remotes/origin/master
/* Be sure to hunt all references to this down when you change the size of
 * the kernel stack */
#define THREAD_SIZE             (PAGE_SIZE << THREAD_SIZE_ORDER)
#define THREAD_SHIFT            (PAGE_SHIFT + THREAD_SIZE_ORDER)

<<<<<<< HEAD
#define PREEMPT_ACTIVE_BIT	28
#define PREEMPT_ACTIVE		(1 << PREEMPT_ACTIVE_BIT)

=======
>>>>>>> refs/remotes/origin/master
/*
 * thread information flags
 */
#define TIF_SYSCALL_TRACE	0	/* syscall trace active */
#define TIF_SIGPENDING		1	/* signal pending */
#define TIF_NEED_RESCHED	2	/* rescheduling necessary */
#define TIF_POLLING_NRFLAG	3	/* true if poll_idle() is polling TIF_NEED_RESCHED */
#define TIF_32BIT               4       /* 32 bit binary */
#define TIF_MEMDIE		5	/* is terminating due to OOM killer */
#define TIF_RESTORE_SIGMASK	6	/* restore saved signal mask */
<<<<<<< HEAD
<<<<<<< HEAD
#define TIF_FREEZE		7	/* is freezing for suspend */
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define TIF_SYSCALL_AUDIT	7	/* syscall auditing active */
>>>>>>> refs/remotes/origin/master
#define TIF_NOTIFY_RESUME	8	/* callback before returning to user */
#define TIF_SINGLESTEP		9	/* single stepping? */
#define TIF_BLOCKSTEP		10	/* branch stepping? */

#define _TIF_SYSCALL_TRACE	(1 << TIF_SYSCALL_TRACE)
#define _TIF_SIGPENDING		(1 << TIF_SIGPENDING)
#define _TIF_NEED_RESCHED	(1 << TIF_NEED_RESCHED)
#define _TIF_POLLING_NRFLAG	(1 << TIF_POLLING_NRFLAG)
#define _TIF_32BIT		(1 << TIF_32BIT)
<<<<<<< HEAD
#define _TIF_RESTORE_SIGMASK	(1 << TIF_RESTORE_SIGMASK)
<<<<<<< HEAD
#define _TIF_FREEZE		(1 << TIF_FREEZE)
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define _TIF_SYSCALL_AUDIT	(1 << TIF_SYSCALL_AUDIT)
>>>>>>> refs/remotes/origin/master
#define _TIF_NOTIFY_RESUME	(1 << TIF_NOTIFY_RESUME)
#define _TIF_SINGLESTEP		(1 << TIF_SINGLESTEP)
#define _TIF_BLOCKSTEP		(1 << TIF_BLOCKSTEP)

#define _TIF_USER_WORK_MASK     (_TIF_SIGPENDING | _TIF_NOTIFY_RESUME | \
<<<<<<< HEAD
                                 _TIF_NEED_RESCHED | _TIF_RESTORE_SIGMASK)
=======
                                 _TIF_NEED_RESCHED)
#define _TIF_SYSCALL_TRACE_MASK (_TIF_SYSCALL_TRACE | _TIF_SINGLESTEP |	\
				 _TIF_BLOCKSTEP | _TIF_SYSCALL_AUDIT)
>>>>>>> refs/remotes/origin/master

#endif /* __KERNEL__ */

#endif /* _ASM_PARISC_THREAD_INFO_H */
