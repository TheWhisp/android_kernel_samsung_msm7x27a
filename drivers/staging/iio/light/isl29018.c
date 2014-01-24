/*
 * A iio driver for the light sensor ISL 29018.
 *
 * IIO driver for monitoring ambient light intensity in luxi, proximity
 * sensing and infrared sensing.
 *
 * Copyright (c) 2010, NVIDIA Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA	02110-1301, USA.
 */

#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include <linux/mutex.h>
#include <linux/delay.h>
<<<<<<< HEAD
#include <linux/slab.h>
#include "../iio.h"
<<<<<<< HEAD

=======
#include "../sysfs.h"
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/regmap.h>
#include <linux/slab.h>
#include <linux/iio/iio.h>
#include <linux/iio/sysfs.h>

>>>>>>> refs/remotes/origin/master
#define CONVERSION_TIME_MS		100

#define ISL29018_REG_ADD_COMMAND1	0x00
#define COMMMAND1_OPMODE_SHIFT		5
#define COMMMAND1_OPMODE_MASK		(7 << COMMMAND1_OPMODE_SHIFT)
#define COMMMAND1_OPMODE_POWER_DOWN	0
#define COMMMAND1_OPMODE_ALS_ONCE	1
#define COMMMAND1_OPMODE_IR_ONCE	2
#define COMMMAND1_OPMODE_PROX_ONCE	3

#define ISL29018_REG_ADD_COMMANDII	0x01
#define COMMANDII_RESOLUTION_SHIFT	2
#define COMMANDII_RESOLUTION_MASK	(0x3 << COMMANDII_RESOLUTION_SHIFT)

#define COMMANDII_RANGE_SHIFT		0
#define COMMANDII_RANGE_MASK		(0x3 << COMMANDII_RANGE_SHIFT)

#define COMMANDII_SCHEME_SHIFT		7
#define COMMANDII_SCHEME_MASK		(0x1 << COMMANDII_SCHEME_SHIFT)

#define ISL29018_REG_ADD_DATA_LSB	0x02
#define ISL29018_REG_ADD_DATA_MSB	0x03
<<<<<<< HEAD
<<<<<<< HEAD
#define ISL29018_MAX_REGS		ISL29018_REG_ADD_DATA_MSB

struct isl29018_chip {
	struct iio_dev		*indio_dev;
	struct i2c_client	*client;
	struct mutex		lock;
=======
#define ISL29018_MAX_REGS		(ISL29018_REG_ADD_DATA_MSB+1)
=======
>>>>>>> refs/remotes/origin/master

#define ISL29018_REG_TEST		0x08
#define ISL29018_TEST_SHIFT		0
#define ISL29018_TEST_MASK		(0xFF << ISL29018_TEST_SHIFT)

struct isl29018_chip {
<<<<<<< HEAD
	struct i2c_client	*client;
	struct mutex		lock;
	unsigned int		lux_scale;
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned int		range;
	unsigned int		adc_bit;
	int			prox_scheme;
	u8			reg_cache[ISL29018_MAX_REGS];
};

static int isl29018_write_data(struct i2c_client *client, u8 reg,
			u8 val, u8 mask, u8 shift)
{
<<<<<<< HEAD
	u8 regval;
	int ret = 0;
	struct isl29018_chip *chip = i2c_get_clientdata(client);

	regval = chip->reg_cache[reg];
	regval &= ~mask;
	regval |= val << shift;
=======
	u8 regval = val;
	int ret;
	struct isl29018_chip *chip = iio_priv(i2c_get_clientdata(client));

	/* don't cache or mask REG_TEST */
	if (reg < ISL29018_MAX_REGS) {
		regval = chip->reg_cache[reg];
		regval &= ~mask;
		regval |= val << shift;
	}
>>>>>>> refs/remotes/origin/cm-10.0

	ret = i2c_smbus_write_byte_data(client, reg, regval);
	if (ret) {
		dev_err(&client->dev, "Write to device fails status %x\n", ret);
<<<<<<< HEAD
		return ret;
	}
	chip->reg_cache[reg] = regval;

	return 0;
=======
	} else {
		/* don't update cache on err */
		if (reg < ISL29018_MAX_REGS)
			chip->reg_cache[reg] = regval;
	}

	return ret;
>>>>>>> refs/remotes/origin/cm-10.0
}

static int isl29018_set_range(struct i2c_client *client, unsigned long range,
=======
	struct device		*dev;
	struct regmap		*regmap;
	struct mutex		lock;
	unsigned int		lux_scale;
	unsigned int		lux_uscale;
	unsigned int		range;
	unsigned int		adc_bit;
	int			prox_scheme;
	bool			suspended;
};

static int isl29018_set_range(struct isl29018_chip *chip, unsigned long range,
>>>>>>> refs/remotes/origin/master
		unsigned int *new_range)
{
	static const unsigned long supp_ranges[] = {1000, 4000, 16000, 64000};
	int i;

	for (i = 0; i < ARRAY_SIZE(supp_ranges); ++i) {
		if (range <= supp_ranges[i]) {
			*new_range = (unsigned int)supp_ranges[i];
			break;
		}
	}

	if (i >= ARRAY_SIZE(supp_ranges))
		return -EINVAL;

<<<<<<< HEAD
	return isl29018_write_data(client, ISL29018_REG_ADD_COMMANDII,
			i, COMMANDII_RANGE_MASK, COMMANDII_RANGE_SHIFT);
}

static int isl29018_set_resolution(struct i2c_client *client,
=======
	return regmap_update_bits(chip->regmap, ISL29018_REG_ADD_COMMANDII,
			COMMANDII_RANGE_MASK, i << COMMANDII_RANGE_SHIFT);
}

static int isl29018_set_resolution(struct isl29018_chip *chip,
>>>>>>> refs/remotes/origin/master
			unsigned long adcbit, unsigned int *conf_adc_bit)
{
	static const unsigned long supp_adcbit[] = {16, 12, 8, 4};
	int i;

	for (i = 0; i < ARRAY_SIZE(supp_adcbit); ++i) {
		if (adcbit >= supp_adcbit[i]) {
			*conf_adc_bit = (unsigned int)supp_adcbit[i];
			break;
		}
	}

	if (i >= ARRAY_SIZE(supp_adcbit))
		return -EINVAL;

<<<<<<< HEAD
	return isl29018_write_data(client, ISL29018_REG_ADD_COMMANDII,
			i, COMMANDII_RESOLUTION_MASK,
			COMMANDII_RESOLUTION_SHIFT);
}

static int isl29018_read_sensor_input(struct i2c_client *client, int mode)
{
	int status;
	int lsb;
	int msb;

	/* Set mode */
	status = isl29018_write_data(client, ISL29018_REG_ADD_COMMAND1,
			mode, COMMMAND1_OPMODE_MASK, COMMMAND1_OPMODE_SHIFT);
	if (status) {
		dev_err(&client->dev, "Error in setting operating mode\n");
		return status;
	}
	msleep(CONVERSION_TIME_MS);
	lsb = i2c_smbus_read_byte_data(client, ISL29018_REG_ADD_DATA_LSB);
	if (lsb < 0) {
		dev_err(&client->dev, "Error in reading LSB DATA\n");
		return lsb;
	}

	msb = i2c_smbus_read_byte_data(client, ISL29018_REG_ADD_DATA_MSB);
	if (msb < 0) {
		dev_err(&client->dev, "Error in reading MSB DATA\n");
		return msb;
	}
	dev_vdbg(&client->dev, "MSB 0x%x and LSB 0x%x\n", msb, lsb);
=======
	return regmap_update_bits(chip->regmap, ISL29018_REG_ADD_COMMANDII,
			COMMANDII_RESOLUTION_MASK,
			i << COMMANDII_RESOLUTION_SHIFT);
}

static int isl29018_read_sensor_input(struct isl29018_chip *chip, int mode)
{
	int status;
	unsigned int lsb;
	unsigned int msb;

	/* Set mode */
	status = regmap_write(chip->regmap, ISL29018_REG_ADD_COMMAND1,
			mode << COMMMAND1_OPMODE_SHIFT);
	if (status) {
		dev_err(chip->dev,
			"Error in setting operating mode err %d\n", status);
		return status;
	}
	msleep(CONVERSION_TIME_MS);
	status = regmap_read(chip->regmap, ISL29018_REG_ADD_DATA_LSB, &lsb);
	if (status < 0) {
		dev_err(chip->dev,
			"Error in reading LSB DATA with err %d\n", status);
		return status;
	}

	status = regmap_read(chip->regmap, ISL29018_REG_ADD_DATA_MSB, &msb);
	if (status < 0) {
		dev_err(chip->dev,
			"Error in reading MSB DATA with error %d\n", status);
		return status;
	}
	dev_vdbg(chip->dev, "MSB 0x%x and LSB 0x%x\n", msb, lsb);
>>>>>>> refs/remotes/origin/master

	return (msb << 8) | lsb;
}

<<<<<<< HEAD
static int isl29018_read_lux(struct i2c_client *client, int *lux)
{
	int lux_data;
<<<<<<< HEAD
	struct isl29018_chip *chip = i2c_get_clientdata(client);
=======
	struct isl29018_chip *chip = iio_priv(i2c_get_clientdata(client));
>>>>>>> refs/remotes/origin/cm-10.0

	lux_data = isl29018_read_sensor_input(client,
				COMMMAND1_OPMODE_ALS_ONCE);
=======
static int isl29018_read_lux(struct isl29018_chip *chip, int *lux)
{
	int lux_data;
	unsigned int data_x_range, lux_unshifted;

	lux_data = isl29018_read_sensor_input(chip, COMMMAND1_OPMODE_ALS_ONCE);
>>>>>>> refs/remotes/origin/master

	if (lux_data < 0)
		return lux_data;

<<<<<<< HEAD
<<<<<<< HEAD
	*lux = (lux_data * chip->range) >> chip->adc_bit;
=======
	*lux = (lux_data * chip->range * chip->lux_scale) >> chip->adc_bit;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	/* To support fractional scaling, separate the unshifted lux
	 * into two calculations: int scaling and micro-scaling.
	 * lux_uscale ranges from 0-999999, so about 20 bits.  Split
	 * the /1,000,000 in two to reduce the risk of over/underflow.
	 */
	data_x_range = lux_data * chip->range;
	lux_unshifted = data_x_range * chip->lux_scale;
	lux_unshifted += data_x_range / 1000 * chip->lux_uscale / 1000;
	*lux = lux_unshifted >> chip->adc_bit;
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
static int isl29018_read_ir(struct i2c_client *client, int *ir)
{
	int ir_data;

	ir_data = isl29018_read_sensor_input(client, COMMMAND1_OPMODE_IR_ONCE);
=======
static int isl29018_read_ir(struct isl29018_chip *chip, int *ir)
{
	int ir_data;

	ir_data = isl29018_read_sensor_input(chip, COMMMAND1_OPMODE_IR_ONCE);
>>>>>>> refs/remotes/origin/master

	if (ir_data < 0)
		return ir_data;

	*ir = ir_data;

	return 0;
}

<<<<<<< HEAD
static int isl29018_read_proximity_ir(struct i2c_client *client, int scheme,
=======
static int isl29018_read_proximity_ir(struct isl29018_chip *chip, int scheme,
>>>>>>> refs/remotes/origin/master
		int *near_ir)
{
	int status;
	int prox_data = -1;
	int ir_data = -1;

	/* Do proximity sensing with required scheme */
<<<<<<< HEAD
	status = isl29018_write_data(client, ISL29018_REG_ADD_COMMANDII,
			scheme, COMMANDII_SCHEME_MASK, COMMANDII_SCHEME_SHIFT);
	if (status) {
		dev_err(&client->dev, "Error in setting operating mode\n");
		return status;
	}

	prox_data = isl29018_read_sensor_input(client,
=======
	status = regmap_update_bits(chip->regmap, ISL29018_REG_ADD_COMMANDII,
			COMMANDII_SCHEME_MASK,
			scheme << COMMANDII_SCHEME_SHIFT);
	if (status) {
		dev_err(chip->dev, "Error in setting operating mode\n");
		return status;
	}

	prox_data = isl29018_read_sensor_input(chip,
>>>>>>> refs/remotes/origin/master
					COMMMAND1_OPMODE_PROX_ONCE);
	if (prox_data < 0)
		return prox_data;

	if (scheme == 1) {
		*near_ir = prox_data;
		return 0;
	}

<<<<<<< HEAD
	ir_data = isl29018_read_sensor_input(client,
				COMMMAND1_OPMODE_IR_ONCE);
=======
	ir_data = isl29018_read_sensor_input(chip, COMMMAND1_OPMODE_IR_ONCE);
>>>>>>> refs/remotes/origin/master

	if (ir_data < 0)
		return ir_data;

	if (prox_data >= ir_data)
		*near_ir = prox_data - ir_data;
	else
		*near_ir = 0;

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static ssize_t get_sensor_data(struct device *dev, char *buf, int mode)
{
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct isl29018_chip *chip = indio_dev->dev_data;
	struct i2c_client *client = chip->client;
	int value = 0;
	int status;

	mutex_lock(&chip->lock);
	switch (mode) {
	case COMMMAND1_OPMODE_PROX_ONCE:
		status = isl29018_read_proximity_ir(client,
				chip->prox_scheme, &value);
		break;

	case COMMMAND1_OPMODE_ALS_ONCE:
		status = isl29018_read_lux(client, &value);
		break;

	case COMMMAND1_OPMODE_IR_ONCE:
		status = isl29018_read_ir(client, &value);
		break;

	default:
		dev_err(&client->dev, "Mode %d is not supported\n", mode);
		mutex_unlock(&chip->lock);
		return -EBUSY;
	}
	if (status < 0) {
		dev_err(&client->dev, "Error in Reading data");
		mutex_unlock(&chip->lock);
		return status;
	}

	mutex_unlock(&chip->lock);

	return sprintf(buf, "%d\n", value);
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/* Sysfs interface */
/* range */
static ssize_t show_range(struct device *dev,
			struct device_attribute *attr, char *buf)
{
<<<<<<< HEAD
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct isl29018_chip *chip = indio_dev->dev_data;
=======
	struct isl29018_chip *chip = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct iio_dev *indio_dev = dev_to_iio_dev(dev);
	struct isl29018_chip *chip = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/master

	return sprintf(buf, "%u\n", chip->range);
}

static ssize_t store_range(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
<<<<<<< HEAD
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct isl29018_chip *chip = indio_dev->dev_data;
=======
	struct isl29018_chip *chip = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
	struct i2c_client *client = chip->client;
=======
	struct iio_dev *indio_dev = dev_to_iio_dev(dev);
	struct isl29018_chip *chip = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/master
	int status;
	unsigned long lval;
	unsigned int new_range;

<<<<<<< HEAD
	if (strict_strtoul(buf, 10, &lval))
=======
	if (kstrtoul(buf, 10, &lval))
>>>>>>> refs/remotes/origin/master
		return -EINVAL;

	if (!(lval == 1000UL || lval == 4000UL ||
			lval == 16000UL || lval == 64000UL)) {
		dev_err(dev, "The range is not supported\n");
		return -EINVAL;
	}

	mutex_lock(&chip->lock);
<<<<<<< HEAD
	status = isl29018_set_range(client, lval, &new_range);
	if (status < 0) {
		mutex_unlock(&chip->lock);
		dev_err(dev, "Error in setting max range\n");
=======
	status = isl29018_set_range(chip, lval, &new_range);
	if (status < 0) {
		mutex_unlock(&chip->lock);
		dev_err(dev,
			"Error in setting max range with err %d\n", status);
>>>>>>> refs/remotes/origin/master
		return status;
	}
	chip->range = new_range;
	mutex_unlock(&chip->lock);

	return count;
}

/* resolution */
static ssize_t show_resolution(struct device *dev,
			struct device_attribute *attr, char *buf)
{
<<<<<<< HEAD
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct isl29018_chip *chip = indio_dev->dev_data;
=======
	struct isl29018_chip *chip = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct iio_dev *indio_dev = dev_to_iio_dev(dev);
	struct isl29018_chip *chip = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/master

	return sprintf(buf, "%u\n", chip->adc_bit);
}

static ssize_t store_resolution(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
<<<<<<< HEAD
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct isl29018_chip *chip = indio_dev->dev_data;
=======
	struct isl29018_chip *chip = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
	struct i2c_client *client = chip->client;
	int status;
	unsigned long lval;
	unsigned int new_adc_bit;

	if (strict_strtoul(buf, 10, &lval))
		return -EINVAL;
	if (!(lval == 4 || lval == 8 || lval == 12 || lval == 16)) {
=======
	struct iio_dev *indio_dev = dev_to_iio_dev(dev);
	struct isl29018_chip *chip = iio_priv(indio_dev);
	int status;
	unsigned int val;
	unsigned int new_adc_bit;

	if (kstrtouint(buf, 10, &val))
		return -EINVAL;
	if (!(val == 4 || val == 8 || val == 12 || val == 16)) {
>>>>>>> refs/remotes/origin/master
		dev_err(dev, "The resolution is not supported\n");
		return -EINVAL;
	}

	mutex_lock(&chip->lock);
<<<<<<< HEAD
	status = isl29018_set_resolution(client, lval, &new_adc_bit);
=======
	status = isl29018_set_resolution(chip, val, &new_adc_bit);
>>>>>>> refs/remotes/origin/master
	if (status < 0) {
		mutex_unlock(&chip->lock);
		dev_err(dev, "Error in setting resolution\n");
		return status;
	}
	chip->adc_bit = new_adc_bit;
	mutex_unlock(&chip->lock);

	return count;
}

/* proximity scheme */
<<<<<<< HEAD
static ssize_t show_prox_infrared_supression(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct isl29018_chip *chip = indio_dev->dev_data;
=======
	struct isl29018_chip *chip = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0

	/* return the "proximity scheme" i.e. if the chip does on chip
	infrared supression (1 means perform on chip supression) */
	return sprintf(buf, "%d\n", chip->prox_scheme);
}

static ssize_t store_prox_infrared_supression(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct isl29018_chip *chip = indio_dev->dev_data;
=======
	struct isl29018_chip *chip = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned long lval;

	if (strict_strtoul(buf, 10, &lval))
		return -EINVAL;
	if (!(lval == 0UL || lval == 1UL)) {
=======
static ssize_t show_prox_infrared_suppression(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct iio_dev *indio_dev = dev_to_iio_dev(dev);
	struct isl29018_chip *chip = iio_priv(indio_dev);

	/* return the "proximity scheme" i.e. if the chip does on chip
	infrared suppression (1 means perform on chip suppression) */
	return sprintf(buf, "%d\n", chip->prox_scheme);
}

static ssize_t store_prox_infrared_suppression(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct iio_dev *indio_dev = dev_to_iio_dev(dev);
	struct isl29018_chip *chip = iio_priv(indio_dev);
	int val;

	if (kstrtoint(buf, 10, &val))
		return -EINVAL;
	if (!(val == 0 || val == 1)) {
>>>>>>> refs/remotes/origin/master
		dev_err(dev, "The mode is not supported\n");
		return -EINVAL;
	}

	/* get the  "proximity scheme" i.e. if the chip does on chip
<<<<<<< HEAD
	infrared supression (1 means perform on chip supression) */
	mutex_lock(&chip->lock);
	chip->prox_scheme = (int)lval;
=======
	infrared suppression (1 means perform on chip suppression) */
	mutex_lock(&chip->lock);
	chip->prox_scheme = val;
>>>>>>> refs/remotes/origin/master
	mutex_unlock(&chip->lock);

	return count;
}

<<<<<<< HEAD
<<<<<<< HEAD
/* Read lux */
static ssize_t show_lux(struct device *dev,
		struct device_attribute *devattr, char *buf)
{
	return get_sensor_data(dev, buf, COMMMAND1_OPMODE_ALS_ONCE);
}

/* Read ir */
static ssize_t show_ir(struct device *dev,
		struct device_attribute *devattr, char *buf)
{
	return get_sensor_data(dev, buf, COMMMAND1_OPMODE_IR_ONCE);
}

/* Read nearest ir */
static ssize_t show_proxim_ir(struct device *dev,
		struct device_attribute *devattr, char *buf)
{
	return get_sensor_data(dev, buf, COMMMAND1_OPMODE_PROX_ONCE);
}

=======
=======
>>>>>>> refs/remotes/origin/master
/* Channel IO */
static int isl29018_write_raw(struct iio_dev *indio_dev,
			      struct iio_chan_spec const *chan,
			      int val,
			      int val2,
			      long mask)
{
	struct isl29018_chip *chip = iio_priv(indio_dev);
	int ret = -EINVAL;

	mutex_lock(&chip->lock);
	if (mask == IIO_CHAN_INFO_CALIBSCALE && chan->type == IIO_LIGHT) {
		chip->lux_scale = val;
<<<<<<< HEAD
=======
		/* With no write_raw_get_fmt(), val2 is a MICRO fraction. */
		chip->lux_uscale = val2;
>>>>>>> refs/remotes/origin/master
		ret = 0;
	}
	mutex_unlock(&chip->lock);

<<<<<<< HEAD
	return 0;
=======
	return ret;
>>>>>>> refs/remotes/origin/master
}

static int isl29018_read_raw(struct iio_dev *indio_dev,
			     struct iio_chan_spec const *chan,
			     int *val,
			     int *val2,
			     long mask)
{
	int ret = -EINVAL;
	struct isl29018_chip *chip = iio_priv(indio_dev);
<<<<<<< HEAD
	struct i2c_client *client = chip->client;

	mutex_lock(&chip->lock);
	switch (mask) {
	case 0:
		switch (chan->type) {
		case IIO_LIGHT:
			ret = isl29018_read_lux(client, val);
			break;
		case IIO_INTENSITY:
			ret = isl29018_read_ir(client, val);
			break;
		case IIO_PROXIMITY:
			ret = isl29018_read_proximity_ir(client,
=======

	mutex_lock(&chip->lock);
	if (chip->suspended) {
		mutex_unlock(&chip->lock);
		return -EBUSY;
	}
	switch (mask) {
	case IIO_CHAN_INFO_RAW:
	case IIO_CHAN_INFO_PROCESSED:
		switch (chan->type) {
		case IIO_LIGHT:
			ret = isl29018_read_lux(chip, val);
			break;
		case IIO_INTENSITY:
			ret = isl29018_read_ir(chip, val);
			break;
		case IIO_PROXIMITY:
			ret = isl29018_read_proximity_ir(chip,
>>>>>>> refs/remotes/origin/master
					chip->prox_scheme, val);
			break;
		default:
			break;
		}
		if (!ret)
			ret = IIO_VAL_INT;
		break;
	case IIO_CHAN_INFO_CALIBSCALE:
		if (chan->type == IIO_LIGHT) {
			*val = chip->lux_scale;
<<<<<<< HEAD
			ret = IIO_VAL_INT;
=======
			*val2 = chip->lux_uscale;
			ret = IIO_VAL_INT_PLUS_MICRO;
>>>>>>> refs/remotes/origin/master
		}
		break;
	default:
		break;
	}
	mutex_unlock(&chip->lock);
	return ret;
}

static const struct iio_chan_spec isl29018_channels[] = {
	{
		.type = IIO_LIGHT,
		.indexed = 1,
		.channel = 0,
<<<<<<< HEAD
		.processed_val = IIO_PROCESSED,
		.info_mask = IIO_CHAN_INFO_CALIBSCALE_SEPARATE_BIT,
	}, {
		.type = IIO_INTENSITY,
		.modified = 1,
=======
		.info_mask_separate = BIT(IIO_CHAN_INFO_PROCESSED) |
		BIT(IIO_CHAN_INFO_CALIBSCALE),
	}, {
		.type = IIO_INTENSITY,
		.modified = 1,
		.info_mask_separate = BIT(IIO_CHAN_INFO_RAW),
>>>>>>> refs/remotes/origin/master
		.channel2 = IIO_MOD_LIGHT_IR,
	}, {
		/* Unindexed in current ABI.  But perhaps it should be. */
		.type = IIO_PROXIMITY,
<<<<<<< HEAD
	}
};

>>>>>>> refs/remotes/origin/cm-10.0
=======
		.info_mask_separate = BIT(IIO_CHAN_INFO_RAW),
	}
};

>>>>>>> refs/remotes/origin/master
static IIO_DEVICE_ATTR(range, S_IRUGO | S_IWUSR, show_range, store_range, 0);
static IIO_CONST_ATTR(range_available, "1000 4000 16000 64000");
static IIO_CONST_ATTR(adc_resolution_available, "4 8 12 16");
static IIO_DEVICE_ATTR(adc_resolution, S_IRUGO | S_IWUSR,
					show_resolution, store_resolution, 0);
<<<<<<< HEAD
static IIO_DEVICE_ATTR(proximity_on_chip_ambient_infrared_supression,
					S_IRUGO | S_IWUSR,
					show_prox_infrared_supression,
					store_prox_infrared_supression, 0);
<<<<<<< HEAD
static IIO_DEVICE_ATTR(illuminance0_input, S_IRUGO, show_lux, NULL, 0);
static IIO_DEVICE_ATTR(intensity_infrared_raw, S_IRUGO, show_ir, NULL, 0);
static IIO_DEVICE_ATTR(proximity_raw, S_IRUGO, show_proxim_ir, NULL, 0);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
static IIO_DEVICE_ATTR(proximity_on_chip_ambient_infrared_suppression,
					S_IRUGO | S_IWUSR,
					show_prox_infrared_suppression,
					store_prox_infrared_suppression, 0);
>>>>>>> refs/remotes/origin/master

#define ISL29018_DEV_ATTR(name) (&iio_dev_attr_##name.dev_attr.attr)
#define ISL29018_CONST_ATTR(name) (&iio_const_attr_##name.dev_attr.attr)
static struct attribute *isl29018_attributes[] = {
	ISL29018_DEV_ATTR(range),
	ISL29018_CONST_ATTR(range_available),
	ISL29018_DEV_ATTR(adc_resolution),
	ISL29018_CONST_ATTR(adc_resolution_available),
<<<<<<< HEAD
	ISL29018_DEV_ATTR(proximity_on_chip_ambient_infrared_supression),
<<<<<<< HEAD
	ISL29018_DEV_ATTR(illuminance0_input),
	ISL29018_DEV_ATTR(intensity_infrared_raw),
	ISL29018_DEV_ATTR(proximity_raw),
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ISL29018_DEV_ATTR(proximity_on_chip_ambient_infrared_suppression),
>>>>>>> refs/remotes/origin/master
	NULL
};

static const struct attribute_group isl29108_group = {
	.attrs = isl29018_attributes,
};

<<<<<<< HEAD
static int isl29018_chip_init(struct i2c_client *client)
{
<<<<<<< HEAD
	struct isl29018_chip *chip = i2c_get_clientdata(client);
=======
	struct isl29018_chip *chip = iio_priv(i2c_get_clientdata(client));
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int isl29018_chip_init(struct isl29018_chip *chip)
{
>>>>>>> refs/remotes/origin/master
	int status;
	int new_adc_bit;
	unsigned int new_range;

<<<<<<< HEAD
	memset(chip->reg_cache, 0, sizeof(chip->reg_cache));

<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	/* Code added per Intersil Application Note 1534:
	 *     When VDD sinks to approximately 1.8V or below, some of
	 * the part's registers may change their state. When VDD
	 * recovers to 2.25V (or greater), the part may thus be in an
	 * unknown mode of operation. The user can return the part to
	 * a known mode of operation either by (a) setting VDD = 0V for
	 * 1 second or more and then powering back up with a slew rate
	 * of 0.5V/ms or greater, or (b) via I2C disable all ALS/PROX
	 * conversions, clear the test registers, and then rewrite all
	 * registers to the desired values.
	 * ...
	 * FOR ISL29011, ISL29018, ISL29021, ISL29023
	 * 1. Write 0x00 to register 0x08 (TEST)
	 * 2. Write 0x00 to register 0x00 (CMD1)
	 * 3. Rewrite all registers to the desired values
	 *
	 * ISL29018 Data Sheet (FN6619.1, Feb 11, 2010) essentially says
	 * the same thing EXCEPT the data sheet asks for a 1ms delay after
	 * writing the CMD1 register.
	 */
<<<<<<< HEAD
	status = isl29018_write_data(client, ISL29018_REG_TEST, 0,
				ISL29018_TEST_MASK, ISL29018_TEST_SHIFT);
	if (status < 0) {
		dev_err(&client->dev, "Failed to clear isl29018 TEST reg."
=======
	status = regmap_write(chip->regmap, ISL29018_REG_TEST, 0x0);
	if (status < 0) {
		dev_err(chip->dev, "Failed to clear isl29018 TEST reg."
>>>>>>> refs/remotes/origin/master
					"(%d)\n", status);
		return status;
	}

	/* See Intersil AN1534 comments above.
	 * "Operating Mode" (COMMAND1) register is reprogrammed when
	 * data is read from the device.
	 */
<<<<<<< HEAD
	status = isl29018_write_data(client, ISL29018_REG_ADD_COMMAND1, 0,
				0xff, 0);
	if (status < 0) {
		dev_err(&client->dev, "Failed to clear isl29018 CMD1 reg."
=======
	status = regmap_write(chip->regmap, ISL29018_REG_ADD_COMMAND1, 0);
	if (status < 0) {
		dev_err(chip->dev, "Failed to clear isl29018 CMD1 reg."
>>>>>>> refs/remotes/origin/master
					"(%d)\n", status);
		return status;
	}

	msleep(1);	/* per data sheet, page 10 */

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	/* set defaults */
	status = isl29018_set_range(client, chip->range, &new_range);
	if (status < 0) {
		dev_err(&client->dev, "Init of isl29018 fails\n");
		return status;
	}

	status = isl29018_set_resolution(client, chip->adc_bit,
=======
	/* set defaults */
	status = isl29018_set_range(chip, chip->range, &new_range);
	if (status < 0) {
		dev_err(chip->dev, "Init of isl29018 fails\n");
		return status;
	}

	status = isl29018_set_resolution(chip, chip->adc_bit,
>>>>>>> refs/remotes/origin/master
						&new_adc_bit);

	return 0;
}

static const struct iio_info isl29108_info = {
	.attrs = &isl29108_group,
	.driver_module = THIS_MODULE,
<<<<<<< HEAD
<<<<<<< HEAD
=======
	.read_raw = &isl29018_read_raw,
	.write_raw = &isl29018_write_raw,
>>>>>>> refs/remotes/origin/cm-10.0
};

static int __devinit isl29018_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	struct isl29018_chip *chip;
<<<<<<< HEAD
	int err;

	chip = kzalloc(sizeof(struct isl29018_chip), GFP_KERNEL);
	if (!chip) {
		dev_err(&client->dev, "Memory allocation fails\n");
		err = -ENOMEM;
		goto exit;
	}

	i2c_set_clientdata(client, chip);
=======
	struct iio_dev *indio_dev;
	int err;

	indio_dev = iio_allocate_device(sizeof(*chip));
	if (indio_dev == NULL) {
		dev_err(&client->dev, "iio allocation fails\n");
		err = -ENOMEM;
		goto exit;
=======
	.read_raw = &isl29018_read_raw,
	.write_raw = &isl29018_write_raw,
};

static bool is_volatile_reg(struct device *dev, unsigned int reg)
{
	switch (reg) {
	case ISL29018_REG_ADD_DATA_LSB:
	case ISL29018_REG_ADD_DATA_MSB:
	case ISL29018_REG_ADD_COMMAND1:
	case ISL29018_REG_TEST:
		return true;
	default:
		return false;
	}
}

/*
 * isl29018_regmap_config: regmap configuration.
 * Use RBTREE mechanism for caching.
 */
static const struct regmap_config isl29018_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
	.volatile_reg = is_volatile_reg,
	.max_register = ISL29018_REG_TEST,
	.num_reg_defaults_raw = ISL29018_REG_TEST + 1,
	.cache_type = REGCACHE_RBTREE,
};

static int isl29018_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	struct isl29018_chip *chip;
	struct iio_dev *indio_dev;
	int err;

	indio_dev = devm_iio_device_alloc(&client->dev, sizeof(*chip));
	if (indio_dev == NULL) {
		dev_err(&client->dev, "iio allocation fails\n");
		return -ENOMEM;
>>>>>>> refs/remotes/origin/master
	}
	chip = iio_priv(indio_dev);

	i2c_set_clientdata(client, indio_dev);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	chip->client = client;

	mutex_init(&chip->lock);

<<<<<<< HEAD
=======
	chip->lux_scale = 1;
>>>>>>> refs/remotes/origin/cm-10.0
	chip->range = 1000;
	chip->adc_bit = 16;

	err = isl29018_chip_init(client);
	if (err)
<<<<<<< HEAD
		goto exit_free;

	chip->indio_dev = iio_allocate_device(0);
	if (!chip->indio_dev) {
		dev_err(&client->dev, "iio allocation fails\n");
		goto exit_free;
	}
	chip->indio_dev->info = &isl29108_info;
	chip->indio_dev->name = id->name;
	chip->indio_dev->dev.parent = &client->dev;
	chip->indio_dev->dev_data = (void *)(chip);
	chip->indio_dev->modes = INDIO_DIRECT_MODE;
	err = iio_device_register(chip->indio_dev);
=======
		goto exit_iio_free;
=======
	chip->dev = &client->dev;

	mutex_init(&chip->lock);

	chip->lux_scale = 1;
	chip->lux_uscale = 0;
	chip->range = 1000;
	chip->adc_bit = 16;
	chip->suspended = false;

	chip->regmap = devm_regmap_init_i2c(client, &isl29018_regmap_config);
	if (IS_ERR(chip->regmap)) {
		err = PTR_ERR(chip->regmap);
		dev_err(chip->dev, "regmap initialization failed: %d\n", err);
		return err;
	}

	err = isl29018_chip_init(chip);
	if (err)
		return err;
>>>>>>> refs/remotes/origin/master

	indio_dev->info = &isl29108_info;
	indio_dev->channels = isl29018_channels;
	indio_dev->num_channels = ARRAY_SIZE(isl29018_channels);
	indio_dev->name = id->name;
	indio_dev->dev.parent = &client->dev;
	indio_dev->modes = INDIO_DIRECT_MODE;
<<<<<<< HEAD
	err = iio_device_register(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
	if (err) {
		dev_err(&client->dev, "iio registration fails\n");
		goto exit_iio_free;
	}

	return 0;
exit_iio_free:
<<<<<<< HEAD
	iio_free_device(chip->indio_dev);
exit_free:
	kfree(chip);
=======
	iio_free_device(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
exit:
	return err;
}

static int __devexit isl29018_remove(struct i2c_client *client)
{
<<<<<<< HEAD
	struct isl29018_chip *chip = i2c_get_clientdata(client);

	dev_dbg(&client->dev, "%s()\n", __func__);
	iio_device_unregister(chip->indio_dev);
	kfree(chip);
=======
	struct iio_dev *indio_dev = i2c_get_clientdata(client);

	dev_dbg(&client->dev, "%s()\n", __func__);
	iio_device_unregister(indio_dev);
	iio_free_device(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
}

=======
	err = devm_iio_device_register(&client->dev, indio_dev);
	if (err) {
		dev_err(&client->dev, "iio registration fails\n");
		return err;
	}

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int isl29018_suspend(struct device *dev)
{
	struct isl29018_chip *chip = iio_priv(dev_get_drvdata(dev));

	mutex_lock(&chip->lock);

	/* Since this driver uses only polling commands, we are by default in
	 * auto shutdown (ie, power-down) mode.
	 * So we do not have much to do here.
	 */
	chip->suspended = true;

	mutex_unlock(&chip->lock);
	return 0;
}

static int isl29018_resume(struct device *dev)
{
	struct isl29018_chip *chip = iio_priv(dev_get_drvdata(dev));
	int err;

	mutex_lock(&chip->lock);

	err = isl29018_chip_init(chip);
	if (!err)
		chip->suspended = false;

	mutex_unlock(&chip->lock);
	return err;
}

static SIMPLE_DEV_PM_OPS(isl29018_pm_ops, isl29018_suspend, isl29018_resume);
#define ISL29018_PM_OPS (&isl29018_pm_ops)
#else
#define ISL29018_PM_OPS NULL
#endif

>>>>>>> refs/remotes/origin/master
static const struct i2c_device_id isl29018_id[] = {
	{"isl29018", 0},
	{}
};

MODULE_DEVICE_TABLE(i2c, isl29018_id);

<<<<<<< HEAD
<<<<<<< HEAD
=======
static const struct of_device_id isl29018_of_match[] = {
	{ .compatible = "invn,isl29018", },
=======
static const struct of_device_id isl29018_of_match[] = {
	{ .compatible = "isil,isl29018", },
>>>>>>> refs/remotes/origin/master
	{ },
};
MODULE_DEVICE_TABLE(of, isl29018_of_match);

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static struct i2c_driver isl29018_driver = {
	.class	= I2C_CLASS_HWMON,
	.driver	 = {
			.name = "isl29018",
<<<<<<< HEAD
			.owner = THIS_MODULE,
<<<<<<< HEAD
=======
			.of_match_table = isl29018_of_match,
>>>>>>> refs/remotes/origin/cm-10.0
		    },
	.probe	 = isl29018_probe,
	.remove	 = __devexit_p(isl29018_remove),
	.id_table = isl29018_id,
};
<<<<<<< HEAD

static int __init isl29018_init(void)
{
	return i2c_add_driver(&isl29018_driver);
}

static void __exit isl29018_exit(void)
{
	i2c_del_driver(&isl29018_driver);
}

module_init(isl29018_init);
module_exit(isl29018_exit);
=======
module_i2c_driver(isl29018_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			.pm = ISL29018_PM_OPS,
			.owner = THIS_MODULE,
			.of_match_table = isl29018_of_match,
		    },
	.probe	 = isl29018_probe,
	.id_table = isl29018_id,
};
module_i2c_driver(isl29018_driver);
>>>>>>> refs/remotes/origin/master

MODULE_DESCRIPTION("ISL29018 Ambient Light Sensor driver");
MODULE_LICENSE("GPL");
