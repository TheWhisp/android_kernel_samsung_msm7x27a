/*  Copyright (C) 2010 Texas Instruments
    Author: Shubhrajyoti Datta <shubhrajyoti@ti.com>
<<<<<<< HEAD
    Acknowledgement: Jonathan Cameron <jic23@cam.ac.uk> for valuable inputs.
=======
    Acknowledgement: Jonathan Cameron <jic23@kernel.org> for valuable inputs.

    Support for HMC5883 and HMC5883L by Peter Meerwald <pmeerw@pmeerw.net>.
>>>>>>> refs/remotes/origin/master

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <linux/module.h>
<<<<<<< HEAD
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/types.h>
#include "../iio.h"
#include "../sysfs.h"
<<<<<<< HEAD
#include "magnet.h"
=======
>>>>>>> refs/remotes/origin/cm-10.0

#define HMC5843_I2C_ADDRESS			0x1E
=======
#include <linux/i2c.h>
#include <linux/iio/iio.h>
#include <linux/iio/sysfs.h>
#include <linux/iio/trigger_consumer.h>
#include <linux/iio/buffer.h>
#include <linux/iio/triggered_buffer.h>
#include <linux/delay.h>
>>>>>>> refs/remotes/origin/master

#define HMC5843_CONFIG_REG_A			0x00
#define HMC5843_CONFIG_REG_B			0x01
#define HMC5843_MODE_REG			0x02
<<<<<<< HEAD
#define HMC5843_DATA_OUT_X_MSB_REG		0x03
#define HMC5843_DATA_OUT_X_LSB_REG		0x04
#define HMC5843_DATA_OUT_Y_MSB_REG		0x05
#define HMC5843_DATA_OUT_Y_LSB_REG		0x06
#define HMC5843_DATA_OUT_Z_MSB_REG		0x07
#define HMC5843_DATA_OUT_Z_LSB_REG		0x08
#define HMC5843_STATUS_REG			0x09
#define HMC5843_ID_REG_A			0x0A
#define HMC5843_ID_REG_B			0x0B
#define HMC5843_ID_REG_C			0x0C

#define HMC5843_ID_REG_LENGTH			0x03
#define HMC5843_ID_STRING			"H43"

/*
 * Range settings in  (+-)Ga
 * */
#define RANGE_GAIN_OFFSET			0x05

#define	RANGE_0_7				0x00
#define	RANGE_1_0				0x01 /* default */
#define	RANGE_1_5				0x02
#define	RANGE_2_0				0x03
#define	RANGE_3_2				0x04
#define	RANGE_3_8				0x05
#define	RANGE_4_5				0x06
#define	RANGE_6_5				0x07 /* Not recommended */

/*
 * Device status
 */
<<<<<<< HEAD
#define	DATA_READY  				0x01
#define	DATA_OUTPUT_LOCK  			0x02
#define	VOLTAGE_REGULATOR_ENABLED  		0x04
=======
#define	DATA_READY				0x01
#define	DATA_OUTPUT_LOCK			0x02
#define	VOLTAGE_REGULATOR_ENABLED		0x04
>>>>>>> refs/remotes/origin/cm-10.0

/*
 * Mode register configuration
 */
#define	MODE_CONVERSION_CONTINUOUS		0x00
#define	MODE_CONVERSION_SINGLE			0x01
#define	MODE_IDLE				0x02
#define	MODE_SLEEP				0x03

/* Minimum Data Output Rate in 1/10 Hz  */
#define RATE_OFFSET				0x02
#define RATE_BITMASK				0x1C
#define	RATE_5					0x00
#define	RATE_10					0x01
#define	RATE_20					0x02
#define	RATE_50					0x03
#define	RATE_100				0x04
#define	RATE_200				0x05
#define	RATE_500				0x06
#define	RATE_NOT_USED				0x07

/*
<<<<<<< HEAD
 * Device Configutration
 */
#define	CONF_NORMAL  				0x00
=======
 * Device Configuration
 */
#define	CONF_NORMAL				0x00
>>>>>>> refs/remotes/origin/cm-10.0
#define	CONF_POSITIVE_BIAS			0x01
#define	CONF_NEGATIVE_BIAS			0x02
#define	CONF_NOT_USED				0x03
#define	MEAS_CONF_MASK				0x03

<<<<<<< HEAD
static const char *regval_to_scale[] = {
	"0.0000006173",
	"0.0000007692",
	"0.0000010309",
	"0.0000012821",
	"0.0000018868",
	"0.0000021739",
	"0.0000025641",
	"0.0000035714",
};
=======
static int hmc5843_regval_to_nanoscale[] = {
	6173, 7692, 10309, 12821, 18868, 21739, 25641, 35714
};

>>>>>>> refs/remotes/origin/cm-10.0
static const int regval_to_input_field_mg[] = {
	700,
	1000,
	1500,
	2000,
	3200,
	3800,
	4500,
	6500
};
<<<<<<< HEAD
static const char *regval_to_samp_freq[] = {
=======
static const char * const regval_to_samp_freq[] = {
>>>>>>> refs/remotes/origin/cm-10.0
	"0.5",
	"1",
	"2",
	"5",
	"10",
	"20",
	"50",
};

/* Addresses to scan: 0x1E */
static const unsigned short normal_i2c[] = { HMC5843_I2C_ADDRESS,
							I2C_CLIENT_END };

/* Each client has this additional data */
struct hmc5843_data {
<<<<<<< HEAD
	struct iio_dev	*indio_dev;
=======
>>>>>>> refs/remotes/origin/cm-10.0
	struct mutex lock;
	u8		rate;
	u8		meas_conf;
	u8		operating_mode;
	u8		range;
};

static void hmc5843_init_client(struct i2c_client *client);

static s32 hmc5843_configure(struct i2c_client *client,
				       u8 operating_mode)
{
	/* The lower two bits contain the current conversion mode */
	return i2c_smbus_write_byte_data(client,
					HMC5843_MODE_REG,
					(operating_mode & 0x03));
}

<<<<<<< HEAD
/* Return the measurement value from the  specified channel */
static ssize_t hmc5843_read_measurement(struct device *dev,
		struct device_attribute *attr,
		char *buf)
{
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct i2c_client *client = to_i2c_client(indio_dev->dev.parent);
	s16 coordinate_val;
	struct iio_dev_attr *this_attr = to_iio_dev_attr(attr);
	struct hmc5843_data *data = indio_dev->dev_data;
	s32 result;

	mutex_lock(&data->lock);

=======
/* Return the measurement value from the specified channel */
static int hmc5843_read_measurement(struct iio_dev *indio_dev,
				    int address,
				    int *val)
{
	struct i2c_client *client = to_i2c_client(indio_dev->dev.parent);
	struct hmc5843_data *data = iio_priv(indio_dev);
	s32 result;

	mutex_lock(&data->lock);
>>>>>>> refs/remotes/origin/cm-10.0
	result = i2c_smbus_read_byte_data(client, HMC5843_STATUS_REG);
	while (!(result & DATA_READY))
		result = i2c_smbus_read_byte_data(client, HMC5843_STATUS_REG);

<<<<<<< HEAD
	result = i2c_smbus_read_word_data(client, this_attr->address);
=======
	result = i2c_smbus_read_word_data(client, address);
>>>>>>> refs/remotes/origin/cm-10.0
=======
#define HMC5843_DATA_OUT_MSB_REGS		0x03
#define HMC5843_STATUS_REG			0x09
#define HMC5843_ID_REG				0x0a

enum hmc5843_ids {
	HMC5843_ID,
	HMC5883_ID,
	HMC5883L_ID,
};

/*
 * Range gain settings in (+-)Ga
 * Beware: HMC5843 and HMC5883 have different recommended sensor field
 * ranges; default corresponds to +-1.0 Ga and +-1.3 Ga, respectively
 */
#define HMC5843_RANGE_GAIN_OFFSET		0x05
#define HMC5843_RANGE_GAIN_DEFAULT		0x01
#define HMC5843_RANGE_GAINS			8

/* Device status */
#define HMC5843_DATA_READY			0x01
#define HMC5843_DATA_OUTPUT_LOCK		0x02

/* Mode register configuration */
#define HMC5843_MODE_CONVERSION_CONTINUOUS	0x00
#define HMC5843_MODE_CONVERSION_SINGLE		0x01
#define HMC5843_MODE_IDLE			0x02
#define HMC5843_MODE_SLEEP			0x03
#define HMC5843_MODE_MASK			0x03

/*
 * HMC5843: Minimum data output rate
 * HMC5883: Typical data output rate
 */
#define HMC5843_RATE_OFFSET			0x02
#define HMC5843_RATE_DEFAULT			0x04
#define HMC5843_RATES				7

/* Device measurement configuration */
#define HMC5843_MEAS_CONF_NORMAL		0x00
#define HMC5843_MEAS_CONF_POSITIVE_BIAS		0x01
#define HMC5843_MEAS_CONF_NEGATIVE_BIAS		0x02
#define HMC5843_MEAS_CONF_MASK			0x03

/* Scaling factors: 10000000/Gain */
static const int hmc5843_regval_to_nanoscale[HMC5843_RANGE_GAINS] = {
	6173, 7692, 10309, 12821, 18868, 21739, 25641, 35714
};

static const int hmc5883_regval_to_nanoscale[HMC5843_RANGE_GAINS] = {
	7812, 9766, 13021, 16287, 24096, 27701, 32573, 45662
};

static const int hmc5883l_regval_to_nanoscale[HMC5843_RANGE_GAINS] = {
	7299, 9174, 12195, 15152, 22727, 25641, 30303, 43478
};

/*
 * From the datasheet:
 * Value	| HMC5843		| HMC5883/HMC5883L
 *		| Data output rate (Hz)	| Data output rate (Hz)
 * 0		| 0.5			| 0.75
 * 1		| 1			| 1.5
 * 2		| 2			| 3
 * 3		| 5			| 7.5
 * 4		| 10 (default)		| 15
 * 5		| 20			| 30
 * 6		| 50			| 75
 * 7		| Not used		| Not used
 */
static const int hmc5843_regval_to_samp_freq[7][2] = {
	{0, 500000}, {1, 0}, {2, 0}, {5, 0}, {10, 0}, {20, 0}, {50, 0}
};

static const int hmc5883_regval_to_samp_freq[7][2] = {
	{0, 750000}, {1, 500000}, {3, 0}, {7, 500000}, {15, 0}, {30, 0},
	{75, 0}
};

/* Describe chip variants */
struct hmc5843_chip_info {
	const struct iio_chan_spec *channels;
	const int (*regval_to_samp_freq)[2];
	const int *regval_to_nanoscale;
};

/* Each client has this additional data */
struct hmc5843_data {
	struct i2c_client *client;
	struct mutex lock;
	u8 rate;
	u8 meas_conf;
	u8 operating_mode;
	u8 range;
	const struct hmc5843_chip_info *variant;
	__be16 buffer[8]; /* 3x 16-bit channels + padding + 64-bit timestamp */
};

/* The lower two bits contain the current conversion mode */
static s32 hmc5843_set_mode(struct hmc5843_data *data, u8 operating_mode)
{
	int ret;

	mutex_lock(&data->lock);
	ret = i2c_smbus_write_byte_data(data->client, HMC5843_MODE_REG,
					operating_mode & HMC5843_MODE_MASK);
	if (ret >= 0)
		data->operating_mode = operating_mode;
	mutex_unlock(&data->lock);

	return ret;
}

static int hmc5843_wait_measurement(struct hmc5843_data *data)
{
	s32 result;
	int tries = 150;

	while (tries-- > 0) {
		result = i2c_smbus_read_byte_data(data->client,
			HMC5843_STATUS_REG);
		if (result < 0)
			return result;
		if (result & HMC5843_DATA_READY)
			break;
		msleep(20);
	}

	if (tries < 0) {
		dev_err(&data->client->dev, "data not ready\n");
		return -EIO;
	}

	return 0;
}

/* Return the measurement value from the specified channel */
static int hmc5843_read_measurement(struct hmc5843_data *data,
				    int idx, int *val)
{
	s32 result;
	__be16 values[3];

	mutex_lock(&data->lock);
	result = hmc5843_wait_measurement(data);
	if (result < 0) {
		mutex_unlock(&data->lock);
		return result;
	}
	result = i2c_smbus_read_i2c_block_data(data->client,
		HMC5843_DATA_OUT_MSB_REGS, sizeof(values), (u8 *) values);
>>>>>>> refs/remotes/origin/master
	mutex_unlock(&data->lock);
	if (result < 0)
		return -EINVAL;

<<<<<<< HEAD
<<<<<<< HEAD
	coordinate_val	= (s16)swab16((u16)result);
	return sprintf(buf, "%d\n", coordinate_val);
}
static IIO_DEV_ATTR_MAGN_X(hmc5843_read_measurement,
		HMC5843_DATA_OUT_X_MSB_REG);
static IIO_DEV_ATTR_MAGN_Y(hmc5843_read_measurement,
		HMC5843_DATA_OUT_Y_MSB_REG);
static IIO_DEV_ATTR_MAGN_Z(hmc5843_read_measurement,
		HMC5843_DATA_OUT_Z_MSB_REG);
=======
	*val	= (s16)swab16((u16)result);
	return IIO_VAL_INT;
}

>>>>>>> refs/remotes/origin/cm-10.0

/*
 * From the datasheet
 * 0 - Continuous-Conversion Mode: In continuous-conversion mode, the
<<<<<<< HEAD
 * device continuously performs conversions an places the result in the
=======
 * device continuously performs conversions and places the result in the
>>>>>>> refs/remotes/origin/cm-10.0
 * data register.
 *
 * 1 - Single-Conversion Mode : device performs a single measurement,
 *  sets RDY high and returned to sleep mode
 *
 * 2 - Idle Mode :  Device is placed in idle mode.
 *
 * 3 - Sleep Mode. Device is placed in sleep mode.
 *
 */
static ssize_t hmc5843_show_operating_mode(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct hmc5843_data *data = indio_dev->dev_data;
=======
	struct hmc5843_data *data = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
	return sprintf(buf, "%d\n", data->operating_mode);
}

static ssize_t hmc5843_set_operating_mode(struct device *dev,
				struct device_attribute *attr,
				const char *buf,
				size_t count)
{
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct i2c_client *client = to_i2c_client(indio_dev->dev.parent);
<<<<<<< HEAD
	struct hmc5843_data *data = indio_dev->dev_data;
=======
	struct hmc5843_data *data = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
	struct iio_dev_attr *this_attr = to_iio_dev_attr(attr);
	unsigned long operating_mode = 0;
	s32 status;
	int error;
	mutex_lock(&data->lock);
	error = strict_strtoul(buf, 10, &operating_mode);
	if (error) {
		count = error;
		goto exit;
	}
	dev_dbg(dev, "set Conversion mode to %lu\n", operating_mode);
	if (operating_mode > MODE_SLEEP) {
		count = -EINVAL;
		goto exit;
	}

	status = i2c_smbus_write_byte_data(client, this_attr->address,
					operating_mode);
	if (status) {
		count = -EINVAL;
		goto exit;
	}
	data->operating_mode = operating_mode;

exit:
	mutex_unlock(&data->lock);
	return count;
}
static IIO_DEVICE_ATTR(operating_mode,
			S_IWUSR | S_IRUGO,
			hmc5843_show_operating_mode,
			hmc5843_set_operating_mode,
			HMC5843_MODE_REG);

/*
 * API for setting the measurement configuration to
 * Normal, Positive bias and Negative bias
 * From the datasheet
 *
 * Normal measurement configuration (default): In normal measurement
 * configuration the device follows normal measurement flow. Pins BP and BN
 * are left floating and high impedance.
 *
 * Positive bias configuration: In positive bias configuration, a positive
 * current is forced across the resistive load on pins BP and BN.
 *
 * Negative bias configuration. In negative bias configuration, a negative
 * current is forced across the resistive load on pins BP and BN.
 *
 */
static s32 hmc5843_set_meas_conf(struct i2c_client *client,
				      u8 meas_conf)
{
	struct hmc5843_data *data = i2c_get_clientdata(client);
	u8 reg_val;
	reg_val = (meas_conf & MEAS_CONF_MASK) |  (data->rate << RATE_OFFSET);
	return i2c_smbus_write_byte_data(client, HMC5843_CONFIG_REG_A, reg_val);
=======
	*val = sign_extend32(be16_to_cpu(values[idx]), 15);
	return IIO_VAL_INT;
}

/*
 * API for setting the measurement configuration to
 * Normal, Positive bias and Negative bias
 *
 * From the datasheet:
 * 0 - Normal measurement configuration (default): In normal measurement
 *     configuration the device follows normal measurement flow. Pins BP
 *     and BN are left floating and high impedance.
 *
 * 1 - Positive bias configuration: In positive bias configuration, a
 *     positive current is forced across the resistive load on pins BP
 *     and BN.
 *
 * 2 - Negative bias configuration. In negative bias configuration, a
 *     negative current is forced across the resistive load on pins BP
 *     and BN.
 *
 */
static s32 hmc5843_set_meas_conf(struct hmc5843_data *data, u8 meas_conf)
{
	int ret;

	mutex_lock(&data->lock);
	ret = i2c_smbus_write_byte_data(data->client, HMC5843_CONFIG_REG_A,
		(meas_conf & HMC5843_MEAS_CONF_MASK) |
		(data->rate << HMC5843_RATE_OFFSET));
	if (ret >= 0)
		data->meas_conf = meas_conf;
	mutex_unlock(&data->lock);

	return ret;
>>>>>>> refs/remotes/origin/master
}

static ssize_t hmc5843_show_measurement_configuration(struct device *dev,
						struct device_attribute *attr,
						char *buf)
{
<<<<<<< HEAD
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct hmc5843_data *data = indio_dev->dev_data;
=======
	struct hmc5843_data *data = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct hmc5843_data *data = iio_priv(dev_to_iio_dev(dev));
>>>>>>> refs/remotes/origin/master
	return sprintf(buf, "%d\n", data->meas_conf);
}

static ssize_t hmc5843_set_measurement_configuration(struct device *dev,
						struct device_attribute *attr,
						const char *buf,
						size_t count)
{
<<<<<<< HEAD
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct i2c_client *client = to_i2c_client(indio_dev->dev.parent);
	struct hmc5843_data *data = i2c_get_clientdata(client);
	unsigned long meas_conf = 0;
	int error = strict_strtoul(buf, 10, &meas_conf);
	if (error)
		return error;
	mutex_lock(&data->lock);

	dev_dbg(dev, "set mode to %lu\n", meas_conf);
	if (hmc5843_set_meas_conf(client, meas_conf)) {
		count = -EINVAL;
		goto exit;
	}
	data->meas_conf = meas_conf;

exit:
	mutex_unlock(&data->lock);
	return count;
}
=======
	struct hmc5843_data *data = iio_priv(dev_to_iio_dev(dev));
	unsigned long meas_conf = 0;
	int ret;

	ret = kstrtoul(buf, 10, &meas_conf);
	if (ret)
		return ret;
	if (meas_conf >= HMC5843_MEAS_CONF_MASK)
		return -EINVAL;

	ret = hmc5843_set_meas_conf(data, meas_conf);

	return (ret < 0) ? ret : count;
}

>>>>>>> refs/remotes/origin/master
static IIO_DEVICE_ATTR(meas_conf,
			S_IWUSR | S_IRUGO,
			hmc5843_show_measurement_configuration,
			hmc5843_set_measurement_configuration,
			0);

<<<<<<< HEAD
/*
 * From Datasheet
 * The table shows the minimum data output
 * Value	| Minimum data output rate(Hz)
 * 0		| 0.5
 * 1		| 1
 * 2		| 2
 * 3		| 5
 * 4		| 10 (default)
 * 5		| 20
 * 6		| 50
 * 7		| Not used
 */
static IIO_CONST_ATTR_SAMP_FREQ_AVAIL("0.5 1 2 5 10 20 50");

static s32 hmc5843_set_rate(struct i2c_client *client,
				u8 rate)
{
	struct hmc5843_data *data = i2c_get_clientdata(client);
	u8 reg_val;

	reg_val = (data->meas_conf) |  (rate << RATE_OFFSET);
	if (rate >= RATE_NOT_USED) {
		dev_err(&client->dev,
<<<<<<< HEAD
			"This data output rate is not supported \n");
=======
			"This data output rate is not supported\n");
>>>>>>> refs/remotes/origin/cm-10.0
		return -EINVAL;
	}
	return i2c_smbus_write_byte_data(client, HMC5843_CONFIG_REG_A, reg_val);
}

static ssize_t set_sampling_frequency(struct device *dev,
					struct device_attribute *attr,
					const char *buf, size_t count)
{

	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct i2c_client *client = to_i2c_client(indio_dev->dev.parent);
<<<<<<< HEAD
	struct hmc5843_data *data = indio_dev->dev_data;
=======
	struct hmc5843_data *data = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned long rate = 0;

	if (strncmp(buf, "0.5" , 3) == 0)
		rate = RATE_5;
	else if (strncmp(buf, "1" , 1) == 0)
		rate = RATE_10;
	else if (strncmp(buf, "2", 1) == 0)
		rate = RATE_20;
	else if (strncmp(buf, "5", 1) == 0)
		rate = RATE_50;
	else if (strncmp(buf, "10", 2) == 0)
		rate = RATE_100;
	else if (strncmp(buf, "20" , 2) == 0)
		rate = RATE_200;
	else if (strncmp(buf, "50" , 2) == 0)
		rate = RATE_500;
	else
		return -EINVAL;

	mutex_lock(&data->lock);
	dev_dbg(dev, "set rate to %lu\n", rate);
	if (hmc5843_set_rate(client, rate)) {
		count = -EINVAL;
		goto exit;
	}
	data->rate = rate;

exit:
	mutex_unlock(&data->lock);
	return count;
}

static ssize_t show_sampling_frequency(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct i2c_client *client = to_i2c_client(indio_dev->dev.parent);
	struct iio_dev_attr *this_attr = to_iio_dev_attr(attr);
	s32 rate;

	rate = i2c_smbus_read_byte_data(client,  this_attr->address);
	if (rate < 0)
		return rate;
	rate = (rate & RATE_BITMASK) >> RATE_OFFSET;
	return sprintf(buf, "%s\n", regval_to_samp_freq[rate]);
}
static IIO_DEVICE_ATTR(sampling_frequency,
			S_IWUSR | S_IRUGO,
			show_sampling_frequency,
			set_sampling_frequency,
			HMC5843_CONFIG_REG_A);

/*
 * From Datasheet
 *	Nominal gain settings
 * Value	| Sensor Input Field Range(Ga)	| Gain(counts/ milli-gauss)
 *0		|(+-)0.7			|1620
 *1		|(+-)1.0			|1300
 *2		|(+-)1.5			|970
 *3		|(+-)2.0			|780
 *4		|(+-)3.2			|530
 *5		|(+-)3.8			|460
 *6		|(+-)4.5			|390
 *7		|(+-)6.5			|280
 */
static ssize_t show_range(struct device *dev,
				struct device_attribute *attr,
				char *buf)
{
	u8 range;
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct hmc5843_data *data = indio_dev->dev_data;
=======
	struct hmc5843_data *data = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0

	range = data->range;
	return sprintf(buf, "%d\n", regval_to_input_field_mg[range]);
}

static ssize_t set_range(struct device *dev,
			struct device_attribute *attr,
			const char *buf,
			size_t count)
{
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct i2c_client *client = to_i2c_client(indio_dev->dev.parent);
	struct iio_dev_attr *this_attr = to_iio_dev_attr(attr);
<<<<<<< HEAD
	struct hmc5843_data *data = indio_dev->dev_data;
=======
	struct hmc5843_data *data = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned long range = 0;
	int error;
	mutex_lock(&data->lock);
	error = strict_strtoul(buf, 10, &range);
	if (error) {
		count = error;
		goto exit;
	}
	dev_dbg(dev, "set range to %lu\n", range);

	if (range > RANGE_6_5) {
		count = -EINVAL;
		goto exit;
	}

	data->range = range;
	range = range << RANGE_GAIN_OFFSET;
	if (i2c_smbus_write_byte_data(client, this_attr->address, range))
		count = -EINVAL;

exit:
	mutex_unlock(&data->lock);
	return count;

}
<<<<<<< HEAD
static IIO_DEVICE_ATTR(magn_range,
=======
static IIO_DEVICE_ATTR(in_magn_range,
>>>>>>> refs/remotes/origin/cm-10.0
			S_IWUSR | S_IRUGO,
			show_range,
			set_range,
			HMC5843_CONFIG_REG_B);

<<<<<<< HEAD
static ssize_t show_scale(struct device *dev,
			struct device_attribute *attr,
			char *buf)
{
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct hmc5843_data *data = indio_dev->dev_data;
	return strlen(strcpy(buf, regval_to_scale[data->range]));
}
static IIO_DEVICE_ATTR(magn_scale,
			S_IRUGO,
			show_scale,
			NULL , 0);
=======
static int hmc5843_read_raw(struct iio_dev *indio_dev,
			    struct iio_chan_spec const *chan,
			    int *val, int *val2,
			    long mask)
=======
static ssize_t hmc5843_show_samp_freq_avail(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct hmc5843_data *data = iio_priv(dev_to_iio_dev(dev));
	size_t len = 0;
	int i;

	for (i = 0; i < HMC5843_RATES; i++)
		len += scnprintf(buf + len, PAGE_SIZE - len,
			"%d.%d ", data->variant->regval_to_samp_freq[i][0],
			data->variant->regval_to_samp_freq[i][1]);

	/* replace trailing space by newline */
	buf[len - 1] = '\n';

	return len;
}

static IIO_DEV_ATTR_SAMP_FREQ_AVAIL(hmc5843_show_samp_freq_avail);

static int hmc5843_set_samp_freq(struct hmc5843_data *data, u8 rate)
{
	int ret;

	mutex_lock(&data->lock);
	ret = i2c_smbus_write_byte_data(data->client, HMC5843_CONFIG_REG_A,
		data->meas_conf | (rate << HMC5843_RATE_OFFSET));
	if (ret >= 0)
		data->rate = rate;
	mutex_unlock(&data->lock);

	return ret;
}

static int hmc5843_get_samp_freq_index(struct hmc5843_data *data,
				   int val, int val2)
{
	int i;

	for (i = 0; i < HMC5843_RATES; i++)
		if (val == data->variant->regval_to_samp_freq[i][0] &&
			val2 == data->variant->regval_to_samp_freq[i][1])
			return i;

	return -EINVAL;
}

static int hmc5843_set_range_gain(struct hmc5843_data *data, u8 range)
{
	int ret;

	mutex_lock(&data->lock);
	ret = i2c_smbus_write_byte_data(data->client, HMC5843_CONFIG_REG_B,
		range << HMC5843_RANGE_GAIN_OFFSET);
	if (ret >= 0)
		data->range = range;
	mutex_unlock(&data->lock);

	return ret;
}

static ssize_t hmc5843_show_scale_avail(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct hmc5843_data *data = iio_priv(dev_to_iio_dev(dev));

	size_t len = 0;
	int i;

	for (i = 0; i < HMC5843_RANGE_GAINS; i++)
		len += scnprintf(buf + len, PAGE_SIZE - len,
			"0.%09d ", data->variant->regval_to_nanoscale[i]);

	/* replace trailing space by newline */
	buf[len - 1] = '\n';

	return len;
}

static IIO_DEVICE_ATTR(scale_available, S_IRUGO,
	hmc5843_show_scale_avail, NULL, 0);

static int hmc5843_get_scale_index(struct hmc5843_data *data, int val, int val2)
{
	int i;

	if (val != 0)
		return -EINVAL;

	for (i = 0; i < HMC5843_RANGE_GAINS; i++)
		if (val2 == data->variant->regval_to_nanoscale[i])
			return i;

	return -EINVAL;
}

static int hmc5843_read_raw(struct iio_dev *indio_dev,
			    struct iio_chan_spec const *chan,
			    int *val, int *val2, long mask)
>>>>>>> refs/remotes/origin/master
{
	struct hmc5843_data *data = iio_priv(indio_dev);

	switch (mask) {
<<<<<<< HEAD
	case 0:
		return hmc5843_read_measurement(indio_dev,
						chan->address,
						val);
	case IIO_CHAN_INFO_SCALE:
		*val = 0;
		*val2 = hmc5843_regval_to_nanoscale[data->range];
		return IIO_VAL_INT_PLUS_NANO;
	};
	return -EINVAL;
}

#define HMC5843_CHANNEL(axis, add)					\
=======
	case IIO_CHAN_INFO_RAW:
		return hmc5843_read_measurement(data, chan->scan_index, val);
	case IIO_CHAN_INFO_SCALE:
		*val = 0;
		*val2 = data->variant->regval_to_nanoscale[data->range];
		return IIO_VAL_INT_PLUS_NANO;
	case IIO_CHAN_INFO_SAMP_FREQ:
		*val = data->variant->regval_to_samp_freq[data->rate][0];
		*val2 = data->variant->regval_to_samp_freq[data->rate][1];
		return IIO_VAL_INT_PLUS_MICRO;
	}
	return -EINVAL;
}

static int hmc5843_write_raw(struct iio_dev *indio_dev,
			     struct iio_chan_spec const *chan,
			     int val, int val2, long mask)
{
	struct hmc5843_data *data = iio_priv(indio_dev);
	int rate, range;

	switch (mask) {
	case IIO_CHAN_INFO_SAMP_FREQ:
		rate = hmc5843_get_samp_freq_index(data, val, val2);
		if (rate < 0)
			return -EINVAL;

		return hmc5843_set_samp_freq(data, rate);
	case IIO_CHAN_INFO_SCALE:
		range = hmc5843_get_scale_index(data, val, val2);
		if (range < 0)
			return -EINVAL;

		return hmc5843_set_range_gain(data, range);
	default:
		return -EINVAL;
	}
}

static int hmc5843_write_raw_get_fmt(struct iio_dev *indio_dev,
			       struct iio_chan_spec const *chan, long mask)
{
	switch (mask) {
	case IIO_CHAN_INFO_SAMP_FREQ:
		return IIO_VAL_INT_PLUS_MICRO;
	case IIO_CHAN_INFO_SCALE:
		return IIO_VAL_INT_PLUS_NANO;
	default:
		return -EINVAL;
	}
}

static irqreturn_t hmc5843_trigger_handler(int irq, void *p)
{
	struct iio_poll_func *pf = p;
	struct iio_dev *indio_dev = pf->indio_dev;
	struct hmc5843_data *data = iio_priv(indio_dev);
	int ret;

	mutex_lock(&data->lock);
	ret = hmc5843_wait_measurement(data);
	if (ret < 0) {
		mutex_unlock(&data->lock);
		goto done;
	}

	ret = i2c_smbus_read_i2c_block_data(data->client,
		HMC5843_DATA_OUT_MSB_REGS, 3 * sizeof(__be16),
			(u8 *) data->buffer);
	mutex_unlock(&data->lock);
	if (ret < 0)
		goto done;

	iio_push_to_buffers_with_timestamp(indio_dev, data->buffer,
		iio_get_time_ns());

done:
	iio_trigger_notify_done(indio_dev->trig);

	return IRQ_HANDLED;
}

#define HMC5843_CHANNEL(axis, idx)					\
>>>>>>> refs/remotes/origin/master
	{								\
		.type = IIO_MAGN,					\
		.modified = 1,						\
		.channel2 = IIO_MOD_##axis,				\
<<<<<<< HEAD
		.info_mask = IIO_CHAN_INFO_SCALE_SHARED_BIT,		\
		.address = add						\
	}

static const struct iio_chan_spec hmc5843_channels[] = {
	HMC5843_CHANNEL(X, HMC5843_DATA_OUT_X_MSB_REG),
	HMC5843_CHANNEL(Y, HMC5843_DATA_OUT_Y_MSB_REG),
	HMC5843_CHANNEL(Z, HMC5843_DATA_OUT_Z_MSB_REG),
};
>>>>>>> refs/remotes/origin/cm-10.0

static struct attribute *hmc5843_attributes[] = {
	&iio_dev_attr_meas_conf.dev_attr.attr,
	&iio_dev_attr_operating_mode.dev_attr.attr,
	&iio_dev_attr_sampling_frequency.dev_attr.attr,
<<<<<<< HEAD
	&iio_dev_attr_magn_range.dev_attr.attr,
	&iio_dev_attr_magn_scale.dev_attr.attr,
	&iio_dev_attr_magn_x_raw.dev_attr.attr,
	&iio_dev_attr_magn_y_raw.dev_attr.attr,
	&iio_dev_attr_magn_z_raw.dev_attr.attr,
=======
	&iio_dev_attr_in_magn_range.dev_attr.attr,
>>>>>>> refs/remotes/origin/cm-10.0
	&iio_const_attr_sampling_frequency_available.dev_attr.attr,
=======
		.info_mask_separate = BIT(IIO_CHAN_INFO_RAW),		\
		.info_mask_shared_by_type = BIT(IIO_CHAN_INFO_SCALE) |	\
			BIT(IIO_CHAN_INFO_SAMP_FREQ),			\
		.scan_index = idx,					\
		.scan_type = {						\
			.sign = 's',					\
			.realbits = 16,					\
			.storagebits = 16,				\
			.endianness = IIO_BE,				\
		},							\
	}

static const struct iio_chan_spec hmc5843_channels[] = {
	HMC5843_CHANNEL(X, 0),
	HMC5843_CHANNEL(Y, 1),
	HMC5843_CHANNEL(Z, 2),
	IIO_CHAN_SOFT_TIMESTAMP(3),
};

/* Beware: Y and Z are exchanged on HMC5883 */
static const struct iio_chan_spec hmc5883_channels[] = {
	HMC5843_CHANNEL(X, 0),
	HMC5843_CHANNEL(Z, 1),
	HMC5843_CHANNEL(Y, 2),
	IIO_CHAN_SOFT_TIMESTAMP(3),
};

static struct attribute *hmc5843_attributes[] = {
	&iio_dev_attr_meas_conf.dev_attr.attr,
	&iio_dev_attr_scale_available.dev_attr.attr,
	&iio_dev_attr_sampling_frequency_available.dev_attr.attr,
>>>>>>> refs/remotes/origin/master
	NULL
};

static const struct attribute_group hmc5843_group = {
	.attrs = hmc5843_attributes,
};

<<<<<<< HEAD
static int hmc5843_detect(struct i2c_client *client,
			  struct i2c_board_info *info)
{
	unsigned char id_str[HMC5843_ID_REG_LENGTH];

	if (client->addr != HMC5843_I2C_ADDRESS)
		return -ENODEV;

	if (i2c_smbus_read_i2c_block_data(client, HMC5843_ID_REG_A,
				HMC5843_ID_REG_LENGTH, id_str)
			!= HMC5843_ID_REG_LENGTH)
		return -ENODEV;

	if (0 != strncmp(id_str, HMC5843_ID_STRING, HMC5843_ID_REG_LENGTH))
		return -ENODEV;

	return 0;
}

/* Called when we have found a new HMC5843. */
static void hmc5843_init_client(struct i2c_client *client)
{
	struct iio_dev *indio_dev = i2c_get_clientdata(client);
	struct hmc5843_data *data = iio_priv(indio_dev);

	hmc5843_set_meas_conf(client, data->meas_conf);
	hmc5843_set_rate(client, data->rate);
	hmc5843_configure(client, data->operating_mode);
	i2c_smbus_write_byte_data(client, HMC5843_CONFIG_REG_B, data->range);
	mutex_init(&data->lock);
	pr_info("HMC5843 initialized\n");
=======
static const struct hmc5843_chip_info hmc5843_chip_info_tbl[] = {
	[HMC5843_ID] = {
		.channels = hmc5843_channels,
		.regval_to_samp_freq = hmc5843_regval_to_samp_freq,
		.regval_to_nanoscale = hmc5843_regval_to_nanoscale,
	},
	[HMC5883_ID] = {
		.channels = hmc5883_channels,
		.regval_to_samp_freq = hmc5883_regval_to_samp_freq,
		.regval_to_nanoscale = hmc5883_regval_to_nanoscale,
	},
	[HMC5883L_ID] = {
		.channels = hmc5883_channels,
		.regval_to_samp_freq = hmc5883_regval_to_samp_freq,
		.regval_to_nanoscale = hmc5883l_regval_to_nanoscale,
	},
};

static int hmc5843_init(struct hmc5843_data *data)
{
	int ret;
	u8 id[3];

	ret = i2c_smbus_read_i2c_block_data(data->client, HMC5843_ID_REG,
		sizeof(id), id);
	if (ret < 0)
		return ret;
	if (id[0] != 'H' || id[1] != '4' || id[2] != '3') {
		dev_err(&data->client->dev, "no HMC5843/5883/5883L sensor\n");
		return -ENODEV;
	}

	ret = hmc5843_set_meas_conf(data, HMC5843_MEAS_CONF_NORMAL);
	if (ret < 0)
		return ret;
	ret = hmc5843_set_samp_freq(data, HMC5843_RATE_DEFAULT);
	if (ret < 0)
		return ret;
	ret = hmc5843_set_range_gain(data, HMC5843_RANGE_GAIN_DEFAULT);
	if (ret < 0)
		return ret;
	return hmc5843_set_mode(data, HMC5843_MODE_CONVERSION_CONTINUOUS);
>>>>>>> refs/remotes/origin/master
}

static const struct iio_info hmc5843_info = {
	.attrs = &hmc5843_group,
<<<<<<< HEAD
<<<<<<< HEAD
=======
	.read_raw = &hmc5843_read_raw,
>>>>>>> refs/remotes/origin/cm-10.0
	.driver_module = THIS_MODULE,
};

=======
	.read_raw = &hmc5843_read_raw,
	.write_raw = &hmc5843_write_raw,
	.write_raw_get_fmt = &hmc5843_write_raw_get_fmt,
	.driver_module = THIS_MODULE,
};

static const unsigned long hmc5843_scan_masks[] = {0x7, 0};

>>>>>>> refs/remotes/origin/master
static int hmc5843_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	struct hmc5843_data *data;
<<<<<<< HEAD
<<<<<<< HEAD
	int err = 0;

	data = kzalloc(sizeof(struct hmc5843_data), GFP_KERNEL);
	if (!data) {
		err = -ENOMEM;
		goto exit;
	}

=======
	struct iio_dev *indio_dev;
	int err = 0;

	indio_dev = iio_allocate_device(sizeof(*data));
	if (indio_dev == NULL) {
		err = -ENOMEM;
		goto exit;
	}
	data = iio_priv(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
	/* default settings at probe */

	data->meas_conf = CONF_NORMAL;
	data->range = RANGE_1_0;
	data->operating_mode = MODE_CONVERSION_CONTINUOUS;

<<<<<<< HEAD
	i2c_set_clientdata(client, data);
=======
	i2c_set_clientdata(client, indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0

	/* Initialize the HMC5843 chip */
	hmc5843_init_client(client);

<<<<<<< HEAD
	data->indio_dev = iio_allocate_device(0);
	if (!data->indio_dev) {
		err = -ENOMEM;
		goto exit_free1;
	}
	data->indio_dev->info = &hmc5843_info;
	data->indio_dev->dev.parent = &client->dev;
	data->indio_dev->dev_data = (void *)(data);
	data->indio_dev->modes = INDIO_DIRECT_MODE;
	err = iio_device_register(data->indio_dev);
=======
	indio_dev->info = &hmc5843_info;
	indio_dev->name = id->name;
	indio_dev->channels = hmc5843_channels;
	indio_dev->num_channels = ARRAY_SIZE(hmc5843_channels);
	indio_dev->dev.parent = &client->dev;
	indio_dev->modes = INDIO_DIRECT_MODE;
	err = iio_device_register(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
	if (err)
		goto exit_free2;
	return 0;
exit_free2:
<<<<<<< HEAD
	iio_free_device(data->indio_dev);
exit_free1:
	kfree(data);
=======
	iio_free_device(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
exit:
	return err;
}

static int hmc5843_remove(struct i2c_client *client)
{
<<<<<<< HEAD
	struct hmc5843_data *data = i2c_get_clientdata(client);
	 /*  sleep mode to save power */
	hmc5843_configure(client, MODE_SLEEP);
	iio_device_unregister(data->indio_dev);
	kfree(i2c_get_clientdata(client));
	return 0;
}

static int hmc5843_suspend(struct i2c_client *client, pm_message_t mesg)
{
	hmc5843_configure(client, MODE_SLEEP);
	return 0;
}

static int hmc5843_resume(struct i2c_client *client)
{
	struct hmc5843_data *data = i2c_get_clientdata(client);
	hmc5843_configure(client, data->operating_mode);
	return 0;
}

=======
	struct iio_dev *indio_dev = i2c_get_clientdata(client);

	iio_device_unregister(indio_dev);
	 /*  sleep mode to save power */
	hmc5843_configure(client, MODE_SLEEP);
	iio_free_device(indio_dev);
=======
	struct iio_dev *indio_dev;
	int ret;

	indio_dev = devm_iio_device_alloc(&client->dev, sizeof(*data));
	if (indio_dev == NULL)
		return -ENOMEM;

	/* default settings at probe */
	data = iio_priv(indio_dev);
	data->client = client;
	data->variant = &hmc5843_chip_info_tbl[id->driver_data];
	mutex_init(&data->lock);

	i2c_set_clientdata(client, indio_dev);
	indio_dev->info = &hmc5843_info;
	indio_dev->name = id->name;
	indio_dev->dev.parent = &client->dev;
	indio_dev->modes = INDIO_DIRECT_MODE;
	indio_dev->channels = data->variant->channels;
	indio_dev->num_channels = 4;
	indio_dev->available_scan_masks = hmc5843_scan_masks;

	ret = hmc5843_init(data);
	if (ret < 0)
		return ret;

	ret = iio_triggered_buffer_setup(indio_dev, NULL,
		hmc5843_trigger_handler, NULL);
	if (ret < 0)
		return ret;

	ret = iio_device_register(indio_dev);
	if (ret < 0)
		goto buffer_cleanup;

	return 0;

buffer_cleanup:
	iio_triggered_buffer_cleanup(indio_dev);
	return ret;
}

static int hmc5843_remove(struct i2c_client *client)
{
	struct iio_dev *indio_dev = i2c_get_clientdata(client);

	iio_device_unregister(indio_dev);
	iio_triggered_buffer_cleanup(indio_dev);

	/*  sleep mode to save power */
	hmc5843_set_mode(iio_priv(indio_dev), HMC5843_MODE_SLEEP);
>>>>>>> refs/remotes/origin/master

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int hmc5843_suspend(struct device *dev)
{
<<<<<<< HEAD
	hmc5843_configure(to_i2c_client(dev), MODE_SLEEP);
	return 0;
=======
	struct hmc5843_data *data = iio_priv(i2c_get_clientdata(
		to_i2c_client(dev)));

	return hmc5843_set_mode(data, HMC5843_MODE_SLEEP);
>>>>>>> refs/remotes/origin/master
}

static int hmc5843_resume(struct device *dev)
{
<<<<<<< HEAD
	struct hmc5843_data *data = i2c_get_clientdata(to_i2c_client(dev));
	hmc5843_configure(to_i2c_client(dev), data->operating_mode);
	return 0;
=======
	struct hmc5843_data *data = iio_priv(i2c_get_clientdata(
		to_i2c_client(dev)));

	return hmc5843_set_mode(data, HMC5843_MODE_CONVERSION_CONTINUOUS);
>>>>>>> refs/remotes/origin/master
}

static SIMPLE_DEV_PM_OPS(hmc5843_pm_ops, hmc5843_suspend, hmc5843_resume);
#define HMC5843_PM_OPS (&hmc5843_pm_ops)
#else
#define HMC5843_PM_OPS NULL
#endif

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static const struct i2c_device_id hmc5843_id[] = {
	{ "hmc5843", 0 },
	{ }
};
<<<<<<< HEAD
=======
MODULE_DEVICE_TABLE(i2c, hmc5843_id);
>>>>>>> refs/remotes/origin/cm-10.0
=======
static const struct i2c_device_id hmc5843_id[] = {
	{ "hmc5843", HMC5843_ID },
	{ "hmc5883", HMC5883_ID },
	{ "hmc5883l", HMC5883L_ID },
	{ }
};
MODULE_DEVICE_TABLE(i2c, hmc5843_id);

static const struct of_device_id hmc5843_of_match[] = {
	{ .compatible = "honeywell,hmc5843" },
	{}
};
MODULE_DEVICE_TABLE(of, hmc5843_of_match);
>>>>>>> refs/remotes/origin/master

static struct i2c_driver hmc5843_driver = {
	.driver = {
		.name	= "hmc5843",
<<<<<<< HEAD
<<<<<<< HEAD
=======
		.pm	= HMC5843_PM_OPS,
>>>>>>> refs/remotes/origin/cm-10.0
=======
		.pm	= HMC5843_PM_OPS,
		.of_match_table = hmc5843_of_match,
>>>>>>> refs/remotes/origin/master
	},
	.id_table	= hmc5843_id,
	.probe		= hmc5843_probe,
	.remove		= hmc5843_remove,
<<<<<<< HEAD
	.detect		= hmc5843_detect,
	.address_list	= normal_i2c,
<<<<<<< HEAD
	.suspend	= hmc5843_suspend,
	.resume		= hmc5843_resume,
};

static int __init hmc5843_init(void)
{
	return i2c_add_driver(&hmc5843_driver);
}

static void __exit hmc5843_exit(void)
{
	i2c_del_driver(&hmc5843_driver);
}
=======
};
module_i2c_driver(hmc5843_driver);
>>>>>>> refs/remotes/origin/cm-10.0

MODULE_AUTHOR("Shubhrajyoti Datta <shubhrajyoti@ti.com");
MODULE_DESCRIPTION("HMC5843 driver");
MODULE_LICENSE("GPL");
<<<<<<< HEAD

module_init(hmc5843_init);
module_exit(hmc5843_exit);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
};
module_i2c_driver(hmc5843_driver);

MODULE_AUTHOR("Shubhrajyoti Datta <shubhrajyoti@ti.com>");
MODULE_DESCRIPTION("HMC5843/5883/5883L driver");
MODULE_LICENSE("GPL");
>>>>>>> refs/remotes/origin/master
