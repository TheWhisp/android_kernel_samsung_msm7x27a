/*
<<<<<<< HEAD
 * Copyright (C) 2007-2009 Michal Simek <monstr@monstr.eu>
=======
 * Copyright (C) 2007-2013 Michal Simek <monstr@monstr.eu>
 * Copyright (C) 2012-2013 Xilinx, Inc.
>>>>>>> refs/remotes/origin/master
 * Copyright (C) 2007-2009 PetaLogix
 * Copyright (C) 2006 Atmark Techno, Inc.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

<<<<<<< HEAD
#include <linux/init.h>
<<<<<<< HEAD
=======
#include <linux/irqdomain.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/irq.h>
#include <asm/page.h>
#include <linux/io.h>
#include <linux/bug.h>

#include <asm/prom.h>
#include <asm/irq.h>

#ifdef CONFIG_SELFMOD_INTC
#include <asm/selfmod.h>
#define INTC_BASE	BARRIER_BASE_ADDR
#else
static unsigned int intc_baseaddr;
#define INTC_BASE	intc_baseaddr
#endif

<<<<<<< HEAD
unsigned int nr_irq;

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/irqdomain.h>
#include <linux/irq.h>
#include <linux/of_address.h>
#include <linux/io.h>
#include <linux/bug.h>

#include "../../drivers/irqchip/irqchip.h"

static void __iomem *intc_baseaddr;

>>>>>>> refs/remotes/origin/master
/* No one else should require these constants, so define them locally here. */
#define ISR 0x00			/* Interrupt Status Register */
#define IPR 0x04			/* Interrupt Pending Register */
#define IER 0x08			/* Interrupt Enable Register */
#define IAR 0x0c			/* Interrupt Acknowledge Register */
#define SIE 0x10			/* Set Interrupt Enable bits */
#define CIE 0x14			/* Clear Interrupt Enable bits */
#define IVR 0x18			/* Interrupt Vector Register */
#define MER 0x1c			/* Master Enable Register */

#define MER_ME (1<<0)
#define MER_HIE (1<<1)

static void intc_enable_or_unmask(struct irq_data *d)
{
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long mask = 1 << d->irq;
	pr_debug("enable_or_unmask: %d\n", d->irq);
=======
	unsigned long mask = 1 << d->hwirq;

	pr_debug("enable_or_unmask: %ld\n", d->hwirq);
>>>>>>> refs/remotes/origin/cm-10.0
	out_be32(INTC_BASE + SIE, mask);
=======
	unsigned long mask = 1 << d->hwirq;

	pr_debug("enable_or_unmask: %ld\n", d->hwirq);
>>>>>>> refs/remotes/origin/master

	/* ack level irqs because they can't be acked during
	 * ack function since the handle_level_irq function
	 * acks the irq before calling the interrupt handler
	 */
	if (irqd_is_level_type(d))
<<<<<<< HEAD
		out_be32(INTC_BASE + IAR, mask);
=======
		out_be32(intc_baseaddr + IAR, mask);

	out_be32(intc_baseaddr + SIE, mask);
>>>>>>> refs/remotes/origin/master
}

static void intc_disable_or_mask(struct irq_data *d)
{
<<<<<<< HEAD
<<<<<<< HEAD
	pr_debug("disable: %d\n", d->irq);
	out_be32(INTC_BASE + CIE, 1 << d->irq);
=======
	pr_debug("disable: %ld\n", d->hwirq);
	out_be32(INTC_BASE + CIE, 1 << d->hwirq);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pr_debug("disable: %ld\n", d->hwirq);
	out_be32(intc_baseaddr + CIE, 1 << d->hwirq);
>>>>>>> refs/remotes/origin/master
}

static void intc_ack(struct irq_data *d)
{
<<<<<<< HEAD
<<<<<<< HEAD
	pr_debug("ack: %d\n", d->irq);
	out_be32(INTC_BASE + IAR, 1 << d->irq);
=======
	pr_debug("ack: %ld\n", d->hwirq);
	out_be32(INTC_BASE + IAR, 1 << d->hwirq);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pr_debug("ack: %ld\n", d->hwirq);
	out_be32(intc_baseaddr + IAR, 1 << d->hwirq);
>>>>>>> refs/remotes/origin/master
}

static void intc_mask_ack(struct irq_data *d)
{
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long mask = 1 << d->irq;
	pr_debug("disable_and_ack: %d\n", d->irq);
=======
	unsigned long mask = 1 << d->hwirq;

	pr_debug("disable_and_ack: %ld\n", d->hwirq);
>>>>>>> refs/remotes/origin/cm-10.0
	out_be32(INTC_BASE + CIE, mask);
	out_be32(INTC_BASE + IAR, mask);
=======
	unsigned long mask = 1 << d->hwirq;

	pr_debug("disable_and_ack: %ld\n", d->hwirq);
	out_be32(intc_baseaddr + CIE, mask);
	out_be32(intc_baseaddr + IAR, mask);
>>>>>>> refs/remotes/origin/master
}

static struct irq_chip intc_dev = {
	.name = "Xilinx INTC",
	.irq_unmask = intc_enable_or_unmask,
	.irq_mask = intc_disable_or_mask,
	.irq_ack = intc_ack,
	.irq_mask_ack = intc_mask_ack,
};

<<<<<<< HEAD
<<<<<<< HEAD
unsigned int get_irq(struct pt_regs *regs)
{
	int irq;

	/*
	 * NOTE: This function is the one that needs to be improved in
	 * order to handle multiple interrupt controllers. It currently
	 * is hardcoded to check for interrupts only on the first INTC.
	 */
	irq = in_be32(INTC_BASE + IVR);
	pr_debug("get_irq: %d\n", irq);
=======
=======
>>>>>>> refs/remotes/origin/master
static struct irq_domain *root_domain;

unsigned int get_irq(void)
{
	unsigned int hwirq, irq = -1;

<<<<<<< HEAD
	hwirq = in_be32(INTC_BASE + IVR);
=======
	hwirq = in_be32(intc_baseaddr + IVR);
>>>>>>> refs/remotes/origin/master
	if (hwirq != -1U)
		irq = irq_find_mapping(root_domain, hwirq);

	pr_debug("get_irq: hwirq=%d, irq=%d\n", hwirq, irq);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return irq;
}

<<<<<<< HEAD
<<<<<<< HEAD
void __init init_IRQ(void)
{
	u32 i, j, intr_type;
=======
int xintc_map(struct irq_domain *d, unsigned int irq, irq_hw_number_t hw)
=======
static int xintc_map(struct irq_domain *d, unsigned int irq, irq_hw_number_t hw)
>>>>>>> refs/remotes/origin/master
{
	u32 intr_mask = (u32)d->host_data;

	if (intr_mask & (1 << hw)) {
		irq_set_chip_and_handler_name(irq, &intc_dev,
						handle_edge_irq, "edge");
		irq_clear_status_flags(irq, IRQ_LEVEL);
	} else {
		irq_set_chip_and_handler_name(irq, &intc_dev,
						handle_level_irq, "level");
		irq_set_status_flags(irq, IRQ_LEVEL);
	}
	return 0;
}

static const struct irq_domain_ops xintc_irq_domain_ops = {
	.xlate = irq_domain_xlate_onetwocell,
	.map = xintc_map,
};

<<<<<<< HEAD
void __init init_IRQ(void)
{
	u32 nr_irq, intr_mask;
>>>>>>> refs/remotes/origin/cm-10.0
	struct device_node *intc = NULL;
#ifdef CONFIG_SELFMOD_INTC
	unsigned int intc_baseaddr = 0;
	static int arr_func[] = {
				(int)&get_irq,
				(int)&intc_enable_or_unmask,
				(int)&intc_disable_or_mask,
				(int)&intc_mask_ack,
				(int)&intc_ack,
				(int)&intc_end,
				0
			};
#endif
<<<<<<< HEAD
	const char * const intc_list[] = {
				"xlnx,xps-intc-1.00.a",
				NULL
			};

	for (j = 0; intc_list[j] != NULL; j++) {
		intc = of_find_compatible_node(NULL, NULL, intc_list[j]);
		if (intc)
			break;
	}
	BUG_ON(!intc);

	intc_baseaddr = be32_to_cpup(of_get_property(intc,
								"reg", NULL));
=======
	intc = of_find_compatible_node(NULL, NULL, "xlnx,xps-intc-1.00.a");
	BUG_ON(!intc);

	intc_baseaddr = be32_to_cpup(of_get_property(intc, "reg", NULL));
>>>>>>> refs/remotes/origin/cm-10.0
	intc_baseaddr = (unsigned long) ioremap(intc_baseaddr, PAGE_SIZE);
	nr_irq = be32_to_cpup(of_get_property(intc,
						"xlnx,num-intr-inputs", NULL));

<<<<<<< HEAD
	intr_type =
		be32_to_cpup(of_get_property(intc,
						"xlnx,kind-of-intr", NULL));
	if (intr_type >= (1 << (nr_irq + 1)))
=======
	intr_mask =
		be32_to_cpup(of_get_property(intc, "xlnx,kind-of-intr", NULL));
	if (intr_mask > (u32)((1ULL << nr_irq) - 1))
>>>>>>> refs/remotes/origin/cm-10.0
		printk(KERN_INFO " ERROR: Mismatch in kind-of-intr param\n");

#ifdef CONFIG_SELFMOD_INTC
	selfmod_function((int *) arr_func, intc_baseaddr);
#endif
	printk(KERN_INFO "%s #0 at 0x%08x, num_irq=%d, edge=0x%x\n",
<<<<<<< HEAD
		intc_list[j], intc_baseaddr, nr_irq, intr_type);
=======
		intc->name, intc_baseaddr, nr_irq, intr_mask);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int __init xilinx_intc_of_init(struct device_node *intc,
					     struct device_node *parent)
{
	u32 nr_irq, intr_mask;
	int ret;

	intc_baseaddr = of_iomap(intc, 0);
	BUG_ON(!intc_baseaddr);

	ret = of_property_read_u32(intc, "xlnx,num-intr-inputs", &nr_irq);
	if (ret < 0) {
		pr_err("%s: unable to read xlnx,num-intr-inputs\n", __func__);
		return -EINVAL;
	}

	ret = of_property_read_u32(intc, "xlnx,kind-of-intr", &intr_mask);
	if (ret < 0) {
		pr_err("%s: unable to read xlnx,kind-of-intr\n", __func__);
		return -EINVAL;
	}

	if (intr_mask > (u32)((1ULL << nr_irq) - 1))
		pr_info(" ERROR: Mismatch in kind-of-intr param\n");

	pr_info("%s: num_irq=%d, edge=0x%x\n",
		intc->full_name, nr_irq, intr_mask);
>>>>>>> refs/remotes/origin/master

	/*
	 * Disable all external interrupts until they are
	 * explicity requested.
	 */
	out_be32(intc_baseaddr + IER, 0);

	/* Acknowledge any pending interrupts just in case. */
	out_be32(intc_baseaddr + IAR, 0xffffffff);

	/* Turn on the Master Enable. */
	out_be32(intc_baseaddr + MER, MER_HIE | MER_ME);

<<<<<<< HEAD
<<<<<<< HEAD
	for (i = 0; i < nr_irq; ++i) {
		if (intr_type & (0x00000001 << i)) {
			irq_set_chip_and_handler_name(i, &intc_dev,
				handle_edge_irq, "edge");
			irq_clear_status_flags(i, IRQ_LEVEL);
		} else {
			irq_set_chip_and_handler_name(i, &intc_dev,
				handle_level_irq, "level");
			irq_set_status_flags(i, IRQ_LEVEL);
		}
	}
=======
=======
>>>>>>> refs/remotes/origin/master
	/* Yeah, okay, casting the intr_mask to a void* is butt-ugly, but I'm
	 * lazy and Michal can clean it up to something nicer when he tests
	 * and commits this patch.  ~~gcl */
	root_domain = irq_domain_add_linear(intc, nr_irq, &xintc_irq_domain_ops,
							(void *)intr_mask);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
}
=======

	irq_set_default_host(root_domain);

	return 0;
}

IRQCHIP_DECLARE(xilinx_intc, "xlnx,xps-intc-1.00.a", xilinx_intc_of_init);
>>>>>>> refs/remotes/origin/master
