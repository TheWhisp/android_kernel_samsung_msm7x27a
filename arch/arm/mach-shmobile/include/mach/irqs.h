#ifndef __ASM_MACH_IRQS_H
#define __ASM_MACH_IRQS_H

<<<<<<< HEAD
#define NR_IRQS         1024
=======
#include <linux/sh_intc.h>
>>>>>>> refs/remotes/origin/cm-10.0

/* GIC */
#define gic_spi(nr)		((nr) + 32)

<<<<<<< HEAD
/* INTCA */
#define evt2irq(evt)		(((evt) >> 5) - 16)
#define irq2evt(irq)		(((irq) + 16) << 5)

=======
>>>>>>> refs/remotes/origin/cm-10.0
/* INTCS */
#define INTCS_VECT_BASE		0x2200
#define INTCS_VECT(n, vect)	INTC_VECT((n), INTCS_VECT_BASE + (vect))
#define intcs_evt2irq(evt)	evt2irq(INTCS_VECT_BASE + (evt))

#endif /* __ASM_MACH_IRQS_H */
