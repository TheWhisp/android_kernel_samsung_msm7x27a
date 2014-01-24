/*
 *  linux/arch/arm/kernel/entry-armv.S
 *
 *  Copyright (C) 1996,1997,1998 Russell King.
 *  ARM700 fix by Matthew Godbolt (linux-user@willothewisp.demon.co.uk)
 *  nommu support by Hyok S. Choi (hyok.choi@samsung.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Low-level vector interface routines
 *
 *  Note:  there is a StrongARM bug in the STMIA rn, {regs}^ instruction
 *  that causes it to save wrong values...  Be aware!
 */

<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <asm/assembler.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm/assembler.h>
>>>>>>> refs/remotes/origin/master
#include <asm/memory.h>
#include <asm/glue-df.h>
#include <asm/glue-pf.h>
#include <asm/vfpmacros.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <mach/entry-macro.S>
=======
#ifndef CONFIG_MULTI_IRQ_HANDLER
#include <mach/entry-macro.S>
#endif
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifndef CONFIG_MULTI_IRQ_HANDLER
#include <mach/entry-macro.S>
#endif
>>>>>>> refs/remotes/origin/master
#include <asm/thread_notify.h>
#include <asm/unwind.h>
#include <asm/unistd.h>
#include <asm/tls.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <asm/system_info.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <asm/system_info.h>
>>>>>>> refs/remotes/origin/master

#include "entry-header.S"
#include <asm/entry-macro-multi.S>

/*
 * Interrupt handling.
 */
	.macro	irq_handler
#ifdef CONFIG_MULTI_IRQ_HANDLER
	ldr	r1, =handle_arch_irq
	mov	r0, sp
	adr	lr, BSYM(9997f)
	ldr	pc, [r1]
#else
	arch_irq_handler_default
#endif
9997:
	.endm

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	.macro	pabt_helper
	@ PABORT handler takes pt_regs in r2, fault address in r4 and psr in r5
#ifdef MULTI_PABORT
	ldr	ip, .LCprocfns
	mov	lr, pc
	ldr	pc, [ip, #PROCESSOR_PABT_FUNC]
#else
	bl	CPU_PABORT_HANDLER
#endif
	.endm

	.macro	dabt_helper

	@
	@ Call the processor-specific abort handler:
	@
	@  r2 - pt_regs
	@  r4 - aborted context pc
	@  r5 - aborted context psr
	@
	@ The abort handler must return the aborted address in r0, and
	@ the fault status register in r1.  r9 must be preserved.
	@
#ifdef MULTI_DABORT
	ldr	ip, .LCprocfns
	mov	lr, pc
	ldr	pc, [ip, #PROCESSOR_DABT_FUNC]
#else
	bl	CPU_DABORT_HANDLER
#endif
	.endm

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_KPROBES
	.section	.kprobes.text,"ax",%progbits
#else
	.text
#endif

/*
 * Invalid mode handlers
 */
	.macro	inv_entry, reason
	sub	sp, sp, #S_FRAME_SIZE
 ARM(	stmib	sp, {r1 - lr}		)
 THUMB(	stmia	sp, {r0 - r12}		)
 THUMB(	str	sp, [sp, #S_SP]		)
 THUMB(	str	lr, [sp, #S_LR]		)
	mov	r1, #\reason
	.endm

__pabt_invalid:
	inv_entry BAD_PREFETCH
	b	common_invalid
ENDPROC(__pabt_invalid)

__dabt_invalid:
	inv_entry BAD_DATA
	b	common_invalid
ENDPROC(__dabt_invalid)

__irq_invalid:
	inv_entry BAD_IRQ
	b	common_invalid
ENDPROC(__irq_invalid)

__und_invalid:
	inv_entry BAD_UNDEFINSTR

	@
	@ XXX fall through to common_invalid
	@

@
@ common_invalid - generic code for failed exception (re-entrant version of handlers)
@
common_invalid:
	zero_fp

	ldmia	r0, {r4 - r6}
	add	r0, sp, #S_PC		@ here for interlock avoidance
	mov	r7, #-1			@  ""   ""    ""        ""
	str	r4, [sp]		@ save preserved r0
	stmia	r0, {r5 - r7}		@ lr_<exception>,
					@ cpsr_<exception>, "old_r0"

	mov	r0, sp
	b	bad_mode
ENDPROC(__und_invalid)

/*
 * SVC mode handlers
 */

#if defined(CONFIG_AEABI) && (__LINUX_ARM_ARCH__ >= 5)
#define SPFIX(code...) code
#else
#define SPFIX(code...)
#endif

	.macro	svc_entry, stack_hole=0
 UNWIND(.fnstart		)
 UNWIND(.save {r0 - pc}		)
	sub	sp, sp, #(S_FRAME_SIZE + \stack_hole - 4)
#ifdef CONFIG_THUMB2_KERNEL
 SPFIX(	str	r0, [sp]	)	@ temporarily saved
 SPFIX(	mov	r0, sp		)
 SPFIX(	tst	r0, #4		)	@ test original stack alignment
 SPFIX(	ldr	r0, [sp]	)	@ restored
#else
 SPFIX(	tst	sp, #4		)
#endif
 SPFIX(	subeq	sp, sp, #4	)
	stmia	sp, {r1 - r12}

<<<<<<< HEAD
<<<<<<< HEAD
	ldmia	r0, {r1 - r3}
	add	r5, sp, #S_SP - 4	@ here for interlock avoidance
	mov	r4, #-1			@  ""  ""      ""       ""
	add	r0, sp, #(S_FRAME_SIZE + \stack_hole - 4)
 SPFIX(	addeq	r0, r0, #4	)
	str	r1, [sp, #-4]!		@ save the "real" r0 copied
					@ from the exception stack

	mov	r1, lr
=======
=======
>>>>>>> refs/remotes/origin/master
	ldmia	r0, {r3 - r5}
	add	r7, sp, #S_SP - 4	@ here for interlock avoidance
	mov	r6, #-1			@  ""  ""      ""       ""
	add	r2, sp, #(S_FRAME_SIZE + \stack_hole - 4)
 SPFIX(	addeq	r2, r2, #4	)
	str	r3, [sp, #-4]!		@ save the "real" r0 copied
					@ from the exception stack

	mov	r3, lr
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	@
	@ We are now ready to fill in the remaining blanks on the stack:
	@
<<<<<<< HEAD
<<<<<<< HEAD
	@  r0 - sp_svc
	@  r1 - lr_svc
	@  r2 - lr_<exception>, already fixed up for correct return/restart
	@  r3 - spsr_<exception>
	@  r4 - orig_r0 (see pt_regs definition in ptrace.h)
	@
	stmia	r5, {r0 - r4}
=======
=======
>>>>>>> refs/remotes/origin/master
	@  r2 - sp_svc
	@  r3 - lr_svc
	@  r4 - lr_<exception>, already fixed up for correct return/restart
	@  r5 - spsr_<exception>
	@  r6 - orig_r0 (see pt_regs definition in ptrace.h)
	@
	stmia	r7, {r2 - r6}

#ifdef CONFIG_TRACE_IRQFLAGS
	bl	trace_hardirqs_off
#endif
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	.endm

	.align	5
__dabt_svc:
	svc_entry
<<<<<<< HEAD
<<<<<<< HEAD

	@
	@ get ready to re-enable interrupts if appropriate
	@
	mrs	r9, cpsr
	tst	r3, #PSR_I_BIT
	biceq	r9, r9, #PSR_I_BIT

	@
	@ Call the processor-specific abort handler:
	@
	@  r2 - aborted context pc
	@  r3 - aborted context cpsr
	@
	@ The abort handler must return the aborted address in r0, and
	@ the fault status register in r1.  r9 must be preserved.
	@
#ifdef MULTI_DABORT
	ldr	r4, .LCprocfns
	mov	lr, pc
	ldr	pc, [r4, #PROCESSOR_DABT_FUNC]
#else
	bl	CPU_DABORT_HANDLER
#endif

	@
	@ set desired IRQ state, then call main handler
	@
	debug_entry r1
	msr	cpsr_c, r9
	mov	r2, sp
	bl	do_DataAbort
=======
	mov	r2, sp
	dabt_helper
>>>>>>> refs/remotes/origin/cm-10.0

	@
	@ IRQs off again before pulling preserved data off the stack
	@
	disable_irq_notrace

<<<<<<< HEAD
	@
	@ restore SPSR and restart the instruction
	@
	ldr	r2, [sp, #S_PSR]
	svc_exit r2				@ return from exception
=======
#ifdef CONFIG_TRACE_IRQFLAGS
	tst	r5, #PSR_I_BIT
	bleq	trace_hardirqs_on
	tst	r5, #PSR_I_BIT
	blne	trace_hardirqs_off
#endif
	svc_exit r5				@ return from exception
>>>>>>> refs/remotes/origin/cm-10.0
=======
	mov	r2, sp
	dabt_helper
 THUMB(	ldr	r5, [sp, #S_PSR]	)	@ potentially updated CPSR
	svc_exit r5				@ return from exception
>>>>>>> refs/remotes/origin/master
 UNWIND(.fnend		)
ENDPROC(__dabt_svc)

	.align	5
__irq_svc:
	svc_entry
<<<<<<< HEAD
<<<<<<< HEAD

#ifdef CONFIG_TRACE_IRQFLAGS
	bl	trace_hardirqs_off
#endif
#ifdef CONFIG_PREEMPT
	get_thread_info tsk
	ldr	r8, [tsk, #TI_PREEMPT]		@ get preempt count
	add	r7, r8, #1			@ increment it
	str	r7, [tsk, #TI_PREEMPT]
#endif

	irq_handler
#ifdef CONFIG_PREEMPT
	str	r8, [tsk, #TI_PREEMPT]		@ restore preempt count
=======
=======
>>>>>>> refs/remotes/origin/master
	irq_handler

#ifdef CONFIG_PREEMPT
	get_thread_info tsk
	ldr	r8, [tsk, #TI_PREEMPT]		@ get preempt count
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	ldr	r0, [tsk, #TI_FLAGS]		@ get flags
	teq	r8, #0				@ if preempt count != 0
	movne	r0, #0				@ force flags to 0
	tst	r0, #_TIF_NEED_RESCHED
	blne	svc_preempt
#endif
<<<<<<< HEAD
<<<<<<< HEAD
	ldr	r4, [sp, #S_PSR]		@ irqs are already disabled
#ifdef CONFIG_TRACE_IRQFLAGS
	tst	r4, #PSR_I_BIT
	bleq	trace_hardirqs_on
#endif
	svc_exit r4				@ return from exception
=======

#ifdef CONFIG_TRACE_IRQFLAGS
	@ The parent context IRQs must have been enabled to get here in
	@ the first place, so there's no point checking the PSR I bit.
	bl	trace_hardirqs_on
#endif
	svc_exit r5				@ return from exception
>>>>>>> refs/remotes/origin/cm-10.0
=======

	svc_exit r5, irq = 1			@ return from exception
>>>>>>> refs/remotes/origin/master
 UNWIND(.fnend		)
ENDPROC(__irq_svc)

	.ltorg

#ifdef CONFIG_PREEMPT
svc_preempt:
	mov	r8, lr
1:	bl	preempt_schedule_irq		@ irq en/disable is done inside
	ldr	r0, [tsk, #TI_FLAGS]		@ get new tasks TI_FLAGS
	tst	r0, #_TIF_NEED_RESCHED
	moveq	pc, r8				@ go again
	b	1b
#endif

__und_fault:
	@ Correct the PC such that it is pointing at the instruction
	@ which caused the fault.  If the faulting instruction was ARM
	@ the PC will be pointing at the next instruction, and have to
	@ subtract 4.  Otherwise, it is Thumb, and the PC will be
	@ pointing at the second half of the Thumb instruction.  We
	@ have to subtract 2.
	ldr	r2, [r0, #S_PC]
	sub	r2, r2, r1
	str	r2, [r0, #S_PC]
	b	do_undefinstr
ENDPROC(__und_fault)

	.align	5
__und_svc:
#ifdef CONFIG_KPROBES
	@ If a kprobe is about to simulate a "stmdb sp..." instruction,
	@ it obviously needs free stack space which then will belong to
	@ the saved context.
	svc_entry 64
#else
	svc_entry
#endif
<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	@
	@ call emulation code, which returns using r9 if it has emulated
	@ the instruction, or the more conventional lr if we are to treat
	@ this as a real undefined instruction
	@
	@  r0 - instruction
	@
<<<<<<< HEAD
<<<<<<< HEAD
#ifndef	CONFIG_THUMB2_KERNEL
	ldr	r0, [r2, #-4]
#else
	ldrh	r0, [r2, #-2]			@ Thumb instruction at LR - 2
	and	r9, r0, #0xf800
	cmp	r9, #0xe800			@ 32-bit instruction if xx >= 0
	ldrhhs	r9, [r2]			@ bottom 16 bits
	orrhs	r0, r9, r0, lsl #16
#endif
	adr	r9, BSYM(1f)
	bl	call_fpe

	mov	r0, sp				@ struct pt_regs *regs
	bl	do_undefinstr
=======
=======
>>>>>>> refs/remotes/origin/master
#ifndef CONFIG_THUMB2_KERNEL
	ldr	r0, [r4, #-4]
#else
	mov	r1, #2
	ldrh	r0, [r4, #-2]			@ Thumb instruction at LR - 2
	cmp	r0, #0xe800			@ 32-bit instruction if xx >= 0
	blo	__und_svc_fault
	ldrh	r9, [r4]			@ bottom 16 bits
	add	r4, r4, #2
	str	r4, [sp, #S_PC]
	orr	r0, r9, r0, lsl #16
#endif
	adr	r9, BSYM(__und_svc_finish)
	mov	r2, r4
	bl	call_fpe

	mov	r1, #4				@ PC correction to apply
__und_svc_fault:
	mov	r0, sp				@ struct pt_regs *regs
	bl	__und_fault
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

	@
	@ IRQs off again before pulling preserved data off the stack
	@
<<<<<<< HEAD
1:	disable_irq_notrace
=======
__und_svc_finish:
	disable_irq_notrace
>>>>>>> refs/remotes/origin/cm-10.0

	@
	@ restore SPSR and restart the instruction
	@
<<<<<<< HEAD
	ldr	r2, [sp, #S_PSR]		@ Get SVC cpsr
	svc_exit r2				@ return from exception
=======
	ldr	r5, [sp, #S_PSR]		@ Get SVC cpsr
#ifdef CONFIG_TRACE_IRQFLAGS
	tst	r5, #PSR_I_BIT
	bleq	trace_hardirqs_on
	tst	r5, #PSR_I_BIT
	blne	trace_hardirqs_off
#endif
	svc_exit r5				@ return from exception
>>>>>>> refs/remotes/origin/cm-10.0
=======

__und_svc_finish:
	ldr	r5, [sp, #S_PSR]		@ Get SVC cpsr
	svc_exit r5				@ return from exception
>>>>>>> refs/remotes/origin/master
 UNWIND(.fnend		)
ENDPROC(__und_svc)

	.align	5
__pabt_svc:
	svc_entry
<<<<<<< HEAD
<<<<<<< HEAD

	@
	@ re-enable interrupts if appropriate
	@
	mrs	r9, cpsr
	tst	r3, #PSR_I_BIT
	biceq	r9, r9, #PSR_I_BIT

	mov	r0, r2			@ pass address of aborted instruction.
#ifdef MULTI_PABORT
	ldr	r4, .LCprocfns
	mov	lr, pc
	ldr	pc, [r4, #PROCESSOR_PABT_FUNC]
#else
	bl	CPU_PABORT_HANDLER
#endif
	debug_entry r1
	msr	cpsr_c, r9			@ Maybe enable interrupts
	mov	r2, sp				@ regs
	bl	do_PrefetchAbort		@ call abort handler
=======
	mov	r2, sp				@ regs
	pabt_helper
>>>>>>> refs/remotes/origin/cm-10.0

	@
	@ IRQs off again before pulling preserved data off the stack
	@
	disable_irq_notrace

<<<<<<< HEAD
	@
	@ restore SPSR and restart the instruction
	@
	ldr	r2, [sp, #S_PSR]
	svc_exit r2				@ return from exception
=======
#ifdef CONFIG_TRACE_IRQFLAGS
	tst	r5, #PSR_I_BIT
	bleq	trace_hardirqs_on
	tst	r5, #PSR_I_BIT
	blne	trace_hardirqs_off
#endif
	svc_exit r5				@ return from exception
>>>>>>> refs/remotes/origin/cm-10.0
=======
	mov	r2, sp				@ regs
	pabt_helper
	svc_exit r5				@ return from exception
>>>>>>> refs/remotes/origin/master
 UNWIND(.fnend		)
ENDPROC(__pabt_svc)

	.align	5
.LCcralign:
	.word	cr_alignment
#ifdef MULTI_DABORT
.LCprocfns:
	.word	processor
#endif
.LCfp:
	.word	fp_enter

/*
 * User mode handlers
 *
 * EABI note: sp_svc is always 64-bit aligned here, so should S_FRAME_SIZE
 */

#if defined(CONFIG_AEABI) && (__LINUX_ARM_ARCH__ >= 5) && (S_FRAME_SIZE & 7)
#error "sizeof(struct pt_regs) must be a multiple of 8"
#endif

	.macro	usr_entry
 UNWIND(.fnstart	)
 UNWIND(.cantunwind	)	@ don't unwind the user space
	sub	sp, sp, #S_FRAME_SIZE
 ARM(	stmib	sp, {r1 - r12}	)
 THUMB(	stmia	sp, {r0 - r12}	)

<<<<<<< HEAD
<<<<<<< HEAD
	ldmia	r0, {r1 - r3}
	add	r0, sp, #S_PC		@ here for interlock avoidance
	mov	r4, #-1			@  ""  ""     ""        ""

	str	r1, [sp]		@ save the "real" r0 copied
=======
=======
>>>>>>> refs/remotes/origin/master
	ldmia	r0, {r3 - r5}
	add	r0, sp, #S_PC		@ here for interlock avoidance
	mov	r6, #-1			@  ""  ""     ""        ""

	str	r3, [sp]		@ save the "real" r0 copied
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
					@ from the exception stack

	@
	@ We are now ready to fill in the remaining blanks on the stack:
	@
<<<<<<< HEAD
<<<<<<< HEAD
	@  r2 - lr_<exception>, already fixed up for correct return/restart
	@  r3 - spsr_<exception>
	@  r4 - orig_r0 (see pt_regs definition in ptrace.h)
	@
	@ Also, separately save sp_usr and lr_usr
	@
	stmia	r0, {r2 - r4}
=======
=======
>>>>>>> refs/remotes/origin/master
	@  r4 - lr_<exception>, already fixed up for correct return/restart
	@  r5 - spsr_<exception>
	@  r6 - orig_r0 (see pt_regs definition in ptrace.h)
	@
	@ Also, separately save sp_usr and lr_usr
	@
	stmia	r0, {r4 - r6}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 ARM(	stmdb	r0, {sp, lr}^			)
 THUMB(	store_user_sp_lr r0, r1, S_SP - S_PC	)

	@
	@ Enable the alignment trap while in kernel mode
	@
	alignment_trap r0

	@
	@ Clear FP to mark the first stack frame
	@
	zero_fp
<<<<<<< HEAD
<<<<<<< HEAD
	.endm

	.macro	kuser_cmpxchg_check
#if __LINUX_ARM_ARCH__ < 6 && !defined(CONFIG_NEEDS_SYSCALL_FOR_CMPXCHG)
=======
=======
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_IRQSOFF_TRACER
	bl	trace_hardirqs_off
#endif
<<<<<<< HEAD
	.endm

	.macro	kuser_cmpxchg_check
#if !defined(CONFIG_CPU_32v6K) && !defined(CONFIG_NEEDS_SYSCALL_FOR_CMPXCHG)
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ct_user_exit save = 0
	.endm

	.macro	kuser_cmpxchg_check
#if !defined(CONFIG_CPU_32v6K) && defined(CONFIG_KUSER_HELPERS) && \
    !defined(CONFIG_NEEDS_SYSCALL_FOR_CMPXCHG)
>>>>>>> refs/remotes/origin/master
#ifndef CONFIG_MMU
#warning "NPTL on non MMU needs fixing"
#else
	@ Make sure our user space atomic helper is restarted
	@ if it was interrupted in a critical region.  Here we
	@ perform a quick test inline since it should be false
	@ 99.9999% of the time.  The rest is done out of line.
<<<<<<< HEAD
<<<<<<< HEAD
	cmp	r2, #TASK_SIZE
	blhs	kuser_cmpxchg_fixup
=======
	cmp	r4, #TASK_SIZE
	blhs	kuser_cmpxchg64_fixup
>>>>>>> refs/remotes/origin/cm-10.0
=======
	cmp	r4, #TASK_SIZE
	blhs	kuser_cmpxchg64_fixup
>>>>>>> refs/remotes/origin/master
#endif
#endif
	.endm

	.align	5
__dabt_usr:
	usr_entry
	kuser_cmpxchg_check
<<<<<<< HEAD
<<<<<<< HEAD

	@
	@ Call the processor-specific abort handler:
	@
	@  r2 - aborted context pc
	@  r3 - aborted context cpsr
	@
	@ The abort handler must return the aborted address in r0, and
	@ the fault status register in r1.
	@
#ifdef MULTI_DABORT
	ldr	r4, .LCprocfns
	mov	lr, pc
	ldr	pc, [r4, #PROCESSOR_DABT_FUNC]
#else
	bl	CPU_DABORT_HANDLER
#endif

	@
	@ IRQs on, then call the main handler
	@
	debug_entry r1
	enable_irq
	mov	r2, sp
	adr	lr, BSYM(ret_from_exception)
	b	do_DataAbort
=======
	mov	r2, sp
	dabt_helper
	b	ret_from_exception
>>>>>>> refs/remotes/origin/cm-10.0
=======
	mov	r2, sp
	dabt_helper
	b	ret_from_exception
>>>>>>> refs/remotes/origin/master
 UNWIND(.fnend		)
ENDPROC(__dabt_usr)

	.align	5
__irq_usr:
	usr_entry
	kuser_cmpxchg_check
<<<<<<< HEAD
<<<<<<< HEAD

#ifdef CONFIG_IRQSOFF_TRACER
	bl	trace_hardirqs_off
#endif

	get_thread_info tsk
#ifdef CONFIG_PREEMPT
	ldr	r8, [tsk, #TI_PREEMPT]		@ get preempt count
	add	r7, r8, #1			@ increment it
	str	r7, [tsk, #TI_PREEMPT]
#endif

	irq_handler
#ifdef CONFIG_PREEMPT
	ldr	r0, [tsk, #TI_PREEMPT]
	str	r8, [tsk, #TI_PREEMPT]
	teq	r0, r7
 ARM(	strne	r0, [r0, -r0]	)
 THUMB(	movne	r0, #0		)
 THUMB(	strne	r0, [r0]	)
#endif

=======
	irq_handler
	get_thread_info tsk
>>>>>>> refs/remotes/origin/cm-10.0
=======
	irq_handler
	get_thread_info tsk
>>>>>>> refs/remotes/origin/master
	mov	why, #0
	b	ret_to_user_from_irq
 UNWIND(.fnend		)
ENDPROC(__irq_usr)

	.ltorg

	.align	5
__und_usr:
	usr_entry

<<<<<<< HEAD
<<<<<<< HEAD
	@
	@ fall through to the emulation code, which returns using r9 if
	@ it has emulated the instruction, or the more conventional lr
	@ if we are to treat this as a real undefined instruction
	@
	@  r0 - instruction
	@
	adr	r9, BSYM(ret_from_exception)
	adr	lr, BSYM(__und_usr_unknown)
	tst	r3, #PSR_T_BIT			@ Thumb mode?
	itet	eq				@ explicit IT needed for the 1f label
	subeq	r4, r2, #4			@ ARM instr at LR - 4
	subne	r4, r2, #2			@ Thumb instr at LR - 2
1:	ldreqt	r0, [r4]
#ifdef CONFIG_CPU_ENDIAN_BE8
	reveq	r0, r0				@ little endian instruction
#endif
	beq	call_fpe
	@ Thumb instruction
#if __LINUX_ARM_ARCH__ >= 7
2:
 ARM(	ldrht	r5, [r4], #2	)
 THUMB(	ldrht	r5, [r4]	)
 THUMB(	add	r4, r4, #2	)
	and	r0, r5, #0xf800			@ mask bits 111x x... .... ....
	cmp	r0, #0xe800			@ 32bit instruction if xx != 0
	blo	__und_usr_unknown
3:	ldrht	r0, [r4]
	add	r2, r2, #2			@ r2 is PC + 2, make it PC + 4
	orr    	r0, r0, r5, lsl #16
#else
	b	__und_usr_unknown
#endif
 UNWIND(.fnend		)
ENDPROC(__und_usr)

	@
	@ fallthrough to call_fpe
	@

/*
 * The out of line fixup for the ldrt above.
=======
=======
>>>>>>> refs/remotes/origin/master
	mov	r2, r4
	mov	r3, r5

	@ r2 = regs->ARM_pc, which is either 2 or 4 bytes ahead of the
	@      faulting instruction depending on Thumb mode.
	@ r3 = regs->ARM_cpsr
	@
	@ The emulation code returns using r9 if it has emulated the
	@ instruction, or the more conventional lr if we are to treat
	@ this as a real undefined instruction
	@
	adr	r9, BSYM(ret_from_exception)

	tst	r3, #PSR_T_BIT			@ Thumb mode?
	bne	__und_usr_thumb
	sub	r4, r2, #4			@ ARM instr at LR - 4
1:	ldrt	r0, [r4]
<<<<<<< HEAD
#ifdef CONFIG_CPU_ENDIAN_BE8
	rev	r0, r0				@ little endian instruction
#endif
=======
 ARM_BE8(rev	r0, r0)				@ little endian instruction

>>>>>>> refs/remotes/origin/master
	@ r0 = 32-bit ARM instruction which caused the exception
	@ r2 = PC value for the following instruction (:= regs->ARM_pc)
	@ r4 = PC value for the faulting instruction
	@ lr = 32-bit undefined instruction function
	adr	lr, BSYM(__und_usr_fault_32)
	b	call_fpe

__und_usr_thumb:
	@ Thumb instruction
	sub	r4, r2, #2			@ First half of thumb instr at LR - 2
#if CONFIG_ARM_THUMB && __LINUX_ARM_ARCH__ >= 6 && CONFIG_CPU_V7
/*
 * Thumb-2 instruction handling.  Note that because pre-v6 and >= v6 platforms
 * can never be supported in a single kernel, this code is not applicable at
 * all when __LINUX_ARM_ARCH__ < 6.  This allows simplifying assumptions to be
 * made about .arch directives.
 */
#if __LINUX_ARM_ARCH__ < 7
/* If the target CPU may not be Thumb-2-capable, a run-time check is needed: */
#define NEED_CPU_ARCHITECTURE
	ldr	r5, .LCcpu_architecture
	ldr	r5, [r5]
	cmp	r5, #CPU_ARCH_ARMv7
	blo	__und_usr_fault_16		@ 16bit undefined instruction
/*
 * The following code won't get run unless the running CPU really is v7, so
 * coding round the lack of ldrht on older arches is pointless.  Temporarily
 * override the assembler target arch with the minimum required instead:
 */
	.arch	armv6t2
#endif
2:	ldrht	r5, [r4]
	cmp	r5, #0xe800			@ 32bit instruction if xx != 0
	blo	__und_usr_fault_16		@ 16bit undefined instruction
3:	ldrht	r0, [r2]
	add	r2, r2, #2			@ r2 is PC + 2, make it PC + 4
	str	r2, [sp, #S_PC]			@ it's a 2x16bit instr, update
	orr	r0, r0, r5, lsl #16
	adr	lr, BSYM(__und_usr_fault_32)
	@ r0 = the two 16-bit Thumb instructions which caused the exception
	@ r2 = PC value for the following Thumb instruction (:= regs->ARM_pc)
	@ r4 = PC value for the first 16-bit Thumb instruction
	@ lr = 32bit undefined instruction function

#if __LINUX_ARM_ARCH__ < 7
/* If the target arch was overridden, change it back: */
#ifdef CONFIG_CPU_32v6K
	.arch	armv6k
#else
	.arch	armv6
#endif
#endif /* __LINUX_ARM_ARCH__ < 7 */
#else /* !(CONFIG_ARM_THUMB && __LINUX_ARM_ARCH__ >= 6 && CONFIG_CPU_V7) */
	b	__und_usr_fault_16
#endif
 UNWIND(.fnend)
ENDPROC(__und_usr)

/*
 * The out of line fixup for the ldrt instructions above.
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
 */
	.pushsection .fixup, "ax"
=======
 */
	.pushsection .fixup, "ax"
	.align	2
>>>>>>> refs/remotes/origin/master
4:	mov	pc, r9
	.popsection
	.pushsection __ex_table,"a"
	.long	1b, 4b
<<<<<<< HEAD
<<<<<<< HEAD
#if __LINUX_ARM_ARCH__ >= 7
=======
#if CONFIG_ARM_THUMB && __LINUX_ARM_ARCH__ >= 6 && CONFIG_CPU_V7
>>>>>>> refs/remotes/origin/cm-10.0
=======
#if CONFIG_ARM_THUMB && __LINUX_ARM_ARCH__ >= 6 && CONFIG_CPU_V7
>>>>>>> refs/remotes/origin/master
	.long	2b, 4b
	.long	3b, 4b
#endif
	.popsection

/*
 * Check whether the instruction is a co-processor instruction.
 * If yes, we need to call the relevant co-processor handler.
 *
 * Note that we don't do a full check here for the co-processor
 * instructions; all instructions with bit 27 set are well
 * defined.  The only instructions that should fault are the
 * co-processor instructions.  However, we have to watch out
 * for the ARM6/ARM7 SWI bug.
 *
 * NEON is a special case that has to be handled here. Not all
 * NEON instructions are co-processor instructions, so we have
 * to make a special case of checking for them. Plus, there's
 * five groups of them, so we have a table of mask/opcode pairs
 * to check against, and if any match then we branch off into the
 * NEON handler code.
 *
 * Emulators may wish to make use of the following registers:
 *  r0  = instruction opcode (32-bit ARM or two 16-bit Thumb)
 *  r2  = PC value to resume execution after successful emulation
 *  r9  = normal "successful" return address
 *  r10 = this threads thread_info structure
 *  lr  = unrecognised instruction return address
 * IRQs disabled, FIQs enabled.
 */
	@
	@ Fall-through from Thumb-2 __und_usr
	@
#ifdef CONFIG_NEON
<<<<<<< HEAD
=======
	get_thread_info r10			@ get current thread
>>>>>>> refs/remotes/origin/master
	adr	r6, .LCneon_thumb_opcodes
	b	2f
#endif
call_fpe:
<<<<<<< HEAD
#ifdef CONFIG_NEON
	adr	r6, .LCneon_arm_opcodes
2:
	ldr	r7, [r6], #4			@ mask value
	cmp	r7, #0				@ end mask?
	beq	1f
	and	r8, r0, r7
	ldr	r7, [r6], #4			@ opcode bits matching in mask
	cmp	r8, r7				@ NEON instruction?
	bne	2b
	get_thread_info r10
=======
	get_thread_info r10			@ get current thread
#ifdef CONFIG_NEON
	adr	r6, .LCneon_arm_opcodes
2:	ldr	r5, [r6], #4			@ mask value
	ldr	r7, [r6], #4			@ opcode bits matching in mask
	cmp	r5, #0				@ end mask?
	beq	1f
	and	r8, r0, r5
	cmp	r8, r7				@ NEON instruction?
	bne	2b
>>>>>>> refs/remotes/origin/master
	mov	r7, #1
	strb	r7, [r10, #TI_USED_CP + 10]	@ mark CP#10 as used
	strb	r7, [r10, #TI_USED_CP + 11]	@ mark CP#11 as used
	b	do_vfp				@ let VFP handler handle this
1:
#endif
	tst	r0, #0x08000000			@ only CDP/CPRT/LDC/STC have bit 27
	tstne	r0, #0x04000000			@ bit 26 set on both ARM and Thumb-2
<<<<<<< HEAD
#if defined(CONFIG_CPU_ARM610) || defined(CONFIG_CPU_ARM710)
	and	r8, r0, #0x0f000000		@ mask out op-code bits
	teqne	r8, #0x0f000000			@ SWI (ARM6/7 bug)?
#endif
	moveq	pc, lr
	get_thread_info r10			@ get current thread
=======
	moveq	pc, lr
>>>>>>> refs/remotes/origin/master
	and	r8, r0, #0x00000f00		@ mask out CP number
 THUMB(	lsr	r8, r8, #8		)
	mov	r7, #1
	add	r6, r10, #TI_USED_CP
 ARM(	strb	r7, [r6, r8, lsr #8]	)	@ set appropriate used_cp[]
 THUMB(	strb	r7, [r6, r8]		)	@ set appropriate used_cp[]
#ifdef CONFIG_IWMMXT
	@ Test if we need to give access to iWMMXt coprocessors
	ldr	r5, [r10, #TI_FLAGS]
	rsbs	r7, r8, #(1 << 8)		@ CP 0 or 1 only
	movcss	r7, r5, lsr #(TIF_USING_IWMMXT + 1)
	bcs	iwmmxt_task_enable
#endif
 ARM(	add	pc, pc, r8, lsr #6	)
 THUMB(	lsl	r8, r8, #2		)
 THUMB(	add	pc, r8			)
	nop

	movw_pc	lr				@ CP#0
	W(b)	do_fpe				@ CP#1 (FPE)
	W(b)	do_fpe				@ CP#2 (FPE)
	movw_pc	lr				@ CP#3
#ifdef CONFIG_CRUNCH
	b	crunch_task_enable		@ CP#4 (MaverickCrunch)
	b	crunch_task_enable		@ CP#5 (MaverickCrunch)
	b	crunch_task_enable		@ CP#6 (MaverickCrunch)
#else
	movw_pc	lr				@ CP#4
	movw_pc	lr				@ CP#5
	movw_pc	lr				@ CP#6
#endif
	movw_pc	lr				@ CP#7
	movw_pc	lr				@ CP#8
	movw_pc	lr				@ CP#9
#ifdef CONFIG_VFP
	W(b)	do_vfp				@ CP#10 (VFP)
	W(b)	do_vfp				@ CP#11 (VFP)
#else
	movw_pc	lr				@ CP#10 (VFP)
	movw_pc	lr				@ CP#11 (VFP)
#endif
	movw_pc	lr				@ CP#12
	movw_pc	lr				@ CP#13
	movw_pc	lr				@ CP#14 (Debug)
	movw_pc	lr				@ CP#15 (Control)

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#ifdef NEED_CPU_ARCHITECTURE
	.align	2
.LCcpu_architecture:
	.word	__cpu_architecture
#endif

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_NEON
	.align	6

.LCneon_arm_opcodes:
	.word	0xfe000000			@ mask
	.word	0xf2000000			@ opcode

	.word	0xff100000			@ mask
	.word	0xf4000000			@ opcode

	.word	0x00000000			@ mask
	.word	0x00000000			@ opcode

.LCneon_thumb_opcodes:
	.word	0xef000000			@ mask
	.word	0xef000000			@ opcode

	.word	0xff100000			@ mask
	.word	0xf9000000			@ opcode

	.word	0x00000000			@ mask
	.word	0x00000000			@ opcode
#endif

do_fpe:
	enable_irq
	ldr	r4, .LCfp
	add	r10, r10, #TI_FPSTATE		@ r10 = workspace
	ldr	pc, [r4]			@ Call FP module USR entry point

/*
 * The FP module is called with these registers set:
 *  r0  = instruction
 *  r2  = PC+4
 *  r9  = normal "successful" return address
 *  r10 = FP workspace
 *  lr  = unrecognised FP instruction return address
 */

	.pushsection .data
ENTRY(fp_enter)
	.word	no_fp
	.popsection

ENTRY(no_fp)
	mov	pc, lr
ENDPROC(no_fp)

__und_usr_fault_32:
	mov	r1, #4
	b	1f
__und_usr_fault_16:
	mov	r1, #2
1:	enable_irq
	mov	r0, sp
	adr	lr, BSYM(ret_from_exception)
	b	__und_fault
ENDPROC(__und_usr_fault_32)
ENDPROC(__und_usr_fault_16)

	.align	5
__pabt_usr:
	usr_entry
<<<<<<< HEAD
<<<<<<< HEAD

	mov	r0, r2			@ pass address of aborted instruction.
#ifdef MULTI_PABORT
	ldr	r4, .LCprocfns
	mov	lr, pc
	ldr	pc, [r4, #PROCESSOR_PABT_FUNC]
#else
	bl	CPU_PABORT_HANDLER
#endif
	debug_entry r1
	enable_irq				@ Enable interrupts
	mov	r2, sp				@ regs
	bl	do_PrefetchAbort		@ call abort handler
=======
	mov	r2, sp				@ regs
	pabt_helper
>>>>>>> refs/remotes/origin/cm-10.0
=======
	mov	r2, sp				@ regs
	pabt_helper
>>>>>>> refs/remotes/origin/master
 UNWIND(.fnend		)
	/* fall through */
/*
 * This is the return code to user mode for abort handlers
 */
ENTRY(ret_from_exception)
 UNWIND(.fnstart	)
 UNWIND(.cantunwind	)
	get_thread_info tsk
	mov	why, #0
	b	ret_to_user
 UNWIND(.fnend		)
ENDPROC(__pabt_usr)
ENDPROC(ret_from_exception)

/*
 * Register switch for ARMv3 and ARMv4 processors
 * r0 = previous task_struct, r1 = previous thread_info, r2 = next thread_info
 * previous and next are guaranteed not to be the same.
 */
ENTRY(__switch_to)
 UNWIND(.fnstart	)
 UNWIND(.cantunwind	)
	add	ip, r1, #TI_CPU_SAVE
<<<<<<< HEAD
	ldr	r3, [r2, #TI_TP_VALUE]
=======
>>>>>>> refs/remotes/origin/master
 ARM(	stmia	ip!, {r4 - sl, fp, sp, lr} )	@ Store most regs on stack
 THUMB(	stmia	ip!, {r4 - sl, fp}	   )	@ Store most regs on stack
 THUMB(	str	sp, [ip], #4		   )
 THUMB(	str	lr, [ip], #4		   )
<<<<<<< HEAD
#ifdef CONFIG_CPU_USE_DOMAINS
	ldr	r6, [r2, #TI_CPU_DOMAIN]
#endif
	set_tls	r3, r4, r5
=======
	ldr	r4, [r2, #TI_TP_VALUE]
	ldr	r5, [r2, #TI_TP_VALUE + 4]
#ifdef CONFIG_CPU_USE_DOMAINS
	ldr	r6, [r2, #TI_CPU_DOMAIN]
#endif
	switch_tls r1, r4, r5, r3, r7
>>>>>>> refs/remotes/origin/master
#if defined(CONFIG_CC_STACKPROTECTOR) && !defined(CONFIG_SMP)
	ldr	r7, [r2, #TI_TASK]
	ldr	r8, =__stack_chk_guard
	ldr	r7, [r7, #TSK_STACK_CANARY]
#endif
#ifdef CONFIG_CPU_USE_DOMAINS
<<<<<<< HEAD
#ifdef CONFIG_EMULATE_DOMAIN_MANAGER_V7
	stmdb	r13!, {r0-r3, lr}
	mov	r0, r6
	bl	emulate_domain_manager_set
	ldmia	r13!, {r0-r3, lr}
#else
	mcr	p15, 0, r6, c3, c0, 0		@ Set domain register
#endif
#endif
=======
	mcr	p15, 0, r6, c3, c0, 0		@ Set domain register
#endif
>>>>>>> refs/remotes/origin/master
	mov	r5, r0
	add	r4, r2, #TI_CPU_SAVE
	ldr	r0, =thread_notify_head
	mov	r1, #THREAD_NOTIFY_SWITCH
	bl	atomic_notifier_call_chain
#if defined(CONFIG_CC_STACKPROTECTOR) && !defined(CONFIG_SMP)
	str	r7, [r8]
#endif
 THUMB(	mov	ip, r4			   )
	mov	r0, r5
 ARM(	ldmia	r4, {r4 - sl, fp, sp, pc}  )	@ Load all regs saved previously
 THUMB(	ldmia	ip!, {r4 - sl, fp}	   )	@ Load all regs saved previously
 THUMB(	ldr	sp, [ip], #4		   )
 THUMB(	ldr	pc, [ip]		   )
 UNWIND(.fnend		)
ENDPROC(__switch_to)

	__INIT

/*
 * User helpers.
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * These are segment of kernel provided user code reachable from user space
 * at a fixed address in kernel memory.  This is used to provide user space
 * with some operations which require kernel help because of unimplemented
 * native feature and/or instructions in many ARM CPUs. The idea is for
 * this code to be executed directly in user mode for best efficiency but
 * which is too intimate with the kernel counter part to be left to user
 * libraries.  In fact this code might even differ from one CPU to another
 * depending on the available  instruction set and restrictions like on
 * SMP systems.  In other words, the kernel reserves the right to change
 * this code as needed without warning. Only the entry points and their
 * results are guaranteed to be stable.
 *
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * Each segment is 32-byte aligned and will be moved to the top of the high
 * vector page.  New segments (if ever needed) must be added in front of
 * existing ones.  This mechanism should be used only for things that are
 * really small and justified, and not be abused freely.
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * User space is expected to implement those things inline when optimizing
 * for a processor that has the necessary native support, but only if such
 * resulting binaries are already to be incompatible with earlier ARM
 * processors due to the use of unsupported instructions other than what
 * is provided here.  In other words don't make binaries unable to run on
 * earlier processors just for the sake of not using these kernel helpers
 * if your compiled code is not going to use the new instructions for other
 * purpose.
=======
 * See Documentation/arm/kernel_user_helpers.txt for formal definitions.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * See Documentation/arm/kernel_user_helpers.txt for formal definitions.
>>>>>>> refs/remotes/origin/master
 */
 THUMB(	.arm	)

	.macro	usr_ret, reg
#ifdef CONFIG_ARM_THUMB
	bx	\reg
#else
	mov	pc, \reg
#endif
	.endm

<<<<<<< HEAD
=======
	.macro	kuser_pad, sym, size
	.if	(. - \sym) & 3
	.rept	4 - (. - \sym) & 3
	.byte	0
	.endr
	.endif
	.rept	(\size - (. - \sym)) / 4
	.word	0xe7fddef1
	.endr
	.endm

#ifdef CONFIG_KUSER_HELPERS
>>>>>>> refs/remotes/origin/master
	.align	5
	.globl	__kuser_helper_start
__kuser_helper_start:

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * Reference prototype:
 *
 *	void __kernel_memory_barrier(void)
 *
 * Input:
 *
 *	lr = return address
 *
 * Output:
 *
 *	none
 *
 * Clobbered:
 *
 *	none
 *
 * Definition and user space usage example:
 *
 *	typedef void (__kernel_dmb_t)(void);
 *	#define __kernel_dmb (*(__kernel_dmb_t *)0xffff0fa0)
 *
 * Apply any needed memory barrier to preserve consistency with data modified
 * manually and __kuser_cmpxchg usage.
 *
 * This could be used as follows:
 *
 * #define __kernel_dmb() \
 *         asm volatile ( "mov r0, #0xffff0fff; mov lr, pc; sub pc, r0, #95" \
 *	        : : : "r0", "lr","cc" )
 */

__kuser_memory_barrier:				@ 0xffff0fa0
	smp_dmb	arm
	usr_ret	lr

	.align	5

/*
 * Reference prototype:
 *
 *	int __kernel_cmpxchg(int oldval, int newval, int *ptr)
 *
 * Input:
 *
 *	r0 = oldval
 *	r1 = newval
 *	r2 = ptr
 *	lr = return address
 *
 * Output:
 *
 *	r0 = returned value (zero or non-zero)
 *	C flag = set if r0 == 0, clear if r0 != 0
 *
 * Clobbered:
 *
 *	r3, ip, flags
 *
 * Definition and user space usage example:
 *
 *	typedef int (__kernel_cmpxchg_t)(int oldval, int newval, int *ptr);
 *	#define __kernel_cmpxchg (*(__kernel_cmpxchg_t *)0xffff0fc0)
 *
 * Atomically store newval in *ptr if *ptr is equal to oldval for user space.
 * Return zero if *ptr was changed or non-zero if no exchange happened.
 * The C flag is also set if *ptr was changed to allow for assembly
 * optimization in the calling code.
 *
 * Notes:
 *
 *    - This routine already includes memory barriers as needed.
 *
 * For example, a user space atomic_add implementation could look like this:
 *
 * #define atomic_add(ptr, val) \
 *	({ register unsigned int *__ptr asm("r2") = (ptr); \
 *	   register unsigned int __result asm("r1"); \
 *	   asm volatile ( \
 *	       "1: @ atomic_add\n\t" \
 *	       "ldr	r0, [r2]\n\t" \
 *	       "mov	r3, #0xffff0fff\n\t" \
 *	       "add	lr, pc, #4\n\t" \
 *	       "add	r1, r0, %2\n\t" \
 *	       "add	pc, r3, #(0xffff0fc0 - 0xffff0fff)\n\t" \
 *	       "bcc	1b" \
 *	       : "=&r" (__result) \
 *	       : "r" (__ptr), "rIL" (val) \
 *	       : "r0","r3","ip","lr","cc","memory" ); \
 *	   __result; })
 */
=======
=======
>>>>>>> refs/remotes/origin/master
 * Due to the length of some sequences, __kuser_cmpxchg64 spans 2 regular
 * kuser "slots", therefore 0xffff0f80 is not used as a valid entry point.
 */

__kuser_cmpxchg64:				@ 0xffff0f60

#if defined(CONFIG_NEEDS_SYSCALL_FOR_CMPXCHG)

	/*
	 * Poor you.  No fast solution possible...
	 * The kernel itself must perform the operation.
	 * A special ghost syscall is used for that (see traps.c).
	 */
	stmfd	sp!, {r7, lr}
	ldr	r7, 1f			@ it's 20 bits
	swi	__ARM_NR_cmpxchg64
	ldmfd	sp!, {r7, pc}
1:	.word	__ARM_NR_cmpxchg64

#elif defined(CONFIG_CPU_32v6K)

	stmfd	sp!, {r4, r5, r6, r7}
	ldrd	r4, r5, [r0]			@ load old val
	ldrd	r6, r7, [r1]			@ load new val
	smp_dmb	arm
1:	ldrexd	r0, r1, [r2]			@ load current val
	eors	r3, r0, r4			@ compare with oldval (1)
	eoreqs	r3, r1, r5			@ compare with oldval (2)
	strexdeq r3, r6, r7, [r2]		@ store newval if eq
	teqeq	r3, #1				@ success?
	beq	1b				@ if no then retry
	smp_dmb	arm
	rsbs	r0, r3, #0			@ set returned val and C flag
	ldmfd	sp!, {r4, r5, r6, r7}
	usr_ret	lr

#elif !defined(CONFIG_SMP)

#ifdef CONFIG_MMU

	/*
	 * The only thing that can break atomicity in this cmpxchg64
	 * implementation is either an IRQ or a data abort exception
	 * causing another process/thread to be scheduled in the middle of
	 * the critical sequence.  The same strategy as for cmpxchg is used.
	 */
	stmfd	sp!, {r4, r5, r6, lr}
	ldmia	r0, {r4, r5}			@ load old val
	ldmia	r1, {r6, lr}			@ load new val
1:	ldmia	r2, {r0, r1}			@ load current val
	eors	r3, r0, r4			@ compare with oldval (1)
	eoreqs	r3, r1, r5			@ compare with oldval (2)
2:	stmeqia	r2, {r6, lr}			@ store newval if eq
	rsbs	r0, r3, #0			@ set return val and C flag
	ldmfd	sp!, {r4, r5, r6, pc}

	.text
kuser_cmpxchg64_fixup:
	@ Called from kuser_cmpxchg_fixup.
	@ r4 = address of interrupted insn (must be preserved).
	@ sp = saved regs. r7 and r8 are clobbered.
	@ 1b = first critical insn, 2b = last critical insn.
	@ If r4 >= 1b and r4 <= 2b then saved pc_usr is set to 1b.
	mov	r7, #0xffff0fff
	sub	r7, r7, #(0xffff0fff - (0xffff0f60 + (1b - __kuser_cmpxchg64)))
	subs	r8, r4, r7
	rsbcss	r8, r8, #(2b - 1b)
	strcs	r7, [sp, #S_PC]
#if __LINUX_ARM_ARCH__ < 6
	bcc	kuser_cmpxchg32_fixup
#endif
	mov	pc, lr
	.previous

#else
#warning "NPTL on non MMU needs fixing"
	mov	r0, #-1
	adds	r0, r0, #0
	usr_ret	lr
#endif

#else
#error "incoherent kernel configuration"
#endif

<<<<<<< HEAD
	/* pad to next slot */
	.rept	(16 - (. - __kuser_cmpxchg64)/4)
	.word	0
	.endr

	.align	5
=======
	kuser_pad __kuser_cmpxchg64, 64
>>>>>>> refs/remotes/origin/master

__kuser_memory_barrier:				@ 0xffff0fa0
	smp_dmb	arm
	usr_ret	lr

<<<<<<< HEAD
	.align	5
>>>>>>> refs/remotes/origin/cm-10.0
=======
	kuser_pad __kuser_memory_barrier, 32
>>>>>>> refs/remotes/origin/master

__kuser_cmpxchg:				@ 0xffff0fc0

#if defined(CONFIG_NEEDS_SYSCALL_FOR_CMPXCHG)

	/*
	 * Poor you.  No fast solution possible...
	 * The kernel itself must perform the operation.
	 * A special ghost syscall is used for that (see traps.c).
	 */
	stmfd	sp!, {r7, lr}
	ldr	r7, 1f			@ it's 20 bits
	swi	__ARM_NR_cmpxchg
	ldmfd	sp!, {r7, pc}
1:	.word	__ARM_NR_cmpxchg

#elif __LINUX_ARM_ARCH__ < 6

#ifdef CONFIG_MMU

	/*
	 * The only thing that can break atomicity in this cmpxchg
	 * implementation is either an IRQ or a data abort exception
	 * causing another process/thread to be scheduled in the middle
	 * of the critical sequence.  To prevent this, code is added to
	 * the IRQ and data abort exception handlers to set the pc back
	 * to the beginning of the critical section if it is found to be
	 * within that critical section (see kuser_cmpxchg_fixup).
	 */
1:	ldr	r3, [r2]			@ load current val
	subs	r3, r3, r0			@ compare with oldval
2:	streq	r1, [r2]			@ store newval if eq
	rsbs	r0, r3, #0			@ set return val and C flag
	usr_ret	lr

	.text
<<<<<<< HEAD
<<<<<<< HEAD
kuser_cmpxchg_fixup:
	@ Called from kuser_cmpxchg_check macro.
	@ r2 = address of interrupted insn (must be preserved).
	@ sp = saved regs. r7 and r8 are clobbered.
	@ 1b = first critical insn, 2b = last critical insn.
	@ If r2 >= 1b and r2 <= 2b then saved pc_usr is set to 1b.
	mov	r7, #0xffff0fff
	sub	r7, r7, #(0xffff0fff - (0xffff0fc0 + (1b - __kuser_cmpxchg)))
	subs	r8, r2, r7
=======
=======
>>>>>>> refs/remotes/origin/master
kuser_cmpxchg32_fixup:
	@ Called from kuser_cmpxchg_check macro.
	@ r4 = address of interrupted insn (must be preserved).
	@ sp = saved regs. r7 and r8 are clobbered.
	@ 1b = first critical insn, 2b = last critical insn.
	@ If r4 >= 1b and r4 <= 2b then saved pc_usr is set to 1b.
	mov	r7, #0xffff0fff
	sub	r7, r7, #(0xffff0fff - (0xffff0fc0 + (1b - __kuser_cmpxchg)))
	subs	r8, r4, r7
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	rsbcss	r8, r8, #(2b - 1b)
	strcs	r7, [sp, #S_PC]
	mov	pc, lr
	.previous

#else
#warning "NPTL on non MMU needs fixing"
	mov	r0, #-1
	adds	r0, r0, #0
	usr_ret	lr
#endif

#else

	smp_dmb	arm
1:	ldrex	r3, [r2]
	subs	r3, r3, r0
	strexeq	r3, r1, [r2]
	teqeq	r3, #1
	beq	1b
	rsbs	r0, r3, #0
	/* beware -- each __kuser slot must be 8 instructions max */
	ALT_SMP(b	__kuser_memory_barrier)
	ALT_UP(usr_ret	lr)

#endif

<<<<<<< HEAD
	.align	5

<<<<<<< HEAD
/*
 * Reference prototype:
 *
 *	int __kernel_get_tls(void)
 *
 * Input:
 *
 *	lr = return address
 *
 * Output:
 *
 *	r0 = TLS value
 *
 * Clobbered:
 *
 *	none
 *
 * Definition and user space usage example:
 *
 *	typedef int (__kernel_get_tls_t)(void);
 *	#define __kernel_get_tls (*(__kernel_get_tls_t *)0xffff0fe0)
 *
 * Get the TLS value as previously set via the __ARM_NR_set_tls syscall.
 *
 * This could be used as follows:
 *
 * #define __kernel_get_tls() \
 *	({ register unsigned int __val asm("r0"); \
 *         asm( "mov r0, #0xffff0fff; mov lr, pc; sub pc, r0, #31" \
 *	        : "=r" (__val) : : "lr","cc" ); \
 *	   __val; })
 */

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	kuser_pad __kuser_cmpxchg, 32

>>>>>>> refs/remotes/origin/master
__kuser_get_tls:				@ 0xffff0fe0
	ldr	r0, [pc, #(16 - 8)]	@ read TLS, set in kuser_get_tls_init
	usr_ret	lr
	mrc	p15, 0, r0, c13, c0, 3	@ 0xffff0fe8 hardware TLS code
<<<<<<< HEAD
	.rep	4
	.word	0			@ 0xffff0ff0 software TLS value, then
	.endr				@ pad up to __kuser_helper_version

<<<<<<< HEAD
/*
 * Reference declaration:
 *
 *	extern unsigned int __kernel_helper_version;
 *
 * Definition and user space usage example:
 *
 *	#define __kernel_helper_version (*(unsigned int *)0xffff0ffc)
 *
 * User space may read this to determine the curent number of helpers
 * available.
 */

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	kuser_pad __kuser_get_tls, 16
	.rep	3
	.word	0			@ 0xffff0ff0 software TLS value, then
	.endr				@ pad up to __kuser_helper_version

>>>>>>> refs/remotes/origin/master
__kuser_helper_version:				@ 0xffff0ffc
	.word	((__kuser_helper_end - __kuser_helper_start) >> 5)

	.globl	__kuser_helper_end
__kuser_helper_end:

<<<<<<< HEAD
=======
#endif

>>>>>>> refs/remotes/origin/master
 THUMB(	.thumb	)

/*
 * Vector stubs.
 *
<<<<<<< HEAD
 * This code is copied to 0xffff0200 so we can use branches in the
 * vectors, rather than ldr's.  Note that this code must not
 * exceed 0x300 bytes.
=======
 * This code is copied to 0xffff1000 so we can use branches in the
 * vectors, rather than ldr's.  Note that this code must not exceed
 * a page size.
>>>>>>> refs/remotes/origin/master
 *
 * Common stub entry macro:
 *   Enter in IRQ mode, spsr = SVC/USR CPSR, lr = SVC/USR PC
 *
 * SP points to a minimal amount of processor-private memory, the address
 * of which is copied into r0 for the mode specific abort handler.
 */
<<<<<<< HEAD
<<<<<<< HEAD
	.macro	vector_stub, name, mode, fixup, correction=0
=======
	.macro	vector_stub, name, mode, correction=0
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.macro	vector_stub, name, mode, correction=0
>>>>>>> refs/remotes/origin/master
	.align	5

vector_\name:
	.if \correction
	sub	lr, lr, #\correction
	.endif

	@
	@ Save r0, lr_<exception> (parent PC) and spsr_<exception>
	@ (parent CPSR)
	@
	stmia	sp, {r0, lr}		@ save r0, lr
	mrs	lr, spsr
	str	lr, [sp, #8]		@ save spsr

	@
	@ Prepare for SVC32 mode.  IRQs remain disabled.
	@
	mrs	r0, cpsr
	eor	r0, r0, #(\mode ^ SVC_MODE | PSR_ISETSTATE)
	msr	spsr_cxsf, r0

	@
	@ the branch table must immediately follow this code
	@
	and	lr, lr, #0x0f
 THUMB(	adr	r0, 1f			)
 THUMB(	ldr	lr, [r0, lr, lsl #2]	)
<<<<<<< HEAD
<<<<<<< HEAD
	.if	\fixup
#ifdef CONFIG_MSM_KRAIT_WFE_FIXUP
	ldr	r0, .krait_fixup
	ldr	r0, [r0]
	cmp	r0, #0
	beq	10f
	mrc	p15, 7, r0, c15, c0, 5
	orr	r0, r0, #0x10000
	mcr	p15, 7, r0, c15, c0, 5
10:	isb
#endif
	.endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	mov	r0, sp
 ARM(	ldr	lr, [pc, lr, lsl #2]	)
	movs	pc, lr			@ branch to handler in SVC mode
ENDPROC(vector_\name)

	.align	2
	@ handler addresses follow this label
1:
	.endm

<<<<<<< HEAD
	.globl	__stubs_start
__stubs_start:
/*
 * Interrupt dispatcher
 */
<<<<<<< HEAD
	vector_stub	irq, IRQ_MODE, 1, 4
=======
	vector_stub	irq, IRQ_MODE, 4
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.section .stubs, "ax", %progbits
__stubs_start:
	@ This must be the first word
	.word	vector_swi

vector_rst:
 ARM(	swi	SYS_ERROR0	)
 THUMB(	svc	#0		)
 THUMB(	nop			)
	b	vector_und

/*
 * Interrupt dispatcher
 */
	vector_stub	irq, IRQ_MODE, 4
>>>>>>> refs/remotes/origin/master

	.long	__irq_usr			@  0  (USR_26 / USR_32)
	.long	__irq_invalid			@  1  (FIQ_26 / FIQ_32)
	.long	__irq_invalid			@  2  (IRQ_26 / IRQ_32)
	.long	__irq_svc			@  3  (SVC_26 / SVC_32)
	.long	__irq_invalid			@  4
	.long	__irq_invalid			@  5
	.long	__irq_invalid			@  6
	.long	__irq_invalid			@  7
	.long	__irq_invalid			@  8
	.long	__irq_invalid			@  9
	.long	__irq_invalid			@  a
	.long	__irq_invalid			@  b
	.long	__irq_invalid			@  c
	.long	__irq_invalid			@  d
	.long	__irq_invalid			@  e
	.long	__irq_invalid			@  f

/*
 * Data abort dispatcher
 * Enter in ABT mode, spsr = USR CPSR, lr = USR PC
 */
<<<<<<< HEAD
<<<<<<< HEAD
	vector_stub	dabt, ABT_MODE, 0, 8
=======
	vector_stub	dabt, ABT_MODE, 8
>>>>>>> refs/remotes/origin/cm-10.0
=======
	vector_stub	dabt, ABT_MODE, 8
>>>>>>> refs/remotes/origin/master

	.long	__dabt_usr			@  0  (USR_26 / USR_32)
	.long	__dabt_invalid			@  1  (FIQ_26 / FIQ_32)
	.long	__dabt_invalid			@  2  (IRQ_26 / IRQ_32)
	.long	__dabt_svc			@  3  (SVC_26 / SVC_32)
	.long	__dabt_invalid			@  4
	.long	__dabt_invalid			@  5
	.long	__dabt_invalid			@  6
	.long	__dabt_invalid			@  7
	.long	__dabt_invalid			@  8
	.long	__dabt_invalid			@  9
	.long	__dabt_invalid			@  a
	.long	__dabt_invalid			@  b
	.long	__dabt_invalid			@  c
	.long	__dabt_invalid			@  d
	.long	__dabt_invalid			@  e
	.long	__dabt_invalid			@  f

/*
 * Prefetch abort dispatcher
 * Enter in ABT mode, spsr = USR CPSR, lr = USR PC
 */
<<<<<<< HEAD
<<<<<<< HEAD
	vector_stub	pabt, ABT_MODE, 0, 4
=======
	vector_stub	pabt, ABT_MODE, 4
>>>>>>> refs/remotes/origin/cm-10.0
=======
	vector_stub	pabt, ABT_MODE, 4
>>>>>>> refs/remotes/origin/master

	.long	__pabt_usr			@  0 (USR_26 / USR_32)
	.long	__pabt_invalid			@  1 (FIQ_26 / FIQ_32)
	.long	__pabt_invalid			@  2 (IRQ_26 / IRQ_32)
	.long	__pabt_svc			@  3 (SVC_26 / SVC_32)
	.long	__pabt_invalid			@  4
	.long	__pabt_invalid			@  5
	.long	__pabt_invalid			@  6
	.long	__pabt_invalid			@  7
	.long	__pabt_invalid			@  8
	.long	__pabt_invalid			@  9
	.long	__pabt_invalid			@  a
	.long	__pabt_invalid			@  b
	.long	__pabt_invalid			@  c
	.long	__pabt_invalid			@  d
	.long	__pabt_invalid			@  e
	.long	__pabt_invalid			@  f

/*
 * Undef instr entry dispatcher
 * Enter in UND mode, spsr = SVC/USR CPSR, lr = SVC/USR PC
 */
<<<<<<< HEAD
<<<<<<< HEAD
	vector_stub	und, UND_MODE, 0
=======
	vector_stub	und, UND_MODE
>>>>>>> refs/remotes/origin/cm-10.0
=======
	vector_stub	und, UND_MODE
>>>>>>> refs/remotes/origin/master

	.long	__und_usr			@  0 (USR_26 / USR_32)
	.long	__und_invalid			@  1 (FIQ_26 / FIQ_32)
	.long	__und_invalid			@  2 (IRQ_26 / IRQ_32)
	.long	__und_svc			@  3 (SVC_26 / SVC_32)
	.long	__und_invalid			@  4
	.long	__und_invalid			@  5
	.long	__und_invalid			@  6
	.long	__und_invalid			@  7
	.long	__und_invalid			@  8
	.long	__und_invalid			@  9
	.long	__und_invalid			@  a
	.long	__und_invalid			@  b
	.long	__und_invalid			@  c
	.long	__und_invalid			@  d
	.long	__und_invalid			@  e
	.long	__und_invalid			@  f

	.align	5

/*=============================================================================
<<<<<<< HEAD
=======
 * Address exception handler
 *-----------------------------------------------------------------------------
 * These aren't too critical.
 * (they're not supposed to happen, and won't happen in 32-bit data mode).
 */

vector_addrexcptn:
	b	vector_addrexcptn

/*=============================================================================
>>>>>>> refs/remotes/origin/master
 * Undefined FIQs
 *-----------------------------------------------------------------------------
 * Enter in FIQ mode, spsr = ANY CPSR, lr = ANY PC
 * MUST PRESERVE SVC SPSR, but need to switch to SVC mode to show our msg.
 * Basically to switch modes, we *HAVE* to clobber one register...  brain
 * damage alert!  I don't think that we can execute any code in here in any
 * other mode than FIQ...  Ok you can switch to another mode, but you can't
 * get out of that mode without clobbering one register.
 */
vector_fiq:
<<<<<<< HEAD
<<<<<<< HEAD
	disable_fiq
=======
>>>>>>> refs/remotes/origin/cm-10.0
	subs	pc, lr, #4

/*=============================================================================
 * Address exception handler
 *-----------------------------------------------------------------------------
 * These aren't too critical.
 * (they're not supposed to happen, and won't happen in 32-bit data mode).
 */

vector_addrexcptn:
	b	vector_addrexcptn

/*
 * We group all the following data together to optimise
 * for CPUs with separate I & D caches.
 */
	.align	5

.LCvswi:
	.word	vector_swi
<<<<<<< HEAD
.krait_fixup:
	.word	msm_krait_need_wfe_fixup
=======
>>>>>>> refs/remotes/origin/cm-10.0

	.globl	__stubs_end
__stubs_end:

	.equ	stubs_offset, __vectors_start + 0x200 - __stubs_start

	.globl	__vectors_start
__vectors_start:
 ARM(	swi	SYS_ERROR0	)
 THUMB(	svc	#0		)
 THUMB(	nop			)
	W(b)	vector_und + stubs_offset
	W(ldr)	pc, .LCvswi + stubs_offset
	W(b)	vector_pabt + stubs_offset
	W(b)	vector_dabt + stubs_offset
	W(b)	vector_addrexcptn + stubs_offset
	W(b)	vector_irq + stubs_offset
	W(b)	vector_fiq + stubs_offset

	.globl	__vectors_end
__vectors_end:
=======
	subs	pc, lr, #4

	.globl	vector_fiq_offset
	.equ	vector_fiq_offset, vector_fiq

	.section .vectors, "ax", %progbits
__vectors_start:
	W(b)	vector_rst
	W(b)	vector_und
	W(ldr)	pc, __vectors_start + 0x1000
	W(b)	vector_pabt
	W(b)	vector_dabt
	W(b)	vector_addrexcptn
	W(b)	vector_irq
	W(b)	vector_fiq
>>>>>>> refs/remotes/origin/master

	.data

	.globl	cr_alignment
	.globl	cr_no_alignment
cr_alignment:
	.space	4
cr_no_alignment:
	.space	4

#ifdef CONFIG_MULTI_IRQ_HANDLER
	.globl	handle_arch_irq
handle_arch_irq:
	.space	4
#endif
