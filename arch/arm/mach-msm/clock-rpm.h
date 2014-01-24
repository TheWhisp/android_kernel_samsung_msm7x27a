<<<<<<< HEAD
<<<<<<< HEAD
/* Copyright (c) 2010-2011, The Linux Foundation. All rights reserved.
=======
/* Copyright (c) 2010-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
=======
/* Copyright (c) 2010-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-11.0
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

#ifndef __ARCH_ARM_MACH_MSM_CLOCK_RPM_H
#define __ARCH_ARM_MACH_MSM_CLOCK_RPM_H

#include <mach/rpm.h>
<<<<<<< HEAD
<<<<<<< HEAD

struct clk_ops;
extern struct clk_ops clk_ops_rpm;

struct rpm_clk {
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
#include <mach/rpm-smd.h>

struct clk_ops;
struct clk_rpmrs_data;
extern struct clk_ops clk_ops_rpm;
extern struct clk_ops clk_ops_rpm_branch;

struct rpm_clk {
	const int rpm_res_type;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
	const int rpm_clk_id;
	const int rpm_status_id;
	const bool active_only;
	unsigned last_set_khz;
	/* 0 if active_only. Otherwise, same as last_set_khz. */
	unsigned last_set_sleep_khz;
	bool enabled;
<<<<<<< HEAD
<<<<<<< HEAD
=======
	bool branch; /* true: RPM only accepts 1 for ON and 0 for OFF */
	unsigned factor;
	struct clk_rpmrs_data *rpmrs_data;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	bool branch; /* true: RPM only accepts 1 for ON and 0 for OFF */
	unsigned factor;
	struct clk_rpmrs_data *rpmrs_data;
>>>>>>> refs/remotes/origin/cm-11.0

	struct rpm_clk *peer;
	struct clk c;
};

static inline struct rpm_clk *to_rpm_clk(struct clk *clk)
{
	return container_of(clk, struct rpm_clk, c);
}

<<<<<<< HEAD
<<<<<<< HEAD
#define DEFINE_CLK_RPM(name, active, r_id, dep) \
	static struct rpm_clk active; \
	static struct rpm_clk name = { \
		.rpm_clk_id = MSM_RPM_ID_##r_id##_CLK, \
		.rpm_status_id = MSM_RPM_STATUS_ID_##r_id##_CLK, \
		.peer = &active, \
		.c = { \
			.ops = &clk_ops_rpm, \
			.flags = CLKFLAG_SKIP_AUTO_OFF, \
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
extern struct clk_rpmrs_data clk_rpmrs_data;
extern struct clk_rpmrs_data clk_rpmrs_data_smd;

#define __DEFINE_CLK_RPM(name, active, type, r_id, stat_id, dep, rpmrsdata) \
	static struct rpm_clk active; \
	static struct rpm_clk name = { \
		.rpm_res_type = (type), \
		.rpm_clk_id = (r_id), \
		.rpm_status_id = (stat_id), \
		.peer = &active, \
		.factor = 1000, \
		.rpmrs_data = (rpmrsdata),\
		.c = { \
			.ops = &clk_ops_rpm, \
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
			.dbg_name = #name, \
			CLK_INIT(name.c), \
			.depends = dep, \
		}, \
	}; \
	static struct rpm_clk active = { \
<<<<<<< HEAD
<<<<<<< HEAD
		.rpm_clk_id = MSM_RPM_ID_##r_id##_CLK, \
		.rpm_status_id = MSM_RPM_STATUS_ID_##r_id##_CLK, \
		.peer = &name, \
		.active_only = true, \
		.c = { \
			.ops = &clk_ops_rpm, \
			.flags = CLKFLAG_SKIP_AUTO_OFF, \
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
		.rpm_res_type = (type), \
		.rpm_clk_id = (r_id), \
		.rpm_status_id = (stat_id), \
		.peer = &name, \
		.active_only = true, \
		.factor = 1000, \
		.rpmrs_data = (rpmrsdata),\
		.c = { \
			.ops = &clk_ops_rpm, \
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
			.dbg_name = #active, \
			CLK_INIT(active.c), \
			.depends = dep, \
		}, \
	};

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/cm-11.0
#define __DEFINE_CLK_RPM_BRANCH(name, active, type, r_id, stat_id, r, \
					rpmrsdata) \
	static struct rpm_clk active; \
	static struct rpm_clk name = { \
		.rpm_res_type = (type), \
		.rpm_clk_id = (r_id), \
		.rpm_status_id = (stat_id), \
		.peer = &active, \
		.last_set_khz = ((r) / 1000), \
		.last_set_sleep_khz = ((r) / 1000), \
		.factor = 1000, \
		.branch = true, \
		.rpmrs_data = (rpmrsdata),\
		.c = { \
			.ops = &clk_ops_rpm_branch, \
			.dbg_name = #name, \
			.rate = (r), \
			CLK_INIT(name.c), \
			.warned = true, \
		}, \
	}; \
	static struct rpm_clk active = { \
		.rpm_res_type = (type), \
		.rpm_clk_id = (r_id), \
		.rpm_status_id = (stat_id), \
		.peer = &name, \
		.last_set_khz = ((r) / 1000), \
		.active_only = true, \
		.factor = 1000, \
		.branch = true, \
		.rpmrs_data = (rpmrsdata),\
		.c = { \
			.ops = &clk_ops_rpm_branch, \
			.dbg_name = #active, \
			.rate = (r), \
			CLK_INIT(active.c), \
			.warned = true, \
		}, \
	};

#define __DEFINE_CLK_RPM_QDSS(name, active, type, r_id, stat_id, rpmrsdata) \
	static struct rpm_clk active; \
	static struct rpm_clk name = { \
		.rpm_res_type = (type), \
		.rpm_clk_id = (r_id), \
		.rpm_status_id = (stat_id), \
		.peer = &active, \
		.factor = 1, \
		.rpmrs_data = (rpmrsdata),\
		.c = { \
			.ops = &clk_ops_rpm, \
			.dbg_name = #name, \
			CLK_INIT(name.c), \
			.warned = true, \
		}, \
	}; \
	static struct rpm_clk active = { \
		.rpm_res_type = (type), \
		.rpm_clk_id = (r_id), \
		.rpm_status_id = (stat_id), \
		.peer = &name, \
		.active_only = true, \
		.factor = 1, \
		.rpmrs_data = (rpmrsdata),\
		.c = { \
			.ops = &clk_ops_rpm, \
			.dbg_name = #active, \
			CLK_INIT(active.c), \
			.warned = true, \
		}, \
	};

#define DEFINE_CLK_RPM(name, active, r_id, dep) \
	__DEFINE_CLK_RPM(name, active, 0, MSM_RPM_ID_##r_id##_CLK, \
		MSM_RPM_STATUS_ID_##r_id##_CLK, dep, &clk_rpmrs_data)

#define DEFINE_CLK_RPM_QDSS(name, active) \
	__DEFINE_CLK_RPM_QDSS(name, active, 0, MSM_RPM_ID_QDSS_CLK, \
		MSM_RPM_STATUS_ID_QDSS_CLK, &clk_rpmrs_data)

#define DEFINE_CLK_RPM_BRANCH(name, active, r_id, r) \
	__DEFINE_CLK_RPM_BRANCH(name, active, 0, MSM_RPM_ID_##r_id##_CLK, \
			MSM_RPM_STATUS_ID_##r_id##_CLK, r, &clk_rpmrs_data)

#define DEFINE_CLK_RPM_SMD(name, active, type, r_id, dep) \
	__DEFINE_CLK_RPM(name, active, type, r_id, 0, dep, &clk_rpmrs_data_smd)

#define DEFINE_CLK_RPM_SMD_BRANCH(name, active, type, r_id, dep) \
	__DEFINE_CLK_RPM_BRANCH(name, active, type, r_id, 0, dep, \
					&clk_rpmrs_data_smd)

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/cm-11.0
#endif
