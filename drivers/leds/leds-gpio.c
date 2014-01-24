/*
 * LEDs driver for GPIOs
 *
 * Copyright (C) 2007 8D Technologies inc.
 * Raphael Assenat <raph@8d.com>
 * Copyright (C) 2008 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/gpio.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/leds.h>
=======
#include <linux/gpio.h>
#include <linux/leds.h>
#include <linux/of.h>
>>>>>>> refs/remotes/origin/master
#include <linux/of_platform.h>
#include <linux/of_gpio.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
<<<<<<< HEAD
<<<<<<< HEAD

#include <asm/gpio.h>
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/module.h>
#include <linux/err.h>
>>>>>>> refs/remotes/origin/master

struct gpio_led_data {
	struct led_classdev cdev;
	unsigned gpio;
	struct work_struct work;
	u8 new_level;
	u8 can_sleep;
	u8 active_low;
	u8 blinking;
	int (*platform_gpio_blink_set)(unsigned gpio, int state,
			unsigned long *delay_on, unsigned long *delay_off);
};

static void gpio_led_work(struct work_struct *work)
{
	struct gpio_led_data	*led_dat =
		container_of(work, struct gpio_led_data, work);

	if (led_dat->blinking) {
		led_dat->platform_gpio_blink_set(led_dat->gpio,
						 led_dat->new_level,
						 NULL, NULL);
		led_dat->blinking = 0;
	} else
		gpio_set_value_cansleep(led_dat->gpio, led_dat->new_level);
}

static void gpio_led_set(struct led_classdev *led_cdev,
	enum led_brightness value)
{
	struct gpio_led_data *led_dat =
		container_of(led_cdev, struct gpio_led_data, cdev);
	int level;

	if (value == LED_OFF)
		level = 0;
	else
		level = 1;

	if (led_dat->active_low)
		level = !level;

	/* Setting GPIOs with I2C/etc requires a task context, and we don't
	 * seem to have a reliable way to know if we're already in one; so
	 * let's just assume the worst.
	 */
	if (led_dat->can_sleep) {
		led_dat->new_level = level;
		schedule_work(&led_dat->work);
	} else {
		if (led_dat->blinking) {
			led_dat->platform_gpio_blink_set(led_dat->gpio, level,
							 NULL, NULL);
			led_dat->blinking = 0;
		} else
			gpio_set_value(led_dat->gpio, level);
	}
}

static int gpio_blink_set(struct led_classdev *led_cdev,
	unsigned long *delay_on, unsigned long *delay_off)
{
	struct gpio_led_data *led_dat =
		container_of(led_cdev, struct gpio_led_data, cdev);

	led_dat->blinking = 1;
	return led_dat->platform_gpio_blink_set(led_dat->gpio, GPIO_LED_BLINK,
						delay_on, delay_off);
}

<<<<<<< HEAD
static int __devinit create_gpio_led(const struct gpio_led *template,
=======
static int create_gpio_led(const struct gpio_led *template,
>>>>>>> refs/remotes/origin/master
	struct gpio_led_data *led_dat, struct device *parent,
	int (*blink_set)(unsigned, int, unsigned long *, unsigned long *))
{
	int ret, state;

	led_dat->gpio = -1;

	/* skip leds that aren't available */
	if (!gpio_is_valid(template->gpio)) {
<<<<<<< HEAD
		printk(KERN_INFO "Skipping unavailable LED gpio %d (%s)\n",
=======
		dev_info(parent, "Skipping unavailable LED gpio %d (%s)\n",
>>>>>>> refs/remotes/origin/master
				template->gpio, template->name);
		return 0;
	}

<<<<<<< HEAD
	ret = gpio_request(template->gpio, template->name);
=======
	ret = devm_gpio_request(parent, template->gpio, template->name);
>>>>>>> refs/remotes/origin/master
	if (ret < 0)
		return ret;

	led_dat->cdev.name = template->name;
	led_dat->cdev.default_trigger = template->default_trigger;
	led_dat->gpio = template->gpio;
	led_dat->can_sleep = gpio_cansleep(template->gpio);
	led_dat->active_low = template->active_low;
	led_dat->blinking = 0;
	if (blink_set) {
		led_dat->platform_gpio_blink_set = blink_set;
		led_dat->cdev.blink_set = gpio_blink_set;
	}
	led_dat->cdev.brightness_set = gpio_led_set;
	if (template->default_state == LEDS_GPIO_DEFSTATE_KEEP)
<<<<<<< HEAD
<<<<<<< HEAD
		state = !!gpio_get_value(led_dat->gpio) ^ led_dat->active_low;
=======
		state = !!gpio_get_value_cansleep(led_dat->gpio) ^ led_dat->active_low;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		state = !!gpio_get_value_cansleep(led_dat->gpio) ^ led_dat->active_low;
>>>>>>> refs/remotes/origin/master
	else
		state = (template->default_state == LEDS_GPIO_DEFSTATE_ON);
	led_dat->cdev.brightness = state ? LED_FULL : LED_OFF;
	if (!template->retain_state_suspended)
		led_dat->cdev.flags |= LED_CORE_SUSPENDRESUME;

	ret = gpio_direction_output(led_dat->gpio, led_dat->active_low ^ state);
	if (ret < 0)
<<<<<<< HEAD
		goto err;
		
=======
		return ret;

>>>>>>> refs/remotes/origin/master
	INIT_WORK(&led_dat->work, gpio_led_work);

	ret = led_classdev_register(parent, &led_dat->cdev);
	if (ret < 0)
<<<<<<< HEAD
		goto err;

	return 0;
err:
	gpio_free(led_dat->gpio);
	return ret;
=======
		return ret;

	return 0;
>>>>>>> refs/remotes/origin/master
}

static void delete_gpio_led(struct gpio_led_data *led)
{
	if (!gpio_is_valid(led->gpio))
		return;
	led_classdev_unregister(&led->cdev);
	cancel_work_sync(&led->work);
<<<<<<< HEAD
	gpio_free(led->gpio);
=======
>>>>>>> refs/remotes/origin/master
}

struct gpio_leds_priv {
	int num_leds;
	struct gpio_led_data leds[];
};

static inline int sizeof_gpio_leds_priv(int num_leds)
{
	return sizeof(struct gpio_leds_priv) +
		(sizeof(struct gpio_led_data) * num_leds);
}

/* Code to create from OpenFirmware platform devices */
<<<<<<< HEAD
<<<<<<< HEAD
#ifdef CONFIG_LEDS_GPIO_OF
=======
#ifdef CONFIG_OF_GPIO
>>>>>>> refs/remotes/origin/cm-10.0
static struct gpio_leds_priv * __devinit gpio_leds_create_of(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node, *child;
	struct gpio_leds_priv *priv;
	int count = 0, ret;

	/* count LEDs in this device, so we know how much to allocate */
	for_each_child_of_node(np, child)
		count++;
	if (!count)
		return NULL;

	priv = kzalloc(sizeof_gpio_leds_priv(count), GFP_KERNEL);
	if (!priv)
		return NULL;

	for_each_child_of_node(np, child) {
=======
#ifdef CONFIG_OF_GPIO
static struct gpio_leds_priv *gpio_leds_create_of(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node, *child;
	struct gpio_leds_priv *priv;
	int count, ret;

	/* count LEDs in this device, so we know how much to allocate */
	count = of_get_available_child_count(np);
	if (!count)
		return ERR_PTR(-ENODEV);

	for_each_available_child_of_node(np, child)
		if (of_get_gpio(child, 0) == -EPROBE_DEFER)
			return ERR_PTR(-EPROBE_DEFER);

	priv = devm_kzalloc(&pdev->dev, sizeof_gpio_leds_priv(count),
			GFP_KERNEL);
	if (!priv)
		return ERR_PTR(-ENOMEM);

	for_each_available_child_of_node(np, child) {
>>>>>>> refs/remotes/origin/master
		struct gpio_led led = {};
		enum of_gpio_flags flags;
		const char *state;

		led.gpio = of_get_gpio_flags(child, 0, &flags);
		led.active_low = flags & OF_GPIO_ACTIVE_LOW;
		led.name = of_get_property(child, "label", NULL) ? : child->name;
		led.default_trigger =
			of_get_property(child, "linux,default-trigger", NULL);
		state = of_get_property(child, "default-state", NULL);
		if (state) {
			if (!strcmp(state, "keep"))
				led.default_state = LEDS_GPIO_DEFSTATE_KEEP;
			else if (!strcmp(state, "on"))
				led.default_state = LEDS_GPIO_DEFSTATE_ON;
			else
				led.default_state = LEDS_GPIO_DEFSTATE_OFF;
		}

		ret = create_gpio_led(&led, &priv->leds[priv->num_leds++],
				      &pdev->dev, NULL);
		if (ret < 0) {
			of_node_put(child);
			goto err;
		}
	}

	return priv;

err:
	for (count = priv->num_leds - 2; count >= 0; count--)
		delete_gpio_led(&priv->leds[count]);
<<<<<<< HEAD
	kfree(priv);
	return NULL;
=======
	return ERR_PTR(-ENODEV);
>>>>>>> refs/remotes/origin/master
}

static const struct of_device_id of_gpio_leds_match[] = {
	{ .compatible = "gpio-leds", },
	{},
};
<<<<<<< HEAD
<<<<<<< HEAD
#else
=======
#else /* CONFIG_OF_GPIO */
>>>>>>> refs/remotes/origin/cm-10.0
static struct gpio_leds_priv * __devinit gpio_leds_create_of(struct platform_device *pdev)
{
	return NULL;
}
#define of_gpio_leds_match NULL
<<<<<<< HEAD
#endif
=======
#endif /* CONFIG_OF_GPIO */
>>>>>>> refs/remotes/origin/cm-10.0


static int __devinit gpio_led_probe(struct platform_device *pdev)
{
	struct gpio_led_platform_data *pdata = pdev->dev.platform_data;
	struct gpio_leds_priv *priv;
	int i, ret = 0;

	if (pdata && pdata->num_leds) {
		priv = kzalloc(sizeof_gpio_leds_priv(pdata->num_leds),
				GFP_KERNEL);
=======
#else /* CONFIG_OF_GPIO */
static struct gpio_leds_priv *gpio_leds_create_of(struct platform_device *pdev)
{
	return ERR_PTR(-ENODEV);
}
#endif /* CONFIG_OF_GPIO */


static int gpio_led_probe(struct platform_device *pdev)
{
	struct gpio_led_platform_data *pdata = dev_get_platdata(&pdev->dev);
	struct gpio_leds_priv *priv;
	int i, ret = 0;


	if (pdata && pdata->num_leds) {
		priv = devm_kzalloc(&pdev->dev,
				sizeof_gpio_leds_priv(pdata->num_leds),
					GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
		if (!priv)
			return -ENOMEM;

		priv->num_leds = pdata->num_leds;
		for (i = 0; i < priv->num_leds; i++) {
			ret = create_gpio_led(&pdata->leds[i],
					      &priv->leds[i],
					      &pdev->dev, pdata->gpio_blink_set);
			if (ret < 0) {
				/* On failure: unwind the led creations */
				for (i = i - 1; i >= 0; i--)
					delete_gpio_led(&priv->leds[i]);
<<<<<<< HEAD
				kfree(priv);
=======
>>>>>>> refs/remotes/origin/master
				return ret;
			}
		}
	} else {
		priv = gpio_leds_create_of(pdev);
<<<<<<< HEAD
		if (!priv)
			return -ENODEV;
=======
		if (IS_ERR(priv))
			return PTR_ERR(priv);
>>>>>>> refs/remotes/origin/master
	}

	platform_set_drvdata(pdev, priv);

	return 0;
}

<<<<<<< HEAD
static int __devexit gpio_led_remove(struct platform_device *pdev)
{
	struct gpio_leds_priv *priv = dev_get_drvdata(&pdev->dev);
=======
static int gpio_led_remove(struct platform_device *pdev)
{
	struct gpio_leds_priv *priv = platform_get_drvdata(pdev);
>>>>>>> refs/remotes/origin/master
	int i;

	for (i = 0; i < priv->num_leds; i++)
		delete_gpio_led(&priv->leds[i]);

<<<<<<< HEAD
	dev_set_drvdata(&pdev->dev, NULL);
	kfree(priv);

=======
>>>>>>> refs/remotes/origin/master
	return 0;
}

static struct platform_driver gpio_led_driver = {
	.probe		= gpio_led_probe,
<<<<<<< HEAD
	.remove		= __devexit_p(gpio_led_remove),
	.driver		= {
		.name	= "leds-gpio",
		.owner	= THIS_MODULE,
		.of_match_table = of_gpio_leds_match,
	},
};

<<<<<<< HEAD
MODULE_ALIAS("platform:leds-gpio");

static int __init gpio_led_init(void)
{
	return platform_driver_register(&gpio_led_driver);
}

static void __exit gpio_led_exit(void)
{
	platform_driver_unregister(&gpio_led_driver);
}

module_init(gpio_led_init);
module_exit(gpio_led_exit);
=======
module_platform_driver(gpio_led_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.remove		= gpio_led_remove,
	.driver		= {
		.name	= "leds-gpio",
		.owner	= THIS_MODULE,
		.of_match_table = of_match_ptr(of_gpio_leds_match),
	},
};

module_platform_driver(gpio_led_driver);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Raphael Assenat <raph@8d.com>, Trent Piepho <tpiepho@freescale.com>");
MODULE_DESCRIPTION("GPIO LED driver");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
<<<<<<< HEAD
=======
MODULE_ALIAS("platform:leds-gpio");
>>>>>>> refs/remotes/origin/cm-10.0
=======
MODULE_ALIAS("platform:leds-gpio");
>>>>>>> refs/remotes/origin/master
