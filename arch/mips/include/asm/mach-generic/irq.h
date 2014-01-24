/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2003 by Ralf Baechle
 */
#ifndef __ASM_MACH_GENERIC_IRQ_H
#define __ASM_MACH_GENERIC_IRQ_H

#ifndef NR_IRQS
<<<<<<< HEAD
#define NR_IRQS	128
=======
#define NR_IRQS 128
>>>>>>> refs/remotes/origin/master
#endif

#ifdef CONFIG_I8259
#ifndef I8259A_IRQ_BASE
<<<<<<< HEAD
#define I8259A_IRQ_BASE	0
=======
#define I8259A_IRQ_BASE 0
>>>>>>> refs/remotes/origin/master
#endif
#endif

#ifdef CONFIG_IRQ_CPU

#ifndef MIPS_CPU_IRQ_BASE
#ifdef CONFIG_I8259
#define MIPS_CPU_IRQ_BASE 16
#else
#define MIPS_CPU_IRQ_BASE 0
#endif /* CONFIG_I8259 */
#endif

#ifdef CONFIG_IRQ_CPU_RM7K
#ifndef RM7K_CPU_IRQ_BASE
#define RM7K_CPU_IRQ_BASE (MIPS_CPU_IRQ_BASE+8)
#endif
#endif

<<<<<<< HEAD
#ifdef CONFIG_IRQ_CPU_RM9K
#ifndef RM9K_CPU_IRQ_BASE
#define RM9K_CPU_IRQ_BASE (MIPS_CPU_IRQ_BASE+12)
#endif
#endif

=======
>>>>>>> refs/remotes/origin/master
#endif /* CONFIG_IRQ_CPU */

#endif /* __ASM_MACH_GENERIC_IRQ_H */
