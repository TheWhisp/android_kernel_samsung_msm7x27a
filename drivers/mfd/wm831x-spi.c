/*
 * wm831x-spi.c  --  SPI access for Wolfson WM831x PMICs
 *
 * Copyright 2009,2010 Wolfson Microelectronics PLC.
 *
 * Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pm.h>
#include <linux/spi/spi.h>
<<<<<<< HEAD

#include <linux/mfd/wm831x/core.h>

static int wm831x_spi_read_device(struct wm831x *wm831x, unsigned short reg,
				  int bytes, void *dest)
{
	u16 tx_val;
	u16 *d = dest;
	int r, ret;

	/* Go register at a time */
	for (r = reg; r < reg + (bytes / 2); r++) {
		tx_val = r | 0x8000;

		ret = spi_write_then_read(wm831x->control_data,
					  (u8 *)&tx_val, 2, (u8 *)d, 2);
		if (ret != 0)
			return ret;

		*d = be16_to_cpu(*d);

		d++;
	}

	return 0;
}

static int wm831x_spi_write_device(struct wm831x *wm831x, unsigned short reg,
				   int bytes, void *src)
{
	struct spi_device *spi = wm831x->control_data;
	u16 *s = src;
	u16 data[2];
	int ret, r;

	/* Go register at a time */
	for (r = reg; r < reg + (bytes / 2); r++) {
		data[0] = r;
		data[1] = *s++;

		ret = spi_write(spi, (char *)&data, sizeof(data));
		if (ret != 0)
			return ret;
	}

	return 0;
}

static int __devinit wm831x_spi_probe(struct spi_device *spi)
{
	struct wm831x *wm831x;
	enum wm831x_parent type;

	/* Currently SPI support for ID tables is unmerged, we're faking it */
	if (strcmp(spi->modalias, "wm8310") == 0)
		type = WM8310;
	else if (strcmp(spi->modalias, "wm8311") == 0)
		type = WM8311;
	else if (strcmp(spi->modalias, "wm8312") == 0)
		type = WM8312;
	else if (strcmp(spi->modalias, "wm8320") == 0)
		type = WM8320;
	else if (strcmp(spi->modalias, "wm8321") == 0)
		type = WM8321;
	else if (strcmp(spi->modalias, "wm8325") == 0)
		type = WM8325;
	else if (strcmp(spi->modalias, "wm8326") == 0)
		type = WM8326;
	else {
		dev_err(&spi->dev, "Unknown device type\n");
		return -EINVAL;
	}

	wm831x = kzalloc(sizeof(struct wm831x), GFP_KERNEL);
=======
#include <linux/regmap.h>
#include <linux/err.h>

#include <linux/mfd/wm831x/core.h>

static int __devinit wm831x_spi_probe(struct spi_device *spi)
{
	const struct spi_device_id *id = spi_get_device_id(spi);
	struct wm831x *wm831x;
	enum wm831x_parent type;
	int ret;

	type = (enum wm831x_parent)id->driver_data;

	wm831x = devm_kzalloc(&spi->dev, sizeof(struct wm831x), GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-10.0
	if (wm831x == NULL)
		return -ENOMEM;

	spi->bits_per_word = 16;
	spi->mode = SPI_MODE_0;

	dev_set_drvdata(&spi->dev, wm831x);
	wm831x->dev = &spi->dev;
<<<<<<< HEAD
	wm831x->control_data = spi;
	wm831x->read_dev = wm831x_spi_read_device;
	wm831x->write_dev = wm831x_spi_write_device;
=======

	wm831x->regmap = devm_regmap_init_spi(spi, &wm831x_regmap_config);
	if (IS_ERR(wm831x->regmap)) {
		ret = PTR_ERR(wm831x->regmap);
		dev_err(wm831x->dev, "Failed to allocate register map: %d\n",
			ret);
		return ret;
	}
>>>>>>> refs/remotes/origin/cm-10.0

	return wm831x_device_init(wm831x, type, spi->irq);
}

static int __devexit wm831x_spi_remove(struct spi_device *spi)
{
	struct wm831x *wm831x = dev_get_drvdata(&spi->dev);

	wm831x_device_exit(wm831x);

	return 0;
}

static int wm831x_spi_suspend(struct device *dev)
{
	struct wm831x *wm831x = dev_get_drvdata(dev);

	return wm831x_device_suspend(wm831x);
}

<<<<<<< HEAD
=======
static void wm831x_spi_shutdown(struct spi_device *spi)
{
	struct wm831x *wm831x = dev_get_drvdata(&spi->dev);

	wm831x_device_shutdown(wm831x);
}

>>>>>>> refs/remotes/origin/cm-10.0
static const struct dev_pm_ops wm831x_spi_pm = {
	.freeze = wm831x_spi_suspend,
	.suspend = wm831x_spi_suspend,
};

<<<<<<< HEAD
static struct spi_driver wm8310_spi_driver = {
	.driver = {
		.name	= "wm8310",
		.bus	= &spi_bus_type,
		.owner	= THIS_MODULE,
		.pm	= &wm831x_spi_pm,
	},
	.probe		= wm831x_spi_probe,
	.remove		= __devexit_p(wm831x_spi_remove),
};

static struct spi_driver wm8311_spi_driver = {
	.driver = {
		.name	= "wm8311",
		.bus	= &spi_bus_type,
		.owner	= THIS_MODULE,
		.pm	= &wm831x_spi_pm,
	},
	.probe		= wm831x_spi_probe,
	.remove		= __devexit_p(wm831x_spi_remove),
};

static struct spi_driver wm8312_spi_driver = {
	.driver = {
		.name	= "wm8312",
		.bus	= &spi_bus_type,
		.owner	= THIS_MODULE,
		.pm	= &wm831x_spi_pm,
	},
	.probe		= wm831x_spi_probe,
	.remove		= __devexit_p(wm831x_spi_remove),
};

static struct spi_driver wm8320_spi_driver = {
	.driver = {
		.name	= "wm8320",
		.bus	= &spi_bus_type,
		.owner	= THIS_MODULE,
		.pm	= &wm831x_spi_pm,
	},
	.probe		= wm831x_spi_probe,
	.remove		= __devexit_p(wm831x_spi_remove),
};

static struct spi_driver wm8321_spi_driver = {
	.driver = {
		.name	= "wm8321",
		.bus	= &spi_bus_type,
		.owner	= THIS_MODULE,
		.pm	= &wm831x_spi_pm,
	},
	.probe		= wm831x_spi_probe,
	.remove		= __devexit_p(wm831x_spi_remove),
};

static struct spi_driver wm8325_spi_driver = {
	.driver = {
		.name	= "wm8325",
		.bus	= &spi_bus_type,
		.owner	= THIS_MODULE,
		.pm	= &wm831x_spi_pm,
	},
	.probe		= wm831x_spi_probe,
	.remove		= __devexit_p(wm831x_spi_remove),
};

static struct spi_driver wm8326_spi_driver = {
	.driver = {
		.name	= "wm8326",
		.bus	= &spi_bus_type,
		.owner	= THIS_MODULE,
		.pm	= &wm831x_spi_pm,
	},
	.probe		= wm831x_spi_probe,
	.remove		= __devexit_p(wm831x_spi_remove),
=======
static const struct spi_device_id wm831x_spi_ids[] = {
	{ "wm8310", WM8310 },
	{ "wm8311", WM8311 },
	{ "wm8312", WM8312 },
	{ "wm8320", WM8320 },
	{ "wm8321", WM8321 },
	{ "wm8325", WM8325 },
	{ "wm8326", WM8326 },
	{ },
};
MODULE_DEVICE_TABLE(spi, wm831x_spi_ids);

static struct spi_driver wm831x_spi_driver = {
	.driver = {
		.name	= "wm831x",
		.owner	= THIS_MODULE,
		.pm	= &wm831x_spi_pm,
	},
	.id_table	= wm831x_spi_ids,
	.probe		= wm831x_spi_probe,
	.remove		= __devexit_p(wm831x_spi_remove),
	.shutdown	= wm831x_spi_shutdown,
>>>>>>> refs/remotes/origin/cm-10.0
};

static int __init wm831x_spi_init(void)
{
	int ret;

<<<<<<< HEAD
	ret = spi_register_driver(&wm8310_spi_driver);
	if (ret != 0)
		pr_err("Failed to register WM8310 SPI driver: %d\n", ret);

	ret = spi_register_driver(&wm8311_spi_driver);
	if (ret != 0)
		pr_err("Failed to register WM8311 SPI driver: %d\n", ret);

	ret = spi_register_driver(&wm8312_spi_driver);
	if (ret != 0)
		pr_err("Failed to register WM8312 SPI driver: %d\n", ret);

	ret = spi_register_driver(&wm8320_spi_driver);
	if (ret != 0)
		pr_err("Failed to register WM8320 SPI driver: %d\n", ret);

	ret = spi_register_driver(&wm8321_spi_driver);
	if (ret != 0)
		pr_err("Failed to register WM8321 SPI driver: %d\n", ret);

	ret = spi_register_driver(&wm8325_spi_driver);
	if (ret != 0)
		pr_err("Failed to register WM8325 SPI driver: %d\n", ret);

	ret = spi_register_driver(&wm8326_spi_driver);
	if (ret != 0)
		pr_err("Failed to register WM8326 SPI driver: %d\n", ret);
=======
	ret = spi_register_driver(&wm831x_spi_driver);
	if (ret != 0)
		pr_err("Failed to register WM831x SPI driver: %d\n", ret);
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
}
subsys_initcall(wm831x_spi_init);

static void __exit wm831x_spi_exit(void)
{
<<<<<<< HEAD
	spi_unregister_driver(&wm8326_spi_driver);
	spi_unregister_driver(&wm8325_spi_driver);
	spi_unregister_driver(&wm8321_spi_driver);
	spi_unregister_driver(&wm8320_spi_driver);
	spi_unregister_driver(&wm8312_spi_driver);
	spi_unregister_driver(&wm8311_spi_driver);
	spi_unregister_driver(&wm8310_spi_driver);
=======
	spi_unregister_driver(&wm831x_spi_driver);
>>>>>>> refs/remotes/origin/cm-10.0
}
module_exit(wm831x_spi_exit);

MODULE_DESCRIPTION("SPI support for WM831x/2x AudioPlus PMICs");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Brown");
