/*
 * AD5446 SPI DAC driver
 *
 * Copyright 2010 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/spi/spi.h>
#include <linux/regulator/consumer.h>
#include <linux/err.h>
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0

#include "../iio.h"
#include "../sysfs.h"
#include "dac.h"

#include "ad5446.h"

static void ad5446_store_sample(struct ad5446_state *st, unsigned val)
{
<<<<<<< HEAD
	st->data.d16 = cpu_to_be16(AD5446_LOAD |
					(val << st->chip_info->left_shift));
=======
	st->data.d16 = cpu_to_be16(AD5446_LOAD | val);
>>>>>>> refs/remotes/origin/cm-10.0
}

static void ad5542_store_sample(struct ad5446_state *st, unsigned val)
{
<<<<<<< HEAD
	st->data.d16 = cpu_to_be16(val << st->chip_info->left_shift);
=======
	st->data.d16 = cpu_to_be16(val);
>>>>>>> refs/remotes/origin/cm-10.0
}

static void ad5620_store_sample(struct ad5446_state *st, unsigned val)
{
<<<<<<< HEAD
	st->data.d16 = cpu_to_be16(AD5620_LOAD |
					(val << st->chip_info->left_shift));
=======
	st->data.d16 = cpu_to_be16(AD5620_LOAD | val);
>>>>>>> refs/remotes/origin/cm-10.0
}

static void ad5660_store_sample(struct ad5446_state *st, unsigned val)
{
	val |= AD5660_LOAD;
	st->data.d24[0] = (val >> 16) & 0xFF;
	st->data.d24[1] = (val >> 8) & 0xFF;
	st->data.d24[2] = val & 0xFF;
}

static void ad5620_store_pwr_down(struct ad5446_state *st, unsigned mode)
{
	st->data.d16 = cpu_to_be16(mode << 14);
}

static void ad5660_store_pwr_down(struct ad5446_state *st, unsigned mode)
{
	unsigned val = mode << 16;

	st->data.d24[0] = (val >> 16) & 0xFF;
	st->data.d24[1] = (val >> 8) & 0xFF;
	st->data.d24[2] = val & 0xFF;
}

<<<<<<< HEAD
static ssize_t ad5446_write(struct device *dev,
		struct device_attribute *attr,
		const char *buf,
		size_t len)
{
	struct iio_dev *dev_info = dev_get_drvdata(dev);
	struct ad5446_state *st = dev_info->dev_data;
	int ret;
	long val;

	ret = strict_strtol(buf, 10, &val);
	if (ret)
		goto error_ret;

	if (val > RES_MASK(st->chip_info->bits)) {
		ret = -EINVAL;
		goto error_ret;
	}

	mutex_lock(&dev_info->mlock);
	st->cached_val = val;
	st->chip_info->store_sample(st, val);
	ret = spi_sync(st->spi, &st->msg);
	mutex_unlock(&dev_info->mlock);

error_ret:
	return ret ? ret : len;
}

static IIO_DEV_ATTR_OUT_RAW(0, ad5446_write, 0);

static ssize_t ad5446_show_scale(struct device *dev,
				struct device_attribute *attr,
				char *buf)
{
	struct iio_dev *dev_info = dev_get_drvdata(dev);
	struct ad5446_state *st = iio_dev_get_devdata(dev_info);
	/* Corresponds to Vref / 2^(bits) */
	unsigned int scale_uv = (st->vref_mv * 1000) >> st->chip_info->bits;

	return sprintf(buf, "%d.%03d\n", scale_uv / 1000, scale_uv % 1000);
}
static IIO_DEVICE_ATTR(out_scale, S_IRUGO, ad5446_show_scale, NULL, 0);

=======
>>>>>>> refs/remotes/origin/cm-10.0
static ssize_t ad5446_write_powerdown_mode(struct device *dev,
				       struct device_attribute *attr,
				       const char *buf, size_t len)
{
<<<<<<< HEAD
	struct iio_dev *dev_info = dev_get_drvdata(dev);
	struct ad5446_state *st = dev_info->dev_data;
=======
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct ad5446_state *st = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0

	if (sysfs_streq(buf, "1kohm_to_gnd"))
		st->pwr_down_mode = MODE_PWRDWN_1k;
	else if (sysfs_streq(buf, "100kohm_to_gnd"))
		st->pwr_down_mode = MODE_PWRDWN_100k;
	else if (sysfs_streq(buf, "three_state"))
		st->pwr_down_mode = MODE_PWRDWN_TRISTATE;
	else
		return -EINVAL;

	return len;
}

static ssize_t ad5446_read_powerdown_mode(struct device *dev,
				      struct device_attribute *attr, char *buf)
{
<<<<<<< HEAD
	struct iio_dev *dev_info = dev_get_drvdata(dev);
	struct ad5446_state *st = dev_info->dev_data;
=======
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct ad5446_state *st = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0

	char mode[][15] = {"", "1kohm_to_gnd", "100kohm_to_gnd", "three_state"};

	return sprintf(buf, "%s\n", mode[st->pwr_down_mode]);
}

static ssize_t ad5446_read_dac_powerdown(struct device *dev,
					   struct device_attribute *attr,
					   char *buf)
{
<<<<<<< HEAD
	struct iio_dev *dev_info = dev_get_drvdata(dev);
	struct ad5446_state *st = dev_info->dev_data;
=======
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct ad5446_state *st = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0

	return sprintf(buf, "%d\n", st->pwr_down);
}

static ssize_t ad5446_write_dac_powerdown(struct device *dev,
					    struct device_attribute *attr,
					    const char *buf, size_t len)
{
<<<<<<< HEAD
	struct iio_dev *dev_info = dev_get_drvdata(dev);
	struct ad5446_state *st = dev_info->dev_data;
=======
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct ad5446_state *st = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned long readin;
	int ret;

	ret = strict_strtol(buf, 10, &readin);
	if (ret)
		return ret;

	if (readin > 1)
		ret = -EINVAL;

<<<<<<< HEAD
	mutex_lock(&dev_info->mlock);
=======
	mutex_lock(&indio_dev->mlock);
>>>>>>> refs/remotes/origin/cm-10.0
	st->pwr_down = readin;

	if (st->pwr_down)
		st->chip_info->store_pwr_down(st, st->pwr_down_mode);
	else
		st->chip_info->store_sample(st, st->cached_val);

	ret = spi_sync(st->spi, &st->msg);
<<<<<<< HEAD
	mutex_unlock(&dev_info->mlock);
=======
	mutex_unlock(&indio_dev->mlock);
>>>>>>> refs/remotes/origin/cm-10.0

	return ret ? ret : len;
}

<<<<<<< HEAD
static IIO_DEVICE_ATTR(out_powerdown_mode, S_IRUGO | S_IWUSR,
			ad5446_read_powerdown_mode,
			ad5446_write_powerdown_mode, 0);

static IIO_CONST_ATTR(out_powerdown_mode_available,
			"1kohm_to_gnd 100kohm_to_gnd three_state");

static IIO_DEVICE_ATTR(out0_powerdown, S_IRUGO | S_IWUSR,
=======
static IIO_DEVICE_ATTR(out_voltage_powerdown_mode, S_IRUGO | S_IWUSR,
			ad5446_read_powerdown_mode,
			ad5446_write_powerdown_mode, 0);

static IIO_CONST_ATTR(out_voltage_powerdown_mode_available,
			"1kohm_to_gnd 100kohm_to_gnd three_state");

static IIO_DEVICE_ATTR(out_voltage0_powerdown, S_IRUGO | S_IWUSR,
>>>>>>> refs/remotes/origin/cm-10.0
			ad5446_read_dac_powerdown,
			ad5446_write_dac_powerdown, 0);

static struct attribute *ad5446_attributes[] = {
<<<<<<< HEAD
	&iio_dev_attr_out0_raw.dev_attr.attr,
	&iio_dev_attr_out_scale.dev_attr.attr,
	&iio_dev_attr_out0_powerdown.dev_attr.attr,
	&iio_dev_attr_out_powerdown_mode.dev_attr.attr,
	&iio_const_attr_out_powerdown_mode_available.dev_attr.attr,
	NULL,
};

static mode_t ad5446_attr_is_visible(struct kobject *kobj,
				     struct attribute *attr, int n)
{
	struct device *dev = container_of(kobj, struct device, kobj);
	struct iio_dev *dev_info = dev_get_drvdata(dev);
	struct ad5446_state *st = iio_dev_get_devdata(dev_info);

	mode_t mode = attr->mode;

	if (!st->chip_info->store_pwr_down &&
		(attr == &iio_dev_attr_out0_powerdown.dev_attr.attr ||
		attr == &iio_dev_attr_out_powerdown_mode.dev_attr.attr ||
		attr ==
		&iio_const_attr_out_powerdown_mode_available.dev_attr.attr))
		mode = 0;

	return mode;
}

static const struct attribute_group ad5446_attribute_group = {
	.attrs = ad5446_attributes,
	.is_visible = ad5446_attr_is_visible,
};

static const struct ad5446_chip_info ad5446_chip_info_tbl[] = {
	[ID_AD5444] = {
		.bits = 12,
		.storagebits = 16,
		.left_shift = 2,
		.store_sample = ad5446_store_sample,
	},
	[ID_AD5446] = {
		.bits = 14,
		.storagebits = 16,
		.left_shift = 0,
		.store_sample = ad5446_store_sample,
	},
	[ID_AD5541A] = {
		.bits = 16,
		.storagebits = 16,
		.left_shift = 0,
		.store_sample = ad5542_store_sample,
	},
	[ID_AD5542A] = {
		.bits = 16,
		.storagebits = 16,
		.left_shift = 0,
		.store_sample = ad5542_store_sample,
	},
	[ID_AD5543] = {
		.bits = 16,
		.storagebits = 16,
		.left_shift = 0,
		.store_sample = ad5542_store_sample,
	},
	[ID_AD5512A] = {
		.bits = 12,
		.storagebits = 16,
		.left_shift = 4,
		.store_sample = ad5542_store_sample,
	},
	[ID_AD5553] = {
		.bits = 14,
		.storagebits = 16,
		.left_shift = 0,
		.store_sample = ad5542_store_sample,
	},
	[ID_AD5601] = {
		.bits = 8,
		.storagebits = 16,
		.left_shift = 6,
=======
	&iio_dev_attr_out_voltage0_powerdown.dev_attr.attr,
	&iio_dev_attr_out_voltage_powerdown_mode.dev_attr.attr,
	&iio_const_attr_out_voltage_powerdown_mode_available.dev_attr.attr,
	NULL,
};

static const struct attribute_group ad5446_attribute_group = {
	.attrs = ad5446_attributes,
};

#define AD5446_CHANNEL(bits, storage, shift) { \
	.type = IIO_VOLTAGE, \
	.indexed = 1, \
	.output = 1, \
	.channel = 0, \
	.info_mask = IIO_CHAN_INFO_SCALE_SHARED_BIT, \
	.scan_type = IIO_ST('u', (bits), (storage), (shift)) \
}

static const struct ad5446_chip_info ad5446_chip_info_tbl[] = {
	[ID_AD5444] = {
		.channel = AD5446_CHANNEL(12, 16, 2),
		.store_sample = ad5446_store_sample,
	},
	[ID_AD5446] = {
		.channel = AD5446_CHANNEL(14, 16, 0),
		.store_sample = ad5446_store_sample,
	},
	[ID_AD5541A] = {
		.channel = AD5446_CHANNEL(16, 16, 0),
		.store_sample = ad5542_store_sample,
	},
	[ID_AD5542A] = {
		.channel = AD5446_CHANNEL(16, 16, 0),
		.store_sample = ad5542_store_sample,
	},
	[ID_AD5543] = {
		.channel = AD5446_CHANNEL(16, 16, 0),
		.store_sample = ad5542_store_sample,
	},
	[ID_AD5512A] = {
		.channel = AD5446_CHANNEL(12, 16, 4),
		.store_sample = ad5542_store_sample,
	},
	[ID_AD5553] = {
		.channel = AD5446_CHANNEL(14, 16, 0),
		.store_sample = ad5542_store_sample,
	},
	[ID_AD5601] = {
		.channel = AD5446_CHANNEL(8, 16, 6),
>>>>>>> refs/remotes/origin/cm-10.0
		.store_sample = ad5542_store_sample,
		.store_pwr_down = ad5620_store_pwr_down,
	},
	[ID_AD5611] = {
<<<<<<< HEAD
		.bits = 10,
		.storagebits = 16,
		.left_shift = 4,
=======
		.channel = AD5446_CHANNEL(10, 16, 4),
>>>>>>> refs/remotes/origin/cm-10.0
		.store_sample = ad5542_store_sample,
		.store_pwr_down = ad5620_store_pwr_down,
	},
	[ID_AD5621] = {
<<<<<<< HEAD
		.bits = 12,
		.storagebits = 16,
		.left_shift = 2,
=======
		.channel = AD5446_CHANNEL(12, 16, 2),
>>>>>>> refs/remotes/origin/cm-10.0
		.store_sample = ad5542_store_sample,
		.store_pwr_down = ad5620_store_pwr_down,
	},
	[ID_AD5620_2500] = {
<<<<<<< HEAD
		.bits = 12,
		.storagebits = 16,
		.left_shift = 2,
=======
		.channel = AD5446_CHANNEL(12, 16, 2),
>>>>>>> refs/remotes/origin/cm-10.0
		.int_vref_mv = 2500,
		.store_sample = ad5620_store_sample,
		.store_pwr_down = ad5620_store_pwr_down,
	},
	[ID_AD5620_1250] = {
<<<<<<< HEAD
		.bits = 12,
		.storagebits = 16,
		.left_shift = 2,
=======
		.channel = AD5446_CHANNEL(12, 16, 2),
>>>>>>> refs/remotes/origin/cm-10.0
		.int_vref_mv = 1250,
		.store_sample = ad5620_store_sample,
		.store_pwr_down = ad5620_store_pwr_down,
	},
	[ID_AD5640_2500] = {
<<<<<<< HEAD
		.bits = 14,
		.storagebits = 16,
		.left_shift = 0,
=======
		.channel = AD5446_CHANNEL(14, 16, 0),
>>>>>>> refs/remotes/origin/cm-10.0
		.int_vref_mv = 2500,
		.store_sample = ad5620_store_sample,
		.store_pwr_down = ad5620_store_pwr_down,
	},
	[ID_AD5640_1250] = {
<<<<<<< HEAD
		.bits = 14,
		.storagebits = 16,
		.left_shift = 0,
=======
		.channel = AD5446_CHANNEL(14, 16, 0),
>>>>>>> refs/remotes/origin/cm-10.0
		.int_vref_mv = 1250,
		.store_sample = ad5620_store_sample,
		.store_pwr_down = ad5620_store_pwr_down,
	},
	[ID_AD5660_2500] = {
<<<<<<< HEAD
		.bits = 16,
		.storagebits = 24,
		.left_shift = 0,
=======
		.channel = AD5446_CHANNEL(16, 16, 0),
>>>>>>> refs/remotes/origin/cm-10.0
		.int_vref_mv = 2500,
		.store_sample = ad5660_store_sample,
		.store_pwr_down = ad5660_store_pwr_down,
	},
	[ID_AD5660_1250] = {
<<<<<<< HEAD
		.bits = 16,
		.storagebits = 24,
		.left_shift = 0,
=======
		.channel = AD5446_CHANNEL(16, 16, 0),
>>>>>>> refs/remotes/origin/cm-10.0
		.int_vref_mv = 1250,
		.store_sample = ad5660_store_sample,
		.store_pwr_down = ad5660_store_pwr_down,
	},
};

<<<<<<< HEAD
static const struct iio_info ad5446_info = {
=======
static int ad5446_read_raw(struct iio_dev *indio_dev,
			   struct iio_chan_spec const *chan,
			   int *val,
			   int *val2,
			   long m)
{
	struct ad5446_state *st = iio_priv(indio_dev);
	unsigned long scale_uv;

	switch (m) {
	case IIO_CHAN_INFO_SCALE:
		scale_uv = (st->vref_mv * 1000) >> chan->scan_type.realbits;
		*val =  scale_uv / 1000;
		*val2 = (scale_uv % 1000) * 1000;
		return IIO_VAL_INT_PLUS_MICRO;

	}
	return -EINVAL;
}

static int ad5446_write_raw(struct iio_dev *indio_dev,
			       struct iio_chan_spec const *chan,
			       int val,
			       int val2,
			       long mask)
{
	struct ad5446_state *st = iio_priv(indio_dev);
	int ret;

	switch (mask) {
	case 0:
		if (val >= (1 << chan->scan_type.realbits) || val < 0)
			return -EINVAL;

		val <<= chan->scan_type.shift;
		mutex_lock(&indio_dev->mlock);
		st->cached_val = val;
		st->chip_info->store_sample(st, val);
		ret = spi_sync(st->spi, &st->msg);
		mutex_unlock(&indio_dev->mlock);
		break;
	default:
		ret = -EINVAL;
	}

	return ret;
}

static const struct iio_info ad5446_info = {
	.read_raw = ad5446_read_raw,
	.write_raw = ad5446_write_raw,
>>>>>>> refs/remotes/origin/cm-10.0
	.attrs = &ad5446_attribute_group,
	.driver_module = THIS_MODULE,
};

<<<<<<< HEAD
static int __devinit ad5446_probe(struct spi_device *spi)
{
	struct ad5446_state *st;
	int ret, voltage_uv = 0;

	st = kzalloc(sizeof(*st), GFP_KERNEL);
	if (st == NULL) {
		ret = -ENOMEM;
		goto error_ret;
	}

	st->reg = regulator_get(&spi->dev, "vcc");
	if (!IS_ERR(st->reg)) {
		ret = regulator_enable(st->reg);
		if (ret)
			goto error_put_reg;

		voltage_uv = regulator_get_voltage(st->reg);
	}

	st->chip_info =
		&ad5446_chip_info_tbl[spi_get_device_id(spi)->driver_data];

	spi_set_drvdata(spi, st);

	st->spi = spi;

	st->indio_dev = iio_allocate_device(0);
	if (st->indio_dev == NULL) {
		ret = -ENOMEM;
		goto error_disable_reg;
	}

	/* Estabilish that the iio_dev is a child of the spi device */
	st->indio_dev->dev.parent = &spi->dev;
	st->indio_dev->name = spi_get_device_id(spi)->name;
	st->indio_dev->info = &ad5446_info;
	st->indio_dev->dev_data = (void *)(st);
	st->indio_dev->modes = INDIO_DIRECT_MODE;
=======
static const struct iio_info ad5446_info_no_pwr_down = {
	.read_raw = ad5446_read_raw,
	.write_raw = ad5446_write_raw,
	.driver_module = THIS_MODULE,
};

static int __devinit ad5446_probe(struct spi_device *spi)
{
	struct ad5446_state *st;
	struct iio_dev *indio_dev;
	struct regulator *reg;
	int ret, voltage_uv = 0;

	reg = regulator_get(&spi->dev, "vcc");
	if (!IS_ERR(reg)) {
		ret = regulator_enable(reg);
		if (ret)
			goto error_put_reg;

		voltage_uv = regulator_get_voltage(reg);
	}

	indio_dev = iio_allocate_device(sizeof(*st));
	if (indio_dev == NULL) {
		ret = -ENOMEM;
		goto error_disable_reg;
	}
	st = iio_priv(indio_dev);
	st->chip_info =
		&ad5446_chip_info_tbl[spi_get_device_id(spi)->driver_data];

	spi_set_drvdata(spi, indio_dev);
	st->reg = reg;
	st->spi = spi;

	/* Establish that the iio_dev is a child of the spi device */
	indio_dev->dev.parent = &spi->dev;
	indio_dev->name = spi_get_device_id(spi)->name;
	if (st->chip_info->store_pwr_down)
		indio_dev->info = &ad5446_info;
	else
		indio_dev->info = &ad5446_info_no_pwr_down;
	indio_dev->modes = INDIO_DIRECT_MODE;
	indio_dev->channels = &st->chip_info->channel;
	indio_dev->num_channels = 1;
>>>>>>> refs/remotes/origin/cm-10.0

	/* Setup default message */

	st->xfer.tx_buf = &st->data;
<<<<<<< HEAD
	st->xfer.len = st->chip_info->storagebits / 8;
=======
	st->xfer.len = st->chip_info->channel.scan_type.storagebits / 8;
>>>>>>> refs/remotes/origin/cm-10.0

	spi_message_init(&st->msg);
	spi_message_add_tail(&st->xfer, &st->msg);

	switch (spi_get_device_id(spi)->driver_data) {
	case ID_AD5620_2500:
	case ID_AD5620_1250:
	case ID_AD5640_2500:
	case ID_AD5640_1250:
	case ID_AD5660_2500:
	case ID_AD5660_1250:
		st->vref_mv = st->chip_info->int_vref_mv;
		break;
	default:
		if (voltage_uv)
			st->vref_mv = voltage_uv / 1000;
		else
			dev_warn(&spi->dev,
				 "reference voltage unspecified\n");
	}

<<<<<<< HEAD
	ret = iio_device_register(st->indio_dev);
=======
	ret = iio_device_register(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret)
		goto error_free_device;

	return 0;

error_free_device:
<<<<<<< HEAD
	iio_free_device(st->indio_dev);
error_disable_reg:
	if (!IS_ERR(st->reg))
		regulator_disable(st->reg);
error_put_reg:
	if (!IS_ERR(st->reg))
		regulator_put(st->reg);
	kfree(st);
error_ret:
=======
	iio_free_device(indio_dev);
error_disable_reg:
	if (!IS_ERR(reg))
		regulator_disable(reg);
error_put_reg:
	if (!IS_ERR(reg))
		regulator_put(reg);

>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
}

static int ad5446_remove(struct spi_device *spi)
{
<<<<<<< HEAD
	struct ad5446_state *st = spi_get_drvdata(spi);
	struct iio_dev *indio_dev = st->indio_dev;
=======
	struct iio_dev *indio_dev = spi_get_drvdata(spi);
	struct ad5446_state *st = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0

	iio_device_unregister(indio_dev);
	if (!IS_ERR(st->reg)) {
		regulator_disable(st->reg);
		regulator_put(st->reg);
	}
<<<<<<< HEAD
	kfree(st);
=======
	iio_free_device(indio_dev);

>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

static const struct spi_device_id ad5446_id[] = {
	{"ad5444", ID_AD5444},
	{"ad5446", ID_AD5446},
	{"ad5512a", ID_AD5512A},
	{"ad5541a", ID_AD5541A},
	{"ad5542a", ID_AD5542A},
	{"ad5543", ID_AD5543},
	{"ad5553", ID_AD5553},
	{"ad5601", ID_AD5601},
	{"ad5611", ID_AD5611},
	{"ad5621", ID_AD5621},
	{"ad5620-2500", ID_AD5620_2500}, /* AD5620/40/60: */
	{"ad5620-1250", ID_AD5620_1250}, /* part numbers may look differently */
	{"ad5640-2500", ID_AD5640_2500},
	{"ad5640-1250", ID_AD5640_1250},
	{"ad5660-2500", ID_AD5660_2500},
	{"ad5660-1250", ID_AD5660_1250},
	{}
};
<<<<<<< HEAD
=======
MODULE_DEVICE_TABLE(spi, ad5446_id);
>>>>>>> refs/remotes/origin/cm-10.0

static struct spi_driver ad5446_driver = {
	.driver = {
		.name	= "ad5446",
<<<<<<< HEAD
		.bus	= &spi_bus_type,
=======
>>>>>>> refs/remotes/origin/cm-10.0
		.owner	= THIS_MODULE,
	},
	.probe		= ad5446_probe,
	.remove		= __devexit_p(ad5446_remove),
	.id_table	= ad5446_id,
};
<<<<<<< HEAD

static int __init ad5446_init(void)
{
	return spi_register_driver(&ad5446_driver);
}
module_init(ad5446_init);

static void __exit ad5446_exit(void)
{
	spi_unregister_driver(&ad5446_driver);
}
module_exit(ad5446_exit);
=======
module_spi_driver(ad5446_driver);
>>>>>>> refs/remotes/origin/cm-10.0

MODULE_AUTHOR("Michael Hennerich <hennerich@blackfin.uclinux.org>");
MODULE_DESCRIPTION("Analog Devices AD5444/AD5446 DAC");
MODULE_LICENSE("GPL v2");
<<<<<<< HEAD
MODULE_ALIAS("spi:ad5446");
=======
>>>>>>> refs/remotes/origin/cm-10.0
