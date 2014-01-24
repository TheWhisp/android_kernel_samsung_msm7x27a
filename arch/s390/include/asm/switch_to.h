/*
 * Copyright IBM Corp. 1999, 2009
 *
 * Author(s): Martin Schwidefsky <schwidefsky@de.ibm.com>
 */

#ifndef __ASM_SWITCH_TO_H
#define __ASM_SWITCH_TO_H

#include <linux/thread_info.h>
<<<<<<< HEAD

extern struct task_struct *__switch_to(void *, void *);
extern void update_per_regs(struct task_struct *task);

static inline void save_fp_regs(s390_fp_regs *fpregs)
{
	asm volatile(
		"	std	0,%O0+8(%R0)\n"
		"	std	2,%O0+24(%R0)\n"
		"	std	4,%O0+40(%R0)\n"
		"	std	6,%O0+56(%R0)"
		: "=Q" (*fpregs) : "Q" (*fpregs));
	if (!MACHINE_HAS_IEEE)
		return;
	asm volatile(
		"	stfpc	%0\n"
		"	std	1,%O0+16(%R0)\n"
		"	std	3,%O0+32(%R0)\n"
		"	std	5,%O0+48(%R0)\n"
		"	std	7,%O0+64(%R0)\n"
		"	std	8,%O0+72(%R0)\n"
		"	std	9,%O0+80(%R0)\n"
		"	std	10,%O0+88(%R0)\n"
		"	std	11,%O0+96(%R0)\n"
		"	std	12,%O0+104(%R0)\n"
		"	std	13,%O0+112(%R0)\n"
		"	std	14,%O0+120(%R0)\n"
		"	std	15,%O0+128(%R0)\n"
		: "=Q" (*fpregs) : "Q" (*fpregs));
}

static inline void restore_fp_regs(s390_fp_regs *fpregs)
{
	asm volatile(
		"	ld	0,%O0+8(%R0)\n"
		"	ld	2,%O0+24(%R0)\n"
		"	ld	4,%O0+40(%R0)\n"
		"	ld	6,%O0+56(%R0)"
		: : "Q" (*fpregs));
	if (!MACHINE_HAS_IEEE)
		return;
	asm volatile(
		"	lfpc	%0\n"
		"	ld	1,%O0+16(%R0)\n"
		"	ld	3,%O0+32(%R0)\n"
		"	ld	5,%O0+48(%R0)\n"
		"	ld	7,%O0+64(%R0)\n"
		"	ld	8,%O0+72(%R0)\n"
		"	ld	9,%O0+80(%R0)\n"
		"	ld	10,%O0+88(%R0)\n"
		"	ld	11,%O0+96(%R0)\n"
		"	ld	12,%O0+104(%R0)\n"
		"	ld	13,%O0+112(%R0)\n"
		"	ld	14,%O0+120(%R0)\n"
		"	ld	15,%O0+128(%R0)\n"
		: : "Q" (*fpregs));
=======
#include <asm/ptrace.h>

extern struct task_struct *__switch_to(void *, void *);
extern void update_cr_regs(struct task_struct *task);

static inline int test_fp_ctl(u32 fpc)
{
	u32 orig_fpc;
	int rc;

	if (!MACHINE_HAS_IEEE)
		return 0;

	asm volatile(
		"	efpc    %1\n"
		"	sfpc	%2\n"
		"0:	sfpc	%1\n"
		"	la	%0,0\n"
		"1:\n"
		EX_TABLE(0b,1b)
		: "=d" (rc), "=d" (orig_fpc)
		: "d" (fpc), "0" (-EINVAL));
	return rc;
}

static inline void save_fp_ctl(u32 *fpc)
{
	if (!MACHINE_HAS_IEEE)
		return;

	asm volatile(
		"       stfpc   %0\n"
		: "+Q" (*fpc));
}

static inline int restore_fp_ctl(u32 *fpc)
{
	int rc;

	if (!MACHINE_HAS_IEEE)
		return 0;

	asm volatile(
		"0:	lfpc    %1\n"
		"	la	%0,0\n"
		"1:\n"
		EX_TABLE(0b,1b)
		: "=d" (rc) : "Q" (*fpc), "0" (-EINVAL));
	return rc;
}

static inline void save_fp_regs(freg_t *fprs)
{
	asm volatile("std 0,%0" : "=Q" (fprs[0]));
	asm volatile("std 2,%0" : "=Q" (fprs[2]));
	asm volatile("std 4,%0" : "=Q" (fprs[4]));
	asm volatile("std 6,%0" : "=Q" (fprs[6]));
	if (!MACHINE_HAS_IEEE)
		return;
	asm volatile("std 1,%0" : "=Q" (fprs[1]));
	asm volatile("std 3,%0" : "=Q" (fprs[3]));
	asm volatile("std 5,%0" : "=Q" (fprs[5]));
	asm volatile("std 7,%0" : "=Q" (fprs[7]));
	asm volatile("std 8,%0" : "=Q" (fprs[8]));
	asm volatile("std 9,%0" : "=Q" (fprs[9]));
	asm volatile("std 10,%0" : "=Q" (fprs[10]));
	asm volatile("std 11,%0" : "=Q" (fprs[11]));
	asm volatile("std 12,%0" : "=Q" (fprs[12]));
	asm volatile("std 13,%0" : "=Q" (fprs[13]));
	asm volatile("std 14,%0" : "=Q" (fprs[14]));
	asm volatile("std 15,%0" : "=Q" (fprs[15]));
}

static inline void restore_fp_regs(freg_t *fprs)
{
	asm volatile("ld 0,%0" : : "Q" (fprs[0]));
	asm volatile("ld 2,%0" : : "Q" (fprs[2]));
	asm volatile("ld 4,%0" : : "Q" (fprs[4]));
	asm volatile("ld 6,%0" : : "Q" (fprs[6]));
	if (!MACHINE_HAS_IEEE)
		return;
	asm volatile("ld 1,%0" : : "Q" (fprs[1]));
	asm volatile("ld 3,%0" : : "Q" (fprs[3]));
	asm volatile("ld 5,%0" : : "Q" (fprs[5]));
	asm volatile("ld 7,%0" : : "Q" (fprs[7]));
	asm volatile("ld 8,%0" : : "Q" (fprs[8]));
	asm volatile("ld 9,%0" : : "Q" (fprs[9]));
	asm volatile("ld 10,%0" : : "Q" (fprs[10]));
	asm volatile("ld 11,%0" : : "Q" (fprs[11]));
	asm volatile("ld 12,%0" : : "Q" (fprs[12]));
	asm volatile("ld 13,%0" : : "Q" (fprs[13]));
	asm volatile("ld 14,%0" : : "Q" (fprs[14]));
	asm volatile("ld 15,%0" : : "Q" (fprs[15]));
>>>>>>> refs/remotes/origin/master
}

static inline void save_access_regs(unsigned int *acrs)
{
<<<<<<< HEAD
	asm volatile("stam 0,15,%0" : "=Q" (*acrs));
=======
	typedef struct { int _[NUM_ACRS]; } acrstype;

	asm volatile("stam 0,15,%0" : "=Q" (*(acrstype *)acrs));
>>>>>>> refs/remotes/origin/master
}

static inline void restore_access_regs(unsigned int *acrs)
{
<<<<<<< HEAD
	asm volatile("lam 0,15,%0" : : "Q" (*acrs));
=======
	typedef struct { int _[NUM_ACRS]; } acrstype;

	asm volatile("lam 0,15,%0" : : "Q" (*(acrstype *)acrs));
>>>>>>> refs/remotes/origin/master
}

#define switch_to(prev,next,last) do {					\
	if (prev->mm) {							\
<<<<<<< HEAD
		save_fp_regs(&prev->thread.fp_regs);			\
		save_access_regs(&prev->thread.acrs[0]);		\
	}								\
	if (next->mm) {							\
		restore_fp_regs(&next->thread.fp_regs);			\
		restore_access_regs(&next->thread.acrs[0]);		\
		update_per_regs(next);					\
=======
		save_fp_ctl(&prev->thread.fp_regs.fpc);			\
		save_fp_regs(prev->thread.fp_regs.fprs);		\
		save_access_regs(&prev->thread.acrs[0]);		\
		save_ri_cb(prev->thread.ri_cb);				\
	}								\
	if (next->mm) {							\
		restore_fp_ctl(&next->thread.fp_regs.fpc);		\
		restore_fp_regs(next->thread.fp_regs.fprs);		\
		restore_access_regs(&next->thread.acrs[0]);		\
		restore_ri_cb(next->thread.ri_cb, prev->thread.ri_cb);	\
		update_cr_regs(next);					\
>>>>>>> refs/remotes/origin/master
	}								\
	prev = __switch_to(prev,next);					\
} while (0)

<<<<<<< HEAD
extern void account_vtime(struct task_struct *, struct task_struct *);
extern void account_tick_vtime(struct task_struct *);

#define finish_arch_switch(prev) do {					     \
	set_fs(current->thread.mm_segment);				     \
	account_vtime(prev, current);					     \
=======
#define finish_arch_switch(prev) do {					     \
	set_fs(current->thread.mm_segment);				     \
>>>>>>> refs/remotes/origin/master
} while (0)

#endif /* __ASM_SWITCH_TO_H */
