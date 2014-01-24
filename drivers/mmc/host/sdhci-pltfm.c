/*
 * sdhci-pltfm.c Support for SDHCI platform devices
 * Copyright (c) 2009 Intel Corporation
 *
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
 * Copyright (c) 2007, 2011 Freescale Semiconductor, Inc.
 * Copyright (c) 2009 MontaVista Software, Inc.
 *
 * Authors: Xiaobo Xie <X.Xie@freescale.com>
 *	    Anton Vorontsov <avorontsov@ru.mvista.com>
 *
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* Supports:
 * SDHCI platform devices
 *
 * Inspired by sdhci-pci.c, by Pierre Ossman
 */

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/delay.h>
#include <linux/highmem.h>
#include <linux/mod_devicetable.h>
#include <linux/platform_device.h>

#include <linux/mmc/host.h>

#include <linux/io.h>
#include <linux/mmc/sdhci-pltfm.h>

#include "sdhci.h"
#include "sdhci-pltfm.h"

/*****************************************************************************\
 *                                                                           *
 * SDHCI core callbacks                                                      *
 *                                                                           *
\*****************************************************************************/

static struct sdhci_ops sdhci_pltfm_ops = {
};

/*****************************************************************************\
 *                                                                           *
 * Device probing/removal                                                    *
 *                                                                           *
\*****************************************************************************/

static int __devinit sdhci_pltfm_probe(struct platform_device *pdev)
{
	const struct platform_device_id *platid = platform_get_device_id(pdev);
	struct sdhci_pltfm_data *pdata;
	struct sdhci_host *host;
	struct sdhci_pltfm_host *pltfm_host;
	struct resource *iomem;
	int ret;

	if (platid && platid->driver_data)
		pdata = (void *)platid->driver_data;
	else
		pdata = pdev->dev.platform_data;

=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/err.h>
#include <linux/module.h>
#include <linux/of.h>
#ifdef CONFIG_PPC
#include <asm/machdep.h>
#endif
#include "sdhci-pltfm.h"

<<<<<<< HEAD
static struct sdhci_ops sdhci_pltfm_ops = {
=======
unsigned int sdhci_pltfm_clk_get_max_clock(struct sdhci_host *host)
{
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);

	return clk_get_rate(pltfm_host->clk);
}
EXPORT_SYMBOL_GPL(sdhci_pltfm_clk_get_max_clock);

static const struct sdhci_ops sdhci_pltfm_ops = {
>>>>>>> refs/remotes/origin/master
};

#ifdef CONFIG_OF
static bool sdhci_of_wp_inverted(struct device_node *np)
{
<<<<<<< HEAD
	if (of_get_property(np, "sdhci,wp-inverted", NULL))
=======
	if (of_get_property(np, "sdhci,wp-inverted", NULL) ||
	    of_get_property(np, "wp-inverted", NULL))
>>>>>>> refs/remotes/origin/master
		return true;

	/* Old device trees don't have the wp-inverted property. */
#ifdef CONFIG_PPC
	return machine_is(mpc837x_rdb) || machine_is(mpc837x_mds);
#else
	return false;
#endif /* CONFIG_PPC */
}

void sdhci_get_of_property(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct sdhci_host *host = platform_get_drvdata(pdev);
	struct sdhci_pltfm_host *pltfm_host = sdhci_priv(host);
	const __be32 *clk;
<<<<<<< HEAD
=======
	u32 bus_width;
>>>>>>> refs/remotes/origin/master
	int size;

	if (of_device_is_available(np)) {
		if (of_get_property(np, "sdhci,auto-cmd12", NULL))
			host->quirks |= SDHCI_QUIRK_MULTIBLOCK_READ_ACMD12;

<<<<<<< HEAD
		if (of_get_property(np, "sdhci,1-bit-only", NULL))
=======
		if (of_get_property(np, "sdhci,1-bit-only", NULL) ||
		    (of_property_read_u32(np, "bus-width", &bus_width) == 0 &&
		    bus_width == 1))
>>>>>>> refs/remotes/origin/master
			host->quirks |= SDHCI_QUIRK_FORCE_1_BIT_DATA;

		if (sdhci_of_wp_inverted(np))
			host->quirks |= SDHCI_QUIRK_INVERTED_WRITE_PROTECT;

<<<<<<< HEAD
=======
		if (of_get_property(np, "broken-cd", NULL))
			host->quirks |= SDHCI_QUIRK_BROKEN_CARD_DETECTION;

		if (of_get_property(np, "no-1-8-v", NULL))
			host->quirks2 |= SDHCI_QUIRK2_NO_1_8_V;

>>>>>>> refs/remotes/origin/master
		if (of_device_is_compatible(np, "fsl,p2020-rev1-esdhc"))
			host->quirks |= SDHCI_QUIRK_BROKEN_DMA;

		if (of_device_is_compatible(np, "fsl,p2020-esdhc") ||
		    of_device_is_compatible(np, "fsl,p1010-esdhc") ||
<<<<<<< HEAD
=======
		    of_device_is_compatible(np, "fsl,t4240-esdhc") ||
>>>>>>> refs/remotes/origin/master
		    of_device_is_compatible(np, "fsl,mpc8536-esdhc"))
			host->quirks |= SDHCI_QUIRK_BROKEN_TIMEOUT_VAL;

		clk = of_get_property(np, "clock-frequency", &size);
		if (clk && size == sizeof(*clk) && *clk)
			pltfm_host->clock = be32_to_cpup(clk);
<<<<<<< HEAD
=======

		if (of_find_property(np, "keep-power-in-suspend", NULL))
			host->mmc->pm_caps |= MMC_PM_KEEP_POWER;

		if (of_find_property(np, "enable-sdio-wakeup", NULL))
			host->mmc->pm_caps |= MMC_PM_WAKE_SDIO_IRQ;
>>>>>>> refs/remotes/origin/master
	}
}
#else
void sdhci_get_of_property(struct platform_device *pdev) {}
#endif /* CONFIG_OF */
EXPORT_SYMBOL_GPL(sdhci_get_of_property);

struct sdhci_host *sdhci_pltfm_init(struct platform_device *pdev,
<<<<<<< HEAD
				    struct sdhci_pltfm_data *pdata)
{
	struct sdhci_host *host;
	struct sdhci_pltfm_host *pltfm_host;
=======
				    const struct sdhci_pltfm_data *pdata,
				    size_t priv_size)
{
	struct sdhci_host *host;
>>>>>>> refs/remotes/origin/master
	struct device_node *np = pdev->dev.of_node;
	struct resource *iomem;
	int ret;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	iomem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!iomem) {
		ret = -ENOMEM;
		goto err;
	}

	if (resource_size(iomem) < 0x100)
<<<<<<< HEAD
<<<<<<< HEAD
		dev_err(&pdev->dev, "Invalid iomem size. You may "
			"experience problems.\n");

	/* Some PCI-based MFD need the parent here */
	if (pdev->dev.parent != &platform_bus)
=======
=======
>>>>>>> refs/remotes/origin/master
		dev_err(&pdev->dev, "Invalid iomem size!\n");

	/* Some PCI-based MFD need the parent here */
	if (pdev->dev.parent != &platform_bus && !np)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		host = sdhci_alloc_host(pdev->dev.parent, sizeof(*pltfm_host));
	else
		host = sdhci_alloc_host(&pdev->dev, sizeof(*pltfm_host));
=======
		host = sdhci_alloc_host(pdev->dev.parent,
			sizeof(struct sdhci_pltfm_host) + priv_size);
	else
		host = sdhci_alloc_host(&pdev->dev,
			sizeof(struct sdhci_pltfm_host) + priv_size);
>>>>>>> refs/remotes/origin/master

	if (IS_ERR(host)) {
		ret = PTR_ERR(host);
		goto err;
	}

<<<<<<< HEAD
	pltfm_host = sdhci_priv(host);

<<<<<<< HEAD
	host->hw_name = "platform";
=======
	host->hw_name = dev_name(&pdev->dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	host->hw_name = dev_name(&pdev->dev);
>>>>>>> refs/remotes/origin/master
	if (pdata && pdata->ops)
		host->ops = pdata->ops;
	else
		host->ops = &sdhci_pltfm_ops;
<<<<<<< HEAD
	if (pdata)
		host->quirks = pdata->quirks;
=======
	if (pdata) {
		host->quirks = pdata->quirks;
		host->quirks2 = pdata->quirks2;
	}

>>>>>>> refs/remotes/origin/master
	host->irq = platform_get_irq(pdev, 0);

	if (!request_mem_region(iomem->start, resource_size(iomem),
		mmc_hostname(host->mmc))) {
		dev_err(&pdev->dev, "cannot request region\n");
		ret = -EBUSY;
		goto err_request;
	}

	host->ioaddr = ioremap(iomem->start, resource_size(iomem));
	if (!host->ioaddr) {
		dev_err(&pdev->dev, "failed to remap registers\n");
		ret = -ENOMEM;
		goto err_remap;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	if (pdata && pdata->init) {
		ret = pdata->init(host, pdata);
		if (ret)
			goto err_plat_init;
	}

	ret = sdhci_add_host(host);
	if (ret)
		goto err_add_host;

	platform_set_drvdata(pdev, host);

	return 0;

err_add_host:
	if (pdata && pdata->exit)
		pdata->exit(host);
err_plat_init:
	iounmap(host->ioaddr);
=======
=======
	/*
	 * Some platforms need to probe the controller to be able to
	 * determine which caps should be used.
	 */
	if (host->ops && host->ops->platform_init)
		host->ops->platform_init(host);

>>>>>>> refs/remotes/origin/master
	platform_set_drvdata(pdev, host);

	return host;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
err_remap:
	release_mem_region(iomem->start, resource_size(iomem));
err_request:
	sdhci_free_host(host);
err:
<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_ERR"Probing of sdhci-pltfm failed: %d\n", ret);
	return ret;
}

static int __devexit sdhci_pltfm_remove(struct platform_device *pdev)
{
	struct sdhci_pltfm_data *pdata = pdev->dev.platform_data;
	struct sdhci_host *host = platform_get_drvdata(pdev);
	struct resource *iomem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	int dead;
	u32 scratch;

	dead = 0;
	scratch = readl(host->ioaddr + SDHCI_INT_STATUS);
	if (scratch == (u32)-1)
		dead = 1;

	sdhci_remove_host(host, dead);
	if (pdata && pdata->exit)
		pdata->exit(host);
=======
=======
>>>>>>> refs/remotes/origin/master
	dev_err(&pdev->dev, "%s failed %d\n", __func__, ret);
	return ERR_PTR(ret);
}
EXPORT_SYMBOL_GPL(sdhci_pltfm_init);

void sdhci_pltfm_free(struct platform_device *pdev)
{
	struct sdhci_host *host = platform_get_drvdata(pdev);
	struct resource *iomem = platform_get_resource(pdev, IORESOURCE_MEM, 0);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	iounmap(host->ioaddr);
	release_mem_region(iomem->start, resource_size(iomem));
	sdhci_free_host(host);
	platform_set_drvdata(pdev, NULL);
<<<<<<< HEAD

	return 0;
}

static const struct platform_device_id sdhci_pltfm_ids[] = {
	{ "sdhci", },
#ifdef CONFIG_MMC_SDHCI_CNS3XXX
	{ "sdhci-cns3xxx", (kernel_ulong_t)&sdhci_cns3xxx_pdata },
#endif
#ifdef CONFIG_MMC_SDHCI_ESDHC_IMX
	{ "sdhci-esdhc-imx", (kernel_ulong_t)&sdhci_esdhc_imx_pdata },
#endif
#ifdef CONFIG_MMC_SDHCI_DOVE
	{ "sdhci-dove", (kernel_ulong_t)&sdhci_dove_pdata },
#endif
#ifdef CONFIG_MMC_SDHCI_TEGRA
	{ "sdhci-tegra", (kernel_ulong_t)&sdhci_tegra_pdata },
#endif
	{ },
};
MODULE_DEVICE_TABLE(platform, sdhci_pltfm_ids);

#ifdef CONFIG_PM
static int sdhci_pltfm_suspend(struct platform_device *dev, pm_message_t state)
{
	struct sdhci_host *host = platform_get_drvdata(dev);

	return sdhci_suspend_host(host, state);
}

static int sdhci_pltfm_resume(struct platform_device *dev)
{
	struct sdhci_host *host = platform_get_drvdata(dev);

	return sdhci_resume_host(host);
}
#else
#define sdhci_pltfm_suspend	NULL
#define sdhci_pltfm_resume	NULL
#endif	/* CONFIG_PM */

static struct platform_driver sdhci_pltfm_driver = {
	.driver = {
		.name	= "sdhci",
		.owner	= THIS_MODULE,
	},
	.probe		= sdhci_pltfm_probe,
	.remove		= __devexit_p(sdhci_pltfm_remove),
	.id_table	= sdhci_pltfm_ids,
	.suspend	= sdhci_pltfm_suspend,
	.resume		= sdhci_pltfm_resume,
};

/*****************************************************************************\
 *                                                                           *
 * Driver init/exit                                                          *
 *                                                                           *
\*****************************************************************************/

static int __init sdhci_drv_init(void)
{
	return platform_driver_register(&sdhci_pltfm_driver);
}

static void __exit sdhci_drv_exit(void)
{
	platform_driver_unregister(&sdhci_pltfm_driver);
}

module_init(sdhci_drv_init);
module_exit(sdhci_drv_exit);

MODULE_DESCRIPTION("Secure Digital Host Controller Interface platform driver");
MODULE_AUTHOR("Mocean Laboratories <info@mocean-labs.com>");
=======
=======
	iounmap(host->ioaddr);
	release_mem_region(iomem->start, resource_size(iomem));
	sdhci_free_host(host);
>>>>>>> refs/remotes/origin/master
}
EXPORT_SYMBOL_GPL(sdhci_pltfm_free);

int sdhci_pltfm_register(struct platform_device *pdev,
<<<<<<< HEAD
			 struct sdhci_pltfm_data *pdata)
=======
			const struct sdhci_pltfm_data *pdata,
			size_t priv_size)
>>>>>>> refs/remotes/origin/master
{
	struct sdhci_host *host;
	int ret = 0;

<<<<<<< HEAD
	host = sdhci_pltfm_init(pdev, pdata);
=======
	host = sdhci_pltfm_init(pdev, pdata, priv_size);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(host))
		return PTR_ERR(host);

	sdhci_get_of_property(pdev);

	ret = sdhci_add_host(host);
	if (ret)
		sdhci_pltfm_free(pdev);

	return ret;
}
EXPORT_SYMBOL_GPL(sdhci_pltfm_register);

int sdhci_pltfm_unregister(struct platform_device *pdev)
{
	struct sdhci_host *host = platform_get_drvdata(pdev);
	int dead = (readl(host->ioaddr + SDHCI_INT_STATUS) == 0xffffffff);

	sdhci_remove_host(host, dead);
	sdhci_pltfm_free(pdev);

	return 0;
}
EXPORT_SYMBOL_GPL(sdhci_pltfm_unregister);

#ifdef CONFIG_PM
static int sdhci_pltfm_suspend(struct device *dev)
{
	struct sdhci_host *host = dev_get_drvdata(dev);

	return sdhci_suspend_host(host);
}

static int sdhci_pltfm_resume(struct device *dev)
{
	struct sdhci_host *host = dev_get_drvdata(dev);

	return sdhci_resume_host(host);
}

const struct dev_pm_ops sdhci_pltfm_pmops = {
	.suspend	= sdhci_pltfm_suspend,
	.resume		= sdhci_pltfm_resume,
};
EXPORT_SYMBOL_GPL(sdhci_pltfm_pmops);
#endif	/* CONFIG_PM */

static int __init sdhci_pltfm_drv_init(void)
{
	pr_info("sdhci-pltfm: SDHCI platform and OF driver helper\n");

	return 0;
}
module_init(sdhci_pltfm_drv_init);

static void __exit sdhci_pltfm_drv_exit(void)
{
}
module_exit(sdhci_pltfm_drv_exit);

MODULE_DESCRIPTION("SDHCI platform and OF driver helper");
MODULE_AUTHOR("Intel Corporation");
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
MODULE_LICENSE("GPL v2");
