/*
 * Driver for Vitesse PHYs
 *
 * Author: Kriston Carson
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (c) 2005 Freescale Semiconductor, Inc.
=======
 * Copyright (c) 2005, 2009 Freescale Semiconductor, Inc.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright (c) 2005, 2009, 2011 Freescale Semiconductor, Inc.
>>>>>>> refs/remotes/origin/master
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mii.h>
#include <linux/ethtool.h>
#include <linux/phy.h>

<<<<<<< HEAD
=======
/* Vitesse Extended Page Magic Register(s) */
#define MII_VSC82X4_EXT_PAGE_16E	0x10
#define MII_VSC82X4_EXT_PAGE_17E	0x11
#define MII_VSC82X4_EXT_PAGE_18E	0x12

>>>>>>> refs/remotes/origin/master
/* Vitesse Extended Control Register 1 */
#define MII_VSC8244_EXT_CON1           0x17
#define MII_VSC8244_EXTCON1_INIT       0x0000
#define MII_VSC8244_EXTCON1_TX_SKEW_MASK	0x0c00
#define MII_VSC8244_EXTCON1_RX_SKEW_MASK	0x0300
#define MII_VSC8244_EXTCON1_TX_SKEW	0x0800
#define MII_VSC8244_EXTCON1_RX_SKEW	0x0200

/* Vitesse Interrupt Mask Register */
#define MII_VSC8244_IMASK		0x19
#define MII_VSC8244_IMASK_IEN		0x8000
#define MII_VSC8244_IMASK_SPEED		0x4000
#define MII_VSC8244_IMASK_LINK		0x2000
#define MII_VSC8244_IMASK_DUPLEX	0x1000
#define MII_VSC8244_IMASK_MASK		0xf000

#define MII_VSC8221_IMASK_MASK		0xa000

/* Vitesse Interrupt Status Register */
#define MII_VSC8244_ISTAT		0x1a
#define MII_VSC8244_ISTAT_STATUS	0x8000
#define MII_VSC8244_ISTAT_SPEED		0x4000
#define MII_VSC8244_ISTAT_LINK		0x2000
#define MII_VSC8244_ISTAT_DUPLEX	0x1000

/* Vitesse Auxiliary Control/Status Register */
<<<<<<< HEAD
#define MII_VSC8244_AUX_CONSTAT        	0x1c
#define MII_VSC8244_AUXCONSTAT_INIT    	0x0000
#define MII_VSC8244_AUXCONSTAT_DUPLEX  	0x0020
#define MII_VSC8244_AUXCONSTAT_SPEED   	0x0018
#define MII_VSC8244_AUXCONSTAT_GBIT    	0x0010
#define MII_VSC8244_AUXCONSTAT_100     	0x0008
=======
#define MII_VSC8244_AUX_CONSTAT		0x1c
#define MII_VSC8244_AUXCONSTAT_INIT	0x0000
#define MII_VSC8244_AUXCONSTAT_DUPLEX	0x0020
#define MII_VSC8244_AUXCONSTAT_SPEED	0x0018
#define MII_VSC8244_AUXCONSTAT_GBIT	0x0010
#define MII_VSC8244_AUXCONSTAT_100	0x0008
>>>>>>> refs/remotes/origin/master

#define MII_VSC8221_AUXCONSTAT_INIT	0x0004 /* need to set this bit? */
#define MII_VSC8221_AUXCONSTAT_RESERVED	0x0004

<<<<<<< HEAD
#define PHY_ID_VSC8244			0x000fc6c0
#define PHY_ID_VSC8221			0x000fc550
=======
/* Vitesse Extended Page Access Register */
#define MII_VSC82X4_EXT_PAGE_ACCESS	0x1f

#define PHY_ID_VSC8234			0x000fc620
#define PHY_ID_VSC8244			0x000fc6c0
#define PHY_ID_VSC8514			0x00070670
#define PHY_ID_VSC8574			0x000704a0
#define PHY_ID_VSC8662			0x00070660
#define PHY_ID_VSC8221			0x000fc550
#define PHY_ID_VSC8211			0x000fc4b0
>>>>>>> refs/remotes/origin/master

MODULE_DESCRIPTION("Vitesse PHY driver");
MODULE_AUTHOR("Kriston Carson");
MODULE_LICENSE("GPL");

<<<<<<< HEAD
<<<<<<< HEAD
static int vsc824x_config_init(struct phy_device *phydev)
{
	int extcon;
	int err;

	err = phy_write(phydev, MII_VSC8244_AUX_CONSTAT,
			MII_VSC8244_AUXCONSTAT_INIT);
	if (err < 0)
		return err;
=======
int vsc824x_add_skew(struct phy_device *phydev)
{
	int err;
	int extcon;
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int vsc824x_add_skew(struct phy_device *phydev)
{
	int err;
	int extcon;
>>>>>>> refs/remotes/origin/master

	extcon = phy_read(phydev, MII_VSC8244_EXT_CON1);

	if (extcon < 0)
<<<<<<< HEAD
<<<<<<< HEAD
		return err;
=======
		return extcon;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		return extcon;
>>>>>>> refs/remotes/origin/master

	extcon &= ~(MII_VSC8244_EXTCON1_TX_SKEW_MASK |
			MII_VSC8244_EXTCON1_RX_SKEW_MASK);

<<<<<<< HEAD
<<<<<<< HEAD
	if (phydev->interface == PHY_INTERFACE_MODE_RGMII_ID)
		extcon |= (MII_VSC8244_EXTCON1_TX_SKEW |
				MII_VSC8244_EXTCON1_RX_SKEW);
=======
	extcon |= (MII_VSC8244_EXTCON1_TX_SKEW |
			MII_VSC8244_EXTCON1_RX_SKEW);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	extcon |= (MII_VSC8244_EXTCON1_TX_SKEW |
			MII_VSC8244_EXTCON1_RX_SKEW);
>>>>>>> refs/remotes/origin/master

	err = phy_write(phydev, MII_VSC8244_EXT_CON1, extcon);

	return err;
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
EXPORT_SYMBOL(vsc824x_add_skew);
=======
>>>>>>> refs/remotes/origin/master

static int vsc824x_config_init(struct phy_device *phydev)
{
	int err;

	err = phy_write(phydev, MII_VSC8244_AUX_CONSTAT,
			MII_VSC8244_AUXCONSTAT_INIT);
	if (err < 0)
		return err;

	if (phydev->interface == PHY_INTERFACE_MODE_RGMII_ID)
		err = vsc824x_add_skew(phydev);

	return err;
}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

static int vsc824x_ack_interrupt(struct phy_device *phydev)
{
	int err = 0;
<<<<<<< HEAD
	
	/*
	 * Don't bother to ACK the interrupts if interrupts
=======

	/* Don't bother to ACK the interrupts if interrupts
>>>>>>> refs/remotes/origin/master
	 * are disabled.  The 824x cannot clear the interrupts
	 * if they are disabled.
	 */
	if (phydev->interrupts == PHY_INTERRUPT_ENABLED)
		err = phy_read(phydev, MII_VSC8244_ISTAT);

	return (err < 0) ? err : 0;
}

static int vsc82xx_config_intr(struct phy_device *phydev)
{
	int err;

	if (phydev->interrupts == PHY_INTERRUPT_ENABLED)
		err = phy_write(phydev, MII_VSC8244_IMASK,
<<<<<<< HEAD
			phydev->drv->phy_id == PHY_ID_VSC8244 ?
				MII_VSC8244_IMASK_MASK :
				MII_VSC8221_IMASK_MASK);
	else {
		/*
		 * The Vitesse PHY cannot clear the interrupt
=======
			(phydev->drv->phy_id == PHY_ID_VSC8234 ||
			 phydev->drv->phy_id == PHY_ID_VSC8244 ||
			 phydev->drv->phy_id == PHY_ID_VSC8514 ||
			 phydev->drv->phy_id == PHY_ID_VSC8574) ?
				MII_VSC8244_IMASK_MASK :
				MII_VSC8221_IMASK_MASK);
	else {
		/* The Vitesse PHY cannot clear the interrupt
>>>>>>> refs/remotes/origin/master
		 * once it has disabled them, so we clear them first
		 */
		err = phy_read(phydev, MII_VSC8244_ISTAT);

		if (err < 0)
			return err;

		err = phy_write(phydev, MII_VSC8244_IMASK, 0);
	}

	return err;
}

<<<<<<< HEAD
/* Vitesse 824x */
static struct phy_driver vsc8244_driver = {
	.phy_id		= PHY_ID_VSC8244,
	.name		= "Vitesse VSC8244",
	.phy_id_mask	= 0x000fffc0,
	.features	= PHY_GBIT_FEATURES,
	.flags		= PHY_HAS_INTERRUPT,
	.config_init	= &vsc824x_config_init,
	.config_aneg	= &genphy_config_aneg,
	.read_status	= &genphy_read_status,
	.ack_interrupt	= &vsc824x_ack_interrupt,
	.config_intr	= &vsc82xx_config_intr,
	.driver 	= { .owner = THIS_MODULE,},
};

=======
>>>>>>> refs/remotes/origin/master
static int vsc8221_config_init(struct phy_device *phydev)
{
	int err;

	err = phy_write(phydev, MII_VSC8244_AUX_CONSTAT,
			MII_VSC8221_AUXCONSTAT_INIT);
	return err;

	/* Perhaps we should set EXT_CON1 based on the interface?
<<<<<<< HEAD
	   Options are 802.3Z SerDes or SGMII */
}

/* Vitesse 8221 */
static struct phy_driver vsc8221_driver = {
=======
	 * Options are 802.3Z SerDes or SGMII
	 */
}

/* vsc82x4_config_autocross_enable - Enable auto MDI/MDI-X for forced links
 * @phydev: target phy_device struct
 *
 * Enable auto MDI/MDI-X when in 10/100 forced link speeds by writing
 * special values in the VSC8234/VSC8244 extended reserved registers
 */
static int vsc82x4_config_autocross_enable(struct phy_device *phydev)
{
	int ret;

	if (phydev->autoneg == AUTONEG_ENABLE || phydev->speed > SPEED_100)
		return 0;

	/* map extended registers set 0x10 - 0x1e */
	ret = phy_write(phydev, MII_VSC82X4_EXT_PAGE_ACCESS, 0x52b5);
	if (ret >= 0)
		ret = phy_write(phydev, MII_VSC82X4_EXT_PAGE_18E, 0x0012);
	if (ret >= 0)
		ret = phy_write(phydev, MII_VSC82X4_EXT_PAGE_17E, 0x2803);
	if (ret >= 0)
		ret = phy_write(phydev, MII_VSC82X4_EXT_PAGE_16E, 0x87fa);
	/* map standard registers set 0x10 - 0x1e */
	if (ret >= 0)
		ret = phy_write(phydev, MII_VSC82X4_EXT_PAGE_ACCESS, 0x0000);
	else
		phy_write(phydev, MII_VSC82X4_EXT_PAGE_ACCESS, 0x0000);

	return ret;
}

/* vsc82x4_config_aneg - restart auto-negotiation or write BMCR
 * @phydev: target phy_device struct
 *
 * Description: If auto-negotiation is enabled, we configure the
 *   advertising, and then restart auto-negotiation.  If it is not
 *   enabled, then we write the BMCR and also start the auto
 *   MDI/MDI-X feature
 */
static int vsc82x4_config_aneg(struct phy_device *phydev)
{
	int ret;

	/* Enable auto MDI/MDI-X when in 10/100 forced link speeds by
	 * writing special values in the VSC8234 extended reserved registers
	 */
	if (phydev->autoneg != AUTONEG_ENABLE && phydev->speed <= SPEED_100) {
		ret = genphy_setup_forced(phydev);

		if (ret < 0) /* error */
			return ret;

		return vsc82x4_config_autocross_enable(phydev);
	}

	return genphy_config_aneg(phydev);
}

/* Vitesse 82xx */
static struct phy_driver vsc82xx_driver[] = {
{
	.phy_id         = PHY_ID_VSC8234,
	.name           = "Vitesse VSC8234",
	.phy_id_mask    = 0x000ffff0,
	.features       = PHY_GBIT_FEATURES,
	.flags          = PHY_HAS_INTERRUPT,
	.config_init    = &vsc824x_config_init,
	.config_aneg    = &vsc82x4_config_aneg,
	.read_status    = &genphy_read_status,
	.ack_interrupt  = &vsc824x_ack_interrupt,
	.config_intr    = &vsc82xx_config_intr,
	.driver         = { .owner = THIS_MODULE,},
}, {
	.phy_id		= PHY_ID_VSC8244,
	.name		= "Vitesse VSC8244",
	.phy_id_mask	= 0x000fffc0,
	.features	= PHY_GBIT_FEATURES,
	.flags		= PHY_HAS_INTERRUPT,
	.config_init	= &vsc824x_config_init,
	.config_aneg	= &vsc82x4_config_aneg,
	.read_status	= &genphy_read_status,
	.ack_interrupt	= &vsc824x_ack_interrupt,
	.config_intr	= &vsc82xx_config_intr,
	.driver		= { .owner = THIS_MODULE,},
}, {
	.phy_id		= PHY_ID_VSC8514,
	.name		= "Vitesse VSC8514",
	.phy_id_mask	= 0x000ffff0,
	.features	= PHY_GBIT_FEATURES,
	.flags		= PHY_HAS_INTERRUPT,
	.config_init	= &vsc824x_config_init,
	.config_aneg	= &vsc82x4_config_aneg,
	.read_status	= &genphy_read_status,
	.ack_interrupt	= &vsc824x_ack_interrupt,
	.config_intr	= &vsc82xx_config_intr,
	.driver		= { .owner = THIS_MODULE,},
}, {
	.phy_id         = PHY_ID_VSC8574,
	.name           = "Vitesse VSC8574",
	.phy_id_mask    = 0x000ffff0,
	.features       = PHY_GBIT_FEATURES,
	.flags          = PHY_HAS_INTERRUPT,
	.config_init    = &vsc824x_config_init,
	.config_aneg    = &vsc82x4_config_aneg,
	.read_status    = &genphy_read_status,
	.ack_interrupt  = &vsc824x_ack_interrupt,
	.config_intr    = &vsc82xx_config_intr,
	.driver         = { .owner = THIS_MODULE,},
}, {
	.phy_id         = PHY_ID_VSC8662,
	.name           = "Vitesse VSC8662",
	.phy_id_mask    = 0x000ffff0,
	.features       = PHY_GBIT_FEATURES,
	.flags          = PHY_HAS_INTERRUPT,
	.config_init    = &vsc824x_config_init,
	.config_aneg    = &vsc82x4_config_aneg,
	.read_status    = &genphy_read_status,
	.ack_interrupt  = &vsc824x_ack_interrupt,
	.config_intr    = &vsc82xx_config_intr,
	.driver         = { .owner = THIS_MODULE,},
}, {
	/* Vitesse 8221 */
>>>>>>> refs/remotes/origin/master
	.phy_id		= PHY_ID_VSC8221,
	.phy_id_mask	= 0x000ffff0,
	.name		= "Vitesse VSC8221",
	.features	= PHY_GBIT_FEATURES,
	.flags		= PHY_HAS_INTERRUPT,
	.config_init	= &vsc8221_config_init,
	.config_aneg	= &genphy_config_aneg,
	.read_status	= &genphy_read_status,
	.ack_interrupt	= &vsc824x_ack_interrupt,
	.config_intr	= &vsc82xx_config_intr,
<<<<<<< HEAD
	.driver 	= { .owner = THIS_MODULE,},
};

static int __init vsc82xx_init(void)
{
	int err;

	err = phy_driver_register(&vsc8244_driver);
	if (err < 0)
		return err;
	err = phy_driver_register(&vsc8221_driver);
	if (err < 0)
		phy_driver_unregister(&vsc8244_driver);
	return err;
=======
	.driver		= { .owner = THIS_MODULE,},
}, {
	/* Vitesse 8211 */
	.phy_id		= PHY_ID_VSC8211,
	.phy_id_mask	= 0x000ffff0,
	.name		= "Vitesse VSC8211",
	.features	= PHY_GBIT_FEATURES,
	.flags		= PHY_HAS_INTERRUPT,
	.config_init	= &vsc8221_config_init,
	.config_aneg	= &genphy_config_aneg,
	.read_status	= &genphy_read_status,
	.ack_interrupt	= &vsc824x_ack_interrupt,
	.config_intr	= &vsc82xx_config_intr,
	.driver		= { .owner = THIS_MODULE,},
} };

static int __init vsc82xx_init(void)
{
	return phy_drivers_register(vsc82xx_driver,
		ARRAY_SIZE(vsc82xx_driver));
>>>>>>> refs/remotes/origin/master
}

static void __exit vsc82xx_exit(void)
{
<<<<<<< HEAD
	phy_driver_unregister(&vsc8244_driver);
	phy_driver_unregister(&vsc8221_driver);
=======
	return phy_drivers_unregister(vsc82xx_driver,
		ARRAY_SIZE(vsc82xx_driver));
>>>>>>> refs/remotes/origin/master
}

module_init(vsc82xx_init);
module_exit(vsc82xx_exit);

static struct mdio_device_id __maybe_unused vitesse_tbl[] = {
<<<<<<< HEAD
	{ PHY_ID_VSC8244, 0x000fffc0 },
	{ PHY_ID_VSC8221, 0x000ffff0 },
=======
	{ PHY_ID_VSC8234, 0x000ffff0 },
	{ PHY_ID_VSC8244, 0x000fffc0 },
	{ PHY_ID_VSC8514, 0x000ffff0 },
	{ PHY_ID_VSC8574, 0x000ffff0 },
	{ PHY_ID_VSC8662, 0x000ffff0 },
	{ PHY_ID_VSC8221, 0x000ffff0 },
	{ PHY_ID_VSC8211, 0x000ffff0 },
>>>>>>> refs/remotes/origin/master
	{ }
};

MODULE_DEVICE_TABLE(mdio, vitesse_tbl);
