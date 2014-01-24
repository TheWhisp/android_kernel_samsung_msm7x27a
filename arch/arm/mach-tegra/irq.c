/*
 * Copyright (C) 2011 Google, Inc.
 *
 * Author:
 *	Colin Cross <ccross@android.com>
 *
<<<<<<< HEAD
 * Copyright (C) 2010, NVIDIA Corporation
=======
 * Copyright (C) 2010,2013, NVIDIA Corporation
>>>>>>> refs/remotes/origin/master
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/io.h>
<<<<<<< HEAD
=======
#include <linux/of.h>
>>>>>>> refs/remotes/origin/cm-10.0

#include <asm/hardware/gic.h>

#include <mach/iomap.h>

#include "board.h"

<<<<<<< HEAD
#define INT_SYS_NR	(INT_GPIO_BASE - INT_PRI_BASE)
#define INT_SYS_SZ	(INT_SEC_BASE - INT_PRI_BASE)
#define PPI_NR		((INT_SYS_NR+INT_SYS_SZ-1)/INT_SYS_SZ)

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/cpu_pm.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/irqchip/arm-gic.h>
#include <linux/syscore_ops.h>

#include "board.h"
#include "iomap.h"

>>>>>>> refs/remotes/origin/master
#define ICTLR_CPU_IEP_VFIQ	0x08
#define ICTLR_CPU_IEP_FIR	0x14
#define ICTLR_CPU_IEP_FIR_SET	0x18
#define ICTLR_CPU_IEP_FIR_CLR	0x1c

#define ICTLR_CPU_IER		0x20
#define ICTLR_CPU_IER_SET	0x24
#define ICTLR_CPU_IER_CLR	0x28
#define ICTLR_CPU_IEP_CLASS	0x2C

#define ICTLR_COP_IER		0x30
#define ICTLR_COP_IER_SET	0x34
#define ICTLR_COP_IER_CLR	0x38
#define ICTLR_COP_IEP_CLASS	0x3c

<<<<<<< HEAD
<<<<<<< HEAD
#define NUM_ICTLRS 4
#define FIRST_LEGACY_IRQ 32

=======
#define FIRST_LEGACY_IRQ 32

static int num_ictlrs;

>>>>>>> refs/remotes/origin/cm-10.0
=======
#define FIRST_LEGACY_IRQ 32
#define TEGRA_MAX_NUM_ICTLRS	5

#define SGI_MASK 0xFFFF

static int num_ictlrs;

>>>>>>> refs/remotes/origin/master
static void __iomem *ictlr_reg_base[] = {
	IO_ADDRESS(TEGRA_PRIMARY_ICTLR_BASE),
	IO_ADDRESS(TEGRA_SECONDARY_ICTLR_BASE),
	IO_ADDRESS(TEGRA_TERTIARY_ICTLR_BASE),
	IO_ADDRESS(TEGRA_QUATERNARY_ICTLR_BASE),
<<<<<<< HEAD
<<<<<<< HEAD
=======
	IO_ADDRESS(TEGRA_QUINARY_ICTLR_BASE),
>>>>>>> refs/remotes/origin/cm-10.0
};

=======
	IO_ADDRESS(TEGRA_QUINARY_ICTLR_BASE),
};

#ifdef CONFIG_PM_SLEEP
static u32 cop_ier[TEGRA_MAX_NUM_ICTLRS];
static u32 cop_iep[TEGRA_MAX_NUM_ICTLRS];
static u32 cpu_ier[TEGRA_MAX_NUM_ICTLRS];
static u32 cpu_iep[TEGRA_MAX_NUM_ICTLRS];

static u32 ictlr_wake_mask[TEGRA_MAX_NUM_ICTLRS];
static void __iomem *tegra_gic_cpu_base;
#endif

bool tegra_pending_sgi(void)
{
	u32 pending_set;
	void __iomem *distbase = IO_ADDRESS(TEGRA_ARM_INT_DIST_BASE);

	pending_set = readl_relaxed(distbase + GIC_DIST_PENDING_SET);

	if (pending_set & SGI_MASK)
		return true;

	return false;
}

>>>>>>> refs/remotes/origin/master
static inline void tegra_irq_write_mask(unsigned int irq, unsigned long reg)
{
	void __iomem *base;
	u32 mask;

	BUG_ON(irq < FIRST_LEGACY_IRQ ||
<<<<<<< HEAD
<<<<<<< HEAD
		irq >= FIRST_LEGACY_IRQ + NUM_ICTLRS * 32);
=======
		irq >= FIRST_LEGACY_IRQ + num_ictlrs * 32);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		irq >= FIRST_LEGACY_IRQ + num_ictlrs * 32);
>>>>>>> refs/remotes/origin/master

	base = ictlr_reg_base[(irq - FIRST_LEGACY_IRQ) / 32];
	mask = BIT((irq - FIRST_LEGACY_IRQ) % 32);

	__raw_writel(mask, base + reg);
}

static void tegra_mask(struct irq_data *d)
{
	if (d->irq < FIRST_LEGACY_IRQ)
		return;

	tegra_irq_write_mask(d->irq, ICTLR_CPU_IER_CLR);
}

static void tegra_unmask(struct irq_data *d)
{
	if (d->irq < FIRST_LEGACY_IRQ)
		return;

	tegra_irq_write_mask(d->irq, ICTLR_CPU_IER_SET);
}

static void tegra_ack(struct irq_data *d)
{
	if (d->irq < FIRST_LEGACY_IRQ)
		return;

	tegra_irq_write_mask(d->irq, ICTLR_CPU_IEP_FIR_CLR);
}

static void tegra_eoi(struct irq_data *d)
{
	if (d->irq < FIRST_LEGACY_IRQ)
		return;

	tegra_irq_write_mask(d->irq, ICTLR_CPU_IEP_FIR_CLR);
}

static int tegra_retrigger(struct irq_data *d)
{
	if (d->irq < FIRST_LEGACY_IRQ)
		return 0;

	tegra_irq_write_mask(d->irq, ICTLR_CPU_IEP_FIR_SET);

	return 1;
}

<<<<<<< HEAD
void __init tegra_init_irq(void)
{
	int i;
<<<<<<< HEAD

	for (i = 0; i < NUM_ICTLRS; i++) {
=======
=======
#ifdef CONFIG_PM_SLEEP
static int tegra_set_wake(struct irq_data *d, unsigned int enable)
{
	u32 irq = d->irq;
	u32 index, mask;

	if (irq < FIRST_LEGACY_IRQ ||
		irq >= FIRST_LEGACY_IRQ + num_ictlrs * 32)
		return -EINVAL;

	index = ((irq - FIRST_LEGACY_IRQ) / 32);
	mask = BIT((irq - FIRST_LEGACY_IRQ) % 32);
	if (enable)
		ictlr_wake_mask[index] |= mask;
	else
		ictlr_wake_mask[index] &= ~mask;

	return 0;
}

static int tegra_legacy_irq_suspend(void)
{
	unsigned long flags;
	int i;

	local_irq_save(flags);
	for (i = 0; i < num_ictlrs; i++) {
		void __iomem *ictlr = ictlr_reg_base[i];
		/* Save interrupt state */
		cpu_ier[i] = readl_relaxed(ictlr + ICTLR_CPU_IER);
		cpu_iep[i] = readl_relaxed(ictlr + ICTLR_CPU_IEP_CLASS);
		cop_ier[i] = readl_relaxed(ictlr + ICTLR_COP_IER);
		cop_iep[i] = readl_relaxed(ictlr + ICTLR_COP_IEP_CLASS);

		/* Disable COP interrupts */
		writel_relaxed(~0ul, ictlr + ICTLR_COP_IER_CLR);

		/* Disable CPU interrupts */
		writel_relaxed(~0ul, ictlr + ICTLR_CPU_IER_CLR);

		/* Enable the wakeup sources of ictlr */
		writel_relaxed(ictlr_wake_mask[i], ictlr + ICTLR_CPU_IER_SET);
	}
	local_irq_restore(flags);

	return 0;
}

static void tegra_legacy_irq_resume(void)
{
	unsigned long flags;
	int i;

	local_irq_save(flags);
	for (i = 0; i < num_ictlrs; i++) {
		void __iomem *ictlr = ictlr_reg_base[i];
		writel_relaxed(cpu_iep[i], ictlr + ICTLR_CPU_IEP_CLASS);
		writel_relaxed(~0ul, ictlr + ICTLR_CPU_IER_CLR);
		writel_relaxed(cpu_ier[i], ictlr + ICTLR_CPU_IER_SET);
		writel_relaxed(cop_iep[i], ictlr + ICTLR_COP_IEP_CLASS);
		writel_relaxed(~0ul, ictlr + ICTLR_COP_IER_CLR);
		writel_relaxed(cop_ier[i], ictlr + ICTLR_COP_IER_SET);
	}
	local_irq_restore(flags);
}

static struct syscore_ops tegra_legacy_irq_syscore_ops = {
	.suspend = tegra_legacy_irq_suspend,
	.resume = tegra_legacy_irq_resume,
};

int tegra_legacy_irq_syscore_init(void)
{
	register_syscore_ops(&tegra_legacy_irq_syscore_ops);

	return 0;
}

static int tegra_gic_notifier(struct notifier_block *self,
			      unsigned long cmd, void *v)
{
	switch (cmd) {
	case CPU_PM_ENTER:
		writel_relaxed(0x1E0, tegra_gic_cpu_base + GIC_CPU_CTRL);
		break;
	}

	return NOTIFY_OK;
}

static struct notifier_block tegra_gic_notifier_block = {
	.notifier_call = tegra_gic_notifier,
};

static const struct of_device_id tegra114_dt_gic_match[] __initconst = {
	{ .compatible = "arm,cortex-a15-gic" },
	{ }
};

static void tegra114_gic_cpu_pm_registration(void)
{
	struct device_node *dn;

	dn = of_find_matching_node(NULL, tegra114_dt_gic_match);
	if (!dn)
		return;

	tegra_gic_cpu_base = of_iomap(dn, 1);

	cpu_pm_register_notifier(&tegra_gic_notifier_block);
}
#else
#define tegra_set_wake NULL
static void tegra114_gic_cpu_pm_registration(void) { }
#endif

void __init tegra_init_irq(void)
{
	int i;
>>>>>>> refs/remotes/origin/master
	void __iomem *distbase;

	distbase = IO_ADDRESS(TEGRA_ARM_INT_DIST_BASE);
	num_ictlrs = readl_relaxed(distbase + GIC_DIST_CTR) & 0x1f;

	if (num_ictlrs > ARRAY_SIZE(ictlr_reg_base)) {
		WARN(1, "Too many (%d) interrupt controllers found. Maximum is %d.",
			num_ictlrs, ARRAY_SIZE(ictlr_reg_base));
		num_ictlrs = ARRAY_SIZE(ictlr_reg_base);
	}

	for (i = 0; i < num_ictlrs; i++) {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		void __iomem *ictlr = ictlr_reg_base[i];
		writel(~0, ictlr + ICTLR_CPU_IER_CLR);
		writel(0, ictlr + ICTLR_CPU_IEP_CLASS);
	}

	gic_arch_extn.irq_ack = tegra_ack;
	gic_arch_extn.irq_eoi = tegra_eoi;
	gic_arch_extn.irq_mask = tegra_mask;
	gic_arch_extn.irq_unmask = tegra_unmask;
	gic_arch_extn.irq_retrigger = tegra_retrigger;
<<<<<<< HEAD

<<<<<<< HEAD
	gic_init(0, 29, IO_ADDRESS(TEGRA_ARM_INT_DIST_BASE),
		 IO_ADDRESS(TEGRA_ARM_PERIF_BASE + 0x100));
=======
=======
	gic_arch_extn.irq_set_wake = tegra_set_wake;
	gic_arch_extn.flags = IRQCHIP_MASK_ON_SUSPEND;

>>>>>>> refs/remotes/origin/master
	/*
	 * Check if there is a devicetree present, since the GIC will be
	 * initialized elsewhere under DT.
	 */
	if (!of_have_populated_dt())
		gic_init(0, 29, distbase,
			IO_ADDRESS(TEGRA_ARM_PERIF_BASE + 0x100));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======

	tegra114_gic_cpu_pm_registration();
>>>>>>> refs/remotes/origin/master
}
