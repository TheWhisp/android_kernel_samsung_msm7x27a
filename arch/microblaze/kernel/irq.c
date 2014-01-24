/*
 * Copyright (C) 2007-2009 Michal Simek <monstr@monstr.eu>
 * Copyright (C) 2007-2009 PetaLogix
 * Copyright (C) 2006 Atmark Techno, Inc.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#include <linux/init.h>
#include <linux/ftrace.h>
#include <linux/kernel.h>
#include <linux/hardirq.h>
#include <linux/interrupt.h>
#include <linux/irqflags.h>
#include <linux/seq_file.h>
#include <linux/kernel_stat.h>
#include <linux/irq.h>
<<<<<<< HEAD
#include <linux/of_irq.h>
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0

#include <asm/prom.h>
=======
#include <linux/irqchip.h>
#include <linux/of_irq.h>
>>>>>>> refs/remotes/origin/master

static u32 concurrent_irq;

void __irq_entry do_IRQ(struct pt_regs *regs)
{
	unsigned int irq;
	struct pt_regs *old_regs = set_irq_regs(regs);
	trace_hardirqs_off();

	irq_enter();
<<<<<<< HEAD
<<<<<<< HEAD
	irq = get_irq(regs);
next_irq:
	BUG_ON(irq == -1U);
	generic_handle_irq(irq);

	irq = get_irq(regs);
=======
=======
>>>>>>> refs/remotes/origin/master
	irq = get_irq();
next_irq:
	BUG_ON(!irq);
	generic_handle_irq(irq);

	irq = get_irq();
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (irq != -1U) {
		pr_debug("next irq: %d\n", irq);
		++concurrent_irq;
		goto next_irq;
	}

	irq_exit();
	set_irq_regs(old_regs);
	trace_hardirqs_on();
}
<<<<<<< HEAD
<<<<<<< HEAD

/* MS: There is no any advance mapping mechanism. We are using simple 32bit
  intc without any cascades or any connection that's why mapping is 1:1 */
unsigned int irq_create_mapping(struct irq_host *host, irq_hw_number_t hwirq)
{
	return hwirq;
}
EXPORT_SYMBOL_GPL(irq_create_mapping);

unsigned int irq_create_of_mapping(struct device_node *controller,
				   const u32 *intspec, unsigned int intsize)
{
	return intspec[0];
}
EXPORT_SYMBOL_GPL(irq_create_of_mapping);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======

void __init init_IRQ(void)
{
	/* process the entire interrupt tree in one go */
	irqchip_init();
}
>>>>>>> refs/remotes/origin/master
