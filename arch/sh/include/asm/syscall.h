#ifndef __ASM_SH_SYSCALL_H
#define __ASM_SH_SYSCALL_H

extern const unsigned long sys_call_table[];

#ifdef CONFIG_SUPERH32
<<<<<<< HEAD
# include "syscall_32.h"
#else
# include "syscall_64.h"
=======
# include <asm/syscall_32.h>
#else
# include <asm/syscall_64.h>
>>>>>>> refs/remotes/origin/master
#endif

#endif /* __ASM_SH_SYSCALL_H */
