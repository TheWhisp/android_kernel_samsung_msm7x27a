/*
 * arch/sh/kernel/cpu/sh4a/clock-sh7763.c
 *
 * SH7763 support for the clock framework
 *
 *  Copyright (C) 2005  Paul Mundt
 *  Copyright (C) 2007  Yoshihiro Shimoda
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/clkdev.h>
#include <asm/clock.h>
#include <asm/freq.h>
#include <asm/io.h>

static int bfc_divisors[] = { 1, 1, 1, 8, 1, 1, 1, 1 };
static int p0fc_divisors[] = { 1, 1, 1, 8, 1, 1, 1, 1 };
static int cfc_divisors[] = { 1, 1, 4, 1, 1, 1, 1, 1 };

static void master_clk_init(struct clk *clk)
{
	clk->rate *= p0fc_divisors[(__raw_readl(FRQCR) >> 4) & 0x07];
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct clk_ops sh7763_master_clk_ops = {
=======
static struct sh_clk_ops sh7763_master_clk_ops = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct sh_clk_ops sh7763_master_clk_ops = {
>>>>>>> refs/remotes/origin/master
	.init		= master_clk_init,
};

static unsigned long module_clk_recalc(struct clk *clk)
{
	int idx = ((__raw_readl(FRQCR) >> 4) & 0x07);
	return clk->parent->rate / p0fc_divisors[idx];
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct clk_ops sh7763_module_clk_ops = {
=======
static struct sh_clk_ops sh7763_module_clk_ops = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct sh_clk_ops sh7763_module_clk_ops = {
>>>>>>> refs/remotes/origin/master
	.recalc		= module_clk_recalc,
};

static unsigned long bus_clk_recalc(struct clk *clk)
{
	int idx = ((__raw_readl(FRQCR) >> 16) & 0x07);
	return clk->parent->rate / bfc_divisors[idx];
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct clk_ops sh7763_bus_clk_ops = {
	.recalc		= bus_clk_recalc,
};

static struct clk_ops sh7763_cpu_clk_ops = {
	.recalc		= followparent_recalc,
};

static struct clk_ops *sh7763_clk_ops[] = {
=======
=======
>>>>>>> refs/remotes/origin/master
static struct sh_clk_ops sh7763_bus_clk_ops = {
	.recalc		= bus_clk_recalc,
};

static struct sh_clk_ops sh7763_cpu_clk_ops = {
	.recalc		= followparent_recalc,
};

static struct sh_clk_ops *sh7763_clk_ops[] = {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	&sh7763_master_clk_ops,
	&sh7763_module_clk_ops,
	&sh7763_bus_clk_ops,
	&sh7763_cpu_clk_ops,
};

<<<<<<< HEAD
<<<<<<< HEAD
void __init arch_init_clk_ops(struct clk_ops **ops, int idx)
=======
void __init arch_init_clk_ops(struct sh_clk_ops **ops, int idx)
>>>>>>> refs/remotes/origin/cm-10.0
=======
void __init arch_init_clk_ops(struct sh_clk_ops **ops, int idx)
>>>>>>> refs/remotes/origin/master
{
	if (idx < ARRAY_SIZE(sh7763_clk_ops))
		*ops = sh7763_clk_ops[idx];
}

static unsigned long shyway_clk_recalc(struct clk *clk)
{
	int idx = ((__raw_readl(FRQCR) >> 20) & 0x07);
	return clk->parent->rate / cfc_divisors[idx];
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct clk_ops sh7763_shyway_clk_ops = {
=======
static struct sh_clk_ops sh7763_shyway_clk_ops = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct sh_clk_ops sh7763_shyway_clk_ops = {
>>>>>>> refs/remotes/origin/master
	.recalc		= shyway_clk_recalc,
};

static struct clk sh7763_shyway_clk = {
	.flags		= CLK_ENABLE_ON_INIT,
	.ops		= &sh7763_shyway_clk_ops,
};

/*
 * Additional SH7763-specific on-chip clocks that aren't already part of the
 * clock framework
 */
static struct clk *sh7763_onchip_clocks[] = {
	&sh7763_shyway_clk,
};

<<<<<<< HEAD
<<<<<<< HEAD
#define CLKDEV_CON_ID(_id, _clk) { .con_id = _id, .clk = _clk }

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static struct clk_lookup lookups[] = {
	/* main clocks */
	CLKDEV_CON_ID("shyway_clk", &sh7763_shyway_clk),
};

int __init arch_clk_init(void)
{
	struct clk *clk;
	int i, ret = 0;

	cpg_clk_init();

	clk = clk_get(NULL, "master_clk");
	for (i = 0; i < ARRAY_SIZE(sh7763_onchip_clocks); i++) {
		struct clk *clkp = sh7763_onchip_clocks[i];

		clkp->parent = clk;
		ret |= clk_register(clkp);
	}

	clk_put(clk);

	clkdev_add_table(lookups, ARRAY_SIZE(lookups));

	return ret;
}
