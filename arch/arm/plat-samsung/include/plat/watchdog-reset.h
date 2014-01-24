/* arch/arm/plat-s3c/include/plat/watchdog-reset.h
 *
 * Copyright (c) 2008 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * S3C2410 - System define for arch_reset() function
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <plat/clock.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <plat/regs-watchdog.h>
#include <mach/map.h>

#include <linux/clk.h>
#include <linux/err.h>
#include <linux/io.h>
<<<<<<< HEAD

static inline void arch_wdt_reset(void)
{
	struct clk *wdtclk;

=======
#include <linux/delay.h>

static inline void arch_wdt_reset(void)
{
>>>>>>> refs/remotes/origin/cm-10.0
	printk("arch_reset: attempting watchdog reset\n");

	__raw_writel(0, S3C2410_WTCON);	  /* disable watchdog, to be safe  */

<<<<<<< HEAD
	wdtclk = clk_get(NULL, "watchdog");
	if (!IS_ERR(wdtclk)) {
		clk_enable(wdtclk);
	} else
		printk(KERN_WARNING "%s: warning: cannot get watchdog clock\n", __func__);
=======
	if (!IS_ERR(s3c2410_wdtclk))
		clk_enable(s3c2410_wdtclk);
>>>>>>> refs/remotes/origin/cm-10.0

	/* put initial values into count and data */
	__raw_writel(0x80, S3C2410_WTCNT);
	__raw_writel(0x80, S3C2410_WTDAT);

	/* set the watchdog to go and reset... */
	__raw_writel(S3C2410_WTCON_ENABLE|S3C2410_WTCON_DIV16|S3C2410_WTCON_RSTEN |
		     S3C2410_WTCON_PRESCALE(0x20), S3C2410_WTCON);

	/* wait for reset to assert... */
	mdelay(500);

	printk(KERN_ERR "Watchdog reset failed to assert reset\n");

	/* delay to allow the serial port to show the message */
	mdelay(50);
}
=======
#ifndef __PLAT_SAMSUNG_WATCHDOG_RESET_H
#define __PLAT_SAMSUNG_WATCHDOG_RESET_H

extern void samsung_wdt_reset(void);
extern void samsung_wdt_reset_of_init(void);
extern void samsung_wdt_reset_init(void __iomem *base);

#endif /* __PLAT_SAMSUNG_WATCHDOG_RESET_H */
>>>>>>> refs/remotes/origin/master
