/*
 * fixed.c
 *
 * Copyright 2008 Wolfson Microelectronics PLC.
 *
 * Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
 *
 * Copyright (c) 2009 Nokia Corporation
 * Roger Quadros <ext-roger.quadros@nokia.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This is useful for systems with mixed controllable and
 * non-controllable regulators, as well as for allowing testing on
 * systems with no controllable regulators.
 */

#include <linux/err.h>
#include <linux/mutex.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/master
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/fixed.h>
#include <linux/gpio.h>
<<<<<<< HEAD
#include <linux/delay.h>
#include <linux/slab.h>
<<<<<<< HEAD
=======
=======
#include <linux/slab.h>
>>>>>>> refs/remotes/origin/master
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/regulator/of_regulator.h>
#include <linux/regulator/machine.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

struct fixed_voltage_data {
	struct regulator_desc desc;
	struct regulator_dev *dev;
<<<<<<< HEAD
	int microvolts;
	int gpio;
	unsigned startup_delay;
	bool enable_high;
	bool is_enabled;
};

<<<<<<< HEAD
=======
=======
};

>>>>>>> refs/remotes/origin/master

/**
 * of_get_fixed_voltage_config - extract fixed_voltage_config structure info
 * @dev: device requesting for fixed_voltage_config
 *
 * Populates fixed_voltage_config structure by extracting data from device
 * tree node, returns a pointer to the populated structure of NULL if memory
 * alloc fails.
 */
static struct fixed_voltage_config *
of_get_fixed_voltage_config(struct device *dev)
{
	struct fixed_voltage_config *config;
	struct device_node *np = dev->of_node;
	const __be32 *delay;
	struct regulator_init_data *init_data;

	config = devm_kzalloc(dev, sizeof(struct fixed_voltage_config),
								 GFP_KERNEL);
	if (!config)
<<<<<<< HEAD
		return NULL;

	config->init_data = of_get_regulator_init_data(dev, dev->of_node);
	if (!config->init_data)
		return NULL;
=======
		return ERR_PTR(-ENOMEM);

	config->init_data = of_get_regulator_init_data(dev, dev->of_node);
	if (!config->init_data)
		return ERR_PTR(-EINVAL);
>>>>>>> refs/remotes/origin/master

	init_data = config->init_data;
	init_data->constraints.apply_uV = 0;

	config->supply_name = init_data->constraints.name;
	if (init_data->constraints.min_uV == init_data->constraints.max_uV) {
		config->microvolts = init_data->constraints.min_uV;
	} else {
		dev_err(dev,
			 "Fixed regulator specified with variable voltages\n");
<<<<<<< HEAD
		return NULL;
=======
		return ERR_PTR(-EINVAL);
>>>>>>> refs/remotes/origin/master
	}

	if (init_data->constraints.boot_on)
		config->enabled_at_boot = true;

	config->gpio = of_get_named_gpio(np, "gpio", 0);
<<<<<<< HEAD
=======
	/*
	 * of_get_named_gpio() currently returns ENODEV rather than
	 * EPROBE_DEFER. This code attempts to be compatible with both
	 * for now; the ENODEV check can be removed once the API is fixed.
	 * of_get_named_gpio() doesn't differentiate between a missing
	 * property (which would be fine here, since the GPIO is optional)
	 * and some other error. Patches have been posted for both issues.
	 * Once they are check in, we should replace this with:
	 * if (config->gpio < 0 && config->gpio != -ENOENT)
	 */
	if ((config->gpio == -ENODEV) || (config->gpio == -EPROBE_DEFER))
		return ERR_PTR(-EPROBE_DEFER);

>>>>>>> refs/remotes/origin/master
	delay = of_get_property(np, "startup-delay-us", NULL);
	if (delay)
		config->startup_delay = be32_to_cpu(*delay);

	if (of_find_property(np, "enable-active-high", NULL))
		config->enable_high = true;

<<<<<<< HEAD
	return config;
}

>>>>>>> refs/remotes/origin/cm-10.0
static int fixed_voltage_is_enabled(struct regulator_dev *dev)
{
	struct fixed_voltage_data *data = rdev_get_drvdata(dev);

	return data->is_enabled;
}

static int fixed_voltage_enable(struct regulator_dev *dev)
{
	struct fixed_voltage_data *data = rdev_get_drvdata(dev);

	if (gpio_is_valid(data->gpio)) {
		gpio_set_value_cansleep(data->gpio, data->enable_high);
		data->is_enabled = true;
	}

	return 0;
}

static int fixed_voltage_disable(struct regulator_dev *dev)
{
	struct fixed_voltage_data *data = rdev_get_drvdata(dev);

	if (gpio_is_valid(data->gpio)) {
		gpio_set_value_cansleep(data->gpio, !data->enable_high);
		data->is_enabled = false;
	}

	return 0;
}

static int fixed_voltage_enable_time(struct regulator_dev *dev)
{
	struct fixed_voltage_data *data = rdev_get_drvdata(dev);

	return data->startup_delay;
}

static int fixed_voltage_get_voltage(struct regulator_dev *dev)
{
	struct fixed_voltage_data *data = rdev_get_drvdata(dev);

<<<<<<< HEAD
	return data->microvolts;
=======
	if (data->microvolts)
		return data->microvolts;
	else
		return -EINVAL;
>>>>>>> refs/remotes/origin/cm-10.0
}

static int fixed_voltage_list_voltage(struct regulator_dev *dev,
				      unsigned selector)
{
	struct fixed_voltage_data *data = rdev_get_drvdata(dev);

	if (selector != 0)
		return -EINVAL;

	return data->microvolts;
}

static struct regulator_ops fixed_voltage_ops = {
	.is_enabled = fixed_voltage_is_enabled,
	.enable = fixed_voltage_enable,
	.disable = fixed_voltage_disable,
	.enable_time = fixed_voltage_enable_time,
	.get_voltage = fixed_voltage_get_voltage,
	.list_voltage = fixed_voltage_list_voltage,
};

static int __devinit reg_fixed_voltage_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct fixed_voltage_config *config = pdev->dev.platform_data;
	struct fixed_voltage_data *drvdata;
	int ret;

=======
	struct fixed_voltage_config *config;
	struct fixed_voltage_data *drvdata;
	int ret;

	if (pdev->dev.of_node)
		config = of_get_fixed_voltage_config(&pdev->dev);
	else
		config = pdev->dev.platform_data;
=======
	if (of_find_property(np, "gpio-open-drain", NULL))
		config->gpio_is_open_drain = true;

	if (of_find_property(np, "vin-supply", NULL))
		config->input_supply = "vin";

	return config;
}

static struct regulator_ops fixed_voltage_ops = {
};

static int reg_fixed_voltage_probe(struct platform_device *pdev)
{
	struct fixed_voltage_config *config;
	struct fixed_voltage_data *drvdata;
	struct regulator_config cfg = { };
	int ret;

	if (pdev->dev.of_node) {
		config = of_get_fixed_voltage_config(&pdev->dev);
		if (IS_ERR(config))
			return PTR_ERR(config);
	} else {
		config = dev_get_platdata(&pdev->dev);
	}
>>>>>>> refs/remotes/origin/master

	if (!config)
		return -ENOMEM;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	drvdata = kzalloc(sizeof(struct fixed_voltage_data), GFP_KERNEL);
=======
	drvdata = devm_kzalloc(&pdev->dev, sizeof(struct fixed_voltage_data),
			       GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (drvdata == NULL) {
		dev_err(&pdev->dev, "Failed to allocate device data\n");
		ret = -ENOMEM;
		goto err;
	}

	drvdata->desc.name = kstrdup(config->supply_name, GFP_KERNEL);
	if (drvdata->desc.name == NULL) {
		dev_err(&pdev->dev, "Failed to allocate supply name\n");
		ret = -ENOMEM;
		goto err;
	}
	drvdata->desc.type = REGULATOR_VOLTAGE;
	drvdata->desc.owner = THIS_MODULE;
	drvdata->desc.ops = &fixed_voltage_ops;
<<<<<<< HEAD
<<<<<<< HEAD
	drvdata->desc.n_voltages = 1;
=======

	if (config->microvolts)
		drvdata->desc.n_voltages = 1;
>>>>>>> refs/remotes/origin/cm-10.0

	drvdata->microvolts = config->microvolts;
	drvdata->gpio = config->gpio;
	drvdata->startup_delay = config->startup_delay;

	if (gpio_is_valid(config->gpio)) {
		drvdata->enable_high = config->enable_high;

		/* FIXME: Remove below print warning
		 *
		 * config->gpio must be set to -EINVAL by platform code if
		 * GPIO control is not required. However, early adopters
		 * not requiring GPIO control may forget to initialize
		 * config->gpio to -EINVAL. This will cause GPIO 0 to be used
		 * for GPIO control.
		 *
		 * This warning will be removed once there are a couple of users
		 * for this driver.
		 */
		if (!config->gpio)
			dev_warn(&pdev->dev,
				"using GPIO 0 for regulator enable control\n");

		ret = gpio_request(config->gpio, config->supply_name);
		if (ret) {
			dev_err(&pdev->dev,
			   "Could not obtain regulator enable GPIO %d: %d\n",
							config->gpio, ret);
			goto err_name;
		}

		/* set output direction without changing state
		 * to prevent glitch
		 */
		drvdata->is_enabled = config->enabled_at_boot;
		ret = drvdata->is_enabled ?
				config->enable_high : !config->enable_high;

		ret = gpio_direction_output(config->gpio, ret);
		if (ret) {
			dev_err(&pdev->dev,
			   "Could not configure regulator enable GPIO %d direction: %d\n",
							config->gpio, ret);
			goto err_gpio;
		}

	} else {
		/* Regulator without GPIO control is considered
		 * always enabled
		 */
		drvdata->is_enabled = true;
	}

	drvdata->dev = regulator_register(&drvdata->desc, &pdev->dev,
<<<<<<< HEAD
					  config->init_data, drvdata);
=======
					  config->init_data, drvdata,
					  pdev->dev.of_node);
>>>>>>> refs/remotes/origin/cm-10.0
	if (IS_ERR(drvdata->dev)) {
		ret = PTR_ERR(drvdata->dev);
		dev_err(&pdev->dev, "Failed to register regulator: %d\n", ret);
		goto err_gpio;
=======

	drvdata->desc.enable_time = config->startup_delay;

	if (config->input_supply) {
		drvdata->desc.supply_name = kstrdup(config->input_supply,
							GFP_KERNEL);
		if (!drvdata->desc.supply_name) {
			dev_err(&pdev->dev,
				"Failed to allocate input supply\n");
			ret = -ENOMEM;
			goto err_name;
		}
	}

	if (config->microvolts)
		drvdata->desc.n_voltages = 1;

	drvdata->desc.fixed_uV = config->microvolts;

	if (config->gpio >= 0)
		cfg.ena_gpio = config->gpio;
	cfg.ena_gpio_invert = !config->enable_high;
	if (config->enabled_at_boot) {
		if (config->enable_high)
			cfg.ena_gpio_flags |= GPIOF_OUT_INIT_HIGH;
		else
			cfg.ena_gpio_flags |= GPIOF_OUT_INIT_LOW;
	} else {
		if (config->enable_high)
			cfg.ena_gpio_flags |= GPIOF_OUT_INIT_LOW;
		else
			cfg.ena_gpio_flags |= GPIOF_OUT_INIT_HIGH;
	}
	if (config->gpio_is_open_drain)
		cfg.ena_gpio_flags |= GPIOF_OPEN_DRAIN;

	cfg.dev = &pdev->dev;
	cfg.init_data = config->init_data;
	cfg.driver_data = drvdata;
	cfg.of_node = pdev->dev.of_node;

	drvdata->dev = regulator_register(&drvdata->desc, &cfg);
	if (IS_ERR(drvdata->dev)) {
		ret = PTR_ERR(drvdata->dev);
		dev_err(&pdev->dev, "Failed to register regulator: %d\n", ret);
		goto err_input;
>>>>>>> refs/remotes/origin/master
	}

	platform_set_drvdata(pdev, drvdata);

	dev_dbg(&pdev->dev, "%s supplying %duV\n", drvdata->desc.name,
<<<<<<< HEAD
		drvdata->microvolts);

	return 0;

err_gpio:
	if (gpio_is_valid(config->gpio))
		gpio_free(config->gpio);
err_name:
	kfree(drvdata->desc.name);
err:
	kfree(drvdata);
	return ret;
}

static int __devexit reg_fixed_voltage_remove(struct platform_device *pdev)
=======
		drvdata->desc.fixed_uV);

	return 0;

err_input:
	kfree(drvdata->desc.supply_name);
err_name:
	kfree(drvdata->desc.name);
err:
	return ret;
}

static int reg_fixed_voltage_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct fixed_voltage_data *drvdata = platform_get_drvdata(pdev);

	regulator_unregister(drvdata->dev);
<<<<<<< HEAD
	if (gpio_is_valid(drvdata->gpio))
		gpio_free(drvdata->gpio);
	kfree(drvdata->desc.name);
	kfree(drvdata);
=======
	kfree(drvdata->desc.supply_name);
	kfree(drvdata->desc.name);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
#if defined(CONFIG_OF)
static const struct of_device_id fixed_of_match[] __devinitconst = {
=======
#if defined(CONFIG_OF)
static const struct of_device_id fixed_of_match[] = {
>>>>>>> refs/remotes/origin/master
	{ .compatible = "regulator-fixed", },
	{},
};
MODULE_DEVICE_TABLE(of, fixed_of_match);
<<<<<<< HEAD
#else
#define fixed_of_match NULL
#endif

>>>>>>> refs/remotes/origin/cm-10.0
static struct platform_driver regulator_fixed_voltage_driver = {
	.probe		= reg_fixed_voltage_probe,
	.remove		= __devexit_p(reg_fixed_voltage_remove),
	.driver		= {
		.name		= "reg-fixed-voltage",
		.owner		= THIS_MODULE,
<<<<<<< HEAD
=======
		.of_match_table = fixed_of_match,
>>>>>>> refs/remotes/origin/cm-10.0
=======
#endif

static struct platform_driver regulator_fixed_voltage_driver = {
	.probe		= reg_fixed_voltage_probe,
	.remove		= reg_fixed_voltage_remove,
	.driver		= {
		.name		= "reg-fixed-voltage",
		.owner		= THIS_MODULE,
		.of_match_table = of_match_ptr(fixed_of_match),
>>>>>>> refs/remotes/origin/master
	},
};

static int __init regulator_fixed_voltage_init(void)
{
	return platform_driver_register(&regulator_fixed_voltage_driver);
}
subsys_initcall(regulator_fixed_voltage_init);

static void __exit regulator_fixed_voltage_exit(void)
{
	platform_driver_unregister(&regulator_fixed_voltage_driver);
}
module_exit(regulator_fixed_voltage_exit);

MODULE_AUTHOR("Mark Brown <broonie@opensource.wolfsonmicro.com>");
MODULE_DESCRIPTION("Fixed voltage regulator");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:reg-fixed-voltage");
