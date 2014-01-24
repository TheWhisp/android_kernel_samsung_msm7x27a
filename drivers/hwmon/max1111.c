/*
 * max1111.c - +2.7V, Low-Power, Multichannel, Serial 8-bit ADCs
 *
 * Based on arch/arm/mach-pxa/corgi_ssp.c
 *
 * Copyright (C) 2004-2005 Richard Purdie
 *
 * Copyright (C) 2008 Marvell International Ltd.
<<<<<<< HEAD
<<<<<<< HEAD
 * 	Eric Miao <eric.miao@marvell.com>
=======
 *	Eric Miao <eric.miao@marvell.com>
>>>>>>> refs/remotes/origin/cm-10.0
=======
 *	Eric Miao <eric.miao@marvell.com>
>>>>>>> refs/remotes/origin/master
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  publishhed by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
#include <linux/spi/spi.h>
#include <linux/slab.h>

<<<<<<< HEAD
=======
enum chips { max1110, max1111, max1112, max1113 };

>>>>>>> refs/remotes/origin/master
#define MAX1111_TX_BUF_SIZE	1
#define MAX1111_RX_BUF_SIZE	2

/* MAX1111 Commands */
#define MAX1111_CTRL_PD0      (1u << 0)
#define MAX1111_CTRL_PD1      (1u << 1)
#define MAX1111_CTRL_SGL      (1u << 2)
#define MAX1111_CTRL_UNI      (1u << 3)
<<<<<<< HEAD
=======
#define MAX1110_CTRL_SEL_SH   (4)
>>>>>>> refs/remotes/origin/master
#define MAX1111_CTRL_SEL_SH   (5)	/* NOTE: bit 4 is ignored */
#define MAX1111_CTRL_STR      (1u << 7)

struct max1111_data {
	struct spi_device	*spi;
	struct device		*hwmon_dev;
	struct spi_message	msg;
	struct spi_transfer	xfer[2];
<<<<<<< HEAD
<<<<<<< HEAD
	uint8_t *tx_buf;
	uint8_t *rx_buf;
=======
	uint8_t tx_buf[MAX1111_TX_BUF_SIZE];
	uint8_t rx_buf[MAX1111_RX_BUF_SIZE];
>>>>>>> refs/remotes/origin/cm-10.0
	struct mutex		drvdata_lock;
	/* protect msg, xfer and buffers from multiple access */
=======
	uint8_t tx_buf[MAX1111_TX_BUF_SIZE];
	uint8_t rx_buf[MAX1111_RX_BUF_SIZE];
	struct mutex		drvdata_lock;
	/* protect msg, xfer and buffers from multiple access */
	int			sel_sh;
	int			lsb;
>>>>>>> refs/remotes/origin/master
};

static int max1111_read(struct device *dev, int channel)
{
	struct max1111_data *data = dev_get_drvdata(dev);
	uint8_t v1, v2;
	int err;

	/* writing to drvdata struct is not thread safe, wait on mutex */
	mutex_lock(&data->drvdata_lock);

<<<<<<< HEAD
	data->tx_buf[0] = (channel << MAX1111_CTRL_SEL_SH) |
=======
	data->tx_buf[0] = (channel << data->sel_sh) |
>>>>>>> refs/remotes/origin/master
		MAX1111_CTRL_PD0 | MAX1111_CTRL_PD1 |
		MAX1111_CTRL_SGL | MAX1111_CTRL_UNI | MAX1111_CTRL_STR;

	err = spi_sync(data->spi, &data->msg);
	if (err < 0) {
		dev_err(dev, "spi_sync failed with %d\n", err);
		mutex_unlock(&data->drvdata_lock);
		return err;
	}

	v1 = data->rx_buf[0];
	v2 = data->rx_buf[1];

	mutex_unlock(&data->drvdata_lock);

	if ((v1 & 0xc0) || (v2 & 0x3f))
		return -EINVAL;

	return (v1 << 2) | (v2 >> 6);
}

#ifdef CONFIG_SHARPSL_PM
static struct max1111_data *the_max1111;

int max1111_read_channel(int channel)
{
	return max1111_read(&the_max1111->spi->dev, channel);
}
EXPORT_SYMBOL(max1111_read_channel);
#endif

/*
 * NOTE: SPI devices do not have a default 'name' attribute, which is
 * likely to be used by hwmon applications to distinguish between
 * different devices, explicitly add a name attribute here.
 */
static ssize_t show_name(struct device *dev,
			 struct device_attribute *attr, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "max1111\n");
=======
	return sprintf(buf, "%s\n", to_spi_device(dev)->modalias);
>>>>>>> refs/remotes/origin/master
}

static ssize_t show_adc(struct device *dev,
			struct device_attribute *attr, char *buf)
{
<<<<<<< HEAD
=======
	struct max1111_data *data = dev_get_drvdata(dev);
>>>>>>> refs/remotes/origin/master
	int channel = to_sensor_dev_attr(attr)->index;
	int ret;

	ret = max1111_read(dev, channel);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
<<<<<<< HEAD
	return sprintf(buf, "%d\n", ret);
}

#define MAX1111_ADC_ATTR(_id)		\
	SENSOR_DEVICE_ATTR(adc##_id##_in, S_IRUGO, show_adc, NULL, _id)
=======
	/*
	 * assume the reference voltage to be 2.048V, with an 8-bit sample,
	 * the LSB weight is 8mV
	 */
	return sprintf(buf, "%d\n", ret * 8);
=======
	/*
	 * Assume the reference voltage to be 2.048V or 4.096V, with an 8-bit
	 * sample. The LSB weight is 8mV or 16mV depending on the chip type.
	 */
	return sprintf(buf, "%d\n", ret * data->lsb);
>>>>>>> refs/remotes/origin/master
}

#define MAX1111_ADC_ATTR(_id)		\
	SENSOR_DEVICE_ATTR(in##_id##_input, S_IRUGO, show_adc, NULL, _id)
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

static DEVICE_ATTR(name, S_IRUGO, show_name, NULL);
static MAX1111_ADC_ATTR(0);
static MAX1111_ADC_ATTR(1);
static MAX1111_ADC_ATTR(2);
static MAX1111_ADC_ATTR(3);
<<<<<<< HEAD

static struct attribute *max1111_attributes[] = {
	&dev_attr_name.attr,
<<<<<<< HEAD
	&sensor_dev_attr_adc0_in.dev_attr.attr,
	&sensor_dev_attr_adc1_in.dev_attr.attr,
	&sensor_dev_attr_adc2_in.dev_attr.attr,
	&sensor_dev_attr_adc3_in.dev_attr.attr,
=======
=======
static MAX1111_ADC_ATTR(4);
static MAX1111_ADC_ATTR(5);
static MAX1111_ADC_ATTR(6);
static MAX1111_ADC_ATTR(7);

static struct attribute *max1111_attributes[] = {
	&dev_attr_name.attr,
>>>>>>> refs/remotes/origin/master
	&sensor_dev_attr_in0_input.dev_attr.attr,
	&sensor_dev_attr_in1_input.dev_attr.attr,
	&sensor_dev_attr_in2_input.dev_attr.attr,
	&sensor_dev_attr_in3_input.dev_attr.attr,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	NULL,
};

static const struct attribute_group max1111_attr_group = {
	.attrs	= max1111_attributes,
};

<<<<<<< HEAD
<<<<<<< HEAD
static int setup_transfer(struct max1111_data *data)
=======
static int __devinit setup_transfer(struct max1111_data *data)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct attribute *max1110_attributes[] = {
	&sensor_dev_attr_in4_input.dev_attr.attr,
	&sensor_dev_attr_in5_input.dev_attr.attr,
	&sensor_dev_attr_in6_input.dev_attr.attr,
	&sensor_dev_attr_in7_input.dev_attr.attr,
	NULL,
};

static const struct attribute_group max1110_attr_group = {
	.attrs	= max1110_attributes,
};

static int setup_transfer(struct max1111_data *data)
>>>>>>> refs/remotes/origin/master
{
	struct spi_message *m;
	struct spi_transfer *x;

<<<<<<< HEAD
<<<<<<< HEAD
	data->tx_buf = kmalloc(MAX1111_TX_BUF_SIZE, GFP_KERNEL);
	if (!data->tx_buf)
		return -ENOMEM;

	data->rx_buf = kmalloc(MAX1111_RX_BUF_SIZE, GFP_KERNEL);
	if (!data->rx_buf) {
		kfree(data->tx_buf);
		return -ENOMEM;
	}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	m = &data->msg;
	x = &data->xfer[0];

	spi_message_init(m);

	x->tx_buf = &data->tx_buf[0];
<<<<<<< HEAD
<<<<<<< HEAD
	x->len = 1;
=======
	x->len = MAX1111_TX_BUF_SIZE;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	x->len = MAX1111_TX_BUF_SIZE;
>>>>>>> refs/remotes/origin/master
	spi_message_add_tail(x, m);

	x++;
	x->rx_buf = &data->rx_buf[0];
<<<<<<< HEAD
<<<<<<< HEAD
	x->len = 2;
=======
	x->len = MAX1111_RX_BUF_SIZE;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	x->len = MAX1111_RX_BUF_SIZE;
>>>>>>> refs/remotes/origin/master
	spi_message_add_tail(x, m);

	return 0;
}

<<<<<<< HEAD
static int __devinit max1111_probe(struct spi_device *spi)
{
=======
static int max1111_probe(struct spi_device *spi)
{
	enum chips chip = spi_get_device_id(spi)->driver_data;
>>>>>>> refs/remotes/origin/master
	struct max1111_data *data;
	int err;

	spi->bits_per_word = 8;
	spi->mode = SPI_MODE_0;
	err = spi_setup(spi);
	if (err < 0)
		return err;

<<<<<<< HEAD
	data = kzalloc(sizeof(struct max1111_data), GFP_KERNEL);
=======
	data = devm_kzalloc(&spi->dev, sizeof(struct max1111_data), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (data == NULL) {
		dev_err(&spi->dev, "failed to allocate memory\n");
		return -ENOMEM;
	}

<<<<<<< HEAD
	err = setup_transfer(data);
	if (err)
		goto err_free_data;
=======
	switch (chip) {
	case max1110:
		data->lsb = 8;
		data->sel_sh = MAX1110_CTRL_SEL_SH;
		break;
	case max1111:
		data->lsb = 8;
		data->sel_sh = MAX1111_CTRL_SEL_SH;
		break;
	case max1112:
		data->lsb = 16;
		data->sel_sh = MAX1110_CTRL_SEL_SH;
		break;
	case max1113:
		data->lsb = 16;
		data->sel_sh = MAX1111_CTRL_SEL_SH;
		break;
	}
	err = setup_transfer(data);
	if (err)
		return err;
>>>>>>> refs/remotes/origin/master

	mutex_init(&data->drvdata_lock);

	data->spi = spi;
	spi_set_drvdata(spi, data);

	err = sysfs_create_group(&spi->dev.kobj, &max1111_attr_group);
	if (err) {
		dev_err(&spi->dev, "failed to create attribute group\n");
<<<<<<< HEAD
<<<<<<< HEAD
		goto err_free_all;
=======
		goto err_free_data;
>>>>>>> refs/remotes/origin/cm-10.0
=======
		return err;
	}
	if (chip == max1110 || chip == max1112) {
		err = sysfs_create_group(&spi->dev.kobj, &max1110_attr_group);
		if (err) {
			dev_err(&spi->dev,
				"failed to create extended attribute group\n");
			goto err_remove;
		}
>>>>>>> refs/remotes/origin/master
	}

	data->hwmon_dev = hwmon_device_register(&spi->dev);
	if (IS_ERR(data->hwmon_dev)) {
		dev_err(&spi->dev, "failed to create hwmon device\n");
		err = PTR_ERR(data->hwmon_dev);
		goto err_remove;
	}

#ifdef CONFIG_SHARPSL_PM
	the_max1111 = data;
#endif
	return 0;

err_remove:
<<<<<<< HEAD
	sysfs_remove_group(&spi->dev.kobj, &max1111_attr_group);
<<<<<<< HEAD
err_free_all:
	kfree(data->rx_buf);
	kfree(data->tx_buf);
=======
>>>>>>> refs/remotes/origin/cm-10.0
err_free_data:
	kfree(data);
	return err;
}

static int __devexit max1111_remove(struct spi_device *spi)
=======
	sysfs_remove_group(&spi->dev.kobj, &max1110_attr_group);
	sysfs_remove_group(&spi->dev.kobj, &max1111_attr_group);
	return err;
}

static int max1111_remove(struct spi_device *spi)
>>>>>>> refs/remotes/origin/master
{
	struct max1111_data *data = spi_get_drvdata(spi);

	hwmon_device_unregister(data->hwmon_dev);
<<<<<<< HEAD
	sysfs_remove_group(&spi->dev.kobj, &max1111_attr_group);
	mutex_destroy(&data->drvdata_lock);
<<<<<<< HEAD
	kfree(data->rx_buf);
	kfree(data->tx_buf);
=======
>>>>>>> refs/remotes/origin/cm-10.0
	kfree(data);
	return 0;
}

=======
	sysfs_remove_group(&spi->dev.kobj, &max1110_attr_group);
	sysfs_remove_group(&spi->dev.kobj, &max1111_attr_group);
	mutex_destroy(&data->drvdata_lock);
	return 0;
}

static const struct spi_device_id max1111_ids[] = {
	{ "max1110", max1110 },
	{ "max1111", max1111 },
	{ "max1112", max1112 },
	{ "max1113", max1113 },
	{ },
};
MODULE_DEVICE_TABLE(spi, max1111_ids);

>>>>>>> refs/remotes/origin/master
static struct spi_driver max1111_driver = {
	.driver		= {
		.name	= "max1111",
		.owner	= THIS_MODULE,
	},
<<<<<<< HEAD
	.probe		= max1111_probe,
	.remove		= __devexit_p(max1111_remove),
};

<<<<<<< HEAD
static int __init max1111_init(void)
{
	return spi_register_driver(&max1111_driver);
}
module_init(max1111_init);

static void __exit max1111_exit(void)
{
	spi_unregister_driver(&max1111_driver);
}
module_exit(max1111_exit);
=======
module_spi_driver(max1111_driver);
>>>>>>> refs/remotes/origin/cm-10.0

MODULE_AUTHOR("Eric Miao <eric.miao@marvell.com>");
MODULE_DESCRIPTION("MAX1111 ADC Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("spi:max1111");
=======
	.id_table	= max1111_ids,
	.probe		= max1111_probe,
	.remove		= max1111_remove,
};

module_spi_driver(max1111_driver);

MODULE_AUTHOR("Eric Miao <eric.miao@marvell.com>");
MODULE_DESCRIPTION("MAX1110/MAX1111/MAX1112/MAX1113 ADC Driver");
MODULE_LICENSE("GPL");
>>>>>>> refs/remotes/origin/master
