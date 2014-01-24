/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1995-99, 2000- 02, 06 Ralf Baechle <ralf@linux-mips.org>
 * Copyright (C) 2001 MIPS Technologies, Inc.
 * Copyright (C) 2004 Thiemo Seufer
 */
#include <linux/errno.h>
#include <asm/asm.h>
#include <asm/asmmacro.h>
#include <asm/irqflags.h>
#include <asm/mipsregs.h>
#include <asm/regdef.h>
#include <asm/stackframe.h>
#include <asm/isadep.h>
#include <asm/sysmips.h>
#include <asm/thread_info.h>
#include <asm/unistd.h>
#include <asm/war.h>
#include <asm/asm-offsets.h>

/* Highest syscall used of any syscall flavour */
#define MAX_SYSCALL_NO	__NR_O32_Linux + __NR_O32_Linux_syscalls

<<<<<<< HEAD
	.align  5
=======
	.align	5
>>>>>>> refs/remotes/origin/master
NESTED(handle_sys, PT_SIZE, sp)
	.set	noat
	SAVE_SOME
	TRACE_IRQS_ON_RELOAD
	STI
	.set	at

	lw	t1, PT_EPC(sp)		# skip syscall on return

	subu	v0, v0, __NR_O32_Linux	# check syscall number
	sltiu	t0, v0, __NR_O32_Linux_syscalls + 1
	addiu	t1, 4			# skip to next instruction
	sw	t1, PT_EPC(sp)
	beqz	t0, illegal_syscall

<<<<<<< HEAD
	sll	t0, v0, 3
	la	t1, sys_call_table
	addu	t1, t0
	lw	t2, (t1)		# syscall routine
	lw	t3, 4(t1)		# >= 0 if we need stack arguments
	beqz	t2, illegal_syscall

	sw	a3, PT_R26(sp)		# save a3 for syscall restarting
	bgez	t3, stackargs

stack_done:
	lw	t0, TI_FLAGS($28)	# syscall tracing enabled?
	li	t1, _TIF_SYSCALL_TRACE | _TIF_SYSCALL_AUDIT
	and	t0, t1
	bnez	t0, syscall_trace_entry	# -> yes
=======
	sll	t0, v0, 2
	la	t1, sys_call_table
	addu	t1, t0
	lw	t2, (t1)		# syscall routine
	beqz	t2, illegal_syscall

	sw	a3, PT_R26(sp)		# save a3 for syscall restarting

	/*
	 * More than four arguments.  Try to deal with it by copying the
	 * stack arguments from the user stack to the kernel stack.
	 * This Sucks (TM).
	 */
	lw	t0, PT_R29(sp)		# get old user stack pointer

	/*
	 * We intentionally keep the kernel stack a little below the top of
	 * userspace so we don't have to do a slower byte accurate check here.
	 */
	lw	t5, TI_ADDR_LIMIT($28)
	addu	t4, t0, 32
	and	t5, t4
	bltz	t5, bad_stack		# -> sp is bad

	/*
	 * Ok, copy the args from the luser stack to the kernel stack.
	 * t3 is the precomputed number of instruction bytes needed to
	 * load or store arguments 6-8.
	 */

	.set    push
	.set    noreorder
	.set	nomacro

1:	lw	t5, 16(t0)		# argument #5 from usp
4:	lw	t6, 20(t0)		# argument #6 from usp
3:	lw	t7, 24(t0)		# argument #7 from usp
2:	lw	t8, 28(t0)		# argument #8 from usp

	sw	t5, 16(sp)		# argument #5 to ksp
	sw	t6, 20(sp)		# argument #6 to ksp
	sw	t7, 24(sp)		# argument #7 to ksp
	sw	t8, 28(sp)		# argument #8 to ksp
	.set	pop

	.section __ex_table,"a"
	PTR	1b,bad_stack
	PTR	2b,bad_stack
	PTR	3b,bad_stack
	PTR	4b,bad_stack
	.previous

	lw	t0, TI_FLAGS($28)	# syscall tracing enabled?
	li	t1, _TIF_WORK_SYSCALL_ENTRY
	and	t0, t1
	bnez	t0, syscall_trace_entry # -> yes
>>>>>>> refs/remotes/origin/master

	jalr	t2			# Do The Real Thing (TM)

	li	t0, -EMAXERRNO - 1	# error?
	sltu	t0, t0, v0
	sw	t0, PT_R7(sp)		# set error flag
	beqz	t0, 1f

	lw	t1, PT_R2(sp)		# syscall number
	negu	v0			# error
	sw	t1, PT_R0(sp)		# save it for syscall restarting
1:	sw	v0, PT_R2(sp)		# result

o32_syscall_exit:
<<<<<<< HEAD
	local_irq_disable		# make sure need_resched and
					# signals dont change between
					# sampling and return
	lw	a2, TI_FLAGS($28)	# current->work
	li	t0, _TIF_ALLWORK_MASK
	and	t0, a2
	bnez	t0, o32_syscall_exit_work

	j	restore_partial

o32_syscall_exit_work:
	j	syscall_exit_work_partial
=======
	j	syscall_exit_partial
>>>>>>> refs/remotes/origin/master

/* ------------------------------------------------------------------------ */

syscall_trace_entry:
	SAVE_STATIC
	move	s0, t2
	move	a0, sp
	jal	syscall_trace_enter

	move	t0, s0
	RESTORE_STATIC
	lw	a0, PT_R4(sp)		# Restore argument registers
	lw	a1, PT_R5(sp)
	lw	a2, PT_R6(sp)
	lw	a3, PT_R7(sp)
	jalr	t0

	li	t0, -EMAXERRNO - 1	# error?
	sltu	t0, t0, v0
	sw	t0, PT_R7(sp)		# set error flag
	beqz	t0, 1f

	lw	t1, PT_R2(sp)		# syscall number
	negu	v0			# error
	sw	t1, PT_R0(sp)		# save it for syscall restarting
1:	sw	v0, PT_R2(sp)		# result

	j	syscall_exit

/* ------------------------------------------------------------------------ */

	/*
<<<<<<< HEAD
	 * More than four arguments.  Try to deal with it by copying the
	 * stack arguments from the user stack to the kernel stack.
	 * This Sucks (TM).
	 */
stackargs:
	lw	t0, PT_R29(sp)		# get old user stack pointer

	/*
	 * We intentionally keep the kernel stack a little below the top of
	 * userspace so we don't have to do a slower byte accurate check here.
	 */
	lw	t5, TI_ADDR_LIMIT($28)
	addu	t4, t0, 32
	and	t5, t4
	bltz	t5, bad_stack		# -> sp is bad

	/* Ok, copy the args from the luser stack to the kernel stack.
	 * t3 is the precomputed number of instruction bytes needed to
	 * load or store arguments 6-8.
	 */

	la	t1, 5f			# load up to 3 arguments
	subu	t1, t3
1:	lw	t5, 16(t0)		# argument #5 from usp
	.set    push
	.set    noreorder
	.set	nomacro
	jr	t1
	 addiu	t1, 6f - 5f

2:	lw	t8, 28(t0)		# argument #8 from usp
3:	lw	t7, 24(t0)		# argument #7 from usp
4:	lw	t6, 20(t0)		# argument #6 from usp
5:	jr	t1
	 sw	t5, 16(sp)		# argument #5 to ksp

	sw	t8, 28(sp)		# argument #8 to ksp
	sw	t7, 24(sp)		# argument #7 to ksp
	sw	t6, 20(sp)		# argument #6 to ksp
6:	j	stack_done		# go back
	 nop
	.set	pop

	.section __ex_table,"a"
	PTR	1b,bad_stack
	PTR	2b,bad_stack
	PTR	3b,bad_stack
	PTR	4b,bad_stack
	.previous

	/*
=======
>>>>>>> refs/remotes/origin/master
	 * The stackpointer for a call with more than 4 arguments is bad.
	 * We probably should handle this case a bit more drastic.
	 */
bad_stack:
	li	v0, EFAULT
	sw	v0, PT_R2(sp)
	li	t0, 1				# set error flag
	sw	t0, PT_R7(sp)
	j	o32_syscall_exit

	/*
	 * The system call does not exist in this kernel
	 */
illegal_syscall:
	li	v0, ENOSYS			# error
	sw	v0, PT_R2(sp)
	li	t0, 1				# set error flag
	sw	t0, PT_R7(sp)
	j	o32_syscall_exit
	END(handle_sys)

	LEAF(sys_syscall)
	subu	t0, a0, __NR_O32_Linux	# check syscall number
	sltiu	v0, t0, __NR_O32_Linux_syscalls + 1
	beqz	t0, einval		# do not recurse
<<<<<<< HEAD
	sll	t1, t0, 3
=======
	sll	t1, t0, 2
>>>>>>> refs/remotes/origin/master
	beqz	v0, einval
	lw	t2, sys_call_table(t1)		# syscall routine

	/* Some syscalls like execve get their arguments from struct pt_regs
	   and claim zero arguments in the syscall table. Thus we have to
	   assume the worst case and shuffle around all potential arguments.
	   If you want performance, don't use indirect syscalls. */

	move	a0, a1				# shift argument registers
	move	a1, a2
	move	a2, a3
	lw	a3, 16(sp)
	lw	t4, 20(sp)
	lw	t5, 24(sp)
	lw	t6, 28(sp)
	sw	t4, 16(sp)
	sw	t5, 20(sp)
	sw	t6, 24(sp)
	sw	a0, PT_R4(sp)			# .. and push back a0 - a3, some
	sw	a1, PT_R5(sp)			# syscalls expect them there
	sw	a2, PT_R6(sp)
	sw	a3, PT_R7(sp)
	sw	a3, PT_R26(sp)			# update a3 for syscall restarting
	jr	t2
	/* Unreached */

<<<<<<< HEAD
einval:	li	v0, -ENOSYS
	jr	ra
	END(sys_syscall)

	.macro	fifty ptr, nargs, from=1, to=50
	sys	\ptr		\nargs
	.if	\to-\from
	fifty	\ptr,\nargs,"(\from+1)",\to
	.endif
	.endm

	.macro	mille ptr, nargs, from=1, to=20
	fifty	\ptr,\nargs
	.if	\to-\from
	mille	\ptr,\nargs,"(\from+1)",\to
	.endif
	.endm

	.macro	syscalltable
	sys	sys_syscall		8	/* 4000 */
	sys	sys_exit		1
	sys	sys_fork		0
	sys	sys_read		3
	sys	sys_write		3
	sys	sys_open		3	/* 4005 */
	sys	sys_close		1
	sys	sys_waitpid		3
	sys	sys_creat		2
	sys	sys_link		2
	sys	sys_unlink		1	/* 4010 */
	sys	sys_execve		0
	sys	sys_chdir		1
	sys	sys_time		1
	sys	sys_mknod		3
	sys	sys_chmod		2	/* 4015 */
	sys	sys_lchown		3
	sys	sys_ni_syscall		0
	sys	sys_ni_syscall		0	/* was sys_stat */
	sys	sys_lseek		3
	sys	sys_getpid		0	/* 4020 */
	sys	sys_mount		5
	sys	sys_oldumount		1
	sys	sys_setuid		1
	sys	sys_getuid		0
	sys	sys_stime		1	/* 4025 */
	sys	sys_ptrace		4
	sys	sys_alarm		1
	sys	sys_ni_syscall		0	/* was sys_fstat */
	sys	sys_pause		0
	sys	sys_utime		2	/* 4030 */
	sys	sys_ni_syscall		0
	sys	sys_ni_syscall		0
	sys	sys_access		2
	sys	sys_nice		1
	sys	sys_ni_syscall		0	/* 4035 */
	sys	sys_sync		0
	sys	sys_kill		2
	sys	sys_rename		2
	sys	sys_mkdir		2
	sys	sys_rmdir		1	/* 4040 */
	sys	sys_dup			1
	sys	sysm_pipe		0
	sys	sys_times		1
	sys	sys_ni_syscall		0
	sys	sys_brk			1	/* 4045 */
	sys	sys_setgid		1
	sys	sys_getgid		0
	sys	sys_ni_syscall		0	/* was signal(2) */
	sys	sys_geteuid		0
	sys	sys_getegid		0	/* 4050 */
	sys	sys_acct		1
	sys	sys_umount		2
	sys	sys_ni_syscall		0
	sys	sys_ioctl		3
	sys	sys_fcntl		3	/* 4055 */
	sys	sys_ni_syscall		2
	sys	sys_setpgid		2
	sys	sys_ni_syscall		0
	sys	sys_olduname		1
	sys	sys_umask		1	/* 4060 */
	sys	sys_chroot		1
	sys	sys_ustat		2
	sys	sys_dup2		2
	sys	sys_getppid		0
	sys	sys_getpgrp		0	/* 4065 */
	sys	sys_setsid		0
	sys	sys_sigaction		3
	sys	sys_sgetmask		0
	sys	sys_ssetmask		1
	sys	sys_setreuid		2	/* 4070 */
	sys	sys_setregid		2
	sys	sys_sigsuspend		0
	sys	sys_sigpending		1
	sys	sys_sethostname		2
	sys	sys_setrlimit		2	/* 4075 */
	sys	sys_getrlimit		2
	sys	sys_getrusage		2
	sys	sys_gettimeofday	2
	sys	sys_settimeofday	2
	sys	sys_getgroups		2	/* 4080 */
	sys	sys_setgroups		2
	sys	sys_ni_syscall		0	/* old_select */
	sys	sys_symlink		2
	sys	sys_ni_syscall		0	/* was sys_lstat */
	sys	sys_readlink		3	/* 4085 */
	sys	sys_uselib		1
	sys	sys_swapon		2
	sys	sys_reboot		3
	sys	sys_old_readdir		3
	sys	sys_mips_mmap		6	/* 4090 */
	sys	sys_munmap		2
	sys	sys_truncate		2
	sys	sys_ftruncate		2
	sys	sys_fchmod		2
	sys	sys_fchown		3	/* 4095 */
	sys	sys_getpriority		2
	sys	sys_setpriority		3
	sys	sys_ni_syscall		0
	sys	sys_statfs		2
	sys	sys_fstatfs		2	/* 4100 */
	sys	sys_ni_syscall		0	/* was ioperm(2) */
	sys	sys_socketcall		2
	sys	sys_syslog		3
	sys	sys_setitimer		3
	sys	sys_getitimer		2	/* 4105 */
	sys	sys_newstat		2
	sys	sys_newlstat		2
	sys	sys_newfstat		2
	sys	sys_uname		1
	sys	sys_ni_syscall		0	/* 4110 was iopl(2) */
	sys	sys_vhangup		0
	sys	sys_ni_syscall		0	/* was sys_idle() */
	sys	sys_ni_syscall		0	/* was sys_vm86 */
	sys	sys_wait4		4
	sys	sys_swapoff		1	/* 4115 */
	sys	sys_sysinfo		1
	sys	sys_ipc			6
	sys	sys_fsync		1
	sys	sys_sigreturn		0
	sys	sys_clone		0	/* 4120 */
	sys	sys_setdomainname	2
	sys	sys_newuname		1
	sys	sys_ni_syscall		0	/* sys_modify_ldt */
	sys	sys_adjtimex		1
	sys	sys_mprotect		3	/* 4125 */
	sys	sys_sigprocmask		3
	sys	sys_ni_syscall		0	/* was create_module */
	sys	sys_init_module		5
	sys	sys_delete_module	1
	sys	sys_ni_syscall		0	/* 4130	was get_kernel_syms */
	sys	sys_quotactl		4
	sys	sys_getpgid		1
	sys	sys_fchdir		1
	sys	sys_bdflush		2
	sys	sys_sysfs		3	/* 4135 */
	sys	sys_personality		1
	sys	sys_ni_syscall		0	/* for afs_syscall */
	sys	sys_setfsuid		1
	sys	sys_setfsgid		1
	sys	sys_llseek		5	/* 4140 */
	sys	sys_getdents		3
	sys	sys_select		5
	sys	sys_flock		2
	sys	sys_msync		3
	sys	sys_readv		3	/* 4145 */
	sys	sys_writev		3
	sys	sys_cacheflush		3
	sys	sys_cachectl		3
	sys	sys_sysmips		4
	sys	sys_ni_syscall		0	/* 4150 */
	sys	sys_getsid		1
	sys	sys_fdatasync		1
	sys	sys_sysctl		1
	sys	sys_mlock		2
	sys	sys_munlock		2	/* 4155 */
	sys	sys_mlockall		1
	sys	sys_munlockall		0
	sys	sys_sched_setparam	2
	sys	sys_sched_getparam	2
	sys	sys_sched_setscheduler	3	/* 4160 */
	sys	sys_sched_getscheduler	1
	sys	sys_sched_yield		0
	sys	sys_sched_get_priority_max 1
	sys	sys_sched_get_priority_min 1
	sys	sys_sched_rr_get_interval 2	/* 4165 */
	sys	sys_nanosleep,		2
	sys	sys_mremap,		5
	sys	sys_accept		3
	sys	sys_bind		3
	sys	sys_connect		3	/* 4170 */
	sys	sys_getpeername		3
	sys	sys_getsockname		3
	sys	sys_getsockopt		5
	sys	sys_listen		2
	sys	sys_recv		4	/* 4175 */
	sys	sys_recvfrom		6
	sys	sys_recvmsg		3
	sys	sys_send		4
	sys	sys_sendmsg		3
	sys	sys_sendto		6	/* 4180 */
	sys	sys_setsockopt		5
	sys	sys_shutdown		2
	sys	sys_socket		3
	sys	sys_socketpair		4
	sys	sys_setresuid		3	/* 4185 */
	sys	sys_getresuid		3
	sys	sys_ni_syscall		0	/* was sys_query_module */
	sys	sys_poll		3
<<<<<<< HEAD
	sys	sys_nfsservctl		3
=======
	sys	sys_ni_syscall		0	/* was nfsservctl */
>>>>>>> refs/remotes/origin/cm-10.0
	sys	sys_setresgid		3	/* 4190 */
	sys	sys_getresgid		3
	sys	sys_prctl		5
	sys	sys_rt_sigreturn	0
	sys	sys_rt_sigaction	4
	sys	sys_rt_sigprocmask	4	/* 4195 */
	sys	sys_rt_sigpending	2
	sys	sys_rt_sigtimedwait	4
	sys	sys_rt_sigqueueinfo	3
	sys	sys_rt_sigsuspend	0
	sys	sys_pread64		6	/* 4200 */
	sys	sys_pwrite64		6
	sys	sys_chown		3
	sys	sys_getcwd		2
	sys	sys_capget		2
	sys	sys_capset		2	/* 4205 */
	sys	sys_sigaltstack		0
	sys	sys_sendfile		4
	sys	sys_ni_syscall		0
	sys	sys_ni_syscall		0
	sys	sys_mips_mmap2		6	/* 4210 */
	sys	sys_truncate64		4
	sys	sys_ftruncate64		4
	sys	sys_stat64		2
	sys	sys_lstat64		2
	sys	sys_fstat64		2	/* 4215 */
	sys	sys_pivot_root		2
	sys	sys_mincore		3
	sys	sys_madvise		3
	sys	sys_getdents64		3
	sys	sys_fcntl64		3	/* 4220 */
	sys	sys_ni_syscall		0
	sys	sys_gettid		0
	sys	sys_readahead		5
	sys	sys_setxattr		5
	sys	sys_lsetxattr		5	/* 4225 */
	sys	sys_fsetxattr		5
	sys	sys_getxattr		4
	sys	sys_lgetxattr		4
	sys	sys_fgetxattr		4
	sys	sys_listxattr		3	/* 4230 */
	sys	sys_llistxattr		3
	sys	sys_flistxattr		3
	sys	sys_removexattr		2
	sys	sys_lremovexattr	2
	sys	sys_fremovexattr	2	/* 4235 */
	sys	sys_tkill		2
	sys	sys_sendfile64		5
	sys	sys_futex		6
=======
einval: li	v0, -ENOSYS
	jr	ra
	END(sys_syscall)

	.align	2
	.type	sys_call_table, @object
EXPORT(sys_call_table)
	PTR	sys_syscall			/* 4000 */
	PTR	sys_exit
	PTR	__sys_fork
	PTR	sys_read
	PTR	sys_write
	PTR	sys_open			/* 4005 */
	PTR	sys_close
	PTR	sys_waitpid
	PTR	sys_creat
	PTR	sys_link
	PTR	sys_unlink			/* 4010 */
	PTR	sys_execve
	PTR	sys_chdir
	PTR	sys_time
	PTR	sys_mknod
	PTR	sys_chmod			/* 4015 */
	PTR	sys_lchown
	PTR	sys_ni_syscall
	PTR	sys_ni_syscall			/* was sys_stat */
	PTR	sys_lseek
	PTR	sys_getpid			/* 4020 */
	PTR	sys_mount
	PTR	sys_oldumount
	PTR	sys_setuid
	PTR	sys_getuid
	PTR	sys_stime			/* 4025 */
	PTR	sys_ptrace
	PTR	sys_alarm
	PTR	sys_ni_syscall			/* was sys_fstat */
	PTR	sys_pause
	PTR	sys_utime			/* 4030 */
	PTR	sys_ni_syscall
	PTR	sys_ni_syscall
	PTR	sys_access
	PTR	sys_nice
	PTR	sys_ni_syscall			/* 4035 */
	PTR	sys_sync
	PTR	sys_kill
	PTR	sys_rename
	PTR	sys_mkdir
	PTR	sys_rmdir			/* 4040 */
	PTR	sys_dup
	PTR	sysm_pipe
	PTR	sys_times
	PTR	sys_ni_syscall
	PTR	sys_brk				/* 4045 */
	PTR	sys_setgid
	PTR	sys_getgid
	PTR	sys_ni_syscall			/* was signal(2) */
	PTR	sys_geteuid
	PTR	sys_getegid			/* 4050 */
	PTR	sys_acct
	PTR	sys_umount
	PTR	sys_ni_syscall
	PTR	sys_ioctl
	PTR	sys_fcntl			/* 4055 */
	PTR	sys_ni_syscall
	PTR	sys_setpgid
	PTR	sys_ni_syscall
	PTR	sys_olduname
	PTR	sys_umask			/* 4060 */
	PTR	sys_chroot
	PTR	sys_ustat
	PTR	sys_dup2
	PTR	sys_getppid
	PTR	sys_getpgrp			/* 4065 */
	PTR	sys_setsid
	PTR	sys_sigaction
	PTR	sys_sgetmask
	PTR	sys_ssetmask
	PTR	sys_setreuid			/* 4070 */
	PTR	sys_setregid
	PTR	sys_sigsuspend
	PTR	sys_sigpending
	PTR	sys_sethostname
	PTR	sys_setrlimit			/* 4075 */
	PTR	sys_getrlimit
	PTR	sys_getrusage
	PTR	sys_gettimeofday
	PTR	sys_settimeofday
	PTR	sys_getgroups			/* 4080 */
	PTR	sys_setgroups
	PTR	sys_ni_syscall			/* old_select */
	PTR	sys_symlink
	PTR	sys_ni_syscall			/* was sys_lstat */
	PTR	sys_readlink			/* 4085 */
	PTR	sys_uselib
	PTR	sys_swapon
	PTR	sys_reboot
	PTR	sys_old_readdir
	PTR	sys_mips_mmap			/* 4090 */
	PTR	sys_munmap
	PTR	sys_truncate
	PTR	sys_ftruncate
	PTR	sys_fchmod
	PTR	sys_fchown			/* 4095 */
	PTR	sys_getpriority
	PTR	sys_setpriority
	PTR	sys_ni_syscall
	PTR	sys_statfs
	PTR	sys_fstatfs			/* 4100 */
	PTR	sys_ni_syscall			/* was ioperm(2) */
	PTR	sys_socketcall
	PTR	sys_syslog
	PTR	sys_setitimer
	PTR	sys_getitimer			/* 4105 */
	PTR	sys_newstat
	PTR	sys_newlstat
	PTR	sys_newfstat
	PTR	sys_uname
	PTR	sys_ni_syscall			/* 4110 was iopl(2) */
	PTR	sys_vhangup
	PTR	sys_ni_syscall			/* was sys_idle() */
	PTR	sys_ni_syscall			/* was sys_vm86 */
	PTR	sys_wait4
	PTR	sys_swapoff			/* 4115 */
	PTR	sys_sysinfo
	PTR	sys_ipc
	PTR	sys_fsync
	PTR	sys_sigreturn
	PTR	__sys_clone			/* 4120 */
	PTR	sys_setdomainname
	PTR	sys_newuname
	PTR	sys_ni_syscall			/* sys_modify_ldt */
	PTR	sys_adjtimex
	PTR	sys_mprotect			/* 4125 */
	PTR	sys_sigprocmask
	PTR	sys_ni_syscall			/* was create_module */
	PTR	sys_init_module
	PTR	sys_delete_module
	PTR	sys_ni_syscall			/* 4130 was get_kernel_syms */
	PTR	sys_quotactl
	PTR	sys_getpgid
	PTR	sys_fchdir
	PTR	sys_bdflush
	PTR	sys_sysfs			/* 4135 */
	PTR	sys_personality
	PTR	sys_ni_syscall			/* for afs_syscall */
	PTR	sys_setfsuid
	PTR	sys_setfsgid
	PTR	sys_llseek			/* 4140 */
	PTR	sys_getdents
	PTR	sys_select
	PTR	sys_flock
	PTR	sys_msync
	PTR	sys_readv			/* 4145 */
	PTR	sys_writev
	PTR	sys_cacheflush
	PTR	sys_cachectl
	PTR	sys_sysmips
	PTR	sys_ni_syscall			/* 4150 */
	PTR	sys_getsid
	PTR	sys_fdatasync
	PTR	sys_sysctl
	PTR	sys_mlock
	PTR	sys_munlock			/* 4155 */
	PTR	sys_mlockall
	PTR	sys_munlockall
	PTR	sys_sched_setparam
	PTR	sys_sched_getparam
	PTR	sys_sched_setscheduler		/* 4160 */
	PTR	sys_sched_getscheduler
	PTR	sys_sched_yield
	PTR	sys_sched_get_priority_max
	PTR	sys_sched_get_priority_min
	PTR	sys_sched_rr_get_interval	/* 4165 */
	PTR	sys_nanosleep
	PTR	sys_mremap
	PTR	sys_accept
	PTR	sys_bind
	PTR	sys_connect			/* 4170 */
	PTR	sys_getpeername
	PTR	sys_getsockname
	PTR	sys_getsockopt
	PTR	sys_listen
	PTR	sys_recv			/* 4175 */
	PTR	sys_recvfrom
	PTR	sys_recvmsg
	PTR	sys_send
	PTR	sys_sendmsg
	PTR	sys_sendto			/* 4180 */
	PTR	sys_setsockopt
	PTR	sys_shutdown
	PTR	sys_socket
	PTR	sys_socketpair
	PTR	sys_setresuid			/* 4185 */
	PTR	sys_getresuid
	PTR	sys_ni_syscall			/* was sys_query_module */
	PTR	sys_poll
	PTR	sys_ni_syscall			/* was nfsservctl */
	PTR	sys_setresgid			/* 4190 */
	PTR	sys_getresgid
	PTR	sys_prctl
	PTR	sys_rt_sigreturn
	PTR	sys_rt_sigaction
	PTR	sys_rt_sigprocmask		/* 4195 */
	PTR	sys_rt_sigpending
	PTR	sys_rt_sigtimedwait
	PTR	sys_rt_sigqueueinfo
	PTR	sys_rt_sigsuspend
	PTR	sys_pread64			/* 4200 */
	PTR	sys_pwrite64
	PTR	sys_chown
	PTR	sys_getcwd
	PTR	sys_capget
	PTR	sys_capset			/* 4205 */
	PTR	sys_sigaltstack
	PTR	sys_sendfile
	PTR	sys_ni_syscall
	PTR	sys_ni_syscall
	PTR	sys_mips_mmap2			/* 4210 */
	PTR	sys_truncate64
	PTR	sys_ftruncate64
	PTR	sys_stat64
	PTR	sys_lstat64
	PTR	sys_fstat64			/* 4215 */
	PTR	sys_pivot_root
	PTR	sys_mincore
	PTR	sys_madvise
	PTR	sys_getdents64
	PTR	sys_fcntl64			/* 4220 */
	PTR	sys_ni_syscall
	PTR	sys_gettid
	PTR	sys_readahead
	PTR	sys_setxattr
	PTR	sys_lsetxattr			/* 4225 */
	PTR	sys_fsetxattr
	PTR	sys_getxattr
	PTR	sys_lgetxattr
	PTR	sys_fgetxattr
	PTR	sys_listxattr			/* 4230 */
	PTR	sys_llistxattr
	PTR	sys_flistxattr
	PTR	sys_removexattr
	PTR	sys_lremovexattr
	PTR	sys_fremovexattr		/* 4235 */
	PTR	sys_tkill
	PTR	sys_sendfile64
	PTR	sys_futex
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_MIPS_MT_FPAFF
	/*
	 * For FPU affinity scheduling on MIPS MT processors, we need to
	 * intercept sys_sched_xxxaffinity() calls until we get a proper hook
<<<<<<< HEAD
	 * in kernel/sched.c.  Considered only temporary we only support these
	 * hooks for the 32-bit kernel - there is no MIPS64 MT processor atm.
	 */
	sys	mipsmt_sys_sched_setaffinity	3
	sys	mipsmt_sys_sched_getaffinity	3
#else
	sys	sys_sched_setaffinity	3
	sys	sys_sched_getaffinity	3	/* 4240 */
#endif /* CONFIG_MIPS_MT_FPAFF */
	sys	sys_io_setup		2
	sys	sys_io_destroy		1
	sys	sys_io_getevents	5
	sys	sys_io_submit		3
	sys	sys_io_cancel		3	/* 4245 */
	sys	sys_exit_group		1
	sys	sys_lookup_dcookie	4
	sys	sys_epoll_create	1
	sys	sys_epoll_ctl		4
<<<<<<< HEAD
	sys	sys_epoll_wait		3	/* 4250 */
=======
	sys	sys_epoll_wait		4	/* 4250 */
>>>>>>> refs/remotes/origin/cm-10.0
	sys	sys_remap_file_pages	5
	sys	sys_set_tid_address	1
	sys	sys_restart_syscall	0
	sys	sys_fadvise64_64	7
	sys	sys_statfs64		3	/* 4255 */
	sys	sys_fstatfs64		2
	sys	sys_timer_create	3
	sys	sys_timer_settime	4
	sys	sys_timer_gettime	2
	sys	sys_timer_getoverrun	1	/* 4260 */
	sys	sys_timer_delete	1
	sys	sys_clock_settime	2
	sys	sys_clock_gettime	2
	sys	sys_clock_getres	2
	sys	sys_clock_nanosleep	4	/* 4265 */
	sys	sys_tgkill		3
	sys	sys_utimes		2
	sys	sys_mbind		4
	sys	sys_ni_syscall		0	/* sys_get_mempolicy */
	sys	sys_ni_syscall		0	/* 4270 sys_set_mempolicy */
	sys	sys_mq_open		4
	sys	sys_mq_unlink		1
	sys	sys_mq_timedsend	5
	sys	sys_mq_timedreceive	5
	sys	sys_mq_notify		2	/* 4275 */
	sys	sys_mq_getsetattr	3
	sys	sys_ni_syscall		0	/* sys_vserver */
	sys	sys_waitid		5
	sys	sys_ni_syscall		0	/* available, was setaltroot */
	sys	sys_add_key		5	/* 4280 */
	sys	sys_request_key		4
	sys	sys_keyctl		5
	sys	sys_set_thread_area	1
	sys	sys_inotify_init	0
	sys	sys_inotify_add_watch	3	/* 4285 */
	sys	sys_inotify_rm_watch	2
	sys	sys_migrate_pages	4
	sys	sys_openat		4
	sys	sys_mkdirat		3
	sys	sys_mknodat		4	/* 4290 */
	sys	sys_fchownat		5
	sys	sys_futimesat		3
	sys	sys_fstatat64		4
	sys	sys_unlinkat		3
	sys	sys_renameat		4	/* 4295 */
	sys	sys_linkat		5
	sys	sys_symlinkat		3
	sys	sys_readlinkat		4
	sys	sys_fchmodat		3
	sys	sys_faccessat		3	/* 4300 */
	sys	sys_pselect6		6
	sys	sys_ppoll		5
	sys	sys_unshare		1
	sys	sys_splice		6
	sys	sys_sync_file_range	7	/* 4305 */
	sys	sys_tee			4
	sys	sys_vmsplice		4
	sys	sys_move_pages		6
	sys	sys_set_robust_list	2
	sys	sys_get_robust_list	3	/* 4310 */
	sys	sys_kexec_load		4
	sys	sys_getcpu		3
	sys	sys_epoll_pwait		6
	sys	sys_ioprio_set		3
	sys	sys_ioprio_get		2	/* 4315 */
	sys	sys_utimensat		4
	sys	sys_signalfd		3
	sys	sys_ni_syscall		0	/* was timerfd */
	sys	sys_eventfd		1
	sys	sys_fallocate		6	/* 4320 */
	sys	sys_timerfd_create	2
	sys	sys_timerfd_gettime	2
	sys	sys_timerfd_settime	4
	sys	sys_signalfd4		4
	sys	sys_eventfd2		2	/* 4325 */
	sys	sys_epoll_create1	1
	sys	sys_dup3		3
	sys	sys_pipe2		2
	sys	sys_inotify_init1	1
	sys	sys_preadv		6	/* 4330 */
	sys	sys_pwritev		6
	sys	sys_rt_tgsigqueueinfo	4
	sys	sys_perf_event_open	5
	sys	sys_accept4		4
	sys	sys_recvmmsg		5	/* 4335 */
	sys	sys_fanotify_init	2
	sys	sys_fanotify_mark	6
	sys	sys_prlimit64		4
	sys	sys_name_to_handle_at	5
	sys	sys_open_by_handle_at	3	/* 4340 */
	sys	sys_clock_adjtime	2
	sys	sys_syncfs		1
<<<<<<< HEAD
	sys	sys_setns		2
=======
	sys	sys_sendmmsg		4
	sys	sys_setns		2
	sys	sys_process_vm_readv	6	/* 4345 */
	sys	sys_process_vm_writev	6
>>>>>>> refs/remotes/origin/cm-10.0
	.endm

	/* We pre-compute the number of _instruction_ bytes needed to
	   load or store the arguments 6-8. Negative values are ignored. */

	.macro  sys function, nargs
	PTR	\function
	LONG	(\nargs << 2) - (5 << 2)
	.endm

	.align	3
	.type	sys_call_table,@object
EXPORT(sys_call_table)
	syscalltable
	.size	sys_call_table, . - sys_call_table
=======
	 * in kernel/sched/core.c.  Considered only temporary we only support
	 * these hooks for the 32-bit kernel - there is no MIPS64 MT processor
	 * atm.
	 */
	PTR	mipsmt_sys_sched_setaffinity
	PTR	mipsmt_sys_sched_getaffinity
#else
	PTR	sys_sched_setaffinity
	PTR	sys_sched_getaffinity		/* 4240 */
#endif /* CONFIG_MIPS_MT_FPAFF */
	PTR	sys_io_setup
	PTR	sys_io_destroy
	PTR	sys_io_getevents
	PTR	sys_io_submit
	PTR	sys_io_cancel			/* 4245 */
	PTR	sys_exit_group
	PTR	sys_lookup_dcookie
	PTR	sys_epoll_create
	PTR	sys_epoll_ctl
	PTR	sys_epoll_wait			/* 4250 */
	PTR	sys_remap_file_pages
	PTR	sys_set_tid_address
	PTR	sys_restart_syscall
	PTR	sys_fadvise64_64
	PTR	sys_statfs64			/* 4255 */
	PTR	sys_fstatfs64
	PTR	sys_timer_create
	PTR	sys_timer_settime
	PTR	sys_timer_gettime
	PTR	sys_timer_getoverrun		/* 4260 */
	PTR	sys_timer_delete
	PTR	sys_clock_settime
	PTR	sys_clock_gettime
	PTR	sys_clock_getres
	PTR	sys_clock_nanosleep		/* 4265 */
	PTR	sys_tgkill
	PTR	sys_utimes
	PTR	sys_mbind
	PTR	sys_ni_syscall			/* sys_get_mempolicy */
	PTR	sys_ni_syscall			/* 4270 sys_set_mempolicy */
	PTR	sys_mq_open
	PTR	sys_mq_unlink
	PTR	sys_mq_timedsend
	PTR	sys_mq_timedreceive
	PTR	sys_mq_notify			/* 4275 */
	PTR	sys_mq_getsetattr
	PTR	sys_ni_syscall			/* sys_vserver */
	PTR	sys_waitid
	PTR	sys_ni_syscall			/* available, was setaltroot */
	PTR	sys_add_key			/* 4280 */
	PTR	sys_request_key
	PTR	sys_keyctl
	PTR	sys_set_thread_area
	PTR	sys_inotify_init
	PTR	sys_inotify_add_watch		/* 4285 */
	PTR	sys_inotify_rm_watch
	PTR	sys_migrate_pages
	PTR	sys_openat
	PTR	sys_mkdirat
	PTR	sys_mknodat			/* 4290 */
	PTR	sys_fchownat
	PTR	sys_futimesat
	PTR	sys_fstatat64
	PTR	sys_unlinkat
	PTR	sys_renameat			/* 4295 */
	PTR	sys_linkat
	PTR	sys_symlinkat
	PTR	sys_readlinkat
	PTR	sys_fchmodat
	PTR	sys_faccessat			/* 4300 */
	PTR	sys_pselect6
	PTR	sys_ppoll
	PTR	sys_unshare
	PTR	sys_splice
	PTR	sys_sync_file_range		/* 4305 */
	PTR	sys_tee
	PTR	sys_vmsplice
	PTR	sys_move_pages
	PTR	sys_set_robust_list
	PTR	sys_get_robust_list		/* 4310 */
	PTR	sys_kexec_load
	PTR	sys_getcpu
	PTR	sys_epoll_pwait
	PTR	sys_ioprio_set
	PTR	sys_ioprio_get			/* 4315 */
	PTR	sys_utimensat
	PTR	sys_signalfd
	PTR	sys_ni_syscall			/* was timerfd */
	PTR	sys_eventfd
	PTR	sys_fallocate			/* 4320 */
	PTR	sys_timerfd_create
	PTR	sys_timerfd_gettime
	PTR	sys_timerfd_settime
	PTR	sys_signalfd4
	PTR	sys_eventfd2			/* 4325 */
	PTR	sys_epoll_create1
	PTR	sys_dup3
	PTR	sys_pipe2
	PTR	sys_inotify_init1
	PTR	sys_preadv			/* 4330 */
	PTR	sys_pwritev
	PTR	sys_rt_tgsigqueueinfo
	PTR	sys_perf_event_open
	PTR	sys_accept4
	PTR	sys_recvmmsg			/* 4335 */
	PTR	sys_fanotify_init
	PTR	sys_fanotify_mark
	PTR	sys_prlimit64
	PTR	sys_name_to_handle_at
	PTR	sys_open_by_handle_at		/* 4340 */
	PTR	sys_clock_adjtime
	PTR	sys_syncfs
	PTR	sys_sendmmsg
	PTR	sys_setns
	PTR	sys_process_vm_readv		/* 4345 */
	PTR	sys_process_vm_writev
	PTR	sys_kcmp
	PTR	sys_finit_module
>>>>>>> refs/remotes/origin/master
