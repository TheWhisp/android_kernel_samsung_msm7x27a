/*
 * tps65217.c
 *
 * TPS65217 chip family multi-function driver
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/regmap.h>
#include <linux/err.h>
<<<<<<< HEAD
=======
#include <linux/of.h>
#include <linux/of_device.h>
>>>>>>> refs/remotes/origin/master

#include <linux/mfd/core.h>
#include <linux/mfd/tps65217.h>

<<<<<<< HEAD
=======
static const struct mfd_cell tps65217s[] = {
	{
		.name = "tps65217-pmic",
	},
	{
		.name = "tps65217-bl",
	},
};

>>>>>>> refs/remotes/origin/master
/**
 * tps65217_reg_read: Read a single tps65217 register.
 *
 * @tps: Device to read from.
 * @reg: Register to read.
 * @val: Contians the value
 */
int tps65217_reg_read(struct tps65217 *tps, unsigned int reg,
			unsigned int *val)
{
	return regmap_read(tps->regmap, reg, val);
}
EXPORT_SYMBOL_GPL(tps65217_reg_read);

/**
 * tps65217_reg_write: Write a single tps65217 register.
 *
 * @tps65217: Device to write to.
 * @reg: Register to write to.
 * @val: Value to write.
 * @level: Password protected level
 */
int tps65217_reg_write(struct tps65217 *tps, unsigned int reg,
			unsigned int val, unsigned int level)
{
	int ret;
	unsigned int xor_reg_val;

	switch (level) {
	case TPS65217_PROTECT_NONE:
		return regmap_write(tps->regmap, reg, val);
	case TPS65217_PROTECT_L1:
		xor_reg_val = reg ^ TPS65217_PASSWORD_REGS_UNLOCK;
		ret = regmap_write(tps->regmap, TPS65217_REG_PASSWORD,
							xor_reg_val);
		if (ret < 0)
			return ret;

		return regmap_write(tps->regmap, reg, val);
	case TPS65217_PROTECT_L2:
		xor_reg_val = reg ^ TPS65217_PASSWORD_REGS_UNLOCK;
		ret = regmap_write(tps->regmap, TPS65217_REG_PASSWORD,
							xor_reg_val);
		if (ret < 0)
			return ret;
		ret = regmap_write(tps->regmap, reg, val);
		if (ret < 0)
			return ret;
		ret = regmap_write(tps->regmap, TPS65217_REG_PASSWORD,
							xor_reg_val);
		if (ret < 0)
			return ret;
		return regmap_write(tps->regmap, reg, val);
	default:
		return -EINVAL;
	}
}
EXPORT_SYMBOL_GPL(tps65217_reg_write);

/**
 * tps65217_update_bits: Modify bits w.r.t mask, val and level.
 *
 * @tps65217: Device to write to.
 * @reg: Register to read-write to.
 * @mask: Mask.
 * @val: Value to write.
 * @level: Password protected level
 */
<<<<<<< HEAD
int tps65217_update_bits(struct tps65217 *tps, unsigned int reg,
=======
static int tps65217_update_bits(struct tps65217 *tps, unsigned int reg,
>>>>>>> refs/remotes/origin/master
		unsigned int mask, unsigned int val, unsigned int level)
{
	int ret;
	unsigned int data;

	ret = tps65217_reg_read(tps, reg, &data);
	if (ret) {
		dev_err(tps->dev, "Read from reg 0x%x failed\n", reg);
		return ret;
	}

	data &= ~mask;
	data |= val & mask;

	ret = tps65217_reg_write(tps, reg, data, level);
	if (ret)
		dev_err(tps->dev, "Write for reg 0x%x failed\n", reg);

	return ret;
}

int tps65217_set_bits(struct tps65217 *tps, unsigned int reg,
		unsigned int mask, unsigned int val, unsigned int level)
{
	return tps65217_update_bits(tps, reg, mask, val, level);
}
EXPORT_SYMBOL_GPL(tps65217_set_bits);

int tps65217_clear_bits(struct tps65217 *tps, unsigned int reg,
		unsigned int mask, unsigned int level)
{
	return tps65217_update_bits(tps, reg, mask, 0, level);
}
EXPORT_SYMBOL_GPL(tps65217_clear_bits);

static struct regmap_config tps65217_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
};

<<<<<<< HEAD
static int __devinit tps65217_probe(struct i2c_client *client,
				const struct i2c_device_id *ids)
{
	struct tps65217 *tps;
	struct tps65217_board *pdata = client->dev.platform_data;
	int i, ret;
	unsigned int version;
=======
static const struct of_device_id tps65217_of_match[] = {
	{ .compatible = "ti,tps65217", .data = (void *)TPS65217 },
	{ /* sentinel */ },
};

static int tps65217_probe(struct i2c_client *client,
				const struct i2c_device_id *ids)
{
	struct tps65217 *tps;
	unsigned int version;
	unsigned int chip_id = ids->driver_data;
	const struct of_device_id *match;
	bool status_off = false;
	int ret;

	if (client->dev.of_node) {
		match = of_match_device(tps65217_of_match, &client->dev);
		if (!match) {
			dev_err(&client->dev,
				"Failed to find matching dt id\n");
			return -EINVAL;
		}
		chip_id = (unsigned int)match->data;
		status_off = of_property_read_bool(client->dev.of_node,
					"ti,pmic-shutdown-controller");
	}

	if (!chip_id) {
		dev_err(&client->dev, "id is null.\n");
		return -ENODEV;
	}
>>>>>>> refs/remotes/origin/master

	tps = devm_kzalloc(&client->dev, sizeof(*tps), GFP_KERNEL);
	if (!tps)
		return -ENOMEM;

<<<<<<< HEAD
	tps->pdata = pdata;
	tps->regmap = regmap_init_i2c(client, &tps65217_regmap_config);
=======
	i2c_set_clientdata(client, tps);
	tps->dev = &client->dev;
	tps->id = chip_id;

	tps->regmap = devm_regmap_init_i2c(client, &tps65217_regmap_config);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(tps->regmap)) {
		ret = PTR_ERR(tps->regmap);
		dev_err(tps->dev, "Failed to allocate register map: %d\n",
			ret);
		return ret;
	}

<<<<<<< HEAD
	i2c_set_clientdata(client, tps);
	tps->dev = &client->dev;

	ret = tps65217_reg_read(tps, TPS65217_REG_CHIPID, &version);
	if (ret < 0) {
		dev_err(tps->dev, "Failed to read revision"
					" register: %d\n", ret);
		goto err_regmap;
=======
	ret = mfd_add_devices(tps->dev, -1, tps65217s,
			      ARRAY_SIZE(tps65217s), NULL, 0, NULL);
	if (ret < 0) {
		dev_err(tps->dev, "mfd_add_devices failed: %d\n", ret);
		return ret;
	}

	ret = tps65217_reg_read(tps, TPS65217_REG_CHIPID, &version);
	if (ret < 0) {
		dev_err(tps->dev, "Failed to read revision register: %d\n",
			ret);
		return ret;
	}

	/* Set the PMIC to shutdown on PWR_EN toggle */
	if (status_off) {
		ret = tps65217_set_bits(tps, TPS65217_REG_STATUS,
				TPS65217_STATUS_OFF, TPS65217_STATUS_OFF,
				TPS65217_PROTECT_NONE);
		if (ret)
			dev_warn(tps->dev, "unable to set the status OFF\n");
>>>>>>> refs/remotes/origin/master
	}

	dev_info(tps->dev, "TPS65217 ID %#x version 1.%d\n",
			(version & TPS65217_CHIPID_CHIP_MASK) >> 4,
			version & TPS65217_CHIPID_REV_MASK);

<<<<<<< HEAD
	for (i = 0; i < TPS65217_NUM_REGULATOR; i++) {
		struct platform_device *pdev;

		pdev = platform_device_alloc("tps65217-pmic", i);
		if (!pdev) {
			dev_err(tps->dev, "Cannot create regulator %d\n", i);
			continue;
		}

		pdev->dev.parent = tps->dev;
		platform_device_add_data(pdev, &pdata->tps65217_init_data[i],
					sizeof(pdata->tps65217_init_data[i]));
		tps->regulator_pdev[i] = pdev;

		platform_device_add(pdev);
	}

	return 0;

err_regmap:
	regmap_exit(tps->regmap);

	return ret;
}

static int __devexit tps65217_remove(struct i2c_client *client)
{
	struct tps65217 *tps = i2c_get_clientdata(client);
	int i;

	for (i = 0; i < TPS65217_NUM_REGULATOR; i++)
		platform_device_unregister(tps->regulator_pdev[i]);

	regmap_exit(tps->regmap);
=======
	return 0;
}

static int tps65217_remove(struct i2c_client *client)
{
	struct tps65217 *tps = i2c_get_clientdata(client);

	mfd_remove_devices(tps->dev);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static const struct i2c_device_id tps65217_id_table[] = {
<<<<<<< HEAD
	{"tps65217", 0xF0},
	{/* end of list */}
=======
	{"tps65217", TPS65217},
	{ /* sentinel */ }
>>>>>>> refs/remotes/origin/master
};
MODULE_DEVICE_TABLE(i2c, tps65217_id_table);

static struct i2c_driver tps65217_driver = {
	.driver		= {
		.name	= "tps65217",
<<<<<<< HEAD
	},
	.id_table	= tps65217_id_table,
	.probe		= tps65217_probe,
	.remove		= __devexit_p(tps65217_remove),
=======
		.owner	= THIS_MODULE,
		.of_match_table = tps65217_of_match,
	},
	.id_table	= tps65217_id_table,
	.probe		= tps65217_probe,
	.remove		= tps65217_remove,
>>>>>>> refs/remotes/origin/master
};

static int __init tps65217_init(void)
{
	return i2c_add_driver(&tps65217_driver);
}
subsys_initcall(tps65217_init);

static void __exit tps65217_exit(void)
{
	i2c_del_driver(&tps65217_driver);
}
module_exit(tps65217_exit);

MODULE_AUTHOR("AnilKumar Ch <anilkumar@ti.com>");
MODULE_DESCRIPTION("TPS65217 chip family multi-function driver");
MODULE_LICENSE("GPL v2");
