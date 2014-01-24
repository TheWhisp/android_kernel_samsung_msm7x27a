/*
    comedi/comedi_fops.c
    comedi kernel module

    COMEDI - Linux Control and Measurement Device Interface
    Copyright (C) 1997-2000 David A. Schleef <ds@schleef.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
<<<<<<< HEAD

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#undef DEBUG

#define __NO_VERSION__
#include "comedi_fops.h"
=======
*/

>>>>>>> refs/remotes/origin/master
#include "comedi_compat32.h"

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/fcntl.h>
#include <linux/delay.h>
<<<<<<< HEAD
#include <linux/ioport.h>
=======
>>>>>>> refs/remotes/origin/master
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/kmod.h>
#include <linux/poll.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/vmalloc.h>
#include <linux/fs.h>
#include "comedidev.h"
#include <linux/cdev.h>
#include <linux/stat.h>

#include <linux/io.h>
#include <linux/uaccess.h>

<<<<<<< HEAD
#include "internal.h"

MODULE_AUTHOR("http://www.comedi.org");
MODULE_DESCRIPTION("Comedi core module");
MODULE_LICENSE("GPL");

#ifdef CONFIG_COMEDI_DEBUG
int comedi_debug;
EXPORT_SYMBOL(comedi_debug);
module_param(comedi_debug, int, 0644);
#endif

<<<<<<< HEAD
int comedi_autoconfig = 1;
=======
bool comedi_autoconfig = 1;
>>>>>>> refs/remotes/origin/cm-10.0
module_param(comedi_autoconfig, bool, 0444);

static int comedi_num_legacy_minors;
module_param(comedi_num_legacy_minors, int, 0444);

static DEFINE_SPINLOCK(comedi_file_info_table_lock);
static struct comedi_device_file_info
*comedi_file_info_table[COMEDI_NUM_MINORS];

static int do_devconfig_ioctl(struct comedi_device *dev,
			      struct comedi_devconfig __user *arg);
static int do_bufconfig_ioctl(struct comedi_device *dev,
			      struct comedi_bufconfig __user *arg);
static int do_devinfo_ioctl(struct comedi_device *dev,
			    struct comedi_devinfo __user *arg,
			    struct file *file);
static int do_subdinfo_ioctl(struct comedi_device *dev,
			     struct comedi_subdinfo __user *arg, void *file);
static int do_chaninfo_ioctl(struct comedi_device *dev,
			     struct comedi_chaninfo __user *arg);
static int do_bufinfo_ioctl(struct comedi_device *dev,
			    struct comedi_bufinfo __user *arg, void *file);
static int do_cmd_ioctl(struct comedi_device *dev,
			struct comedi_cmd __user *arg, void *file);
static int do_lock_ioctl(struct comedi_device *dev, unsigned int arg,
			 void *file);
static int do_unlock_ioctl(struct comedi_device *dev, unsigned int arg,
			   void *file);
static int do_cancel_ioctl(struct comedi_device *dev, unsigned int arg,
			   void *file);
static int do_cmdtest_ioctl(struct comedi_device *dev,
			    struct comedi_cmd __user *arg, void *file);
static int do_insnlist_ioctl(struct comedi_device *dev,
			     struct comedi_insnlist __user *arg, void *file);
static int do_insn_ioctl(struct comedi_device *dev,
			 struct comedi_insn __user *arg, void *file);
static int do_poll_ioctl(struct comedi_device *dev, unsigned int subd,
			 void *file);

<<<<<<< HEAD
extern void do_become_nonbusy(struct comedi_device *dev,
=======
static void do_become_nonbusy(struct comedi_device *dev,
>>>>>>> refs/remotes/origin/cm-10.0
			      struct comedi_subdevice *s);
static int do_cancel(struct comedi_device *dev, struct comedi_subdevice *s);

static int comedi_fasync(int fd, struct file *file, int on);

static int is_device_busy(struct comedi_device *dev);
static int resize_async_buffer(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_async *async, unsigned new_size);

/* declarations for sysfs attribute files */
static struct device_attribute dev_attr_max_read_buffer_kb;
static struct device_attribute dev_attr_read_buffer_kb;
static struct device_attribute dev_attr_max_write_buffer_kb;
static struct device_attribute dev_attr_write_buffer_kb;

static long comedi_unlocked_ioctl(struct file *file, unsigned int cmd,
				  unsigned long arg)
{
	const unsigned minor = iminor(file->f_dentry->d_inode);
	struct comedi_device_file_info *dev_file_info =
	    comedi_get_device_file_info(minor);
	struct comedi_device *dev;
	int rc;

	if (dev_file_info == NULL || dev_file_info->device == NULL)
		return -ENODEV;
	dev = dev_file_info->device;

	mutex_lock(&dev->mutex);

	/* Device config is special, because it must work on
	 * an unconfigured device. */
	if (cmd == COMEDI_DEVCONFIG) {
		if (minor >= COMEDI_NUM_BOARD_MINORS) {
			/* Device config not appropriate on non-board minors. */
			rc = -ENOTTY;
			goto done;
		}
		rc = do_devconfig_ioctl(dev,
					(struct comedi_devconfig __user *)arg);
		if (rc == 0)
			/* Evade comedi_auto_unconfig(). */
			dev_file_info->hardware_device = NULL;
		goto done;
	}

	if (!dev->attached) {
		DPRINTK("no driver configured on /dev/comedi%i\n", dev->minor);
		rc = -ENODEV;
		goto done;
	}

	switch (cmd) {
	case COMEDI_BUFCONFIG:
		rc = do_bufconfig_ioctl(dev,
					(struct comedi_bufconfig __user *)arg);
		break;
	case COMEDI_DEVINFO:
		rc = do_devinfo_ioctl(dev, (struct comedi_devinfo __user *)arg,
				      file);
		break;
	case COMEDI_SUBDINFO:
		rc = do_subdinfo_ioctl(dev,
				       (struct comedi_subdinfo __user *)arg,
				       file);
		break;
	case COMEDI_CHANINFO:
		rc = do_chaninfo_ioctl(dev, (void __user *)arg);
		break;
	case COMEDI_RANGEINFO:
		rc = do_rangeinfo_ioctl(dev, (void __user *)arg);
		break;
	case COMEDI_BUFINFO:
		rc = do_bufinfo_ioctl(dev,
				      (struct comedi_bufinfo __user *)arg,
				      file);
		break;
	case COMEDI_LOCK:
		rc = do_lock_ioctl(dev, arg, file);
		break;
	case COMEDI_UNLOCK:
		rc = do_unlock_ioctl(dev, arg, file);
		break;
	case COMEDI_CANCEL:
		rc = do_cancel_ioctl(dev, arg, file);
		break;
	case COMEDI_CMD:
		rc = do_cmd_ioctl(dev, (struct comedi_cmd __user *)arg, file);
		break;
	case COMEDI_CMDTEST:
		rc = do_cmdtest_ioctl(dev, (struct comedi_cmd __user *)arg,
				      file);
		break;
	case COMEDI_INSNLIST:
		rc = do_insnlist_ioctl(dev,
				       (struct comedi_insnlist __user *)arg,
				       file);
		break;
	case COMEDI_INSN:
		rc = do_insn_ioctl(dev, (struct comedi_insn __user *)arg,
				   file);
		break;
	case COMEDI_POLL:
		rc = do_poll_ioctl(dev, arg, file);
		break;
	default:
		rc = -ENOTTY;
		break;
	}

done:
	mutex_unlock(&dev->mutex);
	return rc;
=======
#include "comedi_internal.h"

#define COMEDI_NUM_MINORS 0x100
#define COMEDI_NUM_SUBDEVICE_MINORS	\
	(COMEDI_NUM_MINORS - COMEDI_NUM_BOARD_MINORS)

static int comedi_num_legacy_minors;
module_param(comedi_num_legacy_minors, int, S_IRUGO);
MODULE_PARM_DESC(comedi_num_legacy_minors,
		 "number of comedi minor devices to reserve for non-auto-configured devices (default 0)"
		);

unsigned int comedi_default_buf_size_kb = CONFIG_COMEDI_DEFAULT_BUF_SIZE_KB;
module_param(comedi_default_buf_size_kb, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(comedi_default_buf_size_kb,
		 "default asynchronous buffer size in KiB (default "
		 __MODULE_STRING(CONFIG_COMEDI_DEFAULT_BUF_SIZE_KB) ")");

unsigned int comedi_default_buf_maxsize_kb
	= CONFIG_COMEDI_DEFAULT_BUF_MAXSIZE_KB;
module_param(comedi_default_buf_maxsize_kb, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(comedi_default_buf_maxsize_kb,
		 "default maximum size of asynchronous buffer in KiB (default "
		 __MODULE_STRING(CONFIG_COMEDI_DEFAULT_BUF_MAXSIZE_KB) ")");

static DEFINE_MUTEX(comedi_board_minor_table_lock);
static struct comedi_device
*comedi_board_minor_table[COMEDI_NUM_BOARD_MINORS];

static DEFINE_MUTEX(comedi_subdevice_minor_table_lock);
/* Note: indexed by minor - COMEDI_NUM_BOARD_MINORS. */
static struct comedi_subdevice
*comedi_subdevice_minor_table[COMEDI_NUM_SUBDEVICE_MINORS];

static struct class *comedi_class;
static struct cdev comedi_cdev;

static void comedi_device_init(struct comedi_device *dev)
{
	kref_init(&dev->refcount);
	spin_lock_init(&dev->spinlock);
	mutex_init(&dev->mutex);
	init_rwsem(&dev->attach_lock);
	dev->minor = -1;
}

static void comedi_dev_kref_release(struct kref *kref)
{
	struct comedi_device *dev =
		container_of(kref, struct comedi_device, refcount);

	mutex_destroy(&dev->mutex);
	put_device(dev->class_dev);
	kfree(dev);
}

int comedi_dev_put(struct comedi_device *dev)
{
	if (dev)
		return kref_put(&dev->refcount, comedi_dev_kref_release);
	return 1;
}
EXPORT_SYMBOL_GPL(comedi_dev_put);

static struct comedi_device *comedi_dev_get(struct comedi_device *dev)
{
	if (dev)
		kref_get(&dev->refcount);
	return dev;
}

static void comedi_device_cleanup(struct comedi_device *dev)
{
	struct module *driver_module = NULL;

	if (dev == NULL)
		return;
	mutex_lock(&dev->mutex);
	if (dev->attached)
		driver_module = dev->driver->module;
	comedi_device_detach(dev);
	if (driver_module && dev->use_count)
		module_put(driver_module);
	mutex_unlock(&dev->mutex);
}

static bool comedi_clear_board_dev(struct comedi_device *dev)
{
	unsigned int i = dev->minor;
	bool cleared = false;

	mutex_lock(&comedi_board_minor_table_lock);
	if (dev == comedi_board_minor_table[i]) {
		comedi_board_minor_table[i] = NULL;
		cleared = true;
	}
	mutex_unlock(&comedi_board_minor_table_lock);
	return cleared;
}

static struct comedi_device *comedi_clear_board_minor(unsigned minor)
{
	struct comedi_device *dev;

	mutex_lock(&comedi_board_minor_table_lock);
	dev = comedi_board_minor_table[minor];
	comedi_board_minor_table[minor] = NULL;
	mutex_unlock(&comedi_board_minor_table_lock);
	return dev;
}

static void comedi_free_board_dev(struct comedi_device *dev)
{
	if (dev) {
		comedi_device_cleanup(dev);
		if (dev->class_dev) {
			device_destroy(comedi_class,
				       MKDEV(COMEDI_MAJOR, dev->minor));
		}
		comedi_dev_put(dev);
	}
}

static struct comedi_subdevice
*comedi_subdevice_from_minor(const struct comedi_device *dev, unsigned minor)
{
	struct comedi_subdevice *s;
	unsigned int i = minor - COMEDI_NUM_BOARD_MINORS;

	BUG_ON(i >= COMEDI_NUM_SUBDEVICE_MINORS);
	mutex_lock(&comedi_subdevice_minor_table_lock);
	s = comedi_subdevice_minor_table[i];
	if (s && s->device != dev)
		s = NULL;
	mutex_unlock(&comedi_subdevice_minor_table_lock);
	return s;
}

static struct comedi_device *comedi_dev_get_from_board_minor(unsigned minor)
{
	struct comedi_device *dev;

	BUG_ON(minor >= COMEDI_NUM_BOARD_MINORS);
	mutex_lock(&comedi_board_minor_table_lock);
	dev = comedi_dev_get(comedi_board_minor_table[minor]);
	mutex_unlock(&comedi_board_minor_table_lock);
	return dev;
}

static struct comedi_device *comedi_dev_get_from_subdevice_minor(unsigned minor)
{
	struct comedi_device *dev;
	struct comedi_subdevice *s;
	unsigned int i = minor - COMEDI_NUM_BOARD_MINORS;

	BUG_ON(i >= COMEDI_NUM_SUBDEVICE_MINORS);
	mutex_lock(&comedi_subdevice_minor_table_lock);
	s = comedi_subdevice_minor_table[i];
	dev = comedi_dev_get(s ? s->device : NULL);
	mutex_unlock(&comedi_subdevice_minor_table_lock);
	return dev;
}

struct comedi_device *comedi_dev_get_from_minor(unsigned minor)
{
	if (minor < COMEDI_NUM_BOARD_MINORS)
		return comedi_dev_get_from_board_minor(minor);
	else
		return comedi_dev_get_from_subdevice_minor(minor);
}
EXPORT_SYMBOL_GPL(comedi_dev_get_from_minor);

static struct comedi_subdevice *
comedi_read_subdevice(const struct comedi_device *dev, unsigned int minor)
{
	struct comedi_subdevice *s;

	if (minor >= COMEDI_NUM_BOARD_MINORS) {
		s = comedi_subdevice_from_minor(dev, minor);
		if (s == NULL || (s->subdev_flags & SDF_CMD_READ))
			return s;
	}
	return dev->read_subdev;
}

static struct comedi_subdevice *
comedi_write_subdevice(const struct comedi_device *dev, unsigned int minor)
{
	struct comedi_subdevice *s;

	if (minor >= COMEDI_NUM_BOARD_MINORS) {
		s = comedi_subdevice_from_minor(dev, minor);
		if (s == NULL || (s->subdev_flags & SDF_CMD_WRITE))
			return s;
	}
	return dev->write_subdev;
}

static int resize_async_buffer(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_async *async, unsigned new_size)
{
	int retval;

	if (new_size > async->max_bufsize)
		return -EPERM;

	if (s->busy) {
		dev_dbg(dev->class_dev,
			"subdevice is busy, cannot resize buffer\n");
		return -EBUSY;
	}
	if (comedi_buf_is_mmapped(async)) {
		dev_dbg(dev->class_dev,
			"subdevice is mmapped, cannot resize buffer\n");
		return -EBUSY;
	}

	/* make sure buffer is an integral number of pages
	 * (we round up) */
	new_size = (new_size + PAGE_SIZE - 1) & PAGE_MASK;

	retval = comedi_buf_alloc(dev, s, new_size);
	if (retval < 0)
		return retval;

	if (s->buf_change) {
		retval = s->buf_change(dev, s, new_size);
		if (retval < 0)
			return retval;
	}

	dev_dbg(dev->class_dev, "subd %d buffer resized to %i bytes\n",
		s->index, async->prealloc_bufsz);
	return 0;
}

/* sysfs attribute files */

static ssize_t max_read_buffer_kb_show(struct device *csdev,
				       struct device_attribute *attr, char *buf)
{
	unsigned int minor = MINOR(csdev->devt);
	struct comedi_device *dev;
	struct comedi_subdevice *s;
	unsigned int size = 0;

	dev = comedi_dev_get_from_minor(minor);
	if (!dev)
		return -ENODEV;

	mutex_lock(&dev->mutex);
	s = comedi_read_subdevice(dev, minor);
	if (s && (s->subdev_flags & SDF_CMD_READ) && s->async)
		size = s->async->max_bufsize / 1024;
	mutex_unlock(&dev->mutex);

	comedi_dev_put(dev);
	return snprintf(buf, PAGE_SIZE, "%i\n", size);
}

static ssize_t max_read_buffer_kb_store(struct device *csdev,
					struct device_attribute *attr,
					const char *buf, size_t count)
{
	unsigned int minor = MINOR(csdev->devt);
	struct comedi_device *dev;
	struct comedi_subdevice *s;
	unsigned int size;
	int err;

	err = kstrtouint(buf, 10, &size);
	if (err)
		return err;
	if (size > (UINT_MAX / 1024))
		return -EINVAL;
	size *= 1024;

	dev = comedi_dev_get_from_minor(minor);
	if (!dev)
		return -ENODEV;

	mutex_lock(&dev->mutex);
	s = comedi_read_subdevice(dev, minor);
	if (s && (s->subdev_flags & SDF_CMD_READ) && s->async)
		s->async->max_bufsize = size;
	else
		err = -EINVAL;
	mutex_unlock(&dev->mutex);

	comedi_dev_put(dev);
	return err ? err : count;
}
static DEVICE_ATTR_RW(max_read_buffer_kb);

static ssize_t read_buffer_kb_show(struct device *csdev,
				   struct device_attribute *attr, char *buf)
{
	unsigned int minor = MINOR(csdev->devt);
	struct comedi_device *dev;
	struct comedi_subdevice *s;
	unsigned int size = 0;

	dev = comedi_dev_get_from_minor(minor);
	if (!dev)
		return -ENODEV;

	mutex_lock(&dev->mutex);
	s = comedi_read_subdevice(dev, minor);
	if (s && (s->subdev_flags & SDF_CMD_READ) && s->async)
		size = s->async->prealloc_bufsz / 1024;
	mutex_unlock(&dev->mutex);

	comedi_dev_put(dev);
	return snprintf(buf, PAGE_SIZE, "%i\n", size);
}

static ssize_t read_buffer_kb_store(struct device *csdev,
				    struct device_attribute *attr,
				    const char *buf, size_t count)
{
	unsigned int minor = MINOR(csdev->devt);
	struct comedi_device *dev;
	struct comedi_subdevice *s;
	unsigned int size;
	int err;

	err = kstrtouint(buf, 10, &size);
	if (err)
		return err;
	if (size > (UINT_MAX / 1024))
		return -EINVAL;
	size *= 1024;

	dev = comedi_dev_get_from_minor(minor);
	if (!dev)
		return -ENODEV;

	mutex_lock(&dev->mutex);
	s = comedi_read_subdevice(dev, minor);
	if (s && (s->subdev_flags & SDF_CMD_READ) && s->async)
		err = resize_async_buffer(dev, s, s->async, size);
	else
		err = -EINVAL;
	mutex_unlock(&dev->mutex);

	comedi_dev_put(dev);
	return err ? err : count;
}
static DEVICE_ATTR_RW(read_buffer_kb);

static ssize_t max_write_buffer_kb_show(struct device *csdev,
					struct device_attribute *attr,
					char *buf)
{
	unsigned int minor = MINOR(csdev->devt);
	struct comedi_device *dev;
	struct comedi_subdevice *s;
	unsigned int size = 0;

	dev = comedi_dev_get_from_minor(minor);
	if (!dev)
		return -ENODEV;

	mutex_lock(&dev->mutex);
	s = comedi_write_subdevice(dev, minor);
	if (s && (s->subdev_flags & SDF_CMD_WRITE) && s->async)
		size = s->async->max_bufsize / 1024;
	mutex_unlock(&dev->mutex);

	comedi_dev_put(dev);
	return snprintf(buf, PAGE_SIZE, "%i\n", size);
}

static ssize_t max_write_buffer_kb_store(struct device *csdev,
					 struct device_attribute *attr,
					 const char *buf, size_t count)
{
	unsigned int minor = MINOR(csdev->devt);
	struct comedi_device *dev;
	struct comedi_subdevice *s;
	unsigned int size;
	int err;

	err = kstrtouint(buf, 10, &size);
	if (err)
		return err;
	if (size > (UINT_MAX / 1024))
		return -EINVAL;
	size *= 1024;

	dev = comedi_dev_get_from_minor(minor);
	if (!dev)
		return -ENODEV;

	mutex_lock(&dev->mutex);
	s = comedi_write_subdevice(dev, minor);
	if (s && (s->subdev_flags & SDF_CMD_WRITE) && s->async)
		s->async->max_bufsize = size;
	else
		err = -EINVAL;
	mutex_unlock(&dev->mutex);

	comedi_dev_put(dev);
	return err ? err : count;
}
static DEVICE_ATTR_RW(max_write_buffer_kb);

static ssize_t write_buffer_kb_show(struct device *csdev,
				    struct device_attribute *attr, char *buf)
{
	unsigned int minor = MINOR(csdev->devt);
	struct comedi_device *dev;
	struct comedi_subdevice *s;
	unsigned int size = 0;

	dev = comedi_dev_get_from_minor(minor);
	if (!dev)
		return -ENODEV;

	mutex_lock(&dev->mutex);
	s = comedi_write_subdevice(dev, minor);
	if (s && (s->subdev_flags & SDF_CMD_WRITE) && s->async)
		size = s->async->prealloc_bufsz / 1024;
	mutex_unlock(&dev->mutex);

	comedi_dev_put(dev);
	return snprintf(buf, PAGE_SIZE, "%i\n", size);
}

static ssize_t write_buffer_kb_store(struct device *csdev,
				     struct device_attribute *attr,
				     const char *buf, size_t count)
{
	unsigned int minor = MINOR(csdev->devt);
	struct comedi_device *dev;
	struct comedi_subdevice *s;
	unsigned int size;
	int err;

	err = kstrtouint(buf, 10, &size);
	if (err)
		return err;
	if (size > (UINT_MAX / 1024))
		return -EINVAL;
	size *= 1024;

	dev = comedi_dev_get_from_minor(minor);
	if (!dev)
		return -ENODEV;

	mutex_lock(&dev->mutex);
	s = comedi_write_subdevice(dev, minor);
	if (s && (s->subdev_flags & SDF_CMD_WRITE) && s->async)
		err = resize_async_buffer(dev, s, s->async, size);
	else
		err = -EINVAL;
	mutex_unlock(&dev->mutex);

	comedi_dev_put(dev);
	return err ? err : count;
}
static DEVICE_ATTR_RW(write_buffer_kb);

static struct attribute *comedi_dev_attrs[] = {
	&dev_attr_max_read_buffer_kb.attr,
	&dev_attr_read_buffer_kb.attr,
	&dev_attr_max_write_buffer_kb.attr,
	&dev_attr_write_buffer_kb.attr,
	NULL,
};
ATTRIBUTE_GROUPS(comedi_dev);

static void comedi_set_subdevice_runflags(struct comedi_subdevice *s,
					  unsigned mask, unsigned bits)
{
	unsigned long flags;

	spin_lock_irqsave(&s->spin_lock, flags);
	s->runflags &= ~mask;
	s->runflags |= (bits & mask);
	spin_unlock_irqrestore(&s->spin_lock, flags);
}

static unsigned comedi_get_subdevice_runflags(struct comedi_subdevice *s)
{
	unsigned long flags;
	unsigned runflags;

	spin_lock_irqsave(&s->spin_lock, flags);
	runflags = s->runflags;
	spin_unlock_irqrestore(&s->spin_lock, flags);
	return runflags;
}

bool comedi_is_subdevice_running(struct comedi_subdevice *s)
{
	unsigned runflags = comedi_get_subdevice_runflags(s);

	return (runflags & SRF_RUNNING) ? true : false;
}
EXPORT_SYMBOL_GPL(comedi_is_subdevice_running);

static bool comedi_is_subdevice_in_error(struct comedi_subdevice *s)
{
	unsigned runflags = comedi_get_subdevice_runflags(s);

	return (runflags & SRF_ERROR) ? true : false;
}

static bool comedi_is_subdevice_idle(struct comedi_subdevice *s)
{
	unsigned runflags = comedi_get_subdevice_runflags(s);

	return (runflags & (SRF_ERROR | SRF_RUNNING)) ? false : true;
}

/**
 * comedi_alloc_spriv() - Allocate memory for the subdevice private data.
 * @s: comedi_subdevice struct
 * @size: size of the memory to allocate
 *
 * This also sets the subdevice runflags to allow the core to automatically
 * free the private data during the detach.
 */
void *comedi_alloc_spriv(struct comedi_subdevice *s, size_t size)
{
	s->private = kzalloc(size, GFP_KERNEL);
	if (s->private)
		s->runflags |= SRF_FREE_SPRIV;
	return s->private;
}
EXPORT_SYMBOL_GPL(comedi_alloc_spriv);

/*
   This function restores a subdevice to an idle state.
 */
static void do_become_nonbusy(struct comedi_device *dev,
			      struct comedi_subdevice *s)
{
	struct comedi_async *async = s->async;

	comedi_set_subdevice_runflags(s, SRF_RUNNING, 0);
	if (async) {
		comedi_buf_reset(async);
		async->inttrig = NULL;
		kfree(async->cmd.chanlist);
		async->cmd.chanlist = NULL;
		s->busy = NULL;
		wake_up_interruptible_all(&s->async->wait_head);
	} else {
		dev_err(dev->class_dev,
			"BUG: (?) do_become_nonbusy called with async=NULL\n");
		s->busy = NULL;
	}
}

static int do_cancel(struct comedi_device *dev, struct comedi_subdevice *s)
{
	int ret = 0;

	if (comedi_is_subdevice_running(s) && s->cancel)
		ret = s->cancel(dev, s);

	do_become_nonbusy(dev, s);

	return ret;
}

void comedi_device_cancel_all(struct comedi_device *dev)
{
	struct comedi_subdevice *s;
	int i;

	if (!dev->attached)
		return;

	for (i = 0; i < dev->n_subdevices; i++) {
		s = &dev->subdevices[i];
		if (s->async)
			do_cancel(dev, s);
	}
}

static int is_device_busy(struct comedi_device *dev)
{
	struct comedi_subdevice *s;
	int i;

	if (!dev->attached)
		return 0;

	for (i = 0; i < dev->n_subdevices; i++) {
		s = &dev->subdevices[i];
		if (s->busy)
			return 1;
		if (s->async && comedi_buf_is_mmapped(s->async))
			return 1;
	}

	return 0;
>>>>>>> refs/remotes/origin/master
}

/*
	COMEDI_DEVCONFIG
	device config ioctl

	arg:
		pointer to devconfig structure

	reads:
		devconfig structure at arg

	writes:
		none
*/
static int do_devconfig_ioctl(struct comedi_device *dev,
			      struct comedi_devconfig __user *arg)
{
	struct comedi_devconfig it;
<<<<<<< HEAD
	int ret;
	unsigned char *aux_data = NULL;
	int aux_len;
=======
>>>>>>> refs/remotes/origin/master

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	if (arg == NULL) {
		if (is_device_busy(dev))
			return -EBUSY;
		if (dev->attached) {
			struct module *driver_module = dev->driver->module;
			comedi_device_detach(dev);
			module_put(driver_module);
		}
		return 0;
	}

<<<<<<< HEAD
	if (copy_from_user(&it, arg, sizeof(struct comedi_devconfig)))
=======
	if (copy_from_user(&it, arg, sizeof(it)))
>>>>>>> refs/remotes/origin/master
		return -EFAULT;

	it.board_name[COMEDI_NAMELEN - 1] = 0;

<<<<<<< HEAD
	if (comedi_aux_data(it.options, 0) &&
	    it.options[COMEDI_DEVCONF_AUX_DATA_LENGTH]) {
		int bit_shift;
		aux_len = it.options[COMEDI_DEVCONF_AUX_DATA_LENGTH];
		if (aux_len < 0)
			return -EFAULT;

		aux_data = vmalloc(aux_len);
		if (!aux_data)
			return -ENOMEM;

		if (copy_from_user(aux_data,
				   comedi_aux_data(it.options, 0), aux_len)) {
			vfree(aux_data);
			return -EFAULT;
		}
		it.options[COMEDI_DEVCONF_AUX_DATA_LO] =
		    (unsigned long)aux_data;
		if (sizeof(void *) > sizeof(int)) {
			bit_shift = sizeof(int) * 8;
			it.options[COMEDI_DEVCONF_AUX_DATA_HI] =
			    ((unsigned long)aux_data) >> bit_shift;
		} else
			it.options[COMEDI_DEVCONF_AUX_DATA_HI] = 0;
	}

	ret = comedi_device_attach(dev, &it);
	if (ret == 0) {
		if (!try_module_get(dev->driver->module)) {
			comedi_device_detach(dev);
<<<<<<< HEAD
			ret = -ENOSYS;
=======
			return -ENOSYS;
>>>>>>> refs/remotes/origin/cm-10.0
		}
	}

	if (aux_data)
		vfree(aux_data);

	return ret;
=======
	if (it.options[COMEDI_DEVCONF_AUX_DATA_LENGTH]) {
		dev_warn(dev->class_dev,
			 "comedi_config --init_data is deprecated\n");
		return -EINVAL;
	}

	if (dev->minor >= comedi_num_legacy_minors)
		/* don't re-use dynamically allocated comedi devices */
		return -EBUSY;

	/* This increments the driver module count on success. */
	return comedi_device_attach(dev, &it);
>>>>>>> refs/remotes/origin/master
}

/*
	COMEDI_BUFCONFIG
	buffer configuration ioctl

	arg:
		pointer to bufconfig structure

	reads:
		bufconfig at arg

	writes:
		modified bufconfig at arg

*/
static int do_bufconfig_ioctl(struct comedi_device *dev,
			      struct comedi_bufconfig __user *arg)
{
	struct comedi_bufconfig bc;
	struct comedi_async *async;
	struct comedi_subdevice *s;
	int retval = 0;

<<<<<<< HEAD
	if (copy_from_user(&bc, arg, sizeof(struct comedi_bufconfig)))
		return -EFAULT;

	if (bc.subdevice >= dev->n_subdevices || bc.subdevice < 0)
		return -EINVAL;

	s = dev->subdevices + bc.subdevice;
	async = s->async;

	if (!async) {
		DPRINTK("subdevice does not have async capability\n");
=======
	if (copy_from_user(&bc, arg, sizeof(bc)))
		return -EFAULT;

	if (bc.subdevice >= dev->n_subdevices)
		return -EINVAL;

	s = &dev->subdevices[bc.subdevice];
	async = s->async;

	if (!async) {
		dev_dbg(dev->class_dev,
			"subdevice does not have async capability\n");
>>>>>>> refs/remotes/origin/master
		bc.size = 0;
		bc.maximum_size = 0;
		goto copyback;
	}

	if (bc.maximum_size) {
		if (!capable(CAP_SYS_ADMIN))
			return -EPERM;

		async->max_bufsize = bc.maximum_size;
	}

	if (bc.size) {
		retval = resize_async_buffer(dev, s, async, bc.size);
		if (retval < 0)
			return retval;
	}

	bc.size = async->prealloc_bufsz;
	bc.maximum_size = async->max_bufsize;

copyback:
<<<<<<< HEAD
	if (copy_to_user(arg, &bc, sizeof(struct comedi_bufconfig)))
=======
	if (copy_to_user(arg, &bc, sizeof(bc)))
>>>>>>> refs/remotes/origin/master
		return -EFAULT;

	return 0;
}

/*
	COMEDI_DEVINFO
	device info ioctl

	arg:
		pointer to devinfo structure

	reads:
		none

	writes:
		devinfo structure

*/
static int do_devinfo_ioctl(struct comedi_device *dev,
			    struct comedi_devinfo __user *arg,
			    struct file *file)
{
<<<<<<< HEAD
	struct comedi_devinfo devinfo;
	const unsigned minor = iminor(file->f_dentry->d_inode);
	struct comedi_device_file_info *dev_file_info =
	    comedi_get_device_file_info(minor);
	struct comedi_subdevice *read_subdev =
	    comedi_get_read_subdevice(dev_file_info);
	struct comedi_subdevice *write_subdev =
	    comedi_get_write_subdevice(dev_file_info);
=======
	const unsigned minor = iminor(file_inode(file));
	struct comedi_subdevice *s;
	struct comedi_devinfo devinfo;
>>>>>>> refs/remotes/origin/master

	memset(&devinfo, 0, sizeof(devinfo));

	/* fill devinfo structure */
	devinfo.version_code = COMEDI_VERSION_CODE;
	devinfo.n_subdevs = dev->n_subdevices;
	strlcpy(devinfo.driver_name, dev->driver->driver_name, COMEDI_NAMELEN);
	strlcpy(devinfo.board_name, dev->board_name, COMEDI_NAMELEN);

<<<<<<< HEAD
	if (read_subdev)
		devinfo.read_subdevice = read_subdev - dev->subdevices;
	else
		devinfo.read_subdevice = -1;

	if (write_subdev)
		devinfo.write_subdevice = write_subdev - dev->subdevices;
	else
		devinfo.write_subdevice = -1;

	if (copy_to_user(arg, &devinfo, sizeof(struct comedi_devinfo)))
=======
	s = comedi_read_subdevice(dev, minor);
	if (s)
		devinfo.read_subdevice = s->index;
	else
		devinfo.read_subdevice = -1;

	s = comedi_write_subdevice(dev, minor);
	if (s)
		devinfo.write_subdevice = s->index;
	else
		devinfo.write_subdevice = -1;

	if (copy_to_user(arg, &devinfo, sizeof(devinfo)))
>>>>>>> refs/remotes/origin/master
		return -EFAULT;

	return 0;
}

/*
	COMEDI_SUBDINFO
	subdevice info ioctl

	arg:
		pointer to array of subdevice info structures

	reads:
		none

	writes:
		array of subdevice info structures at arg

*/
static int do_subdinfo_ioctl(struct comedi_device *dev,
			     struct comedi_subdinfo __user *arg, void *file)
{
	int ret, i;
	struct comedi_subdinfo *tmp, *us;
	struct comedi_subdevice *s;

<<<<<<< HEAD
	tmp =
	    kcalloc(dev->n_subdevices, sizeof(struct comedi_subdinfo),
		    GFP_KERNEL);
=======
	tmp = kcalloc(dev->n_subdevices, sizeof(*tmp), GFP_KERNEL);
>>>>>>> refs/remotes/origin/master
	if (!tmp)
		return -ENOMEM;

	/* fill subdinfo structs */
	for (i = 0; i < dev->n_subdevices; i++) {
<<<<<<< HEAD
		s = dev->subdevices + i;
=======
		s = &dev->subdevices[i];
>>>>>>> refs/remotes/origin/master
		us = tmp + i;

		us->type = s->type;
		us->n_chan = s->n_chan;
		us->subd_flags = s->subdev_flags;
<<<<<<< HEAD
		if (comedi_get_subdevice_runflags(s) & SRF_RUNNING)
=======
		if (comedi_is_subdevice_running(s))
>>>>>>> refs/remotes/origin/master
			us->subd_flags |= SDF_RUNNING;
#define TIMER_nanosec 5		/* backwards compatibility */
		us->timer_type = TIMER_nanosec;
		us->len_chanlist = s->len_chanlist;
		us->maxdata = s->maxdata;
		if (s->range_table) {
			us->range_type =
			    (i << 24) | (0 << 16) | (s->range_table->length);
		} else {
			us->range_type = 0;	/* XXX */
		}
<<<<<<< HEAD
		us->flags = s->flags;
=======
>>>>>>> refs/remotes/origin/master

		if (s->busy)
			us->subd_flags |= SDF_BUSY;
		if (s->busy == file)
			us->subd_flags |= SDF_BUSY_OWNER;
		if (s->lock)
			us->subd_flags |= SDF_LOCKED;
		if (s->lock == file)
			us->subd_flags |= SDF_LOCK_OWNER;
		if (!s->maxdata && s->maxdata_list)
			us->subd_flags |= SDF_MAXDATA;
<<<<<<< HEAD
		if (s->flaglist)
			us->subd_flags |= SDF_FLAGS;
=======
>>>>>>> refs/remotes/origin/master
		if (s->range_table_list)
			us->subd_flags |= SDF_RANGETYPE;
		if (s->do_cmd)
			us->subd_flags |= SDF_CMD;

		if (s->insn_bits != &insn_inval)
			us->insn_bits_support = COMEDI_SUPPORTED;
		else
			us->insn_bits_support = COMEDI_UNSUPPORTED;
<<<<<<< HEAD

		us->settling_time_0 = s->settling_time_0;
	}

	ret = copy_to_user(arg, tmp,
			   dev->n_subdevices * sizeof(struct comedi_subdinfo));
=======
	}

	ret = copy_to_user(arg, tmp, dev->n_subdevices * sizeof(*tmp));
>>>>>>> refs/remotes/origin/master

	kfree(tmp);

	return ret ? -EFAULT : 0;
}

/*
	COMEDI_CHANINFO
	subdevice info ioctl

	arg:
		pointer to chaninfo structure

	reads:
		chaninfo structure at arg

	writes:
		arrays at elements of chaninfo structure

*/
static int do_chaninfo_ioctl(struct comedi_device *dev,
			     struct comedi_chaninfo __user *arg)
{
	struct comedi_subdevice *s;
	struct comedi_chaninfo it;

<<<<<<< HEAD
	if (copy_from_user(&it, arg, sizeof(struct comedi_chaninfo)))
=======
	if (copy_from_user(&it, arg, sizeof(it)))
>>>>>>> refs/remotes/origin/master
		return -EFAULT;

	if (it.subdev >= dev->n_subdevices)
		return -EINVAL;
<<<<<<< HEAD
	s = dev->subdevices + it.subdev;
=======
	s = &dev->subdevices[it.subdev];
>>>>>>> refs/remotes/origin/master

	if (it.maxdata_list) {
		if (s->maxdata || !s->maxdata_list)
			return -EINVAL;
		if (copy_to_user(it.maxdata_list, s->maxdata_list,
				 s->n_chan * sizeof(unsigned int)))
			return -EFAULT;
	}

<<<<<<< HEAD
	if (it.flaglist) {
		if (!s->flaglist)
			return -EINVAL;
		if (copy_to_user(it.flaglist, s->flaglist,
				 s->n_chan * sizeof(unsigned int)))
			return -EFAULT;
	}
=======
	if (it.flaglist)
		return -EINVAL;	/* flaglist not supported */
>>>>>>> refs/remotes/origin/master

	if (it.rangelist) {
		int i;

		if (!s->range_table_list)
			return -EINVAL;
		for (i = 0; i < s->n_chan; i++) {
			int x;

			x = (dev->minor << 28) | (it.subdev << 24) | (i << 16) |
			    (s->range_table_list[i]->length);
			if (put_user(x, it.rangelist + i))
				return -EFAULT;
		}
#if 0
		if (copy_to_user(it.rangelist, s->range_type_list,
				 s->n_chan * sizeof(unsigned int)))
			return -EFAULT;
#endif
	}

	return 0;
}

 /*
    COMEDI_BUFINFO
    buffer information ioctl

    arg:
    pointer to bufinfo structure

    reads:
    bufinfo at arg

    writes:
    modified bufinfo at arg

  */
static int do_bufinfo_ioctl(struct comedi_device *dev,
			    struct comedi_bufinfo __user *arg, void *file)
{
	struct comedi_bufinfo bi;
	struct comedi_subdevice *s;
	struct comedi_async *async;

<<<<<<< HEAD
	if (copy_from_user(&bi, arg, sizeof(struct comedi_bufinfo)))
		return -EFAULT;

	if (bi.subdevice >= dev->n_subdevices || bi.subdevice < 0)
		return -EINVAL;

	s = dev->subdevices + bi.subdevice;
=======
	if (copy_from_user(&bi, arg, sizeof(bi)))
		return -EFAULT;

	if (bi.subdevice >= dev->n_subdevices)
		return -EINVAL;

	s = &dev->subdevices[bi.subdevice];
>>>>>>> refs/remotes/origin/master

	if (s->lock && s->lock != file)
		return -EACCES;

	async = s->async;

	if (!async) {
<<<<<<< HEAD
		DPRINTK("subdevice does not have async capability\n");
=======
		dev_dbg(dev->class_dev,
			"subdevice does not have async capability\n");
>>>>>>> refs/remotes/origin/master
		bi.buf_write_ptr = 0;
		bi.buf_read_ptr = 0;
		bi.buf_write_count = 0;
		bi.buf_read_count = 0;
		bi.bytes_read = 0;
		bi.bytes_written = 0;
		goto copyback;
	}
	if (!s->busy) {
		bi.bytes_read = 0;
		bi.bytes_written = 0;
		goto copyback_position;
	}
	if (s->busy != file)
		return -EACCES;

	if (bi.bytes_read && (s->subdev_flags & SDF_CMD_READ)) {
		bi.bytes_read = comedi_buf_read_alloc(async, bi.bytes_read);
		comedi_buf_read_free(async, bi.bytes_read);

<<<<<<< HEAD
		if (!(comedi_get_subdevice_runflags(s) & (SRF_ERROR |
							  SRF_RUNNING))
		    && async->buf_write_count == async->buf_read_count) {
=======
		if (comedi_is_subdevice_idle(s) &&
		    async->buf_write_count == async->buf_read_count) {
>>>>>>> refs/remotes/origin/master
			do_become_nonbusy(dev, s);
		}
	}

	if (bi.bytes_written && (s->subdev_flags & SDF_CMD_WRITE)) {
		bi.bytes_written =
		    comedi_buf_write_alloc(async, bi.bytes_written);
		comedi_buf_write_free(async, bi.bytes_written);
	}

copyback_position:
<<<<<<< HEAD
	bi.buf_write_count = async->buf_write_count;
	bi.buf_write_ptr = async->buf_write_ptr;
	bi.buf_read_count = async->buf_read_count;
	bi.buf_read_ptr = async->buf_read_ptr;

copyback:
	if (copy_to_user(arg, &bi, sizeof(struct comedi_bufinfo)))
		return -EFAULT;

	return 0;
}

static int parse_insn(struct comedi_device *dev, struct comedi_insn *insn,
		      unsigned int *data, void *file);
/*
 *	COMEDI_INSNLIST
 *	synchronous instructions
 *
 *	arg:
 *		pointer to sync cmd structure
 *
 *	reads:
 *		sync cmd struct at arg
 *		instruction list
 *		data (for writes)
 *
 *	writes:
 *		data (for reads)
 */
/* arbitrary limits */
#define MAX_SAMPLES 256
static int do_insnlist_ioctl(struct comedi_device *dev,
			     struct comedi_insnlist __user *arg, void *file)
{
	struct comedi_insnlist insnlist;
	struct comedi_insn *insns = NULL;
	unsigned int *data = NULL;
	int i = 0;
	int ret = 0;

	if (copy_from_user(&insnlist, arg, sizeof(struct comedi_insnlist)))
		return -EFAULT;

	data = kmalloc(sizeof(unsigned int) * MAX_SAMPLES, GFP_KERNEL);
	if (!data) {
		DPRINTK("kmalloc failed\n");
		ret = -ENOMEM;
		goto error;
	}

	insns =
<<<<<<< HEAD
	    kmalloc(sizeof(struct comedi_insn) * insnlist.n_insns, GFP_KERNEL);
=======
	    kcalloc(insnlist.n_insns, sizeof(struct comedi_insn), GFP_KERNEL);
>>>>>>> refs/remotes/origin/cm-10.0
	if (!insns) {
		DPRINTK("kmalloc failed\n");
		ret = -ENOMEM;
		goto error;
	}

	if (copy_from_user(insns, insnlist.insns,
			   sizeof(struct comedi_insn) * insnlist.n_insns)) {
		DPRINTK("copy_from_user failed\n");
		ret = -EFAULT;
		goto error;
	}

	for (i = 0; i < insnlist.n_insns; i++) {
		if (insns[i].n > MAX_SAMPLES) {
			DPRINTK("number of samples too large\n");
			ret = -EINVAL;
			goto error;
		}
		if (insns[i].insn & INSN_MASK_WRITE) {
			if (copy_from_user(data, insns[i].data,
					   insns[i].n * sizeof(unsigned int))) {
				DPRINTK("copy_from_user failed\n");
				ret = -EFAULT;
				goto error;
			}
		}
		ret = parse_insn(dev, insns + i, data, file);
		if (ret < 0)
			goto error;
		if (insns[i].insn & INSN_MASK_READ) {
			if (copy_to_user(insns[i].data, data,
					 insns[i].n * sizeof(unsigned int))) {
				DPRINTK("copy_to_user failed\n");
				ret = -EFAULT;
				goto error;
			}
		}
		if (need_resched())
			schedule();
	}

error:
	kfree(insns);
	kfree(data);

	if (ret < 0)
		return ret;
	return i;
=======
	bi.buf_write_count = async->buf_write_count;
	bi.buf_write_ptr = async->buf_write_ptr;
	bi.buf_read_count = async->buf_read_count;
	bi.buf_read_ptr = async->buf_read_ptr;

copyback:
	if (copy_to_user(arg, &bi, sizeof(bi)))
		return -EFAULT;

	return 0;
>>>>>>> refs/remotes/origin/master
}

static int check_insn_config_length(struct comedi_insn *insn,
				    unsigned int *data)
{
	if (insn->n < 1)
		return -EINVAL;

	switch (data[0]) {
	case INSN_CONFIG_DIO_OUTPUT:
	case INSN_CONFIG_DIO_INPUT:
	case INSN_CONFIG_DISARM:
	case INSN_CONFIG_RESET:
		if (insn->n == 1)
			return 0;
		break;
	case INSN_CONFIG_ARM:
	case INSN_CONFIG_DIO_QUERY:
	case INSN_CONFIG_BLOCK_SIZE:
	case INSN_CONFIG_FILTER:
	case INSN_CONFIG_SERIAL_CLOCK:
	case INSN_CONFIG_BIDIRECTIONAL_DATA:
	case INSN_CONFIG_ALT_SOURCE:
	case INSN_CONFIG_SET_COUNTER_MODE:
	case INSN_CONFIG_8254_READ_STATUS:
	case INSN_CONFIG_SET_ROUTING:
	case INSN_CONFIG_GET_ROUTING:
	case INSN_CONFIG_GET_PWM_STATUS:
	case INSN_CONFIG_PWM_SET_PERIOD:
	case INSN_CONFIG_PWM_GET_PERIOD:
		if (insn->n == 2)
			return 0;
		break;
	case INSN_CONFIG_SET_GATE_SRC:
	case INSN_CONFIG_GET_GATE_SRC:
	case INSN_CONFIG_SET_CLOCK_SRC:
	case INSN_CONFIG_GET_CLOCK_SRC:
	case INSN_CONFIG_SET_OTHER_SRC:
	case INSN_CONFIG_GET_COUNTER_STATUS:
	case INSN_CONFIG_PWM_SET_H_BRIDGE:
	case INSN_CONFIG_PWM_GET_H_BRIDGE:
	case INSN_CONFIG_GET_HARDWARE_BUFFER_SIZE:
		if (insn->n == 3)
			return 0;
		break;
	case INSN_CONFIG_PWM_OUTPUT:
	case INSN_CONFIG_ANALOG_TRIG:
		if (insn->n == 5)
			return 0;
		break;
<<<<<<< HEAD
		/* by default we allow the insn since we don't have checks for
		 * all possible cases yet */
	default:
		printk(KERN_WARNING
		       "comedi: no check for data length of config insn id "
		       "%i is implemented.\n"
		       " Add a check to %s in %s.\n"
		       " Assuming n=%i is correct.\n", data[0], __func__,
		       __FILE__, insn->n);
		return 0;
		break;
=======
	case INSN_CONFIG_DIGITAL_TRIG:
		if (insn->n == 6)
			return 0;
		break;
		/* by default we allow the insn since we don't have checks for
		 * all possible cases yet */
	default:
		pr_warn("comedi: No check for data length of config insn id %i is implemented.\n",
			data[0]);
		pr_warn("comedi: Add a check to %s in %s.\n",
			__func__, __FILE__);
		pr_warn("comedi: Assuming n=%i is correct.\n", insn->n);
		return 0;
>>>>>>> refs/remotes/origin/master
	}
	return -EINVAL;
}

static int parse_insn(struct comedi_device *dev, struct comedi_insn *insn,
		      unsigned int *data, void *file)
{
	struct comedi_subdevice *s;
	int ret = 0;
	int i;

	if (insn->insn & INSN_MASK_SPECIAL) {
		/* a non-subdevice instruction */

		switch (insn->insn) {
		case INSN_GTOD:
			{
				struct timeval tv;

				if (insn->n != 2) {
					ret = -EINVAL;
					break;
				}

				do_gettimeofday(&tv);
				data[0] = tv.tv_sec;
				data[1] = tv.tv_usec;
				ret = 2;

				break;
			}
		case INSN_WAIT:
			if (insn->n != 1 || data[0] >= 100000) {
				ret = -EINVAL;
				break;
			}
			udelay(data[0] / 1000);
			ret = 1;
			break;
		case INSN_INTTRIG:
			if (insn->n != 1) {
				ret = -EINVAL;
				break;
			}
			if (insn->subdev >= dev->n_subdevices) {
<<<<<<< HEAD
				DPRINTK("%d not usable subdevice\n",
=======
				dev_dbg(dev->class_dev,
					"%d not usable subdevice\n",
>>>>>>> refs/remotes/origin/master
					insn->subdev);
				ret = -EINVAL;
				break;
			}
<<<<<<< HEAD
			s = dev->subdevices + insn->subdev;
			if (!s->async) {
				DPRINTK("no async\n");
=======
			s = &dev->subdevices[insn->subdev];
			if (!s->async) {
				dev_dbg(dev->class_dev, "no async\n");
>>>>>>> refs/remotes/origin/master
				ret = -EINVAL;
				break;
			}
			if (!s->async->inttrig) {
<<<<<<< HEAD
				DPRINTK("no inttrig\n");
=======
				dev_dbg(dev->class_dev, "no inttrig\n");
>>>>>>> refs/remotes/origin/master
				ret = -EAGAIN;
				break;
			}
			ret = s->async->inttrig(dev, s, data[0]);
			if (ret >= 0)
				ret = 1;
			break;
		default:
<<<<<<< HEAD
			DPRINTK("invalid insn\n");
=======
			dev_dbg(dev->class_dev, "invalid insn\n");
>>>>>>> refs/remotes/origin/master
			ret = -EINVAL;
			break;
		}
	} else {
		/* a subdevice instruction */
		unsigned int maxdata;

		if (insn->subdev >= dev->n_subdevices) {
<<<<<<< HEAD
			DPRINTK("subdevice %d out of range\n", insn->subdev);
			ret = -EINVAL;
			goto out;
		}
		s = dev->subdevices + insn->subdev;

		if (s->type == COMEDI_SUBD_UNUSED) {
			DPRINTK("%d not usable subdevice\n", insn->subdev);
=======
			dev_dbg(dev->class_dev, "subdevice %d out of range\n",
				insn->subdev);
			ret = -EINVAL;
			goto out;
		}
		s = &dev->subdevices[insn->subdev];

		if (s->type == COMEDI_SUBD_UNUSED) {
			dev_dbg(dev->class_dev, "%d not usable subdevice\n",
				insn->subdev);
>>>>>>> refs/remotes/origin/master
			ret = -EIO;
			goto out;
		}

		/* are we locked? (ioctl lock) */
		if (s->lock && s->lock != file) {
<<<<<<< HEAD
			DPRINTK("device locked\n");
=======
			dev_dbg(dev->class_dev, "device locked\n");
>>>>>>> refs/remotes/origin/master
			ret = -EACCES;
			goto out;
		}

		ret = comedi_check_chanlist(s, 1, &insn->chanspec);
		if (ret < 0) {
			ret = -EINVAL;
<<<<<<< HEAD
			DPRINTK("bad chanspec\n");
=======
			dev_dbg(dev->class_dev, "bad chanspec\n");
>>>>>>> refs/remotes/origin/master
			goto out;
		}

		if (s->busy) {
			ret = -EBUSY;
			goto out;
		}
		/* This looks arbitrary.  It is. */
		s->busy = &parse_insn;
		switch (insn->insn) {
		case INSN_READ:
			ret = s->insn_read(dev, s, insn, data);
			break;
		case INSN_WRITE:
			maxdata = s->maxdata_list
			    ? s->maxdata_list[CR_CHAN(insn->chanspec)]
			    : s->maxdata;
			for (i = 0; i < insn->n; ++i) {
				if (data[i] > maxdata) {
					ret = -EINVAL;
<<<<<<< HEAD
					DPRINTK("bad data value(s)\n");
=======
					dev_dbg(dev->class_dev,
						"bad data value(s)\n");
>>>>>>> refs/remotes/origin/master
					break;
				}
			}
			if (ret == 0)
				ret = s->insn_write(dev, s, insn, data);
			break;
		case INSN_BITS:
			if (insn->n != 2) {
				ret = -EINVAL;
			} else {
				/* Most drivers ignore the base channel in
				 * insn->chanspec.  Fix this here if
				 * the subdevice has <= 32 channels.  */
				unsigned int shift;
				unsigned int orig_mask;

				orig_mask = data[0];
				if (s->n_chan <= 32) {
					shift = CR_CHAN(insn->chanspec);
					if (shift > 0) {
						insn->chanspec = 0;
						data[0] <<= shift;
						data[1] <<= shift;
					}
				} else
					shift = 0;
				ret = s->insn_bits(dev, s, insn, data);
				data[0] = orig_mask;
				if (shift > 0)
					data[1] >>= shift;
			}
			break;
		case INSN_CONFIG:
			ret = check_insn_config_length(insn, data);
			if (ret)
				break;
			ret = s->insn_config(dev, s, insn, data);
			break;
		default:
			ret = -EINVAL;
			break;
		}

		s->busy = NULL;
	}

out:
	return ret;
}

/*
<<<<<<< HEAD
=======
 *	COMEDI_INSNLIST
 *	synchronous instructions
 *
 *	arg:
 *		pointer to sync cmd structure
 *
 *	reads:
 *		sync cmd struct at arg
 *		instruction list
 *		data (for writes)
 *
 *	writes:
 *		data (for reads)
 */
/* arbitrary limits */
#define MAX_SAMPLES 256
static int do_insnlist_ioctl(struct comedi_device *dev,
			     struct comedi_insnlist __user *arg, void *file)
{
	struct comedi_insnlist insnlist;
	struct comedi_insn *insns = NULL;
	unsigned int *data = NULL;
	int i = 0;
	int ret = 0;

	if (copy_from_user(&insnlist, arg, sizeof(insnlist)))
		return -EFAULT;

	data = kmalloc(sizeof(unsigned int) * MAX_SAMPLES, GFP_KERNEL);
	if (!data) {
		ret = -ENOMEM;
		goto error;
	}

	insns = kcalloc(insnlist.n_insns, sizeof(*insns), GFP_KERNEL);
	if (!insns) {
		ret = -ENOMEM;
		goto error;
	}

	if (copy_from_user(insns, insnlist.insns,
			   sizeof(*insns) * insnlist.n_insns)) {
		dev_dbg(dev->class_dev, "copy_from_user failed\n");
		ret = -EFAULT;
		goto error;
	}

	for (i = 0; i < insnlist.n_insns; i++) {
		if (insns[i].n > MAX_SAMPLES) {
			dev_dbg(dev->class_dev,
				"number of samples too large\n");
			ret = -EINVAL;
			goto error;
		}
		if (insns[i].insn & INSN_MASK_WRITE) {
			if (copy_from_user(data, insns[i].data,
					   insns[i].n * sizeof(unsigned int))) {
				dev_dbg(dev->class_dev,
					"copy_from_user failed\n");
				ret = -EFAULT;
				goto error;
			}
		}
		ret = parse_insn(dev, insns + i, data, file);
		if (ret < 0)
			goto error;
		if (insns[i].insn & INSN_MASK_READ) {
			if (copy_to_user(insns[i].data, data,
					 insns[i].n * sizeof(unsigned int))) {
				dev_dbg(dev->class_dev,
					"copy_to_user failed\n");
				ret = -EFAULT;
				goto error;
			}
		}
		if (need_resched())
			schedule();
	}

error:
	kfree(insns);
	kfree(data);

	if (ret < 0)
		return ret;
	return i;
}

/*
>>>>>>> refs/remotes/origin/master
 *	COMEDI_INSN
 *	synchronous instructions
 *
 *	arg:
 *		pointer to insn
 *
 *	reads:
 *		struct comedi_insn struct at arg
 *		data (for writes)
 *
 *	writes:
 *		data (for reads)
 */
static int do_insn_ioctl(struct comedi_device *dev,
			 struct comedi_insn __user *arg, void *file)
{
	struct comedi_insn insn;
	unsigned int *data = NULL;
	int ret = 0;

	data = kmalloc(sizeof(unsigned int) * MAX_SAMPLES, GFP_KERNEL);
	if (!data) {
		ret = -ENOMEM;
		goto error;
	}

<<<<<<< HEAD
	if (copy_from_user(&insn, arg, sizeof(struct comedi_insn))) {
=======
	if (copy_from_user(&insn, arg, sizeof(insn))) {
>>>>>>> refs/remotes/origin/master
		ret = -EFAULT;
		goto error;
	}

	/* This is where the behavior of insn and insnlist deviate. */
	if (insn.n > MAX_SAMPLES)
		insn.n = MAX_SAMPLES;
	if (insn.insn & INSN_MASK_WRITE) {
		if (copy_from_user(data,
				   insn.data,
				   insn.n * sizeof(unsigned int))) {
			ret = -EFAULT;
			goto error;
		}
	}
	ret = parse_insn(dev, &insn, data, file);
	if (ret < 0)
		goto error;
	if (insn.insn & INSN_MASK_READ) {
		if (copy_to_user(insn.data,
				 data,
				 insn.n * sizeof(unsigned int))) {
			ret = -EFAULT;
			goto error;
		}
	}
	ret = insn.n;

error:
	kfree(data);

	return ret;
}

<<<<<<< HEAD
static void comedi_set_subdevice_runflags(struct comedi_subdevice *s,
					  unsigned mask, unsigned bits)
{
	unsigned long flags;

	spin_lock_irqsave(&s->spin_lock, flags);
	s->runflags &= ~mask;
	s->runflags |= (bits & mask);
	spin_unlock_irqrestore(&s->spin_lock, flags);
}

static int do_cmd_ioctl(struct comedi_device *dev,
			struct comedi_cmd __user *cmd, void *file)
{
	struct comedi_cmd user_cmd;
	struct comedi_subdevice *s;
	struct comedi_async *async;
	int ret = 0;
	unsigned int __user *chanlist_saver = NULL;

	if (copy_from_user(&user_cmd, cmd, sizeof(struct comedi_cmd))) {
		DPRINTK("bad cmd address\n");
		return -EFAULT;
	}
	/* save user's chanlist pointer so it can be restored later */
	chanlist_saver = user_cmd.chanlist;

	if (user_cmd.subdev >= dev->n_subdevices) {
		DPRINTK("%d no such subdevice\n", user_cmd.subdev);
		return -ENODEV;
	}

	s = dev->subdevices + user_cmd.subdev;
	async = s->async;

	if (s->type == COMEDI_SUBD_UNUSED) {
		DPRINTK("%d not valid subdevice\n", user_cmd.subdev);
=======
static int do_cmd_ioctl(struct comedi_device *dev,
			struct comedi_cmd __user *arg, void *file)
{
	struct comedi_cmd cmd;
	struct comedi_subdevice *s;
	struct comedi_async *async;
	int ret = 0;
	unsigned int __user *user_chanlist;

	if (copy_from_user(&cmd, arg, sizeof(cmd))) {
		dev_dbg(dev->class_dev, "bad cmd address\n");
		return -EFAULT;
	}
	/* save user's chanlist pointer so it can be restored later */
	user_chanlist = (unsigned int __user *)cmd.chanlist;

	if (cmd.subdev >= dev->n_subdevices) {
		dev_dbg(dev->class_dev, "%d no such subdevice\n", cmd.subdev);
		return -ENODEV;
	}

	s = &dev->subdevices[cmd.subdev];
	async = s->async;

	if (s->type == COMEDI_SUBD_UNUSED) {
		dev_dbg(dev->class_dev, "%d not valid subdevice\n", cmd.subdev);
>>>>>>> refs/remotes/origin/master
		return -EIO;
	}

	if (!s->do_cmd || !s->do_cmdtest || !s->async) {
<<<<<<< HEAD
		DPRINTK("subdevice %i does not support commands\n",
			user_cmd.subdev);
=======
		dev_dbg(dev->class_dev,
			"subdevice %i does not support commands\n", cmd.subdev);
>>>>>>> refs/remotes/origin/master
		return -EIO;
	}

	/* are we locked? (ioctl lock) */
	if (s->lock && s->lock != file) {
<<<<<<< HEAD
		DPRINTK("subdevice locked\n");
=======
		dev_dbg(dev->class_dev, "subdevice locked\n");
>>>>>>> refs/remotes/origin/master
		return -EACCES;
	}

	/* are we busy? */
	if (s->busy) {
<<<<<<< HEAD
		DPRINTK("subdevice busy\n");
		return -EBUSY;
	}
	s->busy = file;

	/* make sure channel/gain list isn't too long */
	if (user_cmd.chanlist_len > s->len_chanlist) {
		DPRINTK("channel/gain list too long %u > %d\n",
			user_cmd.chanlist_len, s->len_chanlist);
		ret = -EINVAL;
		goto cleanup;
	}

	/* make sure channel/gain list isn't too short */
	if (user_cmd.chanlist_len < 1) {
		DPRINTK("channel/gain list too short %u < 1\n",
			user_cmd.chanlist_len);
		ret = -EINVAL;
		goto cleanup;
	}

	async->cmd = user_cmd;
	async->cmd.data = NULL;
	/* load channel/gain list */
	async->cmd.chanlist =
	    kmalloc(async->cmd.chanlist_len * sizeof(int), GFP_KERNEL);
	if (!async->cmd.chanlist) {
		DPRINTK("allocation failed\n");
		ret = -ENOMEM;
		goto cleanup;
	}

	if (copy_from_user(async->cmd.chanlist, user_cmd.chanlist,
			   async->cmd.chanlist_len * sizeof(int))) {
		DPRINTK("fault reading chanlist\n");
		ret = -EFAULT;
=======
		dev_dbg(dev->class_dev, "subdevice busy\n");
		return -EBUSY;
	}

	/* make sure channel/gain list isn't too long */
	if (cmd.chanlist_len > s->len_chanlist) {
		dev_dbg(dev->class_dev, "channel/gain list too long %u > %d\n",
			cmd.chanlist_len, s->len_chanlist);
		return -EINVAL;
	}

	/* make sure channel/gain list isn't too short */
	if (cmd.chanlist_len < 1) {
		dev_dbg(dev->class_dev, "channel/gain list too short %u < 1\n",
			cmd.chanlist_len);
		return -EINVAL;
	}

	async->cmd = cmd;
	async->cmd.data = NULL;
	/* load channel/gain list */
	async->cmd.chanlist = memdup_user(user_chanlist,
					  async->cmd.chanlist_len * sizeof(int));
	if (IS_ERR(async->cmd.chanlist)) {
		ret = PTR_ERR(async->cmd.chanlist);
		async->cmd.chanlist = NULL;
		dev_dbg(dev->class_dev, "memdup_user failed with code %d\n",
			ret);
>>>>>>> refs/remotes/origin/master
		goto cleanup;
	}

	/* make sure each element in channel/gain list is valid */
	ret = comedi_check_chanlist(s,
				    async->cmd.chanlist_len,
				    async->cmd.chanlist);
	if (ret < 0) {
<<<<<<< HEAD
		DPRINTK("bad chanlist\n");
=======
		dev_dbg(dev->class_dev, "bad chanlist\n");
>>>>>>> refs/remotes/origin/master
		goto cleanup;
	}

	ret = s->do_cmdtest(dev, s, &async->cmd);

	if (async->cmd.flags & TRIG_BOGUS || ret) {
<<<<<<< HEAD
		DPRINTK("test returned %d\n", ret);
		user_cmd = async->cmd;
		/* restore chanlist pointer before copying back */
		user_cmd.chanlist = chanlist_saver;
		user_cmd.data = NULL;
		if (copy_to_user(cmd, &user_cmd, sizeof(struct comedi_cmd))) {
			DPRINTK("fault writing cmd\n");
=======
		dev_dbg(dev->class_dev, "test returned %d\n", ret);
		cmd = async->cmd;
		/* restore chanlist pointer before copying back */
		cmd.chanlist = (unsigned int __force *)user_chanlist;
		cmd.data = NULL;
		if (copy_to_user(arg, &cmd, sizeof(cmd))) {
			dev_dbg(dev->class_dev, "fault writing cmd\n");
>>>>>>> refs/remotes/origin/master
			ret = -EFAULT;
			goto cleanup;
		}
		ret = -EAGAIN;
		goto cleanup;
	}

	if (!async->prealloc_bufsz) {
		ret = -ENOMEM;
<<<<<<< HEAD
		DPRINTK("no buffer (?)\n");
		goto cleanup;
	}

	comedi_reset_async_buf(async);
=======
		dev_dbg(dev->class_dev, "no buffer (?)\n");
		goto cleanup;
	}

	comedi_buf_reset(async);
>>>>>>> refs/remotes/origin/master

	async->cb_mask =
	    COMEDI_CB_EOA | COMEDI_CB_BLOCK | COMEDI_CB_ERROR |
	    COMEDI_CB_OVERFLOW;
	if (async->cmd.flags & TRIG_WAKE_EOS)
		async->cb_mask |= COMEDI_CB_EOS;

<<<<<<< HEAD
	comedi_set_subdevice_runflags(s, ~0, SRF_USER | SRF_RUNNING);

=======
	comedi_set_subdevice_runflags(s, SRF_ERROR | SRF_RUNNING, SRF_RUNNING);

	/* set s->busy _after_ setting SRF_RUNNING flag to avoid race with
	 * comedi_read() or comedi_write() */
	s->busy = file;
>>>>>>> refs/remotes/origin/master
	ret = s->do_cmd(dev, s);
	if (ret == 0)
		return 0;

cleanup:
	do_become_nonbusy(dev, s);

	return ret;
}

/*
	COMEDI_CMDTEST
	command testing ioctl

	arg:
		pointer to cmd structure

	reads:
		cmd structure at arg
		channel/range list

	writes:
		modified cmd structure at arg

*/
static int do_cmdtest_ioctl(struct comedi_device *dev,
			    struct comedi_cmd __user *arg, void *file)
{
<<<<<<< HEAD
	struct comedi_cmd user_cmd;
	struct comedi_subdevice *s;
	int ret = 0;
	unsigned int *chanlist = NULL;
	unsigned int __user *chanlist_saver = NULL;

	if (copy_from_user(&user_cmd, arg, sizeof(struct comedi_cmd))) {
		DPRINTK("bad cmd address\n");
		return -EFAULT;
	}
	/* save user's chanlist pointer so it can be restored later */
	chanlist_saver = user_cmd.chanlist;

	if (user_cmd.subdev >= dev->n_subdevices) {
		DPRINTK("%d no such subdevice\n", user_cmd.subdev);
		return -ENODEV;
	}

	s = dev->subdevices + user_cmd.subdev;
	if (s->type == COMEDI_SUBD_UNUSED) {
		DPRINTK("%d not valid subdevice\n", user_cmd.subdev);
=======
	struct comedi_cmd cmd;
	struct comedi_subdevice *s;
	int ret = 0;
	unsigned int *chanlist = NULL;
	unsigned int __user *user_chanlist;

	if (copy_from_user(&cmd, arg, sizeof(cmd))) {
		dev_dbg(dev->class_dev, "bad cmd address\n");
		return -EFAULT;
	}
	/* save user's chanlist pointer so it can be restored later */
	user_chanlist = (unsigned int __user *)cmd.chanlist;

	if (cmd.subdev >= dev->n_subdevices) {
		dev_dbg(dev->class_dev, "%d no such subdevice\n", cmd.subdev);
		return -ENODEV;
	}

	s = &dev->subdevices[cmd.subdev];
	if (s->type == COMEDI_SUBD_UNUSED) {
		dev_dbg(dev->class_dev, "%d not valid subdevice\n", cmd.subdev);
>>>>>>> refs/remotes/origin/master
		return -EIO;
	}

	if (!s->do_cmd || !s->do_cmdtest) {
<<<<<<< HEAD
		DPRINTK("subdevice %i does not support commands\n",
			user_cmd.subdev);
=======
		dev_dbg(dev->class_dev,
			"subdevice %i does not support commands\n", cmd.subdev);
>>>>>>> refs/remotes/origin/master
		return -EIO;
	}

	/* make sure channel/gain list isn't too long */
<<<<<<< HEAD
	if (user_cmd.chanlist_len > s->len_chanlist) {
		DPRINTK("channel/gain list too long %d > %d\n",
			user_cmd.chanlist_len, s->len_chanlist);
=======
	if (cmd.chanlist_len > s->len_chanlist) {
		dev_dbg(dev->class_dev, "channel/gain list too long %d > %d\n",
			cmd.chanlist_len, s->len_chanlist);
>>>>>>> refs/remotes/origin/master
		ret = -EINVAL;
		goto cleanup;
	}

	/* load channel/gain list */
<<<<<<< HEAD
	if (user_cmd.chanlist) {
		chanlist =
		    kmalloc(user_cmd.chanlist_len * sizeof(int), GFP_KERNEL);
		if (!chanlist) {
			DPRINTK("allocation failed\n");
			ret = -ENOMEM;
			goto cleanup;
		}

		if (copy_from_user(chanlist, user_cmd.chanlist,
				   user_cmd.chanlist_len * sizeof(int))) {
			DPRINTK("fault reading chanlist\n");
			ret = -EFAULT;
=======
	if (cmd.chanlist) {
		chanlist = memdup_user(user_chanlist,
				       cmd.chanlist_len * sizeof(int));
		if (IS_ERR(chanlist)) {
			ret = PTR_ERR(chanlist);
			chanlist = NULL;
			dev_dbg(dev->class_dev,
				"memdup_user exited with code %d", ret);
>>>>>>> refs/remotes/origin/master
			goto cleanup;
		}

		/* make sure each element in channel/gain list is valid */
<<<<<<< HEAD
		ret = comedi_check_chanlist(s, user_cmd.chanlist_len, chanlist);
		if (ret < 0) {
			DPRINTK("bad chanlist\n");
			goto cleanup;
		}

		user_cmd.chanlist = chanlist;
	}

	ret = s->do_cmdtest(dev, s, &user_cmd);

	/* restore chanlist pointer before copying back */
	user_cmd.chanlist = chanlist_saver;

	if (copy_to_user(arg, &user_cmd, sizeof(struct comedi_cmd))) {
		DPRINTK("bad cmd address\n");
=======
		ret = comedi_check_chanlist(s, cmd.chanlist_len, chanlist);
		if (ret < 0) {
			dev_dbg(dev->class_dev, "bad chanlist\n");
			goto cleanup;
		}

		cmd.chanlist = chanlist;
	}

	ret = s->do_cmdtest(dev, s, &cmd);

	/* restore chanlist pointer before copying back */
	cmd.chanlist = (unsigned int __force *)user_chanlist;

	if (copy_to_user(arg, &cmd, sizeof(cmd))) {
		dev_dbg(dev->class_dev, "bad cmd address\n");
>>>>>>> refs/remotes/origin/master
		ret = -EFAULT;
		goto cleanup;
	}
cleanup:
	kfree(chanlist);

	return ret;
}

/*
	COMEDI_LOCK
	lock subdevice

	arg:
		subdevice number

	reads:
		none

	writes:
		none

*/

static int do_lock_ioctl(struct comedi_device *dev, unsigned int arg,
			 void *file)
{
	int ret = 0;
	unsigned long flags;
	struct comedi_subdevice *s;

	if (arg >= dev->n_subdevices)
		return -EINVAL;
<<<<<<< HEAD
	s = dev->subdevices + arg;
=======
	s = &dev->subdevices[arg];
>>>>>>> refs/remotes/origin/master

	spin_lock_irqsave(&s->spin_lock, flags);
	if (s->busy || s->lock)
		ret = -EBUSY;
	else
		s->lock = file;
	spin_unlock_irqrestore(&s->spin_lock, flags);

<<<<<<< HEAD
<<<<<<< HEAD
	if (ret < 0)
		return ret;

#if 0
=======
=======
>>>>>>> refs/remotes/origin/master
#if 0
	if (ret < 0)
		return ret;

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	if (s->lock_f)
		ret = s->lock_f(dev, s);
#endif

	return ret;
}

/*
	COMEDI_UNLOCK
	unlock subdevice

	arg:
		subdevice number

	reads:
		none

	writes:
		none

	This function isn't protected by the semaphore, since
	we already own the lock.
*/
static int do_unlock_ioctl(struct comedi_device *dev, unsigned int arg,
			   void *file)
{
	struct comedi_subdevice *s;

	if (arg >= dev->n_subdevices)
		return -EINVAL;
<<<<<<< HEAD
	s = dev->subdevices + arg;
=======
	s = &dev->subdevices[arg];
>>>>>>> refs/remotes/origin/master

	if (s->busy)
		return -EBUSY;

	if (s->lock && s->lock != file)
		return -EACCES;

	if (s->lock == file) {
#if 0
		if (s->unlock)
			s->unlock(dev, s);
#endif

		s->lock = NULL;
	}

	return 0;
}

/*
	COMEDI_CANCEL
	cancel acquisition ioctl

	arg:
		subdevice number

	reads:
		nothing

	writes:
		nothing

*/
static int do_cancel_ioctl(struct comedi_device *dev, unsigned int arg,
			   void *file)
{
	struct comedi_subdevice *s;
	int ret;

	if (arg >= dev->n_subdevices)
		return -EINVAL;
<<<<<<< HEAD
	s = dev->subdevices + arg;
=======
	s = &dev->subdevices[arg];
>>>>>>> refs/remotes/origin/master
	if (s->async == NULL)
		return -EINVAL;

	if (s->lock && s->lock != file)
		return -EACCES;

	if (!s->busy)
		return 0;

	if (s->busy != file)
		return -EBUSY;

	ret = do_cancel(dev, s);
<<<<<<< HEAD
<<<<<<< HEAD
	if (comedi_get_subdevice_runflags(s) & SRF_USER)
		wake_up_interruptible(&s->async->wait_head);
=======
>>>>>>> refs/remotes/origin/master
=======
	if (comedi_get_subdevice_runflags(s) & SRF_USER)
		wake_up_interruptible(&s->async->wait_head);
>>>>>>> refs/remotes/origin/cm-11.0

	return ret;
}

/*
	COMEDI_POLL ioctl
	instructs driver to synchronize buffers

	arg:
		subdevice number

	reads:
		nothing

	writes:
		nothing

*/
static int do_poll_ioctl(struct comedi_device *dev, unsigned int arg,
			 void *file)
{
	struct comedi_subdevice *s;

	if (arg >= dev->n_subdevices)
		return -EINVAL;
<<<<<<< HEAD
	s = dev->subdevices + arg;
=======
	s = &dev->subdevices[arg];
>>>>>>> refs/remotes/origin/master

	if (s->lock && s->lock != file)
		return -EACCES;

	if (!s->busy)
		return 0;

	if (s->busy != file)
		return -EBUSY;

	if (s->poll)
		return s->poll(dev, s);

	return -EINVAL;
}

<<<<<<< HEAD
static int do_cancel(struct comedi_device *dev, struct comedi_subdevice *s)
{
	int ret = 0;

	if ((comedi_get_subdevice_runflags(s) & SRF_RUNNING) && s->cancel)
		ret = s->cancel(dev, s);

	do_become_nonbusy(dev, s);

	return ret;
}


static void comedi_vm_open(struct vm_area_struct *area)
{
	struct comedi_async *async;
	struct comedi_device *dev;

	async = area->vm_private_data;
	dev = async->subdevice->device;

	mutex_lock(&dev->mutex);
	async->mmap_count++;
	mutex_unlock(&dev->mutex);
=======
static long comedi_unlocked_ioctl(struct file *file, unsigned int cmd,
				  unsigned long arg)
{
	const unsigned minor = iminor(file_inode(file));
	struct comedi_device *dev = file->private_data;
	int rc;

	mutex_lock(&dev->mutex);

	/* Device config is special, because it must work on
	 * an unconfigured device. */
	if (cmd == COMEDI_DEVCONFIG) {
		if (minor >= COMEDI_NUM_BOARD_MINORS) {
			/* Device config not appropriate on non-board minors. */
			rc = -ENOTTY;
			goto done;
		}
		rc = do_devconfig_ioctl(dev,
					(struct comedi_devconfig __user *)arg);
		if (rc == 0) {
			if (arg == 0 &&
			    dev->minor >= comedi_num_legacy_minors) {
				/* Successfully unconfigured a dynamically
				 * allocated device.  Try and remove it. */
				if (comedi_clear_board_dev(dev)) {
					mutex_unlock(&dev->mutex);
					comedi_free_board_dev(dev);
					return rc;
				}
			}
		}
		goto done;
	}

	if (!dev->attached) {
		dev_dbg(dev->class_dev, "no driver attached\n");
		rc = -ENODEV;
		goto done;
	}

	switch (cmd) {
	case COMEDI_BUFCONFIG:
		rc = do_bufconfig_ioctl(dev,
					(struct comedi_bufconfig __user *)arg);
		break;
	case COMEDI_DEVINFO:
		rc = do_devinfo_ioctl(dev, (struct comedi_devinfo __user *)arg,
				      file);
		break;
	case COMEDI_SUBDINFO:
		rc = do_subdinfo_ioctl(dev,
				       (struct comedi_subdinfo __user *)arg,
				       file);
		break;
	case COMEDI_CHANINFO:
		rc = do_chaninfo_ioctl(dev, (void __user *)arg);
		break;
	case COMEDI_RANGEINFO:
		rc = do_rangeinfo_ioctl(dev, (void __user *)arg);
		break;
	case COMEDI_BUFINFO:
		rc = do_bufinfo_ioctl(dev,
				      (struct comedi_bufinfo __user *)arg,
				      file);
		break;
	case COMEDI_LOCK:
		rc = do_lock_ioctl(dev, arg, file);
		break;
	case COMEDI_UNLOCK:
		rc = do_unlock_ioctl(dev, arg, file);
		break;
	case COMEDI_CANCEL:
		rc = do_cancel_ioctl(dev, arg, file);
		break;
	case COMEDI_CMD:
		rc = do_cmd_ioctl(dev, (struct comedi_cmd __user *)arg, file);
		break;
	case COMEDI_CMDTEST:
		rc = do_cmdtest_ioctl(dev, (struct comedi_cmd __user *)arg,
				      file);
		break;
	case COMEDI_INSNLIST:
		rc = do_insnlist_ioctl(dev,
				       (struct comedi_insnlist __user *)arg,
				       file);
		break;
	case COMEDI_INSN:
		rc = do_insn_ioctl(dev, (struct comedi_insn __user *)arg,
				   file);
		break;
	case COMEDI_POLL:
		rc = do_poll_ioctl(dev, arg, file);
		break;
	default:
		rc = -ENOTTY;
		break;
	}

done:
	mutex_unlock(&dev->mutex);
	return rc;
}

static void comedi_vm_open(struct vm_area_struct *area)
{
	struct comedi_buf_map *bm;

	bm = area->vm_private_data;
	comedi_buf_map_get(bm);
>>>>>>> refs/remotes/origin/master
}

static void comedi_vm_close(struct vm_area_struct *area)
{
<<<<<<< HEAD
	struct comedi_async *async;
	struct comedi_device *dev;

	async = area->vm_private_data;
	dev = async->subdevice->device;

	mutex_lock(&dev->mutex);
	async->mmap_count--;
	mutex_unlock(&dev->mutex);
=======
	struct comedi_buf_map *bm;

	bm = area->vm_private_data;
	comedi_buf_map_put(bm);
>>>>>>> refs/remotes/origin/master
}

static struct vm_operations_struct comedi_vm_ops = {
	.open = comedi_vm_open,
	.close = comedi_vm_close,
};

static int comedi_mmap(struct file *file, struct vm_area_struct *vma)
{
<<<<<<< HEAD
	const unsigned minor = iminor(file->f_dentry->d_inode);
	struct comedi_async *async = NULL;
=======
	const unsigned minor = iminor(file_inode(file));
	struct comedi_device *dev = file->private_data;
	struct comedi_subdevice *s;
	struct comedi_async *async;
	struct comedi_buf_map *bm;
>>>>>>> refs/remotes/origin/master
	unsigned long start = vma->vm_start;
	unsigned long size;
	int n_pages;
	int i;
	int retval;
<<<<<<< HEAD
	struct comedi_subdevice *s;
	struct comedi_device_file_info *dev_file_info;
	struct comedi_device *dev;

	dev_file_info = comedi_get_device_file_info(minor);
	if (dev_file_info == NULL)
<<<<<<< HEAD
	        return -ENODEV;
	dev = dev_file_info->device;
	if (dev == NULL)
	        return -ENODEV;
=======
		return -ENODEV;
	dev = dev_file_info->device;
	if (dev == NULL)
		return -ENODEV;
>>>>>>> refs/remotes/origin/cm-10.0

	mutex_lock(&dev->mutex);
	if (!dev->attached) {
		DPRINTK("no driver configured on comedi%i\n", dev->minor);
		retval = -ENODEV;
		goto done;
	}
	if (vma->vm_flags & VM_WRITE)
		s = comedi_get_write_subdevice(dev_file_info);
	else
		s = comedi_get_read_subdevice(dev_file_info);

	if (s == NULL) {
		retval = -EINVAL;
		goto done;
	}
	async = s->async;
	if (async == NULL) {
=======

	mutex_lock(&dev->mutex);

	if (!dev->attached) {
		dev_dbg(dev->class_dev, "no driver attached\n");
		retval = -ENODEV;
		goto done;
	}

	if (vma->vm_flags & VM_WRITE)
		s = comedi_write_subdevice(dev, minor);
	else
		s = comedi_read_subdevice(dev, minor);
	if (!s) {
		retval = -EINVAL;
		goto done;
	}

	async = s->async;
	if (!async) {
>>>>>>> refs/remotes/origin/master
		retval = -EINVAL;
		goto done;
	}

	if (vma->vm_pgoff != 0) {
<<<<<<< HEAD
		DPRINTK("comedi: mmap() offset must be 0.\n");
=======
		dev_dbg(dev->class_dev, "mmap() offset must be 0.\n");
>>>>>>> refs/remotes/origin/master
		retval = -EINVAL;
		goto done;
	}

	size = vma->vm_end - vma->vm_start;
	if (size > async->prealloc_bufsz) {
		retval = -EFAULT;
		goto done;
	}
	if (size & (~PAGE_MASK)) {
		retval = -EFAULT;
		goto done;
	}

	n_pages = size >> PAGE_SHIFT;
<<<<<<< HEAD
	for (i = 0; i < n_pages; ++i) {
		if (remap_pfn_range(vma, start,
				    page_to_pfn(virt_to_page
						(async->buf_page_list
						 [i].virt_addr)), PAGE_SIZE,
				    PAGE_SHARED)) {
=======
	bm = async->buf_map;
	if (!bm || n_pages > bm->n_pages) {
		retval = -EINVAL;
		goto done;
	}
	for (i = 0; i < n_pages; ++i) {
		struct comedi_buf_page *buf = &bm->page_list[i];

		if (remap_pfn_range(vma, start,
				    page_to_pfn(virt_to_page(buf->virt_addr)),
				    PAGE_SIZE, PAGE_SHARED)) {
>>>>>>> refs/remotes/origin/master
			retval = -EAGAIN;
			goto done;
		}
		start += PAGE_SIZE;
	}

	vma->vm_ops = &comedi_vm_ops;
<<<<<<< HEAD
	vma->vm_private_data = async;

	async->mmap_count++;
=======
	vma->vm_private_data = bm;

	vma->vm_ops->open(vma);
>>>>>>> refs/remotes/origin/master

	retval = 0;
done:
	mutex_unlock(&dev->mutex);
	return retval;
}

<<<<<<< HEAD
static unsigned int comedi_poll(struct file *file, poll_table * wait)
{
	unsigned int mask = 0;
	const unsigned minor = iminor(file->f_dentry->d_inode);
	struct comedi_subdevice *read_subdev;
	struct comedi_subdevice *write_subdev;
	struct comedi_device_file_info *dev_file_info;
	struct comedi_device *dev;
	dev_file_info = comedi_get_device_file_info(minor);

	if (dev_file_info == NULL)
<<<<<<< HEAD
	        return -ENODEV;
	dev = dev_file_info->device;
	if (dev == NULL)
	        return -ENODEV;
=======
		return -ENODEV;
	dev = dev_file_info->device;
	if (dev == NULL)
		return -ENODEV;
>>>>>>> refs/remotes/origin/cm-10.0

	mutex_lock(&dev->mutex);
	if (!dev->attached) {
		DPRINTK("no driver configured on comedi%i\n", dev->minor);
		mutex_unlock(&dev->mutex);
		return 0;
	}

	mask = 0;
	read_subdev = comedi_get_read_subdevice(dev_file_info);
	if (read_subdev && read_subdev->async) {
		poll_wait(file, &read_subdev->async->wait_head, wait);
		if (!read_subdev->busy
		    || comedi_buf_read_n_available(read_subdev->async) > 0
		    || !(comedi_get_subdevice_runflags(read_subdev) &
			 SRF_RUNNING)) {
			mask |= POLLIN | POLLRDNORM;
		}
	}
	write_subdev = comedi_get_write_subdevice(dev_file_info);
	if (write_subdev && write_subdev->async) {
		poll_wait(file, &write_subdev->async->wait_head, wait);
		comedi_buf_write_alloc(write_subdev->async,
				       write_subdev->async->prealloc_bufsz);
		if (!write_subdev->busy
		    || !(comedi_get_subdevice_runflags(write_subdev) &
			 SRF_RUNNING)
		    || comedi_buf_write_n_allocated(write_subdev->async) >=
		    bytes_per_sample(write_subdev->async->subdevice)) {
			mask |= POLLOUT | POLLWRNORM;
		}
	}

=======
static unsigned int comedi_poll(struct file *file, poll_table *wait)
{
	unsigned int mask = 0;
	const unsigned minor = iminor(file_inode(file));
	struct comedi_device *dev = file->private_data;
	struct comedi_subdevice *s;

	mutex_lock(&dev->mutex);

	if (!dev->attached) {
		dev_dbg(dev->class_dev, "no driver attached\n");
		goto done;
	}

	s = comedi_read_subdevice(dev, minor);
	if (s && s->async) {
		poll_wait(file, &s->async->wait_head, wait);
		if (!s->busy || !comedi_is_subdevice_running(s) ||
		    comedi_buf_read_n_available(s->async) > 0)
			mask |= POLLIN | POLLRDNORM;
	}

	s = comedi_write_subdevice(dev, minor);
	if (s && s->async) {
		unsigned int bps = bytes_per_sample(s->async->subdevice);

		poll_wait(file, &s->async->wait_head, wait);
		comedi_buf_write_alloc(s->async, s->async->prealloc_bufsz);
		if (!s->busy || !comedi_is_subdevice_running(s) ||
		    comedi_buf_write_n_allocated(s->async) >= bps)
			mask |= POLLOUT | POLLWRNORM;
	}

done:
>>>>>>> refs/remotes/origin/master
	mutex_unlock(&dev->mutex);
	return mask;
}

static ssize_t comedi_write(struct file *file, const char __user *buf,
			    size_t nbytes, loff_t *offset)
{
	struct comedi_subdevice *s;
	struct comedi_async *async;
	int n, m, count = 0, retval = 0;
	DECLARE_WAITQUEUE(wait, current);
<<<<<<< HEAD
	const unsigned minor = iminor(file->f_dentry->d_inode);
	struct comedi_device_file_info *dev_file_info;
	struct comedi_device *dev;
	dev_file_info = comedi_get_device_file_info(minor);

	if (dev_file_info == NULL)
<<<<<<< HEAD
	        return -ENODEV;
	dev = dev_file_info->device;
	if (dev == NULL)
	        return -ENODEV;
=======
		return -ENODEV;
	dev = dev_file_info->device;
	if (dev == NULL)
		return -ENODEV;
>>>>>>> refs/remotes/origin/cm-10.0

	if (!dev->attached) {
		DPRINTK("no driver configured on comedi%i\n", dev->minor);
		retval = -ENODEV;
		goto done;
	}

	s = comedi_get_write_subdevice(dev_file_info);
	if (s == NULL || s->async == NULL) {
		retval = -EIO;
		goto done;
	}
	async = s->async;

	if (!nbytes) {
		retval = 0;
		goto done;
	}
	if (!s->busy) {
		retval = 0;
		goto done;
	}
	if (s->busy != file) {
		retval = -EACCES;
		goto done;
	}
	add_wait_queue(&async->wait_head, &wait);
	while (nbytes > 0 && !retval) {
		set_current_state(TASK_INTERRUPTIBLE);

		if (!(comedi_get_subdevice_runflags(s) & SRF_RUNNING)) {
			if (count == 0) {
				if (comedi_get_subdevice_runflags(s) &
					SRF_ERROR) {
					retval = -EPIPE;
				} else {
					retval = 0;
				}
				do_become_nonbusy(dev, s);
=======
	const unsigned minor = iminor(file_inode(file));
	struct comedi_device *dev = file->private_data;
	bool on_wait_queue = false;
	bool attach_locked;
	unsigned int old_detach_count;

	/* Protect against device detachment during operation. */
	down_read(&dev->attach_lock);
	attach_locked = true;
	old_detach_count = dev->detach_count;

	if (!dev->attached) {
		dev_dbg(dev->class_dev, "no driver attached\n");
		retval = -ENODEV;
		goto out;
	}

	s = comedi_write_subdevice(dev, minor);
	if (!s || !s->async) {
		retval = -EIO;
		goto out;
	}

	async = s->async;

	if (!s->busy || !nbytes)
		goto out;
	if (s->busy != file) {
		retval = -EACCES;
		goto out;
	}

	add_wait_queue(&async->wait_head, &wait);
	on_wait_queue = true;
	while (nbytes > 0 && !retval) {
		set_current_state(TASK_INTERRUPTIBLE);

		if (!comedi_is_subdevice_running(s)) {
			if (count == 0) {
				struct comedi_subdevice *new_s;

				if (comedi_is_subdevice_in_error(s))
					retval = -EPIPE;
				else
					retval = 0;
				/*
				 * To avoid deadlock, cannot acquire dev->mutex
				 * while dev->attach_lock is held.  Need to
				 * remove task from the async wait queue before
				 * releasing dev->attach_lock, as it might not
				 * be valid afterwards.
				 */
				remove_wait_queue(&async->wait_head, &wait);
				on_wait_queue = false;
				up_read(&dev->attach_lock);
				attach_locked = false;
				mutex_lock(&dev->mutex);
				/*
				 * Become non-busy unless things have changed
				 * behind our back.  Checking dev->detach_count
				 * is unchanged ought to be sufficient (unless
				 * there have been 2**32 detaches in the
				 * meantime!), but check the subdevice pointer
				 * as well just in case.
				 */
				new_s = comedi_write_subdevice(dev, minor);
				if (dev->attached &&
				    old_detach_count == dev->detach_count &&
				    s == new_s && new_s->async == async)
					do_become_nonbusy(dev, s);
				mutex_unlock(&dev->mutex);
>>>>>>> refs/remotes/origin/master
			}
			break;
		}

		n = nbytes;

		m = n;
		if (async->buf_write_ptr + m > async->prealloc_bufsz)
			m = async->prealloc_bufsz - async->buf_write_ptr;
		comedi_buf_write_alloc(async, async->prealloc_bufsz);
		if (m > comedi_buf_write_n_allocated(async))
			m = comedi_buf_write_n_allocated(async);
		if (m < n)
			n = m;

		if (n == 0) {
			if (file->f_flags & O_NONBLOCK) {
				retval = -EAGAIN;
				break;
			}
			schedule();
			if (signal_pending(current)) {
				retval = -ERESTARTSYS;
				break;
			}
			if (!s->busy)
				break;
			if (s->busy != file) {
				retval = -EACCES;
				break;
			}
			continue;
		}

		m = copy_from_user(async->prealloc_buf + async->buf_write_ptr,
				   buf, n);
		if (m) {
			n -= m;
			retval = -EFAULT;
		}
		comedi_buf_write_free(async, n);

		count += n;
		nbytes -= n;

		buf += n;
		break;		/* makes device work like a pipe */
	}
<<<<<<< HEAD
	set_current_state(TASK_RUNNING);
	remove_wait_queue(&async->wait_head, &wait);

done:
=======
out:
	if (on_wait_queue)
		remove_wait_queue(&async->wait_head, &wait);
	set_current_state(TASK_RUNNING);
	if (attach_locked)
		up_read(&dev->attach_lock);

>>>>>>> refs/remotes/origin/master
	return count ? count : retval;
}

static ssize_t comedi_read(struct file *file, char __user *buf, size_t nbytes,
				loff_t *offset)
{
	struct comedi_subdevice *s;
	struct comedi_async *async;
	int n, m, count = 0, retval = 0;
	DECLARE_WAITQUEUE(wait, current);
<<<<<<< HEAD
	const unsigned minor = iminor(file->f_dentry->d_inode);
	struct comedi_device_file_info *dev_file_info;
	struct comedi_device *dev;
	dev_file_info = comedi_get_device_file_info(minor);

	if (dev_file_info == NULL)
<<<<<<< HEAD
	        return -ENODEV;
	dev = dev_file_info->device;
	if (dev == NULL)
	        return -ENODEV;
=======
		return -ENODEV;
	dev = dev_file_info->device;
	if (dev == NULL)
		return -ENODEV;
>>>>>>> refs/remotes/origin/cm-10.0

	if (!dev->attached) {
		DPRINTK("no driver configured on comedi%i\n", dev->minor);
		retval = -ENODEV;
		goto done;
	}

	s = comedi_get_read_subdevice(dev_file_info);
	if (s == NULL || s->async == NULL) {
		retval = -EIO;
		goto done;
	}
	async = s->async;
	if (!nbytes) {
		retval = 0;
		goto done;
	}
	if (!s->busy) {
		retval = 0;
		goto done;
	}
	if (s->busy != file) {
		retval = -EACCES;
		goto done;
=======
	const unsigned minor = iminor(file_inode(file));
	struct comedi_device *dev = file->private_data;
	unsigned int old_detach_count;
	bool become_nonbusy = false;
	bool attach_locked;

	/* Protect against device detachment during operation. */
	down_read(&dev->attach_lock);
	attach_locked = true;
	old_detach_count = dev->detach_count;

	if (!dev->attached) {
		dev_dbg(dev->class_dev, "no driver attached\n");
		retval = -ENODEV;
		goto out;
	}

	s = comedi_read_subdevice(dev, minor);
	if (!s || !s->async) {
		retval = -EIO;
		goto out;
	}

	async = s->async;
	if (!s->busy || !nbytes)
		goto out;
	if (s->busy != file) {
		retval = -EACCES;
		goto out;
>>>>>>> refs/remotes/origin/master
	}

	add_wait_queue(&async->wait_head, &wait);
	while (nbytes > 0 && !retval) {
		set_current_state(TASK_INTERRUPTIBLE);

		n = nbytes;

		m = comedi_buf_read_n_available(async);
		/* printk("%d available\n",m); */
		if (async->buf_read_ptr + m > async->prealloc_bufsz)
			m = async->prealloc_bufsz - async->buf_read_ptr;
		/* printk("%d contiguous\n",m); */
		if (m < n)
			n = m;

		if (n == 0) {
<<<<<<< HEAD
			if (!(comedi_get_subdevice_runflags(s) & SRF_RUNNING)) {
				do_become_nonbusy(dev, s);
				if (comedi_get_subdevice_runflags(s) &
				    SRF_ERROR) {
					retval = -EPIPE;
				} else {
					retval = 0;
				}
=======
			if (!comedi_is_subdevice_running(s)) {
				if (comedi_is_subdevice_in_error(s))
					retval = -EPIPE;
				else
					retval = 0;
				become_nonbusy = true;
>>>>>>> refs/remotes/origin/master
				break;
			}
			if (file->f_flags & O_NONBLOCK) {
				retval = -EAGAIN;
				break;
			}
			schedule();
			if (signal_pending(current)) {
				retval = -ERESTARTSYS;
				break;
			}
			if (!s->busy) {
				retval = 0;
				break;
			}
			if (s->busy != file) {
				retval = -EACCES;
				break;
			}
			continue;
		}
		m = copy_to_user(buf, async->prealloc_buf +
				 async->buf_read_ptr, n);
		if (m) {
			n -= m;
			retval = -EFAULT;
		}

		comedi_buf_read_alloc(async, n);
		comedi_buf_read_free(async, n);

		count += n;
		nbytes -= n;

		buf += n;
		break;		/* makes device work like a pipe */
	}
<<<<<<< HEAD
	if (!(comedi_get_subdevice_runflags(s) & (SRF_ERROR | SRF_RUNNING)) &&
	    async->buf_read_count - async->buf_write_count == 0) {
		do_become_nonbusy(dev, s);
	}
	set_current_state(TASK_RUNNING);
	remove_wait_queue(&async->wait_head, &wait);

done:
	return count ? count : retval;
}

/*
   This function restores a subdevice to an idle state.
 */
void do_become_nonbusy(struct comedi_device *dev, struct comedi_subdevice *s)
{
	struct comedi_async *async = s->async;

	comedi_set_subdevice_runflags(s, SRF_RUNNING, 0);
	if (async) {
		comedi_reset_async_buf(async);
		async->inttrig = NULL;
		kfree(async->cmd.chanlist);
		async->cmd.chanlist = NULL;
	} else {
		printk(KERN_ERR
		       "BUG: (?) do_become_nonbusy called with async=0\n");
	}

	s->busy = NULL;
=======
	remove_wait_queue(&async->wait_head, &wait);
	set_current_state(TASK_RUNNING);
	if (become_nonbusy || comedi_is_subdevice_idle(s)) {
		struct comedi_subdevice *new_s;

		/*
		 * To avoid deadlock, cannot acquire dev->mutex
		 * while dev->attach_lock is held.
		 */
		up_read(&dev->attach_lock);
		attach_locked = false;
		mutex_lock(&dev->mutex);
		/*
		 * Check device hasn't become detached behind our back.
		 * Checking dev->detach_count is unchanged ought to be
		 * sufficient (unless there have been 2**32 detaches in the
		 * meantime!), but check the subdevice pointer as well just in
		 * case.
		 */
		new_s = comedi_read_subdevice(dev, minor);
		if (dev->attached && old_detach_count == dev->detach_count &&
		    s == new_s && new_s->async == async) {
			if (become_nonbusy ||
			    async->buf_read_count - async->buf_write_count == 0)
				do_become_nonbusy(dev, s);
		}
		mutex_unlock(&dev->mutex);
	}
out:
	if (attach_locked)
		up_read(&dev->attach_lock);

	return count ? count : retval;
>>>>>>> refs/remotes/origin/master
}

static int comedi_open(struct inode *inode, struct file *file)
{
	const unsigned minor = iminor(inode);
<<<<<<< HEAD
	struct comedi_device_file_info *dev_file_info =
	    comedi_get_device_file_info(minor);
	struct comedi_device *dev =
	    dev_file_info ? dev_file_info->device : NULL;

	if (dev == NULL) {
		DPRINTK("invalid minor number\n");
=======
	struct comedi_device *dev = comedi_dev_get_from_minor(minor);
	int rc;

	if (!dev) {
		pr_debug("invalid minor number\n");
>>>>>>> refs/remotes/origin/master
		return -ENODEV;
	}

	/* This is slightly hacky, but we want module autoloading
	 * to work for root.
	 * case: user opens device, attached -> ok
<<<<<<< HEAD
	 * case: user opens device, unattached, in_request_module=0 -> autoload
	 * case: user opens device, unattached, in_request_module=1 -> fail
	 * case: root opens device, attached -> ok
	 * case: root opens device, unattached, in_request_module=1 -> ok
	 *   (typically called from modprobe)
	 * case: root opens device, unattached, in_request_module=0 -> autoload
=======
	 * case: user opens device, unattached, !in_request_module -> autoload
	 * case: user opens device, unattached, in_request_module -> fail
	 * case: root opens device, attached -> ok
	 * case: root opens device, unattached, in_request_module -> ok
	 *   (typically called from modprobe)
	 * case: root opens device, unattached, !in_request_module -> autoload
>>>>>>> refs/remotes/origin/master
	 *
	 * The last could be changed to "-> ok", which would deny root
	 * autoloading.
	 */
	mutex_lock(&dev->mutex);
	if (dev->attached)
		goto ok;
	if (!capable(CAP_NET_ADMIN) && dev->in_request_module) {
<<<<<<< HEAD
		DPRINTK("in request module\n");
		mutex_unlock(&dev->mutex);
		return -ENODEV;
=======
		dev_dbg(dev->class_dev, "in request module\n");
		rc = -ENODEV;
		goto out;
>>>>>>> refs/remotes/origin/master
	}
	if (capable(CAP_NET_ADMIN) && dev->in_request_module)
		goto ok;

<<<<<<< HEAD
	dev->in_request_module = 1;
=======
	dev->in_request_module = true;
>>>>>>> refs/remotes/origin/master

#ifdef CONFIG_KMOD
	mutex_unlock(&dev->mutex);
	request_module("char-major-%i-%i", COMEDI_MAJOR, dev->minor);
	mutex_lock(&dev->mutex);
#endif

<<<<<<< HEAD
	dev->in_request_module = 0;

	if (!dev->attached && !capable(CAP_NET_ADMIN)) {
		DPRINTK("not attached and not CAP_NET_ADMIN\n");
		mutex_unlock(&dev->mutex);
		return -ENODEV;
	}
ok:
	__module_get(THIS_MODULE);

	if (dev->attached) {
		if (!try_module_get(dev->driver->module)) {
			module_put(THIS_MODULE);
			mutex_unlock(&dev->mutex);
			return -ENOSYS;
		}
	}

	if (dev->attached && dev->use_count == 0 && dev->open) {
		int rc = dev->open(dev);
		if (rc < 0) {
			module_put(dev->driver->module);
			module_put(THIS_MODULE);
			mutex_unlock(&dev->mutex);
			return rc;
=======
	dev->in_request_module = false;

	if (!dev->attached && !capable(CAP_NET_ADMIN)) {
		dev_dbg(dev->class_dev, "not attached and not CAP_NET_ADMIN\n");
		rc = -ENODEV;
		goto out;
	}
ok:
	if (dev->attached && dev->use_count == 0) {
		if (!try_module_get(dev->driver->module)) {
			rc = -ENOSYS;
			goto out;
		}
		if (dev->open) {
			rc = dev->open(dev);
			if (rc < 0) {
				module_put(dev->driver->module);
				goto out;
			}
>>>>>>> refs/remotes/origin/master
		}
	}

	dev->use_count++;
<<<<<<< HEAD

	mutex_unlock(&dev->mutex);

	return 0;
=======
	file->private_data = dev;
	rc = 0;

out:
	mutex_unlock(&dev->mutex);
	if (rc)
		comedi_dev_put(dev);
	return rc;
}

static int comedi_fasync(int fd, struct file *file, int on)
{
	struct comedi_device *dev = file->private_data;

	return fasync_helper(fd, file, on, &dev->async_queue);
>>>>>>> refs/remotes/origin/master
}

static int comedi_close(struct inode *inode, struct file *file)
{
<<<<<<< HEAD
	const unsigned minor = iminor(inode);
	struct comedi_subdevice *s = NULL;
	int i;
	struct comedi_device_file_info *dev_file_info;
	struct comedi_device *dev;
	dev_file_info = comedi_get_device_file_info(minor);

	if (dev_file_info == NULL)
<<<<<<< HEAD
	        return -ENODEV;
	dev = dev_file_info->device;
	if (dev == NULL)
	        return -ENODEV;
=======
		return -ENODEV;
	dev = dev_file_info->device;
	if (dev == NULL)
		return -ENODEV;
>>>>>>> refs/remotes/origin/cm-10.0
=======
	struct comedi_device *dev = file->private_data;
	struct comedi_subdevice *s = NULL;
	int i;
>>>>>>> refs/remotes/origin/master

	mutex_lock(&dev->mutex);

	if (dev->subdevices) {
		for (i = 0; i < dev->n_subdevices; i++) {
<<<<<<< HEAD
			s = dev->subdevices + i;
=======
			s = &dev->subdevices[i];
>>>>>>> refs/remotes/origin/master

			if (s->busy == file)
				do_cancel(dev, s);
			if (s->lock == file)
				s->lock = NULL;
		}
	}
<<<<<<< HEAD
	if (dev->attached && dev->use_count == 1 && dev->close)
		dev->close(dev);

	module_put(THIS_MODULE);
	if (dev->attached)
		module_put(dev->driver->module);
=======
	if (dev->attached && dev->use_count == 1) {
		if (dev->close)
			dev->close(dev);
		module_put(dev->driver->module);
	}
>>>>>>> refs/remotes/origin/master

	dev->use_count--;

	mutex_unlock(&dev->mutex);
<<<<<<< HEAD

	if (file->f_flags & FASYNC)
		comedi_fasync(-1, file, 0);
=======
	comedi_dev_put(dev);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
static int comedi_fasync(int fd, struct file *file, int on)
{
	const unsigned minor = iminor(file->f_dentry->d_inode);
	struct comedi_device_file_info *dev_file_info;
	struct comedi_device *dev;
	dev_file_info = comedi_get_device_file_info(minor);

	if (dev_file_info == NULL)
<<<<<<< HEAD
	        return -ENODEV;
	dev = dev_file_info->device;
	if (dev == NULL)
	        return -ENODEV;
=======
		return -ENODEV;
	dev = dev_file_info->device;
	if (dev == NULL)
		return -ENODEV;
>>>>>>> refs/remotes/origin/cm-10.0

	return fasync_helper(fd, file, on, &dev->async_queue);
}

const struct file_operations comedi_fops = {
=======
static const struct file_operations comedi_fops = {
>>>>>>> refs/remotes/origin/master
	.owner = THIS_MODULE,
	.unlocked_ioctl = comedi_unlocked_ioctl,
	.compat_ioctl = comedi_compat_ioctl,
	.open = comedi_open,
	.release = comedi_close,
	.read = comedi_read,
	.write = comedi_write,
	.mmap = comedi_mmap,
	.poll = comedi_poll,
	.fasync = comedi_fasync,
	.llseek = noop_llseek,
};

<<<<<<< HEAD
struct class *comedi_class;
static struct cdev comedi_cdev;

static void comedi_cleanup_legacy_minors(void)
{
	unsigned i;

	for (i = 0; i < comedi_num_legacy_minors; i++)
		comedi_free_board_minor(i);
}

static int __init comedi_init(void)
{
	int i;
	int retval;

	printk(KERN_INFO "comedi: version " COMEDI_RELEASE
	       " - http://www.comedi.org\n");

	if (comedi_num_legacy_minors < 0 ||
	    comedi_num_legacy_minors > COMEDI_NUM_BOARD_MINORS) {
		printk(KERN_ERR "comedi: error: invalid value for module "
		       "parameter \"comedi_num_legacy_minors\".  Valid values "
		       "are 0 through %i.\n", COMEDI_NUM_BOARD_MINORS);
		return -EINVAL;
	}

	/*
	 * comedi is unusable if both comedi_autoconfig and
	 * comedi_num_legacy_minors are zero, so we might as well adjust the
	 * defaults in that case
	 */
	if (comedi_autoconfig == 0 && comedi_num_legacy_minors == 0)
		comedi_num_legacy_minors = 16;

	memset(comedi_file_info_table, 0,
	       sizeof(struct comedi_device_file_info *) * COMEDI_NUM_MINORS);

	retval = register_chrdev_region(MKDEV(COMEDI_MAJOR, 0),
					COMEDI_NUM_MINORS, "comedi");
	if (retval)
		return -EIO;
	cdev_init(&comedi_cdev, &comedi_fops);
	comedi_cdev.owner = THIS_MODULE;
	kobject_set_name(&comedi_cdev.kobj, "comedi");
	if (cdev_add(&comedi_cdev, MKDEV(COMEDI_MAJOR, 0), COMEDI_NUM_MINORS)) {
		unregister_chrdev_region(MKDEV(COMEDI_MAJOR, 0),
					 COMEDI_NUM_MINORS);
		return -EIO;
	}
	comedi_class = class_create(THIS_MODULE, "comedi");
	if (IS_ERR(comedi_class)) {
		printk(KERN_ERR "comedi: failed to create class");
		cdev_del(&comedi_cdev);
		unregister_chrdev_region(MKDEV(COMEDI_MAJOR, 0),
					 COMEDI_NUM_MINORS);
		return PTR_ERR(comedi_class);
	}

	/* XXX requires /proc interface */
	comedi_proc_init();

	/* create devices files for legacy/manual use */
	for (i = 0; i < comedi_num_legacy_minors; i++) {
		int minor;
		minor = comedi_alloc_board_minor(NULL);
		if (minor < 0) {
			comedi_cleanup_legacy_minors();
			cdev_del(&comedi_cdev);
			unregister_chrdev_region(MKDEV(COMEDI_MAJOR, 0),
						 COMEDI_NUM_MINORS);
			return minor;
		}
	}

	return 0;
}

static void __exit comedi_cleanup(void)
{
	int i;

	comedi_cleanup_legacy_minors();
	for (i = 0; i < COMEDI_NUM_MINORS; ++i)
		BUG_ON(comedi_file_info_table[i]);

	class_destroy(comedi_class);
	cdev_del(&comedi_cdev);
	unregister_chrdev_region(MKDEV(COMEDI_MAJOR, 0), COMEDI_NUM_MINORS);

	comedi_proc_cleanup();
}

module_init(comedi_init);
module_exit(comedi_cleanup);

void comedi_error(const struct comedi_device *dev, const char *s)
{
	printk(KERN_ERR "comedi%d: %s: %s\n", dev->minor,
	       dev->driver->driver_name, s);
}
EXPORT_SYMBOL(comedi_error);

void comedi_event(struct comedi_device *dev, struct comedi_subdevice *s)
{
	struct comedi_async *async = s->async;
	unsigned runflags = 0;
	unsigned runflags_mask = 0;

	/* DPRINTK("comedi_event 0x%x\n",mask); */

	if ((comedi_get_subdevice_runflags(s) & SRF_RUNNING) == 0)
		return;

	if (s->
	    async->events & (COMEDI_CB_EOA | COMEDI_CB_ERROR |
			     COMEDI_CB_OVERFLOW)) {
		runflags_mask |= SRF_RUNNING;
	}
	/* remember if an error event has occurred, so an error
	 * can be returned the next time the user does a read() */
	if (s->async->events & (COMEDI_CB_ERROR | COMEDI_CB_OVERFLOW)) {
		runflags_mask |= SRF_ERROR;
		runflags |= SRF_ERROR;
	}
	if (runflags_mask) {
		/*sets SRF_ERROR and SRF_RUNNING together atomically */
		comedi_set_subdevice_runflags(s, runflags_mask, runflags);
	}

	if (async->cb_mask & s->async->events) {
		if (comedi_get_subdevice_runflags(s) & SRF_USER) {
			wake_up_interruptible(&async->wait_head);
			if (s->subdev_flags & SDF_CMD_READ)
				kill_fasync(&dev->async_queue, SIGIO, POLL_IN);
			if (s->subdev_flags & SDF_CMD_WRITE)
				kill_fasync(&dev->async_queue, SIGIO, POLL_OUT);
		} else {
			if (async->cb_func)
				async->cb_func(s->async->events, async->cb_arg);
		}
	}
	s->async->events = 0;
}
EXPORT_SYMBOL(comedi_event);

unsigned comedi_get_subdevice_runflags(struct comedi_subdevice *s)
{
	unsigned long flags;
	unsigned runflags;

	spin_lock_irqsave(&s->spin_lock, flags);
	runflags = s->runflags;
	spin_unlock_irqrestore(&s->spin_lock, flags);
	return runflags;
}
EXPORT_SYMBOL(comedi_get_subdevice_runflags);

static int is_device_busy(struct comedi_device *dev)
{
	struct comedi_subdevice *s;
	int i;

	if (!dev->attached)
		return 0;

	for (i = 0; i < dev->n_subdevices; i++) {
		s = dev->subdevices + i;
		if (s->busy)
			return 1;
		if (s->async && s->async->mmap_count)
			return 1;
	}

	return 0;
}

static void comedi_device_init(struct comedi_device *dev)
{
	memset(dev, 0, sizeof(struct comedi_device));
	spin_lock_init(&dev->spinlock);
	mutex_init(&dev->mutex);
	dev->minor = -1;
}

static void comedi_device_cleanup(struct comedi_device *dev)
{
	if (dev == NULL)
		return;
	mutex_lock(&dev->mutex);
	comedi_device_detach(dev);
	mutex_unlock(&dev->mutex);
	mutex_destroy(&dev->mutex);
}

int comedi_alloc_board_minor(struct device *hardware_device)
{
	unsigned long flags;
	struct comedi_device_file_info *info;
	struct device *csdev;
	unsigned i;
	int retval;

	info = kzalloc(sizeof(struct comedi_device_file_info), GFP_KERNEL);
	if (info == NULL)
		return -ENOMEM;
	info->device = kzalloc(sizeof(struct comedi_device), GFP_KERNEL);
	if (info->device == NULL) {
		kfree(info);
		return -ENOMEM;
	}
	info->hardware_device = hardware_device;
	comedi_device_init(info->device);
	spin_lock_irqsave(&comedi_file_info_table_lock, flags);
	for (i = 0; i < COMEDI_NUM_BOARD_MINORS; ++i) {
		if (comedi_file_info_table[i] == NULL) {
			comedi_file_info_table[i] = info;
			break;
		}
	}
	spin_unlock_irqrestore(&comedi_file_info_table_lock, flags);
	if (i == COMEDI_NUM_BOARD_MINORS) {
		comedi_device_cleanup(info->device);
		kfree(info->device);
		kfree(info);
		printk(KERN_ERR
		       "comedi: error: "
		       "ran out of minor numbers for board device files.\n");
		return -EBUSY;
	}
	info->device->minor = i;
<<<<<<< HEAD
	csdev = COMEDI_DEVICE_CREATE(comedi_class, NULL,
				     MKDEV(COMEDI_MAJOR, i), NULL,
				     hardware_device, "comedi%i", i);
=======
	csdev = device_create(comedi_class, hardware_device,
			      MKDEV(COMEDI_MAJOR, i), NULL, "comedi%i", i);
>>>>>>> refs/remotes/origin/cm-10.0
	if (!IS_ERR(csdev))
		info->device->class_dev = csdev;
	dev_set_drvdata(csdev, info);
	retval = device_create_file(csdev, &dev_attr_max_read_buffer_kb);
	if (retval) {
		printk(KERN_ERR
		       "comedi: "
		       "failed to create sysfs attribute file \"%s\".\n",
		       dev_attr_max_read_buffer_kb.attr.name);
		comedi_free_board_minor(i);
		return retval;
	}
	retval = device_create_file(csdev, &dev_attr_read_buffer_kb);
	if (retval) {
		printk(KERN_ERR
		       "comedi: "
		       "failed to create sysfs attribute file \"%s\".\n",
		       dev_attr_read_buffer_kb.attr.name);
		comedi_free_board_minor(i);
		return retval;
	}
	retval = device_create_file(csdev, &dev_attr_max_write_buffer_kb);
	if (retval) {
		printk(KERN_ERR
		       "comedi: "
		       "failed to create sysfs attribute file \"%s\".\n",
		       dev_attr_max_write_buffer_kb.attr.name);
		comedi_free_board_minor(i);
		return retval;
	}
	retval = device_create_file(csdev, &dev_attr_write_buffer_kb);
	if (retval) {
		printk(KERN_ERR
		       "comedi: "
		       "failed to create sysfs attribute file \"%s\".\n",
		       dev_attr_write_buffer_kb.attr.name);
		comedi_free_board_minor(i);
		return retval;
	}
	return i;
}

void comedi_free_board_minor(unsigned minor)
{
	unsigned long flags;
	struct comedi_device_file_info *info;

	BUG_ON(minor >= COMEDI_NUM_BOARD_MINORS);
	spin_lock_irqsave(&comedi_file_info_table_lock, flags);
	info = comedi_file_info_table[minor];
	comedi_file_info_table[minor] = NULL;
	spin_unlock_irqrestore(&comedi_file_info_table_lock, flags);

	if (info) {
		struct comedi_device *dev = info->device;
		if (dev) {
			if (dev->class_dev) {
				device_destroy(comedi_class,
					       MKDEV(COMEDI_MAJOR, dev->minor));
			}
			comedi_device_cleanup(dev);
			kfree(dev);
		}
		kfree(info);
	}
}

int comedi_find_board_minor(struct device *hardware_device)
{
	int minor;
	struct comedi_device_file_info *info;

	for (minor = 0; minor < COMEDI_NUM_BOARD_MINORS; minor++) {
		spin_lock(&comedi_file_info_table_lock);
		info = comedi_file_info_table[minor];
		if (info && info->hardware_device == hardware_device) {
			spin_unlock(&comedi_file_info_table_lock);
			return minor;
		}
		spin_unlock(&comedi_file_info_table_lock);
	}
	return -ENODEV;
}

int comedi_alloc_subdevice_minor(struct comedi_device *dev,
				 struct comedi_subdevice *s)
{
	unsigned long flags;
	struct comedi_device_file_info *info;
	struct device *csdev;
	unsigned i;
	int retval;

	info = kmalloc(sizeof(struct comedi_device_file_info), GFP_KERNEL);
	if (info == NULL)
		return -ENOMEM;
	info->device = dev;
	info->read_subdevice = s;
	info->write_subdevice = s;
	spin_lock_irqsave(&comedi_file_info_table_lock, flags);
	for (i = COMEDI_FIRST_SUBDEVICE_MINOR; i < COMEDI_NUM_MINORS; ++i) {
		if (comedi_file_info_table[i] == NULL) {
			comedi_file_info_table[i] = info;
			break;
		}
	}
	spin_unlock_irqrestore(&comedi_file_info_table_lock, flags);
	if (i == COMEDI_NUM_MINORS) {
		kfree(info);
		printk(KERN_ERR
		       "comedi: error: "
		       "ran out of minor numbers for board device files.\n");
		return -EBUSY;
	}
	s->minor = i;
<<<<<<< HEAD
	csdev = COMEDI_DEVICE_CREATE(comedi_class, dev->class_dev,
				     MKDEV(COMEDI_MAJOR, i), NULL, NULL,
				     "comedi%i_subd%i", dev->minor,
				     (int)(s - dev->subdevices));
=======
	csdev = device_create(comedi_class, dev->class_dev,
			      MKDEV(COMEDI_MAJOR, i), NULL, "comedi%i_subd%i",
			      dev->minor, (int)(s - dev->subdevices));
>>>>>>> refs/remotes/origin/cm-10.0
	if (!IS_ERR(csdev))
		s->class_dev = csdev;
	dev_set_drvdata(csdev, info);
	retval = device_create_file(csdev, &dev_attr_max_read_buffer_kb);
	if (retval) {
		printk(KERN_ERR
		       "comedi: "
		       "failed to create sysfs attribute file \"%s\".\n",
		       dev_attr_max_read_buffer_kb.attr.name);
		comedi_free_subdevice_minor(s);
		return retval;
	}
	retval = device_create_file(csdev, &dev_attr_read_buffer_kb);
	if (retval) {
		printk(KERN_ERR
		       "comedi: "
		       "failed to create sysfs attribute file \"%s\".\n",
		       dev_attr_read_buffer_kb.attr.name);
		comedi_free_subdevice_minor(s);
		return retval;
	}
	retval = device_create_file(csdev, &dev_attr_max_write_buffer_kb);
	if (retval) {
		printk(KERN_ERR
		       "comedi: "
		       "failed to create sysfs attribute file \"%s\".\n",
		       dev_attr_max_write_buffer_kb.attr.name);
		comedi_free_subdevice_minor(s);
		return retval;
	}
	retval = device_create_file(csdev, &dev_attr_write_buffer_kb);
	if (retval) {
		printk(KERN_ERR
		       "comedi: "
		       "failed to create sysfs attribute file \"%s\".\n",
		       dev_attr_write_buffer_kb.attr.name);
		comedi_free_subdevice_minor(s);
		return retval;
	}
	return i;
=======
void comedi_error(const struct comedi_device *dev, const char *s)
{
	dev_err(dev->class_dev, "%s: %s\n", dev->driver->driver_name, s);
}
EXPORT_SYMBOL_GPL(comedi_error);

void comedi_event(struct comedi_device *dev, struct comedi_subdevice *s)
{
	struct comedi_async *async = s->async;
	unsigned runflags = 0;
	unsigned runflags_mask = 0;

	if (!comedi_is_subdevice_running(s))
		return;

	if (s->
	    async->events & (COMEDI_CB_EOA | COMEDI_CB_ERROR |
			     COMEDI_CB_OVERFLOW)) {
		runflags_mask |= SRF_RUNNING;
	}
	/* remember if an error event has occurred, so an error
	 * can be returned the next time the user does a read() */
	if (s->async->events & (COMEDI_CB_ERROR | COMEDI_CB_OVERFLOW)) {
		runflags_mask |= SRF_ERROR;
		runflags |= SRF_ERROR;
	}
	if (runflags_mask) {
		/*sets SRF_ERROR and SRF_RUNNING together atomically */
		comedi_set_subdevice_runflags(s, runflags_mask, runflags);
	}

	if (async->cb_mask & s->async->events) {
		wake_up_interruptible(&async->wait_head);
		if (s->subdev_flags & SDF_CMD_READ)
			kill_fasync(&dev->async_queue, SIGIO, POLL_IN);
		if (s->subdev_flags & SDF_CMD_WRITE)
			kill_fasync(&dev->async_queue, SIGIO, POLL_OUT);
	}
	s->async->events = 0;
}
EXPORT_SYMBOL_GPL(comedi_event);

/* Note: the ->mutex is pre-locked on successful return */
struct comedi_device *comedi_alloc_board_minor(struct device *hardware_device)
{
	struct comedi_device *dev;
	struct device *csdev;
	unsigned i;

	dev = kzalloc(sizeof(struct comedi_device), GFP_KERNEL);
	if (dev == NULL)
		return ERR_PTR(-ENOMEM);
	comedi_device_init(dev);
	comedi_set_hw_dev(dev, hardware_device);
	mutex_lock(&dev->mutex);
	mutex_lock(&comedi_board_minor_table_lock);
	for (i = hardware_device ? comedi_num_legacy_minors : 0;
	     i < COMEDI_NUM_BOARD_MINORS; ++i) {
		if (comedi_board_minor_table[i] == NULL) {
			comedi_board_minor_table[i] = dev;
			break;
		}
	}
	mutex_unlock(&comedi_board_minor_table_lock);
	if (i == COMEDI_NUM_BOARD_MINORS) {
		mutex_unlock(&dev->mutex);
		comedi_device_cleanup(dev);
		comedi_dev_put(dev);
		pr_err("comedi: error: ran out of minor numbers for board device files.\n");
		return ERR_PTR(-EBUSY);
	}
	dev->minor = i;
	csdev = device_create(comedi_class, hardware_device,
			      MKDEV(COMEDI_MAJOR, i), NULL, "comedi%i", i);
	if (!IS_ERR(csdev))
		dev->class_dev = get_device(csdev);

	/* Note: dev->mutex needs to be unlocked by the caller. */
	return dev;
}

static void comedi_free_board_minor(unsigned minor)
{
	BUG_ON(minor >= COMEDI_NUM_BOARD_MINORS);
	comedi_free_board_dev(comedi_clear_board_minor(minor));
}

void comedi_release_hardware_device(struct device *hardware_device)
{
	int minor;
	struct comedi_device *dev;

	for (minor = comedi_num_legacy_minors; minor < COMEDI_NUM_BOARD_MINORS;
	     minor++) {
		mutex_lock(&comedi_board_minor_table_lock);
		dev = comedi_board_minor_table[minor];
		if (dev && dev->hw_dev == hardware_device) {
			comedi_board_minor_table[minor] = NULL;
			mutex_unlock(&comedi_board_minor_table_lock);
			comedi_free_board_dev(dev);
			break;
		}
		mutex_unlock(&comedi_board_minor_table_lock);
	}
}

int comedi_alloc_subdevice_minor(struct comedi_subdevice *s)
{
	struct comedi_device *dev = s->device;
	struct device *csdev;
	unsigned i;

	mutex_lock(&comedi_subdevice_minor_table_lock);
	for (i = 0; i < COMEDI_NUM_SUBDEVICE_MINORS; ++i) {
		if (comedi_subdevice_minor_table[i] == NULL) {
			comedi_subdevice_minor_table[i] = s;
			break;
		}
	}
	mutex_unlock(&comedi_subdevice_minor_table_lock);
	if (i == COMEDI_NUM_SUBDEVICE_MINORS) {
		pr_err("comedi: error: ran out of minor numbers for subdevice files.\n");
		return -EBUSY;
	}
	i += COMEDI_NUM_BOARD_MINORS;
	s->minor = i;
	csdev = device_create(comedi_class, dev->class_dev,
			      MKDEV(COMEDI_MAJOR, i), NULL, "comedi%i_subd%i",
			      dev->minor, s->index);
	if (!IS_ERR(csdev))
		s->class_dev = csdev;

	return 0;
>>>>>>> refs/remotes/origin/master
}

void comedi_free_subdevice_minor(struct comedi_subdevice *s)
{
<<<<<<< HEAD
	unsigned long flags;
	struct comedi_device_file_info *info;
=======
	unsigned int i;
>>>>>>> refs/remotes/origin/master

	if (s == NULL)
		return;
	if (s->minor < 0)
		return;

	BUG_ON(s->minor >= COMEDI_NUM_MINORS);
<<<<<<< HEAD
	BUG_ON(s->minor < COMEDI_FIRST_SUBDEVICE_MINOR);

	spin_lock_irqsave(&comedi_file_info_table_lock, flags);
	info = comedi_file_info_table[s->minor];
	comedi_file_info_table[s->minor] = NULL;
	spin_unlock_irqrestore(&comedi_file_info_table_lock, flags);

=======
	BUG_ON(s->minor < COMEDI_NUM_BOARD_MINORS);

	i = s->minor - COMEDI_NUM_BOARD_MINORS;
	mutex_lock(&comedi_subdevice_minor_table_lock);
	if (s == comedi_subdevice_minor_table[i])
		comedi_subdevice_minor_table[i] = NULL;
	mutex_unlock(&comedi_subdevice_minor_table_lock);
>>>>>>> refs/remotes/origin/master
	if (s->class_dev) {
		device_destroy(comedi_class, MKDEV(COMEDI_MAJOR, s->minor));
		s->class_dev = NULL;
	}
<<<<<<< HEAD
	kfree(info);
}

struct comedi_device_file_info *comedi_get_device_file_info(unsigned minor)
{
	unsigned long flags;
	struct comedi_device_file_info *info;

	BUG_ON(minor >= COMEDI_NUM_MINORS);
	spin_lock_irqsave(&comedi_file_info_table_lock, flags);
	info = comedi_file_info_table[minor];
	spin_unlock_irqrestore(&comedi_file_info_table_lock, flags);
	return info;
}
EXPORT_SYMBOL_GPL(comedi_get_device_file_info);

static int resize_async_buffer(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_async *async, unsigned new_size)
{
	int retval;

	if (new_size > async->max_bufsize)
		return -EPERM;

	if (s->busy) {
		DPRINTK("subdevice is busy, cannot resize buffer\n");
		return -EBUSY;
	}
	if (async->mmap_count) {
		DPRINTK("subdevice is mmapped, cannot resize buffer\n");
		return -EBUSY;
	}

	if (!async->prealloc_buf)
		return -EINVAL;

	/* make sure buffer is an integral number of pages
	 * (we round up) */
	new_size = (new_size + PAGE_SIZE - 1) & PAGE_MASK;

	retval = comedi_buf_alloc(dev, s, new_size);
	if (retval < 0)
		return retval;

	if (s->buf_change) {
		retval = s->buf_change(dev, s, new_size);
		if (retval < 0)
			return retval;
	}

	DPRINTK("comedi%i subd %d buffer resized to %i bytes\n",
		dev->minor, (int)(s - dev->subdevices), async->prealloc_bufsz);
	return 0;
}

/* sysfs attribute files */

static const unsigned bytes_per_kibi = 1024;

static ssize_t show_max_read_buffer_kb(struct device *dev,
				       struct device_attribute *attr, char *buf)
{
	ssize_t retval;
	struct comedi_device_file_info *info = dev_get_drvdata(dev);
	unsigned max_buffer_size_kb = 0;
	struct comedi_subdevice *const read_subdevice =
	    comedi_get_read_subdevice(info);

	mutex_lock(&info->device->mutex);
	if (read_subdevice &&
	    (read_subdevice->subdev_flags & SDF_CMD_READ) &&
	    read_subdevice->async) {
		max_buffer_size_kb = read_subdevice->async->max_bufsize /
		    bytes_per_kibi;
	}
	retval = snprintf(buf, PAGE_SIZE, "%i\n", max_buffer_size_kb);
	mutex_unlock(&info->device->mutex);

	return retval;
}

static ssize_t store_max_read_buffer_kb(struct device *dev,
					struct device_attribute *attr,
					const char *buf, size_t count)
{
	struct comedi_device_file_info *info = dev_get_drvdata(dev);
<<<<<<< HEAD
	unsigned long new_max_size_kb;
	uint64_t new_max_size;
	struct comedi_subdevice *const read_subdevice =
	    comedi_get_read_subdevice(info);

	if (strict_strtoul(buf, 10, &new_max_size_kb))
		return -EINVAL;
	if (new_max_size_kb != (uint32_t) new_max_size_kb)
		return -EINVAL;
	new_max_size = ((uint64_t) new_max_size_kb) * bytes_per_kibi;
	if (new_max_size != (uint32_t) new_max_size)
		return -EINVAL;
=======
	unsigned int new_max_size_kb;
	unsigned int new_max_size;
	int ret;
	struct comedi_subdevice *const read_subdevice =
	    comedi_get_read_subdevice(info);

	ret = kstrtouint(buf, 10, &new_max_size_kb);
	if (ret)
		return ret;
	if (new_max_size_kb > (UINT_MAX / bytes_per_kibi))
		return -EINVAL;
	new_max_size = new_max_size_kb * bytes_per_kibi;
>>>>>>> refs/remotes/origin/cm-10.0

	mutex_lock(&info->device->mutex);
	if (read_subdevice == NULL ||
	    (read_subdevice->subdev_flags & SDF_CMD_READ) == 0 ||
	    read_subdevice->async == NULL) {
		mutex_unlock(&info->device->mutex);
		return -EINVAL;
	}
	read_subdevice->async->max_bufsize = new_max_size;
	mutex_unlock(&info->device->mutex);

	return count;
}

static struct device_attribute dev_attr_max_read_buffer_kb = {
	.attr = {
		 .name = "max_read_buffer_kb",
		 .mode = S_IRUGO | S_IWUSR},
	.show = &show_max_read_buffer_kb,
	.store = &store_max_read_buffer_kb
};

static ssize_t show_read_buffer_kb(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
	ssize_t retval;
	struct comedi_device_file_info *info = dev_get_drvdata(dev);
	unsigned buffer_size_kb = 0;
	struct comedi_subdevice *const read_subdevice =
	    comedi_get_read_subdevice(info);

	mutex_lock(&info->device->mutex);
	if (read_subdevice &&
	    (read_subdevice->subdev_flags & SDF_CMD_READ) &&
	    read_subdevice->async) {
		buffer_size_kb = read_subdevice->async->prealloc_bufsz /
		    bytes_per_kibi;
	}
	retval = snprintf(buf, PAGE_SIZE, "%i\n", buffer_size_kb);
	mutex_unlock(&info->device->mutex);

	return retval;
}

static ssize_t store_read_buffer_kb(struct device *dev,
				    struct device_attribute *attr,
				    const char *buf, size_t count)
{
	struct comedi_device_file_info *info = dev_get_drvdata(dev);
<<<<<<< HEAD
	unsigned long new_size_kb;
	uint64_t new_size;
	int retval;
	struct comedi_subdevice *const read_subdevice =
	    comedi_get_read_subdevice(info);

	if (strict_strtoul(buf, 10, &new_size_kb))
		return -EINVAL;
	if (new_size_kb != (uint32_t) new_size_kb)
		return -EINVAL;
	new_size = ((uint64_t) new_size_kb) * bytes_per_kibi;
	if (new_size != (uint32_t) new_size)
		return -EINVAL;
=======
	unsigned int new_size_kb;
	unsigned int new_size;
	int retval;
	int ret;
	struct comedi_subdevice *const read_subdevice =
	    comedi_get_read_subdevice(info);

	ret = kstrtouint(buf, 10, &new_size_kb);
	if (ret)
		return ret;
	if (new_size_kb > (UINT_MAX / bytes_per_kibi))
		return -EINVAL;
	new_size = new_size_kb * bytes_per_kibi;
>>>>>>> refs/remotes/origin/cm-10.0

	mutex_lock(&info->device->mutex);
	if (read_subdevice == NULL ||
	    (read_subdevice->subdev_flags & SDF_CMD_READ) == 0 ||
	    read_subdevice->async == NULL) {
		mutex_unlock(&info->device->mutex);
		return -EINVAL;
	}
	retval = resize_async_buffer(info->device, read_subdevice,
				     read_subdevice->async, new_size);
	mutex_unlock(&info->device->mutex);

	if (retval < 0)
		return retval;
	return count;
}

static struct device_attribute dev_attr_read_buffer_kb = {
	.attr = {
		 .name = "read_buffer_kb",
		 .mode = S_IRUGO | S_IWUSR | S_IWGRP},
	.show = &show_read_buffer_kb,
	.store = &store_read_buffer_kb
};

static ssize_t show_max_write_buffer_kb(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	ssize_t retval;
	struct comedi_device_file_info *info = dev_get_drvdata(dev);
	unsigned max_buffer_size_kb = 0;
	struct comedi_subdevice *const write_subdevice =
	    comedi_get_write_subdevice(info);

	mutex_lock(&info->device->mutex);
	if (write_subdevice &&
	    (write_subdevice->subdev_flags & SDF_CMD_WRITE) &&
	    write_subdevice->async) {
		max_buffer_size_kb = write_subdevice->async->max_bufsize /
		    bytes_per_kibi;
	}
	retval = snprintf(buf, PAGE_SIZE, "%i\n", max_buffer_size_kb);
	mutex_unlock(&info->device->mutex);

	return retval;
}

static ssize_t store_max_write_buffer_kb(struct device *dev,
					 struct device_attribute *attr,
					 const char *buf, size_t count)
{
	struct comedi_device_file_info *info = dev_get_drvdata(dev);
<<<<<<< HEAD
	unsigned long new_max_size_kb;
	uint64_t new_max_size;
	struct comedi_subdevice *const write_subdevice =
	    comedi_get_write_subdevice(info);

	if (strict_strtoul(buf, 10, &new_max_size_kb))
		return -EINVAL;
	if (new_max_size_kb != (uint32_t) new_max_size_kb)
		return -EINVAL;
	new_max_size = ((uint64_t) new_max_size_kb) * bytes_per_kibi;
	if (new_max_size != (uint32_t) new_max_size)
		return -EINVAL;
=======
	unsigned int new_max_size_kb;
	unsigned int new_max_size;
	int ret;
	struct comedi_subdevice *const write_subdevice =
	    comedi_get_write_subdevice(info);

	ret = kstrtouint(buf, 10, &new_max_size_kb);
	if (ret)
		return ret;
	if (new_max_size_kb > (UINT_MAX / bytes_per_kibi))
		return -EINVAL;
	new_max_size = new_max_size_kb * bytes_per_kibi;
>>>>>>> refs/remotes/origin/cm-10.0

	mutex_lock(&info->device->mutex);
	if (write_subdevice == NULL ||
	    (write_subdevice->subdev_flags & SDF_CMD_WRITE) == 0 ||
	    write_subdevice->async == NULL) {
		mutex_unlock(&info->device->mutex);
		return -EINVAL;
	}
	write_subdevice->async->max_bufsize = new_max_size;
	mutex_unlock(&info->device->mutex);

	return count;
}

static struct device_attribute dev_attr_max_write_buffer_kb = {
	.attr = {
		 .name = "max_write_buffer_kb",
		 .mode = S_IRUGO | S_IWUSR},
	.show = &show_max_write_buffer_kb,
	.store = &store_max_write_buffer_kb
};

static ssize_t show_write_buffer_kb(struct device *dev,
				    struct device_attribute *attr, char *buf)
{
	ssize_t retval;
	struct comedi_device_file_info *info = dev_get_drvdata(dev);
	unsigned buffer_size_kb = 0;
	struct comedi_subdevice *const write_subdevice =
	    comedi_get_write_subdevice(info);

	mutex_lock(&info->device->mutex);
	if (write_subdevice &&
	    (write_subdevice->subdev_flags & SDF_CMD_WRITE) &&
	    write_subdevice->async) {
		buffer_size_kb = write_subdevice->async->prealloc_bufsz /
		    bytes_per_kibi;
	}
	retval = snprintf(buf, PAGE_SIZE, "%i\n", buffer_size_kb);
	mutex_unlock(&info->device->mutex);

	return retval;
}

static ssize_t store_write_buffer_kb(struct device *dev,
				     struct device_attribute *attr,
				     const char *buf, size_t count)
{
	struct comedi_device_file_info *info = dev_get_drvdata(dev);
<<<<<<< HEAD
	unsigned long new_size_kb;
	uint64_t new_size;
	int retval;
	struct comedi_subdevice *const write_subdevice =
	    comedi_get_write_subdevice(info);

	if (strict_strtoul(buf, 10, &new_size_kb))
		return -EINVAL;
	if (new_size_kb != (uint32_t) new_size_kb)
		return -EINVAL;
	new_size = ((uint64_t) new_size_kb) * bytes_per_kibi;
	if (new_size != (uint32_t) new_size)
		return -EINVAL;
=======
	unsigned int new_size_kb;
	unsigned int new_size;
	int retval;
	int ret;
	struct comedi_subdevice *const write_subdevice =
	    comedi_get_write_subdevice(info);

	ret = kstrtouint(buf, 10, &new_size_kb);
	if (ret)
		return ret;
	if (new_size_kb > (UINT_MAX / bytes_per_kibi))
		return -EINVAL;
	new_size = ((uint64_t) new_size_kb) * bytes_per_kibi;
>>>>>>> refs/remotes/origin/cm-10.0

	mutex_lock(&info->device->mutex);
	if (write_subdevice == NULL ||
	    (write_subdevice->subdev_flags & SDF_CMD_WRITE) == 0 ||
	    write_subdevice->async == NULL) {
		mutex_unlock(&info->device->mutex);
		return -EINVAL;
	}
	retval = resize_async_buffer(info->device, write_subdevice,
				     write_subdevice->async, new_size);
	mutex_unlock(&info->device->mutex);

	if (retval < 0)
		return retval;
	return count;
}

static struct device_attribute dev_attr_write_buffer_kb = {
	.attr = {
		 .name = "write_buffer_kb",
		 .mode = S_IRUGO | S_IWUSR | S_IWGRP},
	.show = &show_write_buffer_kb,
	.store = &store_write_buffer_kb
};
=======
}

static void comedi_cleanup_board_minors(void)
{
	unsigned i;

	for (i = 0; i < COMEDI_NUM_BOARD_MINORS; i++)
		comedi_free_board_minor(i);
}

static int __init comedi_init(void)
{
	int i;
	int retval;

	pr_info("comedi: version " COMEDI_RELEASE " - http://www.comedi.org\n");

	if (comedi_num_legacy_minors < 0 ||
	    comedi_num_legacy_minors > COMEDI_NUM_BOARD_MINORS) {
		pr_err("comedi: error: invalid value for module parameter \"comedi_num_legacy_minors\".  Valid values are 0 through %i.\n",
		       COMEDI_NUM_BOARD_MINORS);
		return -EINVAL;
	}

	retval = register_chrdev_region(MKDEV(COMEDI_MAJOR, 0),
					COMEDI_NUM_MINORS, "comedi");
	if (retval)
		return -EIO;
	cdev_init(&comedi_cdev, &comedi_fops);
	comedi_cdev.owner = THIS_MODULE;
	kobject_set_name(&comedi_cdev.kobj, "comedi");
	if (cdev_add(&comedi_cdev, MKDEV(COMEDI_MAJOR, 0), COMEDI_NUM_MINORS)) {
		unregister_chrdev_region(MKDEV(COMEDI_MAJOR, 0),
					 COMEDI_NUM_MINORS);
		return -EIO;
	}
	comedi_class = class_create(THIS_MODULE, "comedi");
	if (IS_ERR(comedi_class)) {
		pr_err("comedi: failed to create class\n");
		cdev_del(&comedi_cdev);
		unregister_chrdev_region(MKDEV(COMEDI_MAJOR, 0),
					 COMEDI_NUM_MINORS);
		return PTR_ERR(comedi_class);
	}

	comedi_class->dev_groups = comedi_dev_groups;

	/* XXX requires /proc interface */
	comedi_proc_init();

	/* create devices files for legacy/manual use */
	for (i = 0; i < comedi_num_legacy_minors; i++) {
		struct comedi_device *dev;
		dev = comedi_alloc_board_minor(NULL);
		if (IS_ERR(dev)) {
			comedi_cleanup_board_minors();
			cdev_del(&comedi_cdev);
			unregister_chrdev_region(MKDEV(COMEDI_MAJOR, 0),
						 COMEDI_NUM_MINORS);
			return PTR_ERR(dev);
		} else {
			/* comedi_alloc_board_minor() locked the mutex */
			mutex_unlock(&dev->mutex);
		}
	}

	return 0;
}
module_init(comedi_init);

static void __exit comedi_cleanup(void)
{
	int i;

	comedi_cleanup_board_minors();
	for (i = 0; i < COMEDI_NUM_BOARD_MINORS; ++i)
		BUG_ON(comedi_board_minor_table[i]);
	for (i = 0; i < COMEDI_NUM_SUBDEVICE_MINORS; ++i)
		BUG_ON(comedi_subdevice_minor_table[i]);

	class_destroy(comedi_class);
	cdev_del(&comedi_cdev);
	unregister_chrdev_region(MKDEV(COMEDI_MAJOR, 0), COMEDI_NUM_MINORS);

	comedi_proc_cleanup();
}
module_exit(comedi_cleanup);

MODULE_AUTHOR("http://www.comedi.org");
MODULE_DESCRIPTION("Comedi core module");
MODULE_LICENSE("GPL");
>>>>>>> refs/remotes/origin/master
