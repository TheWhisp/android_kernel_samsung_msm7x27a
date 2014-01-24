/*
 *  linux/arch/arm/mach-versatile/core.h
 *
 *  Copyright (C) 2004 ARM Limited
 *  Copyright (C) 2000 Deep Blue Solutions Ltd
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
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __ASM_ARCH_VERSATILE_H
#define __ASM_ARCH_VERSATILE_H

#include <linux/amba/bus.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/of_platform.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/of_platform.h>
#include <linux/reboot.h>
>>>>>>> refs/remotes/origin/master

extern void __init versatile_init(void);
extern void __init versatile_init_early(void);
extern void __init versatile_init_irq(void);
extern void __init versatile_map_io(void);
<<<<<<< HEAD
extern struct sys_timer versatile_timer;
<<<<<<< HEAD
extern unsigned int mmc_status(struct device *dev);

#define AMBA_DEVICE(name,busid,base,plat)			\
static struct amba_device name##_device = {			\
	.dev		= {					\
		.coherent_dma_mask = ~0,			\
		.init_name = busid,				\
		.platform_data = plat,				\
	},							\
	.res		= {					\
		.start	= VERSATILE_##base##_BASE,		\
		.end	= (VERSATILE_##base##_BASE) + SZ_4K - 1,\
		.flags	= IORESOURCE_MEM,			\
	},							\
	.dma_mask	= ~0,					\
	.irq		= base##_IRQ,				\
}
=======
extern void versatile_restart(char, const char *);
=======
extern void versatile_timer_init(void);
extern void versatile_restart(enum reboot_mode, const char *);
>>>>>>> refs/remotes/origin/master
extern unsigned int mmc_status(struct device *dev);
#ifdef CONFIG_OF
extern struct of_dev_auxdata versatile_auxdata_lookup[];
#endif

#define APB_DEVICE(name, busid, base, plat)	\
static AMBA_APB_DEVICE(name, busid, 0, VERSATILE_##base##_BASE, base##_IRQ, plat)

#define AHB_DEVICE(name, busid, base, plat)	\
static AMBA_AHB_DEVICE(name, busid, 0, VERSATILE_##base##_BASE, base##_IRQ, plat)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#endif
