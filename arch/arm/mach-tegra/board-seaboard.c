/*
 * Copyright (c) 2010, 2011 NVIDIA Corporation.
 * Copyright (C) 2010, 2011 Google, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/serial_8250.h>
#include <linux/i2c.h>
<<<<<<< HEAD
#include <linux/i2c-tegra.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/delay.h>
#include <linux/input.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/gpio_keys.h>

<<<<<<< HEAD
#include <mach/iomap.h>
#include <mach/irqs.h>
#include <mach/sdhci.h>

#include <asm/mach-types.h>
#include <asm/mach/arch.h>
=======
#include <sound/wm8903.h>

#include <mach/iomap.h>
#include <mach/irqs.h>
#include <mach/sdhci.h>
#include <mach/tegra_wm8903_pdata.h>

#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/hardware/gic.h>
>>>>>>> refs/remotes/origin/cm-10.0

#include "board.h"
#include "board-seaboard.h"
#include "clock.h"
#include "devices.h"
#include "gpio-names.h"

static struct plat_serial8250_port debug_uart_platform_data[] = {
	{
		/* Memory and IRQ filled in before registration */
<<<<<<< HEAD
		.flags		= UPF_BOOT_AUTOCONF,
=======
		.flags		= UPF_BOOT_AUTOCONF | UPF_FIXED_TYPE,
		.type		= PORT_TEGRA,
>>>>>>> refs/remotes/origin/cm-10.0
		.iotype		= UPIO_MEM,
		.regshift	= 2,
		.uartclk	= 216000000,
	}, {
		.flags		= 0,
	}
};

static struct platform_device debug_uart = {
	.name = "serial8250",
	.id = PLAT8250_DEV_PLATFORM,
	.dev = {
		.platform_data = debug_uart_platform_data,
	},
};

static __initdata struct tegra_clk_init_table seaboard_clk_init_table[] = {
	/* name		parent		rate		enabled */
	{ "uartb",	"pll_p",	216000000,	true},
	{ "uartd",	"pll_p",	216000000,	true},
<<<<<<< HEAD
	{ NULL,		NULL,		0,		0},
};

static struct tegra_i2c_platform_data seaboard_i2c1_platform_data = {
	.bus_clk_rate	= 400000.
};

static struct tegra_i2c_platform_data seaboard_i2c2_platform_data = {
	.bus_clk_rate	= 400000,
};

static struct tegra_i2c_platform_data seaboard_i2c3_platform_data = {
	.bus_clk_rate	= 400000,
};

static struct tegra_i2c_platform_data seaboard_dvc_platform_data = {
	.bus_clk_rate	= 400000,
};

=======
	{ "pll_a",	"pll_p_out1",	56448000,	true },
	{ "pll_a_out0",	"pll_a",	11289600,	true },
	{ "cdev1",	NULL,		0,		true },
	{ "i2s1",	"pll_a_out0",	11289600,	false},
	{ "usbd",	"clk_m",	12000000,	true},
	{ "usb3",	"clk_m",	12000000,	true},
	{ NULL,		NULL,		0,		0},
};

>>>>>>> refs/remotes/origin/cm-10.0
static struct gpio_keys_button seaboard_gpio_keys_buttons[] = {
	{
		.code		= SW_LID,
		.gpio		= TEGRA_GPIO_LIDSWITCH,
		.active_low	= 0,
		.desc		= "Lid",
		.type		= EV_SW,
		.wakeup		= 1,
		.debounce_interval = 1,
	},
	{
		.code		= KEY_POWER,
		.gpio		= TEGRA_GPIO_POWERKEY,
		.active_low	= 1,
		.desc		= "Power",
		.type		= EV_KEY,
		.wakeup		= 1,
	},
};

static struct gpio_keys_platform_data seaboard_gpio_keys = {
	.buttons	= seaboard_gpio_keys_buttons,
	.nbuttons	= ARRAY_SIZE(seaboard_gpio_keys_buttons),
};

static struct platform_device seaboard_gpio_keys_device = {
	.name		= "gpio-keys",
	.id		= -1,
	.dev		= {
		.platform_data = &seaboard_gpio_keys,
	}
};

static struct tegra_sdhci_platform_data sdhci_pdata1 = {
	.cd_gpio	= -1,
	.wp_gpio	= -1,
	.power_gpio	= -1,
};

static struct tegra_sdhci_platform_data sdhci_pdata3 = {
	.cd_gpio	= TEGRA_GPIO_SD2_CD,
	.wp_gpio	= TEGRA_GPIO_SD2_WP,
	.power_gpio	= TEGRA_GPIO_SD2_POWER,
};

static struct tegra_sdhci_platform_data sdhci_pdata4 = {
	.cd_gpio	= -1,
	.wp_gpio	= -1,
	.power_gpio	= -1,
	.is_8bit	= 1,
};

<<<<<<< HEAD
static struct platform_device *seaboard_devices[] __initdata = {
	&debug_uart,
	&tegra_pmu_device,
	&tegra_sdhci_device1,
	&tegra_sdhci_device3,
	&tegra_sdhci_device4,
	&seaboard_gpio_keys_device,
=======
static struct tegra_wm8903_platform_data seaboard_audio_pdata = {
	.gpio_spkr_en		= TEGRA_GPIO_SPKR_EN,
	.gpio_hp_det		= TEGRA_GPIO_HP_DET,
	.gpio_hp_mute		= -1,
	.gpio_int_mic_en	= -1,
	.gpio_ext_mic_en	= -1,
};

static struct platform_device seaboard_audio_device = {
	.name	= "tegra-snd-wm8903",
	.id	= 0,
	.dev	= {
		.platform_data  = &seaboard_audio_pdata,
	},
};

static struct platform_device *seaboard_devices[] __initdata = {
	&debug_uart,
	&tegra_pmu_device,
	&tegra_sdhci_device4,
	&tegra_sdhci_device3,
	&tegra_sdhci_device1,
	&seaboard_gpio_keys_device,
	&tegra_i2s_device1,
	&tegra_das_device,
	&tegra_pcm_device,
	&seaboard_audio_device,
>>>>>>> refs/remotes/origin/cm-10.0
};

static struct i2c_board_info __initdata isl29018_device = {
	I2C_BOARD_INFO("isl29018", 0x44),
<<<<<<< HEAD
	.irq = TEGRA_GPIO_TO_IRQ(TEGRA_GPIO_ISL29018_IRQ),
=======
>>>>>>> refs/remotes/origin/cm-10.0
};

static struct i2c_board_info __initdata adt7461_device = {
	I2C_BOARD_INFO("adt7461", 0x4c),
};

<<<<<<< HEAD
=======
static struct wm8903_platform_data wm8903_pdata = {
	.irq_active_low = 0,
	.micdet_cfg = 0,
	.micdet_delay = 100,
	.gpio_base = SEABOARD_GPIO_WM8903(0),
	.gpio_cfg = {
		0,
		0,
		WM8903_GPIO_CONFIG_ZERO,
		0,
		0,
	},
};

static struct i2c_board_info __initdata wm8903_device = {
	I2C_BOARD_INFO("wm8903", 0x1a),
	.platform_data = &wm8903_pdata,
};

static int seaboard_ehci_init(void)
{
	int gpio_status;

	gpio_status = gpio_request(TEGRA_GPIO_USB1, "VBUS_USB1");
	if (gpio_status < 0) {
		pr_err("VBUS_USB1 request GPIO FAILED\n");
		WARN_ON(1);
	}

	gpio_status = gpio_direction_output(TEGRA_GPIO_USB1, 1);
	if (gpio_status < 0) {
		pr_err("VBUS_USB1 request GPIO DIRECTION FAILED\n");
		WARN_ON(1);
	}
	gpio_set_value(TEGRA_GPIO_USB1, 1);

	platform_device_register(&tegra_ehci1_device);
	platform_device_register(&tegra_ehci3_device);

	return 0;
}

>>>>>>> refs/remotes/origin/cm-10.0
static void __init seaboard_i2c_init(void)
{
	gpio_request(TEGRA_GPIO_ISL29018_IRQ, "isl29018");
	gpio_direction_input(TEGRA_GPIO_ISL29018_IRQ);

<<<<<<< HEAD
	i2c_register_board_info(0, &isl29018_device, 1);

	i2c_register_board_info(4, &adt7461_device, 1);

	tegra_i2c_device1.dev.platform_data = &seaboard_i2c1_platform_data;
	tegra_i2c_device2.dev.platform_data = &seaboard_i2c2_platform_data;
	tegra_i2c_device3.dev.platform_data = &seaboard_i2c3_platform_data;
	tegra_i2c_device4.dev.platform_data = &seaboard_dvc_platform_data;
=======
	isl29018_device.irq = gpio_to_irq(TEGRA_GPIO_ISL29018_IRQ);
	i2c_register_board_info(0, &isl29018_device, 1);

	wm8903_device.irq = gpio_to_irq(TEGRA_GPIO_CDC_IRQ);
	i2c_register_board_info(0, &wm8903_device, 1);

	i2c_register_board_info(3, &adt7461_device, 1);
>>>>>>> refs/remotes/origin/cm-10.0

	platform_device_register(&tegra_i2c_device1);
	platform_device_register(&tegra_i2c_device2);
	platform_device_register(&tegra_i2c_device3);
	platform_device_register(&tegra_i2c_device4);
}

static void __init seaboard_common_init(void)
{
	seaboard_pinmux_init();

	tegra_clk_init_from_table(seaboard_clk_init_table);

	tegra_sdhci_device1.dev.platform_data = &sdhci_pdata1;
	tegra_sdhci_device3.dev.platform_data = &sdhci_pdata3;
	tegra_sdhci_device4.dev.platform_data = &sdhci_pdata4;

	platform_add_devices(seaboard_devices, ARRAY_SIZE(seaboard_devices));
<<<<<<< HEAD
=======

	seaboard_ehci_init();
>>>>>>> refs/remotes/origin/cm-10.0
}

static void __init tegra_seaboard_init(void)
{
	/* Seaboard uses UARTD for the debug port. */
	debug_uart_platform_data[0].membase = IO_ADDRESS(TEGRA_UARTD_BASE);
	debug_uart_platform_data[0].mapbase = TEGRA_UARTD_BASE;
	debug_uart_platform_data[0].irq = INT_UARTD;

	seaboard_common_init();

	seaboard_i2c_init();
}

static void __init tegra_kaen_init(void)
{
	/* Kaen uses UARTB for the debug port. */
	debug_uart_platform_data[0].membase = IO_ADDRESS(TEGRA_UARTB_BASE);
	debug_uart_platform_data[0].mapbase = TEGRA_UARTB_BASE;
	debug_uart_platform_data[0].irq = INT_UARTB;

<<<<<<< HEAD
=======
	seaboard_audio_pdata.gpio_hp_mute = TEGRA_GPIO_KAEN_HP_MUTE;
	tegra_gpio_enable(TEGRA_GPIO_KAEN_HP_MUTE);

>>>>>>> refs/remotes/origin/cm-10.0
	seaboard_common_init();

	seaboard_i2c_init();
}

static void __init tegra_wario_init(void)
{
	/* Wario uses UARTB for the debug port. */
	debug_uart_platform_data[0].membase = IO_ADDRESS(TEGRA_UARTB_BASE);
	debug_uart_platform_data[0].mapbase = TEGRA_UARTB_BASE;
	debug_uart_platform_data[0].irq = INT_UARTB;

	seaboard_common_init();

	seaboard_i2c_init();
}


MACHINE_START(SEABOARD, "seaboard")
<<<<<<< HEAD
	.boot_params    = 0x00000100,
	.map_io         = tegra_map_common_io,
	.init_early     = tegra_init_early,
	.init_irq       = tegra_init_irq,
	.timer          = &tegra_timer,
	.init_machine   = tegra_seaboard_init,
MACHINE_END

MACHINE_START(KAEN, "kaen")
	.boot_params    = 0x00000100,
	.map_io         = tegra_map_common_io,
	.init_early     = tegra_init_early,
	.init_irq       = tegra_init_irq,
	.timer          = &tegra_timer,
	.init_machine   = tegra_kaen_init,
MACHINE_END

MACHINE_START(WARIO, "wario")
	.boot_params    = 0x00000100,
	.map_io         = tegra_map_common_io,
	.init_early     = tegra_init_early,
	.init_irq       = tegra_init_irq,
	.timer          = &tegra_timer,
	.init_machine   = tegra_wario_init,
=======
	.atag_offset    = 0x100,
	.map_io         = tegra_map_common_io,
	.init_early     = tegra20_init_early,
	.init_irq       = tegra_init_irq,
	.handle_irq	= gic_handle_irq,
	.timer          = &tegra_timer,
	.init_machine   = tegra_seaboard_init,
	.restart	= tegra_assert_system_reset,
MACHINE_END

MACHINE_START(KAEN, "kaen")
	.atag_offset    = 0x100,
	.map_io         = tegra_map_common_io,
	.init_early     = tegra20_init_early,
	.init_irq       = tegra_init_irq,
	.handle_irq	= gic_handle_irq,
	.timer          = &tegra_timer,
	.init_machine   = tegra_kaen_init,
	.restart	= tegra_assert_system_reset,
MACHINE_END

MACHINE_START(WARIO, "wario")
	.atag_offset    = 0x100,
	.map_io         = tegra_map_common_io,
	.init_early     = tegra20_init_early,
	.init_irq       = tegra_init_irq,
	.handle_irq	= gic_handle_irq,
	.timer          = &tegra_timer,
	.init_machine   = tegra_wario_init,
	.restart	= tegra_assert_system_reset,
>>>>>>> refs/remotes/origin/cm-10.0
MACHINE_END
