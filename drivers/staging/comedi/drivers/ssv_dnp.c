/*
    comedi/drivers/ssv_dnp.c
    generic comedi driver for SSV Embedded Systems' DIL/Net-PCs
    Copyright (C) 2001 Robert Schwebel <robert@schwebel.de>

    COMEDI - Linux Control and Measurement Device Interface
    Copyright (C) 2000 David A. Schleef <ds@schleef.org>

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
Driver: ssv_dnp
Description: SSV Embedded Systems DIL/Net-PC
Author: Robert Schwebel <robert@schwebel.de>
Devices: [SSV Embedded Systems] DIL/Net-PC 1486 (dnp-1486)
Status: unknown
*/

/* include files ----------------------------------------------------------- */

<<<<<<< HEAD
=======
#include <linux/module.h>
>>>>>>> refs/remotes/origin/master
#include "../comedidev.h"

/* Some global definitions: the registers of the DNP ----------------------- */
/*                                                                           */
/* For port A and B the mode register has bits corresponding to the output   */
/* pins, where Bit-N = 0 -> input, Bit-N = 1 -> output. Note that bits       */
/* 4 to 7 correspond to pin 0..3 for port C data register. Ensure that bits  */
/* 0..3 remain unchanged! For details about Port C Mode Register see         */
/* the remarks in dnp_insn_config() below.                                   */

#define CSCIR 0x22		/* Chip Setup and Control Index Register     */
#define CSCDR 0x23		/* Chip Setup and Control Data Register      */
#define PAMR  0xa5		/* Port A Mode Register                      */
#define PADR  0xa9		/* Port A Data Register                      */
#define PBMR  0xa4		/* Port B Mode Register                      */
#define PBDR  0xa8		/* Port B Data Register                      */
#define PCMR  0xa3		/* Port C Mode Register                      */
#define PCDR  0xa7		/* Port C Data Register                      */

<<<<<<< HEAD
/* This data structure holds information about the supported boards -------- */

struct dnp_board {
	const char *name;
	int ai_chans;
	int ai_bits;
	int have_dio;
};

/* We only support one DNP 'board' variant at the moment */
static const struct dnp_board dnp_boards[] = {
{
	 .name = "dnp-1486",
	 .ai_chans = 16,
	 .ai_bits = 12,
	 .have_dio = 1,
	 },
};

/* Useful for shorthand access to the particular board structure ----------- */
#define thisboard ((const struct dnp_board *)dev->board_ptr)

/* This structure is for data unique to the DNP driver --------------------- */
struct dnp_private_data {

};

/* Shorthand macro for faster access to the private data ------------------- */
#define devpriv ((dnp_private *)dev->private)

/* ------------------------------------------------------------------------- */
/* The struct comedi_driver structure tells the Comedi core module which     */
/* functions to call to configure/deconfigure (attach/detach) the board, and */
/* also about the kernel module that contains the device code.               */
/*                                                                           */
/* In the following section we define the API of this driver.                */
/* ------------------------------------------------------------------------- */

static int dnp_attach(struct comedi_device *dev, struct comedi_devconfig *it);
static int dnp_detach(struct comedi_device *dev);

static struct comedi_driver driver_dnp = {
	.driver_name = "ssv_dnp",
	.module = THIS_MODULE,
	.attach = dnp_attach,
	.detach = dnp_detach,
	.board_name = &dnp_boards[0].name,
	/* only necessary for non-PnP devs   */
	.offset = sizeof(struct dnp_board),   /* like ISA-PnP, PCI or PCMCIA */
	.num_names = ARRAY_SIZE(dnp_boards),
};

static int __init driver_dnp_init_module(void)
{
	return comedi_driver_register(&driver_dnp);
}

static void __exit driver_dnp_cleanup_module(void)
{
	comedi_driver_unregister(&driver_dnp);
}

module_init(driver_dnp_init_module);
module_exit(driver_dnp_cleanup_module);

static int dnp_dio_insn_bits(struct comedi_device *dev,
			     struct comedi_subdevice *s,
			     struct comedi_insn *insn, unsigned int *data);

static int dnp_dio_insn_config(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn, unsigned int *data);

/* ------------------------------------------------------------------------- */
/* Attach is called by comedi core to configure the driver for a particular  */
/* board. If you specified a board_name array in the driver structure,       */
/* dev->board_ptr contains that address.                                     */
/* ------------------------------------------------------------------------- */

static int dnp_attach(struct comedi_device *dev, struct comedi_devconfig *it)
{

	struct comedi_subdevice *s;

	printk(KERN_INFO "comedi%d: dnp: ", dev->minor);

	/* Autoprobing: this should find out which board we have. Currently  */
	/* only the 1486 board is supported and autoprobing is not           */
	/* implemented :-)                                                   */
	/* dev->board_ptr = dnp_probe(dev); */

	/* Initialize the name of the board.                                 */
	/* We can use the "thisboard" macro now.                             */
	dev->board_name = thisboard->name;

	/* Allocate the private structure area. alloc_private() is a         */
	/* convenient macro defined in comedidev.h.                          */
	if (alloc_private(dev, sizeof(struct dnp_private_data)) < 0)
		return -ENOMEM;

	/* Allocate the subdevice structures. alloc_subdevice() is a         */
	/* convenient macro defined in comedidev.h.                          */

	if (alloc_subdevices(dev, 1) < 0)
		return -ENOMEM;

	s = dev->subdevices + 0;
=======
static int dnp_dio_insn_bits(struct comedi_device *dev,
			     struct comedi_subdevice *s,
			     struct comedi_insn *insn,
			     unsigned int *data)
{
	unsigned int mask;
	unsigned int val;

	/*
	 * Ports A and B are straight forward: each bit corresponds to an
	 * output pin with the same order. Port C is different: bits 0...3
	 * correspond to bits 4...7 of the output register (PCDR).
	 */

	mask = comedi_dio_update_state(s, data);
	if (mask) {
		outb(PADR, CSCIR);
		outb(s->state & 0xff, CSCDR);

		outb(PBDR, CSCIR);
		outb((s->state >> 8) & 0xff, CSCDR);

		outb(PCDR, CSCIR);
		val = inb(CSCDR) & 0x0f;
		outb(((s->state >> 12) & 0xf0) | val, CSCDR);
	}

	outb(PADR, CSCIR);
	val = inb(CSCDR);
	outb(PBDR, CSCIR);
	val |= (inb(CSCDR) << 8);
	outb(PCDR, CSCIR);
	val |= ((inb(CSCDR) & 0xf0) << 12);

	data[1] = val;

	return insn->n;
}

static int dnp_dio_insn_config(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn,
			       unsigned int *data)
{
	unsigned int chan = CR_CHAN(insn->chanspec);
	unsigned int mask;
	unsigned int val;
	int ret;

	ret = comedi_dio_insn_config(dev, s, insn, data, 0);
	if (ret)
		return ret;

	if (chan < 8) {			/* Port A */
		mask = 1 << chan;
		outb(PAMR, CSCIR);
	} else if (chan < 16) {		/* Port B */
		mask = 1 << (chan - 8);
		outb(PBMR, CSCIR);
	} else {			/* Port C */
		/*
		 * We have to pay attention with port C.
		 * This is the meaning of PCMR:
		 *   Bit in PCMR:              7 6 5 4 3 2 1 0
		 *   Corresponding port C pin: d 3 d 2 d 1 d 0   d= don't touch
		 *
		 * Multiplication by 2 brings bits into correct position
		 * for PCMR!
		 */
		mask = 1 << ((chan - 16) * 2);
		outb(PCMR, CSCIR);
	}

	val = inb(CSCDR);
	if (data[0] == COMEDI_OUTPUT)
		val |= mask;
	else
		val &= ~mask;
	outb(val, CSCDR);

	return insn->n;

}

static int dnp_attach(struct comedi_device *dev, struct comedi_devconfig *it)
{
	struct comedi_subdevice *s;
	int ret;

	ret = comedi_alloc_subdevices(dev, 1);
	if (ret)
		return ret;

	s = &dev->subdevices[0];
>>>>>>> refs/remotes/origin/master
	/* digital i/o subdevice                                             */
	s->type = COMEDI_SUBD_DIO;
	s->subdev_flags = SDF_READABLE | SDF_WRITABLE;
	s->n_chan = 20;
	s->maxdata = 1;
	s->range_table = &range_digital;
	s->insn_bits = dnp_dio_insn_bits;
	s->insn_config = dnp_dio_insn_config;

<<<<<<< HEAD
	printk("attached\n");

=======
>>>>>>> refs/remotes/origin/master
	/* We use the I/O ports 0x22,0x23 and 0xa3-0xa9, which are always
	 * allocated for the primary 8259, so we don't need to allocate them
	 * ourselves. */

	/* configure all ports as input (default)                            */
	outb(PAMR, CSCIR);
	outb(0x00, CSCDR);
	outb(PBMR, CSCIR);
	outb(0x00, CSCDR);
	outb(PCMR, CSCIR);
	outb((inb(CSCDR) & 0xAA), CSCDR);

<<<<<<< HEAD
	return 1;

}

/* ------------------------------------------------------------------------- */
/* detach is called to deconfigure a device. It should deallocate the        */
/* resources. This function is also called when _attach() fails, so it       */
/* should be careful not to release resources that were not necessarily      */
/* allocated by _attach(). dev->private and dev->subdevices are              */
/* deallocated automatically by the core.                                    */
/* ------------------------------------------------------------------------- */

static int dnp_detach(struct comedi_device *dev)
{

	/* configure all ports as input (default)                            */
=======
	dev_info(dev->class_dev, "%s: attached\n", dev->board_name);
	return 1;
}

static void dnp_detach(struct comedi_device *dev)
{
>>>>>>> refs/remotes/origin/master
	outb(PAMR, CSCIR);
	outb(0x00, CSCDR);
	outb(PBMR, CSCIR);
	outb(0x00, CSCDR);
	outb(PCMR, CSCIR);
	outb((inb(CSCDR) & 0xAA), CSCDR);
<<<<<<< HEAD

	/* announce that we are finished                                     */
	printk(KERN_INFO "comedi%d: dnp: remove\n", dev->minor);

	return 0;

}

/* ------------------------------------------------------------------------- */
/* The insn_bits interface allows packed reading/writing of DIO channels.    */
/* The comedi core can convert between insn_bits and insn_read/write, so you */
/* are able to use these instructions as well.                               */
/* ------------------------------------------------------------------------- */

static int dnp_dio_insn_bits(struct comedi_device *dev,
			     struct comedi_subdevice *s,
			     struct comedi_insn *insn, unsigned int *data)
{

	if (insn->n != 2)
		return -EINVAL;	/* insn uses data[0] and data[1]     */

	/* The insn data is a mask in data[0] and the new data in data[1],   */
	/* each channel cooresponding to a bit.                              */

	/* Ports A and B are straight forward: each bit corresponds to an    */
	/* output pin with the same order. Port C is different: bits 0...3   */
	/* correspond to bits 4...7 of the output register (PCDR).           */

	if (data[0]) {

		outb(PADR, CSCIR);
		outb((inb(CSCDR)
		      & ~(u8) (data[0] & 0x0000FF))
		     | (u8) (data[1] & 0x0000FF), CSCDR);

		outb(PBDR, CSCIR);
		outb((inb(CSCDR)
		      & ~(u8) ((data[0] & 0x00FF00) >> 8))
		     | (u8) ((data[1] & 0x00FF00) >> 8), CSCDR);

		outb(PCDR, CSCIR);
		outb((inb(CSCDR)
		      & ~(u8) ((data[0] & 0x0F0000) >> 12))
		     | (u8) ((data[1] & 0x0F0000) >> 12), CSCDR);
	}

	/* on return, data[1] contains the value of the digital input lines. */
	outb(PADR, CSCIR);
<<<<<<< HEAD
<<<<<<< HEAD
	data[1] = inb(CSCDR);
	outb(PBDR, CSCIR);
	data[1] += inb(CSCDR) << 8;
	outb(PCDR, CSCIR);
	data[1] += ((inb(CSCDR) & 0xF0) << 12);
=======
	data[0] = inb(CSCDR);
=======
	data[1] = inb(CSCDR);
>>>>>>> refs/remotes/origin/cm-11.0
	outb(PBDR, CSCIR);
	data[1] += inb(CSCDR) << 8;
	outb(PCDR, CSCIR);
<<<<<<< HEAD
	data[0] += ((inb(CSCDR) & 0xF0) << 12);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	data[1] += ((inb(CSCDR) & 0xF0) << 12);
>>>>>>> refs/remotes/origin/cm-11.0

	return 2;

}

/* ------------------------------------------------------------------------- */
/* Configure the direction of the bidirectional digital i/o pins. chanspec   */
/* contains the channel to be changed and data[0] contains either            */
/* COMEDI_INPUT or COMEDI_OUTPUT.                                            */
/* ------------------------------------------------------------------------- */

static int dnp_dio_insn_config(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn, unsigned int *data)
{

	u8 register_buffer;

	/* reduces chanspec to lower 16 bits */
	int chan = CR_CHAN(insn->chanspec);

	switch (data[0]) {
	case INSN_CONFIG_DIO_OUTPUT:
	case INSN_CONFIG_DIO_INPUT:
		break;
	case INSN_CONFIG_DIO_QUERY:
		data[1] =
		    (inb(CSCDR) & (1 << chan)) ? COMEDI_OUTPUT : COMEDI_INPUT;
		return insn->n;
		break;
	default:
		return -EINVAL;
		break;
	}
	/* Test: which port does the channel belong to?                       */

	/* We have to pay attention with port C: this is the meaning of PCMR: */
	/* Bit in PCMR:              7 6 5 4 3 2 1 0                          */
	/* Corresponding port C pin: d 3 d 2 d 1 d 0   d= don't touch         */

	if ((chan >= 0) && (chan <= 7)) {
		/* this is port A */
		outb(PAMR, CSCIR);
	} else if ((chan >= 8) && (chan <= 15)) {
		/* this is port B */
		chan -= 8;
		outb(PBMR, CSCIR);
	} else if ((chan >= 16) && (chan <= 19)) {
		/* this is port C; multiplication with 2 brings bits into     */
		/* correct position for PCMR!                                 */
		chan -= 16;
		chan *= 2;
		outb(PCMR, CSCIR);
	} else {
		return -EINVAL;
	}

	/* read 'old' direction of the port and set bits (out=1, in=0)        */
	register_buffer = inb(CSCDR);
	if (data[0] == COMEDI_OUTPUT)
		register_buffer |= (1 << chan);
	else
		register_buffer &= ~(1 << chan);

	outb(register_buffer, CSCDR);

	return 1;

}
=======
}

static struct comedi_driver dnp_driver = {
	.driver_name	= "dnp-1486",
	.module		= THIS_MODULE,
	.attach		= dnp_attach,
	.detach		= dnp_detach,
};
module_comedi_driver(dnp_driver);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Comedi http://www.comedi.org");
MODULE_DESCRIPTION("Comedi low-level driver");
MODULE_LICENSE("GPL");
