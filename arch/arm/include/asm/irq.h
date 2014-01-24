#ifndef __ASM_ARM_IRQ_H
#define __ASM_ARM_IRQ_H

<<<<<<< HEAD
<<<<<<< HEAD
#include <mach/irqs.h>
=======
=======
>>>>>>> refs/remotes/origin/master
#define NR_IRQS_LEGACY	16

#ifndef CONFIG_SPARSE_IRQ
#include <mach/irqs.h>
#else
#define NR_IRQS NR_IRQS_LEGACY
#endif
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#ifndef irq_canonicalize
#define irq_canonicalize(i)	(i)
#endif

<<<<<<< HEAD
<<<<<<< HEAD
#define NR_IRQS_LEGACY	16

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * Use this value to indicate lack of interrupt
 * capability
 */
#ifndef NO_IRQ
#define NO_IRQ	((unsigned int)(-1))
#endif

#ifndef __ASSEMBLY__
struct irqaction;
struct pt_regs;
extern void migrate_irqs(void);

extern void asm_do_IRQ(unsigned int, struct pt_regs *);
void handle_IRQ(unsigned int, struct pt_regs *);
void init_IRQ(void);

<<<<<<< HEAD
<<<<<<< HEAD
void arch_trigger_all_cpu_backtrace(void);
#define arch_trigger_all_cpu_backtrace arch_trigger_all_cpu_backtrace
=======
#ifdef CONFIG_MULTI_IRQ_HANDLER
extern void (*handle_arch_irq)(struct pt_regs *);
extern void set_handle_irq(void (*handle_irq)(struct pt_regs *));
#endif
>>>>>>> refs/remotes/origin/master
=======
void arch_trigger_all_cpu_backtrace(void);
#define arch_trigger_all_cpu_backtrace arch_trigger_all_cpu_backtrace
>>>>>>> refs/remotes/origin/cm-11.0

#endif

#endif

