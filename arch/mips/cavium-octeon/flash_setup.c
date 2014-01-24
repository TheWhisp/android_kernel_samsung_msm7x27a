/*
 *   Octeon Bootbus flash setup
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2007, 2008 Cavium Networks
 */
#include <linux/kernel.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
>>>>>>> refs/remotes/origin/master
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>

#include <asm/octeon/octeon.h>

static struct map_info flash_map;
static struct mtd_info *mymtd;
<<<<<<< HEAD
<<<<<<< HEAD
static int nr_parts;
static struct mtd_partition *parts;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static const char *part_probe_types[] = {
	"cmdlinepart",
#ifdef CONFIG_MTD_REDBOOT_PARTS
	"RedBoot",
#endif
	NULL
};

/**
 * Module/ driver initialization.
 *
 * Returns Zero on success
 */
static int __init flash_init(void)
{
	/*
	 * Read the bootbus region 0 setup to determine the base
	 * address of the flash.
	 */
	union cvmx_mio_boot_reg_cfgx region_cfg;
	region_cfg.u64 = cvmx_read_csr(CVMX_MIO_BOOT_REG_CFGX(0));
	if (region_cfg.s.en) {
		/*
		 * The bootloader always takes the flash and sets its
		 * address so the entire flash fits below
		 * 0x1fc00000. This way the flash aliases to
		 * 0x1fc00000 for booting. Software can access the
		 * full flash at the true address, while core boot can
		 * access 4MB.
		 */
		/* Use this name so old part lines work */
		flash_map.name = "phys_mapped_flash";
		flash_map.phys = region_cfg.s.base << 16;
		flash_map.size = 0x1fc00000 - flash_map.phys;
<<<<<<< HEAD
		flash_map.bankwidth = 1;
=======
		/* 8-bit bus (0 + 1) or 16-bit bus (1 + 1) */
		flash_map.bankwidth = region_cfg.s.width + 1;
>>>>>>> refs/remotes/origin/master
		flash_map.virt = ioremap(flash_map.phys, flash_map.size);
		pr_notice("Bootbus flash: Setting flash for %luMB flash at "
			  "0x%08llx\n", flash_map.size >> 20, flash_map.phys);
		simple_map_init(&flash_map);
		mymtd = do_map_probe("cfi_probe", &flash_map);
		if (mymtd) {
			mymtd->owner = THIS_MODULE;
<<<<<<< HEAD
<<<<<<< HEAD

			nr_parts = parse_mtd_partitions(mymtd,
							part_probe_types,
							&parts, 0);
			mtd_device_register(mymtd, parts, nr_parts);
=======
			mtd_device_parse_register(mymtd, part_probe_types,
						  NULL, NULL, 0);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			mtd_device_parse_register(mymtd, part_probe_types,
						  NULL, NULL, 0);
>>>>>>> refs/remotes/origin/master
		} else {
			pr_err("Failed to register MTD device for flash\n");
		}
	}
	return 0;
}

late_initcall(flash_init);
