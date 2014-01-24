/*
<<<<<<< HEAD
 * linux/arch/sh/boards/se/7722/irq.c
 *
 * Copyright (C) 2007  Nobuhiro Iwamatsu
 *
 * Hitachi UL SolutionEngine 7722 Support.
=======
 * Hitachi UL SolutionEngine 7722 FPGA IRQ Support.
 *
 * Copyright (C) 2007  Nobuhiro Iwamatsu
 * Copyright (C) 2012  Paul Mundt
>>>>>>> refs/remotes/origin/master
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
<<<<<<< HEAD
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <mach-se/mach/se7722.h>

unsigned int se7722_fpga_irq[SE7722_FPGA_IRQ_NR] = { 0, };

static void disable_se7722_irq(struct irq_data *data)
{
	unsigned int bit = (unsigned int)irq_data_get_irq_chip_data(data);
	__raw_writew(__raw_readw(IRQ01_MASK) | 1 << bit, IRQ01_MASK);
}

static void enable_se7722_irq(struct irq_data *data)
{
	unsigned int bit = (unsigned int)irq_data_get_irq_chip_data(data);
	__raw_writew(__raw_readw(IRQ01_MASK) & ~(1 << bit), IRQ01_MASK);
}

static struct irq_chip se7722_irq_chip __read_mostly = {
	.name		= "SE7722-FPGA",
	.irq_mask	= disable_se7722_irq,
	.irq_unmask	= enable_se7722_irq,
};

static void se7722_irq_demux(unsigned int irq, struct irq_desc *desc)
{
	unsigned short intv = __raw_readw(IRQ01_STS);
	unsigned int ext_irq = 0;

	intv &= (1 << SE7722_FPGA_IRQ_NR) - 1;

	for (; intv; intv >>= 1, ext_irq++) {
		if (!(intv & 1))
			continue;

		generic_handle_irq(se7722_fpga_irq[ext_irq]);
	}
}

/*
 * Initialize IRQ setting
 */
void __init init_se7722_IRQ(void)
{
	int i, irq;

	__raw_writew(0, IRQ01_MASK);       /* disable all irqs */
	__raw_writew(0x2000, 0xb03fffec);  /* mrshpc irq enable */

	for (i = 0; i < SE7722_FPGA_IRQ_NR; i++) {
		irq = create_irq();
		if (irq < 0)
			return;
		se7722_fpga_irq[i] = irq;

		irq_set_chip_and_handler_name(se7722_fpga_irq[i],
					      &se7722_irq_chip,
					      handle_level_irq,
					      "level");

		irq_set_chip_data(se7722_fpga_irq[i], (void *)i);
	}
=======
#define DRV_NAME "SE7722-FPGA"
#define pr_fmt(fmt) DRV_NAME ": " fmt

#define irq_reg_readl	ioread16
#define irq_reg_writel	iowrite16

#include <linux/init.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/irqdomain.h>
#include <linux/io.h>
#include <linux/err.h>
#include <asm/sizes.h>
#include <mach-se/mach/se7722.h>

#define IRQ01_BASE_ADDR	0x11800000
#define IRQ01_MODE_REG	0
#define IRQ01_STS_REG	4
#define IRQ01_MASK_REG	8

static void __iomem *se7722_irq_regs;
struct irq_domain *se7722_irq_domain;

static void se7722_irq_demux(unsigned int irq, struct irq_desc *desc)
{
	struct irq_data *data = irq_get_irq_data(irq);
	struct irq_chip *chip = irq_data_get_irq_chip(data);
	unsigned long mask;
	int bit;

	chip->irq_mask_ack(data);

	mask = ioread16(se7722_irq_regs + IRQ01_STS_REG);

	for_each_set_bit(bit, &mask, SE7722_FPGA_IRQ_NR)
		generic_handle_irq(irq_linear_revmap(se7722_irq_domain, bit));

	chip->irq_unmask(data);
}

static void __init se7722_domain_init(void)
{
	int i;

	se7722_irq_domain = irq_domain_add_linear(NULL, SE7722_FPGA_IRQ_NR,
						  &irq_domain_simple_ops, NULL);
	if (unlikely(!se7722_irq_domain)) {
		printk("Failed to get IRQ domain\n");
		return;
	}

	for (i = 0; i < SE7722_FPGA_IRQ_NR; i++) {
		int irq = irq_create_mapping(se7722_irq_domain, i);

		if (unlikely(irq == 0)) {
			printk("Failed to allocate IRQ %d\n", i);
			return;
		}
	}
}

static void __init se7722_gc_init(void)
{
	struct irq_chip_generic *gc;
	struct irq_chip_type *ct;
	unsigned int irq_base;

	irq_base = irq_linear_revmap(se7722_irq_domain, 0);

	gc = irq_alloc_generic_chip(DRV_NAME, 1, irq_base, se7722_irq_regs,
				    handle_level_irq);
	if (unlikely(!gc))
		return;

	ct = gc->chip_types;
	ct->chip.irq_mask = irq_gc_mask_set_bit;
	ct->chip.irq_unmask = irq_gc_mask_clr_bit;

	ct->regs.mask = IRQ01_MASK_REG;

	irq_setup_generic_chip(gc, IRQ_MSK(SE7722_FPGA_IRQ_NR),
			       IRQ_GC_INIT_MASK_CACHE,
			       IRQ_NOREQUEST | IRQ_NOPROBE, 0);
>>>>>>> refs/remotes/origin/master

	irq_set_chained_handler(IRQ0_IRQ, se7722_irq_demux);
	irq_set_irq_type(IRQ0_IRQ, IRQ_TYPE_LEVEL_LOW);

	irq_set_chained_handler(IRQ1_IRQ, se7722_irq_demux);
	irq_set_irq_type(IRQ1_IRQ, IRQ_TYPE_LEVEL_LOW);
}
<<<<<<< HEAD
=======

/*
 * Initialize FPGA IRQs
 */
void __init init_se7722_IRQ(void)
{
	se7722_irq_regs = ioremap(IRQ01_BASE_ADDR, SZ_16);
	if (unlikely(!se7722_irq_regs)) {
		printk("Failed to remap IRQ01 regs\n");
		return;
	}

	/*
	 * All FPGA IRQs disabled by default
	 */
	iowrite16(0, se7722_irq_regs + IRQ01_MASK_REG);

	__raw_writew(0x2000, 0xb03fffec);  /* mrshpc irq enable */

	se7722_domain_init();
	se7722_gc_init();
}
>>>>>>> refs/remotes/origin/master
