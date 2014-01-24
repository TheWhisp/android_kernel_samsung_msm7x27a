/*
 * Backlight driver for OMAP based boards.
 *
 * Copyright (c) 2006 Andrzej Zaborowski  <balrog@zabor.org>
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this package; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/fb.h>
#include <linux/backlight.h>
#include <linux/slab.h>
<<<<<<< HEAD

#include <mach/hardware.h>
#include <plat/board.h>
#include <plat/mux.h>
=======
#include <linux/platform_data/omap1_bl.h>

#include <mach/hardware.h>
#include <mach/mux.h>
>>>>>>> refs/remotes/origin/master

#define OMAPBL_MAX_INTENSITY		0xff

struct omap_backlight {
	int powermode;
	int current_intensity;

	struct device *dev;
	struct omap_backlight_config *pdata;
};

<<<<<<< HEAD
static void inline omapbl_send_intensity(int intensity)
=======
static inline void omapbl_send_intensity(int intensity)
>>>>>>> refs/remotes/origin/master
{
	omap_writeb(intensity, OMAP_PWL_ENABLE);
}

<<<<<<< HEAD
static void inline omapbl_send_enable(int enable)
=======
static inline void omapbl_send_enable(int enable)
>>>>>>> refs/remotes/origin/master
{
	omap_writeb(enable, OMAP_PWL_CLK_ENABLE);
}

static void omapbl_blank(struct omap_backlight *bl, int mode)
{
	if (bl->pdata->set_power)
		bl->pdata->set_power(bl->dev, mode);

	switch (mode) {
	case FB_BLANK_NORMAL:
	case FB_BLANK_VSYNC_SUSPEND:
	case FB_BLANK_HSYNC_SUSPEND:
	case FB_BLANK_POWERDOWN:
		omapbl_send_intensity(0);
		omapbl_send_enable(0);
		break;

	case FB_BLANK_UNBLANK:
		omapbl_send_intensity(bl->current_intensity);
		omapbl_send_enable(1);
		break;
	}
}

<<<<<<< HEAD
#ifdef CONFIG_PM
static int omapbl_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct backlight_device *dev = platform_get_drvdata(pdev);
	struct omap_backlight *bl = dev_get_drvdata(&dev->dev);
=======
#ifdef CONFIG_PM_SLEEP
static int omapbl_suspend(struct device *dev)
{
	struct backlight_device *bl_dev = dev_get_drvdata(dev);
	struct omap_backlight *bl = bl_get_data(bl_dev);
>>>>>>> refs/remotes/origin/master

	omapbl_blank(bl, FB_BLANK_POWERDOWN);
	return 0;
}

<<<<<<< HEAD
static int omapbl_resume(struct platform_device *pdev)
{
	struct backlight_device *dev = platform_get_drvdata(pdev);
	struct omap_backlight *bl = dev_get_drvdata(&dev->dev);
=======
static int omapbl_resume(struct device *dev)
{
	struct backlight_device *bl_dev = dev_get_drvdata(dev);
	struct omap_backlight *bl = bl_get_data(bl_dev);
>>>>>>> refs/remotes/origin/master

	omapbl_blank(bl, bl->powermode);
	return 0;
}
<<<<<<< HEAD
#else
#define omapbl_suspend	NULL
#define omapbl_resume	NULL
=======
>>>>>>> refs/remotes/origin/master
#endif

static int omapbl_set_power(struct backlight_device *dev, int state)
{
<<<<<<< HEAD
	struct omap_backlight *bl = dev_get_drvdata(&dev->dev);
=======
	struct omap_backlight *bl = bl_get_data(dev);
>>>>>>> refs/remotes/origin/master

	omapbl_blank(bl, state);
	bl->powermode = state;

	return 0;
}

static int omapbl_update_status(struct backlight_device *dev)
{
<<<<<<< HEAD
	struct omap_backlight *bl = dev_get_drvdata(&dev->dev);
=======
	struct omap_backlight *bl = bl_get_data(dev);
>>>>>>> refs/remotes/origin/master

	if (bl->current_intensity != dev->props.brightness) {
		if (bl->powermode == FB_BLANK_UNBLANK)
			omapbl_send_intensity(dev->props.brightness);
		bl->current_intensity = dev->props.brightness;
	}

	if (dev->props.fb_blank != bl->powermode)
		omapbl_set_power(dev, dev->props.fb_blank);

	return 0;
}

static int omapbl_get_intensity(struct backlight_device *dev)
{
<<<<<<< HEAD
	struct omap_backlight *bl = dev_get_drvdata(&dev->dev);
=======
	struct omap_backlight *bl = bl_get_data(dev);
>>>>>>> refs/remotes/origin/master
	return bl->current_intensity;
}

static const struct backlight_ops omapbl_ops = {
	.get_brightness = omapbl_get_intensity,
	.update_status  = omapbl_update_status,
};

static int omapbl_probe(struct platform_device *pdev)
{
	struct backlight_properties props;
	struct backlight_device *dev;
	struct omap_backlight *bl;
<<<<<<< HEAD
	struct omap_backlight_config *pdata = pdev->dev.platform_data;
=======
	struct omap_backlight_config *pdata = dev_get_platdata(&pdev->dev);
>>>>>>> refs/remotes/origin/master

	if (!pdata)
		return -ENXIO;

<<<<<<< HEAD
<<<<<<< HEAD
	bl = kzalloc(sizeof(struct omap_backlight), GFP_KERNEL);
=======
	bl = devm_kzalloc(&pdev->dev, sizeof(struct omap_backlight),
			  GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bl = devm_kzalloc(&pdev->dev, sizeof(struct omap_backlight),
			  GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (unlikely(!bl))
		return -ENOMEM;

	memset(&props, 0, sizeof(struct backlight_properties));
	props.type = BACKLIGHT_RAW;
	props.max_brightness = OMAPBL_MAX_INTENSITY;
	dev = backlight_device_register("omap-bl", &pdev->dev, bl, &omapbl_ops,
					&props);
<<<<<<< HEAD
<<<<<<< HEAD
	if (IS_ERR(dev)) {
		kfree(bl);
		return PTR_ERR(dev);
	}
=======
	if (IS_ERR(dev))
		return PTR_ERR(dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (IS_ERR(dev))
		return PTR_ERR(dev);
>>>>>>> refs/remotes/origin/master

	bl->powermode = FB_BLANK_POWERDOWN;
	bl->current_intensity = 0;

	bl->pdata = pdata;
	bl->dev = &pdev->dev;

	platform_set_drvdata(pdev, dev);

	omap_cfg_reg(PWL);	/* Conflicts with UART3 */

	dev->props.fb_blank = FB_BLANK_UNBLANK;
	dev->props.brightness = pdata->default_intensity;
	omapbl_update_status(dev);

<<<<<<< HEAD
	printk(KERN_INFO "OMAP LCD backlight initialised\n");
=======
	dev_info(&pdev->dev, "OMAP LCD backlight initialised\n");
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int omapbl_remove(struct platform_device *pdev)
{
	struct backlight_device *dev = platform_get_drvdata(pdev);
<<<<<<< HEAD
<<<<<<< HEAD
	struct omap_backlight *bl = dev_get_drvdata(&dev->dev);

	backlight_device_unregister(dev);
	kfree(bl);
=======

	backlight_device_unregister(dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======

	backlight_device_unregister(dev);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
static struct platform_driver omapbl_driver = {
	.probe		= omapbl_probe,
	.remove		= omapbl_remove,
	.suspend	= omapbl_suspend,
	.resume		= omapbl_resume,
	.driver		= {
		.name	= "omap-bl",
	},
};

<<<<<<< HEAD
static int __init omapbl_init(void)
{
	return platform_driver_register(&omapbl_driver);
}

static void __exit omapbl_exit(void)
{
	platform_driver_unregister(&omapbl_driver);
}

module_init(omapbl_init);
module_exit(omapbl_exit);
=======
module_platform_driver(omapbl_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static SIMPLE_DEV_PM_OPS(omapbl_pm_ops, omapbl_suspend, omapbl_resume);

static struct platform_driver omapbl_driver = {
	.probe		= omapbl_probe,
	.remove		= omapbl_remove,
	.driver		= {
		.name	= "omap-bl",
		.pm	= &omapbl_pm_ops,
	},
};

module_platform_driver(omapbl_driver);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Andrzej Zaborowski <balrog@zabor.org>");
MODULE_DESCRIPTION("OMAP LCD Backlight driver");
MODULE_LICENSE("GPL");
