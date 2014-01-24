<<<<<<< HEAD
/* linux/arch/arm/mach-msm/gpio.c
 *
=======
/*
>>>>>>> refs/remotes/origin/master
 * Copyright (C) 2007 Google, Inc.
 * Copyright (c) 2009-2012, The Linux Foundation. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/bitops.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/platform_device.h>
#include <asm/mach/irq.h>
#include <mach/gpiomux.h>
#include <mach/msm_iomap.h>
#include <mach/msm_smsm.h>
#include <mach/proc_comm.h>

=======
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/err.h>

#include <mach/msm_gpiomux.h>
>>>>>>> refs/remotes/origin/master

/* see 80-VA736-2 Rev C pp 695-751
**
** These are actually the *shadow* gpio registers, since the
** real ones (which allow full access) are only available to the
** ARM9 side of the world.
**
** Since the _BASE need to be page-aligned when we're mapping them
** to virtual addresses, adjust for the additional offset in these
** macros.
*/

<<<<<<< HEAD
#if defined(CONFIG_ARCH_MSM7X30)
#define MSM_GPIO1_REG(off) (MSM_GPIO1_BASE + (off))
#define MSM_GPIO2_REG(off) (MSM_GPIO2_BASE + 0x400 + (off))
#else
#define MSM_GPIO1_REG(off) (MSM_GPIO1_BASE + 0x800 + (off))
#define MSM_GPIO2_REG(off) (MSM_GPIO2_BASE + 0xC00 + (off))
#endif

#if defined(CONFIG_ARCH_MSM7X00A) || defined(CONFIG_ARCH_MSM7X25) ||\
    defined(CONFIG_ARCH_MSM7X27)

/* output value */
#define MSM_GPIO_OUT_0         MSM_GPIO1_REG(0x00)  /* gpio  15-0  */
#define MSM_GPIO_OUT_1         MSM_GPIO2_REG(0x00)  /* gpio  42-16 */
#define MSM_GPIO_OUT_2         MSM_GPIO1_REG(0x04)  /* gpio  67-43 */
#define MSM_GPIO_OUT_3         MSM_GPIO1_REG(0x08)  /* gpio  94-68 */
#define MSM_GPIO_OUT_4         MSM_GPIO1_REG(0x0C)  /* gpio 106-95 */
#define MSM_GPIO_OUT_5         MSM_GPIO1_REG(0x50)  /* gpio 107-121 */

/* same pin map as above, output enable */
#define MSM_GPIO_OE_0          MSM_GPIO1_REG(0x10)
#define MSM_GPIO_OE_1          MSM_GPIO2_REG(0x08)
#define MSM_GPIO_OE_2          MSM_GPIO1_REG(0x14)
#define MSM_GPIO_OE_3          MSM_GPIO1_REG(0x18)
#define MSM_GPIO_OE_4          MSM_GPIO1_REG(0x1C)
#define MSM_GPIO_OE_5          MSM_GPIO1_REG(0x54)

/* same pin map as above, input read */
#define MSM_GPIO_IN_0          MSM_GPIO1_REG(0x34)
#define MSM_GPIO_IN_1          MSM_GPIO2_REG(0x20)
#define MSM_GPIO_IN_2          MSM_GPIO1_REG(0x38)
#define MSM_GPIO_IN_3          MSM_GPIO1_REG(0x3C)
#define MSM_GPIO_IN_4          MSM_GPIO1_REG(0x40)
#define MSM_GPIO_IN_5          MSM_GPIO1_REG(0x44)

/* same pin map as above, 1=edge 0=level interrup */
#define MSM_GPIO_INT_EDGE_0    MSM_GPIO1_REG(0x60)
#define MSM_GPIO_INT_EDGE_1    MSM_GPIO2_REG(0x50)
#define MSM_GPIO_INT_EDGE_2    MSM_GPIO1_REG(0x64)
#define MSM_GPIO_INT_EDGE_3    MSM_GPIO1_REG(0x68)
#define MSM_GPIO_INT_EDGE_4    MSM_GPIO1_REG(0x6C)
#define MSM_GPIO_INT_EDGE_5    MSM_GPIO1_REG(0xC0)

/* same pin map as above, 1=positive 0=negative */
#define MSM_GPIO_INT_POS_0     MSM_GPIO1_REG(0x70)
#define MSM_GPIO_INT_POS_1     MSM_GPIO2_REG(0x58)
#define MSM_GPIO_INT_POS_2     MSM_GPIO1_REG(0x74)
#define MSM_GPIO_INT_POS_3     MSM_GPIO1_REG(0x78)
#define MSM_GPIO_INT_POS_4     MSM_GPIO1_REG(0x7C)
#define MSM_GPIO_INT_POS_5     MSM_GPIO1_REG(0xBC)

/* same pin map as above, interrupt enable */
#define MSM_GPIO_INT_EN_0      MSM_GPIO1_REG(0x80)
#define MSM_GPIO_INT_EN_1      MSM_GPIO2_REG(0x60)
#define MSM_GPIO_INT_EN_2      MSM_GPIO1_REG(0x84)
#define MSM_GPIO_INT_EN_3      MSM_GPIO1_REG(0x88)
#define MSM_GPIO_INT_EN_4      MSM_GPIO1_REG(0x8C)
#define MSM_GPIO_INT_EN_5      MSM_GPIO1_REG(0xB8)

/* same pin map as above, write 1 to clear interrupt */
#define MSM_GPIO_INT_CLEAR_0   MSM_GPIO1_REG(0x90)
#define MSM_GPIO_INT_CLEAR_1   MSM_GPIO2_REG(0x68)
#define MSM_GPIO_INT_CLEAR_2   MSM_GPIO1_REG(0x94)
#define MSM_GPIO_INT_CLEAR_3   MSM_GPIO1_REG(0x98)
#define MSM_GPIO_INT_CLEAR_4   MSM_GPIO1_REG(0x9C)
#define MSM_GPIO_INT_CLEAR_5   MSM_GPIO1_REG(0xB4)

/* same pin map as above, 1=interrupt pending */
#define MSM_GPIO_INT_STATUS_0  MSM_GPIO1_REG(0xA0)
#define MSM_GPIO_INT_STATUS_1  MSM_GPIO2_REG(0x70)
#define MSM_GPIO_INT_STATUS_2  MSM_GPIO1_REG(0xA4)
#define MSM_GPIO_INT_STATUS_3  MSM_GPIO1_REG(0xA8)
#define MSM_GPIO_INT_STATUS_4  MSM_GPIO1_REG(0xAC)
#define MSM_GPIO_INT_STATUS_5  MSM_GPIO1_REG(0xB0)

#endif

#if defined(CONFIG_ARCH_MSM7X30)

/* output value */
#define MSM_GPIO_OUT_0         MSM_GPIO1_REG(0x00)   /* gpio  15-0   */
#define MSM_GPIO_OUT_1         MSM_GPIO2_REG(0x00)   /* gpio  43-16  */
#define MSM_GPIO_OUT_2         MSM_GPIO1_REG(0x04)   /* gpio  67-44  */
#define MSM_GPIO_OUT_3         MSM_GPIO1_REG(0x08)   /* gpio  94-68  */
#define MSM_GPIO_OUT_4         MSM_GPIO1_REG(0x0C)   /* gpio 106-95  */
#define MSM_GPIO_OUT_5         MSM_GPIO1_REG(0x50)   /* gpio 133-107 */
#define MSM_GPIO_OUT_6         MSM_GPIO1_REG(0xC4)   /* gpio 150-134 */
#define MSM_GPIO_OUT_7         MSM_GPIO1_REG(0x214)  /* gpio 181-151 */

/* same pin map as above, output enable */
#define MSM_GPIO_OE_0          MSM_GPIO1_REG(0x10)
#define MSM_GPIO_OE_1          MSM_GPIO2_REG(0x08)
#define MSM_GPIO_OE_2          MSM_GPIO1_REG(0x14)
#define MSM_GPIO_OE_3          MSM_GPIO1_REG(0x18)
#define MSM_GPIO_OE_4          MSM_GPIO1_REG(0x1C)
#define MSM_GPIO_OE_5          MSM_GPIO1_REG(0x54)
#define MSM_GPIO_OE_6          MSM_GPIO1_REG(0xC8)
#define MSM_GPIO_OE_7          MSM_GPIO1_REG(0x218)

/* same pin map as above, input read */
#define MSM_GPIO_IN_0          MSM_GPIO1_REG(0x34)
#define MSM_GPIO_IN_1          MSM_GPIO2_REG(0x20)
#define MSM_GPIO_IN_2          MSM_GPIO1_REG(0x38)
#define MSM_GPIO_IN_3          MSM_GPIO1_REG(0x3C)
#define MSM_GPIO_IN_4          MSM_GPIO1_REG(0x40)
#define MSM_GPIO_IN_5          MSM_GPIO1_REG(0x44)
#define MSM_GPIO_IN_6          MSM_GPIO1_REG(0xCC)
#define MSM_GPIO_IN_7          MSM_GPIO1_REG(0x21C)

/* same pin map as above, 1=edge 0=level interrup */
#define MSM_GPIO_INT_EDGE_0    MSM_GPIO1_REG(0x60)
#define MSM_GPIO_INT_EDGE_1    MSM_GPIO2_REG(0x50)
#define MSM_GPIO_INT_EDGE_2    MSM_GPIO1_REG(0x64)
#define MSM_GPIO_INT_EDGE_3    MSM_GPIO1_REG(0x68)
#define MSM_GPIO_INT_EDGE_4    MSM_GPIO1_REG(0x6C)
#define MSM_GPIO_INT_EDGE_5    MSM_GPIO1_REG(0xC0)
#define MSM_GPIO_INT_EDGE_6    MSM_GPIO1_REG(0xD0)
#define MSM_GPIO_INT_EDGE_7    MSM_GPIO1_REG(0x240)

/* same pin map as above, 1=positive 0=negative */
#define MSM_GPIO_INT_POS_0     MSM_GPIO1_REG(0x70)
#define MSM_GPIO_INT_POS_1     MSM_GPIO2_REG(0x58)
#define MSM_GPIO_INT_POS_2     MSM_GPIO1_REG(0x74)
#define MSM_GPIO_INT_POS_3     MSM_GPIO1_REG(0x78)
#define MSM_GPIO_INT_POS_4     MSM_GPIO1_REG(0x7C)
#define MSM_GPIO_INT_POS_5     MSM_GPIO1_REG(0xBC)
#define MSM_GPIO_INT_POS_6     MSM_GPIO1_REG(0xD4)
#define MSM_GPIO_INT_POS_7     MSM_GPIO1_REG(0x228)

/* same pin map as above, interrupt enable */
#define MSM_GPIO_INT_EN_0      MSM_GPIO1_REG(0x80)
#define MSM_GPIO_INT_EN_1      MSM_GPIO2_REG(0x60)
#define MSM_GPIO_INT_EN_2      MSM_GPIO1_REG(0x84)
#define MSM_GPIO_INT_EN_3      MSM_GPIO1_REG(0x88)
#define MSM_GPIO_INT_EN_4      MSM_GPIO1_REG(0x8C)
#define MSM_GPIO_INT_EN_5      MSM_GPIO1_REG(0xB8)
#define MSM_GPIO_INT_EN_6      MSM_GPIO1_REG(0xD8)
#define MSM_GPIO_INT_EN_7      MSM_GPIO1_REG(0x22C)

/* same pin map as above, write 1 to clear interrupt */
#define MSM_GPIO_INT_CLEAR_0   MSM_GPIO1_REG(0x90)
#define MSM_GPIO_INT_CLEAR_1   MSM_GPIO2_REG(0x68)
#define MSM_GPIO_INT_CLEAR_2   MSM_GPIO1_REG(0x94)
#define MSM_GPIO_INT_CLEAR_3   MSM_GPIO1_REG(0x98)
#define MSM_GPIO_INT_CLEAR_4   MSM_GPIO1_REG(0x9C)
#define MSM_GPIO_INT_CLEAR_5   MSM_GPIO1_REG(0xB4)
#define MSM_GPIO_INT_CLEAR_6   MSM_GPIO1_REG(0xDC)
#define MSM_GPIO_INT_CLEAR_7   MSM_GPIO1_REG(0x230)

/* same pin map as above, 1=interrupt pending */
#define MSM_GPIO_INT_STATUS_0  MSM_GPIO1_REG(0xA0)
#define MSM_GPIO_INT_STATUS_1  MSM_GPIO2_REG(0x70)
#define MSM_GPIO_INT_STATUS_2  MSM_GPIO1_REG(0xA4)
#define MSM_GPIO_INT_STATUS_3  MSM_GPIO1_REG(0xA8)
#define MSM_GPIO_INT_STATUS_4  MSM_GPIO1_REG(0xAC)
#define MSM_GPIO_INT_STATUS_5  MSM_GPIO1_REG(0xB0)
#define MSM_GPIO_INT_STATUS_6  MSM_GPIO1_REG(0xE0)
#define MSM_GPIO_INT_STATUS_7  MSM_GPIO1_REG(0x234)

#endif

enum {
	GPIO_DEBUG_SLEEP = 1U << 0,
};
static int msm_gpio_debug_mask;
module_param_named(debug_mask, msm_gpio_debug_mask, int,
		   S_IRUGO | S_IWUSR | S_IWGRP);

#define FIRST_GPIO_IRQ MSM_GPIO_TO_INT(0)

#define MSM_GPIO_BANK(bank, first, last)				\
	{								\
		.regs = {						\
			.out =         MSM_GPIO_OUT_##bank,		\
			.in =          MSM_GPIO_IN_##bank,		\
			.int_status =  MSM_GPIO_INT_STATUS_##bank,	\
			.int_clear =   MSM_GPIO_INT_CLEAR_##bank,	\
			.int_en =      MSM_GPIO_INT_EN_##bank,		\
			.int_edge =    MSM_GPIO_INT_EDGE_##bank,	\
			.int_pos =     MSM_GPIO_INT_POS_##bank,		\
			.oe =          MSM_GPIO_OE_##bank,		\
		},							\
=======
#define MSM_GPIO1_REG(off) (off)
#define MSM_GPIO2_REG(off) (off)
#define MSM_GPIO1_SHADOW_REG(off) (off)
#define MSM_GPIO2_SHADOW_REG(off) (off)

/*
 * MSM7X00 registers
 */
/* output value */
#define MSM7X00_GPIO_OUT_0	MSM_GPIO1_SHADOW_REG(0x00)  /* gpio  15-0  */
#define MSM7X00_GPIO_OUT_1	MSM_GPIO2_SHADOW_REG(0x00)  /* gpio  42-16 */
#define MSM7X00_GPIO_OUT_2	MSM_GPIO1_SHADOW_REG(0x04)  /* gpio  67-43 */
#define MSM7X00_GPIO_OUT_3	MSM_GPIO1_SHADOW_REG(0x08)  /* gpio  94-68 */
#define MSM7X00_GPIO_OUT_4	MSM_GPIO1_SHADOW_REG(0x0C)  /* gpio 106-95 */
#define MSM7X00_GPIO_OUT_5	MSM_GPIO1_SHADOW_REG(0x50)  /* gpio 107-121 */

/* same pin map as above, output enable */
#define MSM7X00_GPIO_OE_0	MSM_GPIO1_SHADOW_REG(0x10)
#define MSM7X00_GPIO_OE_1	MSM_GPIO2_SHADOW_REG(0x08)
#define MSM7X00_GPIO_OE_2	MSM_GPIO1_SHADOW_REG(0x14)
#define MSM7X00_GPIO_OE_3	MSM_GPIO1_SHADOW_REG(0x18)
#define MSM7X00_GPIO_OE_4	MSM_GPIO1_SHADOW_REG(0x1C)
#define MSM7X00_GPIO_OE_5	MSM_GPIO1_SHADOW_REG(0x54)

/* same pin map as above, input read */
#define MSM7X00_GPIO_IN_0	MSM_GPIO1_SHADOW_REG(0x34)
#define MSM7X00_GPIO_IN_1	MSM_GPIO2_SHADOW_REG(0x20)
#define MSM7X00_GPIO_IN_2	MSM_GPIO1_SHADOW_REG(0x38)
#define MSM7X00_GPIO_IN_3	MSM_GPIO1_SHADOW_REG(0x3C)
#define MSM7X00_GPIO_IN_4	MSM_GPIO1_SHADOW_REG(0x40)
#define MSM7X00_GPIO_IN_5	MSM_GPIO1_SHADOW_REG(0x44)

/* same pin map as above, 1=edge 0=level interrup */
#define MSM7X00_GPIO_INT_EDGE_0	MSM_GPIO1_SHADOW_REG(0x60)
#define MSM7X00_GPIO_INT_EDGE_1	MSM_GPIO2_SHADOW_REG(0x50)
#define MSM7X00_GPIO_INT_EDGE_2	MSM_GPIO1_SHADOW_REG(0x64)
#define MSM7X00_GPIO_INT_EDGE_3	MSM_GPIO1_SHADOW_REG(0x68)
#define MSM7X00_GPIO_INT_EDGE_4	MSM_GPIO1_SHADOW_REG(0x6C)
#define MSM7X00_GPIO_INT_EDGE_5	MSM_GPIO1_SHADOW_REG(0xC0)

/* same pin map as above, 1=positive 0=negative */
#define MSM7X00_GPIO_INT_POS_0	MSM_GPIO1_SHADOW_REG(0x70)
#define MSM7X00_GPIO_INT_POS_1	MSM_GPIO2_SHADOW_REG(0x58)
#define MSM7X00_GPIO_INT_POS_2	MSM_GPIO1_SHADOW_REG(0x74)
#define MSM7X00_GPIO_INT_POS_3	MSM_GPIO1_SHADOW_REG(0x78)
#define MSM7X00_GPIO_INT_POS_4	MSM_GPIO1_SHADOW_REG(0x7C)
#define MSM7X00_GPIO_INT_POS_5	MSM_GPIO1_SHADOW_REG(0xBC)

/* same pin map as above, interrupt enable */
#define MSM7X00_GPIO_INT_EN_0	MSM_GPIO1_SHADOW_REG(0x80)
#define MSM7X00_GPIO_INT_EN_1	MSM_GPIO2_SHADOW_REG(0x60)
#define MSM7X00_GPIO_INT_EN_2	MSM_GPIO1_SHADOW_REG(0x84)
#define MSM7X00_GPIO_INT_EN_3	MSM_GPIO1_SHADOW_REG(0x88)
#define MSM7X00_GPIO_INT_EN_4	MSM_GPIO1_SHADOW_REG(0x8C)
#define MSM7X00_GPIO_INT_EN_5	MSM_GPIO1_SHADOW_REG(0xB8)

/* same pin map as above, write 1 to clear interrupt */
#define MSM7X00_GPIO_INT_CLEAR_0	MSM_GPIO1_SHADOW_REG(0x90)
#define MSM7X00_GPIO_INT_CLEAR_1	MSM_GPIO2_SHADOW_REG(0x68)
#define MSM7X00_GPIO_INT_CLEAR_2	MSM_GPIO1_SHADOW_REG(0x94)
#define MSM7X00_GPIO_INT_CLEAR_3	MSM_GPIO1_SHADOW_REG(0x98)
#define MSM7X00_GPIO_INT_CLEAR_4	MSM_GPIO1_SHADOW_REG(0x9C)
#define MSM7X00_GPIO_INT_CLEAR_5	MSM_GPIO1_SHADOW_REG(0xB4)

/* same pin map as above, 1=interrupt pending */
#define MSM7X00_GPIO_INT_STATUS_0	MSM_GPIO1_SHADOW_REG(0xA0)
#define MSM7X00_GPIO_INT_STATUS_1	MSM_GPIO2_SHADOW_REG(0x70)
#define MSM7X00_GPIO_INT_STATUS_2	MSM_GPIO1_SHADOW_REG(0xA4)
#define MSM7X00_GPIO_INT_STATUS_3	MSM_GPIO1_SHADOW_REG(0xA8)
#define MSM7X00_GPIO_INT_STATUS_4	MSM_GPIO1_SHADOW_REG(0xAC)
#define MSM7X00_GPIO_INT_STATUS_5	MSM_GPIO1_SHADOW_REG(0xB0)

/*
 * QSD8X50 registers
 */
/* output value */
#define QSD8X50_GPIO_OUT_0	MSM_GPIO1_SHADOW_REG(0x00)  /* gpio  15-0   */
#define QSD8X50_GPIO_OUT_1	MSM_GPIO2_SHADOW_REG(0x00)  /* gpio  42-16  */
#define QSD8X50_GPIO_OUT_2	MSM_GPIO1_SHADOW_REG(0x04)  /* gpio  67-43  */
#define QSD8X50_GPIO_OUT_3	MSM_GPIO1_SHADOW_REG(0x08)  /* gpio  94-68  */
#define QSD8X50_GPIO_OUT_4	MSM_GPIO1_SHADOW_REG(0x0C)  /* gpio 103-95  */
#define QSD8X50_GPIO_OUT_5	MSM_GPIO1_SHADOW_REG(0x10)  /* gpio 121-104 */
#define QSD8X50_GPIO_OUT_6	MSM_GPIO1_SHADOW_REG(0x14)  /* gpio 152-122 */
#define QSD8X50_GPIO_OUT_7	MSM_GPIO1_SHADOW_REG(0x18)  /* gpio 164-153 */

/* same pin map as above, output enable */
#define QSD8X50_GPIO_OE_0	MSM_GPIO1_SHADOW_REG(0x20)
#define QSD8X50_GPIO_OE_1	MSM_GPIO2_SHADOW_REG(0x08)
#define QSD8X50_GPIO_OE_2	MSM_GPIO1_SHADOW_REG(0x24)
#define QSD8X50_GPIO_OE_3	MSM_GPIO1_SHADOW_REG(0x28)
#define QSD8X50_GPIO_OE_4	MSM_GPIO1_SHADOW_REG(0x2C)
#define QSD8X50_GPIO_OE_5	MSM_GPIO1_SHADOW_REG(0x30)
#define QSD8X50_GPIO_OE_6	MSM_GPIO1_SHADOW_REG(0x34)
#define QSD8X50_GPIO_OE_7	MSM_GPIO1_SHADOW_REG(0x38)

/* same pin map as above, input read */
#define QSD8X50_GPIO_IN_0	MSM_GPIO1_SHADOW_REG(0x50)
#define QSD8X50_GPIO_IN_1	MSM_GPIO2_SHADOW_REG(0x20)
#define QSD8X50_GPIO_IN_2	MSM_GPIO1_SHADOW_REG(0x54)
#define QSD8X50_GPIO_IN_3	MSM_GPIO1_SHADOW_REG(0x58)
#define QSD8X50_GPIO_IN_4	MSM_GPIO1_SHADOW_REG(0x5C)
#define QSD8X50_GPIO_IN_5	MSM_GPIO1_SHADOW_REG(0x60)
#define QSD8X50_GPIO_IN_6	MSM_GPIO1_SHADOW_REG(0x64)
#define QSD8X50_GPIO_IN_7	MSM_GPIO1_SHADOW_REG(0x68)

/* same pin map as above, 1=edge 0=level interrup */
#define QSD8X50_GPIO_INT_EDGE_0	MSM_GPIO1_SHADOW_REG(0x70)
#define QSD8X50_GPIO_INT_EDGE_1	MSM_GPIO2_SHADOW_REG(0x50)
#define QSD8X50_GPIO_INT_EDGE_2	MSM_GPIO1_SHADOW_REG(0x74)
#define QSD8X50_GPIO_INT_EDGE_3	MSM_GPIO1_SHADOW_REG(0x78)
#define QSD8X50_GPIO_INT_EDGE_4	MSM_GPIO1_SHADOW_REG(0x7C)
#define QSD8X50_GPIO_INT_EDGE_5	MSM_GPIO1_SHADOW_REG(0x80)
#define QSD8X50_GPIO_INT_EDGE_6	MSM_GPIO1_SHADOW_REG(0x84)
#define QSD8X50_GPIO_INT_EDGE_7	MSM_GPIO1_SHADOW_REG(0x88)

/* same pin map as above, 1=positive 0=negative */
#define QSD8X50_GPIO_INT_POS_0	MSM_GPIO1_SHADOW_REG(0x90)
#define QSD8X50_GPIO_INT_POS_1	MSM_GPIO2_SHADOW_REG(0x58)
#define QSD8X50_GPIO_INT_POS_2	MSM_GPIO1_SHADOW_REG(0x94)
#define QSD8X50_GPIO_INT_POS_3	MSM_GPIO1_SHADOW_REG(0x98)
#define QSD8X50_GPIO_INT_POS_4	MSM_GPIO1_SHADOW_REG(0x9C)
#define QSD8X50_GPIO_INT_POS_5	MSM_GPIO1_SHADOW_REG(0xA0)
#define QSD8X50_GPIO_INT_POS_6	MSM_GPIO1_SHADOW_REG(0xA4)
#define QSD8X50_GPIO_INT_POS_7	MSM_GPIO1_SHADOW_REG(0xA8)

/* same pin map as above, interrupt enable */
#define QSD8X50_GPIO_INT_EN_0	MSM_GPIO1_SHADOW_REG(0xB0)
#define QSD8X50_GPIO_INT_EN_1	MSM_GPIO2_SHADOW_REG(0x60)
#define QSD8X50_GPIO_INT_EN_2	MSM_GPIO1_SHADOW_REG(0xB4)
#define QSD8X50_GPIO_INT_EN_3	MSM_GPIO1_SHADOW_REG(0xB8)
#define QSD8X50_GPIO_INT_EN_4	MSM_GPIO1_SHADOW_REG(0xBC)
#define QSD8X50_GPIO_INT_EN_5	MSM_GPIO1_SHADOW_REG(0xC0)
#define QSD8X50_GPIO_INT_EN_6	MSM_GPIO1_SHADOW_REG(0xC4)
#define QSD8X50_GPIO_INT_EN_7	MSM_GPIO1_SHADOW_REG(0xC8)

/* same pin map as above, write 1 to clear interrupt */
#define QSD8X50_GPIO_INT_CLEAR_0	MSM_GPIO1_SHADOW_REG(0xD0)
#define QSD8X50_GPIO_INT_CLEAR_1	MSM_GPIO2_SHADOW_REG(0x68)
#define QSD8X50_GPIO_INT_CLEAR_2	MSM_GPIO1_SHADOW_REG(0xD4)
#define QSD8X50_GPIO_INT_CLEAR_3	MSM_GPIO1_SHADOW_REG(0xD8)
#define QSD8X50_GPIO_INT_CLEAR_4	MSM_GPIO1_SHADOW_REG(0xDC)
#define QSD8X50_GPIO_INT_CLEAR_5	MSM_GPIO1_SHADOW_REG(0xE0)
#define QSD8X50_GPIO_INT_CLEAR_6	MSM_GPIO1_SHADOW_REG(0xE4)
#define QSD8X50_GPIO_INT_CLEAR_7	MSM_GPIO1_SHADOW_REG(0xE8)

/* same pin map as above, 1=interrupt pending */
#define QSD8X50_GPIO_INT_STATUS_0	MSM_GPIO1_SHADOW_REG(0xF0)
#define QSD8X50_GPIO_INT_STATUS_1	MSM_GPIO2_SHADOW_REG(0x70)
#define QSD8X50_GPIO_INT_STATUS_2	MSM_GPIO1_SHADOW_REG(0xF4)
#define QSD8X50_GPIO_INT_STATUS_3	MSM_GPIO1_SHADOW_REG(0xF8)
#define QSD8X50_GPIO_INT_STATUS_4	MSM_GPIO1_SHADOW_REG(0xFC)
#define QSD8X50_GPIO_INT_STATUS_5	MSM_GPIO1_SHADOW_REG(0x100)
#define QSD8X50_GPIO_INT_STATUS_6	MSM_GPIO1_SHADOW_REG(0x104)
#define QSD8X50_GPIO_INT_STATUS_7	MSM_GPIO1_SHADOW_REG(0x108)

/*
 * MSM7X30 registers
 */
/* output value */
#define MSM7X30_GPIO_OUT_0	MSM_GPIO1_REG(0x00)   /* gpio  15-0   */
#define MSM7X30_GPIO_OUT_1	MSM_GPIO2_REG(0x00)   /* gpio  43-16  */
#define MSM7X30_GPIO_OUT_2	MSM_GPIO1_REG(0x04)   /* gpio  67-44  */
#define MSM7X30_GPIO_OUT_3	MSM_GPIO1_REG(0x08)   /* gpio  94-68  */
#define MSM7X30_GPIO_OUT_4	MSM_GPIO1_REG(0x0C)   /* gpio 106-95  */
#define MSM7X30_GPIO_OUT_5	MSM_GPIO1_REG(0x50)   /* gpio 133-107 */
#define MSM7X30_GPIO_OUT_6	MSM_GPIO1_REG(0xC4)   /* gpio 150-134 */
#define MSM7X30_GPIO_OUT_7	MSM_GPIO1_REG(0x214)  /* gpio 181-151 */

/* same pin map as above, output enable */
#define MSM7X30_GPIO_OE_0	MSM_GPIO1_REG(0x10)
#define MSM7X30_GPIO_OE_1	MSM_GPIO2_REG(0x08)
#define MSM7X30_GPIO_OE_2	MSM_GPIO1_REG(0x14)
#define MSM7X30_GPIO_OE_3	MSM_GPIO1_REG(0x18)
#define MSM7X30_GPIO_OE_4	MSM_GPIO1_REG(0x1C)
#define MSM7X30_GPIO_OE_5	MSM_GPIO1_REG(0x54)
#define MSM7X30_GPIO_OE_6	MSM_GPIO1_REG(0xC8)
#define MSM7X30_GPIO_OE_7	MSM_GPIO1_REG(0x218)

/* same pin map as above, input read */
#define MSM7X30_GPIO_IN_0	MSM_GPIO1_REG(0x34)
#define MSM7X30_GPIO_IN_1	MSM_GPIO2_REG(0x20)
#define MSM7X30_GPIO_IN_2	MSM_GPIO1_REG(0x38)
#define MSM7X30_GPIO_IN_3	MSM_GPIO1_REG(0x3C)
#define MSM7X30_GPIO_IN_4	MSM_GPIO1_REG(0x40)
#define MSM7X30_GPIO_IN_5	MSM_GPIO1_REG(0x44)
#define MSM7X30_GPIO_IN_6	MSM_GPIO1_REG(0xCC)
#define MSM7X30_GPIO_IN_7	MSM_GPIO1_REG(0x21C)

/* same pin map as above, 1=edge 0=level interrup */
#define MSM7X30_GPIO_INT_EDGE_0	MSM_GPIO1_REG(0x60)
#define MSM7X30_GPIO_INT_EDGE_1	MSM_GPIO2_REG(0x50)
#define MSM7X30_GPIO_INT_EDGE_2	MSM_GPIO1_REG(0x64)
#define MSM7X30_GPIO_INT_EDGE_3	MSM_GPIO1_REG(0x68)
#define MSM7X30_GPIO_INT_EDGE_4	MSM_GPIO1_REG(0x6C)
#define MSM7X30_GPIO_INT_EDGE_5	MSM_GPIO1_REG(0xC0)
#define MSM7X30_GPIO_INT_EDGE_6	MSM_GPIO1_REG(0xD0)
#define MSM7X30_GPIO_INT_EDGE_7	MSM_GPIO1_REG(0x240)

/* same pin map as above, 1=positive 0=negative */
#define MSM7X30_GPIO_INT_POS_0	MSM_GPIO1_REG(0x70)
#define MSM7X30_GPIO_INT_POS_1	MSM_GPIO2_REG(0x58)
#define MSM7X30_GPIO_INT_POS_2	MSM_GPIO1_REG(0x74)
#define MSM7X30_GPIO_INT_POS_3	MSM_GPIO1_REG(0x78)
#define MSM7X30_GPIO_INT_POS_4	MSM_GPIO1_REG(0x7C)
#define MSM7X30_GPIO_INT_POS_5	MSM_GPIO1_REG(0xBC)
#define MSM7X30_GPIO_INT_POS_6	MSM_GPIO1_REG(0xD4)
#define MSM7X30_GPIO_INT_POS_7	MSM_GPIO1_REG(0x228)

/* same pin map as above, interrupt enable */
#define MSM7X30_GPIO_INT_EN_0	MSM_GPIO1_REG(0x80)
#define MSM7X30_GPIO_INT_EN_1	MSM_GPIO2_REG(0x60)
#define MSM7X30_GPIO_INT_EN_2	MSM_GPIO1_REG(0x84)
#define MSM7X30_GPIO_INT_EN_3	MSM_GPIO1_REG(0x88)
#define MSM7X30_GPIO_INT_EN_4	MSM_GPIO1_REG(0x8C)
#define MSM7X30_GPIO_INT_EN_5	MSM_GPIO1_REG(0xB8)
#define MSM7X30_GPIO_INT_EN_6	MSM_GPIO1_REG(0xD8)
#define MSM7X30_GPIO_INT_EN_7	MSM_GPIO1_REG(0x22C)

/* same pin map as above, write 1 to clear interrupt */
#define MSM7X30_GPIO_INT_CLEAR_0	MSM_GPIO1_REG(0x90)
#define MSM7X30_GPIO_INT_CLEAR_1	MSM_GPIO2_REG(0x68)
#define MSM7X30_GPIO_INT_CLEAR_2	MSM_GPIO1_REG(0x94)
#define MSM7X30_GPIO_INT_CLEAR_3	MSM_GPIO1_REG(0x98)
#define MSM7X30_GPIO_INT_CLEAR_4	MSM_GPIO1_REG(0x9C)
#define MSM7X30_GPIO_INT_CLEAR_5	MSM_GPIO1_REG(0xB4)
#define MSM7X30_GPIO_INT_CLEAR_6	MSM_GPIO1_REG(0xDC)
#define MSM7X30_GPIO_INT_CLEAR_7	MSM_GPIO1_REG(0x230)

/* same pin map as above, 1=interrupt pending */
#define MSM7X30_GPIO_INT_STATUS_0	MSM_GPIO1_REG(0xA0)
#define MSM7X30_GPIO_INT_STATUS_1	MSM_GPIO2_REG(0x70)
#define MSM7X30_GPIO_INT_STATUS_2	MSM_GPIO1_REG(0xA4)
#define MSM7X30_GPIO_INT_STATUS_3	MSM_GPIO1_REG(0xA8)
#define MSM7X30_GPIO_INT_STATUS_4	MSM_GPIO1_REG(0xAC)
#define MSM7X30_GPIO_INT_STATUS_5	MSM_GPIO1_REG(0xB0)
#define MSM7X30_GPIO_INT_STATUS_6	MSM_GPIO1_REG(0xE0)
#define MSM7X30_GPIO_INT_STATUS_7	MSM_GPIO1_REG(0x234)

#define FIRST_GPIO_IRQ MSM_GPIO_TO_INT(0)

#define MSM_GPIO_BANK(soc, bank, first, last)				\
	{								\
		.regs[MSM_GPIO_OUT] =         soc##_GPIO_OUT_##bank,	\
		.regs[MSM_GPIO_IN] =          soc##_GPIO_IN_##bank,	\
		.regs[MSM_GPIO_INT_STATUS] =  soc##_GPIO_INT_STATUS_##bank, \
		.regs[MSM_GPIO_INT_CLEAR] =   soc##_GPIO_INT_CLEAR_##bank, \
		.regs[MSM_GPIO_INT_EN] =      soc##_GPIO_INT_EN_##bank,	\
		.regs[MSM_GPIO_INT_EDGE] =    soc##_GPIO_INT_EDGE_##bank, \
		.regs[MSM_GPIO_INT_POS] =     soc##_GPIO_INT_POS_##bank, \
		.regs[MSM_GPIO_OE] =          soc##_GPIO_OE_##bank,	\
>>>>>>> refs/remotes/origin/master
		.chip = {						\
			.base = (first),				\
			.ngpio = (last) - (first) + 1,			\
			.get = msm_gpio_get,				\
			.set = msm_gpio_set,				\
			.direction_input = msm_gpio_direction_input,	\
			.direction_output = msm_gpio_direction_output,	\
			.to_irq = msm_gpio_to_irq,			\
			.request = msm_gpio_request,			\
			.free = msm_gpio_free,				\
		}							\
	}

#define MSM_GPIO_BROKEN_INT_CLEAR 1

<<<<<<< HEAD
struct msm_gpio_regs {
	void __iomem *out;
	void __iomem *in;
	void __iomem *int_status;
	void __iomem *int_clear;
	void __iomem *int_en;
	void __iomem *int_edge;
	void __iomem *int_pos;
	void __iomem *oe;
=======
enum msm_gpio_reg {
	MSM_GPIO_IN,
	MSM_GPIO_OUT,
	MSM_GPIO_INT_STATUS,
	MSM_GPIO_INT_CLEAR,
	MSM_GPIO_INT_EN,
	MSM_GPIO_INT_EDGE,
	MSM_GPIO_INT_POS,
	MSM_GPIO_OE,
	MSM_GPIO_REG_NR
>>>>>>> refs/remotes/origin/master
};

struct msm_gpio_chip {
	spinlock_t		lock;
	struct gpio_chip	chip;
<<<<<<< HEAD
	struct msm_gpio_regs	regs;
=======
	unsigned long		regs[MSM_GPIO_REG_NR];
>>>>>>> refs/remotes/origin/master
#if MSM_GPIO_BROKEN_INT_CLEAR
	unsigned                int_status_copy;
#endif
	unsigned int            both_edge_detect;
	unsigned int            int_enable[2]; /* 0: awake, 1: sleep */
<<<<<<< HEAD
};

=======
	void __iomem		*base;
};

struct msm_gpio_initdata {
	struct msm_gpio_chip *chips;
	int count;
};

static void msm_gpio_writel(struct msm_gpio_chip *chip, u32 val,
			    enum msm_gpio_reg reg)
{
	writel(val, chip->base + chip->regs[reg]);
}

static u32 msm_gpio_readl(struct msm_gpio_chip *chip, enum msm_gpio_reg reg)
{
	return readl(chip->base + chip->regs[reg]);
}

>>>>>>> refs/remotes/origin/master
static int msm_gpio_write(struct msm_gpio_chip *msm_chip,
			  unsigned offset, unsigned on)
{
	unsigned mask = BIT(offset);
	unsigned val;

<<<<<<< HEAD
	val = __raw_readl(msm_chip->regs.out);
	if (on)
		__raw_writel(val | mask, msm_chip->regs.out);
	else
		__raw_writel(val & ~mask, msm_chip->regs.out);
=======
	val = msm_gpio_readl(msm_chip, MSM_GPIO_OUT);
	if (on)
		msm_gpio_writel(msm_chip, val | mask, MSM_GPIO_OUT);
	else
		msm_gpio_writel(msm_chip, val & ~mask, MSM_GPIO_OUT);
>>>>>>> refs/remotes/origin/master
	return 0;
}

static void msm_gpio_update_both_edge_detect(struct msm_gpio_chip *msm_chip)
{
	int loop_limit = 100;
	unsigned pol, val, val2, intstat;
	do {
<<<<<<< HEAD
		val = __raw_readl(msm_chip->regs.in);
		pol = __raw_readl(msm_chip->regs.int_pos);
		pol = (pol & ~msm_chip->both_edge_detect) |
		      (~val & msm_chip->both_edge_detect);
		__raw_writel(pol, msm_chip->regs.int_pos);
		intstat = __raw_readl(msm_chip->regs.int_status);
		val2 = __raw_readl(msm_chip->regs.in);
=======
		val = msm_gpio_readl(msm_chip, MSM_GPIO_IN);
		pol = msm_gpio_readl(msm_chip, MSM_GPIO_INT_POS);
		pol = (pol & ~msm_chip->both_edge_detect) |
		      (~val & msm_chip->both_edge_detect);
		msm_gpio_writel(msm_chip, pol, MSM_GPIO_INT_POS);
		intstat = msm_gpio_readl(msm_chip, MSM_GPIO_INT_STATUS);
		val2 = msm_gpio_readl(msm_chip, MSM_GPIO_IN);
>>>>>>> refs/remotes/origin/master
		if (((val ^ val2) & msm_chip->both_edge_detect & ~intstat) == 0)
			return;
	} while (loop_limit-- > 0);
	printk(KERN_ERR "msm_gpio_update_both_edge_detect, "
	       "failed to reach stable state %x != %x\n", val, val2);
}

static int msm_gpio_clear_detect_status(struct msm_gpio_chip *msm_chip,
					unsigned offset)
{
	unsigned bit = BIT(offset);

#if MSM_GPIO_BROKEN_INT_CLEAR
	/* Save interrupts that already triggered before we loose them. */
	/* Any interrupt that triggers between the read of int_status */
	/* and the write to int_clear will still be lost though. */
<<<<<<< HEAD
	msm_chip->int_status_copy |= __raw_readl(msm_chip->regs.int_status);
	msm_chip->int_status_copy &= ~bit;
#endif
	__raw_writel(bit, msm_chip->regs.int_clear);
=======
	msm_chip->int_status_copy |=
		msm_gpio_readl(msm_chip, MSM_GPIO_INT_STATUS);
	msm_chip->int_status_copy &= ~bit;
#endif
	msm_gpio_writel(msm_chip, bit, MSM_GPIO_INT_CLEAR);
>>>>>>> refs/remotes/origin/master
	msm_gpio_update_both_edge_detect(msm_chip);
	return 0;
}

static int msm_gpio_direction_input(struct gpio_chip *chip, unsigned offset)
{
	struct msm_gpio_chip *msm_chip;
	unsigned long irq_flags;
<<<<<<< HEAD

	msm_chip = container_of(chip, struct msm_gpio_chip, chip);
	spin_lock_irqsave(&msm_chip->lock, irq_flags);
	__raw_writel(__raw_readl(msm_chip->regs.oe) & ~BIT(offset),
			msm_chip->regs.oe);
	mb();
=======
	u32 val;

	msm_chip = container_of(chip, struct msm_gpio_chip, chip);
	spin_lock_irqsave(&msm_chip->lock, irq_flags);
	val = msm_gpio_readl(msm_chip, MSM_GPIO_OE) & ~BIT(offset);
	msm_gpio_writel(msm_chip, val, MSM_GPIO_OE);
>>>>>>> refs/remotes/origin/master
	spin_unlock_irqrestore(&msm_chip->lock, irq_flags);
	return 0;
}

static int
msm_gpio_direction_output(struct gpio_chip *chip, unsigned offset, int value)
{
	struct msm_gpio_chip *msm_chip;
	unsigned long irq_flags;
<<<<<<< HEAD
=======
	u32 val;
>>>>>>> refs/remotes/origin/master

	msm_chip = container_of(chip, struct msm_gpio_chip, chip);
	spin_lock_irqsave(&msm_chip->lock, irq_flags);
	msm_gpio_write(msm_chip, offset, value);
<<<<<<< HEAD
	__raw_writel(__raw_readl(msm_chip->regs.oe) | BIT(offset),
			msm_chip->regs.oe);
	mb();
=======
	val = msm_gpio_readl(msm_chip, MSM_GPIO_OE) | BIT(offset);
	msm_gpio_writel(msm_chip, val, MSM_GPIO_OE);
>>>>>>> refs/remotes/origin/master
	spin_unlock_irqrestore(&msm_chip->lock, irq_flags);
	return 0;
}

static int msm_gpio_get(struct gpio_chip *chip, unsigned offset)
{
	struct msm_gpio_chip *msm_chip;
<<<<<<< HEAD
	int rc;

	msm_chip = container_of(chip, struct msm_gpio_chip, chip);
	rc = (__raw_readl(msm_chip->regs.in) & (1U << offset)) ? 1 : 0;
	mb();
	return rc;
=======

	msm_chip = container_of(chip, struct msm_gpio_chip, chip);
	return (msm_gpio_readl(msm_chip, MSM_GPIO_IN) & (1U << offset)) ? 1 : 0;
>>>>>>> refs/remotes/origin/master
}

static void msm_gpio_set(struct gpio_chip *chip, unsigned offset, int value)
{
	struct msm_gpio_chip *msm_chip;
	unsigned long irq_flags;

	msm_chip = container_of(chip, struct msm_gpio_chip, chip);
	spin_lock_irqsave(&msm_chip->lock, irq_flags);
	msm_gpio_write(msm_chip, offset, value);
<<<<<<< HEAD
	mb();
=======
>>>>>>> refs/remotes/origin/master
	spin_unlock_irqrestore(&msm_chip->lock, irq_flags);
}

static int msm_gpio_to_irq(struct gpio_chip *chip, unsigned offset)
{
	return MSM_GPIO_TO_INT(chip->base + offset);
}

#ifdef CONFIG_MSM_GPIOMUX
static int msm_gpio_request(struct gpio_chip *chip, unsigned offset)
{
	return msm_gpiomux_get(chip->base + offset);
}

static void msm_gpio_free(struct gpio_chip *chip, unsigned offset)
{
	msm_gpiomux_put(chip->base + offset);
}
#else
#define msm_gpio_request NULL
#define msm_gpio_free NULL
#endif

<<<<<<< HEAD
struct msm_gpio_chip msm_gpio_chips[] = {
#if defined(CONFIG_ARCH_MSM7X00A)
	MSM_GPIO_BANK(0,   0,  15),
	MSM_GPIO_BANK(1,  16,  42),
	MSM_GPIO_BANK(2,  43,  67),
	MSM_GPIO_BANK(3,  68,  94),
	MSM_GPIO_BANK(4,  95, 106),
	MSM_GPIO_BANK(5, 107, 121),
#elif defined(CONFIG_ARCH_MSM7X25) || defined(CONFIG_ARCH_MSM7X27)
	MSM_GPIO_BANK(0,   0,  15),
	MSM_GPIO_BANK(1,  16,  42),
	MSM_GPIO_BANK(2,  43,  67),
	MSM_GPIO_BANK(3,  68,  94),
	MSM_GPIO_BANK(4,  95, 106),
	MSM_GPIO_BANK(5, 107, 132),
#elif defined(CONFIG_ARCH_MSM7X30)
	MSM_GPIO_BANK(0,   0,  15),
	MSM_GPIO_BANK(1,  16,  43),
	MSM_GPIO_BANK(2,  44,  67),
	MSM_GPIO_BANK(3,  68,  94),
	MSM_GPIO_BANK(4,  95, 106),
	MSM_GPIO_BANK(5, 107, 133),
	MSM_GPIO_BANK(6, 134, 150),
	MSM_GPIO_BANK(7, 151, 181),
#elif defined(CONFIG_ARCH_QSD8X50)
	MSM_GPIO_BANK(0,   0,  15),
	MSM_GPIO_BANK(1,  16,  42),
	MSM_GPIO_BANK(2,  43,  67),
	MSM_GPIO_BANK(3,  68,  94),
	MSM_GPIO_BANK(4,  95, 103),
	MSM_GPIO_BANK(5, 104, 121),
	MSM_GPIO_BANK(6, 122, 152),
	MSM_GPIO_BANK(7, 153, 164),
#endif
=======
static struct msm_gpio_chip *msm_gpio_chips;
static int msm_gpio_count;

static struct msm_gpio_chip msm_gpio_chips_msm7x01[] = {
	MSM_GPIO_BANK(MSM7X00, 0,   0,  15),
	MSM_GPIO_BANK(MSM7X00, 1,  16,  42),
	MSM_GPIO_BANK(MSM7X00, 2,  43,  67),
	MSM_GPIO_BANK(MSM7X00, 3,  68,  94),
	MSM_GPIO_BANK(MSM7X00, 4,  95, 106),
	MSM_GPIO_BANK(MSM7X00, 5, 107, 121),
};

static struct msm_gpio_initdata msm_gpio_7x01_init = {
	.chips = msm_gpio_chips_msm7x01,
	.count = ARRAY_SIZE(msm_gpio_chips_msm7x01),
};

static struct msm_gpio_chip msm_gpio_chips_msm7x30[] = {
	MSM_GPIO_BANK(MSM7X30, 0,   0,  15),
	MSM_GPIO_BANK(MSM7X30, 1,  16,  43),
	MSM_GPIO_BANK(MSM7X30, 2,  44,  67),
	MSM_GPIO_BANK(MSM7X30, 3,  68,  94),
	MSM_GPIO_BANK(MSM7X30, 4,  95, 106),
	MSM_GPIO_BANK(MSM7X30, 5, 107, 133),
	MSM_GPIO_BANK(MSM7X30, 6, 134, 150),
	MSM_GPIO_BANK(MSM7X30, 7, 151, 181),
};

static struct msm_gpio_initdata msm_gpio_7x30_init = {
	.chips = msm_gpio_chips_msm7x30,
	.count = ARRAY_SIZE(msm_gpio_chips_msm7x30),
};

static struct msm_gpio_chip msm_gpio_chips_qsd8x50[] = {
	MSM_GPIO_BANK(QSD8X50, 0,   0,  15),
	MSM_GPIO_BANK(QSD8X50, 1,  16,  42),
	MSM_GPIO_BANK(QSD8X50, 2,  43,  67),
	MSM_GPIO_BANK(QSD8X50, 3,  68,  94),
	MSM_GPIO_BANK(QSD8X50, 4,  95, 103),
	MSM_GPIO_BANK(QSD8X50, 5, 104, 121),
	MSM_GPIO_BANK(QSD8X50, 6, 122, 152),
	MSM_GPIO_BANK(QSD8X50, 7, 153, 164),
};

static struct msm_gpio_initdata msm_gpio_8x50_init = {
	.chips = msm_gpio_chips_qsd8x50,
	.count = ARRAY_SIZE(msm_gpio_chips_qsd8x50),
>>>>>>> refs/remotes/origin/master
};

static void msm_gpio_irq_ack(struct irq_data *d)
{
	unsigned long irq_flags;
<<<<<<< HEAD
	struct msm_gpio_chip *msm_chip = irq_get_chip_data(d->irq);
	spin_lock_irqsave(&msm_chip->lock, irq_flags);
	msm_gpio_clear_detect_status(msm_chip,
			     d->irq - gpio_to_irq(msm_chip->chip.base));
=======
	struct msm_gpio_chip *msm_chip = irq_data_get_irq_chip_data(d);
	spin_lock_irqsave(&msm_chip->lock, irq_flags);
	msm_gpio_clear_detect_status(msm_chip,
				     d->irq - gpio_to_irq(msm_chip->chip.base));
>>>>>>> refs/remotes/origin/master
	spin_unlock_irqrestore(&msm_chip->lock, irq_flags);
}

static void msm_gpio_irq_mask(struct irq_data *d)
{
	unsigned long irq_flags;
<<<<<<< HEAD
	struct msm_gpio_chip *msm_chip = irq_get_chip_data(d->irq);
=======
	struct msm_gpio_chip *msm_chip = irq_data_get_irq_chip_data(d);
>>>>>>> refs/remotes/origin/master
	unsigned offset = d->irq - gpio_to_irq(msm_chip->chip.base);

	spin_lock_irqsave(&msm_chip->lock, irq_flags);
	/* level triggered interrupts are also latched */
<<<<<<< HEAD
	if (!(__raw_readl(msm_chip->regs.int_edge) & BIT(offset)))
		msm_gpio_clear_detect_status(msm_chip, offset);
	msm_chip->int_enable[0] &= ~BIT(offset);
	__raw_writel(msm_chip->int_enable[0], msm_chip->regs.int_en);
	mb();
=======
	if (!(msm_gpio_readl(msm_chip, MSM_GPIO_INT_EDGE) & BIT(offset)))
		msm_gpio_clear_detect_status(msm_chip, offset);
	msm_chip->int_enable[0] &= ~BIT(offset);
	msm_gpio_writel(msm_chip, msm_chip->int_enable[0], MSM_GPIO_INT_EN);
>>>>>>> refs/remotes/origin/master
	spin_unlock_irqrestore(&msm_chip->lock, irq_flags);
}

static void msm_gpio_irq_unmask(struct irq_data *d)
{
	unsigned long irq_flags;
<<<<<<< HEAD
	struct msm_gpio_chip *msm_chip = irq_get_chip_data(d->irq);
=======
	struct msm_gpio_chip *msm_chip = irq_data_get_irq_chip_data(d);
>>>>>>> refs/remotes/origin/master
	unsigned offset = d->irq - gpio_to_irq(msm_chip->chip.base);

	spin_lock_irqsave(&msm_chip->lock, irq_flags);
	/* level triggered interrupts are also latched */
<<<<<<< HEAD
	if (!(__raw_readl(msm_chip->regs.int_edge) & BIT(offset)))
		msm_gpio_clear_detect_status(msm_chip, offset);
	msm_chip->int_enable[0] |= BIT(offset);
	__raw_writel(msm_chip->int_enable[0], msm_chip->regs.int_en);
	mb();
=======
	if (!(msm_gpio_readl(msm_chip, MSM_GPIO_INT_EDGE) & BIT(offset)))
		msm_gpio_clear_detect_status(msm_chip, offset);
	msm_chip->int_enable[0] |= BIT(offset);
	msm_gpio_writel(msm_chip, msm_chip->int_enable[0], MSM_GPIO_INT_EN);
>>>>>>> refs/remotes/origin/master
	spin_unlock_irqrestore(&msm_chip->lock, irq_flags);
}

static int msm_gpio_irq_set_wake(struct irq_data *d, unsigned int on)
{
	unsigned long irq_flags;
<<<<<<< HEAD
	struct msm_gpio_chip *msm_chip = irq_get_chip_data(d->irq);
=======
	struct msm_gpio_chip *msm_chip = irq_data_get_irq_chip_data(d);
>>>>>>> refs/remotes/origin/master
	unsigned offset = d->irq - gpio_to_irq(msm_chip->chip.base);

	spin_lock_irqsave(&msm_chip->lock, irq_flags);

	if (on)
		msm_chip->int_enable[1] |= BIT(offset);
	else
		msm_chip->int_enable[1] &= ~BIT(offset);

	spin_unlock_irqrestore(&msm_chip->lock, irq_flags);
	return 0;
}

static int msm_gpio_irq_set_type(struct irq_data *d, unsigned int flow_type)
{
	unsigned long irq_flags;
<<<<<<< HEAD
	struct msm_gpio_chip *msm_chip = irq_get_chip_data(d->irq);
=======
	struct msm_gpio_chip *msm_chip = irq_data_get_irq_chip_data(d);
>>>>>>> refs/remotes/origin/master
	unsigned offset = d->irq - gpio_to_irq(msm_chip->chip.base);
	unsigned val, mask = BIT(offset);

	spin_lock_irqsave(&msm_chip->lock, irq_flags);
<<<<<<< HEAD
	val = __raw_readl(msm_chip->regs.int_edge);
	if (flow_type & IRQ_TYPE_EDGE_BOTH) {
		__raw_writel(val | mask, msm_chip->regs.int_edge);
		__irq_set_handler_locked(d->irq, handle_edge_irq);
	} else {
		__raw_writel(val & ~mask, msm_chip->regs.int_edge);
=======
	val = msm_gpio_readl(msm_chip, MSM_GPIO_INT_EDGE);
	if (flow_type & IRQ_TYPE_EDGE_BOTH) {
		msm_gpio_writel(msm_chip, val | mask, MSM_GPIO_INT_EDGE);
		__irq_set_handler_locked(d->irq, handle_edge_irq);
	} else {
		msm_gpio_writel(msm_chip, val & ~mask, MSM_GPIO_INT_EDGE);
>>>>>>> refs/remotes/origin/master
		__irq_set_handler_locked(d->irq, handle_level_irq);
	}
	if ((flow_type & IRQ_TYPE_EDGE_BOTH) == IRQ_TYPE_EDGE_BOTH) {
		msm_chip->both_edge_detect |= mask;
		msm_gpio_update_both_edge_detect(msm_chip);
	} else {
		msm_chip->both_edge_detect &= ~mask;
<<<<<<< HEAD
		val = __raw_readl(msm_chip->regs.int_pos);
		if (flow_type & (IRQF_TRIGGER_RISING | IRQF_TRIGGER_HIGH))
			__raw_writel(val | mask, msm_chip->regs.int_pos);
		else
			__raw_writel(val & ~mask, msm_chip->regs.int_pos);
	}
	mb();
=======
		val = msm_gpio_readl(msm_chip, MSM_GPIO_INT_POS);
		if (flow_type & (IRQF_TRIGGER_RISING | IRQF_TRIGGER_HIGH))
			val |= mask;
		else
			val &= ~mask;
		msm_gpio_writel(msm_chip, val, MSM_GPIO_INT_POS);
	}
>>>>>>> refs/remotes/origin/master
	spin_unlock_irqrestore(&msm_chip->lock, irq_flags);
	return 0;
}

static void msm_gpio_irq_handler(unsigned int irq, struct irq_desc *desc)
{
	int i, j, mask;
	unsigned val;
<<<<<<< HEAD
	struct irq_chip *chip = irq_desc_get_chip(desc);

	chained_irq_enter(chip, desc);

	for (i = 0; i < ARRAY_SIZE(msm_gpio_chips); i++) {
		struct msm_gpio_chip *msm_chip = &msm_gpio_chips[i];
		val = __raw_readl(msm_chip->regs.int_status);
=======

	for (i = 0; i < msm_gpio_count; i++) {
		struct msm_gpio_chip *msm_chip = &msm_gpio_chips[i];
		val = msm_gpio_readl(msm_chip, MSM_GPIO_INT_STATUS);
>>>>>>> refs/remotes/origin/master
		val &= msm_chip->int_enable[0];
		while (val) {
			mask = val & -val;
			j = fls(mask) - 1;
			/* printk("%s %08x %08x bit %d gpio %d irq %d\n",
				__func__, v, m, j, msm_chip->chip.start + j,
				FIRST_GPIO_IRQ + msm_chip->chip.start + j); */
			val &= ~mask;
			generic_handle_irq(FIRST_GPIO_IRQ +
					   msm_chip->chip.base + j);
		}
	}
<<<<<<< HEAD

	chained_irq_exit(chip, desc);
}

static struct irq_chip msm_gpio_irq_chip = {
	.name      = "msmgpio",
	.irq_ack	= msm_gpio_irq_ack,
	.irq_mask	= msm_gpio_irq_mask,
	.irq_unmask	= msm_gpio_irq_unmask,
	.irq_set_wake	= msm_gpio_irq_set_wake,
	.irq_set_type	= msm_gpio_irq_set_type,
};

#define NUM_GPIO_SMEM_BANKS 6
#define GPIO_SMEM_NUM_GROUPS 2
#define GPIO_SMEM_MAX_PC_INTERRUPTS 8
struct tramp_gpio_smem {
	uint16_t num_fired[GPIO_SMEM_NUM_GROUPS];
	uint16_t fired[GPIO_SMEM_NUM_GROUPS][GPIO_SMEM_MAX_PC_INTERRUPTS];
	uint32_t enabled[NUM_GPIO_SMEM_BANKS];
	uint32_t detection[NUM_GPIO_SMEM_BANKS];
	uint32_t polarity[NUM_GPIO_SMEM_BANKS];
};

static void msm_gpio_sleep_int(unsigned long arg)
{
	int i, j;
	struct tramp_gpio_smem *smem_gpio;

	BUILD_BUG_ON(NR_GPIO_IRQS > NUM_GPIO_SMEM_BANKS * 32);

	smem_gpio = smem_alloc(SMEM_GPIO_INT, sizeof(*smem_gpio));
	if (smem_gpio == NULL)
		return;

	local_irq_disable();
	for (i = 0; i < GPIO_SMEM_NUM_GROUPS; i++) {
		int count = smem_gpio->num_fired[i];
		for (j = 0; j < count; j++) {
			/* TODO: Check mask */
			generic_handle_irq(
				MSM_GPIO_TO_INT(smem_gpio->fired[i][j]));
		}
	}
	local_irq_enable();
}

static DECLARE_TASKLET(msm_gpio_sleep_int_tasklet, msm_gpio_sleep_int, 0);

void msm_gpio_enter_sleep(int from_idle)
{
	int i;
	struct tramp_gpio_smem *smem_gpio;

	smem_gpio = smem_alloc(SMEM_GPIO_INT, sizeof(*smem_gpio));

	if (smem_gpio) {
		for (i = 0; i < ARRAY_SIZE(smem_gpio->enabled); i++) {
			smem_gpio->enabled[i] = 0;
			smem_gpio->detection[i] = 0;
			smem_gpio->polarity[i] = 0;
		}
	}

	for (i = 0; i < ARRAY_SIZE(msm_gpio_chips); i++) {
		__raw_writel(msm_gpio_chips[i].int_enable[!from_idle],
		       msm_gpio_chips[i].regs.int_en);
		if (smem_gpio) {
			uint32_t tmp;
			int start, index, shiftl, shiftr;
			start = msm_gpio_chips[i].chip.base;
			index = start / 32;
			shiftl = start % 32;
			shiftr = 32 - shiftl;
			tmp = msm_gpio_chips[i].int_enable[!from_idle];
			smem_gpio->enabled[index] |= tmp << shiftl;
			smem_gpio->enabled[index+1] |= tmp >> shiftr;
			smem_gpio->detection[index] |=
				__raw_readl(msm_gpio_chips[i].regs.int_edge) <<
				shiftl;
			smem_gpio->detection[index+1] |=
				__raw_readl(msm_gpio_chips[i].regs.int_edge) >>
				shiftr;
			smem_gpio->polarity[index] |=
				__raw_readl(msm_gpio_chips[i].regs.int_pos) <<
				shiftl;
			smem_gpio->polarity[index+1] |=
				__raw_readl(msm_gpio_chips[i].regs.int_pos) >>
				shiftr;
		}
	}
	mb();

	if (smem_gpio) {
		if (msm_gpio_debug_mask & GPIO_DEBUG_SLEEP)
			for (i = 0; i < ARRAY_SIZE(smem_gpio->enabled); i++) {
				printk("msm_gpio_enter_sleep gpio %d-%d: enable"
				       " %08x, edge %08x, polarity %08x\n",
				       i * 32, i * 32 + 31,
				       smem_gpio->enabled[i],
				       smem_gpio->detection[i],
				       smem_gpio->polarity[i]);
			}
		for (i = 0; i < GPIO_SMEM_NUM_GROUPS; i++)
			smem_gpio->num_fired[i] = 0;
	}
}

void msm_gpio_exit_sleep(void)
{
	int i;
	struct tramp_gpio_smem *smem_gpio;

	smem_gpio = smem_alloc(SMEM_GPIO_INT, sizeof(*smem_gpio));

	for (i = 0; i < ARRAY_SIZE(msm_gpio_chips); i++) {
		__raw_writel(msm_gpio_chips[i].int_enable[0],
		       msm_gpio_chips[i].regs.int_en);
	}
	mb();

	if (smem_gpio && (smem_gpio->num_fired[0] || smem_gpio->num_fired[1])) {
		if (msm_gpio_debug_mask & GPIO_DEBUG_SLEEP)
			printk(KERN_INFO "gpio: fired %x %x\n",
			      smem_gpio->num_fired[0], smem_gpio->num_fired[1]);
		tasklet_schedule(&msm_gpio_sleep_int_tasklet);
	}
}


int gpio_tlmm_config(unsigned config, unsigned disable)
{
	return msm_proc_comm(PCOM_RPC_GPIO_TLMM_CONFIG_EX, &config, &disable);
}
EXPORT_SYMBOL(gpio_tlmm_config);

int msm_gpios_request_enable(const struct msm_gpio *table, int size)
{
	int rc = msm_gpios_request(table, size);
	if (rc)
		return rc;
	rc = msm_gpios_enable(table, size);
	if (rc)
		msm_gpios_free(table, size);
	return rc;
}
EXPORT_SYMBOL(msm_gpios_request_enable);

void msm_gpios_disable_free(const struct msm_gpio *table, int size)
{
	msm_gpios_disable(table, size);
	msm_gpios_free(table, size);
}
EXPORT_SYMBOL(msm_gpios_disable_free);

int msm_gpios_request(const struct msm_gpio *table, int size)
{
	int rc;
	int i;
	const struct msm_gpio *g;
	for (i = 0; i < size; i++) {
		g = table + i;
		rc = gpio_request(GPIO_PIN(g->gpio_cfg), g->label);
		if (rc) {
			pr_err("gpio_request(%d) <%s> failed: %d\n",
			       GPIO_PIN(g->gpio_cfg), g->label ?: "?", rc);
			goto err;
		}
	}
	return 0;
err:
	msm_gpios_free(table, i);
	return rc;
}
EXPORT_SYMBOL(msm_gpios_request);

void msm_gpios_free(const struct msm_gpio *table, int size)
{
	int i;
	const struct msm_gpio *g;
	for (i = size-1; i >= 0; i--) {
		g = table + i;
		gpio_free(GPIO_PIN(g->gpio_cfg));
	}
}
EXPORT_SYMBOL(msm_gpios_free);

int msm_gpios_enable(const struct msm_gpio *table, int size)
{
	int rc;
	int i;
	const struct msm_gpio *g;
	for (i = 0; i < size; i++) {
		g = table + i;
		rc = gpio_tlmm_config(g->gpio_cfg, GPIO_CFG_ENABLE);
		if (rc) {
			pr_err("gpio_tlmm_config(0x%08x, GPIO_CFG_ENABLE)"
			       " <%s> failed: %d\n",
			       g->gpio_cfg, g->label ?: "?", rc);
			pr_err("pin %d func %d dir %d pull %d drvstr %d\n",
			       GPIO_PIN(g->gpio_cfg), GPIO_FUNC(g->gpio_cfg),
			       GPIO_DIR(g->gpio_cfg), GPIO_PULL(g->gpio_cfg),
			       GPIO_DRVSTR(g->gpio_cfg));
			goto err;
		}
	}
	return 0;
err:
	msm_gpios_disable(table, i);
	return rc;
}
EXPORT_SYMBOL(msm_gpios_enable);

int msm_gpios_disable(const struct msm_gpio *table, int size)
{
	int rc = 0;
	int i;
	const struct msm_gpio *g;
	for (i = size-1; i >= 0; i--) {
		int tmp;
		g = table + i;
		tmp = gpio_tlmm_config(g->gpio_cfg, GPIO_CFG_DISABLE);
		if (tmp) {
			pr_err("gpio_tlmm_config(0x%08x, GPIO_CFG_DISABLE)"
			       " <%s> failed: %d\n",
			       g->gpio_cfg, g->label ?: "?", rc);
			pr_err("pin %d func %d dir %d pull %d drvstr %d\n",
			       GPIO_PIN(g->gpio_cfg), GPIO_FUNC(g->gpio_cfg),
			       GPIO_DIR(g->gpio_cfg), GPIO_PULL(g->gpio_cfg),
			       GPIO_DRVSTR(g->gpio_cfg));
			if (!rc)
				rc = tmp;
		}
	}

	return rc;
}
EXPORT_SYMBOL(msm_gpios_disable);

/* Locate the GPIO_OUT register for the given GPIO and return its address
 * and the bit position of the gpio's bit within the register.
 *
 * This function is used by gpiomux-v1 in order to support output transitions.
 */
void msm_gpio_find_out(const unsigned gpio, void __iomem **out,
	unsigned *offset)
{
	struct msm_gpio_chip *msm_chip = msm_gpio_chips;

	while (gpio >= msm_chip->chip.base + msm_chip->chip.ngpio)
		++msm_chip;

	*out = msm_chip->regs.out;
	*offset = gpio - msm_chip->chip.base;
}

static int __devinit msm_gpio_probe(struct platform_device *dev)
{
	int i, j = 0;
	int grp_irq;
=======
	desc->irq_data.chip->irq_ack(&desc->irq_data);
}

static struct irq_chip msm_gpio_irq_chip = {
	.name          = "msmgpio",
	.irq_ack       = msm_gpio_irq_ack,
	.irq_mask      = msm_gpio_irq_mask,
	.irq_unmask    = msm_gpio_irq_unmask,
	.irq_set_wake  = msm_gpio_irq_set_wake,
	.irq_set_type  = msm_gpio_irq_set_type,
};

static int gpio_msm_v1_probe(struct platform_device *pdev)
{
	int i, j = 0;
	const struct platform_device_id *dev_id = platform_get_device_id(pdev);
	struct msm_gpio_initdata *data;
	int irq1, irq2;
	struct resource *res;
	void __iomem *base1, __iomem *base2;

	data = (struct msm_gpio_initdata *)dev_id->driver_data;
	msm_gpio_chips = data->chips;
	msm_gpio_count = data->count;

	irq1 = platform_get_irq(pdev, 0);
	if (irq1 < 0)
		return irq1;

	irq2 = platform_get_irq(pdev, 1);
	if (irq2 < 0)
		return irq2;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	base1 = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(base1))
		return PTR_ERR(base1);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	base2 = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(base2))
		return PTR_ERR(base2);
>>>>>>> refs/remotes/origin/master

	for (i = FIRST_GPIO_IRQ; i < FIRST_GPIO_IRQ + NR_GPIO_IRQS; i++) {
		if (i - FIRST_GPIO_IRQ >=
			msm_gpio_chips[j].chip.base +
			msm_gpio_chips[j].chip.ngpio)
			j++;
		irq_set_chip_data(i, &msm_gpio_chips[j]);
		irq_set_chip_and_handler(i, &msm_gpio_irq_chip,
					 handle_edge_irq);
		set_irq_flags(i, IRQF_VALID);
	}

<<<<<<< HEAD
	for (i = 0; i < dev->num_resources; i++) {
		grp_irq = platform_get_irq(dev, i);
		if (grp_irq < 0)
			return -ENXIO;

		irq_set_chained_handler(grp_irq, msm_gpio_irq_handler);
		irq_set_irq_wake(grp_irq, (i + 1));
	}

	for (i = 0; i < ARRAY_SIZE(msm_gpio_chips); i++) {
		spin_lock_init(&msm_gpio_chips[i].lock);
		__raw_writel(0, msm_gpio_chips[i].regs.int_en);
		gpiochip_add(&msm_gpio_chips[i].chip);
	}

	mb();
	return 0;
}

static struct platform_driver msm_gpio_driver = {
	.probe = msm_gpio_probe,
	.driver = {
		.name = "msmgpio",
		.owner = THIS_MODULE,
	},
};

static int __init msm_gpio_init(void)
{
	return platform_driver_register(&msm_gpio_driver);
}
postcore_initcall(msm_gpio_init);
=======
	for (i = 0; i < msm_gpio_count; i++) {
		if (i == 1)
			msm_gpio_chips[i].base = base2;
		else
			msm_gpio_chips[i].base = base1;
		spin_lock_init(&msm_gpio_chips[i].lock);
		msm_gpio_writel(&msm_gpio_chips[i], 0, MSM_GPIO_INT_EN);
		gpiochip_add(&msm_gpio_chips[i].chip);
	}

	irq_set_chained_handler(irq1, msm_gpio_irq_handler);
	irq_set_chained_handler(irq2, msm_gpio_irq_handler);
	irq_set_irq_wake(irq1, 1);
	irq_set_irq_wake(irq2, 2);
	return 0;
}

static struct platform_device_id gpio_msm_v1_device_ids[] = {
	{ "gpio-msm-7201", (unsigned long)&msm_gpio_7x01_init },
	{ "gpio-msm-7x30", (unsigned long)&msm_gpio_7x30_init },
	{ "gpio-msm-8x50", (unsigned long)&msm_gpio_8x50_init },
	{ }
};
MODULE_DEVICE_TABLE(platform, gpio_msm_v1_device_ids);

static struct platform_driver gpio_msm_v1_driver = {
	.driver = {
		.name = "gpio-msm-v1",
		.owner = THIS_MODULE,
	},
	.probe = gpio_msm_v1_probe,
	.id_table = gpio_msm_v1_device_ids,
};

static int __init gpio_msm_v1_init(void)
{
	return platform_driver_register(&gpio_msm_v1_driver);
}
postcore_initcall(gpio_msm_v1_init);
MODULE_LICENSE("GPL v2");
>>>>>>> refs/remotes/origin/master
