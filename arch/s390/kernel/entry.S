/*
 *  arch/s390/kernel/entry.S
 *    S390 low-level entry points.
 *
<<<<<<< HEAD
 *    Copyright (C) IBM Corp. 1999,2006
=======
 *    Copyright (C) IBM Corp. 1999,2012
>>>>>>> refs/remotes/origin/cm-10.0
 *    Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com),
 *		 Hartmut Penner (hp@de.ibm.com),
 *		 Denis Joseph Barrow (djbarrow@de.ibm.com,barrow_dj@yahoo.com),
 *		 Heiko Carstens <heiko.carstens@de.ibm.com>
 */

<<<<<<< HEAD
#include <linux/linkage.h>
#include <linux/init.h>
=======
#include <linux/init.h>
#include <linux/linkage.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <asm/cache.h>
#include <asm/errno.h>
#include <asm/ptrace.h>
#include <asm/thread_info.h>
#include <asm/asm-offsets.h>
#include <asm/unistd.h>
#include <asm/page.h>

<<<<<<< HEAD
/*
 * Stack layout for the system_call stack entry.
 * The first few entries are identical to the user_regs_struct.
 */
SP_PTREGS    =	STACK_FRAME_OVERHEAD
SP_ARGS      =	STACK_FRAME_OVERHEAD + __PT_ARGS
SP_PSW	     =	STACK_FRAME_OVERHEAD + __PT_PSW
SP_R0	     =	STACK_FRAME_OVERHEAD + __PT_GPRS
SP_R1	     =	STACK_FRAME_OVERHEAD + __PT_GPRS + 4
SP_R2	     =	STACK_FRAME_OVERHEAD + __PT_GPRS + 8
SP_R3	     =	STACK_FRAME_OVERHEAD + __PT_GPRS + 12
SP_R4	     =	STACK_FRAME_OVERHEAD + __PT_GPRS + 16
SP_R5	     =	STACK_FRAME_OVERHEAD + __PT_GPRS + 20
SP_R6	     =	STACK_FRAME_OVERHEAD + __PT_GPRS + 24
SP_R7	     =	STACK_FRAME_OVERHEAD + __PT_GPRS + 28
SP_R8	     =	STACK_FRAME_OVERHEAD + __PT_GPRS + 32
SP_R9	     =	STACK_FRAME_OVERHEAD + __PT_GPRS + 36
SP_R10	     =	STACK_FRAME_OVERHEAD + __PT_GPRS + 40
SP_R11	     =	STACK_FRAME_OVERHEAD + __PT_GPRS + 44
SP_R12	     =	STACK_FRAME_OVERHEAD + __PT_GPRS + 48
SP_R13	     =	STACK_FRAME_OVERHEAD + __PT_GPRS + 52
SP_R14	     =	STACK_FRAME_OVERHEAD + __PT_GPRS + 56
SP_R15	     =	STACK_FRAME_OVERHEAD + __PT_GPRS + 60
SP_ORIG_R2   =	STACK_FRAME_OVERHEAD + __PT_ORIG_GPR2
SP_ILC	     =	STACK_FRAME_OVERHEAD + __PT_ILC
SP_SVCNR     =	STACK_FRAME_OVERHEAD + __PT_SVCNR
SP_SIZE      =	STACK_FRAME_OVERHEAD + __PT_SIZE

_TIF_WORK_SVC = (_TIF_SIGPENDING | _TIF_NOTIFY_RESUME | _TIF_NEED_RESCHED | \
		 _TIF_MCCK_PENDING | _TIF_RESTART_SVC | _TIF_PER_TRAP )
_TIF_WORK_INT = (_TIF_SIGPENDING | _TIF_NOTIFY_RESUME | _TIF_NEED_RESCHED | \
		 _TIF_MCCK_PENDING)
_TIF_SYSCALL = (_TIF_SYSCALL_TRACE>>8 | _TIF_SYSCALL_AUDIT>>8 | \
		_TIF_SECCOMP>>8 | _TIF_SYSCALL_TRACEPOINT>>8)
=======
__PT_R0      =	__PT_GPRS
__PT_R1      =	__PT_GPRS + 4
__PT_R2      =	__PT_GPRS + 8
__PT_R3      =	__PT_GPRS + 12
__PT_R4      =	__PT_GPRS + 16
__PT_R5      =	__PT_GPRS + 20
__PT_R6      =	__PT_GPRS + 24
__PT_R7      =	__PT_GPRS + 28
__PT_R8      =	__PT_GPRS + 32
__PT_R9      =	__PT_GPRS + 36
__PT_R10     =	__PT_GPRS + 40
__PT_R11     =	__PT_GPRS + 44
__PT_R12     =	__PT_GPRS + 48
__PT_R13     =	__PT_GPRS + 524
__PT_R14     =	__PT_GPRS + 56
__PT_R15     =	__PT_GPRS + 60

_TIF_WORK_SVC = (_TIF_SIGPENDING | _TIF_NOTIFY_RESUME | _TIF_NEED_RESCHED | \
		 _TIF_MCCK_PENDING | _TIF_PER_TRAP )
_TIF_WORK_INT = (_TIF_SIGPENDING | _TIF_NOTIFY_RESUME | _TIF_NEED_RESCHED | \
		 _TIF_MCCK_PENDING)
_TIF_TRACE    = (_TIF_SYSCALL_TRACE | _TIF_SYSCALL_AUDIT | _TIF_SECCOMP | \
		 _TIF_SYSCALL_TRACEPOINT)
>>>>>>> refs/remotes/origin/cm-10.0

STACK_SHIFT = PAGE_SHIFT + THREAD_ORDER
STACK_SIZE  = 1 << STACK_SHIFT

#define BASED(name) name-system_call(%r13)

<<<<<<< HEAD
#ifdef CONFIG_TRACE_IRQFLAGS
	.macro	TRACE_IRQS_ON
	basr	%r2,%r0
	l	%r1,BASED(.Ltrace_irq_on_caller)
	basr	%r14,%r1
	.endm

	.macro	TRACE_IRQS_OFF
	basr	%r2,%r0
	l	%r1,BASED(.Ltrace_irq_off_caller)
	basr	%r14,%r1
	.endm
#else
#define TRACE_IRQS_ON
#define TRACE_IRQS_OFF
#endif

#ifdef CONFIG_LOCKDEP
	.macro	LOCKDEP_SYS_EXIT
	tm	SP_PSW+1(%r15),0x01	# returning to user ?
	jz	0f
	l	%r1,BASED(.Llockdep_sys_exit)
	basr	%r14,%r1
0:
	.endm
#else
#define LOCKDEP_SYS_EXIT
#endif

/*
 * Register usage in interrupt handlers:
 *    R9  - pointer to current task structure
 *    R13 - pointer to literal pool
 *    R14 - return register for function calls
 *    R15 - kernel stack pointer
 */

	.macro	UPDATE_VTIME lc_from,lc_to,lc_sum
	lm	%r10,%r11,\lc_from
	sl	%r10,\lc_to
	sl	%r11,\lc_to+4
	bc	3,BASED(0f)
	sl	%r10,BASED(.Lc_1)
0:	al	%r10,\lc_sum
	al	%r11,\lc_sum+4
	bc	12,BASED(1f)
	al	%r10,BASED(.Lc_1)
1:	stm	%r10,%r11,\lc_sum
	.endm

	.macro	SAVE_ALL_SVC psworg,savearea
	stm	%r12,%r15,\savearea
	l	%r13,__LC_SVC_NEW_PSW+4	# load &system_call to %r13
	l	%r15,__LC_KERNEL_STACK	# problem state -> load ksp
	s	%r15,BASED(.Lc_spsize)	# make room for registers & psw
	.endm

	.macro	SAVE_ALL_BASE savearea
	stm	%r12,%r15,\savearea
	l	%r13,__LC_SVC_NEW_PSW+4	# load &system_call to %r13
	.endm

	.macro	SAVE_ALL_PGM psworg,savearea
	tm	\psworg+1,0x01		# test problem state bit
#ifdef CONFIG_CHECK_STACK
	bnz	BASED(1f)
	tml	%r15,STACK_SIZE - CONFIG_STACK_GUARD
	bnz	BASED(2f)
	la	%r12,\psworg
	b	BASED(stack_overflow)
#else
	bz	BASED(2f)
#endif
1:	l	%r15,__LC_KERNEL_STACK	# problem state -> load ksp
2:	s	%r15,BASED(.Lc_spsize)	# make room for registers & psw
	.endm

	.macro	SAVE_ALL_ASYNC psworg,savearea
	stm	%r12,%r15,\savearea
	l	%r13,__LC_SVC_NEW_PSW+4	# load &system_call to %r13
	la	%r12,\psworg
	tm	\psworg+1,0x01		# test problem state bit
	bnz	BASED(1f)		# from user -> load async stack
	clc	\psworg+4(4),BASED(.Lcritical_end)
	bhe	BASED(0f)
	clc	\psworg+4(4),BASED(.Lcritical_start)
	bl	BASED(0f)
	l	%r14,BASED(.Lcleanup_critical)
	basr	%r14,%r14
	tm	1(%r12),0x01		# retest problem state after cleanup
	bnz	BASED(1f)
0:	l	%r14,__LC_ASYNC_STACK	# are we already on the async stack ?
	slr	%r14,%r15
	sra	%r14,STACK_SHIFT
#ifdef CONFIG_CHECK_STACK
	bnz	BASED(1f)
	tml	%r15,STACK_SIZE - CONFIG_STACK_GUARD
	bnz	BASED(2f)
	b	BASED(stack_overflow)
#else
	bz	BASED(2f)
#endif
1:	l	%r15,__LC_ASYNC_STACK
2:	s	%r15,BASED(.Lc_spsize)	# make room for registers & psw
	.endm

	.macro	CREATE_STACK_FRAME savearea
	xc	__SF_BACKCHAIN(4,%r15),__SF_BACKCHAIN(%r15)
	st	%r2,SP_ORIG_R2(%r15)	# store original content of gpr 2
	mvc	SP_R12(16,%r15),\savearea # move %r12-%r15 to stack
	stm	%r0,%r11,SP_R0(%r15)	# store gprs %r0-%r11 to kernel stack
	.endm

	.macro	RESTORE_ALL psworg,sync
	mvc	\psworg(8),SP_PSW(%r15) # move user PSW to lowcore
	.if !\sync
	ni	\psworg+1,0xfd		# clear wait state bit
	.endif
	lm	%r0,%r15,SP_R0(%r15)	# load gprs 0-15 of user
	stpt	__LC_EXIT_TIMER
	lpsw	\psworg			# back to caller
	.endm

	.macro REENABLE_IRQS
	mvc	__SF_EMPTY(1,%r15),SP_PSW(%r15)
	ni	__SF_EMPTY(%r15),0xbf
	ssm	__SF_EMPTY(%r15)
=======
	.macro	TRACE_IRQS_ON
#ifdef CONFIG_TRACE_IRQFLAGS
	basr	%r2,%r0
	l	%r1,BASED(.Lhardirqs_on)
	basr	%r14,%r1		# call trace_hardirqs_on_caller
#endif
	.endm

	.macro	TRACE_IRQS_OFF
#ifdef CONFIG_TRACE_IRQFLAGS
	basr	%r2,%r0
	l	%r1,BASED(.Lhardirqs_off)
	basr	%r14,%r1		# call trace_hardirqs_off_caller
#endif
	.endm

	.macro	LOCKDEP_SYS_EXIT
#ifdef CONFIG_LOCKDEP
	tm	__PT_PSW+1(%r11),0x01	# returning to user ?
	jz	.+10
	l	%r1,BASED(.Llockdep_sys_exit)
	basr	%r14,%r1		# call lockdep_sys_exit
#endif
	.endm

	.macro	CHECK_STACK stacksize,savearea
#ifdef CONFIG_CHECK_STACK
	tml	%r15,\stacksize - CONFIG_STACK_GUARD
	la	%r14,\savearea
	jz	stack_overflow
#endif
	.endm

	.macro	SWITCH_ASYNC savearea,stack,shift
	tmh	%r8,0x0001		# interrupting from user ?
	jnz	1f
	lr	%r14,%r9
	sl	%r14,BASED(.Lcritical_start)
	cl	%r14,BASED(.Lcritical_length)
	jhe	0f
	la	%r11,\savearea		# inside critical section, do cleanup
	bras	%r14,cleanup_critical
	tmh	%r8,0x0001		# retest problem state after cleanup
	jnz	1f
0:	l	%r14,\stack		# are we already on the target stack?
	slr	%r14,%r15
	sra	%r14,\shift
	jnz	1f
	CHECK_STACK 1<<\shift,\savearea
	j	2f
1:	l	%r15,\stack		# load target stack
2:	ahi	%r15,-(STACK_FRAME_OVERHEAD + __PT_SIZE)
	la	%r11,STACK_FRAME_OVERHEAD(%r15)
	.endm

	.macro	ADD64 high,low,timer
	al	\high,\timer
	al	\low,4+\timer
	brc	12,.+8
	ahi	\high,1
	.endm

	.macro	SUB64 high,low,timer
	sl	\high,\timer
	sl	\low,4+\timer
	brc	3,.+8
	ahi	\high,-1
	.endm

	.macro	UPDATE_VTIME high,low,enter_timer
	lm	\high,\low,__LC_EXIT_TIMER
	SUB64	\high,\low,\enter_timer
	ADD64	\high,\low,__LC_USER_TIMER
	stm	\high,\low,__LC_USER_TIMER
	lm	\high,\low,__LC_LAST_UPDATE_TIMER
	SUB64	\high,\low,__LC_EXIT_TIMER
	ADD64	\high,\low,__LC_SYSTEM_TIMER
	stm	\high,\low,__LC_SYSTEM_TIMER
	mvc	__LC_LAST_UPDATE_TIMER(8),\enter_timer
	.endm

	.macro REENABLE_IRQS
	st	%r8,__LC_RETURN_PSW
	ni	__LC_RETURN_PSW,0xbf
	ssm	__LC_RETURN_PSW
>>>>>>> refs/remotes/origin/cm-10.0
	.endm

	.section .kprobes.text, "ax"

/*
 * Scheduler resume function, called by switch_to
 *  gpr2 = (task_struct *) prev
 *  gpr3 = (task_struct *) next
 * Returns:
 *  gpr2 = prev
 */
<<<<<<< HEAD
	.globl	__switch_to
__switch_to:
	basr	%r1,0
0:	l	%r4,__THREAD_info(%r2)		# get thread_info of prev
	l	%r5,__THREAD_info(%r3)		# get thread_info of next
	tm	__TI_flags+3(%r4),_TIF_MCCK_PENDING # machine check pending?
	bz	1f-0b(%r1)
	ni	__TI_flags+3(%r4),255-_TIF_MCCK_PENDING	# clear flag in prev
	oi	__TI_flags+3(%r5),_TIF_MCCK_PENDING	# set it in next
1:	stm	%r6,%r15,__SF_GPRS(%r15)	# store gprs of prev task
=======
ENTRY(__switch_to)
	l	%r4,__THREAD_info(%r2)		# get thread_info of prev
	l	%r5,__THREAD_info(%r3)		# get thread_info of next
	tm	__TI_flags+3(%r4),_TIF_MCCK_PENDING # machine check pending?
	jz	0f
	ni	__TI_flags+3(%r4),255-_TIF_MCCK_PENDING	# clear flag in prev
	oi	__TI_flags+3(%r5),_TIF_MCCK_PENDING	# set it in next
0:	stm	%r6,%r15,__SF_GPRS(%r15)	# store gprs of prev task
>>>>>>> refs/remotes/origin/cm-10.0
	st	%r15,__THREAD_ksp(%r2)		# store kernel stack of prev
	l	%r15,__THREAD_ksp(%r3)		# load kernel stack of next
	lctl	%c4,%c4,__TASK_pid(%r3)		# load pid to control reg. 4
	lm	%r6,%r15,__SF_GPRS(%r15)	# load gprs of next task
	st	%r3,__LC_CURRENT		# store task struct of next
	mvc	__LC_CURRENT_PID(4,%r0),__TASK_pid(%r3)	# store pid of next
	st	%r5,__LC_THREAD_INFO		# store thread info of next
	ahi	%r5,STACK_SIZE			# end of kernel stack of next
	st	%r5,__LC_KERNEL_STACK		# store end of kernel stack
	br	%r14

__critical_start:
/*
 * SVC interrupt handler routine. System calls are synchronous events and
 * are executed with interrupts enabled.
 */

<<<<<<< HEAD
	.globl	system_call
system_call:
	stpt	__LC_SYNC_ENTER_TIMER
sysc_saveall:
	SAVE_ALL_SVC __LC_SVC_OLD_PSW,__LC_SAVE_AREA
	CREATE_STACK_FRAME __LC_SAVE_AREA
	mvc	SP_PSW(8,%r15),__LC_SVC_OLD_PSW
	mvc	SP_ILC(4,%r15),__LC_SVC_ILC
	l	%r12,__LC_THREAD_INFO	# load pointer to thread_info struct
sysc_vtime:
	UPDATE_VTIME __LC_EXIT_TIMER,__LC_SYNC_ENTER_TIMER,__LC_USER_TIMER
sysc_stime:
	UPDATE_VTIME __LC_LAST_UPDATE_TIMER,__LC_EXIT_TIMER,__LC_SYSTEM_TIMER
sysc_update:
	mvc	__LC_LAST_UPDATE_TIMER(8),__LC_SYNC_ENTER_TIMER
sysc_do_svc:
	xr	%r7,%r7
	icm	%r7,3,SP_SVCNR(%r15)	# load svc number and test for svc 0
	bnz	BASED(sysc_nr_ok)	# svc number > 0
	# svc 0: system call number in %r1
	cl	%r1,BASED(.Lnr_syscalls)
	bnl	BASED(sysc_nr_ok)
	sth	%r1,SP_SVCNR(%r15)
	lr	%r7,%r1 	  # copy svc number to %r7
sysc_nr_ok:
	sll	%r7,2		  # svc number *4
	l	%r10,BASED(.Lsysc_table)
	tm	__TI_flags+2(%r12),_TIF_SYSCALL
	mvc	SP_ARGS(4,%r15),SP_R7(%r15)
	l	%r8,0(%r7,%r10)	  # get system call addr.
	bnz	BASED(sysc_tracesys)
	basr	%r14,%r8	  # call sys_xxxx
	st	%r2,SP_R2(%r15)   # store return value (change R2 on stack)
=======
ENTRY(system_call)
	stpt	__LC_SYNC_ENTER_TIMER
sysc_stm:
	stm	%r8,%r15,__LC_SAVE_AREA_SYNC
	l	%r12,__LC_THREAD_INFO
	l	%r13,__LC_SVC_NEW_PSW+4
sysc_per:
	l	%r15,__LC_KERNEL_STACK
	ahi	%r15,-(STACK_FRAME_OVERHEAD + __PT_SIZE)
	la	%r11,STACK_FRAME_OVERHEAD(%r15)	# pointer to pt_regs
sysc_vtime:
	UPDATE_VTIME %r8,%r9,__LC_SYNC_ENTER_TIMER
	stm	%r0,%r7,__PT_R0(%r11)
	mvc	__PT_R8(32,%r11),__LC_SAVE_AREA_SYNC
	mvc	__PT_PSW(8,%r11),__LC_SVC_OLD_PSW
	mvc	__PT_INT_CODE(4,%r11),__LC_SVC_ILC
sysc_do_svc:
	oi	__TI_flags+3(%r12),_TIF_SYSCALL
	lh	%r8,__PT_INT_CODE+2(%r11)
	sla	%r8,2				# shift and test for svc0
	jnz	sysc_nr_ok
	# svc 0: system call number in %r1
	cl	%r1,BASED(.Lnr_syscalls)
	jnl	sysc_nr_ok
	sth	%r1,__PT_INT_CODE+2(%r11)
	lr	%r8,%r1
	sla	%r8,2
sysc_nr_ok:
	l	%r10,BASED(.Lsys_call_table)	# 31 bit system call table
	xc	__SF_BACKCHAIN(4,%r15),__SF_BACKCHAIN(%r15)
	st	%r2,__PT_ORIG_GPR2(%r11)
	st	%r7,STACK_FRAME_OVERHEAD(%r15)
	l	%r9,0(%r8,%r10)			# get system call addr.
	tm	__TI_flags+2(%r12),_TIF_TRACE >> 8
	jnz	sysc_tracesys
	basr	%r14,%r9			# call sys_xxxx
	st	%r2,__PT_R2(%r11)		# store return value
>>>>>>> refs/remotes/origin/cm-10.0

sysc_return:
	LOCKDEP_SYS_EXIT
sysc_tif:
<<<<<<< HEAD
	tm	__TI_flags+3(%r12),_TIF_WORK_SVC
	bnz	BASED(sysc_work)  # there is work to do (signals etc.)
sysc_restore:
	RESTORE_ALL __LC_RETURN_PSW,1
sysc_done:

#
# There is work to do, but first we need to check if we return to userspace.
#
sysc_work:
	tm	SP_PSW+1(%r15),0x01	# returning to user ?
	bno	BASED(sysc_restore)

#
# One of the work bits is on. Find out which one.
#
sysc_work_tif:
	tm	__TI_flags+3(%r12),_TIF_MCCK_PENDING
	bo	BASED(sysc_mcck_pending)
	tm	__TI_flags+3(%r12),_TIF_NEED_RESCHED
	bo	BASED(sysc_reschedule)
	tm	__TI_flags+3(%r12),_TIF_SIGPENDING
	bo	BASED(sysc_sigpending)
	tm	__TI_flags+3(%r12),_TIF_NOTIFY_RESUME
	bo	BASED(sysc_notify_resume)
	tm	__TI_flags+3(%r12),_TIF_RESTART_SVC
	bo	BASED(sysc_restart)
	tm	__TI_flags+3(%r12),_TIF_PER_TRAP
	bo	BASED(sysc_singlestep)
	b	BASED(sysc_return)	# beware of critical section cleanup
=======
	tm	__PT_PSW+1(%r11),0x01		# returning to user ?
	jno	sysc_restore
	tm	__TI_flags+3(%r12),_TIF_WORK_SVC
	jnz	sysc_work			# check for work
	ni	__TI_flags+3(%r12),255-_TIF_SYSCALL
sysc_restore:
	mvc	__LC_RETURN_PSW(8),__PT_PSW(%r11)
	stpt	__LC_EXIT_TIMER
	lm	%r0,%r15,__PT_R0(%r11)
	lpsw	__LC_RETURN_PSW
sysc_done:

#
# One of the work bits is on. Find out which one.
#
sysc_work:
	tm	__TI_flags+3(%r12),_TIF_MCCK_PENDING
	jo	sysc_mcck_pending
	tm	__TI_flags+3(%r12),_TIF_NEED_RESCHED
	jo	sysc_reschedule
	tm	__TI_flags+3(%r12),_TIF_SIGPENDING
	jo	sysc_sigpending
	tm	__TI_flags+3(%r12),_TIF_NOTIFY_RESUME
	jo	sysc_notify_resume
	tm	__TI_flags+3(%r12),_TIF_PER_TRAP
	jo	sysc_singlestep
	j	sysc_return		# beware of critical section cleanup
>>>>>>> refs/remotes/origin/cm-10.0

#
# _TIF_NEED_RESCHED is set, call schedule
#
sysc_reschedule:
	l	%r1,BASED(.Lschedule)
	la	%r14,BASED(sysc_return)
<<<<<<< HEAD
	br	%r1			# call scheduler
=======
	br	%r1			# call schedule
>>>>>>> refs/remotes/origin/cm-10.0

#
# _TIF_MCCK_PENDING is set, call handler
#
sysc_mcck_pending:
<<<<<<< HEAD
	l	%r1,BASED(.Ls390_handle_mcck)
=======
	l	%r1,BASED(.Lhandle_mcck)
>>>>>>> refs/remotes/origin/cm-10.0
	la	%r14,BASED(sysc_return)
	br	%r1			# TIF bit will be cleared by handler

#
# _TIF_SIGPENDING is set, call do_signal
#
sysc_sigpending:
	ni	__TI_flags+3(%r12),255-_TIF_PER_TRAP # clear TIF_PER_TRAP
<<<<<<< HEAD
	la	%r2,SP_PTREGS(%r15)	# load pt_regs
	l	%r1,BASED(.Ldo_signal)
	basr	%r14,%r1		# call do_signal
	tm	__TI_flags+3(%r12),_TIF_RESTART_SVC
	bo	BASED(sysc_restart)
	tm	__TI_flags+3(%r12),_TIF_PER_TRAP
	bo	BASED(sysc_singlestep)
	b	BASED(sysc_return)
=======
	lr	%r2,%r11		# pass pointer to pt_regs
	l	%r1,BASED(.Ldo_signal)
	basr	%r14,%r1		# call do_signal
	tm	__TI_flags+3(%r12),_TIF_SYSCALL
	jno	sysc_return
	lm	%r2,%r7,__PT_R2(%r11)	# load svc arguments
	xr	%r8,%r8			# svc 0 returns -ENOSYS
	clc	__PT_INT_CODE+2(2,%r11),BASED(.Lnr_syscalls+2)
	jnl	sysc_nr_ok		# invalid svc number -> do svc 0
	lh	%r8,__PT_INT_CODE+2(%r11)	# load new svc number
	sla	%r8,2
	j	sysc_nr_ok		# restart svc
>>>>>>> refs/remotes/origin/cm-10.0

#
# _TIF_NOTIFY_RESUME is set, call do_notify_resume
#
sysc_notify_resume:
<<<<<<< HEAD
	la	%r2,SP_PTREGS(%r15)	# load pt_regs
=======
	lr	%r2,%r11		# pass pointer to pt_regs
>>>>>>> refs/remotes/origin/cm-10.0
	l	%r1,BASED(.Ldo_notify_resume)
	la	%r14,BASED(sysc_return)
	br	%r1			# call do_notify_resume

<<<<<<< HEAD

#
# _TIF_RESTART_SVC is set, set up registers and restart svc
#
sysc_restart:
	ni	__TI_flags+3(%r12),255-_TIF_RESTART_SVC # clear TIF_RESTART_SVC
	l	%r7,SP_R2(%r15) 	# load new svc number
	mvc	SP_R2(4,%r15),SP_ORIG_R2(%r15) # restore first argument
	lm	%r2,%r6,SP_R2(%r15)	# load svc arguments
	sth	%r7,SP_SVCNR(%r15)
	b	BASED(sysc_nr_ok)	# restart svc

=======
>>>>>>> refs/remotes/origin/cm-10.0
#
# _TIF_PER_TRAP is set, call do_per_trap
#
sysc_singlestep:
<<<<<<< HEAD
	ni	__TI_flags+3(%r12),255-_TIF_PER_TRAP # clear TIF_PER_TRAP
	xc	SP_SVCNR(2,%r15),SP_SVCNR(%r15)		# clear svc number
	la	%r2,SP_PTREGS(%r15)	# address of register-save area
	l	%r1,BASED(.Lhandle_per)	# load adr. of per handler
	la	%r14,BASED(sysc_return)	# load adr. of system return
	br	%r1			# branch to do_per_trap
=======
	ni	__TI_flags+3(%r12),255-(_TIF_SYSCALL | _TIF_PER_TRAP)
	lr	%r2,%r11		# pass pointer to pt_regs
	l	%r1,BASED(.Ldo_per_trap)
	la	%r14,BASED(sysc_return)
	br	%r1			# call do_per_trap
>>>>>>> refs/remotes/origin/cm-10.0

#
# call tracehook_report_syscall_entry/tracehook_report_syscall_exit before
# and after the system call
#
sysc_tracesys:
<<<<<<< HEAD
	l	%r1,BASED(.Ltrace_entry)
	la	%r2,SP_PTREGS(%r15)	# load pt_regs
	la	%r3,0
	xr	%r0,%r0
	icm	%r0,3,SP_SVCNR(%r15)
	st	%r0,SP_R2(%r15)
	basr	%r14,%r1
	cl	%r2,BASED(.Lnr_syscalls)
	bnl	BASED(sysc_tracenogo)
	lr	%r7,%r2
	sll	%r7,2			# svc number *4
	l	%r8,0(%r7,%r10)
sysc_tracego:
	lm	%r3,%r6,SP_R3(%r15)
	mvc	SP_ARGS(4,%r15),SP_R7(%r15)
	l	%r2,SP_ORIG_R2(%r15)
	basr	%r14,%r8		# call sys_xxx
	st	%r2,SP_R2(%r15)		# store return value
sysc_tracenogo:
	tm	__TI_flags+2(%r12),_TIF_SYSCALL
	bz	BASED(sysc_return)
	l	%r1,BASED(.Ltrace_exit)
	la	%r2,SP_PTREGS(%r15)	# load pt_regs
	la	%r14,BASED(sysc_return)
	br	%r1
=======
	l	%r1,BASED(.Ltrace_enter)
	lr	%r2,%r11		# pass pointer to pt_regs
	la	%r3,0
	xr	%r0,%r0
	icm	%r0,3,__PT_INT_CODE+2(%r11)
	st	%r0,__PT_R2(%r11)
	basr	%r14,%r1		# call do_syscall_trace_enter
	cl	%r2,BASED(.Lnr_syscalls)
	jnl	sysc_tracenogo
	lr	%r8,%r2
	sll	%r8,2
	l	%r9,0(%r8,%r10)
sysc_tracego:
	lm	%r3,%r7,__PT_R3(%r11)
	st	%r7,STACK_FRAME_OVERHEAD(%r15)
	l	%r2,__PT_ORIG_GPR2(%r11)
	basr	%r14,%r9		# call sys_xxx
	st	%r2,__PT_R2(%r11)	# store return value
sysc_tracenogo:
	tm	__TI_flags+2(%r12),_TIF_TRACE >> 8
	jz	sysc_return
	l	%r1,BASED(.Ltrace_exit)
	lr	%r2,%r11		# pass pointer to pt_regs
	la	%r14,BASED(sysc_return)
	br	%r1			# call do_syscall_trace_exit
>>>>>>> refs/remotes/origin/cm-10.0

#
# a new process exits the kernel with ret_from_fork
#
<<<<<<< HEAD
	.globl	ret_from_fork
ret_from_fork:
	l	%r13,__LC_SVC_NEW_PSW+4
	l	%r12,__LC_THREAD_INFO	# load pointer to thread_info struct
	tm	SP_PSW+1(%r15),0x01	# forking a kernel thread ?
	bo	BASED(0f)
	st	%r15,SP_R15(%r15)	# store stack pointer for new kthread
0:	l	%r1,BASED(.Lschedtail)
	basr	%r14,%r1
	TRACE_IRQS_ON
	stosm	__SF_EMPTY(%r15),0x03	# reenable interrupts
	b	BASED(sysc_tracenogo)
=======
ENTRY(ret_from_fork)
	la	%r11,STACK_FRAME_OVERHEAD(%r15)
	l	%r12,__LC_THREAD_INFO
	l	%r13,__LC_SVC_NEW_PSW+4
	tm	__PT_PSW+1(%r11),0x01	# forking a kernel thread ?
	jo	0f
	st	%r15,__PT_R15(%r11)	# store stack pointer for new kthread
0:	l	%r1,BASED(.Lschedule_tail)
	basr	%r14,%r1		# call schedule_tail
	TRACE_IRQS_ON
	ssm	__LC_SVC_NEW_PSW	# reenable interrupts
	j	sysc_tracenogo
>>>>>>> refs/remotes/origin/cm-10.0

#
# kernel_execve function needs to deal with pt_regs that is not
# at the usual place
#
<<<<<<< HEAD
	.globl	kernel_execve
kernel_execve:
	stm	%r12,%r15,48(%r15)
	lr	%r14,%r15
	l	%r13,__LC_SVC_NEW_PSW+4
	s	%r15,BASED(.Lc_spsize)
	st	%r14,__SF_BACKCHAIN(%r15)
	la	%r12,SP_PTREGS(%r15)
	xc	0(__PT_SIZE,%r12),0(%r12)
	l	%r1,BASED(.Ldo_execve)
	lr	%r5,%r12
	basr	%r14,%r1
	ltr	%r2,%r2
	be	BASED(0f)
	a	%r15,BASED(.Lc_spsize)
	lm	%r12,%r15,48(%r15)
	br	%r14
	# execve succeeded.
0:	stnsm	__SF_EMPTY(%r15),0xfc	# disable interrupts
	l	%r15,__LC_KERNEL_STACK	# load ksp
	s	%r15,BASED(.Lc_spsize)	# make room for registers & psw
	mvc	SP_PTREGS(__PT_SIZE,%r15),0(%r12)	# copy pt_regs
	l	%r12,__LC_THREAD_INFO
	xc	__SF_BACKCHAIN(4,%r15),__SF_BACKCHAIN(%r15)
	stosm	__SF_EMPTY(%r15),0x03	# reenable interrupts
	l	%r1,BASED(.Lexecve_tail)
	basr	%r14,%r1
	b	BASED(sysc_return)
=======
ENTRY(kernel_execve)
	stm	%r12,%r15,48(%r15)
	lr	%r14,%r15
	l	%r13,__LC_SVC_NEW_PSW+4
	ahi	%r15,-(STACK_FRAME_OVERHEAD + __PT_SIZE)
	st	%r14,__SF_BACKCHAIN(%r15)
	la	%r12,STACK_FRAME_OVERHEAD(%r15)
	xc	0(__PT_SIZE,%r12),0(%r12)
	l	%r1,BASED(.Ldo_execve)
	lr	%r5,%r12
	basr	%r14,%r1		# call do_execve
	ltr	%r2,%r2
	je	0f
	ahi	%r15,(STACK_FRAME_OVERHEAD + __PT_SIZE)
	lm	%r12,%r15,48(%r15)
	br	%r14
	# execve succeeded.
0:	ssm	__LC_PGM_NEW_PSW	# disable I/O and ext. interrupts
	l	%r15,__LC_KERNEL_STACK	# load ksp
	ahi	%r15,-(STACK_FRAME_OVERHEAD + __PT_SIZE)
	la	%r11,STACK_FRAME_OVERHEAD(%r15)
	mvc	0(__PT_SIZE,%r11),0(%r12)	# copy pt_regs
	l	%r12,__LC_THREAD_INFO
	xc	__SF_BACKCHAIN(4,%r15),__SF_BACKCHAIN(%r15)
	ssm	__LC_SVC_NEW_PSW	# reenable interrupts
	l	%r1,BASED(.Lexecve_tail)
	basr	%r14,%r1		# call execve_tail
	j	sysc_return
>>>>>>> refs/remotes/origin/cm-10.0

/*
 * Program check handler routine
 */

<<<<<<< HEAD
	.globl	pgm_check_handler
pgm_check_handler:
/*
 * First we need to check for a special case:
 * Single stepping an instruction that disables the PER event mask will
 * cause a PER event AFTER the mask has been set. Example: SVC or LPSW.
 * For a single stepped SVC the program check handler gets control after
 * the SVC new PSW has been loaded. But we want to execute the SVC first and
 * then handle the PER event. Therefore we update the SVC old PSW to point
 * to the pgm_check_handler and branch to the SVC handler after we checked
 * if we have to load the kernel stack register.
 * For every other possible cause for PER event without the PER mask set
 * we just ignore the PER event (FIXME: is there anything we have to do
 * for LPSW?).
 */
	stpt	__LC_SYNC_ENTER_TIMER
	SAVE_ALL_BASE __LC_SAVE_AREA
	tm	__LC_PGM_INT_CODE+1,0x80 # check whether we got a per exception
	bnz	BASED(pgm_per)		# got per exception -> special case
	SAVE_ALL_PGM __LC_PGM_OLD_PSW,__LC_SAVE_AREA
	CREATE_STACK_FRAME __LC_SAVE_AREA
	xc	SP_ILC(4,%r15),SP_ILC(%r15)
	mvc	SP_PSW(8,%r15),__LC_PGM_OLD_PSW
	l	%r12,__LC_THREAD_INFO	# load pointer to thread_info struct
	tm	SP_PSW+1(%r15),0x01	# interrupting from user ?
	bz	BASED(pgm_no_vtime)
	UPDATE_VTIME __LC_EXIT_TIMER,__LC_SYNC_ENTER_TIMER,__LC_USER_TIMER
	UPDATE_VTIME __LC_LAST_UPDATE_TIMER,__LC_EXIT_TIMER,__LC_SYSTEM_TIMER
	mvc	__LC_LAST_UPDATE_TIMER(8),__LC_SYNC_ENTER_TIMER
pgm_no_vtime:
	l	%r3,__LC_PGM_ILC	# load program interruption code
	l	%r4,__LC_TRANS_EXC_CODE
	REENABLE_IRQS
	la	%r8,0x7f
	nr	%r8,%r3
	sll	%r8,2
	l	%r1,BASED(.Ljump_table)
	l	%r1,0(%r8,%r1)		# load address of handler routine
	la	%r2,SP_PTREGS(%r15)	# address of register-save area
	basr	%r14,%r1		# branch to interrupt-handler
pgm_exit:
	b	BASED(sysc_return)

#
# handle per exception
#
pgm_per:
	tm	__LC_PGM_OLD_PSW,0x40	# test if per event recording is on
	bnz	BASED(pgm_per_std)	# ok, normal per event from user space
# ok its one of the special cases, now we need to find out which one
	clc	__LC_PGM_OLD_PSW(8),__LC_SVC_NEW_PSW
	be	BASED(pgm_svcper)
# no interesting special case, ignore PER event
	lm	%r12,%r15,__LC_SAVE_AREA
	lpsw	0x28

#
# Normal per exception
#
pgm_per_std:
	SAVE_ALL_PGM __LC_PGM_OLD_PSW,__LC_SAVE_AREA
	CREATE_STACK_FRAME __LC_SAVE_AREA
	mvc	SP_PSW(8,%r15),__LC_PGM_OLD_PSW
	l	%r12,__LC_THREAD_INFO	# load pointer to thread_info struct
	tm	SP_PSW+1(%r15),0x01	# interrupting from user ?
	bz	BASED(pgm_no_vtime2)
	UPDATE_VTIME __LC_EXIT_TIMER,__LC_SYNC_ENTER_TIMER,__LC_USER_TIMER
	UPDATE_VTIME __LC_LAST_UPDATE_TIMER,__LC_EXIT_TIMER,__LC_SYSTEM_TIMER
	mvc	__LC_LAST_UPDATE_TIMER(8),__LC_SYNC_ENTER_TIMER
pgm_no_vtime2:
	l	%r1,__TI_task(%r12)
	tm	SP_PSW+1(%r15),0x01	# kernel per event ?
	bz	BASED(kernel_per)
	mvc	__THREAD_per_cause(2,%r1),__LC_PER_CAUSE
	mvc	__THREAD_per_address(4,%r1),__LC_PER_ADDRESS
	mvc	__THREAD_per_paid(1,%r1),__LC_PER_PAID
	oi	__TI_flags+3(%r12),_TIF_PER_TRAP # set TIF_PER_TRAP
	l	%r3,__LC_PGM_ILC	# load program interruption code
	l	%r4,__LC_TRANS_EXC_CODE
	REENABLE_IRQS
	la	%r8,0x7f
	nr	%r8,%r3 		# clear per-event-bit and ilc
	be	BASED(pgm_exit2)	# only per or per+check ?
	sll	%r8,2
	l	%r1,BASED(.Ljump_table)
	l	%r1,0(%r8,%r1)		# load address of handler routine
	la	%r2,SP_PTREGS(%r15)	# address of register-save area
	basr	%r14,%r1		# branch to interrupt-handler
pgm_exit2:
	b	BASED(sysc_return)

#
# it was a single stepped SVC that is causing all the trouble
#
pgm_svcper:
	SAVE_ALL_PGM __LC_SVC_OLD_PSW,__LC_SAVE_AREA
	CREATE_STACK_FRAME __LC_SAVE_AREA
	mvc	SP_PSW(8,%r15),__LC_SVC_OLD_PSW
	mvc	SP_ILC(4,%r15),__LC_SVC_ILC
	l	%r12,__LC_THREAD_INFO	# load pointer to thread_info struct
	UPDATE_VTIME __LC_EXIT_TIMER,__LC_SYNC_ENTER_TIMER,__LC_USER_TIMER
	UPDATE_VTIME __LC_LAST_UPDATE_TIMER,__LC_EXIT_TIMER,__LC_SYSTEM_TIMER
	mvc	__LC_LAST_UPDATE_TIMER(8),__LC_SYNC_ENTER_TIMER
	l	%r8,__TI_task(%r12)
	mvc	__THREAD_per_cause(2,%r8),__LC_PER_CAUSE
	mvc	__THREAD_per_address(4,%r8),__LC_PER_ADDRESS
	mvc	__THREAD_per_paid(1,%r8),__LC_PER_PAID
	oi	__TI_flags+3(%r12),_TIF_PER_TRAP # set TIF_PER_TRAP
	stosm	__SF_EMPTY(%r15),0x03	# reenable interrupts
	lm	%r2,%r6,SP_R2(%r15)	# load svc arguments
	b	BASED(sysc_do_svc)

#
# per was called from kernel, must be kprobes
#
kernel_per:
	REENABLE_IRQS
	xc	SP_SVCNR(2,%r15),SP_SVCNR(%r15)
	la	%r2,SP_PTREGS(%r15)	# address of register-save area
	l	%r1,BASED(.Lhandle_per)	# load adr. of per handler
	basr	%r14,%r1		# branch to do_single_step
	b	BASED(pgm_exit)
=======
ENTRY(pgm_check_handler)
	stpt	__LC_SYNC_ENTER_TIMER
	stm	%r8,%r15,__LC_SAVE_AREA_SYNC
	l	%r12,__LC_THREAD_INFO
	l	%r13,__LC_SVC_NEW_PSW+4
	lm	%r8,%r9,__LC_PGM_OLD_PSW
	tmh	%r8,0x0001		# test problem state bit
	jnz	1f			# -> fault in user space
	tmh	%r8,0x4000		# PER bit set in old PSW ?
	jnz	0f			# -> enabled, can't be a double fault
	tm	__LC_PGM_ILC+3,0x80	# check for per exception
	jnz	pgm_svcper		# -> single stepped svc
0:	CHECK_STACK STACK_SIZE,__LC_SAVE_AREA_SYNC
	j	2f
1:	UPDATE_VTIME %r14,%r15,__LC_SYNC_ENTER_TIMER
	l	%r15,__LC_KERNEL_STACK
2:	ahi	%r15,-(STACK_FRAME_OVERHEAD + __PT_SIZE)
	la	%r11,STACK_FRAME_OVERHEAD(%r15)
	stm	%r0,%r7,__PT_R0(%r11)
	mvc	__PT_R8(32,%r11),__LC_SAVE_AREA_SYNC
	stm	%r8,%r9,__PT_PSW(%r11)
	mvc	__PT_INT_CODE(4,%r11),__LC_PGM_ILC
	mvc	__PT_INT_PARM_LONG(4,%r11),__LC_TRANS_EXC_CODE
	tm	__LC_PGM_ILC+3,0x80	# check for per exception
	jz	0f
	l	%r1,__TI_task(%r12)
	tmh	%r8,0x0001		# kernel per event ?
	jz	pgm_kprobe
	oi	__TI_flags+3(%r12),_TIF_PER_TRAP
	mvc	__THREAD_per_address(4,%r1),__LC_PER_ADDRESS
	mvc	__THREAD_per_cause(2,%r1),__LC_PER_CAUSE
	mvc	__THREAD_per_paid(1,%r1),__LC_PER_PAID
0:	REENABLE_IRQS
	xc	__SF_BACKCHAIN(4,%r15),__SF_BACKCHAIN(%r15)
	l	%r1,BASED(.Ljump_table)
	la	%r10,0x7f
	n	%r10,__PT_INT_CODE(%r11)
	je	sysc_return
	sll	%r10,2
	l	%r1,0(%r10,%r1)		# load address of handler routine
	lr	%r2,%r11		# pass pointer to pt_regs
	basr	%r14,%r1		# branch to interrupt-handler
	j	sysc_return

#
# PER event in supervisor state, must be kprobes
#
pgm_kprobe:
	REENABLE_IRQS
	xc	__SF_BACKCHAIN(4,%r15),__SF_BACKCHAIN(%r15)
	l	%r1,BASED(.Ldo_per_trap)
	lr	%r2,%r11		# pass pointer to pt_regs
	basr	%r14,%r1		# call do_per_trap
	j	sysc_return

#
# single stepped system call
#
pgm_svcper:
	oi	__TI_flags+3(%r12),_TIF_PER_TRAP
	mvc	__LC_RETURN_PSW(4),__LC_SVC_NEW_PSW
	mvc	__LC_RETURN_PSW+4(4),BASED(.Lsysc_per)
	lpsw	__LC_RETURN_PSW		# branch to sysc_per and enable irqs
>>>>>>> refs/remotes/origin/cm-10.0

/*
 * IO interrupt handler routine
 */

<<<<<<< HEAD
	.globl io_int_handler
io_int_handler:
	stck	__LC_INT_CLOCK
	stpt	__LC_ASYNC_ENTER_TIMER
	SAVE_ALL_ASYNC __LC_IO_OLD_PSW,__LC_SAVE_AREA+16
	CREATE_STACK_FRAME __LC_SAVE_AREA+16
	mvc	SP_PSW(8,%r15),0(%r12)	# move user PSW to stack
	l	%r12,__LC_THREAD_INFO	# load pointer to thread_info struct
	tm	SP_PSW+1(%r15),0x01	# interrupting from user ?
	bz	BASED(io_no_vtime)
	UPDATE_VTIME __LC_EXIT_TIMER,__LC_ASYNC_ENTER_TIMER,__LC_USER_TIMER
	UPDATE_VTIME __LC_LAST_UPDATE_TIMER,__LC_EXIT_TIMER,__LC_SYSTEM_TIMER
	mvc	__LC_LAST_UPDATE_TIMER(8),__LC_ASYNC_ENTER_TIMER
io_no_vtime:
	TRACE_IRQS_OFF
	l	%r1,BASED(.Ldo_IRQ)	# load address of do_IRQ
	la	%r2,SP_PTREGS(%r15)	# address of register-save area
	basr	%r14,%r1		# branch to standard irq handler
=======
ENTRY(io_int_handler)
	stck	__LC_INT_CLOCK
	stpt	__LC_ASYNC_ENTER_TIMER
	stm	%r8,%r15,__LC_SAVE_AREA_ASYNC
	l	%r12,__LC_THREAD_INFO
	l	%r13,__LC_SVC_NEW_PSW+4
	lm	%r8,%r9,__LC_IO_OLD_PSW
	tmh	%r8,0x0001		# interrupting from user ?
	jz	io_skip
	UPDATE_VTIME %r14,%r15,__LC_ASYNC_ENTER_TIMER
io_skip:
	SWITCH_ASYNC __LC_SAVE_AREA_ASYNC,__LC_ASYNC_STACK,STACK_SHIFT
	stm	%r0,%r7,__PT_R0(%r11)
	mvc	__PT_R8(32,%r11),__LC_SAVE_AREA_ASYNC
	stm	%r8,%r9,__PT_PSW(%r11)
	TRACE_IRQS_OFF
	xc	__SF_BACKCHAIN(4,%r15),__SF_BACKCHAIN(%r15)
	l	%r1,BASED(.Ldo_IRQ)
	lr	%r2,%r11		# pass pointer to pt_regs
	basr	%r14,%r1		# call do_IRQ
>>>>>>> refs/remotes/origin/cm-10.0
io_return:
	LOCKDEP_SYS_EXIT
	TRACE_IRQS_ON
io_tif:
	tm	__TI_flags+3(%r12),_TIF_WORK_INT
<<<<<<< HEAD
	bnz	BASED(io_work)		# there is work to do (signals etc.)
io_restore:
	RESTORE_ALL __LC_RETURN_PSW,0
=======
	jnz	io_work			# there is work to do (signals etc.)
io_restore:
	mvc	__LC_RETURN_PSW(8),__PT_PSW(%r11)
	stpt	__LC_EXIT_TIMER
	lm	%r0,%r15,__PT_R0(%r11)
	lpsw	__LC_RETURN_PSW
>>>>>>> refs/remotes/origin/cm-10.0
io_done:

#
# There is work todo, find out in which context we have been interrupted:
# 1) if we return to user space we can do all _TIF_WORK_INT work
# 2) if we return to kernel code and preemptive scheduling is enabled check
#    the preemption counter and if it is zero call preempt_schedule_irq
# Before any work can be done, a switch to the kernel stack is required.
#
io_work:
<<<<<<< HEAD
	tm	SP_PSW+1(%r15),0x01	# returning to user ?
	bo	BASED(io_work_user)	# yes -> do resched & signal
#ifdef CONFIG_PREEMPT
	# check for preemptive scheduling
	icm	%r0,15,__TI_precount(%r12)
	bnz	BASED(io_restore)	# preemption disabled
	tm	__TI_flags+3(%r12),_TIF_NEED_RESCHED
	bno	BASED(io_restore)
	# switch to kernel stack
	l	%r1,SP_R15(%r15)
	s	%r1,BASED(.Lc_spsize)
	mvc	SP_PTREGS(__PT_SIZE,%r1),SP_PTREGS(%r15)
	xc	__SF_BACKCHAIN(4,%r1),__SF_BACKCHAIN(%r1) # clear back chain
=======
	tm	__PT_PSW+1(%r11),0x01	# returning to user ?
	jo	io_work_user		# yes -> do resched & signal
#ifdef CONFIG_PREEMPT
	# check for preemptive scheduling
	icm	%r0,15,__TI_precount(%r12)
	jnz	io_restore		# preemption disabled
	tm	__TI_flags+3(%r12),_TIF_NEED_RESCHED
	jno	io_restore
	# switch to kernel stack
	l	%r1,__PT_R15(%r11)
	ahi	%r1,-(STACK_FRAME_OVERHEAD + __PT_SIZE)
	mvc	STACK_FRAME_OVERHEAD(__PT_SIZE,%r1),0(%r11)
	xc	__SF_BACKCHAIN(4,%r1),__SF_BACKCHAIN(%r1)
	la	%r11,STACK_FRAME_OVERHEAD(%r1)
>>>>>>> refs/remotes/origin/cm-10.0
	lr	%r15,%r1
	# TRACE_IRQS_ON already done at io_return, call
	# TRACE_IRQS_OFF to keep things symmetrical
	TRACE_IRQS_OFF
<<<<<<< HEAD
	l	%r1,BASED(.Lpreempt_schedule_irq)
	basr	%r14,%r1		# call preempt_schedule_irq
	b	BASED(io_return)
#else
	b	BASED(io_restore)
=======
	l	%r1,BASED(.Lpreempt_irq)
	basr	%r14,%r1		# call preempt_schedule_irq
	j	io_return
#else
	j	io_restore
>>>>>>> refs/remotes/origin/cm-10.0
#endif

#
# Need to do work before returning to userspace, switch to kernel stack
#
io_work_user:
	l	%r1,__LC_KERNEL_STACK
<<<<<<< HEAD
	s	%r1,BASED(.Lc_spsize)
	mvc	SP_PTREGS(__PT_SIZE,%r1),SP_PTREGS(%r15)
	xc	__SF_BACKCHAIN(4,%r1),__SF_BACKCHAIN(%r1) # clear back chain
=======
	ahi	%r1,-(STACK_FRAME_OVERHEAD + __PT_SIZE)
	mvc	STACK_FRAME_OVERHEAD(__PT_SIZE,%r1),0(%r11)
	xc	__SF_BACKCHAIN(4,%r1),__SF_BACKCHAIN(%r1)
	la	%r11,STACK_FRAME_OVERHEAD(%r1)
>>>>>>> refs/remotes/origin/cm-10.0
	lr	%r15,%r1

#
# One of the work bits is on. Find out which one.
# Checked are: _TIF_SIGPENDING, _TIF_NOTIFY_RESUME, _TIF_NEED_RESCHED
#		and _TIF_MCCK_PENDING
#
io_work_tif:
	tm	__TI_flags+3(%r12),_TIF_MCCK_PENDING
<<<<<<< HEAD
	bo	BASED(io_mcck_pending)
	tm	__TI_flags+3(%r12),_TIF_NEED_RESCHED
	bo	BASED(io_reschedule)
	tm	__TI_flags+3(%r12),_TIF_SIGPENDING
	bo	BASED(io_sigpending)
	tm	__TI_flags+3(%r12),_TIF_NOTIFY_RESUME
	bo	BASED(io_notify_resume)
	b	BASED(io_return)	# beware of critical section cleanup
=======
	jo	io_mcck_pending
	tm	__TI_flags+3(%r12),_TIF_NEED_RESCHED
	jo	io_reschedule
	tm	__TI_flags+3(%r12),_TIF_SIGPENDING
	jo	io_sigpending
	tm	__TI_flags+3(%r12),_TIF_NOTIFY_RESUME
	jo	io_notify_resume
	j	io_return		# beware of critical section cleanup
>>>>>>> refs/remotes/origin/cm-10.0

#
# _TIF_MCCK_PENDING is set, call handler
#
io_mcck_pending:
	# TRACE_IRQS_ON already done at io_return
<<<<<<< HEAD
	l	%r1,BASED(.Ls390_handle_mcck)
	basr	%r14,%r1		# TIF bit will be cleared by handler
	TRACE_IRQS_OFF
	b	BASED(io_return)
=======
	l	%r1,BASED(.Lhandle_mcck)
	basr	%r14,%r1		# TIF bit will be cleared by handler
	TRACE_IRQS_OFF
	j	io_return
>>>>>>> refs/remotes/origin/cm-10.0

#
# _TIF_NEED_RESCHED is set, call schedule
#
io_reschedule:
	# TRACE_IRQS_ON already done at io_return
	l	%r1,BASED(.Lschedule)
<<<<<<< HEAD
	stosm	__SF_EMPTY(%r15),0x03	# reenable interrupts
	basr	%r14,%r1		# call scheduler
	stnsm	__SF_EMPTY(%r15),0xfc	# disable I/O and ext. interrupts
	TRACE_IRQS_OFF
	b	BASED(io_return)
=======
	ssm	__LC_SVC_NEW_PSW	# reenable interrupts
	basr	%r14,%r1		# call scheduler
	ssm	__LC_PGM_NEW_PSW	# disable I/O and ext. interrupts
	TRACE_IRQS_OFF
	j	io_return
>>>>>>> refs/remotes/origin/cm-10.0

#
# _TIF_SIGPENDING is set, call do_signal
#
io_sigpending:
	# TRACE_IRQS_ON already done at io_return
<<<<<<< HEAD
	stosm	__SF_EMPTY(%r15),0x03	# reenable interrupts
	la	%r2,SP_PTREGS(%r15)	# load pt_regs
	l	%r1,BASED(.Ldo_signal)
	basr	%r14,%r1		# call do_signal
	stnsm	__SF_EMPTY(%r15),0xfc	# disable I/O and ext. interrupts
	TRACE_IRQS_OFF
	b	BASED(io_return)
=======
	l	%r1,BASED(.Ldo_signal)
	ssm	__LC_SVC_NEW_PSW	# reenable interrupts
	lr	%r2,%r11		# pass pointer to pt_regs
	basr	%r14,%r1		# call do_signal
	ssm	__LC_PGM_NEW_PSW	# disable I/O and ext. interrupts
	TRACE_IRQS_OFF
	j	io_return
>>>>>>> refs/remotes/origin/cm-10.0

#
# _TIF_SIGPENDING is set, call do_signal
#
io_notify_resume:
	# TRACE_IRQS_ON already done at io_return
<<<<<<< HEAD
	stosm	__SF_EMPTY(%r15),0x03	# reenable interrupts
	la	%r2,SP_PTREGS(%r15)	# load pt_regs
	l	%r1,BASED(.Ldo_notify_resume)
	basr	%r14,%r1		# call do_signal
	stnsm	__SF_EMPTY(%r15),0xfc	# disable I/O and ext. interrupts
	TRACE_IRQS_OFF
	b	BASED(io_return)
=======
	l	%r1,BASED(.Ldo_notify_resume)
	ssm	__LC_SVC_NEW_PSW	# reenable interrupts
	lr	%r2,%r11		# pass pointer to pt_regs
	basr	%r14,%r1		# call do_notify_resume
	ssm	__LC_PGM_NEW_PSW	# disable I/O and ext. interrupts
	TRACE_IRQS_OFF
	j	io_return
>>>>>>> refs/remotes/origin/cm-10.0

/*
 * External interrupt handler routine
 */

<<<<<<< HEAD
	.globl	ext_int_handler
ext_int_handler:
	stck	__LC_INT_CLOCK
	stpt	__LC_ASYNC_ENTER_TIMER
	SAVE_ALL_ASYNC __LC_EXT_OLD_PSW,__LC_SAVE_AREA+16
	CREATE_STACK_FRAME __LC_SAVE_AREA+16
	mvc	SP_PSW(8,%r15),0(%r12)	# move user PSW to stack
	l	%r12,__LC_THREAD_INFO	# load pointer to thread_info struct
	tm	SP_PSW+1(%r15),0x01	# interrupting from user ?
	bz	BASED(ext_no_vtime)
	UPDATE_VTIME __LC_EXIT_TIMER,__LC_ASYNC_ENTER_TIMER,__LC_USER_TIMER
	UPDATE_VTIME __LC_LAST_UPDATE_TIMER,__LC_EXIT_TIMER,__LC_SYSTEM_TIMER
	mvc	__LC_LAST_UPDATE_TIMER(8),__LC_ASYNC_ENTER_TIMER
ext_no_vtime:
	TRACE_IRQS_OFF
	la	%r2,SP_PTREGS(%r15)	# address of register-save area
	l	%r3,__LC_CPU_ADDRESS	# get cpu address + interruption code
	l	%r4,__LC_EXT_PARAMS	# get external parameters
	l	%r1,BASED(.Ldo_extint)
	basr	%r14,%r1
	b	BASED(io_return)
=======
ENTRY(ext_int_handler)
	stck	__LC_INT_CLOCK
	stpt	__LC_ASYNC_ENTER_TIMER
	stm	%r8,%r15,__LC_SAVE_AREA_ASYNC
	l	%r12,__LC_THREAD_INFO
	l	%r13,__LC_SVC_NEW_PSW+4
	lm	%r8,%r9,__LC_EXT_OLD_PSW
	tmh	%r8,0x0001		# interrupting from user ?
	jz	ext_skip
	UPDATE_VTIME %r14,%r15,__LC_ASYNC_ENTER_TIMER
ext_skip:
	SWITCH_ASYNC __LC_SAVE_AREA_ASYNC,__LC_ASYNC_STACK,STACK_SHIFT
	stm	%r0,%r7,__PT_R0(%r11)
	mvc	__PT_R8(32,%r11),__LC_SAVE_AREA_ASYNC
	stm	%r8,%r9,__PT_PSW(%r11)
	TRACE_IRQS_OFF
	lr	%r2,%r11		# pass pointer to pt_regs
	l	%r3,__LC_EXT_CPU_ADDR	# get cpu address + interruption code
	l	%r4,__LC_EXT_PARAMS	# get external parameters
	l	%r1,BASED(.Ldo_extint)
	basr	%r14,%r1		# call do_extint
	j	io_return

/*
 * Load idle PSW. The second "half" of this function is in cleanup_idle.
 */
ENTRY(psw_idle)
	st	%r4,__SF_EMPTY(%r15)
	basr	%r1,0
	la	%r1,psw_idle_lpsw+4-.(%r1)
	st	%r1,__SF_EMPTY+4(%r15)
	oi	__SF_EMPTY+4(%r15),0x80
	la	%r1,.Lvtimer_max-psw_idle_lpsw-4(%r1)
	stck	__IDLE_ENTER(%r2)
	ltr	%r5,%r5
	stpt	__VQ_IDLE_ENTER(%r3)
	jz	psw_idle_lpsw
	spt	0(%r1)
psw_idle_lpsw:
	lpsw	__SF_EMPTY(%r15)
	br	%r14
psw_idle_end:
>>>>>>> refs/remotes/origin/cm-10.0

__critical_end:

/*
 * Machine check handler routines
 */

<<<<<<< HEAD
	.globl mcck_int_handler
mcck_int_handler:
	stck	__LC_MCCK_CLOCK
	spt	__LC_CPU_TIMER_SAVE_AREA	# revalidate cpu timer
	lm	%r0,%r15,__LC_GPREGS_SAVE_AREA	# revalidate gprs
	SAVE_ALL_BASE __LC_SAVE_AREA+32
	la	%r12,__LC_MCK_OLD_PSW
	tm	__LC_MCCK_CODE,0x80	# system damage?
	bo	BASED(mcck_int_main)	# yes -> rest of mcck code invalid
	mvc	__LC_MCCK_ENTER_TIMER(8),__LC_CPU_TIMER_SAVE_AREA
	tm	__LC_MCCK_CODE+5,0x02	# stored cpu timer value valid?
	bo	BASED(1f)
	la	%r14,__LC_SYNC_ENTER_TIMER
	clc	0(8,%r14),__LC_ASYNC_ENTER_TIMER
	bl	BASED(0f)
	la	%r14,__LC_ASYNC_ENTER_TIMER
0:	clc	0(8,%r14),__LC_EXIT_TIMER
	bl	BASED(0f)
	la	%r14,__LC_EXIT_TIMER
0:	clc	0(8,%r14),__LC_LAST_UPDATE_TIMER
	bl	BASED(0f)
	la	%r14,__LC_LAST_UPDATE_TIMER
0:	spt	0(%r14)
	mvc	__LC_MCCK_ENTER_TIMER(8),0(%r14)
1:	tm	__LC_MCCK_CODE+2,0x09	# mwp + ia of old psw valid?
	bno	BASED(mcck_int_main)	# no -> skip cleanup critical
	tm	__LC_MCK_OLD_PSW+1,0x01	# test problem state bit
	bnz	BASED(mcck_int_main)	# from user -> load async stack
	clc	__LC_MCK_OLD_PSW+4(4),BASED(.Lcritical_end)
	bhe	BASED(mcck_int_main)
	clc	__LC_MCK_OLD_PSW+4(4),BASED(.Lcritical_start)
	bl	BASED(mcck_int_main)
	l	%r14,BASED(.Lcleanup_critical)
	basr	%r14,%r14
mcck_int_main:
	l	%r14,__LC_PANIC_STACK	# are we already on the panic stack?
	slr	%r14,%r15
	sra	%r14,PAGE_SHIFT
	be	BASED(0f)
	l	%r15,__LC_PANIC_STACK	# load panic stack
0:	s	%r15,BASED(.Lc_spsize)	# make room for registers & psw
	CREATE_STACK_FRAME __LC_SAVE_AREA+32
	mvc	SP_PSW(8,%r15),0(%r12)
	l	%r12,__LC_THREAD_INFO	# load pointer to thread_info struct
	tm	__LC_MCCK_CODE+2,0x08	# mwp of old psw valid?
	bno	BASED(mcck_no_vtime)	# no -> skip cleanup critical
	tm	SP_PSW+1(%r15),0x01	# interrupting from user ?
	bz	BASED(mcck_no_vtime)
	UPDATE_VTIME __LC_EXIT_TIMER,__LC_MCCK_ENTER_TIMER,__LC_USER_TIMER
	UPDATE_VTIME __LC_LAST_UPDATE_TIMER,__LC_EXIT_TIMER,__LC_SYSTEM_TIMER
	mvc	__LC_LAST_UPDATE_TIMER(8),__LC_MCCK_ENTER_TIMER
mcck_no_vtime:
	la	%r2,SP_PTREGS(%r15)	# load pt_regs
	l	%r1,BASED(.Ls390_mcck)
	basr	%r14,%r1		# call machine check handler
	tm	SP_PSW+1(%r15),0x01	# returning to user ?
	bno	BASED(mcck_return)
	l	%r1,__LC_KERNEL_STACK	# switch to kernel stack
	s	%r1,BASED(.Lc_spsize)
	mvc	SP_PTREGS(__PT_SIZE,%r1),SP_PTREGS(%r15)
	xc	__SF_BACKCHAIN(4,%r1),__SF_BACKCHAIN(%r1) # clear back chain
	lr	%r15,%r1
	stosm	__SF_EMPTY(%r15),0x04	# turn dat on
	tm	__TI_flags+3(%r12),_TIF_MCCK_PENDING
	bno	BASED(mcck_return)
	TRACE_IRQS_OFF
	l	%r1,BASED(.Ls390_handle_mcck)
	basr	%r14,%r1		# call machine check handler
	TRACE_IRQS_ON
mcck_return:
	mvc	__LC_RETURN_MCCK_PSW(8),SP_PSW(%r15) # move return PSW
	ni	__LC_RETURN_MCCK_PSW+1,0xfd # clear wait state bit
	tm	__LC_RETURN_MCCK_PSW+1,0x01 # returning to user ?
	bno	BASED(0f)
	lm	%r0,%r15,SP_R0(%r15)	# load gprs 0-15
	stpt	__LC_EXIT_TIMER
	lpsw	__LC_RETURN_MCCK_PSW	# back to caller
0:	lm	%r0,%r15,SP_R0(%r15)	# load gprs 0-15
	lpsw	__LC_RETURN_MCCK_PSW	# back to caller

	RESTORE_ALL __LC_RETURN_MCCK_PSW,0

/*
 * Restart interruption handler, kick starter for additional CPUs
 */
#ifdef CONFIG_SMP
	__CPUINIT
	.globl restart_int_handler
restart_int_handler:
	basr	%r1,0
restart_base:
	spt	restart_vtime-restart_base(%r1)
	stck	__LC_LAST_UPDATE_CLOCK
	mvc	__LC_LAST_UPDATE_TIMER(8),restart_vtime-restart_base(%r1)
	mvc	__LC_EXIT_TIMER(8),restart_vtime-restart_base(%r1)
	l	%r15,__LC_SAVE_AREA+60	# load ksp
	lctl	%c0,%c15,__LC_CREGS_SAVE_AREA # get new ctl regs
	lam	%a0,%a15,__LC_AREGS_SAVE_AREA
	lm	%r6,%r15,__SF_GPRS(%r15) # load registers from clone
	l	%r1,__LC_THREAD_INFO
	mvc	__LC_USER_TIMER(8),__TI_user_timer(%r1)
	mvc	__LC_SYSTEM_TIMER(8),__TI_system_timer(%r1)
	xc	__LC_STEAL_TIMER(8),__LC_STEAL_TIMER
	stosm	__SF_EMPTY(%r15),0x04	# now we can turn dat on
	basr	%r14,0
	l	%r14,restart_addr-.(%r14)
	basr	%r14,%r14		# branch to start_secondary
restart_addr:
	.long	start_secondary
	.align	8
restart_vtime:
	.long	0x7fffffff,0xffffffff
	.previous
#else
/*
 * If we do not run with SMP enabled, let the new CPU crash ...
 */
	.globl restart_int_handler
restart_int_handler:
	basr	%r1,0
restart_base:
	lpsw	restart_crash-restart_base(%r1)
	.align	8
restart_crash:
	.long	0x000a0000,0x00000000
restart_go:
#endif
=======
ENTRY(mcck_int_handler)
	stck	__LC_MCCK_CLOCK
	spt	__LC_CPU_TIMER_SAVE_AREA	# revalidate cpu timer
	lm	%r0,%r15,__LC_GPREGS_SAVE_AREA	# revalidate gprs
	l	%r12,__LC_THREAD_INFO
	l	%r13,__LC_SVC_NEW_PSW+4
	lm	%r8,%r9,__LC_MCK_OLD_PSW
	tm	__LC_MCCK_CODE,0x80	# system damage?
	jo	mcck_panic		# yes -> rest of mcck code invalid
	la	%r14,__LC_CPU_TIMER_SAVE_AREA
	mvc	__LC_MCCK_ENTER_TIMER(8),0(%r14)
	tm	__LC_MCCK_CODE+5,0x02	# stored cpu timer value valid?
	jo	3f
	la	%r14,__LC_SYNC_ENTER_TIMER
	clc	0(8,%r14),__LC_ASYNC_ENTER_TIMER
	jl	0f
	la	%r14,__LC_ASYNC_ENTER_TIMER
0:	clc	0(8,%r14),__LC_EXIT_TIMER
	jl	1f
	la	%r14,__LC_EXIT_TIMER
1:	clc	0(8,%r14),__LC_LAST_UPDATE_TIMER
	jl	2f
	la	%r14,__LC_LAST_UPDATE_TIMER
2:	spt	0(%r14)
	mvc	__LC_MCCK_ENTER_TIMER(8),0(%r14)
3:	tm	__LC_MCCK_CODE+2,0x09	# mwp + ia of old psw valid?
	jno	mcck_panic		# no -> skip cleanup critical
	tm	%r8,0x0001		# interrupting from user ?
	jz	mcck_skip
	UPDATE_VTIME %r14,%r15,__LC_MCCK_ENTER_TIMER
mcck_skip:
	SWITCH_ASYNC __LC_GPREGS_SAVE_AREA+32,__LC_PANIC_STACK,PAGE_SHIFT
	stm	%r0,%r7,__PT_R0(%r11)
	mvc	__PT_R8(32,%r11),__LC_GPREGS_SAVE_AREA+32
	stm	%r8,%r9,__PT_PSW(%r11)
	xc	__SF_BACKCHAIN(4,%r15),__SF_BACKCHAIN(%r15)
	l	%r1,BASED(.Ldo_machine_check)
	lr	%r2,%r11		# pass pointer to pt_regs
	basr	%r14,%r1		# call s390_do_machine_check
	tm	__PT_PSW+1(%r11),0x01	# returning to user ?
	jno	mcck_return
	l	%r1,__LC_KERNEL_STACK	# switch to kernel stack
	ahi	%r1,-(STACK_FRAME_OVERHEAD + __PT_SIZE)
	mvc	STACK_FRAME_OVERHEAD(__PT_SIZE,%r1),0(%r11)
	xc	__SF_BACKCHAIN(4,%r1),__SF_BACKCHAIN(%r1)
	la	%r11,STACK_FRAME_OVERHEAD(%r15)
	lr	%r15,%r1
	ssm	__LC_PGM_NEW_PSW	# turn dat on, keep irqs off
	tm	__TI_flags+3(%r12),_TIF_MCCK_PENDING
	jno	mcck_return
	TRACE_IRQS_OFF
	l	%r1,BASED(.Lhandle_mcck)
	basr	%r14,%r1		# call s390_handle_mcck
	TRACE_IRQS_ON
mcck_return:
	mvc	__LC_RETURN_MCCK_PSW(8),__PT_PSW(%r11) # move return PSW
	tm	__LC_RETURN_MCCK_PSW+1,0x01 # returning to user ?
	jno	0f
	lm	%r0,%r15,__PT_R0(%r11)
	stpt	__LC_EXIT_TIMER
	lpsw	__LC_RETURN_MCCK_PSW
0:	lm	%r0,%r15,__PT_R0(%r11)
	lpsw	__LC_RETURN_MCCK_PSW

mcck_panic:
	l	%r14,__LC_PANIC_STACK
	slr	%r14,%r15
	sra	%r14,PAGE_SHIFT
	jz	0f
	l	%r15,__LC_PANIC_STACK
0:	ahi	%r15,-(STACK_FRAME_OVERHEAD + __PT_SIZE)
	j	mcck_skip

#
# PSW restart interrupt handler
#
ENTRY(restart_int_handler)
	st	%r15,__LC_SAVE_AREA_RESTART
	l	%r15,__LC_RESTART_STACK
	ahi	%r15,-__PT_SIZE			# create pt_regs on stack
	xc	0(__PT_SIZE,%r15),0(%r15)
	stm	%r0,%r14,__PT_R0(%r15)
	mvc	__PT_R15(4,%r15),__LC_SAVE_AREA_RESTART
	mvc	__PT_PSW(8,%r15),__LC_RST_OLD_PSW # store restart old psw
	ahi	%r15,-STACK_FRAME_OVERHEAD	# create stack frame on stack
	xc	0(STACK_FRAME_OVERHEAD,%r15),0(%r15)
	lm	%r1,%r3,__LC_RESTART_FN		# load fn, parm & source cpu
	ltr	%r3,%r3				# test source cpu address
	jm	1f				# negative -> skip source stop
0:	sigp	%r4,%r3,1			# sigp sense to source cpu
	brc	10,0b				# wait for status stored
1:	basr	%r14,%r1			# call function
	stap	__SF_EMPTY(%r15)		# store cpu address
	lh	%r3,__SF_EMPTY(%r15)
2:	sigp	%r4,%r3,5			# sigp stop to current cpu
	brc	2,2b
3:	j	3b
>>>>>>> refs/remotes/origin/cm-10.0

	.section .kprobes.text, "ax"

#ifdef CONFIG_CHECK_STACK
/*
 * The synchronous or the asynchronous stack overflowed. We are dead.
 * No need to properly save the registers, we are going to panic anyway.
 * Setup a pt_regs so that show_trace can provide a good call trace.
 */
stack_overflow:
	l	%r15,__LC_PANIC_STACK	# change to panic stack
<<<<<<< HEAD
	sl	%r15,BASED(.Lc_spsize)
	mvc	SP_PSW(8,%r15),0(%r12)	# move user PSW to stack
	stm	%r0,%r11,SP_R0(%r15)	# store gprs %r0-%r11 to kernel stack
	la	%r1,__LC_SAVE_AREA
	ch	%r12,BASED(.L0x020)	# old psw addr == __LC_SVC_OLD_PSW ?
	be	BASED(0f)
	ch	%r12,BASED(.L0x028)	# old psw addr == __LC_PGM_OLD_PSW ?
	be	BASED(0f)
	la	%r1,__LC_SAVE_AREA+16
0:	mvc	SP_R12(16,%r15),0(%r1)	# move %r12-%r15 to stack
	xc	__SF_BACKCHAIN(4,%r15),__SF_BACKCHAIN(%r15) # clear back chain
	l	%r1,BASED(1f)		# branch to kernel_stack_overflow
	la	%r2,SP_PTREGS(%r15)	# load pt_regs
	br	%r1
1:	.long	kernel_stack_overflow
#endif

cleanup_table_system_call:
	.long	system_call + 0x80000000, sysc_do_svc + 0x80000000
cleanup_table_sysc_tif:
	.long	sysc_tif + 0x80000000, sysc_restore + 0x80000000
cleanup_table_sysc_restore:
	.long	sysc_restore + 0x80000000, sysc_done + 0x80000000
cleanup_table_io_tif:
	.long	io_tif + 0x80000000, io_restore + 0x80000000
cleanup_table_io_restore:
	.long	io_restore + 0x80000000, io_done + 0x80000000

cleanup_critical:
	clc	4(4,%r12),BASED(cleanup_table_system_call)
	bl	BASED(0f)
	clc	4(4,%r12),BASED(cleanup_table_system_call+4)
	bl	BASED(cleanup_system_call)
0:
	clc	4(4,%r12),BASED(cleanup_table_sysc_tif)
	bl	BASED(0f)
	clc	4(4,%r12),BASED(cleanup_table_sysc_tif+4)
	bl	BASED(cleanup_sysc_tif)
0:
	clc	4(4,%r12),BASED(cleanup_table_sysc_restore)
	bl	BASED(0f)
	clc	4(4,%r12),BASED(cleanup_table_sysc_restore+4)
	bl	BASED(cleanup_sysc_restore)
0:
	clc	4(4,%r12),BASED(cleanup_table_io_tif)
	bl	BASED(0f)
	clc	4(4,%r12),BASED(cleanup_table_io_tif+4)
	bl	BASED(cleanup_io_tif)
0:
	clc	4(4,%r12),BASED(cleanup_table_io_restore)
	bl	BASED(0f)
	clc	4(4,%r12),BASED(cleanup_table_io_restore+4)
	bl	BASED(cleanup_io_restore)
0:
	br	%r14

cleanup_system_call:
	mvc	__LC_RETURN_PSW(8),0(%r12)
	clc	__LC_RETURN_PSW+4(4),BASED(cleanup_system_call_insn+4)
	bh	BASED(0f)
	mvc	__LC_SYNC_ENTER_TIMER(8),__LC_MCCK_ENTER_TIMER
	c	%r12,BASED(.Lmck_old_psw)
	be	BASED(0f)
	mvc	__LC_SYNC_ENTER_TIMER(8),__LC_ASYNC_ENTER_TIMER
0:	c	%r12,BASED(.Lmck_old_psw)
	la	%r12,__LC_SAVE_AREA+32
	be	BASED(0f)
	la	%r12,__LC_SAVE_AREA+16
0:	clc	__LC_RETURN_PSW+4(4),BASED(cleanup_system_call_insn+8)
	bhe	BASED(cleanup_vtime)
	clc	__LC_RETURN_PSW+4(4),BASED(cleanup_system_call_insn)
	bh	BASED(0f)
	mvc	__LC_SAVE_AREA(16),0(%r12)
0:	st	%r13,4(%r12)
	l	%r15,__LC_KERNEL_STACK	# problem state -> load ksp
	s	%r15,BASED(.Lc_spsize)	# make room for registers & psw
	st	%r15,12(%r12)
	CREATE_STACK_FRAME __LC_SAVE_AREA
	mvc	SP_PSW(8,%r15),__LC_SVC_OLD_PSW
	mvc	SP_ILC(4,%r15),__LC_SVC_ILC
	mvc	0(4,%r12),__LC_THREAD_INFO
cleanup_vtime:
	clc	__LC_RETURN_PSW+4(4),BASED(cleanup_system_call_insn+12)
	bhe	BASED(cleanup_stime)
	UPDATE_VTIME __LC_EXIT_TIMER,__LC_SYNC_ENTER_TIMER,__LC_USER_TIMER
cleanup_stime:
	clc	__LC_RETURN_PSW+4(4),BASED(cleanup_system_call_insn+16)
	bh	BASED(cleanup_update)
	UPDATE_VTIME __LC_LAST_UPDATE_TIMER,__LC_EXIT_TIMER,__LC_SYSTEM_TIMER
cleanup_update:
	mvc	__LC_LAST_UPDATE_TIMER(8),__LC_SYNC_ENTER_TIMER
	mvc	__LC_RETURN_PSW+4(4),BASED(cleanup_table_system_call+4)
	la	%r12,__LC_RETURN_PSW
	br	%r14
cleanup_system_call_insn:
	.long	sysc_saveall + 0x80000000
	.long	system_call + 0x80000000
	.long	sysc_vtime + 0x80000000
	.long	sysc_stime + 0x80000000
	.long	sysc_update + 0x80000000

cleanup_sysc_tif:
	mvc	__LC_RETURN_PSW(4),0(%r12)
	mvc	__LC_RETURN_PSW+4(4),BASED(cleanup_table_sysc_tif)
	la	%r12,__LC_RETURN_PSW
	br	%r14

cleanup_sysc_restore:
	clc	4(4,%r12),BASED(cleanup_sysc_restore_insn)
	be	BASED(2f)
	mvc	__LC_EXIT_TIMER(8),__LC_MCCK_ENTER_TIMER
	c	%r12,BASED(.Lmck_old_psw)
	be	BASED(0f)
	mvc	__LC_EXIT_TIMER(8),__LC_ASYNC_ENTER_TIMER
0:	clc	4(4,%r12),BASED(cleanup_sysc_restore_insn+4)
	be	BASED(2f)
	mvc	__LC_RETURN_PSW(8),SP_PSW(%r15)
	c	%r12,BASED(.Lmck_old_psw)
	la	%r12,__LC_SAVE_AREA+32
	be	BASED(1f)
	la	%r12,__LC_SAVE_AREA+16
1:	mvc	0(16,%r12),SP_R12(%r15)
	lm	%r0,%r11,SP_R0(%r15)
	l	%r15,SP_R15(%r15)
2:	la	%r12,__LC_RETURN_PSW
	br	%r14
cleanup_sysc_restore_insn:
	.long	sysc_done - 4 + 0x80000000
	.long	sysc_done - 8 + 0x80000000

cleanup_io_tif:
	mvc	__LC_RETURN_PSW(4),0(%r12)
	mvc	__LC_RETURN_PSW+4(4),BASED(cleanup_table_io_tif)
	la	%r12,__LC_RETURN_PSW
	br	%r14

cleanup_io_restore:
	clc	4(4,%r12),BASED(cleanup_io_restore_insn)
	be	BASED(1f)
	mvc	__LC_EXIT_TIMER(8),__LC_MCCK_ENTER_TIMER
	clc	4(4,%r12),BASED(cleanup_io_restore_insn+4)
	be	BASED(1f)
	mvc	__LC_RETURN_PSW(8),SP_PSW(%r15)
	mvc	__LC_SAVE_AREA+32(16),SP_R12(%r15)
	lm	%r0,%r11,SP_R0(%r15)
	l	%r15,SP_R15(%r15)
1:	la	%r12,__LC_RETURN_PSW
	br	%r14
cleanup_io_restore_insn:
	.long	io_done - 4 + 0x80000000
	.long	io_done - 8 + 0x80000000
=======
	ahi	%r15,-__PT_SIZE		# create pt_regs
	stm	%r0,%r7,__PT_R0(%r15)
	stm	%r8,%r9,__PT_PSW(%r15)
	mvc	__PT_R8(32,%r11),0(%r14)
	lr	%r15,%r11
	ahi	%r15,-STACK_FRAME_OVERHEAD
	l	%r1,BASED(1f)
	xc	__SF_BACKCHAIN(4,%r15),__SF_BACKCHAIN(%r15)
	lr	%r2,%r11		# pass pointer to pt_regs
	br	%r1			# branch to kernel_stack_overflow
1:	.long	kernel_stack_overflow
#endif

cleanup_table:
	.long	system_call + 0x80000000
	.long	sysc_do_svc + 0x80000000
	.long	sysc_tif + 0x80000000
	.long	sysc_restore + 0x80000000
	.long	sysc_done + 0x80000000
	.long	io_tif + 0x80000000
	.long	io_restore + 0x80000000
	.long	io_done + 0x80000000
	.long	psw_idle + 0x80000000
	.long	psw_idle_end + 0x80000000

cleanup_critical:
	cl	%r9,BASED(cleanup_table)	# system_call
	jl	0f
	cl	%r9,BASED(cleanup_table+4)	# sysc_do_svc
	jl	cleanup_system_call
	cl	%r9,BASED(cleanup_table+8)	# sysc_tif
	jl	0f
	cl	%r9,BASED(cleanup_table+12)	# sysc_restore
	jl	cleanup_sysc_tif
	cl	%r9,BASED(cleanup_table+16)	# sysc_done
	jl	cleanup_sysc_restore
	cl	%r9,BASED(cleanup_table+20)	# io_tif
	jl	0f
	cl	%r9,BASED(cleanup_table+24)	# io_restore
	jl	cleanup_io_tif
	cl	%r9,BASED(cleanup_table+28)	# io_done
	jl	cleanup_io_restore
	cl	%r9,BASED(cleanup_table+32)	# psw_idle
	jl	0f
	cl	%r9,BASED(cleanup_table+36)	# psw_idle_end
	jl	cleanup_idle
0:	br	%r14

cleanup_system_call:
	# check if stpt has been executed
	cl	%r9,BASED(cleanup_system_call_insn)
	jh	0f
	mvc	__LC_SYNC_ENTER_TIMER(8),__LC_ASYNC_ENTER_TIMER
	chi	%r11,__LC_SAVE_AREA_ASYNC
	je	0f
	mvc	__LC_SYNC_ENTER_TIMER(8),__LC_MCCK_ENTER_TIMER
0:	# check if stm has been executed
	cl	%r9,BASED(cleanup_system_call_insn+4)
	jh	0f
	mvc	__LC_SAVE_AREA_SYNC(32),0(%r11)
0:	# set up saved registers r12, and r13
	st	%r12,16(%r11)		# r12 thread-info pointer
	st	%r13,20(%r11)		# r13 literal-pool pointer
	# check if the user time calculation has been done
	cl	%r9,BASED(cleanup_system_call_insn+8)
	jh	0f
	l	%r10,__LC_EXIT_TIMER
	l	%r15,__LC_EXIT_TIMER+4
	SUB64	%r10,%r15,__LC_SYNC_ENTER_TIMER
	ADD64	%r10,%r15,__LC_USER_TIMER
	st	%r10,__LC_USER_TIMER
	st	%r15,__LC_USER_TIMER+4
0:	# check if the system time calculation has been done
	cl	%r9,BASED(cleanup_system_call_insn+12)
	jh	0f
	l	%r10,__LC_LAST_UPDATE_TIMER
	l	%r15,__LC_LAST_UPDATE_TIMER+4
	SUB64	%r10,%r15,__LC_EXIT_TIMER
	ADD64	%r10,%r15,__LC_SYSTEM_TIMER
	st	%r10,__LC_SYSTEM_TIMER
	st	%r15,__LC_SYSTEM_TIMER+4
0:	# update accounting time stamp
	mvc	__LC_LAST_UPDATE_TIMER(8),__LC_SYNC_ENTER_TIMER
	# set up saved register 11
	l	%r15,__LC_KERNEL_STACK
	ahi	%r15,-__PT_SIZE
	st	%r15,12(%r11)		# r11 pt_regs pointer
	# fill pt_regs
	mvc	__PT_R8(32,%r15),__LC_SAVE_AREA_SYNC
	stm	%r0,%r7,__PT_R0(%r15)
	mvc	__PT_PSW(8,%r15),__LC_SVC_OLD_PSW
	mvc	__PT_INT_CODE(4,%r15),__LC_SVC_ILC
	# setup saved register 15
	ahi	%r15,-STACK_FRAME_OVERHEAD
	st	%r15,28(%r11)		# r15 stack pointer
	# set new psw address and exit
	l	%r9,BASED(cleanup_table+4)	# sysc_do_svc + 0x80000000
	br	%r14
cleanup_system_call_insn:
	.long	system_call + 0x80000000
	.long	sysc_stm + 0x80000000
	.long	sysc_vtime + 0x80000000 + 36
	.long	sysc_vtime + 0x80000000 + 76

cleanup_sysc_tif:
	l	%r9,BASED(cleanup_table+8)	# sysc_tif + 0x80000000
	br	%r14

cleanup_sysc_restore:
	cl	%r9,BASED(cleanup_sysc_restore_insn)
	jhe	0f
	l	%r9,12(%r11)		# get saved pointer to pt_regs
	mvc	__LC_RETURN_PSW(8),__PT_PSW(%r9)
	mvc	0(32,%r11),__PT_R8(%r9)
	lm	%r0,%r7,__PT_R0(%r9)
0:	lm	%r8,%r9,__LC_RETURN_PSW
	br	%r14
cleanup_sysc_restore_insn:
	.long	sysc_done - 4 + 0x80000000

cleanup_io_tif:
	l	%r9,BASED(cleanup_table+20)	# io_tif + 0x80000000
	br	%r14

cleanup_io_restore:
	cl	%r9,BASED(cleanup_io_restore_insn)
	jhe	0f
	l	%r9,12(%r11)		# get saved r11 pointer to pt_regs
	mvc	__LC_RETURN_PSW(8),__PT_PSW(%r9)
	mvc	0(32,%r11),__PT_R8(%r9)
	lm	%r0,%r7,__PT_R0(%r9)
0:	lm	%r8,%r9,__LC_RETURN_PSW
	br	%r14
cleanup_io_restore_insn:
	.long	io_done - 4 + 0x80000000

cleanup_idle:
	# copy interrupt clock & cpu timer
	mvc	__IDLE_EXIT(8,%r2),__LC_INT_CLOCK
	mvc	__VQ_IDLE_EXIT(8,%r3),__LC_ASYNC_ENTER_TIMER
	chi	%r11,__LC_SAVE_AREA_ASYNC
	je	0f
	mvc	__IDLE_EXIT(8,%r2),__LC_MCCK_CLOCK
	mvc	__VQ_IDLE_EXIT(8,%r3),__LC_MCCK_ENTER_TIMER
0:	# check if stck has been executed
	cl	%r9,BASED(cleanup_idle_insn)
	jhe	1f
	mvc	__IDLE_ENTER(8,%r2),__IDLE_EXIT(%r2)
	mvc	__VQ_IDLE_ENTER(8,%r3),__VQ_IDLE_EXIT(%r3)
	j	2f
1:	# check if the cpu timer has been reprogrammed
	ltr	%r5,%r5
	jz	2f
	spt	__VQ_IDLE_ENTER(%r3)
2:	# account system time going idle
	lm	%r9,%r10,__LC_STEAL_TIMER
	ADD64	%r9,%r10,__IDLE_ENTER(%r2)
	SUB64	%r9,%r10,__LC_LAST_UPDATE_CLOCK
	stm	%r9,%r10,__LC_STEAL_TIMER
	mvc	__LC_LAST_UPDATE_CLOCK(8),__IDLE_EXIT(%r2)
	lm	%r9,%r10,__LC_SYSTEM_TIMER
	ADD64	%r9,%r10,__LC_LAST_UPDATE_TIMER
	SUB64	%r9,%r10,__VQ_IDLE_ENTER(%r3)
	stm	%r9,%r10,__LC_SYSTEM_TIMER
	mvc	__LC_LAST_UPDATE_TIMER(8),__VQ_IDLE_EXIT(%r3)
	# prepare return psw
	n	%r8,BASED(cleanup_idle_wait)	# clear wait state bit
	l	%r9,24(%r11)			# return from psw_idle
	br	%r14
cleanup_idle_insn:
	.long	psw_idle_lpsw + 0x80000000
cleanup_idle_wait:
	.long	0xfffdffff
>>>>>>> refs/remotes/origin/cm-10.0

/*
 * Integer constants
 */
<<<<<<< HEAD
		.align	4
.Lc_spsize:	.long	SP_SIZE
.Lc_overhead:	.long	STACK_FRAME_OVERHEAD
.Lnr_syscalls:	.long	NR_syscalls
.L0x018:	.short	0x018
.L0x020:	.short	0x020
.L0x028:	.short	0x028
.L0x030:	.short	0x030
.L0x038:	.short	0x038
.Lc_1:		.long	1
=======
	.align	4
.Lnr_syscalls:
	.long	NR_syscalls
.Lvtimer_max:
	.quad	0x7fffffffffffffff
>>>>>>> refs/remotes/origin/cm-10.0

/*
 * Symbol constants
 */
<<<<<<< HEAD
.Ls390_mcck:	.long	s390_do_machine_check
.Ls390_handle_mcck:
		.long	s390_handle_mcck
.Lmck_old_psw:	.long	__LC_MCK_OLD_PSW
.Ldo_IRQ:	.long	do_IRQ
.Ldo_extint:	.long	do_extint
.Ldo_signal:	.long	do_signal
.Ldo_notify_resume:
		.long	do_notify_resume
.Lhandle_per:	.long	do_per_trap
.Ldo_execve:	.long	do_execve
.Lexecve_tail:	.long	execve_tail
.Ljump_table:	.long	pgm_check_table
.Lschedule:	.long	schedule
#ifdef CONFIG_PREEMPT
.Lpreempt_schedule_irq:
		.long	preempt_schedule_irq
#endif
.Ltrace_entry:	.long	do_syscall_trace_enter
.Ltrace_exit:	.long	do_syscall_trace_exit
.Lschedtail:	.long	schedule_tail
.Lsysc_table:	.long	sys_call_table
#ifdef CONFIG_TRACE_IRQFLAGS
.Ltrace_irq_on_caller:
		.long	trace_hardirqs_on_caller
.Ltrace_irq_off_caller:
		.long	trace_hardirqs_off_caller
#endif
#ifdef CONFIG_LOCKDEP
.Llockdep_sys_exit:
		.long	lockdep_sys_exit
#endif
.Lcritical_start:
		.long	__critical_start + 0x80000000
.Lcritical_end:
		.long	__critical_end + 0x80000000
.Lcleanup_critical:
		.long	cleanup_critical
=======
.Ldo_machine_check:	.long	s390_do_machine_check
.Lhandle_mcck:		.long	s390_handle_mcck
.Ldo_IRQ:		.long	do_IRQ
.Ldo_extint:		.long	do_extint
.Ldo_signal:		.long	do_signal
.Ldo_notify_resume:	.long	do_notify_resume
.Ldo_per_trap:		.long	do_per_trap
.Ldo_execve:		.long	do_execve
.Lexecve_tail:		.long	execve_tail
.Ljump_table:		.long	pgm_check_table
.Lschedule:		.long	schedule
#ifdef CONFIG_PREEMPT
.Lpreempt_irq:		.long	preempt_schedule_irq
#endif
.Ltrace_enter:		.long	do_syscall_trace_enter
.Ltrace_exit:		.long	do_syscall_trace_exit
.Lschedule_tail:	.long	schedule_tail
.Lsys_call_table:	.long	sys_call_table
.Lsysc_per:		.long	sysc_per + 0x80000000
#ifdef CONFIG_TRACE_IRQFLAGS
.Lhardirqs_on:		.long	trace_hardirqs_on_caller
.Lhardirqs_off:		.long	trace_hardirqs_off_caller
#endif
#ifdef CONFIG_LOCKDEP
.Llockdep_sys_exit:	.long	lockdep_sys_exit
#endif
.Lcritical_start:	.long	__critical_start + 0x80000000
.Lcritical_length:	.long	__critical_end - __critical_start
>>>>>>> refs/remotes/origin/cm-10.0

		.section .rodata, "a"
#define SYSCALL(esa,esame,emu)	.long esa
	.globl	sys_call_table
sys_call_table:
#include "syscalls.S"
#undef SYSCALL
