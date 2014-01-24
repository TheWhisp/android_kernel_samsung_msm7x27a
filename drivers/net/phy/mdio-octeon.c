/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
<<<<<<< HEAD
 * Copyright (C) 2009 Cavium Networks
 */

#include <linux/gfp.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/phy.h>
=======
 * Copyright (C) 2009-2012 Cavium, Inc.
 */

#include <linux/platform_device.h>
#include <linux/of_mdio.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/gfp.h>
#include <linux/phy.h>
#include <linux/io.h>
>>>>>>> refs/remotes/origin/master

#include <asm/octeon/octeon.h>
#include <asm/octeon/cvmx-smix-defs.h>

#define DRV_VERSION "1.0"
#define DRV_DESCRIPTION "Cavium Networks Octeon SMI/MDIO driver"

<<<<<<< HEAD
struct octeon_mdiobus {
	struct mii_bus *mii_bus;
	int unit;
	int phy_irq[PHY_MAX_ADDR];
};

=======
#define SMI_CMD		0x0
#define SMI_WR_DAT	0x8
#define SMI_RD_DAT	0x10
#define SMI_CLK		0x18
#define SMI_EN		0x20

enum octeon_mdiobus_mode {
	UNINIT = 0,
	C22,
	C45
};

struct octeon_mdiobus {
	struct mii_bus *mii_bus;
	u64 register_base;
	resource_size_t mdio_phys;
	resource_size_t regsize;
	enum octeon_mdiobus_mode mode;
	int phy_irq[PHY_MAX_ADDR];
};

static void octeon_mdiobus_set_mode(struct octeon_mdiobus *p,
				    enum octeon_mdiobus_mode m)
{
	union cvmx_smix_clk smi_clk;

	if (m == p->mode)
		return;

	smi_clk.u64 = cvmx_read_csr(p->register_base + SMI_CLK);
	smi_clk.s.mode = (m == C45) ? 1 : 0;
	smi_clk.s.preamble = 1;
	cvmx_write_csr(p->register_base + SMI_CLK, smi_clk.u64);
	p->mode = m;
}

static int octeon_mdiobus_c45_addr(struct octeon_mdiobus *p,
				   int phy_id, int regnum)
{
	union cvmx_smix_cmd smi_cmd;
	union cvmx_smix_wr_dat smi_wr;
	int timeout = 1000;

	octeon_mdiobus_set_mode(p, C45);

	smi_wr.u64 = 0;
	smi_wr.s.dat = regnum & 0xffff;
	cvmx_write_csr(p->register_base + SMI_WR_DAT, smi_wr.u64);

	regnum = (regnum >> 16) & 0x1f;

	smi_cmd.u64 = 0;
	smi_cmd.s.phy_op = 0; /* MDIO_CLAUSE_45_ADDRESS */
	smi_cmd.s.phy_adr = phy_id;
	smi_cmd.s.reg_adr = regnum;
	cvmx_write_csr(p->register_base + SMI_CMD, smi_cmd.u64);

	do {
		/* Wait 1000 clocks so we don't saturate the RSL bus
		 * doing reads.
		 */
		__delay(1000);
		smi_wr.u64 = cvmx_read_csr(p->register_base + SMI_WR_DAT);
	} while (smi_wr.s.pending && --timeout);

	if (timeout <= 0)
		return -EIO;
	return 0;
}

>>>>>>> refs/remotes/origin/master
static int octeon_mdiobus_read(struct mii_bus *bus, int phy_id, int regnum)
{
	struct octeon_mdiobus *p = bus->priv;
	union cvmx_smix_cmd smi_cmd;
	union cvmx_smix_rd_dat smi_rd;
<<<<<<< HEAD
	int timeout = 1000;

	smi_cmd.u64 = 0;
	smi_cmd.s.phy_op = 1; /* MDIO_CLAUSE_22_READ */
	smi_cmd.s.phy_adr = phy_id;
	smi_cmd.s.reg_adr = regnum;
	cvmx_write_csr(CVMX_SMIX_CMD(p->unit), smi_cmd.u64);

	do {
		/*
		 * Wait 1000 clocks so we don't saturate the RSL bus
		 * doing reads.
		 */
		cvmx_wait(1000);
		smi_rd.u64 = cvmx_read_csr(CVMX_SMIX_RD_DAT(p->unit));
=======
	unsigned int op = 1; /* MDIO_CLAUSE_22_READ */
	int timeout = 1000;

	if (regnum & MII_ADDR_C45) {
		int r = octeon_mdiobus_c45_addr(p, phy_id, regnum);
		if (r < 0)
			return r;

		regnum = (regnum >> 16) & 0x1f;
		op = 3; /* MDIO_CLAUSE_45_READ */
	} else {
		octeon_mdiobus_set_mode(p, C22);
	}


	smi_cmd.u64 = 0;
	smi_cmd.s.phy_op = op;
	smi_cmd.s.phy_adr = phy_id;
	smi_cmd.s.reg_adr = regnum;
	cvmx_write_csr(p->register_base + SMI_CMD, smi_cmd.u64);

	do {
		/* Wait 1000 clocks so we don't saturate the RSL bus
		 * doing reads.
		 */
		__delay(1000);
		smi_rd.u64 = cvmx_read_csr(p->register_base + SMI_RD_DAT);
>>>>>>> refs/remotes/origin/master
	} while (smi_rd.s.pending && --timeout);

	if (smi_rd.s.val)
		return smi_rd.s.dat;
	else
		return -EIO;
}

static int octeon_mdiobus_write(struct mii_bus *bus, int phy_id,
				int regnum, u16 val)
{
	struct octeon_mdiobus *p = bus->priv;
	union cvmx_smix_cmd smi_cmd;
	union cvmx_smix_wr_dat smi_wr;
<<<<<<< HEAD
	int timeout = 1000;

	smi_wr.u64 = 0;
	smi_wr.s.dat = val;
	cvmx_write_csr(CVMX_SMIX_WR_DAT(p->unit), smi_wr.u64);

	smi_cmd.u64 = 0;
	smi_cmd.s.phy_op = 0; /* MDIO_CLAUSE_22_WRITE */
	smi_cmd.s.phy_adr = phy_id;
	smi_cmd.s.reg_adr = regnum;
	cvmx_write_csr(CVMX_SMIX_CMD(p->unit), smi_cmd.u64);

	do {
		/*
		 * Wait 1000 clocks so we don't saturate the RSL bus
		 * doing reads.
		 */
		cvmx_wait(1000);
		smi_wr.u64 = cvmx_read_csr(CVMX_SMIX_WR_DAT(p->unit));
=======
	unsigned int op = 0; /* MDIO_CLAUSE_22_WRITE */
	int timeout = 1000;


	if (regnum & MII_ADDR_C45) {
		int r = octeon_mdiobus_c45_addr(p, phy_id, regnum);
		if (r < 0)
			return r;

		regnum = (regnum >> 16) & 0x1f;
		op = 1; /* MDIO_CLAUSE_45_WRITE */
	} else {
		octeon_mdiobus_set_mode(p, C22);
	}

	smi_wr.u64 = 0;
	smi_wr.s.dat = val;
	cvmx_write_csr(p->register_base + SMI_WR_DAT, smi_wr.u64);

	smi_cmd.u64 = 0;
	smi_cmd.s.phy_op = op;
	smi_cmd.s.phy_adr = phy_id;
	smi_cmd.s.reg_adr = regnum;
	cvmx_write_csr(p->register_base + SMI_CMD, smi_cmd.u64);

	do {
		/* Wait 1000 clocks so we don't saturate the RSL bus
		 * doing reads.
		 */
		__delay(1000);
		smi_wr.u64 = cvmx_read_csr(p->register_base + SMI_WR_DAT);
>>>>>>> refs/remotes/origin/master
	} while (smi_wr.s.pending && --timeout);

	if (timeout <= 0)
		return -EIO;

	return 0;
}

<<<<<<< HEAD
static int __devinit octeon_mdiobus_probe(struct platform_device *pdev)
{
	struct octeon_mdiobus *bus;
	union cvmx_smix_en smi_en;
	int i;
=======
static int octeon_mdiobus_probe(struct platform_device *pdev)
{
	struct octeon_mdiobus *bus;
	struct resource *res_mem;
	union cvmx_smix_en smi_en;
>>>>>>> refs/remotes/origin/master
	int err = -ENOENT;

	bus = devm_kzalloc(&pdev->dev, sizeof(*bus), GFP_KERNEL);
	if (!bus)
		return -ENOMEM;

<<<<<<< HEAD
	/* The platform_device id is our unit number.  */
	bus->unit = pdev->id;
=======
	res_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	if (res_mem == NULL) {
		dev_err(&pdev->dev, "found no memory resource\n");
		err = -ENXIO;
		goto fail;
	}
	bus->mdio_phys = res_mem->start;
	bus->regsize = resource_size(res_mem);
	if (!devm_request_mem_region(&pdev->dev, bus->mdio_phys, bus->regsize,
				     res_mem->name)) {
		dev_err(&pdev->dev, "request_mem_region failed\n");
		goto fail;
	}
	bus->register_base =
		(u64)devm_ioremap(&pdev->dev, bus->mdio_phys, bus->regsize);
>>>>>>> refs/remotes/origin/master

	bus->mii_bus = mdiobus_alloc();

	if (!bus->mii_bus)
<<<<<<< HEAD
		goto err;

	smi_en.u64 = 0;
	smi_en.s.en = 1;
	cvmx_write_csr(CVMX_SMIX_EN(bus->unit), smi_en.u64);

	/*
	 * Standard Octeon evaluation boards don't support phy
	 * interrupts, we need to poll.
	 */
	for (i = 0; i < PHY_MAX_ADDR; i++)
		bus->phy_irq[i] = PHY_POLL;
=======
		goto fail;

	smi_en.u64 = 0;
	smi_en.s.en = 1;
	cvmx_write_csr(bus->register_base + SMI_EN, smi_en.u64);
>>>>>>> refs/remotes/origin/master

	bus->mii_bus->priv = bus;
	bus->mii_bus->irq = bus->phy_irq;
	bus->mii_bus->name = "mdio-octeon";
<<<<<<< HEAD
<<<<<<< HEAD
	snprintf(bus->mii_bus->id, MII_BUS_ID_SIZE, "%x", bus->unit);
=======
	snprintf(bus->mii_bus->id, MII_BUS_ID_SIZE, "%s-%x",
		bus->mii_bus->name, bus->unit);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	snprintf(bus->mii_bus->id, MII_BUS_ID_SIZE, "%llx", bus->register_base);
>>>>>>> refs/remotes/origin/master
	bus->mii_bus->parent = &pdev->dev;

	bus->mii_bus->read = octeon_mdiobus_read;
	bus->mii_bus->write = octeon_mdiobus_write;

<<<<<<< HEAD
	dev_set_drvdata(&pdev->dev, bus);

	err = mdiobus_register(bus->mii_bus);
	if (err)
		goto err_register;
=======
	platform_set_drvdata(pdev, bus);

	err = of_mdiobus_register(bus->mii_bus, pdev->dev.of_node);
	if (err)
		goto fail_register;
>>>>>>> refs/remotes/origin/master

	dev_info(&pdev->dev, "Version " DRV_VERSION "\n");

	return 0;
<<<<<<< HEAD
err_register:
	mdiobus_free(bus->mii_bus);

err:
	devm_kfree(&pdev->dev, bus);
	smi_en.u64 = 0;
	cvmx_write_csr(CVMX_SMIX_EN(bus->unit), smi_en.u64);
	return err;
}

static int __devexit octeon_mdiobus_remove(struct platform_device *pdev)
=======
fail_register:
	mdiobus_free(bus->mii_bus);
fail:
	smi_en.u64 = 0;
	cvmx_write_csr(bus->register_base + SMI_EN, smi_en.u64);
	return err;
}

static int octeon_mdiobus_remove(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	struct octeon_mdiobus *bus;
	union cvmx_smix_en smi_en;

<<<<<<< HEAD
	bus = dev_get_drvdata(&pdev->dev);
=======
	bus = platform_get_drvdata(pdev);
>>>>>>> refs/remotes/origin/master

	mdiobus_unregister(bus->mii_bus);
	mdiobus_free(bus->mii_bus);
	smi_en.u64 = 0;
<<<<<<< HEAD
	cvmx_write_csr(CVMX_SMIX_EN(bus->unit), smi_en.u64);
	return 0;
}

=======
	cvmx_write_csr(bus->register_base + SMI_EN, smi_en.u64);
	return 0;
}

static struct of_device_id octeon_mdiobus_match[] = {
	{
		.compatible = "cavium,octeon-3860-mdio",
	},
	{},
};
MODULE_DEVICE_TABLE(of, octeon_mdiobus_match);

>>>>>>> refs/remotes/origin/master
static struct platform_driver octeon_mdiobus_driver = {
	.driver = {
		.name		= "mdio-octeon",
		.owner		= THIS_MODULE,
<<<<<<< HEAD
	},
	.probe		= octeon_mdiobus_probe,
	.remove		= __devexit_p(octeon_mdiobus_remove),
=======
		.of_match_table = octeon_mdiobus_match,
	},
	.probe		= octeon_mdiobus_probe,
	.remove		= octeon_mdiobus_remove,
>>>>>>> refs/remotes/origin/master
};

void octeon_mdiobus_force_mod_depencency(void)
{
	/* Let ethernet drivers force us to be loaded.  */
}
EXPORT_SYMBOL(octeon_mdiobus_force_mod_depencency);

<<<<<<< HEAD
static int __init octeon_mdiobus_mod_init(void)
{
	return platform_driver_register(&octeon_mdiobus_driver);
}

static void __exit octeon_mdiobus_mod_exit(void)
{
	platform_driver_unregister(&octeon_mdiobus_driver);
}

module_init(octeon_mdiobus_mod_init);
module_exit(octeon_mdiobus_mod_exit);
=======
module_platform_driver(octeon_mdiobus_driver);
>>>>>>> refs/remotes/origin/master

MODULE_DESCRIPTION(DRV_DESCRIPTION);
MODULE_VERSION(DRV_VERSION);
MODULE_AUTHOR("David Daney");
MODULE_LICENSE("GPL");
