/*******************************************************************************
  This contains the functions to handle the platform driver.

  Copyright (C) 2007-2011  STMicroelectronics Ltd

  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.

  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.

  The full GNU General Public License is included in this distribution in
  the file called "COPYING".

  Author: Giuseppe Cavallaro <peppe.cavallaro@st.com>
*******************************************************************************/

#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_net.h>
#include "stmmac.h"

#ifdef CONFIG_OF
<<<<<<< HEAD
static int __devinit stmmac_probe_config_dt(struct platform_device *pdev,
					    struct plat_stmmacenet_data *plat,
					    const char **mac)
{
	struct device_node *np = pdev->dev.of_node;
=======
static int stmmac_probe_config_dt(struct platform_device *pdev,
				  struct plat_stmmacenet_data *plat,
				  const char **mac)
{
	struct device_node *np = pdev->dev.of_node;
	struct stmmac_dma_cfg *dma_cfg;
>>>>>>> refs/remotes/origin/master

	if (!np)
		return -ENODEV;

	*mac = of_get_mac_address(np);
	plat->interface = of_get_phy_mode(np);
<<<<<<< HEAD
=======

	plat->bus_id = of_alias_get_id(np, "ethernet");
	if (plat->bus_id < 0)
		plat->bus_id = 0;

	of_property_read_u32(np, "snps,phy-addr", &plat->phy_addr);

>>>>>>> refs/remotes/origin/master
	plat->mdio_bus_data = devm_kzalloc(&pdev->dev,
					   sizeof(struct stmmac_mdio_bus_data),
					   GFP_KERNEL);

	/*
	 * Currently only the properties needed on SPEAr600
	 * are provided. All other properties should be added
	 * once needed on other platforms.
	 */
<<<<<<< HEAD
	if (of_device_is_compatible(np, "st,spear600-gmac")) {
		plat->pbl = 8;
=======
	if (of_device_is_compatible(np, "st,spear600-gmac") ||
		of_device_is_compatible(np, "snps,dwmac-3.70a") ||
		of_device_is_compatible(np, "snps,dwmac")) {
>>>>>>> refs/remotes/origin/master
		plat->has_gmac = 1;
		plat->pmt = 1;
	}

<<<<<<< HEAD
	return 0;
}
#else
static int __devinit stmmac_probe_config_dt(struct platform_device *pdev,
					    struct plat_stmmacenet_data *plat,
					    const char **mac)
=======
	if (of_device_is_compatible(np, "snps,dwmac-3.610") ||
		of_device_is_compatible(np, "snps,dwmac-3.710")) {
		plat->enh_desc = 1;
		plat->bugged_jumbo = 1;
		plat->force_sf_dma_mode = 1;
	}

	if (of_find_property(np, "snps,pbl", NULL)) {
		dma_cfg = devm_kzalloc(&pdev->dev, sizeof(*dma_cfg),
				       GFP_KERNEL);
		if (!dma_cfg)
			return -ENOMEM;
		plat->dma_cfg = dma_cfg;
		of_property_read_u32(np, "snps,pbl", &dma_cfg->pbl);
		dma_cfg->fixed_burst =
			of_property_read_bool(np, "snps,fixed-burst");
		dma_cfg->mixed_burst =
			of_property_read_bool(np, "snps,mixed-burst");
	}
	plat->force_thresh_dma_mode = of_property_read_bool(np, "snps,force_thresh_dma_mode");
	if (plat->force_thresh_dma_mode) {
		plat->force_sf_dma_mode = 0;
		pr_warn("force_sf_dma_mode is ignored if force_thresh_dma_mode is set.");
	}

	return 0;
}
#else
static int stmmac_probe_config_dt(struct platform_device *pdev,
				  struct plat_stmmacenet_data *plat,
				  const char **mac)
>>>>>>> refs/remotes/origin/master
{
	return -ENOSYS;
}
#endif /* CONFIG_OF */

/**
 * stmmac_pltfr_probe
 * @pdev: platform device pointer
 * Description: platform_device probe function. It allocates
 * the necessary resources and invokes the main to init
 * the net device, register the mdio bus etc.
 */
static int stmmac_pltfr_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct resource *res;
<<<<<<< HEAD
=======
	struct device *dev = &pdev->dev;
>>>>>>> refs/remotes/origin/master
	void __iomem *addr = NULL;
	struct stmmac_priv *priv = NULL;
	struct plat_stmmacenet_data *plat_dat = NULL;
	const char *mac = NULL;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
<<<<<<< HEAD
	if (!res)
		return -ENODEV;

	if (!request_mem_region(res->start, resource_size(res), pdev->name)) {
		pr_err("%s: ERROR: memory allocation failed"
		       "cannot get the I/O addr 0x%x\n",
		       __func__, (unsigned int)res->start);
		return -EBUSY;
	}

	addr = ioremap(res->start, resource_size(res));
	if (!addr) {
		pr_err("%s: ERROR: memory mapping failed", __func__);
		ret = -ENOMEM;
		goto out_release_region;
	}

	if (pdev->dev.of_node) {
		plat_dat = devm_kzalloc(&pdev->dev,
=======
	addr = devm_ioremap_resource(dev, res);
	if (IS_ERR(addr))
		return PTR_ERR(addr);

	plat_dat = dev_get_platdata(&pdev->dev);
	if (pdev->dev.of_node) {
		if (!plat_dat)
			plat_dat = devm_kzalloc(&pdev->dev,
>>>>>>> refs/remotes/origin/master
					sizeof(struct plat_stmmacenet_data),
					GFP_KERNEL);
		if (!plat_dat) {
			pr_err("%s: ERROR: no memory", __func__);
<<<<<<< HEAD
			ret = -ENOMEM;
			goto out_unmap;
=======
			return  -ENOMEM;
>>>>>>> refs/remotes/origin/master
		}

		ret = stmmac_probe_config_dt(pdev, plat_dat, &mac);
		if (ret) {
			pr_err("%s: main dt probe failed", __func__);
<<<<<<< HEAD
			goto out_unmap;
		}
	} else {
		plat_dat = pdev->dev.platform_data;
=======
			return ret;
		}
>>>>>>> refs/remotes/origin/master
	}

	/* Custom initialisation (if needed)*/
	if (plat_dat->init) {
		ret = plat_dat->init(pdev);
		if (unlikely(ret))
<<<<<<< HEAD
			goto out_unmap;
=======
			return ret;
>>>>>>> refs/remotes/origin/master
	}

	priv = stmmac_dvr_probe(&(pdev->dev), plat_dat, addr);
	if (!priv) {
		pr_err("%s: main driver probe failed", __func__);
<<<<<<< HEAD
		goto out_unmap;
=======
		return -ENODEV;
>>>>>>> refs/remotes/origin/master
	}

	/* Get MAC address if available (DT) */
	if (mac)
		memcpy(priv->dev->dev_addr, mac, ETH_ALEN);

	/* Get the MAC information */
	priv->dev->irq = platform_get_irq_byname(pdev, "macirq");
	if (priv->dev->irq == -ENXIO) {
		pr_err("%s: ERROR: MAC IRQ configuration "
		       "information not found\n", __func__);
<<<<<<< HEAD
		ret = -ENXIO;
		goto out_unmap;
=======
		return -ENXIO;
>>>>>>> refs/remotes/origin/master
	}

	/*
	 * On some platforms e.g. SPEAr the wake up irq differs from the mac irq
	 * The external wake up irq can be passed through the platform code
	 * named as "eth_wake_irq"
	 *
	 * In case the wake up interrupt is not passed from the platform
	 * so the driver will continue to use the mac irq (ndev->irq)
	 */
	priv->wol_irq = platform_get_irq_byname(pdev, "eth_wake_irq");
	if (priv->wol_irq == -ENXIO)
		priv->wol_irq = priv->dev->irq;

<<<<<<< HEAD
=======
	priv->lpi_irq = platform_get_irq_byname(pdev, "eth_lpi");

>>>>>>> refs/remotes/origin/master
	platform_set_drvdata(pdev, priv->dev);

	pr_debug("STMMAC platform driver registration completed");

	return 0;
<<<<<<< HEAD

out_unmap:
	iounmap(addr);
	platform_set_drvdata(pdev, NULL);

out_release_region:
	release_mem_region(res->start, resource_size(res));

	return ret;
=======
>>>>>>> refs/remotes/origin/master
}

/**
 * stmmac_pltfr_remove
 * @pdev: platform device pointer
 * Description: this function calls the main to free the net resources
 * and calls the platforms hook and release the resources (e.g. mem).
 */
static int stmmac_pltfr_remove(struct platform_device *pdev)
{
	struct net_device *ndev = platform_get_drvdata(pdev);
	struct stmmac_priv *priv = netdev_priv(ndev);
<<<<<<< HEAD
	struct resource *res;
=======
>>>>>>> refs/remotes/origin/master
	int ret = stmmac_dvr_remove(ndev);

	if (priv->plat->exit)
		priv->plat->exit(pdev);

<<<<<<< HEAD
	if (priv->plat->exit)
		priv->plat->exit(pdev);

	platform_set_drvdata(pdev, NULL);

	iounmap((void *)priv->ioaddr);
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	release_mem_region(res->start, resource_size(res));

=======
>>>>>>> refs/remotes/origin/master
	return ret;
}

#ifdef CONFIG_PM
static int stmmac_pltfr_suspend(struct device *dev)
{
	struct net_device *ndev = dev_get_drvdata(dev);

	return stmmac_suspend(ndev);
}

static int stmmac_pltfr_resume(struct device *dev)
{
	struct net_device *ndev = dev_get_drvdata(dev);

	return stmmac_resume(ndev);
}

int stmmac_pltfr_freeze(struct device *dev)
{
<<<<<<< HEAD
	struct net_device *ndev = dev_get_drvdata(dev);

	return stmmac_freeze(ndev);
=======
	int ret;
	struct plat_stmmacenet_data *plat_dat = dev_get_platdata(dev);
	struct net_device *ndev = dev_get_drvdata(dev);
	struct platform_device *pdev = to_platform_device(dev);

	ret = stmmac_freeze(ndev);
	if (plat_dat->exit)
		plat_dat->exit(pdev);

	return ret;
>>>>>>> refs/remotes/origin/master
}

int stmmac_pltfr_restore(struct device *dev)
{
<<<<<<< HEAD
	struct net_device *ndev = dev_get_drvdata(dev);
=======
	struct plat_stmmacenet_data *plat_dat = dev_get_platdata(dev);
	struct net_device *ndev = dev_get_drvdata(dev);
	struct platform_device *pdev = to_platform_device(dev);

	if (plat_dat->init)
		plat_dat->init(pdev);
>>>>>>> refs/remotes/origin/master

	return stmmac_restore(ndev);
}

static const struct dev_pm_ops stmmac_pltfr_pm_ops = {
	.suspend = stmmac_pltfr_suspend,
	.resume = stmmac_pltfr_resume,
	.freeze = stmmac_pltfr_freeze,
	.thaw = stmmac_pltfr_restore,
	.restore = stmmac_pltfr_restore,
};
#else
static const struct dev_pm_ops stmmac_pltfr_pm_ops;
#endif /* CONFIG_PM */

static const struct of_device_id stmmac_dt_ids[] = {
<<<<<<< HEAD
	{ .compatible = "st,spear600-gmac", },
=======
	{ .compatible = "st,spear600-gmac"},
	{ .compatible = "snps,dwmac-3.610"},
	{ .compatible = "snps,dwmac-3.70a"},
	{ .compatible = "snps,dwmac-3.710"},
	{ .compatible = "snps,dwmac"},
>>>>>>> refs/remotes/origin/master
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, stmmac_dt_ids);

<<<<<<< HEAD
static struct platform_driver stmmac_driver = {
=======
struct platform_driver stmmac_pltfr_driver = {
>>>>>>> refs/remotes/origin/master
	.probe = stmmac_pltfr_probe,
	.remove = stmmac_pltfr_remove,
	.driver = {
		   .name = STMMAC_RESOURCE_NAME,
		   .owner = THIS_MODULE,
		   .pm = &stmmac_pltfr_pm_ops,
		   .of_match_table = of_match_ptr(stmmac_dt_ids),
		   },
};

<<<<<<< HEAD
module_platform_driver(stmmac_driver);

=======
>>>>>>> refs/remotes/origin/master
MODULE_DESCRIPTION("STMMAC 10/100/1000 Ethernet PLATFORM driver");
MODULE_AUTHOR("Giuseppe Cavallaro <peppe.cavallaro@st.com>");
MODULE_LICENSE("GPL");
