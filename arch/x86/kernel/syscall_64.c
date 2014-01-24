/* System call table for x86-64. */

#include <linux/linkage.h>
#include <linux/sys.h>
#include <linux/cache.h>
#include <asm/asm-offsets.h>
<<<<<<< HEAD

<<<<<<< HEAD
#define __NO_STUBS

#define __SYSCALL(nr, sym) extern asmlinkage void sym(void) ;
#undef _ASM_X86_UNISTD_64_H
#include <asm/unistd_64.h>

#undef __SYSCALL
#define __SYSCALL(nr, sym) [nr] = sym,
#undef _ASM_X86_UNISTD_64_H
=======
=======
#include <asm/syscall.h>

>>>>>>> refs/remotes/origin/master
#define __SYSCALL_COMMON(nr, sym, compat) __SYSCALL_64(nr, sym, compat)

#ifdef CONFIG_X86_X32_ABI
# define __SYSCALL_X32(nr, sym, compat) __SYSCALL_64(nr, sym, compat)
#else
# define __SYSCALL_X32(nr, sym, compat) /* nothing */
#endif

#define __SYSCALL_64(nr, sym, compat) extern asmlinkage void sym(void) ;
#include <asm/syscalls_64.h>
#undef __SYSCALL_64

#define __SYSCALL_64(nr, sym, compat) [nr] = sym,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

typedef void (*sys_call_ptr_t)(void);

extern void sys_ni_syscall(void);

const sys_call_ptr_t sys_call_table[__NR_syscall_max+1] = {
	/*
<<<<<<< HEAD
	*Smells like a like a compiler bug -- it doesn't work
	*when the & below is removed.
	*/
	[0 ... __NR_syscall_max] = &sys_ni_syscall,
#include <asm/unistd_64.h>
=======
=======

extern void sys_ni_syscall(void);

asmlinkage const sys_call_ptr_t sys_call_table[__NR_syscall_max+1] = {
	/*
>>>>>>> refs/remotes/origin/master
	 * Smells like a compiler bug -- it doesn't work
	 * when the & below is removed.
	 */
	[0 ... __NR_syscall_max] = &sys_ni_syscall,
#include <asm/syscalls_64.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};
