#ifndef _CRIS_PTRACE_H
#define _CRIS_PTRACE_H

<<<<<<< HEAD
#include <arch/ptrace.h>

#ifdef __KERNEL__
=======
#include <uapi/asm/ptrace.h>

>>>>>>> refs/remotes/origin/master

/* Arbitrarily choose the same ptrace numbers as used by the Sparc code. */
#define PTRACE_GETREGS            12
#define PTRACE_SETREGS            13

#define profile_pc(regs) instruction_pointer(regs)
<<<<<<< HEAD

#endif /* __KERNEL__ */
=======
#define current_user_stack_pointer() rdusp()
>>>>>>> refs/remotes/origin/master

#endif /* _CRIS_PTRACE_H */
