/*
 *     comedi/drivers/ni_daq_700.c
<<<<<<< HEAD
 *     Driver for DAQCard-700 DIO only
=======
 *     Driver for DAQCard-700 DIO/AI
>>>>>>> refs/remotes/origin/master
 *     copied from 8255
 *
 *     COMEDI - Linux Control and Measurement Device Interface
 *     Copyright (C) 1998 David A. Schleef <ds@schleef.org>
 *
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
<<<<<<< HEAD
 *
 *     You should have received a copy of the GNU General Public License
 *     along with this program; if not, write to the Free Software
 *     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
=======
>>>>>>> refs/remotes/origin/master
 */

/*
Driver: ni_daq_700
Description: National Instruments PCMCIA DAQCard-700 DIO only
Author: Fred Brooks <nsaspook@nsaspook.com>,
  based on ni_daq_dio24 by Daniel Vecino Castel <dvecino@able.es>
Devices: [National Instruments] PCMCIA DAQ-Card-700 (ni_daq_700)
Status: works
<<<<<<< HEAD
Updated: Thu, 21 Feb 2008 12:07:20 +0000

The daqcard-700 appears in Comedi as a single digital I/O subdevice with
16 channels.  The channel 0 corresponds to the daqcard-700's output
port, bit 0; channel 8 corresponds to the input port, bit 0.

Direction configuration: channels 0-7 output, 8-15 input (8225 device
emu as port A output, port B input, port C N/A).

IRQ is assigned but not used.
*/

#include <linux/interrupt.h>
#include <linux/slab.h>
#include "../comedidev.h"

#include <linux/ioport.h>

#include <pcmcia/cistpl.h>
#include <pcmcia/cisreg.h>
#include <pcmcia/ds.h>

<<<<<<< HEAD
static struct pcmcia_device *pcmcia_cur_dev = NULL;
=======
static struct pcmcia_device *pcmcia_cur_dev;
>>>>>>> refs/remotes/origin/cm-10.0

#define DIO700_SIZE 8		/*  size of io region used by board */

static int dio700_attach(struct comedi_device *dev,
			 struct comedi_devconfig *it);
static int dio700_detach(struct comedi_device *dev);

enum dio700_bustype { pcmcia_bustype };

struct dio700_board {
	const char *name;
	int device_id;		/*  device id for pcmcia board */
	enum dio700_bustype bustype;	/*  PCMCIA */
	int have_dio;		/*  have daqcard-700 dio */
	/*  function pointers so we can use inb/outb or readb/writeb */
	/*  as appropriate */
	unsigned int (*read_byte) (unsigned int address);
	void (*write_byte) (unsigned int byte, unsigned int address);
};

static const struct dio700_board dio700_boards[] = {
	{
	 .name = "daqcard-700",
	  /*  0x10b is manufacturer id, 0x4743 is device id */
	 .device_id = 0x4743,
	 .bustype = pcmcia_bustype,
	 .have_dio = 1,
	 },
	{
	 .name = "ni_daq_700",
	  /*  0x10b is manufacturer id, 0x4743 is device id */
	 .device_id = 0x4743,
	 .bustype = pcmcia_bustype,
	 .have_dio = 1,
	 },
};

/*
 * Useful for shorthand access to the particular board structure
 */
#define thisboard ((const struct dio700_board *)dev->board_ptr)

struct dio700_private {

	int data;		/* number of data points left to be taken */
};

#define devpriv ((struct dio700_private *)dev->private)

static struct comedi_driver driver_dio700 = {
	.driver_name = "ni_daq_700",
	.module = THIS_MODULE,
	.attach = dio700_attach,
	.detach = dio700_detach,
	.num_names = ARRAY_SIZE(dio700_boards),
	.board_name = &dio700_boards[0].name,
	.offset = sizeof(struct dio700_board),
};

/*	the real driver routines	*/

#define _700_SIZE 8

#define _700_DATA 0

#define DIO_W		0x04
#define DIO_R		0x05

struct subdev_700_struct {
	unsigned long cb_arg;
	int (*cb_func) (int, int, int, unsigned long);
	int have_irq;
};

#define CALLBACK_ARG	(((struct subdev_700_struct *)s->private)->cb_arg)
#define CALLBACK_FUNC	(((struct subdev_700_struct *)s->private)->cb_func)
#define subdevpriv	((struct subdev_700_struct *)s->private)

static void do_config(struct comedi_device *dev, struct comedi_subdevice *s);

void subdev_700_interrupt(struct comedi_device *dev, struct comedi_subdevice *s)
{
	short d;

	d = CALLBACK_FUNC(0, _700_DATA, 0, CALLBACK_ARG);

	comedi_buf_put(s->async, d);
	s->async->events |= COMEDI_CB_EOS;

	comedi_event(dev, s);
}
EXPORT_SYMBOL(subdev_700_interrupt);

static int subdev_700_cb(int dir, int port, int data, unsigned long arg)
{
	/* port is always A for output and B for input (8255 emu) */
	unsigned long iobase = arg;

	if (dir) {
		outb(data, iobase + DIO_W);
		return 0;
	} else {
		return inb(iobase + DIO_R);
	}
}

static int subdev_700_insn(struct comedi_device *dev,
			   struct comedi_subdevice *s, struct comedi_insn *insn,
			   unsigned int *data)
{
	if (data[0]) {
		s->state &= ~data[0];
		s->state |= (data[0] & data[1]);

		if (data[0] & 0xff)
			CALLBACK_FUNC(1, _700_DATA, s->state & 0xff,
				      CALLBACK_ARG);
	}

	data[1] = s->state & 0xff;
	data[1] |= CALLBACK_FUNC(0, _700_DATA, 0, CALLBACK_ARG) << 8;

	return 2;
}

static int subdev_700_insn_config(struct comedi_device *dev,
				  struct comedi_subdevice *s,
				  struct comedi_insn *insn, unsigned int *data)
{

	switch (data[0]) {
	case INSN_CONFIG_DIO_INPUT:
		break;
	case INSN_CONFIG_DIO_OUTPUT:
		break;
	case INSN_CONFIG_DIO_QUERY:
		data[1] =
		    (s->
		     io_bits & (1 << CR_CHAN(insn->chanspec))) ? COMEDI_OUTPUT :
		    COMEDI_INPUT;
		return insn->n;
		break;
	default:
		return -EINVAL;
	}

	return 1;
}

static void do_config(struct comedi_device *dev, struct comedi_subdevice *s)
{				/* use powerup defaults */
	return;
}

static int subdev_700_cmdtest(struct comedi_device *dev,
			      struct comedi_subdevice *s,
			      struct comedi_cmd *cmd)
{
	int err = 0;
	unsigned int tmp;

	/* step 1 */

	tmp = cmd->start_src;
	cmd->start_src &= TRIG_NOW;
	if (!cmd->start_src || tmp != cmd->start_src)
		err++;

	tmp = cmd->scan_begin_src;
	cmd->scan_begin_src &= TRIG_EXT;
	if (!cmd->scan_begin_src || tmp != cmd->scan_begin_src)
		err++;

	tmp = cmd->convert_src;
	cmd->convert_src &= TRIG_FOLLOW;
	if (!cmd->convert_src || tmp != cmd->convert_src)
		err++;

	tmp = cmd->scan_end_src;
	cmd->scan_end_src &= TRIG_COUNT;
	if (!cmd->scan_end_src || tmp != cmd->scan_end_src)
		err++;

	tmp = cmd->stop_src;
	cmd->stop_src &= TRIG_NONE;
	if (!cmd->stop_src || tmp != cmd->stop_src)
		err++;

	if (err)
		return 1;

	/* step 2 */

	if (err)
		return 2;

	/* step 3 */

	if (cmd->start_arg != 0) {
		cmd->start_arg = 0;
		err++;
	}
	if (cmd->scan_begin_arg != 0) {
		cmd->scan_begin_arg = 0;
		err++;
	}
	if (cmd->convert_arg != 0) {
		cmd->convert_arg = 0;
		err++;
	}
	if (cmd->scan_end_arg != 1) {
		cmd->scan_end_arg = 1;
		err++;
	}
	if (cmd->stop_arg != 0) {
		cmd->stop_arg = 0;
		err++;
	}

	if (err)
		return 3;

	/* step 4 */

	if (err)
		return 4;

	return 0;
}

static int subdev_700_cmd(struct comedi_device *dev, struct comedi_subdevice *s)
{
	/* FIXME */

	return 0;
}

static int subdev_700_cancel(struct comedi_device *dev,
			     struct comedi_subdevice *s)
{
	/* FIXME */

	return 0;
}

int subdev_700_init(struct comedi_device *dev, struct comedi_subdevice *s,
		    int (*cb) (int, int, int, unsigned long), unsigned long arg)
{
	s->type = COMEDI_SUBD_DIO;
	s->subdev_flags = SDF_READABLE | SDF_WRITABLE;
	s->n_chan = 16;
	s->range_table = &range_digital;
	s->maxdata = 1;

	s->private = kmalloc(sizeof(struct subdev_700_struct), GFP_KERNEL);
	if (!s->private)
		return -ENOMEM;

	CALLBACK_ARG = arg;
	if (cb == NULL)
		CALLBACK_FUNC = subdev_700_cb;
	 else
		CALLBACK_FUNC = cb;

	s->insn_bits = subdev_700_insn;
	s->insn_config = subdev_700_insn_config;

	s->state = 0;
	s->io_bits = 0x00ff;
	do_config(dev, s);

	return 0;
}
EXPORT_SYMBOL(subdev_700_init);

int subdev_700_init_irq(struct comedi_device *dev, struct comedi_subdevice *s,
			int (*cb) (int, int, int, unsigned long),
			unsigned long arg)
{
	int ret;

	ret = subdev_700_init(dev, s, cb, arg);
	if (ret < 0)
		return ret;

	s->do_cmdtest = subdev_700_cmdtest;
	s->do_cmd = subdev_700_cmd;
	s->cancel = subdev_700_cancel;

	subdevpriv->have_irq = 1;

	return 0;
}
EXPORT_SYMBOL(subdev_700_init_irq);

void subdev_700_cleanup(struct comedi_device *dev, struct comedi_subdevice *s)
{
	if (s->private)
		if (subdevpriv->have_irq)

			kfree(s->private);
}
EXPORT_SYMBOL(subdev_700_cleanup);

static int dio700_attach(struct comedi_device *dev, struct comedi_devconfig *it)
{
	struct comedi_subdevice *s;
	unsigned long iobase = 0;
#ifdef incomplete
	unsigned int irq = 0;
#endif
	struct pcmcia_device *link;

	/* allocate and initialize dev->private */
	if (alloc_private(dev, sizeof(struct dio700_private)) < 0)
		return -ENOMEM;

	/*  get base address, irq etc. based on bustype */
	switch (thisboard->bustype) {
	case pcmcia_bustype:
		link = pcmcia_cur_dev;	/* XXX hack */
		if (!link)
			return -EIO;
		iobase = link->resource[0]->start;
#ifdef incomplete
		irq = link->irq;
#endif
		break;
	default:
<<<<<<< HEAD
		printk("bug! couldn't determine board type\n");
		return -EINVAL;
		break;
	}
	printk("comedi%d: ni_daq_700: %s, io 0x%lx", dev->minor,
=======
		printk(KERN_ERR "bug! couldn't determine board type\n");
		return -EINVAL;
		break;
	}
	printk(KERN_ERR "comedi%d: ni_daq_700: %s, io 0x%lx", dev->minor,
>>>>>>> refs/remotes/origin/cm-10.0
	       thisboard->name, iobase);
#ifdef incomplete
	if (irq)
		printk(", irq %u", irq);

#endif

	printk("\n");

	if (iobase == 0) {
<<<<<<< HEAD
		printk("io base address is zero!\n");
=======
		printk(KERN_ERR "io base address is zero!\n");
>>>>>>> refs/remotes/origin/cm-10.0
		return -EINVAL;
	}

	dev->iobase = iobase;

#ifdef incomplete
	/* grab our IRQ */
	dev->irq = irq;
#endif

	dev->board_name = thisboard->name;

	if (alloc_subdevices(dev, 1) < 0)
		return -ENOMEM;

	/* DAQCard-700 dio */
	s = dev->subdevices + 0;
	subdev_700_init(dev, s, NULL, dev->iobase);

	return 0;
};

static int dio700_detach(struct comedi_device *dev)
{
<<<<<<< HEAD
	printk("comedi%d: ni_daq_700: cs-remove\n", dev->minor);
=======
	printk(KERN_ERR "comedi%d: ni_daq_700: cs-remove\n", dev->minor);
>>>>>>> refs/remotes/origin/cm-10.0

	if (dev->subdevices)
		subdev_700_cleanup(dev, dev->subdevices + 0);

	if (thisboard->bustype != pcmcia_bustype && dev->iobase)
		release_region(dev->iobase, DIO700_SIZE);
	if (dev->irq)
		free_irq(dev->irq, dev);

	return 0;
};

static void dio700_config(struct pcmcia_device *link);
static void dio700_release(struct pcmcia_device *link);
static int dio700_cs_suspend(struct pcmcia_device *p_dev);
static int dio700_cs_resume(struct pcmcia_device *p_dev);

static int dio700_cs_attach(struct pcmcia_device *);
static void dio700_cs_detach(struct pcmcia_device *);

struct local_info_t {
	struct pcmcia_device *link;
	int stop;
	struct bus_operations *bus;
};

static int dio700_cs_attach(struct pcmcia_device *link)
{
	struct local_info_t *local;

	printk(KERN_INFO "ni_daq_700:  cs-attach\n");

	dev_dbg(&link->dev, "dio700_cs_attach()\n");

	/* Allocate space for private device-specific data */
	local = kzalloc(sizeof(struct local_info_t), GFP_KERNEL);
	if (!local)
		return -ENOMEM;
	local->link = link;
	link->priv = local;

	pcmcia_cur_dev = link;

	dio700_config(link);

	return 0;
}				/* dio700_cs_attach */

static void dio700_cs_detach(struct pcmcia_device *link)
{

	printk(KERN_INFO "ni_daq_700: cs-detach!\n");

	dev_dbg(&link->dev, "dio700_cs_detach\n");

	((struct local_info_t *)link->priv)->stop = 1;
	dio700_release(link);

	/* This points to the parent struct local_info_t struct */
	kfree(link->priv);

}				/* dio700_cs_detach */

static int dio700_pcmcia_config_loop(struct pcmcia_device *p_dev,
				void *priv_data)
{
	if (p_dev->config_index == 0)
		return -EINVAL;

	return pcmcia_request_io(p_dev);
}

static void dio700_config(struct pcmcia_device *link)
{
	int ret;

	printk(KERN_INFO "ni_daq_700:  cs-config\n");

	dev_dbg(&link->dev, "dio700_config\n");

	link->config_flags |= CONF_ENABLE_IRQ | CONF_AUTO_AUDIO |
		CONF_AUTO_SET_IO;

	ret = pcmcia_loop_config(link, dio700_pcmcia_config_loop, NULL);
	if (ret) {
		dev_warn(&link->dev, "no configuration found\n");
		goto failed;
	}

	if (!link->irq)
		goto failed;

	ret = pcmcia_enable_device(link);
	if (ret != 0)
		goto failed;

	return;

failed:
	printk(KERN_INFO "ni_daq_700 cs failed");
	dio700_release(link);

}				/* dio700_config */

static void dio700_release(struct pcmcia_device *link)
{
	dev_dbg(&link->dev, "dio700_release\n");

	pcmcia_disable_device(link);
}				/* dio700_release */

static int dio700_cs_suspend(struct pcmcia_device *link)
{
	struct local_info_t *local = link->priv;

	/* Mark the device as stopped, to block IO until later */
	local->stop = 1;
	return 0;
}				/* dio700_cs_suspend */

static int dio700_cs_resume(struct pcmcia_device *link)
{
	struct local_info_t *local = link->priv;

	local->stop = 0;
	return 0;
}				/* dio700_cs_resume */

/*====================================================================*/

static const struct pcmcia_device_id dio700_cs_ids[] = {
	/* N.B. These IDs should match those in dio700_boards */
	PCMCIA_DEVICE_MANF_CARD(0x010b, 0x4743),	/* daqcard-700 */
	PCMCIA_DEVICE_NULL
};


MODULE_DEVICE_TABLE(pcmcia, dio700_cs_ids);
MODULE_AUTHOR("Fred Brooks <nsaspook@nsaspook.com>");
MODULE_DESCRIPTION("Comedi driver for National Instruments "
		   "PCMCIA DAQCard-700 DIO");
MODULE_LICENSE("GPL");

struct pcmcia_driver dio700_cs_driver = {
	.probe = dio700_cs_attach,
	.remove = dio700_cs_detach,
	.suspend = dio700_cs_suspend,
	.resume = dio700_cs_resume,
	.id_table = dio700_cs_ids,
	.owner = THIS_MODULE,
	.name = "ni_daq_700",
};

static int __init init_dio700_cs(void)
{
	pcmcia_register_driver(&dio700_cs_driver);
	return 0;
}

static void __exit exit_dio700_cs(void)
{
	pr_debug("ni_daq_700: unloading\n");
	pcmcia_unregister_driver(&dio700_cs_driver);
}

int __init init_module(void)
{
	int ret;

	ret = init_dio700_cs();
	if (ret < 0)
		return ret;

	return comedi_driver_register(&driver_dio700);
}

void __exit cleanup_module(void)
{
	exit_dio700_cs();
	comedi_driver_unregister(&driver_dio700);
}
=======
Updated: Wed, 19 Sep 2012 12:07:20 +0000

The daqcard-700 appears in Comedi as a  digital I/O subdevice (0) with
16 channels and a analog input subdevice (1) with 16 single-ended channels.

Digital:  The channel 0 corresponds to the daqcard-700's output
port, bit 0; channel 8 corresponds to the input port, bit 0.

Digital direction configuration: channels 0-7 output, 8-15 input (8225 device
emu as port A output, port B input, port C N/A).

Analog: The input  range is 0 to 4095 for -10 to +10 volts
IRQ is assigned but not used.

Version 0.1	Original DIO only driver
Version 0.2	DIO and basic AI analog input support on 16 se channels

Manuals:	Register level:	http://www.ni.com/pdf/manuals/340698.pdf
		User Manual:	http://www.ni.com/pdf/manuals/320676d.pdf
*/

#include <linux/module.h>
#include <linux/delay.h>
#include <linux/interrupt.h>

#include "../comedidev.h"

#include <pcmcia/cistpl.h>
#include <pcmcia/ds.h>

/* daqcard700 registers */
#define DIO_W		0x04	/* WO 8bit */
#define DIO_R		0x05	/* RO 8bit */
#define CMD_R1		0x00	/* WO 8bit */
#define CMD_R2		0x07	/* RW 8bit */
#define CMD_R3		0x05	/* W0 8bit */
#define STA_R1		0x00	/* RO 8bit */
#define STA_R2		0x01	/* RO 8bit */
#define ADFIFO_R	0x02	/* RO 16bit */
#define ADCLEAR_R	0x01	/* WO 8bit */
#define CDA_R0		0x08	/* RW 8bit */
#define CDA_R1		0x09	/* RW 8bit */
#define CDA_R2		0x0A	/* RW 8bit */
#define CMO_R		0x0B	/* RO 8bit */
#define TIC_R		0x06	/* WO 8bit */

static int daq700_dio_insn_bits(struct comedi_device *dev,
				struct comedi_subdevice *s,
				struct comedi_insn *insn,
				unsigned int *data)
{
	unsigned int mask;
	unsigned int val;

	mask = comedi_dio_update_state(s, data);
	if (mask) {
		if (mask & 0xff)
			outb(s->state & 0xff, dev->iobase + DIO_W);
	}

	val = s->state & 0xff;
	val |= inb(dev->iobase + DIO_R) << 8;

	data[1] = val;

	return insn->n;
}

static int daq700_dio_insn_config(struct comedi_device *dev,
				  struct comedi_subdevice *s,
				  struct comedi_insn *insn,
				  unsigned int *data)
{
	int ret;

	ret = comedi_dio_insn_config(dev, s, insn, data, 0);
	if (ret)
		return ret;

	/* The DIO channels are not configurable, fix the io_bits */
	s->io_bits = 0x00ff;

	return insn->n;
}

static int daq700_ai_rinsn(struct comedi_device *dev,
			   struct comedi_subdevice *s,
			   struct comedi_insn *insn, unsigned int *data)
{
	int n, i, chan;
	int d;
	unsigned int status;
	enum { TIMEOUT = 100 };

	chan = CR_CHAN(insn->chanspec);
	/* write channel to multiplexer */
	/* set mask scan bit high to disable scanning */
	outb(chan | 0x80, dev->iobase + CMD_R1);

	/* convert n samples */
	for (n = 0; n < insn->n; n++) {
		/* trigger conversion with out0 L to H */
		outb(0x00, dev->iobase + CMD_R2); /* enable ADC conversions */
		outb(0x30, dev->iobase + CMO_R); /* mode 0 out0 L, from H */
		/* mode 1 out0 H, L to H, start conversion */
		outb(0x32, dev->iobase + CMO_R);
		/* wait for conversion to end */
		for (i = 0; i < TIMEOUT; i++) {
			status = inb(dev->iobase + STA_R2);
			if ((status & 0x03) != 0) {
				dev_info(dev->class_dev,
					 "Overflow/run Error\n");
				return -EOVERFLOW;
			}
			status = inb(dev->iobase + STA_R1);
			if ((status & 0x02) != 0) {
				dev_info(dev->class_dev, "Data Error\n");
				return -ENODATA;
			}
			if ((status & 0x11) == 0x01) {
				/* ADC conversion complete */
				break;
			}
			udelay(1);
		}
		if (i == TIMEOUT) {
			dev_info(dev->class_dev,
				 "timeout during ADC conversion\n");
			return -ETIMEDOUT;
		}
		/* read data */
		d = inw(dev->iobase + ADFIFO_R);
		/* mangle the data as necessary */
		/* Bipolar Offset Binary: 0 to 4095 for -10 to +10 */
		d &= 0x0fff;
		d ^= 0x0800;
		data[n] = d;
	}
	return n;
}

/*
 * Data acquisition is enabled.
 * The counter 0 output is high.
 * The I/O connector pin CLK1 drives counter 1 source.
 * Multiple-channel scanning is disabled.
 * All interrupts are disabled.
 * The analog input range is set to +-10 V
 * The analog input mode is single-ended.
 * The analog input circuitry is initialized to channel 0.
 * The A/D FIFO is cleared.
 */
static void daq700_ai_config(struct comedi_device *dev,
			     struct comedi_subdevice *s)
{
	unsigned long iobase = dev->iobase;

	outb(0x80, iobase + CMD_R1);	/* disable scanning, ADC to chan 0 */
	outb(0x00, iobase + CMD_R2);	/* clear all bits */
	outb(0x00, iobase + CMD_R3);	/* set +-10 range */
	outb(0x32, iobase + CMO_R);	/* config counter mode1, out0 to H */
	outb(0x00, iobase + TIC_R);	/* clear counter interrupt */
	outb(0x00, iobase + ADCLEAR_R);	/* clear the ADC FIFO */
	inw(iobase + ADFIFO_R);		/* read 16bit junk from FIFO to clear */
}

static int daq700_auto_attach(struct comedi_device *dev,
			      unsigned long context)
{
	struct pcmcia_device *link = comedi_to_pcmcia_dev(dev);
	struct comedi_subdevice *s;
	int ret;

	link->config_flags |= CONF_AUTO_SET_IO;
	ret = comedi_pcmcia_enable(dev, NULL);
	if (ret)
		return ret;
	dev->iobase = link->resource[0]->start;

	ret = comedi_alloc_subdevices(dev, 2);
	if (ret)
		return ret;

	/* DAQCard-700 dio */
	s = &dev->subdevices[0];
	s->type		= COMEDI_SUBD_DIO;
	s->subdev_flags	= SDF_READABLE | SDF_WRITABLE;
	s->n_chan	= 16;
	s->range_table	= &range_digital;
	s->maxdata	= 1;
	s->insn_bits	= daq700_dio_insn_bits;
	s->insn_config	= daq700_dio_insn_config;
	s->io_bits	= 0x00ff;

	/* DAQCard-700 ai */
	s = &dev->subdevices[1];
	s->type = COMEDI_SUBD_AI;
	/* we support single-ended (ground)  */
	s->subdev_flags = SDF_READABLE | SDF_GROUND;
	s->n_chan = 16;
	s->maxdata = (1 << 12) - 1;
	s->range_table = &range_bipolar10;
	s->insn_read = daq700_ai_rinsn;
	daq700_ai_config(dev, s);

	dev_info(dev->class_dev, "%s: %s, io 0x%lx\n",
		dev->driver->driver_name,
		dev->board_name,
		dev->iobase);

	return 0;
}

static struct comedi_driver daq700_driver = {
	.driver_name	= "ni_daq_700",
	.module		= THIS_MODULE,
	.auto_attach	= daq700_auto_attach,
	.detach		= comedi_pcmcia_disable,
};

static int daq700_cs_attach(struct pcmcia_device *link)
{
	return comedi_pcmcia_auto_config(link, &daq700_driver);
}

static const struct pcmcia_device_id daq700_cs_ids[] = {
	PCMCIA_DEVICE_MANF_CARD(0x010b, 0x4743),
	PCMCIA_DEVICE_NULL
};
MODULE_DEVICE_TABLE(pcmcia, daq700_cs_ids);

static struct pcmcia_driver daq700_cs_driver = {
	.name		= "ni_daq_700",
	.owner		= THIS_MODULE,
	.id_table	= daq700_cs_ids,
	.probe		= daq700_cs_attach,
	.remove		= comedi_pcmcia_auto_unconfig,
};
module_comedi_pcmcia_driver(daq700_driver, daq700_cs_driver);

MODULE_AUTHOR("Fred Brooks <nsaspook@nsaspook.com>");
MODULE_DESCRIPTION(
	"Comedi driver for National Instruments PCMCIA DAQCard-700 DIO/AI");
MODULE_VERSION("0.2.00");
MODULE_LICENSE("GPL");
>>>>>>> refs/remotes/origin/master
