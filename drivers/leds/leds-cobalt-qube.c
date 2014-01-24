/*
 * Copyright 2006 - Florian Fainelli <florian@openwrt.org>
 *
 * Control the Cobalt Qube/RaQ front LED
 */
#include <linux/init.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/leds.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/types.h>

#define LED_FRONT_LEFT	0x01
#define LED_FRONT_RIGHT	0x02

static void __iomem *led_port;
static u8 led_value;

static void qube_front_led_set(struct led_classdev *led_cdev,
			       enum led_brightness brightness)
{
	if (brightness)
		led_value = LED_FRONT_LEFT | LED_FRONT_RIGHT;
	else
		led_value = ~(LED_FRONT_LEFT | LED_FRONT_RIGHT);
	writeb(led_value, led_port);
}

static struct led_classdev qube_front_led = {
	.name			= "qube::front",
	.brightness		= LED_FULL,
	.brightness_set		= qube_front_led_set,
	.default_trigger	= "default-on",
};

<<<<<<< HEAD
static int __devinit cobalt_qube_led_probe(struct platform_device *pdev)
=======
static int cobalt_qube_led_probe(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct resource *res;
	int retval;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
		return -EBUSY;

<<<<<<< HEAD
	led_port = ioremap(res->start, resource_size(res));
=======
	led_port = devm_ioremap(&pdev->dev, res->start, resource_size(res));
>>>>>>> refs/remotes/origin/master
	if (!led_port)
		return -ENOMEM;

	led_value = LED_FRONT_LEFT | LED_FRONT_RIGHT;
	writeb(led_value, led_port);

	retval = led_classdev_register(&pdev->dev, &qube_front_led);
	if (retval)
<<<<<<< HEAD
		goto err_iounmap;

	return 0;

err_iounmap:
	iounmap(led_port);
=======
		goto err_null;

	return 0;

err_null:
>>>>>>> refs/remotes/origin/master
	led_port = NULL;

	return retval;
}

<<<<<<< HEAD
static int __devexit cobalt_qube_led_remove(struct platform_device *pdev)
{
	led_classdev_unregister(&qube_front_led);

	if (led_port) {
		iounmap(led_port);
		led_port = NULL;
	}
=======
static int cobalt_qube_led_remove(struct platform_device *pdev)
{
	led_classdev_unregister(&qube_front_led);

	if (led_port)
		led_port = NULL;
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
/* work with hotplug and coldplug */
MODULE_ALIAS("platform:cobalt-qube-leds");

=======
>>>>>>> refs/remotes/origin/cm-10.0
static struct platform_driver cobalt_qube_led_driver = {
	.probe	= cobalt_qube_led_probe,
	.remove	= __devexit_p(cobalt_qube_led_remove),
=======
static struct platform_driver cobalt_qube_led_driver = {
	.probe	= cobalt_qube_led_probe,
	.remove	= cobalt_qube_led_remove,
>>>>>>> refs/remotes/origin/master
	.driver	= {
		.name	= "cobalt-qube-leds",
		.owner	= THIS_MODULE,
	},
};

<<<<<<< HEAD
<<<<<<< HEAD
static int __init cobalt_qube_led_init(void)
{
	return platform_driver_register(&cobalt_qube_led_driver);
}

static void __exit cobalt_qube_led_exit(void)
{
	platform_driver_unregister(&cobalt_qube_led_driver);
}

module_init(cobalt_qube_led_init);
module_exit(cobalt_qube_led_exit);
=======
module_platform_driver(cobalt_qube_led_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
module_platform_driver(cobalt_qube_led_driver);
>>>>>>> refs/remotes/origin/master

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Front LED support for Cobalt Server");
MODULE_AUTHOR("Florian Fainelli <florian@openwrt.org>");
<<<<<<< HEAD
<<<<<<< HEAD
=======
MODULE_ALIAS("platform:cobalt-qube-leds");
>>>>>>> refs/remotes/origin/cm-10.0
=======
MODULE_ALIAS("platform:cobalt-qube-leds");
>>>>>>> refs/remotes/origin/master
