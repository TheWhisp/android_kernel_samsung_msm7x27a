/*
 * Copyright (C) 1999 ARM Limited
 * Copyright (C) 2000 Deep Blue Solutions Ltd
 * Copyright 2006-2007 Freescale Semiconductor, Inc. All Rights Reserved.
 * Copyright 2008 Juergen Beisert, kernel@pengutronix.de
 * Copyright 2009 Ilya Yanok, Emcraft Systems Ltd, yanok@emcraft.com
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
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/err.h>
#include <linux/delay.h>
<<<<<<< HEAD

#include <mach/hardware.h>
#include <mach/common.h>
#include <asm/proc-fns.h>
#include <asm/system.h>
#include <asm/mach-types.h>

=======
#include <linux/module.h>

#include <mach/hardware.h>
#include <mach/common.h>
#include <asm/system_misc.h>
#include <asm/proc-fns.h>
#include <asm/mach-types.h>

void __iomem *(*imx_ioremap)(unsigned long, size_t, unsigned int) = NULL;
EXPORT_SYMBOL_GPL(imx_ioremap);

>>>>>>> refs/remotes/origin/cm-10.0
static void __iomem *wdog_base;

/*
 * Reset the system. It is called by machine_restart().
 */
<<<<<<< HEAD
void arch_reset(char mode, const char *cmd)
{
	unsigned int wcr_enable;

#ifdef CONFIG_MACH_MX51_EFIKAMX
	if (machine_is_mx51_efikamx()) {
		mx51_efikamx_reset();
		return;
	}
#endif

=======
void mxc_restart(char mode, const char *cmd)
{
	unsigned int wcr_enable;

>>>>>>> refs/remotes/origin/cm-10.0
	if (cpu_is_mx1()) {
		wcr_enable = (1 << 0);
	} else {
		struct clk *clk;

		clk = clk_get_sys("imx2-wdt.0", NULL);
		if (!IS_ERR(clk))
<<<<<<< HEAD
			clk_enable(clk);
=======
			clk_prepare_enable(clk);
>>>>>>> refs/remotes/origin/cm-10.0
		wcr_enable = (1 << 2);
	}

	/* Assert SRS signal */
	__raw_writew(wcr_enable, wdog_base);

	/* wait for reset to assert... */
	mdelay(500);

	printk(KERN_ERR "Watchdog reset failed to assert reset\n");

	/* delay to allow the serial port to show the message */
	mdelay(50);

	/* we'll take a jump through zero as a poor second */
<<<<<<< HEAD
	cpu_reset(0);
=======
	soft_restart(0);
>>>>>>> refs/remotes/origin/cm-10.0
}

void mxc_arch_reset_init(void __iomem *base)
{
	wdog_base = base;
}
