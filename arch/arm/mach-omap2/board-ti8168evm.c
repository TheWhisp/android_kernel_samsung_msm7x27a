/*
<<<<<<< HEAD
 * Code for TI8168 EVM.
=======
 * Code for TI8168/TI8148 EVM.
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * Copyright (C) 2010 Texas Instruments, Inc. - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/kernel.h>
#include <linux/init.h>

#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include <plat/irqs.h>
#include <plat/board.h>
<<<<<<< HEAD
#include <plat/common.h>

static struct omap_board_config_kernel ti8168_evm_config[] __initdata = {
};

static void __init ti8168_init_early(void)
{
	omap2_init_common_infrastructure();
	omap2_init_common_devices(NULL, NULL);
}

static void __init ti8168_evm_init_irq(void)
{
	omap_init_irq();
}

static void __init ti8168_evm_init(void)
{
	omap_serial_init();
	omap_board_config = ti8168_evm_config;
	omap_board_config_size = ARRAY_SIZE(ti8168_evm_config);
}

static void __init ti8168_evm_map_io(void)
{
	omap2_set_globals_ti816x();
	omapti816x_map_common_io();
=======
#include "common.h"
#include <plat/usb.h>

static struct omap_musb_board_data musb_board_data = {
	.set_phy_power	= ti81xx_musb_phy_power,
	.interface_type	= MUSB_INTERFACE_ULPI,
	.mode           = MUSB_OTG,
	.power		= 500,
};

static struct omap_board_config_kernel ti81xx_evm_config[] __initdata = {
};

static void __init ti81xx_evm_init(void)
{
	omap_serial_init();
	omap_sdrc_init(NULL, NULL);
	omap_board_config = ti81xx_evm_config;
	omap_board_config_size = ARRAY_SIZE(ti81xx_evm_config);
	usb_musb_init(&musb_board_data);
>>>>>>> refs/remotes/origin/cm-10.0
}

MACHINE_START(TI8168EVM, "ti8168evm")
	/* Maintainer: Texas Instruments */
<<<<<<< HEAD
	.boot_params	= 0x80000100,
	.map_io		= ti8168_evm_map_io,
	.init_early	= ti8168_init_early,
	.init_irq	= ti8168_evm_init_irq,
	.timer		= &omap_timer,
	.init_machine	= ti8168_evm_init,
=======
	.atag_offset	= 0x100,
	.map_io		= ti81xx_map_io,
	.init_early	= ti81xx_init_early,
	.init_irq	= ti81xx_init_irq,
	.timer		= &omap3_timer,
	.init_machine	= ti81xx_evm_init,
	.restart	= omap_prcm_restart,
MACHINE_END

MACHINE_START(TI8148EVM, "ti8148evm")
	/* Maintainer: Texas Instruments */
	.atag_offset	= 0x100,
	.map_io		= ti81xx_map_io,
	.init_early	= ti81xx_init_early,
	.init_irq	= ti81xx_init_irq,
	.timer		= &omap3_timer,
	.init_machine	= ti81xx_evm_init,
	.restart	= omap_prcm_restart,
>>>>>>> refs/remotes/origin/cm-10.0
MACHINE_END
