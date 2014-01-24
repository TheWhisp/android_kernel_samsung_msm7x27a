/*
 * LEDs driver for the "User LED" on Routerboard532
 *
 * Copyright (C) 2009 Phil Sutter <n0-1@freewrt.org>
 *
 * Based on leds-cobalt-qube.c by Florian Fainelly and
 * rb-diag.c (my own standalone driver for both LED and
 * button of Routerboard532).
 */

#include <linux/leds.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#include <asm/mach-rc32434/gpio.h>
#include <asm/mach-rc32434/rb.h>

static void rb532_led_set(struct led_classdev *cdev,
<<<<<<< HEAD
                          enum led_brightness brightness)
=======
			  enum led_brightness brightness)
>>>>>>> refs/remotes/origin/master
{
	if (brightness)
		set_latch_u5(LO_ULED, 0);

	else
		set_latch_u5(0, LO_ULED);
}

static enum led_brightness rb532_led_get(struct led_classdev *cdev)
{
	return (get_latch_u5() & LO_ULED) ? LED_FULL : LED_OFF;
}

static struct led_classdev rb532_uled = {
	.name = "uled",
	.brightness_set = rb532_led_set,
	.brightness_get = rb532_led_get,
	.default_trigger = "nand-disk",
};

<<<<<<< HEAD
static int __devinit rb532_led_probe(struct platform_device *pdev)
=======
static int rb532_led_probe(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	return led_classdev_register(&pdev->dev, &rb532_uled);
}

<<<<<<< HEAD
static int __devexit rb532_led_remove(struct platform_device *pdev)
=======
static int rb532_led_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	led_classdev_unregister(&rb532_uled);
	return 0;
}

static struct platform_driver rb532_led_driver = {
	.probe = rb532_led_probe,
<<<<<<< HEAD
	.remove = __devexit_p(rb532_led_remove),
=======
	.remove = rb532_led_remove,
>>>>>>> refs/remotes/origin/master
	.driver = {
		.name = "rb532-led",
		.owner = THIS_MODULE,
	},
};

<<<<<<< HEAD
<<<<<<< HEAD
static int __init rb532_led_init(void)
{
	return platform_driver_register(&rb532_led_driver);
}

static void __exit rb532_led_exit(void)
{
	platform_driver_unregister(&rb532_led_driver);
}

module_init(rb532_led_init);
module_exit(rb532_led_exit);

MODULE_ALIAS("platform:rb532-led");
=======
module_platform_driver(rb532_led_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
module_platform_driver(rb532_led_driver);
>>>>>>> refs/remotes/origin/master

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("User LED support for Routerboard532");
MODULE_AUTHOR("Phil Sutter <n0-1@freewrt.org>");
<<<<<<< HEAD
<<<<<<< HEAD
=======
MODULE_ALIAS("platform:rb532-led");
>>>>>>> refs/remotes/origin/cm-10.0
=======
MODULE_ALIAS("platform:rb532-led");
>>>>>>> refs/remotes/origin/master
