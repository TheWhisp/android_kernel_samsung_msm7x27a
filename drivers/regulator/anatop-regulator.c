/*
 * Copyright (C) 2011 Freescale Semiconductor, Inc. All Rights Reserved.
 */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <linux/slab.h>
#include <linux/device.h>
#include <linux/module.h>
<<<<<<< HEAD
=======
#include <linux/mfd/syscon.h>
>>>>>>> refs/remotes/origin/master
#include <linux/err.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_address.h>
<<<<<<< HEAD
#include <linux/mfd/anatop.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/of_regulator.h>

struct anatop_regulator {
	const char *name;
	u32 control_reg;
	struct anatop *mfd;
	int vol_bit_shift;
	int vol_bit_width;
=======
#include <linux/regmap.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/of_regulator.h>

#define LDO_RAMP_UP_UNIT_IN_CYCLES      64 /* 64 cycles per step */
#define LDO_RAMP_UP_FREQ_IN_MHZ         24 /* cycle based on 24M OSC */

struct anatop_regulator {
	const char *name;
	u32 control_reg;
	struct regmap *anatop;
	int vol_bit_shift;
	int vol_bit_width;
	u32 delay_reg;
	int delay_bit_shift;
	int delay_bit_width;
>>>>>>> refs/remotes/origin/master
	int min_bit_val;
	int min_voltage;
	int max_voltage;
	struct regulator_desc rdesc;
	struct regulator_init_data *initdata;
};

<<<<<<< HEAD
static int anatop_set_voltage(struct regulator_dev *reg, int min_uV,
				  int max_uV, unsigned *selector)
{
	struct anatop_regulator *anatop_reg = rdev_get_drvdata(reg);
	u32 val, sel;
	int uv;

	uv = min_uV;
	dev_dbg(&reg->dev, "%s: uv %d, min %d, max %d\n", __func__,
		uv, anatop_reg->min_voltage,
		anatop_reg->max_voltage);

	if (uv < anatop_reg->min_voltage) {
		if (max_uV > anatop_reg->min_voltage)
			uv = anatop_reg->min_voltage;
		else
			return -EINVAL;
	}
=======
static int anatop_regmap_set_voltage_sel(struct regulator_dev *reg,
					unsigned selector)
{
	struct anatop_regulator *anatop_reg = rdev_get_drvdata(reg);
>>>>>>> refs/remotes/origin/master

	if (!anatop_reg->control_reg)
		return -ENOTSUPP;

<<<<<<< HEAD
	sel = DIV_ROUND_UP(uv - anatop_reg->min_voltage, 25000);
	if (sel * 25000 + anatop_reg->min_voltage > anatop_reg->max_voltage)
		return -EINVAL;
	val = anatop_reg->min_bit_val + sel;
	*selector = sel;
	dev_dbg(&reg->dev, "%s: calculated val %d\n", __func__, val);
	anatop_set_bits(anatop_reg->mfd,
			anatop_reg->control_reg,
			anatop_reg->vol_bit_shift,
			anatop_reg->vol_bit_width,
			val);

	return 0;
}

static int anatop_get_voltage_sel(struct regulator_dev *reg)
{
	struct anatop_regulator *anatop_reg = rdev_get_drvdata(reg);
	u32 val;

	if (!anatop_reg->control_reg)
		return -ENOTSUPP;

	val = anatop_get_bits(anatop_reg->mfd,
			      anatop_reg->control_reg,
			      anatop_reg->vol_bit_shift,
			      anatop_reg->vol_bit_width);

	return val - anatop_reg->min_bit_val;
}

static int anatop_list_voltage(struct regulator_dev *reg, unsigned selector)
{
	struct anatop_regulator *anatop_reg = rdev_get_drvdata(reg);
	int uv;

	uv = anatop_reg->min_voltage + selector * 25000;
	dev_dbg(&reg->dev, "vddio = %d, selector = %u\n", uv, selector);

	return uv;
}

static struct regulator_ops anatop_rops = {
	.set_voltage     = anatop_set_voltage,
	.get_voltage_sel = anatop_get_voltage_sel,
	.list_voltage    = anatop_list_voltage,
};

static int __devinit anatop_regulator_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
=======
	return regulator_set_voltage_sel_regmap(reg, selector);
}

static int anatop_regmap_set_voltage_time_sel(struct regulator_dev *reg,
	unsigned int old_sel,
	unsigned int new_sel)
{
	struct anatop_regulator *anatop_reg = rdev_get_drvdata(reg);
	u32 val;
	int ret = 0;

	/* check whether need to care about LDO ramp up speed */
	if (anatop_reg->delay_bit_width && new_sel > old_sel) {
		/*
		 * the delay for LDO ramp up time is
		 * based on the register setting, we need
		 * to calculate how many steps LDO need to
		 * ramp up, and how much delay needed. (us)
		 */
		regmap_read(anatop_reg->anatop, anatop_reg->delay_reg, &val);
		val = (val >> anatop_reg->delay_bit_shift) &
			((1 << anatop_reg->delay_bit_width) - 1);
		ret = (new_sel - old_sel) * (LDO_RAMP_UP_UNIT_IN_CYCLES <<
			val) / LDO_RAMP_UP_FREQ_IN_MHZ + 1;
	}

	return ret;
}

static int anatop_regmap_get_voltage_sel(struct regulator_dev *reg)
{
	struct anatop_regulator *anatop_reg = rdev_get_drvdata(reg);

	if (!anatop_reg->control_reg)
		return -ENOTSUPP;

	return regulator_get_voltage_sel_regmap(reg);
}

static struct regulator_ops anatop_rops = {
	.set_voltage_sel = anatop_regmap_set_voltage_sel,
	.set_voltage_time_sel = anatop_regmap_set_voltage_time_sel,
	.get_voltage_sel = anatop_regmap_get_voltage_sel,
	.list_voltage = regulator_list_voltage_linear,
	.map_voltage = regulator_map_voltage_linear,
};

static int anatop_regulator_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	struct device_node *anatop_np;
>>>>>>> refs/remotes/origin/master
	struct regulator_desc *rdesc;
	struct regulator_dev *rdev;
	struct anatop_regulator *sreg;
	struct regulator_init_data *initdata;
<<<<<<< HEAD
	struct anatop *anatopmfd = dev_get_drvdata(pdev->dev.parent);
=======
	struct regulator_config config = { };
>>>>>>> refs/remotes/origin/master
	int ret = 0;

	initdata = of_get_regulator_init_data(dev, np);
	sreg = devm_kzalloc(dev, sizeof(*sreg), GFP_KERNEL);
	if (!sreg)
		return -ENOMEM;
	sreg->initdata = initdata;
	sreg->name = kstrdup(of_get_property(np, "regulator-name", NULL),
			     GFP_KERNEL);
	rdesc = &sreg->rdesc;
	memset(rdesc, 0, sizeof(*rdesc));
	rdesc->name = sreg->name;
	rdesc->ops = &anatop_rops;
	rdesc->type = REGULATOR_VOLTAGE;
	rdesc->owner = THIS_MODULE;
<<<<<<< HEAD
	sreg->mfd = anatopmfd;
=======

	anatop_np = of_get_parent(np);
	if (!anatop_np)
		return -ENODEV;
	sreg->anatop = syscon_node_to_regmap(anatop_np);
	of_node_put(anatop_np);
	if (IS_ERR(sreg->anatop))
		return PTR_ERR(sreg->anatop);

>>>>>>> refs/remotes/origin/master
	ret = of_property_read_u32(np, "anatop-reg-offset",
				   &sreg->control_reg);
	if (ret) {
		dev_err(dev, "no anatop-reg-offset property set\n");
		goto anatop_probe_end;
	}
	ret = of_property_read_u32(np, "anatop-vol-bit-width",
				   &sreg->vol_bit_width);
	if (ret) {
		dev_err(dev, "no anatop-vol-bit-width property set\n");
		goto anatop_probe_end;
	}
	ret = of_property_read_u32(np, "anatop-vol-bit-shift",
				   &sreg->vol_bit_shift);
	if (ret) {
		dev_err(dev, "no anatop-vol-bit-shift property set\n");
		goto anatop_probe_end;
	}
	ret = of_property_read_u32(np, "anatop-min-bit-val",
				   &sreg->min_bit_val);
	if (ret) {
		dev_err(dev, "no anatop-min-bit-val property set\n");
		goto anatop_probe_end;
	}
	ret = of_property_read_u32(np, "anatop-min-voltage",
				   &sreg->min_voltage);
	if (ret) {
		dev_err(dev, "no anatop-min-voltage property set\n");
		goto anatop_probe_end;
	}
	ret = of_property_read_u32(np, "anatop-max-voltage",
				   &sreg->max_voltage);
	if (ret) {
		dev_err(dev, "no anatop-max-voltage property set\n");
		goto anatop_probe_end;
	}

<<<<<<< HEAD
	rdesc->n_voltages = (sreg->max_voltage - sreg->min_voltage)
		/ 25000 + 1;

	/* register regulator */
	rdev = regulator_register(rdesc, dev,
				  initdata, sreg, pdev->dev.of_node);
=======
	/* read LDO ramp up setting, only for core reg */
	of_property_read_u32(np, "anatop-delay-reg-offset",
			     &sreg->delay_reg);
	of_property_read_u32(np, "anatop-delay-bit-width",
			     &sreg->delay_bit_width);
	of_property_read_u32(np, "anatop-delay-bit-shift",
			     &sreg->delay_bit_shift);

	rdesc->n_voltages = (sreg->max_voltage - sreg->min_voltage) / 25000 + 1
			    + sreg->min_bit_val;
	rdesc->min_uV = sreg->min_voltage;
	rdesc->uV_step = 25000;
	rdesc->linear_min_sel = sreg->min_bit_val;
	rdesc->vsel_reg = sreg->control_reg;
	rdesc->vsel_mask = ((1 << sreg->vol_bit_width) - 1) <<
			   sreg->vol_bit_shift;

	config.dev = &pdev->dev;
	config.init_data = initdata;
	config.driver_data = sreg;
	config.of_node = pdev->dev.of_node;
	config.regmap = sreg->anatop;

	/* register regulator */
	rdev = devm_regulator_register(dev, rdesc, &config);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(rdev)) {
		dev_err(dev, "failed to register %s\n",
			rdesc->name);
		ret = PTR_ERR(rdev);
		goto anatop_probe_end;
	}

	platform_set_drvdata(pdev, rdev);

anatop_probe_end:
	if (ret)
		kfree(sreg->name);

	return ret;
}

<<<<<<< HEAD
static int __devexit anatop_regulator_remove(struct platform_device *pdev)
=======
static int anatop_regulator_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct regulator_dev *rdev = platform_get_drvdata(pdev);
	struct anatop_regulator *sreg = rdev_get_drvdata(rdev);
	const char *name = sreg->name;

<<<<<<< HEAD
	regulator_unregister(rdev);
=======
>>>>>>> refs/remotes/origin/master
	kfree(name);

	return 0;
}

<<<<<<< HEAD
static struct of_device_id __devinitdata of_anatop_regulator_match_tbl[] = {
=======
static struct of_device_id of_anatop_regulator_match_tbl[] = {
>>>>>>> refs/remotes/origin/master
	{ .compatible = "fsl,anatop-regulator", },
	{ /* end */ }
};

static struct platform_driver anatop_regulator_driver = {
	.driver = {
		.name	= "anatop_regulator",
		.owner  = THIS_MODULE,
		.of_match_table = of_anatop_regulator_match_tbl,
	},
	.probe	= anatop_regulator_probe,
	.remove	= anatop_regulator_remove,
};

static int __init anatop_regulator_init(void)
{
	return platform_driver_register(&anatop_regulator_driver);
}
postcore_initcall(anatop_regulator_init);

static void __exit anatop_regulator_exit(void)
{
	platform_driver_unregister(&anatop_regulator_driver);
}
module_exit(anatop_regulator_exit);

<<<<<<< HEAD
MODULE_AUTHOR("Nancy Chen <Nancy.Chen@freescale.com>, "
	      "Ying-Chun Liu (PaulLiu) <paul.liu@linaro.org>");
=======
MODULE_AUTHOR("Nancy Chen <Nancy.Chen@freescale.com>");
MODULE_AUTHOR("Ying-Chun Liu (PaulLiu) <paul.liu@linaro.org>");
>>>>>>> refs/remotes/origin/master
MODULE_DESCRIPTION("ANATOP Regulator driver");
MODULE_LICENSE("GPL v2");
