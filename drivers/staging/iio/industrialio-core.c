/* The industrial I/O core
 *
 * Copyright (c) 2008 Jonathan Cameron
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * Based on elements of hwmon and input subsystems.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/idr.h>
#include <linux/kdev_t.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/cdev.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include "iio.h"
#include "trigger_consumer.h"

#define IIO_ID_PREFIX "device"
#define IIO_ID_FORMAT IIO_ID_PREFIX "%d"

/* IDR to assign each registered device a unique id*/
static DEFINE_IDA(iio_ida);
/* IDR to allocate character device minor numbers */
static DEFINE_IDA(iio_chrdev_ida);
/* Lock used to protect both of the above */
static DEFINE_SPINLOCK(iio_ida_lock);

dev_t iio_devt;
EXPORT_SYMBOL(iio_devt);
=======
#include <linux/anon_inodes.h>
#include <linux/debugfs.h>
#include "iio.h"
#include "iio_core.h"
#include "iio_core_trigger.h"
#include "sysfs.h"
#include "events.h"

/* IDA to assign each registered device a unique id*/
static DEFINE_IDA(iio_ida);

static dev_t iio_devt;
>>>>>>> refs/remotes/origin/cm-10.0

#define IIO_DEV_MAX 256
struct bus_type iio_bus_type = {
	.name = "iio",
};
EXPORT_SYMBOL(iio_bus_type);

<<<<<<< HEAD
static const char * const iio_chan_type_name_spec_shared[] = {
	[IIO_TIMESTAMP] = "timestamp",
	[IIO_ACCEL] = "accel",
	[IIO_IN] = "in",
	[IIO_CURRENT] = "current",
	[IIO_POWER] = "power",
	[IIO_IN_DIFF] = "in-in",
	[IIO_GYRO] = "gyro",
	[IIO_TEMP] = "temp",
	[IIO_MAGN] = "magn",
	[IIO_INCLI] = "incli",
	[IIO_ROT] = "rot",
	[IIO_INTENSITY] = "intensity",
	[IIO_LIGHT] = "illuminance",
	[IIO_ANGL] = "angl",
};

static const char * const iio_chan_type_name_spec_complex[] = {
	[IIO_IN_DIFF] = "in%d-in%d",
};

static const char * const iio_modifier_names_light[] = {
	[IIO_MOD_LIGHT_BOTH] = "both",
	[IIO_MOD_LIGHT_IR] = "ir",
};

static const char * const iio_modifier_names_axial[] = {
	[IIO_MOD_X] = "x",
	[IIO_MOD_Y] = "y",
	[IIO_MOD_Z] = "z",
=======
static struct dentry *iio_debugfs_dentry;

static const char * const iio_data_type_name[] = {
	[IIO_RAW] = "raw",
	[IIO_PROCESSED] = "input",
};

static const char * const iio_direction[] = {
	[0] = "in",
	[1] = "out",
};

static const char * const iio_chan_type_name_spec[] = {
	[IIO_VOLTAGE] = "voltage",
	[IIO_CURRENT] = "current",
	[IIO_POWER] = "power",
	[IIO_ACCEL] = "accel",
	[IIO_ANGL_VEL] = "anglvel",
	[IIO_MAGN] = "magn",
	[IIO_LIGHT] = "illuminance",
	[IIO_INTENSITY] = "intensity",
	[IIO_PROXIMITY] = "proximity",
	[IIO_TEMP] = "temp",
	[IIO_INCLI] = "incli",
	[IIO_ROT] = "rot",
	[IIO_ANGL] = "angl",
	[IIO_TIMESTAMP] = "timestamp",
	[IIO_CAPACITANCE] = "capacitance",
};

static const char * const iio_modifier_names[] = {
	[IIO_MOD_X] = "x",
	[IIO_MOD_Y] = "y",
	[IIO_MOD_Z] = "z",
	[IIO_MOD_LIGHT_BOTH] = "both",
	[IIO_MOD_LIGHT_IR] = "ir",
>>>>>>> refs/remotes/origin/cm-10.0
};

/* relies on pairs of these shared then separate */
static const char * const iio_chan_info_postfix[] = {
<<<<<<< HEAD
	[IIO_CHAN_INFO_SCALE_SHARED/2] = "scale",
	[IIO_CHAN_INFO_OFFSET_SHARED/2] = "offset",
	[IIO_CHAN_INFO_CALIBSCALE_SHARED/2] = "calibscale",
	[IIO_CHAN_INFO_CALIBBIAS_SHARED/2] = "calibbias",
	[IIO_CHAN_INFO_PEAK_SHARED/2] = "peak_raw",
	[IIO_CHAN_INFO_PEAK_SCALE_SHARED/2] = "peak_scale",
};

int iio_push_event(struct iio_dev *dev_info,
		   int ev_line,
		   int ev_code,
		   s64 timestamp)
{
	struct iio_event_interface *ev_int
		= &dev_info->event_interfaces[ev_line];
	struct iio_detected_event_list *ev;
	int ret = 0;

	/* Does anyone care? */
	mutex_lock(&ev_int->event_list_lock);
	if (test_bit(IIO_BUSY_BIT_POS, &ev_int->handler.flags)) {
		if (ev_int->current_events == ev_int->max_events) {
			mutex_unlock(&ev_int->event_list_lock);
			return 0;
		}
		ev = kmalloc(sizeof(*ev), GFP_KERNEL);
		if (ev == NULL) {
			ret = -ENOMEM;
			mutex_unlock(&ev_int->event_list_lock);
			goto error_ret;
		}
		ev->ev.id = ev_code;
		ev->ev.timestamp = timestamp;

		list_add_tail(&ev->list, &ev_int->det_events);
		ev_int->current_events++;
		mutex_unlock(&ev_int->event_list_lock);
		wake_up_interruptible(&ev_int->wait);
	} else
		mutex_unlock(&ev_int->event_list_lock);

error_ret:
	return ret;
}
EXPORT_SYMBOL(iio_push_event);

=======
	[IIO_CHAN_INFO_SCALE] = "scale",
	[IIO_CHAN_INFO_OFFSET] = "offset",
	[IIO_CHAN_INFO_CALIBSCALE] = "calibscale",
	[IIO_CHAN_INFO_CALIBBIAS] = "calibbias",
	[IIO_CHAN_INFO_PEAK] = "peak_raw",
	[IIO_CHAN_INFO_PEAK_SCALE] = "peak_scale",
	[IIO_CHAN_INFO_QUADRATURE_CORRECTION_RAW] = "quadrature_correction_raw",
	[IIO_CHAN_INFO_AVERAGE_RAW] = "mean_raw",
	[IIO_CHAN_INFO_LOW_PASS_FILTER_3DB_FREQUENCY]
	= "filter_low_pass_3db_frequency",
};

const struct iio_chan_spec
*iio_find_channel_from_si(struct iio_dev *indio_dev, int si)
{
	int i;

	for (i = 0; i < indio_dev->num_channels; i++)
		if (indio_dev->channels[i].scan_index == si)
			return &indio_dev->channels[i];
	return NULL;
}
>>>>>>> refs/remotes/origin/cm-10.0

/* This turns up an awful lot */
ssize_t iio_read_const_attr(struct device *dev,
			    struct device_attribute *attr,
			    char *buf)
{
	return sprintf(buf, "%s\n", to_iio_const_attr(attr)->string);
}
EXPORT_SYMBOL(iio_read_const_attr);

<<<<<<< HEAD

static ssize_t iio_event_chrdev_read(struct file *filep,
				     char __user *buf,
				     size_t count,
				     loff_t *f_ps)
{
	struct iio_event_interface *ev_int = filep->private_data;
	struct iio_detected_event_list *el;
	int ret;
	size_t len;

	mutex_lock(&ev_int->event_list_lock);
	if (list_empty(&ev_int->det_events)) {
		if (filep->f_flags & O_NONBLOCK) {
			ret = -EAGAIN;
			goto error_mutex_unlock;
		}
		mutex_unlock(&ev_int->event_list_lock);
		/* Blocking on device; waiting for something to be there */
		ret = wait_event_interruptible(ev_int->wait,
					       !list_empty(&ev_int
							   ->det_events));
		if (ret)
			goto error_ret;
		/* Single access device so no one else can get the data */
		mutex_lock(&ev_int->event_list_lock);
	}

	el = list_first_entry(&ev_int->det_events,
			      struct iio_detected_event_list,
			      list);
	len = sizeof el->ev;
	if (copy_to_user(buf, &(el->ev), len)) {
		ret = -EFAULT;
		goto error_mutex_unlock;
	}
	list_del(&el->list);
	ev_int->current_events--;
	mutex_unlock(&ev_int->event_list_lock);
	kfree(el);

	return len;

error_mutex_unlock:
	mutex_unlock(&ev_int->event_list_lock);
error_ret:

	return ret;
}

static int iio_event_chrdev_release(struct inode *inode, struct file *filep)
{
	struct iio_handler *hand = iio_cdev_to_handler(inode->i_cdev);
	struct iio_event_interface *ev_int = hand->private;
	struct iio_detected_event_list *el, *t;

	mutex_lock(&ev_int->event_list_lock);
	clear_bit(IIO_BUSY_BIT_POS, &ev_int->handler.flags);
	/*
	 * In order to maintain a clean state for reopening,
	 * clear out any awaiting events. The mask will prevent
	 * any new __iio_push_event calls running.
	 */
	list_for_each_entry_safe(el, t, &ev_int->det_events, list) {
		list_del(&el->list);
		kfree(el);
	}
	mutex_unlock(&ev_int->event_list_lock);
=======
static int __init iio_init(void)
{
	int ret;

	/* Register sysfs bus */
	ret  = bus_register(&iio_bus_type);
	if (ret < 0) {
		printk(KERN_ERR
		       "%s could not register bus type\n",
			__FILE__);
		goto error_nothing;
	}

	ret = alloc_chrdev_region(&iio_devt, 0, IIO_DEV_MAX, "iio");
	if (ret < 0) {
		printk(KERN_ERR "%s: failed to allocate char dev region\n",
		       __FILE__);
		goto error_unregister_bus_type;
	}

	iio_debugfs_dentry = debugfs_create_dir("iio", NULL);

	return 0;

error_unregister_bus_type:
	bus_unregister(&iio_bus_type);
error_nothing:
	return ret;
}

static void __exit iio_exit(void)
{
	if (iio_devt)
		unregister_chrdev_region(iio_devt, IIO_DEV_MAX);
	bus_unregister(&iio_bus_type);
	debugfs_remove(iio_debugfs_dentry);
}

#if defined(CONFIG_DEBUG_FS)
static int iio_debugfs_open(struct inode *inode, struct file *file)
{
	if (inode->i_private)
		file->private_data = inode->i_private;
>>>>>>> refs/remotes/origin/cm-10.0

	return 0;
}

<<<<<<< HEAD
static int iio_event_chrdev_open(struct inode *inode, struct file *filep)
{
	struct iio_handler *hand = iio_cdev_to_handler(inode->i_cdev);
	struct iio_event_interface *ev_int = hand->private;

	mutex_lock(&ev_int->event_list_lock);
	if (test_and_set_bit(IIO_BUSY_BIT_POS, &hand->flags)) {
		fops_put(filep->f_op);
		mutex_unlock(&ev_int->event_list_lock);
		return -EBUSY;
	}
	filep->private_data = hand->private;
	mutex_unlock(&ev_int->event_list_lock);

	return 0;
}

static const struct file_operations iio_event_chrdev_fileops = {
	.read =  iio_event_chrdev_read,
	.release = iio_event_chrdev_release,
	.open = iio_event_chrdev_open,
	.owner = THIS_MODULE,
	.llseek = noop_llseek,
};

static void iio_event_dev_release(struct device *dev)
{
	struct iio_event_interface *ev_int
		= container_of(dev, struct iio_event_interface, dev);
	cdev_del(&ev_int->handler.chrdev);
	iio_device_free_chrdev_minor(MINOR(dev->devt));
};

static struct device_type iio_event_type = {
	.release = iio_event_dev_release,
};

int iio_device_get_chrdev_minor(void)
{
	int ret, val;

ida_again:
	if (unlikely(ida_pre_get(&iio_chrdev_ida, GFP_KERNEL) == 0))
		return -ENOMEM;
	spin_lock(&iio_ida_lock);
	ret = ida_get_new(&iio_chrdev_ida, &val);
	spin_unlock(&iio_ida_lock);
	if (unlikely(ret == -EAGAIN))
		goto ida_again;
	else if (unlikely(ret))
		return ret;
	if (val > IIO_DEV_MAX)
		return -ENOMEM;
	return val;
}

void iio_device_free_chrdev_minor(int val)
{
	spin_lock(&iio_ida_lock);
	ida_remove(&iio_chrdev_ida, val);
	spin_unlock(&iio_ida_lock);
}

static int iio_setup_ev_int(struct iio_event_interface *ev_int,
			    const char *dev_name,
			    int index,
			    struct module *owner,
			    struct device *dev)
{
	int ret, minor;

	ev_int->dev.bus = &iio_bus_type;
	ev_int->dev.parent = dev;
	ev_int->dev.type = &iio_event_type;
	device_initialize(&ev_int->dev);

	minor = iio_device_get_chrdev_minor();
	if (minor < 0) {
		ret = minor;
		goto error_device_put;
	}
	ev_int->dev.devt = MKDEV(MAJOR(iio_devt), minor);
	dev_set_name(&ev_int->dev, "%s:event%d", dev_name, index);

	ret = device_add(&ev_int->dev);
	if (ret)
		goto error_free_minor;

	cdev_init(&ev_int->handler.chrdev, &iio_event_chrdev_fileops);
	ev_int->handler.chrdev.owner = owner;

	mutex_init(&ev_int->event_list_lock);
	/* discussion point - make this variable? */
	ev_int->max_events = 10;
	ev_int->current_events = 0;
	INIT_LIST_HEAD(&ev_int->det_events);
	init_waitqueue_head(&ev_int->wait);
	ev_int->handler.private = ev_int;
	ev_int->handler.flags = 0;

	ret = cdev_add(&ev_int->handler.chrdev, ev_int->dev.devt, 1);
	if (ret)
		goto error_unreg_device;

	return 0;

error_unreg_device:
	device_unregister(&ev_int->dev);
error_free_minor:
	iio_device_free_chrdev_minor(minor);
error_device_put:
	put_device(&ev_int->dev);

	return ret;
}

static void iio_free_ev_int(struct iio_event_interface *ev_int)
{
	device_unregister(&ev_int->dev);
	put_device(&ev_int->dev);
}

static int __init iio_dev_init(void)
{
	int err;

	err = alloc_chrdev_region(&iio_devt, 0, IIO_DEV_MAX, "iio");
	if (err < 0)
		printk(KERN_ERR "%s: failed to allocate char dev region\n",
		       __FILE__);

	return err;
}

static void __exit iio_dev_exit(void)
{
	if (iio_devt)
		unregister_chrdev_region(iio_devt, IIO_DEV_MAX);
}

static int __init iio_init(void)
{
	int ret;

	/* Register sysfs bus */
	ret  = bus_register(&iio_bus_type);
	if (ret < 0) {
		printk(KERN_ERR
		       "%s could not register bus type\n",
			__FILE__);
		goto error_nothing;
	}

	ret = iio_dev_init();
	if (ret < 0)
		goto error_unregister_bus_type;

	return 0;

error_unregister_bus_type:
	bus_unregister(&iio_bus_type);
error_nothing:
	return ret;
}

static void __exit iio_exit(void)
{
	iio_dev_exit();
	bus_unregister(&iio_bus_type);
=======
static ssize_t iio_debugfs_read_reg(struct file *file, char __user *userbuf,
			      size_t count, loff_t *ppos)
{
	struct iio_dev *indio_dev = file->private_data;
	char buf[20];
	unsigned val = 0;
	ssize_t len;
	int ret;

	ret = indio_dev->info->debugfs_reg_access(indio_dev,
						  indio_dev->cached_reg_addr,
						  0, &val);
	if (ret)
		dev_err(indio_dev->dev.parent, "%s: read failed\n", __func__);

	len = snprintf(buf, sizeof(buf), "0x%X\n", val);

	return simple_read_from_buffer(userbuf, count, ppos, buf, len);
}

static ssize_t iio_debugfs_write_reg(struct file *file,
		     const char __user *userbuf, size_t count, loff_t *ppos)
{
	struct iio_dev *indio_dev = file->private_data;
	unsigned reg, val;
	char buf[80];
	int ret;

	count = min_t(size_t, count, (sizeof(buf)-1));
	if (copy_from_user(buf, userbuf, count))
		return -EFAULT;

	buf[count] = 0;

	ret = sscanf(buf, "%i %i", &reg, &val);

	switch (ret) {
	case 1:
		indio_dev->cached_reg_addr = reg;
		break;
	case 2:
		indio_dev->cached_reg_addr = reg;
		ret = indio_dev->info->debugfs_reg_access(indio_dev, reg,
							  val, NULL);
		if (ret) {
			dev_err(indio_dev->dev.parent, "%s: write failed\n",
				__func__);
			return ret;
		}
		break;
	default:
		return -EINVAL;
	}

	return count;
}

static const struct file_operations iio_debugfs_reg_fops = {
	.open = iio_debugfs_open,
	.read = iio_debugfs_read_reg,
	.write = iio_debugfs_write_reg,
};

static void iio_device_unregister_debugfs(struct iio_dev *indio_dev)
{
	debugfs_remove_recursive(indio_dev->debugfs_dentry);
}

static int iio_device_register_debugfs(struct iio_dev *indio_dev)
{
	struct dentry *d;

	if (indio_dev->info->debugfs_reg_access == NULL)
		return 0;

	if (IS_ERR(iio_debugfs_dentry))
		return 0;

	indio_dev->debugfs_dentry =
		debugfs_create_dir(dev_name(&indio_dev->dev),
				   iio_debugfs_dentry);
	if (IS_ERR(indio_dev->debugfs_dentry))
		return PTR_ERR(indio_dev->debugfs_dentry);

	if (indio_dev->debugfs_dentry == NULL) {
		dev_warn(indio_dev->dev.parent,
			 "Failed to create debugfs directory\n");
		return -EFAULT;
	}

	d = debugfs_create_file("direct_reg_access", 0644,
				indio_dev->debugfs_dentry,
				indio_dev, &iio_debugfs_reg_fops);
	if (!d) {
		iio_device_unregister_debugfs(indio_dev);
		return -ENOMEM;
	}

	return 0;
}
#else
static int iio_device_register_debugfs(struct iio_dev *indio_dev)
{
	return 0;
}

static void iio_device_unregister_debugfs(struct iio_dev *indio_dev)
{
}
#endif /* CONFIG_DEBUG_FS */

static ssize_t iio_read_channel_ext_info(struct device *dev,
				     struct device_attribute *attr,
				     char *buf)
{
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct iio_dev_attr *this_attr = to_iio_dev_attr(attr);
	const struct iio_chan_spec_ext_info *ext_info;

	ext_info = &this_attr->c->ext_info[this_attr->address];

	return ext_info->read(indio_dev, this_attr->c, buf);
}

static ssize_t iio_write_channel_ext_info(struct device *dev,
				     struct device_attribute *attr,
				     const char *buf,
					 size_t len)
{
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct iio_dev_attr *this_attr = to_iio_dev_attr(attr);
	const struct iio_chan_spec_ext_info *ext_info;

	ext_info = &this_attr->c->ext_info[this_attr->address];

	return ext_info->write(indio_dev, this_attr->c, buf, len);
>>>>>>> refs/remotes/origin/cm-10.0
}

static ssize_t iio_read_channel_info(struct device *dev,
				     struct device_attribute *attr,
				     char *buf)
{
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct iio_dev_attr *this_attr = to_iio_dev_attr(attr);
	int val, val2;
	int ret = indio_dev->info->read_raw(indio_dev, this_attr->c,
					    &val, &val2, this_attr->address);

	if (ret < 0)
		return ret;

	if (ret == IIO_VAL_INT)
		return sprintf(buf, "%d\n", val);
	else if (ret == IIO_VAL_INT_PLUS_MICRO) {
		if (val2 < 0)
			return sprintf(buf, "-%d.%06u\n", val, -val2);
		else
			return sprintf(buf, "%d.%06u\n", val, val2);
<<<<<<< HEAD
=======
	} else if (ret == IIO_VAL_INT_PLUS_NANO) {
		if (val2 < 0)
			return sprintf(buf, "-%d.%09u\n", val, -val2);
		else
			return sprintf(buf, "%d.%09u\n", val, val2);
>>>>>>> refs/remotes/origin/cm-10.0
	} else
		return 0;
}

static ssize_t iio_write_channel_info(struct device *dev,
				      struct device_attribute *attr,
				      const char *buf,
				      size_t len)
{
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct iio_dev_attr *this_attr = to_iio_dev_attr(attr);
<<<<<<< HEAD
	int ret, integer = 0, micro = 0, micro_mult = 100000;
=======
	int ret, integer = 0, fract = 0, fract_mult = 100000;
>>>>>>> refs/remotes/origin/cm-10.0
	bool integer_part = true, negative = false;

	/* Assumes decimal - precision based on number of digits */
	if (!indio_dev->info->write_raw)
		return -EINVAL;
<<<<<<< HEAD
=======

	if (indio_dev->info->write_raw_get_fmt)
		switch (indio_dev->info->write_raw_get_fmt(indio_dev,
			this_attr->c, this_attr->address)) {
		case IIO_VAL_INT_PLUS_MICRO:
			fract_mult = 100000;
			break;
		case IIO_VAL_INT_PLUS_NANO:
			fract_mult = 100000000;
			break;
		default:
			return -EINVAL;
		}

>>>>>>> refs/remotes/origin/cm-10.0
	if (buf[0] == '-') {
		negative = true;
		buf++;
	}
<<<<<<< HEAD
=======

>>>>>>> refs/remotes/origin/cm-10.0
	while (*buf) {
		if ('0' <= *buf && *buf <= '9') {
			if (integer_part)
				integer = integer*10 + *buf - '0';
			else {
<<<<<<< HEAD
				micro += micro_mult*(*buf - '0');
				if (micro_mult == 1)
					break;
				micro_mult /= 10;
=======
				fract += fract_mult*(*buf - '0');
				if (fract_mult == 1)
					break;
				fract_mult /= 10;
>>>>>>> refs/remotes/origin/cm-10.0
			}
		} else if (*buf == '\n') {
			if (*(buf + 1) == '\0')
				break;
			else
				return -EINVAL;
		} else if (*buf == '.') {
			integer_part = false;
		} else {
			return -EINVAL;
		}
		buf++;
	}
	if (negative) {
		if (integer)
			integer = -integer;
		else
<<<<<<< HEAD
			micro = -micro;
	}

	ret = indio_dev->info->write_raw(indio_dev, this_attr->c,
					 integer, micro, this_attr->address);
=======
			fract = -fract;
	}

	ret = indio_dev->info->write_raw(indio_dev, this_attr->c,
					 integer, fract, this_attr->address);
>>>>>>> refs/remotes/origin/cm-10.0
	if (ret)
		return ret;

	return len;
}

<<<<<<< HEAD
static int __iio_build_postfix(struct iio_chan_spec const *chan,
			       bool generic,
			       const char *postfix,
			       char **result)
{
	char *all_post;
	/* 3 options - generic, extend_name, modified - if generic, extend_name
	* and modified cannot apply.*/

	if (generic || (!chan->modified && !chan->extend_name)) {
		all_post = kasprintf(GFP_KERNEL, "%s", postfix);
	} else if (chan->modified) {
		const char *intermediate;
		switch (chan->type) {
		case IIO_INTENSITY:
			intermediate
				= iio_modifier_names_light[chan->channel2];
			break;
		case IIO_ACCEL:
		case IIO_GYRO:
		case IIO_MAGN:
		case IIO_INCLI:
		case IIO_ROT:
		case IIO_ANGL:
			intermediate
				= iio_modifier_names_axial[chan->channel2];
			break;
		default:
			return -EINVAL;
		}
		if (chan->extend_name)
			all_post = kasprintf(GFP_KERNEL, "%s_%s_%s",
					     intermediate,
					     chan->extend_name,
					     postfix);
		else
			all_post = kasprintf(GFP_KERNEL, "%s_%s",
					     intermediate,
					     postfix);
	} else
		all_post = kasprintf(GFP_KERNEL, "%s_%s", chan->extend_name,
				     postfix);
	if (all_post == NULL)
		return -ENOMEM;
	*result = all_post;
	return 0;
}

=======
static
>>>>>>> refs/remotes/origin/cm-10.0
int __iio_device_attr_init(struct device_attribute *dev_attr,
			   const char *postfix,
			   struct iio_chan_spec const *chan,
			   ssize_t (*readfunc)(struct device *dev,
					       struct device_attribute *attr,
					       char *buf),
			   ssize_t (*writefunc)(struct device *dev,
						struct device_attribute *attr,
						const char *buf,
						size_t len),
			   bool generic)
{
	int ret;
	char *name_format, *full_postfix;
	sysfs_attr_init(&dev_attr->attr);
<<<<<<< HEAD
	ret = __iio_build_postfix(chan, generic, postfix, &full_postfix);
	if (ret)
		goto error_ret;

	/* Special case for types that uses both channel numbers in naming */
	if (chan->type == IIO_IN_DIFF && !generic)
		name_format
			= kasprintf(GFP_KERNEL, "%s_%s",
				    iio_chan_type_name_spec_complex[chan->type],
				    full_postfix);
	else if (generic || !chan->indexed)
		name_format
			= kasprintf(GFP_KERNEL, "%s_%s",
				    iio_chan_type_name_spec_shared[chan->type],
				    full_postfix);
	else
		name_format
			= kasprintf(GFP_KERNEL, "%s%d_%s",
				    iio_chan_type_name_spec_shared[chan->type],
				    chan->channel,
				    full_postfix);

=======

	/* Build up postfix of <extend_name>_<modifier>_postfix */
	if (chan->modified && !generic) {
		if (chan->extend_name)
			full_postfix = kasprintf(GFP_KERNEL, "%s_%s_%s",
						 iio_modifier_names[chan
								    ->channel2],
						 chan->extend_name,
						 postfix);
		else
			full_postfix = kasprintf(GFP_KERNEL, "%s_%s",
						 iio_modifier_names[chan
								    ->channel2],
						 postfix);
	} else {
		if (chan->extend_name == NULL)
			full_postfix = kstrdup(postfix, GFP_KERNEL);
		else
			full_postfix = kasprintf(GFP_KERNEL,
						 "%s_%s",
						 chan->extend_name,
						 postfix);
	}
	if (full_postfix == NULL) {
		ret = -ENOMEM;
		goto error_ret;
	}

	if (chan->differential) { /* Differential can not have modifier */
		if (generic)
			name_format
				= kasprintf(GFP_KERNEL, "%s_%s-%s_%s",
					    iio_direction[chan->output],
					    iio_chan_type_name_spec[chan->type],
					    iio_chan_type_name_spec[chan->type],
					    full_postfix);
		else if (chan->indexed)
			name_format
				= kasprintf(GFP_KERNEL, "%s_%s%d-%s%d_%s",
					    iio_direction[chan->output],
					    iio_chan_type_name_spec[chan->type],
					    chan->channel,
					    iio_chan_type_name_spec[chan->type],
					    chan->channel2,
					    full_postfix);
		else {
			WARN_ON("Differential channels must be indexed\n");
			ret = -EINVAL;
			goto error_free_full_postfix;
		}
	} else { /* Single ended */
		if (generic)
			name_format
				= kasprintf(GFP_KERNEL, "%s_%s_%s",
					    iio_direction[chan->output],
					    iio_chan_type_name_spec[chan->type],
					    full_postfix);
		else if (chan->indexed)
			name_format
				= kasprintf(GFP_KERNEL, "%s_%s%d_%s",
					    iio_direction[chan->output],
					    iio_chan_type_name_spec[chan->type],
					    chan->channel,
					    full_postfix);
		else
			name_format
				= kasprintf(GFP_KERNEL, "%s_%s_%s",
					    iio_direction[chan->output],
					    iio_chan_type_name_spec[chan->type],
					    full_postfix);
	}
>>>>>>> refs/remotes/origin/cm-10.0
	if (name_format == NULL) {
		ret = -ENOMEM;
		goto error_free_full_postfix;
	}
	dev_attr->attr.name = kasprintf(GFP_KERNEL,
					name_format,
					chan->channel,
					chan->channel2);
	if (dev_attr->attr.name == NULL) {
		ret = -ENOMEM;
		goto error_free_name_format;
	}

	if (readfunc) {
		dev_attr->attr.mode |= S_IRUGO;
		dev_attr->show = readfunc;
	}

	if (writefunc) {
		dev_attr->attr.mode |= S_IWUSR;
		dev_attr->store = writefunc;
	}
	kfree(name_format);
	kfree(full_postfix);

	return 0;

error_free_name_format:
	kfree(name_format);
error_free_full_postfix:
	kfree(full_postfix);
error_ret:
	return ret;
}

<<<<<<< HEAD
void __iio_device_attr_deinit(struct device_attribute *dev_attr)
=======
static void __iio_device_attr_deinit(struct device_attribute *dev_attr)
>>>>>>> refs/remotes/origin/cm-10.0
{
	kfree(dev_attr->attr.name);
}

int __iio_add_chan_devattr(const char *postfix,
<<<<<<< HEAD
			   const char *group,
=======
>>>>>>> refs/remotes/origin/cm-10.0
			   struct iio_chan_spec const *chan,
			   ssize_t (*readfunc)(struct device *dev,
					       struct device_attribute *attr,
					       char *buf),
			   ssize_t (*writefunc)(struct device *dev,
						struct device_attribute *attr,
						const char *buf,
						size_t len),
<<<<<<< HEAD
			   int mask,
=======
			   u64 mask,
>>>>>>> refs/remotes/origin/cm-10.0
			   bool generic,
			   struct device *dev,
			   struct list_head *attr_list)
{
	int ret;
	struct iio_dev_attr *iio_attr, *t;

	iio_attr = kzalloc(sizeof *iio_attr, GFP_KERNEL);
	if (iio_attr == NULL) {
		ret = -ENOMEM;
		goto error_ret;
	}
	ret = __iio_device_attr_init(&iio_attr->dev_attr,
				     postfix, chan,
				     readfunc, writefunc, generic);
	if (ret)
		goto error_iio_dev_attr_free;
	iio_attr->c = chan;
	iio_attr->address = mask;
	list_for_each_entry(t, attr_list, l)
		if (strcmp(t->dev_attr.attr.name,
			   iio_attr->dev_attr.attr.name) == 0) {
			if (!generic)
				dev_err(dev, "tried to double register : %s\n",
					t->dev_attr.attr.name);
			ret = -EBUSY;
			goto error_device_attr_deinit;
		}
<<<<<<< HEAD

	ret = sysfs_add_file_to_group(&dev->kobj,
				      &iio_attr->dev_attr.attr, group);
	if (ret < 0)
		goto error_device_attr_deinit;

=======
>>>>>>> refs/remotes/origin/cm-10.0
	list_add(&iio_attr->l, attr_list);

	return 0;

error_device_attr_deinit:
	__iio_device_attr_deinit(&iio_attr->dev_attr);
error_iio_dev_attr_free:
	kfree(iio_attr);
error_ret:
	return ret;
}

<<<<<<< HEAD
static int iio_device_add_channel_sysfs(struct iio_dev *dev_info,
					struct iio_chan_spec const *chan)
{
	int ret, i;


	if (chan->channel < 0)
		return 0;
	if (chan->processed_val)
		ret = __iio_add_chan_devattr("input", NULL, chan,
					     &iio_read_channel_info,
					     NULL,
					     0,
					     0,
					     &dev_info->dev,
					     &dev_info->channel_attr_list);
	else
		ret = __iio_add_chan_devattr("raw", NULL, chan,
					     &iio_read_channel_info,
					     NULL,
					     0,
					     0,
					     &dev_info->dev,
					     &dev_info->channel_attr_list);
	if (ret)
		goto error_ret;

	for_each_set_bit(i, &chan->info_mask, sizeof(long)*8) {
		ret = __iio_add_chan_devattr(iio_chan_info_postfix[i/2],
					     NULL, chan,
					     &iio_read_channel_info,
					     &iio_write_channel_info,
					     (1 << i),
					     !(i%2),
					     &dev_info->dev,
					     &dev_info->channel_attr_list);
=======
static int iio_device_add_channel_sysfs(struct iio_dev *indio_dev,
					struct iio_chan_spec const *chan)
{
	int ret, i, attrcount = 0;
	const struct iio_chan_spec_ext_info *ext_info;

	if (chan->channel < 0)
		return 0;

	ret = __iio_add_chan_devattr(iio_data_type_name[chan->processed_val],
				     chan,
				     &iio_read_channel_info,
				     (chan->output ?
				      &iio_write_channel_info : NULL),
				     0,
				     0,
				     &indio_dev->dev,
				     &indio_dev->channel_attr_list);
	if (ret)
		goto error_ret;
	attrcount++;

	for_each_set_bit(i, &chan->info_mask, sizeof(long)*8) {
		ret = __iio_add_chan_devattr(iio_chan_info_postfix[i/2],
					     chan,
					     &iio_read_channel_info,
					     &iio_write_channel_info,
					     i/2,
					     !(i%2),
					     &indio_dev->dev,
					     &indio_dev->channel_attr_list);
>>>>>>> refs/remotes/origin/cm-10.0
		if (ret == -EBUSY && (i%2 == 0)) {
			ret = 0;
			continue;
		}
		if (ret < 0)
			goto error_ret;
<<<<<<< HEAD
	}
=======
		attrcount++;
	}

	if (chan->ext_info) {
		unsigned int i = 0;
		for (ext_info = chan->ext_info; ext_info->name; ext_info++) {
			ret = __iio_add_chan_devattr(ext_info->name,
					chan,
					ext_info->read ?
					    &iio_read_channel_ext_info : NULL,
					ext_info->write ?
					    &iio_write_channel_ext_info : NULL,
					i,
					ext_info->shared,
					&indio_dev->dev,
					&indio_dev->channel_attr_list);
			i++;
			if (ret == -EBUSY && ext_info->shared)
				continue;

			if (ret)
				goto error_ret;

			attrcount++;
		}
	}

	ret = attrcount;
>>>>>>> refs/remotes/origin/cm-10.0
error_ret:
	return ret;
}

<<<<<<< HEAD
static void iio_device_remove_and_free_read_attr(struct iio_dev *dev_info,
						 struct iio_dev_attr *p)
{
	sysfs_remove_file_from_group(&dev_info->dev.kobj,
				     &p->dev_attr.attr, NULL);
=======
static void iio_device_remove_and_free_read_attr(struct iio_dev *indio_dev,
						 struct iio_dev_attr *p)
{
>>>>>>> refs/remotes/origin/cm-10.0
	kfree(p->dev_attr.attr.name);
	kfree(p);
}

static ssize_t iio_show_dev_name(struct device *dev,
				 struct device_attribute *attr,
				 char *buf)
{
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	return sprintf(buf, "%s\n", indio_dev->name);
}

static DEVICE_ATTR(name, S_IRUGO, iio_show_dev_name, NULL);

<<<<<<< HEAD
static int iio_device_register_sysfs(struct iio_dev *dev_info)
{
	int i, ret = 0;
	struct iio_dev_attr *p, *n;

	if (dev_info->info->attrs) {
		ret = sysfs_create_group(&dev_info->dev.kobj,
					 dev_info->info->attrs);
		if (ret) {
			dev_err(dev_info->dev.parent,
				"Failed to register sysfs hooks\n");
			goto error_ret;
		}
	}

	/*
	 * New channel registration method - relies on the fact a group does
	 *  not need to be initialized if it is name is NULL.
	 */
	INIT_LIST_HEAD(&dev_info->channel_attr_list);
	if (dev_info->channels)
		for (i = 0; i < dev_info->num_channels; i++) {
			ret = iio_device_add_channel_sysfs(dev_info,
							   &dev_info
							   ->channels[i]);
			if (ret < 0)
				goto error_clear_attrs;
		}
	if (dev_info->name) {
		ret = sysfs_add_file_to_group(&dev_info->dev.kobj,
					      &dev_attr_name.attr,
					      NULL);
		if (ret)
			goto error_clear_attrs;
	}
	return 0;

error_clear_attrs:
	list_for_each_entry_safe(p, n,
				 &dev_info->channel_attr_list, l) {
		list_del(&p->l);
		iio_device_remove_and_free_read_attr(dev_info, p);
	}
	if (dev_info->info->attrs)
		sysfs_remove_group(&dev_info->dev.kobj, dev_info->info->attrs);
error_ret:
	return ret;

}

static void iio_device_unregister_sysfs(struct iio_dev *dev_info)
{

	struct iio_dev_attr *p, *n;
	if (dev_info->name)
		sysfs_remove_file_from_group(&dev_info->dev.kobj,
					     &dev_attr_name.attr,
					     NULL);
	list_for_each_entry_safe(p, n, &dev_info->channel_attr_list, l) {
		list_del(&p->l);
		iio_device_remove_and_free_read_attr(dev_info, p);
	}

	if (dev_info->info->attrs)
		sysfs_remove_group(&dev_info->dev.kobj, dev_info->info->attrs);
}

/* Return a negative errno on failure */
int iio_get_new_ida_val(struct ida *this_ida)
{
	int ret;
	int val;

ida_again:
	if (unlikely(ida_pre_get(this_ida, GFP_KERNEL) == 0))
		return -ENOMEM;

	spin_lock(&iio_ida_lock);
	ret = ida_get_new(this_ida, &val);
	spin_unlock(&iio_ida_lock);
	if (unlikely(ret == -EAGAIN))
		goto ida_again;
	else if (unlikely(ret))
		return ret;

	return val;
}
EXPORT_SYMBOL(iio_get_new_ida_val);

void iio_free_ida_val(struct ida *this_ida, int id)
{
	spin_lock(&iio_ida_lock);
	ida_remove(this_ida, id);
	spin_unlock(&iio_ida_lock);
}
EXPORT_SYMBOL(iio_free_ida_val);

static const char * const iio_ev_type_text[] = {
	[IIO_EV_TYPE_THRESH] = "thresh",
	[IIO_EV_TYPE_MAG] = "mag",
	[IIO_EV_TYPE_ROC] = "roc"
};

static const char * const iio_ev_dir_text[] = {
	[IIO_EV_DIR_EITHER] = "either",
	[IIO_EV_DIR_RISING] = "rising",
	[IIO_EV_DIR_FALLING] = "falling"
};

static ssize_t iio_ev_state_store(struct device *dev,
				  struct device_attribute *attr,
				  const char *buf,
				  size_t len)
{
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct iio_dev_attr *this_attr = to_iio_dev_attr(attr);
	int ret;
	bool val;

	ret = strtobool(buf, &val);
	if (ret < 0)
		return ret;

	ret = indio_dev->info->write_event_config(indio_dev,
						  this_attr->address,
						  val);
	return (ret < 0) ? ret : len;
}

static ssize_t iio_ev_state_show(struct device *dev,
				 struct device_attribute *attr,
				 char *buf)
{
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct iio_dev_attr *this_attr = to_iio_dev_attr(attr);
	int val = indio_dev->info->read_event_config(indio_dev,
						     this_attr->address);

	if (val < 0)
		return val;
	else
		return sprintf(buf, "%d\n", val);
}

static ssize_t iio_ev_value_show(struct device *dev,
				 struct device_attribute *attr,
				 char *buf)
{
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct iio_dev_attr *this_attr = to_iio_dev_attr(attr);
	int val, ret;

	ret = indio_dev->info->read_event_value(indio_dev,
						this_attr->address, &val);
	if (ret < 0)
		return ret;

	return sprintf(buf, "%d\n", val);
}

static ssize_t iio_ev_value_store(struct device *dev,
				  struct device_attribute *attr,
				  const char *buf,
				  size_t len)
{
	struct iio_dev *indio_dev = dev_get_drvdata(dev);
	struct iio_dev_attr *this_attr = to_iio_dev_attr(attr);
	unsigned long val;
	int ret;

	ret = strict_strtoul(buf, 10, &val);
	if (ret)
		return ret;

	ret = indio_dev->info->write_event_value(indio_dev, this_attr->address,
						 val);
	if (ret < 0)
		return ret;

	return len;
}

static int iio_device_add_event_sysfs(struct iio_dev *dev_info,
				      struct iio_chan_spec const *chan)
{

	int ret = 0, i, mask;
	char *postfix;
	if (!chan->event_mask)
		return 0;

	for_each_set_bit(i, &chan->event_mask, sizeof(chan->event_mask)*8) {
		postfix = kasprintf(GFP_KERNEL, "%s_%s_en",
				    iio_ev_type_text[i/IIO_EV_TYPE_MAX],
				    iio_ev_dir_text[i%IIO_EV_TYPE_MAX]);
		if (postfix == NULL) {
			ret = -ENOMEM;
			goto error_ret;
		}
		switch (chan->type) {
			/* Switch this to a table at some point */
		case IIO_IN:
			mask = IIO_UNMOD_EVENT_CODE(chan->type, chan->channel,
						    i/IIO_EV_TYPE_MAX,
						    i%IIO_EV_TYPE_MAX);
			break;
		case IIO_ACCEL:
			mask = IIO_MOD_EVENT_CODE(chan->type, 0, chan->channel,
						  i/IIO_EV_TYPE_MAX,
						  i%IIO_EV_TYPE_MAX);
			break;
		case IIO_IN_DIFF:
			mask = IIO_MOD_EVENT_CODE(chan->type, chan->channel,
						  chan->channel2,
						  i/IIO_EV_TYPE_MAX,
						  i%IIO_EV_TYPE_MAX);
			break;
		default:
			printk(KERN_INFO "currently unhandled type of event\n");
		}
		ret = __iio_add_chan_devattr(postfix,
					     NULL,
					     chan,
					     &iio_ev_state_show,
					     iio_ev_state_store,
					     mask,
					     /*HACK. - limits us to one
					       event interface - fix by
					       extending the bitmask - but
					       how far*/
					     0,
					     &dev_info->event_interfaces[0].dev,
					     &dev_info->event_interfaces[0].
					     dev_attr_list);
		kfree(postfix);
		if (ret)
			goto error_ret;

		postfix = kasprintf(GFP_KERNEL, "%s_%s_value",
				    iio_ev_type_text[i/IIO_EV_TYPE_MAX],
				    iio_ev_dir_text[i%IIO_EV_TYPE_MAX]);
		if (postfix == NULL) {
			ret = -ENOMEM;
			goto error_ret;
		}
		ret = __iio_add_chan_devattr(postfix, NULL, chan,
					     iio_ev_value_show,
					     iio_ev_value_store,
					     mask,
					     0,
					     &dev_info->event_interfaces[0]
					     .dev,
					     &dev_info->event_interfaces[0]
					     .dev_attr_list);
		kfree(postfix);
		if (ret)
			goto error_ret;

	}

error_ret:
	return ret;
}

static inline void __iio_remove_all_event_sysfs(struct iio_dev *dev_info,
						const char *groupname,
						int num)
{
	struct iio_dev_attr *p, *n;
	list_for_each_entry_safe(p, n,
				 &dev_info->event_interfaces[num].
				 dev_attr_list, l) {
		sysfs_remove_file_from_group(&dev_info
					     ->event_interfaces[num].dev.kobj,
					     &p->dev_attr.attr,
					     groupname);
		kfree(p->dev_attr.attr.name);
		kfree(p);
	}
}

static inline int __iio_add_event_config_attrs(struct iio_dev *dev_info, int i)
{
	int j;
	int ret;
	INIT_LIST_HEAD(&dev_info->event_interfaces[0].dev_attr_list);
	/* Dynically created from the channels array */
	if (dev_info->channels) {
		for (j = 0; j < dev_info->num_channels; j++) {
			ret = iio_device_add_event_sysfs(dev_info,
							 &dev_info
							 ->channels[j]);
			if (ret)
				goto error_clear_attrs;
		}
	}
	return 0;

error_clear_attrs:
	__iio_remove_all_event_sysfs(dev_info, NULL, i);

	return ret;
}

static inline int __iio_remove_event_config_attrs(struct iio_dev *dev_info,
						  int i)
{
	__iio_remove_all_event_sysfs(dev_info, NULL, i);
	return 0;
}

static int iio_device_register_eventset(struct iio_dev *dev_info)
{
	int ret = 0, i, j;

	if (dev_info->info->num_interrupt_lines == 0)
		return 0;

	dev_info->event_interfaces =
		kzalloc(sizeof(struct iio_event_interface)
			*dev_info->info->num_interrupt_lines,
			GFP_KERNEL);
	if (dev_info->event_interfaces == NULL) {
		ret = -ENOMEM;
		goto error_ret;
	}

	for (i = 0; i < dev_info->info->num_interrupt_lines; i++) {
		ret = iio_setup_ev_int(&dev_info->event_interfaces[i],
				       dev_name(&dev_info->dev),
				       i,
				       dev_info->info->driver_module,
				       &dev_info->dev);
		if (ret) {
			dev_err(&dev_info->dev,
				"Could not get chrdev interface\n");
			goto error_free_setup_ev_ints;
		}

		dev_set_drvdata(&dev_info->event_interfaces[i].dev,
				(void *)dev_info);

		if (dev_info->info->event_attrs != NULL)
			ret = sysfs_create_group(&dev_info
						 ->event_interfaces[i]
						 .dev.kobj,
						 &dev_info->info
						 ->event_attrs[i]);

		if (ret) {
			dev_err(&dev_info->dev,
				"Failed to register sysfs for event attrs");
			goto error_remove_sysfs_interfaces;
		}
	}

	for (i = 0; i < dev_info->info->num_interrupt_lines; i++) {
		ret = __iio_add_event_config_attrs(dev_info, i);
		if (ret)
			goto error_unregister_config_attrs;
	}

	return 0;

error_unregister_config_attrs:
	for (j = 0; j < i; j++)
		__iio_remove_event_config_attrs(dev_info, i);
	i = dev_info->info->num_interrupt_lines - 1;
error_remove_sysfs_interfaces:
	for (j = 0; j < i; j++)
		if (dev_info->info->event_attrs != NULL)
			sysfs_remove_group(&dev_info
				   ->event_interfaces[j].dev.kobj,
				   &dev_info->info->event_attrs[j]);
error_free_setup_ev_ints:
	for (j = 0; j < i; j++)
		iio_free_ev_int(&dev_info->event_interfaces[j]);
	kfree(dev_info->event_interfaces);
error_ret:

	return ret;
}

static void iio_device_unregister_eventset(struct iio_dev *dev_info)
{
	int i;

	if (dev_info->info->num_interrupt_lines == 0)
		return;
	for (i = 0; i < dev_info->info->num_interrupt_lines; i++) {
		__iio_remove_event_config_attrs(dev_info, i);
		if (dev_info->info->event_attrs != NULL)
			sysfs_remove_group(&dev_info
					   ->event_interfaces[i].dev.kobj,
					   &dev_info->info->event_attrs[i]);
	}

	for (i = 0; i < dev_info->info->num_interrupt_lines; i++)
		iio_free_ev_int(&dev_info->event_interfaces[i]);
	kfree(dev_info->event_interfaces);
=======
static int iio_device_register_sysfs(struct iio_dev *indio_dev)
{
	int i, ret = 0, attrcount, attrn, attrcount_orig = 0;
	struct iio_dev_attr *p, *n;
	struct attribute **attr;

	/* First count elements in any existing group */
	if (indio_dev->info->attrs) {
		attr = indio_dev->info->attrs->attrs;
		while (*attr++ != NULL)
			attrcount_orig++;
	}
	attrcount = attrcount_orig;
	/*
	 * New channel registration method - relies on the fact a group does
	 * not need to be initialized if it is name is NULL.
	 */
	INIT_LIST_HEAD(&indio_dev->channel_attr_list);
	if (indio_dev->channels)
		for (i = 0; i < indio_dev->num_channels; i++) {
			ret = iio_device_add_channel_sysfs(indio_dev,
							   &indio_dev
							   ->channels[i]);
			if (ret < 0)
				goto error_clear_attrs;
			attrcount += ret;
		}

	if (indio_dev->name)
		attrcount++;

	indio_dev->chan_attr_group.attrs = kcalloc(attrcount + 1,
						   sizeof(indio_dev->chan_attr_group.attrs[0]),
						   GFP_KERNEL);
	if (indio_dev->chan_attr_group.attrs == NULL) {
		ret = -ENOMEM;
		goto error_clear_attrs;
	}
	/* Copy across original attributes */
	if (indio_dev->info->attrs)
		memcpy(indio_dev->chan_attr_group.attrs,
		       indio_dev->info->attrs->attrs,
		       sizeof(indio_dev->chan_attr_group.attrs[0])
		       *attrcount_orig);
	attrn = attrcount_orig;
	/* Add all elements from the list. */
	list_for_each_entry(p, &indio_dev->channel_attr_list, l)
		indio_dev->chan_attr_group.attrs[attrn++] = &p->dev_attr.attr;
	if (indio_dev->name)
		indio_dev->chan_attr_group.attrs[attrn++] = &dev_attr_name.attr;

	indio_dev->groups[indio_dev->groupcounter++] =
		&indio_dev->chan_attr_group;

	return 0;

error_clear_attrs:
	list_for_each_entry_safe(p, n,
				 &indio_dev->channel_attr_list, l) {
		list_del(&p->l);
		iio_device_remove_and_free_read_attr(indio_dev, p);
	}

	return ret;
}

static void iio_device_unregister_sysfs(struct iio_dev *indio_dev)
{

	struct iio_dev_attr *p, *n;

	list_for_each_entry_safe(p, n, &indio_dev->channel_attr_list, l) {
		list_del(&p->l);
		iio_device_remove_and_free_read_attr(indio_dev, p);
	}
	kfree(indio_dev->chan_attr_group.attrs);
>>>>>>> refs/remotes/origin/cm-10.0
}

static void iio_dev_release(struct device *device)
{
<<<<<<< HEAD
	iio_put();
	kfree(to_iio_dev(device));
=======
	struct iio_dev *indio_dev = container_of(device, struct iio_dev, dev);
	cdev_del(&indio_dev->chrdev);
	if (indio_dev->modes & INDIO_BUFFER_TRIGGERED)
		iio_device_unregister_trigger_consumer(indio_dev);
	iio_device_unregister_eventset(indio_dev);
	iio_device_unregister_sysfs(indio_dev);
	iio_device_unregister_debugfs(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
}

static struct device_type iio_dev_type = {
	.name = "iio_device",
	.release = iio_dev_release,
};

struct iio_dev *iio_allocate_device(int sizeof_priv)
{
	struct iio_dev *dev;
	size_t alloc_size;

	alloc_size = sizeof(struct iio_dev);
	if (sizeof_priv) {
		alloc_size = ALIGN(alloc_size, IIO_ALIGN);
		alloc_size += sizeof_priv;
	}
	/* ensure 32-byte alignment of whole construct ? */
	alloc_size += IIO_ALIGN - 1;

	dev = kzalloc(alloc_size, GFP_KERNEL);

	if (dev) {
<<<<<<< HEAD
=======
		dev->dev.groups = dev->groups;
>>>>>>> refs/remotes/origin/cm-10.0
		dev->dev.type = &iio_dev_type;
		dev->dev.bus = &iio_bus_type;
		device_initialize(&dev->dev);
		dev_set_drvdata(&dev->dev, (void *)dev);
		mutex_init(&dev->mlock);
<<<<<<< HEAD
		iio_get();
=======
		mutex_init(&dev->info_exist_lock);

		dev->id = ida_simple_get(&iio_ida, 0, 0, GFP_KERNEL);
		if (dev->id < 0) {
			/* cannot use a dev_err as the name isn't available */
			printk(KERN_ERR "Failed to get id\n");
			kfree(dev);
			return NULL;
		}
		dev_set_name(&dev->dev, "iio:device%d", dev->id);
>>>>>>> refs/remotes/origin/cm-10.0
	}

	return dev;
}
EXPORT_SYMBOL(iio_allocate_device);

void iio_free_device(struct iio_dev *dev)
{
<<<<<<< HEAD
	if (dev)
		iio_put_device(dev);
}
EXPORT_SYMBOL(iio_free_device);

int iio_device_register(struct iio_dev *dev_info)
{
	int ret;

	dev_info->id = iio_get_new_ida_val(&iio_ida);
	if (dev_info->id < 0) {
		ret = dev_info->id;
		dev_err(&dev_info->dev, "Failed to get id\n");
		goto error_ret;
	}
	dev_set_name(&dev_info->dev, "device%d", dev_info->id);

	ret = device_add(&dev_info->dev);
	if (ret)
		goto error_free_ida;
	ret = iio_device_register_sysfs(dev_info);
	if (ret) {
		dev_err(dev_info->dev.parent,
			"Failed to register sysfs interfaces\n");
		goto error_del_device;
	}
	ret = iio_device_register_eventset(dev_info);
	if (ret) {
		dev_err(dev_info->dev.parent,
			"Failed to register event set\n");
		goto error_free_sysfs;
	}
	if (dev_info->modes & INDIO_RING_TRIGGERED)
		iio_device_register_trigger_consumer(dev_info);

	return 0;

error_free_sysfs:
	iio_device_unregister_sysfs(dev_info);
error_del_device:
	device_del(&dev_info->dev);
error_free_ida:
	iio_free_ida_val(&iio_ida, dev_info->id);
=======
	if (dev) {
		ida_simple_remove(&iio_ida, dev->id);
		kfree(dev);
	}
}
EXPORT_SYMBOL(iio_free_device);

/**
 * iio_chrdev_open() - chrdev file open for buffer access and ioctls
 **/
static int iio_chrdev_open(struct inode *inode, struct file *filp)
{
	struct iio_dev *indio_dev = container_of(inode->i_cdev,
						struct iio_dev, chrdev);

	if (test_and_set_bit(IIO_BUSY_BIT_POS, &indio_dev->flags))
		return -EBUSY;

	filp->private_data = indio_dev;

	return 0;
}

/**
 * iio_chrdev_release() - chrdev file close buffer access and ioctls
 **/
static int iio_chrdev_release(struct inode *inode, struct file *filp)
{
	struct iio_dev *indio_dev = container_of(inode->i_cdev,
						struct iio_dev, chrdev);
	clear_bit(IIO_BUSY_BIT_POS, &indio_dev->flags);
	return 0;
}

/* Somewhat of a cross file organization violation - ioctls here are actually
 * event related */
static long iio_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct iio_dev *indio_dev = filp->private_data;
	int __user *ip = (int __user *)arg;
	int fd;

	if (cmd == IIO_GET_EVENT_FD_IOCTL) {
		fd = iio_event_getfd(indio_dev);
		if (copy_to_user(ip, &fd, sizeof(fd)))
			return -EFAULT;
		return 0;
	}
	return -EINVAL;
}

static const struct file_operations iio_buffer_fileops = {
	.read = iio_buffer_read_first_n_outer_addr,
	.release = iio_chrdev_release,
	.open = iio_chrdev_open,
	.poll = iio_buffer_poll_addr,
	.owner = THIS_MODULE,
	.llseek = noop_llseek,
	.unlocked_ioctl = iio_ioctl,
	.compat_ioctl = iio_ioctl,
};

static const struct iio_buffer_setup_ops noop_ring_setup_ops;

int iio_device_register(struct iio_dev *indio_dev)
{
	int ret;

	/* configure elements for the chrdev */
	indio_dev->dev.devt = MKDEV(MAJOR(iio_devt), indio_dev->id);

	ret = iio_device_register_debugfs(indio_dev);
	if (ret) {
		dev_err(indio_dev->dev.parent,
			"Failed to register debugfs interfaces\n");
		goto error_ret;
	}
	ret = iio_device_register_sysfs(indio_dev);
	if (ret) {
		dev_err(indio_dev->dev.parent,
			"Failed to register sysfs interfaces\n");
		goto error_unreg_debugfs;
	}
	ret = iio_device_register_eventset(indio_dev);
	if (ret) {
		dev_err(indio_dev->dev.parent,
			"Failed to register event set\n");
		goto error_free_sysfs;
	}
	if (indio_dev->modes & INDIO_BUFFER_TRIGGERED)
		iio_device_register_trigger_consumer(indio_dev);

	if ((indio_dev->modes & INDIO_ALL_BUFFER_MODES) &&
		indio_dev->setup_ops == NULL)
		indio_dev->setup_ops = &noop_ring_setup_ops;

	ret = device_add(&indio_dev->dev);
	if (ret < 0)
		goto error_unreg_eventset;
	cdev_init(&indio_dev->chrdev, &iio_buffer_fileops);
	indio_dev->chrdev.owner = indio_dev->info->driver_module;
	ret = cdev_add(&indio_dev->chrdev, indio_dev->dev.devt, 1);
	if (ret < 0)
		goto error_del_device;
	return 0;

error_del_device:
	device_del(&indio_dev->dev);
error_unreg_eventset:
	iio_device_unregister_eventset(indio_dev);
error_free_sysfs:
	iio_device_unregister_sysfs(indio_dev);
error_unreg_debugfs:
	iio_device_unregister_debugfs(indio_dev);
>>>>>>> refs/remotes/origin/cm-10.0
error_ret:
	return ret;
}
EXPORT_SYMBOL(iio_device_register);

<<<<<<< HEAD
void iio_device_unregister(struct iio_dev *dev_info)
{
	if (dev_info->modes & INDIO_RING_TRIGGERED)
		iio_device_unregister_trigger_consumer(dev_info);
	iio_device_unregister_eventset(dev_info);
	iio_device_unregister_sysfs(dev_info);
	iio_free_ida_val(&iio_ida, dev_info->id);
	device_unregister(&dev_info->dev);
}
EXPORT_SYMBOL(iio_device_unregister);

void iio_put(void)
{
	module_put(THIS_MODULE);
}

void iio_get(void)
{
	__module_get(THIS_MODULE);
}

=======
void iio_device_unregister(struct iio_dev *indio_dev)
{
	mutex_lock(&indio_dev->info_exist_lock);
	indio_dev->info = NULL;
	mutex_unlock(&indio_dev->info_exist_lock);
	device_unregister(&indio_dev->dev);
}
EXPORT_SYMBOL(iio_device_unregister);
>>>>>>> refs/remotes/origin/cm-10.0
subsys_initcall(iio_init);
module_exit(iio_exit);

MODULE_AUTHOR("Jonathan Cameron <jic23@cam.ac.uk>");
MODULE_DESCRIPTION("Industrial I/O core");
MODULE_LICENSE("GPL");
