/*
 * arch/arm/mach-ep93xx/simone.c
 * Simplemachines Sim.One support.
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (C) 2010 Ryan Mallon <ryan@bluewatersys.com>
=======
 * Copyright (C) 2010 Ryan Mallon
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright (C) 2010 Ryan Mallon
>>>>>>> refs/remotes/origin/master
 *
 * Based on the 2.6.24.7 support:
 *   Copyright (C) 2009 Simplemachines
 *   MMC support by Peter Ivanov <ivanovp@gmail.com>, 2007
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/gpio.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <linux/i2c.h>
#include <linux/i2c-gpio.h>

#include <mach/hardware.h>
<<<<<<< HEAD
#include <mach/fb.h>
<<<<<<< HEAD

#include <asm/mach-types.h>
#include <asm/mach/arch.h>

=======
#include <mach/gpio-ep93xx.h>

#include <asm/hardware/vic.h>
=======
#include <linux/platform_data/video-ep93xx.h>
#include <mach/gpio-ep93xx.h>

>>>>>>> refs/remotes/origin/master
#include <asm/mach-types.h>
#include <asm/mach/arch.h>

#include "soc.h"

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static struct ep93xx_eth_data __initdata simone_eth_data = {
	.phy_id		= 1,
};

static struct ep93xxfb_mach_info __initdata simone_fb_info = {
	.num_modes	= EP93XXFB_USE_MODEDB,
	.bpp		= 16,
	.flags		= EP93XXFB_USE_SDCSN0 | EP93XXFB_PCLK_FALLING,
};

static struct i2c_gpio_platform_data __initdata simone_i2c_gpio_data = {
	.sda_pin		= EP93XX_GPIO_LINE_EEDAT,
	.sda_is_open_drain	= 0,
	.scl_pin		= EP93XX_GPIO_LINE_EECLK,
	.scl_is_open_drain	= 0,
	.udelay			= 0,
	.timeout		= 0,
};

static struct i2c_board_info __initdata simone_i2c_board_info[] = {
	{
		I2C_BOARD_INFO("ds1337", 0x68),
	},
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static struct platform_device simone_audio_device = {
	.name		= "simone-audio",
	.id		= -1,
};

static void __init simone_register_audio(void)
{
	ep93xx_register_ac97();
	platform_device_register(&simone_audio_device);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static void __init simone_init_machine(void)
{
	ep93xx_init_devices();
	ep93xx_register_flash(2, EP93XX_CS6_PHYS_BASE, SZ_8M);
	ep93xx_register_eth(&simone_eth_data, 1);
	ep93xx_register_fb(&simone_fb_info);
	ep93xx_register_i2c(&simone_i2c_gpio_data, simone_i2c_board_info,
			    ARRAY_SIZE(simone_i2c_board_info));
<<<<<<< HEAD
<<<<<<< HEAD
	ep93xx_register_ac97();
}

MACHINE_START(SIM_ONE, "Simplemachines Sim.One Board")
/* Maintainer: Ryan Mallon <ryan@bluewatersys.com> */
	.boot_params	= EP93XX_SDCE0_PHYS_BASE + 0x100,
	.map_io		= ep93xx_map_io,
	.init_irq	= ep93xx_init_irq,
	.timer		= &ep93xx_timer,
	.init_machine	= simone_init_machine,
=======
=======
>>>>>>> refs/remotes/origin/master
	simone_register_audio();
}

MACHINE_START(SIM_ONE, "Simplemachines Sim.One Board")
	/* Maintainer: Ryan Mallon */
	.atag_offset	= 0x100,
	.map_io		= ep93xx_map_io,
	.init_irq	= ep93xx_init_irq,
<<<<<<< HEAD
	.handle_irq	= vic_handle_irq,
	.timer		= &ep93xx_timer,
	.init_machine	= simone_init_machine,
	.restart	= ep93xx_restart,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.init_time	= ep93xx_timer_init,
	.init_machine	= simone_init_machine,
	.init_late	= ep93xx_init_late,
	.restart	= ep93xx_restart,
>>>>>>> refs/remotes/origin/master
MACHINE_END
