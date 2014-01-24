/*
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (c) 2011, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#define pr_fmt(fmt) "%s: " fmt, __func__

#include <linux/err.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/gpio.h>
=======
=======
>>>>>>> refs/remotes/origin/master
 * gpio-regulator.c
 *
 * Copyright 2011 Heiko Stuebner <heiko@sntech.de>
 *
 * based on fixed.c
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
#include <linux/module.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/gpio-regulator.h>
<<<<<<< HEAD

struct gpio_vreg {
	struct regulator_desc	desc;
	struct regulator_dev	*rdev;
	char			*gpio_label;
	char			*name;
	unsigned		gpio;
	int			active_low;
	bool			gpio_requested;
};

static int gpio_vreg_request_gpio(struct gpio_vreg *vreg)
{
	int rc = 0;

	/* Request GPIO now if it hasn't been requested before. */
	if (!vreg->gpio_requested) {
		rc = gpio_request(vreg->gpio, vreg->gpio_label);
		if (rc < 0)
			pr_err("failed to request gpio %u (%s), rc=%d\n",
				vreg->gpio, vreg->gpio_label, rc);
		else
			vreg->gpio_requested = true;

		rc = gpio_sysfs_set_active_low(vreg->gpio, vreg->active_low);
		if (rc < 0)
			pr_err("active_low=%d failed for gpio %u, rc=%d\n",
				vreg->active_low, vreg->gpio, rc);
	}

	return rc;
}

static int gpio_vreg_is_enabled(struct regulator_dev *rdev)
{
	struct gpio_vreg *vreg = rdev_get_drvdata(rdev);
	int rc;

	rc = gpio_vreg_request_gpio(vreg);
	if (rc < 0)
		return rc;

	return (gpio_get_value_cansleep(vreg->gpio) ? 1 : 0) ^ vreg->active_low;
}

static int gpio_vreg_enable(struct regulator_dev *rdev)
{
	struct gpio_vreg *vreg = rdev_get_drvdata(rdev);
	int rc;

	rc = gpio_vreg_request_gpio(vreg);
	if (rc < 0)
		return rc;

	return gpio_direction_output(vreg->gpio, !vreg->active_low);
}

static int gpio_vreg_disable(struct regulator_dev *rdev)
{
	struct gpio_vreg *vreg = rdev_get_drvdata(rdev);
	int rc;

	rc = gpio_vreg_request_gpio(vreg);
	if (rc < 0)
		return rc;

	return gpio_direction_output(vreg->gpio, vreg->active_low);
}

static struct regulator_ops gpio_vreg_ops = {
	.enable		= gpio_vreg_enable,
	.disable	= gpio_vreg_disable,
	.is_enabled	= gpio_vreg_is_enabled,
};

static int __devinit gpio_vreg_probe(struct platform_device *pdev)
{
	const struct gpio_regulator_platform_data *pdata;
	struct gpio_vreg *vreg;
	int rc = 0;

	pdata = pdev->dev.platform_data;

	if (!pdata) {
		pr_err("platform data required.\n");
		return -EINVAL;
	}

	if (!pdata->gpio_label) {
		pr_err("gpio_label required.\n");
		return -EINVAL;
	}

	if (!pdata->regulator_name) {
		pr_err("regulator_name required.\n");
		return -EINVAL;
	}

	vreg = kzalloc(sizeof(struct gpio_vreg), GFP_KERNEL);
	if (!vreg) {
		pr_err("kzalloc failed.\n");
		return -ENOMEM;
	}

	vreg->name = kstrdup(pdata->regulator_name, GFP_KERNEL);
	if (!vreg->name) {
		pr_err("kzalloc failed.\n");
		rc = -ENOMEM;
		goto free_vreg;
	}

	vreg->gpio_label = kstrdup(pdata->gpio_label, GFP_KERNEL);
	if (!vreg->gpio_label) {
		pr_err("kzalloc failed.\n");
		rc = -ENOMEM;
		goto free_name;
	}

	vreg->gpio		= pdata->gpio;
	vreg->active_low	= (pdata->active_low ? 1 : 0);
	vreg->gpio_requested	= false;

	vreg->desc.name		= vreg->name;
	vreg->desc.id		= pdev->id;
	vreg->desc.ops		= &gpio_vreg_ops;
	vreg->desc.type		= REGULATOR_VOLTAGE;
	vreg->desc.owner	= THIS_MODULE;

	vreg->rdev = regulator_register(&vreg->desc, &pdev->dev,
					&pdata->init_data, vreg);
	if (IS_ERR(vreg->rdev)) {
		rc = PTR_ERR(vreg->rdev);
		pr_err("%s: regulator_register failed, rc=%d.\n", vreg->name,
			rc);
		goto free_gpio_label;
	}

	platform_set_drvdata(pdev, vreg);

	pr_info("id=%d, name=%s, gpio=%u, gpio_label=%s\n", pdev->id,
		vreg->name, vreg->gpio, vreg->gpio_label);

	return rc;

free_gpio_label:
	kfree(vreg->gpio_label);
free_name:
	kfree(vreg->name);
free_vreg:
	kfree(vreg);

	return rc;
}

static int __devexit gpio_vreg_remove(struct platform_device *pdev)
{
	struct gpio_vreg *vreg = platform_get_drvdata(pdev);

	if (vreg->gpio_requested)
		gpio_free(vreg->gpio);

	regulator_unregister(vreg->rdev);
	kfree(vreg->name);
	kfree(vreg->gpio_label);
	kfree(vreg);
	platform_set_drvdata(pdev, NULL);
=======
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/slab.h>
=======
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/of_regulator.h>
#include <linux/regulator/gpio-regulator.h>
#include <linux/gpio.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
>>>>>>> refs/remotes/origin/master

struct gpio_regulator_data {
	struct regulator_desc desc;
	struct regulator_dev *dev;

<<<<<<< HEAD
	int enable_gpio;
	bool enable_high;
	bool is_enabled;
	unsigned startup_delay;

=======
>>>>>>> refs/remotes/origin/master
	struct gpio *gpios;
	int nr_gpios;

	struct gpio_regulator_state *states;
	int nr_states;

	int state;
};

<<<<<<< HEAD
static int gpio_regulator_is_enabled(struct regulator_dev *dev)
{
	struct gpio_regulator_data *data = rdev_get_drvdata(dev);

	return data->is_enabled;
}

static int gpio_regulator_enable(struct regulator_dev *dev)
{
	struct gpio_regulator_data *data = rdev_get_drvdata(dev);

	if (gpio_is_valid(data->enable_gpio)) {
		gpio_set_value_cansleep(data->enable_gpio, data->enable_high);
		data->is_enabled = true;
	}

	return 0;
}

static int gpio_regulator_disable(struct regulator_dev *dev)
{
	struct gpio_regulator_data *data = rdev_get_drvdata(dev);

	if (gpio_is_valid(data->enable_gpio)) {
		gpio_set_value_cansleep(data->enable_gpio, !data->enable_high);
		data->is_enabled = false;
	}

	return 0;
}

static int gpio_regulator_enable_time(struct regulator_dev *dev)
{
	struct gpio_regulator_data *data = rdev_get_drvdata(dev);

	return data->startup_delay;
}

=======
>>>>>>> refs/remotes/origin/master
static int gpio_regulator_get_value(struct regulator_dev *dev)
{
	struct gpio_regulator_data *data = rdev_get_drvdata(dev);
	int ptr;

	for (ptr = 0; ptr < data->nr_states; ptr++)
		if (data->states[ptr].gpios == data->state)
			return data->states[ptr].value;

	return -EINVAL;
}

<<<<<<< HEAD
static int gpio_regulator_set_value(struct regulator_dev *dev,
					int min, int max)
{
	struct gpio_regulator_data *data = rdev_get_drvdata(dev);
	int ptr, target, state;

	target = -1;
	for (ptr = 0; ptr < data->nr_states; ptr++)
		if (data->states[ptr].value >= min &&
		    data->states[ptr].value <= max)
			target = data->states[ptr].gpios;

	if (target < 0)
=======
static int gpio_regulator_set_voltage(struct regulator_dev *dev,
					int min_uV, int max_uV,
					unsigned *selector)
{
	struct gpio_regulator_data *data = rdev_get_drvdata(dev);
	int ptr, target = 0, state, best_val = INT_MAX;

	for (ptr = 0; ptr < data->nr_states; ptr++)
		if (data->states[ptr].value < best_val &&
		    data->states[ptr].value >= min_uV &&
		    data->states[ptr].value <= max_uV) {
			target = data->states[ptr].gpios;
			best_val = data->states[ptr].value;
			if (selector)
				*selector = ptr;
		}

	if (best_val == INT_MAX)
>>>>>>> refs/remotes/origin/master
		return -EINVAL;

	for (ptr = 0; ptr < data->nr_gpios; ptr++) {
		state = (target & (1 << ptr)) >> ptr;
<<<<<<< HEAD
		gpio_set_value(data->gpios[ptr].gpio, state);
=======
		gpio_set_value_cansleep(data->gpios[ptr].gpio, state);
>>>>>>> refs/remotes/origin/master
	}
	data->state = target;

	return 0;
}

<<<<<<< HEAD
static int gpio_regulator_set_voltage(struct regulator_dev *dev,
					int min_uV, int max_uV,
					unsigned *selector)
{
	return gpio_regulator_set_value(dev, min_uV, max_uV);
}

=======
>>>>>>> refs/remotes/origin/master
static int gpio_regulator_list_voltage(struct regulator_dev *dev,
				      unsigned selector)
{
	struct gpio_regulator_data *data = rdev_get_drvdata(dev);

	if (selector >= data->nr_states)
		return -EINVAL;

	return data->states[selector].value;
}

static int gpio_regulator_set_current_limit(struct regulator_dev *dev,
					int min_uA, int max_uA)
{
<<<<<<< HEAD
	return gpio_regulator_set_value(dev, min_uA, max_uA);
}

static struct regulator_ops gpio_regulator_voltage_ops = {
	.is_enabled = gpio_regulator_is_enabled,
	.enable = gpio_regulator_enable,
	.disable = gpio_regulator_disable,
	.enable_time = gpio_regulator_enable_time,
=======
	struct gpio_regulator_data *data = rdev_get_drvdata(dev);
	int ptr, target = 0, state, best_val = 0;

	for (ptr = 0; ptr < data->nr_states; ptr++)
		if (data->states[ptr].value > best_val &&
		    data->states[ptr].value >= min_uA &&
		    data->states[ptr].value <= max_uA) {
			target = data->states[ptr].gpios;
			best_val = data->states[ptr].value;
		}

	if (best_val == 0)
		return -EINVAL;

	for (ptr = 0; ptr < data->nr_gpios; ptr++) {
		state = (target & (1 << ptr)) >> ptr;
		gpio_set_value_cansleep(data->gpios[ptr].gpio, state);
	}
	data->state = target;

	return 0;
}

static struct regulator_ops gpio_regulator_voltage_ops = {
>>>>>>> refs/remotes/origin/master
	.get_voltage = gpio_regulator_get_value,
	.set_voltage = gpio_regulator_set_voltage,
	.list_voltage = gpio_regulator_list_voltage,
};

<<<<<<< HEAD
static struct regulator_ops gpio_regulator_current_ops = {
	.is_enabled = gpio_regulator_is_enabled,
	.enable = gpio_regulator_enable,
	.disable = gpio_regulator_disable,
	.enable_time = gpio_regulator_enable_time,
=======
static struct gpio_regulator_config *
of_get_gpio_regulator_config(struct device *dev, struct device_node *np)
{
	struct gpio_regulator_config *config;
	struct property *prop;
	const char *regtype;
	int proplen, gpio, i;
	int ret;

	config = devm_kzalloc(dev,
			sizeof(struct gpio_regulator_config),
			GFP_KERNEL);
	if (!config)
		return ERR_PTR(-ENOMEM);

	config->init_data = of_get_regulator_init_data(dev, np);
	if (!config->init_data)
		return ERR_PTR(-EINVAL);

	config->supply_name = config->init_data->constraints.name;

	if (of_property_read_bool(np, "enable-active-high"))
		config->enable_high = true;

	if (of_property_read_bool(np, "enable-at-boot"))
		config->enabled_at_boot = true;

	of_property_read_u32(np, "startup-delay-us", &config->startup_delay);

	config->enable_gpio = of_get_named_gpio(np, "enable-gpio", 0);

	/* Fetch GPIOs. */
	config->nr_gpios = of_gpio_count(np);

	config->gpios = devm_kzalloc(dev,
				sizeof(struct gpio) * config->nr_gpios,
				GFP_KERNEL);
	if (!config->gpios)
		return ERR_PTR(-ENOMEM);

	for (i = 0; i < config->nr_gpios; i++) {
		gpio = of_get_named_gpio(np, "gpios", i);
		if (gpio < 0)
			break;
		config->gpios[i].gpio = gpio;
	}

	/* Fetch states. */
	prop = of_find_property(np, "states", NULL);
	if (!prop) {
		dev_err(dev, "No 'states' property found\n");
		return ERR_PTR(-EINVAL);
	}

	proplen = prop->length / sizeof(int);

	config->states = devm_kzalloc(dev,
				sizeof(struct gpio_regulator_state)
				* (proplen / 2),
				GFP_KERNEL);
	if (!config->states)
		return ERR_PTR(-ENOMEM);

	for (i = 0; i < proplen / 2; i++) {
		config->states[i].value =
			be32_to_cpup((int *)prop->value + (i * 2));
		config->states[i].gpios =
			be32_to_cpup((int *)prop->value + (i * 2 + 1));
	}
	config->nr_states = i;

	ret = of_property_read_string(np, "regulator-type", &regtype);
	if (ret < 0) {
		dev_err(dev, "Missing 'regulator-type' property\n");
		return ERR_PTR(-EINVAL);
	}

	if (!strncmp("voltage", regtype, 7))
		config->type = REGULATOR_VOLTAGE;
	else if (!strncmp("current", regtype, 7))
		config->type = REGULATOR_CURRENT;

	return config;
}

static struct regulator_ops gpio_regulator_current_ops = {
>>>>>>> refs/remotes/origin/master
	.get_current_limit = gpio_regulator_get_value,
	.set_current_limit = gpio_regulator_set_current_limit,
};

<<<<<<< HEAD
static int __devinit gpio_regulator_probe(struct platform_device *pdev)
{
	struct gpio_regulator_config *config = pdev->dev.platform_data;
	struct gpio_regulator_data *drvdata;
	int ptr, ret, state;

	drvdata = kzalloc(sizeof(struct gpio_regulator_data), GFP_KERNEL);
=======
static int gpio_regulator_probe(struct platform_device *pdev)
{
	struct gpio_regulator_config *config = dev_get_platdata(&pdev->dev);
	struct device_node *np = pdev->dev.of_node;
	struct gpio_regulator_data *drvdata;
	struct regulator_config cfg = { };
	int ptr, ret, state;

	if (np) {
		config = of_get_gpio_regulator_config(&pdev->dev, np);
		if (IS_ERR(config))
			return PTR_ERR(config);
	}

	drvdata = devm_kzalloc(&pdev->dev, sizeof(struct gpio_regulator_data),
			       GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (drvdata == NULL) {
		dev_err(&pdev->dev, "Failed to allocate device data\n");
		return -ENOMEM;
	}

	drvdata->desc.name = kstrdup(config->supply_name, GFP_KERNEL);
	if (drvdata->desc.name == NULL) {
		dev_err(&pdev->dev, "Failed to allocate supply name\n");
		ret = -ENOMEM;
		goto err;
	}

	drvdata->gpios = kmemdup(config->gpios,
				 config->nr_gpios * sizeof(struct gpio),
				 GFP_KERNEL);
	if (drvdata->gpios == NULL) {
		dev_err(&pdev->dev, "Failed to allocate gpio data\n");
		ret = -ENOMEM;
		goto err_name;
	}

	drvdata->states = kmemdup(config->states,
				  config->nr_states *
					 sizeof(struct gpio_regulator_state),
				  GFP_KERNEL);
	if (drvdata->states == NULL) {
		dev_err(&pdev->dev, "Failed to allocate state data\n");
		ret = -ENOMEM;
		goto err_memgpio;
	}
	drvdata->nr_states = config->nr_states;

	drvdata->desc.owner = THIS_MODULE;
<<<<<<< HEAD
=======
	drvdata->desc.enable_time = config->startup_delay;
>>>>>>> refs/remotes/origin/master

	/* handle regulator type*/
	switch (config->type) {
	case REGULATOR_VOLTAGE:
		drvdata->desc.type = REGULATOR_VOLTAGE;
		drvdata->desc.ops = &gpio_regulator_voltage_ops;
		drvdata->desc.n_voltages = config->nr_states;
		break;
	case REGULATOR_CURRENT:
		drvdata->desc.type = REGULATOR_CURRENT;
		drvdata->desc.ops = &gpio_regulator_current_ops;
		break;
	default:
		dev_err(&pdev->dev, "No regulator type set\n");
		ret = -EINVAL;
		goto err_memgpio;
<<<<<<< HEAD
		break;
	}

	drvdata->enable_gpio = config->enable_gpio;
	drvdata->startup_delay = config->startup_delay;

	if (gpio_is_valid(config->enable_gpio)) {
		drvdata->enable_high = config->enable_high;

		ret = gpio_request(config->enable_gpio, config->supply_name);
		if (ret) {
			dev_err(&pdev->dev,
			   "Could not obtain regulator enable GPIO %d: %d\n",
						config->enable_gpio, ret);
			goto err_memstate;
		}

		/* set output direction without changing state
		 * to prevent glitch
		 */
		if (config->enabled_at_boot) {
			drvdata->is_enabled = true;
			ret = gpio_direction_output(config->enable_gpio,
						    config->enable_high);
		} else {
			drvdata->is_enabled = false;
			ret = gpio_direction_output(config->enable_gpio,
						    !config->enable_high);
		}

		if (ret) {
			dev_err(&pdev->dev,
			   "Could not configure regulator enable GPIO %d direction: %d\n",
						config->enable_gpio, ret);
			goto err_enablegpio;
		}
	} else {
		/* Regulator without GPIO control is considered
		 * always enabled
		 */
		drvdata->is_enabled = true;
=======
>>>>>>> refs/remotes/origin/master
	}

	drvdata->nr_gpios = config->nr_gpios;
	ret = gpio_request_array(drvdata->gpios, drvdata->nr_gpios);
	if (ret) {
		dev_err(&pdev->dev,
		   "Could not obtain regulator setting GPIOs: %d\n", ret);
<<<<<<< HEAD
		goto err_enablegpio;
=======
		goto err_memstate;
>>>>>>> refs/remotes/origin/master
	}

	/* build initial state from gpio init data. */
	state = 0;
	for (ptr = 0; ptr < drvdata->nr_gpios; ptr++) {
		if (config->gpios[ptr].flags & GPIOF_OUT_INIT_HIGH)
			state |= (1 << ptr);
	}
	drvdata->state = state;

<<<<<<< HEAD
	drvdata->dev = regulator_register(&drvdata->desc, &pdev->dev,
					  config->init_data, drvdata, NULL);
=======
	cfg.dev = &pdev->dev;
	cfg.init_data = config->init_data;
	cfg.driver_data = drvdata;
	cfg.of_node = np;

	if (config->enable_gpio >= 0)
		cfg.ena_gpio = config->enable_gpio;
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

	drvdata->dev = regulator_register(&drvdata->desc, &cfg);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(drvdata->dev)) {
		ret = PTR_ERR(drvdata->dev);
		dev_err(&pdev->dev, "Failed to register regulator: %d\n", ret);
		goto err_stategpio;
	}

	platform_set_drvdata(pdev, drvdata);

	return 0;

err_stategpio:
	gpio_free_array(drvdata->gpios, drvdata->nr_gpios);
<<<<<<< HEAD
err_enablegpio:
	if (gpio_is_valid(config->enable_gpio))
		gpio_free(config->enable_gpio);
=======
>>>>>>> refs/remotes/origin/master
err_memstate:
	kfree(drvdata->states);
err_memgpio:
	kfree(drvdata->gpios);
err_name:
	kfree(drvdata->desc.name);
err:
<<<<<<< HEAD
	kfree(drvdata);
	return ret;
}

static int __devexit gpio_regulator_remove(struct platform_device *pdev)
=======
	return ret;
}

static int gpio_regulator_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct gpio_regulator_data *drvdata = platform_get_drvdata(pdev);

	regulator_unregister(drvdata->dev);

	gpio_free_array(drvdata->gpios, drvdata->nr_gpios);

	kfree(drvdata->states);
	kfree(drvdata->gpios);

<<<<<<< HEAD
	if (gpio_is_valid(drvdata->enable_gpio))
		gpio_free(drvdata->enable_gpio);

	kfree(drvdata->desc.name);
	kfree(drvdata);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	kfree(drvdata->desc.name);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct platform_driver gpio_vreg_driver = {
	.probe = gpio_vreg_probe,
	.remove = __devexit_p(gpio_vreg_remove),
	.driver = {
		.name = GPIO_REGULATOR_DEV_NAME,
		.owner = THIS_MODULE,
	},
};

static int __init gpio_vreg_init(void)
{
	return platform_driver_register(&gpio_vreg_driver);
}

static void __exit gpio_vreg_exit(void)
{
	platform_driver_unregister(&gpio_vreg_driver);
}

postcore_initcall(gpio_vreg_init);
module_exit(gpio_vreg_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("GPIO regulator driver");
MODULE_VERSION("1.0");
MODULE_ALIAS("platform:" GPIO_REGULATOR_DEV_NAME);
=======
static struct platform_driver gpio_regulator_driver = {
	.probe		= gpio_regulator_probe,
	.remove		= __devexit_p(gpio_regulator_remove),
	.driver		= {
		.name		= "gpio-regulator",
		.owner		= THIS_MODULE,
=======
#if defined(CONFIG_OF)
static const struct of_device_id regulator_gpio_of_match[] = {
	{ .compatible = "regulator-gpio", },
	{},
};
#endif

static struct platform_driver gpio_regulator_driver = {
	.probe		= gpio_regulator_probe,
	.remove		= gpio_regulator_remove,
	.driver		= {
		.name		= "gpio-regulator",
		.owner		= THIS_MODULE,
		.of_match_table = of_match_ptr(regulator_gpio_of_match),
>>>>>>> refs/remotes/origin/master
	},
};

static int __init gpio_regulator_init(void)
{
	return platform_driver_register(&gpio_regulator_driver);
}
subsys_initcall(gpio_regulator_init);

static void __exit gpio_regulator_exit(void)
{
	platform_driver_unregister(&gpio_regulator_driver);
}
module_exit(gpio_regulator_exit);

MODULE_AUTHOR("Heiko Stuebner <heiko@sntech.de>");
MODULE_DESCRIPTION("gpio voltage regulator");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:gpio-regulator");
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
