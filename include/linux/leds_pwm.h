/*
 * PWM LED driver data - see drivers/leds/leds-pwm.c
 */
#ifndef __LINUX_LEDS_PWM_H
#define __LINUX_LEDS_PWM_H

struct led_pwm {
	const char	*name;
	const char	*default_trigger;
<<<<<<< HEAD
	unsigned	pwm_id;
=======
	unsigned	pwm_id __deprecated;
>>>>>>> refs/remotes/origin/master
	u8 		active_low;
	unsigned 	max_brightness;
	unsigned	pwm_period_ns;
};

struct led_pwm_platform_data {
	int			num_leds;
	struct led_pwm	*leds;
};

#endif
