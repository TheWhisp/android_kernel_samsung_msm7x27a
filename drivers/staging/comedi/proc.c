/*
    module/proc.c
    /proc interface for comedi

    COMEDI - Linux Control and Measurement Device Interface
    Copyright (C) 1998 David A. Schleef <ds@schleef.org>

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

=======
>>>>>>> refs/remotes/origin/master
*/

/*
	This is some serious bloatware.

	Taken from Dave A.'s PCL-711 driver, 'cuz I thought it
	was cool.
*/

<<<<<<< HEAD
#define __NO_VERSION__
#include "comedidev.h"
#include "comedi_fops.h"
#include <linux/proc_fs.h>
#include <linux/string.h>

#ifdef CONFIG_PROC_FS
static int comedi_read(char *buf, char **start, off_t offset, int len,
		       int *eof, void *data)
{
	int i;
	int devices_q = 0;
	int l = 0;
	struct comedi_driver *driv;

	l += sprintf(buf + l,
=======
#include "comedidev.h"
#include "comedi_internal.h"
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

static int comedi_read(struct seq_file *m, void *v)
{
	int i;
	int devices_q = 0;
	struct comedi_driver *driv;

	seq_printf(m,
>>>>>>> refs/remotes/origin/master
		     "comedi version " COMEDI_RELEASE "\n"
		     "format string: %s\n",
		     "\"%2d: %-20s %-20s %4d\", i, "
		     "driver_name, board_name, n_subdevices");

	for (i = 0; i < COMEDI_NUM_BOARD_MINORS; i++) {
<<<<<<< HEAD
		struct comedi_device_file_info *dev_file_info =
		    comedi_get_device_file_info(i);
		struct comedi_device *dev;

		if (dev_file_info == NULL)
			continue;
		dev = dev_file_info->device;

		if (dev->attached) {
			devices_q = 1;
			l += sprintf(buf + l, "%2d: %-20s %-20s %4d\n",
				     i,
				     dev->driver->driver_name,
				     dev->board_name, dev->n_subdevices);
		}
	}
	if (!devices_q)
		l += sprintf(buf + l, "no devices\n");

	for (driv = comedi_drivers; driv; driv = driv->next) {
		l += sprintf(buf + l, "%s:\n", driv->driver_name);
		for (i = 0; i < driv->num_names; i++) {
			l += sprintf(buf + l, " %s\n",
				     *(char **)((char *)driv->board_name +
						i * driv->offset));
		}
		if (!driv->num_names)
			l += sprintf(buf + l, " %s\n", driv->driver_name);
	}

	return l;
}

void comedi_proc_init(void)
{
	struct proc_dir_entry *comedi_proc;

	comedi_proc = create_proc_entry("comedi", S_IFREG | S_IRUGO, NULL);
	if (comedi_proc)
		comedi_proc->read_proc = comedi_read;
=======
		struct comedi_device *dev = comedi_dev_get_from_minor(i);

		if (!dev)
			continue;

		down_read(&dev->attach_lock);
		if (dev->attached) {
			devices_q = 1;
			seq_printf(m, "%2d: %-20s %-20s %4d\n",
				   i, dev->driver->driver_name,
				   dev->board_name, dev->n_subdevices);
		}
		up_read(&dev->attach_lock);
		comedi_dev_put(dev);
	}
	if (!devices_q)
		seq_puts(m, "no devices\n");

	mutex_lock(&comedi_drivers_list_lock);
	for (driv = comedi_drivers; driv; driv = driv->next) {
		seq_printf(m, "%s:\n", driv->driver_name);
		for (i = 0; i < driv->num_names; i++)
			seq_printf(m, " %s\n",
				   *(char **)((char *)driv->board_name +
					      i * driv->offset));

		if (!driv->num_names)
			seq_printf(m, " %s\n", driv->driver_name);
	}
	mutex_unlock(&comedi_drivers_list_lock);

	return 0;
}

/*
 * seq_file wrappers for procfile show routines.
 */
static int comedi_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, comedi_read, NULL);
}

static const struct file_operations comedi_proc_fops = {
	.open		= comedi_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

void comedi_proc_init(void)
{
	proc_create("comedi", 0644, NULL, &comedi_proc_fops);
>>>>>>> refs/remotes/origin/master
}

void comedi_proc_cleanup(void)
{
	remove_proc_entry("comedi", NULL);
}
<<<<<<< HEAD
#endif
=======
>>>>>>> refs/remotes/origin/master
