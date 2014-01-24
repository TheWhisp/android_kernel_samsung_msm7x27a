/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 * Copyright (C) 2010 Thomas Langer <thomas.langer@lantiq.com>
 * Copyright (C) 2010 John Crispin <blogic@openwrt.org>
 */
#include <linux/io.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/clk.h>
<<<<<<< HEAD
=======
#include <linux/clkdev.h>
>>>>>>> refs/remotes/origin/master
#include <linux/err.h>
#include <linux/list.h>

#include <asm/time.h>
#include <asm/irq.h>
#include <asm/div64.h>

#include <lantiq_soc.h>

#include "clk.h"
<<<<<<< HEAD

struct clk {
	const char *name;
	unsigned long rate;
	unsigned long (*get_rate) (void);
};

static struct clk *cpu_clk;
static int cpu_clk_cnt;

/* lantiq socs have 3 static clocks */
static struct clk cpu_clk_generic[] = {
	{
		.name = "cpu",
		.get_rate = ltq_get_cpu_hz,
	}, {
		.name = "fpi",
		.get_rate = ltq_get_fpi_hz,
	}, {
		.name = "io",
		.get_rate = ltq_get_io_region_clock,
	},
};

static struct resource ltq_cgu_resource = {
	.name	= "cgu",
	.start	= LTQ_CGU_BASE_ADDR,
	.end	= LTQ_CGU_BASE_ADDR + LTQ_CGU_SIZE - 1,
	.flags	= IORESOURCE_MEM,
};

/* remapped clock register range */
void __iomem *ltq_cgu_membase;

void clk_init(void)
{
	cpu_clk = cpu_clk_generic;
	cpu_clk_cnt = ARRAY_SIZE(cpu_clk_generic);
}
=======
#include "prom.h"

/* lantiq socs have 3 static clocks */
static struct clk cpu_clk_generic[4];

void clkdev_add_static(unsigned long cpu, unsigned long fpi,
			unsigned long io, unsigned long ppe)
{
	cpu_clk_generic[0].rate = cpu;
	cpu_clk_generic[1].rate = fpi;
	cpu_clk_generic[2].rate = io;
	cpu_clk_generic[3].rate = ppe;
}

struct clk *clk_get_cpu(void)
{
	return &cpu_clk_generic[0];
}

struct clk *clk_get_fpi(void)
{
	return &cpu_clk_generic[1];
}
EXPORT_SYMBOL_GPL(clk_get_fpi);

struct clk *clk_get_io(void)
{
	return &cpu_clk_generic[2];
}

struct clk *clk_get_ppe(void)
{
	return &cpu_clk_generic[3];
}
EXPORT_SYMBOL_GPL(clk_get_ppe);
>>>>>>> refs/remotes/origin/master

static inline int clk_good(struct clk *clk)
{
	return clk && !IS_ERR(clk);
}

unsigned long clk_get_rate(struct clk *clk)
{
	if (unlikely(!clk_good(clk)))
		return 0;

	if (clk->rate != 0)
		return clk->rate;

	if (clk->get_rate != NULL)
		return clk->get_rate();

	return 0;
}
EXPORT_SYMBOL(clk_get_rate);

<<<<<<< HEAD
struct clk *clk_get(struct device *dev, const char *id)
{
	int i;

	for (i = 0; i < cpu_clk_cnt; i++)
		if (!strcmp(id, cpu_clk[i].name))
			return &cpu_clk[i];
	BUG();
	return ERR_PTR(-ENOENT);
}
EXPORT_SYMBOL(clk_get);

void clk_put(struct clk *clk)
{
	/* not used */
}
EXPORT_SYMBOL(clk_put);

<<<<<<< HEAD
=======
int clk_enable(struct clk *clk)
{
	/* not used */
	return 0;
=======
int clk_set_rate(struct clk *clk, unsigned long rate)
{
	if (unlikely(!clk_good(clk)))
		return 0;
	if (clk->rates && *clk->rates) {
		unsigned long *r = clk->rates;

		while (*r && (*r != rate))
			r++;
		if (!*r) {
			pr_err("clk %s.%s: trying to set invalid rate %ld\n",
				clk->cl.dev_id, clk->cl.con_id, rate);
			return -1;
		}
	}
	clk->rate = rate;
	return 0;
}
EXPORT_SYMBOL(clk_set_rate);

int clk_enable(struct clk *clk)
{
	if (unlikely(!clk_good(clk)))
		return -1;

	if (clk->enable)
		return clk->enable(clk);

	return -1;
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(clk_enable);

void clk_disable(struct clk *clk)
{
<<<<<<< HEAD
	/* not used */
}
EXPORT_SYMBOL(clk_disable);

>>>>>>> refs/remotes/origin/cm-10.0
static inline u32 ltq_get_counter_resolution(void)
=======
	if (unlikely(!clk_good(clk)))
		return;

	if (clk->disable)
		clk->disable(clk);
}
EXPORT_SYMBOL(clk_disable);

int clk_activate(struct clk *clk)
{
	if (unlikely(!clk_good(clk)))
		return -1;

	if (clk->activate)
		return clk->activate(clk);

	return -1;
}
EXPORT_SYMBOL(clk_activate);

void clk_deactivate(struct clk *clk)
{
	if (unlikely(!clk_good(clk)))
		return;

	if (clk->deactivate)
		clk->deactivate(clk);
}
EXPORT_SYMBOL(clk_deactivate);

struct clk *of_clk_get_from_provider(struct of_phandle_args *clkspec)
{
	return NULL;
}

static inline u32 get_counter_resolution(void)
>>>>>>> refs/remotes/origin/master
{
	u32 res;

	__asm__ __volatile__(
<<<<<<< HEAD
		".set   push\n"
		".set   mips32r2\n"
		"rdhwr  %0, $3\n"
=======
		".set	push\n"
		".set	mips32r2\n"
		"rdhwr	%0, $3\n"
>>>>>>> refs/remotes/origin/master
		".set pop\n"
		: "=&r" (res)
		: /* no input */
		: "memory");

	return res;
}

void __init plat_time_init(void)
{
	struct clk *clk;

<<<<<<< HEAD
	if (insert_resource(&iomem_resource, &ltq_cgu_resource) < 0)
<<<<<<< HEAD
		panic("Failed to insert cgu memory\n");

	if (request_mem_region(ltq_cgu_resource.start,
			resource_size(&ltq_cgu_resource), "cgu") < 0)
		panic("Failed to request cgu memory\n");
=======
		panic("Failed to insert cgu memory");

	if (request_mem_region(ltq_cgu_resource.start,
			resource_size(&ltq_cgu_resource), "cgu") < 0)
		panic("Failed to request cgu memory");
>>>>>>> refs/remotes/origin/cm-10.0

	ltq_cgu_membase = ioremap_nocache(ltq_cgu_resource.start,
				resource_size(&ltq_cgu_resource));
	if (!ltq_cgu_membase) {
		pr_err("Failed to remap cgu memory\n");
		unreachable();
	}
	clk = clk_get(0, "cpu");
	mips_hpt_frequency = clk_get_rate(clk) / ltq_get_counter_resolution();
	write_c0_compare(read_c0_count());
=======
	ltq_soc_init();

	clk = clk_get_cpu();
	mips_hpt_frequency = clk_get_rate(clk) / get_counter_resolution();
	write_c0_compare(read_c0_count());
	pr_info("CPU Clock: %ldMHz\n", clk_get_rate(clk) / 1000000);
>>>>>>> refs/remotes/origin/master
	clk_put(clk);
}
