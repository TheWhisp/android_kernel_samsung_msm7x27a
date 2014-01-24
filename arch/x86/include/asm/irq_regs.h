/*
 * Per-cpu current frame pointer - the location of the last exception frame on
 * the stack, stored in the per-cpu area.
 *
 * Jeremy Fitzhardinge <jeremy@goop.org>
 */
#ifndef _ASM_X86_IRQ_REGS_H
#define _ASM_X86_IRQ_REGS_H

#include <asm/percpu.h>

#define ARCH_HAS_OWN_IRQ_REGS

DECLARE_PER_CPU(struct pt_regs *, irq_regs);

static inline struct pt_regs *get_irq_regs(void)
{
<<<<<<< HEAD
	return percpu_read(irq_regs);
=======
	return this_cpu_read(irq_regs);
>>>>>>> refs/remotes/origin/master
}

static inline struct pt_regs *set_irq_regs(struct pt_regs *new_regs)
{
	struct pt_regs *old_regs;

	old_regs = get_irq_regs();
<<<<<<< HEAD
	percpu_write(irq_regs, new_regs);
=======
	this_cpu_write(irq_regs, new_regs);
>>>>>>> refs/remotes/origin/master

	return old_regs;
}

#endif /* _ASM_X86_IRQ_REGS_32_H */
