#ifndef _SPARC64_SYSCALLS_H
#define _SPARC64_SYSCALLS_H

struct pt_regs;

extern asmlinkage long sparc_do_fork(unsigned long clone_flags,
				     unsigned long stack_start,
				     struct pt_regs *regs,
				     unsigned long stack_size);

<<<<<<< HEAD
extern asmlinkage int sparc_execve(struct pt_regs *regs);

=======
>>>>>>> refs/remotes/origin/master
#endif /* _SPARC64_SYSCALLS_H */
