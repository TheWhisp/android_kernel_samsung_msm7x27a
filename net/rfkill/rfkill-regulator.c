/*
 * rfkill-regulator.c - Regulator consumer driver for rfkill
 *
 * Copyright (C) 2009  Guiming Zhuo <gmzhuo@gmail.com>
 * Copyright (C) 2011  Antonio Ospite <ospite@studenti.unina.it>
 *
 * Implementation inspired by leds-regulator driver.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/module.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
#include <linux/rfkill.h>
#include <linux/rfkill-regulator.h>

struct rfkill_regulator_data {
	struct rfkill *rf_kill;
	bool reg_enabled;

	struct regulator *vcc;
};

static int rfkill_regulator_set_block(void *data, bool blocked)
{
	struct rfkill_regulator_data *rfkill_data = data;
<<<<<<< HEAD
=======
	int ret = 0;
>>>>>>> refs/remotes/origin/master

	pr_debug("%s: blocked: %d\n", __func__, blocked);

	if (blocked) {
		if (rfkill_data->reg_enabled) {
			regulator_disable(rfkill_data->vcc);
<<<<<<< HEAD
<<<<<<< HEAD
			rfkill_data->reg_enabled = 0;
=======
			rfkill_data->reg_enabled = false;
>>>>>>> refs/remotes/origin/cm-10.0
		}
	} else {
		if (!rfkill_data->reg_enabled) {
			regulator_enable(rfkill_data->vcc);
<<<<<<< HEAD
			rfkill_data->reg_enabled = 1;
=======
			rfkill_data->reg_enabled = true;
>>>>>>> refs/remotes/origin/cm-10.0
=======
			rfkill_data->reg_enabled = false;
		}
	} else {
		if (!rfkill_data->reg_enabled) {
			ret = regulator_enable(rfkill_data->vcc);
			if (!ret)
				rfkill_data->reg_enabled = true;
>>>>>>> refs/remotes/origin/master
		}
	}

	pr_debug("%s: regulator_is_enabled after set_block: %d\n", __func__,
		regulator_is_enabled(rfkill_data->vcc));

<<<<<<< HEAD
	return 0;
}

struct rfkill_ops rfkill_regulator_ops = {
	.set_block = rfkill_regulator_set_block,
};

static int __devinit rfkill_regulator_probe(struct platform_device *pdev)
=======
	return ret;
}

static struct rfkill_ops rfkill_regulator_ops = {
	.set_block = rfkill_regulator_set_block,
};

static int rfkill_regulator_probe(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct rfkill_regulator_platform_data *pdata = pdev->dev.platform_data;
	struct rfkill_regulator_data *rfkill_data;
	struct regulator *vcc;
	struct rfkill *rf_kill;
	int ret = 0;

	if (pdata == NULL) {
		dev_err(&pdev->dev, "no platform data\n");
		return -ENODEV;
	}

	if (pdata->name == NULL || pdata->type == 0) {
		dev_err(&pdev->dev, "invalid name or type in platform data\n");
		return -EINVAL;
	}

	vcc = regulator_get_exclusive(&pdev->dev, "vrfkill");
	if (IS_ERR(vcc)) {
		dev_err(&pdev->dev, "Cannot get vcc for %s\n", pdata->name);
		ret = PTR_ERR(vcc);
		goto out;
	}

	rfkill_data = kzalloc(sizeof(*rfkill_data), GFP_KERNEL);
	if (rfkill_data == NULL) {
		ret = -ENOMEM;
		goto err_data_alloc;
	}

	rf_kill = rfkill_alloc(pdata->name, &pdev->dev,
				pdata->type,
				&rfkill_regulator_ops, rfkill_data);
	if (rf_kill == NULL) {
<<<<<<< HEAD
<<<<<<< HEAD
		dev_err(&pdev->dev, "Cannot alloc rfkill device\n");
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		ret = -ENOMEM;
		goto err_rfkill_alloc;
	}

	if (regulator_is_enabled(vcc)) {
		dev_dbg(&pdev->dev, "Regulator already enabled\n");
<<<<<<< HEAD
<<<<<<< HEAD
		rfkill_data->reg_enabled = 1;
=======
		rfkill_data->reg_enabled = true;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		rfkill_data->reg_enabled = true;
>>>>>>> refs/remotes/origin/master
	}
	rfkill_data->vcc = vcc;
	rfkill_data->rf_kill = rf_kill;

	ret = rfkill_register(rf_kill);
	if (ret) {
		dev_err(&pdev->dev, "Cannot register rfkill device\n");
		goto err_rfkill_register;
	}

	platform_set_drvdata(pdev, rfkill_data);
	dev_info(&pdev->dev, "%s initialized\n", pdata->name);

	return 0;

err_rfkill_register:
	rfkill_destroy(rf_kill);
err_rfkill_alloc:
	kfree(rfkill_data);
err_data_alloc:
	regulator_put(vcc);
out:
	return ret;
}

<<<<<<< HEAD
static int __devexit rfkill_regulator_remove(struct platform_device *pdev)
=======
static int rfkill_regulator_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct rfkill_regulator_data *rfkill_data = platform_get_drvdata(pdev);
	struct rfkill *rf_kill = rfkill_data->rf_kill;

	rfkill_unregister(rf_kill);
	rfkill_destroy(rf_kill);
	regulator_put(rfkill_data->vcc);
	kfree(rfkill_data);

	return 0;
}

static struct platform_driver rfkill_regulator_driver = {
	.probe = rfkill_regulator_probe,
<<<<<<< HEAD
	.remove = __devexit_p(rfkill_regulator_remove),
=======
	.remove = rfkill_regulator_remove,
>>>>>>> refs/remotes/origin/master
	.driver = {
		.name = "rfkill-regulator",
		.owner = THIS_MODULE,
	},
};

<<<<<<< HEAD
<<<<<<< HEAD
static int __init rfkill_regulator_init(void)
{
	return platform_driver_register(&rfkill_regulator_driver);
}
module_init(rfkill_regulator_init);

static void __exit rfkill_regulator_exit(void)
{
	platform_driver_unregister(&rfkill_regulator_driver);
}
module_exit(rfkill_regulator_exit);
=======
module_platform_driver(rfkill_regulator_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
module_platform_driver(rfkill_regulator_driver);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Guiming Zhuo <gmzhuo@gmail.com>");
MODULE_AUTHOR("Antonio Ospite <ospite@studenti.unina.it>");
MODULE_DESCRIPTION("Regulator consumer driver for rfkill");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:rfkill-regulator");
