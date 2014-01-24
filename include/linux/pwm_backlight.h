/*
 * Generic PWM backlight driver data - see drivers/video/backlight/pwm_bl.c
 */
#ifndef __LINUX_PWM_BACKLIGHT_H
#define __LINUX_PWM_BACKLIGHT_H

#include <linux/backlight.h>

<<<<<<< HEAD
=======
/* TODO: convert to gpiod_*() API once it has been merged */
#define PWM_BACKLIGHT_GPIO_ACTIVE_LOW	(1 << 0)

>>>>>>> refs/remotes/origin/master
struct platform_pwm_backlight_data {
	int pwm_id;
	unsigned int max_brightness;
	unsigned int dft_brightness;
	unsigned int lth_brightness;
	unsigned int pwm_period_ns;
<<<<<<< HEAD
	int (*init)(struct device *dev);
	int (*notify)(struct device *dev, int brightness);
<<<<<<< HEAD
=======
	void (*notify_after)(struct device *dev, int brightness);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	unsigned int *levels;
	int enable_gpio;
	unsigned long enable_gpio_flags;
	int (*init)(struct device *dev);
	int (*notify)(struct device *dev, int brightness);
	void (*notify_after)(struct device *dev, int brightness);
>>>>>>> refs/remotes/origin/master
	void (*exit)(struct device *dev);
	int (*check_fb)(struct device *dev, struct fb_info *info);
};

#endif
