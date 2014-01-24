/*
 * AD7314 digital temperature sensor driver for AD7314, ADT7301 and ADT7302
 *
 * Copyright 2010 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 *
 * Conversion to hwmon from IIO done by Jonathan Cameron <jic23@cam.ac.uk>
 */
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
#include <linux/spi/spi.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>

/*
<<<<<<< HEAD
 * AD7314 power mode
 */
#define AD7314_PD		0x2000

/*
 * AD7314 temperature masks
 */
#define AD7314_TEMP_SIGN		0x200
#define AD7314_TEMP_MASK		0x7FE0
#define AD7314_TEMP_OFFSET		5
=======
 * AD7314 temperature masks
 */
#define AD7314_TEMP_MASK		0x7FE0
#define AD7314_TEMP_SHIFT		5
>>>>>>> refs/remotes/origin/master

/*
 * ADT7301 and ADT7302 temperature masks
 */
<<<<<<< HEAD
#define ADT7301_TEMP_SIGN		0x2000
=======
>>>>>>> refs/remotes/origin/master
#define ADT7301_TEMP_MASK		0x3FFF

enum ad7314_variant {
	adt7301,
	adt7302,
	ad7314,
};

struct ad7314_data {
	struct spi_device	*spi_dev;
	struct device		*hwmon_dev;
	u16 rx ____cacheline_aligned;
};

static int ad7314_spi_read(struct ad7314_data *chip)
{
	int ret;

	ret = spi_read(chip->spi_dev, (u8 *)&chip->rx, sizeof(chip->rx));
	if (ret < 0) {
		dev_err(&chip->spi_dev->dev, "SPI read error\n");
		return ret;
	}

	return be16_to_cpu(chip->rx);
}

static ssize_t ad7314_show_temperature(struct device *dev,
		struct device_attribute *attr,
		char *buf)
{
	struct ad7314_data *chip = dev_get_drvdata(dev);
	s16 data;
	int ret;

	ret = ad7314_spi_read(chip);
	if (ret < 0)
		return ret;
	switch (spi_get_device_id(chip->spi_dev)->driver_data) {
	case ad7314:
<<<<<<< HEAD
		data = (ret & AD7314_TEMP_MASK) >> AD7314_TEMP_OFFSET;
=======
		data = (ret & AD7314_TEMP_MASK) >> AD7314_TEMP_SHIFT;
>>>>>>> refs/remotes/origin/master
		data = (data << 6) >> 6;

		return sprintf(buf, "%d\n", 250 * data);
	case adt7301:
	case adt7302:
		/*
		 * Documented as a 13 bit twos complement register
		 * with a sign bit - which is a 14 bit 2's complement
		 * register.  1lsb - 31.25 milli degrees centigrade
		 */
		data = ret & ADT7301_TEMP_MASK;
		data = (data << 2) >> 2;

		return sprintf(buf, "%d\n",
			       DIV_ROUND_CLOSEST(data * 3125, 100));
	default:
		return -EINVAL;
	}
}

static ssize_t ad7314_show_name(struct device *dev,
				struct device_attribute *devattr, char *buf)
{
	return sprintf(buf, "%s\n", to_spi_device(dev)->modalias);
}

static DEVICE_ATTR(name, S_IRUGO, ad7314_show_name, NULL);
static SENSOR_DEVICE_ATTR(temp1_input, S_IRUGO,
			  ad7314_show_temperature, NULL, 0);

static struct attribute *ad7314_attributes[] = {
	&dev_attr_name.attr,
	&sensor_dev_attr_temp1_input.dev_attr.attr,
	NULL,
};

static const struct attribute_group ad7314_group = {
	.attrs = ad7314_attributes,
};

<<<<<<< HEAD
static int __devinit ad7314_probe(struct spi_device *spi_dev)
=======
static int ad7314_probe(struct spi_device *spi_dev)
>>>>>>> refs/remotes/origin/master
{
	int ret;
	struct ad7314_data *chip;

<<<<<<< HEAD
	chip = kzalloc(sizeof(*chip), GFP_KERNEL);
	if (chip == NULL) {
		ret = -ENOMEM;
		goto error_ret;
	}
	dev_set_drvdata(&spi_dev->dev, chip);

	ret = sysfs_create_group(&spi_dev->dev.kobj, &ad7314_group);
	if (ret < 0)
		goto error_free_chip;
=======
	chip = devm_kzalloc(&spi_dev->dev, sizeof(*chip), GFP_KERNEL);
	if (chip == NULL)
		return -ENOMEM;

	spi_set_drvdata(spi_dev, chip);

	ret = sysfs_create_group(&spi_dev->dev.kobj, &ad7314_group);
	if (ret < 0)
		return ret;

>>>>>>> refs/remotes/origin/master
	chip->hwmon_dev = hwmon_device_register(&spi_dev->dev);
	if (IS_ERR(chip->hwmon_dev)) {
		ret = PTR_ERR(chip->hwmon_dev);
		goto error_remove_group;
	}
	chip->spi_dev = spi_dev;

	return 0;
error_remove_group:
	sysfs_remove_group(&spi_dev->dev.kobj, &ad7314_group);
<<<<<<< HEAD
error_free_chip:
	kfree(chip);
error_ret:
	return ret;
}

static int __devexit ad7314_remove(struct spi_device *spi_dev)
{
	struct ad7314_data *chip = dev_get_drvdata(&spi_dev->dev);

	hwmon_device_unregister(chip->hwmon_dev);
	sysfs_remove_group(&spi_dev->dev.kobj, &ad7314_group);
	kfree(chip);
=======
	return ret;
}

static int ad7314_remove(struct spi_device *spi_dev)
{
	struct ad7314_data *chip = spi_get_drvdata(spi_dev);

	hwmon_device_unregister(chip->hwmon_dev);
	sysfs_remove_group(&spi_dev->dev.kobj, &ad7314_group);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static const struct spi_device_id ad7314_id[] = {
	{ "adt7301", adt7301 },
	{ "adt7302", adt7302 },
	{ "ad7314", ad7314 },
	{ }
};
MODULE_DEVICE_TABLE(spi, ad7314_id);

static struct spi_driver ad7314_driver = {
	.driver = {
		.name = "ad7314",
		.owner = THIS_MODULE,
	},
	.probe = ad7314_probe,
<<<<<<< HEAD
	.remove = __devexit_p(ad7314_remove),
=======
	.remove = ad7314_remove,
>>>>>>> refs/remotes/origin/master
	.id_table = ad7314_id,
};

module_spi_driver(ad7314_driver);

MODULE_AUTHOR("Sonic Zhang <sonic.zhang@analog.com>");
<<<<<<< HEAD
MODULE_DESCRIPTION("Analog Devices AD7314, ADT7301 and ADT7302 digital"
			" temperature sensor driver");
=======
MODULE_DESCRIPTION("Analog Devices AD7314, ADT7301 and ADT7302 digital temperature sensor driver");
>>>>>>> refs/remotes/origin/master
MODULE_LICENSE("GPL v2");
