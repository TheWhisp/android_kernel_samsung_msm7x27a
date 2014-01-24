/*
 * Copyright STMicroelectronics, 2007.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/types.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/amba/bus.h>
#include <linux/platform_device.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/gpio.h>

=======
#include <linux/io.h>

#include <plat/gpio-nomadik.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <mach/hardware.h>
#include <mach/irqs.h>
#include <asm/mach/map.h>
#include <asm/hardware/vic.h>
=======
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/dma-mapping.h>
#include <linux/of_irq.h>
#include <linux/of_gpio.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/gpio.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/time.h>
#include <asm/mach-types.h>
>>>>>>> refs/remotes/origin/master

#include <asm/cacheflush.h>
#include <asm/hardware/cache-l2x0.h>

<<<<<<< HEAD
#include "clock.h"
<<<<<<< HEAD
=======
#include "cpu-8815.h"
>>>>>>> refs/remotes/origin/cm-10.0

#define __MEM_4K_RESOURCE(x) \
	.res = {.start = (x), .end = (x) + SZ_4K - 1, .flags = IORESOURCE_MEM}

/* The 8815 has 4 GPIO blocks, let's register them immediately */

#define GPIO_RESOURCE(block)						\
	{								\
		.start	= NOMADIK_GPIO##block##_BASE,			\
		.end	= NOMADIK_GPIO##block##_BASE + SZ_4K - 1,	\
		.flags	= IORESOURCE_MEM,				\
	},								\
	{								\
		.start	= IRQ_GPIO##block,				\
		.end	= IRQ_GPIO##block,				\
		.flags	= IORESOURCE_IRQ,				\
	}

#define GPIO_DEVICE(block)						\
	{								\
		.name 		= "gpio",				\
		.id		= block,				\
		.num_resources 	= 2,					\
		.resource	= &cpu8815_gpio_resources[block * 2],	\
		.dev = {						\
			.platform_data = &cpu8815_gpio[block],		\
		},							\
	}

static struct nmk_gpio_platform_data cpu8815_gpio[] = {
	{
		.name = "GPIO-0-31",
		.first_gpio = 0,
		.first_irq = NOMADIK_GPIO_TO_IRQ(0),
	}, {
		.name = "GPIO-32-63",
		.first_gpio = 32,
		.first_irq = NOMADIK_GPIO_TO_IRQ(32),
	}, {
		.name = "GPIO-64-95",
		.first_gpio = 64,
		.first_irq = NOMADIK_GPIO_TO_IRQ(64),
	}, {
		.name = "GPIO-96-127", /* 124..127 not routed to pin */
		.first_gpio = 96,
		.first_irq = NOMADIK_GPIO_TO_IRQ(96),
	}
};

static struct resource cpu8815_gpio_resources[] = {
	GPIO_RESOURCE(0),
	GPIO_RESOURCE(1),
	GPIO_RESOURCE(2),
	GPIO_RESOURCE(3),
};

static struct platform_device cpu8815_platform_gpio[] = {
	GPIO_DEVICE(0),
	GPIO_DEVICE(1),
	GPIO_DEVICE(2),
	GPIO_DEVICE(3),
};

<<<<<<< HEAD
static struct amba_device cpu8815_amba_rng = {
	.dev = {
		.init_name = "rng",
	},
	__MEM_4K_RESOURCE(NOMADIK_RNG_BASE),
};
=======
static AMBA_APB_DEVICE(cpu8815_amba_rng, "rng", 0, NOMADIK_RNG_BASE, { }, NULL);
>>>>>>> refs/remotes/origin/cm-10.0

static struct platform_device *platform_devs[] __initdata = {
	cpu8815_platform_gpio + 0,
	cpu8815_platform_gpio + 1,
	cpu8815_platform_gpio + 2,
	cpu8815_platform_gpio + 3,
};

static struct amba_device *amba_devs[] __initdata = {
<<<<<<< HEAD
	&cpu8815_amba_rng
=======
	&cpu8815_amba_rng_device
>>>>>>> refs/remotes/origin/cm-10.0
};

static int __init cpu8815_init(void)
{
	int i;

	platform_add_devices(platform_devs, ARRAY_SIZE(platform_devs));
	for (i = 0; i < ARRAY_SIZE(amba_devs); i++)
		amba_device_register(amba_devs[i], &iomem_resource);
	return 0;
}
arch_initcall(cpu8815_init);

/* All SoC devices live in the same area (see hardware.h) */
static struct map_desc nomadik_io_desc[] __initdata = {
	{
		.virtual =	NOMADIK_IO_VIRTUAL,
		.pfn =		__phys_to_pfn(NOMADIK_IO_PHYSICAL),
		.length =	NOMADIK_IO_SIZE,
		.type = 	MT_DEVICE,
	}
	/* static ram and secured ram may be added later */
};

void __init cpu8815_map_io(void)
{
	iotable_init(nomadik_io_desc, ARRAY_SIZE(nomadik_io_desc));
}

void __init cpu8815_init_irq(void)
{
	/* This modified VIC cell has two register blocks, at 0 and 0x20 */
	vic_init(io_p2v(NOMADIK_IC_BASE + 0x00), IRQ_VIC_START +  0, ~0, 0);
	vic_init(io_p2v(NOMADIK_IC_BASE + 0x20), IRQ_VIC_START + 32, ~0, 0);

	/*
	 * Init clocks here so that they are available for system timer
	 * initialization.
	 */
	clk_init();
}

/*
 * This function is called from the board init ("init_machine").
 */
 void __init cpu8815_platform_init(void)
{
#ifdef CONFIG_CACHE_L2X0
	/* At full speed latency must be >=2, so 0x249 in low bits */
	l2x0_init(io_p2v(NOMADIK_L2CC_BASE), 0x00730249, 0xfe000fff);
#endif
	 return;
}
<<<<<<< HEAD
=======

void cpu8815_restart(char mode, const char *cmd)
{
	void __iomem *src_rstsr = io_p2v(NOMADIK_SRC_BASE + 0x18);

	/* FIXME: use egpio when implemented */

	/* Write anything to Reset status register */
	writel(1, src_rstsr);
}
>>>>>>> refs/remotes/origin/cm-10.0
=======
/*
 * These are the only hard-coded address offsets we still have to use.
 */
#define NOMADIK_FSMC_BASE	0x10100000	/* FSMC registers */
#define NOMADIK_SDRAMC_BASE	0x10110000	/* SDRAM Controller */
#define NOMADIK_CLCDC_BASE	0x10120000	/* CLCD Controller */
#define NOMADIK_MDIF_BASE	0x10120000	/* MDIF */
#define NOMADIK_DMA0_BASE	0x10130000	/* DMA0 Controller */
#define NOMADIK_IC_BASE		0x10140000	/* Vectored Irq Controller */
#define NOMADIK_DMA1_BASE	0x10150000	/* DMA1 Controller */
#define NOMADIK_USB_BASE	0x10170000	/* USB-OTG conf reg base */
#define NOMADIK_CRYP_BASE	0x10180000	/* Crypto processor */
#define NOMADIK_SHA1_BASE	0x10190000	/* SHA-1 Processor */
#define NOMADIK_XTI_BASE	0x101A0000	/* XTI */
#define NOMADIK_RNG_BASE	0x101B0000	/* Random number generator */
#define NOMADIK_SRC_BASE	0x101E0000	/* SRC base */
#define NOMADIK_WDOG_BASE	0x101E1000	/* Watchdog */
#define NOMADIK_MTU0_BASE	0x101E2000	/* Multiple Timer 0 */
#define NOMADIK_MTU1_BASE	0x101E3000	/* Multiple Timer 1 */
#define NOMADIK_GPIO0_BASE	0x101E4000	/* GPIO0 */
#define NOMADIK_GPIO1_BASE	0x101E5000	/* GPIO1 */
#define NOMADIK_GPIO2_BASE	0x101E6000	/* GPIO2 */
#define NOMADIK_GPIO3_BASE	0x101E7000	/* GPIO3 */
#define NOMADIK_RTC_BASE	0x101E8000	/* Real Time Clock base */
#define NOMADIK_PMU_BASE	0x101E9000	/* Power Management Unit */
#define NOMADIK_OWM_BASE	0x101EA000	/* One wire master */
#define NOMADIK_SCR_BASE	0x101EF000	/* Secure Control registers */
#define NOMADIK_MSP2_BASE	0x101F0000	/* MSP 2 interface */
#define NOMADIK_MSP1_BASE	0x101F1000	/* MSP 1 interface */
#define NOMADIK_UART2_BASE	0x101F2000	/* UART 2 interface */
#define NOMADIK_SSIRx_BASE	0x101F3000	/* SSI 8-ch rx interface */
#define NOMADIK_SSITx_BASE	0x101F4000	/* SSI 8-ch tx interface */
#define NOMADIK_MSHC_BASE	0x101F5000	/* Memory Stick(Pro) Host */
#define NOMADIK_SDI_BASE	0x101F6000	/* SD-card/MM-Card */
#define NOMADIK_I2C1_BASE	0x101F7000	/* I2C1 interface */
#define NOMADIK_I2C0_BASE	0x101F8000	/* I2C0 interface */
#define NOMADIK_MSP0_BASE	0x101F9000	/* MSP 0 interface */
#define NOMADIK_FIRDA_BASE	0x101FA000	/* FIrDA interface */
#define NOMADIK_UART1_BASE	0x101FB000	/* UART 1 interface */
#define NOMADIK_SSP_BASE	0x101FC000	/* SSP interface */
#define NOMADIK_UART0_BASE	0x101FD000	/* UART 0 interface */
#define NOMADIK_SGA_BASE	0x101FE000	/* SGA interface */
#define NOMADIK_L2CC_BASE	0x10210000	/* L2 Cache controller */
#define NOMADIK_UART1_VBASE	0xF01FB000

/* This is needed for LL-debug/earlyprintk/debug-macro.S */
static struct map_desc cpu8815_io_desc[] __initdata = {
	{
		.virtual =	NOMADIK_UART1_VBASE,
		.pfn =		__phys_to_pfn(NOMADIK_UART1_BASE),
		.length =	SZ_4K,
		.type =		MT_DEVICE,
	},
};

static void __init cpu8815_map_io(void)
{
	iotable_init(cpu8815_io_desc, ARRAY_SIZE(cpu8815_io_desc));
}

static void cpu8815_restart(enum reboot_mode mode, const char *cmd)
{
	void __iomem *srcbase = ioremap(NOMADIK_SRC_BASE, SZ_4K);

	/* FIXME: use egpio when implemented */

	/* Write anything to Reset status register */
	writel(1, srcbase + 0x18);
}

/*
 * The SMSC911x IRQ is connected to a GPIO pin, but the driver expects
 * to simply request an IRQ passed as a resource. So the GPIO pin needs
 * to be requested by this hog and set as input.
 */
static int __init cpu8815_eth_init(void)
{
	struct device_node *eth;
	int gpio, irq, err;

	eth = of_find_node_by_path("/usb-s8815/ethernet-gpio");
	if (!eth) {
		pr_info("could not find any ethernet GPIO\n");
		return 0;
	}
	gpio = of_get_gpio(eth, 0);
	err = gpio_request(gpio, "eth_irq");
	if (err) {
		pr_info("failed to request ethernet GPIO\n");
		return -ENODEV;
	}
	err = gpio_direction_input(gpio);
	if (err) {
		pr_info("failed to set ethernet GPIO as input\n");
		return -ENODEV;
	}
	irq = gpio_to_irq(gpio);
	pr_info("enabled USB-S8815 ethernet GPIO %d, IRQ %d\n", gpio, irq);
	return 0;
}
device_initcall(cpu8815_eth_init);

/*
 * This GPIO pin turns on a line that is used to detect card insertion
 * on this board.
 */
static int __init cpu8815_mmcsd_init(void)
{
	struct device_node *cdbias;
	int gpio, err;

	cdbias = of_find_node_by_path("/usb-s8815/mmcsd-gpio");
	if (!cdbias) {
		pr_info("could not find MMC/SD card detect bias node\n");
		return 0;
	}
	gpio = of_get_gpio(cdbias, 0);
	if (gpio < 0) {
		pr_info("could not obtain MMC/SD card detect bias GPIO\n");
		return 0;
	}
	err = gpio_request(gpio, "card detect bias");
	if (err) {
		pr_info("failed to request card detect bias GPIO %d\n", gpio);
		return -ENODEV;
	}
	err = gpio_direction_output(gpio, 0);
	if (err){
		pr_info("failed to set GPIO %d as output, low\n", gpio);
		return err;
	}
	pr_info("enabled USB-S8815 CD bias GPIO %d, low\n", gpio);
	return 0;
}
device_initcall(cpu8815_mmcsd_init);

static void __init cpu8815_init_of(void)
{
#ifdef CONFIG_CACHE_L2X0
	/* At full speed latency must be >=2, so 0x249 in low bits */
	l2x0_of_init(0x00730249, 0xfe000fff);
#endif
	of_platform_populate(NULL, of_default_bus_match_table, NULL, NULL);
}

static const char * cpu8815_board_compat[] = {
	"calaosystems,usb-s8815",
	NULL,
};

DT_MACHINE_START(NOMADIK_DT, "Nomadik STn8815")
	.map_io		= cpu8815_map_io,
	.init_machine	= cpu8815_init_of,
	.restart	= cpu8815_restart,
	.dt_compat      = cpu8815_board_compat,
MACHINE_END
>>>>>>> refs/remotes/origin/master
