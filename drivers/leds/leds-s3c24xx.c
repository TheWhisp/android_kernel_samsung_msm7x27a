/* drivers/leds/leds-s3c24xx.c
 *
 * (c) 2006 Simtec Electronics
 *	http://armlinux.simtec.co.uk/
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * S3C24XX - LEDs GPIO driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/leds.h>
#include <linux/gpio.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0

#include <mach/hardware.h>
#include <mach/regs-gpio.h>
#include <mach/leds-gpio.h>
=======
#include <linux/module.h>

#include <mach/hardware.h>
#include <mach/regs-gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/platform_data/leds-s3c24xx.h>
>>>>>>> refs/remotes/origin/master

/* our context */

struct s3c24xx_gpio_led {
	struct led_classdev		 cdev;
	struct s3c24xx_led_platdata	*pdata;
};

static inline struct s3c24xx_gpio_led *pdev_to_gpio(struct platform_device *dev)
{
	return platform_get_drvdata(dev);
}

static inline struct s3c24xx_gpio_led *to_gpio(struct led_classdev *led_cdev)
{
	return container_of(led_cdev, struct s3c24xx_gpio_led, cdev);
}

static void s3c24xx_led_set(struct led_classdev *led_cdev,
			    enum led_brightness value)
{
	struct s3c24xx_gpio_led *led = to_gpio(led_cdev);
	struct s3c24xx_led_platdata *pd = led->pdata;
<<<<<<< HEAD
=======
	int state = (value ? 1 : 0) ^ (pd->flags & S3C24XX_LEDF_ACTLOW);
>>>>>>> refs/remotes/origin/master

	/* there will be a short delay between setting the output and
	 * going from output to input when using tristate. */

<<<<<<< HEAD
	s3c2410_gpio_setpin(pd->gpio, (value ? 1 : 0) ^
			    (pd->flags & S3C24XX_LEDF_ACTLOW));

	if (pd->flags & S3C24XX_LEDF_TRISTATE)
		s3c2410_gpio_cfgpin(pd->gpio,
			value ? S3C2410_GPIO_OUTPUT : S3C2410_GPIO_INPUT);

=======
	gpio_set_value(pd->gpio, state);

	if (pd->flags & S3C24XX_LEDF_TRISTATE) {
		if (value)
			gpio_direction_output(pd->gpio, state);
		else
			gpio_direction_input(pd->gpio);
	}
>>>>>>> refs/remotes/origin/master
}

static int s3c24xx_led_remove(struct platform_device *dev)
{
	struct s3c24xx_gpio_led *led = pdev_to_gpio(dev);

	led_classdev_unregister(&led->cdev);
<<<<<<< HEAD
	kfree(led);
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int s3c24xx_led_probe(struct platform_device *dev)
{
<<<<<<< HEAD
	struct s3c24xx_led_platdata *pdata = dev->dev.platform_data;
	struct s3c24xx_gpio_led *led;
	int ret;

	led = kzalloc(sizeof(struct s3c24xx_gpio_led), GFP_KERNEL);
=======
	struct s3c24xx_led_platdata *pdata = dev_get_platdata(&dev->dev);
	struct s3c24xx_gpio_led *led;
	int ret;

	led = devm_kzalloc(&dev->dev, sizeof(struct s3c24xx_gpio_led),
			   GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (led == NULL) {
		dev_err(&dev->dev, "No memory for device\n");
		return -ENOMEM;
	}

	platform_set_drvdata(dev, led);

	led->cdev.brightness_set = s3c24xx_led_set;
	led->cdev.default_trigger = pdata->def_trigger;
	led->cdev.name = pdata->name;
	led->cdev.flags |= LED_CORE_SUSPENDRESUME;

	led->pdata = pdata;

<<<<<<< HEAD
	/* no point in having a pull-up if we are always driving */

	if (pdata->flags & S3C24XX_LEDF_TRISTATE) {
		s3c2410_gpio_setpin(pdata->gpio, 0);
		s3c2410_gpio_cfgpin(pdata->gpio, S3C2410_GPIO_INPUT);
	} else {
		s3c2410_gpio_pullup(pdata->gpio, 0);
		s3c2410_gpio_setpin(pdata->gpio, 0);
		s3c2410_gpio_cfgpin(pdata->gpio, S3C2410_GPIO_OUTPUT);
	}
=======
	ret = devm_gpio_request(&dev->dev, pdata->gpio, "S3C24XX_LED");
	if (ret < 0)
		return ret;

	/* no point in having a pull-up if we are always driving */

	s3c_gpio_setpull(pdata->gpio, S3C_GPIO_PULL_NONE);

	if (pdata->flags & S3C24XX_LEDF_TRISTATE)
		gpio_direction_input(pdata->gpio);
	else
		gpio_direction_output(pdata->gpio,
			pdata->flags & S3C24XX_LEDF_ACTLOW ? 1 : 0);
>>>>>>> refs/remotes/origin/master

	/* register our new led device */

	ret = led_classdev_register(&dev->dev, &led->cdev);
<<<<<<< HEAD
	if (ret < 0) {
		dev_err(&dev->dev, "led_classdev_register failed\n");
		kfree(led);
		return ret;
	}

	return 0;
=======
	if (ret < 0)
		dev_err(&dev->dev, "led_classdev_register failed\n");

	return ret;
>>>>>>> refs/remotes/origin/master
}

static struct platform_driver s3c24xx_led_driver = {
	.probe		= s3c24xx_led_probe,
	.remove		= s3c24xx_led_remove,
	.driver		= {
		.name		= "s3c24xx_led",
		.owner		= THIS_MODULE,
	},
};

<<<<<<< HEAD
<<<<<<< HEAD
static int __init s3c24xx_led_init(void)
{
	return platform_driver_register(&s3c24xx_led_driver);
}

static void __exit s3c24xx_led_exit(void)
{
	platform_driver_unregister(&s3c24xx_led_driver);
}

module_init(s3c24xx_led_init);
module_exit(s3c24xx_led_exit);
=======
module_platform_driver(s3c24xx_led_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
module_platform_driver(s3c24xx_led_driver);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Ben Dooks <ben@simtec.co.uk>");
MODULE_DESCRIPTION("S3C24XX LED driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:s3c24xx_led");
