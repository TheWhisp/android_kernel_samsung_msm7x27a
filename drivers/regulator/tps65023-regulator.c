/*
 * tps65023-regulator.c
 *
 * Supports TPS65023 Regulator
 *
 * Copyright (C) 2009 Texas Instrument Incorporated - http://www.ti.com/
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
#include <linux/regulator/machine.h>
#include <linux/i2c.h>
<<<<<<< HEAD
#include <linux/delay.h>
#include <linux/slab.h>
<<<<<<< HEAD
=======
#include <linux/regmap.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/slab.h>
#include <linux/regmap.h>
>>>>>>> refs/remotes/origin/master

/* Register definitions */
#define	TPS65023_REG_VERSION		0
#define	TPS65023_REG_PGOODZ		1
#define	TPS65023_REG_MASK		2
#define	TPS65023_REG_REG_CTRL		3
#define	TPS65023_REG_CON_CTRL		4
#define	TPS65023_REG_CON_CTRL2		5
#define	TPS65023_REG_DEF_CORE		6
#define	TPS65023_REG_DEFSLEW		7
#define	TPS65023_REG_LDO_CTRL		8

/* PGOODZ bitfields */
#define	TPS65023_PGOODZ_PWRFAILZ	BIT(7)
#define	TPS65023_PGOODZ_LOWBATTZ	BIT(6)
#define	TPS65023_PGOODZ_VDCDC1		BIT(5)
#define	TPS65023_PGOODZ_VDCDC2		BIT(4)
#define	TPS65023_PGOODZ_VDCDC3		BIT(3)
#define	TPS65023_PGOODZ_LDO2		BIT(2)
#define	TPS65023_PGOODZ_LDO1		BIT(1)

/* MASK bitfields */
#define	TPS65023_MASK_PWRFAILZ		BIT(7)
#define	TPS65023_MASK_LOWBATTZ		BIT(6)
#define	TPS65023_MASK_VDCDC1		BIT(5)
#define	TPS65023_MASK_VDCDC2		BIT(4)
#define	TPS65023_MASK_VDCDC3		BIT(3)
#define	TPS65023_MASK_LDO2		BIT(2)
#define	TPS65023_MASK_LDO1		BIT(1)

/* REG_CTRL bitfields */
#define TPS65023_REG_CTRL_VDCDC1_EN	BIT(5)
#define TPS65023_REG_CTRL_VDCDC2_EN	BIT(4)
#define TPS65023_REG_CTRL_VDCDC3_EN	BIT(3)
#define TPS65023_REG_CTRL_LDO2_EN	BIT(2)
#define TPS65023_REG_CTRL_LDO1_EN	BIT(1)

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
/* REG_CTRL2 bitfields */
#define TPS65023_REG_CTRL2_GO		BIT(7)
#define TPS65023_REG_CTRL2_CORE_ADJ	BIT(6)
#define TPS65023_REG_CTRL2_DCDC2	BIT(2)
#define TPS65023_REG_CTRL2_DCDC1	BIT(1)
#define TPS65023_REG_CTRL2_DCDC3	BIT(0)

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
/* LDO_CTRL bitfields */
#define TPS65023_LDO_CTRL_LDOx_SHIFT(ldo_id)	((ldo_id)*4)
#define TPS65023_LDO_CTRL_LDOx_MASK(ldo_id)	(0xF0 >> ((ldo_id)*4))

=======
>>>>>>> refs/remotes/origin/master
/* Number of step-down converters available */
#define TPS65023_NUM_DCDC		3
/* Number of LDO voltage regulators  available */
#define TPS65023_NUM_LDO		2
/* Number of total regulators available */
#define TPS65023_NUM_REGULATOR	(TPS65023_NUM_DCDC + TPS65023_NUM_LDO)

/* DCDCs */
#define TPS65023_DCDC_1			0
#define TPS65023_DCDC_2			1
#define TPS65023_DCDC_3			2
/* LDOs */
#define TPS65023_LDO_1			3
#define TPS65023_LDO_2			4

#define TPS65023_MAX_REG_ID		TPS65023_LDO_2

/* Supported voltage values for regulators */
<<<<<<< HEAD
<<<<<<< HEAD
static const u16 VDCDC1_VSEL_table[] = {
=======
static const u16 VCORE_VSEL_table[] = {
>>>>>>> refs/remotes/origin/cm-10.0
	800, 825, 850, 875,
	900, 925, 950, 975,
	1000, 1025, 1050, 1075,
	1100, 1125, 1150, 1175,
	1200, 1225, 1250, 1275,
	1300, 1325, 1350, 1375,
	1400, 1425, 1450, 1475,
	1500, 1525, 1550, 1600,
};

<<<<<<< HEAD
static const u16 LDO1_VSEL_table[] = {
=======
/* Supported voltage values for LDO regulators for tps65020 */
static const u16 TPS65020_LDO1_VSEL_table[] = {
	1000, 1050, 1100, 1300,
	1800, 2500, 3000, 3300,
};

static const u16 TPS65020_LDO2_VSEL_table[] = {
	1000, 1050, 1100, 1300,
	1800, 2500, 3000, 3300,
=======
static const unsigned int VCORE_VSEL_table[] = {
	800000, 825000, 850000, 875000,
	900000, 925000, 950000, 975000,
	1000000, 1025000, 1050000, 1075000,
	1100000, 1125000, 1150000, 1175000,
	1200000, 1225000, 1250000, 1275000,
	1300000, 1325000, 1350000, 1375000,
	1400000, 1425000, 1450000, 1475000,
	1500000, 1525000, 1550000, 1600000,
};

static const unsigned int DCDC_FIXED_3300000_VSEL_table[] = {
	3300000,
};

static const unsigned int DCDC_FIXED_1800000_VSEL_table[] = {
	1800000,
};

/* Supported voltage values for LDO regulators for tps65020 */
static const unsigned int TPS65020_LDO_VSEL_table[] = {
	1000000, 1050000, 1100000, 1300000,
	1800000, 2500000, 3000000, 3300000,
>>>>>>> refs/remotes/origin/master
};

/* Supported voltage values for LDO regulators
 * for tps65021 and tps65023 */
<<<<<<< HEAD
static const u16 TPS65023_LDO1_VSEL_table[] = {
>>>>>>> refs/remotes/origin/cm-10.0
	1000, 1100, 1300, 1800,
	2200, 2600, 2800, 3150,
};

<<<<<<< HEAD
static const u16 LDO2_VSEL_table[] = {
=======
static const u16 TPS65023_LDO2_VSEL_table[] = {
>>>>>>> refs/remotes/origin/cm-10.0
	1050, 1200, 1300, 1800,
	2500, 2800, 3000, 3300,
};

<<<<<<< HEAD
static unsigned int num_voltages[] = {ARRAY_SIZE(VDCDC1_VSEL_table),
				0, 0, ARRAY_SIZE(LDO1_VSEL_table),
				ARRAY_SIZE(LDO2_VSEL_table)};

=======
>>>>>>> refs/remotes/origin/cm-10.0
/* Regulator specific details */
struct tps_info {
	const char *name;
	unsigned min_uV;
	unsigned max_uV;
	bool fixed;
	u8 table_len;
	const u16 *table;
=======
static const unsigned int TPS65023_LDO1_VSEL_table[] = {
	1000000, 1100000, 1300000, 1800000,
	2200000, 2600000, 2800000, 3150000,
};

static const unsigned int TPS65023_LDO2_VSEL_table[] = {
	1050000, 1200000, 1300000, 1800000,
	2500000, 2800000, 3000000, 3300000,
};

/* Regulator specific details */
struct tps_info {
	const char *name;
	u8 table_len;
	const unsigned int *table;
>>>>>>> refs/remotes/origin/master
};

/* PMIC details */
struct tps_pmic {
	struct regulator_desc desc[TPS65023_NUM_REGULATOR];
<<<<<<< HEAD
	struct i2c_client *client;
	struct regulator_dev *rdev[TPS65023_NUM_REGULATOR];
	const struct tps_info *info[TPS65023_NUM_REGULATOR];
<<<<<<< HEAD
	struct mutex io_lock;
};

static inline int tps_65023_read(struct tps_pmic *tps, u8 reg)
{
	return i2c_smbus_read_byte_data(tps->client, reg);
}

static inline int tps_65023_write(struct tps_pmic *tps, u8 reg, u8 val)
{
	return i2c_smbus_write_byte_data(tps->client, reg, val);
}

static int tps_65023_set_bits(struct tps_pmic *tps, u8 reg, u8 mask)
{
	int err, data;

	mutex_lock(&tps->io_lock);

	data = tps_65023_read(tps, reg);
	if (data < 0) {
		dev_err(&tps->client->dev, "Read from reg 0x%x failed\n", reg);
		err = data;
		goto out;
	}

	data |= mask;
	err = tps_65023_write(tps, reg, data);
	if (err)
		dev_err(&tps->client->dev, "Write for reg 0x%x failed\n", reg);

out:
	mutex_unlock(&tps->io_lock);
	return err;
}

static int tps_65023_clear_bits(struct tps_pmic *tps, u8 reg, u8 mask)
{
	int err, data;

	mutex_lock(&tps->io_lock);

	data = tps_65023_read(tps, reg);
	if (data < 0) {
		dev_err(&tps->client->dev, "Read from reg 0x%x failed\n", reg);
		err = data;
		goto out;
	}

	data &= ~mask;

	err = tps_65023_write(tps, reg, data);
	if (err)
		dev_err(&tps->client->dev, "Write for reg 0x%x failed\n", reg);

out:
	mutex_unlock(&tps->io_lock);
	return err;

}

static int tps_65023_reg_read(struct tps_pmic *tps, u8 reg)
{
	int data;

	mutex_lock(&tps->io_lock);

	data = tps_65023_read(tps, reg);
	if (data < 0)
		dev_err(&tps->client->dev, "Read from reg 0x%x failed\n", reg);

	mutex_unlock(&tps->io_lock);
	return data;
}

static int tps_65023_reg_write(struct tps_pmic *tps, u8 reg, u8 val)
{
	int err;

	mutex_lock(&tps->io_lock);

	err = tps_65023_write(tps, reg, val);
	if (err < 0)
		dev_err(&tps->client->dev, "Write for reg 0x%x failed\n", reg);

	mutex_unlock(&tps->io_lock);
	return err;
}
=======
=======
	struct regulator_dev *rdev[TPS65023_NUM_REGULATOR];
	const struct tps_info *info[TPS65023_NUM_REGULATOR];
>>>>>>> refs/remotes/origin/master
	struct regmap *regmap;
	u8 core_regulator;
};

/* Struct passed as driver data */
struct tps_driver_data {
	const struct tps_info *info;
	u8 core_regulator;
};
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

static int tps65023_dcdc_is_enabled(struct regulator_dev *dev)
{
	struct tps_pmic *tps = rdev_get_drvdata(dev);
	int data, dcdc = rdev_get_id(dev);
<<<<<<< HEAD
=======
	int ret;
>>>>>>> refs/remotes/origin/cm-10.0
	u8 shift;

	if (dcdc < TPS65023_DCDC_1 || dcdc > TPS65023_DCDC_3)
		return -EINVAL;

	shift = TPS65023_NUM_REGULATOR - dcdc;
<<<<<<< HEAD
	data = tps_65023_reg_read(tps, TPS65023_REG_REG_CTRL);

	if (data < 0)
		return data;
=======
	ret = regmap_read(tps->regmap, TPS65023_REG_REG_CTRL, &data);

	if (ret != 0)
		return ret;
>>>>>>> refs/remotes/origin/cm-10.0
	else
		return (data & 1<<shift) ? 1 : 0;
}

static int tps65023_ldo_is_enabled(struct regulator_dev *dev)
{
	struct tps_pmic *tps = rdev_get_drvdata(dev);
	int data, ldo = rdev_get_id(dev);
<<<<<<< HEAD
=======
	int ret;
>>>>>>> refs/remotes/origin/cm-10.0
	u8 shift;

	if (ldo < TPS65023_LDO_1 || ldo > TPS65023_LDO_2)
		return -EINVAL;

	shift = (ldo == TPS65023_LDO_1 ? 1 : 2);
<<<<<<< HEAD
	data = tps_65023_reg_read(tps, TPS65023_REG_REG_CTRL);

	if (data < 0)
		return data;
=======
	ret = regmap_read(tps->regmap, TPS65023_REG_REG_CTRL, &data);

	if (ret != 0)
		return ret;
>>>>>>> refs/remotes/origin/cm-10.0
	else
		return (data & 1<<shift) ? 1 : 0;
}

static int tps65023_dcdc_enable(struct regulator_dev *dev)
{
	struct tps_pmic *tps = rdev_get_drvdata(dev);
	int dcdc = rdev_get_id(dev);
	u8 shift;
=======

static int tps65023_dcdc_get_voltage_sel(struct regulator_dev *dev)
{
	struct tps_pmic *tps = rdev_get_drvdata(dev);
	int dcdc = rdev_get_id(dev);
>>>>>>> refs/remotes/origin/master

	if (dcdc < TPS65023_DCDC_1 || dcdc > TPS65023_DCDC_3)
		return -EINVAL;

<<<<<<< HEAD
	shift = TPS65023_NUM_REGULATOR - dcdc;
<<<<<<< HEAD
	return tps_65023_set_bits(tps, TPS65023_REG_REG_CTRL, 1 << shift);
=======
	return regmap_update_bits(tps->regmap, TPS65023_REG_REG_CTRL, 1 << shift, 1 << shift);
>>>>>>> refs/remotes/origin/cm-10.0
}

static int tps65023_dcdc_disable(struct regulator_dev *dev)
{
	struct tps_pmic *tps = rdev_get_drvdata(dev);
	int dcdc = rdev_get_id(dev);
	u8 shift;

	if (dcdc < TPS65023_DCDC_1 || dcdc > TPS65023_DCDC_3)
		return -EINVAL;

	shift = TPS65023_NUM_REGULATOR - dcdc;
<<<<<<< HEAD
	return tps_65023_clear_bits(tps, TPS65023_REG_REG_CTRL, 1 << shift);
=======
	return regmap_update_bits(tps->regmap, TPS65023_REG_REG_CTRL, 1 << shift, 0);
>>>>>>> refs/remotes/origin/cm-10.0
}

static int tps65023_ldo_enable(struct regulator_dev *dev)
{
	struct tps_pmic *tps = rdev_get_drvdata(dev);
	int ldo = rdev_get_id(dev);
	u8 shift;

	if (ldo < TPS65023_LDO_1 || ldo > TPS65023_LDO_2)
		return -EINVAL;

	shift = (ldo == TPS65023_LDO_1 ? 1 : 2);
<<<<<<< HEAD
	return tps_65023_set_bits(tps, TPS65023_REG_REG_CTRL, 1 << shift);
=======
	return regmap_update_bits(tps->regmap, TPS65023_REG_REG_CTRL, 1 << shift, 1 << shift);
>>>>>>> refs/remotes/origin/cm-10.0
}

static int tps65023_ldo_disable(struct regulator_dev *dev)
{
	struct tps_pmic *tps = rdev_get_drvdata(dev);
	int ldo = rdev_get_id(dev);
	u8 shift;

	if (ldo < TPS65023_LDO_1 || ldo > TPS65023_LDO_2)
		return -EINVAL;

	shift = (ldo == TPS65023_LDO_1 ? 1 : 2);
<<<<<<< HEAD
	return tps_65023_clear_bits(tps, TPS65023_REG_REG_CTRL, 1 << shift);
=======
	return regmap_update_bits(tps->regmap, TPS65023_REG_REG_CTRL, 1 << shift, 0);
>>>>>>> refs/remotes/origin/cm-10.0
}

static int tps65023_dcdc_get_voltage(struct regulator_dev *dev)
{
	struct tps_pmic *tps = rdev_get_drvdata(dev);
<<<<<<< HEAD
=======
	int ret;
>>>>>>> refs/remotes/origin/cm-10.0
	int data, dcdc = rdev_get_id(dev);

	if (dcdc < TPS65023_DCDC_1 || dcdc > TPS65023_DCDC_3)
		return -EINVAL;

<<<<<<< HEAD
	if (dcdc == TPS65023_DCDC_1) {
		data = tps_65023_reg_read(tps, TPS65023_REG_DEF_CORE);
		if (data < 0)
			return data;
=======
	if (dcdc == tps->core_regulator) {
		ret = regmap_read(tps->regmap, TPS65023_REG_DEF_CORE, &data);
		if (ret != 0)
			return ret;
>>>>>>> refs/remotes/origin/cm-10.0
		data &= (tps->info[dcdc]->table_len - 1);
		return tps->info[dcdc]->table[data] * 1000;
	} else
		return tps->info[dcdc]->min_uV;
}

static int tps65023_dcdc_set_voltage(struct regulator_dev *dev,
				     int min_uV, int max_uV,
				     unsigned *selector)
{
	struct tps_pmic *tps = rdev_get_drvdata(dev);
	int dcdc = rdev_get_id(dev);
	int vsel;
<<<<<<< HEAD

	if (dcdc != TPS65023_DCDC_1)
		return -EINVAL;

=======
	int ret;

	if (dcdc != tps->core_regulator)
		return -EINVAL;
>>>>>>> refs/remotes/origin/cm-10.0
	if (min_uV < tps->info[dcdc]->min_uV
			|| min_uV > tps->info[dcdc]->max_uV)
		return -EINVAL;
	if (max_uV < tps->info[dcdc]->min_uV
			|| max_uV > tps->info[dcdc]->max_uV)
		return -EINVAL;

	for (vsel = 0; vsel < tps->info[dcdc]->table_len; vsel++) {
		int mV = tps->info[dcdc]->table[vsel];
		int uV = mV * 1000;

		/* Break at the first in-range value */
		if (min_uV <= uV && uV <= max_uV)
			break;
	}

	*selector = vsel;

<<<<<<< HEAD
	/* write to the register in case we found a match */
	if (vsel == tps->info[dcdc]->table_len)
		return -EINVAL;
	else
		return tps_65023_reg_write(tps, TPS65023_REG_DEF_CORE, vsel);
=======
	if (vsel == tps->info[dcdc]->table_len)
		goto failed;

	ret = regmap_write(tps->regmap, TPS65023_REG_DEF_CORE, vsel);

	/* Tell the chip that we have changed the value in DEFCORE
	 * and its time to update the core voltage
	 */
	regmap_update_bits(tps->regmap, TPS65023_REG_CON_CTRL2,
			TPS65023_REG_CTRL2_GO, TPS65023_REG_CTRL2_GO);

	return ret;

failed:
	return -EINVAL;
>>>>>>> refs/remotes/origin/cm-10.0
}

static int tps65023_ldo_get_voltage(struct regulator_dev *dev)
{
	struct tps_pmic *tps = rdev_get_drvdata(dev);
	int data, ldo = rdev_get_id(dev);
<<<<<<< HEAD
=======
	int ret;
>>>>>>> refs/remotes/origin/cm-10.0

	if (ldo < TPS65023_LDO_1 || ldo > TPS65023_LDO_2)
		return -EINVAL;

<<<<<<< HEAD
	data = tps_65023_reg_read(tps, TPS65023_REG_LDO_CTRL);
	if (data < 0)
		return data;
=======
	ret = regmap_read(tps->regmap, TPS65023_REG_LDO_CTRL, &data);
	if (ret != 0)
		return ret;
>>>>>>> refs/remotes/origin/cm-10.0

	data >>= (TPS65023_LDO_CTRL_LDOx_SHIFT(ldo - TPS65023_LDO_1));
	data &= (tps->info[ldo]->table_len - 1);
	return tps->info[ldo]->table[data] * 1000;
}

static int tps65023_ldo_set_voltage(struct regulator_dev *dev,
				    int min_uV, int max_uV, unsigned *selector)
{
	struct tps_pmic *tps = rdev_get_drvdata(dev);
	int data, vsel, ldo = rdev_get_id(dev);
<<<<<<< HEAD
=======
	int ret;
>>>>>>> refs/remotes/origin/cm-10.0

	if (ldo < TPS65023_LDO_1 || ldo > TPS65023_LDO_2)
		return -EINVAL;

	if (min_uV < tps->info[ldo]->min_uV || min_uV > tps->info[ldo]->max_uV)
		return -EINVAL;
	if (max_uV < tps->info[ldo]->min_uV || max_uV > tps->info[ldo]->max_uV)
		return -EINVAL;

	for (vsel = 0; vsel < tps->info[ldo]->table_len; vsel++) {
		int mV = tps->info[ldo]->table[vsel];
		int uV = mV * 1000;

		/* Break at the first in-range value */
		if (min_uV <= uV && uV <= max_uV)
			break;
	}

	if (vsel == tps->info[ldo]->table_len)
		return -EINVAL;

	*selector = vsel;

<<<<<<< HEAD
	data = tps_65023_reg_read(tps, TPS65023_REG_LDO_CTRL);
	if (data < 0)
		return data;

	data &= TPS65023_LDO_CTRL_LDOx_MASK(ldo - TPS65023_LDO_1);
	data |= (vsel << (TPS65023_LDO_CTRL_LDOx_SHIFT(ldo - TPS65023_LDO_1)));
	return tps_65023_reg_write(tps, TPS65023_REG_LDO_CTRL, data);
=======
	ret = regmap_read(tps->regmap, TPS65023_REG_LDO_CTRL, &data);
	if (ret != 0)
		return ret;

	data &= TPS65023_LDO_CTRL_LDOx_MASK(ldo - TPS65023_LDO_1);
	data |= (vsel << (TPS65023_LDO_CTRL_LDOx_SHIFT(ldo - TPS65023_LDO_1)));
	return regmap_write(tps->regmap, TPS65023_REG_LDO_CTRL, data);
>>>>>>> refs/remotes/origin/cm-10.0
}

static int tps65023_dcdc_list_voltage(struct regulator_dev *dev,
					unsigned selector)
=======
	if (dcdc != tps->core_regulator)
		return 0;

	return regulator_get_voltage_sel_regmap(dev);
}

static int tps65023_dcdc_set_voltage_sel(struct regulator_dev *dev,
					 unsigned selector)
>>>>>>> refs/remotes/origin/master
{
	struct tps_pmic *tps = rdev_get_drvdata(dev);
	int dcdc = rdev_get_id(dev);

<<<<<<< HEAD
	if (dcdc < TPS65023_DCDC_1 || dcdc > TPS65023_DCDC_3)
		return -EINVAL;

<<<<<<< HEAD
	if (dcdc == TPS65023_DCDC_1) {
=======
	if (dcdc == tps->core_regulator) {
>>>>>>> refs/remotes/origin/cm-10.0
		if (selector >= tps->info[dcdc]->table_len)
			return -EINVAL;
		else
			return tps->info[dcdc]->table[selector] * 1000;
	} else
		return tps->info[dcdc]->min_uV;
}

static int tps65023_ldo_list_voltage(struct regulator_dev *dev,
					unsigned selector)
{
	struct tps_pmic *tps = rdev_get_drvdata(dev);
	int ldo = rdev_get_id(dev);

	if (ldo < TPS65023_LDO_1 || ldo > TPS65023_LDO_2)
		return -EINVAL;

	if (selector >= tps->info[ldo]->table_len)
		return -EINVAL;
	else
		return tps->info[ldo]->table[selector] * 1000;
=======
	if (dcdc != tps->core_regulator)
		return -EINVAL;

	return regulator_set_voltage_sel_regmap(dev, selector);
>>>>>>> refs/remotes/origin/master
}

/* Operations permitted on VDCDCx */
static struct regulator_ops tps65023_dcdc_ops = {
<<<<<<< HEAD
	.is_enabled = tps65023_dcdc_is_enabled,
	.enable = tps65023_dcdc_enable,
	.disable = tps65023_dcdc_disable,
	.get_voltage = tps65023_dcdc_get_voltage,
	.set_voltage = tps65023_dcdc_set_voltage,
	.list_voltage = tps65023_dcdc_list_voltage,
=======
	.is_enabled = regulator_is_enabled_regmap,
	.enable = regulator_enable_regmap,
	.disable = regulator_disable_regmap,
	.get_voltage_sel = tps65023_dcdc_get_voltage_sel,
	.set_voltage_sel = tps65023_dcdc_set_voltage_sel,
	.list_voltage = regulator_list_voltage_table,
	.map_voltage = regulator_map_voltage_ascend,
>>>>>>> refs/remotes/origin/master
};

/* Operations permitted on LDOx */
static struct regulator_ops tps65023_ldo_ops = {
<<<<<<< HEAD
	.is_enabled = tps65023_ldo_is_enabled,
	.enable = tps65023_ldo_enable,
	.disable = tps65023_ldo_disable,
	.get_voltage = tps65023_ldo_get_voltage,
	.set_voltage = tps65023_ldo_set_voltage,
	.list_voltage = tps65023_ldo_list_voltage,
};

<<<<<<< HEAD
static int __devinit tps_65023_probe(struct i2c_client *client,
				     const struct i2c_device_id *id)
{
	const struct tps_info *info = (void *)id->driver_data;
=======
=======
	.is_enabled = regulator_is_enabled_regmap,
	.enable = regulator_enable_regmap,
	.disable = regulator_disable_regmap,
	.get_voltage_sel = regulator_get_voltage_sel_regmap,
	.set_voltage_sel = regulator_set_voltage_sel_regmap,
	.list_voltage = regulator_list_voltage_table,
	.map_voltage = regulator_map_voltage_ascend,
};

>>>>>>> refs/remotes/origin/master
static struct regmap_config tps65023_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
};

<<<<<<< HEAD
static int __devinit tps_65023_probe(struct i2c_client *client,
=======
static int tps_65023_probe(struct i2c_client *client,
>>>>>>> refs/remotes/origin/master
				     const struct i2c_device_id *id)
{
	const struct tps_driver_data *drv_data = (void *)id->driver_data;
	const struct tps_info *info = drv_data->info;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct regulator_config config = { };
>>>>>>> refs/remotes/origin/master
	struct regulator_init_data *init_data;
	struct regulator_dev *rdev;
	struct tps_pmic *tps;
	int i;
	int error;

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_BYTE_DATA))
		return -EIO;

	/**
	 * init_data points to array of regulator_init structures
	 * coming from the board-evm file.
	 */
<<<<<<< HEAD
	init_data = client->dev.platform_data;
	if (!init_data)
		return -EIO;

	tps = kzalloc(sizeof(*tps), GFP_KERNEL);
	if (!tps)
		return -ENOMEM;

<<<<<<< HEAD
	mutex_init(&tps->io_lock);

	/* common for all regulators */
	tps->client = client;
=======
	tps->regmap = regmap_init_i2c(client, &tps65023_regmap_config);
=======
	init_data = dev_get_platdata(&client->dev);
	if (!init_data)
		return -EIO;

	tps = devm_kzalloc(&client->dev, sizeof(*tps), GFP_KERNEL);
	if (!tps)
		return -ENOMEM;

	tps->regmap = devm_regmap_init_i2c(client, &tps65023_regmap_config);
>>>>>>> refs/remotes/origin/master
	if (IS_ERR(tps->regmap)) {
		error = PTR_ERR(tps->regmap);
		dev_err(&client->dev, "Failed to allocate register map: %d\n",
			error);
<<<<<<< HEAD
		goto fail_alloc;
	}

	/* common for all regulators */
	tps->client = client;
	tps->core_regulator = drv_data->core_regulator;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		return error;
	}

	/* common for all regulators */
	tps->core_regulator = drv_data->core_regulator;
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < TPS65023_NUM_REGULATOR; i++, info++, init_data++) {
		/* Store regulator specific information */
		tps->info[i] = info;

		tps->desc[i].name = info->name;
		tps->desc[i].id = i;
<<<<<<< HEAD
<<<<<<< HEAD
		tps->desc[i].n_voltages = num_voltages[i];
=======
		tps->desc[i].n_voltages = info->table_len;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		tps->desc[i].n_voltages = info->table_len;
		tps->desc[i].volt_table = info->table;
>>>>>>> refs/remotes/origin/master
		tps->desc[i].ops = (i > TPS65023_DCDC_3 ?
					&tps65023_ldo_ops : &tps65023_dcdc_ops);
		tps->desc[i].type = REGULATOR_VOLTAGE;
		tps->desc[i].owner = THIS_MODULE;

<<<<<<< HEAD
		/* Register the regulators */
		rdev = regulator_register(&tps->desc[i], &client->dev,
<<<<<<< HEAD
					  init_data, tps);
=======
					  init_data, tps, NULL);
>>>>>>> refs/remotes/origin/cm-10.0
		if (IS_ERR(rdev)) {
			dev_err(&client->dev, "failed to register %s\n",
				id->name);
			error = PTR_ERR(rdev);
			goto fail;
=======
		tps->desc[i].enable_reg = TPS65023_REG_REG_CTRL;
		switch (i) {
		case TPS65023_LDO_1:
			tps->desc[i].vsel_reg = TPS65023_REG_LDO_CTRL;
			tps->desc[i].vsel_mask = 0x07;
			tps->desc[i].enable_mask = 1 << 1;
			break;
		case TPS65023_LDO_2:
			tps->desc[i].vsel_reg = TPS65023_REG_LDO_CTRL;
			tps->desc[i].vsel_mask = 0x70;
			tps->desc[i].enable_mask = 1 << 2;
			break;
		default: /* DCDCx */
			tps->desc[i].enable_mask =
					1 << (TPS65023_NUM_REGULATOR - i);
			tps->desc[i].vsel_reg = TPS65023_REG_DEF_CORE;
			tps->desc[i].vsel_mask = info->table_len - 1;
			tps->desc[i].apply_reg = TPS65023_REG_CON_CTRL2;
			tps->desc[i].apply_bit = TPS65023_REG_CTRL2_GO;
		}

		config.dev = &client->dev;
		config.init_data = init_data;
		config.driver_data = tps;
		config.regmap = tps->regmap;

		/* Register the regulators */
		rdev = devm_regulator_register(&client->dev, &tps->desc[i],
					       &config);
		if (IS_ERR(rdev)) {
			dev_err(&client->dev, "failed to register %s\n",
				id->name);
			return PTR_ERR(rdev);
>>>>>>> refs/remotes/origin/master
		}

		/* Save regulator for cleanup */
		tps->rdev[i] = rdev;
	}

	i2c_set_clientdata(client, tps);

<<<<<<< HEAD
<<<<<<< HEAD
=======
	/* Enable setting output voltage by I2C */
	regmap_update_bits(tps->regmap, TPS65023_REG_CON_CTRL2,
			TPS65023_REG_CTRL2_CORE_ADJ, TPS65023_REG_CTRL2_CORE_ADJ);

>>>>>>> refs/remotes/origin/cm-10.0
	return 0;

 fail:
	while (--i >= 0)
		regulator_unregister(tps->rdev[i]);

<<<<<<< HEAD
=======
	regmap_exit(tps->regmap);
 fail_alloc:
>>>>>>> refs/remotes/origin/cm-10.0
	kfree(tps);
	return error;
}

/**
 * tps_65023_remove - TPS65023 driver i2c remove handler
 * @client: i2c driver client device structure
 *
 * Unregister TPS driver as an i2c client device driver
 */
static int __devexit tps_65023_remove(struct i2c_client *client)
{
	struct tps_pmic *tps = i2c_get_clientdata(client);
	int i;

	for (i = 0; i < TPS65023_NUM_REGULATOR; i++)
		regulator_unregister(tps->rdev[i]);

<<<<<<< HEAD
=======
	regmap_exit(tps->regmap);
>>>>>>> refs/remotes/origin/cm-10.0
	kfree(tps);

	return 0;
}

<<<<<<< HEAD
=======
static const struct tps_info tps65020_regs[] = {
	{
		.name = "VDCDC1",
		.min_uV = 3300000,
		.max_uV = 3300000,
		.fixed	= 1,
	},
	{
		.name = "VDCDC2",
		.min_uV =  1800000,
		.max_uV = 1800000,
		.fixed = 1,
	},
	{
		.name = "VDCDC3",
		.min_uV =  800000,
		.max_uV = 1600000,
		.table_len = ARRAY_SIZE(VCORE_VSEL_table),
		.table = VCORE_VSEL_table,
	},

	{
		.name = "LDO1",
		.min_uV = 1000000,
		.max_uV = 3150000,
		.table_len = ARRAY_SIZE(TPS65020_LDO1_VSEL_table),
		.table = TPS65020_LDO1_VSEL_table,
	},
	{
		.name = "LDO2",
		.min_uV = 1050000,
		.max_uV = 3300000,
		.table_len = ARRAY_SIZE(TPS65020_LDO2_VSEL_table),
		.table = TPS65020_LDO2_VSEL_table,
=======
	/* Enable setting output voltage by I2C */
	regmap_update_bits(tps->regmap, TPS65023_REG_CON_CTRL2,
					TPS65023_REG_CTRL2_CORE_ADJ,
					TPS65023_REG_CTRL2_CORE_ADJ);

	return 0;
}

static const struct tps_info tps65020_regs[] = {
	{
		.name = "VDCDC1",
		.table_len = ARRAY_SIZE(DCDC_FIXED_3300000_VSEL_table),
		.table = DCDC_FIXED_3300000_VSEL_table,
	},
	{
		.name = "VDCDC2",
		.table_len = ARRAY_SIZE(DCDC_FIXED_1800000_VSEL_table),
		.table = DCDC_FIXED_1800000_VSEL_table,
	},
	{
		.name = "VDCDC3",
		.table_len = ARRAY_SIZE(VCORE_VSEL_table),
		.table = VCORE_VSEL_table,
	},
	{
		.name = "LDO1",
		.table_len = ARRAY_SIZE(TPS65020_LDO_VSEL_table),
		.table = TPS65020_LDO_VSEL_table,
	},
	{
		.name = "LDO2",
		.table_len = ARRAY_SIZE(TPS65020_LDO_VSEL_table),
		.table = TPS65020_LDO_VSEL_table,
>>>>>>> refs/remotes/origin/master
	},
};

static const struct tps_info tps65021_regs[] = {
	{
		.name = "VDCDC1",
<<<<<<< HEAD
		.min_uV =  3300000,
		.max_uV = 3300000,
		.fixed = 1,
	},
	{
		.name = "VDCDC2",
		.min_uV =  1800000,
		.max_uV = 1800000,
		.fixed = 1,
	},
	{
		.name = "VDCDC3",
		.min_uV =  800000,
		.max_uV = 1600000,
=======
		.table_len = ARRAY_SIZE(DCDC_FIXED_3300000_VSEL_table),
		.table = DCDC_FIXED_3300000_VSEL_table,
	},
	{
		.name = "VDCDC2",
		.table_len = ARRAY_SIZE(DCDC_FIXED_1800000_VSEL_table),
		.table = DCDC_FIXED_1800000_VSEL_table,
	},
	{
		.name = "VDCDC3",
>>>>>>> refs/remotes/origin/master
		.table_len = ARRAY_SIZE(VCORE_VSEL_table),
		.table = VCORE_VSEL_table,
	},
	{
		.name = "LDO1",
<<<<<<< HEAD
		.min_uV = 1000000,
		.max_uV = 3150000,
=======
>>>>>>> refs/remotes/origin/master
		.table_len = ARRAY_SIZE(TPS65023_LDO1_VSEL_table),
		.table = TPS65023_LDO1_VSEL_table,
	},
	{
		.name = "LDO2",
<<<<<<< HEAD
		.min_uV = 1050000,
		.max_uV = 3300000,
=======
>>>>>>> refs/remotes/origin/master
		.table_len = ARRAY_SIZE(TPS65023_LDO2_VSEL_table),
		.table = TPS65023_LDO2_VSEL_table,
	},
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static const struct tps_info tps65023_regs[] = {
	{
		.name = "VDCDC1",
		.min_uV =  800000,
		.max_uV = 1600000,
<<<<<<< HEAD
		.table_len = ARRAY_SIZE(VDCDC1_VSEL_table),
		.table = VDCDC1_VSEL_table,
=======
		.table_len = ARRAY_SIZE(VCORE_VSEL_table),
		.table = VCORE_VSEL_table,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "VDCDC2",
		.min_uV =  3300000,
		.max_uV = 3300000,
		.fixed = 1,
	},
	{
		.name = "VDCDC3",
		.min_uV =  1800000,
		.max_uV = 1800000,
		.fixed = 1,
	},
	{
		.name = "LDO1",
		.min_uV = 1000000,
		.max_uV = 3150000,
<<<<<<< HEAD
		.table_len = ARRAY_SIZE(LDO1_VSEL_table),
		.table = LDO1_VSEL_table,
=======
		.table_len = ARRAY_SIZE(TPS65023_LDO1_VSEL_table),
		.table = TPS65023_LDO1_VSEL_table,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	{
		.name = "LDO2",
		.min_uV = 1050000,
		.max_uV = 3300000,
<<<<<<< HEAD
		.table_len = ARRAY_SIZE(LDO2_VSEL_table),
		.table = LDO2_VSEL_table,
	},
};

static const struct i2c_device_id tps_65023_id[] = {
	{.name = "tps65023",
	.driver_data = (unsigned long) tps65023_regs,},
	{.name = "tps65021",
	.driver_data = (unsigned long) tps65023_regs,},
=======
=======
static const struct tps_info tps65023_regs[] = {
	{
		.name = "VDCDC1",
		.table_len = ARRAY_SIZE(VCORE_VSEL_table),
		.table = VCORE_VSEL_table,
	},
	{
		.name = "VDCDC2",
		.table_len = ARRAY_SIZE(DCDC_FIXED_3300000_VSEL_table),
		.table = DCDC_FIXED_3300000_VSEL_table,
	},
	{
		.name = "VDCDC3",
		.table_len = ARRAY_SIZE(DCDC_FIXED_1800000_VSEL_table),
		.table = DCDC_FIXED_1800000_VSEL_table,
	},
	{
		.name = "LDO1",
		.table_len = ARRAY_SIZE(TPS65023_LDO1_VSEL_table),
		.table = TPS65023_LDO1_VSEL_table,
	},
	{
		.name = "LDO2",
>>>>>>> refs/remotes/origin/master
		.table_len = ARRAY_SIZE(TPS65023_LDO2_VSEL_table),
		.table = TPS65023_LDO2_VSEL_table,
	},
};

static struct tps_driver_data tps65020_drv_data = {
	.info = tps65020_regs,
	.core_regulator = TPS65023_DCDC_3,
};

static struct tps_driver_data tps65021_drv_data = {
<<<<<<< HEAD
		.info = tps65021_regs,
		.core_regulator = TPS65023_DCDC_3,
};

static struct tps_driver_data tps65023_drv_data = {
		.info = tps65023_regs,
		.core_regulator = TPS65023_DCDC_1,
=======
	.info = tps65021_regs,
	.core_regulator = TPS65023_DCDC_3,
};

static struct tps_driver_data tps65023_drv_data = {
	.info = tps65023_regs,
	.core_regulator = TPS65023_DCDC_1,
>>>>>>> refs/remotes/origin/master
};

static const struct i2c_device_id tps_65023_id[] = {
	{.name = "tps65023",
	.driver_data = (unsigned long) &tps65023_drv_data},
	{.name = "tps65021",
	.driver_data = (unsigned long) &tps65021_drv_data,},
	{.name = "tps65020",
	.driver_data = (unsigned long) &tps65020_drv_data},
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	{ },
};

MODULE_DEVICE_TABLE(i2c, tps_65023_id);

static struct i2c_driver tps_65023_i2c_driver = {
	.driver = {
		.name = "tps65023",
		.owner = THIS_MODULE,
	},
	.probe = tps_65023_probe,
<<<<<<< HEAD
	.remove = __devexit_p(tps_65023_remove),
	.id_table = tps_65023_id,
};

/**
 * tps_65023_init
 *
 * Module init function
 */
=======
	.id_table = tps_65023_id,
};

>>>>>>> refs/remotes/origin/master
static int __init tps_65023_init(void)
{
	return i2c_add_driver(&tps_65023_i2c_driver);
}
subsys_initcall(tps_65023_init);

<<<<<<< HEAD
/**
 * tps_65023_cleanup
 *
 * Module exit function
 */
=======
>>>>>>> refs/remotes/origin/master
static void __exit tps_65023_cleanup(void)
{
	i2c_del_driver(&tps_65023_i2c_driver);
}
module_exit(tps_65023_cleanup);

MODULE_AUTHOR("Texas Instruments");
MODULE_DESCRIPTION("TPS65023 voltage regulator driver");
MODULE_LICENSE("GPL v2");
