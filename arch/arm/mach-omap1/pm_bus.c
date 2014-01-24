/*
 * Runtime PM support code for OMAP1
 *
 * Author: Kevin Hilman, Deep Root Systems, LLC
 *
 * Copyright (C) 2010 Texas Instruments, Inc.
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/pm_runtime.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/pm_clock.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/pm_clock.h>
>>>>>>> refs/remotes/origin/master
#include <linux/platform_device.h>
#include <linux/mutex.h>
#include <linux/clk.h>
#include <linux/err.h>

<<<<<<< HEAD
#include <plat/omap_device.h>
#include <plat/omap-pm.h>
=======
#include "soc.h"
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_PM_RUNTIME
static int omap1_pm_runtime_suspend(struct device *dev)
{
	int ret;

	dev_dbg(dev, "%s\n", __func__);

	ret = pm_generic_runtime_suspend(dev);
	if (ret)
		return ret;

<<<<<<< HEAD
<<<<<<< HEAD
	ret = pm_runtime_clk_suspend(dev);
=======
	ret = pm_clk_suspend(dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = pm_clk_suspend(dev);
>>>>>>> refs/remotes/origin/master
	if (ret) {
		pm_generic_runtime_resume(dev);
		return ret;
	}

	return 0;
}

static int omap1_pm_runtime_resume(struct device *dev)
{
	dev_dbg(dev, "%s\n", __func__);

<<<<<<< HEAD
<<<<<<< HEAD
	pm_runtime_clk_resume(dev);
	return pm_generic_runtime_resume(dev);
}

static struct dev_power_domain default_power_domain = {
=======
=======
>>>>>>> refs/remotes/origin/master
	pm_clk_resume(dev);
	return pm_generic_runtime_resume(dev);
}

static struct dev_pm_domain default_pm_domain = {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	.ops = {
		.runtime_suspend = omap1_pm_runtime_suspend,
		.runtime_resume = omap1_pm_runtime_resume,
		USE_PLATFORM_PM_SLEEP_OPS
	},
};
<<<<<<< HEAD
<<<<<<< HEAD
#define OMAP1_PWR_DOMAIN (&default_power_domain)
#else
#define OMAP1_PWR_DOMAIN NULL
#endif /* CONFIG_PM_RUNTIME */

static struct pm_clk_notifier_block platform_bus_notifier = {
	.pwr_domain = OMAP1_PWR_DOMAIN,
=======
=======
>>>>>>> refs/remotes/origin/master
#define OMAP1_PM_DOMAIN (&default_pm_domain)
#else
#define OMAP1_PM_DOMAIN NULL
#endif /* CONFIG_PM_RUNTIME */

static struct pm_clk_notifier_block platform_bus_notifier = {
	.pm_domain = OMAP1_PM_DOMAIN,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	.con_ids = { "ick", "fck", NULL, },
};

static int __init omap1_pm_runtime_init(void)
{
	if (!cpu_class_is_omap1())
		return -ENODEV;

<<<<<<< HEAD
<<<<<<< HEAD
	pm_runtime_clk_add_notifier(&platform_bus_type, &platform_bus_notifier);
=======
	pm_clk_add_notifier(&platform_bus_type, &platform_bus_notifier);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pm_clk_add_notifier(&platform_bus_type, &platform_bus_notifier);
>>>>>>> refs/remotes/origin/master

	return 0;
}
core_initcall(omap1_pm_runtime_init);

