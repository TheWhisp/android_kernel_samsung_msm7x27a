/*
 * OMAP3/OMAP4 smartreflex device file
 *
 * Author: Thara Gopinath	<thara@ti.com>
 *
 * Based originally on code from smartreflex.c
 * Copyright (C) 2010 Texas Instruments, Inc.
 * Thara Gopinath <thara@ti.com>
 *
 * Copyright (C) 2008 Nokia Corporation
 * Kalle Jokiniemi
 *
 * Copyright (C) 2007 Texas Instruments, Inc.
 * Lesly A M <x0080970@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
<<<<<<< HEAD
=======
#include <linux/power/smartreflex.h>
>>>>>>> refs/remotes/origin/master

#include <linux/err.h>
#include <linux/slab.h>
#include <linux/io.h>

<<<<<<< HEAD
#include <plat/omap_device.h>

#include "smartreflex.h"
=======
#include "soc.h"
#include "omap_device.h"
>>>>>>> refs/remotes/origin/master
#include "voltage.h"
#include "control.h"
#include "pm.h"

static bool sr_enable_on_init;

<<<<<<< HEAD
<<<<<<< HEAD
static struct omap_device_pm_latency omap_sr_latency[] = {
	{
		.deactivate_func = omap_device_idle_hwmods,
		.activate_func	 = omap_device_enable_hwmods,
		.flags = OMAP_DEVICE_LATENCY_AUTO_ADJUST
	},
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/* Read EFUSE values from control registers for OMAP3430 */
static void __init sr_set_nvalues(struct omap_volt_data *volt_data,
				struct omap_sr_data *sr_data)
{
	struct omap_sr_nvalue_table *nvalue_table;
<<<<<<< HEAD
	int i, count = 0;
=======
	int i, j, count = 0;

	sr_data->nvalue_count = 0;
	sr_data->nvalue_table = NULL;
>>>>>>> refs/remotes/origin/master

	while (volt_data[count].volt_nominal)
		count++;

	nvalue_table = kzalloc(sizeof(struct omap_sr_nvalue_table)*count,
			GFP_KERNEL);

<<<<<<< HEAD
	for (i = 0; i < count; i++) {
		u32 v;
=======
	if (!nvalue_table) {
		pr_err("OMAP: SmartReflex: cannot allocate memory for n-value table\n");
		return;
	}

	for (i = 0, j = 0; i < count; i++) {
		u32 v;

>>>>>>> refs/remotes/origin/master
		/*
		 * In OMAP4 the efuse registers are 24 bit aligned.
		 * A __raw_readl will fail for non-32 bit aligned address
		 * and hence the 8-bit read and shift.
		 */
		if (cpu_is_omap44xx()) {
			u16 offset = volt_data[i].sr_efuse_offs;

			v = omap_ctrl_readb(offset) |
				omap_ctrl_readb(offset + 1) << 8 |
				omap_ctrl_readb(offset + 2) << 16;
		} else {
<<<<<<< HEAD
			 v = omap_ctrl_readl(volt_data[i].sr_efuse_offs);
		}

		nvalue_table[i].efuse_offs = volt_data[i].sr_efuse_offs;
		nvalue_table[i].nvalue = v;
	}

	sr_data->nvalue_table = nvalue_table;
	sr_data->nvalue_count = count;
}

<<<<<<< HEAD
static int sr_dev_init(struct omap_hwmod *oh, void *user)
{
	struct omap_sr_data *sr_data;
	struct omap_device *od;
	struct omap_volt_data *volt_data;
=======
=======
			v = omap_ctrl_readl(volt_data[i].sr_efuse_offs);
		}

		/*
		 * Many OMAP SoCs don't have the eFuse values set.
		 * For example, pretty much all OMAP3xxx before
		 * ES3.something.
		 *
		 * XXX There needs to be some way for board files or
		 * userspace to add these in.
		 */
		if (v == 0)
			continue;

		nvalue_table[j].nvalue = v;
		nvalue_table[j].efuse_offs = volt_data[i].sr_efuse_offs;
		nvalue_table[j].errminlimit = volt_data[i].sr_errminlimit;
		nvalue_table[j].volt_nominal = volt_data[i].volt_nominal;

		j++;
	}

	sr_data->nvalue_table = nvalue_table;
	sr_data->nvalue_count = j;
}

>>>>>>> refs/remotes/origin/master
static int __init sr_dev_init(struct omap_hwmod *oh, void *user)
{
	struct omap_sr_data *sr_data;
	struct platform_device *pdev;
	struct omap_volt_data *volt_data;
	struct omap_smartreflex_dev_attr *sr_dev_attr;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	char *name = "smartreflex";
	static int i;

	sr_data = kzalloc(sizeof(struct omap_sr_data), GFP_KERNEL);
	if (!sr_data) {
<<<<<<< HEAD
		pr_err("%s: Unable to allocate memory for %s sr_data.Error!\n",
			__func__, oh->name);
		return -ENOMEM;
	}

<<<<<<< HEAD
	if (!oh->vdd_name) {
		pr_err("%s: No voltage domain specified for %s."
			"Cannot initialize\n", __func__, oh->name);
=======
	sr_dev_attr = (struct omap_smartreflex_dev_attr *)oh->dev_attr;
	if (!sr_dev_attr || !sr_dev_attr->sensor_voltdm_name) {
		pr_err("%s: No voltage domain specified for %s."
				"Cannot initialize\n", __func__,
					oh->name);
>>>>>>> refs/remotes/origin/cm-10.0
		goto exit;
	}

=======
		pr_err("%s: Unable to allocate memory for %s sr_data\n",
		       __func__, oh->name);
		return -ENOMEM;
	}

	sr_dev_attr = (struct omap_smartreflex_dev_attr *)oh->dev_attr;
	if (!sr_dev_attr || !sr_dev_attr->sensor_voltdm_name) {
		pr_err("%s: No voltage domain specified for %s. Cannot initialize\n",
		       __func__, oh->name);
		goto exit;
	}

	sr_data->name = oh->name;
>>>>>>> refs/remotes/origin/master
	sr_data->ip_type = oh->class->rev;
	sr_data->senn_mod = 0x1;
	sr_data->senp_mod = 0x1;

<<<<<<< HEAD
<<<<<<< HEAD
	sr_data->voltdm = omap_voltage_domain_lookup(oh->vdd_name);
	if (IS_ERR(sr_data->voltdm)) {
		pr_err("%s: Unable to get voltage domain pointer for VDD %s\n",
			__func__, oh->vdd_name);
=======
	sr_data->voltdm = voltdm_lookup(sr_dev_attr->sensor_voltdm_name);
	if (IS_ERR(sr_data->voltdm)) {
		pr_err("%s: Unable to get voltage domain pointer for VDD %s\n",
			__func__, sr_dev_attr->sensor_voltdm_name);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (cpu_is_omap34xx() || cpu_is_omap44xx()) {
		sr_data->err_weight = OMAP3430_SR_ERRWEIGHT;
		sr_data->err_maxlimit = OMAP3430_SR_ERRMAXLIMIT;
		sr_data->accum_data = OMAP3430_SR_ACCUMDATA;
		if (!(strcmp(sr_data->name, "smartreflex_mpu"))) {
			sr_data->senn_avgweight = OMAP3430_SR1_SENNAVGWEIGHT;
			sr_data->senp_avgweight = OMAP3430_SR1_SENPAVGWEIGHT;
		} else {
			sr_data->senn_avgweight = OMAP3430_SR2_SENNAVGWEIGHT;
			sr_data->senp_avgweight = OMAP3430_SR2_SENPAVGWEIGHT;
		}
	}

	sr_data->voltdm = voltdm_lookup(sr_dev_attr->sensor_voltdm_name);
	if (!sr_data->voltdm) {
		pr_err("%s: Unable to get voltage domain pointer for VDD %s\n",
			__func__, sr_dev_attr->sensor_voltdm_name);
>>>>>>> refs/remotes/origin/master
		goto exit;
	}

	omap_voltage_get_volttable(sr_data->voltdm, &volt_data);
	if (!volt_data) {
<<<<<<< HEAD
		pr_warning("%s: No Voltage table registerd fo VDD%d."
			"Something really wrong\n\n", __func__, i + 1);
=======
		pr_err("%s: No Voltage table registered for VDD%d\n",
		       __func__, i + 1);
>>>>>>> refs/remotes/origin/master
		goto exit;
	}

	sr_set_nvalues(volt_data, sr_data);

	sr_data->enable_on_init = sr_enable_on_init;

<<<<<<< HEAD
<<<<<<< HEAD
	od = omap_device_build(name, i, oh, sr_data, sizeof(*sr_data),
			       omap_sr_latency,
			       ARRAY_SIZE(omap_sr_latency), 0);
	if (IS_ERR(od))
=======
	pdev = omap_device_build(name, i, oh, sr_data, sizeof(*sr_data),
				 NULL, 0, 0);
	if (IS_ERR(pdev))
>>>>>>> refs/remotes/origin/cm-10.0
=======
	pdev = omap_device_build(name, i, oh, sr_data, sizeof(*sr_data));
	if (IS_ERR(pdev))
>>>>>>> refs/remotes/origin/master
		pr_warning("%s: Could not build omap_device for %s: %s.\n\n",
			__func__, name, oh->name);
exit:
	i++;
	kfree(sr_data);
	return 0;
}

/*
 * API to be called from board files to enable smartreflex
 * autocompensation at init.
 */
void __init omap_enable_smartreflex_on_init(void)
{
	sr_enable_on_init = true;
}

int __init omap_devinit_smartreflex(void)
{
	return omap_hwmod_for_each_by_class("smartreflex", sr_dev_init, NULL);
}
