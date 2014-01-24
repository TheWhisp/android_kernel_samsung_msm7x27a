/*
 * ADIS16220 Programmable Digital Vibration Sensor driver
 *
 * Copyright 2010 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/gpio.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/spi/spi.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/list.h>

#include "../iio.h"
#include "../sysfs.h"
#include "accel.h"
#include "../adc/adc.h"
=======
#include <linux/module.h>

#include "../iio.h"
#include "../sysfs.h"
>>>>>>> refs/remotes/origin/cm-10.0

#include "adis16220.h"

#define DRIVER_NAME		"adis16220"

/**
 * adis16220_spi_write_reg_8() - write single byte to a register
<<<<<<< HEAD
 * @dev: device associated with child of actual device (iio_dev or iio_trig)
 * @reg_address: the address of the register to be written
 * @val: the value to write
 **/
static int adis16220_spi_write_reg_8(struct device *dev,
=======
 * @indio_dev: iio device associated with child of actual device
 * @reg_address: the address of the register to be written
 * @val: the value to write
 **/
static int adis16220_spi_write_reg_8(struct iio_dev *indio_dev,
>>>>>>> refs/remotes/origin/cm-10.0
		u8 reg_address,
		u8 val)
{
	int ret;
<<<<<<< HEAD
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct adis16220_state *st = iio_dev_get_devdata(indio_dev);
=======
	struct adis16220_state *st = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0

	mutex_lock(&st->buf_lock);
	st->tx[0] = ADIS16220_WRITE_REG(reg_address);
	st->tx[1] = val;

	ret = spi_write(st->us, st->tx, 2);
	mutex_unlock(&st->buf_lock);

	return ret;
}

/**
 * adis16220_spi_write_reg_16() - write 2 bytes to a pair of registers
<<<<<<< HEAD
 * @dev: device associated with child of actual device (iio_dev or iio_trig)
=======
 * @indio_dev:  iio device associated with child of actual device
>>>>>>> refs/remotes/origin/cm-10.0
 * @reg_address: the address of the lower of the two registers. Second register
 *               is assumed to have address one greater.
 * @val: value to be written
 **/
<<<<<<< HEAD
static int adis16220_spi_write_reg_16(struct device *dev,
=======
static int adis16220_spi_write_reg_16(struct iio_dev *indio_dev,
>>>>>>> refs/remotes/origin/cm-10.0
		u8 lower_reg_address,
		u16 value)
{
	int ret;
	struct spi_message msg;
<<<<<<< HEAD
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct adis16220_state *st = iio_dev_get_devdata(indio_dev);
=======
	struct adis16220_state *st = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
	struct spi_transfer xfers[] = {
		{
			.tx_buf = st->tx,
			.bits_per_word = 8,
			.len = 2,
			.cs_change = 1,
			.delay_usecs = 35,
		}, {
			.tx_buf = st->tx + 2,
			.bits_per_word = 8,
			.len = 2,
<<<<<<< HEAD
			.cs_change = 1,
=======
>>>>>>> refs/remotes/origin/cm-10.0
			.delay_usecs = 35,
		},
	};

	mutex_lock(&st->buf_lock);
	st->tx[0] = ADIS16220_WRITE_REG(lower_reg_address);
	st->tx[1] = value & 0xFF;
	st->tx[2] = ADIS16220_WRITE_REG(lower_reg_address + 1);
	st->tx[3] = (value >> 8) & 0xFF;

	spi_message_init(&msg);
	spi_message_add_tail(&xfers[0], &msg);
	spi_message_add_tail(&xfers[1], &msg);
	ret = spi_sync(st->us, &msg);
	mutex_unlock(&st->buf_lock);

	return ret;
}

/**
 * adis16220_spi_read_reg_16() - read 2 bytes from a 16-bit register
<<<<<<< HEAD
 * @dev: device associated with child of actual device (iio_dev or iio_trig)
=======
 * @indio_dev: iio device associated with child of actual device
>>>>>>> refs/remotes/origin/cm-10.0
 * @reg_address: the address of the lower of the two registers. Second register
 *               is assumed to have address one greater.
 * @val: somewhere to pass back the value read
 **/
<<<<<<< HEAD
static int adis16220_spi_read_reg_16(struct device *dev,
		u8 lower_reg_address,
		u16 *val)
{
	struct spi_message msg;
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct adis16220_state *st = iio_dev_get_devdata(indio_dev);
=======
static int adis16220_spi_read_reg_16(struct iio_dev *indio_dev,
				     u8 lower_reg_address,
				     u16 *val)
{
	struct spi_message msg;
	struct adis16220_state *st = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
	int ret;
	struct spi_transfer xfers[] = {
		{
			.tx_buf = st->tx,
			.bits_per_word = 8,
			.len = 2,
			.cs_change = 1,
			.delay_usecs = 35,
		}, {
			.rx_buf = st->rx,
			.bits_per_word = 8,
			.len = 2,
			.cs_change = 1,
			.delay_usecs = 35,
		},
	};

	mutex_lock(&st->buf_lock);
	st->tx[0] = ADIS16220_READ_REG(lower_reg_address);
	st->tx[1] = 0;

	spi_message_init(&msg);
	spi_message_add_tail(&xfers[0], &msg);
	spi_message_add_tail(&xfers[1], &msg);
	ret = spi_sync(st->us, &msg);
	if (ret) {
		dev_err(&st->us->dev,
			"problem when reading 16 bit register 0x%02X",
			lower_reg_address);
		goto error_ret;
	}
	*val = (st->rx[0] << 8) | st->rx[1];

error_ret:
	mutex_unlock(&st->buf_lock);
	return ret;
}

<<<<<<< HEAD
static ssize_t adis16220_spi_read_signed(struct device *dev,
		struct device_attribute *attr,
		char *buf,
		unsigned bits)
{
	int ret;
	s16 val = 0;
	unsigned shift = 16 - bits;
	struct iio_dev_attr *this_attr = to_iio_dev_attr(attr);

	ret = adis16220_spi_read_reg_16(dev, this_attr->address, (u16 *)&val);
	if (ret)
		return ret;

	val = ((s16)(val << shift) >> shift);
	return sprintf(buf, "%d\n", val);
}

static ssize_t adis16220_read_12bit_unsigned(struct device *dev,
		struct device_attribute *attr,
		char *buf)
{
	int ret;
	u16 val = 0;
	struct iio_dev_attr *this_attr = to_iio_dev_attr(attr);

	ret = adis16220_spi_read_reg_16(dev, this_attr->address, &val);
	if (ret)
		return ret;

	return sprintf(buf, "%u\n", val & 0x0FFF);
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/module.h>

#include <linux/iio/iio.h>
#include <linux/iio/sysfs.h>

#include "adis16220.h"

>>>>>>> refs/remotes/origin/master
static ssize_t adis16220_read_16bit(struct device *dev,
		struct device_attribute *attr,
		char *buf)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	ssize_t ret;

	/* Take the iio_dev status lock */
	mutex_lock(&indio_dev->mlock);
	ret =  adis16220_spi_read_signed(dev, attr, buf, 16);
	mutex_unlock(&indio_dev->mlock);

	return ret;
=======
	struct iio_dev_attr *this_attr = to_iio_dev_attr(attr);
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
=======
	struct iio_dev_attr *this_attr = to_iio_dev_attr(attr);
	struct iio_dev *indio_dev = dev_to_iio_dev(dev);
	struct adis16220_state *st = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/master
	ssize_t ret;
	s16 val = 0;

	/* Take the iio_dev status lock */
	mutex_lock(&indio_dev->mlock);
<<<<<<< HEAD
	ret = adis16220_spi_read_reg_16(indio_dev, this_attr->address,
=======
	ret = adis_read_reg_16(&st->adis, this_attr->address,
>>>>>>> refs/remotes/origin/master
					(u16 *)&val);
	mutex_unlock(&indio_dev->mlock);
	if (ret)
		return ret;
	return sprintf(buf, "%d\n", val);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static ssize_t adis16220_write_16bit(struct device *dev,
		struct device_attribute *attr,
		const char *buf,
		size_t len)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct iio_dev_attr *this_attr = to_iio_dev_attr(attr);
	int ret;
	long val;

	ret = strict_strtol(buf, 10, &val);
	if (ret)
		goto error_ret;
	ret = adis16220_spi_write_reg_16(dev, this_attr->address, val);
=======
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct iio_dev_attr *this_attr = to_iio_dev_attr(attr);
=======
	struct iio_dev *indio_dev = dev_to_iio_dev(dev);
	struct iio_dev_attr *this_attr = to_iio_dev_attr(attr);
	struct adis16220_state *st = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/master
	int ret;
	u16 val;

	ret = kstrtou16(buf, 10, &val);
	if (ret)
		goto error_ret;
<<<<<<< HEAD
	ret = adis16220_spi_write_reg_16(indio_dev, this_attr->address, val);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = adis_write_reg_16(&st->adis, this_attr->address, val);
>>>>>>> refs/remotes/origin/master

error_ret:
	return ret ? ret : len;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int adis16220_capture(struct device *dev)
{
	int ret;
	ret = adis16220_spi_write_reg_16(dev,
			ADIS16220_GLOB_CMD,
			0xBF08); /* initiates a manual data capture */
	if (ret)
		dev_err(dev, "problem beginning capture");
=======
static int adis16220_capture(struct iio_dev *indio_dev)
{
	int ret;
	ret = adis16220_spi_write_reg_16(indio_dev,
			ADIS16220_GLOB_CMD,
			0xBF08); /* initiates a manual data capture */
	if (ret)
		dev_err(&indio_dev->dev, "problem beginning capture");
>>>>>>> refs/remotes/origin/cm-10.0
=======
static int adis16220_capture(struct iio_dev *indio_dev)
{
	struct adis16220_state *st = iio_priv(indio_dev);
	int ret;

	 /* initiates a manual data capture */
	ret = adis_write_reg_16(&st->adis, ADIS16220_GLOB_CMD, 0xBF08);
	if (ret)
		dev_err(&indio_dev->dev, "problem beginning capture");
>>>>>>> refs/remotes/origin/master

	msleep(10); /* delay for capture to finish */

	return ret;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int adis16220_reset(struct device *dev)
{
	int ret;
	ret = adis16220_spi_write_reg_8(dev,
			ADIS16220_GLOB_CMD,
			ADIS16220_GLOB_CMD_SW_RESET);
	if (ret)
		dev_err(dev, "problem resetting device");
=======
static int adis16220_reset(struct iio_dev *indio_dev)
{
	int ret;
	ret = adis16220_spi_write_reg_8(indio_dev,
			ADIS16220_GLOB_CMD,
			ADIS16220_GLOB_CMD_SW_RESET);
	if (ret)
		dev_err(&indio_dev->dev, "problem resetting device");
>>>>>>> refs/remotes/origin/cm-10.0

	return ret;
}

static ssize_t adis16220_write_reset(struct device *dev,
		struct device_attribute *attr,
		const char *buf, size_t len)
{
<<<<<<< HEAD
	if (len < 1)
		return -1;
	switch (buf[0]) {
	case '1':
	case 'y':
	case 'Y':
		return adis16220_reset(dev) == 0 ? len : -EIO;
	}
	return -1;
=======
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	bool val;
	int ret;

	ret = strtobool(buf, &val);
	if (ret)
		return ret;
	if (!val)
		return -EINVAL;

	ret = adis16220_reset(indio_dev);
	if (ret)
		return ret;
	return len;
>>>>>>> refs/remotes/origin/cm-10.0
}

=======
>>>>>>> refs/remotes/origin/master
static ssize_t adis16220_write_capture(struct device *dev,
		struct device_attribute *attr,
		const char *buf, size_t len)
{
<<<<<<< HEAD
<<<<<<< HEAD
	if (len < 1)
		return -1;
	switch (buf[0]) {
	case '1':
	case 'y':
	case 'Y':
		return adis16220_capture(dev) == 0 ? len : -EIO;
	}
	return -1;
}

static int adis16220_check_status(struct device *dev)
=======
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
=======
	struct iio_dev *indio_dev = dev_to_iio_dev(dev);
>>>>>>> refs/remotes/origin/master
	bool val;
	int ret;

	ret = strtobool(buf, &val);
	if (ret)
		return ret;
	if (!val)
		return -EINVAL;
	ret = adis16220_capture(indio_dev);
	if (ret)
		return ret;

	return len;
}

<<<<<<< HEAD
static int adis16220_check_status(struct iio_dev *indio_dev)
>>>>>>> refs/remotes/origin/cm-10.0
{
	u16 status;
	int ret;

<<<<<<< HEAD
	ret = adis16220_spi_read_reg_16(dev, ADIS16220_DIAG_STAT, &status);

	if (ret < 0) {
		dev_err(dev, "Reading status failed\n");
=======
	ret = adis16220_spi_read_reg_16(indio_dev, ADIS16220_DIAG_STAT,
					&status);

	if (ret < 0) {
		dev_err(&indio_dev->dev, "Reading status failed\n");
>>>>>>> refs/remotes/origin/cm-10.0
		goto error_ret;
	}
	ret = status & 0x7F;

	if (status & ADIS16220_DIAG_STAT_VIOLATION)
<<<<<<< HEAD
		dev_err(dev, "Capture period violation/interruption\n");
	if (status & ADIS16220_DIAG_STAT_SPI_FAIL)
		dev_err(dev, "SPI failure\n");
	if (status & ADIS16220_DIAG_STAT_FLASH_UPT)
		dev_err(dev, "Flash update failed\n");
	if (status & ADIS16220_DIAG_STAT_POWER_HIGH)
		dev_err(dev, "Power supply above 3.625V\n");
	if (status & ADIS16220_DIAG_STAT_POWER_LOW)
		dev_err(dev, "Power supply below 3.15V\n");
=======
		dev_err(&indio_dev->dev,
			"Capture period violation/interruption\n");
	if (status & ADIS16220_DIAG_STAT_SPI_FAIL)
		dev_err(&indio_dev->dev, "SPI failure\n");
	if (status & ADIS16220_DIAG_STAT_FLASH_UPT)
		dev_err(&indio_dev->dev, "Flash update failed\n");
	if (status & ADIS16220_DIAG_STAT_POWER_HIGH)
		dev_err(&indio_dev->dev, "Power supply above 3.625V\n");
	if (status & ADIS16220_DIAG_STAT_POWER_LOW)
		dev_err(&indio_dev->dev, "Power supply below 3.15V\n");
>>>>>>> refs/remotes/origin/cm-10.0

error_ret:
	return ret;
}

<<<<<<< HEAD
static int adis16220_self_test(struct device *dev)
{
	int ret;
	ret = adis16220_spi_write_reg_16(dev,
			ADIS16220_MSC_CTRL,
			ADIS16220_MSC_CTRL_SELF_TEST_EN);
	if (ret) {
		dev_err(dev, "problem starting self test");
		goto err_ret;
	}

	adis16220_check_status(dev);
=======
static int adis16220_self_test(struct iio_dev *indio_dev)
{
	int ret;
	ret = adis16220_spi_write_reg_16(indio_dev,
			ADIS16220_MSC_CTRL,
			ADIS16220_MSC_CTRL_SELF_TEST_EN);
	if (ret) {
		dev_err(&indio_dev->dev, "problem starting self test");
		goto err_ret;
	}

	adis16220_check_status(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0

err_ret:
	return ret;
}

<<<<<<< HEAD
static int adis16220_initial_setup(struct adis16220_state *st)
{
	int ret;
	struct device *dev = &st->indio_dev->dev;

	/* Do self test */
	ret = adis16220_self_test(dev);
	if (ret) {
		dev_err(dev, "self test failure");
=======
static int adis16220_initial_setup(struct iio_dev *indio_dev)
{
	int ret;

	/* Do self test */
	ret = adis16220_self_test(indio_dev);
	if (ret) {
		dev_err(&indio_dev->dev, "self test failure");
>>>>>>> refs/remotes/origin/cm-10.0
		goto err_ret;
	}

	/* Read status register to check the result */
<<<<<<< HEAD
	ret = adis16220_check_status(dev);
	if (ret) {
		adis16220_reset(dev);
		dev_err(dev, "device not playing ball -> reset");
		msleep(ADIS16220_STARTUP_DELAY);
		ret = adis16220_check_status(dev);
		if (ret) {
			dev_err(dev, "giving up");
=======
	ret = adis16220_check_status(indio_dev);
	if (ret) {
		adis16220_reset(indio_dev);
		dev_err(&indio_dev->dev, "device not playing ball -> reset");
		msleep(ADIS16220_STARTUP_DELAY);
		ret = adis16220_check_status(indio_dev);
		if (ret) {
			dev_err(&indio_dev->dev, "giving up");
>>>>>>> refs/remotes/origin/cm-10.0
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

<<<<<<< HEAD
static ssize_t adis16220_capture_buffer_read(struct adis16220_state *st,
=======
static ssize_t adis16220_capture_buffer_read(struct iio_dev *indio_dev,
>>>>>>> refs/remotes/origin/cm-10.0
=======
static ssize_t adis16220_capture_buffer_read(struct iio_dev *indio_dev,
>>>>>>> refs/remotes/origin/master
					char *buf,
					loff_t off,
					size_t count,
					int addr)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
	struct adis16220_state *st = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
	struct spi_message msg;
=======
	struct adis16220_state *st = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/master
	struct spi_transfer xfers[] = {
		{
			.tx_buf = st->tx,
			.bits_per_word = 8,
			.len = 2,
			.cs_change = 1,
			.delay_usecs = 25,
		}, {
			.tx_buf = st->tx,
			.rx_buf = st->rx,
			.bits_per_word = 8,
			.cs_change = 1,
			.delay_usecs = 25,
		},
	};
	int ret;
	int i;

	if (unlikely(!count))
		return count;

	if ((off >= ADIS16220_CAPTURE_SIZE) || (count & 1) || (off & 1))
		return -EINVAL;

	if (off + count > ADIS16220_CAPTURE_SIZE)
		count = ADIS16220_CAPTURE_SIZE - off;

	/* write the begin position of capture buffer */
<<<<<<< HEAD
<<<<<<< HEAD
	ret = adis16220_spi_write_reg_16(&st->indio_dev->dev,
=======
	ret = adis16220_spi_write_reg_16(indio_dev,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	ret = adis_write_reg_16(&st->adis,
>>>>>>> refs/remotes/origin/master
					ADIS16220_CAPT_PNTR,
					off > 1);
	if (ret)
		return -EIO;

	/* read count/2 values from capture buffer */
	mutex_lock(&st->buf_lock);

<<<<<<< HEAD
	for (i = 0; i < count; i += 2) {
		st->tx[i] = ADIS16220_READ_REG(addr);
=======

	for (i = 0; i < count; i += 2) {
		st->tx[i] = ADIS_READ_REG(addr);
>>>>>>> refs/remotes/origin/master
		st->tx[i + 1] = 0;
	}
	xfers[1].len = count;

<<<<<<< HEAD
	spi_message_init(&msg);
	spi_message_add_tail(&xfers[0], &msg);
	spi_message_add_tail(&xfers[1], &msg);
	ret = spi_sync(st->us, &msg);
=======
	ret = spi_sync_transfer(st->adis.spi, xfers, ARRAY_SIZE(xfers));
>>>>>>> refs/remotes/origin/master
	if (ret) {

		mutex_unlock(&st->buf_lock);
		return -EIO;
	}

	memcpy(buf, st->rx, count);

	mutex_unlock(&st->buf_lock);
	return count;
}

static ssize_t adis16220_accel_bin_read(struct file *filp, struct kobject *kobj,
					struct bin_attribute *attr,
					char *buf,
					loff_t off,
					size_t count)
{
<<<<<<< HEAD
	struct device *dev = container_of(kobj, struct device, kobj);
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct adis16220_state *st = iio_dev_get_devdata(indio_dev);

	return adis16220_capture_buffer_read(st, buf,
=======

	return adis16220_capture_buffer_read(indio_dev, buf,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct iio_dev *indio_dev = dev_to_iio_dev(kobj_to_dev(kobj));

	return adis16220_capture_buffer_read(indio_dev, buf,
>>>>>>> refs/remotes/origin/master
					off, count,
					ADIS16220_CAPT_BUFA);
}

static struct bin_attribute accel_bin = {
	.attr = {
		.name = "accel_bin",
		.mode = S_IRUGO,
	},
	.read = adis16220_accel_bin_read,
	.size = ADIS16220_CAPTURE_SIZE,
};

static ssize_t adis16220_adc1_bin_read(struct file *filp, struct kobject *kobj,
				struct bin_attribute *attr,
				char *buf, loff_t off,
				size_t count)
{
<<<<<<< HEAD
	struct device *dev = container_of(kobj, struct device, kobj);
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct adis16220_state *st = iio_dev_get_devdata(indio_dev);

	return adis16220_capture_buffer_read(st, buf,
=======

	return adis16220_capture_buffer_read(indio_dev, buf,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct iio_dev *indio_dev = dev_to_iio_dev(kobj_to_dev(kobj));

	return adis16220_capture_buffer_read(indio_dev, buf,
>>>>>>> refs/remotes/origin/master
					off, count,
					ADIS16220_CAPT_BUF1);
}

static struct bin_attribute adc1_bin = {
	.attr = {
		.name = "in0_bin",
		.mode = S_IRUGO,
	},
	.read =  adis16220_adc1_bin_read,
	.size = ADIS16220_CAPTURE_SIZE,
};

static ssize_t adis16220_adc2_bin_read(struct file *filp, struct kobject *kobj,
				struct bin_attribute *attr,
				char *buf, loff_t off,
				size_t count)
{
<<<<<<< HEAD
	struct device *dev = container_of(kobj, struct device, kobj);
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct adis16220_state *st = iio_dev_get_devdata(indio_dev);

	return adis16220_capture_buffer_read(st, buf,
=======

	return adis16220_capture_buffer_read(indio_dev, buf,
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct iio_dev *indio_dev = dev_to_iio_dev(kobj_to_dev(kobj));

	return adis16220_capture_buffer_read(indio_dev, buf,
>>>>>>> refs/remotes/origin/master
					off, count,
					ADIS16220_CAPT_BUF2);
}


static struct bin_attribute adc2_bin = {
	.attr = {
		.name = "in1_bin",
		.mode = S_IRUGO,
	},
	.read =  adis16220_adc2_bin_read,
	.size = ADIS16220_CAPTURE_SIZE,
};

<<<<<<< HEAD
<<<<<<< HEAD
static IIO_DEV_ATTR_IN_NAMED_RAW(0, supply, adis16220_read_12bit_unsigned,
		ADIS16220_CAPT_SUPPLY);
static IIO_CONST_ATTR_IN_NAMED_SCALE(0, supply, "0.0012207");
static IIO_DEV_ATTR_ACCEL(adis16220_read_16bit, ADIS16220_CAPT_BUFA);
static IIO_DEVICE_ATTR(accel_peak_raw, S_IRUGO, adis16220_read_16bit,
		NULL, ADIS16220_CAPT_PEAKA);
static IIO_DEV_ATTR_ACCEL_OFFSET(S_IWUSR | S_IRUGO,
		adis16220_read_16bit,
		adis16220_write_16bit,
		ADIS16220_ACCL_NULL);
static IIO_CONST_ATTR_ACCEL_SCALE("0.18704223545");
static IIO_DEV_ATTR_TEMP_RAW(adis16220_read_12bit_unsigned);
static IIO_CONST_ATTR_TEMP_OFFSET("25");
static IIO_CONST_ATTR_TEMP_SCALE("-0.47");

static IIO_DEV_ATTR_IN_RAW(1, adis16220_read_16bit, ADIS16220_CAPT_BUF1);
static IIO_DEV_ATTR_IN_RAW(2, adis16220_read_16bit, ADIS16220_CAPT_BUF2);

=======
>>>>>>> refs/remotes/origin/cm-10.0
static IIO_DEVICE_ATTR(reset, S_IWUSR, NULL,
		adis16220_write_reset, 0);

=======
>>>>>>> refs/remotes/origin/master
#define IIO_DEV_ATTR_CAPTURE(_store)				\
	IIO_DEVICE_ATTR(capture, S_IWUSR, NULL, _store, 0)

static IIO_DEV_ATTR_CAPTURE(adis16220_write_capture);

#define IIO_DEV_ATTR_CAPTURE_COUNT(_mode, _show, _store, _addr)		\
	IIO_DEVICE_ATTR(capture_count, _mode, _show, _store, _addr)

static IIO_DEV_ATTR_CAPTURE_COUNT(S_IWUSR | S_IRUGO,
		adis16220_read_16bit,
		adis16220_write_16bit,
		ADIS16220_CAPT_PNTR);

<<<<<<< HEAD
<<<<<<< HEAD
static IIO_CONST_ATTR_SAMP_FREQ_AVAIL("100200");

static struct attribute *adis16220_attributes[] = {
	&iio_dev_attr_in0_supply_raw.dev_attr.attr,
	&iio_const_attr_in0_supply_scale.dev_attr.attr,
	&iio_dev_attr_accel_raw.dev_attr.attr,
	&iio_dev_attr_accel_offset.dev_attr.attr,
	&iio_dev_attr_accel_peak_raw.dev_attr.attr,
	&iio_const_attr_accel_scale.dev_attr.attr,
	&iio_dev_attr_temp_raw.dev_attr.attr,
	&iio_dev_attr_in1_raw.dev_attr.attr,
	&iio_dev_attr_in2_raw.dev_attr.attr,
	&iio_const_attr_temp_offset.dev_attr.attr,
	&iio_const_attr_temp_scale.dev_attr.attr,
	&iio_const_attr_sampling_frequency_available.dev_attr.attr,
=======
=======
>>>>>>> refs/remotes/origin/master
enum adis16220_channel {
	in_supply, in_1, in_2, accel, temp
};

struct adis16220_address_spec {
	u8 addr;
	u8 bits;
	bool sign;
};

/* Address / bits / signed */
static const struct adis16220_address_spec adis16220_addresses[][3] = {
	[in_supply] =	{ { ADIS16220_CAPT_SUPPLY,	12, 0 }, },
	[in_1] =	{ { ADIS16220_CAPT_BUF1,	16, 1 },
			  { ADIS16220_AIN1_NULL,	16, 1 },
			  { ADIS16220_CAPT_PEAK1,	16, 1 }, },
	[in_2] =	{ { ADIS16220_CAPT_BUF2,	16, 1 },
			  { ADIS16220_AIN2_NULL,	16, 1 },
			  { ADIS16220_CAPT_PEAK2,	16, 1 }, },
	[accel] =	{ { ADIS16220_CAPT_BUFA,	16, 1 },
			  { ADIS16220_ACCL_NULL,	16, 1 },
			  { ADIS16220_CAPT_PEAKA,	16, 1 }, },
	[temp] =	{ { ADIS16220_CAPT_TEMP,	12, 0 }, }
};

static int adis16220_read_raw(struct iio_dev *indio_dev,
			      struct iio_chan_spec const *chan,
			      int *val, int *val2,
			      long mask)
{
<<<<<<< HEAD
=======
	struct adis16220_state *st = iio_priv(indio_dev);
	const struct adis16220_address_spec *addr;
>>>>>>> refs/remotes/origin/master
	int ret = -EINVAL;
	int addrind = 0;
	u16 uval;
	s16 sval;
	u8 bits;

	switch (mask) {
<<<<<<< HEAD
	case 0:
=======
	case IIO_CHAN_INFO_RAW:
>>>>>>> refs/remotes/origin/master
		addrind = 0;
		break;
	case IIO_CHAN_INFO_OFFSET:
		if (chan->type == IIO_TEMP) {
<<<<<<< HEAD
			*val = 25;
=======
			*val = 25000 / -470 - 1278; /* 25 C = 1278 */
>>>>>>> refs/remotes/origin/master
			return IIO_VAL_INT;
		}
		addrind = 1;
		break;
	case IIO_CHAN_INFO_PEAK:
		addrind = 2;
		break;
	case IIO_CHAN_INFO_SCALE:
<<<<<<< HEAD
		*val = 0;
		switch (chan->type) {
		case IIO_TEMP:
			*val2 = -470000;
			return IIO_VAL_INT_PLUS_MICRO;
		case IIO_ACCEL:
			*val2 = 1887042;
			return IIO_VAL_INT_PLUS_MICRO;
		case IIO_VOLTAGE:
			if (chan->channel == 0)
				*val2 = 0012221;
			else /* Should really be dependent on VDD */
				*val2 = 305;
=======
		switch (chan->type) {
		case IIO_TEMP:
			*val = -470; /* -0.47 C */
			*val2 = 0;
			return IIO_VAL_INT_PLUS_MICRO;
		case IIO_ACCEL:
			*val2 = IIO_G_TO_M_S_2(19073); /* 19.073 g */
			return IIO_VAL_INT_PLUS_MICRO;
		case IIO_VOLTAGE:
			if (chan->channel == 0) {
				*val = 1;
				*val2 = 220700; /* 1.2207 mV */
			} else {
				/* Should really be dependent on VDD */
				*val2 = 305180; /* 305.18 uV */
			}
>>>>>>> refs/remotes/origin/master
			return IIO_VAL_INT_PLUS_MICRO;
		default:
			return -EINVAL;
		}
	default:
		return -EINVAL;
	}
<<<<<<< HEAD
	if (adis16220_addresses[chan->address][addrind].sign) {
		ret = adis16220_spi_read_reg_16(indio_dev,
						adis16220_addresses[chan
								    ->address]
						[addrind].addr,
						&sval);
		if (ret)
			return ret;
		bits = adis16220_addresses[chan->address][addrind].bits;
=======
	addr = &adis16220_addresses[chan->address][addrind];
	if (addr->sign) {
		ret = adis_read_reg_16(&st->adis, addr->addr, &sval);
		if (ret)
			return ret;
		bits = addr->bits;
>>>>>>> refs/remotes/origin/master
		sval &= (1 << bits) - 1;
		sval = (s16)(sval << (16 - bits)) >> (16 - bits);
		*val = sval;
		return IIO_VAL_INT;
	} else {
<<<<<<< HEAD
		ret = adis16220_spi_read_reg_16(indio_dev,
						adis16220_addresses[chan
								    ->address]
						[addrind].addr,
						&uval);
		if (ret)
			return ret;
		bits = adis16220_addresses[chan->address][addrind].bits;
=======
		ret = adis_read_reg_16(&st->adis, addr->addr, &uval);
		if (ret)
			return ret;
		bits = addr->bits;
>>>>>>> refs/remotes/origin/master
		uval &= (1 << bits) - 1;
		*val = uval;
		return IIO_VAL_INT;
	}
}

static const struct iio_chan_spec adis16220_channels[] = {
	{
		.type = IIO_VOLTAGE,
		.indexed = 1,
		.channel = 0,
		.extend_name = "supply",
<<<<<<< HEAD
		.info_mask = IIO_CHAN_INFO_SCALE_SEPARATE_BIT,
		.address = in_supply,
	}, {
		.type = IIO_ACCEL,
		.info_mask = IIO_CHAN_INFO_OFFSET_SEPARATE_BIT |
			     IIO_CHAN_INFO_SCALE_SEPARATE_BIT |
			     IIO_CHAN_INFO_PEAK_SEPARATE_BIT,
=======
		.info_mask_separate = BIT(IIO_CHAN_INFO_RAW) |
			BIT(IIO_CHAN_INFO_SCALE),
		.address = in_supply,
	}, {
		.type = IIO_ACCEL,
		.info_mask_separate = BIT(IIO_CHAN_INFO_RAW) |
			BIT(IIO_CHAN_INFO_OFFSET) |
			BIT(IIO_CHAN_INFO_SCALE) |
			BIT(IIO_CHAN_INFO_PEAK),
>>>>>>> refs/remotes/origin/master
		.address = accel,
	}, {
		.type = IIO_TEMP,
		.indexed = 1,
		.channel = 0,
<<<<<<< HEAD
		.info_mask = IIO_CHAN_INFO_OFFSET_SEPARATE_BIT |
			     IIO_CHAN_INFO_SCALE_SEPARATE_BIT,
=======
		.info_mask_separate = BIT(IIO_CHAN_INFO_RAW) |
			BIT(IIO_CHAN_INFO_OFFSET) |
			BIT(IIO_CHAN_INFO_SCALE),
>>>>>>> refs/remotes/origin/master
		.address = temp,
	}, {
		.type = IIO_VOLTAGE,
		.indexed = 1,
		.channel = 1,
<<<<<<< HEAD
		.info_mask = IIO_CHAN_INFO_OFFSET_SEPARATE_BIT |
			     IIO_CHAN_INFO_SCALE_SEPARATE_BIT,
=======
		.info_mask_separate = BIT(IIO_CHAN_INFO_RAW) |
			BIT(IIO_CHAN_INFO_OFFSET) |
			BIT(IIO_CHAN_INFO_SCALE),
>>>>>>> refs/remotes/origin/master
		.address = in_1,
	}, {
		.type = IIO_VOLTAGE,
		.indexed = 1,
		.channel = 2,
<<<<<<< HEAD
=======
		.info_mask_separate = BIT(IIO_CHAN_INFO_RAW),
>>>>>>> refs/remotes/origin/master
		.address = in_2,
	}
};

static struct attribute *adis16220_attributes[] = {
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	&iio_dev_attr_reset.dev_attr.attr,
=======
>>>>>>> refs/remotes/origin/master
	&iio_dev_attr_capture.dev_attr.attr,
	&iio_dev_attr_capture_count.dev_attr.attr,
	NULL
};

static const struct attribute_group adis16220_attribute_group = {
	.attrs = adis16220_attributes,
};

static const struct iio_info adis16220_info = {
	.attrs = &adis16220_attribute_group,
	.driver_module = THIS_MODULE,
<<<<<<< HEAD
<<<<<<< HEAD
};
static int __devinit adis16220_probe(struct spi_device *spi)
{
	int ret, regdone = 0;
	struct adis16220_state *st = kzalloc(sizeof *st, GFP_KERNEL);
	if (!st) {
		ret =  -ENOMEM;
		goto error_ret;
	}
	/* this is only used for removal purposes */
	spi_set_drvdata(spi, st);

	/* Allocate the comms buffers */
	st->rx = kzalloc(sizeof(*st->rx)*ADIS16220_MAX_RX, GFP_KERNEL);
	if (st->rx == NULL) {
		ret = -ENOMEM;
		goto error_free_st;
	}
	st->tx = kzalloc(sizeof(*st->tx)*ADIS16220_MAX_TX, GFP_KERNEL);
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
	st->indio_dev->info = &adis16220_info;
	st->indio_dev->dev_data = (void *)(st);
	st->indio_dev->modes = INDIO_DIRECT_MODE;

	ret = iio_device_register(st->indio_dev);
	if (ret)
		goto error_free_dev;
	regdone = 1;

	ret = sysfs_create_bin_file(&st->indio_dev->dev.kobj, &accel_bin);
	if (ret)
		goto error_free_dev;

	ret = sysfs_create_bin_file(&st->indio_dev->dev.kobj, &adc1_bin);
	if (ret)
		goto error_rm_accel_bin;

	ret = sysfs_create_bin_file(&st->indio_dev->dev.kobj, &adc2_bin);
=======
	.read_raw = &adis16220_read_raw,
};

static int __devinit adis16220_probe(struct spi_device *spi)
=======
	.read_raw = &adis16220_read_raw,
};

static const char * const adis16220_status_error_msgs[] = {
	[ADIS16220_DIAG_STAT_VIOLATION_BIT] = "Capture period violation/interruption",
	[ADIS16220_DIAG_STAT_SPI_FAIL_BIT] = "SPI failure",
	[ADIS16220_DIAG_STAT_FLASH_UPT_BIT] = "Flash update failed",
	[ADIS16220_DIAG_STAT_POWER_HIGH_BIT] = "Power supply above 3.625V",
	[ADIS16220_DIAG_STAT_POWER_LOW_BIT] = "Power supply below 3.15V",
};

static const struct adis_data adis16220_data = {
	.read_delay = 35,
	.write_delay = 35,
	.msc_ctrl_reg = ADIS16220_MSC_CTRL,
	.glob_cmd_reg = ADIS16220_GLOB_CMD,
	.diag_stat_reg = ADIS16220_DIAG_STAT,

	.self_test_mask = ADIS16220_MSC_CTRL_SELF_TEST_EN,
	.startup_delay = ADIS16220_STARTUP_DELAY,

	.status_error_msgs = adis16220_status_error_msgs,
	.status_error_mask = BIT(ADIS16220_DIAG_STAT_VIOLATION_BIT) |
		BIT(ADIS16220_DIAG_STAT_SPI_FAIL_BIT) |
		BIT(ADIS16220_DIAG_STAT_FLASH_UPT_BIT) |
		BIT(ADIS16220_DIAG_STAT_POWER_HIGH_BIT) |
		BIT(ADIS16220_DIAG_STAT_POWER_LOW_BIT),
};

static int adis16220_probe(struct spi_device *spi)
>>>>>>> refs/remotes/origin/master
{
	int ret;
	struct adis16220_state *st;
	struct iio_dev *indio_dev;

	/* setup the industrialio driver allocated elements */
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
	/* this is only used for removal purposes */
	spi_set_drvdata(spi, indio_dev);

<<<<<<< HEAD
	st->us = spi;
	mutex_init(&st->buf_lock);

=======
>>>>>>> refs/remotes/origin/master
	indio_dev->name = spi->dev.driver->name;
	indio_dev->dev.parent = &spi->dev;
	indio_dev->info = &adis16220_info;
	indio_dev->modes = INDIO_DIRECT_MODE;
	indio_dev->channels = adis16220_channels;
	indio_dev->num_channels = ARRAY_SIZE(adis16220_channels);

<<<<<<< HEAD
	ret = iio_device_register(indio_dev);
	if (ret)
		goto error_free_dev;

	ret = sysfs_create_bin_file(&indio_dev->dev.kobj, &accel_bin);
	if (ret)
		goto error_unregister_dev;
=======
	ret = devm_iio_device_register(&spi->dev, indio_dev);
	if (ret)
		return ret;

	ret = sysfs_create_bin_file(&indio_dev->dev.kobj, &accel_bin);
	if (ret)
		return ret;
>>>>>>> refs/remotes/origin/master

	ret = sysfs_create_bin_file(&indio_dev->dev.kobj, &adc1_bin);
	if (ret)
		goto error_rm_accel_bin;

	ret = sysfs_create_bin_file(&indio_dev->dev.kobj, &adc2_bin);
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret)
		goto error_rm_adc1_bin;

	/* Get the device into a sane initial state */
<<<<<<< HEAD
	ret = adis16220_initial_setup(st);
=======
	ret = adis16220_initial_setup(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	if (ret)
		goto error_rm_adc1_bin;

	ret = adis_init(&st->adis, indio_dev, spi, &adis16220_data);
	if (ret)
		goto error_rm_adc2_bin;
	/* Get the device into a sane initial state */
	ret = adis_initial_startup(&st->adis);
>>>>>>> refs/remotes/origin/master
	if (ret)
		goto error_rm_adc2_bin;
	return 0;

error_rm_adc2_bin:
<<<<<<< HEAD
<<<<<<< HEAD
	sysfs_remove_bin_file(&st->indio_dev->dev.kobj, &adc2_bin);
error_rm_adc1_bin:
	sysfs_remove_bin_file(&st->indio_dev->dev.kobj, &adc1_bin);
error_rm_accel_bin:
	sysfs_remove_bin_file(&st->indio_dev->dev.kobj, &accel_bin);
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
=======
>>>>>>> refs/remotes/origin/master
	sysfs_remove_bin_file(&indio_dev->dev.kobj, &adc2_bin);
error_rm_adc1_bin:
	sysfs_remove_bin_file(&indio_dev->dev.kobj, &adc1_bin);
error_rm_accel_bin:
	sysfs_remove_bin_file(&indio_dev->dev.kobj, &accel_bin);
<<<<<<< HEAD
error_unregister_dev:
	iio_device_unregister(indio_dev);
error_free_dev:
	iio_free_device(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
error_ret:
=======
>>>>>>> refs/remotes/origin/master
	return ret;
}

static int adis16220_remove(struct spi_device *spi)
{
<<<<<<< HEAD
<<<<<<< HEAD
	struct adis16220_state *st = spi_get_drvdata(spi);
	struct iio_dev *indio_dev = st->indio_dev;

	flush_scheduled_work();

	sysfs_remove_bin_file(&st->indio_dev->dev.kobj, &adc2_bin);
	sysfs_remove_bin_file(&st->indio_dev->dev.kobj, &adc1_bin);
	sysfs_remove_bin_file(&st->indio_dev->dev.kobj, &accel_bin);
	iio_device_unregister(indio_dev);
	kfree(st->tx);
	kfree(st->rx);
	kfree(st);
=======
	struct iio_dev *indio_dev = spi_get_drvdata(spi);

	flush_scheduled_work();

	sysfs_remove_bin_file(&indio_dev->dev.kobj, &adc2_bin);
	sysfs_remove_bin_file(&indio_dev->dev.kobj, &adc1_bin);
	sysfs_remove_bin_file(&indio_dev->dev.kobj, &accel_bin);
	iio_device_unregister(indio_dev);
	iio_free_device(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct iio_dev *indio_dev = spi_get_drvdata(spi);

	sysfs_remove_bin_file(&indio_dev->dev.kobj, &adc2_bin);
	sysfs_remove_bin_file(&indio_dev->dev.kobj, &adc1_bin);
	sysfs_remove_bin_file(&indio_dev->dev.kobj, &accel_bin);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static struct spi_driver adis16220_driver = {
	.driver = {
		.name = "adis16220",
		.owner = THIS_MODULE,
	},
	.probe = adis16220_probe,
<<<<<<< HEAD
	.remove = __devexit_p(adis16220_remove),
};
<<<<<<< HEAD

static __init int adis16220_init(void)
{
	return spi_register_driver(&adis16220_driver);
}
module_init(adis16220_init);

static __exit void adis16220_exit(void)
{
	spi_unregister_driver(&adis16220_driver);
}
module_exit(adis16220_exit);
=======
module_spi_driver(adis16220_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	.remove = adis16220_remove,
};
module_spi_driver(adis16220_driver);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Barry Song <21cnbao@gmail.com>");
MODULE_DESCRIPTION("Analog Devices ADIS16220 Digital Vibration Sensor");
MODULE_LICENSE("GPL v2");
<<<<<<< HEAD
<<<<<<< HEAD
=======
MODULE_ALIAS("spi:adis16220");
>>>>>>> refs/remotes/origin/cm-10.0
=======
MODULE_ALIAS("spi:adis16220");
>>>>>>> refs/remotes/origin/master
