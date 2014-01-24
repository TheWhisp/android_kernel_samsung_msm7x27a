/*
 * wm8994-regulator.c  --  Regulator driver for the WM8994
 *
 * Copyright 2009 Wolfson Microelectronics PLC.
 *
 * Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/bitops.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
<<<<<<< HEAD
=======
#include <linux/regulator/machine.h>
>>>>>>> refs/remotes/origin/master
#include <linux/gpio.h>
#include <linux/slab.h>

#include <linux/mfd/wm8994/core.h>
#include <linux/mfd/wm8994/registers.h>
#include <linux/mfd/wm8994/pdata.h>

struct wm8994_ldo {
<<<<<<< HEAD
	int enable;
	bool is_enabled;
	struct regulator_dev *regulator;
	struct wm8994 *wm8994;
=======
	struct regulator_dev *regulator;
	struct wm8994 *wm8994;
	struct regulator_consumer_supply supply;
	struct regulator_init_data init_data;
>>>>>>> refs/remotes/origin/master
};

#define WM8994_LDO1_MAX_SELECTOR 0x7
#define WM8994_LDO2_MAX_SELECTOR 0x3

<<<<<<< HEAD
static int wm8994_ldo_enable(struct regulator_dev *rdev)
{
	struct wm8994_ldo *ldo = rdev_get_drvdata(rdev);

	/* If we have no soft control assume that the LDO is always enabled. */
	if (!ldo->enable)
		return 0;

<<<<<<< HEAD
	gpio_set_value(ldo->enable, 1);
=======
	gpio_set_value_cansleep(ldo->enable, 1);
>>>>>>> refs/remotes/origin/cm-10.0
	ldo->is_enabled = true;

	return 0;
}

static int wm8994_ldo_disable(struct regulator_dev *rdev)
{
	struct wm8994_ldo *ldo = rdev_get_drvdata(rdev);

	/* If we have no soft control assume that the LDO is always enabled. */
	if (!ldo->enable)
		return -EINVAL;

<<<<<<< HEAD
	gpio_set_value(ldo->enable, 0);
=======
	gpio_set_value_cansleep(ldo->enable, 0);
>>>>>>> refs/remotes/origin/cm-10.0
	ldo->is_enabled = false;

	return 0;
}

static int wm8994_ldo_is_enabled(struct regulator_dev *rdev)
{
	struct wm8994_ldo *ldo = rdev_get_drvdata(rdev);

	return ldo->is_enabled;
}

static int wm8994_ldo_enable_time(struct regulator_dev *rdev)
{
	/* 3ms is fairly conservative but this shouldn't be too performance
	 * critical; can be tweaked per-system if required. */
	return 3000;
}

static int wm8994_ldo1_list_voltage(struct regulator_dev *rdev,
				    unsigned int selector)
{
	if (selector > WM8994_LDO1_MAX_SELECTOR)
		return -EINVAL;

	return (selector * 100000) + 2400000;
}

static int wm8994_ldo1_get_voltage_sel(struct regulator_dev *rdev)
{
	struct wm8994_ldo *ldo = rdev_get_drvdata(rdev);
	int val;

	val = wm8994_reg_read(ldo->wm8994, WM8994_LDO_1);
	if (val < 0)
		return val;

	return (val & WM8994_LDO1_VSEL_MASK) >> WM8994_LDO1_VSEL_SHIFT;
}

static int wm8994_ldo1_set_voltage(struct regulator_dev *rdev,
				   int min_uV, int max_uV, unsigned *s)
{
	struct wm8994_ldo *ldo = rdev_get_drvdata(rdev);
	int selector, v;

	selector = (min_uV - 2400000) / 100000;
	v = wm8994_ldo1_list_voltage(rdev, selector);
	if (v < 0 || v > max_uV)
		return -EINVAL;

	*s = selector;
	selector <<= WM8994_LDO1_VSEL_SHIFT;

	return wm8994_set_bits(ldo->wm8994, WM8994_LDO_1,
			       WM8994_LDO1_VSEL_MASK, selector);
}

static struct regulator_ops wm8994_ldo1_ops = {
	.enable = wm8994_ldo_enable,
	.disable = wm8994_ldo_disable,
	.is_enabled = wm8994_ldo_is_enabled,
	.enable_time = wm8994_ldo_enable_time,

	.list_voltage = wm8994_ldo1_list_voltage,
	.get_voltage_sel = wm8994_ldo1_get_voltage_sel,
	.set_voltage = wm8994_ldo1_set_voltage,
=======
static struct regulator_ops wm8994_ldo1_ops = {
	.list_voltage = regulator_list_voltage_linear,
	.map_voltage = regulator_map_voltage_linear,
	.get_voltage_sel = regulator_get_voltage_sel_regmap,
	.set_voltage_sel = regulator_set_voltage_sel_regmap,
>>>>>>> refs/remotes/origin/master
};

static int wm8994_ldo2_list_voltage(struct regulator_dev *rdev,
				    unsigned int selector)
{
	struct wm8994_ldo *ldo = rdev_get_drvdata(rdev);

	if (selector > WM8994_LDO2_MAX_SELECTOR)
		return -EINVAL;

	switch (ldo->wm8994->type) {
	case WM8994:
		return (selector * 100000) + 900000;
	case WM8958:
		return (selector * 100000) + 1000000;
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	case WM1811:
		switch (selector) {
		case 0:
			return -EINVAL;
		default:
			return (selector * 100000) + 950000;
		}
		break;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	default:
		return -EINVAL;
	}
}

<<<<<<< HEAD
static int wm8994_ldo2_get_voltage_sel(struct regulator_dev *rdev)
{
	struct wm8994_ldo *ldo = rdev_get_drvdata(rdev);
	int val;

	val = wm8994_reg_read(ldo->wm8994, WM8994_LDO_2);
	if (val < 0)
		return val;

	return (val & WM8994_LDO2_VSEL_MASK) >> WM8994_LDO2_VSEL_SHIFT;
}

static int wm8994_ldo2_set_voltage(struct regulator_dev *rdev,
				   int min_uV, int max_uV, unsigned *s)
{
	struct wm8994_ldo *ldo = rdev_get_drvdata(rdev);
	int selector, v;

	switch (ldo->wm8994->type) {
	case WM8994:
		selector = (min_uV - 900000) / 100000;
		break;
	case WM8958:
		selector = (min_uV - 1000000) / 100000;
		break;
<<<<<<< HEAD
=======
	case WM1811:
		selector = (min_uV - 950000) / 100000;
		if (selector == 0)
			selector = 1;
		break;
>>>>>>> refs/remotes/origin/cm-10.0
	default:
		return -EINVAL;
	}

	v = wm8994_ldo2_list_voltage(rdev, selector);
	if (v < 0 || v > max_uV)
		return -EINVAL;

	*s = selector;
	selector <<= WM8994_LDO2_VSEL_SHIFT;

	return wm8994_set_bits(ldo->wm8994, WM8994_LDO_2,
			       WM8994_LDO2_VSEL_MASK, selector);
}

static struct regulator_ops wm8994_ldo2_ops = {
	.enable = wm8994_ldo_enable,
	.disable = wm8994_ldo_disable,
	.is_enabled = wm8994_ldo_is_enabled,
	.enable_time = wm8994_ldo_enable_time,

	.list_voltage = wm8994_ldo2_list_voltage,
	.get_voltage_sel = wm8994_ldo2_get_voltage_sel,
	.set_voltage = wm8994_ldo2_set_voltage,
};

static struct regulator_desc wm8994_ldo_desc[] = {
=======
static struct regulator_ops wm8994_ldo2_ops = {
	.list_voltage = wm8994_ldo2_list_voltage,
	.get_voltage_sel = regulator_get_voltage_sel_regmap,
	.set_voltage_sel = regulator_set_voltage_sel_regmap,
};

static const struct regulator_desc wm8994_ldo_desc[] = {
>>>>>>> refs/remotes/origin/master
	{
		.name = "LDO1",
		.id = 1,
		.type = REGULATOR_VOLTAGE,
		.n_voltages = WM8994_LDO1_MAX_SELECTOR + 1,
<<<<<<< HEAD
		.ops = &wm8994_ldo1_ops,
=======
		.vsel_reg = WM8994_LDO_1,
		.vsel_mask = WM8994_LDO1_VSEL_MASK,
		.ops = &wm8994_ldo1_ops,
		.min_uV = 2400000,
		.uV_step = 100000,
		.enable_time = 3000,
>>>>>>> refs/remotes/origin/master
		.owner = THIS_MODULE,
	},
	{
		.name = "LDO2",
		.id = 2,
		.type = REGULATOR_VOLTAGE,
		.n_voltages = WM8994_LDO2_MAX_SELECTOR + 1,
<<<<<<< HEAD
		.ops = &wm8994_ldo2_ops,
=======
		.vsel_reg = WM8994_LDO_2,
		.vsel_mask = WM8994_LDO2_VSEL_MASK,
		.ops = &wm8994_ldo2_ops,
		.enable_time = 3000,
>>>>>>> refs/remotes/origin/master
		.owner = THIS_MODULE,
	},
};

<<<<<<< HEAD
static __devinit int wm8994_ldo_probe(struct platform_device *pdev)
{
	struct wm8994 *wm8994 = dev_get_drvdata(pdev->dev.parent);
	struct wm8994_pdata *pdata = wm8994->dev->platform_data;
	int id = pdev->id % ARRAY_SIZE(pdata->ldo);
=======
static const struct regulator_consumer_supply wm8994_ldo_consumer[] = {
	{ .supply = "AVDD1" },
	{ .supply = "DCVDD" },
};

static const struct regulator_init_data wm8994_ldo_default[] = {
	{
		.constraints = {
			.valid_ops_mask = REGULATOR_CHANGE_STATUS,
		},
		.num_consumer_supplies = 1,
	},
	{
		.constraints = {
			.valid_ops_mask = REGULATOR_CHANGE_STATUS,
		},
		.num_consumer_supplies = 1,
	},
};

static int wm8994_ldo_probe(struct platform_device *pdev)
{
	struct wm8994 *wm8994 = dev_get_drvdata(pdev->dev.parent);
	struct wm8994_pdata *pdata = dev_get_platdata(wm8994->dev);
	int id = pdev->id % ARRAY_SIZE(pdata->ldo);
	struct regulator_config config = { };
>>>>>>> refs/remotes/origin/master
	struct wm8994_ldo *ldo;
	int ret;

	dev_dbg(&pdev->dev, "Probing LDO%d\n", id + 1);

<<<<<<< HEAD
	if (!pdata)
		return -ENODEV;

<<<<<<< HEAD
	ldo = kzalloc(sizeof(struct wm8994_ldo), GFP_KERNEL);
=======
	ldo = devm_kzalloc(&pdev->dev, sizeof(struct wm8994_ldo), GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ldo = devm_kzalloc(&pdev->dev, sizeof(struct wm8994_ldo), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (ldo == NULL) {
		dev_err(&pdev->dev, "Unable to allocate private data\n");
		return -ENOMEM;
	}

	ldo->wm8994 = wm8994;
<<<<<<< HEAD

	if (pdata->ldo[id].enable && gpio_is_valid(pdata->ldo[id].enable)) {
		ldo->enable = pdata->ldo[id].enable;

		ret = gpio_request(ldo->enable, "WM8994 LDO enable");
		if (ret < 0) {
			dev_err(&pdev->dev, "Failed to get enable GPIO: %d\n",
				ret);
			goto err;
		}

		ret = gpio_direction_output(ldo->enable, ldo->is_enabled);
		if (ret < 0) {
			dev_err(&pdev->dev, "Failed to set GPIO up: %d\n",
				ret);
			goto err_gpio;
		}
	} else
		ldo->is_enabled = true;

	ldo->regulator = regulator_register(&wm8994_ldo_desc[id], &pdev->dev,
<<<<<<< HEAD
					     pdata->ldo[id].init_data, ldo);
=======
					     pdata->ldo[id].init_data, ldo, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ldo->supply = wm8994_ldo_consumer[id];
	ldo->supply.dev_name = dev_name(wm8994->dev);

	config.dev = wm8994->dev;
	config.driver_data = ldo;
	config.regmap = wm8994->regmap;
	config.init_data = &ldo->init_data;
	if (pdata)
		config.ena_gpio = pdata->ldo[id].enable;
	else if (wm8994->dev->of_node)
		config.ena_gpio = wm8994->pdata.ldo[id].enable;

	/* Use default constraints if none set up */
	if (!pdata || !pdata->ldo[id].init_data || wm8994->dev->of_node) {
		dev_dbg(wm8994->dev, "Using default init data, supply %s %s\n",
			ldo->supply.dev_name, ldo->supply.supply);

		ldo->init_data = wm8994_ldo_default[id];
		ldo->init_data.consumer_supplies = &ldo->supply;
		if (!config.ena_gpio)
			ldo->init_data.constraints.valid_ops_mask = 0;
	} else {
		ldo->init_data = *pdata->ldo[id].init_data;
	}

	ldo->regulator = devm_regulator_register(&pdev->dev,
						 &wm8994_ldo_desc[id],
						 &config);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(ldo->regulator)) {
		ret = PTR_ERR(ldo->regulator);
		dev_err(wm8994->dev, "Failed to register LDO%d: %d\n",
			id + 1, ret);
<<<<<<< HEAD
		goto err_gpio;
=======
		goto err;
>>>>>>> refs/remotes/origin/master
	}

	platform_set_drvdata(pdev, ldo);

	return 0;

<<<<<<< HEAD
err_gpio:
	if (gpio_is_valid(ldo->enable))
		gpio_free(ldo->enable);
err:
<<<<<<< HEAD
	kfree(ldo);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
}

static __devexit int wm8994_ldo_remove(struct platform_device *pdev)
{
	struct wm8994_ldo *ldo = platform_get_drvdata(pdev);

	platform_set_drvdata(pdev, NULL);

	regulator_unregister(ldo->regulator);
	if (gpio_is_valid(ldo->enable))
		gpio_free(ldo->enable);
<<<<<<< HEAD
	kfree(ldo);
=======
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
}

static struct platform_driver wm8994_ldo_driver = {
	.probe = wm8994_ldo_probe,
	.remove = __devexit_p(wm8994_ldo_remove),
=======
err:
	return ret;
}

static struct platform_driver wm8994_ldo_driver = {
	.probe = wm8994_ldo_probe,
>>>>>>> refs/remotes/origin/master
	.driver		= {
		.name	= "wm8994-ldo",
		.owner	= THIS_MODULE,
	},
};

<<<<<<< HEAD
static int __init wm8994_ldo_init(void)
{
	int ret;

	ret = platform_driver_register(&wm8994_ldo_driver);
	if (ret != 0)
		pr_err("Failed to register Wm8994 GP LDO driver: %d\n", ret);

	return ret;
}
subsys_initcall(wm8994_ldo_init);

static void __exit wm8994_ldo_exit(void)
{
	platform_driver_unregister(&wm8994_ldo_driver);
}
module_exit(wm8994_ldo_exit);
=======
module_platform_driver(wm8994_ldo_driver);
>>>>>>> refs/remotes/origin/master

/* Module information */
MODULE_AUTHOR("Mark Brown <broonie@opensource.wolfsonmicro.com>");
MODULE_DESCRIPTION("WM8994 LDO driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:wm8994-ldo");
