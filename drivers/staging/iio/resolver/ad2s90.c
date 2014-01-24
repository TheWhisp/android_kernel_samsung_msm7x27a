/*
 * ad2s90.c simple support for the ADI Resolver to Digital Converters: AD2S90
 *
 * Copyright (c) 2010-2010 Analog Devices Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/device.h>
#include <linux/spi/spi.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/cm-10.0

#include "../iio.h"
#include "../sysfs.h"

<<<<<<< HEAD
#define DRV_NAME "ad2s90"

struct ad2s90_state {
	struct mutex lock;
	struct iio_dev *idev;
	struct spi_device *sdev;
	u8 rx[2];
	u8 tx[2];
};

static ssize_t ad2s90_show_angular(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct spi_message msg;
	struct spi_transfer xfer;
	int ret;
	ssize_t len = 0;
	u16 val;
	struct iio_dev *idev = dev_get_drvdata(dev);
	struct ad2s90_state *st = idev->dev_data;

	xfer.len = 1;
	xfer.tx_buf = st->tx;
	xfer.rx_buf = st->rx;
	mutex_lock(&st->lock);

	spi_message_init(&msg);
	spi_message_add_tail(&xfer, &msg);
	ret = spi_sync(st->sdev, &msg);
	if (ret)
		goto error_ret;
	val = (((u16)(st->rx[0])) << 4) | ((st->rx[1] & 0xF0) >> 4);
	len = sprintf(buf, "%d\n", val);
error_ret:
	mutex_unlock(&st->lock);

	return ret ? ret : len;
}

#define IIO_DEV_ATTR_SIMPLE_RESOLVER(_show) \
	IIO_DEVICE_ATTR(angular, S_IRUGO, _show, NULL, 0)

static IIO_CONST_ATTR(description,
	"Low Cost, Complete 12-Bit Resolver-to-Digital Converter");
static IIO_DEV_ATTR_SIMPLE_RESOLVER(ad2s90_show_angular);

static struct attribute *ad2s90_attributes[] = {
	&iio_const_attr_description.dev_attr.attr,
	&iio_dev_attr_angular.dev_attr.attr,
	NULL,
};

static const struct attribute_group ad2s90_attribute_group = {
	.name = DRV_NAME,
	.attrs = ad2s90_attributes,
};

static const struct iio_info ad2s90_info = {
	.attrs = &ad2s90_attribute_group,
	.driver_module = THIS_MODULE,
};

static int __devinit ad2s90_probe(struct spi_device *spi)
{
	struct ad2s90_state *st;
	int ret = 0;

	st = kzalloc(sizeof(*st), GFP_KERNEL);
	if (st == NULL) {
		ret = -ENOMEM;
		goto error_ret;
	}
	spi_set_drvdata(spi, st);

	mutex_init(&st->lock);
	st->sdev = spi;

	st->idev = iio_allocate_device(0);
	if (st->idev == NULL) {
		ret = -ENOMEM;
		goto error_free_st;
	}
	st->idev->dev.parent = &spi->dev;

	st->idev->info = &ad2s90_info;
	st->idev->dev_data = (void *)(st);
	st->idev->modes = INDIO_DIRECT_MODE;

	ret = iio_device_register(st->idev);
=======
=======
#include <linux/module.h>

#include <linux/iio/iio.h>
#include <linux/iio/sysfs.h>

>>>>>>> refs/remotes/origin/master
struct ad2s90_state {
	struct mutex lock;
	struct spi_device *sdev;
	u8 rx[2] ____cacheline_aligned;
};

static int ad2s90_read_raw(struct iio_dev *indio_dev,
			   struct iio_chan_spec const *chan,
			   int *val,
			   int *val2,
			   long m)
{
	int ret;
	struct ad2s90_state *st = iio_priv(indio_dev);

	mutex_lock(&st->lock);
	ret = spi_read(st->sdev, st->rx, 2);
	if (ret)
		goto error_ret;
	*val = (((u16)(st->rx[0])) << 4) | ((st->rx[1] & 0xF0) >> 4);

error_ret:
	mutex_unlock(&st->lock);

	return IIO_VAL_INT;
}

static const struct iio_info ad2s90_info = {
	.read_raw = &ad2s90_read_raw,
	.driver_module = THIS_MODULE,
};

static const struct iio_chan_spec ad2s90_chan = {
	.type = IIO_ANGL,
	.indexed = 1,
	.channel = 0,
<<<<<<< HEAD
};

static int __devinit ad2s90_probe(struct spi_device *spi)
=======
	.info_mask_separate = BIT(IIO_CHAN_INFO_RAW),
};

static int ad2s90_probe(struct spi_device *spi)
>>>>>>> refs/remotes/origin/master
{
	struct iio_dev *indio_dev;
	struct ad2s90_state *st;
	int ret = 0;

<<<<<<< HEAD
	indio_dev = iio_allocate_device(sizeof(*st));
	if (indio_dev == NULL) {
		ret = -ENOMEM;
		goto error_ret;
	}
=======
	indio_dev = devm_iio_device_alloc(&spi->dev, sizeof(*st));
	if (!indio_dev)
		return -ENOMEM;
>>>>>>> refs/remotes/origin/master
	st = iio_priv(indio_dev);
	spi_set_drvdata(spi, indio_dev);

	mutex_init(&st->lock);
	st->sdev = spi;
	indio_dev->dev.parent = &spi->dev;
	indio_dev->info = &ad2s90_info;
	indio_dev->modes = INDIO_DIRECT_MODE;
	indio_dev->channels = &ad2s90_chan;
	indio_dev->num_channels = 1;
	indio_dev->name = spi_get_device_id(spi)->name;

	ret = iio_device_register(indio_dev);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret)
		goto error_free_dev;
=======
	if (ret)
		return ret;
>>>>>>> refs/remotes/origin/master

	/* need 600ns between CS and the first falling edge of SCLK */
	spi->max_speed_hz = 830000;
	spi->mode = SPI_MODE_3;
	spi_setup(spi);

	return 0;
<<<<<<< HEAD

error_free_dev:
<<<<<<< HEAD
	iio_free_device(st->idev);
error_free_st:
	kfree(st);
=======
	iio_free_device(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
error_ret:
	return ret;
}

static int __devexit ad2s90_remove(struct spi_device *spi)
{
<<<<<<< HEAD
	struct ad2s90_state *st = spi_get_drvdata(spi);

	iio_device_unregister(st->idev);
	kfree(st);
=======
	iio_device_unregister(spi_get_drvdata(spi));
	iio_free_device(spi_get_drvdata(spi));
>>>>>>> refs/remotes/origin/cm-10.0
=======
}

static int ad2s90_remove(struct spi_device *spi)
{
	iio_device_unregister(spi_get_drvdata(spi));
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
<<<<<<< HEAD
static struct spi_driver ad2s90_driver = {
	.driver = {
		.name = DRV_NAME,
=======
=======
>>>>>>> refs/remotes/origin/master
static const struct spi_device_id ad2s90_id[] = {
	{ "ad2s90" },
	{}
};
MODULE_DEVICE_TABLE(spi, ad2s90_id);

static struct spi_driver ad2s90_driver = {
	.driver = {
		.name = "ad2s90",
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
		.owner = THIS_MODULE,
	},
	.probe = ad2s90_probe,
	.remove = __devexit_p(ad2s90_remove),
<<<<<<< HEAD
};

static __init int ad2s90_spi_init(void)
{
	return spi_register_driver(&ad2s90_driver);
}
module_init(ad2s90_spi_init);

static __exit void ad2s90_spi_exit(void)
{
	spi_unregister_driver(&ad2s90_driver);
}
module_exit(ad2s90_spi_exit);
=======
	.id_table = ad2s90_id,
};
module_spi_driver(ad2s90_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
		.owner = THIS_MODULE,
	},
	.probe = ad2s90_probe,
	.remove = ad2s90_remove,
	.id_table = ad2s90_id,
};
module_spi_driver(ad2s90_driver);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Graff Yang <graff.yang@gmail.com>");
MODULE_DESCRIPTION("Analog Devices AD2S90 Resolver to Digital SPI driver");
MODULE_LICENSE("GPL v2");
