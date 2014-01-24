#undef TRACE_SYSTEM
#define TRACE_SYSTEM signal

#if !defined(_TRACE_SIGNAL_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_SIGNAL_H

#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/tracepoint.h>

#define TP_STORE_SIGINFO(__entry, info)				\
	do {							\
		if (info == SEND_SIG_NOINFO ||			\
		    info == SEND_SIG_FORCED) {			\
			__entry->errno	= 0;			\
			__entry->code	= SI_USER;		\
		} else if (info == SEND_SIG_PRIV) {		\
			__entry->errno	= 0;			\
			__entry->code	= SI_KERNEL;		\
		} else {					\
			__entry->errno	= info->si_errno;	\
			__entry->code	= info->si_code;	\
		}						\
	} while (0)

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#ifndef TRACE_HEADER_MULTI_READ
enum {
	TRACE_SIGNAL_DELIVERED,
	TRACE_SIGNAL_IGNORED,
	TRACE_SIGNAL_ALREADY_PENDING,
	TRACE_SIGNAL_OVERFLOW_FAIL,
	TRACE_SIGNAL_LOSE_INFO,
};
#endif

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/**
 * signal_generate - called when a signal is generated
 * @sig: signal number
 * @info: pointer to struct siginfo
 * @task: pointer to struct task_struct
<<<<<<< HEAD
<<<<<<< HEAD
=======
 * @group: shared or private
 * @result: TRACE_SIGNAL_*
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * @group: shared or private
 * @result: TRACE_SIGNAL_*
>>>>>>> refs/remotes/origin/master
 *
 * Current process sends a 'sig' signal to 'task' process with
 * 'info' siginfo. If 'info' is SEND_SIG_NOINFO or SEND_SIG_PRIV,
 * 'info' is not a pointer and you can't access its field. Instead,
 * SEND_SIG_NOINFO means that si_code is SI_USER, and SEND_SIG_PRIV
 * means that si_code is SI_KERNEL.
 */
TRACE_EVENT(signal_generate,

<<<<<<< HEAD
<<<<<<< HEAD
	TP_PROTO(int sig, struct siginfo *info, struct task_struct *task),

	TP_ARGS(sig, info, task),
=======
=======
>>>>>>> refs/remotes/origin/master
	TP_PROTO(int sig, struct siginfo *info, struct task_struct *task,
			int group, int result),

	TP_ARGS(sig, info, task, group, result),
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	TP_STRUCT__entry(
		__field(	int,	sig			)
		__field(	int,	errno			)
		__field(	int,	code			)
		__array(	char,	comm,	TASK_COMM_LEN	)
		__field(	pid_t,	pid			)
<<<<<<< HEAD
<<<<<<< HEAD
=======
		__field(	int,	group			)
		__field(	int,	result			)
>>>>>>> refs/remotes/origin/cm-10.0
=======
		__field(	int,	group			)
		__field(	int,	result			)
>>>>>>> refs/remotes/origin/master
	),

	TP_fast_assign(
		__entry->sig	= sig;
		TP_STORE_SIGINFO(__entry, info);
		memcpy(__entry->comm, task->comm, TASK_COMM_LEN);
		__entry->pid	= task->pid;
<<<<<<< HEAD
<<<<<<< HEAD
	),

	TP_printk("sig=%d errno=%d code=%d comm=%s pid=%d",
		  __entry->sig, __entry->errno, __entry->code,
		  __entry->comm, __entry->pid)
=======
=======
>>>>>>> refs/remotes/origin/master
		__entry->group	= group;
		__entry->result	= result;
	),

	TP_printk("sig=%d errno=%d code=%d comm=%s pid=%d grp=%d res=%d",
		  __entry->sig, __entry->errno, __entry->code,
		  __entry->comm, __entry->pid, __entry->group,
		  __entry->result)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
);

/**
 * signal_deliver - called when a signal is delivered
 * @sig: signal number
 * @info: pointer to struct siginfo
 * @ka: pointer to struct k_sigaction
 *
 * A 'sig' signal is delivered to current process with 'info' siginfo,
 * and it will be handled by 'ka'. ka->sa.sa_handler can be SIG_IGN or
 * SIG_DFL.
 * Note that some signals reported by signal_generate tracepoint can be
 * lost, ignored or modified (by debugger) before hitting this tracepoint.
 * This means, this can show which signals are actually delivered, but
 * matching generated signals and delivered signals may not be correct.
 */
TRACE_EVENT(signal_deliver,

	TP_PROTO(int sig, struct siginfo *info, struct k_sigaction *ka),

	TP_ARGS(sig, info, ka),

	TP_STRUCT__entry(
		__field(	int,		sig		)
		__field(	int,		errno		)
		__field(	int,		code		)
		__field(	unsigned long,	sa_handler	)
		__field(	unsigned long,	sa_flags	)
	),

	TP_fast_assign(
		__entry->sig	= sig;
		TP_STORE_SIGINFO(__entry, info);
		__entry->sa_handler	= (unsigned long)ka->sa.sa_handler;
		__entry->sa_flags	= ka->sa.sa_flags;
	),

	TP_printk("sig=%d errno=%d code=%d sa_handler=%lx sa_flags=%lx",
		  __entry->sig, __entry->errno, __entry->code,
		  __entry->sa_handler, __entry->sa_flags)
);

<<<<<<< HEAD
<<<<<<< HEAD
DECLARE_EVENT_CLASS(signal_queue_overflow,

	TP_PROTO(int sig, int group, struct siginfo *info),

	TP_ARGS(sig, group, info),

	TP_STRUCT__entry(
		__field(	int,	sig	)
		__field(	int,	group	)
		__field(	int,	errno	)
		__field(	int,	code	)
	),

	TP_fast_assign(
		__entry->sig	= sig;
		__entry->group	= group;
		TP_STORE_SIGINFO(__entry, info);
	),

	TP_printk("sig=%d group=%d errno=%d code=%d",
		  __entry->sig, __entry->group, __entry->errno, __entry->code)
);

/**
 * signal_overflow_fail - called when signal queue is overflow
 * @sig: signal number
 * @group: signal to process group or not (bool)
 * @info: pointer to struct siginfo
 *
 * Kernel fails to generate 'sig' signal with 'info' siginfo, because
 * siginfo queue is overflow, and the signal is dropped.
 * 'group' is not 0 if the signal will be sent to a process group.
 * 'sig' is always one of RT signals.
 */
DEFINE_EVENT(signal_queue_overflow, signal_overflow_fail,

	TP_PROTO(int sig, int group, struct siginfo *info),

	TP_ARGS(sig, group, info)
);

/**
 * signal_lose_info - called when siginfo is lost
 * @sig: signal number
 * @group: signal to process group or not (bool)
 * @info: pointer to struct siginfo
 *
 * Kernel generates 'sig' signal but loses 'info' siginfo, because siginfo
 * queue is overflow.
 * 'group' is not 0 if the signal will be sent to a process group.
 * 'sig' is always one of non-RT signals.
 */
DEFINE_EVENT(signal_queue_overflow, signal_lose_info,

	TP_PROTO(int sig, int group, struct siginfo *info),

	TP_ARGS(sig, group, info)
);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#endif /* _TRACE_SIGNAL_H */

/* This part must be outside protection */
#include <trace/define_trace.h>
