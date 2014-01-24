/*
 * Copyright (C) 2007 Google, Inc.
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (c) 2007-2011, The Linux Foundation. All rights reserved.
=======
 * Copyright (c) 2007-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/master
=======
 * Copyright (c) 2007-2011, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-11.0
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 */

#include <linux/kernel.h>
#include <linux/err.h>
<<<<<<< HEAD
<<<<<<< HEAD

#include <mach/clk.h>
#include <mach/socinfo.h>
<<<<<<< HEAD

#include "proc_comm.h"
=======
#include <mach/proc_comm.h>

>>>>>>> refs/remotes/origin/cm-10.0
=======

#include <mach/clk.h>
#include <mach/socinfo.h>
#include <mach/proc_comm.h>

>>>>>>> refs/remotes/origin/cm-11.0
#include "clock.h"
#include "clock-pcom.h"

/*
 * glue for the proc_comm interface
 */
static int pc_clk_enable(struct clk *clk)
<<<<<<< HEAD
{
	int rc;
	int id = to_pcom_clk(clk)->id;

	/* Ignore clocks that are always on */
	if (id == P_EBI1_CLK || id == P_EBI1_FIXED_CLK)
		return 0;

	rc = msm_proc_comm(PCOM_CLKCTL_RPC_ENABLE, &id, NULL);
=======
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/clk-provider.h>
#include <linux/clkdev.h>

#include <mach/clk.h>

#include "proc_comm.h"
#include "clock.h"
#include "clock-pcom.h"

struct clk_pcom {
	unsigned id;
	unsigned long flags;
	struct msm_clk msm_clk;
};

static inline struct clk_pcom *to_clk_pcom(struct clk_hw *hw)
{
	return container_of(to_msm_clk(hw), struct clk_pcom, msm_clk);
}

static int pc_clk_enable(struct clk_hw *hw)
{
	unsigned id = to_clk_pcom(hw)->id;
	int rc = msm_proc_comm(PCOM_CLKCTL_RPC_ENABLE, &id, NULL);
>>>>>>> refs/remotes/origin/master
=======
{
	int rc;
	int id = to_pcom_clk(clk)->id;

	/* Ignore clocks that are always on */
	if (id == P_EBI1_CLK || id == P_EBI1_FIXED_CLK)
		return 0;

	rc = msm_proc_comm(PCOM_CLKCTL_RPC_ENABLE, &id, NULL);
>>>>>>> refs/remotes/origin/cm-11.0
	if (rc < 0)
		return rc;
	else
		return (int)id < 0 ? -EINVAL : 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
static void pc_clk_disable(struct clk *clk)
{
	int id = to_pcom_clk(clk)->id;

	/* Ignore clocks that are always on */
	if (id == P_EBI1_CLK || id == P_EBI1_FIXED_CLK)
		return;

	msm_proc_comm(PCOM_CLKCTL_RPC_DISABLE, &id, NULL);
}

int pc_clk_reset(unsigned id, enum clk_reset_action action)
{
	int rc;
=======
static void pc_clk_disable(struct clk_hw *hw)
{
	unsigned id = to_clk_pcom(hw)->id;
	msm_proc_comm(PCOM_CLKCTL_RPC_DISABLE, &id, NULL);
}

static int pc_clk_reset(struct clk_hw *hw, enum clk_reset_action action)
{
	int rc;
	unsigned id = to_clk_pcom(hw)->id;
>>>>>>> refs/remotes/origin/master

	if (action == CLK_RESET_ASSERT)
		rc = msm_proc_comm(PCOM_CLKCTL_RPC_RESET_ASSERT, &id, NULL);
	else
		rc = msm_proc_comm(PCOM_CLKCTL_RPC_RESET_DEASSERT, &id, NULL);

	if (rc < 0)
		return rc;
	else
		return (int)id < 0 ? -EINVAL : 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
static int pc_reset(struct clk *clk, enum clk_reset_action action)
{
	int id = to_pcom_clk(clk)->id;
	return pc_clk_reset(id, action);
}

static int _pc_clk_set_rate(struct clk *clk, unsigned long rate)
{
	/* The rate _might_ be rounded off to the nearest KHz value by the
	 * remote function. So a return value of 0 doesn't necessarily mean
	 * that the exact rate was set successfully.
	 */
	unsigned r = rate;
	int id = to_pcom_clk(clk)->id;
	int rc = msm_proc_comm(PCOM_CLKCTL_RPC_SET_RATE, &id, &r);
	if (rc < 0)
		return rc;
	else
		return (int)id < 0 ? -EINVAL : 0;
}

static int _pc_clk_set_min_rate(struct clk *clk, unsigned long rate)
<<<<<<< HEAD
{
	int rc;
	int id = to_pcom_clk(clk)->id;
	bool ignore_error = (cpu_is_msm7x27() && id == P_EBI1_CLK &&
				rate >= INT_MAX);
	unsigned r = rate;
	rc = msm_proc_comm(PCOM_CLKCTL_RPC_MIN_RATE, &id, &r);
	if (rc < 0)
		return rc;
	else if (ignore_error)
		return 0;
=======
static int pc_clk_set_rate(struct clk_hw *hw, unsigned long new_rate,
			   unsigned long p_rate)
{
	struct clk_pcom *p = to_clk_pcom(hw);
	unsigned id = p->id, rate = new_rate;
	int rc;

	/*
	 * The rate _might_ be rounded off to the nearest KHz value by the
	 * remote function. So a return value of 0 doesn't necessarily mean
	 * that the exact rate was set successfully.
	 */
	if (p->flags & CLKFLAG_MIN)
		rc = msm_proc_comm(PCOM_CLKCTL_RPC_MIN_RATE, &id, &rate);
	else
		rc = msm_proc_comm(PCOM_CLKCTL_RPC_SET_RATE, &id, &rate);
	if (rc < 0)
		return rc;
>>>>>>> refs/remotes/origin/master
=======
{
	int rc;
	int id = to_pcom_clk(clk)->id;
	bool ignore_error = (cpu_is_msm7x27() && id == P_EBI1_CLK &&
				rate >= INT_MAX);
	unsigned r = rate;
	rc = msm_proc_comm(PCOM_CLKCTL_RPC_MIN_RATE, &id, &r);
	if (rc < 0)
		return rc;
	else if (ignore_error)
		return 0;
>>>>>>> refs/remotes/origin/cm-11.0
	else
		return (int)id < 0 ? -EINVAL : 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int pc_clk_set_rate(struct clk *clk, unsigned long rate)
{
	if (clk->flags & CLKFLAG_MIN)
		return _pc_clk_set_min_rate(clk, rate);
	else
		return _pc_clk_set_rate(clk, rate);
}

static int pc_clk_set_max_rate(struct clk *clk, unsigned long rate)
{
=======
static int pc_clk_set_rate(struct clk *clk, unsigned long rate)
{
	if (clk->flags & CLKFLAG_MIN)
		return _pc_clk_set_min_rate(clk, rate);
	else
		return _pc_clk_set_rate(clk, rate);
}

static int pc_clk_set_max_rate(struct clk *clk, unsigned long rate)
{
>>>>>>> refs/remotes/origin/cm-11.0
	int id = to_pcom_clk(clk)->id;
	unsigned r = rate;
	int rc = msm_proc_comm(PCOM_CLKCTL_RPC_MAX_RATE, &id, &r);
	if (rc < 0)
		return rc;
	else
		return (int)id < 0 ? -EINVAL : 0;
}

static int pc_clk_set_flags(struct clk *clk, unsigned flags)
{
	int id = to_pcom_clk(clk)->id;
	int rc = msm_proc_comm(PCOM_CLKCTL_RPC_SET_FLAGS, &id, &flags);
	if (rc < 0)
		return rc;
	else
		return (int)id < 0 ? -EINVAL : 0;
}

static int pc_clk_set_ext_config(struct clk *clk, unsigned long config)
<<<<<<< HEAD
{
	int id = to_pcom_clk(clk)->id;
	unsigned c = config;
	int rc = msm_proc_comm(PCOM_CLKCTL_RPC_SET_EXT_CONFIG, &id, &c);
	if (rc < 0)
		return rc;
	else
		return (int)id < 0 ? -EINVAL : 0;
}

static unsigned long pc_clk_get_rate(struct clk *clk)
{
	int id = to_pcom_clk(clk)->id;
=======
static unsigned long pc_clk_recalc_rate(struct clk_hw *hw, unsigned long p_rate)
{
	unsigned id = to_clk_pcom(hw)->id;
>>>>>>> refs/remotes/origin/master
=======
{
	int id = to_pcom_clk(clk)->id;
	unsigned c = config;
	int rc = msm_proc_comm(PCOM_CLKCTL_RPC_SET_EXT_CONFIG, &id, &c);
	if (rc < 0)
		return rc;
	else
		return (int)id < 0 ? -EINVAL : 0;
}

static unsigned long pc_clk_get_rate(struct clk *clk)
{
	int id = to_pcom_clk(clk)->id;
>>>>>>> refs/remotes/origin/cm-11.0
	if (msm_proc_comm(PCOM_CLKCTL_RPC_RATE, &id, NULL))
		return 0;
	else
		return id;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int pc_clk_is_enabled(struct clk *clk)
{
	int id = to_pcom_clk(clk)->id;
=======
static int pc_clk_is_enabled(struct clk_hw *hw)
{
	unsigned id = to_clk_pcom(hw)->id;
>>>>>>> refs/remotes/origin/master
=======
static int pc_clk_is_enabled(struct clk *clk)
{
	int id = to_pcom_clk(clk)->id;
>>>>>>> refs/remotes/origin/cm-11.0
	if (msm_proc_comm(PCOM_CLKCTL_RPC_ENABLED, &id, NULL))
		return 0;
	else
		return id;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
static long pc_clk_round_rate(struct clk *clk, unsigned long rate)
{

=======
static long pc_clk_round_rate(struct clk_hw *hw, unsigned long rate,
			      unsigned long *p_rate)
{
>>>>>>> refs/remotes/origin/master
	/* Not really supported; pc_clk_set_rate() does rounding on it's own. */
	return rate;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
static bool pc_clk_is_local(struct clk *clk)
{
	return false;
}

struct clk_ops clk_ops_pcom = {
	.enable = pc_clk_enable,
	.disable = pc_clk_disable,
<<<<<<< HEAD
<<<<<<< HEAD
	.auto_off = pc_clk_disable,
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	.reset = pc_reset,
	.set_rate = pc_clk_set_rate,
	.set_max_rate = pc_clk_set_max_rate,
	.set_flags = pc_clk_set_flags,
	.get_rate = pc_clk_get_rate,
	.is_enabled = pc_clk_is_enabled,
	.round_rate = pc_clk_round_rate,
	.is_local = pc_clk_is_local,
};

struct clk_ops clk_ops_pcom_ext_config = {
	.enable = pc_clk_enable,
	.disable = pc_clk_disable,
<<<<<<< HEAD
<<<<<<< HEAD
	.auto_off = pc_clk_disable,
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	.reset = pc_reset,
	.set_rate = pc_clk_set_ext_config,
	.set_max_rate = pc_clk_set_max_rate,
	.set_flags = pc_clk_set_flags,
	.get_rate = pc_clk_get_rate,
	.is_enabled = pc_clk_is_enabled,
	.round_rate = pc_clk_round_rate,
	.is_local = pc_clk_is_local,
};

<<<<<<< HEAD
=======
static struct clk_ops clk_ops_pcom = {
	.enable = pc_clk_enable,
	.disable = pc_clk_disable,
	.set_rate = pc_clk_set_rate,
	.recalc_rate = pc_clk_recalc_rate,
	.is_enabled = pc_clk_is_enabled,
	.round_rate = pc_clk_round_rate,
};

static int msm_clock_pcom_probe(struct platform_device *pdev)
{
	const struct pcom_clk_pdata *pdata = pdev->dev.platform_data;
	int i, ret;

	for (i = 0; i < pdata->num_lookups; i++) {
		const struct clk_pcom_desc *desc = &pdata->lookup[i];
		struct clk *c;
		struct clk_pcom *p;
		struct clk_hw *hw;
		struct clk_init_data init;

		p = devm_kzalloc(&pdev->dev, sizeof(*p), GFP_KERNEL);
		if (!p)
			return -ENOMEM;

		p->id = desc->id;
		p->flags = desc->flags;
		p->msm_clk.reset = pc_clk_reset;

		hw = &p->msm_clk.hw;
		hw->init = &init;

		init.name = desc->name;
		init.ops = &clk_ops_pcom;
		init.num_parents = 0;
		init.flags = CLK_IS_ROOT;

		if (!(p->flags & CLKFLAG_AUTO_OFF))
			init.flags |= CLK_IGNORE_UNUSED;

		c = devm_clk_register(&pdev->dev, hw);
		ret = clk_register_clkdev(c, desc->con, desc->dev);
		if (ret)
			return ret;
	}

	return 0;
}

static struct platform_driver msm_clock_pcom_driver = {
	.probe		= msm_clock_pcom_probe,
	.driver		= {
		.name	= "msm-clock-pcom",
		.owner	= THIS_MODULE,
	},
};
module_platform_driver(msm_clock_pcom_driver);

MODULE_LICENSE("GPL v2");
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
