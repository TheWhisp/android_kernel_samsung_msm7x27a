/* drivers/video/backlight/platform_lcd.c
 *
 * Copyright 2008 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * Generic platform-device LCD power control interface.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/fb.h>
#include <linux/backlight.h>
#include <linux/lcd.h>
<<<<<<< HEAD
=======
#include <linux/of.h>
>>>>>>> refs/remotes/origin/master
#include <linux/slab.h>

#include <video/platform_lcd.h>

struct platform_lcd {
	struct device		*us;
	struct lcd_device	*lcd;
	struct plat_lcd_data	*pdata;

	unsigned int		 power;
<<<<<<< HEAD
	unsigned int		 suspended : 1;
=======
	unsigned int		 suspended:1;
>>>>>>> refs/remotes/origin/master
};

static inline struct platform_lcd *to_our_lcd(struct lcd_device *lcd)
{
	return lcd_get_data(lcd);
}

static int platform_lcd_get_power(struct lcd_device *lcd)
{
	struct platform_lcd *plcd = to_our_lcd(lcd);

	return plcd->power;
}

static int platform_lcd_set_power(struct lcd_device *lcd, int power)
{
	struct platform_lcd *plcd = to_our_lcd(lcd);
	int lcd_power = 1;

	if (power == FB_BLANK_POWERDOWN || plcd->suspended)
		lcd_power = 0;

	plcd->pdata->set_power(plcd->pdata, lcd_power);
	plcd->power = power;

	return 0;
}

static int platform_lcd_match(struct lcd_device *lcd, struct fb_info *info)
{
	struct platform_lcd *plcd = to_our_lcd(lcd);
	struct plat_lcd_data *pdata = plcd->pdata;

	if (pdata->match_fb)
		return pdata->match_fb(pdata, info);

	return plcd->us->parent == info->device;
}

static struct lcd_ops platform_lcd_ops = {
	.get_power	= platform_lcd_get_power,
	.set_power	= platform_lcd_set_power,
	.check_fb	= platform_lcd_match,
};

<<<<<<< HEAD
static int __devinit platform_lcd_probe(struct platform_device *pdev)
=======
static int platform_lcd_probe(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct plat_lcd_data *pdata;
	struct platform_lcd *plcd;
	struct device *dev = &pdev->dev;
	int err;

<<<<<<< HEAD
	pdata = pdev->dev.platform_data;
=======
	pdata = dev_get_platdata(&pdev->dev);
>>>>>>> refs/remotes/origin/master
	if (!pdata) {
		dev_err(dev, "no platform data supplied\n");
		return -EINVAL;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	plcd = kzalloc(sizeof(struct platform_lcd), GFP_KERNEL);
=======
	plcd = devm_kzalloc(&pdev->dev, sizeof(struct platform_lcd),
			    GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (pdata->probe) {
		err = pdata->probe(pdata);
		if (err)
			return err;
	}

	plcd = devm_kzalloc(&pdev->dev, sizeof(struct platform_lcd),
			    GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!plcd) {
		dev_err(dev, "no memory for state\n");
		return -ENOMEM;
	}

	plcd->us = dev;
	plcd->pdata = pdata;
<<<<<<< HEAD
	plcd->lcd = lcd_device_register(dev_name(dev), dev,
					plcd, &platform_lcd_ops);
	if (IS_ERR(plcd->lcd)) {
		dev_err(dev, "cannot register lcd device\n");
		err = PTR_ERR(plcd->lcd);
<<<<<<< HEAD
		goto err_mem;
=======
		goto err;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	plcd->lcd = devm_lcd_device_register(&pdev->dev, dev_name(dev), dev,
						plcd, &platform_lcd_ops);
	if (IS_ERR(plcd->lcd)) {
		dev_err(dev, "cannot register lcd device\n");
		return PTR_ERR(plcd->lcd);
>>>>>>> refs/remotes/origin/master
	}

	platform_set_drvdata(pdev, plcd);
	platform_lcd_set_power(plcd->lcd, FB_BLANK_NORMAL);

	return 0;
<<<<<<< HEAD

<<<<<<< HEAD
 err_mem:
	kfree(plcd);
=======
 err:
>>>>>>> refs/remotes/origin/cm-10.0
	return err;
}

static int __devexit platform_lcd_remove(struct platform_device *pdev)
{
	struct platform_lcd *plcd = platform_get_drvdata(pdev);

	lcd_device_unregister(plcd->lcd);
<<<<<<< HEAD
	kfree(plcd);
=======
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
}

#ifdef CONFIG_PM
<<<<<<< HEAD
static int platform_lcd_suspend(struct platform_device *pdev, pm_message_t st)
{
	struct platform_lcd *plcd = platform_get_drvdata(pdev);
=======
static int platform_lcd_suspend(struct device *dev)
{
	struct platform_lcd *plcd = dev_get_drvdata(dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
}

#ifdef CONFIG_PM_SLEEP
static int platform_lcd_suspend(struct device *dev)
{
	struct platform_lcd *plcd = dev_get_drvdata(dev);
>>>>>>> refs/remotes/origin/master

	plcd->suspended = 1;
	platform_lcd_set_power(plcd->lcd, plcd->power);

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int platform_lcd_resume(struct platform_device *pdev)
{
	struct platform_lcd *plcd = platform_get_drvdata(pdev);
=======
static int platform_lcd_resume(struct device *dev)
{
	struct platform_lcd *plcd = dev_get_drvdata(dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int platform_lcd_resume(struct device *dev)
{
	struct platform_lcd *plcd = dev_get_drvdata(dev);
>>>>>>> refs/remotes/origin/master

	plcd->suspended = 0;
	platform_lcd_set_power(plcd->lcd, plcd->power);

	return 0;
}
<<<<<<< HEAD
<<<<<<< HEAD
#else
#define platform_lcd_suspend NULL
#define platform_lcd_resume NULL
=======

static SIMPLE_DEV_PM_OPS(platform_lcd_pm_ops, platform_lcd_suspend,
			platform_lcd_resume);
>>>>>>> refs/remotes/origin/cm-10.0
=======
#endif

static SIMPLE_DEV_PM_OPS(platform_lcd_pm_ops, platform_lcd_suspend,
			platform_lcd_resume);

#ifdef CONFIG_OF
static const struct of_device_id platform_lcd_of_match[] = {
	{ .compatible = "platform-lcd" },
	{},
};
MODULE_DEVICE_TABLE(of, platform_lcd_of_match);
>>>>>>> refs/remotes/origin/master
#endif

static struct platform_driver platform_lcd_driver = {
	.driver		= {
		.name	= "platform-lcd",
		.owner	= THIS_MODULE,
<<<<<<< HEAD
<<<<<<< HEAD
	},
	.probe		= platform_lcd_probe,
	.remove		= __devexit_p(platform_lcd_remove),
	.suspend        = platform_lcd_suspend,
	.resume         = platform_lcd_resume,
};

static int __init platform_lcd_init(void)
{
	return platform_driver_register(&platform_lcd_driver);
}

static void __exit platform_lcd_cleanup(void)
{
	platform_driver_unregister(&platform_lcd_driver);
}

module_init(platform_lcd_init);
module_exit(platform_lcd_cleanup);
=======
#ifdef CONFIG_PM
		.pm	= &platform_lcd_pm_ops,
#endif
	},
	.probe		= platform_lcd_probe,
	.remove		= __devexit_p(platform_lcd_remove),
};

module_platform_driver(platform_lcd_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		.pm	= &platform_lcd_pm_ops,
		.of_match_table = of_match_ptr(platform_lcd_of_match),
	},
	.probe		= platform_lcd_probe,
};

module_platform_driver(platform_lcd_driver);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Ben Dooks <ben-linux@fluff.org>");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:platform-lcd");
