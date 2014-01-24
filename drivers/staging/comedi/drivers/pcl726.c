/*
<<<<<<< HEAD
    comedi/drivers/pcl726.c

    hardware driver for Advantech cards:
     card:   PCL-726, PCL-727, PCL-728
     driver: pcl726,  pcl727,  pcl728
    and for ADLink cards:
     card:   ACL-6126, ACL-6128
     driver: acl6126,  acl6128

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

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/
/*
Driver: pcl726
Description: Advantech PCL-726 & compatibles
Author: ds
Status: untested
Devices: [Advantech] PCL-726 (pcl726), PCL-727 (pcl727), PCL-728 (pcl728),
  [ADLink] ACL-6126 (acl6126), ACL-6128 (acl6128)

Interrupts are not supported.

    Options for PCL-726:
     [0] - IO Base
     [2]...[7] - D/A output range for channel 1-6:
		0: 0-5V, 1: 0-10V, 2: +/-5V, 3: +/-10V,
		4: 4-20mA, 5: unknown (external reference)

    Options for PCL-727:
     [0] - IO Base
     [2]...[13] - D/A output range for channel 1-12:
		0: 0-5V, 1: 0-10V, 2: +/-5V,
		3: 4-20mA

    Options for PCL-728 and ACL-6128:
     [0] - IO Base
     [2], [3] - D/A output range for channel 1 and 2:
		0: 0-5V, 1: 0-10V, 2: +/-5V, 3: +/-10V,
		4: 4-20mA, 5: 0-20mA

    Options for ACL-6126:
     [0] - IO Base
     [1] - IRQ (0=disable, 3, 5, 6, 7, 9, 10, 11, 12, 15) (currently ignored)
     [2]...[7] - D/A output range for channel 1-6:
		0: 0-5V, 1: 0-10V, 2: +/-5V, 3: +/-10V,
		4: 4-20mA
*/

/*
    Thanks to Circuit Specialists for having programming info (!) on
    their web page.  (http://www.cir.com/)
*/

#include "../comedidev.h"

#include <linux/ioport.h>

#undef ACL6126_IRQ		/* no interrupt support (yet) */

#define PCL726_SIZE 16
#define PCL727_SIZE 32
#define PCL728_SIZE 8

#define PCL726_DAC0_HI 0
#define PCL726_DAC0_LO 1

#define PCL726_DO_HI 12
#define PCL726_DO_LO 13
#define PCL726_DI_HI 14
#define PCL726_DI_LO 15

#define PCL727_DO_HI 24
#define PCL727_DO_LO 25
#define PCL727_DI_HI  0
#define PCL727_DI_LO  1

static const struct comedi_lrange range_4_20mA = { 1, {RANGE_mA(4, 20)} };
static const struct comedi_lrange range_0_20mA = { 1, {RANGE_mA(0, 20)} };

static const struct comedi_lrange *const rangelist_726[] = {
	&range_unipolar5, &range_unipolar10,
	&range_bipolar5, &range_bipolar10,
	&range_4_20mA, &range_unknown
};

static const struct comedi_lrange *const rangelist_727[] = {
	&range_unipolar5, &range_unipolar10,
=======
 * pcl726.c
 * Comedi driver for 6/12-Channel D/A Output and DIO cards
 *
 * COMEDI - Linux Control and Measurement Device Interface
 * Copyright (C) 1998 David A. Schleef <ds@schleef.org>
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
 */

/*
 * Driver: pcl726
 * Description: Advantech PCL-726 & compatibles
 * Author: David A. Schleef <ds@schleef.org>
 * Status: untested
 * Devices: (Advantech) PCL-726 [pcl726]
 *	    (Advantech) PCL-727 [pcl727]
 *	    (Advantech) PCL-728 [pcl728]
 *	    (ADLink) ACL-6126 [acl6126]
 *	    (ADLink) ACL-6128 [acl6128]
 *
 * Configuration Options:
 *   [0]  - IO Base
 *   [1]  - IRQ (ACL-6126 only)
 *   [2]  - D/A output range for channel 0
 *   [3]  - D/A output range for channel 1
 *
 * Boards with > 2 analog output channels:
 *   [4]  - D/A output range for channel 2
 *   [5]  - D/A output range for channel 3
 *   [6]  - D/A output range for channel 4
 *   [7]  - D/A output range for channel 5
 *
 * Boards with > 6 analog output channels:
 *   [8]  - D/A output range for channel 6
 *   [9]  - D/A output range for channel 7
 *   [10] - D/A output range for channel 8
 *   [11] - D/A output range for channel 9
 *   [12] - D/A output range for channel 10
 *   [13] - D/A output range for channel 11
 *
 * For PCL-726 the D/A output ranges are:
 *   0: 0-5V, 1: 0-10V, 2: +/-5V, 3: +/-10V, 4: 4-20mA, 5: unknown
 *
 * For PCL-727:
 *   0: 0-5V, 1: 0-10V, 2: +/-5V, 3: 4-20mA
 *
 * For PCL-728 and ACL-6128:
 *   0: 0-5V, 1: 0-10V, 2: +/-5V, 3: +/-10V, 4: 4-20mA, 5: 0-20mA
 *
 * For ACL-6126:
 *   0: 0-5V, 1: 0-10V, 2: +/-5V, 3: +/-10V, 4: 4-20mA
 */

#include <linux/module.h>
#include <linux/interrupt.h>

#include "../comedidev.h"

#include "comedi_fc.h"

#define PCL726_AO_MSB_REG(x)	(0x00 + ((x) * 2))
#define PCL726_AO_LSB_REG(x)	(0x01 + ((x) * 2))
#define PCL726_DO_MSB_REG	0x0c
#define PCL726_DO_LSB_REG	0x0d
#define PCL726_DI_MSB_REG	0x0e
#define PCL726_DI_LSB_REG	0x0f

#define PCL727_DI_MSB_REG	0x00
#define PCL727_DI_LSB_REG	0x01
#define PCL727_DO_MSB_REG	0x18
#define PCL727_DO_LSB_REG	0x19

static const struct comedi_lrange *const rangelist_726[] = {
	&range_unipolar5,
	&range_unipolar10,
	&range_bipolar5,
	&range_bipolar10,
	&range_4_20mA,
	&range_unknown
};

static const struct comedi_lrange *const rangelist_727[] = {
	&range_unipolar5,
	&range_unipolar10,
>>>>>>> refs/remotes/origin/master
	&range_bipolar5,
	&range_4_20mA
};

static const struct comedi_lrange *const rangelist_728[] = {
<<<<<<< HEAD
	&range_unipolar5, &range_unipolar10,
	&range_bipolar5, &range_bipolar10,
	&range_4_20mA, &range_0_20mA
};

static int pcl726_attach(struct comedi_device *dev,
			 struct comedi_devconfig *it);
static int pcl726_detach(struct comedi_device *dev);

struct pcl726_board {

	const char *name;	/*  driver name */
	int n_aochan;		/*  num of D/A chans */
	int num_of_ranges;	/*  num of ranges */
	unsigned int IRQbits;	/*  allowed interrupts */
	unsigned int io_range;	/*  len of IO space */
	char have_dio;		/*  1=card have DI/DO ports */
	int di_hi;		/*  ports for DI/DO operations */
	int di_lo;
	int do_hi;
	int do_lo;
	const struct comedi_lrange *const *range_type_list;
	/*  list of supported ranges */
};

static const struct pcl726_board boardtypes[] = {
	{"pcl726", 6, 6, 0x0000, PCL726_SIZE, 1,
	 PCL726_DI_HI, PCL726_DI_LO, PCL726_DO_HI, PCL726_DO_LO,
	 &rangelist_726[0],},
	{"pcl727", 12, 4, 0x0000, PCL727_SIZE, 1,
	 PCL727_DI_HI, PCL727_DI_LO, PCL727_DO_HI, PCL727_DO_LO,
	 &rangelist_727[0],},
	{"pcl728", 2, 6, 0x0000, PCL728_SIZE, 0,
	 0, 0, 0, 0,
	 &rangelist_728[0],},
	{"acl6126", 6, 5, 0x96e8, PCL726_SIZE, 1,
	 PCL726_DI_HI, PCL726_DI_LO, PCL726_DO_HI, PCL726_DO_LO,
	 &rangelist_726[0],},
	{"acl6128", 2, 6, 0x0000, PCL728_SIZE, 0,
	 0, 0, 0, 0,
	 &rangelist_728[0],},
};

#define n_boardtypes (sizeof(boardtypes)/sizeof(struct pcl726_board))
#define this_board ((const struct pcl726_board *)dev->board_ptr)

static struct comedi_driver driver_pcl726 = {
	.driver_name = "pcl726",
	.module = THIS_MODULE,
	.attach = pcl726_attach,
	.detach = pcl726_detach,
	.board_name = &boardtypes[0].name,
	.num_names = n_boardtypes,
	.offset = sizeof(struct pcl726_board),
};

static int __init driver_pcl726_init_module(void)
{
	return comedi_driver_register(&driver_pcl726);
}

static void __exit driver_pcl726_cleanup_module(void)
{
	comedi_driver_unregister(&driver_pcl726);
}

module_init(driver_pcl726_init_module);
module_exit(driver_pcl726_cleanup_module);

struct pcl726_private {

	int bipolar[12];
	const struct comedi_lrange *rangelist[12];
	unsigned int ao_readback[12];
};

#define devpriv ((struct pcl726_private *)dev->private)

static int pcl726_ao_insn(struct comedi_device *dev, struct comedi_subdevice *s,
			  struct comedi_insn *insn, unsigned int *data)
{
	int hi, lo;
	int n;
	int chan = CR_CHAN(insn->chanspec);

	for (n = 0; n < insn->n; n++) {
		lo = data[n] & 0xff;
		hi = (data[n] >> 8) & 0xf;
		if (devpriv->bipolar[chan])
			hi ^= 0x8;
		/*
		 * the programming info did not say which order
		 * to write bytes.  switch the order of the next
		 * two lines if you get glitches.
		 */
		outb(hi, dev->iobase + PCL726_DAC0_HI + 2 * chan);
		outb(lo, dev->iobase + PCL726_DAC0_LO + 2 * chan);
		devpriv->ao_readback[chan] = data[n];
	}

	return n;
=======
	&range_unipolar5,
	&range_unipolar10,
	&range_bipolar5,
	&range_bipolar10,
	&range_4_20mA,
	&range_0_20mA
};

struct pcl726_board {
	const char *name;
	unsigned long io_len;
	unsigned int irq_mask;
	const struct comedi_lrange *const *ao_ranges;
	int ao_num_ranges;
	int ao_nchan;
	unsigned int have_dio:1;
	unsigned int is_pcl727:1;
};

static const struct pcl726_board pcl726_boards[] = {
	{
		.name		= "pcl726",
		.io_len		= 0x10,
		.ao_ranges	= &rangelist_726[0],
		.ao_num_ranges	= ARRAY_SIZE(rangelist_726),
		.ao_nchan	= 6,
		.have_dio	= 1,
	}, {
		.name		= "pcl727",
		.io_len		= 0x20,
		.ao_ranges	= &rangelist_727[0],
		.ao_num_ranges	= ARRAY_SIZE(rangelist_727),
		.ao_nchan	= 12,
		.have_dio	= 1,
		.is_pcl727	= 1,
	}, {
		.name		= "pcl728",
		.io_len		= 0x08,
		.ao_num_ranges	= ARRAY_SIZE(rangelist_728),
		.ao_ranges	= &rangelist_728[0],
		.ao_nchan	= 2,
	}, {
		.name		= "acl6126",
		.io_len		= 0x10,
		.irq_mask	= 0x96e8,
		.ao_num_ranges	= ARRAY_SIZE(rangelist_726),
		.ao_ranges	= &rangelist_726[0],
		.ao_nchan	= 6,
		.have_dio	= 1,
	}, {
		.name		= "acl6128",
		.io_len		= 0x08,
		.ao_num_ranges	= ARRAY_SIZE(rangelist_728),
		.ao_ranges	= &rangelist_728[0],
		.ao_nchan	= 2,
	},
};

struct pcl726_private {
	const struct comedi_lrange *rangelist[12];
	unsigned int ao_readback[12];
	unsigned int cmd_running:1;
};

static int pcl726_intr_insn_bits(struct comedi_device *dev,
				 struct comedi_subdevice *s,
				 struct comedi_insn *insn,
				 unsigned int *data)
{
	data[1] = 0;
	return insn->n;
}

static int pcl726_intr_cmdtest(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_cmd *cmd)
{
	int err = 0;

	/* Step 1 : check if triggers are trivially valid */

	err |= cfc_check_trigger_src(&cmd->start_src, TRIG_NOW);
	err |= cfc_check_trigger_src(&cmd->scan_begin_src, TRIG_EXT);
	err |= cfc_check_trigger_src(&cmd->convert_src, TRIG_FOLLOW);
	err |= cfc_check_trigger_src(&cmd->scan_end_src, TRIG_COUNT);
	err |= cfc_check_trigger_src(&cmd->stop_src, TRIG_NONE);

	if (err)
		return 1;

	/* Step 2a : make sure trigger sources are unique */
	/* Step 2b : and mutually compatible */

	if (err)
		return 2;

	/* Step 3: check if arguments are trivially valid */

	err |= cfc_check_trigger_arg_is(&cmd->start_arg, 0);
	err |= cfc_check_trigger_arg_is(&cmd->scan_begin_arg, 0);
	err |= cfc_check_trigger_arg_is(&cmd->convert_arg, 0);
	err |= cfc_check_trigger_arg_is(&cmd->scan_end_arg, 1);
	err |= cfc_check_trigger_arg_is(&cmd->stop_arg, 0);

	if (err)
		return 3;

	/* step 4: ignored */

	if (err)
		return 4;

	return 0;
}

static int pcl726_intr_cmd(struct comedi_device *dev,
			   struct comedi_subdevice *s)
{
	struct pcl726_private *devpriv = dev->private;

	devpriv->cmd_running = 1;

	return 0;
}

static int pcl726_intr_cancel(struct comedi_device *dev,
			      struct comedi_subdevice *s)
{
	struct pcl726_private *devpriv = dev->private;

	devpriv->cmd_running = 0;

	return 0;
}

static irqreturn_t pcl726_interrupt(int irq, void *d)
{
	struct comedi_device *dev = d;
	struct comedi_subdevice *s = dev->read_subdev;
	struct pcl726_private *devpriv = dev->private;

	if (devpriv->cmd_running) {
		pcl726_intr_cancel(dev, s);

		comedi_buf_put(s->async, 0);
		s->async->events |= (COMEDI_CB_BLOCK | COMEDI_CB_EOS);
		comedi_event(dev, s);
	}

	return IRQ_HANDLED;
}

static int pcl726_ao_insn_write(struct comedi_device *dev,
				struct comedi_subdevice *s,
				struct comedi_insn *insn,
				unsigned int *data)
{
	struct pcl726_private *devpriv = dev->private;
	unsigned int chan = CR_CHAN(insn->chanspec);
	unsigned int range = CR_RANGE(insn->chanspec);
	unsigned int val;
	int i;

	for (i = 0; i < insn->n; i++) {
		val = data[i];
		devpriv->ao_readback[chan] = val;

		/* bipolar data to the DAC is two's complement */
		if (comedi_chan_range_is_bipolar(s, chan, range))
			val = comedi_offset_munge(s, val);

		/* order is important, MSB then LSB */
		outb((val >> 8) & 0xff, dev->iobase + PCL726_AO_MSB_REG(chan));
		outb(val & 0xff, dev->iobase + PCL726_AO_LSB_REG(chan));
	}

	return insn->n;
>>>>>>> refs/remotes/origin/master
}

static int pcl726_ao_insn_read(struct comedi_device *dev,
			       struct comedi_subdevice *s,
<<<<<<< HEAD
			       struct comedi_insn *insn, unsigned int *data)
{
	int chan = CR_CHAN(insn->chanspec);
	int n;

	for (n = 0; n < insn->n; n++)
		data[n] = devpriv->ao_readback[chan];
	return n;
=======
			       struct comedi_insn *insn,
			       unsigned int *data)
{
	struct pcl726_private *devpriv = dev->private;
	unsigned int chan = CR_CHAN(insn->chanspec);
	int i;

	for (i = 0; i < insn->n; i++)
		data[i] = devpriv->ao_readback[chan];

	return insn->n;
>>>>>>> refs/remotes/origin/master
}

static int pcl726_di_insn_bits(struct comedi_device *dev,
			       struct comedi_subdevice *s,
<<<<<<< HEAD
			       struct comedi_insn *insn, unsigned int *data)
{
	if (insn->n != 2)
		return -EINVAL;

	data[1] = inb(dev->iobase + this_board->di_lo) |
	    (inb(dev->iobase + this_board->di_hi) << 8);

	return 2;
=======
			       struct comedi_insn *insn,
			       unsigned int *data)
{
	const struct pcl726_board *board = comedi_board(dev);
	unsigned int val;

	if (board->is_pcl727) {
		val = inb(dev->iobase + PCL727_DI_LSB_REG);
		val |= (inb(dev->iobase + PCL727_DI_MSB_REG) << 8);
	} else {
		val = inb(dev->iobase + PCL726_DI_LSB_REG);
		val |= (inb(dev->iobase + PCL726_DI_MSB_REG) << 8);
	}

	data[1] = val;

	return insn->n;
>>>>>>> refs/remotes/origin/master
}

static int pcl726_do_insn_bits(struct comedi_device *dev,
			       struct comedi_subdevice *s,
<<<<<<< HEAD
			       struct comedi_insn *insn, unsigned int *data)
{
	if (insn->n != 2)
		return -EINVAL;

	if (data[0]) {
		s->state &= ~data[0];
		s->state |= data[0] & data[1];
	}
	if (data[1] & 0x00ff)
		outb(s->state & 0xff, dev->iobase + this_board->do_lo);
	if (data[1] & 0xff00)
		outb((s->state >> 8), dev->iobase + this_board->do_hi);

	data[1] = s->state;

	return 2;
}

static int pcl726_attach(struct comedi_device *dev, struct comedi_devconfig *it)
{
	struct comedi_subdevice *s;
	unsigned long iobase;
	unsigned int iorange;
	int ret, i;
#ifdef ACL6126_IRQ
	unsigned int irq;
#endif

	iobase = it->options[0];
	iorange = this_board->io_range;
	printk(KERN_WARNING "comedi%d: pcl726: board=%s, 0x%03lx ", dev->minor,
	       this_board->name, iobase);
	if (!request_region(iobase, iorange, "pcl726")) {
		printk(KERN_WARNING "I/O port conflict\n");
		return -EIO;
	}

	dev->iobase = iobase;

	dev->board_name = this_board->name;

	ret = alloc_private(dev, sizeof(struct pcl726_private));
	if (ret < 0)
		return -ENOMEM;

	for (i = 0; i < 12; i++) {
		devpriv->bipolar[i] = 0;
		devpriv->rangelist[i] = &range_unknown;
	}

#ifdef ACL6126_IRQ
	irq = 0;
	if (boardtypes[board].IRQbits != 0) {	/* board support IRQ */
		irq = it->options[1];
		devpriv->first_chan = 2;
		if (irq) {	/* we want to use IRQ */
			if (((1 << irq) & boardtypes[board].IRQbits) == 0) {
				printk(KERN_WARNING
					", IRQ %d is out of allowed range,"
					" DISABLING IT", irq);
				irq = 0;	/* Bad IRQ */
			} else {
				if (request_irq(irq, interrupt_pcl818, 0,
						"pcl726", dev)) {
					printk(KERN_WARNING
						", unable to allocate IRQ %d,"
						" DISABLING IT", irq);
					irq = 0;	/* Can't use IRQ */
				} else {
					printk(", irq=%d", irq);
				}
			}
		}
	}

	dev->irq = irq;
#endif

	printk("\n");

	ret = alloc_subdevices(dev, 3);
	if (ret < 0)
		return ret;

	s = dev->subdevices + 0;
	/* ao */
	s->type = COMEDI_SUBD_AO;
	s->subdev_flags = SDF_WRITABLE | SDF_GROUND;
	s->n_chan = this_board->n_aochan;
	s->maxdata = 0xfff;
	s->len_chanlist = 1;
	s->insn_write = pcl726_ao_insn;
	s->insn_read = pcl726_ao_insn_read;
	s->range_table_list = devpriv->rangelist;
	for (i = 0; i < this_board->n_aochan; i++) {
		int j;

		j = it->options[2 + 1];
		if ((j < 0) || (j >= this_board->num_of_ranges)) {
			printk
			    ("Invalid range for channel %d! Must be 0<=%d<%d\n",
			     i, j, this_board->num_of_ranges - 1);
			j = 0;
		}
		devpriv->rangelist[i] = this_board->range_type_list[j];
		if (devpriv->rangelist[i]->range[0].min ==
		    -devpriv->rangelist[i]->range[0].max)
			devpriv->bipolar[i] = 1;	/* bipolar range */
	}

	s = dev->subdevices + 1;
	/* di */
	if (!this_board->have_dio) {
		s->type = COMEDI_SUBD_UNUSED;
	} else {
		s->type = COMEDI_SUBD_DI;
		s->subdev_flags = SDF_READABLE | SDF_GROUND;
		s->n_chan = 16;
		s->maxdata = 1;
		s->len_chanlist = 1;
		s->insn_bits = pcl726_di_insn_bits;
		s->range_table = &range_digital;
	}

	s = dev->subdevices + 2;
	/* do */
	if (!this_board->have_dio) {
		s->type = COMEDI_SUBD_UNUSED;
	} else {
		s->type = COMEDI_SUBD_DO;
		s->subdev_flags = SDF_WRITABLE | SDF_GROUND;
		s->n_chan = 16;
		s->maxdata = 1;
		s->len_chanlist = 1;
		s->insn_bits = pcl726_do_insn_bits;
		s->range_table = &range_digital;
=======
			       struct comedi_insn *insn,
			       unsigned int *data)
{
	const struct pcl726_board *board = comedi_board(dev);
	unsigned long io = dev->iobase;
	unsigned int mask;

	mask = comedi_dio_update_state(s, data);
	if (mask) {
		if (board->is_pcl727) {
			if (mask & 0x00ff)
				outb(s->state & 0xff, io + PCL727_DO_LSB_REG);
			if (mask & 0xff00)
				outb((s->state >> 8), io + PCL727_DO_MSB_REG);
		} else {
			if (mask & 0x00ff)
				outb(s->state & 0xff, io + PCL726_DO_LSB_REG);
			if (mask & 0xff00)
				outb((s->state >> 8), io + PCL726_DO_MSB_REG);
		}
	}

	data[1] = s->state;

	return insn->n;
}

static int pcl726_attach(struct comedi_device *dev,
			 struct comedi_devconfig *it)
{
	const struct pcl726_board *board = comedi_board(dev);
	struct pcl726_private *devpriv;
	struct comedi_subdevice *s;
	int subdev;
	int ret;
	int i;

	ret = comedi_request_region(dev, it->options[0], board->io_len);
	if (ret)
		return ret;

	devpriv = comedi_alloc_devpriv(dev, sizeof(*devpriv));
	if (!devpriv)
		return -ENOMEM;

	/*
	 * Hook up the external trigger source interrupt only if the
	 * user config option is valid and the board supports interrupts.
	 */
	if (it->options[1] && (board->irq_mask & (1 << it->options[1]))) {
		ret = request_irq(it->options[1], pcl726_interrupt, 0,
				  dev->board_name, dev);
		if (ret == 0) {
			/* External trigger source is from Pin-17 of CN3 */
			dev->irq = it->options[1];
		}
	}

	/* setup the per-channel analog output range_table_list */
	for (i = 0; i < 12; i++) {
		unsigned int opt = it->options[2 + i];

		if (opt < board->ao_num_ranges && i < board->ao_nchan)
			devpriv->rangelist[i] = board->ao_ranges[opt];
		else
			devpriv->rangelist[i] = &range_unknown;
	}

	subdev = board->have_dio ? 3 : 1;
	if (dev->irq)
		subdev++;
	ret = comedi_alloc_subdevices(dev, subdev);
	if (ret)
		return ret;

	subdev = 0;

	/* Analog Output subdevice */
	s = &dev->subdevices[subdev++];
	s->type		= COMEDI_SUBD_AO;
	s->subdev_flags	= SDF_WRITABLE | SDF_GROUND;
	s->n_chan	= board->ao_nchan;
	s->maxdata	= 0x0fff;
	s->range_table_list = devpriv->rangelist;
	s->insn_write	= pcl726_ao_insn_write;
	s->insn_read	= pcl726_ao_insn_read;

	if (board->have_dio) {
		/* Digital Input subdevice */
		s = &dev->subdevices[subdev++];
		s->type		= COMEDI_SUBD_DI;
		s->subdev_flags	= SDF_READABLE;
		s->n_chan	= 16;
		s->maxdata	= 1;
		s->insn_bits	= pcl726_di_insn_bits;
		s->range_table	= &range_digital;

		/* Digital Output subdevice */
		s = &dev->subdevices[subdev++];
		s->type		= COMEDI_SUBD_DO;
		s->subdev_flags	= SDF_WRITABLE;
		s->n_chan	= 16;
		s->maxdata	= 1;
		s->insn_bits	= pcl726_do_insn_bits;
		s->range_table	= &range_digital;
	}

	if (dev->irq) {
		/* Digial Input subdevice - Interrupt support */
		s = &dev->subdevices[subdev++];
		dev->read_subdev = s;
		s->type		= COMEDI_SUBD_DI;
		s->subdev_flags	= SDF_READABLE | SDF_CMD_READ;
		s->n_chan	= 1;
		s->maxdata	= 1;
		s->range_table	= &range_digital;
		s->insn_bits	= pcl726_intr_insn_bits;
		s->do_cmdtest	= pcl726_intr_cmdtest;
		s->do_cmd	= pcl726_intr_cmd;
		s->cancel	= pcl726_intr_cancel;
>>>>>>> refs/remotes/origin/master
	}

	return 0;
}

<<<<<<< HEAD
static int pcl726_detach(struct comedi_device *dev)
{
/* printk("comedi%d: pcl726: remove\n",dev->minor); */

#ifdef ACL6126_IRQ
	if (dev->irq)
		free_irq(dev->irq, dev);
#endif

	if (dev->iobase)
		release_region(dev->iobase, this_board->io_range);

	return 0;
}

MODULE_AUTHOR("Comedi http://www.comedi.org");
MODULE_DESCRIPTION("Comedi low-level driver");
=======
static struct comedi_driver pcl726_driver = {
	.driver_name	= "pcl726",
	.module		= THIS_MODULE,
	.attach		= pcl726_attach,
	.detach		= comedi_legacy_detach,
	.board_name	= &pcl726_boards[0].name,
	.num_names	= ARRAY_SIZE(pcl726_boards),
	.offset		= sizeof(struct pcl726_board),
};
module_comedi_driver(pcl726_driver);

MODULE_AUTHOR("Comedi http://www.comedi.org");
MODULE_DESCRIPTION("Comedi driver for Advantech PCL-726 & compatibles");
>>>>>>> refs/remotes/origin/master
MODULE_LICENSE("GPL");
