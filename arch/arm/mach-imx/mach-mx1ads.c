/*
 * arch/arm/mach-imx/mach-mx1ads.c
 *
 * Initially based on:
 *	linux-2.6.7-imx/arch/arm/mach-imx/scb9328.c
 *	Copyright (c) 2004 Sascha Hauer <sascha@saschahauer.de>
 *
 * 2004 (c) MontaVista Software, Inc.
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/i2c.h>
#include <linux/i2c/pcf857x.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/mtd/physmap.h>

#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>

<<<<<<< HEAD
#include <mach/common.h>
#include <mach/hardware.h>
#include <mach/iomux-mx1.h>
#include <mach/irqs.h>

#include "devices-imx1.h"
=======
#include "common.h"
#include "devices-imx1.h"
#include "hardware.h"
#include "iomux-mx1.h"
>>>>>>> refs/remotes/origin/master

static const int mx1ads_pins[] __initconst = {
	/* UART1 */
	PC9_PF_UART1_CTS,
	PC10_PF_UART1_RTS,
	PC11_PF_UART1_TXD,
	PC12_PF_UART1_RXD,
	/* UART2 */
	PB28_PF_UART2_CTS,
	PB29_PF_UART2_RTS,
	PB30_PF_UART2_TXD,
	PB31_PF_UART2_RXD,
	/* I2C */
	PA15_PF_I2C_SDA,
	PA16_PF_I2C_SCL,
	/* SPI */
	PC13_PF_SPI1_SPI_RDY,
	PC14_PF_SPI1_SCLK,
	PC15_PF_SPI1_SS,
	PC16_PF_SPI1_MISO,
	PC17_PF_SPI1_MOSI,
};

/*
 * UARTs platform data
 */

static const struct imxuart_platform_data uart0_pdata __initconst = {
	.flags = IMXUART_HAVE_RTSCTS,
};
       
static const struct imxuart_platform_data uart1_pdata __initconst = {
	.flags = IMXUART_HAVE_RTSCTS,
};

/*
 * Physmap flash
 */

<<<<<<< HEAD
<<<<<<< HEAD
static struct physmap_flash_data mx1ads_flash_data = {
	.width		= 4,		/* bankwidth in bytes */
};

static struct resource flash_resource = {
=======
=======
>>>>>>> refs/remotes/origin/master
static const struct physmap_flash_data mx1ads_flash_data __initconst = {
	.width		= 4,		/* bankwidth in bytes */
};

static const struct resource flash_resource __initconst = {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	.start	= MX1_CS0_PHYS,
	.end	= MX1_CS0_PHYS + SZ_32M - 1,
	.flags	= IORESOURCE_MEM,
};

<<<<<<< HEAD
<<<<<<< HEAD
static struct platform_device flash_device = {
	.name	= "physmap-flash",
	.id	= 0,
	.resource = &flash_resource,
	.num_resources = 1,
};

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/*
 * I2C
 */
static struct pcf857x_platform_data pcf857x_data[] = {
	{
		.gpio_base = 4 * 32,
	}, {
		.gpio_base = 4 * 32 + 16,
	}
};

static const struct imxi2c_platform_data mx1ads_i2c_data __initconst = {
	.bitrate = 100000,
};

static struct i2c_board_info mx1ads_i2c_devices[] = {
	{
		I2C_BOARD_INFO("pcf8575", 0x22),
		.platform_data = &pcf857x_data[0],
	}, {
		I2C_BOARD_INFO("pcf8575", 0x24),
		.platform_data = &pcf857x_data[1],
	},
};

/*
 * Board init
 */
static void __init mx1ads_init(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	imx1_soc_init();

>>>>>>> refs/remotes/origin/cm-10.0
=======
	imx1_soc_init();

>>>>>>> refs/remotes/origin/master
	mxc_gpio_setup_multiple_pins(mx1ads_pins,
		ARRAY_SIZE(mx1ads_pins), "mx1ads");

	/* UART */
	imx1_add_imx_uart0(&uart0_pdata);
	imx1_add_imx_uart1(&uart1_pdata);

	/* Physmap flash */
<<<<<<< HEAD
<<<<<<< HEAD
	mxc_register_device(&flash_device, &mx1ads_flash_data);
=======
	platform_device_register_resndata(NULL, "physmap-flash", 0,
			&flash_resource, 1,
			&mx1ads_flash_data, sizeof(mx1ads_flash_data));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	platform_device_register_resndata(NULL, "physmap-flash", 0,
			&flash_resource, 1,
			&mx1ads_flash_data, sizeof(mx1ads_flash_data));
>>>>>>> refs/remotes/origin/master

	/* I2C */
	i2c_register_board_info(0, mx1ads_i2c_devices,
				ARRAY_SIZE(mx1ads_i2c_devices));

	imx1_add_imx_i2c(&mx1ads_i2c_data);
}

static void __init mx1ads_timer_init(void)
{
	mx1_clocks_init(32000);
}

<<<<<<< HEAD
struct sys_timer mx1ads_timer = {
	.init	= mx1ads_timer_init,
};

MACHINE_START(MX1ADS, "Freescale MX1ADS")
	/* Maintainer: Sascha Hauer, Pengutronix */
<<<<<<< HEAD
	.boot_params = MX1_PHYS_OFFSET + 0x100,
	.map_io = mx1_map_io,
	.init_early = imx1_init_early,
	.init_irq = mx1_init_irq,
	.timer = &mx1ads_timer,
	.init_machine = mx1ads_init,
MACHINE_END

MACHINE_START(MXLADS, "Freescale MXLADS")
	.boot_params = MX1_PHYS_OFFSET + 0x100,
	.map_io = mx1_map_io,
	.init_early = imx1_init_early,
	.init_irq = mx1_init_irq,
	.timer = &mx1ads_timer,
	.init_machine = mx1ads_init,
=======
=======
MACHINE_START(MX1ADS, "Freescale MX1ADS")
	/* Maintainer: Sascha Hauer, Pengutronix */
>>>>>>> refs/remotes/origin/master
	.atag_offset = 0x100,
	.map_io = mx1_map_io,
	.init_early = imx1_init_early,
	.init_irq = mx1_init_irq,
	.handle_irq = imx1_handle_irq,
<<<<<<< HEAD
	.timer = &mx1ads_timer,
=======
	.init_time	= mx1ads_timer_init,
>>>>>>> refs/remotes/origin/master
	.init_machine = mx1ads_init,
	.restart	= mxc_restart,
MACHINE_END

MACHINE_START(MXLADS, "Freescale MXLADS")
	.atag_offset = 0x100,
	.map_io = mx1_map_io,
	.init_early = imx1_init_early,
	.init_irq = mx1_init_irq,
	.handle_irq = imx1_handle_irq,
<<<<<<< HEAD
	.timer = &mx1ads_timer,
	.init_machine = mx1ads_init,
	.restart	= mxc_restart,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.init_time	= mx1ads_timer_init,
	.init_machine = mx1ads_init,
	.restart	= mxc_restart,
>>>>>>> refs/remotes/origin/master
MACHINE_END
