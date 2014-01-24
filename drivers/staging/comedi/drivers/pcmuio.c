/*
<<<<<<< HEAD
    comedi/drivers/pcmuio.c
    Driver for Winsystems PC-104 based 48-channel and 96-channel DIO boards.

    COMEDI - Linux Control and Measurement Device Interface
    Copyright (C) 2006 Calin A. Culianu <calin@ajvar.org>

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
Driver: pcmuio
Description: A driver for the PCM-UIO48A and PCM-UIO96A boards from Winsystems.
Devices: [Winsystems] PCM-UIO48A (pcmuio48), PCM-UIO96A (pcmuio96)
Author: Calin Culianu <calin@ajvar.org>
Updated: Fri, 13 Jan 2006 12:01:01 -0500
Status: works

A driver for the relatively straightforward-to-program PCM-UIO48A and
PCM-UIO96A boards from Winsystems.  These boards use either one or two
(in the 96-DIO version) WS16C48 ASIC HighDensity I/O Chips (HDIO).
This chip is interesting in that each I/O line is individually
programmable for INPUT or OUTPUT (thus comedi_dio_config can be done
on a per-channel basis).  Also, each chip supports edge-triggered
interrupts for the first 24 I/O lines.  Of course, since the
96-channel version of the board has two ASICs, it can detect polarity
changes on up to 48 I/O lines.  Since this is essentially an (non-PnP)
ISA board, I/O Address and IRQ selection are done through jumpers on
the board.  You need to pass that information to this driver as the
first and second comedi_config option, respectively.  Note that the
48-channel version uses 16 bytes of IO memory and the 96-channel
version uses 32-bytes (in case you are worried about conflicts).  The
48-channel board is split into two 24-channel comedi subdevices.
The 96-channel board is split into 4 24-channel DIO subdevices.

Note that IRQ support has been added, but it is untested.

To use edge-detection IRQ support, pass the IRQs of both ASICS
(for the 96 channel version) or just 1 ASIC (for 48-channel version).
Then, use use comedi_commands with TRIG_NOW.
Your callback will be called each time an edge is triggered, and the data
values will be two sample_t's, which should be concatenated to form one
32-bit unsigned int.  This value is the mask of channels that had
edges detected from your channel list.  Note that the bits positions
in the mask correspond to positions in your chanlist when you specified
the command and *not* channel id's!

To set the polarity of the edge-detection interrupts pass a nonzero value for
either CR_RANGE or CR_AREF for edge-up polarity, or a zero value for both
CR_RANGE and CR_AREF if you want edge-down polarity.

In the 48-channel version:

On subdev 0, the first 24 channels channels are edge-detect channels.

In the 96-channel board you have the collowing channels that can do edge detection:

subdev 0, channels 0-24  (first 24 channels of 1st ASIC)
subdev 2, channels 0-24  (first 24 channels of 2nd ASIC)

Configuration Options:
  [0] - I/O port base address
  [1] - IRQ (for first ASIC, or first 24 channels)
  [2] - IRQ for second ASIC (pcmuio96 only - IRQ for chans 48-72 .. can be the same as first irq!)
*/

#include <linux/interrupt.h>
#include <linux/slab.h>
#include "../comedidev.h"
#include "pcm_common.h"

#include <linux/pci.h>		/* for PCI devices */

#define CHANS_PER_PORT   8
#define PORTS_PER_ASIC   6
#define INTR_PORTS_PER_ASIC   3
#define MAX_CHANS_PER_SUBDEV 24	/* number of channels per comedi subdevice */
#define PORTS_PER_SUBDEV (MAX_CHANS_PER_SUBDEV/CHANS_PER_PORT)
#define CHANS_PER_ASIC (CHANS_PER_PORT*PORTS_PER_ASIC)
#define INTR_CHANS_PER_ASIC 24
#define INTR_PORTS_PER_SUBDEV (INTR_CHANS_PER_ASIC/CHANS_PER_PORT)
#define MAX_DIO_CHANS   (PORTS_PER_ASIC*2*CHANS_PER_PORT)
#define MAX_ASICS       (MAX_DIO_CHANS/CHANS_PER_ASIC)
#define SDEV_NO ((int)(s - dev->subdevices))
#define CALC_N_SUBDEVS(nchans) ((nchans)/MAX_CHANS_PER_SUBDEV + (!!((nchans)%MAX_CHANS_PER_SUBDEV)) /*+ (nchans > INTR_CHANS_PER_ASIC ? 2 : 1)*/)
/* IO Memory sizes */
#define ASIC_IOSIZE (0x10)
#define PCMUIO48_IOSIZE ASIC_IOSIZE
#define PCMUIO96_IOSIZE (ASIC_IOSIZE*2)

/* Some offsets - these are all in the 16byte IO memory offset from
   the base address.  Note that there is a paging scheme to swap out
   offsets 0x8-0xA using the PAGELOCK register.  See the table below.

  Register(s)       Pages        R/W?        Description
  --------------------------------------------------------------
  REG_PORTx         All          R/W         Read/Write/Configure IO
  REG_INT_PENDING   All          ReadOnly    Quickly see which INT_IDx has int.
  REG_PAGELOCK      All          WriteOnly   Select a page
  REG_POLx          Pg. 1 only   WriteOnly   Select edge-detection polarity
  REG_ENABx         Pg. 2 only   WriteOnly   Enable/Disable edge-detect. int.
  REG_INT_IDx       Pg. 3 only   R/W         See which ports/bits have ints.
 */
#define REG_PORT0 0x0
#define REG_PORT1 0x1
#define REG_PORT2 0x2
#define REG_PORT3 0x3
#define REG_PORT4 0x4
#define REG_PORT5 0x5
#define REG_INT_PENDING 0x6
#define REG_PAGELOCK 0x7	/* page selector register, upper 2 bits select a page
				   and bits 0-5 are used to 'lock down' a particular
				   port above to make it readonly.  */
#define REG_POL0 0x8
#define REG_POL1 0x9
#define REG_POL2 0xA
#define REG_ENAB0 0x8
#define REG_ENAB1 0x9
#define REG_ENAB2 0xA
#define REG_INT_ID0 0x8
#define REG_INT_ID1 0x9
#define REG_INT_ID2 0xA

#define NUM_PAGED_REGS 3
#define NUM_PAGES 4
#define FIRST_PAGED_REG 0x8
#define REG_PAGE_BITOFFSET 6
#define REG_LOCK_BITOFFSET 0
#define REG_PAGE_MASK (~((0x1<<REG_PAGE_BITOFFSET)-1))
#define REG_LOCK_MASK ~(REG_PAGE_MASK)
#define PAGE_POL 1
#define PAGE_ENAB 2
#define PAGE_INT_ID 3

/*
 * Board descriptions for two imaginary boards.  Describing the
 * boards in this way is optional, and completely driver-dependent.
 * Some drivers use arrays such as this, other do not.
 */
struct pcmuio_board {
	const char *name;
	const int num_asics;
	const int num_channels_per_port;
	const int num_ports;
=======
 * pcmuio.c
 * Comedi driver for Winsystems PC-104 based 48/96-channel DIO boards.
 *
 * COMEDI - Linux Control and Measurement Device Interface
 * Copyright (C) 2006 Calin A. Culianu <calin@ajvar.org>
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
 * Driver: pcmuio
 * Description: Winsystems PC-104 based 48/96-channel DIO boards.
 * Devices: (Winsystems) PCM-UIO48A [pcmuio48]
 *	    (Winsystems) PCM-UIO96A [pcmuio96]
 * Author: Calin Culianu <calin@ajvar.org>
 * Updated: Fri, 13 Jan 2006 12:01:01 -0500
 * Status: works
 *
 * A driver for the relatively straightforward-to-program PCM-UIO48A and
 * PCM-UIO96A boards from Winsystems. These boards use either one or two
 * (in the 96-DIO version) WS16C48 ASIC HighDensity I/O Chips (HDIO). This
 * chip is interesting in that each I/O line is individually programmable
 * for INPUT or OUTPUT (thus comedi_dio_config can be done on a per-channel
 * basis). Also, each chip supports edge-triggered interrupts for the first
 * 24 I/O lines. Of course, since the 96-channel version of the board has
 * two ASICs, it can detect polarity changes on up to 48 I/O lines. Since
 * this is essentially an (non-PnP) ISA board, I/O Address and IRQ selection
 * are done through jumpers on the board. You need to pass that information
 * to this driver as the first and second comedi_config option, respectively.
 * Note that the 48-channel version uses 16 bytes of IO memory and the 96-
 * channel version uses 32-bytes (in case you are worried about conflicts).
 * The 48-channel board is split into two 24-channel comedi subdevices. The
 * 96-channel board is split into 4 24-channel DIO subdevices.
 *
 * Note that IRQ support has been added, but it is untested.
 *
 * To use edge-detection IRQ support, pass the IRQs of both ASICS (for the
 * 96 channel version) or just 1 ASIC (for 48-channel version). Then, use
 * comedi_commands with TRIG_NOW. Your callback will be called each time an
 * edge is triggered, and the data values will be two sample_t's, which
 * should be concatenated to form one 32-bit unsigned int.  This value is
 * the mask of channels that had edges detected from your channel list. Note
 * that the bits positions in the mask correspond to positions in your
 * chanlist when you specified the command and *not* channel id's!
 *
 * To set the polarity of the edge-detection interrupts pass a nonzero value
 * for either CR_RANGE or CR_AREF for edge-up polarity, or a zero value for
 * both CR_RANGE and CR_AREF if you want edge-down polarity.
 *
 * In the 48-channel version:
 *
 * On subdev 0, the first 24 channels channels are edge-detect channels.
 *
 * In the 96-channel board you have the following channels that can do edge
 * detection:
 *
 * subdev 0, channels 0-24  (first 24 channels of 1st ASIC)
 * subdev 2, channels 0-24  (first 24 channels of 2nd ASIC)
 *
 * Configuration Options:
 *  [0] - I/O port base address
 *  [1] - IRQ (for first ASIC, or first 24 channels)
 *  [2] - IRQ (for second ASIC, pcmuio96 only - IRQ for chans 48-72
 *             can be the same as first irq!)
 */

#include <linux/module.h>
#include <linux/interrupt.h>

#include "../comedidev.h"

#include "comedi_fc.h"

/*
 * Register I/O map
 *
 * Offset    Page 0       Page 1       Page 2       Page 3
 * ------  -----------  -----------  -----------  -----------
 *  0x00   Port 0 I/O   Port 0 I/O   Port 0 I/O   Port 0 I/O
 *  0x01   Port 1 I/O   Port 1 I/O   Port 1 I/O   Port 1 I/O
 *  0x02   Port 2 I/O   Port 2 I/O   Port 2 I/O   Port 2 I/O
 *  0x03   Port 3 I/O   Port 3 I/O   Port 3 I/O   Port 3 I/O
 *  0x04   Port 4 I/O   Port 4 I/O   Port 4 I/O   Port 4 I/O
 *  0x05   Port 5 I/O   Port 5 I/O   Port 5 I/O   Port 5 I/O
 *  0x06   INT_PENDING  INT_PENDING  INT_PENDING  INT_PENDING
 *  0x07    Page/Lock    Page/Lock    Page/Lock    Page/Lock
 *  0x08       N/A         POL_0       ENAB_0       INT_ID0
 *  0x09       N/A         POL_1       ENAB_1       INT_ID1
 *  0x0a       N/A         POL_2       ENAB_2       INT_ID2
 */
#define PCMUIO_PORT_REG(x)		(0x00 + (x))
#define PCMUIO_INT_PENDING_REG		0x06
#define PCMUIO_PAGE_LOCK_REG		0x07
#define PCMUIO_LOCK_PORT(x)		((1 << (x)) & 0x3f)
#define PCMUIO_PAGE(x)			(((x) & 0x3) << 6)
#define PCMUIO_PAGE_MASK		PCMUIO_PAGE(3)
#define PCMUIO_PAGE_POL			1
#define PCMUIO_PAGE_ENAB		2
#define PCMUIO_PAGE_INT_ID		3
#define PCMUIO_PAGE_REG(x)		(0x08 + (x))

#define PCMUIO_ASIC_IOSIZE		0x10
#define PCMUIO_MAX_ASICS		2

struct pcmuio_board {
	const char *name;
	const int num_asics;
>>>>>>> refs/remotes/origin/master
};

static const struct pcmuio_board pcmuio_boards[] = {
	{
<<<<<<< HEAD
	 .name = "pcmuio48",
	 .num_asics = 1,
	 .num_ports = 6,
	 },
	{
	 .name = "pcmuio96",
	 .num_asics = 2,
	 .num_ports = 12,
	 },
};

/*
 * Useful for shorthand access to the particular board structure
 */
#define thisboard ((const struct pcmuio_board *)dev->board_ptr)

/* this structure is for data unique to this subdevice.  */
struct pcmuio_subdev_private {
	/* mapping of halfwords (bytes) in port/chanarray to iobase */
	unsigned long iobases[PORTS_PER_SUBDEV];

	/* The below is only used for intr subdevices */
	struct {
		int asic;	/* if non-negative, this subdev has an interrupt asic */
		int first_chan;	/* if nonnegative, the first channel id for
				   interrupts. */
		int num_asic_chans;	/* the number of asic channels in this subdev
					   that have interrutps */
		int asic_chan;	/* if nonnegative, the first channel id with
				   respect to the asic that has interrupts */
		int enabled_mask;	/* subdev-relative channel mask for channels
					   we are interested in */
		int active;
		int stop_count;
		int continuous;
		spinlock_t spinlock;
	} intr;
};

/* this structure is for data unique to this hardware driver.  If
   several hardware drivers keep similar information in this structure,
   feel free to suggest moving the variable to the struct comedi_device struct.  */
struct pcmuio_private {
	struct {
		unsigned char pagelock;	/* current page and lock */
		unsigned char pol[NUM_PAGED_REGS];	/* shadow of POLx registers */
		unsigned char enab[NUM_PAGED_REGS];	/* shadow of ENABx registers */
		int num;
		unsigned long iobase;
		unsigned int irq;
		spinlock_t spinlock;
	} asics[MAX_ASICS];
	struct pcmuio_subdev_private *sprivs;
};

/*
 * most drivers define the following macro to make it easy to
 * access the private structure.
 */
#define devpriv ((struct pcmuio_private *)dev->private)
#define subpriv ((struct pcmuio_subdev_private *)s->private)
/*
 * The struct comedi_driver structure tells the Comedi core module
 * which functions to call to configure/deconfigure (attach/detach)
 * the board, and also about the kernel module that contains
 * the device code.
 */
static int pcmuio_attach(struct comedi_device *dev,
			 struct comedi_devconfig *it);
static int pcmuio_detach(struct comedi_device *dev);

static struct comedi_driver driver = {
	.driver_name = "pcmuio",
	.module = THIS_MODULE,
	.attach = pcmuio_attach,
	.detach = pcmuio_detach,
/* It is not necessary to implement the following members if you are
 * writing a driver for a ISA PnP or PCI card */
	/* Most drivers will support multiple types of boards by
	 * having an array of board structures.  These were defined
	 * in pcmuio_boards[] above.  Note that the element 'name'
	 * was first in the structure -- Comedi uses this fact to
	 * extract the name of the board without knowing any details
	 * about the structure except for its length.
	 * When a device is attached (by comedi_config), the name
	 * of the device is given to Comedi, and Comedi tries to
	 * match it by going through the list of board names.  If
	 * there is a match, the address of the pointer is put
	 * into dev->board_ptr and driver->attach() is called.
	 *
	 * Note that these are not necessary if you can determine
	 * the type of board in software.  ISA PnP, PCI, and PCMCIA
	 * devices are such boards.
	 */
	.board_name = &pcmuio_boards[0].name,
	.offset = sizeof(struct pcmuio_board),
	.num_names = ARRAY_SIZE(pcmuio_boards),
};

static int pcmuio_dio_insn_bits(struct comedi_device *dev,
				struct comedi_subdevice *s,
				struct comedi_insn *insn, unsigned int *data);
static int pcmuio_dio_insn_config(struct comedi_device *dev,
				  struct comedi_subdevice *s,
				  struct comedi_insn *insn, unsigned int *data);

static irqreturn_t interrupt_pcmuio(int irq, void *d);
static void pcmuio_stop_intr(struct comedi_device *, struct comedi_subdevice *);
static int pcmuio_cancel(struct comedi_device *dev, struct comedi_subdevice *s);
static int pcmuio_cmd(struct comedi_device *dev, struct comedi_subdevice *s);
static int pcmuio_cmdtest(struct comedi_device *dev, struct comedi_subdevice *s,
			  struct comedi_cmd *cmd);

/* some helper functions to deal with specifics of this device's registers */
static void init_asics(struct comedi_device *dev);	/* sets up/clears ASIC chips to defaults */
static void switch_page(struct comedi_device *dev, int asic, int page);
#ifdef notused
static void lock_port(struct comedi_device *dev, int asic, int port);
static void unlock_port(struct comedi_device *dev, int asic, int port);
#endif

/*
 * Attach is called by the Comedi core to configure the driver
 * for a particular board.  If you specified a board_name array
 * in the driver structure, dev->board_ptr contains that
 * address.
 */
static int pcmuio_attach(struct comedi_device *dev, struct comedi_devconfig *it)
{
	struct comedi_subdevice *s;
	int sdev_no, chans_left, n_subdevs, port, asic, thisasic_chanct = 0;
	unsigned long iobase;
	unsigned int irq[MAX_ASICS];

	iobase = it->options[0];
	irq[0] = it->options[1];
	irq[1] = it->options[2];

<<<<<<< HEAD
	printk("comedi%d: %s: io: %lx ", dev->minor, driver.driver_name,
	       iobase);
=======
	dev_dbg(dev->hw_dev, "comedi%d: %s: io: %lx attached\n", dev->minor,
		driver.driver_name, iobase);
>>>>>>> refs/remotes/origin/cm-10.0

	dev->iobase = iobase;

	if (!iobase || !request_region(iobase,
				       thisboard->num_asics * ASIC_IOSIZE,
				       driver.driver_name)) {
<<<<<<< HEAD
		printk("I/O port conflict\n");
=======
		dev_err(dev->hw_dev, "I/O port conflict\n");
>>>>>>> refs/remotes/origin/cm-10.0
		return -EIO;
	}

/*
 * Initialize dev->board_name.  Note that we can use the "thisboard"
 * macro now, since we just initialized it in the last line.
 */
	dev->board_name = thisboard->name;

/*
 * Allocate the private structure area.  alloc_private() is a
 * convenient macro defined in comedidev.h.
 */
	if (alloc_private(dev, sizeof(struct pcmuio_private)) < 0) {
<<<<<<< HEAD
		printk("cannot allocate private data structure\n");
=======
		dev_warn(dev->hw_dev, "cannot allocate private data structure\n");
>>>>>>> refs/remotes/origin/cm-10.0
		return -ENOMEM;
	}

	for (asic = 0; asic < MAX_ASICS; ++asic) {
		devpriv->asics[asic].num = asic;
		devpriv->asics[asic].iobase = dev->iobase + asic * ASIC_IOSIZE;
		devpriv->asics[asic].irq = 0;	/* this gets actually set at the end of
						   this function when we
						   request_irqs */
		spin_lock_init(&devpriv->asics[asic].spinlock);
	}

	chans_left = CHANS_PER_ASIC * thisboard->num_asics;
	n_subdevs = CALC_N_SUBDEVS(chans_left);
	devpriv->sprivs =
	    kcalloc(n_subdevs, sizeof(struct pcmuio_subdev_private),
		    GFP_KERNEL);
	if (!devpriv->sprivs) {
<<<<<<< HEAD
		printk("cannot allocate subdevice private data structures\n");
=======
		dev_warn(dev->hw_dev, "cannot allocate subdevice private data structures\n");
>>>>>>> refs/remotes/origin/cm-10.0
		return -ENOMEM;
	}
	/*
	 * Allocate the subdevice structures.  alloc_subdevice() is a
	 * convenient macro defined in comedidev.h.
	 *
	 * Allocate 2 subdevs (32 + 16 DIO lines) or 3 32 DIO subdevs for the
	 * 96-channel version of the board.
	 */
	if (alloc_subdevices(dev, n_subdevs) < 0) {
<<<<<<< HEAD
		printk("cannot allocate subdevice data structures\n");
=======
		dev_dbg(dev->hw_dev, "cannot allocate subdevice data structures\n");
>>>>>>> refs/remotes/origin/cm-10.0
		return -ENOMEM;
	}

	port = 0;
	asic = 0;
	for (sdev_no = 0; sdev_no < (int)dev->n_subdevices; ++sdev_no) {
		int byte_no;

		s = dev->subdevices + sdev_no;
		s->private = devpriv->sprivs + sdev_no;
		s->maxdata = 1;
		s->range_table = &range_digital;
		s->subdev_flags = SDF_READABLE | SDF_WRITABLE;
		s->type = COMEDI_SUBD_DIO;
		s->insn_bits = pcmuio_dio_insn_bits;
		s->insn_config = pcmuio_dio_insn_config;
		s->n_chan = min(chans_left, MAX_CHANS_PER_SUBDEV);
		subpriv->intr.asic = -1;
		subpriv->intr.first_chan = -1;
		subpriv->intr.asic_chan = -1;
		subpriv->intr.num_asic_chans = -1;
		subpriv->intr.active = 0;
		s->len_chanlist = 1;

		/* save the ioport address for each 'port' of 8 channels in the
		   subdevice */
		for (byte_no = 0; byte_no < PORTS_PER_SUBDEV; ++byte_no, ++port) {
			if (port >= PORTS_PER_ASIC) {
				port = 0;
				++asic;
				thisasic_chanct = 0;
			}
			subpriv->iobases[byte_no] =
			    devpriv->asics[asic].iobase + port;

			if (thisasic_chanct <
			    CHANS_PER_PORT * INTR_PORTS_PER_ASIC
			    && subpriv->intr.asic < 0) {
				/* this is an interrupt subdevice, so setup the struct */
				subpriv->intr.asic = asic;
				subpriv->intr.active = 0;
				subpriv->intr.stop_count = 0;
				subpriv->intr.first_chan = byte_no * 8;
				subpriv->intr.asic_chan = thisasic_chanct;
				subpriv->intr.num_asic_chans =
				    s->n_chan - subpriv->intr.first_chan;
				dev->read_subdev = s;
				s->subdev_flags |= SDF_CMD_READ;
				s->cancel = pcmuio_cancel;
				s->do_cmd = pcmuio_cmd;
				s->do_cmdtest = pcmuio_cmdtest;
				s->len_chanlist = subpriv->intr.num_asic_chans;
			}
			thisasic_chanct += CHANS_PER_PORT;
		}
		spin_lock_init(&subpriv->intr.spinlock);

		chans_left -= s->n_chan;

		if (!chans_left) {
			asic = 0;	/* reset the asic to our first asic, to do intr subdevs */
			port = 0;
		}

	}

	init_asics(dev);	/* clear out all the registers, basically */

	for (asic = 0; irq[0] && asic < MAX_ASICS; ++asic) {
		if (irq[asic]
		    && request_irq(irq[asic], interrupt_pcmuio,
				   IRQF_SHARED, thisboard->name, dev)) {
			int i;
			/* unroll the allocated irqs.. */
			for (i = asic - 1; i >= 0; --i) {
				free_irq(irq[i], dev);
				devpriv->asics[i].irq = irq[i] = 0;
			}
			irq[asic] = 0;
		}
		devpriv->asics[asic].irq = irq[asic];
	}

	dev->irq = irq[0];	/* grr.. wish comedi dev struct supported multiple
				   irqs.. */

	if (irq[0]) {
<<<<<<< HEAD
		printk("irq: %u ", irq[0]);
		if (irq[1] && thisboard->num_asics == 2)
			printk("second ASIC irq: %u ", irq[1]);
	} else {
		printk("(IRQ mode disabled) ");
	}

	printk("attached\n");
=======
		dev_dbg(dev->hw_dev, "irq: %u\n", irq[0]);
		if (irq[1] && thisboard->num_asics == 2)
			dev_dbg(dev->hw_dev, "second ASIC irq: %u\n", irq[1]);
	} else {
		dev_dbg(dev->hw_dev, "(IRQ mode disabled)\n");
	}

>>>>>>> refs/remotes/origin/cm-10.0

	return 1;
}

/*
 * _detach is called to deconfigure a device.  It should deallocate
 * resources.
 * This function is also called when _attach() fails, so it should be
 * careful not to release resources that were not necessarily
 * allocated by _attach().  dev->private and dev->subdevices are
 * deallocated automatically by the core.
 */
static int pcmuio_detach(struct comedi_device *dev)
{
	int i;

<<<<<<< HEAD
	printk("comedi%d: %s: remove\n", dev->minor, driver.driver_name);
	if (dev->iobase)
		release_region(dev->iobase, ASIC_IOSIZE * thisboard->num_asics);

	if (devpriv) {
		for (i = 0; i < MAX_ASICS; ++i) {
			if (devpriv->asics[i].irq)
				free_irq(devpriv->asics[i].irq, dev);
		}
		kfree(devpriv->sprivs);
	}

=======
	dev_dbg(dev->hw_dev, "comedi%d: %s: remove\n", dev->minor,
		driver.driver_name);
	if (dev->iobase)
		release_region(dev->iobase, ASIC_IOSIZE * thisboard->num_asics);

	if (devpriv) {
		for (i = 0; i < MAX_ASICS; ++i) {
			if (devpriv->asics[i].irq)
				free_irq(devpriv->asics[i].irq, dev);
		}
		kfree(devpriv->sprivs);
	}

>>>>>>> refs/remotes/origin/cm-10.0
	return 0;
}

/* DIO devices are slightly special.  Although it is possible to
 * implement the insn_read/insn_write interface, it is much more
 * useful to applications if you implement the insn_bits interface.
 * This allows packed reading/writing of the DIO channels.  The
 * comedi core can convert between insn_bits and insn_read/write */
static int pcmuio_dio_insn_bits(struct comedi_device *dev,
				struct comedi_subdevice *s,
				struct comedi_insn *insn, unsigned int *data)
{
	int byte_no;
	if (insn->n != 2)
		return -EINVAL;

	/* NOTE:
	   reading a 0 means this channel was high
	   writine a 0 sets the channel high
	   reading a 1 means this channel was low
	   writing a 1 means set this channel low

	   Therefore everything is always inverted. */

	/* The insn data is a mask in data[0] and the new data
	 * in data[1], each channel cooresponding to a bit. */

#ifdef DAMMIT_ITS_BROKEN
	/* DEBUG */
<<<<<<< HEAD
	printk("write mask: %08x  data: %08x\n", data[0], data[1]);
=======
	dev_dbg(dev->hw_dev, "write mask: %08x  data: %08x\n", data[0],
		data[1]);
>>>>>>> refs/remotes/origin/cm-10.0
#endif

	s->state = 0;

	for (byte_no = 0; byte_no < s->n_chan / CHANS_PER_PORT; ++byte_no) {
		/* address of 8-bit port */
		unsigned long ioaddr = subpriv->iobases[byte_no],
		    /* bit offset of port in 32-bit doubleword */
		    offset = byte_no * 8;
		/* this 8-bit port's data */
		unsigned char byte = 0,
		    /* The write mask for this port (if any) */
		    write_mask_byte = (data[0] >> offset) & 0xff,
		    /* The data byte for this port */
		    data_byte = (data[1] >> offset) & 0xff;

		byte = inb(ioaddr);	/* read all 8-bits for this port */

#ifdef DAMMIT_ITS_BROKEN
		/* DEBUG */
		printk
		    ("byte %d wmb %02x db %02x offset %02d io %04x, data_in %02x ",
		     byte_no, (unsigned)write_mask_byte, (unsigned)data_byte,
		     offset, ioaddr, (unsigned)byte);
#endif

		if (write_mask_byte) {
			/* this byte has some write_bits -- so set the output lines */
			byte &= ~write_mask_byte;	/* clear bits for write mask */
			byte |= ~data_byte & write_mask_byte;	/* set to inverted data_byte */
			/* Write out the new digital output state */
			outb(byte, ioaddr);
		}
#ifdef DAMMIT_ITS_BROKEN
		/* DEBUG */
<<<<<<< HEAD
		printk("data_out_byte %02x\n", (unsigned)byte);
=======
		dev_dbg(dev->hw_dev, "data_out_byte %02x\n", (unsigned)byte);
>>>>>>> refs/remotes/origin/cm-10.0
#endif
		/* save the digital input lines for this byte.. */
		s->state |= ((unsigned int)byte) << offset;
	}

	/* now return the DIO lines to data[1] - note they came inverted! */
	data[1] = ~s->state;

#ifdef DAMMIT_ITS_BROKEN
	/* DEBUG */
<<<<<<< HEAD
	printk("s->state %08x data_out %08x\n", s->state, data[1]);
=======
	dev_dbg(dev->hw_dev, "s->state %08x data_out %08x\n", s->state,
		data[1]);
>>>>>>> refs/remotes/origin/cm-10.0
#endif

	return 2;
}

/* The input or output configuration of each digital line is
 * configured by a special insn_config instruction.  chanspec
 * contains the channel to be changed, and data[0] contains the
 * value COMEDI_INPUT or COMEDI_OUTPUT. */
static int pcmuio_dio_insn_config(struct comedi_device *dev,
				  struct comedi_subdevice *s,
				  struct comedi_insn *insn, unsigned int *data)
{
	int chan = CR_CHAN(insn->chanspec), byte_no = chan / 8, bit_no =
	    chan % 8;
	unsigned long ioaddr;
	unsigned char byte;

	/* Compute ioaddr for this channel */
	ioaddr = subpriv->iobases[byte_no];

	/* NOTE:
	   writing a 0 an IO channel's bit sets the channel to INPUT
	   and pulls the line high as well

	   writing a 1 to an IO channel's  bit pulls the line low

	   All channels are implicitly always in OUTPUT mode -- but when
	   they are high they can be considered to be in INPUT mode..

	   Thus, we only force channels low if the config request was INPUT,
	   otherwise we do nothing to the hardware.    */

	switch (data[0]) {
	case INSN_CONFIG_DIO_OUTPUT:
		/* save to io_bits -- don't actually do anything since
		   all input channels are also output channels... */
		s->io_bits |= 1 << chan;
		break;
	case INSN_CONFIG_DIO_INPUT:
		/* write a 0 to the actual register representing the channel
		   to set it to 'input'.  0 means "float high". */
		byte = inb(ioaddr);
		byte &= ~(1 << bit_no);
				/**< set input channel to '0' */

		/* write out byte -- this is the only time we actually affect the
		   hardware as all channels are implicitly output -- but input
		   channels are set to float-high */
		outb(byte, ioaddr);

		/* save to io_bits */
		s->io_bits &= ~(1 << chan);
		break;

	case INSN_CONFIG_DIO_QUERY:
		/* retrieve from shadow register */
		data[1] =
		    (s->io_bits & (1 << chan)) ? COMEDI_OUTPUT : COMEDI_INPUT;
		return insn->n;
		break;

	default:
		return -EINVAL;
		break;
	}

	return insn->n;
}

static void init_asics(struct comedi_device *dev)
{				/* sets up an
				   ASIC chip to defaults */
	int asic;

	for (asic = 0; asic < thisboard->num_asics; ++asic) {
		int port, page;
		unsigned long baseaddr = dev->iobase + asic * ASIC_IOSIZE;

		switch_page(dev, asic, 0);	/* switch back to page 0 */

		/* first, clear all the DIO port bits */
		for (port = 0; port < PORTS_PER_ASIC; ++port)
			outb(0, baseaddr + REG_PORT0 + port);

		/* Next, clear all the paged registers for each page */
		for (page = 1; page < NUM_PAGES; ++page) {
			int reg;
			/* now clear all the paged registers */
			switch_page(dev, asic, page);
			for (reg = FIRST_PAGED_REG;
			     reg < FIRST_PAGED_REG + NUM_PAGED_REGS; ++reg)
				outb(0, baseaddr + reg);
		}

		/* DEBUG  set rising edge interrupts on port0 of both asics */
		/*switch_page(dev, asic, PAGE_POL);
		   outb(0xff, baseaddr + REG_POL0);
		   switch_page(dev, asic, PAGE_ENAB);
		   outb(0xff, baseaddr + REG_ENAB0); */
		/* END DEBUG */

		switch_page(dev, asic, 0);	/* switch back to default page 0 */

	}
}

static void switch_page(struct comedi_device *dev, int asic, int page)
{
	if (asic < 0 || asic >= thisboard->num_asics)
		return;		/* paranoia */
	if (page < 0 || page >= NUM_PAGES)
		return;		/* more paranoia */

	devpriv->asics[asic].pagelock &= ~REG_PAGE_MASK;
	devpriv->asics[asic].pagelock |= page << REG_PAGE_BITOFFSET;

	/* now write out the shadow register */
	outb(devpriv->asics[asic].pagelock,
	     dev->iobase + ASIC_IOSIZE * asic + REG_PAGELOCK);
}

#ifdef notused
static void lock_port(struct comedi_device *dev, int asic, int port)
{
	if (asic < 0 || asic >= thisboard->num_asics)
		return;		/* paranoia */
	if (port < 0 || port >= PORTS_PER_ASIC)
		return;		/* more paranoia */

	devpriv->asics[asic].pagelock |= 0x1 << port;
	/* now write out the shadow register */
	outb(devpriv->asics[asic].pagelock,
	     dev->iobase + ASIC_IOSIZE * asic + REG_PAGELOCK);
}

static void unlock_port(struct comedi_device *dev, int asic, int port)
{
	if (asic < 0 || asic >= thisboard->num_asics)
		return;		/* paranoia */
	if (port < 0 || port >= PORTS_PER_ASIC)
		return;		/* more paranoia */
	devpriv->asics[asic].pagelock &= ~(0x1 << port) | REG_LOCK_MASK;
	/* now write out the shadow register */
	outb(devpriv->asics[asic].pagelock,
	     dev->iobase + ASIC_IOSIZE * asic + REG_PAGELOCK);
}
#endif /* notused */

static irqreturn_t interrupt_pcmuio(int irq, void *d)
{
	int asic, got1 = 0;
	struct comedi_device *dev = (struct comedi_device *)d;

	for (asic = 0; asic < MAX_ASICS; ++asic) {
		if (irq == devpriv->asics[asic].irq) {
			unsigned long flags;
			unsigned triggered = 0;
			unsigned long iobase = devpriv->asics[asic].iobase;
			/* it is an interrupt for ASIC #asic */
			unsigned char int_pend;

			spin_lock_irqsave(&devpriv->asics[asic].spinlock,
					  flags);

			int_pend = inb(iobase + REG_INT_PENDING) & 0x07;

			if (int_pend) {
				int port;
				for (port = 0; port < INTR_PORTS_PER_ASIC;
				     ++port) {
					if (int_pend & (0x1 << port)) {
						unsigned char
						    io_lines_with_edges = 0;
						switch_page(dev, asic,
							    PAGE_INT_ID);
						io_lines_with_edges =
						    inb(iobase +
							REG_INT_ID0 + port);

						if (io_lines_with_edges)
							/* clear pending interrupt */
							outb(0, iobase +
							     REG_INT_ID0 +
							     port);

						triggered |=
						    io_lines_with_edges <<
						    port * 8;
					}
				}

				++got1;
			}

			spin_unlock_irqrestore(&devpriv->asics[asic].spinlock,
					       flags);

			if (triggered) {
				struct comedi_subdevice *s;
				/* TODO here: dispatch io lines to subdevs with commands.. */
				printk
				    ("PCMUIO DEBUG: got edge detect interrupt %d asic %d which_chans: %06x\n",
				     irq, asic, triggered);
				for (s = dev->subdevices;
				     s < dev->subdevices + dev->n_subdevices;
				     ++s) {
					if (subpriv->intr.asic == asic) {	/* this is an interrupt subdev, and it matches this asic! */
						unsigned long flags;
						unsigned oldevents;

						spin_lock_irqsave(&subpriv->
								  intr.spinlock,
								  flags);

						oldevents = s->async->events;

						if (subpriv->intr.active) {
							unsigned mytrig =
							    ((triggered >>
							      subpriv->intr.asic_chan)
							     &
							     ((0x1 << subpriv->
							       intr.
							       num_asic_chans) -
							      1)) << subpriv->
							    intr.first_chan;
							if (mytrig &
							    subpriv->intr.enabled_mask)
							{
								unsigned int val
								    = 0;
								unsigned int n,
								    ch, len;

								len =
								    s->
								    async->cmd.chanlist_len;
								for (n = 0;
								     n < len;
								     n++) {
									ch = CR_CHAN(s->async->cmd.chanlist[n]);
									if (mytrig & (1U << ch)) {
										val |= (1U << n);
									}
								}
								/* Write the scan to the buffer. */
								if (comedi_buf_put(s->async, ((short *)&val)[0])
								    &&
								    comedi_buf_put
								    (s->async,
								     ((short *)
								      &val)[1]))
								{
									s->async->events |= (COMEDI_CB_BLOCK | COMEDI_CB_EOS);
								} else {
									/* Overflow! Stop acquisition!! */
									/* TODO: STOP_ACQUISITION_CALL_HERE!! */
									pcmuio_stop_intr
									    (dev,
									     s);
								}

								/* Check for end of acquisition. */
								if (!subpriv->intr.continuous) {
									/* stop_src == TRIG_COUNT */
									if (subpriv->intr.stop_count > 0) {
										subpriv->intr.stop_count--;
										if (subpriv->intr.stop_count == 0) {
											s->async->events |= COMEDI_CB_EOA;
											/* TODO: STOP_ACQUISITION_CALL_HERE!! */
											pcmuio_stop_intr
											    (dev,
											     s);
										}
									}
								}
							}
						}

						spin_unlock_irqrestore
						    (&subpriv->intr.spinlock,
						     flags);

						if (oldevents !=
						    s->async->events) {
							comedi_event(dev, s);
						}

					}

				}
			}

		}
	}
	if (!got1)
		return IRQ_NONE;	/* interrupt from other source */
	return IRQ_HANDLED;
}

static void pcmuio_stop_intr(struct comedi_device *dev,
			     struct comedi_subdevice *s)
{
	int nports, firstport, asic, port;

	asic = subpriv->intr.asic;
	if (asic < 0)
		return;		/* not an interrupt subdev */

	subpriv->intr.enabled_mask = 0;
	subpriv->intr.active = 0;
	s->async->inttrig = 0;
	nports = subpriv->intr.num_asic_chans / CHANS_PER_PORT;
	firstport = subpriv->intr.asic_chan / CHANS_PER_PORT;
	switch_page(dev, asic, PAGE_ENAB);
	for (port = firstport; port < firstport + nports; ++port) {
		/* disable all intrs for this subdev.. */
		outb(0, devpriv->asics[asic].iobase + REG_ENAB0 + port);
	}
}

static int pcmuio_start_intr(struct comedi_device *dev,
			     struct comedi_subdevice *s)
{
	if (!subpriv->intr.continuous && subpriv->intr.stop_count == 0) {
		/* An empty acquisition! */
		s->async->events |= COMEDI_CB_EOA;
		subpriv->intr.active = 0;
		return 1;
	} else {
		unsigned bits = 0, pol_bits = 0, n;
		int nports, firstport, asic, port;
		struct comedi_cmd *cmd = &s->async->cmd;

		asic = subpriv->intr.asic;
		if (asic < 0)
			return 1;	/* not an interrupt
					   subdev */
		subpriv->intr.enabled_mask = 0;
		subpriv->intr.active = 1;
		nports = subpriv->intr.num_asic_chans / CHANS_PER_PORT;
		firstport = subpriv->intr.asic_chan / CHANS_PER_PORT;
		if (cmd->chanlist) {
			for (n = 0; n < cmd->chanlist_len; n++) {
				bits |= (1U << CR_CHAN(cmd->chanlist[n]));
				pol_bits |= (CR_AREF(cmd->chanlist[n])
					     || CR_RANGE(cmd->
							 chanlist[n]) ? 1U : 0U)
				    << CR_CHAN(cmd->chanlist[n]);
			}
		}
		bits &= ((0x1 << subpriv->intr.num_asic_chans) -
			 1) << subpriv->intr.first_chan;
		subpriv->intr.enabled_mask = bits;

		switch_page(dev, asic, PAGE_ENAB);
		for (port = firstport; port < firstport + nports; ++port) {
			unsigned enab =
			    bits >> (subpriv->intr.first_chan + (port -
								 firstport) *
				     8) & 0xff, pol =
			    pol_bits >> (subpriv->intr.first_chan +
					 (port - firstport) * 8) & 0xff;
			/* set enab intrs for this subdev.. */
			outb(enab,
			     devpriv->asics[asic].iobase + REG_ENAB0 + port);
			switch_page(dev, asic, PAGE_POL);
			outb(pol,
			     devpriv->asics[asic].iobase + REG_ENAB0 + port);
		}
	}
=======
		.name		= "pcmuio48",
		.num_asics	= 1,
	}, {
		.name		= "pcmuio96",
		.num_asics	= 2,
	},
};

struct pcmuio_asic {
	spinlock_t pagelock;	/* protects the page registers */
	spinlock_t spinlock;	/* protects member variables */
	unsigned int enabled_mask;
	unsigned int stop_count;
	unsigned int active:1;
	unsigned int continuous:1;
};

struct pcmuio_private {
	struct pcmuio_asic asics[PCMUIO_MAX_ASICS];
	unsigned int irq2;
};

static inline unsigned long pcmuio_asic_iobase(struct comedi_device *dev,
					       int asic)
{
	return dev->iobase + (asic * PCMUIO_ASIC_IOSIZE);
}

static inline int pcmuio_subdevice_to_asic(struct comedi_subdevice *s)
{
	/*
	 * subdevice 0 and 1 are handled by the first asic
	 * subdevice 2 and 3 are handled by the second asic
	 */
	return s->index / 2;
}

static inline int pcmuio_subdevice_to_port(struct comedi_subdevice *s)
{
	/*
	 * subdevice 0 and 2 use port registers 0-2
	 * subdevice 1 and 3 use port registers 3-5
	 */
	return (s->index % 2) ? 3 : 0;
}

static void pcmuio_write(struct comedi_device *dev, unsigned int val,
			 int asic, int page, int port)
{
	struct pcmuio_private *devpriv = dev->private;
	struct pcmuio_asic *chip = &devpriv->asics[asic];
	unsigned long iobase = pcmuio_asic_iobase(dev, asic);
	unsigned long flags;

	spin_lock_irqsave(&chip->pagelock, flags);
	if (page == 0) {
		/* Port registers are valid for any page */
		outb(val & 0xff, iobase + PCMUIO_PORT_REG(port + 0));
		outb((val >> 8) & 0xff, iobase + PCMUIO_PORT_REG(port + 1));
		outb((val >> 16) & 0xff, iobase + PCMUIO_PORT_REG(port + 2));
	} else {
		outb(PCMUIO_PAGE(page), iobase + PCMUIO_PAGE_LOCK_REG);
		outb(val & 0xff, iobase + PCMUIO_PAGE_REG(0));
		outb((val >> 8) & 0xff, iobase + PCMUIO_PAGE_REG(1));
		outb((val >> 16) & 0xff, iobase + PCMUIO_PAGE_REG(2));
	}
	spin_unlock_irqrestore(&chip->pagelock, flags);
}

static unsigned int pcmuio_read(struct comedi_device *dev,
				int asic, int page, int port)
{
	struct pcmuio_private *devpriv = dev->private;
	struct pcmuio_asic *chip = &devpriv->asics[asic];
	unsigned long iobase = pcmuio_asic_iobase(dev, asic);
	unsigned long flags;
	unsigned int val;

	spin_lock_irqsave(&chip->pagelock, flags);
	if (page == 0) {
		/* Port registers are valid for any page */
		val = inb(iobase + PCMUIO_PORT_REG(port + 0));
		val |= (inb(iobase + PCMUIO_PORT_REG(port + 1)) << 8);
		val |= (inb(iobase + PCMUIO_PORT_REG(port + 2)) << 16);
	} else {
		outb(PCMUIO_PAGE(page), iobase + PCMUIO_PAGE_LOCK_REG);
		val = inb(iobase + PCMUIO_PAGE_REG(0));
		val |= (inb(iobase + PCMUIO_PAGE_REG(1)) << 8);
		val |= (inb(iobase + PCMUIO_PAGE_REG(2)) << 16);
	}
	spin_unlock_irqrestore(&chip->pagelock, flags);

	return val;
}

/*
 * Each channel can be individually programmed for input or output.
 * Writing a '0' to a channel causes the corresponding output pin
 * to go to a high-z state (pulled high by an external 10K resistor).
 * This allows it to be used as an input. When used in the input mode,
 * a read reflects the inverted state of the I/O pin, such that a
 * high on the pin will read as a '0' in the register. Writing a '1'
 * to a bit position causes the pin to sink current (up to 12mA),
 * effectively pulling it low.
 */
static int pcmuio_dio_insn_bits(struct comedi_device *dev,
				struct comedi_subdevice *s,
				struct comedi_insn *insn,
				unsigned int *data)
{
	int asic = pcmuio_subdevice_to_asic(s);
	int port = pcmuio_subdevice_to_port(s);
	unsigned int chanmask = (1 << s->n_chan) - 1;
	unsigned int mask;
	unsigned int val;

	mask = comedi_dio_update_state(s, data);
	if (mask) {
		/*
		 * Outputs are inverted, invert the state and
		 * update the channels.
		 *
		 * The s->io_bits mask makes sure the input channels
		 * are '0' so that the outputs pins stay in a high
		 * z-state.
		 */
		val = ~s->state & chanmask;
		val &= s->io_bits;
		pcmuio_write(dev, val, asic, 0, port);
	}

	/* get inverted state of the channels from the port */
	val = pcmuio_read(dev, asic, 0, port);

	/* return the true state of the channels */
	data[1] = ~val & chanmask;

	return insn->n;
}

static int pcmuio_dio_insn_config(struct comedi_device *dev,
				  struct comedi_subdevice *s,
				  struct comedi_insn *insn,
				  unsigned int *data)
{
	int asic = pcmuio_subdevice_to_asic(s);
	int port = pcmuio_subdevice_to_port(s);
	int ret;

	ret = comedi_dio_insn_config(dev, s, insn, data, 0);
	if (ret)
		return ret;

	if (data[0] == INSN_CONFIG_DIO_INPUT)
		pcmuio_write(dev, s->io_bits, asic, 0, port);

	return insn->n;
}

static void pcmuio_reset(struct comedi_device *dev)
{
	const struct pcmuio_board *board = comedi_board(dev);
	int asic;

	for (asic = 0; asic < board->num_asics; ++asic) {
		/* first, clear all the DIO port bits */
		pcmuio_write(dev, 0, asic, 0, 0);
		pcmuio_write(dev, 0, asic, 0, 3);

		/* Next, clear all the paged registers for each page */
		pcmuio_write(dev, 0, asic, PCMUIO_PAGE_POL, 0);
		pcmuio_write(dev, 0, asic, PCMUIO_PAGE_ENAB, 0);
		pcmuio_write(dev, 0, asic, PCMUIO_PAGE_INT_ID, 0);
	}
}

/* chip->spinlock is already locked */
static void pcmuio_stop_intr(struct comedi_device *dev,
			     struct comedi_subdevice *s)
{
	struct pcmuio_private *devpriv = dev->private;
	int asic = pcmuio_subdevice_to_asic(s);
	struct pcmuio_asic *chip = &devpriv->asics[asic];

	chip->enabled_mask = 0;
	chip->active = 0;
	s->async->inttrig = NULL;

	/* disable all intrs for this subdev.. */
	pcmuio_write(dev, 0, asic, PCMUIO_PAGE_ENAB, 0);
}

static void pcmuio_handle_intr_subdev(struct comedi_device *dev,
				      struct comedi_subdevice *s,
				      unsigned triggered)
{
	struct pcmuio_private *devpriv = dev->private;
	int asic = pcmuio_subdevice_to_asic(s);
	struct pcmuio_asic *chip = &devpriv->asics[asic];
	unsigned int len = s->async->cmd.chanlist_len;
	unsigned oldevents = s->async->events;
	unsigned int val = 0;
	unsigned long flags;
	unsigned int i;

	spin_lock_irqsave(&chip->spinlock, flags);

	if (!chip->active)
		goto done;

	if (!(triggered & chip->enabled_mask))
		goto done;

	for (i = 0; i < len; i++) {
		unsigned int chan = CR_CHAN(s->async->cmd.chanlist[i]);
		if (triggered & (1 << chan))
			val |= (1 << i);
	}

	/* Write the scan to the buffer. */
	if (comedi_buf_put(s->async, val) &&
	    comedi_buf_put(s->async, val >> 16)) {
		s->async->events |= (COMEDI_CB_BLOCK | COMEDI_CB_EOS);
	} else {
		/* Overflow! Stop acquisition!! */
		/* TODO: STOP_ACQUISITION_CALL_HERE!! */
		pcmuio_stop_intr(dev, s);
	}

	/* Check for end of acquisition. */
	if (!chip->continuous) {
		/* stop_src == TRIG_COUNT */
		if (chip->stop_count > 0) {
			chip->stop_count--;
			if (chip->stop_count == 0) {
				s->async->events |= COMEDI_CB_EOA;
				/* TODO: STOP_ACQUISITION_CALL_HERE!! */
				pcmuio_stop_intr(dev, s);
			}
		}
	}

done:
	spin_unlock_irqrestore(&chip->spinlock, flags);

	if (oldevents != s->async->events)
		comedi_event(dev, s);
}

static int pcmuio_handle_asic_interrupt(struct comedi_device *dev, int asic)
{
	/* there are could be two asics so we can't use dev->read_subdev */
	struct comedi_subdevice *s = &dev->subdevices[asic * 2];
	unsigned long iobase = pcmuio_asic_iobase(dev, asic);
	unsigned int val;

	/* are there any interrupts pending */
	val = inb(iobase + PCMUIO_INT_PENDING_REG) & 0x07;
	if (!val)
		return 0;

	/* get, and clear, the pending interrupts */
	val = pcmuio_read(dev, asic, PCMUIO_PAGE_INT_ID, 0);
	pcmuio_write(dev, 0, asic, PCMUIO_PAGE_INT_ID, 0);

	/* handle the pending interrupts */
	pcmuio_handle_intr_subdev(dev, s, val);

	return 1;
}

static irqreturn_t pcmuio_interrupt(int irq, void *d)
{
	struct comedi_device *dev = d;
	struct pcmuio_private *devpriv = dev->private;
	int handled = 0;

	if (irq == dev->irq)
		handled += pcmuio_handle_asic_interrupt(dev, 0);
	if (irq == devpriv->irq2)
		handled += pcmuio_handle_asic_interrupt(dev, 1);

	return handled ? IRQ_HANDLED : IRQ_NONE;
}

/* chip->spinlock is already locked */
static int pcmuio_start_intr(struct comedi_device *dev,
			     struct comedi_subdevice *s)
{
	struct pcmuio_private *devpriv = dev->private;
	int asic = pcmuio_subdevice_to_asic(s);
	struct pcmuio_asic *chip = &devpriv->asics[asic];
	struct comedi_cmd *cmd = &s->async->cmd;
	unsigned int bits = 0;
	unsigned int pol_bits = 0;
	int i;

	if (!chip->continuous && chip->stop_count == 0) {
		/* An empty acquisition! */
		s->async->events |= COMEDI_CB_EOA;
		chip->active = 0;
		return 1;
	}

	chip->enabled_mask = 0;
	chip->active = 1;
	if (cmd->chanlist) {
		for (i = 0; i < cmd->chanlist_len; i++) {
			unsigned int chanspec = cmd->chanlist[i];
			unsigned int chan = CR_CHAN(chanspec);
			unsigned int range = CR_RANGE(chanspec);
			unsigned int aref = CR_AREF(chanspec);

			bits |= (1 << chan);
			pol_bits |= ((aref || range) ? 1 : 0) << chan;
		}
	}
	bits &= ((1 << s->n_chan) - 1);
	chip->enabled_mask = bits;

	/* set pol and enab intrs for this subdev.. */
	pcmuio_write(dev, pol_bits, asic, PCMUIO_PAGE_POL, 0);
	pcmuio_write(dev, bits, asic, PCMUIO_PAGE_ENAB, 0);

>>>>>>> refs/remotes/origin/master
	return 0;
}

static int pcmuio_cancel(struct comedi_device *dev, struct comedi_subdevice *s)
{
<<<<<<< HEAD
	unsigned long flags;

	spin_lock_irqsave(&subpriv->intr.spinlock, flags);
	if (subpriv->intr.active)
		pcmuio_stop_intr(dev, s);
	spin_unlock_irqrestore(&subpriv->intr.spinlock, flags);
=======
	struct pcmuio_private *devpriv = dev->private;
	int asic = pcmuio_subdevice_to_asic(s);
	struct pcmuio_asic *chip = &devpriv->asics[asic];
	unsigned long flags;

	spin_lock_irqsave(&chip->spinlock, flags);
	if (chip->active)
		pcmuio_stop_intr(dev, s);
	spin_unlock_irqrestore(&chip->spinlock, flags);
>>>>>>> refs/remotes/origin/master

	return 0;
}

/*
 * Internal trigger function to start acquisition for an 'INTERRUPT' subdevice.
 */
static int
pcmuio_inttrig_start_intr(struct comedi_device *dev, struct comedi_subdevice *s,
			  unsigned int trignum)
{
<<<<<<< HEAD
=======
	struct pcmuio_private *devpriv = dev->private;
	int asic = pcmuio_subdevice_to_asic(s);
	struct pcmuio_asic *chip = &devpriv->asics[asic];
>>>>>>> refs/remotes/origin/master
	unsigned long flags;
	int event = 0;

	if (trignum != 0)
		return -EINVAL;

<<<<<<< HEAD
	spin_lock_irqsave(&subpriv->intr.spinlock, flags);
	s->async->inttrig = 0;
<<<<<<< HEAD
	if (subpriv->intr.active) {
		event = pcmuio_start_intr(dev, s);
	}
	spin_unlock_irqrestore(&subpriv->intr.spinlock, flags);

	if (event) {
		comedi_event(dev, s);
	}
=======
	if (subpriv->intr.active)
		event = pcmuio_start_intr(dev, s);

	spin_unlock_irqrestore(&subpriv->intr.spinlock, flags);

	if (event)
		comedi_event(dev, s);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	spin_lock_irqsave(&chip->spinlock, flags);
	s->async->inttrig = NULL;
	if (chip->active)
		event = pcmuio_start_intr(dev, s);

	spin_unlock_irqrestore(&chip->spinlock, flags);

	if (event)
		comedi_event(dev, s);
>>>>>>> refs/remotes/origin/master

	return 1;
}

/*
 * 'do_cmd' function for an 'INTERRUPT' subdevice.
 */
static int pcmuio_cmd(struct comedi_device *dev, struct comedi_subdevice *s)
{
<<<<<<< HEAD
	struct comedi_cmd *cmd = &s->async->cmd;
	unsigned long flags;
	int event = 0;

	spin_lock_irqsave(&subpriv->intr.spinlock, flags);
	subpriv->intr.active = 1;
=======
	struct pcmuio_private *devpriv = dev->private;
	struct comedi_cmd *cmd = &s->async->cmd;
	int asic = pcmuio_subdevice_to_asic(s);
	struct pcmuio_asic *chip = &devpriv->asics[asic];
	unsigned long flags;
	int event = 0;

	spin_lock_irqsave(&chip->spinlock, flags);
	chip->active = 1;
>>>>>>> refs/remotes/origin/master

	/* Set up end of acquisition. */
	switch (cmd->stop_src) {
	case TRIG_COUNT:
<<<<<<< HEAD
		subpriv->intr.continuous = 0;
		subpriv->intr.stop_count = cmd->stop_arg;
		break;
	default:
		/* TRIG_NONE */
		subpriv->intr.continuous = 1;
		subpriv->intr.stop_count = 0;
=======
		chip->continuous = 0;
		chip->stop_count = cmd->stop_arg;
		break;
	default:
		/* TRIG_NONE */
		chip->continuous = 1;
		chip->stop_count = 0;
>>>>>>> refs/remotes/origin/master
		break;
	}

	/* Set up start of acquisition. */
	switch (cmd->start_src) {
	case TRIG_INT:
		s->async->inttrig = pcmuio_inttrig_start_intr;
		break;
	default:
		/* TRIG_NOW */
		event = pcmuio_start_intr(dev, s);
		break;
	}
<<<<<<< HEAD
	spin_unlock_irqrestore(&subpriv->intr.spinlock, flags);

<<<<<<< HEAD
	if (event) {
		comedi_event(dev, s);
	}
=======
	if (event)
		comedi_event(dev, s);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	spin_unlock_irqrestore(&chip->spinlock, flags);

	if (event)
		comedi_event(dev, s);
>>>>>>> refs/remotes/origin/master

	return 0;
}

<<<<<<< HEAD
static int
pcmuio_cmdtest(struct comedi_device *dev, struct comedi_subdevice *s,
	       struct comedi_cmd *cmd)
{
	return comedi_pcm_cmdtest(dev, s, cmd);
}

/*
 * A convenient macro that defines init_module() and cleanup_module(),
 * as necessary.
 */
static int __init driver_init_module(void)
{
	return comedi_driver_register(&driver);
}

static void __exit driver_cleanup_module(void)
{
	comedi_driver_unregister(&driver);
}

module_init(driver_init_module);
module_exit(driver_cleanup_module);
=======
static int pcmuio_cmdtest(struct comedi_device *dev,
			  struct comedi_subdevice *s,
			  struct comedi_cmd *cmd)
{
	int err = 0;

	/* Step 1 : check if triggers are trivially valid */

	err |= cfc_check_trigger_src(&cmd->start_src, TRIG_NOW | TRIG_INT);
	err |= cfc_check_trigger_src(&cmd->scan_begin_src, TRIG_EXT);
	err |= cfc_check_trigger_src(&cmd->convert_src, TRIG_NOW);
	err |= cfc_check_trigger_src(&cmd->scan_end_src, TRIG_COUNT);
	err |= cfc_check_trigger_src(&cmd->stop_src, TRIG_COUNT | TRIG_NONE);

	if (err)
		return 1;

	/* Step 2a : make sure trigger sources are unique */

	err |= cfc_check_trigger_is_unique(cmd->start_src);
	err |= cfc_check_trigger_is_unique(cmd->stop_src);

	/* Step 2b : and mutually compatible */

	if (err)
		return 2;

	/* Step 3: check if arguments are trivially valid */

	err |= cfc_check_trigger_arg_is(&cmd->start_arg, 0);
	err |= cfc_check_trigger_arg_is(&cmd->scan_begin_arg, 0);
	err |= cfc_check_trigger_arg_is(&cmd->convert_arg, 0);
	err |= cfc_check_trigger_arg_is(&cmd->scan_end_arg, cmd->chanlist_len);

	switch (cmd->stop_src) {
	case TRIG_COUNT:
		/* any count allowed */
		break;
	case TRIG_NONE:
		err |= cfc_check_trigger_arg_is(&cmd->stop_arg, 0);
		break;
	default:
		break;
	}

	if (err)
		return 3;

	/* step 4: fix up any arguments */

	/* if (err) return 4; */

	return 0;
}

static int pcmuio_attach(struct comedi_device *dev, struct comedi_devconfig *it)
{
	const struct pcmuio_board *board = comedi_board(dev);
	struct comedi_subdevice *s;
	struct pcmuio_private *devpriv;
	int ret;
	int i;

	ret = comedi_request_region(dev, it->options[0],
				    board->num_asics * PCMUIO_ASIC_IOSIZE);
	if (ret)
		return ret;

	devpriv = comedi_alloc_devpriv(dev, sizeof(*devpriv));
	if (!devpriv)
		return -ENOMEM;

	for (i = 0; i < PCMUIO_MAX_ASICS; ++i) {
		struct pcmuio_asic *chip = &devpriv->asics[i];

		spin_lock_init(&chip->pagelock);
		spin_lock_init(&chip->spinlock);
	}

	pcmuio_reset(dev);

	if (it->options[1]) {
		/* request the irq for the 1st asic */
		ret = request_irq(it->options[1], pcmuio_interrupt, 0,
				  dev->board_name, dev);
		if (ret == 0)
			dev->irq = it->options[1];
	}

	if (board->num_asics == 2) {
		if (it->options[2] == dev->irq) {
			/* the same irq (or none) is used by both asics */
			devpriv->irq2 = it->options[2];
		} else if (it->options[2]) {
			/* request the irq for the 2nd asic */
			ret = request_irq(it->options[2], pcmuio_interrupt, 0,
					dev->board_name, dev);
			if (ret == 0)
				devpriv->irq2 = it->options[2];
		}
	}

	ret = comedi_alloc_subdevices(dev, board->num_asics * 2);
	if (ret)
		return ret;

	for (i = 0; i < dev->n_subdevices; ++i) {
		s = &dev->subdevices[i];
		s->type		= COMEDI_SUBD_DIO;
		s->subdev_flags	= SDF_READABLE | SDF_WRITABLE;
		s->n_chan	= 24;
		s->maxdata	= 1;
		s->range_table	= &range_digital;
		s->insn_bits	= pcmuio_dio_insn_bits;
		s->insn_config	= pcmuio_dio_insn_config;

		/* subdevices 0 and 2 can suppport interrupts */
		if ((i == 0 && dev->irq) || (i == 2 && devpriv->irq2)) {
			/* setup the interrupt subdevice */
			dev->read_subdev = s;
			s->subdev_flags	|= SDF_CMD_READ;
			s->len_chanlist	= s->n_chan;
			s->cancel	= pcmuio_cancel;
			s->do_cmd	= pcmuio_cmd;
			s->do_cmdtest	= pcmuio_cmdtest;
		}
	}

	return 0;
}

static void pcmuio_detach(struct comedi_device *dev)
{
	struct pcmuio_private *devpriv = dev->private;

	if (devpriv) {
		pcmuio_reset(dev);

		/* free the 2nd irq if used, the core will free the 1st one */
		if (devpriv->irq2 && devpriv->irq2 != dev->irq)
			free_irq(devpriv->irq2, dev);
	}
	comedi_legacy_detach(dev);
}

static struct comedi_driver pcmuio_driver = {
	.driver_name	= "pcmuio",
	.module		= THIS_MODULE,
	.attach		= pcmuio_attach,
	.detach		= pcmuio_detach,
	.board_name	= &pcmuio_boards[0].name,
	.offset		= sizeof(struct pcmuio_board),
	.num_names	= ARRAY_SIZE(pcmuio_boards),
};
module_comedi_driver(pcmuio_driver);
>>>>>>> refs/remotes/origin/master

MODULE_AUTHOR("Comedi http://www.comedi.org");
MODULE_DESCRIPTION("Comedi low-level driver");
MODULE_LICENSE("GPL");
