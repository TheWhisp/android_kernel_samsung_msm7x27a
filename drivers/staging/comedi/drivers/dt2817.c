/*
    comedi/drivers/dt2817.c
    Hardware driver for Data Translation DT2817

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
Driver: dt2817
Description: Data Translation DT2817
Author: ds
Status: complete
Devices: [Data Translation] DT2817 (dt2817)

A very simple digital I/O card.  Four banks of 8 lines, each bank
is configurable for input or output.  One wonders why it takes a
50 page manual to describe this thing.

The driver (which, btw, is much less than 50 pages) has 1 subdevice
with 32 channels, configurable in groups of 8.

Configuration options:
  [0] - I/O port base base address
*/

<<<<<<< HEAD
#include "../comedidev.h"

#include <linux/ioport.h>

=======
#include <linux/module.h>
#include "../comedidev.h"

>>>>>>> refs/remotes/origin/master
#define DT2817_SIZE 5

#define DT2817_CR 0
#define DT2817_DATA 1

<<<<<<< HEAD
static int dt2817_attach(struct comedi_device *dev,
			 struct comedi_devconfig *it);
static int dt2817_detach(struct comedi_device *dev);
static struct comedi_driver driver_dt2817 = {
	.driver_name = "dt2817",
	.module = THIS_MODULE,
	.attach = dt2817_attach,
	.detach = dt2817_detach,
};

static int __init driver_dt2817_init_module(void)
{
	return comedi_driver_register(&driver_dt2817);
}

static void __exit driver_dt2817_cleanup_module(void)
{
	comedi_driver_unregister(&driver_dt2817);
}

module_init(driver_dt2817_init_module);
module_exit(driver_dt2817_cleanup_module);

static int dt2817_dio_insn_config(struct comedi_device *dev,
				  struct comedi_subdevice *s,
				  struct comedi_insn *insn, unsigned int *data)
{
	int mask;
	int chan;
	int oe = 0;

	if (insn->n != 1)
		return -EINVAL;

	chan = CR_CHAN(insn->chanspec);
	if (chan < 8)
		mask = 0xff;
	else if (chan < 16)
		mask = 0xff00;
	else if (chan < 24)
		mask = 0xff0000;
	else
		mask = 0xff000000;
	if (data[0])
		s->io_bits |= mask;
	else
		s->io_bits &= ~mask;
=======
static int dt2817_dio_insn_config(struct comedi_device *dev,
				  struct comedi_subdevice *s,
				  struct comedi_insn *insn,
				  unsigned int *data)
{
	unsigned int chan = CR_CHAN(insn->chanspec);
	unsigned int oe = 0;
	unsigned int mask;
	int ret;

	if (chan < 8)
		mask = 0x000000ff;
	else if (chan < 16)
		mask = 0x0000ff00;
	else if (chan < 24)
		mask = 0x00ff0000;
	else
		mask = 0xff000000;

	ret = comedi_dio_insn_config(dev, s, insn, data, mask);
	if (ret)
		return ret;
>>>>>>> refs/remotes/origin/master

	if (s->io_bits & 0x000000ff)
		oe |= 0x1;
	if (s->io_bits & 0x0000ff00)
		oe |= 0x2;
	if (s->io_bits & 0x00ff0000)
		oe |= 0x4;
	if (s->io_bits & 0xff000000)
		oe |= 0x8;

	outb(oe, dev->iobase + DT2817_CR);

<<<<<<< HEAD
	return 1;
=======
	return insn->n;
>>>>>>> refs/remotes/origin/master
}

static int dt2817_dio_insn_bits(struct comedi_device *dev,
				struct comedi_subdevice *s,
<<<<<<< HEAD
				struct comedi_insn *insn, unsigned int *data)
{
	unsigned int changed;

	/* It's questionable whether it is more important in
	 * a driver like this to be deterministic or fast.
	 * We choose fast. */

	if (data[0]) {
		changed = s->state;
		s->state &= ~data[0];
		s->state |= (data[0] & data[1]);
		changed ^= s->state;
		changed &= s->io_bits;
		if (changed & 0x000000ff)
			outb(s->state & 0xff, dev->iobase + DT2817_DATA + 0);
		if (changed & 0x0000ff00)
			outb((s->state >> 8) & 0xff,
			     dev->iobase + DT2817_DATA + 1);
		if (changed & 0x00ff0000)
			outb((s->state >> 16) & 0xff,
			     dev->iobase + DT2817_DATA + 2);
		if (changed & 0xff000000)
			outb((s->state >> 24) & 0xff,
			     dev->iobase + DT2817_DATA + 3);
	}
	data[1] = inb(dev->iobase + DT2817_DATA + 0);
	data[1] |= (inb(dev->iobase + DT2817_DATA + 1) << 8);
	data[1] |= (inb(dev->iobase + DT2817_DATA + 2) << 16);
	data[1] |= (inb(dev->iobase + DT2817_DATA + 3) << 24);

	return 2;
=======
				struct comedi_insn *insn,
				unsigned int *data)
{
	unsigned long iobase = dev->iobase + DT2817_DATA;
	unsigned int mask;
	unsigned int val;

	mask = comedi_dio_update_state(s, data);
	if (mask) {
		if (mask & 0x000000ff)
			outb(s->state & 0xff, iobase + 0);
		if (mask & 0x0000ff00)
			outb((s->state >> 8) & 0xff, iobase + 1);
		if (mask & 0x00ff0000)
			outb((s->state >> 16) & 0xff, iobase + 2);
		if (mask & 0xff000000)
			outb((s->state >> 24) & 0xff, iobase + 3);
	}

	val = inb(iobase + 0);
	val |= (inb(iobase + 1) << 8);
	val |= (inb(iobase + 2) << 16);
	val |= (inb(iobase + 3) << 24);

	data[1] = val;

	return insn->n;
>>>>>>> refs/remotes/origin/master
}

static int dt2817_attach(struct comedi_device *dev, struct comedi_devconfig *it)
{
	int ret;
	struct comedi_subdevice *s;
<<<<<<< HEAD
	unsigned long iobase;

	iobase = it->options[0];
	printk(KERN_INFO "comedi%d: dt2817: 0x%04lx ", dev->minor, iobase);
	if (!request_region(iobase, DT2817_SIZE, "dt2817")) {
		printk("I/O port conflict\n");
		return -EIO;
	}
	dev->iobase = iobase;
	dev->board_name = "dt2817";

	ret = alloc_subdevices(dev, 1);
	if (ret < 0)
		return ret;

	s = dev->subdevices + 0;
=======

	ret = comedi_request_region(dev, it->options[0], DT2817_SIZE);
	if (ret)
		return ret;

	ret = comedi_alloc_subdevices(dev, 1);
	if (ret)
		return ret;

	s = &dev->subdevices[0];
>>>>>>> refs/remotes/origin/master

	s->n_chan = 32;
	s->type = COMEDI_SUBD_DIO;
	s->subdev_flags = SDF_READABLE | SDF_WRITABLE;
	s->range_table = &range_digital;
	s->maxdata = 1;
	s->insn_bits = dt2817_dio_insn_bits;
	s->insn_config = dt2817_dio_insn_config;

	s->state = 0;
	outb(0, dev->iobase + DT2817_CR);

<<<<<<< HEAD
	printk(KERN_INFO "\n");

	return 0;
}

static int dt2817_detach(struct comedi_device *dev)
{
	printk(KERN_INFO "comedi%d: dt2817: remove\n", dev->minor);

	if (dev->iobase)
		release_region(dev->iobase, DT2817_SIZE);

	return 0;
}
=======
	return 0;
}

static struct comedi_driver dt2817_driver = {
	.driver_name	= "dt2817",
	.module		= THIS_MODULE,
	.attach		= dt2817_attach,
	.detach		= comedi_legacy_detach,
};
module_comedi_driver(dt2817_driver);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Comedi http://www.comedi.org");
MODULE_DESCRIPTION("Comedi low-level driver");
MODULE_LICENSE("GPL");
