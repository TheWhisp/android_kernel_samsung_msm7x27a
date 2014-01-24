/*
<<<<<<< HEAD
<<<<<<< HEAD
 *	OSS handling
=======
 *	Operating System Services (OSS) chip handling
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *	Operating System Services (OSS) chip handling
>>>>>>> refs/remotes/origin/master
 *	Written by Joshua M. Thompson (funaho@jurai.org)
 *
 *
 *	This chip is used in the IIfx in place of VIA #2. It acts like a fancy
 *	VIA chip with prorammable interrupt levels.
 *
 * 990502 (jmt) - Major rewrite for new interrupt architecture as well as some
 *		  recent insights into OSS operational details.
 * 990610 (jmt) - Now taking full advantage of the OSS. Interrupts are mapped
 *		  to mostly match the A/UX interrupt scheme supported on the
 *		  VIA side. Also added support for enabling the ISM irq again
 *		  since we now have a functional IOP manager.
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/delay.h>
#include <linux/init.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/irq.h>
>>>>>>> refs/remotes/origin/cm-10.0

#include <asm/bootinfo.h>
=======
#include <linux/irq.h>

>>>>>>> refs/remotes/origin/master
#include <asm/macintosh.h>
#include <asm/macints.h>
#include <asm/mac_via.h>
#include <asm/mac_oss.h>

int oss_present;
volatile struct mac_oss *oss;

<<<<<<< HEAD
<<<<<<< HEAD
static irqreturn_t oss_irq(int, void *);
static irqreturn_t oss_nubus_irq(int, void *);

extern irqreturn_t via1_irq(int, void *);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * Initialize the OSS
 *
 * The OSS "detection" code is actually in via_init() which is always called
 * before us. Thus we can count on oss_present being valid on entry.
 */

void __init oss_init(void)
{
	int i;

	if (!oss_present) return;

	oss = (struct mac_oss *) OSS_BASE;

	/* Disable all interrupts. Unlike a VIA it looks like we    */
	/* do this by setting the source's interrupt level to zero. */

	for (i = 0; i <= OSS_NUM_SOURCES; i++) {
<<<<<<< HEAD
<<<<<<< HEAD
		oss->irq_level[i] = OSS_IRQLEV_DISABLED;
	}
	/* If we disable VIA1 here, we never really handle it... */
	oss->irq_level[OSS_VIA1] = OSS_IRQLEV_VIA1;
}

/*
 * Register the OSS and NuBus interrupt dispatchers.
 */

void __init oss_register_interrupts(void)
{
	if (request_irq(OSS_IRQLEV_SCSI, oss_irq, IRQ_FLG_LOCK,
			"scsi", (void *) oss))
		pr_err("Couldn't register %s interrupt\n", "scsi");
	if (request_irq(OSS_IRQLEV_NUBUS, oss_nubus_irq, IRQ_FLG_LOCK,
			"nubus", (void *) oss))
		pr_err("Couldn't register %s interrupt\n", "nubus");
	if (request_irq(OSS_IRQLEV_SOUND, oss_irq, IRQ_FLG_LOCK,
			"sound", (void *) oss))
		pr_err("Couldn't register %s interrupt\n", "sound");
	if (request_irq(OSS_IRQLEV_VIA1, via1_irq, IRQ_FLG_LOCK,
			"via1", (void *) via1))
		pr_err("Couldn't register %s interrupt\n", "via1");
=======
		oss->irq_level[i] = 0;
	}
>>>>>>> refs/remotes/origin/cm-10.0
=======
		oss->irq_level[i] = 0;
	}
>>>>>>> refs/remotes/origin/master
}

/*
 * Initialize OSS for Nubus access
 */

void __init oss_nubus_init(void)
{
}

/*
<<<<<<< HEAD
<<<<<<< HEAD
 * Handle miscellaneous OSS interrupts. Right now that's just sound
 * and SCSI; everything else is routed to its own autovector IRQ.
 */

static irqreturn_t oss_irq(int irq, void *dev_id)
{
	int events;

	events = oss->irq_pending & (OSS_IP_SOUND|OSS_IP_SCSI);
	if (!events)
		return IRQ_NONE;

#ifdef DEBUG_IRQS
	if ((console_loglevel == 10) && !(events & OSS_IP_SCSI)) {
		printk("oss_irq: irq %d events = 0x%04X\n", irq,
			(int) oss->irq_pending);
	}
#endif
	/* FIXME: how do you clear a pending IRQ?    */

	if (events & OSS_IP_SOUND) {
		oss->irq_pending &= ~OSS_IP_SOUND;
		/* FIXME: call sound handler */
	} else if (events & OSS_IP_SCSI) {
		oss->irq_pending &= ~OSS_IP_SCSI;
		m68k_handle_int(IRQ_MAC_SCSI);
	} else {
		/* FIXME: error check here? */
	}
	return IRQ_HANDLED;
=======
=======
>>>>>>> refs/remotes/origin/master
 * Handle miscellaneous OSS interrupts.
 */

static void oss_irq(unsigned int irq, struct irq_desc *desc)
{
	int events = oss->irq_pending &
	             (OSS_IP_IOPSCC | OSS_IP_SCSI | OSS_IP_IOPISM);

#ifdef DEBUG_IRQS
	if ((console_loglevel == 10) && !(events & OSS_IP_SCSI)) {
		printk("oss_irq: irq %u events = 0x%04X\n", irq,
			(int) oss->irq_pending);
	}
#endif

	if (events & OSS_IP_IOPSCC) {
		oss->irq_pending &= ~OSS_IP_IOPSCC;
		generic_handle_irq(IRQ_MAC_SCC);
	}

	if (events & OSS_IP_SCSI) {
		oss->irq_pending &= ~OSS_IP_SCSI;
		generic_handle_irq(IRQ_MAC_SCSI);
	}

	if (events & OSS_IP_IOPISM) {
		oss->irq_pending &= ~OSS_IP_IOPISM;
		generic_handle_irq(IRQ_MAC_ADB);
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/*
 * Nubus IRQ handler, OSS style
 *
 * Unlike the VIA/RBV this is on its own autovector interrupt level.
 */

<<<<<<< HEAD
<<<<<<< HEAD
static irqreturn_t oss_nubus_irq(int irq, void *dev_id)
=======
static void oss_nubus_irq(unsigned int irq, struct irq_desc *desc)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void oss_nubus_irq(unsigned int irq, struct irq_desc *desc)
>>>>>>> refs/remotes/origin/master
{
	int events, irq_bit, i;

	events = oss->irq_pending & OSS_IP_NUBUS;
	if (!events)
<<<<<<< HEAD
<<<<<<< HEAD
		return IRQ_NONE;
=======
		return;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		return;
>>>>>>> refs/remotes/origin/master

#ifdef DEBUG_NUBUS_INT
	if (console_loglevel > 7) {
		printk("oss_nubus_irq: events = 0x%04X\n", events);
	}
#endif
	/* There are only six slots on the OSS, not seven */

	i = 6;
	irq_bit = 0x40;
	do {
		--i;
		irq_bit >>= 1;
		if (events & irq_bit) {
			oss->irq_pending &= ~irq_bit;
<<<<<<< HEAD
<<<<<<< HEAD
			m68k_handle_int(NUBUS_SOURCE_BASE + i);
		}
	} while(events & (irq_bit - 1));
	return IRQ_HANDLED;
=======
=======
>>>>>>> refs/remotes/origin/master
			generic_handle_irq(NUBUS_SOURCE_BASE + i);
		}
	} while(events & (irq_bit - 1));
}

/*
 * Register the OSS and NuBus interrupt dispatchers.
 *
 * This IRQ mapping is laid out with two things in mind: first, we try to keep
 * things on their own levels to avoid having to do double-dispatches. Second,
 * the levels match as closely as possible the alternate IRQ mapping mode (aka
 * "A/UX mode") available on some VIA machines.
 */

#define OSS_IRQLEV_IOPISM    IRQ_AUTO_1
#define OSS_IRQLEV_SCSI      IRQ_AUTO_2
#define OSS_IRQLEV_NUBUS     IRQ_AUTO_3
#define OSS_IRQLEV_IOPSCC    IRQ_AUTO_4
#define OSS_IRQLEV_VIA1      IRQ_AUTO_6

void __init oss_register_interrupts(void)
{
	irq_set_chained_handler(OSS_IRQLEV_IOPISM, oss_irq);
	irq_set_chained_handler(OSS_IRQLEV_SCSI,   oss_irq);
	irq_set_chained_handler(OSS_IRQLEV_NUBUS,  oss_nubus_irq);
	irq_set_chained_handler(OSS_IRQLEV_IOPSCC, oss_irq);
	irq_set_chained_handler(OSS_IRQLEV_VIA1,   via1_irq);

	/* OSS_VIA1 gets enabled here because it has no machspec interrupt. */
	oss->irq_level[OSS_VIA1] = IRQ_AUTO_6;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/*
 * Enable an OSS interrupt
 *
 * It looks messy but it's rather straightforward. The switch() statement
 * just maps the machspec interrupt numbers to the right OSS interrupt
 * source (if the OSS handles that interrupt) and then sets the interrupt
 * level for that source to nonzero, thus enabling the interrupt.
 */

void oss_irq_enable(int irq) {
#ifdef DEBUG_IRQUSE
	printk("oss_irq_enable(%d)\n", irq);
#endif
	switch(irq) {
		case IRQ_MAC_SCC:
			oss->irq_level[OSS_IOPSCC] = OSS_IRQLEV_IOPSCC;
<<<<<<< HEAD
<<<<<<< HEAD
			break;
		case IRQ_MAC_ADB:
			oss->irq_level[OSS_IOPISM] = OSS_IRQLEV_IOPISM;
			break;
		case IRQ_MAC_SCSI:
			oss->irq_level[OSS_SCSI] = OSS_IRQLEV_SCSI;
			break;
=======
=======
>>>>>>> refs/remotes/origin/master
			return;
		case IRQ_MAC_ADB:
			oss->irq_level[OSS_IOPISM] = OSS_IRQLEV_IOPISM;
			return;
		case IRQ_MAC_SCSI:
			oss->irq_level[OSS_SCSI] = OSS_IRQLEV_SCSI;
			return;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		case IRQ_NUBUS_9:
		case IRQ_NUBUS_A:
		case IRQ_NUBUS_B:
		case IRQ_NUBUS_C:
		case IRQ_NUBUS_D:
		case IRQ_NUBUS_E:
			irq -= NUBUS_SOURCE_BASE;
			oss->irq_level[irq] = OSS_IRQLEV_NUBUS;
<<<<<<< HEAD
<<<<<<< HEAD
			break;
#ifdef DEBUG_IRQUSE
		default:
			printk("%s unknown irq %d\n", __func__, irq);
			break;
#endif
	}
=======
=======
>>>>>>> refs/remotes/origin/master
			return;
	}

	if (IRQ_SRC(irq) == 1)
		via_irq_enable(irq);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/*
 * Disable an OSS interrupt
 *
 * Same as above except we set the source's interrupt level to zero,
 * to disable the interrupt.
 */

void oss_irq_disable(int irq) {
#ifdef DEBUG_IRQUSE
	printk("oss_irq_disable(%d)\n", irq);
#endif
	switch(irq) {
		case IRQ_MAC_SCC:
<<<<<<< HEAD
<<<<<<< HEAD
			oss->irq_level[OSS_IOPSCC] = OSS_IRQLEV_DISABLED;
			break;
		case IRQ_MAC_ADB:
			oss->irq_level[OSS_IOPISM] = OSS_IRQLEV_DISABLED;
			break;
		case IRQ_MAC_SCSI:
			oss->irq_level[OSS_SCSI] = OSS_IRQLEV_DISABLED;
			break;
		case IRQ_NUBUS_9:
		case IRQ_NUBUS_A:
		case IRQ_NUBUS_B:
		case IRQ_NUBUS_C:
		case IRQ_NUBUS_D:
		case IRQ_NUBUS_E:
			irq -= NUBUS_SOURCE_BASE;
			oss->irq_level[irq] = OSS_IRQLEV_DISABLED;
			break;
#ifdef DEBUG_IRQUSE
		default:
			printk("%s unknown irq %d\n", __func__, irq);
			break;
#endif
	}
}

/*
 * Clear an OSS interrupt
 *
 * Not sure if this works or not but it's the only method I could
 * think of based on the contents of the mac_oss structure.
 */

void oss_irq_clear(int irq) {
	/* FIXME: how to do this on OSS? */
	switch(irq) {
		case IRQ_MAC_SCC:
			oss->irq_pending &= ~OSS_IP_IOPSCC;
			break;
		case IRQ_MAC_ADB:
			oss->irq_pending &= ~OSS_IP_IOPISM;
			break;
		case IRQ_MAC_SCSI:
			oss->irq_pending &= ~OSS_IP_SCSI;
			break;
=======
=======
>>>>>>> refs/remotes/origin/master
			oss->irq_level[OSS_IOPSCC] = 0;
			return;
		case IRQ_MAC_ADB:
			oss->irq_level[OSS_IOPISM] = 0;
			return;
		case IRQ_MAC_SCSI:
			oss->irq_level[OSS_SCSI] = 0;
			return;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		case IRQ_NUBUS_9:
		case IRQ_NUBUS_A:
		case IRQ_NUBUS_B:
		case IRQ_NUBUS_C:
		case IRQ_NUBUS_D:
		case IRQ_NUBUS_E:
			irq -= NUBUS_SOURCE_BASE;
<<<<<<< HEAD
<<<<<<< HEAD
			oss->irq_pending &= ~(1 << irq);
			break;
	}
}

/*
 * Check to see if a specific OSS interrupt is pending
 */

int oss_irq_pending(int irq)
{
	switch(irq) {
		case IRQ_MAC_SCC:
			return oss->irq_pending & OSS_IP_IOPSCC;
			break;
		case IRQ_MAC_ADB:
			return oss->irq_pending & OSS_IP_IOPISM;
			break;
		case IRQ_MAC_SCSI:
			return oss->irq_pending & OSS_IP_SCSI;
			break;
		case IRQ_NUBUS_9:
		case IRQ_NUBUS_A:
		case IRQ_NUBUS_B:
		case IRQ_NUBUS_C:
		case IRQ_NUBUS_D:
		case IRQ_NUBUS_E:
			irq -= NUBUS_SOURCE_BASE;
			return oss->irq_pending & (1 << irq);
			break;
	}
	return 0;
=======
=======
>>>>>>> refs/remotes/origin/master
			oss->irq_level[irq] = 0;
			return;
	}

	if (IRQ_SRC(irq) == 1)
		via_irq_disable(irq);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}
