<<<<<<< HEAD
<<<<<<< HEAD
/* Copyright (c) 2010, The Linux Foundation. All rights reserved.
=======
/* Copyright (c) 2010, Code Aurora Forum. All rights reserved.
>>>>>>> refs/remotes/origin/master
=======
/* Copyright (c) 2010, The Linux Foundation. All rights reserved.
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
<<<<<<< HEAD
<<<<<<< HEAD
 */
#include <linux/module.h>
#include <mach/irqs.h>
#include <mach/gpiomux.h>

static int __init gpiomux_init(void)
{
	return msm_gpiomux_init(NR_GPIO_IRQS);
}
postcore_initcall(gpiomux_init);
=======
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
=======
>>>>>>> refs/remotes/origin/cm-11.0
 */
#include <linux/module.h>
#include <mach/irqs.h>
#include <mach/gpiomux.h>

<<<<<<< HEAD
struct msm_gpiomux_config msm_gpiomux_configs[GPIOMUX_NGPIOS] = {
	[86] = { /* UART3 RX */
		.suspended = GPIOMUX_DRV_2MA | GPIOMUX_PULL_DOWN |
			     GPIOMUX_FUNC_1 | GPIOMUX_VALID,
	},
	[87] = { /* UART3 TX */
		.suspended = GPIOMUX_DRV_2MA | GPIOMUX_PULL_DOWN |
			     GPIOMUX_FUNC_1 | GPIOMUX_VALID,
	},
	/* SDC1 data[3:0] & CMD */
	[51 ... 55] = {
		.active = SDCC_DAT_0_3_CMD_ACTV_CFG,
		.suspended = SDC1_SUSPEND_CONFIG
	},
	/* SDC1 CLK */
	[56] = {
		.active = SDCC_CLK_ACTV_CFG,
		.suspended = SDC1_SUSPEND_CONFIG
	},
};
>>>>>>> refs/remotes/origin/master
=======
static int __init gpiomux_init(void)
{
	return msm_gpiomux_init(NR_GPIO_IRQS);
}
postcore_initcall(gpiomux_init);
>>>>>>> refs/remotes/origin/cm-11.0
