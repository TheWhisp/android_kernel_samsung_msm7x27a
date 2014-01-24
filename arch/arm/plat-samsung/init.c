/* linux/arch/arm/plat-s3c/init.c
 *
 * Copyright (c) 2008 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *	http://armlinux.simtec.co.uk/
 *
 * S3C series CPU initialisation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

<<<<<<< HEAD
=======
/*
 * NOTE: Code in this file is not used on S3C64xx when booting with
 * Device Tree support.
 */

>>>>>>> refs/remotes/origin/master
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/serial_core.h>
#include <linux/platform_device.h>
<<<<<<< HEAD
=======
#include <linux/of.h>
>>>>>>> refs/remotes/origin/master

#include <mach/hardware.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include <plat/cpu.h>
#include <plat/devs.h>
#include <plat/clock.h>

#include <plat/regs-serial.h>

static struct cpu_table *cpu;

static struct cpu_table * __init s3c_lookup_cpu(unsigned long idcode,
						struct cpu_table *tab,
						unsigned int count)
{
	for (; count != 0; count--, tab++) {
		if ((idcode & tab->idmask) == (tab->idcode & tab->idmask))
			return tab;
	}

	return NULL;
}

void __init s3c_init_cpu(unsigned long idcode,
			 struct cpu_table *cputab, unsigned int cputab_size)
{
	cpu = s3c_lookup_cpu(idcode, cputab, cputab_size);

	if (cpu == NULL) {
		printk(KERN_ERR "Unknown CPU type 0x%08lx\n", idcode);
		panic("Unknown S3C24XX CPU");
	}

	printk("CPU %s (id 0x%08lx)\n", cpu->name, idcode);

<<<<<<< HEAD
	if (cpu->map_io == NULL || cpu->init == NULL) {
=======
	if (cpu->init == NULL) {
>>>>>>> refs/remotes/origin/master
		printk(KERN_ERR "CPU %s support not enabled\n", cpu->name);
		panic("Unsupported Samsung CPU");
	}

<<<<<<< HEAD
	cpu->map_io();
=======
	if (cpu->map_io)
		cpu->map_io();
>>>>>>> refs/remotes/origin/master
}

/* s3c24xx_init_clocks
 *
 * Initialise the clock subsystem and associated information from the
 * given master crystal value.
 *
 * xtal  = 0 -> use default PLL crystal value (normally 12MHz)
 *      != 0 -> PLL crystal value in Hz
*/

void __init s3c24xx_init_clocks(int xtal)
{
	if (xtal == 0)
		xtal = 12*1000*1000;

	if (cpu == NULL)
		panic("s3c24xx_init_clocks: no cpu setup?\n");

	if (cpu->init_clocks == NULL)
		panic("s3c24xx_init_clocks: cpu has no clock init\n");
	else
		(cpu->init_clocks)(xtal);
}

/* uart management */
<<<<<<< HEAD

=======
#if IS_ENABLED(CONFIG_SAMSUNG_ATAGS)
>>>>>>> refs/remotes/origin/master
static int nr_uarts __initdata = 0;

static struct s3c2410_uartcfg uart_cfgs[CONFIG_SERIAL_SAMSUNG_UARTS];

/* s3c24xx_init_uartdevs
 *
 * copy the specified platform data and configuration into our central
 * set of devices, before the data is thrown away after the init process.
 *
 * This also fills in the array passed to the serial driver for the
 * early initialisation of the console.
*/

void __init s3c24xx_init_uartdevs(char *name,
				  struct s3c24xx_uart_resources *res,
				  struct s3c2410_uartcfg *cfg, int no)
{
	struct platform_device *platdev;
	struct s3c2410_uartcfg *cfgptr = uart_cfgs;
	struct s3c24xx_uart_resources *resp;
	int uart;

	memcpy(cfgptr, cfg, sizeof(struct s3c2410_uartcfg) * no);

	for (uart = 0; uart < no; uart++, cfg++, cfgptr++) {
		platdev = s3c24xx_uart_src[cfgptr->hwport];

		resp = res + cfgptr->hwport;

		s3c24xx_uart_devs[uart] = platdev;

		platdev->name = name;
		platdev->resource = resp->resources;
		platdev->num_resources = resp->nr_resources;

		platdev->dev.platform_data = cfgptr;
	}

	nr_uarts = no;
}

void __init s3c24xx_init_uarts(struct s3c2410_uartcfg *cfg, int no)
{
	if (cpu == NULL)
		return;

<<<<<<< HEAD
	if (cpu->init_uarts == NULL) {
=======
	if (cpu->init_uarts == NULL && IS_ENABLED(CONFIG_SAMSUNG_ATAGS)) {
>>>>>>> refs/remotes/origin/master
		printk(KERN_ERR "s3c24xx_init_uarts: cpu has no uart init\n");
	} else
		(cpu->init_uarts)(cfg, no);
}
<<<<<<< HEAD
=======
#endif
>>>>>>> refs/remotes/origin/master

static int __init s3c_arch_init(void)
{
	int ret;

	// do the correct init for cpu

<<<<<<< HEAD
	if (cpu == NULL)
		panic("s3c_arch_init: NULL cpu\n");
=======
	if (cpu == NULL) {
		/* Not needed when booting with device tree. */
		if (of_have_populated_dt())
			return 0;
		panic("s3c_arch_init: NULL cpu\n");
	}
>>>>>>> refs/remotes/origin/master

	ret = (cpu->init)();
	if (ret != 0)
		return ret;
<<<<<<< HEAD

	ret = platform_add_devices(s3c24xx_uart_devs, nr_uarts);
=======
#if IS_ENABLED(CONFIG_SAMSUNG_ATAGS)
	ret = platform_add_devices(s3c24xx_uart_devs, nr_uarts);
#endif
>>>>>>> refs/remotes/origin/master
	return ret;
}

arch_initcall(s3c_arch_init);
