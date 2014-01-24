/*
<<<<<<< HEAD
    Copyright (C) 2001-2004 Aurelien Jarno <aurelien@aurel32.net>
    Ported to Linux 2.6 by Aurelien Jarno <aurelien@aurel32.net> with
    the help of Jean Delvare <khali@linux-fr.org>

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
=======
 * Copyright (C) 2001-2004 Aurelien Jarno <aurelien@aurel32.net>
 * Ported to Linux 2.6 by Aurelien Jarno <aurelien@aurel32.net> with
 * the help of Jean Delvare <khali@linux-fr.org>
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
>>>>>>> refs/remotes/origin/cm-10.0

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/mutex.h>
#include <linux/err.h>
#include <linux/hwmon.h>

/* Insmod parameters */

static int input_mode;
module_param(input_mode, int, 0);
MODULE_PARM_DESC(input_mode,
	"Analog input mode:\n"
	" 0 = four single ended inputs\n"
	" 1 = three differential inputs\n"
	" 2 = single ended and differential mixed\n"
	" 3 = two differential inputs\n");

<<<<<<< HEAD
/* The PCF8591 control byte
      7    6    5    4    3    2    1    0
   |  0 |AOEF|   AIP   |  0 |AINC|  AICH   | */
=======
/*
 * The PCF8591 control byte
 *      7    6    5    4    3    2    1    0
 *   |  0 |AOEF|   AIP   |  0 |AINC|  AICH   |
 */
>>>>>>> refs/remotes/origin/cm-10.0

/* Analog Output Enable Flag (analog output active if 1) */
#define PCF8591_CONTROL_AOEF		0x40

<<<<<<< HEAD
/* Analog Input Programming
   0x00 = four single ended inputs
   0x10 = three differential inputs
   0x20 = single ended and differential mixed
   0x30 = two differential inputs */
=======
/*
 * Analog Input Programming
 * 0x00 = four single ended inputs
 * 0x10 = three differential inputs
 * 0x20 = single ended and differential mixed
 * 0x30 = two differential inputs
 */
>>>>>>> refs/remotes/origin/cm-10.0
#define PCF8591_CONTROL_AIP_MASK	0x30

/* Autoincrement Flag (switch on if 1) */
#define PCF8591_CONTROL_AINC		0x04

<<<<<<< HEAD
/* Channel selection
   0x00 = channel 0
   0x01 = channel 1
   0x02 = channel 2
   0x03 = channel 3 */
=======
/*
 * Channel selection
 * 0x00 = channel 0
 * 0x01 = channel 1
 * 0x02 = channel 2
 * 0x03 = channel 3
 */
>>>>>>> refs/remotes/origin/cm-10.0
#define PCF8591_CONTROL_AICH_MASK	0x03

/* Initial values */
#define PCF8591_INIT_CONTROL	((input_mode << 4) | PCF8591_CONTROL_AOEF)
#define PCF8591_INIT_AOUT	0	/* DAC out = 0 */

/* Conversions */
<<<<<<< HEAD
#define REG_TO_SIGNED(reg)	(((reg) & 0x80)?((reg) - 256):(reg))
=======
#define REG_TO_SIGNED(reg)	(((reg) & 0x80) ? ((reg) - 256) : (reg))
>>>>>>> refs/remotes/origin/cm-10.0

struct pcf8591_data {
	struct device *hwmon_dev;
	struct mutex update_lock;

	u8 control;
	u8 aout;
};

static void pcf8591_init_client(struct i2c_client *client);
static int pcf8591_read_channel(struct device *dev, int channel);

/* following are the sysfs callback functions */
#define show_in_channel(channel)					\
<<<<<<< HEAD
static ssize_t show_in##channel##_input(struct device *dev, struct device_attribute *attr, char *buf)	\
=======
static ssize_t show_in##channel##_input(struct device *dev,		\
					struct device_attribute *attr,	\
					char *buf)			\
>>>>>>> refs/remotes/origin/cm-10.0
{									\
	return sprintf(buf, "%d\n", pcf8591_read_channel(dev, channel));\
}									\
static DEVICE_ATTR(in##channel##_input, S_IRUGO,			\
		   show_in##channel##_input, NULL);

show_in_channel(0);
show_in_channel(1);
show_in_channel(2);
show_in_channel(3);

<<<<<<< HEAD
static ssize_t show_out0_ouput(struct device *dev, struct device_attribute *attr, char *buf)
=======
static ssize_t show_out0_ouput(struct device *dev,
			       struct device_attribute *attr, char *buf)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct pcf8591_data *data = i2c_get_clientdata(to_i2c_client(dev));
	return sprintf(buf, "%d\n", data->aout * 10);
}

<<<<<<< HEAD
static ssize_t set_out0_output(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	unsigned int value;
	struct i2c_client *client = to_i2c_client(dev);
	struct pcf8591_data *data = i2c_get_clientdata(client);
	if ((value = (simple_strtoul(buf, NULL, 10) + 5) / 10) <= 255) {
		data->aout = value;
		i2c_smbus_write_byte_data(client, data->control, data->aout);
		return count;
	}
	return -EINVAL;
=======
static ssize_t set_out0_output(struct device *dev,
			       struct device_attribute *attr,
			       const char *buf, size_t count)
{
	unsigned long val;
	struct i2c_client *client = to_i2c_client(dev);
	struct pcf8591_data *data = i2c_get_clientdata(client);
	int err;

	err = kstrtoul(buf, 10, &val);
	if (err)
		return err;

	val /= 10;
	if (val > 255)
		return -EINVAL;

	data->aout = val;
	i2c_smbus_write_byte_data(client, data->control, data->aout);
	return count;
>>>>>>> refs/remotes/origin/cm-10.0
}

static DEVICE_ATTR(out0_output, S_IWUSR | S_IRUGO,
		   show_out0_ouput, set_out0_output);

<<<<<<< HEAD
static ssize_t show_out0_enable(struct device *dev, struct device_attribute *attr, char *buf)
=======
static ssize_t show_out0_enable(struct device *dev,
				struct device_attribute *attr, char *buf)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct pcf8591_data *data = i2c_get_clientdata(to_i2c_client(dev));
	return sprintf(buf, "%u\n", !(!(data->control & PCF8591_CONTROL_AOEF)));
}

<<<<<<< HEAD
static ssize_t set_out0_enable(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct pcf8591_data *data = i2c_get_clientdata(client);
	unsigned long val = simple_strtoul(buf, NULL, 10);
=======
static ssize_t set_out0_enable(struct device *dev,
			       struct device_attribute *attr,
			       const char *buf, size_t count)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct pcf8591_data *data = i2c_get_clientdata(client);
	unsigned long val;
	int err;

	err = kstrtoul(buf, 10, &val);
	if (err)
		return err;
>>>>>>> refs/remotes/origin/cm-10.0

	mutex_lock(&data->update_lock);
	if (val)
		data->control |= PCF8591_CONTROL_AOEF;
	else
		data->control &= ~PCF8591_CONTROL_AOEF;
	i2c_smbus_write_byte(client, data->control);
	mutex_unlock(&data->update_lock);
	return count;
}

static DEVICE_ATTR(out0_enable, S_IWUSR | S_IRUGO,
		   show_out0_enable, set_out0_enable);

static struct attribute *pcf8591_attributes[] = {
	&dev_attr_out0_enable.attr,
	&dev_attr_out0_output.attr,
	&dev_attr_in0_input.attr,
	&dev_attr_in1_input.attr,
	NULL
};

static const struct attribute_group pcf8591_attr_group = {
	.attrs = pcf8591_attributes,
};

static struct attribute *pcf8591_attributes_opt[] = {
	&dev_attr_in2_input.attr,
	&dev_attr_in3_input.attr,
	NULL
};

static const struct attribute_group pcf8591_attr_group_opt = {
	.attrs = pcf8591_attributes_opt,
};

/*
 * Real code
 */

static int pcf8591_probe(struct i2c_client *client,
			 const struct i2c_device_id *id)
{
	struct pcf8591_data *data;
	int err;

<<<<<<< HEAD
	if (!(data = kzalloc(sizeof(struct pcf8591_data), GFP_KERNEL))) {
=======
	data = kzalloc(sizeof(struct pcf8591_data), GFP_KERNEL);
	if (!data) {
>>>>>>> refs/remotes/origin/cm-10.0
		err = -ENOMEM;
		goto exit;
	}

	i2c_set_clientdata(client, data);
	mutex_init(&data->update_lock);

	/* Initialize the PCF8591 chip */
	pcf8591_init_client(client);

	/* Register sysfs hooks */
	err = sysfs_create_group(&client->dev.kobj, &pcf8591_attr_group);
	if (err)
		goto exit_kfree;

	/* Register input2 if not in "two differential inputs" mode */
	if (input_mode != 3) {
<<<<<<< HEAD
		if ((err = device_create_file(&client->dev,
					      &dev_attr_in2_input)))
=======
		err = device_create_file(&client->dev, &dev_attr_in2_input);
		if (err)
>>>>>>> refs/remotes/origin/cm-10.0
			goto exit_sysfs_remove;
	}

	/* Register input3 only in "four single ended inputs" mode */
	if (input_mode == 0) {
<<<<<<< HEAD
		if ((err = device_create_file(&client->dev,
					      &dev_attr_in3_input)))
=======
		err = device_create_file(&client->dev, &dev_attr_in3_input);
		if (err)
>>>>>>> refs/remotes/origin/cm-10.0
			goto exit_sysfs_remove;
	}

	data->hwmon_dev = hwmon_device_register(&client->dev);
	if (IS_ERR(data->hwmon_dev)) {
		err = PTR_ERR(data->hwmon_dev);
		goto exit_sysfs_remove;
	}

	return 0;

exit_sysfs_remove:
	sysfs_remove_group(&client->dev.kobj, &pcf8591_attr_group_opt);
	sysfs_remove_group(&client->dev.kobj, &pcf8591_attr_group);
exit_kfree:
	kfree(data);
exit:
	return err;
}

static int pcf8591_remove(struct i2c_client *client)
{
	struct pcf8591_data *data = i2c_get_clientdata(client);

	hwmon_device_unregister(data->hwmon_dev);
	sysfs_remove_group(&client->dev.kobj, &pcf8591_attr_group_opt);
	sysfs_remove_group(&client->dev.kobj, &pcf8591_attr_group);
	kfree(i2c_get_clientdata(client));
	return 0;
}

/* Called when we have found a new PCF8591. */
static void pcf8591_init_client(struct i2c_client *client)
{
	struct pcf8591_data *data = i2c_get_clientdata(client);
	data->control = PCF8591_INIT_CONTROL;
	data->aout = PCF8591_INIT_AOUT;

	i2c_smbus_write_byte_data(client, data->control, data->aout);

<<<<<<< HEAD
	/* The first byte transmitted contains the conversion code of the
	   previous read cycle. FLUSH IT! */
=======
	/*
	 * The first byte transmitted contains the conversion code of the
	 * previous read cycle. FLUSH IT!
	 */
>>>>>>> refs/remotes/origin/cm-10.0
	i2c_smbus_read_byte(client);
}

static int pcf8591_read_channel(struct device *dev, int channel)
{
	u8 value;
	struct i2c_client *client = to_i2c_client(dev);
	struct pcf8591_data *data = i2c_get_clientdata(client);

	mutex_lock(&data->update_lock);

	if ((data->control & PCF8591_CONTROL_AICH_MASK) != channel) {
		data->control = (data->control & ~PCF8591_CONTROL_AICH_MASK)
			      | channel;
		i2c_smbus_write_byte(client, data->control);

<<<<<<< HEAD
		/* The first byte transmitted contains the conversion code of
		   the previous read cycle. FLUSH IT! */
=======
		/*
		 * The first byte transmitted contains the conversion code of
		 * the previous read cycle. FLUSH IT!
		 */
>>>>>>> refs/remotes/origin/cm-10.0
		i2c_smbus_read_byte(client);
	}
	value = i2c_smbus_read_byte(client);

	mutex_unlock(&data->update_lock);

	if ((channel == 2 && input_mode == 2) ||
	    (channel != 3 && (input_mode == 1 || input_mode == 3)))
<<<<<<< HEAD
		return (10 * REG_TO_SIGNED(value));
	else
		return (10 * value);
=======
		return 10 * REG_TO_SIGNED(value);
	else
		return 10 * value;
>>>>>>> refs/remotes/origin/cm-10.0
}

static const struct i2c_device_id pcf8591_id[] = {
	{ "pcf8591", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, pcf8591_id);

static struct i2c_driver pcf8591_driver = {
	.driver = {
		.name	= "pcf8591",
	},
	.probe		= pcf8591_probe,
	.remove		= pcf8591_remove,
	.id_table	= pcf8591_id,
};

static int __init pcf8591_init(void)
{
	if (input_mode < 0 || input_mode > 3) {
		pr_warn("invalid input_mode (%d)\n", input_mode);
		input_mode = 0;
	}
	return i2c_add_driver(&pcf8591_driver);
}

static void __exit pcf8591_exit(void)
{
	i2c_del_driver(&pcf8591_driver);
}

MODULE_AUTHOR("Aurelien Jarno <aurelien@aurel32.net>");
MODULE_DESCRIPTION("PCF8591 driver");
MODULE_LICENSE("GPL");

module_init(pcf8591_init);
module_exit(pcf8591_exit);
