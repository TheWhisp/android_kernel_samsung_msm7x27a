/*

comedi/drivers/adl_pci9111.c

Hardware driver for PCI9111 ADLink cards:

PCI-9111HR

Copyright (C) 2002-2005 Emmanuel Pacaud <emmanuel.pacaud@univ-poitiers.fr>

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
Driver: adl_pci9111
Description: Adlink PCI-9111HR
Author: Emmanuel Pacaud <emmanuel.pacaud@univ-poitiers.fr>
Devices: [ADLink] PCI-9111HR (adl_pci9111)
Status: experimental

Supports:

	- ai_insn read
	- ao_insn read/write
	- di_insn read
	- do_insn read/write
	- ai_do_cmd mode with the following sources:

	- start_src		TRIG_NOW
	- scan_begin_src	TRIG_FOLLOW	TRIG_TIMER	TRIG_EXT
	- convert_src				TRIG_TIMER	TRIG_EXT
	- scan_end_src		TRIG_COUNT
	- stop_src		TRIG_COUNT	TRIG_NONE

The scanned channels must be consecutive and start from 0. They must
all have the same range and aref.

<<<<<<< HEAD
Configuration options:

	[0] - PCI bus number (optional)
	[1] - PCI slot number (optional)

If bus/slot is not specified, the first available PCI
device will be used.

=======
Configuration options: not applicable, uses PCI auto config
>>>>>>> refs/remotes/origin/master
*/

/*
CHANGELOG:

2005/02/17 Extend AI streaming capabilities. Now, scan_begin_arg can be
a multiple of chanlist_len*convert_arg.
2002/02/19 Fixed the two's complement conversion in pci9111_(hr_)ai_get_data.
2002/02/18 Added external trigger support for analog input.

TODO:

	- Really test implemented functionality.
	- Add support for the PCI-9111DG with a probe routine to identify
	  the card type (perhaps with the help of the channel number readback
	  of the A/D Data register).
	- Add external multiplexer support.

*/

<<<<<<< HEAD
#include "../comedidev.h"

#include <linux/delay.h>
#include <linux/interrupt.h>

#include "8253.h"
#include "comedi_pci.h"
=======
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/interrupt.h>

#include "../comedidev.h"

#include "8253.h"
#include "plx9052.h"
>>>>>>> refs/remotes/origin/master
#include "comedi_fc.h"

#define PCI9111_DRIVER_NAME	"adl_pci9111"
#define PCI9111_HR_DEVICE_ID	0x9111

<<<<<<< HEAD
/*  TODO: Add other pci9111 board id */

#define PCI9111_IO_RANGE	0x0100

#define PCI9111_FIFO_HALF_SIZE	512

#define PCI9111_AI_CHANNEL_NBR			16

#define PCI9111_AI_RESOLUTION			12
#define PCI9111_AI_RESOLUTION_MASK		0x0FFF
#define PCI9111_AI_RESOLUTION_2_CMP_BIT		0x0800

#define PCI9111_HR_AI_RESOLUTION		16
#define PCI9111_HR_AI_RESOLUTION_MASK		0xFFFF
#define PCI9111_HR_AI_RESOLUTION_2_CMP_BIT	0x8000

#define PCI9111_AI_ACQUISITION_PERIOD_MIN_NS	10000
#define PCI9111_AO_CHANNEL_NBR			1
#define	PCI9111_AO_RESOLUTION			12
#define PCI9111_AO_RESOLUTION_MASK		0x0FFF
#define PCI9111_DI_CHANNEL_NBR			16
#define	PCI9111_DO_CHANNEL_NBR			16
#define PCI9111_DO_MASK				0xFFFF
=======
#define PCI9111_FIFO_HALF_SIZE	512

#define PCI9111_AI_ACQUISITION_PERIOD_MIN_NS	10000
>>>>>>> refs/remotes/origin/master

#define PCI9111_RANGE_SETTING_DELAY		10
#define PCI9111_AI_INSTANT_READ_UDELAY_US	2
#define PCI9111_AI_INSTANT_READ_TIMEOUT		100

<<<<<<< HEAD
#define PCI9111_8254_CLOCK_PERIOD_NS		500

#define PCI9111_8254_COUNTER_0			0x00
#define PCI9111_8254_COUNTER_1			0x40
#define PCI9111_8254_COUNTER_2			0x80
#define PCI9111_8254_COUNTER_LATCH		0x00
#define PCI9111_8254_READ_LOAD_LSB_ONLY		0x10
#define PCI9111_8254_READ_LOAD_MSB_ONLY		0x20
#define PCI9111_8254_READ_LOAD_LSB_MSB		0x30
#define PCI9111_8254_MODE_0			0x00
#define PCI9111_8254_MODE_1			0x02
#define PCI9111_8254_MODE_2			0x04
#define PCI9111_8254_MODE_3			0x06
#define PCI9111_8254_MODE_4			0x08
#define PCI9111_8254_MODE_5			0x0A
#define PCI9111_8254_BINARY_COUNTER		0x00
#define PCI9111_8254_BCD_COUNTER		0x01

/* IO address map */

#define PCI9111_REGISTER_AD_FIFO_VALUE			0x00 /* AD Data stored
								in FIFO */
#define PCI9111_REGISTER_DA_OUTPUT			0x00
#define PCI9111_REGISTER_DIGITAL_IO			0x02
#define PCI9111_REGISTER_EXTENDED_IO_PORTS		0x04
#define PCI9111_REGISTER_AD_CHANNEL_CONTROL		0x06 /* Channel
								selection */
#define PCI9111_REGISTER_AD_CHANNEL_READBACK		0x06
#define PCI9111_REGISTER_INPUT_SIGNAL_RANGE		0x08
#define PCI9111_REGISTER_RANGE_STATUS_READBACK		0x08
#define PCI9111_REGISTER_TRIGGER_MODE_CONTROL		0x0A
#define PCI9111_REGISTER_AD_MODE_INTERRUPT_READBACK	0x0A
#define PCI9111_REGISTER_SOFTWARE_TRIGGER		0x0E
#define PCI9111_REGISTER_INTERRUPT_CONTROL		0x0C
#define PCI9111_REGISTER_8254_COUNTER_0			0x40
#define PCI9111_REGISTER_8254_COUNTER_1			0x42
#define PCI9111_REGISTER_8254_COUNTER_2			0X44
#define PCI9111_REGISTER_8254_CONTROL			0x46
#define PCI9111_REGISTER_INTERRUPT_CLEAR		0x48

#define PCI9111_TRIGGER_MASK				0x0F
#define PCI9111_PTRG_OFF				(0 << 3)
#define PCI9111_PTRG_ON					(1 << 3)
#define PCI9111_EITS_EXTERNAL				(1 << 2)
#define PCI9111_EITS_INTERNAL				(0 << 2)
#define PCI9111_TPST_SOFTWARE_TRIGGER			(0 << 1)
#define PCI9111_TPST_TIMER_PACER			(1 << 1)
#define PCI9111_ASCAN_ON				(1 << 0)
#define PCI9111_ASCAN_OFF				(0 << 0)

#define PCI9111_ISC0_SET_IRQ_ON_ENDING_OF_AD_CONVERSION (0 << 0)
#define PCI9111_ISC0_SET_IRQ_ON_FIFO_HALF_FULL		(1 << 0)
#define PCI9111_ISC1_SET_IRQ_ON_TIMER_TICK		(0 << 1)
#define PCI9111_ISC1_SET_IRQ_ON_EXT_TRG			(1 << 1)
#define PCI9111_FFEN_SET_FIFO_ENABLE			(0 << 2)
#define PCI9111_FFEN_SET_FIFO_DISABLE			(1 << 2)

#define PCI9111_CHANNEL_MASK				0x0F

#define PCI9111_RANGE_MASK				0x07
#define PCI9111_FIFO_EMPTY_MASK				0x10
#define PCI9111_FIFO_HALF_FULL_MASK			0x20
#define PCI9111_FIFO_FULL_MASK				0x40
#define PCI9111_AD_BUSY_MASK				0x80

#define PCI9111_IO_BASE (dev->iobase)

/*
 * Define inlined function
 */

#define pci9111_trigger_and_autoscan_get() \
	(inb(PCI9111_IO_BASE+PCI9111_REGISTER_AD_MODE_INTERRUPT_READBACK)&0x0F)

#define pci9111_trigger_and_autoscan_set(flags) \
	outb(flags, PCI9111_IO_BASE+PCI9111_REGISTER_TRIGGER_MODE_CONTROL)

#define pci9111_interrupt_and_fifo_get() \
	((inb(PCI9111_IO_BASE+PCI9111_REGISTER_AD_MODE_INTERRUPT_READBACK) \
		>> 4) & 0x03)

#define pci9111_interrupt_and_fifo_set(flags) \
	outb(flags, PCI9111_IO_BASE+PCI9111_REGISTER_INTERRUPT_CONTROL)

#define pci9111_interrupt_clear() \
	outb(0, PCI9111_IO_BASE+PCI9111_REGISTER_INTERRUPT_CLEAR)

#define pci9111_software_trigger() \
	outb(0, PCI9111_IO_BASE+PCI9111_REGISTER_SOFTWARE_TRIGGER)

#define pci9111_fifo_reset() do { \
	outb(PCI9111_FFEN_SET_FIFO_ENABLE, \
		PCI9111_IO_BASE+PCI9111_REGISTER_INTERRUPT_CONTROL); \
	outb(PCI9111_FFEN_SET_FIFO_DISABLE, \
		PCI9111_IO_BASE+PCI9111_REGISTER_INTERRUPT_CONTROL); \
	outb(PCI9111_FFEN_SET_FIFO_ENABLE, \
		PCI9111_IO_BASE+PCI9111_REGISTER_INTERRUPT_CONTROL); \
	} while (0)

#define pci9111_is_fifo_full() \
	((inb(PCI9111_IO_BASE+PCI9111_REGISTER_RANGE_STATUS_READBACK)& \
		PCI9111_FIFO_FULL_MASK) == 0)

#define pci9111_is_fifo_half_full() \
	((inb(PCI9111_IO_BASE+PCI9111_REGISTER_RANGE_STATUS_READBACK)& \
		PCI9111_FIFO_HALF_FULL_MASK) == 0)

#define pci9111_is_fifo_empty() \
	((inb(PCI9111_IO_BASE+PCI9111_REGISTER_RANGE_STATUS_READBACK)& \
		PCI9111_FIFO_EMPTY_MASK) == 0)

#define pci9111_ai_channel_set(channel) \
	outb((channel)&PCI9111_CHANNEL_MASK, \
		PCI9111_IO_BASE+PCI9111_REGISTER_AD_CHANNEL_CONTROL)

#define pci9111_ai_channel_get() \
	(inb(PCI9111_IO_BASE+PCI9111_REGISTER_AD_CHANNEL_READBACK) \
		&PCI9111_CHANNEL_MASK)

#define pci9111_ai_range_set(range) \
	outb((range)&PCI9111_RANGE_MASK, \
		PCI9111_IO_BASE+PCI9111_REGISTER_INPUT_SIGNAL_RANGE)

#define pci9111_ai_range_get() \
	(inb(PCI9111_IO_BASE+PCI9111_REGISTER_RANGE_STATUS_READBACK) \
		&PCI9111_RANGE_MASK)

#define pci9111_ai_get_data() \
	(((inw(PCI9111_IO_BASE+PCI9111_REGISTER_AD_FIFO_VALUE)>>4) \
		&PCI9111_AI_RESOLUTION_MASK) \
			^ PCI9111_AI_RESOLUTION_2_CMP_BIT)

#define pci9111_hr_ai_get_data() \
	((inw(PCI9111_IO_BASE+PCI9111_REGISTER_AD_FIFO_VALUE) \
		&PCI9111_HR_AI_RESOLUTION_MASK) \
			^ PCI9111_HR_AI_RESOLUTION_2_CMP_BIT)

#define pci9111_ao_set_data(data) \
	outw(data&PCI9111_AO_RESOLUTION_MASK, \
		PCI9111_IO_BASE+PCI9111_REGISTER_DA_OUTPUT)

#define pci9111_di_get_bits() \
	inw(PCI9111_IO_BASE+PCI9111_REGISTER_DIGITAL_IO)

#define pci9111_do_set_bits(bits) \
	outw(bits, PCI9111_IO_BASE+PCI9111_REGISTER_DIGITAL_IO)

#define pci9111_8254_control_set(flags) \
	outb(flags, PCI9111_IO_BASE+PCI9111_REGISTER_8254_CONTROL)

#define pci9111_8254_counter_0_set(data) \
	do { \
		outb(data & 0xFF, \
			PCI9111_IO_BASE+PCI9111_REGISTER_8254_COUNTER_0); \
		outb((data >> 8) & 0xFF, \
			PCI9111_IO_BASE+PCI9111_REGISTER_8254_COUNTER_0); \
	} while (0)

#define pci9111_8254_counter_1_set(data) \
	do { \
		outb(data & 0xFF, \
			PCI9111_IO_BASE+PCI9111_REGISTER_8254_COUNTER_1); \
		outb((data >> 8) & 0xFF, \
			PCI9111_IO_BASE+PCI9111_REGISTER_8254_COUNTER_1); \
	} while (0)

#define pci9111_8254_counter_2_set(data) \
	do { \
		outb(data & 0xFF, \
			PCI9111_IO_BASE+PCI9111_REGISTER_8254_COUNTER_2); \
		outb((data >> 8) & 0xFF, \
			PCI9111_IO_BASE+PCI9111_REGISTER_8254_COUNTER_2); \
	} while (0)

/*  Function prototypes */

static int pci9111_attach(struct comedi_device *dev,
			  struct comedi_devconfig *it);
static int pci9111_detach(struct comedi_device *dev);
static void pci9111_ai_munge(struct comedi_device *dev,
			     struct comedi_subdevice *s, void *data,
			     unsigned int num_bytes,
			     unsigned int start_chan_index);

static const struct comedi_lrange pci9111_hr_ai_range = {
	5,
	{
	 BIP_RANGE(10),
	 BIP_RANGE(5),
	 BIP_RANGE(2.5),
	 BIP_RANGE(1.25),
	 BIP_RANGE(0.625)
	 }
};

static DEFINE_PCI_DEVICE_TABLE(pci9111_pci_table) = {
<<<<<<< HEAD
	{ PCI_VENDOR_ID_ADLINK, PCI9111_HR_DEVICE_ID, PCI_ANY_ID, PCI_ANY_ID,
	  0, 0, 0 },
	/* { PCI_VENDOR_ID_ADLINK, PCI9111_HG_DEVICE_ID, PCI_ANY_ID, PCI_ANY_ID,
	 *   0, 0, 0 }, */
=======
	{ PCI_DEVICE(PCI_VENDOR_ID_ADLINK, PCI9111_HR_DEVICE_ID) },
	/* { PCI_DEVICE(PCI_VENDOR_ID_ADLINK, PCI9111_HG_DEVICE_ID) }, */
>>>>>>> refs/remotes/origin/cm-10.0
	{ 0 }
};

MODULE_DEVICE_TABLE(pci, pci9111_pci_table);

/*  */
/*  Board specification structure */
/*  */

struct pci9111_board {
	const char *name;	/*  driver name */
	int device_id;
	int ai_channel_nbr;	/*  num of A/D chans */
	int ao_channel_nbr;	/*  num of D/A chans */
	int ai_resolution;	/*  resolution of A/D */
	int ai_resolution_mask;
	int ao_resolution;	/*  resolution of D/A */
	int ao_resolution_mask;
	const struct comedi_lrange *ai_range_list;	/*  rangelist for A/D */
	const struct comedi_lrange *ao_range_list;	/*  rangelist for D/A */
	unsigned int ai_acquisition_period_min_ns;
};

static const struct pci9111_board pci9111_boards[] = {
	{
	 .name = "pci9111_hr",
	 .device_id = PCI9111_HR_DEVICE_ID,
	 .ai_channel_nbr = PCI9111_AI_CHANNEL_NBR,
	 .ao_channel_nbr = PCI9111_AO_CHANNEL_NBR,
	 .ai_resolution = PCI9111_HR_AI_RESOLUTION,
	 .ai_resolution_mask = PCI9111_HR_AI_RESOLUTION_MASK,
	 .ao_resolution = PCI9111_AO_RESOLUTION,
	 .ao_resolution_mask = PCI9111_AO_RESOLUTION_MASK,
	 .ai_range_list = &pci9111_hr_ai_range,
	 .ao_range_list = &range_bipolar10,
	 .ai_acquisition_period_min_ns = PCI9111_AI_ACQUISITION_PERIOD_MIN_NS}
};

#define pci9111_board_nbr \
	(sizeof(pci9111_boards)/sizeof(struct pci9111_board))

static struct comedi_driver pci9111_driver = {
	.driver_name = PCI9111_DRIVER_NAME,
	.module = THIS_MODULE,
	.attach = pci9111_attach,
	.detach = pci9111_detach,
};

static int __devinit pci9111_driver_pci_probe(struct pci_dev *dev,
					      const struct pci_device_id *ent)
{
	return comedi_pci_auto_config(dev, pci9111_driver.driver_name);
}

static void __devexit pci9111_driver_pci_remove(struct pci_dev *dev)
{
	comedi_pci_auto_unconfig(dev);
}

static struct pci_driver pci9111_driver_pci_driver = {
	.id_table = pci9111_pci_table,
	.probe = &pci9111_driver_pci_probe,
	.remove = __devexit_p(&pci9111_driver_pci_remove)
};

static int __init pci9111_driver_init_module(void)
{
	int retval;

	retval = comedi_driver_register(&pci9111_driver);
	if (retval < 0)
		return retval;

	pci9111_driver_pci_driver.name = (char *)pci9111_driver.driver_name;
	return pci_register_driver(&pci9111_driver_pci_driver);
}

static void __exit pci9111_driver_cleanup_module(void)
{
	pci_unregister_driver(&pci9111_driver_pci_driver);
	comedi_driver_unregister(&pci9111_driver);
}

module_init(pci9111_driver_init_module);
module_exit(pci9111_driver_cleanup_module);

/*  Private data structure */

struct pci9111_private_data {
	struct pci_dev *pci_device;
	unsigned long io_range;	/*  PCI6503 io range */

	unsigned long lcr_io_base; /* Local configuration register base
				    * address */
	unsigned long lcr_io_range;
=======
/*
 * IO address map and bit defines
 */
#define PCI9111_AI_FIFO_REG		0x00
#define PCI9111_AO_REG			0x00
#define PCI9111_DIO_REG			0x02
#define PCI9111_EDIO_REG		0x04
#define PCI9111_AI_CHANNEL_REG		0x06
#define PCI9111_AI_RANGE_STAT_REG	0x08
#define PCI9111_AI_STAT_AD_BUSY		(1 << 7)
#define PCI9111_AI_STAT_FF_FF		(1 << 6)
#define PCI9111_AI_STAT_FF_HF		(1 << 5)
#define PCI9111_AI_STAT_FF_EF		(1 << 4)
#define PCI9111_AI_RANGE_MASK		(7 << 0)
#define PCI9111_AI_TRIG_CTRL_REG	0x0a
#define PCI9111_AI_TRIG_CTRL_TRGEVENT	(1 << 5)
#define PCI9111_AI_TRIG_CTRL_POTRG	(1 << 4)
#define PCI9111_AI_TRIG_CTRL_PTRG	(1 << 3)
#define PCI9111_AI_TRIG_CTRL_ETIS	(1 << 2)
#define PCI9111_AI_TRIG_CTRL_TPST	(1 << 1)
#define PCI9111_AI_TRIG_CTRL_ASCAN	(1 << 0)
#define PCI9111_INT_CTRL_REG		0x0c
#define PCI9111_INT_CTRL_ISC2		(1 << 3)
#define PCI9111_INT_CTRL_FFEN		(1 << 2)
#define PCI9111_INT_CTRL_ISC1		(1 << 1)
#define PCI9111_INT_CTRL_ISC0		(1 << 0)
#define PCI9111_SOFT_TRIG_REG		0x0e
#define PCI9111_8254_BASE_REG		0x40
#define PCI9111_INT_CLR_REG		0x48

/* PLX 9052 Local Interrupt 1 enabled and active */
#define PCI9111_LI1_ACTIVE	(PLX9052_INTCSR_LI1ENAB |	\
				 PLX9052_INTCSR_LI1STAT)

/* PLX 9052 Local Interrupt 2 enabled and active */
#define PCI9111_LI2_ACTIVE	(PLX9052_INTCSR_LI2ENAB |	\
				 PLX9052_INTCSR_LI2STAT)

static const struct comedi_lrange pci9111_ai_range = {
	5, {
		BIP_RANGE(10),
		BIP_RANGE(5),
		BIP_RANGE(2.5),
		BIP_RANGE(1.25),
		BIP_RANGE(0.625)
	}
};

struct pci9111_private_data {
	unsigned long lcr_io_base;
>>>>>>> refs/remotes/origin/master

	int stop_counter;
	int stop_is_none;

	unsigned int scan_delay;
	unsigned int chanlist_len;
	unsigned int chunk_counter;
	unsigned int chunk_num_samples;

<<<<<<< HEAD
	int ao_readback;	/*  Last written analog output data */

	unsigned int timer_divisor_1; /* Divisor values for the 8254 timer
				       * pacer */
	unsigned int timer_divisor_2;

	int is_valid;		/*  Is device valid */

	short ai_bounce_buffer[2 * PCI9111_FIFO_HALF_SIZE];
};

#define dev_private	((struct pci9111_private_data *)dev->private)

/*  ------------------------------------------------------------------ */
/*  PLX9050 SECTION */
/*  ------------------------------------------------------------------ */

#define PLX9050_REGISTER_INTERRUPT_CONTROL 0x4c

#define PLX9050_LINTI1_ENABLE		(1 << 0)
#define PLX9050_LINTI1_ACTIVE_HIGH	(1 << 1)
#define PLX9050_LINTI1_STATUS		(1 << 2)
#define PLX9050_LINTI2_ENABLE		(1 << 3)
#define PLX9050_LINTI2_ACTIVE_HIGH	(1 << 4)
#define PLX9050_LINTI2_STATUS		(1 << 5)
#define PLX9050_PCI_INTERRUPT_ENABLE	(1 << 6)
#define PLX9050_SOFTWARE_INTERRUPT	(1 << 7)

=======
	int ao_readback;

	unsigned int div1;
	unsigned int div2;

	unsigned short ai_bounce_buffer[2 * PCI9111_FIFO_HALF_SIZE];
};

>>>>>>> refs/remotes/origin/master
static void plx9050_interrupt_control(unsigned long io_base,
				      bool LINTi1_enable,
				      bool LINTi1_active_high,
				      bool LINTi2_enable,
				      bool LINTi2_active_high,
				      bool interrupt_enable)
{
	int flags = 0;

	if (LINTi1_enable)
<<<<<<< HEAD
		flags |= PLX9050_LINTI1_ENABLE;
	if (LINTi1_active_high)
		flags |= PLX9050_LINTI1_ACTIVE_HIGH;
	if (LINTi2_enable)
		flags |= PLX9050_LINTI2_ENABLE;
	if (LINTi2_active_high)
		flags |= PLX9050_LINTI2_ACTIVE_HIGH;

	if (interrupt_enable)
		flags |= PLX9050_PCI_INTERRUPT_ENABLE;

	outb(flags, io_base + PLX9050_REGISTER_INTERRUPT_CONTROL);
}

/*  ------------------------------------------------------------------ */
/*  MISCELLANEOUS SECTION */
/*  ------------------------------------------------------------------ */

/*  8254 timer */

static void pci9111_timer_set(struct comedi_device *dev)
{
	pci9111_8254_control_set(PCI9111_8254_COUNTER_0 |
				 PCI9111_8254_READ_LOAD_LSB_MSB |
				 PCI9111_8254_MODE_0 |
				 PCI9111_8254_BINARY_COUNTER);

	pci9111_8254_control_set(PCI9111_8254_COUNTER_1 |
				 PCI9111_8254_READ_LOAD_LSB_MSB |
				 PCI9111_8254_MODE_2 |
				 PCI9111_8254_BINARY_COUNTER);

	pci9111_8254_control_set(PCI9111_8254_COUNTER_2 |
				 PCI9111_8254_READ_LOAD_LSB_MSB |
				 PCI9111_8254_MODE_2 |
				 PCI9111_8254_BINARY_COUNTER);

	udelay(1);

	pci9111_8254_counter_2_set(dev_private->timer_divisor_2);
	pci9111_8254_counter_1_set(dev_private->timer_divisor_1);
=======
		flags |= PLX9052_INTCSR_LI1ENAB;
	if (LINTi1_active_high)
		flags |= PLX9052_INTCSR_LI1POL;
	if (LINTi2_enable)
		flags |= PLX9052_INTCSR_LI2ENAB;
	if (LINTi2_active_high)
		flags |= PLX9052_INTCSR_LI2POL;

	if (interrupt_enable)
		flags |= PLX9052_INTCSR_PCIENAB;

	outb(flags, io_base + PLX9052_INTCSR);
}

static void pci9111_timer_set(struct comedi_device *dev)
{
	struct pci9111_private_data *dev_private = dev->private;
	unsigned long timer_base = dev->iobase + PCI9111_8254_BASE_REG;

	i8254_set_mode(timer_base, 1, 0, I8254_MODE0 | I8254_BINARY);
	i8254_set_mode(timer_base, 1, 1, I8254_MODE2 | I8254_BINARY);
	i8254_set_mode(timer_base, 1, 2, I8254_MODE2 | I8254_BINARY);

	udelay(1);

	i8254_write(timer_base, 1, 2, dev_private->div2);
	i8254_write(timer_base, 1, 1, dev_private->div1);
>>>>>>> refs/remotes/origin/master
}

enum pci9111_trigger_sources {
	software,
	timer_pacer,
	external
};

static void pci9111_trigger_source_set(struct comedi_device *dev,
				       enum pci9111_trigger_sources source)
{
	int flags;

<<<<<<< HEAD
	flags = pci9111_trigger_and_autoscan_get() & 0x09;

	switch (source) {
	case software:
		flags |= PCI9111_EITS_INTERNAL | PCI9111_TPST_SOFTWARE_TRIGGER;
		break;

	case timer_pacer:
		flags |= PCI9111_EITS_INTERNAL | PCI9111_TPST_TIMER_PACER;
		break;

	case external:
		flags |= PCI9111_EITS_EXTERNAL;
		break;
	}

	pci9111_trigger_and_autoscan_set(flags);
=======
	/* Read the current trigger mode control bits */
	flags = inb(dev->iobase + PCI9111_AI_TRIG_CTRL_REG);
	/* Mask off the EITS and TPST bits */
	flags &= 0x9;

	switch (source) {
	case software:
		break;

	case timer_pacer:
		flags |= PCI9111_AI_TRIG_CTRL_TPST;
		break;

	case external:
		flags |= PCI9111_AI_TRIG_CTRL_ETIS;
		break;
	}

	outb(flags, dev->iobase + PCI9111_AI_TRIG_CTRL_REG);
>>>>>>> refs/remotes/origin/master
}

static void pci9111_pretrigger_set(struct comedi_device *dev, bool pretrigger)
{
	int flags;

<<<<<<< HEAD
	flags = pci9111_trigger_and_autoscan_get() & 0x07;

	if (pretrigger)
		flags |= PCI9111_PTRG_ON;

	pci9111_trigger_and_autoscan_set(flags);
=======
	/* Read the current trigger mode control bits */
	flags = inb(dev->iobase + PCI9111_AI_TRIG_CTRL_REG);
	/* Mask off the PTRG bit */
	flags &= 0x7;

	if (pretrigger)
		flags |= PCI9111_AI_TRIG_CTRL_PTRG;

	outb(flags, dev->iobase + PCI9111_AI_TRIG_CTRL_REG);
>>>>>>> refs/remotes/origin/master
}

static void pci9111_autoscan_set(struct comedi_device *dev, bool autoscan)
{
	int flags;

<<<<<<< HEAD
	flags = pci9111_trigger_and_autoscan_get() & 0x0e;

	if (autoscan)
		flags |= PCI9111_ASCAN_ON;

	pci9111_trigger_and_autoscan_set(flags);
=======
	/* Read the current trigger mode control bits */
	flags = inb(dev->iobase + PCI9111_AI_TRIG_CTRL_REG);
	/* Mask off the ASCAN bit */
	flags &= 0xe;

	if (autoscan)
		flags |= PCI9111_AI_TRIG_CTRL_ASCAN;

	outb(flags, dev->iobase + PCI9111_AI_TRIG_CTRL_REG);
>>>>>>> refs/remotes/origin/master
}

enum pci9111_ISC0_sources {
	irq_on_eoc,
	irq_on_fifo_half_full
};

enum pci9111_ISC1_sources {
	irq_on_timer_tick,
	irq_on_external_trigger
};

static void pci9111_interrupt_source_set(struct comedi_device *dev,
					 enum pci9111_ISC0_sources irq_0_source,
					 enum pci9111_ISC1_sources irq_1_source)
{
	int flags;

<<<<<<< HEAD
	flags = pci9111_interrupt_and_fifo_get() & 0x04;

	if (irq_0_source == irq_on_fifo_half_full)
		flags |= PCI9111_ISC0_SET_IRQ_ON_FIFO_HALF_FULL;

	if (irq_1_source == irq_on_external_trigger)
		flags |= PCI9111_ISC1_SET_IRQ_ON_EXT_TRG;

	pci9111_interrupt_and_fifo_set(flags);
}

/*  ------------------------------------------------------------------ */
/*  HARDWARE TRIGGERED ANALOG INPUT SECTION */
/*  ------------------------------------------------------------------ */

/*  Cancel analog input autoscan */

#undef AI_DO_CMD_DEBUG
=======
	/* Read the current interrupt control bits */
	flags = inb(dev->iobase + PCI9111_AI_TRIG_CTRL_REG);
	/* Shift the bits so they are compatible with the write register */
	flags >>= 4;
	/* Mask off the ISCx bits */
	flags &= 0xc0;

	/* Now set the new ISCx bits */
	if (irq_0_source == irq_on_fifo_half_full)
		flags |= PCI9111_INT_CTRL_ISC0;

	if (irq_1_source == irq_on_external_trigger)
		flags |= PCI9111_INT_CTRL_ISC1;

	outb(flags, dev->iobase + PCI9111_INT_CTRL_REG);
}

static void pci9111_fifo_reset(struct comedi_device *dev)
{
	unsigned long int_ctrl_reg = dev->iobase + PCI9111_INT_CTRL_REG;

	/* To reset the FIFO, set FFEN sequence as 0 -> 1 -> 0 */
	outb(0, int_ctrl_reg);
	outb(PCI9111_INT_CTRL_FFEN, int_ctrl_reg);
	outb(0, int_ctrl_reg);
}
>>>>>>> refs/remotes/origin/master

static int pci9111_ai_cancel(struct comedi_device *dev,
			     struct comedi_subdevice *s)
{
<<<<<<< HEAD
	/*  Disable interrupts */

=======
	struct pci9111_private_data *dev_private = dev->private;

	/*  Disable interrupts */
>>>>>>> refs/remotes/origin/master
	plx9050_interrupt_control(dev_private->lcr_io_base, true, true, true,
				  true, false);

	pci9111_trigger_source_set(dev, software);

	pci9111_autoscan_set(dev, false);

<<<<<<< HEAD
	pci9111_fifo_reset();

#ifdef AI_DO_CMD_DEBUG
	printk(PCI9111_DRIVER_NAME ": ai_cancel\n");
#endif
=======
	pci9111_fifo_reset(dev);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
/*  Test analog input command */

#define pci9111_check_trigger_src(src, flags)	do {			\
		tmp = src;						\
		src &= flags;						\
		if (!src || tmp != src)					\
			error++;					\
	} while (false);

static int
pci9111_ai_do_cmd_test(struct comedi_device *dev,
		       struct comedi_subdevice *s, struct comedi_cmd *cmd)
{
=======
static int pci9111_ai_do_cmd_test(struct comedi_device *dev,
				  struct comedi_subdevice *s,
				  struct comedi_cmd *cmd)
{
	struct pci9111_private_data *dev_private = dev->private;
>>>>>>> refs/remotes/origin/master
	int tmp;
	int error = 0;
	int range, reference;
	int i;
<<<<<<< HEAD
	struct pci9111_board *board = (struct pci9111_board *)dev->board_ptr;

	/*  Step 1 : check if trigger are trivialy valid */

	pci9111_check_trigger_src(cmd->start_src, TRIG_NOW);
	pci9111_check_trigger_src(cmd->scan_begin_src,
				  TRIG_TIMER | TRIG_FOLLOW | TRIG_EXT);
	pci9111_check_trigger_src(cmd->convert_src, TRIG_TIMER | TRIG_EXT);
	pci9111_check_trigger_src(cmd->scan_end_src, TRIG_COUNT);
	pci9111_check_trigger_src(cmd->stop_src, TRIG_COUNT | TRIG_NONE);
=======

	/* Step 1 : check if triggers are trivially valid */

	error |= cfc_check_trigger_src(&cmd->start_src, TRIG_NOW);
	error |= cfc_check_trigger_src(&cmd->scan_begin_src,
					TRIG_TIMER | TRIG_FOLLOW | TRIG_EXT);
	error |= cfc_check_trigger_src(&cmd->convert_src,
					TRIG_TIMER | TRIG_EXT);
	error |= cfc_check_trigger_src(&cmd->scan_end_src, TRIG_COUNT);
	error |= cfc_check_trigger_src(&cmd->stop_src,
					TRIG_COUNT | TRIG_NONE);
>>>>>>> refs/remotes/origin/master

	if (error)
		return 1;

<<<<<<< HEAD
	/*  step 2 : make sure trigger sources are unique and mutually
	 *  compatible */

	if (cmd->start_src != TRIG_NOW)
		error++;

	if ((cmd->scan_begin_src != TRIG_TIMER) &&
	    (cmd->scan_begin_src != TRIG_FOLLOW) &&
	    (cmd->scan_begin_src != TRIG_EXT))
		error++;

	if ((cmd->convert_src != TRIG_TIMER) && (cmd->convert_src != TRIG_EXT))
		error++;
	if ((cmd->convert_src == TRIG_TIMER) &&
	    !((cmd->scan_begin_src == TRIG_TIMER) ||
	      (cmd->scan_begin_src == TRIG_FOLLOW)))
		error++;
	if ((cmd->convert_src == TRIG_EXT) &&
	    !((cmd->scan_begin_src == TRIG_EXT) ||
	      (cmd->scan_begin_src == TRIG_FOLLOW)))
		error++;


	if (cmd->scan_end_src != TRIG_COUNT)
		error++;
	if ((cmd->stop_src != TRIG_COUNT) && (cmd->stop_src != TRIG_NONE))
		error++;
=======
	/* Step 2a : make sure trigger sources are unique */

	error |= cfc_check_trigger_is_unique(cmd->scan_begin_src);
	error |= cfc_check_trigger_is_unique(cmd->convert_src);
	error |= cfc_check_trigger_is_unique(cmd->stop_src);

	/* Step 2b : and mutually compatible */

	if ((cmd->convert_src == TRIG_TIMER) &&
	    !((cmd->scan_begin_src == TRIG_TIMER) ||
	      (cmd->scan_begin_src == TRIG_FOLLOW)))
		error |= -EINVAL;
	if ((cmd->convert_src == TRIG_EXT) &&
	    !((cmd->scan_begin_src == TRIG_EXT) ||
	      (cmd->scan_begin_src == TRIG_FOLLOW)))
		error |= -EINVAL;
>>>>>>> refs/remotes/origin/master

	if (error)
		return 2;

<<<<<<< HEAD
	/*  Step 3 : make sure arguments are trivialy compatible */

	if (cmd->chanlist_len < 1) {
		cmd->chanlist_len = 1;
		error++;
	}

	if (cmd->chanlist_len > board->ai_channel_nbr) {
		cmd->chanlist_len = board->ai_channel_nbr;
		error++;
	}

	if ((cmd->start_src == TRIG_NOW) && (cmd->start_arg != 0)) {
		cmd->start_arg = 0;
		error++;
	}

	if ((cmd->convert_src == TRIG_TIMER) &&
	    (cmd->convert_arg < board->ai_acquisition_period_min_ns)) {
		cmd->convert_arg = board->ai_acquisition_period_min_ns;
		error++;
	}
	if ((cmd->convert_src == TRIG_EXT) && (cmd->convert_arg != 0)) {
		cmd->convert_arg = 0;
		error++;
	}

	if ((cmd->scan_begin_src == TRIG_TIMER) &&
	    (cmd->scan_begin_arg < board->ai_acquisition_period_min_ns)) {
		cmd->scan_begin_arg = board->ai_acquisition_period_min_ns;
		error++;
	}
	if ((cmd->scan_begin_src == TRIG_FOLLOW)
	    && (cmd->scan_begin_arg != 0)) {
		cmd->scan_begin_arg = 0;
		error++;
	}
	if ((cmd->scan_begin_src == TRIG_EXT) && (cmd->scan_begin_arg != 0)) {
		cmd->scan_begin_arg = 0;
		error++;
	}

	if ((cmd->scan_end_src == TRIG_COUNT) &&
	    (cmd->scan_end_arg != cmd->chanlist_len)) {
		cmd->scan_end_arg = cmd->chanlist_len;
		error++;
	}

	if ((cmd->stop_src == TRIG_COUNT) && (cmd->stop_arg < 1)) {
		cmd->stop_arg = 1;
		error++;
	}
	if ((cmd->stop_src == TRIG_NONE) && (cmd->stop_arg != 0)) {
		cmd->stop_arg = 0;
		error++;
	}
=======
	/* Step 3: check if arguments are trivially valid */

	error |= cfc_check_trigger_arg_is(&cmd->start_arg, 0);

	if (cmd->convert_src == TRIG_TIMER)
		error |= cfc_check_trigger_arg_min(&cmd->convert_arg,
					PCI9111_AI_ACQUISITION_PERIOD_MIN_NS);
	else	/* TRIG_EXT */
		error |= cfc_check_trigger_arg_is(&cmd->convert_arg, 0);

	if (cmd->scan_begin_src == TRIG_TIMER)
		error |= cfc_check_trigger_arg_min(&cmd->scan_begin_arg,
					PCI9111_AI_ACQUISITION_PERIOD_MIN_NS);
	else	/* TRIG_FOLLOW || TRIG_EXT */
		error |= cfc_check_trigger_arg_is(&cmd->scan_begin_arg, 0);

	error |= cfc_check_trigger_arg_is(&cmd->scan_end_arg,
					  cmd->chanlist_len);

	if (cmd->stop_src == TRIG_COUNT)
		error |= cfc_check_trigger_arg_min(&cmd->stop_arg, 1);
	else	/* TRIG_NONE */
		error |= cfc_check_trigger_arg_is(&cmd->stop_arg, 0);
>>>>>>> refs/remotes/origin/master

	if (error)
		return 3;

	/*  Step 4 : fix up any arguments */

	if (cmd->convert_src == TRIG_TIMER) {
		tmp = cmd->convert_arg;
<<<<<<< HEAD
		i8253_cascade_ns_to_timer_2div(PCI9111_8254_CLOCK_PERIOD_NS,
					       &(dev_private->timer_divisor_1),
					       &(dev_private->timer_divisor_2),
					       &(cmd->convert_arg),
					       cmd->flags & TRIG_ROUND_MASK);
=======
		i8253_cascade_ns_to_timer(I8254_OSC_BASE_2MHZ,
					  &dev_private->div1,
					  &dev_private->div2,
					  &cmd->convert_arg, cmd->flags);
>>>>>>> refs/remotes/origin/master
		if (tmp != cmd->convert_arg)
			error++;
	}
	/*  There's only one timer on this card, so the scan_begin timer must */
	/*  be a multiple of chanlist_len*convert_arg */

	if (cmd->scan_begin_src == TRIG_TIMER) {

		unsigned int scan_begin_min;
		unsigned int scan_begin_arg;
		unsigned int scan_factor;

		scan_begin_min = cmd->chanlist_len * cmd->convert_arg;

		if (cmd->scan_begin_arg != scan_begin_min) {
			if (scan_begin_min < cmd->scan_begin_arg) {
				scan_factor =
				    cmd->scan_begin_arg / scan_begin_min;
				scan_begin_arg = scan_factor * scan_begin_min;
				if (cmd->scan_begin_arg != scan_begin_arg) {
					cmd->scan_begin_arg = scan_begin_arg;
					error++;
				}
			} else {
				cmd->scan_begin_arg = scan_begin_min;
				error++;
			}
		}
	}

	if (error)
		return 4;

	/*  Step 5 : check channel list */

	if (cmd->chanlist) {

		range = CR_RANGE(cmd->chanlist[0]);
		reference = CR_AREF(cmd->chanlist[0]);

		if (cmd->chanlist_len > 1) {
			for (i = 0; i < cmd->chanlist_len; i++) {
				if (CR_CHAN(cmd->chanlist[i]) != i) {
					comedi_error(dev,
						     "entries in chanlist must be consecutive "
						     "channels,counting upwards from 0\n");
					error++;
				}
				if (CR_RANGE(cmd->chanlist[i]) != range) {
					comedi_error(dev,
						     "entries in chanlist must all have the same gain\n");
					error++;
				}
				if (CR_AREF(cmd->chanlist[i]) != reference) {
					comedi_error(dev,
						     "entries in chanlist must all have the same reference\n");
					error++;
				}
			}
<<<<<<< HEAD
		} else {
			if ((CR_CHAN(cmd->chanlist[0]) >
			     (board->ai_channel_nbr - 1))
			    || (CR_CHAN(cmd->chanlist[0]) < 0)) {
				comedi_error(dev,
					     "channel number is out of limits\n");
				error++;
			}
=======
>>>>>>> refs/remotes/origin/master
		}
	}

	if (error)
		return 5;

	return 0;

}

<<<<<<< HEAD
/*  Analog input command */

static int pci9111_ai_do_cmd(struct comedi_device *dev,
			     struct comedi_subdevice *subdevice)
{
	struct comedi_cmd *async_cmd = &subdevice->async->cmd;

	if (!dev->irq) {
		comedi_error(dev,
			     "no irq assigned for PCI9111, cannot do hardware conversion");
		return -1;
	}
=======
static int pci9111_ai_do_cmd(struct comedi_device *dev,
			     struct comedi_subdevice *s)
{
	struct pci9111_private_data *dev_private = dev->private;
	struct comedi_cmd *async_cmd = &s->async->cmd;

>>>>>>> refs/remotes/origin/master
	/*  Set channel scan limit */
	/*  PCI9111 allows only scanning from channel 0 to channel n */
	/*  TODO: handle the case of an external multiplexer */

	if (async_cmd->chanlist_len > 1) {
<<<<<<< HEAD
		pci9111_ai_channel_set((async_cmd->chanlist_len) - 1);
		pci9111_autoscan_set(dev, true);
	} else {
		pci9111_ai_channel_set(CR_CHAN(async_cmd->chanlist[0]));
=======
		outb(async_cmd->chanlist_len - 1,
			dev->iobase + PCI9111_AI_CHANNEL_REG);
		pci9111_autoscan_set(dev, true);
	} else {
		outb(CR_CHAN(async_cmd->chanlist[0]),
			dev->iobase + PCI9111_AI_CHANNEL_REG);
>>>>>>> refs/remotes/origin/master
		pci9111_autoscan_set(dev, false);
	}

	/*  Set gain */
	/*  This is the same gain on every channel */

<<<<<<< HEAD
	pci9111_ai_range_set(CR_RANGE(async_cmd->chanlist[0]));
=======
	outb(CR_RANGE(async_cmd->chanlist[0]) & PCI9111_AI_RANGE_MASK,
		dev->iobase + PCI9111_AI_RANGE_STAT_REG);
>>>>>>> refs/remotes/origin/master

	/* Set counter */

	switch (async_cmd->stop_src) {
	case TRIG_COUNT:
		dev_private->stop_counter =
		    async_cmd->stop_arg * async_cmd->chanlist_len;
		dev_private->stop_is_none = 0;
		break;

	case TRIG_NONE:
		dev_private->stop_counter = 0;
		dev_private->stop_is_none = 1;
		break;

	default:
		comedi_error(dev, "Invalid stop trigger");
		return -1;
	}

	/*  Set timer pacer */

	dev_private->scan_delay = 0;
	switch (async_cmd->convert_src) {
	case TRIG_TIMER:
<<<<<<< HEAD
		i8253_cascade_ns_to_timer_2div(PCI9111_8254_CLOCK_PERIOD_NS,
					       &(dev_private->timer_divisor_1),
					       &(dev_private->timer_divisor_2),
					       &(async_cmd->convert_arg),
					       async_cmd->
					       flags & TRIG_ROUND_MASK);
#ifdef AI_DO_CMD_DEBUG
		printk(PCI9111_DRIVER_NAME ": divisors = %d, %d\n",
		       dev_private->timer_divisor_1,
		       dev_private->timer_divisor_2);
#endif

		pci9111_trigger_source_set(dev, software);
		pci9111_timer_set(dev);
		pci9111_fifo_reset();
=======
		pci9111_trigger_source_set(dev, software);
		pci9111_timer_set(dev);
		pci9111_fifo_reset(dev);
>>>>>>> refs/remotes/origin/master
		pci9111_interrupt_source_set(dev, irq_on_fifo_half_full,
					     irq_on_timer_tick);
		pci9111_trigger_source_set(dev, timer_pacer);
		plx9050_interrupt_control(dev_private->lcr_io_base, true, true,
					  false, true, true);

		if (async_cmd->scan_begin_src == TRIG_TIMER) {
			dev_private->scan_delay =
				(async_cmd->scan_begin_arg /
				 (async_cmd->convert_arg *
				  async_cmd->chanlist_len)) - 1;
		}

		break;

	case TRIG_EXT:

		pci9111_trigger_source_set(dev, external);
<<<<<<< HEAD
		pci9111_fifo_reset();
=======
		pci9111_fifo_reset(dev);
>>>>>>> refs/remotes/origin/master
		pci9111_interrupt_source_set(dev, irq_on_fifo_half_full,
					     irq_on_timer_tick);
		plx9050_interrupt_control(dev_private->lcr_io_base, true, true,
					  false, true, true);

		break;

	default:
		comedi_error(dev, "Invalid convert trigger");
		return -1;
	}

	dev_private->stop_counter *= (1 + dev_private->scan_delay);
	dev_private->chanlist_len = async_cmd->chanlist_len;
	dev_private->chunk_counter = 0;
	dev_private->chunk_num_samples =
	    dev_private->chanlist_len * (1 + dev_private->scan_delay);

<<<<<<< HEAD
#ifdef AI_DO_CMD_DEBUG
	printk(PCI9111_DRIVER_NAME ": start interruptions!\n");
	printk(PCI9111_DRIVER_NAME ": trigger source = %2x\n",
	       pci9111_trigger_and_autoscan_get());
	printk(PCI9111_DRIVER_NAME ": irq source     = %2x\n",
	       pci9111_interrupt_and_fifo_get());
	printk(PCI9111_DRIVER_NAME ": ai_do_cmd\n");
	printk(PCI9111_DRIVER_NAME ": stop counter   = %d\n",
	       dev_private->stop_counter);
	printk(PCI9111_DRIVER_NAME ": scan delay     = %d\n",
	       dev_private->scan_delay);
	printk(PCI9111_DRIVER_NAME ": chanlist_len   = %d\n",
	       dev_private->chanlist_len);
	printk(PCI9111_DRIVER_NAME ": chunk num samples = %d\n",
	       dev_private->chunk_num_samples);
#endif

=======
>>>>>>> refs/remotes/origin/master
	return 0;
}

static void pci9111_ai_munge(struct comedi_device *dev,
			     struct comedi_subdevice *s, void *data,
			     unsigned int num_bytes,
			     unsigned int start_chan_index)
{
<<<<<<< HEAD
	unsigned int i, num_samples = num_bytes / sizeof(short);
	short *array = data;
	int resolution =
	    ((struct pci9111_board *)dev->board_ptr)->ai_resolution;

	for (i = 0; i < num_samples; i++) {
		if (resolution == PCI9111_HR_AI_RESOLUTION)
			array[i] =
			    (array[i] & PCI9111_HR_AI_RESOLUTION_MASK) ^
			    PCI9111_HR_AI_RESOLUTION_2_CMP_BIT;
		else
			array[i] =
			    ((array[i] >> 4) & PCI9111_AI_RESOLUTION_MASK) ^
			    PCI9111_AI_RESOLUTION_2_CMP_BIT;
	}
}

/*  ------------------------------------------------------------------ */
/*  INTERRUPT SECTION */
/*  ------------------------------------------------------------------ */

#undef INTERRUPT_DEBUG

static irqreturn_t pci9111_interrupt(int irq, void *p_device)
{
	struct comedi_device *dev = p_device;
	struct comedi_subdevice *subdevice = dev->read_subdev;
	struct comedi_async *async;
=======
	unsigned short *array = data;
	unsigned int maxdata = s->maxdata;
	unsigned int invert = (maxdata + 1) >> 1;
	unsigned int shift = (maxdata == 0xffff) ? 0 : 4;
	unsigned int num_samples = num_bytes / sizeof(short);
	unsigned int i;

	for (i = 0; i < num_samples; i++)
		array[i] = ((array[i] >> shift) & maxdata) ^ invert;
}

static irqreturn_t pci9111_interrupt(int irq, void *p_device)
{
	struct comedi_device *dev = p_device;
	struct pci9111_private_data *dev_private = dev->private;
	struct comedi_subdevice *s = dev->read_subdev;
	struct comedi_async *async;
	unsigned int status;
>>>>>>> refs/remotes/origin/master
	unsigned long irq_flags;
	unsigned char intcsr;

	if (!dev->attached) {
		/*  Ignore interrupt before device fully attached. */
		/*  Might not even have allocated subdevices yet! */
		return IRQ_NONE;
	}

<<<<<<< HEAD
	async = subdevice->async;
=======
	async = s->async;
>>>>>>> refs/remotes/origin/master

	spin_lock_irqsave(&dev->spinlock, irq_flags);

	/*  Check if we are source of interrupt */
<<<<<<< HEAD
	intcsr = inb(dev_private->lcr_io_base +
		     PLX9050_REGISTER_INTERRUPT_CONTROL);
	if (!(((intcsr & PLX9050_PCI_INTERRUPT_ENABLE) != 0)
	      && (((intcsr & (PLX9050_LINTI1_ENABLE | PLX9050_LINTI1_STATUS))
		   == (PLX9050_LINTI1_ENABLE | PLX9050_LINTI1_STATUS))
		  || ((intcsr & (PLX9050_LINTI2_ENABLE | PLX9050_LINTI2_STATUS))
		      == (PLX9050_LINTI2_ENABLE | PLX9050_LINTI2_STATUS))))) {
		/*  Not the source of the interrupt. */
		/*  (N.B. not using PLX9050_SOFTWARE_INTERRUPT) */
=======
	intcsr = inb(dev_private->lcr_io_base + PLX9052_INTCSR);
	if (!(((intcsr & PLX9052_INTCSR_PCIENAB) != 0) &&
	      (((intcsr & PCI9111_LI1_ACTIVE) == PCI9111_LI1_ACTIVE) ||
	       ((intcsr & PCI9111_LI2_ACTIVE) == PCI9111_LI2_ACTIVE)))) {
		/*  Not the source of the interrupt. */
		/*  (N.B. not using PLX9052_INTCSR_SOFTINT) */
>>>>>>> refs/remotes/origin/master
		spin_unlock_irqrestore(&dev->spinlock, irq_flags);
		return IRQ_NONE;
	}

<<<<<<< HEAD
	if ((intcsr & (PLX9050_LINTI1_ENABLE | PLX9050_LINTI1_STATUS)) ==
	    (PLX9050_LINTI1_ENABLE | PLX9050_LINTI1_STATUS)) {
		/*  Interrupt comes from fifo_half-full signal */

		if (pci9111_is_fifo_full()) {
			spin_unlock_irqrestore(&dev->spinlock, irq_flags);
			comedi_error(dev, PCI9111_DRIVER_NAME " fifo overflow");
			pci9111_interrupt_clear();
			pci9111_ai_cancel(dev, subdevice);
			async->events |= COMEDI_CB_ERROR | COMEDI_CB_EOA;
			comedi_event(dev, subdevice);
=======
	if ((intcsr & PCI9111_LI1_ACTIVE) == PCI9111_LI1_ACTIVE) {
		/*  Interrupt comes from fifo_half-full signal */

		status = inb(dev->iobase + PCI9111_AI_RANGE_STAT_REG);

		/* '0' means FIFO is full, data may have been lost */
		if (!(status & PCI9111_AI_STAT_FF_FF)) {
			spin_unlock_irqrestore(&dev->spinlock, irq_flags);
			comedi_error(dev, PCI9111_DRIVER_NAME " fifo overflow");
			outb(0, dev->iobase + PCI9111_INT_CLR_REG);
			pci9111_ai_cancel(dev, s);
			async->events |= COMEDI_CB_ERROR | COMEDI_CB_EOA;
			comedi_event(dev, s);
>>>>>>> refs/remotes/origin/master

			return IRQ_HANDLED;
		}

<<<<<<< HEAD
		if (pci9111_is_fifo_half_full()) {
			unsigned int num_samples;
			unsigned int bytes_written = 0;

#ifdef INTERRUPT_DEBUG
			printk(PCI9111_DRIVER_NAME ": fifo is half full\n");
#endif

=======
		/* '0' means FIFO is half-full */
		if (!(status & PCI9111_AI_STAT_FF_HF)) {
			unsigned int num_samples;
			unsigned int bytes_written = 0;

>>>>>>> refs/remotes/origin/master
			num_samples =
			    PCI9111_FIFO_HALF_SIZE >
			    dev_private->stop_counter
			    && !dev_private->
			    stop_is_none ? dev_private->stop_counter :
			    PCI9111_FIFO_HALF_SIZE;
<<<<<<< HEAD
			insw(PCI9111_IO_BASE + PCI9111_REGISTER_AD_FIFO_VALUE,
=======
			insw(dev->iobase + PCI9111_AI_FIFO_REG,
>>>>>>> refs/remotes/origin/master
			     dev_private->ai_bounce_buffer, num_samples);

			if (dev_private->scan_delay < 1) {
				bytes_written =
<<<<<<< HEAD
				    cfc_write_array_to_buffer(subdevice,
=======
				    cfc_write_array_to_buffer(s,
>>>>>>> refs/remotes/origin/master
							      dev_private->
							      ai_bounce_buffer,
							      num_samples *
							      sizeof(short));
			} else {
				int position = 0;
				int to_read;

				while (position < num_samples) {
					if (dev_private->chunk_counter <
					    dev_private->chanlist_len) {
						to_read =
						    dev_private->chanlist_len -
						    dev_private->chunk_counter;

						if (to_read >
						    num_samples - position)
							to_read =
							    num_samples -
							    position;

						bytes_written +=
						    cfc_write_array_to_buffer
<<<<<<< HEAD
						    (subdevice,
=======
						    (s,
>>>>>>> refs/remotes/origin/master
						     dev_private->ai_bounce_buffer
						     + position,
						     to_read * sizeof(short));
					} else {
						to_read =
						    dev_private->chunk_num_samples
						    -
						    dev_private->chunk_counter;
						if (to_read >
						    num_samples - position)
							to_read =
							    num_samples -
							    position;

						bytes_written +=
						    sizeof(short) * to_read;
					}

					position += to_read;
					dev_private->chunk_counter += to_read;

					if (dev_private->chunk_counter >=
					    dev_private->chunk_num_samples)
						dev_private->chunk_counter = 0;
				}
			}

			dev_private->stop_counter -=
			    bytes_written / sizeof(short);
		}
	}

	if ((dev_private->stop_counter == 0) && (!dev_private->stop_is_none)) {
		async->events |= COMEDI_CB_EOA;
<<<<<<< HEAD
		pci9111_ai_cancel(dev, subdevice);
	}

	/* Very important, otherwise another interrupt request will be inserted
	 * and will cause driver hangs on processing interrupt event. */

	pci9111_interrupt_clear();

	spin_unlock_irqrestore(&dev->spinlock, irq_flags);

	comedi_event(dev, subdevice);
=======
		pci9111_ai_cancel(dev, s);
	}

	outb(0, dev->iobase + PCI9111_INT_CLR_REG);

	spin_unlock_irqrestore(&dev->spinlock, irq_flags);

	comedi_event(dev, s);
>>>>>>> refs/remotes/origin/master

	return IRQ_HANDLED;
}

<<<<<<< HEAD
/*  ------------------------------------------------------------------ */
/*  INSTANT ANALOG INPUT OUTPUT SECTION */
/*  ------------------------------------------------------------------ */

/*  analog instant input */

#undef AI_INSN_DEBUG

static int pci9111_ai_insn_read(struct comedi_device *dev,
				struct comedi_subdevice *subdevice,
				struct comedi_insn *insn, unsigned int *data)
{
	int resolution =
	    ((struct pci9111_board *)dev->board_ptr)->ai_resolution;

	int timeout, i;

#ifdef AI_INSN_DEBUG
	printk(PCI9111_DRIVER_NAME ": ai_insn set c/r/n = %2x/%2x/%2x\n",
	       CR_CHAN((&insn->chanspec)[0]),
	       CR_RANGE((&insn->chanspec)[0]), insn->n);
#endif

	pci9111_ai_channel_set(CR_CHAN((&insn->chanspec)[0]));

	if ((pci9111_ai_range_get()) != CR_RANGE((&insn->chanspec)[0]))
		pci9111_ai_range_set(CR_RANGE((&insn->chanspec)[0]));

	pci9111_fifo_reset();

	for (i = 0; i < insn->n; i++) {
		pci9111_software_trigger();
=======
static int pci9111_ai_insn_read(struct comedi_device *dev,
				struct comedi_subdevice *s,
				struct comedi_insn *insn, unsigned int *data)
{
	unsigned int chan = CR_CHAN(insn->chanspec);
	unsigned int range = CR_RANGE(insn->chanspec);
	unsigned int maxdata = s->maxdata;
	unsigned int invert = (maxdata + 1) >> 1;
	unsigned int shift = (maxdata == 0xffff) ? 0 : 4;
	unsigned int status;
	int timeout;
	int i;

	outb(chan, dev->iobase + PCI9111_AI_CHANNEL_REG);

	status = inb(dev->iobase + PCI9111_AI_RANGE_STAT_REG);
	if ((status & PCI9111_AI_RANGE_MASK) != range) {
		outb(range & PCI9111_AI_RANGE_MASK,
			dev->iobase + PCI9111_AI_RANGE_STAT_REG);
	}

	pci9111_fifo_reset(dev);

	for (i = 0; i < insn->n; i++) {
		/* Generate a software trigger */
		outb(0, dev->iobase + PCI9111_SOFT_TRIG_REG);
>>>>>>> refs/remotes/origin/master

		timeout = PCI9111_AI_INSTANT_READ_TIMEOUT;

		while (timeout--) {
<<<<<<< HEAD
			if (!pci9111_is_fifo_empty())
=======
			status = inb(dev->iobase + PCI9111_AI_RANGE_STAT_REG);
			/* '1' means FIFO is not empty */
			if (status & PCI9111_AI_STAT_FF_EF)
>>>>>>> refs/remotes/origin/master
				goto conversion_done;
		}

		comedi_error(dev, "A/D read timeout");
		data[i] = 0;
<<<<<<< HEAD
		pci9111_fifo_reset();
=======
		pci9111_fifo_reset(dev);
>>>>>>> refs/remotes/origin/master
		return -ETIME;

conversion_done:

<<<<<<< HEAD
		if (resolution == PCI9111_HR_AI_RESOLUTION)
			data[i] = pci9111_hr_ai_get_data();
		else
			data[i] = pci9111_ai_get_data();
	}

#ifdef AI_INSN_DEBUG
	printk(PCI9111_DRIVER_NAME ": ai_insn get c/r/t = %2x/%2x/%2x\n",
	       pci9111_ai_channel_get(),
	       pci9111_ai_range_get(), pci9111_trigger_and_autoscan_get());
#endif

	return i;
}

/*  Analog instant output */

static int
pci9111_ao_insn_write(struct comedi_device *dev,
		      struct comedi_subdevice *s, struct comedi_insn *insn,
		      unsigned int *data)
{
	int i;

	for (i = 0; i < insn->n; i++) {
		pci9111_ao_set_data(data[i]);
		dev_private->ao_readback = data[i];
	}

	return i;
}

/*  Analog output readback */

static int pci9111_ao_insn_read(struct comedi_device *dev,
				struct comedi_subdevice *s,
				struct comedi_insn *insn, unsigned int *data)
{
	int i;

	for (i = 0; i < insn->n; i++)
		data[i] = dev_private->ao_readback & PCI9111_AO_RESOLUTION_MASK;

	return i;
}

/*  ------------------------------------------------------------------ */
/*  DIGITAL INPUT OUTPUT SECTION */
/*  ------------------------------------------------------------------ */

/*  Digital inputs */

static int pci9111_di_insn_bits(struct comedi_device *dev,
				struct comedi_subdevice *subdevice,
				struct comedi_insn *insn, unsigned int *data)
{
	unsigned int bits;

	bits = pci9111_di_get_bits();
	data[1] = bits;

	return 2;
}

/*  Digital outputs */

static int pci9111_do_insn_bits(struct comedi_device *dev,
				struct comedi_subdevice *subdevice,
				struct comedi_insn *insn, unsigned int *data)
{
	unsigned int bits;

	/*  Only set bits that have been masked */
	/*  data[0] = mask */
	/*  data[1] = bit state */

	data[0] &= PCI9111_DO_MASK;

	bits = subdevice->state;
	bits &= ~data[0];
	bits |= data[0] & data[1];
	subdevice->state = bits;

	pci9111_do_set_bits(bits);

	data[1] = bits;

	return 2;
}

/*  ------------------------------------------------------------------ */
/*  INITIALISATION SECTION */
/*  ------------------------------------------------------------------ */

/*  Reset device */

static int pci9111_reset(struct comedi_device *dev)
{
	/*  Set trigger source to software */

=======
		data[i] = inw(dev->iobase + PCI9111_AI_FIFO_REG);
		data[i] = ((data[i] >> shift) & maxdata) ^ invert;
	}

	return i;
}

static int pci9111_ao_insn_write(struct comedi_device *dev,
				 struct comedi_subdevice *s,
				 struct comedi_insn *insn,
				 unsigned int *data)
{
	struct pci9111_private_data *dev_private = dev->private;
	unsigned int val = 0;
	int i;

	for (i = 0; i < insn->n; i++) {
		val = data[i];
		outw(val, dev->iobase + PCI9111_AO_REG);
	}
	dev_private->ao_readback = val;

	return insn->n;
}

static int pci9111_ao_insn_read(struct comedi_device *dev,
				struct comedi_subdevice *s,
				struct comedi_insn *insn,
				unsigned int *data)
{
	struct pci9111_private_data *dev_private = dev->private;
	int i;

	for (i = 0; i < insn->n; i++)
		data[i] = dev_private->ao_readback;

	return insn->n;
}

static int pci9111_di_insn_bits(struct comedi_device *dev,
				struct comedi_subdevice *s,
				struct comedi_insn *insn,
				unsigned int *data)
{
	data[1] = inw(dev->iobase + PCI9111_DIO_REG);

	return insn->n;
}

static int pci9111_do_insn_bits(struct comedi_device *dev,
				struct comedi_subdevice *s,
				struct comedi_insn *insn,
				unsigned int *data)
{
	if (comedi_dio_update_state(s, data))
		outw(s->state, dev->iobase + PCI9111_DIO_REG);

	data[1] = s->state;

	return insn->n;
}

static int pci9111_reset(struct comedi_device *dev)
{
	struct pci9111_private_data *dev_private = dev->private;

	/*  Set trigger source to software */
>>>>>>> refs/remotes/origin/master
	plx9050_interrupt_control(dev_private->lcr_io_base, true, true, true,
				  true, false);

	pci9111_trigger_source_set(dev, software);
	pci9111_pretrigger_set(dev, false);
	pci9111_autoscan_set(dev, false);

<<<<<<< HEAD
	/*  Reset 8254 chip */

	dev_private->timer_divisor_1 = 0;
	dev_private->timer_divisor_2 = 0;

=======
	/* Reset 8254 chip */
	dev_private->div1 = 0;
	dev_private->div2 = 0;
>>>>>>> refs/remotes/origin/master
	pci9111_timer_set(dev);

	return 0;
}

<<<<<<< HEAD
/*  Attach */
/*       - Register PCI device */
/*       - Declare device driver capability */

static int pci9111_attach(struct comedi_device *dev,
			  struct comedi_devconfig *it)
{
	struct comedi_subdevice *subdevice;
	unsigned long io_base, io_range, lcr_io_base, lcr_io_range;
	struct pci_dev *pci_device = NULL;
	int error, i;
	const struct pci9111_board *board;

	if (alloc_private(dev, sizeof(struct pci9111_private_data)) < 0)
		return -ENOMEM;
	/*  Probe the device to determine what device in the series it is. */

	printk(KERN_ERR "comedi%d: " PCI9111_DRIVER_NAME " driver\n",
								dev->minor);

	for_each_pci_dev(pci_device) {
		if (pci_device->vendor == PCI_VENDOR_ID_ADLINK) {
			for (i = 0; i < pci9111_board_nbr; i++) {
				if (pci9111_boards[i].device_id ==
				    pci_device->device) {
					/* was a particular bus/slot
					 * requested? */
					if ((it->options[0] != 0)
					    || (it->options[1] != 0)) {
						/* are we on the wrong
						 * bus/slot? */
						if (pci_device->bus->number !=
						    it->options[0]
						    ||
						    PCI_SLOT(pci_device->devfn)
						    != it->options[1]) {
							continue;
						}
					}

					dev->board_ptr = pci9111_boards + i;
					board =
					    (struct pci9111_board *)
					    dev->board_ptr;
					dev_private->pci_device = pci_device;
					goto found;
				}
			}
		}
	}

	printk(KERN_ERR
		"comedi%d: no supported board found! (req. bus/slot : %d/%d)\n",
			dev->minor, it->options[0], it->options[1]);
	return -EIO;

found:

	printk(KERN_ERR "comedi%d: found %s (b:s:f=%d:%d:%d) , irq=%d\n",
	       dev->minor,
	       pci9111_boards[i].name,
	       pci_device->bus->number,
	       PCI_SLOT(pci_device->devfn),
	       PCI_FUNC(pci_device->devfn), pci_device->irq);

	/*  TODO: Warn about non-tested boards. */

	/*  Read local configuration register base address
	 *  [PCI_BASE_ADDRESS #1]. */

	lcr_io_base = pci_resource_start(pci_device, 1);
	lcr_io_range = pci_resource_len(pci_device, 1);

	printk
	    ("comedi%d: local configuration registers at address 0x%4lx [0x%4lx]\n",
	     dev->minor, lcr_io_base, lcr_io_range);

	/*  Enable PCI device and request regions */
	if (comedi_pci_enable(pci_device, PCI9111_DRIVER_NAME) < 0) {
		printk
		    ("comedi%d: Failed to enable PCI device and request regions\n",
		     dev->minor);
		return -EIO;
	}
	/*  Read PCI6308 register base address [PCI_BASE_ADDRESS #2]. */

	io_base = pci_resource_start(pci_device, 2);
	io_range = pci_resource_len(pci_device, 2);

	printk(KERN_ERR "comedi%d: 6503 registers at address 0x%4lx [0x%4lx]\n",
	       dev->minor, io_base, io_range);

	dev->iobase = io_base;
	dev->board_name = board->name;
	dev_private->io_range = io_range;
	dev_private->is_valid = 0;
	dev_private->lcr_io_base = lcr_io_base;
	dev_private->lcr_io_range = lcr_io_range;

	pci9111_reset(dev);

	/*  Irq setup */

	dev->irq = 0;
	if (pci_device->irq > 0) {
		if (request_irq(pci_device->irq, pci9111_interrupt,
				IRQF_SHARED, PCI9111_DRIVER_NAME, dev) != 0) {
			printk(KERN_ERR
				"comedi%d: unable to allocate irq  %u\n",
					dev->minor, pci_device->irq);
			return -EINVAL;
		}
	}
	dev->irq = pci_device->irq;

	/*  TODO: Add external multiplexer setup (according to option[2]). */

	error = alloc_subdevices(dev, 4);
	if (error < 0)
		return error;

	subdevice = dev->subdevices + 0;
	dev->read_subdev = subdevice;

	subdevice->type = COMEDI_SUBD_AI;
	subdevice->subdev_flags = SDF_READABLE | SDF_COMMON | SDF_CMD_READ;

	/*  TODO: Add external multiplexer data */
	/*     if (devpriv->usemux) { subdevice->n_chan = devpriv->usemux; } */
	/*     else { subdevice->n_chan = this_board->n_aichan; } */

	subdevice->n_chan = board->ai_channel_nbr;
	subdevice->maxdata = board->ai_resolution_mask;
	subdevice->len_chanlist = board->ai_channel_nbr;
	subdevice->range_table = board->ai_range_list;
	subdevice->cancel = pci9111_ai_cancel;
	subdevice->insn_read = pci9111_ai_insn_read;
	subdevice->do_cmdtest = pci9111_ai_do_cmd_test;
	subdevice->do_cmd = pci9111_ai_do_cmd;
	subdevice->munge = pci9111_ai_munge;

	subdevice = dev->subdevices + 1;
	subdevice->type = COMEDI_SUBD_AO;
	subdevice->subdev_flags = SDF_WRITABLE | SDF_COMMON;
	subdevice->n_chan = board->ao_channel_nbr;
	subdevice->maxdata = board->ao_resolution_mask;
	subdevice->len_chanlist = board->ao_channel_nbr;
	subdevice->range_table = board->ao_range_list;
	subdevice->insn_write = pci9111_ao_insn_write;
	subdevice->insn_read = pci9111_ao_insn_read;

	subdevice = dev->subdevices + 2;
	subdevice->type = COMEDI_SUBD_DI;
	subdevice->subdev_flags = SDF_READABLE;
	subdevice->n_chan = PCI9111_DI_CHANNEL_NBR;
	subdevice->maxdata = 1;
	subdevice->range_table = &range_digital;
	subdevice->insn_bits = pci9111_di_insn_bits;

	subdevice = dev->subdevices + 3;
	subdevice->type = COMEDI_SUBD_DO;
	subdevice->subdev_flags = SDF_READABLE | SDF_WRITABLE;
	subdevice->n_chan = PCI9111_DO_CHANNEL_NBR;
	subdevice->maxdata = 1;
	subdevice->range_table = &range_digital;
	subdevice->insn_bits = pci9111_do_insn_bits;

	dev_private->is_valid = 1;
=======
static int pci9111_auto_attach(struct comedi_device *dev,
					 unsigned long context_unused)
{
	struct pci_dev *pcidev = comedi_to_pci_dev(dev);
	struct pci9111_private_data *dev_private;
	struct comedi_subdevice *s;
	int ret;

	dev_private = comedi_alloc_devpriv(dev, sizeof(*dev_private));
	if (!dev_private)
		return -ENOMEM;

	ret = comedi_pci_enable(dev);
	if (ret)
		return ret;
	dev_private->lcr_io_base = pci_resource_start(pcidev, 1);
	dev->iobase = pci_resource_start(pcidev, 2);

	pci9111_reset(dev);

	if (pcidev->irq) {
		ret = request_irq(pcidev->irq, pci9111_interrupt,
				  IRQF_SHARED, dev->board_name, dev);
		if (ret == 0)
			dev->irq = pcidev->irq;
	}

	ret = comedi_alloc_subdevices(dev, 4);
	if (ret)
		return ret;

	s = &dev->subdevices[0];
	s->type		= COMEDI_SUBD_AI;
	s->subdev_flags	= SDF_READABLE | SDF_COMMON;
	s->n_chan	= 16;
	s->maxdata	= 0xffff;
	s->range_table	= &pci9111_ai_range;
	s->insn_read	= pci9111_ai_insn_read;
	if (dev->irq) {
		dev->read_subdev = s;
		s->subdev_flags	|= SDF_CMD_READ;
		s->len_chanlist	= s->n_chan;
		s->do_cmdtest	= pci9111_ai_do_cmd_test;
		s->do_cmd	= pci9111_ai_do_cmd;
		s->cancel	= pci9111_ai_cancel;
		s->munge	= pci9111_ai_munge;
	}

	s = &dev->subdevices[1];
	s->type		= COMEDI_SUBD_AO;
	s->subdev_flags	= SDF_WRITABLE | SDF_COMMON;
	s->n_chan	= 1;
	s->maxdata	= 0x0fff;
	s->len_chanlist	= 1;
	s->range_table	= &range_bipolar10;
	s->insn_write	= pci9111_ao_insn_write;
	s->insn_read	= pci9111_ao_insn_read;

	s = &dev->subdevices[2];
	s->type		= COMEDI_SUBD_DI;
	s->subdev_flags	= SDF_READABLE;
	s->n_chan	= 16;
	s->maxdata	= 1;
	s->range_table	= &range_digital;
	s->insn_bits	= pci9111_di_insn_bits;

	s = &dev->subdevices[3];
	s->type		= COMEDI_SUBD_DO;
	s->subdev_flags	= SDF_READABLE | SDF_WRITABLE;
	s->n_chan	= 16;
	s->maxdata	= 1;
	s->range_table	= &range_digital;
	s->insn_bits	= pci9111_do_insn_bits;

	dev_info(dev->class_dev, "%s attached\n", dev->board_name);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
/*  Detach */

static int pci9111_detach(struct comedi_device *dev)
{
	/*  Reset device */

	if (dev->private != NULL) {
		if (dev_private->is_valid)
			pci9111_reset(dev);

	}
	/*  Release previously allocated irq */

	if (dev->irq != 0)
		free_irq(dev->irq, dev);

	if (dev_private != NULL && dev_private->pci_device != NULL) {
		if (dev->iobase)
			comedi_pci_disable(dev_private->pci_device);
		pci_dev_put(dev_private->pci_device);
	}

	return 0;
}

=======
static void pci9111_detach(struct comedi_device *dev)
{
	if (dev->iobase)
		pci9111_reset(dev);
	if (dev->irq != 0)
		free_irq(dev->irq, dev);
	comedi_pci_disable(dev);
}

static struct comedi_driver adl_pci9111_driver = {
	.driver_name	= "adl_pci9111",
	.module		= THIS_MODULE,
	.auto_attach	= pci9111_auto_attach,
	.detach		= pci9111_detach,
};

static int pci9111_pci_probe(struct pci_dev *dev,
			     const struct pci_device_id *id)
{
	return comedi_pci_auto_config(dev, &adl_pci9111_driver,
				      id->driver_data);
}

static const struct pci_device_id pci9111_pci_table[] = {
	{ PCI_DEVICE(PCI_VENDOR_ID_ADLINK, PCI9111_HR_DEVICE_ID) },
	/* { PCI_DEVICE(PCI_VENDOR_ID_ADLINK, PCI9111_HG_DEVICE_ID) }, */
	{ 0 }
};
MODULE_DEVICE_TABLE(pci, pci9111_pci_table);

static struct pci_driver adl_pci9111_pci_driver = {
	.name		= "adl_pci9111",
	.id_table	= pci9111_pci_table,
	.probe		= pci9111_pci_probe,
	.remove		= comedi_pci_auto_unconfig,
};
module_comedi_pci_driver(adl_pci9111_driver, adl_pci9111_pci_driver);

>>>>>>> refs/remotes/origin/master
MODULE_AUTHOR("Comedi http://www.comedi.org");
MODULE_DESCRIPTION("Comedi low-level driver");
MODULE_LICENSE("GPL");
