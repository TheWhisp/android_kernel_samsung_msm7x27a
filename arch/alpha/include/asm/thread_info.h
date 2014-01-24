#ifndef _ALPHA_THREAD_INFO_H
#define _ALPHA_THREAD_INFO_H

#ifdef __KERNEL__

#ifndef __ASSEMBLY__
#include <asm/processor.h>
#include <asm/types.h>
#include <asm/hwrpb.h>
<<<<<<< HEAD
=======
#include <asm/sysinfo.h>
>>>>>>> refs/remotes/origin/master
#endif

#ifndef __ASSEMBLY__
struct thread_info {
	struct pcb_struct	pcb;		/* palcode state */

	struct task_struct	*task;		/* main task structure */
	unsigned int		flags;		/* low level flags */
	unsigned int		ieee_state;	/* see fpu.h */

	struct exec_domain	*exec_domain;	/* execution domain */
	mm_segment_t		addr_limit;	/* thread address space */
	unsigned		cpu;		/* current CPU */
	int			preempt_count; /* 0 => preemptable, <0 => BUG */
<<<<<<< HEAD
=======
	unsigned int		status;		/* thread-synchronous flags */
>>>>>>> refs/remotes/origin/master

	int bpt_nsaved;
	unsigned long bpt_addr[2];		/* breakpoint handling  */
	unsigned int bpt_insn[2];

	struct restart_block	restart_block;
};

/*
 * Macros/functions for gaining access to the thread information structure.
 */
#define INIT_THREAD_INFO(tsk)			\
{						\
	.task		= &tsk,			\
	.exec_domain	= &default_exec_domain,	\
	.addr_limit	= KERNEL_DS,		\
	.preempt_count	= INIT_PREEMPT_COUNT,	\
	.restart_block = {			\
		.fn = do_no_restart_syscall,	\
	},					\
}

#define init_thread_info	(init_thread_union.thread_info)
#define init_stack		(init_thread_union.stack)

/* How to get the thread information struct from C.  */
register struct thread_info *__current_thread_info __asm__("$8");
#define current_thread_info()  __current_thread_info

#endif /* __ASSEMBLY__ */

/* Thread information allocation.  */
#define THREAD_SIZE_ORDER 1
#define THREAD_SIZE (2*PAGE_SIZE)

<<<<<<< HEAD
#define PREEMPT_ACTIVE		0x40000000

=======
>>>>>>> refs/remotes/origin/master
/*
 * Thread information flags:
 * - these are process state flags and used from assembly
 * - pending work-to-be-done flags come first and must be assigned to be
 *   within bits 0 to 7 to fit in and immediate operand.
<<<<<<< HEAD
 * - ALPHA_UAC_SHIFT below must be kept consistent with the unaligned
 *   control flags.
=======
>>>>>>> refs/remotes/origin/master
 *
 * TIF_SYSCALL_TRACE is known to be 0 via blbs.
 */
#define TIF_SYSCALL_TRACE	0	/* syscall trace active */
#define TIF_NOTIFY_RESUME	1	/* callback before returning to user */
#define TIF_SIGPENDING		2	/* signal pending */
#define TIF_NEED_RESCHED	3	/* rescheduling necessary */
<<<<<<< HEAD
#define TIF_POLLING_NRFLAG	8	/* poll_idle is polling NEED_RESCHED */
#define TIF_DIE_IF_KERNEL	9	/* dik recursion lock */
<<<<<<< HEAD
#define TIF_UAC_NOPRINT		10	/* see sysinfo.h */
#define TIF_UAC_NOFIX		11
#define TIF_UAC_SIGBUS		12
#define TIF_MEMDIE		13	/* is terminating due to OOM killer */
#define TIF_RESTORE_SIGMASK	14	/* restore signal mask in do_signal */
#define TIF_FREEZE		16	/* is freezing for suspend */
=======
#define TIF_UAC_NOPRINT		10	/* ! Preserve sequence of following */
#define TIF_UAC_NOFIX		11	/* ! flags as they match            */
#define TIF_UAC_SIGBUS		12	/* ! userspace part of 'osf_sysinfo' */
#define TIF_MEMDIE		13	/* is terminating due to OOM killer */
#define TIF_RESTORE_SIGMASK	14	/* restore signal mask in do_signal */
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define TIF_DIE_IF_KERNEL	9	/* dik recursion lock */
#define TIF_MEMDIE		13	/* is terminating due to OOM killer */
>>>>>>> refs/remotes/origin/master

#define _TIF_SYSCALL_TRACE	(1<<TIF_SYSCALL_TRACE)
#define _TIF_SIGPENDING		(1<<TIF_SIGPENDING)
#define _TIF_NEED_RESCHED	(1<<TIF_NEED_RESCHED)
<<<<<<< HEAD
#define _TIF_POLLING_NRFLAG	(1<<TIF_POLLING_NRFLAG)
#define _TIF_RESTORE_SIGMASK	(1<<TIF_RESTORE_SIGMASK)
#define _TIF_NOTIFY_RESUME	(1<<TIF_NOTIFY_RESUME)
<<<<<<< HEAD
#define _TIF_FREEZE		(1<<TIF_FREEZE)
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define _TIF_NOTIFY_RESUME	(1<<TIF_NOTIFY_RESUME)
>>>>>>> refs/remotes/origin/master

/* Work to do on interrupt/exception return.  */
#define _TIF_WORK_MASK		(_TIF_SIGPENDING | _TIF_NEED_RESCHED | \
				 _TIF_NOTIFY_RESUME)

/* Work to do on any return to userspace.  */
#define _TIF_ALLWORK_MASK	(_TIF_WORK_MASK		\
				 | _TIF_SYSCALL_TRACE)

<<<<<<< HEAD
<<<<<<< HEAD
#define ALPHA_UAC_SHIFT		10
=======
#define ALPHA_UAC_SHIFT		TIF_UAC_NOPRINT
>>>>>>> refs/remotes/origin/cm-10.0
#define ALPHA_UAC_MASK		(1 << TIF_UAC_NOPRINT | 1 << TIF_UAC_NOFIX | \
				 1 << TIF_UAC_SIGBUS)

#define SET_UNALIGN_CTL(task,value)	({				     \
	task_thread_info(task)->flags = ((task_thread_info(task)->flags &    \
		~ALPHA_UAC_MASK)					     \
		| (((value) << ALPHA_UAC_SHIFT)       & (1<<TIF_UAC_NOPRINT))\
		| (((value) << (ALPHA_UAC_SHIFT + 1)) & (1<<TIF_UAC_SIGBUS)) \
		| (((value) << (ALPHA_UAC_SHIFT - 1)) & (1<<TIF_UAC_NOFIX)));\
	0; })

#define GET_UNALIGN_CTL(task,value)	({				\
	put_user((task_thread_info(task)->flags & (1 << TIF_UAC_NOPRINT))\
		  >> ALPHA_UAC_SHIFT					\
		 | (task_thread_info(task)->flags & (1 << TIF_UAC_SIGBUS))\
		 >> (ALPHA_UAC_SHIFT + 1)				\
		 | (task_thread_info(task)->flags & (1 << TIF_UAC_NOFIX))\
		 >> (ALPHA_UAC_SHIFT - 1),				\
		 (int __user *)(value));				\
=======
#define TS_UAC_NOPRINT		0x0001	/* ! Preserve the following three */
#define TS_UAC_NOFIX		0x0002	/* ! flags as they match          */
#define TS_UAC_SIGBUS		0x0004	/* ! userspace part of 'osf_sysinfo' */
#define TS_RESTORE_SIGMASK	0x0008	/* restore signal mask in do_signal() */
#define TS_POLLING		0x0010	/* idle task polling need_resched,
					   skip sending interrupt */

#ifndef __ASSEMBLY__
#define HAVE_SET_RESTORE_SIGMASK	1
static inline void set_restore_sigmask(void)
{
	struct thread_info *ti = current_thread_info();
	ti->status |= TS_RESTORE_SIGMASK;
	WARN_ON(!test_bit(TIF_SIGPENDING, (unsigned long *)&ti->flags));
}
static inline void clear_restore_sigmask(void)
{
	current_thread_info()->status &= ~TS_RESTORE_SIGMASK;
}
static inline bool test_restore_sigmask(void)
{
	return current_thread_info()->status & TS_RESTORE_SIGMASK;
}
static inline bool test_and_clear_restore_sigmask(void)
{
	struct thread_info *ti = current_thread_info();
	if (!(ti->status & TS_RESTORE_SIGMASK))
		return false;
	ti->status &= ~TS_RESTORE_SIGMASK;
	return true;
}
#endif

#define SET_UNALIGN_CTL(task,value)	({				\
	__u32 status = task_thread_info(task)->status & ~UAC_BITMASK;	\
	if (value & PR_UNALIGN_NOPRINT)					\
		status |= TS_UAC_NOPRINT;				\
	if (value & PR_UNALIGN_SIGBUS)					\
		status |= TS_UAC_SIGBUS;				\
	if (value & 4)	/* alpha-specific */				\
		status |= TS_UAC_NOFIX;					\
	task_thread_info(task)->status = status;			\
	0; })

#define GET_UNALIGN_CTL(task,value)	({				\
	__u32 status = task_thread_info(task)->status & ~UAC_BITMASK;	\
	__u32 res = 0;							\
	if (status & TS_UAC_NOPRINT)					\
		res |= PR_UNALIGN_NOPRINT;				\
	if (status & TS_UAC_SIGBUS)					\
		res |= PR_UNALIGN_SIGBUS;				\
	if (status & TS_UAC_NOFIX)					\
		res |= 4;						\
	put_user(res, (int __user *)(value));				\
>>>>>>> refs/remotes/origin/master
	})

#endif /* __KERNEL__ */
#endif /* _ALPHA_THREAD_INFO_H */
