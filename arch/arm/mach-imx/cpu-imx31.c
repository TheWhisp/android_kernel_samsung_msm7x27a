/*
 * MX31 CPU type detection
 *
 * Copyright (c) 2009 Daniel Mack <daniel@caiaq.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/io.h>
<<<<<<< HEAD
#include <mach/hardware.h>
#include <mach/iim.h>
<<<<<<< HEAD

unsigned int mx31_cpu_rev;
EXPORT_SYMBOL(mx31_cpu_rev);
=======
#include <mach/common.h>

static int mx31_cpu_rev = -1;
>>>>>>> refs/remotes/origin/cm-10.0
=======

#include "common.h"
#include "hardware.h"
#include "iim.h"

static int mx31_cpu_rev = -1;
>>>>>>> refs/remotes/origin/master

static struct {
	u8 srev;
	const char *name;
<<<<<<< HEAD
<<<<<<< HEAD
	const char *v;
	unsigned int rev;
} mx31_cpu_type[] __initdata = {
	{ .srev = 0x00, .name = "i.MX31(L)", .v = "1.0",  .rev = IMX_CHIP_REVISION_1_0	},
	{ .srev = 0x10, .name = "i.MX31",    .v = "1.1",  .rev = IMX_CHIP_REVISION_1_1	},
	{ .srev = 0x11, .name = "i.MX31L",   .v = "1.1",  .rev = IMX_CHIP_REVISION_1_1	},
	{ .srev = 0x12, .name = "i.MX31",    .v = "1.15", .rev = IMX_CHIP_REVISION_1_1	},
	{ .srev = 0x13, .name = "i.MX31L",   .v = "1.15", .rev = IMX_CHIP_REVISION_1_1	},
	{ .srev = 0x14, .name = "i.MX31",    .v = "1.2",  .rev = IMX_CHIP_REVISION_1_2	},
	{ .srev = 0x15, .name = "i.MX31L",   .v = "1.2",  .rev = IMX_CHIP_REVISION_1_2	},
	{ .srev = 0x28, .name = "i.MX31",    .v = "2.0",  .rev = IMX_CHIP_REVISION_2_0	},
	{ .srev = 0x29, .name = "i.MX31L",   .v = "2.0",  .rev = IMX_CHIP_REVISION_2_0	},
};

void __init mx31_read_cpu_rev(void)
=======
=======
>>>>>>> refs/remotes/origin/master
	unsigned int rev;
} mx31_cpu_type[] = {
	{ .srev = 0x00, .name = "i.MX31(L)", .rev = IMX_CHIP_REVISION_1_0 },
	{ .srev = 0x10, .name = "i.MX31",    .rev = IMX_CHIP_REVISION_1_1 },
	{ .srev = 0x11, .name = "i.MX31L",   .rev = IMX_CHIP_REVISION_1_1 },
	{ .srev = 0x12, .name = "i.MX31",    .rev = IMX_CHIP_REVISION_1_1 },
	{ .srev = 0x13, .name = "i.MX31L",   .rev = IMX_CHIP_REVISION_1_1 },
	{ .srev = 0x14, .name = "i.MX31",    .rev = IMX_CHIP_REVISION_1_2 },
	{ .srev = 0x15, .name = "i.MX31L",   .rev = IMX_CHIP_REVISION_1_2 },
	{ .srev = 0x28, .name = "i.MX31",    .rev = IMX_CHIP_REVISION_2_0 },
	{ .srev = 0x29, .name = "i.MX31L",   .rev = IMX_CHIP_REVISION_2_0 },
};

static int mx31_read_cpu_rev(void)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
{
	u32 i, srev;

	/* read SREV register from IIM module */
	srev = __raw_readl(MX31_IO_ADDRESS(MX31_IIM_BASE_ADDR + MXC_IIMSREV));
<<<<<<< HEAD
<<<<<<< HEAD

	for (i = 0; i < ARRAY_SIZE(mx31_cpu_type); i++)
		if (srev == mx31_cpu_type[i].srev) {
			printk(KERN_INFO
				"CPU identified as %s, silicon rev %s\n",
				mx31_cpu_type[i].name, mx31_cpu_type[i].v);

			mx31_cpu_rev = mx31_cpu_type[i].rev;
			return;
		}

	mx31_cpu_rev = IMX_CHIP_REVISION_UNKNOWN;

	printk(KERN_WARNING "Unknown CPU identifier. srev = %02x\n", srev);
}
=======
=======
>>>>>>> refs/remotes/origin/master
	srev &= 0xff;

	for (i = 0; i < ARRAY_SIZE(mx31_cpu_type); i++)
		if (srev == mx31_cpu_type[i].srev) {
			imx_print_silicon_rev(mx31_cpu_type[i].name,
						mx31_cpu_type[i].rev);
			return mx31_cpu_type[i].rev;
		}

	imx_print_silicon_rev("i.MX31", IMX_CHIP_REVISION_UNKNOWN);
	return IMX_CHIP_REVISION_UNKNOWN;
}

int mx31_revision(void)
{
	if (mx31_cpu_rev == -1)
		mx31_cpu_rev = mx31_read_cpu_rev();

	return mx31_cpu_rev;
}
EXPORT_SYMBOL(mx31_revision);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
