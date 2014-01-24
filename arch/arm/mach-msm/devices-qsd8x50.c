/*
 * Copyright (C) 2008 Google, Inc.
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (c) 2008-2011, The Linux Foundation. All rights reserved.
=======
 * Copyright (c) 2008-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright (c) 2008-2011, Code Aurora Forum. All rights reserved.
>>>>>>> refs/remotes/origin/master
=======
 * Copyright (c) 2008-2012, The Linux Foundation. All rights reserved.
>>>>>>> refs/remotes/origin/cm-11.0
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

#include <linux/kernel.h>
#include <linux/platform_device.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/msm_kgsl.h>

#include <linux/dma-mapping.h>
#include <asm/clkdev.h>
#include <mach/irqs.h>
#include <mach/msm_iomap.h>
#include <mach/dma.h>
#include <mach/board.h>

#include "devices.h"
<<<<<<< HEAD
#include "gpio_hw.h"
=======
>>>>>>> refs/remotes/origin/cm-10.0

#include <asm/mach/flash.h>

#include <asm/mach/mmc.h>
#include <mach/msm_hsusb.h>
#include <mach/usbdiag.h>
#include <mach/rpc_hsusb.h>
<<<<<<< HEAD
=======
#include "pm.h"

struct platform_device msm8x50_device_acpuclk = {
	.name		= "acpuclk-8x50",
	.id		= -1,
};
>>>>>>> refs/remotes/origin/cm-10.0

static struct resource resources_uart1[] = {
	{
		.start	= INT_UART1,
		.end	= INT_UART1,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= MSM_UART1_PHYS,
		.end	= MSM_UART1_PHYS + MSM_UART1_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
};

static struct resource resources_uart2[] = {
	{
		.start	= INT_UART2,
		.end	= INT_UART2,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= MSM_UART2_PHYS,
		.end	= MSM_UART2_PHYS + MSM_UART2_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
};

=======
#include <linux/clkdev.h>
#include <linux/dma-mapping.h>
=======
#include <linux/msm_kgsl.h>
>>>>>>> refs/remotes/origin/cm-11.0

#include <linux/dma-mapping.h>
#include <asm/clkdev.h>
#include <mach/irqs.h>
#include <mach/msm_iomap.h>
#include <mach/dma.h>

#include "devices.h"
#include "common.h"

#include <asm/mach/flash.h>

<<<<<<< HEAD
#include <linux/platform_data/mmc-msm_sdcc.h>
#include "clock.h"
#include "clock-pcom.h"
=======
#include <asm/mach/mmc.h>
#include <mach/msm_hsusb.h>
#include <mach/usbdiag.h>
#include <mach/rpc_hsusb.h>
#include "pm.h"

struct platform_device msm8x50_device_acpuclk = {
	.name		= "acpuclk-8x50",
	.id		= -1,
};

static struct resource resources_uart1[] = {
	{
		.start	= INT_UART1,
		.end	= INT_UART1,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= MSM_UART1_PHYS,
		.end	= MSM_UART1_PHYS + MSM_UART1_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
};

static struct resource resources_uart2[] = {
	{
		.start	= INT_UART2,
		.end	= INT_UART2,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= MSM_UART2_PHYS,
		.end	= MSM_UART2_PHYS + MSM_UART2_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
};
>>>>>>> refs/remotes/origin/cm-11.0

static struct resource msm_gpio_resources[] = {
	{
		.start	= 64 + 165 + 9,
		.end	= 64 + 165 + 9,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= 64 + 165 + 10,
		.end	= 64 + 165 + 10,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= 0xa9000800,
		.end	= 0xa9000800 + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
		.name  = "gpio1"
	},
	{
		.start	= 0xa9100C00,
		.end	= 0xa9100C00 + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
		.name  = "gpio2"
	},
};

struct platform_device msm_device_gpio_8x50 = {
	.name	= "gpio-msm-8x50",
	.num_resources	= ARRAY_SIZE(msm_gpio_resources),
	.resource	= msm_gpio_resources,
};

>>>>>>> refs/remotes/origin/master
static struct resource resources_uart3[] = {
	{
		.start	= INT_UART3,
		.end	= INT_UART3,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= MSM_UART3_PHYS,
		.end	= MSM_UART3_PHYS + MSM_UART3_SIZE - 1,
		.flags	= IORESOURCE_MEM,
		.name  = "uart_resource"
	},
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
struct platform_device msm_device_uart1 = {
	.name	= "msm_serial",
	.id	= 0,
	.num_resources	= ARRAY_SIZE(resources_uart1),
	.resource	= resources_uart1,
};

struct platform_device msm_device_uart2 = {
	.name	= "msm_serial",
	.id	= 1,
	.num_resources	= ARRAY_SIZE(resources_uart2),
	.resource	= resources_uart2,
};

<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/master
=======
>>>>>>> refs/remotes/origin/cm-11.0
struct platform_device msm_device_uart3 = {
	.name	= "msm_serial",
	.id	= 2,
	.num_resources	= ARRAY_SIZE(resources_uart3),
	.resource	= resources_uart3,
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> refs/remotes/origin/cm-11.0
#define MSM_UART1DM_PHYS      0xA0200000
#define MSM_UART2DM_PHYS      0xA0900000
static struct resource msm_uart1_dm_resources[] = {
	{
		.start = MSM_UART1DM_PHYS,
		.end   = MSM_UART1DM_PHYS + PAGE_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	{
		.start = INT_UART1DM_IRQ,
		.end   = INT_UART1DM_IRQ,
		.flags = IORESOURCE_IRQ,
	},
	{
		.start = INT_UART1DM_RX,
		.end   = INT_UART1DM_RX,
		.flags = IORESOURCE_IRQ,
	},
	{
		.start = DMOV_HSUART1_TX_CHAN,
		.end   = DMOV_HSUART1_RX_CHAN,
		.name  = "uartdm_channels",
		.flags = IORESOURCE_DMA,
	},
	{
		.start = DMOV_HSUART1_TX_CRCI,
		.end   = DMOV_HSUART1_RX_CRCI,
		.name  = "uartdm_crci",
		.flags = IORESOURCE_DMA,
	},
<<<<<<< HEAD
};

static u64 msm_uart_dm1_dma_mask = DMA_BIT_MASK(32);

struct platform_device msm_device_uart_dm1 = {
	.name = "msm_serial_hs",
	.id = 0,
	.num_resources = ARRAY_SIZE(msm_uart1_dm_resources),
	.resource = msm_uart1_dm_resources,
	.dev		= {
		.dma_mask = &msm_uart_dm1_dma_mask,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
};

static struct resource msm_uart2_dm_resources[] = {
	{
		.start = MSM_UART2DM_PHYS,
		.end   = MSM_UART2DM_PHYS + PAGE_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	{
		.start = INT_UART2DM_IRQ,
		.end   = INT_UART2DM_IRQ,
		.flags = IORESOURCE_IRQ,
	},
	{
		.start = INT_UART2DM_RX,
		.end   = INT_UART2DM_RX,
		.flags = IORESOURCE_IRQ,
	},
	{
		.start = DMOV_HSUART2_TX_CHAN,
		.end   = DMOV_HSUART2_RX_CHAN,
		.name  = "uartdm_channels",
		.flags = IORESOURCE_DMA,
	},
	{
		.start = DMOV_HSUART2_TX_CRCI,
		.end   = DMOV_HSUART2_RX_CRCI,
		.name  = "uartdm_crci",
		.flags = IORESOURCE_DMA,
	},
};

static u64 msm_uart_dm2_dma_mask = DMA_BIT_MASK(32);

struct platform_device msm_device_uart_dm2 = {
	.name = "msm_serial_hs",
	.id = 1,
	.num_resources = ARRAY_SIZE(msm_uart2_dm_resources),
	.resource = msm_uart2_dm_resources,
	.dev		= {
		.dma_mask = &msm_uart_dm2_dma_mask,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
};

#define MSM_I2C_SIZE          SZ_4K
#define MSM_I2C_PHYS          0xA9900000

static struct resource resources_i2c[] = {
	{
		.start	= MSM_I2C_PHYS,
		.end	= MSM_I2C_PHYS + MSM_I2C_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_PWB_I2C,
		.end	= INT_PWB_I2C,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device msm_device_i2c = {
	.name		= "msm_i2c",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(resources_i2c),
	.resource	= resources_i2c,
};

#define MSM_HSUSB_PHYS        0xA0800000
static struct resource resources_hsusb_otg[] = {
	{
		.start	= MSM_HSUSB_PHYS,
		.end	= MSM_HSUSB_PHYS + SZ_1K - 1,
=======
struct platform_device msm_device_smd = {
	.name   = "msm_smd",
	.id     = -1,
=======
>>>>>>> refs/remotes/origin/cm-11.0
};

static u64 msm_uart_dm1_dma_mask = DMA_BIT_MASK(32);

struct platform_device msm_device_uart_dm1 = {
	.name = "msm_serial_hs",
	.id = 0,
	.num_resources = ARRAY_SIZE(msm_uart1_dm_resources),
	.resource = msm_uart1_dm_resources,
	.dev		= {
		.dma_mask = &msm_uart_dm1_dma_mask,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
};

static struct resource msm_uart2_dm_resources[] = {
	{
		.start = MSM_UART2DM_PHYS,
		.end   = MSM_UART2DM_PHYS + PAGE_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	{
		.start = INT_UART2DM_IRQ,
		.end   = INT_UART2DM_IRQ,
		.flags = IORESOURCE_IRQ,
	},
	{
		.start = INT_UART2DM_RX,
		.end   = INT_UART2DM_RX,
		.flags = IORESOURCE_IRQ,
	},
	{
		.start = DMOV_HSUART2_TX_CHAN,
		.end   = DMOV_HSUART2_RX_CHAN,
		.name  = "uartdm_channels",
		.flags = IORESOURCE_DMA,
	},
	{
		.start = DMOV_HSUART2_TX_CRCI,
		.end   = DMOV_HSUART2_RX_CRCI,
		.name  = "uartdm_crci",
		.flags = IORESOURCE_DMA,
	},
};

static u64 msm_uart_dm2_dma_mask = DMA_BIT_MASK(32);

struct platform_device msm_device_uart_dm2 = {
	.name = "msm_serial_hs",
	.id = 1,
	.num_resources = ARRAY_SIZE(msm_uart2_dm_resources),
	.resource = msm_uart2_dm_resources,
	.dev		= {
		.dma_mask = &msm_uart_dm2_dma_mask,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
};

#define MSM_I2C_SIZE          SZ_4K
#define MSM_I2C_PHYS          0xA9900000

static struct resource resources_i2c[] = {
	{
		.start	= MSM_I2C_PHYS,
		.end	= MSM_I2C_PHYS + MSM_I2C_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_PWB_I2C,
		.end	= INT_PWB_I2C,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device msm_device_i2c = {
	.name		= "msm_i2c",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(resources_i2c),
	.resource	= resources_i2c,
};

#define MSM_HSUSB_PHYS        0xA0800000
static struct resource resources_hsusb_otg[] = {
	{
		.start	= MSM_HSUSB_PHYS,
<<<<<<< HEAD
		.end	= MSM_HSUSB_PHYS + MSM_HSUSB_SIZE,
>>>>>>> refs/remotes/origin/master
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_USB_HS,
		.end	= INT_USB_HS,
		.flags	= IORESOURCE_IRQ,
	},
};

<<<<<<< HEAD
static u64 dma_mask = 0xffffffffULL;
struct platform_device msm_device_hsusb_otg = {
	.name		= "msm_hsusb_otg",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(resources_hsusb_otg),
	.resource	= resources_hsusb_otg,
	.dev		= {
		.dma_mask 		= &dma_mask,
		.coherent_dma_mask	= 0xffffffffULL,
	},
};

static struct resource resources_hsusb_peripheral[] = {
	{
		.start	= MSM_HSUSB_PHYS,
=======
>>>>>>> refs/remotes/origin/cm-11.0
		.end	= MSM_HSUSB_PHYS + SZ_1K - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_USB_HS,
		.end	= INT_USB_HS,
		.flags	= IORESOURCE_IRQ,
	},
};

<<<<<<< HEAD
static struct resource resources_gadget_peripheral[] = {
	{
		.start	= MSM_HSUSB_PHYS,
		.end	= MSM_HSUSB_PHYS + SZ_1K - 1,
=======
struct platform_device msm_device_otg = {
	.name		= "msm_otg",
=======
static u64 dma_mask = 0xffffffffULL;
struct platform_device msm_device_hsusb_otg = {
	.name		= "msm_hsusb_otg",
>>>>>>> refs/remotes/origin/cm-11.0
	.id		= -1,
	.num_resources	= ARRAY_SIZE(resources_hsusb_otg),
	.resource	= resources_hsusb_otg,
	.dev		= {
		.dma_mask 		= &dma_mask,
		.coherent_dma_mask	= 0xffffffffULL,
	},
};

static struct resource resources_hsusb_peripheral[] = {
	{
		.start	= MSM_HSUSB_PHYS,
<<<<<<< HEAD
		.end	= MSM_HSUSB_PHYS + MSM_HSUSB_SIZE,
>>>>>>> refs/remotes/origin/master
=======
		.end	= MSM_HSUSB_PHYS + SZ_1K - 1,
>>>>>>> refs/remotes/origin/cm-11.0
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_USB_HS,
		.end	= INT_USB_HS,
		.flags	= IORESOURCE_IRQ,
	},
};

<<<<<<< HEAD
<<<<<<< HEAD
struct platform_device msm_device_hsusb_peripheral = {
	.name		= "msm_hsusb_peripheral",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(resources_hsusb_peripheral),
	.resource	= resources_hsusb_peripheral,
	.dev		= {
		.dma_mask 		= &dma_mask,
		.coherent_dma_mask	= 0xffffffffULL,
	},
};

struct platform_device msm_device_gadget_peripheral = {
	.name		= "msm_hsusb",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(resources_gadget_peripheral),
	.resource	= resources_gadget_peripheral,
	.dev		= {
		.dma_mask 		= &dma_mask,
		.coherent_dma_mask	= 0xffffffffULL,
	},
};

#ifdef CONFIG_USB_FS_HOST
#define MSM_HS2USB_PHYS        0xA0800400
static struct resource resources_hsusb_host2[] = {
	{
		.start	= MSM_HS2USB_PHYS,
		.end	= MSM_HS2USB_PHYS + SZ_1K - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_USB_OTG,
		.end	= INT_USB_OTG,
=======
static struct resource resources_gadget_peripheral[] = {
	{
		.start	= MSM_HSUSB_PHYS,
		.end	= MSM_HSUSB_PHYS + SZ_1K - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_USB_HS,
		.end	= INT_USB_HS,
>>>>>>> refs/remotes/origin/cm-11.0
		.flags	= IORESOURCE_IRQ,
	},
};

<<<<<<< HEAD
struct platform_device msm_device_hsusb_host2 = {
	.name		= "msm_hsusb_host",
	.id		= 1,
	.num_resources	= ARRAY_SIZE(resources_hsusb_host2),
	.resource	= resources_hsusb_host2,
=======
struct platform_device msm_device_hsusb_peripheral = {
	.name		= "msm_hsusb_peripheral",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(resources_hsusb_peripheral),
	.resource	= resources_hsusb_peripheral,
>>>>>>> refs/remotes/origin/cm-11.0
	.dev		= {
		.dma_mask 		= &dma_mask,
		.coherent_dma_mask	= 0xffffffffULL,
	},
};
<<<<<<< HEAD
#endif

static struct resource resources_hsusb_host[] = {
	{
		.start	= MSM_HSUSB_PHYS,
		.end	= MSM_HSUSB_PHYS + SZ_1K - 1,
=======
struct platform_device msm_device_hsusb = {
=======

struct platform_device msm_device_gadget_peripheral = {
>>>>>>> refs/remotes/origin/cm-11.0
	.name		= "msm_hsusb",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(resources_gadget_peripheral),
	.resource	= resources_gadget_peripheral,
	.dev		= {
		.dma_mask 		= &dma_mask,
		.coherent_dma_mask	= 0xffffffffULL,
	},
};

#ifdef CONFIG_USB_FS_HOST
#define MSM_HS2USB_PHYS        0xA0800400
static struct resource resources_hsusb_host2[] = {
	{
		.start	= MSM_HS2USB_PHYS,
		.end	= MSM_HS2USB_PHYS + SZ_1K - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_USB_OTG,
		.end	= INT_USB_OTG,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device msm_device_hsusb_host2 = {
	.name		= "msm_hsusb_host",
	.id		= 1,
	.num_resources	= ARRAY_SIZE(resources_hsusb_host2),
	.resource	= resources_hsusb_host2,
	.dev		= {
		.dma_mask 		= &dma_mask,
		.coherent_dma_mask	= 0xffffffffULL,
	},
};
#endif

static struct resource resources_hsusb_host[] = {
	{
		.start	= MSM_HSUSB_PHYS,
<<<<<<< HEAD
		.end	= MSM_HSUSB_PHYS + MSM_HSUSB_SIZE,
>>>>>>> refs/remotes/origin/master
=======
		.end	= MSM_HSUSB_PHYS + SZ_1K - 1,
>>>>>>> refs/remotes/origin/cm-11.0
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_USB_HS,
		.end	= INT_USB_HS,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device msm_device_hsusb_host = {
	.name		= "msm_hsusb_host",
<<<<<<< HEAD
<<<<<<< HEAD
	.id		= 0,
	.num_resources	= ARRAY_SIZE(resources_hsusb_host),
	.resource	= resources_hsusb_host,
	.dev		= {
		.dma_mask 		= &dma_mask,
		.coherent_dma_mask	= 0xffffffffULL,
	},
};

static struct platform_device *msm_host_devices[] = {
	&msm_device_hsusb_host,
#ifdef CONFIG_USB_FS_HOST
	&msm_device_hsusb_host2,
#endif
};

int msm_add_host(unsigned int host, struct msm_usb_host_platform_data *plat)
{
	struct platform_device	*pdev;

	pdev = msm_host_devices[host];
	if (!pdev)
		return -ENODEV;
	pdev->dev.platform_data = plat;
	return platform_device_register(pdev);
}

#ifdef CONFIG_USB_ANDROID
struct usb_diag_platform_data usb_diag_pdata = {
	.ch_name = DIAG_LEGACY,
	.update_pid_and_serial_num = usb_diag_update_pid_and_serial_num,
};

struct platform_device usb_diag_device = {
	.name	= "usb_diag",
	.id	= -1,
	.dev	= {
		.platform_data = &usb_diag_pdata,
	},
};
#endif

#ifdef CONFIG_USB_F_SERIAL
static struct usb_gadget_fserial_platform_data fserial_pdata = {
	.no_ports	= 2,
};

struct platform_device usb_gadget_fserial_device = {
	.name	= "usb_fserial",
	.id	= -1,
	.dev	= {
		.platform_data = &fserial_pdata,
	},
};
#endif

#define MSM_NAND_PHYS		0xA0A00000
static struct resource resources_nand[] = {
	[0] = {
		.name   = "msm_nand_dmac",
		.start	= DMOV_NAND_CHAN,
		.end	= DMOV_NAND_CHAN,
		.flags	= IORESOURCE_DMA,
	},
	[1] = {
		.name   = "msm_nand_phys",
		.start  = MSM_NAND_PHYS,
		.end    = MSM_NAND_PHYS + 0x7FF,
		.flags  = IORESOURCE_MEM,
	},
};

static struct resource resources_otg[] = {
	{
		.start	= MSM_HSUSB_PHYS,
		.end	= MSM_HSUSB_PHYS + SZ_1K - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_USB_HS,
		.end	= INT_USB_HS,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device msm_device_otg = {
	.name		= "msm_otg",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(resources_otg),
	.resource	= resources_otg,
	.dev		= {
		.coherent_dma_mask	= 0xffffffffULL,
	},
};

struct flash_platform_data msm_nand_data = {
	.parts		= NULL,
	.nr_parts	= 0,
};

struct platform_device msm_device_nand = {
	.name		= "msm_nand",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(resources_nand),
	.resource	= resources_nand,
	.dev		= {
		.platform_data	= &msm_nand_data,
	},
};

<<<<<<< HEAD
=======
static struct msm_pm_irq_calls qsd8x50_pm_irq_calls = {
	.irq_pending = msm_irq_pending,
	.idle_sleep_allowed = msm_irq_idle_sleep_allowed,
	.enter_sleep1 = msm_irq_enter_sleep1,
	.enter_sleep2 = msm_irq_enter_sleep2,
	.exit_sleep1 = msm_irq_exit_sleep1,
	.exit_sleep2 = msm_irq_exit_sleep2,
	.exit_sleep3 = msm_irq_exit_sleep3,
};

void __init msm_pm_register_irqs(void)
{
	msm_pm_set_irq_extns(&qsd8x50_pm_irq_calls);
}

>>>>>>> refs/remotes/origin/cm-10.0
struct platform_device msm_device_smd = {
	.name	= "msm_smd",
	.id	= -1,
};

static struct resource msm_dmov_resource[] = {
	{
		.start = INT_ADM_AARM,
		.flags = IORESOURCE_IRQ,
	},
	{
		.start = 0xA9700000,
		.end = 0xA9700000 + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
};

static struct msm_dmov_pdata msm_dmov_pdata = {
	.sd = 3,
	.sd_size = 0x400,
};

struct platform_device msm_device_dmov = {
	.name	= "msm_dmov",
	.id	= -1,
	.resource = msm_dmov_resource,
	.num_resources = ARRAY_SIZE(msm_dmov_resource),
	.dev = {
		.platform_data = &msm_dmov_pdata,
	},
};

#define MSM_SDC1_BASE         0xA0300000
#define MSM_SDC2_BASE         0xA0400000
#define MSM_SDC3_BASE         0xA0500000
#define MSM_SDC4_BASE         0xA0600000
static struct resource resources_sdc1[] = {
	{
		.start	= MSM_SDC1_BASE,
		.end	= MSM_SDC1_BASE + SZ_4K - 1,
=======
	.id		= -1,
=======
	.id		= 0,
>>>>>>> refs/remotes/origin/cm-11.0
	.num_resources	= ARRAY_SIZE(resources_hsusb_host),
	.resource	= resources_hsusb_host,
	.dev		= {
		.dma_mask 		= &dma_mask,
		.coherent_dma_mask	= 0xffffffffULL,
	},
};

static struct platform_device *msm_host_devices[] = {
	&msm_device_hsusb_host,
#ifdef CONFIG_USB_FS_HOST
	&msm_device_hsusb_host2,
#endif
};

int msm_add_host(unsigned int host, struct msm_usb_host_platform_data *plat)
{
	struct platform_device	*pdev;

	pdev = msm_host_devices[host];
	if (!pdev)
		return -ENODEV;
	pdev->dev.platform_data = plat;
	return platform_device_register(pdev);
}

#ifdef CONFIG_USB_ANDROID
struct usb_diag_platform_data usb_diag_pdata = {
	.ch_name = DIAG_LEGACY,
	.update_pid_and_serial_num = usb_diag_update_pid_and_serial_num,
};

struct platform_device usb_diag_device = {
	.name	= "usb_diag",
	.id	= -1,
	.dev	= {
		.platform_data = &usb_diag_pdata,
	},
};
#endif

#ifdef CONFIG_USB_F_SERIAL
static struct usb_gadget_fserial_platform_data fserial_pdata = {
	.no_ports	= 2,
};

struct platform_device usb_gadget_fserial_device = {
	.name	= "usb_fserial",
	.id	= -1,
	.dev	= {
		.platform_data = &fserial_pdata,
	},
};
#endif

#define MSM_NAND_PHYS		0xA0A00000
static struct resource resources_nand[] = {
	[0] = {
		.name   = "msm_nand_dmac",
		.start	= DMOV_NAND_CHAN,
		.end	= DMOV_NAND_CHAN,
		.flags	= IORESOURCE_DMA,
	},
	[1] = {
		.name   = "msm_nand_phys",
		.start  = MSM_NAND_PHYS,
		.end    = MSM_NAND_PHYS + 0x7FF,
		.flags  = IORESOURCE_MEM,
	},
};

static struct resource resources_otg[] = {
	{
		.start	= MSM_HSUSB_PHYS,
		.end	= MSM_HSUSB_PHYS + SZ_1K - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_USB_HS,
		.end	= INT_USB_HS,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device msm_device_otg = {
	.name		= "msm_otg",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(resources_otg),
	.resource	= resources_otg,
	.dev		= {
		.coherent_dma_mask	= 0xffffffffULL,
	},
};

struct flash_platform_data msm_nand_data = {
	.parts		= NULL,
	.nr_parts	= 0,
};

struct platform_device msm_device_nand = {
	.name		= "msm_nand",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(resources_nand),
	.resource	= resources_nand,
	.dev		= {
		.platform_data	= &msm_nand_data,
	},
};

static struct msm_pm_irq_calls qsd8x50_pm_irq_calls = {
	.irq_pending = msm_irq_pending,
	.idle_sleep_allowed = msm_irq_idle_sleep_allowed,
	.enter_sleep1 = msm_irq_enter_sleep1,
	.enter_sleep2 = msm_irq_enter_sleep2,
	.exit_sleep1 = msm_irq_exit_sleep1,
	.exit_sleep2 = msm_irq_exit_sleep2,
	.exit_sleep3 = msm_irq_exit_sleep3,
};

void __init msm_pm_register_irqs(void)
{
	msm_pm_set_irq_extns(&qsd8x50_pm_irq_calls);
}

struct platform_device msm_device_smd = {
	.name	= "msm_smd",
	.id	= -1,
};

static struct resource msm_dmov_resource[] = {
	{
		.start = INT_ADM_AARM,
		.flags = IORESOURCE_IRQ,
	},
	{
		.start = 0xA9700000,
		.end = 0xA9700000 + SZ_4K - 1,
		.flags = IORESOURCE_MEM,
	},
};

static struct msm_dmov_pdata msm_dmov_pdata = {
	.sd = 3,
	.sd_size = 0x400,
};

struct platform_device msm_device_dmov = {
	.name	= "msm_dmov",
	.id	= -1,
	.resource = msm_dmov_resource,
	.num_resources = ARRAY_SIZE(msm_dmov_resource),
	.dev = {
		.platform_data = &msm_dmov_pdata,
	},
};

#define MSM_SDC1_BASE         0xA0300000
#define MSM_SDC2_BASE         0xA0400000
#define MSM_SDC3_BASE         0xA0500000
#define MSM_SDC4_BASE         0xA0600000
static struct resource resources_sdc1[] = {
	{
<<<<<<< HEAD
		.start	= MSM_SDC1_PHYS,
		.end	= MSM_SDC1_PHYS + MSM_SDC1_SIZE - 1,
>>>>>>> refs/remotes/origin/master
=======
		.start	= MSM_SDC1_BASE,
		.end	= MSM_SDC1_BASE + SZ_4K - 1,
>>>>>>> refs/remotes/origin/cm-11.0
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_SDC1_0,
<<<<<<< HEAD
<<<<<<< HEAD
		.end	= INT_SDC1_1,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name	= "sdcc_dma_chnl",
		.start	= DMOV_SDC1_CHAN,
		.end	= DMOV_SDC1_CHAN,
		.flags	= IORESOURCE_DMA,
	},
	{
		.name	= "sdcc_dma_crci",
		.start	= DMOV_SDC1_CRCI,
		.end	= DMOV_SDC1_CRCI,
		.flags	= IORESOURCE_DMA,
	}
=======
		.end	= INT_SDC1_0,
=======
		.end	= INT_SDC1_1,
>>>>>>> refs/remotes/origin/cm-11.0
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name	= "sdcc_dma_chnl",
		.start	= DMOV_SDC1_CHAN,
		.end	= DMOV_SDC1_CHAN,
		.flags	= IORESOURCE_DMA,
	},
	{
		.name	= "sdcc_dma_crci",
		.start	= DMOV_SDC1_CRCI,
		.end	= DMOV_SDC1_CRCI,
		.flags	= IORESOURCE_DMA,
<<<<<<< HEAD
	},
>>>>>>> refs/remotes/origin/master
=======
	}
>>>>>>> refs/remotes/origin/cm-11.0
};

static struct resource resources_sdc2[] = {
	{
<<<<<<< HEAD
<<<<<<< HEAD
		.start	= MSM_SDC2_BASE,
		.end	= MSM_SDC2_BASE + SZ_4K - 1,
=======
		.start	= MSM_SDC2_PHYS,
		.end	= MSM_SDC2_PHYS + MSM_SDC2_SIZE - 1,
>>>>>>> refs/remotes/origin/master
=======
		.start	= MSM_SDC2_BASE,
		.end	= MSM_SDC2_BASE + SZ_4K - 1,
>>>>>>> refs/remotes/origin/cm-11.0
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_SDC2_0,
<<<<<<< HEAD
<<<<<<< HEAD
		.end	= INT_SDC2_1,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name	= "sdcc_dma_chnl",
		.start	= DMOV_SDC2_CHAN,
		.end	= DMOV_SDC2_CHAN,
		.flags	= IORESOURCE_DMA,
	},
	{
		.name	= "sdcc_dma_crci",
		.start	= DMOV_SDC2_CRCI,
		.end	= DMOV_SDC2_CRCI,
		.flags	= IORESOURCE_DMA,
	}
=======
		.end	= INT_SDC2_0,
=======
		.end	= INT_SDC2_1,
>>>>>>> refs/remotes/origin/cm-11.0
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name	= "sdcc_dma_chnl",
		.start	= DMOV_SDC2_CHAN,
		.end	= DMOV_SDC2_CHAN,
		.flags	= IORESOURCE_DMA,
	},
	{
		.name	= "sdcc_dma_crci",
		.start	= DMOV_SDC2_CRCI,
		.end	= DMOV_SDC2_CRCI,
		.flags	= IORESOURCE_DMA,
<<<<<<< HEAD
	},
>>>>>>> refs/remotes/origin/master
=======
	}
>>>>>>> refs/remotes/origin/cm-11.0
};

static struct resource resources_sdc3[] = {
	{
<<<<<<< HEAD
<<<<<<< HEAD
		.start	= MSM_SDC3_BASE,
		.end	= MSM_SDC3_BASE + SZ_4K - 1,
=======
		.start	= MSM_SDC3_PHYS,
		.end	= MSM_SDC3_PHYS + MSM_SDC3_SIZE - 1,
>>>>>>> refs/remotes/origin/master
=======
		.start	= MSM_SDC3_BASE,
		.end	= MSM_SDC3_BASE + SZ_4K - 1,
>>>>>>> refs/remotes/origin/cm-11.0
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_SDC3_0,
<<<<<<< HEAD
<<<<<<< HEAD
		.end	= INT_SDC3_1,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name	= "sdcc_dma_chnl",
		.start	= DMOV_SDC3_CHAN,
		.end	= DMOV_SDC3_CHAN,
		.flags	= IORESOURCE_DMA,
	},
	{
		.name	= "sdcc_dma_crci",
		.start	= DMOV_SDC3_CRCI,
		.end	= DMOV_SDC3_CRCI,
=======
		.end	= INT_SDC3_0,
=======
		.end	= INT_SDC3_1,
>>>>>>> refs/remotes/origin/cm-11.0
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name	= "sdcc_dma_chnl",
		.start	= DMOV_SDC3_CHAN,
		.end	= DMOV_SDC3_CHAN,
		.flags	= IORESOURCE_DMA,
	},
	{
<<<<<<< HEAD
		.start	= 8,
		.end	= 8,
>>>>>>> refs/remotes/origin/master
=======
		.name	= "sdcc_dma_crci",
		.start	= DMOV_SDC3_CRCI,
		.end	= DMOV_SDC3_CRCI,
>>>>>>> refs/remotes/origin/cm-11.0
		.flags	= IORESOURCE_DMA,
	},
};

static struct resource resources_sdc4[] = {
	{
<<<<<<< HEAD
<<<<<<< HEAD
		.start	= MSM_SDC4_BASE,
		.end	= MSM_SDC4_BASE + SZ_4K - 1,
=======
		.start	= MSM_SDC4_PHYS,
		.end	= MSM_SDC4_PHYS + MSM_SDC4_SIZE - 1,
>>>>>>> refs/remotes/origin/master
=======
		.start	= MSM_SDC4_BASE,
		.end	= MSM_SDC4_BASE + SZ_4K - 1,
>>>>>>> refs/remotes/origin/cm-11.0
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_SDC4_0,
<<<<<<< HEAD
<<<<<<< HEAD
		.end	= INT_SDC4_1,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name	= "sdcc_dma_chnl",
		.start	= DMOV_SDC4_CHAN,
		.end	= DMOV_SDC4_CHAN,
		.flags	= IORESOURCE_DMA,
	},
	{
		.name	= "sdcc_dma_crci",
		.start	= DMOV_SDC4_CRCI,
		.end	= DMOV_SDC4_CRCI,
=======
		.end	= INT_SDC4_0,
=======
		.end	= INT_SDC4_1,
>>>>>>> refs/remotes/origin/cm-11.0
		.flags	= IORESOURCE_IRQ,
	},
	{
		.name	= "sdcc_dma_chnl",
		.start	= DMOV_SDC4_CHAN,
		.end	= DMOV_SDC4_CHAN,
		.flags	= IORESOURCE_DMA,
	},
	{
<<<<<<< HEAD
		.start	= 8,
		.end	= 8,
>>>>>>> refs/remotes/origin/master
=======
		.name	= "sdcc_dma_crci",
		.start	= DMOV_SDC4_CRCI,
		.end	= DMOV_SDC4_CRCI,
>>>>>>> refs/remotes/origin/cm-11.0
		.flags	= IORESOURCE_DMA,
	},
};

struct platform_device msm_device_sdc1 = {
	.name		= "msm_sdcc",
	.id		= 1,
	.num_resources	= ARRAY_SIZE(resources_sdc1),
	.resource	= resources_sdc1,
	.dev		= {
		.coherent_dma_mask	= 0xffffffff,
	},
};

struct platform_device msm_device_sdc2 = {
	.name		= "msm_sdcc",
	.id		= 2,
	.num_resources	= ARRAY_SIZE(resources_sdc2),
	.resource	= resources_sdc2,
	.dev		= {
		.coherent_dma_mask	= 0xffffffff,
	},
};

struct platform_device msm_device_sdc3 = {
	.name		= "msm_sdcc",
	.id		= 3,
	.num_resources	= ARRAY_SIZE(resources_sdc3),
	.resource	= resources_sdc3,
	.dev		= {
		.coherent_dma_mask	= 0xffffffff,
	},
};

struct platform_device msm_device_sdc4 = {
	.name		= "msm_sdcc",
	.id		= 4,
	.num_resources	= ARRAY_SIZE(resources_sdc4),
	.resource	= resources_sdc4,
	.dev		= {
		.coherent_dma_mask	= 0xffffffff,
	},
};

static struct platform_device *msm_sdcc_devices[] __initdata = {
	&msm_device_sdc1,
	&msm_device_sdc2,
	&msm_device_sdc3,
	&msm_device_sdc4,
};

<<<<<<< HEAD
<<<<<<< HEAD
int __init msm_add_sdcc(unsigned int controller, struct mmc_platform_data *plat)
{
	struct platform_device	*pdev;
=======
int __init msm_add_sdcc(unsigned int controller,
			struct msm_mmc_platform_data *plat,
			unsigned int stat_irq, unsigned long stat_irq_flags)
{
	struct platform_device	*pdev;
	struct resource *res;
>>>>>>> refs/remotes/origin/master
=======
int __init msm_add_sdcc(unsigned int controller, struct mmc_platform_data *plat)
{
	struct platform_device	*pdev;
>>>>>>> refs/remotes/origin/cm-11.0

	if (controller < 1 || controller > 4)
		return -EINVAL;

	pdev = msm_sdcc_devices[controller-1];
	pdev->dev.platform_data = plat;
<<<<<<< HEAD
<<<<<<< HEAD
	return platform_device_register(pdev);
}

#if defined(CONFIG_FB_MSM_MDP40)
#define MDP_BASE          0xA3F00000
#define PMDH_BASE         0xAD600000
#define EMDH_BASE         0xAD700000
#define TVENC_BASE        0xAD400000
#else
#define MDP_BASE          0xAA200000
#define PMDH_BASE         0xAA600000
#define EMDH_BASE         0xAA700000
#define TVENC_BASE        0xAA400000
#endif

static struct resource msm_mdp_resources[] = {
	{
		.name   = "mdp",
		.start  = MDP_BASE,
		.end    = MDP_BASE + 0x000F0000 - 1,
		.flags  = IORESOURCE_MEM,
	},
	{
		.start  = INT_MDP,
		.end    = INT_MDP,
		.flags  = IORESOURCE_IRQ,
	},
};

static struct resource msm_mddi_resources[] = {
	{
		.name   = "pmdh",
		.start  = PMDH_BASE,
		.end    = PMDH_BASE + PAGE_SIZE - 1,
		.flags  = IORESOURCE_MEM,
	}
};

static struct resource msm_mddi_ext_resources[] = {
	{
		.name   = "emdh",
		.start  = EMDH_BASE,
		.end    = EMDH_BASE + PAGE_SIZE - 1,
		.flags  = IORESOURCE_MEM,
	}
};

static struct resource msm_ebi2_lcd_resources[] = {
	{
		.name   = "base",
		.start  = 0xa0d00000,
		.end    = 0xa0d00000 + PAGE_SIZE - 1,
		.flags  = IORESOURCE_MEM,
	},
	{
		.name   = "lcd01",
		.start  = 0x98000000,
		.end    = 0x98000000 + 0x80000 - 1,
		.flags  = IORESOURCE_MEM,
	},
	{
		.name   = "lcd02",
		.start  = 0x9c000000,
		.end    = 0x9c000000 + 0x80000 - 1,
		.flags  = IORESOURCE_MEM,
	},
};

static struct resource msm_tvenc_resources[] = {
	{
		.name   = "tvenc",
		.start  = TVENC_BASE,
		.end    = TVENC_BASE + PAGE_SIZE - 1,
		.flags  = IORESOURCE_MEM,
	}
};

static struct platform_device msm_mdp_device = {
	.name   = "mdp",
	.id     = 0,
	.num_resources  = ARRAY_SIZE(msm_mdp_resources),
	.resource       = msm_mdp_resources,
};

static struct platform_device msm_mddi_device = {
	.name   = "mddi",
	.id     = 0,
	.num_resources  = ARRAY_SIZE(msm_mddi_resources),
	.resource       = msm_mddi_resources,
};

static struct platform_device msm_mddi_ext_device = {
	.name   = "mddi_ext",
	.id     = 0,
	.num_resources  = ARRAY_SIZE(msm_mddi_ext_resources),
	.resource       = msm_mddi_ext_resources,
};

static struct platform_device msm_ebi2_lcd_device = {
	.name   = "ebi2_lcd",
	.id     = 0,
	.num_resources  = ARRAY_SIZE(msm_ebi2_lcd_resources),
	.resource       = msm_ebi2_lcd_resources,
};

static struct platform_device msm_lcdc_device = {
	.name   = "lcdc",
	.id     = 0,
};

static struct platform_device msm_tvenc_device = {
	.name   = "tvenc",
	.id     = 0,
	.num_resources  = ARRAY_SIZE(msm_tvenc_resources),
	.resource       = msm_tvenc_resources,
};

#if defined(CONFIG_MSM_SOC_REV_A)
#define MSM_QUP_PHYS           0xA1680000
#define MSM_GSBI_QUP_I2C_PHYS  0xA1600000
#define INT_PWB_QUP_ERR        INT_GSBI_QUP
#else
#define MSM_QUP_PHYS           0xA9900000
#define MSM_GSBI_QUP_I2C_PHYS  0xA9900000
#define INT_PWB_QUP_ERR        INT_PWB_I2C
#endif
#define MSM_QUP_SIZE           SZ_4K
static struct resource resources_qup[] = {
	{
		.name   = "qup_phys_addr",
		.start	= MSM_QUP_PHYS,
		.end	= MSM_QUP_PHYS + MSM_QUP_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name   = "gsbi_qup_i2c_addr",
		.start	= MSM_GSBI_QUP_I2C_PHYS,
		.end	= MSM_GSBI_QUP_I2C_PHYS + 4 - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name   = "qup_err_intr",
		.start	= INT_PWB_QUP_ERR,
		.end	= INT_PWB_QUP_ERR,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device qup_device_i2c = {
	.name		= "qup_i2c",
	.id		= 4,
	.num_resources	= ARRAY_SIZE(resources_qup),
	.resource	= resources_qup,
};

/* TSIF begin */
#if defined(CONFIG_TSIF) || defined(CONFIG_TSIF_MODULE)

#define MSM_TSIF_PHYS        (0xa0100000)
#define MSM_TSIF_SIZE        (0x200)

static struct resource tsif_resources[] = {
	[0] = {
		.flags = IORESOURCE_IRQ,
		.start = INT_TSIF_IRQ,
		.end   = INT_TSIF_IRQ,
	},
	[1] = {
		.flags = IORESOURCE_MEM,
		.start = MSM_TSIF_PHYS,
		.end   = MSM_TSIF_PHYS + MSM_TSIF_SIZE - 1,
	},
	[2] = {
		.flags = IORESOURCE_DMA,
		.start = DMOV_TSIF_CHAN,
		.end   = DMOV_TSIF_CRCI,
	},
};

static void tsif_release(struct device *dev)
{
	dev_info(dev, "release\n");
}

struct platform_device msm_device_tsif = {
	.name          = "msm_tsif",
	.id            = 0,
	.num_resources = ARRAY_SIZE(tsif_resources),
	.resource      = tsif_resources,
	.dev = {
		.release       = tsif_release,
	},
};
#endif /* defined(CONFIG_TSIF) || defined(CONFIG_TSIF_MODULE) */
/* TSIF end   */

#define MSM_TSSC_PHYS         0xAA300000
static struct resource resources_tssc[] = {
	{
		.start	= MSM_TSSC_PHYS,
		.end	= MSM_TSSC_PHYS + SZ_4K - 1,
		.name	= "tssc",
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_TCHSCRN1,
		.end	= INT_TCHSCRN1,
		.name	= "tssc1",
		.flags	= IORESOURCE_IRQ | IRQF_TRIGGER_RISING,
	},
	{
		.start	= INT_TCHSCRN2,
		.end	= INT_TCHSCRN2,
		.name	= "tssc2",
		.flags	= IORESOURCE_IRQ | IRQF_TRIGGER_RISING,
	},
};

struct platform_device msm_device_tssc = {
	.name = "msm_touchscreen",
	.id = 0,
	.num_resources = ARRAY_SIZE(resources_tssc),
	.resource = resources_tssc,
};

static void __init msm_register_device(struct platform_device *pdev, void *data)
{
	int ret;

	pdev->dev.platform_data = data;

	ret = platform_device_register(pdev);
	if (ret)
		dev_err(&pdev->dev,
			  "%s: platform_device_register() failed = %d\n",
			  __func__, ret);
}

void __init msm_fb_register_device(char *name, void *data)
{
	if (!strncmp(name, "mdp", 3))
		msm_register_device(&msm_mdp_device, data);
	else if (!strncmp(name, "pmdh", 4))
		msm_register_device(&msm_mddi_device, data);
	else if (!strncmp(name, "emdh", 4))
		msm_register_device(&msm_mddi_ext_device, data);
	else if (!strncmp(name, "ebi2", 4))
		msm_register_device(&msm_ebi2_lcd_device, data);
	else if (!strncmp(name, "tvenc", 5))
		msm_register_device(&msm_tvenc_device, data);
	else if (!strncmp(name, "lcdc", 4))
		msm_register_device(&msm_lcdc_device, data);
	else
		printk(KERN_ERR "%s: unknown device! %s\n", __func__, name);
}

static struct platform_device msm_camera_device = {
	.name	= "msm_camera",
	.id	= 0,
};

void __init msm_camera_register_device(void *res, uint32_t num,
	void *data)
{
	msm_camera_device.num_resources = num;
	msm_camera_device.resource = res;

	msm_register_device(&msm_camera_device, data);
}

static struct resource kgsl_3d0_resources[] = {
	{
		.name  = KGSL_3D0_REG_MEMORY,
		.start = 0xA0000000,
		.end = 0xA001ffff,
		.flags = IORESOURCE_MEM,
	},
	{
		.name = KGSL_3D0_IRQ,
		.start = INT_GRAPHICS,
		.end = INT_GRAPHICS,
		.flags = IORESOURCE_IRQ,
	},
};

static struct kgsl_device_platform_data kgsl_3d0_pdata = {
	.pwrlevel = {
		{
			.gpu_freq = 0,
			.bus_freq = 128000000,
		},
	},
	.init_level = 0,
	.num_levels = 1,
	.set_grp_async = NULL,
	.idle_timeout = HZ/5,
	.clk_map = KGSL_CLK_CORE | KGSL_CLK_MEM,
};

struct platform_device msm_kgsl_3d0 = {
	.name = "kgsl-3d0",
	.id = 0,
	.num_resources = ARRAY_SIZE(kgsl_3d0_resources),
	.resource = kgsl_3d0_resources,
	.dev = {
		.platform_data = &kgsl_3d0_pdata,
	},
};

=======
=======
	return platform_device_register(pdev);
}
>>>>>>> refs/remotes/origin/cm-11.0

#if defined(CONFIG_FB_MSM_MDP40)
#define MDP_BASE          0xA3F00000
#define PMDH_BASE         0xAD600000
#define EMDH_BASE         0xAD700000
#define TVENC_BASE        0xAD400000
#else
#define MDP_BASE          0xAA200000
#define PMDH_BASE         0xAA600000
#define EMDH_BASE         0xAA700000
#define TVENC_BASE        0xAA400000
#endif

static struct resource msm_mdp_resources[] = {
	{
		.name   = "mdp",
		.start  = MDP_BASE,
		.end    = MDP_BASE + 0x000F0000 - 1,
		.flags  = IORESOURCE_MEM,
	},
	{
		.start  = INT_MDP,
		.end    = INT_MDP,
		.flags  = IORESOURCE_IRQ,
	},
};

static struct resource msm_mddi_resources[] = {
	{
		.name   = "pmdh",
		.start  = PMDH_BASE,
		.end    = PMDH_BASE + PAGE_SIZE - 1,
		.flags  = IORESOURCE_MEM,
	}
};

static struct resource msm_mddi_ext_resources[] = {
	{
		.name   = "emdh",
		.start  = EMDH_BASE,
		.end    = EMDH_BASE + PAGE_SIZE - 1,
		.flags  = IORESOURCE_MEM,
	}
};

static struct resource msm_ebi2_lcd_resources[] = {
	{
		.name   = "base",
		.start  = 0xa0d00000,
		.end    = 0xa0d00000 + PAGE_SIZE - 1,
		.flags  = IORESOURCE_MEM,
	},
	{
		.name   = "lcd01",
		.start  = 0x98000000,
		.end    = 0x98000000 + 0x80000 - 1,
		.flags  = IORESOURCE_MEM,
	},
	{
		.name   = "lcd02",
		.start  = 0x9c000000,
		.end    = 0x9c000000 + 0x80000 - 1,
		.flags  = IORESOURCE_MEM,
	},
};

static struct resource msm_tvenc_resources[] = {
	{
		.name   = "tvenc",
		.start  = TVENC_BASE,
		.end    = TVENC_BASE + PAGE_SIZE - 1,
		.flags  = IORESOURCE_MEM,
	}
};

static struct platform_device msm_mdp_device = {
	.name   = "mdp",
	.id     = 0,
	.num_resources  = ARRAY_SIZE(msm_mdp_resources),
	.resource       = msm_mdp_resources,
};

static struct platform_device msm_mddi_device = {
	.name   = "mddi",
	.id     = 0,
	.num_resources  = ARRAY_SIZE(msm_mddi_resources),
	.resource       = msm_mddi_resources,
};

static struct platform_device msm_mddi_ext_device = {
	.name   = "mddi_ext",
	.id     = 0,
	.num_resources  = ARRAY_SIZE(msm_mddi_ext_resources),
	.resource       = msm_mddi_ext_resources,
};

static struct platform_device msm_ebi2_lcd_device = {
	.name   = "ebi2_lcd",
	.id     = 0,
	.num_resources  = ARRAY_SIZE(msm_ebi2_lcd_resources),
	.resource       = msm_ebi2_lcd_resources,
};

static struct platform_device msm_lcdc_device = {
	.name   = "lcdc",
	.id     = 0,
};

static struct platform_device msm_tvenc_device = {
	.name   = "tvenc",
	.id     = 0,
	.num_resources  = ARRAY_SIZE(msm_tvenc_resources),
	.resource       = msm_tvenc_resources,
};

#if defined(CONFIG_MSM_SOC_REV_A)
#define MSM_QUP_PHYS           0xA1680000
#define MSM_GSBI_QUP_I2C_PHYS  0xA1600000
#define INT_PWB_QUP_ERR        INT_GSBI_QUP
#else
#define MSM_QUP_PHYS           0xA9900000
#define MSM_GSBI_QUP_I2C_PHYS  0xA9900000
#define INT_PWB_QUP_ERR        INT_PWB_I2C
#endif
#define MSM_QUP_SIZE           SZ_4K
static struct resource resources_qup[] = {
	{
		.name   = "qup_phys_addr",
		.start	= MSM_QUP_PHYS,
		.end	= MSM_QUP_PHYS + MSM_QUP_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name   = "gsbi_qup_i2c_addr",
		.start	= MSM_GSBI_QUP_I2C_PHYS,
		.end	= MSM_GSBI_QUP_I2C_PHYS + 4 - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.name   = "qup_err_intr",
		.start	= INT_PWB_QUP_ERR,
		.end	= INT_PWB_QUP_ERR,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device qup_device_i2c = {
	.name		= "qup_i2c",
	.id		= 4,
	.num_resources	= ARRAY_SIZE(resources_qup),
	.resource	= resources_qup,
};

/* TSIF begin */
#if defined(CONFIG_TSIF) || defined(CONFIG_TSIF_MODULE)

#define MSM_TSIF_PHYS        (0xa0100000)
#define MSM_TSIF_SIZE        (0x200)

static struct resource tsif_resources[] = {
	[0] = {
		.flags = IORESOURCE_IRQ,
		.start = INT_TSIF_IRQ,
		.end   = INT_TSIF_IRQ,
	},
	[1] = {
		.flags = IORESOURCE_MEM,
		.start = MSM_TSIF_PHYS,
		.end   = MSM_TSIF_PHYS + MSM_TSIF_SIZE - 1,
	},
	[2] = {
		.flags = IORESOURCE_DMA,
		.start = DMOV_TSIF_CHAN,
		.end   = DMOV_TSIF_CRCI,
	},
};

static void tsif_release(struct device *dev)
{
	dev_info(dev, "release\n");
}

<<<<<<< HEAD
static struct clk_pcom_desc msm_clocks_8x50[] = {
	CLK_PCOM("adm_clk",	ADM_CLK,	NULL, 0),
	CLK_PCOM("ce_clk",	CE_CLK,		NULL, 0),
	CLK_PCOM("ebi1_clk",	EBI1_CLK,	NULL, CLK_MIN),
	CLK_PCOM("ebi2_clk",	EBI2_CLK,	NULL, 0),
	CLK_PCOM("ecodec_clk",	ECODEC_CLK,	NULL, 0),
	CLK_PCOM("emdh_clk",	EMDH_CLK,	NULL, OFF | CLK_MINMAX),
	CLK_PCOM("gp_clk",	GP_CLK,		NULL, 0),
	CLK_PCOM("grp_clk",	GRP_3D_CLK,	NULL, 0),
	CLK_PCOM("i2c_clk",	I2C_CLK,	NULL, 0),
	CLK_PCOM("icodec_rx_clk",	ICODEC_RX_CLK,	NULL, 0),
	CLK_PCOM("icodec_tx_clk",	ICODEC_TX_CLK,	NULL, 0),
	CLK_PCOM("imem_clk",	IMEM_CLK,	NULL, OFF),
	CLK_PCOM("mdc_clk",	MDC_CLK,	NULL, 0),
	CLK_PCOM("mddi_clk",	PMDH_CLK,	NULL, OFF | CLK_MINMAX),
	CLK_PCOM("mdp_clk",	MDP_CLK,	NULL, OFF),
	CLK_PCOM("mdp_lcdc_pclk_clk", MDP_LCDC_PCLK_CLK, NULL, 0),
	CLK_PCOM("mdp_lcdc_pad_pclk_clk", MDP_LCDC_PAD_PCLK_CLK, NULL, 0),
	CLK_PCOM("mdp_vsync_clk",	MDP_VSYNC_CLK,	NULL, 0),
	CLK_PCOM("pbus_clk",	PBUS_CLK,	NULL, CLK_MIN),
	CLK_PCOM("pcm_clk",	PCM_CLK,	NULL, 0),
	CLK_PCOM("sdac_clk",	SDAC_CLK,	NULL, OFF),
	CLK_PCOM("sdc_clk",	SDC1_CLK,	"msm_sdcc.1", OFF),
	CLK_PCOM("sdc_pclk",	SDC1_P_CLK,	"msm_sdcc.1", OFF),
	CLK_PCOM("sdc_clk",	SDC2_CLK,	"msm_sdcc.2", OFF),
	CLK_PCOM("sdc_pclk",	SDC2_P_CLK,	"msm_sdcc.2", OFF),
	CLK_PCOM("sdc_clk",	SDC3_CLK,	"msm_sdcc.3", OFF),
	CLK_PCOM("sdc_pclk",	SDC3_P_CLK,	"msm_sdcc.3", OFF),
	CLK_PCOM("sdc_clk",	SDC4_CLK,	"msm_sdcc.4", OFF),
	CLK_PCOM("sdc_pclk",	SDC4_P_CLK,	"msm_sdcc.4", OFF),
	CLK_PCOM("spi_clk",	SPI_CLK,	NULL, 0),
	CLK_PCOM("tsif_clk",	TSIF_CLK,	NULL, 0),
	CLK_PCOM("tsif_ref_clk",	TSIF_REF_CLK,	NULL, 0),
	CLK_PCOM("tv_dac_clk",	TV_DAC_CLK,	NULL, 0),
	CLK_PCOM("tv_enc_clk",	TV_ENC_CLK,	NULL, 0),
	CLK_PCOM("core",	UART1_CLK,	NULL, OFF),
	CLK_PCOM("core",	UART2_CLK,	NULL, 0),
	CLK_PCOM("core",	UART3_CLK,	"msm_serial.2", OFF),
	CLK_PCOM("uartdm_clk",	UART1DM_CLK,	NULL, OFF),
	CLK_PCOM("uartdm_clk",	UART2DM_CLK,	NULL, 0),
	CLK_PCOM("usb_hs_clk",	USB_HS_CLK,	NULL, OFF),
	CLK_PCOM("usb_hs_pclk",	USB_HS_P_CLK,	NULL, OFF),
	CLK_PCOM("usb_otg_clk",	USB_OTG_CLK,	NULL, 0),
	CLK_PCOM("vdc_clk",	VDC_CLK,	NULL, OFF | CLK_MIN),
	CLK_PCOM("vfe_clk",	VFE_CLK,	NULL, OFF),
	CLK_PCOM("vfe_mdc_clk",	VFE_MDC_CLK,	NULL, OFF),
	CLK_PCOM("vfe_axi_clk",	VFE_AXI_CLK,	NULL, OFF),
	CLK_PCOM("usb_hs2_clk",	USB_HS2_CLK,	NULL, OFF),
	CLK_PCOM("usb_hs2_pclk",	USB_HS2_P_CLK,	NULL, OFF),
	CLK_PCOM("usb_hs3_clk",	USB_HS3_CLK,	NULL, OFF),
	CLK_PCOM("usb_hs3_pclk",	USB_HS3_P_CLK,	NULL, OFF),
	CLK_PCOM("usb_phy_clk",	USB_PHY_CLK,	NULL, 0),
};

static struct pcom_clk_pdata msm_clock_8x50_pdata = {
	.lookup = msm_clocks_8x50,
	.num_lookups = ARRAY_SIZE(msm_clocks_8x50),
=======
struct platform_device msm_device_tsif = {
	.name          = "msm_tsif",
	.id            = 0,
	.num_resources = ARRAY_SIZE(tsif_resources),
	.resource      = tsif_resources,
	.dev = {
		.release       = tsif_release,
	},
};
#endif /* defined(CONFIG_TSIF) || defined(CONFIG_TSIF_MODULE) */
/* TSIF end   */

#define MSM_TSSC_PHYS         0xAA300000
static struct resource resources_tssc[] = {
	{
		.start	= MSM_TSSC_PHYS,
		.end	= MSM_TSSC_PHYS + SZ_4K - 1,
		.name	= "tssc",
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_TCHSCRN1,
		.end	= INT_TCHSCRN1,
		.name	= "tssc1",
		.flags	= IORESOURCE_IRQ | IRQF_TRIGGER_RISING,
	},
	{
		.start	= INT_TCHSCRN2,
		.end	= INT_TCHSCRN2,
		.name	= "tssc2",
		.flags	= IORESOURCE_IRQ | IRQF_TRIGGER_RISING,
	},
};

struct platform_device msm_device_tssc = {
	.name = "msm_touchscreen",
	.id = 0,
	.num_resources = ARRAY_SIZE(resources_tssc),
	.resource = resources_tssc,
};

static void __init msm_register_device(struct platform_device *pdev, void *data)
{
	int ret;

	pdev->dev.platform_data = data;

	ret = platform_device_register(pdev);
	if (ret)
		dev_err(&pdev->dev,
			  "%s: platform_device_register() failed = %d\n",
			  __func__, ret);
}

void __init msm_fb_register_device(char *name, void *data)
{
	if (!strncmp(name, "mdp", 3))
		msm_register_device(&msm_mdp_device, data);
	else if (!strncmp(name, "pmdh", 4))
		msm_register_device(&msm_mddi_device, data);
	else if (!strncmp(name, "emdh", 4))
		msm_register_device(&msm_mddi_ext_device, data);
	else if (!strncmp(name, "ebi2", 4))
		msm_register_device(&msm_ebi2_lcd_device, data);
	else if (!strncmp(name, "tvenc", 5))
		msm_register_device(&msm_tvenc_device, data);
	else if (!strncmp(name, "lcdc", 4))
		msm_register_device(&msm_lcdc_device, data);
	else
		printk(KERN_ERR "%s: unknown device! %s\n", __func__, name);
}

static struct platform_device msm_camera_device = {
	.name	= "msm_camera",
	.id	= 0,
};

void __init msm_camera_register_device(void *res, uint32_t num,
	void *data)
{
	msm_camera_device.num_resources = num;
	msm_camera_device.resource = res;

	msm_register_device(&msm_camera_device, data);
}

static struct resource kgsl_3d0_resources[] = {
	{
		.name  = KGSL_3D0_REG_MEMORY,
		.start = 0xA0000000,
		.end = 0xA001ffff,
		.flags = IORESOURCE_MEM,
	},
	{
		.name = KGSL_3D0_IRQ,
		.start = INT_GRAPHICS,
		.end = INT_GRAPHICS,
		.flags = IORESOURCE_IRQ,
	},
};

static struct kgsl_device_platform_data kgsl_3d0_pdata = {
	.pwrlevel = {
		{
			.gpu_freq = 0,
			.bus_freq = 128000000,
		},
	},
	.init_level = 0,
	.num_levels = 1,
	.set_grp_async = NULL,
	.idle_timeout = HZ/5,
	.clk_map = KGSL_CLK_CORE | KGSL_CLK_MEM,
};

struct platform_device msm_kgsl_3d0 = {
	.name = "kgsl-3d0",
	.id = 0,
	.num_resources = ARRAY_SIZE(kgsl_3d0_resources),
	.resource = kgsl_3d0_resources,
	.dev = {
		.platform_data = &kgsl_3d0_pdata,
	},
>>>>>>> refs/remotes/origin/cm-11.0
};

struct platform_device msm_clock_8x50 = {
	.name = "msm-clock-pcom",
	.dev.platform_data = &msm_clock_8x50_pdata,
};
>>>>>>> refs/remotes/origin/master
