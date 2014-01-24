#ifndef BCM63XX_IRQ_H_
#define BCM63XX_IRQ_H_

#include <bcm63xx_cpu.h>

<<<<<<< HEAD
#define IRQ_MIPS_BASE			0
#define IRQ_INTERNAL_BASE		8

#define IRQ_EXT_BASE			(IRQ_MIPS_BASE + 3)
#define IRQ_EXT_0			(IRQ_EXT_BASE + 0)
#define IRQ_EXT_1			(IRQ_EXT_BASE + 1)
#define IRQ_EXT_2			(IRQ_EXT_BASE + 2)
#define IRQ_EXT_3			(IRQ_EXT_BASE + 3)
=======
#define IRQ_INTERNAL_BASE		8
#define IRQ_EXTERNAL_BASE		100
#define IRQ_EXT_0			(IRQ_EXTERNAL_BASE + 0)
#define IRQ_EXT_1			(IRQ_EXTERNAL_BASE + 1)
#define IRQ_EXT_2			(IRQ_EXTERNAL_BASE + 2)
#define IRQ_EXT_3			(IRQ_EXTERNAL_BASE + 3)
>>>>>>> refs/remotes/origin/cm-10.0

#endif /* ! BCM63XX_IRQ_H_ */
