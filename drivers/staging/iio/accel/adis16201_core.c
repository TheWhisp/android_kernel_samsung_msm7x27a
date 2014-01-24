/*
 * ADIS16201 Programmable Digital Vibration Sensor driver
 *
 * Copyright 2010 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/spi/spi.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
<<<<<<< HEAD
<<<<<<< HEAD

#include "../iio.h"
#include "../sysfs.h"
#include "../ring_generic.h"

#include "accel.h"
#include "inclinometer.h"
#include "../adc/adc.h"

#include "adis16201.h"

#define DRIVER_NAME		"adis16201"

=======
#include <linux/module.h>

#include "../iio.h"
#include "../sysfs.h"
#include "../buffer.h"

#include "adis16201.h"

>>>>>>> refs/remotes/origin/cm-10.0
enum adis16201_chan {
	in_supply,
	temp,
	accel_x,
	accel_y,
	incli_x,
	incli_y,
	in_aux,
};

/**
 * adis16201_spi_write_reg_8() - write single byte to a register
 * @dev: device associated with child of actual device (iio_dev or iio_trig)
 * @reg_address: the address of the register to be written
 * @val: the value to write
 **/
<<<<<<< HEAD
static int adis16201_spi_write_reg_8(struct device *dev,
=======
static int adis16201_spi_write_reg_8(struct iio_dev *indio_dev,
>>>>>>> refs/remotes/origin/cm-10.0
		u8 reg_address,
		u8 val)
{
	int ret;
<<<<<<< HEAD
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct adis16201_state *st = iio_dev_get_devdata(indio_dev);
=======
	struct adis16201_state *st = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0

	mutex_lock(&st->buf_lock);
	st->tx[0] = ADIS16201_WRITE_REG(reg_address);
	st->tx[1] = val;

	ret = spi_write(st->us, st->tx, 2);
	mutex_unlock(&st->buf_lock);

	return ret;
}

/**
 * adis16201_spi_write_reg_16() - write 2 bytes to a pair of registers
 * @indio_dev: iio device associated with child of actual device
 * @reg_address: the address of the lower of the two registers. Second register
 *               is assumed to have address one greater.
 * @val: value to be written
 **/
static int adis16201_spi_write_reg_16(struct iio_dev *indio_dev,
				      u8 lower_reg_address,
				      u16 value)
{
	int ret;
	struct spi_message msg;
<<<<<<< HEAD
	struct adis16201_state *st = iio_dev_get_devdata(indio_dev);
=======
	struct adis16201_state *st = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
	struct spi_transfer xfers[] = {
		{
			.tx_buf = st->tx,
			.bits_per_word = 8,
			.len = 2,
			.cs_change = 1,
		}, {
			.tx_buf = st->tx + 2,
			.bits_per_word = 8,
			.len = 2,
		},
	};

	mutex_lock(&st->buf_lock);
	st->tx[0] = ADIS16201_WRITE_REG(lower_reg_address);
	st->tx[1] = value & 0xFF;
	st->tx[2] = ADIS16201_WRITE_REG(lower_reg_address + 1);
	st->tx[3] = (value >> 8) & 0xFF;

	spi_message_init(&msg);
	spi_message_add_tail(&xfers[0], &msg);
	spi_message_add_tail(&xfers[1], &msg);
	ret = spi_sync(st->us, &msg);
	mutex_unlock(&st->buf_lock);

	return ret;
}

/**
 * adis16201_spi_read_reg_16() - read 2 bytes from a 16-bit register
 * @indio_dev: iio device associated with child of actual device
 * @reg_address: the address of the lower of the two registers. Second register
 *               is assumed to have address one greater.
 * @val: somewhere to pass back the value read
 **/
static int adis16201_spi_read_reg_16(struct iio_dev *indio_dev,
		u8 lower_reg_address,
		u16 *val)
{
	struct spi_message msg;
<<<<<<< HEAD
	struct adis16201_state *st = iio_dev_get_devdata(indio_dev);
=======
	struct adis16201_state *st = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
	int ret;
	struct spi_transfer xfers[] = {
		{
			.tx_buf = st->tx,
			.bits_per_word = 8,
			.len = 2,
			.cs_change = 1,
			.delay_usecs = 20,
		}, {
			.rx_buf = st->rx,
			.bits_per_word = 8,
			.len = 2,
			.delay_usecs = 20,
		},
	};

	mutex_lock(&st->buf_lock);
	st->tx[0] = ADIS16201_READ_REG(lower_reg_address);
	st->tx[1] = 0;

	spi_message_init(&msg);
	spi_message_add_tail(&xfers[0], &msg);
	spi_message_add_tail(&xfers[1], &msg);
	ret = spi_sync(st->us, &msg);
	if (ret) {
		dev_err(&st->us->dev, "problem when reading 16 bit register 0x%02X",
				lower_reg_address);
		goto error_ret;
	}
	*val = (st->rx[0] << 8) | st->rx[1];

error_ret:
	mutex_unlock(&st->buf_lock);
	return ret;
}

<<<<<<< HEAD
static int adis16201_reset(struct device *dev)
{
	int ret;
	ret = adis16201_spi_write_reg_8(dev,
			ADIS16201_GLOB_CMD,
			ADIS16201_GLOB_CMD_SW_RESET);
	if (ret)
		dev_err(dev, "problem resetting device");
=======
static int adis16201_reset(struct iio_dev *indio_dev)
{
	int ret;
	struct adis16201_state *st = iio_priv(indio_dev);

	ret = adis16201_spi_write_reg_8(indio_dev,
			ADIS16201_GLOB_CMD,
			ADIS16201_GLOB_CMD_SW_RESET);
	if (ret)
		dev_err(&st->us->dev, "problem resetting device");
>>>>>>> refs/remotes/origin/cm-10.0

	return ret;
}

static ssize_t adis16201_write_reset(struct device *dev,
		struct device_attribute *attr,
		const char *buf, size_t len)
{
<<<<<<< HEAD
	if (len < 1)
		return -EINVAL;
	switch (buf[0]) {
	case '1':
	case 'y':
	case 'Y':
		return adis16201_reset(dev);
	}
	return -EINVAL;
=======
	int ret;
	bool res;

	if (len < 1)
		return -EINVAL;
	ret = strtobool(buf, &res);
	if (ret || !res)
		return ret;
	return adis16201_reset(dev_get_drvdata(dev));
>>>>>>> refs/remotes/origin/cm-10.0
}

int adis16201_set_irq(struct iio_dev *indio_dev, bool enable)
{
	int ret = 0;
	u16 msc;

	ret = adis16201_spi_read_reg_16(indio_dev, ADIS16201_MSC_CTRL, &msc);
	if (ret)
		goto error_ret;

	msc |= ADIS16201_MSC_CTRL_ACTIVE_HIGH;
	msc &= ~ADIS16201_MSC_CTRL_DATA_RDY_DIO1;
	if (enable)
		msc |= ADIS16201_MSC_CTRL_DATA_RDY_EN;
	else
		msc &= ~ADIS16201_MSC_CTRL_DATA_RDY_EN;

	ret = adis16201_spi_write_reg_16(indio_dev, ADIS16201_MSC_CTRL, msc);

error_ret:
	return ret;
}

static int adis16201_check_status(struct iio_dev *indio_dev)
{
	u16 status;
	int ret;

	ret = adis16201_spi_read_reg_16(indio_dev,
					ADIS16201_DIAG_STAT, &status);
	if (ret < 0) {
		dev_err(&indio_dev->dev, "Reading status failed\n");
		goto error_ret;
	}
	ret = status & 0xF;
	if (ret)
		ret = -EFAULT;

	if (status & ADIS16201_DIAG_STAT_SPI_FAIL)
		dev_err(&indio_dev->dev, "SPI failure\n");
	if (status & ADIS16201_DIAG_STAT_FLASH_UPT)
		dev_err(&indio_dev->dev, "Flash update failed\n");
	if (status & ADIS16201_DIAG_STAT_POWER_HIGH)
		dev_err(&indio_dev->dev, "Power supply above 3.625V\n");
	if (status & ADIS16201_DIAG_STAT_POWER_LOW)
		dev_err(&indio_dev->dev, "Power supply below 3.15V\n");

error_ret:
	return ret;
}

static int adis16201_self_test(struct iio_dev *indio_dev)
{
	int ret;
	ret = adis16201_spi_write_reg_16(indio_dev,
			ADIS16201_MSC_CTRL,
			ADIS16201_MSC_CTRL_SELF_TEST_EN);
	if (ret) {
		dev_err(&indio_dev->dev, "problem starting self test");
		goto err_ret;
	}

	ret = adis16201_check_status(indio_dev);

err_ret:
	return ret;
}

<<<<<<< HEAD
static int adis16201_initial_setup(struct adis16201_state *st)
{
	int ret;
	struct device *dev = &st->indio_dev->dev;

	/* Disable IRQ */
	ret = adis16201_set_irq(st->indio_dev, false);
=======
static int adis16201_initial_setup(struct iio_dev *indio_dev)
{
	int ret;
	struct device *dev = &indio_dev->dev;

	/* Disable IRQ */
	ret = adis16201_set_irq(indio_dev, false);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret) {
		dev_err(dev, "disable irq failed");
		goto err_ret;
	}

	/* Do self test */
<<<<<<< HEAD
	ret = adis16201_self_test(st->indio_dev);
=======
	ret = adis16201_self_test(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret) {
		dev_err(dev, "self test failure");
		goto err_ret;
	}

	/* Read status register to check the result */
<<<<<<< HEAD
	ret = adis16201_check_status(st->indio_dev);
	if (ret) {
		adis16201_reset(dev);
		dev_err(dev, "device not playing ball -> reset");
		msleep(ADIS16201_STARTUP_DELAY);
		ret = adis16201_check_status(st->indio_dev);
=======
	ret = adis16201_check_status(indio_dev);
	if (ret) {
		adis16201_reset(indio_dev);
		dev_err(dev, "device not playing ball -> reset");
		msleep(ADIS16201_STARTUP_DELAY);
		ret = adis16201_check_status(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
		if (ret) {
			dev_err(dev, "giving up");
			goto err_ret;
		}
	}

<<<<<<< HEAD
	printk(KERN_INFO DRIVER_NAME ": at CS%d (irq %d)\n",
			st->us->chip_select, st->us->irq);

=======
>>>>>>> refs/remotes/origin/cm-10.0
err_ret:
	return ret;
}

static u8 adis16201_addresses[7][2] = {
	[in_supply] = { ADIS16201_SUPPLY_OUT, },
	[temp] = { ADIS16201_TEMP_OUT },
	[accel_x] = { ADIS16201_XACCL_OUT, ADIS16201_XACCL_OFFS },
	[accel_y] = { ADIS16201_YACCL_OUT, ADIS16201_YACCL_OFFS },
	[in_aux] = { ADIS16201_AUX_ADC },
	[incli_x] = { ADIS16201_XINCL_OUT },
	[incli_y] = { ADIS16201_YINCL_OUT },
=======
#include <linux/module.h>

#include <linux/iio/iio.h>
#include <linux/iio/sysfs.h>
#include <linux/iio/buffer.h>
#include <linux/iio/imu/adis.h>

#include "adis16201.h"

static const u8 adis16201_addresses[] = {
	[ADIS16201_SCAN_ACC_X] = ADIS16201_XACCL_OFFS,
	[ADIS16201_SCAN_ACC_Y] = ADIS16201_YACCL_OFFS,
	[ADIS16201_SCAN_INCLI_X] = ADIS16201_XINCL_OFFS,
	[ADIS16201_SCAN_INCLI_Y] = ADIS16201_YINCL_OFFS,
>>>>>>> refs/remotes/origin/master
};

static int adis16201_read_raw(struct iio_dev *indio_dev,
			      struct iio_chan_spec const *chan,
			      int *val, int *val2,
			      long mask)
{
<<<<<<< HEAD
=======
	struct adis *st = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/master
	int ret;
	int bits;
	u8 addr;
	s16 val16;

	switch (mask) {
<<<<<<< HEAD
	case 0:
		mutex_lock(&indio_dev->mlock);
		addr = adis16201_addresses[chan->address][0];
		ret = adis16201_spi_read_reg_16(indio_dev, addr, &val16);
<<<<<<< HEAD
		if (ret)
			return ret;

		if (val16 & ADIS16201_ERROR_ACTIVE) {
			ret = adis16201_check_status(indio_dev);
			if (ret)
				return ret;
=======
		if (ret) {
			mutex_unlock(&indio_dev->mlock);
			return ret;
		}

		if (val16 & ADIS16201_ERROR_ACTIVE) {
			ret = adis16201_check_status(indio_dev);
			if (ret) {
				mutex_unlock(&indio_dev->mlock);
				return ret;
			}
>>>>>>> refs/remotes/origin/cm-10.0
		}
		val16 = val16 & ((1 << chan->scan_type.realbits) - 1);
		if (chan->scan_type.sign == 's')
			val16 = (s16)(val16 <<
				      (16 - chan->scan_type.realbits)) >>
				(16 - chan->scan_type.realbits);
		*val = val16;
		mutex_unlock(&indio_dev->mlock);
		return IIO_VAL_INT;
<<<<<<< HEAD
	case (1 << IIO_CHAN_INFO_SCALE_SEPARATE):
	case (1 << IIO_CHAN_INFO_SCALE_SHARED):
		switch (chan->type) {
		case IIO_IN:
=======
	case IIO_CHAN_INFO_SCALE:
		switch (chan->type) {
		case IIO_VOLTAGE:
>>>>>>> refs/remotes/origin/cm-10.0
			*val = 0;
			if (chan->channel == 0)
				*val2 = 1220;
			else
				*val2 = 610;
			return IIO_VAL_INT_PLUS_MICRO;
		case IIO_TEMP:
			*val = 0;
			*val2 = -470000;
			return IIO_VAL_INT_PLUS_MICRO;
		case IIO_ACCEL:
			*val = 0;
			*val2 = 462500;
			return IIO_VAL_INT_PLUS_MICRO;
		case IIO_INCLI:
			*val = 0;
			*val2 = 100000;
=======
	case IIO_CHAN_INFO_RAW:
		return adis_single_conversion(indio_dev, chan,
				ADIS16201_ERROR_ACTIVE, val);
	case IIO_CHAN_INFO_SCALE:
		switch (chan->type) {
		case IIO_VOLTAGE:
			if (chan->channel == 0) {
				*val = 1;
				*val2 = 220000; /* 1.22 mV */
			} else {
				*val = 0;
				*val2 = 610000; /* 0.610 mV */
			}
			return IIO_VAL_INT_PLUS_MICRO;
		case IIO_TEMP:
			*val = -470; /* 0.47 C */
			*val2 = 0;
			return IIO_VAL_INT_PLUS_MICRO;
		case IIO_ACCEL:
			*val = 0;
			*val2 = IIO_G_TO_M_S_2(462400); /* 0.4624 mg */
			return IIO_VAL_INT_PLUS_NANO;
		case IIO_INCLI:
			*val = 0;
			*val2 = 100000; /* 0.1 degree */
>>>>>>> refs/remotes/origin/master
			return IIO_VAL_INT_PLUS_MICRO;
		default:
			return -EINVAL;
		}
		break;
<<<<<<< HEAD
<<<<<<< HEAD
	case (1 << IIO_CHAN_INFO_OFFSET_SEPARATE):
		*val = 25;
		return IIO_VAL_INT;
	case (1 << IIO_CHAN_INFO_CALIBBIAS_SEPARATE):
=======
	case IIO_CHAN_INFO_OFFSET:
		*val = 25;
		return IIO_VAL_INT;
	case IIO_CHAN_INFO_CALIBBIAS:
>>>>>>> refs/remotes/origin/cm-10.0
=======
	case IIO_CHAN_INFO_OFFSET:
		*val = 25000 / -470 - 1278; /* 25 C = 1278 */
		return IIO_VAL_INT;
	case IIO_CHAN_INFO_CALIBBIAS:
>>>>>>> refs/remotes/origin/master
		switch (chan->type) {
		case IIO_ACCEL:
			bits = 12;
			break;
		case IIO_INCLI:
			bits = 9;
			break;
		default:
			return -EINVAL;
<<<<<<< HEAD
		};
		mutex_lock(&indio_dev->mlock);
		addr = adis16201_addresses[chan->address][1];
		ret = adis16201_spi_read_reg_16(indio_dev, addr, &val16);
=======
		}
		mutex_lock(&indio_dev->mlock);
		addr = adis16201_addresses[chan->scan_index];
		ret = adis_read_reg_16(st, addr, &val16);
>>>>>>> refs/remotes/origin/master
		if (ret) {
			mutex_unlock(&indio_dev->mlock);
			return ret;
		}
		val16 &= (1 << bits) - 1;
		val16 = (s16)(val16 << (16 - bits)) >> (16 - bits);
		*val = val16;
		mutex_unlock(&indio_dev->mlock);
		return IIO_VAL_INT;
	}
	return -EINVAL;
}

static int adis16201_write_raw(struct iio_dev *indio_dev,
			       struct iio_chan_spec const *chan,
			       int val,
			       int val2,
			       long mask)
{
<<<<<<< HEAD
=======
	struct adis *st = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/master
	int bits;
	s16 val16;
	u8 addr;
	switch (mask) {
<<<<<<< HEAD
<<<<<<< HEAD
	case (1 << IIO_CHAN_INFO_CALIBBIAS_SEPARATE):
=======
	case IIO_CHAN_INFO_CALIBBIAS:
>>>>>>> refs/remotes/origin/cm-10.0
=======
	case IIO_CHAN_INFO_CALIBBIAS:
>>>>>>> refs/remotes/origin/master
		switch (chan->type) {
		case IIO_ACCEL:
			bits = 12;
			break;
		case IIO_INCLI:
			bits = 9;
			break;
		default:
			return -EINVAL;
<<<<<<< HEAD
		};
		val16 = val & ((1 << bits) - 1);
		addr = adis16201_addresses[chan->address][1];
		return adis16201_spi_write_reg_16(indio_dev, addr, val16);
=======
		}
		val16 = val & ((1 << bits) - 1);
		addr = adis16201_addresses[chan->scan_index];
		return adis_write_reg_16(st, addr, val16);
>>>>>>> refs/remotes/origin/master
	}
	return -EINVAL;
}

<<<<<<< HEAD
static struct iio_chan_spec adis16201_channels[] = {
<<<<<<< HEAD
	IIO_CHAN(IIO_IN, 0, 1, 0, "supply", 0, 0,
		 (1 << IIO_CHAN_INFO_SCALE_SEPARATE),
		 in_supply, ADIS16201_SCAN_SUPPLY,
		 IIO_ST('u', 12, 16, 0), 0),
	IIO_CHAN(IIO_TEMP, 0, 1, 0, NULL, 0, 0,
		 (1 << IIO_CHAN_INFO_SCALE_SEPARATE) |
		 (1 << IIO_CHAN_INFO_OFFSET_SEPARATE),
		 temp, ADIS16201_SCAN_TEMP,
		 IIO_ST('u', 12, 16, 0), 0),
	IIO_CHAN(IIO_ACCEL, 1, 0, 0, NULL, 0, IIO_MOD_X,
		 (1 << IIO_CHAN_INFO_SCALE_SHARED) |
		 (1 << IIO_CHAN_INFO_CALIBBIAS_SEPARATE),
		 accel_x, ADIS16201_SCAN_ACC_X,
		 IIO_ST('s', 14, 16, 0), 0),
	IIO_CHAN(IIO_ACCEL, 1, 0, 0, NULL, 0, IIO_MOD_Y,
		 (1 << IIO_CHAN_INFO_SCALE_SHARED) |
		 (1 << IIO_CHAN_INFO_CALIBBIAS_SEPARATE),
		 accel_y, ADIS16201_SCAN_ACC_Y,
		 IIO_ST('s', 14, 16, 0), 0),
	IIO_CHAN(IIO_IN, 0, 1, 0, NULL, 1, 0,
		 (1 << IIO_CHAN_INFO_SCALE_SEPARATE),
		 in_aux, ADIS16201_SCAN_AUX_ADC,
		 IIO_ST('u', 12, 16, 0), 0),
	IIO_CHAN(IIO_INCLI, 1, 0, 0, NULL, 0, IIO_MOD_X,
		 (1 << IIO_CHAN_INFO_SCALE_SHARED) |
		 (1 << IIO_CHAN_INFO_CALIBBIAS_SEPARATE),
		 incli_x, ADIS16201_SCAN_INCLI_X,
		 IIO_ST('s', 14, 16, 0), 0),
	IIO_CHAN(IIO_INCLI, 1, 0, 0, NULL, 0, IIO_MOD_Y,
		 (1 << IIO_CHAN_INFO_SCALE_SHARED) |
		 (1 << IIO_CHAN_INFO_CALIBBIAS_SEPARATE),
=======
	IIO_CHAN(IIO_VOLTAGE, 0, 1, 0, "supply", 0, 0,
		 IIO_CHAN_INFO_SCALE_SEPARATE_BIT,
		 in_supply, ADIS16201_SCAN_SUPPLY,
		 IIO_ST('u', 12, 16, 0), 0),
	IIO_CHAN(IIO_TEMP, 0, 1, 0, NULL, 0, 0,
		 IIO_CHAN_INFO_SCALE_SEPARATE_BIT |
		 IIO_CHAN_INFO_OFFSET_SEPARATE_BIT,
		 temp, ADIS16201_SCAN_TEMP,
		 IIO_ST('u', 12, 16, 0), 0),
	IIO_CHAN(IIO_ACCEL, 1, 0, 0, NULL, 0, IIO_MOD_X,
		 IIO_CHAN_INFO_SCALE_SHARED_BIT |
		 IIO_CHAN_INFO_CALIBBIAS_SEPARATE_BIT,
		 accel_x, ADIS16201_SCAN_ACC_X,
		 IIO_ST('s', 14, 16, 0), 0),
	IIO_CHAN(IIO_ACCEL, 1, 0, 0, NULL, 0, IIO_MOD_Y,
		 IIO_CHAN_INFO_SCALE_SHARED_BIT |
		 IIO_CHAN_INFO_CALIBBIAS_SEPARATE_BIT,
		 accel_y, ADIS16201_SCAN_ACC_Y,
		 IIO_ST('s', 14, 16, 0), 0),
	IIO_CHAN(IIO_VOLTAGE, 0, 1, 0, NULL, 1, 0,
		 IIO_CHAN_INFO_SCALE_SEPARATE_BIT,
		 in_aux, ADIS16201_SCAN_AUX_ADC,
		 IIO_ST('u', 12, 16, 0), 0),
	IIO_CHAN(IIO_INCLI, 1, 0, 0, NULL, 0, IIO_MOD_X,
		 IIO_CHAN_INFO_SCALE_SHARED_BIT |
		 IIO_CHAN_INFO_CALIBBIAS_SEPARATE_BIT,
		 incli_x, ADIS16201_SCAN_INCLI_X,
		 IIO_ST('s', 14, 16, 0), 0),
	IIO_CHAN(IIO_INCLI, 1, 0, 0, NULL, 0, IIO_MOD_Y,
		 IIO_CHAN_INFO_SCALE_SHARED_BIT |
		 IIO_CHAN_INFO_CALIBBIAS_SEPARATE_BIT,
>>>>>>> refs/remotes/origin/cm-10.0
		 incli_y, ADIS16201_SCAN_INCLI_Y,
		 IIO_ST('s', 14, 16, 0), 0),
	IIO_CHAN_SOFT_TIMESTAMP(7)
};

static IIO_DEVICE_ATTR(reset, S_IWUSR, NULL, adis16201_write_reset, 0);

static struct attribute *adis16201_attributes[] = {
	&iio_dev_attr_reset.dev_attr.attr,
	NULL
};

static const struct attribute_group adis16201_attribute_group = {
	.attrs = adis16201_attributes,
};

static const struct iio_info adis16201_info = {
	.attrs = &adis16201_attribute_group,
	.read_raw = &adis16201_read_raw,
	.write_raw = &adis16201_write_raw,
	.driver_module = THIS_MODULE,
};

static int __devinit adis16201_probe(struct spi_device *spi)
{
<<<<<<< HEAD
	int ret, regdone = 0;
	struct adis16201_state *st = kzalloc(sizeof *st, GFP_KERNEL);
	if (!st) {
		ret =  -ENOMEM;
		goto error_ret;
	}
	/* this is only used for removal purposes */
	spi_set_drvdata(spi, st);

	/* Allocate the comms buffers */
	st->rx = kzalloc(sizeof(*st->rx)*ADIS16201_MAX_RX, GFP_KERNEL);
	if (st->rx == NULL) {
		ret = -ENOMEM;
		goto error_free_st;
	}
	st->tx = kzalloc(sizeof(*st->tx)*ADIS16201_MAX_TX, GFP_KERNEL);
	if (st->tx == NULL) {
		ret = -ENOMEM;
		goto error_free_rx;
	}
	st->us = spi;
	mutex_init(&st->buf_lock);
	/* setup the industrialio driver allocated elements */
	st->indio_dev = iio_allocate_device(0);
	if (st->indio_dev == NULL) {
		ret = -ENOMEM;
		goto error_free_tx;
	}

	st->indio_dev->name = spi->dev.driver->name;
	st->indio_dev->dev.parent = &spi->dev;
	st->indio_dev->info = &adis16201_info;

	st->indio_dev->channels = adis16201_channels;
	st->indio_dev->num_channels = ARRAY_SIZE(adis16201_channels);
	st->indio_dev->dev_data = (void *)(st);
	st->indio_dev->modes = INDIO_DIRECT_MODE;

	ret = adis16201_configure_ring(st->indio_dev);
	if (ret)
		goto error_free_dev;

	ret = iio_device_register(st->indio_dev);
	if (ret)
		goto error_unreg_ring_funcs;
	regdone = 1;

	ret = iio_ring_buffer_register_ex(st->indio_dev->ring, 0,
					  adis16201_channels,
					  ARRAY_SIZE(adis16201_channels));
=======
	int ret;
	struct adis16201_state *st;
	struct iio_dev *indio_dev;

	/* setup the industrialio driver allocated elements */
	indio_dev = iio_allocate_device(sizeof(*st));
	if (indio_dev == NULL) {
		ret = -ENOMEM;
		goto error_ret;
	}
=======
static const struct iio_chan_spec adis16201_channels[] = {
	ADIS_SUPPLY_CHAN(ADIS16201_SUPPLY_OUT, ADIS16201_SCAN_SUPPLY, 12),
	ADIS_TEMP_CHAN(ADIS16201_TEMP_OUT, ADIS16201_SCAN_TEMP, 12),
	ADIS_ACCEL_CHAN(X, ADIS16201_XACCL_OUT, ADIS16201_SCAN_ACC_X,
		BIT(IIO_CHAN_INFO_CALIBBIAS), 14),
	ADIS_ACCEL_CHAN(Y, ADIS16201_YACCL_OUT, ADIS16201_SCAN_ACC_Y,
		BIT(IIO_CHAN_INFO_CALIBBIAS), 14),
	ADIS_AUX_ADC_CHAN(ADIS16201_AUX_ADC, ADIS16201_SCAN_AUX_ADC, 12),
	ADIS_INCLI_CHAN(X, ADIS16201_XINCL_OUT, ADIS16201_SCAN_INCLI_X,
		BIT(IIO_CHAN_INFO_CALIBBIAS), 14),
	ADIS_INCLI_CHAN(X, ADIS16201_YINCL_OUT, ADIS16201_SCAN_INCLI_Y,
		BIT(IIO_CHAN_INFO_CALIBBIAS), 14),
	IIO_CHAN_SOFT_TIMESTAMP(7)
};

static const struct iio_info adis16201_info = {
	.read_raw = &adis16201_read_raw,
	.write_raw = &adis16201_write_raw,
	.update_scan_mode = adis_update_scan_mode,
	.driver_module = THIS_MODULE,
};

static const char * const adis16201_status_error_msgs[] = {
	[ADIS16201_DIAG_STAT_SPI_FAIL_BIT] = "SPI failure",
	[ADIS16201_DIAG_STAT_FLASH_UPT_BIT] = "Flash update failed",
	[ADIS16201_DIAG_STAT_POWER_HIGH_BIT] = "Power supply above 3.625V",
	[ADIS16201_DIAG_STAT_POWER_LOW_BIT] = "Power supply below 3.15V",
};

static const struct adis_data adis16201_data = {
	.read_delay = 20,
	.msc_ctrl_reg = ADIS16201_MSC_CTRL,
	.glob_cmd_reg = ADIS16201_GLOB_CMD,
	.diag_stat_reg = ADIS16201_DIAG_STAT,

	.self_test_mask = ADIS16201_MSC_CTRL_SELF_TEST_EN,
	.startup_delay = ADIS16201_STARTUP_DELAY,

	.status_error_msgs = adis16201_status_error_msgs,
	.status_error_mask = BIT(ADIS16201_DIAG_STAT_SPI_FAIL_BIT) |
		BIT(ADIS16201_DIAG_STAT_FLASH_UPT_BIT) |
		BIT(ADIS16201_DIAG_STAT_POWER_HIGH_BIT) |
		BIT(ADIS16201_DIAG_STAT_POWER_LOW_BIT),
};

static int adis16201_probe(struct spi_device *spi)
{
	int ret;
	struct adis *st;
	struct iio_dev *indio_dev;

	/* setup the industrialio driver allocated elements */
	indio_dev = devm_iio_device_alloc(&spi->dev, sizeof(*st));
	if (!indio_dev)
		return -ENOMEM;

>>>>>>> refs/remotes/origin/master
	st = iio_priv(indio_dev);
	/* this is only used for removal purposes */
	spi_set_drvdata(spi, indio_dev);

<<<<<<< HEAD
	st->us = spi;
	mutex_init(&st->buf_lock);

=======
>>>>>>> refs/remotes/origin/master
	indio_dev->name = spi->dev.driver->name;
	indio_dev->dev.parent = &spi->dev;
	indio_dev->info = &adis16201_info;

	indio_dev->channels = adis16201_channels;
	indio_dev->num_channels = ARRAY_SIZE(adis16201_channels);
	indio_dev->modes = INDIO_DIRECT_MODE;

<<<<<<< HEAD
	ret = adis16201_configure_ring(indio_dev);
	if (ret)
		goto error_free_dev;

	ret = iio_buffer_register(indio_dev,
				  adis16201_channels,
				  ARRAY_SIZE(adis16201_channels));
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret) {
		printk(KERN_ERR "failed to initialize the ring\n");
		goto error_unreg_ring_funcs;
	}

	if (spi->irq) {
<<<<<<< HEAD
		ret = adis16201_probe_trigger(st->indio_dev);
=======
		ret = adis16201_probe_trigger(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
		if (ret)
			goto error_uninitialize_ring;
	}

	/* Get the device into a sane initial state */
<<<<<<< HEAD
	ret = adis16201_initial_setup(st);
	if (ret)
		goto error_remove_trigger;
	return 0;

error_remove_trigger:
	adis16201_remove_trigger(st->indio_dev);
error_uninitialize_ring:
	iio_ring_buffer_unregister(st->indio_dev->ring);
error_unreg_ring_funcs:
	adis16201_unconfigure_ring(st->indio_dev);
error_free_dev:
	if (regdone)
		iio_device_unregister(st->indio_dev);
	else
		iio_free_device(st->indio_dev);
error_free_tx:
	kfree(st->tx);
error_free_rx:
	kfree(st->rx);
error_free_st:
	kfree(st);
=======
	ret = adis16201_initial_setup(indio_dev);
	if (ret)
		goto error_remove_trigger;

	ret = iio_device_register(indio_dev);
	if (ret < 0)
		goto error_remove_trigger;
	return 0;

error_remove_trigger:
	adis16201_remove_trigger(indio_dev);
error_uninitialize_ring:
	iio_buffer_unregister(indio_dev);
error_unreg_ring_funcs:
	adis16201_unconfigure_ring(indio_dev);
error_free_dev:
	iio_free_device(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
error_ret:
=======
	ret = adis_init(st, indio_dev, spi, &adis16201_data);
	if (ret)
		return ret;
	ret = adis_setup_buffer_and_trigger(st, indio_dev, NULL);
	if (ret)
		return ret;

	/* Get the device into a sane initial state */
	ret = adis_initial_startup(st);
	if (ret)
		goto error_cleanup_buffer_trigger;

	ret = iio_device_register(indio_dev);
	if (ret < 0)
		goto error_cleanup_buffer_trigger;
	return 0;

error_cleanup_buffer_trigger:
	adis_cleanup_buffer_and_trigger(st, indio_dev);
>>>>>>> refs/remotes/origin/master
	return ret;
}

static int adis16201_remove(struct spi_device *spi)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct adis16201_state *st = spi_get_drvdata(spi);
	struct iio_dev *indio_dev = st->indio_dev;

	adis16201_remove_trigger(indio_dev);
	iio_ring_buffer_unregister(indio_dev->ring);
	iio_device_unregister(indio_dev);
	adis16201_unconfigure_ring(indio_dev);
	kfree(st->tx);
	kfree(st->rx);
	kfree(st);
=======
	struct iio_dev *indio_dev = spi_get_drvdata(spi);

	iio_device_unregister(indio_dev);
	adis16201_remove_trigger(indio_dev);
	iio_buffer_unregister(indio_dev);
	adis16201_unconfigure_ring(indio_dev);
	iio_free_device(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct iio_dev *indio_dev = spi_get_drvdata(spi);
	struct adis *st = iio_priv(indio_dev);

	iio_device_unregister(indio_dev);
	adis_cleanup_buffer_and_trigger(st, indio_dev);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static struct spi_driver adis16201_driver = {
	.driver = {
		.name = "adis16201",
		.owner = THIS_MODULE,
	},
	.probe = adis16201_probe,
<<<<<<< HEAD
	.remove = __devexit_p(adis16201_remove),
};
<<<<<<< HEAD

static __init int adis16201_init(void)
{
	return spi_register_driver(&adis16201_driver);
}
module_init(adis16201_init);

static __exit void adis16201_exit(void)
{
	spi_unregister_driver(&adis16201_driver);
}
module_exit(adis16201_exit);
=======
module_spi_driver(adis16201_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.remove = adis16201_remove,
};
module_spi_driver(adis16201_driver);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Barry Song <21cnbao@gmail.com>");
MODULE_DESCRIPTION("Analog Devices ADIS16201 Programmable Digital Vibration Sensor driver");
MODULE_LICENSE("GPL v2");
<<<<<<< HEAD
<<<<<<< HEAD
=======
MODULE_ALIAS("spi:adis16201");
>>>>>>> refs/remotes/origin/cm-10.0
=======
MODULE_ALIAS("spi:adis16201");
>>>>>>> refs/remotes/origin/master
