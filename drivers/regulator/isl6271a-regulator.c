/*
 * isl6271a-regulator.c
 *
 * Support for Intersil ISL6271A voltage regulator
 *
 * Copyright (C) 2010 Marek Vasut <marek.vasut@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any kind,
 * whether express or implied; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
#include <linux/i2c.h>
<<<<<<< HEAD
#include <linux/delay.h>
=======
>>>>>>> refs/remotes/origin/master
#include <linux/slab.h>

#define	ISL6271A_VOLTAGE_MIN	850000
#define	ISL6271A_VOLTAGE_MAX	1600000
#define	ISL6271A_VOLTAGE_STEP	50000

/* PMIC details */
struct isl_pmic {
	struct i2c_client	*client;
	struct regulator_dev	*rdev[3];
	struct mutex		mtx;
};

<<<<<<< HEAD
static int isl6271a_get_voltage(struct regulator_dev *dev)
{
	struct isl_pmic *pmic = rdev_get_drvdata(dev);
	int idx, data;
=======
static int isl6271a_get_voltage_sel(struct regulator_dev *dev)
{
	struct isl_pmic *pmic = rdev_get_drvdata(dev);
	int idx;
>>>>>>> refs/remotes/origin/master

	mutex_lock(&pmic->mtx);

	idx = i2c_smbus_read_byte(pmic->client);
<<<<<<< HEAD
	if (idx < 0) {
		dev_err(&pmic->client->dev, "Error getting voltage\n");
		data = idx;
		goto out;
	}

	/* Convert the data from chip to microvolts */
	data = ISL6271A_VOLTAGE_MIN + (ISL6271A_VOLTAGE_STEP * (idx & 0xf));

out:
	mutex_unlock(&pmic->mtx);
	return data;
}

static int isl6271a_set_voltage(struct regulator_dev *dev,
				int minuV, int maxuV,
				unsigned *selector)
{
	struct isl_pmic *pmic = rdev_get_drvdata(dev);
<<<<<<< HEAD
	int vsel, err, data;
=======
	int err, data;
>>>>>>> refs/remotes/origin/cm-10.0

	if (minuV < ISL6271A_VOLTAGE_MIN || minuV > ISL6271A_VOLTAGE_MAX)
		return -EINVAL;
	if (maxuV < ISL6271A_VOLTAGE_MIN || maxuV > ISL6271A_VOLTAGE_MAX)
		return -EINVAL;

<<<<<<< HEAD
	/* Align to 50000 mV */
	vsel = minuV - (minuV % ISL6271A_VOLTAGE_STEP);

	/* If the result fell out of [minuV,maxuV] range, put it back */
	if (vsel < minuV)
		vsel += ISL6271A_VOLTAGE_STEP;

	/* Convert the microvolts to data for the chip */
	data = (vsel - ISL6271A_VOLTAGE_MIN) / ISL6271A_VOLTAGE_STEP;

=======
	data = DIV_ROUND_UP(minuV - ISL6271A_VOLTAGE_MIN,
			    ISL6271A_VOLTAGE_STEP);
>>>>>>> refs/remotes/origin/cm-10.0
	*selector = data;

	mutex_lock(&pmic->mtx);

	err = i2c_smbus_write_byte(pmic->client, data);
=======
	if (idx < 0)
		dev_err(&pmic->client->dev, "Error getting voltage\n");

	mutex_unlock(&pmic->mtx);
	return idx;
}

static int isl6271a_set_voltage_sel(struct regulator_dev *dev,
				    unsigned selector)
{
	struct isl_pmic *pmic = rdev_get_drvdata(dev);
	int err;

	mutex_lock(&pmic->mtx);

	err = i2c_smbus_write_byte(pmic->client, selector);
>>>>>>> refs/remotes/origin/master
	if (err < 0)
		dev_err(&pmic->client->dev, "Error setting voltage\n");

	mutex_unlock(&pmic->mtx);
	return err;
}

<<<<<<< HEAD
static int isl6271a_list_voltage(struct regulator_dev *dev, unsigned selector)
{
	return ISL6271A_VOLTAGE_MIN + (ISL6271A_VOLTAGE_STEP * selector);
}

static struct regulator_ops isl_core_ops = {
	.get_voltage	= isl6271a_get_voltage,
	.set_voltage	= isl6271a_set_voltage,
	.list_voltage	= isl6271a_list_voltage,
};

static int isl6271a_get_fixed_voltage(struct regulator_dev *dev)
{
	int id = rdev_get_id(dev);
	return (id == 1) ? 1100000 : 1300000;
}

static int isl6271a_list_fixed_voltage(struct regulator_dev *dev, unsigned selector)
{
	int id = rdev_get_id(dev);
	return (id == 1) ? 1100000 : 1300000;
}

static struct regulator_ops isl_fixed_ops = {
	.get_voltage	= isl6271a_get_fixed_voltage,
	.list_voltage	= isl6271a_list_fixed_voltage,
};

static struct regulator_desc isl_rd[] = {
=======
static struct regulator_ops isl_core_ops = {
	.get_voltage_sel = isl6271a_get_voltage_sel,
	.set_voltage_sel = isl6271a_set_voltage_sel,
	.list_voltage	= regulator_list_voltage_linear,
	.map_voltage	= regulator_map_voltage_linear,
};

static struct regulator_ops isl_fixed_ops = {
	.list_voltage	= regulator_list_voltage_linear,
};

static const struct regulator_desc isl_rd[] = {
>>>>>>> refs/remotes/origin/master
	{
		.name		= "Core Buck",
		.id		= 0,
		.n_voltages	= 16,
		.ops		= &isl_core_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
<<<<<<< HEAD
=======
		.min_uV		= ISL6271A_VOLTAGE_MIN,
		.uV_step	= ISL6271A_VOLTAGE_STEP,
>>>>>>> refs/remotes/origin/master
	}, {
		.name		= "LDO1",
		.id		= 1,
		.n_voltages	= 1,
		.ops		= &isl_fixed_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
<<<<<<< HEAD
=======
		.min_uV		= 1100000,
>>>>>>> refs/remotes/origin/master
	}, {
		.name		= "LDO2",
		.id		= 2,
		.n_voltages	= 1,
		.ops		= &isl_fixed_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
<<<<<<< HEAD
	},
};

static int __devinit isl6271a_probe(struct i2c_client *i2c,
				     const struct i2c_device_id *id)
{
	struct regulator_init_data *init_data	= i2c->dev.platform_data;
	struct isl_pmic *pmic;
	int err, i;
=======
		.min_uV		= 1300000,
	},
};

static int isl6271a_probe(struct i2c_client *i2c,
				     const struct i2c_device_id *id)
{
	struct regulator_config config = { };
	struct regulator_init_data *init_data	= dev_get_platdata(&i2c->dev);
	struct isl_pmic *pmic;
	int i;
>>>>>>> refs/remotes/origin/master

	if (!i2c_check_functionality(i2c->adapter, I2C_FUNC_SMBUS_BYTE_DATA))
		return -EIO;

<<<<<<< HEAD
	if (!init_data) {
		dev_err(&i2c->dev, "no platform data supplied\n");
		return -EIO;
	}

	pmic = kzalloc(sizeof(struct isl_pmic), GFP_KERNEL);
=======
	pmic = devm_kzalloc(&i2c->dev, sizeof(struct isl_pmic), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!pmic)
		return -ENOMEM;

	pmic->client = i2c;

	mutex_init(&pmic->mtx);

	for (i = 0; i < 3; i++) {
<<<<<<< HEAD
		pmic->rdev[i] = regulator_register(&isl_rd[i], &i2c->dev,
<<<<<<< HEAD
						init_data, pmic);
=======
						init_data, pmic, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
		if (IS_ERR(pmic->rdev[i])) {
			dev_err(&i2c->dev, "failed to register %s\n", id->name);
			err = PTR_ERR(pmic->rdev[i]);
			goto error;
=======
		config.dev = &i2c->dev;
		if (i == 0)
			config.init_data = init_data;
		else
			config.init_data = NULL;
		config.driver_data = pmic;

		pmic->rdev[i] = devm_regulator_register(&i2c->dev, &isl_rd[i],
							&config);
		if (IS_ERR(pmic->rdev[i])) {
			dev_err(&i2c->dev, "failed to register %s\n", id->name);
			return PTR_ERR(pmic->rdev[i]);
>>>>>>> refs/remotes/origin/master
		}
	}

	i2c_set_clientdata(i2c, pmic);

	return 0;
<<<<<<< HEAD

error:
	while (--i >= 0)
		regulator_unregister(pmic->rdev[i]);

	kfree(pmic);
	return err;
}

static int __devexit isl6271a_remove(struct i2c_client *i2c)
{
	struct isl_pmic *pmic = i2c_get_clientdata(i2c);
	int i;

	for (i = 0; i < 3; i++)
		regulator_unregister(pmic->rdev[i]);

	kfree(pmic);

	return 0;
=======
>>>>>>> refs/remotes/origin/master
}

static const struct i2c_device_id isl6271a_id[] = {
	{.name = "isl6271a", 0 },
	{ },
};

MODULE_DEVICE_TABLE(i2c, isl6271a_id);

static struct i2c_driver isl6271a_i2c_driver = {
	.driver = {
		.name = "isl6271a",
		.owner = THIS_MODULE,
	},
	.probe = isl6271a_probe,
<<<<<<< HEAD
	.remove = __devexit_p(isl6271a_remove),
=======
>>>>>>> refs/remotes/origin/master
	.id_table = isl6271a_id,
};

static int __init isl6271a_init(void)
{
	return i2c_add_driver(&isl6271a_i2c_driver);
}

static void __exit isl6271a_cleanup(void)
{
	i2c_del_driver(&isl6271a_i2c_driver);
}

subsys_initcall(isl6271a_init);
module_exit(isl6271a_cleanup);

MODULE_AUTHOR("Marek Vasut <marek.vasut@gmail.com>");
MODULE_DESCRIPTION("Intersil ISL6271A voltage regulator driver");
MODULE_LICENSE("GPL v2");
