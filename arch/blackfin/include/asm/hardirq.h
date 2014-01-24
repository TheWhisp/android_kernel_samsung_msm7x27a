/*
 * Copyright 2004-2009 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef __BFIN_HARDIRQ_H
#define __BFIN_HARDIRQ_H

#define __ARCH_IRQ_EXIT_IRQS_DISABLED	1

extern void ack_bad_irq(unsigned int irq);
#define ack_bad_irq ack_bad_irq

<<<<<<< HEAD
/* Define until common code gets sane defaults */
#define HARDIRQ_BITS 9

=======
>>>>>>> refs/remotes/origin/master
#include <asm-generic/hardirq.h>

#endif
