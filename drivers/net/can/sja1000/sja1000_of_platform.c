/*
 * Driver for SJA1000 CAN controllers on the OpenFirmware platform bus
 *
 * Copyright (C) 2008-2009 Wolfgang Grandegger <wg@grandegger.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the version 2 of the GNU General Public License
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/* This is a generic driver for SJA1000 chips on the OpenFirmware platform
 * bus found on embedded PowerPC systems. You need a SJA1000 CAN node
 * definition in your flattened device tree source (DTS) file similar to:
 *
 *   can@3,100 {
 *           compatible = "nxp,sja1000";
 *           reg = <3 0x100 0x80>;
 *           interrupts = <2 0>;
 *           interrupt-parent = <&mpic>;
 *           nxp,external-clock-frequency = <16000000>;
 *   };
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * See "Documentation/powerpc/dts-bindings/can/sja1000.txt" for further
=======
 * See "Documentation/devicetree/bindings/net/can/sja1000.txt" for further
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * See "Documentation/devicetree/bindings/net/can/sja1000.txt" for further
>>>>>>> refs/remotes/origin/master
 * information.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/netdevice.h>
#include <linux/delay.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/io.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/can/dev.h>

#include <linux/of_platform.h>
#include <asm/prom.h>
=======
#include <linux/io.h>
#include <linux/can/dev.h>

#include <linux/of_platform.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
>>>>>>> refs/remotes/origin/master

#include "sja1000.h"

#define DRV_NAME "sja1000_of_platform"

MODULE_AUTHOR("Wolfgang Grandegger <wg@grandegger.com>");
MODULE_DESCRIPTION("Socket-CAN driver for SJA1000 on the OF platform bus");
MODULE_LICENSE("GPL v2");

#define SJA1000_OFP_CAN_CLOCK  (16000000 / 2)

#define SJA1000_OFP_OCR        OCR_TX0_PULLDOWN
#define SJA1000_OFP_CDR        (CDR_CBP | CDR_CLK_OFF)

static u8 sja1000_ofp_read_reg(const struct sja1000_priv *priv, int reg)
{
<<<<<<< HEAD
	return in_8(priv->reg_base + reg);
=======
	return ioread8(priv->reg_base + reg);
>>>>>>> refs/remotes/origin/master
}

static void sja1000_ofp_write_reg(const struct sja1000_priv *priv,
				  int reg, u8 val)
{
<<<<<<< HEAD
	out_8(priv->reg_base + reg, val);
}

static int __devexit sja1000_ofp_remove(struct platform_device *ofdev)
{
	struct net_device *dev = dev_get_drvdata(&ofdev->dev);
=======
	iowrite8(val, priv->reg_base + reg);
}

static int sja1000_ofp_remove(struct platform_device *ofdev)
{
	struct net_device *dev = platform_get_drvdata(ofdev);
>>>>>>> refs/remotes/origin/master
	struct sja1000_priv *priv = netdev_priv(dev);
	struct device_node *np = ofdev->dev.of_node;
	struct resource res;

<<<<<<< HEAD
	dev_set_drvdata(&ofdev->dev, NULL);

=======
>>>>>>> refs/remotes/origin/master
	unregister_sja1000dev(dev);
	free_sja1000dev(dev);
	iounmap(priv->reg_base);
	irq_dispose_mapping(dev->irq);

	of_address_to_resource(np, 0, &res);
	release_mem_region(res.start, resource_size(&res));

	return 0;
}

<<<<<<< HEAD
static int __devinit sja1000_ofp_probe(struct platform_device *ofdev)
=======
static int sja1000_ofp_probe(struct platform_device *ofdev)
>>>>>>> refs/remotes/origin/master
{
	struct device_node *np = ofdev->dev.of_node;
	struct net_device *dev;
	struct sja1000_priv *priv;
	struct resource res;
<<<<<<< HEAD
<<<<<<< HEAD
	const u32 *prop;
	int err, irq, res_size, prop_size;
=======
	u32 prop;
	int err, irq, res_size;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	u32 prop;
	int err, irq, res_size;
>>>>>>> refs/remotes/origin/master
	void __iomem *base;

	err = of_address_to_resource(np, 0, &res);
	if (err) {
		dev_err(&ofdev->dev, "invalid address\n");
		return err;
	}

	res_size = resource_size(&res);

	if (!request_mem_region(res.start, res_size, DRV_NAME)) {
		dev_err(&ofdev->dev, "couldn't request %pR\n", &res);
		return -EBUSY;
	}

	base = ioremap_nocache(res.start, res_size);
	if (!base) {
		dev_err(&ofdev->dev, "couldn't ioremap %pR\n", &res);
		err = -ENOMEM;
		goto exit_release_mem;
	}

	irq = irq_of_parse_and_map(np, 0);
<<<<<<< HEAD
	if (irq == NO_IRQ) {
=======
	if (irq == 0) {
>>>>>>> refs/remotes/origin/master
		dev_err(&ofdev->dev, "no irq found\n");
		err = -ENODEV;
		goto exit_unmap_mem;
	}

	dev = alloc_sja1000dev(0);
	if (!dev) {
		err = -ENOMEM;
		goto exit_dispose_irq;
	}

	priv = netdev_priv(dev);

	priv->read_reg = sja1000_ofp_read_reg;
	priv->write_reg = sja1000_ofp_write_reg;

<<<<<<< HEAD
<<<<<<< HEAD
	prop = of_get_property(np, "nxp,external-clock-frequency", &prop_size);
	if (prop && (prop_size ==  sizeof(u32)))
		priv->can.clock.freq = *prop / 2;
	else
		priv->can.clock.freq = SJA1000_OFP_CAN_CLOCK; /* default */

	prop = of_get_property(np, "nxp,tx-output-mode", &prop_size);
	if (prop && (prop_size == sizeof(u32)))
		priv->ocr |= *prop & OCR_MODE_MASK;
	else
		priv->ocr |= OCR_MODE_NORMAL; /* default */

	prop = of_get_property(np, "nxp,tx-output-config", &prop_size);
	if (prop && (prop_size == sizeof(u32)))
		priv->ocr |= (*prop << OCR_TX_SHIFT) & OCR_TX_MASK;
	else
		priv->ocr |= OCR_TX0_PULLDOWN; /* default */

	prop = of_get_property(np, "nxp,clock-out-frequency", &prop_size);
	if (prop && (prop_size == sizeof(u32)) && *prop) {
		u32 divider = priv->can.clock.freq * 2 / *prop;
=======
=======
>>>>>>> refs/remotes/origin/master
	err = of_property_read_u32(np, "nxp,external-clock-frequency", &prop);
	if (!err)
		priv->can.clock.freq = prop / 2;
	else
		priv->can.clock.freq = SJA1000_OFP_CAN_CLOCK; /* default */

	err = of_property_read_u32(np, "nxp,tx-output-mode", &prop);
	if (!err)
		priv->ocr |= prop & OCR_MODE_MASK;
	else
		priv->ocr |= OCR_MODE_NORMAL; /* default */

	err = of_property_read_u32(np, "nxp,tx-output-config", &prop);
	if (!err)
		priv->ocr |= (prop << OCR_TX_SHIFT) & OCR_TX_MASK;
	else
		priv->ocr |= OCR_TX0_PULLDOWN; /* default */

	err = of_property_read_u32(np, "nxp,clock-out-frequency", &prop);
	if (!err && prop) {
		u32 divider = priv->can.clock.freq * 2 / prop;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

		if (divider > 1)
			priv->cdr |= divider / 2 - 1;
		else
			priv->cdr |= CDR_CLKOUT_MASK;
	} else {
		priv->cdr |= CDR_CLK_OFF; /* default */
	}

<<<<<<< HEAD
<<<<<<< HEAD
	prop = of_get_property(np, "nxp,no-comparator-bypass", NULL);
	if (!prop)
=======
	if (!of_property_read_bool(np, "nxp,no-comparator-bypass"))
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (!of_property_read_bool(np, "nxp,no-comparator-bypass"))
>>>>>>> refs/remotes/origin/master
		priv->cdr |= CDR_CBP; /* default */

	priv->irq_flags = IRQF_SHARED;
	priv->reg_base = base;

	dev->irq = irq;

	dev_info(&ofdev->dev,
		 "reg_base=0x%p irq=%d clock=%d ocr=0x%02x cdr=0x%02x\n",
		 priv->reg_base, dev->irq, priv->can.clock.freq,
		 priv->ocr, priv->cdr);

<<<<<<< HEAD
	dev_set_drvdata(&ofdev->dev, dev);
=======
	platform_set_drvdata(ofdev, dev);
>>>>>>> refs/remotes/origin/master
	SET_NETDEV_DEV(dev, &ofdev->dev);

	err = register_sja1000dev(dev);
	if (err) {
		dev_err(&ofdev->dev, "registering %s failed (err=%d)\n",
			DRV_NAME, err);
		goto exit_free_sja1000;
	}

	return 0;

exit_free_sja1000:
	free_sja1000dev(dev);
exit_dispose_irq:
	irq_dispose_mapping(irq);
exit_unmap_mem:
	iounmap(base);
exit_release_mem:
	release_mem_region(res.start, res_size);

	return err;
}

<<<<<<< HEAD
static struct of_device_id __devinitdata sja1000_ofp_table[] = {
=======
static struct of_device_id sja1000_ofp_table[] = {
>>>>>>> refs/remotes/origin/master
	{.compatible = "nxp,sja1000"},
	{},
};
MODULE_DEVICE_TABLE(of, sja1000_ofp_table);

static struct platform_driver sja1000_ofp_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name = DRV_NAME,
		.of_match_table = sja1000_ofp_table,
	},
	.probe = sja1000_ofp_probe,
<<<<<<< HEAD
	.remove = __devexit_p(sja1000_ofp_remove),
};

<<<<<<< HEAD
static int __init sja1000_ofp_init(void)
{
	return platform_driver_register(&sja1000_ofp_driver);
}
module_init(sja1000_ofp_init);

static void __exit sja1000_ofp_exit(void)
{
	return platform_driver_unregister(&sja1000_ofp_driver);
};
module_exit(sja1000_ofp_exit);
=======
module_platform_driver(sja1000_ofp_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.remove = sja1000_ofp_remove,
};

module_platform_driver(sja1000_ofp_driver);
>>>>>>> refs/remotes/origin/master
