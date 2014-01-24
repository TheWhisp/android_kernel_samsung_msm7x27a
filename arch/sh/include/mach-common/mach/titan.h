/*
 * Platform definitions for Titan
 */
#ifndef _ASM_SH_TITAN_H
#define _ASM_SH_TITAN_H

<<<<<<< HEAD
=======
#include <linux/sh_intc.h>

>>>>>>> refs/remotes/origin/master
#define __IO_PREFIX titan
#include <asm/io_generic.h>

/* IRQ assignments */
<<<<<<< HEAD
#define TITAN_IRQ_WAN		2	/* eth0 (WAN) */
#define TITAN_IRQ_LAN		5	/* eth1 (LAN) */
#define TITAN_IRQ_MPCIA		8	/* mPCI A */
#define TITAN_IRQ_MPCIB		11	/* mPCI B */
#define TITAN_IRQ_USB		11	/* USB */
=======
#define TITAN_IRQ_WAN		evt2irq(0x240)	/* eth0 (WAN) */
#define TITAN_IRQ_LAN		evt2irq(0x2a0)	/* eth1 (LAN) */
#define TITAN_IRQ_MPCIA		evt2irq(0x300)	/* mPCI A */
#define TITAN_IRQ_MPCIB		evt2irq(0x360)	/* mPCI B */
#define TITAN_IRQ_USB		evt2irq(0x360)	/* USB */
>>>>>>> refs/remotes/origin/master

#endif /* __ASM_SH_TITAN_H */
