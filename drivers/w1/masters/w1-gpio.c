/*
 * w1-gpio - GPIO w1 bus master driver
 *
 * Copyright (C) 2007 Ville Syrjala <syrjala@sci.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/w1-gpio.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/gpio.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/gpio.h>
#include <linux/of_platform.h>
#include <linux/of_gpio.h>
#include <linux/err.h>
#include <linux/of.h>
>>>>>>> refs/remotes/origin/master

#include "../w1.h"
#include "../w1_int.h"

<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/gpio.h>

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static void w1_gpio_write_bit_dir(void *data, u8 bit)
{
	struct w1_gpio_platform_data *pdata = data;

	if (bit)
		gpio_direction_input(pdata->pin);
	else
		gpio_direction_output(pdata->pin, 0);
}

static void w1_gpio_write_bit_val(void *data, u8 bit)
{
	struct w1_gpio_platform_data *pdata = data;

	gpio_set_value(pdata->pin, bit);
}

static u8 w1_gpio_read_bit(void *data)
{
	struct w1_gpio_platform_data *pdata = data;

	return gpio_get_value(pdata->pin) ? 1 : 0;
}

<<<<<<< HEAD
static int __init w1_gpio_probe(struct platform_device *pdev)
{
	struct w1_bus_master *master;
	struct w1_gpio_platform_data *pdata = pdev->dev.platform_data;
	int err;

	if (!pdata)
		return -ENXIO;

	master = kzalloc(sizeof(struct w1_bus_master), GFP_KERNEL);
	if (!master)
		return -ENOMEM;

	err = gpio_request(pdata->pin, "w1");
	if (err)
		goto free_master;
=======
#if defined(CONFIG_OF)
static struct of_device_id w1_gpio_dt_ids[] = {
	{ .compatible = "w1-gpio" },
	{}
};
MODULE_DEVICE_TABLE(of, w1_gpio_dt_ids);
#endif

static int w1_gpio_probe_dt(struct platform_device *pdev)
{
	struct w1_gpio_platform_data *pdata = dev_get_platdata(&pdev->dev);
	struct device_node *np = pdev->dev.of_node;
	int gpio;

	pdata = devm_kzalloc(&pdev->dev, sizeof(*pdata), GFP_KERNEL);
	if (!pdata)
		return -ENOMEM;

	if (of_get_property(np, "linux,open-drain", NULL))
		pdata->is_open_drain = 1;

	gpio = of_get_gpio(np, 0);
	if (gpio < 0)
		return gpio;
	pdata->pin = gpio;

	pdata->ext_pullup_enable_pin = of_get_gpio(np, 1);
	pdev->dev.platform_data = pdata;

	return 0;
}

static int w1_gpio_probe(struct platform_device *pdev)
{
	struct w1_bus_master *master;
	struct w1_gpio_platform_data *pdata;
	int err;

	if (of_have_populated_dt()) {
		err = w1_gpio_probe_dt(pdev);
		if (err < 0) {
			dev_err(&pdev->dev, "Failed to parse DT\n");
			return err;
		}
	}

	pdata = dev_get_platdata(&pdev->dev);

	if (!pdata) {
		dev_err(&pdev->dev, "No configuration data\n");
		return -ENXIO;
	}

	master = devm_kzalloc(&pdev->dev, sizeof(struct w1_bus_master),
			GFP_KERNEL);
	if (!master) {
		dev_err(&pdev->dev, "Out of memory\n");
		return -ENOMEM;
	}

	err = devm_gpio_request(&pdev->dev, pdata->pin, "w1");
	if (err) {
		dev_err(&pdev->dev, "gpio_request (pin) failed\n");
		return err;
	}

	if (gpio_is_valid(pdata->ext_pullup_enable_pin)) {
		err = devm_gpio_request_one(&pdev->dev,
				pdata->ext_pullup_enable_pin, GPIOF_INIT_LOW,
				"w1 pullup");
		if (err < 0) {
			dev_err(&pdev->dev, "gpio_request_one "
					"(ext_pullup_enable_pin) failed\n");
			return err;
		}
	}
>>>>>>> refs/remotes/origin/master

	master->data = pdata;
	master->read_bit = w1_gpio_read_bit;

	if (pdata->is_open_drain) {
		gpio_direction_output(pdata->pin, 1);
		master->write_bit = w1_gpio_write_bit_val;
	} else {
		gpio_direction_input(pdata->pin);
		master->write_bit = w1_gpio_write_bit_dir;
	}

	err = w1_add_master_device(master);
<<<<<<< HEAD
	if (err)
		goto free_gpio;
=======
	if (err) {
		dev_err(&pdev->dev, "w1_add_master device failed\n");
		return err;
	}
>>>>>>> refs/remotes/origin/master

	if (pdata->enable_external_pullup)
		pdata->enable_external_pullup(1);

<<<<<<< HEAD
	platform_set_drvdata(pdev, master);

	return 0;

 free_gpio:
	gpio_free(pdata->pin);
 free_master:
	kfree(master);

	return err;
}

static int __exit w1_gpio_remove(struct platform_device *pdev)
{
	struct w1_bus_master *master = platform_get_drvdata(pdev);
	struct w1_gpio_platform_data *pdata = pdev->dev.platform_data;
=======
	if (gpio_is_valid(pdata->ext_pullup_enable_pin))
		gpio_set_value(pdata->ext_pullup_enable_pin, 1);

	platform_set_drvdata(pdev, master);

	return 0;
}

static int w1_gpio_remove(struct platform_device *pdev)
{
	struct w1_bus_master *master = platform_get_drvdata(pdev);
	struct w1_gpio_platform_data *pdata = dev_get_platdata(&pdev->dev);
>>>>>>> refs/remotes/origin/master

	if (pdata->enable_external_pullup)
		pdata->enable_external_pullup(0);

<<<<<<< HEAD
	w1_remove_master_device(master);
	gpio_free(pdata->pin);
	kfree(master);
=======
	if (gpio_is_valid(pdata->ext_pullup_enable_pin))
		gpio_set_value(pdata->ext_pullup_enable_pin, 0);

	w1_remove_master_device(master);
>>>>>>> refs/remotes/origin/master

	return 0;
}

#ifdef CONFIG_PM

static int w1_gpio_suspend(struct platform_device *pdev, pm_message_t state)
{
<<<<<<< HEAD
	struct w1_gpio_platform_data *pdata = pdev->dev.platform_data;
=======
	struct w1_gpio_platform_data *pdata = dev_get_platdata(&pdev->dev);
>>>>>>> refs/remotes/origin/master

	if (pdata->enable_external_pullup)
		pdata->enable_external_pullup(0);

	return 0;
}

static int w1_gpio_resume(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct w1_gpio_platform_data *pdata = pdev->dev.platform_data;
=======
	struct w1_gpio_platform_data *pdata = dev_get_platdata(&pdev->dev);
>>>>>>> refs/remotes/origin/master

	if (pdata->enable_external_pullup)
		pdata->enable_external_pullup(1);

	return 0;
}

#else
#define w1_gpio_suspend	NULL
#define w1_gpio_resume	NULL
#endif

static struct platform_driver w1_gpio_driver = {
	.driver = {
		.name	= "w1-gpio",
		.owner	= THIS_MODULE,
<<<<<<< HEAD
	},
	.remove	= __exit_p(w1_gpio_remove),
=======
		.of_match_table = of_match_ptr(w1_gpio_dt_ids),
	},
	.probe = w1_gpio_probe,
	.remove	= w1_gpio_remove,
>>>>>>> refs/remotes/origin/master
	.suspend = w1_gpio_suspend,
	.resume = w1_gpio_resume,
};

<<<<<<< HEAD
static int __init w1_gpio_init(void)
{
	return platform_driver_probe(&w1_gpio_driver, w1_gpio_probe);
}

static void __exit w1_gpio_exit(void)
{
	platform_driver_unregister(&w1_gpio_driver);
}

module_init(w1_gpio_init);
module_exit(w1_gpio_exit);
=======
module_platform_driver(w1_gpio_driver);
>>>>>>> refs/remotes/origin/master

MODULE_DESCRIPTION("GPIO w1 bus master driver");
MODULE_AUTHOR("Ville Syrjala <syrjala@sci.fi>");
MODULE_LICENSE("GPL");
