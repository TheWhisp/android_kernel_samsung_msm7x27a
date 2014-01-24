/*
<<<<<<< HEAD
<<<<<<< HEAD
    lm78.c - Part of lm_sensors, Linux kernel modules for hardware
             monitoring
    Copyright (c) 1998, 1999  Frodo Looijaard <frodol@dds.nl> 
    Copyright (c) 2007        Jean Delvare <khali@linux-fr.org>

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
=======
>>>>>>> refs/remotes/origin/master
 * lm78.c - Part of lm_sensors, Linux kernel modules for hardware
 *	    monitoring
 * Copyright (c) 1998, 1999  Frodo Looijaard <frodol@dds.nl>
 * Copyright (c) 2007, 2011  Jean Delvare <khali@linux-fr.org>
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
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/i2c.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/platform_device.h>
#include <linux/ioport.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
#include <linux/hwmon.h>
#include <linux/hwmon-vid.h>
#include <linux/hwmon-sysfs.h>
#include <linux/err.h>
#include <linux/mutex.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/io.h>

/* ISA device, if found */
static struct platform_device *pdev;
=======
=======
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_ISA
#include <linux/platform_device.h>
#include <linux/ioport.h>
#include <linux/io.h>
#endif
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

/* Addresses to scan */
static const unsigned short normal_i2c[] = { 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d,
						0x2e, 0x2f, I2C_CLIENT_END };
<<<<<<< HEAD
<<<<<<< HEAD
static unsigned short isa_address = 0x290;

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
enum chips { lm78, lm79 };

/* Many LM78 constants specified below */

/* Length of ISA address segment */
#define LM78_EXTENT 8

/* Where are the ISA address/data registers relative to the base address */
#define LM78_ADDR_REG_OFFSET 5
#define LM78_DATA_REG_OFFSET 6

/* The LM78 registers */
#define LM78_REG_IN_MAX(nr) (0x2b + (nr) * 2)
#define LM78_REG_IN_MIN(nr) (0x2c + (nr) * 2)
#define LM78_REG_IN(nr) (0x20 + (nr))

#define LM78_REG_FAN_MIN(nr) (0x3b + (nr))
#define LM78_REG_FAN(nr) (0x28 + (nr))

#define LM78_REG_TEMP 0x27
#define LM78_REG_TEMP_OVER 0x39
#define LM78_REG_TEMP_HYST 0x3a

#define LM78_REG_ALARM1 0x41
#define LM78_REG_ALARM2 0x42

#define LM78_REG_VID_FANDIV 0x47

#define LM78_REG_CONFIG 0x40
#define LM78_REG_CHIPID 0x49
#define LM78_REG_I2C_ADDR 0x48


<<<<<<< HEAD
<<<<<<< HEAD
/* Conversions. Rounding and limit checking is only done on the TO_REG 
   variants. */

/* IN: mV, (0V to 4.08V)
   REG: 16mV/bit */
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * Conversions. Rounding and limit checking is only done on the TO_REG
 * variants.
 */

/*
 * IN: mV (0V to 4.08V)
 * REG: 16mV/bit
 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static inline u8 IN_TO_REG(unsigned long val)
{
	unsigned long nval = SENSORS_LIMIT(val, 0, 4080);
=======
static inline u8 IN_TO_REG(unsigned long val)
{
	unsigned long nval = clamp_val(val, 0, 4080);
>>>>>>> refs/remotes/origin/master
	return (nval + 8) / 16;
}
#define IN_FROM_REG(val) ((val) *  16)

static inline u8 FAN_TO_REG(long rpm, int div)
{
	if (rpm <= 0)
		return 255;
<<<<<<< HEAD
	return SENSORS_LIMIT((1350000 + rpm * div / 2) / (rpm * div), 1, 254);
=======
	if (rpm > 1350000)
		return 1;
	return clamp_val((1350000 + rpm * div / 2) / (rpm * div), 1, 254);
>>>>>>> refs/remotes/origin/master
}

static inline int FAN_FROM_REG(u8 val, int div)
{
<<<<<<< HEAD
<<<<<<< HEAD
	return val==0 ? -1 : val==255 ? 0 : 1350000/(val*div);
}

/* TEMP: mC (-128C to +127C)
   REG: 1C/bit, two's complement */
static inline s8 TEMP_TO_REG(int val)
{
	int nval = SENSORS_LIMIT(val, -128000, 127000) ;
	return nval<0 ? (nval-500)/1000 : (nval+500)/1000;
=======
=======
>>>>>>> refs/remotes/origin/master
	return val == 0 ? -1 : val == 255 ? 0 : 1350000 / (val * div);
}

/*
 * TEMP: mC (-128C to +127C)
 * REG: 1C/bit, two's complement
 */
static inline s8 TEMP_TO_REG(int val)
{
<<<<<<< HEAD
	int nval = SENSORS_LIMIT(val, -128000, 127000) ;
	return nval < 0 ? (nval - 500) / 1000 : (nval + 500) / 1000;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int nval = clamp_val(val, -128000, 127000) ;
	return nval < 0 ? (nval - 500) / 1000 : (nval + 500) / 1000;
>>>>>>> refs/remotes/origin/master
}

static inline int TEMP_FROM_REG(s8 val)
{
	return val * 1000;
}

#define DIV_FROM_REG(val) (1 << (val))

struct lm78_data {
	struct i2c_client *client;
	struct device *hwmon_dev;
	struct mutex lock;
	enum chips type;

	/* For ISA device only */
	const char *name;
	int isa_addr;

	struct mutex update_lock;
	char valid;		/* !=0 if following fields are valid */
	unsigned long last_updated;	/* In jiffies */

	u8 in[7];		/* Register value */
	u8 in_max[7];		/* Register value */
	u8 in_min[7];		/* Register value */
	u8 fan[3];		/* Register value */
	u8 fan_min[3];		/* Register value */
	s8 temp;		/* Register value */
	s8 temp_over;		/* Register value */
	s8 temp_hyst;		/* Register value */
	u8 fan_div[3];		/* Register encoding, shifted right */
	u8 vid;			/* Register encoding, combined */
	u16 alarms;		/* Register encoding, combined */
};


<<<<<<< HEAD
<<<<<<< HEAD
static int lm78_i2c_detect(struct i2c_client *client,
			   struct i2c_board_info *info);
static int lm78_i2c_probe(struct i2c_client *client,
			  const struct i2c_device_id *id);
static int lm78_i2c_remove(struct i2c_client *client);

static int __devinit lm78_isa_probe(struct platform_device *pdev);
static int __devexit lm78_isa_remove(struct platform_device *pdev);

=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int lm78_read_value(struct lm78_data *data, u8 reg);
static int lm78_write_value(struct lm78_data *data, u8 reg, u8 value);
static struct lm78_data *lm78_update_device(struct device *dev);
static void lm78_init_device(struct lm78_data *data);


<<<<<<< HEAD
<<<<<<< HEAD
static const struct i2c_device_id lm78_i2c_id[] = {
	{ "lm78", lm78 },
	{ "lm79", lm79 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, lm78_i2c_id);

static struct i2c_driver lm78_driver = {
	.class		= I2C_CLASS_HWMON,
	.driver = {
		.name	= "lm78",
	},
	.probe		= lm78_i2c_probe,
	.remove		= lm78_i2c_remove,
	.id_table	= lm78_i2c_id,
	.detect		= lm78_i2c_detect,
	.address_list	= normal_i2c,
};

static struct platform_driver lm78_isa_driver = {
	.driver = {
		.owner	= THIS_MODULE,
		.name	= "lm78",
	},
	.probe		= lm78_isa_probe,
	.remove		= __devexit_p(lm78_isa_remove),
};


=======
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/* 7 Voltages */
static ssize_t show_in(struct device *dev, struct device_attribute *da,
		       char *buf)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	struct lm78_data *data = lm78_update_device(dev);
	return sprintf(buf, "%d\n", IN_FROM_REG(data->in[attr->index]));
}

static ssize_t show_in_min(struct device *dev, struct device_attribute *da,
			   char *buf)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	struct lm78_data *data = lm78_update_device(dev);
	return sprintf(buf, "%d\n", IN_FROM_REG(data->in_min[attr->index]));
}

static ssize_t show_in_max(struct device *dev, struct device_attribute *da,
			   char *buf)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	struct lm78_data *data = lm78_update_device(dev);
	return sprintf(buf, "%d\n", IN_FROM_REG(data->in_max[attr->index]));
}

static ssize_t set_in_min(struct device *dev, struct device_attribute *da,
			  const char *buf, size_t count)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	struct lm78_data *data = dev_get_drvdata(dev);
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long val = simple_strtoul(buf, NULL, 10);
	int nr = attr->index;
=======
=======
>>>>>>> refs/remotes/origin/master
	int nr = attr->index;
	unsigned long val;
	int err;

	err = kstrtoul(buf, 10, &val);
	if (err)
		return err;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	mutex_lock(&data->update_lock);
	data->in_min[nr] = IN_TO_REG(val);
	lm78_write_value(data, LM78_REG_IN_MIN(nr), data->in_min[nr]);
	mutex_unlock(&data->update_lock);
	return count;
}

static ssize_t set_in_max(struct device *dev, struct device_attribute *da,
			  const char *buf, size_t count)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	struct lm78_data *data = dev_get_drvdata(dev);
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long val = simple_strtoul(buf, NULL, 10);
	int nr = attr->index;
=======
=======
>>>>>>> refs/remotes/origin/master
	int nr = attr->index;
	unsigned long val;
	int err;

	err = kstrtoul(buf, 10, &val);
	if (err)
		return err;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	mutex_lock(&data->update_lock);
	data->in_max[nr] = IN_TO_REG(val);
	lm78_write_value(data, LM78_REG_IN_MAX(nr), data->in_max[nr]);
	mutex_unlock(&data->update_lock);
	return count;
}
<<<<<<< HEAD
<<<<<<< HEAD
	
=======

>>>>>>> refs/remotes/origin/cm-10.0
=======

>>>>>>> refs/remotes/origin/master
#define show_in_offset(offset)					\
static SENSOR_DEVICE_ATTR(in##offset##_input, S_IRUGO,		\
		show_in, NULL, offset);				\
static SENSOR_DEVICE_ATTR(in##offset##_min, S_IRUGO | S_IWUSR,	\
		show_in_min, set_in_min, offset);		\
static SENSOR_DEVICE_ATTR(in##offset##_max, S_IRUGO | S_IWUSR,	\
		show_in_max, set_in_max, offset);

show_in_offset(0);
show_in_offset(1);
show_in_offset(2);
show_in_offset(3);
show_in_offset(4);
show_in_offset(5);
show_in_offset(6);

/* Temperature */
static ssize_t show_temp(struct device *dev, struct device_attribute *da,
			 char *buf)
{
	struct lm78_data *data = lm78_update_device(dev);
	return sprintf(buf, "%d\n", TEMP_FROM_REG(data->temp));
}

static ssize_t show_temp_over(struct device *dev, struct device_attribute *da,
			      char *buf)
{
	struct lm78_data *data = lm78_update_device(dev);
	return sprintf(buf, "%d\n", TEMP_FROM_REG(data->temp_over));
}

static ssize_t set_temp_over(struct device *dev, struct device_attribute *da,
			     const char *buf, size_t count)
{
	struct lm78_data *data = dev_get_drvdata(dev);
<<<<<<< HEAD
<<<<<<< HEAD
	long val = simple_strtol(buf, NULL, 10);
=======
=======
>>>>>>> refs/remotes/origin/master
	long val;
	int err;

	err = kstrtol(buf, 10, &val);
	if (err)
		return err;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	mutex_lock(&data->update_lock);
	data->temp_over = TEMP_TO_REG(val);
	lm78_write_value(data, LM78_REG_TEMP_OVER, data->temp_over);
	mutex_unlock(&data->update_lock);
	return count;
}

static ssize_t show_temp_hyst(struct device *dev, struct device_attribute *da,
			      char *buf)
{
	struct lm78_data *data = lm78_update_device(dev);
	return sprintf(buf, "%d\n", TEMP_FROM_REG(data->temp_hyst));
}

static ssize_t set_temp_hyst(struct device *dev, struct device_attribute *da,
			     const char *buf, size_t count)
{
	struct lm78_data *data = dev_get_drvdata(dev);
<<<<<<< HEAD
<<<<<<< HEAD
	long val = simple_strtol(buf, NULL, 10);
=======
=======
>>>>>>> refs/remotes/origin/master
	long val;
	int err;

	err = kstrtol(buf, 10, &val);
	if (err)
		return err;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	mutex_lock(&data->update_lock);
	data->temp_hyst = TEMP_TO_REG(val);
	lm78_write_value(data, LM78_REG_TEMP_HYST, data->temp_hyst);
	mutex_unlock(&data->update_lock);
	return count;
}

static DEVICE_ATTR(temp1_input, S_IRUGO, show_temp, NULL);
static DEVICE_ATTR(temp1_max, S_IRUGO | S_IWUSR,
		show_temp_over, set_temp_over);
static DEVICE_ATTR(temp1_max_hyst, S_IRUGO | S_IWUSR,
		show_temp_hyst, set_temp_hyst);

/* 3 Fans */
static ssize_t show_fan(struct device *dev, struct device_attribute *da,
			char *buf)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	struct lm78_data *data = lm78_update_device(dev);
	int nr = attr->index;
	return sprintf(buf, "%d\n", FAN_FROM_REG(data->fan[nr],
<<<<<<< HEAD
<<<<<<< HEAD
		DIV_FROM_REG(data->fan_div[nr])) );
=======
		DIV_FROM_REG(data->fan_div[nr])));
>>>>>>> refs/remotes/origin/cm-10.0
=======
		DIV_FROM_REG(data->fan_div[nr])));
>>>>>>> refs/remotes/origin/master
}

static ssize_t show_fan_min(struct device *dev, struct device_attribute *da,
			    char *buf)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	struct lm78_data *data = lm78_update_device(dev);
	int nr = attr->index;
<<<<<<< HEAD
<<<<<<< HEAD
	return sprintf(buf,"%d\n", FAN_FROM_REG(data->fan_min[nr],
		DIV_FROM_REG(data->fan_div[nr])) );
=======
	return sprintf(buf, "%d\n", FAN_FROM_REG(data->fan_min[nr],
		DIV_FROM_REG(data->fan_div[nr])));
>>>>>>> refs/remotes/origin/cm-10.0
=======
	return sprintf(buf, "%d\n", FAN_FROM_REG(data->fan_min[nr],
		DIV_FROM_REG(data->fan_div[nr])));
>>>>>>> refs/remotes/origin/master
}

static ssize_t set_fan_min(struct device *dev, struct device_attribute *da,
			   const char *buf, size_t count)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	struct lm78_data *data = dev_get_drvdata(dev);
	int nr = attr->index;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long val = simple_strtoul(buf, NULL, 10);
=======
=======
>>>>>>> refs/remotes/origin/master
	unsigned long val;
	int err;

	err = kstrtoul(buf, 10, &val);
	if (err)
		return err;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	mutex_lock(&data->update_lock);
	data->fan_min[nr] = FAN_TO_REG(val, DIV_FROM_REG(data->fan_div[nr]));
	lm78_write_value(data, LM78_REG_FAN_MIN(nr), data->fan_min[nr]);
	mutex_unlock(&data->update_lock);
	return count;
}

static ssize_t show_fan_div(struct device *dev, struct device_attribute *da,
			    char *buf)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	struct lm78_data *data = lm78_update_device(dev);
	return sprintf(buf, "%d\n", DIV_FROM_REG(data->fan_div[attr->index]));
}

<<<<<<< HEAD
<<<<<<< HEAD
/* Note: we save and restore the fan minimum here, because its value is
   determined in part by the fan divisor.  This follows the principle of
   least surprise; the user doesn't expect the fan minimum to change just
   because the divisor changed. */
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * Note: we save and restore the fan minimum here, because its value is
 * determined in part by the fan divisor.  This follows the principle of
 * least surprise; the user doesn't expect the fan minimum to change just
 * because the divisor changed.
 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static ssize_t set_fan_div(struct device *dev, struct device_attribute *da,
			   const char *buf, size_t count)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	struct lm78_data *data = dev_get_drvdata(dev);
	int nr = attr->index;
<<<<<<< HEAD
<<<<<<< HEAD
	unsigned long val = simple_strtoul(buf, NULL, 10);
	unsigned long min;
	u8 reg;
=======
=======
>>>>>>> refs/remotes/origin/master
	unsigned long min;
	u8 reg;
	unsigned long val;
	int err;

	err = kstrtoul(buf, 10, &val);
	if (err)
		return err;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

	mutex_lock(&data->update_lock);
	min = FAN_FROM_REG(data->fan_min[nr],
			   DIV_FROM_REG(data->fan_div[nr]));

	switch (val) {
<<<<<<< HEAD
<<<<<<< HEAD
	case 1: data->fan_div[nr] = 0; break;
	case 2: data->fan_div[nr] = 1; break;
	case 4: data->fan_div[nr] = 2; break;
	case 8: data->fan_div[nr] = 3; break;
=======
=======
>>>>>>> refs/remotes/origin/master
	case 1:
		data->fan_div[nr] = 0;
		break;
	case 2:
		data->fan_div[nr] = 1;
		break;
	case 4:
		data->fan_div[nr] = 2;
		break;
	case 8:
		data->fan_div[nr] = 3;
		break;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
	default:
		dev_err(dev, "fan_div value %ld not "
			"supported. Choose one of 1, 2, 4 or 8!\n", val);
=======
	default:
		dev_err(dev,
			"fan_div value %ld not supported. Choose one of 1, 2, 4 or 8!\n",
			val);
>>>>>>> refs/remotes/origin/master
		mutex_unlock(&data->update_lock);
		return -EINVAL;
	}

	reg = lm78_read_value(data, LM78_REG_VID_FANDIV);
	switch (nr) {
	case 0:
		reg = (reg & 0xcf) | (data->fan_div[nr] << 4);
		break;
	case 1:
		reg = (reg & 0x3f) | (data->fan_div[nr] << 6);
		break;
	}
	lm78_write_value(data, LM78_REG_VID_FANDIV, reg);

	data->fan_min[nr] =
		FAN_TO_REG(min, DIV_FROM_REG(data->fan_div[nr]));
	lm78_write_value(data, LM78_REG_FAN_MIN(nr), data->fan_min[nr]);
	mutex_unlock(&data->update_lock);

	return count;
}

#define show_fan_offset(offset)				\
static SENSOR_DEVICE_ATTR(fan##offset##_input, S_IRUGO,		\
		show_fan, NULL, offset - 1);			\
static SENSOR_DEVICE_ATTR(fan##offset##_min, S_IRUGO | S_IWUSR,	\
		show_fan_min, set_fan_min, offset - 1);

show_fan_offset(1);
show_fan_offset(2);
show_fan_offset(3);

/* Fan 3 divisor is locked in H/W */
static SENSOR_DEVICE_ATTR(fan1_div, S_IRUGO | S_IWUSR,
		show_fan_div, set_fan_div, 0);
static SENSOR_DEVICE_ATTR(fan2_div, S_IRUGO | S_IWUSR,
		show_fan_div, set_fan_div, 1);
static SENSOR_DEVICE_ATTR(fan3_div, S_IRUGO, show_fan_div, NULL, 2);

/* VID */
static ssize_t show_vid(struct device *dev, struct device_attribute *da,
			char *buf)
{
	struct lm78_data *data = lm78_update_device(dev);
	return sprintf(buf, "%d\n", vid_from_reg(data->vid, 82));
}
static DEVICE_ATTR(cpu0_vid, S_IRUGO, show_vid, NULL);

/* Alarms */
static ssize_t show_alarms(struct device *dev, struct device_attribute *da,
			   char *buf)
{
	struct lm78_data *data = lm78_update_device(dev);
	return sprintf(buf, "%u\n", data->alarms);
}
static DEVICE_ATTR(alarms, S_IRUGO, show_alarms, NULL);

static ssize_t show_alarm(struct device *dev, struct device_attribute *da,
			  char *buf)
{
	struct lm78_data *data = lm78_update_device(dev);
	int nr = to_sensor_dev_attr(da)->index;
	return sprintf(buf, "%u\n", (data->alarms >> nr) & 1);
}
static SENSOR_DEVICE_ATTR(in0_alarm, S_IRUGO, show_alarm, NULL, 0);
static SENSOR_DEVICE_ATTR(in1_alarm, S_IRUGO, show_alarm, NULL, 1);
static SENSOR_DEVICE_ATTR(in2_alarm, S_IRUGO, show_alarm, NULL, 2);
static SENSOR_DEVICE_ATTR(in3_alarm, S_IRUGO, show_alarm, NULL, 3);
static SENSOR_DEVICE_ATTR(in4_alarm, S_IRUGO, show_alarm, NULL, 8);
static SENSOR_DEVICE_ATTR(in5_alarm, S_IRUGO, show_alarm, NULL, 9);
static SENSOR_DEVICE_ATTR(in6_alarm, S_IRUGO, show_alarm, NULL, 10);
static SENSOR_DEVICE_ATTR(fan1_alarm, S_IRUGO, show_alarm, NULL, 6);
static SENSOR_DEVICE_ATTR(fan2_alarm, S_IRUGO, show_alarm, NULL, 7);
static SENSOR_DEVICE_ATTR(fan3_alarm, S_IRUGO, show_alarm, NULL, 11);
static SENSOR_DEVICE_ATTR(temp1_alarm, S_IRUGO, show_alarm, NULL, 4);

static struct attribute *lm78_attributes[] = {
	&sensor_dev_attr_in0_input.dev_attr.attr,
	&sensor_dev_attr_in0_min.dev_attr.attr,
	&sensor_dev_attr_in0_max.dev_attr.attr,
	&sensor_dev_attr_in0_alarm.dev_attr.attr,
	&sensor_dev_attr_in1_input.dev_attr.attr,
	&sensor_dev_attr_in1_min.dev_attr.attr,
	&sensor_dev_attr_in1_max.dev_attr.attr,
	&sensor_dev_attr_in1_alarm.dev_attr.attr,
	&sensor_dev_attr_in2_input.dev_attr.attr,
	&sensor_dev_attr_in2_min.dev_attr.attr,
	&sensor_dev_attr_in2_max.dev_attr.attr,
	&sensor_dev_attr_in2_alarm.dev_attr.attr,
	&sensor_dev_attr_in3_input.dev_attr.attr,
	&sensor_dev_attr_in3_min.dev_attr.attr,
	&sensor_dev_attr_in3_max.dev_attr.attr,
	&sensor_dev_attr_in3_alarm.dev_attr.attr,
	&sensor_dev_attr_in4_input.dev_attr.attr,
	&sensor_dev_attr_in4_min.dev_attr.attr,
	&sensor_dev_attr_in4_max.dev_attr.attr,
	&sensor_dev_attr_in4_alarm.dev_attr.attr,
	&sensor_dev_attr_in5_input.dev_attr.attr,
	&sensor_dev_attr_in5_min.dev_attr.attr,
	&sensor_dev_attr_in5_max.dev_attr.attr,
	&sensor_dev_attr_in5_alarm.dev_attr.attr,
	&sensor_dev_attr_in6_input.dev_attr.attr,
	&sensor_dev_attr_in6_min.dev_attr.attr,
	&sensor_dev_attr_in6_max.dev_attr.attr,
	&sensor_dev_attr_in6_alarm.dev_attr.attr,
	&dev_attr_temp1_input.attr,
	&dev_attr_temp1_max.attr,
	&dev_attr_temp1_max_hyst.attr,
	&sensor_dev_attr_temp1_alarm.dev_attr.attr,
	&sensor_dev_attr_fan1_input.dev_attr.attr,
	&sensor_dev_attr_fan1_min.dev_attr.attr,
	&sensor_dev_attr_fan1_div.dev_attr.attr,
	&sensor_dev_attr_fan1_alarm.dev_attr.attr,
	&sensor_dev_attr_fan2_input.dev_attr.attr,
	&sensor_dev_attr_fan2_min.dev_attr.attr,
	&sensor_dev_attr_fan2_div.dev_attr.attr,
	&sensor_dev_attr_fan2_alarm.dev_attr.attr,
	&sensor_dev_attr_fan3_input.dev_attr.attr,
	&sensor_dev_attr_fan3_min.dev_attr.attr,
	&sensor_dev_attr_fan3_div.dev_attr.attr,
	&sensor_dev_attr_fan3_alarm.dev_attr.attr,
	&dev_attr_alarms.attr,
	&dev_attr_cpu0_vid.attr,

	NULL
};

static const struct attribute_group lm78_group = {
	.attrs = lm78_attributes,
};

<<<<<<< HEAD
<<<<<<< HEAD
/* I2C devices get this name attribute automatically, but for ISA devices
   we must create it by ourselves. */
=======
=======
>>>>>>> refs/remotes/origin/master
/*
 * ISA related code
 */
#ifdef CONFIG_ISA

/* ISA device, if found */
static struct platform_device *pdev;

static unsigned short isa_address = 0x290;

/*
 * I2C devices get this name attribute automatically, but for ISA devices
 * we must create it by ourselves.
 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static ssize_t show_name(struct device *dev, struct device_attribute
			 *devattr, char *buf)
{
	struct lm78_data *data = dev_get_drvdata(dev);

	return sprintf(buf, "%s\n", data->name);
}
static DEVICE_ATTR(name, S_IRUGO, show_name, NULL);

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static struct lm78_data *lm78_data_if_isa(void)
{
	return pdev ? platform_get_drvdata(pdev) : NULL;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
/* Returns 1 if the I2C chip appears to be an alias of the ISA chip */
static int lm78_alias_detect(struct i2c_client *client, u8 chipid)
{
	struct lm78_data *isa;
	int i;

	if (!pdev)	/* No ISA chip */
		return 0;
	isa = platform_get_drvdata(pdev);

	if (lm78_read_value(isa, LM78_REG_I2C_ADDR) != client->addr)
		return 0;	/* Address doesn't match */
	if ((lm78_read_value(isa, LM78_REG_CHIPID) & 0xfe) != (chipid & 0xfe))
		return 0;	/* Chip type doesn't match */

<<<<<<< HEAD
<<<<<<< HEAD
	/* We compare all the limit registers, the config register and the
	 * interrupt mask registers */
=======
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * We compare all the limit registers, the config register and the
	 * interrupt mask registers
	 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	for (i = 0x2b; i <= 0x3d; i++) {
		if (lm78_read_value(isa, i) !=
		    i2c_smbus_read_byte_data(client, i))
			return 0;
	}
	if (lm78_read_value(isa, LM78_REG_CONFIG) !=
	    i2c_smbus_read_byte_data(client, LM78_REG_CONFIG))
		return 0;
	for (i = 0x43; i <= 0x46; i++) {
		if (lm78_read_value(isa, i) !=
		    i2c_smbus_read_byte_data(client, i))
			return 0;
	}

	return 1;
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
#else /* !CONFIG_ISA */

static int lm78_alias_detect(struct i2c_client *client, u8 chipid)
{
	return 0;
}

static struct lm78_data *lm78_data_if_isa(void)
{
	return NULL;
}
#endif /* CONFIG_ISA */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master

static int lm78_i2c_detect(struct i2c_client *client,
			   struct i2c_board_info *info)
{
	int i;
<<<<<<< HEAD
<<<<<<< HEAD
	struct lm78_data *isa = pdev ? platform_get_drvdata(pdev) : NULL;
=======
	struct lm78_data *isa = lm78_data_if_isa();
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct lm78_data *isa = lm78_data_if_isa();
>>>>>>> refs/remotes/origin/master
	const char *client_name;
	struct i2c_adapter *adapter = client->adapter;
	int address = client->addr;

	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA))
		return -ENODEV;

<<<<<<< HEAD
<<<<<<< HEAD
	/* We block updates of the ISA device to minimize the risk of
	   concurrent access to the same LM78 chip through different
	   interfaces. */
=======
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * We block updates of the ISA device to minimize the risk of
	 * concurrent access to the same LM78 chip through different
	 * interfaces.
	 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (isa)
		mutex_lock(&isa->update_lock);

	if ((i2c_smbus_read_byte_data(client, LM78_REG_CONFIG) & 0x80)
	 || i2c_smbus_read_byte_data(client, LM78_REG_I2C_ADDR) != address)
		goto err_nodev;

	/* Explicitly prevent the misdetection of Winbond chips */
	i = i2c_smbus_read_byte_data(client, 0x4f);
	if (i == 0xa3 || i == 0x5c)
		goto err_nodev;

	/* Determine the chip type. */
	i = i2c_smbus_read_byte_data(client, LM78_REG_CHIPID);
	if (i == 0x00 || i == 0x20	/* LM78 */
	 || i == 0x40)			/* LM78-J */
		client_name = "lm78";
	else if ((i & 0xfe) == 0xc0)
		client_name = "lm79";
	else
		goto err_nodev;

	if (lm78_alias_detect(client, i)) {
<<<<<<< HEAD
		dev_dbg(&adapter->dev, "Device at 0x%02x appears to "
			"be the same as ISA device\n", address);
=======
		dev_dbg(&adapter->dev,
			"Device at 0x%02x appears to be the same as ISA device\n",
			address);
>>>>>>> refs/remotes/origin/master
		goto err_nodev;
	}

	if (isa)
		mutex_unlock(&isa->update_lock);

	strlcpy(info->type, client_name, I2C_NAME_SIZE);

	return 0;

 err_nodev:
	if (isa)
		mutex_unlock(&isa->update_lock);
	return -ENODEV;
}

static int lm78_i2c_probe(struct i2c_client *client,
			  const struct i2c_device_id *id)
{
	struct lm78_data *data;
	int err;

<<<<<<< HEAD
	data = kzalloc(sizeof(struct lm78_data), GFP_KERNEL);
=======
	data = devm_kzalloc(&client->dev, sizeof(struct lm78_data), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!data)
		return -ENOMEM;

	i2c_set_clientdata(client, data);
	data->client = client;
	data->type = id->driver_data;

	/* Initialize the LM78 chip */
	lm78_init_device(data);

	/* Register sysfs hooks */
	err = sysfs_create_group(&client->dev.kobj, &lm78_group);
	if (err)
<<<<<<< HEAD
		goto ERROR3;
=======
		return err;
>>>>>>> refs/remotes/origin/master

	data->hwmon_dev = hwmon_device_register(&client->dev);
	if (IS_ERR(data->hwmon_dev)) {
		err = PTR_ERR(data->hwmon_dev);
<<<<<<< HEAD
		goto ERROR4;
=======
		goto error;
>>>>>>> refs/remotes/origin/master
	}

	return 0;

<<<<<<< HEAD
ERROR4:
	sysfs_remove_group(&client->dev.kobj, &lm78_group);
ERROR3:
	kfree(data);
=======
error:
	sysfs_remove_group(&client->dev.kobj, &lm78_group);
>>>>>>> refs/remotes/origin/master
	return err;
}

static int lm78_i2c_remove(struct i2c_client *client)
{
	struct lm78_data *data = i2c_get_clientdata(client);

	hwmon_device_unregister(data->hwmon_dev);
	sysfs_remove_group(&client->dev.kobj, &lm78_group);
<<<<<<< HEAD
	kfree(data);

	return 0;
}

<<<<<<< HEAD
static int __devinit lm78_isa_probe(struct platform_device *pdev)
{
	int err;
	struct lm78_data *data;
	struct resource *res;

	/* Reserve the ISA region */
	res = platform_get_resource(pdev, IORESOURCE_IO, 0);
	if (!request_region(res->start + LM78_ADDR_REG_OFFSET, 2, "lm78")) {
		err = -EBUSY;
		goto exit;
	}

	if (!(data = kzalloc(sizeof(struct lm78_data), GFP_KERNEL))) {
		err = -ENOMEM;
		goto exit_release_region;
	}
	mutex_init(&data->lock);
	data->isa_addr = res->start;
	platform_set_drvdata(pdev, data);

	if (lm78_read_value(data, LM78_REG_CHIPID) & 0x80) {
		data->type = lm79;
		data->name = "lm79";
	} else {
		data->type = lm78;
		data->name = "lm78";
	}

	/* Initialize the LM78 chip */
	lm78_init_device(data);

	/* Register sysfs hooks */
	if ((err = sysfs_create_group(&pdev->dev.kobj, &lm78_group))
	 || (err = device_create_file(&pdev->dev, &dev_attr_name)))
		goto exit_remove_files;

	data->hwmon_dev = hwmon_device_register(&pdev->dev);
	if (IS_ERR(data->hwmon_dev)) {
		err = PTR_ERR(data->hwmon_dev);
		goto exit_remove_files;
	}

	return 0;

 exit_remove_files:
	sysfs_remove_group(&pdev->dev.kobj, &lm78_group);
	device_remove_file(&pdev->dev, &dev_attr_name);
	kfree(data);
 exit_release_region:
	release_region(res->start + LM78_ADDR_REG_OFFSET, 2);
 exit:
	return err;
}

static int __devexit lm78_isa_remove(struct platform_device *pdev)
{
	struct lm78_data *data = platform_get_drvdata(pdev);
	struct resource *res;

	hwmon_device_unregister(data->hwmon_dev);
	sysfs_remove_group(&pdev->dev.kobj, &lm78_group);
	device_remove_file(&pdev->dev, &dev_attr_name);
	kfree(data);

	res = platform_get_resource(pdev, IORESOURCE_IO, 0);
	release_region(res->start + LM78_ADDR_REG_OFFSET, 2);
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
/* The SMBus locks itself, but ISA access must be locked explicitly! 
   We don't want to lock the whole ISA bus, so we lock each client
   separately.
   We ignore the LM78 BUSY flag at this moment - it could lead to deadlocks,
   would slow down the LM78 access and should not be necessary.  */
=======
=======
>>>>>>> refs/remotes/origin/master
static const struct i2c_device_id lm78_i2c_id[] = {
	{ "lm78", lm78 },
	{ "lm79", lm79 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, lm78_i2c_id);

static struct i2c_driver lm78_driver = {
	.class		= I2C_CLASS_HWMON,
	.driver = {
		.name	= "lm78",
	},
	.probe		= lm78_i2c_probe,
	.remove		= lm78_i2c_remove,
	.id_table	= lm78_i2c_id,
	.detect		= lm78_i2c_detect,
	.address_list	= normal_i2c,
};

/*
 * The SMBus locks itself, but ISA access must be locked explicitly!
 * We don't want to lock the whole ISA bus, so we lock each client
 * separately.
 * We ignore the LM78 BUSY flag at this moment - it could lead to deadlocks,
 * would slow down the LM78 access and should not be necessary.
 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int lm78_read_value(struct lm78_data *data, u8 reg)
{
	struct i2c_client *client = data->client;

<<<<<<< HEAD
<<<<<<< HEAD
=======
#ifdef CONFIG_ISA
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef CONFIG_ISA
>>>>>>> refs/remotes/origin/master
	if (!client) { /* ISA device */
		int res;
		mutex_lock(&data->lock);
		outb_p(reg, data->isa_addr + LM78_ADDR_REG_OFFSET);
		res = inb_p(data->isa_addr + LM78_DATA_REG_OFFSET);
		mutex_unlock(&data->lock);
		return res;
	} else
<<<<<<< HEAD
<<<<<<< HEAD
		return i2c_smbus_read_byte_data(client, reg);
}

/* The SMBus locks itself, but ISA access muse be locked explicitly! 
   We don't want to lock the whole ISA bus, so we lock each client
   separately.
   We ignore the LM78 BUSY flag at this moment - it could lead to deadlocks,
   would slow down the LM78 access and should not be necessary. 
   There are some ugly typecasts here, but the good new is - they should
   nowhere else be necessary! */
=======
=======
>>>>>>> refs/remotes/origin/master
#endif
		return i2c_smbus_read_byte_data(client, reg);
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
static int lm78_write_value(struct lm78_data *data, u8 reg, u8 value)
{
	struct i2c_client *client = data->client;

<<<<<<< HEAD
<<<<<<< HEAD
=======
#ifdef CONFIG_ISA
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef CONFIG_ISA
>>>>>>> refs/remotes/origin/master
	if (!client) { /* ISA device */
		mutex_lock(&data->lock);
		outb_p(reg, data->isa_addr + LM78_ADDR_REG_OFFSET);
		outb_p(value, data->isa_addr + LM78_DATA_REG_OFFSET);
		mutex_unlock(&data->lock);
		return 0;
	} else
<<<<<<< HEAD
<<<<<<< HEAD
=======
#endif
>>>>>>> refs/remotes/origin/cm-10.0
=======
#endif
>>>>>>> refs/remotes/origin/master
		return i2c_smbus_write_byte_data(client, reg, value);
}

static void lm78_init_device(struct lm78_data *data)
{
	u8 config;
	int i;

	/* Start monitoring */
	config = lm78_read_value(data, LM78_REG_CONFIG);
	if ((config & 0x09) != 0x01)
		lm78_write_value(data, LM78_REG_CONFIG,
				 (config & 0xf7) | 0x01);

	/* A few vars need to be filled upon startup */
	for (i = 0; i < 3; i++) {
		data->fan_min[i] = lm78_read_value(data,
					LM78_REG_FAN_MIN(i));
	}

	mutex_init(&data->update_lock);
}

static struct lm78_data *lm78_update_device(struct device *dev)
{
	struct lm78_data *data = dev_get_drvdata(dev);
	int i;

	mutex_lock(&data->update_lock);

	if (time_after(jiffies, data->last_updated + HZ + HZ / 2)
	    || !data->valid) {

		dev_dbg(dev, "Starting lm78 update\n");

		for (i = 0; i <= 6; i++) {
			data->in[i] =
			    lm78_read_value(data, LM78_REG_IN(i));
			data->in_min[i] =
			    lm78_read_value(data, LM78_REG_IN_MIN(i));
			data->in_max[i] =
			    lm78_read_value(data, LM78_REG_IN_MAX(i));
		}
		for (i = 0; i < 3; i++) {
			data->fan[i] =
			    lm78_read_value(data, LM78_REG_FAN(i));
			data->fan_min[i] =
			    lm78_read_value(data, LM78_REG_FAN_MIN(i));
		}
		data->temp = lm78_read_value(data, LM78_REG_TEMP);
		data->temp_over =
		    lm78_read_value(data, LM78_REG_TEMP_OVER);
		data->temp_hyst =
		    lm78_read_value(data, LM78_REG_TEMP_HYST);
		i = lm78_read_value(data, LM78_REG_VID_FANDIV);
		data->vid = i & 0x0f;
		if (data->type == lm79)
			data->vid |=
			    (lm78_read_value(data, LM78_REG_CHIPID) &
			     0x01) << 4;
		else
			data->vid |= 0x10;
		data->fan_div[0] = (i >> 4) & 0x03;
		data->fan_div[1] = i >> 6;
		data->alarms = lm78_read_value(data, LM78_REG_ALARM1) +
		    (lm78_read_value(data, LM78_REG_ALARM2) << 8);
		data->last_updated = jiffies;
		data->valid = 1;

		data->fan_div[2] = 1;
	}

	mutex_unlock(&data->update_lock);

	return data;
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
#ifdef CONFIG_ISA
static int __devinit lm78_isa_probe(struct platform_device *pdev)
=======
#ifdef CONFIG_ISA
static int lm78_isa_probe(struct platform_device *pdev)
>>>>>>> refs/remotes/origin/master
{
	int err;
	struct lm78_data *data;
	struct resource *res;

	/* Reserve the ISA region */
	res = platform_get_resource(pdev, IORESOURCE_IO, 0);
<<<<<<< HEAD
	if (!request_region(res->start + LM78_ADDR_REG_OFFSET, 2, "lm78")) {
		err = -EBUSY;
		goto exit;
	}

	data = kzalloc(sizeof(struct lm78_data), GFP_KERNEL);
	if (!data) {
		err = -ENOMEM;
		goto exit_release_region;
	}
=======
	if (!devm_request_region(&pdev->dev, res->start + LM78_ADDR_REG_OFFSET,
				 2, "lm78"))
		return -EBUSY;

	data = devm_kzalloc(&pdev->dev, sizeof(struct lm78_data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

>>>>>>> refs/remotes/origin/master
	mutex_init(&data->lock);
	data->isa_addr = res->start;
	platform_set_drvdata(pdev, data);

	if (lm78_read_value(data, LM78_REG_CHIPID) & 0x80) {
		data->type = lm79;
		data->name = "lm79";
	} else {
		data->type = lm78;
		data->name = "lm78";
	}

	/* Initialize the LM78 chip */
	lm78_init_device(data);

	/* Register sysfs hooks */
	err = sysfs_create_group(&pdev->dev.kobj, &lm78_group);
	if (err)
		goto exit_remove_files;
	err = device_create_file(&pdev->dev, &dev_attr_name);
	if (err)
		goto exit_remove_files;

	data->hwmon_dev = hwmon_device_register(&pdev->dev);
	if (IS_ERR(data->hwmon_dev)) {
		err = PTR_ERR(data->hwmon_dev);
		goto exit_remove_files;
	}

	return 0;

 exit_remove_files:
	sysfs_remove_group(&pdev->dev.kobj, &lm78_group);
	device_remove_file(&pdev->dev, &dev_attr_name);
<<<<<<< HEAD
	kfree(data);
 exit_release_region:
	release_region(res->start + LM78_ADDR_REG_OFFSET, 2);
 exit:
	return err;
}

static int __devexit lm78_isa_remove(struct platform_device *pdev)
{
	struct lm78_data *data = platform_get_drvdata(pdev);
	struct resource *res;
=======
	return err;
}

static int lm78_isa_remove(struct platform_device *pdev)
{
	struct lm78_data *data = platform_get_drvdata(pdev);
>>>>>>> refs/remotes/origin/master

	hwmon_device_unregister(data->hwmon_dev);
	sysfs_remove_group(&pdev->dev.kobj, &lm78_group);
	device_remove_file(&pdev->dev, &dev_attr_name);
<<<<<<< HEAD
	kfree(data);

	res = platform_get_resource(pdev, IORESOURCE_IO, 0);
	release_region(res->start + LM78_ADDR_REG_OFFSET, 2);
=======
>>>>>>> refs/remotes/origin/master

	return 0;
}

static struct platform_driver lm78_isa_driver = {
	.driver = {
		.owner	= THIS_MODULE,
		.name	= "lm78",
	},
	.probe		= lm78_isa_probe,
<<<<<<< HEAD
	.remove		= __devexit_p(lm78_isa_remove),
};

>>>>>>> refs/remotes/origin/cm-10.0
=======
	.remove		= lm78_isa_remove,
};

>>>>>>> refs/remotes/origin/master
/* return 1 if a supported chip is found, 0 otherwise */
static int __init lm78_isa_found(unsigned short address)
{
	int val, save, found = 0;
	int port;

<<<<<<< HEAD
<<<<<<< HEAD
	/* Some boards declare base+0 to base+7 as a PNP device, some base+4
	 * to base+7 and some base+5 to base+6. So we better request each port
	 * individually for the probing phase. */
=======
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * Some boards declare base+0 to base+7 as a PNP device, some base+4
	 * to base+7 and some base+5 to base+6. So we better request each port
	 * individually for the probing phase.
	 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	for (port = address; port < address + LM78_EXTENT; port++) {
		if (!request_region(port, 1, "lm78")) {
			pr_debug("Failed to request port 0x%x\n", port);
			goto release;
		}
	}

#define REALLY_SLOW_IO
<<<<<<< HEAD
<<<<<<< HEAD
	/* We need the timeouts for at least some LM78-like
	   chips. But only if we read 'undefined' registers. */
=======
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * We need the timeouts for at least some LM78-like
	 * chips. But only if we read 'undefined' registers.
	 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	val = inb_p(address + 1);
	if (inb_p(address + 2) != val
	 || inb_p(address + 3) != val
	 || inb_p(address + 7) != val)
		goto release;
#undef REALLY_SLOW_IO

<<<<<<< HEAD
<<<<<<< HEAD
	/* We should be able to change the 7 LSB of the address port. The
	   MSB (busy flag) should be clear initially, set after the write. */
=======
=======
>>>>>>> refs/remotes/origin/master
	/*
	 * We should be able to change the 7 LSB of the address port. The
	 * MSB (busy flag) should be clear initially, set after the write.
	 */
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	save = inb_p(address + LM78_ADDR_REG_OFFSET);
	if (save & 0x80)
		goto release;
	val = ~save & 0x7f;
	outb_p(val, address + LM78_ADDR_REG_OFFSET);
	if (inb_p(address + LM78_ADDR_REG_OFFSET) != (val | 0x80)) {
		outb_p(save, address + LM78_ADDR_REG_OFFSET);
		goto release;
	}

	/* We found a device, now see if it could be an LM78 */
	outb_p(LM78_REG_CONFIG, address + LM78_ADDR_REG_OFFSET);
	val = inb_p(address + LM78_DATA_REG_OFFSET);
	if (val & 0x80)
		goto release;
	outb_p(LM78_REG_I2C_ADDR, address + LM78_ADDR_REG_OFFSET);
	val = inb_p(address + LM78_DATA_REG_OFFSET);
	if (val < 0x03 || val > 0x77)	/* Not a valid I2C address */
		goto release;

	/* The busy flag should be clear again */
	if (inb_p(address + LM78_ADDR_REG_OFFSET) & 0x80)
		goto release;

	/* Explicitly prevent the misdetection of Winbond chips */
	outb_p(0x4f, address + LM78_ADDR_REG_OFFSET);
	val = inb_p(address + LM78_DATA_REG_OFFSET);
	if (val == 0xa3 || val == 0x5c)
		goto release;

	/* Explicitly prevent the misdetection of ITE chips */
	outb_p(0x58, address + LM78_ADDR_REG_OFFSET);
	val = inb_p(address + LM78_DATA_REG_OFFSET);
	if (val == 0x90)
		goto release;

	/* Determine the chip type */
	outb_p(LM78_REG_CHIPID, address + LM78_ADDR_REG_OFFSET);
	val = inb_p(address + LM78_DATA_REG_OFFSET);
	if (val == 0x00 || val == 0x20	/* LM78 */
	 || val == 0x40			/* LM78-J */
	 || (val & 0xfe) == 0xc0)	/* LM79 */
		found = 1;

	if (found)
		pr_info("Found an %s chip at %#x\n",
			val & 0x80 ? "LM79" : "LM78", (int)address);

 release:
	for (port--; port >= address; port--)
		release_region(port, 1);
	return found;
}

static int __init lm78_isa_device_add(unsigned short address)
{
	struct resource res = {
		.start	= address,
		.end	= address + LM78_EXTENT - 1,
		.name	= "lm78",
		.flags	= IORESOURCE_IO,
	};
	int err;

	pdev = platform_device_alloc("lm78", address);
	if (!pdev) {
		err = -ENOMEM;
		pr_err("Device allocation failed\n");
		goto exit;
	}

	err = platform_device_add_resources(pdev, &res, 1);
	if (err) {
		pr_err("Device resource addition failed (%d)\n", err);
		goto exit_device_put;
	}

	err = platform_device_add(pdev);
	if (err) {
		pr_err("Device addition failed (%d)\n", err);
		goto exit_device_put;
	}

	return 0;

 exit_device_put:
	platform_device_put(pdev);
 exit:
	pdev = NULL;
	return err;
}

<<<<<<< HEAD
<<<<<<< HEAD
static int __init sm_lm78_init(void)
{
	int res;

	/* We register the ISA device first, so that we can skip the
	 * registration of an I2C interface to the same device. */
=======
=======
>>>>>>> refs/remotes/origin/master
static int __init lm78_isa_register(void)
{
	int res;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (lm78_isa_found(isa_address)) {
		res = platform_driver_register(&lm78_isa_driver);
		if (res)
			goto exit;

		/* Sets global pdev as a side effect */
		res = lm78_isa_device_add(isa_address);
		if (res)
			goto exit_unreg_isa_driver;
	}

<<<<<<< HEAD
<<<<<<< HEAD
	res = i2c_add_driver(&lm78_driver);
	if (res)
		goto exit_unreg_isa_device;

	return 0;

 exit_unreg_isa_device:
	platform_device_unregister(pdev);
=======
	return 0;

>>>>>>> refs/remotes/origin/cm-10.0
=======
	return 0;

>>>>>>> refs/remotes/origin/master
 exit_unreg_isa_driver:
	platform_driver_unregister(&lm78_isa_driver);
 exit:
	return res;
}

<<<<<<< HEAD
<<<<<<< HEAD
static void __exit sm_lm78_exit(void)
=======
static void lm78_isa_unregister(void)
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void lm78_isa_unregister(void)
>>>>>>> refs/remotes/origin/master
{
	if (pdev) {
		platform_device_unregister(pdev);
		platform_driver_unregister(&lm78_isa_driver);
	}
<<<<<<< HEAD
<<<<<<< HEAD
	i2c_del_driver(&lm78_driver);
}



MODULE_AUTHOR("Frodo Looijaard <frodol@dds.nl>");
=======
=======
>>>>>>> refs/remotes/origin/master
}
#else /* !CONFIG_ISA */

static int __init lm78_isa_register(void)
{
	return 0;
}

static void lm78_isa_unregister(void)
{
}
#endif /* CONFIG_ISA */

static int __init sm_lm78_init(void)
{
	int res;

	/*
	 * We register the ISA device first, so that we can skip the
	 * registration of an I2C interface to the same device.
	 */
	res = lm78_isa_register();
	if (res)
		goto exit;

	res = i2c_add_driver(&lm78_driver);
	if (res)
		goto exit_unreg_isa_device;

	return 0;

 exit_unreg_isa_device:
	lm78_isa_unregister();
 exit:
	return res;
}

static void __exit sm_lm78_exit(void)
{
	lm78_isa_unregister();
	i2c_del_driver(&lm78_driver);
}

MODULE_AUTHOR("Frodo Looijaard, Jean Delvare <khali@linux-fr.org>");
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
MODULE_DESCRIPTION("LM78/LM79 driver");
MODULE_LICENSE("GPL");

module_init(sm_lm78_init);
module_exit(sm_lm78_exit);
