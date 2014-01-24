/*
 * CPU idle for DaVinci SoCs
 *
 * Copyright (C) 2009 Texas Instruments Incorporated. http://www.ti.com/
 *
 * Derived from Marvell Kirkwood CPU idle code
 * (arch/arm/mach-kirkwood/cpuidle.c)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/cpuidle.h>
#include <linux/io.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/proc-fns.h>

#include <mach/cpuidle.h>
#include <mach/memory.h>
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/export.h>
#include <asm/proc-fns.h>
#include <asm/cpuidle.h>

#include <mach/cpuidle.h>
#include <mach/ddr2.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

#define DAVINCI_CPUIDLE_MAX_STATES	2

struct davinci_ops {
	void (*enter) (u32 flags);
	void (*exit) (u32 flags);
	u32 flags;
};

<<<<<<< HEAD
=======
/* Actual code that puts the SoC in different idle states */
static int davinci_enter_idle(struct cpuidle_device *dev,
				struct cpuidle_driver *drv,
						int index)
{
	struct cpuidle_state_usage *state_usage = &dev->states_usage[index];
	struct davinci_ops *ops = cpuidle_get_statedata(state_usage);

	if (ops && ops->enter)
		ops->enter(ops->flags);

	index = cpuidle_wrap_enter(dev,	drv, index,
				arm_cpuidle_simple_enter);

	if (ops && ops->exit)
		ops->exit(ops->flags);

	return index;
}

>>>>>>> refs/remotes/origin/cm-10.0
/* fields in davinci_ops.flags */
#define DAVINCI_CPUIDLE_FLAGS_DDR2_PWDN	BIT(0)

static struct cpuidle_driver davinci_idle_driver = {
<<<<<<< HEAD
	.name	= "cpuidle-davinci",
	.owner	= THIS_MODULE,
=======
	.name			= "cpuidle-davinci",
	.owner			= THIS_MODULE,
	.en_core_tk_irqen	= 1,
	.states[0]		= ARM_CPUIDLE_WFI_STATE,
	.states[1]		= {
		.enter			= davinci_enter_idle,
		.exit_latency		= 10,
		.target_residency	= 100000,
		.flags			= CPUIDLE_FLAG_TIME_VALID,
		.name			= "DDR SR",
		.desc			= "WFI and DDR Self Refresh",
	},
	.state_count = DAVINCI_CPUIDLE_MAX_STATES,
>>>>>>> refs/remotes/origin/cm-10.0
};

static DEFINE_PER_CPU(struct cpuidle_device, davinci_cpuidle_device);
static void __iomem *ddr2_reg_base;
=======

#define DAVINCI_CPUIDLE_MAX_STATES	2

static void __iomem *ddr2_reg_base;
static bool ddr2_pdown;
>>>>>>> refs/remotes/origin/master

static void davinci_save_ddr_power(int enter, bool pdown)
{
	u32 val;

	val = __raw_readl(ddr2_reg_base + DDR2_SDRCR_OFFSET);

	if (enter) {
		if (pdown)
			val |= DDR2_SRPD_BIT;
		else
			val &= ~DDR2_SRPD_BIT;
		val |= DDR2_LPMODEN_BIT;
	} else {
		val &= ~(DDR2_SRPD_BIT | DDR2_LPMODEN_BIT);
	}

	__raw_writel(val, ddr2_reg_base + DDR2_SDRCR_OFFSET);
}

<<<<<<< HEAD
static void davinci_c2state_enter(u32 flags)
{
	davinci_save_ddr_power(1, !!(flags & DAVINCI_CPUIDLE_FLAGS_DDR2_PWDN));
}

static void davinci_c2state_exit(u32 flags)
{
	davinci_save_ddr_power(0, !!(flags & DAVINCI_CPUIDLE_FLAGS_DDR2_PWDN));
}

static struct davinci_ops davinci_states[DAVINCI_CPUIDLE_MAX_STATES] = {
	[1] = {
		.enter	= davinci_c2state_enter,
		.exit	= davinci_c2state_exit,
	},
};

<<<<<<< HEAD
/* Actual code that puts the SoC in different idle states */
static int davinci_enter_idle(struct cpuidle_device *dev,
						struct cpuidle_state *state)
{
	struct davinci_ops *ops = cpuidle_get_statedata(state);
	struct timeval before, after;
	int idle_time;

	local_irq_disable();
	do_gettimeofday(&before);

	if (ops && ops->enter)
		ops->enter(ops->flags);
	/* Wait for interrupt state */
	cpu_do_idle();
	if (ops && ops->exit)
		ops->exit(ops->flags);

	do_gettimeofday(&after);
	local_irq_enable();
	idle_time = (after.tv_sec - before.tv_sec) * USEC_PER_SEC +
			(after.tv_usec - before.tv_usec);
	return idle_time;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
static int __init davinci_cpuidle_probe(struct platform_device *pdev)
{
	int ret;
	struct cpuidle_device *device;
	struct davinci_cpuidle_config *pdata = pdev->dev.platform_data;

	device = &per_cpu(davinci_cpuidle_device, smp_processor_id());

=======
/* Actual code that puts the SoC in different idle states */
static int davinci_enter_idle(struct cpuidle_device *dev,
			      struct cpuidle_driver *drv, int index)
{
	davinci_save_ddr_power(1, ddr2_pdown);
	cpu_do_idle();
	davinci_save_ddr_power(0, ddr2_pdown);

	return index;
}

static struct cpuidle_driver davinci_idle_driver = {
	.name			= "cpuidle-davinci",
	.owner			= THIS_MODULE,
	.states[0]		= ARM_CPUIDLE_WFI_STATE,
	.states[1]		= {
		.enter			= davinci_enter_idle,
		.exit_latency		= 10,
		.target_residency	= 10000,
		.flags			= CPUIDLE_FLAG_TIME_VALID,
		.name			= "DDR SR",
		.desc			= "WFI and DDR Self Refresh",
	},
	.state_count = DAVINCI_CPUIDLE_MAX_STATES,
};

static int __init davinci_cpuidle_probe(struct platform_device *pdev)
{
	struct davinci_cpuidle_config *pdata = pdev->dev.platform_data;

>>>>>>> refs/remotes/origin/master
	if (!pdata) {
		dev_err(&pdev->dev, "cannot get platform data\n");
		return -ENOENT;
	}

	ddr2_reg_base = pdata->ddr2_ctlr_base;

<<<<<<< HEAD
<<<<<<< HEAD
=======
	if (pdata->ddr2_pdown)
		davinci_states[1].flags |= DAVINCI_CPUIDLE_FLAGS_DDR2_PWDN;
	cpuidle_set_statedata(&device->states_usage[1], &davinci_states[1]);

	device->state_count = DAVINCI_CPUIDLE_MAX_STATES;

>>>>>>> refs/remotes/origin/cm-10.0
	ret = cpuidle_register_driver(&davinci_idle_driver);
	if (ret) {
		dev_err(&pdev->dev, "failed to register driver\n");
		return ret;
	}

<<<<<<< HEAD
	/* Wait for interrupt state */
	device->states[0].enter = davinci_enter_idle;
	device->states[0].exit_latency = 1;
	device->states[0].target_residency = 10000;
	device->states[0].flags = CPUIDLE_FLAG_TIME_VALID;
	strcpy(device->states[0].name, "WFI");
	strcpy(device->states[0].desc, "Wait for interrupt");

	/* Wait for interrupt and DDR self refresh state */
	device->states[1].enter = davinci_enter_idle;
	device->states[1].exit_latency = 10;
	device->states[1].target_residency = 10000;
	device->states[1].flags = CPUIDLE_FLAG_TIME_VALID;
	strcpy(device->states[1].name, "DDR SR");
	strcpy(device->states[1].desc, "WFI and DDR Self Refresh");
	if (pdata->ddr2_pdown)
		davinci_states[1].flags |= DAVINCI_CPUIDLE_FLAGS_DDR2_PWDN;
	cpuidle_set_statedata(&device->states[1], &davinci_states[1]);

	device->state_count = DAVINCI_CPUIDLE_MAX_STATES;

=======
>>>>>>> refs/remotes/origin/cm-10.0
	ret = cpuidle_register_device(device);
	if (ret) {
		dev_err(&pdev->dev, "failed to register device\n");
		cpuidle_unregister_driver(&davinci_idle_driver);
		return ret;
	}

	return 0;
=======
	ddr2_pdown = pdata->ddr2_pdown;

	return cpuidle_register(&davinci_idle_driver, NULL);
>>>>>>> refs/remotes/origin/master
}

static struct platform_driver davinci_cpuidle_driver = {
	.driver = {
		.name	= "cpuidle-davinci",
		.owner	= THIS_MODULE,
	},
};

static int __init davinci_cpuidle_init(void)
{
	return platform_driver_probe(&davinci_cpuidle_driver,
						davinci_cpuidle_probe);
}
device_initcall(davinci_cpuidle_init);

