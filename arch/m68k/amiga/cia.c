/*
 *  linux/arch/m68k/amiga/cia.c - CIA support
 *
 *  Copyright (C) 1996 Roman Zippel
 *
 *  The concept of some functions bases on the original Amiga OS function
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive
 * for more details.
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/kernel_stat.h>
#include <linux/init.h>
#include <linux/seq_file.h>
#include <linux/interrupt.h>

#include <asm/irq.h>
#include <asm/amigahw.h>
#include <asm/amigaints.h>

struct ciabase {
	volatile struct CIA *cia;
	unsigned char icr_mask, icr_data;
	unsigned short int_mask;
	int handler_irq, cia_irq, server_irq;
	char *name;
} ciaa_base = {
	.cia		= &ciaa,
	.int_mask	= IF_PORTS,
	.handler_irq	= IRQ_AMIGA_PORTS,
	.cia_irq	= IRQ_AMIGA_CIAA,
	.name		= "CIAA"
}, ciab_base = {
	.cia		= &ciab,
	.int_mask	= IF_EXTER,
	.handler_irq	= IRQ_AMIGA_EXTER,
	.cia_irq	= IRQ_AMIGA_CIAB,
	.name		= "CIAB"
};

/*
 *  Cause or clear CIA interrupts, return old interrupt status.
 */

unsigned char cia_set_irq(struct ciabase *base, unsigned char mask)
{
	unsigned char old;

	old = (base->icr_data |= base->cia->icr);
	if (mask & CIA_ICR_SETCLR)
		base->icr_data |= mask;
	else
		base->icr_data &= ~mask;
	if (base->icr_data & base->icr_mask)
		amiga_custom.intreq = IF_SETCLR | base->int_mask;
	return old & base->icr_mask;
}

/*
 *  Enable or disable CIA interrupts, return old interrupt mask,
 */

unsigned char cia_able_irq(struct ciabase *base, unsigned char mask)
{
	unsigned char old;

	old = base->icr_mask;
	base->icr_data |= base->cia->icr;
	base->cia->icr = mask;
	if (mask & CIA_ICR_SETCLR)
		base->icr_mask |= mask;
	else
		base->icr_mask &= ~mask;
	base->icr_mask &= CIA_ICR_ALL;
	if (base->icr_data & base->icr_mask)
		amiga_custom.intreq = IF_SETCLR | base->int_mask;
	return old;
}

static irqreturn_t cia_handler(int irq, void *dev_id)
{
	struct ciabase *base = dev_id;
	int mach_irq;
	unsigned char ints;

	mach_irq = base->cia_irq;
	ints = cia_set_irq(base, CIA_ICR_ALL);
	amiga_custom.intreq = base->int_mask;
	for (; ints; mach_irq++, ints >>= 1) {
		if (ints & 1)
<<<<<<< HEAD
			m68k_handle_int(mach_irq);
=======
			generic_handle_irq(mach_irq);
>>>>>>> refs/remotes/origin/cm-10.0
	}
	return IRQ_HANDLED;
}

<<<<<<< HEAD
static void cia_enable_irq(unsigned int irq)
{
=======
static void cia_irq_enable(struct irq_data *data)
{
	unsigned int irq = data->irq;
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned char mask;

	if (irq >= IRQ_AMIGA_CIAB) {
		mask = 1 << (irq - IRQ_AMIGA_CIAB);
		cia_set_irq(&ciab_base, mask);
		cia_able_irq(&ciab_base, CIA_ICR_SETCLR | mask);
	} else {
		mask = 1 << (irq - IRQ_AMIGA_CIAA);
		cia_set_irq(&ciaa_base, mask);
		cia_able_irq(&ciaa_base, CIA_ICR_SETCLR | mask);
	}
}

<<<<<<< HEAD
static void cia_disable_irq(unsigned int irq)
{
=======
static void cia_irq_disable(struct irq_data *data)
{
	unsigned int irq = data->irq;

>>>>>>> refs/remotes/origin/cm-10.0
	if (irq >= IRQ_AMIGA_CIAB)
		cia_able_irq(&ciab_base, 1 << (irq - IRQ_AMIGA_CIAB));
	else
		cia_able_irq(&ciaa_base, 1 << (irq - IRQ_AMIGA_CIAA));
}

<<<<<<< HEAD
static struct irq_controller cia_irq_controller = {
	.name		= "cia",
	.lock		= __SPIN_LOCK_UNLOCKED(cia_irq_controller.lock),
	.enable		= cia_enable_irq,
	.disable	= cia_disable_irq,
=======
static struct irq_chip cia_irq_chip = {
	.name		= "cia",
	.irq_enable	= cia_irq_enable,
	.irq_disable	= cia_irq_disable,
>>>>>>> refs/remotes/origin/cm-10.0
};

/*
 * Override auto irq 2 & 6 and use them as general chain
 * for external interrupts, we link the CIA interrupt sources
 * into this chain.
 */

<<<<<<< HEAD
static void auto_enable_irq(unsigned int irq)
{
	switch (irq) {
=======
static void auto_irq_enable(struct irq_data *data)
{
	switch (data->irq) {
>>>>>>> refs/remotes/origin/cm-10.0
	case IRQ_AUTO_2:
		amiga_custom.intena = IF_SETCLR | IF_PORTS;
		break;
	case IRQ_AUTO_6:
		amiga_custom.intena = IF_SETCLR | IF_EXTER;
		break;
	}
}

<<<<<<< HEAD
static void auto_disable_irq(unsigned int irq)
{
	switch (irq) {
=======
static void auto_irq_disable(struct irq_data *data)
{
	switch (data->irq) {
>>>>>>> refs/remotes/origin/cm-10.0
	case IRQ_AUTO_2:
		amiga_custom.intena = IF_PORTS;
		break;
	case IRQ_AUTO_6:
		amiga_custom.intena = IF_EXTER;
		break;
	}
}

<<<<<<< HEAD
static struct irq_controller auto_irq_controller = {
	.name		= "auto",
	.lock		= __SPIN_LOCK_UNLOCKED(auto_irq_controller.lock),
	.enable		= auto_enable_irq,
	.disable	= auto_disable_irq,
=======
static struct irq_chip auto_irq_chip = {
	.name		= "auto",
	.irq_enable	= auto_irq_enable,
	.irq_disable	= auto_irq_disable,
>>>>>>> refs/remotes/origin/cm-10.0
};

void __init cia_init_IRQ(struct ciabase *base)
{
<<<<<<< HEAD
	m68k_setup_irq_controller(&cia_irq_controller, base->cia_irq, CIA_IRQS);
=======
	m68k_setup_irq_controller(&cia_irq_chip, handle_simple_irq,
				  base->cia_irq, CIA_IRQS);
>>>>>>> refs/remotes/origin/cm-10.0

	/* clear any pending interrupt and turn off all interrupts */
	cia_set_irq(base, CIA_ICR_ALL);
	cia_able_irq(base, CIA_ICR_ALL);

	/* override auto int and install CIA handler */
<<<<<<< HEAD
	m68k_setup_irq_controller(&auto_irq_controller, base->handler_irq, 1);
	m68k_irq_startup(base->handler_irq);
=======
	m68k_setup_irq_controller(&auto_irq_chip, handle_simple_irq,
				  base->handler_irq, 1);
	m68k_irq_startup_irq(base->handler_irq);
>>>>>>> refs/remotes/origin/cm-10.0
	if (request_irq(base->handler_irq, cia_handler, IRQF_SHARED,
			base->name, base))
		pr_err("Couldn't register %s interrupt\n", base->name);
}
