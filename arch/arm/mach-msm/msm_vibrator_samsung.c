/* include/asm/mach-msm/htc_pwrsink.h
 *
 * Copyright (C) 2008 HTC Corporation.
 * Copyright (C) 2007 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/hrtimer.h>
#include <linux/clk.h>
#include <../../../drivers/staging/android/timed_output.h>
#include <linux/sched.h>

#include <mach/msm_rpcrouter.h>
#include <mach/samsung_vibe.h>
#include <mach/gpio.h>
#include <linux/regulator/consumer.h>
#include <linux/delay.h>
<<<<<<< HEAD
<<<<<<< HEAD

=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-11.0

struct clk *android_vib_clk; /* core_clk */

#define CORE_CLK_M_DEFAULT			21
#define CORE_CLK_N_DEFAULT			18000
#define CORE_CLK_D_DEFAULT			9000	/* 50% duty cycle */ 
#define IMM_PWM_MULTIPLIER		    17778	/* Must be integer */

/*
 * Global variables for LRA PWM M,N and D values.
 */
VibeInt32 g_nLRA_CORE_CLK_M = CORE_CLK_M_DEFAULT;
VibeInt32 g_nLRA_CORE_CLK_N = CORE_CLK_N_DEFAULT;
VibeInt32 g_nLRA_CORE_CLK_D = CORE_CLK_N_DEFAULT;
VibeInt32 g_nLRA_CORE_CLK_PWM_MUL = IMM_PWM_MULTIPLIER;

static struct hrtimer vibe_timer;
static int enabled = 0;


static int msm_vibrator_suspend(struct platform_device *pdev, pm_message_t state);
static int msm_vibrator_resume(struct platform_device *pdev);
static int msm_vibrator_probe(struct platform_device *pdev);
static int msm_vibrator_exit(struct platform_device *pdev);
static int msm_vibrator_power(int power_mode);


/* Variable for setting PWM in Force Out Set */
VibeInt32 g_nForce_32 = 0;

/*
 * This function is used to set and re-set the CORE_CLK M and N counters
 * to output the desired target frequency.
 * 
 */

/* For the suspend/resume VIBRATOR Module */
static struct platform_driver msm_vibrator_platdriver = 
{
	.probe   = msm_vibrator_probe,
	.suspend = msm_vibrator_suspend,
	.resume  = msm_vibrator_resume,
	.remove  = __devexit_p(msm_vibrator_exit),
	.driver = 
	{
		.name = MODULE_NAME,
		.owner = THIS_MODULE,
	},
};

static int msm_vibrator_suspend(struct platform_device *pdev, pm_message_t state)
{
	printk("[VIB] suspend\n");
	return VIBE_S_SUCCESS;
}

static int msm_vibrator_resume(struct platform_device *pdev)
{
	printk("[VIB] resume\n");
	return VIBE_S_SUCCESS;
}

static int __devexit msm_vibrator_exit(struct platform_device *pdev)
{
	printk("[VIB] EXIT\n");
	return 0;
}

static int msm_vibrator_power(int on)
{
	struct regulator *regulator_msm_vibrator;
	int ret;

	regulator_msm_vibrator = regulator_get(NULL, "vreg_vib");

	if (on) {
		if(!enabled) {
			enabled = 1;

			ret = regulator_set_voltage(regulator_msm_vibrator, 3000000, 3000000);
			if (ret) {
				printk(KERN_ERR "%s: regulator (enable vib) set voltage failed (%d)\n",
						__func__, ret);
				return -EIO;
			}

			ret = regulator_enable(regulator_msm_vibrator);
			if (ret) {
				printk(KERN_ERR "%s: regulator enable failed (%d)\n",
						__func__, ret);
				return -EIO;
			}
			mdelay(15);
		}
	} else {
		if(enabled) {
			enabled = 0;

			ret = regulator_set_voltage(regulator_msm_vibrator, 3000000, 3000000);
			if (ret) {
				printk(KERN_ERR "%s: regulator (disable vib) set voltage failed (%d)\n",
					__func__, ret);
				return -EIO;
			}

			ret = regulator_disable(regulator_msm_vibrator);
			if (ret) {
				printk(KERN_ERR "%s: regulator disable failed (%d)\n",
								__func__,ret);
				return -EIO;
			}
		}
	}

	return VIBE_S_SUCCESS;
}

static void vibrator_enable(struct timed_output_dev *dev, int value)
{
	hrtimer_cancel(&vibe_timer);

	if (value == 0) {
		msm_vibrator_power(VIBRATION_OFF);
	} else {
		if(value < 0) {
			value = ~value;
		}

		value = (value > 15000 ? 15000 : value);
		msm_vibrator_power(VIBRATION_ON);
		hrtimer_start(&vibe_timer,
			      ktime_set(value / 1000, (value % 1000) * 1000000),
			      HRTIMER_MODE_REL);
	}
}

static int vibrator_get_time(struct timed_output_dev *dev)
{
	if (hrtimer_active(&vibe_timer)) {
		ktime_t r = hrtimer_get_remaining(&vibe_timer);
		return ktime_to_ms(r);
	} else
		return 0;
}

static int vibrator_timer_do_work(struct work_struct *ignored)
{
	unsigned int remain;

	if (hrtimer_active(&vibe_timer)) {
		ktime_t r = hrtimer_get_remaining(&vibe_timer);
		remain = ktime_to_ms(r);
		remain = remain / 1000;
		if (ktime_to_ms(r) < 0) {
			remain = 0;
		}
		if (!remain)
			msm_vibrator_power(VIBRATION_OFF);
	}
	else {
		msm_vibrator_power(VIBRATION_OFF);
	}

	return 0;
}

static DECLARE_WORK(vibrator_timer_work, vibrator_timer_do_work);

static enum hrtimer_restart vibrator_timer_func(struct hrtimer *timer)
{
	schedule_work(&vibrator_timer_work);

	return HRTIMER_NORESTART;
}

static struct timed_output_dev pmic_vibrator = {
	.name = "vibrator",
	.get_time = vibrator_get_time,
	.enable = vibrator_enable,
};

static int __devinit msm_vibrator_probe(struct platform_device *pdev)
{
	hrtimer_init(&vibe_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	vibe_timer.function = vibrator_timer_func;
	timed_output_dev_register(&pmic_vibrator);
	return 0;
}

static int __init msm_init_pmic_vibrator(void)
{
	int nRet;
	nRet = platform_driver_register(&msm_vibrator_platdriver);

	printk("[VIB] platform driver register result : %d\n",nRet);
	if (nRet)
	{ 
		printk("[VIB] platform_driver_register failed\n");
	}

	return nRet;
}

static void __exit msm_exit_pmic_vibrator(void)
{
	platform_driver_unregister(&msm_vibrator_platdriver);
}

module_init(msm_init_pmic_vibrator);
module_exit(msm_exit_pmic_vibrator);

MODULE_DESCRIPTION("Samsung vibrator device");
MODULE_LICENSE("GPL");
