/*
 * ads1015.c - lm_sensors driver for ads1015 12-bit 4-input ADC
 * (C) Copyright 2010
 * Dirk Eibach, Guntermann & Drunck GmbH <eibach@gdsys.de>
 *
 * Based on the ads7828 driver by Steve Hardy.
 *
 * Datasheet available at: http://focus.ti.com/lit/ds/symlink/ads1015.pdf
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

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
#include <linux/err.h>
#include <linux/mutex.h>
#include <linux/of.h>

#include <linux/i2c/ads1015.h>

/* ADS1015 registers */
enum {
	ADS1015_CONVERSION = 0,
	ADS1015_CONFIG = 1,
};

/* PGA fullscale voltages in mV */
static const unsigned int fullscale_table[8] = {
	6144, 4096, 2048, 1024, 512, 256, 256, 256 };

/* Data rates in samples per second */
<<<<<<< HEAD
static const unsigned int data_rate_table[8] = {
	128, 250, 490, 920, 1600, 2400, 3300, 3300 };
=======
static const unsigned int data_rate_table_1015[8] = {
	128, 250, 490, 920, 1600, 2400, 3300, 3300
};

static const unsigned int data_rate_table_1115[8] = {
	8, 16, 32, 64, 128, 250, 475, 860
};
>>>>>>> refs/remotes/origin/master

#define ADS1015_DEFAULT_CHANNELS 0xff
#define ADS1015_DEFAULT_PGA 2
#define ADS1015_DEFAULT_DATA_RATE 4

<<<<<<< HEAD
=======
enum ads1015_chips {
	ads1015,
	ads1115,
};

>>>>>>> refs/remotes/origin/master
struct ads1015_data {
	struct device *hwmon_dev;
	struct mutex update_lock; /* mutex protect updates */
	struct ads1015_channel_data channel_data[ADS1015_CHANNELS];
<<<<<<< HEAD
};

<<<<<<< HEAD
static s32 ads1015_read_reg(struct i2c_client *client, unsigned int reg)
{
	s32 data = i2c_smbus_read_word_data(client, reg);

	return (data < 0) ? data : swab16(data);
}

static s32 ads1015_write_reg(struct i2c_client *client, unsigned int reg,
			     u16 val)
{
	return i2c_smbus_write_word_data(client, reg, swab16(val));
}

static int ads1015_read_value(struct i2c_client *client, unsigned int channel,
			      int *value)
{
	u16 config;
	s16 conversion;
	struct ads1015_data *data = i2c_get_clientdata(client);
	unsigned int pga = data->channel_data[channel].pga;
	int fullscale;
=======
=======
	enum ads1015_chips id;
};

>>>>>>> refs/remotes/origin/master
static int ads1015_read_adc(struct i2c_client *client, unsigned int channel)
{
	u16 config;
	struct ads1015_data *data = i2c_get_clientdata(client);
	unsigned int pga = data->channel_data[channel].pga;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	unsigned int data_rate = data->channel_data[channel].data_rate;
	unsigned int conversion_time_ms;
=======
	unsigned int data_rate = data->channel_data[channel].data_rate;
	unsigned int conversion_time_ms;
	const unsigned int * const rate_table = data->id == ads1115 ?
		data_rate_table_1115 : data_rate_table_1015;
>>>>>>> refs/remotes/origin/master
	int res;

	mutex_lock(&data->update_lock);

	/* get channel parameters */
<<<<<<< HEAD
<<<<<<< HEAD
	res = ads1015_read_reg(client, ADS1015_CONFIG);
	if (res < 0)
		goto err_unlock;
	config = res;
	fullscale = fullscale_table[pga];
=======
=======
>>>>>>> refs/remotes/origin/master
	res = i2c_smbus_read_word_swapped(client, ADS1015_CONFIG);
	if (res < 0)
		goto err_unlock;
	config = res;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	conversion_time_ms = DIV_ROUND_UP(1000, data_rate_table[data_rate]);
=======
	conversion_time_ms = DIV_ROUND_UP(1000, rate_table[data_rate]);
>>>>>>> refs/remotes/origin/master

	/* setup and start single conversion */
	config &= 0x001f;
	config |= (1 << 15) | (1 << 8);
	config |= (channel & 0x0007) << 12;
	config |= (pga & 0x0007) << 9;
	config |= (data_rate & 0x0007) << 5;

<<<<<<< HEAD
<<<<<<< HEAD
	res = ads1015_write_reg(client, ADS1015_CONFIG, config);
=======
	res = i2c_smbus_write_word_swapped(client, ADS1015_CONFIG, config);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	res = i2c_smbus_write_word_swapped(client, ADS1015_CONFIG, config);
>>>>>>> refs/remotes/origin/master
	if (res < 0)
		goto err_unlock;

	/* wait until conversion finished */
	msleep(conversion_time_ms);
<<<<<<< HEAD
<<<<<<< HEAD
	res = ads1015_read_reg(client, ADS1015_CONFIG);
=======
	res = i2c_smbus_read_word_swapped(client, ADS1015_CONFIG);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	res = i2c_smbus_read_word_swapped(client, ADS1015_CONFIG);
>>>>>>> refs/remotes/origin/master
	if (res < 0)
		goto err_unlock;
	config = res;
	if (!(config & (1 << 15))) {
		/* conversion not finished in time */
		res = -EIO;
		goto err_unlock;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	res = ads1015_read_reg(client, ADS1015_CONVERSION);
	if (res < 0)
		goto err_unlock;
	conversion = res;

	mutex_unlock(&data->update_lock);

	*value = DIV_ROUND_CLOSEST(conversion * fullscale, 0x7ff0);

	return 0;
=======
	res = i2c_smbus_read_word_swapped(client, ADS1015_CONVERSION);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	res = i2c_smbus_read_word_swapped(client, ADS1015_CONVERSION);
>>>>>>> refs/remotes/origin/master

err_unlock:
	mutex_unlock(&data->update_lock);
	return res;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static int ads1015_reg_to_mv(struct i2c_client *client, unsigned int channel,
			     s16 reg)
{
	struct ads1015_data *data = i2c_get_clientdata(client);
	unsigned int pga = data->channel_data[channel].pga;
	int fullscale = fullscale_table[pga];
<<<<<<< HEAD

	return DIV_ROUND_CLOSEST(reg * fullscale, 0x7ff0);
}

>>>>>>> refs/remotes/origin/cm-10.0
=======
	const unsigned mask = data->id == ads1115 ? 0x7fff : 0x7ff0;

	return DIV_ROUND_CLOSEST(reg * fullscale, mask);
}

>>>>>>> refs/remotes/origin/master
/* sysfs callback function */
static ssize_t show_in(struct device *dev, struct device_attribute *da,
	char *buf)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	struct i2c_client *client = to_i2c_client(dev);
<<<<<<< HEAD
<<<<<<< HEAD
	int in;
	int res;

	res = ads1015_read_value(client, attr->index, &in);

	return (res < 0) ? res : sprintf(buf, "%d\n", in);
=======
=======
>>>>>>> refs/remotes/origin/master
	int res;
	int index = attr->index;

	res = ads1015_read_adc(client, index);
	if (res < 0)
		return res;

	return sprintf(buf, "%d\n", ads1015_reg_to_mv(client, index, res));
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
}

static const struct sensor_device_attribute ads1015_in[] = {
	SENSOR_ATTR(in0_input, S_IRUGO, show_in, NULL, 0),
	SENSOR_ATTR(in1_input, S_IRUGO, show_in, NULL, 1),
	SENSOR_ATTR(in2_input, S_IRUGO, show_in, NULL, 2),
	SENSOR_ATTR(in3_input, S_IRUGO, show_in, NULL, 3),
	SENSOR_ATTR(in4_input, S_IRUGO, show_in, NULL, 4),
	SENSOR_ATTR(in5_input, S_IRUGO, show_in, NULL, 5),
	SENSOR_ATTR(in6_input, S_IRUGO, show_in, NULL, 6),
	SENSOR_ATTR(in7_input, S_IRUGO, show_in, NULL, 7),
};

/*
 * Driver interface
 */

static int ads1015_remove(struct i2c_client *client)
{
	struct ads1015_data *data = i2c_get_clientdata(client);
	int k;

	hwmon_device_unregister(data->hwmon_dev);
	for (k = 0; k < ADS1015_CHANNELS; ++k)
		device_remove_file(&client->dev, &ads1015_in[k].dev_attr);
<<<<<<< HEAD
	kfree(data);
=======
>>>>>>> refs/remotes/origin/master
	return 0;
}

#ifdef CONFIG_OF
static int ads1015_get_channels_config_of(struct i2c_client *client)
{
	struct ads1015_data *data = i2c_get_clientdata(client);
	struct device_node *node;

	if (!client->dev.of_node
	    || !of_get_next_child(client->dev.of_node, NULL))
		return -EINVAL;

	for_each_child_of_node(client->dev.of_node, node) {
		const __be32 *property;
		int len;
		unsigned int channel;
		unsigned int pga = ADS1015_DEFAULT_PGA;
		unsigned int data_rate = ADS1015_DEFAULT_DATA_RATE;

		property = of_get_property(node, "reg", &len);
		if (!property || len != sizeof(int)) {
			dev_err(&client->dev, "invalid reg on %s\n",
				node->full_name);
			continue;
		}

		channel = be32_to_cpup(property);
		if (channel > ADS1015_CHANNELS) {
			dev_err(&client->dev,
				"invalid channel index %d on %s\n",
				channel, node->full_name);
			continue;
		}

		property = of_get_property(node, "ti,gain", &len);
		if (property && len == sizeof(int)) {
			pga = be32_to_cpup(property);
			if (pga > 6) {
				dev_err(&client->dev,
					"invalid gain on %s\n",
					node->full_name);
			}
		}

		property = of_get_property(node, "ti,datarate", &len);
		if (property && len == sizeof(int)) {
			data_rate = be32_to_cpup(property);
			if (data_rate > 7) {
				dev_err(&client->dev,
					"invalid data_rate on %s\n",
					node->full_name);
			}
		}

		data->channel_data[channel].enabled = true;
		data->channel_data[channel].pga = pga;
		data->channel_data[channel].data_rate = data_rate;
	}

	return 0;
}
#endif

static void ads1015_get_channels_config(struct i2c_client *client)
{
	unsigned int k;
	struct ads1015_data *data = i2c_get_clientdata(client);
	struct ads1015_platform_data *pdata = dev_get_platdata(&client->dev);

	/* prefer platform data */
	if (pdata) {
		memcpy(data->channel_data, pdata->channel_data,
		       sizeof(data->channel_data));
		return;
	}

#ifdef CONFIG_OF
	if (!ads1015_get_channels_config_of(client))
		return;
#endif

	/* fallback on default configuration */
	for (k = 0; k < ADS1015_CHANNELS; ++k) {
		data->channel_data[k].enabled = true;
		data->channel_data[k].pga = ADS1015_DEFAULT_PGA;
		data->channel_data[k].data_rate = ADS1015_DEFAULT_DATA_RATE;
	}
}

static int ads1015_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	struct ads1015_data *data;
	int err;
	unsigned int k;

<<<<<<< HEAD
	data = kzalloc(sizeof(struct ads1015_data), GFP_KERNEL);
	if (!data) {
		err = -ENOMEM;
		goto exit;
	}

=======
	data = devm_kzalloc(&client->dev, sizeof(struct ads1015_data),
			    GFP_KERNEL);
	if (!data)
		return -ENOMEM;
	data->id = id->driver_data;
>>>>>>> refs/remotes/origin/master
	i2c_set_clientdata(client, data);
	mutex_init(&data->update_lock);

	/* build sysfs attribute group */
	ads1015_get_channels_config(client);
	for (k = 0; k < ADS1015_CHANNELS; ++k) {
		if (!data->channel_data[k].enabled)
			continue;
		err = device_create_file(&client->dev, &ads1015_in[k].dev_attr);
		if (err)
			goto exit_remove;
	}

	data->hwmon_dev = hwmon_device_register(&client->dev);
	if (IS_ERR(data->hwmon_dev)) {
		err = PTR_ERR(data->hwmon_dev);
		goto exit_remove;
	}

	return 0;

exit_remove:
	for (k = 0; k < ADS1015_CHANNELS; ++k)
		device_remove_file(&client->dev, &ads1015_in[k].dev_attr);
<<<<<<< HEAD
	kfree(data);
exit:
=======
>>>>>>> refs/remotes/origin/master
	return err;
}

static const struct i2c_device_id ads1015_id[] = {
<<<<<<< HEAD
	{ "ads1015", 0 },
=======
	{ "ads1015",  ads1015},
	{ "ads1115",  ads1115},
>>>>>>> refs/remotes/origin/master
	{ }
};
MODULE_DEVICE_TABLE(i2c, ads1015_id);

static struct i2c_driver ads1015_driver = {
	.driver = {
		.name = "ads1015",
	},
	.probe = ads1015_probe,
	.remove = ads1015_remove,
	.id_table = ads1015_id,
};

<<<<<<< HEAD
<<<<<<< HEAD
static int __init sensors_ads1015_init(void)
{
	return i2c_add_driver(&ads1015_driver);
}

static void __exit sensors_ads1015_exit(void)
{
	i2c_del_driver(&ads1015_driver);
}
=======
module_i2c_driver(ads1015_driver);
>>>>>>> refs/remotes/origin/cm-10.0
=======
module_i2c_driver(ads1015_driver);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Dirk Eibach <eibach@gdsys.de>");
MODULE_DESCRIPTION("ADS1015 driver");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
<<<<<<< HEAD

module_init(sensors_ads1015_init);
module_exit(sensors_ads1015_exit);
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
