/*
<<<<<<< HEAD
<<<<<<< HEAD
 * AD7291 digital temperature sensor driver supporting AD7291
 *
 * Copyright 2010 Analog Devices Inc.
=======
 * AD7291 8-Channel, I2C, 12-Bit SAR ADC with Temperature Sensor
 *
 * Copyright 2010-2011 Analog Devices Inc.
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * AD7291 8-Channel, I2C, 12-Bit SAR ADC with Temperature Sensor
 *
 * Copyright 2010-2011 Analog Devices Inc.
>>>>>>> refs/remotes/origin/master
 *
 * Licensed under the GPL-2 or later.
 */

#include <linux/interrupt.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/gpio.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/list.h>
#include <linux/i2c.h>

#include "../iio.h"
#include "../sysfs.h"
=======
=======
>>>>>>> refs/remotes/origin/master
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/regulator/consumer.h>
#include <linux/err.h>

<<<<<<< HEAD
#include "../iio.h"
#include "../sysfs.h"
#include "../events.h"
=======
#include <linux/iio/iio.h>
#include <linux/iio/sysfs.h>
#include <linux/iio/events.h>

#include "ad7291.h"
>>>>>>> refs/remotes/origin/master

/*
 * Simplified handling
 *
 * If no events enabled - single polled channel read
 * If event enabled direct reads disable unless channel
 * is in the read mask.
 *
 * The noise-delayed bit as per datasheet suggestion is always enabled.
 *
 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

/*
 * AD7291 registers definition
 */
<<<<<<< HEAD
<<<<<<< HEAD
#define AD7291_COMMAND			0
#define AD7291_VOLTAGE			1
#define AD7291_T_SENSE			2
#define AD7291_T_AVERAGE		3
#define AD7291_VOLTAGE_LIMIT_BASE	4
#define AD7291_VOLTAGE_LIMIT_COUNT	8
#define AD7291_T_SENSE_HIGH		0x1c
#define AD7291_T_SENSE_LOW		0x1d
#define AD7291_T_SENSE_HYST		0x1e
#define AD7291_VOLTAGE_ALERT_STATUS	0x1f
#define AD7291_T_ALERT_STATUS		0x20

/*
 * AD7291 command
 */
#define AD7291_AUTOCYCLE		0x1
#define AD7291_RESET			0x2
#define AD7291_ALART_CLEAR		0x4
#define AD7291_ALART_POLARITY		0x8
#define AD7291_EXT_REF			0x10
#define AD7291_NOISE_DELAY		0x20
#define AD7291_T_SENSE_MASK		0x40
#define AD7291_VOLTAGE_MASK		0xff00
=======
=======
>>>>>>> refs/remotes/origin/master
#define AD7291_COMMAND			0x00
#define AD7291_VOLTAGE			0x01
#define AD7291_T_SENSE			0x02
#define AD7291_T_AVERAGE		0x03
<<<<<<< HEAD
#define AD7291_CH0_DATA_HIGH		0x04
#define AD7291_CH0_DATA_LOW		0x05
#define AD7291_CH0_HYST			0x06
#define AD7291_CH1_DATA_HIGH		0x07
#define AD7291_CH1_DATA_LOW		0x08
#define AD7291_CH1_HYST			0x09
#define AD7291_CH2_DATA_HIGH		0x0A
#define AD7291_CH2_DATA_LOW		0x0B
#define AD7291_CH2_HYST			0x0C
#define AD7291_CH3_DATA_HIGH		0x0D
#define AD7291_CH3_DATA_LOW		0x0E
#define AD7291_CH3_HYST			0x0F
#define AD7291_CH4_DATA_HIGH		0x10
#define AD7291_CH4_DATA_LOW		0x11
#define AD7291_CH4_HYST			0x12
#define AD7291_CH5_DATA_HIGH		0x13
#define AD7291_CH5_DATA_LOW		0x14
#define AD7291_CH5_HYST			0x15
#define AD7291_CH6_DATA_HIGH		0x16
#define AD7291_CH6_DATA_LOW		0x17
#define AD7291_CH6_HYST			0x18
#define AD7291_CH7_DATA_HIGH		0x19
#define AD7291_CH7_DATA_LOW		0x1A
#define AD7291_CH7_HYST			0x2B
#define AD7291_T_SENSE_HIGH		0x1C
#define AD7291_T_SENSE_LOW		0x1D
#define AD7291_T_SENSE_HYST		0x1E
=======
#define AD7291_DATA_HIGH(x)		((x) * 3 + 0x4)
#define AD7291_DATA_LOW(x)		((x) * 3 + 0x5)
#define AD7291_HYST(x)			((x) * 3 + 0x6)
>>>>>>> refs/remotes/origin/master
#define AD7291_VOLTAGE_ALERT_STATUS	0x1F
#define AD7291_T_ALERT_STATUS		0x20

#define AD7291_VOLTAGE_LIMIT_COUNT	8


/*
 * AD7291 command
 */
#define AD7291_AUTOCYCLE		(1 << 0)
#define AD7291_RESET			(1 << 1)
#define AD7291_ALERT_CLEAR		(1 << 2)
#define AD7291_ALERT_POLARITY		(1 << 3)
#define AD7291_EXT_REF			(1 << 4)
#define AD7291_NOISE_DELAY		(1 << 5)
#define AD7291_T_SENSE_MASK		(1 << 7)
#define AD7291_VOLTAGE_MASK		0xFF00
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#define AD7291_VOLTAGE_OFFSET		0x8

/*
 * AD7291 value masks
 */
<<<<<<< HEAD
<<<<<<< HEAD
#define AD7291_CHANNEL_MASK		0xf000
#define AD7291_VALUE_MASK		0xfff
=======
#define AD7291_CHANNEL_MASK		0xF000
#define AD7291_BITS			12
#define AD7291_VALUE_MASK		0xFFF
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define AD7291_CHANNEL_MASK		0xF000
#define AD7291_BITS			12
#define AD7291_VALUE_MASK		0xFFF
>>>>>>> refs/remotes/origin/master
#define AD7291_T_VALUE_SIGN		0x400
#define AD7291_T_VALUE_FLOAT_OFFSET	2
#define AD7291_T_VALUE_FLOAT_MASK	0x2

<<<<<<< HEAD
<<<<<<< HEAD
/*
 * struct ad7291_chip_info - chip specifc information
 */

struct ad7291_chip_info {
	struct i2c_client *client;
	struct iio_dev *indio_dev;
	u16 command;
	u8  channels;	/* Active voltage channels */
};

/*
 * struct ad7291_chip_info - chip specifc information
 */

struct ad7291_limit_regs {
	u16	data_high;
	u16	data_low;
	u16	hysteresis;
};

/*
 * ad7291 register access by I2C
 */
=======
=======
>>>>>>> refs/remotes/origin/master
#define AD7291_BITS			12

struct ad7291_chip_info {
	struct i2c_client	*client;
	struct regulator	*reg;
<<<<<<< HEAD
	u16			int_vref_mv;
=======
>>>>>>> refs/remotes/origin/master
	u16			command;
	u16			c_mask;	/* Active voltage channels for events */
	struct mutex		state_lock;
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int ad7291_i2c_read(struct ad7291_chip_info *chip, u8 reg, u16 *data)
{
	struct i2c_client *client = chip->client;
	int ret = 0;

<<<<<<< HEAD
	ret = i2c_smbus_read_word_data(client, reg);
=======
	ret = i2c_smbus_read_word_swapped(client, reg);
>>>>>>> refs/remotes/origin/master
	if (ret < 0) {
		dev_err(&client->dev, "I2C read error\n");
		return ret;
	}

<<<<<<< HEAD
	*data = swab16((u16)ret);
=======
	*data = ret;
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int ad7291_i2c_write(struct ad7291_chip_info *chip, u8 reg, u16 data)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct i2c_client *client = chip->client;
	int ret = 0;

	ret = i2c_smbus_write_word_data(client, reg, swab16(data));
	if (ret < 0)
		dev_err(&client->dev, "I2C write error\n");

	return ret;
}

/* Returns negative errno, or else the number of words read. */
static int ad7291_i2c_read_data(struct ad7291_chip_info *chip, u8 reg, u16 *data)
{
	struct i2c_client *client = chip->client;
	u8 commands[4];
	int ret = 0;
	int i, count;

	if (reg == AD7291_T_SENSE || reg == AD7291_T_AVERAGE)
		count = 2;
	else if (reg == AD7291_VOLTAGE) {
		if (!chip->channels) {
			dev_err(&client->dev, "No voltage channel is selected.\n");
			return -EINVAL;
		}
		count = 2 + chip->channels * 2;
	} else {
		dev_err(&client->dev, "I2C wrong data register\n");
		return -EINVAL;
	}

	commands[0] = 0;
	commands[1] = (chip->command >> 8) & 0xff;
	commands[2] = chip->command & 0xff;
	commands[3] = reg;

	ret = i2c_master_send(client, commands, 4);
	if (ret < 0) {
		dev_err(&client->dev, "I2C master send error\n");
		return ret;
	}

	ret = i2c_master_recv(client, (u8 *)data, count);
	if (ret < 0) {
		dev_err(&client->dev, "I2C master receive error\n");
		return ret;
	}
	ret >>= 2;

	for (i = 0; i < ret; i++)
		data[i] = swab16(data[i]);

	return ret;
}

static ssize_t ad7291_show_mode(struct device *dev,
		struct device_attribute *attr,
		char *buf)
{
	struct iio_dev *dev_info = dev_get_drvdata(dev);
	struct ad7291_chip_info *chip = dev_info->dev_data;

	if (chip->command & AD7291_AUTOCYCLE)
		return sprintf(buf, "autocycle\n");
	else
		return sprintf(buf, "command\n");
}

static ssize_t ad7291_store_mode(struct device *dev,
		struct device_attribute *attr,
		const char *buf,
		size_t len)
{
	struct iio_dev *dev_info = dev_get_drvdata(dev);
	struct ad7291_chip_info *chip = dev_info->dev_data;
	u16 command;
	int ret;

	command = chip->command & (~AD7291_AUTOCYCLE);
	if (strcmp(buf, "autocycle"))
		command |= AD7291_AUTOCYCLE;

	ret = ad7291_i2c_write(chip, AD7291_COMMAND, command);
	if (ret)
		return -EIO;

	chip->command = command;

	return ret;
}

static IIO_DEVICE_ATTR(mode, S_IRUGO | S_IWUSR,
		ad7291_show_mode,
		ad7291_store_mode,
		0);

static ssize_t ad7291_show_available_modes(struct device *dev,
		struct device_attribute *attr,
		char *buf)
{
	return sprintf(buf, "command\nautocycle\n");
}

static IIO_DEVICE_ATTR(available_modes, S_IRUGO, ad7291_show_available_modes, NULL, 0);

=======
	return i2c_smbus_write_word_data(chip->client, reg, swab16(data));
}

>>>>>>> refs/remotes/origin/cm-10.0
static ssize_t ad7291_store_reset(struct device *dev,
		struct device_attribute *attr,
		const char *buf,
		size_t len)
{
<<<<<<< HEAD
	struct iio_dev *dev_info = dev_get_drvdata(dev);
	struct ad7291_chip_info *chip = dev_info->dev_data;
	u16 command;
	int ret;

	command = chip->command | AD7291_RESET;

	ret = ad7291_i2c_write(chip, AD7291_COMMAND, command);
	if (ret)
		return -EIO;

	return ret;
}

static IIO_DEVICE_ATTR(reset, S_IWUSR,
		NULL,
		ad7291_store_reset,
		0);

static ssize_t ad7291_show_ext_ref(struct device *dev,
		struct device_attribute *attr,
		char *buf)
{
	struct iio_dev *dev_info = dev_get_drvdata(dev);
	struct ad7291_chip_info *chip = dev_info->dev_data;

	return sprintf(buf, "%d\n", !!(chip->command & AD7291_EXT_REF));
}

static ssize_t ad7291_store_ext_ref(struct device *dev,
		struct device_attribute *attr,
		const char *buf,
		size_t len)
{
	struct iio_dev *dev_info = dev_get_drvdata(dev);
	struct ad7291_chip_info *chip = dev_info->dev_data;
	u16 command;
	int ret;

	command = chip->command & (~AD7291_EXT_REF);
	if (strcmp(buf, "1"))
		command |= AD7291_EXT_REF;

	ret = ad7291_i2c_write(chip, AD7291_COMMAND, command);
	if (ret)
		return -EIO;

	chip->command = command;

	return ret;
}

static IIO_DEVICE_ATTR(ext_ref, S_IRUGO | S_IWUSR,
		ad7291_show_ext_ref,
		ad7291_store_ext_ref,
		0);

static ssize_t ad7291_show_noise_delay(struct device *dev,
		struct device_attribute *attr,
		char *buf)
{
	struct iio_dev *dev_info = dev_get_drvdata(dev);
	struct ad7291_chip_info *chip = dev_info->dev_data;

	return sprintf(buf, "%d\n", !!(chip->command & AD7291_NOISE_DELAY));
}

static ssize_t ad7291_store_noise_delay(struct device *dev,
		struct device_attribute *attr,
		const char *buf,
		size_t len)
{
	struct iio_dev *dev_info = dev_get_drvdata(dev);
	struct ad7291_chip_info *chip = dev_info->dev_data;
	u16 command;
	int ret;

	command = chip->command & (~AD7291_NOISE_DELAY);
	if (strcmp(buf, "1"))
		command |= AD7291_NOISE_DELAY;

	ret = ad7291_i2c_write(chip, AD7291_COMMAND, command);
	if (ret)
		return -EIO;

	chip->command = command;

	return ret;
}

static IIO_DEVICE_ATTR(noise_delay, S_IRUGO | S_IWUSR,
		ad7291_show_noise_delay,
		ad7291_store_noise_delay,
		0);

static ssize_t ad7291_show_t_sense(struct device *dev,
		struct device_attribute *attr,
		char *buf)
{
	struct iio_dev *dev_info = dev_get_drvdata(dev);
	struct ad7291_chip_info *chip = dev_info->dev_data;
	u16 data;
	char sign = ' ';
	int ret;

	ret = ad7291_i2c_read_data(chip, AD7291_T_SENSE, &data);
	if (ret)
		return -EIO;

	if (data & AD7291_T_VALUE_SIGN) {
		/* convert supplement to positive value */
		data = (AD7291_T_VALUE_SIGN << 1) - data;
		sign = '-';
	}

	return sprintf(buf, "%c%d.%.2d\n", sign,
		(data >> AD7291_T_VALUE_FLOAT_OFFSET),
		(data & AD7291_T_VALUE_FLOAT_MASK) * 25);
}

static IIO_DEVICE_ATTR(t_sense, S_IRUGO, ad7291_show_t_sense, NULL, 0);

static ssize_t ad7291_show_t_average(struct device *dev,
		struct device_attribute *attr,
		char *buf)
{
	struct iio_dev *dev_info = dev_get_drvdata(dev);
	struct ad7291_chip_info *chip = dev_info->dev_data;
	u16 data;
	char sign = ' ';
	int ret;

	ret = ad7291_i2c_read_data(chip, AD7291_T_AVERAGE, &data);
	if (ret)
		return -EIO;

	if (data & AD7291_T_VALUE_SIGN) {
		/* convert supplement to positive value */
		data = (AD7291_T_VALUE_SIGN << 1) - data;
		sign = '-';
	}

	return sprintf(buf, "%c%d.%.2d\n", sign,
		(data >> AD7291_T_VALUE_FLOAT_OFFSET),
		(data & AD7291_T_VALUE_FLOAT_MASK) * 25);
}

static IIO_DEVICE_ATTR(t_average, S_IRUGO, ad7291_show_t_average, NULL, 0);

static ssize_t ad7291_show_voltage(struct device *dev,
		struct device_attribute *attr,
		char *buf)
{
	struct iio_dev *dev_info = dev_get_drvdata(dev);
	struct ad7291_chip_info *chip = dev_info->dev_data;
	u16 data[AD7291_VOLTAGE_LIMIT_COUNT];
	int i, size, ret;

	ret = ad7291_i2c_read_data(chip, AD7291_VOLTAGE, data);
	if (ret)
		return -EIO;

	for (i = 0; i < AD7291_VOLTAGE_LIMIT_COUNT; i++) {
		if (chip->command & (AD7291_T_SENSE_MASK << i)) {
			ret = sprintf(buf, "channel[%d]=%d\n", i,
					data[i] & AD7291_VALUE_MASK);
			if (ret < 0)
				break;
			buf += ret;
			size += ret;
		}
	}

	return size;
}

static IIO_DEVICE_ATTR(voltage, S_IRUGO, ad7291_show_voltage, NULL, 0);

static ssize_t ad7291_show_channel_mask(struct device *dev,
		struct device_attribute *attr,
		char *buf)
{
	struct iio_dev *dev_info = dev_get_drvdata(dev);
	struct ad7291_chip_info *chip = dev_info->dev_data;

	return sprintf(buf, "0x%x\n", (chip->command & AD7291_VOLTAGE_MASK) >>
			AD7291_VOLTAGE_OFFSET);
}

static ssize_t ad7291_store_channel_mask(struct device *dev,
		struct device_attribute *attr,
		const char *buf,
		size_t len)
{
	struct iio_dev *dev_info = dev_get_drvdata(dev);
	struct ad7291_chip_info *chip = dev_info->dev_data;
	u16 command;
	unsigned long data;
	int i, ret;

	ret = strict_strtoul(buf, 16, &data);
	if (ret || data > 0xff)
		return -EINVAL;

	command = chip->command & (~AD7291_VOLTAGE_MASK);
	command |= data << AD7291_VOLTAGE_OFFSET;

	ret = ad7291_i2c_write(chip, AD7291_COMMAND, command);
	if (ret)
		return -EIO;

	chip->command = command;

	for (i = 0, chip->channels = 0; i < AD7291_VOLTAGE_LIMIT_COUNT; i++) {
		if (chip->command & (AD7291_T_SENSE_MASK << i))
			chip->channels++;
	}

	return ret;
}

static IIO_DEVICE_ATTR(channel_mask, S_IRUGO | S_IWUSR,
		ad7291_show_channel_mask,
		ad7291_store_channel_mask,
		0);

static struct attribute *ad7291_attributes[] = {
	&iio_dev_attr_available_modes.dev_attr.attr,
	&iio_dev_attr_mode.dev_attr.attr,
	&iio_dev_attr_reset.dev_attr.attr,
	&iio_dev_attr_ext_ref.dev_attr.attr,
	&iio_dev_attr_noise_delay.dev_attr.attr,
	&iio_dev_attr_t_sense.dev_attr.attr,
	&iio_dev_attr_t_average.dev_attr.attr,
	&iio_dev_attr_voltage.dev_attr.attr,
	&iio_dev_attr_channel_mask.dev_attr.attr,
=======
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct ad7291_chip_info *chip = iio_priv(indio_dev);

	return ad7291_i2c_write(chip, AD7291_COMMAND,
				chip->command | AD7291_RESET);
}

static IIO_DEVICE_ATTR(reset, S_IWUSR, NULL, ad7291_store_reset, 0);

static struct attribute *ad7291_attributes[] = {
	&iio_dev_attr_reset.dev_attr.attr,
>>>>>>> refs/remotes/origin/cm-10.0
	NULL,
};

static const struct attribute_group ad7291_attribute_group = {
	.attrs = ad7291_attributes,
};

<<<<<<< HEAD
/*
 * temperature bound events
 */

static irqreturn_t ad7291_event_handler(int irq, void *private)
{
	struct iio_dev *indio_dev = private;
	struct ad7291_chip_info *chip = iio_dev_get_devdata(private);
=======
=======
	return i2c_smbus_write_word_swapped(chip->client, reg, data);
}

>>>>>>> refs/remotes/origin/master
static irqreturn_t ad7291_event_handler(int irq, void *private)
{
	struct iio_dev *indio_dev = private;
	struct ad7291_chip_info *chip = iio_priv(private);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	u16 t_status, v_status;
	u16 command;
	int i;
	s64 timestamp = iio_get_time_ns();

	if (ad7291_i2c_read(chip, AD7291_T_ALERT_STATUS, &t_status))
		return IRQ_HANDLED;

	if (ad7291_i2c_read(chip, AD7291_VOLTAGE_ALERT_STATUS, &v_status))
		return IRQ_HANDLED;

	if (!(t_status || v_status))
		return IRQ_HANDLED;

<<<<<<< HEAD
<<<<<<< HEAD
	command = chip->command | AD7291_ALART_CLEAR;
	ad7291_i2c_write(chip, AD7291_COMMAND, command);

	command = chip->command & ~AD7291_ALART_CLEAR;
	ad7291_i2c_write(chip, AD7291_COMMAND, command);

	if (t_status & (1 << 0))
		iio_push_event(indio_dev, 0,
			       IIO_UNMOD_EVENT_CODE(IIO_TEMP,
						    0,
						    IIO_EV_TYPE_THRESH,
						    IIO_EV_DIR_FALLING),
			       timestamp);
	if (t_status & (1 << 1))
		iio_push_event(indio_dev, 0,
			       IIO_UNMOD_EVENT_CODE(IIO_TEMP,
						    0,
						    IIO_EV_TYPE_THRESH,
						    IIO_EV_DIR_RISING),
			       timestamp);
	if (t_status & (1 << 2))
		iio_push_event(indio_dev, 0,
=======
=======
>>>>>>> refs/remotes/origin/master
	command = chip->command | AD7291_ALERT_CLEAR;
	ad7291_i2c_write(chip, AD7291_COMMAND, command);

	command = chip->command & ~AD7291_ALERT_CLEAR;
	ad7291_i2c_write(chip, AD7291_COMMAND, command);

	/* For now treat t_sense and t_sense_average the same */
	if ((t_status & (1 << 0)) || (t_status & (1 << 2)))
		iio_push_event(indio_dev,
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
			       IIO_UNMOD_EVENT_CODE(IIO_TEMP,
						    0,
						    IIO_EV_TYPE_THRESH,
						    IIO_EV_DIR_FALLING),
			       timestamp);
<<<<<<< HEAD
<<<<<<< HEAD
	if (t_status & (1 << 3))
		iio_push_event(indio_dev, 0,
=======
	if ((t_status & (1 << 1)) || (t_status & (1 << 3)))
		iio_push_event(indio_dev,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if ((t_status & (1 << 1)) || (t_status & (1 << 3)))
		iio_push_event(indio_dev,
>>>>>>> refs/remotes/origin/master
			       IIO_UNMOD_EVENT_CODE(IIO_TEMP,
						    0,
						    IIO_EV_TYPE_THRESH,
						    IIO_EV_DIR_RISING),
			       timestamp);

	for (i = 0; i < AD7291_VOLTAGE_LIMIT_COUNT*2; i += 2) {
		if (v_status & (1 << i))
<<<<<<< HEAD
<<<<<<< HEAD
			iio_push_event(indio_dev, 0,
				       IIO_UNMOD_EVENT_CODE(IIO_IN,
=======
			iio_push_event(indio_dev,
				       IIO_UNMOD_EVENT_CODE(IIO_VOLTAGE,
>>>>>>> refs/remotes/origin/cm-10.0
=======
			iio_push_event(indio_dev,
				       IIO_UNMOD_EVENT_CODE(IIO_VOLTAGE,
>>>>>>> refs/remotes/origin/master
							    i/2,
							    IIO_EV_TYPE_THRESH,
							    IIO_EV_DIR_FALLING),
				       timestamp);
		if (v_status & (1 << (i + 1)))
<<<<<<< HEAD
<<<<<<< HEAD
			iio_push_event(indio_dev, 0,
				       IIO_UNMOD_EVENT_CODE(IIO_IN,
=======
			iio_push_event(indio_dev,
				       IIO_UNMOD_EVENT_CODE(IIO_VOLTAGE,
>>>>>>> refs/remotes/origin/cm-10.0
=======
			iio_push_event(indio_dev,
				       IIO_UNMOD_EVENT_CODE(IIO_VOLTAGE,
>>>>>>> refs/remotes/origin/master
							    i/2,
							    IIO_EV_TYPE_THRESH,
							    IIO_EV_DIR_RISING),
				       timestamp);
	}

	return IRQ_HANDLED;
}

<<<<<<< HEAD
<<<<<<< HEAD
static inline ssize_t ad7291_show_t_bound(struct device *dev,
		struct device_attribute *attr,
		char *buf)
{
	struct iio_dev *dev_info = dev_get_drvdata(dev);
	struct ad7291_chip_info *chip = dev_info->dev_data;
	struct iio_dev_attr *this_attr = to_iio_dev_attr(attr);
	u16 data;
	char sign = ' ';
	int ret;

	ret = ad7291_i2c_read(chip, this_attr->address, &data);
	if (ret)
		return -EIO;

	data &= AD7291_VALUE_MASK;
	if (data & AD7291_T_VALUE_SIGN) {
		/* convert supplement to positive value */
		data = (AD7291_T_VALUE_SIGN << 1) - data;
		sign = '-';
	}

	return sprintf(buf, "%c%d.%.2d\n", sign,
			data >> AD7291_T_VALUE_FLOAT_OFFSET,
			(data & AD7291_T_VALUE_FLOAT_MASK) * 25);
}

static inline ssize_t ad7291_set_t_bound(struct device *dev,
		struct device_attribute *attr,
		const char *buf,
		size_t len)
{
	struct iio_dev *dev_info = dev_get_drvdata(dev);
	struct ad7291_chip_info *chip = dev_info->dev_data;
	struct iio_dev_attr *this_attr = to_iio_dev_attr(attr);
	long tmp1, tmp2;
	u16 data;
	char *pos;
	int ret;

	pos = strchr(buf, '.');

	ret = strict_strtol(buf, 10, &tmp1);

	if (ret || tmp1 > 127 || tmp1 < -128)
		return -EINVAL;

	if (pos) {
		len = strlen(pos);
		if (len > AD7291_T_VALUE_FLOAT_OFFSET)
			len = AD7291_T_VALUE_FLOAT_OFFSET;
		pos[len] = 0;
		ret = strict_strtol(pos, 10, &tmp2);

		if (!ret)
			tmp2 = (tmp2 / 25) * 25;
	}

	if (tmp1 < 0)
		data = (u16)(-tmp1);
	else
		data = (u16)tmp1;
	data = (data << AD7291_T_VALUE_FLOAT_OFFSET) |
		(tmp2 & AD7291_T_VALUE_FLOAT_MASK);
	if (tmp1 < 0)
		/* convert positive value to supplyment */
		data = (AD7291_T_VALUE_SIGN << 1) - data;

	ret = ad7291_i2c_write(chip, this_attr->address, data);
	if (ret)
		return -EIO;

	return ret;
}

static inline ssize_t ad7291_show_v_bound(struct device *dev,
		struct device_attribute *attr,
		u8 bound_reg,
		char *buf)
{
	struct iio_dev *dev_info = dev_get_drvdata(dev);
	struct ad7291_chip_info *chip = dev_info->dev_data;
	u16 data;
	int ret;

	if (bound_reg < AD7291_VOLTAGE_LIMIT_BASE ||
		bound_reg >= AD7291_VOLTAGE_LIMIT_BASE +
		AD7291_VOLTAGE_LIMIT_COUNT)
		return -EINVAL;

	ret = ad7291_i2c_read(chip, bound_reg, &data);
	if (ret)
		return -EIO;

	data &= AD7291_VALUE_MASK;

	return sprintf(buf, "%d\n", data);
}

static inline ssize_t ad7291_set_v_bound(struct device *dev,
		struct device_attribute *attr,
		u8 bound_reg,
		const char *buf,
		size_t len)
{
	struct iio_dev *dev_info = dev_get_drvdata(dev);
	struct ad7291_chip_info *chip = dev_info->dev_data;
	unsigned long value;
	u16 data;
	int ret;

	if (bound_reg < AD7291_VOLTAGE_LIMIT_BASE ||
		bound_reg >= AD7291_VOLTAGE_LIMIT_BASE +
		AD7291_VOLTAGE_LIMIT_COUNT)
		return -EINVAL;

	ret = strict_strtoul(buf, 10, &value);

	if (ret || value >= 4096)
		return -EINVAL;

	data = (u16)value;
	ret = ad7291_i2c_write(chip, bound_reg, data);
	if (ret)
		return -EIO;

	return ret;
}

static IIO_DEVICE_ATTR(t_sense_high_value,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound,
		       AD7291_T_SENSE_HIGH);
static IIO_DEVICE_ATTR(t_sense_low_value,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound,
		       AD7291_T_SENSE_LOW);
static IIO_DEVICE_ATTR(t_sense_hyst_value,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound,
		       AD7291_T_SENSE_HYST);
static IIO_DEVICE_ATTR(v0_high,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound, 0x04);
static IIO_DEVICE_ATTR(v0_low,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound, 0x05);
static IIO_DEVICE_ATTR(v0_hyst,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound, 0x06);
static IIO_DEVICE_ATTR(v1_high,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound, 0x07);
static IIO_DEVICE_ATTR(v1_low,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound, 0x08);
static IIO_DEVICE_ATTR(v1_hyst,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound, 0x09);
static IIO_DEVICE_ATTR(v2_high,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound, 0x0A);
static IIO_DEVICE_ATTR(v2_low,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound, 0x0B);
static IIO_DEVICE_ATTR(v2_hyst,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound, 0x0C);
static IIO_DEVICE_ATTR(v3_high,
		       S_IRUGO | S_IWUSR,
		       /* Datasheet suggests this one and this one only
			  has the registers in different order */
		       ad7291_show_t_bound, ad7291_set_t_bound, 0x0E);
static IIO_DEVICE_ATTR(v3_low,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound, 0x0D);
static IIO_DEVICE_ATTR(v3_hyst,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound, 0x0F);
static IIO_DEVICE_ATTR(v4_high,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound, 0x10);
static IIO_DEVICE_ATTR(v4_low,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound, 0x11);
static IIO_DEVICE_ATTR(v4_hyst,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound, 0x12);
static IIO_DEVICE_ATTR(v5_high,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound, 0x13);
static IIO_DEVICE_ATTR(v5_low,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound, 0x14);
static IIO_DEVICE_ATTR(v5_hyst,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound, 0x15);
static IIO_DEVICE_ATTR(v6_high,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound, 0x16);
static IIO_DEVICE_ATTR(v6_low,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound, 0x17);
static IIO_DEVICE_ATTR(v6_hyst,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound, 0x18);
static IIO_DEVICE_ATTR(v7_high,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound, 0x19);
static IIO_DEVICE_ATTR(v7_low,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound, 0x1A);
static IIO_DEVICE_ATTR(v7_hyst,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_t_bound, ad7291_set_t_bound, 0x1B);

static struct attribute *ad7291_event_attributes[] = {
	&iio_dev_attr_t_sense_high_value.dev_attr.attr,
	&iio_dev_attr_t_sense_low_value.dev_attr.attr,
	&iio_dev_attr_t_sense_hyst_value.dev_attr.attr,
	&iio_dev_attr_v0_high.dev_attr.attr,
	&iio_dev_attr_v0_low.dev_attr.attr,
	&iio_dev_attr_v0_hyst.dev_attr.attr,
	&iio_dev_attr_v1_high.dev_attr.attr,
	&iio_dev_attr_v1_low.dev_attr.attr,
	&iio_dev_attr_v1_hyst.dev_attr.attr,
	&iio_dev_attr_v2_high.dev_attr.attr,
	&iio_dev_attr_v2_low.dev_attr.attr,
	&iio_dev_attr_v2_hyst.dev_attr.attr,
	&iio_dev_attr_v3_high.dev_attr.attr,
	&iio_dev_attr_v3_low.dev_attr.attr,
	&iio_dev_attr_v3_hyst.dev_attr.attr,
	&iio_dev_attr_v4_high.dev_attr.attr,
	&iio_dev_attr_v4_low.dev_attr.attr,
	&iio_dev_attr_v4_hyst.dev_attr.attr,
	&iio_dev_attr_v5_high.dev_attr.attr,
	&iio_dev_attr_v5_low.dev_attr.attr,
	&iio_dev_attr_v5_hyst.dev_attr.attr,
	&iio_dev_attr_v6_high.dev_attr.attr,
	&iio_dev_attr_v6_low.dev_attr.attr,
	&iio_dev_attr_v6_hyst.dev_attr.attr,
	&iio_dev_attr_v7_high.dev_attr.attr,
	&iio_dev_attr_v7_low.dev_attr.attr,
	&iio_dev_attr_v7_hyst.dev_attr.attr,
	NULL,
=======
static inline ssize_t ad7291_show_hyst(struct device *dev,
		struct device_attribute *attr,
		char *buf)
{
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct ad7291_chip_info *chip = iio_priv(indio_dev);
	struct iio_dev_attr *this_attr = to_iio_dev_attr(attr);
	u16 data;
	int ret;

	ret = ad7291_i2c_read(chip, this_attr->address, &data);
	if (ret < 0)
		return ret;

	return sprintf(buf, "%d\n", data & AD7291_VALUE_MASK);
}

static inline ssize_t ad7291_set_hyst(struct device *dev,
				      struct device_attribute *attr,
				      const char *buf,
				      size_t len)
{
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct ad7291_chip_info *chip = iio_priv(indio_dev);
	struct iio_dev_attr *this_attr = to_iio_dev_attr(attr);
	u16 data;
	int ret;

	ret = kstrtou16(buf, 10, &data);

	if (ret < 0)
		return ret;
	if (data > AD7291_VALUE_MASK)
		return -EINVAL;

	ret = ad7291_i2c_write(chip, this_attr->address, data);
	if (ret < 0)
		return ret;

	return len;
}

static IIO_DEVICE_ATTR(in_temp0_thresh_both_hyst_raw,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_hyst, ad7291_set_hyst,
		       AD7291_T_SENSE_HYST);
static IIO_DEVICE_ATTR(in_voltage0_thresh_both_hyst_raw,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_hyst, ad7291_set_hyst, AD7291_CH0_HYST);
static IIO_DEVICE_ATTR(in_voltage1_thresh_both_hyst_raw,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_hyst, ad7291_set_hyst, AD7291_CH1_HYST);
static IIO_DEVICE_ATTR(in_voltage2_thresh_both_hyst_raw,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_hyst, ad7291_set_hyst, AD7291_CH2_HYST);
static IIO_DEVICE_ATTR(in_voltage3_thresh_both_hyst_raw,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_hyst, ad7291_set_hyst, AD7291_CH3_HYST);
static IIO_DEVICE_ATTR(in_voltage4_thresh_both_hyst_raw,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_hyst, ad7291_set_hyst, AD7291_CH4_HYST);
static IIO_DEVICE_ATTR(in_voltage5_thresh_both_hyst_raw,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_hyst, ad7291_set_hyst, AD7291_CH5_HYST);
static IIO_DEVICE_ATTR(in_voltage6_thresh_both_hyst_raw,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_hyst, ad7291_set_hyst, AD7291_CH6_HYST);
static IIO_DEVICE_ATTR(in_voltage7_thresh_both_hyst_raw,
		       S_IRUGO | S_IWUSR,
		       ad7291_show_hyst, ad7291_set_hyst, AD7291_CH7_HYST);

static struct attribute *ad7291_event_attributes[] = {
	&iio_dev_attr_in_temp0_thresh_both_hyst_raw.dev_attr.attr,
	&iio_dev_attr_in_voltage0_thresh_both_hyst_raw.dev_attr.attr,
	&iio_dev_attr_in_voltage1_thresh_both_hyst_raw.dev_attr.attr,
	&iio_dev_attr_in_voltage2_thresh_both_hyst_raw.dev_attr.attr,
	&iio_dev_attr_in_voltage3_thresh_both_hyst_raw.dev_attr.attr,
	&iio_dev_attr_in_voltage4_thresh_both_hyst_raw.dev_attr.attr,
	&iio_dev_attr_in_voltage5_thresh_both_hyst_raw.dev_attr.attr,
	&iio_dev_attr_in_voltage6_thresh_both_hyst_raw.dev_attr.attr,
	&iio_dev_attr_in_voltage7_thresh_both_hyst_raw.dev_attr.attr,
	NULL,
};

/* high / low */
static u8 ad7291_limit_regs[9][2] = {
	{ AD7291_CH0_DATA_HIGH, AD7291_CH0_DATA_LOW },
	{ AD7291_CH1_DATA_HIGH, AD7291_CH1_DATA_LOW },
	{ AD7291_CH2_DATA_HIGH, AD7291_CH2_DATA_LOW },
	{ AD7291_CH3_DATA_HIGH, AD7291_CH3_DATA_LOW }, /* FIXME: ? */
	{ AD7291_CH4_DATA_HIGH, AD7291_CH4_DATA_LOW },
	{ AD7291_CH5_DATA_HIGH, AD7291_CH5_DATA_LOW },
	{ AD7291_CH6_DATA_HIGH, AD7291_CH6_DATA_LOW },
	{ AD7291_CH7_DATA_HIGH, AD7291_CH7_DATA_LOW },
	/* temp */
	{ AD7291_T_SENSE_HIGH, AD7291_T_SENSE_LOW },
};

static int ad7291_read_event_value(struct iio_dev *indio_dev,
				   u64 event_code,
				   int *val)
{
	struct ad7291_chip_info *chip = iio_priv(indio_dev);

	int ret;
	u8 reg;
	u16 uval;
	s16 signval;

	switch (IIO_EVENT_CODE_EXTRACT_CHAN_TYPE(event_code)) {
	case IIO_VOLTAGE:
		reg = ad7291_limit_regs[IIO_EVENT_CODE_EXTRACT_CHAN(event_code)]
			[!(IIO_EVENT_CODE_EXTRACT_DIR(event_code) ==
			   IIO_EV_DIR_RISING)];

		ret = ad7291_i2c_read(chip, reg, &uval);
		if (ret < 0)
			return ret;
		*val = uval & AD7291_VALUE_MASK;
		return 0;

	case IIO_TEMP:
		reg = ad7291_limit_regs[8]
			[!(IIO_EVENT_CODE_EXTRACT_DIR(event_code) ==
			   IIO_EV_DIR_RISING)];

		ret = ad7291_i2c_read(chip, reg, &signval);
		if (ret < 0)
			return ret;
		signval = (s16)((signval & AD7291_VALUE_MASK) << 4) >> 4;
		*val = signval;
		return 0;
	default:
		return -EINVAL;
	};
}

static int ad7291_write_event_value(struct iio_dev *indio_dev,
				    u64 event_code,
				    int val)
{
	struct ad7291_chip_info *chip = iio_priv(indio_dev);
	u8 reg;
	s16 signval;

	switch (IIO_EVENT_CODE_EXTRACT_CHAN_TYPE(event_code)) {
	case IIO_VOLTAGE:
		if (val > AD7291_VALUE_MASK || val < 0)
			return -EINVAL;
		reg = ad7291_limit_regs[IIO_EVENT_CODE_EXTRACT_CHAN(event_code)]
			[!(IIO_EVENT_CODE_EXTRACT_DIR(event_code) ==
			   IIO_EV_DIR_RISING)];
		return ad7291_i2c_write(chip, reg, val);
	case IIO_TEMP:
		if (val > 2047 || val < -2048)
			return -EINVAL;
		reg = ad7291_limit_regs[8]
			[!(IIO_EVENT_CODE_EXTRACT_DIR(event_code) ==
			   IIO_EV_DIR_RISING)];
		signval = val;
		return ad7291_i2c_write(chip, reg, *(u16 *)&signval);
	default:
		return -EINVAL;
	};
}

static int ad7291_read_event_config(struct iio_dev *indio_dev,
				    u64 event_code)
=======
static unsigned int ad7291_threshold_reg(const struct iio_chan_spec *chan,
	enum iio_event_direction dir, enum iio_event_info info)
{
	unsigned int offset;

	switch (chan->type) {
	case IIO_VOLTAGE:
		offset = chan->channel;
		break;
	case IIO_TEMP:
		offset = 8;
		break;
	default:
	    return 0;
	}

	switch (info) {
	case IIO_EV_INFO_VALUE:
			if (dir == IIO_EV_DIR_FALLING)
					return AD7291_DATA_HIGH(offset);
			else
					return AD7291_DATA_LOW(offset);
	case IIO_EV_INFO_HYSTERESIS:
			return AD7291_HYST(offset);
	default:
			break;
	}
	return 0;
}

static int ad7291_read_event_value(struct iio_dev *indio_dev,
				   const struct iio_chan_spec *chan,
				   enum iio_event_type type,
				   enum iio_event_direction dir,
				   enum iio_event_info info,
				   int *val, int *val2)
{
	struct ad7291_chip_info *chip = iio_priv(indio_dev);
	int ret;
	u16 uval;

	ret = ad7291_i2c_read(chip, ad7291_threshold_reg(chan, dir, info),
		&uval);
	if (ret < 0)
		return ret;

	if (info == IIO_EV_INFO_HYSTERESIS || chan->type == IIO_VOLTAGE)
		*val = uval & AD7291_VALUE_MASK;

	else
		*val = sign_extend32(uval, 11);

	return IIO_VAL_INT;
}

static int ad7291_write_event_value(struct iio_dev *indio_dev,
				    const struct iio_chan_spec *chan,
				    enum iio_event_type type,
				    enum iio_event_direction dir,
				    enum iio_event_info info,
				    int val, int val2)
{
	struct ad7291_chip_info *chip = iio_priv(indio_dev);

	if (info == IIO_EV_INFO_HYSTERESIS || chan->type == IIO_VOLTAGE) {
		if (val > AD7291_VALUE_MASK || val < 0)
			return -EINVAL;
	} else {
		if (val > 2047 || val < -2048)
			return -EINVAL;
	}

	return ad7291_i2c_write(chip, ad7291_threshold_reg(chan, dir, info),
		val);
}

static int ad7291_read_event_config(struct iio_dev *indio_dev,
				    const struct iio_chan_spec *chan,
				    enum iio_event_type type,
				    enum iio_event_direction dir)
>>>>>>> refs/remotes/origin/master
{
	struct ad7291_chip_info *chip = iio_priv(indio_dev);
	/* To be enabled the channel must simply be on. If any are enabled
	   we are in continuous sampling mode */

<<<<<<< HEAD
	switch (IIO_EVENT_CODE_EXTRACT_CHAN_TYPE(event_code)) {
	case IIO_VOLTAGE:
		if (chip->c_mask &
		    (1 << (15 - IIO_EVENT_CODE_EXTRACT_CHAN(event_code))))
=======
	switch (chan->type) {
	case IIO_VOLTAGE:
		if (chip->c_mask & (1 << (15 - chan->channel)))
>>>>>>> refs/remotes/origin/master
			return 1;
		else
			return 0;
	case IIO_TEMP:
		/* always on */
		return 1;
	default:
		return -EINVAL;
	}

}

static int ad7291_write_event_config(struct iio_dev *indio_dev,
<<<<<<< HEAD
				     u64 event_code,
=======
				     const struct iio_chan_spec *chan,
				     enum iio_event_type type,
				     enum iio_event_direction dir,
>>>>>>> refs/remotes/origin/master
				     int state)
{
	int ret = 0;
	struct ad7291_chip_info *chip = iio_priv(indio_dev);
<<<<<<< HEAD
=======
	unsigned int mask;
>>>>>>> refs/remotes/origin/master
	u16 regval;

	mutex_lock(&chip->state_lock);
	regval = chip->command;
	/*
	 * To be enabled the channel must simply be on. If any are enabled
	 * use continuous sampling mode.
	 * Possible to disable temp as well but that makes single read tricky.
	 */

<<<<<<< HEAD
	switch (IIO_EVENT_CODE_EXTRACT_TYPE(event_code)) {
	case IIO_VOLTAGE:
		if ((!state) && (chip->c_mask & (1 << (15 -
				IIO_EVENT_CODE_EXTRACT_CHAN(event_code)))))
			chip->c_mask &= ~(1 << (15 - IIO_EVENT_CODE_EXTRACT_CHAN
							(event_code)));
		else if (state && (!(chip->c_mask & (1 << (15 -
				IIO_EVENT_CODE_EXTRACT_CHAN(event_code))))))
			chip->c_mask |= (1 << (15 - IIO_EVENT_CODE_EXTRACT_CHAN
							(event_code)));
=======
	mask = BIT(15 - chan->channel);

	switch (chan->type) {
	case IIO_VOLTAGE:
		if ((!state) && (chip->c_mask & mask))
			chip->c_mask &= ~mask;
		else if (state && (!(chip->c_mask & mask)))
			chip->c_mask |= mask;
>>>>>>> refs/remotes/origin/master
		else
			break;

		regval &= ~AD7291_AUTOCYCLE;
		regval |= chip->c_mask;
		if (chip->c_mask) /* Enable autocycle? */
			regval |= AD7291_AUTOCYCLE;

		ret = ad7291_i2c_write(chip, AD7291_COMMAND, regval);
		if (ret < 0)
			goto error_ret;

		chip->command = regval;
		break;
	default:
		ret = -EINVAL;
	}

error_ret:
	mutex_unlock(&chip->state_lock);
	return ret;
}

static int ad7291_read_raw(struct iio_dev *indio_dev,
			   struct iio_chan_spec const *chan,
			   int *val,
			   int *val2,
			   long mask)
{
	int ret;
	struct ad7291_chip_info *chip = iio_priv(indio_dev);
<<<<<<< HEAD
	unsigned int scale_uv;
	u16 regval;
	s16 signval;

	switch (mask) {
	case 0:
=======
	u16 regval;

	switch (mask) {
	case IIO_CHAN_INFO_RAW:
>>>>>>> refs/remotes/origin/master
		switch (chan->type) {
		case IIO_VOLTAGE:
			mutex_lock(&chip->state_lock);
			/* If in autocycle mode drop through */
			if (chip->command & AD7291_AUTOCYCLE) {
				mutex_unlock(&chip->state_lock);
				return -EBUSY;
			}
			/* Enable this channel alone */
			regval = chip->command & (~AD7291_VOLTAGE_MASK);
			regval |= 1 << (15 - chan->channel);
			ret = ad7291_i2c_write(chip, AD7291_COMMAND, regval);
			if (ret < 0) {
				mutex_unlock(&chip->state_lock);
				return ret;
			}
			/* Read voltage */
<<<<<<< HEAD
			ret = i2c_smbus_read_word_data(chip->client,
=======
			ret = i2c_smbus_read_word_swapped(chip->client,
>>>>>>> refs/remotes/origin/master
						       AD7291_VOLTAGE);
			if (ret < 0) {
				mutex_unlock(&chip->state_lock);
				return ret;
			}
<<<<<<< HEAD
			*val = swab16((u16)ret) & AD7291_VALUE_MASK;
=======
			*val = ret & AD7291_VALUE_MASK;
>>>>>>> refs/remotes/origin/master
			mutex_unlock(&chip->state_lock);
			return IIO_VAL_INT;
		case IIO_TEMP:
			/* Assumes tsense bit of command register always set */
<<<<<<< HEAD
			ret = i2c_smbus_read_word_data(chip->client,
						       AD7291_T_SENSE);
			if (ret < 0)
				return ret;
			signval = (s16)((swab16((u16)ret) &
				AD7291_VALUE_MASK) << 4) >> 4;
			*val = signval;
=======
			ret = i2c_smbus_read_word_swapped(chip->client,
						       AD7291_T_SENSE);
			if (ret < 0)
				return ret;
			*val = sign_extend32(ret, 11);
>>>>>>> refs/remotes/origin/master
			return IIO_VAL_INT;
		default:
			return -EINVAL;
		}
	case IIO_CHAN_INFO_AVERAGE_RAW:
<<<<<<< HEAD
		ret = i2c_smbus_read_word_data(chip->client,
					       AD7291_T_AVERAGE);
			if (ret < 0)
				return ret;
			signval = (s16)((swab16((u16)ret) &
				AD7291_VALUE_MASK) << 4) >> 4;
			*val = signval;
=======
		ret = i2c_smbus_read_word_swapped(chip->client,
					       AD7291_T_AVERAGE);
			if (ret < 0)
				return ret;
			*val = sign_extend32(ret, 11);
>>>>>>> refs/remotes/origin/master
			return IIO_VAL_INT;
	case IIO_CHAN_INFO_SCALE:
		switch (chan->type) {
		case IIO_VOLTAGE:
<<<<<<< HEAD
			scale_uv = (chip->int_vref_mv * 1000) >> AD7291_BITS;
			*val =  scale_uv / 1000;
			*val2 = (scale_uv % 1000) * 1000;
			return IIO_VAL_INT_PLUS_MICRO;
=======
			if (chip->reg) {
				int vref;
				vref = regulator_get_voltage(chip->reg);
				if (vref < 0)
					return vref;
				*val = vref / 1000;
			} else {
				*val = 2500;
			}
			*val2 = AD7291_BITS;
			return IIO_VAL_FRACTIONAL_LOG2;
>>>>>>> refs/remotes/origin/master
		case IIO_TEMP:
			/*
			 * One LSB of the ADC corresponds to 0.25 deg C.
			 * The temperature reading is in 12-bit twos
			 * complement format
			 */
			*val = 250;
			return IIO_VAL_INT;
		default:
			return -EINVAL;
		}
	default:
		return -EINVAL;
	}
}

<<<<<<< HEAD
#define AD7291_VOLTAGE_CHAN(_chan)					\
{									\
	.type = IIO_VOLTAGE,						\
	.info_mask = IIO_CHAN_INFO_SCALE_SHARED_BIT,			\
	.indexed = 1,							\
	.channel = _chan,						\
	.event_mask = IIO_EV_BIT(IIO_EV_TYPE_THRESH, IIO_EV_DIR_RISING)|\
	IIO_EV_BIT(IIO_EV_TYPE_THRESH, IIO_EV_DIR_FALLING)		\
=======
static const struct iio_event_spec ad7291_events[] = {
	{
		.type = IIO_EV_TYPE_THRESH,
		.dir = IIO_EV_DIR_RISING,
		.mask_separate = BIT(IIO_EV_INFO_VALUE) |
			BIT(IIO_EV_INFO_ENABLE),
	}, {
		.type = IIO_EV_TYPE_THRESH,
		.dir = IIO_EV_DIR_FALLING,
		.mask_separate = BIT(IIO_EV_INFO_VALUE) |
			BIT(IIO_EV_INFO_ENABLE),
	}, {
		.type = IIO_EV_TYPE_THRESH,
		.dir = IIO_EV_DIR_EITHER,
		.mask_separate = BIT(IIO_EV_INFO_HYSTERESIS),
	},
};

#define AD7291_VOLTAGE_CHAN(_chan)					\
{									\
	.type = IIO_VOLTAGE,						\
	.info_mask_separate = BIT(IIO_CHAN_INFO_RAW),			\
	.info_mask_shared_by_type = BIT(IIO_CHAN_INFO_SCALE),		\
	.indexed = 1,							\
	.channel = _chan,						\
	.event_spec = ad7291_events,					\
	.num_event_specs = ARRAY_SIZE(ad7291_events),			\
>>>>>>> refs/remotes/origin/master
}

static const struct iio_chan_spec ad7291_channels[] = {
	AD7291_VOLTAGE_CHAN(0),
	AD7291_VOLTAGE_CHAN(1),
	AD7291_VOLTAGE_CHAN(2),
	AD7291_VOLTAGE_CHAN(3),
	AD7291_VOLTAGE_CHAN(4),
	AD7291_VOLTAGE_CHAN(5),
	AD7291_VOLTAGE_CHAN(6),
	AD7291_VOLTAGE_CHAN(7),
	{
		.type = IIO_TEMP,
<<<<<<< HEAD
		.info_mask = IIO_CHAN_INFO_AVERAGE_RAW_SEPARATE_BIT |
				IIO_CHAN_INFO_SCALE_SEPARATE_BIT,
		.indexed = 1,
		.channel = 0,
		.event_mask =
		IIO_EV_BIT(IIO_EV_TYPE_THRESH, IIO_EV_DIR_RISING)|
		IIO_EV_BIT(IIO_EV_TYPE_THRESH, IIO_EV_DIR_FALLING)
	}
>>>>>>> refs/remotes/origin/cm-10.0
};

static struct attribute_group ad7291_event_attribute_group = {
	.attrs = ad7291_event_attributes,
};

static const struct iio_info ad7291_info = {
	.attrs = &ad7291_attribute_group,
<<<<<<< HEAD
	.num_interrupt_lines = 1,
	.event_attrs = &ad7291_event_attribute_group,
};

/*
 * device probe and remove
 */

=======
=======
		.info_mask_separate = BIT(IIO_CHAN_INFO_RAW) |
				BIT(IIO_CHAN_INFO_AVERAGE_RAW) |
				BIT(IIO_CHAN_INFO_SCALE),
		.indexed = 1,
		.channel = 0,
		.event_spec = ad7291_events,
		.num_event_specs = ARRAY_SIZE(ad7291_events),
	}
};

static const struct iio_info ad7291_info = {
>>>>>>> refs/remotes/origin/master
	.read_raw = &ad7291_read_raw,
	.read_event_config = &ad7291_read_event_config,
	.write_event_config = &ad7291_write_event_config,
	.read_event_value = &ad7291_read_event_value,
	.write_event_value = &ad7291_write_event_value,
<<<<<<< HEAD
	.event_attrs = &ad7291_event_attribute_group,
};

>>>>>>> refs/remotes/origin/cm-10.0
static int __devinit ad7291_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{
	struct ad7291_chip_info *chip;
<<<<<<< HEAD
	int ret = 0;

	chip = kzalloc(sizeof(struct ad7291_chip_info), GFP_KERNEL);

	if (chip == NULL)
		return -ENOMEM;

	/* this is only used for device removal purposes */
	i2c_set_clientdata(client, chip);

	chip->client = client;
	chip->command = AD7291_NOISE_DELAY | AD7291_T_SENSE_MASK;

	chip->indio_dev = iio_allocate_device(0);
	if (chip->indio_dev == NULL) {
		ret = -ENOMEM;
		goto error_free_chip;
	}

	chip->indio_dev->name = id->name;
	chip->indio_dev->dev.parent = &client->dev;
	chip->indio_dev->info = &ad7291_info;
	chip->indio_dev->dev_data = (void *)chip;
	chip->indio_dev->modes = INDIO_DIRECT_MODE;

	ret = iio_device_register(chip->indio_dev);
	if (ret)
		goto error_free_dev;
=======
	struct iio_dev *indio_dev;
	int ret = 0, voltage_uv = 0;

	indio_dev = iio_allocate_device(sizeof(*chip));
	if (indio_dev == NULL) {
		ret = -ENOMEM;
		goto error_ret;
	}
	chip = iio_priv(indio_dev);

	chip->reg = regulator_get(&client->dev, "vcc");
	if (!IS_ERR(chip->reg)) {
		ret = regulator_enable(chip->reg);
		if (ret)
			goto error_put_reg;
		voltage_uv = regulator_get_voltage(chip->reg);
=======
	.driver_module = THIS_MODULE,
};

static int ad7291_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{
	struct ad7291_platform_data *pdata = client->dev.platform_data;
	struct ad7291_chip_info *chip;
	struct iio_dev *indio_dev;
	int ret = 0;

	indio_dev = devm_iio_device_alloc(&client->dev, sizeof(*chip));
	if (!indio_dev)
		return -ENOMEM;
	chip = iio_priv(indio_dev);

	if (pdata && pdata->use_external_ref) {
		chip->reg = devm_regulator_get(&client->dev, "vref");
		if (IS_ERR(chip->reg))
			return ret;

		ret = regulator_enable(chip->reg);
		if (ret)
			return ret;
>>>>>>> refs/remotes/origin/master
	}

	mutex_init(&chip->state_lock);
	/* this is only used for device removal purposes */
	i2c_set_clientdata(client, indio_dev);

	chip->client = client;

	chip->command = AD7291_NOISE_DELAY |
			AD7291_T_SENSE_MASK | /* Tsense always enabled */
			AD7291_ALERT_POLARITY; /* set irq polarity low level */

<<<<<<< HEAD
	if (voltage_uv) {
		chip->int_vref_mv = voltage_uv / 1000;
		chip->command |= AD7291_EXT_REF;
	} else {
		chip->int_vref_mv = 2500; /* Build-in ref */
	}
=======
	if (pdata && pdata->use_external_ref)
		chip->command |= AD7291_EXT_REF;
>>>>>>> refs/remotes/origin/master

	indio_dev->name = id->name;
	indio_dev->channels = ad7291_channels;
	indio_dev->num_channels = ARRAY_SIZE(ad7291_channels);

	indio_dev->dev.parent = &client->dev;
	indio_dev->info = &ad7291_info;
	indio_dev->modes = INDIO_DIRECT_MODE;

	ret = ad7291_i2c_write(chip, AD7291_COMMAND, AD7291_RESET);
	if (ret) {
		ret = -EIO;
		goto error_disable_reg;
	}

	ret = ad7291_i2c_write(chip, AD7291_COMMAND, chip->command);
	if (ret) {
		ret = -EIO;
		goto error_disable_reg;
	}
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	if (client->irq > 0) {
		ret = request_threaded_irq(client->irq,
					   NULL,
					   &ad7291_event_handler,
					   IRQF_TRIGGER_LOW | IRQF_ONESHOT,
					   id->name,
<<<<<<< HEAD
<<<<<<< HEAD
					   chip->indio_dev);
		if (ret)
			goto error_unreg_dev;

		/* set irq polarity low level */
		chip->command |= AD7291_ALART_POLARITY;
	}

	ret = ad7291_i2c_write(chip, AD7291_COMMAND, chip->command);
	if (ret) {
		ret = -EIO;
		goto error_unreg_irq;
	}

	dev_info(&client->dev, "%s temperature sensor registered.\n",
=======
=======
>>>>>>> refs/remotes/origin/master
					   indio_dev);
		if (ret)
			goto error_disable_reg;
	}

	ret = iio_device_register(indio_dev);
	if (ret)
		goto error_unreg_irq;

<<<<<<< HEAD
	dev_info(&client->dev, "%s ADC registered.\n",
>>>>>>> refs/remotes/origin/cm-10.0
			 id->name);

	return 0;

error_unreg_irq:
<<<<<<< HEAD
	free_irq(client->irq, chip->indio_dev);
error_unreg_dev:
	iio_device_unregister(chip->indio_dev);
error_free_dev:
	iio_free_device(chip->indio_dev);
error_free_chip:
	kfree(chip);

=======
	if (client->irq)
		free_irq(client->irq, indio_dev);
error_disable_reg:
	if (!IS_ERR(chip->reg))
		regulator_disable(chip->reg);
error_put_reg:
	if (!IS_ERR(chip->reg))
		regulator_put(chip->reg);

	iio_free_device(indio_dev);
error_ret:
>>>>>>> refs/remotes/origin/cm-10.0
	return ret;
}

static int __devexit ad7291_remove(struct i2c_client *client)
{
<<<<<<< HEAD
	struct ad7291_chip_info *chip = i2c_get_clientdata(client);
	struct iio_dev *indio_dev = chip->indio_dev;

	if (client->irq)
		free_irq(client->irq, chip->indio_dev);
	iio_device_unregister(indio_dev);
	iio_free_device(chip->indio_dev);
	kfree(chip);
=======
=======
	return 0;

error_unreg_irq:
	if (client->irq)
		free_irq(client->irq, indio_dev);
error_disable_reg:
	if (chip->reg)
		regulator_disable(chip->reg);

	return ret;
}

static int ad7291_remove(struct i2c_client *client)
{
>>>>>>> refs/remotes/origin/master
	struct iio_dev *indio_dev = i2c_get_clientdata(client);
	struct ad7291_chip_info *chip = iio_priv(indio_dev);

	iio_device_unregister(indio_dev);

	if (client->irq)
		free_irq(client->irq, indio_dev);

<<<<<<< HEAD
	if (!IS_ERR(chip->reg)) {
		regulator_disable(chip->reg);
		regulator_put(chip->reg);
	}

	iio_free_device(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (chip->reg)
		regulator_disable(chip->reg);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static const struct i2c_device_id ad7291_id[] = {
	{ "ad7291", 0 },
	{}
};

MODULE_DEVICE_TABLE(i2c, ad7291_id);

static struct i2c_driver ad7291_driver = {
	.driver = {
<<<<<<< HEAD
<<<<<<< HEAD
		.name = "ad7291",
=======
		.name = KBUILD_MODNAME,
>>>>>>> refs/remotes/origin/cm-10.0
	},
	.probe = ad7291_probe,
	.remove = __devexit_p(ad7291_remove),
	.id_table = ad7291_id,
};
<<<<<<< HEAD

static __init int ad7291_init(void)
{
	return i2c_add_driver(&ad7291_driver);
}

static __exit void ad7291_exit(void)
{
	i2c_del_driver(&ad7291_driver);
}

MODULE_AUTHOR("Sonic Zhang <sonic.zhang@analog.com>");
MODULE_DESCRIPTION("Analog Devices AD7291 digital"
			" temperature sensor driver");
MODULE_LICENSE("GPL v2");

module_init(ad7291_init);
module_exit(ad7291_exit);
=======
=======
		.name = KBUILD_MODNAME,
	},
	.probe = ad7291_probe,
	.remove = ad7291_remove,
	.id_table = ad7291_id,
};
>>>>>>> refs/remotes/origin/master
module_i2c_driver(ad7291_driver);

MODULE_AUTHOR("Sonic Zhang <sonic.zhang@analog.com>");
MODULE_DESCRIPTION("Analog Devices AD7291 ADC driver");
MODULE_LICENSE("GPL v2");
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
