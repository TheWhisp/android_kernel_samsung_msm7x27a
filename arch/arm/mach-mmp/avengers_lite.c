/*
 *  linux/arch/arm/mach-mmp/avengers_lite.c
 *
 *  Support for the Marvell PXA168-based Avengers lite Development Platform.
 *
 *  Copyright (C) 2009-2010 Marvell International Ltd.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  publishhed by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/kernel.h>
<<<<<<< HEAD
=======
#include <linux/gpio-pxa.h>
>>>>>>> refs/remotes/origin/master
#include <linux/platform_device.h>

#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <mach/addr-map.h>
#include <mach/mfp-pxa168.h>
#include <mach/pxa168.h>
#include <mach/irqs.h>


#include "common.h"
#include <linux/delay.h>

/* Avengers lite MFP configurations */
static unsigned long avengers_lite_pin_config_V16F[] __initdata = {
	/* DEBUG_UART */
	GPIO88_UART2_TXD,
	GPIO89_UART2_RXD,
};

<<<<<<< HEAD
=======
static struct pxa_gpio_platform_data pxa168_gpio_pdata = {
	.irq_base	= MMP_GPIO_TO_IRQ(0),
};

>>>>>>> refs/remotes/origin/master
static void __init avengers_lite_init(void)
{
	mfp_config(ARRAY_AND_SIZE(avengers_lite_pin_config_V16F));

	/* on-chip devices */
	pxa168_add_uart(2);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	platform_device_register(&pxa168_device_gpio);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	platform_device_add_data(&pxa168_device_gpio, &pxa168_gpio_pdata,
				 sizeof(struct pxa_gpio_platform_data));
	platform_device_register(&pxa168_device_gpio);
>>>>>>> refs/remotes/origin/master
}

MACHINE_START(AVENGERS_LITE, "PXA168 Avengers lite Development Platform")
	.map_io		= mmp_map_io,
<<<<<<< HEAD
<<<<<<< HEAD
	.init_irq       = pxa168_init_irq,
	.timer          = &pxa168_timer,
	.init_machine   = avengers_lite_init,
=======
	.nr_irqs	= MMP_NR_IRQS,
	.init_irq       = pxa168_init_irq,
	.timer          = &pxa168_timer,
	.init_machine   = avengers_lite_init,
	.restart	= pxa168_restart,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.nr_irqs	= MMP_NR_IRQS,
	.init_irq       = pxa168_init_irq,
	.init_time	= pxa168_timer_init,
	.init_machine   = avengers_lite_init,
	.restart	= pxa168_restart,
>>>>>>> refs/remotes/origin/master
MACHINE_END
