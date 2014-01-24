#ifndef _ASM_X86_FTRACE_H
#define _ASM_X86_FTRACE_H

#ifdef __ASSEMBLY__

<<<<<<< HEAD
	.macro MCOUNT_SAVE_FRAME
	/* taken from glibc */
	subq $0x38, %rsp
	movq %rax, (%rsp)
	movq %rcx, 8(%rsp)
	movq %rdx, 16(%rsp)
	movq %rsi, 24(%rsp)
	movq %rdi, 32(%rsp)
	movq %r8, 40(%rsp)
	movq %r9, 48(%rsp)
	.endm

	.macro MCOUNT_RESTORE_FRAME
	movq 48(%rsp), %r9
	movq 40(%rsp), %r8
	movq 32(%rsp), %rdi
	movq 24(%rsp), %rsi
	movq 16(%rsp), %rdx
	movq 8(%rsp), %rcx
	movq (%rsp), %rax
	addq $0x38, %rsp
=======
	/* skip is set if the stack was already partially adjusted */
	.macro MCOUNT_SAVE_FRAME skip=0
	 /*
	  * We add enough stack to save all regs.
	  */
	subq $(SS+8-\skip), %rsp
	movq %rax, RAX(%rsp)
	movq %rcx, RCX(%rsp)
	movq %rdx, RDX(%rsp)
	movq %rsi, RSI(%rsp)
	movq %rdi, RDI(%rsp)
	movq %r8, R8(%rsp)
	movq %r9, R9(%rsp)
	 /* Move RIP to its proper location */
	movq SS+8(%rsp), %rdx
	movq %rdx, RIP(%rsp)
	.endm

	.macro MCOUNT_RESTORE_FRAME skip=0
	movq R9(%rsp), %r9
	movq R8(%rsp), %r8
	movq RDI(%rsp), %rdi
	movq RSI(%rsp), %rsi
	movq RDX(%rsp), %rdx
	movq RCX(%rsp), %rcx
	movq RAX(%rsp), %rax
	addq $(SS+8-\skip), %rsp
>>>>>>> refs/remotes/origin/master
	.endm

#endif

#ifdef CONFIG_FUNCTION_TRACER
<<<<<<< HEAD
#define MCOUNT_ADDR		((long)(mcount))
#define MCOUNT_INSN_SIZE	5 /* sizeof mcount call */

#ifndef __ASSEMBLY__
extern void mcount(void);
=======
#ifdef CC_USING_FENTRY
# define MCOUNT_ADDR		((long)(__fentry__))
#else
# define MCOUNT_ADDR		((long)(mcount))
#endif
#define MCOUNT_INSN_SIZE	5 /* sizeof mcount call */

#ifdef CONFIG_DYNAMIC_FTRACE
#define ARCH_SUPPORTS_FTRACE_OPS 1
#endif

#ifndef __ASSEMBLY__
extern void mcount(void);
extern atomic_t modifying_ftrace_code;
extern void __fentry__(void);
>>>>>>> refs/remotes/origin/master

static inline unsigned long ftrace_call_adjust(unsigned long addr)
{
	/*
	 * addr is the address of the mcount call instruction.
	 * recordmcount does the necessary offset calculation.
	 */
	return addr;
}

#ifdef CONFIG_DYNAMIC_FTRACE

struct dyn_arch_ftrace {
	/* No extra data needed for x86 */
};

<<<<<<< HEAD
=======
int ftrace_int3_handler(struct pt_regs *regs);

>>>>>>> refs/remotes/origin/master
#endif /*  CONFIG_DYNAMIC_FTRACE */
#endif /* __ASSEMBLY__ */
#endif /* CONFIG_FUNCTION_TRACER */

<<<<<<< HEAD
=======

#if !defined(__ASSEMBLY__) && !defined(COMPILE_OFFSETS)

#if defined(CONFIG_FTRACE_SYSCALLS) && defined(CONFIG_IA32_EMULATION)
#include <asm/compat.h>

/*
 * Because ia32 syscalls do not map to x86_64 syscall numbers
 * this screws up the trace output when tracing a ia32 task.
 * Instead of reporting bogus syscalls, just do not trace them.
 *
 * If the user realy wants these, then they should use the
 * raw syscall tracepoints with filtering.
 */
#define ARCH_TRACE_IGNORE_COMPAT_SYSCALLS 1
static inline bool arch_trace_is_compat_syscall(struct pt_regs *regs)
{
	if (is_compat_task())
		return true;
	return false;
}
#endif /* CONFIG_FTRACE_SYSCALLS && CONFIG_IA32_EMULATION */
#endif /* !__ASSEMBLY__  && !COMPILE_OFFSETS */

>>>>>>> refs/remotes/origin/master
#endif /* _ASM_X86_FTRACE_H */
