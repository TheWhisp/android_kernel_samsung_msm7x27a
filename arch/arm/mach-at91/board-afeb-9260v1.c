/*
 * linux/arch/arm/mach-at91/board-afeb-9260v1.c
 *
 *  Copyright (C) 2005 SAN People
 *  Copyright (C) 2006 Atmel
 *  Copyright (C) 2008 Sergey Lapin
 *
 * A custom board designed as open hardware; PCBs and various information
 * is available at http://groups.google.com/group/arm9fpga-evolution-board/
 * Subversion repository: svn://194.85.238.22/home/users/george/svn/arm9eb
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

#include <linux/types.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/gpio.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/gpio.h>
>>>>>>> refs/remotes/origin/master
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/clk.h>
#include <linux/dma-mapping.h>

#include <mach/hardware.h>
#include <asm/setup.h>
#include <asm/mach-types.h>
#include <asm/irq.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

<<<<<<< HEAD
#include <mach/board.h>
<<<<<<< HEAD
#include <mach/gpio.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0

=======
#include "at91_aic.h"
#include "board.h"
>>>>>>> refs/remotes/origin/master
#include "generic.h"


static void __init afeb9260_init_early(void)
{
	/* Initialize processor: 18.432 MHz crystal */
<<<<<<< HEAD
<<<<<<< HEAD
	at91sam9260_initialize(18432000);
=======
	at91_initialize(18432000);
>>>>>>> refs/remotes/origin/cm-10.0

	/* DBGU on ttyS0. (Rx & Tx only) */
	at91_register_uart(0, 0, 0);

	/* USART0 on ttyS1. (Rx, Tx, CTS, RTS, DTR, DSR, DCD, RI) */
	at91_register_uart(AT91SAM9260_ID_US0, 1,
			     ATMEL_UART_CTS | ATMEL_UART_RTS
			   | ATMEL_UART_DTR | ATMEL_UART_DSR
			   | ATMEL_UART_DCD | ATMEL_UART_RI);

	/* USART1 on ttyS2. (Rx, Tx, RTS, CTS) */
	at91_register_uart(AT91SAM9260_ID_US1, 2,
			ATMEL_UART_CTS | ATMEL_UART_RTS);

	/* set serial console to ttyS0 (ie, DBGU) */
	at91_set_serial_console(0);
}

<<<<<<< HEAD
static void __init afeb9260_init_irq(void)
{
	at91sam9260_init_interrupts(NULL);
}


=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	at91_initialize(18432000);
}

>>>>>>> refs/remotes/origin/master
/*
 * USB Host port
 */
static struct at91_usbh_data __initdata afeb9260_usbh_data = {
	.ports		= 1,
<<<<<<< HEAD
<<<<<<< HEAD
=======
	.vbus_pin	= {-EINVAL, -EINVAL},
	.overcurrent_pin= {-EINVAL, -EINVAL},
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.vbus_pin	= {-EINVAL, -EINVAL},
	.overcurrent_pin= {-EINVAL, -EINVAL},
>>>>>>> refs/remotes/origin/master
};

/*
 * USB Device port
 */
static struct at91_udc_data __initdata afeb9260_udc_data = {
	.vbus_pin	= AT91_PIN_PC5,
<<<<<<< HEAD
<<<<<<< HEAD
	.pullup_pin	= 0,		/* pull-up driven by UDC */
=======
	.pullup_pin	= -EINVAL,		/* pull-up driven by UDC */
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.pullup_pin	= -EINVAL,		/* pull-up driven by UDC */
>>>>>>> refs/remotes/origin/master
};



/*
 * SPI devices.
 */
static struct spi_board_info afeb9260_spi_devices[] = {
	{	/* DataFlash chip */
		.modalias	= "mtd_dataflash",
		.chip_select	= 1,
		.max_speed_hz	= 15 * 1000 * 1000,
		.bus_num	= 0,
	},
};


/*
 * MACB Ethernet device
 */
<<<<<<< HEAD
<<<<<<< HEAD
static struct at91_eth_data __initdata afeb9260_macb_data = {
=======
static struct macb_platform_data __initdata afeb9260_macb_data = {
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct macb_platform_data __initdata afeb9260_macb_data = {
>>>>>>> refs/remotes/origin/master
	.phy_irq_pin	= AT91_PIN_PA9,
	.is_rmii	= 0,
};


/*
 * NAND flash
 */
static struct mtd_partition __initdata afeb9260_nand_partition[] = {
	{
		.name	= "bootloader",
		.offset	= 0,
		.size	= (640 * SZ_1K),
	},
	{
		.name	= "kernel",
		.offset	= MTDPART_OFS_NXTBLK,
		.size	= SZ_2M,
	},
	{
		.name	= "rootfs",
		.offset	= MTDPART_OFS_NXTBLK,
		.size	= MTDPART_SIZ_FULL,
	},
};

<<<<<<< HEAD
<<<<<<< HEAD
static struct mtd_partition * __init nand_partitions(int size, int *num_partitions)
{
	*num_partitions = ARRAY_SIZE(afeb9260_nand_partition);
	return afeb9260_nand_partition;
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static struct atmel_nand_data __initdata afeb9260_nand_data = {
	.ale		= 21,
	.cle		= 22,
	.rdy_pin	= AT91_PIN_PC13,
	.enable_pin	= AT91_PIN_PC14,
<<<<<<< HEAD
<<<<<<< HEAD
	.partition_info	= nand_partitions,
	.bus_width_16	= 0,
=======
=======
>>>>>>> refs/remotes/origin/master
	.bus_width_16	= 0,
	.ecc_mode	= NAND_ECC_SOFT,
	.parts		= afeb9260_nand_partition,
	.num_parts	= ARRAY_SIZE(afeb9260_nand_partition),
	.det_pin	= -EINVAL,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};


/*
 * MCI (SD/MMC)
 */
<<<<<<< HEAD
static struct at91_mmc_data __initdata afeb9260_mmc_data = {
	.det_pin 	= AT91_PIN_PC9,
	.wp_pin 	= AT91_PIN_PC4,
	.slot_b		= 1,
	.wire4		= 1,
<<<<<<< HEAD
=======
	.vcc_pin	= -EINVAL,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct mci_platform_data __initdata afeb9260_mci0_data = {
	.slot[1] = {
		.bus_width	= 4,
		.detect_pin	= AT91_PIN_PC9,
		.wp_pin		= AT91_PIN_PC4,
	},
>>>>>>> refs/remotes/origin/master
};



static struct i2c_board_info __initdata afeb9260_i2c_devices[] = {
	{
		I2C_BOARD_INFO("tlv320aic23", 0x1a),
	}, {
		I2C_BOARD_INFO("fm3130", 0x68),
	}, {
		I2C_BOARD_INFO("24c64", 0x50),
	},
};

/*
 * IDE (CF True IDE mode)
 */
static struct at91_cf_data afeb9260_cf_data = {
	.chipselect = 4,
	.irq_pin    = AT91_PIN_PA6,
<<<<<<< HEAD
<<<<<<< HEAD
=======
	.det_pin	= -EINVAL,
	.vcc_pin	= -EINVAL,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.det_pin	= -EINVAL,
	.vcc_pin	= -EINVAL,
>>>>>>> refs/remotes/origin/master
	.rst_pin    = AT91_PIN_PA7,
	.flags      = AT91_CF_TRUE_IDE,
};

static void __init afeb9260_board_init(void)
{
	/* Serial */
<<<<<<< HEAD
=======
	/* DBGU on ttyS0. (Rx & Tx only) */
	at91_register_uart(0, 0, 0);

	/* USART0 on ttyS1. (Rx, Tx, CTS, RTS, DTR, DSR, DCD, RI) */
	at91_register_uart(AT91SAM9260_ID_US0, 1,
			     ATMEL_UART_CTS | ATMEL_UART_RTS
			   | ATMEL_UART_DTR | ATMEL_UART_DSR
			   | ATMEL_UART_DCD | ATMEL_UART_RI);

	/* USART1 on ttyS2. (Rx, Tx, RTS, CTS) */
	at91_register_uart(AT91SAM9260_ID_US1, 2,
			ATMEL_UART_CTS | ATMEL_UART_RTS);
>>>>>>> refs/remotes/origin/master
	at91_add_device_serial();
	/* USB Host */
	at91_add_device_usbh(&afeb9260_usbh_data);
	/* USB Device */
	at91_add_device_udc(&afeb9260_udc_data);
	/* SPI */
	at91_add_device_spi(afeb9260_spi_devices,
			ARRAY_SIZE(afeb9260_spi_devices));
	/* NAND */
	at91_add_device_nand(&afeb9260_nand_data);
	/* Ethernet */
	at91_add_device_eth(&afeb9260_macb_data);

	/* Standard function's pin assignments are not
	 * appropriate for us and generic code provide
	 * no API to configure these pins any other way */
	at91_set_B_periph(AT91_PIN_PA10, 0);	/* ETX2 */
	at91_set_B_periph(AT91_PIN_PA11, 0);	/* ETX3 */
	/* MMC */
<<<<<<< HEAD
	at91_add_device_mmc(0, &afeb9260_mmc_data);
=======
	at91_add_device_mci(0, &afeb9260_mci0_data);
>>>>>>> refs/remotes/origin/master
	/* I2C */
	at91_add_device_i2c(afeb9260_i2c_devices,
			ARRAY_SIZE(afeb9260_i2c_devices));
	/* Audio */
	at91_add_device_ssc(AT91SAM9260_ID_SSC, ATMEL_SSC_TX);
	/* IDE */
	at91_add_device_cf(&afeb9260_cf_data);
}

MACHINE_START(AFEB9260, "Custom afeb9260 board")
	/* Maintainer: Sergey Lapin <slapin@ossfans.org> */
<<<<<<< HEAD
	.timer		= &at91sam926x_timer,
<<<<<<< HEAD
	.map_io		= at91sam9260_map_io,
	.init_early	= afeb9260_init_early,
	.init_irq	= afeb9260_init_irq,
=======
	.map_io		= at91_map_io,
	.init_early	= afeb9260_init_early,
	.init_irq	= at91_init_irq_default,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.init_time	= at91sam926x_pit_init,
	.map_io		= at91_map_io,
	.handle_irq	= at91_aic_handle_irq,
	.init_early	= afeb9260_init_early,
	.init_irq	= at91_init_irq_default,
>>>>>>> refs/remotes/origin/master
	.init_machine	= afeb9260_board_init,
MACHINE_END

