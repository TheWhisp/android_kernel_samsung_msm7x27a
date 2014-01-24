/*

    comedi/drivers/aio_aio12_8.c

    Driver for Access I/O Products PC-104 AIO12-8 Analog I/O Board
    Copyright (C) 2006 C&C Technologies, Inc.

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

Driver: aio_aio12_8
Description: Access I/O Products PC-104 AIO12-8 Analog I/O Board
Author: Pablo Mejia <pablo.mejia@cctechnol.com>
<<<<<<< HEAD
Devices:
 [Access I/O] PC-104 AIO12-8
=======
Devices: [Access I/O] PC-104 AIO12-8 (aio_aio12_8)
	 [Access I/O] PC-104 AI12-8 (aio_ai12_8)
	 [Access I/O] PC-104 AO12-8 (aio_ao12_8)
>>>>>>> refs/remotes/origin/master
Status: experimental

Configuration Options:
  [0] - I/O port base address

Notes:

  Only synchronous operations are supported.

*/

<<<<<<< HEAD
#include "../comedidev.h"
#include <linux/ioport.h>
#include "8255.h"

#define AIO12_8_STATUS			0x00
#define AIO12_8_INTERRUPT		0x01
#define AIO12_8_ADC			0x02
#define AIO12_8_DAC_0			0x04
#define AIO12_8_DAC_1			0x06
#define AIO12_8_DAC_2			0x08
#define AIO12_8_DAC_3			0x0A
#define AIO12_8_COUNTER_0		0x0C
#define AIO12_8_COUNTER_1		0x0D
#define AIO12_8_COUNTER_2		0x0E
#define AIO12_8_COUNTER_CONTROL		0x0F
#define AIO12_8_DIO_0			0x10
#define AIO12_8_DIO_1			0x11
#define AIO12_8_DIO_2			0x12
#define AIO12_8_DIO_STATUS		0x13
#define AIO12_8_DIO_CONTROL		0x14
#define AIO12_8_ADC_TRIGGER_CONTROL	0x15
#define AIO12_8_TRIGGER			0x16
#define AIO12_8_POWER			0x17

#define STATUS_ADC_EOC			0x80

#define ADC_MODE_NORMAL			0x00
#define ADC_MODE_INTERNAL_CLOCK		0x40
#define ADC_MODE_STANDBY		0x80
#define ADC_MODE_POWERDOWN		0xC0

#define DAC_ENABLE			0x18

struct aio12_8_boardtype {
	const char *name;
=======
#include <linux/module.h>
#include "../comedidev.h"
#include "8255.h"

/*
 * Register map
 */
#define AIO12_8_STATUS_REG		0x00
#define AIO12_8_STATUS_ADC_EOC		(1 << 7)
#define AIO12_8_STATUS_PORT_C_COS	(1 << 6)
#define AIO12_8_STATUS_IRQ_ENA		(1 << 2)
#define AIO12_8_INTERRUPT_REG		0x01
#define AIO12_8_INTERRUPT_ADC		(1 << 7)
#define AIO12_8_INTERRUPT_COS		(1 << 6)
#define AIO12_8_INTERRUPT_COUNTER1	(1 << 5)
#define AIO12_8_INTERRUPT_PORT_C3	(1 << 4)
#define AIO12_8_INTERRUPT_PORT_C0	(1 << 3)
#define AIO12_8_INTERRUPT_ENA		(1 << 2)
#define AIO12_8_ADC_REG			0x02
#define AIO12_8_ADC_MODE_NORMAL		(0 << 6)
#define AIO12_8_ADC_MODE_INT_CLK	(1 << 6)
#define AIO12_8_ADC_MODE_STANDBY	(2 << 6)
#define AIO12_8_ADC_MODE_POWERDOWN	(3 << 6)
#define AIO12_8_ADC_ACQ_3USEC		(0 << 5)
#define AIO12_8_ADC_ACQ_PROGRAM		(1 << 5)
#define AIO12_8_ADC_RANGE(x)		((x) << 3)
#define AIO12_8_ADC_CHAN(x)		((x) << 0)
#define AIO12_8_DAC_REG(x)		(0x04 + (x) * 2)
#define AIO12_8_8254_BASE_REG		0x0c
#define AIO12_8_8255_BASE_REG		0x10
#define AIO12_8_DIO_CONTROL_REG		0x14
#define AIO12_8_DIO_CONTROL_TST		(1 << 0)
#define AIO12_8_ADC_TRIGGER_REG		0x15
#define AIO12_8_ADC_TRIGGER_RANGE(x)	((x) << 3)
#define AIO12_8_ADC_TRIGGER_CHAN(x)	((x) << 0)
#define AIO12_8_TRIGGER_REG		0x16
#define AIO12_8_TRIGGER_ADTRIG		(1 << 1)
#define AIO12_8_TRIGGER_DACTRIG		(1 << 0)
#define AIO12_8_COS_REG			0x17
#define AIO12_8_DAC_ENABLE_REG		0x18
#define AIO12_8_DAC_ENABLE_REF_ENA	(1 << 0)

struct aio12_8_boardtype {
	const char *name;
	int ai_nchan;
	int ao_nchan;
>>>>>>> refs/remotes/origin/master
};

static const struct aio12_8_boardtype board_types[] = {
	{
<<<<<<< HEAD
	 .name = "aio_aio12_8"},
};

#define	thisboard	((const struct aio12_8_boardtype  *) dev->board_ptr)

=======
		.name		= "aio_aio12_8",
		.ai_nchan	= 8,
		.ao_nchan	= 4,
	}, {
		.name		= "aio_ai12_8",
		.ai_nchan	= 8,
	}, {
		.name		= "aio_ao12_8",
		.ao_nchan	= 4,
	},
};

>>>>>>> refs/remotes/origin/master
struct aio12_8_private {
	unsigned int ao_readback[4];
};

<<<<<<< HEAD
#define devpriv	((struct aio12_8_private *) dev->private)

=======
>>>>>>> refs/remotes/origin/master
static int aio_aio12_8_ai_read(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn, unsigned int *data)
{
<<<<<<< HEAD
	int n;
	unsigned char control =
	    ADC_MODE_NORMAL |
	    (CR_RANGE(insn->chanspec) << 3) | CR_CHAN(insn->chanspec);

	/* read status to clear EOC latch */
	inb(dev->iobase + AIO12_8_STATUS);
=======
	unsigned int chan = CR_CHAN(insn->chanspec);
	unsigned int range = CR_RANGE(insn->chanspec);
	unsigned int val;
	unsigned char control;
	int n;

	/*
	 * Setup the control byte for internal 2MHz clock, 3uS conversion,
	 * at the desired range of the requested channel.
	 */
	control = AIO12_8_ADC_MODE_NORMAL | AIO12_8_ADC_ACQ_3USEC |
		  AIO12_8_ADC_RANGE(range) | AIO12_8_ADC_CHAN(chan);

	/* Read status to clear EOC latch */
	inb(dev->iobase + AIO12_8_STATUS_REG);
>>>>>>> refs/remotes/origin/master

	for (n = 0; n < insn->n; n++) {
		int timeout = 5;

		/*  Setup and start conversion */
<<<<<<< HEAD
		outb(control, dev->iobase + AIO12_8_ADC);

		/*  Wait for conversion to complete */
		while (timeout &&
		       !(inb(dev->iobase + AIO12_8_STATUS) & STATUS_ADC_EOC)) {
			timeout--;
			printk(KERN_ERR "timeout %d\n", timeout);
			udelay(1);
		}
		if (timeout == 0) {
			comedi_error(dev, "ADC timeout");
			return -EIO;
		}

		data[n] = inw(dev->iobase + AIO12_8_ADC) & 0x0FFF;
	}
	return n;
=======
		outb(control, dev->iobase + AIO12_8_ADC_REG);

		/*  Wait for conversion to complete */
		do {
			val = inb(dev->iobase + AIO12_8_STATUS_REG);
			timeout--;
			if (timeout == 0) {
				dev_err(dev->class_dev, "ADC timeout\n");
				return -ETIMEDOUT;
			}
		} while (!(val & AIO12_8_STATUS_ADC_EOC));

		data[n] = inw(dev->iobase + AIO12_8_ADC_REG) & s->maxdata;
	}

	return insn->n;
>>>>>>> refs/remotes/origin/master
}

static int aio_aio12_8_ao_read(struct comedi_device *dev,
			       struct comedi_subdevice *s,
			       struct comedi_insn *insn, unsigned int *data)
{
<<<<<<< HEAD
	int i;
	int val = devpriv->ao_readback[CR_CHAN(insn->chanspec)];
=======
	struct aio12_8_private *devpriv = dev->private;
	unsigned int chan = CR_CHAN(insn->chanspec);
	int val = devpriv->ao_readback[chan];
	int i;
>>>>>>> refs/remotes/origin/master

	for (i = 0; i < insn->n; i++)
		data[i] = val;
	return insn->n;
}

static int aio_aio12_8_ao_write(struct comedi_device *dev,
				struct comedi_subdevice *s,
				struct comedi_insn *insn, unsigned int *data)
{
<<<<<<< HEAD
	int i;
	int chan = CR_CHAN(insn->chanspec);
	unsigned long port = dev->iobase + AIO12_8_DAC_0 + (2 * chan);

	/* enable DACs */
	outb(0x01, dev->iobase + DAC_ENABLE);

	for (i = 0; i < insn->n; i++) {
		outb(data[i] & 0xFF, port);	/*  LSB */
		outb((data[i] >> 8) & 0x0F, port + 1);	/*  MSB */
		devpriv->ao_readback[chan] = data[i];
	}
=======
	struct aio12_8_private *devpriv = dev->private;
	unsigned int chan = CR_CHAN(insn->chanspec);
	unsigned long port = dev->iobase + AIO12_8_DAC_REG(chan);
	unsigned int val = 0;
	int i;

	/* enable DACs */
	outb(AIO12_8_DAC_ENABLE_REF_ENA, dev->iobase + AIO12_8_DAC_ENABLE_REG);

	for (i = 0; i < insn->n; i++) {
		val = data[i];
		outw(val, port);
	}

	devpriv->ao_readback[chan] = val;

>>>>>>> refs/remotes/origin/master
	return insn->n;
}

static const struct comedi_lrange range_aio_aio12_8 = {
<<<<<<< HEAD
	4,
	{
	 UNI_RANGE(5),
	 BIP_RANGE(5),
	 UNI_RANGE(10),
	 BIP_RANGE(10),
	 }
=======
	4, {
		UNI_RANGE(5),
		BIP_RANGE(5),
		UNI_RANGE(10),
		BIP_RANGE(10)
	}
>>>>>>> refs/remotes/origin/master
};

static int aio_aio12_8_attach(struct comedi_device *dev,
			      struct comedi_devconfig *it)
{
<<<<<<< HEAD
	int iobase;
	struct comedi_subdevice *s;

	iobase = it->options[0];
	if (!request_region(iobase, 24, "aio_aio12_8")) {
		printk(KERN_ERR "I/O port conflict");
		return -EIO;
	}

	dev->board_name = thisboard->name;

	dev->iobase = iobase;

	if (alloc_private(dev, sizeof(struct aio12_8_private)) < 0)
		return -ENOMEM;

	if (alloc_subdevices(dev, 3) < 0)
		return -ENOMEM;

	s = &dev->subdevices[0];
	s->type = COMEDI_SUBD_AI;
	s->subdev_flags = SDF_READABLE | SDF_GROUND | SDF_DIFF;
	s->n_chan = 8;
	s->maxdata = (1 << 12) - 1;
	s->range_table = &range_aio_aio12_8;
	s->insn_read = aio_aio12_8_ai_read;

	s = &dev->subdevices[1];
	s->type = COMEDI_SUBD_AO;
	s->subdev_flags = SDF_WRITABLE | SDF_GROUND | SDF_DIFF;
	s->n_chan = 4;
	s->maxdata = (1 << 12) - 1;
	s->range_table = &range_aio_aio12_8;
	s->insn_read = aio_aio12_8_ao_read;
	s->insn_write = aio_aio12_8_ao_write;

	s = &dev->subdevices[2];
	subdev_8255_init(dev, s, NULL, dev->iobase + AIO12_8_DIO_0);

	return 0;
}

static int aio_aio12_8_detach(struct comedi_device *dev)
{
	subdev_8255_cleanup(dev, &dev->subdevices[2]);
	if (dev->iobase)
		release_region(dev->iobase, 24);
	return 0;
}

static struct comedi_driver driver_aio_aio12_8 = {
	.driver_name = "aio_aio12_8",
	.module = THIS_MODULE,
	.attach = aio_aio12_8_attach,
	.detach = aio_aio12_8_detach,
	.board_name = &board_types[0].name,
	.num_names = 1,
	.offset = sizeof(struct aio12_8_boardtype),
};

static int __init driver_aio_aio12_8_init_module(void)
{
	return comedi_driver_register(&driver_aio_aio12_8);
}

static void __exit driver_aio_aio12_8_cleanup_module(void)
{
	comedi_driver_unregister(&driver_aio_aio12_8);
}

module_init(driver_aio_aio12_8_init_module);
module_exit(driver_aio_aio12_8_cleanup_module);
=======
	const struct aio12_8_boardtype *board = comedi_board(dev);
	struct aio12_8_private *devpriv;
	struct comedi_subdevice *s;
	int ret;

	ret = comedi_request_region(dev, it->options[0], 32);
	if (ret)
		return ret;

	devpriv = comedi_alloc_devpriv(dev, sizeof(*devpriv));
	if (!devpriv)
		return -ENOMEM;

	ret = comedi_alloc_subdevices(dev, 4);
	if (ret)
		return ret;

	s = &dev->subdevices[0];
	if (board->ai_nchan) {
		/* Analog input subdevice */
		s->type		= COMEDI_SUBD_AI;
		s->subdev_flags	= SDF_READABLE | SDF_GROUND | SDF_DIFF;
		s->n_chan	= board->ai_nchan;
		s->maxdata	= 0x0fff;
		s->range_table	= &range_aio_aio12_8;
		s->insn_read	= aio_aio12_8_ai_read;
	} else {
		s->type = COMEDI_SUBD_UNUSED;
	}

	s = &dev->subdevices[1];
	if (board->ao_nchan) {
		/* Analog output subdevice */
		s->type		= COMEDI_SUBD_AO;
		s->subdev_flags	= SDF_WRITABLE | SDF_GROUND | SDF_DIFF;
		s->n_chan	= 4;
		s->maxdata	= 0x0fff;
		s->range_table	= &range_aio_aio12_8;
		s->insn_read	= aio_aio12_8_ao_read;
		s->insn_write	= aio_aio12_8_ao_write;
	} else {
		s->type = COMEDI_SUBD_UNUSED;
	}

	s = &dev->subdevices[2];
	/* 8255 Digital i/o subdevice */
	ret = subdev_8255_init(dev, s, NULL,
			       dev->iobase + AIO12_8_8255_BASE_REG);
	if (ret)
		return ret;

	s = &dev->subdevices[3];
	/* 8254 counter/timer subdevice */
	s->type		= COMEDI_SUBD_UNUSED;

	dev_info(dev->class_dev, "%s: %s attached\n",
		dev->driver->driver_name, dev->board_name);

	return 0;
}

static struct comedi_driver aio_aio12_8_driver = {
	.driver_name	= "aio_aio12_8",
	.module		= THIS_MODULE,
	.attach		= aio_aio12_8_attach,
	.detach		= comedi_legacy_detach,
	.board_name	= &board_types[0].name,
	.num_names	= ARRAY_SIZE(board_types),
	.offset		= sizeof(struct aio12_8_boardtype),
};
module_comedi_driver(aio_aio12_8_driver);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Comedi http://www.comedi.org");
MODULE_DESCRIPTION("Comedi low-level driver");
MODULE_LICENSE("GPL");
