/*
 * arch/arm/mach-ep93xx/adssphere.c
 * ADS Sphere support.
 *
 * Copyright (C) 2006 Lennert Buytenhek <buytenh@wantstofly.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
<<<<<<< HEAD

#include <mach/hardware.h>

<<<<<<< HEAD
#include <asm/mach-types.h>
#include <asm/mach/arch.h>

=======
#include <asm/hardware/vic.h>
=======
#include <linux/sizes.h>

#include <mach/hardware.h>

>>>>>>> refs/remotes/origin/master
#include <asm/mach-types.h>
#include <asm/mach/arch.h>

#include "soc.h"
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

static struct ep93xx_eth_data __initdata adssphere_eth_data = {
	.phy_id		= 1,
};

static void __init adssphere_init_machine(void)
{
	ep93xx_init_devices();
	ep93xx_register_flash(4, EP93XX_CS6_PHYS_BASE, SZ_32M);
	ep93xx_register_eth(&adssphere_eth_data, 1);
}

MACHINE_START(ADSSPHERE, "ADS Sphere board")
	/* Maintainer: Lennert Buytenhek <buytenh@wantstofly.org> */
<<<<<<< HEAD
<<<<<<< HEAD
	.boot_params	= EP93XX_SDCE3_PHYS_BASE_SYNC + 0x100,
	.map_io		= ep93xx_map_io,
	.init_irq	= ep93xx_init_irq,
	.timer		= &ep93xx_timer,
	.init_machine	= adssphere_init_machine,
=======
	.atag_offset	= 0x100,
	.map_io		= ep93xx_map_io,
	.init_irq	= ep93xx_init_irq,
	.handle_irq	= vic_handle_irq,
	.timer		= &ep93xx_timer,
	.init_machine	= adssphere_init_machine,
	.restart	= ep93xx_restart,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.atag_offset	= 0x100,
	.map_io		= ep93xx_map_io,
	.init_irq	= ep93xx_init_irq,
	.init_time	= ep93xx_timer_init,
	.init_machine	= adssphere_init_machine,
	.init_late	= ep93xx_init_late,
	.restart	= ep93xx_restart,
>>>>>>> refs/remotes/origin/master
MACHINE_END
