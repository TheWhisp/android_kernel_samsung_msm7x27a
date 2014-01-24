/*
 * pm.c - Common OMAP2+ power management-related code
 *
 * Copyright (C) 2010 Texas Instruments, Inc.
 * Copyright (C) 2010 Nokia Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/err.h>
#include <linux/opp.h>
<<<<<<< HEAD

#include <plat/omap-pm.h>
#include <plat/omap_device.h>
#include <plat/common.h>

=======
#include <linux/export.h>
#include <linux/suspend.h>

#include <asm/system_misc.h>

#include <plat/omap-pm.h>
#include <plat/omap_device.h>
#include "common.h"

#include "prcm-common.h"
>>>>>>> refs/remotes/origin/cm-10.0
#include "voltage.h"
#include "powerdomain.h"
#include "clockdomain.h"
#include "pm.h"
<<<<<<< HEAD

static struct omap_device_pm_latency *pm_lats;

static struct device *mpu_dev;
static struct device *iva_dev;
static struct device *l3_dev;
static struct device *dsp_dev;

struct device *omap2_get_mpuss_device(void)
{
	WARN_ON_ONCE(!mpu_dev);
	return mpu_dev;
}

struct device *omap2_get_iva_device(void)
{
	WARN_ON_ONCE(!iva_dev);
	return iva_dev;
}

struct device *omap2_get_l3_device(void)
{
	WARN_ON_ONCE(!l3_dev);
	return l3_dev;
}

struct device *omap4_get_dsp_device(void)
{
	WARN_ON_ONCE(!dsp_dev);
	return dsp_dev;
}
EXPORT_SYMBOL(omap4_get_dsp_device);

/* static int _init_omap_device(struct omap_hwmod *oh, void *user) */
static int _init_omap_device(char *name, struct device **new_dev)
{
	struct omap_hwmod *oh;
	struct omap_device *od;
=======
#include "twl-common.h"

static struct omap_device_pm_latency *pm_lats;

/*
 * omap_pm_suspend: points to a function that does the SoC-specific
 * suspend work
 */
int (*omap_pm_suspend)(void);

static int __init _init_omap_device(char *name)
{
	struct omap_hwmod *oh;
	struct platform_device *pdev;
>>>>>>> refs/remotes/origin/cm-10.0

	oh = omap_hwmod_lookup(name);
	if (WARN(!oh, "%s: could not find omap_hwmod for %s\n",
		 __func__, name))
		return -ENODEV;

<<<<<<< HEAD
	od = omap_device_build(oh->name, 0, oh, NULL, 0, pm_lats, 0, false);
	if (WARN(IS_ERR(od), "%s: could not build omap_device for %s\n",
		 __func__, name))
		return -ENODEV;

	*new_dev = &od->pdev.dev;

=======
	pdev = omap_device_build(oh->name, 0, oh, NULL, 0, pm_lats, 0, false);
	if (WARN(IS_ERR(pdev), "%s: could not build omap_device for %s\n",
		 __func__, name))
		return -ENODEV;

>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

/*
 * Build omap_devices for processors and bus.
 */
<<<<<<< HEAD
static void omap2_init_processor_devices(void)
{
	_init_omap_device("mpu", &mpu_dev);
	if (omap3_has_iva())
		_init_omap_device("iva", &iva_dev);

	if (cpu_is_omap44xx()) {
		_init_omap_device("l3_main_1", &l3_dev);
		_init_omap_device("dsp", &dsp_dev);
		_init_omap_device("iva", &iva_dev);
	} else {
		_init_omap_device("l3_main", &l3_dev);
=======
static void __init omap2_init_processor_devices(void)
{
	_init_omap_device("mpu");
	if (omap3_has_iva())
		_init_omap_device("iva");

	if (cpu_is_omap44xx()) {
		_init_omap_device("l3_main_1");
		_init_omap_device("dsp");
		_init_omap_device("iva");
	} else {
		_init_omap_device("l3_main");
>>>>>>> refs/remotes/origin/cm-10.0
	}
}

/* Types of sleep_switch used in omap_set_pwrdm_state */
#define FORCEWAKEUP_SWITCH	0
#define LOWPOWERSTATE_SWITCH	1

<<<<<<< HEAD
=======
int __init omap_pm_clkdms_setup(struct clockdomain *clkdm, void *unused)
{
	if (clkdm->flags & CLKDM_CAN_ENABLE_AUTO)
		clkdm_allow_idle(clkdm);
	else if (clkdm->flags & CLKDM_CAN_FORCE_SLEEP &&
		 atomic_read(&clkdm->usecount) == 0)
		clkdm_sleep(clkdm);
	return 0;
}

>>>>>>> refs/remotes/origin/cm-10.0
/*
 * This sets pwrdm state (other than mpu & core. Currently only ON &
 * RET are supported.
 */
<<<<<<< HEAD
int omap_set_pwrdm_state(struct powerdomain *pwrdm, u32 state)
{
	u32 cur_state;
	int sleep_switch = 0;
	int ret = 0;

	if (pwrdm == NULL || IS_ERR(pwrdm))
		return -EINVAL;

	while (!(pwrdm->pwrsts & (1 << state))) {
		if (state == PWRDM_POWER_OFF)
			return ret;
		state--;
	}

	cur_state = pwrdm_read_next_pwrst(pwrdm);
	if (cur_state == state)
		return ret;

	if (pwrdm_read_pwrst(pwrdm) < PWRDM_POWER_ON) {
		if ((pwrdm_read_pwrst(pwrdm) > state) &&
			(pwrdm->flags & PWRDM_HAS_LOWPOWERSTATECHANGE)) {
			sleep_switch = LOWPOWERSTATE_SWITCH;
		} else {
			clkdm_wakeup(pwrdm->pwrdm_clkdms[0]);
			pwrdm_wait_transition(pwrdm);
=======
int omap_set_pwrdm_state(struct powerdomain *pwrdm, u32 pwrst)
{
	u8 curr_pwrst, next_pwrst;
	int sleep_switch = -1, ret = 0, hwsup = 0;

	if (!pwrdm || IS_ERR(pwrdm))
		return -EINVAL;

	while (!(pwrdm->pwrsts & (1 << pwrst))) {
		if (pwrst == PWRDM_POWER_OFF)
			return ret;
		pwrst--;
	}

	next_pwrst = pwrdm_read_next_pwrst(pwrdm);
	if (next_pwrst == pwrst)
		return ret;

	curr_pwrst = pwrdm_read_pwrst(pwrdm);
	if (curr_pwrst < PWRDM_POWER_ON) {
		if ((curr_pwrst > pwrst) &&
			(pwrdm->flags & PWRDM_HAS_LOWPOWERSTATECHANGE)) {
			sleep_switch = LOWPOWERSTATE_SWITCH;
		} else {
			hwsup = clkdm_in_hwsup(pwrdm->pwrdm_clkdms[0]);
			clkdm_wakeup(pwrdm->pwrdm_clkdms[0]);
>>>>>>> refs/remotes/origin/cm-10.0
			sleep_switch = FORCEWAKEUP_SWITCH;
		}
	}

<<<<<<< HEAD
	ret = pwrdm_set_next_pwrst(pwrdm, state);
	if (ret) {
		printk(KERN_ERR "Unable to set state of powerdomain: %s\n",
		       pwrdm->name);
		goto err;
	}

	switch (sleep_switch) {
	case FORCEWAKEUP_SWITCH:
		if (pwrdm->pwrdm_clkdms[0]->flags & CLKDM_CAN_ENABLE_AUTO)
=======
	ret = pwrdm_set_next_pwrst(pwrdm, pwrst);
	if (ret)
		pr_err("%s: unable to set power state of powerdomain: %s\n",
		       __func__, pwrdm->name);

	switch (sleep_switch) {
	case FORCEWAKEUP_SWITCH:
		if (hwsup)
>>>>>>> refs/remotes/origin/cm-10.0
			clkdm_allow_idle(pwrdm->pwrdm_clkdms[0]);
		else
			clkdm_sleep(pwrdm->pwrdm_clkdms[0]);
		break;
	case LOWPOWERSTATE_SWITCH:
		pwrdm_set_lowpwrstchange(pwrdm);
<<<<<<< HEAD
		break;
	default:
		return ret;
	}

	pwrdm_wait_transition(pwrdm);
	pwrdm_state_switch(pwrdm);
err:
	return ret;
}

/*
 * This API is to be called during init to put the various voltage
 * domains to the voltage as per the opp table. Typically we boot up
 * at the nominal voltage. So this function finds out the rate of
 * the clock associated with the voltage domain, finds out the correct
 * opp entry and puts the voltage domain to the voltage specifies
 * in the opp entry
 */
static int __init omap2_set_init_voltage(char *vdd_name, char *clk_name,
						struct device *dev)
=======
		pwrdm_wait_transition(pwrdm);
		pwrdm_state_switch(pwrdm);
		break;
	}

	return ret;
}



/*
 * This API is to be called during init to set the various voltage
 * domains to the voltage as per the opp table. Typically we boot up
 * at the nominal voltage. So this function finds out the rate of
 * the clock associated with the voltage domain, finds out the correct
 * opp entry and sets the voltage domain to the voltage specified
 * in the opp entry
 */
static int __init omap2_set_init_voltage(char *vdd_name, char *clk_name,
					 const char *oh_name)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct voltagedomain *voltdm;
	struct clk *clk;
	struct opp *opp;
	unsigned long freq, bootup_volt;
<<<<<<< HEAD

	if (!vdd_name || !clk_name || !dev) {
		printk(KERN_ERR "%s: Invalid parameters!\n", __func__);
		goto exit;
	}

	voltdm = omap_voltage_domain_lookup(vdd_name);
	if (IS_ERR(voltdm)) {
		printk(KERN_ERR "%s: Unable to get vdd pointer for vdd_%s\n",
=======
	struct device *dev;

	if (!vdd_name || !clk_name || !oh_name) {
		pr_err("%s: invalid parameters\n", __func__);
		goto exit;
	}

	dev = omap_device_get_by_hwmod_name(oh_name);
	if (IS_ERR(dev)) {
		pr_err("%s: Unable to get dev pointer for hwmod %s\n",
			__func__, oh_name);
		goto exit;
	}

	voltdm = voltdm_lookup(vdd_name);
	if (IS_ERR(voltdm)) {
		pr_err("%s: unable to get vdd pointer for vdd_%s\n",
>>>>>>> refs/remotes/origin/cm-10.0
			__func__, vdd_name);
		goto exit;
	}

	clk =  clk_get(NULL, clk_name);
	if (IS_ERR(clk)) {
<<<<<<< HEAD
		printk(KERN_ERR "%s: unable to get clk %s\n",
			__func__, clk_name);
=======
		pr_err("%s: unable to get clk %s\n", __func__, clk_name);
>>>>>>> refs/remotes/origin/cm-10.0
		goto exit;
	}

	freq = clk->rate;
	clk_put(clk);

<<<<<<< HEAD
	opp = opp_find_freq_ceil(dev, &freq);
	if (IS_ERR(opp)) {
		printk(KERN_ERR "%s: unable to find boot up OPP for vdd_%s\n",
=======
	rcu_read_lock();
	opp = opp_find_freq_ceil(dev, &freq);
	if (IS_ERR(opp)) {
		rcu_read_unlock();
		pr_err("%s: unable to find boot up OPP for vdd_%s\n",
>>>>>>> refs/remotes/origin/cm-10.0
			__func__, vdd_name);
		goto exit;
	}

	bootup_volt = opp_get_voltage(opp);
<<<<<<< HEAD
	if (!bootup_volt) {
		printk(KERN_ERR "%s: unable to find voltage corresponding"
=======
	rcu_read_unlock();
	if (!bootup_volt) {
		pr_err("%s: unable to find voltage corresponding "
>>>>>>> refs/remotes/origin/cm-10.0
			"to the bootup OPP for vdd_%s\n", __func__, vdd_name);
		goto exit;
	}

<<<<<<< HEAD
	omap_voltage_scale_vdd(voltdm, bootup_volt);
	return 0;

exit:
	printk(KERN_ERR "%s: Unable to put vdd_%s to its init voltage\n\n",
		__func__, vdd_name);
	return -EINVAL;
}

=======
	voltdm_scale(voltdm, bootup_volt);
	return 0;

exit:
	pr_err("%s: unable to set vdd_%s\n", __func__, vdd_name);
	return -EINVAL;
}

#ifdef CONFIG_SUSPEND
static int omap_pm_enter(suspend_state_t suspend_state)
{
	int ret = 0;

	if (!omap_pm_suspend)
		return -ENOENT; /* XXX doublecheck */

	switch (suspend_state) {
	case PM_SUSPEND_STANDBY:
	case PM_SUSPEND_MEM:
		ret = omap_pm_suspend();
		break;
	default:
		ret = -EINVAL;
	}

	return ret;
}

static int omap_pm_begin(suspend_state_t state)
{
	disable_hlt();
	if (cpu_is_omap34xx())
		omap_prcm_irq_prepare();
	return 0;
}

static void omap_pm_end(void)
{
	enable_hlt();
	return;
}

static void omap_pm_finish(void)
{
	if (cpu_is_omap34xx())
		omap_prcm_irq_complete();
}

static const struct platform_suspend_ops omap_pm_ops = {
	.begin		= omap_pm_begin,
	.end		= omap_pm_end,
	.enter		= omap_pm_enter,
	.finish		= omap_pm_finish,
	.valid		= suspend_valid_only_mem,
};

#endif /* CONFIG_SUSPEND */

>>>>>>> refs/remotes/origin/cm-10.0
static void __init omap3_init_voltages(void)
{
	if (!cpu_is_omap34xx())
		return;

<<<<<<< HEAD
	omap2_set_init_voltage("mpu", "dpll1_ck", mpu_dev);
	omap2_set_init_voltage("core", "l3_ick", l3_dev);
=======
	omap2_set_init_voltage("mpu_iva", "dpll1_ck", "mpu");
	omap2_set_init_voltage("core", "l3_ick", "l3_main");
>>>>>>> refs/remotes/origin/cm-10.0
}

static void __init omap4_init_voltages(void)
{
	if (!cpu_is_omap44xx())
		return;

<<<<<<< HEAD
	omap2_set_init_voltage("mpu", "dpll_mpu_ck", mpu_dev);
	omap2_set_init_voltage("core", "l3_div_ck", l3_dev);
	omap2_set_init_voltage("iva", "dpll_iva_m5x2_ck", iva_dev);
=======
	omap2_set_init_voltage("mpu", "dpll_mpu_ck", "mpu");
	omap2_set_init_voltage("core", "l3_div_ck", "l3_main_1");
	omap2_set_init_voltage("iva", "dpll_iva_m5x2_ck", "iva");
>>>>>>> refs/remotes/origin/cm-10.0
}

static int __init omap2_common_pm_init(void)
{
<<<<<<< HEAD
	omap2_init_processor_devices();
=======
	if (!of_have_populated_dt())
		omap2_init_processor_devices();
>>>>>>> refs/remotes/origin/cm-10.0
	omap_pm_if_init();

	return 0;
}
postcore_initcall(omap2_common_pm_init);

static int __init omap2_common_pm_late_init(void)
{
<<<<<<< HEAD
	/* Init the OMAP TWL parameters */
	omap3_twl_init();
	omap4_twl_init();

	/* Init the voltage layer */
=======
	/*
	 * In the case of DT, the PMIC and SR initialization will be done using
	 * a completely different mechanism.
	 * Disable this part if a DT blob is available.
	 */
	if (of_have_populated_dt())
		return 0;

	/* Init the voltage layer */
	omap_pmic_late_init();
>>>>>>> refs/remotes/origin/cm-10.0
	omap_voltage_late_init();

	/* Initialize the voltages */
	omap3_init_voltages();
	omap4_init_voltages();

	/* Smartreflex device init */
	omap_devinit_smartreflex();

<<<<<<< HEAD
=======
#ifdef CONFIG_SUSPEND
	suspend_set_ops(&omap_pm_ops);
#endif

>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}
late_initcall(omap2_common_pm_late_init);
