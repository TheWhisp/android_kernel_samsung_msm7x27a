/*
<<<<<<< HEAD
    comedi/drivers/pcmad.c
    Hardware driver for Winsystems PCM-A/D12 and PCM-A/D16

    COMEDI - Linux Control and Measurement Device Interface
    Copyright (C) 2000,2001 David A. Schleef <ds@schleef.org>

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
Driver: pcmad
Description: Winsystems PCM-A/D12, PCM-A/D16
Author: ds
Devices: [Winsystems] PCM-A/D12 (pcmad12), PCM-A/D16 (pcmad16)
Status: untested

This driver was written on a bet that I couldn't write a driver
in less than 2 hours.  I won the bet, but never got paid.  =(

Configuration options:
  [0] - I/O port base
  [1] - unused
  [2] - Analog input reference
	0 = single ended
	1 = differential
  [3] - Analog input encoding (must match jumpers)
	0 = straight binary
	1 = two's complement
*/

#include <linux/interrupt.h>
#include "../comedidev.h"

#include <linux/ioport.h>

#define PCMAD_SIZE		4
=======
 * pcmad.c
 * Hardware driver for Winsystems PCM-A/D12 and PCM-A/D16
 *
 * COMEDI - Linux Control and Measurement Device Interface
 * Copyright (C) 2000,2001 David A. Schleef <ds@schleef.org>
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
 * Driver: pcmad
 * Description: Winsystems PCM-A/D12, PCM-A/D16
 * Devices: (Winsystems) PCM-A/D12 [pcmad12]
 *	    (Winsystems) PCM-A/D16 [pcmad16]
 * Author: ds
 * Status: untested
 *
 * This driver was written on a bet that I couldn't write a driver
 * in less than 2 hours.  I won the bet, but never got paid.  =(
 *
 * Configuration options:
 *   [0] - I/O port base
 *   [1] - IRQ (unused)
 *   [2] - Analog input reference (must match jumpers)
 *	   0 = single-ended (16 channels)
 *	   1 = differential (8 channels)
 *   [3] - Analog input encoding (must match jumpers)
 *	   0 = straight binary (0-5V input range)
 *	   1 = two's complement (+-10V input range)
 */

#include <linux/module.h>
#include "../comedidev.h"
>>>>>>> refs/remotes/origin/master

#define PCMAD_STATUS		0
#define PCMAD_LSB		1
#define PCMAD_MSB		2
#define PCMAD_CONVERT		1

struct pcmad_board_struct {
	const char *name;
<<<<<<< HEAD
	int n_ai_bits;
};
static const struct pcmad_board_struct pcmad_boards[] = {
	{
	 .name = "pcmad12",
	 .n_ai_bits = 12,
	 },
	{
	 .name = "pcmad16",
	 .n_ai_bits = 16,
	 },
};

#define this_board ((const struct pcmad_board_struct *)(dev->board_ptr))
#define n_pcmad_boards ARRAY_SIZE(pcmad_boards)

struct pcmad_priv_struct {
	int differential;
	int twos_comp;
};
#define devpriv ((struct pcmad_priv_struct *)dev->private)

static int pcmad_attach(struct comedi_device *dev, struct comedi_devconfig *it);
static int pcmad_detach(struct comedi_device *dev);
static struct comedi_driver driver_pcmad = {
	.driver_name = "pcmad",
	.module = THIS_MODULE,
	.attach = pcmad_attach,
	.detach = pcmad_detach,
	.board_name = &pcmad_boards[0].name,
	.num_names = n_pcmad_boards,
	.offset = sizeof(pcmad_boards[0]),
};

static int __init driver_pcmad_init_module(void)
{
	return comedi_driver_register(&driver_pcmad);
}

static void __exit driver_pcmad_cleanup_module(void)
{
	comedi_driver_unregister(&driver_pcmad);
}

module_init(driver_pcmad_init_module);
module_exit(driver_pcmad_cleanup_module);

#define TIMEOUT	100

static int pcmad_ai_insn_read(struct comedi_device *dev,
			      struct comedi_subdevice *s,
			      struct comedi_insn *insn, unsigned int *data)
{
	int i;
	int chan;
	int n;

	chan = CR_CHAN(insn->chanspec);

	for (n = 0; n < insn->n; n++) {
		outb(chan, dev->iobase + PCMAD_CONVERT);

		for (i = 0; i < TIMEOUT; i++) {
			if ((inb(dev->iobase + PCMAD_STATUS) & 0x3) == 0x3)
				break;
		}
		data[n] = inb(dev->iobase + PCMAD_LSB);
		data[n] |= (inb(dev->iobase + PCMAD_MSB) << 8);

		if (devpriv->twos_comp)
			data[n] ^= (1 << (this_board->n_ai_bits - 1));
	}

	return n;
}

/*
 * options:
 * 0	i/o base
 * 1	unused
 * 2	0=single ended 1=differential
 * 3	0=straight binary 1=two's comp
 */
static int pcmad_attach(struct comedi_device *dev, struct comedi_devconfig *it)
{
	int ret;
	struct comedi_subdevice *s;
	unsigned long iobase;

	iobase = it->options[0];
	printk(KERN_INFO "comedi%d: pcmad: 0x%04lx ", dev->minor, iobase);
	if (!request_region(iobase, PCMAD_SIZE, "pcmad")) {
		printk(KERN_CONT "I/O port conflict\n");
		return -EIO;
	}
	printk(KERN_CONT "\n");
	dev->iobase = iobase;

	ret = alloc_subdevices(dev, 1);
	if (ret < 0)
		return ret;

	ret = alloc_private(dev, sizeof(struct pcmad_priv_struct));
	if (ret < 0)
		return ret;

	dev->board_name = this_board->name;

	s = dev->subdevices + 0;
	s->type = COMEDI_SUBD_AI;
	s->subdev_flags = SDF_READABLE | AREF_GROUND;
	s->n_chan = 16;		/* XXX */
	s->len_chanlist = 1;
	s->insn_read = pcmad_ai_insn_read;
	s->maxdata = (1 << this_board->n_ai_bits) - 1;
	s->range_table = &range_unknown;
=======
	unsigned int ai_maxdata;
};

static const struct pcmad_board_struct pcmad_boards[] = {
	{
		.name		= "pcmad12",
		.ai_maxdata	= 0x0fff,
	}, {
		.name		= "pcmad16",
		.ai_maxdata	= 0xffff,
	},
};

#define TIMEOUT	100

static int pcmad_ai_wait_for_eoc(struct comedi_device *dev,
				 int timeout)
{
	int i;

	for (i = 0; i < timeout; i++) {
		if ((inb(dev->iobase + PCMAD_STATUS) & 0x3) == 0x3)
			return 0;
	}
	return -ETIME;
}

static int pcmad_ai_insn_read(struct comedi_device *dev,
			      struct comedi_subdevice *s,
			      struct comedi_insn *insn,
			      unsigned int *data)
{
	unsigned int chan = CR_CHAN(insn->chanspec);
	unsigned int range = CR_RANGE(insn->chanspec);
	unsigned int val;
	int ret;
	int i;

	for (i = 0; i < insn->n; i++) {
		outb(chan, dev->iobase + PCMAD_CONVERT);

		ret = pcmad_ai_wait_for_eoc(dev, TIMEOUT);
		if (ret)
			return ret;

		val = inb(dev->iobase + PCMAD_LSB) |
		      (inb(dev->iobase + PCMAD_MSB) << 8);

		/* data is shifted on the pcmad12, fix it */
		if (s->maxdata == 0x0fff)
			val >>= 4;

		if (comedi_range_is_bipolar(s, range)) {
			/* munge the two's complement value */
			val ^= ((s->maxdata + 1) >> 1);
		}

		data[i] = val;
	}

	return insn->n;
}

static int pcmad_attach(struct comedi_device *dev, struct comedi_devconfig *it)
{
	const struct pcmad_board_struct *board = comedi_board(dev);
	struct comedi_subdevice *s;
	int ret;

	ret = comedi_request_region(dev, it->options[0], 0x04);
	if (ret)
		return ret;

	ret = comedi_alloc_subdevices(dev, 1);
	if (ret)
		return ret;

	s = &dev->subdevices[0];
	s->type		= COMEDI_SUBD_AI;
	if (it->options[1]) {
		/* 8 differential channels */
		s->subdev_flags	= SDF_READABLE | AREF_DIFF;
		s->n_chan	= 8;
	} else {
		/* 16 single-ended channels */
		s->subdev_flags	= SDF_READABLE | AREF_GROUND;
		s->n_chan	= 16;
	}
	s->len_chanlist	= 1;
	s->maxdata	= board->ai_maxdata;
	s->range_table	= it->options[2] ? &range_bipolar10 : &range_unipolar5;
	s->insn_read	= pcmad_ai_insn_read;
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
static int pcmad_detach(struct comedi_device *dev)
{
	printk(KERN_INFO "comedi%d: pcmad: remove\n", dev->minor);

	if (dev->irq)
		free_irq(dev->irq, dev);

	if (dev->iobase)
		release_region(dev->iobase, PCMAD_SIZE);

	return 0;
}
=======
static struct comedi_driver pcmad_driver = {
	.driver_name	= "pcmad",
	.module		= THIS_MODULE,
	.attach		= pcmad_attach,
	.detach		= comedi_legacy_detach,
	.board_name	= &pcmad_boards[0].name,
	.num_names	= ARRAY_SIZE(pcmad_boards),
	.offset		= sizeof(pcmad_boards[0]),
};
module_comedi_driver(pcmad_driver);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Comedi http://www.comedi.org");
MODULE_DESCRIPTION("Comedi low-level driver");
MODULE_LICENSE("GPL");
