/*
 * drivers/net/phy/smsc.c
 *
 * Driver for SMSC PHYs
 *
 * Author: Herbert Valerio Riedel
 *
 * Copyright (c) 2006 Herbert Valerio Riedel <hvr@gnu.org>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
<<<<<<< HEAD
 * Support added for SMSC LAN8187 and LAN8700 by steve.glendinning@smsc.com
=======
 * Support added for SMSC LAN8187 and LAN8700 by steve.glendinning@shawell.net
>>>>>>> refs/remotes/origin/master
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mii.h>
#include <linux/ethtool.h>
#include <linux/phy.h>
#include <linux/netdevice.h>
<<<<<<< HEAD
<<<<<<< HEAD

#define MII_LAN83C185_ISF 29 /* Interrupt Source Flags */
#define MII_LAN83C185_IM  30 /* Interrupt Mask */
#define MII_LAN83C185_CTRL_STATUS 17 /* Mode/Status Register */

#define MII_LAN83C185_ISF_INT1 (1<<1) /* Auto-Negotiation Page Received */
#define MII_LAN83C185_ISF_INT2 (1<<2) /* Parallel Detection Fault */
#define MII_LAN83C185_ISF_INT3 (1<<3) /* Auto-Negotiation LP Ack */
#define MII_LAN83C185_ISF_INT4 (1<<4) /* Link Down */
#define MII_LAN83C185_ISF_INT5 (1<<5) /* Remote Fault Detected */
#define MII_LAN83C185_ISF_INT6 (1<<6) /* Auto-Negotiation complete */
#define MII_LAN83C185_ISF_INT7 (1<<7) /* ENERGYON */

#define MII_LAN83C185_ISF_INT_ALL (0x0e)

#define MII_LAN83C185_ISF_INT_PHYLIB_EVENTS \
	(MII_LAN83C185_ISF_INT6 | MII_LAN83C185_ISF_INT4 | \
	 MII_LAN83C185_ISF_INT7)

#define MII_LAN83C185_EDPWRDOWN	(1 << 13) /* EDPWRDOWN */
=======
#include <linux/smscphy.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/smscphy.h>
>>>>>>> refs/remotes/origin/master

static int smsc_phy_config_intr(struct phy_device *phydev)
{
	int rc = phy_write (phydev, MII_LAN83C185_IM,
			((PHY_INTERRUPT_ENABLED == phydev->interrupts)
			? MII_LAN83C185_ISF_INT_PHYLIB_EVENTS
			: 0));

	return rc < 0 ? rc : 0;
}

static int smsc_phy_ack_interrupt(struct phy_device *phydev)
{
	int rc = phy_read (phydev, MII_LAN83C185_ISF);

	return rc < 0 ? rc : 0;
}

static int smsc_phy_config_init(struct phy_device *phydev)
{
<<<<<<< HEAD
	int rc = phy_read(phydev, MII_LAN83C185_CTRL_STATUS);
=======
	int rc = phy_read(phydev, MII_LAN83C185_SPECIAL_MODES);
	if (rc < 0)
		return rc;

	/* If the SMSC PHY is in power down mode, then set it
	 * in all capable mode before using it.
	 */
	if ((rc & MII_LAN83C185_MODE_MASK) == MII_LAN83C185_MODE_POWERDOWN) {
		int timeout = 50000;

		/* set "all capable" mode and reset the phy */
		rc |= MII_LAN83C185_MODE_ALL;
		phy_write(phydev, MII_LAN83C185_SPECIAL_MODES, rc);
		phy_write(phydev, MII_BMCR, BMCR_RESET);

		/* wait end of reset (max 500 ms) */
		do {
			udelay(10);
			if (timeout-- == 0)
				return -1;
			rc = phy_read(phydev, MII_BMCR);
		} while (rc & BMCR_RESET);
	}

	rc = phy_read(phydev, MII_LAN83C185_CTRL_STATUS);
>>>>>>> refs/remotes/origin/master
	if (rc < 0)
		return rc;

	/* Enable energy detect mode for this SMSC Transceivers */
	rc = phy_write(phydev, MII_LAN83C185_CTRL_STATUS,
		       rc | MII_LAN83C185_EDPWRDOWN);
	if (rc < 0)
		return rc;

	return smsc_phy_ack_interrupt (phydev);
}

static int lan911x_config_init(struct phy_device *phydev)
{
	return smsc_phy_ack_interrupt(phydev);
}

<<<<<<< HEAD
static struct phy_driver lan83c185_driver = {
=======
/*
 * The LAN8710/LAN8720 requires a minimum of 2 link pulses within 64ms of each
 * other in order to set the ENERGYON bit and exit EDPD mode.  If a link partner
 * does send the pulses within this interval, the PHY will remained powered
 * down.
 *
 * This workaround will manually toggle the PHY on/off upon calls to read_status
 * in order to generate link test pulses if the link is down.  If a link partner
 * is present, it will respond to the pulses, which will cause the ENERGYON bit
 * to be set and will cause the EDPD mode to be exited.
 */
static int lan87xx_read_status(struct phy_device *phydev)
{
	int err = genphy_read_status(phydev);

	if (!phydev->link) {
		/* Disable EDPD to wake up PHY */
		int rc = phy_read(phydev, MII_LAN83C185_CTRL_STATUS);
		if (rc < 0)
			return rc;

		rc = phy_write(phydev, MII_LAN83C185_CTRL_STATUS,
			       rc & ~MII_LAN83C185_EDPWRDOWN);
		if (rc < 0)
			return rc;

		/* Sleep 64 ms to allow ~5 link test pulses to be sent */
		msleep(64);

		/* Re-enable EDPD */
		rc = phy_read(phydev, MII_LAN83C185_CTRL_STATUS);
		if (rc < 0)
			return rc;

		rc = phy_write(phydev, MII_LAN83C185_CTRL_STATUS,
			       rc | MII_LAN83C185_EDPWRDOWN);
		if (rc < 0)
			return rc;
	}

	return err;
}

static struct phy_driver smsc_phy_driver[] = {
{
>>>>>>> refs/remotes/origin/master
	.phy_id		= 0x0007c0a0, /* OUI=0x00800f, Model#=0x0a */
	.phy_id_mask	= 0xfffffff0,
	.name		= "SMSC LAN83C185",

	.features	= (PHY_BASIC_FEATURES | SUPPORTED_Pause
				| SUPPORTED_Asym_Pause),
	.flags		= PHY_HAS_INTERRUPT | PHY_HAS_MAGICANEG,

	/* basic functions */
	.config_aneg	= genphy_config_aneg,
	.read_status	= genphy_read_status,
	.config_init	= smsc_phy_config_init,

	/* IRQ related */
	.ack_interrupt	= smsc_phy_ack_interrupt,
	.config_intr	= smsc_phy_config_intr,

	.suspend	= genphy_suspend,
	.resume		= genphy_resume,

	.driver		= { .owner = THIS_MODULE, }
<<<<<<< HEAD
};

static struct phy_driver lan8187_driver = {
=======
}, {
>>>>>>> refs/remotes/origin/master
	.phy_id		= 0x0007c0b0, /* OUI=0x00800f, Model#=0x0b */
	.phy_id_mask	= 0xfffffff0,
	.name		= "SMSC LAN8187",

	.features	= (PHY_BASIC_FEATURES | SUPPORTED_Pause
				| SUPPORTED_Asym_Pause),
	.flags		= PHY_HAS_INTERRUPT | PHY_HAS_MAGICANEG,

	/* basic functions */
	.config_aneg	= genphy_config_aneg,
	.read_status	= genphy_read_status,
	.config_init	= smsc_phy_config_init,

	/* IRQ related */
	.ack_interrupt	= smsc_phy_ack_interrupt,
	.config_intr	= smsc_phy_config_intr,

	.suspend	= genphy_suspend,
	.resume		= genphy_resume,

	.driver		= { .owner = THIS_MODULE, }
<<<<<<< HEAD
};

static struct phy_driver lan8700_driver = {
=======
}, {
>>>>>>> refs/remotes/origin/master
	.phy_id		= 0x0007c0c0, /* OUI=0x00800f, Model#=0x0c */
	.phy_id_mask	= 0xfffffff0,
	.name		= "SMSC LAN8700",

	.features	= (PHY_BASIC_FEATURES | SUPPORTED_Pause
				| SUPPORTED_Asym_Pause),
	.flags		= PHY_HAS_INTERRUPT | PHY_HAS_MAGICANEG,

	/* basic functions */
	.config_aneg	= genphy_config_aneg,
	.read_status	= genphy_read_status,
	.config_init	= smsc_phy_config_init,

	/* IRQ related */
	.ack_interrupt	= smsc_phy_ack_interrupt,
	.config_intr	= smsc_phy_config_intr,

	.suspend	= genphy_suspend,
	.resume		= genphy_resume,

	.driver		= { .owner = THIS_MODULE, }
<<<<<<< HEAD
};

static struct phy_driver lan911x_int_driver = {
=======
}, {
>>>>>>> refs/remotes/origin/master
	.phy_id		= 0x0007c0d0, /* OUI=0x00800f, Model#=0x0d */
	.phy_id_mask	= 0xfffffff0,
	.name		= "SMSC LAN911x Internal PHY",

	.features	= (PHY_BASIC_FEATURES | SUPPORTED_Pause
				| SUPPORTED_Asym_Pause),
	.flags		= PHY_HAS_INTERRUPT | PHY_HAS_MAGICANEG,

	/* basic functions */
	.config_aneg	= genphy_config_aneg,
	.read_status	= genphy_read_status,
	.config_init	= lan911x_config_init,

	/* IRQ related */
	.ack_interrupt	= smsc_phy_ack_interrupt,
	.config_intr	= smsc_phy_config_intr,

	.suspend	= genphy_suspend,
	.resume		= genphy_resume,

	.driver		= { .owner = THIS_MODULE, }
<<<<<<< HEAD
};

static struct phy_driver lan8710_driver = {
=======
}, {
>>>>>>> refs/remotes/origin/master
	.phy_id		= 0x0007c0f0, /* OUI=0x00800f, Model#=0x0f */
	.phy_id_mask	= 0xfffffff0,
	.name		= "SMSC LAN8710/LAN8720",

	.features	= (PHY_BASIC_FEATURES | SUPPORTED_Pause
				| SUPPORTED_Asym_Pause),
	.flags		= PHY_HAS_INTERRUPT | PHY_HAS_MAGICANEG,

	/* basic functions */
	.config_aneg	= genphy_config_aneg,
<<<<<<< HEAD
	.read_status	= genphy_read_status,
=======
	.read_status	= lan87xx_read_status,
>>>>>>> refs/remotes/origin/master
	.config_init	= smsc_phy_config_init,

	/* IRQ related */
	.ack_interrupt	= smsc_phy_ack_interrupt,
	.config_intr	= smsc_phy_config_intr,

	.suspend	= genphy_suspend,
	.resume		= genphy_resume,

	.driver		= { .owner = THIS_MODULE, }
<<<<<<< HEAD
};

static int __init smsc_init(void)
{
	int ret;

	ret = phy_driver_register (&lan83c185_driver);
	if (ret)
		goto err1;

	ret = phy_driver_register (&lan8187_driver);
	if (ret)
		goto err2;

	ret = phy_driver_register (&lan8700_driver);
	if (ret)
		goto err3;

	ret = phy_driver_register (&lan911x_int_driver);
	if (ret)
		goto err4;

	ret = phy_driver_register (&lan8710_driver);
	if (ret)
		goto err5;

	return 0;

err5:
	phy_driver_unregister (&lan911x_int_driver);
err4:
	phy_driver_unregister (&lan8700_driver);
err3:
	phy_driver_unregister (&lan8187_driver);
err2:
	phy_driver_unregister (&lan83c185_driver);
err1:
	return ret;
=======
} };

static int __init smsc_init(void)
{
	return phy_drivers_register(smsc_phy_driver,
		ARRAY_SIZE(smsc_phy_driver));
>>>>>>> refs/remotes/origin/master
}

static void __exit smsc_exit(void)
{
<<<<<<< HEAD
	phy_driver_unregister (&lan8710_driver);
	phy_driver_unregister (&lan911x_int_driver);
	phy_driver_unregister (&lan8700_driver);
	phy_driver_unregister (&lan8187_driver);
	phy_driver_unregister (&lan83c185_driver);
=======
	return phy_drivers_unregister(smsc_phy_driver,
		ARRAY_SIZE(smsc_phy_driver));
>>>>>>> refs/remotes/origin/master
}

MODULE_DESCRIPTION("SMSC PHY driver");
MODULE_AUTHOR("Herbert Valerio Riedel");
MODULE_LICENSE("GPL");

module_init(smsc_init);
module_exit(smsc_exit);

static struct mdio_device_id __maybe_unused smsc_tbl[] = {
	{ 0x0007c0a0, 0xfffffff0 },
	{ 0x0007c0b0, 0xfffffff0 },
	{ 0x0007c0c0, 0xfffffff0 },
	{ 0x0007c0d0, 0xfffffff0 },
	{ 0x0007c0f0, 0xfffffff0 },
	{ }
};

MODULE_DEVICE_TABLE(mdio, smsc_tbl);
