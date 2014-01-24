/*
 * Map driver for Intel XScale PXA2xx platforms.
 *
 * Author:	Nicolas Pitre
 * Copyright:	(C) 2001 MontaVista Software Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>

#include <asm/io.h>
#include <mach/hardware.h>

#include <asm/mach/flash.h>

#define CACHELINESIZE	32

static void pxa2xx_map_inval_cache(struct map_info *map, unsigned long from,
				      ssize_t len)
{
	unsigned long start = (unsigned long)map->cached + from;
	unsigned long end = start + len;

	start &= ~(CACHELINESIZE - 1);
	while (start < end) {
		/* invalidate D cache line */
		asm volatile ("mcr p15, 0, %0, c7, c6, 1" : : "r" (start));
		start += CACHELINESIZE;
	}
}

struct pxa2xx_flash_info {
<<<<<<< HEAD
<<<<<<< HEAD
	struct mtd_partition	*parts;
	int			nr_parts;
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	struct mtd_info		*mtd;
	struct map_info		map;
};

<<<<<<< HEAD

static const char *probes[] = { "RedBoot", "cmdlinepart", NULL };


static int __devinit pxa2xx_flash_probe(struct platform_device *pdev)
{
	struct flash_platform_data *flash = pdev->dev.platform_data;
	struct pxa2xx_flash_info *info;
<<<<<<< HEAD
	struct mtd_partition *parts;
	struct resource *res;
	int ret = 0;
=======
	struct resource *res;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static const char * const probes[] = { "RedBoot", "cmdlinepart", NULL };

static int pxa2xx_flash_probe(struct platform_device *pdev)
{
	struct flash_platform_data *flash = dev_get_platdata(&pdev->dev);
	struct pxa2xx_flash_info *info;
	struct resource *res;
>>>>>>> refs/remotes/origin/master

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
		return -ENODEV;

	info = kzalloc(sizeof(struct pxa2xx_flash_info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;

	info->map.name = (char *) flash->name;
	info->map.bankwidth = flash->width;
	info->map.phys = res->start;
<<<<<<< HEAD
<<<<<<< HEAD
	info->map.size = res->end - res->start + 1;
	info->parts = flash->parts;
	info->nr_parts = flash->nr_parts;
=======
	info->map.size = resource_size(res);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	info->map.size = resource_size(res);
>>>>>>> refs/remotes/origin/master

	info->map.virt = ioremap(info->map.phys, info->map.size);
	if (!info->map.virt) {
		printk(KERN_WARNING "Failed to ioremap %s\n",
		       info->map.name);
		return -ENOMEM;
	}
	info->map.cached =
<<<<<<< HEAD
		ioremap_cached(info->map.phys, info->map.size);
=======
		ioremap_cache(info->map.phys, info->map.size);
>>>>>>> refs/remotes/origin/master
	if (!info->map.cached)
		printk(KERN_WARNING "Failed to ioremap cached %s\n",
		       info->map.name);
	info->map.inval_cache = pxa2xx_map_inval_cache;
	simple_map_init(&info->map);

	printk(KERN_NOTICE
	       "Probing %s at physical address 0x%08lx"
	       " (%d-bit bankwidth)\n",
	       info->map.name, (unsigned long)info->map.phys,
	       info->map.bankwidth * 8);

	info->mtd = do_map_probe(flash->map_name, &info->map);

	if (!info->mtd) {
		iounmap((void *)info->map.virt);
		if (info->map.cached)
			iounmap(info->map.cached);
		return -EIO;
	}
	info->mtd->owner = THIS_MODULE;

<<<<<<< HEAD
<<<<<<< HEAD
	ret = parse_mtd_partitions(info->mtd, probes, &parts, 0);

	if (ret > 0) {
		info->nr_parts = ret;
		info->parts = parts;
	}

	if (!info->nr_parts)
		printk("Registering %s as whole device\n",
		       info->map.name);

	mtd_device_register(info->mtd, info->parts, info->nr_parts);
=======
	mtd_device_parse_register(info->mtd, probes, NULL, flash->parts,
				  flash->nr_parts);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	mtd_device_parse_register(info->mtd, probes, NULL, flash->parts,
				  flash->nr_parts);
>>>>>>> refs/remotes/origin/master

	platform_set_drvdata(pdev, info);
	return 0;
}

<<<<<<< HEAD
static int __devexit pxa2xx_flash_remove(struct platform_device *dev)
{
	struct pxa2xx_flash_info *info = platform_get_drvdata(dev);

	platform_set_drvdata(dev, NULL);

=======
static int pxa2xx_flash_remove(struct platform_device *dev)
{
	struct pxa2xx_flash_info *info = platform_get_drvdata(dev);

>>>>>>> refs/remotes/origin/master
	mtd_device_unregister(info->mtd);

	map_destroy(info->mtd);
	iounmap(info->map.virt);
	if (info->map.cached)
		iounmap(info->map.cached);
<<<<<<< HEAD
<<<<<<< HEAD
	kfree(info->parts);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	kfree(info);
	return 0;
}

#ifdef CONFIG_PM
static void pxa2xx_flash_shutdown(struct platform_device *dev)
{
	struct pxa2xx_flash_info *info = platform_get_drvdata(dev);

<<<<<<< HEAD
<<<<<<< HEAD
	if (info && info->mtd->suspend(info->mtd) == 0)
		info->mtd->resume(info->mtd);
=======
	if (info && mtd_suspend(info->mtd) == 0)
		mtd_resume(info->mtd);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (info && mtd_suspend(info->mtd) == 0)
		mtd_resume(info->mtd);
>>>>>>> refs/remotes/origin/master
}
#else
#define pxa2xx_flash_shutdown NULL
#endif

static struct platform_driver pxa2xx_flash_driver = {
	.driver = {
		.name		= "pxa2xx-flash",
		.owner		= THIS_MODULE,
	},
	.probe		= pxa2xx_flash_probe,
<<<<<<< HEAD
	.remove		= __devexit_p(pxa2xx_flash_remove),
	.shutdown	= pxa2xx_flash_shutdown,
};

<<<<<<< HEAD
static int __init init_pxa2xx_flash(void)
{
	return platform_driver_register(&pxa2xx_flash_driver);
}

static void __exit cleanup_pxa2xx_flash(void)
{
	platform_driver_unregister(&pxa2xx_flash_driver);
}

module_init(init_pxa2xx_flash);
module_exit(cleanup_pxa2xx_flash);
=======
module_platform_driver(pxa2xx_flash_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.remove		= pxa2xx_flash_remove,
	.shutdown	= pxa2xx_flash_shutdown,
};

module_platform_driver(pxa2xx_flash_driver);
>>>>>>> refs/remotes/origin/master

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nicolas Pitre <nico@fluxnic.net>");
MODULE_DESCRIPTION("MTD map driver for Intel XScale PXA2xx");
