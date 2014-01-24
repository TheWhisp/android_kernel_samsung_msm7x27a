/*
 * PPS sysfs support
 *
 *
 * Copyright (C) 2007-2009   Rodolfo Giometti <giometti@linux.it>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


#include <linux/device.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/pps_kernel.h>

/*
 * Attribute functions
 */

<<<<<<< HEAD
static ssize_t pps_show_assert(struct device *dev,
				struct device_attribute *attr, char *buf)
=======
static ssize_t assert_show(struct device *dev, struct device_attribute *attr,
			   char *buf)
>>>>>>> refs/remotes/origin/master
{
	struct pps_device *pps = dev_get_drvdata(dev);

	if (!(pps->info.mode & PPS_CAPTUREASSERT))
		return 0;

	return sprintf(buf, "%lld.%09d#%d\n",
			(long long) pps->assert_tu.sec, pps->assert_tu.nsec,
			pps->assert_sequence);
}
<<<<<<< HEAD

static ssize_t pps_show_clear(struct device *dev,
				struct device_attribute *attr, char *buf)
=======
static DEVICE_ATTR_RO(assert);

static ssize_t clear_show(struct device *dev, struct device_attribute *attr,
			  char *buf)
>>>>>>> refs/remotes/origin/master
{
	struct pps_device *pps = dev_get_drvdata(dev);

	if (!(pps->info.mode & PPS_CAPTURECLEAR))
		return 0;

	return sprintf(buf, "%lld.%09d#%d\n",
			(long long) pps->clear_tu.sec, pps->clear_tu.nsec,
			pps->clear_sequence);
}
<<<<<<< HEAD

static ssize_t pps_show_mode(struct device *dev,
				struct device_attribute *attr, char *buf)
=======
static DEVICE_ATTR_RO(clear);

static ssize_t mode_show(struct device *dev, struct device_attribute *attr,
			 char *buf)
>>>>>>> refs/remotes/origin/master
{
	struct pps_device *pps = dev_get_drvdata(dev);

	return sprintf(buf, "%4x\n", pps->info.mode);
}
<<<<<<< HEAD

static ssize_t pps_show_echo(struct device *dev,
				struct device_attribute *attr, char *buf)
=======
static DEVICE_ATTR_RO(mode);

static ssize_t echo_show(struct device *dev, struct device_attribute *attr,
			 char *buf)
>>>>>>> refs/remotes/origin/master
{
	struct pps_device *pps = dev_get_drvdata(dev);

	return sprintf(buf, "%d\n", !!pps->info.echo);
}
<<<<<<< HEAD

static ssize_t pps_show_name(struct device *dev,
				struct device_attribute *attr, char *buf)
=======
static DEVICE_ATTR_RO(echo);

static ssize_t name_show(struct device *dev, struct device_attribute *attr,
			 char *buf)
>>>>>>> refs/remotes/origin/master
{
	struct pps_device *pps = dev_get_drvdata(dev);

	return sprintf(buf, "%s\n", pps->info.name);
}
<<<<<<< HEAD

static ssize_t pps_show_path(struct device *dev,
				struct device_attribute *attr, char *buf)
=======
static DEVICE_ATTR_RO(name);

static ssize_t path_show(struct device *dev, struct device_attribute *attr,
			 char *buf)
>>>>>>> refs/remotes/origin/master
{
	struct pps_device *pps = dev_get_drvdata(dev);

	return sprintf(buf, "%s\n", pps->info.path);
}
<<<<<<< HEAD

struct device_attribute pps_attrs[] = {
	__ATTR(assert, S_IRUGO, pps_show_assert, NULL),
	__ATTR(clear, S_IRUGO, pps_show_clear, NULL),
	__ATTR(mode, S_IRUGO, pps_show_mode, NULL),
	__ATTR(echo, S_IRUGO, pps_show_echo, NULL),
	__ATTR(name, S_IRUGO, pps_show_name, NULL),
	__ATTR(path, S_IRUGO, pps_show_path, NULL),
	__ATTR_NULL,
=======
static DEVICE_ATTR_RO(path);

static struct attribute *pps_attrs[] = {
	&dev_attr_assert.attr,
	&dev_attr_clear.attr,
	&dev_attr_mode.attr,
	&dev_attr_echo.attr,
	&dev_attr_name.attr,
	&dev_attr_path.attr,
	NULL,
};

static const struct attribute_group pps_group = {
	.attrs = pps_attrs,
};

const struct attribute_group *pps_groups[] = {
	&pps_group,
	NULL,
>>>>>>> refs/remotes/origin/master
};
