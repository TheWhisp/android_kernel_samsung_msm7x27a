/*
<<<<<<< HEAD
 *  include/asm-s390/hardirq.h
 *
 *  S390 version
 *    Copyright (C) 1999,2000 IBM Deutschland Entwicklung GmbH, IBM Corporation
=======
 *  S390 version
 *    Copyright IBM Corp. 1999, 2000
>>>>>>> refs/remotes/origin/master
 *    Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com),
 *               Denis Joseph Barrow (djbarrow@de.ibm.com,barrow_dj@yahoo.com)
 *
 *  Derived from "include/asm-i386/hardirq.h"
 */

#ifndef __ASM_HARDIRQ_H
#define __ASM_HARDIRQ_H

#include <asm/lowcore.h>

#define local_softirq_pending() (S390_lowcore.softirq_pending)

#define __ARCH_IRQ_STAT
#define __ARCH_HAS_DO_SOFTIRQ
<<<<<<< HEAD
<<<<<<< HEAD
=======
#define __ARCH_IRQ_EXIT_IRQS_DISABLED
>>>>>>> refs/remotes/origin/cm-10.0

#define HARDIRQ_BITS	8
=======
#define __ARCH_IRQ_EXIT_IRQS_DISABLED

static inline void ack_bad_irq(unsigned int irq)
{
	printk(KERN_CRIT "unexpected IRQ trap at vector %02x\n", irq);
}
>>>>>>> refs/remotes/origin/master

#endif /* __ASM_HARDIRQ_H */
