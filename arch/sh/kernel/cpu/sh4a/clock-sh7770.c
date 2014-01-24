/*
 * arch/sh/kernel/cpu/sh4a/clock-sh7770.c
 *
 * SH7770 support for the clock framework
 *
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

static int ifc_divisors[] = { 1, 1, 1, 1, 1, 1, 1, 1 };
static int bfc_divisors[] = { 1, 1, 1, 1, 1, 8,12, 1 };
static int pfc_divisors[] = { 1, 8, 1,10,12,16, 1, 1 };

static void master_clk_init(struct clk *clk)
{
	clk->rate *= pfc_divisors[(__raw_readl(FRQCR) >> 28) & 0x000f];
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct clk_ops sh7770_master_clk_ops = {
=======
static struct sh_clk_ops sh7770_master_clk_ops = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct sh_clk_ops sh7770_master_clk_ops = {
>>>>>>> refs/remotes/origin/master
	.init		= master_clk_init,
};

static unsigned long module_clk_recalc(struct clk *clk)
{
	int idx = ((__raw_readl(FRQCR) >> 28) & 0x000f);
	return clk->parent->rate / pfc_divisors[idx];
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct clk_ops sh7770_module_clk_ops = {
=======
static struct sh_clk_ops sh7770_module_clk_ops = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct sh_clk_ops sh7770_module_clk_ops = {
>>>>>>> refs/remotes/origin/master
	.recalc		= module_clk_recalc,
};

static unsigned long bus_clk_recalc(struct clk *clk)
{
	int idx = (__raw_readl(FRQCR) & 0x000f);
	return clk->parent->rate / bfc_divisors[idx];
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct clk_ops sh7770_bus_clk_ops = {
=======
static struct sh_clk_ops sh7770_bus_clk_ops = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct sh_clk_ops sh7770_bus_clk_ops = {
>>>>>>> refs/remotes/origin/master
	.recalc		= bus_clk_recalc,
};

static unsigned long cpu_clk_recalc(struct clk *clk)
{
	int idx = ((__raw_readl(FRQCR) >> 24) & 0x000f);
	return clk->parent->rate / ifc_divisors[idx];
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct clk_ops sh7770_cpu_clk_ops = {
	.recalc		= cpu_clk_recalc,
};

static struct clk_ops *sh7770_clk_ops[] = {
=======
=======
>>>>>>> refs/remotes/origin/master
static struct sh_clk_ops sh7770_cpu_clk_ops = {
	.recalc		= cpu_clk_recalc,
};

static struct sh_clk_ops *sh7770_clk_ops[] = {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	&sh7770_master_clk_ops,
	&sh7770_module_clk_ops,
	&sh7770_bus_clk_ops,
	&sh7770_cpu_clk_ops,
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
	if (idx < ARRAY_SIZE(sh7770_clk_ops))
		*ops = sh7770_clk_ops[idx];
}

