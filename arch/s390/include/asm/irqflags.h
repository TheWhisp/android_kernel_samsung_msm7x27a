/*
<<<<<<< HEAD
 *    Copyright IBM Corp. 2006,2010
=======
 *    Copyright IBM Corp. 2006, 2010
>>>>>>> refs/remotes/origin/master
 *    Author(s): Martin Schwidefsky <schwidefsky@de.ibm.com>
 */

#ifndef __ASM_IRQFLAGS_H
#define __ASM_IRQFLAGS_H

#include <linux/types.h>

/* store then OR system mask. */
#define __arch_local_irq_stosm(__or)					\
({									\
	unsigned long __mask;						\
	asm volatile(							\
		"	stosm	%0,%1"					\
		: "=Q" (__mask) : "i" (__or) : "memory");		\
	__mask;								\
})

/* store then AND system mask. */
#define __arch_local_irq_stnsm(__and)					\
({									\
	unsigned long __mask;						\
	asm volatile(							\
		"	stnsm	%0,%1"					\
		: "=Q" (__mask) : "i" (__and) : "memory");		\
	__mask;								\
})

/* set system mask. */
<<<<<<< HEAD
<<<<<<< HEAD
static inline void __arch_local_irq_ssm(unsigned long flags)
=======
static inline notrace void __arch_local_irq_ssm(unsigned long flags)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline notrace void __arch_local_irq_ssm(unsigned long flags)
>>>>>>> refs/remotes/origin/master
{
	asm volatile("ssm   %0" : : "Q" (flags) : "memory");
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline unsigned long arch_local_save_flags(void)
=======
static inline notrace unsigned long arch_local_save_flags(void)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline notrace unsigned long arch_local_save_flags(void)
>>>>>>> refs/remotes/origin/master
{
	return __arch_local_irq_stosm(0x00);
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline unsigned long arch_local_irq_save(void)
=======
static inline notrace unsigned long arch_local_irq_save(void)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline notrace unsigned long arch_local_irq_save(void)
>>>>>>> refs/remotes/origin/master
{
	return __arch_local_irq_stnsm(0xfc);
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline void arch_local_irq_disable(void)
=======
static inline notrace void arch_local_irq_disable(void)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline notrace void arch_local_irq_disable(void)
>>>>>>> refs/remotes/origin/master
{
	arch_local_irq_save();
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline void arch_local_irq_enable(void)
=======
static inline notrace void arch_local_irq_enable(void)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline notrace void arch_local_irq_enable(void)
>>>>>>> refs/remotes/origin/master
{
	__arch_local_irq_stosm(0x03);
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline void arch_local_irq_restore(unsigned long flags)
=======
static inline notrace void arch_local_irq_restore(unsigned long flags)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline notrace void arch_local_irq_restore(unsigned long flags)
>>>>>>> refs/remotes/origin/master
{
	__arch_local_irq_ssm(flags);
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline bool arch_irqs_disabled_flags(unsigned long flags)
=======
static inline notrace bool arch_irqs_disabled_flags(unsigned long flags)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline notrace bool arch_irqs_disabled_flags(unsigned long flags)
>>>>>>> refs/remotes/origin/master
{
	return !(flags & (3UL << (BITS_PER_LONG - 8)));
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline bool arch_irqs_disabled(void)
=======
static inline notrace bool arch_irqs_disabled(void)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static inline notrace bool arch_irqs_disabled(void)
>>>>>>> refs/remotes/origin/master
{
	return arch_irqs_disabled_flags(arch_local_save_flags());
}

#endif /* __ASM_IRQFLAGS_H */
