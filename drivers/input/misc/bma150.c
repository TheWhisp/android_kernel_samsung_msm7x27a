<<<<<<< HEAD
/*  Date: 2011/3/7 11:00:00
 *  Revision: 2.11
 */

/*
 * This software program is licensed subject to the GNU General Public License
 * (GPL).Version 2,June 1991, available at http://www.fsf.org/copyleft/gpl.html

 * (C) Copyright 2011 Bosch Sensortec GmbH
 * All Rights Reserved
 */


/* file BMA150.c
   brief This file contains all function implementations for the BMA150 in linux

*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/bma150.h>

#define SENSOR_NAME			"bma150"
#define GRAVITY_EARTH		9806550
#define ABSMIN_2G			(-GRAVITY_EARTH * 2)
#define ABSMAX_2G			(GRAVITY_EARTH * 2)
#define BMA150_MAX_DELAY	200
#define BMA150_CHIP_ID		2
#define BMA150_RANGE_SET	0
#define BMA150_BW_SET		4



#define BMA150_CHIP_ID_REG		0x00
#define BMA150_X_AXIS_LSB_REG	0x02
#define BMA150_X_AXIS_MSB_REG	0x03
#define BMA150_Y_AXIS_LSB_REG	0x04
#define BMA150_Y_AXIS_MSB_REG	0x05
#define BMA150_Z_AXIS_LSB_REG	0x06
#define BMA150_Z_AXIS_MSB_REG	0x07
#define BMA150_STATUS_REG		0x09
#define BMA150_CTRL_REG			0x0a
#define BMA150_CONF1_REG		0x0b

#define BMA150_CUSTOMER1_REG	0x12
#define BMA150_CUSTOMER2_REG	0x13
#define BMA150_RANGE_BWIDTH_REG	0x14
#define BMA150_CONF2_REG		0x15

#define BMA150_OFFS_GAIN_X_REG	0x16
#define BMA150_OFFS_GAIN_Y_REG	0x17
#define BMA150_OFFS_GAIN_Z_REG	0x18
#define BMA150_OFFS_GAIN_T_REG	0x19
#define BMA150_OFFSET_X_REG		0x1a
#define BMA150_OFFSET_Y_REG		0x1b
#define BMA150_OFFSET_Z_REG		0x1c
#define BMA150_OFFSET_T_REG		0x1d

#define BMA150_CHIP_ID__POS		0
#define BMA150_CHIP_ID__MSK		0x07
#define BMA150_CHIP_ID__LEN		3
#define BMA150_CHIP_ID__REG		BMA150_CHIP_ID_REG

/* DATA REGISTERS */

#define BMA150_NEW_DATA_X__POS		0
#define BMA150_NEW_DATA_X__LEN		1
#define BMA150_NEW_DATA_X__MSK		0x01
#define BMA150_NEW_DATA_X__REG		BMA150_X_AXIS_LSB_REG

#define BMA150_ACC_X_LSB__POS		6
#define BMA150_ACC_X_LSB__LEN		2
#define BMA150_ACC_X_LSB__MSK		0xC0
#define BMA150_ACC_X_LSB__REG		BMA150_X_AXIS_LSB_REG

#define BMA150_ACC_X_MSB__POS		0
#define BMA150_ACC_X_MSB__LEN		8
#define BMA150_ACC_X_MSB__MSK		0xFF
#define BMA150_ACC_X_MSB__REG		BMA150_X_AXIS_MSB_REG

#define BMA150_ACC_Y_LSB__POS		6
#define BMA150_ACC_Y_LSB__LEN		2
#define BMA150_ACC_Y_LSB__MSK		0xC0
#define BMA150_ACC_Y_LSB__REG		BMA150_Y_AXIS_LSB_REG

#define BMA150_ACC_Y_MSB__POS		0
#define BMA150_ACC_Y_MSB__LEN		8
#define BMA150_ACC_Y_MSB__MSK		0xFF
#define BMA150_ACC_Y_MSB__REG		BMA150_Y_AXIS_MSB_REG

#define BMA150_ACC_Z_LSB__POS		6
#define BMA150_ACC_Z_LSB__LEN		2
#define BMA150_ACC_Z_LSB__MSK		0xC0
#define BMA150_ACC_Z_LSB__REG		BMA150_Z_AXIS_LSB_REG

#define BMA150_ACC_Z_MSB__POS		0
#define BMA150_ACC_Z_MSB__LEN		8
#define BMA150_ACC_Z_MSB__MSK		0xFF
#define BMA150_ACC_Z_MSB__REG		BMA150_Z_AXIS_MSB_REG

/* CONTROL BITS */

#define BMA150_SLEEP__POS			0
#define BMA150_SLEEP__LEN			1
#define BMA150_SLEEP__MSK			0x01
#define BMA150_SLEEP__REG			BMA150_CTRL_REG

#define BMA150_SOFT_RESET__POS		1
#define BMA150_SOFT_RESET__LEN		1
#define BMA150_SOFT_RESET__MSK		0x02
#define BMA150_SOFT_RESET__REG		BMA150_CTRL_REG

#define BMA150_EE_W__POS			4
#define BMA150_EE_W__LEN			1
#define BMA150_EE_W__MSK			0x10
#define BMA150_EE_W__REG			BMA150_CTRL_REG

#define BMA150_UPDATE_IMAGE__POS	5
#define BMA150_UPDATE_IMAGE__LEN	1
#define BMA150_UPDATE_IMAGE__MSK	0x20
#define BMA150_UPDATE_IMAGE__REG	BMA150_CTRL_REG

#define BMA150_RESET_INT__POS		6
#define BMA150_RESET_INT__LEN		1
#define BMA150_RESET_INT__MSK		0x40
#define BMA150_RESET_INT__REG		BMA150_CTRL_REG

/* BANDWIDTH dependend definitions */

#define BMA150_BANDWIDTH__POS				0
#define BMA150_BANDWIDTH__LEN				3
#define BMA150_BANDWIDTH__MSK				0x07
#define BMA150_BANDWIDTH__REG				BMA150_RANGE_BWIDTH_REG

/* RANGE */

#define BMA150_RANGE__POS				3
#define BMA150_RANGE__LEN				2
#define BMA150_RANGE__MSK				0x18
#define BMA150_RANGE__REG				BMA150_RANGE_BWIDTH_REG

/* WAKE UP */

#define BMA150_WAKE_UP__POS			0
#define BMA150_WAKE_UP__LEN			1
#define BMA150_WAKE_UP__MSK			0x01
#define BMA150_WAKE_UP__REG			BMA150_CONF2_REG

#define BMA150_WAKE_UP_PAUSE__POS		1
#define BMA150_WAKE_UP_PAUSE__LEN		2
#define BMA150_WAKE_UP_PAUSE__MSK		0x06
#define BMA150_WAKE_UP_PAUSE__REG		BMA150_CONF2_REG

#define BMA150_GET_BITSLICE(regvar, bitname)\
	((regvar & bitname##__MSK) >> bitname##__POS)


#define BMA150_SET_BITSLICE(regvar, bitname, val)\
	((regvar & ~bitname##__MSK) | ((val<<bitname##__POS)&bitname##__MSK))

/* range and bandwidth */

#define BMA150_RANGE_2G			0
#define BMA150_RANGE_4G			1
#define BMA150_RANGE_8G			2

#define BMA150_BW_25HZ		0
#define BMA150_BW_50HZ		1
#define BMA150_BW_100HZ		2
#define BMA150_BW_190HZ		3
#define BMA150_BW_375HZ		4
#define BMA150_BW_750HZ		5
#define BMA150_BW_1500HZ	6

/* mode settings */

#define BMA150_MODE_NORMAL      0
#define BMA150_MODE_SLEEP       2
#define BMA150_MODE_WAKE_UP     3

struct bma150acc{
	s16	x,
		y,
		z;
} ;

struct bma150_data {
	struct i2c_client *bma150_client;
	struct bma150_platform_data *platform_data;
	int IRQ;
	atomic_t delay;
	unsigned char mode;
	struct input_dev *input;
	struct bma150acc value;
	struct mutex value_mutex;
	struct mutex mode_mutex;
	struct delayed_work work;
	struct work_struct irq_work;
};

static int bma150_smbus_read_byte(struct i2c_client *client,
		unsigned char reg_addr, unsigned char *data)
{
	s32 dummy;
	dummy = i2c_smbus_read_byte_data(client, reg_addr);
	if (dummy < 0)
		return -EPERM;
	*data = dummy & 0x000000ff;

	return 0;
}

static int bma150_smbus_write_byte(struct i2c_client *client,
		unsigned char reg_addr, unsigned char *data)
{
	s32 dummy;
	dummy = i2c_smbus_write_byte_data(client, reg_addr, *data);
	if (dummy < 0)
		return -EPERM;
	return 0;
}

static int bma150_smbus_read_byte_block(struct i2c_client *client,
		unsigned char reg_addr, unsigned char *data, unsigned char len)
{
	s32 dummy;
	dummy = i2c_smbus_read_i2c_block_data(client, reg_addr, len, data);
	if (dummy < 0)
		return -EPERM;
	return 0;
}

static int bma150_set_mode(struct i2c_client *client, unsigned char Mode)
{
	int comres = 0;
	unsigned char data1 = 0, data2 = 0;
	struct bma150_data *bma150 = i2c_get_clientdata(client);

	if (client == NULL) {
		comres = -1;
	} else{
		if (Mode < 4 && Mode != 1) {

			comres = bma150_smbus_read_byte(client,
						BMA150_WAKE_UP__REG, &data1);
			data1 = BMA150_SET_BITSLICE(data1,
						BMA150_WAKE_UP, Mode);
			comres += bma150_smbus_read_byte(client,
						BMA150_SLEEP__REG, &data2);
			data2 = BMA150_SET_BITSLICE(data2,
						BMA150_SLEEP, (Mode>>1));
			comres += bma150_smbus_write_byte(client,
						BMA150_WAKE_UP__REG, &data1);
			comres += bma150_smbus_write_byte(client,
						BMA150_SLEEP__REG, &data2);
			mutex_lock(&bma150->mode_mutex);
			bma150->mode = (unsigned char) Mode;
			mutex_unlock(&bma150->mode_mutex);

		} else{
			comres = -1;
		}
	}

	return comres;
}


static int bma150_set_range(struct i2c_client *client, unsigned char Range)
{
	int comres = 0;
	unsigned char data = 0;

	if (client == NULL) {
		comres = -1;
	} else{
		if (Range < 3) {

			comres = bma150_smbus_read_byte(client,
						BMA150_RANGE__REG, &data);
			data = BMA150_SET_BITSLICE(data, BMA150_RANGE, Range);
			comres += bma150_smbus_write_byte(client,
						BMA150_RANGE__REG, &data);

		} else{
			comres = -1;
		}
	}

	return comres;
}

static int bma150_get_range(struct i2c_client *client, unsigned char *Range)
{
	int comres = 0;
	unsigned char data;

	if (client == NULL) {
		comres = -1;
	} else{
		comres = bma150_smbus_read_byte(client,
						BMA150_RANGE__REG, &data);

		*Range = BMA150_GET_BITSLICE(data, BMA150_RANGE);

	}

	return comres;
}



static int bma150_set_bandwidth(struct i2c_client *client, unsigned char BW)
{
	int comres = 0;
	unsigned char data = 0;

	if (client == NULL) {
		comres = -1;
	} else{
		if (BW < 8) {
			comres = bma150_smbus_read_byte(client,
						BMA150_BANDWIDTH__REG, &data);
			data = BMA150_SET_BITSLICE(data, BMA150_BANDWIDTH, BW);
			comres += bma150_smbus_write_byte(client,
						BMA150_BANDWIDTH__REG, &data);

		} else{
			comres = -1;
		}
	}

	return comres;
}

static int bma150_get_bandwidth(struct i2c_client *client, unsigned char *BW)
{
	int comres = 0;
	unsigned char data;

	if (client == NULL) {
		comres = -1;
	} else{


		comres = bma150_smbus_read_byte(client,
						BMA150_BANDWIDTH__REG, &data);

		*BW = BMA150_GET_BITSLICE(data, BMA150_BANDWIDTH);


	}

	return comres;
}

static int bma150_read_accel_xyz(struct i2c_client *client,
		struct bma150acc *acc)
{
	int comres;
	unsigned char data[6];
	if (client == NULL) {
		comres = -1;
	} else{


		comres = bma150_smbus_read_byte_block(client,
					BMA150_ACC_X_LSB__REG, &data[0], 6);

		acc->x = BMA150_GET_BITSLICE(data[0], BMA150_ACC_X_LSB) |
			(BMA150_GET_BITSLICE(data[1], BMA150_ACC_X_MSB)<<
							BMA150_ACC_X_LSB__LEN);
		acc->x = acc->x << (sizeof(short)*8-(BMA150_ACC_X_LSB__LEN+
							BMA150_ACC_X_MSB__LEN));
		acc->x = acc->x >> (sizeof(short)*8-(BMA150_ACC_X_LSB__LEN+
							BMA150_ACC_X_MSB__LEN));

		acc->y = BMA150_GET_BITSLICE(data[2], BMA150_ACC_Y_LSB) |
			(BMA150_GET_BITSLICE(data[3], BMA150_ACC_Y_MSB)<<
							BMA150_ACC_Y_LSB__LEN);
		acc->y = acc->y << (sizeof(short)*8-(BMA150_ACC_Y_LSB__LEN +
							BMA150_ACC_Y_MSB__LEN));
		acc->y = acc->y >> (sizeof(short)*8-(BMA150_ACC_Y_LSB__LEN +
							BMA150_ACC_Y_MSB__LEN));


		acc->z = BMA150_GET_BITSLICE(data[4], BMA150_ACC_Z_LSB);
		acc->z |= (BMA150_GET_BITSLICE(data[5], BMA150_ACC_Z_MSB)<<
							BMA150_ACC_Z_LSB__LEN);
		acc->z = acc->z << (sizeof(short)*8-(BMA150_ACC_Z_LSB__LEN+
							BMA150_ACC_Z_MSB__LEN));
		acc->z = acc->z >> (sizeof(short)*8-(BMA150_ACC_Z_LSB__LEN+
							BMA150_ACC_Z_MSB__LEN));

	}

	return comres;
}

static void bma150_work_func(struct work_struct *work)
{
	struct bma150_data *bma150 = container_of((struct delayed_work *)work,
			struct bma150_data, work);
	static struct bma150acc acc;
	unsigned long delay = msecs_to_jiffies(atomic_read(&bma150->delay));



	bma150_read_accel_xyz(bma150->bma150_client, &acc);
	input_report_abs(bma150->input, ABS_X, acc.x);
	input_report_abs(bma150->input, ABS_Y, acc.y);
	input_report_abs(bma150->input, ABS_Z, acc.z);
	input_sync(bma150->input);
	mutex_lock(&bma150->value_mutex);
	bma150->value = acc;
	mutex_unlock(&bma150->value_mutex);
	schedule_delayed_work(&bma150->work, delay);
}

static ssize_t bma150_mode_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	unsigned char data;
	struct i2c_client *client = to_i2c_client(dev);
	struct bma150_data *bma150 = i2c_get_clientdata(client);

	mutex_lock(&bma150->mode_mutex);
	data = bma150->mode;
	mutex_unlock(&bma150->mode_mutex);

	return sprintf(buf, "%d\n", data);
}

static ssize_t bma150_mode_store(struct device *dev,
		struct device_attribute *attr,
		const char *buf, size_t count)
{
	unsigned long data;
	int error;
	struct i2c_client *client = to_i2c_client(dev);
	struct bma150_data *bma150 = i2c_get_clientdata(client);

	error = strict_strtoul(buf, 10, &data);
	if (error)
		return error;
	if (bma150_set_mode(bma150->bma150_client, (unsigned char) data) < 0)
		return -EINVAL;


	return count;
}
static ssize_t bma150_range_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	unsigned char data;
	struct i2c_client *client = to_i2c_client(dev);
	struct bma150_data *bma150 = i2c_get_clientdata(client);

	if (bma150_get_range(bma150->bma150_client, &data) < 0)
		return sprintf(buf, "Read error\n");

	return sprintf(buf, "%d\n", data);
}

static ssize_t bma150_range_store(struct device *dev,
		struct device_attribute *attr,
		const char *buf, size_t count)
{
	unsigned long data;
	int error;
	struct i2c_client *client = to_i2c_client(dev);
	struct bma150_data *bma150 = i2c_get_clientdata(client);

	error = strict_strtoul(buf, 10, &data);
	if (error)
		return error;
	if (bma150_set_range(bma150->bma150_client, (unsigned char) data) < 0)
		return -EINVAL;

	return count;
}

static ssize_t bma150_bandwidth_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	unsigned char data;
	struct i2c_client *client = to_i2c_client(dev);
	struct bma150_data *bma150 = i2c_get_clientdata(client);

	if (bma150_get_bandwidth(bma150->bma150_client, &data) < 0)
		return sprintf(buf, "Read error\n");

	return sprintf(buf, "%d\n", data);

}

static ssize_t bma150_bandwidth_store(struct device *dev,
		struct device_attribute *attr,
		const char *buf, size_t count)
{
	unsigned long data;
	int error;
	struct i2c_client *client = to_i2c_client(dev);
	struct bma150_data *bma150 = i2c_get_clientdata(client);

	error = strict_strtoul(buf, 10, &data);
	if (error)
		return error;
	if (bma150_set_bandwidth(bma150->bma150_client,
				(unsigned char) data) < 0)
		return -EINVAL;

	return count;
}

static ssize_t bma150_value_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct input_dev *input = to_input_dev(dev);
	struct bma150_data *bma150 = input_get_drvdata(input);
	struct bma150acc acc_value;

	mutex_lock(&bma150->value_mutex);
	acc_value = bma150->value;
	mutex_unlock(&bma150->value_mutex);

	return sprintf(buf, "%d %d %d\n", acc_value.x, acc_value.y,
			acc_value.z);
}



static ssize_t bma150_delay_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct bma150_data *bma150 = i2c_get_clientdata(client);

	return sprintf(buf, "%d\n", atomic_read(&bma150->delay));

}

static ssize_t bma150_delay_store(struct device *dev,
		struct device_attribute *attr,
		const char *buf, size_t count)
{
	unsigned long data;
	int error;
	struct i2c_client *client = to_i2c_client(dev);
	struct bma150_data *bma150 = i2c_get_clientdata(client);

	error = strict_strtoul(buf, 10, &data);
	if (error)
		return error;
	if (data > BMA150_MAX_DELAY)
		data = BMA150_MAX_DELAY;
	atomic_set(&bma150->delay, (unsigned int) data);

	return count;
}

static DEVICE_ATTR(range, S_IRUGO|S_IWUSR|S_IWGRP,
		bma150_range_show, bma150_range_store);
static DEVICE_ATTR(bandwidth, S_IRUGO|S_IWUSR|S_IWGRP,
		bma150_bandwidth_show, bma150_bandwidth_store);
static DEVICE_ATTR(mode, S_IRUGO|S_IWUSR|S_IWGRP,
		bma150_mode_show, bma150_mode_store);
static DEVICE_ATTR(value, S_IRUGO|S_IWUSR|S_IWGRP,
		bma150_value_show, NULL);
static DEVICE_ATTR(delay, S_IRUGO|S_IWUSR|S_IWGRP|S_IWOTH,
		bma150_delay_show, bma150_delay_store);

static struct attribute *bma150_attributes[] = {
	&dev_attr_range.attr,
	&dev_attr_bandwidth.attr,
	&dev_attr_mode.attr,
	&dev_attr_value.attr,
	&dev_attr_delay.attr,
	NULL
};

static struct attribute_group bma150_attribute_group = {
	.attrs = bma150_attributes
};

static int bma150_detect(struct i2c_client *client,
			  struct i2c_board_info *info)
{
	struct i2c_adapter *adapter = client->adapter;

	if (!i2c_check_functionality(adapter, I2C_FUNC_I2C))
		return -ENODEV;

	strlcpy(info->type, SENSOR_NAME, I2C_NAME_SIZE);

	return 0;
}

static int bma150_input_init(struct bma150_data *bma150)
{
	struct input_dev *dev;
	int err;

	dev = input_allocate_device();
	if (!dev)
		return -ENOMEM;
	dev->name = SENSOR_NAME;
	dev->id.bustype = BUS_I2C;

	input_set_capability(dev, EV_ABS, ABS_MISC);
	input_set_abs_params(dev, ABS_X, ABSMIN_2G, ABSMAX_2G, 0, 0);
	input_set_abs_params(dev, ABS_Y, ABSMIN_2G, ABSMAX_2G, 0, 0);
	input_set_abs_params(dev, ABS_Z, ABSMIN_2G, ABSMAX_2G, 0, 0);
	input_set_drvdata(dev, bma150);

	err = input_register_device(dev);
	if (err < 0) {
		input_free_device(dev);
		return err;
	}
	bma150->input = dev;

	return 0;
}

static void bma150_input_delete(struct bma150_data *bma150)
{
	struct input_dev *dev = bma150->input;

	input_unregister_device(dev);
	input_free_device(dev);
}

static int bma150_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{
	int err = 0;
	int tempvalue;
	struct bma150_data *data;

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		printk(KERN_INFO "i2c_check_functionality error\n");
		goto exit;
	}
	data = kzalloc(sizeof(struct bma150_data), GFP_KERNEL);
	if (!data) {
		err = -ENOMEM;
		goto exit;
	}

	i2c_set_clientdata(client, data);
	data->platform_data = client->dev.platform_data;

	if (data->platform_data->power_on)
		data->platform_data->power_on();
	else
		printk(KERN_ERR "power_on function not defined!!\n");

	tempvalue = 0;
	tempvalue = i2c_smbus_read_word_data(client, BMA150_CHIP_ID_REG);

	if ((tempvalue&0x00FF) == BMA150_CHIP_ID) {
		printk(KERN_INFO "Bosch Sensortec Device detected!\n" \
				"BMA150 registered I2C driver!\n");
	} else{
		printk(KERN_INFO "Bosch Sensortec Device not found" \
			"i2c error %d\n", tempvalue);
		err = -1;
		goto kfree_exit;
	}
	i2c_set_clientdata(client, data);
	data->bma150_client = client;
	mutex_init(&data->value_mutex);
	mutex_init(&data->mode_mutex);
	bma150_set_bandwidth(client, BMA150_BW_SET);
	bma150_set_range(client, BMA150_RANGE_SET);


	INIT_DELAYED_WORK(&data->work, bma150_work_func);
	atomic_set(&data->delay, BMA150_MAX_DELAY);
	err = bma150_input_init(data);
	if (err < 0)
		goto kfree_exit;

	err = sysfs_create_group(&data->input->dev.kobj,
			&bma150_attribute_group);
	if (err < 0)
		goto error_sysfs;

	schedule_delayed_work(&data->work,
			msecs_to_jiffies(atomic_read(&data->delay)));

	return 0;

error_sysfs:
	bma150_input_delete(data);

kfree_exit:
	kfree(data);
exit:
	return err;
}

static int bma150_suspend(struct i2c_client *client, pm_message_t mesg)
{
	struct bma150_data *data = i2c_get_clientdata(client);

	cancel_delayed_work_sync(&data->work);

	bma150_set_mode(client, BMA150_MODE_SLEEP);

	if ((data->platform_data) && (data->platform_data->power_off))
		data->platform_data->power_off();
=======
/*
 * Copyright (c) 2011 Bosch Sensortec GmbH
 * Copyright (c) 2011 Unixphere
 *
 * This driver adds support for Bosch Sensortec's digital acceleration
 * sensors BMA150 and SMB380.
 * The SMB380 is fully compatible with BMA150 and only differs in packaging.
 *
 * The datasheet for the BMA150 chip can be found here:
 * http://www.bosch-sensortec.com/content/language1/downloads/BST-BMA150-DS000-07.pdf
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/input-polldev.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/pm.h>
#include <linux/pm_runtime.h>
#include <linux/bma150.h>

#define ABSMAX_ACC_VAL		0x01FF
#define ABSMIN_ACC_VAL		-(ABSMAX_ACC_VAL)

/* Each axis is represented by a 2-byte data word */
#define BMA150_XYZ_DATA_SIZE	6

/* Input poll interval in milliseconds */
#define BMA150_POLL_INTERVAL	10
#define BMA150_POLL_MAX		200
#define BMA150_POLL_MIN		0

#define BMA150_MODE_NORMAL	0
#define BMA150_MODE_SLEEP	2
#define BMA150_MODE_WAKE_UP	3

/* Data register addresses */
#define BMA150_DATA_0_REG	0x00
#define BMA150_DATA_1_REG	0x01
#define BMA150_DATA_2_REG	0x02

/* Control register addresses */
#define BMA150_CTRL_0_REG	0x0A
#define BMA150_CTRL_1_REG	0x0B
#define BMA150_CTRL_2_REG	0x14
#define BMA150_CTRL_3_REG	0x15

/* Configuration/Setting register addresses */
#define BMA150_CFG_0_REG	0x0C
#define BMA150_CFG_1_REG	0x0D
#define BMA150_CFG_2_REG	0x0E
#define BMA150_CFG_3_REG	0x0F
#define BMA150_CFG_4_REG	0x10
#define BMA150_CFG_5_REG	0x11

#define BMA150_CHIP_ID		2
#define BMA150_CHIP_ID_REG	BMA150_DATA_0_REG

#define BMA150_ACC_X_LSB_REG	BMA150_DATA_2_REG

#define BMA150_SLEEP_POS	0
#define BMA150_SLEEP_MSK	0x01
#define BMA150_SLEEP_REG	BMA150_CTRL_0_REG

#define BMA150_BANDWIDTH_POS	0
#define BMA150_BANDWIDTH_MSK	0x07
#define BMA150_BANDWIDTH_REG	BMA150_CTRL_2_REG

#define BMA150_RANGE_POS	3
#define BMA150_RANGE_MSK	0x18
#define BMA150_RANGE_REG	BMA150_CTRL_2_REG

#define BMA150_WAKE_UP_POS	0
#define BMA150_WAKE_UP_MSK	0x01
#define BMA150_WAKE_UP_REG	BMA150_CTRL_3_REG

#define BMA150_SW_RES_POS	1
#define BMA150_SW_RES_MSK	0x02
#define BMA150_SW_RES_REG	BMA150_CTRL_0_REG

/* Any-motion interrupt register fields */
#define BMA150_ANY_MOTION_EN_POS	6
#define BMA150_ANY_MOTION_EN_MSK	0x40
#define BMA150_ANY_MOTION_EN_REG	BMA150_CTRL_1_REG

#define BMA150_ANY_MOTION_DUR_POS	6
#define BMA150_ANY_MOTION_DUR_MSK	0xC0
#define BMA150_ANY_MOTION_DUR_REG	BMA150_CFG_5_REG

#define BMA150_ANY_MOTION_THRES_REG	BMA150_CFG_4_REG

/* Advanced interrupt register fields */
#define BMA150_ADV_INT_EN_POS		6
#define BMA150_ADV_INT_EN_MSK		0x40
#define BMA150_ADV_INT_EN_REG		BMA150_CTRL_3_REG

/* High-G interrupt register fields */
#define BMA150_HIGH_G_EN_POS		1
#define BMA150_HIGH_G_EN_MSK		0x02
#define BMA150_HIGH_G_EN_REG		BMA150_CTRL_1_REG

#define BMA150_HIGH_G_HYST_POS		3
#define BMA150_HIGH_G_HYST_MSK		0x38
#define BMA150_HIGH_G_HYST_REG		BMA150_CFG_5_REG

#define BMA150_HIGH_G_DUR_REG		BMA150_CFG_3_REG
#define BMA150_HIGH_G_THRES_REG		BMA150_CFG_2_REG

/* Low-G interrupt register fields */
#define BMA150_LOW_G_EN_POS		0
#define BMA150_LOW_G_EN_MSK		0x01
#define BMA150_LOW_G_EN_REG		BMA150_CTRL_1_REG

#define BMA150_LOW_G_HYST_POS		0
#define BMA150_LOW_G_HYST_MSK		0x07
#define BMA150_LOW_G_HYST_REG		BMA150_CFG_5_REG

#define BMA150_LOW_G_DUR_REG		BMA150_CFG_1_REG
#define BMA150_LOW_G_THRES_REG		BMA150_CFG_0_REG

struct bma150_data {
	struct i2c_client *client;
	struct input_polled_dev *input_polled;
	struct input_dev *input;
	u8 mode;
};

/*
 * The settings for the given range, bandwidth and interrupt features
 * are stated and verified by Bosch Sensortec where they are configured
 * to provide a generic sensitivity performance.
 */
static struct bma150_cfg default_cfg = {
	.any_motion_int = 1,
	.hg_int = 1,
	.lg_int = 1,
	.any_motion_dur = 0,
	.any_motion_thres = 0,
	.hg_hyst = 0,
	.hg_dur = 150,
	.hg_thres = 160,
	.lg_hyst = 0,
	.lg_dur = 150,
	.lg_thres = 20,
	.range = BMA150_RANGE_2G,
	.bandwidth = BMA150_BW_50HZ
};

static int bma150_write_byte(struct i2c_client *client, u8 reg, u8 val)
{
	s32 ret;

	/* As per specification, disable irq in between register writes */
	if (client->irq)
		disable_irq_nosync(client->irq);

	ret = i2c_smbus_write_byte_data(client, reg, val);

	if (client->irq)
		enable_irq(client->irq);

	return ret;
}

static int bma150_set_reg_bits(struct i2c_client *client,
					int val, int shift, u8 mask, u8 reg)
{
	int data;

	data = i2c_smbus_read_byte_data(client, reg);
	if (data < 0)
		return data;

	data = (data & ~mask) | ((val << shift) & mask);
	return bma150_write_byte(client, reg, data);
}

static int bma150_set_mode(struct bma150_data *bma150, u8 mode)
{
	int error;

	error = bma150_set_reg_bits(bma150->client, mode, BMA150_WAKE_UP_POS,
				BMA150_WAKE_UP_MSK, BMA150_WAKE_UP_REG);
	if (error)
		return error;

	error = bma150_set_reg_bits(bma150->client, mode, BMA150_SLEEP_POS,
				BMA150_SLEEP_MSK, BMA150_SLEEP_REG);
	if (error)
		return error;

	if (mode == BMA150_MODE_NORMAL)
		msleep(2);

	bma150->mode = mode;
	return 0;
}

static int bma150_soft_reset(struct bma150_data *bma150)
{
	int error;

	error = bma150_set_reg_bits(bma150->client, 1, BMA150_SW_RES_POS,
				BMA150_SW_RES_MSK, BMA150_SW_RES_REG);
	if (error)
		return error;

	msleep(2);
	return 0;
}

static int bma150_set_range(struct bma150_data *bma150, u8 range)
{
	return bma150_set_reg_bits(bma150->client, range, BMA150_RANGE_POS,
				BMA150_RANGE_MSK, BMA150_RANGE_REG);
}

static int bma150_set_bandwidth(struct bma150_data *bma150, u8 bw)
{
	return bma150_set_reg_bits(bma150->client, bw, BMA150_BANDWIDTH_POS,
				BMA150_BANDWIDTH_MSK, BMA150_BANDWIDTH_REG);
}

static int bma150_set_low_g_interrupt(struct bma150_data *bma150,
					u8 enable, u8 hyst, u8 dur, u8 thres)
{
	int error;

	error = bma150_set_reg_bits(bma150->client, hyst,
				BMA150_LOW_G_HYST_POS, BMA150_LOW_G_HYST_MSK,
				BMA150_LOW_G_HYST_REG);
	if (error)
		return error;

	error = bma150_write_byte(bma150->client, BMA150_LOW_G_DUR_REG, dur);
	if (error)
		return error;

	error = bma150_write_byte(bma150->client, BMA150_LOW_G_THRES_REG, thres);
	if (error)
		return error;

	return bma150_set_reg_bits(bma150->client, !!enable,
				BMA150_LOW_G_EN_POS, BMA150_LOW_G_EN_MSK,
				BMA150_LOW_G_EN_REG);
}

static int bma150_set_high_g_interrupt(struct bma150_data *bma150,
					u8 enable, u8 hyst, u8 dur, u8 thres)
{
	int error;

	error = bma150_set_reg_bits(bma150->client, hyst,
				BMA150_HIGH_G_HYST_POS, BMA150_HIGH_G_HYST_MSK,
				BMA150_HIGH_G_HYST_REG);
	if (error)
		return error;

	error = bma150_write_byte(bma150->client,
				BMA150_HIGH_G_DUR_REG, dur);
	if (error)
		return error;

	error = bma150_write_byte(bma150->client,
				BMA150_HIGH_G_THRES_REG, thres);
	if (error)
		return error;

	return bma150_set_reg_bits(bma150->client, !!enable,
				BMA150_HIGH_G_EN_POS, BMA150_HIGH_G_EN_MSK,
				BMA150_HIGH_G_EN_REG);
}


static int bma150_set_any_motion_interrupt(struct bma150_data *bma150,
						u8 enable, u8 dur, u8 thres)
{
	int error;

	error = bma150_set_reg_bits(bma150->client, dur,
				BMA150_ANY_MOTION_DUR_POS,
				BMA150_ANY_MOTION_DUR_MSK,
				BMA150_ANY_MOTION_DUR_REG);
	if (error)
		return error;

	error = bma150_write_byte(bma150->client,
				BMA150_ANY_MOTION_THRES_REG, thres);
	if (error)
		return error;

	error = bma150_set_reg_bits(bma150->client, !!enable,
				BMA150_ADV_INT_EN_POS, BMA150_ADV_INT_EN_MSK,
				BMA150_ADV_INT_EN_REG);
	if (error)
		return error;

	return bma150_set_reg_bits(bma150->client, !!enable,
				BMA150_ANY_MOTION_EN_POS,
				BMA150_ANY_MOTION_EN_MSK,
				BMA150_ANY_MOTION_EN_REG);
}

static void bma150_report_xyz(struct bma150_data *bma150)
{
	u8 data[BMA150_XYZ_DATA_SIZE];
	s16 x, y, z;
	s32 ret;

	ret = i2c_smbus_read_i2c_block_data(bma150->client,
			BMA150_ACC_X_LSB_REG, BMA150_XYZ_DATA_SIZE, data);
	if (ret != BMA150_XYZ_DATA_SIZE)
		return;

	x = ((0xc0 & data[0]) >> 6) | (data[1] << 2);
	y = ((0xc0 & data[2]) >> 6) | (data[3] << 2);
	z = ((0xc0 & data[4]) >> 6) | (data[5] << 2);

	/* sign extension */
	x = (s16) (x << 6) >> 6;
	y = (s16) (y << 6) >> 6;
	z = (s16) (z << 6) >> 6;

	input_report_abs(bma150->input, ABS_X, x);
	input_report_abs(bma150->input, ABS_Y, y);
	input_report_abs(bma150->input, ABS_Z, z);
	input_sync(bma150->input);
}

static irqreturn_t bma150_irq_thread(int irq, void *dev)
{
	bma150_report_xyz(dev);

	return IRQ_HANDLED;
}

static void bma150_poll(struct input_polled_dev *dev)
{
	bma150_report_xyz(dev->private);
}

static int bma150_open(struct bma150_data *bma150)
{
	int error;

	error = pm_runtime_get_sync(&bma150->client->dev);
	if (error < 0 && error != -ENOSYS)
		return error;

	/*
	 * See if runtime PM woke up the device. If runtime PM
	 * is disabled we need to do it ourselves.
	 */
	if (bma150->mode != BMA150_MODE_NORMAL) {
		error = bma150_set_mode(bma150, BMA150_MODE_NORMAL);
		if (error)
			return error;
	}

	return 0;
}

static void bma150_close(struct bma150_data *bma150)
{
	pm_runtime_put_sync(&bma150->client->dev);

	if (bma150->mode != BMA150_MODE_SLEEP)
		bma150_set_mode(bma150, BMA150_MODE_SLEEP);
}

static int bma150_irq_open(struct input_dev *input)
{
	struct bma150_data *bma150 = input_get_drvdata(input);

	return bma150_open(bma150);
}

static void bma150_irq_close(struct input_dev *input)
{
	struct bma150_data *bma150 = input_get_drvdata(input);

	bma150_close(bma150);
}

static void bma150_poll_open(struct input_polled_dev *ipoll_dev)
{
	struct bma150_data *bma150 = ipoll_dev->private;

	bma150_open(bma150);
}

static void bma150_poll_close(struct input_polled_dev *ipoll_dev)
{
	struct bma150_data *bma150 = ipoll_dev->private;

	bma150_close(bma150);
}

static int bma150_initialize(struct bma150_data *bma150,
				       const struct bma150_cfg *cfg)
{
	int error;

	error = bma150_soft_reset(bma150);
	if (error)
		return error;

	error = bma150_set_bandwidth(bma150, cfg->bandwidth);
	if (error)
		return error;

	error = bma150_set_range(bma150, cfg->range);
	if (error)
		return error;

	if (bma150->client->irq) {
		error = bma150_set_any_motion_interrupt(bma150,
					cfg->any_motion_int,
					cfg->any_motion_dur,
					cfg->any_motion_thres);
		if (error)
			return error;

		error = bma150_set_high_g_interrupt(bma150,
					cfg->hg_int, cfg->hg_hyst,
					cfg->hg_dur, cfg->hg_thres);
		if (error)
			return error;

		error = bma150_set_low_g_interrupt(bma150,
					cfg->lg_int, cfg->lg_hyst,
					cfg->lg_dur, cfg->lg_thres);
		if (error)
			return error;
	}

	return bma150_set_mode(bma150, BMA150_MODE_SLEEP);
}

static void bma150_init_input_device(struct bma150_data *bma150,
						struct input_dev *idev)
{
	idev->name = BMA150_DRIVER;
	idev->phys = BMA150_DRIVER "/input0";
	idev->id.bustype = BUS_I2C;
	idev->dev.parent = &bma150->client->dev;

	idev->evbit[0] = BIT_MASK(EV_ABS);
	input_set_abs_params(idev, ABS_X, ABSMIN_ACC_VAL, ABSMAX_ACC_VAL, 0, 0);
	input_set_abs_params(idev, ABS_Y, ABSMIN_ACC_VAL, ABSMAX_ACC_VAL, 0, 0);
	input_set_abs_params(idev, ABS_Z, ABSMIN_ACC_VAL, ABSMAX_ACC_VAL, 0, 0);
}

static int bma150_register_input_device(struct bma150_data *bma150)
{
	struct input_dev *idev;
	int error;

	idev = input_allocate_device();
	if (!idev)
		return -ENOMEM;

	bma150_init_input_device(bma150, idev);

	idev->open = bma150_irq_open;
	idev->close = bma150_irq_close;
	input_set_drvdata(idev, bma150);

	error = input_register_device(idev);
	if (error) {
		input_free_device(idev);
		return error;
	}

	bma150->input = idev;
	return 0;
}

static int bma150_register_polled_device(struct bma150_data *bma150)
{
	struct input_polled_dev *ipoll_dev;
	int error;

	ipoll_dev = input_allocate_polled_device();
	if (!ipoll_dev)
		return -ENOMEM;

	ipoll_dev->private = bma150;
	ipoll_dev->open = bma150_poll_open;
	ipoll_dev->close = bma150_poll_close;
	ipoll_dev->poll = bma150_poll;
	ipoll_dev->poll_interval = BMA150_POLL_INTERVAL;
	ipoll_dev->poll_interval_min = BMA150_POLL_MIN;
	ipoll_dev->poll_interval_max = BMA150_POLL_MAX;

	bma150_init_input_device(bma150, ipoll_dev->input);

	error = input_register_polled_device(ipoll_dev);
	if (error) {
		input_free_polled_device(ipoll_dev);
		return error;
	}

	bma150->input_polled = ipoll_dev;
	bma150->input = ipoll_dev->input;

	return 0;
}

static int bma150_probe(struct i2c_client *client,
				  const struct i2c_device_id *id)
{
	const struct bma150_platform_data *pdata = client->dev.platform_data;
	const struct bma150_cfg *cfg;
	struct bma150_data *bma150;
	int chip_id;
	int error;

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		dev_err(&client->dev, "i2c_check_functionality error\n");
		return -EIO;
	}

	chip_id = i2c_smbus_read_byte_data(client, BMA150_CHIP_ID_REG);
	if (chip_id != BMA150_CHIP_ID) {
		dev_err(&client->dev, "BMA150 chip id error: %d\n", chip_id);
		return -EINVAL;
	}

	bma150 = kzalloc(sizeof(struct bma150_data), GFP_KERNEL);
	if (!bma150)
		return -ENOMEM;

	bma150->client = client;

	if (pdata) {
		if (pdata->irq_gpio_cfg) {
			error = pdata->irq_gpio_cfg();
			if (error) {
				dev_err(&client->dev,
					"IRQ GPIO conf. error %d, error %d\n",
					client->irq, error);
				goto err_free_mem;
			}
		}
		cfg = &pdata->cfg;
	} else {
		cfg = &default_cfg;
	}

	error = bma150_initialize(bma150, cfg);
	if (error)
		goto err_free_mem;

	if (client->irq > 0) {
		error = bma150_register_input_device(bma150);
		if (error)
			goto err_free_mem;

		error = request_threaded_irq(client->irq,
					NULL, bma150_irq_thread,
					IRQF_TRIGGER_RISING | IRQF_ONESHOT,
					BMA150_DRIVER, bma150);
		if (error) {
			dev_err(&client->dev,
				"irq request failed %d, error %d\n",
				client->irq, error);
			input_unregister_device(bma150->input);
			goto err_free_mem;
		}
	} else {
		error = bma150_register_polled_device(bma150);
		if (error)
			goto err_free_mem;
	}

	i2c_set_clientdata(client, bma150);

	pm_runtime_enable(&client->dev);

	return 0;

err_free_mem:
	kfree(bma150);
	return error;
}

static int bma150_remove(struct i2c_client *client)
{
	struct bma150_data *bma150 = i2c_get_clientdata(client);

	pm_runtime_disable(&client->dev);

	if (client->irq > 0) {
		free_irq(client->irq, bma150);
		input_unregister_device(bma150->input);
	} else {
		input_unregister_polled_device(bma150->input_polled);
		input_free_polled_device(bma150->input_polled);
	}

	kfree(bma150);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
static int bma150_resume(struct i2c_client *client)
{
	struct bma150_data *data = i2c_get_clientdata(client);

	if ((data->platform_data) && (data->platform_data->power_on))
		data->platform_data->power_on();

	bma150_set_mode(client, BMA150_MODE_NORMAL);

	schedule_delayed_work(&data->work,
		msecs_to_jiffies(atomic_read(&data->delay)));

	return 0;
}

static int bma150_remove(struct i2c_client *client)
{
	struct bma150_data *data = i2c_get_clientdata(client);

	if (data->platform_data->power_off)
		data->platform_data->power_off();
	else
		printk(KERN_ERR "power_off function not defined!!\n");

	sysfs_remove_group(&data->input->dev.kobj, &bma150_attribute_group);
	bma150_input_delete(data);
	free_irq(data->IRQ, data);
	kfree(data);

	return 0;
}

static const struct i2c_device_id bma150_id[] = {
	{ SENSOR_NAME, 0 },
=======
#ifdef CONFIG_PM
static int bma150_suspend(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct bma150_data *bma150 = i2c_get_clientdata(client);

	return bma150_set_mode(bma150, BMA150_MODE_SLEEP);
}

static int bma150_resume(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct bma150_data *bma150 = i2c_get_clientdata(client);

	return bma150_set_mode(bma150, BMA150_MODE_NORMAL);
}
#endif

static UNIVERSAL_DEV_PM_OPS(bma150_pm, bma150_suspend, bma150_resume, NULL);

static const struct i2c_device_id bma150_id[] = {
	{ "bma150", 0 },
	{ "smb380", 0 },
	{ "bma023", 0 },
>>>>>>> refs/remotes/origin/master
	{ }
};

MODULE_DEVICE_TABLE(i2c, bma150_id);

static struct i2c_driver bma150_driver = {
	.driver = {
		.owner	= THIS_MODULE,
<<<<<<< HEAD
		.name	= SENSOR_NAME,
	},
	.class          = I2C_CLASS_HWMON,
	.id_table	= bma150_id,
	.probe		= bma150_probe,
	.remove		= bma150_remove,
	.detect		= bma150_detect,
	.suspend    = bma150_suspend,
	.resume     = bma150_resume,
};

static int __init BMA150_init(void)
{
	return i2c_add_driver(&bma150_driver);
}

static void __exit BMA150_exit(void)
{
	i2c_del_driver(&bma150_driver);
}

MODULE_DESCRIPTION("BMA150 driver");

module_init(BMA150_init);
module_exit(BMA150_exit);

=======
		.name	= BMA150_DRIVER,
		.pm	= &bma150_pm,
	},
	.class		= I2C_CLASS_HWMON,
	.id_table	= bma150_id,
	.probe		= bma150_probe,
	.remove		= bma150_remove,
};

module_i2c_driver(bma150_driver);

MODULE_AUTHOR("Albert Zhang <xu.zhang@bosch-sensortec.com>");
MODULE_DESCRIPTION("BMA150 driver");
MODULE_LICENSE("GPL");
>>>>>>> refs/remotes/origin/master
