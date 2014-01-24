/*
 *	w1_therm.c
 *
<<<<<<< HEAD
<<<<<<< HEAD
 * Copyright (c) 2004 Evgeniy Polyakov <johnpol@2ka.mipt.ru>
=======
 * Copyright (c) 2004 Evgeniy Polyakov <zbr@ioremap.net>
>>>>>>> refs/remotes/origin/cm-10.0
=======
 * Copyright (c) 2004 Evgeniy Polyakov <zbr@ioremap.net>
>>>>>>> refs/remotes/origin/master
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the therms of the GNU General Public License as published by
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <asm/types.h>

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/delay.h>

#include "../w1.h"
#include "../w1_int.h"
#include "../w1_family.h"

MODULE_LICENSE("GPL");
<<<<<<< HEAD
<<<<<<< HEAD
MODULE_AUTHOR("Evgeniy Polyakov <johnpol@2ka.mipt.ru>");
=======
MODULE_AUTHOR("Evgeniy Polyakov <zbr@ioremap.net>");
>>>>>>> refs/remotes/origin/cm-10.0
MODULE_DESCRIPTION("Driver for 1-wire Dallas network protocol, temperature family.");
=======
MODULE_AUTHOR("Evgeniy Polyakov <zbr@ioremap.net>");
MODULE_DESCRIPTION("Driver for 1-wire Dallas network protocol, temperature family.");
MODULE_ALIAS("w1-family-" __stringify(W1_THERM_DS18S20));
MODULE_ALIAS("w1-family-" __stringify(W1_THERM_DS1822));
MODULE_ALIAS("w1-family-" __stringify(W1_THERM_DS18B20));
MODULE_ALIAS("w1-family-" __stringify(W1_THERM_DS1825));
MODULE_ALIAS("w1-family-" __stringify(W1_THERM_DS28EA00));
>>>>>>> refs/remotes/origin/master

/* Allow the strong pullup to be disabled, but default to enabled.
 * If it was disabled a parasite powered device might not get the require
 * current to do a temperature conversion.  If it is enabled parasite powered
 * devices have a better chance of getting the current required.
<<<<<<< HEAD
=======
 * In case the parasite power-detection is not working (seems to be the case
 * for some DS18S20) the strong pullup can also be forced, regardless of the
 * power state of the devices.
 *
 * Summary of options:
 * - strong_pullup = 0	Disable strong pullup completely
 * - strong_pullup = 1	Enable automatic strong pullup detection
 * - strong_pullup = 2	Force strong pullup
>>>>>>> refs/remotes/origin/master
 */
static int w1_strong_pullup = 1;
module_param_named(strong_pullup, w1_strong_pullup, int, 0);

<<<<<<< HEAD
static u8 bad_roms[][9] = {
				{0xaa, 0x00, 0x4b, 0x46, 0xff, 0xff, 0x0c, 0x10, 0x87},
				{}
			};

static ssize_t w1_therm_read(struct device *device,
	struct device_attribute *attr, char *buf);

static struct device_attribute w1_therm_attr =
	__ATTR(w1_slave, S_IRUGO, w1_therm_read, NULL);

static int w1_therm_add_slave(struct w1_slave *sl)
{
	return device_create_file(&sl->dev, &w1_therm_attr);
}

static void w1_therm_remove_slave(struct w1_slave *sl)
{
	device_remove_file(&sl->dev, &w1_therm_attr);
}

static struct w1_family_ops w1_therm_fops = {
	.add_slave	= w1_therm_add_slave,
	.remove_slave	= w1_therm_remove_slave,
=======

static ssize_t w1_slave_show(struct device *device,
	struct device_attribute *attr, char *buf);

static DEVICE_ATTR_RO(w1_slave);

static struct attribute *w1_therm_attrs[] = {
	&dev_attr_w1_slave.attr,
	NULL,
};
ATTRIBUTE_GROUPS(w1_therm);

static struct w1_family_ops w1_therm_fops = {
	.groups		= w1_therm_groups,
>>>>>>> refs/remotes/origin/master
};

static struct w1_family w1_therm_family_DS18S20 = {
	.fid = W1_THERM_DS18S20,
	.fops = &w1_therm_fops,
};

static struct w1_family w1_therm_family_DS18B20 = {
	.fid = W1_THERM_DS18B20,
	.fops = &w1_therm_fops,
};

static struct w1_family w1_therm_family_DS1822 = {
	.fid = W1_THERM_DS1822,
	.fops = &w1_therm_fops,
};

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
static struct w1_family w1_therm_family_DS28EA00 = {
	.fid = W1_THERM_DS28EA00,
	.fops = &w1_therm_fops,
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
static struct w1_family w1_therm_family_DS1825 = {
	.fid = W1_THERM_DS1825,
	.fops = &w1_therm_fops,
};

>>>>>>> refs/remotes/origin/master
struct w1_therm_family_converter
{
	u8			broken;
	u16			reserved;
	struct w1_family	*f;
	int			(*convert)(u8 rom[9]);
};

/* The return value is millidegrees Centigrade. */
static inline int w1_DS18B20_convert_temp(u8 rom[9]);
static inline int w1_DS18S20_convert_temp(u8 rom[9]);

static struct w1_therm_family_converter w1_therm_families[] = {
	{
		.f		= &w1_therm_family_DS18S20,
		.convert 	= w1_DS18S20_convert_temp
	},
	{
		.f		= &w1_therm_family_DS1822,
		.convert 	= w1_DS18B20_convert_temp
	},
	{
		.f		= &w1_therm_family_DS18B20,
		.convert 	= w1_DS18B20_convert_temp
	},
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
	{
		.f		= &w1_therm_family_DS28EA00,
		.convert	= w1_DS18B20_convert_temp
	},
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
	{
		.f		= &w1_therm_family_DS1825,
		.convert	= w1_DS18B20_convert_temp
	}
>>>>>>> refs/remotes/origin/master
};

static inline int w1_DS18B20_convert_temp(u8 rom[9])
{
	s16 t = le16_to_cpup((__le16 *)rom);
	return t*1000/16;
}

static inline int w1_DS18S20_convert_temp(u8 rom[9])
{
	int t, h;

	if (!rom[7])
		return 0;

	if (rom[1] == 0)
		t = ((s32)rom[0] >> 1)*1000;
	else
		t = 1000*(-1*(s32)(0x100-rom[0]) >> 1);

	t -= 250;
	h = 1000*((s32)rom[7] - (s32)rom[6]);
	h /= (s32)rom[7];
	t += h;

	return t;
}

static inline int w1_convert_temp(u8 rom[9], u8 fid)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(w1_therm_families); ++i)
		if (w1_therm_families[i].f->fid == fid)
			return w1_therm_families[i].convert(rom);

	return 0;
}

<<<<<<< HEAD
static int w1_therm_check_rom(u8 rom[9])
{
	int i;

	for (i=0; i<sizeof(bad_roms)/9; ++i)
		if (!memcmp(bad_roms[i], rom, 9))
			return 1;

	return 0;
}

static ssize_t w1_therm_read(struct device *device,
=======

static ssize_t w1_slave_show(struct device *device,
>>>>>>> refs/remotes/origin/master
	struct device_attribute *attr, char *buf)
{
	struct w1_slave *sl = dev_to_w1_slave(device);
	struct w1_master *dev = sl->master;
<<<<<<< HEAD
<<<<<<< HEAD
	u8 rom[9], crc, verdict;
	int i, max_trying = 10;
	ssize_t c = PAGE_SIZE;

	mutex_lock(&dev->mutex);
=======
=======
>>>>>>> refs/remotes/origin/master
	u8 rom[9], crc, verdict, external_power;
	int i, max_trying = 10;
	ssize_t c = PAGE_SIZE;

<<<<<<< HEAD
	i = mutex_lock_interruptible(&dev->mutex);
	if (i != 0)
		return i;
>>>>>>> refs/remotes/origin/cm-10.0

	memset(rom, 0, sizeof(rom));

	verdict = 0;
	crc = 0;

	while (max_trying--) {
		if (!w1_reset_select_slave(sl)) {
			int count = 0;
			unsigned int tm = 750;
<<<<<<< HEAD

			/* 750ms strong pullup (or delay) after the convert */
			if (w1_strong_pullup)
				w1_next_pullup(dev, tm);
			w1_write_8(dev, W1_CONVERT_TEMP);
			if (!w1_strong_pullup)
				msleep(tm);
=======
=======
	i = mutex_lock_interruptible(&dev->bus_mutex);
	if (i != 0)
		return i;

	memset(rom, 0, sizeof(rom));

	while (max_trying--) {

		verdict = 0;
		crc = 0;

		if (!w1_reset_select_slave(sl)) {
			int count = 0;
			unsigned int tm = 750;
>>>>>>> refs/remotes/origin/master
			unsigned long sleep_rem;

			w1_write_8(dev, W1_READ_PSUPPLY);
			external_power = w1_read_8(dev);

			if (w1_reset_select_slave(sl))
				continue;

			/* 750ms strong pullup (or delay) after the convert */
<<<<<<< HEAD
			if (!external_power && w1_strong_pullup)
=======
			if (w1_strong_pullup == 2 ||
					(!external_power && w1_strong_pullup))
>>>>>>> refs/remotes/origin/master
				w1_next_pullup(dev, tm);

			w1_write_8(dev, W1_CONVERT_TEMP);

			if (external_power) {
<<<<<<< HEAD
				mutex_unlock(&dev->mutex);
=======
				mutex_unlock(&dev->bus_mutex);
>>>>>>> refs/remotes/origin/master

				sleep_rem = msleep_interruptible(tm);
				if (sleep_rem != 0)
					return -EINTR;

<<<<<<< HEAD
				i = mutex_lock_interruptible(&dev->mutex);
=======
				i = mutex_lock_interruptible(&dev->bus_mutex);
>>>>>>> refs/remotes/origin/master
				if (i != 0)
					return i;
			} else if (!w1_strong_pullup) {
				sleep_rem = msleep_interruptible(tm);
				if (sleep_rem != 0) {
<<<<<<< HEAD
					mutex_unlock(&dev->mutex);
					return -EINTR;
				}
			}
>>>>>>> refs/remotes/origin/cm-10.0
=======
					mutex_unlock(&dev->bus_mutex);
					return -EINTR;
				}
			}
>>>>>>> refs/remotes/origin/master

			if (!w1_reset_select_slave(sl)) {

				w1_write_8(dev, W1_READ_SCRATCHPAD);
				if ((count = w1_read_block(dev, rom, 9)) != 9) {
					dev_warn(device, "w1_read_block() "
						"returned %u instead of 9.\n",
						count);
				}

				crc = w1_calc_crc8(rom, 8);

				if (rom[8] == crc)
					verdict = 1;
			}
		}

<<<<<<< HEAD
		if (!w1_therm_check_rom(rom))
=======
		if (verdict)
>>>>>>> refs/remotes/origin/master
			break;
	}

	for (i = 0; i < 9; ++i)
		c -= snprintf(buf + PAGE_SIZE - c, c, "%02x ", rom[i]);
	c -= snprintf(buf + PAGE_SIZE - c, c, ": crc=%02x %s\n",
			   crc, (verdict) ? "YES" : "NO");
	if (verdict)
		memcpy(sl->rom, rom, sizeof(sl->rom));
	else
<<<<<<< HEAD
		dev_warn(device, "18S20 doesn't respond to CONVERT_TEMP.\n");
=======
		dev_warn(device, "Read failed CRC check\n");
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < 9; ++i)
		c -= snprintf(buf + PAGE_SIZE - c, c, "%02x ", sl->rom[i]);

	c -= snprintf(buf + PAGE_SIZE - c, c, "t=%d\n",
		w1_convert_temp(rom, sl->family->fid));
<<<<<<< HEAD
	mutex_unlock(&dev->mutex);
=======
	mutex_unlock(&dev->bus_mutex);
>>>>>>> refs/remotes/origin/master

	return PAGE_SIZE - c;
}

static int __init w1_therm_init(void)
{
	int err, i;

	for (i = 0; i < ARRAY_SIZE(w1_therm_families); ++i) {
		err = w1_register_family(w1_therm_families[i].f);
		if (err)
			w1_therm_families[i].broken = 1;
	}

	return 0;
}

static void __exit w1_therm_fini(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(w1_therm_families); ++i)
		if (!w1_therm_families[i].broken)
			w1_unregister_family(w1_therm_families[i].f);
}

module_init(w1_therm_init);
module_exit(w1_therm_fini);
