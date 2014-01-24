/*
<<<<<<< HEAD
 * linux/arch/arm/mach-omap2/board-generic.c
 *
 * Copyright (C) 2005 Nokia Corporation
 * Author: Paul Mundt <paul.mundt@nokia.com>
 *
 * Modified from mach-omap/omap1/board-generic.c
 *
 * Code for generic OMAP2 board. Should work on many OMAP2 systems where
 * the bootloader passes the board-specific data to the kernel.
 * Do not put any board specific code to this file; create a new machine
 * type if you need custom low-level initializations.
=======
 * Copyright (C) 2005 Nokia Corporation
 * Author: Paul Mundt <paul.mundt@nokia.com>
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Modified from the original mach-omap/omap2/board-generic.c did by Paul
 * to support the OMAP2+ device tree boards with an unique board file.
>>>>>>> refs/remotes/origin/cm-10.0
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
<<<<<<< HEAD

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>

#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include <mach/gpio.h>
#include <plat/usb.h>
#include <plat/board.h>
#include <plat/common.h>

static struct omap_board_config_kernel generic_config[] = {
};

static void __init omap_generic_init_early(void)
{
	omap2_init_common_infrastructure();
	omap2_init_common_devices(NULL, NULL);
}

static void __init omap_generic_init(void)
{
	omap_serial_init();
	omap_board_config = generic_config;
	omap_board_config_size = ARRAY_SIZE(generic_config);
}

static void __init omap_generic_map_io(void)
{
	if (cpu_is_omap242x()) {
		omap2_set_globals_242x();
		omap242x_map_common_io();
	} else if (cpu_is_omap243x()) {
		omap2_set_globals_243x();
		omap243x_map_common_io();
	} else if (cpu_is_omap34xx()) {
		omap2_set_globals_3xxx();
		omap34xx_map_common_io();
	} else if (cpu_is_omap44xx()) {
		omap2_set_globals_443x();
		omap44xx_map_common_io();
	}
}

/* XXX This machine entry name should be updated */
MACHINE_START(OMAP_GENERIC, "Generic OMAP24xx")
	/* Maintainer: Paul Mundt <paul.mundt@nokia.com> */
	.boot_params	= 0x80000100,
	.reserve	= omap_reserve,
	.map_io		= omap_generic_map_io,
	.init_early	= omap_generic_init_early,
	.init_irq	= omap_init_irq,
	.init_machine	= omap_generic_init,
	.timer		= &omap_timer,
MACHINE_END
=======
#include <linux/io.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/irqdomain.h>
#include <linux/i2c/twl.h>

#include <mach/hardware.h>
#include <asm/hardware/gic.h>
#include <asm/mach/arch.h>

#include <plat/board.h>
#include "common.h"
#include "common-board-devices.h"

#if !(defined(CONFIG_ARCH_OMAP2) || defined(CONFIG_ARCH_OMAP3))
#define omap_intc_of_init	NULL
#endif
#ifndef CONFIG_ARCH_OMAP4
#define gic_of_init		NULL
#endif

static struct of_device_id irq_match[] __initdata = {
	{ .compatible = "ti,omap2-intc", .data = omap_intc_of_init, },
	{ .compatible = "arm,cortex-a9-gic", .data = gic_of_init, },
	{ }
};

static void __init omap_init_irq(void)
{
	of_irq_init(irq_match);
}

static struct of_device_id omap_dt_match_table[] __initdata = {
	{ .compatible = "simple-bus", },
	{ .compatible = "ti,omap-infra", },
	{ }
};

static void __init omap_generic_init(void)
{
	omap_sdrc_init(NULL, NULL);

	of_platform_populate(NULL, omap_dt_match_table, NULL, NULL);
}

#ifdef CONFIG_SOC_OMAP2420
static const char *omap242x_boards_compat[] __initdata = {
	"ti,omap2420",
	NULL,
};

DT_MACHINE_START(OMAP242X_DT, "Generic OMAP2420 (Flattened Device Tree)")
	.reserve	= omap_reserve,
	.map_io		= omap242x_map_io,
	.init_early	= omap2420_init_early,
	.init_irq	= omap_init_irq,
	.handle_irq	= omap2_intc_handle_irq,
	.init_machine	= omap_generic_init,
	.timer		= &omap2_timer,
	.dt_compat	= omap242x_boards_compat,
	.restart	= omap_prcm_restart,
MACHINE_END
#endif

#ifdef CONFIG_SOC_OMAP2430
static const char *omap243x_boards_compat[] __initdata = {
	"ti,omap2430",
	NULL,
};

DT_MACHINE_START(OMAP243X_DT, "Generic OMAP2430 (Flattened Device Tree)")
	.reserve	= omap_reserve,
	.map_io		= omap243x_map_io,
	.init_early	= omap2430_init_early,
	.init_irq	= omap_init_irq,
	.handle_irq	= omap2_intc_handle_irq,
	.init_machine	= omap_generic_init,
	.timer		= &omap2_timer,
	.dt_compat	= omap243x_boards_compat,
	.restart	= omap_prcm_restart,
MACHINE_END
#endif

#ifdef CONFIG_ARCH_OMAP3
static struct twl4030_platform_data beagle_twldata = {
	.irq_base	= TWL4030_IRQ_BASE,
	.irq_end	= TWL4030_IRQ_END,
};

static void __init omap3_i2c_init(void)
{
	omap3_pmic_init("twl4030", &beagle_twldata);
}

static void __init omap3_init(void)
{
	omap3_i2c_init();
	omap_generic_init();
}

static const char *omap3_boards_compat[] __initdata = {
	"ti,omap3",
	NULL,
};

DT_MACHINE_START(OMAP3_DT, "Generic OMAP3 (Flattened Device Tree)")
	.reserve	= omap_reserve,
	.map_io		= omap3_map_io,
	.init_early	= omap3430_init_early,
	.init_irq	= omap_init_irq,
	.handle_irq	= omap3_intc_handle_irq,
	.init_machine	= omap3_init,
	.timer		= &omap3_timer,
	.dt_compat	= omap3_boards_compat,
	.restart	= omap_prcm_restart,
MACHINE_END
#endif

#ifdef CONFIG_ARCH_OMAP4
static struct twl4030_platform_data sdp4430_twldata = {
	.irq_base	= TWL6030_IRQ_BASE,
	.irq_end	= TWL6030_IRQ_END,
};

static void __init omap4_i2c_init(void)
{
	omap4_pmic_init("twl6030", &sdp4430_twldata, NULL, 0);
}

static void __init omap4_init(void)
{
	omap4_i2c_init();
	omap_generic_init();
}

static const char *omap4_boards_compat[] __initdata = {
	"ti,omap4",
	NULL,
};

DT_MACHINE_START(OMAP4_DT, "Generic OMAP4 (Flattened Device Tree)")
	.reserve	= omap_reserve,
	.map_io		= omap4_map_io,
	.init_early	= omap4430_init_early,
	.init_irq	= omap_init_irq,
	.handle_irq	= gic_handle_irq,
	.init_machine	= omap4_init,
	.timer		= &omap4_timer,
	.dt_compat	= omap4_boards_compat,
	.restart	= omap_prcm_restart,
MACHINE_END
#endif
>>>>>>> refs/remotes/origin/cm-10.0
