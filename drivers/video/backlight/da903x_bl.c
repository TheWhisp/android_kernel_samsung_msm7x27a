/*
 * Backlight driver for Dialog Semiconductor DA9030/DA9034
 *
 * Copyright (C) 2008 Compulab, Ltd.
<<<<<<< HEAD
 * 	Mike Rapoport <mike@compulab.co.il>
 *
 * Copyright (C) 2006-2008 Marvell International Ltd.
 * 	Eric Miao <eric.miao@marvell.com>
=======
 *	Mike Rapoport <mike@compulab.co.il>
 *
 * Copyright (C) 2006-2008 Marvell International Ltd.
 *	Eric Miao <eric.miao@marvell.com>
>>>>>>> refs/remotes/origin/master
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/fb.h>
#include <linux/backlight.h>
#include <linux/mfd/da903x.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/master

#define DA9030_WLED_CONTROL	0x25
#define DA9030_WLED_CP_EN	(1 << 6)
#define DA9030_WLED_TRIM(x)	((x) & 0x7)

#define DA9034_WLED_CONTROL1	0x3C
#define DA9034_WLED_CONTROL2	0x3D
#define DA9034_WLED_ISET(x)	((x) & 0x1f)

#define DA9034_WLED_BOOST_EN	(1 << 5)

#define DA9030_MAX_BRIGHTNESS	7
#define DA9034_MAX_BRIGHTNESS	0x7f

struct da903x_backlight_data {
	struct device *da903x_dev;
	int id;
	int current_brightness;
};

static int da903x_backlight_set(struct backlight_device *bl, int brightness)
{
	struct da903x_backlight_data *data = bl_get_data(bl);
	struct device *dev = data->da903x_dev;
	uint8_t val;
	int ret = 0;

	switch (data->id) {
	case DA9034_ID_WLED:
		ret = da903x_update(dev, DA9034_WLED_CONTROL1,
				brightness, 0x7f);
		if (ret)
			return ret;

		if (data->current_brightness && brightness == 0)
			ret = da903x_clr_bits(dev,
					DA9034_WLED_CONTROL2,
					DA9034_WLED_BOOST_EN);

		if (data->current_brightness == 0 && brightness)
			ret = da903x_set_bits(dev,
					DA9034_WLED_CONTROL2,
					DA9034_WLED_BOOST_EN);
		break;
	case DA9030_ID_WLED:
		val = DA9030_WLED_TRIM(brightness);
		val |= brightness ? DA9030_WLED_CP_EN : 0;
		ret = da903x_write(dev, DA9030_WLED_CONTROL, val);
		break;
	}

	if (ret)
		return ret;

	data->current_brightness = brightness;
	return 0;
}

static int da903x_backlight_update_status(struct backlight_device *bl)
{
	int brightness = bl->props.brightness;

	if (bl->props.power != FB_BLANK_UNBLANK)
		brightness = 0;

	if (bl->props.fb_blank != FB_BLANK_UNBLANK)
		brightness = 0;

<<<<<<< HEAD
=======
	if (bl->props.state & BL_CORE_SUSPENDED)
		brightness = 0;

>>>>>>> refs/remotes/origin/master
	return da903x_backlight_set(bl, brightness);
}

static int da903x_backlight_get_brightness(struct backlight_device *bl)
{
	struct da903x_backlight_data *data = bl_get_data(bl);
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/master
	return data->current_brightness;
}

static const struct backlight_ops da903x_backlight_ops = {
<<<<<<< HEAD
=======
	.options	= BL_CORE_SUSPENDRESUME,
>>>>>>> refs/remotes/origin/master
	.update_status	= da903x_backlight_update_status,
	.get_brightness	= da903x_backlight_get_brightness,
};

static int da903x_backlight_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct da9034_backlight_pdata *pdata = pdev->dev.platform_data;
=======
	struct da9034_backlight_pdata *pdata = dev_get_platdata(&pdev->dev);
>>>>>>> refs/remotes/origin/master
	struct da903x_backlight_data *data;
	struct backlight_device *bl;
	struct backlight_properties props;
	int max_brightness;

<<<<<<< HEAD
<<<<<<< HEAD
	data = kzalloc(sizeof(*data), GFP_KERNEL);
=======
	data = devm_kzalloc(&pdev->dev, sizeof(*data), GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	data = devm_kzalloc(&pdev->dev, sizeof(*data), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (data == NULL)
		return -ENOMEM;

	switch (pdev->id) {
	case DA9030_ID_WLED:
		max_brightness = DA9030_MAX_BRIGHTNESS;
		break;
	case DA9034_ID_WLED:
		max_brightness = DA9034_MAX_BRIGHTNESS;
		break;
	default:
		dev_err(&pdev->dev, "invalid backlight device ID(%d)\n",
				pdev->id);
<<<<<<< HEAD
<<<<<<< HEAD
		kfree(data);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		return -EINVAL;
	}

	data->id = pdev->id;
	data->da903x_dev = pdev->dev.parent;
	data->current_brightness = 0;

	/* adjust the WLED output current */
	if (pdata)
		da903x_write(data->da903x_dev, DA9034_WLED_CONTROL2,
				DA9034_WLED_ISET(pdata->output_current));

<<<<<<< HEAD
	props.type = BACKLIGHT_RAW;
	props.max_brightness = max_brightness;
	bl = backlight_device_register(pdev->name, data->da903x_dev, data,
				       &da903x_backlight_ops, &props);
	if (IS_ERR(bl)) {
		dev_err(&pdev->dev, "failed to register backlight\n");
<<<<<<< HEAD
		kfree(data);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	memset(&props, 0, sizeof(props));
	props.type = BACKLIGHT_RAW;
	props.max_brightness = max_brightness;
	bl = devm_backlight_device_register(&pdev->dev, pdev->name,
					data->da903x_dev, data,
					&da903x_backlight_ops, &props);
	if (IS_ERR(bl)) {
		dev_err(&pdev->dev, "failed to register backlight\n");
>>>>>>> refs/remotes/origin/master
		return PTR_ERR(bl);
	}

	bl->props.brightness = max_brightness;

	platform_set_drvdata(pdev, bl);
	backlight_update_status(bl);
	return 0;
}

<<<<<<< HEAD
static int da903x_backlight_remove(struct platform_device *pdev)
{
	struct backlight_device *bl = platform_get_drvdata(pdev);
<<<<<<< HEAD
	struct da903x_backlight_data *data = bl_get_data(bl);

	backlight_device_unregister(bl);
	kfree(data);
=======

	backlight_device_unregister(bl);
>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

#ifdef CONFIG_PM
static int da903x_backlight_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct backlight_device *bl = platform_get_drvdata(pdev);
	return da903x_backlight_set(bl, 0);
}

static int da903x_backlight_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct backlight_device *bl = platform_get_drvdata(pdev);

	backlight_update_status(bl);
	return 0;
}

static const struct dev_pm_ops da903x_backlight_pm_ops = {
	.suspend	= da903x_backlight_suspend,
	.resume		= da903x_backlight_resume,
};
#endif

=======
>>>>>>> refs/remotes/origin/master
static struct platform_driver da903x_backlight_driver = {
	.driver		= {
		.name	= "da903x-backlight",
		.owner	= THIS_MODULE,
<<<<<<< HEAD
#ifdef CONFIG_PM
		.pm	= &da903x_backlight_pm_ops,
#endif
	},
	.probe		= da903x_backlight_probe,
	.remove		= da903x_backlight_remove,
};

<<<<<<< HEAD
static int __init da903x_backlight_init(void)
{
	return platform_driver_register(&da903x_backlight_driver);
}
module_init(da903x_backlight_init);

static void __exit da903x_backlight_exit(void)
{
	platform_driver_unregister(&da903x_backlight_driver);
}
module_exit(da903x_backlight_exit);
=======
module_platform_driver(da903x_backlight_driver);
>>>>>>> refs/remotes/origin/cm-10.0

MODULE_DESCRIPTION("Backlight Driver for Dialog Semiconductor DA9030/DA9034");
MODULE_AUTHOR("Eric Miao <eric.miao@marvell.com>"
	      "Mike Rapoport <mike@compulab.co.il>");
=======
	},
	.probe		= da903x_backlight_probe,
};

module_platform_driver(da903x_backlight_driver);

MODULE_DESCRIPTION("Backlight Driver for Dialog Semiconductor DA9030/DA9034");
MODULE_AUTHOR("Eric Miao <eric.miao@marvell.com>");
MODULE_AUTHOR("Mike Rapoport <mike@compulab.co.il>");
>>>>>>> refs/remotes/origin/master
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:da903x-backlight");
