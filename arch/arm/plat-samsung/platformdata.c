/* linux/arch/arm/plat-samsung/platformdata.c
 *
 * Copyright 2010 Ben Dooks <ben-linux <at> fluff.org>
 *
 * Helper for platform data setting
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/kernel.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/slab.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/slab.h>
>>>>>>> refs/remotes/origin/master
#include <linux/string.h>
#include <linux/platform_device.h>

#include <plat/devs.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <plat/sdhci.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <plat/sdhci.h>
>>>>>>> refs/remotes/origin/master

void __init *s3c_set_platdata(void *pd, size_t pdsize,
			      struct platform_device *pdev)
{
	void *npd;

	if (!pd) {
		/* too early to use dev_name(), may not be registered */
		printk(KERN_ERR "%s: no platform data supplied\n", pdev->name);
		return NULL;
	}

	npd = kmemdup(pd, pdsize, GFP_KERNEL);
	if (!npd) {
		printk(KERN_ERR "%s: cannot clone platform data\n", pdev->name);
		return NULL;
	}

	pdev->dev.platform_data = npd;
	return npd;
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master

void s3c_sdhci_set_platdata(struct s3c_sdhci_platdata *pd,
			     struct s3c_sdhci_platdata *set)
{
	set->cd_type = pd->cd_type;
	set->ext_cd_init = pd->ext_cd_init;
	set->ext_cd_cleanup = pd->ext_cd_cleanup;
	set->ext_cd_gpio = pd->ext_cd_gpio;
	set->ext_cd_gpio_invert = pd->ext_cd_gpio_invert;

	if (pd->max_width)
		set->max_width = pd->max_width;
	if (pd->cfg_gpio)
		set->cfg_gpio = pd->cfg_gpio;
	if (pd->host_caps)
		set->host_caps |= pd->host_caps;
	if (pd->host_caps2)
		set->host_caps2 |= pd->host_caps2;
	if (pd->pm_caps)
		set->pm_caps |= pd->pm_caps;
<<<<<<< HEAD
	if (pd->clk_type)
		set->clk_type = pd->clk_type;
}
>>>>>>> refs/remotes/origin/cm-10.0
=======
}
>>>>>>> refs/remotes/origin/master
