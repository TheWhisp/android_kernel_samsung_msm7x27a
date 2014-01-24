/*
<<<<<<< HEAD
 * mcp3021.c - driver for the Microchip MCP3021 chip
 *
 * Copyright (C) 2008-2009, 2012 Freescale Semiconductor, Inc.
 * Author: Mingkai Hu <Mingkai.hu@freescale.com>
=======
 * mcp3021.c - driver for Microchip MCP3021 and MCP3221
 *
 * Copyright (C) 2008-2009, 2012 Freescale Semiconductor, Inc.
 * Author: Mingkai Hu <Mingkai.hu@freescale.com>
 * Reworked by Sven Schuchmann <schuchmann@schleissheimer.de>
>>>>>>> refs/remotes/origin/master
 *
 * This driver export the value of analog input voltage to sysfs, the
 * voltage unit is mV. Through the sysfs interface, lm-sensors tool
 * can also display the input voltage.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/hwmon.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include <linux/device.h>

/* Vdd info */
#define MCP3021_VDD_MAX		5500
#define MCP3021_VDD_MIN		2700
#define MCP3021_VDD_REF		3300

/* output format */
#define MCP3021_SAR_SHIFT	2
#define MCP3021_SAR_MASK	0x3ff

#define MCP3021_OUTPUT_RES	10	/* 10-bit resolution */
#define MCP3021_OUTPUT_SCALE	4

<<<<<<< HEAD
=======
#define MCP3221_SAR_SHIFT	0
#define MCP3221_SAR_MASK	0xfff
#define MCP3221_OUTPUT_RES	12	/* 12-bit resolution */
#define MCP3221_OUTPUT_SCALE	1

enum chips {
	mcp3021,
	mcp3221
};

>>>>>>> refs/remotes/origin/master
/*
 * Client data (each client gets its own)
 */
struct mcp3021_data {
	struct device *hwmon_dev;
	u32 vdd;	/* device power supply */
<<<<<<< HEAD
=======
	u16 sar_shift;
	u16 sar_mask;
	u8 output_res;
	u8 output_scale;
>>>>>>> refs/remotes/origin/master
};

static int mcp3021_read16(struct i2c_client *client)
{
<<<<<<< HEAD
=======
	struct mcp3021_data *data = i2c_get_clientdata(client);
>>>>>>> refs/remotes/origin/master
	int ret;
	u16 reg;
	__be16 buf;

	ret = i2c_master_recv(client, (char *)&buf, 2);
	if (ret < 0)
		return ret;
	if (ret != 2)
		return -EIO;

	/* The output code of the MCP3021 is transmitted with MSB first. */
	reg = be16_to_cpu(buf);

	/*
	 * The ten-bit output code is composed of the lower 4-bit of the
	 * first byte and the upper 6-bit of the second byte.
	 */
<<<<<<< HEAD
	reg = (reg >> MCP3021_SAR_SHIFT) & MCP3021_SAR_MASK;
=======
	reg = (reg >> data->sar_shift) & data->sar_mask;
>>>>>>> refs/remotes/origin/master

	return reg;
}

<<<<<<< HEAD
static inline u16 volts_from_reg(u16 vdd, u16 val)
=======
static inline u16 volts_from_reg(struct mcp3021_data *data, u16 val)
>>>>>>> refs/remotes/origin/master
{
	if (val == 0)
		return 0;

<<<<<<< HEAD
	val = val * MCP3021_OUTPUT_SCALE - MCP3021_OUTPUT_SCALE / 2;

	return val * DIV_ROUND_CLOSEST(vdd,
			(1 << MCP3021_OUTPUT_RES) * MCP3021_OUTPUT_SCALE);
=======
	val = val * data->output_scale - data->output_scale / 2;

	return val * DIV_ROUND_CLOSEST(data->vdd,
			(1 << data->output_res) * data->output_scale);
>>>>>>> refs/remotes/origin/master
}

static ssize_t show_in_input(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct mcp3021_data *data = i2c_get_clientdata(client);
	int reg, in_input;

	reg = mcp3021_read16(client);
	if (reg < 0)
		return reg;

<<<<<<< HEAD
	in_input = volts_from_reg(data->vdd, reg);
=======
	in_input = volts_from_reg(data, reg);

>>>>>>> refs/remotes/origin/master
	return sprintf(buf, "%d\n", in_input);
}

static DEVICE_ATTR(in0_input, S_IRUGO, show_in_input, NULL);

static int mcp3021_probe(struct i2c_client *client,
				const struct i2c_device_id *id)
{
	int err;
	struct mcp3021_data *data = NULL;

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C))
		return -ENODEV;

<<<<<<< HEAD
	data = kzalloc(sizeof(struct mcp3021_data), GFP_KERNEL);
=======
	data = devm_kzalloc(&client->dev, sizeof(struct mcp3021_data),
			    GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!data)
		return -ENOMEM;

	i2c_set_clientdata(client, data);

<<<<<<< HEAD
	if (client->dev.platform_data) {
		data->vdd = *(u32 *)client->dev.platform_data;
		if (data->vdd > MCP3021_VDD_MAX ||
				data->vdd < MCP3021_VDD_MIN) {
			err = -EINVAL;
			goto exit_free;
		}
	} else
		data->vdd = MCP3021_VDD_REF;

	err = sysfs_create_file(&client->dev.kobj, &dev_attr_in0_input.attr);
	if (err)
		goto exit_free;
=======
	switch (id->driver_data) {
	case mcp3021:
		data->sar_shift = MCP3021_SAR_SHIFT;
		data->sar_mask = MCP3021_SAR_MASK;
		data->output_res = MCP3021_OUTPUT_RES;
		data->output_scale = MCP3021_OUTPUT_SCALE;
		break;

	case mcp3221:
		data->sar_shift = MCP3221_SAR_SHIFT;
		data->sar_mask = MCP3221_SAR_MASK;
		data->output_res = MCP3221_OUTPUT_RES;
		data->output_scale = MCP3221_OUTPUT_SCALE;
		break;
	}

	if (dev_get_platdata(&client->dev)) {
		data->vdd = *(u32 *)dev_get_platdata(&client->dev);
		if (data->vdd > MCP3021_VDD_MAX || data->vdd < MCP3021_VDD_MIN)
			return -EINVAL;
	} else {
		data->vdd = MCP3021_VDD_REF;
	}

	err = sysfs_create_file(&client->dev.kobj, &dev_attr_in0_input.attr);
	if (err)
		return err;
>>>>>>> refs/remotes/origin/master

	data->hwmon_dev = hwmon_device_register(&client->dev);
	if (IS_ERR(data->hwmon_dev)) {
		err = PTR_ERR(data->hwmon_dev);
		goto exit_remove;
	}

	return 0;

exit_remove:
	sysfs_remove_file(&client->dev.kobj, &dev_attr_in0_input.attr);
<<<<<<< HEAD
exit_free:
	kfree(data);
=======
>>>>>>> refs/remotes/origin/master
	return err;
}

static int mcp3021_remove(struct i2c_client *client)
{
	struct mcp3021_data *data = i2c_get_clientdata(client);

	hwmon_device_unregister(data->hwmon_dev);
	sysfs_remove_file(&client->dev.kobj, &dev_attr_in0_input.attr);
<<<<<<< HEAD
	kfree(data);
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}

static const struct i2c_device_id mcp3021_id[] = {
<<<<<<< HEAD
	{ "mcp3021", 0 },
=======
	{ "mcp3021", mcp3021 },
	{ "mcp3221", mcp3221 },
>>>>>>> refs/remotes/origin/master
	{ }
};
MODULE_DEVICE_TABLE(i2c, mcp3021_id);

static struct i2c_driver mcp3021_driver = {
	.driver = {
		.name = "mcp3021",
	},
	.probe = mcp3021_probe,
	.remove = mcp3021_remove,
	.id_table = mcp3021_id,
};

module_i2c_driver(mcp3021_driver);

MODULE_AUTHOR("Mingkai Hu <Mingkai.hu@freescale.com>");
<<<<<<< HEAD
MODULE_DESCRIPTION("Microchip MCP3021 driver");
=======
MODULE_DESCRIPTION("Microchip MCP3021/MCP3221 driver");
>>>>>>> refs/remotes/origin/master
MODULE_LICENSE("GPL");
