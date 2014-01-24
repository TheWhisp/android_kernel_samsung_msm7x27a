/*
 * Copyright 2007 Freescale Semiconductor, Inc. All Rights Reserved.
 * Copyright 2008 Juergen Beisert, kernel@pengutronix.de
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

/*
 * i.MX27 specific CPU detection code
 */

#include <linux/io.h>
#include <linux/module.h>

<<<<<<< HEAD
#include <mach/hardware.h>

<<<<<<< HEAD
static int cpu_silicon_rev = -1;
static int cpu_partnumber;

#define SYS_CHIP_ID             0x00    /* The offset of CHIP ID register */

static void query_silicon_parameter(void)
=======
=======
#include "hardware.h"

>>>>>>> refs/remotes/origin/master
static int mx27_cpu_rev = -1;
static int mx27_cpu_partnumber;

#define SYS_CHIP_ID             0x00    /* The offset of CHIP ID register */

static int mx27_read_cpu_rev(void)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	u32 val;
	/*
	 * now we have access to the IO registers. As we need
	 * the silicon revision very early we read it here to
	 * avoid any further hooks
	*/
	val = __raw_readl(MX27_IO_ADDRESS(MX27_SYSCTRL_BASE_ADDR
				+ SYS_CHIP_ID));

<<<<<<< HEAD
<<<<<<< HEAD
	switch (val >> 28) {
	case 0:
		cpu_silicon_rev = IMX_CHIP_REVISION_1_0;
		break;
	case 1:
		cpu_silicon_rev = IMX_CHIP_REVISION_2_0;
		break;
	case 2:
		cpu_silicon_rev = IMX_CHIP_REVISION_2_1;
		break;
	default:
		cpu_silicon_rev = IMX_CHIP_REVISION_UNKNOWN;
	}
	cpu_partnumber = (int)((val >> 12) & 0xFFFF);
=======
=======
>>>>>>> refs/remotes/origin/master
	mx27_cpu_partnumber = (int)((val >> 12) & 0xFFFF);

	switch (val >> 28) {
	case 0:
		return IMX_CHIP_REVISION_1_0;
	case 1:
		return IMX_CHIP_REVISION_2_0;
	case 2:
		return IMX_CHIP_REVISION_2_1;
	default:
		return IMX_CHIP_REVISION_UNKNOWN;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

/*
 * Returns:
 *	the silicon revision of the cpu
 *	-EINVAL - not a mx27
 */
int mx27_revision(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	if (cpu_silicon_rev == -1)
		query_silicon_parameter();

	if (cpu_partnumber != 0x8821)
		return -EINVAL;

	return cpu_silicon_rev;
=======
=======
>>>>>>> refs/remotes/origin/master
	if (mx27_cpu_rev == -1)
		mx27_cpu_rev = mx27_read_cpu_rev();

	if (mx27_cpu_partnumber != 0x8821)
		return -EINVAL;

	return mx27_cpu_rev;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL(mx27_revision);
