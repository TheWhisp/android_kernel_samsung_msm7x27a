/*
 *  linux/arch/arm/mach-omap2/clock.c
 *
 *  Copyright (C) 2005-2008 Texas Instruments, Inc.
 *  Copyright (C) 2004-2010 Nokia Corporation
 *
 *  Contacts:
 *  Richard Woodruff <r-woodruff2@ti.com>
 *  Paul Walmsley
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#undef DEBUG

#include <linux/kernel.h>
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/delay.h>
<<<<<<< HEAD
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/bitops.h>
#include <trace/events/power.h>

#include <asm/cpu.h>
#include <plat/clock.h>
#include "clockdomain.h"
#include <plat/cpu.h>
#include <plat/prcm.h>

#include "clock.h"
#include "cm2xxx_3xxx.h"
#include "cm-regbits-24xx.h"
#include "cm-regbits-34xx.h"

<<<<<<< HEAD
u8 cpu_mask;
=======
=======
#include <linux/clk-provider.h>
#include <linux/io.h>
#include <linux/bitops.h>
#include <linux/clk-private.h>
#include <asm/cpu.h>


#include <trace/events/power.h>

#include "soc.h"
#include "clockdomain.h"
#include "clock.h"
#include "cm.h"
#include "cm2xxx.h"
#include "cm3xxx.h"
#include "cm-regbits-24xx.h"
#include "cm-regbits-34xx.h"
#include "common.h"

/*
 * MAX_MODULE_ENABLE_WAIT: maximum of number of microseconds to wait
 * for a module to indicate that it is no longer in idle
 */
#define MAX_MODULE_ENABLE_WAIT		100000

>>>>>>> refs/remotes/origin/master
u16 cpu_mask;

/*
 * clkdm_control: if true, then when a clock is enabled in the
 * hardware, its clockdomain will first be enabled; and when a clock
 * is disabled in the hardware, its clockdomain will be disabled
 * afterwards.
 */
static bool clkdm_control = true;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======

static LIST_HEAD(clk_hw_omap_clocks);

/*
 * Used for clocks that have the same value as the parent clock,
 * divided by some factor
 */
unsigned long omap_fixed_divisor_recalc(struct clk_hw *hw,
		unsigned long parent_rate)
{
	struct clk_hw_omap *oclk;

	if (!hw) {
		pr_warn("%s: hw is NULL\n", __func__);
		return -EINVAL;
	}

	oclk = to_clk_hw_omap(hw);

	WARN_ON(!oclk->fixed_div);

	return parent_rate / oclk->fixed_div;
}
>>>>>>> refs/remotes/origin/master

/*
 * OMAP2+ specific clock functions
 */

/* Private functions */

<<<<<<< HEAD
=======

/**
 * _wait_idlest_generic - wait for a module to leave the idle state
 * @reg: virtual address of module IDLEST register
 * @mask: value to mask against to determine if the module is active
 * @idlest: idle state indicator (0 or 1) for the clock
 * @name: name of the clock (for printk)
 *
 * Wait for a module to leave idle, where its idle-status register is
 * not inside the CM module.  Returns 1 if the module left idle
 * promptly, or 0 if the module did not leave idle before the timeout
 * elapsed.  XXX Deprecated - should be moved into drivers for the
 * individual IP block that the IDLEST register exists in.
 */
static int _wait_idlest_generic(void __iomem *reg, u32 mask, u8 idlest,
				const char *name)
{
	int i = 0, ena = 0;

	ena = (idlest) ? 0 : mask;

	omap_test_timeout(((__raw_readl(reg) & mask) == ena),
			  MAX_MODULE_ENABLE_WAIT, i);

	if (i < MAX_MODULE_ENABLE_WAIT)
		pr_debug("omap clock: module associated with clock %s ready after %d loops\n",
			 name, i);
	else
		pr_err("omap clock: module associated with clock %s didn't enable in %d tries\n",
		       name, MAX_MODULE_ENABLE_WAIT);

	return (i < MAX_MODULE_ENABLE_WAIT) ? 1 : 0;
};

>>>>>>> refs/remotes/origin/master
/**
 * _omap2_module_wait_ready - wait for an OMAP module to leave IDLE
 * @clk: struct clk * belonging to the module
 *
 * If the necessary clocks for the OMAP hardware IP block that
 * corresponds to clock @clk are enabled, then wait for the module to
 * indicate readiness (i.e., to leave IDLE).  This code does not
 * belong in the clock code and will be moved in the medium term to
 * module-dependent code.  No return value.
 */
<<<<<<< HEAD
static void _omap2_module_wait_ready(struct clk *clk)
{
	void __iomem *companion_reg, *idlest_reg;
	u8 other_bit, idlest_bit, idlest_val;
=======
static void _omap2_module_wait_ready(struct clk_hw_omap *clk)
{
	void __iomem *companion_reg, *idlest_reg;
	u8 other_bit, idlest_bit, idlest_val, idlest_reg_id;
	s16 prcm_mod;
	int r;
>>>>>>> refs/remotes/origin/master

	/* Not all modules have multiple clocks that their IDLEST depends on */
	if (clk->ops->find_companion) {
		clk->ops->find_companion(clk, &companion_reg, &other_bit);
		if (!(__raw_readl(companion_reg) & (1 << other_bit)))
			return;
	}

	clk->ops->find_idlest(clk, &idlest_reg, &idlest_bit, &idlest_val);
<<<<<<< HEAD

	omap2_cm_wait_idlest(idlest_reg, (1 << idlest_bit), idlest_val,
			     clk->name);
=======
	r = cm_split_idlest_reg(idlest_reg, &prcm_mod, &idlest_reg_id);
	if (r) {
		/* IDLEST register not in the CM module */
		_wait_idlest_generic(idlest_reg, (1 << idlest_bit), idlest_val,
				     __clk_get_name(clk->hw.clk));
	} else {
		cm_wait_module_ready(prcm_mod, idlest_reg_id, idlest_bit);
	};
>>>>>>> refs/remotes/origin/master
}

/* Public functions */

/**
 * omap2_init_clk_clkdm - look up a clockdomain name, store pointer in clk
 * @clk: OMAP clock struct ptr to use
 *
 * Convert a clockdomain name stored in a struct clk 'clk' into a
 * clockdomain pointer, and save it into the struct clk.  Intended to be
 * called during clk_register().  No return value.
 */
<<<<<<< HEAD
void omap2_init_clk_clkdm(struct clk *clk)
{
	struct clockdomain *clkdm;
=======
void omap2_init_clk_clkdm(struct clk_hw *hw)
{
	struct clk_hw_omap *clk = to_clk_hw_omap(hw);
	struct clockdomain *clkdm;
	const char *clk_name;
>>>>>>> refs/remotes/origin/master

	if (!clk->clkdm_name)
		return;

<<<<<<< HEAD
	clkdm = clkdm_lookup(clk->clkdm_name);
	if (clkdm) {
		pr_debug("clock: associated clk %s to clkdm %s\n",
			 clk->name, clk->clkdm_name);
		clk->clkdm = clkdm;
	} else {
		pr_debug("clock: could not associate clk %s to "
			 "clkdm %s\n", clk->name, clk->clkdm_name);
=======
	clk_name = __clk_get_name(hw->clk);

	clkdm = clkdm_lookup(clk->clkdm_name);
	if (clkdm) {
		pr_debug("clock: associated clk %s to clkdm %s\n",
			 clk_name, clk->clkdm_name);
		clk->clkdm = clkdm;
	} else {
		pr_debug("clock: could not associate clk %s to clkdm %s\n",
			 clk_name, clk->clkdm_name);
>>>>>>> refs/remotes/origin/master
	}
}

/**
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
 * omap2_clk_disable_clkdm_control - disable clkdm control on clk enable/disable
 *
 * Prevent the OMAP clock code from calling into the clockdomain code
 * when a hardware clock in that clockdomain is enabled or disabled.
 * Intended to be called at init time from omap*_clk_init().  No
 * return value.
 */
void __init omap2_clk_disable_clkdm_control(void)
{
	clkdm_control = false;
}

/**
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * omap2_clk_dflt_find_companion - find companion clock to @clk
 * @clk: struct clk * to find the companion clock of
 * @other_reg: void __iomem ** to return the companion clock CM_*CLKEN va in
 * @other_bit: u8 ** to return the companion clock bit shift in
 *
 * Note: We don't need special code here for INVERT_ENABLE for the
 * time being since INVERT_ENABLE only applies to clocks enabled by
 * CM_CLKEN_PLL
 *
 * Convert CM_ICLKEN* <-> CM_FCLKEN*.  This conversion assumes it's
 * just a matter of XORing the bits.
 *
 * Some clocks don't have companion clocks.  For example, modules with
 * only an interface clock (such as MAILBOXES) don't have a companion
 * clock.  Right now, this code relies on the hardware exporting a bit
 * in the correct companion register that indicates that the
 * nonexistent 'companion clock' is active.  Future patches will
 * associate this type of code with per-module data structures to
 * avoid this issue, and remove the casts.  No return value.
 */
<<<<<<< HEAD
void omap2_clk_dflt_find_companion(struct clk *clk, void __iomem **other_reg,
				   u8 *other_bit)
=======
void omap2_clk_dflt_find_companion(struct clk_hw_omap *clk,
			void __iomem **other_reg, u8 *other_bit)
>>>>>>> refs/remotes/origin/master
{
	u32 r;

	/*
	 * Convert CM_ICLKEN* <-> CM_FCLKEN*.  This conversion assumes
	 * it's just a matter of XORing the bits.
	 */
	r = ((__force u32)clk->enable_reg ^ (CM_FCLKEN ^ CM_ICLKEN));

	*other_reg = (__force void __iomem *)r;
	*other_bit = clk->enable_bit;
}

/**
 * omap2_clk_dflt_find_idlest - find CM_IDLEST reg va, bit shift for @clk
 * @clk: struct clk * to find IDLEST info for
 * @idlest_reg: void __iomem ** to return the CM_IDLEST va in
 * @idlest_bit: u8 * to return the CM_IDLEST bit shift in
 * @idlest_val: u8 * to return the idle status indicator
 *
 * Return the CM_IDLEST register address and bit shift corresponding
 * to the module that "owns" this clock.  This default code assumes
 * that the CM_IDLEST bit shift is the CM_*CLKEN bit shift, and that
 * the IDLEST register address ID corresponds to the CM_*CLKEN
 * register address ID (e.g., that CM_FCLKEN2 corresponds to
 * CM_IDLEST2).  This is not true for all modules.  No return value.
 */
<<<<<<< HEAD
void omap2_clk_dflt_find_idlest(struct clk *clk, void __iomem **idlest_reg,
				u8 *idlest_bit, u8 *idlest_val)
=======
void omap2_clk_dflt_find_idlest(struct clk_hw_omap *clk,
		void __iomem **idlest_reg, u8 *idlest_bit, u8 *idlest_val)
>>>>>>> refs/remotes/origin/master
{
	u32 r;

	r = (((__force u32)clk->enable_reg & ~0xf0) | 0x20);
	*idlest_reg = (__force void __iomem *)r;
	*idlest_bit = clk->enable_bit;

	/*
	 * 24xx uses 0 to indicate not ready, and 1 to indicate ready.
	 * 34xx reverses this, just to keep us on our toes
	 * AM35xx uses both, depending on the module.
	 */
	if (cpu_is_omap24xx())
		*idlest_val = OMAP24XX_CM_IDLEST_VAL;
	else if (cpu_is_omap34xx())
		*idlest_val = OMAP34XX_CM_IDLEST_VAL;
	else
		BUG();

}

<<<<<<< HEAD
int omap2_dflt_clk_enable(struct clk *clk)
{
	u32 v;

	if (unlikely(clk->enable_reg == NULL)) {
		pr_err("clock.c: Enable for %s without enable code\n",
		       clk->name);
		return 0; /* REVISIT: -EINVAL */
	}

=======
/**
 * omap2_dflt_clk_enable - enable a clock in the hardware
 * @hw: struct clk_hw * of the clock to enable
 *
 * Enable the clock @hw in the hardware.  We first call into the OMAP
 * clockdomain code to "enable" the corresponding clockdomain if this
 * is the first enabled user of the clockdomain.  Then program the
 * hardware to enable the clock.  Then wait for the IP block that uses
 * this clock to leave idle (if applicable).  Returns the error value
 * from clkdm_clk_enable() if it terminated with an error, or -EINVAL
 * if @hw has a null clock enable_reg, or zero upon success.
 */
int omap2_dflt_clk_enable(struct clk_hw *hw)
{
	struct clk_hw_omap *clk;
	u32 v;
	int ret = 0;

	clk = to_clk_hw_omap(hw);

	if (clkdm_control && clk->clkdm) {
		ret = clkdm_clk_enable(clk->clkdm, hw->clk);
		if (ret) {
			WARN(1, "%s: could not enable %s's clockdomain %s: %d\n",
			     __func__, __clk_get_name(hw->clk),
			     clk->clkdm->name, ret);
			return ret;
		}
	}

	if (unlikely(clk->enable_reg == NULL)) {
		pr_err("%s: %s missing enable_reg\n", __func__,
		       __clk_get_name(hw->clk));
		ret = -EINVAL;
		goto err;
	}

	/* FIXME should not have INVERT_ENABLE bit here */
>>>>>>> refs/remotes/origin/master
	v = __raw_readl(clk->enable_reg);
	if (clk->flags & INVERT_ENABLE)
		v &= ~(1 << clk->enable_bit);
	else
		v |= (1 << clk->enable_bit);
	__raw_writel(v, clk->enable_reg);
	v = __raw_readl(clk->enable_reg); /* OCP barrier */

<<<<<<< HEAD
	if (clk->ops->find_idlest)
		_omap2_module_wait_ready(clk);

	return 0;
}

void omap2_dflt_clk_disable(struct clk *clk)
{
	u32 v;

	if (!clk->enable_reg) {
		/*
		 * 'Independent' here refers to a clock which is not
		 * controlled by its parent.
		 */
		printk(KERN_ERR "clock: clk_disable called on independent "
		       "clock %s which has no enable_reg\n", clk->name);
=======
	if (clk->ops && clk->ops->find_idlest)
		_omap2_module_wait_ready(clk);

	return 0;

err:
	if (clkdm_control && clk->clkdm)
		clkdm_clk_disable(clk->clkdm, hw->clk);
	return ret;
}

/**
 * omap2_dflt_clk_disable - disable a clock in the hardware
 * @hw: struct clk_hw * of the clock to disable
 *
 * Disable the clock @hw in the hardware, and call into the OMAP
 * clockdomain code to "disable" the corresponding clockdomain if all
 * clocks/hwmods in that clockdomain are now disabled.  No return
 * value.
 */
void omap2_dflt_clk_disable(struct clk_hw *hw)
{
	struct clk_hw_omap *clk;
	u32 v;

	clk = to_clk_hw_omap(hw);
	if (!clk->enable_reg) {
		/*
		 * 'independent' here refers to a clock which is not
		 * controlled by its parent.
		 */
		pr_err("%s: independent clock %s has no enable_reg\n",
		       __func__, __clk_get_name(hw->clk));
>>>>>>> refs/remotes/origin/master
		return;
	}

	v = __raw_readl(clk->enable_reg);
	if (clk->flags & INVERT_ENABLE)
		v |= (1 << clk->enable_bit);
	else
		v &= ~(1 << clk->enable_bit);
	__raw_writel(v, clk->enable_reg);
	/* No OCP barrier needed here since it is a disable operation */
<<<<<<< HEAD
}

const struct clkops clkops_omap2_dflt_wait = {
	.enable		= omap2_dflt_clk_enable,
	.disable	= omap2_dflt_clk_disable,
	.find_companion	= omap2_clk_dflt_find_companion,
	.find_idlest	= omap2_clk_dflt_find_idlest,
};

const struct clkops clkops_omap2_dflt = {
	.enable		= omap2_dflt_clk_enable,
	.disable	= omap2_dflt_clk_disable,
};

/**
 * omap2_clk_disable - disable a clock, if the system is not using it
 * @clk: struct clk * to disable
 *
 * Decrements the usecount on struct clk @clk.  If there are no users
 * left, call the clkops-specific clock disable function to disable it
 * in hardware.  If the clock is part of a clockdomain (which they all
 * should be), request that the clockdomain be disabled.  (It too has
 * a usecount, and so will not be disabled in the hardware until it no
 * longer has any users.)  If the clock has a parent clock (most of
 * them do), then call ourselves, recursing on the parent clock.  This
 * can cause an entire branch of the clock tree to be powered off by
 * simply disabling one clock.  Intended to be called with the clockfw_lock
 * spinlock held.  No return value.
 */
void omap2_clk_disable(struct clk *clk)
{
	if (clk->usecount == 0) {
		WARN(1, "clock: %s: omap2_clk_disable() called, but usecount "
		     "already 0?", clk->name);
		return;
	}

	pr_debug("clock: %s: decrementing usecount\n", clk->name);

	clk->usecount--;

	if (clk->usecount > 0)
		return;

	pr_debug("clock: %s: disabling in hardware\n", clk->name);

	if (clk->ops && clk->ops->disable) {
		trace_clock_disable(clk->name, 0, smp_processor_id());
		clk->ops->disable(clk);
	}

<<<<<<< HEAD
	if (clk->clkdm)
=======
	if (clkdm_control && clk->clkdm)
>>>>>>> refs/remotes/origin/cm-10.0
		clkdm_clk_disable(clk->clkdm, clk);

	if (clk->parent)
		omap2_clk_disable(clk->parent);
}

/**
 * omap2_clk_enable - request that the system enable a clock
 * @clk: struct clk * to enable
 *
 * Increments the usecount on struct clk @clk.  If there were no users
 * previously, then recurse up the clock tree, enabling all of the
 * clock's parents and all of the parent clockdomains, and finally,
 * enabling @clk's clockdomain, and @clk itself.  Intended to be
 * called with the clockfw_lock spinlock held.  Returns 0 upon success
 * or a negative error code upon failure.
 */
int omap2_clk_enable(struct clk *clk)
{
	int ret;

	pr_debug("clock: %s: incrementing usecount\n", clk->name);

	clk->usecount++;

	if (clk->usecount > 1)
		return 0;

	pr_debug("clock: %s: enabling in hardware\n", clk->name);

	if (clk->parent) {
		ret = omap2_clk_enable(clk->parent);
		if (ret) {
			WARN(1, "clock: %s: could not enable parent %s: %d\n",
			     clk->name, clk->parent->name, ret);
			goto oce_err1;
		}
	}

<<<<<<< HEAD
	if (clk->clkdm) {
=======
	if (clkdm_control && clk->clkdm) {
>>>>>>> refs/remotes/origin/cm-10.0
		ret = clkdm_clk_enable(clk->clkdm, clk);
		if (ret) {
			WARN(1, "clock: %s: could not enable clockdomain %s: "
			     "%d\n", clk->name, clk->clkdm->name, ret);
			goto oce_err2;
		}
	}

	if (clk->ops && clk->ops->enable) {
		trace_clock_enable(clk->name, 1, smp_processor_id());
		ret = clk->ops->enable(clk);
		if (ret) {
			WARN(1, "clock: %s: could not enable: %d\n",
			     clk->name, ret);
			goto oce_err3;
		}
	}

	return 0;

oce_err3:
<<<<<<< HEAD
	if (clk->clkdm)
=======
	if (clkdm_control && clk->clkdm)
>>>>>>> refs/remotes/origin/cm-10.0
		clkdm_clk_disable(clk->clkdm, clk);
oce_err2:
	if (clk->parent)
		omap2_clk_disable(clk->parent);
oce_err1:
	clk->usecount--;

	return ret;
}

/* Given a clock and a rate apply a clock specific rounding function */
long omap2_clk_round_rate(struct clk *clk, unsigned long rate)
{
	if (clk->round_rate)
		return clk->round_rate(clk, rate);

	return clk->rate;
}

/* Set the clock rate for a clock source */
int omap2_clk_set_rate(struct clk *clk, unsigned long rate)
{
	int ret = -EINVAL;

	pr_debug("clock: set_rate for clock %s to rate %ld\n", clk->name, rate);

	/* dpll_ck, core_ck, virt_prcm_set; plus all clksel clocks */
	if (clk->set_rate) {
		trace_clock_set_rate(clk->name, rate, smp_processor_id());
		ret = clk->set_rate(clk, rate);
	}

	return ret;
}

int omap2_clk_set_parent(struct clk *clk, struct clk *new_parent)
{
	if (!clk->clksel)
		return -EINVAL;

	if (clk->parent == new_parent)
		return 0;

	return omap2_clksel_set_parent(clk, new_parent);
}

/* OMAP3/4 non-CORE DPLL clkops */

#if defined(CONFIG_ARCH_OMAP3) || defined(CONFIG_ARCH_OMAP4)

const struct clkops clkops_omap3_noncore_dpll_ops = {
	.enable		= omap3_noncore_dpll_enable,
	.disable	= omap3_noncore_dpll_disable,
	.allow_idle	= omap3_dpll_allow_idle,
	.deny_idle	= omap3_dpll_deny_idle,
};

const struct clkops clkops_omap3_core_dpll_ops = {
	.allow_idle	= omap3_dpll_allow_idle,
	.deny_idle	= omap3_dpll_deny_idle,
};

#endif

/*
 * OMAP2+ clock reset and init functions
 */

#ifdef CONFIG_OMAP_RESET_CLOCKS
void omap2_clk_disable_unused(struct clk *clk)
{
	u32 regval32, v;

	v = (clk->flags & INVERT_ENABLE) ? (1 << clk->enable_bit) : 0;

	regval32 = __raw_readl(clk->enable_reg);
	if ((regval32 & (1 << clk->enable_bit)) == v)
		return;

	pr_debug("Disabling unused clock \"%s\"\n", clk->name);
	if (cpu_is_omap34xx()) {
		omap2_clk_enable(clk);
		omap2_clk_disable(clk);
	} else {
		clk->ops->disable(clk);
	}
	if (clk->clkdm != NULL)
		pwrdm_clkdm_state_switch(clk->clkdm);
}
#endif
=======

	if (clkdm_control && clk->clkdm)
		clkdm_clk_disable(clk->clkdm, hw->clk);
}

/**
 * omap2_clkops_enable_clkdm - increment usecount on clkdm of @hw
 * @hw: struct clk_hw * of the clock being enabled
 *
 * Increment the usecount of the clockdomain of the clock pointed to
 * by @hw; if the usecount is 1, the clockdomain will be "enabled."
 * Only needed for clocks that don't use omap2_dflt_clk_enable() as
 * their enable function pointer.  Passes along the return value of
 * clkdm_clk_enable(), -EINVAL if @hw is not associated with a
 * clockdomain, or 0 if clock framework-based clockdomain control is
 * not implemented.
 */
int omap2_clkops_enable_clkdm(struct clk_hw *hw)
{
	struct clk_hw_omap *clk;
	int ret = 0;

	clk = to_clk_hw_omap(hw);

	if (unlikely(!clk->clkdm)) {
		pr_err("%s: %s: no clkdm set ?!\n", __func__,
		       __clk_get_name(hw->clk));
		return -EINVAL;
	}

	if (unlikely(clk->enable_reg))
		pr_err("%s: %s: should use dflt_clk_enable ?!\n", __func__,
		       __clk_get_name(hw->clk));

	if (!clkdm_control) {
		pr_err("%s: %s: clkfw-based clockdomain control disabled ?!\n",
		       __func__, __clk_get_name(hw->clk));
		return 0;
	}

	ret = clkdm_clk_enable(clk->clkdm, hw->clk);
	WARN(ret, "%s: could not enable %s's clockdomain %s: %d\n",
	     __func__, __clk_get_name(hw->clk), clk->clkdm->name, ret);

	return ret;
}

/**
 * omap2_clkops_disable_clkdm - decrement usecount on clkdm of @hw
 * @hw: struct clk_hw * of the clock being disabled
 *
 * Decrement the usecount of the clockdomain of the clock pointed to
 * by @hw; if the usecount is 0, the clockdomain will be "disabled."
 * Only needed for clocks that don't use omap2_dflt_clk_disable() as their
 * disable function pointer.  No return value.
 */
void omap2_clkops_disable_clkdm(struct clk_hw *hw)
{
	struct clk_hw_omap *clk;

	clk = to_clk_hw_omap(hw);

	if (unlikely(!clk->clkdm)) {
		pr_err("%s: %s: no clkdm set ?!\n", __func__,
		       __clk_get_name(hw->clk));
		return;
	}

	if (unlikely(clk->enable_reg))
		pr_err("%s: %s: should use dflt_clk_disable ?!\n", __func__,
		       __clk_get_name(hw->clk));

	if (!clkdm_control) {
		pr_err("%s: %s: clkfw-based clockdomain control disabled ?!\n",
		       __func__, __clk_get_name(hw->clk));
		return;
	}

	clkdm_clk_disable(clk->clkdm, hw->clk);
}

/**
 * omap2_dflt_clk_is_enabled - is clock enabled in the hardware?
 * @hw: struct clk_hw * to check
 *
 * Return 1 if the clock represented by @hw is enabled in the
 * hardware, or 0 otherwise.  Intended for use in the struct
 * clk_ops.is_enabled function pointer.
 */
int omap2_dflt_clk_is_enabled(struct clk_hw *hw)
{
	struct clk_hw_omap *clk = to_clk_hw_omap(hw);
	u32 v;

	v = __raw_readl(clk->enable_reg);

	if (clk->flags & INVERT_ENABLE)
		v ^= BIT(clk->enable_bit);

	v &= BIT(clk->enable_bit);

	return v ? 1 : 0;
}

static int __initdata mpurate;

/*
 * By default we use the rate set by the bootloader.
 * You can override this with mpurate= cmdline option.
 */
static int __init omap_clk_setup(char *str)
{
	get_option(&str, &mpurate);

	if (!mpurate)
		return 1;

	if (mpurate < 1000)
		mpurate *= 1000000;

	return 1;
}
__setup("mpurate=", omap_clk_setup);

/**
 * omap2_init_clk_hw_omap_clocks - initialize an OMAP clock
 * @clk: struct clk * to initialize
 *
 * Add an OMAP clock @clk to the internal list of OMAP clocks.  Used
 * temporarily for autoidle handling, until this support can be
 * integrated into the common clock framework code in some way.  No
 * return value.
 */
void omap2_init_clk_hw_omap_clocks(struct clk *clk)
{
	struct clk_hw_omap *c;

	if (__clk_get_flags(clk) & CLK_IS_BASIC)
		return;

	c = to_clk_hw_omap(__clk_get_hw(clk));
	list_add(&c->node, &clk_hw_omap_clocks);
}

/**
 * omap2_clk_enable_autoidle_all - enable autoidle on all OMAP clocks that
 * support it
 *
 * Enable clock autoidle on all OMAP clocks that have allow_idle
 * function pointers associated with them.  This function is intended
 * to be temporary until support for this is added to the common clock
 * code.  Returns 0.
 */
int omap2_clk_enable_autoidle_all(void)
{
	struct clk_hw_omap *c;

	list_for_each_entry(c, &clk_hw_omap_clocks, node)
		if (c->ops && c->ops->allow_idle)
			c->ops->allow_idle(c);
	return 0;
}

/**
 * omap2_clk_disable_autoidle_all - disable autoidle on all OMAP clocks that
 * support it
 *
 * Disable clock autoidle on all OMAP clocks that have allow_idle
 * function pointers associated with them.  This function is intended
 * to be temporary until support for this is added to the common clock
 * code.  Returns 0.
 */
int omap2_clk_disable_autoidle_all(void)
{
	struct clk_hw_omap *c;

	list_for_each_entry(c, &clk_hw_omap_clocks, node)
		if (c->ops && c->ops->deny_idle)
			c->ops->deny_idle(c);
	return 0;
}

/**
 * omap2_clk_deny_idle - disable autoidle on an OMAP clock
 * @clk: struct clk * to disable autoidle for
 *
 * Disable autoidle on an OMAP clock.
 */
int omap2_clk_deny_idle(struct clk *clk)
{
	struct clk_hw_omap *c;

	if (__clk_get_flags(clk) & CLK_IS_BASIC)
		return -EINVAL;

	c = to_clk_hw_omap(__clk_get_hw(clk));
	if (c->ops && c->ops->deny_idle)
		c->ops->deny_idle(c);
	return 0;
}

/**
 * omap2_clk_allow_idle - enable autoidle on an OMAP clock
 * @clk: struct clk * to enable autoidle for
 *
 * Enable autoidle on an OMAP clock.
 */
int omap2_clk_allow_idle(struct clk *clk)
{
	struct clk_hw_omap *c;

	if (__clk_get_flags(clk) & CLK_IS_BASIC)
		return -EINVAL;

	c = to_clk_hw_omap(__clk_get_hw(clk));
	if (c->ops && c->ops->allow_idle)
		c->ops->allow_idle(c);
	return 0;
}

/**
 * omap2_clk_enable_init_clocks - prepare & enable a list of clocks
 * @clk_names: ptr to an array of strings of clock names to enable
 * @num_clocks: number of clock names in @clk_names
 *
 * Prepare and enable a list of clocks, named by @clk_names.  No
 * return value. XXX Deprecated; only needed until these clocks are
 * properly claimed and enabled by the drivers or core code that uses
 * them.  XXX What code disables & calls clk_put on these clocks?
 */
void omap2_clk_enable_init_clocks(const char **clk_names, u8 num_clocks)
{
	struct clk *init_clk;
	int i;

	for (i = 0; i < num_clocks; i++) {
		init_clk = clk_get(NULL, clk_names[i]);
		clk_prepare_enable(init_clk);
	}
}

const struct clk_hw_omap_ops clkhwops_wait = {
	.find_idlest	= omap2_clk_dflt_find_idlest,
	.find_companion	= omap2_clk_dflt_find_companion,
};

/**
 * omap_clocks_register - register an array of omap_clk
 * @ocs: pointer to an array of omap_clk to register
 */
void __init omap_clocks_register(struct omap_clk oclks[], int cnt)
{
	struct omap_clk *c;

	for (c = oclks; c < oclks + cnt; c++) {
		clkdev_add(&c->lk);
		if (!__clk_init(NULL, c->lk.clk))
			omap2_init_clk_hw_omap_clocks(c->lk.clk);
	}
}
>>>>>>> refs/remotes/origin/master

/**
 * omap2_clk_switch_mpurate_at_boot - switch ARM MPU rate by boot-time argument
 * @mpurate_ck_name: clk name of the clock to change rate
 *
 * Change the ARM MPU clock rate to the rate specified on the command
 * line, if one was specified.  @mpurate_ck_name should be
 * "virt_prcm_set" on OMAP2xxx and "dpll1_ck" on OMAP34xx/OMAP36xx.
 * XXX Does not handle voltage scaling - on OMAP2xxx this is currently
 * handled by the virt_prcm_set clock, but this should be handled by
 * the OPP layer.  XXX This is intended to be handled by the OPP layer
 * code in the near future and should be removed from the clock code.
 * Returns -EINVAL if 'mpurate' is zero or if clk_set_rate() rejects
 * the rate, -ENOENT if the struct clk referred to by @mpurate_ck_name
 * cannot be found, or 0 upon success.
 */
int __init omap2_clk_switch_mpurate_at_boot(const char *mpurate_ck_name)
{
	struct clk *mpurate_ck;
	int r;

	if (!mpurate)
		return -EINVAL;

	mpurate_ck = clk_get(NULL, mpurate_ck_name);
	if (WARN(IS_ERR(mpurate_ck), "Failed to get %s.\n", mpurate_ck_name))
		return -ENOENT;

	r = clk_set_rate(mpurate_ck, mpurate);
<<<<<<< HEAD
	if (IS_ERR_VALUE(r)) {
		WARN(1, "clock: %s: unable to set MPU rate to %d: %d\n",
		     mpurate_ck->name, mpurate, r);
<<<<<<< HEAD
=======
		clk_put(mpurate_ck);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (r < 0) {
		WARN(1, "clock: %s: unable to set MPU rate to %d: %d\n",
		     mpurate_ck_name, mpurate, r);
		clk_put(mpurate_ck);
>>>>>>> refs/remotes/origin/master
		return -EINVAL;
	}

	calibrate_delay();
<<<<<<< HEAD
	recalculate_root_clocks();

=======
>>>>>>> refs/remotes/origin/master
	clk_put(mpurate_ck);

	return 0;
}

/**
 * omap2_clk_print_new_rates - print summary of current clock tree rates
 * @hfclkin_ck_name: clk name for the off-chip HF oscillator
 * @core_ck_name: clk name for the on-chip CORE_CLK
 * @mpu_ck_name: clk name for the ARM MPU clock
 *
 * Prints a short message to the console with the HFCLKIN oscillator
 * rate, the rate of the CORE clock, and the rate of the ARM MPU clock.
 * Called by the boot-time MPU rate switching code.   XXX This is intended
 * to be handled by the OPP layer code in the near future and should be
 * removed from the clock code.  No return value.
 */
void __init omap2_clk_print_new_rates(const char *hfclkin_ck_name,
				      const char *core_ck_name,
				      const char *mpu_ck_name)
{
	struct clk *hfclkin_ck, *core_ck, *mpu_ck;
	unsigned long hfclkin_rate;

	mpu_ck = clk_get(NULL, mpu_ck_name);
	if (WARN(IS_ERR(mpu_ck), "clock: failed to get %s.\n", mpu_ck_name))
		return;

	core_ck = clk_get(NULL, core_ck_name);
	if (WARN(IS_ERR(core_ck), "clock: failed to get %s.\n", core_ck_name))
		return;

	hfclkin_ck = clk_get(NULL, hfclkin_ck_name);
	if (WARN(IS_ERR(hfclkin_ck), "Failed to get %s.\n", hfclkin_ck_name))
		return;

	hfclkin_rate = clk_get_rate(hfclkin_ck);

<<<<<<< HEAD
	pr_info("Switched to new clocking rate (Crystal/Core/MPU): "
		"%ld.%01ld/%ld/%ld MHz\n",
		(hfclkin_rate / 1000000),
		((hfclkin_rate / 100000) % 10),
		(clk_get_rate(core_ck) / 1000000),
		(clk_get_rate(mpu_ck) / 1000000));
}

/* Common data */

struct clk_functions omap2_clk_functions = {
	.clk_enable		= omap2_clk_enable,
	.clk_disable		= omap2_clk_disable,
	.clk_round_rate		= omap2_clk_round_rate,
	.clk_set_rate		= omap2_clk_set_rate,
	.clk_set_parent		= omap2_clk_set_parent,
	.clk_disable_unused	= omap2_clk_disable_unused,
<<<<<<< HEAD
#ifdef CONFIG_CPU_FREQ
	/* These will be removed when the OPP code is integrated */
	.clk_init_cpufreq_table	= omap2_clk_init_cpufreq_table,
	.clk_exit_cpufreq_table	= omap2_clk_exit_cpufreq_table,
#endif
=======
>>>>>>> refs/remotes/origin/cm-10.0
};

=======
	pr_info("Switched to new clocking rate (Crystal/Core/MPU): %ld.%01ld/%ld/%ld MHz\n",
		(hfclkin_rate / 1000000), ((hfclkin_rate / 100000) % 10),
		(clk_get_rate(core_ck) / 1000000),
		(clk_get_rate(mpu_ck) / 1000000));
}
>>>>>>> refs/remotes/origin/master
