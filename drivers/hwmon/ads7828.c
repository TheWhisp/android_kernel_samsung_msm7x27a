/*
<<<<<<< HEAD
<<<<<<< HEAD
	ads7828.c - lm_sensors driver for ads7828 12-bit 8-channel ADC
	(C) 2007 EADS Astrium

	This driver is based on the lm75 and other lm_sensors/hwmon drivers

	Written by Steve Hardy <shardy@redhat.com>

	Datasheet available at: http://focus.ti.com/lit/ds/symlink/ads7828.pdf

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
=======
 * ads7828.c - lm_sensors driver for ads7828 12-bit 8-channel ADC
=======
 * ads7828.c - driver for TI ADS7828 8-channel A/D converter and compatibles
>>>>>>> refs/remotes/origin/master
 * (C) 2007 EADS Astrium
 *
 * This driver is based on the lm75 and other lm_sensors/hwmon drivers
 *
 * Written by Steve Hardy <shardy@redhat.com>
 *
<<<<<<< HEAD
 * Datasheet available at: http://focus.ti.com/lit/ds/symlink/ads7828.pdf
=======
 * ADS7830 support, by Guillaume Roguez <guillaume.roguez@savoirfairelinux.com>
 *
 * For further information, see the Documentation/hwmon/ads7828 file.
>>>>>>> refs/remotes/origin/master
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/i2c.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
#include <linux/err.h>
#include <linux/mutex.h>

/* The ADS7828 registers */
#define ADS7828_NCH 8 /* 8 channels of 12-bit A-D supported */
#define ADS7828_CMD_SD_SE 0x80 /* Single ended inputs */
#define ADS7828_CMD_SD_DIFF 0x00 /* Differential inputs */
#define ADS7828_CMD_PD0 0x0 /* Power Down between A-D conversions */
#define ADS7828_CMD_PD1 0x04 /* Internal ref OFF && A-D ON */
#define ADS7828_CMD_PD2 0x08 /* Internal ref ON && A-D OFF */
#define ADS7828_CMD_PD3 0x0C /* Internal ref ON && A-D ON */
#define ADS7828_INT_VREF_MV 2500 /* Internal vref is 2.5V, 2500mV */

/* Addresses to scan */
static const unsigned short normal_i2c[] = { 0x48, 0x49, 0x4a, 0x4b,
	I2C_CLIENT_END };

/* Module parameters */
<<<<<<< HEAD
static int se_input = 1; /* Default is SE, 0 == diff */
static int int_vref = 1; /* Default is internal ref ON */
=======
static bool se_input = 1; /* Default is SE, 0 == diff */
static bool int_vref = 1; /* Default is internal ref ON */
>>>>>>> refs/remotes/origin/cm-10.0
static int vref_mv = ADS7828_INT_VREF_MV; /* set if vref != 2.5V */
module_param(se_input, bool, S_IRUGO);
module_param(int_vref, bool, S_IRUGO);
module_param(vref_mv, int, S_IRUGO);

/* Global Variables */
static u8 ads7828_cmd_byte; /* cmd byte without channel bits */
static unsigned int ads7828_lsb_resol; /* resolution of the ADC sample lsb */

/* Each client has this additional data */
struct ads7828_data {
	struct device *hwmon_dev;
	struct mutex update_lock; /* mutex protect updates */
	char valid; /* !=0 if following fields are valid */
	unsigned long last_updated; /* In jiffies */
	u16 adc_input[ADS7828_NCH]; /* ADS7828_NCH 12-bit samples */
};

/* Function declaration - necessary due to function dependencies */
static int ads7828_detect(struct i2c_client *client,
			  struct i2c_board_info *info);
static int ads7828_probe(struct i2c_client *client,
			 const struct i2c_device_id *id);

<<<<<<< HEAD
/* The ADS7828 returns the 12-bit sample in two bytes,
	these are read as a word then byte-swapped */
static u16 ads7828_read_value(struct i2c_client *client, u8 reg)
{
	return swab16(i2c_smbus_read_word_data(client, reg));
}

=======
>>>>>>> refs/remotes/origin/cm-10.0
static inline u8 channel_cmd_byte(int ch)
{
	/* cmd byte C2,C1,C0 - see datasheet */
	u8 cmd = (((ch>>1) | (ch&0x01)<<2)<<4);
	cmd |= ads7828_cmd_byte;
	return cmd;
=======

#include <linux/err.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/platform_data/ads7828.h>
#include <linux/slab.h>

/* The ADS7828 registers */
#define ADS7828_NCH		8	/* 8 channels supported */
#define ADS7828_CMD_SD_SE	0x80	/* Single ended inputs */
#define ADS7828_CMD_PD1		0x04	/* Internal vref OFF && A/D ON */
#define ADS7828_CMD_PD3		0x0C	/* Internal vref ON && A/D ON */
#define ADS7828_INT_VREF_MV	2500	/* Internal vref is 2.5V, 2500mV */
#define ADS7828_EXT_VREF_MV_MIN	50	/* External vref min value 0.05V */
#define ADS7828_EXT_VREF_MV_MAX	5250	/* External vref max value 5.25V */

/* List of supported devices */
enum ads7828_chips { ads7828, ads7830 };

/* Client specific data */
struct ads7828_data {
	struct device *hwmon_dev;
	struct mutex update_lock;	/* Mutex protecting updates */
	unsigned long last_updated;	/* Last updated time (in jiffies) */
	u16 adc_input[ADS7828_NCH];	/* ADS7828_NCH samples */
	bool valid;			/* Validity flag */
	bool diff_input;		/* Differential input */
	bool ext_vref;			/* External voltage reference */
	unsigned int vref_mv;		/* voltage reference value */
	u8 cmd_byte;			/* Command byte without channel bits */
	unsigned int lsb_resol;		/* Resolution of the ADC sample LSB */
	s32 (*read_channel)(const struct i2c_client *client, u8 command);
};

/* Command byte C2,C1,C0 - see datasheet */
static inline u8 ads7828_cmd_byte(u8 cmd, int ch)
{
	return cmd | (((ch >> 1) | (ch & 0x01) << 2) << 4);
>>>>>>> refs/remotes/origin/master
}

/* Update data for the device (all 8 channels) */
static struct ads7828_data *ads7828_update_device(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct ads7828_data *data = i2c_get_clientdata(client);

	mutex_lock(&data->update_lock);

	if (time_after(jiffies, data->last_updated + HZ + HZ / 2)
			|| !data->valid) {
		unsigned int ch;
		dev_dbg(&client->dev, "Starting ads7828 update\n");

		for (ch = 0; ch < ADS7828_NCH; ch++) {
<<<<<<< HEAD
			u8 cmd = channel_cmd_byte(ch);
<<<<<<< HEAD
			data->adc_input[ch] = ads7828_read_value(client, cmd);
=======
			data->adc_input[ch] =
				i2c_smbus_read_word_swapped(client, cmd);
>>>>>>> refs/remotes/origin/cm-10.0
		}
		data->last_updated = jiffies;
		data->valid = 1;
=======
			u8 cmd = ads7828_cmd_byte(data->cmd_byte, ch);
			data->adc_input[ch] = data->read_channel(client, cmd);
		}
		data->last_updated = jiffies;
		data->valid = true;
>>>>>>> refs/remotes/origin/master
	}

	mutex_unlock(&data->update_lock);

	return data;
}

/* sysfs callback function */
<<<<<<< HEAD
static ssize_t show_in(struct device *dev, struct device_attribute *da,
	char *buf)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	struct ads7828_data *data = ads7828_update_device(dev);
	/* Print value (in mV as specified in sysfs-interface documentation) */
	return sprintf(buf, "%d\n", (data->adc_input[attr->index] *
		ads7828_lsb_resol)/1000);
}

#define in_reg(offset)\
static SENSOR_DEVICE_ATTR(in##offset##_input, S_IRUGO, show_in,\
	NULL, offset)

in_reg(0);
in_reg(1);
in_reg(2);
in_reg(3);
in_reg(4);
in_reg(5);
in_reg(6);
in_reg(7);
=======
static ssize_t ads7828_show_in(struct device *dev, struct device_attribute *da,
			       char *buf)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	struct ads7828_data *data = ads7828_update_device(dev);
	unsigned int value = DIV_ROUND_CLOSEST(data->adc_input[attr->index] *
					       data->lsb_resol, 1000);

	return sprintf(buf, "%d\n", value);
}

static SENSOR_DEVICE_ATTR(in0_input, S_IRUGO, ads7828_show_in, NULL, 0);
static SENSOR_DEVICE_ATTR(in1_input, S_IRUGO, ads7828_show_in, NULL, 1);
static SENSOR_DEVICE_ATTR(in2_input, S_IRUGO, ads7828_show_in, NULL, 2);
static SENSOR_DEVICE_ATTR(in3_input, S_IRUGO, ads7828_show_in, NULL, 3);
static SENSOR_DEVICE_ATTR(in4_input, S_IRUGO, ads7828_show_in, NULL, 4);
static SENSOR_DEVICE_ATTR(in5_input, S_IRUGO, ads7828_show_in, NULL, 5);
static SENSOR_DEVICE_ATTR(in6_input, S_IRUGO, ads7828_show_in, NULL, 6);
static SENSOR_DEVICE_ATTR(in7_input, S_IRUGO, ads7828_show_in, NULL, 7);
>>>>>>> refs/remotes/origin/master

static struct attribute *ads7828_attributes[] = {
	&sensor_dev_attr_in0_input.dev_attr.attr,
	&sensor_dev_attr_in1_input.dev_attr.attr,
	&sensor_dev_attr_in2_input.dev_attr.attr,
	&sensor_dev_attr_in3_input.dev_attr.attr,
	&sensor_dev_attr_in4_input.dev_attr.attr,
	&sensor_dev_attr_in5_input.dev_attr.attr,
	&sensor_dev_attr_in6_input.dev_attr.attr,
	&sensor_dev_attr_in7_input.dev_attr.attr,
	NULL
};

static const struct attribute_group ads7828_group = {
	.attrs = ads7828_attributes,
};

static int ads7828_remove(struct i2c_client *client)
{
	struct ads7828_data *data = i2c_get_clientdata(client);
<<<<<<< HEAD
	hwmon_device_unregister(data->hwmon_dev);
	sysfs_remove_group(&client->dev.kobj, &ads7828_group);
	kfree(i2c_get_clientdata(client));
	return 0;
}

static const struct i2c_device_id ads7828_id[] = {
	{ "ads7828", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, ads7828_id);

/* This is the driver that will be inserted */
static struct i2c_driver ads7828_driver = {
	.class = I2C_CLASS_HWMON,
	.driver = {
		.name = "ads7828",
	},
	.probe = ads7828_probe,
	.remove = ads7828_remove,
	.id_table = ads7828_id,
	.detect = ads7828_detect,
	.address_list = normal_i2c,
};

/* Return 0 if detection is successful, -ENODEV otherwise */
static int ads7828_detect(struct i2c_client *client,
			  struct i2c_board_info *info)
{
	struct i2c_adapter *adapter = client->adapter;
	int ch;

	/* Check we have a valid client */
	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_READ_WORD_DATA))
		return -ENODEV;

<<<<<<< HEAD
	/* Now, we do the remaining detection. There is no identification
	dedicated register so attempt to sanity check using knowledge of
	the chip
	- Read from the 8 channel addresses
	- Check the top 4 bits of each result are not set (12 data bits)
	*/
	for (ch = 0; ch < ADS7828_NCH; ch++) {
		u16 in_data;
		u8 cmd = channel_cmd_byte(ch);
		in_data = ads7828_read_value(client, cmd);
=======
	/*
	 * Now, we do the remaining detection. There is no identification
	 * dedicated register so attempt to sanity check using knowledge of
	 * the chip
	 * - Read from the 8 channel addresses
	 * - Check the top 4 bits of each result are not set (12 data bits)
	 */
	for (ch = 0; ch < ADS7828_NCH; ch++) {
		u16 in_data;
		u8 cmd = channel_cmd_byte(ch);
		in_data = i2c_smbus_read_word_swapped(client, cmd);
>>>>>>> refs/remotes/origin/cm-10.0
		if (in_data & 0xF000) {
			pr_debug("%s : Doesn't look like an ads7828 device\n",
				 __func__);
			return -ENODEV;
		}
	}

	strlcpy(info->type, "ads7828", I2C_NAME_SIZE);
=======

	hwmon_device_unregister(data->hwmon_dev);
	sysfs_remove_group(&client->dev.kobj, &ads7828_group);
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int ads7828_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
<<<<<<< HEAD
	struct ads7828_data *data;
	int err;

	data = kzalloc(sizeof(struct ads7828_data), GFP_KERNEL);
	if (!data) {
		err = -ENOMEM;
		goto exit;
	}

	i2c_set_clientdata(client, data);
	mutex_init(&data->update_lock);

	/* Register sysfs hooks */
	err = sysfs_create_group(&client->dev.kobj, &ads7828_group);
	if (err)
		goto exit_free;
=======
	struct ads7828_platform_data *pdata = dev_get_platdata(&client->dev);
	struct ads7828_data *data;
	int err;

	data = devm_kzalloc(&client->dev, sizeof(struct ads7828_data),
			    GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	if (pdata) {
		data->diff_input = pdata->diff_input;
		data->ext_vref = pdata->ext_vref;
		if (data->ext_vref)
			data->vref_mv = pdata->vref_mv;
	}

	/* Bound Vref with min/max values if it was provided */
	if (data->vref_mv)
		data->vref_mv = clamp_val(data->vref_mv,
					  ADS7828_EXT_VREF_MV_MIN,
					  ADS7828_EXT_VREF_MV_MAX);
	else
		data->vref_mv = ADS7828_INT_VREF_MV;

	/* ADS7828 uses 12-bit samples, while ADS7830 is 8-bit */
	if (id->driver_data == ads7828) {
		data->lsb_resol = DIV_ROUND_CLOSEST(data->vref_mv * 1000, 4096);
		data->read_channel = i2c_smbus_read_word_swapped;
	} else {
		data->lsb_resol = DIV_ROUND_CLOSEST(data->vref_mv * 1000, 256);
		data->read_channel = i2c_smbus_read_byte_data;
	}

	data->cmd_byte = data->ext_vref ? ADS7828_CMD_PD1 : ADS7828_CMD_PD3;
	if (!data->diff_input)
		data->cmd_byte |= ADS7828_CMD_SD_SE;

	i2c_set_clientdata(client, data);
	mutex_init(&data->update_lock);

	err = sysfs_create_group(&client->dev.kobj, &ads7828_group);
	if (err)
		return err;
>>>>>>> refs/remotes/origin/master

	data->hwmon_dev = hwmon_device_register(&client->dev);
	if (IS_ERR(data->hwmon_dev)) {
		err = PTR_ERR(data->hwmon_dev);
<<<<<<< HEAD
		goto exit_remove;
=======
		goto error;
>>>>>>> refs/remotes/origin/master
	}

	return 0;

<<<<<<< HEAD
exit_remove:
	sysfs_remove_group(&client->dev.kobj, &ads7828_group);
exit_free:
	kfree(data);
exit:
	return err;
}

static int __init sensors_ads7828_init(void)
{
	/* Initialize the command byte according to module parameters */
	ads7828_cmd_byte = se_input ?
		ADS7828_CMD_SD_SE : ADS7828_CMD_SD_DIFF;
	ads7828_cmd_byte |= int_vref ?
		ADS7828_CMD_PD3 : ADS7828_CMD_PD1;

	/* Calculate the LSB resolution */
	ads7828_lsb_resol = (vref_mv*1000)/4096;

	return i2c_add_driver(&ads7828_driver);
}

static void __exit sensors_ads7828_exit(void)
{
	i2c_del_driver(&ads7828_driver);
}

MODULE_AUTHOR("Steve Hardy <shardy@redhat.com>");
MODULE_DESCRIPTION("ADS7828 driver");
MODULE_LICENSE("GPL");

module_init(sensors_ads7828_init);
module_exit(sensors_ads7828_exit);
=======
error:
	sysfs_remove_group(&client->dev.kobj, &ads7828_group);
	return err;
}

static const struct i2c_device_id ads7828_device_ids[] = {
	{ "ads7828", ads7828 },
	{ "ads7830", ads7830 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, ads7828_device_ids);

static struct i2c_driver ads7828_driver = {
	.driver = {
		.name = "ads7828",
	},

	.id_table = ads7828_device_ids,
	.probe = ads7828_probe,
	.remove = ads7828_remove,
};

module_i2c_driver(ads7828_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Steve Hardy <shardy@redhat.com>");
MODULE_DESCRIPTION("Driver for TI ADS7828 A/D converter and compatibles");
>>>>>>> refs/remotes/origin/master
