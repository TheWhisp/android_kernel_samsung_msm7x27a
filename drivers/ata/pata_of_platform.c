/*
 * OF-platform PATA driver
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 *                     Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (Version 2) as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/module.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/of_platform.h>
=======
#include <linux/of_address.h>
#include <linux/platform_device.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/ata_platform.h>

static int __devinit pata_of_platform_probe(struct platform_device *ofdev)
=======
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/ata_platform.h>
#include <linux/libata.h>

static int pata_of_platform_probe(struct platform_device *ofdev)
>>>>>>> refs/remotes/origin/master
{
	int ret;
	struct device_node *dn = ofdev->dev.of_node;
	struct resource io_res;
	struct resource ctl_res;
<<<<<<< HEAD
<<<<<<< HEAD
	struct resource irq_res;
=======
	struct resource *irq_res;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct resource *irq_res;
>>>>>>> refs/remotes/origin/master
	unsigned int reg_shift = 0;
	int pio_mode = 0;
	int pio_mask;
	const u32 *prop;

	ret = of_address_to_resource(dn, 0, &io_res);
	if (ret) {
		dev_err(&ofdev->dev, "can't get IO address from "
			"device tree\n");
		return -EINVAL;
	}

	if (of_device_is_compatible(dn, "electra-ide")) {
		/* Altstatus is really at offset 0x3f6 from the primary window
		 * on electra-ide. Adjust ctl_res and io_res accordingly.
		 */
		ctl_res = io_res;
		ctl_res.start = ctl_res.start+0x3f6;
		io_res.end = ctl_res.start-1;
	} else {
		ret = of_address_to_resource(dn, 1, &ctl_res);
		if (ret) {
			dev_err(&ofdev->dev, "can't get CTL address from "
				"device tree\n");
			return -EINVAL;
		}
	}

<<<<<<< HEAD
<<<<<<< HEAD
	ret = of_irq_to_resource(dn, 0, &irq_res);
	if (ret == NO_IRQ)
		irq_res.start = irq_res.end = 0;
	else
		irq_res.flags = 0;

	prop = of_get_property(dn, "reg-shift", NULL);
	if (prop)
		reg_shift = *prop;

	prop = of_get_property(dn, "pio-mode", NULL);
	if (prop) {
		pio_mode = *prop;
=======
=======
>>>>>>> refs/remotes/origin/master
	irq_res = platform_get_resource(ofdev, IORESOURCE_IRQ, 0);
	if (irq_res)
		irq_res->flags = 0;

	prop = of_get_property(dn, "reg-shift", NULL);
	if (prop)
		reg_shift = be32_to_cpup(prop);

	prop = of_get_property(dn, "pio-mode", NULL);
	if (prop) {
		pio_mode = be32_to_cpup(prop);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
		if (pio_mode > 6) {
			dev_err(&ofdev->dev, "invalid pio-mode\n");
			return -EINVAL;
		}
	} else {
		dev_info(&ofdev->dev, "pio-mode unspecified, assuming PIO0\n");
	}

	pio_mask = 1 << pio_mode;
	pio_mask |= (1 << pio_mode) - 1;

<<<<<<< HEAD
<<<<<<< HEAD
	return __pata_platform_probe(&ofdev->dev, &io_res, &ctl_res, &irq_res,
=======
	return __pata_platform_probe(&ofdev->dev, &io_res, &ctl_res, irq_res,
>>>>>>> refs/remotes/origin/cm-10.0
				     reg_shift, pio_mask);
}

static int __devexit pata_of_platform_remove(struct platform_device *ofdev)
{
	return __pata_platform_remove(&ofdev->dev);
}

=======
	return __pata_platform_probe(&ofdev->dev, &io_res, &ctl_res, irq_res,
				     reg_shift, pio_mask);
}

>>>>>>> refs/remotes/origin/master
static struct of_device_id pata_of_platform_match[] = {
	{ .compatible = "ata-generic", },
	{ .compatible = "electra-ide", },
	{},
};
MODULE_DEVICE_TABLE(of, pata_of_platform_match);

static struct platform_driver pata_of_platform_driver = {
	.driver = {
		.name = "pata_of_platform",
		.owner = THIS_MODULE,
		.of_match_table = pata_of_platform_match,
	},
	.probe		= pata_of_platform_probe,
<<<<<<< HEAD
	.remove		= __devexit_p(pata_of_platform_remove),
};

<<<<<<< HEAD
static int __init pata_of_platform_init(void)
{
	return platform_driver_register(&pata_of_platform_driver);
}
module_init(pata_of_platform_init);

static void __exit pata_of_platform_exit(void)
{
	platform_driver_unregister(&pata_of_platform_driver);
}
module_exit(pata_of_platform_exit);
=======
module_platform_driver(pata_of_platform_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.remove		= ata_platform_remove_one,
};

module_platform_driver(pata_of_platform_driver);
>>>>>>> refs/remotes/origin/master

MODULE_DESCRIPTION("OF-platform PATA driver");
MODULE_AUTHOR("Anton Vorontsov <avorontsov@ru.mvista.com>");
MODULE_LICENSE("GPL");
