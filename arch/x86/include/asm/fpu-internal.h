/*
 * Copyright (C) 1994 Linus Torvalds
 *
 * Pentium III FXSR, SSE support
 * General FPU state handling cleanups
 *	Gareth Hughes <gareth@valinux.com>, May 2000
 * x86-64 work by Andi Kleen 2002
 */

#ifndef _FPU_INTERNAL_H
#define _FPU_INTERNAL_H

#include <linux/kernel_stat.h>
#include <linux/regset.h>
<<<<<<< HEAD
=======
#include <linux/compat.h>
>>>>>>> refs/remotes/origin/master
#include <linux/slab.h>
#include <asm/asm.h>
#include <asm/cpufeature.h>
#include <asm/processor.h>
#include <asm/sigcontext.h>
#include <asm/user.h>
#include <asm/uaccess.h>
#include <asm/xsave.h>
<<<<<<< HEAD

extern unsigned int sig_xstate_size;
extern void fpu_init(void);

DECLARE_PER_CPU(struct task_struct *, fpu_owner_task);

=======
#include <asm/smap.h>

#ifdef CONFIG_X86_64
# include <asm/sigcontext32.h>
# include <asm/user32.h>
struct ksignal;
int ia32_setup_rt_frame(int sig, struct ksignal *ksig,
			compat_sigset_t *set, struct pt_regs *regs);
int ia32_setup_frame(int sig, struct ksignal *ksig,
		     compat_sigset_t *set, struct pt_regs *regs);
#else
# define user_i387_ia32_struct	user_i387_struct
# define user32_fxsr_struct	user_fxsr_struct
# define ia32_setup_frame	__setup_frame
# define ia32_setup_rt_frame	__setup_rt_frame
#endif

extern unsigned int mxcsr_feature_mask;
extern void fpu_init(void);
extern void eager_fpu_init(void);

DECLARE_PER_CPU(struct task_struct *, fpu_owner_task);

extern void convert_from_fxsr(struct user_i387_ia32_struct *env,
			      struct task_struct *tsk);
extern void convert_to_fxsr(struct task_struct *tsk,
			    const struct user_i387_ia32_struct *env);

>>>>>>> refs/remotes/origin/master
extern user_regset_active_fn fpregs_active, xfpregs_active;
extern user_regset_get_fn fpregs_get, xfpregs_get, fpregs_soft_get,
				xstateregs_get;
extern user_regset_set_fn fpregs_set, xfpregs_set, fpregs_soft_set,
				 xstateregs_set;

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/master
/*
 * xstateregs_active == fpregs_active. Please refer to the comment
 * at the definition of fpregs_active.
 */
#define xstateregs_active	fpregs_active

<<<<<<< HEAD
extern struct _fpx_sw_bytes fx_sw_reserved;
#ifdef CONFIG_IA32_EMULATION
extern unsigned int sig_xstate_ia32_size;
extern struct _fpx_sw_bytes fx_sw_reserved_ia32;
struct _fpstate_ia32;
struct _xstate_ia32;
extern int save_i387_xstate_ia32(void __user *buf);
extern int restore_i387_xstate_ia32(void __user *buf);
#endif

=======
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_MATH_EMULATION
extern void finit_soft_fpu(struct i387_soft_struct *soft);
#else
static inline void finit_soft_fpu(struct i387_soft_struct *soft) {}
#endif

<<<<<<< HEAD
#define X87_FSW_ES (1 << 7)	/* Exception Summary */

=======
static inline int is_ia32_compat_frame(void)
{
	return config_enabled(CONFIG_IA32_EMULATION) &&
	       test_thread_flag(TIF_IA32);
}

static inline int is_ia32_frame(void)
{
	return config_enabled(CONFIG_X86_32) || is_ia32_compat_frame();
}

static inline int is_x32_frame(void)
{
	return config_enabled(CONFIG_X86_X32_ABI) && test_thread_flag(TIF_X32);
}

#define X87_FSW_ES (1 << 7)	/* Exception Summary */

static __always_inline __pure bool use_eager_fpu(void)
{
	return static_cpu_has(X86_FEATURE_EAGER_FPU);
}

>>>>>>> refs/remotes/origin/master
static __always_inline __pure bool use_xsaveopt(void)
{
	return static_cpu_has(X86_FEATURE_XSAVEOPT);
}

static __always_inline __pure bool use_xsave(void)
{
	return static_cpu_has(X86_FEATURE_XSAVE);
}

static __always_inline __pure bool use_fxsr(void)
{
        return static_cpu_has(X86_FEATURE_FXSR);
}

<<<<<<< HEAD
=======
static inline void fx_finit(struct i387_fxsave_struct *fx)
{
	memset(fx, 0, xstate_size);
	fx->cwd = 0x37f;
	fx->mxcsr = MXCSR_DEFAULT;
}

>>>>>>> refs/remotes/origin/master
extern void __sanitize_i387_state(struct task_struct *);

static inline void sanitize_i387_state(struct task_struct *tsk)
{
	if (!use_xsaveopt())
		return;
	__sanitize_i387_state(tsk);
}

<<<<<<< HEAD
#ifdef CONFIG_X86_64
static inline int fxrstor_checking(struct i387_fxsave_struct *fx)
{
	int err;

	/* See comment in fxsave() below. */
#ifdef CONFIG_AS_FXSAVEQ
	asm volatile("1:  fxrstorq %[fx]\n\t"
		     "2:\n"
		     ".section .fixup,\"ax\"\n"
		     "3:  movl $-1,%[err]\n"
		     "    jmp  2b\n"
		     ".previous\n"
		     _ASM_EXTABLE(1b, 3b)
		     : [err] "=r" (err)
		     : [fx] "m" (*fx), "0" (0));
#else
	asm volatile("1:  rex64/fxrstor (%[fx])\n\t"
		     "2:\n"
		     ".section .fixup,\"ax\"\n"
		     "3:  movl $-1,%[err]\n"
		     "    jmp  2b\n"
		     ".previous\n"
		     _ASM_EXTABLE(1b, 3b)
		     : [err] "=r" (err)
		     : [fx] "R" (fx), "m" (*fx), "0" (0));
#endif
	return err;
=======
#define user_insn(insn, output, input...)				\
({									\
	int err;							\
	asm volatile(ASM_STAC "\n"					\
		     "1:" #insn "\n\t"					\
		     "2: " ASM_CLAC "\n"				\
		     ".section .fixup,\"ax\"\n"				\
		     "3:  movl $-1,%[err]\n"				\
		     "    jmp  2b\n"					\
		     ".previous\n"					\
		     _ASM_EXTABLE(1b, 3b)				\
		     : [err] "=r" (err), output				\
		     : "0"(0), input);					\
	err;								\
})

#define check_insn(insn, output, input...)				\
({									\
	int err;							\
	asm volatile("1:" #insn "\n\t"					\
		     "2:\n"						\
		     ".section .fixup,\"ax\"\n"				\
		     "3:  movl $-1,%[err]\n"				\
		     "    jmp  2b\n"					\
		     ".previous\n"					\
		     _ASM_EXTABLE(1b, 3b)				\
		     : [err] "=r" (err), output				\
		     : "0"(0), input);					\
	err;								\
})

static inline int fsave_user(struct i387_fsave_struct __user *fx)
{
	return user_insn(fnsave %[fx]; fwait,  [fx] "=m" (*fx), "m" (*fx));
>>>>>>> refs/remotes/origin/master
}

static inline int fxsave_user(struct i387_fxsave_struct __user *fx)
{
<<<<<<< HEAD
	int err;

	/*
	 * Clear the bytes not touched by the fxsave and reserved
	 * for the SW usage.
	 */
	err = __clear_user(&fx->sw_reserved,
			   sizeof(struct _fpx_sw_bytes));
	if (unlikely(err))
		return -EFAULT;

	/* See comment in fxsave() below. */
#ifdef CONFIG_AS_FXSAVEQ
	asm volatile("1:  fxsaveq %[fx]\n\t"
		     "2:\n"
		     ".section .fixup,\"ax\"\n"
		     "3:  movl $-1,%[err]\n"
		     "    jmp  2b\n"
		     ".previous\n"
		     _ASM_EXTABLE(1b, 3b)
		     : [err] "=r" (err), [fx] "=m" (*fx)
		     : "0" (0));
#else
	asm volatile("1:  rex64/fxsave (%[fx])\n\t"
		     "2:\n"
		     ".section .fixup,\"ax\"\n"
		     "3:  movl $-1,%[err]\n"
		     "    jmp  2b\n"
		     ".previous\n"
		     _ASM_EXTABLE(1b, 3b)
		     : [err] "=r" (err), "=m" (*fx)
		     : [fx] "R" (fx), "0" (0));
#endif
	if (unlikely(err) &&
	    __clear_user(fx, sizeof(struct i387_fxsave_struct)))
		err = -EFAULT;
	/* No need to clear here because the caller clears USED_MATH */
	return err;
}

static inline void fpu_fxsave(struct fpu *fpu)
{
	/* Using "rex64; fxsave %0" is broken because, if the memory operand
	   uses any extended registers for addressing, a second REX prefix
	   will be generated (to the assembler, rex64 followed by semicolon
	   is a separate instruction), and hence the 64-bitness is lost. */

#ifdef CONFIG_AS_FXSAVEQ
	/* Using "fxsaveq %0" would be the ideal choice, but is only supported
	   starting with gas 2.16. */
	__asm__ __volatile__("fxsaveq %0"
			     : "=m" (fpu->state->fxsave));
#else
	/* Using, as a workaround, the properly prefixed form below isn't
	   accepted by any binutils version so far released, complaining that
	   the same type of prefix is used twice if an extended register is
	   needed for addressing (fix submitted to mainline 2005-11-21).
	asm volatile("rex64/fxsave %0"
		     : "=m" (fpu->state->fxsave));
	   This, however, we can work around by forcing the compiler to select
	   an addressing mode that doesn't require extended registers. */
	asm volatile("rex64/fxsave (%[fx])"
		     : "=m" (fpu->state->fxsave)
		     : [fx] "R" (&fpu->state->fxsave));
#endif
}

#else  /* CONFIG_X86_32 */

/* perform fxrstor iff the processor has extended states, otherwise frstor */
static inline int fxrstor_checking(struct i387_fxsave_struct *fx)
{
	/*
	 * The "nop" is needed to make the instructions the same
	 * length.
	 */
	alternative_input(
		"nop ; frstor %1",
		"fxrstor %1",
		X86_FEATURE_FXSR,
		"m" (*fx));

	return 0;
=======
	if (config_enabled(CONFIG_X86_32))
		return user_insn(fxsave %[fx], [fx] "=m" (*fx), "m" (*fx));
	else if (config_enabled(CONFIG_AS_FXSAVEQ))
		return user_insn(fxsaveq %[fx], [fx] "=m" (*fx), "m" (*fx));

	/* See comment in fpu_fxsave() below. */
	return user_insn(rex64/fxsave (%[fx]), "=m" (*fx), [fx] "R" (fx));
}

static inline int fxrstor_checking(struct i387_fxsave_struct *fx)
{
	if (config_enabled(CONFIG_X86_32))
		return check_insn(fxrstor %[fx], "=m" (*fx), [fx] "m" (*fx));
	else if (config_enabled(CONFIG_AS_FXSAVEQ))
		return check_insn(fxrstorq %[fx], "=m" (*fx), [fx] "m" (*fx));

	/* See comment in fpu_fxsave() below. */
	return check_insn(rex64/fxrstor (%[fx]), "=m" (*fx), [fx] "R" (fx),
			  "m" (*fx));
}

static inline int fxrstor_user(struct i387_fxsave_struct __user *fx)
{
	if (config_enabled(CONFIG_X86_32))
		return user_insn(fxrstor %[fx], "=m" (*fx), [fx] "m" (*fx));
	else if (config_enabled(CONFIG_AS_FXSAVEQ))
		return user_insn(fxrstorq %[fx], "=m" (*fx), [fx] "m" (*fx));

	/* See comment in fpu_fxsave() below. */
	return user_insn(rex64/fxrstor (%[fx]), "=m" (*fx), [fx] "R" (fx),
			  "m" (*fx));
}

static inline int frstor_checking(struct i387_fsave_struct *fx)
{
	return check_insn(frstor %[fx], "=m" (*fx), [fx] "m" (*fx));
}

static inline int frstor_user(struct i387_fsave_struct __user *fx)
{
	return user_insn(frstor %[fx], "=m" (*fx), [fx] "m" (*fx));
>>>>>>> refs/remotes/origin/master
}

static inline void fpu_fxsave(struct fpu *fpu)
{
<<<<<<< HEAD
	asm volatile("fxsave %[fx]"
		     : [fx] "=m" (fpu->state->fxsave));
}

#endif	/* CONFIG_X86_64 */

=======
	if (config_enabled(CONFIG_X86_32))
		asm volatile( "fxsave %[fx]" : [fx] "=m" (fpu->state->fxsave));
	else if (config_enabled(CONFIG_AS_FXSAVEQ))
		asm volatile("fxsaveq %0" : "=m" (fpu->state->fxsave));
	else {
		/* Using "rex64; fxsave %0" is broken because, if the memory
		 * operand uses any extended registers for addressing, a second
		 * REX prefix will be generated (to the assembler, rex64
		 * followed by semicolon is a separate instruction), and hence
		 * the 64-bitness is lost.
		 *
		 * Using "fxsaveq %0" would be the ideal choice, but is only
		 * supported starting with gas 2.16.
		 *
		 * Using, as a workaround, the properly prefixed form below
		 * isn't accepted by any binutils version so far released,
		 * complaining that the same type of prefix is used twice if
		 * an extended register is needed for addressing (fix submitted
		 * to mainline 2005-11-21).
		 *
		 *  asm volatile("rex64/fxsave %0" : "=m" (fpu->state->fxsave));
		 *
		 * This, however, we can work around by forcing the compiler to
		 * select an addressing mode that doesn't require extended
		 * registers.
		 */
		asm volatile( "rex64/fxsave (%[fx])"
			     : "=m" (fpu->state->fxsave)
			     : [fx] "R" (&fpu->state->fxsave));
	}
}

>>>>>>> refs/remotes/origin/master
/*
 * These must be called with preempt disabled. Returns
 * 'true' if the FPU state is still intact.
 */
static inline int fpu_save_init(struct fpu *fpu)
{
	if (use_xsave()) {
		fpu_xsave(fpu);

		/*
		 * xsave header may indicate the init state of the FP.
		 */
		if (!(fpu->state->xsave.xsave_hdr.xstate_bv & XSTATE_FP))
			return 1;
	} else if (use_fxsr()) {
		fpu_fxsave(fpu);
	} else {
		asm volatile("fnsave %[fx]; fwait"
			     : [fx] "=m" (fpu->state->fsave));
		return 0;
	}

	/*
	 * If exceptions are pending, we need to clear them so
	 * that we don't randomly get exceptions later.
	 *
	 * FIXME! Is this perhaps only true for the old-style
	 * irq13 case? Maybe we could leave the x87 state
	 * intact otherwise?
	 */
	if (unlikely(fpu->state->fxsave.swd & X87_FSW_ES)) {
		asm volatile("fnclex");
		return 0;
	}
	return 1;
}

static inline int __save_init_fpu(struct task_struct *tsk)
{
	return fpu_save_init(&tsk->thread.fpu);
}

<<<<<<< HEAD
static inline int fpu_fxrstor_checking(struct fpu *fpu)
{
	return fxrstor_checking(&fpu->state->fxsave);
}

static inline int fpu_restore_checking(struct fpu *fpu)
{
	if (use_xsave())
		return fpu_xrstor_checking(fpu);
	else
		return fpu_fxrstor_checking(fpu);
=======
static inline int fpu_restore_checking(struct fpu *fpu)
{
	if (use_xsave())
		return fpu_xrstor_checking(&fpu->state->xsave);
	else if (use_fxsr())
		return fxrstor_checking(&fpu->state->fxsave);
	else
		return frstor_checking(&fpu->state->fsave);
>>>>>>> refs/remotes/origin/master
}

static inline int restore_fpu_checking(struct task_struct *tsk)
{
	/* AMD K7/K8 CPUs don't save/restore FDP/FIP/FOP unless an exception
	   is pending.  Clear the x87 state here by setting it to fixed
	   values. "m" is a random variable that should be in L1 */
<<<<<<< HEAD
	alternative_input(
		ASM_NOP8 ASM_NOP2,
		"emms\n\t"		/* clear stack tags */
		"fildl %P[addr]",	/* set F?P to defined value */
		X86_FEATURE_FXSAVE_LEAK,
		[addr] "m" (tsk->thread.fpu.has_fpu));
=======
	if (unlikely(static_cpu_has(X86_FEATURE_FXSAVE_LEAK))) {
		asm volatile(
			"fnclex\n\t"
			"emms\n\t"
			"fildl %P[addr]"	/* set F?P to defined value */
			: : [addr] "m" (tsk->thread.fpu.has_fpu));
	}
>>>>>>> refs/remotes/origin/master

	return fpu_restore_checking(&tsk->thread.fpu);
}

/*
 * Software FPU state helpers. Careful: these need to
 * be preemption protection *and* they need to be
 * properly paired with the CR0.TS changes!
 */
static inline int __thread_has_fpu(struct task_struct *tsk)
{
	return tsk->thread.fpu.has_fpu;
}

/* Must be paired with an 'stts' after! */
static inline void __thread_clear_has_fpu(struct task_struct *tsk)
{
	tsk->thread.fpu.has_fpu = 0;
<<<<<<< HEAD
	percpu_write(fpu_owner_task, NULL);
=======
	this_cpu_write(fpu_owner_task, NULL);
>>>>>>> refs/remotes/origin/master
}

/* Must be paired with a 'clts' before! */
static inline void __thread_set_has_fpu(struct task_struct *tsk)
{
	tsk->thread.fpu.has_fpu = 1;
<<<<<<< HEAD
	percpu_write(fpu_owner_task, tsk);
=======
	this_cpu_write(fpu_owner_task, tsk);
>>>>>>> refs/remotes/origin/master
}

/*
 * Encapsulate the CR0.TS handling together with the
 * software flag.
 *
 * These generally need preemption protection to work,
 * do try to avoid using these on their own.
 */
static inline void __thread_fpu_end(struct task_struct *tsk)
{
	__thread_clear_has_fpu(tsk);
<<<<<<< HEAD
	stts();
=======
	if (!use_eager_fpu())
		stts();
>>>>>>> refs/remotes/origin/master
}

static inline void __thread_fpu_begin(struct task_struct *tsk)
{
<<<<<<< HEAD
	clts();
	__thread_set_has_fpu(tsk);
}

=======
	if (!static_cpu_has_safe(X86_FEATURE_EAGER_FPU))
		clts();
	__thread_set_has_fpu(tsk);
}

static inline void __drop_fpu(struct task_struct *tsk)
{
	if (__thread_has_fpu(tsk)) {
		/* Ignore delayed exceptions from user space */
		asm volatile("1: fwait\n"
			     "2:\n"
			     _ASM_EXTABLE(1b, 2b));
		__thread_fpu_end(tsk);
	}
}

static inline void drop_fpu(struct task_struct *tsk)
{
	/*
	 * Forget coprocessor state..
	 */
	preempt_disable();
	tsk->thread.fpu_counter = 0;
	__drop_fpu(tsk);
	clear_used_math();
	preempt_enable();
}

static inline void drop_init_fpu(struct task_struct *tsk)
{
	if (!use_eager_fpu())
		drop_fpu(tsk);
	else {
		if (use_xsave())
			xrstor_state(init_xstate_buf, -1);
		else
			fxrstor_checking(&init_xstate_buf->i387);
	}
}

>>>>>>> refs/remotes/origin/master
/*
 * FPU state switching for scheduling.
 *
 * This is a two-stage process:
 *
 *  - switch_fpu_prepare() saves the old state and
 *    sets the new state of the CR0.TS bit. This is
 *    done within the context of the old process.
 *
 *  - switch_fpu_finish() restores the new state as
 *    necessary.
 */
typedef struct { int preload; } fpu_switch_t;

/*
 * Must be run with preemption disabled: this clears the fpu_owner_task,
 * on this CPU.
 *
 * This will disable any lazy FPU state restore of the current FPU state,
 * but if the current thread owns the FPU, it will still be saved by.
 */
static inline void __cpu_disable_lazy_restore(unsigned int cpu)
{
	per_cpu(fpu_owner_task, cpu) = NULL;
}

static inline int fpu_lazy_restore(struct task_struct *new, unsigned int cpu)
{
<<<<<<< HEAD
	return new == percpu_read_stable(fpu_owner_task) &&
=======
	return new == this_cpu_read_stable(fpu_owner_task) &&
>>>>>>> refs/remotes/origin/master
		cpu == new->thread.fpu.last_cpu;
}

static inline fpu_switch_t switch_fpu_prepare(struct task_struct *old, struct task_struct *new, int cpu)
{
	fpu_switch_t fpu;

<<<<<<< HEAD
	fpu.preload = tsk_used_math(new) && new->fpu_counter > 5;
=======
	/*
	 * If the task has used the math, pre-load the FPU on xsave processors
	 * or if the past 5 consecutive context-switches used math.
	 */
	fpu.preload = tsk_used_math(new) && (use_eager_fpu() ||
					     new->thread.fpu_counter > 5);
>>>>>>> refs/remotes/origin/master
	if (__thread_has_fpu(old)) {
		if (!__save_init_fpu(old))
			cpu = ~0;
		old->thread.fpu.last_cpu = cpu;
		old->thread.fpu.has_fpu = 0;	/* But leave fpu_owner_task! */

		/* Don't change CR0.TS if we just switch! */
		if (fpu.preload) {
<<<<<<< HEAD
			new->fpu_counter++;
			__thread_set_has_fpu(new);
			prefetch(new->thread.fpu.state);
		} else
			stts();
	} else {
		old->fpu_counter = 0;
		old->thread.fpu.last_cpu = ~0;
		if (fpu.preload) {
			new->fpu_counter++;
			if (fpu_lazy_restore(new, cpu))
=======
			new->thread.fpu_counter++;
			__thread_set_has_fpu(new);
			prefetch(new->thread.fpu.state);
		} else if (!use_eager_fpu())
			stts();
	} else {
		old->thread.fpu_counter = 0;
		old->thread.fpu.last_cpu = ~0;
		if (fpu.preload) {
			new->thread.fpu_counter++;
			if (!use_eager_fpu() && fpu_lazy_restore(new, cpu))
>>>>>>> refs/remotes/origin/master
				fpu.preload = 0;
			else
				prefetch(new->thread.fpu.state);
			__thread_fpu_begin(new);
		}
	}
	return fpu;
}

/*
 * By the time this gets called, we've already cleared CR0.TS and
 * given the process the FPU if we are going to preload the FPU
 * state - all we need to do is to conditionally restore the register
 * state itself.
 */
static inline void switch_fpu_finish(struct task_struct *new, fpu_switch_t fpu)
{
	if (fpu.preload) {
		if (unlikely(restore_fpu_checking(new)))
<<<<<<< HEAD
			__thread_fpu_end(new);
=======
			drop_init_fpu(new);
>>>>>>> refs/remotes/origin/master
	}
}

/*
 * Signal frame handlers...
 */
<<<<<<< HEAD
extern int save_i387_xstate(void __user *buf);
extern int restore_i387_xstate(void __user *buf);

static inline void __clear_fpu(struct task_struct *tsk)
{
	if (__thread_has_fpu(tsk)) {
		/* Ignore delayed exceptions from user space */
		asm volatile("1: fwait\n"
			     "2:\n"
			     _ASM_EXTABLE(1b, 2b));
		__thread_fpu_end(tsk);
	}
}

/*
 * The actual user_fpu_begin/end() functions
 * need to be preemption-safe.
 *
 * NOTE! user_fpu_end() must be used only after you
 * have saved the FP state, and user_fpu_begin() must
 * be used only immediately before restoring it.
 * These functions do not do any save/restore on
 * their own.
 */
static inline void user_fpu_end(void)
{
	preempt_disable();
	__thread_fpu_end(current);
	preempt_enable();
}

=======
extern int save_xstate_sig(void __user *buf, void __user *fx, int size);
extern int __restore_xstate_sig(void __user *buf, void __user *fx, int size);

static inline int xstate_sigframe_size(void)
{
	return use_xsave() ? xstate_size + FP_XSTATE_MAGIC2_SIZE : xstate_size;
}

static inline int restore_xstate_sig(void __user *buf, int ia32_frame)
{
	void __user *buf_fx = buf;
	int size = xstate_sigframe_size();

	if (ia32_frame && use_fxsr()) {
		buf_fx = buf + sizeof(struct i387_fsave_struct);
		size += sizeof(struct i387_fsave_struct);
	}

	return __restore_xstate_sig(buf, buf_fx, size);
}

/*
 * Need to be preemption-safe.
 *
 * NOTE! user_fpu_begin() must be used only immediately before restoring
 * it. This function does not do any save/restore on their own.
 */
>>>>>>> refs/remotes/origin/master
static inline void user_fpu_begin(void)
{
	preempt_disable();
	if (!user_has_fpu())
		__thread_fpu_begin(current);
	preempt_enable();
}

<<<<<<< HEAD
=======
static inline void __save_fpu(struct task_struct *tsk)
{
	if (use_xsave())
		xsave_state(&tsk->thread.fpu.state->xsave, -1);
	else
		fpu_fxsave(&tsk->thread.fpu);
}

>>>>>>> refs/remotes/origin/master
/*
 * These disable preemption on their own and are safe
 */
static inline void save_init_fpu(struct task_struct *tsk)
{
	WARN_ON_ONCE(!__thread_has_fpu(tsk));
<<<<<<< HEAD
=======

	if (use_eager_fpu()) {
		__save_fpu(tsk);
		return;
	}

>>>>>>> refs/remotes/origin/master
	preempt_disable();
	__save_init_fpu(tsk);
	__thread_fpu_end(tsk);
	preempt_enable();
}

<<<<<<< HEAD
static inline void clear_fpu(struct task_struct *tsk)
{
	preempt_disable();
	__clear_fpu(tsk);
	preempt_enable();
}

=======
>>>>>>> refs/remotes/origin/master
/*
 * i387 state interaction
 */
static inline unsigned short get_fpu_cwd(struct task_struct *tsk)
{
	if (cpu_has_fxsr) {
		return tsk->thread.fpu.state->fxsave.cwd;
	} else {
		return (unsigned short)tsk->thread.fpu.state->fsave.cwd;
	}
}

static inline unsigned short get_fpu_swd(struct task_struct *tsk)
{
	if (cpu_has_fxsr) {
		return tsk->thread.fpu.state->fxsave.swd;
	} else {
		return (unsigned short)tsk->thread.fpu.state->fsave.swd;
	}
}

static inline unsigned short get_fpu_mxcsr(struct task_struct *tsk)
{
	if (cpu_has_xmm) {
		return tsk->thread.fpu.state->fxsave.mxcsr;
	} else {
		return MXCSR_DEFAULT;
	}
}

static bool fpu_allocated(struct fpu *fpu)
{
	return fpu->state != NULL;
}

static inline int fpu_alloc(struct fpu *fpu)
{
	if (fpu_allocated(fpu))
		return 0;
	fpu->state = kmem_cache_alloc(task_xstate_cachep, GFP_KERNEL);
	if (!fpu->state)
		return -ENOMEM;
	WARN_ON((unsigned long)fpu->state & 15);
	return 0;
}

static inline void fpu_free(struct fpu *fpu)
{
	if (fpu->state) {
		kmem_cache_free(task_xstate_cachep, fpu->state);
		fpu->state = NULL;
	}
}

<<<<<<< HEAD
static inline void fpu_copy(struct fpu *dst, struct fpu *src)
{
	memcpy(dst->state, src->state, xstate_size);
}

extern void fpu_finit(struct fpu *fpu);
=======
static inline void fpu_copy(struct task_struct *dst, struct task_struct *src)
{
	if (use_eager_fpu()) {
		memset(&dst->thread.fpu.state->xsave, 0, xstate_size);
		__save_fpu(dst);
	} else {
		struct fpu *dfpu = &dst->thread.fpu;
		struct fpu *sfpu = &src->thread.fpu;

		unlazy_fpu(src);
		memcpy(dfpu->state, sfpu->state, xstate_size);
	}
}

static inline unsigned long
alloc_mathframe(unsigned long sp, int ia32_frame, unsigned long *buf_fx,
		unsigned long *size)
{
	unsigned long frame_size = xstate_sigframe_size();

	*buf_fx = sp = round_down(sp - frame_size, 64);
	if (ia32_frame && use_fxsr()) {
		frame_size += sizeof(struct i387_fsave_struct);
		sp -= sizeof(struct i387_fsave_struct);
	}

	*size = frame_size;
	return sp;
}
>>>>>>> refs/remotes/origin/master

#endif
