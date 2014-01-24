/*
 * syscalls.h - Linux syscall interfaces (arch-specific)
 *
 * Copyright (c) 2008 Jaswinder Singh Rajput
 *
 * This file is released under the GPLv2.
 * See the file COPYING for more details.
 */

#ifndef _ASM_X86_SYSCALLS_H
#define _ASM_X86_SYSCALLS_H

#include <linux/compiler.h>
#include <linux/linkage.h>
#include <linux/signal.h>
#include <linux/types.h>

/* Common in X86_32 and X86_64 */
/* kernel/ioport.c */
asmlinkage long sys_ioperm(unsigned long, unsigned long, int);
<<<<<<< HEAD
long sys_iopl(unsigned int, struct pt_regs *);

/* kernel/process.c */
int sys_fork(struct pt_regs *);
int sys_vfork(struct pt_regs *);
long sys_execve(const char __user *,
		const char __user *const __user *,
		const char __user *const __user *, struct pt_regs *);
long sys_clone(unsigned long, unsigned long, void __user *,
	       void __user *, struct pt_regs *);
=======
asmlinkage long sys_iopl(unsigned int);
>>>>>>> refs/remotes/origin/master

/* kernel/ldt.c */
asmlinkage int sys_modify_ldt(int, void __user *, unsigned long);

/* kernel/signal.c */
<<<<<<< HEAD
long sys_rt_sigreturn(struct pt_regs *);
long sys_sigaltstack(const stack_t __user *, stack_t __user *,
		     struct pt_regs *);


/* kernel/tls.c */
asmlinkage int sys_set_thread_area(struct user_desc __user *);
asmlinkage int sys_get_thread_area(struct user_desc __user *);
=======
asmlinkage long sys_rt_sigreturn(void);

/* kernel/tls.c */
asmlinkage long sys_set_thread_area(struct user_desc __user *);
asmlinkage long sys_get_thread_area(struct user_desc __user *);
>>>>>>> refs/remotes/origin/master

/* X86_32 only */
#ifdef CONFIG_X86_32

/* kernel/signal.c */
<<<<<<< HEAD
asmlinkage int sys_sigsuspend(int, int, old_sigset_t);
asmlinkage int sys_sigaction(int, const struct old_sigaction __user *,
			     struct old_sigaction __user *);
unsigned long sys_sigreturn(struct pt_regs *);

/* kernel/vm86_32.c */
int sys_vm86old(struct vm86_struct __user *, struct pt_regs *);
int sys_vm86(unsigned long, unsigned long, struct pt_regs *);
=======
asmlinkage unsigned long sys_sigreturn(void);

/* kernel/vm86_32.c */
asmlinkage long sys_vm86old(struct vm86_struct __user *);
asmlinkage long sys_vm86(unsigned long, unsigned long);
>>>>>>> refs/remotes/origin/master

#else /* CONFIG_X86_32 */

/* X86_64 only */
/* kernel/process_64.c */
<<<<<<< HEAD
long sys_arch_prctl(int, unsigned long);
=======
asmlinkage long sys_arch_prctl(int, unsigned long);
>>>>>>> refs/remotes/origin/master

/* kernel/sys_x86_64.c */
asmlinkage long sys_mmap(unsigned long, unsigned long, unsigned long,
			 unsigned long, unsigned long, unsigned long);

#endif /* CONFIG_X86_32 */
#endif /* _ASM_X86_SYSCALLS_H */
