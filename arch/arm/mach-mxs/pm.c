/*
 * Copyright (C) 2010 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/suspend.h>
#include <linux/io.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <mach/system.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include "pm.h"
>>>>>>> refs/remotes/origin/master

static int mxs_suspend_enter(suspend_state_t state)
{
	switch (state) {
	case PM_SUSPEND_MEM:
<<<<<<< HEAD
<<<<<<< HEAD
		arch_idle();
=======
		cpu_do_idle();
>>>>>>> refs/remotes/origin/cm-10.0
=======
		cpu_do_idle();
>>>>>>> refs/remotes/origin/master
		break;

	default:
		return -EINVAL;
	}
	return 0;
}

static struct platform_suspend_ops mxs_suspend_ops = {
	.enter = mxs_suspend_enter,
	.valid = suspend_valid_only_mem,
};

<<<<<<< HEAD
static int __init mxs_pm_init(void)
{
	suspend_set_ops(&mxs_suspend_ops);
	return 0;
}
device_initcall(mxs_pm_init);
=======
void __init mxs_pm_init(void)
{
	suspend_set_ops(&mxs_suspend_ops);
}
>>>>>>> refs/remotes/origin/master
