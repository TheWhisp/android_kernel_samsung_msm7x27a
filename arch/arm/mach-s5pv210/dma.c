<<<<<<< HEAD
<<<<<<< HEAD
/*
=======
=======
>>>>>>> refs/remotes/origin/master
/* linux/arch/arm/mach-s5pv210/dma.c
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
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
#include <plat/devs.h>
#include <plat/irqs.h>

#include <mach/map.h>
#include <mach/irqs.h>
<<<<<<< HEAD
<<<<<<< HEAD

#include <plat/s3c-pl330-pdata.h>

static u64 dma_dmamask = DMA_BIT_MASK(32);

static struct resource s5pv210_pdma0_resource[] = {
	[0] = {
		.start  = S5PV210_PA_PDMA0,
		.end    = S5PV210_PA_PDMA0 + SZ_4K,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_PDMA0,
		.end	= IRQ_PDMA0,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct s3c_pl330_platdata s5pv210_pdma0_pdata = {
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
		[9] = DMACH_I2S0_RX,
		[10] = DMACH_I2S0_TX,
		[11] = DMACH_I2S0S_TX,
		[12] = DMACH_I2S1_RX,
		[13] = DMACH_I2S1_TX,
		[14] = DMACH_MAX,
		[15] = DMACH_MAX,
		[16] = DMACH_SPI0_RX,
		[17] = DMACH_SPI0_TX,
		[18] = DMACH_SPI1_RX,
		[19] = DMACH_SPI1_TX,
		[20] = DMACH_MAX,
		[21] = DMACH_MAX,
		[22] = DMACH_AC97_MICIN,
		[23] = DMACH_AC97_PCMIN,
		[24] = DMACH_AC97_PCMOUT,
		[25] = DMACH_MAX,
		[26] = DMACH_PWM,
		[27] = DMACH_SPDIF,
		[28] = DMACH_MAX,
		[29] = DMACH_MAX,
		[30] = DMACH_MAX,
		[31] = DMACH_MAX,
	},
};

static struct platform_device s5pv210_device_pdma0 = {
	.name		= "s3c-pl330",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(s5pv210_pdma0_resource),
	.resource	= s5pv210_pdma0_resource,
	.dev		= {
		.dma_mask = &dma_dmamask,
		.coherent_dma_mask = DMA_BIT_MASK(32),
		.platform_data = &s5pv210_pdma0_pdata,
	},
};

static struct resource s5pv210_pdma1_resource[] = {
	[0] = {
		.start  = S5PV210_PA_PDMA1,
		.end    = S5PV210_PA_PDMA1 + SZ_4K,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_PDMA1,
		.end	= IRQ_PDMA1,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct s3c_pl330_platdata s5pv210_pdma1_pdata = {
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
		[9] = DMACH_I2S0_RX,
		[10] = DMACH_I2S0_TX,
		[11] = DMACH_I2S0S_TX,
		[12] = DMACH_I2S1_RX,
		[13] = DMACH_I2S1_TX,
		[14] = DMACH_I2S2_RX,
		[15] = DMACH_I2S2_TX,
		[16] = DMACH_SPI0_RX,
		[17] = DMACH_SPI0_TX,
		[18] = DMACH_SPI1_RX,
		[19] = DMACH_SPI1_TX,
		[20] = DMACH_MAX,
		[21] = DMACH_MAX,
		[22] = DMACH_PCM0_RX,
		[23] = DMACH_PCM0_TX,
		[24] = DMACH_PCM1_RX,
		[25] = DMACH_PCM1_TX,
		[26] = DMACH_MSM_REQ0,
		[27] = DMACH_MSM_REQ1,
		[28] = DMACH_MSM_REQ2,
		[29] = DMACH_MSM_REQ3,
		[30] = DMACH_PCM2_RX,
		[31] = DMACH_PCM2_TX,
	},
};

static struct platform_device s5pv210_device_pdma1 = {
	.name		= "s3c-pl330",
	.id		= 1,
	.num_resources	= ARRAY_SIZE(s5pv210_pdma1_resource),
	.resource	= s5pv210_pdma1_resource,
	.dev		= {
		.dma_mask = &dma_dmamask,
		.coherent_dma_mask = DMA_BIT_MASK(32),
		.platform_data = &s5pv210_pdma1_pdata,
	},
};

static struct platform_device *s5pv210_dmacs[] __initdata = {
	&s5pv210_device_pdma0,
	&s5pv210_device_pdma1,
};

static int __init s5pv210_dma_init(void)
{
	platform_add_devices(s5pv210_dmacs, ARRAY_SIZE(s5pv210_dmacs));
=======
=======
>>>>>>> refs/remotes/origin/master
#include <mach/dma.h>

static u8 pdma0_peri[] = {
	DMACH_UART0_RX,
	DMACH_UART0_TX,
	DMACH_UART1_RX,
	DMACH_UART1_TX,
	DMACH_UART2_RX,
	DMACH_UART2_TX,
	DMACH_UART3_RX,
	DMACH_UART3_TX,
	DMACH_MAX,
	DMACH_I2S0_RX,
	DMACH_I2S0_TX,
	DMACH_I2S0S_TX,
	DMACH_I2S1_RX,
	DMACH_I2S1_TX,
	DMACH_MAX,
	DMACH_MAX,
	DMACH_SPI0_RX,
	DMACH_SPI0_TX,
	DMACH_SPI1_RX,
	DMACH_SPI1_TX,
	DMACH_MAX,
	DMACH_MAX,
	DMACH_AC97_MICIN,
	DMACH_AC97_PCMIN,
	DMACH_AC97_PCMOUT,
	DMACH_MAX,
	DMACH_PWM,
	DMACH_SPDIF,
};

static struct dma_pl330_platdata s5pv210_pdma0_pdata = {
	.nr_valid_peri = ARRAY_SIZE(pdma0_peri),
	.peri_id = pdma0_peri,
};

static AMBA_AHB_DEVICE(s5pv210_pdma0, "dma-pl330.0", 0x00041330,
	S5PV210_PA_PDMA0, {IRQ_PDMA0}, &s5pv210_pdma0_pdata);

static u8 pdma1_peri[] = {
	DMACH_UART0_RX,
	DMACH_UART0_TX,
	DMACH_UART1_RX,
	DMACH_UART1_TX,
	DMACH_UART2_RX,
	DMACH_UART2_TX,
	DMACH_UART3_RX,
	DMACH_UART3_TX,
	DMACH_MAX,
	DMACH_I2S0_RX,
	DMACH_I2S0_TX,
	DMACH_I2S0S_TX,
	DMACH_I2S1_RX,
	DMACH_I2S1_TX,
	DMACH_I2S2_RX,
	DMACH_I2S2_TX,
	DMACH_SPI0_RX,
	DMACH_SPI0_TX,
	DMACH_SPI1_RX,
	DMACH_SPI1_TX,
	DMACH_MAX,
	DMACH_MAX,
	DMACH_PCM0_RX,
	DMACH_PCM0_TX,
	DMACH_PCM1_RX,
	DMACH_PCM1_TX,
	DMACH_MSM_REQ0,
	DMACH_MSM_REQ1,
	DMACH_MSM_REQ2,
	DMACH_MSM_REQ3,
	DMACH_PCM2_RX,
	DMACH_PCM2_TX,
};

static struct dma_pl330_platdata s5pv210_pdma1_pdata = {
	.nr_valid_peri = ARRAY_SIZE(pdma1_peri),
	.peri_id = pdma1_peri,
};

static AMBA_AHB_DEVICE(s5pv210_pdma1, "dma-pl330.1", 0x00041330,
	S5PV210_PA_PDMA1, {IRQ_PDMA1}, &s5pv210_pdma1_pdata);

static int __init s5pv210_dma_init(void)
{
	dma_cap_set(DMA_SLAVE, s5pv210_pdma0_pdata.cap_mask);
	dma_cap_set(DMA_CYCLIC, s5pv210_pdma0_pdata.cap_mask);
	amba_device_register(&s5pv210_pdma0_device, &iomem_resource);

	dma_cap_set(DMA_SLAVE, s5pv210_pdma1_pdata.cap_mask);
	dma_cap_set(DMA_CYCLIC, s5pv210_pdma1_pdata.cap_mask);
	amba_device_register(&s5pv210_pdma1_device, &iomem_resource);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}
arch_initcall(s5pv210_dma_init);
