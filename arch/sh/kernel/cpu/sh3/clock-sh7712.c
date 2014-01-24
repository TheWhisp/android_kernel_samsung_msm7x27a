/*
 * arch/sh/kernel/cpu/sh3/clock-sh7712.c
 *
 * SH7712 support for the clock framework
 *
 *  Copyright (C) 2007  Andrew Murray <amurray@mpc-data.co.uk>
 *
 * Based on arch/sh/kernel/cpu/sh3/clock-sh3.c
 *  Copyright (C) 2005  Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <asm/clock.h>
#include <asm/freq.h>
#include <asm/io.h>

static int multipliers[] = { 1, 2, 3 };
static int divisors[]    = { 1, 2, 3, 4, 6 };

static void master_clk_init(struct clk *clk)
{
	int frqcr = __raw_readw(FRQCR);
	int idx = (frqcr & 0x0300) >> 8;

	clk->rate *= multipliers[idx];
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct clk_ops sh7712_master_clk_ops = {
=======
static struct sh_clk_ops sh7712_master_clk_ops = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct sh_clk_ops sh7712_master_clk_ops = {
>>>>>>> refs/remotes/origin/master
	.init		= master_clk_init,
};

static unsigned long module_clk_recalc(struct clk *clk)
{
	int frqcr = __raw_readw(FRQCR);
	int idx = frqcr & 0x0007;

	return clk->parent->rate / divisors[idx];
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct clk_ops sh7712_module_clk_ops = {
=======
static struct sh_clk_ops sh7712_module_clk_ops = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct sh_clk_ops sh7712_module_clk_ops = {
>>>>>>> refs/remotes/origin/master
	.recalc		= module_clk_recalc,
};

static unsigned long cpu_clk_recalc(struct clk *clk)
{
	int frqcr = __raw_readw(FRQCR);
	int idx = (frqcr & 0x0030) >> 4;

	return clk->parent->rate / divisors[idx];
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct clk_ops sh7712_cpu_clk_ops = {
	.recalc		= cpu_clk_recalc,
};

static struct clk_ops *sh7712_clk_ops[] = {
=======
=======
>>>>>>> refs/remotes/origin/master
static struct sh_clk_ops sh7712_cpu_clk_ops = {
	.recalc		= cpu_clk_recalc,
};

static struct sh_clk_ops *sh7712_clk_ops[] = {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	&sh7712_master_clk_ops,
	&sh7712_module_clk_ops,
	&sh7712_cpu_clk_ops,
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
	if (idx < ARRAY_SIZE(sh7712_clk_ops))
		*ops = sh7712_clk_ops[idx];
}

