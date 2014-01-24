/*
    comedi/drivers/das16cs.c
    Driver for Computer Boards PC-CARD DAS16/16.

    COMEDI - Linux Control and Measurement Device Interface
    Copyright (C) 2000, 2001, 2002 David A. Schleef <ds@schleef.org>

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
    PCMCIA support code for this driver is adapted from the dummy_cs.c
    driver of the Linux PCMCIA Card Services package.

    The initial developer of the original code is David A. Hinds
    <dahinds@users.sourceforge.net>.  Portions created by David A. Hinds
    are Copyright (C) 1999 David A. Hinds.  All Rights Reserved.
>>>>>>> refs/remotes/origin/master

*/
/*
Driver: cb_das16_cs
Description: Computer Boards PC-CARD DAS16/16
Devices: [ComputerBoards] PC-CARD DAS16/16 (cb_das16_cs), PC-CARD DAS16/16-AO
Author: ds
Updated: Mon, 04 Nov 2002 20:04:21 -0800
Status: experimental


*/

<<<<<<< HEAD
#include <linux/interrupt.h>
#include <linux/slab.h>
#include "../comedidev.h"
#include <linux/delay.h>
#include <linux/pci.h>
=======
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/delay.h>

#include "../comedidev.h"
>>>>>>> refs/remotes/origin/master

#include <pcmcia/cistpl.h>
#include <pcmcia/ds.h>

<<<<<<< HEAD
=======
#include "comedi_fc.h"
>>>>>>> refs/remotes/origin/master
#include "8253.h"

#define DAS16CS_SIZE			18

#define DAS16CS_ADC_DATA		0
#define DAS16CS_DIO_MUX			2
#define DAS16CS_MISC1			4
#define DAS16CS_MISC2			6
#define DAS16CS_CTR0			8
#define DAS16CS_CTR1			10
#define DAS16CS_CTR2			12
#define DAS16CS_CTR_CONTROL		14
#define DAS16CS_DIO			16

struct das16cs_board {
	const char *name;
	int device_id;
	int n_ao_chans;
};
<<<<<<< HEAD
static const struct das16cs_board das16cs_boards[] = {
	{
	 .device_id = 0x0000,	/* unknown */
	 .name = "PC-CARD DAS16/16",
	 .n_ao_chans = 0,
	 },
	{
	 .device_id = 0x0039,
	 .name = "PC-CARD DAS16/16-AO",
	 .n_ao_chans = 2,
	 },
	{
	 .device_id = 0x4009,
	 .name = "PCM-DAS16s/16",
	 .n_ao_chans = 0,
	 },
};

#define n_boards ARRAY_SIZE(das16cs_boards)
#define thisboard ((const struct das16cs_board *)dev->board_ptr)

struct das16cs_private {
	struct pcmcia_device *link;

=======

static const struct das16cs_board das16cs_boards[] = {
	{
		.name		= "PC-CARD DAS16/16-AO",
		.device_id	= 0x0039,
		.n_ao_chans	= 2,
	}, {
		.name		= "PCM-DAS16s/16",
		.device_id	= 0x4009,
		.n_ao_chans	= 0,
	}, {
		.name		= "PC-CARD DAS16/16",
		.device_id	= 0x0000,	/* unknown */
		.n_ao_chans	= 0,
	},
};

struct das16cs_private {
>>>>>>> refs/remotes/origin/master
	unsigned int ao_readback[2];
	unsigned short status1;
	unsigned short status2;
};
<<<<<<< HEAD
#define devpriv ((struct das16cs_private *)dev->private)

static int das16cs_attach(struct comedi_device *dev,
			  struct comedi_devconfig *it);
static int das16cs_detach(struct comedi_device *dev);
static struct comedi_driver driver_das16cs = {
	.driver_name = "cb_das16_cs",
	.module = THIS_MODULE,
	.attach = das16cs_attach,
	.detach = das16cs_detach,
};

<<<<<<< HEAD
static struct pcmcia_device *cur_dev = NULL;
=======
static struct pcmcia_device *cur_dev;
>>>>>>> refs/remotes/origin/cm-10.0

static const struct comedi_lrange das16cs_ai_range = { 4, {
							   RANGE(-10, 10),
							   RANGE(-5, 5),
							   RANGE(-2.5, 2.5),
							   RANGE(-1.25, 1.25),
							   }
};

static irqreturn_t das16cs_interrupt(int irq, void *d);
static int das16cs_ai_rinsn(struct comedi_device *dev,
			    struct comedi_subdevice *s,
			    struct comedi_insn *insn, unsigned int *data);
static int das16cs_ai_cmd(struct comedi_device *dev,
			  struct comedi_subdevice *s);
static int das16cs_ai_cmdtest(struct comedi_device *dev,
			      struct comedi_subdevice *s,
			      struct comedi_cmd *cmd);
static int das16cs_ao_winsn(struct comedi_device *dev,
			    struct comedi_subdevice *s,
			    struct comedi_insn *insn, unsigned int *data);
static int das16cs_ao_rinsn(struct comedi_device *dev,
			    struct comedi_subdevice *s,
			    struct comedi_insn *insn, unsigned int *data);
static int das16cs_dio_insn_bits(struct comedi_device *dev,
				 struct comedi_subdevice *s,
				 struct comedi_insn *insn, unsigned int *data);
static int das16cs_dio_insn_config(struct comedi_device *dev,
				   struct comedi_subdevice *s,
				   struct comedi_insn *insn,
				   unsigned int *data);
static int das16cs_timer_insn_read(struct comedi_device *dev,
				   struct comedi_subdevice *s,
				   struct comedi_insn *insn,
				   unsigned int *data);
static int das16cs_timer_insn_config(struct comedi_device *dev,
				     struct comedi_subdevice *s,
				     struct comedi_insn *insn,
				     unsigned int *data);

static const struct das16cs_board *das16cs_probe(struct comedi_device *dev,
						 struct pcmcia_device *link)
{
	int i;

	for (i = 0; i < n_boards; i++) {
		if (das16cs_boards[i].device_id == link->card_id)
			return das16cs_boards + i;
	}

<<<<<<< HEAD
	printk("unknown board!\n");
=======
	dev_dbg(dev->hw_dev, "unknown board!\n");
>>>>>>> refs/remotes/origin/cm-10.0

	return NULL;
}

static int das16cs_attach(struct comedi_device *dev,
			  struct comedi_devconfig *it)
{
	struct pcmcia_device *link;
	struct comedi_subdevice *s;
	int ret;
	int i;

<<<<<<< HEAD
	printk("comedi%d: cb_das16_cs: ", dev->minor);
=======
	dev_dbg(dev->hw_dev, "comedi%d: cb_das16_cs: attached\n", dev->minor);
>>>>>>> refs/remotes/origin/cm-10.0

	link = cur_dev;		/* XXX hack */
	if (!link)
		return -EIO;

	dev->iobase = link->resource[0]->start;
<<<<<<< HEAD
	printk("I/O base=0x%04lx ", dev->iobase);

	printk("fingerprint:\n");
	for (i = 0; i < 48; i += 2)
		printk("%04x ", inw(dev->iobase + i));

	printk("\n");
=======
	dev_dbg(dev->hw_dev, "I/O base=0x%04lx\n", dev->iobase);

	dev_dbg(dev->hw_dev, "fingerprint:\n");
	for (i = 0; i < 48; i += 2)
		dev_dbg(dev->hw_dev, "%04x\n", inw(dev->iobase + i));

>>>>>>> refs/remotes/origin/cm-10.0

	ret = request_irq(link->irq, das16cs_interrupt,
			  IRQF_SHARED, "cb_das16_cs", dev);
	if (ret < 0)
		return ret;

	dev->irq = link->irq;

<<<<<<< HEAD
	printk("irq=%u ", dev->irq);
=======
	dev_dbg(dev->hw_dev, "irq=%u\n", dev->irq);
>>>>>>> refs/remotes/origin/cm-10.0

	dev->board_ptr = das16cs_probe(dev, link);
	if (!dev->board_ptr)
		return -EIO;

	dev->board_name = thisboard->name;

	if (alloc_private(dev, sizeof(struct das16cs_private)) < 0)
		return -ENOMEM;

	if (alloc_subdevices(dev, 4) < 0)
		return -ENOMEM;

	s = dev->subdevices + 0;
	dev->read_subdev = s;
	/* analog input subdevice */
	s->type = COMEDI_SUBD_AI;
	s->subdev_flags = SDF_READABLE | SDF_GROUND | SDF_DIFF | SDF_CMD_READ;
	s->n_chan = 16;
	s->maxdata = 0xffff;
	s->range_table = &das16cs_ai_range;
	s->len_chanlist = 16;
	s->insn_read = das16cs_ai_rinsn;
	s->do_cmd = das16cs_ai_cmd;
	s->do_cmdtest = das16cs_ai_cmdtest;

	s = dev->subdevices + 1;
	/* analog output subdevice */
	if (thisboard->n_ao_chans) {
		s->type = COMEDI_SUBD_AO;
		s->subdev_flags = SDF_WRITABLE;
		s->n_chan = thisboard->n_ao_chans;
		s->maxdata = 0xffff;
		s->range_table = &range_bipolar10;
		s->insn_write = &das16cs_ao_winsn;
		s->insn_read = &das16cs_ao_rinsn;
	}

	s = dev->subdevices + 2;
	/* digital i/o subdevice */
	if (1) {
		s->type = COMEDI_SUBD_DIO;
		s->subdev_flags = SDF_READABLE | SDF_WRITABLE;
		s->n_chan = 8;
		s->maxdata = 1;
		s->range_table = &range_digital;
		s->insn_bits = das16cs_dio_insn_bits;
		s->insn_config = das16cs_dio_insn_config;
	} else {
		s->type = COMEDI_SUBD_UNUSED;
	}

	s = dev->subdevices + 3;
	/* timer subdevice */
	if (0) {
		s->type = COMEDI_SUBD_TIMER;
		s->subdev_flags = SDF_READABLE | SDF_WRITABLE;
		s->n_chan = 1;
		s->maxdata = 0xff;
		s->range_table = &range_unknown;
		s->insn_read = das16cs_timer_insn_read;
		s->insn_config = das16cs_timer_insn_config;
	} else {
		s->type = COMEDI_SUBD_UNUSED;
	}

<<<<<<< HEAD
	printk("attached\n");
=======
>>>>>>> refs/remotes/origin/cm-10.0

	return 1;
}

static int das16cs_detach(struct comedi_device *dev)
{
<<<<<<< HEAD
	printk("comedi%d: das16cs: remove\n", dev->minor);
=======
	dev_dbg(dev->hw_dev, "comedi%d: das16cs: remove\n", dev->minor);
>>>>>>> refs/remotes/origin/cm-10.0

	if (dev->irq)
		free_irq(dev->irq, dev);


	return 0;
}
=======

static const struct comedi_lrange das16cs_ai_range = {
	4, {
		BIP_RANGE(10),
		BIP_RANGE(5),
		BIP_RANGE(2.5),
		BIP_RANGE(1.25),
	}
};
>>>>>>> refs/remotes/origin/master

static irqreturn_t das16cs_interrupt(int irq, void *d)
{
	/* struct comedi_device *dev = d; */
	return IRQ_HANDLED;
}

<<<<<<< HEAD
/*
 * "instructions" read/write data in "one-shot" or "software-triggered"
 * mode.
 */
=======
>>>>>>> refs/remotes/origin/master
static int das16cs_ai_rinsn(struct comedi_device *dev,
			    struct comedi_subdevice *s,
			    struct comedi_insn *insn, unsigned int *data)
{
<<<<<<< HEAD
	int i;
	int to;
	int aref;
	int range;
	int chan;
	static int range_bits[] = { 0x800, 0x000, 0x100, 0x200 };

	chan = CR_CHAN(insn->chanspec);
	aref = CR_AREF(insn->chanspec);
	range = CR_RANGE(insn->chanspec);

	outw(chan, dev->iobase + 2);

	devpriv->status1 &= ~0xf320;
	devpriv->status1 |= (aref == AREF_DIFF) ? 0 : 0x0020;
	outw(devpriv->status1, dev->iobase + 4);

	devpriv->status2 &= ~0xff00;
	devpriv->status2 |= range_bits[range];
	outw(devpriv->status2, dev->iobase + 6);

	for (i = 0; i < insn->n; i++) {
		outw(0, dev->iobase);

#define TIMEOUT 1000
		for (to = 0; to < TIMEOUT; to++) {
			if (inw(dev->iobase + 4) & 0x0080)
				break;
		}
		if (to == TIMEOUT) {
<<<<<<< HEAD
			printk("cb_das16_cs: ai timeout\n");
=======
			dev_dbg(dev->hw_dev, "cb_das16_cs: ai timeout\n");
>>>>>>> refs/remotes/origin/cm-10.0
			return -ETIME;
		}
		data[i] = (unsigned short)inw(dev->iobase + 0);
=======
	struct das16cs_private *devpriv = dev->private;
	int chan = CR_CHAN(insn->chanspec);
	int range = CR_RANGE(insn->chanspec);
	int aref = CR_AREF(insn->chanspec);
	int i;
	int to;

	outw(chan, dev->iobase + DAS16CS_DIO_MUX);

	devpriv->status1 &= ~0xf320;
	devpriv->status1 |= (aref == AREF_DIFF) ? 0 : 0x0020;
	outw(devpriv->status1, dev->iobase + DAS16CS_MISC1);

	devpriv->status2 &= ~0xff00;
	switch (range) {
	case 0:
		devpriv->status2 |= 0x800;
		break;
	case 1:
		devpriv->status2 |= 0x000;
		break;
	case 2:
		devpriv->status2 |= 0x100;
		break;
	case 3:
		devpriv->status2 |= 0x200;
		break;
	}
	outw(devpriv->status2, dev->iobase + DAS16CS_MISC2);

	for (i = 0; i < insn->n; i++) {
		outw(0, dev->iobase + DAS16CS_ADC_DATA);

#define TIMEOUT 1000
		for (to = 0; to < TIMEOUT; to++) {
			if (inw(dev->iobase + DAS16CS_MISC1) & 0x0080)
				break;
		}
		if (to == TIMEOUT) {
			dev_dbg(dev->class_dev, "cb_das16_cs: ai timeout\n");
			return -ETIME;
		}
		data[i] = inw(dev->iobase + DAS16CS_ADC_DATA);
>>>>>>> refs/remotes/origin/master
	}

	return i;
}

static int das16cs_ai_cmd(struct comedi_device *dev, struct comedi_subdevice *s)
{
	return -EINVAL;
}

static int das16cs_ai_cmdtest(struct comedi_device *dev,
			      struct comedi_subdevice *s,
			      struct comedi_cmd *cmd)
{
	int err = 0;
	int tmp;

<<<<<<< HEAD
	/* cmdtest tests a particular command to see if it is valid.
	 * Using the cmdtest ioctl, a user can create a valid cmd
	 * and then have it executes by the cmd ioctl.
	 *
	 * cmdtest returns 1,2,3,4 or 0, depending on which tests
	 * the command passes. */

	/* step 1: make sure trigger sources are trivially valid */

	tmp = cmd->start_src;
	cmd->start_src &= TRIG_NOW;
	if (!cmd->start_src || tmp != cmd->start_src)
		err++;

	tmp = cmd->scan_begin_src;
	cmd->scan_begin_src &= TRIG_TIMER | TRIG_EXT;
	if (!cmd->scan_begin_src || tmp != cmd->scan_begin_src)
		err++;

	tmp = cmd->convert_src;
	cmd->convert_src &= TRIG_TIMER | TRIG_EXT;
	if (!cmd->convert_src || tmp != cmd->convert_src)
		err++;

	tmp = cmd->scan_end_src;
	cmd->scan_end_src &= TRIG_COUNT;
	if (!cmd->scan_end_src || tmp != cmd->scan_end_src)
		err++;

	tmp = cmd->stop_src;
	cmd->stop_src &= TRIG_COUNT | TRIG_NONE;
	if (!cmd->stop_src || tmp != cmd->stop_src)
		err++;
=======
	/* Step 1 : check if triggers are trivially valid */

	err |= cfc_check_trigger_src(&cmd->start_src, TRIG_NOW);
	err |= cfc_check_trigger_src(&cmd->scan_begin_src,
					TRIG_TIMER | TRIG_EXT);
	err |= cfc_check_trigger_src(&cmd->convert_src,
					TRIG_TIMER | TRIG_EXT);
	err |= cfc_check_trigger_src(&cmd->scan_end_src, TRIG_COUNT);
	err |= cfc_check_trigger_src(&cmd->stop_src, TRIG_COUNT | TRIG_NONE);
>>>>>>> refs/remotes/origin/master

	if (err)
		return 1;

<<<<<<< HEAD
<<<<<<< HEAD
	/* step 2: make sure trigger sources are unique and mutually compatible */
=======
	/* step 2: make sure trigger sources are unique and
	 * mutually compatible */
>>>>>>> refs/remotes/origin/cm-10.0

	/* note that mutual compatibility is not an issue here */
	if (cmd->scan_begin_src != TRIG_TIMER &&
	    cmd->scan_begin_src != TRIG_EXT)
		err++;
	if (cmd->convert_src != TRIG_TIMER && cmd->convert_src != TRIG_EXT)
		err++;
	if (cmd->stop_src != TRIG_COUNT && cmd->stop_src != TRIG_NONE)
		err++;
=======
	/* Step 2a : make sure trigger sources are unique */

	err |= cfc_check_trigger_is_unique(cmd->scan_begin_src);
	err |= cfc_check_trigger_is_unique(cmd->convert_src);
	err |= cfc_check_trigger_is_unique(cmd->stop_src);

	/* Step 2b : and mutually compatible */
>>>>>>> refs/remotes/origin/master

	if (err)
		return 2;

<<<<<<< HEAD
	/* step 3: make sure arguments are trivially compatible */

	if (cmd->start_arg != 0) {
		cmd->start_arg = 0;
		err++;
	}
=======
	/* Step 3: check if arguments are trivially valid */

	err |= cfc_check_trigger_arg_is(&cmd->start_arg, 0);

>>>>>>> refs/remotes/origin/master
#define MAX_SPEED	10000	/* in nanoseconds */
#define MIN_SPEED	1000000000	/* in nanoseconds */

	if (cmd->scan_begin_src == TRIG_TIMER) {
<<<<<<< HEAD
		if (cmd->scan_begin_arg < MAX_SPEED) {
			cmd->scan_begin_arg = MAX_SPEED;
			err++;
		}
		if (cmd->scan_begin_arg > MIN_SPEED) {
			cmd->scan_begin_arg = MIN_SPEED;
			err++;
		}
=======
		err |= cfc_check_trigger_arg_min(&cmd->scan_begin_arg,
						 MAX_SPEED);
		err |= cfc_check_trigger_arg_max(&cmd->scan_begin_arg,
						 MIN_SPEED);
>>>>>>> refs/remotes/origin/master
	} else {
		/* external trigger */
		/* should be level/edge, hi/lo specification here */
		/* should specify multiple external triggers */
<<<<<<< HEAD
		if (cmd->scan_begin_arg > 9) {
			cmd->scan_begin_arg = 9;
			err++;
		}
	}
	if (cmd->convert_src == TRIG_TIMER) {
		if (cmd->convert_arg < MAX_SPEED) {
			cmd->convert_arg = MAX_SPEED;
			err++;
		}
		if (cmd->convert_arg > MIN_SPEED) {
			cmd->convert_arg = MIN_SPEED;
			err++;
		}
	} else {
		/* external trigger */
		/* see above */
		if (cmd->convert_arg > 9) {
			cmd->convert_arg = 9;
			err++;
		}
	}

	if (cmd->scan_end_arg != cmd->chanlist_len) {
		cmd->scan_end_arg = cmd->chanlist_len;
		err++;
	}
	if (cmd->stop_src == TRIG_COUNT) {
		if (cmd->stop_arg > 0x00ffffff) {
			cmd->stop_arg = 0x00ffffff;
			err++;
		}
	} else {
		/* TRIG_NONE */
		if (cmd->stop_arg != 0) {
			cmd->stop_arg = 0;
			err++;
		}
	}
=======
		err |= cfc_check_trigger_arg_max(&cmd->scan_begin_arg, 9);
	}
	if (cmd->convert_src == TRIG_TIMER) {
		err |= cfc_check_trigger_arg_min(&cmd->convert_arg,
						 MAX_SPEED);
		err |= cfc_check_trigger_arg_max(&cmd->convert_arg,
						 MIN_SPEED);
	} else {
		/* external trigger */
		/* see above */
		err |= cfc_check_trigger_arg_max(&cmd->convert_arg, 9);
	}

	err |= cfc_check_trigger_arg_is(&cmd->scan_end_arg, cmd->chanlist_len);

	if (cmd->stop_src == TRIG_COUNT)
		err |= cfc_check_trigger_arg_max(&cmd->stop_arg, 0x00ffffff);
	else	/* TRIG_NONE */
		err |= cfc_check_trigger_arg_is(&cmd->stop_arg, 0);
>>>>>>> refs/remotes/origin/master

	if (err)
		return 3;

	/* step 4: fix up any arguments */

	if (cmd->scan_begin_src == TRIG_TIMER) {
		unsigned int div1 = 0, div2 = 0;

		tmp = cmd->scan_begin_arg;
<<<<<<< HEAD
		i8253_cascade_ns_to_timer(100, &div1, &div2,
					  &cmd->scan_begin_arg,
					  cmd->flags & TRIG_ROUND_MASK);
=======
		i8253_cascade_ns_to_timer(I8254_OSC_BASE_10MHZ,
					  &div1, &div2,
					  &cmd->scan_begin_arg, cmd->flags);
>>>>>>> refs/remotes/origin/master
		if (tmp != cmd->scan_begin_arg)
			err++;
	}
	if (cmd->convert_src == TRIG_TIMER) {
		unsigned int div1 = 0, div2 = 0;

		tmp = cmd->convert_arg;
<<<<<<< HEAD
		i8253_cascade_ns_to_timer(100, &div1, &div2,
					  &cmd->scan_begin_arg,
					  cmd->flags & TRIG_ROUND_MASK);
=======
		i8253_cascade_ns_to_timer(I8254_OSC_BASE_10MHZ,
					  &div1, &div2,
					  &cmd->scan_begin_arg, cmd->flags);
>>>>>>> refs/remotes/origin/master
		if (tmp != cmd->convert_arg)
			err++;
		if (cmd->scan_begin_src == TRIG_TIMER &&
		    cmd->scan_begin_arg <
		    cmd->convert_arg * cmd->scan_end_arg) {
			cmd->scan_begin_arg =
			    cmd->convert_arg * cmd->scan_end_arg;
			err++;
		}
	}

	if (err)
		return 4;

	return 0;
}

static int das16cs_ao_winsn(struct comedi_device *dev,
			    struct comedi_subdevice *s,
			    struct comedi_insn *insn, unsigned int *data)
{
<<<<<<< HEAD
=======
	struct das16cs_private *devpriv = dev->private;
>>>>>>> refs/remotes/origin/master
	int i;
	int chan = CR_CHAN(insn->chanspec);
	unsigned short status1;
	unsigned short d;
	int bit;

	for (i = 0; i < insn->n; i++) {
		devpriv->ao_readback[chan] = data[i];
		d = data[i];

<<<<<<< HEAD
		outw(devpriv->status1, dev->iobase + 4);
=======
		outw(devpriv->status1, dev->iobase + DAS16CS_MISC1);
>>>>>>> refs/remotes/origin/master
		udelay(1);

		status1 = devpriv->status1 & ~0xf;
		if (chan)
			status1 |= 0x0001;
		else
			status1 |= 0x0008;

<<<<<<< HEAD
<<<<<<< HEAD
/* 		printk("0x%04x\n",status1);*/
=======
/*		printk("0x%04x\n",status1);*/
>>>>>>> refs/remotes/origin/cm-10.0
		outw(status1, dev->iobase + 4);
=======
		outw(status1, dev->iobase + DAS16CS_MISC1);
>>>>>>> refs/remotes/origin/master
		udelay(1);

		for (bit = 15; bit >= 0; bit--) {
			int b = (d >> bit) & 0x1;
			b <<= 1;
<<<<<<< HEAD
/*			printk("0x%04x\n",status1 | b | 0x0000);*/
			outw(status1 | b | 0x0000, dev->iobase + 4);
			udelay(1);
/*			printk("0x%04x\n",status1 | b | 0x0004);*/
			outw(status1 | b | 0x0004, dev->iobase + 4);
			udelay(1);
		}
/*		make high both DAC0CS and DAC1CS to load
		new data and update analog output*/
		outw(status1 | 0x9, dev->iobase + 4);
=======
			outw(status1 | b | 0x0000, dev->iobase + DAS16CS_MISC1);
			udelay(1);
			outw(status1 | b | 0x0004, dev->iobase + DAS16CS_MISC1);
			udelay(1);
		}
		/*
		 * Make both DAC0CS and DAC1CS high to load
		 * the new data and update analog the output
		 */
		outw(status1 | 0x9, dev->iobase + DAS16CS_MISC1);
>>>>>>> refs/remotes/origin/master
	}

	return i;
}

<<<<<<< HEAD
/* AO subdevices should have a read insn as well as a write insn.
 * Usually this means copying a value stored in devpriv. */
=======
>>>>>>> refs/remotes/origin/master
static int das16cs_ao_rinsn(struct comedi_device *dev,
			    struct comedi_subdevice *s,
			    struct comedi_insn *insn, unsigned int *data)
{
<<<<<<< HEAD
=======
	struct das16cs_private *devpriv = dev->private;
>>>>>>> refs/remotes/origin/master
	int i;
	int chan = CR_CHAN(insn->chanspec);

	for (i = 0; i < insn->n; i++)
		data[i] = devpriv->ao_readback[chan];

	return i;
}

<<<<<<< HEAD
/* DIO devices are slightly special.  Although it is possible to
 * implement the insn_read/insn_write interface, it is much more
 * useful to applications if you implement the insn_bits interface.
 * This allows packed reading/writing of the DIO channels.  The
 * comedi core can convert between insn_bits and insn_read/write */
static int das16cs_dio_insn_bits(struct comedi_device *dev,
				 struct comedi_subdevice *s,
				 struct comedi_insn *insn, unsigned int *data)
{
	if (insn->n != 2)
		return -EINVAL;

	if (data[0]) {
		s->state &= ~data[0];
		s->state |= data[0] & data[1];

		outw(s->state, dev->iobase + 16);
	}

	/* on return, data[1] contains the value of the digital
	 * input and output lines. */
	data[1] = inw(dev->iobase + 16);

	return 2;
=======
static int das16cs_dio_insn_bits(struct comedi_device *dev,
				 struct comedi_subdevice *s,
				 struct comedi_insn *insn,
				 unsigned int *data)
{
	if (comedi_dio_update_state(s, data))
		outw(s->state, dev->iobase + DAS16CS_DIO);

	data[1] = inw(dev->iobase + DAS16CS_DIO);

	return insn->n;
>>>>>>> refs/remotes/origin/master
}

static int das16cs_dio_insn_config(struct comedi_device *dev,
				   struct comedi_subdevice *s,
<<<<<<< HEAD
				   struct comedi_insn *insn, unsigned int *data)
{
	int chan = CR_CHAN(insn->chanspec);
	int bits;

	if (chan < 4)
		bits = 0x0f;
	else
		bits = 0xf0;

	switch (data[0]) {
	case INSN_CONFIG_DIO_OUTPUT:
		s->io_bits |= bits;
		break;
	case INSN_CONFIG_DIO_INPUT:
		s->io_bits &= bits;
		break;
	case INSN_CONFIG_DIO_QUERY:
		data[1] =
		    (s->io_bits & (1 << chan)) ? COMEDI_OUTPUT : COMEDI_INPUT;
		return insn->n;
		break;
	default:
		return -EINVAL;
		break;
	}
=======
				   struct comedi_insn *insn,
				   unsigned int *data)
{
	struct das16cs_private *devpriv = dev->private;
	unsigned int chan = CR_CHAN(insn->chanspec);
	unsigned int mask;
	int ret;

	if (chan < 4)
		mask = 0x0f;
	else
		mask = 0xf0;

	ret = comedi_dio_insn_config(dev, s, insn, data, mask);
	if (ret)
		return ret;
>>>>>>> refs/remotes/origin/master

	devpriv->status2 &= ~0x00c0;
	devpriv->status2 |= (s->io_bits & 0xf0) ? 0x0080 : 0;
	devpriv->status2 |= (s->io_bits & 0x0f) ? 0x0040 : 0;

<<<<<<< HEAD
	outw(devpriv->status2, dev->iobase + 6);
=======
	outw(devpriv->status2, dev->iobase + DAS16CS_MISC2);
>>>>>>> refs/remotes/origin/master

	return insn->n;
}

<<<<<<< HEAD
static int das16cs_timer_insn_read(struct comedi_device *dev,
				   struct comedi_subdevice *s,
				   struct comedi_insn *insn, unsigned int *data)
{
	return -EINVAL;
}

static int das16cs_timer_insn_config(struct comedi_device *dev,
				     struct comedi_subdevice *s,
				     struct comedi_insn *insn,
				     unsigned int *data)
{
	return -EINVAL;
}

/* PCMCIA stuff */

/*======================================================================

    The following pcmcia code for the pcm-das08 is adapted from the
    dummy_cs.c driver of the Linux PCMCIA Card Services package.

    The initial developer of the original code is David A. Hinds
    <dahinds@users.sourceforge.net>.  Portions created by David A. Hinds
    are Copyright (C) 1999 David A. Hinds.  All Rights Reserved.

======================================================================*/

#if defined(CONFIG_PCMCIA) || defined(CONFIG_PCMCIA_MODULE)

static void das16cs_pcmcia_config(struct pcmcia_device *link);
static void das16cs_pcmcia_release(struct pcmcia_device *link);
static int das16cs_pcmcia_suspend(struct pcmcia_device *p_dev);
static int das16cs_pcmcia_resume(struct pcmcia_device *p_dev);

/*
   The attach() and detach() entry points are used to create and destroy
   "instances" of the driver, where each instance represents everything
   needed to manage one actual PCMCIA card.
*/

static int das16cs_pcmcia_attach(struct pcmcia_device *);
static void das16cs_pcmcia_detach(struct pcmcia_device *);

/*
   You'll also need to prototype all the functions that will actually
   be used to talk to your device.  See 'memory_cs' for a good example
   of a fully self-sufficient driver; the other drivers rely more or
   less on other parts of the kernel.
*/

struct local_info_t {
	struct pcmcia_device *link;
	int stop;
	struct bus_operations *bus;
};

/*======================================================================

    das16cs_pcmcia_attach() creates an "instance" of the driver, allocating
    local data structures for one device.  The device is registered
    with Card Services.

    The dev_link structure is initialized, but we don't actually
    configure the card at this point -- we wait until we receive a
    card insertion event.

======================================================================*/

static int das16cs_pcmcia_attach(struct pcmcia_device *link)
{
	struct local_info_t *local;

	dev_dbg(&link->dev, "das16cs_pcmcia_attach()\n");

	/* Allocate space for private device-specific data */
	local = kzalloc(sizeof(struct local_info_t), GFP_KERNEL);
	if (!local)
		return -ENOMEM;
	local->link = link;
	link->priv = local;

	cur_dev = link;

	das16cs_pcmcia_config(link);

	return 0;
}				/* das16cs_pcmcia_attach */

static void das16cs_pcmcia_detach(struct pcmcia_device *link)
{
	dev_dbg(&link->dev, "das16cs_pcmcia_detach\n");

	((struct local_info_t *)link->priv)->stop = 1;
	das16cs_pcmcia_release(link);
	/* This points to the parent struct local_info_t struct */
	kfree(link->priv);
}				/* das16cs_pcmcia_detach */


static int das16cs_pcmcia_config_loop(struct pcmcia_device *p_dev,
				void *priv_data)
{
	if (p_dev->config_index == 0)
		return -EINVAL;

	return pcmcia_request_io(p_dev);
}

static void das16cs_pcmcia_config(struct pcmcia_device *link)
{
	int ret;

	dev_dbg(&link->dev, "das16cs_pcmcia_config\n");

	/* Do we need to allocate an interrupt? */
	link->config_flags |= CONF_ENABLE_IRQ | CONF_AUTO_SET_IO;

	ret = pcmcia_loop_config(link, das16cs_pcmcia_config_loop, NULL);
	if (ret) {
		dev_warn(&link->dev, "no configuration found\n");
		goto failed;
	}

	if (!link->irq)
		goto failed;

	ret = pcmcia_enable_device(link);
	if (ret)
		goto failed;

	return;

failed:
	das16cs_pcmcia_release(link);
}				/* das16cs_pcmcia_config */

static void das16cs_pcmcia_release(struct pcmcia_device *link)
{
	dev_dbg(&link->dev, "das16cs_pcmcia_release\n");
	pcmcia_disable_device(link);
}				/* das16cs_pcmcia_release */

static int das16cs_pcmcia_suspend(struct pcmcia_device *link)
{
	struct local_info_t *local = link->priv;

	/* Mark the device as stopped, to block IO until later */
	local->stop = 1;

	return 0;
}				/* das16cs_pcmcia_suspend */

static int das16cs_pcmcia_resume(struct pcmcia_device *link)
{
	struct local_info_t *local = link->priv;

	local->stop = 0;
	return 0;
}				/* das16cs_pcmcia_resume */

/*====================================================================*/
=======
static const void *das16cs_find_boardinfo(struct comedi_device *dev,
					  struct pcmcia_device *link)
{
	const struct das16cs_board *board;
	int i;

	for (i = 0; i < ARRAY_SIZE(das16cs_boards); i++) {
		board = &das16cs_boards[i];
		if (board->device_id == link->card_id)
			return board;
	}

	return NULL;
}

static int das16cs_auto_attach(struct comedi_device *dev,
			       unsigned long context)
{
	struct pcmcia_device *link = comedi_to_pcmcia_dev(dev);
	const struct das16cs_board *board;
	struct das16cs_private *devpriv;
	struct comedi_subdevice *s;
	int ret;

	board = das16cs_find_boardinfo(dev, link);
	if (!board)
		return -ENODEV;
	dev->board_ptr = board;
	dev->board_name = board->name;

	link->config_flags |= CONF_AUTO_SET_IO | CONF_ENABLE_IRQ;
	ret = comedi_pcmcia_enable(dev, NULL);
	if (ret)
		return ret;
	dev->iobase = link->resource[0]->start;

	link->priv = dev;
	ret = pcmcia_request_irq(link, das16cs_interrupt);
	if (ret)
		return ret;
	dev->irq = link->irq;

	devpriv = comedi_alloc_devpriv(dev, sizeof(*devpriv));
	if (!devpriv)
		return -ENOMEM;

	ret = comedi_alloc_subdevices(dev, 3);
	if (ret)
		return ret;

	s = &dev->subdevices[0];
	dev->read_subdev = s;
	/* analog input subdevice */
	s->type		= COMEDI_SUBD_AI;
	s->subdev_flags	= SDF_READABLE | SDF_GROUND | SDF_DIFF | SDF_CMD_READ;
	s->n_chan	= 16;
	s->maxdata	= 0xffff;
	s->range_table	= &das16cs_ai_range;
	s->len_chanlist	= 16;
	s->insn_read	= das16cs_ai_rinsn;
	s->do_cmd	= das16cs_ai_cmd;
	s->do_cmdtest	= das16cs_ai_cmdtest;

	s = &dev->subdevices[1];
	/* analog output subdevice */
	if (board->n_ao_chans) {
		s->type		= COMEDI_SUBD_AO;
		s->subdev_flags	= SDF_WRITABLE;
		s->n_chan	= board->n_ao_chans;
		s->maxdata	= 0xffff;
		s->range_table	= &range_bipolar10;
		s->insn_write	= &das16cs_ao_winsn;
		s->insn_read	= &das16cs_ao_rinsn;
	} else {
		s->type		= COMEDI_SUBD_UNUSED;
	}

	s = &dev->subdevices[2];
	/* digital i/o subdevice */
	s->type		= COMEDI_SUBD_DIO;
	s->subdev_flags	= SDF_READABLE | SDF_WRITABLE;
	s->n_chan	= 8;
	s->maxdata	= 1;
	s->range_table	= &range_digital;
	s->insn_bits	= das16cs_dio_insn_bits;
	s->insn_config	= das16cs_dio_insn_config;

	dev_info(dev->class_dev, "%s: %s, I/O base=0x%04lx, irq=%u\n",
		dev->driver->driver_name, dev->board_name,
		dev->iobase, dev->irq);

	return 0;
}

static struct comedi_driver driver_das16cs = {
	.driver_name	= "cb_das16_cs",
	.module		= THIS_MODULE,
	.auto_attach	= das16cs_auto_attach,
	.detach		= comedi_pcmcia_disable,
};

static int das16cs_pcmcia_attach(struct pcmcia_device *link)
{
	return comedi_pcmcia_auto_config(link, &driver_das16cs);
}
>>>>>>> refs/remotes/origin/master

static const struct pcmcia_device_id das16cs_id_table[] = {
	PCMCIA_DEVICE_MANF_CARD(0x01c5, 0x0039),
	PCMCIA_DEVICE_MANF_CARD(0x01c5, 0x4009),
	PCMCIA_DEVICE_NULL
};
<<<<<<< HEAD

MODULE_DEVICE_TABLE(pcmcia, das16cs_id_table);
MODULE_AUTHOR("David A. Schleef <ds@schleef.org>");
MODULE_DESCRIPTION("Comedi driver for Computer Boards PC-CARD DAS16/16");
MODULE_LICENSE("GPL");

struct pcmcia_driver das16cs_driver = {
	.probe = das16cs_pcmcia_attach,
	.remove = das16cs_pcmcia_detach,
	.suspend = das16cs_pcmcia_suspend,
	.resume = das16cs_pcmcia_resume,
	.id_table = das16cs_id_table,
	.owner = THIS_MODULE,
	.name = "cb_das16_cs",
};

static int __init init_das16cs_pcmcia_cs(void)
{
	pcmcia_register_driver(&das16cs_driver);
	return 0;
}

static void __exit exit_das16cs_pcmcia_cs(void)
{
	pr_debug("das16cs_pcmcia_cs: unloading\n");
	pcmcia_unregister_driver(&das16cs_driver);
}

int __init init_module(void)
{
	int ret;

	ret = init_das16cs_pcmcia_cs();
	if (ret < 0)
		return ret;

	return comedi_driver_register(&driver_das16cs);
}

void __exit cleanup_module(void)
{
	exit_das16cs_pcmcia_cs();
	comedi_driver_unregister(&driver_das16cs);
}

#else
static int __init driver_das16cs_init_module(void)
{
	return comedi_driver_register(&driver_das16cs);
}

static void __exit driver_das16cs_cleanup_module(void)
{
	comedi_driver_unregister(&driver_das16cs);
}

module_init(driver_das16cs_init_module);
module_exit(driver_das16cs_cleanup_module);
#endif /* CONFIG_PCMCIA */
=======
MODULE_DEVICE_TABLE(pcmcia, das16cs_id_table);

static struct pcmcia_driver das16cs_driver = {
	.name		= "cb_das16_cs",
	.owner		= THIS_MODULE,
	.id_table	= das16cs_id_table,
	.probe		= das16cs_pcmcia_attach,
	.remove		= comedi_pcmcia_auto_unconfig,
};
module_comedi_pcmcia_driver(driver_das16cs, das16cs_driver);

MODULE_AUTHOR("David A. Schleef <ds@schleef.org>");
MODULE_DESCRIPTION("Comedi driver for Computer Boards PC-CARD DAS16/16");
MODULE_LICENSE("GPL");
>>>>>>> refs/remotes/origin/master
