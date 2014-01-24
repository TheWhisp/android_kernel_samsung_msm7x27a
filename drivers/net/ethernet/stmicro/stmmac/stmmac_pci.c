/*******************************************************************************
  This contains the functions to handle the pci driver.

  Copyright (C) 2011-2012  Vayavya Labs Pvt Ltd

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

  Author: Rayagond Kokatanur <rayagond@vayavyalabs.com>
  Author: Giuseppe Cavallaro <peppe.cavallaro@st.com>
*******************************************************************************/

#include <linux/pci.h>
#include "stmmac.h"

struct plat_stmmacenet_data plat_dat;
struct stmmac_mdio_bus_data mdio_data;
<<<<<<< HEAD
=======
struct stmmac_dma_cfg dma_cfg;
>>>>>>> refs/remotes/origin/master

static void stmmac_default_data(void)
{
	memset(&plat_dat, 0, sizeof(struct plat_stmmacenet_data));
	plat_dat.bus_id = 1;
	plat_dat.phy_addr = 0;
	plat_dat.interface = PHY_INTERFACE_MODE_GMII;
<<<<<<< HEAD
	plat_dat.pbl = 32;
=======
>>>>>>> refs/remotes/origin/master
	plat_dat.clk_csr = 2;	/* clk_csr_i = 20-35MHz & MDC = clk_csr_i/16 */
	plat_dat.has_gmac = 1;
	plat_dat.force_sf_dma_mode = 1;

<<<<<<< HEAD
	mdio_data.bus_id = 1;
	mdio_data.phy_reset = NULL;
	mdio_data.phy_mask = 0;
	plat_dat.mdio_bus_data = &mdio_data;
=======
	mdio_data.phy_reset = NULL;
	mdio_data.phy_mask = 0;
	plat_dat.mdio_bus_data = &mdio_data;

	dma_cfg.pbl = 32;
	dma_cfg.burst_len = DMA_AXI_BLEN_256;
	plat_dat.dma_cfg = &dma_cfg;
>>>>>>> refs/remotes/origin/master
}

/**
 * stmmac_pci_probe
 *
 * @pdev: pci device pointer
 * @id: pointer to table of device id/id's.
 *
 * Description: This probing function gets called for all PCI devices which
 * match the ID table and are not "owned" by other driver yet. This function
 * gets passed a "struct pci_dev *" for each device whose entry in the ID table
 * matches the device. The probe functions returns zero when the driver choose
 * to take "ownership" of the device or an error code(-ve no) otherwise.
 */
<<<<<<< HEAD
static int __devinit stmmac_pci_probe(struct pci_dev *pdev,
				      const struct pci_device_id *id)
=======
static int stmmac_pci_probe(struct pci_dev *pdev,
			    const struct pci_device_id *id)
>>>>>>> refs/remotes/origin/master
{
	int ret = 0;
	void __iomem *addr = NULL;
	struct stmmac_priv *priv = NULL;
	int i;

	/* Enable pci device */
	ret = pci_enable_device(pdev);
	if (ret) {
		pr_err("%s : ERROR: failed to enable %s device\n", __func__,
		       pci_name(pdev));
		return ret;
	}
	if (pci_request_regions(pdev, STMMAC_RESOURCE_NAME)) {
		pr_err("%s: ERROR: failed to get PCI region\n", __func__);
		ret = -ENODEV;
		goto err_out_req_reg_failed;
	}

	/* Get the base address of device */
	for (i = 0; i <= 5; i++) {
		if (pci_resource_len(pdev, i) == 0)
			continue;
		addr = pci_iomap(pdev, i, 0);
		if (addr == NULL) {
<<<<<<< HEAD
			pr_err("%s: ERROR: cannot map register memory, aborting",
=======
			pr_err("%s: ERROR: cannot map register memory aborting",
>>>>>>> refs/remotes/origin/master
			       __func__);
			ret = -EIO;
			goto err_out_map_failed;
		}
		break;
	}
	pci_set_master(pdev);

	stmmac_default_data();

	priv = stmmac_dvr_probe(&(pdev->dev), &plat_dat, addr);
	if (!priv) {
		pr_err("%s: main driver probe failed", __func__);
<<<<<<< HEAD
=======
		ret = -ENODEV;
>>>>>>> refs/remotes/origin/master
		goto err_out;
	}
	priv->dev->irq = pdev->irq;
	priv->wol_irq = pdev->irq;

	pci_set_drvdata(pdev, priv->dev);

	pr_debug("STMMAC platform driver registration completed");

	return 0;

err_out:
	pci_clear_master(pdev);
err_out_map_failed:
	pci_release_regions(pdev);
err_out_req_reg_failed:
	pci_disable_device(pdev);

	return ret;
}

/**
<<<<<<< HEAD
 * stmmac_dvr_remove
=======
 * stmmac_pci_remove
>>>>>>> refs/remotes/origin/master
 *
 * @pdev: platform device pointer
 * Description: this function calls the main to free the net resources
 * and releases the PCI resources.
 */
<<<<<<< HEAD
static void __devexit stmmac_pci_remove(struct pci_dev *pdev)
=======
static void stmmac_pci_remove(struct pci_dev *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct net_device *ndev = pci_get_drvdata(pdev);
	struct stmmac_priv *priv = netdev_priv(ndev);

	stmmac_dvr_remove(ndev);

<<<<<<< HEAD
	pci_set_drvdata(pdev, NULL);
=======
>>>>>>> refs/remotes/origin/master
	pci_iounmap(pdev, priv->ioaddr);
	pci_release_regions(pdev);
	pci_disable_device(pdev);
}

#ifdef CONFIG_PM
static int stmmac_pci_suspend(struct pci_dev *pdev, pm_message_t state)
{
	struct net_device *ndev = pci_get_drvdata(pdev);
	int ret;

	ret = stmmac_suspend(ndev);
	pci_save_state(pdev);
	pci_set_power_state(pdev, pci_choose_state(pdev, state));

	return ret;
}

static int stmmac_pci_resume(struct pci_dev *pdev)
{
	struct net_device *ndev = pci_get_drvdata(pdev);

	pci_set_power_state(pdev, PCI_D0);
	pci_restore_state(pdev);

	return stmmac_resume(ndev);
}
#endif

#define STMMAC_VENDOR_ID 0x700
#define STMMAC_DEVICE_ID 0x1108

static DEFINE_PCI_DEVICE_TABLE(stmmac_id_table) = {
	{PCI_DEVICE(STMMAC_VENDOR_ID, STMMAC_DEVICE_ID)},
	{PCI_DEVICE(PCI_VENDOR_ID_STMICRO, PCI_DEVICE_ID_STMICRO_MAC)},
	{}
};

MODULE_DEVICE_TABLE(pci, stmmac_id_table);

<<<<<<< HEAD
static struct pci_driver stmmac_driver = {
	.name = STMMAC_RESOURCE_NAME,
	.id_table = stmmac_id_table,
	.probe = stmmac_pci_probe,
	.remove = __devexit_p(stmmac_pci_remove),
=======
struct pci_driver stmmac_pci_driver = {
	.name = STMMAC_RESOURCE_NAME,
	.id_table = stmmac_id_table,
	.probe = stmmac_pci_probe,
	.remove = stmmac_pci_remove,
>>>>>>> refs/remotes/origin/master
#ifdef CONFIG_PM
	.suspend = stmmac_pci_suspend,
	.resume = stmmac_pci_resume,
#endif
};

<<<<<<< HEAD
/**
 * stmmac_init_module - Entry point for the driver
 * Description: This function is the entry point for the driver.
 */
static int __init stmmac_init_module(void)
{
	int ret;

	ret = pci_register_driver(&stmmac_driver);
	if (ret < 0)
		pr_err("%s: ERROR: driver registration failed\n", __func__);

	return ret;
}

/**
 * stmmac_cleanup_module - Cleanup routine for the driver
 * Description: This function is the cleanup routine for the driver.
 */
static void __exit stmmac_cleanup_module(void)
{
	pci_unregister_driver(&stmmac_driver);
}

module_init(stmmac_init_module);
module_exit(stmmac_cleanup_module);

=======
>>>>>>> refs/remotes/origin/master
MODULE_DESCRIPTION("STMMAC 10/100/1000 Ethernet PCI driver");
MODULE_AUTHOR("Rayagond Kokatanur <rayagond.kokatanur@vayavyalabs.com>");
MODULE_AUTHOR("Giuseppe Cavallaro <peppe.cavallaro@st.com>");
MODULE_LICENSE("GPL");
