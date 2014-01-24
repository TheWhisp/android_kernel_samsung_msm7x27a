/*
<<<<<<< HEAD
 * IR port driver for the Cirrus Logic EP7211 processor.
=======
 * IR port driver for the Cirrus Logic CLPS711X processors
>>>>>>> refs/remotes/origin/master
 *
 * Copyright 2001, Blue Mug Inc.  All rights reserved.
 * Copyright 2007, Samuel Ortiz <samuel@sortiz.org>
 */
<<<<<<< HEAD
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/tty.h>
#include <linux/init.h>
#include <linux/spinlock.h>

#include <net/irda/irda.h>
#include <net/irda/irda_device.h>

#include <asm/io.h>
=======

#include <linux/module.h>
#include <linux/platform_device.h>

>>>>>>> refs/remotes/origin/master
#include <mach/hardware.h>

#include "sir-dev.h"

<<<<<<< HEAD
#define MIN_DELAY 25      /* 15 us, but wait a little more to be sure */
#define MAX_DELAY 10000   /* 1 ms */

static int ep7211_open(struct sir_dev *dev);
static int ep7211_close(struct sir_dev *dev);
static int ep7211_change_speed(struct sir_dev *dev, unsigned speed);
static int ep7211_reset(struct sir_dev *dev);

static struct dongle_driver ep7211 = {
	.owner		= THIS_MODULE,
	.driver_name	= "EP7211 IR driver",
	.type		= IRDA_EP7211_DONGLE,
	.open		= ep7211_open,
	.close		= ep7211_close,
	.reset		= ep7211_reset,
	.set_speed	= ep7211_change_speed,
};

static int __init ep7211_sir_init(void)
{
	return irda_register_dongle(&ep7211);
}

static void __exit ep7211_sir_cleanup(void)
{
	irda_unregister_dongle(&ep7211);
}

static int ep7211_open(struct sir_dev *dev)
=======
static int clps711x_dongle_open(struct sir_dev *dev)
>>>>>>> refs/remotes/origin/master
{
	unsigned int syscon;

	/* Turn on the SIR encoder. */
	syscon = clps_readl(SYSCON1);
	syscon |= SYSCON1_SIREN;
	clps_writel(syscon, SYSCON1);

	return 0;
}

<<<<<<< HEAD
static int ep7211_close(struct sir_dev *dev)
=======
static int clps711x_dongle_close(struct sir_dev *dev)
>>>>>>> refs/remotes/origin/master
{
	unsigned int syscon;

	/* Turn off the SIR encoder. */
	syscon = clps_readl(SYSCON1);
	syscon &= ~SYSCON1_SIREN;
	clps_writel(syscon, SYSCON1);

	return 0;
}

<<<<<<< HEAD
static int ep7211_change_speed(struct sir_dev *dev, unsigned speed)
{
	return 0;
}

static int ep7211_reset(struct sir_dev *dev)
{
	return 0;
}

=======
static struct dongle_driver clps711x_dongle = {
	.owner		= THIS_MODULE,
	.driver_name	= "EP7211 IR driver",
	.type		= IRDA_EP7211_DONGLE,
	.open		= clps711x_dongle_open,
	.close		= clps711x_dongle_close,
};

static int clps711x_sir_probe(struct platform_device *pdev)
{
	return irda_register_dongle(&clps711x_dongle);
}

static int clps711x_sir_remove(struct platform_device *pdev)
{
	return irda_unregister_dongle(&clps711x_dongle);
}

static struct platform_driver clps711x_sir_driver = {
	.driver	= {
		.name	= "sir-clps711x",
		.owner	= THIS_MODULE,
	},
	.probe	= clps711x_sir_probe,
	.remove	= clps711x_sir_remove,
};
module_platform_driver(clps711x_sir_driver);

>>>>>>> refs/remotes/origin/master
MODULE_AUTHOR("Samuel Ortiz <samuel@sortiz.org>");
MODULE_DESCRIPTION("EP7211 IR dongle driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("irda-dongle-13"); /* IRDA_EP7211_DONGLE */
<<<<<<< HEAD

module_init(ep7211_sir_init);
module_exit(ep7211_sir_cleanup);
=======
>>>>>>> refs/remotes/origin/master
