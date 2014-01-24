/* Copyright (c) 2010-2011, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __ARCH_ARM_MACH_MSM_CLOCK_VOTER_H
#define __ARCH_ARM_MACH_MSM_CLOCK_VOTER_H

struct clk_ops;
extern struct clk_ops clk_ops_voter;

struct clk_voter {
	bool enabled;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long rate;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	struct clk *parent;
	struct clk c;
};

static inline struct clk_voter *to_clk_voter(struct clk *clk)
{
	return container_of(clk, struct clk_voter, c);
}

<<<<<<< HEAD
<<<<<<< HEAD
#define DEFINE_CLK_VOTER(clk_name, _parent) \
=======
#define DEFINE_CLK_VOTER(clk_name, _parent, _default_rate) \
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define DEFINE_CLK_VOTER(clk_name, _parent, _default_rate) \
>>>>>>> refs/remotes/origin/cm-11.0
	struct clk_voter clk_name = { \
		.parent = _parent, \
		.c = { \
			.dbg_name = #clk_name, \
			.ops = &clk_ops_voter, \
<<<<<<< HEAD
<<<<<<< HEAD
			.flags = CLKFLAG_SKIP_AUTO_OFF, \
=======
			.rate = _default_rate, \
>>>>>>> refs/remotes/origin/cm-10.0
=======
			.rate = _default_rate, \
>>>>>>> refs/remotes/origin/cm-11.0
			CLK_INIT(clk_name.c), \
		}, \
	}

#endif
