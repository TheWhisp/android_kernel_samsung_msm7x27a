/*
 * thermal support for the cell processor
 *
 * This module adds some sysfs attributes to cpu and spu nodes.
 * Base for measurements are the digital thermal sensors (DTS)
 * located on the chip.
 * The accuracy is 2 degrees, starting from 65 up to 125 degrees celsius
 * The attributes can be found under
 * /sys/devices/system/cpu/cpuX/thermal
 * /sys/devices/system/spu/spuX/thermal
 *
 * The following attributes are added for each node:
 * temperature:
 *	contains the current temperature measured by the DTS
 * throttle_begin:
 *	throttling begins when temperature is greater or equal to
 *	throttle_begin. Setting this value to 125 prevents throttling.
 * throttle_end:
 *	throttling is being ceased, if the temperature is lower than
 *	throttle_end. Due to a delay between applying throttling and
 *	a reduced temperature this value should be less than throttle_begin.
 *	A value equal to throttle_begin provides only a very little hysteresis.
 * throttle_full_stop:
 *	If the temperatrue is greater or equal to throttle_full_stop,
 *	full throttling is applied to the cpu or spu. This value should be
 *	greater than throttle_begin and throttle_end. Setting this value to
 *	65 prevents the unit from running code at all.
 *
 * (C) Copyright IBM Deutschland Entwicklung GmbH 2005
 *
 * Author: Christian Krafft <krafft@de.ibm.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
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
<<<<<<< HEAD
#include <linux/sysdev.h>
=======
#include <linux/device.h>
>>>>>>> refs/remotes/origin/cm-10.0
#include <linux/kernel.h>
#include <linux/cpu.h>
#include <asm/spu.h>
#include <asm/io.h>
#include <asm/prom.h>
#include <asm/cell-regs.h>

#include "spu_priv1_mmio.h"

#define TEMP_MIN 65
#define TEMP_MAX 125

<<<<<<< HEAD
#define SYSDEV_PREFIX_ATTR(_prefix,_name,_mode)			\
struct sysdev_attribute attr_ ## _prefix ## _ ## _name = {	\
=======
#define DEVICE_PREFIX_ATTR(_prefix,_name,_mode)			\
struct device_attribute attr_ ## _prefix ## _ ## _name = {	\
>>>>>>> refs/remotes/origin/cm-10.0
	.attr = { .name = __stringify(_name), .mode = _mode },	\
	.show	= _prefix ## _show_ ## _name,			\
	.store	= _prefix ## _store_ ## _name,			\
};

static inline u8 reg_to_temp(u8 reg_value)
{
	return ((reg_value & 0x3f) << 1) + TEMP_MIN;
}

static inline u8 temp_to_reg(u8 temp)
{
	return ((temp - TEMP_MIN) >> 1) & 0x3f;
}

<<<<<<< HEAD
static struct cbe_pmd_regs __iomem *get_pmd_regs(struct sys_device *sysdev)
{
	struct spu *spu;

	spu = container_of(sysdev, struct spu, sysdev);
=======
static struct cbe_pmd_regs __iomem *get_pmd_regs(struct device *dev)
{
	struct spu *spu;

	spu = container_of(dev, struct spu, dev);
>>>>>>> refs/remotes/origin/cm-10.0

	return cbe_get_pmd_regs(spu_devnode(spu));
}

/* returns the value for a given spu in a given register */
<<<<<<< HEAD
static u8 spu_read_register_value(struct sys_device *sysdev, union spe_reg __iomem *reg)
=======
static u8 spu_read_register_value(struct device *dev, union spe_reg __iomem *reg)
>>>>>>> refs/remotes/origin/cm-10.0
{
	union spe_reg value;
	struct spu *spu;

<<<<<<< HEAD
	spu = container_of(sysdev, struct spu, sysdev);
=======
	spu = container_of(dev, struct spu, dev);
>>>>>>> refs/remotes/origin/cm-10.0
	value.val = in_be64(&reg->val);

	return value.spe[spu->spe_id];
}

<<<<<<< HEAD
static ssize_t spu_show_temp(struct sys_device *sysdev, struct sysdev_attribute *attr,
=======
static ssize_t spu_show_temp(struct device *dev, struct device_attribute *attr,
>>>>>>> refs/remotes/origin/cm-10.0
			char *buf)
{
	u8 value;
	struct cbe_pmd_regs __iomem *pmd_regs;

<<<<<<< HEAD
	pmd_regs = get_pmd_regs(sysdev);

	value = spu_read_register_value(sysdev, &pmd_regs->ts_ctsr1);
=======
	pmd_regs = get_pmd_regs(dev);

	value = spu_read_register_value(dev, &pmd_regs->ts_ctsr1);
>>>>>>> refs/remotes/origin/cm-10.0

	return sprintf(buf, "%d\n", reg_to_temp(value));
}

static ssize_t show_throttle(struct cbe_pmd_regs __iomem *pmd_regs, char *buf, int pos)
{
	u64 value;

	value = in_be64(&pmd_regs->tm_tpr.val);
	/* access the corresponding byte */
	value >>= pos;
	value &= 0x3F;

	return sprintf(buf, "%d\n", reg_to_temp(value));
}

static ssize_t store_throttle(struct cbe_pmd_regs __iomem *pmd_regs, const char *buf, size_t size, int pos)
{
	u64 reg_value;
	int temp;
	u64 new_value;
	int ret;

	ret = sscanf(buf, "%u", &temp);

	if (ret != 1 || temp < TEMP_MIN || temp > TEMP_MAX)
		return -EINVAL;

	new_value = temp_to_reg(temp);

	reg_value = in_be64(&pmd_regs->tm_tpr.val);

	/* zero out bits for new value */
	reg_value &= ~(0xffull << pos);
	/* set bits to new value */
	reg_value |= new_value << pos;

	out_be64(&pmd_regs->tm_tpr.val, reg_value);
	return size;
}

<<<<<<< HEAD
static ssize_t spu_show_throttle_end(struct sys_device *sysdev,
			struct sysdev_attribute *attr, char *buf)
{
	return show_throttle(get_pmd_regs(sysdev), buf, 0);
}

static ssize_t spu_show_throttle_begin(struct sys_device *sysdev,
			struct sysdev_attribute *attr, char *buf)
{
	return show_throttle(get_pmd_regs(sysdev), buf, 8);
}

static ssize_t spu_show_throttle_full_stop(struct sys_device *sysdev,
			struct sysdev_attribute *attr, char *buf)
{
	return show_throttle(get_pmd_regs(sysdev), buf, 16);
}

static ssize_t spu_store_throttle_end(struct sys_device *sysdev,
			struct sysdev_attribute *attr, const char *buf, size_t size)
{
	return store_throttle(get_pmd_regs(sysdev), buf, size, 0);
}

static ssize_t spu_store_throttle_begin(struct sys_device *sysdev,
			struct sysdev_attribute *attr, const char *buf, size_t size)
{
	return store_throttle(get_pmd_regs(sysdev), buf, size, 8);
}

static ssize_t spu_store_throttle_full_stop(struct sys_device *sysdev,
			struct sysdev_attribute *attr, const char *buf, size_t size)
{
	return store_throttle(get_pmd_regs(sysdev), buf, size, 16);
}

static ssize_t ppe_show_temp(struct sys_device *sysdev, char *buf, int pos)
=======
static ssize_t spu_show_throttle_end(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	return show_throttle(get_pmd_regs(dev), buf, 0);
}

static ssize_t spu_show_throttle_begin(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	return show_throttle(get_pmd_regs(dev), buf, 8);
}

static ssize_t spu_show_throttle_full_stop(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	return show_throttle(get_pmd_regs(dev), buf, 16);
}

static ssize_t spu_store_throttle_end(struct device *dev,
			struct device_attribute *attr, const char *buf, size_t size)
{
	return store_throttle(get_pmd_regs(dev), buf, size, 0);
}

static ssize_t spu_store_throttle_begin(struct device *dev,
			struct device_attribute *attr, const char *buf, size_t size)
{
	return store_throttle(get_pmd_regs(dev), buf, size, 8);
}

static ssize_t spu_store_throttle_full_stop(struct device *dev,
			struct device_attribute *attr, const char *buf, size_t size)
{
	return store_throttle(get_pmd_regs(dev), buf, size, 16);
}

static ssize_t ppe_show_temp(struct device *dev, char *buf, int pos)
>>>>>>> refs/remotes/origin/cm-10.0
{
	struct cbe_pmd_regs __iomem *pmd_regs;
	u64 value;

<<<<<<< HEAD
	pmd_regs = cbe_get_cpu_pmd_regs(sysdev->id);
=======
	pmd_regs = cbe_get_cpu_pmd_regs(dev->id);
>>>>>>> refs/remotes/origin/cm-10.0
	value = in_be64(&pmd_regs->ts_ctsr2);

	value = (value >> pos) & 0x3f;

	return sprintf(buf, "%d\n", reg_to_temp(value));
}


/* shows the temperature of the DTS on the PPE,
 * located near the linear thermal sensor */
<<<<<<< HEAD
static ssize_t ppe_show_temp0(struct sys_device *sysdev,
			struct sysdev_attribute *attr, char *buf)
{
	return ppe_show_temp(sysdev, buf, 32);
}

/* shows the temperature of the second DTS on the PPE */
static ssize_t ppe_show_temp1(struct sys_device *sysdev,
			struct sysdev_attribute *attr, char *buf)
{
	return ppe_show_temp(sysdev, buf, 0);
}

static ssize_t ppe_show_throttle_end(struct sys_device *sysdev,
			struct sysdev_attribute *attr, char *buf)
{
	return show_throttle(cbe_get_cpu_pmd_regs(sysdev->id), buf, 32);
}

static ssize_t ppe_show_throttle_begin(struct sys_device *sysdev,
			struct sysdev_attribute *attr, char *buf)
{
	return show_throttle(cbe_get_cpu_pmd_regs(sysdev->id), buf, 40);
}

static ssize_t ppe_show_throttle_full_stop(struct sys_device *sysdev,
			struct sysdev_attribute *attr, char *buf)
{
	return show_throttle(cbe_get_cpu_pmd_regs(sysdev->id), buf, 48);
}

static ssize_t ppe_store_throttle_end(struct sys_device *sysdev,
			struct sysdev_attribute *attr, const char *buf, size_t size)
{
	return store_throttle(cbe_get_cpu_pmd_regs(sysdev->id), buf, size, 32);
}

static ssize_t ppe_store_throttle_begin(struct sys_device *sysdev,
			struct sysdev_attribute *attr, const char *buf, size_t size)
{
	return store_throttle(cbe_get_cpu_pmd_regs(sysdev->id), buf, size, 40);
}

static ssize_t ppe_store_throttle_full_stop(struct sys_device *sysdev,
			struct sysdev_attribute *attr, const char *buf, size_t size)
{
	return store_throttle(cbe_get_cpu_pmd_regs(sysdev->id), buf, size, 48);
}


static struct sysdev_attribute attr_spu_temperature = {
=======
static ssize_t ppe_show_temp0(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	return ppe_show_temp(dev, buf, 32);
}

/* shows the temperature of the second DTS on the PPE */
static ssize_t ppe_show_temp1(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	return ppe_show_temp(dev, buf, 0);
}

static ssize_t ppe_show_throttle_end(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	return show_throttle(cbe_get_cpu_pmd_regs(dev->id), buf, 32);
}

static ssize_t ppe_show_throttle_begin(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	return show_throttle(cbe_get_cpu_pmd_regs(dev->id), buf, 40);
}

static ssize_t ppe_show_throttle_full_stop(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	return show_throttle(cbe_get_cpu_pmd_regs(dev->id), buf, 48);
}

static ssize_t ppe_store_throttle_end(struct device *dev,
			struct device_attribute *attr, const char *buf, size_t size)
{
	return store_throttle(cbe_get_cpu_pmd_regs(dev->id), buf, size, 32);
}

static ssize_t ppe_store_throttle_begin(struct device *dev,
			struct device_attribute *attr, const char *buf, size_t size)
{
	return store_throttle(cbe_get_cpu_pmd_regs(dev->id), buf, size, 40);
}

static ssize_t ppe_store_throttle_full_stop(struct device *dev,
			struct device_attribute *attr, const char *buf, size_t size)
{
	return store_throttle(cbe_get_cpu_pmd_regs(dev->id), buf, size, 48);
}


static struct device_attribute attr_spu_temperature = {
>>>>>>> refs/remotes/origin/cm-10.0
	.attr = {.name = "temperature", .mode = 0400 },
	.show = spu_show_temp,
};

<<<<<<< HEAD
static SYSDEV_PREFIX_ATTR(spu, throttle_end, 0600);
static SYSDEV_PREFIX_ATTR(spu, throttle_begin, 0600);
static SYSDEV_PREFIX_ATTR(spu, throttle_full_stop, 0600);
=======
static DEVICE_PREFIX_ATTR(spu, throttle_end, 0600);
static DEVICE_PREFIX_ATTR(spu, throttle_begin, 0600);
static DEVICE_PREFIX_ATTR(spu, throttle_full_stop, 0600);
>>>>>>> refs/remotes/origin/cm-10.0


static struct attribute *spu_attributes[] = {
	&attr_spu_temperature.attr,
	&attr_spu_throttle_end.attr,
	&attr_spu_throttle_begin.attr,
	&attr_spu_throttle_full_stop.attr,
	NULL,
};

static struct attribute_group spu_attribute_group = {
	.name	= "thermal",
	.attrs	= spu_attributes,
};

<<<<<<< HEAD
static struct sysdev_attribute attr_ppe_temperature0 = {
=======
static struct device_attribute attr_ppe_temperature0 = {
>>>>>>> refs/remotes/origin/cm-10.0
	.attr = {.name = "temperature0", .mode = 0400 },
	.show = ppe_show_temp0,
};

<<<<<<< HEAD
static struct sysdev_attribute attr_ppe_temperature1 = {
=======
static struct device_attribute attr_ppe_temperature1 = {
>>>>>>> refs/remotes/origin/cm-10.0
	.attr = {.name = "temperature1", .mode = 0400 },
	.show = ppe_show_temp1,
};

<<<<<<< HEAD
static SYSDEV_PREFIX_ATTR(ppe, throttle_end, 0600);
static SYSDEV_PREFIX_ATTR(ppe, throttle_begin, 0600);
static SYSDEV_PREFIX_ATTR(ppe, throttle_full_stop, 0600);
=======
static DEVICE_PREFIX_ATTR(ppe, throttle_end, 0600);
static DEVICE_PREFIX_ATTR(ppe, throttle_begin, 0600);
static DEVICE_PREFIX_ATTR(ppe, throttle_full_stop, 0600);
>>>>>>> refs/remotes/origin/cm-10.0

static struct attribute *ppe_attributes[] = {
	&attr_ppe_temperature0.attr,
	&attr_ppe_temperature1.attr,
	&attr_ppe_throttle_end.attr,
	&attr_ppe_throttle_begin.attr,
	&attr_ppe_throttle_full_stop.attr,
	NULL,
};

static struct attribute_group ppe_attribute_group = {
	.name	= "thermal",
	.attrs	= ppe_attributes,
};

/*
 * initialize throttling with default values
 */
static int __init init_default_values(void)
{
	int cpu;
	struct cbe_pmd_regs __iomem *pmd_regs;
<<<<<<< HEAD
	struct sys_device *sysdev;
=======
	struct device *dev;
>>>>>>> refs/remotes/origin/cm-10.0
	union ppe_spe_reg tpr;
	union spe_reg str1;
	u64 str2;
	union spe_reg cr1;
	u64 cr2;

	/* TPR defaults */
	/* ppe
	 *	1F - no full stop
	 *	08 - dynamic throttling starts if over 80 degrees
	 *	03 - dynamic throttling ceases if below 70 degrees */
	tpr.ppe = 0x1F0803;
	/* spe
	 *	10 - full stopped when over 96 degrees
	 *	08 - dynamic throttling starts if over 80 degrees
	 *	03 - dynamic throttling ceases if below 70 degrees
	 */
	tpr.spe = 0x100803;

	/* STR defaults */
	/* str1
	 *	10 - stop 16 of 32 cycles
	 */
	str1.val = 0x1010101010101010ull;
	/* str2
	 *	10 - stop 16 of 32 cycles
	 */
	str2 = 0x10;

	/* CR defaults */
	/* cr1
	 *	4 - normal operation
	 */
	cr1.val = 0x0404040404040404ull;
	/* cr2
	 *	4 - normal operation
	 */
	cr2 = 0x04;

	for_each_possible_cpu (cpu) {
		pr_debug("processing cpu %d\n", cpu);
<<<<<<< HEAD
		sysdev = get_cpu_sysdev(cpu);

		if (!sysdev) {
			pr_info("invalid sysdev pointer for cbe_thermal\n");
			return -EINVAL;
		}

		pmd_regs = cbe_get_cpu_pmd_regs(sysdev->id);
=======
		dev = get_cpu_device(cpu);

		if (!dev) {
			pr_info("invalid dev pointer for cbe_thermal\n");
			return -EINVAL;
		}

		pmd_regs = cbe_get_cpu_pmd_regs(dev->id);
>>>>>>> refs/remotes/origin/cm-10.0

		if (!pmd_regs) {
			pr_info("invalid CBE regs pointer for cbe_thermal\n");
			return -EINVAL;
		}

		out_be64(&pmd_regs->tm_str2, str2);
		out_be64(&pmd_regs->tm_str1.val, str1.val);
		out_be64(&pmd_regs->tm_tpr.val, tpr.val);
		out_be64(&pmd_regs->tm_cr1.val, cr1.val);
		out_be64(&pmd_regs->tm_cr2, cr2);
	}

	return 0;
}


static int __init thermal_init(void)
{
	int rc = init_default_values();

	if (rc == 0) {
<<<<<<< HEAD
		spu_add_sysdev_attr_group(&spu_attribute_group);
		cpu_add_sysdev_attr_group(&ppe_attribute_group);
=======
		spu_add_dev_attr_group(&spu_attribute_group);
		cpu_add_dev_attr_group(&ppe_attribute_group);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	return rc;
}
module_init(thermal_init);

static void __exit thermal_exit(void)
{
<<<<<<< HEAD
	spu_remove_sysdev_attr_group(&spu_attribute_group);
	cpu_remove_sysdev_attr_group(&ppe_attribute_group);
=======
	spu_remove_dev_attr_group(&spu_attribute_group);
	cpu_remove_dev_attr_group(&ppe_attribute_group);
>>>>>>> refs/remotes/origin/cm-10.0
}
module_exit(thermal_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Christian Krafft <krafft@de.ibm.com>");

