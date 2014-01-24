/*
 * Copyright (C) 2011 Samsung Electronics Co.Ltd
 * Authors:
 *	Seung-Woo Kim <sw0312.kim@samsung.com>
 *	Inki Dae <inki.dae@samsung.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

<<<<<<< HEAD
#include "drmP.h"

#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/module.h>
=======
#include <drm/drmP.h>

#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/of.h>
>>>>>>> refs/remotes/origin/master

#include "exynos_drm_drv.h"
#include "exynos_hdmi.h"


static int hdmiphy_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	hdmi_attach_hdmiphy_client(client);

	dev_info(&client->adapter->dev, "attached s5p_hdmiphy "
		"into i2c adapter successfully\n");

	return 0;
}

static int hdmiphy_remove(struct i2c_client *client)
{
	dev_info(&client->adapter->dev, "detached s5p_hdmiphy "
		"from i2c adapter successfully\n");

	return 0;
}

<<<<<<< HEAD
static const struct i2c_device_id hdmiphy_id[] = {
	{ "s5p_hdmiphy", 0 },
	{ },
=======
static struct of_device_id hdmiphy_match_types[] = {
	{
		.compatible = "samsung,exynos5-hdmiphy",
	}, {
		.compatible = "samsung,exynos4210-hdmiphy",
	}, {
		.compatible = "samsung,exynos4212-hdmiphy",
	}, {
		/* end node */
	}
>>>>>>> refs/remotes/origin/master
};

struct i2c_driver hdmiphy_driver = {
	.driver = {
<<<<<<< HEAD
		.name	= "s5p-hdmiphy",
		.owner	= THIS_MODULE,
	},
	.id_table = hdmiphy_id,
	.probe		= hdmiphy_probe,
	.remove		= __devexit_p(hdmiphy_remove),
=======
		.name	= "exynos-hdmiphy",
		.owner	= THIS_MODULE,
		.of_match_table = hdmiphy_match_types,
	},
	.probe		= hdmiphy_probe,
	.remove		= hdmiphy_remove,
>>>>>>> refs/remotes/origin/master
	.command		= NULL,
};
EXPORT_SYMBOL(hdmiphy_driver);
