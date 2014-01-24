#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/leds.h>
#include <linux/io.h>
#include <linux/atmel_pwm.h>
#include <linux/slab.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/master


struct pwmled {
	struct led_classdev	cdev;
	struct pwm_channel	pwmc;
	struct gpio_led		*desc;
	u32			mult;
	u8			active_low;
};


/*
 * For simplicity, we use "brightness" as if it were a linear function
 * of PWM duty cycle.  However, a logarithmic function of duty cycle is
 * probably a better match for perceived brightness: two is half as bright
 * as four, four is half as bright as eight, etc
 */
static void pwmled_brightness(struct led_classdev *cdev, enum led_brightness b)
{
	struct pwmled		 *led;

	/* update the duty cycle for the *next* period */
	led = container_of(cdev, struct pwmled, cdev);
	pwm_channel_writel(&led->pwmc, PWM_CUPD, led->mult * (unsigned) b);
}

/*
 * NOTE:  we reuse the platform_data structure of GPIO leds,
 * but repurpose its "gpio" number as a PWM channel number.
 */
<<<<<<< HEAD
<<<<<<< HEAD
static int __init pwmled_probe(struct platform_device *pdev)
=======
static int __devinit pwmled_probe(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int pwmled_probe(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	const struct gpio_led_platform_data	*pdata;
	struct pwmled				*leds;
	int					i;
	int					status;

<<<<<<< HEAD
	pdata = pdev->dev.platform_data;
	if (!pdata || pdata->num_leds < 1)
		return -ENODEV;

	leds = kcalloc(pdata->num_leds, sizeof(*leds), GFP_KERNEL);
=======
	pdata = dev_get_platdata(&pdev->dev);
	if (!pdata || pdata->num_leds < 1)
		return -ENODEV;

	leds = devm_kzalloc(&pdev->dev, pdata->num_leds * sizeof(*leds),
			GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!leds)
		return -ENOMEM;

	for (i = 0; i < pdata->num_leds; i++) {
		struct pwmled		*led = leds + i;
		const struct gpio_led	*dat = pdata->leds + i;
		u32			tmp;

		led->cdev.name = dat->name;
		led->cdev.brightness = LED_OFF;
		led->cdev.brightness_set = pwmled_brightness;
		led->cdev.default_trigger = dat->default_trigger;

		led->active_low = dat->active_low;

		status = pwm_channel_alloc(dat->gpio, &led->pwmc);
		if (status < 0)
			goto err;

		/*
		 * Prescale clock by 2^x, so PWM counts in low MHz.
		 * Start each cycle with the LED active, so increasing
		 * the duty cycle gives us more time on (== brighter).
		 */
		tmp = 5;
		if (!led->active_low)
			tmp |= PWM_CPR_CPOL;
		pwm_channel_writel(&led->pwmc, PWM_CMR, tmp);

		/*
		 * Pick a period so PWM cycles at 100+ Hz; and a multiplier
		 * for scaling duty cycle:  brightness * mult.
		 */
		tmp = (led->pwmc.mck / (1 << 5)) / 100;
		tmp /= 255;
		led->mult = tmp;
		pwm_channel_writel(&led->pwmc, PWM_CDTY,
				led->cdev.brightness * 255);
		pwm_channel_writel(&led->pwmc, PWM_CPRD,
				LED_FULL * tmp);

		pwm_channel_enable(&led->pwmc);

		/* Hand it over to the LED framework */
		status = led_classdev_register(&pdev->dev, &led->cdev);
		if (status < 0) {
			pwm_channel_free(&led->pwmc);
			goto err;
		}
	}

	platform_set_drvdata(pdev, leds);
	return 0;

err:
	if (i > 0) {
		for (i = i - 1; i >= 0; i--) {
			led_classdev_unregister(&leds[i].cdev);
			pwm_channel_free(&leds[i].pwmc);
		}
	}
<<<<<<< HEAD
	kfree(leds);
=======
>>>>>>> refs/remotes/origin/master

	return status;
}

<<<<<<< HEAD
static int __exit pwmled_remove(struct platform_device *pdev)
=======
static int pwmled_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	const struct gpio_led_platform_data	*pdata;
	struct pwmled				*leds;
	unsigned				i;

<<<<<<< HEAD
	pdata = pdev->dev.platform_data;
=======
	pdata = dev_get_platdata(&pdev->dev);
>>>>>>> refs/remotes/origin/master
	leds = platform_get_drvdata(pdev);

	for (i = 0; i < pdata->num_leds; i++) {
		struct pwmled		*led = leds + i;

		led_classdev_unregister(&led->cdev);
		pwm_channel_free(&led->pwmc);
	}

<<<<<<< HEAD
	kfree(leds);
	platform_set_drvdata(pdev, NULL);
	return 0;
}

<<<<<<< HEAD
/* work with hotplug and coldplug */
MODULE_ALIAS("platform:leds-atmel-pwm");

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return 0;
}

>>>>>>> refs/remotes/origin/master
static struct platform_driver pwmled_driver = {
	.driver = {
		.name =		"leds-atmel-pwm",
		.owner =	THIS_MODULE,
	},
	/* REVISIT add suspend() and resume() methods */
<<<<<<< HEAD
<<<<<<< HEAD
	.remove =	__exit_p(pwmled_remove),
};

static int __init modinit(void)
{
	return platform_driver_probe(&pwmled_driver, pwmled_probe);
}
module_init(modinit);

static void __exit modexit(void)
{
	platform_driver_unregister(&pwmled_driver);
}
module_exit(modexit);

MODULE_DESCRIPTION("Driver for LEDs with PWM-controlled brightness");
MODULE_LICENSE("GPL");
=======
	.probe =	pwmled_probe,
	.remove =	__exit_p(pwmled_remove),
=======
	.probe =	pwmled_probe,
	.remove =	pwmled_remove,
>>>>>>> refs/remotes/origin/master
};

module_platform_driver(pwmled_driver);

MODULE_DESCRIPTION("Driver for LEDs with PWM-controlled brightness");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:leds-atmel-pwm");
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
