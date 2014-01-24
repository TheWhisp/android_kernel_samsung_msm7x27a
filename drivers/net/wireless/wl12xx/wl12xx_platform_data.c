<<<<<<< HEAD
=======
/*
 * This file is part of wl12xx
 *
 * Copyright (C) 2010-2011 Texas Instruments, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/module.h>
#include <linux/err.h>
#include <linux/wl12xx.h>

<<<<<<< HEAD
static const struct wl12xx_platform_data *platform_data;
=======
static struct wl12xx_platform_data *platform_data;
>>>>>>> refs/remotes/origin/cm-10.0

int __init wl12xx_set_platform_data(const struct wl12xx_platform_data *data)
{
	if (platform_data)
		return -EBUSY;
	if (!data)
		return -EINVAL;

	platform_data = kmemdup(data, sizeof(*data), GFP_KERNEL);
	if (!platform_data)
		return -ENOMEM;

	return 0;
}

<<<<<<< HEAD
const struct wl12xx_platform_data *wl12xx_get_platform_data(void)
=======
struct wl12xx_platform_data *wl12xx_get_platform_data(void)
>>>>>>> refs/remotes/origin/cm-10.0
{
	if (!platform_data)
		return ERR_PTR(-ENODEV);

	return platform_data;
}
EXPORT_SYMBOL(wl12xx_get_platform_data);
