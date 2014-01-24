/*
 * linux/arch/arm/mach-at91/board-flexibity.c
 *
<<<<<<< HEAD
<<<<<<< HEAD
 *  Copyright (C) 2010 Flexibity
=======
 *  Copyright (C) 2010-2011 Flexibity
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *  Copyright (C) 2010-2011 Flexibity
>>>>>>> refs/remotes/origin/master
 *  Copyright (C) 2005 SAN People
 *  Copyright (C) 2006 Atmel
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

#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/input.h>
#include <linux/gpio.h>

#include <asm/mach-types.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include <mach/hardware.h>
<<<<<<< HEAD
#include <mach/board.h>

=======

#include "at91_aic.h"
#include "board.h"
>>>>>>> refs/remotes/origin/master
#include "generic.h"

static void __init flexibity_init_early(void)
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

	/* set serial console to ttyS0 (ie, DBGU) */
	at91_set_serial_console(0);
}

<<<<<<< HEAD
static void __init flexibity_init_irq(void)
{
	at91sam9260_init_interrupts(NULL);
=======
	at91_initialize(18432000);
>>>>>>> refs/remotes/origin/master
}

/* USB Host port */
static struct at91_usbh_data __initdata flexibity_usbh_data = {
	.ports		= 2,
<<<<<<< HEAD
=======
/* USB Host port */
static struct at91_usbh_data __initdata flexibity_usbh_data = {
	.ports		= 2,
	.vbus_pin	= {-EINVAL, -EINVAL},
	.overcurrent_pin= {-EINVAL, -EINVAL},
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.vbus_pin	= {-EINVAL, -EINVAL},
	.overcurrent_pin= {-EINVAL, -EINVAL},
>>>>>>> refs/remotes/origin/master
};

/* USB Device port */
static struct at91_udc_data __initdata flexibity_udc_data = {
	.vbus_pin	= AT91_PIN_PC5,
<<<<<<< HEAD
<<<<<<< HEAD
	.pullup_pin	= 0,		/* pull-up driven by UDC */
=======
=======
>>>>>>> refs/remotes/origin/master
	.pullup_pin	= -EINVAL,		/* pull-up driven by UDC */
};

/* I2C devices */
static struct i2c_board_info __initdata flexibity_i2c_devices[] = {
	{
		I2C_BOARD_INFO("ds1307", 0x68),
	},
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
};

/* SPI devices */
static struct spi_board_info flexibity_spi_devices[] = {
	{	/* DataFlash chip */
		.modalias	= "mtd_dataflash",
		.chip_select	= 1,
		.max_speed_hz	= 15 * 1000 * 1000,
		.bus_num	= 0,
	},
};

/* MCI (SD/MMC) */
<<<<<<< HEAD
static struct at91_mmc_data __initdata flexibity_mmc_data = {
	.slot_b		= 0,
	.wire4		= 1,
	.det_pin	= AT91_PIN_PC9,
	.wp_pin		= AT91_PIN_PC4,
<<<<<<< HEAD
=======
	.vcc_pin	= -EINVAL,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct mci_platform_data __initdata flexibity_mci0_data = {
	.slot[0] = {
		.bus_width	= 4,
		.detect_pin	= AT91_PIN_PC9,
		.wp_pin		= AT91_PIN_PC4,
	},
>>>>>>> refs/remotes/origin/master
};

/* LEDs */
static struct gpio_led flexibity_leds[] = {
	{
		.name			= "usb1:green",
		.gpio			= AT91_PIN_PA12,
		.active_low		= 1,
		.default_trigger	= "default-on",
	},
	{
		.name			= "usb1:red",
		.gpio			= AT91_PIN_PA13,
		.active_low		= 1,
		.default_trigger	= "default-on",
	},
	{
		.name			= "usb2:green",
		.gpio			= AT91_PIN_PB26,
		.active_low		= 1,
		.default_trigger	= "default-on",
	},
	{
		.name			= "usb2:red",
		.gpio			= AT91_PIN_PB27,
		.active_low		= 1,
		.default_trigger	= "default-on",
	},
	{
		.name			= "usb3:green",
		.gpio			= AT91_PIN_PC8,
		.active_low		= 1,
		.default_trigger	= "default-on",
	},
	{
		.name			= "usb3:red",
		.gpio			= AT91_PIN_PC6,
		.active_low		= 1,
		.default_trigger	= "default-on",
	},
	{
		.name			= "usb4:green",
		.gpio			= AT91_PIN_PB4,
		.active_low		= 1,
		.default_trigger	= "default-on",
	},
	{
		.name			= "usb4:red",
		.gpio			= AT91_PIN_PB5,
		.active_low		= 1,
		.default_trigger	= "default-on",
	}
};

static void __init flexibity_board_init(void)
{
	/* Serial */
<<<<<<< HEAD
=======
	/* DBGU on ttyS0. (Rx & Tx only) */
	at91_register_uart(0, 0, 0);
>>>>>>> refs/remotes/origin/master
	at91_add_device_serial();
	/* USB Host */
	at91_add_device_usbh(&flexibity_usbh_data);
	/* USB Device */
	at91_add_device_udc(&flexibity_udc_data);
<<<<<<< HEAD
<<<<<<< HEAD
=======
	/* I2C */
	at91_add_device_i2c(flexibity_i2c_devices,
		ARRAY_SIZE(flexibity_i2c_devices));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* I2C */
	at91_add_device_i2c(flexibity_i2c_devices,
		ARRAY_SIZE(flexibity_i2c_devices));
>>>>>>> refs/remotes/origin/master
	/* SPI */
	at91_add_device_spi(flexibity_spi_devices,
		ARRAY_SIZE(flexibity_spi_devices));
	/* MMC */
<<<<<<< HEAD
	at91_add_device_mmc(0, &flexibity_mmc_data);
=======
	at91_add_device_mci(0, &flexibity_mci0_data);
>>>>>>> refs/remotes/origin/master
	/* LEDs */
	at91_gpio_leds(flexibity_leds, ARRAY_SIZE(flexibity_leds));
}

MACHINE_START(FLEXIBITY, "Flexibity Connect")
	/* Maintainer: Maxim Osipov */
<<<<<<< HEAD
	.timer		= &at91sam926x_timer,
<<<<<<< HEAD
	.map_io		= at91sam9260_map_io,
	.init_early	= flexibity_init_early,
	.init_irq	= flexibity_init_irq,
=======
	.map_io		= at91_map_io,
	.init_early	= flexibity_init_early,
	.init_irq	= at91_init_irq_default,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.init_time	= at91sam926x_pit_init,
	.map_io		= at91_map_io,
	.handle_irq	= at91_aic_handle_irq,
	.init_early	= flexibity_init_early,
	.init_irq	= at91_init_irq_default,
>>>>>>> refs/remotes/origin/master
	.init_machine	= flexibity_board_init,
MACHINE_END
