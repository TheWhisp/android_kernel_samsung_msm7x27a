/* linux/arch/arm/mach-msm/board-sapphire.c
 * Copyright (C) 2007-2009 HTC Corporation.
 * Author: Thomas Tsai <thomas_tsai@htc.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
*/
<<<<<<< HEAD

=======
#include <linux/gpio.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
<<<<<<< HEAD
#include <linux/sysdev.h>

#include <linux/delay.h>

#include <asm/gpio.h>
=======
#include <linux/device.h>

#include <linux/delay.h>

>>>>>>> refs/remotes/origin/cm-10.0
#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/flash.h>
<<<<<<< HEAD
#include <asm/system.h>
#include <mach/system.h>
#include <mach/vreg.h>
#include <mach/board.h>
=======
#include <mach/system.h>
#include <mach/vreg.h>
#include <mach/board.h>
#include <mach/proc_comm.h>
>>>>>>> refs/remotes/origin/cm-10.0

#include <asm/io.h>
#include <asm/delay.h>
#include <asm/setup.h>

#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>

#include "gpio_chip.h"
#include "board-sapphire.h"
<<<<<<< HEAD
#include "proc_comm.h"
=======
>>>>>>> refs/remotes/origin/cm-10.0
#include "devices.h"

void msm_init_irq(void);
void msm_init_gpio(void);

static struct platform_device *devices[] __initdata = {
	&msm_device_smd,
	&msm_device_dmov,
	&msm_device_nand,
	&msm_device_uart1,
	&msm_device_uart3,
};

extern struct sys_timer msm_timer;

static void __init sapphire_init_irq(void)
{
	msm_init_irq();
}

static void __init sapphire_init(void)
{
	platform_add_devices(devices, ARRAY_SIZE(devices));
}

static struct map_desc sapphire_io_desc[] __initdata = {
	{
		.virtual = SAPPHIRE_CPLD_BASE,
		.pfn     = __phys_to_pfn(SAPPHIRE_CPLD_START),
		.length  = SAPPHIRE_CPLD_SIZE,
		.type    = MT_DEVICE_NONSHARED
	}
};

<<<<<<< HEAD
static void __init sapphire_fixup(struct machine_desc *desc, struct tag *tags,
				  char **cmdline, struct meminfo *mi)
=======
static void __init sapphire_fixup(struct tag *tags, char **cmdline,
				  struct meminfo *mi)
>>>>>>> refs/remotes/origin/cm-10.0
{
	int smi_sz = parse_tag_smi((const struct tag *)tags);

	mi->nr_banks = 1;
	mi->bank[0].start = PHYS_OFFSET;
	mi->bank[0].node = PHYS_TO_NID(PHYS_OFFSET);
	if (smi_sz == 32) {
		mi->bank[0].size = (84*1024*1024);
	} else if (smi_sz == 64) {
		mi->bank[0].size = (101*1024*1024);
	} else {
		/* Give a default value when not get smi size */
		smi_sz = 64;
		mi->bank[0].size = (101*1024*1024);
	}
}

static void __init sapphire_map_io(void)
{
	msm_map_common_io();
	iotable_init(sapphire_io_desc, ARRAY_SIZE(sapphire_io_desc));
	msm_clock_init();
}

MACHINE_START(SAPPHIRE, "sapphire")
/* Maintainer: Brian Swetland <swetland@google.com> */
<<<<<<< HEAD
	.boot_params    = PLAT_PHYS_OFFSET + 0x100,
=======
	.atag_offset    = 0x100,
>>>>>>> refs/remotes/origin/cm-10.0
	.fixup          = sapphire_fixup,
	.map_io         = sapphire_map_io,
	.init_irq       = sapphire_init_irq,
	.init_machine   = sapphire_init,
	.timer          = &msm_timer,
MACHINE_END
