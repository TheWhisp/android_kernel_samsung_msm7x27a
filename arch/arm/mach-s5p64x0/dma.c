/* linux/arch/arm/mach-s5p64x0/dma.c
 *
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * Copyright (C) 2010 Samsung Electronics Co. Ltd.
 *	Jaswinder Singh <jassi.brar@samsung.com>
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/dma-mapping.h>
#include <linux/amba/bus.h>
#include <linux/amba/pl330.h>

#include <asm/irq.h>
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#include <mach/map.h>
#include <mach/irqs.h>
#include <mach/regs-clock.h>
<<<<<<< HEAD
<<<<<<< HEAD

#include <plat/devs.h>
#include <plat/s3c-pl330-pdata.h>

static u64 dma_dmamask = DMA_BIT_MASK(32);

static struct resource s5p64x0_pdma_resource[] = {
	[0] = {
		.start	= S5P64X0_PA_PDMA,
		.end	= S5P64X0_PA_PDMA + SZ_4K,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_DMA0,
		.end	= IRQ_DMA0,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct s3c_pl330_platdata s5p6440_pdma_pdata = {
	.peri = {
		[0] = DMACH_UART0_RX,
		[1] = DMACH_UART0_TX,
		[2] = DMACH_UART1_RX,
		[3] = DMACH_UART1_TX,
		[4] = DMACH_UART2_RX,
		[5] = DMACH_UART2_TX,
		[6] = DMACH_UART3_RX,
		[7] = DMACH_UART3_TX,
		[8] = DMACH_MAX,
		[9] = DMACH_MAX,
		[10] = DMACH_PCM0_TX,
		[11] = DMACH_PCM0_RX,
		[12] = DMACH_I2S0_TX,
		[13] = DMACH_I2S0_RX,
		[14] = DMACH_SPI0_TX,
		[15] = DMACH_SPI0_RX,
		[16] = DMACH_MAX,
		[17] = DMACH_MAX,
		[18] = DMACH_MAX,
		[19] = DMACH_MAX,
		[20] = DMACH_SPI1_TX,
		[21] = DMACH_SPI1_RX,
		[22] = DMACH_MAX,
		[23] = DMACH_MAX,
		[24] = DMACH_MAX,
		[25] = DMACH_MAX,
		[26] = DMACH_MAX,
		[27] = DMACH_MAX,
		[28] = DMACH_MAX,
		[29] = DMACH_PWM,
		[30] = DMACH_MAX,
		[31] = DMACH_MAX,
	},
};

static struct s3c_pl330_platdata s5p6450_pdma_pdata = {
	.peri = {
		[0] = DMACH_UART0_RX,
		[1] = DMACH_UART0_TX,
		[2] = DMACH_UART1_RX,
		[3] = DMACH_UART1_TX,
		[4] = DMACH_UART2_RX,
		[5] = DMACH_UART2_TX,
		[6] = DMACH_UART3_RX,
		[7] = DMACH_UART3_TX,
		[8] = DMACH_UART4_RX,
		[9] = DMACH_UART4_TX,
		[10] = DMACH_PCM0_TX,
		[11] = DMACH_PCM0_RX,
		[12] = DMACH_I2S0_TX,
		[13] = DMACH_I2S0_RX,
		[14] = DMACH_SPI0_TX,
		[15] = DMACH_SPI0_RX,
		[16] = DMACH_PCM1_TX,
		[17] = DMACH_PCM1_RX,
		[18] = DMACH_PCM2_TX,
		[19] = DMACH_PCM2_RX,
		[20] = DMACH_SPI1_TX,
		[21] = DMACH_SPI1_RX,
		[22] = DMACH_USI_TX,
		[23] = DMACH_USI_RX,
		[24] = DMACH_MAX,
		[25] = DMACH_I2S1_TX,
		[26] = DMACH_I2S1_RX,
		[27] = DMACH_I2S2_TX,
		[28] = DMACH_I2S2_RX,
		[29] = DMACH_PWM,
		[30] = DMACH_UART5_RX,
		[31] = DMACH_UART5_TX,
	},
};

static struct platform_device s5p64x0_device_pdma = {
	.name		= "s3c-pl330",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(s5p64x0_pdma_resource),
	.resource	= s5p64x0_pdma_resource,
	.dev		= {
		.dma_mask = &dma_dmamask,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
};

static int __init s5p64x0_dma_init(void)
{
	unsigned int id;

	id = __raw_readl(S5P64X0_SYS_ID) & 0xFF000;

	if (id == 0x50000)
		s5p64x0_device_pdma.dev.platform_data = &s5p6450_pdma_pdata;
	else
		s5p64x0_device_pdma.dev.platform_data = &s5p6440_pdma_pdata;

	platform_device_register(&s5p64x0_device_pdma);
=======
=======
>>>>>>> refs/remotes/origin/master
#include <mach/dma.h>

#include <plat/cpu.h>
#include <plat/devs.h>
#include <plat/irqs.h>

<<<<<<< HEAD
static u64 dma_dmamask = DMA_BIT_MASK(32);

=======
>>>>>>> refs/remotes/origin/master
static u8 s5p6440_pdma_peri[] = {
	DMACH_UART0_RX,
	DMACH_UART0_TX,
	DMACH_UART1_RX,
	DMACH_UART1_TX,
	DMACH_UART2_RX,
	DMACH_UART2_TX,
	DMACH_UART3_RX,
	DMACH_UART3_TX,
	DMACH_MAX,
	DMACH_MAX,
	DMACH_PCM0_TX,
	DMACH_PCM0_RX,
	DMACH_I2S0_TX,
	DMACH_I2S0_RX,
	DMACH_SPI0_TX,
	DMACH_SPI0_RX,
	DMACH_MAX,
	DMACH_MAX,
	DMACH_MAX,
	DMACH_MAX,
	DMACH_SPI1_TX,
	DMACH_SPI1_RX,
};

static struct dma_pl330_platdata s5p6440_pdma_pdata = {
	.nr_valid_peri = ARRAY_SIZE(s5p6440_pdma_peri),
	.peri_id = s5p6440_pdma_peri,
};

static u8 s5p6450_pdma_peri[] = {
	DMACH_UART0_RX,
	DMACH_UART0_TX,
	DMACH_UART1_RX,
	DMACH_UART1_TX,
	DMACH_UART2_RX,
	DMACH_UART2_TX,
	DMACH_UART3_RX,
	DMACH_UART3_TX,
	DMACH_UART4_RX,
	DMACH_UART4_TX,
	DMACH_PCM0_TX,
	DMACH_PCM0_RX,
	DMACH_I2S0_TX,
	DMACH_I2S0_RX,
	DMACH_SPI0_TX,
	DMACH_SPI0_RX,
	DMACH_PCM1_TX,
	DMACH_PCM1_RX,
	DMACH_PCM2_TX,
	DMACH_PCM2_RX,
	DMACH_SPI1_TX,
	DMACH_SPI1_RX,
	DMACH_USI_TX,
	DMACH_USI_RX,
	DMACH_MAX,
	DMACH_I2S1_TX,
	DMACH_I2S1_RX,
	DMACH_I2S2_TX,
	DMACH_I2S2_RX,
	DMACH_PWM,
	DMACH_UART5_RX,
	DMACH_UART5_TX,
};

static struct dma_pl330_platdata s5p6450_pdma_pdata = {
	.nr_valid_peri = ARRAY_SIZE(s5p6450_pdma_peri),
	.peri_id = s5p6450_pdma_peri,
};

static AMBA_AHB_DEVICE(s5p64x0_pdma, "dma-pl330", 0x00041330,
	S5P64X0_PA_PDMA, {IRQ_DMA0}, NULL);

static int __init s5p64x0_dma_init(void)
{
	if (soc_is_s5p6450()) {
		dma_cap_set(DMA_SLAVE, s5p6450_pdma_pdata.cap_mask);
		dma_cap_set(DMA_CYCLIC, s5p6450_pdma_pdata.cap_mask);
		s5p64x0_pdma_device.dev.platform_data = &s5p6450_pdma_pdata;
	} else {
		dma_cap_set(DMA_SLAVE, s5p6440_pdma_pdata.cap_mask);
		dma_cap_set(DMA_CYCLIC, s5p6440_pdma_pdata.cap_mask);
		s5p64x0_pdma_device.dev.platform_data = &s5p6440_pdma_pdata;
	}

	amba_device_register(&s5p64x0_pdma_device, &iomem_resource);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}
arch_initcall(s5p64x0_dma_init);
