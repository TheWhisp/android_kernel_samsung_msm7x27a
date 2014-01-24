/*
 * Copyright (C)2004-2010 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/io.h>

#include <asm/mach/irq.h>
<<<<<<< HEAD
=======
#include <asm/exception.h>
>>>>>>> refs/remotes/origin/cm-10.0

#include <mach/hardware.h>
#include <mach/common.h>

#include "irq-common.h"

/*
 *****************************************
 * TZIC Registers                        *
 *****************************************
 */

#define TZIC_INTCNTL	0x0000	/* Control register */
#define TZIC_INTTYPE	0x0004	/* Controller Type register */
#define TZIC_IMPID	0x0008	/* Distributor Implementer Identification */
#define TZIC_PRIOMASK	0x000C	/* Priority Mask Reg */
#define TZIC_SYNCCTRL	0x0010	/* Synchronizer Control register */
#define TZIC_DSMINT	0x0014	/* DSM interrupt Holdoffregister */
#define TZIC_INTSEC0(i)	(0x0080 + ((i) << 2)) /* Interrupt Security Reg 0 */
#define TZIC_ENSET0(i)	(0x0100 + ((i) << 2)) /* Enable Set Reg 0 */
#define TZIC_ENCLEAR0(i) (0x0180 + ((i) << 2)) /* Enable Clear Reg 0 */
#define TZIC_SRCSET0	0x0200	/* Source Set Register 0 */
#define TZIC_SRCCLAR0	0x0280	/* Source Clear Register 0 */
#define TZIC_PRIORITY0	0x0400	/* Priority Register 0 */
#define TZIC_PND0	0x0D00	/* Pending Register 0 */
<<<<<<< HEAD
#define TZIC_HIPND0	0x0D80	/* High Priority Pending Register */
=======
#define TZIC_HIPND(i)	(0x0D80+ ((i) << 2))	/* High Priority Pending Register */
>>>>>>> refs/remotes/origin/cm-10.0
#define TZIC_WAKEUP0(i)	(0x0E00 + ((i) << 2))	/* Wakeup Config Register */
#define TZIC_SWINT	0x0F00	/* Software Interrupt Rigger Register */
#define TZIC_ID0	0x0FD0	/* Indentification Register 0 */

void __iomem *tzic_base; /* Used as irq controller base in entry-macro.S */

<<<<<<< HEAD
=======
#define TZIC_NUM_IRQS 128

>>>>>>> refs/remotes/origin/cm-10.0
#ifdef CONFIG_FIQ
static int tzic_set_irq_fiq(unsigned int irq, unsigned int type)
{
	unsigned int index, mask, value;

	index = irq >> 5;
	if (unlikely(index >= 4))
		return -EINVAL;
	mask = 1U << (irq & 0x1F);

	value = __raw_readl(tzic_base + TZIC_INTSEC0(index)) | mask;
	if (type)
		value &= ~mask;
	__raw_writel(value, tzic_base + TZIC_INTSEC0(index));

	return 0;
}
<<<<<<< HEAD
#endif

/**
 * tzic_mask_irq() - Disable interrupt source "d" in the TZIC
 *
 * @param  d            interrupt source
 */
static void tzic_mask_irq(struct irq_data *d)
{
	int index, off;

	index = d->irq >> 5;
	off = d->irq & 0x1F;
	__raw_writel(1 << off, tzic_base + TZIC_ENCLEAR0(index));
}

/**
 * tzic_unmask_irq() - Enable interrupt source "d" in the TZIC
 *
 * @param  d            interrupt source
 */
static void tzic_unmask_irq(struct irq_data *d)
{
	int index, off;

	index = d->irq >> 5;
	off = d->irq & 0x1F;
	__raw_writel(1 << off, tzic_base + TZIC_ENSET0(index));
}

static unsigned int wakeup_intr[4];

/**
 * tzic_set_wake_irq() - Set interrupt source "d" in the TZIC as a wake-up source.
 *
 * @param  d            interrupt source
 * @param  enable       enable as wake-up if equal to non-zero
 * 			disble as wake-up if equal to zero
 *
 * @return       This function returns 0 on success.
 */
static int tzic_set_wake_irq(struct irq_data *d, unsigned int enable)
{
	unsigned int index, off;

	index = d->irq >> 5;
	off = d->irq & 0x1F;

	if (index > 3)
		return -EINVAL;

	if (enable)
		wakeup_intr[index] |= (1 << off);
	else
		wakeup_intr[index] &= ~(1 << off);

	return 0;
}

static struct mxc_irq_chip mxc_tzic_chip = {
	.base = {
		.name = "MXC_TZIC",
		.irq_ack = tzic_mask_irq,
		.irq_mask = tzic_mask_irq,
		.irq_unmask = tzic_unmask_irq,
		.irq_set_wake = tzic_set_wake_irq,
	},
=======
#else
#define tzic_set_irq_fiq NULL
#endif

#ifdef CONFIG_PM
static void tzic_irq_suspend(struct irq_data *d)
{
	struct irq_chip_generic *gc = irq_data_get_irq_chip_data(d);
	int idx = gc->irq_base >> 5;

	__raw_writel(gc->wake_active, tzic_base + TZIC_WAKEUP0(idx));
}

static void tzic_irq_resume(struct irq_data *d)
{
	struct irq_chip_generic *gc = irq_data_get_irq_chip_data(d);
	int idx = gc->irq_base >> 5;

	__raw_writel(__raw_readl(tzic_base + TZIC_ENSET0(idx)),
		     tzic_base + TZIC_WAKEUP0(idx));
}

#else
#define tzic_irq_suspend NULL
#define tzic_irq_resume NULL
#endif

static struct mxc_extra_irq tzic_extra_irq = {
>>>>>>> refs/remotes/origin/cm-10.0
#ifdef CONFIG_FIQ
	.set_irq_fiq = tzic_set_irq_fiq,
#endif
};

<<<<<<< HEAD
=======
static __init void tzic_init_gc(unsigned int irq_start)
{
	struct irq_chip_generic *gc;
	struct irq_chip_type *ct;
	int idx = irq_start >> 5;

	gc = irq_alloc_generic_chip("tzic", 1, irq_start, tzic_base,
				    handle_level_irq);
	gc->private = &tzic_extra_irq;
	gc->wake_enabled = IRQ_MSK(32);

	ct = gc->chip_types;
	ct->chip.irq_mask = irq_gc_mask_disable_reg;
	ct->chip.irq_unmask = irq_gc_unmask_enable_reg;
	ct->chip.irq_set_wake = irq_gc_set_wake;
	ct->chip.irq_suspend = tzic_irq_suspend;
	ct->chip.irq_resume = tzic_irq_resume;
	ct->regs.disable = TZIC_ENCLEAR0(idx);
	ct->regs.enable = TZIC_ENSET0(idx);

	irq_setup_generic_chip(gc, IRQ_MSK(32), 0, IRQ_NOREQUEST, 0);
}

asmlinkage void __exception_irq_entry tzic_handle_irq(struct pt_regs *regs)
{
	u32 stat;
	int i, irqofs, handled;

	do {
		handled = 0;

		for (i = 0; i < 4; i++) {
			stat = __raw_readl(tzic_base + TZIC_HIPND(i)) &
				__raw_readl(tzic_base + TZIC_INTSEC0(i));

			while (stat) {
				handled = 1;
				irqofs = fls(stat) - 1;
				handle_IRQ(irqofs + i * 32, regs);
				stat &= ~(1 << irqofs);
			}
		}
	} while (handled);
}

>>>>>>> refs/remotes/origin/cm-10.0
/*
 * This function initializes the TZIC hardware and disables all the
 * interrupts. It registers the interrupt enable and disable functions
 * to the kernel for each interrupt source.
 */
void __init tzic_init_irq(void __iomem *irqbase)
{
	int i;

	tzic_base = irqbase;
	/* put the TZIC into the reset value with
	 * all interrupts disabled
	 */
	i = __raw_readl(tzic_base + TZIC_INTCNTL);

	__raw_writel(0x80010001, tzic_base + TZIC_INTCNTL);
	__raw_writel(0x1f, tzic_base + TZIC_PRIOMASK);
	__raw_writel(0x02, tzic_base + TZIC_SYNCCTRL);

	for (i = 0; i < 4; i++)
		__raw_writel(0xFFFFFFFF, tzic_base + TZIC_INTSEC0(i));

	/* disable all interrupts */
	for (i = 0; i < 4; i++)
		__raw_writel(0xFFFFFFFF, tzic_base + TZIC_ENCLEAR0(i));

	/* all IRQ no FIQ Warning :: No selection */

<<<<<<< HEAD
	for (i = 0; i < MXC_INTERNAL_IRQS; i++) {
		irq_set_chip_and_handler(i, &mxc_tzic_chip.base,
					 handle_level_irq);
		set_irq_flags(i, IRQF_VALID);
	}
=======
	for (i = 0; i < TZIC_NUM_IRQS; i += 32)
		tzic_init_gc(i);
>>>>>>> refs/remotes/origin/cm-10.0

#ifdef CONFIG_FIQ
	/* Initialize FIQ */
	init_FIQ();
#endif

	pr_info("TrustZone Interrupt Controller (TZIC) initialized\n");
}

/**
 * tzic_enable_wake() - enable wakeup interrupt
 *
<<<<<<< HEAD
 * @param is_idle		1 if called in idle loop (ENSET0 register);
 *				0 to be used when called from low power entry
 * @return			0 if successful; non-zero otherwise
 */
int tzic_enable_wake(int is_idle)
{
	unsigned int i, v;
=======
 * @return			0 if successful; non-zero otherwise
 */
int tzic_enable_wake(void)
{
	unsigned int i;
>>>>>>> refs/remotes/origin/cm-10.0

	__raw_writel(1, tzic_base + TZIC_DSMINT);
	if (unlikely(__raw_readl(tzic_base + TZIC_DSMINT) == 0))
		return -EAGAIN;

<<<<<<< HEAD
	for (i = 0; i < 4; i++) {
		v = is_idle ? __raw_readl(tzic_base + TZIC_ENSET0(i)) :
			wakeup_intr[i];
		__raw_writel(v, tzic_base + TZIC_WAKEUP0(i));
	}
=======
	for (i = 0; i < 4; i++)
		__raw_writel(__raw_readl(tzic_base + TZIC_ENSET0(i)),
			     tzic_base + TZIC_WAKEUP0(i));
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
}
