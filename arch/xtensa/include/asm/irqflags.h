/*
 * Xtensa IRQ flags handling functions
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2001 - 2005 Tensilica Inc.
 */

#ifndef _XTENSA_IRQFLAGS_H
#define _XTENSA_IRQFLAGS_H

#include <linux/types.h>

static inline unsigned long arch_local_save_flags(void)
{
	unsigned long flags;
<<<<<<< HEAD
	asm volatile("rsr %0,"__stringify(PS) : "=a" (flags));
=======
	asm volatile("rsr %0, ps" : "=a" (flags));
>>>>>>> refs/remotes/origin/master
	return flags;
}

static inline unsigned long arch_local_irq_save(void)
{
	unsigned long flags;
	asm volatile("rsil %0, "__stringify(LOCKLEVEL)
		     : "=a" (flags) :: "memory");
	return flags;
}

static inline void arch_local_irq_disable(void)
{
	arch_local_irq_save();
}

static inline void arch_local_irq_enable(void)
{
	unsigned long flags;
	asm volatile("rsil %0, 0" : "=a" (flags) :: "memory");
}

static inline void arch_local_irq_restore(unsigned long flags)
{
<<<<<<< HEAD
	asm volatile("wsr %0, "__stringify(PS)" ; rsync"
=======
	asm volatile("wsr %0, ps; rsync"
>>>>>>> refs/remotes/origin/master
		     :: "a" (flags) : "memory");
}

static inline bool arch_irqs_disabled_flags(unsigned long flags)
{
<<<<<<< HEAD
	return (flags & 0xf) != 0;
=======
#if XCHAL_EXCM_LEVEL < LOCKLEVEL || (1 << PS_EXCM_BIT) < LOCKLEVEL
#error "XCHAL_EXCM_LEVEL and 1<<PS_EXCM_BIT must be no less than LOCKLEVEL"
#endif
	return (flags & (PS_INTLEVEL_MASK | (1 << PS_EXCM_BIT))) >= LOCKLEVEL;
>>>>>>> refs/remotes/origin/master
}

static inline bool arch_irqs_disabled(void)
{
	return arch_irqs_disabled_flags(arch_local_save_flags());
}

#endif /* _XTENSA_IRQFLAGS_H */
