#ifndef __ASM_HARDIRQ_H
#define __ASM_HARDIRQ_H

#include <asm/irq.h>
<<<<<<< HEAD

#define HARDIRQ_BITS	8

/*
 * The hardirq mask has to be large enough to have
 * space for potentially all IRQ sources in the system
 * nesting on a single CPU:
 */
#if (1 << HARDIRQ_BITS) < NR_IRQS
# error HARDIRQ_BITS is too low!
#endif

=======
>>>>>>> refs/remotes/origin/master
#include <asm-generic/hardirq.h>

#endif /* __ASM_HARDIRQ_H */
