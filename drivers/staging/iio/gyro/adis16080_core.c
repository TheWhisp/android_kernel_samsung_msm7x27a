/*
 * ADIS16080/100 Yaw Rate Gyroscope with SPI driver
 *
 * Copyright 2010 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */
<<<<<<< HEAD
#include <linux/gpio.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/spi/spi.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
<<<<<<< HEAD

#include "../iio.h"
#include "../sysfs.h"
#include "gyro.h"
#include "../adc/adc.h"
=======
#include <linux/module.h>

#include "../iio.h"
#include "../sysfs.h"
>>>>>>> refs/remotes/origin/cm-10.0

#define ADIS16080_DIN_GYRO   (0 << 10) /* Gyroscope output */
#define ADIS16080_DIN_TEMP   (1 << 10) /* Temperature output */
#define ADIS16080_DIN_AIN1   (2 << 10)
#define ADIS16080_DIN_AIN2   (3 << 10)

/*
 * 1: Write contents on DIN to control register.
 * 0: No changes to control register.
 */

#define ADIS16080_DIN_WRITE  (1 << 15)

/**
 * struct adis16080_state - device instance specific data
 * @us:			actual spi_device to write data
<<<<<<< HEAD
 * @indio_dev:		industrial I/O device structure
=======
>>>>>>> refs/remotes/origin/cm-10.0
 * @buf:		transmit or receive buffer
 * @buf_lock:		mutex to protect tx and rx
 **/
struct adis16080_state {
	struct spi_device		*us;
<<<<<<< HEAD
	struct iio_dev			*indio_dev;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	struct mutex			buf_lock;

	u8 buf[2] ____cacheline_aligned;
};

<<<<<<< HEAD
static int adis16080_spi_write(struct device *dev,
		u16 val)
{
	int ret;
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct adis16080_state *st = iio_dev_get_devdata(indio_dev);
=======
static int adis16080_spi_write(struct iio_dev *indio_dev,
		u16 val)
{
	int ret;
	struct adis16080_state *st = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0

	mutex_lock(&st->buf_lock);
	st->buf[0] = val >> 8;
	st->buf[1] = val;

	ret = spi_write(st->us, st->buf, 2);
	mutex_unlock(&st->buf_lock);

	return ret;
}

<<<<<<< HEAD
static int adis16080_spi_read(struct device *dev,
		u16 *val)
{
	int ret;
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct adis16080_state *st = iio_dev_get_devdata(indio_dev);
=======
static int adis16080_spi_read(struct iio_dev *indio_dev,
			      u16 *val)
{
	int ret;
	struct adis16080_state *st = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0

	mutex_lock(&st->buf_lock);

	ret = spi_read(st->us, st->buf, 2);

	if (ret == 0)
		*val = ((st->buf[0] & 0xF) << 8) | st->buf[1];
	mutex_unlock(&st->buf_lock);

	return ret;
}

<<<<<<< HEAD
static ssize_t adis16080_read(struct device *dev,
		struct device_attribute *attr,
		char *buf)
{
	struct iio_dev_attr *this_attr = to_iio_dev_attr(attr);
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	u16 val = 0;
	ssize_t ret;

	/* Take the iio_dev status lock */
	mutex_lock(&indio_dev->mlock);
	ret = adis16080_spi_write(dev,
				  this_attr->address | ADIS16080_DIN_WRITE);
	if (ret < 0)
		goto error_ret;
	ret =  adis16080_spi_read(dev, &val);
error_ret:
	mutex_unlock(&indio_dev->mlock);

	if (ret == 0)
		return sprintf(buf, "%d\n", val);
	else
		return ret;
}
static IIO_DEV_ATTR_GYRO_Z(adis16080_read, ADIS16080_DIN_GYRO);
static IIO_DEVICE_ATTR(temp_raw, S_IRUGO, adis16080_read, NULL,
		       ADIS16080_DIN_TEMP);
static IIO_DEV_ATTR_IN_RAW(0, adis16080_read, ADIS16080_DIN_AIN1);
static IIO_DEV_ATTR_IN_RAW(1, adis16080_read, ADIS16080_DIN_AIN2);

static struct attribute *adis16080_attributes[] = {
	&iio_dev_attr_gyro_z_raw.dev_attr.attr,
	&iio_dev_attr_temp_raw.dev_attr.attr,
	&iio_dev_attr_in0_raw.dev_attr.attr,
	&iio_dev_attr_in1_raw.dev_attr.attr,
	NULL
};

static const struct attribute_group adis16080_attribute_group = {
	.attrs = adis16080_attributes,
};

static const struct iio_info adis16080_info = {
	.attrs = &adis16080_attribute_group,
=======
static int adis16080_read_raw(struct iio_dev *indio_dev,
			     struct iio_chan_spec const *chan,
			     int *val,
			     int *val2,
			     long mask)
{
	int ret = -EINVAL;
	u16 ut;
	/* Take the iio_dev status lock */

	mutex_lock(&indio_dev->mlock);
	switch (mask) {
	case 0:
		ret = adis16080_spi_write(indio_dev,
					  chan->address |
					  ADIS16080_DIN_WRITE);
		if (ret < 0)
			break;
		ret = adis16080_spi_read(indio_dev, &ut);
		if (ret < 0)
			break;
		*val = ut;
		ret = IIO_VAL_INT;
		break;
	}
	mutex_unlock(&indio_dev->mlock);

	return ret;
}

static const struct iio_chan_spec adis16080_channels[] = {
	{
		.type = IIO_ANGL_VEL,
		.modified = 1,
		.channel2 = IIO_MOD_Z,
		.address = ADIS16080_DIN_GYRO,
	}, {
		.type = IIO_VOLTAGE,
		.indexed = 1,
		.channel = 0,
		.address = ADIS16080_DIN_AIN1,
	}, {
		.type = IIO_VOLTAGE,
		.indexed = 1,
		.channel = 1,
		.address = ADIS16080_DIN_AIN2,
	}, {
		.type = IIO_TEMP,
		.indexed = 1,
		.channel = 0,
		.address = ADIS16080_DIN_TEMP,
	}
};

static const struct iio_info adis16080_info = {
	.read_raw = &adis16080_read_raw,
>>>>>>> refs/remotes/origin/cm-10.0
	.driver_module = THIS_MODULE,
};

static int __devinit adis16080_probe(struct spi_device *spi)
{
<<<<<<< HEAD
	int ret, regdone = 0;
	struct adis16080_state *st = kzalloc(sizeof *st, GFP_KERNEL);
	if (!st) {
		ret =  -ENOMEM;
		goto error_ret;
	}
	/* this is only used for removal purposes */
	spi_set_drvdata(spi, st);
=======
	int ret;
	struct adis16080_state *st;
	struct iio_dev *indio_dev;

	/* setup the industrialio driver allocated elements */
	indio_dev = iio_allocate_device(sizeof(*st));
	if (indio_dev == NULL) {
		ret = -ENOMEM;
		goto error_ret;
	}
	st = iio_priv(indio_dev);
	/* this is only used for removal purposes */
	spi_set_drvdata(spi, indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0

	/* Allocate the comms buffers */
	st->us = spi;
	mutex_init(&st->buf_lock);
<<<<<<< HEAD
	/* setup the industrialio driver allocated elements */
	st->indio_dev = iio_allocate_device(0);
	if (st->indio_dev == NULL) {
		ret = -ENOMEM;
		goto error_free_st;
	}

	st->indio_dev->name = spi->dev.driver->name;
	st->indio_dev->dev.parent = &spi->dev;
	st->indio_dev->info = &adis16080_info;
	st->indio_dev->dev_data = (void *)(st);
	st->indio_dev->modes = INDIO_DIRECT_MODE;

	ret = iio_device_register(st->indio_dev);
	if (ret)
		goto error_free_dev;
	regdone = 1;

	return 0;

error_free_dev:
	if (regdone)
		iio_device_unregister(st->indio_dev);
	else
		iio_free_device(st->indio_dev);
error_free_st:
	kfree(st);
=======

	indio_dev->name = spi->dev.driver->name;
	indio_dev->channels = adis16080_channels;
	indio_dev->num_channels = ARRAY_SIZE(adis16080_channels);
	indio_dev->dev.parent = &spi->dev;
	indio_dev->info = &adis16080_info;
	indio_dev->modes = INDIO_DIRECT_MODE;

	ret = iio_device_register(indio_dev);
	if (ret)
		goto error_free_dev;
	return 0;

error_free_dev:
	iio_free_device(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
error_ret:
	return ret;
}

/* fixme, confirm ordering in this function */
static int adis16080_remove(struct spi_device *spi)
{
<<<<<<< HEAD
	struct adis16080_state *st = spi_get_drvdata(spi);
	struct iio_dev *indio_dev = st->indio_dev;

	iio_device_unregister(indio_dev);
	kfree(st);
=======
	iio_device_unregister(spi_get_drvdata(spi));
	iio_free_device(spi_get_drvdata(spi));
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
}

static struct spi_driver adis16080_driver = {
	.driver = {
		.name = "adis16080",
		.owner = THIS_MODULE,
	},
	.probe = adis16080_probe,
	.remove = __devexit_p(adis16080_remove),
};
<<<<<<< HEAD

static __init int adis16080_init(void)
{
	return spi_register_driver(&adis16080_driver);
}
module_init(adis16080_init);

static __exit void adis16080_exit(void)
{
	spi_unregister_driver(&adis16080_driver);
}
module_exit(adis16080_exit);
=======
module_spi_driver(adis16080_driver);
>>>>>>> refs/remotes/origin/cm-10.0

MODULE_AUTHOR("Barry Song <21cnbao@gmail.com>");
MODULE_DESCRIPTION("Analog Devices ADIS16080/100 Yaw Rate Gyroscope Driver");
MODULE_LICENSE("GPL v2");
<<<<<<< HEAD
=======
MODULE_ALIAS("spi:adis16080");
>>>>>>> refs/remotes/origin/cm-10.0
