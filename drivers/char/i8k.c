/*
 * i8k.c -- Linux driver for accessing the SMM BIOS on Dell laptops.
<<<<<<< HEAD
 *	    See http://www.debian.org/~dz/i8k/ for more information
 *	    and for latest version of this driver.
=======
>>>>>>> refs/remotes/origin/master
 *
 * Copyright (C) 2001  Massimo Dal Zotto <dz@debian.org>
 *
 * Hwmon integration:
 * Copyright (C) 2011  Jean Delvare <khali@linux-fr.org>
<<<<<<< HEAD
=======
 * Copyright (C) 2013  Guenter Roeck <linux@roeck-us.net>
>>>>>>> refs/remotes/origin/master
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

<<<<<<< HEAD
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> refs/remotes/origin/master
#include <linux/module.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/dmi.h>
#include <linux/capability.h>
#include <linux/mutex.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
<<<<<<< HEAD
#include <asm/uaccess.h>
#include <asm/io.h>

#include <linux/i8k.h>

#define I8K_VERSION		"1.14 21/02/2005"

=======
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/sched.h>

#include <linux/i8k.h>

>>>>>>> refs/remotes/origin/master
#define I8K_SMM_FN_STATUS	0x0025
#define I8K_SMM_POWER_STATUS	0x0069
#define I8K_SMM_SET_FAN		0x01a3
#define I8K_SMM_GET_FAN		0x00a3
#define I8K_SMM_GET_SPEED	0x02a3
#define I8K_SMM_GET_TEMP	0x10a3
#define I8K_SMM_GET_DELL_SIG1	0xfea3
#define I8K_SMM_GET_DELL_SIG2	0xffa3
<<<<<<< HEAD
#define I8K_SMM_BIOS_VERSION	0x00a6
=======
>>>>>>> refs/remotes/origin/master

#define I8K_FAN_MULT		30
#define I8K_MAX_TEMP		127

#define I8K_FN_NONE		0x00
#define I8K_FN_UP		0x01
#define I8K_FN_DOWN		0x02
#define I8K_FN_MUTE		0x04
#define I8K_FN_MASK		0x07
#define I8K_FN_SHIFT		8

#define I8K_POWER_AC		0x05
#define I8K_POWER_BATTERY	0x01

#define I8K_TEMPERATURE_BUG	1

static DEFINE_MUTEX(i8k_mutex);
static char bios_version[4];
static struct device *i8k_hwmon_dev;
<<<<<<< HEAD
=======
static u32 i8k_hwmon_flags;
static int i8k_fan_mult;

#define I8K_HWMON_HAVE_TEMP1	(1 << 0)
#define I8K_HWMON_HAVE_TEMP2	(1 << 1)
#define I8K_HWMON_HAVE_TEMP3	(1 << 2)
#define I8K_HWMON_HAVE_TEMP4	(1 << 3)
#define I8K_HWMON_HAVE_FAN1	(1 << 4)
#define I8K_HWMON_HAVE_FAN2	(1 << 5)
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Massimo Dal Zotto (dz@debian.org)");
MODULE_DESCRIPTION("Driver for accessing SMM BIOS on Dell laptops");
MODULE_LICENSE("GPL");

<<<<<<< HEAD
<<<<<<< HEAD
static int force;
module_param(force, bool, 0);
MODULE_PARM_DESC(force, "Force loading without checking for supported models");

static int ignore_dmi;
module_param(ignore_dmi, bool, 0);
MODULE_PARM_DESC(ignore_dmi, "Continue probing hardware even if DMI data does not match");

static int restricted;
module_param(restricted, bool, 0);
MODULE_PARM_DESC(restricted, "Allow fan control if SYS_ADMIN capability set");

static int power_status;
=======
=======
>>>>>>> refs/remotes/origin/master
static bool force;
module_param(force, bool, 0);
MODULE_PARM_DESC(force, "Force loading without checking for supported models");

static bool ignore_dmi;
module_param(ignore_dmi, bool, 0);
MODULE_PARM_DESC(ignore_dmi, "Continue probing hardware even if DMI data does not match");

static bool restricted;
module_param(restricted, bool, 0);
MODULE_PARM_DESC(restricted, "Allow fan control if SYS_ADMIN capability set");

static bool power_status;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
module_param(power_status, bool, 0600);
MODULE_PARM_DESC(power_status, "Report power status in /proc/i8k");

static int fan_mult = I8K_FAN_MULT;
module_param(fan_mult, int, 0);
MODULE_PARM_DESC(fan_mult, "Factor to multiply fan speed with");

static int i8k_open_fs(struct inode *inode, struct file *file);
static long i8k_ioctl(struct file *, unsigned int, unsigned long);

static const struct file_operations i8k_fops = {
	.owner		= THIS_MODULE,
	.open		= i8k_open_fs,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
	.unlocked_ioctl	= i8k_ioctl,
};

struct smm_regs {
	unsigned int eax;
<<<<<<< HEAD
	unsigned int ebx __attribute__ ((packed));
	unsigned int ecx __attribute__ ((packed));
	unsigned int edx __attribute__ ((packed));
	unsigned int esi __attribute__ ((packed));
	unsigned int edi __attribute__ ((packed));
=======
	unsigned int ebx __packed;
	unsigned int ecx __packed;
	unsigned int edx __packed;
	unsigned int esi __packed;
	unsigned int edi __packed;
>>>>>>> refs/remotes/origin/master
};

static inline const char *i8k_get_dmi_data(int field)
{
	const char *dmi_data = dmi_get_system_info(field);

	return dmi_data && *dmi_data ? dmi_data : "?";
}

/*
 * Call the System Management Mode BIOS. Code provided by Jonathan Buzzard.
 */
static int i8k_smm(struct smm_regs *regs)
{
	int rc;
	int eax = regs->eax;
<<<<<<< HEAD
=======
	cpumask_var_t old_mask;

	/* SMM requires CPU 0 */
	if (!alloc_cpumask_var(&old_mask, GFP_KERNEL))
		return -ENOMEM;
	cpumask_copy(old_mask, &current->cpus_allowed);
	set_cpus_allowed_ptr(current, cpumask_of(0));
	if (smp_processor_id() != 0) {
		rc = -EBUSY;
		goto out;
	}
>>>>>>> refs/remotes/origin/master

#if defined(CONFIG_X86_64)
	asm volatile("pushq %%rax\n\t"
		"movl 0(%%rax),%%edx\n\t"
		"pushq %%rdx\n\t"
		"movl 4(%%rax),%%ebx\n\t"
		"movl 8(%%rax),%%ecx\n\t"
		"movl 12(%%rax),%%edx\n\t"
		"movl 16(%%rax),%%esi\n\t"
		"movl 20(%%rax),%%edi\n\t"
		"popq %%rax\n\t"
		"out %%al,$0xb2\n\t"
		"out %%al,$0x84\n\t"
		"xchgq %%rax,(%%rsp)\n\t"
		"movl %%ebx,4(%%rax)\n\t"
		"movl %%ecx,8(%%rax)\n\t"
		"movl %%edx,12(%%rax)\n\t"
		"movl %%esi,16(%%rax)\n\t"
		"movl %%edi,20(%%rax)\n\t"
		"popq %%rdx\n\t"
		"movl %%edx,0(%%rax)\n\t"
		"pushfq\n\t"
		"popq %%rax\n\t"
		"andl $1,%%eax\n"
<<<<<<< HEAD
		:"=a"(rc)
=======
		: "=a"(rc)
>>>>>>> refs/remotes/origin/master
		:    "a"(regs)
		:    "%ebx", "%ecx", "%edx", "%esi", "%edi", "memory");
#else
	asm volatile("pushl %%eax\n\t"
	    "movl 0(%%eax),%%edx\n\t"
	    "push %%edx\n\t"
	    "movl 4(%%eax),%%ebx\n\t"
	    "movl 8(%%eax),%%ecx\n\t"
	    "movl 12(%%eax),%%edx\n\t"
	    "movl 16(%%eax),%%esi\n\t"
	    "movl 20(%%eax),%%edi\n\t"
	    "popl %%eax\n\t"
	    "out %%al,$0xb2\n\t"
	    "out %%al,$0x84\n\t"
	    "xchgl %%eax,(%%esp)\n\t"
	    "movl %%ebx,4(%%eax)\n\t"
	    "movl %%ecx,8(%%eax)\n\t"
	    "movl %%edx,12(%%eax)\n\t"
	    "movl %%esi,16(%%eax)\n\t"
	    "movl %%edi,20(%%eax)\n\t"
	    "popl %%edx\n\t"
	    "movl %%edx,0(%%eax)\n\t"
	    "lahf\n\t"
	    "shrl $8,%%eax\n\t"
	    "andl $1,%%eax\n"
<<<<<<< HEAD
	    :"=a"(rc)
=======
	    : "=a"(rc)
>>>>>>> refs/remotes/origin/master
	    :    "a"(regs)
	    :    "%ebx", "%ecx", "%edx", "%esi", "%edi", "memory");
#endif
	if (rc != 0 || (regs->eax & 0xffff) == 0xffff || regs->eax == eax)
<<<<<<< HEAD
		return -EINVAL;

	return 0;
}

/*
 * Read the bios version. Return the version as an integer corresponding
 * to the ascii value, for example "A17" is returned as 0x00413137.
 */
static int i8k_get_bios_version(void)
{
	struct smm_regs regs = { .eax = I8K_SMM_BIOS_VERSION, };

	return i8k_smm(&regs) ? : regs.eax;
=======
		rc = -EINVAL;

out:
	set_cpus_allowed_ptr(current, old_mask);
	free_cpumask_var(old_mask);
	return rc;
>>>>>>> refs/remotes/origin/master
}

/*
 * Read the Fn key status.
 */
static int i8k_get_fn_status(void)
{
	struct smm_regs regs = { .eax = I8K_SMM_FN_STATUS, };
	int rc;

<<<<<<< HEAD
	if ((rc = i8k_smm(&regs)) < 0)
=======
	rc = i8k_smm(&regs);
	if (rc < 0)
>>>>>>> refs/remotes/origin/master
		return rc;

	switch ((regs.eax >> I8K_FN_SHIFT) & I8K_FN_MASK) {
	case I8K_FN_UP:
		return I8K_VOL_UP;
	case I8K_FN_DOWN:
		return I8K_VOL_DOWN;
	case I8K_FN_MUTE:
		return I8K_VOL_MUTE;
	default:
		return 0;
	}
}

/*
 * Read the power status.
 */
static int i8k_get_power_status(void)
{
	struct smm_regs regs = { .eax = I8K_SMM_POWER_STATUS, };
	int rc;

<<<<<<< HEAD
	if ((rc = i8k_smm(&regs)) < 0)
=======
	rc = i8k_smm(&regs);
	if (rc < 0)
>>>>>>> refs/remotes/origin/master
		return rc;

	return (regs.eax & 0xff) == I8K_POWER_AC ? I8K_AC : I8K_BATTERY;
}

/*
 * Read the fan status.
 */
static int i8k_get_fan_status(int fan)
{
	struct smm_regs regs = { .eax = I8K_SMM_GET_FAN, };

	regs.ebx = fan & 0xff;
	return i8k_smm(&regs) ? : regs.eax & 0xff;
}

/*
 * Read the fan speed in RPM.
 */
static int i8k_get_fan_speed(int fan)
{
	struct smm_regs regs = { .eax = I8K_SMM_GET_SPEED, };

	regs.ebx = fan & 0xff;
<<<<<<< HEAD
	return i8k_smm(&regs) ? : (regs.eax & 0xffff) * fan_mult;
=======
	return i8k_smm(&regs) ? : (regs.eax & 0xffff) * i8k_fan_mult;
>>>>>>> refs/remotes/origin/master
}

/*
 * Set the fan speed (off, low, high). Returns the new fan status.
 */
static int i8k_set_fan(int fan, int speed)
{
	struct smm_regs regs = { .eax = I8K_SMM_SET_FAN, };

	speed = (speed < 0) ? 0 : ((speed > I8K_FAN_MAX) ? I8K_FAN_MAX : speed);
	regs.ebx = (fan & 0xff) | (speed << 8);

	return i8k_smm(&regs) ? : i8k_get_fan_status(fan);
}

/*
 * Read the cpu temperature.
 */
static int i8k_get_temp(int sensor)
{
	struct smm_regs regs = { .eax = I8K_SMM_GET_TEMP, };
	int rc;
	int temp;

#ifdef I8K_TEMPERATURE_BUG
<<<<<<< HEAD
	static int prev;
#endif
	regs.ebx = sensor & 0xff;
	if ((rc = i8k_smm(&regs)) < 0)
=======
	static int prev[4];
#endif
	regs.ebx = sensor & 0xff;
	rc = i8k_smm(&regs);
	if (rc < 0)
>>>>>>> refs/remotes/origin/master
		return rc;

	temp = regs.eax & 0xff;

#ifdef I8K_TEMPERATURE_BUG
	/*
	 * Sometimes the temperature sensor returns 0x99, which is out of range.
	 * In this case we return (once) the previous cached value. For example:
	 # 1003655137 00000058 00005a4b
	 # 1003655138 00000099 00003a80 <--- 0x99 = 153 degrees
	 # 1003655139 00000054 00005c52
	 */
	if (temp > I8K_MAX_TEMP) {
<<<<<<< HEAD
		temp = prev;
		prev = I8K_MAX_TEMP;
	} else {
		prev = temp;
=======
		temp = prev[sensor];
		prev[sensor] = I8K_MAX_TEMP;
	} else {
		prev[sensor] = temp;
>>>>>>> refs/remotes/origin/master
	}
#endif

	return temp;
}

static int i8k_get_dell_signature(int req_fn)
{
	struct smm_regs regs = { .eax = req_fn, };
	int rc;

<<<<<<< HEAD
	if ((rc = i8k_smm(&regs)) < 0)
=======
	rc = i8k_smm(&regs);
	if (rc < 0)
>>>>>>> refs/remotes/origin/master
		return rc;

	return regs.eax == 1145651527 && regs.edx == 1145392204 ? 0 : -1;
}

static int
i8k_ioctl_unlocked(struct file *fp, unsigned int cmd, unsigned long arg)
{
	int val = 0;
	int speed;
	unsigned char buff[16];
	int __user *argp = (int __user *)arg;

	if (!argp)
		return -EINVAL;

	switch (cmd) {
	case I8K_BIOS_VERSION:
<<<<<<< HEAD
		val = i8k_get_bios_version();
=======
		val = (bios_version[0] << 16) |
				(bios_version[1] << 8) | bios_version[2];
>>>>>>> refs/remotes/origin/master
		break;

	case I8K_MACHINE_ID:
		memset(buff, 0, 16);
<<<<<<< HEAD
		strlcpy(buff, i8k_get_dmi_data(DMI_PRODUCT_SERIAL), sizeof(buff));
=======
		strlcpy(buff, i8k_get_dmi_data(DMI_PRODUCT_SERIAL),
			sizeof(buff));
>>>>>>> refs/remotes/origin/master
		break;

	case I8K_FN_STATUS:
		val = i8k_get_fn_status();
		break;

	case I8K_POWER_STATUS:
		val = i8k_get_power_status();
		break;

	case I8K_GET_TEMP:
		val = i8k_get_temp(0);
		break;

	case I8K_GET_SPEED:
		if (copy_from_user(&val, argp, sizeof(int)))
			return -EFAULT;

		val = i8k_get_fan_speed(val);
		break;

	case I8K_GET_FAN:
		if (copy_from_user(&val, argp, sizeof(int)))
			return -EFAULT;

		val = i8k_get_fan_status(val);
		break;

	case I8K_SET_FAN:
		if (restricted && !capable(CAP_SYS_ADMIN))
			return -EPERM;

		if (copy_from_user(&val, argp, sizeof(int)))
			return -EFAULT;

		if (copy_from_user(&speed, argp + 1, sizeof(int)))
			return -EFAULT;

		val = i8k_set_fan(val, speed);
		break;

	default:
		return -EINVAL;
	}

	if (val < 0)
		return val;

	switch (cmd) {
	case I8K_BIOS_VERSION:
		if (copy_to_user(argp, &val, 4))
			return -EFAULT;

		break;
	case I8K_MACHINE_ID:
		if (copy_to_user(argp, buff, 16))
			return -EFAULT;

		break;
	default:
		if (copy_to_user(argp, &val, sizeof(int)))
			return -EFAULT;

		break;
	}

	return 0;
}

static long i8k_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
	long ret;

	mutex_lock(&i8k_mutex);
	ret = i8k_ioctl_unlocked(fp, cmd, arg);
	mutex_unlock(&i8k_mutex);

	return ret;
}

/*
 * Print the information for /proc/i8k.
 */
static int i8k_proc_show(struct seq_file *seq, void *offset)
{
	int fn_key, cpu_temp, ac_power;
	int left_fan, right_fan, left_speed, right_speed;

	cpu_temp	= i8k_get_temp(0);			/* 11100 µs */
	left_fan	= i8k_get_fan_status(I8K_FAN_LEFT);	/*   580 µs */
	right_fan	= i8k_get_fan_status(I8K_FAN_RIGHT);	/*   580 µs */
	left_speed	= i8k_get_fan_speed(I8K_FAN_LEFT);	/*   580 µs */
	right_speed	= i8k_get_fan_speed(I8K_FAN_RIGHT);	/*   580 µs */
	fn_key		= i8k_get_fn_status();			/*   750 µs */
	if (power_status)
		ac_power = i8k_get_power_status();		/* 14700 µs */
	else
		ac_power = -1;

	/*
	 * Info:
	 *
	 * 1)  Format version (this will change if format changes)
	 * 2)  BIOS version
	 * 3)  BIOS machine ID
	 * 4)  Cpu temperature
	 * 5)  Left fan status
	 * 6)  Right fan status
	 * 7)  Left fan speed
	 * 8)  Right fan speed
	 * 9)  AC power
	 * 10) Fn Key status
	 */
	return seq_printf(seq, "%s %s %s %d %d %d %d %d %d %d\n",
			  I8K_PROC_FMT,
			  bios_version,
			  i8k_get_dmi_data(DMI_PRODUCT_SERIAL),
			  cpu_temp,
			  left_fan, right_fan, left_speed, right_speed,
			  ac_power, fn_key);
}

static int i8k_open_fs(struct inode *inode, struct file *file)
{
	return single_open(file, i8k_proc_show, NULL);
}


/*
 * Hwmon interface
 */

static ssize_t i8k_hwmon_show_temp(struct device *dev,
				   struct device_attribute *devattr,
				   char *buf)
{
<<<<<<< HEAD
	int cpu_temp;

	cpu_temp = i8k_get_temp(0);
	if (cpu_temp < 0)
		return cpu_temp;
	return sprintf(buf, "%d\n", cpu_temp * 1000);
=======
	int index = to_sensor_dev_attr(devattr)->index;
	int temp;

	temp = i8k_get_temp(index);
	if (temp < 0)
		return temp;
	return sprintf(buf, "%d\n", temp * 1000);
>>>>>>> refs/remotes/origin/master
}

static ssize_t i8k_hwmon_show_fan(struct device *dev,
				  struct device_attribute *devattr,
				  char *buf)
{
	int index = to_sensor_dev_attr(devattr)->index;
	int fan_speed;

	fan_speed = i8k_get_fan_speed(index);
	if (fan_speed < 0)
		return fan_speed;
	return sprintf(buf, "%d\n", fan_speed);
}

<<<<<<< HEAD
=======
static ssize_t i8k_hwmon_show_pwm(struct device *dev,
				  struct device_attribute *devattr,
				  char *buf)
{
	int index = to_sensor_dev_attr(devattr)->index;
	int status;

	status = i8k_get_fan_status(index);
	if (status < 0)
		return -EIO;
	return sprintf(buf, "%d\n", clamp_val(status * 128, 0, 255));
}

static ssize_t i8k_hwmon_set_pwm(struct device *dev,
				 struct device_attribute *attr,
				 const char *buf, size_t count)
{
	int index = to_sensor_dev_attr(attr)->index;
	unsigned long val;
	int err;

	err = kstrtoul(buf, 10, &val);
	if (err)
		return err;
	val = clamp_val(DIV_ROUND_CLOSEST(val, 128), 0, 2);

	mutex_lock(&i8k_mutex);
	err = i8k_set_fan(index, val);
	mutex_unlock(&i8k_mutex);

	return err < 0 ? -EIO : count;
}

>>>>>>> refs/remotes/origin/master
static ssize_t i8k_hwmon_show_label(struct device *dev,
				    struct device_attribute *devattr,
				    char *buf)
{
<<<<<<< HEAD
	static const char *labels[4] = {
		"i8k",
=======
	static const char *labels[3] = {
>>>>>>> refs/remotes/origin/master
		"CPU",
		"Left Fan",
		"Right Fan",
	};
	int index = to_sensor_dev_attr(devattr)->index;

	return sprintf(buf, "%s\n", labels[index]);
}

<<<<<<< HEAD
static DEVICE_ATTR(temp1_input, S_IRUGO, i8k_hwmon_show_temp, NULL);
static SENSOR_DEVICE_ATTR(fan1_input, S_IRUGO, i8k_hwmon_show_fan, NULL,
			  I8K_FAN_LEFT);
static SENSOR_DEVICE_ATTR(fan2_input, S_IRUGO, i8k_hwmon_show_fan, NULL,
			  I8K_FAN_RIGHT);
static SENSOR_DEVICE_ATTR(name, S_IRUGO, i8k_hwmon_show_label, NULL, 0);
static SENSOR_DEVICE_ATTR(temp1_label, S_IRUGO, i8k_hwmon_show_label, NULL, 1);
static SENSOR_DEVICE_ATTR(fan1_label, S_IRUGO, i8k_hwmon_show_label, NULL, 2);
static SENSOR_DEVICE_ATTR(fan2_label, S_IRUGO, i8k_hwmon_show_label, NULL, 3);

static void i8k_hwmon_remove_files(struct device *dev)
{
	device_remove_file(dev, &dev_attr_temp1_input);
	device_remove_file(dev, &sensor_dev_attr_fan1_input.dev_attr);
	device_remove_file(dev, &sensor_dev_attr_fan2_input.dev_attr);
	device_remove_file(dev, &sensor_dev_attr_temp1_label.dev_attr);
	device_remove_file(dev, &sensor_dev_attr_fan1_label.dev_attr);
	device_remove_file(dev, &sensor_dev_attr_fan2_label.dev_attr);
	device_remove_file(dev, &sensor_dev_attr_name.dev_attr);
}

=======
static SENSOR_DEVICE_ATTR(temp1_input, S_IRUGO, i8k_hwmon_show_temp, NULL, 0);
static SENSOR_DEVICE_ATTR(temp2_input, S_IRUGO, i8k_hwmon_show_temp, NULL, 1);
static SENSOR_DEVICE_ATTR(temp3_input, S_IRUGO, i8k_hwmon_show_temp, NULL, 2);
static SENSOR_DEVICE_ATTR(temp4_input, S_IRUGO, i8k_hwmon_show_temp, NULL, 3);
static SENSOR_DEVICE_ATTR(fan1_input, S_IRUGO, i8k_hwmon_show_fan, NULL,
			  I8K_FAN_LEFT);
static SENSOR_DEVICE_ATTR(pwm1, S_IRUGO | S_IWUSR, i8k_hwmon_show_pwm,
			  i8k_hwmon_set_pwm, I8K_FAN_LEFT);
static SENSOR_DEVICE_ATTR(fan2_input, S_IRUGO, i8k_hwmon_show_fan, NULL,
			  I8K_FAN_RIGHT);
static SENSOR_DEVICE_ATTR(pwm2, S_IRUGO | S_IWUSR, i8k_hwmon_show_pwm,
			  i8k_hwmon_set_pwm, I8K_FAN_RIGHT);
static SENSOR_DEVICE_ATTR(temp1_label, S_IRUGO, i8k_hwmon_show_label, NULL, 0);
static SENSOR_DEVICE_ATTR(fan1_label, S_IRUGO, i8k_hwmon_show_label, NULL, 1);
static SENSOR_DEVICE_ATTR(fan2_label, S_IRUGO, i8k_hwmon_show_label, NULL, 2);

static struct attribute *i8k_attrs[] = {
	&sensor_dev_attr_temp1_input.dev_attr.attr,	/* 0 */
	&sensor_dev_attr_temp1_label.dev_attr.attr,	/* 1 */
	&sensor_dev_attr_temp2_input.dev_attr.attr,	/* 2 */
	&sensor_dev_attr_temp3_input.dev_attr.attr,	/* 3 */
	&sensor_dev_attr_temp4_input.dev_attr.attr,	/* 4 */
	&sensor_dev_attr_fan1_input.dev_attr.attr,	/* 5 */
	&sensor_dev_attr_pwm1.dev_attr.attr,		/* 6 */
	&sensor_dev_attr_fan1_label.dev_attr.attr,	/* 7 */
	&sensor_dev_attr_fan2_input.dev_attr.attr,	/* 8 */
	&sensor_dev_attr_pwm2.dev_attr.attr,		/* 9 */
	&sensor_dev_attr_fan2_label.dev_attr.attr,	/* 10 */
	NULL
};

static umode_t i8k_is_visible(struct kobject *kobj, struct attribute *attr,
			      int index)
{
	if ((index == 0 || index == 1) &&
	    !(i8k_hwmon_flags & I8K_HWMON_HAVE_TEMP1))
		return 0;
	if (index == 2 && !(i8k_hwmon_flags & I8K_HWMON_HAVE_TEMP2))
		return 0;
	if (index == 3 && !(i8k_hwmon_flags & I8K_HWMON_HAVE_TEMP3))
		return 0;
	if (index == 4 && !(i8k_hwmon_flags & I8K_HWMON_HAVE_TEMP4))
		return 0;
	if (index >= 5 && index <= 7 &&
	    !(i8k_hwmon_flags & I8K_HWMON_HAVE_FAN1))
		return 0;
	if (index >= 8 && index <= 10 &&
	    !(i8k_hwmon_flags & I8K_HWMON_HAVE_FAN2))
		return 0;

	return attr->mode;
}

static const struct attribute_group i8k_group = {
	.attrs = i8k_attrs,
	.is_visible = i8k_is_visible,
};
__ATTRIBUTE_GROUPS(i8k);

>>>>>>> refs/remotes/origin/master
static int __init i8k_init_hwmon(void)
{
	int err;

<<<<<<< HEAD
	i8k_hwmon_dev = hwmon_device_register(NULL);
	if (IS_ERR(i8k_hwmon_dev)) {
		err = PTR_ERR(i8k_hwmon_dev);
		i8k_hwmon_dev = NULL;
		printk(KERN_ERR "i8k: hwmon registration failed (%d)\n", err);
		return err;
	}

	/* Required name attribute */
	err = device_create_file(i8k_hwmon_dev,
				 &sensor_dev_attr_name.dev_attr);
	if (err)
		goto exit_unregister;

	/* CPU temperature attributes, if temperature reading is OK */
	err = i8k_get_temp(0);
	if (err < 0) {
		dev_dbg(i8k_hwmon_dev,
			"Not creating temperature attributes (%d)\n", err);
	} else {
		err = device_create_file(i8k_hwmon_dev, &dev_attr_temp1_input);
		if (err)
			goto exit_remove_files;
		err = device_create_file(i8k_hwmon_dev,
					 &sensor_dev_attr_temp1_label.dev_attr);
		if (err)
			goto exit_remove_files;
	}

	/* Left fan attributes, if left fan is present */
	err = i8k_get_fan_status(I8K_FAN_LEFT);
	if (err < 0) {
		dev_dbg(i8k_hwmon_dev,
			"Not creating %s fan attributes (%d)\n", "left", err);
	} else {
		err = device_create_file(i8k_hwmon_dev,
					 &sensor_dev_attr_fan1_input.dev_attr);
		if (err)
			goto exit_remove_files;
		err = device_create_file(i8k_hwmon_dev,
					 &sensor_dev_attr_fan1_label.dev_attr);
		if (err)
			goto exit_remove_files;
	}

	/* Right fan attributes, if right fan is present */
	err = i8k_get_fan_status(I8K_FAN_RIGHT);
	if (err < 0) {
		dev_dbg(i8k_hwmon_dev,
			"Not creating %s fan attributes (%d)\n", "right", err);
	} else {
		err = device_create_file(i8k_hwmon_dev,
					 &sensor_dev_attr_fan2_input.dev_attr);
		if (err)
			goto exit_remove_files;
		err = device_create_file(i8k_hwmon_dev,
					 &sensor_dev_attr_fan2_label.dev_attr);
		if (err)
			goto exit_remove_files;
	}

	return 0;

 exit_remove_files:
	i8k_hwmon_remove_files(i8k_hwmon_dev);
 exit_unregister:
	hwmon_device_unregister(i8k_hwmon_dev);
	return err;
}

static void __exit i8k_exit_hwmon(void)
{
	i8k_hwmon_remove_files(i8k_hwmon_dev);
	hwmon_device_unregister(i8k_hwmon_dev);
}

static struct dmi_system_id __initdata i8k_dmi_table[] = {
=======
	i8k_hwmon_flags = 0;

	/* CPU temperature attributes, if temperature reading is OK */
	err = i8k_get_temp(0);
	if (err >= 0)
		i8k_hwmon_flags |= I8K_HWMON_HAVE_TEMP1;
	/* check for additional temperature sensors */
	err = i8k_get_temp(1);
	if (err >= 0)
		i8k_hwmon_flags |= I8K_HWMON_HAVE_TEMP2;
	err = i8k_get_temp(2);
	if (err >= 0)
		i8k_hwmon_flags |= I8K_HWMON_HAVE_TEMP3;
	err = i8k_get_temp(3);
	if (err >= 0)
		i8k_hwmon_flags |= I8K_HWMON_HAVE_TEMP4;

	/* Left fan attributes, if left fan is present */
	err = i8k_get_fan_status(I8K_FAN_LEFT);
	if (err >= 0)
		i8k_hwmon_flags |= I8K_HWMON_HAVE_FAN1;

	/* Right fan attributes, if right fan is present */
	err = i8k_get_fan_status(I8K_FAN_RIGHT);
	if (err >= 0)
		i8k_hwmon_flags |= I8K_HWMON_HAVE_FAN2;

	i8k_hwmon_dev = hwmon_device_register_with_groups(NULL, "i8k", NULL,
							  i8k_groups);
	if (IS_ERR(i8k_hwmon_dev)) {
		err = PTR_ERR(i8k_hwmon_dev);
		i8k_hwmon_dev = NULL;
		pr_err("hwmon registration failed (%d)\n", err);
		return err;
	}
	return 0;
}

static struct dmi_system_id i8k_dmi_table[] __initdata = {
>>>>>>> refs/remotes/origin/master
	{
		.ident = "Dell Inspiron",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "Dell Computer"),
			DMI_MATCH(DMI_PRODUCT_NAME, "Inspiron"),
		},
	},
	{
		.ident = "Dell Latitude",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "Dell Computer"),
			DMI_MATCH(DMI_PRODUCT_NAME, "Latitude"),
		},
	},
	{
		.ident = "Dell Inspiron 2",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "Dell Inc."),
			DMI_MATCH(DMI_PRODUCT_NAME, "Inspiron"),
		},
	},
	{
		.ident = "Dell Latitude 2",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "Dell Inc."),
			DMI_MATCH(DMI_PRODUCT_NAME, "Latitude"),
		},
	},
	{	/* UK Inspiron 6400  */
		.ident = "Dell Inspiron 3",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "Dell Inc."),
			DMI_MATCH(DMI_PRODUCT_NAME, "MM061"),
		},
	},
	{
		.ident = "Dell Inspiron 3",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "Dell Inc."),
			DMI_MATCH(DMI_PRODUCT_NAME, "MP061"),
		},
	},
	{
		.ident = "Dell Precision",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "Dell Inc."),
			DMI_MATCH(DMI_PRODUCT_NAME, "Precision"),
		},
	},
	{
		.ident = "Dell Vostro",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "Dell Inc."),
			DMI_MATCH(DMI_PRODUCT_NAME, "Vostro"),
		},
	},
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
        {
        	.ident = "Dell XPS421",
		.matches = {
                        DMI_MATCH(DMI_SYS_VENDOR, "Dell Inc."),
			DMI_MATCH(DMI_PRODUCT_NAME, "XPS L421X"), 
		}
		 { }
=======
=======
	{
		.ident = "Dell XPS421",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "Dell Inc."),
			DMI_MATCH(DMI_PRODUCT_NAME, "XPS L421X"),
		},
	},
>>>>>>> refs/remotes/origin/cm-11.0
        { }
>>>>>>> refs/remotes/origin/cm-10.0
=======
	{
		.ident = "Dell XPS421",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "Dell Inc."),
			DMI_MATCH(DMI_PRODUCT_NAME, "XPS L421X"),
		},
	},
	{
		.ident = "Dell Studio",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "Dell Inc."),
			DMI_MATCH(DMI_PRODUCT_NAME, "Studio"),
		},
		.driver_data = (void *)1,	/* fan multiplier override */
	},
	{
		.ident = "Dell XPS M140",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "Dell Inc."),
			DMI_MATCH(DMI_PRODUCT_NAME, "MXC051"),
		},
		.driver_data = (void *)1,	/* fan multiplier override */
	},
	{ }
>>>>>>> refs/remotes/origin/master
};

/*
 * Probe for the presence of a supported laptop.
 */
static int __init i8k_probe(void)
{
<<<<<<< HEAD
	char buff[4];
	int version;
=======
	const struct dmi_system_id *id;
>>>>>>> refs/remotes/origin/master

	/*
	 * Get DMI information
	 */
	if (!dmi_check_system(i8k_dmi_table)) {
		if (!ignore_dmi && !force)
			return -ENODEV;

<<<<<<< HEAD
		printk(KERN_INFO "i8k: not running on a supported Dell system.\n");
		printk(KERN_INFO "i8k: vendor=%s, model=%s, version=%s\n",
=======
		pr_info("not running on a supported Dell system.\n");
		pr_info("vendor=%s, model=%s, version=%s\n",
>>>>>>> refs/remotes/origin/master
			i8k_get_dmi_data(DMI_SYS_VENDOR),
			i8k_get_dmi_data(DMI_PRODUCT_NAME),
			i8k_get_dmi_data(DMI_BIOS_VERSION));
	}

<<<<<<< HEAD
	strlcpy(bios_version, i8k_get_dmi_data(DMI_BIOS_VERSION), sizeof(bios_version));
=======
	strlcpy(bios_version, i8k_get_dmi_data(DMI_BIOS_VERSION),
		sizeof(bios_version));
>>>>>>> refs/remotes/origin/master

	/*
	 * Get SMM Dell signature
	 */
	if (i8k_get_dell_signature(I8K_SMM_GET_DELL_SIG1) &&
	    i8k_get_dell_signature(I8K_SMM_GET_DELL_SIG2)) {
<<<<<<< HEAD
		printk(KERN_ERR "i8k: unable to get SMM Dell signature\n");
=======
		pr_err("unable to get SMM Dell signature\n");
>>>>>>> refs/remotes/origin/master
		if (!force)
			return -ENODEV;
	}

<<<<<<< HEAD
	/*
	 * Get SMM BIOS version.
	 */
	version = i8k_get_bios_version();
	if (version <= 0) {
		printk(KERN_WARNING "i8k: unable to get SMM BIOS version\n");
	} else {
		buff[0] = (version >> 16) & 0xff;
		buff[1] = (version >> 8) & 0xff;
		buff[2] = (version) & 0xff;
		buff[3] = '\0';
		/*
		 * If DMI BIOS version is unknown use SMM BIOS version.
		 */
		if (!dmi_get_system_info(DMI_BIOS_VERSION))
			strlcpy(bios_version, buff, sizeof(bios_version));

		/*
		 * Check if the two versions match.
		 */
		if (strncmp(buff, bios_version, sizeof(bios_version)) != 0)
			printk(KERN_WARNING "i8k: BIOS version mismatch: %s != %s\n",
				buff, bios_version);
	}
=======
	i8k_fan_mult = fan_mult;
	id = dmi_first_match(i8k_dmi_table);
	if (id && fan_mult == I8K_FAN_MULT && id->driver_data)
		i8k_fan_mult = (unsigned long)id->driver_data;
>>>>>>> refs/remotes/origin/master

	return 0;
}

static int __init i8k_init(void)
{
	struct proc_dir_entry *proc_i8k;
	int err;

	/* Are we running on an supported laptop? */
	if (i8k_probe())
		return -ENODEV;

	/* Register the proc entry */
	proc_i8k = proc_create("i8k", 0, NULL, &i8k_fops);
	if (!proc_i8k)
		return -ENOENT;

	err = i8k_init_hwmon();
	if (err)
		goto exit_remove_proc;

<<<<<<< HEAD
	printk(KERN_INFO
	       "Dell laptop SMM driver v%s Massimo Dal Zotto (dz@debian.org)\n",
	       I8K_VERSION);

=======
>>>>>>> refs/remotes/origin/master
	return 0;

 exit_remove_proc:
	remove_proc_entry("i8k", NULL);
	return err;
}

static void __exit i8k_exit(void)
{
<<<<<<< HEAD
	i8k_exit_hwmon();
=======
	hwmon_device_unregister(i8k_hwmon_dev);
>>>>>>> refs/remotes/origin/master
	remove_proc_entry("i8k", NULL);
}

module_init(i8k_init);
module_exit(i8k_exit);
