/*
 * OMAP2-specific DPLL control functions
 *
 * Copyright (C) 2011 Nokia Corporation
 * Paul Walmsley
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/clk.h>
#include <linux/io.h>

<<<<<<< HEAD
#include <plat/clock.h>

#include "clock.h"
#include "cm2xxx_3xxx.h"
=======
#include "clock.h"
#include "cm2xxx.h"
>>>>>>> refs/remotes/origin/master
#include "cm-regbits-24xx.h"

/* Private functions */

/**
 * _allow_idle - enable DPLL autoidle bits
 * @clk: struct clk * of the DPLL to operate on
 *
 * Enable DPLL automatic idle control.  The DPLL will enter low-power
 * stop when its downstream clocks are gated.  No return value.
 * REVISIT: DPLL can optionally enter low-power bypass by writing 0x1
 * instead.  Add some mechanism to optionally enter this mode.
 */
<<<<<<< HEAD
static void _allow_idle(struct clk *clk)
=======
static void _allow_idle(struct clk_hw_omap *clk)
>>>>>>> refs/remotes/origin/master
{
	if (!clk || !clk->dpll_data)
		return;

	omap2xxx_cm_set_dpll_auto_low_power_stop();
}

/**
 * _deny_idle - prevent DPLL from automatically idling
 * @clk: struct clk * of the DPLL to operate on
 *
 * Disable DPLL automatic idle control.  No return value.
 */
<<<<<<< HEAD
static void _deny_idle(struct clk *clk)
=======
static void _deny_idle(struct clk_hw_omap *clk)
>>>>>>> refs/remotes/origin/master
{
	if (!clk || !clk->dpll_data)
		return;

	omap2xxx_cm_set_dpll_disable_autoidle();
}


/* Public data */
<<<<<<< HEAD

const struct clkops clkops_omap2xxx_dpll_ops = {
	.allow_idle	= _allow_idle,
	.deny_idle	= _deny_idle,
};

=======
const struct clk_hw_omap_ops clkhwops_omap2xxx_dpll = {
	.allow_idle	= _allow_idle,
	.deny_idle	= _deny_idle,
};
>>>>>>> refs/remotes/origin/master
